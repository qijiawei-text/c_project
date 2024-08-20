/***MODU+******************************************************************************************
* FileName    : dsc_net.c
* Description : Define the network functions.
* History     :
*     [Author]          [Date]          [Version]        [Description]
* [1] Chun Ye           2022/11/05      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/
#include "DSC_inc.h"

/**************************************************************************************************/
/* definition of support ethernet name array, used to check the ethernet name in xpath            */
/**************************************************************************************************/
/* @ {                                                                                            */
static char m_aacSupportEthName[DEF_DSC_ETH_CNT][DEF_NETWORK_INTERFACE_NAME_MAX_LEN] = {
    "eth0",
    "eth1",
    "eth2",
    "",
    "eth4",
    "inband",
    "",
    ""
};
/* @ }                                                                                            */

/**************************************************************************************************/
/* ONLY eth4 has default gateway, when BGP/OSPF is running on eth0                                */
/**************************************************************************************************/
/* @ {                                                                                            */
/* #define __only_eth4_has_ipv4_default_gw_                                                       */
/* @ }                                                                                            */


/***FUNC+******************************************************************************************/
/* Name   : __BuildEthList                                                                        */
/* Descrp : build the ST_NETWORK_IP_INTERFACE array by sr_get_items data from db                  */
/* Input  : pstVal  -- A pointer of sr_get_items value                                            */
/*        : nCnt -- the count of pstVal                                                           */
/* Output : pstIpInfo -- A pointer of network ethernet information                                */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_INVAL_ARG :                                                           */
/***FUNC-******************************************************************************************/
static int
__BuildEthList(const sr_val_t *pstVal, const size_t nCnt, ST_NETWORK_IP_INTERFACE *pstIpInfo)
{
    int rc = SR_ERR_OK;

    /* i is the loop node in pstVal                                             */
    int i = 0;

    /* j is the index of network ethernet                                       */
    int j = 0;

    /* check parameter                                                          */
    if (NULL == pstVal || NULL == pstIpInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p", pstVal, pstIpInfo);
        return SR_ERR_INVAL_ARG;
    }

    /* loop the pstVal and get the ethernet information                         */
    j = -1;
    for (i = 0; i < nCnt; i++) {
        /* Based on yang-model, the key of name always be the first node        */
        if (strstr(pstVal[i].xpath, "/name") != NULL) {
            j++;
            /* If the index >= max, just break and write log, but return succ.  */
            if (j >= DEF_DSC_ETH_CNT) {
                LOG_WRITE(EN_LOG_ERROR, "Ethernet num %d out of range: %d", j, DEF_DSC_ETH_CNT);
                break;
            }
            strncpy(pstIpInfo->stEthInterface[j].acInterfaceName, pstVal[i].data.string_val, DEF_NETWORK_INTERFACE_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_ETH_ENABLE) != NULL) {
            pstIpInfo->stEthInterface[j].bEnable = pstVal[i].data.bool_val;
        } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_MAC_ADDR) != NULL) {
            strncpy(pstIpInfo->stEthInterface[j].acPhyMacAddress, pstVal[i].data.string_val, DEF_NET_MAC_ADDR_LEN - 1);
        } else if (strstr(pstVal[i].xpath, DEF_NET_XPATH_IPV4_KEY) != NULL) {
            /* Find the key '/ipv4/' in xpath                                   */
            if (strstr(pstVal[i].xpath, DEF_NET_LEAF_IP_ORIGIN) != NULL) {
                DSC_EthOriginStrToValue(pstVal[i].data.enum_val, &pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.enOriginType);
                if (EN_NETWORK_ORIGIN_DHCP == pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.enOriginType) {
                    pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.enDhcpType = EN_NETWORK_DHCP_CLIENT;
                }
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_IP_ADDR) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stStaticCfg.acIpAddress, pstVal[i].data.string_val, DEF_IPV4_ADDR_MAX_LEN - 1);
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_NETMASK) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stStaticCfg.acNetMask, pstVal[i].data.string_val, DEF_IPV4_ADDR_MAX_LEN - 1);
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_GATEWAY) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stStaticCfg.acDefaultGateway, pstVal[i].data.string_val, DEF_IPV4_ADDR_MAX_LEN - 1);
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_CLIENT_TIMEOUT) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.Dhcp.stDhcpClient.ulTimeOut = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_CLIETN_LEASE) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.Dhcp.stDhcpClient.ulLease = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.Dhcp.stDhcpServer.ulLeaseTime = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_START_IP) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.Dhcp.stDhcpServer.acPoolStartIP, pstVal[i].data.string_val, DEF_IP_ADDR_MAX_LEN - 1);
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.enDhcpType = EN_NETWORK_DHCP_SERVER;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_END_IP) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv4Cfg.stDhcpCfg.Dhcp.stDhcpServer.acPoolEndIP, pstVal[i].data.string_val, DEF_IP_ADDR_MAX_LEN - 1);
            }
        } else if (strstr(pstVal[i].xpath, DEF_NET_XPATH_IPV6_KEY) != NULL) {
            /* Find the key '/ipv6/' in xpath                                   */
            if (strstr(pstVal[i].xpath, DEF_NET_LEAF_IP_ORIGIN_V6) != NULL) {
                DSC_EthOriginStrToValue(pstVal[i].data.enum_val, &pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.enOriginType);
                if (EN_NETWORK_ORIGIN_DHCP == pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.enOriginType) {
                    pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.enDhcpType = EN_NETWORK_DHCP_CLIENT;
                }
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_IP_ADDR_V6) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stStaticCfg.acIpAddress, pstVal[i].data.string_val, DEF_IPV6_ADDR_MAX_LEN - 1);
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_PREFIX_V6) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stStaticCfg.ucPrefixLen = pstVal[i].data.uint8_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_GATEWAY_V6) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stStaticCfg.acDefaultGateway, pstVal[i].data.string_val, DEF_IPV6_ADDR_MAX_LEN - 1);
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_CLIETN_TIMEOUT_V6) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.Dhcp.stDhcpClient.ulTimeOut = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_CLIETN_LEASE_V6) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.Dhcp.stDhcpClient.ulLease = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME_V6) != NULL) {
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.Dhcp.stDhcpServer.ulLeaseTime = pstVal[i].data.uint32_val;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_START_IP_V6) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.Dhcp.stDhcpServer.acPoolStartIP, pstVal[i].data.string_val, DEF_IP_ADDR_MAX_LEN - 1);
                pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.enDhcpType = EN_NETWORK_DHCP_SERVER;
            } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_DHCP_SERVER_END_IP_V6) != NULL) {
                strncpy(pstIpInfo->stEthInterface[j].stInterfaceCfg.stIpv6Cfg.stDhcpCfg.Dhcp.stDhcpServer.acPoolEndIP, pstVal[i].data.string_val, DEF_IP_ADDR_MAX_LEN - 1);
            }
        } else if (strstr(pstVal[i].xpath, "/type") != NULL) {
            strncpy(pstIpInfo->stEthInterface[j].acType, pstVal[i].data.identityref_val, DEF_NET_TYPE_LEN - 1);
        }
    }

    pstIpInfo->ulTotal = j + 1;

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __DelNetNode                                                                          */
/* Descrp : Delete the network node in db. This function is used in exchange static or dhcp       */
/* Input  : pcXpath  -- A pointer of path to be deleted                                           */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
static int
__DelNetNode(char *pcXpath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto cleanup;
    }

    /* Check parameter                                                          */
    if (NULL == pcXpath) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcXpath);
        goto cleanup;
    }

    /* Using the flag SR_EDIT_STRICT, the identified element must exist         */
    rc = sr_delete_item(pstSess, pcXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Delete: %s error: %s", pcXpath, sr_strerror(rc));
        goto cleanup;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", pcXpath, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;

}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthName                                                                        */
/* Descrp : Get the ethernet name by xpath and key                                                */
/* Input  : pcXpath  -- A pointer of path                                                         */
/* Output : pcOutEthName  -- A pointer of ethernet name                                           */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthName(char *pcXpath, char *pcOutEthName)
{
    char *pcTmp = NULL;
    sr_xpath_ctx_t stState = {0};
    int i = 0;

    /* Check parameter                                                          */
    if (NULL == pcXpath || NULL == pcOutEthName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcXpath, pcOutEthName);
        goto cleanup;
    }

    /* Get key value from the xpath                                             */
    pcTmp = sr_xpath_key_value(pcXpath, "interface", "name", &stState);
    if (NULL == pcTmp) {
        LOG_WRITE(EN_LOG_ERROR, "Parse the xpath error: %s", pcXpath);
        goto cleanup;
    }
    strncpy(pcOutEthName, pcTmp, DEF_NETWORK_INTERFACE_NAME_MAX_LEN);
    sr_xpath_recover(&stState);

    /* Check the support eth list in device                                     */
    for (i = 0; i < DEF_DSC_ETH_CNT; i++) {
        if (0 == strcmp(pcOutEthName, m_aacSupportEthName[i])) {
            break;
        }
    }

    if (i >= DEF_DSC_ETH_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "Not support name : %s, %s", pcXpath, pcOutEthName);
        goto cleanup;
    }

    return SR_ERR_OK;
