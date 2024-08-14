#ifndef _DSC_OCSYSTEM_H
#define _DSC_OCSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif
#include "DSC_alarm.h"
#include "DSC_tca.h"

/* openconfig-system */
#define MODULE_OCS                        "openconfig-system"
#define OCS_XPATH_PREFIX                  "/" MODULE_OCS ":system"

/* config */
#define OCS_XPATH_CONFIG                   OCS_XPATH_PREFIX "/config"
#define OCS_XPATH_CONFIG_ALL               OCS_XPATH_CONFIG "//*"

#define LEAF_HOSTNAME                      "/hostname"
#define LEAF_DOMAIN_NAME                   "/domain-name"
#define LEAF_LOGIN_BANNER                  "/login-banner"
#define LEAF_MOTD_BANNER                   "/motd-banner"
#define LEAF_CURRENT_DATETIME              "/current-datetime"
#define LEAF_BOOT_TIME                     "/boot-time"

#define OCS_XPATH_CFG_HOSTNAME             OCS_XPATH_CONFIG LEAF_HOSTNAME
#define OCS_XPATH_CFG_DOMAIN_NAME          OCS_XPATH_CONFIG LEAF_DOMAIN_NAME
#define OCS_XPATH_CFG_LOGIN_BANNER         OCS_XPATH_CONFIG LEAF_LOGIN_BANNER
#define OCS_XPATH_CFG_MOTD_BANNER          OCS_XPATH_CONFIG LEAF_MOTD_BANNER

/* state */
#define OCS_XPATH_STATE                    OCS_XPATH_PREFIX "/state"
#define OCS_XPATH_STATE_ALL                OCS_XPATH_STATE "//*"
#define OCS_XPATH_STATE_HOSTNAME           OCS_XPATH_STATE LEAF_HOSTNAME
#define OCS_XPATH_STATE_DOMAIN_NAME        OCS_XPATH_STATE LEAF_DOMAIN_NAME
#define OCS_XPATH_STATE_LOGIN_BANNER       OCS_XPATH_STATE LEAF_LOGIN_BANNER
#define OCS_XPATH_STATE_MOTD_BANNER        OCS_XPATH_STATE LEAF_MOTD_BANNER
#define OCS_XPATH_STATE_CURRENT_DATETIME   OCS_XPATH_STATE LEAF_CURRENT_DATETIME
#define OCS_XPATH_STATE_BOOT_TIME          OCS_XPATH_STATE LEAF_BOOT_TIME

/* clock */
#define OCS_XPATH_CLOCK                    OCS_XPATH_PREFIX "/clock"
#define OCS_XPATH_CLOCK_CONFIG             OCS_XPATH_CLOCK "/config"
#define OCS_XPATH_CLOCK_CONFIG_ALL                 OCS_XPATH_CLOCK_CONFIG "//*"
#define LEAF_TIMEZONE_NAME                 "/timezone-name"
#define OCS_XPATH_CLOCK_CFG_TIMEZONE_NAME  OCS_XPATH_CLOCK_CONFIG LEAF_TIMEZONE_NAME

#define OCS_XPATH_CLOCK_STATE              OCS_XPATH_CLOCK "/state"
#define OCS_XPATH_CLOCK_STATE_TIMEZONE_NAME  OCS_XPATH_CLOCK_STATE LEAF_TIMEZONE_NAME

#define OCS_XPATH_DNS                      OCS_XPATH_PREFIX "/dns"
#define OCS_XPATH_SNMP                     OCS_XPATH_PREFIX "/snmp"
/* #define OCS_XPATH_NTP                      OCS_XPATH_PREFIX "/ntp" */
#define OCS_XPATH_GRPC_SERV                OCS_XPATH_PREFIX "/grpc-server"
#define OCS_XPATH_SSH_SERV                 OCS_XPATH_PREFIX "/ssh-server"
#define OCS_XPATH_TELNET_SERV              OCS_XPATH_PREFIX "/telnet-server"
#define OCS_XPATH_AAA                      OCS_XPATH_PREFIX "/aaa"
#define OCS_XPATH_MEMORY                   OCS_XPATH_PREFIX "/memory"
#define OCS_XPATH_CPUS                     OCS_XPATH_PREFIX "/cpus"
#define OCS_XPATH_PROCESS                  OCS_XPATH_PREFIX "/processes"

