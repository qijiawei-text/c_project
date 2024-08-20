
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_ops.h"
#include "DSC_port.h"
#include "DSC_tca.h"

static int
__GetOpsTcaIdx(char *pcCfgKey, size_t nItemIndex, int iBaseType)
{
    int iOpsUnit = (TCA_IDX_OPS_END - TCA_IDX_OPS_START + 1) / (OPS_DEV_NUM_MAX + 1);
    return 0 == strcmp(pcCfgKey, OPS_CFG_KEY) ? iBaseType : (iBaseType + iOpsUnit * (nItemIndex + 1));
}

int
DSC_GetOpsCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = DSC_GetKeyValue(pcXpath, "aps-module", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_SetForceSwitch(char *pcCfgKey, char *pcTargetPath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(xpath, OPLK_PATH_MAX_LEN, OPS_XPATH_FORCE_TO_PORT, pcCfgKey);

    rc = set_enum_item(pstSess, xpath, pcTargetPath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OPS revertive fail: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_RpcSetManualSwitch(char *pcCfgKey, char *pcTargetPath)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_OPS_TARGET);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_OPS_TARGET, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(pcTargetPath);

    rc = sr_val_set_xpath(&input[1], RPC_XPATH_OPS_MESSAGE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_OPS_TARGET, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcCfgKey);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_OPS_MANUAL_TO_PORT, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_OPS_MANUAL_TO_PORT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (strcmp(output[0].data.string_val, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[0].xpath, output[0].data.string_val);
        rc = SR_ERR_OPERATION_FAILED;
    }

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_SetOpsRevertive(char *pcCfgKey, bool bRevertive)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_REVERTIVE, pcCfgKey);

    LOG_WRITE(EN_LOG_NOTICE, "Set ops revertive %s", bRevertive ? "true" : "false");

    rc = set_bool_item(sess, acXpath, bRevertive);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_SetOpsPortSwitchThr(char *pcCfgKey, OPS_PATH_EN enOpsPath, double dThreshold)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (PRIMARY_PATH == enOpsPath) {
        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_PRIMARY_SWITCH_THR, pcCfgKey);
    } else if (SECONDARY_PATH == enOpsPath) {
        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_SECONDARY_SWITCH_THR, pcCfgKey);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enOpsPath: %d", enOpsPath);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_decimal64_item(pstSess, acXpath, dThreshold);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetOpsPortSwitchThrHys(char *pcCfgKey, OPS_PATH_EN enOpsPath, double dHys)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (PRIMARY_PATH == enOpsPath) {
        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_PRIMARY_SWITCH_HYS, pcCfgKey);
    } else if (SECONDARY_PATH == enOpsPath) {
        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_SECONDARY_SWITCH_HYS, pcCfgKey);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enOpsPath: %d", enOpsPath);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_decimal64_item(pstSess, acXpath, dHys);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetOpsRelativeThr(char *pcCfgKey, double dThreshold)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_RELATIVE_SWITCH_THR, pcCfgKey);

    rc = set_decimal64_item(pstSess, acXpath, dThreshold);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetOpsRelativeThrOffset(char *pcCfgKey, double dOffset)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};


    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_RELATIVE_SWITCH_THR_OFFSET, pcCfgKey);

    rc = set_decimal64_item(pstSess, acXpath, dOffset);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

/**
 * @brief
 *
 * @param [in]
 * @param [in] iOpsWtr, the unit is milliseconds
 * @return
 */
int
DSC_SetOpsWtr(char *pcCfgKey, uint32_t uiOpsWtr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_WTR, pcCfgKey);

    rc = set_uint32_item(pstSess, acXpath, uiOpsWtr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetOpsHoldOffTime(char *pcCfgKey, uint32_t uiHoldOff)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_HOLD_OFF_TIME, pcCfgKey);

    rc = set_uint32_item(pstSess, acXpath, uiHoldOff);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_OpsSwitchPortEnumToStr(OPS_SWITCH_PORT_EN enForceToPort, char *pcPathStr, size_t nSize)
{
    int rc = SR_ERR_OK;
    if (NULL == pcPathStr) {
        return SR_ERR_INVAL_ARG;
    }

    switch (enForceToPort) {
    case SWITCH_TO_NONE:
        snprintf(pcPathStr, nSize, "%s", STR_NONE);
        break;
    case SWITCH_TO_PRIMARY:
        snprintf(pcPathStr, nSize, "%s", STR_PRIMARY);
        break;
    case SWITCH_TO_SECONDARY:
        snprintf(pcPathStr, nSize, "%s", STR_SECONDARY);
        break;
    default:
        rc = SR_ERR_NOT_FOUND;
        break;
    }

    return rc;
}

