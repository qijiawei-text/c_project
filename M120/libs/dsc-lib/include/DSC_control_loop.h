#ifndef DSC_CONTROL_LOOP_H
#define DSC_CONTROL_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
#include "DSC_net.h"

/* Yang is different for echo prpject */
#define AUTO_CTRL_FOR_NOPPORO_AMP          (0)
#define AUTO_CTRL_FOR_NOPPORO_AMP_OPS      (0)
#define AUTO_CTRL_FOR_RAINIER              (0)
#define AUTO_CTRL_FOR_400GZR               (1)

typedef enum {
    DIR_EGRESS,
    DIR_INGRESS,
    DIR_TYPE_MAX
} DIR_TYPE_EN;

typedef enum LINE_en {
    WORKING_LINE = 0,
    PROTECTION_LINE,
    LINE_CNT
} LINE_EN;


typedef enum {
    FIBER_SSMF = 0,
    FIBER_ALLWAVE = 1,
    FIBER_TERALIGHT = 2,
    FIBER_TRUEWAVE_REACH = 3,
    FIBER_LEAF = 4,
    FIBER_METROCORE = 5,
    FIBER_TRUEWAVE_RS = 6,
    FIBER_TRUEWAVE_CPLUS = 7,
    FIBER_LS = 8,
    FIBER_DSF = 9,
    FIBER_HCF = 10,
    FIBER_TYPE_CNT
} FIBER_TYPE_EN;

#define FIBER_SSMF_STR           "ssmf"
#define FIBER_ALLWAVE_STR        "allwave"
#define FIBER_TERALIGHT_STR      "teralight"
#define FIBER_TRUEWAVE_REACH_STR "truewave-reach"
#define FIBER_LEAF_STR           "leaf"
#define FIBER_METROCORE_STR      "metrocore"
#define FIBER_TRUEWAVE_RS_STR    "truewave-rs"
#define FIBER_TRUEWAVE_CPLUS_STR "truewave-cplus"
#define FIBER_LS_STR             "ls"
#define FIBER_DSF_STR            "dsf"
#define FIBER_HCF_STR            "hcf"

#define W_PATH_OPS_APR_FLAG_FILE          "/tmp/w_path_ops_apr_flag"
#define P_PATH_OPS_APR_FLAG_FILE          "/tmp/p_path_ops_apr_flag"

#define MODULE_NS_CTRL_LOOP               "com-oplink-control-loop"

#define AUTO_CONTROL_MODULE               "/" MODULE_NS_CTRL_LOOP

#define AUTO_CONTROL_XPATH_HEADER         AUTO_CONTROL_MODULE ":" "auto-control/controls"

typedef struct st_FIBER_PARM {
    FIBER_TYPE_EN enFiberType;
    double dDispersion;
    double dFiberLoss;
    double dLauchingPower;
    double dSupportedSpanLoss;
    double dSignalLossTilt;
    double dOscLossTilt;
    double dSrsK;
} FIBER_PARM_ST;

typedef struct {
    uint32_t uiTotal;
    FIBER_PARM_ST astFiberPara[FIBER_TYPE_CNT];
} FIBER_PARM_LIST_ST;

