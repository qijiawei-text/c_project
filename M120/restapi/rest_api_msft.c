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
#include "rest_edfa.h"
#include "rest_handle.h"
#include "DevEdfaCApi.h"

void
API_SetSystem(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_SYSTEM_ST stSystem;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stSystem, 0, sizeof(stSystem));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseSystem((const cJSON *)pstBody, &stSystem)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetSystem(&stSystem);
    if (SR_ERR_OK != rc) {
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
API_SetFullConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_FULLCONFIG_ST stFullConfig;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stFullConfig, 0, sizeof(stFullConfig));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if ((NULL == pstBody) || (!cJSON_IsObject(pstBody))) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseFullConfig((const cJSON *)pstBody, &stFullConfig)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetFullConfig(&stFullConfig);
    if (SR_ERR_OK != rc) {
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
API_GetFullConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    cJSON *psJsonObj = NULL;
    int rc = SR_ERR_OK;
    REST_FULLCONFIG_ST stFullConfig;
    int iResult = HTTP_STATUS_CODE_200;
    char acFiberType[64] = {0};
    REST_AMP_CARD_TYPE_EN enCardType = EN_REST_AMP_CARD_MAX;

    CLEAR_NO_USED_WARNING(pstReq);

    memset(&stFullConfig, 0, sizeof(stFullConfig));

    /* get data */
    rc = REST_GetFullConfig(&stFullConfig);
    if (rc != SR_ERR_OK) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    /*add system*/
    psJsonObj = cJSON_AddObjectToObject(pstBody, STR_SYSTEM);
    cJSON_AddStringToObject(psJsonObj, STR_NODE_NAME, stFullConfig.stSystem.acNodeName);
    cJSON_AddStringToObject(psJsonObj, STR_IP_ADDRESS, stFullConfig.stSystem.acIPV4);
    cJSON_AddStringToObject(psJsonObj, STR_NETMASK, stFullConfig.stSystem.acNetmask);
    cJSON_AddStringToObject(psJsonObj, STR_GATEWAY, stFullConfig.stSystem.acGatewayV4);
    cJSON_AddStringToObject(psJsonObj, STR_NTP_IP_1, stFullConfig.stSystem.aacNtpIp[0]);
    cJSON_AddStringToObject(psJsonObj, STR_NTP_IP_2, stFullConfig.stSystem.aacNtpIp[1]);
    cJSON_AddStringToObject(psJsonObj, STR_SYSLOG_IP_1, stFullConfig.stSystem.aacSyslogIp[0]);
    cJSON_AddStringToObject(psJsonObj, STR_SYSLOG_IP_2, stFullConfig.stSystem.aacSyslogIp[1]);

    cJSON_AddStringToObject(psJsonObj, STR_TACACS_IP_1, stFullConfig.stSystem.astTacacsServer[0].acIp);
    cJSON_AddStringToObject(psJsonObj, STR_TACACS_KEY_1, (0 == strlen(stFullConfig.stSystem.astTacacsServer[0].acIp)) ? "" : "********");
    cJSON_AddStringToObject(psJsonObj, STR_TACACS_IP_2, stFullConfig.stSystem.astTacacsServer[1].acIp);
    cJSON_AddStringToObject(psJsonObj, STR_TACACS_KEY_2, (0 == strlen(stFullConfig.stSystem.astTacacsServer[1].acIp)) ? "" : "********");

#if defined(__OCS__)
    /* clear compile warnning */
    memset(acFiberType, 0, sizeof(acFiberType));
    enCardType = enCardType;
#else
    /* working line */
    cJSON_AddStringToObject(psJsonObj, STR_FIBER_TYPE, DSC_FiberTypeToString(acFiberType, stFullConfig.stSystem.aenFiberType[0]));
    cJSON_AddNumberToObject(psJsonObj, STR_LAUNCHING_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.stSystem.adLauchingPower[0]));
    cJSON_AddNumberToObject(psJsonObj, STR_DROP_POWER_PER_CHANNEL, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.stSystem.adDropPower[0]));
    cJSON_AddNumberToObject(psJsonObj, STR_NO_OF_CARRIERS, stFullConfig.stSystem.aucCarries[0]);
    cJSON_AddStringToObject(psJsonObj, STR_LONG_SPAN, stFullConfig.stSystem.abLongSpan[0] ? STR_ENABLE : STR_DISABLE);

