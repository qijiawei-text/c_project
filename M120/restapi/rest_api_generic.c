/******************************************************************************
*
* Copyright(C), 2020, OPLINK LLC, a Molex company
*
* Filename:      rest_sr_generic.c
* Description:   REST APIs of process request generically based on yang model.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_log.h"
#include "rest_api_generic.h"
#include "rest_sr_set.h"
#include "rest_sr_get.h"

void
API_sr_set(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        iResult = HTTP_STATUS_CODE_405;
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        goto END_LABEL;
    }

    rc = REST_Set(pstReq->acUrl, pstBody);
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
API_sr_rpc(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    cJSON *pstOutput = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;

    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        iResult = HTTP_STATUS_CODE_405;
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        goto END_LABEL;
    }

    cJSON_Delete(pstResp->pstResponse);
    pstResp->pstResponse = NULL;

    rc = REST_Rpc(pstReq->acUrl, pstBody, &pstOutput);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

    pstResp->pstResponse = pstOutput;

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_sr_get(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;

    /* pstBody = pstResp->pstResponse; */
    cJSON_Delete(pstResp->pstResponse);
    pstResp->pstResponse = NULL;

    rc = REST_Get(pstReq->acUrl, &pstBody);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

    if (NULL == pstBody) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "empty response data");
        goto END_LABEL;
    }

END_LABEL:
    pstResp->pstResponse = pstBody;
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_sr_delete(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;

    rc = REST_Delete(pstReq->acUrl);
    if (SR_ERR_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

