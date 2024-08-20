
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_aaa.h"
#include "DSC_auth.h"

#define DEFAULT_ADMIN_PASSWD "m4X!izP#"

/*set Default Admin User*/
int
DSC_SetDefaultUserAdmin(void)
{
    int rc = SR_ERR_OK;
    USER_ST stUser;

    LOG_WRITE(EN_LOG_NOTICE, "set default admin user");

    snprintf(stUser.acName, sizeof(stUser.acName), "%s", ADMIN_USER_NAME);
    snprintf(stUser.acPassword, sizeof(stUser.acPassword), "%s", DEFAULT_ADMIN_PASSWD);
    stUser.iUserRole = ADMIN;

    rc = DSC_AddUser(&stUser, &stUser);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetDefaultUserAdmin ok");

    return SR_ERR_OK;

ERR_LABEL:

    return -1;

}


int
DSC_Login(LOGIN_REQ_ST *pstReqLogin, LOGIN_RESP_ST *pstRespLogin)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t input_cnt = 0, output_cnt = 0;
    sr_session_ctx_t *session = NULL;
    sr_conn_ctx_t *login_conn = NULL;
    sr_session_ctx_t *login_sess = NULL;
    LOGIN_RESP_ST stRespLogin;
    ANONYMOUS_ST astAnonymous[] = {{XPATH_RPC_LOGIN_STATUS, SR_INT32_T, (char *)&(stRespLogin.iResult), sizeof(stRespLogin.iResult)},
                                   {XPATH_RPC_LOGIN_USERROLE, SR_UINT32_T, (char *)&(stRespLogin.uiAccess), sizeof(stRespLogin.uiAccess)},
                                   {XPATH_RPC_LOGIN_AUTH_CHANNEL, SR_UINT32_T, (char *)&(stRespLogin.uiAuthChannel), sizeof(stRespLogin.uiAuthChannel)},
                                   {XPATH_RPC_LOGIN_TOKEN, SR_STRING_T, stRespLogin.acToken, sizeof(stRespLogin.acToken)}};

    if (NULL == pstReqLogin || NULL == pstRespLogin ||
        0 == strlen(pstReqLogin->acUserName) || 0 == strlen(pstReqLogin->acPassword)) {
        return SR_ERR_INVAL_ARG;
    }

    /* must be running session */
    session = DSC_GetRunningSession();

    /*not get session for CMD before logining*/
    if (CMD == pstReqLogin->uiApp && NULL == session) {
        rc = sr_connect(SR_CONN_DEFAULT, &login_conn);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

        rc = sr_session_start(login_conn, SR_DS_RUNNING, &login_sess);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

        session = login_sess;
    }

    memset(&stRespLogin, 0, sizeof(stRespLogin));

    /* allocate input values */
    input_cnt = 6;
    rc = sr_new_values(input_cnt, &input);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set 'input/name' leaf */
    rc = set_string_val_by_xpath(&input[0], XPATH_RPC_LOGIN_NAME, pstReqLogin->acUserName);

    /* set 'input/password' leaf */
    rc |= set_string_val_by_xpath(&input[1], XPATH_RPC_LOGIN_PASSWORD, pstReqLogin->acPassword);

    /* set 'input/application' leaf */
    rc |= set_uint32_val_by_xpath(&input[2], XPATH_RPC_LOGIN_APPLICATION, pstReqLogin->uiApp);

    /* set 'input/login-type' leaf */
    rc |= set_uint32_val_by_xpath(&input[3], XPATH_RPC_LOGIN_TYPE, pstReqLogin->uiLoginType);

    rc |= set_string_val_by_xpath(&input[4], XPATH_RPC_LOGIN_REMOTE_ADDRESS, pstReqLogin->acRemoteAddr);

    rc |= set_string_val_by_xpath(&input[5], XPATH_RPC_LOGIN_TOKEN, pstReqLogin->acOldToken);

    /* TBD: set 'input/session-id' leaf */
    /* rc |= set_string_val_by_xpath(&input[3], XPATH_RPC_LOGIN_SESSION , ""); */
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* execute RPC */
    rc = DSC_RpcSend(session, RPC_XPATH_LOGIN, input, input_cnt, &output, &output_cnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "rpc (%s) send failed. %s", RPC_XPATH_LOGIN, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_ParseValuesByXpath(output, output_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "parse output failed (%d).", rc);
        goto ERR_LABEL;
    }

    memcpy(pstRespLogin, &stRespLogin, sizeof(stRespLogin));

    if (stRespLogin.iResult != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s, iResult: %d", __func__, stRespLogin.iResult);
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    DSC_UserNacmAdd(pstReqLogin->acUserName, stRespLogin.uiAccess);

ERR_LABEL:
    /* don't forget to de-allocate the output values */
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);

    if (login_sess != NULL) {
        sr_session_stop(login_sess);
        login_sess = NULL;
    }

    if (login_conn != NULL) {
        sr_disconnect(login_conn);
        login_conn = NULL;
    }

    return rc;

}


