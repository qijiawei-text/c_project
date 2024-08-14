/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_pm.h"

static ENTITY_TABLE_ST m_astEntityTable[] = {{EN_ENTITY_IN65, PM_ENTITY_NAME_IN65_STR},
                                             {EN_ENTITY_IN66, PM_ENTITY_NAME_IN66_STR}};

EN_PM_PARA_UNIT
DSC_PmGetUnitEnum(char *pcString)
{
    if (NULL == pcString) {
        return EN_PARA_UNIT_UNKNOWN;
    }
    if (0 == strcasecmp(pcString, UNIT_NAME_UKNOWN)) {
        return EN_PARA_UNIT_UNKNOWN;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_DB)) {
        return EN_PARA_UNIT_DB;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_DBM)) {
        return EN_PARA_UNIT_DBM;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_MA)) {
        return EN_PARA_UNIT_MA;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_CENTIGRADE)) {
        return EN_PARA_UNIT_CENTIGRADE;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_SECOND)) {
        return EN_PARA_UNIT_SECOND;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_AMPS)) {
        return EN_PARA_UNIT_AMPS;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_VOLTS)) {
        return EN_PARA_UNIT_VOLTS;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_RPM)) {
        return EN_PARA_UNIT_RPM;
    } else if (0 == strcasecmp(pcString, UNIT_NAME_WATTS)) {
        return EN_PARA_UNIT_WATTS;
    } else {
        return EN_PARA_UNIT_UNKNOWN;
    }
}
char *
DSC_PmGetUnitString(char *pcString, EN_PM_PARA_UNIT enUnit)
{
    if (NULL == pcString) {
        return NULL;
    }
    switch (enUnit) {
    case EN_PARA_UNIT_UNKNOWN:
        strcpy(pcString, UNIT_NAME_UKNOWN);
        break;
    case EN_PARA_UNIT_DB:
        strcpy(pcString, UNIT_NAME_DB);
        break;
    case EN_PARA_UNIT_DBM:
        strcpy(pcString, UNIT_NAME_DBM);
        break;
    case EN_PARA_UNIT_MA:
        strcpy(pcString, UNIT_NAME_MA);
        break;
    case EN_PARA_UNIT_CENTIGRADE:
        strcpy(pcString, UNIT_NAME_CENTIGRADE);
        break;
    case EN_PARA_UNIT_SECOND:
        strcpy(pcString, UNIT_NAME_SECOND);
        break;
    case EN_PARA_UNIT_AMPS:
        strcpy(pcString, UNIT_NAME_AMPS);
        break;
    case EN_PARA_UNIT_VOLTS:
        strcpy(pcString, UNIT_NAME_VOLTS);
        break;
    case EN_PARA_UNIT_RPM:
        strcpy(pcString, UNIT_NAME_RPM);
        break;
    case EN_PARA_UNIT_WATTS:
        strcpy(pcString, UNIT_NAME_WATTS);
        break;
    default:
        strcpy(pcString, UNIT_NAME_UKNOWN);
        break;
    }
    return pcString;
}
int
DSC_GetCurrentPM(ST_DSC_PM_DATA aastPmUnit[][EN_DSC_PM_GRANULARITIES_MAX], size_t *pnCnt)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    int j = 0, k = 0;

    if (NULL == pnCnt || NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /* get all list instances with their content (recursive) */
    rc = get_items(pstSess, PM_XPATH_CURRENT_ALL, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), PM_XPATH_CURRENT_ALL);

        *pnCnt = 0;
        rc = SR_ERR_OK;
        goto cleanup;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto cleanup;
    }

    j = -1;

#if 0
#ifdef _NR1004_
    k = EN_DSC_PM_15MIN;
#else
    k = EN_DSC_PM_1MIN;
