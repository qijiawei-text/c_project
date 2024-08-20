/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_common.c
* Description:   common functions
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2016/12/08  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"

static REQUEST_METHOD_KV_ST m_stReqMethod[] = {{"GET", METHOD_GET},
                                               {"POST", METHOD_POST},
                                               {"PUT", METHOD_PUT},
                                               {"PATCH", METHOD_PATCH},
                                               {"DELETE", METHOD_DELETE},
                                               {"CONNECT", METHOD_CONNECT},
                                               {"OPTIONS", METHOD_OPTIONS},
                                               {"HEAD", METHOD_HEAD},
                                               {"TRACE", METHOD_TRACE}};


static char m_acProductName[DEF_BUF_SIZE_32] = REST_PRODUCT_NAME_HCF;

/*************************************************************
* Name   : CMN_IsNumberString
* Descrp : validate the string is number string
* Input  : pcBuf: pointer to string
*          uiLen:  the length of string
* Output : NULL
* Return : VAL_OK: valid value, VAL_ERR: invalid value
*************************************************************/

static int
__IsNumberString(char *pcBuf, unsigned int uiLen)
{
    unsigned int uiIndex = 0;

    if (NULL == pcBuf || 0 == uiLen) {
        goto ERR_LABEL;
    }

    for (uiIndex = 0; uiIndex < uiLen; uiIndex++) {
        if (!isdigit(*(pcBuf + uiIndex))) {
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;

}

/*********************************************************************************************
   Name   : GetPageParas
   Descrp : Get the key value of the specified key name  from URL query string
   Input  :
            pcName   ------ key name
            pcValue   ------ key value
            uiLen       ------- the length of key value
            eType      ------  the type of key value, eg: int, string
   Output :
   Return : void
*********************************************************************************************/

int
GetSinglePara(URL_QUERY_AP_ST *pstQueryAP, const char *pcName, void *pcValue, unsigned int uiLen, VALUE_TYPE_E eType)
{
    unsigned int i = 0;

    if (NULL == pstQueryAP || NULL == pcName ||
        NULL == pcValue || uiLen < 1) {
        goto END_LABEL;
    }


    for (i = 0; i < pstQueryAP->uiNum; i++) {

        if (!strcasecmp(pcName, pstQueryAP->stAttrPair[i].acKey)) {
            if (INT_VALUE_TYPE == eType) {
                if (OPLK_OK != __IsNumberString(pstQueryAP->stAttrPair[i].acValue,
                                                strlen(pstQueryAP->stAttrPair[i].acValue))) {
                    goto END_LABEL;
                }
                *((int *)pcValue) = atoi(pstQueryAP->stAttrPair[i].acValue);
            } else {
                snprintf((char *)pcValue, uiLen, "%s", pstQueryAP->stAttrPair[i].acValue);
            }

            break;
        }
    }

    return OPLK_OK;
END_LABEL:
    return OPLK_ERR;

}

/*********************************************************************************************
   Name   : GetPageParas
   Descrp : Get page and page_size key-value in URL query string
   Input  :
            piPage   ------ page
            piPageSize   ------ page_size
   Output :
   Return : void
*********************************************************************************************/

int
GetPageParas(URL_QUERY_AP_ST *pstQueryAP, int *piPage, int *piPageSize)
{
    int iPage = 0;
    int iPageSize = 0;
    int iResult = 0;

    if (NULL == pstQueryAP || NULL == piPage || NULL == piPageSize) {
        goto END_LABEL;
    }

    /* at most 1 paras */
    if (0 == pstQueryAP->uiNum) {
        /* if there are no query conditions in the URL */
        iPage = 1;
    } else if (2 == pstQueryAP->uiNum) {
        iResult = GetSinglePara(pstQueryAP, KEY_PAGE_NUM, (void *)&iPage, sizeof(iPage), INT_VALUE_TYPE);
        iResult |= GetSinglePara(pstQueryAP, KEY_PAGE_SIZE, (void *)&iPageSize, sizeof(iPageSize), INT_VALUE_TYPE);
        if ((iResult != OPLK_OK) || iPage < 1 || iPageSize < 1) {
            goto END_LABEL;
        }

    } else {
        goto END_LABEL;
    }

    *piPage = iPage;
    *piPageSize = iPageSize;

    return OPLK_OK;
END_LABEL:
    return OPLK_ERR;
}

/* a Simple Numeric conversion function */
/* JSON number is double type and does not distinguish between integers and real numbers */
static int
__GetNumberValue(const cJSON *pstJson, JSON_OBJ_ST *pstJsonObj)
{
    /* process number */
    if (!cJSON_IsNumber(pstJson) && !cJSON_IsBool(pstJson)) {
        LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not number", pstJsonObj->pcNodeName);
        goto ERR_LABEL;
    }

    switch (pstJsonObj->iDataType) {
    case EN_DATA_TYPE_CHAR:
        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, pstJson->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not char! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        if (pstJson->valueint < -128 || pstJson->valueint > 127) {
            goto ERR_LABEL;
        }
        TAKE_VALUE_BY_TYPE(char, pstJsonObj->pvValue, pstJson->valueint);
        break;

    case EN_DATA_TYPE_UCHAR:
        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, pstJson->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not unsigned char! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        if (pstJson->valueint < 0 || pstJson->valueint > 255) {
            goto ERR_LABEL;
        }

        TAKE_VALUE_BY_TYPE(unsigned char, pstJsonObj->pvValue, pstJson->valueint);
        break;

    case EN_DATA_TYPE_SHORT:
        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, pstJson->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not short! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        if (pstJson->valueint < -32768 || pstJson->valueint > 32767) {
            goto ERR_LABEL;
        }
        TAKE_VALUE_BY_TYPE(short, pstJsonObj->pvValue, pstJson->valueint);
        break;

    case EN_DATA_TYPE_USHORT:
        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, pstJson->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not unsigned short! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        if (pstJson->valueint < 0 || pstJson->valueint > 65535) {
            goto ERR_LABEL;
        }

        TAKE_VALUE_BY_TYPE(unsigned short, pstJsonObj->pvValue, pstJson->valueint);
        break;

    case EN_DATA_TYPE_INT:
        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, pstJson->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not int! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }
        TAKE_VALUE_BY_TYPE(int, pstJsonObj->pvValue, pstJson->valueint);
        break;

    case EN_DATA_TYPE_UINT:
        if (pstJson->valuedouble < 0) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not unsigned int! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        if (!UTIL_IsDoubleEqual(pstJson->valuedouble, (unsigned int)pstJson->valuedouble)) {
            LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not unsigned int! value: %g", pstJsonObj->pcNodeName, pstJson->valuedouble);
            goto ERR_LABEL;
        }

        TAKE_VALUE_BY_TYPE(unsigned int, pstJsonObj->pvValue, pstJson->valuedouble);
        break;
    case EN_DATA_TYPE_FLOAT:
        TAKE_VALUE_BY_TYPE(float, pstJsonObj->pvValue, pstJson->valuedouble);
        break;

    case EN_DATA_TYPE_DOUBLE:
        TAKE_VALUE_BY_TYPE(double, pstJsonObj->pvValue, pstJson->valuedouble);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "Json type error. type:%d", pstJsonObj->iDataType);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}
int
JSON_GetObjItems(const cJSON *pstObjJson, JSON_OBJ_ST *pstJsonObj)
{
    int iIndex = 0;
    int iJsonLen = 0;
    cJSON *pstJson = NULL;

    while (pstJsonObj->pcNodeName != NULL) {
        /* fandy 20190131: json check node */
        pstJson = cJSON_GetArrayItem(pstObjJson, iIndex);
        /* pstJson = cJSON_GetObjectItem(pstObjJson, pstJsonObj->pcNodeName); */
        if (NULL == pstJson) {
            LOG_WRITE(EN_LOG_ERROR, "not find json node %s", pstJsonObj->pcNodeName);
            goto ERR_LABEL;
        }

        if (0 != strcmp(pstJsonObj->pcNodeName, pstJson->string)) {
            LOG_WRITE(EN_LOG_ERROR, "Json node name is error. must:%s. actual:%s", pstJsonObj->pcNodeName, pstJson->string);
            goto ERR_LABEL;
        }

        if (EN_DATA_TYPE_STRING == pstJsonObj->iDataType) {
            /* process string */
            if (!cJSON_IsString(pstJson)) {
                LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" is not string", pstJsonObj->pcNodeName);
                goto ERR_LABEL;
            }

            if (NULL == pstJson->valuestring) {
                LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" value is NULL", pstJsonObj->pcNodeName);
                goto ERR_LABEL;
            }

            if (strlen(pstJson->valuestring) > (pstJsonObj->uiLen - 1)) {
                LOG_WRITE(EN_LOG_ERROR, "the date type of node \"%s\" len %lu out of range %u",
                          pstJsonObj->pcNodeName, strlen(pstJson->valuestring), (pstJsonObj->uiLen - 1));
                goto ERR_LABEL;
            }

            snprintf(pstJsonObj->pvValue, pstJsonObj->uiLen, "%s", pstJson->valuestring);
        } else {
            /* process number */
            if (OPLK_OK != __GetNumberValue((const cJSON *)pstJson, pstJsonObj)) {
                goto ERR_LABEL;
            }
        }

        pstJsonObj++;
        iIndex++;
    }

    /* fandy 20190131: json check length */
    iJsonLen = cJSON_GetArraySize(pstObjJson);
    if (iIndex != iJsonLen) {
        LOG_WRITE(EN_LOG_ERROR, "Json length error. must:%d. actual:%d", iIndex, iJsonLen);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

cJSON *
JSON_ParseObject(char *pcValue, size_t tValLen)
{
    const char *pcReturnParseEnd = NULL;
    cJSON *pstBody = NULL;

    pstBody = cJSON_ParseWithOpts(pcValue, (const char **)&pcReturnParseEnd, 1);
    /* fprintf(stderr, "pstBody:%d, pcReturnParseEnd:%d\r\n", pstBody == NULL, pcReturnParseEnd == NULL); */

    /*if (pcReturnParseEnd != NULL)
       fprintf(stderr, "pcReturnParseEnd:%s, len:%d\r\n", pcReturnParseEnd, strlen(pcReturnParseEnd));*/

    return pstBody;
}

int
JSON_GetObjItemsOptional(const cJSON *pstObjJson, JSON_OBJ_NEW_ST *pstJsonObj)
{
    cJSON *pstJson = NULL;
    size_t i = 0, nItemNum = 0;
    bool bFound = false;
    JSON_OBJ_NEW_ST *pstDataObj = NULL;

    nItemNum = cJSON_GetArraySize(pstObjJson);

    if (0 == nItemNum) {
        LOG_WRITE(EN_LOG_ERROR, "no json node");
        goto ERR_LABEL;
    }

    for (i = 0; i < nItemNum; i++) {
        pstJson = cJSON_GetArrayItem(pstObjJson, i);
        if (NULL == pstJson) {
            LOG_WRITE(EN_LOG_ERROR, "not find json node %s", pstJsonObj->pcNodeName);
            goto ERR_LABEL;
        }

        bFound = false;
        pstDataObj = pstJsonObj;
        while (pstDataObj->pcNodeName != NULL) {
            if (0 == strcmp(pstDataObj->pcNodeName, pstJson->string)) {
                bFound = true;
                break;
            }

            pstDataObj++;
        }

        if (false == bFound) {
            LOG_WRITE(EN_LOG_ERROR, "undesired json node: %s", pstJson->string);
            goto ERR_LABEL;
        }

        *(pstDataObj->pbExist) = false;

        if (EN_DATA_TYPE_STRING == pstDataObj->iDataType) {
            /* process string */
            if (cJSON_IsNull(pstJson)) {
                LOG_WRITE(EN_LOG_WARNING, "the data type of node \"%s\" is null", pstDataObj->pcNodeName);
                *(pstDataObj->pbExist) = true;
                *((char *)pstDataObj->pvValue) = '\0';
                continue;
            }
            if (!cJSON_IsString(pstJson)) {
                LOG_WRITE(EN_LOG_ERROR, "the data type of node \"%s\" is not string", pstDataObj->pcNodeName);
                goto ERR_LABEL;
            }

            if (NULL == pstJson->valuestring) {
                LOG_WRITE(EN_LOG_WARNING, "the data type of node \"%s\" value is NULL", pstDataObj->pcNodeName);
                *(pstDataObj->pbExist) = true;
                *((char *)pstDataObj->pvValue) = '\0';
                continue;
            }

            if (strlen(pstJson->valuestring) > (pstDataObj->uiLen - 1)) {
                LOG_WRITE(EN_LOG_ERROR, "the data type of node \"%s\" len %lu out of range %u",
                          pstDataObj->pcNodeName, strlen(pstJson->valuestring), (pstDataObj->uiLen - 1));
                goto ERR_LABEL;
            }

            snprintf(pstDataObj->pvValue, pstDataObj->uiLen, "%s", pstJson->valuestring);
        } else {
            /* process number */
            if (OPLK_OK != __GetNumberValue((const cJSON *)pstJson, (JSON_OBJ_ST *)pstDataObj)) {
                goto ERR_LABEL;
            }


        }

        *(pstDataObj->pbExist) = true;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

REQ_METHOD_EN
REST_MethodToEnum(char *pcMethod)
{
    size_t i = 0, nCount = 0;
    if (NULL == pcMethod) {
        goto END_LABEL;
    }

    nCount = sizeof(m_stReqMethod) / sizeof(m_stReqMethod[0]);
    for (i = 0; i < nCount; i++) {
        if (0 == strcasecmp(pcMethod, m_stReqMethod[i].pcMethod)) {
            return m_stReqMethod[i].enMethode;
        }
    }

END_LABEL:

    return METHOD_INVALID;
}

const char *
REST_MethodToString(REQ_METHOD_EN enMethod)
{
    size_t i = 0, nCount = 0;

    nCount = sizeof(m_stReqMethod) / sizeof(m_stReqMethod[0]);
    for (i = 0; i < nCount; i++) {
        if (m_stReqMethod[i].enMethode == enMethod) {
            return m_stReqMethod[i].pcMethod;
        }
    }

    return "Invalid";
}

/* *pulFileLen = actual length + 1 */
char *
REST_GetFileContentWithEndChar(char *pcFileName, uint32_t *puiFileLen)
{
    uint32_t uiFileLen = 0;
    char *pcBuf = NULL;
    FILE *pfFile = NULL;

    if ((NULL == pcFileName) ||
        (NULL == puiFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "input is null");
        goto ERR_LABEL;
    }

    /* Gets file length for malloc RAM                                          */
    uiFileLen = UTIL_GetFileLen(pcFileName);
    if (0 == uiFileLen) {
        LOG_WRITE(EN_LOG_ERROR, "ulFileLen is zero");
        goto ERR_LABEL;
    }

    /* add String termination character "\0" */
    uiFileLen += 1;

    /* Malloc RAM                                                               */
    if (NULL == (pcBuf = (char *)malloc(uiFileLen))) {
        LOG_WRITE(EN_LOG_ERROR, "malloc is error");
        goto ERR_LABEL;
    }
    memset(pcBuf, 0, uiFileLen);

    /* Copies the file into RAM buffer                                          */
    if (NULL == (pfFile = fopen(pcFileName, "r"))) {
        LOG_WRITE(EN_LOG_ERROR, "fopen is error");
        goto ERR_LABEL;
    }

    /* Copies file to RAM                                                       */
    if ((uiFileLen - 1) != fread(pcBuf, sizeof(uint8_t), uiFileLen - 1, pfFile)) {
        LOG_WRITE(EN_LOG_ERROR, "fread is error");
        goto ERR_LABEL;
    }

    /* File is no use, close it now                                             */
    DEF_SAFE_FCLOSE(pfFile);

    *puiFileLen = uiFileLen;

    return pcBuf;

ERR_LABEL:
    DEF_SAFE_FCLOSE(pfFile);
    DEF_SAFE_FREE(pcBuf);

    return NULL;
}

int
REST_SetEachFlag(void *pstFlag, uint32_t uiLen, bool bFlag)
{
    uint32_t i = 0;

    if (NULL == pstFlag || uiLen == 0) {

        return OPLK_ERR;
    }

    /* sizeof(bool) = 1 */
    for (i = 0; i < uiLen; i++) {
        *((bool *)pstFlag + i) = bFlag;
    }

    return OPLK_OK;
}

void
REST_SetProductName(const char *pcName)
{
    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_NOTICE, "default product name: %s", m_acProductName);
    } else {
        snprintf(m_acProductName, sizeof(m_acProductName), "%s", pcName);
        LOG_WRITE(EN_LOG_NOTICE, "set product name: %s", m_acProductName);
    }

    return;
}

bool
REST_IsProductName(const char *pcName)
{
    if (NULL == pcName) {
        return false;
    }

    return (0 == strcmp(m_acProductName, pcName)) ? true : false;
}
