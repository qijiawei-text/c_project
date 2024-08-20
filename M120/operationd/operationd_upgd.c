
/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_upgd.c
   Description : upgrade
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "sysrepo.h"
#include "util_inc.h"
#include "DSC_inc.h"
#include "oplkupgd.h"
#include "oplkipc.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"

#include "operationd_config_mfg.h"
#include "operationd_queue.h"
#include "operationd_alm.h"
#include "operationd_upgd.h"



static pthread_t m_astAutoUpgdThread[SLOT_MAX_NUM];



static int __UpgradeStateProviderCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath_in,
                                    const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_data);
static int __RpcSwStageCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                          sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcSwActivateCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                             sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcSwCommitCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                           sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);



static ST_PROVIDER_SUB m_astUpgdProvider[] = {
    {MODULE_OOD, __UpgradeStateProviderCb, DEVICE_XPATH_UPG_STATE, MODULE_COD}
};

static ST_RPC_SUB m_astUpgdRpcSub[] = {
    {MODULE_NS_OOSWDL, __RpcSwStageCb, RPC_XPATH_SW_STAGE},
    {MODULE_NS_OOSWDL, __RpcSwActivateCb, RPC_XPATH_SW_ACTIVATE},
    {MODULE_NS_OOSWDL, __RpcSwCommitCb, RPC_XPATH_SW_CANCEL},
};

static int
__SendSwStageNotice(int32_t iFlag)
{
#define WR_CNT 1

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

#define XPATH_SW_STAGE_NOTICE  "/org-openroadm-swdl:sw-stage-notification"
#define XPATH_SW_STAGE_STATUS XPATH_SW_STAGE_NOTICE "/status"

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = XPATH_SW_STAGE_STATUS;
    astValues[0].type = SR_ENUM_T;
    if (OPLK_OK == iFlag) {
        astValues[0].data.string_val = "Successful";
    } else {
        astValues[0].data.string_val = "Failed";
    }

    rc = sr_notif_send(pstSess,
                       XPATH_SW_STAGE_NOTICE,
                       astValues,
                       WR_CNT, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}

static int
__RpcSwStageCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
               sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    ST_UPGD_INFO stUpgdInfo;
    ST_UTIL_QUEUE_MSG stQueueMsg;

    LOG_WRITE(EN_LOG_NOTICE, "Executing sw upgrade: %s", input[0].data.string_val);

    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);

    if (OPLK_ERR == (rc = access(input[0].data.string_val, F_OK))) {
        LOG_WRITE(EN_LOG_ERROR, "Bundle file %s is not exist", input[0].data.string_val);
        goto cleanup;
    }

    if (EN_SYS_NORMAL == stUpgdInfo.ucSysState || EN_SYS_TRANSFER_COMPLETE == stUpgdInfo.ucSysState) {    /*check upgd is in running*/
        memset(&stQueueMsg, 0, sizeof(stQueueMsg));
        stQueueMsg.ulMsgOid = OID_START_UPGD;
        strcpy(stQueueMsg.acMsgBuf, input[0].data.string_val);
        stQueueMsg.ulMsgLen = strlen(input[0].data.string_val) + 1;
        if (0 != UTIL_AddQueueNode(OPERATIOND_GetQueueIdx(), (char *)&stQueueMsg, sizeof(stQueueMsg))) {
            LOG_WRITE(EN_LOG_ERROR, "add node to queue failed");
            rc = OPLK_ERR;
            goto cleanup;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "upgd is running!");
        rc = OPLK_ERR;
        goto cleanup;
    }

    DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_UPGRADE_SOFTWARE, "");

cleanup:
    if (OPLK_OK == rc) {
        DSC_RpcCreateOutput(RPC_XPATH_SW_STAGE, RPC_STATUS_SUCCESS, "System sw stage sw started", output, output_cnt);
        __SendSwStageNotice(OPLK_OK);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_SW_STAGE, RPC_STATUS_FAILED, "System sw stage sw error", output, output_cnt);
        __SendSwStageNotice(OPLK_ERR);
    }

    return SR_ERR_OK;
}

