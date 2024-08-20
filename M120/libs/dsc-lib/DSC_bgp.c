/***MODU+******************************************************************************************
* Copyright(C), 2021, Molex Tech.Co.,Ltd
* FileName    : dsc_bgp.c
* Description : Define the bgp control functions.
* History     :
*     [Author]      [Date]          [Version]        [Description]
* [1] Chun Ye       2021/06/05      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "DSC_common.h"
#include "DSC_log.h"
#include "DSC_bgp.h"

int
DSC_GetBgpNeighborKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = DSC_GetKeyValue(pcXpath, "neighbor", "neighbor-address", DEF_BUF_SIZE_32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_GetBgpPeerGroupKey(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = DSC_GetKeyValue(pcXpath, "peer-group", "peer-group-name", DEF_BUF_SIZE_32, pcKeyValue);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_GetBgpConfig(ST_BGP_CONFIG *pstBgpConfig)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t nValCount = 0;
    int j = -1;

    if (NULL == pstBgpConfig) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto cleanup;
    }

    /* clear bgp infomation.                                                    */
    memset(pstBgpConfig, 0x00, sizeof(ST_BGP_CONFIG));

    /* check running session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "Running session is null");
        goto cleanup;
    }

    /* Get as number.                                                           */
    rc = get_uint32_item(pstSess, DEF_BGP_XPATH_CONFIG_AS, &(pstBgpConfig->ulAs));
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_NOTICE, "Not config bgp");
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get as: %s error: %s", DEF_BGP_XPATH_CONFIG_AS, sr_strerror(rc));
        }
        goto cleanup;
    }

    /* Get router-id.                                                           */
    rc = get_string_item(pstSess, DEF_BGP_XPATH_CONFIG_ROUTER_ID, pstBgpConfig->acRouterId, sizeof(pstBgpConfig->acRouterId));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get router-id: %s error: %s", DEF_BGP_XPATH_CONFIG_ROUTER_ID, sr_strerror(rc));
    }

    /* get neighbor information.                                                */
    rc = get_items(pstSess, DEF_BGP_XPATH_NEIGHBORS "//*", &pstValues, &nValCount);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath);
        rc = SR_ERR_OK;
        goto cleanup;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    j = -1;
    for (int i = 0; i < nValCount; i++) {
#if 0
        sr_print_val(pstValues + i);
#endif
        if (strstr(pstValues[i].xpath, DEF_BGP_LEAF_CONFIG_NEIGHBOR_ADDR) != NULL) {
            j++;
            if (j >= DEF_BGP_NEIGHBOR_MAX_CNT) {
                break;
            }
            strncpy(pstBgpConfig->astNeighbor[j].acIpAddr, pstValues[i].data.string_val, DEF_BUF_SIZE_32 - 1);
        } else if (strstr(pstValues[i].xpath, DEF_BGP_LEAF_CONFIG_PEER_GROUP) != NULL) {
            strncpy(pstBgpConfig->astNeighbor[j].acPeerGroup, pstValues[i].data.string_val, DEF_BUF_SIZE_32 - 1);
        }
    }

    if (pstValues != NULL) {
        sr_free_values(pstValues, nValCount);
        pstValues = NULL;
    }

    /* get peer-group information.                                              */
    for (int i = 0; i < DEF_BGP_NEIGHBOR_MAX_CNT; i++) {
        if (0 == strlen(pstBgpConfig->astNeighbor[i].acPeerGroup)) {
            continue;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_CONFIG_PEER_AS, pstBgpConfig->astNeighbor[i].acPeerGroup);
        rc = get_uint32_item(pstSess, acXpath, &(pstBgpConfig->astNeighbor[i].ulRemoteAs));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Get as: %s error: %s", acXpath, sr_strerror(rc));
        }
    }

cleanup:
    if (pstValues != NULL) {
        sr_free_values(pstValues, nValCount);
        pstValues = NULL;
    }
    return rc;
}

