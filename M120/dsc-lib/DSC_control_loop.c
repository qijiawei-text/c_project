
/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_edfa.h"
#include "DSC_control_loop.h"

int
DSC_AutoSetLongSpan(char *pcLine, bool bFlag)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_bool_item(pstSess, __sprintfStr(acPath, LONG_SPAN_XPATH, pcLine), bFlag);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by long span: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}


int
DSC_AutoSetOcmBasedCarriers(char *pcLine, bool bOcmBasedCarriers)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_bool_item(pstSess, __sprintfStr(acPath, CARRIERS_BASED_OCM_XPATH, pcLine), bOcmBasedCarriers);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by ocm based carriers: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetNumOfCarriers(char *pcLine, uint8_t ucNumOfCarriers)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

#if 0 /* for 400GZR */
    if (0 != (ucNumOfCarriers % 2)) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Number of carries is not even!");
        return rc;
    }
#endif

    rc = set_uint8_item(pstSess, __sprintfStr(acPath, NUM_OF_CARRIERS_XPATH, pcLine), ucNumOfCarriers);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by number of carriers: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoGetNumOfCarriers(char *pcLine, uint8_t *pucNumOfCarriers)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(NUM_OF_CARRIERS_XPATH, SR_UINT8_T, (char *)pucNumOfCarriers),
    };

    if (NULL == pucNumOfCarriers) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetItems(pstSess, __sprintfStr(acPath, AUTO_CONTROL_CONFIG_ALL, pcLine), astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetManualOscIp(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstRemoteInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = set_string_item(pstSess, __sprintfStr(acPath, MANUAL_REMOTE_IP_XPATH, pcLine), pstRemoteInfo->acRemoteOscIp, strlen(pstRemoteInfo->acRemoteOscIp));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting remote osc ip address: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_string_item(pstSess, __sprintfStr(acPath, MANUAL_LOCAL_OSC_IP_XPATH, pcLine), pstRemoteInfo->acLocalOscIp, strlen(pstRemoteInfo->acLocalOscIp));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting remote osc ip address: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoGetMaunalOscIp(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstRemoteInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, MANUAL_REMOTE_IP_XPATH, pcLine), pstRemoteInfo->acRemoteOscIp, sizeof(pstRemoteInfo->acRemoteOscIp));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting remote osc ip address: %s", sr_strerror(rc));
        return rc;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, MANUAL_LOCAL_OSC_IP_XPATH, pcLine), pstRemoteInfo->acLocalOscIp, sizeof(pstRemoteInfo->acLocalOscIp));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting local ip address: %s", sr_strerror(rc));
        return rc;
    }


    return rc;
}


int
DSC_AutoSetLineTx(char *pcLine, double dLineTx)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, LINE_TX_TARGET_APR_POWER_XPATH, pcLine), dLineTx);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting line tx target power: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoGetLineTx(char *pcLine, double *pdLineTx)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdLineTx) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acPath, LINE_TX_TARGET_APR_POWER_XPATH, pcLine), pdLineTx);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting line tx target power: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfgWithoutNumOfCarriers(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acFiberTypeStr[DEF_BUF_SIZE_32] = {0};
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstAutoCfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == DSC_FiberTypeToString(acFiberTypeStr, pstAutoCfg->enFiberType)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid fiber type: %d", pstAutoCfg->enFiberType);
        return SR_ERR_INVAL_ARG;
    }
    rc = set_enum_item(pstSess, __sprintfStr(acPath, FIBER_TYPE_XPATH, pcLine), acFiberTypeStr);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by fiber type: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, LAUNCHING_POWER_XPATH, pcLine), pstAutoCfg->dLauchingPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by launching power per channel: %s", sr_strerror(rc));
        return rc;
    }

#if 1
    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, TARGET_POWER_XPATH, pcLine), pstAutoCfg->dTargetPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by target power per channel: %s", sr_strerror(rc));
        return rc;
    }
