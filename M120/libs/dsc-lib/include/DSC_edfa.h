#ifndef _DSC_EDFA_H
#define _DSC_EDFA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define EVOA_PADDING_CFG_KEY      "PADDING-VOA"
#define EVOA_PA_CFG_KEY           "PA-VOA"
#define EVOA_PADDING_P_CFG_KEY    "PADDING-VOA-P"
#define EVOA_PA_P_CFG_KEY         "PA-VOA-P"
#define EVOA_ASE_CFG_KEY          "ASE-VOA"
#define EVOA_OSC_CFG_KEY          "OSC-VOA"
#define EVOA_OSC_TX_CFG_KEY       "OSC-TX-VOA"
#define EVOA_OSC_RX_CFG_KEY       "OSC-RX-VOA"
#define EVOA_OSC_P_CFG_KEY        "OSC-VOA-P"
#define EVOA_OSC_TX_P_CFG_KEY     "OSC-TX-VOA-P"
#define EVOA_OSC_RX_P_CFG_KEY     "OSC-RX-VOA-P"
#define EVOA_OCM_CFG_KEY          "OCM-VOA"
#define EVOA_OCM_P_CFG_KEY        "OCM-VOA-P"


#define EVOA_OSC1_CFG_KEY         "OSC1-VOA"
#define EVOA_OSC2_CFG_KEY         "OSC2-VOA"
#define EDFA_KEY_MAX            (8)

#ifndef DEFINED_EDFA_IDX_EN
#define DEFINED_EDFA_IDX_EN

typedef enum
{
    EDFA_IDX_MIN = 0,
    EDFA_IDX_1,
    EDFA_IDX_2,
    EDFA_IDX_3,
    EDFA_IDX_MAX,

    EN_EDFA_IDX_PA = 0,
    EN_EDFA_IDX_BA = 1,
    EN_EDFA_IDX_PA_P = 2,
    EN_EDFA_IDX_BA_P = 3,
    EN_EDFA_IDX_OSC,
    EN_EDFA_IDX_MAX

} EN_EDFA_IDX;

#endif

typedef struct  st_edfa_key {
    int32_t iCnt;
    char aacEdfaKeys[EDFA_KEY_MAX][8];
} EDFA_KEY_ST;

typedef struct  st_voa_key {
    int32_t iCnt;
    char aacVoaKeys[EDFA_KEY_MAX][16];
} VOA_KEY_ST;

typedef struct
{
    char acEvoaKey[32];
    unsigned char ucId;
} ST_EVOA_KEY;

#define EDFA_PA_CFG_KEY      "PA"
#define EDFA_BA_CFG_KEY      "BA"
#define EDFA_PAP_CFG_KEY     "PA-P"
#define EDFA_BAP_CFG_KEY     "BA-P"
#define EDFA_OSC_KEY         "OSC"
#define EDFA_OSC_P_KEY       "OSC-P"
#define EDFA_ASE_CFG_KEY     "ASE"
#define EDFA_OSC_CFG_KEY     "OSC"

#define DEF_PORT_CLIENT_R_CFG_KEY       "CLIENT-R"
#define DEF_PORT_CLIENT_R_P_CFG_KEY     "CLIENT-R-P"
#define DEF_PORT_CLIENT_T_CFG_KEY       "CLIENT-T"
#define DEF_PORT_CLIENT_T_P_CFG_KEY     "CLIENT-T-P"
#define DEF_PORT_LINE_R_CFG_KEY         "LINE-R"
#define DEF_PORT_LINE_R_P_CFG_KEY       "LINE-R-P"
#define DEF_PORT_LINE_T_CFG_KEY         "LINE-T"
#define DEF_PORT_LINE_T_P_CFG_KEY       "LINE-T-P"
#define DEF_BA_PUMP1_CFG_KEY            "BA-PUMP1"
#define DEF_BA_PUMP2_CFG_KEY            "BA-PUMP2"
#define DEF_BA_PUMP3_CFG_KEY            "BA-PUMP3"
#define DEF_BA_P_PUMP1_CFG_KEY          "BA-P-PUMP1"
#define DEF_BA_P_PUMP2_CFG_KEY          "BA-P-PUMP2"
#define DEF_BA_P_PUMP3_CFG_KEY          "BA-P-PUMP3"
#define DEF_PA_PUMP1_CFG_KEY            "PA-PUMP1"
#define DEF_PA_PUMP2_CFG_KEY            "PA-PUMP2"
#define DEF_PA_P_PUMP1_CFG_KEY          "PA-P-PUMP1"
#define DEF_PA_P_PUMP2_CFG_KEY          "PA-P-PUMP2"

