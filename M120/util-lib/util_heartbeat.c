/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_heartbeat.c
* Description:   heart beat
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                dongyangf  2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_heartbeat.h"

static ST_HEART_BEAT m_stHeartBeat = {
    -1,
    "",
    ""
};

static int
__CreateFd()
{
    int iFd = -1;

    memset(&m_stHeartBeat, 0, sizeof(m_stHeartBeat));
    if (OPLK_ERR == UTIL_GetProcName(m_stHeartBeat.acProcName, sizeof(m_stHeartBeat.acProcName) - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_GetProcName failure");
        return OPLK_ERR;
    }

    sprintf(m_stHeartBeat.acSoketPath, HEARTBEAT_SEND_SOCKET_PATH, m_stHeartBeat.acProcName);

    iFd = UTIL_CreateMsgSock(m_stHeartBeat.acSoketPath);
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateMsgSock failure %d", iFd);
        return OPLK_ERR;
    }

    m_stHeartBeat.iFd = iFd;

    return iFd;
}

int
UTIL_SendHeartbeat()
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    strcpy(stCfg.acProcName, m_stHeartBeat.acProcName);

    lLen = UTIL_CreateOopMsg(acBuffer, DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_HEART_BEAT, 0, (char *)&stCfg, sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        /* LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen); */
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        /* LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", pcDestPath); */
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_HeartbeatCfgEnalbe(int iEnable)
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    strcpy(stCfg.acProcName, m_stHeartBeat.acProcName);
    stCfg.iCmdId = DEF_MON_CFG_PROC_VALID;
    stCfg.iValue = iEnable;

    lLen = UTIL_CreateOopMsg(acBuffer,
                             DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_CFG, 0, (char *)&stCfg,
                             sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", MONITOR_RECV_SOCKET_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


int
UTIL_HeartbeatCfgEnalbeByName(int iEnable, char *pcProcName)
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (NULL == pcProcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    strncpy(stCfg.acProcName, pcProcName, sizeof(stCfg.acProcName) - 1);
    stCfg.iCmdId = DEF_MON_CFG_PROC_VALID;
    stCfg.iValue = iEnable;

    lLen = UTIL_CreateOopMsg(acBuffer,
                             DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_CFG, 0, (char *)&stCfg,
                             sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", MONITOR_RECV_SOCKET_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


int
UTIL_HeartbeatCfgChkInterval(int iInterval)
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    strcpy(stCfg.acProcName, m_stHeartBeat.acProcName);
    stCfg.iCmdId = DEF_MON_CFG_CHK_INTERVAL;
    stCfg.iValue = iInterval;

    lLen = UTIL_CreateOopMsg(acBuffer,
                             DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_CFG, 0, (char *)&stCfg,
                             sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", MONITOR_RECV_SOCKET_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_HeartbeatCfgChkTimeout(int iTimeout)
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    strcpy(stCfg.acProcName, m_stHeartBeat.acProcName);
    stCfg.iCmdId = DEF_MON_CFG_CHK_TIMEOUT;
    stCfg.iValue = iTimeout;

    lLen = UTIL_CreateOopMsg(acBuffer,
                             DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_CFG, 0, (char *)&stCfg,
                             sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", MONITOR_RECV_SOCKET_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_HeartbeatCfgResetProcess(char *pcProName)
{
    int iRet = OPLK_ERR;
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    ST_MON_CFG_INFO stCfg;

    if (m_stHeartBeat.iFd < 0) {
        __CreateFd();
    }

    memset(&stCfg, 0, sizeof(stCfg));
    memcpy(stCfg.acProcName, m_stHeartBeat.acProcName, sizeof(stCfg.acProcName) - 1);
    stCfg.iCmdId = DEF_MON_CFG_RESET_PROCESS;
    strcpy(stCfg.auBufExt, pcProName);

    lLen = UTIL_CreateOopMsg(acBuffer,
                             DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_MON_CFG, 0, (char *)&stCfg,
                             sizeof(ST_MON_CFG_INFO));
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(m_stHeartBeat.iFd, acBuffer, lLen, MONITOR_RECV_SOCKET_PATH);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "send heartbeat to %s failure", MONITOR_RECV_SOCKET_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_CloseHeartBeat()
{
    DEF_SAFE_CLOSE(m_stHeartBeat.iFd);
    return OPLK_OK;
}

static ST_THREAD_HEART_BEAT m_astThreadHeartBeat[THREAD_HEART_BEAT_MAX_CNT] =
{
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
};

/* Called by thread init for monitor */
int
UTIL_InitThreadHeartBeart()
{
    int i = 0;
    int iId = 0;
    int iDefThread = sizeof(m_astThreadHeartBeat) / sizeof(ST_THREAD_HEART_BEAT);

    for (i = 0; i < iDefThread; i++) {
        if (!m_astThreadHeartBeat[i].bIsUsed) {
            iId = i;
            m_astThreadHeartBeat[i].bIsUsed = true;
            m_astThreadHeartBeat[i].ullCount = 0;
            m_astThreadHeartBeat[i].iTimeoutCnt = 0;
            break;
        }
    }

    return iId;
}

/* Called by thread loop for monitor */
int
UTIL_IncreaseThreadHeartBeat(int iId)
{
    int iDefThread = sizeof(m_astThreadHeartBeat) / sizeof(ST_THREAD_HEART_BEAT);

    if ((iId < 0) || (iId >= iDefThread)) {
        LOG_WRITE(EN_LOG_ERROR, "iId error %d", iId);
        return OPLK_ERR;
    }

    m_astThreadHeartBeat[iId].ullCount++;
    return OPLK_OK;
}

/* Default is enabled.
 * When long time something execute then can disabled heart beat. for example upgrade.
 */
int
UTIL_EnableThreadHeartBeat(int iId, bool bEnable)
{
    int iDefThread = sizeof(m_astThreadHeartBeat) / sizeof(ST_THREAD_HEART_BEAT);

    if ((iId < 0) || (iId >= iDefThread)) {
        LOG_WRITE(EN_LOG_ERROR, "iId error %d", iId);
        return OPLK_ERR;
    }

    m_astThreadHeartBeat[iId].bIsUsed = bEnable;
    return OPLK_OK;
}


/* called by main thread */
bool
UTIL_CheckThreadHeartBeart()
{
    bool bThread = true;

    int i = 0;
    int iDefThread = sizeof(m_astThreadHeartBeat) / sizeof(ST_THREAD_HEART_BEAT);

    static ST_THREAD_HEART_BEAT astLastThreadHeartBeat[THREAD_HEART_BEAT_MAX_CNT] =
    {
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
        {false, 0, 0},
    };

    for (i = 0; i < iDefThread; i++) {
        if (m_astThreadHeartBeat[i].bIsUsed) {
            if (m_astThreadHeartBeat[i].iTimeoutCnt >= THREAD_HEART_BEAT_TIMEOUT) {
                bThread = false;
                LOG_WRITE(EN_LOG_ERROR, "iId = %d heart beat failed, iTimeoutCnt = %d", i, m_astThreadHeartBeat[i].iTimeoutCnt);
                break;
            }

            if (astLastThreadHeartBeat[i].ullCount == m_astThreadHeartBeat[i].ullCount) {
                m_astThreadHeartBeat[i].iTimeoutCnt++;
                /* LOG_WRITE(EN_LOG_WARNING, "iId = %d iTimeoutCnt++, iTimeoutCnt = %d", i, m_astThreadHeartBeat[i].iTimeoutCnt); */
            } else {
                m_astThreadHeartBeat[i].iTimeoutCnt = 0;
            }
        }
    }

    memcpy(&astLastThreadHeartBeat, &m_astThreadHeartBeat, sizeof(astLastThreadHeartBeat));
    return bThread;
}

