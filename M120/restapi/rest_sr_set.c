#include "DSC_common.h"
#include "rest_sr_set.h"
#include "rest_common.h"

#define DEF_KEY_VALUE_TYPE_NUMBER    (1)
#define DEF_KEY_VALUE_TYPE_STRING    (2)

typedef struct {
    const char *pcListName;
    char *pcKeyName;
    uint8_t ucValueType;
    char acResv[7];
    const char *pcUniqueKeywords;
} LIST_KEY_TYPE_MAP_TBL;

static LIST_KEY_TYPE_MAP_TBL m_astListKeyTypeMapTbl[] = {
    {"connection", "index", DEF_KEY_VALUE_TYPE_NUMBER, "", NULL}
};

static uint8_t
__GetValueTypeOfListKey(const char *pcXpath, const char *pcListName, const char *pcKeyName)
{
    size_t nCnt = 0;
    size_t i = 0;

    nCnt = sizeof(m_astListKeyTypeMapTbl) / sizeof(m_astListKeyTypeMapTbl[0]);
    for (i = 0; i < nCnt; i++) {
        if (0 != strcasecmp(m_astListKeyTypeMapTbl[i].pcListName, pcListName)) {
            continue;
        }

        if (0 != strcasecmp(m_astListKeyTypeMapTbl[i].pcKeyName, pcKeyName)) {
            continue;
        }

        if (NULL == m_astListKeyTypeMapTbl[i].pcUniqueKeywords) {

            return m_astListKeyTypeMapTbl[i].ucValueType;
        } else {
            if (NULL != strstr(pcXpath, m_astListKeyTypeMapTbl[i].pcUniqueKeywords)) {
                return m_astListKeyTypeMapTbl[i].ucValueType;
            }
        }
    }

    /* default value type */
    return DEF_KEY_VALUE_TYPE_STRING;
}

/*  pcXpath example:
    /openconfig-transport-line-protection:aps/aps-modules/aps-module/config
 */
static int
__GetModuleNameByYangXpath(const char *pcXpath, char *pcModuleName, size_t nLen)
{
    char *pcPos = NULL;
    size_t nLenTmp = 0;
    if (NULL == pcXpath || NULL == pcModuleName || 0 == strlen(pcXpath) || nLen <= 0) {
        goto END_LABEL;
    }

    if (*pcXpath != '/') {
        goto END_LABEL;
    }

    pcPos = strchr(pcXpath, ':');
    if (NULL == pcPos) {
        goto END_LABEL;
    }

    nLenTmp = pcPos - pcXpath - 1;
    if (nLenTmp >= nLen) {
        goto END_LABEL;
    }
    strncpy(pcModuleName, pcXpath + 1, nLenTmp);
    pcModuleName[nLenTmp] = '\0';
    return OPLK_OK;
END_LABEL:
    return OPLK_ERR;

}

static int
__GetKeyNameAndValue(const char *pcStr, char *pcKeyName, size_t nNameLen, char *pcKeyValue, size_t nValueLen)
{
    char *pcEndPos = NULL;
    char *pcEqualPos = NULL;
    size_t nLen = 0;
    char acKeyName[DEF_BUF_SIZE_128] = {0};
    char acKeyValue[DEF_BUF_SIZE_128] = {0};

    if (*pcStr != '[') {
        goto END_LABEL;
    }

    pcEqualPos = strchr(pcStr, '=');
    if (NULL == pcEqualPos) {
        /* error */
        goto END_LABEL;
    }

    nLen = pcEqualPos - pcStr - 1;
    if (nLen >= nNameLen) {
        LOG_WRITE(EN_LOG_ERROR, "pcEqualPos: %s", pcEqualPos);
        goto END_LABEL;
    }

    strncpy(acKeyName, pcStr + 1, nLen);

    pcEndPos = strchr(pcEqualPos + 1, ']');
    if (NULL == pcEndPos) {
        /* error */
        LOG_WRITE(EN_LOG_ERROR, "pcEqualPos: %s", pcEqualPos);
        goto END_LABEL;
    }

    /* remove '' */
    if ('\'' == *(pcEqualPos + 1)) {
        pcEqualPos += 2;
    } else {
        pcEqualPos++;
    }

    if ('\'' == *(pcEndPos - 1)) {
        nLen = pcEndPos - pcEqualPos - 1;
    } else {
        nLen = pcEndPos - pcEqualPos;
    }


    if (nLen >= nValueLen) {
        goto END_LABEL;
    }

    strncpy(acKeyValue, pcEqualPos, nLen);

    /*  */
    strncpy(pcKeyName, acKeyName, nNameLen);
    strncpy(pcKeyValue, acKeyValue, nValueLen);

    return OPLK_OK;
END_LABEL:
    return OPLK_ERR;
}

