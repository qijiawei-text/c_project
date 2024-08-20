/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_reset.c
   Description : reset
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
#include "oplkupgd.h"
#include "oplkipc.h"
#include "DSC_inc.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"
#include "DevWssCApi.h"
#include "DevOpsCApi.h"
#include "DevDbgCApi.h"

#include "operationd_alm.h"
#include "operationd_file.h"
#include "operationd_reset.h"
#include "operationd_queue.h"
#include "operationd_config_mfg.h"


typedef struct {
    const char *pcResetModName;
    uint8_t ucIndex;
    uint32_t uiSlotIdx;
} RESET_MOD_MAPPING_ST;

int
__SendRstNotice(void)
{
#define WR_CNT 2

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

#define RST_XPATH_NOTICE  "/org-openroadm-de-operations:restart-notification"
#define RST_XPATH_NOTICE_NODEID RST_XPATH_NOTICE "/device/node-id"
#define RST_XPATH_NOTICE_CIRCUIT_PACK_NAME RST_XPATH_NOTICE "/resource/resource/node-id"
#define RST_XPATH_NOTICE_RSC_TYPE RST_XPATH_NOTICE "/resourceType/type"


    char acNodeId[NODE_ID_MAX_SIZE] = {0};
    memset(acNodeId, 0, sizeof(acNodeId));
    rc = DSC_GetNodeId(acNodeId, sizeof(acNodeId));
    if (rc != SR_ERR_OK) {
        snprintf(acNodeId, sizeof(acNodeId), "%s", "openroadm");
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetNodeId: %s", sr_strerror(rc));
    }
    LOG_WRITE(EN_LOG_NOTICE, "acNodeId: %s", acNodeId);
    /* Operationd_GetNodeId(acNodeId); */

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = RST_XPATH_NOTICE_NODEID;
    astValues[0].type = SR_STRING_T;
    astValues[0].data.string_val = acNodeId;


    /* see org-openroadm-resource-types.yang's typedef resource-type-enum*/
    astValues[1].xpath = RST_XPATH_NOTICE_RSC_TYPE;
    astValues[1].type = SR_ENUM_T;
    astValues[1].data.enum_val = "shelf";

    rc = sr_notif_send(pstSess,
                       RST_XPATH_NOTICE,
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

#if 0
static void
__WaitOpsResetStatus(void)
{
    int rc = OPLK_OK;
    int iCnt = 0;

    /* max waiting time is 30s */
    while (iCnt < 10) {
        rc = OPS_GetUpgadeStatus(0);
        if (OPLK_OK == rc) {
            break;
        } else {
            iCnt++;
            DEF_SLEEP_MS(3000);
        }
    }

    return;
}
#endif

/*luowei modify*/
#if 0
static int
__RpcResetCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
             sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    char acResetType[32] = {0};
    char acResetModule[32] = {0};

    int iResetOption = 0;
    char acResetOption[32] = {0};

    unsigned char ucIndex = 0;
    unsigned char ucLedIdx = 0;

    ANONYMOUS_ST astAnonymous[] = {{RPC_XPATH_RESTART_TYPE, SR_ENUM_T, acResetType, sizeof(acResetType) - 1},
                                   {RPC_XPATH_RESTART_EXTENSION, SR_STRING_T, acResetModule, sizeof(acResetModule) - 1},
                                   {RPC_XPATH_RESTART_OPTION, SR_ENUM_T, acResetOption, sizeof(acResetOption) - 1}};

    rc = DSC_ParseValuesByXpath(input, input_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Parse rpc parameters error: %d", rc);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "type: %s, module: %s, option: %s", acResetType, acResetModule, acResetOption);

    if (0 == strcmp(acResetOption, COLD_RESTART_STR)) {
        iResetOption = RST_MODE_COLD;
    } else if (0 == strcmp(acResetOption, WARM_RESTART_STR)) {
        iResetOption = RST_MODE_WARM;
    }

    if (RST_CHASSIS == ucIndex || RST_CARD_SCC == ucIndex) {
        rc = IPC_SendResetRpc((sr_session_ctx_t *)private_ctx, ucIndex, iResetOption);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Reset system %d error %d", ucIndex, rc);
            goto cleanup;
        }
    } else {
        BOARD_SetLedState(ucLedIdx, LED_STATUS_GREEN_BLINK);
        /* TODO: */
        /* rc = DEV_Reset(ucIndex, iResetOption); */
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Reset slot%d error %d", ucIndex, rc);
            goto cleanup;
        }
        BOARD_SetLedState(ucLedIdx, LED_STATUS_GREEN);
    }

    DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_SUCCESS, "System reset started...", output, output_cnt);
    __SendRstNotice();

    if (0 == strcmp(acResetOption, COLD_RESTART_STR)) {
        OPERATIOND_SetRstAlm(RESET_ALM_COLD, true);
    } else if (0 == strcmp(acResetOption, WARM_RESTART_STR)) {
        OPERATIOND_SetRstAlm(RESET_ALM_WARM, true);
    }

    DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, USER_LOG_TYPE_ID_REBOOT, UTIL_UpperStr(acResetOption));

    return SR_ERR_OK;
