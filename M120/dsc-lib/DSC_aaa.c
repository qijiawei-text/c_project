
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_aaa.h"
#include <crypt.h>
#include <ctype.h>
#include "DSC_platform.h"

static sr_node_t *
DSC_GetTreeChild(sr_node_t *pstTree, char *pcName, uint32_t ulMaxCnt)
{
    sr_node_inner_t *pstChildNode = NULL;
    sr_node_inner_t *pstInnerTree = NULL;
    uint32_t ulCnt = 0;

    /* Check input parameter. */
    if ((NULL == pstTree) || (NULL == pcName)) {
        LOG_WRITE(EN_LOG_ERROR, "pstTree: %p, pcName: %p null", pstTree, pcName);
        return NULL;
    }

    /* trans node to inner node for list use */
    pstInnerTree = (sr_node_inner_t *)pstTree;
    pstChildNode = (sr_node_inner_t *)(pstInnerTree->child);

    /* find the specified child */
    while (pstChildNode) {
        if (0 == strcmp(pstChildNode->schema->name, pcName)) {
            if (ulCnt >= ulMaxCnt) {
                /* max count, return. */
                return (sr_node_t *)pstChildNode;
            }

            ulCnt++;
        }

        pstChildNode = (sr_node_inner_t *)pstChildNode->next;
    }

    return NULL;

}

static uint32_t
DSC_GetTreeChildListNum(sr_node_t *pstTree, char *pcName)
{
    sr_node_inner_t *pstChildNode = NULL;
    sr_node_inner_t *pstInnerTree = NULL;
    uint32_t ulCnt = 0;

    /* Check input parameter. */
    if ((NULL == pstTree) || (NULL == pcName)) {
        LOG_WRITE(EN_LOG_ERROR, "pstTree: %p, pcName: %p null", pstTree, pcName);
        return 0;
    }

    /* trans node to inner node for list use */
    pstInnerTree = (sr_node_inner_t *)pstTree;
    pstChildNode = (sr_node_inner_t *)(pstInnerTree->child);

    /* find out all leaves */
    while (pstChildNode) {
        if (0 == strcmp(pstChildNode->schema->name, pcName)) {
            ulCnt++;
        }

        pstChildNode = (sr_node_inner_t *)pstChildNode->next;
    }

    return ulCnt;

}

signed int
DSC_AddRadiusServer(RADIUS_SERVER_ST *pstServer)
{

    signed int rc = SR_ERR_OK;
    char xpath[512] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstServer) {
        LOG_WRITE(EN_LOG_ERROR, "pstServer is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. add or modify a list */
    /* server/udp/address */
    sprintf(xpath, XPATH_RADIUS_SERVER_UDP_ADDR, pstServer->acName);
    rc = set_string_item(pstSess, xpath, pstServer->acIp, sizeof(pstServer->acIp));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* server/udp/authentication-port */
    sprintf(xpath, XPATH_RADIUS_SERVER_UDP_PORT, pstServer->acName);
    rc = set_uint16_item(pstSess, xpath, (uint16_t)pstServer->uiPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* server/udp/shared-secret */
    sprintf(xpath, XPATH_RADIUS_SERVER_UDP_KEY, pstServer->acName);
    rc = set_string_item(pstSess, xpath, pstServer->acKey, sizeof(pstServer->acKey));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp key: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add radius server DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "add radius server success %d", rc);
    }

    return rc;
}

signed int
DSC_SetRadiusPara(RADIUS_PARA_ST *pstPara)
{
    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstPara) {
        LOG_WRITE(EN_LOG_ERROR, "pstPara is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. modify some elements */
    /* options/timeout */
    rc = set_uint8_item(pstSess, XPATH_RADIUS_OPTIONS_TIMEOUT, (uint8_t)pstPara->uiTimeout);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-options timeout: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* options/attempts */
    rc = set_uint8_item(pstSess, XPATH_RADIUS_OPTIONS_ATTEMPTS, (uint8_t)pstPara->uiRetryTimes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-options attempts: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD Radius para DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "set radius parameters success %d", rc);
    }

    return rc;
}

signed int
DSC_DelRadiusServer(char *pcServerName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[512] = {0};

    if (NULL == pcServerName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, XPATH_RADIUS_SERVER, pcServerName);

    return DSC_DelNode(sess, xpath);
}

