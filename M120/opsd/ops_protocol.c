#include "ops_common.h"
#include "ops_protocol.h"
#include "ops_callback.h"
#include "ops_queue.h"
#include "DevOpsCApi.h"


/************************************************************************************************/
/* Definition for server socket port                                                            */
/************************************************************************************************/
/* @ }                                                                                          */
#define APS_SERVER_PORT             (9001)
/* @ }                                                                                          */


/************************************************************************************************/
/* Definition for APS switch                                                                    */
/************************************************************************************************/
/* @ }                                                                                          */
#define APS_SW_POS_PRIMARY          (1)
#define APS_SW_POS_SECONDARY        (0)
#define APS_COMM_STABLE_INTERVAL    (5)
#define APS_COMM_CHANGE_INTERVAL    (3.3)
#define APS_SEND_REMOTE_COUNT       (3)
#define APS_RECV_FAIL_COUNT         (9)
/* @ }                                                                                          */


/************************************************************************************************/
/* Definition for OPS event information                                                         */
/************************************************************************************************/
/* @ }                                                                                          */
#define APS_EVENT_STATE_ACTIVE       (1)
#define APS_EVENT_STATE_INACTIVE     (0)
#define APS_EVENT_LOCAL_BYTES        (3)
/* @ }                                                                                          */

/************************************************************************************************/
/* Thread data for server and client                                                            */
/************************************************************************************************/
/* @ }                                                                                          */
static OPS_THREAD_ST m_stClientThread = {0, "OpsClient", false};
static OPS_THREAD_ST m_stServerThread = {0, "OpsServer", false};
/* @ }                                                                                          */

/************************************************************************************************/
/* Local OPS status data, client socket id                                                      */
/************************************************************************************************/
/* @ }                                                                                          */
static volatile uint8_t m_ucEventFlag = APS_EVENT_STATE_INACTIVE;
static COpsBidirectionStatus m_stLocalSta;
static OPS_REQUEST_UN m_stRemoteSta;
static uint8_t m_ucRevertSta = APS_PROT_TYPE_R_NO_REVERT;
static int m_iSwitchingType = SWITCHING_TYPE_UNI;
static int m_iClientSock = -1;
static struct sockaddr_in m_stRemoteAddr;
static pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
/* @ }                                                                                          */


static void
__OPS_UpdateRemoteCache(uint8_t ucState, uint8_t ucPosition)
{
    m_stRemoteSta.stApsPduFmt.status.ucApsState = ucState;
    m_stRemoteSta.stApsPduFmt.status.ucRequestedSignal =
        (APS_SW_POS_PRIMARY == ucPosition) ? APS_SIGNAL_NULL : APS_SIGNAL_NORMAL;
}

static void
__OPS_SetEventState(int iState)
{
    m_ucEventFlag = APS_EVENT_STATE_ACTIVE;

    pthread_mutex_lock(&m_mutex);

    /* Update local state.                                                                      */
    memcpy(&m_stLocalSta, &iState, APS_EVENT_LOCAL_BYTES);
    __OPS_UpdateRemoteCache(m_stLocalSta.ucLocalStatus, m_stLocalSta.ucLocalPosition);

    pthread_mutex_unlock(&m_mutex);

    LOG_WRITE(EN_LOG_NOTICE, "OPS change event, a_pos = %u, a_sta = %u; z_sta = %u, z_req = %u",
              m_stLocalSta.ucLocalPosition, m_stLocalSta.ucLocalStatus,
              m_stLocalSta.stRemoteStatus.ucApsState, m_stLocalSta.stRemoteStatus.ucRequestedSignal);
}

static void
__OPS_ResetEventState(void)
{
    m_ucEventFlag = APS_EVENT_STATE_INACTIVE;
}


