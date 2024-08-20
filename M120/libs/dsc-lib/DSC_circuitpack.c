#include "DSC_log.h"
#include "DSC_common.h"
#include "DSC_circuitpack.h"
#include "DSC_cod.h"
#include "DSC_shelf.h"
#include "DSC_port.h"


int
DSC_CP_GetInfoByName(char *pcCircuitPackName, ST_CIRCUIT_PACK *pstCircuitPackInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcCircuitPackName ||
        NULL == pstCircuitPackInfo) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OOD_XPATH_CIRCUIT_PACK_NAME, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->acName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OOD_XPATH_CIRCUIT_PACK_PRODUCT_CODE, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->acType),
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2],  COD_XPATH_CIRCUIT_PACK_LIFECYCLE_STATE, pcCircuitPackName), SR_STRING_T, (char *) (&pstCircuitPackInfo->enLifeCycleState)), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3],  COD_XPATH_CIRCUIT_PACK_ADMIN_STATE, pcCircuitPackName), SR_BOOL_T, (char *) (&pstCircuitPackInfo->enLifeCycleState)), */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OOD_XPATH_CIRCUIT_PACK_VENDOR, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stNode.acVendor),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], OOD_XPATH_CIRCUIT_PACK_MODEL, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stNode.acModel),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], OOD_XPATH_CIRCUIT_PACK_SERIAL_ID, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stNode.acSN),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], OOD_XPATH_CIRCUIT_PACK_COMMON_TYPE, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stCommon.acType),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], OOD_XPATH_CIRCUIT_PACK_COMMON_PRODUCT_CODE, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stCommon.acProductCode),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[9], OOD_XPATH_CIRCUIT_PACK_MFG_DATE, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stCommon.acMfgDate),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[10], OOD_XPATH_CIRCUIT_PACK_CLEI, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stCommon.acClei),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[11], OOD_XPATH_CIRCUIT_PACK_HW_VER, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->stPhysical.stCommon.acHwVer),
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[12], OOD_XPATH_CIRCUIT_PACK_OPRATNAL_STATE, acName), SR_BOOL_T, (char *) (&pstCircuitPackInfo->stPhysicalCommonInfo.enOperationalState)), */
        /* ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[13], OOD_XPATH_CIRCUIT_PACK_EQPMT_STATE, acName), SR_BOOL_T, (char *)(&pstCircuitPackInfo->enEqptState)), */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[14], OOD_XPATH_CIRCUIT_PACK_MODE, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->acMode),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], OOD_XPATH_CIRCUIT_PACK_REF_SHELF_NAME, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->acRefShelfName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[16], OOD_XPATH_CIRCUIT_PACK_REF_SLOT, pcCircuitPackName), SR_STRING_T, pstCircuitPackInfo->acRefSlotName)
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstCircuitPackInfo, 0x00, sizeof(ST_CIRCUIT_PACK));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_CIRCUIT_PACK_ALL, pcCircuitPackName);
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", OOD_XPATH_CIRCUIT_PACK_ALL, sr_strerror(rc));
        return rc;
    }

cleanup:
    return rc;
}