#endif

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfgFibertype(char *pcLine, FIBER_TYPE_EN enFiberType)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acFiberTypeStr[DEF_BUF_SIZE_32] = {0};
    double dLauchingPowerDefaultValue[FIBER_TYPE_CNT] = {5.0, 5.0, 1.0, 1.0, 0.0, 0.0, -1.0, -2.0, -10.0, -10.0};
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (FIBER_TYPE_CNT <= enFiberType) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == DSC_FiberTypeToString(acFiberTypeStr, enFiberType)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid fiber type: %d", enFiberType);
        return SR_ERR_INVAL_ARG;
    }
    rc = set_enum_item(pstSess, __sprintfStr(acPath, FIBER_TYPE_XPATH, pcLine), acFiberTypeStr);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by fiber type: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, LAUNCHING_POWER_XPATH, pcLine), dLauchingPowerDefaultValue[enFiberType]);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by launching power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}


int
DSC_AutoSetCfgLanchingPower(char *pcLine, double dLauchingPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, LAUNCHING_POWER_XPATH, pcLine), dLauchingPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by launching power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfgDropPower(char *pcLine, double dTargetPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, TARGET_POWER_XPATH, pcLine), dTargetPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by target power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfgSaturationDrop(char *pcLine, double dSaturationDrop)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, SATURATION_DROP_XPATH, pcLine), dSaturationDrop);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by saturation drop: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfgPaInputTargetPower(char *pcLine, double dTargetPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, PA_INPUT_TARGET_POWER_XPATH, pcLine), dTargetPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by pa input target power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCfg(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acFiberTypeStr[DEF_BUF_SIZE_32] = {0};
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstAutoCfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == DSC_FiberTypeToString(acFiberTypeStr, pstAutoCfg->enFiberType)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid fiber type: %d", pstAutoCfg->enFiberType);
        return SR_ERR_INVAL_ARG;
    }
    rc = set_enum_item(pstSess, __sprintfStr(acPath, FIBER_TYPE_XPATH, pcLine), acFiberTypeStr);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by fiber type: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_uint8_item(pstSess, __sprintfStr(acPath, NUM_OF_CARRIERS_XPATH, pcLine), pstAutoCfg->ucNumOfCarriers);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by number of carriers: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, LAUNCHING_POWER_XPATH, pcLine), pstAutoCfg->dLauchingPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by launching power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, TARGET_POWER_XPATH, pcLine), pstAutoCfg->dTargetPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by target power per channel: %s", sr_strerror(rc));
        return rc;
    }

    rc = set_bool_item(pstSess, __sprintfStr(acPath, LONG_SPAN_XPATH, pcLine), pstAutoCfg->bLongSpan);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by long span: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoGetCfg(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    /* sr_session_ctx_t *pstSess = DSC_GetRunningSession(); */
    char acFiberTypeStr[DEF_BUF_SIZE_32] = {0};
    AUTO_CFG_DATA_ST stAutoCfg;
    char aacXpath[32][OPLK_PATH_MAX_LEN];

    memset(aacXpath, 0, sizeof(aacXpath));

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[0], NUM_OF_CARRIERS_XPATH, pcLine), SR_UINT8_T, (char *)(&stAutoCfg.ucNumOfCarriers)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], FIBER_TYPE_XPATH, pcLine), SR_ENUM_T, acFiberTypeStr),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], LAUNCHING_POWER_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoCfg.dLauchingPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[3], TARGET_POWER_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoCfg.dTargetPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[4], CARRIERS_BASED_OCM_XPATH, pcLine), SR_BOOL_T, (char *)(&stAutoCfg.bBasedOcmCarriers)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[5], LONG_SPAN_XPATH, pcLine), SR_BOOL_T, (char *)(&stAutoCfg.bLongSpan)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[6], SATURATION_DROP_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoCfg.dSaturationDrop)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[7], PA_INPUT_TARGET_POWER_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoCfg.dPaInputTargetPower)),
    };

    if (NULL == pstAutoCfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(&stAutoCfg, 0, sizeof(stAutoCfg));
    rc = DSC_GetItems(pstSess, __sprintfStr(aacXpath[8], AUTO_CONTROL_CONFIG_ALL, pcLine), astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
    } else {
        stAutoCfg.enFiberType = DSC_FiberTypeToEnum(acFiberTypeStr);
    }

    memcpy(pstAutoCfg, &stAutoCfg, sizeof(stAutoCfg));
    return rc;
}

