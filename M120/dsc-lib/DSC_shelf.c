#include "DSC_log.h"
#include "DSC_common.h"
#include "DSC_shelf.h"
#include "DSC_cod.h"

static ST_SHELF m_astShelf[SHELF_MAX];

static int
__InitGlobal()
{
    for (size_t i = 0; i < SHELF_MAX; i++) {
        memset(&m_astShelf[i], 0x00, sizeof(ST_SHELF));
    }
    return SR_ERR_OK;
}

static int
__UpdateName(char *pcXpath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstVal = NULL;
    size_t tValCnt = 0;
    int j = -1;

    if (NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    __InitGlobal();

    pstSess = DSC_GetOperSession();
    rc = sr_get_items(pstSess, pcXpath, 0, SR_OPER_NO_SUBS, &pstVal, &tValCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s, %s", pcXpath, strerror(rc));
        goto cleanup;
    }

    for (size_t i = 0; i < tValCnt; i++) {
        if (NULL != strstr(pstVal[i].xpath, LEAF_SHELF_NAME)) {
            j++;
            if (j >= SHELF_MAX) {
                goto cleanup;
            }
            strncpy(m_astShelf[j].acName, pstVal[i].data.string_val, sizeof(m_astShelf[j].acName) - 1);
        }
    }

cleanup:

    sr_free_values(pstVal, tValCnt);
    return rc;
}

int
__GetNameById(EN_SHELF enShelf, char *pcName, size_t tNameMaxLen)
{
    int rc = SR_ERR_OK;
    static bool m_bIsRead = false;

    if (enShelf >= SHELF_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "enShelf: %d out of range: %d", enShelf, SHELF_MAX);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (NULL == pcName) {
        LOG_WRITE(EN_LOG_ERROR, "pcName is null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (false == m_bIsRead) {
        rc = __UpdateName(OOD_XPATH_SHELF_NAME);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "__SHELF_UpdateName: %s", strerror(rc));
            goto cleanup;
        }
        m_bIsRead = true;
    }

    memset(pcName, 0x00, tNameMaxLen);
    strncpy(pcName, m_astShelf[enShelf].acName, tNameMaxLen);

cleanup:
    return rc;
}


int
DSC_SHELF_GetName(char *pcName, size_t tNameMaxLen)
{
    return __GetNameById(SHELF_DEFAULT, pcName, tNameMaxLen);
}

int
DSC_SHELF_GetNameFromStartup(char *pcName, size_t nLen)
{

    int rc = SR_ERR_OK;
    static char m_acShelfName[DEF_BUF_SIZE_32] = {0};
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstVal = NULL;
    size_t nValCnt = 0;
    int j = 0;

    if (NULL == pcName || nLen < 1) {
        return SR_ERR_INVAL_ARG;
    }

    if (strlen(m_acShelfName) > 0) {
        snprintf(pcName, nLen, "%s", m_acShelfName);
        return rc;
    }

    pstSess = DSC_GetStartupSession();
    rc = get_items(pstSess, OOD_XPATH_SHELF_NAME, &pstVal, &nValCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s, %s", OOD_XPATH_SHELF_NAME, strerror(rc));
        goto cleanup;
    }

    for (size_t i = 0; i < nValCnt; i++) {
        if (NULL != strstr(pstVal[i].xpath, LEAF_SHELF_NAME)) {
            j++;
            if (j > SHELF_MAX) {
                LOG_WRITE(EN_LOG_ERROR, "more than one shelf! m_acShelfName: %s, others: %s", m_acShelfName, pstVal[i].data.string_val);
                goto cleanup;
            }
            strncpy(m_acShelfName, pstVal[i].data.string_val, sizeof(m_acShelfName) - 1);
            snprintf(pcName, nLen, "%s", m_acShelfName);
        }
    }

cleanup:

    sr_free_values(pstVal, nValCnt);

    return rc;
}


