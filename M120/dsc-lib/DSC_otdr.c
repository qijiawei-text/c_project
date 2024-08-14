/**
 * Copyright (c) Oplink Communications, LLC
 * fandy <Dongyang.Fan@Molex.com>
 * Jul. 24th, 2020
 */

#include "DSC_otdr.h"
#include <math.h>
#include <limits.h>

/* this works because MODULE_NS_OTDR and MODULE_NS_OTDR_I are same length*/
static char *
__toInternal(char *xpath)
{
    static char path[OPLK_PATH_MAX_LEN] = {0};
    strncpy(path, xpath, sizeof(path));
    char *p = strstr(path, MODULE_NS_OTDR);
    if (p != NULL) {
        memcpy(p, MODULE_NS_OTDR_I, strlen(MODULE_NS_OTDR));
    }
    return path;
}

OTDR_PORT_EN
DSC_GetOtdrPortNameEn(char *pcPortName)
{
    OTDR_PORT_EN enPort = OTDR_PORT_CNT;

    if (NULL == pcPortName) {
        return enPort;
    }

    if (0 == strncmp(pcPortName, OTDR_STR_EXTERNAL_PORT, strlen(OTDR_STR_EXTERNAL_PORT))) {
        return OTDR_EXTERNAL_PORT;
    } else if (0 == strncmp(pcPortName, OTDR_STR_LINE_IN_PORT, strlen(OTDR_STR_LINE_IN_PORT))) {
        return OTDR_LINE_IN_PORT;
    } else if (0 == strncmp(pcPortName, OTDR_STR_LINE_OUT_PORT, strlen(OTDR_STR_LINE_OUT_PORT))) {
        return OTDR_LINE_OUT_PORT;
    } else if (0 == strncmp(pcPortName, OTDR_STR_EXTERNAL_P_PORT, strlen(OTDR_STR_EXTERNAL_P_PORT))) {
        return OTDR_EXTERNAL_P_PORT;
    } else if (0 == strncmp(pcPortName, OTDR_STR_LINE_IN_P_PORT, strlen(OTDR_STR_LINE_IN_P_PORT))) {
        return OTDR_LINE_IN_P_PORT;
    } else if (0 == strncmp(pcPortName, OTDR_STR_LINE_OUT_P_PORT, strlen(OTDR_STR_LINE_OUT_P_PORT))) {
        return OTDR_LINE_OUT_P_PORT;
    }

    return enPort;
}

char *
DSC_GetOtdrPortNameStr(char *pcStr, OTDR_PORT_EN enVal)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enVal) {
    case OTDR_LINE_IN_PORT:
        strcpy(pcStr, OTDR_STR_LINE_IN_PORT);
        break;
    case OTDR_LINE_OUT_PORT:
        strcpy(pcStr, OTDR_STR_LINE_OUT_PORT);
        break;
    case OTDR_EXTERNAL_PORT:
        strcpy(pcStr, OTDR_STR_EXTERNAL_PORT);
        break;
    case OTDR_LINE_IN_P_PORT:
        strcpy(pcStr, OTDR_STR_LINE_IN_P_PORT);
        break;
    case OTDR_LINE_OUT_P_PORT:
        strcpy(pcStr, OTDR_STR_LINE_OUT_P_PORT);
        break;
    default:
        strcpy(pcStr, OTDR_STR_EXTERNAL_P_PORT);
        break;
    }

    return pcStr;
}

OTDR_MODE_EN
DSC_GetOtdrModeEn(char *pcMode)
{
    OTDR_MODE_EN enMode = OTDR_MODE_CNT;

    if (NULL == pcMode) {
        return enMode;
    }

    if (0 == strncmp(pcMode, OTDR_STR_MODE_IDLE, strlen(OTDR_STR_MODE_IDLE))) {
        return OTDR_MODE_IDLE;
    } else if (0 == strncmp(pcMode, OTDR_STR_MODE_MEASURING, strlen(OTDR_STR_MODE_MEASURING))) {
        return OTDR_MODE_MEASURING;
    } else if (0 == strncmp(pcMode, OTDR_STR_MODE_PENDING, strlen(OTDR_STR_MODE_PENDING))) {
        return OTDR_MODE_PENDING;
    } else if (0 == strncmp(pcMode, OTDR_STR_MODE_UNKNOWN, strlen(OTDR_STR_MODE_UNKNOWN))) {
        return OTDR_MODE_UNKNOWN;
    }

    return enMode;
}

