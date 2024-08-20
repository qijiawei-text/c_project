/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_api_msft.c
* Description:   REST APIs of handling JSON data, only used for MSFT
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/13  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_api_msft.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "oplk_file.h"
#include "rest_edfa.h"
#include "rest_handle.h"

#define REST_PARSE_INEX_VAULE(INDEX, pstJsonObj) \
    { \
        pstLeaf = cJSON_GetObjectItem(pstJsonObj, "index"); \
        if (NULL == pstLeaf) { \
            LOG_WRITE(EN_LOG_ERROR, "Not found %s", "index"); \
            goto ERR_LABEL; \
        } \
        if (!cJSON_IsNumber(pstLeaf)) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid data type for index: %d", pstLeaf->type); \
            goto ERR_LABEL; \
        } \
        if (!UTIL_IsDoubleEqual(pstLeaf->valuedouble, pstLeaf->valueint)) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for index: %g", pstLeaf->valuedouble); \
            goto ERR_LABEL; \
        } \
        if (pstLeaf->valueint <= 0 || pstLeaf->valueint > OCS_CONNECTIONS_MAX) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for index: %g", pstLeaf->valueint); \
            goto ERR_LABEL; \
        } \
         \
        INDEX = pstLeaf->valueint; \
    }

#define REST_PARSE_PORT_VALUE(PORT_VAR, PORT_NODE_NAME, PORT_NAME_PREFIX, pstJsonObj) \
    { \
        pstLeaf = cJSON_GetObjectItem(pstJsonObj, PORT_NODE_NAME); \
        if (NULL == pstLeaf) { \
            LOG_WRITE(EN_LOG_ERROR, "Not found %s", PORT_NODE_NAME); \
            goto ERR_LABEL; \
        } \
        if (!cJSON_IsString(pstLeaf)) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid data type for %s: %d", PORT_NODE_NAME, pstLeaf->type); \
            goto ERR_LABEL; \
        } \
        iPortId = 0; \
        if (sscanf(pstLeaf->valuestring, PORT_NAME_PREFIX "%d", &iPortId) != 1) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid %s: %s!", PORT_NODE_NAME, pstLeaf->valuestring); \
            goto ERR_LABEL; \
        } \
    \
        if (iPortId <= 0 || iPortId > OCS_CONNECTIONS_MAX) { \
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for %s: %s", PORT_NODE_NAME, pstLeaf->valuestring); \
            goto ERR_LABEL; \
        } \
    \
        snprintf(PORT_VAR, sizeof(PORT_VAR), "%s", pstLeaf->valuestring); \
    }

static int
__ParseConnections(char *pcBody, size_t nLen, OCS_CONNECTIONS_ST *pstConns)
{
    cJSON *pstBody = NULL;
    cJSON *pstArrayJson = NULL;
    cJSON *pstJson = NULL;
    cJSON *pstLeaf = NULL;
    cJSON *pstConfig = NULL;
    int iCnt = 0;
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    int iPortId = 0;
    uint32_t uiIndex = 0;

    memset(&stConns, 0, sizeof(stConns));
    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject(pcBody, nLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        goto ERR_LABEL;
    }
    /* only have "connections" object */
    iCnt = cJSON_GetArraySize(pstBody);
    if (iCnt != 1) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    pstJson = cJSON_GetObjectItem(pstBody, "connections");
    if (NULL == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "Not found %s", "connections");
        goto ERR_LABEL;
    }

    /* only have "connection" object */
    iCnt = cJSON_GetArraySize(pstJson);
    if (iCnt != 1) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data for connections! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    pstArrayJson = cJSON_GetObjectItem(pstJson, "connection");
    if (NULL == pstArrayJson) {
        LOG_WRITE(EN_LOG_ERROR, "Not found %s", "connection");
        goto ERR_LABEL;
    }


    cJSON_ArrayForEach(pstJson, pstArrayJson) {
        stConns.uiConnCnt++;
        if (stConns.uiConnCnt > OCS_CONNECTIONS_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid body data! connections are more than %s", OCS_CONNECTIONS_MAX);
            goto ERR_LABEL;
        }
        pstConn = stConns.astConnection + (stConns.uiConnCnt - 1);

        /* have two node "index" and "config" */
        iCnt = cJSON_GetArraySize(pstJson);
        if (iCnt != 2) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid body data for connection! iCnt: %d", iCnt);
            goto ERR_LABEL;
        }

        REST_PARSE_INEX_VAULE(pstConn->uiIndex, pstJson);
#if 0
        pstLeaf = cJSON_GetObjectItem(pstJson, "index");
        if (NULL == pstLeaf) {
            LOG_WRITE(EN_LOG_ERROR, "Not found %s", "index");
            goto ERR_LABEL;
        }

        if (!cJSON_IsNumber(pstLeaf)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid data type for index: %d", pstLeaf->type);
            goto ERR_LABEL;
        }

        if (!UTIL_IsDoubleEqual(pstLeaf->valuedouble, pstLeaf->valueint)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for index: %g", pstLeaf->valuedouble);
            goto ERR_LABEL;
        }

        if (pstLeaf->valueint <= 0 || pstLeaf->valueint > OCS_CONNECTIONS_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for index: %g", pstLeaf->valueint);
            goto ERR_LABEL;
        }

        pstConn->uiIndex = pstLeaf->valueint;
#endif

        pstConfig = cJSON_GetObjectItem(pstJson, "config");
        if (NULL == pstConfig) {
            LOG_WRITE(EN_LOG_ERROR, "Not found %s", "config");
            goto ERR_LABEL;
        }

        iCnt = cJSON_GetArraySize(pstConfig);
        if (iCnt != 3) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid body data for config! iCnt: %d", iCnt);
            goto ERR_LABEL;
        }


        REST_PARSE_INEX_VAULE(uiIndex, pstConfig);
        if (pstConn->uiIndex != uiIndex) {
            LOG_WRITE(EN_LOG_ERROR, "Inconsistent index! %d, %d", pstConn->uiIndex, uiIndex);
            goto ERR_LABEL;
        }

        REST_PARSE_PORT_VALUE(pstConn->acSPort, OCS_SOURCE_NODE, DEF_OCS_PORT_IN_PREFIX, pstConfig);
        if (pstConn->uiIndex != iPortId) {
            LOG_WRITE(EN_LOG_ERROR, "Inconsistent index and source! %d, %s", pstConn->uiIndex, pstConn->acSPort);
            goto ERR_LABEL;
        }

        REST_PARSE_PORT_VALUE(pstConn->acDPort, OCS_DEST_NODE, DEF_OCS_PORT_OUT_PREFIX, pstConfig);
