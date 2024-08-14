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

void
API_EdfaSetPA(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_EDFA_PA_ST stEdfaPA;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    REST_AMP_CARD_TYPE_EN enAmpCardType = EN_REST_AMP_CARD_MAX;

    memset(&stEdfaPA, 0, sizeof(stEdfaPA));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseEdfaPA((const cJSON *)pstBody, &stEdfaPA)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    enAmpCardType = (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_PREAMP)) ? EN_REST_AMP_W_CARD : EN_REST_AMP_P_CARD;
    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "AMP card is offline for %s", pstReq->acUrl);
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetEdfaPA(enAmpCardType, &stEdfaPA);
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
API_EdfaSetBA(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_EDFA_BA_ST stEdfaBA;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    REST_AMP_CARD_TYPE_EN enAmpCardType = EN_REST_AMP_CARD_MAX;

    memset(&stEdfaBA, 0, sizeof(stEdfaBA));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseEdfaBA((const cJSON *)pstBody, &stEdfaBA)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_ParseEdfaBA is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    enAmpCardType = (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_BOOSTERAMP)) ? EN_REST_AMP_W_CARD : EN_REST_AMP_P_CARD;
    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "AMP card is offline for %s", pstReq->acUrl);
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetEdfaBA(enAmpCardType, &stEdfaBA);
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

/* put /omt/config/pre-amp */
void
API_EdfaSetPaOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_EDFA_PA_ST stEdfaPA;
    REST_EDFA_FLAG_ST stPaExistFlag;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    REST_AMP_CARD_TYPE_EN enAmpCardType = EN_REST_AMP_CARD_MAX;

    memset(&stEdfaPA, 0, sizeof(stEdfaPA));
    memset(&stPaExistFlag, 0, sizeof(stPaExistFlag));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseEdfaPaOptional((const cJSON *)pstBody, &stEdfaPA, &stPaExistFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    enAmpCardType = (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_PREAMP)) ? EN_REST_AMP_W_CARD : EN_REST_AMP_P_CARD;
    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "AMP card is offline for %s", pstReq->acUrl);
        goto END_LABEL;
    }

    rc = REST_SetEdfaPaOptional(enAmpCardType, &stEdfaPA, &stPaExistFlag);
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
API_EdfaSetBaOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_EDFA_BA_ST stEdfaBA;
    REST_EDFA_FLAG_ST stBaExistFlag;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;
    REST_AMP_CARD_TYPE_EN enAmpCardType = EN_REST_AMP_CARD_MAX;

    memset(&stEdfaBA, 0, sizeof(stEdfaBA));
    memset(&stBaExistFlag, 0, sizeof(stBaExistFlag));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseEdfaBaOptional((const cJSON *)pstBody, &stEdfaBA, &stBaExistFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_ParseEdfaBA is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    enAmpCardType = (0 == strcasecmp(pstReq->acUrl, URL_OMT_CONFIG_BOOSTERAMP)) ? EN_REST_AMP_W_CARD : EN_REST_AMP_P_CARD;
    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "AMP card is offline for %s", pstReq->acUrl);
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SetEdfaBaOptional(enAmpCardType, &stEdfaBA, &stBaExistFlag);
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

