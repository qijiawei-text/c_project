/**
 * Copyright (c) Oplink Communications, LLC
 */

#include <ctype.h>

#include "DSC_system.h"
#include "DSC_shelf.h"
#include "DSC_circuitpack.h"

static CLIENT_SERVER_ST m_astClientServer[MAX_CTRL] = {{0}};

#if CONFIG_DSC_API

int
DSC_GetConfigInfo(DEVICE_CONFIG_ST *pstDevCfg, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    ANONYMOUS_ST config_maps[] = {
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_NODE_ID, SR_STRING_T, pstDevCfg->acNodeId),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_NODE_NO, SR_UINT32_T, (char *)(&pstDevCfg->u32NodeNumber)),
    };

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pstDevCfg || size < sizeof(DEVICE_CONFIG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstDevCfg, 0, size);
    rc = DSC_GetItems(sess, DEVICE_XPATH_CONFIG_ALL, config_maps, sizeof(config_maps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetNodeId(char *pcNodeId, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == pcNodeId) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* check node-id by org-openroadm-resource-types.yang define
       typedef node-id-type {
            type string {
              length "7..63";
              pattern '([a-zA-Z][a-zA-Z0-9-]{5,61}[a-zA-Z0-9])' {
                error-message
                  "A node-id must be 7 to 63 characters in length.
                   A node-id can contain letters, numbers, and hyphens.
                   The first character must be a letter.
                   The last character must be a letter or number.";
              }
            }
            description
              "Globally unique identifier for a device.";
       }
     */
    rc = set_string_item(sess, DEVICE_XPATH_NODE_ID, pcNodeId, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_NODE_ID, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetNodeId DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetContact(char *pcStr, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == pcStr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, DEVICE_XPATH_CONTACT, pcStr, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_CONTACT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetHostname(char *pcStr, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == pcStr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, DEVICE_XPATH_HOSTNAME, pcStr, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_HOSTNAME, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetLocation(char *pcStr, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == pcStr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, DEVICE_XPATH_LOCATION, pcStr, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_LOCATION, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetNodeNumber(uint32_t u32NodeNumber)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(sess, DEVICE_XPATH_NODE_NO, u32NodeNumber);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_uint32_item: %s %s", DEVICE_XPATH_NODE_NO, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetNodeNumber DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetTemperatureThr(double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_decimal64_item(sess, DEVICE_XPATH_TEMP_HIGH_THR, dHighThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_TEMP_HIGH_THR, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_decimal64_item(sess, DEVICE_XPATH_TEMP_LOW_THR, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", DEVICE_XPATH_TEMP_LOW_THR, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTemperatureThr DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetNodeSwVersion(char *pcSwVersion, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    if (NULL == pcSwVersion) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

#ifdef __OPER__
    pstSess = DSC_GetOperSession();
#else
    pstSess = DSC_GetRunningSession();
#endif
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "sess is null %s", sr_strerror(rc));
        goto cleanup;
    }

#ifdef __OPER__
    rc = set_string_item(pstSess, OOD_XPATH_SW_VER, pcSwVersion, nSize);
#else
    rc = set_string_item(pstSess, COD_XPATH_SW_VER, pcSwVersion, nSize);
#endif
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", OOD_XPATH_SW_VER, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_SetBuildSwVersion(char *pcSwVersion, size_t nSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    if (NULL == pcSwVersion) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "sess is null %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = set_string_item(pstSess, OOD_XPATH_SW_VER_BUILD, pcSwVersion, nSize);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", OOD_XPATH_SW_VER_BUILD, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

#endif


#if SERVICE_DSC_API
SERVICE_EN
DSC_ServiceToEnum(char *pcService)
{
    if (0 == strcasecmp(SERVICE_SNMP_STR, pcService)) {
        return SERVICE_SNMP;
    } else if (0 == strcasecmp(SERVICE_NETCONF_STR, pcService)) {
        return SERVICE_NETCONF;
    } else if (0 == strcasecmp(SERVICE_WEB_STR, pcService)) {
        return SERVICE_WEB;
    } else if (0 == strcasecmp(SERVICE_REST_STR, pcService)) {
        return SERVICE_REST;
    } else if (0 == strcasecmp(SERVICE_GNMI_STR, pcService)) {
        return SERVICE_GNMI;
    } else if (0 == strcasecmp(SERVICE_BGP_STR, pcService)) {
        return SERVICE_BGP;
    } else if (0 == strcasecmp(SERVICE_OSPF_STR, pcService)) {
        return SERVICE_OSPF;
    } else if (0 == strcasecmp(SERVICE_LLDP_STR, pcService)) {
        return SERVICE_LLDP;
    } else if (0 == strcasecmp(SERVICE_SFTP_STR, pcService)) {
        return SERVICE_SFTP;
    } else if (0 == strcasecmp(SERVICE_PROXYARP_STR, pcService)) {
        return SERVICE_PROXYARP;
    } else if (0 == strcmp(SERVICE_KLISH_SERVER_STR, pcService)) {
        return SERVICE_KLISH;
    }

    return SERVICE_NONE;
}

char *
DSC_ServiceToString(SERVICE_EN pcService, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (pcService) {
    case SERVICE_SNMP:
        strcpy(pcBuf, SERVICE_SNMP_STR);
        break;
    case SERVICE_NETCONF:
        strcpy(pcBuf, SERVICE_NETCONF_STR);
        break;
    case SERVICE_WEB:
        strcpy(pcBuf, SERVICE_WEB_STR);
        break;
    case SERVICE_REST:
        strcpy(pcBuf, SERVICE_REST_STR);
        break;
    case SERVICE_SFTP:
        strcpy(pcBuf, SERVICE_SFTP_STR);
        break;
    case SERVICE_BGP:
        strcpy(pcBuf, SERVICE_BGP_STR);
        break;
    case SERVICE_GNMI:
        strcpy(pcBuf, SERVICE_GNMI_STR);
        break;
    case SERVICE_LLDP:
        strcpy(pcBuf, SERVICE_LLDP_STR);
        break;
    case SERVICE_OSPF:
        strcpy(pcBuf, SERVICE_OSPF_STR);
        break;
    case SERVICE_PROXYARP:
        strcpy(pcBuf, SERVICE_PROXYARP_STR);
        break;
    case SERVICE_KLISH:
        strcpy(pcBuf, SERVICE_KLISH_SERVER_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

bool
DSC_GetServiceState(SERVICE_EN enService)
{
    int rc = SR_ERR_OK;
    char acBuf[32];
    char acXpath[OPLK_PATH_MAX_LEN];
    bool bState = false;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    DSC_ServiceToString(enService, acBuf);

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, SERVICE_XPATH_ENABLE, acBuf);
    rc = get_bool_item(sess, acXpath, &bState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_bool_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return bState;
}

int
DSC_SetServiceEnable(SERVICE_EN enService, bool bState)
{
    int rc = SR_ERR_OK;
    char acBuf[32];
    char acXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    DSC_ServiceToString(enService, acBuf);

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, SERVICE_XPATH_ENABLE, acBuf);
    rc = set_bool_item(sess, acXpath, bState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}
#endif

#if CLIENT_DSC_API

CLIENT_EN
DSC_ClientToEnum(char *pcClient)
{
    if (0 == strcasecmp(CLIENT_SNMPTRAP_STR, pcClient)) {
        return CLIENT_SNMPTRAP;
    }

    return CLIENT_NONE;
}

char *
DSC_ClientToString(CLIENT_EN enClient, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enClient) {
    case CLIENT_SNMPTRAP:
        strcpy(pcBuf, CLIENT_SNMPTRAP_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}



ROLE_EN
DSC_RoleToEnum(char *pcRole)
{
    if (0 == strcmp(ROLE_EQUAL_STR, pcRole)) {
        return ROLE_EQUAL;
    } else if (0 == strcmp(ROLE_MASTER_STR, pcRole)) {
        return ROLE_MASTER;
    } else if (0 == strcmp(ROLE_SLAVE_STR, pcRole)) {
        return ROLE_SLAVE;
    }

    return ROLE_EQUAL;
}

char *
DSC_RoleToString(ROLE_EN enRole, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enRole) {
    case ROLE_EQUAL:
        strcpy(pcBuf, ROLE_EQUAL_STR);
        break;
    case ROLE_MASTER:
        strcpy(pcBuf, ROLE_MASTER_STR);
        break;
    case ROLE_SLAVE:
        strcpy(pcBuf, ROLE_SLAVE_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

HOST_TYPE_EN
DSC_GetHostType(char *pcHost)
{
    int i = 0;

    if (pcHost == NULL) {
        return HOST_TYPE_UNKOWN;
    }

    if (strlen(pcHost) == 0) {
        return HOST_TYPE_UNKOWN;
    }

    if ((strstr(pcHost, ":") != NULL)
        || (strstr(pcHost, "::") != NULL)) {
        return HOST_TYPE_IPV6;
    }

    for (i = 0; i < strlen(pcHost); i++) {
        if ((pcHost[i] != '.')
            && ((pcHost[i] < '0') || (pcHost[i] > '9'))) {
            return HOST_TYPE_DOMAIN;
        }
    }

    return HOST_TYPE_IPV4;
}

int
DSC_GetClientServerInfo(CLIENT_EN enClient, CLIENT_SERVER_ST **ppstServerInfo, size_t *count)
{
    int rc = SR_ERR_OK, i = 0, j = 0;
    sr_val_t *val = NULL;
    size_t cnt = 0;
    CLIENT_SERVER_ST *pstClientServer = m_astClientServer;
    char acBuf[32];
    char acXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    DSC_ClientToString(enClient, acBuf);

    memset(pstClientServer, 0, sizeof(m_astClientServer));
    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CLIENT_XPATH_SERVERS_ALL, acBuf);
    rc = get_items(sess, acXpath, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, SERVER_ID) != NULL) {
            j++;
            pstClientServer[j].u32ServerId = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, SERVER_ROLE) != NULL) {
            pstClientServer[j].enRole = DSC_RoleToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, SERVER_IP) != NULL) {
            memcpy(pstClientServer[j].acServerIp, val[i].data.string_val, sizeof(pstClientServer[j].acServerIp));
        }
    }
    sr_free_values(val, cnt);

    *count = j + 1;
    *ppstServerInfo = pstClientServer;
    return rc;

ERR_LABEL:
    if (val != NULL)
        sr_free_values(val, cnt);
    *count = 0;
    return rc;
}

int
DSC_AddClientServerIp(CLIENT_EN enClient, int uiId, char *pcIpAddress)
{
    int rc = SR_ERR_OK;
    char acBuf[32];
    char acXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    DSC_ClientToString(enClient, acBuf);

    /* set serverip */
    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CLIENT_XPATH_SERVERS_IP, acBuf, uiId);
    rc = set_string_item(sess, acXpath, pcIpAddress, CLIENT_NTP_SERVER_HOST_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add client server ip error %s", sr_strerror(rc));
        LOG_WRITE(EN_LOG_ERROR, "Add client server ip: %s", pcIpAddress);
        goto ERR_LABEL;
    }

    /* set server role */
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CLIENT_XPATH_SERVERS_ROLE, acBuf, uiId);
    rc = set_enum_item(sess, acXpath, ROLE_EQUAL_STR);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add client server role error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add client server DSC_Commit error %s", sr_strerror(rc));
    } /* else {
         LOG_WRITE(EN_LOG_NOTICE, "Dsc add client server success %d", rc);
         }*/

ERR_LABEL:
    return rc;

}

int
DSC_DeleteClientServerIp(CLIENT_EN enClient, int uiId)
{
    int rc = SR_ERR_OK;
    char acBuf[32];
    char acXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    DSC_ClientToString(enClient, acBuf);

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CLIENT_XPATH_SERVERS, acBuf, uiId);
    rc = sr_delete_item(sess, acXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Delete client server DSC_Commit error %s", sr_strerror(rc));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Dsc delete client server success %d", rc);
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetOfConfig(OF_CONFIG_ST *pstOfConfig, size_t size)
{
    int rc = SR_ERR_OK, i = 0;
    CLIENT_SERVER_ST *pstClientServer = NULL;
    size_t iCount = 0;

    if (NULL == pstOfConfig || size < sizeof(OF_CONFIG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "sess or intput is null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetClientServerInfo(CLIENT_OPENFLOW, &pstClientServer, &iCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get client server info : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    pstOfConfig->cOnlineCtrl = iCount;
    for (i = 0; i < iCount; i++) {
        sprintf(pstOfConfig->astOfCtrl[i].acCtrlId, "%d", pstClientServer[i].u32ServerId);
        DSC_RoleToString(pstClientServer[i].enRole, pstOfConfig->astOfCtrl[i].acRole);
        strcpy(pstOfConfig->astOfCtrl[i].acProtocol, OF_PROTOCO_TCP);
        strcpy(pstOfConfig->astOfCtrl[i].acCtrlIp, pstClientServer[i].acServerIp);
        pstOfConfig->astOfCtrl[i].u16CtrlPort = OF_PORT_DEF;
    }

    return rc;

ERR_LABEL:

    pstOfConfig->cOnlineCtrl = 0;

    return rc;
}


#endif

#if STATUS_DSC_API

SYS_STATE_EN
DSC_SysStateToEnum(char *pcSysState)
{
    if (0 == strncmp(pcSysState, SYS_STATE_IN_SERVICE_STR, strlen(SYS_STATE_IN_SERVICE_STR))) {
        return SYS_STATE_IN_SERVICE;
    } else if (0 == strncmp(pcSysState, SYS_STATE_OUT_OF_SERVICE_STR, strlen(SYS_STATE_OUT_OF_SERVICE_STR))) {
        return SYS_STATE_OUT_OF_SERVICE;
    } else if (0 == strncmp(pcSysState, SYS_STATE_DEGRADED_STR, strlen(SYS_STATE_DEGRADED_STR))) {
        return SYS_STATE_DEGRADED;
    }

    return SYS_STATE_IN_SERVICE;

}

char *
DSC_SysStateToString(SYS_STATE_EN enSysState, char *pcBuf)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enSysState) {
    case SYS_STATE_IN_SERVICE:
        strcpy(pcBuf, SYS_STATE_IN_SERVICE_STR);
        break;
    case SYS_STATE_OUT_OF_SERVICE:
        strcpy(pcBuf, SYS_STATE_OUT_OF_SERVICE_STR);
        break;
    case SYS_STATE_DEGRADED:
        strcpy(pcBuf, SYS_STATE_DEGRADED_STR);
        break;
    default:
        break;
    }

    return pcBuf;

}


int
DSC_GetSystemStatus(SYS_STATE_EN *penSysState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acBuf[32];

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == penSysState) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    rc = get_enum_item(sess, DEVICE_XPATH_DEVICE_STATE, acBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", DEVICE_XPATH_DEVICE_STATE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    *penSysState = DSC_SysStateToEnum(acBuf);

ERR_LABEL:
    return rc;
}

int
DSC_GetSystemRstCause(char *pstr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acBuf[32];

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pstr) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    rc = get_enum_item(sess, DEVICE_XPATH_RST_CAUSE, acBuf, sizeof(acBuf));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", DEVICE_XPATH_RST_CAUSE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    strcpy(pstr, acBuf);

ERR_LABEL:
    return rc;
}

static
int
__getPhysicalInfo(DEVICE_PHYSICAL_ST *pstDevPhysical, size_t size, char *pcXpath)
{
    sr_session_ctx_t *sess = NULL;

    ANONYMOUS_ST physical_maps[] = {
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_NODE_TYPE, SR_STRING_T, pstDevPhysical->acNodeType),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_VENDOR, SR_STRING_T, pstDevPhysical->acVendor),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_MODEL, SR_STRING_T, pstDevPhysical->acModel),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PRODUCT_CODE, SR_STRING_T, pstDevPhysical->acProductCode),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_SERIAL_ID, SR_STRING_T, pstDevPhysical->acSerialId),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_MFR_DATE, SR_STRING_T, pstDevPhysical->acManufactorDate),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_CLLI, SR_STRING_T, pstDevPhysical->acClei),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_TYPE, SR_STRING_T, pstDevPhysical->acType),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_HW_VER, SR_STRING_T, pstDevPhysical->acHwVer),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_SW_VER, SR_STRING_T, pstDevPhysical->acSwVer),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_CPLD_0_VER, SR_STRING_T, pstDevPhysical->aacCpldVer[0]),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_CPLD_1_VER, SR_STRING_T, pstDevPhysical->aacCpldVer[1]),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_FPGA_VER, SR_STRING_T, pstDevPhysical->aacFpgaVer[0]),
    };

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pstDevPhysical || size < sizeof(DEVICE_PHYSICAL_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstDevPhysical, 0, size);

    return DSC_GetItems(sess, pcXpath, physical_maps, sizeof(physical_maps));
}