int
DSC_AutoStartGainCtrl(char *pcLine)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL;
    size_t input_cnt = 0;
    sr_val_t *output = NULL;
    size_t output_cnt = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acEdfaType[8] = {0};

    if (0 == strcasecmp(pcLine, WORKING_LINE_YANG)) {
        strcpy(acEdfaType, EDFA_BA_CFG_KEY);
    } else if (0 == strcasecmp(pcLine, PROTECTION_LINE_YANG)) {
        strcpy(acEdfaType, EDFA_BAP_CFG_KEY);
    } else {
        rc = SR_ERR_INVAL_ARG;
        return rc;
    }

    if (EDFA_MODE_APC == DSC_GetEdfaModeByName(acEdfaType)) {
        LOG_WRITE(EN_LOG_ERROR, "%s APC Mode", EDFA_BA_CFG_KEY);
        rc = SR_ERR_UNSUPPORTED;
        goto END_LABEL;
    }

    /* allocate input values path*/
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set input/path leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_AUTO_GAIN_START_PATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_AUTO_GAIN_START_PATH, sr_strerror(rc));
        goto END_LABEL;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcLine);

    rc = DSC_RpcSend(pstSess, RPC_XPATH_AUTO_GAIN_START, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_AUTO_GAIN_START, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_ProcessRPCResponse(output, output_cnt);

END_LABEL:

    sr_free_values(input, input_cnt);
    LOG_WRITE(EN_LOG_NOTICE, "%s (rc:%d)", __func__, rc);
    return rc;
}

int
DSC_AutoGetGainState(char *pcLine, AUTO_GAIN_STATE_ST *pstAutoGainState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    AUTO_GAIN_STATE_ST stAutoGainState;
    char aacXpath[32][OPLK_PATH_MAX_LEN];

    char acCtrlState[DEF_BUF_SIZE_32] = {0};
    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[0], SPAN_LOSS_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dSpanLoss)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], PRE_AMP_GAIN_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dPreAmpGain)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], PRE_AMP_VOA_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dPreVoaAtten)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[3], BOOSTER_AMP_GAIN_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dBoosterAmpGain)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[4], BOOSTER_AMP_VOA_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dBoosterVoaAtten)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[5], GAIN_PROGRESS_XPATH, pcLine), SR_UINT8_T, (char *)(&stAutoGainState.ucProgress)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[6], GAIN_STATE_XPATH, pcLine), SR_ENUM_T, acCtrlState),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[7], RECV_SPAN_LOSS_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dReceiveSpanloss)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[8], OSC_INPUT_POWER_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dOscInPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[9], PRE_AMP_INPUT_POWER_XPATH, pcLine), SR_DECIMAL64_T, (char *)(&stAutoGainState.dPreAmpInputPower)),
    };

    if (NULL == pstAutoGainState) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(&stAutoGainState, 0, sizeof(stAutoGainState));
    rc = DSC_GetItems(pstSess, __sprintfStr(aacXpath[10], AUTO_CONTROL_GAIN_STATE_ALL, pcLine), astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
    } else {
        stAutoGainState.enState = DSC_ControlStateToEnum(acCtrlState);
    }

    memcpy(pstAutoGainState, &stAutoGainState, sizeof(stAutoGainState));
    return rc;
}


int
DSC_AutoClearALarm()
{
    int rc = SR_ERR_OK;
    sr_val_t *output = NULL;
    size_t output_cnt = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    LOG_WRITE(EN_LOG_NOTICE, "DSC_AutoClearALarm");
    rc = sr_rpc_send(pstSess, RPC_XPATH_AUTO_CLEAR_ALM, NULL, 0, 0, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", RPC_XPATH_AUTO_CLEAR_ALM, sr_strerror(rc));
        return rc;
    }

    return DSC_ProcessRPCResponse(output, output_cnt);
}