#if defined(__OLSP__)
    /* protect line */
    memset(acFiberType, 0, sizeof(acFiberType));
    cJSON_AddStringToObject(psJsonObj, STR_FIBER_TYPE_P, DSC_FiberTypeToString(acFiberType, stFullConfig.stSystem.aenFiberType[1]));
    cJSON_AddNumberToObject(psJsonObj, STR_LAUNCHING_POWER_P, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.stSystem.adLauchingPower[1]));
    cJSON_AddNumberToObject(psJsonObj, STR_DROP_POWER_PER_CHANNEL_P, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.stSystem.adDropPower[1]));
    cJSON_AddNumberToObject(psJsonObj, STR_NO_OF_CARRIERS_P, stFullConfig.stSystem.aucCarries[1]);
    cJSON_AddStringToObject(psJsonObj, STR_LONG_SPAN_P, stFullConfig.stSystem.abLongSpan[1] ? STR_ENABLE : STR_DISABLE);
#endif
#endif

    cJSON_AddNumberToObject(psJsonObj, STR_TIMEZONE, (int)stFullConfig.stSystem.sTimezone);


    /* 20220214 add by guangjun */
    cJSON_AddStringToObject(psJsonObj, STR_IPV6_MODE, stFullConfig.stSystem.acIpv6Mode);
    cJSON_AddStringToObject(psJsonObj, STR_IPV6_ADDRESS, stFullConfig.stSystem.acIPv6);
    cJSON_AddStringToObject(psJsonObj, STR_IPV6_GATEWAY, stFullConfig.stSystem.acGatewayV6);
#if defined(__OCS__)

#else
    for (enCardType = EN_REST_AMP_W_CARD; enCardType < REST_GetAmpCardCount(); enCardType++) {

        /*add BA*/
        psJsonObj = cJSON_AddObjectToObject(pstBody, (EN_REST_AMP_W_CARD == enCardType) ? STR_BOOSTER_AMP : STR_BOOSTER_AMP_P);
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaBA[enCardType].dGain));
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_GAIN_TILT, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaBA[enCardType].dGainTilt));
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaBA[enCardType].dVoa));
        cJSON_AddStringToObject(psJsonObj, STR_APR_ENABLE, stFullConfig.astEdfaBA[enCardType].bAprEnable ? STR_ENABLE : STR_DISABLE);
        cJSON_AddStringToObject(psJsonObj, STR_STATE, stFullConfig.astEdfaBA[enCardType].bState ? STR_UP : STR_DOWN);

        /*add PA*/
        psJsonObj = cJSON_AddObjectToObject(pstBody, (EN_REST_AMP_W_CARD == enCardType) ? STR_PRE_AMP : STR_PRE_AMP_P);
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaPA[enCardType].dGain));
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_GAIN_TILT, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaPA[enCardType].dGainTilt));
        cJSON_AddNumberToObject(psJsonObj, STR_TARGET_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stFullConfig.astEdfaPA[enCardType].dVoa));
        cJSON_AddStringToObject(psJsonObj, STR_APR_ENABLE, stFullConfig.astEdfaPA[enCardType].bAprEnable ? STR_ENABLE : STR_DISABLE);
        cJSON_AddStringToObject(psJsonObj, STR_STATE, stFullConfig.astEdfaPA[enCardType].bState ? STR_UP : STR_DOWN);

    }
