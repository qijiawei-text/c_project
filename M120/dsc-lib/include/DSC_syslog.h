/***MODU+******************************************************************************************
* FileName    : dsc_syslog.h
* Description : Define the syslog functions:
                org-openroadm-syslog.yang
                openconfig-system-logging.yang
* History     :
*     [Author]          [Date]          [Version]        [Description]
* [1] Chun Ye           2022/11/13      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/
#ifndef _DSC_SYSLOG_H
#define _DSC_SYSLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define SYSLOG_SERVER_NUM                          2
#define SYSLOG_LOCAL_LOG_NAME_MAX_LEN              20
#define SYSLOG_SERVER_NAME_MAX_LEN                 128
#define SYSLOG_REMOTE_IP_MAX_LEN                   40
#define SYSLOG_FACILITY_NAME_MAX_LEN               60
#define SYSLOG_FACILITY_NUM_MAX                    10
#define SYSLOG_OPERATOR_MAX_LEN                    4
#define SYSLOG_SEVERITY_MAX_LEN                    8



#define XPATH_SYSLOG_MODULE                                              "org-openroadm-syslog"
#define XPATH_SYSLOG                                                     "/org-openroadm-syslog:syslog"
#define XPATH_SYSLOG_ALL                                                 XPATH_SYSLOG "//*"
#define XPATH_SYSLOG_LOCAL_LOG_FILENAME                                  XPATH_SYSLOG "/local-syslog-filename"
#define XPATH_SYSLOG_LOG_ACTIONS                                         XPATH_SYSLOG "/log-actions"
#define XPATH_SYSLOG_REMOTE                                              XPATH_SYSLOG_LOG_ACTIONS "/remote"
#define XPATH_SYSLOG_SERVER_LIST                                         XPATH_SYSLOG_REMOTE "/destination"
#define XPATH_SYSLOG_SERVER_LIST_NAME                                    XPATH_SYSLOG_REMOTE "/destination[name='%s']"
#define XPATH_SYSLOG_SERVER_LIST_ALL                                     XPATH_SYSLOG_SERVER_LIST_NAME "//*"
#define XPATH_SYSLOG_SERVER_LIST_TCP                                     XPATH_SYSLOG_SERVER_LIST_NAME "/tcp"
#define XPATH_SYSLOG_SERVER_LIST_TCP_ADDR                                XPATH_SYSLOG_SERVER_LIST_TCP "/address"
#define XPATH_SYSLOG_SERVER_LIST_TCP_PORT                                XPATH_SYSLOG_SERVER_LIST_TCP "/port"
#define XPATH_SYSLOG_SERVER_LIST_UDP                                     XPATH_SYSLOG_SERVER_LIST_NAME "/udp"
#define XPATH_SYSLOG_SERVER_LIST_UDP_ADDR                                XPATH_SYSLOG_SERVER_LIST_UDP "/address"
#define XPATH_SYSLOG_SERVER_LIST_UDP_PORT                                XPATH_SYSLOG_SERVER_LIST_UDP "/port"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR                            XPATH_SYSLOG_SERVER_LIST_NAME "/log-selector"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_NO_FACILITIES              XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR "/no-facilities"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST            XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR "/log-facility"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_FACILITIES XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR "/log-facility[facility='%s']"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_ALL        XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_FACILITIES "/*"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_SEVERITY   XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_FACILITIES "/severity"
#define XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_OPERATOR   XPATH_SYSLOG_SERVER_LIST_LOG_SELECTOR_FACILITIES_LIST_FACILITIES "/severity-operator"

#define SYSLOG_FACILITY_TYPE_ENUM_ALL_FOR_CLI_STR     "*"
#define SYSLOG_FACILITY_TYPE_ENUM_ALL_STR             "all"
#define SYSLOG_FACILITY_TYPE_IDENTIFY_OTHER_STR       "org-openroadm-syslog:%s"



#define SYSLOG_SEVERITY_EMERGENCY_STR "emergency"
#define SYSLOG_SEVERITY_EMERG_FOR_CLI_STR "emerg"
#define SYSLOG_SEVERITY_ALERT_STR     "alert"
#define SYSLOG_SEVERITY_CRITICAL_STR  "critical"
#define SYSLOG_SEVERITY_CRIT_FOR_CLI_STR  "crit"
#define SYSLOG_SEVERITY_ERROR_STR     "error"
#define SYSLOG_SEVERITY_ERR_FOR_CLI_STR "err"
#define SYSLOG_SEVERITY_WARNING_STR   "warning"
#define SYSLOG_SEVERITY_NOTICE_STR    "notice"
#define SYSLOG_SEVERITY_INFO_STR      "info"
#define SYSLOG_SEVERITY_DEBUG_STR     "debug"
#define SYSLOG_SEVERITY_ALL_STR       "all"
#define SYSLOG_SEVERITY_ALL_FOR_CLI_STR "*"
#define SYSLOG_SEVERITY_NONE_STR      "none"




typedef enum en_FACILITY_TYPE
{
    FACILITY_NONE_TYPE = 0,
    FACILITY_NOT_NONE_TYPE
} FACILITY_TYPE_EN;

typedef enum en_SYSLOG_SEVERITY_TYPE
{
    SYSLOG_SEVERITY_EMERGENCY_TYPE = 0,
    SYSLOG_SEVERITY_ALERT_TYPE,
    SYSLOG_SEVERITY_CRITICAL_TYPE,
    SYSLOG_SEVERITY_ERROR_TYPE,
    SYSLOG_SEVERITY_WARNING_TYPE,
    SYSLOG_SEVERITY_NOTICE_TYPE,
    SYSLOG_SEVERITY_INFO_TYPE,
    SYSLOG_SEVERITY_DEBUG_TYPE,
    SYSLOG_SEVERITY_ALL_TYPE,
    SYSLOG_SEVERITY_NONE_TYPE
} SYSLOG_SEVERITY_TYPE_EN;

typedef enum en_SYSLOG_OPERATOR_TYPE
{
    EQUALS_OR_HIGHER_TYPE = 0,
    EQUALS_TYPE,
    NOT_EQUALS_TYPE
} SYSLOG_OPERATOR_TYPE_EN;


#define SYSLOG_OPERATOR_EQUALS_OR_HIGHER_STR      "equals-or-higher"
#define SYSLOG_OPERATOR_EQUALS_STR                "equals"
#define SYSLOG_OPERATOR_NOT_EQUALS_STR            "not-equals"


typedef struct st_SYSLOG_FACILITY_SUB
{
    char acFacilityName[SYSLOG_FACILITY_NAME_MAX_LEN];
    SYSLOG_SEVERITY_TYPE_EN enSeverity;
    SYSLOG_OPERATOR_TYPE_EN enOpType;
} SYSLOG_FACILITY_SUB_ST;

typedef struct st_SYSLOG_FACILITY
{
    FACILITY_TYPE_EN enFacilityType;
    SYSLOG_FACILITY_SUB_ST astFacilitySub[SYSLOG_FACILITY_NUM_MAX];
    uint32_t ulFacilityNum;
} SYSLOG_FACILITY_ST;

typedef enum en_PROTOCOL_TYPE
{
    PROTOCOL_TCP_TYPE = 0,
    PROTOCOL_UDP_TYPE,
} SYSLOG_PROTOCOL_TYPE_EN;

#define DEF_SYSLOG_PROTOCOL_TCP "TCP"
#define DEF_SYSLOG_PROTOCOL_UDP "UDP"

typedef struct st_SYSLOG_ACTION
{
    char acIp[SYSLOG_REMOTE_IP_MAX_LEN];
    uint16_t usPort;
    uint16_t usResv;
    SYSLOG_PROTOCOL_TYPE_EN enProtocolType;
} SYSLOG_ACTION_ST;

typedef struct st_SYSLOG_SERVER
{
    char acName[SYSLOG_SERVER_NAME_MAX_LEN];
    SYSLOG_ACTION_ST stAction;
    SYSLOG_FACILITY_ST stFacility;
} SYSLOG_SERVER_ST;

typedef struct st_SYSLOG_CONFIG_LIST
{
    char acLocalLogName[SYSLOG_LOCAL_LOG_NAME_MAX_LEN];
    SYSLOG_SERVER_ST astSyslogServer[SYSLOG_SERVER_NUM];
    uint32_t ulTotal;
} SYSLOG_SERVER_LIST_ST;
/* end syslog */


