/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_inc.h"

static char *
__SyslogSeverityToString(bool bIsUsedForCmd, SYSLOG_SEVERITY_TYPE_EN enSeverity, char *pcBuf)
{

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enSeverity) {
    case SYSLOG_SEVERITY_EMERGENCY_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_EMERG_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_EMERGENCY_STR);
        }
        break;
    case SYSLOG_SEVERITY_ALERT_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_ALERT_STR);
        break;
    case SYSLOG_SEVERITY_CRITICAL_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_CRIT_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_CRITICAL_STR);
        }
        break;
    case SYSLOG_SEVERITY_ERROR_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_ERR_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_ERROR_STR);
        }
        break;
    case SYSLOG_SEVERITY_WARNING_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_WARNING_STR);
        break;
    case SYSLOG_SEVERITY_NOTICE_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_NOTICE_STR);
        break;
    case SYSLOG_SEVERITY_INFO_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_INFO_STR);
        break;
    case SYSLOG_SEVERITY_DEBUG_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_DEBUG_STR);
        break;
    case SYSLOG_SEVERITY_ALL_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_ALL_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_ALL_STR);
        }
        break;
    case SYSLOG_SEVERITY_NONE_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_NONE_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

static char *
__SyslogOperatorToString(SYSLOG_OPERATOR_TYPE_EN enOperator, char *pcBuf)
{

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enOperator) {
    case EQUALS_OR_HIGHER_TYPE:
        strcpy(pcBuf, "");
        break;
    case EQUALS_TYPE:
        strcpy(pcBuf, "=");
        break;
    case NOT_EQUALS_TYPE:
        strcpy(pcBuf, "!=");
        break;
    default:
        break;
    }

    return pcBuf;
}

static int
__SyslogOperatorToValue(char ch, SYSLOG_OPERATOR_TYPE_EN *penOperator)
{
    int rc = SR_ERR_OK;

    if (NULL == penOperator) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if ('=' == ch) {
        *penOperator = EQUALS_TYPE;
    } else if ('!' == ch) {
        *penOperator = NOT_EQUALS_TYPE;
    } else {
        *penOperator = EQUALS_OR_HIGHER_TYPE;
    }

    return rc;
}

static char *
__SyslogOperatorEnumToString(SYSLOG_OPERATOR_TYPE_EN penOperator, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (penOperator) {
    case EQUALS_OR_HIGHER_TYPE:
        strcpy(pcBuf, SYSLOG_OPERATOR_EQUALS_OR_HIGHER_STR);
        break;
    case EQUALS_TYPE:
        strcpy(pcBuf, SYSLOG_OPERATOR_EQUALS_STR);
        break;
    case NOT_EQUALS_TYPE:
        strcpy(pcBuf, SYSLOG_OPERATOR_NOT_EQUALS_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

static int
__SyslogFacilityToValue(char *pcBuf, FACILITY_TYPE_EN *penFacilityType, SYSLOG_FACILITY_SUB_ST *pstFacilitySub)
{
    int rc = SR_ERR_OK;
    int i = 0;
    bool bFlag = false;

    if (NULL == penFacilityType || NULL == pcBuf || NULL == pstFacilitySub) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (pcBuf[0] == '*') {
        if (pcBuf[1] == '.') {
            if (0 == strcmp(&pcBuf[2], "none")) {
                *penFacilityType = FACILITY_NONE_TYPE;
            } else {
                *penFacilityType = FACILITY_NOT_NONE_TYPE;
                strcpy(pstFacilitySub->acFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR);
                __SyslogOperatorToValue(pcBuf[2], &pstFacilitySub->enOpType);
                if (pstFacilitySub->enOpType == EQUALS_OR_HIGHER_TYPE) {
                    rc = DSC_SyslogSeverityToValue(&pcBuf[2], &pstFacilitySub->enSeverity);
                    if (SR_ERR_OK != rc) {
                        goto ERR_LABEL;
                    }
                } else {
                    rc = DSC_SyslogSeverityToValue(&pcBuf[3], &pstFacilitySub->enSeverity);
                    if (SR_ERR_OK != rc) {
                        goto ERR_LABEL;
                    }
                }
            }
        } else {
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }
    } else {
        for (i = 0; i < SYSLOG_FACILITY_NAME_MAX_LEN; i++) {
            if (pcBuf[i] != '.') {
                pstFacilitySub->acFacilityName[i] = pcBuf[i];
            } else {
                bFlag = true;
                pstFacilitySub->acFacilityName[i] = '\0';
                break;
            }
        }

        if (!bFlag) {
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        *penFacilityType = FACILITY_NOT_NONE_TYPE;
        __SyslogOperatorToValue(pcBuf[i + 1], &pstFacilitySub->enOpType);
        if (pstFacilitySub->enOpType == EQUALS_OR_HIGHER_TYPE) {
            rc = DSC_SyslogSeverityToValue(&pcBuf[i + 1], &pstFacilitySub->enSeverity);
            if (SR_ERR_OK != rc) {
                goto ERR_LABEL;
            }
        } else {
            if (pcBuf[i + 2] != '=') {
                rc = DSC_SyslogSeverityToValue(&pcBuf[i + 2], &pstFacilitySub->enSeverity);
            } else {
                rc = DSC_SyslogSeverityToValue(&pcBuf[i + 3], &pstFacilitySub->enSeverity);
            }
            if (SR_ERR_OK != rc) {
                goto ERR_LABEL;
            }
        }
    }

ERR_LABEL:

    return rc;
}

static int
__BuildSyslogServerList(const sr_val_t *pstVal, const size_t nCnt, SYSLOG_SERVER_LIST_ST *pstServerList)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0, k = 0;
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/local-syslog-filename") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog local name: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->acLocalLogName, pstVal[i].data.string_val, sizeof(pstServerList->acLocalLogName) - 1);
        } else if (strstr(pstVal[i].xpath, "/name") != NULL) {
            j++;
            k = -1;
            if (j >= SYSLOG_SERVER_NUM) {
                break;
            }
            LOG_WRITE(EN_LOG_DEBUG, "syslog server name: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->astSyslogServer[j].acName, pstVal[i].data.string_val, SYSLOG_SERVER_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/tcp/address") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog is tcp ip addr: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->astSyslogServer[j].stAction.acIp, pstVal[i].data.string_val, SYSLOG_REMOTE_IP_MAX_LEN - 1);
            pstServerList->astSyslogServer[j].stAction.enProtocolType = PROTOCOL_TCP_TYPE;
        } else if (strstr(pstVal[i].xpath, "/udp/address") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog is udp ip addr: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->astSyslogServer[j].stAction.acIp, pstVal[i].data.string_val, SYSLOG_REMOTE_IP_MAX_LEN - 1);
            pstServerList->astSyslogServer[j].stAction.enProtocolType = PROTOCOL_UDP_TYPE;
        } else if (strstr(pstVal[i].xpath, "/port") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server port: %d", pstVal[i].data.uint16_val);
            pstServerList->astSyslogServer[j].stAction.usPort = pstVal[i].data.uint16_val;
        } else if (strstr(pstVal[i].xpath, "/no-facilities") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_NONE_TYPE");
            pstServerList->astSyslogServer[j].stFacility.enFacilityType = FACILITY_NONE_TYPE;
        } else if (strstr(pstVal[i].xpath, "/facility") != NULL) {
            k++;
            pstServerList->astSyslogServer[j].stFacility.ulFacilityNum++;
            LOG_WRITE(EN_LOG_DEBUG, "syslog server:%d facility num: %d", j, pstServerList->astSyslogServer[j].stFacility.ulFacilityNum);
            if (k >= SYSLOG_FACILITY_NUM_MAX) {
                break;
            }
            pstServerList->astSyslogServer[j].stFacility.enFacilityType = FACILITY_NOT_NONE_TYPE;
            if (strstr(pstVal[i].xpath, "all") != NULL) {
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_ALL");
                strncpy(pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName,
                        pstVal[i].data.enum_val, SYSLOG_FACILITY_NAME_MAX_LEN - 1);
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityName: %s", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName);
            } else {
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_OTHER");


                memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
                sscanf(pstVal[i].data.identityref_val, "org-openroadm-syslog:%s", acFacilityNameTmp);

                strcpy(pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName, acFacilityNameTmp);
            }
        } else if ((strstr(pstVal[i].xpath, "/severity") != NULL) && (strstr(pstVal[i].xpath, "operator") == NULL)) {
            DSC_SyslogSeverityToValue(pstVal[i].data.enum_val, &pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enSeverity);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server severity: %d", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enSeverity);
        } else if (strstr(pstVal[i].xpath, "/severity-operator") != NULL) {
            DSC_SyslogOperatorEnumToValue(pstVal[i].data.enum_val, &pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enOpType);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server operator: %d", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enOpType);
        }
    }

    pstServerList->ulTotal = j + 1;
    LOG_WRITE(EN_LOG_DEBUG, "syslog server num: %d", pstServerList->ulTotal);

    return rc;
}

