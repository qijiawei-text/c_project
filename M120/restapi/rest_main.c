/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_main.c
* Description:   REST APIs processing for Oplink Devices
* Author:        Jimmy and Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1] Jimmy and Guangjun Guo   2016/12/07  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_handle.h"
#include "rest_main.h"
#include "rest_log.h"
#include "rest_msft.h"
#include "rest_auth.h"
#include "rest_api_auth.h"
#include "rest_pm.h"
#include "rest_sr_util.h"

void
__Mg_CopyMgstr(const struct mg_str *pstMgStr, char *pcDestStr, size_t nDestLen)
{
    size_t nLen = 0;

    if (NULL == pstMgStr || NULL == pcDestStr || 0 == nDestLen) {
        return;
    }

    nLen = (nDestLen < pstMgStr->len) ? nDestLen : pstMgStr->len;

    memcpy(pcDestStr, pstMgStr->ptr, nLen);

    *(pcDestStr + nLen) = '\0';

    return;
}

static int
__REST_InitEntryData(struct mg_http_message *pstHttpMsg, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    struct mg_str *pstMgStrTmp = NULL;
    unsigned int uiLen = 0;
    char acMethod[16] = {0};

    /* get http request method */
    __Mg_CopyMgstr(&pstHttpMsg->method, acMethod, sizeof(acMethod));

    pstReq->enMethod = REST_MethodToEnum(acMethod);
    if (METHOD_INVALID == pstReq->enMethod) {
        goto ERR_LABEL;
    }

    /* get url */
    __Mg_CopyMgstr(&pstHttpMsg->uri, pstReq->acUrl, sizeof(pstReq->acUrl));
    /* remove  last "/" for URL that has been terminated by '/' character, eg: /omt/config/system/, except root path "/" */
    uiLen = strlen(pstReq->acUrl);
    if ((uiLen > 1) && ('/' == pstReq->acUrl[uiLen - 1])) {
        pstReq->acUrl[uiLen - 1] = '\0';
    }

    /* parse TOKEN, optional */
    pstMgStrTmp = mg_http_get_header(pstHttpMsg, "TOKEN");
    if (pstMgStrTmp != NULL) {
        __Mg_CopyMgstr(pstMgStrTmp, pstReq->acToken, sizeof(pstReq->acToken));
    }

    pstMgStrTmp = mg_http_get_header(pstHttpMsg, "X-Forwarded-For");
    if (pstMgStrTmp != NULL) {
        __Mg_CopyMgstr(pstMgStrTmp, pstReq->acRemoteAddr, sizeof(pstReq->acRemoteAddr));
    }
    /* create JSON obj node */
    pstResp->pstResponse = cJSON_CreateObject();

    /* get body  */
    pstReq->pcBody = NULL;
    pstReq->nBodyLen = 0;
    if ((METHOD_POST == pstReq->enMethod) || (METHOD_PUT == pstReq->enMethod)
        || (METHOD_PATCH == pstReq->enMethod)) {

        if (pstHttpMsg->body.len > HTTP_BODY_MAX_LEN) {
            LOG_WRITE(EN_LOG_ERROR, "Data length has exceeded the upper limit.");
            return -1;
        }

        if (pstHttpMsg->body.len != 0) {
            pstReq->nBodyLen = pstHttpMsg->body.len + 1;
            pstReq->pcBody = (char *)malloc(pstReq->nBodyLen);
            if (NULL == pstReq->pcBody) {
                LOG_WRITE(EN_LOG_ERROR, "malloc failed");
                return -1;
            }

            memset(pstReq->pcBody, 0, pstReq->nBodyLen);
            memcpy(pstReq->pcBody, pstHttpMsg->body.ptr, pstHttpMsg->body.len);
        }
    }

    pstResp->iStatus = HTTP_STATUS_CODE_500;

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static void
__REST_FreeEntryData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{

    DEF_SAFE_FREE(pstReq->pcBody);

    cJSON_Delete(pstResp->pstResponse);
    pstResp->pstResponse = NULL;

    return;
}

/* Convert the status code into string format */
/* copy mg_http_status_code_str from mongoose.c */
static const char *
__ConvertStatusCodeToStr(int status_code)
{
    switch (status_code) {
    case 100: return "Continue";
    case 201: return "Created";
    case 202: return "Accepted";
    case 204: return "No Content";
    case 206: return "Partial Content";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 304: return "Not Modified";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 418: return "I'm a teapot";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    default: return "OK";
    }
}

static void
__SendResponse(struct mg_connection *pstMgConn, int iStatusCode, char *pcResponse, const char *pcExtraHeaders)
{
    /* Send headers, add http status code description, TODO */
    if (NULL == pcExtraHeaders) {
        mg_printf(pstMgConn, "HTTP/1.1 %d %s\r\nTransfer-Encoding: chunked\r\n\r\n", iStatusCode, __ConvertStatusCodeToStr(iStatusCode));
    } else {
        mg_printf(pstMgConn, "HTTP/1.1 %d %s\r\n%s\r\nTransfer-Encoding: chunked\r\n\r\n", iStatusCode, __ConvertStatusCodeToStr(iStatusCode), pcExtraHeaders);
    }

    /* send body data */
    if (pcResponse != NULL) {
        mg_http_printf_chunk(pstMgConn, pcResponse);
    }

    mg_http_write_chunk(pstMgConn, "", 0);     /* Send empty chunk, the end of response */

}

static bool
__HasResponseData(char *pcUrl, REQ_METHOD_EN enMethod)
{

    if (METHOD_GET == enMethod) {
        if (0 == strncasecmp(pcUrl, URL_OMT_USER_LOGOUT, strlen(URL_OMT_USER_LOGOUT))) {
            return false;
        } else {
            return true;
        }
    }

    if (0 == strncasecmp(pcUrl, "/operations/", strlen("/operations/"))) {
        return true;
    }

    return false;
}

static void
__REST_Response(struct mg_connection *pstMgConn, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    char *pcResponse = NULL;
    char acExtraHeaders[DEF_BUF_SIZE_512] = {0};
    int iStatus = pstResp->iStatus;


    if (iStatus >= HTTP_STATUS_CODE_200 && iStatus <= HTTP_STATUS_CODE_299) {
        /* response ok */
        /* add extra header data */
        if ((METHOD_POST == pstReq->enMethod) && (0 == strcasecmp(pstReq->acUrl, URL_OMT_USER_LOGIN))) {
            snprintf(acExtraHeaders, sizeof(acExtraHeaders), "TOKEN: %s", pstReq->acToken);
        }

        /* add body data */
        if (__HasResponseData(pstReq->acUrl, pstReq->enMethod)) {
            pcResponse = cJSON_Print(pstResp->pstResponse);
        }

    } else {

        /* response failed, TBD */
    }

    if (strlen(acExtraHeaders) > 0) {
        __SendResponse(pstMgConn, iStatus, pcResponse, (const char *)acExtraHeaders);
    } else {
        __SendResponse(pstMgConn, iStatus, pcResponse, NULL);
    }

    DEF_SAFE_FREE(pcResponse);

    return;
}

void
REST_MainEntry(struct mg_connection *pstMgConn, struct mg_http_message *pstHttpMsg)
{
    REST_REQ_ST stReq;
    REST_RESP_ST stResp;
    REST_HANDLE_ST *pstHandle = NULL;
    int iStatus = HTTP_STATUS_CODE_200;
    TOKEN_ST *pstToken = NULL;
    AUTH_ST stAuth;

    /* 1. init request and response structural body */
    memset(&stReq, 0, sizeof(stReq));
    memset(&stResp, 0, sizeof(stResp));

    /* fprintf(stderr, "\r\nmethod: %s[%ld]\r\n", pstHttpMsg->method.p, pstHttpMsg->method.len); */

    if (OPLK_OK != __REST_InitEntryData(pstHttpMsg, &stReq, &stResp)) {
        stResp.iStatus = HTTP_STATUS_CODE_405;
        LOG_WRITE(EN_LOG_ERROR, "Error by __REST_InitEntryData.");
        goto END_LABEL;
    }

    /* fprintf(stderr, "\r\nmessage: %s\r\n", hm->message.p); */

    /* 2. refresh session */
    /* (void)DSC_ManuRefeshSession(); */

    /* 3. handle rest api request */
    if ((METHOD_POST == stReq.enMethod) && (0 == strcasecmp(stReq.acUrl, URL_OMT_USER_LOGIN))) {
        /* 3.1 handle login */
        API_Login(&stReq, &stResp);
    } else {
        /* 3.2 auth */
        memset(&stAuth, 0, sizeof(stAuth));
        memcpy(&stAuth.acToken, stReq.acToken, sizeof(stAuth.acToken));
        stAuth.uiApp = REST;
        iStatus = REST_Auth(&stAuth);
        if (iStatus != HTTP_STATUS_CODE_200) {
            stResp.iStatus = iStatus;
            goto END_LABEL;
        }

        /* 3.3 handle rest api request */
        pstHandle = REST_GetHandle(stReq.enMethod, (const char *)stReq.acUrl);
        if (NULL == pstHandle) {
            stResp.iStatus = HTTP_STATUS_CODE_404;      /* not found */
            LOG_WRITE(EN_LOG_ERROR, "wrong url");
            goto END_LABEL;
        } else {

            /* b1. get detailed token info of current login account; */
            pstToken = REST_GetCurTokenRecord();

            /* b2. check user access level */
            if (pstToken->uiAccess < pstHandle->uiMinAccess) {
                stResp.iStatus = HTTP_STATUS_CODE_403;
                LOG_WRITE(EN_LOG_ERROR, "access mismatch, token access: %d, min access: %d", pstToken->uiAccess, pstHandle->uiMinAccess);
                goto END_LABEL;
            }

            /* b3. handle */
            pstHandle->pfHandlerFunc(&stReq, &stResp);
        }
    }

END_LABEL:

    /* 4. response by result */
    __REST_Response(pstMgConn, &stReq, &stResp);

    /* 5. free memory */
    __REST_FreeEntryData(&stReq, &stResp);

    return;
}

static int
__NodeIdChangeCb(sr_session_ctx_t *pstSess, uint32_t sub_id, const char *pcModuleName, const char *xpath, sr_event_t enEvent, uint32_t request_id, void *pvPrivateCtx)
{
    sr_change_iter_t *it = NULL;
    int rc = SR_ERR_OK;
    sr_change_oper_t oper;
    sr_val_t *pstOldValue = NULL;
    sr_val_t *pstNewValue = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "device Config Changes, event: %s", ev_to_str(enEvent));

    rc = sr_get_changes_iter(pstSess, OOD_XPATH_NODE_ID, &it);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get changes iter failed for xpath");
        goto cleanup;
    }

    while (SR_ERR_OK == sr_get_change_next(pstSess, it,
                                           &oper, &pstOldValue, &pstNewValue)) {

        /* assume only add or modify, so we always use the new value */
        if (NULL == pstNewValue) {
            if (pstOldValue != NULL) {
                LOG_WRITE(EN_LOG_ERROR, "xpath: %s", pstOldValue->xpath);
            }
            LOG_WRITE(EN_LOG_ERROR, "Get device changes iter failed");
            goto cleanup;
        }

        if (sr_xpath_node_name_eq(pstNewValue->xpath, "node-id")) {
            /* strncpy(m_acNodeId, pstNewValue->data.string_val, sizeof(m_acNodeId) - 1); */
            REST_SetNodeName(pstNewValue->data.string_val);
            LOG_WRITE(EN_LOG_NOTICE, "node id change, the new value is : %s", pstNewValue->data.string_val);
        }

        SR_SAFE_FREE_VAL(pstOldValue);
        SR_SAFE_FREE_VAL(pstNewValue);
    }

    rc = SR_ERR_OK;