static int
__GetCircuitPackCount(size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;

    if (NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    pstSess = DSC_GetOperSession();
    rc = sr_get_items(pstSess, OOD_XPATH_CIRCUIT_PACK_NAME_ALL, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    *ptCount = tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetCircuitPackInfo(ST_CIRCUIT_PACK *pstCircuitPackInfo, size_t tMaxCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

/*    LOG_WRITE(EN_LOG_DEBUG, "%s, max count: %lu", __func__, tMaxCount); */

    if (NULL == pstCircuitPackInfo) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();

    rc = sr_get_items(pstSess, OOD_XPATH_LIST_CIRCUIT_PACKS_ALL, 0, SR_OPER_DEFAULT, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", OOD_XPATH_LIST_CIRCUIT_PACKS, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_CP_NAME)) {
            j++;
            if (j >= tMaxCount || j >= CIRCUIT_PACK_NUM_MAX) {
                goto cleanup;
            }
            strncpy(pstCircuitPackInfo[j].acName, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].acName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_REF_SHELF_NAME)) {
            strncpy(pstCircuitPackInfo[j].acRefShelfName, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].acRefShelfName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_REF_SLOT)) {
            strncpy(pstCircuitPackInfo[j].acRefSlotName, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].acRefSlotName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_LIFECYCLE_STATE)) {
            /*TBD*/
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_ADMIN_STATE)) {
            /*TBD*/
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_VENDOR)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stNode.acVendor, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stNode.acVendor) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_MODEL)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stNode.acModel, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stNode.acModel) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_SERIAL_ID)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stNode.acSN, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stNode.acSN) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMMON_TYPE)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stCommon.acType, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stCommon.acType) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMMON_PRODUCT_CODE)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stCommon.acProductCode, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stCommon.acProductCode) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_MFG_DATE)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stCommon.acMfgDate, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stCommon.acMfgDate) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_CLEI)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stCommon.acClei, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stCommon.acClei) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_HW_VER)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stCommon.acHwVer, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stCommon.acHwVer) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_SERVICE_UNIT_NAMET)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stFujitsu.acUnitName, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stFujitsu.acUnitName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_SERVICE_PACK_FC_NUMBER)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stFujitsu.acFcNumber, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stFujitsu.acFcNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_SERVICE_PACK_PIU_ISSUE_NUMBER)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stFujitsu.acPiuIssueNumber, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stFujitsu.acPiuIssueNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_SERVICE_PACK_FUJITSU_PART_NUMBER)) {
            strncpy(pstCircuitPackInfo[j].stPhysical.stFujitsu.acFujitsuPartNumber, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stPhysical.stFujitsu.acFujitsuPartNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_OPRATNAL_STATE)) {
            /*TBD*/
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_RESET_CAUSE)) {
            /*TBD*/
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_CATEGORY_TYPE)) {
            /*TBD*/
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_CATEGORYE_EXTSN)) {
            strncpy(pstCircuitPackInfo[j].stCategory.acExtension, pstValues[i].data.string_val, sizeof(pstCircuitPackInfo[j].stCategory.acExtension) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_EQPMT_STATE)) {
        }
    }

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
__GetInfos(ST_CIRCUIT_PACK **ppstCircuitPackInfo, size_t *ptCircuitPackCount)
{
    int rc = SR_ERR_OK;
    size_t tCircuitPackCount = 0;
    ST_CIRCUIT_PACK *pstCircuitPack = NULL;

    if (NULL == ppstCircuitPackInfo || NULL == ptCircuitPackCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetCircuitPackCount(&tCircuitPackCount);
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            rc = SR_ERR_OK;
            *ptCircuitPackCount = 0;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get circuit pack count error: %s", sr_strerror(rc));
        }
        goto cleanup;
    }

/*    LOG_WRITE(EN_LOG_DEBUG, "circuit pack count: %lu", tCircuitPackCount); */

    pstCircuitPack = (ST_CIRCUIT_PACK *)malloc(sizeof(ST_CIRCUIT_PACK) * tCircuitPackCount);
    if (NULL == pstCircuitPack) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstCircuitPack, 0x00, (sizeof(ST_CIRCUIT_PACK) * tCircuitPackCount));

    rc = __GetCircuitPackInfo(pstCircuitPack, tCircuitPackCount);
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstCircuitPack);
        goto cleanup;
    }

    /* for (size_t i = 0; i < tCircuitPackCount; i++) {
        LOG_WRITE(EN_LOG_ERROR, "%s, %s, %s",
                pstCircuitPack[i].acName,
                pstCircuitPack[i].acRefShelfName,
                pstCircuitPack[i].acRefSlotName);
       } */

    *ppstCircuitPackInfo = pstCircuitPack;
    *ptCircuitPackCount = tCircuitPackCount;

cleanup:
    return rc;
}

int
DSC_CP_GetInfos(ST_CIRCUIT_PACK **ppstCircuitPackInfo, size_t *ptCircuitPackCount)
{
    return __GetInfos(ppstCircuitPackInfo, ptCircuitPackCount);
}