static int
__BuildSyslogServer(const sr_val_t *pstVal, const size_t nCnt, SYSLOG_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    int i = 0, k = 0;
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};

    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/name") != NULL) {
            k = -1;
            LOG_WRITE(EN_LOG_DEBUG, "syslog server name: %s", pstVal[i].data.string_val);
            strncpy(pstServer->acName, pstVal[i].data.string_val, SYSLOG_SERVER_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/tcp/address") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog is tcp ip addr: %s", pstVal[i].data.string_val);
            strncpy(pstServer->stAction.acIp, pstVal[i].data.string_val, SYSLOG_REMOTE_IP_MAX_LEN - 1);
            pstServer->stAction.enProtocolType = PROTOCOL_TCP_TYPE;
        } else if (strstr(pstVal[i].xpath, "/udp/address") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog is udp ip addr: %s", pstVal[i].data.string_val);
            strncpy(pstServer->stAction.acIp, pstVal[i].data.string_val, SYSLOG_REMOTE_IP_MAX_LEN - 1);
            pstServer->stAction.enProtocolType = PROTOCOL_UDP_TYPE;
        } else if (strstr(pstVal[i].xpath, "/port") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server port: %d", pstVal[i].data.uint16_val);
            pstServer->stAction.usPort = pstVal[i].data.uint16_val;
        } else if (strstr(pstVal[i].xpath, "/no-facilities") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_NONE_TYPE");
            pstServer->stFacility.enFacilityType = FACILITY_NONE_TYPE;
        } else if (strstr(pstVal[i].xpath, "/facility") != NULL) {
            k++;
            pstServer->stFacility.ulFacilityNum++;
            LOG_WRITE(EN_LOG_DEBUG, "syslog server facility num: %d", pstServer->stFacility.ulFacilityNum);
            if (k >= SYSLOG_FACILITY_NUM_MAX) {
                break;
            }
            pstServer->stFacility.enFacilityType = FACILITY_NOT_NONE_TYPE;
            if (strstr(pstVal[i].xpath, "all") != NULL) {
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_ALL");
                strncpy(pstServer->stFacility.astFacilitySub[k].acFacilityName,
                        pstVal[i].data.enum_val, SYSLOG_FACILITY_NAME_MAX_LEN - 1);
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityName: %s", pstServer->stFacility.astFacilitySub[k].acFacilityName);
            } else {
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_OTHER");


                memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
                sscanf(pstVal[i].data.identityref_val, "org-openroadm-syslog:%s", acFacilityNameTmp);

                strcpy(pstServer->stFacility.astFacilitySub[k].acFacilityName, acFacilityNameTmp);
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityName: %s", pstServer->stFacility.astFacilitySub[k].acFacilityName);
            }
        } else if ((strstr(pstVal[i].xpath, "/severity") != NULL) && (strstr(pstVal[i].xpath, "operator") == NULL)) {
            DSC_SyslogSeverityToValue(pstVal[i].data.enum_val, &pstServer->stFacility.astFacilitySub[k].enSeverity);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server severity: %d", pstServer->stFacility.astFacilitySub[k].enSeverity);
        } else if (strstr(pstVal[i].xpath, "/severity-operator") != NULL) {
            DSC_SyslogOperatorEnumToValue(pstVal[i].data.enum_val, &pstServer->stFacility.astFacilitySub[k].enOpType);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server operator: %d", pstServer->stFacility.astFacilitySub[k].enOpType);
        }
    }

    return rc;
}

int
DSC_GetSyslogServerName(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "destination", "name", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, SYSLOG_SERVER_NAME_MAX_LEN);
    sr_xpath_recover(&state);

    if ((strcmp(pcKey, "1") != 0)
        && (strcmp(pcKey, "2") != 0)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetSyslogServerName not support name : %s", pcKey);
        return SR_ERR_INVAL_ARG;
    }

    return 0;
}

int
DSC_GetSyslogFacilityName(char *pcXpath, char *pcKey)
{
    char *key_tmp = NULL;
    sr_xpath_ctx_t state = {0};

    key_tmp = sr_xpath_key_value(pcXpath, "log-facility", "facility", &state);
    if (key_tmp == NULL) {
        return -1;
    }
    strncpy(pcKey, key_tmp, SYSLOG_FACILITY_NAME_MAX_LEN);
    sr_xpath_recover(&state);

    return 0;
}

