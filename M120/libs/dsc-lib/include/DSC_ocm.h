#ifndef _DSC_OCM_H
#define _DSC_OCM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define OCM_CFG_KEY        "OCM"

/***************************************
* @OCM in openconfig-channel-monitor.yang
***************************************/
#define MODULE_NS_OCM_OPENCONFIG        "openconfig-channel-monitor"
#define OCM_OPENCONFIG_XPATH_HEADER     "/openconfig-channel-monitor:channel-monitors"
#define OCM_OPENCONFIG_XPATH_ALL        OCM_OPENCONFIG_XPATH_HEADER "//*"

#define OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS    OCM_OPENCONFIG_XPATH_HEADER
#define OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR_LIST    OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/channel-monitor"
#define OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR_LIST_NAME OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/channel-monitor/name"
#define OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR     OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/channel-monitor[name='%s']"
#define OCM_OPENCONFIG_XPATH_ALL_CONFIG          OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/channel-monitor/config//*"
#define OCM_OPENCONFIG_XPATH_CONFIG              OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR "/config"
#define OCM_OPENCONFIG_XPATH_CONFIG_ALL          OCM_OPENCONFIG_XPATH_CONFIG "//*"
#define OCM_OPENCONFIG_XPATH_CONFIG_NAME         OCM_OPENCONFIG_XPATH_CONFIG "/name"
#define OCM_OPENCONFIG_XPATH_CONFIG_MONITOR_PORT    OCM_OPENCONFIG_XPATH_CONFIG "/monitor-port"

#define OCM_OPENCONFIG_XPATH_STATE               OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR "/state"
#define OCM_OPENCONFIG_XPATH_STATE_ALL           OCM_OPENCONFIG_XPATH_STATE "//*"
#define OCM_OPENCONFIG_XPATH_STATE_NAME          OCM_OPENCONFIG_XPATH_STATE "/name"
#define OCM_OPENCONFIG_XPATH_STATE_PORT          OCM_OPENCONFIG_XPATH_STATE "/monitor-port"
#define OCM_OPENCONFIG_XPATH_NAME_ALL      OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR_LIST "/name"

#define OCM_OPENCONFIG_XPATH_CHANNELS            OCM_OPENCONFIG_XPATH_CHANNEL_MONITOR "/channels"
#define OCM_OPENCONFIG_XPATH_CHANNELS_ALL        OCM_OPENCONFIG_XPATH_CHANNELS "//*"

#define OCM_OPENCONFIG_XPATH_CHANNEL             OCM_OPENCONFIG_XPATH_CHANNELS "/channel"
#define OCM_OPENCONFIG_XPATH_CHANNEL_FREQUENCY   OCM_OPENCONFIG_XPATH_CHANNELS "/channel[lower-frequency='%lu'][upper-frequency='%lu']"
#define OCM_OPENCONFIG_XPATH_CHANNEL_STATE       OCM_OPENCONFIG_XPATH_CHANNEL_FREQUENCY "/state"
#define OCM_OPENCONFIG_XPATH_CHANNEL_STATE_LOW       OCM_OPENCONFIG_XPATH_CHANNEL_STATE "/lower-frequency"
#define OCM_OPENCONFIG_XPATH_CHANNEL_STATE_UPPER     OCM_OPENCONFIG_XPATH_CHANNEL_STATE "/upper-frequency"
#define OCM_OPENCONFIG_XPATH_CHANNEL_STATE_POWER     OCM_OPENCONFIG_XPATH_CHANNEL_STATE "/power"

#define COO_I_XPATH_OCM_MODULE_ALL               COO_I_XPATH_PREFIX "/optical-modules/channel-monitors/ocm-modules/ocm-module[name='%s']//*"
#define COO_I_XPATH_OCM_NAME_COMPONENT_ALL       COO_I_XPATH_PREFIX "/optical-modules/channel-monitors/ocm-modules/ocm-module[component-name='%s']/name"


