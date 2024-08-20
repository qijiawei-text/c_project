/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_waveplan.h"
#include "DSC_wavelength_router.h"

/* static int m_iChannelCount = 0; */

#if 0
static int
get_waveplan_info(char *pcPath, WAVEPLAN_ST **ppstWaveplan, size_t *count)
{
    int i = 0, j = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sr_val_t *val = NULL;
    size_t cnt = 0;
    WAVEPLAN_ST *waveplan = NULL;

    sess = DSC_GetOperSession();

    if (NULL == pcPath || NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL != strstr(pcPath, "default-waveplan")) { /* default */
        rc = DSC_GetDefaultWaveplansCnt(count);
        LOG_WRITE(EN_LOG_DEBUG, "default-waveplan, count: %zu", *count);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetDefaultWaveplansCnt error: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else { /* flex */
        LOG_WRITE(EN_LOG_DEBUG, "flex-waveplan, count: %zu", *count);
        rc = DSC_GetWaveplansCnt(count);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetWaveplansCnt error: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (*count >= MAX_CHL) {
        LOG_WRITE(EN_LOG_ERROR, "count: %d out of range", (int)(*count));
        goto ERR_LABEL;
    }

    waveplan = malloc(sizeof(WAVEPLAN_ST) * (*count));
    if (waveplan == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error %d", SR_ERR_INTERNAL);
        goto ERR_LABEL;
    }

    memset(waveplan, 0, sizeof(WAVEPLAN_ST) * (*count));
    rc = get_items(sess, pcPath, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), pcPath);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_items, %s", pcPath);
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, CHANNEL_ID_XPATH) != NULL) {
            j++;
            if (j >= *count) {
                break;
            }
            waveplan[j].uiChannelId = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, CHANNEL_NAME_XPATH) != NULL) {
            strncpy(waveplan[j].acChannelName, val[i].data.string_val, sizeof(waveplan[j].acChannelName) - 1);
        } else if (strstr(val[i].xpath, FREQUENCY_XPATH) != NULL) {
            waveplan[j].dCenterFreq = FREQUENCY_GHZ(val[i].data.decimal64_val);
        }
        /* else if (strstr(value->xpath, WAVELENGTH_XPATH) != NULL) { */
        else if (0 == strcmp(sr_xpath_node_name(val[i].xpath), WAVELENGTH_NODE)) {
            waveplan[j].dWavelength = val[i].data.decimal64_val;
        } else if (strstr(val[i].xpath, BANDWIDTH_XPATH) != NULL) {
            waveplan[j].dBandwidth = BANDWIDTH_GHZ(val[i].data.uint32_val);
        }

    }
    sr_free_values(val, cnt);

    *ppstWaveplan = waveplan;
    return rc;

ERR_LABEL:
    if (val != NULL) {
        sr_free_values(val, cnt);
    }
    DEF_SAFE_FREE(waveplan);
    *count = 0;
    return rc;
}

int
DSC_GetWaveplansCnt(size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *values = NULL;
    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* the number of  wavelength never changes, so cache it */
    if (m_iChannelCount > 0 ) {
        *count = m_iChannelCount;
        return rc;
    }

    rc = get_items(sess, WAVEPLAN_XPATH_CHANNEL_ID_ALL, &values, count);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s", WAVEPLAN_XPATH_CHANNEL_ID_ALL);
    }

    m_iChannelCount = *count;
    sr_free_values(values, *count);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_WARNING, "Not found item: %s", WAVEPLAN_XPATH_CHANNEL_ID_ALL);
        *count = 0;
        return SR_ERR_OK;
    }

    return rc;
}

int
DSC_GetDefaultWaveplansCnt(size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sess = DSC_GetOperSession();
    sr_val_t *values = NULL;
    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* the number of  wavelength never changes, so cache it */
    if (m_iChannelCount > 0 ) {
        *count = m_iChannelCount;
        return rc;
    }

    rc = get_items(sess, WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID, &values, count);
    if (SR_ERR_OK == rc) {
        sr_free_values(values, *count);
    } else if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_WARNING, "Not found item: %s", WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID);
        *count = 0;
        rc = SR_ERR_OK;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s", WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID);
        *count = 0;
    }

    m_iChannelCount = *count;

    return rc;
}

int
DSC_GetWaveplansInfo(WAVEPLAN_ST **ppstWaveplan, size_t *count)
{
    return get_waveplan_info(WAVEPLAN_XPATH_ALL, ppstWaveplan, count);
}


int
DSC_GetDefaultWaveplansInfo(WAVEPLAN_ST **ppstWaveplan, size_t *count)
{
    return get_waveplan_info(WAVEPLAN_XPATH_DEFAULT_ALL, ppstWaveplan, count);
}