int
DSC_SyslogSeverityToValue(char *pcBuf, SYSLOG_SEVERITY_TYPE_EN *penSeverity)
{
    int rc = SR_ERR_OK;

    if (NULL == pcBuf || NULL == penSeverity) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if ((0 == strcmp(pcBuf, SYSLOG_SEVERITY_EMERG_FOR_CLI_STR)) || (0 == strcmp(pcBuf, SYSLOG_SEVERITY_EMERGENCY_STR))) {
        *penSeverity = SYSLOG_SEVERITY_EMERGENCY_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_ALERT_STR)) {
        *penSeverity = SYSLOG_SEVERITY_ALERT_TYPE;
    } else if ((0 == strcmp(pcBuf, SYSLOG_SEVERITY_CRIT_FOR_CLI_STR)) || (0 == strcmp(pcBuf, SYSLOG_SEVERITY_CRITICAL_STR))) {
        *penSeverity = SYSLOG_SEVERITY_CRITICAL_TYPE;
    } else if ((0 == strcmp(pcBuf, SYSLOG_SEVERITY_ERROR_STR)) || (0 == strcmp(pcBuf, SYSLOG_SEVERITY_ERR_FOR_CLI_STR))) {
        *penSeverity = SYSLOG_SEVERITY_ERROR_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_WARNING_STR)) {
        *penSeverity = SYSLOG_SEVERITY_WARNING_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_NOTICE_STR)) {
        *penSeverity = SYSLOG_SEVERITY_NOTICE_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_INFO_STR)) {
        *penSeverity = SYSLOG_SEVERITY_INFO_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_DEBUG_STR)) {
        *penSeverity = SYSLOG_SEVERITY_DEBUG_TYPE;
    } else if ((0 == strcmp(pcBuf, SYSLOG_SEVERITY_ALL_FOR_CLI_STR)) || (0 == strcmp(pcBuf, SYSLOG_SEVERITY_ALL_STR))) {
        *penSeverity = SYSLOG_SEVERITY_ALL_TYPE;
    } else if (0 == strcmp(pcBuf, SYSLOG_SEVERITY_NONE_STR)) {
        *penSeverity = SYSLOG_SEVERITY_NONE_TYPE;
    } else {
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

int
DSC_SyslogOperatorEnumToValue(char *pcOperator, SYSLOG_OPERATOR_TYPE_EN *penOperator)
{
    int rc = SR_ERR_OK;

    if (NULL == penOperator || NULL == pcOperator) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strcmp(pcOperator, SYSLOG_OPERATOR_EQUALS_STR)) {
        *penOperator = EQUALS_TYPE;
    } else if (0 == strcmp(pcOperator, SYSLOG_OPERATOR_NOT_EQUALS_STR)) {
        *penOperator = NOT_EQUALS_TYPE;
    } else if (0 == strcmp(pcOperator, SYSLOG_OPERATOR_EQUALS_OR_HIGHER_STR)) {
        *penOperator = EQUALS_OR_HIGHER_TYPE;
    } else {
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

char *
DSC_SyslogFacilityToString(SYSLOG_FACILITY_ST *pstFacility, char *pcBuf)
{
    int i = 0;
    char acSeveriyt[16] = {0};
    char acOperator[16] = {0};

    if (NULL == pstFacility || NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    if (FACILITY_NONE_TYPE == pstFacility->enFacilityType) {
        strcpy(pcBuf, "*.none");
        return pcBuf;
    }

    for (i = 0; i < pstFacility->ulFacilityNum; i++) {
        if (0 == strcmp(pstFacility->astFacilitySub[i].acFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR)) {
            strcat(pcBuf, "*.");
        } else {
            strcat(pcBuf, pstFacility->astFacilitySub[i].acFacilityName);
            strcat(pcBuf, ".");
        }

        memset(acOperator, 0, sizeof(acOperator));
        __SyslogOperatorToString(pstFacility->astFacilitySub[i].enOpType, acOperator);
        memset(acSeveriyt, 0, sizeof(acSeveriyt));
        __SyslogSeverityToString(true, pstFacility->astFacilitySub[i].enSeverity, acSeveriyt);

        strcat(pcBuf, acOperator);
        strcat(pcBuf, acSeveriyt);
        strcat(pcBuf, ";");
    }

    pcBuf[strlen(pcBuf) - 1] = '\0';

    return pcBuf;
}

char *
DSC_SyslogActionToString(SYSLOG_ACTION_ST *pstAction, char *pcBuf)
{

    if (NULL == pstAction || NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (pstAction->enProtocolType) {
    case PROTOCOL_TCP_TYPE:
        /* sprintf(pcBuf, "@@%s:%d", pstAction->acIp, pstAction->usPort); */
        sprintf(pcBuf, "%s", pstAction->acIp);
        break;
    case PROTOCOL_UDP_TYPE:
        /* sprintf(pcBuf, "@%s:%d", pstAction->acIp, pstAction->usPort); */
        sprintf(pcBuf, "%s", pstAction->acIp);
        break;
    default:
        break;
    }

    return pcBuf;
}

int
DSC_SyslogFacilityParse(char *pcBuf, SYSLOG_FACILITY_ST *pstFacility)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0, k = 0;
    char acBuf[64] = {0};

    if (NULL == pcBuf || NULL == pstFacility) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acBuf, 0, sizeof(acBuf));
    for (i = 0, j = 0; i < strlen(pcBuf); i++) {
        if (pcBuf[i] == ';') {
            rc = __SyslogFacilityToValue(acBuf, &pstFacility->enFacilityType, &pstFacility->astFacilitySub[k]);
            if (SR_ERR_OK != rc) {
                goto ERR_LABEL;
            }
            j = 0;
            memset(acBuf, 0, sizeof(acBuf));
            k++;
        } else {
            acBuf[j++] = pcBuf[i];
        }
    }
    rc = __SyslogFacilityToValue(acBuf, &pstFacility->enFacilityType, &pstFacility->astFacilitySub[k]);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    k++;
    pstFacility->ulFacilityNum = k;

ERR_LABEL:

    return rc;
}

int
DSC_SyslogActionToValue(char *pcBuf, SYSLOG_ACTION_ST *pstAction)
{
    int rc = SR_ERR_OK;

    if (NULL == pcBuf || NULL == pstAction) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if ('@' == pcBuf[0]) {
        if ('@' == pcBuf[1]) {
            pstAction->enProtocolType = PROTOCOL_TCP_TYPE;
            if (strlen(pcBuf) > SYSLOG_REMOTE_IP_MAX_LEN + 1) {
                rc = SR_ERR_INVAL_ARG;
                goto ERR_LABEL;
            } else {
                strcpy(pstAction->acIp, &pcBuf[2]);
            }
        } else {
            pstAction->enProtocolType = PROTOCOL_UDP_TYPE;
            if (strlen(pcBuf) > SYSLOG_REMOTE_IP_MAX_LEN) {
                rc = SR_ERR_INVAL_ARG;
                goto ERR_LABEL;
            } else {
                strcpy(pstAction->acIp, &pcBuf[1]);
            }
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

ERR_LABEL:

    return rc;
}

int
DSC_GetSyslogServer(SYSLOG_SERVER_LIST_ST *pstServers)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, XPATH_SYSLOG_ALL, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s", sr_strerror(rc));

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        pstServers->ulTotal = 0;
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        pstServers->ulTotal = 0;
        return rc;
    }

    rc = __BuildSyslogServerList(pstVal, nCnt, pstServers);
    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_AddSyslogServer(SYSLOG_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int i = 0;
    char acBuf[64] = {0};
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServer) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error pstNtpServerInfo is null!");
        goto ERR_LABEL;
    }

    /* server/tcp or udp/address */
    memset(acXpath, 0, sizeof(acXpath));
    if (pstServer->stAction.enProtocolType == PROTOCOL_TCP_TYPE) {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_TCP_ADDR, pstServer->acName);
    } else if (pstServer->stAction.enProtocolType == PROTOCOL_UDP_TYPE) {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_UDP_ADDR, pstServer->acName);
    } else {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    rc = set_string_item(sess, acXpath, pstServer->stAction.acIp, SYSLOG_REMOTE_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp or tcp address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    /* server/tcp or udp/port */
    if (pstServer->stAction.enProtocolType == PROTOCOL_TCP_TYPE) {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_TCP_PORT, pstServer->acName);
    } else if (pstServer->stAction.enProtocolType == PROTOCOL_UDP_TYPE) {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_UDP_PORT, pstServer->acName);
    } else {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
    rc = set_uint16_item(sess, acXpath, (uint16_t)pstServer->stAction.usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp or tcp port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    for (i = 0; i < pstServer->stFacility.ulFacilityNum; i++) {
        switch (pstServer->stFacility.enFacilityType) {
        case FACILITY_NONE_TYPE:
            sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_NO_FACILITIES, pstServer->acName);
            /* rc = DSC_SetUint16Item(sess, acXpath, (uint16_t)pstServer->stAction.usPort); */
            /* if (SR_ERR_OK != rc) { */
            /*    LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp or tcp port: %s\n", sr_strerror(rc)); */

            /*    goto ERR_LABEL; */
            /* } */
            break;
        case FACILITY_NOT_NONE_TYPE:
            /* facility */
            memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
            if (0 == strcmp(pstServer->stFacility.astFacilitySub[i].acFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR)) {
                strcpy(acFacilityNameTmp, pstServer->stFacility.astFacilitySub[i].acFacilityName);
            } else {
                sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstServer->stFacility.astFacilitySub[i].acFacilityName);
            }

            /* severity */
            memset(acBuf, 0, sizeof(acBuf));
            memset(acXpath, 0, sizeof(acXpath));
            __SyslogSeverityToString(false, pstServer->stFacility.astFacilitySub[i].enSeverity, acBuf);
            sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_SEVERITY,
                    pstServer->acName, acFacilityNameTmp);
            LOG_WRITE(EN_LOG_DEBUG, "Add severity server severity xpath : %s, severity : %s", acXpath, acBuf);
            rc = set_enum_item(sess, acXpath, acBuf);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
                goto ERR_LABEL;
            }

            /* operator */
            if (pstServer->stFacility.astFacilitySub[i].enSeverity != SYSLOG_SEVERITY_ALL_TYPE
                && pstServer->stFacility.astFacilitySub[i].enSeverity != SYSLOG_SEVERITY_NONE_TYPE) {
                memset(acBuf, 0, sizeof(acBuf));
                memset(acXpath, 0, sizeof(acXpath));
                __SyslogOperatorEnumToString(pstServer->stFacility.astFacilitySub[i].enOpType, acBuf);
                sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_OPERATOR,
                        pstServer->acName, acFacilityNameTmp);
                LOG_WRITE(EN_LOG_DEBUG, "Add severity server operator xpath : %s, operator : %s", acXpath, acBuf);
                rc = set_enum_item(sess, acXpath, acBuf);
                if (SR_ERR_OK != rc) {
                    LOG_WRITE(EN_LOG_ERROR, "Add operator error %s", sr_strerror(rc));
                    goto ERR_LABEL;
                }
            }
            break;
        default:
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
            break;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add syslog server DSC_Commit error %s", sr_strerror(rc));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Dsc add syslog server success!");
    }

ERR_LABEL:
    return rc;
}