#if 0
        pstLeaf = cJSON_GetObjectItem(pstConfig, OCS_SOURCE_NODE);
        if (NULL == pstLeaf) {
            LOG_WRITE(EN_LOG_ERROR, "Not found %s", OCS_SOURCE_NODE);
            goto ERR_LABEL;
        }
        if (!cJSON_IsString(pstLeaf)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid data type for %s: %d", OCS_SOURCE_NODE, pstLeaf->type);
            goto ERR_LABEL;
        }
        /* snprintf(pstConn->acSPort, sizeof(pstConn->acSPort), "%s", pstLeaf->valuestring); */

        iPortId = 0;
        if (sscanf(pstLeaf->valuestring, DEF_OCS_PORT_IN_PREFIX "%d", &iPortId) != 1) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid source: %s", pstLeaf->valuestring);
            goto ERR_LABEL;
        }

        if (pstLeaf->valueint <= 0 || pstLeaf->valueint > OCS_CONNECTIONS_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid data for %s: %g", OCS_SOURCE_NODE, pstLeaf->valuestring);
            goto ERR_LABEL;
        }

        snprintf(pstConn->acSPort, sizeof(pstConn->acSPort), "%s", pstLeaf->valuestring);
#endif
        /* LOG_WRITE(EN_LOG_ERROR, "[%d] source: %s", j, pstLeaf->valuestring); */
        /* LOG_WRITE(EN_LOG_ERROR, "[%d] dest: %s", j, pstLeaf->valuestring); */
    }


    if (0 == stConns.uiConnCnt) {
        LOG_WRITE(EN_LOG_ERROR, "stConns.uiConnCnt == 0");
        goto ERR_LABEL;
    }

    memcpy(pstConns, &stConns, sizeof(stConns));
    cJSON_Delete(pstBody);
    return OPLK_OK;
ERR_LABEL:
    cJSON_Delete(pstBody);
    return OPLK_ERR;

}

/* POST /data/connections */
void
API_OcsAddConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCS_CONNECTIONS_ST stConns;

    rc = __ParseConnections((char *)pstReq->pcBody, pstReq->nBodyLen, &stConns);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__ParseConnections is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_OcsAddConnections(&stConns);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

/* TODO */
void
API_OcsUpdateConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCS_CONNECTIONS_ST stConns;

    rc = __ParseConnections((char *)pstReq->pcBody, pstReq->nBodyLen, &stConns);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__ParseConnections is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_OcsUpdateConnections(&stConns);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

/* PUT /data/connections */
void
API_OcsReplaceConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCS_CONNECTIONS_ST stConns;

    rc = __ParseConnections((char *)pstReq->pcBody, pstReq->nBodyLen, &stConns);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__ParseConnections is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_OcsReplaceConnections(&stConns);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

