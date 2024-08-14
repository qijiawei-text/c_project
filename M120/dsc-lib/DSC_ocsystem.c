#include "DSC_common.h"
#include "DSC_ocsystem.h"
#include "DSC_alarm.h"
#include "DSC_system.h"

/* openconfig-system */
int
DSC_GetOcSystemConfig(OCSYSTEM_CONFIG_ST *pstConfig)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    /* printf("%s-%d,OCS_XPATH_CONFIG_ALL=%s\n",__func__,__LINE__,OCS_XPATH_CONFIG_ALL); */
    /* printf("%s-%d,buff_size=%d\n",__func__,__LINE__,(int)sizeof((char *)&pstConfig->acHostName)); */
    /* printf("%s-%d,buff_size=%d\n",__func__,__LINE__,(int)sizeof((char *)&pstConfig->acDomainName)); */
    /* printf("%s-%d,buff_size=%d\n",__func__,__LINE__,(int)sizeof(pstConfig->acLoginBanner)); */
    /* printf("%s-%d,buff_size=%d\n",__func__,__LINE__,(int)sizeof(pstConfig->acMotdBanner)); */

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_CFG_HOSTNAME, SR_STRING_T, pstConfig->acHostName),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_CFG_DOMAIN_NAME, SR_STRING_T, pstConfig->acDomainName),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_CFG_LOGIN_BANNER, SR_STRING_T, pstConfig->acLoginBanner),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_CFG_MOTD_BANNER, SR_STRING_T, pstConfig->acMotdBanner),
    };

    pstSess = DSC_GetOperSession();
    /* printf("%s-%d,pstSess = %p\n",__func__,__LINE__,pstSess); */

    if (NULL == pstSess || NULL == pstConfig) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstConfig, 0, sizeof(OCSYSTEM_CONFIG_ST));
    /* printf("%s-%d,hostname=%s\n",__func__,__LINE__,pstConfig->acHostName); */
    /* printf("%s-%d,acDomainName=%s\n",__func__,__LINE__,pstConfig->acDomainName); */
    /* printf("%s-%d,acLoginBanner=%s\n",__func__,__LINE__,pstConfig->acLoginBanner); */
    /* printf("%s-%d,acMotdBanner=%s\n",__func__,__LINE__,pstConfig->acMotdBanner); */
    rc = DSC_GetItems(pstSess, OCS_XPATH_CONFIG_ALL, astMaps, sizeof(astMaps));
    /* printf("%s-%d,hostname=%s\n", __func__, __LINE__, pstConfig->acHostName); */
    /* printf("%s-%d,acDomainName=%s\n", __func__, __LINE__, pstConfig->acDomainName); */
    /* printf("%s-%d,acLoginBanner=%s\n", __func__, __LINE__, pstConfig->acLoginBanner); */
    /* printf("%s-%d,acMotdBanner=%s\n", __func__, __LINE__, pstConfig->acMotdBanner); */

    return rc;
}

int
DSC_GetOcSystemState(OCSYSTEM_STATE_ST *pstState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_HOSTNAME, SR_STRING_T, pstState->acHostName),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_DOMAIN_NAME, SR_STRING_T, pstState->acDomainName),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_LOGIN_BANNER, SR_STRING_T, pstState->acLoginBanner),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_MOTD_BANNER, SR_STRING_T, pstState->acMotdBanner),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_CURRENT_DATETIME, SR_STRING_T, pstState->acCurrentDatetime),
        ANONYMOUS_MAP_ENTRY(OCS_XPATH_STATE_BOOT_TIME, SR_UINT64_T, (char *)&pstState->ullBootTime),
    };

    pstSess = DSC_GetOperSession();

    if (NULL == pstSess || NULL == pstState) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstState, 0, sizeof(OCSYSTEM_STATE_ST));
    rc = DSC_GetItems(pstSess, OCS_XPATH_STATE_ALL, astMaps, sizeof(astMaps));
    /* printf("%s-%d,hostname=%s\n",__func__,__LINE__,pstState->acHostName); */
    /* printf("%s-%d,acDomainName=%s\n",__func__,__LINE__,pstState->acDomainName); */
    /* printf("%s-%d,acLoginBanner=%s\n",__func__,__LINE__,pstState->acLoginBanner); */
    /* printf("%s-%d,acMotdBanner=%s\n",__func__,__LINE__,pstState->acMotdBanner); */
    /* printf("%s-%d,acCurrentDatetime=%s\n",__func__,__LINE__,pstState->acCurrentDatetime); */
    /* printf("%s-%d,ullBootTime=%ld\n",__func__,__LINE__,pstState->ullBootTime); */
    /* printf("%s-%d,ullBootTime=%lld\n",__func__,__LINE__,pstState->ullBootTime); */
    return rc;
}

