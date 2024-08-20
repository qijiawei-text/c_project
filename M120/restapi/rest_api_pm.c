/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_api_pm.c
* Description:   REST APIs of pm module
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2016/12/08  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "rest_common.h"
#include "rest_api_pm.h"
#include "rest_msft.h"
#include "rest_pm.h"
#include "rest_log.h"

void
API_GetCurrentPmValues(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{

    int iResult = HTTP_STATUS_CODE_200;
    cJSON *pstBody = NULL;
    char acBuf[DEF_BUF_SIZE_64] = {0};

    CLEAR_NO_USED_WARNING(pstReq);


#if defined(__HCF__)
    REST_PM_NEW_ST stPm;
    int i = 0;
    memset(&stPm, 0, sizeof(stPm));
    if (0 != REST_GetCurrentPmHcf(&stPm)) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    cJSON_AddStringToObject(pstBody, STR_TIMESTAMP, stPm.acTimpstamp);

    /* PA */
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dInputPower));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dOutputPower));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dReturnLoss));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_VOA_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dVoaOutputPower));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_TILT, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dTilt));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_REFLECTION_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dReflectionPower));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_MODULE_TEMP, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.dModTemp));

    for (i = 0; i < stPm.stOptical.stPa.uiPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PRE_AMP_PUMP_CURRENT_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.adPumpCurrent[i]));
    }

    for (i = 0; i < stPm.stOptical.stPa.uiPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PRE_AMP_PUMP_TEMP_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stPa.adPumpTemp[i]));
    }

    /* BA */
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dInputPower));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dOutputPower));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dReturnLoss));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_VOA_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dVoaOutputPower));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_TILT, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dTilt));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_REFLECTION_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dReflectionPower));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_MODULE_TEMP, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.dModTemp));

    for (i = 0; i < stPm.stOptical.stBa.uiPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_BOOSTER_AMP_PUMP_CURRENT_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.adPumpCurrent[i]));
    }

    for (i = 0; i < stPm.stOptical.stBa.uiPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_BOOSTER_AMP_PUMP_TEMP_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.adPumpTemp[i]));
    }

    for (i = 0; i < stPm.stOptical.stBa.uiMmPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_BOOSTER_AMP_MM_PUMP_CURRENT_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.adMmPumpCurrent[i]));
    }

    for (i = 0; i < stPm.stOptical.stBa.uiMmPumpCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_BOOSTER_AMP_MM_PUMP_TEMP_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stBa.adMmPumpTemp[i]));
    }

    /* OSC */
    cJSON_AddNumberToObject(pstBody, STR_OSC_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stOsc.dOutputPower));
    cJSON_AddNumberToObject(pstBody, STR_OSC_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stOsc.dInputPower));
    cJSON_AddNumberToObject(pstBody, STR_OSC_ADD_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stOsc.dAddPower));
    cJSON_AddNumberToObject(pstBody, STR_OSC_DROP_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stOsc.dDropPower));
    cJSON_AddNumberToObject(pstBody, STR_OSC_RX_VOA_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.stOsc.dRxVoaLoss));

    /* SCC */
    cJSON_AddNumberToObject(pstBody, STR_PM_SCC_CARD_TEMP, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stScc.dCardTemp));

    /* FAN */
    for (i = 0; i < stPm.uiFanCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_FAN_SPEED_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astFan[i].dFanSpeed));
    }

    /* PSU */
    for (i = 0; i < stPm.uiPsuCount; i++) {
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_TEMP_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dTemp));
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_INPUT_CURRENT_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dInputCurrent));
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_OUTPUT_CURRENT_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dOutputCurrent));
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_INPUT_VOLTAGE_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dInputVoltage));
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_OUTPUT_VOLTAGE_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dOutputVoltage));
        snprintf(acBuf, sizeof(acBuf), "%s%d", STR_PM_PSU_OUTPUT_POWER_PREFIX, i + 1);
        cJSON_AddNumberToObject(pstBody, acBuf, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.astPsu[i].dOutputPower));
    }

    /* others */
    cJSON_AddNumberToObject(pstBody, STR_PM_LINE_CV_TX_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.dLineCvTxPower));
    cJSON_AddNumberToObject(pstBody, STR_PM_LINE_CV_RX_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.dLineCvRxPower));
    cJSON_AddNumberToObject(pstBody, STR_SPAN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOptical.dSpanLoss));

#else
    REST_PM_ST stPm;
    REST_PM_AMP_ST *pstPmAmp = NULL;

    /* get data */
    memset(&stPm, 0, sizeof(stPm));
    if (0 != REST_GetCurrentPm(&stPm)) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    pstBody = pstResp->pstResponse;
    cJSON_AddStringToObject(pstBody, STR_TIMESTAMP, stPm.acTimpstamp);

    /* AMP card */
    pstPmAmp = &stPm.astAmp[EN_REST_AMP_W_CARD];
    cJSON_AddNumberToObject(pstBody, STR_LINE_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dLineOut));
    cJSON_AddNumberToObject(pstBody, STR_LINE_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dLineIn));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaIn));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaOut));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaIn));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaOut));
    cJSON_AddNumberToObject(pstBody, STR_OSC_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dOscOut));
    cJSON_AddNumberToObject(pstBody, STR_OSC_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dOscIn));
    cJSON_AddNumberToObject(pstBody, STR_PRE_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_PRE_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaReturnLoss));
    cJSON_AddNumberToObject(pstBody, STR_BOOSTER_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_BOOSTER_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaReturnLoss));
