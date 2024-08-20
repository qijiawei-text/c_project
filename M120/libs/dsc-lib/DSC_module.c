
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_ops.h"
#include "DSC_edfa.h"
#include "DSC_tdcm.h"
#include "DSC_ocm.h"
#include "DSC_log.h"
#include "DSC_common.h"
#include "DSC_circuitpack.h"
#include "DSC_cod.h"
#include "DSC_shelf.h"
#include "DSC_ood.h"
#include "DSC_module.h"
#include "DSC_waveplan.h"
#include "DSC_otdr.h"

static ST_CARD_CP_MAPPING m_astCardAndCp[SHELF_SLOT_NUM_MAX] = {
    {DSC_SLOT_NONE, CARD_UNKOWN, ""},
    /* {DSC_SLOT_IDX0, CARD_AMP, CIRCUIT_PACK_AMP}, */
};

ST_MODULE_TYPE_KEY_MAPPING m_astModuleCfgKey[] = {
    {MODULE_EDFA, EDFA_PA_CFG_KEY},
    {MODULE_EDFA, EDFA_BA_CFG_KEY},
    {MODULE_OPS, OPS_CFG_KEY},
    /* {MODULE_WSS, WSS1_CFG_KEY}, */
    /* {MODULE_WSS, WSS2_CFG_KEY}, */
    {MODULE_OCM, OCM_CFG_KEY},
    {MODULE_OTDR, OTDR_CFG_KEY},
    {MODULE_OTDR, OTDR_P_CFG_KEY}
    /* {MODULE_TDCM, TDCM_CFG_KEY}, */
};