int
REST_CreateJsonInputFile(const cJSON *pstJson, char *pcTmpFile, size_t nLen)
{
    /* int suffix = 5; */
    int iFd = -1;
    char *pcJsonStr = NULL;

    snprintf(pcTmpFile, nLen, "/tmp/restapi_%d.json", getpid());

    /* iFd = mkstemps(pcTmpFile, suffix); */
    iFd = open(pcTmpFile, O_RDWR | O_CREAT | O_SYNC, 0666);
    if (iFd == -1) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to open file (%s), %s", pcTmpFile, strerror(errno));
        return OPLK_ERR;
    }

    pcJsonStr = cJSON_Print(pstJson);

    /* LOG_WRITE(EN_LOG_NOTICE, "pcJsonStr: %s", pcJsonStr); */
    if (NULL == pcJsonStr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcJsonStr");
        goto END_LABEL;
    }

    if (write(iFd, pcJsonStr, strlen(pcJsonStr)) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "write failed");
        goto END_LABEL;
    }

    DEF_SAFE_CLOSE(iFd);
    DEF_SAFE_FREE(pcJsonStr);

    return OPLK_OK;
END_LABEL:

    DEF_SAFE_CLOSE(iFd);
    DEF_SAFE_FREE(pcJsonStr);
    return OPLK_ERR;
}
/******************************************************************************
** the following cases are NOT Supported. To optimize
** put /data/optical-attenuator/attenuators/attenuator={name}
** put /data/optical-amplifier/amplifiers/amplifier={name}
******************************************************************************/
int
REST_CreateJsonByXpath(const char *pcXpath, cJSON **ppstRoot, cJSON **ppstLastNodeObj, cJSON **ppstLastNodeParentObj)
{

    const char *pcXpathTmp = pcXpath;
    /* char *pcLastNode = NULL; */
    cJSON *pstRootObj = NULL;
    cJSON *pstObj = NULL, *pstLastNodeObj = NULL, *pstLastNodeParentObj = NULL;
    char acYangNode[DEF_BUF_SIZE_256] = {0};
    size_t nYangNodeLen = 0, nLen = 0;
    char *pcSlashPos = NULL, *pcEqualPos = NULL, *pcPos = NULL, *pcStartPos = NULL;
    char acListName[DEF_BUF_SIZE_128] = {0};
    char acKeyName[DEF_BUF_SIZE_128] = {0};
    char acKeyValue[DEF_BUF_SIZE_128] = {0};
    int rc = OPLK_OK;
    uint8_t ucValueType = DEF_KEY_VALUE_TYPE_STRING;

    pstRootObj = cJSON_CreateObject();
    if (NULL == pstRootObj) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstRootObj");
        goto END_LABEL;
    }
    pstLastNodeParentObj = pstRootObj;
    pstLastNodeObj = pstRootObj;
    /* cJSON_AddItemToObject(pstRootObj, "name", name); */
    pcXpathTmp++;
    while ((NULL != pcXpathTmp) && ('\0' != *pcXpathTmp)) {
        /* LOG_WRITE(EN_LOG_NOTICE, "pcXpathTmp: %s", pcXpathTmp); */
        memset(acYangNode, 0, DEF_BUF_SIZE_256);
        pcSlashPos = strchr(pcXpathTmp, '/');
        if (NULL == pcSlashPos) {
            nYangNodeLen = strlen((char *)pcXpathTmp);
            if (nYangNodeLen >= sizeof(acYangNode)) {
                LOG_WRITE(EN_LOG_ERROR, "pcXpathTmp: %s", pcXpathTmp);
                goto END_LABEL;
            }
            memcpy(acYangNode, (char *)pcXpathTmp, nYangNodeLen);
            /* break; */
            pcXpathTmp = NULL;
        } else {
            nYangNodeLen = pcSlashPos - pcXpathTmp;
            strncpy(acYangNode, pcXpathTmp, nYangNodeLen);
            pcXpathTmp = pcSlashPos + 1;
        }

        pcEqualPos = strchr(acYangNode, '=');
        if (NULL == pcEqualPos) {
            pstObj = cJSON_CreateObject();
            if (NULL == pstObj) {
                LOG_WRITE(EN_LOG_ERROR, "NULL == pstRootObj");
                goto END_LABEL;
            }

            cJSON_AddItemToObject(pstLastNodeObj, acYangNode, pstObj);

            pstLastNodeParentObj = pstLastNodeObj;
            pstLastNodeObj = pstObj;
        } else {
            /* list node, acYangNode example:
               1. aps-module[name='OLP']
               2. channel[lower-frequency='196062500'][upper-frequency='196137500']
             */
            /* LOG_WRITE(EN_LOG_NOTICE, "acYangNode:%s", acYangNode); */
            pcPos = strchr(acYangNode, '[');
            if (NULL == pcPos) {
                goto END_LABEL;
            }

            nLen = pcPos - acYangNode;
            strncpy(acListName, acYangNode, nLen);

            cJSON *pstArrays = NULL;
            pstArrays = cJSON_CreateArray();
            if (NULL == pstArrays) {
                LOG_WRITE(EN_LOG_ERROR, "NULL == pstArrays");
                goto END_LABEL;
            }

            cJSON_AddItemToObject(pstLastNodeObj, acListName, pstArrays);
            pstObj = cJSON_CreateObject();
            if (NULL == pstObj) {
                LOG_WRITE(EN_LOG_ERROR, "NULL == pstObj");
                goto END_LABEL;
            }

            cJSON_AddItemToArray(pstArrays, pstObj);
            while (NULL != pcPos && '\0' != *pcPos) {
                pcStartPos = strchr(pcPos, '[');
                if (NULL == pcStartPos) {
                    /* NO key */
                    break;
                }

                memset(acKeyName, 0, sizeof(acKeyName));
                rc = __GetKeyNameAndValue(pcStartPos, acKeyName, sizeof(acKeyName), acKeyValue, sizeof(acKeyValue));
                if (rc != OPLK_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by __GetKeyNameAndValue(%s)", pcStartPos);
                    goto END_LABEL;
                }

                /* add list key (only support string type , TODO support other type) */
                ucValueType = __GetValueTypeOfListKey(pcXpath, acListName, acKeyName);
                /* LOG_WRITE(EN_LOG_NOTICE, "pcXpath: %s, acKeyName:%s, acKeyValue:%s, ucValueType:%d", pcXpath, acKeyName, acKeyValue, ucValueType); */
                if (DEF_KEY_VALUE_TYPE_NUMBER == ucValueType) {
                    if (NULL == cJSON_AddNumberToObject(pstObj, acKeyName, atoi(acKeyValue))) {
                        LOG_WRITE(EN_LOG_ERROR, "Error by cJSON_AddNumberToObject, acKeyName:%s, acKeyValue:%s", acKeyName, acKeyValue);
                        goto END_LABEL;
                    }

                } else {
                    if (NULL == cJSON_AddStringToObject(pstObj, acKeyName, acKeyValue)) {
                        LOG_WRITE(EN_LOG_ERROR, "Error by cJSON_AddStringToObject, acKeyName:%s, acKeyValue:%s", acKeyName, acKeyValue);
                        goto END_LABEL;
                    }
                }

                pcPos = pcStartPos + 1;
            }
            pstLastNodeParentObj = pstLastNodeObj;
            pstLastNodeObj = pstObj;
        }
    }

    *ppstRoot = pstRootObj;
    *ppstLastNodeObj = pstLastNodeObj;
    *ppstLastNodeParentObj = pstLastNodeParentObj;
    return OPLK_OK;
