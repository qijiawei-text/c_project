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
/* #include "rest_api_msft.h" */
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_ocm.h"
/* #include "rest_pluggable.h" */
#include "oplk_file.h"
#include "DSC_ocm.h"

/* GET /data/channel-monitors/ */
void
API_GetOcmAllData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_CHANNELS_LIST_ST stList;
    OCM_PORT_DESC_ST stOcmPortDesc;
    int rc = SR_ERR_OK;
    cJSON *pstBody = NULL, *pstPortArray = NULL, *pstItem = NULL;
    int iResult = HTTP_STATUS_CODE_200;
    OCM_KEY_ST stOcmKey;
    int i = 0;

    pstBody = pstResp->pstResponse;

    memset(&stOcmKey, 0, sizeof(stOcmKey));
    rc = DSC_GetOcmCfgKeyFromDb(DSC_GetRunningSession(), &stOcmKey);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmCfgKeyFromDb.");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstPortArray = cJSON_CreateArray();
    for (i = 0; i < stOcmKey.iCnt; i++) {
        memset(&stList, 0, sizeof(stList));
        rc = DSC_GetOcmChannelsByPort(OCM_CFG_KEY, stOcmKey.aacOcmKeys[i], &stList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmChannelsByPort. port: %s", stOcmKey.aacOcmKeys[i]);
            break;
        }

        memset(&stOcmPortDesc, 0, sizeof(stOcmPortDesc));
        rc = DSC_GetOcmPortConfig(OCM_CFG_KEY, stOcmKey.aacOcmKeys[i], &stOcmPortDesc);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmPortConfig. port: %s", stOcmKey.aacOcmKeys[i]);
            break;
        }

        pstItem = cJSON_CreateObject();
        rc = REST_GetOnePortObjJson(&stOcmPortDesc, &stList, pstItem);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetOnePortObjJson. port: %s", stOcmKey.aacOcmKeys[i]);
            cJSON_Delete(pstItem);
            break;
        }

        cJSON_AddItemToArray(pstPortArray, pstItem);

        /* free */
        if (stList.pstChannelArray != NULL) {
            free(stList.pstChannelArray);
            stList.pstChannelArray = NULL;
        }
    }

    if (i < stOcmKey.iCnt) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get data. port: %s", stOcmKey.aacOcmKeys[i]);
        iResult = HTTP_STATUS_CODE_406;
        cJSON_Delete(pstPortArray);
        goto END_LABEL;
    }

    cJSON_AddItemToObject(pstBody, STR_CHANNEL_MONITOR, pstPortArray);

#if defined(__HCF__)
    OCM_MODE_CONFIG_ST stModeCfg;
    OCM_MODE_STATE_ST stModeState;

    memset(&stModeCfg, 0, sizeof(stModeCfg));
    if (SR_ERR_OK == REST_GetOcmModeCfg(&stModeCfg)) {
        pstItem = cJSON_AddObjectToObject(pstBody, STR_CONFIG);
        cJSON_AddStringToObject(pstItem, STR_WAVEPLAN_MODE, stModeCfg.acWaveplanMode);
    }

    memset(&stModeState, 0, sizeof(stModeState));
    if (SR_ERR_OK == DSC_GetOcmModeState(&stModeState)) {
        pstItem = cJSON_AddObjectToObject(pstBody, STR_STATE);
        cJSON_AddStringToObject(pstItem, STR_WAVEPLAN_MODE, stModeCfg.acWaveplanMode);
        cJSON_AddStringToObject(pstItem, STR_CURRENT_WAVEPLAN, stModeState.acCurWaveplanMode);
    }

#endif

END_LABEL:
    if (stList.pstChannelArray != NULL) {
        free(stList.pstChannelArray);
        stList.pstChannelArray = NULL;
    }
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

}

static int
__ParsePortNameFromUrl(char *pcUrl, char *pcKey, char *pcPortName, size_t nLen)
{
    int rc = SR_ERR_OK;
    char *pcPos = NULL, *pcPosTemp = NULL;
    size_t nTmpLen = 0;

    if (NULL == pcUrl || NULL == pcKey || NULL == pcPortName || nLen < 2) {
        return SR_ERR_INVAL_ARG;
    }

    pcPos = strstr(pcUrl, pcKey);
    if (NULL == pcPos) {
        rc = SR_ERR_NOT_FOUND;
        return rc;
    }
    pcPos = pcPos + strlen(pcKey);
    pcPosTemp = strchr(pcPos, '/');
    if (NULL == pcPosTemp) {
        nTmpLen = strlen(pcPos);
    } else {
        nTmpLen = pcPosTemp - pcPos;
    }

    if (0 == nLen || nTmpLen >= nLen) {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "%s", "Length is out of range");
        goto ERR_LABEL;
    }

    /* find */
    memcpy(pcPortName, pcPos, nTmpLen);
    *(pcPortName + nTmpLen) = '\0';

    return SR_ERR_OK;
