/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_msft.c
* Description:   only used for MSFT, parse requested json data and
*                      get business data for reponsing http/https request
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/17  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "util_inc.h"
#include "DevData.h"
#include "DevSlotCApi.h"
#include "DevOcmCApi.h"
#include "rest_ocm.h"

int
REST_GetOneChannelState(char *pcPortName, char *pcLowerFreq, char *pcUpperFreq, OCM_CHANNEL_ST *pstChannel)
{
    OCM_CHANNELS_LIST_ST stList;
    int rc = SR_ERR_OK;
    size_t i = 0;
    char acLowFreq[32] = {0};
    char acHighFreq[32] = {0};

    if (NULL == pcPortName || NULL == pcLowerFreq || NULL == pcUpperFreq || NULL == pstChannel) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Invalid Parameter");
        goto END_LABEL;
    }

    memset(&stList, 0, sizeof(stList));
    rc = DSC_GetOcmChannelsByPort(OCM_CFG_KEY, pcPortName, &stList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOcmChannelsByPort. port: %s", pcPortName);
        goto END_LABEL;
    }

    for (i = 0; i < stList.nNum; i++) {

        snprintf(acLowFreq, sizeof(acLowFreq), "%lu", stList.pstChannelArray[i].ulLowFrequency);
        snprintf(acHighFreq, sizeof(acHighFreq), "%lu", stList.pstChannelArray[i].ulHighFrequency);
        if ((0 == strcmp(acLowFreq, pcLowerFreq)) && (0 == strcmp(acHighFreq, pcUpperFreq))) {
            memcpy(pstChannel, &stList.pstChannelArray[i], sizeof(OCM_CHANNEL_ST));
            break;
        }
    }

    if (i >= stList.nNum) {
        rc = SR_ERR_NOT_FOUND;
        LOG_WRITE(EN_LOG_ERROR, "Not find. port: %s, low-freq: %s, high-freq: %s", pcPortName, pcLowerFreq, pcUpperFreq);
    }

END_LABEL:
    return rc;
}

int
REST_GetPortDescObjJson(const OCM_PORT_DESC_ST *pstPortDesc, cJSON *pstObjJson)
{
    if (NULL == pstPortDesc || NULL == pstObjJson) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Invalid Parameter");
        return SR_ERR_INVAL_ARG;
    }

    cJSON_AddStringToObject(pstObjJson, STR_MONITOR_PORT, pstPortDesc->acMonitorPort);
    cJSON_AddStringToObject(pstObjJson, STR_NAME, pstPortDesc->acName);

    return SR_ERR_OK;
}

int
REST_GetChannelArrayJson(const OCM_CHANNELS_LIST_ST *pstList, cJSON *pstArrayJson)
{
    cJSON *pstItem = NULL, *pstStateItem = NULL, *pstArray = NULL;
    size_t i = 0;
    OCM_CHANNEL_ST *pstChannel = NULL;

    if (NULL == pstList || NULL == pstArrayJson) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Invalid Parameter");
        return SR_ERR_INVAL_ARG;
    }

    pstArray = cJSON_CreateArray();
    for (i = 0; i < pstList->nNum; i++) {
        pstItem = cJSON_CreateObject();
        pstStateItem = cJSON_AddObjectToObject(pstItem, STR_STATE);
        pstChannel = &(pstList->pstChannelArray[i]);

        cJSON_AddStringToObject(pstStateItem, STR_CHANNEL_NO, pstChannel->acChannelNo);
        cJSON_AddNumberToObject(pstStateItem, STR_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstChannel->dPower));
        cJSON_AddNumberToObject(pstStateItem, STR_UPPER_FREQUENCY, pstChannel->ulHighFrequency);
        cJSON_AddNumberToObject(pstStateItem, STR_LOWER_FREQUENCY, pstChannel->ulLowFrequency);

        cJSON_AddNumberToObject(pstItem, STR_UPPER_FREQUENCY, pstChannel->ulHighFrequency);
        cJSON_AddNumberToObject(pstItem, STR_LOWER_FREQUENCY, pstChannel->ulLowFrequency);

        cJSON_AddItemToArray(pstArray, pstItem);
    }

    cJSON_AddItemToObject(pstArrayJson, STR_CHANNEL, pstArray);

    return SR_ERR_OK;
}

