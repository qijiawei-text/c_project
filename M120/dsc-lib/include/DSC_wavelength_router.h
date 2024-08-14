#ifndef __DSC_WAVELENGTH_ROUTER_H__
#define __DSC_WAVELENGTH_ROUTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
/* WR: Wavelength Router*/

#define WR_MEIWU_PROC_FEATURE                1

#define WR_LOWER_FREQUENCY_MIN               (186050000)    /* Unit: MHZ */
#define WR_UPPER_FREQUENCY_MAX               (190850000)    /* Unit: MHZ */


#define WR_CENTER_FREQUENCY_BASE             (193100000)    /* Unit: MHZ, center frequency = 193.1THz + n*6.25GHz,  the type of n is int */
#define WR_CENTER_FREQ_OFFSET_UNIT           (6250)         /* Unit: MHZ, */

#define WR_CHANNEL_WIDTH_MIN                 (12500)        /* Unit: MHZ, channel width = m*12.5GHz, the type of m is uint32 */
#define WR_WSS_BAND_WIDTH_PER_SLICE          (6250)         /* Unit: MHZ */

#define DSC_MULTIPLE_1000                    (1000)
#define DSC_MULTIPLE_100                     (100)
#define DSC_MULTIPLE_10                      (10)

#define WR_CHANNEL_ATTN_WORKING              (10.0)         /* Unit: dB */
#define WR_CHANNEL_ATTN_MAX                  (25.5)         /* Unit: dB */
#define WR_CHANNEL_ATTN_MIN                  (0)            /* Unit: dB */

#define WR_INVALID_POWER                     (-60.00)       /* Unit: dBm */

#define WR_TARGET_POWER_MIN                  (-34.00)       /* Unit: dBm */
#define WR_TARGET_POWER_MAX                  (0)            /* Unit: dBm */

#define WR_ATTN_VAL_255                      (255)         /* Unit: 0.1dB */

#define WR_INDEX_MIN                         (1)
#define WR_INDEX_MAX                         (1000)

#define MODULE_ALIBABA_WSS_EXT                "com-alibaba-wavelength-router-ext"
#define ALI_WSS_EXT_XPATH_HEADER              "/" MODULE_ALIBABA_WSS_EXT
#define ALI_NODE_PORT_NAME                    "/port-name"
#define ALI_NODE_COLLECTION_TIME_PERIOD       "/collection-time-period"
#define ALI_NODE_SAMPLING_INTERVAL            "/sampling-interval"
#define ALI_NODE_STATUS                       "/status"
#define ALI_NODE_STATUS_MESSAGE               "/status-message"
#define ALI_NODE_WAVEFORM                     "/waveform"
#define ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH   ALI_WSS_EXT_XPATH_HEADER ":collect-port-waveform"

/*connection validation rpc */
#define ALI_RPC_CONNETION_VALIDATION_XPATH   ALI_WSS_EXT_XPATH_HEADER ":specify-connection-validation-ports"
#define ALI_TX_PORT "/port-pairs[1]/tx-port"
#define ALI_RX_PORT "/port-pairs[1]/rx-port"
#define ALI_HOLD_ON_TIME "/hold-on-time"
#define ALI_FAR_END_STATUS "/far-end-infos[1]/rx-status"
#define ALI_FAR_END_CHASSIS "/far-end-infos[1]/far-end-info/chassis-name"
#define ALI_FAR_END_PORT "/far-end-infos[1]/far-end-info/port-name"
#define ALI_FAR_END_LOSS "/far-end-infos[1]/far-end-info/connection-loss"

#define MODULE_WAVELENGTH_ROUTER              "openconfig-wavelength-router"
#define WAVE_ROUTER_XPATH_HEADER              "/" MODULE_WAVELENGTH_ROUTER ":wavelength-router"
#define WR_XPATH_MEDIA_CHANNELS               WAVE_ROUTER_XPATH_HEADER "/media-channels"
#define WR_LIST_CHANNEL                       "channel"
#define WR_KEY_INDEX                          "index"
#define WR_XPATH_CHANNEL                      WR_XPATH_MEDIA_CHANNELS "/channel[index='%d']"
#define WR_NODE_CONFIG                        "/config"
#define WR_XPATH_CHANNEL_CONFIG               WR_XPATH_CHANNEL WR_NODE_CONFIG

