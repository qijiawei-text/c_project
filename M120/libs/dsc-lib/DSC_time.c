/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_inc.h"



#define XPATH_MAX_LEN    (512)



ASSOCIATION_TYPE_EN
DSC_NtpTypeToEnum(char *pcType)
{
    if (0 == strcmp(NTP_TYPE_SERVER_STR, pcType)) {
        return ASSOCIATION_TYPE_SERVER;
    } else if (0 == strcmp(NTP_TYPE_PEER_STR, pcType)) {
        return ASSOCIATION_TYPE_PEER;
    } else if (0 == strcmp(NTP_TYPE_POOL_STR, pcType)) {
        return ASSOCIATION_TYPE_POOL;
    }

    return ASSOCIATION_TYPE_POOL;
}

char *
DSC_NtpTypeToString(ASSOCIATION_TYPE_EN enType, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enType) {
    case ASSOCIATION_TYPE_SERVER:
        strcpy(pcBuf, NTP_TYPE_SERVER_STR);
        break;
    case ASSOCIATION_TYPE_PEER:
        strcpy(pcBuf, NTP_TYPE_PEER_STR);
        break;
    case ASSOCIATION_TYPE_POOL:
        strcpy(pcBuf, NTP_TYPE_POOL_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

#ifdef OC_SYS_NTP
int
DSC_OC_GetNtpServerAddr(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "server", "address", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, NTP_SERVER_IP_MAX_LEN);
    sr_xpath_recover(&state);

    return 0;
}

int
DSC_OC_GetNtpServer(bool bOnlyConfig, NTP_SERVER_LIST_ST *pstServers)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    int i = 0, j = 0;

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "input is null!");
        return SR_ERR_INVAL_ARG;
    }

    if (false == bOnlyConfig) {
        sess = DSC_GetOperSession();
    }

    rc = get_items(sess, OCS_XPATH_NTP_ALL, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s", sr_strerror(rc));

        if (pstVal != NULL) {
            sr_free_values(pstVal, nCnt);
        }
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        return rc;
    }

    memset(pstServers->acSelectIp, 0, sizeof(pstServers->acSelectIp));
    strcpy(pstServers->acSelectIp, "127.0.0.1");

    j = -1;
    for (i = 0; i < nCnt; i++) {
        LOG_WRITE(EN_LOG_DEBUG, "xpath: %s", pstVal[i].xpath);
        if (strstr(pstVal[i].xpath, "]/address") != NULL) {
            j++;
            if (j >= NTP_SERVER_MAX_NUM) {
                break;
            }

            strncpy(pstServers->stNtpServer[j].acIp, pstVal[i].data.string_val, NTP_SERVER_IP_MAX_LEN - 1);
            LOG_WRITE(EN_LOG_DEBUG, "acIp: %s", pstServers->stNtpServer[j].acIp);
        } else if (strstr(pstVal[i].xpath, "/config/port") != NULL) {
            pstServers->stNtpServer[j].usPort = pstVal[i].data.uint16_val;
            LOG_WRITE(EN_LOG_DEBUG, "usPort: %u", pstServers->stNtpServer[j].usPort);
        } else if (strstr(pstVal[i].xpath, "/config/version") != NULL) {
            pstServers->stNtpServer[j].iVersion = pstVal[i].data.uint8_val;
            LOG_WRITE(EN_LOG_DEBUG, "iVersion: %u", pstVal[i].data.uint8_val);
        } else if (strstr(pstVal[i].xpath, "/config/iburst") != NULL) {
            pstServers->stNtpServer[j].bBurst = pstVal[i].data.bool_val;
            LOG_WRITE(EN_LOG_DEBUG, "bBurst: %d", pstServers->stNtpServer[j].bBurst);
        } else if (strstr(pstVal[i].xpath, "/config/prefer") != NULL) {
            pstServers->stNtpServer[j].bPrefer = pstVal[i].data.bool_val;
            LOG_WRITE(EN_LOG_DEBUG, "bPrefer: %d", pstServers->stNtpServer[j].bPrefer);
        } else if (strstr(pstVal[i].xpath, "/ntp/config/enabled") != NULL) {
            pstServers->bEnable = pstVal[i].data.bool_val;
            LOG_WRITE(EN_LOG_DEBUG, "bEnable: %d", pstServers->bEnable);
        } else if (strstr(pstVal[i].xpath, "/state/stratum") != NULL) {
            pstServers->stNtpServer[j].ucStratum = pstVal[i].data.uint8_val;
            LOG_WRITE(EN_LOG_DEBUG, "stratum: %u", pstVal[i].data.uint8_val);
        } else if (strstr(pstVal[i].xpath, "/ntp/state/ntp-source-address") != NULL) {
            strncpy(pstServers->acSourceAddr, pstVal[i].data.string_val, sizeof(pstServers->acSourceAddr) - 1);
            LOG_WRITE(EN_LOG_DEBUG, "acSourceAddr: %s", pstVal[i].data.string_val);
        } else if (strstr(pstVal[i].xpath, "/state/root-delay") != NULL) {
            pstServers->stNtpServer[j].ulRootDelay = pstVal[i].data.uint32_val;
            LOG_WRITE(EN_LOG_DEBUG, "root-delay: %u", pstVal[i].data.uint32_val);
        } else if (strstr(pstVal[i].xpath, "/state/root-dispersion") != NULL) {
            pstServers->stNtpServer[j].ullRootDisp = pstVal[i].data.uint64_val;
            LOG_WRITE(EN_LOG_DEBUG, "root-dispersion: %lu", pstVal[i].data.uint64_val);
        } else if (strstr(pstVal[i].xpath, "/state/offset") != NULL) {
            pstServers->stNtpServer[j].ullOffset = pstVal[i].data.uint64_val;
            LOG_WRITE(EN_LOG_DEBUG, "offset: %lu", pstVal[i].data.uint64_val);
        } else if (strstr(pstVal[i].xpath, "/state/poll-interval") != NULL) {
            pstServers->stNtpServer[j].ulPollInterval = pstVal[i].data.uint32_val;
            LOG_WRITE(EN_LOG_DEBUG, "poll-interval: %u", pstVal[i].data.uint32_val);
        } else if (strstr(pstVal[i].xpath, "/com-oplink-system-ext:ntp-server-selected") != NULL) {
            memset(pstServers->acSelectIp, 0, sizeof(pstServers->acSelectIp));
            strcpy(pstServers->acSelectIp, pstVal[i].data.string_val);
            LOG_WRITE(EN_LOG_DEBUG, "ntp-server-selected: %s", pstVal[i].data.string_val);
        } else if (strstr(pstVal[i].xpath, "/com-oplink-system-ext:ntp-status") != NULL) {
            strcpy(pstServers->acNtpState, pstVal[i].data.string_val);
            LOG_WRITE(EN_LOG_DEBUG, "ntp-status: %s", pstVal[i].data.string_val);
        }
    }

    pstServers->ulTotal = j + 1;

    LOG_WRITE(EN_LOG_DEBUG, "pstServers: %d", pstServers->ulTotal);

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_OC_AddNtpServer(NTP_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServer) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error pstNtpServerInfo is null!");
        goto ERR_LABEL;
    }

    /* server/udp/address */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_ADDRESS, pstServer->acIp);
    rc = set_string_item(sess, acXpath, pstServer->acIp, NTP_SERVER_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp address: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* port only support 123 */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_PORT, pstServer->acIp);
    rc = set_uint16_item(sess, acXpath, (uint16_t)pstServer->usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp port: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* AssociationType */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_TYPE, pstServer->acIp);
    rc = set_enum_item(sess, acXpath, NTP_TYPE_SERVER_STR);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server association type error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* version */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_VERSION, pstServer->acIp);
    rc = set_uint8_item(sess, acXpath, pstServer->iVersion);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server version error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* burst */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_IBURST, pstServer->acIp);
    rc = set_bool_item(sess, acXpath, pstServer->bBurst);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server burst error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* prefer */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_PREFER, pstServer->acIp);
    rc = set_bool_item(sess, acXpath, pstServer->bPrefer);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server prefer error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server DSC_Commit error %s", sr_strerror(rc));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Dsc add ntp server success!");

    }

