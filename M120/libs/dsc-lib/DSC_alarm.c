/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_alarm.h"
#include "oplkalm.h"
/* TODO update liboplkalm                                                                         */

static ALARM_INFO_ST m_astAlm[MAX_ALM] = {{{0}, {0}, {0}, 0, {0}, 0, 0, {0}, {0}, {0}, 0}};
static DSC_event_notif_cb event_callback = NULL;
static sr_subscription_ctx_t *event_sub = NULL;

static SEVERITY_EN
__alarmSeverityToEnum(char *pcStr)
{
    SEVERITY_EN enSeverity = SEVERITY_INDETERMINATE;

    if (0 == strncmp(pcStr, ALARM_CRITICAL_STR, strlen(ALARM_CRITICAL_STR))) {
        enSeverity = SEVERITY_CRITICAL;
    } else if (0 == strncmp(pcStr, ALARM_MAJOR_STR, strlen(ALARM_MAJOR_STR))) {
        enSeverity = SEVERITY_MAJOR;
    } else if (0 == strncmp(pcStr, ALARM_MINOR_STR, strlen(ALARM_MINOR_STR))) {
        enSeverity = SEVERITY_MINOR;
    } else if (0 == strncmp(pcStr, ALARM_WARNING_STR, strlen(ALARM_WARNING_STR))) {
        enSeverity = SEVERITY_WARNING;
    } else if (0 == strncmp(pcStr, ALARM_CLEAR_STR, strlen(ALARM_CLEAR_STR))) {
        enSeverity = SEVERITY_CLEAR;
    }

    return enSeverity;
}

static REOURCE_TYPE_EN
__resourceTypeToEnum(char *pcStr)
{
    REOURCE_TYPE_EN enResource = RESOURCE_TYPE_OTHER;

    if (0 == strncmp(pcStr, ALARM_RSC_DEVICE_STR, strlen(ALARM_RSC_DEVICE_STR))) {
        enResource = RESOURCE_TYPE_DEVICE;
    } else if (0 == strncmp(pcStr, ALARM_RSC_PORT_STR, strlen(ALARM_RSC_PORT_STR))) {
        enResource = RESOURCE_TYPE_PORT;
    } else if (0 == strncmp(pcStr, ALARM_RSC_SERVICE_STR, strlen(ALARM_RSC_SERVICE_STR))) {
        enResource = RESOURCE_TYPE_SERVICE;
    } else if (0 == strncmp(pcStr, ALARM_RSC_CPACK_STR, strlen(ALARM_RSC_CPACK_STR))) {
        enResource = RESOURCE_TYPE_CIRCUIT_PACK;
    }

    return enResource;
}

static ALARM_TYPE_EN
__alarmTypeToEnum(char *pcStr)
{
    ALARM_TYPE_EN enAlarm = ALARM_TYPE_UNKNOWN;

    if (0 == strncmp(pcStr, ALARM_TYPE_COMM_STR, strlen(ALARM_TYPE_COMM_STR))) {
        enAlarm = ALARM_TYPE_COMMUNICATION;
    } else if (0 == strncmp(pcStr, ALARM_TYPE_QOS_STR, strlen(ALARM_TYPE_QOS_STR))) {
        enAlarm = ALARM_TYPE_QOS;
    } else if (0 == strncmp(pcStr, ALARM_TYPE_PROC_STR, strlen(ALARM_TYPE_PROC_STR))) {
        enAlarm = ALARM_TYPE_PROCESSING;
    } else if (0 == strncmp(pcStr, ALARM_TYPE_EQUIP_STR, strlen(ALARM_TYPE_EQUIP_STR))) {
        enAlarm = ALARM_TYPE_EQUIPMENT;
    } else if (0 == strncmp(pcStr, ALARM_TYPE_ENV_STR, strlen(ALARM_TYPE_ENV_STR))) {
        enAlarm = ALARM_TYPE_ENVIRONMENTAL;
    }

    return enAlarm;
}


