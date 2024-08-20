/**
 * Copyright (c) Oplink Communications, LLC
 * fandy <Dongyang.Fan@Molex.com>
 * Jun. 28th, 2016
 */

#include <stdarg.h>
#include "DSC_edfa.h"
#include "DSC_osc.h"
#include "DSC_tca.h"

AMP_FSM_MODE_EN
DSC_EdfaFsmModeToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return AMP_FSM_MODE_MAX;
    }

    if (0 == strncmp(pcStr, AMP_FSM_MODE_AUTO_STR, strlen(AMP_FSM_MODE_AUTO_STR))) {
        return AMP_FSM_MODE_AUTO;
    } else if (0 == strncmp(pcStr, AMP_FSM_MODE_MANUAL_STR, strlen(AMP_FSM_MODE_MANUAL_STR))) {
        return AMP_FSM_MODE_MANUAL;
    } else {
        return AMP_FSM_MODE_MAX;
    }
}

char *
DSC_EdfaFsmModeToStr(char *pcStr, AMP_FSM_MODE_EN enMode)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enMode) {
    case AMP_FSM_MODE_AUTO:
        strcpy(pcStr, AMP_FSM_MODE_AUTO_STR);
        break;
    case AMP_FSM_MODE_MANUAL:
        strcpy(pcStr, AMP_FSM_MODE_MANUAL_STR);
        break;
    default:
        strcpy(pcStr, AMP_FSM_MODE_ERR_STR);
        break;
    }

    return pcStr;
}

AMP_FSM_STATE_EN
DSC_EdfaFsmStateToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return AMP_FSM_STATE_MAX;
    }

    if (0 == strncmp(pcStr, AMP_FSM_STATE_DISABLE_STR, strlen(AMP_FSM_STATE_DISABLE_STR))) {
        return AMP_FSM_STATE_DISABLE;
    } else if (0 == strncmp(pcStr, AMP_FSM_STATE_NORMAL_STR, strlen(AMP_FSM_STATE_NORMAL_STR))) {
        return AMP_FSM_STATE_NORMAL;
    } else if (0 == strncmp(pcStr, AMP_FSM_STATE_SAFETY_STR, strlen(AMP_FSM_STATE_SAFETY_STR))) {
        return AMP_FSM_STATE_SAFETY;
    } else if (0 == strncmp(pcStr, AMP_FSM_STATE_READY_STR, strlen(AMP_FSM_STATE_READY_STR))) {
        return AMP_FSM_STATE_READY;
    } else {
        return AMP_FSM_STATE_MAX;
    }
}

char *
DSC_EdfaFsmStateToStr(char *pcStr, AMP_FSM_STATE_EN enState)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enState) {
    case AMP_FSM_STATE_DISABLE:
        strcpy(pcStr, AMP_FSM_STATE_DISABLE_STR);
        break;
    case AMP_FSM_STATE_NORMAL:
        strcpy(pcStr, AMP_FSM_STATE_NORMAL_STR);
        break;
    case AMP_FSM_STATE_SAFETY:
        strcpy(pcStr, AMP_FSM_STATE_SAFETY_STR);
        break;
    case AMP_FSM_STATE_READY:
        strcpy(pcStr, AMP_FSM_STATE_READY_STR);
        break;
    default:
        strcpy(pcStr, AMP_FSM_STATE_ERR_STR);
        break;
    }

    return pcStr;
}

static EDFA_GAIN_RANGE_EN
__edfaGainrangeToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return EDFA_GAIN_RANGE_LOW;
    }

    if (0 == strncmp(pcStr, DEF_AMP_LOW_GAIN_STR, strlen(DEF_AMP_LOW_GAIN_STR))) {
        return EDFA_GAIN_RANGE_LOW;
    } else if (0 == strncmp(pcStr, DEF_AMP_HIGH_GAIN_STR, strlen(DEF_AMP_HIGH_GAIN_STR))) {
        return EDFA_GAIN_RANGE_HIGH;
    } else if (0 == strncmp(pcStr, DEF_AMP_MID_GAIN_STR, strlen(DEF_AMP_MID_GAIN_STR))) {
        return EDFA_GAIN_RANGE_MID;
    } else if (0 == strncmp(pcStr, DEF_AMP_FIXED_GAIN_STR, strlen(DEF_AMP_FIXED_GAIN_STR))) {
        return EDFA_GAIN_FIXED;
    }

    return EDFA_GAIN_RANGE_LOW;
}

static EDFA_STATE_EN
__edfaStateToEnum(char *pcStr)
{
    if (0 == strncmp(pcStr, EDFA_STATE_NORMAL_STR, strlen(EDFA_STATE_NORMAL_STR))) {
        return EDFA_STATE_NORMAL;
    } else if (0 == strncmp(pcStr, EDFA_STATE_DISABLED_STR, strlen(EDFA_STATE_DISABLED_STR))) {
        return EDFA_STATE_DISABLED;
    } else if (0 == strncmp(pcStr, EDFA_STATE_AUTO_STC_STR, strlen(EDFA_STATE_AUTO_STC_STR))) {
        return EDFA_STATE_AUTO_STC;
    } else if (0 == strncmp(pcStr, EDFA_STATE_STANDBY_STR, strlen(EDFA_STATE_STANDBY_STR))) {
        return EDFA_STATE_STANDBY;
    } else if (0 == strncmp(pcStr, EDFA_STATE_PLIM_STR, strlen(EDFA_STATE_PLIM_STR))) {
        return EDFA_STATE_PLIM;
    } else if (0 == strncmp(pcStr, EDFA_STATE_GLIM_STR, strlen(EDFA_STATE_GLIM_STR))) {
        return EDFA_STATE_GLIM;
    } else if (0 == strncmp(pcStr, EDFA_STATE_APR_STR, strlen(EDFA_STATE_APR_STR))) {
        return EDFA_STATE_APR;
    }

    return EDFA_STATE_NORMAL;
}

