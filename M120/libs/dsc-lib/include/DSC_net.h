/***MODU+******************************************************************************************
* FileName    : dsc_net.h
* Description : Define the network functions.
* History     :
*     [Author]          [Date]          [Version]        [Description]
* [1] Chun Ye           2022/11/05      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/

#ifndef _DSC_NET_H
#define _DSC_NET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

/**************************************************************************************************/
/* definition of network common len                                                               */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NETWORK_INTERFACE_NAME_MAX_LEN      (128)
#define DEF_IPV4_ADDR_MAX_LEN                   (16)
#define DEF_IPV6_ADDR_MAX_LEN                   (40)
#define DEF_IP_ADDR_MAX_LEN                     (DEF_IPV6_ADDR_MAX_LEN)
#define DEF_NET_MAC_ADDR_LEN                    (20)
#define DEF_NET_TYPE_LEN                        (128)

#define DEF_NET_DEFAULT_GATEWAY_ON_ETH          "eth0"
#define DEF_NET_INBAND_KEY                      "inband"

/* } @                                                                                            */

/**************************************************************************************************/
/* definition of DHCP, includes client and server. But they won't be used in one ethernet port    */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_NETWORK_DHCP_NONE = 0,
    EN_NETWORK_DHCP_CLIENT,
    EN_NETWORK_DHCP_SERVER
} EN_NETWORK_DHCP_TYPE;

typedef struct {
    uint32_t ulTimeOut;
    uint32_t ulLease;
} ST_NETWORK_DHCP_CLIENT;

typedef struct {
    uint32_t ulLeaseTime;
    char acPoolStartIP[DEF_IP_ADDR_MAX_LEN];
    char acPoolEndIP[DEF_IP_ADDR_MAX_LEN];
} ST_NETWORK_DHCP_SERVER;


typedef struct {
    EN_NETWORK_DHCP_TYPE enDhcpType;
    union {
        ST_NETWORK_DHCP_CLIENT stDhcpClient;
        ST_NETWORK_DHCP_SERVER stDhcpServer;
    } Dhcp;
} ST_NETWORK_DHCP;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of ethernet ipv4/ipv6 type                                                          */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_NETWORK_TYPE_IPV4 = 0,
    EN_NETWORK_TYPE_IPV6 = 1,
    EN_NETWORK_TYPE_MAX
} EN_NETWORK_TYPE;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of ethernet information                                                             */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_NETWORK_ORIGIN_OTHER = 0,
    EN_NETWORK_ORIGIN_STATIC,
    EN_NETWORK_ORIGIN_DHCP,
    EN_NETWORK_ORIGIN_LINK_LAYER,
    EN_NETWORK_ORIGIN_RANDOM
} EN_NETWORK_IP_ADDRESS_ORIGIN;

#define DEF_NET_IP_ORIGIN_OTHER_STR       "other"
#define DEF_NET_IP_ORIGIN_STATIC_STR      "static"
#define DEF_NET_IP_ORIGIN_DHCP_STR        "dhcp"
#define DEF_NET_IP_ORIGIN_LINK_LAYER_STR  "link-layer"
#define DEF_NET_IP_ORIGIN_RANDOM_STR      "random"
#define DEF_ETH0_NAME_STR                 "eth0"


typedef struct {
    char acIpAddress[DEF_IPV4_ADDR_MAX_LEN];
    char acNetMask[DEF_IPV4_ADDR_MAX_LEN];
    char acDefaultGateway[DEF_IPV4_ADDR_MAX_LEN];
} ST_NETWORK_IPV4_STATIC;

typedef struct {
    char acIpAddress[DEF_IPV6_ADDR_MAX_LEN];
    uint8_t ucPrefixLen;
    uint8_t aucResv[3];
    char acDefaultGateway[DEF_IPV6_ADDR_MAX_LEN];
} ST_NETWORK_IPV6_STATIC;