int
DSC_GetPhysicalInfo(DEVICE_PHYSICAL_ST *pstDevPhysical, size_t size)
{
    return __getPhysicalInfo(pstDevPhysical, size, DEVICE_XPATH_PHYSICAL_ALL);
}

int
DSC_GetPhysicalInfoTimeout(DEVICE_PHYSICAL_ST *pstDevPhysical, size_t size, int seconds)
{
    int rc = SR_ERR_OK;

    do {
        rc = __getPhysicalInfo(pstDevPhysical, size, DEVICE_XPATH_PHYSICAL_ALL);
        if (SR_ERR_OK != rc) {
            sleep(1);
        } else {
            break;
        }
    } while (seconds-- > 0);


    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__getPhysicalInfo error: %s", sr_strerror(rc));
    }

    return rc;
}


int
DSC_GetPhysicalExtension(EXT_DEVICE_PHYSICAL_ST *pstIdeeprom)
{
#define DEVICE_XPATH_PHYSICAL_EXT_BOARD_REVISION  DEVICE_XPATH_PHYSICAL_EXT "/hardware-version"
#define DEVICE_XPATH_PHYSICAL_EXT_PN  DEVICE_XPATH_PHYSICAL_EXT "/product-code"
#define DEVICE_XPATH_PHYSICAL_EXT_CLEI  DEVICE_XPATH_PHYSICAL_EXT "/clei"
#define DEVICE_XPATH_PHYSICAL_EXT_MODEL  DEVICE_XPATH_PHYSICAL_EXT "/model"
#define DEVICE_XPATH_PHYSICAL_EXT_MODEL_REVISION  DEVICE_XPATH_PHYSICAL_EXT "/reversion"

    sr_session_ctx_t *sess = NULL;

    ANONYMOUS_ST physical_maps[] = {
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PHYSICAL_EXT_BOARD_REVISION, SR_STRING_T, pstIdeeprom->acBoardRevision),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PHYSICAL_EXT_PN, SR_STRING_T, pstIdeeprom->acPN),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PHYSICAL_EXT_CLEI, SR_STRING_T, pstIdeeprom->acClei),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PHYSICAL_EXT_MODEL, SR_STRING_T, pstIdeeprom->acModel),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_PHYSICAL_EXT_MODEL_REVISION, SR_STRING_T, pstIdeeprom->acModelRevision),

    };

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pstIdeeprom ) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    return DSC_GetItems(sess, DEVICE_XPATH_PHYSICAL_EXT_ALL, physical_maps, sizeof(physical_maps));
}