static int
__SendSwActiveNotice(int32_t iFlag)
{
    /* !!!!!!!! not test !!!!!! */
#define WR_CNT 1

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

#define XPATH_SW_ACTIVE_NOTICE  "/org-openroadm-swdl:sw-activate-notification"
#define XPATH_SW_ACTIVE_STATUS XPATH_SW_ACTIVE_NOTICE "/status"

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = XPATH_SW_ACTIVE_STATUS;
    astValues[0].type = SR_ENUM_T;
    if (OPLK_OK == iFlag) {
        astValues[0].data.string_val = "Successful";
    } else {
        astValues[0].data.string_val = "Failed";
    }

    rc = sr_notif_send(pstSess,
                       XPATH_SW_ACTIVE_NOTICE,
                       astValues,
                       WR_CNT, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}


static int
__RpcSwActivateCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                  sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, "Executing sw activate: %s", input[0].data.string_val);

    rc = UPGD_Switch();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Switch error");
        goto cleanup;
    }

    /* TODO: warm or cold, need get hitless flag from upgrade */
    rc = IPC_SendResetRpc((sr_session_ctx_t *)private_ctx, RST_CHASSIS, RST_MODE_WARM);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Reset chassis error %d, Type %d", rc, RST_MODE_WARM);
        goto cleanup;
    }

cleanup:
    if (OPLK_OK == rc) {
        DSC_RpcCreateOutput(RPC_XPATH_SW_ACTIVATE, RPC_STATUS_SUCCESS, "System active sw started", output, output_cnt);
        __SendSwActiveNotice(OPLK_OK);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_SW_ACTIVATE, RPC_STATUS_FAILED, "System active sw error", output, output_cnt);
        __SendSwActiveNotice(OPLK_ERR);
    }

    return SR_ERR_OK;
}

static int
__RpcSwCommitCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, "Executing sw cancel valication timer: %s", (input[0].data.bool_val ? "true" : "false"));

    /* NOT support, TODO */
#if 0
    rc = UPGD_Commit();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Commit error");
    }
#endif

    if (OPLK_OK == rc) {
        DSC_RpcCreateOutput(RPC_XPATH_SW_CANCEL, RPC_STATUS_SUCCESS, "System cancel sw valication timer ok", output, output_cnt);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_SW_CANCEL, RPC_STATUS_FAILED, "System cancel sw valication timer error", output, output_cnt);
    }

    return SR_ERR_OK;
}

static int
__UpgdGetTransferPercent()
{
#define FW_LENGTH (60583968)
    char acFileName[128] = {0};
    char acFullPath[256] = {0};
    int iFileLength = 0;
    int32_t iPercent = 0;
    int rc = OPLK_OK;

    memset(acFileName, 0, sizeof(acFileName));
    rc = UTIL_GetFileName(UPGD_IMAGE_TRANSFERRING_DIRECTORY, acFileName, sizeof(acFileName) - 1);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "File not exist!");
        goto cleanup;
    }

    memset(acFullPath, 0, sizeof(acFullPath));
    snprintf(acFullPath, sizeof(acFullPath), "%s/%s", UPGD_IMAGE_TRANSFERRING_DIRECTORY, acFileName);
    iFileLength = UTIL_GetFileLen(acFullPath);

    if (OPLK_ERR == iFileLength) {
        LOG_WRITE(EN_LOG_ERROR, "%s may be not exist.", acFullPath);
        goto cleanup;
    }

    iPercent = (int32_t)(((uint64_t)iFileLength * 100) / FW_LENGTH);
    if (iPercent >= 100) {
        iPercent = 99;
    }

    return iPercent;
cleanup:
    return OPLK_ERR;
}