extern int DSC_GetSyslogServerName(char *pcXpath, char *pcKey);
extern int DSC_GetSyslogFacilityName(char *pcXpath, char *pcKey);
extern int DSC_SyslogSeverityToValue(char *pcBuf, SYSLOG_SEVERITY_TYPE_EN *penSeverity);
extern int DSC_SyslogOperatorEnumToValue(char *pcOperator, SYSLOG_OPERATOR_TYPE_EN *penOperator);
extern char *DSC_SyslogFacilityToString(SYSLOG_FACILITY_ST *pstFacility, char *pcBuf);
extern char *DSC_SyslogActionToString(SYSLOG_ACTION_ST *pstAction, char *pcBuf);
extern int DSC_SyslogFacilityParse(char *pcBuf, SYSLOG_FACILITY_ST *pstFacility);
extern int DSC_SyslogActionToValue(char *pcBuf, SYSLOG_ACTION_ST *pstAction);
extern int DSC_GetSyslogServer(SYSLOG_SERVER_LIST_ST *pstServers);
extern int DSC_AddSyslogServer(SYSLOG_SERVER_ST *pstServer);
extern int DSC_AddSyslogServerSpec(char *pcName, char *pcIpAddr);
extern int DSC_DelSyslogServer(char *pcServerName);
extern int DSC_SetSyslogIP(char *pcServerName, char *pcServerIP);
extern int DSC_AddSyslogFacility(char *pcServerName, SYSLOG_FACILITY_SUB_ST *pstFacility);
extern int DSC_DelSyslogFacility(char *pcServerName, char *pcFacilityName);
extern int DSC_SetSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN enSeverity);
extern int DSC_GetSyslogSeverity(char *pcName, char *pstFacilityName, SYSLOG_SEVERITY_TYPE_EN *penSeverity);
extern int DSC_SetSyslogOperator(char *pcName, char *pstFacilityName, SYSLOG_OPERATOR_TYPE_EN enOperator);