ERR_LABEL:
    return SR_ERR_INVAL_ARG;
}

int
__ParseFrequencyFromUrl(char *pcUrl, char *pcLowerFreq, size_t nLowerLen, char *pcUpperFreq, size_t nUpperLen)
{
    int rc = SR_ERR_OK;
    char *pcPos = NULL, *pcPosTemp = NULL;
    size_t nLen = 0;

    if (NULL == pcUrl || NULL == pcLowerFreq || nLowerLen < 2
        || NULL == pcUpperFreq || nUpperLen < 2) {
        return SR_ERR_INVAL_ARG;
    }

    pcPos = strstr(pcUrl, STR_KEY_CHANNEL);
    if (NULL == pcPos) {
        rc = SR_ERR_NOT_FOUND;
        goto ERR_LABEL;
    }

    pcPos = pcPos + strlen(STR_KEY_CHANNEL);

    pcPosTemp = strchr(pcPos, ',');
    if (NULL == pcPosTemp) {
        rc = SR_ERR_NOT_FOUND;
        goto ERR_LABEL;
    }

    nLen = pcPosTemp - pcPos;
    if (nLen >= nLowerLen) {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "%s", "Length is out of range");
        goto ERR_LABEL;
    }

    /* find lower frequency */
    memcpy(pcLowerFreq, pcPos, nLen);
    *(pcLowerFreq + nLen) = '\0';

    pcPos = pcPosTemp + 1;
    pcPosTemp = strchr(pcPos, '/');
    if (NULL == pcPosTemp) {
        nLen = strlen(pcPos);
    } else {
        nLen = pcPosTemp - pcPos;
    }

    if (0 == nLen || nLen >= nUpperLen) {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "%s", "Length is out of range");
        goto ERR_LABEL;
    }

    /* find lower frequency */
    memcpy(pcUpperFreq, pcPos, nLen);
    *(pcUpperFreq + nLen) = '\0';

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}

/*
   all ocm URL:
   (1) /data/channel-monitors/
   (2) /data/channel-monitors/channel-monitor/
   (3) /data/channel-monitors/channel-monitor={name}/
   (4) /data/channel-monitors/channel-monitor={name}/channels/
   (5) /data/channel-monitors/channel-monitor={name}/channels/channel={lower-frequency},{upper-frequency}/
   (6) /data/channel-monitors/channel-monitor={name}/channels/channel={lower-frequency},{upper-frequency}/state/
   (7) /data/channel-monitors/channel-monitor={name}/config/
   (8) /data/channel-monitors/channel-monitor={name}/state/
 */

static void
__API_GetOcmChannelMonitor(char *pcPortName, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_CHANNELS_LIST_ST stList;
    OCM_PORT_DESC_ST stOcmPortDesc;
    int rc = SR_ERR_OK;
    cJSON *pstBody = NULL;
    int iResult = HTTP_STATUS_CODE_200;

    memset(&stList, 0, sizeof(stList));
    rc = DSC_GetOcmChannelsByPort(OCM_CFG_KEY, pcPortName, &stList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by DSC_GetOcmChannelsByPort");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    memset(&stOcmPortDesc, 0, sizeof(stOcmPortDesc));
    rc = DSC_GetOcmPortConfig(OCM_CFG_KEY, pcPortName, &stOcmPortDesc);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by DSC_GetOcmPortConfig");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    rc = REST_GetOnePortObjJson(&stOcmPortDesc, &stList, pstBody);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by REST_GetOnePortObjJson");
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

END_LABEL:
    if (stList.pstChannelArray != NULL) {
        free(stList.pstChannelArray);
        stList.pstChannelArray = NULL;
    }
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

}

