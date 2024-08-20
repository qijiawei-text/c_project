/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_msft.h
* Description:   The header file
* Author:         Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1] Guangjun Guo   2018/03/17   Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __REST_MSFT_H__
#define __REST_MSFT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* #define REST_DATA_ROLLBACK                (1) */

/*define name description in JSON structure*/
#define STR_PRE_AMP                       "pre-amp"
#define STR_PRE_AMP_P                     "pre-amp-p"
#define STR_TARGET_GAIN                   "target-gain"
#define STR_ACTUAL_GAIN                   "actual-gain"
#define STR_TARGET_GAIN_TILT              "target-gain-tilt"
#define STR_APR_ENABLE                    "apr-enable"
#define STR_ENABLE                        "enable"
#define STR_DISABLE                       "disable"
#define STR_STATE                         "state"
#define STR_UP                            "up"
#define STR_DOWN                          "down"
#define STR_STATUS                        "status"
#define STR_HITLESS                       "hitless"
#define STR_FILE_SIZE                     "filesize"


#define STR_BOOSTER_AMP                   "booster-amp"
#define STR_BOOSTER_AMP_P                 "booster-amp-p"
#define STR_TARGET_VOA                    "voa"    /*   "target-voa" */
#define STR_ACTUAL_VOA                    "actual-voa"
/* #define STR_TCDC                          "todc-value" / * fandy 20190125 * / */

#define STR_SYSTEM                        "system"
#define STR_NODE_NAME                     "nodename"
#define STR_GATEWAY                       "gateway"
#define STR_NETMASK                       "netmask"
#define STR_IP_ADDRESS                    "ip-address"
#define STR_OSC_IP_ADDRESS_REMOTE         "osc-ip-address-remote"
#define STR_OSC_IP_ADDRESS_LOCAL          "osc-ip-address-local"

#define STR_NTP_IP_1                      "ntp-ip-1"
#define STR_NTP_IP_2                      "ntp-ip-2"

#define STR_SYSLOG_IP_1                   "syslog-ip-1"
#define STR_SYSLOG_IP_2                   "syslog-ip-2"

#define STR_TACACS_IP_1                   "tacacs-ip-1"
#define STR_TACACS_KEY_1                  "tacacs-server-secret-1"
#define STR_TACACS_PORT_1                 "tacacs port 1"
#define STR_TACACS_IP_2                   "tacacs-ip-2"
#define STR_TACACS_KEY_2                  "tacacs-server-secret-2"

#define STR_SERIAL_NUMBER                 "serial-number"
#define STR_PART_NUMBER                   "part-number"
#define STR_SOFTWARE_VERSION              "software-version"
#define STR_FIRMWARE_VERSION              "firmware-version"

#define STR_SERIAL_NUMBER_MODULE          "serial-number-module"
#define STR_PART_NUMBER_MODULE            "part-number-module"
#define STR_SOFTWARE_VERSION_MODULE       "software-version-module"
#define STR_VENDOR                        "vendor"
#define STR_MOLEX                         "MOLEX"
#define STR_APSD_ENABLE                   "apsd-enable"
#define STR_APSD_STATUS                   "apsd-status"

#define STR_SERIAL_NUMBER_OCS             "serial-number-ocs"
#define STR_PART_NUMBER_OCS               "part-number-ocs"
#define STR_SERIAL_NUMBER_OPS             "serial-number-ops"
#define STR_PART_NUMBER_OPS               "part-number-ops"
#define STR_SERIAL_NUMBER_AMP             "serial-number-amp"
#define STR_PART_NUMBER_AMP               "part-number-amp"
#define STR_SERIAL_NUMBER_AMP_P           "serial-number-amp-p"
#define STR_PART_NUMBER_AMP_P             "part-number-amp-p"
#define STR_SERIAL_NUMBER_SCC             "serial-number-scc"
#define STR_PART_NUMBER_SCC               "part-number-scc"
#define STR_SERIAL_NUMBER_OPTICAL         "serial-number-optical"
#define STR_PART_NUMBER_OPTICAL           "part-number-optical"

