/**
 * Copyright (c) Oplink Communications, LLC
 */

/* For use strcasestr */
#define _GNU_SOURCE  /* clear strcasestr compile warning msg */
#include <string.h>

#include "DSC_tca.h"
#include "DSC_log.h"
#include "DSC_edfa.h"
#include "DSC_osc.h"
#include "DSC_ocsystem.h"

static TCA_INFO_ST m_astTca[TCA_IDX_MAX] = {{0}};

/* This list should be same as the yang-model/oplink/fmpm/ */
/*  org-openroadm-tca-startup-[product].xml                */
static int
__TcaNameToIdx(char *pcTcaName, int *pIdx)
{
    int i = 0;
    int num = 0;
    char acName[TCA_NAME_MAX_LEN];
    char *pcItem = NULL;
    char *pcTemp = NULL;

    /* for translate name to id */
    ST_TCA_TRANS astTrans[] = {
        {TCA_IDX_EDFA1_INPUT, "PA_INPUT_LOS"},
        {TCA_IDX_EDFA1_OUTPUT, "PA_OUTPUT_LOS"},
        {TCA_IDX_PA_OUT_CHANNEL, "PA_OUT_CHANNEL"},

        {TCA_IDX_EDFA2_INPUT, "BA_INPUT_LOS"},
        {TCA_IDX_EDFA2_OUTPUT, "BA_OUTPUT_LOS"},
        {TCA_IDX_BA_P_IN_CHANNEL, "BA_IN_CHANNEL"},

        {TCA_IDX_EDFA3_INPUT, "PA-P_INPUT_LOS"},
        {TCA_IDX_EDFA3_OUTPUT, "PA-P_OUTPUT_LOS"},
        {TCA_IDX_PA_P_OUT_CHANNEL, "PA-P_OUT_CHANNEL"},

        {TCA_IDX_EDFA4_INPUT, "BA-P_INPUT_LOS"},
        {TCA_IDX_EDFA4_OUTPUT, "BA-P_OUTPUT_LOS"},
        {TCA_IDX_BA_P_IN_CHANNEL, "BA-P_IN_CHANNEL"},

        {TCA_IDX_OSC1_ADD, "OSC_ADD_LOS"},
        {TCA_IDX_OSC1_DROP, "OSC_DROP_LOS"},
        {TCA_IDX_OSC1_TX, "OSC_TX_LOS"},
        {TCA_IDX_OSC1_RX, "OSC_RX_LOS"},

        {TCA_IDX_OSC2_ADD, "OSC-P_ADD_LOS"},
        {TCA_IDX_OSC2_DROP, "OSC-P_DROP_LOS"},
        {TCA_IDX_OSC2_TX, "OSC-P_TX_LOS"},
        {TCA_IDX_OSC2_RX, "OSC-P_RX_LOS"},

        {TCA_IDX_CV_REF, "CV_REF"},
        {TCA_IDX_CV_MUX_R, "CV_MUX_R"},
        {TCA_IDX_CV_MUX_T, "CV_MUX_T"},
        {TCA_IDX_CV_CLIENT_W_R, "CV_CLIENT_W_R"},
        {TCA_IDX_CV_CLIENT_W_T, "CV_CLIENT_W_T"},
        {TCA_IDX_CV_CLIENT_P_R, "CV_CLIENT_P_R"},
        {TCA_IDX_CV_CLIENT_P_T, "CV_CLIENT_P_T"},

        {TCA_IDX_SPANLOSS, "SPANLOSS"},
        {TCA_IDX_SPANLOS_P, "Line-P_SPANLOSS"},

        {TCA_IDX_TEMP_HIGH_ALM, "TOA_CHASSIS_TEMPERATURE_HIGH_ALARM"},
        {TCA_IDX_TEMP_HIGH_WARN, "TOA_CHASSIS_TEMPERATURE_HIGH_WARNING"},
        {TCA_IDX_TEMP_LOW_ALM, "TOA_CHASSIS_TEMPERATURE_LOW_ALARM"},
        {TCA_IDX_TEMP_LOW_WARN, "TOA_CHASSIS_TEMPERATURE_LOW_WARNING"},

        {TCA_IDX_CPU_LOAD_HIGH, "TOA_CPU_LOAD_HIGH"},
        {TCA_IDX_MEMORY_RESIDUAL_LOW, "TOA_RESIDUAL_MEMORY_LOW"},

        {TCA_IDX_AC_VOL_LOW, "PSU_AC_VOL_LOW"},
        {TCA_IDX_AC_VOL_HIGH, "PSU_AC_VOL_HIGH"},
        {TCA_IDX_DC_VOL_LOW, "PSU_DC_VOL_LOW"},
        {TCA_IDX_DC_VOL_HIGH, "PSU_DC_VOL_HIGH"},

        {TCA_IDX_LINEIN_LOS, "LineIn_LOS"},
        {TCA_IDX_LINEIN_LOW, "LineIn_RX_POWER_LOW"},
        {TCA_IDX_LINEIN_HIGH, "LineIn_RX_POWER_HIGH"},
        {TCA_IDX_LINEIN_P_LOS, "LineIn-P_LOS"},
        {TCA_IDX_LINEIN_P_LOW, "LineIn-P_RX_POWER_LOW"},
        {TCA_IDX_LINEIN_P_HIGH, "LineIn-P_RX_POWER_HIGH"},

        {TCA_IDX_LINET_LOS, "LINET_LOS"},
        {TCA_IDX_LINET_P_LOS, "LINET-P_LOS"},

        {TCA_IDX_CLIENTT_LOS, "CLIENTT_LOS"},
        {TCA_IDX_CLIENTT_P_LOS, "CLIENTT-P_LOS"},

        {TCA_IDX_CLIENT_RX_LOW, "CLIENT_RX_POWER_LOW"},
        {TCA_IDX_CLIENT_RX_HIGH, "CLIENT_RX_POWER_HIGH"},
        {TCA_IDX_CLIENT_P_RX_LOW, "CLIENT-P_RX_POWER_LOW"},
        {TCA_IDX_CLIENT_P_RX_HIGH, "CLIENT-P_RX_POWER_HIGH"},

        {TCA_IDX_CLIENT_TX_LOW, "CLIENT_TX_POWER_LOW"},
        {TCA_IDX_CLIENT_TX_HIGH, "CLIENT_TX_POWER_HIGH"},
        {TCA_IDX_CLIENT_P_TX_LOW, "CLIENT-P_TX_POWER_LOW"},
        {TCA_IDX_CLIENT_P_TX_HIGH, "CLIENT-P_TX_POWER_HIGH"},

        {TCA_IDX_LINE_RX_LOW, "LINE_RX_POWER_LOW"},
        {TCA_IDX_LINE_RX_HIGH, "LINE_RX_POWER_HIGH"},
        {TCA_IDX_LINE_P_RX_LOW, "LINE-P_RX_POWER_LOW"},
        {TCA_IDX_LINE_P_RX_HIGH, "LINE-P_RX_POWER_HIGH"},

        {TCA_IDX_LINE_TX_LOW, "LINE_TX_POWER_LOW"},
        {TCA_IDX_LINE_TX_HIGH, "LINE_TX_POWER_HIGH"},
        {TCA_IDX_LINE_P_TX_LOW, "LINE-P_TX_POWER_LOW"},
        {TCA_IDX_LINE_P_TX_HIGH, "LINE-P_TX_POWER_HIGH"},

        {TCA_IDX_BA_CHANNELIN_LOW, "BA_CHANNELIN_POWER_LOW"},
        {TCA_IDX_BA_CHANNELIN_HIGH, "BA_CHANNELIN_POWER_HIGH"},
        {TCA_IDX_BA_P_CHANNELIN_LOW, "BA-P_CHANNELIN_POWER_LOW"},
        {TCA_IDX_BA_P_CHANNELIN_HIGH, "BA-P_CHANNELIN_POWER_HIGH"},

        {TCA_IDX_BA_CHANNELOUT_LOW, "BA_CHANNELOUT_POWER_LOW"},
        {TCA_IDX_BA_CHANNELOUT_HIGH, "BA_CHANNELOUT_POWER_HIGH"},
        {TCA_IDX_BA_P_CHANNELOUT_LOW, "BA-P_CHANNELOUT_POWER_LOW"},
        {TCA_IDX_BA_P_CHANNELOUT_HIGH, "BA-P_CHANNELOUT_POWER_HIGH"},

        {TCA_IDX_PA_CHANNELIN_LOW, "PA_CHANNELIN_POWER_LOW"},
        {TCA_IDX_PA_CHANNELIN_HIGH, "PA_CHANNELIN_POWER_HIGH"},
        {TCA_IDX_PA_P_CHANNELIN_LOW, "PA-P_CHANNELIN_POWER_LOW"},
        {TCA_IDX_PA_P_CHANNELIN_HIGH, "PA-P_CHANNELIN_POWER_HIGH"},

        {TCA_IDX_PA_CHANNELOUT_LOW, "PA_CHANNELOUT_POWER_LOW"},
        {TCA_IDX_PA_CHANNELOUT_HIGH, "PA_CHANNELOUT_POWER_HIGH"},
        {TCA_IDX_PA_P_CHANNELOUT_LOW, "PA-P_CHANNELOUT_POWER_LOW"},
        {TCA_IDX_PA_P_CHANNELOUT_HIGH, "PA-P_CHANNELOUT_POWER_HIGH"},

        {TCA_IDX_OSC_TX_LOW, "OSC_TX_POWER_LOW"},
        {TCA_IDX_OSC_TX_HIGH, "OSC_TX_POWER_HIGH"},
        {TCA_IDX_OSC_P_TX_LOW, "OSC-P_TX_POWER_LOW"},
        {TCA_IDX_OSC_P_TX_HIGH, "OSC-P_TX_POWER_HIGH"},

        {TCA_IDX_OSC_RX_LOW, "OSC_RX_POWER_LOW"},
        {TCA_IDX_OSC_RX_HIGH, "OSC_RX_POWER_HIGH"},
        {TCA_IDX_OSC_P_RX_LOW, "OSC-P_RX_POWER_LOW"},
        {TCA_IDX_OSC_P_RX_HIGH, "OSC-P_RX_POWER_HIGH"},

        {TCA_IDX_OSC_ADD_LOW, "OSC_ADD_POWER_LOW"},
        {TCA_IDX_OSC_ADD_HIGH, "OSC_ADD_POWER_HIGH"},
        {TCA_IDX_OSC_P_ADD_LOW, "OSC-P_ADD_POWER_LOW"},
        {TCA_IDX_OSC_P_ADD_HIGH, "OSC-P_ADD_POWER_HIGH"},

        {TCA_IDX_OSC_DROP_LOW, "OSC_DROP_POWER_LOW"},
        {TCA_IDX_OSC_DROP_HIGH, "OSC_DROP_POWER_HIGH"},
        {TCA_IDX_OSC_P_DROP_LOW, "OSC-P_DROP_POWER_LOW"},
        {TCA_IDX_OSC_P_DROP_HIGH, "OSC-P_DROP_POWER_HIGH"},

        {TCA_IDX_SPANLOSS_VARIATION_LOW, "LINE_SPANLOSS_VARIATION_LOW"},
        {TCA_IDX_SPANLOSS_VARIATION_HIGH, "LINE_SPANLOSS_VARIATION_HIGH"},
        {TCA_IDX_SPANLOSS_P_VARIATION_LOW, "LINE-P_SPANLOSS_VARIATION_LOW"},
        {TCA_IDX_SPANLOSS_P_VARIATION_HIGH, "LINE-P_SPANLOSS_VARIATION_HIGH"},

        {TCA_IDX_BA_PUMP1_CURRENT_HIGH, "BA_PUMP1_CURRENT_HIGH"},
        {TCA_IDX_BA_PUMP2_CURRENT_HIGH, "BA_PUMP2_CURRENT_HIGH"},
        {TCA_IDX_BA_PUMP3_CURRENT_HIGH, "BA_PUMP3_CURRENT_HIGH"},
        {TCA_IDX_BA_P_PUMP1_CURRENT_HIGH, "BA-P_PUMP1_CURRENT_HIGH"},
        {TCA_IDX_BA_P_PUMP2_CURRENT_HIGH, "BA-P_PUMP2_CURRENT_HIGH"},
        {TCA_IDX_BA_P_PUMP3_CURRENT_HIGH, "BA-P_PUMP3_CURRENT_HIGH"},

        {TCA_IDX_PA_PUMP1_CURRENT_HIGH, "PA_PUMP1_CURRENT_HIGH"},
        {TCA_IDX_PA_PUMP2_CURRENT_HIGH, "PA_PUMP2_CURRENT_HIGH"},
        {TCA_IDX_PA_P_PUMP1_CURRENT_HIGH, "PA-P_PUMP1_CURRENT_HIGH"},
        {TCA_IDX_PA_P_PUMP2_CURRENT_HIGH, "PA-P_PUMP2_CURRENT_HIGH"},

        {TCA_IDX_LASERSAFETY_RX_LOS, "LASERSAFETY_RX_LOS"},
        {TCA_IDX_LASERSAFETY_P_RX_LOS, "LASERSAFETY-P_RX_LOS"},
        {TCA_IDX_LASERSAFETY_TX_LOS, "LASERSAFETY_TX_LOS"},
        {TCA_IDX_LASERSAFETY_P_TX_LOS, "LASERSAFETY-P_TX_LOS"},
        {TCA_IDX_PA_RFL, "PA_OUT_REFLECTION"},
        {TCA_IDX_BA_RFL, "BA_OUT_REFLECTION"},

        {TCA_IDX_OPS_COM_IN_LOS, "OLP_COM_IN_LOS"},
        {TCA_IDX_OPS_POWER_DIFFER, "OLP_POWER_DIFFER"},

        {TCA_IDX_OPS_COM_IN_LOW, "OLP_COM_RX_POWER_LOW"},
        {TCA_IDX_OPS_COM_IN_HIGH, "OLP_COM_RX_POWER_HIGH"},
        {TCA_IDX_OPS_PRI_IN_LOW, "OLP_PRI_RX_POWER_LOW"},
        {TCA_IDX_OPS_PRI_IN_HIGH, "OLP_PRI_RX_POWER_HIGH"},
        {TCA_IDX_OPS_SEC_IN_LOW, "OLP_SEC_RX_POWER_LOW"},
        {TCA_IDX_OPS_SEC_IN_HIGH, "OLP_SEC_RX_POWER_HIGH"},

        {TCA_IDX_MAX, ""}
    };

    if (NULL == pcTcaName || NULL == pIdx) {
        goto ERR_LABEL;
    }

    for (i = 0; i < (sizeof(astTrans) / sizeof(ST_TCA_TRANS)); i++) {
        /* "i" is not consecutive, 1,2,4,5,8,9,11,12,14,15,...,28 */
        /*  so "LINE-P_SPANLOSS_VARIATION_HIGH" is NOT found */
        /* BUG: strlen(astTrans[i].acId) is undefined length */
        /* if (0 == strncasecmp(pcTcaName, astTrans[i].acId, strlen(astTrans[i].acId))) { */
        if (0 == strncasecmp(pcTcaName, astTrans[i].acId, strlen(pcTcaName))) {
            *pIdx = astTrans[i].iIdx;
            return SR_ERR_OK;
        }
    }

    /************************************************************************************/
    /* find multiple ops tca id dynamically                                             */
    strncpy(acName, pcTcaName, sizeof(acName));
    if (strcasestr(acName, OPS_CFG_KEY) == acName) {
        pcItem = acName + strlen(OPS_CFG_KEY);
        pcTemp = strchr(pcItem, '_');
        if (NULL == pcTemp) {
            goto ERR_LABEL;
        }
        *pcTemp = 0;
        num = atoi(pcItem);
        if (num < 1 || num > OPS_DEV_NUM_MAX) {
            goto ERR_LABEL;
        }

        ++pcTemp;
        for (i = sizeof(astTrans) / sizeof(ST_TCA_TRANS) - 1; i >= 0; i--) {
            if (NULL != strcasestr(astTrans[i].acId, pcTemp)) {
                *pIdx = astTrans[i].iIdx + num * ((TCA_IDX_OPS_END - TCA_IDX_OPS_START + 1) / (OPS_DEV_NUM_MAX + 1));
                return SR_ERR_OK;
            }
        }
    }
    /************************************************************************************/

ERR_LABEL:
    /* LOG_WRITE(EN_LOG_ERROR, "TCA name: %s to idx error", pcTcaName); */
    return SR_ERR_OPERATION_FAILED;
}