/* alarm */
#define OCS_XPATH_ALARM                    OCS_XPATH_PREFIX  "/alarms"
#define OCS_XPATH_ALARMLIST                OCS_XPATH_ALARM "/alarm//*"
#define ALARM_LEAF_ID                      "/state/id"
#define ALARM_LEAF_RESOURCE                "/resource"
#define ALARM_LEAF_TEXT                    "/text"
#define ALARM_LEAF_TIMECREATED             "/time-created"
#define ALARM_LEAF_SEVERITY                "/severity"
#define ALARM_LEAF_TYPEID                  "/type-id"

/* #define ALARM_XPATH_RESOURCE_CP_NAME     "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']/resource/resource/circuit-pack-name" */
#define OCS_XPATH_ALARM_LEAF_ID            "/openconfig-system:system/alarms/alarm[id='%s']/state/id"
#define OCS_XPATH_ALARM_LEAF_RESOURCE      "/openconfig-system:system/alarms/alarm[id='%s']/state/resource"
#define OCS_XPATH_ALARM_LEAF_TEXT          "/openconfig-system:system/alarms/alarm[id='%s']/state/text"
#define OCS_XPATH_ALARM_LEAF_TIMECREATED   "/openconfig-system:system/alarms/alarm[id='%s']/state/time-created"
#define OCS_XPATH_ALARM_LEAF_SEVERITY      "/openconfig-system:system/alarms/alarm[id='%s']/state/severity"
#define OCS_XPATH_ALARM_LEAF_TYPEID        "/openconfig-system:system/alarms/alarm[id='%s']/state/type-id"

#define OPENCONFIG_ALARM_TYPE_ID_AIS       "AIS"
#define OPENCONFIG_ALARM_TYPE_ID_EQPT      "EQPT"
#define OPENCONFIG_ALARM_TYPE_ID_LOS       "LOS"
#define OPENCONFIG_ALARM_TYPE_ID_OTS       "OTS"

/* rpc */
#define OC_RPC_XPATH_RESTART               "/openconfig-system:reboot"
#define OC_RPC_XPATH_RESTART_OPTION        OC_RPC_XPATH_RESTART "/method"
#define OC_RPC_XPATH_RESTART_MESSAGE_REQ   OC_RPC_XPATH_RESTART "/message"       /* Informational reason for the reboot. */
#define OC_RPC_XPATH_RESTART_EXTENSION     OC_RPC_XPATH_RESTART "/entity-name"   /* name of the entity to reboot */
#define OC_RPC_XPATH_RESTART_MESSAGE_RES   OC_RPC_XPATH_RESTART "/message"       /* Informational response for the reboot.; */

#define OC_UNKNOWN_RESTART_STR             "UNKNOWN"
#define OC_COLD_RESTART_STR                "COLD"
#define OC_POWERDOWN_RESTART_STR           "POWERDOWN"
#define OC_HALT_RESTART_STR                "HALT"
#define OC_WARM_RESTART_STR                "WARM"
#define OC_NSF_RESTART_STR                 "NSF"
#define OC_RESET_RESTART_STR               "RESET"
#define OC_POWERUP_RESTART_STR             "POWERUP"

#define OCS_STATE_ATTR_COUNT               (6)
#define OCS_HOSTNAME_MAX_LEN               (253)
#define OCS_TIMEZONENAME_MAX_LEN           (64)
#define OCS_ALARM_ATTR_COUNT               (6)

/*OPENCONFIG_ALARM_SEVERITY */
#define  OC_ALM_SEVERITY_UNKNOWN           "UNKNOWN"
#define  OC_ALM_SEVERITY_MINOR             "MINOR"
#define  OC_ALM_SEVERITY_WARNING           "WARNING"
#define  OC_ALM_SEVERITY_MAJOR             "MAJOR"
#define  OC_ALM_SEVERITY_CRITICAL          "CRITICAL"
/* OPENCONFIG_ALARM_TYPE_ID */
#define  OC_ALM_TYPE_ID_AIS                "AIS"     /* Defines an alarm indication signal type of alarm */
#define  OC_ALM_TYPE_ID_EQPT               "EQPT"    /* Defines an equipment related type of alarm that is specific to the physical hardware */
#define  OC_ALM_TYPE_ID_LOS                "LOS"     /* Defines a loss of signal type of alarm */
#define  OC_ALM_TYPE_ID_OTS                "OTS"     /* Defines a optical transport signal type of alarm */

