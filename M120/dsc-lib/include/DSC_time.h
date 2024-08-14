#ifndef _DSC_TIME_H
#define _DSC_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define NTP_SERVER_IP_MAX_LEN            (40)
#define NTPD_ON                          (1)
#define NTPD_OFF                         (0)
#define NTP_SERVER_NAME_MAX_LEN          (128)
#define NTP_SERVER_MAX_NUM               (2)


#ifdef OC_SYS_NTP
#define DSC_NTP_SERVER_STATE_MAX_LEN     (64)

/* openconfig-system */
#define MODULE_OCS                                  "openconfig-system"
#define OCS_XPATH_PREFIX                            "/" MODULE_OCS ":system"

/* ntp */
#define OCS_XPATH_NTP                               OCS_XPATH_PREFIX "/ntp"
#define OCS_XPATH_NTP_ALL                           OCS_XPATH_NTP "//*"
#define OCS_XPATH_NTP_CONFIG                        OCS_XPATH_NTP "/config"
#define OCS_XPATH_NTP_CONFIG_ALL                    OCS_XPATH_NTP "/config//*"
#define OCS_XPATH_NTP_STATE                         OCS_XPATH_NTP "/state"
#define OCS_XPATH_NTP_ENABLED_CFG                   OCS_XPATH_NTP_CONFIG "/enabled"
#define OCS_XPATH_NTP_ENABLED_STATE                 OCS_XPATH_NTP_STATE "/enabled"
#define OCS_XPATH_NTP_SOURCE_ADDR_CFG               OCS_XPATH_NTP_CONFIG "/ntp-source-address"
#define OCS_XPATH_NTP_SOURCE_ADDR_STATE             OCS_XPATH_NTP_STATE "/ntp-source-address"


#define OCS_XPATH_NTP_SERVER_SELECT_STATE           OCS_XPATH_NTP_STATE "/com-oplink-system-ext:ntp-server-selected"
#define OCS_XPATH_NTP_SERVER_SYNC_STATE             OCS_XPATH_NTP_STATE "/com-oplink-system-ext:ntp-status"


#define OCS_XPATH_NTP_SERVERS                       OCS_XPATH_NTP "/servers"
#define OCS_XPATH_NTP_SERVERS_LIST                  OCS_XPATH_NTP_SERVERS "/server"
#define OCS_XPATH_NTP_SERVER                        OCS_XPATH_NTP_SERVERS "/server[address='%s']"
/* key */
#define OCS_XPATH_NTP_SERVER_ADDRESS                OCS_XPATH_NTP_SERVER "/address"
/* config */
#define OCS_XPATH_NTP_SERVER_CONF                   OCS_XPATH_NTP_SERVER "/config"
#define OCS_XPATH_NTP_SERVER_CONF_ALL               OCS_XPATH_NTP_SERVERS_LIST "/config//*"
#define OCS_XPATH_NTP_SERVER_CONF_ADDRESS           OCS_XPATH_NTP_SERVER_CONF "/address"
#define OCS_XPATH_NTP_SERVER_CONF_PORT              OCS_XPATH_NTP_SERVER_CONF "/port"
#define OCS_XPATH_NTP_SERVER_CONF_VERSION           OCS_XPATH_NTP_SERVER_CONF "/version"
#define OCS_XPATH_NTP_SERVER_CONF_TYPE              OCS_XPATH_NTP_SERVER_CONF "/association-type"
#define OCS_XPATH_NTP_SERVER_CONF_IBURST            OCS_XPATH_NTP_SERVER_CONF "/iburst"
#define OCS_XPATH_NTP_SERVER_CONF_PREFER            OCS_XPATH_NTP_SERVER_CONF "/prefer"
/* state */
#define OCS_XPATH_NTP_SERVER_STATE                  OCS_XPATH_NTP_SERVER "/state"
#define OCS_XPATH_NTP_SERVER_STATE_ADDRESS          OCS_XPATH_NTP_SERVER_STATE "/address"
#define OCS_XPATH_NTP_SERVER_STATE_PORT             OCS_XPATH_NTP_SERVER_STATE "/port"
#define OCS_XPATH_NTP_SERVER_STATE_VERSION          OCS_XPATH_NTP_SERVER_STATE "/version"
#define OCS_XPATH_NTP_SERVER_STATE_TYPE             OCS_XPATH_NTP_SERVER_STATE "/association-type"
#define OCS_XPATH_NTP_SERVER_STATE_IBURST           OCS_XPATH_NTP_SERVER_STATE "/iburst"
#define OCS_XPATH_NTP_SERVER_STATE_PREFER           OCS_XPATH_NTP_SERVER_STATE "/prefer"
#define OCS_XPATH_NTP_SERVER_STATE_STRATUM          OCS_XPATH_NTP_SERVER_STATE "/stratum"