#define STR_SERIAL_NUMBER_POWER_A         "serial-number-power-a"
#define STR_PART_NUMBER_POWER_A           "part-number-power-a"
#define STR_SERIAL_NUMBER_POWER_B         "serial-number-power-b"
#define STR_PART_NUMBER_POWER_B           "part-number-power-b"

#define STR_SERIAL_NUMBER_FAN_A           "serial-number-fan-0"
#define STR_PART_NUMBER_FAN_A             "part-number-fan-0"
#define STR_SERIAL_NUMBER_FAN_B           "serial-number-fan-1"
#define STR_PART_NUMBER_FAN_B             "part-number-fan-1"
#define STR_SERIAL_NUMBER_FAN_C           "serial-number-fan-2"
#define STR_PART_NUMBER_FAN_C             "part-number-fan-2"
#define STR_SERIAL_NUMBER_FAN_D           "serial-number-fan-3"
#define STR_PART_NUMBER_FAN_D             "part-number-fan-3"

#define STR_SERIAL_NUMBER_MUX             "serial-number-mux"
#define STR_PART_NUMBER_MUX               "part-number-mux"

#define STR_USER_NAME                     "username"
#define STR_PASSWORD                      "password"
#define STR_ACCESS                        "access"

#define STR_ENTITY                        "entity"
#define STR_RESTART                       "restart"

#define STR_FILE_NAME                     "file_name"
#define STR_LOAD                          "load"
#define STR_USER_ID                       "user-id"
#define STR_SOURCE_FILE_NAME              "source-file-name"
#define STR_DESTINATION_FILE_NAME         "destination-file-name"

#define STR_ALARM_SEVERITY                "alarm-severity"
#define STR_TIMESTAMP                     "timestamp"
#define STR_ENTITY                        "entity"
#define STR_DESCRIPTION                   "description"

#define STR_ENTITY_MODULE                 "module"
#define STR_ENTITY_SCC                    "scc"
#define STR_ENTITY_AMP_CARD               "amp"
#define STR_ENTITY_AMP_P_CARD             "amp-p"
#define STR_ENTITY_OPS                    "ops"
#define STR_ENTITY_OCS                    "ocs"
#define STR_ENTITY_OPTICAL_CARD           "optical-card"
#define STR_ENTITY_OPTICAL_CARD_P         "optical-card-p"

#define STR_ENTITY_PRE_AMP                "pre-amp"
#define STR_ENTITY_BOOSTER_AMP            "booster-amp"
#define STR_ENTITY_OCM                    "ocm"
#define STR_ENTITY_OTDR                   "otdr"

#define STR_TIMESTAMP                        "timestamp"
#define STR_LINE_OUTPUT_POWER                "line-output-power"
#define STR_LINE_INPUT_POWER                 "line-input-power"
#define STR_PRE_AMP_INPUT_POWER              "pre-amp-input-power"
#define STR_PRE_AMP_OUTPUT_POWER             "pre-amp-output-power"
#define STR_BOOSTER_AMP_INPUT_POWER          "booster-amp-input-power"
#define STR_BOOSTER_AMP_OUTPUT_POWER         "booster-amp-output-power"
#define STR_OSC_OUTPUT_POWER                 "osc-output-power"
#define STR_OSC_INPUT_POWER                  "osc-input-power"
#define STR_OSC_ADD_POWER                    "osc-add-power"
#define STR_OSC_DROP_POWER                   "osc-drop-power"
#define STR_OSC_RX_VOA_LOSS                  "osc-rx-voa-loss"