cleanup:
    DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_FAILED, "System reset error", output, output_cnt);
    return SR_ERR_OK;
}
#endif


static int
__GetResetModuleIdxAndSlotIdx(const char *pcModuleName, uint8_t *pucIndex, uint32_t *puiSlotIdx)
{
    /* card reset module info */
    static RESET_MOD_MAPPING_ST m_astResetModMapping[EN_PRODUCT_TYPE_MAX][RST_MODULE_MAX] = {
        /* OLS-P */
        {
            {RESET_DEVICE, RST_CHASSIS, DSC_SLOT_NONE},
            {RESET_CARD_SCC, RST_CARD_SCC, DSC_SLOT_NONE},

            {RESET_CARD_OPS, RST_CARD_OPS, DSC_SLOT_IDX0},

            {RESET_CARD_AMP, RST_CARD_AMP, DSC_SLOT_IDX1},
            {RESET_CARD_EDFA, RST_CARD_EDFA, DSC_SLOT_IDX1},
            {RESET_CARD_OCM, RST_CARD_OCM, DSC_SLOT_IDX1},

            {RESET_CARD_AMP_P, RST_CARD_AMP_P, DSC_SLOT_IDX2},
            {RESET_CARD_EDFA_P, RST_CARD_EDFA_P, DSC_SLOT_IDX2},
            {RESET_CARD_OCM_P, RST_CARD_OCM_P, DSC_SLOT_IDX2},

            {NULL, RST_MODULE_MAX, DSC_SLOT_NUM},
        },
        /* NR1004 AMP */
        {
            {RESET_DEVICE, RST_CHASSIS, DSC_SLOT_NONE},
            {RESET_CARD_SCC, RST_CARD_SCC, DSC_SLOT_NONE},

            {RESET_CARD_OPS, RST_CARD_OPS, DSC_SLOT_IDX0},

            {RESET_CARD_AMP, RST_CARD_AMP, DSC_SLOT_IDX1},
            {RESET_CARD_EDFA, RST_CARD_EDFA, DSC_SLOT_IDX1},
            {RESET_CARD_OCM, RST_CARD_OCM, DSC_SLOT_IDX1},

            {RESET_CARD_AMP_P, RST_CARD_AMP_P, DSC_SLOT_IDX2},
            {RESET_CARD_EDFA_P, RST_CARD_EDFA_P, DSC_SLOT_IDX2},
            {RESET_CARD_OCM_P, RST_CARD_OCM_P, DSC_SLOT_IDX2},

            {NULL, RST_MODULE_MAX, DSC_SLOT_NUM}
        },
        /* NR1004 OPS, TODO */
        {
            {RESET_DEVICE, RST_CHASSIS, DSC_SLOT_NONE},
            {RESET_CARD_SCC, RST_CARD_SCC, DSC_SLOT_NONE},

            {RESET_CARD_OPS, RST_CARD_OPS, DSC_SLOT_IDX0},

            /* TODO for OPS4_A and OPS_B */

            {NULL, RST_MODULE_MAX, DSC_SLOT_NUM}
        },
        /* OLS-HP or HCF */
        {
            {RESET_DEVICE, RST_CHASSIS, DSC_SLOT_NONE},
            {RESET_CARD_SCC, RST_CARD_SCC, DSC_SLOT_NONE},

            {RESET_CARD_OPTICAL, RST_CARD_AMP, DSC_SLOT_IDX0},
            {RESET_CARD_AMP, RST_CARD_AMP, DSC_SLOT_IDX0},
            {RESET_CARD_EDFA, RST_CARD_EDFA, DSC_SLOT_IDX0},
            {RESET_CARD_OCM, RST_CARD_OCM, DSC_SLOT_IDX0},
            {RESET_CARD_OTDR, RST_CARD_OTDR, DSC_SLOT_IDX0},

            {NULL, RST_MODULE_MAX, DSC_SLOT_NUM}
        },
        /* OCS */
        {
            {RESET_DEVICE, RST_CHASSIS, DSC_SLOT_NONE},
            {RESET_CARD_SCC, RST_CARD_SCC, DSC_SLOT_NONE},

            {RESET_CARD_OCS, RST_CARD_OCS, DSC_SLOT_IDX0},

            {NULL, RST_MODULE_MAX, DSC_SLOT_NUM}
        }
    };
    uint32_t uiProductType = EN_PRODUCT_TYPE_MAX;
    size_t i = 0;
    size_t nCnt = 0;
    bool bFound = false;

    uiProductType = BOARD_GetProductType();
    if (uiProductType >= EN_PRODUCT_TYPE_MAX) {
        uiProductType = EN_PRODUCT_TYPE_HCF;
    }

    nCnt = sizeof(m_astResetModMapping[uiProductType]) / sizeof(m_astResetModMapping[uiProductType][0]);
    for (i = 0; i < nCnt; i++) {
        if (NULL == m_astResetModMapping[uiProductType][i].pcResetModName) {
            break;
        }

        if (0 == strcmp(pcModuleName, m_astResetModMapping[uiProductType][i].pcResetModName)) {
            bFound = true;
            break;
        }
    }

    if (!bFound) {
        return OPLK_ERR;
    }

    *pucIndex = m_astResetModMapping[uiProductType][i].ucIndex;
    *puiSlotIdx = m_astResetModMapping[uiProductType][i].uiSlotIdx;

    return OPLK_OK;

}