int
DSC_BuildAlarmList(const sr_val_t *val, const size_t cnt, ALARM_INFO_ST **ppstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    ALARM_INFO_ST *pstAlm = NULL;
    int i = 0, j = 0;

    memset(m_astAlm, 0x00, sizeof(m_astAlm));
    pstAlm = (ALARM_INFO_ST *)&m_astAlm;
    j = -1;
    for (i = 0; i < cnt; i++) {
        /* TODO: fill in the alarm structure                                    */
        if (strstr(val[i].xpath, "/id") != NULL) {
            j++;
            if (j >= MAX_ALM) {
                break;
            }
            strncpy(pstAlm[j].acAlarmId, val[i].data.string_val, sizeof(pstAlm[j].acAlarmId) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_RAISETIME) != NULL) {
            strncpy(pstAlm[j].acRaiseTime, val[i].data.string_val, sizeof(pstAlm[j].acRaiseTime) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_CIRCUIT_PACK) != NULL) {
            strncpy(pstAlm[j].acCircuitPack, val[i].data.string_val, sizeof(pstAlm[j].acCircuitPack) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_SERVERITY) != NULL) {
            strcpy(pstAlm[j].acSeverity, val[i].data.enum_val);
            pstAlm[j].enSeverity = __alarmSeverityToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_RESOURCE) != NULL) {
            strcpy(pstAlm[j].acResource, val[i].data.enum_val);
            pstAlm[j].enResourceType = __resourceTypeToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_TYPE) != NULL) {
            pstAlm[j].enAlarmType = __alarmTypeToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_CAUSE_EXT) != NULL) {
            strncpy(pstAlm[j].acCaseExt, val[i].data.string_val, sizeof(pstAlm[j].acCaseExt) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_ADD_DETAIL) != NULL) {
            strncpy(pstAlm[j].acDetail, val[i].data.string_val, sizeof(pstAlm[j].acDetail) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_EXT_ID_OOG) != NULL) {
            strncpy(pstAlm[j].acExternalId, val[i].data.string_val, sizeof(pstAlm[j].acExternalId) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_EXT_ENTITY_OOG) != NULL) {
            strncpy(pstAlm[j].acEntity, val[i].data.string_val, sizeof(pstAlm[j].acEntity) - 1);
        }
        /* sr_print_val(val + i);                                               */
    }

    *ppstAlarm = pstAlm;
    *count = j + 1;

    return rc;
}

static int
__BuildSupportAlarmList(const sr_val_t *val, const size_t cnt, ALARM_INFO_ST *pstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    j = -1;

    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, "/id") != NULL) {
            j++;
            if (j >= MAX_ALM) {
                break;
            }
            strncpy(pstAlarm[j].acAlarmId, val[i].data.string_val, sizeof(pstAlarm[j].acAlarmId) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_RAISETIME) != NULL) {
            strncpy(pstAlarm[j].acRaiseTime, val[i].data.string_val, sizeof(pstAlarm[j].acRaiseTime) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_CIRCUIT_PACK) != NULL) {
            strncpy(pstAlarm[j].acCircuitPack, val[i].data.string_val, sizeof(pstAlarm[j].acCircuitPack) - 1);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_SERVERITY) != NULL) {
            strcpy(pstAlarm[j].acSeverity, val[i].data.enum_val);
            pstAlarm[j].enSeverity = __alarmSeverityToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_RESOURCE) != NULL) {
            strcpy(pstAlarm[j].acResource, val[i].data.enum_val);
            pstAlarm[j].enResourceType = __resourceTypeToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_TYPE) != NULL) {
            pstAlarm[j].enAlarmType = __alarmTypeToEnum(val[i].data.enum_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_NOTIF_CAUSE_EXT) != NULL) {
            strcpy(pstAlarm[j].acCaseExt, val[i].data.string_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_MASK_NODE) != NULL) {
            pstAlarm[j].bMask = val[i].data.bool_val;
        } else if (strstr(val[i].xpath, ALARM_XPATH_EXT_ID) != NULL) {
            strcpy(pstAlarm[j].acExternalId, val[i].data.string_val);
        } else if (strstr(val[i].xpath, ALARM_XPATH_EXT_ENTITY) != NULL) {
            strcpy(pstAlarm[j].acEntity, val[i].data.string_val);
        }
    }

    *count = j + 1;

    return rc;
}
int
DSC_GetAlarmList(ALARM_INFO_ST **ppstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;

    /* get all list instances with their content (recursive)                    */
    rc = sr_get_items(sess, ALARM_XPATH_LIST, 0, 0, &val, &cnt);
    /* rc = sr_get_items(sess, "/com-oplink-alarm:active-alarm-list/activeAlarms// *", 0, 0, &val, &cnt);*/
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_LIST);

        if (val != NULL)
            sr_free_values(val, cnt);

        *count = 0;
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        *count = 0;
        return rc;
    }

    rc = DSC_BuildAlarmList(val, cnt, ppstAlarm, count);
    sr_free_values(val, cnt);
    return rc;
}

