/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "ieee_float.h"

#include "DSC_waveplan.h"
#include "DSC_ocm.h"
#include "DSC_tca.h"


static int m_iOcmPortCnt = 0;

static OCM_PORT_ST m_astOcmPort[EN_OCM_PORT_ID_MAX] = {0};

int
DSC_GetOcmPortArrayLength()
{
    return sizeof(m_astOcmPort) / sizeof(m_astOcmPort[0]);
}

int
DSC_GetOcmPortCnt()
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
        return m_iOcmPortCnt;
    }

    pstSess = DSC_GetRunningSession();

    rc = get_subtrees(pstSess, OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR_LIST, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR_LIST_NAME, &nodeSet);
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
                m_astOcmPort[i].ucId = i + 1;
                pstLeaf = (sr_node_t *)&(astLeaf[j]);
                strncpy(m_astOcmPort[i].acName, lyd_get_value(pstLeaf), sizeof(m_astOcmPort[i].acName) - 1);
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

    m_iOcmPortCnt = nCnt;

    LOG_WRITE(EN_LOG_NOTICE, "First read ocm port count %d", m_iOcmPortCnt);
    return (int)nCnt;
}

int
DSC_GetSingleOcmPortCnt(const char *pcComponentKey)
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

    snprintf(acXpath, sizeof(acXpath), COO_I_XPATH_OCM_NAME_COMPONENT_ALL, key);

    rc = sr_get_items(DSC_GetRunningSession(), acXpath, 0, SR_OPER_DEFAULT, &pstValues, &tSize);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    iCount = (int)tSize;
    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetOcmPortCnt, port_count: %d", iCount);

cleanup:
    sr_free_values(pstValues, tSize);
    return iCount;
}