/* DELETE /data/connections */
void
API_OcsDeleteAllConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    rc = DSC_OcsDeleteAllConnections();
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

/* DELETE /data/connections/connection={index} */
/* DELETE /data/connections/connection={index}/config */
void
API_OcsDeleteConnection(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    int iIndex = 0;
    char acSuffix[DEF_BUF_SIZE_128] = {0};

    rc = sscanf(pstReq->acUrl, URL_DATA_OCS_CONNECTION_ONE "%d%s", &iIndex, acSuffix);
    if (1 == rc || (2 == rc && 0 == strcasecmp(acSuffix, "/config"))) {
        rc = DSC_OcsDeleteConnectionByIndex(iIndex);
        if (SR_ERR_OK != rc) {
            iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
            goto END_LABEL;
        }
    } else {
        iResult = HTTP_STATUS_CODE_405;
        LOG_WRITE(EN_LOG_ERROR, "Invalid request! method: %s, url: %s", pstReq->enMethod, pstReq->acUrl);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

static int
__ParseOneConnection(char *pcBody, size_t nLen, OCS_CONNECTION_ST *pstConn)
{
    cJSON *pstBody = NULL;
    cJSON *pstJson = NULL;
    cJSON *pstLeaf = NULL;
    cJSON *pstConfig = NULL;
    int iCnt = 0;
    OCS_CONNECTION_ST stConn;
    int iPortId = 0;
    uint32_t uiIndex = 0;

    memset(&stConn, 0, sizeof(stConn));
    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject(pcBody, nLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        goto ERR_LABEL;
    }

    /* only have "connection" object */
    iCnt = cJSON_GetArraySize(pstBody);
    if (iCnt != 1) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data for connections! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    pstJson = cJSON_GetObjectItem(pstBody, "connection");
    if (NULL == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "Not found %s", "connection");
        goto ERR_LABEL;
    }


    /* have two node "index" and "config" */
    iCnt = cJSON_GetArraySize(pstJson);
    if (iCnt != 2) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data for connection! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    REST_PARSE_INEX_VAULE(stConn.uiIndex, pstJson);

    pstConfig = cJSON_GetObjectItem(pstJson, "config");
    if (NULL == pstConfig) {
        LOG_WRITE(EN_LOG_ERROR, "Not found %s", "config");
        goto ERR_LABEL;
    }

    iCnt = cJSON_GetArraySize(pstConfig);
    if (iCnt != 3) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data for config! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }


    REST_PARSE_INEX_VAULE(uiIndex, pstConfig);
    if (stConn.uiIndex != uiIndex) {
        LOG_WRITE(EN_LOG_ERROR, "Inconsistent index! %d, %d", stConn.uiIndex, uiIndex);
        goto ERR_LABEL;
    }

    REST_PARSE_PORT_VALUE(stConn.acSPort, OCS_SOURCE_NODE, DEF_OCS_PORT_IN_PREFIX, pstConfig);
    if (stConn.uiIndex != iPortId) {
        LOG_WRITE(EN_LOG_ERROR, "Inconsistent index and source! %d, %s", stConn.uiIndex, stConn.acSPort);
        goto ERR_LABEL;
    }

    REST_PARSE_PORT_VALUE(stConn.acDPort, OCS_DEST_NODE, DEF_OCS_PORT_OUT_PREFIX, pstConfig);

    memcpy(pstConn, &stConn, sizeof(stConn));
    cJSON_Delete(pstBody);
    return OPLK_OK;
ERR_LABEL:
    cJSON_Delete(pstBody);
    return OPLK_ERR;
}