static int
__RpcResetCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
             sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    char acResetType[32] = {0};
    char acResetModule[32] = {0};
    int iResetOption = 0;
    char acResetOption[32] = {0};
    unsigned char ucIndex = 0;
    uint32_t uiSlotIdx = DSC_SLOT_NUM;

    ANONYMOUS_ST astAnonymous[] = {{RPC_XPATH_RESTART_TYPE, SR_ENUM_T, acResetType, sizeof(acResetType) - 1},
                                   {RPC_XPATH_RESTART_EXTENSION, SR_STRING_T, acResetModule, sizeof(acResetModule) - 1},
                                   {RPC_XPATH_RESTART_OPTION, SR_ENUM_T, acResetOption, sizeof(acResetOption) - 1}};

    rc = DSC_ParseValuesByXpath(input, input_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Parse rpc parameters error: %d", rc);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "type: %s, module: %s, option: %s", acResetType, acResetModule, acResetOption);

    rc = __GetResetModuleIdxAndSlotIdx(acResetModule, &ucIndex, &uiSlotIdx);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid acResetModule: %s", acResetModule);
        rc = ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(acResetOption, COLD_RESTART_STR)) {
        iResetOption = RST_MODE_COLD;
    } else if (0 == strcmp(acResetOption, WARM_RESTART_STR)) {
        iResetOption = RST_MODE_WARM;
    } else {
        rc = ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (RST_CHASSIS == ucIndex || RST_CARD_SCC == ucIndex) {
        rc = IPC_SendResetRpc((sr_session_ctx_t *)private_ctx, ucIndex, iResetOption);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Reset system %d error %d", ucIndex, rc);
            goto ERR_LABEL;
        }
    } else {
        ST_UTIL_QUEUE_MSG stQueueMsg;

        if (false == OPERATIOND_GetCardRestFlag(uiSlotIdx)) {        /*check whether card reset is in running*/
            memset(&stQueueMsg, 0, sizeof(stQueueMsg));
            stQueueMsg.ulMsgOid = OID_CARD_RESET + ucIndex;
            strcpy(stQueueMsg.acMsgBuf, acResetOption);
            stQueueMsg.ulMsgLen = strlen(stQueueMsg.acMsgBuf) + 1;

            if (0 != UTIL_AddQueueNode(OPERATIOND_GetQueueIdx(), (char *)&stQueueMsg, sizeof(stQueueMsg))) {
                LOG_WRITE(EN_LOG_ERROR, "add node to queue failed");
                goto ERR_LABEL;
            }
            OPERATIOND_SetCardRestFlag(uiSlotIdx, true);
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Card reset is running.");
            goto ERR_LABEL;
        }
    }

    if (RST_CHASSIS == ucIndex) {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_SUCCESS, "System reset started...", output, output_cnt);
        __SendRstNotice();
    } else if (RST_CARD_SCC == ucIndex) {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_SUCCESS, "Scc reset started...", output, output_cnt);
        __SendRstNotice();
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_SUCCESS, "Optical card reset started...", output, output_cnt);
    }

    if (RST_CHASSIS == ucIndex) {
        if (0 == strcmp(acResetOption, COLD_RESTART_STR)) {
            OPERATIOND_SetRstAlm(RESET_ALM_COLD, true);
        } else if (0 == strcmp(acResetOption, WARM_RESTART_STR)) {
            OPERATIOND_SetRstAlm(RESET_ALM_WARM, true);
        }
    }

    return SR_ERR_OK;