END_LABEL:
    cJSON_Delete(pstRootObj);
    return OPLK_ERR;
}
int
REST_Set(char *pcUrl, cJSON *pstBody)
{
    /* sr_val_t *pstValues = NULL; */
    int rc = SR_ERR_OK;
    char acYangXpath[OPLK_PATH_MAX_LEN] = {0};
    cJSON *pstRoot = NULL, *pstLastNodeObj = NULL, *pstLastNodeParentObj = NULL, *pstJsonDuplicate = NULL;
    char acJsonInputFile[DEF_BUF_SIZE_128] = {0};
    char acCmd[DEF_BUF_SIZE_512] = {0};
    cJSON *pstJsonTmp = NULL;
    char acModuleName[DEF_BUF_SIZE_64] = {0};
    char acPrefixModuleName[DEF_BUF_SIZE_64] = {0};
    char *pcPos = NULL;
    size_t nLen = 0;

    /* url to yang xpath */
    rc = REST_UrlToYangXpath((const char *)pcUrl, acYangXpath, sizeof(acYangXpath));
    if (rc != SR_ERR_OK) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "acYangXpath: %s", acYangXpath);

    /* convert yang xpath to json tree node */
    rc = REST_CreateJsonByXpath((const char *)acYangXpath, &pstRoot, &pstLastNodeObj, &pstLastNodeParentObj);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "acYangXpath: %s", acYangXpath);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = __GetModuleNameByYangXpath((const char *)acYangXpath, acModuleName, sizeof(acModuleName));
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Parse module name failed. acYangXpath: %s", acYangXpath);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    if (NULL == pstLastNodeObj || NULL == pstLastNodeObj->string) {
        LOG_WRITE(EN_LOG_ERROR, "pstLastNodeObj is NULL or empty object. yang: %s, url: %s", acYangXpath, pcUrl);
        /******************************************************************************
        ** NOTE: the following cases are NOT Supported. TODO
        ** put /data/optical-attenuator/attenuators/attenuator={name}
        ** put /data/optical-amplifier/amplifiers/amplifier={name}
        ******************************************************************************/
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }
    /* get last node object of yang xpath  from POST body data */
    if (cJSON_HasObjectItem(pstBody, pstLastNodeObj->string)) {
        pstJsonTmp = cJSON_GetObjectItem(pstBody, pstLastNodeObj->string);
        /* LOG_WRITE(EN_LOG_NOTICE, "Find: %s", pstLastNodeObj->string); */
    } else {
        /* check if the format of pstLastNodeObj->string is <module-name>:<top-level-container-name> */
        if ((pcPos = strchr(pstLastNodeObj->string, ':')) != NULL) {
            nLen = pcPos - pstLastNodeObj->string;
            if (nLen < sizeof(acPrefixModuleName)) {
                strncpy(acPrefixModuleName, pstLastNodeObj->string, nLen);
                acPrefixModuleName[nLen] = '\0';
                pcPos++;
                /* LOG_WRITE(EN_LOG_NOTICE, "Find: %s, acPrefixModuleName: %s, pcPos: %s", pstLastNodeObj->string, acPrefixModuleName, pcPos); */
                if ((0 == strcmp(acPrefixModuleName, acModuleName)) &&
                    (cJSON_HasObjectItem(pstBody, pcPos))) {
                    pstJsonTmp = cJSON_GetObjectItem(pstBody, pcPos);
                    goto CONTINUE_LABEL;
                }
            }
        }
        pstJsonTmp = pstBody;
        LOG_WRITE(EN_LOG_ERROR, "NOT Found: %s", pstLastNodeObj->string);
        rc = SR_ERR_NOT_FOUND;
        /* TODO, support */
        goto END_LABEL;
    }