int
REST_GetOnePortObjJson(const OCM_PORT_DESC_ST *pstPortDesc, const OCM_CHANNELS_LIST_ST *pstList, cJSON *pstObjJson)
{
    cJSON *pstConfigItem = NULL, *pstStateItem = NULL, *pstChannelsItem = NULL;
    int rc = SR_ERR_OK;

    if (NULL == pstPortDesc || NULL == pstList || NULL == pstObjJson) {
        LOG_WRITE(EN_LOG_ERROR, "%s", "Invalid Parameter");
        return SR_ERR_INVAL_ARG;
    }

    pstStateItem = cJSON_CreateObject();
    pstConfigItem = cJSON_CreateObject();
    rc = REST_GetPortDescObjJson(pstPortDesc, pstStateItem);
    rc |= REST_GetPortDescObjJson(pstPortDesc, pstConfigItem);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    pstChannelsItem = cJSON_CreateObject();
    rc = REST_GetChannelArrayJson(pstList, pstChannelsItem);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    cJSON_AddItemToObject(pstObjJson, STR_CHANNELS, pstChannelsItem);

    cJSON_AddStringToObject(pstObjJson, STR_NAME, pstPortDesc->acName);

    cJSON_AddItemToObject(pstObjJson, STR_STATE, pstStateItem);
    cJSON_AddItemToObject(pstObjJson, STR_CONFIG, pstConfigItem);


    return SR_ERR_OK;

ERR_LABEL:
    cJSON_Delete(pstStateItem);
    cJSON_Delete(pstConfigItem);
    cJSON_Delete(pstChannelsItem);
    return SR_ERR_OPERATION_FAILED;
}

int
REST_ParseOcmPortConfig(const cJSON *pstJson, const char *pcPortName, OCM_PORT_DESC_ST *pstOcmPortDesc)
{
    OCM_PORT_DESC_ST stOcmPortDesc;
    int rc = SR_ERR_OK;
    JSON_OBJ_ST astJsonObj[] = {{STR_MONITOR_PORT, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stOcmPortDesc.acMonitorPort), sizeof(stOcmPortDesc.acMonitorPort)},
                                {STR_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stOcmPortDesc.acName), sizeof(stOcmPortDesc.acName)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stOcmPortDesc, 0, sizeof(stOcmPortDesc));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    /* check valid */
    rc = UTIL_ChkStrSpecChr(stOcmPortDesc.acMonitorPort);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check monitor-port error!");
        goto ERR_LABEL;
    }

    if (0 != strcmp(pcPortName, stOcmPortDesc.acName)) {
        LOG_WRITE(EN_LOG_ERROR, "portname is not matched");
        goto ERR_LABEL;
    }

    memcpy(pstOcmPortDesc, &stOcmPortDesc, sizeof(stOcmPortDesc));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