int
DSC_GetOcTimezoneNameConfig(char *pcTimezone, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess || NULL == pcTimezone) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        goto ERR_LABEL;
    }

    rc = get_string_item(sess, OCS_XPATH_CLOCK_CFG_TIMEZONE_NAME, pcTimezone, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", OCS_XPATH_CLOCK_CFG_TIMEZONE_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetOcTimezoneNameState(char *pcTimezone, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pcTimezone) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        goto ERR_LABEL;
    }

    rc = get_string_item(sess, OCS_XPATH_CLOCK_STATE_TIMEZONE_NAME, pcTimezone, size);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", OCS_XPATH_CLOCK_STATE_TIMEZONE_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetOcTimezoneName(char *pcTimezoneName)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, OCS_XPATH_CLOCK_CFG_TIMEZONE_NAME, pcTimezoneName, OCS_TIMEZONENAME_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", OCS_XPATH_CLOCK_CFG_TIMEZONE_NAME, sr_strerror(rc));
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
DSC_SetOcSystemHostname(char *pcHostName)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = set_string_item(sess, OCS_XPATH_CFG_HOSTNAME, pcHostName, OCS_HOSTNAME_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetStringItem config hostname: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetOcSystemHostname DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_OcRpcCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt)
{
    int rc = SR_ERR_OK;

    if (NULL == xpath ||
        NULL == status ||
        NULL == message ||
        NULL == output ||
        NULL == output_cnt) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto cleanup;
    }

    rc = sr_new_values(1, output);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_new_values: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = sr_val_set_xpath(&(*output)[0], OC_RPC_XPATH_RESTART_MESSAGE_RES);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", OC_RPC_XPATH_RESTART_MESSAGE_RES, sr_strerror(rc));
        goto cleanup;
    }


    rc = sr_val_set_str_data(&(*output)[0], SR_STRING_T, message);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* inform sysrepo about the number of output values */
    *output_cnt = 1;

cleanup:
    return rc;
}

int
DSC_OcGetAlarmList_I(ALARM_INFO_ST **ppstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;

    /* get all list instances with their content (recursive) */
    rc = get_items(sess, "/com-oplink-alarm:active-alarm-list//*", &val, &cnt); /*  ALARM_XPATH_LIST_I */
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_LIST_I);
        /* printf("%s:%d,Not found item: %s, xpath: %s\n", __func__,__LINE__,sr_strerror(rc), ALARM_XPATH_LIST_I); */
        if (val != NULL)
            sr_free_values(val, cnt);

        *count = 0;
        return SR_ERR_OK;
    }
#if 0
    for (size_t i = 0; i < cnt; i++) {
        sr_print_val(val + i);
    }
#endif
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        /* printf("%s:%d,Error by get_items: %s\n", __func__,__LINE__,sr_strerror(rc)); */

        *count = 0;
        return rc;
    }
    /* printf("%s:%d,will build alarmlist struct\n", __func__,__LINE__); */
    rc = DSC_BuildAlarmList(val, cnt, ppstAlarm, count);
    sr_free_values(val, cnt);
    return rc;
}