#endif
END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_GetInventory(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstJson = NULL;
    REST_INVENTORY_ST stInventory;
    int iResult = HTTP_STATUS_CODE_200;

    CLEAR_NO_USED_WARNING(pstReq);

    /* get data */
    memset(&stInventory, 0, sizeof(stInventory));

    if (SR_ERR_OK != REST_GetInventory(&stInventory)) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    /* response data */
    pstJson = pstResp->pstResponse;
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_MODULE, stInventory.acModuleSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_MODULE, stInventory.acModulePN);
    cJSON_AddStringToObject(pstJson, STR_SOFTWARE_VERSION_MODULE, stInventory.acModuleSoftVer);
    cJSON_AddStringToObject(pstJson, STR_VENDOR, stInventory.acVendor);

    /* SCC */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_SCC, stInventory.stScc.acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_SCC, stInventory.stScc.acPN);

#if defined(__OCS__)
    /* OCS */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_OCS, stInventory.stOcs.acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_OCS, stInventory.stOcs.acPN);
#elif defined(__OLSP__)

    /* OPS */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_OPS, stInventory.stOps.acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_OPS, stInventory.stOps.acPN);

    /* AMP */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_AMP, stInventory.astAmp[EN_REST_AMP_W_CARD].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_AMP, stInventory.astAmp[EN_REST_AMP_W_CARD].acPN);

    /* AMP-P */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_AMP_P, stInventory.astAmp[EN_REST_AMP_P_CARD].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_AMP_P, stInventory.astAmp[EN_REST_AMP_P_CARD].acPN);
#elif defined(__HCF__)
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_OPTICAL, stInventory.astAmp[EN_REST_AMP_W_CARD].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_OPTICAL, stInventory.astAmp[EN_REST_AMP_W_CARD].acPN);
#endif

    /* Power */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_POWER_A, stInventory.astPower[POWER_A_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_POWER_A, stInventory.astPower[POWER_A_INDEX].acPN);
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_POWER_B, stInventory.astPower[POWER_B_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_POWER_B, stInventory.astPower[POWER_B_INDEX].acPN);
    /* FAN */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_FAN_A, stInventory.astFAN[FAN_A_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_FAN_A, stInventory.astFAN[FAN_A_INDEX].acPN);
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_FAN_B, stInventory.astFAN[FAN_B_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_FAN_B, stInventory.astFAN[FAN_B_INDEX].acPN);
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_FAN_C, stInventory.astFAN[FAN_C_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_FAN_C, stInventory.astFAN[FAN_C_INDEX].acPN);
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_FAN_D, stInventory.astFAN[FAN_D_INDEX].acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_FAN_D, stInventory.astFAN[FAN_D_INDEX].acPN);

#if defined(__OCS__)
#else
    /* MUX */
    cJSON_AddStringToObject(pstJson, STR_SERIAL_NUMBER_MUX, stInventory.stMux.acSN);
    cJSON_AddStringToObject(pstJson, STR_PART_NUMBER_MUX, stInventory.stMux.acPN);