static void
__API_GetOcmChannels(char *pcPortName, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_CHANNELS_LIST_ST stList;
    int rc = SR_ERR_OK;
    cJSON *pstBody = NULL;
    cJSON *pstArray = NULL;
    int iResult = HTTP_STATUS_CODE_200;

    memset(&stList, 0, sizeof(stList));
    rc = DSC_GetOcmChannelsByPort(OCM_CFG_KEY, pcPortName, &stList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmChannelsByPort. port: %s", pcPortName);
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    /*add channel array*/
    /* pstArray = cJSON_CreateArray(); */

    rc = REST_GetChannelArrayJson(&stList, pstBody);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetChannelArrayJson. port: %s", pcPortName);
        iResult = HTTP_STATUS_CODE_406;
        cJSON_Delete(pstArray);
        goto END_LABEL;
    }

    /* cJSON_AddItemToObject(pstBody, STR_CHANNEL, pstArray); */

END_LABEL:
    if (stList.pstChannelArray != NULL) {
        free(stList.pstChannelArray);
        stList.pstChannelArray = NULL;
    }
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

    return;
}

static void
__API_GetOcmChannelInfo(char *pcPortName, char *pcLowerFreq, char *pcUpperFreq, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_CHANNEL_ST stChannel;
    int rc = SR_ERR_OK;
    cJSON *pstBody = NULL, *pstStateItem = NULL;
    int iResult = HTTP_STATUS_CODE_200;

    memset(&stChannel, 0, sizeof(stChannel));
    rc = REST_GetOneChannelState(pcPortName, pcLowerFreq, pcUpperFreq, &stChannel);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetOneChannelState. port: %s", pcPortName);
        iResult = (SR_ERR_NOT_FOUND == rc) ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    pstStateItem = cJSON_CreateObject();

    cJSON_AddStringToObject(pstStateItem, STR_CHANNEL_NO, stChannel.acChannelNo);
    cJSON_AddNumberToObject(pstStateItem, STR_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stChannel.dPower));
    cJSON_AddNumberToObject(pstStateItem, STR_UPPER_FREQUENCY, stChannel.ulHighFrequency);
    cJSON_AddNumberToObject(pstStateItem, STR_LOWER_FREQUENCY, stChannel.ulLowFrequency);
    cJSON_AddItemToObject(pstBody, STR_STATE, pstStateItem);
    cJSON_AddNumberToObject(pstBody, STR_UPPER_FREQUENCY, stChannel.ulHighFrequency);
    cJSON_AddNumberToObject(pstBody, STR_LOWER_FREQUENCY, stChannel.ulLowFrequency);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
}

static void
__API_GetOcmChannelState(char *pcPortName, char *pcLowerFreq, char *pcUpperFreq, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_CHANNEL_ST stChannel;
    int rc = SR_ERR_OK;
    cJSON *pstBody = NULL;
    int iResult = HTTP_STATUS_CODE_200;

    memset(&stChannel, 0, sizeof(stChannel));
    rc = REST_GetOneChannelState(pcPortName, pcLowerFreq, pcUpperFreq, &stChannel);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetOneChannelState. port: %s, pcLowerFreq: %s, pcUpperFreq: %s", pcPortName, pcLowerFreq, pcUpperFreq);
        iResult = (SR_ERR_NOT_FOUND == rc) ? HTTP_STATUS_CODE_405 : HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    cJSON_AddStringToObject(pstBody, STR_CHANNEL_NO, stChannel.acChannelNo);
    cJSON_AddNumberToObject(pstBody, STR_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stChannel.dPower));
    cJSON_AddNumberToObject(pstBody, STR_UPPER_FREQUENCY, stChannel.ulHighFrequency);
    cJSON_AddNumberToObject(pstBody, STR_LOWER_FREQUENCY, stChannel.ulLowFrequency);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
}

static void
__API_GetOcmPortConfig(char *pcPortName, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_PORT_DESC_ST stOcmPortDesc;
    cJSON *pstBody = NULL;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stOcmPortDesc, 0, sizeof(stOcmPortDesc));
    rc = DSC_GetOcmPortConfig(OCM_CFG_KEY, pcPortName, &stOcmPortDesc);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmPortConfig. port: %s", pcPortName);
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    cJSON_AddStringToObject(pstBody, STR_MONITOR_PORT, stOcmPortDesc.acMonitorPort);
    cJSON_AddStringToObject(pstBody, STR_NAME, stOcmPortDesc.acName);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
}

static void
__API_GetOcmPortState(char *pcPortName, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    /* TBD: state data is the same with config data; */
    __API_GetOcmPortConfig(pcPortName, pstReq, pstResp);
}