ERR_LABEL:
    if (RST_CHASSIS == ucIndex) {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_FAILED, "System reset error", output, output_cnt);
    } else if (RST_CARD_SCC == ucIndex) {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_FAILED, "Scc reset error", output, output_cnt);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_RESTART, RPC_STATUS_FAILED, "Optical card reset error", output, output_cnt);
    }
    return rc;
}

static ST_RPC_SUB m_astRstRpcSub[] = {
    {MODULE_NS_OODO, __RpcResetCb, RPC_XPATH_RESTART},
};

int
Reset_RegisterRpc(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    bool bIsInstall = false;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    for (int i = 0; i < (sizeof(m_astRstRpcSub) / sizeof(ST_RPC_SUB)); i++) {
        bIsInstall = true;
        if (m_astRstRpcSub[i].pcModuleName != NULL) {
            bIsInstall = DSC_IsSchemaInstalled(pstSess, m_astRstRpcSub[i].pcModuleName);
        }

        if (true == bIsInstall) {
            rc = DSC_RpcSubscribe(m_astRstRpcSub[i].pcXpath, m_astRstRpcSub[i].pFCallback, (void *)pstSess);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_rpc_subscribe: %s: %s", m_astRstRpcSub[i].pcXpath, sr_strerror(rc));
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Sub rpc: %s succ", m_astRstRpcSub[i].pcXpath);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "Not install: %s", m_astRstRpcSub[i].pcModuleName);
        }
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);
    return rc;
}

static bool
__IsUpgradeRest()
{
    ST_UPGD_CTRL stUpgdCtrl;

    memset(&stUpgdCtrl, 0, sizeof(stUpgdCtrl));
    /* Read the upgrade variable from the EEprom                                   */
    if (OPLK_OK != BOARD_GetUpgInfo((uint8_t *)(&stUpgdCtrl), sizeof(ST_UPGD_CTRL))) {
        LOG_WRITE(EN_LOG_ERROR, "error BOARD_GetUpgInfo");
        return false;
    }

    return EN_UPGD_SWITCHED == stUpgdCtrl.ucState;
}

int
__ButtonRest()
{
    int i = 0;
    int rc = SR_ERR_OK;
    uint8_t ucFlag[BT_FUNC_MAX] = {0, 0, 0};
    uint8_t ucClear = 0x10;
    ST_RPC_BUTTON_CTRL stRpcButtonCtrl[BT_FUNC_MAX] = {
        {BT_REG_WARM, 0, 0}, {BT_REG_COLD, 0, 0}, {BT_REG_RTFD, 0, 0}
    };

    for (i = 0; i < BT_FUNC_MAX; i++) {
        DBG_ReadLogicReg(0, stRpcButtonCtrl[i].usReg, 1, &stRpcButtonCtrl[i].ucState);
        ucFlag[i] = stRpcButtonCtrl[i].ucState & 0x01;
        LOG_WRITE(EN_LOG_NOTICE, "%s ucFlag[%d]=%d", __func__, i, ucFlag[i]);
    }

    for (i = 0; i < BT_FUNC_MAX; i++) {
        if (1 == ucFlag[i]) {
            LOG_WRITE(EN_LOG_NOTICE, "%s Clear button state, ucFlag[%d]=%d", __func__, i, ucFlag[i]);

            ucClear = 0x10;
            DBG_WriteLogicReg(0, stRpcButtonCtrl[i].usReg, 1, &ucClear);
            ucClear = 0x0;
            DBG_WriteLogicReg(0, stRpcButtonCtrl[i].usReg, 1, &ucClear);

            if (i == BT_FUNC_RTFD) {
                LOG_WRITE(EN_LOG_NOTICE, "!!!RpcButton System will restore default!!!");
                rc = DSC_RestoreDefault(EN_RETORE_DEFAULT_ALL);
                if (rc != SR_ERR_OK) {
                    goto ERR_LABEL;
                }
            }
        }
    }

ERR_LABEL:

    return rc;
}