#endif

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_Restart(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_RESTART_ST stRestart;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    char *pcResetType = NULL;

    memset(&stRestart, 0, sizeof(stRestart));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }


    /*parse data*/
    if (OPLK_OK != REST_ParseRestart((const cJSON *)pstBody, &stRestart)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_OPERATIONS_COLDRESTART)) {
        pcResetType = COLD_RESTART_STR;
    } else {
        pcResetType = WARM_RESTART_STR;
    }

    /*set data*/
    rc = REST_CommonRestart(&stRestart, pcResetType);
    if (SR_ERR_OK != rc) {
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
API_GetAllSlotsStatus(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    cJSON *psJsonObj = NULL;
    int rc = SR_ERR_OK;
    ST_SLOT *pstSlotsInfo = NULL;
    size_t nSlotCount = 0;
    int i = 0;
    int iResult = HTTP_STATUS_CODE_200;

    CLEAR_NO_USED_WARNING(pstReq);

    rc = DSC_SHELF_GetSlots(&pstSlotsInfo, &nSlotCount);
    if (rc != SR_ERR_OK) {
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    for (i = 0; i < nSlotCount; i++) {
        psJsonObj = cJSON_AddObjectToObject(pstBody, pstSlotsInfo[i].acName);
        cJSON_AddStringToObject(psJsonObj, "label", pstSlotsInfo[i].acLabel);
        cJSON_AddStringToObject(psJsonObj, "status",
                                (0 == strcasecmp(pstSlotsInfo[i].acSlotStatus, SLOT_STATUS_INSTALL_MATCH_STR)) ? "online" : "offline");
    }

END_LABEL:
    DEF_SAFE_FREE(pstSlotsInfo);
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}


void
API_TransferFile(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_TRANSFER_FILE_ST stTransferFile;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stTransferFile, 0, sizeof(stTransferFile));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseTransferFile((const cJSON *)pstBody, &stTransferFile)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_TransferFile(&stTransferFile);
    if (SR_ERR_OK != rc) {
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
API_TransferLog(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_TRANSFER_FILE_ST stTransferFile;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stTransferFile, 0, sizeof(stTransferFile));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseTransferLog((const cJSON *)pstBody, &stTransferFile)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_TransferLog(&stTransferFile);
    if (SR_ERR_OK != rc) {
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
API_LampTest(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    OCP_LED_OPERATION_ST stLedOper;

    CLEAR_NO_USED_WARNING(pstReq);

    memset(&stLedOper, 0, sizeof(stLedOper));
    stLedOper.bEnabled = true;
    stLedOper.usInterval = 30; /* The default lamp auto test time is 30s. */
    snprintf(stLedOper.acSrcComponent, sizeof(stLedOper.acSrcComponent), "%s", "ALL");

    rc = DSC_RpcLedOperation(&stLedOper);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "REST_LampTest error");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_SetSystemOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_SYSTEM_ST stSystem;
    REST_SYSTEM_FLAG_ST stSystemFlag;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stSystem, 0, sizeof(stSystem));
    memset(&stSystemFlag, 0, sizeof(stSystemFlag));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseSystemOptional((const cJSON *)pstBody, &stSystem, &stSystemFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetSystemOptional(&stSystem, &stSystemFlag);
    if (SR_ERR_OK != rc) {
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
API_SetApsd(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    bool bApsdEnable = false;
    cJSON *pstBody = NULL;
    EN_APSD_PATH enPath = EN_APSD_PATH_MAX;

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseApsd((const cJSON *)pstBody, &bApsdEnable)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_OPERATIONS_APSD)) {
        enPath = EN_APSD_PATH_W;
    } else {
        enPath = EN_APSD_PATH_P;
    }

    rc = DSC_RpcSetApsd(enPath, bApsdEnable);
    if (rc != SR_ERR_OK) {
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
API_GetApsd(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    ST_APSD_STATE stApsdState;
    EN_APSD_PATH enPath = EN_APSD_PATH_MAX;

    CLEAR_NO_USED_WARNING(pstReq);

    if (0 == strcasecmp(pstReq->acUrl, URL_OMT_OPERATIONS_APSD)) {
        enPath = EN_APSD_PATH_W;
    } else {
        enPath = EN_APSD_PATH_P;
    }

#if 1
    /*get data by edfa api*/
    bool bCfg = false;
    bool bStatus = false;
    bool bEvent = false;
    uint32_t uiDevType = DEV_TYPE_PA;
    if (EN_APSD_PATH_P == enPath) {
        uiDevType = DEV_TYPE_P_PA;
    }
    memset(&stApsdState, 0, sizeof(stApsdState));
    rc = EDFA_GetApsdStatus(uiDevType, &bCfg, &bStatus, &bEvent);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_RpcGetApsdState");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }
#else
    /* get data */

    memset(&stApsdState, 0, sizeof(stApsdState));
    rc = DSC_RpcGetApsdState(enPath, &stApsdState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_RpcGetApsdState");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    cJSON_AddStringToObject(pstBody, STR_APSD_ENABLE, stApsdState.bEnabled ? STR_ENABLE : STR_DISABLE);
    cJSON_AddStringToObject(pstBody, STR_APSD_STATUS, stApsdState.bStatus ? STR_ENABLE : STR_DISABLE);
#endif
    pstBody = pstResp->pstResponse;
    cJSON_AddStringToObject(pstBody, STR_APSD_ENABLE, bCfg ? STR_ENABLE : STR_DISABLE);
    cJSON_AddStringToObject(pstBody, STR_APSD_STATUS, bStatus ? STR_ENABLE : STR_DISABLE);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}



void
API_SetIpacl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    cJSON *pstBody = NULL;

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_ResetAclList((char *)pstReq->pcBody);
    if (rc != SR_ERR_OK) {
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
API_GetIpacl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL, *pstArray = NULL, *pstItem = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    ST_ACL_LIST stAclList;
    uint32_t uiCnt = 0;
    char acTarget[64] = {0};

    /* get data */
    memset(&stAclList, 0, sizeof(stAclList));

    rc = DSC_GetAclList(true, &stAclList, DSC_GetRunningSession());
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetAclList");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_GetAclList(false, &stAclList, DSC_GetRunningSession());
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetAclList for ipv6");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    /* ipv4 ipacl */
    pstArray = cJSON_CreateArray();
    for (uiCnt = 0; uiCnt < stAclList.uiIpv4Cnt; uiCnt++) {
        pstItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pstItem, STR_SOURCE_IP, stAclList.pstIpv4Acl[uiCnt].acSrcAddr);
        memset(acTarget, 0, sizeof(acTarget));
        sscanf(stAclList.pstIpv4Acl[uiCnt].acAction, "openconfig-acl:%s", acTarget);
        cJSON_AddStringToObject(pstItem, STR_TARGET, acTarget);

        cJSON_AddItemToArray(pstArray, pstItem);
    }

    cJSON_AddItemToObject(pstBody, STR_RULES, pstArray);

    /* ipv6 ipacl */
    pstArray = cJSON_CreateArray();
    for (uiCnt = 0; uiCnt < stAclList.uiIpv6Cnt; uiCnt++) {
        pstItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pstItem, STR_SOURCE_IP, stAclList.pstIpv6Acl[uiCnt].acSrcAddr);
        memset(acTarget, 0, sizeof(acTarget));
        sscanf(stAclList.pstIpv6Acl[uiCnt].acAction, "openconfig-acl:%s", acTarget);
        cJSON_AddStringToObject(pstItem, STR_TARGET, acTarget);

        cJSON_AddItemToArray(pstArray, pstItem);
    }

    cJSON_AddItemToObject(pstBody, STR_RULES_IPV6, pstArray);


END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_SetMuxConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{

    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    cJSON *pstBody = NULL;
    REST_MUX_CFG_ST stMuxCfg;

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    memset(&stMuxCfg, 0, sizeof(stMuxCfg));
    rc = REST_ParseMuxConf((const cJSON *)pstBody, &stMuxCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "REST_ParseMuxConf is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = REST_SetMuxCfg(&stMuxCfg);
    if (rc != SR_ERR_OK) {
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
API_GetMuxConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL, *pstJsonObj = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    ST_CV_CONFIG stCfg;

    /* Get connectivity validation all config                                   */
    memset(&stCfg, 0x00, sizeof(stCfg));
    rc = DSC_CvGetConfig(&stCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "CV get config failed!");
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_REFLECTION);
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXT_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_REF].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXT_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_REF].dLowThr));

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CONNECTIVITY);
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_MUX_R].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_MUX_R].dLowThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_MUX_T].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_MUX_T].dLowThr));