/* 150G Coherent com-oplink-ocm-ext.yang*/
#define OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG   OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/com-oplink-ocm-ext:config"
#define WAVEPLAN_MODE_XPATH "/waveplan-mode"
#define OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG_WAVEPLAN_MODE OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG WAVEPLAN_MODE_XPATH
/* #define CHANNEL_POWER_MODE_XPATH "/channel-power-mode" */
#define CURRENT_WAVEPLAN "/current-waveplan"
/* #define OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG_CHANNEL_POWER_MODE OCM_OPENCONFIG_XPATH_EXT_MODULE_CONFIG CHANNEL_POWER_MODE_XPATH */
#define OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE   OCM_OPENCONFIG_XPATH_CHANNEL_MONITORS "/com-oplink-ocm-ext:state"
#define OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE_WAVEPLAN_MODE OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE WAVEPLAN_MODE_XPATH
/* #define OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE_CHANNEL_POWER_MODE OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE CHANNEL_POWER_MODE_XPATH */
#define OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE_CURRENT_WAVEPLAN OCM_OPENCONFIG_XPATH_EXT_MODULE_STATE CURRENT_WAVEPLAN
#define WAVEPLAN_UNINTALL "/usr/local/bin/sysrepoctl --uninstall --module=com-oplink-waveplan"
#define WAVEPLAN_INSTALL "/usr/local/bin/sysrepoctl --install --yang=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan.yang"

#define SWITCH_WAVEPLAN_CFG_50G      "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-50G.xml --datastore running --module com-oplink-waveplan"
#define SWITCH_WAVEPLAN_CFG_75G      "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-75G.xml --datastore running --module com-oplink-waveplan"
#define SWITCH_WAVEPLAN_CFG_100G     "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-100G.xml --datastore running --module com-oplink-waveplan"
#define SWITCH_WAVEPLAN_CFG_150G     "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-150G.xml --datastore running --module com-oplink-waveplan"
#define SWITCH_WAVEPLAN_CFG_175G     "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-NR1004-175G.xml --datastore running --module com-oplink-waveplan"
#define SWITCH_WAVEPLAN_CFG_300G     "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-300G.xml --datastore running --module com-oplink-waveplan"
#if defined __OLSP__ || defined __OTDR__
#define SWITCH_WAVEPLAN_CFG_DEFAULT  "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-OLSP.xml --datastore running --module com-oplink-waveplan"
#endif
#ifdef _NR1004_
#define SWITCH_WAVEPLAN_CFG_DEFAULT  "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-NR1004.xml --datastore running --module com-oplink-waveplan"
#endif
#ifdef __HCF__
#define SWITCH_WAVEPLAN_CFG_DEFAULT  "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-150G.xml --datastore running --module com-oplink-waveplan"
#endif
#ifdef _OCS_
#define SWITCH_WAVEPLAN_CFG_DEFAULT  "/usr/local/bin/sysrepocfg --import=/etc/sysrepo/yang-model/oplink/optical/com-oplink-waveplan-startup-75G.xml --datastore running --module com-oplink-waveplan"
#endif

#define WAVEPLAN_AUTO       "auto"
#define WAVEPLAN_50G        "50G"
#define WAVEPLAN_75G        "75G"
#define WAVEPLAN_100G       "100G"
#define WAVEPLAN_150G       "150G"
#define WAVEPLAN_175G       "175G"
#define WAVEPLAN_300G       "300G"

#define CLI_AWG_MODE_AUTO   "auto"
#define CLI_AWG_MODE_50G    "50G"
#define CLI_AWG_MODE_75G    "75G"
#define CLI_AWG_MODE_100G   "100G"
#define CLI_AWG_MODE_150G   "150G"
#define CLI_AWG_MODE_175G   "175G"
#define CLI_AWG_MODE_300G   "300G"

/* #define CHANNEL_POWER_AUTO  "ocm-auto" */
/* #define CHANNEL_POWER_RAW   "raw-data" */
/* #define CLI_OCM_AUTO        "ocm-auto" */
/* #define CLI_RAW_DATA        "raw-data" */
#define AWG_75G_PN          "1834770039"
#define AWG_150G_PN         "1834770043"
#define AWG_300G_PN         "1831781678"
#define WAVEPLAN_COUNT_50G   80
#define WAVEPLAN_COUNT_75G   65
#define WAVEPLAN_COUNT_100G  49
#define WAVEPLAN_COUNT_150G  33
#define WAVEPLAN_COUNT_175G  27
#define WAVEPLAN_COUNT_300G  17
#define DEF_OCM_CHANNEL_MAX (194)