cleanup:
    return SR_ERR_INVAL_ARG;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_FindIpCfgByName                                                                   */
/* Descrp : Get the ethernet name by xpath and key                                                */
/* Input  : pcName  -- A pointer of ethernet name                                                 */
/* Input  : pstIpCfg  -- A pointer of network ip configuration                                    */
/* Output : pstEthIpCfg  -- A pointer of network ethernet configuration                           */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_NOT_FOUND :                                                           */
/***FUNC-******************************************************************************************/
int
DSC_FindIpCfgByName(const char *pcEthName, const ST_NETWORK_IP_INTERFACE *pstIpCfg, ST_NETWORK_ETH_INTERFACE *pstEthIpCfg)
{
    int i = 0;

    if (NULL == pcEthName) {
        return SR_ERR_NOT_FOUND;
    }

    for (i = 0; i < pstIpCfg->ulTotal; i++) {
        if (0 == strcmp(pcEthName, pstIpCfg->stEthInterface[i].acInterfaceName)) {
            memcpy(pstEthIpCfg, &pstIpCfg->stEthInterface[i], sizeof(ST_NETWORK_ETH_INTERFACE));
            return SR_ERR_OK;
        }
    }

    return SR_ERR_NOT_FOUND;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthEnable                                                                      */
/* Descrp : Set eth enable/disable                                                                */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : bEnable  -- ethernet enable/disable                                                   */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthEnable(char *pcEthName, bool bEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto cleanup;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto cleanup;
    }

    /* Set the type, the type in yang model is mandatory                        */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_ETH_TYPE, pcEthName);
    rc = set_identityref_item(pstSess, acXpath, "org-openroadm-interfaces:ip", strlen("org-openroadm-interfaces:ip"));
    if (SR_ERR_OK != rc) {
        sr_discard_changes(pstSess);
        LOG_WRITE(EN_LOG_ERROR, "set_identityref_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    /* Set the enable/disable                                                   */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_ETH_ENABLE_ALL, pcEthName);
    rc = set_bool_item(pstSess, acXpath, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_bool_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", acXpath, sr_strerror(rc));
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthEnable                                                                      */
/* Descrp : Get eth enable/disable                                                                */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : bEnable -- A pointer of ethernet enable/disable value                                 */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthEnable(char *pcEthName, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto cleanup;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_ETH_ENABLE_ALL, pcEthName);
    rc = get_bool_item(pstSess, acXpath, pbEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get enable: %s error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthIpv4Static                                                                  */
/* Descrp : Get ethernet ipv4 configuration                                                       */
/* Input  : pstSess  -- A pointer of session                                                      */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : piOriginType -- A pointer of ethernet ipv4/ipv6                                       */
/* Output : pcIpAddr -- A pointer of config ip value                                              */
/* Output : pcNetMask -- A pointer of config netmask value                                        */
/* Output : pcGateWay -- A pointer of config gateway value                                        */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthIpv4Static(sr_session_ctx_t *pstSess, char *pcEthName,
                     int *piOriginType, char *pcIpAddr, char *pcNetMask, char *pcGateWay)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acIpAddr[DEF_IPV4_ADDR_MAX_LEN] = {0};
    char acNetMask[DEF_IPV4_ADDR_MAX_LEN] = {0};
    char acGateWay[DEF_IPV4_ADDR_MAX_LEN] = {0};
    char acOriginType[DEF_BUF_SIZE_64] = {0};
    EN_NETWORK_IP_ADDRESS_ORIGIN enOrigin = EN_NETWORK_ORIGIN_STATIC;

    /* Check input parameter                                                    */
    if (NULL == pcEthName ||
        NULL == piOriginType ||
        NULL == pcIpAddr ||
        NULL == pcNetMask ||
        NULL == pcGateWay ) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p, 0x%p, 0x%p, 0x%p",
                  pcEthName, piOriginType, pcIpAddr, pcNetMask, pcGateWay);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        pstSess = DSC_GetOperSession();
    }

    /* Get origin value, static/dhcp                                            */
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_ALL, pcEthName);
    rc = get_enum_item(pstSess, acXpath, acOriginType, sizeof(acOriginType) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get origin: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Origin string to enum                                                    */
    rc = DSC_EthOriginStrToValue(acOriginType, &enOrigin);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Origin value: %s error: %s", acOriginType, sr_strerror(rc));
        goto ERR_LABEL;
    }
    *piOriginType = enOrigin;

    /* Check static value                                                       */
    if (EN_NETWORK_ORIGIN_STATIC == enOrigin) {
        memset(acIpAddr, 0x00, sizeof(acIpAddr));
        memset(acNetMask, 0x00, sizeof(acNetMask));
        memset(acGateWay, 0x00, sizeof(acGateWay));

        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_IP_ADDR_ALL, pcEthName);
        rc = get_string_item(pstSess, acXpath, acIpAddr, sizeof(acIpAddr) - 1);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get ip: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_NETMASK_ALL, pcEthName);
        rc = get_string_item(pstSess, acXpath, acNetMask, sizeof(acNetMask) - 1);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get netmask: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, pcEthName);
        rc = get_string_item(pstSess, acXpath, acGateWay, sizeof(acGateWay) - 1);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get gateway: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        memcpy(pcIpAddr, acIpAddr, sizeof(acIpAddr));
        memcpy(pcNetMask, acNetMask, sizeof(acNetMask));
        memcpy(pcGateWay, acGateWay, sizeof(acGateWay));
    } else {
        /* If the origin value is dhcp, just return                             */
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthIpv4Static                                                                  */
/* Descrp : Set ethernet ipv4 static                                                              */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pstIpv4Cfg -- A pointer of ipv4 config                                                */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthIpv4Static(char *pcEthName, ST_NETWORK_IPV4_STATIC *pstIpv4Cfg)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acUserLog[DEF_BUF_SIZE_256] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName || NULL == pstIpv4Cfg) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pstIpv4Cfg);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(DEF_NET_DEFAULT_GATEWAY_ON_ETH, pcEthName)) {
        rc = UTIL_ChkIpv4EthNetCfg(pstIpv4Cfg->acIpAddress, pstIpv4Cfg->acNetMask, pstIpv4Cfg->acDefaultGateway);
    } else {
        rc = UTIL_ChkIpv4EthNetCfg(pstIpv4Cfg->acIpAddress, pstIpv4Cfg->acNetMask, NULL);
    }

    if (SR_ERR_OK != rc) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid %s ipv4 config. ip: %s, netmask: %s, gateway: %s", pcEthName, pstIpv4Cfg->acIpAddress, pstIpv4Cfg->acNetMask, pstIpv4Cfg->acDefaultGateway);
        goto ERR_LABEL;
    }

    /* Set origin type first.                                                   */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_ALL, pcEthName);
    rc = set_enum_item(pstSess, acXpath, "static");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set origin: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Set IP address                                                           */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_IP_ADDR_ALL, pcEthName);
    rc = set_string_item(pstSess, acXpath, pstIpv4Cfg->acIpAddress, sizeof(pstIpv4Cfg->acIpAddress));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set ip: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Set netmask                                                              */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_NETMASK_ALL, pcEthName);
    rc = set_string_item(pstSess, acXpath, pstIpv4Cfg->acNetMask, sizeof(pstIpv4Cfg->acNetMask));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set netmask: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Record usr log                                                           */
    snprintf(acUserLog, sizeof(acUserLog), "%s,%s,%s", pcEthName, pstIpv4Cfg->acIpAddress, pstIpv4Cfg->acNetMask);

    /* When eth0 BGP/OSPF enable, the eth0 did NOT need gateway.                */