/* POST /data/connections/connection */
/* PUT /data/connections/connection={index} */
/* add or update one connection config */
void
API_OcsSetOneConnection(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCS_CONNECTION_ST stConn;
    OCS_CONNECTIONS_ST stConns;
    int iIndex = 0;
    char acSuffix[DEF_BUF_SIZE_128] = {0};

    memset(&stConn, 0, sizeof(stConn));
    memset(&stConns, 0, sizeof(stConns));

    if (METHOD_POST == pstReq->enMethod) {
        if (0 != strcasecmp(URL_DATA_OCS_CONNECTION, pstReq->acUrl)) {
            iResult = HTTP_STATUS_CODE_404;
            LOG_WRITE(EN_LOG_ERROR, "Invalid request! method: %s, url: %s", pstReq->enMethod, pstReq->acUrl);
            goto END_LABEL;
        }
    } else {
        rc = sscanf(pstReq->acUrl, URL_DATA_OCS_CONNECTION_ONE "%d%s", &iIndex, acSuffix);
        if (1 == rc) {
            /* parse ok. PUT /data/connections/connection={index} */
        } else {
            iResult = HTTP_STATUS_CODE_405;
            LOG_WRITE(EN_LOG_ERROR, "Invalid request! method: %s, url: %s", pstReq->enMethod, pstReq->acUrl);
            goto END_LABEL;
        }
    }

    rc = __ParseOneConnection((char *)pstReq->pcBody, pstReq->nBodyLen, &stConn);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__ParseOneConnection is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    stConns.uiConnCnt = 1;
    memcpy(stConns.astConnection, &stConn, sizeof(stConn));
    if (METHOD_POST == pstReq->enMethod) {
        rc = DSC_OcsAddConnections(&stConns);
    } else if (METHOD_PUT == pstReq->enMethod) {
        if (iIndex != stConn.uiIndex) {
            LOG_WRITE(EN_LOG_ERROR, "Inconsistent index! url index: %d, body index: %d", iIndex, stConn.uiIndex);
            iResult = HTTP_STATUS_CODE_405;
            goto END_LABEL;
        }
        rc = DSC_OcsUpdateConnections(&stConns);
    }
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

static int
__ParseConnectionCfg(char *pcBody, size_t nLen, OCS_CONNECTION_ST *pstConn)
{
    cJSON *pstBody = NULL;
    cJSON *pstJson = NULL;
    cJSON *pstLeaf = NULL;
    int iCnt = 0;
    OCS_CONNECTION_ST stConn;
    int iPortId = 0;

    memset(&stConn, 0, sizeof(stConn));
    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject(pcBody, nLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        goto ERR_LABEL;
    }
    /* only have "config" object */
    iCnt = cJSON_GetArraySize(pstBody);
    if (iCnt != 1) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    pstJson = cJSON_GetObjectItem(pstBody, "config");
    if (NULL == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "Not found %s", "config");
        goto ERR_LABEL;
    }

    /* have two node "index" and "config" */
    iCnt = cJSON_GetArraySize(pstJson);
    if (iCnt != 3) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid body data for connection! iCnt: %d", iCnt);
        goto ERR_LABEL;
    }

    REST_PARSE_INEX_VAULE(stConn.uiIndex, pstJson);

    REST_PARSE_PORT_VALUE(stConn.acSPort, OCS_SOURCE_NODE, DEF_OCS_PORT_IN_PREFIX, pstJson);
    if (stConn.uiIndex != iPortId) {
        LOG_WRITE(EN_LOG_ERROR, "Inconsistent index and source! %d, %s", stConn.uiIndex, stConn.acSPort);
        goto ERR_LABEL;
    }

    REST_PARSE_PORT_VALUE(stConn.acDPort, OCS_DEST_NODE, DEF_OCS_PORT_OUT_PREFIX, pstJson);

    memcpy(pstConn, &stConn, sizeof(stConn));

    cJSON_Delete(pstBody);
    return OPLK_OK;
ERR_LABEL:
    cJSON_Delete(pstBody);
    return OPLK_ERR;

}

/* POST/PUT /data/connections/connection={index}/config */
/* add or update one connection config */
void
API_OcsSetOneConnectionConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCS_CONNECTION_ST stConn;
    OCS_CONNECTIONS_ST stConns;
    int iIndex = 0;
    char acSuffix[DEF_BUF_SIZE_128] = {0};

    memset(&stConn, 0, sizeof(stConn));
    memset(&stConns, 0, sizeof(stConns));

    rc = sscanf(pstReq->acUrl, URL_DATA_OCS_CONNECTION_ONE "%d%s", &iIndex, acSuffix);
    if ((2 == rc) && (0 == strcasecmp(acSuffix, "/config"))) {
        /* parse ok */
    } else {
        iResult = HTTP_STATUS_CODE_405;
        LOG_WRITE(EN_LOG_ERROR, "Invalid request! method: %d, url: %s", pstReq->enMethod, pstReq->acUrl);
        goto END_LABEL;
    }

    rc = __ParseConnectionCfg((char *)pstReq->pcBody, pstReq->nBodyLen, &stConn);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__ParseConnections is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    if (iIndex != stConn.uiIndex) {
        LOG_WRITE(EN_LOG_ERROR, "Inconsistent index! url index: %d, body index: %d", iIndex, stConn.uiIndex);
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    stConns.uiConnCnt = 1;
    memcpy(stConns.astConnection, &stConn, sizeof(stConn));
    if (METHOD_POST == pstReq->enMethod) {
        rc = DSC_OcsAddConnections(&stConns);
    } else if (METHOD_PUT == pstReq->enMethod) {
        rc = DSC_OcsUpdateConnections(&stConns);
    }
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