#define WR_XPATH_CHANNEL_INDEX_ALL            WR_XPATH_MEDIA_CHANNELS "/channel/index"

#define WR_NODE_INDEX                         "/index"
#define WR_NODE_CHANNEL_NAME                  "/name"
#define WR_NODE_LOW_FREQUENCY                 "/lower-frequency"
#define WR_NODE_UPPER_FREQUENCY               "/upper-frequency"
#define WR_NODE_ADMIN_STATUS                  "/admin-status"
#define WR_NODE_SUPPER_CHANNEL                "/super-channel"
#define WR_NODE_SUPPER_CHANNEL_PARENT         "/super-channel-parent"
#define WR_NODE_ASE_CONTROL_MODE              "/ase-control-mode"
#define WR_NODE_ASE_INJECTION_THRESHOLD       "/ase-injection-threshold"
#define WR_NODE_ATTN_CONTROL_MODE             "/attenuation-control-mode"
#define WR_NODE_OPER_STATUS                   "/oper-status"
#define WR_NODE_ASE_STATUS                    "/ase-status"

#define WR_NODE_INPUT_POWER                   "/com-alibaba-wavelength-router-ext:input-power"
#define WR_NODE_OUTPUT_POWER                  "/com-alibaba-wavelength-router-ext:output-power"
#define WR_NODE_ACTUAL_ATTN                   "/com-alibaba-wavelength-router-ext:actual-attenuation"

#define WR_NODE_STATE                         "/state"
#define WR_NODE_SOURCE                        "/source"
#define WR_NODE_DEST                          "/dest"

#define WR_XPATH_CHANNEL_STATE                WR_XPATH_CHANNEL "/state"
#define WR_XPATH_CHANNEL_SOURCE               WR_XPATH_CHANNEL "/source"
#define WR_XPATH_CHANNEL_DEST                 WR_XPATH_CHANNEL "/dest"
#define WR_NODE_PORT_NAME                     "/port-name"
#define WR_XPATH_SOURCE_CONFIG_PORT_NAME      WR_XPATH_CHANNEL_SOURCE WR_NODE_CONFIG WR_NODE_PORT_NAME
#define WR_XPATH_DEST_CONFIG_PORT_NAME        WR_XPATH_CHANNEL_DEST WR_NODE_CONFIG WR_NODE_PORT_NAME
#define WR_NODE_SPECTRUM_POWER_PROFILE        "/spectrum-power-profile"
#define WR_XPATH_SPECTRUM_POWER_PROFILE       WR_XPATH_CHANNEL "/spectrum-power-profile"
#define WR_LIST_DISTRIBUTION                  "distribution"
#define WR_KEY_LOWER_FREQUENCY                "lower-frequency"
#define WR_KEY_UPPER_FREQUENCY                "upper-frequency"
#define WR_XPATH_SPECTRUM_DISTRIBUTION        WR_XPATH_SPECTRUM_POWER_PROFILE "/distribution[lower-frequency='%lu'][upper-frequency='%lu']"

#define WR_XPATH_SPECTRUM_CONFIG              WR_XPATH_SPECTRUM_DISTRIBUTION WR_NODE_CONFIG
#define WR_NODE_TARGET_POWER                  "/target-power"
#define WR_NODE_ATTN                          "/com-alibaba-wavelength-router-ext:attenuation-value"

#define WR_NODE_PORT_SPECTRUM_POWER_PROFILES  "/port-spectrum-power-profiles"

#define WR_STR_UNKNOWN                                "UNKNOWN"
#define WR_STR_ENABLE                                 "enable"
#define WR_STR_DISABLE                                "disable"
#define WR_STR_APC                                    "APC"
#define WR_STR_ATTN                                   "ATTN"

/* channel admin state */
#define WR_STR_ENABLED                                    "ENABLED"
#define WR_STR_DISABLED                                   "DISABLED"
#define WR_STR_MAINT                                      "MAINT"

typedef enum {
    WR_ADMIN_STATUS_ENABLED = 0,
    WR_ADMIN_STATUS_DISABLED,
    WR_ADMIN_STATUS_MAINT,
    WR_ADMIN_STATUS_MAX
} WR_ADMIN_STATUS_EN;