#define WEIGHT_HUNDREDTH        (100)
#define WEIGHT_CURRENT          (10)
#define WEIGHT_TEMPERATURE      (10)

#define NO_POWER                (-60.00)
#define LOW_POWER_THR           (-45.00)


/*
 * For rainier, BA gain range: [7.50 + 7.00, 13.50 + 7.00]
 * 7.00 ---- edfa 3 fixed default gain value.
 */
#define RAINIER_EDFA_BA_GAIN_LOW    (14.50)
#define RAINIER_EDFA_BA_GAIN_HIGH   (20.50)
#define RAINIER_EDFA_3_GAIN_DEFAULT (7.00)

#define EDFA_STATE_NORMAL_STR   "in-service"
#define EDFA_STATE_DISABLED_STR "out-of-service"
#define EDFA_STATE_AUTO_STC_STR "auto-power-shutdown"
#define EDFA_STATE_STANDBY_STR  "standby"
#define EDFA_STATE_PLIM_STR     "plim"
#define EDFA_STATE_GLIM_STR     "glim"
#define EDFA_STATE_APR_STR      "apr"

#define AMP_NAME_BA             "ba"
#define AMP_NAME_BA_G1          "ba-G1"
#define AMP_NAME_BA_G2          "ba-G2"

typedef enum en_EDFA_STATE
{
    EDFA_STATE_NORMAL = 0,
    EDFA_STATE_DISABLED,
    EDFA_STATE_AUTO_STC,
    EDFA_STATE_STANDBY,
    EDFA_STATE_PLIM,
    EDFA_STATE_GLIM,
    EDFA_STATE_APR
} EDFA_STATE_EN;

#define AMP_MODE_CONSTANT_GAIN      "CONSTANT_GAIN"
#define AMP_MODE_CONSTANT_POWER     "CONSTANT_POWER"
#define AMP_MODE_CONSTANT_CURRENT   "CONSTANT_CURRENT"

#ifndef EDFA_MODE_EN_DEFINED
typedef enum EDFA_MODE_en
{
    EDFA_MODE_AGC = 1,
    EDFA_MODE_APC,
    EDFA_MODE_ACC,
    EDFA_MODE_MAX
} EDFA_MODE_EN;
#define EDFA_MODE_EN_DEFINED
#endif


#define DEF_AMP_LOW_GAIN_STR      "LOW_GAIN_RANGE"
#define DEF_AMP_MID_GAIN_STR      "MID_GAIN_RANGE"
#define DEF_AMP_HIGH_GAIN_STR     "HIGH_GAIN_RANGE"
#define DEF_AMP_FIXED_GAIN_STR    "FIXED_GAIN_RANGE"

typedef enum en_EDFA_GAIN_RANGE
{
    EDFA_GAIN_RANGE_LOW = 0,
    EDFA_GAIN_RANGE_HIGH,
    EDFA_GAIN_RANGE_MID,
    EDFA_GAIN_FIXED,
    EDFA_GAIN_RANGE_ERROR,
} EDFA_GAIN_RANGE_EN;

/* HCF beta amplifier state machine mode
   1: Manual Mode
   0: Auto Mode
   Default:
   BA: Manual Mode
   PA: Auto Mode
 */
#define AMP_FSM_MODE_AUTO_STR            "auto"
#define AMP_FSM_MODE_MANUAL_STR          "manual"
#define AMP_FSM_MODE_ERR_STR             "na"
typedef enum AMP_FSM_MODE_en
{
    AMP_FSM_MODE_AUTO = 0,
    AMP_FSM_MODE_MANUAL,
    AMP_FSM_MODE_MAX
} AMP_FSM_MODE_EN;

