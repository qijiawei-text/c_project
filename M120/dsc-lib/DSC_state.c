#include "DSC_log.h"
#include "DSC_common.h"
#include "DSC_circuitpack.h"
#include "DSC_cod.h"
#include "DSC_shelf.h"
#include "DSC_state.h"
#include "DSC_edfa.h"
#include "DSC_ops.h"
#include "DSC_alarm.h"



LIST_KEY_INFO_ST stOpticalListKey[] = {
    {OPTICAL_EDFA_EN, "amplifier", "name"},
    {OPTICAL_VOA_EN, "evoas", "evoa-id"},
    {OPTICAL_OPS_EN, "aps-module", "name"}
};


static int
__AlmSeverityStrToEnum(char *pcSeverityStr, DSC_ALM_SEVERITY_TYPE_EN *penSeverityType)
{
    if (NULL == pcSeverityStr || NULL == penSeverityType) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        goto cleanup;
    }

    if (0 == strcmp(pcSeverityStr, ALARM_SEVERITY_STR_MAJOR)) {
        *penSeverityType = DSC_ALM_SEVERITY_MAJOR;
    } else if (0 == strcmp(pcSeverityStr, ALARM_SEVERITY_STR_CRITICAL)) {
        *penSeverityType = DSC_ALM_SEVERITY_CRITICAL;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "error __AlmSeverityStrToEnum unknow SeverityStr = %s", pcSeverityStr);
        goto cleanup;
    }

    return SR_ERR_OK;

cleanup:
    return SR_ERR_INVAL_ARG;
}

static int
__GetListNameAndKeyName(DSC_OPITICAL_TYEP_EN enType, char *pcListName, char *pcKeyName)
{
    int i = 0;
    int iOpticalNum = 0;

    if (NULL == pcListName || NULL == pcKeyName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        goto cleanup;
    }

    iOpticalNum = sizeof(stOpticalListKey) / sizeof(stOpticalListKey[0]);

    for (i = 0; i < iOpticalNum; i++) {
        if (stOpticalListKey[i].enOpiticalType == enType) {
            strcpy(pcListName, stOpticalListKey[i].acListName);
            strcpy(pcKeyName, stOpticalListKey[i].acKeyName);

            return SR_ERR_OK;
        }
    }

cleanup:
    return SR_ERR_INVAL_ARG;
}

static int
__GetListKeyValue(DSC_OPITICAL_TYEP_EN enType, char *pcXpath, char *pcKeyValue)
{
    char acListName[DSC_LIST_NAME_KEY_MAX_LEN] = {0};
    char acKeyName[DSC_LIST_NAME_KEY_MAX_LEN] = {0};
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acListName, 0, sizeof(acListName));
    memset(acKeyName, 0, sizeof(acKeyName));
    rc = __GetListNameAndKeyName(enType, acListName, acKeyName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __GetListNameAndKeyName enType : %d", enType);
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, acListName, acKeyName, DSC_LIST_NAME_KEY_MAX_LEN, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_GetKeyValue xpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

static void
__CP_CollectAlarmId(ALARM_INFO_ST *pstAlarmInfo, size_t nAlarmCnt, CIRCUIT_PACK_STATE_INFO_ST *pstCpState)
{
    int i = 0;
    int iIndex = 0;
    DSC_ALM_SEVERITY_TYPE_EN enSeverity = DSC_ALM_SEVERITY_CRITICAL;

    if (NULL == pstAlarmInfo || NULL == pstCpState) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return;
    }

    for (i = 0; i < nAlarmCnt; i++) {
        if (0 == strcmp(pstCpState->acCircuilPackName, pstAlarmInfo[i].acCircuitPack)) {
            strncpy(pstCpState->astAlarmState[iIndex].acAlarmId, pstAlarmInfo[i].acAlarmId, DSC_ALARM_STR_MAX_LEN - 1);
            __AlmSeverityStrToEnum(pstAlarmInfo[i].acSeverity, &enSeverity);
            pstCpState->astAlarmState[iIndex].enSeverity = enSeverity;
            iIndex++;
            if (DSC_ALARM_NUM_IN_CP_MAX == iIndex) {
                LOG_WRITE(EN_LOG_ERROR, "Error __CP_CollectAlarmId overflow name : %s", pstCpState->acCircuilPackName);
                break;
            }
        }
    }

    pstCpState->ulSupportAlarmNum = iIndex;

    return;
}