static int
__GetInfo(EN_SHELF enShelf, ST_SHELF *pstShelf)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};

    rc = __GetNameById(enShelf, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", enShelf, acName);
        return SR_ERR_INVAL_ARG;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OOD_XPATH_SHELF_TYPE, acName), SR_STRING_T, pstShelf->acType),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OOD_XPATH_SHELF_RACK, acName), SR_STRING_T, pstShelf->acRack),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OOD_XPATH_SHELF_POSITION, acName), SR_STRING_T, pstShelf->acPosition),
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OOD_XPATH_SHELF_LIFECYCLE, acName), SR_STRING_T, pstShelf->enLifeCycleState), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OOD_XPATH_SHELF_ADMIN_STATE, acName), SR_STRING_T, pstShelf->enEqptAdminState), */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], OOD_XPATH_SHELF_VENDOR, acName), SR_STRING_T, pstShelf->stPhysical.stNode.acVendor),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], OOD_XPATH_SHELF_MODEL, acName), SR_STRING_T, pstShelf->stPhysical.stNode.acModel),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], OOD_XPATH_SHELF_SERIAL_ID, acName), SR_STRING_T, pstShelf->stPhysical.stNode.acSN),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], OOD_XPATH_SHELF_COMMON_TYPE, acName), SR_STRING_T, pstShelf->stPhysical.stCommon.acType),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[9], OOD_XPATH_SHELF_PRODUCT_CODE, acName), SR_STRING_T, pstShelf->stPhysical.stCommon.acProductCode),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[10], OOD_XPATH_SHELF_MFG_DATE, acName), SR_STRING_T, pstShelf->stPhysical.stCommon.acMfgDate),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[11], OOD_XPATH_SHELF_CLEI, acName), SR_STRING_T, pstShelf->stPhysical.stCommon.acClei),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[12], OOD_XPATH_SHELF_HW_VER, acName), SR_STRING_T, pstShelf->stPhysical.stCommon.acHwVer),
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[13], OOD_XPATH_SHELF_OPRATNAL_STATE, acName), SR_STRING_T, pstShelf->stPhysicalCommonInfo.enOperationalState), */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[14], OOD_XPATH_SHELF_IS_PHYSICAL, acName), SR_BOOL_T, (char *)(&pstShelf->bIsPhysical)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], OOD_XPATH_SHELF_IS_PASSIVE, acName), SR_BOOL_T, (char *)(&pstShelf->bIsPassive)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[16], OOD_XPATH_SHELF_FACEPTE_LABEL, acName), SR_STRING_T, pstShelf->acFacePlateLabel),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[17], OOD_XPATH_SHELF_DUE_DATE, acName), SR_STRING_T, pstShelf->acDueDate),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[18], OOD_XPATH_SHELF_UNIT_NAME, acName), SR_STRING_T, pstShelf->stPhysical.stFujitsu.acUnitName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[19], OOD_XPATH_SHELF_FC_NUMBER, acName), SR_STRING_T, pstShelf->stPhysical.stFujitsu.acFcNumber),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[20], OOD_XPATH_SHELF_PIU_ISSUE_NUMBER, acName), SR_STRING_T, pstShelf->stPhysical.stFujitsu.acPiuIssueNumber),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[21], OOD_XPATH_SHELF_FUJITSU_PART_NUMBER, acName), SR_STRING_T, pstShelf->stPhysical.stFujitsu.acFujitsuPartNumber)
    };

    pstSess = DSC_GetOperSession();

    if (enShelf >= SHELF_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "enShelf: %d out of range: %d", enShelf, SHELF_MAX);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (NULL == pstShelf) {
        LOG_WRITE(EN_LOG_ERROR, "pstShelf is null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset((void *)pstShelf, 0x00, sizeof(ST_SHELF));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_ALL, acName);
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", OOD_XPATH_SHELF_ALL, sr_strerror(rc));
        return rc;
    }

    strncpy(pstShelf->acName, acName, sizeof(acName) - 1);
    pstShelf->acName[sizeof(pstShelf->acName) - 1] = '\0';

cleanup:
    return rc;
}

int
DSC_SHELF_GetInfo(ST_SHELF *pstShelf)
{
    return __GetInfo(SHELF_DEFAULT, pstShelf);
}