/* HCF beta amplifier state machine state
   0:Disable_Mode,
   1:Normal_Mode,
   2:Safety_Mode,
   3:Ready_Mode
 */
#define AMP_FSM_STATE_DISABLE_STR        "disable"
#define AMP_FSM_STATE_NORMAL_STR         "normal"
#define AMP_FSM_STATE_SAFETY_STR         "safety"
#define AMP_FSM_STATE_READY_STR          "ready"
#define AMP_FSM_STATE_ERR_STR            "na"
typedef enum AMP_FSM_STATE_en
{
    AMP_FSM_STATE_DISABLE = 0,
    AMP_FSM_STATE_NORMAL,
    AMP_FSM_STATE_SAFETY,
    AMP_FSM_STATE_READY,
    AMP_FSM_STATE_MAX
} AMP_FSM_STATE_EN;

#define MAX_NUM_PUMPS    8
typedef struct st_EDFA_STATE
{
    double dBoardTemp;
    double dGainValue;
    double dTiltValue;
    double dInputPower;
    double dInputSignalPower;  /* c-band power */
    double dInputLPower;       /* l-band power */
    double dOutputSignalPower; /* c-band power */
    double dOutputLPower;      /* l-band power */
    double dOutputTotalPower;
    double dDcmInputPower;
    double dDcmOutputPower;
    double dMsaInputPower;
    double dMsaOutputPower;
    double dBackRefPower;
    double dLaserBiasCurrent;
    double dOpticalReturnLoss;
    double dEdfaPumpTemp;
    EDFA_STATE_EN enState;
    double dLaserBiasCurrents[MAX_NUM_PUMPS];
    AMP_FSM_MODE_EN enFsmMode;
    AMP_FSM_STATE_EN enFsmCurrent;
} EDFA_STATE_ST;

#define AMP_TYPE_EDFA_STR            "EDFA"
#define AMP_TYPE_FORWARD_RAMAN_STR   "FORWARD_RAMAN"
#define AMP_TYPE_BACKWARD_RAMAN_STR  "BACKWARD_RAMAN"
typedef enum {
    AMP_TYPE_EDFA,
    AMP_TYPE_FORWARD_RAMAN,
    AMP_TYPE_BACKWARD_RAMAN
} AMP_TYPE_EN;

#ifndef DEFINED_EVOA_IDX_EN
#define DEFINED_EVOA_IDX_EN
typedef enum
{
    EN_EVOA_IDX_BA = 0,
    EN_EVOA_IDX_PA = 1,
    EN_EVOA_IDX_PA_P = 2,
    EN_EVOA_IDX_BA_P = 3,
    EN_EVOA_IDX_OSC_TX = 4,
    EN_EVOA_IDX_OSC_TX_P = 5,
    EN_EVOA_IDX_MAX

} EN_EVOA_IDX;
#endif


typedef struct st_EDFA_CFG
{
    char acName[32];   /* openconfig */
    AMP_TYPE_EN enType;     /* openconfig */
    double dGainSetPoint;
    double dTiltSetPoint;
    double dInLosHighThr;
    double dInLosLowThr;
    double dOutLosHighThr;
    double dOutLosLowThr;
    double dDcmInHighThr;
    double dDcmInLowThr;
    double dDcmOutHighThr;
    double dDcmOutLowThr;
    double dMsaInHighThr;
    double dMsaInLowThr;
    double dMsaOutHighThr;
    double dMsaOutLowThr;
    double dBackRefRatio;
    double dDcmTargetLos;
    double dAprPower;
    double dGlimPower;
    double dPlimPower;
    double dTargetPower;
    bool bOutputEnable;
    bool bAutoLosEnable;
    bool bAprEnable;
    bool bStandbyEnable;
    bool bPlimEnable;
    EDFA_MODE_EN enMode;
    EDFA_GAIN_RANGE_EN enGainRange;
    double dLaserBiasCurrents[MAX_NUM_PUMPS];
    uint8_t ucNumOfPumps;
    AMP_FSM_MODE_EN enFsmMode;
    double dReadyPower;
} EDFA_CFG_ST;