OTDR_TRIGGER_EN
DSC_GetOtdrTriggerEn(char *pcTrigger)
{
    OTDR_TRIGGER_EN enTrigger = OTDR_TRIGGER_CNT;

    if (0 == strncmp(pcTrigger, OTDR_STR_TRIGGER_LOF_SET, strlen(OTDR_STR_TRIGGER_LOF_SET))) {
        return OTDR_TRIGGER_LOF_SET;
    } else if (0 == strncmp(pcTrigger, OTDR_STR_TRIGGER_LOF_CLEAR, strlen(OTDR_STR_TRIGGER_LOF_CLEAR))) {
        return OTDR_TRIGGER_LOF_CLEAR;
    } else if (0 == strncmp(pcTrigger, OTDR_STR_TRIGGER_MANUAL, strlen(OTDR_STR_TRIGGER_MANUAL))) {
        return OTDR_TRIGGER_MANUAL;
    } else if (0 == strncmp(pcTrigger, OTDR_STR_TRIGGER_EXTERNAL, strlen(OTDR_STR_TRIGGER_EXTERNAL))) {
        return OTDR_TRIGGER_EXTERNAL;
    }
    return enTrigger;
}

char *
DSC_GetOtdrModeStr(char *pcStr, OTDR_MODE_EN enVal)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enVal) {
    case OTDR_MODE_MEASURING:
        strcpy(pcStr, OTDR_STR_MODE_MEASURING);
        break;
    default:
        strcpy(pcStr, OTDR_STR_MODE_IDLE);
        break;
    }

    return pcStr;
}

OTDR_EVENT_RET_EN
DSC_GetOtdrEventTypeEn(char *pcEventType)
{
    OTDR_EVENT_RET_EN enType = OTDR_EVENT_RET_CNT;

    if (NULL == pcEventType) {
        return enType;
    }

    if (0 == strncmp(pcEventType, OTDR_STR_EVENT_TYPE_NO_REF, strlen(OTDR_STR_EVENT_TYPE_NO_REF))) {
        return OTDR_EVENT_RET_NON_REF;
    } else if (0 == strncmp(pcEventType, OTDR_STR_EVENT_TYPE_REF, strlen(OTDR_STR_EVENT_TYPE_REF))) {
        return OTDR_EVENT_RET_REF;
    } else if (0 == strncmp(pcEventType, OTDR_STR_EVENT_TYPE_FIBER_END, strlen(OTDR_STR_EVENT_TYPE_FIBER_END))) {
        return OTDR_EVENT_RET_FIBER_END;
    }

    return enType;
}

char *
DSC_GetOtdrEventTypeStr(char *pcStr, OTDR_EVENT_RET_EN enVal)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enVal) {
    case OTDR_EVENT_RET_NON_REF:
        strcpy(pcStr, OTDR_STR_EVENT_TYPE_NO_REF);
        break;
    case OTDR_EVENT_RET_REF:
        strcpy(pcStr, OTDR_STR_EVENT_TYPE_REF);
        break;
    default:
        strcpy(pcStr, OTDR_STR_EVENT_TYPE_FIBER_END);
        break;
    }

    return pcStr;
}

OTDR_SCAN_TYPE_EN
DSC_GetOtdrScanTypeEn(char *pcScanType)
{
    OTDR_SCAN_TYPE_EN enType = OTDR_SCAN_TYPE_CNT;

    if (NULL == pcScanType) {
        return enType;
    }

    if (0 == strncmp(pcScanType, OTDR_STR_SCAN_TYPE_SHORT, strlen(OTDR_STR_SCAN_TYPE_SHORT))) {
        return OTDR_SCAN_TYPE_SHORT;
    } else if (0 == strncmp(pcScanType, OTDR_STR_SCAN_TYPE_MED, strlen(OTDR_STR_SCAN_TYPE_MED))) {
        return OTDR_SCAN_TYPE_MED;
    } else if (0 == strncmp(pcScanType, OTDR_STR_SCAN_TYPE_LONG, strlen(OTDR_STR_SCAN_TYPE_LONG))) {
        return OTDR_SCAN_TYPE_LONG;
    } else if (0 == strncmp(pcScanType, OTDR_STR_SCAN_TYPE_CUSTOM, strlen(OTDR_STR_SCAN_TYPE_CUSTOM))) {
        return OTDR_SCAN_TYPE_CUSTOM;
    }
    if (0 == strncmp(pcScanType, OTDR_STR_SCAN_TYPE_AUTO, strlen(OTDR_STR_SCAN_TYPE_AUTO))) {
        return OTDR_SCAN_TYPE_AUTO;
    }

    return enType;
}

char *
DSC_GetOtdrScanTypeStr(char *pcStr, OTDR_SCAN_TYPE_EN enVal)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enVal) {
    case OTDR_SCAN_TYPE_SHORT:
        strcpy(pcStr, OTDR_STR_SCAN_TYPE_SHORT);
        break;
    case OTDR_SCAN_TYPE_MED:
        strcpy(pcStr, OTDR_STR_SCAN_TYPE_MED);
        break;
    case OTDR_SCAN_TYPE_LONG:
        strcpy(pcStr, OTDR_STR_SCAN_TYPE_LONG);
        break;
    case OTDR_SCAN_TYPE_CUSTOM:
        strcpy(pcStr, OTDR_STR_SCAN_TYPE_CUSTOM);
        break;
    default:
        strcpy(pcStr, OTDR_STR_SCAN_TYPE_AUTO);
        break;
    }

    return pcStr;
}