#endif
#endif
    k = EN_DSC_PM_15MIN;
    for (int i = 0; i < nCnt; i++) {
        /* dsc_print_value(&pstVal[i]); */
        /* check one pm start date */
        if (strstr(pstVal[i].xpath, PM_XPATH_PM_ID) != NULL) {
            j++;
#if 0
            #ifdef _NR1004_
            strncpy(aastPmUnit[j][1].acPmId, pstVal[i].data.string_val, sizeof(aastPmUnit[j][0].acPmId) - 1);
            #else
            strncpy(aastPmUnit[j][0].acPmId, pstVal[i].data.string_val, sizeof(aastPmUnit[j][0].acPmId) - 1);
            #endif
#endif
            strncpy(aastPmUnit[j][EN_DSC_PM_15MIN].acPmId, pstVal[i].data.string_val, sizeof(aastPmUnit[j][0].acPmId) - 1);
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_UNIT) != NULL) {
            strncpy(aastPmUnit[j][k].acUnit, pstVal[i].data.string_val, sizeof(aastPmUnit[j][k].acUnit) - 1);
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_GRANULARITY) != NULL) {
            if (0 == strcmp(PM_STR_1_MIN, pstVal[i].data.string_val)) {
                k = EN_DSC_PM_1MIN;
            } else if (0 == strcmp(PM_STR_15_MIN, pstVal[i].data.string_val)) {
                k = EN_DSC_PM_15MIN;
            } else if (0 == strcmp(PM_STR_24_HOUR, pstVal[i].data.string_val)) {
                k = EN_DSC_PM_24HOUR;
            }
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_RETRIEVAL_TIME) != NULL) {
            strncpy(aastPmUnit[j][k].acTime, pstVal[i].data.string_val, sizeof(aastPmUnit[j][k].acTime) - 1);
        }

        if (strstr(pstVal[i].xpath, PM_XPATH_PM_MIN_VALUE) != NULL) {
            aastPmUnit[j][k].dMin = pstVal[i].data.decimal64_val;
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_MAX_VALUE) != NULL) {
            aastPmUnit[j][k].dMax = pstVal[i].data.decimal64_val;
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_AVERAGE_VALUE) != NULL) {
            aastPmUnit[j][k].dAvg = pstVal[i].data.decimal64_val;
        }

        if (strstr(pstVal[i].xpath, PM_XPATH_PM_MIN_TIMESTAMP) != NULL) {
            strncpy(aastPmUnit[j][k].acMinTime, pstVal[i].data.string_val, sizeof(aastPmUnit[j][k].acMinTime) - 1);
        } else if (strstr(pstVal[i].xpath, PM_XPATH_PM_MAX_TIMESTAMP) != NULL) {
            strncpy(aastPmUnit[j][k].acMaxTime, pstVal[i].data.string_val, sizeof(aastPmUnit[j][k].acMaxTime) - 1);
        }
    }

    *pnCnt = j + 1;

cleanup:
    sr_free_values(pstVal, nCnt);
    return rc;
}