#define AUTO_CONTROL_CONFIG_HEADER        AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/auto-config"
#define AUTO_CONTROL_CONFIG_ALL           AUTO_CONTROL_CONFIG_HEADER "//*"
#define NUM_OF_CARRIERS_NODE              "number-of-carriers"
#define NUM_OF_CARRIERS_XPATH             AUTO_CONTROL_CONFIG_HEADER "/" NUM_OF_CARRIERS_NODE
#define FIBER_TYPE_NODE                   "fiber-type"
#define FIBER_TYPE_XPATH                  AUTO_CONTROL_CONFIG_HEADER "/" FIBER_TYPE_NODE
#define LAUNCHING_POWER_NODE              "launching-power-per-channel"
#define LAUNCHING_POWER_XPATH             AUTO_CONTROL_CONFIG_HEADER "/" LAUNCHING_POWER_NODE
#define TARGET_POWER_NODE                 "target-power-per-channel"
#define TARGET_POWER_XPATH                AUTO_CONTROL_CONFIG_HEADER "/" TARGET_POWER_NODE
#define LINE_TX_TARGET_APR_POWER_NODE     "line-tx-target-apr-power"
#define LINE_TX_TARGET_APR_POWER_XPATH    AUTO_CONTROL_CONFIG_HEADER "/" LINE_TX_TARGET_APR_POWER_NODE
#define CARRIERS_BASED_OCM_NODE           "ocm-based-carriers"
#define CARRIERS_BASED_OCM_XPATH          AUTO_CONTROL_CONFIG_HEADER "/" CARRIERS_BASED_OCM_NODE
#define LONG_SPAN_NODE                    "long-span"
#define LONG_SPAN_XPATH                   AUTO_CONTROL_CONFIG_HEADER "/" LONG_SPAN_NODE
#define SATURATION_DROP_NODE              "saturation-drop"
#define SATURATION_DROP_XPATH             AUTO_CONTROL_CONFIG_HEADER "/" SATURATION_DROP_NODE
#define PA_INPUT_TARGET_POWER_NODE        "pre-amp-input-target-power-per-channel"
#define PA_INPUT_TARGET_POWER_XPATH       AUTO_CONTROL_CONFIG_HEADER "/" PA_INPUT_TARGET_POWER_NODE

#define REMOTE_IP_NODE                    "remote-ip"
#define LOCAL_OSC_IP_NODE                 "local-osc-ip"
#define AUTO_MANUAL_CONFIG_HEADER         AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/manual-config"
#define MANUAL_REMOTE_IP_XPATH            AUTO_MANUAL_CONFIG_HEADER "/" REMOTE_IP_NODE
#define MANUAL_LOCAL_OSC_IP_XPATH         AUTO_MANUAL_CONFIG_HEADER "/" LOCAL_OSC_IP_NODE

#define REMOTE_INFO_HEADER                AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/remote-info"
#define REMOTE_INFO_HEADER_W              AUTO_CONTROL_XPATH_HEADER "/control[name='W']/remote-info"
#define REMOTE_INFO_HEADER_P              AUTO_CONTROL_XPATH_HEADER "/control[name='P']/remote-info"
#define REMOTE_INFO_ALL                   REMOTE_INFO_HEADER "//*"
#define REMOTE_NODENAME_NODE              "remote-node-name"
#define REMOTE_OSC_IP_NODE                "remote-osc-ip"
#define REMOTE_INFO_NODENAME_XPATH        REMOTE_INFO_HEADER "/" REMOTE_NODENAME_NODE
#define REMOTE_INFO_NODEIP_XPATH          REMOTE_INFO_HEADER "/" REMOTE_IP_NODE
#define REMOTE_INFO_OSCIP_XPATH           REMOTE_INFO_HEADER "/" REMOTE_OSC_IP_NODE
#define LOCAL_INFO_OSCIP_XPATH            REMOTE_INFO_HEADER "/" LOCAL_OSC_IP_NODE

#define LAST_AUTO_CONTROL_HEADER          AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/last-auto-control"
#define LAST_CALIBRATION_DATA_HEADER      LAST_AUTO_CONTROL_HEADER  "/" "last-calibration-data"
#define LAST_SPANLOSS_NODE                "last-spanloss"
#define LAST_SPANLOSS_XPATH               LAST_CALIBRATION_DATA_HEADER "/" LAST_SPANLOSS_NODE

#define CONTROL_THRESHOLD_HEADRE          AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/control-threshold"
#define SPANLOSS_THRESHOLD_NODE           "span-loss-threshold"
#define SPANLOSS_THRESHOLD_XPATH          CONTROL_THRESHOLD_HEADRE "/" SPANLOSS_THRESHOLD_NODE