int
DSC_SetWaveplansInfo(WAVEPLAN_ST stWaveplan[], size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char selectXpath[OPLK_PATH_MAX_LEN];
    int iChlCnt = 0;

    if ((sess = DSC_GetRunningSession()) == NULL || size < sizeof(WAVEPLAN_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        goto ERR_LABEL;
    }

    iChlCnt = size / sizeof(WAVEPLAN_ST);

    for (int i = 0; i < iChlCnt; i++) {
        snprintf(selectXpath, OPLK_PATH_MAX_LEN, WAVEPLAN_XPATH_FREQUENCY, stWaveplan[i].u32ChannelId);
        rc = set_decimal64_item(sess, selectXpath, stWaveplan[i].dFrequency);
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "set waveplan frequency error %s", sr_strerror(rc));
            goto ERR_LABEL;
        }

        snprintf(selectXpath, OPLK_PATH_MAX_LEN, WAVEPLAN_XPATH_BANDWIDTH, stWaveplan[i].u32ChannelId);
        rc = set_uint32_item(sess, selectXpath, stWaveplan[i].u32Bandwidth);
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "set waveplan bandwith error %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set waveplan commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

#endif
/* just for csm to get waveplan */

int
DSC_GetInnerWaveplansCnt(size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *values = NULL;
    sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID_I, &values, count);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s", WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID_I);
    }

    sr_free_values(values, *count);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_WARNING, "Not found item: %s", WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID_I);
        *count = 0;
        return SR_ERR_OK;
    }

    return rc;
}

static int
DSC_GetInnerDefaultWaveplansInfo(WAVEPLAN_ST **ppstWaveplan, size_t *count)
{
    int i = 0, j = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *val = NULL;
    size_t cnt = 0;
    WAVEPLAN_ST *waveplan = NULL;

    sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null, %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetInnerWaveplansCnt(count);
    LOG_WRITE(EN_LOG_DEBUG, "default-waveplan, count: %zu", *count);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetInnerWaveplansCnt error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (0 == *count) {
        rc = SR_ERR_OK;
        LOG_WRITE(EN_LOG_WARNING, "No waveplan info: %s", sr_strerror(rc));
        goto ERR_LABEL;
    } else if (*count >= MAX_CHL) {
        LOG_WRITE(EN_LOG_ERROR, "count: %d out of range", (int)(*count));
        goto ERR_LABEL;
    } else {
        /* do nothing */
    }

    waveplan = malloc(sizeof(WAVEPLAN_ST) * (*count));
    if (waveplan == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error %d", SR_ERR_INTERNAL);
        goto ERR_LABEL;
    }

    memset(waveplan, 0, sizeof(WAVEPLAN_ST) * (*count));
    rc = get_items(sess, WAVEPLAN_XPATH_DEFAULT_ALL_I, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), WAVEPLAN_XPATH_DEFAULT_ALL_I);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_items, %s", WAVEPLAN_XPATH_DEFAULT_ALL_I);
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, CHANNEL_ID_XPATH) != NULL) {
            j++;
            if (j >= *count) {
                break;
            }
            waveplan[j].uiChannelId = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, CHANNEL_NAME_XPATH) != NULL) {
            strncpy(waveplan[j].acChannelName, val[i].data.string_val, sizeof(waveplan[j].acChannelName) - 1);
        } else if (strstr(val[i].xpath, FREQUENCY_XPATH) != NULL) {
            waveplan[j].dCenterFreq = FREQUENCY_GHZ(val[i].data.decimal64_val);
        }
        /* else if (strstr(value->xpath, WAVELENGTH_XPATH) != NULL) { */
        else if (0 == strcmp(sr_xpath_node_name(val[i].xpath), WAVELENGTH_NODE)) {
            waveplan[j].dWavelength = val[i].data.decimal64_val;
        } else if (strstr(val[i].xpath, BANDWIDTH_XPATH) != NULL) {
            waveplan[j].dBandwidth = BANDWIDTH_GHZ(val[i].data.uint32_val);
        }

    }
    sr_free_values(val, cnt);

    *ppstWaveplan = waveplan;
    return rc;

ERR_LABEL:
    if (val != NULL)
        sr_free_values(val, cnt);
    if (waveplan != NULL) {
        free(waveplan);
        waveplan = NULL;
    }
    *count = 0;
    return rc;
}

