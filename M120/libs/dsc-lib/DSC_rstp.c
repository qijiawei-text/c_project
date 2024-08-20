/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_rstp.h"

L2_PORT_IFNAME_EN
DSC_GetL2PortIfName(char *pcXpath)
{
    int iIfNameid = L2_PORT_IFNAME_NONE;

    if (NULL == pcXpath) {
        return iIfNameid;
    }

    if (NULL != strstr(pcXpath, L2_PORT_IFNAME_1_STR)) {
        iIfNameid = L2_PORT_IFNAME_1;
    } else if (NULL != strstr(pcXpath, L2_PORT_IFNAME_2_STR)) {
        iIfNameid = L2_PORT_IFNAME_2;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error rstp prot ifname%s", "");
    }

    return iIfNameid;
}

char *
DSC_L2PortToStr(char *pcStr, L2_PORT_IFNAME_EN ifName)
{
    if (NULL == pcStr) {
        return NULL;
    }

    switch (ifName) {
    case L2_PORT_IFNAME_1:
        strcpy(pcStr, L2_PORT_IFNAME_1_STR);
        break;
    case L2_PORT_IFNAME_2:
        strcpy(pcStr, L2_PORT_IFNAME_2_STR);
        break;
    default:
        strcpy(pcStr, "");
        break;
    }

    return pcStr;
}


int
DSC_GetRstpBridgeState(RSTP_BRIDGE_STATE_ST *pstRstpBridgeState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;
    int i = 0, j = 0;

    /* get all list instances with their content (recursive) */
    rc = get_items(sess, DEVICE_XPATH_PROTOCOLS_RSTP_HEADER_ALL, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), DEVICE_XPATH_PROTOCOLS_RSTP_HEADER_ALL);
        if (val != NULL) {
            sr_free_values(val, cnt);
        }
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        return rc;
    }

    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, ROOT_BRIDGE_PORT) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootBridgePort = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_PATH_COST) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootPathCost = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_BRIDGE_PRIORRITY) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootBridgePriority = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_BRIDGE_ID) != NULL) {
            strcpy(pstRstpBridgeState->stRstpBridgeAttr.acRootBridgeID, val[i].data.string_val);
        } else if (strstr(val[i].xpath, ROOT_HOLD_TIME) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootHoldTime = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_HELLO_TIME) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootHelloTime = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_MAX_AGE) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootMaxAge = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, ROOT_FORWARD_DELAY) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32RootForwardDelay = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, BRIDGE_ID) != NULL) {
            strcpy(pstRstpBridgeState->stRstpBridgeAttr.acBridgeID, val[i].data.string_val);
        } else if (strstr(val[i].xpath, TOPO_CHANGE_COUNT) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32TopoChangeCount = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, TIME_SINCE_TOPO_CHANGE) != NULL) {
            pstRstpBridgeState->stRstpBridgeAttr.u32TimeSinceTopoChange = val[i].data.uint32_val;
        } else if (NULL != strstr(val[i].xpath, RSTP_BRIDGE_PORT_ATTR)) {
            if (NULL != strstr(val[i].xpath, RSTP_BRIDGE_PORT_STATE)) {
                strcpy(pstRstpBridgeState->stRstpBridgePortAttr[j].acBridgePortState, val[i].data.enum_val);
            } else if (NULL != strstr(val[i].xpath, RSTP_BRIDGE_PORT_ROLE)) {
                strcpy(pstRstpBridgeState->stRstpBridgePortAttr[j].acBridgePortRole, val[i].data.enum_val);
            } else if (NULL != strstr(val[i].xpath, RSTP_BRIDGE_PORT_ID)) {
                pstRstpBridgeState->stRstpBridgePortAttr[j].u32BridgePortID = val[i].data.uint32_val;
            } else if (NULL != strstr(val[i].xpath, RSTP_DESIGNATED_BRIDGE_PORT)) {
                pstRstpBridgeState->stRstpBridgePortAttr[j].u32DesignatedBridgePort = val[i].data.uint32_val;
            } else if (NULL != strstr(val[i].xpath, RSTP_DESIGNATED_BRIDGE_ID)) {
                strcpy(pstRstpBridgeState->stRstpBridgePortAttr[j].acDesignatedBridgeID, val[i].data.string_val);
                DSC_L2PortToStr(pstRstpBridgeState->stRstpBridgePortAttr[j].acIfName, j + 1);
                if (j >= L2_PORT_IFNAME_CNT) {
                    break;
                }
                j++;
            }
        }
    }

    sr_free_values(val, cnt);
    return rc;
}