static int
__GetAlarmEnableByAdminState(char *pcAdminState, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    EN_EQPT_ADMIN_STATES enAdminState;

    if (NULL == pcAdminState || NULL == pbEnable) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_AdminStateStrToEnum(pcAdminState, &enAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_GetAlarmEnableByAdminState pcAdminState : %s", pcAdminState);
        goto cleanup;
    }

    switch (enAdminState) {
    case EQPT_ADMIN_STATES_INSERVICE:
        *pbEnable = true;
        break;
    case EQPT_ADMIN_STATES_OUTOFSERVICE:
    case EQPT_ADMIN_STATES_MAINTENANCE:
        *pbEnable = false;
        break;
    default:
        rc = SR_ERR_INVAL_ARG;
        break;
    }

cleanup:
    return rc;
}

static int
__CP_GetNameByAlarmId(char *pcAlarmId, char *pcCpName)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acName[DSC_CP_NAME_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcAlarmId || NULL == pcCpName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), ALARM_XPATH_RESOURCE_CP_NAME, pcAlarmId);

    memset(acName, 0, sizeof(acName));
    rc = get_string_item(pstSess, acXpath, acName, sizeof(acName) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pcCpName, acName);
    pcCpName[DSC_CP_NAME_MAX_LEN - 1] = '\0';

cleanup:
    return rc;
}