typedef struct {
    EN_NETWORK_IP_ADDRESS_ORIGIN enOriginType;
    ST_NETWORK_IPV4_STATIC stStaticCfg;
    ST_NETWORK_DHCP stDhcpCfg;
} ST_NETWORK_IPV4;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of ethernet ipv4 information                                                        */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    EN_NETWORK_IP_ADDRESS_ORIGIN enOriginType;
    ST_NETWORK_IPV6_STATIC stStaticCfg;
    ST_NETWORK_DHCP stDhcpCfg;
} ST_NETWORK_IPV6;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of ethernet ipv4 and ipv6 configuration                                             */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    ST_NETWORK_IPV4 stIpv4Cfg;
    ST_NETWORK_IPV6 stIpv6Cfg;
} ST_NETWORK_INTERFACE_CFG;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network ethernet state, this is real value from Linux                            */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    char acMacAddress[DEF_NET_MAC_ADDR_LEN];
    ST_NETWORK_IPV4_STATIC stIpv4Info;
    ST_NETWORK_IPV6_STATIC stIpv6Info;
} ST_NETWORK_INTERFACE_STATE;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network ethernet config, this is config from database                            */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    char acInterfaceName[DEF_NETWORK_INTERFACE_NAME_MAX_LEN];
    bool bEnable;
    char acPhyMacAddress[DEF_NET_MAC_ADDR_LEN];
    ST_NETWORK_INTERFACE_CFG stInterfaceCfg;
    char acType[DEF_NET_TYPE_LEN];
} ST_NETWORK_ETH_INTERFACE;

#define DEF_DSC_ETH_CNT  (8)
typedef struct {
    ST_NETWORK_ETH_INTERFACE stEthInterface[DEF_DSC_ETH_CNT];
    uint32_t ulTotal;
} ST_NETWORK_IP_INTERFACE;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network MAC address                                                              */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    char acPhyMacAddress[DEF_NET_MAC_ADDR_LEN];
    char acInterfaceName[DEF_NETWORK_INTERFACE_NAME_MAX_LEN];
} ST_NETWORK_PHY_MAC_ADDR;

typedef struct {
    ST_NETWORK_PHY_MAC_ADDR stEthMacAddr[DEF_DSC_ETH_CNT];
    uint32_t ulTotal;
} ST_NETWORK_MAC_INTERFACE;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network yang path header                                                         */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_XPATH_CONFIG_NET             "/org-openroadm-device:org-openroadm-device/info"
#define DEF_NET_XPATH_INTERFACE_CONTAINER    DEF_NET_XPATH_CONFIG_NET "/ip-interfaces"
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network ip-interfaces list                                                       */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_XPATH_INTERFACE_HEADER       DEF_NET_XPATH_CONFIG_NET "/ip-interfaces/interface"
#define DEF_NET_XPATH_INTERFACE_ALL          DEF_NET_XPATH_INTERFACE_HEADER "//*"
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network config leaf node                                                         */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_LEAF_IP_ORIGIN                       "/ipv4/origin"
#define DEF_NET_LEAF_IP_ADDR                         "/ipv4/ipAddress"
#define DEF_NET_LEAF_NETMASK                         "/ipv4/netmask"
#define DEF_NET_LEAF_GATEWAY                         "/ipv4/defaultGateway"
#define DEF_NET_LEAF_DHCP_CLIENT_TIMEOUT             "/ipv4/dhcp-client/timeout"
#define DEF_NET_LEAF_DHCP_CLIETN_LEASE               "/ipv4/dhcp-client/lease"
#define DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME          "/ipv4/dhcp-server/lease-time"
#define DEF_NET_LEAF_DHCP_SERVER                     "/ipv4/dhcp-server"
#define DEF_NET_LEAF_DHCP_SERVER_START_IP            "/ipv4/dhcp-server/pool-start-ip"
#define DEF_NET_LEAF_DHCP_SERVER_END_IP              "/ipv4/dhcp-server/pool-end-ip"

#define DEF_NET_LEAF_IP_ORIGIN_V6                    "/ipv6/originV6"
#define DEF_NET_LEAF_IP_ADDR_V6                      "/ipv6/ipAddressV6"
#define DEF_NET_LEAF_PREFIX_V6                       "/ipv6/prefix-length"
#define DEF_NET_LEAF_GATEWAY_V6                      "/ipv6/defaultGatewayV6"
#define DEF_NET_LEAF_DHCP_CLIETN_TIMEOUT_V6          "/ipv6/dhcp-clientV6/timeout"
#define DEF_NET_LEAF_DHCP_CLIETN_LEASE_V6            "/ipv6/dhcp-clientV6/lease"
#define DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME_V6       "/ipv6/dhcp-serverV6/lease-time"

