#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "sysrepo.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"
#include "DSC_inc.h"
#include "util_inc.h"
#include "operationd.h"
#include "oplkipc.h"
#include "operationd_queue.h"
#include "operationd_config_mfg.h"
#include "operationd_file.h"
#include "operationd_led.h"
#include "operationd_reset.h"

#include "operationd_upgd.h"
#include "operationd_alm.h"

#define OPERATIOND_PID_FILE   "/var/run/operation.pid"

static bool m_bExitOperationdFlag = false;


static void
__InterruptHandler(int signum)
{
    LOG_WRITE(EN_LOG_NOTICE, "++ recv signal %d, %s", signum, strsignal(signum));

    if (SIGPIPE == signum) {
        /* Check SIGPIPE source cause in main loop */
    } else if (SIGUSR1 == signum) {
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Operationd exit requested, exiting");
        m_bExitOperationdFlag = true;
    }
}

static void
__RegisterHalEvent()
{
    int rc = OPLK_OK;

    rc = UTIL_RegEventCb(DEF_OPERATION_EVENT_SOCKET_PATH, Slot_HalEventCb);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by UTIL_RegEventCb: %s", sr_strerror(rc));
    }

    return;
}

static char *
__GetType(int iType, int iSubType)
{
    char *pc = NULL;

    switch (iType) {
    case 0:             /* uiCardType */
        if (CARD_TYPE_OTA == iSubType) {
            pc = "amp";
        } else if (CARD_TYPE_P_OTA == iSubType) {
            pc = "amp-p";
        } else if (CARD_TYPE_OPS == iSubType) {
            pc = "ops";
        } else {
            pc = "UNKOWN";
        }
        break;
    case 4:             /* uiResetType */
        if (RST_MODE_POWER == iSubType) {
            pc = "RST_MODE_POWER";
        } else if (RST_MODE_WARM == iSubType) {
            pc = "RST_MODE_WARM";
        } else if (RST_MODE_COLD == iSubType) {
            pc = "RST_MODE_COLD";
        } else if (RST_MODE_WDT == iSubType) {
            pc = "RST_MODE_WDT";
        } else if (RST_MODE_BUTTON == iSubType) {
            pc = "RST_MODE_BUTTON";
        } else {
            pc = "UNKOWN";
        }
        break;

    default:
        pc = "UNKOWN";
        break;
    }


    return pc;
}

static void
__PrintHalSlotInfo(int iSlotId, CSlotInfo *pstSlotInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "=====slot id======  %d", iSlotId);
    LOG_WRITE(EN_LOG_NOTICE, "=====uiCardType======  %d:%s", pstSlotInfo->uiCardType, __GetType(0, pstSlotInfo->uiCardType));
    LOG_WRITE(EN_LOG_NOTICE, "=====uiSlotState======  %d", pstSlotInfo->uiSlotState);
    LOG_WRITE(EN_LOG_NOTICE, "=====uiPlugFlag======  %s", pstSlotInfo->uiPlugFlag == 0 ? "false" : "true");
    LOG_WRITE(EN_LOG_NOTICE, "=====uiResetType======  %s", __GetType(4, pstSlotInfo->uiResetType));

    LOG_WRITE(EN_LOG_NOTICE, "__PrintHalSlotInfo end.");

    return;
}

static int
__GetSlotInfo(CSlotInfo *pstInfo)
{
    CSlotInfo astSlotInfo[SLOT_MAX_NUM];
    uint32_t ulSlotNum = 0;
    int rc = OPLK_OK;
    int i = 0;

    rc = SLOT_GetSlotNum(&ulSlotNum);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot number error. errNum = %d", rc);
        goto cleanup;
    }

    memset(astSlotInfo, 0, sizeof(astSlotInfo));
    for (i = 0; i < ulSlotNum; i++) {
        rc = SLOT_GetInfo(i, &astSlotInfo[i]);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get slot[%d] info error. errNum = %d", i, rc);
            continue;
        }

        __PrintHalSlotInfo(i, &astSlotInfo[i]);
    }

    memcpy(pstInfo, astSlotInfo, sizeof(astSlotInfo));

cleanup:
    return ulSlotNum;
}