static int
__CP_GetAdminStateByAlarmId(char *pcAlarmId, char *pcAdminState)
{
    int rc = SR_ERR_OK;
    char acName[DSC_CP_NAME_MAX_LEN] = {0};

    if (NULL == pcAlarmId || NULL == pcAdminState) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acName, 0, sizeof(acName));
    rc = __CP_GetNameByAlarmId(pcAlarmId, acName);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__CP_GetNameByAlarmId failed alarm id : %s", pcAlarmId);
        goto cleanup;
    }

    rc = DSC_CP_GetAdminState(acName, pcAdminState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_GetAdminState failed acName : %s", acName);
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__CP_GetAlarmEnableByAlarmId(char *pcAlarmId, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    char acAdminState[DSC_STATE_STR_MAX_LEN] = {0};

    if (NULL == pcAlarmId || NULL == pbEnable) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acAdminState, 0, sizeof(acAdminState));
    rc = __CP_GetAdminStateByAlarmId(pcAlarmId, acAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__CP_GetAdminStateByAlarmId AlarmId : %s", pcAlarmId);
        goto cleanup;
    }

    rc = __GetAlarmEnableByAdminState(acAdminState, pbEnable);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_GetAlarmEnableByAdminState acAdminState : %s", acAdminState);
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__CP_GetNameByOpticalIdx(DSC_OPITICAL_TYEP_EN enType, char *pcXpath, char *pcCpName)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acName[DSC_CP_NAME_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acKeyValue[DSC_LIST_NAME_KEY_MAX_LEN] = {0};

    if (NULL == pcCpName || NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acKeyValue, 0, sizeof(acKeyValue));
    rc = __GetListKeyValue(enType, pcXpath, acKeyValue);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error __GetListKeyValue enType: %d, pcXpath: %s", enType, pcXpath);
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    switch (enType) {
    case OPTICAL_EDFA_EN:
        snprintf(acXpath, sizeof(acXpath), EDFA_CFG_PARENT_CP_PATH, acKeyValue);
        break;
    case OPTICAL_VOA_EN:
        snprintf(acXpath, sizeof(acXpath), EDFA_CFG_PARENT_CP_PATH, acKeyValue);
        break;
    case OPTICAL_OPS_EN:
        snprintf(acXpath, sizeof(acXpath), OPS_CFG_PARENT_CP_PATH, acKeyValue);
        break;
    default:
        break;
    }

    memset(acName, 0, sizeof(acName));
    rc = get_string_item(pstSess, acXpath, acName, sizeof(acName) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strncpy(pcCpName, acName, sizeof(acName) - 1);

cleanup:
    return rc;
}

static int
__CP_GetAdminStateByOpticalIdx(DSC_OPITICAL_TYEP_EN enType, char *pcXpath, char *pcAdminState)
{
    int rc = SR_ERR_OK;
    char acName[DSC_CP_NAME_MAX_LEN] = {0};

    if (NULL == pcAdminState || NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acName, 0, sizeof(acName));
    rc = __CP_GetNameByOpticalIdx(enType, pcXpath, acName);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__CP_GetNameByOpticalIdx failed enType : %d", enType);
        goto cleanup;
    }

    rc = DSC_CP_GetAdminState(acName, pcAdminState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_GetAdminState failed acName : %s", acName);
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__CP_GetConfEnableByAdminState(char *pcAdminState, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    EN_EQPT_ADMIN_STATES enAdminState;

    if (NULL == pcAdminState || NULL == pbEnable) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_AdminStateStrToEnum(pcAdminState, &enAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_GetAlarmEnableByAdminState pcAdminState : %s", pcAdminState);
        goto cleanup;
    }

    switch (enAdminState) {
    case EQPT_ADMIN_STATES_INSERVICE:
    case EQPT_ADMIN_STATES_MAINTENANCE:
        *pbEnable = true;
        break;
    case EQPT_ADMIN_STATES_OUTOFSERVICE:
        *pbEnable = false;
        break;
    default:
        rc = SR_ERR_INVAL_ARG;
        break;
    }

cleanup:
    return rc;
}

char *
DSC_AdminStateEnumToStr(EN_EQPT_ADMIN_STATES enAdminState, char *pcBuf)
{

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        return NULL;
    }

    switch (enAdminState) {
    case EQPT_ADMIN_STATES_INSERVICE:
        strcpy(pcBuf, ADMIN_STATE_INSERVICE_STR);
        break;
    case EQPT_ADMIN_STATES_OUTOFSERVICE:
        strcpy(pcBuf, ADMIN_STATE_OUTOFSERVICE_STR);
        break;
    case EQPT_ADMIN_STATES_MAINTENANCE:
        strcpy(pcBuf, ADMIN_STATE_MAINTENANCE_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

int
DSC_AdminStateStrToEnum(char *pcStateString, EN_EQPT_ADMIN_STATES *penAdminState)
{
    int rc = SR_ERR_OK;

    if (NULL == penAdminState || NULL == pcStateString) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strcmp(pcStateString, ADMIN_STATE_INSERVICE_STR)) {
        *penAdminState = EQPT_ADMIN_STATES_INSERVICE;
    } else if (0 == strcmp(pcStateString, ADMIN_STATE_OUTOFSERVICE_STR)) {
        *penAdminState = EQPT_ADMIN_STATES_OUTOFSERVICE;
    } else if (0 == strcmp(pcStateString, ADMIN_STATE_MAINTENANCE_STR)) {
        *penAdminState = EQPT_ADMIN_STATES_MAINTENANCE;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AdminStateStrToEnum input arg error!");
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

char *
DSC_OpStateEnumToStr(EN_STATE enOpState, char *pcBuf)
{

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        return NULL;
    }

    switch (enOpState) {
    case STATE_INSERVICE:
        strcpy(pcBuf, OPERATIONAL_STATE_INSERVICE_STR);
        break;
    case STATE_OUTOFSERVICE:
        strcpy(pcBuf, OPERATIONAL_STATE_OUTOFSERVICE_STR);
        break;
    case STATE_DEGRADED:
        strcpy(pcBuf, OPERATIONAL_STATE_DEGRADED_STR);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "error enOpState unknow type!");
        return NULL;
    }

    return pcBuf;
}

int
DSC_OpStateStrToEnum(char *pcStateString, EN_STATE *penOpState)
{
    int rc = SR_ERR_OK;

    if (NULL == penOpState || NULL == pcStateString) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strcmp(pcStateString, OPERATIONAL_STATE_INSERVICE_STR)) {
        *penOpState = STATE_INSERVICE;
    } else if (0 == strcmp(pcStateString, OPERATIONAL_STATE_OUTOFSERVICE_STR)) {
        *penOpState = STATE_OUTOFSERVICE;
    } else if (0 == strcmp(pcStateString, OPERATIONAL_STATE_DEGRADED_STR)) {
        *penOpState = STATE_DEGRADED;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "DSC_OpStateStrToEnum input arg error!");
        rc = SR_ERR_INVAL_ARG;
    }

    return rc;
}

int
DSC_SHELF_GetShelfName(char *pcName)
{
    int rc = SR_ERR_OK;
    char acName[DSC_SHELF_NAME_MAX_LEN] = {0};

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acName, 0, sizeof(acName));
    rc = DSC_SHELF_GetName(acName, sizeof(acName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SHELF_GetName error!");
        goto cleanup;
    }

    strcpy(pcName, acName);

cleanup:
    return rc;
}

int
DSC_SHELF_GetAdminStateStr(char *pcName, char *pcState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acState[DSC_STATE_STR_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcName || NULL == pcState) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acState, 0, sizeof(acState));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_SHELF_ADMIN_STATE, pcName);
    rc = get_enum_item(pstSess, acXpath, acState, sizeof(acState) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pcState, acState);

cleanup:
    return rc;
}