/* OCM raw data rpc */
#define RPC_XPATH_OCM_RAW          "/com-oplink-rpc:get-ocm-raw"
#define RPC_XPATH_OCM_RAW_PORT     RPC_XPATH_OCM_RAW "/port"
#define RPC_XPATH_OCM_RAW_LEN      RPC_XPATH_OCM_RAW "/length"
#define RPC_XPATH_OCM_RAW_DATA     RPC_XPATH_OCM_RAW "/data"
#define RPC_XPATH_OCM_RAW_STATUS   RPC_XPATH_OCM_RAW "/status"


#define OCM_NODE_CHANNELS                        "channels"
#define OCM_NODE_NAME                            "name"
#define OCM_NODE_MONITOR_PORT                    "monitor-port"
#define OCM_NODE_CHANNEL_MONITOR                 "channel-monitor"
#define OCM_NODE_STATE                           "state"
#define OCM_NODE_LOWER_FREQUENCY                 "lower-frequency"
#define OCM_NODE_UPPER_FREQUENCY                 "upper-frequency"
#define OCM_NODE_POWER                           "power"

/* Refer to OCM module */

/*#define OCM_MODULE_START_FREQUENCY           (186050000)*/    /* Unit: MHZ */
/*#define OCM_MODULE_END_FREQUENCY             (190850000)*/    /* Unit: MHZ */
#define OCM_MODULE_START_FREQUENCY           (191175000)    /* Unit: MHZ */
#define OCM_MODULE_END_FREQUENCY             (196175000)    /* Unit: MHZ */
#define OCM_MODULE_SLICE_BANDWIDTH           (6250)         /* Unit: MHZ */
#define OCM_MODULE_SLICE_NUM_MAX             (768)


#define OCM_CHANNEL_NO_MAX_LEN        (32)

typedef struct {
    uint64_t ulLowFrequency;       /* Units: MHz, 1THz = 10^3 GHz = 10^6 MHz */
    uint64_t ulHighFrequency;      /* Units: MHz */
    double dPower;                 /* Units: dBm */
    char acChannelNo[OCM_CHANNEL_NO_MAX_LEN];    /* format: PXX.[1/2], eg: P20.1, P20.2 */
} OCM_CHANNEL_ST;

typedef struct {
    size_t nNum;
    OCM_CHANNEL_ST *pstChannelArray;
} OCM_CHANNELS_LIST_ST;

typedef struct {
    char acName[32];
    char acMonitorPort[32];
    char acCircuitPackName[32];
    char acComponentName[32];
    char acComponentType[64];
    uint32_t uiComponentIndex;
} OCM_PORT_DESC_ST;

#define OCM_PORT_UNKNOWN_STR                    "unknown"

#define OCM_PORT_PRE_AMP_INPUT_KEY              "pre-amp-input"
#define OCM_PORT_PRE_AMP_OUTPUT_KEY             "pre-amp-output"
#define OCM_PORT_BOOSTER_AMP_INPUT_KEY          "booster-amp-input"
#define OCM_PORT_BOOSTER_AMP_OUTPUT_KEY         "booster-amp-output"
#define OCM_PORT_EXTERNAL_KEY                   "external"
#define OCM_P_PORT_PRE_AMP_INPUT_KEY            "pre-amp-p-input"
#define OCM_P_PORT_PRE_AMP_OUTPUT_KEY           "pre-amp-p-output"
#define OCM_P_PORT_BOOSTER_AMP_INPUT_KEY        "booster-amp-p-input"
#define OCM_P_PORT_BOOSTER_AMP_OUTPUT_KEY       "booster-amp-p-output"
#define OCM_P_PORT_EXTERNAL_KEY                 "external-p"



#define DEF_OCM_PRE_AMP_INPUT               (1)
#define DEF_OCM_BOOSTER_AMP_OUTPUT          (2)
#define DEF_OCM_BOOSTER_AMP_INPUT           (3)
#define DEF_OCM_PRE_AMP_OUTPUT              (4)
#define DEF_OCM_EXTERNAL                    (5)
#define DEF_OCM_P_PRE_AMP_INPUT             (6)
#define DEF_OCM_P_BOOSTER_AMP_OUTPUT        (7)
#define DEF_OCM_P_BOOSTER_AMP_INPUT         (8)
#define DEF_OCM_P_PRE_AMP_OUTPUT            (9)
#define DEF_OCM_P_EXTERNAL                  (10)

