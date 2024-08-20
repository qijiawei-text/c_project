/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_inc.h"



#define XPATH_MAX_LEN    (512)


int
DSC_GetDnsServerName(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "server", "name", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, DNS_NAME_MAX_LEN);
    sr_xpath_recover(&state);

    return 0;
}

int
DSC_AddDnsServer(DNS_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    char xpath[XPATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServer) {
        LOG_WRITE(EN_LOG_ERROR, "pstServer is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* server/udp-and-tcp/address */
    sprintf(xpath, XPATH_DNS_SERVER_UDPANDTCP_ADDR, pstServer->acName);
    rc = set_string_item(sess, xpath, pstServer->acIp, DNS_NAME_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* server/udp-and-tcp/port */
    sprintf(xpath, XPATH_DNS_SERVER_UDPANDTCP_PORT, pstServer->acName);
    rc = set_uint16_item(sess, xpath, (uint16_t)pstServer->usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udpandtcp port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "add dns server error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "add dns server success");
    }

    return rc;
}

int
DSC_DelDnsServer(char *pcServerName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[XPATH_MAX_LEN] = {0};

    if (NULL == pcServerName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, XPATH_DNS_SERVER, pcServerName);

    return DSC_DelNode(sess, xpath);
}

int
DSC_AddDnsSearch(char *pcDomain)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcDomain) {
        LOG_WRITE(EN_LOG_ERROR, "input is null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, XPATH_DNS_SEARCH, pcDomain, DNS_SERACH_DOMAIN_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-dns-search: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ADD dns search DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_DelDnsSearch(char *pcDomain)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[XPATH_MAX_LEN] = {0};

    if (NULL == pcDomain) {
        LOG_WRITE(EN_LOG_ERROR, "input is null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    sprintf(xpath, XPATH_DNS_SEARCH_ENTRY, pcDomain);
    rc = DSC_DelNode(sess, xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete dns search error: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

int
DSC_SetDnsOption(uint32_t ulTimeout, uint32_t ulRetryTimes)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* options/timeout */
    rc = set_uint8_item(sess, XPATH_DNS_OPTIONS_TIMEOUT, (uint8_t)ulTimeout);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-options timeout: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    /* options/attempts */
    rc = set_uint8_item(sess, XPATH_DNS_OPTIONS_ATTEMPTS, (uint8_t)ulRetryTimes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-options attempts: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetDnsOption DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_GetDnsServers(DNS_SERVER_LIST_ST *pstServers)
{
    sr_data_t *pstSubtrees = NULL;
    size_t nServerCnt = 0;
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[20];
    sr_val_t *pstVal = NULL;
    size_t nValCnt = 0;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(sess, XPATH_DNS_SERVER_LIST, 0, SR_OPER_DEFAULT, &pstSubtrees);
    LOG_WRITE(EN_LOG_DEBUG, "get_subtrees rc: %s, count: %zu", sr_strerror(rc), nServerCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        nServerCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_DNS_SERVER_LIST, &pstNodeSet);
        if (LY_SUCCESS == rc) {
            nServerCnt = pstNodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "get_subtrees rc: %s, count: %zu", sr_strerror(rc), nServerCnt);

    if (nServerCnt > DNS_SERVER_NUM) {
        rc = SR_ERR_VALIDATION_FAILED;
        goto ERR_LABEL;
    }

    /* if there is one server at least */
    if (nServerCnt != 0) {
        for (i = 0; i < nServerCnt; i++) {
            ulLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < ulLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(apstLeaf[j]->schema->name, "name")) {
                    strncpy(pstServers->stDnsServer[i].acName, lyd_get_value(pstLeaf), DNS_NAME_MAX_LEN);
                } else if (0 == strcmp(apstLeaf[j]->schema->name, "address")) {
                    strncpy(pstServers->stDnsServer[i].acIp, lyd_get_value(pstLeaf), DNS_IP_MAX_LEN);
                } else if (0 == strcmp(apstLeaf[j]->schema->name, "port")) {
                    pstServers->stDnsServer[i].usPort = (uint16_t)atoi(lyd_get_value(pstLeaf));
                }
            }
        }
    }

    pstServers->ulTotal = nServerCnt;

    /* options/timeout */
    rc = get_uint8_item(sess, XPATH_DNS_OPTIONS_TIMEOUT, (uint8_t *)&pstServers->ulTimeout);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetUint8Item(NULL error-options timeout: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DSC_GetUint8Item(NULL rc: %s, timeout: %d", sr_strerror(rc), pstServers->ulTimeout);

    /* options/attempts */
    rc = get_uint8_item(sess, XPATH_DNS_OPTIONS_ATTEMPTS, (uint8_t *)&pstServers->ulRetryTimes);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetUint8Item(NULL error-options timeout: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DSC_GetUint8Item(NULL rc: %s, RetryTimes: %d", sr_strerror(rc), pstServers->ulRetryTimes);

    rc = get_items(sess, XPATH_DNS_SEARCH, &pstVal, &nValCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        nValCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_items dns-search: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (nValCnt > DNS_SERACH_DOMAIN_NUM) {
        rc = SR_ERR_VALIDATION_FAILED;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "get_items rc: %s, stDnsSearch: %zu", sr_strerror(rc), nValCnt);
    for (i = 0; i < nValCnt; i++) {
        strcpy(pstServers->stDnsSearch[i], pstVal[i].data.string_val);
        LOG_WRITE(EN_LOG_DEBUG, "stDnsSearch: %s", pstServers->stDnsSearch[i]);
    }

    if (pstVal != NULL) {
        sr_free_values(pstVal, nValCnt);
        pstVal = NULL;
    }

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