static int
__UpgradeStateProviderCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath_in,
                         const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_data)
{
    sr_val_t *pstValues = NULL;
    int rc = SR_ERR_OK;
    ST_UPGD_INFO stUpgdInfo;
    char *pcUpgdStepStr = NULL;
    char *pcUpgdSysStatusStr = NULL;
    int32_t iPercent = 0;

    if (NULL == xpath_in) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /* /org-openroadm-device:org-openroadm-device/pending-sw/com-oplink-swdl-ext:sw-upgrade-state/upgrade-state */
    /* /org-openroadm-device:org-openroadm-device/pending-sw/com-oplink-swdl-ext:sw-upgrade-state/sys-status    */
    /* /org-openroadm-device:org-openroadm-device/pending-sw/com-oplink-swdl-ext:sw-upgrade-state/upgrade-step  */
    /* /org-openroadm-device:org-openroadm-device/pending-sw/com-oplink-swdl-ext:sw-upgrade-state/hitless       */
    /* /org-openroadm-device:org-openroadm-device/pending-sw/com-oplink-swdl-ext:sw-upgrade-state/percent       */
    /* There is a problem: why NOT report upgrade-state? TBD                                                    */
    rc = sr_new_values(DEVICE_XPATH_UPG_STATE_NUM, &pstValues);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error: %s", sr_strerror(rc));
        goto cleanup;
    }

    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    rc = UPGD_GetInfo(&stUpgdInfo);
    if (OPLK_OK != rc) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "UPGD_GetInfo error");
        goto cleanup;
    }

    if (EN_SYS_TRANSFERRING == stUpgdInfo.ucSysState || EN_SYS_TRANSFER_FAILED == stUpgdInfo.ucSysState) {
        iPercent = __UpgdGetTransferPercent();
        if (OPLK_ERR == iPercent) {
            stUpgdInfo.ucUpgdPerc = 0;
        } else {
            stUpgdInfo.ucUpgdPerc = (uint8_t)iPercent;
        }
    }

    pcUpgdSysStatusStr = UPGD_GetSysStatusStr(stUpgdInfo.ucSysState);
    pcUpgdStepStr = UPGD_GetStepStr(stUpgdInfo.ucUpgdStep);
    if (NULL == pcUpgdStepStr || NULL == pcUpgdSysStatusStr) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "error upgd info invalid!");
        goto cleanup;
    }

    sr_val_set_xpath(&pstValues[0], DEVICE_XPATH_UPG_STATE_SYS_STATUS);
    pstValues[0].type = SR_ENUM_T;
    sr_val_set_str_data(&pstValues[0], SR_ENUM_T, pcUpgdSysStatusStr);

    sr_val_set_xpath(&pstValues[1], DEVICE_XPATH_UPG_STATE_STEP);
    pstValues[1].type = SR_ENUM_T;
    sr_val_set_str_data(&pstValues[1], SR_ENUM_T, pcUpgdStepStr);

    sr_val_set_xpath(&pstValues[2], DEVICE_XPATH_UPG_STATE_PERCENT);
    pstValues[2].type = SR_UINT8_T;
    pstValues[2].data.uint8_val = stUpgdInfo.ucUpgdPerc;

    sr_val_set_xpath(&pstValues[3], DEVICE_XPATH_UPG_STATE_HITLESS);
    pstValues[3].type = SR_BOOL_T;
    pstValues[3].data.bool_val = stUpgdInfo.bHitless;

    /*Convert sr_values to libyang tree */
    rc = DSC_SrValues2LydNode(session, xpath_in, pstValues, DEVICE_XPATH_UPG_STATE_NUM, parent);

cleanup:
    if (pstValues != NULL) {
        sr_free_values(pstValues, DEVICE_XPATH_UPG_STATE_NUM);
        pstValues = NULL;
    }
    return rc;
}