#define AUTO_CONTROL_GAIN_STATE_HEADER    AUTO_CONTROL_XPATH_HEADER "/control[name='%s']/auto-gain-state"
#define AUTO_CONTROL_GAIN_STATE_HEADER_W  AUTO_CONTROL_XPATH_HEADER "/control[name='W']/auto-gain-state"
#define AUTO_CONTROL_GAIN_STATE_HEADER_P  AUTO_CONTROL_XPATH_HEADER "/control[name='P']/auto-gain-state"
#define AUTO_CONTROL_GAIN_STATE_ALL       AUTO_CONTROL_GAIN_STATE_HEADER "//*"
#define SPAN_LOSS_NODE                    "span-loss"
#define SPAN_LOSS_XPATH                   AUTO_CONTROL_GAIN_STATE_HEADER "/" SPAN_LOSS_NODE
#define RECV_SPAN_LOSS_NODE               "receive-span-loss"
#define RECV_SPAN_LOSS_XPATH              AUTO_CONTROL_GAIN_STATE_HEADER "/" RECV_SPAN_LOSS_NODE
#define OSC_INPUT_POWER_NODE              "osc-input-power"
#define OSC_INPUT_POWER_XPATH             AUTO_CONTROL_GAIN_STATE_HEADER "/" OSC_INPUT_POWER_NODE
#define PRE_AMP_INPUT_POWER_NODE          "pre-amp-input-power"
#define PRE_AMP_INPUT_POWER_XPATH         AUTO_CONTROL_GAIN_STATE_HEADER "/" PRE_AMP_INPUT_POWER_NODE
#define PRE_AMP_GAIN_NODE                 "pre-amp-gain"
#define PRE_AMP_GAIN_XPATH                AUTO_CONTROL_GAIN_STATE_HEADER "/" PRE_AMP_GAIN_NODE
#define PRE_AMP_VOA_NODE                  "pre-amp-voa"
#define PRE_AMP_VOA_XPATH                 AUTO_CONTROL_GAIN_STATE_HEADER "/" PRE_AMP_VOA_NODE
#define BOOSTER_AMP_GAIN_NODE             "booster-amp-gain"
#define BOOSTER_AMP_GAIN_XPATH            AUTO_CONTROL_GAIN_STATE_HEADER "/" BOOSTER_AMP_GAIN_NODE
#define BOOSTER_AMP_VOA_NODE              "booster-amp-voa"
#define BOOSTER_AMP_VOA_XPATH             AUTO_CONTROL_GAIN_STATE_HEADER "/" BOOSTER_AMP_VOA_NODE
#define PROGRESS_NODE                     "progress"
#define GAIN_PROGRESS_XPATH               AUTO_CONTROL_GAIN_STATE_HEADER "/" PROGRESS_NODE
#define STATE_NODE                        "state"
#define GAIN_STATE_XPATH                  AUTO_CONTROL_GAIN_STATE_HEADER "/" STATE_NODE


#define CURRENT_WOKING_PATH_NODE          "current-working-path"
#define CURRENT_WOKING_PATH_XPATH         AUTO_CONTROL_GAIN_STATE_HEADER "/" CURRENT_WOKING_PATH_NODE

#define WORKING_LINE_YANG                 "W"
#define PROTECTION_LINE_YANG              "P"

#define DEF_DIRECTION_EGRESS              "ba"
#define DEF_DIRECTION_INGRESS             "pa"

#define RPC_XPATH_AUTO_GAIN_START         AUTO_CONTROL_MODULE  ":" "auto_gain_start"
#define RPC_XPATH_AUTO_GAIN_START_PATH    RPC_XPATH_AUTO_GAIN_START "/path"

