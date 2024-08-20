/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_api_msft.c
* Description:   REST APIs of handling JSON data, only used for MSFT
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/13  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_api_msft.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "oplk_file.h"
#include "rest_handle.h"

void
API_StartAutoGainCtrl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    char *pcLine = NULL;

    CLEAR_NO_USED_WARNING(pstReq);

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_OPERATIONS_AUTO_GAIN_W)) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    rc = DSC_AutoStartGainCtrl(pcLine);
    if (rc != SR_ERR_OK) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_GetAutoGainCtrlState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    AUTO_GAIN_STATE_ST stAutoGainState;
    int iResult = HTTP_STATUS_CODE_200;
    char acTmp[64] = {0};
    char *pcLine = NULL;

    CLEAR_NO_USED_WARNING(pstReq);

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_OPERATIONS_AUTO_GAIN_W)) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    memset(&stAutoGainState, 0, sizeof(stAutoGainState));

    rc = DSC_AutoGetGainState(pcLine, &stAutoGainState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AutoGetGainState error");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    cJSON_AddNumberToObject(pstBody, STR_SPAN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stAutoGainState.dSpanLoss));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stAutoGainState.dPreAmpGain));
    cJSON_AddNumberToObject(pstBody, STR_PRE_VOA_ATTENUATION, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stAutoGainState.dPreVoaAtten));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stAutoGainState.dBoosterAmpGain));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stAutoGainState.dBoosterVoaAtten));
    cJSON_AddNumberToObject(pstBody, STR_PROGRESS, stAutoGainState.ucProgress);
    memset(acTmp, 0, sizeof(acTmp));
    cJSON_AddStringToObject(pstBody, STR_STATE, DSC_ControlStateToString(acTmp, stAutoGainState.enState));

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_GetRemoteNodeInfo(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    REMOTE_NODE_INFO_ST stRemoteInfo;
    char *pcLine = NULL;

    CLEAR_NO_USED_WARNING(pstReq);

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_REMOTE_NODE_INFO)) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    pstBody = pstResp->pstResponse;

    /* 20200303 add by fandy */
    memset(&stRemoteInfo, 0, sizeof(stRemoteInfo));
    rc = DSC_GetRemoteInfo(pcLine, &stRemoteInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetRemoteInfo error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    } else {
        cJSON_AddStringToObject(pstBody, STR_NODE_NAME, stRemoteInfo.acNodeName);
        cJSON_AddStringToObject(pstBody, STR_IP_ADDRESS, stRemoteInfo.acRemoteIp);
        /* cJSON_AddStringToObject(pstBody, STR_OSC_IP_ADDRESS_REMOTE, stRemoteInfo.acRemoteOscIp); */
        /* cJSON_AddStringToObject(pstBody, STR_OSC_IP_ADDRESS_LOCAL, stRemoteInfo.acLocalOscIp); */
    }

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

