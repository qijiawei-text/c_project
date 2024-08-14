#include "rest_common.h"
#include "rest_msft.h"
#include "rest_pm.h"
#include <sys/time.h>
/* #include <pthread.h> */
#include <sys/sysinfo.h>
#include "rest_edfa.h"
#include "DevEdfaData.h"
#include "DevEdfaCApi.h"
#include "DevBoardData.h"
#include "DevBoardCApi.h"

#define INIT_PM_AMP_DATA(stPm) do { \
        stPm.dLineOut = DEF_PM_INVALID_VALUE; \
        stPm.dLineIn = DEF_PM_INVALID_VALUE; \
        stPm.dPaIn = DEF_PM_INVALID_VALUE; \
        stPm.dPaOut = DEF_PM_INVALID_VALUE; \
        stPm.dBaIn = DEF_PM_INVALID_VALUE; \
        stPm.dBaOut = DEF_PM_INVALID_VALUE; \
        stPm.dOscIn = DEF_PM_INVALID_VALUE; \
        stPm.dOscOut = DEF_PM_INVALID_VALUE; \
        stPm.dPaGain = DEF_PM_INVALID_VALUE; \
        stPm.dPaVoa = DEF_PM_INVALID_VALUE; \
        stPm.dBaGain = DEF_PM_INVALID_VALUE; \
        stPm.dBaVoa = DEF_PM_INVALID_VALUE; \
        stPm.dPaReturnLoss = DEF_PM_INVALID_VALUE; \
        stPm.dBaReturnLoss = DEF_PM_INVALID_VALUE; \
} while (0)

#define INIT_PM_OPS_DATA(stPm) do { \
        stPm.dOpsWInput = DEF_PM_INVALID_VALUE; \
        stPm.dOpsPInput = DEF_PM_INVALID_VALUE; \
} while (0)



static int
__REST_GetAmpPm(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_PM_AMP_ST *pstPm)
{
    REST_PM_AMP_ST stPm;
    OSC_INFO_ST stOscInfo;
    EDFA_STATE_ST stEdfaState;
    int rc = SR_ERR_OK;
    double dEdfaVoa = 0;
    char *pcCfgKey = NULL;

    if (NULL == pstPm) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&stPm, 0, sizeof(stPm));
    INIT_PM_AMP_DATA(stPm);

    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        rc = SR_ERR_OK;
        goto END_LABEL;
    }

    /* ba */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_BA_CFG_KEY : EDFA_BAP_CFG_KEY;
    memset(&stEdfaState, 0, sizeof(stEdfaState));
    rc = DSC_GetEdfaState(pcCfgKey, &stEdfaState, sizeof(EDFA_STATE_ST));
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
    stPm.dLineOut = stEdfaState.dOutputSignalPower;
    stPm.dBaIn = stEdfaState.dInputSignalPower;
    stPm.dBaOut = stEdfaState.dOutputSignalPower;
    stPm.dBaGain = stEdfaState.dGainValue;            /* db */
    stPm.dBaReturnLoss = stEdfaState.dOpticalReturnLoss;

    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PADDING_CFG_KEY : EVOA_PADDING_P_CFG_KEY;
    rc = DSC_GetEvoaActualAttn(pcCfgKey, &dEdfaVoa);        /* db */
    if (SR_ERR_OK == rc) {
        stPm.dBaVoa = dEdfaVoa;
    }

    /* pa */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_PA_CFG_KEY : EDFA_PAP_CFG_KEY;
    memset(&stEdfaState, 0, sizeof(stEdfaState));
    rc = DSC_GetEdfaState(pcCfgKey, &stEdfaState, sizeof(EDFA_STATE_ST));
    if (SR_ERR_OK == rc) {
        stPm.dLineIn = stEdfaState.dInputSignalPower;
        stPm.dPaIn = stEdfaState.dInputSignalPower;
        stPm.dPaOut = stEdfaState.dOutputSignalPower;
        stPm.dPaGain = stEdfaState.dGainValue;                 /* db */
        stPm.dPaReturnLoss = stEdfaState.dOpticalReturnLoss;
    }

    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PA_CFG_KEY : EVOA_PA_P_CFG_KEY;
    rc = DSC_GetEvoaActualAttn(EVOA_PA_CFG_KEY, &dEdfaVoa);            /* db */
    if (SR_ERR_OK == rc) {
        stPm.dPaVoa = dEdfaVoa;
    }

    /* osc */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? OSC_CFG_KEY : OSC_P_CFG_KEY;
    memset(&stOscInfo, 0x00, sizeof(stOscInfo));
    rc = DSC_GetOscInfo(pcCfgKey, &stOscInfo, sizeof(stOscInfo));
    if (SR_ERR_OK == rc) {
        stPm.dOscIn = stOscInfo.stOscDropMon.dPower;            /* dbm */
        stPm.dOscOut = stOscInfo.stOscAddMon.dPower;            /* equip with stOscInfo.stOscTxMon.dPower; / * dbm * / */
    }