#ifdef __only_eth4_has_ipv4_default_gw_
    if (0 == strcmp("eth4", pcEthName)) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pstIpv4Cfg->acDefaultGateway, sizeof(pstIpv4Cfg->acDefaultGateway));
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set gateway: %s error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        snprintf(acUserLog + strlen(acUserLog), sizeof(acUserLog) - strlen(acUserLog), ",%s", pstIpv4Cfg->acDefaultGateway);
    }
#else
    if (0 == strcasecmp(DEF_NET_DEFAULT_GATEWAY_ON_ETH, pcEthName)) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pstIpv4Cfg->acDefaultGateway, sizeof(pstIpv4Cfg->acDefaultGateway));
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set gateway: %s error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        snprintf(acUserLog + strlen(acUserLog), sizeof(acUserLog) - strlen(acUserLog), ",%s", pstIpv4Cfg->acDefaultGateway);
    }

#endif

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", "set ipv4", sr_strerror(rc));
    } else {
        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_SET_NETWORK_IPV4, acUserLog);
    }

ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthIpv4Static                                                                  */
/* Descrp : Set ethernet ipv4 static                                                              */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pstIpv6Cfg -- A pointer of ipv6 config                                                */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthIpv6Static(char *pcEthName, ST_NETWORK_IPV6_STATIC *pstIpv6Cfg)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName || NULL == pstIpv6Cfg) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pstIpv6Cfg);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set origin type first.                                                   */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_V6_ALL, pcEthName);
    rc = set_enum_item(pstSess, acXpath, DEF_NET_IP_ORIGIN_STATIC_STR);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set eth ipv6 origin error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Set ipv6 ip                                                              */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_IP_ADDR_V6_ALL, pcEthName);
    rc = set_string_item(pstSess, acXpath, pstIpv6Cfg->acIpAddress, sizeof(pstIpv6Cfg->acIpAddress));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Set ipv6 gateway.                                                        */
    if (0 == strcasecmp(DEF_NET_DEFAULT_GATEWAY_ON_ETH, pcEthName)) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_GATEWAY_V6_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pstIpv6Cfg->acDefaultGateway, sizeof(pstIpv6Cfg->acDefaultGateway));
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    /* Set ipv6 prefix                                                          */
    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_PREFIX_LEN_V6_ALL, pcEthName);
    rc = set_uint8_item(pstSess, acXpath, pstIpv6Cfg->ucPrefixLen);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", "set ipv6", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthDhcpCliTimeout                                                              */