int
DSC_GetHistoryPM(EN_DSC_PM_GRANULARITIES gran, ST_DSC_PM_HIS **ppstPmHis, size_t *pnCnt)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    char acPmGran[PM_STRING_LEN] = {0};
    size_t nCnt = 0;
    /* int j = 0; */
    int k = 0;
    ST_DSC_PM_DATA *pstPmUnit = NULL;
    /* bool bMove = true; */
    /* int iBinNum = 0; */
    sr_data_t *pstSubTrees = NULL;
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    sr_node_t *pstPmParameterValueNode = NULL;
    sr_node_t *pstMeasurementNode = NULL;
    sr_node_t *pstMeasurementsNode = NULL;
    sr_node_t *pstBinnedPmNode = NULL;

    if (NULL == pnCnt || NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    memset(acPmGran, 0x00, sizeof(acPmGran));

    if (gran == EN_DSC_PM_1MIN) {
        snprintf(acPmGran, PM_STRING_LEN, PM_STR_1_MIN);
    } else if (gran == EN_DSC_PM_15MIN) {
        snprintf(acPmGran, PM_STRING_LEN, PM_STR_15_MIN);
    } else if (gran == EN_DSC_PM_24HOUR) {
        snprintf(acPmGran, PM_STRING_LEN, PM_STR_24_HOUR);
    }

    rc = get_subtrees(pstSess, "/org-openroadm-pm:historicalPmlist/historicalPm", 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error sr_get_items %s, %s", PM_XPATH_HISTORICAL_ALL_ID, sr_strerror(rc));
        goto cleanup;
    }

    rc = lyd_find_xpath(pstSubTrees->tree, "/org-openroadm-pm:historicalPmlist/historicalPm", &nodeSet);
    if (LY_SUCCESS == rc) {
        nCnt = nodeSet->count;
    }

    *ppstPmHis = (ST_DSC_PM_HIS *)malloc(sizeof(ST_DSC_PM_HIS) * nCnt);
    if (NULL == *ppstPmHis) {
        LOG_WRITE(EN_LOG_ERROR, "error malloc for history pm");
        rc = SR_ERR_NO_MEMORY;
        goto cleanup;
    }

    for (int i = 0; i < nCnt; i++) {

        if (gran == EN_DSC_PM_1MIN) {
            pstPmUnit = (*ppstPmHis)[k++].ast1MinHisPm;
        } else if (gran == EN_DSC_PM_15MIN) {
            pstPmUnit = (*ppstPmHis)[k++].ast15MinHisPm;
        } else if (gran == EN_DSC_PM_24HOUR) {
            pstPmUnit = (*ppstPmHis)[k++].ast24HourHisPm;
        }

        pstNode = nodeSet->dnodes[i]; /*historicalPm*/
        pstNode = lyd_node_to_inner(pstNode)->child; /*id*/
        strncpy(pstPmUnit->acPmId, lyd_get_value(pstNode), PM_STRING_LEN - 1);
        pstMeasurementsNode = pstNode->next->next->next; /*measurements*/

        while (pstMeasurementsNode) {
            pstNode = lyd_node_to_inner(pstMeasurementsNode)->child; /*granularity*/
            if (strcmp(lyd_get_value(pstNode), acPmGran) == 0) {
                pstBinnedPmNode = pstNode->next;  /*binned-pm*/
                while (pstBinnedPmNode) {
                    pstNode = lyd_node_to_inner(pstBinnedPmNode)->child; /*bin-number*/
                    pstMeasurementNode = pstNode->next; /*measurement*/

                    pstPmParameterValueNode = lyd_node_to_inner(pstMeasurementNode)->child; /*pmParameterValue*/
                    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
                    pstLeaf = pstLeaf->next; /*min*/
                    pstPmUnit->dMin = atof(lyd_get_value(pstLeaf));

                    pstPmParameterValueNode = pstPmParameterValueNode->next;
                    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
                    pstLeaf = pstLeaf->next;
                    pstPmUnit->dMax = atof(lyd_get_value(pstLeaf));

                    pstPmParameterValueNode = pstPmParameterValueNode->next;
                    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
                    pstLeaf = pstLeaf->next;
                    pstPmUnit->dAvg = atof(lyd_get_value(pstLeaf));

                    pstLeaf = pstPmParameterValueNode->next; /*pmParameterUnit*/
                    strncpy(pstPmUnit->acUnit, lyd_get_value(pstLeaf), PM_STRING_LEN - 1);

                    pstLeaf = pstMeasurementNode->next;
                    strncpy(pstPmUnit->acTime, lyd_get_value(pstLeaf), PM_STRING_LEN - 1);
                    pstPmUnit++;
                    pstBinnedPmNode = pstBinnedPmNode->next;
                }
            }
            pstMeasurementsNode = pstMeasurementsNode->next;
        }
    }

    *pnCnt = k;

cleanup:
    if (pstSubTrees != NULL) {
        sr_release_data(pstSubTrees);
        pstSubTrees = NULL;
    }

    if (nodeSet != NULL) {
        ly_set_free(nodeSet, NULL);
    }

    return rc;
}