#define STR_PRE_AMP_GAIN                     "pre-amp-gain"
#define STR_PRE_VOA_ATTENUATION              "pre-voa-attenuation"
#define STR_TARGET_PRE_AMP_GAIN              "target-pre-amp-gain"
#define STR_PRE_AMP_TILT                     "pre-amp-tilt"
#define STR_BOOSTER_AMP_GAIN                 "booster-amp-gain"
#define STR_TARGET_BOOSTER_AMP_GAIN          "target-booster-amp-gain"
#define STR_BOOSTER_AMP_TILT                 "booster-amp-tilt"
#define STR_BOOSTER_AMP_VOA                  "booster-voa-attenuation"
#define STR_PM_BOOSTER_AMP_VOA               "booster-amp-voa"
#define STR_PM_PRE_AMP_VOA                   "pre-amp-voa"
#define STR_PM_BOOSTER_AMP_RETURN_LOSS       "booster-amp-return-loss"
#define STR_PM_PRE_AMP_RETURN_LOSS           "pre-amp-return-loss"
#define STR_PM_PRE_AMP_VOA_OUTPUT_POWER      "pre-amp-voa-output-power"
#define STR_PM_BOOSTER_AMP_VOA_OUTPUT_POWER  "booster-amp-voa-output-power"
#define STR_PM_PRE_AMP_REFLECTION_POWER      "pre-amp-reflection-power"
#define STR_PM_BOOSTER_AMP_REFLECTION_POWER  "booster-amp-reflection-power"
#define STR_PM_PRE_AMP_PUMP_CURRENT_PREFIX   "pre-amp-pump-current-"
#define STR_PM_BOOSTER_AMP_PUMP_CURRENT_PREFIX   "booster-amp-pump-current-"
#define STR_PM_PRE_AMP_PUMP_TEMP_PREFIX      "pre-amp-pump-temperature-"
#define STR_PM_BOOSTER_AMP_PUMP_TEMP_PREFIX      "booster-amp-pump-temperature-"
#define STR_PM_BOOSTER_AMP_MM_PUMP_CURRENT_PREFIX   "booster-amp-mm-pump-current-"
#define STR_PM_BOOSTER_AMP_MM_PUMP_TEMP_PREFIX      "booster-amp-mm-pump-temperature-"
#define STR_PM_PRE_AMP_MODULE_TEMP                  "pre-amp-module-temperature"
#define STR_PM_BOOSTER_AMP_MODULE_TEMP       "booster-amp-module-temperature"

#define STR_PM_SCC_CARD_TEMP                 "scc-card-temperature"

#define STR_PM_FAN_SPEED_PREFIX              "fan-speed-"

#define STR_PM_PSU_TEMP_PREFIX              "psu-card-temperature-"
#define STR_PM_PSU_INPUT_CURRENT_PREFIX     "psu-card-input-current-"
#define STR_PM_PSU_OUTPUT_CURRENT_PREFIX    "psu-card-output-current-"
#define STR_PM_PSU_INPUT_VOLTAGE_PREFIX     "psu-card-input-voltage-"
#define STR_PM_PSU_OUTPUT_VOLTAGE_PREFIX    "psu-card-output-voltage-"
#define STR_PM_PSU_OUTPUT_POWER_PREFIX      "psu-card-output-power-"

#define STR_PM_LINE_CV_TX_POWER             "line-cv-tx-power"
#define STR_PM_LINE_CV_RX_POWER             "line-cv-rx-power"