/* Descrp : Set ethernet ipv4/ipv6 dhcp client timeout                                            */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Input  : ulTimeout  -- dhcp client timeout value, unit s                                       */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthDhcpCliTimeout(char *pcEthName, EN_NETWORK_TYPE enType, uint32_t ulTimeout)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set ethernet dhcp client timeout                                         */
    if (EN_NETWORK_TYPE_IPV4 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_CLIENT_TIMEOUT_ALL, pcEthName);
        rc = set_uint32_item(pstSess, acXpath, ulTimeout);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp client timeout: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if (EN_NETWORK_TYPE_IPV6 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_CLIETN_TIMEOUT_V6_ALL, pcEthName);
        rc = set_uint32_item(pstSess, acXpath, ulTimeout);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp client timeout: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input type: %d error", enType);
        goto ERR_LABEL;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit dhcp client timeout: %d error: %s", enType, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthDhcpCliLease                                                                */
/* Descrp : Set ethernet ipv4/ipv6 dhcp client lease                                              */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Input  : ulLease  -- dhcp client timeout lease                                                 */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthDhcpCliLease(char *pcEthName, EN_NETWORK_TYPE enType, uint32_t ulLease)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set ethernet dhcp client lease                                           */
    if (EN_NETWORK_TYPE_IPV4 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_CLIETN_LEASE_ALL, pcEthName);
        rc = set_uint32_item(pstSess, acXpath, ulLease);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp client lease: %s error: %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if (EN_NETWORK_TYPE_IPV6 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_CLIETN_LEASE_V6_ALL, pcEthName);
        rc = set_uint32_item(pstSess, acXpath, ulLease);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp client lease: %s error: %s", acXpath, sr_strerror(rc));

            goto ERR_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input type: %d error", enType);
        goto ERR_LABEL;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit dhcp client lease: %d error: %s", enType, sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}