int
DSC_Logout(LOGOUT_ST *pstLogout)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t input_cnt = 0, output_cnt = 0;
    sr_session_ctx_t *session = NULL;
    int iResult = OPLK_OK;

    ANONYMOUS_ST astAnonymous[] = {{XPATH_RPC_LOGOUT_STATUS, SR_INT32_T, (char *)&iResult, sizeof(iResult)}};

    if (NULL == pstLogout || 0 == strlen(pstLogout->acToken)) {
        return SR_ERR_INVAL_ARG;
    }

    /* must be running session */
    session = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 4;
    rc = sr_new_values(input_cnt, &input);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set 'input/name' leaf */
    rc = set_string_val_by_xpath(&input[0], XPATH_RPC_LOGOUT_NAME, pstLogout->acUserName);

    /* set 'input/application' leaf */
    rc |= set_uint32_val_by_xpath(&input[1], XPATH_RPC_LOGOUT_APPLICATION, pstLogout->uiApp);

    rc |= set_uint32_val_by_xpath(&input[2], XPATH_RPC_LOGOUT_LOGIN_TYPE, pstLogout->uiAuthLoginType);
    rc |= set_string_val_by_xpath(&input[3], XPATH_RPC_LOGOUT_TOKEN, pstLogout->acToken);

    /* TBD: set 'input/session-id' leaf */
    /* rc |= set_string_val_by_xpath(&input[3], XPATH_RPC_LOGIN_SESSION , ""); */
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* execute RPC */
    rc = DSC_RpcSend(session, RPC_XPATH_LOGOUT, input, input_cnt, &output, &output_cnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "rpc (%s) send failed. %s", RPC_XPATH_LOGOUT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_ParseValuesByXpath(output, output_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "parse output failed (%d).", rc);
        goto ERR_LABEL;
    }

    if (iResult != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s, iResult: %d", __func__, iResult);
    }

ERR_LABEL:
    /* don't forget to de-allocate the output values */
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);

    return rc;

}

int
DSC_Auth(AUTH_ST *pstAuth)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t input_cnt = 0, output_cnt = 0;
    sr_session_ctx_t *session = NULL;
    int iResult = OPLK_OK;

    ANONYMOUS_ST astAnonymous[] = {{XPATH_RPC_AUTH_STATUS, SR_INT32_T, (char *)&iResult, sizeof(iResult)}};

    if (NULL == pstAuth || 0 == strlen(pstAuth->acToken)) {
        return SR_ERR_INVAL_ARG;
    }

    session = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* input node*/
    rc = set_string_val_by_xpath(&input[0], XPATH_RPC_AUTH_TOKEN, pstAuth->acToken);

    rc |= set_uint32_val_by_xpath(&input[1], XPATH_RPC_AUTH_APPLICATION, pstAuth->uiApp);

    rc |= set_string_val_by_xpath(&input[2], XPATH_RPC_AUTH_OPER_CMD, pstAuth->acOperCmd);

    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* execute RPC */
    rc = DSC_RpcSend(session, RPC_XPATH_AUTH, input, input_cnt, &output, &output_cnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "rpc (%s) send failed. %s", RPC_XPATH_AUTH, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_ParseValuesByXpath(output, output_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "parse output failed (%d).", rc);
        goto ERR_LABEL;
    }

    pstAuth->iStatus = iResult;

    if (iResult != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s, iResult: %d", __func__, iResult);
    }