int
DSC_SetBgpAs(uint32_t ulAsNumber)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    /* check  as-number                                                         */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto ERR_LABEL;
    }

    /* set as-number to xpath                                                   */
    rc = set_uint32_item(pstSess, DEF_BGP_XPATH_CONFIG_AS, ulAsNumber);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set as: %s, %u, error: %s", DEF_BGP_XPATH_CONFIG_AS, ulAsNumber, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* commit.                                                                  */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set AS error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetBgpRouterId(char *pcAddr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    /* check input parameter                                                    */
    if (NULL == pcAddr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto ERR_LABEL;
    }

    /* check running-session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto ERR_LABEL;
    }

    /* set value to xpath                                                       */
    rc = set_string_item(pstSess, DEF_BGP_XPATH_CONFIG_ROUTER_ID, pcAddr, strlen(pcAddr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set router-id: %s, %s, error: %s", DEF_BGP_XPATH_CONFIG_ROUTER_ID, pcAddr, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* commit.                                                                  */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set router-id error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

/* the peer-group-name need to be set, we just use the ip as the peer-group-name                  */

int
DSC_SetBgpNeighbor(char *pcAddr, uint32_t ulRemoteAs)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = NULL;

    /* check input parameter                                                    */
    if (NULL == pcAddr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto ERR_LABEL;
    }

    /* check running-session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_CONFIG_GROUP_NAME, pcAddr);
    rc = set_string_item(pstSess, acXpath, pcAddr, strlen(pcAddr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set peer-group-name: %s, %s, error: %s", acXpath, pcAddr, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_CONFIG_PEER_AS, pcAddr);
    rc = set_uint32_item(pstSess, acXpath, ulRemoteAs);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set peer-as: %s, %u, error: %s", acXpath, ulRemoteAs, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* commit peer-group-name                                                   */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set peer-group-name error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set neighbor-address                                                     */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_CONFIG_NEIGHBOR_ADDR, pcAddr);
    rc = set_string_item(pstSess, acXpath, pcAddr, strlen(pcAddr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set neighbor-address: %s, %s, error: %s", acXpath, pcAddr, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set peer-group                                                           */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_CONFIG_PEER_GROUP, pcAddr);
    rc = set_string_item(pstSess, acXpath, pcAddr, strlen(pcAddr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set peer-group: %s, %s, error: %s", acXpath, pcAddr, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* commit peer-group-name                                                   */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set peer-group-name error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_DelBgpAs(uint32_t ulAsNumber)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    uint32_t ulCurAsNum = 0;

    /* check running-session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto cleanup;
    }

    /* Get as number.                                                           */
    rc = get_uint32_item(pstSess, DEF_BGP_XPATH_CONFIG_AS, &ulCurAsNum);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get as: %s error: %s", DEF_BGP_XPATH_CONFIG_AS, sr_strerror(rc));
        goto cleanup;
    }

    if (ulCurAsNum != ulAsNumber) {
        LOG_WRITE(EN_LOG_ERROR, "AS number: %u NOT exiting, running is %u", ulAsNumber, ulCurAsNum);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /* delete neighbors node.                                                   */
    rc = sr_delete_item(pstSess, DEF_BGP_XPATH_NEIGHBORS, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete neighbor error: %s", sr_strerror(rc));
    }

    /* delete peer-group node.                                                  */
    rc = sr_delete_item(pstSess, DEF_BGP_XPATH_PEER_GROUPS, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete peer-groups error: %s", sr_strerror(rc));
    }

    /* delete as-number node.                                                   */
    rc = sr_delete_item(pstSess, DEF_BGP_XPATH_CONFIG_ROUTER_ID, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete router-id error: %s", sr_strerror(rc));
    }

    /* delete as-number node.                                                   */
    rc = sr_delete_item(pstSess, DEF_BGP_XPATH_CONFIG_AS, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete as error: %s", sr_strerror(rc));
    }

    /* commit.                                                                  */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete as error %s", sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_DelBgpRouterId(char *pcAddr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acRouterId[DEF_BUF_SIZE_64] = {0};

    if (NULL == pcAddr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto cleanup;
    }

    /* check running-session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto cleanup;
    }

    /* Get router-id.                                                           */
    memset(acRouterId, 0x00, sizeof(acRouterId));
    rc = get_string_item(pstSess, DEF_BGP_XPATH_CONFIG_ROUTER_ID, acRouterId, sizeof(acRouterId));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get router-id: %s error: %s", DEF_BGP_XPATH_CONFIG_ROUTER_ID, sr_strerror(rc));
        goto cleanup;
    }

    if (strcmp(acRouterId, pcAddr) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "loopback: %s NOT exiting, running is %s", acRouterId, pcAddr);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /* delete as-number node.                                                   */
    rc = sr_delete_item(pstSess, DEF_BGP_XPATH_CONFIG_ROUTER_ID, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete router-id error: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* commit.                                                                  */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Delete error %s", sr_strerror(rc));
        goto cleanup;
    }
cleanup:
    return rc;
}

int
DSC_DelBgpNeighbor(char *pcAddr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    /* check running-session                                                    */
    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Running session null");
        goto cleanup;
    }

    /* delete neighbor node.                                                    */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_NEIGHBORS "[neighbor-address='%s']", pcAddr);
    rc = sr_delete_item(pstSess, acXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete neighbor: %s error: %s", pcAddr, sr_strerror(rc));
        goto cleanup;
    }

    /* delete neighbor node.                                                    */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_BGP_XPATH_PEER_GROUPS "[peer-group-name='%s']", pcAddr);
    rc = sr_delete_item(pstSess, acXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete peer-group: %s error: %s", pcAddr, sr_strerror(rc));
        goto cleanup;
    }
    /* commit.                                                                  */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Delas error %s", sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}