/* ASE control mode */
#define WR_STR_ASE_ENABLED                            "ASE_ENABLED"
#define WR_STR_ASE_DISABLED                           "ASE_DISABLED"
#define WR_STR_AUTO_ASE_ON_FAILURE                    "AUTO_ASE_ON_FAILURE"
#define WR_STR_AUTO_ASE_FAILURE_AND_RESTORE           "AUTO_ASE_FAILURE_AND_RESTORE"

typedef enum {
    WR_ASE_ENABLED = 0,
    WR_ASE_DISABLED,
    WR_AUTO_ASE_ON_FAILURE,
    WR_AUTO_ASE_FAILURE_AND_RESTORE,
    WR_ASE_CONTROL_MODE_MAX
} WR_ASE_CONTROL_MODE_EN;

/* Operational state of the media channel */
#define WR_STR_UP                                     "UP"
#define WR_STR_DOWN                                   "DOWN"

typedef enum {
    WR_OPER_STATUS_UP = 0,
    WR_OPER_STATUS_DOWN,
    WR_OPER_STATUS_MAX
} WR_OPER_STATUS_EN;

/* Status of injected ASE noise on the media channel */
#define WR_STR_PRESENT                                "PRESENT"
#define WR_STR_NOT_PRESENT                            "NOT_PRESENT"

typedef enum {
    WR_ASE_STATUS_PRESENT = 0,
    WR_ASE_STATUS_NOT_PRESENT,
    WR_ASE_STATUS_MAX
} WR_ASE_STATUS_EN;

/* Status of injected ASE noise on the media channel */
#define WR_STR_ATTN_FIXED_LOSS                        "ATTENUATION_FIXED_LOSS"
#define WR_STR_ATTN_DYNAMIC_LOSS                      "ATTENUATION_DYNAMIC_LOSS"
#define WR_STR_SET_ATTENUATION                        "SET_ATTENUATION"

typedef enum {

    WR_ATTENUATION_FIXED_LOSS = 0,
    WR_ATTENUATION_DYNAMIC_LOSS,
    WR_SET_ATTENUATION,
    WR_ATTN_CONTROL_MODE_MAX
} WR_ATTN_CONTROL_MODE_EN;

#define WR_CHANNEL_NAME_LEN_MAX           (24)

typedef struct CHANNEL_CFG_st {
    uint32_t uiIndex;
    char acName[WR_CHANNEL_NAME_LEN_MAX];
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
    WR_ADMIN_STATUS_EN enAdminStatus;
    bool bSupperChannel;
    uint32_t uiSupperChanIdx;
    WR_ASE_CONTROL_MODE_EN enAseCtrlMode;
    double dAseInjectThr;
    WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode;
} CHANNEL_CFG_ST;

typedef struct CHANNEL_STATE_st {
    uint32_t uiIndex;
    char acName[WR_CHANNEL_NAME_LEN_MAX];
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
    WR_ADMIN_STATUS_EN enAdminStatus;
    bool bSupperChannel;
    uint32_t uiSupperChanIdx;
    WR_ASE_CONTROL_MODE_EN enAseCtrlMode;
    double dAseInjectThr;
    WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode;
    WR_OPER_STATUS_EN enOperStatus;
    WR_ASE_STATUS_EN enAseStatus;
    double dInputPwr;
    double dOutputPwr;
    double dActualAttn;
} CHANNEL_STATE_ST;

typedef struct CONN_PORT_st {
    char acPortName[16];
} CONN_PORT_ST;

typedef struct SPECTRUM_POWER_st {
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
    double dTargetPwr;
    double dAttn;
} SPECTRUM_POWER_ST;

typedef struct DISTRIBUTION_st {
    SPECTRUM_POWER_ST stSpecPwrCfg;
    SPECTRUM_POWER_ST stSpecPwrState;
} DISTRIBUTION_ST;

#define WR_DISTRIBUTION_NUM_UPPER_LIMIT    (1)  /* less than WR_DISTRIBUTION_NUM_MAX */
#define WR_DISTRIBUTION_NUM_MAX           (10)

