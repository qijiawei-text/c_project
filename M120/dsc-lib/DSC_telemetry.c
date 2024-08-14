/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_telemetry.h"




char *
DSC_GetModeFromCli(char *pcMode)
{
    char *pcStr = TELEMETRY_MODE_TARGET_DEFINED;

    if (0 == strcasecmp("all", pcMode)) {
        pcStr = TELEMETRY_MODE_ALL;
    } else if (0 == strcasecmp("strict-priority", pcMode)) {
        pcStr = TELEMETRY_MODE_STRICT_PRIORITY;
    } else if (0 == strcasecmp("target-defined", pcMode)) {
        pcStr = TELEMETRY_MODE_TARGET_DEFINED;
    }

    return pcStr;
}

int
DSC_SubsModeToEnum(char *pcMode)
{
    if (0 == strcmp(TELEMETRY_MODE_ALL, pcMode)) {
        return TELEMETRY_SUBS_MODE_ALL;
    } else if (0 == strcmp(TELEMETRY_MODE_STRICT_PRIORITY, pcMode)) {
        return TELEMETRY_SUBS_MODE_STRICT_PRIORITY;
    } else if (0 == strcmp(TELEMETRY_MODE_TARGET_DEFINED, pcMode)) {
        return TELEMETRY_SUBS_MODE_TARGET_DEFINED;
    }

    return TELEMETRY_SUBS_MODE_TARGET_DEFINED;
}

char *
DSC_SubsModeToStr(int iMode)
{
    char *pcModeStr = TELEMETRY_MODE_TARGET_DEFINED;

    switch (iMode) {
    case TELEMETRY_SUBS_MODE_ALL:
        pcModeStr = TELEMETRY_MODE_ALL;
        break;
    case TELEMETRY_SUBS_MODE_STRICT_PRIORITY:
        pcModeStr = TELEMETRY_MODE_STRICT_PRIORITY;
        break;
    case TELEMETRY_SUBS_MODE_TARGET_DEFINED:
        pcModeStr = TELEMETRY_MODE_TARGET_DEFINED;
        break;
    default:
        break;
    }

    return pcModeStr;
}

