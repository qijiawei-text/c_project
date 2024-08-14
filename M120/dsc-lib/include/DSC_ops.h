#ifndef _DSC_OPS_H
#define _DSC_OPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define OPS_DEV_NUM_MAX                 9
#define OPS_CFG_KEY                    "OLP"


typedef enum
{
    OPS_INDEX_1 = 1,
    OPS_INDEX_2,
    OPS_INDEX_3,
    OPS_INDEX_4,
    OPS_INDEX_5,
    OPS_INDEX_6,
    OPS_INDEX_7,
    OPS_INDEX_8,
    OPS_INDEX_9,

    OPS_INDEX_MAX = OPS_INDEX_9
} EN_OPS_IDX;

typedef enum OPS_WORK_MODE_en
{
    OPS_CLEAR = 0,
    OPS_FORCE_P = 1,    /* force to primary */
    OPS_FORCE_S = 2,    /* force to secondary */
    OPS_CLEAR_BLOCKED = 3,
    OPS_MFG_TEST_MODE = 4,
    OPS_MAN_P = 5,
    OPS_MAN_S = 6,
    OPS_CLEAR_WTR = 7,
    OPS_LOCK_OUT = 8,
    OPS_WORK_MODE_MAX
} OPS_WORK_MODE_en;

#if 0

typedef struct ops_state_all_st {

    uint32_t uiWtr; /* units milliseconds */
    char acActivePath[OPLK_PATH_MAX_LEN];
    char acSwitchState[OPLK_PATH_MAX_LEN];

} OPS_STATE_ALL_ST;

typedef struct ops_cfg_all_t {

    double dPwrDiffThr;
    double dPwrDiffHys;

} OPS_CFG_ALL_T;

#define OPS_ON_PRIMARY_STR         "on-primary"
#define OPS_ON_SECONDARY_STR       "on-secondary"

#define OPS_AUTO_STR               "auto"
#define OPS_LOCK_OUT_STR           "lock-out"
#define OPS_CLEAR_STR              "clear"
#define OPS_BLOCKED_STR            "blocked"
#define OPS_UNBLOCKED_STR          "unblocked"
#define OPS_FORCE_P_STR            "force-primary"
#define OPS_FORCE_S_STR            "force-secondary"
#define OPS_MANUAL_P_STR           "manual-primary"
#define OPS_MANUAL_S_STR           "manual-secondary"


/* #define OPS_NAME_PREFIX             "OPS" / * "ops-" * / */
#define OPS_HYS_DEFAULT             (1.5)
typedef enum en_OPS_STATE
{
    OPS_ON_P = 0,
    OPS_ON_S,
} OPS_STATE_EN;
#endif

typedef enum OPS_PORT_en
{
    OPS_PORT_LINE_PRIMARY_IN = 0,
    OPS_PORT_LINE_PRIMARY_OUT,
    OPS_PORT_LINE_SECONDARY_IN,
    OPS_PORT_LINE_SECONDARY_OUT,
    OPS_PORT_COMMON_IN,
    OPS_PORT_COMMON_OUTPUT,
    OPS_PORT_MAX
} OPS_PORT_EN;

typedef enum OPS_PATH_en {
    PRIMARY_PATH = 0,
    SECONDARY_PATH,
    OPS_PATH_MAX
} OPS_PATH_EN;

typedef enum OPS_SWITCH_PORT_en {
    SWITCH_TO_NONE = 0,
    SWITCH_TO_PRIMARY,
    SWITCH_TO_SECONDARY,
    SWITCH_TO_PORT_MAX
} OPS_SWITCH_PORT_EN;

typedef enum OPS_SWITCHING_TYPE_en {
    SWITCHING_TYPE_UNI = 0,
    SWITCHING_TYPE_BI,
    SWITCHING_TYPE_MAX
} OPS_SWITCHING_TYPE_EN;