typedef struct SPECTRUM_POWER_PROFILE_st {
    uint32_t uiTotal;
    /* SPECTRUM_POWER_ST astSpecPwr[WR_DISTRIBUTION_NUM_MAX]; */
    DISTRIBUTION_ST astDist[WR_DISTRIBUTION_NUM_MAX];
} SPECTRUM_POWER_PROFILE_ST;

typedef struct WAVE_ROUTER_CHANNEL_st {
    CHANNEL_CFG_ST stCfg;
    CHANNEL_STATE_ST stState;
    CONN_PORT_ST stSrcConnPort;
    CONN_PORT_ST stDestConnPort;
    SPECTRUM_POWER_PROFILE_ST stSpecPwrProfile;
} CHANNEL_ST;

typedef struct MEDIA_CHANNELS_st {
    uint32_t uiTotal;
    CHANNEL_ST *pastChanList;
} MEDIA_CHANNELS_ST;

typedef struct WR_WAVEPLAN_st {
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
} WR_WAVEPLAN_ST;

#define WR_WAVEPLAN_NUM_MAX         (387)
typedef struct WR_WAVEPLAN_LIST_st {
    uint32_t uiTotal;
    WR_WAVEPLAN_ST astWaveplan[WR_WAVEPLAN_NUM_MAX];
} WR_WAVEPLAN_LIST_ST;

typedef enum WR_WSS_IDX_en {
    WR_WSS_IDX_1 = 0,
    WR_WSS_IDX_2,
    WR_WSS_IDX_MAX
} WR_WSS_IDX_EN;

typedef enum WR_WSS_DIRECTION_en {
    WR_WSS_DIRECTION_EGRESS = 0,     /* egress:    In01~In64 (source port) ---> Out64/65 (dest port) */
    WR_WSS_DIRECTION_INGRESS,        /* ingress:   In65/66 (source port) ---> Out0165 (dest port) */
    WR_WSS_DIRECTION_MAX
} WR_WSS_DIRECTION_EN;

typedef struct WR_WSS_CFG_st {
    uint32_t uiIndex;
    uint32_t uiWssIdx;              /* refer to WR_WSS_IDX_EN */
    uint32_t uiWssDirection;        /* refer to WR_WSS_DIRECTION_EN */
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
    /* uint32_t uiSrcPort; */
    /* uint32_t uiDestPort; */
    uint32_t uiComPort;             /* refer to WR_ROUTER_COMMON_PORT_EN */
    uint32_t uiExpPort;             /* range: [WR_WSS1_PORT_MIN, WR_WSS2_PORT_MAX] */
    double dTargetPwr;
    double dAttn;
    WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode;
    WR_ADMIN_STATUS_EN enAdminStatus;

} WR_WSS_CFG_ST;

typedef struct WR_WSS_CFG_LIST_st {
    uint32_t uiTotal;
    WR_WSS_CFG_ST astWssCfg[WR_WAVEPLAN_NUM_MAX];
} WR_WSS_CFG_LIST_ST;

/* wss ingress/egress port */
#define WR_STR_PREFIX_IN               "In"
#define WR_STR_PREFIX_OUT              "Out"
#define WR_STR_PREFIX_DROP             "Drop"

#ifdef MEIWU_PRO
#define WR_WSS1_PORT_MIN                (1)
#define WR_WSS1_PORT_MAX                (32)

#define WR_WSS2_PORT_MIN                (33)
#define WR_WSS2_PORT_MAX                (64)
#define WR_WSS_ADD_PORT_TOTAL_NUM       (64)
#define WR_WSS_DROP_PORT_TOTAL_NUM      (64)
#else
#define WR_WSS1_PORT_MIN                (1)
#define WR_WSS1_PORT_MAX                (20)
#define WR_WSS_ADD_PORT_TOTAL_NUM       (20)
#define WR_WSS_DROP_PORT_TOTAL_NUM      (20)
#endif

#define WR_STR_ADD1_COM1                ("Add1_Com1")
#define WR_STR_ADD1_COM2                ("Add1_Com2")
#define WR_STR_DROP1_COM1               ("Drop1_Com1")
#define WR_STR_DROP1_COM2               ("Drop1_Com2")