int
DSC_GetPhysicalExtension_1()
{
    int rc = SR_ERR_OK;
    sr_val_t *values = NULL;
    size_t i, count = 0;
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%d", 0);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, DEVICE_XPATH_PHYSICAL_EXT_ALL, &values, &count);
    if (SR_ERR_OK != rc) {
        goto cleanup;
    }

    for (i = 0; i < count; i++) {
        sr_print_val(values + i);
    }

    sr_free_values(values, count);
cleanup:
    return rc;
}

int
DSC_GetModMfgInfo(DEVICE_PHYSICAL_ST **ppstDevPhysical, size_t *count)
{
    int i = 0, j = 0, rc = SR_ERR_OK;
    DEVICE_PHYSICAL_ST *pstDevPhysical = NULL;
    char selectXpath[OPLK_PATH_MAX_LEN];
    sr_val_t *val = NULL;
    size_t cnt = 0;
    sr_val_t *values = NULL;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == ppstDevPhysical) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%d", 0);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, MODULE_XPATH_MOD_ID_ALL, &values, count);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), selectXpath);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s", MODULE_XPATH_MOD_ID_ALL);
        goto ERR_LABEL;
    }

    pstDevPhysical = (DEVICE_PHYSICAL_ST *)malloc(sizeof(DEVICE_PHYSICAL_ST) * (*count));
    if (NULL == pstDevPhysical) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error %d", SR_ERR_INTERNAL);
        goto ERR_LABEL;
    }

    memset(pstDevPhysical, 0, sizeof(DEVICE_PHYSICAL_ST) * (*count));
    snprintf(selectXpath, OPLK_PATH_MAX_LEN, MODULE_XPATH_ALL);
    rc = get_items(sess, selectXpath, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), selectXpath);
        rc = SR_ERR_OK;
        goto ERR_LABEL;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, MOD_ID_NODE) != NULL) {
            j++;
        } else if (strstr(val[i].xpath, MODEL_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acModel, val[i].data.string_val, sizeof(pstDevPhysical[j].acModel) - 1);
        } else if (strstr(val[i].xpath, MOD_TYPE_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acNodeType, val[i].data.string_val, sizeof(pstDevPhysical[j].acNodeType) - 1);
        } else if (strstr(val[i].xpath, PRODUCT_CODE_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acProductCode, val[i].data.string_val, sizeof(pstDevPhysical[j].acProductCode) - 1);
        } else if (strstr(val[i].xpath, SERIAL_ID_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acSerialId, val[i].data.string_val, sizeof(pstDevPhysical[j].acSerialId) - 1);
        } else if (strstr(val[i].xpath, SW_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acSwVer, val[i].data.string_val, sizeof(pstDevPhysical[j].acSwVer) - 1);
        } else if (strstr(val[i].xpath, HW_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acHwVer, val[i].data.string_val, sizeof(pstDevPhysical[j].acHwVer) - 1);
        } else if (strstr(val[i].xpath, CLEI_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acClei, val[i].data.string_val, sizeof(pstDevPhysical[j].acClei) - 1);
        } else if (strstr(val[i].xpath, MFR_DATE_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acManufactorDate, val[i].data.string_val, sizeof(pstDevPhysical[j].acManufactorDate) - 1);
        } else if (strstr(val[i].xpath, CPLD_0_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].aacCpldVer[0], val[i].data.string_val, sizeof(pstDevPhysical[j].aacCpldVer[0]) - 1);
        } else if (strstr(val[i].xpath, CPLD_1_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].aacCpldVer[1], val[i].data.string_val, sizeof(pstDevPhysical[j].aacCpldVer[1]) - 1);
        } else if (strstr(val[i].xpath, FPGA_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].aacFpgaVer[0], val[i].data.string_val, sizeof(pstDevPhysical[j].aacFpgaVer[0]) - 1);
        } else if (strstr(val[i].xpath, EEPROM_VER_NODE) != NULL) {
            strncpy(pstDevPhysical[j].acEepromVer, val[i].data.string_val, sizeof(pstDevPhysical[j].acEepromVer) - 1);
        }

    }
    sr_free_values(val, cnt);
    sr_free_values(values, *count);

    *ppstDevPhysical = pstDevPhysical;

    return rc;