EDFA_MODE_EN
DSC_EdfaModeToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return EDFA_MODE_AGC;
    }

    if (NULL != strstr(pcStr, AMP_MODE_CONSTANT_CURRENT)) {
        return EDFA_MODE_ACC;
    } else if (NULL != strstr(pcStr, AMP_MODE_CONSTANT_POWER)) {
        return EDFA_MODE_APC;
    } else {
        return EDFA_MODE_AGC;
    }

    return EDFA_MODE_AGC;
}

EDFA_GAIN_RANGE_EN
DSC_EdfaGainRangeToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return EDFA_GAIN_RANGE_ERROR;
    }

    if (NULL != strstr(pcStr, DEF_AMP_LOW_GAIN_STR)) {
        return EDFA_GAIN_RANGE_LOW;
    } else if (NULL != strstr(pcStr, DEF_AMP_HIGH_GAIN_STR)) {
        return EDFA_GAIN_RANGE_HIGH;
        /* system not support EDFA_GAIN_RANGE_MID and EDFA_GAIN_FIXED now     */
#if 0
    } else if (NULL != strstr(pcStr, DEF_AMP_MID_GAIN_STR)) {
        return EDFA_GAIN_RANGE_MID;
    } else if (NULL != strstr(pcStr, DEF_AMP_FIXED_GAIN_STR)) {
        return EDFA_GAIN_FIXED;
#endif
    }

    return EDFA_GAIN_RANGE_ERROR;
}

AMP_TYPE_EN
DSC_AmpTypeToEnum(char *pcStr)
{
    if (NULL == pcStr) {
        return AMP_TYPE_EDFA;
    }

    if (0 == strncmp(pcStr, AMP_TYPE_EDFA_STR, strlen(AMP_TYPE_EDFA_STR))) {
        return AMP_TYPE_EDFA;
    } else if (0 == strncmp(pcStr, AMP_TYPE_FORWARD_RAMAN_STR, strlen(AMP_TYPE_FORWARD_RAMAN_STR))) {
        return AMP_TYPE_FORWARD_RAMAN;
    } else if (0 == strncmp(pcStr, AMP_TYPE_BACKWARD_RAMAN_STR, strlen(AMP_TYPE_BACKWARD_RAMAN_STR))) {
        return AMP_TYPE_BACKWARD_RAMAN;
    }

    return AMP_TYPE_EDFA;
}

char *
DSC_EdfaGainrangeToStr(char *pcStr, EDFA_GAIN_RANGE_EN enGainrange)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enGainrange) {
    case EDFA_GAIN_RANGE_LOW:
        strcpy(pcStr, DEF_AMP_LOW_GAIN_STR);
        break;
    case EDFA_GAIN_RANGE_HIGH:
        strcpy(pcStr, DEF_AMP_HIGH_GAIN_STR);
        break;
    case EDFA_GAIN_RANGE_MID:
        strcpy(pcStr, DEF_AMP_MID_GAIN_STR);
        break;
    case EDFA_GAIN_FIXED:
        strcpy(pcStr, DEF_AMP_FIXED_GAIN_STR);
        break;
    default:
        strcpy(pcStr, DEF_AMP_LOW_GAIN_STR);
        break;
    }

    return pcStr;
}

char *
DSC_EdfaModeToStr(EDFA_MODE_EN enMode, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enMode) {
    case EDFA_MODE_AGC:
        snprintf(pcBuf, nSize, "%s", "openconfig-optical-amplifier:" AMP_MODE_CONSTANT_GAIN);
        break;
    case EDFA_MODE_APC:
        snprintf(pcBuf, nSize, "%s", "openconfig-optical-amplifier:" AMP_MODE_CONSTANT_POWER);
        break;
    case EDFA_MODE_ACC:
        snprintf(pcBuf, nSize, "%s", "com-oplink-amplifier-ext:" AMP_MODE_CONSTANT_CURRENT);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}

char *
DSC_AmpTypeToStr(char *pcStr, AMP_TYPE_EN enMode)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enMode) {
    case AMP_TYPE_EDFA:
        strcpy(pcStr, AMP_TYPE_EDFA_STR);
        break;
    case AMP_TYPE_FORWARD_RAMAN:
        strcpy(pcStr, AMP_TYPE_FORWARD_RAMAN_STR);
        break;
    case AMP_TYPE_BACKWARD_RAMAN:
        strcpy(pcStr, AMP_TYPE_BACKWARD_RAMAN_STR);
        break;
    default:
        strcpy(pcStr, AMP_TYPE_EDFA_STR);
        break;
    }

    return pcStr;
}

char *
DSC_EdfaStateToStr(char *pcStr, EDFA_STATE_EN enState)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enState) {
    case EDFA_STATE_NORMAL:
        strcpy(pcStr, EDFA_STATE_NORMAL_STR);
        break;
    case EDFA_STATE_DISABLED:
        strcpy(pcStr, EDFA_STATE_DISABLED_STR);
        break;
    case EDFA_STATE_AUTO_STC:
        strcpy(pcStr, EDFA_STATE_AUTO_STC_STR);
        break;
    case EDFA_STATE_STANDBY:
        strcpy(pcStr, EDFA_STATE_STANDBY_STR);
        break;
    case EDFA_STATE_PLIM:
        strcpy(pcStr, EDFA_STATE_PLIM_STR);
        break;
    case EDFA_STATE_GLIM:
        strcpy(pcStr, EDFA_STATE_GLIM_STR);
        break;
    case EDFA_STATE_APR:
        strcpy(pcStr, EDFA_STATE_APR_STR);
        break;
    default:
        strcpy(pcStr, EDFA_STATE_NORMAL_STR);
        break;
    }

    return pcStr;
}

