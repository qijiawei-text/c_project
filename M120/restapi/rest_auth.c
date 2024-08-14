/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_auth.c
* Description:   user authentication.
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/05/26  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_msft.h"
#include "rest_auth.h"

/* The token record of current user */
static TOKEN_ST m_stToken;

int
REST_ParseLoginUser(const cJSON *pstJson, USER_ST *pstUser)
{
    USER_ST stUser;
    JSON_OBJ_ST astJsonObj[] = {{STR_USER_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stUser.acName), sizeof(stUser.acName)},
                                {STR_PASSWORD, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stUser.acPassword), sizeof(stUser.acPassword)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stUser, 0, sizeof(stUser));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstUser, &stUser, sizeof(stUser));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

/* support local user or tacacs+ user login */
int
REST_AuthLogin(LOGIN_REQ_ST *pstLoginReq, LOGIN_RESP_ST *pstLoginResp)
{
    int rc = SR_ERR_OK;

    if (NULL == pstLoginReq || NULL == pstLoginResp) {
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_LoginWithResetSession(pstLoginReq, pstLoginResp);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "login failed, rc: %d", rc);
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

int
REST_AuthLogout(const char *pcToken)
{
    int rc = SR_ERR_OK;
    LOGOUT_ST stLogout;
    TOKEN_ST *pstCurToken = REST_GetCurTokenRecord();

    memset(&stLogout, 0, sizeof(stLogout));
    /* snprintf(stLogout.acUserName, sizeof(stLogout.acUserName), "%s", pstCurToken->acUsername); */
    memcpy(stLogout.acUserName, pstCurToken->acUsername, sizeof(stLogout.acUserName) - 1);
    snprintf(stLogout.acToken, sizeof(stLogout.acToken), "%s", pcToken);
    stLogout.uiApp = REST;

    rc = DSC_Logout(&stLogout);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "logout failed. username: %s, token: %s, rc: %d", pstCurToken->acUsername, pstCurToken->acToken, rc);
    }

    return rc;
}

int
REST_ParseUser(const cJSON *pstJson, USER_ST *pstUser)
{
    USER_ST stUser;
    char acBuf[32] = {0};
    JSON_OBJ_ST astJsonObj[] = {{STR_USER_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stUser.acName), sizeof(stUser.acName)},
                                {STR_PASSWORD, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stUser.acPassword), sizeof(stUser.acPassword)},
                                {STR_ACCESS, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(acBuf), sizeof(acBuf)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stUser, 0, sizeof(stUser));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != DSC_ConvertRoleToInt(acBuf, &stUser.iUserRole)) {
        goto ERR_LABEL;
    }

    memcpy(pstUser, &stUser, sizeof(stUser));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_AuthGetLoginTokenRecord(const char *pcToken)
{
    int rc = SR_ERR_OK;
    TOKEN_ST stToken;

    if (NULL == pcToken) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&m_stToken, 0, sizeof(m_stToken));

    snprintf(stToken.acToken, sizeof(stToken.acToken), "%s", pcToken);
    rc = DSC_GetCurTokenRecord(&stToken);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting logined token (%s) record.", pcToken);
        goto END_LABEL;
    }

#if 0
    LOG_WRITE(EN_LOG_NOTICE, "%s", pcToken);
    LOG_WRITE(EN_LOG_NOTICE, "%s", stToken.acToken);
    LOG_WRITE(EN_LOG_NOTICE, "%s %d", stToken.acUsername, stToken.uiAccess);
#endif

    memcpy(&m_stToken, &stToken, sizeof(stToken));

END_LABEL:

    return rc;
}

TOKEN_ST *
REST_GetCurTokenRecord()
{

    return &m_stToken;
}

void
REST_ClearCurTokenRecord()
{
    memset(&m_stToken, 0, sizeof(m_stToken));
}

/* only used for login REST API */
void
REST_SetUsernameToCurToken(char *pcUsername)
{
    if (NULL == pcUsername) {
        return;
    }

    memcpy(m_stToken.acUsername, pcUsername, sizeof(m_stToken.acUsername) - 1);
}

int
REST_Auth(AUTH_ST *pstAuth)
{
    int iStatus = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    if ((NULL == pstAuth) || (0 == strlen(pstAuth->acToken))) {
        /* LOG_WRITE(EN_LOG_ERROR, "Token is empty"); */
        return HTTP_STATUS_CODE_401;
    }

    rc = DSC_AuthWithResetSession(pstAuth);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Auth, rc: %d, iStatus: %d", rc, pstAuth->iStatus);
        /* TODO: */
        if (-4 == pstAuth->iStatus) {
            iStatus = HTTP_STATUS_CODE_408;
        } else {
            iStatus = HTTP_STATUS_CODE_401;
        }
        goto END_LABEL;
    }

    rc = REST_AuthGetLoginTokenRecord((const char *)pstAuth->acToken);
    if (rc != SR_ERR_OK) {
        iStatus = HTTP_STATUS_CODE_403;
        LOG_WRITE(EN_LOG_ERROR, "Error by getting logined token record.");
        goto END_LABEL;
    }

    iStatus = HTTP_STATUS_CODE_200;

END_LABEL:

    return iStatus;
}