int
DSC_AddSyslogServerSpec(char *pcName, char *pcIpAddr)
{
    int rc = SR_ERR_OK;
    SYSLOG_SERVER_ST stSyslogServerInfo;
    int i = 0;
    char acBuffTmp[128] = {0};
    char acAction[128] = {0};
    SYSLOG_SERVER_LIST_ST stSyslogServerList;

    if ((strcmp(pcName, "1") != 0) && (strcmp(pcName, "2") != 0)) {
        LOG_WRITE(EN_LOG_ERROR, "CMD_AddSyslogServer para error, index : %s", pcName);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stSyslogServerList, 0, sizeof(SYSLOG_SERVER_LIST_ST));
    rc = DSC_GetSyslogServer(&stSyslogServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get syslog server list %s error!", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < stSyslogServerList.ulTotal; i++) {
        memset(acAction, 0, sizeof(acAction));
        DSC_SyslogActionToString(&stSyslogServerList.astSyslogServer[i].stAction, acAction);
        if (0 == strcmp(pcIpAddr, acAction)) {
            LOG_WRITE(EN_LOG_ERROR, "error CMD_AddSyslogServer exist the same ip : %s", acAction);
            rc = SR_ERR_EXISTS;
            goto ERR_LABEL;
        }
    }

    memset(&stSyslogServerInfo, 0, sizeof(SYSLOG_SERVER_ST));
    strncpy(stSyslogServerInfo.acName, pcName, SYSLOG_SERVER_NAME_MAX_LEN - 1);
    rc = DSC_SyslogFacilityParse("*.=alert", &stSyslogServerInfo.stFacility);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SyslogFacilityParse error!");
        goto ERR_LABEL;
    }

    memset(acBuffTmp, 0, sizeof(acBuffTmp));
    sprintf(acBuffTmp, "@%s", pcIpAddr);
    rc = DSC_SyslogActionToValue(acBuffTmp, &stSyslogServerInfo.stAction);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SyslogActionToValue error!");
        goto ERR_LABEL;
    }

    stSyslogServerInfo.stAction.usPort = 514;
    rc = DSC_AddSyslogServer(&stSyslogServerInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddSyslogServer error!");
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_DelSyslogServer(char *pcServerName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcServerName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, XPATH_SYSLOG_SERVER_LIST_NAME, pcServerName);

    return DSC_DelNode(sess, xpath);
}

int
DSC_SetSyslogIP(char *pcServerName, char *pcServerIP)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    SYSLOG_SERVER_ST stServerTmp;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_ALL, pcServerName);

    rc = get_items(sess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s", sr_strerror(rc));

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        return rc;
    }

    memset(&stServerTmp, 0, sizeof(SYSLOG_SERVER_ST));
    rc = __BuildSyslogServer(pstVal, nCnt, &stServerTmp);
    sr_free_values(pstVal, nCnt);

    memset(acXpath, 0, sizeof(acXpath));
    if (PROTOCOL_TCP_TYPE == stServerTmp.stAction.enProtocolType) {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_TCP_ADDR, pcServerName);
    } else {
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_UDP_ADDR, pcServerName);
    }

    rc = set_string_item(sess, acXpath, pcServerIP, SYSLOG_REMOTE_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp or tcp address: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSyslogIP DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_SetSyslogIP parameters success!");
    }

    return rc;
}

int
DSC_AddSyslogFacility(char *pcServerName, SYSLOG_FACILITY_SUB_ST *pstFacility)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acBuf[64] = {0};
    char acFacilityNameTmp[DEF_BUF_SIZE_128] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pstFacility->acFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR)) {
        strcpy(acFacilityNameTmp, pstFacility->acFacilityName);
    } else {
        sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstFacility->acFacilityName);
    }

    LOG_WRITE(EN_LOG_DEBUG, "DSC_AddSyslogFacility acFacilityNameTmp:  %s", acFacilityNameTmp);

    /* severity */
    memset(acBuf, 0, sizeof(acBuf));
    __SyslogSeverityToString(false, pstFacility->enSeverity, acBuf);
    LOG_WRITE(EN_LOG_DEBUG, "DSC_AddSyslogFacility enSeverity:  %s", acBuf);
    sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_SEVERITY,
            pcServerName, acFacilityNameTmp);
    rc = set_enum_item(sess, acXpath, acBuf);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* operator */
    if (pstFacility->enSeverity != SYSLOG_SEVERITY_ALL_TYPE
        && pstFacility->enSeverity != SYSLOG_SEVERITY_NONE_TYPE) {
        memset(acBuf, 0, sizeof(acBuf));
        __SyslogOperatorEnumToString(pstFacility->enOpType, acBuf);
        LOG_WRITE(EN_LOG_DEBUG, "DSC_AddSyslogFacility enSeverity:  %s", acBuf);
        sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_OPERATOR,
                pcServerName, acFacilityNameTmp);
        rc = set_enum_item(sess, acXpath, acBuf);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Add operator error %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddSyslogFacility DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_AddSyslogFacility parameters success!");
    }

    return rc;
}

int
DSC_DelSyslogFacility(char *pcServerName, char *pcFacilityName)
{
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};

    if (NULL == pcServerName || NULL == pcFacilityName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName or pcFacilityName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pcFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_FOR_CLI_STR)) {
        strcpy(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR);
    } else {
        sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pcFacilityName);
    }

    sprintf(xpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_FACILITIES, pcServerName, acFacilityNameTmp);

    return DSC_DelNode(sess, xpath);
}

int
DSC_SetSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN enSeverity)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acBuf[64] = {0};
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pstFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_FOR_CLI_STR)) {
        strcpy(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR);
    } else {
        sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstFacilityName);
    }

    /* severity */
    memset(acBuf, 0, sizeof(acBuf));
    __SyslogSeverityToString(false, enSeverity, acBuf);
    sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_SEVERITY,
            pcName, pstFacilityName);
    rc = set_enum_item(sess, acXpath, acBuf);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set severity error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSyslogSeverity DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_SetSyslogSeverity parameters success!");
    }

    return rc;
}

int
DSC_GetSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN *penSeverity)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    size_t nCnt = 0;
    int i = 0;
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetOperSession();

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pstFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_FOR_CLI_STR)) {
        strcpy(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR);
    } else {
        sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstFacilityName);
    }

    sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_ALL,
            pcName, acFacilityNameTmp);
    rc = get_items(sess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath);

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        return SR_ERR_INVAL_ARG;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        return rc;
    }

    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/severity") != NULL) {
            DSC_SyslogSeverityToValue(pstVal[i].data.enum_val, penSeverity);
            break;
        }
    }

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_SetSyslogOperator(char *pcName, char *pstFacilityName, SYSLOG_OPERATOR_TYPE_EN enOperator)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acBuf[64] = {0};
    SYSLOG_SEVERITY_TYPE_EN enSeverity = SYSLOG_SEVERITY_NONE_TYPE;
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    rc = DSC_GetSyslogSeverity(pcName, pstFacilityName, &enSeverity);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSyslogOperator severity not support error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (enSeverity == SYSLOG_SEVERITY_ALL_TYPE || enSeverity == SYSLOG_SEVERITY_NONE_TYPE) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSyslogOperator severity not support error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pstFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_FOR_CLI_STR)) {
        strcpy(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR);
    } else {
        sprintf(acFacilityNameTmp, SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstFacilityName);
    }

    /* severity */
    memset(acBuf, 0, sizeof(acBuf));
    __SyslogOperatorEnumToString(enOperator, acBuf);
    sprintf(acXpath, XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_OPERATOR, pcName, acFacilityNameTmp);
    rc = set_enum_item(sess, acXpath, acBuf);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set operator error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSyslogIP DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */
    } else {
        LOG_WRITE(EN_LOG_DEBUG, "DSC_SetSyslogOperator parameters success!");
    }

    return rc;
}


/**************************************************************************************************/
/* definition of openconfig-system-logging.yang syslog support command table.                     */
/**************************************************************************************************/