int
DSC_GetEdfaCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "amplifier", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

int
DSC_GetEdfaCfgKeyFromDb(EDFA_KEY_ST *pstEdfaKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;

    if (NULL == pstEdfaKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstEdfaKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetStartupSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, AMP_XPATH_NAME_ALL, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        strncpy(pstEdfaKey->aacEdfaKeys[i], pstValues[i].data.string_val, sizeof(pstEdfaKey->aacEdfaKeys[i]) - 1);
    }
    pstEdfaKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

static int
__GetEdfaConfig(char *pcCfgKey, EDFA_CFG_ST *pstEdfaCfg, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char aacEnum[4][64];
    char *p = NULL;

    sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == pcCfgKey || NULL == pstEdfaCfg || size < sizeof(EDFA_CFG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset(xpath, 0, sizeof(xpath));
    memset(aacEnum, 0, sizeof(aacEnum));

    ANONYMOUS_ST edfa_maps[23] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], AMP_XPATH_MODE, pcCfgKey), SR_IDENTITYREF_T, aacEnum[0]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], AMP_XPATH_GAIN_RANGE, pcCfgKey), SR_IDENTITYREF_T, aacEnum[1]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], AMP_XPATH_NAME, pcCfgKey), SR_STRING_T, pstEdfaCfg->acName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], AMP_XPATH_TYPE, pcCfgKey), SR_IDENTITYREF_T, aacEnum[2]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], AMP_XPATH_TARGET_POWER, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dTargetPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], AMP_XPATH_GAIN, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dGainSetPoint)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], AMP_XPATH_TILT, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dTiltSetPoint)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], AMP_XPATH_APR_POWER, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dAprPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[9], AMP_XPATH_AUTOLOS_ENABLE, pcCfgKey), SR_BOOL_T, (char *)(&pstEdfaCfg->bAutoLosEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[10], AMP_XPATH_APR_ENABLE, pcCfgKey), SR_BOOL_T, (char *)(&pstEdfaCfg->bAprEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[11], AMP_XPATH_ENABLE, pcCfgKey), SR_BOOL_T, (char *)(&pstEdfaCfg->bOutputEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[12], AMP_XPATH_PLIM_ENABLE, pcCfgKey), SR_BOOL_T, (char *)(&pstEdfaCfg->bPlimEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[13], AMP_XPATH_PLIM_POWER, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dPlimPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[14], AMP_XPATH_NUM_OF_PUMPS, pcCfgKey), SR_UINT8_T, (char *)(&pstEdfaCfg->ucNumOfPumps)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], AMP_CFG_FSM_MODE, pcCfgKey), SR_ENUM_T, aacEnum[3]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[16], AMP_CFG_READY_POWER, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dReadyPower)),
    };

    int j = 16, k = 17;
    for (int i = 0; i < 8; ++i) {
        ANONYMOUS_ST current = ANONYMOUS_MAP_ENTRY((sprintf(xpath[k], AMP_XPATH_CURRENT, pcCfgKey, i + 1), xpath[k++]),
                                                   SR_DECIMAL64_T, (char *)(&pstEdfaCfg->dLaserBiasCurrents[i]));
        memcpy(&edfa_maps[j++], &current, sizeof(ANONYMOUS_ST));
    }

    memset((void *)pstEdfaCfg, 0, size);
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_CFG_ALL, pcCfgKey);
    rc = DSC_GetItems(sess, acSelectXpath, edfa_maps, sizeof(edfa_maps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        return rc;
    }

    p = NULL;
    p = strtok(aacEnum[0], ":");
    p = strtok(NULL, ":");
    pstEdfaCfg->enMode = DSC_EdfaModeToEnum(p);
    p = NULL;
    p = strtok(aacEnum[1], ":");
    p = strtok(NULL, ":");
    pstEdfaCfg->enGainRange = __edfaGainrangeToEnum(p);
    p = NULL;
    p = strtok(aacEnum[2], ":");
    p = strtok(NULL, ":");
    pstEdfaCfg->enType = DSC_AmpTypeToEnum(p);
    p = NULL;

    pstEdfaCfg->enFsmMode = DSC_EdfaFsmModeToEnum(aacEnum[3]);
    LOG_WRITE(EN_LOG_NOTICE, "FsmMode: %s, %d", aacEnum[3], pstEdfaCfg->enFsmMode);

    return rc;
}

int
DSC_GetEdfaConfigWithoutTca(char *pcCfgKey, EDFA_CFG_ST *pstEdfaCfg, size_t size)
{
    return __GetEdfaConfig(pcCfgKey, pstEdfaCfg, size);
}