static void *
__AutoUpgdProcessFunc(void *pvArg)
{

#if DEF_FEATURE_AUTO_UPGRADE_ENABLE
    CBoardStatusData stBoardStaData;
    CSlotInfo stSlotInfo;
    int rc = OPLK_OK;
    AUTO_UPGD_SLOT_INFO_ST stAutoUpgdInfo = {0};
    bool bGetSysLedStatus = false;


    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pvArg) {
        LOG_WRITE(EN_LOG_ERROR, "input para null!");
        return NULL;
    }

    memset(&stAutoUpgdInfo, 0, sizeof(stAutoUpgdInfo));
    memcpy(&stAutoUpgdInfo, (AUTO_UPGD_SLOT_INFO_ST *)pvArg, sizeof(AUTO_UPGD_SLOT_INFO_ST));
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId: %d, bNeedConfig: %d", __func__, stAutoUpgdInfo.iSlotId, stAutoUpgdInfo.bNeedConfig);

    /* 1. light SYS led when upgrading */
    memset(&stBoardStaData, 0, sizeof(stBoardStaData));
    rc = BOARD_GetStatusData(&stBoardStaData);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "BOARD_GetStatusData failed!");
    } else {
        bGetSysLedStatus = true;
        rc = BOARD_SetLedState(LED0, DSC_LED_STATE_GREEN_BLINK);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Init system led green error");
        }
    }

    /* 2. auto upgrade card */
    rc = UPGD_AutoUpgradeSlot(stAutoUpgdInfo.iSlotId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error UPGD_AutoUpgradeSlot iSlotId = %d", stAutoUpgdInfo.iSlotId);
        OPERATIOND_SetUpgdAlm(UPGRADE_ALM_FAILED, true);
        goto cleanup;
    }

    /* 3. get slot info */
    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(stAutoUpgdInfo.iSlotId, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error SLOT_GetInfo iSlotId = %d", stAutoUpgdInfo.iSlotId);
        goto cleanup;
    }

    /* 4.update mfg */
    Slot_PutPlugableDeviceMfgToOrgAndOc(stAutoUpgdInfo.iSlotId, SLOT_STATE_ONLINE, &stSlotInfo);

    /* 5.if all modules in slot have none diff ver, clear alarm */
    if (false == UPGD_IsSlotVerDif(stAutoUpgdInfo.iSlotId, false)) {
        OPERATIOND_SetUpgdAlm(UPGRADE_ALM_FAILED, false);

        /* tranfer slot-id from hal to app layer(defined in org-openroadm-device) */
        /* OPERATIOND_SetSlotAlm(stAutoUpgdInfo.iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_SW_MISMATCH, false); */
    }