char *
DSC_FiberTypeToString(char *pcFiberTypeStr, FIBER_TYPE_EN enFiberType)
{
    if (NULL == pcFiberTypeStr) {
        return NULL;
    }

    switch (enFiberType) {
    case FIBER_SSMF:
        strcpy(pcFiberTypeStr, FIBER_SSMF_STR);
        break;
    case FIBER_ALLWAVE:
        strcpy(pcFiberTypeStr, FIBER_ALLWAVE_STR);
        break;
    case FIBER_TERALIGHT:
        strcpy(pcFiberTypeStr, FIBER_TERALIGHT_STR);
        break;
    case FIBER_TRUEWAVE_REACH:
        strcpy(pcFiberTypeStr, FIBER_TRUEWAVE_REACH_STR);
        break;
    case FIBER_LEAF:
        strcpy(pcFiberTypeStr, FIBER_LEAF_STR);
        break;
    case FIBER_METROCORE:
        strcpy(pcFiberTypeStr, FIBER_METROCORE_STR);
        break;
    case FIBER_TRUEWAVE_RS:
        strcpy(pcFiberTypeStr, FIBER_TRUEWAVE_RS_STR);
        break;
    case FIBER_TRUEWAVE_CPLUS:
        strcpy(pcFiberTypeStr, FIBER_TRUEWAVE_CPLUS_STR);
        break;
    case FIBER_LS:
        strcpy(pcFiberTypeStr, FIBER_LS_STR);
        break;
    case FIBER_DSF:
        strcpy(pcFiberTypeStr, FIBER_DSF_STR);
        break;
    case FIBER_HCF:
        strcpy(pcFiberTypeStr, FIBER_HCF_STR);
        break;
    default:
        return NULL;
        break;
    }

    return pcFiberTypeStr;
}

FIBER_TYPE_EN
DSC_FiberTypeToEnum(char *pcFiberTypeStr)
{
    if (NULL == pcFiberTypeStr) {
        return FIBER_TYPE_CNT;
    }

    if (0 == strcasecmp(pcFiberTypeStr, FIBER_SSMF_STR)) {
        return FIBER_SSMF;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_ALLWAVE_STR)) {
        return FIBER_ALLWAVE;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_TERALIGHT_STR)) {
        return FIBER_TERALIGHT;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_TRUEWAVE_REACH_STR)) {
        return FIBER_TRUEWAVE_REACH;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_LEAF_STR)) {
        return FIBER_LEAF;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_METROCORE_STR)) {
        return FIBER_METROCORE;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_TRUEWAVE_RS_STR)) {
        return FIBER_TRUEWAVE_RS;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_TRUEWAVE_CPLUS_STR)) {
        return FIBER_TRUEWAVE_CPLUS;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_LS_STR)) {
        return FIBER_LS;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_DSF_STR)) {
        return FIBER_DSF;
    } else if (0 == strcasecmp(pcFiberTypeStr, FIBER_HCF_STR)) {
        return FIBER_HCF;
    } else {
        return FIBER_TYPE_CNT;
    }

    return FIBER_TYPE_CNT;
}



