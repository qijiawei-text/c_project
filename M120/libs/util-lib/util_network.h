/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_network.h
* Description:   get or set network config
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_NETWORK_H__
#define __UTIL_NETWORK_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup  define ip length
 * @{
 */
#define DEF_UTIL_IPV4_MAX_LEN                (16u)
#define DEF_UTIL_IPV6_MAX_LEN                (40u)
#define DEF_UTIL_IP_MAX_LEN                  (40u)
#define DEF_UTIL_MAC_MAX_LEN                 (20u)
/**
 * @}
 */

typedef enum {
    EN_IPV6_FIND = 0,
    EN_IPV6_NOT_FIND,
    EN_IPV6_FIND_ERR,
} EN_UTIL_IPV6_FIND;


typedef enum
{
    EN_HOST_TYPE_IPV4 = 0,
    EN_HOST_TYPE_IPV6,
    EN_HOST_TYPE_DOMAIN,
    EN_HOST_TYPE_UNKOWN
} EN_HOST_TYPE;


extern int UTIL_GetNetMaskLength(char *pcMask);
extern char *UTIL_GetNetMaskString(int iMaskLen);
extern char *UTIL_GetBroadCastIp(char *pcIp, char *pcNetMask);
extern char *UTIL_GetSubNet(char *pcIp, char *pcNetMask);
extern EN_HOST_TYPE UTIL_GetHostType(char *pcHost);
extern int UTIL_ChkIpv6AddrExist(char *pcEth, char *pcIPv6, uint32_t ulPrefix);
extern int UTIL_ChkIpv6DefGatewayExist(char *pcEth, char *pcGateWay);
extern int UTIL_ChkIpV4Addr(char *pcIpaddr);
extern int UTIL_ChkIpV6Addr(char *pcIpaddr);
extern bool UTIL_IsIpAddr(const char *pcIpaddr);
extern int UTIL_ChkHostNameAddr(char *pcHostnameaddr);
extern int UTIL_ChkNetAddr(char *pcNetaddr);
extern int UTIL_GetIpV4(char *pcEth, char *pcIP);
extern int UTIL_SetIpV4(char *pcEth, char *pcIp, char *pcNetmask, char *pcGateway);
extern int UTIL_SetIpV6(char *pcEth, char *pcIp, uint32_t ulPrefix, char *pcGateway);
extern int UTIL_ChkNetmask(char *pcNetmask);
extern int UTIL_ChkIpv6Prefix(uint32_t ulPrefix);
extern int UTIL_GetNetmask(char *pcEth, uint32_t *pulAddr);
extern int UTIL_GetGateway(char *pcEth, char *pcGateway);
extern int UTIL_GetMacAddr(char *pcEth, char *pcMacAddr);
extern int UTIL_SetMacAddr(char *pcEth, char *pcMacAddr);
extern int UTIL_IsNetworkSegmentSame(char *pcIpaddr1, char *pcNetmask1, char *pcIpaddr2, char *pcNetmask2);
extern int UTIL_IsIpv6NetworkSegmentSame(char *pcIpaddr1, uint8_t ucPrefixLen1, char *pcIpaddr2, uint8_t ucPrefixLen2);
extern int UTIL_SeparateIpv6AddrAndPrefix(char *pcIpv6, char *pcAddr, size_t nAddrLen, uint8_t *pucPrefix);
extern int UTIL_SetVlan(char *pcEthName, char *pcVlanId1, char *pcVlanId2);
extern int UTIL_ChkIpv4EthNetCfg(char *pcIp, char *pcNetmask, char *pcGateway);
extern int UTIL_ChkIpv6EthNetCfg(char *pcIp, uint8_t ucPrefixlen, char *pcGateway);

#ifdef __cplusplus
}
#endif

#endif