int
DSC_GetOcmCfgKeyFromDb(sr_session_ctx_t *pstSess, OCM_KEY_ST *pstOcmKey)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSessTmp = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;

    if (NULL == pstOcmKey) {
        LOG_WRITE(EN_LOG_ERROR, "pstOcmKey is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSessTmp = (NULL == pstSess) ?  DSC_GetStartupSession() : pstSess;
    if (NULL == pstSessTmp) {
        LOG_WRITE(EN_LOG_ERROR, "get startup sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSessTmp, OCM_OPENCONFIG_XPATH_NAME_ALL, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "ocm key %d: %s", i, pstValues[i].data.string_val);
        snprintf(pstOcmKey->aacOcmKeys[i], sizeof(pstOcmKey->aacOcmKeys[i]), "%s", pstValues[i].data.string_val);
        /*strncpy(pstOcmKey->aacOcmKeys[i], pstValues[i].data.string_val, sizeof(pstOcmKey->aacOcmKeys[i]) - 1);*/
    }
    pstOcmKey->iCnt = tCount;

    sr_free_values(pstValues, tCount);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}


char *
DSC_GetOcmPortName(unsigned char ucPortId)
{
    size_t nCount = 0;
    size_t i = 0;

    nCount = DSC_GetOcmPortCnt();
    LOG_WRITE(EN_LOG_TRACE, "port count: %d", nCount);
    for (i = 0; i < nCount; i++) {
        if (ucPortId == m_astOcmPort[i].ucId) {
            return m_astOcmPort[i].acName;
        }
    }

    return OCM_PORT_UNKNOWN_STR;
}


unsigned char
DSC_GetOcmPortIdByNameNew(char *pcPortName)
{
    size_t nCount = 0;
    size_t i = 0;

    if (NULL == pcPortName) {
        return EN_OCM_PORT_NONE;
    }

    nCount = DSC_GetOcmPortCnt();

    for (i = 0; i < nCount; i++) {
        if (0 == strcmp(m_astOcmPort[i].acName, pcPortName)) {
            return m_astOcmPort[i].ucId;
        }
    }

    return EN_OCM_PORT_NONE;
}

int
DSC_GetOcmPortNameByXpath(char *pcXpath, char *pcKey, size_t nLen)
{
    char *pcKeyTmp = NULL;
    sr_xpath_ctx_t state = {0};
    size_t i = 0;
    char cChar;

    if (NULL == pcXpath || NULL == pcKey || nLen < 1) {
        LOG_WRITE(EN_LOG_ERROR, "Para error");
        return SR_ERR_INVAL_ARG;
    }

    pcKeyTmp = sr_xpath_key_value(pcXpath, OCM_NODE_CHANNEL_MONITOR, OCM_NODE_NAME, &state);
    if (NULL == pcKeyTmp) {
        return SR_ERR_NOT_FOUND;
    }

    /* example: pcKeyTmp = "pre-amp-input']/channels" */

    memset(pcKey, 0, nLen);
    for (i = 0; (i < nLen - 1) && (*(pcKeyTmp + i) != '\0'); i++) {
        cChar = *(pcKeyTmp + i);
        if ('\'' == cChar) {
            break;
        } else {
            *(pcKey + i) = cChar;
        }
    }

    sr_xpath_recover(&state);

    return SR_ERR_OK;
}

/* caller must free pstList->pstChannelArray */
int
DSC_GetOcmChannelsByPort(char *pcCfgKey, char *pcPortName, OCM_CHANNELS_LIST_ST *pstList)
{
    int rc = SR_ERR_OK;
    sr_val_t *val = NULL;
    size_t i = 0, nCnt = 0, nActualNum = -1, nChannelTotal = 0;
    sr_session_ctx_t *sess = NULL;
    char *pcNodeName = NULL;
    char acXpath[512] = {0};
    WAVEPLAN_ST *pstWaveplan = NULL;
    /* OCM_CHANNEL_ST stChannel; */

    sess = DSC_GetOperSession();
    if (NULL == pcPortName || NULL == pstList) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    pstList->pstChannelArray = NULL;     /* init */
    rc = DSC_GetOcmWaveplanInfo(&pstWaveplan, &nChannelTotal);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmWaveplanInfo: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }
    if (0 == nChannelTotal) {
        LOG_WRITE(EN_LOG_ERROR, "nChannelTotal is 0");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    pstList->pstChannelArray = (OCM_CHANNEL_ST *)malloc(sizeof(OCM_CHANNEL_ST) * nChannelTotal);
    if (NULL == pstList->pstChannelArray) {
        LOG_WRITE(EN_LOG_ERROR, "nChannelTotal is 0");
        rc = ERR_NOMEM;
        goto ERR_LABEL;
    }
    snprintf(acXpath, sizeof(acXpath), OCM_OPENCONFIG_XPATH_CHANNELS_ALL, pcPortName);
    rc = sr_get_items(sess, acXpath, 0, 0, &val, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s. xpath: %s", sr_strerror(rc), acXpath);
        goto ERR_LABEL;
    }
    pstList->nNum = nChannelTotal;
    memset(pstList->pstChannelArray, 0, sizeof(OCM_CHANNEL_ST) * nChannelTotal);
    for (i = 0; i < nCnt; i++) {
        if (SR_LIST_T == val[i].type) {
            nActualNum++;
            continue;
        }

        if (nActualNum < 0 || nActualNum >= nChannelTotal) {
            LOG_WRITE(EN_LOG_ERROR, "nActualNum is out of range. nActualNum: %d, nChannelTotal: %lu", (int)nActualNum, nChannelTotal);
            break;
        }

        pcNodeName = sr_xpath_node_name(val[i].xpath);

        if (0 == strcmp(pcNodeName, OCM_NODE_LOWER_FREQUENCY)) {
            pstList->pstChannelArray[nActualNum].ulLowFrequency = val[i].data.uint64_val;
        } else if (0 == strcmp(pcNodeName, OCM_NODE_UPPER_FREQUENCY)) {
            pstList->pstChannelArray[nActualNum].ulHighFrequency = val[i].data.uint64_val;
            /* generate channel-no by upper frequency */
            strncpy(pstList->pstChannelArray[nActualNum].acChannelNo, pstWaveplan[nActualNum].acChannelName, OCM_CHANNEL_NO_MAX_LEN);

        } else if (0 == strcmp(pcNodeName, OCM_NODE_POWER)) {
            pstList->pstChannelArray[nActualNum].dPower = val[i].data.decimal64_val;
        }

    }

    /* check data number */
    if (nChannelTotal != nActualNum + 1 ) {
        LOG_WRITE(EN_LOG_ERROR, "Error by parsing channels. nChannelTotal: %lu, nActualNum: %lu", nChannelTotal, nActualNum);
        rc = SR_ERR_VALIDATION_FAILED;
        goto ERR_LABEL;
    }

#if 0
    /* channel 1 swap channel 2? */
    for (i = 1; i < pstList->nNum; ) {
        memset(&stChannel, 0, sizeof(stChannel));
        memcpy(&stChannel, &(pstList->pstChannelArray[i - 1]), sizeof(OCM_CHANNEL_ST));
        memcpy(&(pstList->pstChannelArray[i - 1]), &(pstList->pstChannelArray[i]), sizeof(OCM_CHANNEL_ST));
        memcpy(&(pstList->pstChannelArray[i]), &stChannel, sizeof(OCM_CHANNEL_ST));
        i += 2;
    }
#endif
    sr_free_values(val, nCnt);
    DEF_SAFE_FREE(pstWaveplan);
    return SR_ERR_OK;
ERR_LABEL:
    if (NULL != pstList) {
        DEF_SAFE_FREE(pstList->pstChannelArray);
    }

    if (val != NULL) {
        sr_free_values(val, nCnt);
    }
    DEF_SAFE_FREE(pstWaveplan);
    return rc;
}

int
DSC_GetOcmPortConfig(char *pcCfgKey, char *pcPortName, OCM_PORT_DESC_ST *pstPortDesc)
{
    int rc = SR_ERR_OK;
    sr_val_t *val = NULL;
    size_t i = 0, nCnt = 0;
    sr_session_ctx_t *sess = NULL;
    char *pcNodeName = NULL;
    char acXpath[512] = {0};

    sess = DSC_GetRunningSession();

    if (NULL == pcPortName || NULL == pstPortDesc) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), OCM_OPENCONFIG_XPATH_CONFIG_ALL, pcPortName);
    LOG_WRITE(EN_LOG_TRACE, "pcPortName: %s", pcPortName);
    LOG_WRITE(EN_LOG_TRACE, "acXpath: %s", acXpath);

    rc = sr_get_items(sess, acXpath, 0, SR_OPER_DEFAULT, &val, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        pcNodeName = sr_xpath_node_name(val[i].xpath);
        if (0 == strcmp(pcNodeName, OCM_NODE_NAME)) {
            snprintf(pstPortDesc->acName, sizeof(pstPortDesc->acName), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, OCM_NODE_MONITOR_PORT)) {
            snprintf(pstPortDesc->acMonitorPort, sizeof(pstPortDesc->acMonitorPort), "%s", val[i].data.string_val);
        }
    }

    sr_free_values(val, nCnt);


    /* ATTENTION:
       We add component type and index in /com-oplink-optical-i:optical-control/optical-modules.
       The purpose of these arguments is to make a general method to solve the hard-coded convertion for HAL.

       eg. OCM port LineIn, the component type is DEV_TYPE_OCM, port index is 0, then we can pass these to HAL directly.
       No need to convert or figure them additionally.
     */
    snprintf(acXpath, sizeof(acXpath), COO_I_XPATH_OCM_MODULE_ALL, pcPortName);

    rc = sr_get_items(sess, acXpath, 0, SR_OPER_DEFAULT, &val, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        pcNodeName = sr_xpath_node_name(val[i].xpath);
        if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_CIRCUIT_PACK_NAME)) {
            snprintf(pstPortDesc->acCircuitPackName, sizeof(pstPortDesc->acCircuitPackName), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_NAME)) {
            snprintf(pstPortDesc->acComponentName, sizeof(pstPortDesc->acComponentName), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_TYPE)) {
            snprintf(pstPortDesc->acComponentType, sizeof(pstPortDesc->acComponentType), "%s", val[i].data.string_val);
        } else if (0 == strcmp(pcNodeName, COO_I_OPTICAL_MODULE_COMPONENT_INDEX)) {
            pstPortDesc->uiComponentIndex = val[i].data.uint32_val;
        }
    }

    sr_free_values(val, nCnt);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_SetOcmConfig(char *pcCfgKey, OCM_PORT_DESC_ST *pstOcmPortDesc)
{
    signed int rc = SR_ERR_OK;
    char acXpath[512] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstOcmPortDesc) {
        LOG_WRITE(EN_LOG_ERROR, "pstOcmPortDesc is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2.  add or modify a list */
    snprintf(acXpath, sizeof(acXpath), OCM_OPENCONFIG_XPATH_CONFIG_NAME, pstOcmPortDesc->acName);
    rc = set_string_item(pstSess, acXpath, pstOcmPortDesc->acName, sizeof(pstOcmPortDesc->acName));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-name: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), OCM_OPENCONFIG_XPATH_CONFIG_MONITOR_PORT, pstOcmPortDesc->acName);
    rc = set_string_item(pstSess, acXpath, pstOcmPortDesc->acMonitorPort, sizeof(pstOcmPortDesc->acMonitorPort));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-monitor-port: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_UpdateOcmConfig(char *pcCfgKey, OCM_PORT_DESC_ST *pstOcmPortDesc)
{
    signed int rc = SR_ERR_OK;
    char acXpath[512] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    OCM_PORT_ID_EN enPortId = EN_OCM_PORT_NONE;

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstOcmPortDesc) {
        LOG_WRITE(EN_LOG_ERROR, "pstOcmPortDesc is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    enPortId = DSC_GetOcmPortIdByNameNew(pstOcmPortDesc->acName);
    if (enPortId < EN_OCM_PORT_NONE || enPortId >= EN_OCM_PORT_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Not find ocm portname: %s", pstOcmPortDesc->acName);
        return SR_ERR_INVAL_ARG;
    }
    /* 2. set parameters */
    snprintf(acXpath, sizeof(acXpath), OCM_OPENCONFIG_XPATH_CONFIG_MONITOR_PORT, pstOcmPortDesc->acName);
    rc = set_string_item(pstSess, acXpath, pstOcmPortDesc->acMonitorPort, sizeof(pstOcmPortDesc->acMonitorPort));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-monitor-port: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}


int
DSC_GetOsaRpc(char *pcPortName, int16_t *psData, int16_t usBufLen, uint16_t *pusLen)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    size_t nRspLen = 0;
    unsigned char *pucDecode = NULL;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcPortName || NULL == psData) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }
    /* allocate input values filename*/
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error %d", rc);
        return rc;
    }
    /* set 'input/action leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_OCM_RAW_PORT);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_OCM_RAW_PORT, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcPortName);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_OCM_RAW, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_OCM_RAW, sr_strerror(rc));
        goto ERR_LABEL;
    }

#if 0
    if (strlen(output[1].data.binary_val) != output[0].data.uint32_val) {
        LOG_WRITE(EN_LOG_ERROR, "Len error: %d %d", strlen(output[1].data.binary_val), output[0].data.uint32_val);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
#endif

    pucDecode = b64_decode_ex(output[1].data.binary_val, strlen(output[1].data.binary_val), &nRspLen);
    if (NULL == pucDecode) {
        LOG_WRITE(EN_LOG_ERROR, "Error by b64_decode_ex");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (strcmp(output[output_cnt - 2].data.enum_val, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error return value %s", output[output_cnt - 2].data.enum_val);
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    *pusLen = nRspLen / sizeof(int16_t);
    if (*pusLen > usBufLen) {
        LOG_WRITE(EN_LOG_ERROR, "Buffer overflow %d %d", *pusLen, usBufLen);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memcpy(psData, pucDecode, nRspLen);

    DEF_SAFE_FREE(pucDecode);
    sr_free_values(input, input_cnt);

    return rc;

ERR_LABEL:
    DEF_SAFE_FREE(pucDecode);
    sr_free_values(input, input_cnt);
    return rc;
}

/*
   For OCM module, meiwu start-frequency is 191175GHz and end-frequency is 196175GHz.
   It is divided into 800 slices by 6.25GHz.

 */