/* cpus  ,add after Meiwu Bundle 0.02.0001*/
#define OCS_XPATH_CPUS_INDEX_INDEX         OCS_XPATH_CPUS  "/cpu[index='%s']/index"
#define OCS_XPATH_CPUS_INDEX               OCS_XPATH_CPUS  "/cpu[index='%s']/state"
#define OCS_XPATH_CPUS_STATE_INDEX         OCS_XPATH_CPUS  "/cpu[index='%s']/state/index"
#define OCS_XPATH_CPUS_INDEX_TOTAL         OCS_XPATH_CPUS_INDEX "/total"
#define OCS_XPATH_CPUS_INDEX_USER          OCS_XPATH_CPUS_INDEX "/user"
#define OCS_XPATH_CPUS_INDEX_KERNEL        OCS_XPATH_CPUS_INDEX "/kernel"
#define OCS_XPATH_CPUS_INDEX_NICE          OCS_XPATH_CPUS_INDEX "/nice"
#define OCS_XPATH_CPUS_INDEX_IDLE          OCS_XPATH_CPUS_INDEX "/idle"
#define OCS_XPATH_CPUS_INDEX_WAIT          OCS_XPATH_CPUS_INDEX "/wait"
#define OCS_XPATH_CPUS_INDEX_HI            OCS_XPATH_CPUS_INDEX "/hardware-interrupt"
#define OCS_XPATH_CPUS_INDEX_SI            OCS_XPATH_CPUS_INDEX "/software-interrupt"
#define LEAF_INSTANT                        "/instant"
#define LEAF_AVG                            "/avg"
#define LEAF_MIN                            "/min"
#define LEAF_MAX                            "/max"
#define LEAF_INTERVAL                       "/interval"
#define LEAF_MIN_TIME                       "/min-time"
#define LEAF_MAX_TIME                       "/max-time"

#define OCS_XPATH_CPUS_INDEX_TOTAL_INS      OCS_XPATH_CPUS_INDEX_TOTAL LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_TOTAL_AVG      OCS_XPATH_CPUS_INDEX_TOTAL LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_TOTAL_MIN      OCS_XPATH_CPUS_INDEX_TOTAL LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_TOTAL_MAX      OCS_XPATH_CPUS_INDEX_TOTAL LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_TOTAL_INTERVAL OCS_XPATH_CPUS_INDEX_TOTAL LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_TOTAL_MINTIME  OCS_XPATH_CPUS_INDEX_TOTAL LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_TOTAL_MAXTIME  OCS_XPATH_CPUS_INDEX_TOTAL LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_USER_INS       OCS_XPATH_CPUS_INDEX_USER LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_USER_AVG       OCS_XPATH_CPUS_INDEX_USER LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_USER_MIN       OCS_XPATH_CPUS_INDEX_USER LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_USER_MAX       OCS_XPATH_CPUS_INDEX_USER LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_USER_INTERVAL  OCS_XPATH_CPUS_INDEX_USER LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_USER_MINTIME   OCS_XPATH_CPUS_INDEX_USER LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_USER_MAXTIME   OCS_XPATH_CPUS_INDEX_USER LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_KERNEL_INS      OCS_XPATH_CPUS_INDEX_KERNEL LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_KERNEL_AVG      OCS_XPATH_CPUS_INDEX_KERNEL LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_KERNEL_MIN      OCS_XPATH_CPUS_INDEX_KERNEL LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_KERNEL_MAX      OCS_XPATH_CPUS_INDEX_KERNEL LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_KERNEL_INTERVAL OCS_XPATH_CPUS_INDEX_KERNEL LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_KERNEL_MINTIME  OCS_XPATH_CPUS_INDEX_KERNEL LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_KERNEL_MAXTIME  OCS_XPATH_CPUS_INDEX_KERNEL LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_NICE_INS       OCS_XPATH_CPUS_INDEX_NICE LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_NICE_AVG       OCS_XPATH_CPUS_INDEX_NICE LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_NICE_MIN       OCS_XPATH_CPUS_INDEX_NICE LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_NICE_MAX       OCS_XPATH_CPUS_INDEX_NICE LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_NICE_INTERVAL  OCS_XPATH_CPUS_INDEX_NICE LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_NICE_MINTIME   OCS_XPATH_CPUS_INDEX_NICE LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_NICE_MAXTIME   OCS_XPATH_CPUS_INDEX_NICE LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_IDLE_INS       OCS_XPATH_CPUS_INDEX_IDLE LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_IDLE_AVG       OCS_XPATH_CPUS_INDEX_IDLE LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_IDLE_MIN       OCS_XPATH_CPUS_INDEX_IDLE LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_IDLE_MAX       OCS_XPATH_CPUS_INDEX_IDLE LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_IDLE_INTERVAL  OCS_XPATH_CPUS_INDEX_IDLE LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_IDLE_MINTIME   OCS_XPATH_CPUS_INDEX_IDLE LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_IDLE_MAXTIME   OCS_XPATH_CPUS_INDEX_IDLE LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_WAIT_INS       OCS_XPATH_CPUS_INDEX_WAIT LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_WAIT_AVG       OCS_XPATH_CPUS_INDEX_WAIT LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_WAIT_MIN       OCS_XPATH_CPUS_INDEX_WAIT LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_WAIT_MAX       OCS_XPATH_CPUS_INDEX_WAIT LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_WAIT_INTERVAL  OCS_XPATH_CPUS_INDEX_WAIT LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_WAIT_MINTIME   OCS_XPATH_CPUS_INDEX_WAIT LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_WAIT_MAXTIME   OCS_XPATH_CPUS_INDEX_WAIT LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_HI_INS         OCS_XPATH_CPUS_INDEX_HI LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_HI_AVG         OCS_XPATH_CPUS_INDEX_HI LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_HI_MIN         OCS_XPATH_CPUS_INDEX_HI LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_HI_MAX         OCS_XPATH_CPUS_INDEX_HI LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_HI_INTERVAL    OCS_XPATH_CPUS_INDEX_HI LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_HI_MINTIME     OCS_XPATH_CPUS_INDEX_HI LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_HI_MAXTIME     OCS_XPATH_CPUS_INDEX_HI LEAF_MAX_TIME