int
DSC_GetPMData(char *acPmId, char *acGran, uint32_t uiBinNum, ST_DSC_PM_DATA *pstPmUnit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
    sr_data_t *pstSubTrees = NULL;
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    sr_node_t *pstPmParameterValueNode = NULL;
    sr_node_t *pstMeasurementNode = NULL;
    char acPath[256] = {0};

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    sprintf(acPath, "/org-openroadm-pm:historicalPmlist/historicalPm[id='%s']/measurements[granularity='%s']/binned-pm[bin-number='%d']", acPmId, acGran, uiBinNum);
    rc = get_subtrees(pstSess, acPath, 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error sr_get_items %s, %s", PM_XPATH_HISTORICAL_ALL_ID, sr_strerror(rc));
        goto cleanup;
    }

    rc = lyd_find_xpath(pstSubTrees->tree, acPath, &nodeSet);
    if (LY_SUCCESS != rc) {
        goto cleanup;
    }

    pstNode = nodeSet->dnodes[0];
    pstNode = lyd_node_to_inner(pstNode)->child; /*bin-number*/
    pstMeasurementNode = pstNode->next; /*measurement*/

    pstPmParameterValueNode = lyd_node_to_inner(pstMeasurementNode)->child; /*pmParameterValue*/
    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
    pstLeaf = pstLeaf->next; /*min*/
    pstPmUnit->dMin = atof(lyd_get_value(pstLeaf));

    pstPmParameterValueNode = pstPmParameterValueNode->next;
    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
    pstLeaf = pstLeaf->next;
    pstPmUnit->dMax = atof(lyd_get_value(pstLeaf));

    pstPmParameterValueNode = pstPmParameterValueNode->next;
    pstLeaf = lyd_node_to_inner(pstPmParameterValueNode)->child; /*type*/
    pstLeaf = pstLeaf->next;
    pstPmUnit->dAvg = atof(lyd_get_value(pstLeaf));

    pstLeaf = pstPmParameterValueNode->next; /*pmParameterUnit*/
    strncpy(pstPmUnit->acUnit, lyd_get_value(pstLeaf), PM_STRING_LEN - 1);

    pstLeaf = pstMeasurementNode->next;
    strncpy(pstPmUnit->acTime, lyd_get_value(pstLeaf), PM_STRING_LEN - 1);

cleanup:
    if (pstSubTrees != NULL) {
        sr_release_data(pstSubTrees);
        pstSubTrees = NULL;
    }

    if (nodeSet != NULL) {
        ly_set_free(nodeSet, NULL);
    }

    return rc;
}

int
DSC_PmCollection(char *pcFileName, char *pcType, char *pcInterval)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcFileName || NULL == pcType || NULL == pcInterval || NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        return rc;
    }

    /* allocate input values filename*/
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error. %s", sr_strerror(rc));
        return rc;
    }

    /* set 'input/filename leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_PM_COLLECTION_FILENAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PM_COLLECTION_FILENAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pcFileName);

    rc = sr_val_set_xpath(&input[1], RPC_XPATH_PM_COLLECTION_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PM_COLLECTION_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_ENUM_T;
    input[1].data.enum_val = strdup(pcType);

    rc = sr_val_set_xpath(&input[2], RPC_XPATH_PM_COLLECTION_INTERVAL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PM_COLLECTION_INTERVAL, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_ENUM_T;
    input[2].data.enum_val = strdup(pcInterval);

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, RPC_XPATH_PM_COLLECTION, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_PM_COLLECTION, sr_strerror(rc));
        goto ERR_LABEL;
    }
    /**path + filename***/
    strcpy(pcFileName, output[2].data.string_val);

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);
ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

int
DSC_PmGetIdKey(char *pcXpath, char *pcKey, size_t nMaxLen)
{
    char *pcKeyId = NULL;
    sr_xpath_ctx_t stCtxState;
    if (NULL == pcXpath || NULL == pcKey) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return OPLK_ERR;
    }
    memset(&stCtxState, 0, sizeof(stCtxState));
    pcKeyId = sr_xpath_key_value(pcXpath, "historicalPm", "id", &stCtxState);
    if (NULL == pcKeyId) {
        return OPLK_ERR;
    }
    strncpy(pcKey, pcKeyId, nMaxLen);
    sr_xpath_recover(&stCtxState);
    return OPLK_OK;
}

