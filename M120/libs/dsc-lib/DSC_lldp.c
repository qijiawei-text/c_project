/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_lldp.h"

int
DSC_GetLldpPortName(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "interface", "name", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, LLDP_STR_LEN);
    sr_xpath_recover(&state);

    return 0;
}

int
DSC_GetLldpCouters(char *portIf, LLDP_COUNTER_ST *counters)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *value = NULL;
    size_t cnt = 0;
    int idx = 0;
    char xpath[OPLK_PATH_MAX_LEN] = {0};


    if (portIf == NULL) {
        /*top level counters */
        snprintf(xpath, OPLK_PATH_MAX_LEN, "%s", XPATH_LLDP_STATE);
    } else {
        /*individule port counters */
        snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_LLDP_PORT_STATE, portIf);
    }

    rc = get_items(sess, xpath, &value, &cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        return rc;
    }

    for (idx = 0; idx < cnt; idx++) {
        if (sr_xpath_node_name_eq(value[idx].xpath, LLDP_FRAME_IN)) {
            counters->frame_in = value[idx].data.uint64_val;
        } else if (sr_xpath_node_name_eq(value[idx].xpath, LLDP_FRAME_OUT)) {
            counters->frame_out = value[idx].data.uint64_val;
        } else if (sr_xpath_node_name_eq(value[idx].xpath, LLDP_FRAME_DISGARD)) {
            counters->frame_disgard = value[idx].data.uint64_val;
        } else if (sr_xpath_node_name_eq(value[idx].xpath, LLDP_FRAME_ERROR_IN)) {
            counters->frame_error_in = value[idx].data.uint64_val;
        } else if (sr_xpath_node_name_eq(value[idx].xpath, LLDP_LAST_CLEAR)) {
            snprintf(counters->last_clear, 64, "%s", value[idx].data.string_val);
        }
    }
    sr_free_values(value, cnt);
    return rc;
}