static int
__OPS_QueryRemoteSockAddr(struct sockaddr_in *pSockAddr)
{
    REMOTE_NODE_INFO_ST stRemoteInfo;
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    if (NULL == pSockAddr) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_QueryRemoteSockAddr");
        return OPLK_ERR;
    }

    /* Query remote address information.                                                        */
    memset(&stRemoteInfo, 0, sizeof(REMOTE_NODE_INFO_ST));

    if (SR_ERR_OK != DSC_GetRemoteInfo(PROTECTION_LINE_YANG, &stRemoteInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetRemoteInfo");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetRemoteInfo, OSC IP: %s", stRemoteInfo.acRemoteOscIp);

    pthread_mutex_lock(&mutex);
    /* Fill socket information.                                                                 */
    pSockAddr->sin_family = AF_INET;
    pSockAddr->sin_port = htons(APS_SERVER_PORT);
    /* Converts ip address to binary data.                                                      */
    pSockAddr->sin_addr.s_addr = inet_addr(stRemoteInfo.acRemoteOscIp);
    pthread_mutex_unlock(&mutex);

    return OPLK_OK;
}

static int
__OPS_SendApsMessage(int iSockFd)
{
    struct sockaddr_in stRemoteAddr = m_stRemoteAddr;
    int rc = OPLK_ERR;

    pthread_mutex_lock(&m_mutex);

    /* sendto is non-block, not check return value                                              */
    rc = sendto(iSockFd, m_stRemoteSta.pchApsFrames, APS_PDU_BYTES, 0,
                (const struct sockaddr *)&stRemoteAddr, sizeof(struct sockaddr_in));

    pthread_mutex_unlock(&m_mutex);

    if (rc < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Send APS message to far-end failed, error: %s", strerror(rc));
        /* Update remote IP address to rule out IP change cause                                 */
        __OPS_QueryRemoteSockAddr(&m_stRemoteAddr);
    }
    return OPLK_OK;
}

static int
__OPS_SetSwitchingType(void)
{
    uint32_t uiDevId = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;
    OPS_CONFIG_ST stOpsCfg;
    char (*pacNames)[DEF_BUF_SIZE_64] = NULL;
    size_t nNamesCount = 0;

    if (SR_ERR_OK != OPS_GetNames(&pacNames, &nNamesCount)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetNames");
        return OPLK_ERR;
    }

    for (size_t index = 0; index < nNamesCount; index++) {
        /* Query OPS config                                                                     */
        memset(&stOpsCfg, 0, sizeof(stOpsCfg));
        if (SR_ERR_OK != DSC_GetOpsConfig(*(pacNames + index), &stOpsCfg, sizeof(stOpsCfg))) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsConfig");
            return OPLK_ERR;
        }
        m_iSwitchingType = stOpsCfg.enSwitchingType;

        if (SR_ERR_OK != OPS_GetOpsDevIdByKey(*(pacNames + index), &uiDevId, &uiPortId)) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey key: %s", *(pacNames + index));
            return OPLK_ERR;
        }

        if (OPLK_OK != OPS_SetSwitchingType(uiDevId, (uint8_t)m_iSwitchingType, uiPortId)) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetSwitchingType, key: %s", *(pacNames + index));
            return OPLK_ERR;
        }
    }

    return OPLK_OK;
}

static int
__OPS_QueryModuleState(COpsBidirectionStatus *pstSta)
{
    int rc = OPLK_ERR;
    uint32_t uiDevId = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;
    COpsBidirectionStatus stBidiSta;

    if (SR_ERR_OK != OPS_GetOpsDevIdByKey(OPS_CFG_KEY, &uiDevId, &uiPortId)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey key: %s", OPS_CFG_KEY);
        return rc;
    }

    /* Query OPS request status from local module                                               */
    memset(&stBidiSta, 0, sizeof(stBidiSta));
    rc = OPS_GetRequestStatus(uiDevId, &stBidiSta, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetRequestStatus, uiDevId : %d", uiDevId);
        return rc;
    }

    *pstSta = stBidiSta;

    return rc;
}

