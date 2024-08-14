
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_dfb.h"
#include "DSC_tca.h"

char aacDfbName[EN_DFB_IDX_MAX][16] = {"dfb-hp"};

static int m_iDfbCnt = 0;
static DFB_ST m_astDfb[EN_DFB_IDX_MAX] = {0};
static DFB_KEY_ST m_stDfbKeys = {0, {{0}}};

int
DSC_GetDfbArrayLength()
{
    return sizeof(m_astDfb) / sizeof(m_astDfb[0]);
}

int
DSC_GetDfbCnt()
{
    int rc = SR_ERR_OK;
    sr_data_t *pstSubtrees = NULL;
    size_t nCnt = 0;
    sr_session_ctx_t *pstSess = NULL;
    static bool m_bRead = false;
    size_t nLeafCnt = 0;
    sr_node_term_t astLeaf[DEF_LEAF_NAX_CNT] = {0};
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    if (m_bRead) {
        return m_iDfbCnt;
    }

    pstSess = DSC_GetRunningSession();

    rc = get_subtrees(pstSess, DFB_XPATH_DFB_LIST, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, DFB_XPATH_DFB_LIST_NAME, &nodeSet);
        if (LY_SUCCESS == rc) {
            nCnt = nodeSet->count;
        }
    }
    if (0 == nCnt) {
        LOG_WRITE(EN_LOG_WARNING, "port count 0");
        goto cleanup;
    }
    for (int i = 0; i < nCnt; i++) {
        memset(astLeaf, 0, sizeof(astLeaf));
        nLeafCnt = DSC_IterateTreeLeaf(nodeSet->dnodes[i], astLeaf, DEF_LEAF_NAX_CNT);
        for (int j = 0; j < nLeafCnt; j++) {
            if (0 == strcmp(astLeaf[j].schema->name, "name")) {
                m_astDfb[i].ucId = i + 1;
                pstLeaf = (sr_node_t *)&(astLeaf[j]);
                strncpy(m_astDfb[i].acName, lyd_get_value(pstLeaf), sizeof(m_astDfb[i].acName) - 1);
            }
        }
    }

cleanup:

    m_bRead = true;

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    m_iDfbCnt = nCnt;

    LOG_WRITE(EN_LOG_NOTICE, "First read dfb count %d", m_iDfbCnt);
    return (int)nCnt;
}

int
DSC_GetSingleDfbCnt(const char *pcComponentKey)
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

    snprintf(acXpath, sizeof(acXpath), COO_I_XPATH_DFB_NAME_COMPONENT_ALL, key);

    rc = sr_get_items(DSC_GetRunningSession(), acXpath, 0, SR_OPER_DEFAULT, &pstValues, &tSize);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    iCount = (int)tSize;
    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetDfbCnt, port_count: %d", iCount);

cleanup:
    sr_free_values(pstValues, tSize);
    return iCount;
}

int
DSC_GetDfbCfgKeyFromDb(DFB_KEY_ST *pstDfbKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;

    if (NULL == pstDfbKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstDfbKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetStartupSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, DFB_XPATH_DFB_LIST_NAME, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "OSC key %d: %s", i, pstValues[i].data.string_val);
        snprintf(pstDfbKey->aacDfbKeys[i], sizeof(pstDfbKey->aacDfbKeys[i]), "%s", pstValues[i].data.string_val);
    }
    pstDfbKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_GetDfbCfgKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "dfb", "name", 32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

int
DSC_GetDfbDevIdByKey(char *pcCfgKey, int *piDevId)
{
    int rc = SR_ERR_OK;

    if (0 == strcmp(pcCfgKey, DFB_CFG_KEY)) {
        *piDevId = 0;
    } else if (0 == strcmp(pcCfgKey, DFB_P_CFG_KEY)) {
        *piDevId = 1;
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetDfbDevIdByKey pcCfgKey : %s not find devid.", pcCfgKey);
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_GetDfbAllName(char aacName[][32], size_t *pnCount)
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
    rc = get_items(pstSess, DFB_XPATH_DFB_LIST_NAME, &pstVal, pnCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items %s: %s", DFB_XPATH_DFB_LIST_NAME, sr_strerror(rc));
        goto cleanup;
    }

    for (nIndex = 0; nIndex < (*pnCount); nIndex++) {
        if (NULL != strstr(pstVal[nIndex].xpath, "/name")) {
            j++;
            if (j >= EN_DFB_IDX_MAX) {
                break;
            }
            strncpy(aacName[j], pstVal[nIndex].data.string_val, sizeof(aacName[j]) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "DFB name ---- %s", pstVal[nIndex].data.string_val);
        }
    }

cleanup:
    sr_free_val(pstVal);
    return rc;
}

int
CSM_DfbGetCfgKeyFromDb()
{
    return DSC_GetDfbCfgKeyFromDb(&m_stDfbKeys);
}

/* only get Dfb state info */
int
DSC_GetDfbInfo(char *pcKey, DFB_INFO_ST *pstDfbInfo, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acPort[EN_DFB_PORT_ID_MAX][OPLK_PATH_MAX_LEN] = {"line-in", "line-out", "mux-in", "mux-out"};

    memset(xpath, 0, sizeof(xpath));
    memset(pstDfbInfo, 0, sizeof(DFB_INFO_ST));

    ANONYMOUS_ST dfb_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], DFB_XPATH_STATE_TARGET_OUTPUT_POWER, pcKey), SR_DECIMAL64_T, (char *)(&pstDfbInfo->dTargetOutputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[3], DFB_XPATH_STATE_DFB_PORT_INPUT_POWER, pcKey, acPort[0]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbLineInMon.dInputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[4], DFB_XPATH_STATE_DFB_PORT_OUTPUT_POWER, pcKey, acPort[1]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbLineOutMon.dOutputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[5], DFB_XPATH_STATE_DFB_PORT_INPUT_POWER, pcKey, acPort[2]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbMuxInMon.dInputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[6], DFB_XPATH_STATE_DFB_PORT_OUTPUT_POWER, pcKey, acPort[2]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbMuxInMon.dOutputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[7], DFB_XPATH_STATE_DFB_PORT_INPUT_POWER, pcKey, acPort[3]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbMuxOutMon.dInputPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr2(xpath[8], DFB_XPATH_STATE_DFB_PORT_OUTPUT_POWER, pcKey, acPort[3]), SR_DECIMAL64_T, (char *)(&pstDfbInfo->stDfbMuxOutMon.dOutputPower)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstDfbInfo || size < sizeof(DFB_INFO_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    /* snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, DFB_XPATH_STATE_DFBS_ALL, pcKey); */
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, "%s", DFB_XPATH_STATE_DFBS_ALL);
    rc = DSC_GetItems(sess, acSelectXpath, dfb_maps, sizeof(dfb_maps));

    LOG_WRITE(EN_LOG_NOTICE, "rc:%d, dTargetOutputPower:%f, dInputPower:%f, dOutputPower:%f", rc, pstDfbInfo->dTargetOutputPower, pstDfbInfo->stDfbLineInMon.dInputPower, pstDfbInfo->stDfbLineOutMon.dOutputPower);

    return rc;
}