END_LABEL:

    memcpy(pstPm, &stPm, sizeof(stPm));

    return rc;
}

static int
__REST_GetOpsPm(REST_PM_OPS_ST *pstPm)
{
    REST_PM_OPS_ST stPm;
    OPS_PORT_STATE_ST stPortState;
    OPS_PORT_EN enOpsPort;
    int rc = SR_ERR_OK;

    if (NULL == pstPm) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&stPm, 0, sizeof(stPm));
    INIT_PM_OPS_DATA(stPm);

    if (false == DSC_IsCircuitPackInService(CIRCUIT_PACK_OPS)) {
        rc = SR_ERR_OK;
        goto END_LABEL;
    }

    memset(&stPortState, 0, sizeof(stPortState));
    enOpsPort = OPS_PORT_LINE_PRIMARY_IN;
    rc = DSC_GetOpsPortState(OPS_CFG_KEY, enOpsPort, &stPortState, sizeof(OPS_PORT_STATE_ST));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsPortState: %d", enOpsPort);
    } else {
        stPm.dOpsWInput = stPortState.stPortPwr.dInstant;
    }

    memset(&stPortState, 0, sizeof(stPortState));
    enOpsPort = OPS_PORT_LINE_SECONDARY_IN;
    rc = DSC_GetOpsPortState(OPS_CFG_KEY, enOpsPort, &stPortState, sizeof(OPS_PORT_STATE_ST));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsPortState: %d", enOpsPort);
    } else {
        stPm.dOpsPInput = stPortState.stPortPwr.dInstant;
    }

END_LABEL:
    memcpy(pstPm, &stPm, sizeof(stPm));
    return rc;
}

static int
__REST_GetCurrentPm(REST_PM_ST *pstPm)
{
    REST_PM_ST stPm;
    int rc = SR_ERR_OK;

    if (NULL == pstPm) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stPm, 0, sizeof(stPm));

    __REST_GetAmpPm(EN_REST_AMP_W_CARD, &(stPm.astAmp[EN_REST_AMP_W_CARD]));

    __REST_GetAmpPm(EN_REST_AMP_P_CARD, &(stPm.astAmp[EN_REST_AMP_P_CARD]));

    __REST_GetOpsPm(&stPm.stOps);

    memcpy(pstPm, &stPm, sizeof(stPm));

ERR_LABEL:

    return rc;
}

#define INIT_PM_OPTICAL_DATA(stPm) do { \
        stPm.stBa.dGain = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dTilt = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dInputPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dVoa = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dVoaOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dReturnLoss = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dReflectionPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dChannelInputPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.dChannelOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stBa.uiPumpCount = 3; \
        stPm.stBa.uiMmPumpCount = 3; \
        stPm.stPa.dGain = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dTilt = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dInputPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dVoa = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dVoaOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dReturnLoss = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dReflectionPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dChannelInputPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.dChannelOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stPa.uiPumpCount = 3; \
        stPm.stOsc.dAddPower = DEF_PM_INVALID_VALUE; \
        stPm.stOsc.dDropPower = DEF_PM_INVALID_VALUE; \
        stPm.stOsc.dOutputPower = DEF_PM_INVALID_VALUE; \
        stPm.stOsc.dInputPower = DEF_PM_INVALID_VALUE; \
        stPm.stOsc.dRxVoaLoss = DEF_PM_INVALID_VALUE; \
        stPm.dLineCvTxPower = DEF_PM_INVALID_VALUE; \
        stPm.dLineCvRxPower = DEF_PM_INVALID_VALUE; \
        stPm.dSpanLoss = DEF_PM_INVALID_VALUE; \
} while (0)