#if defined(__OLSP__)

    /* AMP-P card */
    pstPmAmp = &stPm.astAmp[EN_REST_AMP_P_CARD];
    cJSON_AddNumberToObject(pstBody, STR_LINEP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dLineOut));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dLineIn));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_PRE_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaIn));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_PRE_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaOut));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_BOOSTER_AMP_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaIn));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_BOOSTER_AMP_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaOut));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_OSC_OUTPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dOscOut));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_OSC_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dOscIn));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_PRE_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_LINEP_PRE_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_LINEP_PRE_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dPaReturnLoss));
    cJSON_AddNumberToObject(pstBody, STR_LINEP_BOOSTER_AMP_GAIN, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaGain));
    cJSON_AddNumberToObject(pstBody, STR_PM_LINEP_BOOSTER_AMP_VOA, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaVoa));
    cJSON_AddNumberToObject(pstBody, STR_PM_LINEP_BOOSTER_AMP_RETURN_LOSS, GET_2_DECIMAL_DIGITS_OF_DOUBLE(pstPmAmp->dBaReturnLoss));

    /* OPS card */
    cJSON_AddNumberToObject(pstBody, STR_PM_OPS_W_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOps.dOpsWInput));
    cJSON_AddNumberToObject(pstBody, STR_PM_OPS_P_INPUT_POWER, GET_2_DECIMAL_DIGITS_OF_DOUBLE(stPm.stOps.dOpsPInput));
#endif
#endif

END_LABEL:

    pstResp->iStatus = iResult;
    REST_USER_LOG(pstReq, pstResp);

}

static int
__BuildCurrentPmData(cJSON *pstBody, int iPmGranularity)
{
    int iResult = HTTP_STATUS_CODE_200;
    cJSON *pstJsonObj = NULL;
    cJSON *pstSubJsonObj = NULL;
    ST_DSC_PM_DATA aastPmUnit[OCP_COMPONENT_NUM_MAX][EN_DSC_PM_GRANULARITIES_MAX];
    size_t nCount = 0;
    size_t i = 0;
    char acTime[PM_STRING_LEN] = {0};
    int iInterval = 0;
    struct timeval stTv;
    struct tm *pstTm = NULL;

    if ((NULL == pstBody) || (iPmGranularity < EN_DSC_PM_1MIN || iPmGranularity >= EN_DSC_PM_GRANULARITIES_MAX)) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    /* get current time and time interval(the seconds since lastest pm statistics began) */
    gettimeofday(&stTv, NULL);
    pstTm = localtime(&stTv.tv_sec);
    if (NULL == pstTm) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }
    snprintf(acTime, sizeof(acTime), "%d-%02d-%02dT%02d:%02d:%02dZ", pstTm->tm_year + 1900,
             pstTm->tm_mon + 1, pstTm->tm_mday, pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
    if (EN_DSC_PM_15MIN == iPmGranularity) {
        iInterval = (pstTm->tm_min % 15) * 60 + pstTm->tm_sec;
    } else if (EN_DSC_PM_24HOUR == iPmGranularity) {
        iInterval = (pstTm->tm_hour * 60 + pstTm->tm_min) * 60 + pstTm->tm_sec;
    } else {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    /* get current pm data (1min 15min 24hour) */
    memset(aastPmUnit, 0, sizeof(aastPmUnit));
    iResult = DSC_GetCurrentPM(aastPmUnit, &nCount);
    if (iResult != SR_ERR_OK) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    if (0 == nCount) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    /* fill data */
    cJSON_AddStringToObject(pstBody, STR_TIMESTAMP, acTime);
    cJSON_AddNumberToObject(pstBody, STR_INTERVAL_SEC, iInterval);
    for (i = 0; i < nCount; i++) {

        pstJsonObj = cJSON_AddObjectToObject(pstBody, aastPmUnit[i][EN_DSC_PM_15MIN].acPmId);

        /* cJSON_AddStringToObject(pstJsonObj, "unit", aastPmUnit[i][EN_DSC_PM_15MIN].acUnit); */

        cJSON_AddNumberToObject(pstJsonObj, STR_AVG, GET_2_DECIMAL_DIGITS_OF_DOUBLE(aastPmUnit[i][iPmGranularity].dAvg));

        pstSubJsonObj = cJSON_AddObjectToObject(pstJsonObj, STR_MIN);
        cJSON_AddNumberToObject(pstSubJsonObj, STR_VALUE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(aastPmUnit[i][iPmGranularity].dMin));
        cJSON_AddStringToObject(pstSubJsonObj, STR_TIMESTAMP, aastPmUnit[i][iPmGranularity].acMinTime);

        pstSubJsonObj = cJSON_AddObjectToObject(pstJsonObj, STR_MAX);
        cJSON_AddNumberToObject(pstSubJsonObj, STR_VALUE, GET_2_DECIMAL_DIGITS_OF_DOUBLE(aastPmUnit[i][iPmGranularity].dMax));
        cJSON_AddStringToObject(pstSubJsonObj, STR_TIMESTAMP, aastPmUnit[i][iPmGranularity].acMaxTime);
    }

    iResult = HTTP_STATUS_CODE_200;

END_LABEL:

    return iResult;

}

void
API_GetCurrentPm15min(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;

    pstBody = pstResp->pstResponse;
    pstResp->iStatus = __BuildCurrentPmData(pstBody, EN_DSC_PM_15MIN);

    REST_USER_LOG(pstReq, pstResp);
}


void
API_GetCurrentPm24hour(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;

    pstBody = pstResp->pstResponse;
    pstResp->iStatus = __BuildCurrentPmData(pstBody, EN_DSC_PM_24HOUR);

    REST_USER_LOG(pstReq, pstResp);
}