int
DSC_GetOcmPowerByRawData(OCM_CHANNELS_LIST_ST *pstList, int16_t *psSliceRawData, uint32_t uiSliceCnt)
{
    int rc = SR_ERR_OK;
    uint32_t i = 0, uiStartSliceIdx, uiEndSliceIdx;
    uint64_t ulLowFrequency = 0, ulHighFrequency = 0;

    for (i = 0; i < pstList->nNum; i++) {
        ulLowFrequency = pstList->pstChannelArray[i].ulLowFrequency;
        ulHighFrequency = pstList->pstChannelArray[i].ulHighFrequency;
        if (ulLowFrequency < OCM_MODULE_START_FREQUENCY ||
            ulHighFrequency > OCM_MODULE_END_FREQUENCY ||
            ulHighFrequency <= ulLowFrequency) {
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "Invalid channel freq. low: %ld, high: %ld", ulLowFrequency, ulHighFrequency);
            goto END_LABEL;
        }
        /* cali start */
        uiStartSliceIdx = (pstList->pstChannelArray[i].ulLowFrequency - OCM_MODULE_START_FREQUENCY) / OCM_MODULE_SLICE_BANDWIDTH;
        uiEndSliceIdx = (pstList->pstChannelArray[i].ulHighFrequency - OCM_MODULE_START_FREQUENCY) / OCM_MODULE_SLICE_BANDWIDTH;

        /*
           if (uiEndSliceIdx >= OCM_MODULE_SLICE_NUM_MAX ||
            uiEndSliceIdx >= uiSliceCnt) {
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "Invalid slice. start: %d, end: %d, uiSliceCnt: %d", uiStartSliceIdx, uiEndSliceIdx, uiSliceCnt);
            goto END_LABEL;
           }
         */

        pstList->pstChannelArray[i].dPower = UTIL_GetChanPowerByRawdata(psSliceRawData + uiStartSliceIdx, uiEndSliceIdx - uiStartSliceIdx, -60);

    }