#define OCS_XPATH_NTP_SERVER_STATE_ROOT_DELAY       OCS_XPATH_NTP_SERVER_STATE "/root-delay"
#define OCS_XPATH_NTP_SERVER_STATE_ROOT_DISP        OCS_XPATH_NTP_SERVER_STATE "/root-dispersion"
#define OCS_XPATH_NTP_SERVER_STATE_OFFSET           OCS_XPATH_NTP_SERVER_STATE "/offset"
#define OCS_XPATH_NTP_SERVER_STATE_POLL_INTERVAL    OCS_XPATH_NTP_SERVER_STATE "/poll-interval"
#else
#define XPATH_NTP                               "/ietf-system:system/ntp"
#define XPATH_NTP_ALL                           XPATH_NTP "//*"
#define XPATH_NTP_ENABLED                       XPATH_NTP "/enabled"
#define XPATH_NTP_SERVER_LIST                   XPATH_NTP "/server"
#define XPATH_NTP_SERVER                        XPATH_NTP "/server[name='%s']"
#define XPATH_NTP_SERVER_UDP_ADDR               XPATH_NTP_SERVER "/udp/address"
#define XPATH_NTP_SERVER_UDP_PORT               XPATH_NTP_SERVER "/udp/port"
#define XPATH_NTP_SERVER_ASSOCIATION_TYPE       XPATH_NTP_SERVER "/association-type"
#define XPATH_NTP_SERVER_IBURST                 XPATH_NTP_SERVER "/iburst"
#define XPATH_NTP_SERVER_PREFER                 XPATH_NTP_SERVER "/prefer"
#define XPATH_NTP_SERVER_NAME_NODE              "name"
#define XPATH_NTP_SERVER_ADDRESS_NODE           "address"
#define XPATH_NTP_SERVER_PORT_NODE              "port"
#define XPATH_NTP_SERVER_ASSOCIATION_TYPE_NODE  "association-type"
#define XPATH_NTP_SERVER_IBURST_NODE            "iburst"
#define XPATH_NTP_SERVER_PREFER_NODE            "prefer"
#endif

#ifdef OC_SYS_NTP
#define NTP_TYPE_SERVER_STR         "SERVER"
#define NTP_TYPE_PEER_STR           "PEER"
#define NTP_TYPE_POOL_STR           "POOL"
#else
#define NTP_TYPE_SERVER_STR         "server"
#define NTP_TYPE_PEER_STR           "peer"
#define NTP_TYPE_POOL_STR           "pool"
#endif


#define XPATH_TIME_STATE                        "/ietf-system:system-state/clock"
#define XPATH_TIME_CURRENT_TIME                 XPATH_TIME_STATE "/current-datetime"
#define XPATH_TIME_BOOT_TIME                    XPATH_TIME_STATE "/boot-datetime"

#define RPC_XPATH_TIME                          "/ietf-system:set-current-datetime"
#define RPC_XPATH_TIME_DATETIME                 RPC_XPATH_TIME "/current-datetime"

#define XPATH_TIMEZONE                          "/ietf-system:system/clock"
#define XPATH_TIMEZONE_NAME                     XPATH_TIMEZONE "/timezone-name"
#define XPATH_TIMEZONE_UTC_OFFSET               XPATH_TIMEZONE "/timezone-utc-offset"




typedef enum en_ASSOCIATION_TYPE
{
    ASSOCIATION_TYPE_SERVER = 0,
    ASSOCIATION_TYPE_PEER,
    ASSOCIATION_TYPE_POOL,
} ASSOCIATION_TYPE_EN;

typedef struct st_NTP_SERVER
{
    char acName[NTP_SERVER_NAME_MAX_LEN];
    char acIp[NTP_SERVER_IP_MAX_LEN];
    ASSOCIATION_TYPE_EN enAssociationType;
    uint16_t usPort;
    bool bPrefer;
    bool bBurst;
    int32_t iVersion;
#ifdef OC_SYS_NTP
    uint32_t ulRootDelay;
    uint64_t ullRootDisp;
    uint64_t ullOffset;
    uint32_t ulPollInterval;
    uint8_t ucStratum;
#endif
} NTP_SERVER_ST;

typedef struct st_NTP_CONFIG_LIST
{
    NTP_SERVER_ST stNtpServer[NTP_SERVER_MAX_NUM];
    uint32_t ulTotal;
    bool bEnable;
#ifdef OC_SYS_NTP
    char acSourceAddr[NTP_SERVER_IP_MAX_LEN];
    char acSelectIp[NTP_SERVER_IP_MAX_LEN];
    char acNtpState[DSC_NTP_SERVER_STATE_MAX_LEN];
#endif
} NTP_SERVER_LIST_ST;


extern int DSC_SetSystemTime(char *pcTime);
extern int DSC_SetTimezoneUtcOffset(int16_t sMinutes);
extern int DSC_GetTimezoneUtcOffset(int16_t *psMinutes);
extern int DSC_GetUptime(char *pcUptime, size_t size);
extern int DSC_GetSystemTime(char *pcTime, size_t size);
extern int DSC_GetBootTime(char *pcBootTime, size_t size);
extern char *DSC_NtpTypeToString(ASSOCIATION_TYPE_EN enType, char *pcBuf);

#ifdef OC_SYS_NTP
extern int DSC_OC_GetNtpServerAddr(char *pcXpath, char *pcKey);
extern int DSC_OC_GetNtpServer(bool bOnlyConfig, NTP_SERVER_LIST_ST *pstServers);
extern int DSC_OC_AddNtpServer(NTP_SERVER_ST *pstServer);
extern int DSC_OC_DeleteNtpServer(char *pcAddr);
extern int DSC_OC_SetNtpEnable(bool bEnable);
extern int DSC_OC_SetNtpServerIP(char *pcIpAddr);
extern int DSC_OC_SetNtpServerPrefer(char *pcAddr, bool bEnable);
extern int DSC_OC_SetNtpServerIburst(char *pcAddr, bool bIburst);
extern int DSC_OC_SetNtpServerVersion(char *pcAddr, int iVersion);
#else
extern int DSC_GetNtpServerName(char *pcXpath, char *pcKey);
extern int DSC_GetNtpServer(NTP_SERVER_LIST_ST *pstServers);
extern int DSC_AddNtpServer(NTP_SERVER_ST *pstNtpServerInfo);
extern int DSC_DeleteNtpServer(char *pcServerName);
extern int DSC_SetNtpEnable(bool bEnable);
extern int DSC_SetNtpServerIP(char *pcServerName, char *pcIpAddr);
extern int DSC_SetNtpServerPrefer(char *pcServerName, bool bEnable);
#endif





#ifdef __cplusplus
}
#endif

#endif