int
DSC_GetSupportAlarmList(ALARM_INFO_ST *pstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;

    rc = sr_get_items(sess, ALARM_XPATH_SUPPORTED_LIST, 0, SR_OPER_NO_STATE, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_SUPPORTED_LIST);
        if (val != NULL)
            sr_free_values(val, cnt);
        *count = 0;
        return SR_ERR_OK;
    }
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        *count = 0;
        return rc;
    }
    rc = __BuildSupportAlarmList(val, cnt, pstAlarm, count);
    sr_free_values(val, cnt);

    return rc;
}

static void
event_notif_cb(sr_session_ctx_t *session, uint32_t sub_id, const sr_ev_notif_type_t notif_type,
               const char *xpath, const sr_val_t *values, const size_t values_cnt, struct timespec *timestamp, void *private_data)
{
    int rc = SR_ERR_OK;
    ALARM_INFO_ST *pstAlarm;
    size_t count;

    LOG_WRITE(EN_LOG_DEBUG, "========== RECEIVED EVENT NOTIFICATION ==========");
    rc = DSC_BuildAlarmList(values, values_cnt, &pstAlarm, &count);

    rc = event_callback(pstAlarm, count);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by event_callback: %s\n", sr_strerror(rc));
    }
}

int
DSC_WriteCurrentAlarm(sr_session_ctx_t *pstSess, ALARM_INFO_ST *pstAlarm)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acXpathUpdate[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstVal = NULL;
    sr_val_t stWriteVal;
    size_t nCnt = 0;

    if (NULL == pstAlarm || NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, ALARM_XPATH_SUPPORTED, pstAlarm->acAlarmId);
    rc = sr_get_items(DSC_GetOperSession(), acXpath, 0, SR_OPER_NO_STATE, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_SUPPORTED);

        rc = SR_ERR_OK;
        goto cleanup;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (0 != strcmp(pstAlarm->acSeverity, "clear")) {
        /* raise                                                                */
        for (int i = 0; i < nCnt; i++) {

            if ((pstVal[i].type != SR_CONTAINER_T) &&
                (pstVal[i].type != SR_CONTAINER_PRESENCE_T) &&
                (pstVal[i].type != SR_LIST_T) &&
                (NULL == strstr(pstVal[i].xpath, ALARM_XPATH_CHILD_NODE)) &&
                (NULL == strstr(pstVal[i].xpath, ALARM_XPATH_PARENT_NODE)) &&
                (NULL == strstr(pstVal[i].xpath, ALARM_XPATH_MASK_NODE))) {

                memset(acXpath, 0, sizeof(acXpath));
                strcpy(acXpath, pstVal[i].xpath);
                UTIL_StrRpc(acXpath, "supported-alarm-list/alarm-definition", "active-alarm-list/activeAlarms");
                ALM_UpdateNodeNamespace(acXpath, acXpathUpdate, sizeof(acXpathUpdate));

                memset(&stWriteVal, 0, sizeof(stWriteVal));
                if (SR_ENUM_T == pstVal[i].type) {
                    stWriteVal.type = SR_ENUM_T;
                    stWriteVal.data.enum_val = pstVal[i].data.enum_val;
                } else if (SR_STRING_T == pstVal[i].type) {
                    stWriteVal.type = SR_STRING_T;
                    stWriteVal.data.string_val = pstVal[i].data.string_val;
                } else {
                    memcpy(&stWriteVal, pstVal, sizeof(stWriteVal));
                }

                /* raise time                                                   */
                if (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "raiseTime")) {
                    stWriteVal.data.string_val = strdup(pstAlarm->acRaiseTime);
                } else if (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "id")) {
                    /* To clear error log of sr_set_item, please ignore set the key leaf node*/
                    /* stWriteVal.data.string_val = strdup(pstAlarm->acAlarmId);*/
                    continue;
                } else if (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "extension")) {
                    stWriteVal.data.string_val = strdup(pstAlarm->acCaseExt);
                } else if (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "additional-detail")) {
                    stWriteVal.data.string_val = strdup(pstAlarm->acDetail);
                }

                rc = sr_set_item(pstSess, acXpathUpdate, &stWriteVal, SR_EDIT_DEFAULT);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item: %s, acXpath:%s", sr_strerror(rc), acXpathUpdate);
                    /* continue;                                                */
                }

                if ((0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "raiseTime")) ||
                    (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "id")) ||
                    (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "extension")) ||
                    (0 == strcmp(sr_xpath_node_name(pstVal[i].xpath), "additional-detail"))) {
                    DEF_SAFE_FREE(stWriteVal.data.string_val);
                }
            }
        }
    } else {
        /* clear                                                                */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), ALARM_XPATH_ACTIVE, pstAlarm->acAlarmId);
        rc = sr_delete_item(pstSess, acXpath, SR_EDIT_DEFAULT);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item: %s", sr_strerror(rc));
            goto cleanup;
        }
    }

    rc = sr_apply_changes(pstSess, 0);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_apply_changes: %s", sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
    }
    return rc;
}

