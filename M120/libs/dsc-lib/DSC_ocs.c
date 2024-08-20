/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_ocs.h"

int
DSC_OcsGetSourcePort(char *pcXpath, char *pcPort)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "connection", "port-name", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcPort, key_tmp, OPLK_NAME_MAX_LEN - 1);
    sr_xpath_recover(&state);

    return 0;
}

int
DSC_OcsPortNumber(char *pcPort, char *prefix)
{
    return atoi(strstr(pcPort, prefix) + strlen(prefix));
}

#if 0
OCS_MODE_EN
DSC_GetOcsModeEn(char *pcMode)
{
    OCS_MODE_EN enMode = OCS_MODE_CNT;

    if (0 == strncmp(pcMode, OCS_STR_MODE_SWAP, strlen(OCS_STR_MODE_SWAP))) {
        enMode = OCS_MODE_SWAP;
    } else if (0 == strncmp(pcMode, OCS_STR_MODE_BLOCK, strlen(OCS_STR_MODE_BLOCK))) {
        enMode = OCS_MODE_BLOCK;
    } else if (0 == strncmp(pcMode, OCS_STR_MODE_NATIVE, strlen(OCS_STR_MODE_NATIVE))) {
        enMode = OCS_MODE_NATIVE;
    }

    return enMode;
}

char *
DSC_GetOcsModeStr(char *pcStr, OCS_MODE_EN enVal)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (enVal) {
    case OCS_MODE_BLOCK:
        strcpy(pcStr, OCS_STR_MODE_BLOCK);
        break;
    case OCS_MODE_NATIVE:
        strcpy(pcStr, OCS_STR_MODE_NATIVE);
        break;
    default:
        strcpy(pcStr, OCS_STR_MODE_SWAP);
        break;
    }

    return pcStr;
}
#endif

int
DSC_OcsGetConnections(OCS_CONNECTIONS_ST *pstConns, bool isCfg)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstVal = NULL;
    sr_val_t *pstValTmp = NULL;
    size_t nCnt = 0;
    size_t i = 0, j = 0;
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    OCS_CONNECTION_ST stConn;


    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    memset(&stConns, 0, sizeof(stConns));

    pstSess = isCfg ? DSC_GetRunningSession() : DSC_GetOperSession();

    rc = get_items(pstSess, OCS_XPATH_CONNECTION_ALL, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_NOTICE, "Not found item: %s, xpath: %s", sr_strerror(rc), OCS_XPATH_CONNECTION_ALL);
            pstConns->uiConnCnt = 0;
            return rc = SR_ERR_OK;
        }
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        pstValTmp = pstVal + i;

        if (SR_LIST_T == pstValTmp->type || SR_CONTAINER_T == pstValTmp->type || SR_CONTAINER_PRESENCE_T == pstValTmp->type) {
            continue;
        }
        if (NULL != strstr(pstValTmp->xpath, "]/index")) {
            if (stConns.uiConnCnt++ > OCS_CONNECTIONS_MAX) {
                rc = SR_ERR_INVAL_ARG;
                goto END_LABEL;
            }

            pstConn = stConns.astConnection + (stConns.uiConnCnt - 1);
            pstConn->uiIndex = pstValTmp->data.uint32_val;
            continue;
        }

        if (NULL != strstr(pstValTmp->xpath, isCfg ? "/config" : "/state")) {
            if (sr_xpath_node_name_eq(pstValTmp->xpath, OCS_SOURCE_NODE)) {
                snprintf(pstConn->acSPort, sizeof(pstConn->acSPort), "%s", pstValTmp->data.string_val);
            } else if (sr_xpath_node_name_eq(pstValTmp->xpath, OCS_DEST_NODE)) {
                snprintf(pstConn->acDPort, sizeof(pstConn->acDPort), "%s", pstValTmp->data.string_val);
            }
        }
    }

    /* sort by index */
    if (stConns.uiConnCnt > 0) {
        for (i = 0; i < stConns.uiConnCnt - 1; i++) {
            for (j = 0; j < stConns.uiConnCnt - i - 1; j++) {
                if (stConns.astConnection[j].uiIndex > stConns.astConnection[j + 1].uiIndex) {
                    memset(&stConn, 0, sizeof(stConn));
                    memcpy(&stConn, stConns.astConnection + j, sizeof(stConn));
                    memcpy(stConns.astConnection + j, stConns.astConnection + j + 1, sizeof(stConn));
                    memcpy(stConns.astConnection + j + 1, &stConn, sizeof(stConn));
                }
            }
        }
    }

    memcpy(pstConns, &stConns, sizeof(stConns));

END_LABEL:

    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
    }

    return rc;
}