int
DSC_GetOcsRemoteSrvHostKey(char *pcXpath, char *pcKey)
{
    int rc = SR_ERR_OK;
    char *pcTmp = NULL;
    sr_xpath_ctx_t state = {0};

    if (NULL == pcXpath || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p", pcXpath, pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pcTmp = sr_xpath_key_value(pcXpath, "remote-server", "host", &state);
    if (pcTmp == NULL) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    strncpy(pcKey, pcTmp, SYSLOG_SERVER_NAME_MAX_LEN - 1);
    sr_xpath_recover(&state);

cleanup:
    return rc;
}

int
DSC_GetOcsSelectorFacilityKey(char *pcXpath, char *pcKey)
{
    int rc = SR_ERR_OK;
    char *pcTmp = NULL;
    sr_xpath_ctx_t state = {0};

    if (NULL == pcXpath || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p", pcXpath, pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pcTmp = sr_xpath_key_value(pcXpath, "selector", "facility", &state);
    if (NULL == pcTmp) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    strncpy(pcKey, pcTmp, SYSLOG_FACILITY_NAME_MAX_LEN - 1);
    sr_xpath_recover(&state);

cleanup:
    return rc;
}

int
DSC_GetOcsSelectorSeverityKey(char *pcXpath, char *pcKey)
{
    int rc = SR_ERR_OK;
    char *pcTmp = NULL;
    sr_xpath_ctx_t state = {0};

    if (NULL == pcXpath || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p", pcXpath, pcKey);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pcTmp = sr_xpath_key_value(pcXpath, "selector", "severity", &state);
    if (NULL == pcTmp) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    strncpy(pcKey, pcTmp, SYSLOG_FACILITY_NAME_MAX_LEN - 1);
    sr_xpath_recover(&state);

cleanup:
    return rc;
}

static char *
__OcSyslogSeverityToString(bool bIsUsedForCmd, SYSLOG_SEVERITY_TYPE_EN enSeverity, char *pcBuf)
{

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enSeverity) {
    case SYSLOG_SEVERITY_EMERGENCY_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_EMERG_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_EMERGENCY_STR);
        }
        break;
    case SYSLOG_SEVERITY_ALERT_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_ALERT_STR);
        break;
    case SYSLOG_SEVERITY_CRITICAL_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_CRIT_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_CRITICAL_STR);
        }
        break;
    case SYSLOG_SEVERITY_ERROR_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_ERR_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_ERROR_STR);
        }
        break;
    case SYSLOG_SEVERITY_WARNING_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_WARNING_STR);
        break;
    case SYSLOG_SEVERITY_NOTICE_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_NOTICE_STR);
        break;
    case SYSLOG_SEVERITY_INFO_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_INFO_STR);
        break;
    case SYSLOG_SEVERITY_DEBUG_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_DEBUG_STR);
        break;
    case SYSLOG_SEVERITY_ALL_TYPE:
        if (bIsUsedForCmd) {
            strcpy(pcBuf, SYSLOG_SEVERITY_ALL_FOR_CLI_STR);
        } else {
            strcpy(pcBuf, SYSLOG_SEVERITY_ALL_STR);
        }
        break;
    case SYSLOG_SEVERITY_NONE_TYPE:
        strcpy(pcBuf, SYSLOG_SEVERITY_NONE_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

static int
__DelServiceNode(char *pcXPath)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pcXPath) {
        LOG_WRITE(EN_LOG_ERROR, "pcXPath is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_delete_item(sess, pcXPath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete is error,%s", sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

int
DSC_OcSyslogSeverityToValue(char *pcBuf, SYSLOG_SEVERITY_TYPE_EN *penSeverity)
{
    int rc = SR_ERR_OK;

    if (NULL == pcBuf || NULL == penSeverity) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if ((0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_EMERG_FOR_CLI_STR)) || (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_EMERGENCY_STR))) {
        *penSeverity = SYSLOG_SEVERITY_EMERGENCY_TYPE;
    } else if (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_ALERT_STR)) {
        *penSeverity = SYSLOG_SEVERITY_ALERT_TYPE;
    } else if ((0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_CRIT_FOR_CLI_STR)) || (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_CRITICAL_STR))) {
        *penSeverity = SYSLOG_SEVERITY_CRITICAL_TYPE;
    } else if ((0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_ERROR_STR)) || (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_ERR_FOR_CLI_STR))) {
        *penSeverity = SYSLOG_SEVERITY_ERROR_TYPE;
    } else if (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_WARNING_STR)) {
        *penSeverity = SYSLOG_SEVERITY_WARNING_TYPE;
    } else if (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_NOTICE_STR)) {
        *penSeverity = SYSLOG_SEVERITY_NOTICE_TYPE;
    } else if (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_INFO_STR)) {
        *penSeverity = SYSLOG_SEVERITY_INFO_TYPE;
    } else if (0 == strcasecmp(pcBuf, OCS_LOGGING_SEVERITY_DEBUG_STR)) {
        *penSeverity = SYSLOG_SEVERITY_DEBUG_TYPE;
    } else {
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

/*
   qingy@shasta-1:~/work/shasta/apps_pub/sysrepo/build/examples$ ./sr_get_items_example
   count=38
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/host = 1
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/config/host = 1
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/config/source-address = 127.0.0.1
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/config/remote-port = 514
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/config/com-oplink-system-logging:transport-protocol = com-oplink-system-logging:UDP
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT'] (list instance)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/facility = openconfig-system-logging:ALL
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/severity = ALERT
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config/facility = openconfig-system-logging:ALL
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config/severity = ALERT
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING'] (list instance)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/facility = openconfig-system-logging:USER
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/severity = WARNING
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config/facility = openconfig-system-logging:USER
   /openconfig-system:system/logging/remote-servers/remote-server[host='1']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config/severity = WARNING
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/host = 2
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/config/host = 2
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/config/source-address = 192.168.1.111
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/config/remote-port = 514
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/config/com-oplink-system-logging:transport-protocol = com-oplink-system-logging:TCP
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT'] (list instance)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/facility = openconfig-system-logging:ALL
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/severity = ALERT
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config/facility = openconfig-system-logging:ALL
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:ALL'][severity='ALERT']/config/severity = ALERT
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING'] (list instance)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/facility = openconfig-system-logging:USER
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/severity = WARNING
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config (container)
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config/facility = openconfig-system-logging:USER
   /openconfig-system:system/logging/remote-servers/remote-server[host='2']/selectors/selector[facility='openconfig-system-logging:USER'][severity='WARNING']/config/severity = WARNING
 */
static int
__BuildOcSyslogServerList(const sr_val_t *pstVal, const size_t nCnt, SYSLOG_SERVER_LIST_ST *pstServerList)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0, k = 0;
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "]/host") != NULL) {
            j++;
            k = -1;
            if (j >= SYSLOG_SERVER_NUM) {
                break;
            }
            LOG_WRITE(EN_LOG_DEBUG, "syslog server name: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->astSyslogServer[j].acName, pstVal[i].data.string_val, SYSLOG_SERVER_NAME_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "state") != NULL) {
            continue;
        } else if (strstr(pstVal[i].xpath, "/source-address") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server ip addr: %s", pstVal[i].data.string_val);
            strncpy(pstServerList->astSyslogServer[j].stAction.acIp, pstVal[i].data.string_val, SYSLOG_REMOTE_IP_MAX_LEN - 1);
        } else if (strstr(pstVal[i].xpath, "/remote-port") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog server port: %d", pstVal[i].data.uint16_val);
            pstServerList->astSyslogServer[j].stAction.usPort = pstVal[i].data.uint16_val;
        } else if (strstr(pstVal[i].xpath, "transport-protocol") != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "syslog transport-protocol: %s", pstVal[i].data.identityref_val);
            if (NULL != strstr(pstVal[i].data.identityref_val, TRANSPORT_PROTOCOL_UDP)) {
                pstServerList->astSyslogServer[j].stAction.enProtocolType = PROTOCOL_UDP_TYPE;
                LOG_WRITE(EN_LOG_DEBUG, "PROTOCOL_UDP_TYPE");
            } else if (NULL != strstr(pstVal[i].data.identityref_val, TRANSPORT_PROTOCOL_TCP)) {
                pstServerList->astSyslogServer[j].stAction.enProtocolType = PROTOCOL_TCP_TYPE;
                LOG_WRITE(EN_LOG_DEBUG, "PROTOCOL_TCP_TYPE");
            }
        } else if (strstr(pstVal[i].xpath, "]/facility") != NULL) {
            k++;
            pstServerList->astSyslogServer[j].stFacility.ulFacilityNum++;
            LOG_WRITE(EN_LOG_DEBUG, "syslog server:%d facility num: %d", j, pstServerList->astSyslogServer[j].stFacility.ulFacilityNum);
            if (k >= SYSLOG_FACILITY_NUM_MAX) {
                break;
            }
            pstServerList->astSyslogServer[j].stFacility.enFacilityType = FACILITY_NOT_NONE_TYPE;
            /* if (strstr(pstVal[i].xpath, "all") != NULL) {
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_ALL");
                strncpy(pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName,
                        pstVal[i].data.enum_val, SYSLOG_FACILITY_NAME_MAX_LEN - 1);
                LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityName: %s", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName);
               } else {*/
            LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityType: FACILITY_TYPE_OTHER");


            memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
            sscanf(pstVal[i].data.identityref_val, "openconfig-system-logging:%s", acFacilityNameTmp);

            UTIL_LowerStr(acFacilityNameTmp);

            strncpy(pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName,
                    acFacilityNameTmp, SYSLOG_FACILITY_NAME_MAX_LEN);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server FacilityName: %s", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].acFacilityName);
            /* } */
        } else if (strstr(pstVal[i].xpath, "]/severity") != NULL) {
            DSC_OcSyslogSeverityToValue(pstVal[i].data.enum_val, &pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enSeverity);
            LOG_WRITE(EN_LOG_DEBUG, "syslog server severity: %d", pstServerList->astSyslogServer[j].stFacility.astFacilitySub[k].enSeverity);
        }
    }

    pstServerList->ulTotal = j + 1;
    LOG_WRITE(EN_LOG_DEBUG, "syslog server num: %d", pstServerList->ulTotal);

    return rc;
}