#define RPC_XPATH_AUTO_CD_START           AUTO_CONTROL_MODULE  ":" "auto_cd_start"
#define RPC_XPATH_OTDR_SCAN_START         AUTO_CONTROL_MODULE  ":" "otdr_scan_start"
#define RPC_XPATH_AUTO_CLEAR_ALM          AUTO_CONTROL_MODULE  ":" "auto_clear_alarm"
#define RPC_XPATH_COLLECT_FIBER_CONFIG    AUTO_CONTROL_MODULE  ":" "collect-fiber-config"

#define RPC_XPATH_AUTO_POWER              AUTO_CONTROL_MODULE  ":" "auto_power"
#define RPC_XPATH_AUTO_POWER_PATH         RPC_XPATH_AUTO_POWER "/path"
#define RPC_XPATH_AUTO_POWER_DIR          RPC_XPATH_AUTO_POWER "/direction"

#define FIBER_CONFIG_NODE              "fiber-config"
#define DISPERSION_NODE                "dispersion"
#define FIBER_LOSS_NODE                "fiber_loss"
#define LAUNCHING_POWER_STR_NODE       "launching_power"
#define SUPPORTED_SPAN_lOSS_NODE       "supported_span_loss"
#define SIGNAL_LOSS_TILT_NODE          "signal_loss_tilt"
#define OSC_LOSS_TILT_NODE             "osc_loss_tilt"
#define SRS_K_NODE                     "srs_k"

#define XPATH_FIBER_CONFIG             RPC_XPATH_COLLECT_FIBER_CONFIG "/fiber-config[fiber-type='%s']/"
#define XPATH_FIBER_TYPE_NAME          RPC_XPATH_COLLECT_FIBER_CONFIG "/fiber-type-name"
#define XPATH_FIBER_CONFIG_PATH        RPC_XPATH_COLLECT_FIBER_CONFIG "/path"

#define MODULE_NS_CTRL_LOOP_I             "com-oplink-control-loop-i"

#define AUTO_CONTROL_I_MODULE             "/" MODULE_NS_CTRL_LOOP_I
#define AUTO_CONTROL_I_XPATH_HEADER       AUTO_CONTROL_I_MODULE ":" "auto-control/controls"

#define ACI_LAST_AUTO_CONTROL_HEADER            AUTO_CONTROL_I_XPATH_HEADER "/control[name='%s']/last-auto-control"
#define ACI_LAST_CALIBRATION_DATA_HEADER        ACI_LAST_AUTO_CONTROL_HEADER  "/" "last-calibration-data"
#define LAST_OSC_TX_POWER_NODE                  "last-osc-tx-power"
#define ACI_LAST_OSC_TX_POWER_XPATH             ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_OSC_TX_POWER_NODE
#define LAST_OSC_RX_POWER_NODE                  "last-osc-rx-power"
#define ACI_LAST_OSC_RX_POWER_XPATH             ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_OSC_RX_POWER_NODE
#define LAST_NUM_OF_CARRIERS_NODE               "last-number-of-carriers"
#define ACI_LAST_NUM_OF_CARRIERS_XPATH          ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_NUM_OF_CARRIERS_NODE
#define LAST_BOOSTER_AMP_INPUT_POWER_NODE       "last-booster-amp-input-power"
#define ACI_LAST_BOOSTER_AMP_INPUT_POWER_XPATH  ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_BOOSTER_AMP_INPUT_POWER_NODE
#define LAST_BOOSTER_AMP_GAIN_NODE              "last-booster-amp-gain"
#define ACI_LAST_BOOSTER_AMP_GAIN_XPATH         ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_BOOSTER_AMP_GAIN_NODE
#define LAST_BOOSTER_AMP_VOA_NODE               "last-booster-amp-voa"
#define ACI_LAST_BOOSTER_AMP_VOA_XPATH          ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_BOOSTER_AMP_VOA_NODE
#define LAST_PRE_AMP_INPUT_POWER_NODE           "last-pre-amp-input-power"
#define ACI_LAST_PRE_AMP_INPUT_POWER_XPATH      ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_PRE_AMP_INPUT_POWER_NODE
#define LAST_PRE_AMP_GAIN_NODE                  "last-pre-amp-gain"
#define ACI_LAST_PRE_AMP_GAIN_XPATH             ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_PRE_AMP_GAIN_NODE
#define LAST_PRE_AMP_VOA_NODE                   "last-pre-amp-voa"
#define ACI_LAST_PRE_AMP_VOA_XPATH              ACI_LAST_CALIBRATION_DATA_HEADER "/" LAST_PRE_AMP_VOA_NODE