ERR_LABEL:
    /* don't forget to de-allocate the output values */
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);

    return rc;
}

int
DSC_LoginWithResetSession(LOGIN_REQ_ST *pstReqLogin, LOGIN_RESP_ST *pstRespLogin)
{
    int rc = SR_ERR_OK;

    rc = DSC_Login(pstReqLogin, pstRespLogin);

    return rc;
}

int
DSC_AuthWithResetSession(AUTH_ST *pstAuth)
{
    int rc = SR_ERR_OK;

    rc = DSC_Auth(pstAuth);

    return rc;
}

int
DSC_GetCurTokenRecord(TOKEN_ST *pstToken)
{
    int i = 0;
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    TOKEN_ST *pstTokenLists = NULL;

    if (NULL == pstToken) {
        LOG_WRITE(EN_LOG_ERROR, "input is null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetTokenLists(&pstTokenLists, &nCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetTokenLists error");
        goto ERR_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        if (0 == strncmp(pstToken->acToken, pstTokenLists[i].acToken, sizeof(pstToken->acToken) - 1)) {
            break;
        }
    }

    if (i == nCnt) {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "Not found token");

        goto ERR_LABEL;
    }

    memcpy(pstToken, &(pstTokenLists[i]), sizeof(TOKEN_ST));

ERR_LABEL:
    DEF_SAFE_FREE(pstTokenLists);
    return rc;
}


int
DSC_GetTokenLists(TOKEN_ST **ppstToken, size_t *pnCnt)
{
    int i = 0, j = 0;
    int rc = SR_ERR_OK;
    sr_data_t *pstSubtrees = NULL;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    size_t nLeafCnt = 0, nCnt = 0;;
    sr_node_t *apstLeaf[20];
    TOKEN_ST *pstToken = NULL;
    sr_node_t *pstLeaf = NULL;
    struct ly_set *pstNodeSet = NULL;

    if (NULL == pstSess || NULL == pnCnt) {
        LOG_WRITE(EN_LOG_ERROR, "sess or input is null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(pstSess, XPATH_AUTH_TOKENS, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        nCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees %s error: %s", XPATH_AUTH_TOKENS, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (NULL != pstSubtrees) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_AUTH_TOKENS, &pstNodeSet);
        if (LY_SUCCESS == rc) {
            nCnt = pstNodeSet->count;
        }
    } else {
        nCnt = 0;
    }

    if (nCnt != 0) {
        pstToken = (TOKEN_ST *)malloc(nCnt * sizeof(TOKEN_ST));
        if (NULL == pstToken) {
            LOG_WRITE(EN_LOG_ERROR, "malloc pstToken error");

            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        memset(pstToken, 0, nCnt * sizeof(TOKEN_ST));
        for (i = 0; i < nCnt; i++) {
            nLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < nLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(pstLeaf->schema->name, NODE_TOKEN)) {
                    strncpy(pstToken[i].acToken, lyd_get_value(pstLeaf), sizeof(pstToken[i].acToken) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_NAME)) {
                    strncpy(pstToken[i].acUsername, lyd_get_value(pstLeaf), sizeof(pstToken[i].acUsername) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_REMOTE_ADDRESS)) {
                    strncpy(pstToken[i].acRemoteAddr, lyd_get_value(pstLeaf), sizeof(pstToken[i].acRemoteAddr) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_EXPIRE_TIME)) {
                    pstToken[i].uiExpiration = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_LAST_LOGIN_TIME)) {
                    pstToken[i].uiLastLoginTime = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_LAST_OPT_TIME)) {
                    pstToken[i].uiLastOperationTime = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_APPLICATION)) {
                    pstToken[i].uiApp = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_LOGIN_TYPE)) {
                    pstToken[i].uiAuthLoginType = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_USER_ROLE)) {
                    pstToken[i].uiAccess = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_PRIV_LV)) {
                    pstToken[i].uiPriv = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, NODE_AUTH_CHANNEL)) {
                    pstToken[i].uiAuthChannel = ((sr_node_term_t *)pstLeaf)->value.uint32;
                }
            }
        }
    }

    *pnCnt = nCnt;
    *ppstToken = pstToken;