static int
__GetComponentCount(char *pcCircuitPackName, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcCircuitPackName || NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_CPS_COMPONENT_NAME_ALL, pcCircuitPackName);

    rc = sr_get_items(pstSess, acXpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    *ptCount = tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetComponentInfo(char *pcCircuitPackName, ST_CIRCUIT_PACK_COMPONENT *pstComponent, size_t tMaxCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

    if (NULL == pstComponent) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_CP_LIST_COMPONENTS_ALL, pcCircuitPackName);

    pstSess = DSC_GetOperSession();
    rc = sr_get_items(pstSess, acXpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMPONENT_NAME)) {
            j++;
            if (j >= tMaxCount || j >= CIRCUIT_PACK_NUM_MAX) {
                goto cleanup;
            }
            strncpy(pstComponent[j].acName, pstValues[i].data.string_val, sizeof(pstComponent[j].acName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMPONENT_BOOT)) {
            /* TBD  */
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMPONENT_CURRENT_VER)) {
            strncpy(pstComponent[j].acCurrentVer, pstValues[i].data.string_val, sizeof(pstComponent[j].acCurrentVer) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_COMPONENT_VER_TO_APPLY)) {
            strncpy(pstComponent[j].acVerToApply, pstValues[i].data.string_val, sizeof(pstComponent[j].acVerToApply) - 1);
        }
    }

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
__GetComponents(char *pcCircuitPackName, ST_CIRCUIT_PACK_COMPONENT **ppstCpComponent, size_t *ptCompntCount)
{
    int rc = SR_ERR_OK;
    ST_CIRCUIT_PACK_COMPONENT *pstComponent = NULL;

    if (NULL == pcCircuitPackName ||
        NULL == ppstCpComponent ||
        NULL == ptCompntCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetComponentCount(pcCircuitPackName, ptCompntCount);
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            rc = SR_ERR_OK;
            *ptCompntCount = 0;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get component count error: %s", sr_strerror(rc));
        }
        goto cleanup;
    }

    pstComponent = (ST_CIRCUIT_PACK_COMPONENT *)malloc(sizeof(ST_CIRCUIT_PACK_COMPONENT) * (*ptCompntCount));
    if (NULL == pstComponent) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstComponent, 0x00, (sizeof(ST_CIRCUIT_PACK_COMPONENT) * (*ptCompntCount)));

    rc = __GetComponentInfo(pcCircuitPackName, pstComponent, *ptCompntCount);
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstComponent);
        goto cleanup;
    }

    *ppstCpComponent = pstComponent;

cleanup:
    return rc;
}

int
DSC_CP_GetComponents(char *pcCircuitPackName, ST_CIRCUIT_PACK_COMPONENT **ppstCpComponent, size_t *ptCompntCount)
{
    return __GetComponents(pcCircuitPackName, ppstCpComponent, ptCompntCount);
}

static int
__SetComponentCurrentVer(char *pcCircuitPackName, char *pcComponentName, char *pcCurrentVer)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcCircuitPackName ||
        NULL == pcComponentName ||
        NULL == pcCurrentVer) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CPS_COMPONENT_CURRENT_VER, pcCircuitPackName, pcComponentName);

    rc = set_string_item(DSC_GetOperSession(), acXpath, pcCurrentVer, strlen(pcCurrentVer));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, pcCurrentVer, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(DSC_GetOperSession());
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit: %s, %s, %s", acXpath, pcCurrentVer, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_CP_SetComponentCurrentVer(char *pcCircuitPackName, char *pcComponentName, char *pcCurrentVer)
{
    return __SetComponentCurrentVer(pcCircuitPackName, pcComponentName, pcCurrentVer);
}