ERR_LABEL:
    if (values != NULL)
        sr_free_values(values, *count);
    if (val != NULL)
        sr_free_values(val, cnt);

    if (pstDevPhysical != NULL) {
        free(pstDevPhysical);
        pstDevPhysical = NULL;
    }
    *count = 0;

    return rc;
}

NODE_TYPE_EN
DSC_NodetypeStrToEnum(char *pcNodeType)
{
    NODE_TYPE_EN enNodeType;

    if (NULL == pcNodeType) {
        return MOD_IDX_BOARD;
    }

    if (0 == strcasecmp(NODE_TYPE_ROADM_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_ROADM;
    } else if (0 == strcasecmp(NODE_TYPE_XPDR_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_XPDR;
    } else if (0 == strcasecmp(NODE_TYPE_EDFA_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_EDFA;
    } else if (0 == strcasecmp(NODE_TYPE_OCM_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_OCM;
    } else if (0 == strcasecmp(NODE_TYPE_OPS_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_OPS;
    } else if (0 == strcasecmp(NODE_TYPE_TOA_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_TOA;
    } else if (0 == strcasecmp(NODE_TYPE_OCMLE_STR, pcNodeType)) {
        enNodeType = NODE_TYPE_OCMLE;
    } else {
        enNodeType = MOD_IDX_BOARD;
    }

    return enNodeType;
}

MOD_IDX_EN
DSC_ModuleTypeStrToEnum(char *pcModuleType)
{
    MOD_IDX_EN enModType;

    if (NULL == pcModuleType) {
        return MOD_IDX_BOARD;
    }

    if (0 == strcasecmp(MOD_IDX_EDFA_STR, pcModuleType)) {
        enModType = MOD_IDX_EDFA;
    } else if (0 == strcasecmp(MOD_IDX_EDFA_STR, pcModuleType)) {
        enModType = MOD_IDX_EDFA;
    } else if (0 == strcasecmp(MOD_IDX_EDFA_PA, pcModuleType)) {
        enModType = MOD_IDX_EDFA;
    } else if (0 == strcasecmp(MOD_IDX_EDFA_BA, pcModuleType)) {
        enModType = MOD_IDX_WSS;
    } else if (0 == strcasecmp(MOD_IDX_MUX_STR, pcModuleType)) {
        enModType = MOD_IDX_MUX;
    } else if (0 == strcasecmp(MOD_IDX_OCM_STR, pcModuleType)) {
        enModType = MOD_IDX_OCM;
    } else if (0 == strcasecmp(MOD_IDX_OPS_STR, pcModuleType)) {
        enModType = MOD_IDX_OPS;
    } else if (0 == strcasecmp(MOD_IDX_PAM4_STR, pcModuleType)) {
        enModType = MOD_IDX_PAM4;
    } else if (0 == strcasecmp(MOD_IDX_TDCM_STR, pcModuleType)) {
        enModType = MOD_IDX_TDCM;
    } else if (0 == strcasecmp(MOD_IDX_OSC_STR, pcModuleType)) {
        enModType = MOD_IDX_OSC;
    } else if (0 == strcasecmp(MOD_IDX_VOA_STR, pcModuleType)) {
        enModType = MOD_IDX_VOA;
    } else if (0 == strcasecmp(MOD_IDX_APPS_STR, pcModuleType)) {
        enModType = MOD_IDX_APPS;
    } else if (0 == strcasecmp(MOD_IDX_CPLD_STR, pcModuleType)) {
        enModType = MOD_IDX_CPLD;
    } else if (0 == strcasecmp(MOD_IDX_FPGA_STR, pcModuleType)) {
        enModType = MOD_IDX_FPGA;
    } else if (0 == strcasecmp(MOD_IDX_FAN_STR, pcModuleType)) {
        enModType = MOD_IDX_FAN;
    } else if (0 == strcasecmp(MOD_IDX_PWR_STR, pcModuleType)) {
        enModType = MOD_IDX_PWR;
    } else if (0 == strcasecmp(MOD_IDX_CARD_CHASSIS_STR, pcModuleType)) {
        enModType = MOD_IDX_CARD_CHASSIS;
    } else if (0 == strcasecmp(MOD_IDX_CARD_OCM_STR, pcModuleType)) {
        enModType = MOD_IDX_CARD_OCM;
    } else if (0 == strcasecmp(MOD_IDX_CARD_BA_STR, pcModuleType)) {
        enModType = MOD_IDX_CARD_BA;
    } else if (0 == strcasecmp(MOD_IDX_CARD_PA_STR, pcModuleType)) {
        enModType = MOD_IDX_CARD_PA;
    } else if (0 == strcasecmp(MOD_IDX_CARD_SCC_STR, pcModuleType)) {
        enModType = MOD_IDX_CARD_SCC;
    } else {
        enModType = MOD_IDX_BOARD;
    }

    return enModType;
}

int
DSC_ModuleTypeEnumToString(MOD_IDX_EN enModType, char *pcModuleType, size_t len)
{
    if (NULL == pcModuleType) {
        return MOD_IDX_BOARD;
    }

    switch (enModType) {
    case MOD_IDX_EDFA:
        strncpy(pcModuleType, MOD_IDX_EDFA_STR, len);
        break;
    case MOD_IDX_WSS:
        strncpy(pcModuleType, MOD_IDX_WSS_STR, len);
        break;
    case MOD_IDX_MUX:
        strncpy(pcModuleType, MOD_IDX_MUX_STR, len);
        break;
    case MOD_IDX_OCM:
        strncpy(pcModuleType, MOD_IDX_OCM_STR, len);
        break;
    case MOD_IDX_OPS:
        strncpy(pcModuleType, MOD_IDX_OPS_STR, len);
        break;
    case MOD_IDX_PAM4:
        strncpy(pcModuleType, MOD_IDX_PAM4_STR, len);
        break;
    case MOD_IDX_TDCM:
        strncpy(pcModuleType, MOD_IDX_TDCM_STR, len);
        break;
    case MOD_IDX_OSC:
        strncpy(pcModuleType, MOD_IDX_OSC_STR, len);
        break;
    case MOD_IDX_VOA:
        strncpy(pcModuleType, MOD_IDX_VOA_STR, len);
        break;
    case MOD_IDX_APPS:
        strncpy(pcModuleType, MOD_IDX_APPS_STR, len);
        break;
    case MOD_IDX_CPLD:
        strncpy(pcModuleType, MOD_IDX_CPLD_STR, len);
        break;
    case MOD_IDX_FPGA:
        strncpy(pcModuleType, MOD_IDX_FPGA_STR, len);
        break;
    case MOD_IDX_FAN:
        strncpy(pcModuleType, MOD_IDX_FAN_STR, len);
        break;
    case MOD_IDX_PWR:
        strncpy(pcModuleType, MOD_IDX_PWR_STR, len);
        break;
    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

DSC_SLOT_STATUS_EN
DSC_SlotStatusToEnum(char *pcBuf)
{
    DSC_SLOT_STATUS_EN enSlotStatus = DSC_SLOT_STATUS_EMPTY_NOT_PROV;

    if (NULL == pcBuf) {
        return DSC_SLOT_STATUS_EMPTY_NOT_PROV;
    }

    if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_EMPTY_NOT_PROV_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_EMPTY_NOT_PROV;
    } else if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_EMPTY_PROV_MATCH_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_EMPTY_PROV_MATCH;
    } else if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_EMPTY_PROV_MISMATCH_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_EMPTY_PROV_MISMATCH;
    } else if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_INSTALLED_NOT_PROV_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_INSTALLED_NOT_PROV;
    } else if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_INSTALLED_PROV_MATCH_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_INSTALLED_PROV_MATCH;
    } else if (0 == strcmp(pcBuf, DSC_SLOT_STATUS_INSTALLED_PROV_MISMATCH_STR)) {
        enSlotStatus = DSC_SLOT_STATUS_INSTALLED_PROV_MISMATCH;
    }

    return enSlotStatus;
}

int
DSC_GetAllSlotsState(SHELF_SLOTS_ST *pstShelfSlots)
{
    int i = 0, j = 0, rc = SR_ERR_OK;
    char selectXpath[OPLK_PATH_MAX_LEN];
    sr_val_t *val = NULL;
    size_t cnt = 0;
    sr_session_ctx_t *sess = NULL;

    SHELF_SLOTS_ST stShelfSlots;

    memset(&stShelfSlots, 0, sizeof(stShelfSlots));

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pstShelfSlots) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%d", 0);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(selectXpath, OPLK_PATH_MAX_LEN, "%s", SLOTS_XPATH_ALL);
    rc = get_items(sess, selectXpath, &val, &cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, SLOT_ID_NODE) != NULL) {
            j++;
            stShelfSlots.astSlotInfo[j].uiSlotId = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, SLOT_NAME_NODE) != NULL) {
            strncpy(stShelfSlots.astSlotInfo[j].acSlotName, val[i].data.string_val, sizeof(stShelfSlots.astSlotInfo[j].acSlotName) - 1);
        } else if (strstr(val[i].xpath, LABEL_NODE) != NULL) {
            strncpy(stShelfSlots.astSlotInfo[j].acLabel, val[i].data.string_val, sizeof(stShelfSlots.astSlotInfo[j].acLabel) - 1);
        } else if (strstr(val[i].xpath, SLOT_STATUS_NODE) != NULL) {
            stShelfSlots.astSlotInfo[j].enStatus = DSC_SlotStatusToEnum(val[i].data.enum_val);
        }

        if (j >= DSC_SLOT_MAX) {
            rc = ERR_NOMEM;
            goto ERR_LABEL;
        }
    }

    stShelfSlots.uiSlotCnt = j >= 0 ? j + 1 : 0;

    memcpy(pstShelfSlots, &stShelfSlots, sizeof(stShelfSlots));

    sr_free_values(val, cnt);

    return rc;