static int
__REST_GetOpticalPm(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_PM_OPTICAL_ST *pstPm)
{
    REST_PM_OPTICAL_ST stPm;
    OSC_INFO_ST stOscInfo;
    EDFA_STATE_ST stEdfaState;
    int rc = SR_ERR_OK;
    double dVoa = 0;
    char *pcCfgKey = NULL;
    int i = 0;
    COaStatusData stEdfaStatus;
    AUTO_GAIN_STATE_ST stAutoGainState;
    DFB_INFO_ST stDfbInfo;
    ST_OC_COMPONENT_PORT stOcComponentPortsOut[OCP_COMPONENT_NUM_MAX];
    size_t nCount = 0;

    if (NULL == pstPm) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&stPm, 0, sizeof(stPm));
    INIT_PM_OPTICAL_DATA(stPm);

    if (false == REST_IsAmpCardOnline(enAmpCardType)) {
        rc = SR_ERR_OK;
        goto END_LABEL;
    }

    /* ba */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_BA_CFG_KEY : EDFA_BAP_CFG_KEY;
    memset(&stEdfaState, 0, sizeof(stEdfaState));
    rc = DSC_GetEdfaState(pcCfgKey, &stEdfaState, sizeof(EDFA_STATE_ST));
    if (SR_ERR_OK == rc) {
        stPm.stBa.dGain = stEdfaState.dGainValue;            /* db */
        stPm.stBa.dInputPower = stEdfaState.dInputSignalPower;
        stPm.stBa.dOutputPower = stEdfaState.dOutputSignalPower;
        stPm.stBa.dReflectionPower = stEdfaState.dBackRefPower;
        stPm.stBa.dReturnLoss = stEdfaState.dOpticalReturnLoss;
        stPm.stBa.dTilt = stEdfaState.dTiltValue;
    }

    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PADDING_CFG_KEY : EVOA_PADDING_P_CFG_KEY;
    rc = DSC_GetEvoaActualAttn(pcCfgKey, &dVoa);        /* db */
    if (SR_ERR_OK == rc) {
        stPm.stBa.dVoa = dVoa;    /* padding-voa */
    }

    memset(&stEdfaStatus, 0, sizeof(stEdfaStatus));
    rc = EDFA_GetStatusData(DEV_TYPE_BA, &stEdfaStatus);
    if (OPLK_OK == rc) {
        stPm.stBa.dModTemp = (double)stEdfaStatus.stOaCurData.asCaseTemp[0] / WEIGHT_TEMPERATURE;

        stPm.stBa.uiPumpCount = 3;
        for (i = 0; i < stPm.stBa.uiPumpCount; i++) {
            stPm.stBa.adPumpCurrent[i] = (double)stEdfaStatus.stOaCurData.asPumpCurrent[i] / WEIGHT_CURRENT;
            stPm.stBa.adPumpTemp[i] = (double)stEdfaStatus.stOaCurData.asPumpTemp[i] / WEIGHT_TEMPERATURE;
        }

        stPm.stBa.uiMmPumpCount = 4;
        for (i = 3; i < stPm.stBa.uiMmPumpCount + 3; i++) {
            stPm.stBa.adMmPumpCurrent[i - 3] = (double)stEdfaStatus.stOaCurData.asPumpCurrent[i] / WEIGHT_CURRENT;
            stPm.stBa.adMmPumpTemp[i - 3] = (double)stEdfaStatus.stOaCurData.asPumpTemp[i] / WEIGHT_TEMPERATURE;
        }

        stPm.stBa.dVoaOutputPower = (double)stEdfaStatus.stOaCurData.sPortOutTotalPower / WEIGHT_HUNDREDTH;
    }

    /* pa */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_PA_CFG_KEY : EDFA_PAP_CFG_KEY;
    memset(&stEdfaState, 0, sizeof(stEdfaState));
    rc = DSC_GetEdfaState(pcCfgKey, &stEdfaState, sizeof(EDFA_STATE_ST));
    if (SR_ERR_OK == rc) {
        stPm.stPa.dGain = stEdfaState.dGainValue;                                /* db */
        stPm.stPa.dInputPower = stEdfaState.dInputSignalPower;
        stPm.stPa.dOutputPower = stEdfaState.dOutputSignalPower;
        stPm.stPa.dReflectionPower = stEdfaState.dBackRefPower;
        stPm.stPa.dReturnLoss = stEdfaState.dOpticalReturnLoss;
        stPm.stPa.dTilt = stEdfaState.dTiltValue;
    }

    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PA_CFG_KEY : EVOA_PA_P_CFG_KEY;
    rc = DSC_GetEvoaActualAttn(EVOA_PA_CFG_KEY, &dVoa);            /* db */
    if (SR_ERR_OK == rc) {
        stPm.stPa.dVoa = dVoa;      /* padding-voa */
    }

    memset(&stEdfaStatus, 0, sizeof(stEdfaStatus));
    rc = EDFA_GetStatusData(DEV_TYPE_PA, &stEdfaStatus);
    if (OPLK_OK == rc) {
        stPm.stPa.dModTemp = (double)stEdfaStatus.stOaCurData.asCaseTemp[0] / WEIGHT_TEMPERATURE;

        stPm.stPa.uiPumpCount = 3;
        for (i = 0; i < stPm.stPa.uiPumpCount; i++) {
            stPm.stPa.adPumpCurrent[i] = (double)stEdfaStatus.stOaCurData.asPumpCurrent[i] / WEIGHT_CURRENT;
            stPm.stPa.adPumpTemp[i] = (double)stEdfaStatus.stOaCurData.asPumpTemp[i] / WEIGHT_TEMPERATURE;
        }

        /* stPm.stPa.dVoaOutputPower = (double)stEdfaStatus.stOaCurData.sPortOutTotalPower / WEIGHT_HUNDREDTH; */
    }

    /* osc */
    pcCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? OSC_CFG_KEY : OSC_P_CFG_KEY;
    memset(&stOscInfo, 0x00, sizeof(stOscInfo));
    rc = DSC_GetOscInfo(pcCfgKey, &stOscInfo, sizeof(stOscInfo));
    if (SR_ERR_OK == rc) {
        stPm.stOsc.dAddPower = stOscInfo.stOscAddMon.dPower;
        stPm.stOsc.dDropPower = stOscInfo.stOscDropMon.dPower;
        stPm.stOsc.dOutputPower = stOscInfo.stOscTxMon.dPower;
        stPm.stOsc.dInputPower = stOscInfo.stOscRxMon.dPower;

    }

    dVoa = 0;
    rc = DSC_GetEvoaActualAttn(EVOA_OSC_CFG_KEY, &dVoa);
    if (SR_ERR_OK == rc) {
        stPm.stOsc.dRxVoaLoss = dVoa;
    }

    memset(&stDfbInfo, 0, sizeof(DFB_INFO_ST));
    rc = DSC_GetDfbInfo("dfb-hp", &stDfbInfo, sizeof(DFB_INFO_ST));
    if (SR_ERR_OK == rc) {
        stPm.dLineCvRxPower = stDfbInfo.stDfbLineInMon.dInputPower;
        stPm.dLineCvTxPower = stDfbInfo.stDfbLineOutMon.dOutputPower;
    }

    /* span-loss */
    memset(&stAutoGainState, 0, sizeof(stAutoGainState));
    rc = DSC_AutoGetGainState(WORKING_LINE_YANG, &stAutoGainState);
    if (SR_ERR_OK == rc) {
        stPm.dSpanLoss = stAutoGainState.dSpanLoss;
    }

    memset(stOcComponentPortsOut, 0, sizeof(stOcComponentPortsOut));
    rc = DSC_OCP_GetOpticalPortPowerOutInfo(stOcComponentPortsOut, &nCount);
    if (SR_ERR_OK == rc) {
        for (i = 0; i < nCount; i++) {
            if (0 == strcmp(stOcComponentPortsOut[i].acName, "mux-out")) {
                stPm.stPa.dVoaOutputPower = stOcComponentPortsOut[i].stOptiaclPower.dInstant;
            } else if (0 == strcmp(stOcComponentPortsOut[i].acName, "line-out")) {
                stPm.stBa.dVoaOutputPower = stOcComponentPortsOut[i].stOptiaclPower.dInstant;
            }
        }
    }

