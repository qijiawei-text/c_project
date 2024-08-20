#include "DSC_common.h"
#include "rest_common.h"
#include "rest_sr_util.h"
#include "rest_handle.h"

static REST_JSON_CONFIG_ST m_stJsonConfig = {NULL, NULL, NULL, NULL};
/* special handler for conflict listname in list_key.json */
static LIST_KEY_MAPPING_TABLE_ST m_astListKeyMapTab[] = {{"channel", "lower-frequency upper-frequency", "/channel-monitors/"},   /*openconfig-channel-monitor.yang*/
                                                         {"channel", "index", "/media-channels/"},                               /*openconfig-wavelength-router.yang*/
                                                         {"destination", "name", "/syslog/"},                                    /*org-openroadm-syslog.yang*/
                                                         {"destination", "destination-address destination-port", "/telemetry-system/"}, /*openconfig-telemetry.yang*/
                                                         {"otdr-port", "port-name", "/optical-otdr/"}, /*openconfig-telemetry.yang*/
};

/* only internal use */
#define LOAD_JSON_CONFIG_FILE(pcJsonBuf, json_file, json_object, uiLen) \
    do { \
        pcJsonBuf = REST_GetFileContentWithEndChar(json_file, &uiLen); \
        if (NULL == pcJsonBuf) { \
            LOG_WRITE(EN_LOG_ERROR, "Read file failed: %s", json_file); \
            goto END_LABEL; \
        } \
        json_object = cJSON_Parse(pcJsonBuf); \
        if (NULL == json_object) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid json: %s", json_file); \
            goto END_LABEL; \
        } \
        if (NULL == json_object->child) { \
            LOG_WRITE(EN_LOG_ERROR, "empty json: %s", json_file); \
            goto END_LABEL; \
        } \
        DEF_SAFE_FREE(pcJsonBuf); \
    } while (0)

#define FREE_JSON_OBJECT(json_object) \
    do { \
        if (json_object != NULL) { \
            cJSON_Delete(json_object); \
            json_object = NULL; \
        } \
    } while (0)

int
REST_LoadJsonConfig()
{

    char *pcJsonBuf = NULL;
    uint32_t uiLen = 0;

    LOAD_JSON_CONFIG_FILE(pcJsonBuf, JSON_FILE_MODULE_PATH, m_stJsonConfig.pstObjModulePath, uiLen);
    /* LOAD_JSON_CONFIG_FILE(pcJsonBuf, JSON_FILE_NUMBER_TYPE, m_stJsonConfig.pstObjNumberType, uiLen); */
    /* LOAD_JSON_CONFIG_FILE(pcJsonBuf, JSON_FILE_STRING_TYPE, m_stJsonConfig.pstObjStringType, uiLen); */
    LOAD_JSON_CONFIG_FILE(pcJsonBuf, JSON_FILE_LIST_KEY, m_stJsonConfig.pstObjListKey, uiLen);

    DEF_SAFE_FREE(pcJsonBuf);
    return OPLK_OK;

END_LABEL:
    DEF_SAFE_FREE(pcJsonBuf);
    REST_FreeJsonConfig();
    return OPLK_ERR;
}

#undef LOAD_JSON_CONFIG_FILE

cJSON *
REST_GetConfigObj(JSON_CONFIG_OBJECT_TYPE_EN enObjType)
{
    cJSON *pstJsonObj = NULL;

    switch (enObjType) {
    case EN_MODULE_PATH_OBJECT:
        pstJsonObj = m_stJsonConfig.pstObjModulePath;
        break;
    case EN_NUMBER_TYPE_OBJECT:
        /* pstJsonObj = m_stJsonConfig.pstObjNumberType; */
        break;
    case EN_STRING_TYPE_OBJECT:
        /* pstJsonObj = m_stJsonConfig.pstObjStringType; */
        break;
    case EN_LIST_KEY_OBJECT:
        pstJsonObj = m_stJsonConfig.pstObjListKey;
        break;
    default:
        pstJsonObj = NULL;
        LOG_WRITE(EN_LOG_ERROR, "error enObjType: %d", enObjType);
        break;
    }

    return pstJsonObj;
}

void
REST_FreeJsonConfig()
{
    FREE_JSON_OBJECT(m_stJsonConfig.pstObjModulePath);
    /* FREE_JSON_OBJECT(m_stJsonConfig.pstObjNumberType); */
    /* FREE_JSON_OBJECT(m_stJsonConfig.pstObjStringType); */
    FREE_JSON_OBJECT(m_stJsonConfig.pstObjModulePath);
}

#undef FREE_JSON_OBJECT

/* special handler for conflict list name in list_key.json */
static int
__GetKeyNameFromMappingTable(const char *pcUrl, const char *pcNewPrefix, const char *pcListName, char *pcKeyName, uint32_t uiLen)
{
    size_t i = 0, nCount = 0;

    if (NULL == pcUrl || NULL == pcNewPrefix || NULL == pcListName || NULL == pcKeyName) {
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        return SR_ERR_INVAL_ARG;
    }

    nCount = sizeof(m_astListKeyMapTab) / sizeof(m_astListKeyMapTab[0]);
    for (i = 0; i < nCount; i++) {

        if (0 != strcmp(pcListName, m_astListKeyMapTab[i].pcListName)) {
            continue;
        }

        if ((NULL != strstr(pcUrl, m_astListKeyMapTab[i].pcUniqueKeywords)) ||
            (NULL != strstr(pcNewPrefix, m_astListKeyMapTab[i].pcUniqueKeywords))) {

            snprintf(pcKeyName, uiLen, "%s", m_astListKeyMapTab[i].pcKeyName);
            return SR_ERR_OK;
        }
    }

    return SR_ERR_NOT_FOUND;
}

static int
__GetKeyNameFromConfig(const char *pcListName, char *pcKeyName, uint32_t uiLen)
{
    cJSON *pstCurItem = NULL;

    if (NULL == pcListName || NULL == pcKeyName) {
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        return SR_ERR_INVAL_ARG;
    }

    /* get first node from module_path.json */
    pstCurItem = m_stJsonConfig.pstObjListKey->child;

    while (pstCurItem != NULL) {
        /*****************************************
                list_name : pstCurItem->string
                 key_name : pstCurItem->valuestring
        *****************************************/
        if (0 == strcmp(pcListName, pstCurItem->string)) {
            snprintf(pcKeyName, uiLen, "%s", pstCurItem->valuestring);
            /* printf("list_name: %s key_name: %s\n", pstCurItem->string, pstCurItem->valuestring); */
            return SR_ERR_OK;
        }

        pstCurItem = pstCurItem->next;
    }

    LOG_WRITE(EN_LOG_ERROR, "not find key name of list name: %s", pcListName);

    return SR_ERR_NOT_FOUND;
}

/* <string1><cSeparator><string2>cSeparator> */
static uint32_t
__GetDimension(const char *pcStr, int iSeparator)
{

    uint32_t uiDim = 1;
    char *pcTmp = NULL;

    pcTmp = strchr(pcStr, iSeparator);
    while (pcTmp != NULL) {
        uiDim++;
        pcTmp = strchr(pcTmp + 1, iSeparator);
    }

    return uiDim;
}

/**************************************************************
   pcKeyNameSet format:  "keyname1 keyname2 keyname3"
   pcKeyValueSet format: "keyvalue1 keyvalue2 keyvalue3"
 ***************************************************************/
static int
__ConstructXpathKey(const char *pcKeyNameSet, const char *pcKeyValueSet, const char *pcListName, char *pcXpathKey, uint32_t uiLen)
{
    uint32_t uiKeyNameDim = 0;
    uint32_t uiKeyValueDim = 0;
    char *pcNameTmp = NULL, *pcValueTmp = NULL;
    char *pcNextNameTmp = NULL, *pcNextValueTmp = NULL;
    char acKeyName[32] = {0};
    char acKeyValue[32] = {0};
    char acXpathKey[OPLK_PATH_MAX_LEN] = {0};
    uint32_t uiLenTmp = 0, uiOffsetLen = 0;
    int rc = SR_ERR_OK;

    uiKeyNameDim = __GetDimension(pcKeyNameSet, ' ');
    uiKeyValueDim = __GetDimension(pcKeyValueSet, ',');

    if (uiKeyNameDim != uiKeyValueDim) {
        LOG_WRITE(EN_LOG_ERROR, "unmatched list key. pcListName: %s, pcKeyNameSet: %s, pcKeyValueSet: %s", pcListName, pcKeyNameSet, pcKeyValueSet);
        return SR_ERR_NOT_FOUND;
    }

    if (1 == uiKeyNameDim) {
        /*single key */
        snprintf(acXpathKey, sizeof(acXpathKey), "%s[%s='%s']", pcListName, pcKeyNameSet, pcKeyValueSet);
    } else {
        /* multiple keys */
        pcNameTmp = (char *)pcKeyNameSet;
        pcValueTmp = (char *)pcKeyValueSet;
        snprintf(acXpathKey, sizeof(acXpathKey), "%s", pcListName);          /* fill listname */
        while (NULL != pcNameTmp && NULL != pcValueTmp) {
            pcNextNameTmp = strchr(pcNameTmp, ' ');
            pcNextValueTmp = strchr(pcValueTmp, ',');
            /* get key name */
            memset(acKeyName, 0, sizeof(acKeyName));
            uiLenTmp = (NULL == pcNextNameTmp) ? (strlen(pcNameTmp)) : (pcNextNameTmp - pcNameTmp);
            if (uiLenTmp >= sizeof(acKeyName)) {
                LOG_WRITE(EN_LOG_ERROR, "no enough memory for key value. uiLenTmp:%d, listname: %s", uiLenTmp, pcListName);
                rc = ERR_NOMEM;
                goto END_LABEL;
            }
            memcpy(acKeyName, pcNameTmp, uiLenTmp);

            /* get key value */
            memset(acKeyValue, 0, sizeof(acKeyValue));
            uiLenTmp = (NULL == pcNextValueTmp) ? (strlen(pcValueTmp)) : (pcNextValueTmp - pcValueTmp);
            if (uiLenTmp >= sizeof(acKeyValue)) {
                rc = ERR_NOMEM;
                LOG_WRITE(EN_LOG_ERROR, "no enough memory for key value. uiLenTmp:%d, listname: %s", uiLenTmp, pcListName);
                goto END_LABEL;
            }
            memcpy(acKeyValue, pcValueTmp, uiLenTmp);

            /* add new key to tail */
            uiOffsetLen = strlen(acXpathKey);
            if (uiOffsetLen >= OPLK_PATH_MAX_LEN - 1) {
                rc = ERR_NOMEM;
                LOG_WRITE(EN_LOG_ERROR, "no enough memory for xpath key. uiOffsetLen:%d, listname: %s", uiOffsetLen, pcListName);
                goto END_LABEL;
            }
            snprintf(acXpathKey + uiOffsetLen, OPLK_PATH_MAX_LEN - uiOffsetLen, "[%s='%s']", acKeyName, acKeyValue);

            /* next key */
            pcNameTmp = (NULL == pcNextNameTmp) ? NULL : (pcNextNameTmp + 1);
            pcValueTmp = (NULL == pcNextValueTmp) ? NULL : (pcNextValueTmp + 1);
        }
    }

    snprintf(pcXpathKey, uiLen, "%s", acXpathKey);
    LOG_WRITE(EN_LOG_DEBUG, "xpath key: %s", acXpathKey);

END_LABEL:
    return rc;
}


/*
   eg1:
        URL:   /data/optical-amplifier/amplifiers/amplifier=PA/config
        xpath: /data/optical-amplifier/amplifiers/amplifier['name'=PA]/config

   eg2:
        URL:   /data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=196062500,196137500/
        xpath: /data/channel-monitors/channel-monitor['name'=booster-amp-input]/channels/channel['lower-frequency'=196062500]['upper-frequency'=196137500]]/

 */
static int
__ConvertListKey(const char *pcUrl, const char *pcNewPrefix, char *pcXpath, uint32_t uiLen)
{
    int rc = SR_ERR_OK;
    const char *pcLnStart = NULL, *pcLnEnd = NULL;
    const char *pcKvStart = NULL, *pcKvEnd = NULL;
    char acListName[32] = {0};
    char acKeyNameSet[64] = {0};       /* single or multi keynames */
    char acKeyValueSet[64] = {0};      /* single or multi keyvalues */
    char acXpathKey[OPLK_PATH_MAX_LEN] = {0};
    uint32_t uiLenTmp = 0;
    char acUrlPrefix[OPLK_PATH_MAX_LEN] = {0};
    char acUrlSuffix[OPLK_PATH_MAX_LEN] = {0};
    char acNewUrlPrefix[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcUrl || NULL == pcNewPrefix || NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /**********************************************************************
            Full URL:	   | pcNewPrefix | url prefix | list | url suffix |
    **********************************************************************/

    /*list_name end before = */
    if (NULL == (pcLnEnd = strchr(pcUrl, '='))) {
        /* URL already converted ok */
        snprintf(pcXpath, uiLen, "%s%s", pcNewPrefix, pcUrl);
        return SR_ERR_OK;
    }

    /*list name start after previous / */
    pcLnStart = pcLnEnd - 1;
    while (*pcLnStart != '/') {
        pcLnStart--;
    }
    pcLnStart++;

    /*key value start after = */
    pcKvStart = pcLnEnd + 1;
    /*key value end before next / */
    pcKvEnd = strchr(pcKvStart, '/');
    if (pcKvEnd == NULL) {
        pcKvEnd = pcUrl + strlen(pcUrl);
    }

    /* get url prefix */
    strncpy(acUrlPrefix, pcUrl, pcLnStart - pcUrl);
    /* get usr suffix */
    snprintf(acUrlSuffix, sizeof(acUrlSuffix), "%s", pcKvEnd);

    /* get listname (eg: amplifier) */
    uiLenTmp = pcLnEnd - pcLnStart + 1;     /* actual length + '\0' */
    if (uiLenTmp > sizeof(acListName)) {
        LOG_WRITE(EN_LOG_ERROR, "no enough buffer for list name. uiLenTmp: %d", uiLenTmp);
        goto END_LABEL;
    }
    snprintf(acListName, uiLenTmp, "%s", pcLnStart);

    /* get list keyvalueset (single key or multiple keys) (eg: PA) */
    uiLenTmp = pcKvEnd - pcKvStart + 1;     /* actual length + '\0' */
    if (uiLenTmp > sizeof(acKeyValueSet)) {
        LOG_WRITE(EN_LOG_ERROR, "no enough buffer for key value set. uiLenTmp: %d", uiLenTmp);
        goto END_LABEL;
    }
    snprintf(acKeyValueSet, uiLenTmp, "%s", pcKvStart);

    /*there are multiple same listname  in list_key.json, hard code for now */
    rc = __GetKeyNameFromMappingTable(pcUrl, pcNewPrefix, acListName, acKeyNameSet, sizeof(acKeyNameSet));
    if (rc != SR_ERR_OK) {
        rc = __GetKeyNameFromConfig(acListName, acKeyNameSet, sizeof(acKeyNameSet));
        if (rc != SR_ERR_OK ) {
            LOG_WRITE(EN_LOG_ERROR, "get key name failed: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    }

    /*
            listname=keyvalue             =>   listname[keyname='keyvalue']
            listname=keyvalue1,keyvalue2  =>   listname=[keyname1='keyvalue1'][keyname2='keyvalue2']
     */
    rc = __ConstructXpathKey((const char *)acKeyNameSet, (const char *)acKeyValueSet, (const char *)acListName, acXpathKey, sizeof(acXpathKey));
    if (rc != SR_ERR_OK ) {
        LOG_WRITE(EN_LOG_ERROR, "construct xpath key filed: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* printf("%s[%d], acXpathKey: %s\r\n", __func__, __LINE__, acXpathKey); */

    /*replace url key with xpath key */
    snprintf(acNewUrlPrefix, OPLK_PATH_MAX_LEN, "%s%s%s", pcNewPrefix, acUrlPrefix, acXpathKey);

    /* move to next list key */
    rc = __ConvertListKey((const char *)acUrlSuffix, acNewUrlPrefix, pcXpath, uiLen);

END_LABEL:

    return rc;
}


int
REST_GetYangModule(const char *pcUrl, char *pcYangModule, uint32_t uiLen)
{
    const cJSON *pstCurItem = NULL;
    const cJSON *pstUrlNode = NULL;

    if (NULL == pcUrl || NULL == pcYangModule) {
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == m_stJsonConfig.pstObjModulePath) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == m_stJsonConfig.pstObjModulePath");
        return SR_ERR_INVAL_ARG;
    }
    /* get first node from module_path.json */
    pstCurItem = m_stJsonConfig.pstObjModulePath->child;

    while (pstCurItem != NULL) {
        /* printf("module: %s\n", pstCurItem->string); */

        /*go through array find the url path */
        pstUrlNode = pstCurItem->child;
        while (pstUrlNode != NULL) {
            /* input url path contains url in module_path.json */
            if (0 == strncasecmp(pcUrl, pstUrlNode->valuestring, strlen(pstUrlNode->valuestring))) {
                /* find yang module */
                snprintf(pcYangModule, uiLen, "%s", pstCurItem->string);
                /* printf("url path: %s, yang module: %s\n", cJSON_Print(path), module); * */
                return SR_ERR_OK;
            }
            pstUrlNode = pstUrlNode->next;
        }

        pstCurItem = pstCurItem->next;
    }

    LOG_WRITE(EN_LOG_ERROR, "not find yang module of url: %s", pcUrl);

    return SR_ERR_NOT_FOUND;
}

int
REST_UrlToYangXpath(const char *pcUrl, char *pcYangXpath, uint32_t uiLen)
{
    int rc = SR_ERR_OK;
    char acYangModule[OPLK_PATH_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acXpathTmp[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcUrl || NULL == pcYangXpath) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        goto END_LABEL;
    }
    /* convert yang list key from url path */
    if (NULL != strchr(pcUrl, '=')) {
        rc = __ConvertListKey((const char *)pcUrl, "", acXpath, sizeof(acXpath));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "convert list key failed: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    } else {
        memcpy(acXpath, pcUrl, sizeof(acXpath) - 1);
    }

    /* remove prefix from URL path */
    if (0 == strncmp(acXpath, URL_DATA_PREFIX, strlen(URL_DATA_PREFIX))) {
        snprintf(acXpathTmp, OPLK_PATH_MAX_LEN, "%s", acXpath + strlen(URL_DATA_PREFIX));
    } else if (0 == strncmp(acXpath, URL_OPT_PREFIX, strlen(URL_OPT_PREFIX))) {
        snprintf(acXpathTmp, OPLK_PATH_MAX_LEN, "%s", pcUrl + strlen(URL_OPT_PREFIX));
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "invalid url: %s", pcUrl);
        goto END_LABEL;
    }

    /*find yang schema from url path */
    rc = REST_GetYangModule(pcUrl, acYangModule, sizeof(acYangModule));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get yang model failed: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* yang xpath */
    snprintf(pcYangXpath, uiLen, "/%s:%s", acYangModule, acXpathTmp);

END_LABEL:
    return rc;
}