typedef struct {
    char acName[16];
    bool bRevertive;
    uint32_t uiWtr;
    uint32_t uiHoldOffTime;
    double dPrimarySwitchThr;
    double dSecondarySwitchThr;
    double dPrimaryHys;
    double dSecondaryHys;
    double dRelativeThr;
    double dRelativeThrOffset;
    OPS_SWITCH_PORT_EN enForceToPort;
    OPS_SWITCHING_TYPE_EN enSwitchingType;
} OPS_CONFIG_ST;

typedef struct {
    char acName[16];
    bool bRevertive;
    uint32_t uiWtr;
    uint32_t uiHoldOffTime;
    double dPrimarySwitchThr;
    double dSecondarySwitchThr;
    double dPrimaryHys;
    double dSecondaryHys;
    double dRelativeThr;
    double dRelativeThrOffset;
    OPS_SWITCH_PORT_EN enForceToPort;
    OPS_PATH_EN enActivePath;
    char acSwitchState[16];
    char acSwitchingType[32];
} OPS_STATE_ST;

typedef struct {
    bool bEnable;
    double dTargetAttn;      /* no used for Meiwu project */
} OPS_PORT_CFG_ST;

typedef struct {
    double dInstant;
    double dAvg;
    double dMin;
    double dMax;
    uint64_t ulInterval;     /* units nanoseconds */
    uint64_t ulMinTime;
    uint64_t ulMaxTime;
} OPS_PORT_POWER_ST;

typedef struct {
    bool bEnable;
    double dTargetAttn;      /* no used for Meiwu project */
    OPS_PORT_POWER_ST stPortPwr;
} OPS_PORT_STATE_ST;

#define MODULE_NS_OPS           "openconfig-transport-line-protection"
#define OPS_MODULE_XPATH           "/" MODULE_NS_OPS
/* #define RPC_XPATH_OPS_MODE         OPS_MODULE_XPATH ":manual-to-port" */
#define RPC_XPATH_OPS_MANUAL_TO_PORT         OPS_MODULE_XPATH ":manual-to-port"
#define RPC_XPATH_OPS_TARGET       RPC_XPATH_OPS_MANUAL_TO_PORT "/manual-to-port"
#define RPC_XPATH_OPS_MESSAGE      RPC_XPATH_OPS_MANUAL_TO_PORT "/message"

#define OPS_XPATH_HEADER           OPS_MODULE_XPATH ":aps"
#define OPS_XPATH_ALL              OPS_XPATH_HEADER "//*"
#define OPS_XPATH_MODULES          OPS_XPATH_HEADER "/aps-modules/aps-module"

#define OPS_XPATH_CONFIG                     OPS_XPATH_HEADER "/aps-modules/aps-module[name='%s']/config"
#define OPS_XPATH_ALL_CONFIG                 OPS_XPATH_HEADER "/aps-modules/aps-module/config//*"
#define OPS_NODE_NAME                        "/name"
#define OPS_XPATH_MODULE_NAME                OPS_XPATH_MODULES OPS_NODE_NAME
#define OPS_XPATH_MODULES_CFG_ALL            OPS_XPATH_CONFIG "//*"

#define OPS_NODE_WTR                         "/wait-to-restore-time"
#define OPS_XPATH_WTR                        OPS_XPATH_CONFIG OPS_NODE_WTR

#define OPS_NODE_REVERTIVE                   "/revertive"
#define OPS_XPATH_REVERTIVE                  OPS_XPATH_CONFIG OPS_NODE_REVERTIVE

#define OPS_NODE_PRIMARY_SWITCH_THR          "/primary-switch-threshold"
#define OPS_XPATH_PRIMARY_SWITCH_THR         OPS_XPATH_CONFIG OPS_NODE_PRIMARY_SWITCH_THR