static int
__OcsSetConnection(sr_session_ctx_t *pstRunSess, OCS_CONNECTION_ST *pstConn, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstConn) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    pstSess = (NULL == pstRunSess) ? DSC_GetRunningSession() : pstRunSess;

    /* config/index */
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCS_CONFIG_INDEX_XPATH, pstConn->uiIndex);
    rc = set_uint32_item(pstSess, acSelectXpath, pstConn->uiIndex);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OCS config/index error %s", sr_strerror(rc));
        goto cleanup;
    }

    /* source port */
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCS_CONFIG_SOURCE_XPATH, pstConn->uiIndex);
    rc = set_string_item(pstSess, acSelectXpath, pstConn->acSPort, strlen(pstConn->acSPort));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OCS source port error %s", sr_strerror(rc));
        goto cleanup;
    }

    /* destination port */
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCS_CONFIG_DEST_XPATH, pstConn->uiIndex);
    rc = set_string_item(pstSess, acSelectXpath, pstConn->acDPort, strlen(pstConn->acDPort));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OCS dest port error %s", sr_strerror(rc));
        goto cleanup;
    }

    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

cleanup:
    return rc;
}

#if 0
OCS_MODE_EN
DSC_OcsGetSwitchMode()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    OCS_MODE_EN enMode = OCS_MODE_CNT;
    char acTmp[64];

    pstSess = DSC_GetRunningSession();
    rc = get_enum_item(pstSess, OCS_XPATH_MODE, acTmp, sizeof(acTmp) - 1);
    char *tmp = strstr(acTmp, ":");
    tmp = tmp + 1;
    LOG_WRITE(EN_LOG_NOTICE, "OCS Mode get from dsc is %s", tmp);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OCS mode error %s", sr_strerror(rc));
        return rc;
    }
    enMode = DSC_GetOcsModeEn(tmp);
    return enMode;
}

int
DSC_OcsSetSwitchMode(OCS_MODE_EN enMode)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acTmp[64];

    pstSess = DSC_GetRunningSession();
    rc = set_enum_item(pstSess, OCS_XPATH_MODE, DSC_GetOcsModeStr(acTmp, enMode));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set OCS mode error %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}
#endif

/** Molex defination:
**  add/update connections based on connections
**/
int
DSC_OcsSetConnections(const OCS_CONNECTIONS_ST *pstConns)
{
    int rc = SR_ERR_OK;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0;

    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        rc = __OcsSetConnection(NULL, pstConn, ((i + 1) == pstConns->uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsSetConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, pstConns->uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            goto cleanup;
        }
    }

cleanup:
    return rc;
}

/** MSFT requiremnt from proto defination:
**  SAFE will cause the SetCrossConnects rpc to fail if any of the ports in any of the cross connects are already used in another cross connect
**/
int
DSC_OcsAddConnections(const OCS_CONNECTIONS_ST *pstConns)
{
    int rc = SR_ERR_OK;
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0, j = 0;

    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stConns, 0, sizeof(stConns));
    rc = DSC_OcsGetConnections(&stConns, true);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* check */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        for (j = 0; j < stConns.uiConnCnt; j++) {
            if (0 == strcmp(pstConn->acSPort, stConns.astConnection[j].acSPort)) {
                LOG_WRITE(EN_LOG_ERROR, "source %s already existed", stConns.astConnection[j].acSPort);
                break;
            }

            if (0 == strcmp(pstConn->acDPort, stConns.astConnection[j].acDPort)) {
                LOG_WRITE(EN_LOG_ERROR, "dest %s already existed", stConns.astConnection[j].acDPort);
                break;
            }
        }

        if (j < stConns.uiConnCnt) {
            rc = SR_ERR_INVAL_ARG;
            goto cleanup;
        }
    }

    /* add */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        rc = __OcsSetConnection(NULL, pstConn, ((i + 1) == pstConns->uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsSetConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, pstConns->uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

cleanup:
    return rc;
}

/** MSFT requiremnt from proto defination:
** UPDATE will cause the SetCrossConnects rpc to set the cross connect even if the ports in the request are currently being used by another cross connect
**/

int
DSC_OcsUpdateConnections(const OCS_CONNECTIONS_ST *pstConns)
{
    int rc = SR_ERR_OK;
    /* sr_session_ctx_t *pstSess = NULL; */
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0, j = 0;

    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stConns, 0, sizeof(stConns));
    rc = DSC_OcsGetConnections(&stConns, true);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* check */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        for (j = 0; j < stConns.uiConnCnt; j++) {
            if (0 == strcmp(pstConn->acSPort, stConns.astConnection[j].acSPort)) {

                break;
            }
        }

        if (j >= stConns.uiConnCnt) {
            LOG_WRITE(EN_LOG_ERROR, "Not found source %s", pstConn->acSPort);
            rc = SR_ERR_INVAL_ARG;
            goto cleanup;
        }
    }

    /* update */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        rc = __OcsSetConnection(NULL, pstConn, ((i + 1) == pstConns->uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsSetConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, pstConns->uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

cleanup:
    return rc;
}

static int
__OcsDeleteConnection(uint32_t uiIndex, bool bStrict, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acXpath[DEF_BUF_SIZE_512];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    snprintf(acXpath, sizeof(acXpath), OCS_XPATH_CONN, uiIndex);
    rc = sr_delete_item(pstSess, acXpath, bStrict ? SR_EDIT_STRICT : SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item %s: %s", acXpath, sr_strerror(rc));
        goto END_LABEL;
    }
    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    }

END_LABEL:

    return rc;
}