/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthDhclientTimeout                                                             */
/* Descrp : Set ethernet ipv4/ipv6 dhcp client lease                                              */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthDhcpCli(char *pcEthName, EN_NETWORK_TYPE enType)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set ethernet dhcp origin                                                 */
    if (EN_NETWORK_TYPE_IPV4 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_ALL, pcEthName);
        rc = set_enum_item(pstSess, acXpath, DEF_NET_IP_ORIGIN_DHCP_STR);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set eth origin: %s error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if (EN_NETWORK_TYPE_IPV6 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_V6_ALL, pcEthName);
        rc = set_enum_item(pstSess, acXpath, DEF_NET_IP_ORIGIN_DHCP_STR);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set eth origin: %s error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input type: %d error", enType);
        goto ERR_LABEL;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit origin: %d error: %s", enType, sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthIpv6Linklayer                                                               */
/* Descrp : Set ethernet ipv6 link layer                                                          */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthIpv6Linklayer(char *pcEthName, EN_NETWORK_IP_ADDRESS_ORIGIN enOrigin)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    if (EN_NETWORK_ORIGIN_DHCP == enOrigin) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_V6_ALL, pcEthName);
        rc = set_enum_item(pstSess, acXpath, DEF_NET_IP_ORIGIN_DHCP_STR);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set eth link layer: %s  error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if (EN_NETWORK_ORIGIN_LINK_LAYER == enOrigin) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_IP_ORIGIN_V6_ALL, pcEthName);
        rc = set_enum_item(pstSess, acXpath, DEF_NET_IP_ORIGIN_LINK_LAYER_STR);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set eth link layer: %s  error %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Input origin: %d error", enOrigin);
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", acXpath, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthDhcpSrvDisable                                                              */
/* Descrp : Set ethernet ipv4/ipv6 dhcp server disable                                            */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthDhcpSrvDisable(char *pcEthName, EN_NETWORK_TYPE enType)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set ethernet dhcp server                                                 */
    if (EN_NETWORK_TYPE_IPV4 == enType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_ALL, pcEthName);
    } else if (EN_NETWORK_TYPE_IPV6 == enType) {
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_V6_ALL, pcEthName);
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input type: %d error", enType);
        goto ERR_LABEL;
    }

    return __DelNetNode(acXpath);
ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetEthDhcpSrvIpRange                                                              */
/* Descrp : Set ethernet ipv4/ipv6 dhcp server ip range                                           */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Input  : enType  -- ipv4/ipv6 type                                                             */
/* Input  : pcStartIp  -- A pointer of start string                                               */
/* Input  : nStartStrLen  -- len of start string                                                  */
/* Input  : pcEndIp  -- A pointer of end string                                                   */
/* Input  : nEndStrLen  -- len of end string                                                      */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetEthDhcpSrvIpRange(char *pcEthName, EN_NETWORK_TYPE enType,
                         char *pcStartIp, size_t nStartStrLen, char *pcEndIp, size_t nEndStrLen)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Set ethernet dhcp ip range                                               */
    if (EN_NETWORK_TYPE_IPV4 == enType) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_START_IP_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pcStartIp, nStartStrLen);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp ip range: %s %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_END_IP_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pcEndIp, nEndStrLen);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp ip range: %s %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if (EN_NETWORK_TYPE_IPV6 == enType) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_START_IP_V6_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pcStartIp, nStartStrLen);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp ip range: %s %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }

        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, DEF_NET_XPATH_DHCP_SERVER_END_IP_V6_ALL, pcEthName);
        rc = set_string_item(pstSess, acXpath, pcEndIp, nEndStrLen);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set dhcp ip range: %s %s", acXpath, sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input type: %d error", enType);
        goto ERR_LABEL;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit ip range: %d error: %s", enType, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthIpv4State                                                                   */