#if 0
static int
__GetCaliData(char *pcPortName, double *pdCali)
{
    int rc = SR_ERR_OK;
    CCali astCali[16] = {0};
    double dCali = 0.0;
    int16_t sOcmCali = 0;
    uint8_t ucPort = 0;

    rc = SLOT_GetCali(0, &astCali[0]);
    if (rc != OPLK_OK) {
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    /* check ocm port */
    ucPort = DSC_GetOcmPortIdByNameNew(pcPortName);

    switch (ucPort) {
    case EN_OCM_PORT_PRE_AMP_INPUT:
        dCali = (double)astCali[0].sPaInToOcm / WEIGHT_HUNDREDTH;
        break;
    case EN_OCM_PORT_PRE_AMP_OUTPUT:
        dCali = (double)astCali[0].sPaOutToOcm / WEIGHT_HUNDREDTH;
        break;
    case EN_OCM_PORT_BOOSTER_AMP_INPUT:
        dCali = (double)astCali[0].sBaInToOcm / WEIGHT_HUNDREDTH;
        break;
    case EN_OCM_PORT_BOOSTER_AMP_OUTPUT:
        dCali = (double)astCali[0].sBaOutToOcm / WEIGHT_HUNDREDTH;
        break;
    case EN_OCM_PORT_EXTERNAL:
        rc = OCM_GetVoa(0, &sOcmCali);
        if (rc != OPLK_OK) {
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }

        dCali = (double)sOcmCali / WEIGHT_HUNDREDTH;
        break;
    default:
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    *pdCali = dCali;

ERR_LABEL:

    return rc;

}
#endif

int
REST_GetOcmOsaData(char *pcPortName, REST_OSA_DATA_ST *pstOsaData)
{
    int rc = SR_ERR_OK;
    int16_t asBuf[2048] = {0};
    uint16_t usLen = 0;
    int i = 0;
    REST_OSA_DATA_ST stOsaData;
    double dPower = 0;

    memset(&asBuf, 0, sizeof(asBuf));
    rc = DSC_GetOsaRpc(pcPortName, asBuf, sizeof(asBuf), &usLen);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    memset(&stOsaData, 0, sizeof(stOsaData));
    for (i = 0; i < usLen; i++) {
        dPower = ((double)asBuf[i]) / 100;
        stOsaData.adPwr[stOsaData.uiTotal] = dPower;
        stOsaData.uiTotal++;
    }

    memcpy(pstOsaData, &stOsaData, sizeof(stOsaData));

ERR_LABEL:

    return rc;
}

int
REST_GetOcmModeCfg(OCM_MODE_CONFIG_ST *pstModeCfg)
{
    int rc = SR_ERR_OK;
    /* CHANNEL_POWER_MODE_TYPE enChPwrMode = CHANNEL_POWER_MODE_NONE_TYPE; */
    OCM_MODE_CONFIG_ST stModeCfg;
    WAVEPLAN_MODE_TYPE enWaveplanMode = WAVEPLAN_MODE_NONE_TYPE;

    if (NULL == pstModeCfg) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid para!");
        goto ERR_LABEL;
    }

    memset(&stModeCfg, 0, sizeof(stModeCfg));

#if 0
    rc = DSC_GetChannelPowerModeConfig(&enChPwrMode);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetWaveplanModeConfig!");
        goto ERR_LABEL;
    }

    if (CHANNEL_POWER_MODE_AUTO_TYPE == enChPwrMode) {
        snprintf(stModeCfg.acChPwrMode, sizeof(stModeCfg.acChPwrMode), "%s", CHANNEL_POWER_AUTO);
    } else if (CHANNEL_POWER_MODE_RAW_TYPE == enChPwrMode) {
        snprintf(stModeCfg.acChPwrMode, sizeof(stModeCfg.acChPwrMode), "%s", CHANNEL_POWER_RAW);
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid enChPwrMode: %d!", enChPwrMode);
        goto ERR_LABEL;
    }
#endif

    rc = DSC_GetWaveplanModeConfig(&enWaveplanMode);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetWaveplanModeConfig failed!");
        goto ERR_LABEL;
    }

    if (WAVEPLAN_MODE_AUTO_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_AUTO);
    } else if (WAVEPLAN_MODE_75G_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_75G);
    } else if (WAVEPLAN_MODE_100G_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_100G);
    } else if (WAVEPLAN_MODE_150G_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_150G);
    } else if (WAVEPLAN_MODE_175G_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_175G);
    } else if (WAVEPLAN_MODE_300G_TYPE == enWaveplanMode) {
        snprintf(stModeCfg.acWaveplanMode, sizeof(stModeCfg.acWaveplanMode), "%s", WAVEPLAN_300G);
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid enWaveplanMode: %d!", enWaveplanMode);
        goto ERR_LABEL;
    }

    memcpy(pstModeCfg, &stModeCfg, sizeof(stModeCfg));

ERR_LABEL:

    return rc;
}

int
REST_ParseOcmModeCfg(const cJSON *pstJson, OCM_MODE_CONFIG_ST *pstModeCfg)
{
    OCM_MODE_CONFIG_ST stModeCfg;
    JSON_OBJ_ST astJsonObj[] = {{STR_WAVEPLAN_MODE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stModeCfg.acWaveplanMode), sizeof(stModeCfg.acWaveplanMode)},
                                /* {STR_CHANNEL_POWER_MODE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stModeCfg.acChPwrMode), sizeof(stModeCfg.acChPwrMode)}, */
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    if (NULL == pstJson || NULL == pstModeCfg) {
        goto ERR_LABEL;
    }

    memset(&stModeCfg, 0, sizeof(stModeCfg));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstModeCfg, &stModeCfg, sizeof(stModeCfg));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