#define OCS_XPATH_CPUS_INDEX_SI_INS         OCS_XPATH_CPUS_INDEX_SI LEAF_INSTANT
#define OCS_XPATH_CPUS_INDEX_SI_AVG         OCS_XPATH_CPUS_INDEX_SI LEAF_AVG
#define OCS_XPATH_CPUS_INDEX_SI_MIN         OCS_XPATH_CPUS_INDEX_SI LEAF_MIN
#define OCS_XPATH_CPUS_INDEX_SI_MAX         OCS_XPATH_CPUS_INDEX_SI LEAF_MAX
#define OCS_XPATH_CPUS_INDEX_SI_INTERVAL    OCS_XPATH_CPUS_INDEX_SI LEAF_INTERVAL
#define OCS_XPATH_CPUS_INDEX_SI_MINTIME     OCS_XPATH_CPUS_INDEX_SI LEAF_MIN_TIME
#define OCS_XPATH_CPUS_INDEX_SI_MAXTIME     OCS_XPATH_CPUS_INDEX_SI LEAF_MAX_TIME

#define PMS_XPATH_PM_DATA                                       "/openconfig-transport-pms:pm-data"

#define USER_LOG_RESOURCE_FMD                "FMD"

#define USER_LOG_TYPE_ID_SET_TIMEZONE        "SET_TIMEZONE"
#define USER_LOG_TYPE_ID_SET_HOSTNAME        "SET_HOSTNAME"
#define USER_LOG_TYPE_ID_SET_SYSLOG_SERVER   "SET_SYSLOG_SERVER"
#define USER_LOG_TYPE_ID_DEL_SYSLOG_SERVER   "DEL_SYSLOG_SERVER"
#define USER_LOG_TYPE_ID_UPLOAD_SYSLOG       "UPLOAD_SYSLOG"
#define USER_LOG_TYPE_ID_REBOOT              "REBOOT"
#define USER_LOG_TYPE_ID_SET_TCA_FGA1_INPUT  "SET_TCA_FGA1_INPUT"
#define USER_LOG_TYPE_ID_SET_TCA_FGA2_INPUT  "SET_TCA_FGA2_INPUT"
#define USER_LOG_TYPE_ID_SET_TCA_TEMP_HIGH_ALM  "SET_TCA_TEMP_HI_ALM"
#define USER_LOG_TYPE_ID_SET_TCA_TEMP_HIGH_WARN "SET_TCA_TEMP_HI_WARN"
#define USER_LOG_TYPE_ID_SET_TCA_TEMP_LOW_ALM   "SET_TCA_TEMP_LOW_ALM"
#define USER_LOG_TYPE_ID_SET_TCA_TEMP_LOW_WARN  "SET_TCA_TEMP_LOW_WARN"
#define USER_LOG_TYPE_ID_RPC_GET_PM          "RPC_GET_PM"
#define USER_LOG_TYPE_ID_RPC_CLEAR_PM_DATA   "CLEAR_PM_DATA"
#define USER_LOG_TYPE_ID_SET_LAMPTEST        "SET_LAMPTEST"