/* EVOA */
typedef enum {
    EN_VOA_IDX_PADDING = 0,
    EN_VOA_IDX_PA = 1,
    EN_VOA_IDX_OSC_TX = 2,
    EN_VOA_IDX_OCM = 3,
    EN_VOA_IDX_PADDING_P = 4,
    EN_VOA_IDX_PA_P = 5,
    EN_VOA_IDX_OSC_TX_P = 6,
    EN_VOA_IDX_OCM_P = 7,
    EN_VOA_IDX_OSC_RX = 8,
    EN_VOA_IDX_OSC_RX_P = 9,

    EN_VOA_IDX_MAX
} EN_VOA_IDX;

#define MODULE_NS_EVOA                     "openconfig-optical-attenuator"
#define ATTN_MAX_VAL                       (2550)
#define EVOA_MAX_MON_ATTR                  (8)
#define EVOA_XPATH_HEADER                  "/openconfig-optical-attenuator:optical-attenuator"
#define EVOA_XPATH_CFG                     EVOA_XPATH_HEADER "/attenuators/attenuator[name='%s']/config"
#define EVOA_XPATH_ALL_CFG                 EVOA_XPATH_HEADER "/attenuators/attenuator/config//*"
#define EVOA_ATTN_NODE                     "/attenuation"
#define EVOA_TARGET_NODE                   "/target-output-power"
#define EVOA_ENABLE_NODE                   "/enabled"
#define EVOA_XPATH_ATTN                    EVOA_XPATH_CFG EVOA_ATTN_NODE
#define EVOA_XPATH_ENABLE                  EVOA_XPATH_CFG EVOA_ENABLE_NODE
#define EVOA_XPATH_TARGET                  EVOA_XPATH_CFG EVOA_TARGET_NODE
#define EVOA_ATTN_ACTUAL_NODE              "/actual-attenuation/instant"
#define EVOA_XPATH_STATE                   EVOA_XPATH_HEADER "/attenuators/attenuator[name='%s']/state"
#define EVOA_XPATH_ATTN_AUTUAL             EVOA_XPATH_STATE EVOA_ATTN_ACTUAL_NODE
#define EVOA_XPATH_TARGET_AUTUAL           EVOA_XPATH_STATE EVOA_TARGET_NODE
#define EVOA_XPATH_ATTN_AUTUAL_AVG         EVOA_XPATH_STATE "/actual-attenuation/avg"
#define EVOA_XPATH_ATTN_AUTUAL_MIN         EVOA_XPATH_STATE "/actual-attenuation/min"
#define EVOA_XPATH_ATTN_AUTUAL_MAX         EVOA_XPATH_STATE "/actual-attenuation/max"
#define EVOA_XPATH_ATTN_AUTUAL_INTERVAL    EVOA_XPATH_STATE "/actual-attenuation/interval"
#define EVOA_XPATH_ATTN_AUTUAL_MIN_TIME    EVOA_XPATH_STATE "/actual-attenuation/min-time"
#define EVOA_XPATH_ATTN_AUTUAL_MAX_TIME    EVOA_XPATH_STATE "/actual-attenuation/max-time"
#define EVOA_XPATH_NAME_ALL                EVOA_XPATH_HEADER "/attenuators/attenuator/name"

#define EVOA_XPATH_LIST                    EVOA_XPATH_HEADER_I "/attenuator"
/* openconfig amplifier */
/* Note that use the module name as the namespace, but not prefix 'oc-opt-amp' */
#define _OPENCONFIG   1
#define MODULE_NS_AMP                     "openconfig-optical-amplifier"
#define AMP_XPATH_HEADER                  "/openconfig-optical-amplifier:optical-amplifier/amplifiers"
#define AMP_XPATH_HEADER_ALL              "/openconfig-optical-amplifier:optical-amplifier"

#define AMP_XPATH                         AMP_XPATH_HEADER "//*"
#define AMP_NAME                          AMP_XPATH_HEADER "/amplifiers/amplifier[name='%s']/name"

#define AMP_NAME_XPATH_KEY                "]/name"