int
DSC_SHELF_GetAdminStateEnum(EN_EQPT_ADMIN_STATES *penAdminState)
{
    char acShelfNameBuff[DSC_SHELF_NAME_MAX_LEN] = {0};
    char acShelfAdminBuff[DSC_STATE_STR_MAX_LEN] = {0};
    int rc = OPLK_OK;

    if (NULL == penAdminState) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acShelfNameBuff, 0, sizeof(acShelfNameBuff));
    memset(acShelfAdminBuff, 0, sizeof(acShelfAdminBuff));
    rc = DSC_SHELF_GetName(acShelfNameBuff, sizeof(acShelfNameBuff));
    if (SR_ERR_OK != rc || 0 == strlen(acShelfNameBuff)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", SHELF_DEFAULT, acShelfNameBuff);
        goto cleanup;
    }
    rc = DSC_SHELF_GetAdminStateStr(acShelfNameBuff, acShelfAdminBuff);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetAdminStateStr name: %s ", acShelfNameBuff);
        goto cleanup;
    }

    rc = DSC_AdminStateStrToEnum(acShelfAdminBuff, penAdminState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_AdminStateStrToEnum acShelfAdminBuff: %s ", acShelfAdminBuff);
    }

cleanup:
    return rc;
}

int
DSC_SHELF_SetAdminState(char *pcName, EN_EQPT_ADMIN_STATES enState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acAdminStateBuff[DSC_STATE_STR_MAX_LEN] = {0};

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acAdminStateBuff, 0, sizeof(acAdminStateBuff));
    DSC_AdminStateEnumToStr(enState, acAdminStateBuff);
    if (0 == strlen(acAdminStateBuff)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AdminStateEnumToStr error, state:%d", enState);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_SHELF_ADMIN_STATE, pcName);

    rc = set_enum_item(pstSess, acXpath, acAdminStateBuff);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_enum_item: %s %s %s", acXpath, acAdminStateBuff, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SHELF_SetAdminState DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_SHELF_GetOpState(char *pcName, char *pcState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acState[DSC_STATE_STR_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcName || NULL == pcState) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acState, 0, sizeof(acState));
    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_SHELF_OPRATNAL_STATE, pcName);
    rc = get_enum_item(pstSess, acXpath, acState, sizeof(acState) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pcState, acState);

cleanup:
    return rc;
}