/* Descrp : Get ethernet ipv4 real value                                                          */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pstIpv4State -- A pointer of ipv4 real value                                          */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthIpv4State(char *pcEthName, ST_NETWORK_IPV4_STATIC *pstIpv4State)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName || NULL == pstIpv4State) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pstIpv4State);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_IP_ADDR_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pstIpv4State->acIpAddress, sizeof(pstIpv4State->acIpAddress));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get ip: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_NETMASK_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pstIpv4State->acNetMask, sizeof(pstIpv4State->acNetMask));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get netmask: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_GATEWAY_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pstIpv4State->acDefaultGateway, sizeof(pstIpv4State->acDefaultGateway));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get gw: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthIpv6State                                                                   */
/* Descrp : Get ethernet ipv6 real value                                                          */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pstIpv6State -- A pointer of ipv6 real value                                          */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthIpv6State(char *pcEthName, ST_NETWORK_IPV6_STATIC *pstIpv6State)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName || NULL == pstIpv6State) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pstIpv6State);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_IP_ADDR_V6_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pstIpv6State->acIpAddress, sizeof(pstIpv6State->acIpAddress));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get ip: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_PREFIX_LEN_V6_ALL, pcEthName);
    rc = get_uint8_item(pstSess, acXpath, &pstIpv6State->ucPrefixLen);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get prefix: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_GATEWAY_V6_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pstIpv6State->acDefaultGateway, sizeof(pstIpv6State->acDefaultGateway));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get gw: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthMacAddrState                                                                */
