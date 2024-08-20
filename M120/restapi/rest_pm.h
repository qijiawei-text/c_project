#ifndef __REST_PM_H__
#define __REST_PM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define DEF_PM_INVALID_VALUE                  (-99.00)

typedef struct REST_PM_AMP_st
{
    double dLineOut;
    double dLineIn;
    double dPaIn;
    double dPaOut;
    double dBaIn;
    double dBaOut;
    double dOscIn;
    double dOscOut;
    double dPaGain;
    /* double dPaTilt; */
    double dPaVoa;
    double dBaGain;
    /* double dBaTilt; */
    double dBaVoa;
    double dPaReturnLoss;
    double dBaReturnLoss;
} REST_PM_AMP_ST;

typedef struct REST_PM_OPS_st {
    double dOpsWInput;
    double dOpsPInput;
} REST_PM_OPS_ST;

typedef struct REST_PM_st
{
    char acTimpstamp[64];
    time_t tCurTime;
    REST_PM_AMP_ST astAmp[EN_REST_AMP_CARD_MAX];
    REST_PM_OPS_ST stOps;
} REST_PM_ST;

typedef struct REST_PM_EDFA_st {
    double dModTemp;
    double dGain;
    double dTilt;
    double dInputPower;
    double dOutputPower;
    double dVoa;
    double dVoaOutputPower;
    double dReturnLoss;
    double dReflectionPower;
    double adPumpCurrent[10];
    double adPumpTemp[10];
    uint32_t uiPumpCount;
    uint32_t uiMmPumpCount;      /* BA exist, PA NOT exist */
    double adMmPumpCurrent[10];
    double adMmPumpTemp[10];
    double dChannelInputPower;
    double dChannelOutputPower;
} REST_PM_EDFA_ST;

typedef struct REST_PM_OSC_st {
    double dAddPower;
    double dDropPower;
    double dOutputPower;
    double dInputPower;
    double dRxVoaLoss;
    /* double dTxVoaLoss; */
} REST_PM_OSC_ST;

typedef struct REST_PM_OPTICAL_st
{
    REST_PM_EDFA_ST stPa;
    REST_PM_EDFA_ST stBa;
    REST_PM_OSC_ST stOsc;
    double dLineCvTxPower;
    double dLineCvRxPower;
    double dSpanLoss;
} REST_PM_OPTICAL_ST;

typedef struct REST_PM_SCC_st {
    double dCardTemp;
} REST_PM_SCC_ST;

typedef struct REST_PM_FAN_st {
    double dFanSpeed;
} REST_PM_FAN_ST;

typedef struct REST_PM_PSU_st {
    double dTemp;
    double dInputCurrent;
    double dInputVoltage;
    double dOutputCurrent;
    double dOutputVoltage;
    double dOutputPower;
} REST_PM_PSU_ST;

typedef struct REST_PM_new_st
{
    char acTimpstamp[64];
    time_t tCurTime;
    REST_PM_OPTICAL_ST stOptical;
    REST_PM_SCC_ST stScc;
    REST_PM_FAN_ST astFan[4];
    REST_PM_PSU_ST astPsu[2];
    uint32_t uiFanCount;
    uint32_t uiPsuCount;
} REST_PM_NEW_ST;

int REST_GetCurrentPm(REST_PM_ST *pstPm);
int REST_GetCurrentPmHcf(REST_PM_NEW_ST *pstPm);

#ifdef __cplusplus
}
#endif

#endif