END_LABEL:
    return rc;
}

/**************************** add waveplan--- ************************************** */

int
DSC_WavplanModeToValue(char *pcBuf, WAVEPLAN_MODE_TYPE *penWaveplanMode)
{
    int rc = SR_ERR_OK;

    if (NULL == pcBuf || NULL == penWaveplanMode) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

#ifdef __OLSP__
    if ((0 == strcasecmp(pcBuf, WAVEPLAN_AUTO))) {
        *penWaveplanMode = WAVEPLAN_MODE_AUTO_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_75G))) {
        *penWaveplanMode = WAVEPLAN_MODE_75G_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_150G))) {
        *penWaveplanMode = WAVEPLAN_MODE_150G_TYPE;
    } else {
        *penWaveplanMode = WAVEPLAN_MODE_AUTO_TYPE;
    }
#endif

#ifdef _NR1004_
    if ((0 == strcasecmp(pcBuf, WAVEPLAN_150G))) {
        *penWaveplanMode = WAVEPLAN_MODE_150G_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_175G))) {
        *penWaveplanMode = WAVEPLAN_MODE_175G_TYPE;
    } else {
        *penWaveplanMode = WAVEPLAN_MODE_150G_TYPE;
    }
#endif

#ifdef __HCF__
    if ((0 == strcasecmp(pcBuf, WAVEPLAN_AUTO))) {
        *penWaveplanMode = WAVEPLAN_MODE_AUTO_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_75G))) {
        *penWaveplanMode = WAVEPLAN_MODE_75G_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_150G))) {
        *penWaveplanMode = WAVEPLAN_MODE_150G_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_300G))) {
        *penWaveplanMode = WAVEPLAN_MODE_300G_TYPE;
    } else if ((0 == strcasecmp(pcBuf, WAVEPLAN_100G))) {
        *penWaveplanMode = WAVEPLAN_MODE_100G_TYPE;
    } else {
        *penWaveplanMode = WAVEPLAN_MODE_AUTO_TYPE;
    }