int
DSC_GetTcaList(sr_session_ctx_t *sess, TCA_INFO_ST **ppstTca, size_t *count)
{
    int rc = SR_ERR_OK;
    sr_val_t *val = NULL;
    size_t cnt = 0;
    TCA_INFO_ST *pstTca = (TCA_INFO_ST *)&m_astTca;
    int i = 0, j = 0;

    if (NULL == ppstTca || NULL == count) {
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == sess) {
        sess = DSC_GetRunningSession();
    }

    /* get all list instances with their content (recursive) */
    rc = get_items(sess, TCA_XPATH_ALL, &val, &cnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), TCA_XPATH_ALL);
        if (val != NULL)
            sr_free_values(val, cnt);
        *count = 0;
        return SR_ERR_OK;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        *count = 0;
        return rc;
    }

    j = -1;
    for (i = 0; i < cnt; i++) {
        if (strstr(val[i].xpath, TCA_ID_LEAF) != NULL) {
            j++;
            if (j >= TCA_IDX_MAX) {
                break;
            }
            strncpy(pstTca[j].acId, val[i].data.string_val, sizeof(pstTca[j].acId) - 1);
            __TcaNameToIdx(pstTca[j].acId, &pstTca[j].iIdx);
        } else if (strstr(val[i].xpath, TCA_LOW_VALUE_LEAF) != NULL) {
            pstTca[j].dLowThr = val[i].data.decimal64_val;
        } else if (strstr(val[i].xpath, TCA_HIGH_VALUE_LEAF) != NULL) {
            pstTca[j].dHighThr = val[i].data.decimal64_val;
        } else if (strstr(val[i].xpath, TCA_CP_NAME_LEAF) != NULL) {
            strncpy(pstTca[j].acCpName, val[i].data.string_val, sizeof(pstTca[j].acCpName) - 1);
        } else if (strstr(val[i].xpath, TCA_PORT_NAME_LEAF) != NULL) {
            strncpy(pstTca[j].acPortName, val[i].data.string_val, sizeof(pstTca[j].acPortName) - 1);
        }
    }

    sr_free_values(val, cnt);

    *ppstTca = pstTca;
    *count = j + 1;

    return rc;
}