int
DSC_GetTelemetrySubsInfo(TELEMETRY_SUBSCRIPTION_GROUPS_ST *pstSubsInfo)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    int i = 0, j = 0, m = 0, n = 0;

    if (NULL == pstSubsInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input is null!");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(DSC_GetRunningSession(), TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST_ALL, &pstVal, &nCnt);
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

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "]/name") != NULL && strstr(pstVal[i].xpath, "/config/name") == NULL) {
            j++;
            m = -1;
            n = -1;
            if (j >= DSC_SUBSCRIPTIONS_NUM_MAX) {
                break;
            }
            LOG_WRITE(EN_LOG_DEBUG, "subscription name: %s", pstVal[i].data.string_val);
            strncpy(pstSubsInfo->astSubScriptionGroup[j].acSubsName, \
                    pstVal[i].data.string_val, DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/sensor-group") != NULL && strstr(pstVal[i].xpath, "/config/sensor-group") == NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "sensor-group name : %s", pstVal[i].data.string_val);
            m++;
            if (m >= DSC_IN_SUBSCRIPTION_SENSOR_GROUP_NUM_MAX) {
                break;
            }
            strncpy(pstSubsInfo->astSubScriptionGroup[j].astSensorGroupInfo[m].acName, \
                    pstVal[i].data.string_val, DSC_SENSOR_GROUP_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/config/sample-interval") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "sensor sample-interval : %lu", pstVal[i].data.uint64_val);
            pstSubsInfo->astSubScriptionGroup[j].astSensorGroupInfo[m].ullIntervalTime = pstVal[i].data.uint64_val;
        } else if (strstr(pstVal[i].xpath, "/config/heartbeat-interval") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "sensor heartbeat-interval : %lu", pstVal[i].data.uint64_val);
            pstSubsInfo->astSubScriptionGroup[j].astSensorGroupInfo[m].ullHeartBeat = pstVal[i].data.uint64_val;
            pstSubsInfo->astSubScriptionGroup[j].astSensorGroupInfo[m].bHasHeartbeat = true;
        } else if (strstr(pstVal[i].xpath, "/config/group-id") != NULL) {
            n++;
            if (n >= DSC_IN_SUBSCRIPTION_ADDR_GROUP_NUM_MAX) {
                break;
            }
            LOG_WRITE(EN_LOG_DEBUG, "dest-group name : %s", pstVal[i].data.string_val);
            strncpy(pstSubsInfo->astSubScriptionGroup[j].aacAddrGroupName[n], \
                    pstVal[i].data.string_val, DSC_ADDR_GROUP_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/protocol") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "protoco : %s", pstVal[i].data.identityref_val);
        } else if (strstr(pstVal[i].xpath, "/encoding") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "encoding : %s", pstVal[i].data.identityref_val);
        } else if (strstr(pstVal[i].xpath, "/config/com-alibaba-telemetry-ext:telemetry-mode") != NULL) {
            pstSubsInfo->astSubScriptionGroup[j].iMode = DSC_SubsModeToEnum(pstVal[i].data.enum_val);
        }
    }

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_GetTelemetrySensorInfo(TELEMETRY_SENSOR_INFO_ST *pstSensorInfo)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    int i = 0, j = 0, k = 0;

    if (NULL == pstSensorInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input is null!");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(DSC_GetRunningSession(), TELEMETRY_SENSOR_GROUPS_LIST_ALL, &pstVal, &nCnt);
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

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/sensor-group-id") != NULL && strstr(pstVal[i].xpath, "/config/sensor-group-id") == NULL) {
            j++;
            k = -1;
            if (j >= DSC_SENSOR_GROUP_NUM_MAX) {
                break;
            }

            strncpy(pstSensorInfo[j].acName, \
                    pstVal[i].data.string_val, DSC_SENSOR_GROUP_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/config/path") != NULL) {
            k++;
            if (k >= DSC_IN_SENSOR_GROUP_XPATH_NUM_MAX) {
                break;
            }
            strncpy(pstSensorInfo[j].astPath[k].acPath, \
                    pstVal[i].data.string_val, DSC_XPATH_LEN_MAX - 1);
        } else {
        }
    }

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_GetTelemetryAddrInfo(bool bIsRunningSess, TELEMETRY_ADDR_GROUP_ST *pstAddrInfo)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    int i = 0, j = 0, k = 0;
    sr_session_ctx_t *pstSess;

    if (NULL == pstAddrInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input is null!");
        return SR_ERR_INVAL_ARG;
    }

    if (bIsRunningSess) {
        pstSess = DSC_GetRunningSession();
    } else {
        pstSess = DSC_GetOperSession();
    }

    rc = get_items(pstSess, TELEMETRY_DEST_GROUPS_LIST_ALL, &pstVal, &nCnt);
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

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/group-id") != NULL &&
            strstr(pstVal[i].xpath, "/config/group-id") == NULL &&
            strstr(pstVal[i].xpath, "/state/group-id") == NULL) {
            j++;
            k = -1;
            if (j >= DSC_ADDR_GROUP_NUM_MAX) {
                break;
            }

            strncpy(pstAddrInfo[j].acName, \
                    pstVal[i].data.string_val, DSC_ADDR_GROUP_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/config/destination-address") != NULL) {
            k++;
            if (k >= DSC_IN_ADDR_GROUP_IP_NUM_MAX) {
                break;
            }
            strncpy(pstAddrInfo[j].astRemoteAddr[k].acIpAddress, \
                    pstVal[i].data.string_val, DSC_IP_ADDRESS_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/config/destination-port") != NULL) {
            pstAddrInfo[j].astRemoteAddr[k].usPort = pstVal[i].data.uint16_val;
        } else if (strstr(pstVal[i].xpath, "/config/com-alibaba-telemetry-ext:priority") != NULL) {
            pstAddrInfo[j].astRemoteAddr[k].usPri = pstVal[i].data.uint16_val;
        } else if (strstr(pstVal[i].xpath, "/state/com-alibaba-telemetry-ext:conn-state") != NULL) {
            strncpy(pstAddrInfo[j].astRemoteAddr[k].acConnState, pstVal[i].data.enum_val, sizeof(pstAddrInfo[j].astRemoteAddr[k].acConnState) - 1);
        } else if (strstr(pstVal[i].xpath, "/state/com-alibaba-telemetry-ext:is-selected") != NULL) {
            pstAddrInfo[j].astRemoteAddr[k].bIsSelected = pstVal[i].data.bool_val;
        }
    }

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_AddPathToSensorGroup(char *pcSensorGroupName, char *pcPath)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acSensorGroup[DSC_SENSOR_GROUP_NAME_MAX_LEN];

    if (NULL == pcSensorGroupName || NULL == pcPath) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddPathToSensorGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acSensorGroup, 0, sizeof(acSensorGroup));
    sprintf(acXpath, TELEMETRY_SENSOR_GROUPS_LIST_CONFIG_GROUP_ID, pcSensorGroupName);
    rc = get_string_item(pstSess, acXpath, acSensorGroup, sizeof(acSensorGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SENSOR_GROUPS_LIST_CONFIG_GROUP_ID, pcSensorGroupName);
        rc = set_string_item(pstSess, acXpath, pcSensorGroupName, DSC_SENSOR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST_CONFIG_PATH, pcSensorGroupName, pcPath);
    rc = set_string_item(pstSess, acXpath, pcPath, DSC_XPATH_LEN_MAX);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddPathToSensorGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelPathFromSensorGroup(char *pcSensorGroupName, char *pcPath)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSensorGroupName || NULL == pcPath) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelPathFromSensorGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST, pcSensorGroupName, pcPath);

    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelPathFromSensorGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelSensorGroup(char *pcSensorGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSensorGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSensorGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SENSOR_GROUPS_LIST, pcSensorGroupName);

    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error del_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSensorGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AddAddrToDestGroup(char *pcDestGroupName, char *pcIp, uint16_t usPort, bool bHasPri, uint16_t usPri)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acDestGroup[DSC_ADDR_GROUP_NAME_MAX_LEN];

    if (NULL == pcDestGroupName || NULL == pcIp) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddAddrToDestGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acDestGroup, 0, sizeof(acDestGroup));
    sprintf(acXpath, TELEMETRY_DEST_GROUPS_LIST_CONFIG_GROUP_ID, pcDestGroupName);
    rc = get_string_item(pstSess, acXpath, acDestGroup, sizeof(acDestGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST_CONFIG_GROUP_ID, pcDestGroupName);
        rc = set_string_item(pstSess, acXpath, pcDestGroupName, DSC_ADDR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_IP, pcDestGroupName, pcIp, usPort);
    rc = set_string_item(pstSess, acXpath, pcIp, DSC_XPATH_LEN_MAX);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_PORT, pcDestGroupName, pcIp, usPort);
    rc = set_uint16_item(pstSess, acXpath, usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_uint16_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    if (true == bHasPri) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_PRIORITY, pcDestGroupName, pcIp, usPort);
        rc = set_uint16_item(pstSess, acXpath, usPri);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_uint16_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddAddrToDestGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelAddrFromDestGroup(char *pcDestGroupName, char *pcIp, uint16_t usPort)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcDestGroupName || NULL == pcIp) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelAddrFromDestGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST_DEST_LIST, pcDestGroupName, pcIp, usPort);
    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error del_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelAddrFromDestGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelDestGroup(char *pcDestGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcDestGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelDestGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_DEST_GROUPS_LIST, pcDestGroupName);

    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error del_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelDestGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_AddSubscription(char *pcSubsGroupName, char *pcMode, char *pcSensorGroupName, char *pcDestGroupName,
                    uint64_t ullSampleInterval, bool bHasHeartbeat, uint64_t ullHeartbeatInterval)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    char *pcModeTmp = NULL;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acSubsGroup[DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN];
    char acDestGroup[DSC_ADDR_GROUP_NAME_MAX_LEN];
    char acSensorGroup[DSC_SENSOR_GROUP_NAME_MAX_LEN];

    if (NULL == pcSubsGroupName || NULL == pcSensorGroupName || NULL == pcDestGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddSubscription input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acSubsGroup, 0, sizeof(acSubsGroup));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_NAME, pcSubsGroupName);
    rc = get_string_item(pstSess, acXpath, acSubsGroup, sizeof(acSubsGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_NAME, pcSubsGroupName);
        rc = set_string_item(pstSess, acXpath, pcSubsGroupName, DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_PROTOCOL, pcSubsGroupName);
    rc = set_identityref_item(pstSess, acXpath, "openconfig-telemetry-types:STREAM_GRPC", strlen("openconfig-telemetry-types:STREAM_GRPC"));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_identityref_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_ENCODING, pcSubsGroupName);
    rc = set_identityref_item(pstSess, acXpath, "openconfig-telemetry-types:ENC_JSON_IETF", strlen("openconfig-telemetry-types:ENC_JSON_IETF"));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_identityref_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    pcModeTmp = DSC_GetModeFromCli(pcMode);
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_MODE, pcSubsGroupName);
    rc = set_enum_item(pstSess, acXpath, pcModeTmp);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_enum_item: %s %s %s", acXpath, pcModeTmp, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acDestGroup, 0, sizeof(acDestGroup));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ID, pcSubsGroupName, pcDestGroupName);
    rc = get_string_item(pstSess, acXpath, acDestGroup, sizeof(acDestGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ID, pcSubsGroupName, pcDestGroupName);
        rc = set_string_item(pstSess, acXpath, pcDestGroupName, DSC_ADDR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acSensorGroup, 0, sizeof(acSensorGroup));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_GROUP_IP, pcSubsGroupName, pcSensorGroupName);
    rc = get_string_item(pstSess, acXpath, acSensorGroup, sizeof(acSensorGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_GROUP_IP, pcSubsGroupName, pcSensorGroupName);
        rc = set_string_item(pstSess, acXpath, pcSensorGroupName, DSC_SENSOR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_SAMPLE,
             pcSubsGroupName, pcSensorGroupName);
    rc = set_uint64_item(pstSess, acXpath, ullSampleInterval);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    if (true == bHasHeartbeat) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_HEARTBEAT,
                 pcSubsGroupName, pcSensorGroupName);
        rc = set_uint64_item(pstSess, acXpath, ullHeartbeatInterval);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddPathToSensorGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelSubsGroup(char *pcSubsGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSubsGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsGroup input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST, pcSubsGroupName);

    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error del_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsGroup DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetSubsSensor(char *pcSubsGroupName, char *pcSensorGroupName,
                  uint64_t ullSampleInterval, bool bHasHeartbeat, uint64_t ullHeartbeatInterval)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acSensorGroup[DSC_SENSOR_GROUP_NAME_MAX_LEN];

    if (NULL == pcSubsGroupName || NULL == pcSensorGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsSensor input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acSensorGroup, 0, sizeof(acSensorGroup));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_GROUP_IP, pcSubsGroupName, pcSensorGroupName);
    rc = get_string_item(pstSess, acXpath, acSensorGroup, sizeof(acSensorGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_GROUP_IP, pcSubsGroupName, pcSensorGroupName);
        rc = set_string_item(pstSess, acXpath, pcSensorGroupName, DSC_SENSOR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_SAMPLE,
             pcSubsGroupName, pcSensorGroupName);
    rc = set_uint64_item(pstSess, acXpath, ullSampleInterval);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    if (true == bHasHeartbeat) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_HEARTBEAT,
                 pcSubsGroupName, pcSensorGroupName);
        rc = set_uint64_item(pstSess, acXpath, ullHeartbeatInterval);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsSensor DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelSubsSensor(char *pcSubsGroupName, char *pcSensorGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSensorGroupName || NULL == pcSensorGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsSensor input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST, pcSubsGroupName, pcSensorGroupName);
    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsSensor DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetSubsMode(char *pcSubsGroupName, char *pcMode)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSubsGroupName || NULL == pcMode) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsMode input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_MODE, pcSubsGroupName);
    rc = set_enum_item(pstSess, acXpath, pcMode);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_enum_item: %s %s %s", acXpath, pcMode, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsMode DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetSubsDest(char *pcSubsGroupName, char *pcDestGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acDestGroup[DSC_ADDR_GROUP_NAME_MAX_LEN];

    if (NULL == pcSubsGroupName || NULL == pcDestGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsDest input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acDestGroup, 0, sizeof(acDestGroup));
    sprintf(acXpath, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ID, pcSubsGroupName, pcDestGroupName);
    rc = get_string_item(pstSess, acXpath, acDestGroup, sizeof(acDestGroup));
    if (SR_ERR_OK == rc) {
    } else if (SR_ERR_NOT_FOUND == rc) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ID, pcSubsGroupName, pcDestGroupName);
        rc = set_string_item(pstSess, acXpath, pcDestGroupName, DSC_ADDR_GROUP_NAME_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
            return rc;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item error %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSubsDest DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_DelSubsDest(char *pcSubsGroupName, char *pcDestGroupName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcSubsGroupName || NULL == pcDestGroupName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsDest input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST, pcSubsGroupName, pcDestGroupName);
    rc = del_item(pstSess, acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set_string_item: %s %s", acXpath, sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelSubsDest DSC_Commit error %s", sr_strerror(rc));
    }

    return rc;
}