int
DSC_GetEdfaConfig(char *pcCfgKey, EDFA_CFG_ST *pstEdfaCfg, size_t size)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaInputId = 0;
    int iTcaEdfaOutputId = 0;
    int iTcaEdfaMsaInputId = 0;
    int iTcaEdfaMsaOutputId = 0;

    if (NULL == pcCfgKey || NULL == pstEdfaCfg || size < sizeof(EDFA_CFG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    rc = __GetEdfaConfig(pcCfgKey, pstEdfaCfg, size);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __GetEdfaConfig");
        return rc;
    }

    /* Get thresh from TCA */
    {
        int rc = SR_ERR_OK;
        TCA_INFO_ST *pstTcaInfo = NULL;
        size_t count = 0;

        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
            return rc;
        }

        if (0 == strcasecmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
            iTcaEdfaInputId = TCA_IDX_EDFA1_INPUT;
            iTcaEdfaOutputId = TCA_IDX_CLIENTT_LOS;
            iTcaEdfaMsaInputId = TCA_IDX_EDFA1_MSA_INPUT;
            iTcaEdfaMsaOutputId = TCA_IDX_EDFA1_MSA_OUTPUT;
        } else if (0 == strcasecmp(pcCfgKey, EDFA_BA_CFG_KEY)) {
            iTcaEdfaInputId = TCA_IDX_EDFA2_INPUT;
            iTcaEdfaOutputId = TCA_IDX_LINET_LOS;
            iTcaEdfaMsaInputId = TCA_IDX_EDFA2_MSA_INPUT;
            iTcaEdfaMsaOutputId = TCA_IDX_EDFA2_MSA_OUTPUT;
        } else if (0 == strcasecmp(pcCfgKey, EDFA_PAP_CFG_KEY)) {
            iTcaEdfaInputId = TCA_IDX_EDFA3_INPUT;
            iTcaEdfaOutputId = TCA_IDX_CLIENTT_P_LOS;
            iTcaEdfaMsaInputId = TCA_IDX_EDFA3_MSA_INPUT;
            iTcaEdfaMsaOutputId = TCA_IDX_EDFA3_MSA_OUTPUT;
        } else if (0 == strcasecmp(pcCfgKey, EDFA_BAP_CFG_KEY)) {
            iTcaEdfaInputId = TCA_IDX_EDFA4_INPUT;
            iTcaEdfaOutputId = TCA_IDX_LINET_P_LOS;
            iTcaEdfaMsaInputId = TCA_IDX_EDFA4_MSA_INPUT;
            iTcaEdfaMsaOutputId = TCA_IDX_EDFA4_MSA_OUTPUT;
        }

        DSC_GetTcaThresh(iTcaEdfaInputId,
                         pstTcaInfo, &(pstEdfaCfg->dInLosHighThr), &(pstEdfaCfg->dInLosLowThr));
        DSC_GetTcaThresh(iTcaEdfaOutputId,
                         pstTcaInfo, &(pstEdfaCfg->dOutLosHighThr), &(pstEdfaCfg->dOutLosLowThr));
        DSC_GetTcaThresh(iTcaEdfaMsaInputId,
                         pstTcaInfo, &(pstEdfaCfg->dMsaInHighThr), &(pstEdfaCfg->dMsaInLowThr));
        DSC_GetTcaThresh(iTcaEdfaMsaOutputId,
                         pstTcaInfo, &(pstEdfaCfg->dMsaOutHighThr), &(pstEdfaCfg->dMsaOutLowThr));
#if 0
        int i = 0;
        char acTmpStr[1024] = {0};
        for (i = 0; i < count; i++) {

            if (i > 0 && i % 2 == 0) {
                LOG_WRITE(EN_LOG_NOTICE, "%s", acTmpStr);
                memset(acTmpStr, 0, sizeof(acTmpStr));
            }
            snprintf(acTmpStr + strlen(acTmpStr), 1024 - strlen(acTmpStr), "(%d, %d, %s, %s, %.2f, %.2f)", i, pstTcaInfo[i].iIdx, pstTcaInfo[i].acId, pstTcaInfo[i].acPortName, pstTcaInfo[i].dLowThr, pstTcaInfo[i].dHighThr);
            /* LOG_WRITE(EN_LOG_NOTICE, "i: %d, iIdx: %d, acId: %s, acPortName: %s, dLowThr: %.2f, dHighThr: %.2f", i, pstTcaInfo->iIdx, pstTcaInfo->acId, pstTcaInfo->acPortName, pstTcaInfo->dLowThr, pstTcaInfo->dHighThr); */
        }

        LOG_WRITE(EN_LOG_NOTICE, "333333%s, count: %ld", acTmpStr, count);
#endif
    }

    return rc;
}