/* openconfig amplifier config */
#define AMP_CFG_ATTR_COUNT                10
#define AMP_XPATH_CFG                     AMP_XPATH_HEADER "/amplifier[name='%s']/config"
#define AMP_XPATH_CFG_ALL                 AMP_XPATH_CFG "//*"
#define AMP_XPATH_ALL_CFG                 AMP_XPATH_HEADER "/amplifier/config//*"
#define AMP_CFG_NAME                      "/name"
#define AMP_XPATH_NAME                    AMP_XPATH_CFG AMP_CFG_NAME
#define AMP_XPATH_NAME_ALL                AMP_XPATH_HEADER "/amplifier/name"
#define AMP_CFG_TYPE                      "/type"
#define AMP_XPATH_TYPE                     AMP_XPATH_CFG AMP_CFG_TYPE
#define AMP_CFG_GAIN                      "/target-gain"
#define AMP_XPATH_GAIN                    AMP_XPATH_CFG AMP_CFG_GAIN
#define AMP_CFG_GAIN_TILT                 "/target-gain-tilt"
#define AMP_XPATH_TILT                    AMP_XPATH_CFG AMP_CFG_GAIN_TILT
#define AMP_CFG_GAIN_RANGE                "/gain-range"
#define AMP_XPATH_GAIN_RANGE              AMP_XPATH_CFG AMP_CFG_GAIN_RANGE
#define AMP_CFG_MODE                      "/amp-mode"
#define AMP_XPATH_MODE                    AMP_XPATH_CFG AMP_CFG_MODE
#define AMP_CFG_TARGET_POWER              "/target-output-power"
#define AMP_XPATH_TARGET_POWER            AMP_XPATH_CFG AMP_CFG_TARGET_POWER
#define AMP_CFG_ENABLE                    "/enabled"
#define AMP_XPATH_ENABLE                  AMP_XPATH_CFG AMP_CFG_ENABLE
#define AMP_CFG_AUTOLOS_ENABLE            "/com-oplink-amplifier-ext:autolos"
#define AMP_XPATH_AUTOLOS_ENABLE          AMP_XPATH_CFG AMP_CFG_AUTOLOS_ENABLE
#define AMP_CFG_APR_POWER                 "/com-oplink-amplifier-ext:apr-power"
#define AMP_XPATH_APR_POWER               AMP_XPATH_CFG AMP_CFG_APR_POWER
#define AMP_CFG_APR_ENABLE                "/com-oplink-amplifier-ext:apr-enabled"
#define AMP_XPATH_APR_ENABLE              AMP_XPATH_CFG AMP_CFG_APR_ENABLE
#define AMP_CFG_PLIM_POWER                "/com-oplink-amplifier-ext:plim-power"
#define AMP_XPATH_PLIM_POWER              AMP_XPATH_CFG AMP_CFG_PLIM_POWER
#define AMP_CFG_PLIM_ENABLE               "/com-oplink-amplifier-ext:plim-enabled"
#define AMP_XPATH_PLIM_ENABLE             AMP_XPATH_CFG AMP_CFG_PLIM_ENABLE
#define AMP_CFG_CURRENT_NODE              "/current"
#define AMP_CFG_CURRENT                   "/com-oplink-amplifier-ext:laser-bias-currents[id='%d']/current"
#define AMP_XPATH_CURRENT                 AMP_XPATH_CFG AMP_CFG_CURRENT
#define AMP_CFG_NUM_OF_PUMPS              "/com-oplink-amplifier-ext:number-of-pumps"
#define AMP_XPATH_NUM_OF_PUMPS            AMP_XPATH_CFG AMP_CFG_NUM_OF_PUMPS