/* TBD, refer to yang */
#if 0
#define STATE_SUCCESS              (0)
#define STATE_PROCESSING           (1)
#define STATE_FAILED_CD            (2)
#define STATE_FAILED_SPANLOSS      (2)
#define STATE_FAILED_BA_INPUT      (3)
#define STATE_FAILED_GAIN_OTHER    (4)
#endif

/* refer to yang define */
#define AUTO_STATE_SUCCESS_STR               "success"
#define AUTO_STATE_PROCESSING_STR            "processing"
#define AUTO_STATE_FAILED_SPANLOSS_STR       "failed-in-spanloss"
#define AUTO_STATE_FAILED_BA_INPUT_STR       "failed-in-ba-input-power"
#define AUTO_STATE_FAILED_GAIN_OTHER_STR     "failed-in-gain"
#define AUTO_STATE_FAILED_CD_STR             "failed-in-cd"
#define AUTO_STATE_FAILED_OSC_STR            "failed-in-osc"
#define AUTO_STATE_FAILED_BA_PUMP_STR        "failed-in-ba-pump"
#define AUTO_STATE_FAILED_BA_RFL_STR         "failed-in-ba-reflection"
#define AUTO_STATE_FAILED_BA_APR_STR         "failed-in-ba-apr"
#define AUTO_STATE_FAILED_BA_LASER_SAFETY_STR "failed-in-ba-laser-safety"
#define AUTO_STATE_FAILED_UPD_ACTIVATE_STR   "failed-in-upgrade-activate"
#define AUTO_STATE_FAILED_SCC_SW_MISMATCH_STR "failed-in-scc-software-mismatch"
#define AUTO_STATE_FAILED_AMP_SW_MISMATCH_STR "failed-in-amp-software-mismatch"
#define AUTO_STATE_FAILED_APPLY_TO_MODULE_STR "failed-in-apply-to-module"

typedef enum {
    AUTO_STATE_SUCCESS = 0,
    AUTO_STATE_PROCESSING = 1,
    AUTO_STATE_FAILED_SPANLOSS = 2,
    AUTO_STATE_FAILED_BA_INPUT = 3,
    AUTO_STATE_FAILED_GAIN_OTHER = 4,
    AUTO_STATE_FAILED_CD = 5,
    AUTO_STATE_FAILED_OSC = 6,
    AUTO_STATE_FAILED_BA_PUMP = 7,
    AUTO_STATE_FAILED_BA_RFL = 8,
    AUTO_STATE_FAILED_BA_APR = 9,
    AUTO_STATE_FAILED_BA_LASER_SAFETY = 10,
    AUTO_STATE_FAILED_UPD_ACTIVATE = 11,
    AUTO_STATE_FAILED_SCC_SW_MISMATCH = 12,
    AUTO_STATE_FAILED_AMP_SW_MISMATCH = 13,
    AUTO_STATE_FAILED_APPLY_TO_MODULE = 14,

    AUTO_STATE_CNT
} AUTO_CONTROL_STATE_EN;

typedef enum  {
    EN_OPTICAL_PATH_W = 0,     /* path W */
    EN_OPTICAL_PATH_P,         /* path P */
    EN_OPTICAL_PATH_MAX
} OPTICAL_PATH_EN;