static int
__ParseSpanlossThreshold(const cJSON *pstJson, double *pdSpanlossThreshold)
{
    double dSpanlossThreshold = 0.0;
    JSON_OBJ_ST astJsonObj[] = {{SPANLOSS_THRESHOLD_NODE, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(dSpanlossThreshold), sizeof(dSpanlossThreshold)},

                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }
#if 0
    /* check spanloss threshold range */
    if (dSpanlossThreshold < SPANLOSS_THRESHOLD_MIN
        || dSpanlossThreshold > SPNALOSS_THRESHOLD_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Spanloss threshold is out range. %d", dSpanlossThreshold);
        goto ERR_LABEL;
    }
#endif
    memcpy(pdSpanlossThreshold, &dSpanlossThreshold, sizeof(dSpanlossThreshold));


    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

void
API_SetSpanlossThreshold(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    double dSpanlossThreshold = 0.0;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    char *pcLine = NULL;

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if ((NULL == pstBody) || (!cJSON_IsObject(pstBody))) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != __ParseSpanlossThreshold((const cJSON *)pstBody, &dSpanlossThreshold)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_THRESHOLD)) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    /*set data*/

    rc = DSC_AutoSetSpanlossThreshold(pcLine, dSpanlossThreshold);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AutoSetSpanlossThreshold error");
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:
    cJSON_Delete(pstBody);
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_GetSpanlossThreshold(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    double dSpanlossThreshold = 0.0;
    int iResult = HTTP_STATUS_CODE_200;
    char *pcLine = NULL;

    CLEAR_NO_USED_WARNING(pstReq);

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_THRESHOLD)) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    /* get data */
    rc = DSC_AutoGetSpanlossThreshold(pcLine, &dSpanlossThreshold);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_AutoGetSpanlossThreshold error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    /* cJSON_AddNumberToObject(pstBody, STR_SPANLOSS_THRESHOLD, dSpanlossThreshold); */
    cJSON_AddNumberToObject(pstBody, STR_SPANLOSS_THRESHOLD, GET_2_DECIMAL_DIGITS_OF_DOUBLE(dSpanlossThreshold));

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_GetFiberCfgByType(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    char acFiberType[32] = {0};
    char *pcType = NULL;
    int iResult = HTTP_STATUS_CODE_404;
    uint32_t i = 0;
    cJSON *pstBody = NULL;
    cJSON *psJsonObj = NULL;
    FIBER_PARM_LIST_ST stFiberList;
    FIBER_PARM_ST *pstFiberPara = NULL;
    char *pcLine = NULL;

    pcType = strchr(pstReq->acUrl, '=');
    if (NULL == pcType) {
        iResult = HTTP_STATUS_CODE_404;
        goto END_LABEL;
    } else {
        snprintf(acFiberType, sizeof(acFiberType), "%s", pcType + 1);
    }

    if (0 == strncasecmp(pstReq->acUrl, URL_OMT_GET_FIBER_CONFIG_BY_TYPE, strlen(URL_OMT_GET_FIBER_CONFIG_BY_TYPE))) {
        pcLine = WORKING_LINE_YANG;
    } else {
        pcLine = PROTECTION_LINE_YANG;
    }

    memset(&stFiberList, 0, sizeof(stFiberList));
    rc = DSC_CollectFiberConfig(pcLine, acFiberType, &stFiberList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_CollectFiberConfig");
        iResult = (SR_ERR_INVAL_ARG == rc) ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    for (i = 0; i < stFiberList.uiTotal; i++) {
        pstFiberPara = stFiberList.astFiberPara + i;

        memset(acFiberType, 0, sizeof(acFiberType));
        if (NULL == DSC_FiberTypeToString(acFiberType, pstFiberPara->enFiberType)) {
            iResult = HTTP_STATUS_CODE_406;
            LOG_WRITE(EN_LOG_ERROR, "Invalid enFiberType: %d", pstFiberPara->enFiberType);
            goto END_LABEL;
        }
        psJsonObj = cJSON_AddObjectToObject(pstBody, acFiberType);

        cJSON_AddNumberToObject(psJsonObj, DISPERSION_NODE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dDispersion));
        cJSON_AddNumberToObject(psJsonObj, FIBER_LOSS_NODE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dFiberLoss));
        cJSON_AddNumberToObject(psJsonObj, LAUNCHING_POWER_STR_NODE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dLauchingPower));
        cJSON_AddNumberToObject(psJsonObj, SUPPORTED_SPAN_lOSS_NODE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dSupportedSpanLoss));
        cJSON_AddNumberToObject(psJsonObj, SIGNAL_LOSS_TILT_NODE, GET_4_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dSignalLossTilt));
        cJSON_AddNumberToObject(psJsonObj, OSC_LOSS_TILT_NODE, GET_4_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dOscLossTilt));
        cJSON_AddNumberToObject(psJsonObj, SRS_K_NODE, GET_4_DECIMAL_DIGITS_OF_DOUBLE(pstFiberPara->dSrsK));
    }

    iResult = HTTP_STATUS_CODE_200;

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
}