int
DSC_OtdrGetCfgKeyFromDb(OTDR_KEY_ST *pstOtdrKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;

    if (NULL == pstOtdrKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstOtdrKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, OTDR_XPATH_NAME_ALL, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "otdr key %d: %s", i, pstValues[i].data.string_val);
        snprintf(pstOtdrKey->aacOtdrKeys[i], sizeof(pstOtdrKey->aacOtdrKeys[i]), "%s", pstValues[i].data.string_val);
    }
    pstOtdrKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_OtdrGetPortKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "otdr", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        /* do nothing. */
    }

cleanup:
    return rc;
}

/* port config */
int
DSC_OtdrGetCfg(sr_session_ctx_t *pstSess, char *pcKey, OTDR_CFG_ST *pstCfg)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char xpath[16][OPLK_PATH_MAX_LEN] = {{0}};

    ANONYMOUS_ST otdr_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OTDR_PORT_MEASURING_TIME_XPATH, pcKey), SR_UINT32_T, (char *)(&pstCfg->ulMeasuringTime)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OTDR_PORT_DIST_RANGE_XPATH, pcKey), SR_UINT32_T, (char *)(&pstCfg->stPara.ulDistRange)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OTDR_PORT_PULSE_WIDTH_XPATH, pcKey), SR_UINT32_T, (char *)(&pstCfg->stPara.ulPulseWidth)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OTDR_PORT_SAMP_RESOL_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stPara.dSampleResolution)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OTDR_PORT_REF_THR_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dRefLosThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], OTDR_PORT_SPLICE_THR_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dSpliceLosThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], OTDR_PORT_FIBER_END_THR_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dFiberEndThr)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], OTDR_PORT_REFRACTIVE_INDEX_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stThr.reflectiveIndex)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], OTDR_PORT_BACKSCATTER_INDEX_XPATH, pcKey), SR_DECIMAL64_T, (char *)(&pstCfg->stThr.backscatterIndex)),
    };

    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    if (NULL == pstSess || NULL == pstCfg || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p, %p",
                  pstSess, pstCfg, pcKey);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstCfg, 0, sizeof(OTDR_CFG_ST));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_CFG_HEADER_ALL, pcKey);
    rc = DSC_GetItems(pstSess, acSelectXpath, otdr_maps, sizeof(otdr_maps));

    return rc;
}

int
DSC_OtdrSetMTime(sr_session_ctx_t *pstSess, char *pcKey, uint32_t ulMTime, bool isCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    bool isCommon = false;

    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    if (NULL == pstSess || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p", pstSess, pcKey);
        return SR_ERR_INVAL_ARG;
    }

    if (OTDR_PORT_CNT == DSC_GetOtdrPortNameEn(pcKey)) {
        /* it is not a port, so it is common config(short, mediam or long) */
        isCommon = true;
    }

    if (isCommon) {
        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_MEASURING_TIME_XPATH, pcKey);
    } else {
        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_MEASURING_TIME_XPATH, pcKey);
    }
    rc = set_uint32_item(pstSess, acSelectXpath, ulMTime);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!isCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;

}