int
DSC_SHELF_SetOpState(char *pcName, uint32_t ulOpState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acState[DSC_STATE_STR_MAX_LEN] = {0};

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acState, 0, sizeof(acState));
    if (NULL == DSC_OpStateEnumToStr(ulOpState, acState)) {
        LOG_WRITE(EN_LOG_ERROR, "ulOpState error : %d", ulOpState);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_SHELF_OPRATNAL_STATE, pcName);

    rc = set_enum_item(pstSess, acXpath, acState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SHELF_SetOpState DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_CP_GetListKeyValue(char *pcXpath, char *pcKeyValue)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_GetKeyValue(pcXpath, "circuit-packs", "circuit-pack-name", DSC_LIST_NAME_KEY_MAX_LEN, pcKeyValue);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetKeyValue pcXpath : %s", pcXpath);
    }

cleanup:
    return rc;
}

int
DSC_CP_GetCpName(char aacName[][DSC_CP_NAME_MAX_LEN], size_t *pnCount)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

    if (NULL == pnCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = get_items(DSC_GetOperSession(), OOD_XPATH_LIST_CIRCUIT_PACKS_ALL, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", OOD_XPATH_LIST_CIRCUIT_PACKS_ALL, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_CP_NAME)) {
            j++;
            if (j >= CIRCUIT_PACK_NUM_MAX) {
                rc = SR_ERR_INVAL_ARG;
                goto cleanup;
            }
            strncpy(aacName[j], pstValues[i].data.string_val, DSC_CP_NAME_MAX_LEN - 1);
        } else {
        }
    }

    *pnCount = j + 1;

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_CP_GetAdminState(char *pcName, char *pcState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acState[DSC_STATE_STR_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcName || NULL == pcState) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    memset(acState, 0, sizeof(acState));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_ADMIN_STATE, pcName);
    rc = get_enum_item(pstSess, acXpath, acState, sizeof(acState) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pcState, acState);

cleanup:
    return rc;
}

int
DSC_CP_GetServiceAffect(char *pcName, bool *pbAffect)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    bool bServiceAffect = false;

    if (NULL == pcName || NULL == pbAffect) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
#ifdef __OPER__
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_SERVICE_AFFECT, pcName);
#else
    snprintf(acXpath, sizeof(acXpath), COD_XPATH_CIRCUIT_PACK_SERVICE_AFFECT, pcName);
#endif

    rc = get_bool_item(pstSess, acXpath, &bServiceAffect);
    if (SR_ERR_NOT_FOUND == rc) {
        rc = SR_ERR_OK;
        *pbAffect = false;
        goto cleanup;
    }
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_bool_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    *pbAffect = bServiceAffect;

cleanup:
    return rc;
}

int
DSC_CP_SetAdminState(char *pcName, EN_EQPT_ADMIN_STATES enState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acAdminStateBuff[DSC_STATE_STR_MAX_LEN] = {0};

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acAdminStateBuff, 0, sizeof(acAdminStateBuff));
    DSC_AdminStateEnumToStr(enState, acAdminStateBuff);
    if (0 == strlen(acAdminStateBuff)) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AdminStateEnumToStr error, state:%d", enState);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_ADMIN_STATE, pcName);
    rc = set_enum_item(pstSess, acXpath, acAdminStateBuff);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_enum_item: %s %s %s", acXpath, acAdminStateBuff, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_SetAdminState DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_CP_GetOpState(char *pcName, char *pcState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acState[DSC_STATE_STR_MAX_LEN] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcName || NULL == pcState) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_OPRATNAL_STATE, pcName);

    memset(acState, 0, sizeof(acState));
    rc = get_enum_item(pstSess, acXpath, acState, sizeof(acState) - 1);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pcState, acState);

cleanup:
    return rc;
}