char *
DSC_ControlStateToString(char *pcControlStateStr, AUTO_CONTROL_STATE_EN enControlState)
{
    if (NULL == pcControlStateStr) {
        return NULL;
    }

    switch (enControlState) {
    case AUTO_STATE_SUCCESS:
        strcpy(pcControlStateStr, AUTO_STATE_SUCCESS_STR);
        break;
    case AUTO_STATE_PROCESSING:
        strcpy(pcControlStateStr, AUTO_STATE_PROCESSING_STR);
        break;
    case AUTO_STATE_FAILED_SPANLOSS:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_SPANLOSS_STR);
        break;
    case AUTO_STATE_FAILED_BA_INPUT:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_BA_INPUT_STR);
        break;
    case AUTO_STATE_FAILED_GAIN_OTHER:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_GAIN_OTHER_STR);
        break;
    case AUTO_STATE_FAILED_CD:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_CD_STR);
        break;
    case AUTO_STATE_FAILED_OSC:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_OSC_STR);
        break;
    case AUTO_STATE_FAILED_BA_PUMP:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_BA_PUMP_STR);
        break;
    case AUTO_STATE_FAILED_BA_RFL:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_BA_RFL_STR);
        break;
    case AUTO_STATE_FAILED_BA_APR:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_BA_APR_STR);
        break;
    case AUTO_STATE_FAILED_BA_LASER_SAFETY:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_BA_LASER_SAFETY_STR);
        break;
    case AUTO_STATE_FAILED_UPD_ACTIVATE:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_UPD_ACTIVATE_STR);
        break;
    case AUTO_STATE_FAILED_SCC_SW_MISMATCH:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_SCC_SW_MISMATCH_STR);
        break;
    case AUTO_STATE_FAILED_AMP_SW_MISMATCH:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_AMP_SW_MISMATCH_STR);
        break;
    case AUTO_STATE_FAILED_APPLY_TO_MODULE:
        strcpy(pcControlStateStr, AUTO_STATE_FAILED_APPLY_TO_MODULE_STR);
        break;
    default:
        return NULL;
        break;
    }

    return pcControlStateStr;
}

AUTO_CONTROL_STATE_EN
DSC_ControlStateToEnum(char *pcControlStateStr)
{
    if (NULL == pcControlStateStr) {
        return AUTO_STATE_CNT;
    }

    if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_SUCCESS_STR)) {
        return AUTO_STATE_SUCCESS;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_PROCESSING_STR)) {
        return AUTO_STATE_PROCESSING;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_SPANLOSS_STR)) {
        return AUTO_STATE_FAILED_SPANLOSS;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_BA_INPUT_STR)) {
        return AUTO_STATE_FAILED_BA_INPUT;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_GAIN_OTHER_STR)) {
        return AUTO_STATE_FAILED_GAIN_OTHER;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_CD_STR)) {
        return AUTO_STATE_FAILED_CD;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_OSC_STR)) {
        return AUTO_STATE_FAILED_OSC;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_BA_PUMP_STR)) {
        return AUTO_STATE_FAILED_BA_PUMP;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_BA_RFL_STR)) {
        return AUTO_STATE_FAILED_BA_RFL;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_BA_APR_STR)) {
        return AUTO_STATE_FAILED_BA_APR;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_BA_LASER_SAFETY_STR)) {
        return AUTO_STATE_FAILED_BA_LASER_SAFETY;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_UPD_ACTIVATE_STR)) {
        return AUTO_STATE_FAILED_UPD_ACTIVATE;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_SCC_SW_MISMATCH_STR)) {
        return AUTO_STATE_FAILED_SCC_SW_MISMATCH;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_AMP_SW_MISMATCH_STR)) {
        return AUTO_STATE_FAILED_AMP_SW_MISMATCH;
    } else if (0 == strcasecmp(pcControlStateStr, AUTO_STATE_FAILED_APPLY_TO_MODULE_STR)) {
        return AUTO_STATE_FAILED_APPLY_TO_MODULE;
    } else {
        return AUTO_STATE_CNT;
    }

    return AUTO_STATE_CNT;
}

int
DSC_AutoSetSpanloss(char *pcLine, double dSpanloss)
{
    return DSC_AutoSetSpanloss_Commit(pcLine, dSpanloss, true);
}