END_LABEL:

    memcpy(pstPm, &stPm, sizeof(stPm));

    return rc;
}

static int
__REST_GetSccPm(REST_PM_SCC_ST *pstPm)
{

#if 0
    SHELF_TEMP_INFO_ST stTempInfo;
    int rc = SR_ERR_OK;


    memset(&stTempInfo, 0, sizeof(stTempInfo));
    rc = DSC_SHELF_GetTemperature(&stTempInfo);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }
    pstPm->dCardTemp = stTempInfo.dBoardTemp;
#endif

    int iResult = 0;
    CBoardStatusData stData;

    memset(&stData, 0, sizeof(CBoardStatusData));

    iResult = BOARD_GetStatusData(&stData);
    if (iResult < 0) {
        return OPLK_ERR;
    }

    pstPm->dCardTemp = (double)stData.aiTemp[0] / WEIGHT_TEMPERATURE;

    return OPLK_OK;
}

static int
__REST_GetFanPm(REST_PM_FAN_ST *pstPm, uint32_t *puiCount)
{
    int iResult = 0;
    CBoardStatusData stData;
    size_t i = 0;

    *puiCount = 4;

    memset(&stData, 0, sizeof(CBoardStatusData));
    iResult = BOARD_GetStatusData(&stData);
    if (iResult < 0) {
        return OPLK_ERR;
    }

    for (i = 0; i < *puiCount; i++) {
        pstPm[i].dFanSpeed = stData.astFan[i].uiSpeed;
    }

    return OPLK_OK;
}