int
DSC_GetEdfaState(char *pcCfgKey, EDFA_STATE_ST *pstEdfaState, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char xpath[23][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char aacEnum[3][64];

    memset(xpath, 0, sizeof(xpath));
    memset(aacEnum, 0, sizeof(aacEnum));
    ANONYMOUS_ST edfa_maps[22] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], AMP_STATE_GAIN, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dGainValue)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], AMP_STATE_GAIN_TILT, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dTiltValue)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], AMP_STATE_IN_POWER_T, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dInputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], AMP_STATE_OUT_POWER_C, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dOutputSignalPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], AMP_STATE_OUT_POWER_T, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dOutputTotalPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], AMP_STATE_IN_POWER_C, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dInputSignalPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], AMP_STATE_IN_POWER_L, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dInputLPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], AMP_STATE_OUT_POWER_L, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dOutputLPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[9], AMP_STATE_LASER_BIAS, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dLaserBiasCurrent)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[10], AMP_STATE_REFLECTED_POWER, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dBackRefPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[11], AMP_STATE_OPTICAL_LOSS, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dOpticalReturnLoss)),

        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[12], AMP_STATE_OPT, pcCfgKey), SR_ENUM_T, aacEnum[0]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[13], AMP_STATE_FSM_MODE, pcCfgKey), SR_ENUM_T, aacEnum[1]),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[14], AMP_STATE_FSM_CURRENT, pcCfgKey), SR_ENUM_T, aacEnum[2]),
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], AMP_STATE_OPT, acAmpName), SR_ENUM_T, aacEnum[0]), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[16], AMP_STATE_PUMP_TEMPERATURE, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dEdfaPumpTemp)), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[17], AMP_STATE_MSA_INPUT_POWER_C, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dMsaInputPower)), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[18], AMP_STATE_MSA_OUT_POWER_C, pcCfgKey), SR_DECIMAL64_T, (char *)(&pstEdfaState->dMsaOutputPower)), */
    };

    int j = 14, k = 15;
    for (int i = 0; i < 8; ++i) {
        ANONYMOUS_ST current = ANONYMOUS_MAP_ENTRY((sprintf(xpath[k], AMP_STATE_CURRENT, pcCfgKey, i + 1), xpath[k++]),
                                                   SR_DECIMAL64_T, (char *)(&pstEdfaState->dLaserBiasCurrents[i]));
        memcpy(&edfa_maps[j++], &current, sizeof(ANONYMOUS_ST));
    }

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstEdfaState || size < sizeof(EDFA_STATE_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstEdfaState, 0, size);
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_STATE_ALL, pcCfgKey);
    rc = DSC_GetItems(sess, acSelectXpath, edfa_maps, sizeof(edfa_maps));

    LOG_WRITE(EN_LOG_NOTICE, "aacEnum[0]:%s, aacEnum[1]:%s aacEnum[2]:%s", aacEnum[0], aacEnum[1], aacEnum[2]);

    pstEdfaState->enState = __edfaStateToEnum(aacEnum[0]);
    LOG_WRITE(EN_LOG_NOTICE, "aacEnum[0]:%s, pstEdfaState->enState:%d", aacEnum[0], pstEdfaState->enState);

    pstEdfaState->enFsmMode = DSC_EdfaFsmModeToEnum(aacEnum[1]);
    LOG_WRITE(EN_LOG_NOTICE, "aacEnum[1]:%s, pstEdfaState->enFsmMode:%d", aacEnum[1], pstEdfaState->enFsmMode);

    pstEdfaState->enFsmCurrent = DSC_EdfaFsmStateToEnum(aacEnum[2]);
    LOG_WRITE(EN_LOG_NOTICE, "aacEnum[2]:%s, pstEdfaState->enFsmCurrent:%d", aacEnum[2], pstEdfaState->enFsmCurrent);

    return rc;
}


int
DSC_SetEdfaGainRange(char *pcCfgKey, EDFA_GAIN_RANGE_EN enGainRange)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char acBuf[32];
    char acTmpBuf[64];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_GAIN_RANGE, pcCfgKey);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetEdfaGainRange xpath: %s, enGainRange : %d", acSelectXpath, enGainRange);

    memset(acBuf, 0, sizeof(acBuf));
    snprintf(acTmpBuf, sizeof(acTmpBuf), "openconfig-optical-amplifier:%s", DSC_EdfaGainrangeToStr(acBuf, enGainRange));
    rc = set_identityref_item(sess, acSelectXpath, acTmpBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaBackRefRatio(char *pcCfgKey, double dBackRefRatio)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    /* snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EDFA_XPATH_BACK_REF_RATIO, enDirection); */
    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetEdfaBackRefRatio xpath: %s, dBackRefRatio : %f", acSelectXpath, dBackRefRatio);

    rc = set_decimal64_item(sess, acSelectXpath, dBackRefRatio);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaInputId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
        iTcaEdfaInputId = TCA_IDX_EDFA1_INPUT;
    } else if (0 == strcmp(pcCfgKey, EDFA_BA_CFG_KEY)) {
        iTcaEdfaInputId = TCA_IDX_EDFA2_INPUT;
    } else if (0 == strcmp(pcCfgKey, EDFA_PAP_CFG_KEY)) {
        iTcaEdfaInputId = TCA_IDX_EDFA3_INPUT;
    } else if (0 == strcmp(pcCfgKey, EDFA_BAP_CFG_KEY)) {
        iTcaEdfaInputId = TCA_IDX_EDFA4_INPUT;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find edfa: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaEdfaInputId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set edfa input los thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaOutputId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
        iTcaEdfaOutputId = TCA_IDX_CLIENTT_LOS;
    } else if (0 == strcmp(pcCfgKey, EDFA_BA_CFG_KEY)) {
        iTcaEdfaOutputId = TCA_IDX_LINET_LOS;
    } else if (0 == strcmp(pcCfgKey, EDFA_PAP_CFG_KEY)) {
        iTcaEdfaOutputId = TCA_IDX_CLIENTT_P_LOS;
    } else if (0 == strcmp(pcCfgKey, EDFA_BAP_CFG_KEY)) {
        iTcaEdfaOutputId = TCA_IDX_LINET_P_LOS;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find edfa: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaEdfaOutputId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set edfa output los thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaMsaInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaMsaInputId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
        iTcaEdfaMsaInputId = TCA_IDX_EDFA1_MSA_INPUT;
    } else {
        iTcaEdfaMsaInputId = TCA_IDX_EDFA2_MSA_INPUT;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaEdfaMsaInputId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set edfa output los thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaMsaOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaMsaMsaOutputId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
        iTcaEdfaMsaMsaOutputId = TCA_IDX_EDFA1_MSA_OUTPUT;
    } else {
        iTcaEdfaMsaMsaOutputId = TCA_IDX_EDFA2_MSA_OUTPUT;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaEdfaMsaMsaOutputId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set edfa output los thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaDcmInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;

    return rc;
}