static int
__GetModuleCfgMapping(ST_MODULE_TYPE_KEY_MAPPING *pstTypeKey, ST_MODULE_CFG_CP_MAPPING *pstMappingInfo)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acCpXpath[OPLK_PATH_MAX_LEN] = {0};
    char acComponentXpath[OPLK_PATH_MAX_LEN] = {0};


    if (NULL == pstMappingInfo ||
        NULL == pstTypeKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acCpXpath, 0, sizeof(acCpXpath));
    memset(acComponentXpath, 0, sizeof(acComponentXpath));
    switch (pstTypeKey->iModuleType) {
    case MODULE_EDFA:
        sprintf(acCpXpath, EDFA_CFG_PARENT_CP_PATH, pstTypeKey->acCfgKey);
        sprintf(acComponentXpath, EDFA_CFG_PARENT_COMPONENT_PATH, pstTypeKey->acCfgKey);
        break;
    case MODULE_OPS:
        sprintf(acCpXpath, OPS_CFG_PARENT_CP_PATH, pstTypeKey->acCfgKey);
        sprintf(acComponentXpath, OPS_CFG_PARENT_COMPONENT_PATH, pstTypeKey->acCfgKey);
        break;
    case MODULE_TDCM:
        sprintf(acCpXpath, TDCM_CFG_PARENT_CP_PATH, pstTypeKey->acCfgKey);
        sprintf(acComponentXpath, TDCM_CFG_PARENT_COMPONENT_PATH, pstTypeKey->acCfgKey);
        break;
    case MODULE_WSS:
        snprintf(acCpXpath, sizeof(acCpXpath), WSS_CFG_PARENT_CP_PATH, pstTypeKey->acCfgKey);
        snprintf(acComponentXpath, sizeof(acComponentXpath), WSS_CFG_PARENT_COMPONENT_PATH, pstTypeKey->acCfgKey);
        break;
    case MODULE_OCM:
        sprintf(acCpXpath, OCM_CFG_PARENT_CP_PATH, pstTypeKey->acCfgKey);
        sprintf(acComponentXpath, OCM_CFG_PARENT_COMPONENT_PATH, pstTypeKey->acCfgKey);
        break;
    default:
        break;
    }

    rc = get_string_item(pstSess, acCpXpath, pstMappingInfo->acCpName, sizeof(pstMappingInfo->acCpName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", acCpXpath, sr_strerror(rc));
        printf("Error by get_string_item: %s, %s\n", acCpXpath, sr_strerror(rc));
        goto cleanup;
    }

    rc = get_string_item(pstSess, acComponentXpath, pstMappingInfo->acComponentName, sizeof(pstMappingInfo->acComponentName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", acComponentXpath, sr_strerror(rc));
        goto cleanup;
    }

    strcpy(pstMappingInfo->acCfgKey, pstTypeKey->acCfgKey);

cleanup:
    return rc;
}

int
DSC_GetALLModuleCfgMapping(ST_MODULE_CFG_CP_MAPPING *pstMappingInfo)
{
    int rc = SR_ERR_OK;
    int i = 0;
    size_t nCnt = sizeof(m_astModuleCfgKey) / sizeof(m_astModuleCfgKey[0]);
    int iIndex = 0;
    ST_MODULE_CFG_CP_MAPPING astModuleKey[MODULE_MAX];

    memset(astModuleKey, 0, sizeof(astModuleKey));
    for (i = 0; i < nCnt; i++) {
        __GetModuleCfgMapping(&m_astModuleCfgKey[i], &astModuleKey[iIndex]);
        iIndex++;
    }

    memcpy(pstMappingInfo, astModuleKey, sizeof(ST_MODULE_CFG_CP_MAPPING) * iIndex);

    return rc;
}

int
DSC_GetOpticalCfgInfo(char *pcXpath, ST_OPTICAL_CONFIG *pstOpticalInfo, size_t *pnCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

    if (NULL == pstOpticalInfo) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = sr_get_items(pstSess, pcXpath, 0, SR_OPER_NO_SUBS, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items path : %s name, error: %s", pcXpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, OPTICAL_CFG_PARENT_KEY)) {
            j++;
            if (j > OPTICAL_CFG_PARENT_MAX_NUM) {
                goto cleanup;
            }
            strncpy(pstOpticalInfo[j].acKey, pstValues[i].data.string_val, sizeof(pstOpticalInfo[j].acKey) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, OPTICAL_CFG_PARENT_CP_NAME)) {
            strncpy(pstOpticalInfo[j].acCpName, pstValues[i].data.string_val, sizeof(pstOpticalInfo[j].acCpName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, OPTICAL_CFG_PARENT_COMPONENT_NAME)) {
            strncpy(pstOpticalInfo[j].acComponentName, pstValues[i].data.string_val, sizeof(pstOpticalInfo[j].acComponentName) - 1);
        } else {
        }
    }

    *pnCount = j + 1;

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_GetModuleCfgKeyByCpName(char *pcCpName, char *pcComponentName,
                            ST_MODULE_CFG_CP_MAPPING *pstAllInfo, char *pcKey)
{
    int rc = SR_ERR_OK;
    int i = 0;

    if (NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto cleanup;
    }

    for (i = 0; i < OPTICAL_CFG_PARENT_MAX_NUM; i++) {
        if ((0 == strcmp(pstAllInfo[i].acCpName, pcCpName)) && (0 == strcmp(pstAllInfo[i].acComponentName, pcComponentName))) {
            strcpy(pcKey, pstAllInfo[i].acCfgKey);
            return rc;
        }
    }

    rc = SR_ERR_NOT_FOUND;

cleanup:

    return rc;
}

int
DSC_GetSlotIdByModuleCfgKey(char *pcKey, ST_MODULE_CFG_CP_MAPPING *pstAllInfo, int *piSlotId)
{
    int rc = SR_ERR_OK;
    int i = 0;
    char acCpName[32] = {0};
    char acSlotName[32] = {0};

    if (NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acCpName, 0, sizeof(acCpName));
    memset(acSlotName, 0, sizeof(acSlotName));
    for (i = 0; i < OPTICAL_CFG_PARENT_MAX_NUM; i++) {
        if (0 == strcmp(pstAllInfo[i].acCfgKey, pcKey)) {
            strcpy(acCpName, pstAllInfo[i].acCpName);
            rc = DSC_CP_GetModuleSlot(acCpName, acSlotName);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetModuleSlot failed acCpName = %s", acCpName);
                goto cleanup;
            }

            rc = DSC_SHELF_GetSlotsIdByName(acSlotName, piSlotId);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetSlotsIdByName acSlotName = %s", acSlotName);
            }
            goto cleanup;
        }
    }

    rc = SR_ERR_NOT_FOUND;

cleanup:

    return rc;
}