static int
__GetSlotCount(char *pcShelfName, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcShelfName || NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SLOT_NAME, pcShelfName);

    rc = sr_get_items(pstSess, acXpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    *ptCount = tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetSlots(EN_SHELF enShelf, ST_SLOT **ppstSlot, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;
    int k = 0;
    ST_SLOT *pstSlot = NULL;

    if (NULL == ppstSlot || NULL == ptCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    *ppstSlot = NULL;
    *ptCount = 0;

    rc = __GetNameById(enShelf, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", enShelf, acName);
        goto cleanup;
    }

    rc = __GetSlotCount(acName, ptCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot count error, name: %s, error: %s", acName, sr_strerror(rc));
        goto cleanup;
    }

    pstSlot = (ST_SLOT *)malloc(sizeof(ST_SLOT) * (*ptCount));
    if (NULL == pstSlot) {
        LOG_WRITE(EN_LOG_ERROR, "Malloc slot info error");
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstSlot, 0x00, sizeof(ST_SLOT) * (*ptCount));

    memset(xpath, 0x00, sizeof(xpath));
    snprintf(xpath, OPLK_PATH_MAX_LEN, OOD_XPATH_LIST_SHELVE_SLOTS_ALL, acName);

    pstSess = DSC_GetOperSession();
    rc = sr_get_items(pstSess, xpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", xpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_SLOT_NAME)) {
            j++;
            k = 0;
            if (j >= SHELF_SLOT_NUM_MAX) {
                goto cleanup;
            }
            strncpy(pstSlot[j].acName, pstValues[i].data.string_val, sizeof(pstSlot[j].acName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_SLOT_LABEL)) {
            strncpy(pstSlot[j].acLabel, pstValues[i].data.string_val, sizeof(pstSlot[j].acLabel) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_SLOT_PRVSN_CIRCUIT_PAC)) {
            strncpy(pstSlot[j].aacRefProvisionedCircuitPack[k], pstValues[i].data.string_val, sizeof(pstSlot[j].aacRefProvisionedCircuitPack[k]) - 1);
            k++;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_SLOT_STATUS)) {
            strncpy(pstSlot[j].acSlotStatus, pstValues[i].data.enum_val, sizeof(pstSlot[j].acSlotStatus) - 1);
        }
    }

    *ppstSlot = pstSlot;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_SHELF_GetSlots(ST_SLOT **ppstSlot, size_t *ptCount)
{
    return __GetSlots(SHELF_DEFAULT, ppstSlot, ptCount);
}

int
DSC_SHELF_GetSlotsNameById(int iSlotId, char *pcSlotName)
{
    ST_SLOT *pstSlot = NULL;
    size_t nSlotNum = 0;
    char acSlotLabel[8] = {0};
    int i = 0;
    int rc = SR_ERR_OK;

    memset(acSlotLabel, 0, sizeof(acSlotLabel));
    sprintf(acSlotLabel, "%d", iSlotId);

    rc = DSC_SHELF_GetSlots(&pstSlot, &nSlotNum);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetSlots!");
        goto cleanup;
    }

    for (i = 0; i < nSlotNum; i++) {
        if (0 == strcmp(acSlotLabel, pstSlot[i].acLabel)) {
            strcpy(pcSlotName, pstSlot[i].acName);
            DEF_SAFE_FREE(pstSlot);
            return rc;
        }
    }

    rc = SR_ERR_NOT_FOUND;
cleanup:
    DEF_SAFE_FREE(pstSlot);
    return rc;
}

int
DSC_SHELF_GetSlotsIdByName(char *pcSlotName, int *piSlotId)
{
    ST_SLOT *pstSlot = NULL;
    size_t nSlotNum = 0;
    char acSlotLabel[8] = {0};
    int i = 0;
    int rc = SR_ERR_OK;

    memset(acSlotLabel, 0, sizeof(acSlotLabel));

    rc = DSC_SHELF_GetSlots(&pstSlot, &nSlotNum);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetSlots_I!");
        goto cleanup;
    }

    for (i = 0; i < nSlotNum; i++) {
        if (0 == strcmp(pcSlotName, pstSlot[i].acName)) {
            strcpy(acSlotLabel, pstSlot[i].acLabel);
            *piSlotId = atoi(acSlotLabel);
            DEF_SAFE_FREE(pstSlot);
            return rc;
        }
    }

    rc = SR_ERR_NOT_FOUND;
cleanup:
    DEF_SAFE_FREE(pstSlot);
    return rc;
}