static int
__OPS_QueryRevertState(uint8_t *pucSta)
{
    OPS_CONFIG_ST stOpsCfg;

    if (NULL == pucSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_QueryRevertState");
        return OPLK_ERR;
    }

    /* Query OPS config                                                                         */
    memset(&stOpsCfg, 0, sizeof(stOpsCfg));
    if (SR_ERR_OK != DSC_GetOpsConfig(OPS_CFG_KEY, &stOpsCfg, sizeof(stOpsCfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsConfig");
        return OPLK_ERR;
    }

    *pucSta = (uint8_t)(stOpsCfg.bRevertive ? APS_PROT_TYPE_R_REVERT : APS_PROT_TYPE_R_NO_REVERT);

    return OPLK_OK;
}

static int
__OPS_SetRemoteState(OPS_REQUEST_UN *pstRemoteSta)
{
    uint32_t uiDevId = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;
    int rc = OPLK_ERR;
    COpsReqStatus *status = NULL;

    if (NULL == pstRemoteSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_SetRemoteState");
        return rc;
    }

    status = &pstRemoteSta->stApsPduFmt.status;

    LOG_WRITE(EN_LOG_NOTICE, "__OPS_SetRemoteState, R: %u, D: %u, B: %u, A: %u, sta: %u, req: %u, bri: %u",
              status->ucProtTypeR, status->ucProtTypeD, status->ucProtTypeB, status->ucProtTypeA,
              status->ucApsState, status->ucRequestedSignal, status->ucBridgedSignal);

    if (SR_ERR_OK != OPS_GetOpsDevIdByKey(OPS_CFG_KEY, &uiDevId, &uiPortId)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey, key: %s", OPS_CFG_KEY);
        return rc;
    }

    rc = OPS_SetRemoteStatus(uiDevId, status, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetRemoteStatus, uiDevId : %d", uiDevId);
        return rc;
    }

    /* Update remote state.                                                                     */
    pthread_mutex_lock(&m_mutex);
    memcpy(&m_stLocalSta.stRemoteStatus, status, sizeof(COpsReqStatus));
    pthread_mutex_unlock(&m_mutex);

    /* Push the remote state to background processing */
    OPS_StateAdd(status->ucApsState);

    LOG_WRITE(EN_LOG_DEBUG, "__OPS_SetRemoteState --");

    return rc;
}

static void
__OPS_InitState(OPS_REQUEST_UN *pstSta)
{
    if (NULL == pstSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_InitState");
        return;
    }

    pstSta->stApsPduFmt.opcode = APS_PDU_OPCODE;
    pstSta->stApsPduFmt.tlv_offset = APS_PDU_TLV_OFFSET;
    pstSta->stApsPduFmt.status.ucApsState = APS_STATE_NR;
    pstSta->stApsPduFmt.status.ucProtTypeA = APS_PROT_TYPE_A_CHANEL;
    pstSta->stApsPduFmt.status.ucProtTypeB = APS_PROT_TYPE_B_1P1;
    pstSta->stApsPduFmt.status.ucProtTypeD = APS_PROT_TYPE_D_BIDIR;
    pstSta->stApsPduFmt.status.ucProtTypeR = m_ucRevertSta;
    pstSta->stApsPduFmt.status.ucRequestedSignal = APS_SIGNAL_NULL;
    pstSta->stApsPduFmt.status.ucBridgedSignal = APS_SIGNAL_NORMAL;
}

static int
__OPS_UpdateLocalState(void)
{
    int rc = OPLK_ERR;
    COpsBidirectionStatus status;

    rc = __OPS_QueryModuleState(&status);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __OPS_UpdateLocalState");
        return rc;
    }

    pthread_mutex_lock(&m_mutex);

    memcpy(&m_stLocalSta, &status, sizeof(status));
    __OPS_UpdateRemoteCache(m_stLocalSta.ucLocalStatus, m_stLocalSta.ucLocalPosition);

    pthread_mutex_unlock(&m_mutex);

    return rc;
}

/***FUNC+****************************************************************************************/
/* Name   : __OPS_CheckState                                                                    */
/* Descrp : Check the far-end requested status with local                                       */
/* Input  : pstSta  -- A pointer of far-end requested status                                    */
/* Output : None                                                                                */
/* Return : The far-end status is changed or not                                                */
/*            This parameter can be one of the following values:                                */
/*              @arg true  : The far-end status is different with local                         */
/*              @arg false : The far-end status is the same with local                          */
/***FUNC-****************************************************************************************/
static bool
__OPS_CheckState(OPS_REQUEST_UN *pstSta)
{
    int iCmp = 0;

    if (NULL == pstSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_CheckState");
        return false;
    }


    /* 1. Check the frame is valid or not                                                       */
    if (APS_PDU_OPCODE != pstSta->stApsPduFmt.opcode || \
        APS_PDU_TLV_OFFSET != pstSta->stApsPduFmt.tlv_offset || \
        APS_PROT_TYPE_A_CHANEL != pstSta->stApsPduFmt.status.ucProtTypeA || \
        APS_PROT_TYPE_D_BIDIR != pstSta->stApsPduFmt.status.ucProtTypeD || \
        pstSta->stApsPduFmt.status.ucRequestedSignal > APS_SIGNAL_NORMAL) {

        return false;
    }

    /* 2. Compare the requested status with the near end                                        */
    pthread_mutex_lock(&m_mutex);
    iCmp = memcmp(&m_stLocalSta.stRemoteStatus, &pstSta->stApsPduFmt.status, sizeof(COpsReqStatus));
    pthread_mutex_unlock(&m_mutex);

    /* 3. different: true, same : false                                                         */
    return 0 != iCmp;
}

static void
__OPS_UnidirectState(OPS_REQUEST_UN *pstSta)
{
    if (NULL == pstSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_UnidirectState");
        return;
    }

    __OPS_InitState(pstSta);
}

static void *
__OPS_ClientThread(void *pvArg)
{
    m_iClientSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_iClientSock < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Create OPS client socket failed.");
        return NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "OPS client thread start.");

    OPS_SetRunningThreadName(m_stClientThread.pcName);

    while (!m_stClientThread.bExitLoop) {

        /* 1. When the APS state machine is stable, the APS logic sends an APS packet every 5s. */
        DEF_SLEEP_S(APS_COMM_STABLE_INTERVAL);

        /* 2. Send local APS state to far-end.                                                 */
        if (APS_EVENT_STATE_ACTIVE == m_ucEventFlag) {
            continue;
        }

        __OPS_SendApsMessage(m_iClientSock);
    }

    LOG_WRITE(EN_LOG_NOTICE, "OPS client thread exit.");
    DEF_SAFE_CLOSE(m_iClientSock);
    return NULL;
}