#define OPS_NODE_PRIMARY_SWITCH_HYS          "/primary-switch-hysteresis"
#define OPS_XPATH_PRIMARY_SWITCH_HYS         OPS_XPATH_CONFIG OPS_NODE_PRIMARY_SWITCH_HYS

#define OPS_NODE_SECONDARY_SWITCH_THR        "/secondary-switch-threshold"
#define OPS_XPATH_SECONDARY_SWITCH_THR       OPS_XPATH_CONFIG OPS_NODE_SECONDARY_SWITCH_THR

#define OPS_NODE_SECONDARY_SWITCH_HYS          "/secondary-switch-hysteresis"
#define OPS_XPATH_SECONDARY_SWITCH_HYS         OPS_XPATH_CONFIG OPS_NODE_SECONDARY_SWITCH_HYS

#define OPS_NODE_HOLD_OFF_TIME               "/hold-off-time"
#define OPS_XPATH_HOLD_OFF_TIME              OPS_XPATH_CONFIG OPS_NODE_HOLD_OFF_TIME

#define OPS_NODE_RELATIVE_SWITCH_THR         "/relative-switch-threshold"
#define OPS_XPATH_RELATIVE_SWITCH_THR        OPS_XPATH_CONFIG OPS_NODE_RELATIVE_SWITCH_THR

#define OPS_NODE_RELATIVE_SWITCH_THR_OFFSET   "/relative-switch-threshold-offset"
#define OPS_XPATH_RELATIVE_SWITCH_THR_OFFSET   OPS_XPATH_CONFIG OPS_NODE_RELATIVE_SWITCH_THR_OFFSET

#define OPS_NODE_FORCE_TO_PORT               "/force-to-port"
#define OPS_XPATH_FORCE_TO_PORT              OPS_XPATH_CONFIG OPS_NODE_FORCE_TO_PORT

#define OPS_NODE_SWITCHING_TYPE              "/com-oplink-ops-ext:switching-type"
#define OPS_XPATH_SWITCHING_TYPE             OPS_XPATH_CONFIG OPS_NODE_SWITCHING_TYPE

#define OPS_XPATH_STATE                      OPS_XPATH_HEADER "/aps-modules/aps-module[name='%s']/state"
#define OPS_XPATH_MODULES_STATE_ALL          OPS_XPATH_STATE "//*"
#define OPS_ACTIVE_PATH                      OPS_XPATH_STATE "/active-path"
#define OPS_STATE_WTR                        OPS_XPATH_STATE "/wait-to-restore-time"
#define OPS_SWITCH_STATE                     OPS_XPATH_STATE "/com-oplink-ops-ext:switch-state"
#define OPS_SWITCHING_TYPE                   OPS_XPATH_STATE OPS_NODE_SWITCHING_TYPE

#define OPS_XPATH_PORTS       OPS_XPATH_HEADER "/aps-modules/aps-module[name='%s']/ports"

#if 0
#define OPS_PRIMARY_IN_PWR    OPS_XPATH_PORTS "/line-primary-in/state/optical-power/instant"
#define OPS_SECONDARY_IN_PWR  OPS_XPATH_PORTS "/line-secondary-in/state/optical-power/instant"
#define OPS_CMN_OUT_PWR       OPS_XPATH_PORTS "/common-output/state/optical-power/instant"
#endif

#define OPS_XPATH_PORTS_CONFIG               OPS_XPATH_PORTS "/%s/config"
#define OPS_XPATH_PORTS_CONFIG_ALL           OPS_XPATH_PORTS_CONFIG "//*"
#define OPS_XPATH_PORTS_STATE                OPS_XPATH_PORTS "/%s/state"
#define OPS_XPATH_PORTS_STATE_ALL            OPS_XPATH_PORTS_STATE "//*"

#define OPS_PORT_CONFIG_ENABELED             OPS_XPATH_PORTS_CONFIG "/enabled"
#define OPS_PORT_STATE_ENABELED              OPS_XPATH_PORTS_STATE "/enabled"
#define OPS_PORT_POWER_INSTANT               OPS_XPATH_PORTS_STATE "/optical-power/instant"