ERR_LABEL:

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }
    return rc;
}


int
DSC_DeteleTokensByUsername(const char *pcUsername)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t input_cnt = 0, output_cnt = 0;
    sr_session_ctx_t *session = NULL;
    int iResult = OPLK_OK;

    ANONYMOUS_ST astAnonymous[] = {{XPATH_RPC_DELETE_TOKES_STATUS, SR_INT32_T, (char *)&iResult, sizeof(iResult)}};

    if (NULL == pcUsername) {
        return SR_ERR_INVAL_ARG;
    }

    /* must be running session */
    session = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set 'input/name' leaf */
    rc = set_string_val_by_xpath(&input[0], XPATH_RPC_DELETE_TOKES_USERNAME, pcUsername);

    /* TBD: set 'input/session-id' leaf */
    /* rc |= set_string_val_by_xpath(&input[3], XPATH_RPC_LOGIN_SESSION , ""); */
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set input paras failed. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* execute RPC */
    rc = DSC_RpcSend(session, RPC_XPATH_DELETE_TOKENS_BY_USERNAME, input, input_cnt, &output, &output_cnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "rpc (%s) send failed. %s", RPC_XPATH_DELETE_TOKENS_BY_USERNAME, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_ParseValuesByXpath(output, output_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "parse output failed (%d).", rc);
        goto ERR_LABEL;
    }

    if (iResult != OPLK_OK) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "%s, iResult: %d", __func__, iResult);
        goto ERR_LABEL;
    }

ERR_LABEL:
    /* don't forget to de-allocate the output values */
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);

    return rc;

}

int
DSC_SetSecuritySessionMaxCount(uint32_t uiMaxCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(pstSess, XPATH_NODE_SESSION_MAX_COUNT, uiMaxCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_uint32_item: %s, vaule: %u, error: %s", XPATH_NODE_SESSION_MAX_COUNT, uiMaxCount, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s DSC_commit error %s", __func__, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}
int
DSC_SetSecuritySessionTimeout(uint32_t uiTimeout)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(pstSess, XPATH_NODE_SESSION_TIMEOUT, uiTimeout);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_uint32_item: %s, vaule: %u, error: %s", XPATH_NODE_SESSION_TIMEOUT, uiTimeout, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s DSC_commit error %s", __func__, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetSecurityPerCmdAuthorization(bool bEnable)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_bool_item(pstSess, XPATH_NODE_COMMAND_AUTHORIZATION, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_bool_item: %s, vaule: %d, error: %s", XPATH_NODE_COMMAND_AUTHORIZATION, bEnable, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s DSC_commit error %s", __func__, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_DeletePerCmdAuthorization()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = sr_delete_item(pstSess, XPATH_NODE_COMMAND_AUTHORIZATION, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s DSC_commit error %s", __func__, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetSecurityConfig(AUTH_SECURITY_ST *pstSecurityConfig)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess || NULL == pstSecurityConfig) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = get_uint32_item(pstSess, XPATH_NODE_SESSION_MAX_COUNT, &(pstSecurityConfig->uiSessionMaxCount));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_uint32_item: %s %s", XPATH_NODE_SESSION_MAX_COUNT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = get_uint32_item(pstSess, XPATH_NODE_SESSION_TIMEOUT, &(pstSecurityConfig->uiSessionTimeout));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_uint32_item: %s %s", XPATH_NODE_SESSION_TIMEOUT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = get_bool_item(pstSess, XPATH_NODE_COMMAND_AUTHORIZATION, &(pstSecurityConfig->bPerCmdAuthorization));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_bool_item: %s %s", XPATH_NODE_COMMAND_AUTHORIZATION, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}


int
DSC_CheckSslCerts(char *pcCertFile, char *pcKeyFile)
{
    char acCmdLine[256];
    char acRsp[1024];
    const char *pcTmpCert = "/tmp/certpubkey.txt";
    const char *pcTmpKey = "/tmp/keypubkey.txt";

    if (NULL == pcCertFile || NULL == pcKeyFile) {
        goto ERR_LABEL;
    }

    /* checkSslCert.exp  crt <crt_file> */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s crt %s >/dev/null 2>&1; echo $?", SSL_CHECK_SCRIPT, pcCertFile);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    /* checkSslCert.exp  key <key_file> */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s key %s >/dev/null 2>&1; echo $?", SSL_CHECK_SCRIPT, pcKeyFile);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    /* openssl x509 -pubkey -noout -in server.pem */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s x509 -pubkey -noout -in %s > %s 2>/dev/null", SSL_OPENSSL, pcCertFile, pcTmpCert);
    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    /* openssl rsa -pubout -in server.key */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s rsa -pubout -in %s > %s 2>/dev/null", SSL_OPENSSL, pcKeyFile, pcTmpKey);
    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "diff %s %s; echo $?", pcTmpCert, pcTmpKey);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    return 0;
ERR_LABEL:
    return -1;
}