ERR_LABEL:
    return rc;

}

int
DSC_OC_DeleteNtpServer(char *pcAddr)
{
    char xpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcAddr) {
        LOG_WRITE(EN_LOG_ERROR, "pcAddr is null.");
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, OCS_XPATH_NTP_SERVER, pcAddr);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_DeleteNtpServer xpath is : %s", xpath);

    return DSC_DelNode(sess, xpath);
}

int
DSC_OC_SetNtpEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    rc = set_bool_item(sess, OCS_XPATH_NTP_ENABLED_CFG, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_bool_item config ntp enable: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD connection DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_OC_SetNtpServerPrefer(char *pcAddr, bool bEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_PREFER, pcAddr);
    rc = set_bool_item(sess, acXpath, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_bool_item config ntp prefer: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetNtpServerPrefer DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_OC_SetNtpServerIburst(char *pcAddr, bool bIburst)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_IBURST, pcAddr);
    rc = set_bool_item(sess, acXpath, bIburst);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_bool_item config ntp prefer: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_OC_SetNtpServerIburst DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_OC_SetNtpServerVersion(char *pcAddr, int iVersion)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_NTP_SERVER_CONF_VERSION, pcAddr);
    rc = set_uint8_item(sess, acXpath, (uint8_t)iVersion);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_uint8_item config ntp version: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_OC_SetNtpServerVersion DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}


