#ifndef _DSC_DNS_H
#define _DSC_DNS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"



#define DNS_NAME_MAX_LEN                    36
#define DNS_IP_MAX_LEN                      40
#define DNS_SERACH_DOMAIN_MAX_LEN           64
#define DNS_SERACH_DOMAIN_NUM               2
#define DNS_SERVER_NUM                      2

#define XPATH_DNS                               "/ietf-system:system/dns-resolver"
#define XPATH_DNS_ALL                           XPATH_DNS "//*"
#define XPATH_DNS_SERVER_LIST                   XPATH_DNS "/server"
#define XPATH_DNS_SERVER_LIST_NAME              XPATH_DNS_SERVER_LIST "/name"
#define XPATH_DNS_SERVER_LIST_UDPANDTCP         XPATH_DNS_SERVER_LIST "/udp-and-tcp"
#define XPATH_DNS_SERVER_LIST_UDPANDTCP_ADDR    XPATH_DNS_SERVER_LIST_UDPANDTCP "/address"
#define XPATH_DNS_SERVER_LIST_UDPANDTCP_PORT    XPATH_DNS_SERVER_LIST_UDPANDTCP "/port"
#define XPATH_DNS_OPTIONS                       XPATH_DNS "/options"
#define XPATH_DNS_OPTIONS_TIMEOUT               XPATH_DNS_OPTIONS "/timeout"
#define XPATH_DNS_OPTIONS_ATTEMPTS              XPATH_DNS_OPTIONS "/attempts"
#define XPATH_DNS_SERVER                        XPATH_DNS "/server[name='%s']"
#define XPATH_DNS_SERVER_UDPANDTCP_ADDR         XPATH_DNS_SERVER "/udp-and-tcp/address"
#define XPATH_DNS_SERVER_UDPANDTCP_PORT         XPATH_DNS_SERVER "/udp-and-tcp/port"
#define XPATH_DNS_SEARCH                        XPATH_DNS "/search"
#define XPATH_DNS_SEARCH_ENTRY                  XPATH_DNS "/search[.='%s']"

typedef struct
{
    char acName[DNS_NAME_MAX_LEN];
    char acIp[DNS_IP_MAX_LEN];
    uint16_t usPort;
    uint16_t usResv;
} DNS_SERVER_ST;

typedef struct DNS_CONFIG_LIST_st
{
    DNS_SERVER_ST stDnsServer[DNS_SERVER_NUM];
    uint32_t ulTotal;
    char stDnsSearch[DNS_SERACH_DOMAIN_NUM][DNS_SERACH_DOMAIN_MAX_LEN];
    uint32_t ulTimeout;
    uint32_t ulRetryTimes;
} DNS_SERVER_LIST_ST;

extern int DSC_GetDnsServerName(char *pcXpath, char *pcKey);
extern int DSC_AddDnsServer(DNS_SERVER_ST *pstServer);
extern int DSC_DelDnsServer(char *pcServerName);
extern int DSC_GetDnsServers(DNS_SERVER_LIST_ST *pstServers);
extern int DSC_AddDnsSearch(char *pcDomain);
extern int DSC_DelDnsSearch(char *pcDomain);
extern int DSC_SetDnsOption(uint32_t ulTimeout, uint32_t ulRetryTimes);


#ifdef __cplusplus
}
#endif

#endif