#endif

    return rc;
}

char *
DSC_WavplanModeToStr(char *pcStr, WAVEPLAN_MODE_TYPE enMode)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enMode) {
    case WAVEPLAN_MODE_AUTO_TYPE:
        strcpy(pcStr, WAVEPLAN_AUTO);
        break;
    case WAVEPLAN_MODE_75G_TYPE:
        strcpy(pcStr, WAVEPLAN_75G);
        break;
    case WAVEPLAN_MODE_150G_TYPE:
        strcpy(pcStr, WAVEPLAN_150G);
        break;
    case WAVEPLAN_MODE_300G_TYPE:
        strcpy(pcStr, WAVEPLAN_300G);
        break;
    case WAVEPLAN_MODE_100G_TYPE:
        strcpy(pcStr, WAVEPLAN_100G);
        break;
    default:
        strcpy(pcStr, WAVEPLAN_AUTO);
        break;
    }

    return pcStr;
}

int
DSC_GetWaveplanModeConfig(WAVEPLAN_MODE_TYPE *enWaveplanMode)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acWaveplanMode[128] = {0};

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == enWaveplanMode) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    rc = get_enum_item(sess, OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG_WAVEPLAN_MODE, acWaveplanMode, sizeof(acWaveplanMode));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG_WAVEPLAN_MODE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_WavplanModeToValue(acWaveplanMode, enWaveplanMode);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_WavplanModeToValue error: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
Dsc_SetWaveplanMode(char *pcMode, bool bCommit)
{
    signed int rc = SR_ERR_OK;
    char acXpath[512] = {0};
    char acBufferTmp[256] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strcasecmp(pcMode, CLI_AWG_MODE_AUTO)) {
        memcpy(acBufferTmp, WAVEPLAN_AUTO, sizeof(WAVEPLAN_AUTO));
    } else if (0 == strcasecmp(pcMode, CLI_AWG_MODE_75G)) {
        memcpy(acBufferTmp, WAVEPLAN_75G, sizeof(WAVEPLAN_75G));
    } else if (0 == strcasecmp(pcMode, CLI_AWG_MODE_150G)) {
        memcpy(acBufferTmp, WAVEPLAN_150G, sizeof(WAVEPLAN_150G));
    } else if (0 == strcasecmp(pcMode, CLI_AWG_MODE_175G)) {
        memcpy(acBufferTmp, WAVEPLAN_175G, sizeof(WAVEPLAN_175G));
    } else if (0 == strcasecmp(pcMode, CLI_AWG_MODE_100G)) {
        memcpy(acBufferTmp, WAVEPLAN_100G, sizeof(WAVEPLAN_100G));
    } else if (0 == strcasecmp(pcMode, CLI_AWG_MODE_300G)) {
        memcpy(acBufferTmp, WAVEPLAN_300G, sizeof(WAVEPLAN_300G));
    } else {
        LOG_WRITE(EN_LOG_ERROR, "pcMode is inval, %s", pcMode);
        return SR_ERR_INVAL_ARG;
    }

    /* 2.  add or modify a list */
    snprintf(acXpath, sizeof(acXpath), "%s", OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG_WAVEPLAN_MODE);
    LOG_WRITE(EN_LOG_NOTICE, "acXpath=:%s  acBufferTmp=:%s", acXpath, acBufferTmp);
    rc = set_enum_item(pstSess, acXpath, acBufferTmp);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-name: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* 3. commit */
    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_commit: %s", sr_strerror(rc));
        }
    }