int
DSC_CheckGnmiCerts(char *pcCertFile, char *pcKeyFile, char *pcCaFile)
{
    char acCmdLine[256];
    char acRsp[1024];
    const char *pcTmpCert = "/tmp/certpubkey.txt";
    const char *pcTmpKey = "/tmp/keypubkey.txt";

    if (NULL == pcCertFile || NULL == pcKeyFile) {
        goto ERR_LABEL;
    }

    /* checkSslCert.exp  crt <crt_file> */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s crt %s >/dev/null 2>&1; echo $?", SSL_CHECK_SCRIPT, pcCertFile);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    /* checkSslCert.exp  key <key_file> */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s key %s >/dev/null 2>&1; echo $?", SSL_CHECK_SCRIPT, pcKeyFile);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    /* checkSslCert.exp  crt <ca_file> */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s crt %s >/dev/null 2>&1; echo $?", SSL_CHECK_SCRIPT, pcCaFile);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    /* openssl x509 -pubkey -noout -in server.pem */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s x509 -pubkey -noout -in %s > %s 2>/dev/null", SSL_OPENSSL, pcCertFile, pcTmpCert);
    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    /* openssl rsa -pubout -in server.key */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s rsa -pubout -in %s > %s 2>/dev/null", SSL_OPENSSL, pcKeyFile, pcTmpKey);
    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "diff %s %s; echo $?", pcTmpCert, pcTmpKey);
    memset(acRsp, 0, sizeof(acRsp));
    if (0 != UTIL_ExceCmdUnresp(acCmdLine, acRsp, sizeof(acRsp), 0)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    } else if (0 != atoi(acRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed code %s", acCmdLine, acRsp);
        goto ERR_LABEL;
    }

    return 0;
ERR_LABEL:
    return -1;
}

#if 0
int
DSC_LoadSslCerts(char *pcCertFile, char *pcKeyFile)
{
    char acCmdLine[256];

    if (NULL == pcCertFile || NULL == pcKeyFile) {
        goto ERR_LABEL;
    }

    if (0 != DSC_CheckSslCerts(pcCertFile, pcKeyFile)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CheckSslCerts failed");
        goto ERR_LABEL;
    }

    /* copy to work dir */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "cp -f %s %s", pcCertFile, SSL_CERT_WORK_CERT_FILE);
    if (0 != system(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "cp -f %s %s", pcKeyFile, SSL_CERT_WORK_KEY_FILE);
    if (0 != system(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    return 0;
ERR_LABEL:
    return -1;
}

int
DSC_RestoreSslCerts()
{
    char acCmdLine[256];

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "rm -f %s", SSL_CERT_SAVE_CERT_FILE);
    if (0 != system(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "rm -f %s", SSL_CERT_SAVE_KEY_FILE);
    if (0 != system(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }


    return 0;
ERR_LABEL:
    return -1;
}
#endif
