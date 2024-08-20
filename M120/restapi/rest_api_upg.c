#include "rest_common.h"
/* #include "rest_api_msft.h" */
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_upg.h"
/* #include "rest_pluggable.h" */
#include "oplk_file.h"

void
API_SoftwareUpgrade(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_SOFTWARE_LOAD_ST stSoftwareLoad;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stSoftwareLoad, 0, sizeof(stSoftwareLoad));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseSoftwareLoad((const cJSON *)pstBody, &stSoftwareLoad)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*download, then auto activate in operationd process, modified on 2022-02-23 */
    rc = REST_SoftwareDownload(&stSoftwareLoad, true);
    if (OPLK_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

#if 0
    /*activate*/
    rc = REST_SoftwareActivate();
    if (OPLK_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }
#endif

END_LABEL:
    cJSON_Delete(pstBody);
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}


void
API_SoftwareDownLoad(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    REST_SOFTWARE_LOAD_ST stSoftwareLoad;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stSoftwareLoad, 0, sizeof(stSoftwareLoad));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    if (OPLK_OK != REST_ParseSoftwareLoad((const cJSON *)pstBody, &stSoftwareLoad)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*set data*/
    rc = REST_SoftwareDownload(&stSoftwareLoad, false);
    if (OPLK_OK != rc) {
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
API_SoftwareActivate(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    /*set data*/
    rc = REST_SoftwareActivate();
    if (OPLK_OK != rc) {
        iResult = (SR_ERR_INVAL_ARG == rc ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406);
        goto END_LABEL;
    }

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

void
API_GetSoftwareState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    ST_UPG_STATE stUpgState;
    int iResult = HTTP_STATUS_CODE_200;
    FILE_ATTR_ALL_INFO_ST stAllFileInfo;
    char acTmp[DEF_BUF_SIZE_64] = {0};

    CLEAR_NO_USED_WARNING(pstReq);

    memset(&stUpgState, 0x00, sizeof(stUpgState));
    rc = DSC_GetUpgState(&stUpgState, sizeof(stUpgState));
    if (rc != SR_ERR_OK) {
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_GetSystemFile(IMAGE_EXTERNAL_PATH, &stAllFileInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_GetSystemFile %s", IMAGE_EXTERNAL_PATH);
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    cJSON_AddStringToObject(pstBody, STR_STATE, stUpgState.acUpgdStep);

    cJSON_AddStringToObject(pstBody, STR_STATUS, stUpgState.acSysState);

    cJSON_AddNumberToObject(pstBody, STR_PROGRESS, stUpgState.u8Percent);


    memset(acTmp, 0, sizeof(acTmp));
    snprintf(acTmp, sizeof(acTmp) - 1, "%ld bytes", (0 == stAllFileInfo.ulNum) ? 0 : stAllFileInfo.astFileEntryInfo[0].ullFileSize);
    cJSON_AddStringToObject(pstBody, STR_FILE_SIZE, acTmp);

    cJSON_AddStringToObject(pstBody, STR_HITLESS, stUpgState.bHitLess ? "yes" : "no");


END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