cleanup:
    SR_SAFE_FREE_VAL(pstOldValue);
    SR_SAFE_FREE_VAL(pstNewValue);
    sr_free_change_iter(it);
    return rc;
}

static int
__NodeIdChangeSub(void)
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, "__NodeIdChangeSub ++");

    rc = DSC_ChangeSubscribePassive(MODULE_OOD, OOD_XPATH_NODE_ID, __NodeIdChangeCb, NULL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error conf subscribe %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "__NodeIdChangeSub --");
    return rc;
}

static int
__InitProductName()
{
    if (DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OCS)) {
        REST_SetProductName(REST_PRODUCT_NAME_OCS);
    }

    if (DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_AMP)) {
        /* TODO */
        if (DSC_OCP_IsComponentExist(COMPONENT_KEY_AMP_P)) {
            REST_SetProductName(REST_PRODUCT_NAME_OLSP);
        } else if (DSC_OCP_IsComponentExist(COMPONENT_KEY_AMP)) {
            REST_SetProductName(REST_PRODUCT_NAME_HCF);
        }
    }

    return OPLK_OK;
}

int
REST_Init(char *pcListenPort)
{
    int rc = SR_ERR_OK;
    char acCurAppName[32] = {0};
    char acNodeId[128] = {0};

    if (NULL == pcListenPort) {
        LOG_WRITE(EN_LOG_ERROR, "pcListenPort is NULL");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* connect to sysrepo */
    snprintf(acCurAppName, sizeof(acCurAppName), "%s_%s", APP_NAME_RESTAPI, pcListenPort);
    rc = DSC_StartAgentComSess(acCurAppName, CONN_DEF_TIMEOUT);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_connect: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    __InitProductName();

    REST_RegisterApi();

    /* subscribe nodeid */
    rc = __NodeIdChangeSub();
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by subscribe %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* get nodeid */
    rc = DSC_GetNodeId(acNodeId, sizeof(acNodeId) - 1);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetNodeId: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }
    REST_SetNodeName(acNodeId);

    /* load json config files, TBD */
    rc = REST_LoadJsonConfig();

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}

int
REST_Stop(char *pcListenPort)
{

    if (NULL == pcListenPort) {
        LOG_WRITE(EN_LOG_ERROR, "pcListenPort is NULL");
    }

    REST_UnRegisterApi();

    REST_FreeJsonConfig();

    DSC_UnSubscribe();

    /* stop sysrepo session */
    DSC_StopSess();

    return SR_ERR_OK;
}