#if defined(__OLSP__)
    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CLIENT_W);
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_W_R].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_W_R].dLowThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_W_T].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_W_T].dLowThr));

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CLIENT_P);
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_P_R].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_P_R].dLowThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_HIGH_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_P_T].dHighThr));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_LOW_THR, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stCfg.astThr[EN_DIFFER_CLIENT_P_T].dLowThr));
#endif

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_GetMuxState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL, *pstJsonObj = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    ST_CV_STATE stState;

    memset(&stState, 0, sizeof(stState));
    rc = DSC_CvGetState(&stState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "CV get state failed!");
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_REFLECTION);
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXT_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astReflect[EN_REFLECT_MUX].dOutputPower));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXT_REFLECT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astReflect[EN_REFLECT_MUX].dRefPower));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXT_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astReflect[EN_REFLECT_MUX].dDifferPower));

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CONNECTIVITY);

    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTR_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dOutputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTR_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dInputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTR_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dDifferPowerR));

    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTT_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dOutputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTT_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dInputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_CLIENTT_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.stMux.dDifferPowerT));

#if defined(__OLSP__)

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CLIENT_W);

    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dOutputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dInputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dDifferPowerR));

    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dOutputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dInputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[WORKING_LINE].dDifferPowerT));

    pstJsonObj = cJSON_AddObjectToObject(pstBody, STR_CLIENT_P);

    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dOutputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dInputPowerR));
    cJSON_AddNumberToObject(pstJsonObj, STR_MUXR_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dDifferPowerR));

    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dOutputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dInputPowerT));
    cJSON_AddNumberToObject(pstJsonObj, STR_DEMUXT_DIFFER_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stState.astClient[PROTECTION_LINE].dDifferPowerT));