/* telemetry begin */
#define USER_LOG_TYPE_ID_TELE_SET_SUBS              "SET_TMY_SUBS_GRP"
#define USER_LOG_TYPE_ID_TELE_SET_ADDR_GROUP        "SET_TMY_ADDR_GRP"
#define USER_LOG_TYPE_ID_TELE_SET_SENSOR_GROUP      "SET_TMY_SENS_GRP"
#define USER_LOG_TYPE_ID_TELE_SET_IP_PORT           "SET_TMY_IP_PORT"
#define USER_LOG_TYPE_ID_TELE_SET_PRIORITY          "SET_TMY_PRIORITY"
#define USER_LOG_TYPE_ID_TELE_SET_PATH              "SET_TMY_PATH"
#define USER_LOG_TYPE_ID_TELE_SET_SAMPLE            "SET_TMY_SAMPLE_INTERVAL"
#define USER_LOG_TYPE_ID_TELE_SET_HEARTBEAT         "SET_TMY_HEARTBEAT"
#define USER_LOG_TYPE_ID_TELE_SET_ADDR_TO_SUBS      "SET_TMY_ADDR_GRP_TO_SUBS"
#define USER_LOG_TYPE_ID_TELE_SET_SENSOR_TO_SUBS    "SET_TMY_SENS_GRP_TO_SUBS"
#define USER_LOG_TYPE_ID_TELE_SET_MODE_TO_SUBS      "SET_TMY_MODE_TO_SUBS"

#define USER_LOG_TYPE_ID_TELE_DEL_SUBS              "DEL_TMY_SUBS_GRP"
#define USER_LOG_TYPE_ID_TELE_DEL_ADDR_GROUP        "DEL_TMY_ADDR_GRP"
#define USER_LOG_TYPE_ID_TELE_DEL_SENSOR_GROUP      "DEL_TMY_SENS_GRP"
#define USER_LOG_TYPE_ID_TELE_DEL_PATH              "DEL_TMY_PATH"
#define USER_LOG_TYPE_ID_TELE_DEL_IP_FROM_GROUP     "DEL_TMY_IP_PORT"
#define USER_LOG_TYPE_ID_TELE_DEL_PRIORITY          "DEL_TMY_PRIORITY"
#define USER_LOG_TYPE_ID_TELE_DEL_SENSOR_FROM_SUBS  "DEL_TMY_SENS_GRP_FROM_SUBS"
#define USER_LOG_TYPE_ID_TELE_DEL_ADDR_FROM_SUBS    "DEL_TMY_ADDR_GRP_FROM_SUBS"
#define USER_LOG_TYPE_ID_TELE_DEL_HEARTBEAT         "DEL_TMY_HEARTBEAT"

/* telemetry end */



/* openconfig-system*/
typedef struct st_OCSYSTEM_CONFIG
{
    char acHostName[256];
    char acDomainName[256];
    char acLoginBanner[256];
    char acMotdBanner[256];
} OCSYSTEM_CONFIG_ST;

typedef struct st_OCSYSTEM_STATE
{
    char acHostName[256];
    char acDomainName[256];
    char acLoginBanner[256];
    char acMotdBanner[256];
    char acCurrentDatetime[64]; /* YYYY-MM-DDTHH:MM:SSZ+-hh:mm */
    uint64_t ullBootTime;
} OCSYSTEM_STATE_ST;


typedef struct st_OCSYSTEM_ALARM_INFO
{
    uint64_t ullSeqNo;
    char acAlarmId[128];
    char acResource[128];
    char acText[1024];
    uint64_t ullTimeCreated;
    char acRaiseTime[64];
    char acSeverity[128];
    char acTypeId[128];
} OCSYSTEM_ALARM_INFO_ST;