/* Descrp : Get ethernet MAC real value                                                           */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pcMacAddr -- A pointer of MAC real value                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthMacAddrState(char *pcEthName, char *pcMacAddr)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName || NULL == pcMacAddr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pcMacAddr);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_CURRENT_MAC_ADDR_ALL, pcEthName);
    rc = get_string_item(pstSess, acXpath, pcMacAddr, DEF_NET_MAC_ADDR_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get MAC: %s error %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthMacAddrState                                                                */
/* Descrp : Get ethernet MAC real value                                                           */
/* Input  : pcEthName  -- A pointer of ethernet name                                              */
/* Output : pcMacAddr -- A pointer of MAC real value                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_EthOriginStrToValue(char *pcStr, EN_NETWORK_IP_ADDRESS_ORIGIN *penOrigin)
{
    int rc = SR_ERR_OK;

    if (NULL == pcStr || NULL == penOrigin) {
        LOG_WRITE(EN_LOG_ERROR, "input null: 0x%p, 0x%p", pcStr, penOrigin);
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strcmp(pcStr, DEF_NET_IP_ORIGIN_OTHER_STR)) {
        *penOrigin = EN_NETWORK_ORIGIN_OTHER;
    } else if (0 == strcmp(pcStr, DEF_NET_IP_ORIGIN_STATIC_STR)) {
        *penOrigin = EN_NETWORK_ORIGIN_STATIC;
    } else if (0 == strcmp(pcStr, DEF_NET_IP_ORIGIN_DHCP_STR)) {
        *penOrigin = EN_NETWORK_ORIGIN_DHCP;
    } else if (0 == strcmp(pcStr, DEF_NET_IP_ORIGIN_LINK_LAYER_STR)) {
        *penOrigin = EN_NETWORK_ORIGIN_LINK_LAYER;
    } else if (0 == strcmp(pcStr, DEF_NET_IP_ORIGIN_RANDOM_STR)) {
        *penOrigin = EN_NETWORK_ORIGIN_RANDOM;
    } else {
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthMacCfg                                                                      */
/* Descrp : Get ethernet MAC config in db                                                         */
/* Input  : NONE                                                                                  */
/* Output : pstMacInfo -- A pointer of MAC configuration                                          */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthMacCfg(ST_NETWORK_MAC_INTERFACE *pstMacInfo)
{
    int rc = SR_ERR_OK, i = 0;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pstMacInfo) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pstMacInfo);
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto ERR_LABEL;
    }

    /* Get from com-oplink-device                                               */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, DEF_NET_XPATH_MACADDR_ALL_I);
    rc = sr_get_items(pstSess, acXpath, 0, 0, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item xpath: %s", acXpath);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s, %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Fill the data.                                                           */
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, DEF_NET_LEAF_NAME) != NULL) {
            strcpy(pstMacInfo->stEthMacAddr[pstMacInfo->ulTotal].acInterfaceName, pstVal[i].data.string_val);
        } else if (strstr(pstVal[i].xpath, DEF_NET_LEAF_MAC_ADDR) != NULL) {
            strcpy(pstMacInfo->stEthMacAddr[pstMacInfo->ulTotal].acPhyMacAddress, pstVal[i].data.string_val);
            pstMacInfo->ulTotal++;
        }
    }

    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
        pstVal = NULL;
    }
    return rc;