ERR_LABEL:
    if (val != NULL)
        sr_free_values(val, cnt);

    return rc;
}

int
DSC_GetNodeId(char *pcNodeId, size_t nMaxSize)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();
    if (NULL == pstSess ||
        NULL == pcNodeId) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length: %zu", nMaxSize);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_NODE_ID, pcNodeId, nMaxSize);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__GetNodeConfigInfo(sr_session_ctx_t *pstSess, ST_NODE_INFO *pstNodeInfo)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess) {
        pstSess = DSC_GetRunningSession();
    }

    rc = get_string_item(pstSess, OOD_XPATH_NODE_ID, pstNodeInfo->acNodeId, sizeof(pstNodeInfo->acNodeId));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_uint32_item(pstSess, OOD_XPATH_NODE_NUMBER, &(pstNodeInfo->ulNodeNumber));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_NODE_NUMBER, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_NODE_TYPE, pstNodeInfo->acNodeType, sizeof(pstNodeInfo->acNodeType));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_NODE_TYPE, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_CLLI, pstNodeInfo->acClli, sizeof(pstNodeInfo->acClli));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_CLLI, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__GetNodeStatusInfo(sr_session_ctx_t *pstSess, ST_NODE_INFO *pstNodeInfo)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess) {
        pstSess = DSC_GetOperSession();
    }

    rc = get_string_item(pstSess, OOD_XPATH_VENDOR, pstNodeInfo->acVendor, sizeof(pstNodeInfo->acVendor));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_VENDOR, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_MODEL, pstNodeInfo->acModel, sizeof(pstNodeInfo->acModel));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_MODEL, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_SW_VER, pstNodeInfo->acSwVer, sizeof(pstNodeInfo->acSwVer));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_SW_VER, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, OOD_XPATH_SW_VER_BUILD, pstNodeInfo->acSwBuild, sizeof(pstNodeInfo->acSwBuild));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_SW_VER_BUILD, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_GetNodeInfo(ST_NODE_INFO *pstNodeInfo, size_t nMaxSize)
{
    int rc = SR_ERR_OK;

    if (NULL == pstNodeInfo || nMaxSize < sizeof(ST_NODE_INFO)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length: %zu", nMaxSize);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset((void *)pstNodeInfo, 0x00, nMaxSize);

    rc = __GetNodeConfigInfo(NULL, pstNodeInfo);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    rc = __GetNodeStatusInfo(NULL, pstNodeInfo);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

cleanup:
    if ((rc != SR_ERR_OK) &&
        (pstNodeInfo != NULL)) {
        memset((void *)pstNodeInfo, 0x00, nMaxSize);
    }

    return rc;
}


#endif

#if OPERATE_DSC_API

int
DSC_Reset(char *pcResetModule, char *pcResetType)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/reset-type leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_RESTART_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_RESTART_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup("device");   /* todo */

    /* set 'input/extension leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_RESTART_EXTENSION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_RESTART_EXTENSION, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcResetModule);

    /* set 'input/reset-type leaf */
    rc = sr_val_set_xpath(&input[2], RPC_XPATH_RESTART_OPTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_RESTART_OPTION, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_ENUM_T;
    input[2].data.enum_val = strdup(pcResetType);
    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_RESTART, input, 3, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_RESTART, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    rc = DSC_ProcessRPCResponse(output, output_cnt);
    sleep(1);
    return rc;

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_SwStage(char *pcFilename)
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

    /* set 'input/Filename' leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SW_STAGE_FILENAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SW_STAGE_FILENAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilename);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SW_STAGE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SW_STAGE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_SwActivate(char *pcVersion, char *pcValidationTimer, char *pcStatus)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/version' leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SW_ACTIVATE_VERSION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SW_ACTIVATE_VERSION, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcVersion);

    /* set 'input/validationTimer' leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_SW_ACTIVATE_VALID);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SW_ACTIVATE_VALID, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcVersion);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SW_ACTIVATE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SW_ACTIVATE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    strcpy(pcStatus, output[1].data.string_val);

    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_SwUpgrade(char *pcFilename, char *pcStatus)
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

    /* set 'input/Filename' leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SW_STAGE_FILENAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SW_STAGE_FILENAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilename);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SW_STAGE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SW_STAGE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    strcpy(pcStatus, output[1].data.string_val);


    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_SwCancel(bool bBoolean)
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

    /* set 'input/ACCEPT leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SW_CANCEL_ACCEPT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SW_CANCEL_ACCEPT, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;  /* "true" : "false" */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SW_CANCEL, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SW_CANCEL, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}


int
DSC_GetUpgState(ST_UPG_STATE *pstUpgState, size_t size)
{
    sr_session_ctx_t *sess = NULL;

    ANONYMOUS_ST state_maps[] = {
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_UPG_STATE_SYS_STATUS, SR_ENUM_T, pstUpgState->acSysState),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_UPG_STATE_STEP, SR_ENUM_T, pstUpgState->acUpgdStep),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_UPG_STATE_PERCENT, SR_UINT8_T, (char *)(&pstUpgState->u8Percent)),
        ANONYMOUS_MAP_ENTRY(DEVICE_XPATH_UPG_STATE_HITLESS, SR_BOOL_T, (char *)(&pstUpgState->bHitLess)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstUpgState || size < sizeof(ST_UPG_STATE)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstUpgState, 0, size);

    return DSC_GetItems(sess, DEVICE_XPATH_UPG_STATE_ALL, state_maps, sizeof(state_maps));
}