static int
__OPS_RecvOpsMsg(int iSockFd, OPS_REQUEST_UN *pstRemoteSta)
{
    int rc = OPLK_ERR;
    int iMsgLen = 0;
    struct sockaddr_in stRemoteAddr = m_stRemoteAddr;
    socklen_t nSockLen = sizeof(stRemoteAddr);

    if (NULL == pstRemoteSta) {
        LOG_WRITE(EN_LOG_ERROR, "Input null at __OPS_RecvOpsMsg");
        return rc;
    }

    /* 1. Receive data from the remote address                                                  */
    iMsgLen = recvfrom(iSockFd, pstRemoteSta->pchApsFrames, APS_PDU_BYTES, 0,
                       (struct sockaddr *)&stRemoteAddr, &nSockLen);
    if (APS_PDU_BYTES != iMsgLen) {
        /* Here time out error would be happened.                                               */
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_DEBUG, "__OPS_RecvOpsMsg received msg.");

    /* 2 The local state is the same as remote, nothing to do.                                  */
    if (!__OPS_CheckState(pstRemoteSta)) {
        return OPLK_OK;
    }

    /* 3. Update the local state when the far-end state is changed                              */
    __OPS_SetRemoteState(pstRemoteSta);

    return OPLK_OK;
}

static void *
__OPS_ServerThread(void *pvArg)
{
    int iServerSock = -1;
    OPS_REQUEST_UN stRemoteSta;
    static int m_iRecvErrCnt = 0;

    /* Creat UDP socket for server                                                              */
    iServerSock = OPS_CreateUDPServer(APS_SERVER_PORT);
    if (iServerSock < 0) {
        LOG_WRITE(EN_LOG_ERROR, "create OPS server socket failed.");
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "OPS server thread start.");

    /* Set 'recvfrom' call timeout (2s).                                                        */
    OPS_SetRecvfromNonblocking(iServerSock);

    OPS_SetRunningThreadName(m_stServerThread.pcName);

    while (false == m_stServerThread.bExitLoop) {

        if (OPLK_OK != __OPS_RecvOpsMsg(iServerSock, &stRemoteSta)) {
            /* receive data timeout or an error occurred                                        */
            if (++m_iRecvErrCnt > APS_RECV_FAIL_COUNT) {
                m_iRecvErrCnt = 0;
                LOG_WRITE(EN_LOG_ERROR, "Receive message from far-end time out.");
                /* Set the failure of protocol defects alarm                                    */
                OPS_OpsAlmProtoFail(true);
                /* Set the OPS module to remote NR(0,1)                                         */
                __OPS_UnidirectState(&stRemoteSta);
                __OPS_SetRemoteState(&stRemoteSta);
                /* Update remote IP address to rule out IP change cause                         */
                __OPS_QueryRemoteSockAddr(&m_stRemoteAddr);

            }
        } else {
            m_iRecvErrCnt = 0;
            /* Clear the failure of protocol defects alarm                                      */
            OPS_OpsAlmProtoFail(false);
        }
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "OPS server thread exit.");
    DEF_SAFE_CLOSE(iServerSock);
    return NULL;
}