int
DSC_SetOcPmThreshold(sr_session_ctx_t *pstSess, OC_PM_POINT_ST *pstPmPoint)
{
    int rc = SR_ERR_OK;
    char acXpath[DEF_BUF_SIZE_512] = {0};
    uint32_t i = 0, j = 0;
    OC_PM_SET_ST *pstPmSet = NULL;
    OC_PM_THR_ST *pstPmThr = NULL;
    sr_session_ctx_t *pstSetSess = NULL;
    char acNumberStr[32] = {0};

    pstSetSess = (NULL == pstSess) ? DSC_GetRunningSession() : pstSess;

    if (NULL == pstPmPoint || NULL == pstSetSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        return rc;
    }

    for (i = 0; i < pstPmPoint->stSetList.uiTotal; i++) {

        if (pstPmPoint->stSetList.uiTotal > PM_SET_NUMBER_MAX) {
            rc = SR_ERR_INTERNAL;
            LOG_WRITE(EN_LOG_ERROR, "the number of pm-set is over range: %d", pstPmPoint->stSetList.uiTotal);
            goto END_LABEL;
        }

        pstPmSet = pstPmPoint->stSetList.astPmSet + i;
        for (j = 0; j < pstPmSet->stThrList.uiTotal; j++) {

            if (pstPmSet->stThrList.uiTotal > PM_THR_NUMBER_MAX) {
                rc = SR_ERR_INTERNAL;
                LOG_WRITE(EN_LOG_ERROR, "the number of pm-threshold is over range: %d", pstPmSet->stThrList.uiTotal);
                goto END_LABEL;
            }

            pstPmThr = pstPmSet->stThrList.astThr + j;
            snprintf(acXpath, sizeof(acXpath), OC_PM_XPATH_PM_THR_LIST "/%s",
                     pstPmPoint->acEntityName,
                     pstPmSet->acTimePeriod,
                     pstPmSet->acType,
                     pstPmThr->acPara,
                     OC_PM_NODE_PM_HIGH_THR);

            /* LOG_WRITE(EN_LOG_NOTICE, "[%d, %d] %s", i, j, acXpath); */
            snprintf(acNumberStr, sizeof(acNumberStr), "%.2f", pstPmThr->dHighThr);
            rc = set_string_item(pstSetSess, acXpath, acNumberStr, sizeof(acNumberStr));
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
                goto END_LABEL;
            }

            snprintf(acXpath, sizeof(acXpath), OC_PM_XPATH_PM_THR_LIST "/%s",
                     pstPmPoint->acEntityName,
                     pstPmSet->acTimePeriod,
                     pstPmSet->acType,
                     pstPmThr->acPara,
                     OC_PM_NODE_PM_LOW_THR);
            snprintf(acNumberStr, sizeof(acNumberStr), "%.2f", pstPmThr->dLowThr);
            rc = set_string_item(pstSetSess, acXpath, acNumberStr, sizeof(acNumberStr));
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
                goto END_LABEL;
            }
        }
    }

    rc = DSC_Commit(pstSetSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by setting pm threshold: %s", sr_strerror(rc));
    }

END_LABEL:
    return rc;
}


int
DSC_GetEntityIdx(char *pcEntityName)
{
    size_t nCnt = 0, i = 0;

    if (NULL == pcEntityName) {
        return EN_ENTITY_MAX;
    }

    nCnt = sizeof(m_astEntityTable) / sizeof(m_astEntityTable[0]);

    for (i = 0; i < nCnt; i++) {

        if (0 == strcmp(pcEntityName, m_astEntityTable[i].acName)) {
            return m_astEntityTable[i].enIdx;
        }
    }

    return EN_ENTITY_MAX;
}