int
DSC_AutoSetSpanloss_Commit(char *pcLine, double dSpanloss, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, LAST_SPANLOSS_XPATH, pcLine), dSpanloss);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting last spanloss: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetSpanloss(char *pcLine, double *pdSpanloss)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdSpanloss) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, LAST_SPANLOSS_XPATH, pcLine), pdSpanloss);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting last spanloss: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetSpanlossThreshold(char *pcLine, double dSpanlossThreshold)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acPath, SPANLOSS_THRESHOLD_XPATH, pcLine), dSpanlossThreshold);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by spanloss threshold: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoGetSpanlossThreshold(char *pcLine, double *pdSpanlossThreshold)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdSpanlossThreshold) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: %s", "");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acPath, SPANLOSS_THRESHOLD_XPATH, pcLine), pdSpanlossThreshold);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting spanloss threshold: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliOscTxPower(char *pcLine, double dPower)
{
    return DSC_AutoSetCaliOscTxPower_Commit(pcLine, dPower, true);
}

int
DSC_AutoSetCaliOscTxPower_Commit(char *pcLine, double dPower, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_OSC_TX_POWER_XPATH, pcLine), dPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali osc tx power: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliOscTxPower(char *pcLine, double *pdPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdPower) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_OSC_TX_POWER_XPATH, pcLine), pdPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting last osc tx power: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliOscRxPower(char *pcLine, double dPower)
{
    return DSC_AutoSetCaliOscRxPower_Commit(pcLine, dPower, true);
}

int
DSC_AutoSetCaliOscRxPower_Commit(char *pcLine, double dPower, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_OSC_RX_POWER_XPATH, pcLine), dPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali osc rx power: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliOscRxPower(char *pcLine, double *pdPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdPower) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_OSC_RX_POWER_XPATH, pcLine), pdPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali osc rx power: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliSpanloss(char *pcLine, double dSpanloss)
{
    return DSC_AutoSetSpanloss(pcLine, dSpanloss);
}

int
DSC_AutoSetCaliSpanloss_Commit(char *pcLine, double dSpanloss, bool bCommit)
{
    return DSC_AutoSetSpanloss_Commit(pcLine, dSpanloss, bCommit);
}

int
DSC_AutoGetCaliSpanloss(char *pcLine, double *pdSpanloss)
{
    return DSC_AutoGetSpanloss(pcLine, pdSpanloss);
}

int
DSC_AutoSetCaliNumOfCarriers(char *pcLine, uint8_t ucNumOfCarriers)
{
    return DSC_AutoSetCaliNumOfCarriers_Commit(pcLine, ucNumOfCarriers, true);
}

int
DSC_AutoSetCaliNumOfCarriers_Commit(char *pcLine, uint8_t ucNumOfCarriers, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_uint8_item(pstSess, __sprintfStr(acXpath, ACI_LAST_NUM_OF_CARRIERS_XPATH, pcLine), ucNumOfCarriers);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali number of carriers: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliNumOfCarriers(char *pcLine, uint8_t *pucNumOfCarriers)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pucNumOfCarriers) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_uint8_item(pstSess, __sprintfStr(acXpath, ACI_LAST_NUM_OF_CARRIERS_XPATH, pcLine), pucNumOfCarriers);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali number of carriers: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliBaInputPower(char *pcLine, double dPower)
{
    return DSC_AutoSetCaliBaInputPower_Commit(pcLine, dPower, true);
}

int
DSC_AutoSetCaliBaInputPower_Commit(char *pcLine, double dPower, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_INPUT_POWER_XPATH, pcLine), dPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali ba input power: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliBaInputPower(char *pcLine, double *pdPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdPower) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_INPUT_POWER_XPATH, pcLine), pdPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali ba input power: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliBaGain(char *pcLine, double dGain)
{
    return DSC_AutoSetCaliBaGain_Commit(pcLine, dGain, true);
}

int
DSC_AutoSetCaliBaGain_Commit(char *pcLine, double dGain, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_GAIN_XPATH, pcLine), dGain);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali ba gain: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliBaGain(char *pcLine, double *pdGain)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdGain) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_GAIN_XPATH, pcLine), pdGain);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting last ba gain: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliBaPaddingVoa(char *pcLine, double dVoa)
{
    return DSC_AutoSetCaliBaPaddingVoa_Commit(pcLine, dVoa, true);
}