static int
__GetPortCount(char *pcCircuitPackName, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    if (NULL == pcCircuitPackName || NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_CP_PORT_ID_ALL, pcCircuitPackName);
    rc = sr_get_items(pstSess, acXpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        *ptCount = 0;
        goto cleanup;
    }

    *ptCount = tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetPortInfo(char *pcCircuitPackName, ST_CIRCUIT_PACK_PORT *pstPorts, size_t tMaxCount)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    if (NULL == pcCircuitPackName || NULL == pstPorts) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OOD_XPATH_CP_LIST_PORTS_ALL, pcCircuitPackName);
    rc = sr_get_items(pstSess, acXpath, 0, 0, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_ID)) {
            j++;
            if (j >= tMaxCount || j >= CP_PORT_NUM_MAX) {
                goto cleanup;
            }
            pstPorts[j].u32PortId = pstValues[i].data.uint32_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_NAME)) {
            strncpy(pstPorts[j].acName, pstValues[i].data.string_val, sizeof(pstPorts[j].acName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_TYPE)) {
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_WAVELENGTH_TYPE)) {
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_QUAL)) {
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_CURRENT_POWER)) {
            pstPorts[j].dCurrentPower = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_DIRECTION)) {
            pstPorts[j].enDirection = DSC_PortDirectionToEnum(pstValues[i].data.enum_val);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_CAPA_MIN_RX)) {
            pstPorts[j].stProperty.dCapMinRxPower = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_CAPA_MAX_RX)) {
            pstPorts[j].stProperty.dCapMaxRxPower = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_CAPA_MIN_TX)) {
            pstPorts[j].stProperty.dCapMinTxPower = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_CP_PORT_CAPA_MAX_TX)) {
            pstPorts[j].stProperty.dCapMaxTxPower = pstValues[i].data.decimal64_val;
        }
    }

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
__GetPorts(char *pcCircuitPackName, ST_CIRCUIT_PACK_PORT **ppstPorts, size_t *ptPortsCount)
{
    int rc = SR_ERR_OK;
    ST_CIRCUIT_PACK_PORT *pstPorts = NULL;

    if (NULL == pcCircuitPackName ||
        NULL == ppstPorts ||
        NULL == ptPortsCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetPortCount(pcCircuitPackName, ptPortsCount);
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            rc = SR_ERR_OK;
            *ptPortsCount = 0;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get port count error: %s", sr_strerror(rc));
        }
        goto cleanup;
    }

    pstPorts = (ST_CIRCUIT_PACK_PORT *)malloc(sizeof(ST_CIRCUIT_PACK_PORT) * (*ptPortsCount));
    if (NULL == pstPorts) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstPorts, 0x00, (sizeof(ST_CIRCUIT_PACK_PORT) * (*ptPortsCount)));

    rc = __GetPortInfo(pcCircuitPackName, pstPorts, *ptPortsCount);
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstPorts);
        goto cleanup;
    }

    *ppstPorts = pstPorts;

cleanup:
    return rc;
}

int
DSC_CP_GetPorts(char *pcCircuitPackName, ST_CIRCUIT_PACK_PORT **ppstPorts, size_t *ptPortsCount, bool bIsInternal)
{
    return __GetPorts(pcCircuitPackName, ppstPorts, ptPortsCount);
}

int
DSC_CP_GetMainboardPorts(ST_CIRCUIT_PACK_PORT **ppstPorts, size_t *ptPortsCount)
{
    return __GetPorts(CIRCUIT_PACK_BOARD, ppstPorts, ptPortsCount);
}