#define DEF_NET_LEAF_DHCP_SERVER_V6                  "/ipv6/dhcp-serverV6"
#define DEF_NET_LEAF_DHCP_SERVER_START_IP_V6         "/ipv6/dhcp-serverV6/pool-start-ip"
#define DEF_NET_LEAF_DHCP_SERVER_END_IP_V6           "/ipv6/dhcp-serverV6/pool-end-ip"

#define DEF_NET_LEAF_ETH_ENABLE                      "/enabled"
#define DEF_NET_LEAF_NAME                            "/name"
#define DEF_NET_LEAF_INTERFACE_KEY                   "[name"
#define DEF_NET_LEAF_TYPE                            "/type"
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network config xpath                                                             */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_XPATH_IPV4_KEY                       "/ipv4/"
#define DEF_NET_XPATH_ETH_ENABLE_ALL                 DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_ETH_ENABLE
#define DEF_NET_XPATH_ETH_NAME                       DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_NAME
#define DEF_NET_XPATH_ETH_TYPE                       DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_TYPE

#define DEF_NET_XPATH_IP_ORIGIN_ALL                  DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_IP_ORIGIN
#define DEF_NET_XPATH_IP_ADDR_ALL                    DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_IP_ADDR
#define DEF_NET_XPATH_NETMASK_ALL                    DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_NETMASK
#define DEF_NET_XPATH_GATEWAY_ALL                    DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_GATEWAY

#define DEF_NET_XPATH_DHCP_CLIENT_TIMEOUT_ALL        DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_CLIENT_TIMEOUT
#define DEF_NET_XPATH_DHCP_CLIETN_LEASE_ALL          DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_CLIETN_LEASE
#define DEF_NET_XPATH_DHCP_SERVER_LEASE_TIME_ALL     DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME

#define DEF_NET_XPATH_DHCP_SERVER_ALL                DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER
#define DEF_NET_XPATH_DHCP_SERVER_START_IP_ALL       DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_START_IP
#define DEF_NET_XPATH_DHCP_SERVER_END_IP_ALL         DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_END_IP


#define DEF_NET_XPATH_IPV6_KEY                       "/ipv6/"
#define DEF_NET_XPATH_IP_ORIGIN_V6_ALL               DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_IP_ORIGIN_V6
#define DEF_NET_XPATH_IP_ADDR_V6_ALL                 DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_IP_ADDR_V6
#define DEF_NET_XPATH_GATEWAY_V6_ALL                 DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_GATEWAY_V6
#define DEF_NET_XPATH_PREFIX_LEN_V6_ALL              DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_PREFIX_V6

#define DEF_NET_XPATH_DHCP_CLIETN_TIMEOUT_V6_ALL     DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_CLIETN_TIMEOUT_V6
#define DEF_NET_XPATH_DHCP_CLIETN_LEASE_V6_ALL       DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_CLIETN_LEASE_V6
#define DEF_NET_XPATH_DHCP_SERVER_LEASE_TIME_V6_ALL  DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_LEASE_TIME_V6

#define DEF_NET_XPATH_DHCP_SERVER_V6_ALL             DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_V6
#define DEF_NET_XPATH_DHCP_SERVER_START_IP_V6_ALL    DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_START_IP_V6
#define DEF_NET_XPATH_DHCP_SERVER_END_IP_V6_ALL      DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_DHCP_SERVER_END_IP_V6
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network state leaf node                                                          */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_LEAF_MAC_ADDR                        "/macAddress"

#define DEF_NET_LEAF_CURRENT_IP_ADDR                 "/ipv4/current-ipAddress"
#define DEF_NET_LEAF_CURRENT_NETMASK                 "/ipv4/current-netmask"
#define DEF_NET_LEAF_CURRENT_GATEWAY                 "/ipv4/current-defaultGateway"

#define DEF_NET_LEAF_CURRENT_IP_ADDR_V6              "/ipv6/current-ipAddressV6"
#define DEF_NET_LEAF_CURRENT_PREFIX_LEN_V6           "/ipv6/current-prefix-length"
#define DEF_NET_LEAF_CURRENT_GATEWAY_V6              "/ipv6/current-defaultGatewayV6"
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of network state xpath                                                              */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_STATE_ATTR_COUNT                     (7)
#define DEF_NET_XPATH_CURRENT_MAC_ADDR_ALL            DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_MAC_ADDR