/* openconfig amplifier state */
#define AMP_MAX_MON_ATTR                  20
#define AMP_STATE_ATTR_COUNT              40 /* 37 remove msa power, pump temperature */
#define AMP_XPATH_STATE                   AMP_XPATH_HEADER "/amplifier[name='%s']/state"
#define AMP_XPATH_STATE_ALL               AMP_XPATH_STATE "//*"
#define AMP_STATE_GAIN                    AMP_XPATH_STATE "/actual-gain/instant"
#define AMP_STATE_GAIN_AVG                AMP_XPATH_STATE "/actual-gain/avg"
#define AMP_STATE_GAIN_MIN                AMP_XPATH_STATE "/actual-gain/min"
#define AMP_STATE_GAIN_MAX                AMP_XPATH_STATE "/actual-gain/max"
#define AMP_STATE_GAIN_INTERVAL           AMP_XPATH_STATE "/actual-gain/interval"
#define AMP_STATE_GAIN_MIN_TIME           AMP_XPATH_STATE "/actual-gain/min-time"
#define AMP_STATE_GAIN_MAX_TIME           AMP_XPATH_STATE "/actual-gain/max-time"

#define AMP_STATE_GAIN_TILT               AMP_XPATH_STATE "/actual-gain-tilt/instant"
#define AMP_STATE_IN_POWER_T              AMP_XPATH_STATE "/input-power-total/instant"
#define AMP_STATE_IN_POWER_T_AVG          AMP_XPATH_STATE "/input-power-total/avg"
#define AMP_STATE_IN_POWER_T_MIN          AMP_XPATH_STATE "/input-power-total/min"
#define AMP_STATE_IN_POWER_T_MAX          AMP_XPATH_STATE "/input-power-total/max"
#define AMP_STATE_IN_POWER_T_INTERVAL     AMP_XPATH_STATE "/input-power-total/interval"
#define AMP_STATE_IN_POWER_T_MIN_TIME     AMP_XPATH_STATE "/input-power-total/min-time"
#define AMP_STATE_IN_POWER_T_MAX_TIME     AMP_XPATH_STATE "/input-power-total/max-time"

#define AMP_STATE_IN_POWER_C              AMP_XPATH_STATE "/input-power-c-band/instant"
#define AMP_STATE_IN_POWER_L              AMP_XPATH_STATE "/input-power-l-band/instant"
#define AMP_STATE_MSA_INPUT_POWER_C       AMP_XPATH_STATE "/msa-input-power"
#define AMP_STATE_OUT_POWER_T             AMP_XPATH_STATE "/output-power-total/instant"
#define AMP_STATE_OUT_POWER_T_AVG         AMP_XPATH_STATE "/output-power-total/avg"
#define AMP_STATE_OUT_POWER_T_MIN         AMP_XPATH_STATE "/output-power-total/min"
#define AMP_STATE_OUT_POWER_T_MAX         AMP_XPATH_STATE "/output-power-total/max"
#define AMP_STATE_OUT_POWER_T_INTERVAL    AMP_XPATH_STATE "/output-power-total/interval"
#define AMP_STATE_OUT_POWER_T_MIN_TIME    AMP_XPATH_STATE "/output-power-total/min-time"
#define AMP_STATE_OUT_POWER_T_MAX_TIME    AMP_XPATH_STATE "/output-power-total/max-time"

#define AMP_STATE_OUT_POWER_C             AMP_XPATH_STATE "/output-power-c-band/instant"
#define AMP_STATE_OUT_POWER_C_AVG         AMP_XPATH_STATE "/output-power-c-band/avg"
#define AMP_STATE_OUT_POWER_C_MIN         AMP_XPATH_STATE "/output-power-c-band/min"
#define AMP_STATE_OUT_POWER_C_MAX         AMP_XPATH_STATE "/output-power-c-band/max"
#define AMP_STATE_OUT_POWER_C_INTERVAL    AMP_XPATH_STATE "/output-power-c-band/interval"
#define AMP_STATE_OUT_POWER_C_MIN_TIME    AMP_XPATH_STATE "/output-power-c-band/min-time"
#define AMP_STATE_OUT_POWER_C_MAX_TIME    AMP_XPATH_STATE "/output-power-c-band/max-time"