#define STR_LINEP_OUTPUT_POWER                     "lineP-output-power"
#define STR_LINEP_INPUT_POWER                      "lineP-input-power"
#define STR_LINEP_PRE_AMP_INPUT_POWER              "lineP-pre-amp-input-power"
#define STR_LINEP_PRE_AMP_OUTPUT_POWER             "lineP-pre-amp-output-power"
#define STR_LINEP_BOOSTER_AMP_INPUT_POWER          "lineP-booster-amp-input-power"
#define STR_LINEP_BOOSTER_AMP_OUTPUT_POWER         "lineP-booster-amp-output-power"
#define STR_LINEP_OSC_OUTPUT_POWER                 "lineP-osc-output-power"
#define STR_LINEP_OSC_INPUT_POWER                  "lineP-osc-input-power"
#define STR_LINEP_PRE_AMP_GAIN                     "lineP-pre-amp-gain"
#define STR_LINEP_PRE_VOA_ATTENUATION              "lineP-pre-voa-attenuation"
#define STR_LINEP_TARGET_PRE_AMP_GAIN              "lineP-target-pre-amp-gain"
#define STR_LINEP_PRE_AMP_TILT                     "lineP-pre-amp-tilt"
#define STR_LINEP_BOOSTER_AMP_GAIN                 "lineP-booster-amp-gain"
#define STR_LINEP_TARGET_BOOSTER_AMP_GAIN          "lineP-target-booster-amp-gain"
#define STR_LINEP_BOOSTER_AMP_TILT                 "lineP-booster-amp-tilt"
#define STR_LINEP_LINEP_BOOSTER_AMP_VOA            "lineP-booster-voa-attenuation"
#define STR_PM_LINEP_BOOSTER_AMP_VOA               "lineP-booster-amp-voa"
#define STR_PM_LINEP_PRE_AMP_VOA                   "lineP-pre-amp-voa"
#define STR_PM_LINEP_BOOSTER_AMP_RETURN_LOSS       "lineP-booster-amp-return-loss"
#define STR_PM_LINEP_PRE_AMP_RETURN_LOSS           "lineP-pre-amp-return-loss"

#define STR_PM_OPS_W_INPUT_POWER                   "ops-w-input-power"
#define STR_PM_OPS_P_INPUT_POWER                   "ops-p-input-power"

/* #define STR_PRE_AMP_MSA_INPUT                "pre-amp-mid-stage-input" */
/* #define STR_PRE_AMP_MSA_OUTPUT               "pre-amp-mid-stage-output" */
/* #define STR_BOOSTER_AMP_MSA_INPUT            "booster-amp-mid-stage-input" */
/* #define STR_BOOSTER_AMP_MSA_OUTPUT           "booster-amp-mid-stage-output" */
/* #define STR_TODC_DISPERSION                  "todc-dispersion" */

#define STR_INTERVAL_SEC                     "interval (second)"
#define STR_AVG                              "avg"
#define STR_MIN                              "min"
#define STR_MAX                              "max"
#define STR_VALUE                            "value"

#define STR_SPAN_LOSS                        "span-loss"  /* --- */
#define STR_PROGRESS                         "progress"
#define STR_CD_VAL                           "todc-value"

#define STR_FIBER_TYPE                       "fiber-type"
#define STR_NO_OF_CARRIERS                   "number-of-carriers"
#define STR_LAUNCHING_POWER                  "launching-power-per-channel"
#define STR_DROP_POWER_PER_CHANNEL           "drop-power-per-channel"
#define STR_LONG_SPAN                        "long-span"
#define STR_SPANLOSS_THRESHOLD               "span-loss-threshold"

#define STR_FIBER_TYPE_P                     "fiber-type-p"
#define STR_NO_OF_CARRIERS_P                 "number-of-carriers-p"
#define STR_LAUNCHING_POWER_P                "launching-power-per-channel-p"
#define STR_DROP_POWER_PER_CHANNEL_P         "drop-power-per-channel-p"
#define STR_LONG_SPAN_P                      "long-span-p"
#define STR_SPANLOSS_THRESHOLD_P             "span-loss-threshold-p"

#define STR_TIMEZONE                         "timezone"

#define STR_IPV6_MODE                        "ipv6-mode"
#define STR_IPV6_ADDRESS                     "ipv6-address"
#define STR_IPV6_GATEWAY                     "ipv6-gateway"

#define STR_WAVEPLAN_MODE                    "waveplan-mode"
#define STR_CHANNEL_POWER_MODE               "channel-power-mode"
#define STR_CURRENT_WAVEPLAN                 "current-waveplan"

#define STR_PATH_SW_IMAGE_FIRMWAREBUNDLE     "/sw-image/FirmwareBundle"
#define STR_PATH_TMP_FIRMWAREBUNDLE          "/tmp/FirmwareBundle"