static void
__InitSlotInfo()
{
    if (DSC_SLOT_NONE != m_astCardAndCp[0].iSlotIdx) {
        return;
    }

    /*
       TODO:
       The filter name would be extended by actual requirement.
       Add the new filter name if needed.
     */
    static ST_MODULE_TYPE_KEY_MAPPING m_pcSlotFilters[] = {
        {CARD_AMP, "amp"},
        {CARD_OPS, "ops"},
        {CARD_OCS, "ocs"}
    };

    ST_SLOT *pstSlot = NULL;
    ST_CIRCUIT_PACK *pstCp = NULL;
    size_t nSlotNum = 0, nCpNum = 0;
    size_t i = 0, j = 0, count = 0, temp = 0;
    int aiIndexVsLabel[SHELF_SLOT_NUM_MAX][2] = {0};
    int aiSwapCache[2] = {0};

    if (DSC_SHELF_GetSlots(&pstSlot, &nSlotNum) != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetSlots!");
        goto cleanup;
    }

    if (DSC_CP_GetAll(&pstCp, &nCpNum) != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetAll!");
        goto cleanup;
    }

    /* Filter the slot components by specified filters */
    for (i = 0; i < nSlotNum; i++) {
        for (j = 0; j < sizeof(m_pcSlotFilters) / sizeof(m_pcSlotFilters[0]); j++) {
            if (NULL == strstr(pstSlot[i].acName, m_pcSlotFilters[j].acCfgKey)) {
                continue;
            }
            aiIndexVsLabel[count][0] = i;
            aiIndexVsLabel[count++][1] = atoi(pstSlot[i].acLabel);
        }
    }

    /* Sort by the label number */
    for (i = 0; (i < count) && (i < count - 1); i++) {
        temp = i;
        for (j = i + 1; j < count; j++) {
            if (aiIndexVsLabel[temp][1] > aiIndexVsLabel[j][1]) {
                temp = j;
            }
        }

        if (temp != i) {
            memcpy(aiSwapCache, aiIndexVsLabel[i], sizeof(aiSwapCache));
            memcpy(aiIndexVsLabel[i], aiIndexVsLabel[temp], sizeof(aiSwapCache));
            memcpy(aiIndexVsLabel[temp], aiSwapCache, sizeof(aiSwapCache));
        }
    }

    /* Fill the result */
    for (i = 0; i < count; i++) {
        m_astCardAndCp[i].iSlotIdx = i;
        m_astCardAndCp[i].iCardType = CARD_UNKOWN;
        memset(m_astCardAndCp[i].acCpName, 0, sizeof(m_astCardAndCp[i].acCpName));

        /* Match card type */
        for (j = 0; j < sizeof(m_pcSlotFilters) / sizeof(m_pcSlotFilters[0]); j++) {
            if (strstr(pstSlot[aiIndexVsLabel[i][0]].acName, m_pcSlotFilters[j].acCfgKey)) {
                m_astCardAndCp[i].iCardType = m_pcSlotFilters[j].iModuleType;
                break;
            }
        }

        if (m_astCardAndCp[i].iCardType == CARD_UNKOWN) {
            LOG_WRITE(EN_LOG_ERROR, "Cannot determine the card type for slot name %s", pstSlot[aiIndexVsLabel[i][0]].acName);
            goto cleanup;
        }

        /* Match circuit-packs name */
        for (j = 0; j < nCpNum; j++) {
            if (strcmp(pstCp[j].acRefSlotName, pstSlot[aiIndexVsLabel[i][0]].acName) == 0) {
                strncpy(m_astCardAndCp[i].acCpName, pstCp[j].acName, sizeof(m_astCardAndCp[i].acCpName));
                break;
            }
        }

        if (j == nCpNum) {
            LOG_WRITE(EN_LOG_ERROR, "Cannot find circuit-packs name for slot name %s", pstSlot[aiIndexVsLabel[i][0]].acName);
            goto cleanup;
        }

        /* Record the result */
        LOG_WRITE(EN_LOG_NOTICE, "slot: %d, card type: %d, cp name: %s", m_astCardAndCp[i].iSlotIdx,
                  m_astCardAndCp[i].iCardType, m_astCardAndCp[i].acCpName);
    }

    /* Reset the others */
    for (i = count; i < sizeof(m_astCardAndCp) / sizeof(m_astCardAndCp[0]); i++) {
        m_astCardAndCp[i].iSlotIdx = DSC_SLOT_NONE;
        m_astCardAndCp[i].iCardType = CARD_UNKOWN;
        memset(m_astCardAndCp[i].acCpName, 0, sizeof(m_astCardAndCp[i].acCpName));
    }

cleanup:
    DEF_SAFE_FREE(pstSlot);
    DEF_SAFE_FREE(pstCp);
    return;
}