ERR_LABEL:

    return rc;

}

#include "DSC_circuitpack.h"

int
Dsc_SwitchWaveplan(int iCurCount, WAVEPLAN_MODE_TYPE enWaveplanMode, bool *pbNeedSetOcm)
{
    int rc = SR_ERR_OK;
    char acCmdLine[OPLK_PATH_MAX_LEN] = {0};
    char acBufTmp[512] = {0};
    LOG_WRITE(EN_LOG_NOTICE, "current enWaveplanMode:%d", enWaveplanMode);
    /*  reinstall wavepan xml cfg  */
    if ( WAVEPLAN_MODE_AUTO_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_AUTO_TYPE");
        /* get mux/demux mfg from hal*/
        ST_CIRCUIT_PACK stCpInfo;

        for (int i = 0; i < 3; i++) {
            LOG_WRITE(EN_LOG_NOTICE, "DSC_CP_GetInfoByName_I,i:%d", i);
            memset(&stCpInfo, 0, sizeof(stCpInfo));
            rc = DSC_CP_GetInfoByName(CIRCUIT_PACK_MUX, &stCpInfo);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "MUX_GetMfg failed!");
                sleep(1);
                continue;
            }

            if (0 == strlen(stCpInfo.acRefSlotName)) {
                LOG_WRITE(EN_LOG_ERROR, "MUX not plug!");
                rc = SR_ERR_OK; /* SR_ERR_NOT_FOUND; */
                sleep(1);
            } else {
                rc = SR_ERR_OK;
                break;
            }
        }

        LOG_WRITE(EN_LOG_NOTICE, "stCpInfo.acProductCode:%s,acModel:%s,sn:%s,name:%s,acProductCode:%s", stCpInfo.acProductCode, stCpInfo.stPhysical.stNode.acModel, stCpInfo.stPhysical.stNode.acSN, stCpInfo.acName, stCpInfo.stPhysical.stCommon.acProductCode);

        /*  get mux/demux model*/
        if (strlen(stCpInfo.stPhysical.stCommon.acProductCode) > 0) {
            if ( 0 == strcmp(stCpInfo.stPhysical.stCommon.acProductCode, AWG_300G_PN)) {
                LOG_WRITE(EN_LOG_NOTICE, "should be AWG_300G_PN,acProductCode: %s", stCpInfo.stPhysical.stCommon.acProductCode);
                /* switch waveplan if needed*/
                if (iCurCount != 17) {
                    memset(acCmdLine, 0, sizeof(acCmdLine));
                    memset(acBufTmp, 0, sizeof(acBufTmp));
                    snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_300G);
                    LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
                    /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
                    rc = system(acCmdLine);
                    if ( OPLK_OK != rc ) {
                        LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_150G");
                        rc = SR_ERR_OPERATION_FAILED;
                        goto cleanup;
                    }
                    *pbNeedSetOcm = true;
                }
            } else if ( 0 == strcmp(stCpInfo.stPhysical.stCommon.acProductCode, AWG_150G_PN)) {
                LOG_WRITE(EN_LOG_NOTICE, "should be AWG_150G_PN,acProductCode: %s", stCpInfo.stPhysical.stCommon.acProductCode);
                /* switch waveplan if needed*/
                if (iCurCount != 33) {
                    memset(acCmdLine, 0, sizeof(acCmdLine));
                    memset(acBufTmp, 0, sizeof(acBufTmp));
                    snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_150G);
                    LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
                    /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
                    rc = system(acCmdLine);
                    if ( OPLK_OK != rc ) {
                        LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_150G");
                        rc = SR_ERR_OPERATION_FAILED;
                        goto cleanup;
                    }
                    *pbNeedSetOcm = true;
                }
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "should be DEFAULT,acProductCode: %s", stCpInfo.stPhysical.stCommon.acProductCode);
                /* switch waveplan if needed*/