#define STR_ETH0                      "eth0"

#define STR_UPG_STATE_COMMITTED       "committed"         /* TBD */
#define STR_UPG_STATE_DOWNLOADING     "downloading"       /* TBD */
#define STR_UPG_STATE_STAGED          "staged"            /* TBD */
#define STR_UPG_STATE_READY_DOWNLOAD  "ready to activate" /* TBD */
#define STR_UPG_STATE_ACTIVATING      "activating"        /* TBD */

#define STR_UPG_STATUS_SUCCESS        "success"           /* TBD */
#define STR_UPG_STATUS_IN_PROGRESS    "in-progress"       /* TBD */
#define STR_UPG_STATUS_FAILURE        "failure"           /* TBD */
#define STR_UPG_STATUS_CHECK_ERR      "check-error"       /* TBD */


#define STR_NA                               "NA"

#define STR_CHANNELS                         "channels"
#define STR_CHANNEL                          "channel"
#define STR_LOWER_FREQUENCY                  "lower-frequency"
#define STR_UPPER_FREQUENCY                  "upper-frequency"
#define STR_PSD                              "psd"
#define STR_POWER                            "power"
#define STR_CHANNEL_NO                       "channel-no"
#define STR_CHANNEL_MONITOR                  "channel-monitor"
#define STR_MONITOR_PORT                     "monitor-port"

#define STR_NAME                             "name"
#define STR_CONFIG                           "config"

#define STR_KEY_CHANNEL_MONITOR              "channel-monitor="
#define STR_KEY_CHANNEL                      "channel="
#define URL_PATH_OCM_CHANNEL_MONITOR         "/data/channel-monitors/channel-monitor=%s"
#define URL_PATH_OCM_CHANNELS                "/data/channel-monitors/channel-monitor=%s/channels"
#define URL_PATH_OCM_CHANNEL                 "/data/channel-monitors/channel-monitor=%s/channels/channel=%s,%s"
#define URL_PATH_OCM_CHANNEL_STATE           "/data/channel-monitors/channel-monitor=%s/channels/channel=%s,%s/state"
#define URL_PATH_OCM_CONFIG                  "/data/channel-monitors/channel-monitor=%s/config"
#define URL_PATH_OCM_STATE                   "/data/channel-monitors/channel-monitor=%s/state"

#define STR_PROTOCOL                         "protocol"
#define STR_RULES                            "rules"
#define STR_SOURCE_IP                        "source-ip"
#define STR_TARGET                           "target"
#define STR_RULES_IPV6                       "rules-ipv6"

#define STR_REFLECTION                       "reflection"
#define STR_CONNECTIVITY                     "connectivity"
#define STR_CLIENT_W                                "client-w"
#define STR_CLIENT_P                                     "client-p"
#define STR_MUXT_DIFFER_HIGH_THR             "mux-t-differ-high-thr"
#define STR_MUXT_DIFFER_LOW_THR              "mux-t-differ-low-thr"
#define STR_MUXR_DIFFER_HIGH_THR             "mux-r-differ-high-thr"
#define STR_MUXR_DIFFER_LOW_THR              "mux-r-differ-low-thr"
#define STR_DEMUXT_DIFFER_HIGH_THR           "demux-t-differ-high-thr"
#define STR_DEMUXT_DIFFER_LOW_THR            "demux-t-differ-low-thr"

#define STR_MUXT_OUTPUT_POWER                "mux-t-output-power"
#define STR_MUXT_REFLECT_POWER               "mux-t-reflect-power"
#define STR_MUXT_DIFFER_POWER                "mux-t-differ-power"
#define STR_MUXR_OUTPUT_POWER                "mux-r-output-power"
#define STR_MUXR_INPUT_POWER                 "mux-r-input-power"
#define STR_MUXR_DIFFER_POWER                "mux-r-differ-power"
#define STR_DEMUXT_OUTPUT_POWER              "demux-t-output-power"
#define STR_DEMUXT_INPUT_POWER               "demux-t-input-power"
#define STR_DEMUXT_DIFFER_POWER              "demux-t-differ-power"