static OCSYSTEM_ALARM_INFO_ST m_astAlm[MAX_ALM] = {{0, {0}, {0}, {0}, 0, {0}, {0}}};
int
DSC_OcBuildAlarmList(const sr_val_t *val, const size_t cnt, OCSYSTEM_ALARM_INFO_ST **ppstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    OCSYSTEM_ALARM_INFO_ST *pstAlm = NULL;
    int i = 0, j = 0;

    memset(m_astAlm, 0x00, sizeof(m_astAlm));
    pstAlm = (OCSYSTEM_ALARM_INFO_ST *)&m_astAlm;
    j = -1;
    for (i = 0; i < cnt; i++) {
        /* TODO: fill in the alarm structure */
        if (strstr(val[i].xpath, ALARM_LEAF_ID) != NULL) {
            j++;
            if (j >= MAX_ALM) {
                break;
            }
            strncpy(pstAlm[j].acAlarmId, val[i].data.string_val, sizeof(pstAlm[j].acAlarmId) - 1);
        } else if (strstr(val[i].xpath, ALARM_LEAF_RESOURCE) != NULL) {
            strncpy(pstAlm[j].acResource, val[i].data.string_val, sizeof(pstAlm[j].acResource) - 1);
        } else if (strstr(val[i].xpath, ALARM_LEAF_TEXT) != NULL) {
            strncpy(pstAlm[j].acText, val[i].data.string_val, sizeof(pstAlm[j].acText) - 1);
        } else if (strstr(val[i].xpath, ALARM_LEAF_TIMECREATED) != NULL) {
            pstAlm[j].ullTimeCreated = val[i].data.uint64_val;
        } else if (strstr(val[i].xpath, ALARM_LEAF_SEVERITY) != NULL) {
            strncpy(pstAlm[j].acSeverity, val[i].data.string_val, sizeof(pstAlm[j].acSeverity) - 1);
        } else if (strstr(val[i].xpath, ALARM_LEAF_TYPEID) != NULL) {
            strncpy(pstAlm[j].acTypeId, val[i].data.string_val, sizeof(pstAlm[j].acTypeId) - 1);
        }
        /* sr_print_val(val + i); */
    }

    *ppstAlarm = pstAlm;
    *count = j + 1;

    /* printf("%s:%d,alarmcount= %d\n", __func__,__LINE__,(int)*count); */
    return rc;
}

int
DSC_OcGetAlarmList(OCSYSTEM_ALARM_INFO_ST **ppstAlarm, size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetOperSession();
    sr_val_t *val = NULL;
    size_t cnt = 0;

    /* get all list instances with their content (recursive) */
    rc = get_items(sess, "/openconfig-system:system/alarms//*", &val, &cnt); /*  ALARM_XPATH_LIST_I */
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), ALARM_XPATH_LIST_I);
        /* printf("%s:%d,Not found item: %s, xpath: %s\n", __func__,__LINE__,sr_strerror(rc), ALARM_XPATH_LIST_I); */
        if (val != NULL)
            sr_free_values(val, cnt);

        *count = 0;
        return SR_ERR_OK;
    }

    /* printf("%s:%d,cnt: %d\n", __func__,__LINE__,(int)cnt); */
#if 0
    for (size_t i = 0; i < cnt; i++) {
        sr_print_val(val + i);
    }
#endif
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        /* printf("%s:%d,Error by get_items: %s\n", __func__,__LINE__,sr_strerror(rc)); */

        *count = 0;
        return rc;
    }
    /* printf("%s:%d,will build alarmlist struct\n", __func__,__LINE__); */
    rc = DSC_OcBuildAlarmList(val, cnt, ppstAlarm, count);
    sr_free_values(val, cnt);
    return rc;
}

/* 2016-12-10T11:12:22Z */
time_t
__CovertToOcTime(char *pcTime)
{
    struct tm tm1;
    time_t time1;
    /* printf("%s:%d pcTime=%s\n", __func__, __LINE__, pcTime); */
    sscanf(pcTime, "%4d-%2d-%2dT%2d:%2d:%2dZ",
           &tm1.tm_year,
           &tm1.tm_mon,
           &tm1.tm_mday,
           &tm1.tm_hour,
           &tm1.tm_min,
           &tm1.tm_sec);
    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    /*printf("%s:%d tm1.tm_year=%d\n", __func__, __LINE__, tm1.tm_year);
       printf("%s:%d tm1.tm_mon=%d\n", __func__, __LINE__, tm1.tm_mon);
       printf("%s:%d tm1.tm_mday=%d\n", __func__, __LINE__, tm1.tm_mday);
       printf("%s:%d tm1.tm_hour=%d\n", __func__, __LINE__, tm1.tm_hour);
       printf("%s:%d tm1.tm_min=%d\n", __func__, __LINE__, tm1.tm_min);
       printf("%s:%d tm1.tm_sec=%d\n", __func__, __LINE__, tm1.tm_sec);*/
    tm1.tm_isdst = -1;
    time1 = mktime(&tm1);
    /* printf("%s:%d time1=%d\n", __func__, __LINE__, (int)time1); */
    return time1;
}