typedef struct {
    double dSpanLoss;
    double dReceiveSpanloss;
    double dOscInPower;
    double dPreAmpInputPower;
    double dPreAmpGain;
    double dPreVoaAtten;
    double dBoosterAmpGain;
    double dBoosterVoaAtten;
    uint8_t ucProgress;                             /* range: 0 ~ 100, Unit: %d */
    char acResv[3];
    AUTO_CONTROL_STATE_EN enState;                  /* refer to AUTO_CONTROL_STATE_EN */
    double adSpanLoss[EN_OPTICAL_PATH_MAX];         /* span loss of both path w and path p */
    double adOpsAprVoa[EN_OPTICAL_PATH_MAX];        /* ops voa value in ops-apr, -1 stands for out-of-ops-apr */
    uint8_t aucOpsAprState[EN_OPTICAL_PATH_MAX];
    uint8_t ucCurWorkingPath;                           /* current optical working path, refer to OPTICAL_PATH_EN */
    char cResv2;
} AUTO_GAIN_STATE_ST;

typedef struct {
    double dCdValue;
    uint8_t ucProgress;        /* range: 0 ~ 100, Unit: %d */
    char acResv[3];
    AUTO_CONTROL_STATE_EN enState;           /* refer to AUTO_CONTROL_STATE_EN */
} AUTO_CD_STATE_ST;

typedef struct  {
    uint8_t ucNumOfCarriers;
    char acResv[3];                 /* reserve, 4-byte alignment */
    FIBER_TYPE_EN enFiberType;
    double dLauchingPower;
    double dTargetPower;
    bool bBasedOcmCarriers;
    bool bLongSpan;
    double dSaturationDrop;
    double dPaInputTargetPower;
} AUTO_CFG_DATA_ST;

typedef struct  {
    double dSpanlossThreshold;
} AUTO_THRESHOLD_ST;

typedef struct {
    char acNodeName[128];
    char acRemoteIp[64];
    char acRemoteChassisId[64];    /* chassis id == ??? MAC */
    char acRemoteOscIp[64];        /* for osc communication ip */
    char acLocalOscIp[64];         /* for local osc ip */
} REMOTE_NODE_INFO_ST;


int DSC_AutoSetLongSpan(char *pcLine, bool bFlag);

int DSC_AutoSetNumOfCarriers(char *pcLine, uint8_t ucNumOfCarriers);
int DSC_AutoGetNumOfCarriers(char *pcLine, uint8_t *pucNumOfCarriers);

int DSC_AutoSetManualOscIp(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo);
int DSC_AutoGetMaunalOscIp(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo);

int DSC_AutoSetLineTx(char *pcLine, double dLineTx);
int DSC_AutoGetLineTx(char *pcLine, double *pdLineTx);

int DSC_AutoSetCfgWithoutNumOfCarriers(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg);  /* not contain number of carriers */
int DSC_AutoSetCfgFibertype(char *pcLine, FIBER_TYPE_EN enFiberType);
int DSC_AutoSetCfgLanchingPower(char *pcLine, double dLauchingPower);
int DSC_AutoSetCfgDropPower(char *pcLine, double dTargetPower);
int DSC_AutoSetCfgSaturationDrop(char *pcLine, double dSaturationDrop);
int DSC_AutoSetCfgPaInputTargetPower(char *pcLine, double dTargetPower);
int DSC_AutoSetCfg(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg);   /* set all */
int DSC_AutoGetCfg(char *pcLine, AUTO_CFG_DATA_ST *pstAutoCfg);   /* get all, contain number of carriers */

int DSC_AutoStartGainCtrl(char *pcLine);
int DSC_AutoGetGainState(char *pcLine, AUTO_GAIN_STATE_ST *pstAutoGainState);

int DSC_AutoClearALarm();

char *DSC_FiberTypeToString(char *pcFiberTypeStr, FIBER_TYPE_EN enFiberType);
FIBER_TYPE_EN DSC_FiberTypeToEnum(char *pcFiberTypeStr);

char *DSC_ControlStateToString(char *pcControlStateStr, AUTO_CONTROL_STATE_EN enControlState);
AUTO_CONTROL_STATE_EN DSC_ControlStateToEnum(char *pcControlStateStr);