int
DSC_GetCpNameByCardType(int iSlotIdx, int iCardType, char *pcCpName)
{
    int i = 0;
    size_t nCardNum = sizeof(m_astCardAndCp) / sizeof(m_astCardAndCp[0]);

    __InitSlotInfo();

    for (i = 0; i < nCardNum; i++) {
        if (m_astCardAndCp[i].iSlotIdx == DSC_SLOT_NONE) {
            break;
        }

        if ((iSlotIdx == m_astCardAndCp[i].iSlotIdx) && (m_astCardAndCp[i].iCardType == iCardType)) {
            strcpy(pcCpName, m_astCardAndCp[i].acCpName);
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}

int
DSC_GetCpNameBySlotId(int iSlotIdx, char *pcCpName, ST_SHELF *pstShelf)
{
    int i = 0;
    char acSlotLabel[8] = {0};

    memset(acSlotLabel, 0, sizeof(acSlotLabel));
    sprintf(acSlotLabel, "%d", iSlotIdx);

    for (i = 0; i < SHELF_SLOT_NUM_MAX; i++) {
        if (0 == strcmp(acSlotLabel, pstShelf->astSlot[i].acLabel)) {
            strcpy(pcCpName, pstShelf->astSlot[i].aacRefProvisionedCircuitPack[0]);
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}

int
DSC_GetCardTypeByCpName(char *pcCpName, int *piCardType)
{
    int i = 0;
    size_t nCardNum = sizeof(m_astCardAndCp) / sizeof(m_astCardAndCp[0]);

    __InitSlotInfo();

    for (i = 0; i < nCardNum; i++) {
        if (m_astCardAndCp[i].iSlotIdx == DSC_SLOT_NONE) {
            break;
        }

        if (0 == strcmp(pcCpName, m_astCardAndCp[i].acCpName)) {
            *piCardType = m_astCardAndCp[i].iCardType;
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}

int
DSC_GetSlotIndexByCpKey(const char *pcCpKey, int *piSlotIndex)
{
    __InitSlotInfo();

    char key[32] = {0};
    memset(key, 0, sizeof(key));
    strncpy(key, pcCpKey, sizeof(key) - 1);
    UTIL_LowerStr(key);

    for (int i = 0; i < sizeof(m_astCardAndCp) / sizeof(m_astCardAndCp[0]); i++) {
        if (m_astCardAndCp[i].iSlotIdx == DSC_SLOT_NONE) {
            break;
        }

        if (strstr(m_astCardAndCp[i].acCpName, key)) {
            *piSlotIndex = m_astCardAndCp[i].iSlotIdx;
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}