#define WR_STR_ADD2_COM1                ("Add2_Com1")
#define WR_STR_ADD2_COM2                ("Add2_Com2")
#define WR_STR_DROP2_COM1               ("Drop2_Com1")
#define WR_STR_DROP2_COM2               ("Drop2_Com2")


#ifdef MEIWU_PRO
#define WR_STR_IN65                     ("In65")
#define WR_STR_IN66                     ("In66")
#define WR_STR_OUT65                    ("Out65")
#define WR_STR_OUT66                    ("Out66")
#define WR_STR_IN1                      ("IN1")
#define WR_STR_IN2                      ("IN2")
#define WR_STR_OUT1                     ("OUT1")
#define WR_STR_OUT2                     ("OUT2")
#else
#define WR_STR_IN1                      ("IN1")
#define WR_STR_OUT1                     ("OUT1")
#endif


/* wss common port */
#ifdef MEIWU_PRO
typedef enum WR_ROUTER_COMMON_PORT_en {
#if 0
    WR_COMMON_PORT_MIN = 0,
    WR_COMMON_PORT_1,        /* wss1-1 business port */
    WR_COMMON_PORT_2,        /* wss1-1 loopback port */
    WR_COMMON_PORT_3,        /* wss1-2 business port */
    WR_COMMON_PORT_4,        /* wss1-2 loopback port */
    WR_COMMON_PORT_5,        /* wss2-1 business port */
    WR_COMMON_PORT_6,        /* wss2-1 loopback port */
    WR_COMMON_PORT_7,        /* wss2-2 business port */
    WR_COMMON_PORT_8,        /* wss2-2 loopback port */
    WR_COMMON_PORT_MAX
#else
    WR_COMMON_PORT_MIN = 0,
    WR_ADD1_COM1,                /* wss1-1 business port */
    WR_ADD1_COM2,                /* wss1-1 loopback port */
    WR_DROP1_COM1,               /* wss1-2 business port */
    WR_DROP1_COM2,               /* wss1-2 loopback port */
    WR_ADD2_COM1,                /* wss2-1 business port */
    WR_ADD2_COM2,                /* wss2-1 loopback port */
    WR_DROP2_COM1,               /* wss2-2 business port */
    WR_DROP2_COM2,               /* wss2-2 loopback port */
    WR_COMMON_PORT_MAX
#endif
} WR_ROUTER_COMMON_PORT_EN;
#else
typedef enum WR_ROUTER_COMMON_PORT_en {
    WR_COMMON_PORT_MIN = 0,
    WR_ADD1_COM1,                /* wss1-1 business port */
    WR_ADD1_COM2,                /* wss1-1 loopback port */
    WR_DROP1_COM1,               /* wss1-2 business port */
    WR_DROP1_COM2,               /* wss1-2 loopback port */
    WR_COMMON_PORT_MAX
} WR_ROUTER_COMMON_PORT_EN;

#endif



typedef struct {
    char acPortName[DEF_BUF_SIZE_16];
    uint32_t uiPortNum;
    uint16_t usTimePeriod;
    uint16_t usResv;
    double aadPwr[64][600];
} WSS_WAVEFORM_ST;

WR_ASE_STATUS_EN DSC_WrAseStatusToEnum(char *pcAseStatus);
char *DSC_WrAseStatusToStr(WR_ASE_STATUS_EN enAseStatus, char *pcBuf, size_t nSize);

WR_ATTN_CONTROL_MODE_EN DSC_WrAttnCtrlModeToEnum(char *pcAttnCtrlMode);
char *DSC_WrAttnCtrlModeToStr(WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode, char *pcBuf, size_t nSize);

WR_ADMIN_STATUS_EN DSC_WrAdminStatusToEnum(char *pcAdminStatus);
char *DSC_WrAdminStatusToStr(WR_ADMIN_STATUS_EN enAdminStatus, char *pcBuf, size_t nSize);

WR_ASE_CONTROL_MODE_EN DSC_WrAseCtrlModeToEnum(char *pcAseCtrlMode);
char *DSC_WrAseCtrlModeToStr(WR_ASE_CONTROL_MODE_EN enAseCtrlMode, char *pcBuf, size_t nSize);