cleanup:

    /* 7.config */
    if (true == stAutoUpgdInfo.bNeedConfig) {
        LOG_WRITE(EN_LOG_NOTICE, "auto upgd complete, need to config, iSlotId = %d", stAutoUpgdInfo.iSlotId);
        Slot_Config(stAutoUpgdInfo.iSlotId);
    }

    if (bGetSysLedStatus) {
        BOARD_SetLedState(LED0, stBoardStaData.astLed[DSC_LED_SYS].uiState);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId: %d, bNeedConfig: %d --", __func__, stAutoUpgdInfo.iSlotId, stAutoUpgdInfo.bNeedConfig);
    return NULL;
#else
    CSlotInfo stSlotInfo;
    int rc = OPLK_OK;
    AUTO_UPGD_SLOT_INFO_ST stAutoUpgdInfo = {0};

    LOG_WRITE(EN_LOG_NOTICE, "%s [ignore auto upgrade] ++", __func__);

    if (NULL == pvArg) {
        LOG_WRITE(EN_LOG_ERROR, "input para null!");
        return NULL;
    }

    memset(&stAutoUpgdInfo, 0, sizeof(stAutoUpgdInfo));
    memcpy(&stAutoUpgdInfo, (AUTO_UPGD_SLOT_INFO_ST *)pvArg, sizeof(AUTO_UPGD_SLOT_INFO_ST));
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId: %d, bNeedConfig: %d", __func__, stAutoUpgdInfo.iSlotId, stAutoUpgdInfo.bNeedConfig);

    /* get slot info */
    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(stAutoUpgdInfo.iSlotId, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error SLOT_GetInfo iSlotId = %d", stAutoUpgdInfo.iSlotId);
        goto cleanup;
    }

    /* update mfg */
    Slot_PutPlugableDeviceMfgToOrgAndOc(stAutoUpgdInfo.iSlotId, SLOT_STATE_ONLINE, &stSlotInfo);

cleanup:
    /* config */
    if (true == stAutoUpgdInfo.bNeedConfig) {
        LOG_WRITE(EN_LOG_NOTICE, "auto upgd complete, need to config, iSlotId = %d", stAutoUpgdInfo.iSlotId);
        Slot_Config(stAutoUpgdInfo.iSlotId);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s [ignore auto upgrade] iSlotId: %d, bNeedConfig: %d --", __func__, stAutoUpgdInfo.iSlotId, stAutoUpgdInfo.bNeedConfig);

    return NULL;

#endif
}

int
CreateAutoUpgdThread(AUTO_UPGD_SLOT_INFO_ST *pstAutoUpgdSlotInfo)
{
    int rc = OPLK_OK;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (pstAutoUpgdSlotInfo->iSlotId >= SLOT_MAX_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "input para error, iSlotId = %d", pstAutoUpgdSlotInfo->iSlotId);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Create auto upgrade thread, SlotId = %d, NeedConfig = %d",
              pstAutoUpgdSlotInfo->iSlotId, pstAutoUpgdSlotInfo->bNeedConfig);

    rc = pthread_create(&m_astAutoUpgdThread[pstAutoUpgdSlotInfo->iSlotId], NULL, __AutoUpgdProcessFunc, (void *)pstAutoUpgdSlotInfo);
    if (0 != rc) {
        LOG_WRITE(EN_LOG_ERROR, "start queue thread fail, iSlotId = %d", pstAutoUpgdSlotInfo->iSlotId);
        return OPLK_ERR;
    }

    sleep(1);

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return OPLK_OK;
}

int
StopAutoUpgdThread(int iSlotId)
{

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (iSlotId >= SLOT_MAX_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "input para error, iSlotId = %d", iSlotId);
        return OPLK_ERR;
    }

    if (m_astAutoUpgdThread[iSlotId] > 0) {
        pthread_cancel(m_astAutoUpgdThread[iSlotId]);
        pthread_join(m_astAutoUpgdThread[iSlotId], NULL);
        LOG_WRITE(EN_LOG_NOTICE, "stop auto upgrade thread, iSlotId = %d", iSlotId);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return OPLK_OK;
}

int
Upgd_OperDataSub(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    bool bIsInstall = false;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    for (int i = 0; i < (sizeof(m_astUpgdProvider) / sizeof(ST_PROVIDER_SUB)); i++) {
        bIsInstall = true;
        if (m_astUpgdProvider[i].pcModuleName != NULL) {
            bIsInstall = DSC_IsSchemaInstalled(pstSess, m_astUpgdProvider[i].pcModuleName);
        }

        if (true == bIsInstall) {
            rc = DSC_GetItemsSubscribe(m_astUpgdProvider[i].pcModuleName,
                                       m_astUpgdProvider[i].pcXpath,
                                       m_astUpgdProvider[i].pFCallback,
                                       m_astUpgdProvider[i].pvPrivateCtx);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItemsSubscribe: %s: %s", m_astUpgdProvider[i].pcXpath, sr_strerror(rc));
            }
            LOG_WRITE(EN_LOG_DEBUG, "Sub get_items: %s succ", m_astUpgdProvider[i].pcXpath);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "Not install: %s", m_astUpgdProvider[i].pcModuleName);
        }
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);
    return rc;
}