int
DSC_OtdrSetPara(sr_session_ctx_t *pstSess, char *pcKey, OTDR_PARA_ST *pstPara, bool isCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    bool isCommon = false;

    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    if (NULL == pstSess || NULL == pstPara || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p, %p",
                  pstSess, pstPara, pcKey);
        return SR_ERR_INVAL_ARG;
    }

    if (OTDR_PORT_CNT == DSC_GetOtdrPortNameEn(pcKey)) {
        /* it is not a port, so it is common config(short, mediam or long) */
        isCommon = true;
    }

    if (pstPara->ulDistRange != UINT_MAX) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_DIST_RANGE_XPATH, pcKey);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_DIST_RANGE_XPATH, pcKey);
        }

        rc = set_uint32_item(pstSess, acSelectXpath, pstPara->ulDistRange);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (pstPara->ulPulseWidth != UINT_MAX) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PULSE_WIDTH_XPATH, pcKey);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_PULSE_WIDTH_XPATH, pcKey);
        }
        rc = set_uint32_item(pstSess, acSelectXpath, pstPara->ulPulseWidth);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isnan(pstPara->dSampleResolution)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_SAMP_RESOL_XPATH, pcKey);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_SAMP_RESOL_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstPara->dSampleResolution);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_OtdrSetThr(sr_session_ctx_t *pstSess, char *pcKey, OTDR_THR_ST *pstThr, bool isCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    bool isCommon = false;

    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    if (NULL == pstSess || NULL == pstThr || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p, %p", pstSess, pstThr, pcKey);
        return SR_ERR_INVAL_ARG;
    }

    if (OTDR_PORT_CNT == DSC_GetOtdrPortNameEn(pcKey)) {
        /* it is not a port, so it is common config(short, mediam or long) */
        isCommon = true;
    }

    if (!isnan(pstThr->dRefLosThr)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_REF_THR_XPATH);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_REF_THR_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstThr->dRefLosThr);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isnan(pstThr->dSpliceLosThr)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_SPLICE_THR_XPATH);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_SPLICE_THR_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstThr->dSpliceLosThr);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isnan(pstThr->dFiberEndThr)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_FIBER_END_THR_XPATH);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_FIBER_END_THR_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstThr->dFiberEndThr);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isnan(pstThr->reflectiveIndex)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_REFRACTIVE_INDEX_XPATH);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_REFRACTIVE_INDEX_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstThr->reflectiveIndex);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isnan(pstThr->backscatterIndex)) {
        if (isCommon) {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_BACKSCATTER_INDEX_XPATH);
        } else {
            snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_BACKSCATTER_INDEX_XPATH, pcKey);
        }
        rc = set_decimal64_item(pstSess, acSelectXpath, pstThr->backscatterIndex);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    if (!isCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_OtdrGetCommonCfg(sr_session_ctx_t *pstSess, char *pcScanType, OTDR_CFG_ST *pstCfg)
{
    int rc = SR_ERR_OK;
    char xpath[4][OPLK_PATH_MAX_LEN] = {{0}};

    ANONYMOUS_ST otdr_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OTDR_MEASURING_TIME_XPATH, pcScanType), SR_UINT32_T, (char *)(&pstCfg->ulMeasuringTime)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OTDR_DIST_RANGE_XPATH, pcScanType), SR_UINT32_T, (char *)(&pstCfg->stPara.ulDistRange)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OTDR_PULSE_WIDTH_XPATH, pcScanType), SR_UINT32_T, (char *)(&pstCfg->stPara.ulPulseWidth)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OTDR_SAMP_RESOL_XPATH, pcScanType), SR_DECIMAL64_T, (char *)(&pstCfg->stPara.dSampleResolution)),
        ANONYMOUS_MAP_ENTRY(OTDR_REF_THR_XPATH, SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dRefLosThr)),
        ANONYMOUS_MAP_ENTRY(OTDR_SPLICE_THR_XPATH, SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dSpliceLosThr)),
        ANONYMOUS_MAP_ENTRY(OTDR_FIBER_END_THR_XPATH, SR_DECIMAL64_T, (char *)(&pstCfg->stThr.dFiberEndThr)),
        ANONYMOUS_MAP_ENTRY(OTDR_BACKSCATTER_INDEX_XPATH, SR_DECIMAL64_T, (char *)(&pstCfg->stThr.backscatterIndex)),
        ANONYMOUS_MAP_ENTRY(OTDR_REFRACTIVE_INDEX_XPATH, SR_DECIMAL64_T, (char *)(&pstCfg->stThr.reflectiveIndex))
    };

    if (NULL == pstSess) {
        pstSess = DSC_GetOperSession();
    }

    if (NULL == pstSess || NULL == pstCfg || NULL == pcScanType) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p, %p",
                  pstSess, pstCfg, pcScanType);
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetItems(pstSess, OTDR_XPATH_COMMON_ALL, otdr_maps, sizeof(otdr_maps));
    pstCfg->enScanType = DSC_GetOtdrScanTypeEn(pcScanType);

    return rc;
}

/* get port mode and scan results for a port */
int
DSC_OtdrGetPortState(char *pcPortName, OTDR_STATE_ST *pstState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *stVal = NULL;
    size_t nCnt = 0;
    char acTmp[64];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    strncpy(pstState->acPortName, pcPortName, sizeof(pstState->acPortName) - 1);

    pstSess = DSC_GetOperSession();

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_MODE_XPATH, pcPortName);
    rc = get_enum_item(pstSess, acSelectXpath, acTmp, sizeof(acTmp) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s get_enum_item error %s", __func__, sr_strerror(rc));
        return rc;
    }
    pstState->portMode = DSC_GetOtdrModeEn(acTmp);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_COMPLETION_TIME_ALL, pcPortName);
    rc = get_items(pstSess, acSelectXpath, &stVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s get_items error %s", __func__, sr_strerror(rc));
        return rc;
    }
    pstState->traceCnt = nCnt;
    for (int i = 0; i < nCnt; i++) {
        rc = DSC_OtdrGetTrace(pcPortName, stVal[i].data.string_val, &pstState->traces[i]);
    }

    return rc;
}