#define STR_CLIENTR_OUTPUT_POWER                         "client-r-output-power"
#define STR_CLIENTR_INPUT_POWER                          "client-r-input-power"
#define STR_CLIENTR_DIFFER_POWER                         "client-r-differ-power"
#define STR_CLIENTT_OUTPUT_POWER                         "client-t-output-power"
#define STR_CLIENTT_INPUT_POWER                          "client-t-input-power"
#define STR_CLIENTT_DIFFER_POWER                         "client-t-differ-power"


#define STR_FILE                             "file"
#define STR_FILENAME                         "filename"
#define STR_MODIFIED_DATE                    "modified-date"

#define STR_PORT                             "port"
#define STR_KEY_OCM_OSA                      "osa="
#define URL_PATH_OCM_OSA                     "/data/ocm/osa=%s/raw-data"

#define PM_HISTORY_MAX_NUM                (600)

#define STR_1                             "1"
#define STR_2                             "2"

#define SPANLOSS_THRESHOLD_MIN (1.0)
#define SPNALOSS_THRESHOLD_MAX (10.0)

#define REST_PA_GAIN_MIN                  (10.50)
#define REST_PA_GAIN_MAX                  (22.50)

#define REST_PA_GAIN_TILT_MIN             (-2.00)
#define REST_PA_GAIN_TILT_MAX             (0)

#define REST_PA_TDCM_CD_MIN               (-1300)
#define REST_PA_TDCM_CD_MAX               (80)

#define REST_BA_GAIN_MIN                  (17.00)
#define REST_BA_GAIN_MAX                  (21.00)

#define REST_BA_GAIN_TILT_MIN             (-2.00)
#define REST_BA_GAIN_TILT_MAX             (0)

#define REST_BA_PADDING_VOA_MIN           (0)
#define REST_BA_PADDING_VOA_MAX           (25.50)

#define REST_PUMP_STATE_ENABLE            (1)
#define REST_PUMP_STATE_DISABLE           (0)

#define REST_EDFA_INVALID_VALUE           (-60.00)
#define REST_DISPERSION_INVALID_VALUE     (-6000)

#define REST_SLOT_OCM                   DSC_SLOT_1
#define REST_SLOT_BA                    DSC_SLOT_2
#define REST_SLOT_PA                    DSC_SLOT_3
#define REST_SLOT_SCC                   DSC_SLOT_4

#define AUTO_CFG_LAUNCH_POWER_MIN    (-10.00)
#define AUTO_CFG_LAUNCH_POWER_MAX    (5.00)
#define AUTO_CFG_NUM_OF_CARRIER_MIN    (2)
#define AUTO_CFG_NUM_OF_CARRIER_MAX    (80)

/* #define FIRST_PRIORITY_TACACS_NAME        "1"   / * TBD * / */
/* #define SECOND_PRIORITY_TACACS_NAME       "2"   / * TBD * / */
#define TACACS_KEY_MAX_LEN                     (128)

#define RESET_SYSTEM_FLAG_FILE          "/tmp/rest_reset_system_flag"

typedef enum  {
    EN_REST_OPTICAL_PATH_W = 0,     /* path W */
    EN_REST_OPTICAL_PATH_P,         /* path P */
    EN_REST_OPTICAL_PATH_MAX
} REST_OPTICAL_PATH_EN;

typedef enum {
    EN_REST_AMP_W_CARD = 0,
    EN_REST_AMP_P_CARD = 1,
    EN_REST_AMP_CARD_MAX
} REST_AMP_CARD_TYPE_EN;

typedef struct REST_TACACS_SERVER_st {
    char acName[36];
    char acIp[40];
    unsigned int uiTimeOut;
    unsigned int uiPort;
    char acKey[TACACS_KEY_MAX_LEN + 1];
} REST_TACACS_SERVER_ST;