OPS_SWITCH_PORT_EN
DSC_OpsSwitchPortStrToEnum(char *pcPathStr)
{
    OPS_SWITCH_PORT_EN enForceToPort = SWITCH_TO_PORT_MAX;

    if (NULL == pcPathStr) {
        return SWITCH_TO_PORT_MAX;
    }

    if (0 == strcasecmp(pcPathStr, STR_NONE)) {
        enForceToPort = SWITCH_TO_NONE;
    } else if (0 == strcasecmp(pcPathStr, STR_PRIMARY)) {
        enForceToPort = SWITCH_TO_PRIMARY;
    } else if (0 == strcasecmp(pcPathStr, STR_SECONDARY)) {
        enForceToPort = SWITCH_TO_SECONDARY;
    } else {
        enForceToPort = SWITCH_TO_PORT_MAX;
    }

    return enForceToPort;
}

int
DSC_OpsPathEnumToStr(OPS_PATH_EN enOpsPath, char *pcPathStr, size_t nSize)
{
    int rc = SR_ERR_OK;
    if (NULL == pcPathStr) {
        return SR_ERR_INVAL_ARG;
    }

    switch (enOpsPath) {
    case PRIMARY_PATH:
        snprintf(pcPathStr, nSize, "%s", STR_PRIMARY);
        break;
    case SECONDARY_PATH:
        snprintf(pcPathStr, nSize, "%s", STR_SECONDARY);
        break;
    default:
        rc = SR_ERR_NOT_FOUND;
        break;
    }

    return rc;
}

OPS_PATH_EN
DSC_OpsPathToEnum(char *pcPathStr)
{
    OPS_PATH_EN enOpsPath = OPS_PATH_MAX;

    if (NULL == pcPathStr) {
        return OPS_PATH_MAX;
    }

    if (NULL != strstr(pcPathStr, STR_PRIMARY)) {
        enOpsPath = PRIMARY_PATH;
    } else if (NULL != strstr(pcPathStr, STR_SECONDARY)) {
        enOpsPath = SECONDARY_PATH;
    } else {
        enOpsPath = OPS_PATH_MAX;
    }

    return enOpsPath;
}

OPS_SWITCHING_TYPE_EN
DSC_OpsSwitchingTypeStrToEnum(char *pcTypeStr)
{
    OPS_SWITCHING_TYPE_EN enSwitchingType = SWITCHING_TYPE_UNI;

    if (NULL == pcTypeStr) {
        return SWITCHING_TYPE_UNI;
    }

    if (0 == strcasecmp(pcTypeStr, STR_UNIDIRECTIONAL)) {
        enSwitchingType = SWITCHING_TYPE_UNI;
    } else if (0 == strcasecmp(pcTypeStr, STR_BIDIRECTIONAL)) {
        enSwitchingType = SWITCHING_TYPE_BI;
    }

    return enSwitchingType;
}

int
DSC_GetOpsConfig(char *pcCfgKey, OPS_CONFIG_ST *pstOpsCfg, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char aacXpath[16][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acForceToPort[DEF_BUF_SIZE_32] = {0};
    char acSwitchingType[DEF_BUF_SIZE_32] = {0};

    if (NULL == pcCfgKey || NULL == pstOpsCfg || nSize != sizeof(OPS_CONFIG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", nSize);
        return SR_ERR_INVAL_ARG;
    }

    memset(aacXpath, 0, sizeof(aacXpath));

    ANONYMOUS_ST astOpsMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], OPS_XPATH_MODULE_NAME, pcCfgKey), SR_STRING_T, pstOpsCfg->acName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], OPS_XPATH_REVERTIVE, pcCfgKey), SR_BOOL_T, (char *)(&pstOpsCfg->bRevertive)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[3], OPS_XPATH_WTR, pcCfgKey), SR_UINT32_T, (char *)(&pstOpsCfg->uiWtr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[4], OPS_XPATH_HOLD_OFF_TIME, pcCfgKey), SR_UINT32_T, (char *)(&pstOpsCfg->uiHoldOffTime)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[5], OPS_XPATH_PRIMARY_SWITCH_THR, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dPrimarySwitchThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[6], OPS_XPATH_PRIMARY_SWITCH_HYS, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dPrimaryHys)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[7], OPS_XPATH_SECONDARY_SWITCH_THR, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dSecondarySwitchThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[8], OPS_XPATH_SECONDARY_SWITCH_HYS, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dSecondaryHys)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[9], OPS_XPATH_RELATIVE_SWITCH_THR, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dRelativeThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[10], OPS_XPATH_RELATIVE_SWITCH_THR_OFFSET, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstOpsCfg->dRelativeThrOffset)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[11], OPS_XPATH_FORCE_TO_PORT, pcCfgKey), SR_ENUM_T, acForceToPort),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[12], OPS_XPATH_SWITCHING_TYPE, pcCfgKey), SR_ENUM_T, acSwitchingType)
    };

    memset(pstOpsCfg, 0, sizeof(OPS_CONFIG_ST));

    pstSess = DSC_GetRunningSession();
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_MODULES_CFG_ALL, pcCfgKey);
    rc = DSC_GetItems(pstSess, acSelectXpath, astOpsMaps, sizeof(astOpsMaps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    pstOpsCfg->enForceToPort = DSC_OpsSwitchPortStrToEnum(acForceToPort);
    if (SWITCH_TO_PORT_MAX == pstOpsCfg->enForceToPort) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enForceToPort: %s", acForceToPort);
        rc = SR_ERR_INVAL_ARG;
    }

    pstOpsCfg->enSwitchingType = DSC_OpsSwitchingTypeStrToEnum(acSwitchingType);
    if (SWITCHING_TYPE_MAX == pstOpsCfg->enSwitchingType) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enSwitchingType: %s", acSwitchingType);
        rc = SR_ERR_INVAL_ARG;
    }