int DSC_AutoSetSpanloss(char *pcLine, double dSpanloss);
int DSC_AutoSetSpanloss_Commit(char *pcLine, double dSpanloss, bool bCommit);
int DSC_AutoGetSpanloss(char *pcLine, double *pdSpanloss);

int DSC_AutoSetSpanlossThreshold(char *pcLine, double dSpanlossThreshold);
int DSC_AutoGetSpanlossThreshold(char *pcLine, double *pdSpanlossThreshold);

int DSC_AutoSetCaliOscTxPower(char *pcLine, double dPower);
int DSC_AutoSetCaliOscTxPower_Commit(char *pcLine, double dPower, bool bCommit);
int DSC_AutoGetCaliOscTxPower(char *pcLine, double *pdPower);

int DSC_AutoSetCaliOscRxPower(char *pcLine, double dPower);
int DSC_AutoSetCaliOscRxPower_Commit(char *pcLine, double dPower, bool bCommit);
int DSC_AutoGetCaliOscRxPower(char *pcLine, double *pdPower);

int DSC_AutoSetCaliSpanloss(char *pcLine, double dSpanloss);
int DSC_AutoSetCaliSpanloss_Commit(char *pcLine, double dSpanloss, bool bCommit);
int DSC_AutoGetCaliSpanloss(char *pcLine, double *pdSpanloss);

int DSC_AutoSetCaliNumOfCarriers(char *pcLine, uint8_t ucNumOfCarriers);
int DSC_AutoSetCaliNumOfCarriers_Commit(char *pcLine, uint8_t ucNumOfCarriers, bool bCommit);
int DSC_AutoGetCaliNumOfCarriers(char *pcLine, uint8_t *pucNumOfCarriers);

int DSC_AutoSetCaliBaInputPower(char *pcLine, double dPower);
int DSC_AutoSetCaliBaInputPower_Commit(char *pcLine, double dPower, bool bCommit);
int DSC_AutoGetCaliBaInputPower(char *pcLine, double *pdPower);

int DSC_AutoSetCaliBaGain(char *pcLine, double dGain);
int DSC_AutoSetCaliBaGain_Commit(char *pcLine, double dGain, bool bCommit);
int DSC_AutoGetCaliBaGain(char *pcLine, double *pdGain);

int DSC_AutoSetCaliBaPaddingVoa(char *pcLine, double dVoa);
int DSC_AutoSetCaliBaPaddingVoa_Commit(char *pcLine, double dVoa, bool bCommit);
int DSC_AutoGetCaliBaPaddingVoa(char *pcLine, double *pdVoa);

int DSC_AutoSetCaliPaInputPower(char *pcLine, double dPower);
int DSC_AutoSetCaliPaInputPower_Commit(char *pcLine, double dPower, bool bCommit);
int DSC_AutoGetCaliPaInputPower(char *pcLine, double *pdPower);

int DSC_AutoSetCaliPaGain(char *pcLine, double dGain);
int DSC_AutoSetCaliPaGain_Commit(char *pcLine, double dGain, bool bCommit);
int DSC_AutoGetCaliPaGain(char *pcLine, double *pdGain);

int DSC_AutoSetCaliPaVoa(char *pcLine, double dVoa);
int DSC_AutoSetCaliPaVoa_Commit(char *pcLine, double dVoa, bool bCommit);
int DSC_AutoGetCaliPaVoa(char *pcLine, double *pdVoa);

int DSC_GetRemoteInfo(char *pcLine, REMOTE_NODE_INFO_ST *pstRemoteInfo);

int DSC_CollectFiberConfig(char *pcLine, char *pcFiberTypeName, FIBER_PARM_LIST_ST *pstFiberList);

int DSC_AutoStartPowerCtrl(char *pcLine, char *pcDir);


#ifdef __cplusplus
}
#endif

#endif /* DSC_CONTROL_LOOP_H */