int
DSC_RestoreDefault(EN_RESTORE_DEFAULT_TYPE enRestoreType)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char *acRestoreType = (enRestoreType == EN_RETORE_DEFAULT_OPTICAL) ? RPC_TYPE_OPTICAL : RPC_TYPE_ALL;

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/reset-type leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_RESTORE_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_RESTORE_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(acRestoreType);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_RESTORE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_RESTORE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

/* will save all database, cp all run to startup, and save to eeprom */
int
DSC_SaveConfig(const char *module)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL;
    sr_val_t *output = NULL;
    size_t output_cnt = 0;
    size_t input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    LOG_WRITE(EN_LOG_NOTICE, "++ DSC_SaveConfig MODULE: %s", module);

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    /* set 'input/save-type leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SAVE_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SAVE_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(RPC_TYPE_ALL);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SAVE_CONF, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SAVE_CONF, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_NtpSync(char *pcIpAddress)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    return DSC_NtpSync_Session(sess, pcIpAddress);
}

int
DSC_NtpSync_Session(sr_session_ctx_t *sess, char *pcIpAddress)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "Input sess null %s", "");
        return SR_ERR_INVAL_ARG;
    }

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/filename leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_NTP_SYNC_IP);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_NTP_SYNC_IP, sr_strerror(rc));
        return rc;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcIpAddress);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_NTP_SYNC, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_NTP_SYNC, sr_strerror(rc));
        return rc;
    }
    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_SaveLog()
{
    int rc = SR_ERR_OK;

    /* save log to disk */

    return rc;
}

int
DSC_GetSystemFile(char *pcFilePath, FILE_ATTR_ALL_INFO_ST *pstAllFileInfo)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    int iCount = 0;
    int iFileIndex = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values filename */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/filename leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SHOW_FILE_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SHOW_FILE_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilePath);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_SHOW_FILE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SHOW_FILE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    iFileIndex = -1;
    for (iCount = 0; iCount < output_cnt; iCount++) {

        if (NULL != strstr(output[iCount].xpath, "/filename")) {
            iFileIndex++;
            /* OC2A-496 'show db-backup' stack (backupfile count limit) */
            if (iFileIndex >= DSC_FILE_NUM_MAX) {
                LOG_WRITE(EN_LOG_WARNING, "file index: %d out of range", iFileIndex);
                iFileIndex = iFileIndex - 1;
                break;
            }
            strncpy(pstAllFileInfo->astFileEntryInfo[iFileIndex].acFileName,
                    output[iCount].data.string_val, DSC_FILE_NAME_LEN_MAX - 1);
        } else if (NULL != strstr(output[iCount].xpath, "/file-size")) {
            pstAllFileInfo->astFileEntryInfo[iFileIndex].ullFileSize = output[iCount].data.uint64_val;
        } else if (NULL != strstr(output[iCount].xpath, "/modified-date")) {
            strncpy(pstAllFileInfo->astFileEntryInfo[iFileIndex].acFileModiyTime,
                    output[iCount].data.string_val, DSC_MODIFY_TIME_LEN_MAX - 1);
        } else {
        }
    }

    pstAllFileInfo->ulNum = iFileIndex + 1;

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_DelSystemFile(char *pcFilePath)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values filename*/
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/filename leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_DEL_FILE_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_DEL_FILE_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilePath);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_DEL_FILE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_DEL_FILE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_TransferFile(char *pcAction, char *pcLocalPath, char *pcRemotePath)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values filename*/
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/action leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_FILE_TRANSFER_ACTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_FILE_TRANSFER_ACTION, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(pcAction);

    rc = sr_val_set_xpath(&input[1], RPC_XPATH_FILE_TRANSFER_LOCAL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_FILE_TRANSFER_LOCAL, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcLocalPath);

    rc = sr_val_set_xpath(&input[2], RPC_XPATH_FILE_TRANSFER_REMOTE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_FILE_TRANSFER_REMOTE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_STRING_T;
    input[2].data.string_val = strdup(pcRemotePath);

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_FILE_TRANSFER, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_FILE_TRANSFER, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}