#define STR_NONE            "NONE"
#define STR_PRIMARY         "PRIMARY"
#define STR_SECONDARY       "SECONDARY"

#define STR_LINE_PRIMARY_IN       "line-primary-in"
#define STR_LINE_PRIMARY_OUT      "line-primary-out"
#define STR_LINE_SECONDARY_IN     "line-secondary-in"
#define STR_LINE_SECONDARY_OUT    "line-secondary-out"
#define STR_COMMON_IN             "common-in"
#define STR_COMMON_OUTPUT         "common-output"

#if 0
#define STR_LOCKOUT "lockoutprotect"
#define STR_LOCK_OUT_PROTECT "Lock-Out-Protect"
#define STR_FORCE_SWITCH    "Force-Switch"
#define STR_MANUAL_SWITCH   "Manual-Switch"
#define STR_CLEAR           "Clear"
#endif

#define STR_LOP "LoP"
#define STR_FS  "FS"
#define STR_SF  "SF"
#define STR_MS  "MS"
#define STR_WTR "WTR"
#define STR_NR  "NR"
#define STR_RS  "RS"

#define STR_FS_P  "FS-P"
#define STR_SF_P  "SF-P"
#define STR_MS_P  "MS-P"
#define STR_NR_P  "NR-P"
#define STR_RS_P  "RS-P"
#define STR_DNR   "DNR"
#define STR_RR    "RR"
#define STR_EXER  "EXER"
#define STR_SD    "SD"

#define STR_UNIDIRECTIONAL  "unidirectional"
#define STR_BIDIRECTIONAL   "bidirectional"

#if 0
typedef struct save_ops_mode_t {
    char acActivePath[32];
    char acSwCmd[32];
    char acSwState[32];

} SAVE_OPS_MODE_T;
#endif
#if 0
extern int DSC_GetOPSSwitchCnt(char *pcCfgKey, OPS_PORT_EN enPort, int *piSwitchCnt);
extern int DSC_GetOPSWtr(char *pcCfgKey, uint32_t *puiOpsWtr);
extern int DSC_SetOPSWtr(char *pcCfgKey, int iOpsWtr);
extern int DSC_GetOPSStateStr(char *pcCfgKey, char *pcState, int length);
extern int DSC_GetOPSState(char *pcCfgKey, OPS_STATE_EN *state);
extern int DSC_GetOPSPortThreshold(char *pcCfgKey, OPS_PORT_EN enPort, double *threshold, double *hysteresis);
extern int DSC_SetOPSPortThreshold(char *pcCfgKey, OPS_PORT_EN enPort, double threshold);
extern int DSC_SetOPSPortHys(char *pcCfgKey, OPS_PORT_EN enPort, double dHys);
extern int DSC_GetOpsPower(char *pcCfgKey, OPS_PORT_EN enPort, double *pdPower);

extern int DSC_GetOPSRevertive(char *pcCfgKey, bool *revertive);
extern int DSC_SetOPSRevertive(char *pcCfgKey, bool bRevertive);
extern int DSC_GetOPSCount(size_t *count);
extern int DSC_GetOPSConfig(char *pcCfgKey, OPS_CONFIG *config);

extern int DSC_OpsPathEnumToString(OPS_STATE_EN path, char *pcPathStr);
extern int DSC_SetOPSHoldOffTime(char *pcCfgKey, int iHoldOff);
extern int DSC_RpcSetOpsMode(char *pcCfgKey, char *pcName, char *pcSwCmd, char *pcTargetPath);
extern int DSC_GetOPSHoldOffTime(char *pcCfgKey, uint32_t *puiHoldOff);
extern int DSC_GetOpsStateAll(char *pcCfgKey, OPS_STATE_ALL_ST *pstStateAll, size_t size);