int
Upgd_RegisterRpc(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    bool bIsInstall = false;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    for (int i = 0; i < (sizeof(m_astUpgdRpcSub) / sizeof(ST_RPC_SUB)); i++) {
        bIsInstall = true;
        if (m_astUpgdRpcSub[i].pcModuleName != NULL) {
            bIsInstall = DSC_IsSchemaInstalled(pstSess, m_astUpgdRpcSub[i].pcModuleName);
        }

        if (true == bIsInstall) {
            rc = DSC_RpcSubscribe(m_astUpgdRpcSub[i].pcXpath, m_astUpgdRpcSub[i].pFCallback, (void *)pstSess);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_rpc_subscribe: %s: %s", m_astUpgdRpcSub[i].pcXpath, sr_strerror(rc));
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Sub rpc: %s succ", m_astUpgdRpcSub[i].pcXpath);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "Not install: %s", m_astUpgdRpcSub[i].pcModuleName);
        }
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);
    return rc;
}

void
Upgd_Process()
{
    bool bHasUpgrade = false;
    ST_UPGD_INFO stUpgdInfo;
    ST_BDL_VER stEepromBdlVer;
    bool bBundleMatched = true;
    int rc = OPLK_OK;
    int iSlotId = 0;
    int iUpgdSlotId = 0;

    rc = UPGD_Init(&bHasUpgrade);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade init error. rc = %d", rc);
    }

    if (true == bHasUpgrade) {
        OPERATIOND_SetUpgdAlm(UPGRADE_ALM_ACTIVE, true);
    }

    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);

    memset(&stEepromBdlVer, 0, sizeof(stEepromBdlVer));
    UPGD_GetBdlVer(&stEepromBdlVer);

    bBundleMatched = (0 == strncmp(stUpgdInfo.acBdlVerRun, stEepromBdlVer.acBdlVerOnChassis, DEF_VER_LEN_MAX)) ? true : false;

    LOG_WRITE(EN_LOG_NOTICE, "bundle versions are %s between running version(%s) and eeprom version(%s).", bBundleMatched ? "matched" : "mismatched",
              stUpgdInfo.acBdlVerRun, stEepromBdlVer.acBdlVerOnChassis);

    /* check auto upgrade condition */
    if (bBundleMatched) {
        if ((EN_UPGD_BANK_B == stUpgdInfo.stBootInfo.ucAppRunning)
            && ((true == stUpgdInfo.bVerDif))) {
            UPGD_SetPlugUpgdCondition(false);
        } else {
            UPGD_SetPlugUpgdCondition(true);
        }
    } else {
        UPGD_SetPlugUpgdCondition(false);
        /*When the upgrade is successful, running bundle version is written to chassis/rpc eeprom, otherwise it is not written.*/
        /* report scc mismatch alarm when there is no upgrade. */
        /* running bundle version would be writed to eeprom when the upgrade is successful */
        if (!bHasUpgrade) {
            OPERATIOND_SetUpgdAlm(UPGRADE_ALM_SCC_SW_MISMATCH, true);
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "bVerDif: %d, bHasUpgrade: %d", stUpgdInfo.bVerDif, bHasUpgrade);
    if (true == stUpgdInfo.bVerDif) {

        if (true == bHasUpgrade) {
            OPERATIOND_SetUpgdAlm(UPGRADE_ALM_FAILED, true);
        }

        /* check Slot Version (OPS/AMP/AMP-P/SCC card) */
        for (iSlotId = DSC_SLOT_1; iSlotId <= DSC_SLOT_4; iSlotId++) {

            iUpgdSlotId = (DSC_SLOT_4 == iSlotId) ?  (DEF_UPGD_SLOT_MAX - 1) : (iSlotId - SLOT_ID_SLC_OFFSET);

            if (UPGD_IsSlotVerDif(iUpgdSlotId, true)) {
                OPERATIOND_SetSlotAlm(iSlotId, EN_CARD_ALM_SW_MISMATCH, true);
                LOG_WRITE(EN_LOG_WARNING, "SW version mismatch on slot%d", iSlotId);
            }
        }
    }

    return;
}