/* get all trace key: completion-time */
static int
__OtdrGetTraceAll(sr_session_ctx_t *pstSess, const char *xpath, char (*time)[OPLK_NAME_MAX_LEN], uint32_t *nCnt)
{
    int rc = SR_ERR_OK;
    sr_val_t *stVal = NULL;
    size_t count = 0;

    rc = get_items(pstSess, xpath, &stVal, &count);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s, %s", xpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    *nCnt = count;
    for (int i = 0; i < count; i++) {
        strncpy(time[i], stVal[i].data.string_val, sizeof(time[i]) - 1);
    }

ERR_LABEL:
    if (stVal != NULL) {
        sr_free_values(stVal, count);
    }
    return rc;
}

int
DSC_OtdrGetTraceAll(char *pcPortName, char (*time)[OPLK_NAME_MAX_LEN], uint32_t *nCnt)
{
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_PORT_COMPLETION_TIME_ALL, pcPortName);
    return __OtdrGetTraceAll(pstSess, acSelectXpath, time, nCnt);
}

/* get all trace key: completion-time */
int
DSC_OtdrGetTraceAllI(char *pcPortName, char (*time)[OPLK_NAME_MAX_LEN], uint32_t *nCnt)
{
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_COMPLETION_TIME_ALL), pcPortName);
    return __OtdrGetTraceAll(pstSess, acSelectXpath, time, nCnt);
}

/* get one trace of a port*/
static int
__OtdrGetTrace(sr_session_ctx_t *pstSess, const char *xpath, OTDR_TRACE_ST *pstState)
{
    int rc = SR_ERR_OK;
    sr_val_t *stVal = NULL;
    size_t nCnt = 0;
    int i = 0, j = 0;

    memset((void *)pstState, 0, sizeof(OTDR_TRACE_ST));

    rc = get_items(pstSess, xpath, &stVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s, %s", xpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < nCnt; i++) {
        LOG_WRITE(EN_LOG_DEBUG, "Xpath: %s", stVal[i].xpath);

        if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_COMPLETION_TIME_NODE)) {
            strncpy(pstState->acCompleteTime, stVal[i].data.string_val, sizeof(pstState->acCompleteTime) - 1);
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_EVENT_CNT_NODE)) {
            pstState->ulEventCnt = stVal[i].data.uint32_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_FIBER_LEN_NODE)) {
            pstState->ulFiberLength = stVal[i].data.uint32_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_LOSS_NODE)) {
            pstState->dTotalLoss = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_REF_NODE)) {
            pstState->dReturnLoss = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_EVENT_ID_NODE)) {
            j++;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_EVENT_TYPE_NODE)) {
            pstState->astEvent[j].enType = DSC_GetOtdrEventTypeEn(stVal[i].data.enum_val);
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_EVENT_LOSS_NODE)) {
            pstState->astEvent[j].dEventLoss = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_EVENT_REFLECTION_NODE)) {
            pstState->astEvent[j].dReflectance = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_EVENT_DISTANCE_NODE)) {
            pstState->astEvent[j].ulFiberLength = stVal[i].data.uint32_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_AVG_LOSS_NODE)) {
            pstState->dAverageLoss = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_LOSS_NODE)) {
            pstState->dTotalLoss = stVal[i].data.decimal64_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_TRIGGER_NODE)) {
            pstState->trigger = DSC_GetOtdrTriggerEn(stVal[i].data.enum_val);
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_SCAN_TYPE_NODE)) {
            pstState->scanType = DSC_GetOtdrScanTypeEn(stVal[i].data.enum_val);
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_BASELINE_NODE)) {
            pstState->baseline = stVal[i].data.bool_val;
        } else if (sr_xpath_node_name_eq(stVal[i].xpath, OTDR_PORT_SOR_FILE_NODE)) {
            strncpy(pstState->acSorfile, stVal[i].data.string_val, sizeof(pstState->acSorfile) - 1);
        }
    }

ERR_LABEL:
    if (stVal != NULL) {
        sr_free_values(stVal, nCnt);
    }
    return rc;
}

int
DSC_OtdrGetTrace(char *pcPortName, char *completion_time, OTDR_TRACE_ST *pstState)
{
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OTDR_TRACE_SUBTREE_XPATH, pcPortName, completion_time);
    return __OtdrGetTrace(pstSess, acSelectXpath, pstState);
}

int
DSC_OtdrGetTraceI(char *pcPortName, char *completion_time, OTDR_TRACE_ST *pstState)
{
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_TRACE_SUBTREE_XPATH), pcPortName, completion_time);
    return __OtdrGetTrace(pstSess, acSelectXpath, pstState);
}