int
__GetAll(ST_CIRCUIT_PACK **ppCircuitPack, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    ST_CIRCUIT_PACK *pstCircuitPackInfos = NULL;
    size_t tCircuitPacksCount = 0;

    ST_CIRCUIT_PACK_COMPONENT *pstComponents = NULL;
    size_t tComponentsCount = 0;

    if (NULL == ppCircuitPack ||
        NULL == ptCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetInfos(&pstCircuitPackInfos, &tCircuitPacksCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "circuit pack count: %lu", tCircuitPacksCount); */

    for (size_t i = 0; i < tCircuitPacksCount; i++) {
        rc = __GetComponents(pstCircuitPackInfos[i].acName, &pstComponents, &tComponentsCount);
        if (rc != SR_ERR_OK) {
            goto cleanup;
        }
        if (pstComponents != NULL) {
            memcpy(pstCircuitPackInfos[i].astComponent, pstComponents, sizeof(ST_CIRCUIT_PACK_COMPONENT) * tComponentsCount);
            free(pstComponents);
            pstComponents = NULL;
        }

        /* LOG_WRITE(EN_LOG_DEBUG, "+++++++++++%s, component +++++++++++", pstCircuitPackInfos[i].acName); */
        /* for (size_t j = 0; j < tComponentsCount; j++) {
            LOG_WRITE(EN_LOG_DEBUG, "%s, current: %s, apply: %s",
                    pstCircuitPackInfos[i].astComponent[j].acName,
                    pstCircuitPackInfos[i].astComponent[j].acCurrentVer,
                    pstCircuitPackInfos[i].astComponent[j].acVerToApply);
           }*/
#if 0
        rc = __GetPorts(pstCircuitPackInfos[i].acName, &pstPorts, &tPortsCount, bIsInternal);
        if (rc != SR_ERR_OK) {
            goto cleanup;
        }
        if (pstPorts != NULL) {
            memcpy(pstCircuitPackInfos[i].astPort, pstPorts, sizeof(ST_CIRCUIT_PACK_PORT) * tPortsCount);
            free(pstPorts);
            pstPorts = NULL;
        }
#endif
        /*for (size_t j = 0; j < tPortsCount; j++) {
            LOG_WRITE(EN_LOG_DEBUG, "port-id: %u, type: %s, min-rx: %.2f",
                    pstCircuitPackInfos[i].astPort[j].u32PortId,
                    pstCircuitPackInfos[i].astPort[j].acType,
                    pstCircuitPackInfos[i].astPort[j].stProperty.dCapMinRxPower);
           }*/
    }


    *ppCircuitPack = pstCircuitPackInfos;
    *ptCount = tCircuitPacksCount;

    return rc;
cleanup:
    DEF_SAFE_FREE(pstCircuitPackInfos);
    DEF_SAFE_FREE(pstComponents);
    return rc;
}

int
DSC_CP_GetAll(ST_CIRCUIT_PACK **ppCircuitPack, size_t *ptCount)
{
    return __GetAll(ppCircuitPack, ptCount);
}

int
DSC_CP_SetCpldVer(uint32_t u32CpldIndex, char *pcVersion)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcVersion) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CPS_COMPONENT_CURRENT_VER, "main-board", "cpld");
    rc = set_string_item(DSC_GetOperSession(), acXpath, pcVersion, strlen(pcVersion));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, pcVersion, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(DSC_GetOperSession());
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit: %s, %s, %s", acXpath, pcVersion, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

#define __set_circuitpack_mfginfo(path, data_value) do {          \
        memset(acXpath, 0x00, sizeof(acXpath));              \
        snprintf(acXpath, sizeof(acXpath), path, pcCpName);  \
        rc = set_string_item(pstSess, acXpath, data_value, strlen(data_value)); \
        if (rc != SR_ERR_OK) { \
            LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, data_value, sr_strerror(rc)); \
            goto cleanup; \
        } \
        LOG_WRITE(EN_LOG_DEBUG, "%s %s", acXpath, data_value); \
} while (0)

#define __del_circuitpack_mfginfo(path) do {          \
        memset(acXpath, 0x00, sizeof(acXpath));                                                  \
        snprintf(acXpath, sizeof(acXpath), path, pcCpName);  \
        rc = sr_oper_delete_item_str(pstSess, acXpath, NULL, SR_EDIT_DEFAULT); \
        if (rc != SR_ERR_OK) { \
            LOG_WRITE(EN_LOG_ERROR, "sr_oper_delete_item_str: %s, %s", acXpath, sr_strerror(rc)); \
            goto cleanup; \
        } \
        LOG_WRITE(EN_LOG_DEBUG, "sr_delete_item:%s", acXpath); \
} while (0)