int
DSC_Event_Subscribe(const char *xpath, DSC_event_notif_cb callback)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;

    /* subscribe for the notification                                           */
    rc = sr_notif_subscribe(sess, MODULE_OOA, xpath, 0, 0, event_notif_cb, NULL, SR_SUBSCR_DEFAULT, &event_sub);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_event_notif_subscribe: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    event_callback = callback;
cleanup:
    return rc;
}

/* delete the mapping table in dsc_alarm, use the setting in yang file */
/* there be some different alarm id, but have same extern-id and entity, such as:  */
/*    EDFA1_Failure	module-failed	SA	optical-card                               */
/*    EDFA2_Failure	module-failed	SA	optical-card                               */
/* In MSFT project, need to remove the same extern-id and entity                   */
static ALARM_INFO_ST m_astRainierAlarmList[MAX_ALM];
int
DSC_GetRainierAlmList(ALARM_INFO_ST **ppstAlarm, size_t *pnCount, bool bSkipSame, bool bTca)
{
    int rc = SR_ERR_OK;
    size_t nAlmCnt = 0;
    size_t nRainerAlmCnt = 0;
    int i = 0;
    int j = 0;
    ALARM_INFO_ST *pstTmpInfo = NULL;

    if (NULL == pnCount) {
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetAlarmList(&pstTmpInfo, &nAlmCnt);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    for (i = 0; i < nAlmCnt; i++) {
        bool bSkip = (true == bTca) ^ (NULL != strstr(pstTmpInfo[i].acAlarmId, "TCA"));
        if (true == bSkip) {
            continue;
        }

        for (j = 0; j < nRainerAlmCnt; j++) {
            if ((0 == strcmp(pstTmpInfo[i].acExternalId, m_astRainierAlarmList[j].acExternalId)) &&
                (0 == strcmp(pstTmpInfo[i].acEntity, m_astRainierAlarmList[j].acEntity))) {

                if (bSkipSame) {
                    /* same alarm id and entity, skip it                        */
                    break;
                }
            }
        }
        if (j >= nRainerAlmCnt) {
            /* not found same alarm id and entity, add it to list               */
            memcpy(&m_astRainierAlarmList[nRainerAlmCnt], &pstTmpInfo[i], sizeof(m_astRainierAlarmList[nRainerAlmCnt]));
            nRainerAlmCnt++;
        }
    }
    *pnCount = nRainerAlmCnt;
    *ppstAlarm = (ALARM_INFO_ST *)&m_astRainierAlarmList;
ERR_LABEL:
    return rc;
}

int
DSC_GetAlarmDefinationList(ALARM_DEFINATION_ST **ppastAlmDefList, size_t *pnCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    size_t i = 0, nAllCount = 0, nAlmTotal = 0;
    ALARM_DEFINATION_ST *pstAlmDefList = NULL, *pstAlmDef = NULL;
    sr_val_t *pstAllVal = NULL;

    if (NULL == ppastAlmDefList || NULL == pnCount) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        return rc;
    }

    rc = sr_get_items(pstSess, ALARM_XPATH_SUPPORTED_LIST_ALL, 0, SR_OPER_NO_STATE, &pstAllVal, &nAllCount);
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_SUPPORTED_LIST);
            rc = SR_ERR_OK;
            *pnCount = 0;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        }

        goto cleanup;
    }

    nAlmTotal = 0;

    for (i = 0; i < nAllCount; i++) {
        if (SR_LIST_T == pstAllVal[i].type) {
            if ((NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_CHILDREN_ID_NODE)) || (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_PARENTS_ID_NODE))) {
                /* ignore */
                continue;
            }
            nAlmTotal++;
            pstAlmDefList = realloc(pstAlmDefList, nAlmTotal * sizeof(ALARM_DEFINATION_ST));
            if (NULL == pstAlmDefList) {
                rc = ERR_NOMEM;
                LOG_WRITE(EN_LOG_ERROR, "Error by realloc");
                goto cleanup;
            }

            pstAlmDef = pstAlmDefList + (nAlmTotal - 1);
            memset(pstAlmDef, 0, sizeof(*pstAlmDef));
            continue;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "i:%ld, type: %d, xpath: %s", i, pstAllVal[i].type, pstAllVal[i].xpath);*/
        if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_ID)) {
            snprintf(pstAlmDef->acAlarmId, sizeof(pstAlmDef->acAlarmId), "%s", pstAllVal[i].data.string_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_RESOURCE)) {
            snprintf(pstAlmDef->acResourceType, sizeof(pstAlmDef->acResourceType), "%s", pstAllVal[i].data.enum_val);

        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_CIRCUIT_PACK)) {
            snprintf(pstAlmDef->acCircuitPack, sizeof(pstAlmDef->acCircuitPack), "%s", pstAllVal[i].data.string_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, "/resource/device")) {
            /* TODO                                                             */
        } else if (NULL != strstr(pstAllVal[i].xpath, "/probableCause/cause")) {
            /* TODO, pstAlmInfo->enCause                                        */
            snprintf(pstAlmDef->acCause, sizeof(pstAlmDef->acCause), "%s", pstAllVal[i].data.enum_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_CAUSE_EXT)) {
            snprintf(pstAlmDef->acCauseExt, sizeof(pstAlmDef->acCauseExt), "%s", pstAllVal[i].data.string_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_TYPE)) {
            snprintf(pstAlmDef->acAlarmType, sizeof(pstAlmDef->acAlarmType), "%s", pstAllVal[i].data.enum_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_RAISETIME)) {
            snprintf(pstAlmDef->acRaiseTime, sizeof(pstAlmDef->acRaiseTime), "%s", pstAllVal[i].data.string_val);
        } else if (NULL != strstr(pstAllVal[i].xpath, ALARM_XPATH_NOTIF_SERVERITY)) {
            snprintf(pstAlmDef->acSeverity, sizeof(pstAlmDef->acSeverity), "%s", pstAllVal[i].data.enum_val);
        } else if (strstr(pstAllVal[i].xpath, ALARM_XPATH_EXT_ID) != NULL) {
            strcpy(pstAlmDef->acExternalId, pstAllVal[i].data.string_val);
        } else if (strstr(pstAllVal[i].xpath, ALARM_XPATH_EXT_ENTITY) != NULL) {
            strcpy(pstAlmDef->acEntity, pstAllVal[i].data.string_val);
        } else {
            continue;
        }
    }

    *pnCount = nAlmTotal;
    if (nAlmTotal > 0) {
        *ppastAlmDefList = pstAlmDefList;
    }

    LOG_WRITE(EN_LOG_NOTICE, "alarm total: %ld", nAlmTotal);