int
DSC_OtdrSetBaselineI(char *pcPortName, char *completionTime)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();
    char acAllTraceTime[OTDR_TRACE_MAX][OPLK_NAME_MAX_LEN] = {{0}};
    OTDR_TRACE_ST stTrace;
    uint32_t nCnt = 0;
    char acSorfile[OPLK_BUF_MAX_LEN];
    char acCmdLine[OPLK_BUF_MAX_LEN * 3];
    char *find = NULL;
    int len = 0;

    rc = DSC_OtdrGetTraceAllI(pcPortName, acAllTraceTime, &nCnt);
    if (SR_ERR_OK != rc || nCnt == 0) {
        LOG_WRITE(EN_LOG_ERROR, "No trace exsits, %s", sr_strerror(rc));
        goto cleanup;
    }

    for (uint32_t i = 0; i < nCnt; i++) {
        memset(&stTrace, 0, sizeof(stTrace));
        rc = DSC_OtdrGetTraceI(pcPortName, acAllTraceTime[i], &stTrace);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_OtdrGetTraceI error, %s, %s, %s", sr_strerror(rc), pcPortName, completionTime);
            goto cleanup;
        }

        if (0 == strcmp(completionTime, acAllTraceTime[i])) {
            if (stTrace.baseline) {
                goto cleanup;
            }

            stTrace.baseline = true;

            find = strstr(stTrace.acSorfile, acAllTraceTime[i]);
            if (NULL == find) {
                LOG_WRITE(EN_LOG_ERROR, "invalid sor file name. %s", stTrace.acSorfile);
                goto cleanup;
            }

            len = find - stTrace.acSorfile;
            memcpy(acSorfile, stTrace.acSorfile, len);
            memcpy(acSorfile + len, OTDR_PORT_BASELINE_NODE, strlen(OTDR_PORT_BASELINE_NODE));
            len += strlen(OTDR_PORT_BASELINE_NODE);
            strcpy(acSorfile + len, find - 1);
        } else {
            if (!stTrace.baseline) {
                continue;
            }
            stTrace.baseline = false;

            find = strstr(stTrace.acSorfile, OTDR_PORT_BASELINE_NODE);
            if (NULL == find) {
                DSC_OtdrSetTraceI(pcPortName, acAllTraceTime[i], &stTrace);
                continue;
            }

            len = find - stTrace.acSorfile;
            memcpy(acSorfile, stTrace.acSorfile, len);
            strcpy(acSorfile + len, find + strlen(OTDR_PORT_BASELINE_NODE) + 1);
        }

        /* command line */
        snprintf(acCmdLine, sizeof(acCmdLine), "mv %s%s %s%s;sync", OTDR_SOR_INNER_PATH, stTrace.acSorfile, OTDR_SOR_INNER_PATH, acSorfile);
        rc = UTIL_System(acCmdLine);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
            goto cleanup;
        }

        strcpy(stTrace.acSorfile, acSorfile);
        DSC_OtdrSetTraceI(pcPortName, acAllTraceTime[i], &stTrace);
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_OtdrSetPortModeI(char *pcPortName, OTDR_MODE_EN status)
{
    int rc = SR_ERR_OK;
    char acEnTmp[32];
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_MODE_XPATH), pcPortName);
    rc = set_enum_item(pstSess, acSelectXpath, DSC_GetOtdrModeStr(acEnTmp, status));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set otdr mode error %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_OtdrSetTraceI(char *pcPortName, char *pcCompletionTime, OTDR_TRACE_ST *pstState)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acEnTmp[32];
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();

    if (OTDR_PORT_CNT == DSC_GetOtdrPortNameEn(pcPortName)) {
        LOG_WRITE(EN_LOG_ERROR, "port name %s error.", pcPortName);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_RETURN_LOSS_XPATH), pcPortName, pcCompletionTime);
    rc |= set_decimal64_item(pstSess, acSelectXpath, pstState->dReturnLoss);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_LOSS_XPATH), pcPortName, pcCompletionTime);
    rc |= set_decimal64_item(pstSess, acSelectXpath, pstState->dTotalLoss);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_EVENT_CNT_XPATH), pcPortName, pcCompletionTime);
    rc |= set_uint32_item(pstSess, acSelectXpath, pstState->ulEventCnt);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_FIBER_LEN_XPATH), pcPortName, pcCompletionTime);
    rc |= set_uint32_item(pstSess, acSelectXpath, pstState->ulFiberLength);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_SOR_FILE_XPATH), pcPortName, pcCompletionTime);
    rc |= set_string_item(pstSess, acSelectXpath, pstState->acSorfile, strlen(pstState->acSorfile));

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_BASELINE_XPATH), pcPortName, pcCompletionTime);
    rc |= set_bool_item(pstSess, acSelectXpath, pstState->baseline);

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_PORT_SCAN_TYPE_XPATH), pcPortName, pcCompletionTime);
    rc |= set_enum_item(pstSess, acSelectXpath, DSC_GetOtdrScanTypeStr(acEnTmp, pstState->scanType));

    for (int i = 0; i < pstState->ulEventCnt; i++) {
        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_EVENT_LOSS_XPATH), pcPortName, pcCompletionTime, i + 1);
        rc |= set_decimal64_item(pstSess, acSelectXpath, pstState->astEvent[i].dEventLoss);

        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_EVENT_REFLECTION_XPATH), pcPortName, pcCompletionTime, i + 1);
        rc |= set_decimal64_item(pstSess, acSelectXpath, pstState->astEvent[i].dReflectance);

        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_EVENT_DISTANCE_XPATH), pcPortName, pcCompletionTime, i + 1);
        rc |= set_uint32_item(pstSess, acSelectXpath, pstState->astEvent[i].ulFiberLength);

        snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_EVENT_TYPE_XPATH), pcPortName, pcCompletionTime, i + 1);
        rc |= set_enum_item(pstSess, acSelectXpath, DSC_GetOtdrEventTypeStr(acEnTmp, pstState->astEvent[i].enType));
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_OtdrDeleteTraceI(char *pcPortName, char *pcCompletionTime)
{
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, __toInternal(OTDR_TRACE_SUBTREE_XPATH), pcPortName, pcCompletionTime);
    return sr_delete_item(DSC_GetStartupSession(), acSelectXpath, SR_EDIT_DEFAULT);
}