int
DSC_GetDfbConfig(char *pcKey, DFB_CONF_ST *pstDfbCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t nCount = 0;

    if (NULL == pstSess || NULL == pcKey || NULL == pstDfbCfg) {
        LOG_WRITE(EN_LOG_NOTICE, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    memset(xpath, 0, sizeof(xpath));
    ANONYMOUS_ST dfb_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], DFB_XPATH_CONFIG_ENABLED, pcKey), SR_BOOL_T, (char *)(&pstDfbCfg->bEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], DFB_XPATH_CONFIG_TARGET_OUTPUT_POWER, pcKey), SR_DECIMAL64_T, (char *)(&pstDfbCfg->dTargetOutputPower)),
    };

    snprintf(acXpath, OPLK_PATH_MAX_LEN, DFB_XPATH_CONFIG_ALL, pcKey);
    rc = DSC_GetItems(pstSess, acXpath, dfb_maps, sizeof(dfb_maps));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* Get threshold from TCA                                                   */
    rc = DSC_GetTcaList(NULL, &pstTcaInfo, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
        goto END_LABEL;
    }

    rc = DSC_GetTcaThresh(TCA_IDX_LASERSAFETY_RX_LOS, pstTcaInfo, &(pstDfbCfg->dLaserSafetyRHighThr), &(pstDfbCfg->dLaserSafetyRLowThr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca lasersafety threshold error, tca id: %d", TCA_IDX_LASERSAFETY_RX_LOS);
        goto END_LABEL;
    }
    LOG_WRITE(EN_LOG_NOTICE, "dLaserSafetyHighThr: %f, dLaserSafetyLowThr: %f", pstDfbCfg->dLaserSafetyRHighThr, pstDfbCfg->dLaserSafetyRLowThr);

    rc = DSC_GetTcaThresh(TCA_IDX_LASERSAFETY_TX_LOS, pstTcaInfo, &(pstDfbCfg->dLaserSafetyTHighThr), &(pstDfbCfg->dLaserSafetyTLowThr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca lasersafety threshold error, tca id: %d", TCA_IDX_LASERSAFETY_TX_LOS);
        goto END_LABEL;
    }
    LOG_WRITE(EN_LOG_NOTICE, "dLaserSafetyHighThr: %f, dLaserSafetyLowThr: %f", pstDfbCfg->dLaserSafetyTHighThr, pstDfbCfg->dLaserSafetyTLowThr);

END_LABEL:

    return rc;
}

int
DSC_SetDfbOutputEnable(char *pcKey, bool state, double dTargetOutputPower)
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
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, DFB_XPATH_CONFIG_ENABLED, pcKey);
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetDfbOutputEnable xpath: %s, state : %s", acSelectXpath, state ? "enable" : "disable");
    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, DFB_XPATH_CONFIG_TARGET_OUTPUT_POWER, pcKey);
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetDfbOutputEnable xpath: %s, dTargetOutputPower : %f", acSelectXpath, dTargetOutputPower);
    rc = set_decimal64_item(sess, acSelectXpath, dTargetOutputPower);
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
DSC_SetLaserSafetyRxLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, DFB_CFG_KEY)) {
        iTcaIdx = TCA_IDX_LASERSAFETY_RX_LOS;
    } else if (0 == strcmp(pcKey, DFB_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_LASERSAFETY_P_RX_LOS;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s lasersafety rx los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetLaserSafetyTxLosThreshold(char *pcKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaIdx = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcKey, DFB_CFG_KEY)) {
        iTcaIdx = TCA_IDX_LASERSAFETY_TX_LOS;
    } else if (0 == strcmp(pcKey, DFB_P_CFG_KEY)) {
        iTcaIdx = TCA_IDX_LASERSAFETY_P_TX_LOS;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s lasersafety tx los thresh: %s", pcKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