cleanup:
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstAlmDefList);
    }

    if (pstAllVal != NULL) {
        sr_free_values(pstAllVal, nAllCount);
    }

    return rc;
}

int
DSC_RaiseAllAlarm(bool bRaise)
{
    int i = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    sr_val_t *pstVal = NULL;
    size_t nAlmTotal = 0;
    ALARM_DEFINATION_ST *pastAlmDefList = NULL, *pstAlmDef = NULL;
    size_t nValCnt = 0, nAttrNumTotal = 0;
    char acTimeStr[32] = {0};
    time_t tNow;
    sr_val_t *pstValStart = NULL;
    size_t nValCntStart = 0;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++, bRaise: %d", __func__, bRaise);

    rc = DSC_GetAlarmDefinationList(&pastAlmDefList, &nAlmTotal);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (0 == nAlmTotal) {
        LOG_WRITE(EN_LOG_NOTICE, "Not find alarm-defination");
        goto cleanup;
    }

    nAttrNumTotal = nAlmTotal * 13;
    rc = sr_new_values(nAttrNumTotal, &pstVal);
    if (rc != SR_ERR_OK || NULL == pstVal) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error: %s", sr_strerror(rc));
        goto cleanup;
    }

    nValCnt = 0;
    for (i = 0; i < nAlmTotal; i++) {
        pstAlmDef = pastAlmDefList + i;

        pstValStart = pstVal + nValCnt;
        nValCntStart = nValCnt;

        set_string_val_by_xpath(pstVal + (nValCnt++), ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_ID, pstAlmDef->acAlarmId);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_RESOURCE);
        sr_val_set_str_data(&pstVal[nValCnt++], SR_ENUM_T, pstAlmDef->acResourceType);

        set_string_val_by_xpath(pstVal + (nValCnt++), ALARM_XPATH_NOTIF "/resource/resource" ALARM_XPATH_NOTIF_CIRCUIT_PACK, pstAlmDef->acCircuitPack);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF "/probableCause/cause");
        sr_val_set_str_data(&pstVal[nValCnt++], SR_ENUM_T, pstAlmDef->acCause);

        set_string_val_by_xpath(pstVal + (nValCnt++), ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_CAUSE_EXT, pstAlmDef->acCauseExt);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_TYPE);
        sr_val_set_str_data(&pstVal[nValCnt++], SR_ENUM_T, pstAlmDef->acAlarmType);

        tNow = time(NULL);
        memset(acTimeStr, 0, sizeof(acTimeStr));
        UTIL_TimeToStr(tNow, acTimeStr);
        set_string_val_by_xpath(pstVal + (nValCnt++), ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_RAISETIME, acTimeStr);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_NOTIF_SERVERITY);
        sr_val_set_str_data(&pstVal[nValCnt++], SR_ENUM_T, bRaise ? pstAlmDef->acSeverity : "clear");

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_EXT_ID_OOG);
        sr_val_build_str_data(&pstVal[nValCnt], SR_STRING_T, "%s", pstAlmDef->acExternalId);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_EXT_ENTITY_OOG);
        sr_val_build_str_data(&pstVal[nValCnt], SR_STRING_T, "%s", pstAlmDef->acEntity);

        sr_val_set_xpath(&pstVal[nValCnt], ALARM_XPATH_NOTIF ALARM_XPATH_ADD_DETAIL);
        sr_val_build_str_data(&pstVal[nValCnt], SR_STRING_T, "%s", (true == bRaise ? "simulate" : ""));

        /* only support to send single alarm                                    */
        rc = sr_notif_send(pstSess, ALARM_XPATH_NOTIF, pstValStart, nValCnt - nValCntStart, 0, 0);
        if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
            LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s, i: %d, alarmId: %s", sr_strerror(rc), i, pstAlmDef->acAlarmId);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "event_notif_send: %s, i: %d, alarmId: %s", sr_strerror(rc), i, pstAlmDef->acAlarmId);
        }

    }

    LOG_WRITE(EN_LOG_NOTICE, "%s --, bRaise: %d, nAlmTotal: %ld", __func__, bRaise, nAlmTotal);