ERR_LABEL:
    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
        pstVal = NULL;
    }
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetEthMacCfg                                                                      */
/* Descrp : Get ethernet  config in db                                                            */
/* Input  : NONE                                                                                  */
/* Output : pstIpInfo -- A pointer of ethernet configuration                                      */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetEthIpCfg(ST_NETWORK_IP_INTERFACE *pstIpInfo)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pstIpInfo) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pstIpInfo);
        goto cleanup;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto cleanup;
    }

    /* Get from org-openroadm-device                                            */
    rc = sr_get_items(pstSess, DEF_NET_XPATH_INTERFACE_ALL, 0, 0, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        pstIpInfo->ulTotal = 0;
        rc = SR_ERR_OK;
        goto cleanup;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get eth: %s error %s", DEF_NET_XPATH_INTERFACE_ALL, sr_strerror(rc));
        pstIpInfo->ulTotal = 0;
        goto cleanup;
    }

    /* Build the ethernet list by pstVal                                        */
    rc = __BuildEthList(pstVal, nCnt, pstIpInfo);

cleanup:
    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
        pstVal = NULL;
    }
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CheckEthExist                                                                     */
/* Descrp : Check the ethernet exist or NOT                                                       */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                       */
/*              @arg false :                                                                      */
/***FUNC-******************************************************************************************/
bool
DSC_CheckEthExist(char *pcEthName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        return false;
    }

    /* Check session                                                            */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        return false;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_ETH_NAME, pcEthName);
    rc = get_string_item(pstSess, acXpath, pcEthName, strlen(pcEthName));
    if (rc != SR_ERR_OK) {
        return false;
    }

    return true;
}

int
DSC_DelEthIpv6Cfg(char *pcEthName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcEthName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcEthName);
        goto END_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']/ipv6", pcEthName);

    rc = sr_delete_item(pstSess, acXpath, SR_EDIT_DEFAULT);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item(%s): %s", pcEthName, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "%s DSC_Commit error %s", __func__, sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_GetEthIpv6OriginStr(char *pcEthName, char *pcOrigin, size_t nLen)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acOriginType[DEF_BUF_SIZE_64] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcEthName || NULL == pcOrigin) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p, 0x%p", pcEthName, pcOrigin);
        goto END_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), DEF_NET_XPATH_IP_ORIGIN_V6_ALL, pcEthName);
    rc = get_enum_item(pstSess, acXpath, acOriginType, sizeof(acOriginType) - 1);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_enum_item(%s):%s", pcEthName, sr_strerror(rc));
        goto END_LABEL;
    }

    snprintf(pcOrigin, nLen, "%s", acOriginType);

END_LABEL:
    return rc;
}

int
DSC_SetInbandGateway(char *pcGateway, size_t nLen)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcGateway) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = UTIL_ChkIpV4Addr(pcGateway);
    if (rc != OPLK_OK) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, DEF_NET_INBAND_KEY);
    rc = set_string_item(pstSess, acXpath, pcGateway, nLen);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set gateway: %s error %s", acXpath, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
        goto END_LABEL;
    }


END_LABEL:
    return rc;
}

int
DSC_DelInbandGateway()
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    memset(acXpath, 0x00, sizeof(acXpath));
    sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, DEF_NET_INBAND_KEY);
    rc = sr_delete_item(pstSess, acXpath, SR_EDIT_DEFAULT);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item(%s): %s", acXpath, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

int
DSC_GetInbandGateway(char *pcGateway, size_t nLen)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    char acIp[64];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    memset(acIp, 0x00, sizeof(acIp));
    sprintf(acXpath, DEF_NET_XPATH_GATEWAY_ALL, DEF_NET_INBAND_KEY);
    rc = get_string_item_log_trace(sess, acXpath, acIp, sizeof(acIp));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_TRACE, "Error by get_string_item_log_trace: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memcpy(pcGateway, acIp, (nLen < sizeof(acIp) ? nLen : sizeof(acIp)));

ERR_LABEL:
    return rc;
}