typedef struct REST_EDFA_FLAG_st {
    bool bGainFlag;
    bool bTiltFlag;
    /* bool bTodcFlag;     / * only exist in PA * / */
    bool bVoaFlag;      /* only exist in BA */
    bool bAprEnableFlag;
    bool bStateFlag;
} REST_EDFA_FLAG_ST;

typedef struct REST_EDFA_PA_st
{
    double dGain;
    double dGainTilt;
    /* double dTcdc; */
    double dVoa;
    bool bAprEnable;
    bool bState;
    char acResv[6];
    double dActualGain;     /*state*/
} REST_EDFA_PA_ST;

typedef struct REST_EDFA_BA_st
{
    double dGain;
    double dGainTilt;
    double dVoa;
    /* int iAprEnable; */
    /* int iState; */
    bool bAprEnable;
    bool bState;
    char acResv[6];
    double dActualGain;     /*state*/
    double dActualVoa;      /*state*/
} REST_EDFA_BA_ST;

typedef struct REST_SYSTEM_FLAG_st {
    bool bNodeNameFlag;
    bool bIpv4Flag;
    bool bNetmaskFlag;
    bool bGatewayFlag;
    bool abNtpFlag[2];
    bool abSyslogFlag[2];
    bool abTacacsIpFlag[2];
    bool abTacacsKeyFlag[2];

    bool abFiberTypeFlag[2];
    bool abLaunchPwrFlag[2];
    bool abDropPwrFlag[2];
    bool abCarriersFlag[2];
    /* bool bBasedOcmCarFlag; */
    bool bTimezone;
    bool abLongSpanFlag[2];
    bool bIpv6ModeFlag;
    bool bIpv6AddrFlag;
    bool bIpv6GatewayFlag;
} REST_SYSTEM_FLAG_ST;

typedef struct REST_SYSTEM_st
{
    char acNodeName[128];
    char acIPV4[40];
    char acNetmask[40];
    char acGatewayV4[40];
    char aacNtpIp[2][40];
    char aacSyslogIp[2][40];
    REST_TACACS_SERVER_ST astTacacsServer[2];
    FIBER_TYPE_EN aenFiberType[2];
    double adLauchingPower[2];
    double adDropPower[2];
    unsigned char aucCarries[2];
    /* bool bBasedOcmCarriers; */
    short sTimezone;
    bool abLongSpan[2];
    char acIpv6Mode[16];       /* static/dhcp/link-layer */
    char acIPv6[48];           /* format: <ipv6>/<prefix> */
    char acGatewayV6[40];
} REST_SYSTEM_ST;

typedef enum {
    FAN_A_INDEX = 0,
    FAN_B_INDEX,
    FAN_C_INDEX,
    FAN_D_INDEX,
    FAN_MAX_INDEX
} REST_FAN_INDEX_EN;

typedef enum {
    POWER_A_INDEX = 0,
    POWER_B_INDEX,
    POWER_MAX_INDEX
} REST_POWER_INDEX_EN;


typedef struct PN_SN_st {
    char acSN[128];          /*SN*/
    char acPN[128];          /*PN*/
} PN_SN_ST;

typedef struct REST_INVENTORY_st
{
    char acModuleSN[128];        /*product SN*/
    char acModulePN[128];        /*protduct PN*/
    char acModuleSoftVer[32];    /*product software version*/
    char acVendor[128];
    PN_SN_ST stScc;                              /*SCC Card*/
    PN_SN_ST stOps;                              /*OPS Card*/
    PN_SN_ST stOcs;                              /*OCS Card*/
    PN_SN_ST astAmp[EN_REST_AMP_CARD_MAX];       /*AMP / AMP-PCard*/
    PN_SN_ST astPower[POWER_MAX_INDEX];
    PN_SN_ST astFAN[FAN_MAX_INDEX];
    PN_SN_ST stMux;                /* MUX */
} REST_INVENTORY_ST;