/* for openroadm-tca Wss&Ops Port threshold setting */
#define PORT_THR_IN         "In"
#define PORT_THR_OUT        "Out"
#define OPS_PORT_01_NUM     (65)
#define OPS_PORT_02_NUM     (66)

#if 0
#define PORT_THR_TYPE_LOS   "Los"
#define PORT_THR_TYPE_LOW   "Low"
#define PORT_THR_TYPE_HIGH  "High"
#else
#define PORT_THR_TYPE_LOS   "LOS"
#define PORT_THR_TYPE_LOW   "LOW"
#define PORT_THR_TYPE_HIGH  "HIGH"
#endif

typedef enum WSS_PORT_THR_TYPE_en
{
    TYPE_LOS = 0,
    TYPE_LOW,
    TYPE_HIGH
}WSS_PORT_THR_TYPE_EN;

typedef struct
{
    double dInLowThr;
    double dInHighThr;
} WSS_PORT_THR_CONFIG_ST;

enum
{
    OPS_In65 = 0,
    OPS_In66,
    OPS_Out65,
    OPS_Out66
};

#define SPEED_COEFFICIENT 220  /* 240 */
#define SPEED_MAX_RPM  22000   /* 4800 */

/* for avg/min/max/min_time/max_time */
#define PM_ITEM_COUNT  81

#define STATISTICS_INTERVAL (900000000000)  /* 15min in nanosecond unit */

#define NANOSECOND (1000000000)

/* openconfig-system */
extern int DSC_GetOcSystemConfig(OCSYSTEM_CONFIG_ST *pstConfig);
extern int DSC_GetOcSystemState(OCSYSTEM_STATE_ST *pstState);
extern int DSC_SetOcSystemHostname(char *pcHostName);
extern int DSC_GetOcTimezoneNameConfig(char *pcTimezone, size_t size);
extern int DSC_GetOcTimezoneNameState(char *pcTimezone, size_t size);
extern int DSC_SetOcTimezoneName(char *pcTimezoneName);
extern int DSC_OcRpcCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt);
extern int DSC_OcGetAlarmList_I(ALARM_INFO_ST **ppstAlarm, size_t *count);
extern int DSC_OcGetAlarmList(OCSYSTEM_ALARM_INFO_ST **ppstAlarm, size_t *count);
extern int DSC_ConvertToOcAlarm(ALARM_INFO_ST *pstAlarm, OCSYSTEM_ALARM_INFO_ST *pstOcAlarm);

/* extern int TIME_GetSystemTimeString(char *pcTimeString, int lLen); */
/* extern int TIME_GetBootTime(uint64_t *pulTime); */

extern int DSC_GetWssPortThrNumAndType(char *pcId, uint32_t *iPortNum, uint32_t *iThrType);
extern int DSC_GetWssPortThrConfig(EN_TCA_IDX enIdx, WSS_PORT_THR_CONFIG_ST *pstConfig);
extern int DSC_SetWssPortThr(char *pcId, double dHighThr, double dLowThr);
extern int DSC_SetTcaPortThreshold(char *pcTcaName, double dThr);
extern int DSC_SetTcaPortHysteresis(char *pcTcaName, double dHys);
extern int DSC_Clear_Pms(char *pcPmBinType, char *pcEntityName, char *pcPmTimePeriod, char *pcPmpType, char *pcPmParameter, char *pcValidity);
extern int DSC_SetTcaPortThresholdAndHysteresis(char *pcTcaName, double dThr, double dHys);

/* link discovery for meiwu_pro */
#define RX_STATUS_SUCCESS "Success"
#define RX_STATUS_TIMEOUT "Timeout"
#define RX_STATUS_ERROR   "Error"

typedef struct st_LINK_DISCOVERY_RPC_RESP
{
    int iIndex;
    char acRxStatus[32];
    char acMessage[256];
    char acRemoteChassisName[256];
    char acRemotePortName[32];
    char acConnectionLoss[32];
} ST_LINK_DISCOVERY_RPC_RESP;

extern int DSC_LinkDiscoveyRpc(int iIndex, char *pcTxPort, char *pcRxPort, int iHoldOnTime, ST_LINK_DISCOVERY_RPC_RESP *stLinkDiscoveryRpcResp);

#ifdef __cplusplus
}
#endif

#endif