int
DSC_AutoSetCaliBaPaddingVoa_Commit(char *pcLine, double dVoa, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_VOA_XPATH, pcLine), dVoa);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali ba padding voa: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliBaPaddingVoa(char *pcLine, double *pdVoa)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdVoa) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_BOOSTER_AMP_VOA_XPATH, pcLine), pdVoa);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali ba padding voa: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliPaInputPower(char *pcLine, double dPower)
{
    return DSC_AutoSetCaliPaInputPower_Commit(pcLine, dPower, true);
}

int
DSC_AutoSetCaliPaInputPower_Commit(char *pcLine, double dPower, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_INPUT_POWER_XPATH, pcLine), dPower);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali pa input power: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliPaInputPower(char *pcLine, double *pdPower)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdPower) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_INPUT_POWER_XPATH, pcLine), pdPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali pa input power: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliPaGain(char *pcLine, double dGain)
{
    return DSC_AutoSetCaliPaGain_Commit(pcLine, dGain, true);
}

int
DSC_AutoSetCaliPaGain_Commit(char *pcLine, double dGain, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_GAIN_XPATH, pcLine), dGain);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting cali pa gain: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliPaGain(char *pcLine, double *pdGain)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdGain) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_GAIN_XPATH, pcLine), pdGain);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting cali pa gain: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AutoSetCaliPaVoa(char *pcLine, double dVoa)
{
    return DSC_AutoSetCaliPaVoa_Commit(pcLine, dVoa, true);
}

int
DSC_AutoSetCaliPaVoa_Commit(char *pcLine, double dVoa, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = set_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_VOA_XPATH, pcLine), dVoa);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by setting last pa voa: %s", sr_strerror(rc));
        return rc;
    }

    if (true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_AutoGetCaliPaVoa(char *pcLine, double *pdVoa)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pdVoa) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_decimal64_item(pstSess, __sprintfStr(acXpath, ACI_LAST_PRE_AMP_VOA_XPATH, pcLine), pdVoa);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting last pa voa: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_GetRemoteInfo(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo)
{
#if 0
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acPath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstRemoteInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: %s", "");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, REMOTE_INFO_NODEIP_XPATH, pcLine), pstRemoteInfo->acRemoteIp, sizeof(pstRemoteInfo->acRemoteIp));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting remote ip address: %s", sr_strerror(rc));
        return rc;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, REMOTE_INFO_NODENAME_XPATH, pcLine), pstRemoteInfo->acNodeName, sizeof(pstRemoteInfo->acNodeName));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting remote nodename address: %s", sr_strerror(rc));
        return rc;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, REMOTE_INFO_OSCIP_XPATH, pcLine), pstRemoteInfo->acRemoteOscIp, sizeof(pstRemoteInfo->acRemoteOscIp));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting remote osc ip address: %s", sr_strerror(rc));
        return rc;
    }

    rc = get_string_item(pstSess, __sprintfStr(acPath, LOCAL_INFO_OSCIP_XPATH, pcLine), pstRemoteInfo->acLocalOscIp, sizeof(pstRemoteInfo->acLocalOscIp));
    if (SR_ERR_NOT_FOUND == rc) {
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting local osc ip address: %s", sr_strerror(rc));
        return rc;
    }

    return rc;
#endif
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    REMOTE_NODE_INFO_ST stRemoteInfo;
    char aacXpath[4][OPLK_PATH_MAX_LEN];

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[0], REMOTE_INFO_NODEIP_XPATH, pcLine), SR_STRING_T, stRemoteInfo.acRemoteIp),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], REMOTE_INFO_NODENAME_XPATH, pcLine), SR_STRING_T, stRemoteInfo.acNodeName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], REMOTE_INFO_OSCIP_XPATH, pcLine), SR_STRING_T, stRemoteInfo.acRemoteOscIp),
    };


    if (NULL == pstRemoteInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(&stRemoteInfo, 0, sizeof(stRemoteInfo));
    rc = DSC_GetItems(pstSess, __sprintfStr(aacXpath[3], REMOTE_INFO_ALL, pcLine), astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
    }

    memcpy(pstRemoteInfo, &stRemoteInfo, sizeof(stRemoteInfo));
    return rc;
}