/* 2021-01-22 05:15:21.837852 */
static uint64_t
__CovertToOcTimeMicrosecond(char *pcTime)
{
    struct tm tm1;
    time_t time1;
    uint32_t uiMicrosecond = 0;
    uint64_t uiTimeNanosecond = 0;
    /* printf("%s:%d pcTime=%s\n", __func__, __LINE__, pcTime); */
    sscanf(pcTime, "%4d-%2d-%2dT%2d:%2d:%2d.%6dZ",
           &tm1.tm_year,
           &tm1.tm_mon,
           &tm1.tm_mday,
           &tm1.tm_hour,
           &tm1.tm_min,
           &tm1.tm_sec,
           &uiMicrosecond);
    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    /*printf("%s:%d tm1.tm_year=%d\n", __func__, __LINE__, tm1.tm_year);
       printf("%s:%d tm1.tm_mon=%d\n", __func__, __LINE__, tm1.tm_mon);
       printf("%s:%d tm1.tm_mday=%d\n", __func__, __LINE__, tm1.tm_mday);
       printf("%s:%d tm1.tm_hour=%d\n", __func__, __LINE__, tm1.tm_hour);
       printf("%s:%d tm1.tm_min=%d\n", __func__, __LINE__, tm1.tm_min);
       printf("%s:%d tm1.tm_sec=%d\n", __func__, __LINE__, tm1.tm_sec);*/
    tm1.tm_isdst = -1;
    time1 = mktime(&tm1);

    uiTimeNanosecond = time1 * NANOSECOND + uiMicrosecond * 1000;
    /* printf("%s:%d time1=%d\n", __func__, __LINE__, (int)time1); */
    return uiTimeNanosecond;
}

static int
__AlarmSeverityToOcSystem(char *pcSrc, char *pcDst)
{
    int rc = SR_ERR_OK;
    if (NULL == pcSrc || NULL == pcDst) {
        goto cleanup;
    }
    if (0 == strncmp(pcSrc, ALARM_CRITICAL_STR, strlen(ALARM_CRITICAL_STR))) {
        strcpy(pcDst, OC_ALM_SEVERITY_CRITICAL);
        /* sprintf(pcDst, "openconfig-alarm-types:%s", OC_ALM_SEVERITY_CRITICAL); */
    } else if (0 == strncmp(pcSrc, ALARM_MAJOR_STR, strlen(ALARM_MAJOR_STR))) {
        strcpy(pcDst, OC_ALM_SEVERITY_MAJOR);
        /* sprintf(pcDst, "openconfig-alarm-types:%s", OC_ALM_SEVERITY_MAJOR); */
    } else if (0 == strncmp(pcSrc, ALARM_MINOR_STR, strlen(ALARM_MINOR_STR))) {
        strcpy(pcDst, OC_ALM_SEVERITY_MINOR);
        /* sprintf(pcDst, "openconfig-alarm-types:%s", OC_ALM_SEVERITY_MINOR); */
    } else if (0 == strncmp(pcSrc, ALARM_WARNING_STR, strlen(ALARM_WARNING_STR))) {
        strcpy(pcDst, OC_ALM_SEVERITY_WARNING);
        /* sprintf(pcDst, "openconfig-alarm-types:%s", OC_ALM_SEVERITY_WARNING); */
    } else {
        strcpy(pcDst, OC_ALM_SEVERITY_UNKNOWN);
        /* sprintf(pcDst, "openconfig-alarm-types:%s", OC_ALM_SEVERITY_UNKNOWN); */
    }
cleanup:
    return rc;
}