#define OCM_KEY_MAX                         (10)

typedef struct {
    int32_t iCnt;
    char aacOcmKeys[OCM_KEY_MAX][24];
} OCM_KEY_ST;


#ifdef _NR1004_
typedef enum {

    EN_OCM_PORT_NONE = 0,

    EN_OCM_PORT_PRE_AMP_INPUT = 1,
    EN_OCM_PORT_BOOSTER_AMP_OUTPUT = 2,
    EN_OCM_PORT_BOOSTER_AMP_INPUT = 3,
    EN_OCM_PORT_PRE_AMP_OUTPUT = 4,

    EN_OCM_P_PORT_PRE_AMP_INPUT = 5,
    EN_OCM_P_PORT_BOOSTER_AMP_OUTPUT = 6,
    EN_OCM_P_PORT_BOOSTER_AMP_INPUT = 7,
    EN_OCM_P_PORT_PRE_AMP_OUTPUT = 8,

    EN_OCM_PORT_MAX

} OCM_PORT_ID_EN;
#else
typedef enum {

    EN_OCM_PORT_NONE = 0,

    EN_OCM_PORT_PRE_AMP_INPUT = 1,
    EN_OCM_PORT_BOOSTER_AMP_OUTPUT = 2,
    EN_OCM_PORT_BOOSTER_AMP_INPUT = 3,
    EN_OCM_PORT_PRE_AMP_OUTPUT = 4,
    EN_OCM_PORT_EXTERNAL = 5,

    EN_OCM_P_PORT_PRE_AMP_INPUT = 6,
    EN_OCM_P_PORT_BOOSTER_AMP_OUTPUT = 7,
    EN_OCM_P_PORT_BOOSTER_AMP_INPUT = 8,
    EN_OCM_P_PORT_PRE_AMP_OUTPUT = 9,
    EN_OCM_P_PORT_EXTERNAL = 10,

    EN_OCM_PORT_MAX

} OCM_PORT_ID_EN;
#endif


typedef enum {
    EN_OCM_PORT_ID_LINE_R = 0,
    EN_OCM_PORT_ID_LINE_T,
    EN_OCM_PORT_ID_LINE_COM_IN,
    EN_OCM_PORT_ID_LINE_COM_OUT,

    EN_OCM_PORT_ID_LINE_R_P = 4,
    EN_OCM_PORT_ID_LINE_T_P,
    EN_OCM_PORT_ID_LINE_COM_IN_P,
    EN_OCM_PORT_ID_LINE_COM_OUT_P,

    EN_OCM_PORT_ID_MAX = 8
} EN_OCM_PORT_ID;


typedef struct OCM_PORT_st {
    char acName[32];
    unsigned char ucId;
} OCM_PORT_ST;

typedef enum en_WAVEPLAN_MODE_TYPE
{
    WAVEPLAN_MODE_AUTO_TYPE = 0,
    WAVEPLAN_MODE_75G_TYPE,
    WAVEPLAN_MODE_150G_TYPE,
    WAVEPLAN_MODE_175G_TYPE,
    WAVEPLAN_MODE_300G_TYPE,
    WAVEPLAN_MODE_50G_TYPE,
    WAVEPLAN_MODE_100G_TYPE,
    WAVEPLAN_MODE_NONE_TYPE,
} WAVEPLAN_MODE_TYPE;

typedef enum en_CHANNEL_POWER_MODE_TYPE
{
    CHANNEL_POWER_MODE_AUTO_TYPE = 0,
    CHANNEL_POWER_MODE_RAW_TYPE,
    CHANNEL_POWER_MODE_NONE_TYPE,
} CHANNEL_POWER_MODE_TYPE;

typedef struct {
    char acWaveplanMode[32];
    /* char acChPwrMode[32]; */
} OCM_MODE_CONFIG_ST;

typedef struct {
    char acCurWaveplanMode[32];
} OCM_MODE_STATE_ST;

#define OCM_PORT_OUT65            (1)
#define OCM_PORT_OUT66            (2)
#define OCM_PORT_DROP1_COM1       (3)
#define OCM_PORT_DROP2_COM1       (4)