#else
int
DSC_GetNtpServerName(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "server", "name", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, NTP_SERVER_NAME_MAX_LEN);
    sr_xpath_recover(&state);

    if ((strcmp(pcKey, "1") != 0)
        && (strcmp(pcKey, "2") != 0)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetNtpServerName not support name : %s", pcKey);
        return SR_ERR_INVAL_ARG;
    }

    return 0;
}

int
DSC_GetNtpServer(NTP_SERVER_LIST_ST *pstServers)
{
    sr_data_t *pstSubtrees = NULL;
    size_t nServerCnt = 0;
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[32] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(sess, XPATH_NTP_SERVER_LIST, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        nServerCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_NTP_SERVER_LIST, &nodeSet);
        if (LY_SUCCESS == rc) {
            nServerCnt = nodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "sr_get_subtrees rc: %s, count: %zu", sr_strerror(rc), nServerCnt);

    if (nServerCnt > NTP_SERVER_MAX_NUM) {
        rc = SR_ERR_VALIDATION_FAILED;
        goto ERR_LABEL;
    }

    /* if there is one server at least */
    if (nServerCnt != 0) {
        for (i = 0; i < nServerCnt; i++) {
            ulLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < ulLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_NAME_NODE)) {
                    LOG_WRITE(EN_LOG_DEBUG, "name: %s", lyd_get_value(pstLeaf));
                    strncpy(pstServers->stNtpServer[i].acName, lyd_get_value(pstLeaf), NTP_SERVER_NAME_MAX_LEN);
                } else if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_ADDRESS_NODE)) {
                    LOG_WRITE(EN_LOG_DEBUG, "address: %s", lyd_get_value(pstLeaf));
                    strncpy(pstServers->stNtpServer[i].acIp, lyd_get_value(pstLeaf), NTP_SERVER_IP_MAX_LEN);
                } else if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_PORT_NODE)) {
                    LOG_WRITE(EN_LOG_DEBUG, "port: %d", ((sr_node_term_t *)pstLeaf)->value.uint16);
                    pstServers->stNtpServer[i].usPort = ((sr_node_term_t *)pstLeaf)->value.uint16;
                } else if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_ASSOCIATION_TYPE_NODE)) {
                    /*LOG_WRITE(EN_LOG_DEBUG, "association-type: %s", ((sr_node_term_t *)pstLeaf)->value.enm); */
                    /*pstServers->stNtpServer[i].enAssociationType = DSC_NtpTypeToEnum((char *)lyd_get_value(pstLeaf)); */
                } else if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_IBURST_NODE)) {
                    LOG_WRITE(EN_LOG_DEBUG, "iburst: %d", ((sr_node_term_t *)pstLeaf)->value.bln);
                    pstServers->stNtpServer[i].bBurst = ((sr_node_term_t *)pstLeaf)->value.bln;
                } else if (0 == strcmp(pstLeaf->schema->name, XPATH_NTP_SERVER_PREFER_NODE)) {
                    LOG_WRITE(EN_LOG_DEBUG, "prefer: %d", ((sr_node_term_t *)pstLeaf)->value.bln);
                    pstServers->stNtpServer[i].bPrefer = ((sr_node_term_t *)pstLeaf)->value.bln;
                }
            }
        }
    }

    pstServers->ulTotal = nServerCnt;
    LOG_WRITE(EN_LOG_DEBUG, "ulServerCnt: %d", pstServers->ulTotal);

    /* enable */
    rc = get_bool_item(sess, XPATH_NTP_ENABLED, &pstServers->bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_bool_item error enable: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "get_bool_item rc: %s, enable: %d", sr_strerror(rc), pstServers->bEnable);