static void
__TcaParse(char *pcTca, char *pcSource, char *pcType)
{
    int iCnt = 0;
    char accTacType[4][32] = {"15MIN_AVG_TOOHIGH", "24HOUR_AVG_TOOHIGH", "15MIN_AVG_TOOLOW", "24HOUR_AVG_TOOLOW"};

    if (pcTca == NULL || pcSource == NULL || pcType == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (strstr(pcTca, accTacType[i])) {
            iCnt = strlen(accTacType[i]);
            snprintf(pcSource, strlen(pcTca) - iCnt, "%s", pcTca);
            strcpy(pcType, accTacType[i]);
            break;
        }
    }
}

int
DSC_ConvertToOcAlarm(ALARM_INFO_ST *pstAlarm, OCSYSTEM_ALARM_INFO_ST *pstOcAlarm)
{
    int rc = SR_ERR_OK;
    char *pcStr = NULL;
    char *pcBuf = NULL;
    char acBuf[128];

    if (NULL == pstAlarm || NULL == pstOcAlarm) {
        return SR_ERR_INVAL_ARG;
    }

    memset(pstOcAlarm, 0, sizeof(OCSYSTEM_ALARM_INFO_ST));

    strncpy(pstOcAlarm->acAlarmId, pstAlarm->acAlarmId, sizeof(pstOcAlarm->acAlarmId) - 1);
    memcpy(pstOcAlarm->acRaiseTime, pstAlarm->acRaiseTime, sizeof(pstOcAlarm->acRaiseTime) - 1);
    pstOcAlarm->acRaiseTime[sizeof(pstOcAlarm->acRaiseTime) - 1] = '\0';
    pstOcAlarm->ullTimeCreated = (uint64_t)__CovertToOcTimeMicrosecond(pstAlarm->acRaiseTime);
    /* strncpy(pstOcAlarm->acSeverity, pstAlarm->acSeverity, sizeof(pstOcAlarm->acSeverity) - 1); */
    __AlarmSeverityToOcSystem(pstAlarm->acSeverity, pstOcAlarm->acSeverity);
    /* strncpy(pstOcAlarm->acText, pstAlarm->acCaseExt, sizeof(pstOcAlarm->acText) - 1); */
    if (strlen(pstAlarm->acDetail) > 0) {
        snprintf(pstOcAlarm->acText, sizeof(pstOcAlarm->acText), "%s:%s", pstAlarm->acCaseExt, pstAlarm->acDetail);
    } else {
        strncpy(pstOcAlarm->acText, pstAlarm->acCaseExt, sizeof(pstOcAlarm->acText));
    }

    memset(acBuf, 0, sizeof(acBuf));
    strncpy(acBuf, pstAlarm->acAlarmId, sizeof(acBuf) - 1);
    pcBuf = acBuf;

    pcStr = strsep(&pcBuf, "_");

    if (pcStr) {
        if (NULL != pcBuf) {
            if (strstr("TCA", pcStr)) {
                __TcaParse(pstAlarm->acAlarmId, pstOcAlarm->acResource, pstOcAlarm->acTypeId);
            } else {
                strncpy(pstOcAlarm->acResource, pcStr, sizeof(pstOcAlarm->acResource) - 1);
                if (NULL != pcBuf) {
                    strcpy(pstOcAlarm->acTypeId, pcBuf);
                    /* strncpy(pstOcAlarm->acTypeId, acBuf, sizeof(pstOcAlarm->acTypeId) - 1); */
                }
            }
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Alarm Id error: %s", pstAlarm->acAlarmId);
        return SR_ERR_NOT_FOUND;
    }

    return rc;
}

/* for WSS port and OPS port Threshold Setting */

int
DSC_GetWssPortThrNumAndType(char *pcId, uint32_t *piPortNum, uint32_t *piThrType)
{
    int rc = SR_ERR_OK;
    uint32_t iPortNum = 0;

    if (NULL == pcId) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    if (NULL != strstr(pcId, PORT_THR_IN)) {
        if (NULL != strstr(pcId, PORT_THR_TYPE_LOS)) {
            sscanf(pcId, "In%d_LOS", &iPortNum);  /*In%dLos In01Los to In01_LOS*/
            *piThrType = TYPE_LOS;
        } else if (NULL != strstr(pcId, PORT_THR_TYPE_LOW)) {
            sscanf(pcId, "In%d_RX_POWER_LOW", &iPortNum);   /*In%dLow In01Low to In01_RX_POWER_LOW*/
            *piThrType = TYPE_LOW;
        } else if (NULL != strstr(pcId, PORT_THR_TYPE_HIGH)) {
            sscanf(pcId, "In%d_RX_POWER_HIGH", &iPortNum);  /*In%dHigh In01High to In01_RX_POWER_HIGH*/
            *piThrType = TYPE_HIGH;
        }
        if ( iPortNum == OPS_PORT_01_NUM) {
            *piPortNum = OPS_In65;
        } else if ( iPortNum == OPS_PORT_02_NUM) {
            *piPortNum = OPS_In66;
        } else {
            *piPortNum = iPortNum;
        }
    } else if (NULL != strstr(pcId, PORT_THR_OUT)) {
        if (NULL != strstr(pcId, PORT_THR_TYPE_LOW)) {
            sscanf(pcId, "Out%d_TX_POWER_LOW", &iPortNum);  /* Out%dLow */
            *piThrType = TYPE_LOW;
        } else if (NULL != strstr(pcId, PORT_THR_TYPE_HIGH)) {
            sscanf(pcId, "Out%d_TX_POWER_High", &iPortNum);   /* Out%dHigh */
            *piThrType = TYPE_HIGH;
        }
        if ( iPortNum == OPS_PORT_01_NUM) {
            *piPortNum = OPS_Out65;
        } else if ( iPortNum == OPS_PORT_02_NUM) {
            *piPortNum = OPS_Out66;
        }
    }

    /* printf("%s:%d iPortNum=%d,iThrType=%d\n", __func__, __LINE__, *piPortNum, *piThrType); */

ERR_LABEL:

    return rc;
}


int
DSC_GetWssPortThrConfig(EN_TCA_IDX enIdx, WSS_PORT_THR_CONFIG_ST *pstConfig)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t count = 0;

    rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
        return rc;
    }

    rc = DSC_GetTcaThresh(enIdx,
                          pstTcaInfo,
                          &(pstConfig->dInHighThr),
                          &(pstConfig->dInLowThr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca threshold error");
    }
    /* LOG_WRITE(EN_LOG_NOTICE, "enIdx:%d,dInHighThr=%f,dInLowThr=%f", enIdx, pstConfig->dInHighThr, pstConfig->dInLowThr); */

    return rc;
}