void
API_GetOcmOnePortData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    char acPort[32] = {0};
    char acUrlPath[256];
    char acLowerFreq[32] = {0};
    char acUpperFreq[32] = {0};
    /* bool bHavePort = false; */
    bool bHaveFrequency = false;
    unsigned char ucPortId = EN_OCM_PORT_NONE;
    int iResult = HTTP_STATUS_CODE_404;

    /* parse ocm port name */
    rc = __ParsePortNameFromUrl(pstReq->acUrl, STR_KEY_CHANNEL_MONITOR, acPort, sizeof(acPort));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by parsing port name");
        goto INVALID_URL;
    }

    /* LOG_WRITE(OPT_LOG, "%s acPort: %s", __func__, acPort); */
    /* check ocm port */
    ucPortId = DSC_GetOcmPortIdByNameNew(acPort);
    if (ucPortId <= EN_OCM_PORT_NONE || ucPortId >= EN_OCM_PORT_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by parsing port name");
        iResult = HTTP_STATUS_CODE_405;
        goto INVALID_URL;
    }

    /* parse lower and upper frequency */
    rc = __ParseFrequencyFromUrl(pstReq->acUrl, acLowerFreq, sizeof(acLowerFreq), acUpperFreq, sizeof(acUpperFreq));
    if (SR_ERR_OK == rc) {
        bHaveFrequency = true;
        /* LOG_WRITE(OPT_LOG, "%s", "find lower and upper frequency"); */
    }

    if (!bHaveFrequency) {
        /* 1. GET /data/channel-monitors/channel-monitor={name}/             */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CHANNEL_MONITOR, acPort);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmChannelMonitor(acPort, pstReq, pstResp);
            return;
        }

        /* 2. GET /data/channel-monitors/channel-monitor={name}/channels/    */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CHANNELS, acPort);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmChannels(acPort, pstReq, pstResp);
            return;
        }

        /* 3. GET /data/channel-monitors/channel-monitor={name}/config/      */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CONFIG, acPort);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmPortConfig(acPort, pstReq, pstResp);
            return;
        }

        /* 4. GET /data/channel-monitors/channel-monitor={name}/state/      */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_STATE, acPort);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmPortState(acPort, pstReq, pstResp);
            return;
        }

    } else {
        /* 5. GET /data/channel-monitors/channel-monitor={name}/channels/channel={lower-frequency},{upper-frequency}/          */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CHANNEL, acPort, acLowerFreq, acUpperFreq);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmChannelInfo(acPort, acLowerFreq, acUpperFreq, pstReq, pstResp);
            return;
        }

        /* 6. GET /data/channel-monitors/channel-monitor={name}/channels/channel={lower-frequency},{upper-frequency}/state/    */
        snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CHANNEL_STATE, acPort, acLowerFreq, acUpperFreq);
        if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
            __API_GetOcmChannelState(acPort, acLowerFreq, acUpperFreq, pstReq, pstResp);
            return;
        }
    }

    /* invalid url, TBD*/
    iResult = HTTP_STATUS_CODE_404;

INVALID_URL:
    pstResp->iStatus = iResult;
    LOG_WRITE(EN_LOG_ERROR, "Invalid URL: %s", pstReq->acUrl);
    return;

}

static void
__API_SetOcmPortConfig(char *pcPortName, REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    OCM_PORT_DESC_ST stOcmPortDesc;
    cJSON *pstBody = NULL;
    int iResult = HTTP_STATUS_CODE_200;
    int rc = SR_ERR_OK;

    memset(&stOcmPortDesc, 0, sizeof(stOcmPortDesc));

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    rc = REST_ParseOcmPortConfig((const cJSON *)pstBody, (const char *)pcPortName, &stOcmPortDesc);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_UpdateOcmConfig(OCM_CFG_KEY, &stOcmPortDesc);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_UpdateOcmConfig. port: %s", pcPortName);
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

END_LABEL:
    cJSON_Delete(pstBody);
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

}

void
API_SetOcmOnePortData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    char acPort[32] = {0};
    char acUrlPath[256];
    unsigned char ucPortId = EN_OCM_PORT_NONE;
    int iResult = HTTP_STATUS_CODE_404;

    /* parse ocm port name */
    rc = __ParsePortNameFromUrl(pstReq->acUrl, STR_KEY_CHANNEL_MONITOR, acPort, sizeof(acPort));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by parsing port name");
        goto INVALID_URL;
    }

    /* check ocm port */
    ucPortId = DSC_GetOcmPortIdByNameNew(acPort);
    if (ucPortId <= EN_OCM_PORT_NONE || ucPortId >= EN_OCM_PORT_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by parsing port name");
        iResult = HTTP_STATUS_CODE_405;
        goto INVALID_URL;
    }

    /* PUT /data/channel-monitors/channel-monitor={name}/config/      */
    snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_CONFIG, acPort);
    if (0 == strcmp(acUrlPath, pstReq->acUrl)) {
        __API_SetOcmPortConfig(acPort, pstReq, pstResp);
        return;
    }


    /* invalid url, TBD*/
    iResult = HTTP_STATUS_CODE_404;