void
Reset_Process(uint32_t ulSlotNum, CSlotInfo *pstSlotInfo, bool *pbNeedConfig, uint8_t *pucResetType)
{
    int rc = OPLK_OK;
    uint8_t ucResetType = RST_MODE_WDT;
    bool bNeedConfig = false;
    int i = 0;

    /* In ueno project, the reset cause is saved in fan as a extension */
    uint8_t ucResetExtInFan = 0;

    __ButtonRest();

    rc = BOARD_GetResetType(0, &ucResetType);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get reset type error. errNum = %d", rc);
    }

    FAN_GetResetCauseInFan(&ucResetExtInFan);

    if (RST_MODE_POWER == ucResetType) {
        if (ucResetExtInFan != 0) {
            /* means the chassis is NOT power off, ONLY plug the SCC card. */
            bNeedConfig = false;
        } else {
            /* means the chassis and scc both are off. */
            bNeedConfig = true;
        }
    } else if (RST_MODE_COLD == ucResetType) {
        bNeedConfig = true;
    } else {
        bNeedConfig = false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "####### System Reset Type = %hhu, infan: %hhu, config: %d", ucResetType, ucResetExtInFan, bNeedConfig);

    rc = DSC_SHELF_SetResetCause(ucResetType);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_SetResetCause ResetType = %d", ucResetType);
    }

    for (i = 0; i < ulSlotNum; i++) {
        if (SLOT_STATE_ONLINE != (pstSlotInfo + i)->uiSlotState) {
            continue;
        }

        if ((CARD_TYPE_UNKOWN == (pstSlotInfo + i)->uiCardType) ||
            (CARD_TYPE_MUX == (pstSlotInfo + i)->uiCardType)) {
            LOG_WRITE(EN_LOG_ERROR, "Slot%d card type unknow.", i);
            continue;
        }

        if ( RST_MODE_COLD == ucResetType || RST_MODE_WARM == ucResetType) {
            rc = SLOT_Reset(i, ucResetType);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "SLOT_Reset slot = %d, reset = %d, rc = %d", i, ucResetType, rc);
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "SLOT_Reset slot = %d, reset = %d, rc = %d", i, ucResetType, rc);
            }
        } else {
            /* ingnore for other reset type (eg. RST_MODE_POWER , RST_MODE_WDT) */
        }
    }

    if (RST_MODE_COLD == ucResetType) {
        /* a. wss cold reset need 40s;
           b. ops/edfa hard reset need to reload cpld, suggest to wait 90s (TODO) */
        sleep(90);

        LOG_WRITE(EN_LOG_NOTICE, "All modules hard reset complete!");
    } else if (RST_MODE_WARM == ucResetType) {
        /* a. wss warm reset need 20s;
           b. edfa upgrading reset need 90s (max);
           c. ops upgrading reset need 90s (max), including ops's cpld
         */
        if (__IsUpgradeRest()) {
            LOG_WRITE(EN_LOG_NOTICE, "Upgrading reset need to wait 90s!");
            sleep(120);            /* OC2A-263 ops upg 0.01.0023 to 0.02.0002 reset need 99s , 20231128 110 to 120 for v0.03.0001 as upgrade cpld need 109s*/
        } else {
            sleep(20);
        }

        LOG_WRITE(EN_LOG_NOTICE, "All modules soft reset complete!");
    }

    *pbNeedConfig = bNeedConfig;
    *pucResetType = ucResetType;

    rc = BOARD_SetResetType(RST_MODE_WDT);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Clear reset type error. errNum = %d", rc);
    }

    rc = FAN_SetResetCauseInFan(RST_MODE_WDT);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set reset type in fan. errNum = %d", rc);
    }

    return;
}