int
DSC_OcSyslogGetFacilityName(char *pcNameIn, char *pcNameOut)
{
    if (NULL == pcNameIn || NULL == pcNameOut) {
        LOG_WRITE(EN_LOG_ERROR, "__SyslogGetFacilityName input null");
        return SR_ERR_INVAL_ARG;
    }

    if (strcmp(pcNameIn, "all") == 0) {
        strcpy(pcNameOut, pcNameIn);
    } else {
        /* sprintf(pcNameOut, "openconfig-system-logging:%s", pcNameIn); */
        sscanf(pcNameIn, "openconfig-system-logging:%s", pcNameOut);
        /* strncpy(pcNameOut, pcNameIn+strlen("openconfig-system-logging:"), sizeof(SYSLOG_SERVER_NAME_MAX_LEN)-1); */
    }

    return SR_ERR_OK;
}

int
DSC_GetOcSyslogServerConfig(SYSLOG_SERVER_LIST_ST *pstServers)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_ALL, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_NOTICE, "Not found item: %s", sr_strerror(rc));

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        pstServers->ulTotal = 0;
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        pstServers->ulTotal = 0;
        return rc;
    }

#if 0
    /* printf("nCnt=%d\n",(int)nCnt); */
    for (size_t i = 0; i < nCnt; i++) {
        sr_print_val(pstVal + i);
    }
#endif

    LOG_WRITE(EN_LOG_DEBUG, "nCnt: %d", (int)nCnt);

    rc = __BuildOcSyslogServerList(pstVal, nCnt, pstServers);
    sr_free_values(pstVal, nCnt);

    return rc;
}


int
DSC_GetOcSyslogServer(SYSLOG_SERVER_LIST_ST *pstServers)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == pstServers) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_ALL, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s", sr_strerror(rc));

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        pstServers->ulTotal = 0;
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        pstServers->ulTotal = 0;
        return rc;
    }

    LOG_WRITE(EN_LOG_DEBUG, "nCnt: %d", (int)nCnt);

    rc = __BuildOcSyslogServerList(pstVal, nCnt, pstServers);
    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_AddOcSyslogServer(SYSLOG_SERVER_ST *pstServer)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int i = 0;
    char acBuf[64] = {0};
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstServer) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Error pstNtpServerInfo is null!");
        goto ERR_LABEL;
    }
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_HOST, pstServer->acName);
    rc = set_string_item(sess, acXpath, pstServer->acName, SYSLOG_SERVER_NAME_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-host: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* server source-address*/
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_SOURCE_ADDRESS, pstServer->acName);
    rc = set_string_item(sess, acXpath, pstServer->stAction.acIp, SYSLOG_REMOTE_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-source-address: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_REMOTE_PORT, pstServer->acName);
    rc = set_uint16_item(sess, acXpath, (uint16_t)pstServer->stAction.usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-remote port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_PROTOCOL, pstServer->acName);
    if ( PROTOCOL_UDP_TYPE == pstServer->stAction.enProtocolType ) {
        rc = set_identityref_item(sess, acXpath, "com-oplink-system-logging:UDP", strlen("com-oplink-system-logging:UDP"));
    } else {
        rc = set_identityref_item(sess, acXpath, "com-oplink-system-logging:TCP", strlen("com-oplink-system-logging:TCP"));
    }
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-remote port: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }


    for (i = 0; i < pstServer->stFacility.ulFacilityNum; i++) {
        /* facility */
        memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
        sprintf(acFacilityNameTmp, "openconfig-system-logging:%s", UTIL_UpperStr(pstServer->stFacility.astFacilitySub[i].acFacilityName));

        /* severity */
        memset(acBuf, 0, sizeof(acBuf));
        memset(acXpath, 0, sizeof(acXpath));
        __OcSyslogSeverityToString(false, pstServer->stFacility.astFacilitySub[i].enSeverity, acBuf);
        UTIL_UpperStr(acBuf);


        sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_FACILITY,
                pstServer->acName, acFacilityNameTmp, acBuf);
        LOG_WRITE(EN_LOG_DEBUG, "Add severity server facility xpath : %s", acXpath);
        LOG_WRITE(EN_LOG_DEBUG, "facility : %s, severity : %s", acFacilityNameTmp, acBuf);
        rc = set_identityref_item(sess, acXpath, acFacilityNameTmp, SYSLOG_REMOTE_IP_MAX_LEN);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
            goto ERR_LABEL;
        }

        sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_SEVERITY,
                pstServer->acName, acFacilityNameTmp, acBuf);
        LOG_WRITE(EN_LOG_DEBUG, "severity : %s", acBuf);
        rc = set_enum_item(sess, acXpath, acBuf);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
            goto ERR_LABEL;
        }


    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add ocsyslog server DSC_Commit error %s", sr_strerror(rc));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Dsc add ocsyslog server success!");
    }

ERR_LABEL:
    return rc;
}

int
DSC_AddOcSyslogServerSpec(char *pcName, char *pcProtol, char *pcIpAddr, uint16_t usPort)
{
    int rc = SR_ERR_OK;
    SYSLOG_SERVER_ST stSyslogServerInfo;
    int i = 0;
    char acBuffTmp[128] = {0};
    char acAction[128] = {0};
    SYSLOG_SERVER_LIST_ST stSyslogServerList;
    SYSLOG_PROTOCOL_TYPE_EN enProtocolType;
#if 0
    if ((strcmp(pcName, "1") != 0) && (strcmp(pcName, "2") != 0)) {
        LOG_WRITE(EN_LOG_ERROR, "CMD_AddSyslogServer para error, index : %s", pcName);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }
#endif
    rc = UTIL_ChkIpV4Addr(pcIpAddr);
    LOG_WRITE(EN_LOG_NOTICE, "UTIL_ChkIpV4Addr rc:%d, pcIpAddr:%s", rc, pcIpAddr);
    if (rc != OPLK_OK) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "UTIL_ChkIpV4Addr rc:%d, pcIpAddr:%s", rc, pcIpAddr);
        goto ERR_LABEL;
    }

    if (usPort < 1 || usPort > 65535) {
        LOG_WRITE(EN_LOG_ERROR, "CMD_AddSyslogServer para error, port : %d", usPort);
        /* usPort = 514; */
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (0 == strncasecmp(pcProtol, TRANSPORT_PROTOCOL_UDP, strlen(TRANSPORT_PROTOCOL_UDP))) {
        enProtocolType = PROTOCOL_UDP_TYPE;
    } else if (0 == strncasecmp(pcProtol, TRANSPORT_PROTOCOL_TCP, strlen(TRANSPORT_PROTOCOL_TCP))) {
        enProtocolType = PROTOCOL_TCP_TYPE;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddOcSyslogServer Protol error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stSyslogServerList, 0, sizeof(SYSLOG_SERVER_LIST_ST));
    rc = DSC_GetOcSyslogServerConfig(&stSyslogServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get syslog server list %s error!", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < stSyslogServerList.ulTotal; i++) {
        memset(acAction, 0, sizeof(acAction));
        DSC_SyslogActionToString(&stSyslogServerList.astSyslogServer[i].stAction, acAction);
        if (0 == strcmp(pcIpAddr, acAction)
            && usPort == stSyslogServerList.astSyslogServer[i].stAction.usPort
            && enProtocolType == stSyslogServerList.astSyslogServer[i].stAction.enProtocolType) {
            LOG_WRITE(EN_LOG_ERROR, "CMD_AddOcSyslogServer exist the same server,ip : %s,port:%d,ProtocolType:%d", acAction, usPort, enProtocolType);
            rc = SR_ERR_EXISTS;

            goto ERR_LABEL;
        }
    }

    memset(&stSyslogServerInfo, 0, sizeof(SYSLOG_SERVER_ST));
    strncpy(stSyslogServerInfo.acName, pcName, SYSLOG_SERVER_NAME_MAX_LEN - 1);
    rc = DSC_SyslogFacilityParse("local6.debug", &stSyslogServerInfo.stFacility); /* "*.=alert", */
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SyslogFacilityParse error!");
        goto ERR_LABEL;
    }

    memset(acBuffTmp, 0, sizeof(acBuffTmp));
    sprintf(acBuffTmp, "@%s", pcIpAddr);
    rc = DSC_SyslogActionToValue(acBuffTmp, &stSyslogServerInfo.stAction);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SyslogActionToValue error!");
        goto ERR_LABEL;
    }

    stSyslogServerInfo.stAction.usPort = usPort;  /* default 514 */

    if (0 == strncasecmp(pcProtol, TRANSPORT_PROTOCOL_UDP, strlen(TRANSPORT_PROTOCOL_UDP))) {
        stSyslogServerInfo.stAction.enProtocolType = PROTOCOL_UDP_TYPE;
    } else if (0 == strncasecmp(pcProtol, TRANSPORT_PROTOCOL_TCP, strlen(TRANSPORT_PROTOCOL_TCP))) {

        stSyslogServerInfo.stAction.enProtocolType = PROTOCOL_TCP_TYPE;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddOcSyslogServer Protol error!");
        goto ERR_LABEL;
    }

    rc = DSC_AddOcSyslogServer(&stSyslogServerInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddSyslogServer error!");
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
DSC_DelOcSyslogServer(char *pcServerName)
{
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcServerName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    sprintf(xpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER, pcServerName);

    return __DelServiceNode(xpath);
}

int
DSC_SetOcSyslogIP(char *pcServerName, char *pcServerIP)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_SOURCE_ADDRESS, pcServerName);

    rc = set_string_item(sess, acXpath, pcServerIP, SYSLOG_REMOTE_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-udp or tcp address: %s", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetOcSyslogIP DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_NOTICE, "DSC_SetOcSyslogIP parameters success!");
    }

    return rc;
}