/**************************************************************************************************/
/* definition of openconfig-system-logging.yang syslog support command table.                     */
/**************************************************************************************************/
#define OCS_XPATH_LOGGING  OCS_XPATH_PREFIX "/logging"

#define OCS_XPATH_LOGGING_HOST_STATE                "/openconfig-system:system/logging/remote-servers"
#define OCS_XPATH_LOGGING_HOST1_STATE               "/openconfig-system:system/logging/remote-servers/remote-server[host='1']/state"
#define OCS_XPATH_LOGGING_HOST2_STATE               "/openconfig-system:system/logging/remote-servers/remote-server[host='2']/state"

#define OCS_XPATH_LOGGING_CONSOLE                                    OCS_XPATH_LOGGING "/console"
#define OCS_XPATH_LOGGING_CONSOLE_CONFIG                             OCS_XPATH_LOGGING_CONSOLE "/config"
#define OCS_XPATH_LOGGING_CONSOLE_ALL_CONFIG                         OCS_XPATH_LOGGING_CONSOLE "/config//."
#define OCS_XPATH_LOGGING_CONSOLE_STATE                              OCS_XPATH_LOGGING_CONSOLE "/state"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS                          OCS_XPATH_LOGGING_CONSOLE "/selectors"
#define LOGGING_SELECTOR                                             "/selector[facility='%s'][severity='%s']"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR                 OCS_XPATH_LOGGING_CONSOLE_SELECTORS LOGGING_SELECTOR
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_FACILITY        OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG "/facility"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_SEVERITY        OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG "/severity"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG          OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR "/config"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_ALL_CONFIG      OCS_XPATH_LOGGING_CONSOLE_SELECTORS "/selector" "/config//*"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG_FACILITY OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG "/facility"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG_SEVERITY OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_CONFIG "/severity"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_STATE           OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR "/state"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_STATE_FACILITY  OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_STATE "/facility"
#define OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_STATE_SEVERITY  OCS_XPATH_LOGGING_CONSOLE_SELECTORS_SELECTOR_STATE "/severity"