static int
__SetCircuitPackPhysical(char *pcCpName, ST_PHYSICAL_INFO *pstPhysical)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    if (NULL == pcCpName || NULL == pstPhysical) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /*
       vendor/model/serial-id are mandatory nodes in org-openroadm-physical-types.yang
     */
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "++++++++++pcCpName:%s", pcCpName);
    LOG_WRITE(EN_LOG_NOTICE, "acVendor:%s, acModel:%s, acSN:%s, acProductCode:%s,", pstPhysical->stNode.acVendor, pstPhysical->stNode.acModel,
              pstPhysical->stNode.acSN, pstPhysical->stCommon.acProductCode);

    LOG_WRITE(EN_LOG_NOTICE, "acClei:%s, acHwVer:%s, acMfgDate:%s,", pstPhysical->stCommon.acClei,
              pstPhysical->stCommon.acHwVer, pstPhysical->stCommon.acMfgDate);
#endif
    if (strlen(pstPhysical->stNode.acVendor) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_VENDOR, pstPhysical->stNode.acVendor);
    } else {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_VENDOR, DEF_OOD_VENDOR);
    }

    if (strlen(pstPhysical->stNode.acModel) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_MODEL, pstPhysical->stNode.acModel);
    } else {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_MODEL, DEF_OOD_MODEL);
    }

    if (strlen(pstPhysical->stNode.acSN) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_SERIAL_ID, pstPhysical->stNode.acSN);
    } else {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_SERIAL_ID, DEF_OOD_SERIAL_ID);
    }

    if (strlen(pstPhysical->stCommon.acProductCode) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_COMMON_PRODUCT_CODE, pstPhysical->stCommon.acProductCode);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_COMMON_PRODUCT_CODE);
    }

    if (strlen(pstPhysical->stCommon.acClei) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_CLEI, pstPhysical->stCommon.acClei);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_CLEI);
    }

    if (strlen(pstPhysical->stCommon.acHwVer) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_HW_VER, pstPhysical->stCommon.acHwVer);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_HW_VER);
    }

    if (strlen(pstPhysical->stCommon.acMfgDate) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_MFG_DATE, pstPhysical->stCommon.acMfgDate);
    } else {
        /* NOTE: set default value because of deleting node failed, TODO */
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_MFG_DATE, DEF_OOD_MANUFACTURE_DATE);
        /* __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_MFG_DATE); */
    }

    if (strlen(pstPhysical->stFujitsu.acUnitName) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_UNIT_NAME, pstPhysical->stFujitsu.acUnitName);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_UNIT_NAME);
    }

    if (strlen(pstPhysical->stFujitsu.acFcNumber) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_FC_NUMBER, pstPhysical->stFujitsu.acFcNumber);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_FC_NUMBER);
    }

    if (strlen(pstPhysical->stFujitsu.acPiuIssueNumber) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_PIU_ISSUE_NUMBER, pstPhysical->stFujitsu.acPiuIssueNumber);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_PIU_ISSUE_NUMBER);
    }

    if (strlen(pstPhysical->stFujitsu.acFujitsuPartNumber) > 0) {
        __set_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_FUJITSU_PART_NUMBER, pstPhysical->stFujitsu.acFujitsuPartNumber);
    } else {
        __del_circuitpack_mfginfo(OOD_XPATH_CIRCUIT_PACK_FUJITSU_PART_NUMBER);
    }

    rc = DSC_Commit(pstSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "commit error: %s", sr_strerror(rc));
    }

cleanup:
    /* LOG_WRITE(EN_LOG_NOTICE, "--------pcCpName:%s, rc:%d", pcCpName, rc); */
    return rc;
}

#undef __set_circuitpack_mfginfo

int
DSC_CP_SetFanPhysical(int iIndex, ST_PHYSICAL_INFO *pstPhysical)
{
    char acName[32] = {0};

    memset(acName, 0x00, sizeof(acName));
    snprintf(acName, sizeof(acName), "%s%d", CIRCUIT_PACK_KEY_FAN, iIndex);

    return __SetCircuitPackPhysical(acName, pstPhysical);
}