int
DSC_CollectFiberConfig(char *pcLine, char *pcFiberTypeName, FIBER_PARM_LIST_ST *pstFiberList)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t input_cnt = 0, output_cnt = 0;
    sr_session_ctx_t *pstSess = NULL;
    size_t i = 0, j = -1;
    FIBER_PARM_LIST_ST stFiberList;
    FIBER_PARM_ST *pstFiberPara = NULL;

    memset(&stFiberList, 0, sizeof(stFiberList));

    if (NULL == pcFiberTypeName || NULL == pstFiberList) {
        return SR_ERR_INVAL_ARG;
    }

    pstSess = DSC_GetOperSession();

    /* allocate input values */
    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(rc));
        goto END_LABEL;
    }

    rc = set_string_val_by_xpath(&input[0], XPATH_FIBER_CONFIG_PATH, pcLine);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* input node*/
    rc = set_string_val_by_xpath(&input[1], XPATH_FIBER_TYPE_NAME, pcFiberTypeName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, RPC_XPATH_COLLECT_FIBER_CONFIG, input, input_cnt, &output, &output_cnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "rpc (%s) send failed. %s", RPC_XPATH_COLLECT_FIBER_CONFIG, sr_strerror(rc));
        goto END_LABEL;
    }

    /* parse output data */
    for (i = 0, j = -1; i < output_cnt; i++) {

        if (SR_LIST_T == output[i].type) {
            j++;
            continue;
        }

        if (j < 0 || j >= FIBER_TYPE_CNT) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid j:%ld", j);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        pstFiberPara = stFiberList.astFiberPara + j;

        if (sr_xpath_node_name_eq(output[i].xpath, FIBER_TYPE_NODE)) {
            pstFiberPara->enFiberType = DSC_FiberTypeToEnum(output[i].data.string_val);
        } else if (sr_xpath_node_name_eq(output[i].xpath, DISPERSION_NODE)) {
            pstFiberPara->dDispersion = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, FIBER_LOSS_NODE)) {
            pstFiberPara->dFiberLoss = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, LAUNCHING_POWER_STR_NODE)) {
            pstFiberPara->dLauchingPower = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, SUPPORTED_SPAN_lOSS_NODE)) {
            pstFiberPara->dSupportedSpanLoss = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, SIGNAL_LOSS_TILT_NODE)) {
            pstFiberPara->dSignalLossTilt = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, OSC_LOSS_TILT_NODE)) {
            pstFiberPara->dOscLossTilt = output[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(output[i].xpath, SRS_K_NODE)) {
            pstFiberPara->dSrsK = output[i].data.decimal64_val;
        } else {
            /* ignore */
        }
    }

    stFiberList.uiTotal = j + 1;
    memcpy(pstFiberList, &stFiberList, sizeof(stFiberList));

END_LABEL:
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);
    return rc;

}

int
DSC_AutoStartPowerCtrl(char *pcLine, char *pcDir)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL;
    size_t input_cnt = 0;
    sr_val_t *output = NULL;
    size_t output_cnt = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* allocate input values path*/
    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        goto END_LABEL;
    }

    /* set input/path leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_AUTO_POWER_PATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_AUTO_POWER_PATH, sr_strerror(rc));
        goto END_LABEL;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcLine);

    /* set input/direction leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_AUTO_POWER_DIR);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_AUTO_POWER_DIR, sr_strerror(rc));
        goto END_LABEL;
    }

    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcDir);

    rc = DSC_RpcSend(pstSess, RPC_XPATH_AUTO_POWER, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_AUTO_POWER, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_ProcessRPCResponse(output, output_cnt);

END_LABEL:
    sr_free_values(input, input_cnt);
    LOG_WRITE(EN_LOG_NOTICE, "%s (rc:%d)", __func__, rc);
    return rc;
}
