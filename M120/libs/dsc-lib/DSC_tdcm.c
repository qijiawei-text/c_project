
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_tdcm.h"
#include "DSC_tca.h"



int
DSC_GetTdcmCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "tdcms", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

int
DSC_SetTdcmFrequency(char *pcCfgKey, double d32Frequency)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_FREQUENCY, pcCfgKey);
    rc = set_decimal64_item(sess, acSelectXpath, d32Frequency);
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
DSC_SetTdcmDispersn(char *pcCfgKey, double d32Dispersn)
{
    return DSC_SetTdcmDispersn_Commit(pcCfgKey, d32Dispersn, true);
}

int
DSC_SetTdcmDispersn_Commit(char *pcCfgKey, double d32Dispersn, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_DISPERSION, pcCfgKey);
    rc = set_decimal64_item(sess, acSelectXpath, d32Dispersn);
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
DSC_SetTdcmDispersnStep(char *pcCfgKey, double dRoughStep, double dSmoothStep)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    rc = set_decimal64_item(sess, TDCM_XPATH_DISPERSION_ROUGH_STEP, dRoughStep);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_decimal64_item(sess, TDCM_XPATH_DISPERSION_SMOOTH_STEP, dSmoothStep);
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
DSC_GetTdcmDispersnStep(char *pcCfgKey, double *pdRoughStep, double *pdSmoothStep)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (NULL == pdRoughStep || NULL == pdSmoothStep) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    rc = get_decimal64_item(sess, TDCM_XPATH_DISPERSION_ROUGH_STEP, pdRoughStep);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_decimal64_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = get_decimal64_item(sess, TDCM_XPATH_DISPERSION_SMOOTH_STEP, pdSmoothStep);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_decimal64_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetTdcmDispersn(char *pcCfgKey, double *pdDispersn)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (NULL == pdDispersn) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_DISPERSION, TDCM_CFG_KEY);
    rc = get_decimal64_item(sess, acSelectXpath, pdDispersn);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_decimal64_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetTdcmFrequency(char *pcCfgKey, double *pdFrequency)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    if (NULL == pdFrequency) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_FREQUENCY, pcCfgKey);
    rc = get_decimal64_item(sess, acSelectXpath, pdFrequency);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_decimal64_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetTdcmInfo(char *pcCfgKey, ST_TDCM_INFO *psTdcmInfo, size_t size)
{
#if 0
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    ANONYMOUS_ST astTdcmMaps[] = {
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_CHANNEL_NUMBER, SR_UINT32_T, (char *)(&psTdcmInfo->u32ChlNumber)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_CHANNEL_SPACING, SR_DECIMAL64_T, (char *)(&psTdcmInfo->dChlSpacing)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_DISPERSION_ACTUAL, SR_DECIMAL64_T, (char *)(&psTdcmInfo->dDispersnActual)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_FIRST_FREQNCY, SR_DECIMAL64_T, (char *)(&psTdcmInfo->dFirstFreqncy)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_DISPERSION_MIN, SR_DECIMAL64_T, (char *)(&psTdcmInfo->dMinDispersion)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_DISPERSION_MAX, SR_DECIMAL64_T, (char *)(&psTdcmInfo->dMaxDispersion)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_ACTIVE, SR_BOOL_T, (char *)(&psTdcmInfo->bActive)),
        ANONYMOUS_MAP_ENTRY(TDCM_XPATH_READY, SR_BOOL_T, (char *)(&psTdcmInfo->bReady)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == psTdcmInfo || size < sizeof(ST_TDCM_INFO)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length: %zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)psTdcmInfo, 0x00, size);
    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_MONITOR_ALL, TDCM_CFG_KEY);
    rc = DSC_GetItems(sess, acSelectXpath, astTdcmMaps, sizeof(astTdcmMaps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetItems path : %s", acSelectXpath);
    }

    return rc;
#endif

#if 1
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_FIRST_FREQNCY, pcCfgKey);
    rc = get_decimal64_item(pstSess, acXpath, &psTdcmInfo->dFirstFreqncy);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_decimal64_item : %s, xpath : %s", sr_strerror(rc), acXpath);
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TDCM_XPATH_DISPERSION_ACTUAL, pcCfgKey);
    rc = get_decimal64_item(pstSess, acXpath, &psTdcmInfo->dDispersnActual);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_decimal64_item : %s, xpath : %s", sr_strerror(rc), acXpath);
        return rc;
    }

    return rc;
#endif
}

int
DSC_TdcmScan(double dStart, double dStop, double dStep)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'stop-dispersion" leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_TDCM_SCAN_START_DISPERSN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_TDCM_SCAN_START_DISPERSN, sr_strerror(rc));
        return rc;
    }
    input[0].type = SR_DECIMAL64_T;
    input[0].data.decimal64_val = dStart;

    /* set 'start-dispersion' leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_TDCM_SCAN_STOP_DISPERSN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_TDCM_SCAN_STOP_DISPERSN, sr_strerror(rc));
        return rc;
    }
    input[1].type = SR_DECIMAL64_T;
    input[1].data.decimal64_val = dStop;

    /* set 'disprsn-step' leaf */
    rc = sr_val_set_xpath(&input[2], RPC_XPATH_TDCM_SCAN_STEP);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_TDCM_SCAN_STEP, sr_strerror(rc));
        return rc;
    }
    input[2].type = SR_DECIMAL64_T;
    input[2].data.decimal64_val = dStep;

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_TDCM_SCAN, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_TDCM_SCAN, sr_strerror(rc));
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_TdcmScanBySpanloss(double dSpanloss)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/span-loss' leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_TDCM_SCAN_SPANLOSS);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_TDCM_SCAN_SPANLOSS, sr_strerror(rc));
        return rc;
    }
    input[0].type = SR_DECIMAL64_T;
    input[0].data.decimal64_val = dSpanloss;

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_TDCM_SCAN, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_TDCM_SCAN, sr_strerror(rc));
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