static int
__GetWrOcmWaveplanInfo(WAVEPLAN_ST **ppstWaveplan, size_t *pnCount)
{
    int rc = SR_ERR_OK;
    WR_WAVEPLAN_LIST_ST stOcmWaveplanList;
    WAVEPLAN_ST *pastWaveplan = NULL;
    uint32_t uiLen = 0, i = 0;

    if (NULL == ppstWaveplan || NULL == pnCount) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_WrGetOcmWaveplan(&stOcmWaveplanList);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    if (0 == stOcmWaveplanList.uiTotal) {
        *ppstWaveplan = NULL;
        *pnCount = 0;
        return SR_ERR_OK;
    }

    uiLen = stOcmWaveplanList.uiTotal * sizeof(WAVEPLAN_ST);

    pastWaveplan = (WAVEPLAN_ST *)malloc(uiLen);
    if (NULL == pastWaveplan) {
        rc = ERR_NOMEM;
        goto ERR_LABEL;
    }

    memset(pastWaveplan, 0, uiLen);
    for (i = 0; i < stOcmWaveplanList.uiTotal; i++) {
        pastWaveplan[i].uiChannelId = i + 1;
        pastWaveplan[i].dBandwidth = (double)(stOcmWaveplanList.astWaveplan[i].ulUpperFreq - stOcmWaveplanList.astWaveplan[i].ulLowerFreq) / 1000;
        pastWaveplan[i].dCenterFreq = ((double)stOcmWaveplanList.astWaveplan[i].ulLowerFreq) / 1000 + pastWaveplan[i].dBandwidth / 2;
        pastWaveplan[i].dWavelength = 0;         /* TBD */
        snprintf(pastWaveplan[i].acChannelName, sizeof(pastWaveplan[i].acChannelName), "Channel_%d", i + 1);          /* TBD */
    }

    *ppstWaveplan = pastWaveplan;
    *pnCount = stOcmWaveplanList.uiTotal;

    return SR_ERR_OK;

ERR_LABEL:

    DEF_SAFE_FREE(pastWaveplan);
    return rc;
}

WAVEPLAN_TYPE_EN
DSC_GetWaveplanType()
{
    static WAVEPLAN_TYPE_EN m_enWaveplanType = EN_WAVEPLAN_TYPE_MAX;
    sr_session_ctx_t *pstSess = NULL;

    if (m_enWaveplanType != EN_WAVEPLAN_TYPE_MAX) {
        return m_enWaveplanType;
    }

    pstSess = DSC_GetRunningSession();

    if (true == DSC_IsSchemaInstalled(pstSess, MODULE_NS_WAVEPLAN)) {
        m_enWaveplanType = EN_WAVEPLAN_TYPE_FIX;
    }

    if (true == DSC_IsSchemaInstalled(pstSess, MODULE_WAVELENGTH_ROUTER)) {
        m_enWaveplanType = EN_WAVEPLAN_TYPE_FLEX;
    }

    LOG_WRITE(EN_LOG_DEBUG, "waveplan type: %d", m_enWaveplanType);

    return m_enWaveplanType;
}

int
DSC_GetOcmWaveplanInfo(WAVEPLAN_ST **ppstWaveplan, size_t *pnCount)
{
    WAVEPLAN_TYPE_EN enWaveplanType = EN_WAVEPLAN_TYPE_MAX;
    int rc = SR_ERR_OK;

    enWaveplanType = DSC_GetWaveplanType();

    switch (enWaveplanType) {
    case EN_WAVEPLAN_TYPE_FIX:
        rc = DSC_GetInnerDefaultWaveplansInfo(ppstWaveplan, pnCount);
        break;

    case EN_WAVEPLAN_TYPE_FLEX:
        rc = __GetWrOcmWaveplanInfo(ppstWaveplan, pnCount);
        break;

    default:
        rc = SR_ERR_NOT_FOUND;
        break;
    }

    return rc;

}

int
DSC_GetOcmWaveplanNumber(size_t *pnCount)
{
    WAVEPLAN_TYPE_EN enWaveplanType = EN_WAVEPLAN_TYPE_MAX;
    int rc = SR_ERR_OK;
    WR_WAVEPLAN_LIST_ST stOcmWaveplanList;
    size_t nCnt = 0;

    enWaveplanType = DSC_GetWaveplanType();

    switch (enWaveplanType) {
    case EN_WAVEPLAN_TYPE_FIX:
        rc = DSC_GetInnerWaveplansCnt(&nCnt);
        break;

    case EN_WAVEPLAN_TYPE_FLEX:
        memset(&stOcmWaveplanList, 0, sizeof(stOcmWaveplanList));
        rc = DSC_WrGetOcmWaveplan(&stOcmWaveplanList);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
        nCnt = stOcmWaveplanList.uiTotal;
        break;

    default:
        rc = SR_ERR_NOT_FOUND;
    }

END_LABEL:

    if (SR_ERR_OK == rc) {
        *pnCount = nCnt;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting waveplan number. waveplan type: %d, rc: %d", enWaveplanType, rc);
    }

    return rc;
}

