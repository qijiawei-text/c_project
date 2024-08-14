
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_osc.h"
#include "DSC_tca.h"
#include "DSC_circuitpack.h"
#include "DSC_edfa.h"

int
DSC_GetOscCfgKeyFromDb(OSC_KEY_ST *pstOscKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;

    if (NULL == pstOscKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstOscKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetStartupSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, OSC_XPATH_NAME, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        snprintf(pstOscKey->aacOscKeys[i], sizeof(pstOscKey->aacOscKeys[i]), "%s", pstValues[i].data.string_val);
    }
    pstOscKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_GetOscCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "supervisory-channel", "interface", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

int
DSC_GetOscDevIdByKey(char *pcCfgKey, int *piDevId)
{
    int rc = SR_ERR_OK;

    if (0 == strcmp(pcCfgKey, OSC_CFG_KEY)) {
        *piDevId = 0;
    } else if (0 == strcmp(pcCfgKey, OSC_P_CFG_KEY)) {
        *piDevId = 1;
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetOscDevIdByKey pcCfgKey : %s not find devid.", pcCfgKey);
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_GetOscAllName(char aacName[][32], size_t *pnCount)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    size_t nIndex = 0;
    int j = -1;

    if (NULL == pnCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    *pnCount = 0;
    rc = get_items(pstSess, OSC_XPATH_NAME, &pstVal, pnCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items %s: %s", OSC_XPATH_NAME, sr_strerror(rc));
        goto cleanup;
    }

    for (nIndex = 0; nIndex < (*pnCount); nIndex++) {
        if (NULL != strstr(pstVal[nIndex].xpath, "/interface")) {
            j++;
            if (j >= EN_OSC_IDX_MAX) {
                break;
            }
            strncpy(aacName[j], pstVal[nIndex].data.string_val, sizeof(aacName[j]) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "OSC name ---- %s", pstVal[nIndex].data.string_val);
        }
    }

cleanup:
    sr_free_val(pstVal);
    return rc;
}

/* only get osc state info */
int
DSC_GetOscInfo(char *pcKey, OSC_INFO_ST *pstOscInfo, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    int iTcaRxIdx = 0;
    int iTcaTxIdx = 0;
    int iTcaAddIdx = 0;
    int iTcaDropIdx = 0;

    memset(xpath, 0, sizeof(xpath));
    memset(pstOscInfo, 0, sizeof(OSC_INFO_ST));

    ANONYMOUS_ST osc_maps[] = {
        /*ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OSC_XPATH_TX_OC3_ENABLE, pcKey), SR_BOOL_T, (char *)(&pstOscInfo->stOscTxConf.bEnable)),*/
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OSC_XPATH_RX_PWR, pcKey), SR_DECIMAL64_T, (char *)(&pstOscInfo->stOscRxMon.dPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OSC_XPATH_TX_PWR, pcKey), SR_DECIMAL64_T, (char *)(&pstOscInfo->stOscTxMon.dPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OSC_XPATH_DROP_PWR, pcKey), SR_DECIMAL64_T, (char *)(&pstOscInfo->stOscDropMon.dPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], OSC_XPATH_ADD_PWR, pcKey), SR_DECIMAL64_T, (char *)(&pstOscInfo->stOscAddMon.dPower)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstOscInfo || size < sizeof(OSC_INFO_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OSC_XPATH_STATE_ALL, pcKey);
    rc = DSC_GetItems(sess, acSelectXpath, osc_maps, sizeof(osc_maps));

    /* NOT support threshold in OLSP. TODO: suggest to create new interface for OSC threshold */
#if 1
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

        if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
            iTcaRxIdx = TCA_IDX_OSC1_RX;
            iTcaTxIdx = TCA_IDX_OSC1_TX;
            iTcaAddIdx = TCA_IDX_OSC1_ADD;
            iTcaDropIdx = TCA_IDX_OSC1_DROP;
        } else {
            iTcaRxIdx = TCA_IDX_OSC2_RX;
            iTcaTxIdx = TCA_IDX_OSC2_TX;
            iTcaAddIdx = TCA_IDX_OSC2_ADD;
            iTcaDropIdx = TCA_IDX_OSC2_DROP;
        }

        DSC_GetTcaThresh(iTcaRxIdx,
                         pstTcaInfo,
                         &(pstOscInfo->stOscRxConf.dHighThr),
                         &(pstOscInfo->stOscRxConf.dLowThr));
        DSC_GetTcaThresh(iTcaTxIdx,
                         pstTcaInfo,
                         &(pstOscInfo->stOscTxConf.dHighThr),
                         &(pstOscInfo->stOscTxConf.dLowThr));
        DSC_GetTcaThresh(iTcaAddIdx,
                         pstTcaInfo,
                         &(pstOscInfo->stOscAddConf.dHighThr),
                         &(pstOscInfo->stOscAddConf.dLowThr));
        DSC_GetTcaThresh(iTcaDropIdx,
                         pstTcaInfo,
                         &(pstOscInfo->stOscDropConf.dHighThr),
                         &(pstOscInfo->stOscDropConf.dLowThr));
    }
#endif
#if 0
/*  1.01.0001 */
    if (0 == strcasecmp(AMP_OSC_KEY, pcKey)) {
        rc = DSC_GetEvoaEnable(AMP_OSC_TX_VOA_KEY, &pstOscInfo->stOscAddConf.bEnable);
    } else if (0 == strcasecmp(AMP_P_OSC_KEY, pcKey)) {
        rc = DSC_GetEvoaEnable(AMP_P_OSC_TX_VOA_KEY, &pstOscInfo->stOscAddConf.bEnable);
    } else {
    }

    if (0 == strcasecmp(AMP_OSC_KEY, pcKey)) {
        rc = DSC_GetEvoaTarget(AMP_OSC_TX_VOA_KEY, &pstOscInfo->stOscAddConf.dPowerSetPoint);
    } else if (0 == strcasecmp(AMP_P_OSC_KEY, pcKey)) {
        rc = DSC_GetEvoaTarget(AMP_P_OSC_TX_VOA_KEY, &pstOscInfo->stOscAddConf.dPowerSetPoint);
    } else {
    }
#else
    /*  1.02.000x */
    if (0 == strcasecmp(AMP_OSC_KEY, pcKey)) {
        rc = DSC_GetEdfaOutputEnableByName(AMP_OSC_KEY, &pstOscInfo->stOscAddConf.bEnable);
    } else if (0 == strcasecmp(AMP_P_OSC_KEY, pcKey)) {
        rc = DSC_GetEdfaOutputEnableByName(AMP_P_OSC_KEY, &pstOscInfo->stOscAddConf.bEnable);
    } else {
    }

    if (0 == strcasecmp(AMP_OSC_KEY, pcKey)) {
        rc = DSC_GetEdfaOutputPowerByName(AMP_OSC_KEY, &pstOscInfo->stOscAddConf.dPowerSetPoint);
    } else if (0 == strcasecmp(AMP_P_OSC_KEY, pcKey)) {
        rc = DSC_GetEdfaOutputPowerByName(AMP_P_OSC_KEY, &pstOscInfo->stOscAddConf.dPowerSetPoint);
    } else {
    }
#endif
    return rc;
}