#define OCS_XPATH_LOGGING_REMOTE_SERVERS                             OCS_XPATH_LOGGING "/remote-servers"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_ALL                  OCS_XPATH_LOGGING_REMOTE_SERVERS "/remote-server//*"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_A                    OCS_XPATH_LOGGING_REMOTE_SERVERS "/remote-server"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER                      OCS_XPATH_LOGGING_REMOTE_SERVERS "/remote-server[host='%s']"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_HOST                 OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER "/remote-servers"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG               OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER "/config"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_ALL_CONFIG           OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_A "/config//*"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_HOST          OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG "/host"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_SOURCE_ADDRESS  OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG "/source-address"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_REMOTE_PORT   OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG "/remote-port"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG_PROTOCOL      OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_CONFIG "/com-oplink-system-logging:transport-protocol"

#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE                OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER "/state"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_PROTOCOL      OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:transport-protocol"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_CONN_ALIVE    OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:connection-alive"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_CON_CRT_TIME  OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:connection-creation-time"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_OUT_MSG       OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:out-messages"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_OUT_OCT       OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:out-octets"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_OUT_MSG_LST_MIN      OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:out-messages-last-minute"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_OUT_OCT_LST_MIN      OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:out-octets-last-minute"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_OUT_ERR_MSG   OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/com-oplink-system-logging:out-error-messages"

#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_HOST           OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/host"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_SOURCE_ADDRESS OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/source-address"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE_REMOTE_PORT    OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_STATE "/remote-port"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS            OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER "/selectors"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_ALL        OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS "//*"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR   OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS LOGGING_SELECTOR
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG   OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR "/config"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_FACILITY OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG "/facility"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG_SEVERITY OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_CONFIG "/severity"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_STATE           OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR "/state"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_STATE_FACILITY  OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_STATE "/facility"
#define OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_STATE_SEVERITY  OCS_XPATH_LOGGING_REMOTE_SERVERS_SERVER_SELECTORS_SELECTOR_STATE "/severity"

#define OCS_LOGGING_SEVERITY_EMERGENCY_STR     "EMERGENCY"
#define OCS_LOGGING_SEVERITY_EMERG_FOR_CLI_STR "emerg"
#define OCS_LOGGING_SEVERITY_ALERT_STR         "ALERT"
#define OCS_LOGGING_SEVERITY_CRITICAL_STR      "CRITICAL"
#define OCS_LOGGING_SEVERITY_CRIT_FOR_CLI_STR  "crit"
#define OCS_LOGGING_SEVERITY_ERROR_STR         "ERROR"
#define OCS_LOGGING_SEVERITY_ERR_FOR_CLI_STR   "err"
#define OCS_LOGGING_SEVERITY_WARNING_STR       "WARNING"
#define OCS_LOGGING_SEVERITY_NOTICE_STR        "NOTICE"
#define OCS_LOGGING_SEVERITY_INFO_STR          "INFORMATIONAL"
#define OCS_LOGGING_SEVERITY_DEBUG_STR         "DEBUG"
#define OCS_LOGGING_SEVERITY_ALL_STR           "all"
#define OCS_LOGGING_SEVERITY_ALL_FOR_CLI_STR   "*"
#define OCS_LOGGING_SEVERITY_NONE_STR          "none"