#endif

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_ClearAutoAlarms(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;

    CLEAR_NO_USED_WARNING(pstReq);

    rc = DSC_AutoClearALarm();
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
API_GetOtdrSorFiles(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL, *pstArray = NULL, *pstItem = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    FILE_ATTR_ALL_INFO_ST stAllFileInfo;
    int i = 0;
    char acTmp[DEF_BUF_SIZE_128] = {0};
    bool bIsOtdrW = true;

    if (0 == strcasecmp(pstReq->acUrl, URL_OTDR_SOR_FILES)) {
        bIsOtdrW = true;
    } else {
        bIsOtdrW = false;
    }

    /* get data */
    memset(&stAllFileInfo, 0, sizeof(stAllFileInfo));
    rc = DSC_GetSystemFile(bIsOtdrW ? OTDR_SOR_EXTERNAL_PATH : OTDR_P_SOR_EXTERNAL_PATH, &stAllFileInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_GetSystemFile %s", bIsOtdrW ? OTDR_SOR_EXTERNAL_PATH : OTDR_P_SOR_EXTERNAL_PATH);
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    pstArray = cJSON_CreateArray();
    for (i = 0; i < stAllFileInfo.ulNum; i++) {
        pstItem = cJSON_CreateObject();
        snprintf(acTmp, sizeof(acTmp), "%s/%s", bIsOtdrW ? OTDR_SOR_EXTERNAL_PATH : OTDR_P_SOR_EXTERNAL_PATH, stAllFileInfo.astFileEntryInfo[i].acFileName);
        cJSON_AddStringToObject(pstItem, STR_FILENAME, acTmp);
        snprintf(acTmp, sizeof(acTmp), "%ld bytes", stAllFileInfo.astFileEntryInfo[i].ullFileSize);
        cJSON_AddStringToObject(pstItem, STR_FILE_SIZE, acTmp);
        cJSON_AddStringToObject(pstItem, STR_MODIFIED_DATE, stAllFileInfo.astFileEntryInfo[i].acFileModiyTime);
        cJSON_AddItemToArray(pstArray, pstItem);
    }

    cJSON_AddItemToObject(pstBody, STR_FILE, pstArray);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