int
DSC_GetOscConfig(char *pcKey, OSC_CONF_ST *pstOscCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pcKey || NULL == pstOscCfg) {
        LOG_WRITE(EN_LOG_NOTICE, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    snprintf(acXpath, OPLK_PATH_MAX_LEN, OSC_XPATH_TX_OC3_ENABLE, pcKey);

    rc = get_bool_item(pstSess, acXpath, &pstOscCfg->bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_bool_item(%s): %s", acXpath, sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

int
DSC_SetOscOutputEnable(char *pcKey, bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OSC_XPATH_TX_OC3_ENABLE, pcKey);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetOscOutputEnable xpath: %s, state : %s", acSelectXpath, state ? "enable" : "disable");

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
DSC_SetOscTxLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC1_TX;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC2_TX;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscRxLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC1_RX;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC2_RX;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscAddLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC1_ADD;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC2_ADD;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s add los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscDropLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC1_DROP;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC2_DROP;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s drop los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscRxLowThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_RX_LOW;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_RX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx low thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscRxHighThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_RX_HIGH;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_RX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s rx high thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscTxLowThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_TX_LOW;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_TX_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx low thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscTxHighThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_TX_HIGH;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_TX_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s tx high thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscDropLowThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_DROP_LOW;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_DROP_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s drop low thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscDropHighThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_DROP_HIGH;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_DROP_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s drop high thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscAddLowThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_ADD_LOW;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_ADD_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s add low thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetOscAddHighThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, OSC_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_ADD_HIGH;
    } else if (0 == strcmp(pcKey, OSC_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_OSC_P_ADD_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s add high thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetSpanlossVariationLowThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, DEF_SPANLOSS_CFG_KEY)) {
        iTcaIdx = TCA_IDX_SPANLOSS_VARIATION_LOW;
    } else if (0 == strcmp(pcKey, DEF_SPANLOSS_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_SPANLOSS_P_VARIATION_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s variation low thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetSpanlossVariationHighThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, DEF_SPANLOSS_CFG_KEY)) {
        iTcaIdx = TCA_IDX_SPANLOSS_VARIATION_HIGH;
    } else if (0 == strcmp(pcKey, DEF_SPANLOSS_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_SPANLOSS_P_VARIATION_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s variation high thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