int
DSC_AddOcSyslogFacility(char *pcServerName, SYSLOG_FACILITY_SUB_ST *pstFacility)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acBuf[64] = {0};
    char acFacilityNameTmp[DEF_BUF_SIZE_128] = {0};
    /* char acFacilityNameTmp[DEF_BUF_SIZE_128] = {0}; */
    sr_session_ctx_t *sess = DSC_GetRunningSession();


    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    if (0 == strcmp(pstFacility->acFacilityName, SYSLOG_FACILITY_TYPE_ENUM_ALL_STR)) {
        strcpy(acFacilityNameTmp, pstFacility->acFacilityName);
    } else {
        snprintf(acFacilityNameTmp, sizeof(acFacilityNameTmp), SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR, pstFacility->acFacilityName);
    }

    memset(acBuf, 0, sizeof(acBuf));
    __OcSyslogSeverityToString(false, pstFacility->enSeverity, acBuf);
    UTIL_UpperStr(acBuf);


    LOG_WRITE(EN_LOG_DEBUG, "DSC_AddOcSyslogFacility acFacilityNameTmp:  %s", acFacilityNameTmp);
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_FACILITY,
            pcServerName, acFacilityNameTmp, acBuf);
    LOG_WRITE(EN_LOG_DEBUG, "Add Secl server severity xpath : %s, severity : %s", acXpath, acBuf);
    rc = set_string_item(sess, acXpath, acFacilityNameTmp, SYSLOG_REMOTE_IP_MAX_LEN);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* severity */
    LOG_WRITE(EN_LOG_DEBUG, "DSC_AddOcSyslogFacility enSeverity:  %s", acBuf);
    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_SEVERITY,
            pcServerName, acFacilityNameTmp, acBuf);
    rc = set_enum_item(sess, acXpath, acBuf);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Add severity error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }


    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AddOcSyslogFacility DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_NOTICE, "DSC_AddSyslogFacility parameters success!");
    }

    return rc;
}

int
DSC_DelOcSyslogFacility(char *pcServerName, char *pcFacilityName, char *pcSeverityName)
{
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char acFacilityNameTmp[SYSLOG_FACILITY_NAME_MAX_LEN] = {0};

    if (NULL == pcServerName || NULL == pcFacilityName || NULL == pcSeverityName) {
        LOG_WRITE(EN_LOG_ERROR, "pcServerName or pcFacilityName is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    memset(acFacilityNameTmp, 0, sizeof(acFacilityNameTmp));
    sprintf(acFacilityNameTmp, "openconfig-system-logging:%s", pcFacilityName);

    sprintf(xpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR, pcServerName, acFacilityNameTmp, pcSeverityName);

    return __DelServiceNode(xpath);
}

int
DSC_SetOcSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN enSeverity)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acBuf[64] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* severity */
    memset(acBuf, 0, sizeof(acBuf));
    __OcSyslogSeverityToString(false, enSeverity, acBuf);
    UTIL_UpperStr(acBuf);

    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_SEVERITY,
            pcName, pstFacilityName, acBuf);
    rc = set_enum_item(sess, acXpath, acBuf);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set severity error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetOcSyslogSeverity DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    if (SR_ERR_OK != rc) {
        /* make the elements which have been set successfully invalid */

    } else {
        LOG_WRITE(EN_LOG_NOTICE, "DSC_SetOcSyslogSeverity parameters success!");
    }

    return rc;
}

int
DSC_GetOcSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN *penSeverity)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    size_t nCnt = 0;
    int i = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    sprintf(acXpath, OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_ALL,
            pcName);
    rc = get_items(sess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath);

        if (pstVal != NULL)
            sr_free_values(pstVal, nCnt);

        return SR_ERR_INVAL_ARG;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items: %s", sr_strerror(rc));
        return rc;
    }

    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "]/severity") != NULL) {
            DSC_SyslogSeverityToValue(pstVal[i].data.enum_val, penSeverity);
            break;
        }
    }

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_OcProcessRPCResponse(sr_val_t *output, int output_cnt)
{
    int rc = SR_ERR_OK;

    /* print output values */
    /* printf("\n>>> Received an RPC response:\n\n"); */
    /* for (size_t i = 0; i < output_cnt; ++i) { */
    /*    sr_print_val(output+i); */
    /* } */
    /* printf("\n"); */

    /* expect at least 1 return value */
    if (output_cnt < 1) {
        rc = SR_ERR_OPERATION_FAILED;
    } else if (strcmp(output[0].data.string_val, RPC_STATUS_SUCCESS) == 0) {
        LOG_WRITE(EN_LOG_NOTICE, "rpc %s, succeed: %s", output[0].xpath, output[0].data.string_val);
        rc = SR_ERR_OK;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[0].xpath, output[0].data.string_val);
        rc = SR_ERR_OPERATION_FAILED;
    }

    /* don't forget to de-allocate the output values */
    sr_free_values(output, output_cnt);
    return rc;
}


int
DSC_UploadSyslogRpc(char *hostname, uint16_t port, char *username, char *password)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 4;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(hostname);
    rc = sr_val_set_xpath(&input[0], OC_RPC_XPATH_UPLOAD_SYSLOG_HOSTNAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OC_RPC_XPATH_UPLOAD_SYSLOG_HOSTNAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_UINT16_T;
    input[1].data.uint16_val = port;
    rc = sr_val_set_xpath(&input[1], OC_RPC_XPATH_UPLOAD_SYSLOG_PORT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OC_RPC_XPATH_UPLOAD_SYSLOG_PORT, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_STRING_T;
    input[2].data.string_val = strdup(username);
    rc = sr_val_set_xpath(&input[2], OC_RPC_XPATH_UPLOAD_SYSLOG_USERNAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OC_RPC_XPATH_UPLOAD_SYSLOG_USERNAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[3].type = SR_STRING_T;
    input[3].data.string_val = strdup(password);
    rc = sr_val_set_xpath(&input[3], OC_RPC_XPATH_UPLOAD_SYSLOG_PASSWORD);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", OC_RPC_XPATH_UPLOAD_SYSLOG_PASSWORD, sr_strerror(rc));
        goto ERR_LABEL;
    }
    LOG_WRITE(EN_LOG_NOTICE, "DSC_UploadOcSyslogRpc");
    rc = DSC_RpcSend(sess, OC_RPC_XPATH_UPLOAD_SYSLOG, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_UploadSyslogRpc Send: %s %s", OC_RPC_XPATH_UPLOAD_SYSLOG, sr_strerror(rc));
        goto ERR_LABEL;
    }
    sr_free_values(input, input_cnt);
    return DSC_OcProcessRPCResponse(output, output_cnt);
ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}
int
DSC_OcRpcUploadSyslogCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt)
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
    rc = sr_val_set_xpath(&(*output)[0], OC_RPC_XPATH_UPLOAD_SYSLOG_MESSAGE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", OC_RPC_XPATH_UPLOAD_SYSLOG_MESSAGE, sr_strerror(rc));
        goto cleanup;
    }
    rc = sr_val_set_str_data(&(*output)[0], SR_STRING_T, message);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
        goto cleanup;
    }
    *output_cnt = 1;
cleanup:
    return rc;
}