static int
__REST_GetPsuPm(REST_PM_PSU_ST *pstPm, uint32_t *puiCount)
{
    int iResult = 0;
    CBoardStatusData stData;
    size_t i = 0;

    *puiCount = 2;
    memset(&stData, 0, sizeof(CBoardStatusData));
    iResult = BOARD_GetStatusData(&stData);
    if (iResult < 0) {
        return OPLK_ERR;
    }

    for (i = 0; i < *puiCount; i++) {
        pstPm[i].dInputVoltage = (double)stData.astPower[i].usInVol / WEIGHT_HUNDREDTH;
        pstPm[i].dOutputVoltage = (double)stData.astPower[i].usOutVol / WEIGHT_HUNDREDTH;

        pstPm[i].dInputCurrent = (double)stData.astPower[i].uiInCurrent / WEIGHT_CURRENT;
        pstPm[i].dOutputCurrent = (double)stData.astPower[i].uiOutCurrent / WEIGHT_CURRENT;

        pstPm[i].dTemp = (double)stData.astPower[i].iTemp / WEIGHT_TEMPERATURE;

        pstPm[i].dOutputPower = (double)stData.astPower[i].uiWatts / WEIGHT_TEMPERATURE;
    }

    return OPLK_OK;

}

static int
__REST_GetCurrentPmHcf(REST_PM_NEW_ST *pstPm)
{
    REST_PM_NEW_ST stPm;
    int rc = SR_ERR_OK;

    if (NULL == pstPm) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stPm, 0, sizeof(stPm));

    __REST_GetOpticalPm(EN_REST_AMP_W_CARD, &(stPm.stOptical));

    __REST_GetSccPm(&stPm.stScc);

    __REST_GetFanPm(stPm.astFan, &stPm.uiFanCount);

    __REST_GetPsuPm(stPm.astPsu, &stPm.uiPsuCount);

    memcpy(pstPm, &stPm, sizeof(stPm));

ERR_LABEL:

    return rc;
}

int
REST_GetCurrentPm(REST_PM_ST *pstPm)
{
    REST_PM_ST stPm;
    char acTimpstamp[64] = {0};
    int rc = SR_ERR_OK;
    time_t now;
    struct tm *pstTmNow = NULL;

    if (NULL == pstPm) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stPm, 0, sizeof(stPm));

    time(&now);
    pstTmNow = localtime(&now);
    snprintf(acTimpstamp, sizeof(acTimpstamp), "%4d-%02d-%02dT%02d:%02d:%02dZ",
             pstTmNow->tm_year + 1900, pstTmNow->tm_mon + 1, pstTmNow->tm_mday, pstTmNow->tm_hour,
             pstTmNow->tm_min, pstTmNow->tm_sec);

    rc = __REST_GetCurrentPm(&stPm);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }
    memcpy(stPm.acTimpstamp, acTimpstamp, sizeof(acTimpstamp));
    memcpy(pstPm, &stPm, sizeof(stPm));

ERR_LABEL:

    return rc;
}

int
REST_GetCurrentPmHcf(REST_PM_NEW_ST *pstPm)
{
    REST_PM_NEW_ST stPm;
    char acTimpstamp[64] = {0};
    int rc = SR_ERR_OK;
    time_t now;
    struct tm *pstTmNow = NULL;

    if (NULL == pstPm) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(&stPm, 0, sizeof(stPm));

    time(&now);
    pstTmNow = localtime(&now);
    snprintf(acTimpstamp, sizeof(acTimpstamp), "%4d-%02d-%02dT%02d:%02d:%02dZ",
             pstTmNow->tm_year + 1900, pstTmNow->tm_mon + 1, pstTmNow->tm_mday, pstTmNow->tm_hour,
             pstTmNow->tm_min, pstTmNow->tm_sec);

    rc = __REST_GetCurrentPmHcf(&stPm);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }
    memcpy(stPm.acTimpstamp, acTimpstamp, sizeof(acTimpstamp));
    memcpy(pstPm, &stPm, sizeof(stPm));

ERR_LABEL:

    return rc;
}