#define DEF_NET_XPATH_CURRENT_IP_ADDR_ALL             DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_IP_ADDR
#define DEF_NET_XPATH_CURRENT_NETMASK_ALL             DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_NETMASK
#define DEF_NET_XPATH_CURRENT_GATEWAY_ALL             DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_GATEWAY

#define DEF_NET_XPATH_CURRENT_IP_ADDR_V6_ALL          DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_IP_ADDR_V6
#define DEF_NET_XPATH_CURRENT_PREFIX_LEN_V6_ALL       DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_PREFIX_LEN_V6
#define DEF_NET_XPATH_CURRENT_GATEWAY_V6_ALL          DEF_NET_XPATH_INTERFACE_HEADER "[name='%s']" DEF_NET_LEAF_CURRENT_GATEWAY_V6
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of MAX xpath. For networkd setup early before operationd, the com-oplink-device had */
/* NOT been loader into org-openroadm-device                                                      */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_NET_XPATH_MACADDR_ALL_I "/com-oplink-device:org-openroadm-device/info/ip-interfaces/interface//*"
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of Function                                                                         */
/**************************************************************************************************/
/* @ {                                                                                            */
int DSC_GetEthName(char *pcXpath, char *pcKey);
int DSC_FindIpCfgByName(const char *pcEthName, const ST_NETWORK_IP_INTERFACE *pstIpCfg,
                        ST_NETWORK_ETH_INTERFACE *pstEthIpCfg);
int DSC_SetEthEnable(char *pcEthName, bool bEnable);
int DSC_GetEthEnable(char *pcEthName, bool *pbEnable);
int DSC_SetEthIpv4Static(char *pcEthName, ST_NETWORK_IPV4_STATIC *pstIpv4Cfg);
int DSC_GetEthIpv4Static(sr_session_ctx_t *pstSess, char *pcEthName,
                         int *piOriginType, char *pcIpAddr, char *pcNetMask, char *pcGateWay);
int DSC_SetEthIpv6Static(char *pcEthName, ST_NETWORK_IPV6_STATIC *pstIpv6Cfg);
int DSC_SetEthDhcpCliTimeout(char *pcEthName, EN_NETWORK_TYPE enType, uint32_t ulTimeout);
int DSC_SetEthDhcpCliLease(char *pcEthName, EN_NETWORK_TYPE enType, uint32_t ulLease);
int DSC_SetEthDhcpCli(char *pcEthName, EN_NETWORK_TYPE enType);
int DSC_SetEthIpv6Linklayer(char *pcEthName, EN_NETWORK_IP_ADDRESS_ORIGIN enOrigin);

int DSC_SetEthDhcpSrvDisable(char *pcEthName, EN_NETWORK_TYPE enType);
int DSC_SetEthDhcpSrvIpRange(char *pcEthName, EN_NETWORK_TYPE enType,
                             char *pcStartIp, size_t nStartStrLen, char *pcEndIp, size_t nEndStrLen);
int DSC_GetEthIpv4State(char *pcEthName, ST_NETWORK_IPV4_STATIC *pstIpv4State);
int DSC_GetEthIpv6State(char *pcEthName, ST_NETWORK_IPV6_STATIC *pstIpv6State);
int DSC_GetEthMacAddrState(char *pcEthName, char *pcMacAddr);
int DSC_EthOriginStrToValue(char *pcStr, EN_NETWORK_IP_ADDRESS_ORIGIN *penOrigin);
int DSC_GetEthMacCfg(ST_NETWORK_MAC_INTERFACE *pstMacInfo);
int DSC_GetEthIpCfg(ST_NETWORK_IP_INTERFACE *pstIpInfo);
bool DSC_CheckEthExist(char *pcEthName);
int DSC_DelEthIpv6Cfg(char *pcEthName);
int DSC_GetEthIpv6OriginStr(char *pcEthName, char *pcOrigin, size_t nLen);
int DSC_SetInbandGateway(char *pcGateway, size_t nLen);
int DSC_DelInbandGateway();
int DSC_GetInbandGateway(char *pcGateway, size_t nLen);

/* } @                                                                                            */


#ifdef __cplusplus
}
#endif

#endif