END_LABEL:

    return rc;
}

int
DSC_GetOpsState(char *pcCfgKey, OPS_STATE_ST *pstOpsState, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char aacXpath[16][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char aacEnum[3][DEF_BUF_SIZE_64];
    int iLen = 0;

    if (NULL == pcCfgKey || NULL == pstOpsState || nSize != sizeof(OPS_STATE_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", nSize);
        return SR_ERR_INVAL_ARG;
    }

    memset(aacXpath, 0, sizeof(aacXpath));

    ANONYMOUS_ST astOpsMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], OPS_ACTIVE_PATH, pcCfgKey), SR_IDENTITYREF_T, aacEnum[0]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], OPS_SWITCH_STATE, pcCfgKey), SR_ENUM_T, aacEnum[1]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[3], OPS_SWITCHING_TYPE, pcCfgKey), SR_ENUM_T, aacEnum[2])
    };

    memset(pstOpsState, 0, sizeof(OPS_STATE_ST));
    memset(aacEnum, 0, sizeof(aacEnum));

    pstSess = DSC_GetOperSession();
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_MODULES_STATE_ALL, pcCfgKey);
    rc = DSC_GetItems(pstSess, acSelectXpath, astOpsMaps, sizeof(astOpsMaps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto END_LABEL;
    }


    /* parse active path */
    pstOpsState->enActivePath = DSC_OpsPathToEnum(aacEnum[0]);
    if (OPS_PATH_MAX == pstOpsState->enActivePath) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid ActivePath: %s", aacEnum[0]);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /* parse switch state */
    iLen = snprintf(pstOpsState->acSwitchState, sizeof(pstOpsState->acSwitchState), "%s", aacEnum[1]);
    if (iLen < 0) {
        LOG_WRITE(EN_LOG_ERROR, "aacEnum[1]: %s directive output may be truncated", aacEnum[1]);
    }

    /* parse switching type */
    iLen = snprintf(pstOpsState->acSwitchingType, sizeof(pstOpsState->acSwitchingType), "%s", aacEnum[2]);
    if (iLen < 0) {
        LOG_WRITE(EN_LOG_ERROR, "aacEnum[2]: %s directive output may be truncated", aacEnum[2]);
    }

END_LABEL:
    return rc;
}