signed int
DSC_GetRadiusServers(RADIUS_SERVER_LIST_ST *pstServers)
{
    sr_data_t *pstSubtrees = NULL;
    size_t uiServerCnt = 0;
    signed int rc = SR_ERR_OK;
    unsigned int i, j;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    unsigned int uiLeafCnt;
    sr_node_t *apstLeaf[20];
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all radius servers */

    rc = get_subtrees(pstSess, XPATH_RADIUS_SERVER_LIST "//*", 0, SR_OPER_NO_STATE, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiServerCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_RADIUS_SERVER_LIST, &pstNodeSet);
    if (LY_SUCCESS == rc) {
        uiServerCnt = pstNodeSet->count;
    }

    /* if there is one server at least */
    if (uiServerCnt != 0) {
        pstServers->pstRadiusServer = (RADIUS_SERVER_ST *)malloc(uiServerCnt * sizeof(RADIUS_SERVER_ST));

        if (NULL == pstServers->pstRadiusServer) {
            LOG_WRITE(EN_LOG_ERROR, "pstServers->pstRadiusServer is null");

            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        for (i = 0; i < uiServerCnt; i++) {
            uiLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < uiLeafCnt; j++) {
                pstLeaf = apstLeaf[j];
                if (0 == strcmp(apstLeaf[j]->schema->name, "name")) {
                    pstServers->pstRadiusServer[i].acName[sizeof(pstServers->pstRadiusServer[i].acName) - 1] = 0;
                    strncpy(pstServers->pstRadiusServer[i].acName, lyd_get_value(pstLeaf), sizeof(pstServers->pstRadiusServer[i].acName) - 1);
                } else if (0 == strcmp(apstLeaf[j]->schema->name, "address")) {
                    pstServers->pstRadiusServer[i].acIp[sizeof(pstServers->pstRadiusServer[i].acIp) - 1] = 0;
                    strncpy(pstServers->pstRadiusServer[i].acIp, lyd_get_value(pstLeaf), sizeof(pstServers->pstRadiusServer[i].acIp) - 1);
                } else if (0 == strcmp(apstLeaf[j]->schema->name, "authentication-port")) {
                    pstServers->pstRadiusServer[i].uiPort = (uint16_t)atoi(lyd_get_value(pstLeaf));
                } else if (0 == strcmp(apstLeaf[j]->schema->name, "shared-secret")) {
                    pstServers->pstRadiusServer[i].acKey[sizeof(pstServers->pstRadiusServer[i].acKey) - 1] = 0;
                    strncpy(pstServers->pstRadiusServer[i].acKey, lyd_get_value(pstLeaf), sizeof(pstServers->pstRadiusServer[i].acKey) - 1);
                }
            }
        }
    }

    pstServers->uiTotal = uiServerCnt;

    /* 3. get radius parameters */

    /* options/timeout */
    rc = get_uint8_item(pstSess, XPATH_RADIUS_OPTIONS_TIMEOUT, (uint8_t *)&pstServers->stRadiusPara.uiTimeout);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_uint8_item error-options timeout: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* options/attempts */
    rc = get_uint8_item(pstSess, XPATH_RADIUS_OPTIONS_ATTEMPTS, (uint8_t *)&pstServers->stRadiusPara.uiRetryTimes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_uint8_item error-options timeout: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

ERR_LABEL:

    if (pstNodeSet != NULL) {
        ly_set_free(pstNodeSet, NULL);
        pstNodeSet = NULL;
    }

    if (pstSubtrees != NULL) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (rc != SR_ERR_OK) {
        if (pstServers->pstRadiusServer) {
            free(pstServers->pstRadiusServer);
            pstServers->pstRadiusServer = NULL;
        }
    }

    return rc;
}

int
DSC_CheckUser(USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    size_t nLen = 0;

    if (NULL == pstUser || 0 == strlen(pstUser->acName) || 0 == strlen(pstUser->acPassword)) {
        LOG_WRITE(EN_LOG_ERROR, "User is null");
        return SR_ERR_INVAL_ARG;
    }

    /*check user name */
    if (OPLK_ERR == UTIL_ChkStrSpecChr(pstUser->acName)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /*check password */
    if (OPLK_ERR == UTIL_ChkStrSpecChr(pstUser->acPassword)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /* the length of username is checked by yang model */

    /*check the length of clear-text password */
    nLen = strlen(pstUser->acPassword);
    if (nLen < 4 || nLen > 63) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "the length of password is %ld, and not in [4, 63]", nLen);
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

static int
__AddUser(USER_ST *pstUser)
{
    signed int rc = SR_ERR_OK;
    char xpath[512] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2.  add or modify a list */

    /* user/password */
    snprintf(xpath, sizeof(xpath), XPATH_AUTH_USER_PASSWORD, pstUser->acName);
    rc = set_string_item(pstSess, xpath, pstUser->acPassword, sizeof(pstUser->acPassword));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-password: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    snprintf(xpath, sizeof(xpath), XPATH_AUTH_USER_ROLE, pstUser->acName);
    rc = set_uint32_item(pstSess, xpath, (uint32_t)(pstUser->iUserRole));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-role: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD user DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

static int
__User_UpdateOwnPasswordForQueryUser(USER_ST *pstUser)
{
    sr_conn_ctx_t *user_conn = NULL;
    sr_session_ctx_t *user_sess = NULL;
    int rc = SR_ERR_OK;
    sr_val_t _v = {0};
    char xpath[512] = {0};

    rc = sr_connect(SR_CONN_DEFAULT, &user_conn);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    rc = sr_session_start(user_conn, SR_DS_RUNNING, &user_sess);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    snprintf(xpath, sizeof(xpath), XPATH_AUTH_USER_PASSWORD, pstUser->acName);

    _v.type = SR_STRING_T;
    _v.data.string_val = pstUser->acPassword;

    rc = sr_set_item(user_sess, xpath, &_v, SR_EDIT_NON_RECURSIVE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-password: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = sr_apply_changes(user_sess, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_apply_changes: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    if (user_sess != NULL) {
        sr_session_stop(user_sess);
        user_sess = NULL;
    }

    if (user_conn != NULL) {
        sr_disconnect(user_conn);
        user_conn = NULL;
    }
    return rc;

}

int
DSC_AddUser(USER_ST *pstUser, USER_ST *pstCurUser)
{
    int rc = SR_ERR_OK;
    int i = 0;
    USER_LIST_ST stInfo = {NULL, 0};
    char acGroupName[NACM_NAME_MAX_LEN] = {0};
    char acPassword[USER_PASSWORD_MAX_LEN] = {0};
    char acUserLog[DEF_BUF_SIZE_256] = {0};

    if (NULL == pstUser || NULL == pstCurUser) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto clean_up;
    }

    /* check username and password */
    if (SR_ERR_OK != DSC_CheckUser(pstUser)) {
        LOG_WRITE(EN_LOG_ERROR, "invalid username or password");
        rc = SR_ERR_INVAL_ARG;
        /* ? what error */
        goto clean_up;
    }

    /* encrypt password */
    strcpy(acPassword, pstUser->acPassword);
    memset(pstUser->acPassword, 0, sizeof(pstUser->acPassword));
    if (SR_ERR_OK != DSC_CryptHash(EN_HASH_SHA512, HASH_COMMON_SALT, acPassword, pstUser->acPassword, sizeof(pstUser->acPassword))) {
        LOG_WRITE(EN_LOG_ERROR, "encrypt password failed");
        goto clean_up;
    }

    if (QUERY == pstCurUser->iUserRole) {

        /* only update self password */
        if ((pstCurUser->iUserRole != pstUser->iUserRole) ||
            (0 != strcmp(pstCurUser->acName, pstUser->acName))) {
            LOG_WRITE(EN_LOG_ERROR, "Only update self password");
            rc = SR_ERR_INVAL_ARG;
            goto clean_up;
        }

        /* query user may change own password */
        rc = DSC_GetUsers(&stInfo);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetUsers failed");
            goto clean_up;
        }

        /* user info */
        for (i = 0; i < stInfo.uiTotal; i++) {
            if (0 == strcmp(stInfo.pstUser[i].acName, pstCurUser->acName)) {
                break;
            }
        }

        if (i >= stInfo.uiTotal) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetUsers not found user");
            rc = SR_ERR_INVAL_ARG;
            goto clean_up;
        }

        rc = __User_UpdateOwnPasswordForQueryUser(pstUser);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "__User_UpdateOwnPasswordForQueryUser failed");
            goto clean_up;
        }

        snprintf(acUserLog, sizeof(acUserLog), "%s,%s", pstUser->acName, NACM_GROUP_READ_EXEC);
        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_SET_USER, acUserLog);
    } else {

        /* Only admin user can update admin passowrd */
        if (0 == strcmp(ADMIN_USER_NAME, pstUser->acName)) {
            if ((0 != strcmp(ADMIN_USER_NAME, pstCurUser->acName)) ||
                (ADMIN != pstUser->iUserRole) ||
                (ADMIN != pstCurUser->iUserRole)) {
                LOG_WRITE(EN_LOG_ERROR, "Only admin user can update admin passowrd");
                rc = SR_ERR_INVAL_ARG;
                goto clean_up;
            }
        }

        /* can not modify own role */
        if ((0 == strcmp(pstUser->acName, pstCurUser->acName)) && (pstUser->iUserRole != pstCurUser->iUserRole)) {
            LOG_WRITE(EN_LOG_ERROR, "can not modify own role");
            rc = SR_ERR_INVAL_ARG;
            goto clean_up;
        }

        /* cant not add/update high role user*/
        if (pstCurUser->iUserRole < pstUser->iUserRole) {
            LOG_WRITE(EN_LOG_ERROR, "can not add/update higher role user");
            rc = SR_ERR_INVAL_ARG;
            goto clean_up;
        }

        rc = __AddUser(pstUser);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "__AddUser failed");
            goto clean_up;
        }

        memset(acGroupName, 0, sizeof(acGroupName));
        if (OPERATOR == pstUser->iUserRole || ADMIN == pstUser->iUserRole) {
            strncpy(acGroupName, NACM_GROUP_READ_WRITE_EXEC, sizeof(acGroupName) - 1);
            snprintf(acUserLog, sizeof(acUserLog), "%s,%s", pstUser->acName,
                     (ADMIN == pstUser->iUserRole) ? NACM_GROUP_SUPER_USER : NACM_GROUP_READ_WRITE_EXEC);
        } else if (QUERY == pstUser->iUserRole) {
            strncpy(acGroupName, NACM_GROUP_READ_EXEC, sizeof(acGroupName) - 1);
            snprintf(acUserLog, sizeof(acUserLog), "%s,%s", pstUser->acName, NACM_GROUP_READ_EXEC);
        } else {
            rc = SR_ERR_INVAL_ARG;
            goto clean_up;
        }

        rc = DSC_UserNacmAdd(pstUser->acName, pstUser->iUserRole);
        if (rc != SR_ERR_OK) {
            goto clean_up;
        }

        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_SET_USER, acUserLog);
    }