static void
__LoadOperDataToOOD()
{
    /** Do nothing for ip-interfaces container data.
    ** these already existed in operational db by sysrepo subscribe mechanism(running data automatically synchronizes to operational data).
    *  NOTE: due to __NetStateProviderCB, mac adresses are NOT need to push to operational datastore.
    **/

    /** 1. copy running datastore to operational datastore
    **  TODO: move org-openroadm-device config change subscribe from csm to operationd. put before __LoadOperDataToOOD adn remove these code
    **/
    DSC_LoadOperData(OOD_XPATH_PEND_SW "//*", OOD_XPATH_PEND_SW, __func__);
    DSC_LoadOperData(OOD_XPATH_LIST_SHELVES "//*", OOD_XPATH_LIST_SHELVES, __func__);
    DSC_LoadOperData(OOD_XPATH_LIST_CIRCUIT_PACKS "//*", OOD_XPATH_LIST_CIRCUIT_PACKS, __func__);

    /* 2. load internal db com-oplink-device to org-openroadm-device operational datastore */
    DSC_LoadOperData(COD_XPATH_PREFIX "/pending-sw//*", OOD_XPATH_PEND_SW, __func__);
    DSC_LoadOperData(COD_XPATH_LIST_SHELVES "//*", OOD_XPATH_LIST_SHELVES, __func__);
    DSC_LoadOperData(COD_XPATH_LIST_CIRCUIT_PACKS "//*", OOD_XPATH_LIST_CIRCUIT_PACKS, __func__);
    /* special handle for info/vendor and info/model */
    DSC_LoadOperDataOfSingleLeafNode(COD_XPATH_VENDOR, OOD_XPATH_VENDOR);
    DSC_LoadOperDataOfSingleLeafNode(COD_XPATH_MODEL, OOD_XPATH_MODEL);

    return;
}

static void
__LoadOperDataToOCP()
{

    /** copy running datastore to operational datastore
    **  TODO: move openconfig-platform config change subscribe from csm to operationd. put before __LoadOperDataToOOD adn remove these code
    **/
    DSC_LoadOperData(OCP_XPATH_COMPONENTS "//*", OCP_XPATH_COMPONENTS, __func__);

    /* load internal db com-oplink-platform to openconfig-platform operational datastore. */
    DSC_LoadOperData(COP_XPATH_COMPONENTS_I "//*", OCP_XPATH_COMPONENTS, __func__);

    return;
}

int
main(int argc, char *argv[])
{
    int rc = SR_ERR_OK;
    uint8_t ucResetType = RST_MODE_WDT;
    uint32_t ulSlotNum = SLOT_MAX_NUM;
    CSlotInfo astSlotInfo[SLOT_MAX_NUM];
    bool bNeedConfig = false;
    int32_t iPidFd = -1;

    sr_session_ctx_t *pstStartupSess = NULL;
    sr_session_ctx_t *pstOperSess = NULL;
    sr_session_ctx_t *pstRunningSess = NULL;

    /* kill or ctrl+c is received */
    signal(SIGINT, __InterruptHandler);
    signal(SIGKILL, __InterruptHandler);
    signal(SIGTERM, __InterruptHandler);
    signal(SIGUSR1, __InterruptHandler);
    signal(SIGPIPE, __InterruptHandler);

    UTIL_InitClientLog();

    LOG_WRITE(EN_LOG_NOTICE, "===========OPERATIOND SATRT");

    rc = DSC_StartDeamonSess("OPManager", CONN_DEF_TIMEOUT, &pstStartupSess, &pstRunningSess, &pstOperSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_connect: %s", sr_strerror(rc));
        goto cleanup;
    }

    __LoadOperDataToOOD();
    __LoadOperDataToOCP();

    /* register alarm */
    OPERATIOND_AlmSub(pstStartupSess, pstRunningSess);

    /* get slot number and info */
    memset(astSlotInfo, 0, sizeof(astSlotInfo));
    ulSlotNum = __GetSlotInfo(astSlotInfo);
    /* reset */
    Reset_Process(ulSlotNum, astSlotInfo, &bNeedConfig, &ucResetType);
    /* upgrade */
    Upgd_Process();
    /* process slot for mfg and conf */
    Slot_Process(ulSlotNum, astSlotInfo, bNeedConfig);
    /* create queue for operationd */
    OPERATIOND_StartQueueThread();

    Db_Init();

    /* register rpc and oper state */
    File_RegisterRpc(pstStartupSess);
    Led_RegisterRpc(pstStartupSess);
    Reset_RegisterRpc(pstStartupSess);
    Upgd_RegisterRpc(pstStartupSess);
    Upgd_OperDataSub(pstStartupSess);

    /* register hal event */
    __RegisterHalEvent();

    DSC_SwitchToRunningSess();

    LOG_WRITE(EN_LOG_NOTICE, "-----OPERATIOND is running");

    UTIL_CreatePidFile(OPERATIOND_PID_FILE, &iPidFd);
    UTIL_HeartbeatCfgEnalbe(1);

    while (!m_bExitOperationdFlag) {
        UTIL_SendHeartbeat();
        DEF_SLEEP_S(3);
    }

cleanup:

    OPERATIOND_AlmUnSub(pstStartupSess, pstRunningSess);

    DSC_UnSubscribe();
    DSC_StopSess();

    OPERATIOND_QueueExit();

    UTIL_CloseHeartBeat();
    UTIL_DelPidFile(OPERATIOND_PID_FILE, iPidFd);
    UTIL_CloseClientLog();

    exit((SR_ERR_OK == rc) ? EXIT_SUCCESS : EXIT_FAILURE);
}