char *
DSC_GetTcaName(EN_TCA_IDX enIdx)
{
    int i = 0;
    int rc = SR_ERR_OK;
    static bool m_bFlag = false;
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t count = 0;

    if (enIdx < 0 || enIdx >= TCA_IDX_MAX) {
        goto ERR_LABEL;
    }

    if (false == m_bFlag) {
        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK == rc) {
            m_bFlag = true;
        }
    }

    for (i = 0; i < TCA_IDX_MAX; i++) {
        if (m_astTca[i].iIdx == enIdx) {
            LOG_WRITE(EN_LOG_NOTICE, "Get tca: %d, name: %s", enIdx, m_astTca[i].acId);
            return m_astTca[i].acId;
        }
    }

ERR_LABEL:
    return NULL;
}

int
DSC_GetTcaIdx(char *xpath)
{
    int rc = 0;
    int i = 0;
    size_t count = 0;
    TCA_INFO_ST *pstTcaInfo = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    static bool m_bFlag = false;
    char acKeyId[64] = {0};

    if (NULL == xpath) {
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    strncpy(acXpath, xpath, sizeof(acXpath) - 1);

    rc = DSC_GetKeyValue(acXpath, "tca", "id", sizeof(acKeyId), acKeyId);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error sr_xpath_key_value: %s", xpath);
        goto ERR_LABEL;
    }

    if (false == m_bFlag) {
        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK == rc) {
            m_bFlag = true;
        }
    }

    for (i = 0; i < (sizeof(m_astTca) / sizeof(TCA_INFO_ST)); i++) {
        if (0 == strcmp(acKeyId, m_astTca[i].acId)) {
            return m_astTca[i].iIdx;
        }

    }

