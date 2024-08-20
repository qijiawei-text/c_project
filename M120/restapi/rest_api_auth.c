/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_api_auth.c
* Description:   REST APIs of auth module
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2016/12/08  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_api_auth.h"
#include "rest_msft.h"
#include "rest_auth.h"
#include "rest_log.h"
#include "rest_handle.h"

static void
_Debug_PrintUser(USER_ST *pstUser)
{
    CLEAR_NO_USED_WARNING(pstUser);
#if DEBUG_REST_PRINT
    fprintf(stderr, "\r\n acUserName: %s", pstUser->acName);
    fprintf(stderr, "\r\n acUserPwd: %s", pstUser->acPassword);
    fprintf(stderr, "\r\n uiAccessLevel: %d", pstUser->iUserRole);

    fprintf(stderr, "\r\n");
#endif
}

void
API_Login(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int iStatus = HTTP_STATUS_CODE_200;
    LOGIN_REQ_ST stLoginReq;
    LOGIN_RESP_ST stLoginResp;
    USER_ST stUser;

    memset(&stLoginReq, 0, sizeof(stLoginReq));
    memset(&stLoginResp, 0, sizeof(stLoginResp));
    memset(&stUser, 0, sizeof(stUser));

    /* clear last token record */
    REST_ClearCurTokenRecord();

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        iStatus = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseLoginUser((const cJSON *)pstBody, &stUser)) {
        iStatus = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    memcpy(stLoginReq.acUserName, stUser.acName, sizeof(stLoginReq.acUserName) - 1);
    memcpy(stLoginReq.acPassword, stUser.acPassword, sizeof(stLoginReq.acPassword) - 1);
    stLoginReq.uiApp = REST;
    stLoginReq.uiLoginType = EN_LOGIN_TOKEN_AUTH;
    snprintf(stLoginReq.acOldToken, sizeof(stLoginReq.acOldToken), "%s", pstReq->acToken);      /* old token */
    snprintf(stLoginReq.acRemoteAddr, sizeof(stLoginReq.acRemoteAddr), "%s", pstReq->acRemoteAddr);

    if (SR_ERR_OK != REST_AuthLogin(&stLoginReq, &stLoginResp)) {
        /* RESULT_MAX_SESSION_LIMIT -4 */
        iStatus = (-4 == stLoginResp.iResult) ? HTTP_STATUS_CODE_421 : HTTP_STATUS_CODE_400;
        goto END_LABEL;
    }

    /* generate new token */
    snprintf(pstReq->acToken, sizeof(pstReq->acToken), "%s", stLoginResp.acToken);

END_LABEL:
    cJSON_Delete(pstBody);

    /* update username of current token record */
    REST_SetUsernameToCurToken((strlen(stUser.acName) != 0) ? stUser.acName : "Unknown");

    pstResp->iStatus = iStatus;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_Logout(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iStatus = HTTP_STATUS_CODE_200;

    if (SR_ERR_OK != REST_AuthLogout((const char *)pstReq->acToken)) {
        iStatus = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iStatus;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_UserAddOrUpdate(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    USER_ST stUser, stCurUser;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    TOKEN_ST *pstCurToken = NULL;

    memset(&stUser, 0, sizeof(stUser));
    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseUser((const cJSON *)pstBody, &stUser)) {
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    _Debug_PrintUser(&stUser);

    /* current user */
    pstCurToken = REST_GetCurTokenRecord();
    memset(&stCurUser, 0, sizeof(stCurUser));
    stCurUser.iUserRole = pstCurToken->uiAccess;
    memcpy(stCurUser.acName, pstCurToken->acUsername, sizeof(stCurUser.acName) - 1);

    /*set data*/
    rc = DSC_AddUser(&stUser, &stCurUser);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:

    cJSON_Delete(pstBody);
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_UserGetAll(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    /* TODO */
    cJSON *pstBody = NULL;
    cJSON *psJsonObj = NULL;
    /* int rc = SR_ERR_OK; */
    int iResult = HTTP_STATUS_CODE_200;
    int i = 0;
    USER_LIST_ST stInfo = {NULL, 0};

    pstBody = pstResp->pstResponse;

    memset(&stInfo, 0, sizeof(stInfo));
    if (SR_ERR_OK != DSC_GetUsers(&stInfo)) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    /*add user object*/
    for (i = 0; i < stInfo.uiTotal; i++) {
        psJsonObj = cJSON_AddObjectToObject(pstBody, stInfo.pstUser[i].acName);
        cJSON_AddStringToObject(psJsonObj, STR_USER_NAME, stInfo.pstUser[i].acName);
        cJSON_AddStringToObject(psJsonObj, STR_PASSWORD, stInfo.pstUser[i].acPassword);
        cJSON_AddStringToObject(psJsonObj, STR_ACCESS, DSC_ConvertRoleToString(stInfo.pstUser[i].iUserRole));
    }

END_LABEL:
    DEF_SAFE_FREE(stInfo.pstUser);

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
}

void
API_UserDelete(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    USER_ST stUser, stCurUser;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    TOKEN_ST *pstCurToken = NULL;

    /* 20190130, add to check user name valid */
    rc = UTIL_ChkStrSpecChr(pstReq->acUrl + strlen(URL_OMT_USER_ROOT));
    if (OPLK_ERR == rc) {
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /* parse username */
    memset(&stUser, 0, sizeof(stUser));
    snprintf(stUser.acName, sizeof(stUser.acName), "%s", pstReq->acUrl + strlen(URL_OMT_USER_ROOT));

    /* current user */
    pstCurToken = REST_GetCurTokenRecord();
    memset(&stCurUser, 0, sizeof(stCurUser));
    stCurUser.iUserRole = pstCurToken->uiAccess;
    memcpy(stCurUser.acName, pstCurToken->acUsername, sizeof(stCurUser.acName) - 1);

    /* delete user data and access data */
    if (SR_ERR_OK != DSC_DelUser(stUser.acName, &stCurUser)) {
        iResult = HTTP_STATUS_CODE_404;
        goto END_LABEL;
    }

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}