int
DSC_SetWssPortThr(char *pcId, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    int iIdx = 0;

    rc = DSC_GetTcaIdxFromName(pcId, &iIdx);
    if ( SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, iIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set wss or ops port threshold: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetTcaPortThreshold(char *pcTcaName, double dThr)
{
    int rc = SR_ERR_OK;
    /* char *pcTcaName = NULL; */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = NULL;
    /* EN_TCA_IDX enIdx; */
    uint32_t iPortNum = 0;
    uint32_t iThrType = 0;

    if (NULL == pcTcaName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTcaThreshold Error not found id");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }


    rc = DSC_GetWssPortThrNumAndType(pcTcaName, &iPortNum, &iThrType);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetWssPortThrNumAndType: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }


    if ( TYPE_LOS == iThrType || TYPE_LOW == iThrType) {

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_LOW, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, dThr);

        rc = set_decimal64_item(sess, acXpath, dThr);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if ( TYPE_HIGH == iThrType) {
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_HIGH, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, dThr);

        rc = set_decimal64_item(sess, acXpath, dThr);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetTcaPortHysteresis(char *pcTcaName, double dHys)
{
    int rc = SR_ERR_OK;
    /* char *pcTcaName = NULL; */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = NULL;
    /* EN_TCA_IDX enIdx; */
    uint32_t iPortNum = 0;
    uint32_t iThrType = 0;
    int iIdx = 0;
    WSS_PORT_THR_CONFIG_ST stWssPortThrConfig;

    if (NULL == pcTcaName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTcaThreshold Error not found id");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_GetWssPortThrNumAndType(pcTcaName, &iPortNum, &iThrType);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetWssPortThrNumAndType: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* get current config */
    rc = DSC_GetTcaIdxFromName(pcTcaName, &iIdx);
    if ( SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetTcaIdxFromName: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(&stWssPortThrConfig, 0x00, sizeof(stWssPortThrConfig));
    rc = DSC_GetWssPortThrConfig(iIdx, &stWssPortThrConfig);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetWssPortThrConfig: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }


    /*then calculation the high or low threhold with Hysteresis
               if TYPE_LOS or TYPE_LOW,  real threshold is lowThresholdValue, then calc and set highThresholdValue = lowThresholdValue + hysteresis;
               if TYPE_HIGH,			 real threshold is HighThresholdValue,then calc and set lowThresholdValue = highThresholdValue - hysteresis;
     */

    if ( TYPE_LOS == iThrType || TYPE_LOW == iThrType) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_HIGH, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, stWssPortThrConfig.dInHighThr + dHys);

        rc = set_decimal64_item(sess, acXpath, stWssPortThrConfig.dInLowThr + dHys);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    } else if ( TYPE_HIGH == iThrType) {
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_LOW, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, stWssPortThrConfig.dInHighThr - dHys);

        rc = set_decimal64_item(sess, acXpath, stWssPortThrConfig.dInHighThr - dHys);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_SetTcaPortThresholdAndHysteresis(char *pcTcaName, double dThr, double dHys)
{
    int rc = SR_ERR_OK;
    /* char *pcTcaName = NULL; */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = NULL;
    /* EN_TCA_IDX enIdx; */
    uint32_t iPortNum = 0;
    uint32_t iThrType = 0;

    if (NULL == pcTcaName) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTcaThreshold Error not found id");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }


    rc = DSC_GetWssPortThrNumAndType(pcTcaName, &iPortNum, &iThrType);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetWssPortThrNumAndType: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if ( TYPE_LOS == iThrType || TYPE_LOW == iThrType) {

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_LOW, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, dThr);

        rc = set_decimal64_item(sess, acXpath, dThr);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_HIGH, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, dThr + dHys);

        rc = set_decimal64_item(sess, acXpath, dThr + dHys);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }


    } else if ( TYPE_HIGH == iThrType) {
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_HIGH, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, dThr);

        rc = set_decimal64_item(sess, acXpath, dThr);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_LOW, pcTcaName);
        LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, dThr - dHys);

        rc = set_decimal64_item(sess, acXpath, dThr - dHys);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }

    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_Clear_Pms(char *pcPmBinType, char *pcEntityName, char *pcPmTimePeriod, char *pcPmpType, char *pcPmParameter, char *pcValidity)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 6;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    /* set 'pm-bin-type' leaf */
    rc = sr_val_set_xpath(&input[0], "/openconfig-transport-pms:clear-pm-data/pm-bin-type");
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(pcPmBinType);   /* history or current*/

    rc = sr_val_set_xpath(&input[1], "/openconfig-transport-pms:clear-pm-data/entity-name");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/entity-name", sr_strerror(rc));
        return rc;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcEntityName);   /* all */


    /* set 'pm-time-period leaf */
    rc = sr_val_set_xpath(&input[2], "/openconfig-transport-pms:clear-pm-data/pm-time-period");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pm-time-period", sr_strerror(rc));
        return rc;
    }
    input[2].type = SR_ENUM_T;
    input[2].data.string_val = strdup(pcPmTimePeriod); /* 15min or 24h */   /* 15min */


    /* set 'pmp-type leaf */
    rc = sr_val_set_xpath(&input[3], "/openconfig-transport-pms:clear-pm-data/pmp-type");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pmp-type", sr_strerror(rc));
        return rc;
    }
    input[3].type = SR_ENUM_T;
    input[3].data.string_val = strdup(pcPmpType);   /* all */


    /* set 'pm-parameter leaf */
    rc = sr_val_set_xpath(&input[4], "/openconfig-transport-pms:clear-pm-data/pm-parameter");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pm-parameter", sr_strerror(rc));
        return rc;
    }
    input[4].type = SR_ENUM_T;
    input[4].data.string_val = strdup(pcPmParameter);   /* all */

    /* set 'validity leaf */
    rc = sr_val_set_xpath(&input[5], "/openconfig-transport-pms:clear-pm-data/validity");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/validity", sr_strerror(rc));
        return rc;
    }
    input[5].type = SR_ENUM_T;
    input[5].data.string_val = strdup(pcValidity);   /* all */
    /* execute RPC */
    rc = DSC_RpcSend(sess, "/openconfig-transport-pms:clear-pm-data", input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", "/openconfig-transport-pms:clear-pm-data", sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);
    /* rc = DSC_ProcessRPCResponse(output, output_cnt); */
    sleep(1);
    return rc;

ERR_LABEL:
    sr_free_values(input, input_cnt);

    return rc;
}
#if 0
static int
rpc_caller_getpm(sr_session_ctx_t *session, char *pm - bin - type, char *entity - name, char *pm - time - period, char *pmp - type, char *pm - parameter, char *validity)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;

    /* allocate input values */
    input_cnt = 6;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    /* set 'pm-bin-type' leaf */
    rc = sr_val_set_xpath(&input[0], "/openconfig-transport-pms:clear-pm-data/pm-bin-type");
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = "history"; /* todo */   /* current current */

    rc = sr_val_set_xpath(&input[1], "/openconfig-transport-pms:clear-pm-data/entity-name");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/entity-name", sr_strerror(rc));
        return rc;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = "FMD"; /* todo */   /* all */


    /* set 'pm-time-period leaf */
    rc = sr_val_set_xpath(&input[2], "/openconfig-transport-pms:clear-pm-data/pm-time-period");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pm-time-period", sr_strerror(rc));
        return rc;
    }
    input[2].type = SR_ENUM_T;
    input[2].data.string_val = "24h"; /* todo */   /* 15min */


    /* set 'pmp-type leaf */
    rc = sr_val_set_xpath(&input[3], "/openconfig-transport-pms:clear-pm-data/pmp-type");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pmp-type", sr_strerror(rc));
        return rc;
    }
    input[3].type = SR_ENUM_T;
    input[3].data.string_val = "all";   /* todo */


    /* set 'pm-parameter leaf */
    rc = sr_val_set_xpath(&input[4], "/openconfig-transport-pms:clear-pm-data/pm-parameter");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/pm-parameter", sr_strerror(rc));
        return rc;
    }
    input[4].type = SR_ENUM_T;
    input[4].data.string_val = "all";   /* todo */

    /* set 'validity leaf */
    rc = sr_val_set_xpath(&input[5], "/openconfig-transport-pms:clear-pm-data/validity");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/validity", sr_strerror(rc));
        return rc;
    }
    input[5].type = SR_ENUM_T;
    input[5].data.string_val = "all";   /* todo */