int
DSC_SetOcPmThresholdOfOlp(sr_session_ctx_t *pstSess, ENTITY_IDX_EN enEntityIdx, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    OC_PM_POINT_ST stPmPoint;

    memset(&stPmPoint, 0, sizeof(stPmPoint));

    switch (enEntityIdx) {
    case EN_ENTITY_IN65:
        snprintf(stPmPoint.acEntityName, sizeof(stPmPoint.acEntityName), "%s", PM_ENTITY_NAME_IN65_STR);
        break;
    case EN_ENTITY_IN66:
        snprintf(stPmPoint.acEntityName, sizeof(stPmPoint.acEntityName), "%s", PM_ENTITY_NAME_IN66_STR);
        break;
    default:
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid enEntityIdx: %d", enEntityIdx);
        goto END_LABEL;
    }


    stPmPoint.stSetList.uiTotal = 1;
    snprintf(stPmPoint.stSetList.astPmSet[0].acTimePeriod, sizeof(stPmPoint.stSetList.astPmSet[0].acTimePeriod), "%s", OC_PM_NOT_APPLICABLE_STR);
    snprintf(stPmPoint.stSetList.astPmSet[0].acType, sizeof(stPmPoint.stSetList.astPmSet[0].acType), "%s", OC_PM_TYPE_OPTICAL_POWER_STR);

    stPmPoint.stSetList.astPmSet[0].stThrList.uiTotal = 1;
    snprintf(stPmPoint.stSetList.astPmSet[0].stThrList.astThr[0].acPara, sizeof(stPmPoint.stSetList.astPmSet[0].stThrList.astThr[0].acPara), "%s", OC_PM_PARA_OPR_INST_STR);
    stPmPoint.stSetList.astPmSet[0].stThrList.astThr[0].dHighThr = dHighThr;

    stPmPoint.stSetList.astPmSet[0].stThrList.astThr[0].dLowThr = dLowThr;

    rc = DSC_SetOcPmThreshold(pstSess, &stPmPoint);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetOcPmThreshold, data: %d, %.2f, %.2f", enEntityIdx, dHighThr, dLowThr);
    }

END_LABEL:

    return rc;
}

int
DSC_GetPmIdList(char *pcMoudleName, char *pcSubName, PM_ID_LIST_ST *pstPmIdList, PM_ID_LIST_ST *pstSubPmIdList)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tCount = 0;
    int i = 0;
    char acPackName[16] = {0};
    char acPath[128] = {0};

    if (NULL == pcMoudleName || NULL == pstPmIdList) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcMoudleName || NULL == pstPmIdList is NULL.");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pstSess = DSC_GetSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "get sess failed!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = sr_get_items(pstSess, PM_XPATH_CURRENT_ID, 0, SR_OPER_DEFAULT, &pstValues, &tCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tCount; i++) {
        sprintf(acPath, PM_XPATH_CURRENT_CIRCUIT_PACK_NAME, pstValues[i].data.string_val);
        get_string_item(pstSess, acPath, acPackName, 32);
        if (strstr(acPackName, pcMoudleName)) {
            if (pcSubName && strstr(pstValues[i].data.string_val, pcSubName)) {
                strcpy(pstSubPmIdList->aacIdList[pstSubPmIdList->uiCnt++], pstValues[i].data.string_val);
            } else {
                strcpy(pstPmIdList->aacIdList[pstPmIdList->uiCnt++], pstValues[i].data.string_val);
            }
        }
    }

    sr_free_values(pstValues, tCount);
    return SR_ERR_OK;

ERR_LABEL:

    return rc;
}

int
DSC_ClearPm(char *pcType, char *pcGranularity)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pcType || NULL == pcGranularity || NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        return rc;
    }

    /* allocate input values */
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error. %s", sr_strerror(rc));
        return rc;
    }

    /* set 'input/resourceType/type leaf */
    rc = sr_val_set_xpath(&input[0], RPC_XPATH_CLEAR_PM_RESOURCE_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_CLEAR_PM_RESOURCE_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_ENUM_T;
    input[0].data.string_val = strdup("port");

    /* set 'input/pm-type leaf */
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_CLEAR_PM_TYPE);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_CLEAR_PM_TYPE, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_ENUM_T;
    input[1].data.enum_val = strdup(pcType);

    /* set 'input/granularity leaf */
    rc = sr_val_set_xpath(&input[2], RPC_XPATH_CLEAR_PM_GRANULARITY);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_CLEAR_PM_GRANULARITY, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[2].type = SR_ENUM_T;
    input[2].data.enum_val = strdup(pcGranularity);

    /* execute RPC */
    rc = DSC_RpcSend(pstSess, RPC_XPATH_CLEAR_PM, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_CLEAR_PM, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);
    return DSC_ProcessRPCResponse(output, output_cnt);
ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;
}