int
DSC_GetLldpNeighbors(char *port_name, LLDP_PORT_STATE_ST *portState)
{
    int rc = SR_ERR_OK;
    sr_data_t *pstSubtrees = NULL;
    size_t nNbrCnt = 0;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[32] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_LLDP_NBR_LIST, port_name);

    rc = get_subtrees(sess, xpath, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        portState->neighbor_cnt = nNbrCnt;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, xpath, &nodeSet);
        if (LY_SUCCESS == rc) {
            nNbrCnt = nodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "sr_get_subtrees rc: %s, count: %zu", sr_strerror(rc), nNbrCnt);

    portState->neighbor_cnt = nNbrCnt;

    for (i = 0; i < nNbrCnt; i++) {
        ulLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);
        for (j = 0; j < ulLeafCnt; j++) {
            pstLeaf = (sr_node_t *)apstLeaf[j];
            if (0 == strcmp(pstLeaf->schema->name, LLDP_NBR_ID)) {
                LOG_WRITE(EN_LOG_DEBUG, "neighbot_id: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].neighbor_id, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_SYSTEM_NAME)) {
                LOG_WRITE(EN_LOG_DEBUG, "system name: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteSysName, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_SYSTEM_DESC)) {
                LOG_WRITE(EN_LOG_DEBUG, "system description: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteSysDesc, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_CHASSIS_ID)) {
                LOG_WRITE(EN_LOG_DEBUG, "chassis is: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteChassisId, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_CHASSIS_ID_TYPE)) {
                LOG_WRITE(EN_LOG_DEBUG, "chassis id type is: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteChassisIdSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_MANAGEMENT_IP)) {
                LOG_WRITE(EN_LOG_DEBUG, "mgmt ip: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteMgmtAddress, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_MANAGEMENT_IP_TYPE)) {
                LOG_WRITE(EN_LOG_DEBUG, "mgmt ip type: %s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteMgmtAddressSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_ID)) {
                LOG_WRITE(EN_LOG_DEBUG, "Port id :%s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remotePortId, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_ID_TYPE)) {
                LOG_WRITE(EN_LOG_DEBUG, "port id type :%s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remotePortIdSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_LAST_UPDATE)) {
                LOG_WRITE(EN_LOG_DEBUG, "last claer :%s", lyd_get_value(pstLeaf));
                strncpy(portState->neighbor[i].remoteLastUpdate, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            }

            /*TODO: get custom tlvs. */
        }
    }

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
DSC_GetLldpPortState(LLDP_PORT_STATE_ST *portState)
{
    int rc = SR_ERR_OK;
    sr_data_t *pstSubtrees = NULL;
    size_t nPortCnt = 0;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[32] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /*get port state, only top level attributes */
    rc = get_subtrees(sess, XPATH_LLDP_PORT_LIST_STATE, 2, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        nPortCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_LLDP_PORT_LIST_STATE, &nodeSet);
        if (LY_SUCCESS == rc) {
            nPortCnt = nodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "sr_get_subtrees rc: %s, count: %zu", sr_strerror(rc), nPortCnt);

    for (i = 0; i < nPortCnt; i++) {
        ulLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);

        for (j = 0; j < ulLeafCnt; j++) {
            pstLeaf = (sr_node_t *)apstLeaf[j];
            if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_NAME)) {
                LOG_WRITE(EN_LOG_DEBUG, "name: %s", lyd_get_value(pstLeaf));
                strncpy(portState[i].port_name, lyd_get_value(pstLeaf), LLDP_STR_LEN);
                /*get port counters */
                rc = DSC_GetLldpCouters(portState[i].port_name, &(portState[i].counters));
            }

            rc = DSC_GetLldpNeighbors(portState[i].port_name, &portState[i]);
        }
    }

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
DSC_GetLldpState(LLDP_STATE_ST *pstLldpState)
{
    int rc = SR_ERR_OK;

    rc = DSC_GetLldpCouters(NULL, &(pstLldpState->counters));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetLldpCouters top level");
        return rc;
    }

    DSC_GetLldpPortState(pstLldpState->portState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetPortStated");
    }

    return rc;
}

int
DSC_GetLldpTlvConfig(char *acPortName, LLDP_CUSTOM_TLV_ST *customTlv, int *tlvCnt)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_data_t *pstSubtrees = NULL;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[32] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /*get tlv config */
    snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_LLDP_PORT_CONFIG_TLV_ALL, acPortName);
    rc = get_subtrees(sess, xpath, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        *tlvCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_LLDP_PORT_LIST_CONFIG, &nodeSet);
        if (LY_SUCCESS == rc) {
            *tlvCnt = nodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "sr_get_subtrees rc: %s, count: %d", sr_strerror(rc), *tlvCnt);

    for (i = 0; i < *tlvCnt; i++) {
        ulLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);

        for (j = 0; j < ulLeafCnt; j++) {
            pstLeaf = (sr_node_t *)apstLeaf[j];
            if (0 == strcmp(pstLeaf->schema->name, LLDP_TLV_TYPE)) {
                LOG_WRITE(EN_LOG_DEBUG, "Type: %d", ((sr_node_term_t *)pstLeaf)->value.int32);
                customTlv[i].type = ((sr_node_term_t *)pstLeaf)->value.int32;
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_TLV_OUI)) {
                LOG_WRITE(EN_LOG_DEBUG, "OUI: %s", lyd_get_value(pstLeaf));
                strncpy(customTlv[i].oui, lyd_get_value(pstLeaf), LLDP_OUI_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_TLV_OUI_TYPE)) {
                LOG_WRITE(EN_LOG_DEBUG, "OUI type: %s", lyd_get_value(pstLeaf));
                strncpy(customTlv[i].ouiSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_TLV_VALUE)) {
                LOG_WRITE(EN_LOG_DEBUG, "tlv Local value: %s", lyd_get_value(pstLeaf));
                strncpy(customTlv[i].ouiSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            }
        }
    }

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
DSC_GetLldpPortConfig(LLDP_PORT_CONFIG_ST *portConfig)
{
    int rc = SR_ERR_OK;
    sr_data_t *pstSubtrees = NULL;
    size_t nPortCnt = 0;
    int i = 0, j = 0;
    uint32_t ulLeafCnt = 0;
    sr_node_t *apstLeaf[32] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /*get port config, only 2 level*/
    rc = get_subtrees(sess, XPATH_LLDP_PORT_LIST_CONFIG, 2, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        nPortCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_subtrees error: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, XPATH_LLDP_PORT_LIST_CONFIG, &nodeSet);
        if (LY_SUCCESS == rc) {
            nPortCnt = nodeSet->count;
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "sr_get_subtrees rc: %s, count: %zu", sr_strerror(rc), nPortCnt);

    for (i = 0; i < nPortCnt; i++) {
        ulLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);

        for (j = 0; j < ulLeafCnt; j++) {
            pstLeaf = (sr_node_t *)apstLeaf[j];
            if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_NAME)) {
                LOG_WRITE(EN_LOG_NOTICE, "port name: %s", lyd_get_value(pstLeaf));
                strncpy(portConfig[i].acPortName, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_DESC)) {
                LOG_WRITE(EN_LOG_NOTICE, "port description: %s", lyd_get_value(pstLeaf));
                strncpy(portConfig[i].acPortDescription, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_ENABLED)) {
                LOG_WRITE(EN_LOG_NOTICE, "prot enabled: %d", ((sr_node_term_t *)pstLeaf)->value.boolean);
                portConfig[i].enabled = ((sr_node_term_t *)pstLeaf)->value.boolean;
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_ID_TYPE)) {
                LOG_WRITE(EN_LOG_NOTICE, "port ID type: %s", lyd_get_value(pstLeaf));
                strncpy(portConfig[i].acPortIdSubType, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            } else if (0 == strcmp(pstLeaf->schema->name, LLDP_PORT_LOCAL)) {
                LOG_WRITE(EN_LOG_NOTICE, "port Local value: %s", lyd_get_value(pstLeaf));
                strncpy(portConfig[i].acLocalValue, lyd_get_value(pstLeaf), LLDP_STR_LEN);
            }

            rc = DSC_GetLldpTlvConfig(portConfig[i].acPortName, portConfig[i].customTlv, (int *)&portConfig[i].tlv_count);
        }
    }

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
DSC_GetLldpConfig(LLDP_CONFIG_ST *pstLldpConfig)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;
    int i = 0;

    /*get top level config */
    rc = get_items(sess, XPATH_LLDP_CONFIG_ALL, &val, &cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        return rc;
    }

    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_ENABLED) != NULL) {
            pstLldpConfig->enabled = val[i].data.bool_val;
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_TXINTERVAL) != NULL) {
            pstLldpConfig->txInterval = val[i].data.uint64_val;
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_HOLDMULTIPLIER) != NULL) {
            pstLldpConfig->holdMultiplier = val[i].data.uint64_val;
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_SYSTEM_NAME) != NULL) {
            strncpy(pstLldpConfig->acSysName, val[i].data.string_val, LLDP_STR_LEN - 1);
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_SYSTEM_DESC) != NULL) {
            strncpy(pstLldpConfig->acSysDescription, val[i].data.string_val, LLDP_STR_LEN - 1);
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_CHASSIS_ID) != NULL) {
            strncpy(pstLldpConfig->acChassisId, val[i].data.string_val, LLDP_STR_LEN - 1);
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_CHASSIS_ID_TYPE) != NULL) {
            strncpy(pstLldpConfig->acChassisIdSubType, val[i].data.string_val, LLDP_STR_LEN - 1);
        } else if (strstr(val[i].xpath, XPATH_LLDP_CONFIG_MANAGEMENT_IP) != NULL) {
            strncpy(pstLldpConfig->acMgmtAddress, val[i].data.string_val, LLDP_STR_LEN - 1);
        }
    }

    /*Get port config */
    DSC_GetLldpPortConfig(pstLldpConfig->stPortConfig);

    sr_free_values(val, cnt);
    return rc;
}