#define AMP_STATE_OUT_POWER_L             AMP_XPATH_STATE "/output-power-l-band/instant"
#define AMP_STATE_MSA_OUT_POWER_C         AMP_XPATH_STATE "/msa-output-power"
#define AMP_STATE_LASER_BIAS              AMP_XPATH_STATE "/laser-bias-current/instant"
#define AMP_STATE_OPTICAL_LOSS            AMP_XPATH_STATE "/optical-return-loss/instant"
/* #define AMP_STATE_OPT                     AMP_XPATH_STATE "/operational-state" */
#define AMP_STATE_PUMP_TEMPERATURE        AMP_XPATH_STATE "/pump-temperature"
#define AMP_STATE_CURRENT                 AMP_XPATH_STATE "/com-oplink-amplifier-ext:laser-bias-currents[id='%d']/current"
#define AMP_STATE_REFLECTED_POWER         AMP_XPATH_STATE "/com-oplink-amplifier-ext:reflected-power"
#define AMP_STATE_OPT                     AMP_XPATH_STATE "/com-oplink-amplifier-ext:operational-state"

#define AMP_CFG_FSM_MODE                  AMP_XPATH_CFG   "/com-oplink-amplifier-ext:fsm-mode"
#define AMP_CFG_FSM_MODE_NODE             "fsm-mode"
#define AMP_STATE_FSM_MODE                AMP_XPATH_STATE "/com-oplink-amplifier-ext:fsm-mode"
#define AMP_STATE_FSM_CURRENT             AMP_XPATH_STATE "/com-oplink-amplifier-ext:fsm-current"
#define RPC_XPATH_READY_TRIGGER           "/com-oplink-rpc:ready-trigger"
#define RPC_XPATH_READY_TRIGGER_MODULE    RPC_XPATH_READY_TRIGGER "/module"
#define RPC_XPATH_READY_TRIGGER_VALUE     RPC_XPATH_READY_TRIGGER "/value"
#define AMP_CFG_READY_POWER               AMP_XPATH_CFG   "/com-oplink-amplifier-ext:ready-power"
#define AMP_CFG_READY_POWER_NODE          "ready-power"

typedef enum {
    EN_TRIG_MOD_PA = 0,
    EN_TRIG_MOD_BA = 1,
    EN_TRIG_MOD_MAX
} EN_TRIG_PATH;

extern EDFA_MODE_EN DSC_EdfaModeToEnum(char *pcStr);
extern EDFA_GAIN_RANGE_EN DSC_EdfaGainRangeToEnum(char *pcStr);
extern char *DSC_EdfaModeToStr(EDFA_MODE_EN enMode, char *pcBuf, size_t nSize);
extern char *DSC_EdfaGainrangeToStr(char *pcStr, EDFA_GAIN_RANGE_EN enGainrange);
extern char *DSC_EdfaStateToStr(char *pcStr, EDFA_STATE_EN enState);
extern char *DSC_AmpTypeToStr(char *pcStr, AMP_TYPE_EN enMode);
extern int DSC_GetEdfaCfgKey(char *pcXpath, char *pcKeyValue);
extern int DSC_GetEdfaCfgKeyFromDb(EDFA_KEY_ST *pstEdfaKey);
extern int DSC_GetEdfaConfig(char *pcCfgKey, EDFA_CFG_ST *pstEdfaCfg, size_t size);
extern int DSC_GetEdfaConfigWithoutTca(char *pcCfgKey, EDFA_CFG_ST *pstEdfaCfg, size_t size);
extern int DSC_GetEdfaState(char *pcCfgKey, EDFA_STATE_ST *pstEdfaState, size_t size);
extern int DSC_SetEdfaGainRange(char *pcCfgKey, EDFA_GAIN_RANGE_EN enGainRange);
extern int DSC_SetEdfaBackRefRatio(char *pcCfgKey, double dBackRefRatio);
extern int DSC_SetEdfaInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaDcmInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaDcmOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaGain(char *pcCfgKey, double dGainSetPoint);
extern int DSC_SetEdfaTilt(char *pcCfgKey, double dTiltSetPoint);
extern int DSC_SetEdfaOutputEnable(char *pcCfgKey, bool state);
extern int DSC_SetEdfaAutolosEnable(char *pcCfgKey, bool state);
extern int DSC_SetEdfaMode(char *pcCfgKey, EDFA_MODE_EN enMode);
extern int DSC_SetEdfaDcmTargetLos(char *pcCfgKey, double dDcmTargetLos);
extern int DSC_SetEdfaAprEnable(char *pcCfgKey, bool state);
extern int DSC_SetEdfaAprPower(char *pcCfgKey, double dAprPower);
extern int DSC_SetEdfaGlimPower(char *pcCfgKey, double dGlimPower);
extern int DSC_SetEdfaPlimPower(char *pcCfgKey, double dPlimPower);
extern int DSC_SetEdfaPlimEnable(char *pcCfgKey, bool state);