/** MSFT requiremnt from proto defination:
** OVERWRITE will wipe the existing cross connects and set the newly requested ones
**/
int
DSC_OcsReplaceConnections(const OCS_CONNECTIONS_ST *pstConns)
{
    int rc = SR_ERR_OK;
    /* sr_session_ctx_t *pstSess = NULL; */
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0;
    uint32_t j = 0;

    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (0 == pstConns->uiConnCnt) {
        LOG_WRITE(EN_LOG_ERROR, "pstConns->uiConnCnt: %d", pstConns->uiConnCnt);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stConns, 0, sizeof(stConns));
    rc = DSC_OcsGetConnections(&stConns, true);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* delete existed connections */
    for (i = 0; i < stConns.uiConnCnt; i++) {
        pstConn = stConns.astConnection + i;

        for (j = 0; j < pstConns->uiConnCnt; j++) {
            if (pstConn->uiIndex == pstConns->astConnection[j].uiIndex) {
                break;
            }
        }

        if (j < pstConns->uiConnCnt) {
            continue;
        }

        rc = __OcsDeleteConnection(pstConn->uiIndex, true, false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsDeleteConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, stConns.uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

    /* add new connections */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        rc = __OcsSetConnection(NULL, pstConn, ((i + 1) == pstConns->uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsSetConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, pstConns->uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

cleanup:

    return rc;
}

int
DSC_OcsDeleteConnections(const OCS_CONNECTIONS_ST *pstConns)
{
    int rc = SR_ERR_OK;
    /* sr_session_ctx_t *pstSess = NULL; */
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0, j = 0;

    if (NULL == pstConns) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stConns, 0, sizeof(stConns));
    rc = DSC_OcsGetConnections(&stConns, true);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* check if the deleting connections already exist */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)(pstConns->astConnection + i);
        for (j = 0; j < stConns.uiConnCnt; j++) {
            if ((0 == strcmp(pstConn->acSPort, stConns.astConnection[j].acSPort)) &&
                (0 == strcmp(pstConn->acDPort, stConns.astConnection[j].acDPort))) {
                break;
            }
        }

        if (j >= stConns.uiConnCnt) {
            LOG_WRITE(EN_LOG_ERROR, "Not found source %s and dest %s", pstConn->acSPort, pstConn->acDPort);
            rc = SR_ERR_INVAL_ARG;
            goto cleanup;
        }
    }

    /* delete existed connections */
    for (i = 0; i < pstConns->uiConnCnt; i++) {
        pstConn = (OCS_CONNECTION_ST *)pstConns->astConnection + i;
        rc = __OcsDeleteConnection(pstConn->uiIndex, true, ((i + 1) == pstConns->uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsDeleteConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, pstConns->uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

cleanup:
    return rc;
}

int
DSC_OcsDeleteAllConnections()
{
    int rc = SR_ERR_OK;
    OCS_CONNECTIONS_ST stConns;
    OCS_CONNECTION_ST *pstConn = NULL;
    uint32_t i = 0;

    memset(&stConns, 0, sizeof(stConns));
    rc = DSC_OcsGetConnections(&stConns, true);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* delete existed connections */
    for (i = 0; i < stConns.uiConnCnt; i++) {
        pstConn = stConns.astConnection + i;
        rc = __OcsDeleteConnection(pstConn->uiIndex, true, ((i + 1) == stConns.uiConnCnt) ? true : false);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OcsDeleteConnection. [%d/%d] uiIndex:%d, acSPort:%s, acDPort:%s", i + 1, stConns.uiConnCnt,
                      pstConn->uiIndex, pstConn->acSPort, pstConn->acDPort);
            sr_discard_changes(DSC_GetRunningSession());
            goto cleanup;
        }
    }

cleanup:
    return rc;
}

int
DSC_OcsDeleteConnectionByIndex(uint32_t uiIndex)
{

    return __OcsDeleteConnection(uiIndex, true, true);
}