int
__SetSlot(char *pcSlotName, char *pcSlotStatusStr)
{
    int rc = SR_ERR_OK;
    char acShelfName[32] = {0};
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *session = DSC_GetOperSession();

    if (NULL == pcSlotName ||
        0 == strlen(pcSlotName)) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acShelfName, 0x00, sizeof(acShelfName));
    rc = __GetNameById(SHELF_DEFAULT, acShelfName, sizeof(acShelfName));
    if (SR_ERR_OK != rc || 0 == strlen(acShelfName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf name: %s error", acShelfName);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_SLOT_STATUS, acShelfName, pcSlotName);
    rc = set_enum_item(session, acXpath, pcSlotStatusStr);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set xpath:%s, value: %s error: %s", acXpath, pcSlotStatusStr, sr_strerror(rc));
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = DSC_Commit(session);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_SHELF_SetSlotFanStatus(int iIndex, char *pcSlotStatusStr)
{
    char acSlotName[32] = {0};

    memset(acSlotName, 0x00, sizeof(acSlotName));
    snprintf(acSlotName, sizeof(acSlotName), "%s%d", SLOT_KEY_FAN, iIndex);

    return __SetSlot(acSlotName, pcSlotStatusStr);
}

int
DSC_SHELF_SetSlotPowerStatus(int iIndex, char *pcSlotStatusStr)
{
    char acSlotName[32] = {0};

    memset(acSlotName, 0x00, sizeof(acSlotName));
    snprintf(acSlotName, sizeof(acSlotName), "%s%d", SLOT_KEY_POWER, iIndex);

    return __SetSlot(acSlotName, pcSlotStatusStr);
}

int
DSC_SHELF_SetSlotModuleStatus(char *pcSlotName, char *pcSlotStatus)
{
    return __SetSlot(pcSlotName, pcSlotStatus);
}

int
DSC_SHELF_ResetCauseEnumToString(EN_COMMON_INFO_RESET_CAUSE enResetCause, char *pcString, size_t nLen)
{
    if (NULL == pcString) {
        return SR_ERR_INVAL_ARG;
    }

    switch (enResetCause) {
    case EN_COMMON_INFO_RESET_CAUSE_POWER:
        strncpy(pcString, EN_COMMON_INFO_RESET_CAUSE_POWER_STR, nLen - 1);
        break;
    case EN_COMMON_INFO_RESET_CAUSE_WARM:
        strncpy(pcString, EN_COMMON_INFO_RESET_CAUSE_WARM_STR, nLen - 1);
        break;
    case EN_COMMON_INFO_RESET_CAUSE_COLD:
        strncpy(pcString, EN_COMMON_INFO_RESET_CAUSE_COLD_STR, nLen - 1);
        break;
    case EN_COMMON_INFO_RESET_CAUSE_WATCHDOG:
        strncpy(pcString, EN_COMMON_INFO_RESET_CAUSE_WATCHDOG_STR, nLen - 1);
        break;
    case EN_COMMON_INFO_RESET_CAUSE_BUTTON:
        strncpy(pcString, EN_COMMON_INFO_RESET_CAUSE_BUTTON_STR, nLen - 1);
        break;
    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

/*TODO: test operational datastore */
int
DSC_SHELF_SetResetCause(uint8_t ucRestCause)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};
    char acResetCause[32] = {0};
    sr_session_ctx_t *session = DSC_GetOperSession();

    memset(acResetCause, 0x00, sizeof(acResetCause));
    rc = DSC_SHELF_ResetCauseEnumToString(ucRestCause, acResetCause, sizeof(acResetCause));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_ResetCauseEnumToString ucRestCause = %d", ucRestCause);
        goto cleanup;
    }

    rc = __GetNameById(SHELF_DEFAULT, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", SHELF_DEFAULT, acName);
        goto cleanup;
    }

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_RESET_CAUSE, acName);
    rc = set_enum_item(session, acSelectXpath, acResetCause);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set_enum_item: %s, %s", acSelectXpath, sr_strerror(rc));
    }

    rc = DSC_Commit(session);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_SHELF_GetResetCause(char *pcBuf, size_t nLen)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};

    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "pcBuf is null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetNameById(SHELF_DEFAULT, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", SHELF_DEFAULT, acName);
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_RESET_CAUSE, acName);
    rc = get_enum_item(pstSess, acSelectXpath, pcBuf, nLen);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_enum_item: %s, %s", acSelectXpath, sr_strerror(rc));
        return rc;
    }

cleanup:
    return rc;
}

int
DSC_SHELF_GetTemperature(SHELF_TEMP_INFO_ST *pstTempInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};

    if (NULL == pstTempInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    rc = __GetNameById(SHELF_DEFAULT, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", SHELF_DEFAULT, acName);
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_TEMPERATURE, acName);
    rc = get_decimal64_item(pstSess, acSelectXpath, &(pstTempInfo->dBoardTemp));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        return rc;
    }

    /* NOT support temperature threshold in OLS-P */
#if 0
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_TEMP_UP_THR, acName);
    rc = get_decimal64_item(pstSess, acSelectXpath, &(pstTempInfo->dHighThr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        return rc;
    }

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_TEMP_LOW_THR, acName);
    rc = get_decimal64_item(pstSess, acSelectXpath, &(pstTempInfo->dLowThr));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        return rc;
    }
#endif

cleanup:
    return rc;
}

int
DSC_SHELF_SetTemperatureThr(double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};
    char acName[32] = {0};

    sess = DSC_GetOperSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = __GetNameById(SHELF_DEFAULT, acName, sizeof(acName));
    if (SR_ERR_OK != rc || 0 == strlen(acName)) {
        LOG_WRITE(EN_LOG_ERROR, "Get shelf: %d name: %s error", SHELF_DEFAULT, acName);
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_TEMP_UP_THR, acName);
    rc = set_decimal64_item(sess, acSelectXpath, dHighThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", OOD_XPATH_SHELF_TEMP_UP_THR, sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_SHELF_TEMP_LOW_THR, acName);
    rc = set_decimal64_item(sess, acSelectXpath, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s %s", OOD_XPATH_SHELF_TEMP_LOW_THR, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetTemperatureThr DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