typedef struct REST_FULLCONFIG_st
{
    REST_SYSTEM_ST stSystem;
    REST_EDFA_BA_ST astEdfaBA[EN_REST_OPTICAL_PATH_MAX];
    REST_EDFA_PA_ST astEdfaPA[EN_REST_OPTICAL_PATH_MAX];
} REST_FULLCONFIG_ST;

typedef struct REST_RESTART_st
{
    char acEntity[16];
    /* int iRestart; */
}REST_RESTART_ST;

typedef struct REST_USER_LIST_st
{
    USER_ST *pstUser;
    unsigned int uiTotal;
} REST_USER_LIST_ST;

typedef struct REST_TRANSFER_FILE_st
{
    char acUserId[64];
    char acPassword[64];
    char acSrcFileName[256];
    char acDestFileName[256];
} REST_TRANSFER_FILE_ST;

typedef struct ENTITY_TYPE_KV_st {
    char *pcEntity;
    char *pcResetModule;
} ENTITY_TYPE_KV_ST;

typedef struct REST_THRESHOLD_st {
    double dHighThr;
    double dLowThr;
} REST_THRESHOLD_ST;

typedef struct REST_MUX_CFG_st {
    REST_THRESHOLD_ST stMuxtDifferThr;
    REST_THRESHOLD_ST stMuxrDifferThr;
    REST_THRESHOLD_ST stDemuxtDifferThr;
} REST_MUX_CFG_ST;

typedef struct REST_MUX_STATE_st {
    /* reflection power */
    double dMuxtOutPwr;       /* PDb */
    double dMuxtReflectPwr;   /* PDa */
    double dMuxtDifferPwr;    /* PDb - PDa */

    /* connectivity power */
    double dMuxrOutPwr;       /* PD10 */
    double dMuxrInputPwr;     /* PD9 */
    double dMuxrDifferPwr;    /* PD10 - PD9 */

    double dDemuxtOutPwr;     /* PD12 */
    double dDemuxtInputPwr;   /* PD11 */
    double dDemuxtDifferPwr;  /* PD12 - PD11 */
} REST_MUX_STATE_ST;

int REST_ParseSystem(const cJSON *pstJson, REST_SYSTEM_ST *pstSystem);
int REST_SetSystem(REST_SYSTEM_ST *pstSystem);
int REST_GetSystem(REST_SYSTEM_ST *pstSystem);

int REST_ParseFullConfig(const cJSON *pstJson, REST_FULLCONFIG_ST *pstFullConfig);
int REST_SetFullConfig(REST_FULLCONFIG_ST *pstFullConfig);
int REST_GetFullConfig(REST_FULLCONFIG_ST *pstFullConfig);

int REST_GetInventory(REST_INVENTORY_ST *pstInventory);

int REST_ParseRestart(const cJSON *pstJson, REST_RESTART_ST *pstRestart);
int REST_CommonRestart(REST_RESTART_ST *pstRestart, char *pcResetType);

int REST_ParseTransferFile(const cJSON *pstJson, REST_TRANSFER_FILE_ST *pstTransferFile);
int REST_TransferFile(REST_TRANSFER_FILE_ST *pstTransferFile);

int REST_ParseTransferLog(const cJSON *pstJson, REST_TRANSFER_FILE_ST *pstTransferFile);
int REST_TransferLog(REST_TRANSFER_FILE_ST *pstTransferFile);

int REST_GetSystemOptional(REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag);
int REST_ParseSystemOptional(const cJSON *pstJson, REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag);
int REST_SetSystemOptional(REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag);

int REST_ParseApsd(const cJSON *pstJson, bool *pbApsdEnable);

int REST_ParseMuxConf(const cJSON *pstJson, REST_MUX_CFG_ST *pstMuxCfg);
int REST_SetMuxCfg(REST_MUX_CFG_ST *pstMuxCfg);
int REST_GetMuxCfg(REST_MUX_CFG_ST *pstMuxCfg);
int REST_GetMuxState(REST_MUX_STATE_ST *pstMuxState);

#ifdef __cplusplus
}
#endif

#endif