#if 0
    /* set 'input/start-number-of-bin */
    rc = sr_val_set_xpath(&input[6], "/openconfig-transport-pms:clear-pm-data/start-number-of-bin");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/start-number-of-bin", sr_strerror(rc));
        return rc;
    }
    input[6].type = SR_UINT16_T;
    input[6].data.uint16_val = 3;   /* todo */

    /* set 'input/end-number-of-bin leaf */
    rc = sr_val_set_xpath(&input[7], "/openconfig-transport-pms:clear-pm-data/end-number-of-bin");
    if (SR_ERR_OK != rc) {
        printf("sr_val_set_xpath: %s %s\n", "/openconfig-transport-pms:clear-pm-data/end-number-of-bin", sr_strerror(rc));
        return rc;
    }
    input[7].type = SR_UINT16_T;
    input[7].data.uint16_val = 5;   /* todo */
#endif

    printf("\n\n ========== EXECUTING RPC ==========\n\n");
    printf(">>> RPC Input:input_cnt=%d\n\n", input_cnt);
    for (size_t i = 0; i < input_cnt; ++i) {
        sr_print_val(&input[i]);
    }
    printf("\n");

    /* execute RPC */
    printf("\n\n ========== sr_rpc_send==========\n\n");
    rc = sr_rpc_send(session, "/openconfig-transport-pms:clear-pm-data", input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        printf("SR_ERR_OK != rc,rc=%d\n", rc);
        return rc;
    }

    /* print output values */
    printf("\n>>> Received an RPC response:\n\n");
    for (size_t i = 0; i < output_cnt; ++i) {
        sr_print_val(output + i);
    }
    printf("\n");

    /* don't forget to de-allocate the output values */
    sr_free_values(output, output_cnt);
    return SR_ERR_OK;
}

#endif