#ifdef MEIWU_PRO
#define OCM_PORT_OUT65            (1)
#define OCM_PORT_OUT66            (2)
#define OCM_PORT_DROP1_COM1       (3)
#define OCM_PORT_DROP2_COM1       (4)
#else
#define OCM_PORT_COM1_TX          (1)
#define OCM_PORT_COM1_RX          (2)
#endif

#define DEF_OCM_PORT_LINEIN_STR       "line-in"
#define DEF_OCM_PORT_LINEOUT_STR      "line-out"
#define DEF_OCM_PORT_LINEINP_STR      "line-in-p"
#define DEF_OCM_PORT_LINEOUTP_STR     "line-out-p"
#define DEF_OCM_PORT_COMIN_STR        "ocm-in"
#define DEF_OCM_PORT_COMOUT_STR       "ocm-out"

#define DEF_OCM_PORT_NUMBER_MAX       (6)

#define DEF_OCM_BA_CFG_KEY            "OCM-BA"
#define DEF_OCM_PA_CFG_KEY            "OCM-PA"
#define DEF_OCM_P_BA_CFG_KEY          "OCM-P-BA"
#define DEF_OCM_P_PA_CFG_KEY          "OCM-P-PA"


int DSC_GetOcmPortCnt();
int DSC_GetSingleOcmPortCnt(const char *pcComponentKey);
extern int DSC_GetOcmCfgKeyFromDb(sr_session_ctx_t *pstSess, OCM_KEY_ST *pstOcmKey);
extern char *DSC_GetOcmPortName(unsigned char ucPortId);
extern unsigned char DSC_GetOcmPortIdByNameNew(char *pcPortName);
extern int DSC_GetOcmPortNameByXpath(char *pcXpath, char *pcKey, size_t nLen);
extern int DSC_GetOcmChannelsByPort(char *pcCfgKey, char *pcPortName, OCM_CHANNELS_LIST_ST *pstList);
extern int DSC_GetOcmPortConfig(char *pcCfgKey, char *pcPortName, OCM_PORT_DESC_ST *pstPortDesc);
extern int DSC_SetOcmConfig(char *pcCfgKey, OCM_PORT_DESC_ST *pstOcmPortDesc);
extern int DSC_UpdateOcmConfig(char *pcCfgKey, OCM_PORT_DESC_ST *pstOcmPortDesc);
extern int DSC_GetOsaRpc(char *pcPortName, int16_t *psData, int16_t usBufLen, uint16_t *pusLen);
extern int DSC_GetOcmPowerByRawData(OCM_CHANNELS_LIST_ST *pstList, int16_t *psSliceRawData, uint32_t uiSliceCnt);
extern int DSC_GetOcmPortArrayLength();

/* add waveplan+++++ */
extern int DSC_GetWaveplanModeConfig(WAVEPLAN_MODE_TYPE *enWaveplanMode);
extern int Dsc_SetWaveplanMode(char *pcMode, bool bCommit);
/* extern int Dsc_SetChannelPowerMode(char *pcMode, bool bCommit); */
/* extern int DSC_GetChannelPowerModeConfig(CHANNEL_POWER_MODE_TYPE *enChannelPowerMode); */
extern int Dsc_SwitchWaveplan(int iCurCount, WAVEPLAN_MODE_TYPE enWaveplanMode, bool *pbNeedSetOcm);
/* extern int Dsc_SwitchWaveplanForce(char *pcWaveplanMode); */
extern int DSC_WavplanModeToValue(char *pcBuf, WAVEPLAN_MODE_TYPE *penWaveplanMode);
/* extern int DSC_ChannelPowerModeToValue(char *pcBuf, CHANNEL_POWER_MODE_TYPE *penChannelPowerMode); */
extern int DSC_SetOcmModeCfg(OCM_MODE_CONFIG_ST *pstModeCfg);
extern int DSC_GetOcmModeState(OCM_MODE_STATE_ST *pstModeState);
/* add waveplan----- */

extern int DSC_SetBaChInLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetBaChInHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetBaChOutLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetBaChOutHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetPaChInLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetPaChInHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetPaChOutLowThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern int DSC_SetPaChOutHighThreshold(char *pcCfgKey, double dHighThr, double dLowThr);
extern char *DSC_WavplanModeToStr(char *pcStr, WAVEPLAN_MODE_TYPE enMode);
#ifdef __cplusplus
}
#endif

#endif