/*
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1'] (list instance)
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/index = 1
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/rx-status = Success
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/message = Rx-Status Success
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/far-end-info (container)
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/far-end-info/chassis-name = FMD01
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/far-end-info/port-name = In05
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/far-end-infos[index='1']/connection-loss = 1.74
 */
int
DSC_LinkDiscoveyRpcResp(sr_val_t *output, int output_cnt, ST_LINK_DISCOVERY_RPC_RESP *stLinkDiscoveryRpcResp)
{
    int rc = SR_ERR_OK;
#if 0
    /* print output values */
    printf("\n>>> Received an RPC response:\n\n");
    for (size_t i = 0; i < output_cnt; ++i) {
        sr_print_val(output + i);
    }
    printf("\n");
#endif
    /* expect at least 1 return value */
    if (output_cnt < 1) {
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    for (size_t i = 0; i < output_cnt; ++i) {
        if (strstr(output[i].xpath, "]/index") != NULL) {
            stLinkDiscoveryRpcResp->iIndex = output[i].data.int32_val;
            LOG_WRITE(EN_LOG_NOTICE, "iIndex: %d", stLinkDiscoveryRpcResp->iIndex);
        } else if (strstr(output[i].xpath, "]/rx-status") != NULL) {
            memcpy(stLinkDiscoveryRpcResp->acRxStatus, output[i].data.enum_val, sizeof(stLinkDiscoveryRpcResp->acRxStatus) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "acRxStatus: %s", stLinkDiscoveryRpcResp->acRxStatus);
        } else if (strstr(output[i].xpath, "]/message") != NULL) {
            memcpy(stLinkDiscoveryRpcResp->acMessage, output[i].data.string_val, sizeof(stLinkDiscoveryRpcResp->acMessage) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "acMessage: %s", stLinkDiscoveryRpcResp->acMessage);
        } else if (strstr(output[i].xpath, "]/far-end-info/chassis-name") != NULL) {
            memcpy(stLinkDiscoveryRpcResp->acRemoteChassisName, output[i].data.string_val, sizeof(stLinkDiscoveryRpcResp->acRemoteChassisName) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "acRemoteChassisName: %s", stLinkDiscoveryRpcResp->acRemoteChassisName);
        } else if (strstr(output[i].xpath, "]/far-end-info/port-name") != NULL) {
            memcpy(stLinkDiscoveryRpcResp->acRemotePortName, output[i].data.string_val, sizeof(stLinkDiscoveryRpcResp->acRemotePortName) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "acRemotePortName: %s", stLinkDiscoveryRpcResp->acRemotePortName);
        } else if (strstr(output[i].xpath, "]/far-end-info/connection-loss") != NULL) {
            memcpy(stLinkDiscoveryRpcResp->acConnectionLoss, output[i].data.string_val, sizeof(stLinkDiscoveryRpcResp->acConnectionLoss) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "acConnectionLoss: %s", stLinkDiscoveryRpcResp->acConnectionLoss);
        }
    }

ERR_LABEL:

    /* don't forget to de-allocate the output values */
    sr_free_values(output, output_cnt);
    return rc;
}

/*
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/port-pairs[index='1']/index = 1
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/port-pairs[index='1']/tx-port = Out22
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/port-pairs[index='1']/rx-port = In01
   /openconfig-wavelength-selective-switch:specify-connection-validation-ports/hold-on-time = 10
 */
int
DSC_LinkDiscoveyRpc(int iIndex, char *pcTxPort, char *pcRxPort, int iHoldOnTime, ST_LINK_DISCOVERY_RPC_RESP *stLinkDiscoveryRpcResp)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 4;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[0].type = SR_INT32_T;
    input[0].data.int32_val = iIndex;
    rc = sr_val_set_xpath(&input[0], "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/index");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/index", sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_STRING_T;
    input[1].data.string_val = strdup(pcTxPort);;
    rc = sr_val_set_xpath(&input[1], "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/tx-port");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/tx-port", sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_STRING_T;
    input[2].data.string_val = strdup(pcRxPort);
    rc = sr_val_set_xpath(&input[2], "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/rx-port");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/port-pairs[index='1']/rx-port", sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[3].type = SR_INT32_T;
    input[3].data.int32_val = iHoldOnTime;
    rc = sr_val_set_xpath(&input[3], "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/hold-on-time");
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports/hold-on-time", sr_strerror(rc));
        goto ERR_LABEL;
    }
    LOG_WRITE(EN_LOG_NOTICE, "DSC_LinkDiscoveyRpc");
    rc = DSC_RpcSend(sess, "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports", input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_LinkDiscoveyRpc Send: %s %s", "/com-alibaba-wavelength-router-ext:specify-connection-validation-ports", sr_strerror(rc));
        goto ERR_LABEL;
    }
    sr_free_values(input, input_cnt);
    return DSC_LinkDiscoveyRpcResp(output, output_cnt, stLinkDiscoveryRpcResp);
ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

static sr_subscription_ctx_t *pstLogLevelEventSub = NULL;

static void
__NotifyLogLevelCb(sr_session_ctx_t *session, uint32_t sub_id, const sr_ev_notif_type_t notif_type,
                   const char *xpath, const sr_val_t *values, const size_t values_cnt, struct timespec *timestamp, void *private_data)
{
    int rc = SR_ERR_OK;
    /* Parse the input parameter                                                */
    char acInputName[DEF_BUF_SIZE_64] = {0};
    char acSeverity[DEF_BUF_SIZE_64] = {0};
    uint8_t ucSeverity = 0;
    char acProcessName[DEF_BUF_SIZE_64] = {0};

    ANONYMOUS_ST astAnonymous[] = {{DEF_NOTIFY_LEAF_LOG_LEVEL_NAME, SR_STRING_T, acInputName, sizeof(acInputName) - 1},
                                   {DEF_NOTIFY_LEAF_LOG_LEVEL_SEVERITY, SR_ENUM_T, acSeverity, sizeof(acSeverity) - 1}};

    rc = DSC_ParseValuesByXpath(values, values_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Parse rpc parameters error: %d", rc);
        goto ERR_LABEL;
    }

    rc = DSC_OcSyslogSeverityToValue(acSeverity, (SYSLOG_SEVERITY_TYPE_EN *)&ucSeverity);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Parse rpc parameters error: %s", acSeverity);
        goto ERR_LABEL;
    }

    UTIL_GetProcName(acProcessName, sizeof(acProcessName));

    if (0 == strcasecmp(acInputName, "all") ||
        0 == strcasecmp(acInputName, acProcessName)) {
        /* Cause the 'SYSLOG_SEVERITY_TYPE_EN' and 'EN_UTIL_LOG_SEVERITY' is same. */
        LOG_WRITE(EN_LOG_NOTICE, "Set log level, name: %s, level: %s, process: %s", acInputName, acSeverity, acProcessName);
        UTIL_SetClientLogSeverity((EN_UTIL_LOG_SEVERITY)ucSeverity);
    }

    return;
ERR_LABEL:
    return;
}

int
DSC_SubLogLevel(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;

    rc = sr_notif_subscribe(pstSess, "com-oplink-system-logging", DEF_NOTIFY_XPATH_LOG_LEVEL,
                            0, 0, __NotifyLogLevelCb, NULL, SR_SUBSCR_DEFAULT, &pstLogLevelEventSub);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_event_notif_subscribe: '%s', %s\n", DEF_NOTIFY_XPATH_LOG_LEVEL, sr_strerror(rc));
        return rc;
    }

    return rc;
}

int
DSC_SetLogLevel(char *pcName, char *pcLogLevel)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL;
    size_t input_cnt = 2;

    if (NULL == pcName || NULL == pcLogLevel) {
        goto ERR_LABEL;
    }

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcName);

    rc = sr_val_set_xpath(&input[0], DEF_NOTIFY_LEAF_LOG_LEVEL_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_NOTIFY_LEAF_LOG_LEVEL_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[1].type = SR_ENUM_T;
    input[1].data.enum_val = strdup(pcLogLevel);
    rc = sr_val_set_xpath(&input[1], DEF_NOTIFY_LEAF_LOG_LEVEL_SEVERITY);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_NOTIFY_LEAF_LOG_LEVEL_SEVERITY, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = sr_notif_send(pstSess,
                       DEF_NOTIFY_XPATH_LOG_LEVEL,
                       input,
                       input_cnt, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation '%s' failed: %s", DEF_NOTIFY_XPATH_LOG_LEVEL, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (input != NULL) {
        sr_free_values(input, input_cnt);
        input = NULL;
    }
    return rc;

ERR_LABEL:
    if (input != NULL) {
        sr_free_values(input, input_cnt);
        input = NULL;
    }
    return rc;
}