#define TRANSPORT_PROTOCOL_UDP                 "UDP"
#define TRANSPORT_PROTOCOL_TCP                 "TCP"

#define OC_RPC_XPATH_UPLOAD_SYSLOG             "/com-oplink-system-logging:upload-syslog"
#define OC_RPC_XPATH_UPLOAD_SYSLOG_HOSTNAME    OC_RPC_XPATH_UPLOAD_SYSLOG "/sftp-server/hostname"
#define OC_RPC_XPATH_UPLOAD_SYSLOG_PORT        OC_RPC_XPATH_UPLOAD_SYSLOG "/sftp-server/port"
#define OC_RPC_XPATH_UPLOAD_SYSLOG_USERNAME    OC_RPC_XPATH_UPLOAD_SYSLOG "/sftp-server/username"
#define OC_RPC_XPATH_UPLOAD_SYSLOG_PASSWORD    OC_RPC_XPATH_UPLOAD_SYSLOG "/sftp-server/password"
#define OC_RPC_XPATH_UPLOAD_SYSLOG_MESSAGE     OC_RPC_XPATH_UPLOAD_SYSLOG "/message"

typedef struct {
    char transport_protocol[64];
    bool connection_alive;
    char connection_creation_time[64];
    uint64_t out_messages;
    uint64_t out_octets;
    uint64_t out_messages_last_minute;
    uint64_t out_octets_last_minute;
    uint64_t out_error_messages;
} ST_OCS_LOGGING_EXT;

extern int DSC_GetOcSyslogServer(SYSLOG_SERVER_LIST_ST *pstServers);
extern int DSC_GetOcSyslogServerConfig(SYSLOG_SERVER_LIST_ST *pstServers);
extern int DSC_OcSyslogGetFacilityName(char *pcNameIn, char *pcNameOut);
extern int DSC_OcSyslogSeverityToValue(char *pcBuf, SYSLOG_SEVERITY_TYPE_EN *penSeverity);
extern int DSC_AddOcSyslogServerSpec(char *pcName, char *pcProtol, char *pcIpAddr, uint16_t usPort);
extern int DSC_AddOcSyslogFacility(char *pcServerName, SYSLOG_FACILITY_SUB_ST *pstFacility);
extern int DSC_DelOcSyslogFacility(char *pcServerName, char *pcFacilityName, char *pcSeverityName);
extern int DSC_DelOcSyslogServer(char *pcServerName);
extern int DSC_SetOcSyslogIP(char *pcServerName, char *pcServerIP);
extern char *UTIL_LowerStr(char *pcStr);
extern int DSC_UploadSyslogRpc(char *hostname, uint16_t port, char *username, char *password);
extern int DSC_OcRpcUploadSyslogCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt);
extern int DSC_GetOcsRemoteSrvHostKey(char *pcXpath, char *pcKey);
extern int DSC_GetOcsSelectorFacilityKey(char *pcXpath, char *pcKey);
extern int DSC_GetOcsSelectorSeverityKey(char *pcXpath, char *pcKey);

#define DEF_NOTIFY_XPATH_LOG_LEVEL           "/com-oplink-system-logging:log-level-notifi"
#define DEF_NOTIFY_LEAF_LOG_LEVEL_NAME       DEF_NOTIFY_XPATH_LOG_LEVEL "/name"
#define DEF_NOTIFY_LEAF_LOG_LEVEL_SEVERITY   DEF_NOTIFY_XPATH_LOG_LEVEL "/severity"
extern int DSC_SubLogLevel(sr_session_ctx_t *pstSess);
extern int DSC_SetLogLevel(char *pcName, char *pcLogLevel);

#ifdef __cplusplus
}
#endif

#endif