ERR_LABEL:
    return -1;
}

int
DSC_GetTcaCpNamePortName(int iTcaId, char *pcCpName, char *pcPortName)
{
    int i = 0;

    if (NULL == pcCpName || NULL == pcPortName) {
        goto ERR_LABEL;
    }

    for (i = 0; i < (sizeof(m_astTca) / sizeof(TCA_INFO_ST)); i++) {
        if (iTcaId == m_astTca[i].iIdx) {
            strcpy(pcCpName, m_astTca[i].acCpName);
            strcpy(pcPortName, m_astTca[i].acPortName);
            return SR_ERR_OK;
        }
    }

ERR_LABEL:
    return SR_ERR_NOT_FOUND;
}

int
DSC_SetTcaThreshold(sr_session_ctx_t *pstSess, EN_TCA_IDX enIdx, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    char *pcTcaName = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = NULL;

    if (enIdx < 0 || enIdx >= TCA_IDX_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "input idx %d out of range", enIdx);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    sess = (NULL == pstSess) ? DSC_GetRunningSession() : pstSess;
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    pcTcaName = DSC_GetTcaName(enIdx);
    if (NULL == pcTcaName) {
        LOG_WRITE(EN_LOG_ERROR, "Error not found id %d", enIdx);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_HIGH, pcTcaName);
    /* LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, dHighThr); */

    rc = set_decimal64_item(sess, acXpath, dHighThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_LOW, pcTcaName);
    /* LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, dLowThr); */

    rc = set_decimal64_item(sess, acXpath, dLowThr);
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
DSC_SetTcaThresh(sr_session_ctx_t *pstSess, const char *pcTcaName, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = NULL;

    sess = (NULL == pstSess) ? DSC_GetRunningSession() : pstSess;
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath) - 1, TCA_XPATH_TCA_HIGH, pcTcaName);
    /* LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, HighThr %.2f", acXpath, dHighThr); */

    rc = set_decimal64_item(sess, acXpath, dHighThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, TCA_XPATH_TCA_LOW, pcTcaName);
    /* LOG_WRITE(EN_LOG_NOTICE, "Xpath: %s, LowThr %.2f", acXpath, dLowThr); */

    rc = set_decimal64_item(sess, acXpath, dLowThr);
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
DSC_GetTcaThresh(EN_TCA_IDX enIdx, TCA_INFO_ST *pstInfo, double *pdHighThr, double *pdLowThr)
{
    int i = 0;

    if (enIdx < 0 || enIdx >= TCA_IDX_MAX || NULL == pstInfo
        || NULL == pdHighThr || NULL == pdLowThr) {
        return SR_ERR_INVAL_ARG;
    }

    for (i = 0; i < TCA_IDX_MAX; i++) {
        if (pstInfo[i].iIdx == enIdx) {
            *pdHighThr = pstInfo[i].dHighThr;
            *pdLowThr = pstInfo[i].dLowThr;

            return SR_ERR_OK;
        }
    }

    return SR_ERR_NOT_FOUND;
}

int
DSC_GetTcaIdxFromName(char *pcId, int *iIdx)
{
    int i = 0;
    int rc = SR_ERR_OK;
    static bool m_bFlag = false;
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t count = 0;

    if (NULL == pcId) {
        goto ERR_LABEL;
    }

    if (false == m_bFlag) {
        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK == rc) {
            m_bFlag = true;
        }
    }

    for (i = 0; i < TCA_IDX_MAX; i++) {
        if (0 == strcmp(pcId, m_astTca[i].acId)) {
            *iIdx = m_astTca[i].iIdx;
            return SR_ERR_OK;
        }
    }

ERR_LABEL:
    return SR_ERR_NOT_FOUND;
}