#ifdef __HCF__
                /* hcf default is 150G */
                if (iCurCount != WAVEPLAN_COUNT_150G) {
#else
                if (iCurCount != WAVEPLAN_COUNT_75G) {
#endif
                    memset(acCmdLine, 0, sizeof(acCmdLine));
                    memset(acBufTmp, 0, sizeof(acBufTmp));
                    snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_DEFAULT);
                    LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
                    /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
                    rc = system(acCmdLine);
                    if ( OPLK_OK != rc ) {
                        LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to DEFAULT");
                        rc = SR_ERR_OPERATION_FAILED;
                        goto cleanup;
                    }
                    *pbNeedSetOcm = true;
                }
            }
        } else {
            /* if wavplan-mode is auto,  mfg is offline or model err, use default waveplan */
            LOG_WRITE(EN_LOG_ERROR, "MUX model err! or not pulg in");
#ifdef __HCF__
            /* hcf default is 150G */
            if (iCurCount != WAVEPLAN_COUNT_150G) {
#else
            if (iCurCount != WAVEPLAN_COUNT_75G) {
#endif
                memset(acCmdLine, 0, sizeof(acCmdLine));
                memset(acBufTmp, 0, sizeof(acBufTmp));
                snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_DEFAULT);
                LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
                /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
                rc = system(acCmdLine);
                if ( OPLK_OK != rc ) {
                    LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to DEFAULT");
                    rc = SR_ERR_OPERATION_FAILED;
                    goto cleanup;
                }
                *pbNeedSetOcm = true;
            }
        }
    } else if ( WAVEPLAN_MODE_75G_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_75G_TYPE");
        if (iCurCount != WAVEPLAN_COUNT_75G) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            memset(acBufTmp, 0, sizeof(acBufTmp));
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_75G);
            /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
            rc = system(acCmdLine);
            if ( OPLK_OK != rc ) {
                LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_75G");
                rc = SR_ERR_OPERATION_FAILED;
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_NOTICE, "switch waveplan to AWG_75G, rc= %d", rc);
            *pbNeedSetOcm = true;
        }
    } else if ( WAVEPLAN_MODE_150G_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_150G_TYPE");
        if (iCurCount != WAVEPLAN_COUNT_150G) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            memset(acBufTmp, 0, sizeof(acBufTmp));
#ifndef _NR1004_
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_150G);
#else
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_DEFAULT);
#endif
            LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
            /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100); */
            rc = system(acCmdLine);
            if ( OPLK_OK != rc ) {
                LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_150G, rc= %d", rc);
                rc = SR_ERR_OPERATION_FAILED;
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_NOTICE, "switch waveplan to AWG_150G, rc= %d", rc);
            *pbNeedSetOcm = true;
        }
    } else if ( WAVEPLAN_MODE_175G_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_175G_TYPE");
        if (iCurCount != WAVEPLAN_COUNT_175G) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            memset(acBufTmp, 0, sizeof(acBufTmp));
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_175G);
            LOG_WRITE(EN_LOG_NOTICE, "acCmdLine:%s", acCmdLine);
            /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100); */
            rc = system(acCmdLine);
            if ( OPLK_OK != rc ) {
                LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_175G, rc= %d", rc);
                rc = SR_ERR_OPERATION_FAILED;
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_NOTICE, "switch waveplan to AWG_175G, rc= %d", rc);
            *pbNeedSetOcm = true;
        }
    } else if ( WAVEPLAN_MODE_100G_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_100G_TYPE");
        if (iCurCount != WAVEPLAN_COUNT_100G) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            memset(acBufTmp, 0, sizeof(acBufTmp));
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_100G);
            /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
            rc = system(acCmdLine);
            if ( OPLK_OK != rc ) {
                LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_100G");
                rc = SR_ERR_OPERATION_FAILED;
                goto cleanup;
            }
            *pbNeedSetOcm = true;
        }
    } else if ( WAVEPLAN_MODE_300G_TYPE == enWaveplanMode ) {
        LOG_WRITE(EN_LOG_NOTICE, "enWaveplanMode : WAVEPLAN_MODE_300G_TYPE");
        if (iCurCount != WAVEPLAN_COUNT_300G) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            memset(acBufTmp, 0, sizeof(acBufTmp));
            snprintf(acCmdLine, sizeof(acCmdLine), "%s", SWITCH_WAVEPLAN_CFG_300G);
            /*rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);*/
            rc = system(acCmdLine);
            if ( OPLK_OK != rc ) {
                LOG_WRITE(EN_LOG_ERROR, "error switch waveplan to AWG_300G");
                rc = SR_ERR_OPERATION_FAILED;
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_NOTICE, "switch waveplan to AWG_300G, rc= %d", rc);
            *pbNeedSetOcm = true;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "waveplan model err!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

cleanup:
    return rc;

}