clean_up:
    LOG_WRITE(EN_LOG_NOTICE, "User_AddUser return %d. username %s, role %d",
              rc, pstUser->acName, pstUser->iUserRole);

    return rc;
}


signed int
DSC_SetHashPassword(XPATH_PWD_ST *pstXpathPwd)
{
    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    rc = DSC_SetHashPassword_Session(pstSess, pstXpathPwd);

    return rc;
}

signed int
DSC_SetHashPassword_Session(sr_session_ctx_t *pstSess, XPATH_PWD_ST *pstXpathPwd)
{

    signed int rc = SR_ERR_OK;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstXpathPwd) {
        LOG_WRITE(EN_LOG_ERROR, "pstXpathPwd is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_string_item(pstSess, pstXpathPwd->acXpath, pstXpathPwd->acPwd, sizeof(pstXpathPwd->acPwd));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-hashpassword: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD password DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

static int
__DelUser(char *pcUserName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[512] = {0};

    if (NULL == pcUserName) {
        LOG_WRITE(EN_LOG_ERROR, "pcUserName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* can't delete admin */
    if (0 == strcmp(ADMIN_USER_NAME, pcUserName)) {
        LOG_WRITE(EN_LOG_ERROR, "Can't delete admin");
        return SR_ERR_OPERATION_FAILED;
    }

    snprintf(xpath, sizeof(xpath), XPATH_AUTH_USER, pcUserName);

    return DSC_DelNode(sess, xpath);
}

int
DSC_DelUser(char *pcUserName, USER_ST *pstCurUser)
{
    int rc = SR_ERR_OK;
    int iUserRole = QUERY;

    NACM_GROUP_LIST_ST stNacmGropList = {NULL, 0};
    int i = 0;
    int j = 0;

    if (NULL == pcUserName || NULL == pstCurUser) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (0 == strcmp(pstCurUser->acName, pcUserName)) {
        LOG_WRITE(EN_LOG_ERROR, "Can't delete self. Username : %s", pcUserName);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (0 == strcmp(pcUserName, ADMIN_USER_NAME)) {
        LOG_WRITE(EN_LOG_ERROR, "Can't delete admin");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (pstCurUser->iUserRole <= QUERY) {
        LOG_WRITE(EN_LOG_ERROR, "Current user access is query. Username : %s", pstCurUser->acName);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetUserRole((const char *)pcUserName, &iUserRole);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "user may be not existed : %s", pcUserName);
        rc = SR_ERR_NOT_FOUND;
        goto cleanup;
    }

    if (pstCurUser->iUserRole < iUserRole) {
        LOG_WRITE(EN_LOG_ERROR, "write-user can not delete super-user. role: (%d, %d)", pstCurUser->iUserRole, iUserRole);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __DelUser(pcUserName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__DelUser failed. %s", pcUserName);
        goto cleanup;
    }

    rc = DSC_GetNacmGroups(&stNacmGropList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetNacmGroups failed. %s", pcUserName);
        goto cleanup;
    }

    /* delete the user in all groups */
    for (i = 0; i < stNacmGropList.uiGroupCnt; i++) {
        for (j = 0; j < stNacmGropList.pstGroup[i].uiUserCnt; j++) {
            if (0 == strcmp(stNacmGropList.pstGroup[i].pacUserName[j], pcUserName)) {
                rc = DSC_DelNacmGroupUser(stNacmGropList.pstGroup[i].acGroupName, pcUserName);
                if (SR_ERR_OK != rc) {
                    LOG_WRITE(EN_LOG_ERROR, "DSC_DelNacmGroupUser failed. %s", pcUserName);
                    goto cleanup;
                }
            }
        }
    }

    /* clear token */
    DSC_DeteleTokensByUsername(pcUserName);

    DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_DELETE_USER, pcUserName);

cleanup:

    if (NULL != stNacmGropList.pstGroup) {
        for (i = 0; i < stNacmGropList.uiGroupCnt; i++) {
            DEF_SAFE_FREE(stNacmGropList.pstGroup[i].pacUserName);
        }

        DEF_SAFE_FREE(stNacmGropList.pstGroup);
    }

    LOG_WRITE(EN_LOG_NOTICE, "User_DelUser return %d. username %s, role %d",
              rc, pcUserName, pstCurUser->iUserRole);

    return rc;
}


signed int
DSC_GetUsers(USER_LIST_ST *pstUsers)
{
    sr_data_t *pstSubtrees = NULL;
    uint32_t uiUserCnt = 0;
    signed int rc = SR_ERR_OK;
    unsigned int i, j;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    unsigned int uiLeafCnt;
    sr_node_t *apstLeaf[20];
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUsers) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all users */

    rc = get_subtrees(pstSess, XPATH_AUTH_USER_LIST, 0, SR_OPER_NO_STATE, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiUserCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_AUTH_USER_LIST, &pstNodeSet);
    if (LY_SUCCESS == rc) {
        uiUserCnt = pstNodeSet->count;
    }

    /* if there is one user at least */
    if (uiUserCnt != 0) {
        pstUsers->pstUser = (USER_ST *)malloc(uiUserCnt * sizeof(USER_ST));
        if (NULL == pstUsers->pstUser) {
            LOG_WRITE(EN_LOG_ERROR, "pstUsers->pstUser is null");

            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        memset(pstUsers->pstUser, 0, sizeof(uiUserCnt * sizeof(USER_ST)));

        for (i = 0; i < uiUserCnt; i++) {
            uiLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < uiLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(pstLeaf->schema->name, "name")) {
                    strncpy(pstUsers->pstUser[i].acName, lyd_get_value(pstLeaf), sizeof(pstUsers->pstUser[i].acName) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, "password")) {
                    memset(pstUsers->pstUser[i].acPassword, 0x2A, 10); /*cancel display password,display "*"(0x2A) instead. */
                    pstUsers->pstUser[i].acPassword[10] = '\0';
                } else if (0 == strcmp(pstLeaf->schema->name, "user-role")) {
                    pstUsers->pstUser[i].iUserRole = ((sr_node_term_t *)pstLeaf)->value.uint32;
                }
            }
        }
    }

    pstUsers->uiTotal = uiUserCnt;

ERR_LABEL:

    if (pstNodeSet) {
        ly_set_free(pstNodeSet, NULL);
    }

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    return rc;
}

signed int
DSC_GetUserRole(const char *pcUsername, int *piUserRole)
{

    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[DEF_BUF_SIZE_512] = {0};
    uint32_t uiUserRole = 0;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pcUsername || NULL == piUserRole) {
        LOG_WRITE(EN_LOG_ERROR, "invlid iput para %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all users */

    snprintf(acXpath, sizeof(acXpath), XPATH_AUTH_USER USER_ROLE_NODE, pcUsername);
    rc = get_uint32_item(pstSess, acXpath, &uiUserRole);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_uint32_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    *piUserRole = (int)uiUserRole;

ERR_LABEL:

    return rc;
}

signed int
DSC_GetHashPwdByUsername(sr_session_ctx_t *pstSess, const char *pcUsername, char *pcHashPwd, unsigned int uiLen)
{
    signed int rc = SR_ERR_OK;
    char acXpath[DEF_BUF_SIZE_512] = {0};
    char acPassword[USER_PASSWORD_MAX_LEN] = {0};

    /* 1. check parameters */
    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pcUsername || NULL == pcHashPwd || 0 == uiLen) {
        LOG_WRITE(EN_LOG_ERROR, "invlid iput para %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get hash password of the user */
    snprintf(acXpath, sizeof(acXpath), XPATH_AUTH_USER USER_PWD_NODE, pcUsername);
    rc = get_string_item(pstSess, acXpath, acPassword, sizeof(acPassword) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    snprintf(pcHashPwd, uiLen, "%s", acPassword);

ERR_LABEL:

    return rc;
}

char *
DSC_AuthToString(AUTH_TYPE_EN enAuthType, char *pcAuthStr)
{
    if (AUTH_RADIUS == enAuthType) {
        strcpy(pcAuthStr, AUTH_RADIUS_STR);
    } else if (AUTH_TACACS == enAuthType) {
        strcpy(pcAuthStr, AUTH_TACACS_STR);
    } else {
        strcpy(pcAuthStr, AUTH_LOCAL_STR);
    }

    return pcAuthStr;
}

signed int
DSC_GetAuthOrder(int *piAuthOrder, int *piAuthCnt)
{
    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_val_t *pstVal = NULL;
    size_t uiValCnt = 0;

    if ((NULL == piAuthOrder) || (NULL == piAuthCnt) || (NULL == pstSess)) {
        LOG_WRITE(EN_LOG_ERROR, "input is null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, XPATH_AUTH_ORDER_LIST, 0, SR_OPER_NO_STATE, &pstVal, &uiValCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_items auth order: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    for (int i = 0; i < uiValCnt; i++) {
        if (strstr(pstVal[i].data.string_val, "radius")) {
            piAuthOrder[i] = AUTH_RADIUS;
        } else if (strstr(pstVal[i].data.string_val, "tacacs")) {
            piAuthOrder[i] = AUTH_TACACS;
        } else if (strstr(pstVal[i].data.string_val, "local-users")) {
            piAuthOrder[i] = AUTH_LOCAL;
        }
    }

    *piAuthCnt = uiValCnt;

    sr_free_values(pstVal, uiValCnt);
ERR_LABEL:

    return rc;
}

static USER_AUTH_ORDER_EN
__GetUserAuthOrder()
{
    USER_AUTH_ORDER_EN enUserAuthOrder = EN_AUTH_ORDER_MAX;
    int aiAuthOrder[DEF_USER_AUTHENTICATION_ORDER_MAX_ELEMENTS] = {0};
    int iAuthCnt = 0;

    /* get authentication order */
    DSC_GetAuthOrder(aiAuthOrder, &iAuthCnt);

    if (1 == iAuthCnt) {
        if (AUTH_LOCAL == aiAuthOrder[0]) {
            enUserAuthOrder = EN_AUTH_ORDER_ONLY_LOCAL;
        } else if (AUTH_TACACS == aiAuthOrder[0]) {
            enUserAuthOrder = EN_AUTH_ORDER_ONLY_TACACS;
        }
    } else if (2 == iAuthCnt) {
        if (AUTH_LOCAL == aiAuthOrder[0] && AUTH_TACACS == aiAuthOrder[1]) {
            /* enUserAuthOrder = EN_AUTH_ORDER_LOCAL_THEN_TACACS; */
            enUserAuthOrder = EN_AUTH_ORDER_TACACS_THEN_LOCAL;
        } else if (AUTH_TACACS == aiAuthOrder[0] && AUTH_LOCAL == aiAuthOrder[1]) {
            /* enUserAuthOrder = EN_AUTH_ORDER_TACACS_THEN_LOCAL; */
            enUserAuthOrder = EN_AUTH_ORDER_TACACSONLY_THEN_LOCAL;
        }
    }

    return enUserAuthOrder;
}
#if 0
static int
__AddAuthOrder(sr_session_ctx_t *pstSess, char *pcAuthOrder, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acBuf[32] = {0};

    if (NULL == pstSess || NULL == pcAuthOrder) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    snprintf(acBuf, sizeof(acBuf), "%s", pcAuthOrder);

    rc = set_identityref_item(pstSess, XPATH_AUTH_ORDER_LIST, acBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s, %s", acBuf, sr_strerror(rc));
        goto END_LABEL;
    }

    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

END_LABEL:

    return rc;
}
#endif
int
DSC_SetUserAuthOrder(USER_AUTH_ORDER_EN enUserAuthOrder)
{
    int rc = SR_ERR_OK;
    USER_AUTH_ORDER_EN enCurAuthOrder = EN_AUTH_ORDER_MAX;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acXpath[DEF_BUF_SIZE_256] = {0};

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    enCurAuthOrder = __GetUserAuthOrder();
    if (enCurAuthOrder == enUserAuthOrder) {
        rc = SR_ERR_OK;
        LOG_WRITE(EN_LOG_NOTICE, "user-auth-order is same: %d", enCurAuthOrder);
        goto END_LABEL;
    }

    /**************************************************************************************************************************
    ** RNR-134(implemented on 2023-12-21):
    **    To ensure the normal operation of authentication functions in new and old FirmwareBundle, special handle as follows
    **     local-then-tacacs (old) == tacacs-then-local (new)
    **     tacacs-then-local (old) == tacacsonly-then-local (new)
    ***************************************************************************************************************************/

    if (EN_AUTH_ORDER_TACACS_THEN_LOCAL == enUserAuthOrder) {
        snprintf(acXpath, sizeof(acXpath), XPATH_AUTH_ORDER_LIST "[.='%s']", "ietf-system:" AUTH_LOCAL_STR);
        rc = sr_move_item(pstSess, acXpath, SR_MOVE_FIRST, NULL, NULL, NULL, 0);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_move_item: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    } else if (EN_AUTH_ORDER_TACACSONLY_THEN_LOCAL == enUserAuthOrder) {
        snprintf(acXpath, sizeof(acXpath), XPATH_AUTH_ORDER_LIST "[.='%s']", EXT_TACACS_TYPE_STR);
        rc = sr_move_item(pstSess, acXpath, SR_MOVE_FIRST, NULL, NULL, NULL, 0);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_move_item: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid enUserAuthOrder: %d", enUserAuthOrder);
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_SET_USER_AUTH_ORDER,
                     (EN_AUTH_ORDER_TACACS_THEN_LOCAL == enUserAuthOrder) ? TACACS_THEN_LOCAL_STR : TACACSONLY_THEN_LOCAL_STR);

END_LABEL:

    return rc;
}

signed int
DSC_SetNacmEnable(bool bState)
{
    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. modify element */

    /* nacm/enable-nacm */
    rc = set_bool_item(pstSess, XPATH_NACM_ENABLE, bState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-nacm enable-nacm: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD nacm enable DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

signed int
DSC_AddNacmGroupUser(char *pcGroupName, char *pcUserName)
{
    signed int rc = SR_ERR_OK;
    char xpath[512] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if ((NULL == pcGroupName) || (NULL == pcUserName)) {
        LOG_WRITE(EN_LOG_ERROR, "pcUserName or pcUserName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2.  add a leaf */

    /* group/user-name */
    sprintf(xpath, XPATH_NACM_GROUPS_GROUP_USERNAME_LIST, pcGroupName);
    rc = set_string_item(pstSess, xpath, pcUserName, 64);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-group user-name: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD nacm group DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

signed int
DSC_DelNacmGroupUser(char *pcGroupName, char *pcUserName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[512] = {0};

    if ((NULL == pcGroupName) || (NULL == pcUserName)) {
        LOG_WRITE(EN_LOG_ERROR, "pcGroupName or pcUserName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, sizeof(xpath), XPATH_NACM_GROUPS_GROUP_USERNAME, pcGroupName, pcUserName);

    return DSC_DelNode(sess, xpath);
}

signed int
DSC_GetNacmBasic(NACM_BASIC_ST *pstNacmBasic)
{
    signed int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    sr_val_t *pstVal = NULL;
    size_t uiValCnt = 0;
    unsigned int i;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstNacmBasic) {
        LOG_WRITE(EN_LOG_ERROR, "pstNacmBasic is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get nacm basic info */

    rc = sr_get_items(pstSess, XPATH_NACM_CHILD_NODE, 0, SR_OPER_NO_STATE, &pstVal, &uiValCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_items error-nacm basic: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    for (i = 0; i < uiValCnt; i++) {
        if (strstr(pstVal[i].xpath, "enable-nacm")) {
            pstNacmBasic->bEnable = pstVal[i].data.bool_val;
        } else if (strstr(pstVal[i].xpath, "read-default")) {
            pstNacmBasic->enReadActionType = (0 == strcmp(pstVal[i].data.string_val, "permit")) ?
                                             ACTION_TYPE_PERMIT : ACTION_TYPE_DENY;
        } else if (strstr(pstVal[i].xpath, "write-default")) {
            pstNacmBasic->enWriteActionType = (0 == strcmp(pstVal[i].data.string_val, "permit")) ?
                                              ACTION_TYPE_PERMIT : ACTION_TYPE_DENY;
        } else if (strstr(pstVal[i].xpath, "exec-default")) {
            pstNacmBasic->enExecActionType = (0 == strcmp(pstVal[i].data.string_val, "permit")) ?
                                             ACTION_TYPE_PERMIT : ACTION_TYPE_DENY;
        } else if (strstr(pstVal[i].xpath, "enable-external-groups")) {
            pstNacmBasic->bExtGroupEnable = pstVal[i].data.bool_val;
        }
    }

    sr_free_values(pstVal, uiValCnt);

ERR_LABEL:

    return rc;
}

signed int
DSC_GetNacmGroups(NACM_GROUP_LIST_ST *pstNacmGroups)
{
    sr_data_t *pstSubtrees = NULL;
    uint32_t uiGroupCnt = 0, uiUserCnt = 0;
    signed int rc = SR_ERR_OK;
    unsigned int i, j;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_node_t *pstGroupNameNode = NULL;
    sr_node_t *pstGroupTree = NULL;
    sr_node_t *pstUserNameNode = NULL;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null");
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstNacmGroups) {
        LOG_WRITE(EN_LOG_ERROR, "pstNacmGroups is null");
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all nacm groups */
    rc = get_subtree(pstSess, XPATH_NACM_GROUPS, SR_OPER_NO_STATE, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiGroupCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    uiGroupCnt = DSC_GetTreeChildListNum(pstSubtrees->tree, "group");

    /* if there is one group at least */
    if (uiGroupCnt != 0) {
        pstNacmGroups->pstGroup = (NACM_GROUP_ST *)calloc(uiGroupCnt, sizeof(NACM_GROUP_ST));

        if (NULL == pstNacmGroups->pstGroup) {
            LOG_WRITE(EN_LOG_ERROR, "pstNacmGroups->pstGroup is null");

            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }
        pstGroupTree = ((sr_node_inner_t *)(pstSubtrees->tree))->child;
        for (i = 0; i < uiGroupCnt; i++) {
            /* get group name */
            pstGroupNameNode = DSC_GetTreeChild(pstGroupTree, "name", 0);
            if (NULL == pstGroupNameNode) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_GetTreeChild error: group %d", i);

                rc = SR_ERR_INVAL_ARG;
                goto ERR_LABEL;
            }

            strncpy(pstNacmGroups->pstGroup[i].acGroupName, pstGroupNameNode->schema->name, NACM_NAME_MAX_LEN - 1);

            /* get user-name */
            uiUserCnt = DSC_GetTreeChildListNum(pstGroupTree, "user-name");
            if (uiUserCnt != 0) {
                pstNacmGroups->pstGroup[i].pacUserName = (char (*)[NACM_NAME_MAX_LEN])calloc(uiUserCnt, NACM_NAME_MAX_LEN);

                if (NULL == pstNacmGroups->pstGroup[i].pacUserName) {
                    LOG_WRITE(EN_LOG_ERROR, "pstNacmGroups->pstGroup[i].pacUserName is null");

                    rc = SR_ERR_INVAL_ARG;
                    goto ERR_LABEL;
                }

                for (j = 0; j < uiUserCnt; j++) {
                    pstUserNameNode = DSC_GetTreeChild(pstGroupTree, "user-name", j);
                    if (NULL == pstUserNameNode) {
                        LOG_WRITE(EN_LOG_ERROR, "pstUserName is NULL");

                        rc = SR_ERR_INVAL_ARG;
                        goto ERR_LABEL;
                    }

                    strncpy((char *)(pstNacmGroups->pstGroup[i].pacUserName + j), pstUserNameNode->schema->name, NACM_NAME_MAX_LEN - 1);
                }
            }

            pstNacmGroups->pstGroup[i].uiUserCnt = uiUserCnt;

            pstGroupTree = pstGroupTree->next;
        }
    }

    pstNacmGroups->uiGroupCnt = uiGroupCnt;

ERR_LABEL:


    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (rc != SR_ERR_OK) {
        if (pstNacmGroups->pstGroup) {
            for (i = 0; i < pstNacmGroups->uiGroupCnt; i++) {
                if (pstNacmGroups->pstGroup[i].pacUserName) {
                    free(pstNacmGroups->pstGroup[i].pacUserName);
                }
            }

            free(pstNacmGroups->pstGroup);
            pstNacmGroups->pstGroup = NULL;
        }
    }

    return rc;
}

signed int
DSC_GetNacmRulelists(RULELIST_LIST_ST *pstNacmrulelists)
{
    sr_data_t *pstSubtrees = NULL;
    size_t uiRulelistCnt = 0, uiGroupCnt = 0, uiRuleCnt = 0;
    signed int rc = SR_ERR_OK;
    unsigned int i, j, k;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_node_t *pstRulelistName = NULL;
    sr_node_t *pstGroup = NULL;
    sr_node_t *pstRuleList = NULL;
    unsigned int uiLeafCnt;
    sr_node_t *apstLeaf[20];
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /* 1. check parameters */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstNacmrulelists) {
        LOG_WRITE(EN_LOG_ERROR, "pstNacmrulelists is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all nacm rulelist */

    rc = get_subtrees(pstSess, XPATH_NACM_RULELIST_LIST, 0, SR_OPER_NO_STATE, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiRulelistCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_NACM_RULELIST_LIST, &nodeSet);
    if (LY_SUCCESS == rc) {
        uiRulelistCnt = nodeSet->count;
    }

    /* if there is one rulelist at least */
    if (uiRulelistCnt != 0) {
        pstNacmrulelists->pstRulelist = (RULELIST_ST *)calloc(uiRulelistCnt, sizeof(RULELIST_ST));

        if (NULL == pstNacmrulelists->pstRulelist) {
            LOG_WRITE(EN_LOG_ERROR, "pstNacmrulelists->pstRulelist is null");

            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        for (i = 0; i < uiRulelistCnt; i++) {
            /* get rulelist name */
            pstRulelistName = DSC_GetTreeChild(nodeSet->dnodes[i], "name", 0);
            if (NULL == pstRulelistName) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_GetTreeChild error: rulelist %d", i);

                rc = SR_ERR_INVAL_ARG;
                goto ERR_LABEL;
            }
            pstLeaf = (sr_node_t *)pstGroup;
            strncpy(pstNacmrulelists->pstRulelist[i].acRuleListName, lyd_get_value(pstLeaf), sizeof(pstNacmrulelists->pstRulelist[i].acRuleListName) - 1);

            /* get groups */
            uiGroupCnt = DSC_GetTreeChildListNum(nodeSet->dnodes[i], "group");
            if (uiGroupCnt != 0) {
                pstNacmrulelists->pstRulelist[i].pacGroupName = (char (*)[NACM_NAME_MAX_LEN])calloc(uiGroupCnt, NACM_NAME_MAX_LEN);

                if (NULL == pstNacmrulelists->pstRulelist[i].pacGroupName) {
                    LOG_WRITE(EN_LOG_ERROR, "pstNacmrulelists->pstRulelist[i].pacGroupName is null");

                    rc = SR_ERR_INVAL_ARG;
                    goto ERR_LABEL;
                }

                for (j = 0; j < uiGroupCnt; j++) {
                    pstGroup = DSC_GetTreeChild(nodeSet->dnodes[i], "group", j);
                    if (NULL == pstGroup) {
                        LOG_WRITE(EN_LOG_ERROR, "pstGroup is NULL");

                        rc = SR_ERR_INVAL_ARG;
                        goto ERR_LABEL;
                    }
                    pstLeaf = (sr_node_t *)pstGroup;
                    strncpy((char *)(pstNacmrulelists->pstRulelist[i].pacGroupName + j), lyd_get_value(pstLeaf), NACM_NAME_MAX_LEN - 1);
                }
            }

            pstNacmrulelists->pstRulelist[i].uiGroupCnt = uiGroupCnt;

            /* get rules */
            uiRuleCnt = DSC_GetTreeChildListNum(nodeSet->dnodes[i], "rule");
            if (uiRuleCnt != 0) {
                pstNacmrulelists->pstRulelist[i].stRuleList.pstRule = (RULE_ST *)calloc(uiRuleCnt, sizeof(RULE_ST));

                if (NULL == pstNacmrulelists->pstRulelist[i].stRuleList.pstRule) {
                    LOG_WRITE(EN_LOG_ERROR, "pstNacmrulelists->pstRulelist[i].stRuleList.pstRule is null");

                    rc = SR_ERR_INVAL_ARG;
                    goto ERR_LABEL;
                }

                for (j = 0; j < uiRuleCnt; j++) {
                    pstRuleList = DSC_GetTreeChild(nodeSet->dnodes[i], "rule", j);

                    if (NULL == pstRuleList) {
                        LOG_WRITE(EN_LOG_ERROR, "pstRuleList is null");

                        rc = SR_ERR_INVAL_ARG;
                        goto ERR_LABEL;
                    }

                    uiLeafCnt = DSC_GetTreeLeaf(pstRuleList, apstLeaf);

                    for (k = 0; k < uiLeafCnt; k++) {
                        pstLeaf = (sr_node_t *)apstLeaf[k];
                        if (0 == strcmp(apstLeaf[k]->schema->name, "name")) {
                            strncpy(pstNacmrulelists->pstRulelist[i].stRuleList.pstRule[j].acRuleName, lyd_get_value(pstLeaf), NACM_NAME_MAX_LEN - 1);
                        } else if (0 == strcmp(apstLeaf[k]->schema->name, "module-name")) {
                            strncpy(pstNacmrulelists->pstRulelist[i].stRuleList.pstRule[j].acModName, lyd_get_value(pstLeaf), NACM_NAME_MAX_LEN - 1);
                        } else if (0 == strcmp(apstLeaf[k]->schema->name, "access-operations")) {
                            strncpy(pstNacmrulelists->pstRulelist[i].stRuleList.pstRule[j].acAccessOp, lyd_get_value(pstLeaf), NACM_RULE_MAX_LEN - 1);
                        } else if (0 == strcmp(apstLeaf[k]->schema->name, "action")) {
                            pstNacmrulelists->pstRulelist[i].stRuleList.pstRule[j].enActionType = (0 == strcmp(lyd_get_value(pstLeaf), "permit")) ?
                                                                                                  ACTION_TYPE_PERMIT : ACTION_TYPE_DENY;
                        } else if (0 == strcmp(apstLeaf[k]->schema->name, "comment")) {
                            strncpy(pstNacmrulelists->pstRulelist[i].stRuleList.pstRule[j].acComment, lyd_get_value(pstLeaf), NACM_RULE_MAX_LEN - 1);
                        }
                    }
                }
            }

            pstNacmrulelists->pstRulelist[i].stRuleList.uiRuleCnt = uiRuleCnt;
        }
    }

    pstNacmrulelists->uiRulelistCnt = uiRulelistCnt;

ERR_LABEL:

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (rc != SR_ERR_OK) {
        if (pstNacmrulelists->pstRulelist) {
            for (i = 0; i < pstNacmrulelists->uiRulelistCnt; i++) {
                if (pstNacmrulelists->pstRulelist[i].pacGroupName) {
                    free(pstNacmrulelists->pstRulelist[i].pacGroupName);
                }

                if (pstNacmrulelists->pstRulelist[i].stRuleList.pstRule) {
                    free(pstNacmrulelists->pstRulelist[i].stRuleList.pstRule);
                }
            }

            free(pstNacmrulelists->pstRulelist);
            pstNacmrulelists->pstRulelist = NULL;
        }
    }

    return rc;
}

int
DSC_UserNacmDelete(char *pcUserName)
{
    NACM_GROUP_LIST_ST stNacmGroups;
    NACM_GROUP_ST *pstGroup = NULL;
    int i = 0;
    int j = 0;
    int ret = SR_ERR_OK;

    memset(&stNacmGroups, 0, sizeof(stNacmGroups));
    stNacmGroups.pstGroup = NULL;

    ret = DSC_GetNacmGroups(&stNacmGroups);
    if (ret != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    for (i = 0; i < stNacmGroups.uiGroupCnt; i++) {
        pstGroup = stNacmGroups.pstGroup + i;
        if (NULL == pstGroup) {
            ret = SR_ERR_NOT_FOUND;
            goto ERR_LABEL;
        }

        for (j = 0; j < pstGroup->uiUserCnt; j++) {
            if ((0 == strcmp(pstGroup->pacUserName[j], pcUserName))
                && ((0 == strcmp(NACM_GROUP_READ_EXEC, pstGroup->acGroupName))
                    || (0 == strcmp(NACM_GROUP_READ_WRITE_EXEC, pstGroup->acGroupName)))) {
                ret = DSC_DelNacmGroupUser(pstGroup->acGroupName, pstGroup->pacUserName[j]);
                if (ret != SR_ERR_OK) {
                    goto ERR_LABEL;
                }
            }
        }
    }

    if (i == stNacmGroups.uiGroupCnt) { /* not found */
        ret = SR_ERR_NOT_FOUND;
    }

ERR_LABEL:
    if (NULL != stNacmGroups.pstGroup) {
        free(stNacmGroups.pstGroup);
        stNacmGroups.pstGroup = NULL;
    }

    return ret;
}

static int
__UserNacmUpdate(char *pcGroupName, char *pcUserName)
{
    NACM_GROUP_LIST_ST stNacmGroups;
    NACM_GROUP_ST *pstGroup = NULL;
    int i = 0;
    int j = 0;
    int ret = SR_ERR_OK;

    memset(&stNacmGroups, 0, sizeof(stNacmGroups));
    stNacmGroups.pstGroup = NULL;

    ret = DSC_GetNacmGroups(&stNacmGroups);
    if (ret != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    for (i = 0; i < stNacmGroups.uiGroupCnt; i++) {
        pstGroup = stNacmGroups.pstGroup + i;
        if (NULL == pstGroup) {
            ret = SR_ERR_NOT_FOUND;
            goto ERR_LABEL;
        }

        if (0 == strcmp(pcGroupName, pstGroup->acGroupName)) {
            for (j = 0; j < pstGroup->uiUserCnt; j++) {
                if (0 == strcmp(pstGroup->pacUserName[j], pcUserName)) {
                    /* user exists in the group already */
                    goto ERR_LABEL;
                }
            }
        } else {
            for (j = 0; j < pstGroup->uiUserCnt; j++) {
                if (0 == strcmp(pstGroup->pacUserName[j], pcUserName)) {
                    DSC_DelNacmGroupUser(pstGroup->acGroupName, pstGroup->pacUserName[j]);
                    break;
                }
            }
        }
    }

    DSC_AddNacmGroupUser(pcGroupName, pcUserName);

ERR_LABEL:
    if (NULL != stNacmGroups.pstGroup) {
        free(stNacmGroups.pstGroup);
        stNacmGroups.pstGroup = NULL;
    }

    return ret;
}

int
DSC_UserNacmAdd(char *pcUserName, ACCESS_EN iUserRole)
{
    int rc = SR_ERR_OK;

    /* save user privilege to NACM */
    if (QUERY == iUserRole) {
        rc = __UserNacmUpdate(NACM_GROUP_READ_EXEC, pcUserName);
    } else if (OPERATOR == iUserRole) {
        rc = __UserNacmUpdate(NACM_GROUP_READ_WRITE_EXEC, pcUserName);
    } else if (ADMIN == iUserRole) {
        rc = __UserNacmUpdate(NACM_GROUP_READ_WRITE_EXEC, pcUserName);
    } else {
        rc = SR_ERR_NOT_FOUND;
    }

    return rc;
}

char *
DSC_GetNacmGroupNameByRole(int iRole)
{

    switch (iRole) {
    case ADMIN:
        return NACM_GROUP_SUPER_USER;
    case OPERATOR:
        return NACM_GROUP_READ_WRITE_EXEC;
    case QUERY:
        return NACM_GROUP_READ_EXEC;
    default:
        return "Unknown";
    }

}


int
DSC_CryptHash(HASH_ALGORITHM_EN enHashMethod, char *pcSalt, char *pcPwd, char *pcHashPwd, unsigned int uiLen)
{
    char acHash[USER_PASSWORD_MAX_LEN] = {0};
    unsigned int uiSaltLen = 0;
    int rc = SR_ERR_OK;

    if (NULL == pcSalt || NULL == pcPwd || NULL == pcHashPwd || 0 == uiLen) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "para is null %d", SR_ERR_INVAL_ARG);
        goto ERR_LABEL;
    }

    uiSaltLen = strlen(pcSalt);
    if (EN_HASH_MD5 == enHashMethod) {
        if (uiSaltLen > HASH_MD5_SALT_MAX_LEN) {
            pcSalt[HASH_MD5_SALT_MAX_LEN] = '\0';
        }
    } else if (EN_HASH_SHA256 == enHashMethod || EN_HASH_SHA512 == enHashMethod) {
        if (uiSaltLen > HASH_SHA256_SALT_MAX_LEN) {
            pcSalt[HASH_SHA256_SALT_MAX_LEN] = '\0';
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "error hash %d", enHashMethod);
        goto ERR_LABEL;
    }

    snprintf(acHash, USER_PASSWORD_MAX_LEN, "$%d$%s$", enHashMethod, pcSalt);

    strncpy(acHash, crypt(pcPwd, acHash), USER_PASSWORD_MAX_LEN - 1);

    snprintf(pcHashPwd, uiLen, "%s", acHash);

    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}


static int
__AddTacServer(TACACS_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char xpath[512] = {0};

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstServer) {
        LOG_WRITE(EN_LOG_ERROR, "pstServer is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. add or modify a list */
    sprintf(xpath, XPATH_TACACS_SERVER_ADDR, pstServer->acName);
    rc = set_string_item(pstSess, xpath, pstServer->acIp, sizeof(pstServer->acIp));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    sprintf(xpath, XPATH_TACACS_SERVER_PORT, pstServer->acName);
    rc = set_uint16_item(pstSess, xpath, (uint16_t)pstServer->uiPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    sprintf(xpath, XPATH_TACACS_SERVER_KEY, pstServer->acName);
    rc = set_string_item(pstSess, xpath, pstServer->acKey, sizeof(pstServer->acKey));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp key: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD tacacs server DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_AddTacServer(TACACS_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    TACACS_SERVER_LIST_ST stServerList = {NULL, 0};
    int i = 0;
    bool bExistedFlag = false;
    char acUserLog[DEF_BUF_SIZE_128] = {0};

    if (NULL == pstServer) {
        LOG_WRITE(EN_LOG_ERROR, "Input null!");
        rc = SR_ERR_INVAL_ARG;
        goto clean_up;
    }

    rc = UTIL_ChkStrSpecChr(pstServer->acKey);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Check TacacsServer Key %s error!", pstServer->acKey);
        rc = SR_ERR_INVAL_ARG;
        goto clean_up;
    }

    rc = DSC_GetTacServers(&stServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetTacServers error!");
        goto clean_up;
    }

    /* check if existed */
    for (i = 0; i < stServerList.uiTotal; i++) {
        if (0 == strcmp(pstServer->acName, stServerList.pstTacacsServer[i].acName)) {
            bExistedFlag = true;
            break;
        }
    }

    if (bExistedFlag) {
        /* delete existed priority tacacs server */
        rc = DSC_DelTacServer(stServerList.pstTacacsServer[i].acIp);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_DelTacServer. tacacs server: %s", stServerList.pstTacacsServer[i].acIp);
            goto clean_up;
        }
    } else {
        if (stServerList.uiTotal >= TACACS_SERVER_MAX_NUM) {
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "The number of tacacs server reached the upper limit");
            goto clean_up;
        }
    }

    /* add it */
    rc = __AddTacServer(pstServer);
    if (rc != SR_ERR_OK) {
        /* restore old tacacs */
        if (bExistedFlag) {
            (void)__AddTacServer(&(stServerList.pstTacacsServer[i]));
        }
        goto clean_up;
    } else {
        snprintf(acUserLog, sizeof(acUserLog), "%s,%s", pstServer->acName, pstServer->acIp);
        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_SET_TACACS, acUserLog);
    }

clean_up:
    LOG_WRITE(EN_LOG_NOTICE, "%s return code = %d. acName:%s,"
              "acIp:%s,"
              "acKey:%s,"
              "uiPort = %d",
              __func__, rc, pstServer->acName, pstServer->acIp, PASSWORD_NOT_CLEAR_TEXT, pstServer->uiPort);

    DEF_SAFE_FREE(stServerList.pstTacacsServer);

    return rc;
}



int
DSC_DelTacServer(char *pcServerIp)
{
    char xpath[512] = {0};
    int rc = SR_ERR_OK;
    sr_session_ctx_t *session = DSC_GetRunningSession();

    TACACS_SERVER_LIST_ST stServerList = {NULL, 0};
    int i = 0;
    bool bExistedFlag = false;
    char acUserLog[DEF_BUF_SIZE_128] = {0};

    if (NULL == pcServerIp) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerIp is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetTacServers(&stServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetTacServers error!");
        goto clean_up;
    }

    /* check if existed */
    for (i = 0; i < stServerList.uiTotal; i++) {
        if (0 == strcmp(pcServerIp, stServerList.pstTacacsServer[i].acIp)) {
            bExistedFlag = true;
            break;
        }
    }

    if (bExistedFlag) {

        sprintf(xpath, XPATH_TACACS_SERVER, stServerList.pstTacacsServer[i].acName);
        rc = sr_delete_item(session, xpath, SR_EDIT_STRICT);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "sr_delete_item error! error %s", sr_strerror(rc));
            goto clean_up;
        }

        /* commit */
        rc = DSC_Commit(session);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Del tacacs server DSC_Commit error %s", sr_strerror(rc));
        }

        snprintf(acUserLog, sizeof(acUserLog), "%s,%s", stServerList.pstTacacsServer[i].acName, stServerList.pstTacacsServer[i].acIp);
        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_DELETE_TACACS, acUserLog);
    } else {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "NOT find server %s", pcServerIp);
    }

clean_up:

    return rc;
}

int
DSC_GetTacServers(TACACS_SERVER_LIST_ST *pstTacacsList)
{
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_data_t *pstSubtrees = NULL;
    uint32_t uiServerCnt = 0;
    signed int rc = SR_ERR_OK;
    unsigned int i, j;
    unsigned int uiLeafCnt;
    sr_node_t *apstLeaf[64];
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstTacacsList) {
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(pstSess, XPATH_TACACS_SERVER_LIST, 0, SR_OPER_NO_STATE, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiServerCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_TACACS_SERVER_LIST, &pstNodeSet);
        if (LY_SUCCESS == rc) {
            uiServerCnt = pstNodeSet->count;
        }
    }
    pstTacacsList->uiTotal = uiServerCnt;

    /* if there is one server at least */
    if (uiServerCnt != 0) {
        pstTacacsList->pstTacacsServer = (TACACS_SERVER_ST *)malloc(uiServerCnt * sizeof(TACACS_SERVER_ST));

        if (NULL == pstTacacsList->pstTacacsServer) {
            LOG_WRITE(EN_LOG_ERROR, "pstTacacsList->pstTacacsServer is null");
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        memset(pstTacacsList->pstTacacsServer, 0, uiServerCnt * sizeof(TACACS_SERVER_ST));

        for (i = 0; i < uiServerCnt; i++) {
            uiLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);
            for (j = 0; j < uiLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(pstLeaf->schema->name, "name")) {
                    strncpy(pstTacacsList->pstTacacsServer[i].acName, lyd_get_value(pstLeaf), sizeof(pstTacacsList->pstTacacsServer[i].acName) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, "address")) {
                    strncpy(pstTacacsList->pstTacacsServer[i].acIp, lyd_get_value(pstLeaf), sizeof(pstTacacsList->pstTacacsServer[i].acIp) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, "port")) {
                    pstTacacsList->pstTacacsServer[i].uiPort = ((sr_node_term_t *)pstLeaf)->value.uint16;
                } else if (0 == strcmp(pstLeaf->schema->name, "shared-secret")) {
                    strncpy(pstTacacsList->pstTacacsServer[i].acKey, lyd_get_value(pstLeaf), sizeof(pstTacacsList->pstTacacsServer[i].acKey) - 1);
                }
            }
        }
    }

ERR_LABEL:
    if (pstNodeSet) {
        ly_set_free(pstNodeSet, NULL);
    }

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (rc != SR_ERR_OK) {
        if (pstTacacsList->pstTacacsServer) {
            free(pstTacacsList->pstTacacsServer);
            pstTacacsList->pstTacacsServer = NULL;
        }
    }

    return rc;
}
/* 400GZR requirement */
char *
DSC_ConvertRoleToString(int iRole)
{
    switch (iRole) {
    case READ_ONLY_ROLE:
        return "readonly";
    case READ_WRITE_ROLE:
        return "readwrite";
    case FULL_ROLE:
        return "full";
    default:
        return "unknown";

    }
}

int
DSC_ConvertRoleToInt(const char *pcRole, int *piRole)
{
    int rc = SR_ERR_OK;

    if (NULL == pcRole || NULL == piRole) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    if (0 == strcasecmp(pcRole, "readonly")) {
        *piRole = READ_ONLY_ROLE;
    } else if (0 == strcasecmp(pcRole, "readwrite")) {
        *piRole = READ_WRITE_ROLE;
    } else if (0 == strcasecmp(pcRole, "full")) {
        *piRole = FULL_ROLE;
    } else {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "Invalid role: %s", pcRole);
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