INVALID_URL:
    pstResp->iStatus = iResult;
    LOG_WRITE(EN_LOG_ERROR, "Invalid URL: %s", pstReq->acUrl);

    return;

}

void
API_GetOcmOsaData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL, *pstArray = NULL, *pstNumNode;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    REST_OSA_DATA_ST stOsaData;
    uint32_t i = 0;
    char acPortName[32] = {0};
    char acUrlPath[256] = {0};
    double dPower = 0;

    /* parse ocm osa port name */
    rc = __ParsePortNameFromUrl(pstReq->acUrl, STR_KEY_OCM_OSA, acPortName, sizeof(acPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Error by parsing port name");
        iResult = HTTP_STATUS_CODE_404;
        goto END_LABEL;
    }

    /* check url */
    snprintf(acUrlPath, sizeof(acUrlPath), URL_PATH_OCM_OSA, acPortName);
    if (0 != strcmp(acUrlPath, pstReq->acUrl)) {
        iResult = HTTP_STATUS_CODE_404;
        LOG_WRITE(EN_LOG_ERROR, "%s", "Invalid url path");
        goto END_LABEL;
    }

    memset(&stOsaData, 0, sizeof(stOsaData));
    rc = REST_GetOcmOsaData(acPortName, &stOsaData);
    if (rc != SR_ERR_OK) {
        iResult = HTTP_STATUS_CODE_406;
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetOcmOsaData: %s", acPortName);
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    pstArray = cJSON_CreateArray();

    for (i = 0; i < stOsaData.uiTotal; i++) {
        dPower = stOsaData.adPwr[stOsaData.uiTotal - i - 1];
        pstNumNode = cJSON_CreateNumber(GET_2_DECIMAL_DIGITS_OF_DOUBLE(dPower));
        cJSON_AddItemToArray(pstArray, pstNumNode);
    }

    cJSON_AddStringToObject(pstBody, STR_PORT, acPortName);
    cJSON_AddItemToObject(pstBody, STR_POWER, pstArray);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_GetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    OCM_MODE_CONFIG_ST stModeCfg;

    CLEAR_NO_USED_WARNING(pstReq);

    /* get data */
    memset(&stModeCfg, 0, sizeof(stModeCfg));
    rc = REST_GetOcmModeCfg(&stModeCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetOcmConfig!");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;

    cJSON_AddStringToObject(pstBody, STR_WAVEPLAN_MODE, stModeCfg.acWaveplanMode);
    /* cJSON_AddStringToObject(pstBody, STR_CHANNEL_POWER_MODE, stModeCfg.acChPwrMode); */

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

void
API_SetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    OCM_MODE_CONFIG_ST stModeCfg;
    cJSON *pstBody = NULL;

    /*The data in the message body is converted to JSON data*/
    pstBody = JSON_ParseObject((char *)pstReq->pcBody, pstReq->nBodyLen);
    if (NULL == pstBody) {
        LOG_WRITE(EN_LOG_ERROR, "JSON_ParseObject is null");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    /*parse data*/
    memset(&stModeCfg, 0, sizeof(stModeCfg));
    if (OPLK_OK != REST_ParseOcmModeCfg((const cJSON *)pstBody, &stModeCfg)) {
        LOG_WRITE(EN_LOG_ERROR, "REST_Parse is error");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_SetOcmModeCfg(&stModeCfg);
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
API_GetOcmModeState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    int rc = SR_ERR_OK;
    int iResult = HTTP_STATUS_CODE_200;
    OCM_MODE_CONFIG_ST stModeCfg;
    OCM_MODE_STATE_ST stModeState;

    memset(&stModeCfg, 0, sizeof(stModeCfg));
    memset(&stModeState, 0, sizeof(stModeState));

    rc = REST_GetOcmModeCfg(&stModeCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmModeState!");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    rc = DSC_GetOcmModeState(&stModeState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmModeState!");
        iResult = HTTP_STATUS_CODE_405;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;


    cJSON_AddStringToObject(pstBody, STR_WAVEPLAN_MODE, stModeCfg.acWaveplanMode);
    cJSON_AddStringToObject(pstBody, STR_CURRENT_WAVEPLAN, stModeState.acCurWaveplanMode);

END_LABEL:
    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);
    return;
}