/**************************** add waveplan--- ************************************** */


int
DSC_SetOcmModeCfg(OCM_MODE_CONFIG_ST *pstModeCfg)
{
    int rc = SR_ERR_OK;

    if (NULL == pstModeCfg) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = Dsc_SetWaveplanMode(pstModeCfg->acWaveplanMode, true);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
/*
    rc = Dsc_SetChannelPowerMode(pstModeCfg->acChPwrMode, true);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
 */
END_LABEL:

    return rc;
}

int
DSC_GetOcmModeState(OCM_MODE_STATE_ST *pstModeState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    OCM_MODE_STATE_ST stModeState;

    sess = DSC_GetSession();

    if (NULL == sess || NULL == pstModeState) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(&stModeState, 0, sizeof(stModeState));

    rc = get_enum_item(sess, OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE_CURRENT_WAVEPLAN, stModeState.acCurWaveplanMode, sizeof(stModeState.acCurWaveplanMode));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE_CURRENT_WAVEPLAN, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memcpy(pstModeState, &stModeState, sizeof(stModeState));

ERR_LABEL:
    return rc;
}

int
DSC_SetBaChInLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_CHANNELIN_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_CHANNELIN_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s input low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetBaChInHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_CHANNELIN_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_CHANNELIN_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s input high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetBaChOutLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_CHANNELOUT_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_CHANNELOUT_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s output low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetBaChOutHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_CHANNELOUT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_BA_CFG_KEY)) {
        iTcaId = TCA_IDX_BA_P_CHANNELOUT_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s output high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetPaChInLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_CHANNELIN_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_CHANNELIN_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s input low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetPaChInHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_CHANNELIN_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_CHANNELIN_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s input high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetPaChOutLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_CHANNELOUT_LOW;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_CHANNELOUT_LOW;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s output low thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetPaChOutHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iTcaId = 0;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strcmp(pcCfgKey, DEF_OCM_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_CHANNELOUT_HIGH;
    } else if (0 == strcmp(pcCfgKey, DEF_OCM_P_PA_CFG_KEY)) {
        iTcaId = TCA_IDX_PA_P_CHANNELOUT_HIGH;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error not find key: %s", pcCfgKey);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iTcaId, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set %s output high thresh: %s", pcCfgKey, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