int
DSC_CP_SetPowerPhysical(int iIndex, ST_PHYSICAL_INFO *pstPhysical)
{
    char acName[32] = {0};

    memset(acName, 0x00, sizeof(acName));
    snprintf(acName, sizeof(acName), "%s%d", CIRCUIT_PACK_KEY_POWER, iIndex);

    return __SetCircuitPackPhysical(acName, pstPhysical);
}

int
DSC_CP_SetCircuitPackPhysical(char *pcCircuitPack, ST_PHYSICAL_INFO *pstPhysical)
{
    return __SetCircuitPackPhysical(pcCircuitPack, pstPhysical);
}

static int
__SetCircuitPackSlot(char *pcCpName, char *pcSlotName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    if (NULL == pcCpName ||
        NULL == pcSlotName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_REF_SLOT, pcCpName);

    rc = set_string_item(pstSess, acXpath, pcSlotName, strlen(pcSlotName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, pcSlotName, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "commit error: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_CP_SetFanSlot(int iIndex, char *pcSlotName)
{
    char acName[32] = {0};

    memset(acName, 0x00, sizeof(acName));
    snprintf(acName, sizeof(acName), "%s%d", CIRCUIT_PACK_KEY_FAN, iIndex);

    return __SetCircuitPackSlot(acName, pcSlotName);
}

int
DSC_CP_SetPowerSlot(int iIndex, char *pcSlotName)
{
    char acName[32] = {0};

    memset(acName, 0x00, sizeof(acName));
    snprintf(acName, sizeof(acName), "%s%d", CIRCUIT_PACK_KEY_POWER, iIndex);

    return __SetCircuitPackSlot(acName, pcSlotName);
}

int
DSC_CP_SetModuleSlot(char *pcCpName, char *pcSlotName)
{
    return __SetCircuitPackSlot(pcCpName, pcSlotName);
}

int
DSC_CP_GetModuleSlot(char *pcCpName, char *pcSlotName)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcCpName ||
        NULL == pcSlotName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OOD_XPATH_CIRCUIT_PACK_REF_SLOT, pcCpName);

    rc = get_string_item(DSC_GetOperSession(), acXpath, pcSlotName, 32);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, pcSlotName, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

#if 0
int
DSC_CP_GetInfoByName(char *pcName, ST_CIRCUIT_PACK *pstOutCpInfo)
{
    int i = 0;
    ST_CIRCUIT_PACK *pstCp = NULL;
    size_t nCpCount = 0;
    int rc = SR_ERR_OK;

    rc = DSC_CP_GetInfos(&pstCp, &nCpCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetInfos");
        goto cleanup;
    }

    for (i = 0; i < nCpCount; i++) {
        if (0 == strcmp(pstCp[i].acName, pcName)) {
            memcpy(pstOutCpInfo, &pstCp[i], sizeof(ST_CIRCUIT_PACK));
            DEF_SAFE_FREE(pstCp);
            return SR_ERR_OK;
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetInfoByName pcName = %s", pcName);
    rc = SR_ERR_NOT_FOUND;

cleanup:
    DEF_SAFE_FREE(pstCp);
    return rc;
}
#endif
int
DSC_CP_GetCurCpInSlot(char *pcSlotName, char *pcCpName)
{
    ST_CIRCUIT_PACK *pstCircuitPack = NULL;
    size_t nCircuitPackCount = 0;
    int rc = SR_ERR_OK;
    int i = 0;
    bool bFindFlag = false;


    rc = DSC_CP_GetAll(&pstCircuitPack, &nCircuitPackCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetAll : %s", sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < nCircuitPackCount; i++) {
        if (0 == strcmp(pstCircuitPack[i].acRefSlotName, pcSlotName)) {
            bFindFlag = true;
            break;
        }
    }

    if (true == bFindFlag) {
        strcpy(pcCpName, pstCircuitPack[i].acName);
    } else {
        rc = SR_ERR_NOT_FOUND;
    }

cleanup:
    DEF_SAFE_FREE(pstCircuitPack);
    return rc;
}