int
DSC_TransferLog(char *pcUser, char *pcPwd, char *pcDestFile)
{
    char acCmd[256];
    char acPath[512] = {0};

    if (NULL == pcUser || NULL == pcPwd || NULL == pcDestFile) {
        return SR_ERR_INVAL_ARG;
    }

    memset(acCmd, 0, sizeof(acCmd));
#if 0
    snprintf(acCmd, sizeof(acCmd) - 1, "tar zcvf /home/oplink/log/get_log.tgz /var/log/* /home/oplink/log/*-log.tar.gz >/dev/null 2>&1");
#endif
    if (OPLK_OK == access("/home/oplink/log/internal-log.tar.gz", F_OK)) {
        snprintf(acCmd, sizeof(acCmd) - 1, "tar zcvf /home/oplink/log/get_log.tgz /var/log/* /home/oplink/log/core_dir /home/oplink/log/*-log.tar.gz >/dev/null 2>&1");
    } else {
        snprintf(acCmd, sizeof(acCmd) - 1, "tar zcvf /home/oplink/log/get_log.tgz /var/log/* /home/oplink/log/core_dir >/dev/null 2>&1");
    }

    if (0 != system(acCmd)) {
    }
    DEF_SLEEP_S(1);

    memset(acPath, 0, sizeof(acPath));
    snprintf(acPath, sizeof(acPath), "%s:%s@%s&%s",
             FILE_TRANSSPORT_PROTOCAL,
             pcUser,
             pcDestFile,
             pcPwd);

    return DSC_TransferFile(FILE_TRANSFER_UPLOAD, GET_LOG_FILE, acPath);
}


int
DSC_SetAdminState(char *pcState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcState) {
        return SR_ERR_INVAL_ARG;
    }
    rc = set_enum_item(sess, DEVICE_XPATH_ADMIN_STATE, pcState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_SetAdminState failed: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }
    return rc;
}

int
DSC_GetAdminState(char *pcState, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();

    rc = get_enum_item(sess, DEVICE_XPATH_ADMIN_STATE, pcState, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_GetAdminState failed: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetSystemMode(SYSTEM_MODE_EN enMode)
{
    int rc = SR_ERR_OK;
    char *pcMode = NULL;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    switch (enMode) {
    case SYSTEM_AUTO:
        pcMode = SYSTEM_MODE_AUTO;
        break;
    case SYSTEM_MANUAL:
        pcMode = SYSTEM_MODE_MANUAL;
        break;
    case SYSTEM_DEBUG:
        pcMode = SYSTEM_MODE_DEBUG;
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "Input mode error: %d", enMode);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_enum_item(sess, DEVICE_XPATH_WORK_MODE, pcMode);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Set system mode failed: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_GetSystemMode(SYSTEM_MODE_EN *penSystemMode)
{
    int rc = SR_ERR_OK;
    char acMode[32] = {0};
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == penSystemMode) {
        LOG_WRITE(EN_LOG_ERROR, "Input null %s", "");
        return SR_ERR_INVAL_ARG;
    }

    memset(acMode, 0x00, sizeof(acMode));
    rc = get_enum_item(sess, DEVICE_XPATH_WORK_MODE, acMode, sizeof(acMode));

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get system mode failed: %s", sr_strerror(rc));
        return rc;
    }

    if (0 == strcmp(SYSTEM_MODE_AUTO, acMode)) {
        *penSystemMode = SYSTEM_AUTO;
    } else if (0 == strcmp(SYSTEM_MODE_MANUAL, acMode)) {
        *penSystemMode = SYSTEM_MANUAL;
    } else if (0 == strcmp(SYSTEM_MODE_DEBUG, acMode)) {
        *penSystemMode = SYSTEM_DEBUG;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "System mode error: %s", acMode);
        return rc;
    }

    return rc;
}