int
DSC_GetOpsPortConfig(char *pcCfgKey, OPS_PORT_EN enOpsPort, OPS_PORT_CFG_ST *pstOpsPortCfg, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char aacXpath[16][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acPortName[DEF_BUF_SIZE_32] = {0};

    if (NULL == pcCfgKey || NULL == pstOpsPortCfg || nSize != sizeof(OPS_PORT_CFG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", nSize);
        return SR_ERR_INVAL_ARG;
    }

    /* parse port name */
    rc = DSC_OpsPortEnumToStr(enOpsPort, acPortName, sizeof(acPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enOpsPort: %d", enOpsPort);
        goto END_LABEL;
    }

    memset(aacXpath, 0, sizeof(aacXpath));
    snprintf(aacXpath[1], sizeof(aacXpath[1]), OPS_PORT_CONFIG_ENABELED, pcCfgKey, acPortName);

    ANONYMOUS_ST astOpsMaps[] = {
        ANONYMOUS_MAP_ENTRY(aacXpath[1], SR_BOOL_T, (char *)(&pstOpsPortCfg->bEnable))
    };

    memset(pstOpsPortCfg, 0, sizeof(OPS_PORT_CFG_ST));

    pstSess = DSC_GetOperSession();
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_PORTS_CONFIG_ALL, pcCfgKey, acPortName);

    rc = DSC_GetItems(pstSess, acSelectXpath, astOpsMaps, sizeof(astOpsMaps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}


int
DSC_GetOpsPortState(char *pcCfgKey, OPS_PORT_EN enOpsPort, OPS_PORT_STATE_ST *pstOpsPortState, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char aacXpath[16][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acPortName[DEF_BUF_SIZE_32] = {0};

    if (NULL == pcCfgKey || NULL == pstOpsPortState || nSize != sizeof(OPS_PORT_STATE_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", nSize);
        return SR_ERR_INVAL_ARG;
    }

    /* parse port name */
    rc = DSC_OpsPortEnumToStr(enOpsPort, acPortName, sizeof(acPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enOpsPort: %d", enOpsPort);
        goto END_LABEL;
    }

    memset(aacXpath, 0, sizeof(aacXpath));
    snprintf(aacXpath[1], sizeof(aacXpath[1]), OPS_PORT_STATE_ENABELED, pcCfgKey, acPortName);
    snprintf(aacXpath[2], sizeof(aacXpath[2]), OPS_PORT_POWER_INSTANT, pcCfgKey, acPortName);

    ANONYMOUS_ST astOpsMaps[] = {
        ANONYMOUS_MAP_ENTRY(aacXpath[1], SR_BOOL_T, (char *)(&pstOpsPortState->bEnable)),
        ANONYMOUS_MAP_ENTRY(aacXpath[2], SR_DECIMAL64_T, (char *)(&pstOpsPortState->stPortPwr.dInstant))
    };

    memset(pstOpsPortState, 0, sizeof(OPS_PORT_STATE_ST));

    pstSess = DSC_GetOperSession();
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OPS_XPATH_PORTS_STATE_ALL, pcCfgKey, acPortName);
    rc = DSC_GetItems(pstSess, acSelectXpath, astOpsMaps, sizeof(astOpsMaps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

int
DSC_OpsPortEnumToStr(OPS_PORT_EN enOpsPort, char *pcOpsPortName, size_t nSize)
{
    int rc = SR_ERR_OK;
    if (NULL == pcOpsPortName) {
        return SR_ERR_INVAL_ARG;
    }

    switch (enOpsPort) {
    case OPS_PORT_LINE_PRIMARY_IN:
        snprintf(pcOpsPortName, nSize, "%s", STR_LINE_PRIMARY_IN);
        break;
    case OPS_PORT_LINE_PRIMARY_OUT:
        snprintf(pcOpsPortName, nSize, "%s", STR_LINE_PRIMARY_OUT);
        break;
    case OPS_PORT_LINE_SECONDARY_IN:
        snprintf(pcOpsPortName, nSize, "%s", STR_LINE_SECONDARY_IN);
        break;
    case OPS_PORT_LINE_SECONDARY_OUT:
        snprintf(pcOpsPortName, nSize, "%s", STR_LINE_SECONDARY_OUT);
        break;
    case OPS_PORT_COMMON_IN:
        snprintf(pcOpsPortName, nSize, "%s", STR_COMMON_IN);
        break;
    case OPS_PORT_COMMON_OUTPUT:
        snprintf(pcOpsPortName, nSize, "%s", STR_COMMON_OUTPUT);
        break;
    default:
        rc = SR_ERR_NOT_FOUND;
        break;
    }

    return rc;
}

OPS_PORT_EN
DSC_OpsPortStrToEnum(char *pcOpsPortName)
{
    OPS_PORT_EN enOpsPort = OPS_PORT_MAX;

    if (NULL == pcOpsPortName) {
        return OPS_PORT_MAX;
    }

    if (0 == strcasecmp(pcOpsPortName, STR_LINE_PRIMARY_IN)) {
        enOpsPort = OPS_PORT_LINE_PRIMARY_IN;
    } else if (0 == strcasecmp(pcOpsPortName, STR_LINE_PRIMARY_OUT)) {
        enOpsPort = OPS_PORT_LINE_PRIMARY_OUT;
    } else if (0 == strcasecmp(pcOpsPortName, STR_LINE_SECONDARY_IN)) {
        enOpsPort = OPS_PORT_LINE_SECONDARY_IN;
    } else if (0 == strcasecmp(pcOpsPortName, STR_LINE_SECONDARY_OUT)) {
        enOpsPort = OPS_PORT_LINE_SECONDARY_OUT;
    } else if (0 == strcasecmp(pcOpsPortName, STR_COMMON_IN)) {
        enOpsPort = OPS_PORT_COMMON_IN;
    } else if (0 == strcasecmp(pcOpsPortName, STR_COMMON_OUTPUT)) {
        enOpsPort = OPS_PORT_COMMON_OUTPUT;
    } else {
        enOpsPort = OPS_PORT_MAX;
    }

    return enOpsPort;
}

int
DSC_SetOpsPortEnable(char *pcCfgKey, OPS_PORT_EN enOpsPort, bool bEnable)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acPortName[DEF_BUF_SIZE_32] = {0};

    /* parse port name */
    rc = DSC_OpsPortEnumToStr(enOpsPort, acPortName, sizeof(acPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid enOpsPort: %d", enOpsPort);
        goto END_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_PORT_CONFIG_ENABELED, pcCfgKey, acPortName);

    /* LOG_WRITE(EN_LOG_NOTICE, "Set ops revertive %s", bEnable ? "true" : "false"); */

    rc = set_bool_item(pstSess, acXpath, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_GetOpsNames(char *pcNames, size_t nNameSize, size_t *pnCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    const char *xpath = OPS_XPATH_MODULE_NAME;
    sr_val_t *values = NULL;
    size_t values_count = 0;

    rc = get_items(sess, xpath, &values, &values_count);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s %s", xpath, sr_strerror(rc));
        return rc;
    }

    if (NULL != pnCount) {
        *pnCount = values_count;
    }

    if (NULL != pcNames) {
        for (size_t i = 0; i < values_count; i++) {
            strncpy(pcNames + i * nNameSize, values[i].data.string_val, nNameSize);
        }
    }

    if (values != NULL) {
        sr_free_values(values, values_count);
    }

    return rc;
}

int
DSC_SetOpsTcaValue(char *pcCfgKey, size_t nItemIndex, int iBaseType, bool bHys, double dblValue)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *tcaInfo = NULL;
    size_t nCount = 0;
    double dblHighThr = 0.0, dblLowThr = 0.0;
    int iTcaId = __GetOpsTcaIdx(pcCfgKey, nItemIndex, iBaseType);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    rc = DSC_GetTcaList(sess, &tcaInfo, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetTcaList: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_GetTcaThresh(iTcaId, tcaInfo, &dblHighThr, &dblLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetTcaThresh: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (bHys) {
        if (iBaseType == TCA_IDX_OPS_POWER_DIFFER ||
            iBaseType == TCA_IDX_OPS_COM_IN_HIGH ||
            iBaseType == TCA_IDX_OPS_PRI_IN_HIGH ||
            iBaseType == TCA_IDX_OPS_SEC_IN_HIGH) {
            dblLowThr = dblHighThr - dblValue;
        } else {
            dblHighThr = dblLowThr + dblValue;
        }
    } else {
        if (iBaseType == TCA_IDX_OPS_POWER_DIFFER ||
            iBaseType == TCA_IDX_OPS_COM_IN_HIGH ||
            iBaseType == TCA_IDX_OPS_PRI_IN_HIGH ||
            iBaseType == TCA_IDX_OPS_SEC_IN_HIGH) {
            dblLowThr += dblValue - dblHighThr;
            dblHighThr = dblValue;
        } else {
            dblHighThr += dblValue - dblLowThr;
            dblLowThr = dblValue;
        }
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dblHighThr, dblLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error ops tca thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetOpsTcaValue(char *pcCfgKey, size_t nItemIndex, int iBaseType, double *pdblThr, double *pdblHys)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *tcaInfo = NULL;
    size_t nCount = 0;
    double dblHighThr = 0.0, dblLowThr = 0.0;
    int iTcaId = __GetOpsTcaIdx(pcCfgKey, nItemIndex, iBaseType);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    rc = DSC_GetTcaList(sess, &tcaInfo, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetTcaList: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_GetTcaThresh(iTcaId, tcaInfo, &dblHighThr, &dblLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetTcaThresh: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    *pdblHys = dblHighThr - dblLowThr;
    *pdblThr = iBaseType == TCA_IDX_OPS_POWER_DIFFER ? dblHighThr : dblLowThr;

ERR_LABEL:
    return rc;
}