CONTINUE_LABEL:

    /* copy data from pstBody, in case of memory free abnormal */
    pstJsonDuplicate = cJSON_Duplicate((const cJSON *)pstJsonTmp, 1);
    if (NULL == pstJsonDuplicate) {
        LOG_WRITE(EN_LOG_ERROR, "error cJSON_Duplicate");
        rc = SR_ERR_NO_MEMORY;
        goto END_LABEL;
    }

    /* remove last node object */
    if (NULL == cJSON_DetachItemViaPointer(pstLastNodeParentObj, pstLastNodeObj)) {
        LOG_WRITE(EN_LOG_ERROR, "error cJSON_DetachItemViaPointer");
        cJSON_Delete(pstJsonDuplicate);
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

    /* add subobject of pstBody to pstRoot (new json tree) */
    cJSON_AddItemToObject(pstLastNodeParentObj, pstLastNodeObj->string, pstJsonDuplicate);
    cJSON_Delete(pstLastNodeObj);

    /* creat sysrepo json data */
    rc = REST_CreateJsonInputFile((const cJSON *)pstRoot, acJsonInputFile, sizeof(acJsonInputFile));
    if (rc != OPLK_OK) {
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }
    /* LOG_WRITE(EN_LOG_NOTICE, "acJsonInputFile: %s", acJsonInputFile); */
    snprintf(acCmd, sizeof(acCmd), "/usr/local/bin/sysrepocfg --edit=%s --module %s --datastore running --format json", acJsonInputFile, acModuleName);
    rc = UTIL_System(acCmd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "rc: %d, acCmd: %s", rc, acCmd);
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "acJsonInputFile: %s", acJsonInputFile); */
END_LABEL:
    cJSON_Delete(pstRoot);
    UTIL_DeleteFile(acJsonInputFile);
    return rc;
}