int
DSC_BackupDB(char *pcFilePath)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values filename*/
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/action leaf */
    rc = sr_val_set_xpath(&input[0], DB_XPATH_BACKUP_FILE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DB_XPATH_BACKUP_FILE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilePath);

    /* execute RPC */
    rc = DSC_RpcSend(sess, DB_XPATH_BACKUP, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", DB_XPATH_BACKUP, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}


int
DSC_RestoreDB(char *pcFilePath, bool bNodeIdCheck)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values filename*/
    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/action leaf */
    rc = sr_val_set_xpath(&input[0], DB_XPATH_RESTORE_FILE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DB_XPATH_RESTORE_FILE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFilePath);

    rc = sr_val_set_xpath(&input[1], DB_XPATH_RESTORE_NODEID_CHECK);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DB_XPATH_RESTORE_NODEID_CHECK, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_BOOL_T;
    input[1].data.bool_val = bNodeIdCheck;

    /* execute RPC */
    rc = DSC_RpcSend(sess, DB_XPATH_RESTORE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", DB_XPATH_RESTORE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_ActivateDB(char *pcValidationTimer)
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

    /* set 'input/validationTimer' leaf */
    rc = sr_val_set_xpath(&input[0], DB_XPATH_ROLLBACK_TIMER);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DB_XPATH_ROLLBACK_TIMER, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcValidationTimer);

    /* execute RPC */
    rc = sr_rpc_send(sess, DB_XPATH_ACTIVATE, input, input_cnt, 0, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", DB_XPATH_ACTIVATE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_CancelDB(bool bBoolean)
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

    /* set 'input/ACCEPT leaf */
    rc = sr_val_set_xpath(&input[0], DB_XPATH_CANCEL_ACCEPT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DB_XPATH_CANCEL_ACCEPT, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;  /* "true" : "false" */

    /* execute RPC */
    rc = sr_rpc_send(sess, DB_XPATH_CANCEL, input, input_cnt, 0, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", DB_XPATH_CANCEL, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_RpcSetApsd                                                                        */
/* Descrp : RPC set APSD                                                                          */
/* Input  : pcPath -- APSD path                                                                   */
/*        : bEnabled -- enable/disable APSD                                                       */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_RpcSetApsd(EN_APSD_PATH enPath, bool bEnabled)
{
    sr_val_t *pstInput = NULL;
    sr_val_t *pstOutput = NULL;
    size_t nOutputCnt = 0;
    size_t nInputCnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check parameter.                                                     */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "Session null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* allocate input values                                                */
    nInputCnt = 2;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "new values error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set 'input/enable  leaf.                                             */
    rc = sr_val_set_xpath(&pstInput[0], DEF_RPC_XPATH_SET_APSD DEF_RPC_NODE_APSD_ENABLED);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_RPC_XPATH_SET_APSD DEF_RPC_NODE_APSD_ENABLED, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_BOOL_T;
    pstInput[0].data.bool_val = bEnabled;

    /* set 'input/path leaf.                                                */
    rc = sr_val_set_xpath(&pstInput[1], DEF_RPC_XPATH_SET_APSD DEF_RPC_NODE_APSD_PATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_RPC_XPATH_SET_APSD DEF_RPC_NODE_APSD_PATH, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_ENUM_T;
    pstInput[1].data.enum_val = (EN_APSD_PATH_W == enPath ? strdup("W") : strdup("P"));

    /* execute RPC.                                                         */
    rc = sr_rpc_send(pstSess, DEF_RPC_XPATH_SET_APSD, pstInput, nInputCnt, 0, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", DEF_RPC_XPATH_SET_APSD, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    return DSC_ProcessRPCResponse(pstOutput, nOutputCnt);

ERR_LABEL:
    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_RpcGetApsdState                                                                   */
/* Descrp : Get APSD state                                                                        */
/* Input  : enPath -- APSD path                                                                   */
/* Output : pstState -- A pointer of cv state buffer                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_RpcGetApsdState(EN_APSD_PATH enPath, ST_APSD_STATE *pstApsdState)
{
    sr_val_t *pstInput = NULL;
    sr_val_t *pstOutput = NULL;
    size_t nOutputCnt = 0;
    size_t nInputCnt = 0;
    int rc = SR_ERR_OK;
    ST_APSD_STATE stApsdState;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acOperStatus[DEF_BUF_SIZE_32] = {0};

    ANONYMOUS_ST astAnonymous[] = {{DEF_RPC_XPATH_GET_APSD DEF_RPC_NODE_APSD_ENABLED, SR_BOOL_T, (char *)&(stApsdState.bEnabled), sizeof(stApsdState.bEnabled)},
                                   {DEF_RPC_XPATH_GET_APSD DEF_RPC_NODE_APSD_STATUS, SR_BOOL_T, (char *)&(stApsdState.bStatus), sizeof(stApsdState.bStatus)},
                                   {DEF_RPC_XPATH_GET_APSD DEF_RPC_NODE_STATUS, SR_ENUM_T, acOperStatus, sizeof(acOperStatus)}};

    if (NULL == pstApsdState) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&stApsdState, 0, sizeof(stApsdState));

    /* allocate input values                                                */
    nInputCnt = 1;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "new values error: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* Set input path value                                                 */
    rc = sr_val_set_xpath(&pstInput[0], DEF_RPC_XPATH_GET_APSD DEF_RPC_NODE_APSD_PATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_RPC_XPATH_GET_APSD DEF_RPC_NODE_APSD_PATH, sr_strerror(rc));
        goto END_LABEL;
    }
    pstInput[0].type = SR_ENUM_T;
    pstInput[0].data.enum_val = (EN_APSD_PATH_W == enPath ? strdup("W") : strdup("P"));

    /* execute RPC                                                          */
    rc = sr_rpc_send(pstSess, DEF_RPC_XPATH_GET_APSD, pstInput, nInputCnt, 0, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", DEF_RPC_XPATH_GET_APSD, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_ParseValuesByXpath(pstOutput, nOutputCnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "parse output failed (%d).", rc);
        goto END_LABEL;
    }

    if (strcmp(acOperStatus, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", DEF_RPC_XPATH_GET_APSD, acOperStatus);
        rc = SR_ERR_OPERATION_FAILED;
        goto END_LABEL;
    } else {
        memcpy(pstApsdState, &stApsdState, sizeof(stApsdState));
    }

END_LABEL:
    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    if (pstInput != NULL) {
        sr_free_values(pstOutput, nOutputCnt);
        pstOutput = NULL;
    }
    return rc;
}

#endif

int
DSC_LampTest(bool enabled)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 2;

    sr_session_ctx_t *sess = DSC_GetRunningSession();

    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup("cp-main-board");   /*NOPPORO main board name, must be same as led-control/circuit-pack-name*/
    /* set 'input/reset-type leaf */
    rc = sr_val_set_xpath(&input[0], "/org-openroadm-device:led-control/circuit-pack-name");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/org-openroadm-device:led-control/equipment-entity", sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[1].type = SR_BOOL_T;
    input[1].data.bool_val = enabled;  /* "true" : "false" */
    rc = sr_val_set_xpath(&input[1], "/org-openroadm-device:led-control/enabled");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/org-openroadm-device:led-control/enabled", sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DSC_LampTest");
    rc = DSC_RpcSend(sess, RPC_XPATH_LAMP_TEST, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_LAMP_TEST, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_PortLampTest(bool bEnabled, char *pcPortName)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 2;

    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcPortName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcPortName);

    rc = sr_val_set_xpath(&input[0], LED_CTRL_XPATH_PORT_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/org-openroadm-device:led-control/equipment-entity", sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[1].type = SR_BOOL_T;
    input[1].data.bool_val = bEnabled;
    rc = sr_val_set_xpath(&input[1], LED_CTRL_XPATH_ENABELD);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", LED_CTRL_XPATH_ENABELD, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_RpcSend(sess, RPC_XPATH_LAMP_TEST, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_LAMP_TEST, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}


bool
DSC_IsScpServerEnabled()
{
    if (access(SCP_PATH, F_OK) != 0) {
        return false;
    }

    return true;
}

int
DSC_SetScpServerEnable(bool bEnable)
{
    bool bCurEnableState = DSC_IsScpServerEnabled();
    char acCmdBuf[DEF_BUF_SIZE_256] = {0};

    if (bCurEnableState == bEnable) {
        return SR_ERR_OK;
    }

    /* NOTE: for security/reliability reason SCP server is disabled by default after each device restart. */

    if (bEnable) {
        snprintf(acCmdBuf, sizeof(acCmdBuf), "ln -s %s %s >/dev/null 2>&1", SCP_OPENSSH_PATH, SCP_PATH);
    } else {
        snprintf(acCmdBuf, sizeof(acCmdBuf), "rm -f %s >/dev/null 2>&1", SCP_PATH);
    }

    if (OPLK_OK != UTIL_System(acCmdBuf)) {
        return SR_ERR_OPERATION_FAILED;
    }
    return SR_ERR_OK;
}

int
DSC_HashLogEnable(bool bBoolean)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    LOG_WRITE(EN_LOG_NOTICE, "Executing Hash type: %s", bBoolean ? "enabled" : "disabled");

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    /* set 'input/filename leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_LOG_HASH_ENABLE);
    if (SR_ERR_OK != rc) {
        sr_free_values(input, input_cnt);
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_LOG_HASH_ENABLE, sr_strerror(rc));
        return rc;
    }
    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;

    /* execute RPC */
    rc = sr_rpc_send(sess, RPC_XPATH_LOG_HASH, input, input_cnt, 0, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        sr_free_values(input, input_cnt);
        LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send: %s %s", RPC_XPATH_LOG_HASH, sr_strerror(rc));
        return rc;
    }
    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_AutoSetAutoCtrlRamp(char *pcLine, double dStep, uint32_t uiInterval)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL;
    size_t input_cnt = 0;
    sr_val_t *output = NULL;
    size_t output_cnt = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* allocate input values path*/
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        goto END_LABEL;
    }

    /* set input/path leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_SET_AUTO_CTRL_RAMP_PATH);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SET_AUTO_CTRL_RAMP_PATH, sr_strerror(rc));
        goto END_LABEL;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcLine);

    /* set input/step leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_SE_AUTO_CTRLT_RAMP_STEP);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SE_AUTO_CTRLT_RAMP_STEP, sr_strerror(rc));
        goto END_LABEL;
    }

    input[1].type = SR_DECIMAL64_T;
    input[1].data.decimal64_val = dStep;

    /* set input/interval leaf */
    rc = sr_val_set_xpath(&input[2], RPC_XPATH_SET_AUTO_CTRL_RAMP_INTERVAL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_SET_AUTO_CTRL_RAMP_INTERVAL, sr_strerror(rc));
        goto END_LABEL;
    }

    input[2].type = SR_UINT32_T;
    input[2].data.uint8_val = uiInterval;

    rc = DSC_RpcSend(pstSess, RPC_XPATH_SET_AUTO_CTRL_RAMP, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_SET_AUTO_CTRL_RAMP, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_ProcessRPCResponse(output, output_cnt);

END_LABEL:
    sr_free_values(input, input_cnt);
    LOG_WRITE(EN_LOG_NOTICE, "%s (rc:%d)", __func__, rc);
    return rc;
}