bool
DSC_IsLldpdRunning()
{
    int rc = 0;
    rc = system("ps aux | grep lldpd | grep -v grep");

    if (0 == rc) {
        return true;
    } else {
        return false;
    }
}

bool
DSC_GetLldpGlobalAdminStatus()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    bool enabled;

    sess = DSC_GetOperSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = get_bool_item(sess, XPATH_LLDP_CONFIG_ENABLED, &enabled);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_bool_item: %s, %s", XPATH_LLDP_CONFIG_ENABLED, sr_strerror(rc));
        goto ERR_LABEL;
    }


    if (!DSC_IsLldpdRunning()) {
        return false;
    }

ERR_LABEL:
    return enabled;
}

int
DSC_SetLldpPortAdminStatus(char *portName, bool enabled)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN];
    char acIfName[32] = {0};

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* set port adminstataus */
    memset(acIfName, 0, sizeof(acIfName));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, XPATH_LLDP_PORT_CONFIG_ENABLED, portName);
    rc = set_bool_item(sess, acXpath, enabled);
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


int
DSC_SetLldpGlobalAdminStatus(bool enabled)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();

    rc = set_bool_item(sess, XPATH_LLDP_CONFIG_ENABLED, enabled);
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

int
DSC_SetLldpGlobalMsgTxInterval(uint32_t u32MsgTxInterval)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(sess, XPATH_LLDP_CONFIG_TXINTERVAL, u32MsgTxInterval);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", XPATH_LLDP_CONFIG_TXINTERVAL, sr_strerror(rc));
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
DSC_SetLldpGlobalMsgTxHoldMultiplier(uint32_t u32MsgTxHoldMultiplier)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_uint32_item(sess, XPATH_LLDP_CONFIG_HOLDMULTIPLIER, u32MsgTxHoldMultiplier);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", XPATH_LLDP_CONFIG_HOLDMULTIPLIER, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}