int
DSC_SetEdfaDcmOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;

    return rc;
}

int
DSC_SetEdfaOutRflThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaEdfaRflId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, EDFA_PA_CFG_KEY)) {
        iTcaEdfaRflId = TCA_IDX_PA_RFL;
    } else if (0 == strcmp(pcCfgKey, EDFA_BA_CFG_KEY)) {
        iTcaEdfaRflId = TCA_IDX_BA_RFL;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find edfa: %s", pcCfgKey);
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaEdfaRflId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set edfa output reflection thresh: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaGain(char *pcCfgKey, double dGainSetPoint)
{
    return DSC_SetEdfaGain_Commit(pcCfgKey, dGainSetPoint, true);
}

int
DSC_SetEdfaGain_Commit(char *pcCfgKey, double dGainSetPoint, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

#ifndef _NR1004_
    if (EDFA_MODE_APC == DSC_GetEdfaModeByName(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s APC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }
#endif

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_GAIN, pcCfgKey);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetEdfaGain xpath: %s, dGainSetPoint : %f", acSelectXpath, dGainSetPoint);

    rc = set_decimal64_item(sess, acSelectXpath, dGainSetPoint);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (true == bCommit) {
        rc = DSC_Commit(sess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaTilt(char *pcCfgKey, double dTiltSetPoint)
{
    return DSC_SetEdfaTilt_Commit(pcCfgKey, dTiltSetPoint, true);
}

int
DSC_SetEdfaTilt_Commit(char *pcCfgKey, double dTiltSetPoint, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

#ifndef _NR1004_
    if (EDFA_MODE_APC == DSC_GetEdfaModeByName(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s APC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }
#endif

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_TILT, pcCfgKey);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetEdfaTilt xpath: %s, dTiltSetPoint : %f", acSelectXpath, dTiltSetPoint);

    rc = set_decimal64_item(sess, acSelectXpath, dTiltSetPoint);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (true == bCommit) {
        rc = DSC_Commit(sess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaOutputEnable(char *pcCfgKey, bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

#if 0
    if (EDFA_MODE_APC == DSC_GetEdfaModeByName(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s APC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }
#endif

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_ENABLE, pcCfgKey);

    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaAutolosEnable(char *pcCfgKey, bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

#ifndef _NR1004_
    if (EDFA_MODE_APC == DSC_GetEdfaModeByName(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s APC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }
#endif

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_AUTOLOS_ENABLE, pcCfgKey);

    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetEdfaMode(char *pcCfgKey, EDFA_MODE_EN enMode)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char acBuf[DEF_BUF_SIZE_64];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_MODE, pcCfgKey);

    memset(acBuf, 0, sizeof(acBuf));
    if (NULL == DSC_EdfaModeToStr(enMode, acBuf, sizeof(acBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_EdfaModeToStr: %d", enMode);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    rc = set_identityref_item(sess, acSelectXpath, acBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaOutputPower(char *pcCfgKey, double dPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char acBuf[32];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }
#if 0
    if (EDFA_MODE_APC != m(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s NOT APC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }
#endif
    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_TARGET_POWER, pcCfgKey);

    memset(acBuf, 0, sizeof(acBuf));
    rc = set_decimal64_item(sess, acSelectXpath, dPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaType(char *pcCfgKey, AMP_TYPE_EN enType)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char acBuf[32];
    char acTmpBuf[64];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_TYPE, pcCfgKey);

    memset(acBuf, 0, sizeof(acBuf));
    snprintf(acTmpBuf, sizeof(acTmpBuf), "openconfig-optical-amplifier:%s", DSC_AmpTypeToStr(acBuf, enType));
    rc = set_identityref_item(sess, acSelectXpath, acTmpBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaDcmTargetLos(char *pcCfgKey, double dDcmTargetLos)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    /* snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EDFA_XPATH_TARGET_DCM_LOS, enDirection); */

    rc = set_decimal64_item(sess, acSelectXpath, dDcmTargetLos);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaAprEnable(char *pcCfgKey, bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_APR_ENABLE, pcCfgKey);

    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaAprPower(char *pcCfgKey, double dAprPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_APR_POWER, pcCfgKey);

    rc = set_decimal64_item(sess, acSelectXpath, dAprPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaPlimEnable(char *pcCfgKey, bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_PLIM_ENABLE, pcCfgKey);

    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaGlimPower(char *pcCfgKey, double dGlimPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    /* snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EDFA_XPATH_GLIM_TARGET_PWR, enDirection); */

    rc = set_decimal64_item(sess, acSelectXpath, dGlimPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaPlimPower(char *pcCfgKey, double dPlimPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_PLIM_POWER, pcCfgKey);

    rc = set_decimal64_item(sess, acSelectXpath, dPlimPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_GetVoaCfgKeyFromDb(VOA_KEY_ST *pstVoaKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;


    if (NULL == pstVoaKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstVoaKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetStartupSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, EVOA_XPATH_NAME_ALL, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        snprintf(pstVoaKey->aacVoaKeys[i], sizeof(pstVoaKey->aacVoaKeys[i]), "%s", pstValues[i].data.string_val);
        /*strncpy(pstVoaKey->aacVoaKeys[i], pstValues[i].data.string_val, sizeof(pstVoaKey->aacVoaKeys[i]) - 1);*/
    }
    pstVoaKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_GetEVoaCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "attenuator", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        /* LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath); */
    }

cleanup:
    return rc;
}

int
DSC_SetEvoaAttn(char *pcCfgKey, double dAttn)
{
    return DSC_SetEvoaAttn_Commit(pcCfgKey, dAttn, true);
}



int
DSC_SetEvoaAttn_Commit(char *pcCfgKey, double dAttn, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_ATTN, pcCfgKey);

    rc = set_decimal64_item(sess, acSelectXpath, dAttn);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (true == bCommit) {
        rc = DSC_Commit(sess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

ERR_LABEL:
    return rc;

}

int
DSC_SetEvoaTarget(char *pcCfgKey, double dTarget)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_TARGET, pcCfgKey);
    LOG_WRITE(EN_LOG_NOTICE, "acSelectXpath: %s", acSelectXpath);

    rc = set_decimal64_item(sess, acSelectXpath, dTarget);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;

}

int
DSC_SetEvoaEnable(char *pcCfgKey, bool bEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_ENABLE, pcCfgKey);
    LOG_WRITE(EN_LOG_NOTICE, "acSelectXpath: %s", acSelectXpath);

    rc = set_bool_item(sess, acSelectXpath, bEnable);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;

}

int
DSC_GetEvoaTarget(char *pcCfgKey, double *pdTarget)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_TARGET, pcCfgKey);

    rc = get_decimal64_item(sess, acSelectXpath, pdTarget);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by evoa target power: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEvoaEnable(char *pcCfgKey, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_ENABLE, pcCfgKey);

    rc = get_bool_item(sess, acSelectXpath, pbEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by evoa enable : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEvoaActualTarget(char *pcCfgKey, double *pdTargetActual)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char *pcTmpKey = pcCfgKey;

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (NULL == pdTargetActual) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "Target Actual");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if ( 0 == strcmp(pcCfgKey, "OSC-TX-VOA") || 0 == strcmp(pcCfgKey, "OSC-RX-VOA")) {
        pcTmpKey = "OSC";
    } else if (0 == strcmp(pcCfgKey, "OSC-TX-VOA-P") || 0 == strcmp(pcCfgKey, "OSC-RX-VOA-P")) {
        pcTmpKey = "OSC-P";
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OSC_XPATH_ADD_PWR, pcTmpKey);

    rc = get_decimal64_item(sess, acSelectXpath, pdTargetActual);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by evoa target power actual: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEvoaAttn(char *pcCfgKey, double *pdAttn)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_ATTN, pcCfgKey);

    rc = get_decimal64_item(sess, acSelectXpath, pdAttn);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by evoa attn: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEvoaActualAttn(char *pcCfgKey, double *pdAttn)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (NULL == pdAttn) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "attn");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, EVOA_XPATH_ATTN_AUTUAL, pcCfgKey);

    rc = get_decimal64_item(sess, acSelectXpath, pdAttn);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEdfaGainByName(char *name, double *gain)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, AMP_XPATH_GAIN, name);

    return get_decimal64_item(sess, acXpath, gain);
}


EDFA_MODE_EN
DSC_GetEdfaModeByName(char *name)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    char acBuf[128];
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return EDFA_MODE_AGC;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, AMP_XPATH_MODE, name);

    memset(acBuf, 0, sizeof(acBuf));
    if (SR_ERR_OK != get_identityref_item(sess, acXpath, acBuf, sizeof(acBuf))) {
        return EDFA_MODE_AGC;
    }

    return DSC_EdfaModeToEnum(acBuf);
}

int
DSC_GetEdfaThrConfig(int iTcaIdx, double *pdHighThr, double *pdLowThr)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t nCount = 0;

    if (NULL == pdHighThr || NULL == pdLowThr) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetTcaList(NULL, &pstTcaInfo, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
        return rc;
    }

    rc = DSC_GetTcaThresh(iTcaIdx,
                          pstTcaInfo,
                          pdHighThr,
                          pdLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get edfa threshold error, enIdx:%d", iTcaIdx);
    }

    LOG_WRITE(EN_LOG_NOTICE, "enIdx:%d,dInHighThr=%f,dInLowThr=%f", iTcaIdx, *pdHighThr, *pdLowThr);

    return rc;
}

EDFA_GAIN_RANGE_EN
DSC_GetEdfaGainRangeByName(char *name)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    char acBuf[DEF_BUF_SIZE_128];
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return EDFA_GAIN_RANGE_ERROR;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), AMP_XPATH_GAIN_RANGE, name);

    memset(acBuf, 0, sizeof(acBuf));
    if (SR_ERR_OK != get_identityref_item(sess, acXpath, acBuf, sizeof(acBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "ERROR get gain-range , acXpath:%s", acXpath);
        return EDFA_GAIN_RANGE_ERROR;
    }

    return DSC_EdfaGainRangeToEnum(acBuf);
}

int
DSC_GetEdfaOutputPowerByName(char *name, double *power)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, AMP_XPATH_TARGET_POWER, name);

    return get_decimal64_item(sess, acXpath, power);
}