extern int DSC_GetEVoaCfgKey(char *pcXpath, char *pcKeyValue);
extern int DSC_GetVoaCfgKeyFromDb(VOA_KEY_ST *pstVoaKey);
extern int DSC_SetEvoaAttn(char *pcCfgKey, double dAttn);
extern int DSC_SetEvoaTarget(char *pcCfgKey, double dTarget);
extern int DSC_GetEvoaAttn(char *pcCfgKey, double *pdAttn);
extern int DSC_GetEvoaActualAttn(char *pcCfgKey, double *pdAttn);
extern int DSC_GetEvoaTarget(char *pcCfgKey, double *pdTarget);
extern int DSC_GetEvoaActualTarget(char *pcCfgKey, double *pdTarget);

extern int DSC_SetEdfaOutputPower(char *pcCfgKey, double dPower);
extern int DSC_SetEdfaType(char *pcCfgKey, AMP_TYPE_EN enType);

extern int DSC_SetEdfaMsaInLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaMsaOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);

extern int DSC_GetEdfaGainByName(char *name, double *gain);
extern EDFA_MODE_EN DSC_GetEdfaModeByName(char *name);
extern EDFA_GAIN_RANGE_EN DSC_GetEdfaGainRangeByName(char *name);
extern int DSC_GetEdfaOutputPowerByName(char *name, double *power);

extern int DSC_SetEdfaGain_Commit(char *pcCfgKey, double dGainSetPoint, bool bCommit);
extern int DSC_SetEdfaTilt_Commit(char *pcCfgKey, double dTiltSetPoint, bool bCommit);
extern int DSC_SetEvoaAttn_Commit(char *pcCfgKey, double dAttn, bool bCommit);
extern int DSC_GetEdfaThrConfig(int iTcaIdx, double *pdHighThr, double *pdLowThr);

extern int DSC_SetPortOutLosThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetClientRxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetClientRxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetClientTxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetClientTxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetLineRxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetLineRxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetLineTxLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetLineTxHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaCurrentHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEdfaCurrents(char *pcCfgKey, uint8_t ucNumOfPumps, double *pdCurrents);
extern int DSC_GetEdfaNumOfPumps(char *pcCfgKey, uint8_t *pucNumOfPumps);
extern int DSC_SetEdfaFsmMode(char *pcCfgKey, AMP_FSM_MODE_EN enFsmMode);
extern int DSC_SetEdfaFsmMode_Commit(char *pcCfgKey, AMP_FSM_MODE_EN enFsmMode, bool bCommit);
extern int DSC_GetEdfaFsmMode(char *pcCfgKey, AMP_FSM_MODE_EN *penFsmMode);
extern int DSC_RpcReadyTrigger(char *pcModule, uint32_t value);
extern AMP_FSM_MODE_EN DSC_EdfaFsmModeToEnum(char *pcStr);
extern char *DSC_EdfaFsmModeToStr(char *pcStr, AMP_FSM_MODE_EN enMode);
extern AMP_FSM_STATE_EN DSC_EdfaFsmStateToEnum(char *pcStr);
extern char *DSC_EdfaFsmStateToStr(char *pcStr, AMP_FSM_STATE_EN enState);
extern int DSC_SetEdfaOutRflThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetEvoaEnable(char *pcCfgKey, bool bEnable);
extern int DSC_GetEvoaEnable(char *pcCfgKey, bool *pbEnable);
extern int DSC_SetEdfaReadyPower(char *pcCfgKey, double dAprPower);
extern int DSC_GetEdfaOutputEnableByName(char *pcCfgKey, bool *pbEnable);

#ifdef __cplusplus
}
#endif

#endif