cleanup:

    DEF_SAFE_FREE(pastAlmDefList);

    if (pstVal != NULL) {
        sr_free_values(pstVal, nAttrNumTotal);
    }
    return rc;
}

static int
__SetActiveAlm(char *pcAlmId, bool bSetValue)
{
    sr_session_ctx_t *pstRunning = DSC_GetRunningSession();
    sr_session_ctx_t *pstStartup = NULL;
    ALM_UNIT_ST stAlm = {"", "", "", 0, false, false};
    int rc = SR_ERR_OK;

    /* Init the table                                                           */
    strncpy(stAlm.acAlmStr, pcAlmId, sizeof(stAlm.acAlmStr) - 1);

    rc = sr_session_start(DSC_GetConn(), SR_DS_STARTUP, &pstStartup);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_session_start startup error: %s", sr_strerror(rc));
        return rc;
    }

    ALM_Init(pstStartup, pstRunning, &stAlm, 1);

    stAlm.bLastAlm = (bSetValue ? false : true);
    stAlm.bCurAlm = bSetValue;

    /* clear the alarms                                                         */
    ALM_SetByUser(&stAlm, 1, true);

    if (NULL != pstStartup) {
        sr_session_stop(pstStartup);
        pstStartup = NULL;
    }

    ALM_Free();

    return rc;
}