int
__ParseRcpResponse(const char *pcRpcOutputFile, cJSON **ppstOutputJson)
{
    int rc = SR_ERR_OK;
    uint32_t uiLen = 0;
    char *pcJsonBuf = NULL;
    cJSON *pstRoot = NULL;

    pcJsonBuf = REST_GetFileContentWithEndChar((char *)pcRpcOutputFile, &uiLen);
    if (NULL == pcJsonBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Read file failed: %s", pcRpcOutputFile);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    pstRoot = cJSON_Parse(pcJsonBuf);
    if (NULL == pstRoot) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid json: %s", pcRpcOutputFile);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }
    /* DEF_SAFE_FREE(pcJsonBuf); */

    if (NULL == pstRoot->child) { \
        LOG_WRITE(EN_LOG_ERROR, "empty json: %s", pcRpcOutputFile);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL; \
    }
#if 0
    char *pcJsonStr = NULL;
    pcJsonStr = cJSON_Print(pstRoot->child);
    LOG_WRITE(EN_LOG_NOTICE, "New json object: %s", pcJsonStr);
    DEF_SAFE_FREE(pcJsonStr);
#endif
    /* remove unused node and return valid data */
    *ppstOutputJson = cJSON_Duplicate((const cJSON *)pstRoot->child, 1);
    if (NULL == *ppstOutputJson) {
        LOG_WRITE(EN_LOG_ERROR, "error cJSON_Duplicate");
        rc = SR_ERR_NO_MEMORY;
        goto END_LABEL;
    }


END_LABEL:
    DEF_SAFE_FREE(pcJsonBuf);
    cJSON_Delete(pstRoot);
    return rc;
}

int
REST_Rpc(char *pcUrl, const cJSON *pstInputJson, cJSON **ppstOutputJson)
{
    int rc = SR_ERR_OK;
    char acYangXpath[OPLK_PATH_MAX_LEN] = {0};
    cJSON *pstRoot = NULL, *pstLastNodeObj = NULL, *pstLastNodeParentObj = NULL, *pstJsonDuplicate = NULL;
    char acCmd[DEF_BUF_SIZE_512] = {0};
    char acJsonInputFile[DEF_BUF_SIZE_128] = {0};
    char acJsonOutputFile[DEF_BUF_SIZE_128] = {0};

    /* url to yang xpath */
    rc = REST_UrlToYangXpath((const char *)pcUrl, acYangXpath, sizeof(acYangXpath));
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acYangXpath: %s", acYangXpath);

    /* convert yang xpath to json tree node */
    rc = REST_CreateJsonByXpath((const char *)acYangXpath, &pstRoot, &pstLastNodeObj, &pstLastNodeParentObj);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "acYangXpath: %s", acYangXpath);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }
#if 0
    char *pcJsonStr = NULL;
    pcJsonStr = cJSON_Print(pstRoot);
    LOG_WRITE(EN_LOG_NOTICE, "json object: %s", pcJsonStr);
    DEF_SAFE_FREE(pcJsonStr);
#endif
    /* copy data from pstInputJson, in case of memory free abnormal */
    pstJsonDuplicate = cJSON_Duplicate((const cJSON *)pstInputJson, 1);
    if (NULL == pstJsonDuplicate) {
        LOG_WRITE(EN_LOG_ERROR, "error cJSON_Duplicate");
        rc = SR_ERR_NO_MEMORY;
        goto END_LABEL;
    }

    /* remove last node object */
    if (NULL == cJSON_DetachItemViaPointer(pstLastNodeParentObj, pstLastNodeObj)) {
        LOG_WRITE(EN_LOG_ERROR, "error cJSON_DetachItemViaPointer");
        cJSON_Delete(pstJsonDuplicate);
        pstJsonDuplicate = NULL;
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

    /* add subobject of pstBody to pstRoot (new json tree) */
    cJSON_AddItemToObject(pstLastNodeParentObj, pstLastNodeObj->string, pstJsonDuplicate);
    cJSON_Delete(pstLastNodeObj);
#if 0
    pcJsonStr = cJSON_Print(pstRoot);
    LOG_WRITE(EN_LOG_NOTICE, "New json object: %s", pcJsonStr);
    DEF_SAFE_FREE(pcJsonStr);
#endif
    /* creat sysrepo json data */
    rc = REST_CreateJsonInputFile((const cJSON *)pstRoot, acJsonInputFile, sizeof(acJsonInputFile));
    if (rc != OPLK_OK) {
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

    snprintf(acJsonOutputFile, sizeof(acJsonOutputFile), "restapi_rpc_output_%d.json", getpid());

    LOG_WRITE(EN_LOG_NOTICE, "acJsonInputFile: %s, acJsonOutputFile: %s", acJsonInputFile, acJsonOutputFile);
    snprintf(acCmd, sizeof(acCmd), "/usr/local/bin/sysrepocfg --rpc=%s --datastore running --format json > %s", acJsonInputFile, acJsonOutputFile);
    rc = UTIL_System(acCmd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "rc: %d, acCmd: %s", rc, acCmd);
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

    rc = __ParseRcpResponse(acJsonOutputFile, ppstOutputJson);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "error __ParseRcpResponse, rc: %d", rc);
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    }

END_LABEL:
    cJSON_Delete(pstRoot);
    UTIL_DeleteFile(acJsonInputFile);
    UTIL_DeleteFile(acJsonOutputFile);
    return rc;

}

int
REST_Delete(char *pcUrl)
{
    int rc = SR_ERR_OK;
    char acYangXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    rc = REST_UrlToYangXpath((const char *)pcUrl, acYangXpath, sizeof(acYangXpath));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_UrlToYangXpath");
        goto ERR_LABEL;
    }

    rc = sr_delete_item(pstSess, acYangXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_delete_item failed: %s, xpath: %s", sr_strerror(rc), acYangXpath);
        goto ERR_LABEL;
    }

    /* commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit failed in %s", __func__);
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;

}