int
OPS_Client_StartThreads()
{
    if (SWITCHING_TYPE_UNI == m_iSwitchingType) {
        return OPLK_OK;
    }

    if (0 != pthread_create(&m_stClientThread.tId, NULL, __OPS_ClientThread, NULL)) {
        LOG_WRITE(EN_LOG_ERROR, "create OPS client thread failed.");
        return OPLK_ERR;
    }

    return OPLK_OK;
}

void
OPS_Client_StopThreads()
{
    if (SWITCHING_TYPE_UNI == m_iSwitchingType) {
        return;
    }

    m_stClientThread.bExitLoop = true;

    /* Waiting for the thread to exit normally                                                  */
    if (m_stClientThread.tId > 0) {
        pthread_join(m_stClientThread.tId, NULL);
    }
}

int
OPS_Server_StartThreads()
{
    int rc = OPLK_ERR;

    /* 1. Set switching type first for local module                                             */
    rc = __OPS_SetSwitchingType();

    /* Ignore error because the device default type is unidirectional.                          */
    if (SWITCHING_TYPE_UNI == m_iSwitchingType) {
        return OPLK_OK;
    }

    /* The device type must be configurated for bidirectional type.                             */
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __OPS_SetSwitchingType.");
        return rc;
    }

    /* 2. Init queue service                                                                    */
    rc = OPS_QueueStart();
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_QueueStart");
        return rc;
    }

    /* 3. Init the near-end information first for server                                        */
    __OPS_InitState(&m_stRemoteSta);

    rc = __OPS_QueryRevertState(&m_ucRevertSta);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __OPS_QueryRevertState");
        return rc;
    }

    rc = __OPS_QueryRemoteSockAddr(&m_stRemoteAddr);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __OPS_QueryRemoteSockAddr");
        return rc;
    }

    rc = __OPS_UpdateLocalState();
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __OPS_UpdateLocalState");
        return rc;
    }

    /* 4. Start thread for server                                                               */
    if (0 != pthread_create(&m_stServerThread.tId, NULL, __OPS_ServerThread, NULL)) {
        LOG_WRITE(EN_LOG_ERROR, "create OPS server thread failed.");
        return OPLK_ERR;
    }

    return rc;
}

void
OPS_Server_StopThreads(void)
{
    if (SWITCHING_TYPE_UNI == m_iSwitchingType) {
        return;
    }

    OPS_QueueStop();

    m_stServerThread.bExitLoop = true;

    /* Waiting for the thread to exit normally                                                  */
    if (m_stServerThread.tId > 0) {
        pthread_join(m_stServerThread.tId, NULL);
    }
}

void
OPS_SendAPSRapidMessage(int iSubEventId)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    if (SWITCHING_TYPE_UNI == m_iSwitchingType) {
        return;
    }

    pthread_mutex_lock(&mutex);

    __OPS_SetEventState(iSubEventId);

    /* Send 3 times for 3.3ms interval                                                          */
    for (int i = 0; i < APS_SEND_REMOTE_COUNT; i++) {
        __OPS_SendApsMessage(m_iClientSock);
        DEF_SLEEP_MS(APS_COMM_CHANGE_INTERVAL);
    }

    __OPS_ResetEventState();

    pthread_mutex_unlock(&mutex);
}