ERR_LABEL:

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    if (pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    return rc;
}

int
DSC_AddNtpServer(NTP_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServer) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error pstNtpServerInfo is null!");
        goto ERR_LABEL;
    }

    /* server/udp/address */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_UDP_ADDR, pstServer->acName);
    rc = set_string_item(sess, acXpath, pstServer->acIp, NTP_SERVER_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* server/udp/port */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_UDP_PORT, pstServer->acName);
    rc = set_uint16_item(sess, acXpath, (uint16_t)pstServer->usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* server/AssociationType */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_ASSOCIATION_TYPE, pstServer->acName);
    rc = set_enum_item(sess, acXpath, NTP_TYPE_POOL_STR);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server association type error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* server/burst */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_IBURST, pstServer->acName);
    rc = set_bool_item(sess, acXpath, pstServer->bBurst);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server burst error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* server/prefer */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_PREFER, pstServer->acName);
    rc = set_bool_item(sess, acXpath, pstServer->bPrefer);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server prefer error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ntp server DSC_Commit error %s", sr_strerror(rc));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Dsc add ntp server success!");

    }

ERR_LABEL:
    return rc;

}

int
DSC_DeleteNtpServer(char *pcServerName)
{
    char xpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcServerName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, XPATH_NTP_SERVER, pcServerName);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_DeleteNtpServer xpath is : %s", xpath);

    return DSC_DelNode(sess, xpath);
}

int
DSC_SetNtpEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    rc = set_bool_item(sess, XPATH_NTP_ENABLED, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_bool_item config ntp enable: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD connection DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_SetNtpServerIP(char *pcServerName, char *pcIpAddr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char acXpath[XPATH_MAX_LEN] = {0};

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_UDP_ADDR, pcServerName);
    rc = set_string_item(sess, acXpath, pcIpAddr, NTP_SERVER_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetStringItem config ntp ipaddr: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetNtpServerIP DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_SetNtpServerPrefer(char *pcServerName, bool bEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NTP_SERVER_PREFER, pcServerName);
    rc = set_bool_item(sess, acXpath, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_bool_item config ntp prefer: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetNtpServerPrefer DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}
#endif









int
DSC_SetSystemTime(char *pcTime)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/current-datetime' leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_TIME_DATETIME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_TIME_DATETIME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcTime);
    /* input[0].data.string_val = "2016-12-10T11:11:11"; */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_TIME, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        rc = DSC_RpcSend(sess, RPC_XPATH_TIME, input, input_cnt, &output, &output_cnt);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_TIME, sr_strerror(rc));
        }
        goto ERR_LABEL;
    }

ERR_LABEL:
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);
    return rc;
}

int
DSC_SetTimezoneUtcOffset(int16_t sMinutes)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_int16_item(sess, XPATH_TIMEZONE_UTC_OFFSET, sMinutes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", XPATH_TIMEZONE_UTC_OFFSET, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTimezoneUtcOffset DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetTimezoneUtcOffset(int16_t *psMinutes)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    int16_t sMinutes = 0;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == psMinutes) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    rc = get_int16_item(sess, XPATH_TIMEZONE_UTC_OFFSET, &sMinutes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", XPATH_TIMEZONE_UTC_OFFSET, sr_strerror(rc));
        goto ERR_LABEL;
    }

    *psMinutes = sMinutes;

ERR_LABEL:
    return rc;
}

int
DSC_GetBootTime(char *pcBootTime, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pcBootTime) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        goto ERR_LABEL;
    }

    rc = get_string_item(sess, XPATH_TIME_BOOT_TIME, pcBootTime, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", XPATH_TIME_BOOT_TIME, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetSystemTime(char *pcTime, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pcTime) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        goto ERR_LABEL;
    }

    rc = get_string_item(sess, XPATH_TIME_CURRENT_TIME, pcTime, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", XPATH_TIME_CURRENT_TIME, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}