int
DSC_CP_SetOpState(char *pcName, uint32_t ulOpState)
{
    int rc = SR_ERR_OK;
#ifdef __OPER__
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
#else
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
#endif
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acState[DSC_STATE_STR_MAX_LEN] = {0};

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acState, 0, sizeof(acState));
    if (NULL == DSC_OpStateEnumToStr(ulOpState, acState)) {
        LOG_WRITE(EN_LOG_ERROR, "ulOpState error : %d", ulOpState);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_OPRATNAL_STATE, pcName);

    rc = set_enum_item(pstSess, acXpath, acState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_enum_item: %s %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_CP_SetOpState_I DSC_Commit error %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_CP_GetAllCpInfo(ST_CIRCUIT_PACK *pstCircuitPackInfo, size_t *ptCircuitPackCount)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

    if (NULL == pstCircuitPackInfo || NULL == ptCircuitPackCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = get_items(DSC_GetOperSession(), OOD_XPATH_LIST_CIRCUIT_PACKS_ALL, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", OOD_XPATH_LIST_CIRCUIT_PACKS_ALL, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_CP_NAME)) {
            j++;
            if (j >= CIRCUIT_PACK_NUM_MAX) {
                rc = SR_ERR_INVAL_ARG;
                goto cleanup;
            }
            strncpy(pstCircuitPackInfo[j].acName, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].acName) - 1);
        } else if ((NULL != strstr(pstValues[i].xpath, LEAF_CP_ADMIN_STATE))
                   && (NULL == strstr(pstValues[i].xpath, LEAF_CP_PORT_ADMIN_STATE))) {
            DSC_AdminStateStrToEnum(pstValues[i].data.enum_val, &pstCircuitPackInfo[j].enEqptAdminState);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_OPRATNAL_STATE)) {
            DSC_OpStateStrToEnum(pstValues[i].data.enum_val, &pstCircuitPackInfo[j].stPhysical.stCommon.enOperationalState);
        } else {
        }
    }

    *ptCircuitPackCount = j + 1;

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_GetConfEnable(DSC_OPITICAL_TYEP_EN enType, char *pcXpath, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    EN_EQPT_ADMIN_STATES enAdminState;
    char acAdminState[DSC_STATE_STR_MAX_LEN] = {0};

    return rc;

    if (NULL == pbEnable || NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_SHELF_GetAdminStateEnum(&enAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetAdminStateEnum failed.");
        goto cleanup;
    }

    if (EQPT_ADMIN_STATES_OUTOFSERVICE == enAdminState) {
        pbEnable = false;
        goto cleanup;
    }


    memset(acAdminState, 0, sizeof(acAdminState));
    rc = __CP_GetAdminStateByOpticalIdx(enType, pcXpath, acAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__CP_GetAdminStateByOpticalIdx enType : %d", enType);
        goto cleanup;
    }

    rc = __CP_GetConfEnableByAdminState(acAdminState, pbEnable);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__CP_GetConfEnableByAdminState acAdminState : %s", acAdminState);
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_GetAlarmEnable(char *pcAlarmIdStr, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    EN_EQPT_ADMIN_STATES enAdminState;

    if (NULL == pbEnable || NULL == pcAlarmIdStr) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_SHELF_GetAdminStateEnum(&enAdminState);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SHELF_GetAdminStateEnum failed.");
        goto cleanup;
    }

    if (EQPT_ADMIN_STATES_OUTOFSERVICE == enAdminState || EQPT_ADMIN_STATES_MAINTENANCE == enAdminState) {
        pbEnable = false;
        goto cleanup;
    }

    rc = __CP_GetAlarmEnableByAlarmId(pcAlarmIdStr, pbEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error __CP_GetAlarmEnableByAlarmId alarm str : %s", pcAlarmIdStr);
    }

cleanup:
    return rc;
}

int
DSC_GetSystemStateInfo(SYSTEM_STATE_INFO_ST *pstSystemStateInfo)
{
#define ALARM_ENTRY_MAX_NUM (160)

    int rc = SR_ERR_OK;
    char aacCircuitPackName[DSC_CP_NUM_MAX][DSC_CP_NAME_MAX_LEN];
    size_t nCnt = 0;
    int i = 0;
    char acShelfName[DSC_SHELF_NAME_MAX_LEN];
    char acState[DSC_STATE_STR_MAX_LEN];
    ALARM_INFO_ST astAlarm[ALARM_ENTRY_MAX_NUM];
    size_t nAlarmCnt = 0;
    bool bServiceAffect = false;

    if (NULL == pstSystemStateInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /* 1.get support alarm list info */
    memset(astAlarm, 0, sizeof(astAlarm));
    rc = DSC_GetSupportAlarmList(astAlarm, &nAlarmCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetSupportAlarmList: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* 2.get shelf name */
    memset(acShelfName, 0, sizeof(acShelfName));
    rc = DSC_SHELF_GetShelfName(acShelfName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __GetShelfName: %s", sr_strerror(rc));
        goto cleanup;
    }
    strncpy(pstSystemStateInfo->stShelfState.acShelfName, acShelfName, DSC_SHELF_NAME_MAX_LEN - 1);
    pstSystemStateInfo->stShelfState.acShelfName[DSC_SHELF_NAME_MAX_LEN - 1] = '\0';

    /* __SHELF_CollectAlarmId(astAlarm, nAlarmCnt, &pstSystemStateInfo->stShelfState); */
    /* 3.get shelf admin state */
    memset(acState, 0, sizeof(acState));
    rc = DSC_SHELF_GetAdminStateStr(pstSystemStateInfo->stShelfState.acShelfName, acState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SHELF_GetAdminStateStr: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* 4.change the str of shelf admin state to enum */
    rc = DSC_AdminStateStrToEnum(acState, &pstSystemStateInfo->stShelfState.ulAdminState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_AdminStateStrToEnum acState : %s", acState);
        goto cleanup;
    }

    /* 5.get all cp names */
    memset(aacCircuitPackName, 0, sizeof(aacCircuitPackName));
    rc = DSC_CP_GetCpName(aacCircuitPackName, &nCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetCpName: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* 6.get every cp info */
    for (i = 0; i < nCnt; i++) {
        strncpy(pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName, aacCircuitPackName[i], DSC_CP_NAME_MAX_LEN - 1);
        LOG_WRITE(EN_LOG_DEBUG, "DSC_GetSystemStateInfo CP name: %s", pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName);

        memset(acState, 0, sizeof(acState));
        rc = DSC_CP_GetAdminState(pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName, acState);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetAdminState: %s", sr_strerror(rc));
            goto cleanup;
        }
        LOG_WRITE(EN_LOG_DEBUG, "DSC_GetSystemStateInfo CP admin name : %s, state : %s", pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName, acState);

        rc = DSC_AdminStateStrToEnum(acState, &pstSystemStateInfo->astCircuitPackState[i].ulAdminState);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error DSC_AdminStateStrToEnum acState : %s", acState);
            goto cleanup;
        }

        rc = DSC_CP_GetServiceAffect(pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName, &bServiceAffect);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetServiceAffect cp name : %s", pstSystemStateInfo->astCircuitPackState[i].acCircuilPackName);
            goto cleanup;
        }

        pstSystemStateInfo->astCircuitPackState[i].bServiceAffect = bServiceAffect;

        __CP_CollectAlarmId(astAlarm, nAlarmCnt, &pstSystemStateInfo->astCircuitPackState[i]);
    }

    /* 7.get cp number */
    pstSystemStateInfo->ulCircuitPackNum = nCnt;
    LOG_WRITE(EN_LOG_DEBUG, "DSC_GetSystemStateInfo ulCircuitPackNum: %d", pstSystemStateInfo->ulCircuitPackNum);

cleanup:
    return rc;
}

bool
DSC_IsCircuitPackInService(const char *pcCpName)
{
    char acState[DSC_STATE_STR_MAX_LEN] = {0};
    int rc = SR_ERR_OK;

    rc = DSC_CP_GetAdminState((char *)pcCpName, acState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetAdminState acState : %s", acState);
        return false;
    }

    if (0 == strcmp(acState, ADMIN_STATE_INSERVICE_STR)) {
        return true;
    } else {
        return false;
    }
}