int
DSC_OtdrStartScan(char *pcPortName, char *pcScanType, char *pcOperation)
{
    sr_val_t *pstInput = NULL, *pstOutput = NULL;
    size_t nOutputCnt = 0, nInputCnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (OTDR_PORT_CNT == DSC_GetOtdrPortNameEn(pcPortName)) {
        LOG_WRITE(EN_LOG_ERROR, "port name %s error.", pcPortName);
        return SR_ERR_INVAL_ARG;
    }

    if (OTDR_SCAN_TYPE_CNT == DSC_GetOtdrScanTypeEn(pcScanType)) {
        LOG_WRITE(EN_LOG_ERROR, "scan type %s error.", pcScanType);
        return SR_ERR_INVAL_ARG;
    }

    /* allocate input values filename*/
    nInputCnt = 3;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* scan port name*/
    rc = sr_val_set_xpath(&pstInput[0], OTDR_RPC_SCAN_NAME_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_SCAN_NAME_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_STRING_T;
    pstInput[0].data.string_val = strdup(pcPortName);

    /* scan operation: start or stop */
    rc = sr_val_set_xpath(&pstInput[1], OTDR_RPC_SCAN_OPERATION_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_SCAN_OPERATION_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_ENUM_T;
    pstInput[1].data.enum_val = strdup(pcOperation);

    /* scan distance, default 0 */
    rc = sr_val_set_xpath(&pstInput[2], OTDR_RPC_SCAN_TYPE_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_SCAN_TYPE_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[2].type = SR_ENUM_T;
    pstInput[2].data.enum_val = strdup(pcScanType);

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, OTDR_RPC_SCAN_XPATH, pstInput, nInputCnt, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", OTDR_RPC_SCAN_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(pstInput, nInputCnt);
    return DSC_ProcessRPCResponse(pstOutput, nOutputCnt);

ERR_LABEL:
    sr_free_values(pstInput, nInputCnt);
    sr_free_values(pstOutput, nOutputCnt);
    return rc;
}

int
DSC_OtdrSetTraceBaseline(char *pcPortName, char *pcTime)
{
    sr_val_t *pstInput = NULL, *pstOutput = NULL;
    size_t nOutputCnt = 0, nInputCnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcPortName) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p", pcPortName);
        return SR_ERR_INVAL_ARG;
    }

    /* allocate input values filename*/
    nInputCnt = 2;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set baseline port name */
    rc = sr_val_set_xpath(&pstInput[0], OTDR_RPC_BASELINE_NAME_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_BASELINE_NAME_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_STRING_T;
    pstInput[0].data.string_val = strdup(pcPortName);

    /* scan operation: start or stop */
    rc = sr_val_set_xpath(&pstInput[1], OTDR_RPC_BASELINE_TIME_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_BASELINE_TIME_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_STRING_T;
    pstInput[1].data.enum_val = strdup(pcTime);

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, OTDR_RPC_BASELINE_XPATH, pstInput, nInputCnt, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", OTDR_RPC_BASELINE_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(pstInput, nInputCnt);
    return DSC_ProcessRPCResponse(pstOutput, nOutputCnt);

ERR_LABEL:
    sr_free_values(pstInput, nInputCnt);
    sr_free_values(pstOutput, nOutputCnt);
    return rc;
}

int
DSC_OtdrGetTraceBaseline(char *pcKey, ST_OTDR_BASE_LINE_FILE *pstBaseLineFile)
{
    sr_val_t *pstInput = NULL, *pstOutput = NULL;
    size_t nOutputCnt = 0, nInputCnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pstBaseLineFile || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %p, %p",
                  pstBaseLineFile, pcKey);
        return SR_ERR_INVAL_ARG;
    }

    /* allocate input values filename*/
    nInputCnt = 2;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error %d", rc);
        return rc;
    }

    /* scan name: OTDR or OTDR-P */
    rc = sr_val_set_xpath(&pstInput[0], OTDR_RPC_SCAN_NAME_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_SCAN_NAME_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_STRING_T;
    pstInput[0].data.string_val = strdup(pcKey);

    /* scan operation: start or stop */
    rc = sr_val_set_xpath(&pstInput[1], OTDR_RPC_SCAN_OPERATION_XPATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OTDR_RPC_SCAN_OPERATION_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_ENUM_T;
    pstInput[1].data.enum_val = strdup("get");

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, OTDR_RPC_SCAN_XPATH, pstInput, nInputCnt, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", OTDR_RPC_SCAN_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (int i = 0; i < nOutputCnt; i++) {

        if ((NULL != strstr(pstOutput[i].xpath, "/result-file")) &&
            (NULL != strstr(pstOutput[i].data.string_val, OTDR_STR_EXTERNAL_PORT))) {
            strncpy(pstBaseLineFile->acBaseLineFile[OTDR_EXTERNAL_PORT],
                    pstOutput[i].data.string_val, sizeof(pstBaseLineFile->acBaseLineFile[OTDR_EXTERNAL_PORT]) - 1);
        }

        if ((NULL != strstr(pstOutput[i].xpath, "/result-file")) &&
            (NULL != strstr(pstOutput[i].data.string_val, OTDR_STR_LINE_IN_PORT))) {
            strncpy(pstBaseLineFile->acBaseLineFile[OTDR_LINE_IN_PORT],
                    pstOutput[i].data.string_val, sizeof(pstBaseLineFile->acBaseLineFile[OTDR_EXTERNAL_PORT]) - 1);
        }

        if ((NULL != strstr(pstOutput[i].xpath, "/result-file")) &&
            (NULL != strstr(pstOutput[i].data.string_val, OTDR_STR_LINE_OUT_PORT))) {
            strncpy(pstBaseLineFile->acBaseLineFile[OTDR_LINE_OUT_PORT],
                    pstOutput[i].data.string_val, sizeof(pstBaseLineFile->acBaseLineFile[OTDR_EXTERNAL_PORT]) - 1);
        }
    }

    if (strcmp(pstOutput[nOutputCnt - 2].data.enum_val, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error return value %s", pstOutput[nOutputCnt - 2].data.enum_val);
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

ERR_LABEL:
    sr_free_values(pstInput, nInputCnt);
    sr_free_values(pstOutput, nOutputCnt);
    return rc;
}

int
DSC_OtdrGetPortDesc(const char *pcPortName, OPTICAL_MODULE_DESC_ST *pstDesc)
{
    int rc = SR_ERR_OK;
    sr_val_t *val = NULL;
    size_t i = 0, nCnt = 0;
    sr_session_ctx_t *sess = NULL;
    char *pcNodeName = NULL;
    char acXpath[512] = {0};

    sess = DSC_GetRunningSession();

    if (NULL == pcPortName || NULL == pstDesc) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), COO_I_XPATH_OTDR_MODULE_ALL, pcPortName);

    rc = sr_get_items(sess, acXpath, 0, SR_OPER_DEFAULT, &val, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        pcNodeName = sr_xpath_node_name(val[i].xpath);
        if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_CIRCUIT_PACK_NAME)) {
            snprintf(pstDesc->acCircuitPackName, sizeof(pstDesc->acCircuitPackName), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_NAME)) {
            snprintf(pstDesc->acComponentName, sizeof(pstDesc->acComponentName), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_TYPE)) {
            snprintf(pstDesc->acComponentType, sizeof(pstDesc->acComponentType), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_INDEX)) {
            pstDesc->uiComponentIndex = val[i].data.uint32_val;
        }
    }

    sr_free_values(val, nCnt);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_OtdrGetPortCnt(const char *pcComponentKey)
{
    sr_val_t *pstValues = NULL;
    size_t tSize = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char key[32] = {0};
    int iCount = 0;
    int rc = SR_ERR_OK;

    memset(key, 0, sizeof(key));
    strncpy(key, pcComponentKey, sizeof(key) - 1);
    UTIL_LowerStr(key);

    snprintf(acXpath, sizeof(acXpath), COO_I_XPATH_OTDR_NAME_COMPONENT_ALL, key);

    rc = sr_get_items(DSC_GetRunningSession(), acXpath, 0, SR_OPER_DEFAULT, &pstValues, &tSize);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    iCount = (int)tSize;
    LOG_WRITE(EN_LOG_NOTICE, "DSC_OtdrGetPortCnt, port_count: %d", iCount);

cleanup:
    sr_free_values(pstValues, tSize);
    return iCount;
}