extern int DSC_GetLineInPwr(char *pcCfgKey, double *pdPriInPwr, double *pdSecInPwr);
extern int DSC_SaveOPSMode(char *pcCfgKey, SAVE_OPS_MODE_T stMode);
extern int DSC_GetOpsMode(char *pcCfgKey, SAVE_OPS_MODE_T *pstMode);

extern int DSC_SetOpsRelativeSwithThr(char *pcCfgKey, double dThreshold);
extern int DSC_SetOpsRelativeSwithThrOffset(char *pcCfgKey, double dOffset);
extern int DSC_GetOpsCfgAll(char *pcCfgKey, OPS_CFG_ALL_T *pstCfgAll, size_t size);
extern int DSC_GetOpsCfgKey(char *pcXpath, char *pcKeyValue);
#endif

extern int DSC_GetOpsCfgKey(char *pcXpath, char *pcKeyValue);

extern int DSC_SetForceSwitch(char *pcCfgKey, char *pcTargetPath);
extern int DSC_RpcSetManualSwitch(char *pcCfgKey, char *pcTargetPath);

extern int DSC_SetOpsRevertive(char *pcCfgKey, bool bRevertive);
extern int DSC_SetOpsPortSwitchThr(char *pcCfgKey, OPS_PATH_EN enOpsPath, double dThreshold);
extern int DSC_SetOpsPortSwitchThrHys(char *pcCfgKey, OPS_PATH_EN enOpsPath, double dHys);
extern int DSC_SetOpsRelativeThr(char *pcCfgKey, double dThreshold);
extern int DSC_SetOpsRelativeThrOffset(char *pcCfgKey, double dOffset);
extern int DSC_SetOpsWtr(char *pcCfgKey, uint32_t uiOpsWtr);
extern int DSC_SetOpsHoldOffTime(char *pcCfgKey, uint32_t uiHoldOff);

extern int DSC_OpsSwitchPortEnumToStr(OPS_SWITCH_PORT_EN enForceToPort, char *pcPathStr, size_t nSize);
extern OPS_SWITCH_PORT_EN DSC_OpsSwitchPortStrToEnum(char *pcPathStr);

extern int DSC_OpsPathEnumToStr(OPS_PATH_EN enOpsPath, char *pcPathStr, size_t nSize);
extern OPS_PATH_EN DSC_OpsPathToEnum(char *pcPathStr);

extern int DSC_GetOpsConfig(char *pcCfgKey, OPS_CONFIG_ST *pstOpsCfg, size_t nSize);
extern int DSC_GetOpsState(char *pcCfgKey, OPS_STATE_ST *pstOpsState, size_t nSize);

extern int DSC_GetOpsPortConfig(char *pcCfgKey, OPS_PORT_EN enOpsPort, OPS_PORT_CFG_ST *pstOpsPortCfg, size_t nSize);
extern int DSC_GetOpsPortState(char *pcCfgKey, OPS_PORT_EN enOpsPort, OPS_PORT_STATE_ST *pstOpsPortState, size_t nSize);

extern int DSC_SetOpsPortEnable(char *pcCfgKey, OPS_PORT_EN enOpsPort, bool bEnable);

extern int DSC_OpsPortEnumToStr(OPS_PORT_EN enOpsPort, char *pcOpsPortName, size_t nSize);
extern OPS_PORT_EN DSC_OpsPortStrToEnum(char *pcOpsPortName);

extern int DSC_GetOpsNames(char *pcNames, size_t nNameSize, size_t *pnCount);
extern int DSC_SetOpsTcaValue(char *pcCfgKey, size_t nItemIndex, int iBaseType, bool bHys, double dblValue);
extern int DSC_GetOpsTcaValue(char *pcCfgKey, size_t nItemIndex, int iBaseType, double *pdblThr, double *pdblHys);

#ifdef __cplusplus
}
#endif

#endif