int
DSC_GetRstpBridgeConfig(RSTP_BRIDGE_CONFIG_ST *pstRstpBridgeConfig)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;
    int i = 0, j = 0;

    /* get all list instances with their content (recursive) */
    rc = get_items(sess, DEVICE_XPATH_PROTOCOLS_RSTP_HEADER_ALL, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), DEVICE_XPATH_PROTOCOLS_RSTP_HEADER_ALL);
        if (val != NULL)
            sr_free_values(val, cnt);
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        return rc;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, RSTP_XPATH_BRIDGE_NAME) != NULL) {
            strcpy(pstRstpBridgeConfig->acBridgeName, val[i].data.string_val);
        } else if (strstr(val[i].xpath, RSTP_XPATH_BRIDGE_PRIORITY) != NULL) {
            pstRstpBridgeConfig->u32BridgePriority = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, RSTP_XPATH_HOLD_TIME) != NULL) {
            pstRstpBridgeConfig->u32HoldTime = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, RSTP_XPATH_HELLO_TIME) != NULL) {
            pstRstpBridgeConfig->u32HelloTime = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, RSTP_XPATH_MAX_AGE) != NULL) {
            pstRstpBridgeConfig->u32MaxAge = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, RSTP_XPATH_FORWAED_DELAY) != NULL) {
            pstRstpBridgeConfig->u32ForwardDelay = val[i].data.uint32_val;
        } else if (strstr(val[i].xpath, RSTP_XPATH_TRANSMIT_HOLD_COUNT) != NULL) {
            pstRstpBridgeConfig->u32TransmitHoldCount = val[i].data.uint32_val;
        } else if (NULL != strstr(val[i].xpath, RSTP_XPATH_BRIDGE_PORT_TABLE)) {
            if (NULL != strstr(val[i].xpath, RSTP_XPATH_BRIDGE_IFNAME)) {
                j++;
                if (j >= L2_PORT_IFNAME_CNT) {
                    break;
                }
            } else if (NULL != strstr(val[i].xpath, RSTP_XPATH_BRIDGE_PORT_TABLE_COST)) {
                pstRstpBridgeConfig->u32Cost[j] = val[i].data.uint32_val;
            } else if (NULL != strstr(val[i].xpath, RSTP_XPATH_BRIDGE_PORT_TABLE_PRIORITY)) {
                pstRstpBridgeConfig->u32Priority[j] = val[i].data.uint32_val;
            }
        }
    }

    sr_free_values(val, cnt);
    return rc;
}

int
DSC_SetRstpCommon(char *pcXPath, uint32_t u32Value)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(sess, pcXPath, u32Value);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", pcXPath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetRstpCommon DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetRstpBridgePriority(uint32_t u32BridgePriority)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_BRIDGE_PRIORITY, u32BridgePriority);
}

int
DSC_SetRstpHoldTime(uint32_t u32HoldTime)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_HOLD_TIME, u32HoldTime);
}

int
DSC_SetRstpHelloTime(uint32_t u32HelloTime)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_HELLO_TIME, u32HelloTime);
}

int
DSC_SetRstpShutDown(uint32_t u32ShutDown)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_SHUT_DOWN, u32ShutDown);
}

int
DSC_SetRstpMaxAge(uint32_t u32MaxAge)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_MAX_AGE, u32MaxAge);
}

int
DSC_SetRstpForwardDelay(uint32_t u32ForwardDelay)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_FORWARD_DELAY, u32ForwardDelay);
}

int
DSC_SetRstpTransmitHoldCount(uint32_t u32TransmitHoldCount)
{
    return DSC_SetRstpCommon(DEVICE_XPATH_RSTP_TRANS_HOLD_COUNT, u32TransmitHoldCount);
}

int
DSC_SetRstpBridgePortPriority(L2_PORT_IFNAME_EN enPortIfname, uint32_t u32PortPriority)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    char acIfName[32] = {0};

    /* set port priority */
    memset(acIfName, 0, sizeof(acIfName));
    DSC_L2PortToStr(acIfName, enPortIfname);
    snprintf(acXpath, OPLK_PATH_MAX_LEN, DEVICE_XPATH_RSTP_BRIDGE_PORT_PRIORITY, acIfName);
    return DSC_SetRstpCommon(acXpath, u32PortPriority);
}

int
DSC_SetRstpBridgePortCost(L2_PORT_IFNAME_EN enPortIfname, uint32_t u32PortCost)
{
    char acXpath[OPLK_PATH_MAX_LEN];
    char acIfName[32] = {0};

    /* set port cost */
    memset(acIfName, 0, sizeof(acIfName));
    DSC_L2PortToStr(acIfName, enPortIfname);
    snprintf(acXpath, OPLK_PATH_MAX_LEN, DEVICE_XPATH_RSTP_BRIDGE_PORT_COST, acIfName);
    return DSC_SetRstpCommon(acXpath, u32PortCost);
}