int
DSC_GetEdfaOutputEnableByName(char *pcCfgKey, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_ENABLE, pcCfgKey);

    rc = get_bool_item(sess, acSelectXpath, pbEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by evoa enable : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetPortOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_CFG_KEY)) {
        iTcaId = TCA_IDX_LINET_LOS;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENTT_LOS;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_LINET_P_LOS;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENTT_P_LOS;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s output los thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetClientRxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_R_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_RX_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_R_P_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_P_RX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetClientRxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_R_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_RX_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_R_P_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_P_RX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetClientTxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_TX_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_P_TX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetClientTxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_TX_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_CLIENT_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_CLIENT_P_TX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetLineRxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_R_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_RX_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_R_P_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_P_RX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetLineRxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_R_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_RX_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_R_P_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_P_RX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetLineTxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_TX_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_P_TX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetLineTxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_TX_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PORT_LINE_T_P_CFG_KEY)) {
        iTcaId = TCA_IDX_LINE_P_TX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaCurrentHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_BA_PUMP1_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_PUMP1_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_BA_PUMP2_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_PUMP2_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_BA_PUMP3_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_PUMP3_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PA_PUMP1_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_PUMP1_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PA_PUMP2_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_PUMP2_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_BA_P_PUMP1_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_PUMP1_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_BA_P_PUMP2_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_PUMP2_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_BA_P_PUMP3_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_PUMP3_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PA_P_PUMP1_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_PUMP1_CURRENT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_PA_P_PUMP2_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_PUMP2_CURRENT_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetEdfaCurrents(char *pcCfgKey, uint8_t ucNumOfPumps, double *pdCurrents)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (EDFA_MODE_ACC != DSC_GetEdfaModeByName(pcCfgKey)) {
        LOG_WRITE(EN_LOG_ERROR, "%s NOT ACC Mode", pcCfgKey);
        rc = SR_ERR_UNSUPPORTED;
        goto ERR_LABEL;
    }

    for (uint8_t i = 0; i < ucNumOfPumps; i++) {
        memset(acSelectXpath, 0, sizeof(acSelectXpath));
        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_XPATH_CURRENT, pcCfgKey, i + 1);
        rc = set_decimal64_item(sess, acSelectXpath, pdCurrents[i]);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEdfaNumOfPumps(char *pcCfgKey, uint8_t *pucNumOfPumps)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == pucNumOfPumps) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), AMP_XPATH_NUM_OF_PUMPS, pcCfgKey);
    rc = get_uint8_item(sess, acXpath, pucNumOfPumps);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting number of pumps(%s): %s", acXpath, sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetEdfaFsmMode(char *pcCfgKey, AMP_FSM_MODE_EN enFsmMode)
{
    return DSC_SetEdfaFsmMode_Commit(pcCfgKey, enFsmMode, true);
}