static int
__SetMaskValue(char *pcAlmId, bool bMask)
{
    sr_session_ctx_t *pstRunning = DSC_GetRunningSession();
    char acXpath[DEF_BUF_SIZE_1024] = {0};
    int rc = SR_ERR_OK;

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), ALARM_XPATH_MASK, pcAlmId);
    rc = set_bool_item(pstRunning, acXpath, bMask);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        return rc;
    }

    rc = DSC_Commit(pstRunning);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_MaskAlarm(char *pcAlmId, bool bMask)
{
    sr_session_ctx_t *pstRunning = DSC_GetRunningSession();
    char acXpath[DEF_BUF_SIZE_1024] = {0};
    bool bIsMask = false;
    int rc = SR_ERR_OK;
    char acSeverity[DEF_BUF_SIZE_64] = {0};

    if (NULL == pcAlmId) {
        return true;
    }

    /* Check the alarm exist or NOT                                             */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), ALARM_XPATH_MASK, pcAlmId);
    rc = get_bool_item(pstRunning, acXpath, &bIsMask);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    if (bIsMask == bMask) {
        LOG_WRITE(EN_LOG_NOTICE, "alarm '%s' is already mask: %d", pcAlmId, bMask);
        return rc;
    }

    /* If alarm on, clear the alarm first                                       */
    if (true == bMask) {
        /* Check the alarm exist or NOT, if NOT exist just set mask flag        */
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), "/com-oplink-alarm:active-alarm-list/activeAlarms[id='%s']/severity", pcAlmId);
        rc = get_enum_item_log_trace(pstRunning, acXpath, acSeverity, sizeof(acSeverity) - 1);
        if ((SR_ERR_OK == rc) &&
            (0 != strcmp(acSeverity, ALARM_CLEAR_STR))) {
            __SetActiveAlm(pcAlmId, false);
        } else {
            LOG_WRITE(EN_LOG_NOTICE, "alarm '%s' isn't exist, just mask", pcAlmId, bMask);
            rc = SR_ERR_OK;
        }

        /* set alarm mask                                                       */
        __SetMaskValue(pcAlmId, bMask);

    } else {
        /* set alarm mask                                                       */
        __SetMaskValue(pcAlmId, false);

        memset(acXpath, 0, sizeof(acXpath));
        memset(acSeverity, 0, sizeof(acSeverity));
        snprintf(acXpath, sizeof(acXpath), ALARM_XPATH_BACKUP "/severity", pcAlmId);
        rc = get_enum_item_log_trace(pstRunning, acXpath, acSeverity, sizeof(acSeverity) - 1);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_TRACE, "Error by get_enum_item(%s):%s", acXpath, sr_strerror(rc));
            rc = SR_ERR_OK;
            return rc;
        }

        if (0 != strcmp(ALARM_CLEAR_STR, acSeverity)) {
            __SetActiveAlm(pcAlmId, true);
        }

    }


    return rc;
}

int
DSC_RaiseAlarm(char *pcExternId, char *pcEntity, bool bFlag)
{
    int i = 0;
    int rc = SR_ERR_OK;
    ALARM_INFO_ST stAlarm;
    size_t nCount = 0;
    ALARM_INFO_ST astSupportedAlm[512];

    if (NULL == pcExternId || NULL == pcEntity) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(astSupportedAlm, 0x00, sizeof(astSupportedAlm));
    rc = DSC_GetSupportAlarmList(astSupportedAlm, &nCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    for (i = 0; i < nCount; i++) {
        if (0 == strcmp(pcExternId, astSupportedAlm[i].acExternalId) &&
            0 == strcmp(pcEntity, astSupportedAlm[i].acEntity)) {
            strncpy(stAlarm.acAlarmId, astSupportedAlm[i].acAlarmId, sizeof(stAlarm.acAlarmId) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "Simulator alarm %s: %d", stAlarm.acAlarmId, bFlag);
            break;
        }
    }

    if (i >= nCount) {
        LOG_WRITE(EN_LOG_ERROR, "NOT found: %s, %s", pcExternId, pcEntity);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    return __SetActiveAlm(stAlarm.acAlarmId, bFlag);

cleanup:
    return rc;
}