WR_OPER_STATUS_EN DSC_WrOperStatusToEnum(char *pcOperStatus);
char *DSC_WrOperStatusToStr(WR_OPER_STATUS_EN enOperStatus, char *pcBuf, size_t nSize);

int DSC_WrGetMediaChannelsInfo(sr_session_ctx_t *pstSession, MEDIA_CHANNELS_ST *pstMediaChannels);
int DSC_WrParseWaveplan(const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WAVEPLAN_LIST_ST *pstWaveplanList);
int DSC_WrGetWaveplan(WR_WAVEPLAN_LIST_ST *pstWaveplanList);

int DSC_WrCheckChannelCfg(const CHANNEL_ST *pstChanList, WR_WSS_CFG_ST *pstWssCfg);
int DSC_WrCheckMediaChannels(const MEDIA_CHANNELS_ST *pstMediaChannels);

/* int DSC_WrGetChannelsByDirection(WR_WSS_DIRECTION_EN enWssDirection, const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WSS_CFG_LIST_ST *pstWssCfgList); */
/* int DSC_WrParseMetaChannels(const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WSS_CFG_LIST_ST *pstEgressCfgList, WR_WSS_CFG_LIST_ST *pstIngressCfgList); */
int DSC_WrParseMetaChannels(const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WSS_CFG_LIST_ST *pstEgressCfgList, WR_WSS_CFG_LIST_ST *pstIngressWss1CfgList, WR_WSS_CFG_LIST_ST *pstIngressWss2CfgList);

int DSC_WrGetSingleMediaChannel(sr_session_ctx_t *pstSession, uint32_t uiIndex, CHANNEL_ST *pstChannel);
int DSC_WrAddChannelIndex(uint32_t uiIndex, bool bCommit);

int DSC_WrSetChannelName(uint32_t uiIndex, char *pcChannelName, bool bCommit);
int DSC_WrSetChannelFreqency(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit);
int DSC_WrSetChannelAdminStatus(uint32_t uiIndex, WR_ADMIN_STATUS_EN enAdminStatus, bool bCommit);
int DSC_WrSetChannelAttnCtrlMode(uint32_t uiIndex, WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode, bool bCommit);

int DSC_WrSetPortConnection(uint32_t uiIndex, char *pcSrcPortName, char *pcDestPortName, bool bCommit);

int DSC_WrSetDistributionFrequency(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit);
int DSC_WrSetDistributionTargetPwr(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, double dTargetPwr, bool bCommit);
int DSC_WrSetDistributionAttn(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, double dAttn, bool bCommit);
int DSC_WrAddDistribution(uint32_t uiIndex, SPECTRUM_POWER_ST *pstSpectrumPwrCfg, bool bCommit);

int DSC_WrDeleteDistribution(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit);
int DSC_WrDeleteMediaChannel(uint32_t uiIndex);
int DSC_WrDeleteAllMediaChannels();
int DSC_WrDeleteChannelList();

bool DSC_IsPortConnectionWithDirection(WR_WSS_DIRECTION_EN enWssDirection, WR_WSS_IDX_EN enWssIdx, char *pcSrcPortName, char *pcDestPortName);
int DSC_WrAddMediaChannel(CHANNEL_ST *pstChannel);

int DSC_WrInitChannelDefaultVal(CHANNEL_ST *pstChannel, uint32_t uiIndex);

int DSC_WrConvertWaveplanToOcm(const WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WAVEPLAN_LIST_ST *pstOcmWaveplanList);

int DSC_WrGetOcmWaveplan(WR_WAVEPLAN_LIST_ST *pstWaveplanList);

int DSC_RpcCollectPortWaveform(WSS_WAVEFORM_ST *pstWssWaveform);

bool DSC_WrIsChannelExisted(uint32_t uiIndex);

int DSC_WrGetIdleMinIndex(uint32_t *puiIndex);

int DSC_WRGetOldPortNameByNew(const char *pcPortName, char *pcOldPort, size_t nLen);

int DSC_WRGetNewPortNameByOldIn(const char *pcPortName, char *pcNewPortName, size_t nLen);

int DSC_WrGetOpsLedIdxByNewInPortName(const char *pcPortName, uint32_t *puiPordLedIdx);

#ifdef __cplusplus
}
#endif

#endif