int
DSC_SetEdfaFsmMode_Commit(char *pcCfgKey, AMP_FSM_MODE_EN enFsmMode, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    char acFsmModeBuff[64] = {0};

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acFsmModeBuff, 0, sizeof(acFsmModeBuff));
    DSC_EdfaFsmModeToStr(acFsmModeBuff, enFsmMode);
    if (0 == strlen(acFsmModeBuff)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_EdfaFsmModeToStr error, state:%d", enFsmMode);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_CFG_FSM_MODE, pcCfgKey);
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetEdfaFsmMode xpath: %s, uiFsmMode : %d, %s", acSelectXpath, enFsmMode, acFsmModeBuff);

    rc = set_enum_item(sess, acSelectXpath, acFsmModeBuff);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (true == bCommit) {
        rc = DSC_Commit(sess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetEdfaFsmMode(char *pcCfgKey, AMP_FSM_MODE_EN *penFsmMode)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char acFsmMode[64] = {0};

    if (NULL == sess || NULL == penFsmMode) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), AMP_CFG_FSM_MODE, pcCfgKey);
    rc = get_enum_item(sess, acXpath, acFsmMode, sizeof(acFsmMode) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting FsmMode of edfa (%s): %s", acXpath, sr_strerror(rc));
    }

    if (0 == strlen(acFsmMode)) {
        LOG_WRITE(EN_LOG_ERROR, "acFsmMode error");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    *penFsmMode = DSC_EdfaFsmModeToEnum(acFsmMode);

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_RpcReadyTrigger                                                                        */
/* Descrp : RPC ReadyTrigger                                                                           */
/* Input  : pcModule -- Module to be Trigger                                                                   */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_RpcReadyTrigger(char *pcModule, uint32_t value)
{
    sr_val_t *pstInput = NULL;
    sr_val_t *pstOutput = NULL;
    size_t nOutputCnt = 0;
    size_t nInputCnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check parameter.                                                     */
    if (NULL == pstSess || NULL == pcModule) {
        LOG_WRITE(EN_LOG_ERROR, "Session or pcModule null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* allocate input values                                                */
    nInputCnt = 2;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "new values error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set 'input/module leaf.                                                */
    rc = sr_val_set_xpath(&pstInput[0], RPC_XPATH_READY_TRIGGER_MODULE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_READY_TRIGGER_MODULE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_ENUM_T;
    pstInput[0].data.enum_val = strdup(pcModule);

    /* set 'input/value leaf.                                                */
    rc = sr_val_set_xpath(&pstInput[1], RPC_XPATH_READY_TRIGGER_VALUE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_READY_TRIGGER_VALUE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_UINT32_T;
    pstInput[1].data.uint32_val = value;

    /* execute RPC.                                                         */
    rc = sr_rpc_send(pstSess, RPC_XPATH_READY_TRIGGER, pstInput, nInputCnt, 0, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", RPC_XPATH_READY_TRIGGER, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    return DSC_ProcessRPCResponse(pstOutput, nOutputCnt);

ERR_LABEL:
    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    return rc;
}

int
DSC_SetEdfaReadyPower(char *pcCfgKey, double dAprPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, AMP_CFG_READY_POWER, pcCfgKey);

    rc = set_decimal64_item(sess, acSelectXpath, dAprPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

