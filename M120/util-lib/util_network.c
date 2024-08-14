/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_network.c
* Description:   get or set network config
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_network.h"


#define DEF_PATH_PROCNET_IFINET6                 ("/proc/net/if_inet6")
#define DEF_PATH_PROCNET_IPV6_ROUTE              ("/proc/net/ipv6_route")
#define DEF_IPV6_PRE_FIX_MAX_LEN                 (128)                      /* because 0-128 change to hex, save in file               */
#define DEF_IPV6_ROUTE_DEF                       ("00000000000000000000000000000000")
#define DEF_CHECK_VALID_HOSTNAME                 ("echo \"%s\" | grep -E \'^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\\-]*[A-Za-z0-9])$\'")



static char m_acStrFE80[] = "FE80";

static int
__UTIL_SetIpv4Gateway(char *pcEth, char *pcGateway)
{
    int iFd = -1;
    int iRet = OPLK_ERR;
    struct in_addr stAddr;
    struct rtentry stRten;

    if (NULL == pcEth || NULL == pcGateway) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    memset(&stAddr, 0x00, sizeof(stAddr));
    memset(&stRten, 0x00, sizeof(stRten));

    if (0 == inet_aton(pcGateway, &stAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "0 == inet_aton");
        goto ERR_LABEL;
    }

    iFd = socket(PF_INET, SOCK_DGRAM, 0);
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iFd < 0");
        goto ERR_LABEL;
    }
    /* Delete existing defalt gateway                                           */
    memset(&stRten, 0x00, sizeof(stRten));

    stRten.rt_dst.sa_family = AF_INET;
    ((struct sockaddr_in *)&stRten.rt_dst)->sin_addr.s_addr = 0;

    stRten.rt_genmask.sa_family = AF_INET;
    ((struct sockaddr_in *)&stRten.rt_genmask)->sin_addr.s_addr = 0;

    stRten.rt_flags = RTF_UP;

    stRten.rt_dev = pcEth;

    iRet = ioctl(iFd, SIOCDELRT, &stRten);
    if ((0 == iRet) || (errno == ESRCH)) {
        /* Set default gateway                                                  */
        memset(&stRten, 0, sizeof(stRten));

        stRten.rt_dst.sa_family = AF_INET;
        ((struct sockaddr_in *)&stRten.rt_dst)->sin_addr.s_addr = 0;

        stRten.rt_gateway.sa_family = AF_INET;
        ((struct sockaddr_in *)&stRten.rt_gateway)->sin_addr.s_addr = stAddr.s_addr;

        stRten.rt_genmask.sa_family = AF_INET;
        ((struct sockaddr_in *)&stRten.rt_genmask)->sin_addr.s_addr = 0;

        stRten.rt_flags = RTF_UP | RTF_GATEWAY;

        stRten.rt_dev = pcEth;
        iRet = ioctl(iFd, SIOCADDRT, &stRten);
    }

    DEF_SAFE_CLOSE(iFd);
    return iRet;

ERR_LABEL:

    DEF_SAFE_CLOSE(iFd);
    return OPLK_ERR;
}

static int
__UTIL_DelIpv6Addr(char *pcEth)
{
    uint32_t ulIfIndex = 0, ulPriFixRd = 0, ulScope = 0, ulDadStatus = 0;
    char acCmdLine[DEF_BUF_SIZE_256] = {0};
    char acDevName[DEF_BUF_SIZE_32] = {0};
    char acIpv6Rd[DEF_BUF_SIZE_128] = {0};
    char acIpv6[DEF_BUF_SIZE_128] = {0};
    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcEth) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    fp = fopen(DEF_PATH_PROCNET_IFINET6, "r");
    if (fp == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto ERR_LABEL;
    }

    while (fscanf(fp, "%32s %08x %02x %02x %02x %20s\n",
                  acIpv6Rd, &ulIfIndex, &ulPriFixRd, &ulScope, &ulDadStatus, acDevName) != EOF) {
        if (0 == strncasecmp(acDevName, pcEth, strlen(pcEth))) {
            if (0 == strncasecmp(m_acStrFE80, acIpv6Rd, strlen(m_acStrFE80))) {
                continue;
            }

            memset(acIpv6, 0, sizeof(acIpv6));
            memset(acCmdLine, 0x00, sizeof(acCmdLine));
            /*  need ":"here                                                    */
            sprintf(acIpv6, "%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s",
                    acIpv6Rd, acIpv6Rd + 4, acIpv6Rd + 8, acIpv6Rd + 12,
                    acIpv6Rd + 16, acIpv6Rd + 20, acIpv6Rd + 24, acIpv6Rd + 28);
            /*del IPv6                                                          */
            sprintf(acCmdLine, "ifconfig %s inet6 del %s/%d 1>/dev/null 2>&1",
                    pcEth, acIpv6, ulPriFixRd);

            iRet = UTIL_System(acCmdLine);
            if (OPLK_OK != iRet) {
                LOG_WRITE(EN_LOG_ERROR, "cmd error:%s", acCmdLine);
            }
        }
    }

    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;
}

static int
__UTIL_SetIpv6Addr(char *pcEth, char *pcIpv6, uint32_t ulPrefix)
{
    char acCmdLine[DEF_BUF_SIZE_256] = {0};
    uint16_t ausIpv6[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int iRet = OPLK_ERR;

    if (NULL == pcIpv6 || NULL == pcEth) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    /* 1. chk input str                                                         */
    iRet = inet_pton(AF_INET6, pcIpv6, ausIpv6);
    if (iRet <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "inet_pton <= 0");
        goto ERR_LABEL;
    }

    if (0 == strncasecmp(m_acStrFE80, pcIpv6, strlen(m_acStrFE80))) {
        LOG_WRITE(EN_LOG_ERROR, "0xFE80 can not use");
        goto ERR_LABEL;
    }

    if (OPLK_OK != UTIL_ChkIpv6Prefix(ulPrefix)) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 Prefix is invaild");
        goto ERR_LABEL;
    }

    /* 2. del old ipv6 addr, except 0xFE80::... start don't move!               */
    __UTIL_DelIpv6Addr(pcEth);

    /* 3. set ipv6                                                              */
    memset(acCmdLine, 0x00, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "ifconfig %s inet6 add %s/%d 1>/dev/null 2>&1",
             pcEth, pcIpv6, ulPrefix);

    iRet = UTIL_System(acCmdLine);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "set %s ipv6 addr %s fail %x", pcEth, pcIpv6, iRet);
    }

    LOG_WRITE(EN_LOG_TRACE, "%s", acCmdLine);

    if (EN_IPV6_FIND != UTIL_ChkIpv6AddrExist(pcEth, pcIpv6, ulPrefix)) {
        LOG_WRITE(EN_LOG_ERROR, "set %s ipv6 addr %s fail", pcEth, pcIpv6);
        goto ERR_LABEL;
    }


    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_AddSysDefIpv6GateWay(char *pcEth)
{
    int32_t lSrcPrefix = 0;
    int32_t lDstPrefix = 0;

    uint32_t ulMetrix = 0;
    uint32_t ulRefCnt = 0;
    uint32_t ulUsrCnt = 0;
    uint32_t ulFlag = 0;

    char acSrc[DEF_BUF_SIZE_128], acGw[DEF_BUF_SIZE_128];
    char acCmdLine[DEF_BUF_SIZE_128];
    char acIpv6[DEF_BUF_SIZE_128];
    char acDevName[DEF_BUF_SIZE_32];

    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcEth) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    fp = fopen(DEF_PATH_PROCNET_IPV6_ROUTE, "r");
    if (fp == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto ERR_LABEL;
    }

    while ((iRet = fscanf(fp, "%s %x %s %x %s %x %x %x %x %s",
                          acIpv6, &lDstPrefix, acSrc, &lSrcPrefix, acGw,
                          &ulMetrix, &ulRefCnt, &ulUsrCnt, &ulFlag, acDevName)) != EOF) {
        /*return value must be parameter's number                               */
        if (iRet != 10) {
            continue;
        }

        if (0 != strncasecmp(acDevName, pcEth, strlen(pcEth))) {
            continue;
        }

        if (0 == strncasecmp(acIpv6, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acSrc, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acGw, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == lDstPrefix) {
            LOG_WRITE(EN_LOG_DEBUG, "system def gateway exit");
            goto OK_LABEL;
        }

    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "%s%s", "route -A inet6 add default dev ", pcEth);

    iRet = UTIL_System(acCmdLine);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "%s Ret %x", acCmdLine, iRet);
    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;

OK_LABEL:
    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;
}

static int
__UTIL_AddIpv6DefGateway(char *pcEth, char *pcGateWay)
{
    int32_t lSrcPrefix = 0;
    int32_t lDstPrefix = 0;

    uint32_t ulMetrix = 0;
    uint32_t ulRefCnt = 0;
    uint32_t ulUsrCnt = 0;
    uint32_t ulFlag = 0;

    uint16_t ausNewGw[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    char acDevName[DEF_BUF_SIZE_32];
    char acSrc[DEF_BUF_SIZE_128], acGw[DEF_BUF_SIZE_128];
    char acDes[DEF_BUF_SIZE_128] = {0};
    char acNewGw[DEF_BUF_SIZE_128];
    char acCmdLine[DEF_BUF_SIZE_128];

    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcEth || NULL == pcGateWay) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    /* 1. only chk ipv6 gateway                                                 */
    iRet = inet_pton(AF_INET6, pcGateWay, ausNewGw);
    if (iRet <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "inet_pton <= 0");
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    sprintf(acCmdLine, "route -A inet6 add default gw %s dev %s", pcGateWay, pcEth);
    iRet = UTIL_System(acCmdLine);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_DEBUG, "%s, Ret %d", acCmdLine, iRet);
    }

    /* 1.1 add ":" in route format                                              */
    memset(acNewGw, 0, sizeof(acNewGw));
    snprintf(acNewGw, sizeof(acNewGw), "%04x%04x%04x%04x%04x%04x%04x%04x",
             htons(ausNewGw[0]), htons(ausNewGw[1]), htons(ausNewGw[2]), htons(ausNewGw[3]),
             htons(ausNewGw[4]), htons(ausNewGw[5]), htons(ausNewGw[6]), htons(ausNewGw[7]));

    /* 2. chk /proc/net/if_inet6 whether we write in                            */
    fp = fopen(DEF_PATH_PROCNET_IPV6_ROUTE, "r");
    if (fp == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto ERR_LABEL;
    }

    while ((iRet = fscanf(fp, "%s %x %s %x %s %x %x %x %x %s",
                          acDes, &lDstPrefix, acSrc, &lSrcPrefix, acGw,
                          &ulMetrix, &ulRefCnt, &ulUsrCnt, &ulFlag, acDevName)) != EOF) {
        /*return value must be parameter's number                               */
        if (iRet != 10) {
            continue;
        }

        if (0 != strncasecmp(acDevName, pcEth, strlen(pcEth))) {
            continue;
        }

        if (0 == strncasecmp(acDes, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acSrc, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acGw, acNewGw, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == lDstPrefix) {
            LOG_WRITE(EN_LOG_DEBUG, "system def gateway exit");
            goto OK_LABEL;
        }

    }

    LOG_WRITE(EN_LOG_TRACE, "set %s ipv6 gateway fail", pcEth);
ERR_LABEL:                                                                  /* did not fine ip to delete                               */
    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;

OK_LABEL:
    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;
}

static int
__UTIL_DelIpv6DefGateway(char *pcEth)
{
    int32_t lSrcPrefix = 0;
    int32_t lDstPrefix = 0;

    uint32_t ulMetrix = 0;
    uint32_t ulRefCnt = 0;
    uint32_t ulUsrCnt = 0;
    uint32_t ulFlag = 0;

    char acSrc[DEF_BUF_SIZE_128], acGw[DEF_BUF_SIZE_128];
    char acDevName[DEF_BUF_SIZE_32];
    char acDes[DEF_BUF_SIZE_128] = {0};
    char acCmdLine[DEF_BUF_SIZE_256];
    char acOldIpv6[DEF_BUF_SIZE_128];

    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcEth) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    fp = fopen(DEF_PATH_PROCNET_IPV6_ROUTE, "r");
    if (fp == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto ERR_LABEL;
    }

    while ((iRet = fscanf(fp, "%s %x %s %x %s %x %x %x %x %s",
                          acDes, &lDstPrefix, acSrc, &lSrcPrefix, acGw,
                          &ulMetrix, &ulRefCnt, &ulUsrCnt, &ulFlag, acDevName)) != EOF) {
        /*return value must be parameter's number                               */
        if (10 != iRet) {
            continue;
        }

        if (0 != strncasecmp(acDevName, pcEth, strlen(pcEth))) {
            continue;
        }

        if (0 == strncasecmp(acDes, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acSrc, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acGw, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == lDstPrefix) {                                              /* all = 0, except item'gw is equal zero                   */
            memset(acOldIpv6, 0, sizeof(acOldIpv6));
            snprintf(acOldIpv6, sizeof(acOldIpv6), "%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s",
                     acGw, acGw + 4, acGw + 8, acGw + 12,
                     acGw + 16, acGw + 20, acGw + 24, acGw + 28);           /* we need ":"here                                         */

            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "route -A inet6 del default gw %s dev %s", acOldIpv6, pcEth);
            iRet = UTIL_System(acCmdLine);
            if (OPLK_OK != iRet) {
                LOG_WRITE(EN_LOG_ERROR, "%s Ret %x", acCmdLine, iRet);
            }
        }
    }

    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;

ERR_LABEL:

    /* did not fine ip to delete                                                */
    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;
}


static int
__UTIL_SetIpv6Gateway(char *pcEth, char *pcGateWay)
{
    uint16_t ausGateway[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int iRet = OPLK_ERR;

    if (NULL == pcEth || NULL == pcGateWay) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    /* 1. chk input str                                                         */
    iRet = inet_pton(AF_INET6, pcGateWay, ausGateway);
    if (iRet <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 gateWay:%s is invaild", pcGateWay);
        goto ERR_LABEL;
    }

    /* 2. if system default gateway not exit, add it, don't move!               */
    __UTIL_AddSysDefIpv6GateWay(pcEth);
    __UTIL_DelIpv6DefGateway(pcEth);

    /* 3. set ipv6                                                              */
    if (OPLK_OK != __UTIL_AddIpv6DefGateway(pcEth, pcGateWay)) {
        LOG_WRITE(EN_LOG_ERROR, "set %s ipv6 gateWay fail", pcGateWay);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_GetNetMaskLength(char *pcMask)
{
    int iMaskLen = 0, i = 0;
    uint32_t uiNetMask = 0;
    struct in_addr stIpAddr;

    if (NULL == pcMask) {
        return 0;
    }

    if ( inet_aton(pcMask, &stIpAddr)) {
        uiNetMask = ntohl(stIpAddr.s_addr);
    } else {
        uiNetMask = 0;
        return 0;
    }

    while (0 == (uiNetMask & 0x01) && i < 32) {
        i++;
        uiNetMask = uiNetMask >> 1;
    }

    iMaskLen = 32 - i;
    return iMaskLen;
}

char *
UTIL_GetNetMaskString(int iMaskLen)
{
    uint32_t uiMask = 0;
    int i = 0;
    struct in_addr stIpAddr;

    if (iMaskLen < 0 || iMaskLen > 32) {
        return NULL;
    }

    for (i = 0; i < iMaskLen; i++)
        uiMask |= 0x80000000 >> i;

    stIpAddr.s_addr = htonl(uiMask);

    return inet_ntoa(stIpAddr);
}

char *
UTIL_GetBroadCastIp(char *pcIp, char *pcNetMask)
{
    struct in_addr stAddr;
    struct in_addr stMask;
    struct in_addr stBroadIp;

    if (0 == inet_aton(pcIp, &stAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "inet_aton error pcIp = %s", pcIp);
        return NULL;
    }
    if (0 == inet_aton(pcNetMask, &stMask)) {
        LOG_WRITE(EN_LOG_ERROR, "inet_aton error pcNetMask = %s", pcNetMask);
        return NULL;
    }

    stBroadIp.s_addr = stAddr.s_addr | (~stMask.s_addr);
    return inet_ntoa(stBroadIp);
}

char *
UTIL_GetSubNet(char *pcIp, char *pcNetMask)
{
    struct in_addr stAddr;
    struct in_addr stMask;
    struct in_addr stSubNet;

    if (0 == inet_aton(pcIp, &stAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "inet_aton error pcIp = %s", pcIp);
        return NULL;
    }
    if (0 == inet_aton(pcNetMask, &stMask)) {
        LOG_WRITE(EN_LOG_ERROR, "inet_aton error pcNetMask = %s", pcNetMask);
        return NULL;
    }

    stSubNet.s_addr = stAddr.s_addr & stMask.s_addr;
    return inet_ntoa(stSubNet);
}

EN_HOST_TYPE
UTIL_GetHostType(char *pcHost)
{
    int i = 0;

    if (pcHost == NULL) {
        return EN_HOST_TYPE_UNKOWN;
    }

    if (strlen(pcHost) == 0) {
        return EN_HOST_TYPE_UNKOWN;
    }

    if ((strstr(pcHost, ":") != NULL)
        || (strstr(pcHost, "::") != NULL)) {
        return EN_HOST_TYPE_IPV6;
    }

    for (i = 0; i < strlen(pcHost); i++) {
        if ((pcHost[i] != '.')
            && ((pcHost[i] < '0') || (pcHost[i] > '9'))) {
            return EN_HOST_TYPE_DOMAIN;
        }
    }

    return EN_HOST_TYPE_IPV4;
}

int
UTIL_ChkIpV4Addr(char *pcIpaddr)
{
    struct in_addr stV4Addr;
    int iRet = OPLK_ERR;
    int32_t alIP[4];

    if (NULL == pcIpaddr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcIpaddr");
        return OPLK_ERR;
    }

    /*check ipv4                                                                */
    iRet = inet_pton(AF_INET, pcIpaddr, &stV4Addr);
    if (0 >= iRet) {
        LOG_WRITE(EN_LOG_ERROR, "inet_pton iRet %d", iRet);
        return OPLK_ERR;
    }

    sscanf(pcIpaddr, "%d.%d.%d.%d", &alIP[0], &alIP[1], &alIP[2], &alIP[3]);
    if (alIP[0] == 0 &&
        alIP[1] == 0 &&
        alIP[2] == 0 &&
        alIP[3] == 0 ) {
        LOG_WRITE(EN_LOG_ERROR, "alIP 0.0.0.0");
        return OPLK_ERR;
    }

    if (alIP[0] == 255 &&
        alIP[1] == 255 &&
        alIP[2] == 255 &&
        alIP[3] == 255 ) {
        LOG_WRITE(EN_LOG_ERROR, "alIP 255.255.0.0");
        return OPLK_ERR;
    }

    return OPLK_OK;
}

int
UTIL_ChkIpV6Addr(char *pcIpaddr)
{
    struct in6_addr stIpV6Addr;
    int iRet = OPLK_ERR;

    if (NULL == pcIpaddr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcIpaddr");
        return OPLK_ERR;
    }

    /*if not ipv6, then check ipv6                                              */
    iRet = inet_pton(AF_INET6, pcIpaddr, &stIpV6Addr);
    if (0 < iRet) {
        /*IPv6                                                                  */
        return OPLK_OK;
    }

    return OPLK_ERR;
}

bool
UTIL_IsIpAddr(const char *pcIpaddr)
{
    struct in_addr stIpV4Addr;
    struct in6_addr stIpV6Addr;
    int iRet = OPLK_ERR;

    if (NULL == pcIpaddr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcIpaddr");
        return false;
    }

    /* Is IPv4? */
    iRet = inet_pton(AF_INET, pcIpaddr, &stIpV4Addr);
    if (iRet > 0) {
        return true;
    }

    /* Is IPv6? */
    iRet = inet_pton(AF_INET6, pcIpaddr, &stIpV6Addr);
    if (iRet > 0) {
        return true;
    }

    return false;
}

int
UTIL_ChkHostNameAddr(char *pcHostnameaddr)
{
    char acCmd[256] = {0};
    char acBuf[256] = {0};
    int rc = OPLK_OK;

    if (NULL == pcHostnameaddr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcHostnameaddr");
        return OPLK_ERR;
    }

    memset(acCmd, 0, sizeof(acCmd));
    memset(acBuf, 0, sizeof(acBuf));
    snprintf(acCmd, sizeof(acCmd), DEF_CHECK_VALID_HOSTNAME, pcHostnameaddr);
    rc = UTIL_ExceCmdUnresp(acCmd, acBuf, sizeof(acBuf), 0);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_ExceCmdUnresp:%s failed!", acCmd);
        return rc;
    }

    if (strlen(acBuf) > 0) {
        return OPLK_OK;
    } else {
        return OPLK_ERR;
    }

    return rc;
}

int
UTIL_ChkNetAddr(char *pcNetaddr)
{
    if (NULL == pcNetaddr) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcNetaddr");
        return OPLK_ERR;
    }

    if (OPLK_ERR == UTIL_ChkIpV4Addr(pcNetaddr)) {
        if (OPLK_ERR == UTIL_ChkIpV6Addr(pcNetaddr)) {
            if (OPLK_ERR == UTIL_ChkHostNameAddr(pcNetaddr)) {
                LOG_WRITE(EN_LOG_ERROR, "chk net addr %s error", pcNetaddr);
                return OPLK_ERR;
            }
        }
    }

    return OPLK_OK;
}

int
UTIL_ChkNetmask(char *pcNetmask)
{
    uint32_t ulTmpMask = 0;
    uint32_t ulAddr = 0;
    int i = 0;;
    struct in_addr stAddr;

    if (NULL == pcNetmask) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcNetmask");
        goto ERR_LABEL;
    }

    if (0 == inet_aton(pcNetmask, &stAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "0 == inet_aton");
        goto ERR_LABEL;
    }

    ulAddr = ntohl(stAddr.s_addr);
    for (i = 0; i < DEF_SIZE_BM_U32;
         i++) {                                                             /* traverse all the 32 standard netmasks                   */
        ulTmpMask |= (0x01 << (DEF_SIZE_BM_U32 - 1 - i));
        /* compare the given netmask with standard netmask                      */
        if (ulAddr == ulTmpMask) {
            return OPLK_OK;
        }
    }

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_ChkIpv6Prefix(uint32_t ulPrefix)
{
    if (ulPrefix > DEF_IPV6_PRE_FIX_MAX_LEN) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 prefix = %d out of range", ulPrefix);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_ChkIpv6AddrExist(char *pcEth, char *pcIPv6, uint32_t ulPrefix)
{
    uint32_t ulIfIndex = 0, ulPriFixRd = 0, ulScope = 0, ulDadStatus = 0;
    char acDevName[DEF_BUF_SIZE_32] = {0};
    char acIpv6Rd[DEF_BUF_SIZE_128] = {0};
    char acIpAddr[DEF_BUF_SIZE_128] = {0};
    uint16_t ausIpv6[8] = {0};
    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcIPv6 || NULL == pcEth) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto IPV6_FIND_ERR_LABEL;
    }

    memset(acDevName, 0, sizeof(acDevName));
    memset(acIpv6Rd, 0, sizeof(acIpv6Rd));
    memset(acIpAddr, 0, sizeof(acIpAddr));
    memset(ausIpv6, 0, sizeof(ausIpv6));

    /* 1. chk ipv6 addr                                                         */
    iRet = inet_pton(AF_INET6, pcIPv6, ausIpv6);
    if (iRet <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 addr is invaild");
        goto IPV6_FIND_ERR_LABEL;
    }

    memset(acIpAddr, 0, sizeof(acIpAddr));
    snprintf(acIpAddr, sizeof(acIpAddr), "%04x%04x%04x%04x%04x%04x%04x%04x",
             htons(ausIpv6[0]), htons(ausIpv6[1]), htons(ausIpv6[2]), htons(ausIpv6[3]),
             htons(ausIpv6[4]), htons(ausIpv6[5]), htons(ausIpv6[6]), htons(ausIpv6[7]));

    fp = fopen(DEF_PATH_PROCNET_IFINET6, "r");
    if (NULL == fp) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto IPV6_FIND_ERR_LABEL;
    }

    while (fscanf(fp, "%32s %08x %02x %02x %02x %20s\n",
                  acIpv6Rd, &ulIfIndex, &ulPriFixRd, &ulScope, &ulDadStatus, acDevName) != EOF) {
        if (0 == strncasecmp(acIpAddr, acIpv6Rd, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acDevName, pcEth, strlen(pcEth)) &&
            ulPrefix == ulPriFixRd) {
            LOG_WRITE(EN_LOG_ERROR, "%s ipv6 addr was found", pcEth);
            goto IPV6_FIND_LABEL;
        }

    }

    LOG_WRITE(EN_LOG_DEBUG, "%s ipv6 addr was found", pcEth);

    DEF_SAFE_FCLOSE(fp);
    return EN_IPV6_NOT_FIND;

IPV6_FIND_LABEL:

    DEF_SAFE_FCLOSE(fp);
    return EN_IPV6_FIND;

IPV6_FIND_ERR_LABEL:

    DEF_SAFE_FCLOSE(fp);
    return EN_IPV6_FIND_ERR;
}

int
UTIL_ChkIpv6DefGatewayExist(char *pcEth, char *pcGateWay)
{
    int32_t lSrcPrefix = 0;
    int32_t lDstPrefix = 0;

    uint32_t ulMetrix = 0;
    uint32_t ulRefCnt = 0;
    uint32_t ulUsrCnt = 0;
    uint32_t ulFlag = 0;

    uint16_t ausNewGw[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    char acDevName[DEF_BUF_SIZE_32];
    char acSrc[DEF_BUF_SIZE_128], acGw[DEF_BUF_SIZE_128];
    char acDes[DEF_BUF_SIZE_128] = {0};
    char acNewGw[DEF_BUF_SIZE_128];

    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcEth || NULL == pcGateWay) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    memset(ausNewGw, 0, sizeof(ausNewGw));
    memset(acDevName, 0, sizeof(acDevName));
    memset(acSrc, 0, sizeof(acSrc));
    memset(acDes, 0, sizeof(acDes));
    memset(acGw, 0, sizeof(acGw));
    memset(acNewGw, 0, sizeof(acNewGw));

    /* 1. only chk ipv6 gateway                                                 */
    iRet = inet_pton(AF_INET6, pcGateWay, ausNewGw);
    if (iRet <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "inet_pton <= 0");
        goto ERR_LABEL;
    }

    /* 1.1 add ":" in route format                                              */
    memset(acNewGw, 0, sizeof(acNewGw));
    snprintf(acNewGw, sizeof(acNewGw), "%04x%04x%04x%04x%04x%04x%04x%04x",
             htons(ausNewGw[0]), htons(ausNewGw[1]), htons(ausNewGw[2]), htons(ausNewGw[3]),
             htons(ausNewGw[4]), htons(ausNewGw[5]), htons(ausNewGw[6]), htons(ausNewGw[7]));

    /* 2. chk /proc/net/if_inet6 whether we write in                            */
    fp = fopen(DEF_PATH_PROCNET_IPV6_ROUTE, "r");
    if (fp == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error", DEF_PATH_PROCNET_IFINET6);
        goto ERR_LABEL;
    }

    while ((iRet = fscanf(fp, "%s %x %s %x %s %x %x %x %x %s",
                          acDes, &lDstPrefix, acSrc, &lSrcPrefix, acGw,
                          &ulMetrix, &ulRefCnt, &ulUsrCnt, &ulFlag, acDevName)) != EOF) {
        /*return value must be parameter's number                               */
        if (iRet != 10) {
            continue;
        }

        if (0 != strncasecmp(acDevName, pcEth, strlen(pcEth))) {
            continue;
        }

        if (0 == strncasecmp(acDes, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acSrc, DEF_IPV6_ROUTE_DEF, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == strncasecmp(acGw, acNewGw, strlen(DEF_IPV6_ROUTE_DEF)) &&
            0 == lDstPrefix) {
            LOG_WRITE(EN_LOG_DEBUG, "system def gateway exit");
            goto OK_LABEL;
        }

    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;

OK_LABEL:
    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;
}

int
UTIL_GetIpV4(char *pcEth, char *pcIP)
{
    int iSockFd = -1;
    struct sockaddr_in stSin;
    struct ifreq stIfr;

    if (NULL == pcEth || NULL == pcIP) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    memset(&stSin, 0, sizeof(stSin));
    memset(&stIfr, 0, sizeof(stIfr));

    iSockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSockFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iSockFd < 0");
        goto ERR_LABEL;
    }

    strncpy(stIfr.ifr_name, pcEth, IFNAMSIZ);
    stIfr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(iSockFd, SIOCGIFADDR, &stIfr) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "ioctl < 0");
        goto ERR_LABEL;
    }

    memcpy(&stSin, &stIfr.ifr_addr, sizeof(stSin));
    memcpy(pcIP, inet_ntoa(stSin.sin_addr), DEF_UTIL_IPV4_MAX_LEN);

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_ERR;
}

int
UTIL_SetIpV4(char *pcEth, char *pcIp, char *pcNetmask, char *pcGateway)
{
    int iRet = OPLK_ERR;
    char acCmdLine[DEF_BUF_SIZE_256] = {0};

    if (NULL == pcEth || NULL == pcIp || NULL == pcNetmask || NULL == pcGateway) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "config ipv4 %s ip %s %s %s", pcEth, pcIp, pcNetmask,
              pcGateway);

    if (OPLK_ERR == UTIL_ChkIpV4Addr(pcIp)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net ip %s error", pcIp);
        goto ERR_LABEL;
    }

    if (OPLK_ERR == UTIL_ChkNetmask(pcNetmask)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net netmask %s error", pcNetmask);
        goto ERR_LABEL;
    }

    if (OPLK_ERR == UTIL_ChkIpV4Addr(pcGateway)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net gateway %s error", pcGateway);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0x00, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine),
             "ifconfig %s %s netmask %s 1>/dev/null 2>&1",
             pcEth, pcIp, pcNetmask);
    iRet = UTIL_System(acCmdLine);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "set network %s %s %s error %x", pcEth, pcIp, pcNetmask, iRet);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "%s", acCmdLine);

    if (OPLK_OK != __UTIL_SetIpv4Gateway(pcEth, pcGateway)) {
        LOG_WRITE(EN_LOG_ERROR, "set gateway %s %s error", pcEth, pcGateway);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SetIpV6(char *pcEth, char *pcIp, uint32_t ulPrefix, char *pcGateway)
{
    if (NULL == pcEth || NULL == pcIp || NULL == pcGateway) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "config ipv6 %s ip %s %d %s", pcEth, pcIp, ulPrefix,
              pcGateway);

    /* Set the config.                                                          */
    if (__UTIL_SetIpv6Addr(pcEth, pcIp, ulPrefix) != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set ipv6 addr error");
        goto ERR_LABEL;
    }

    if (OPLK_OK != __UTIL_SetIpv6Gateway(pcEth, pcGateway)) {
        LOG_WRITE(EN_LOG_ERROR, "set ipv6 gateway error");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_GetNetmask(char *pcEth, uint32_t *pulAddr)
{
    int iSockFd = -1;
    struct sockaddr_in stSin;
    struct ifreq stIfr;

    if (NULL == pcEth || NULL == pulAddr) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    iSockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSockFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iSockFd < 0");
        goto ERR_LABEL;
    }

    strncpy(stIfr.ifr_name, pcEth, IFNAMSIZ);
    stIfr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(iSockFd, SIOCGIFNETMASK, &stIfr) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "ioctl < 0");
        goto ERR_LABEL;
    }

    memcpy(&stSin, &stIfr.ifr_netmask, sizeof(stSin));
    *pulAddr = htonl(stSin.sin_addr.s_addr);

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_ERR;
}

int
UTIL_GetGateway(char *pcEth, char *pcGateway)
{
    FILE *fp = NULL;
    char acBuf[DEF_BUF_SIZE_256] = {0};
    char acIface[DEF_BUF_SIZE_256] = {0};
    uint32_t ulDestAddr = 0;
    uint32_t ulGateway = 0;

    if (NULL == pcEth || NULL == pcGateway) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcGateway");
        goto ERR_LABEL;
    }

    fp = fopen("/proc/net/route", "r");
    if (NULL == fp) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == fp");
        goto ERR_LABEL;
    }

    /* Skip title line                                                          */
    memset(acBuf, 0x00, sizeof(acBuf));
    if (fgets(acBuf, sizeof(acBuf), fp)) {
    }

    while (fgets(acBuf, sizeof(acBuf), fp)) {
        if (sscanf(acBuf, "%s\t%X\t%X", acIface, &ulDestAddr, &ulGateway) != 3
            || ulDestAddr != 0) {
            memset(acBuf, 0x00, sizeof(acBuf));
            continue;
        }
        if (0 == strcasecmp(acIface, pcEth)) {
            break;
        }
    }
    memcpy(acBuf, &ulGateway, sizeof(uint32_t));

    if (0 == strcasecmp(acIface, pcEth)) {
        sprintf(pcGateway, "%u.%u.%u.%u",
                acBuf[0], acBuf[1], acBuf[2], acBuf[3]);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "0 == strcasecmp");
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(fp);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FCLOSE(fp);
    return OPLK_ERR;
}

int
UTIL_GetMacAddr(char *pcEth, char *pcMacAddr)
{
    int iSockFd = -1;
    struct ifreq ifr;
    char acTmpBuf[DEF_BUF_SIZE_32] = {0};

    if (NULL == pcEth || NULL == pcMacAddr) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        goto ERR_LABEL;
    }

    /*create socket                                                             */
    iSockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSockFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iSockFd < 0");
        goto ERR_LABEL;
    }

    memset(&ifr, 0x00, sizeof(ifr));
    strncpy(ifr.ifr_name, pcEth, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    /*get ethernet mac address                                                  */
    if (ioctl(iSockFd, SIOCGIFHWADDR, &ifr) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "ioctl < 0");
        goto ERR_LABEL;
    }

    /*copy mac addrese to hwaddr                                                */
    memcpy(acTmpBuf, ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data));
    memcpy(pcMacAddr, acTmpBuf, 6);

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_CLOSE(iSockFd);
    return OPLK_ERR;
}

int
UTIL_SetMacAddr(char *pcEth, char *pcMacAddr)
{
    char acCmdLine[DEF_BUF_SIZE_256] = {0};
    int iRet = OPLK_ERR;

    if (NULL == pcEth || NULL == pcMacAddr) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        return OPLK_ERR;
    }

    memset(acCmdLine, 0x00, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "ifconfig %s hw ether %s", pcEth, pcMacAddr);

    iRet = UTIL_System(acCmdLine);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "%s fail, Ret %x", acCmdLine, iRet);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "config %s mac %s", pcEth, pcMacAddr);

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_TransIpAddr(char *pcIpaddr, uint32_t *pulAddr)
{
    struct in_addr stAddr;

    if (NULL == pcIpaddr || NULL == pulAddr) {
        goto ERR_LABEL;
    }

    if (0 == inet_aton(pcIpaddr, &stAddr)) {
        goto ERR_LABEL;
    }

    *pulAddr = ntohl(stAddr.s_addr);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}
/* OPLK_OK: NOT in same segment */
/* OPLK_ERR: error or in same segment */
int
UTIL_IsNetworkSegmentSame(char *pcIpaddr1, char *pcNetmask1, char *pcIpaddr2, char *pcNetmask2)
{
    int iResult = OPLK_OK;
    uint32_t ulIpAddr1 = 0;
    uint32_t ulIpAddr2 = 0;
    uint32_t ulNetMaskTmp1 = 0;
    uint32_t ulNetMaskTmp2 = 0;
    uint32_t ulNetMaskMin = 0;

    if (NULL == pcIpaddr1 || NULL == pcIpaddr2) {
        goto ERR_LABEL;
    }

    iResult = UTIL_TransIpAddr(pcIpaddr1, &ulIpAddr1);
    if (iResult != OPLK_OK) {
        goto ERR_LABEL;
    }

    iResult = UTIL_TransIpAddr(pcNetmask1, &ulNetMaskTmp1);
    if (iResult != OPLK_OK) {
        goto ERR_LABEL;
    }

    iResult = UTIL_TransIpAddr(pcIpaddr2, &ulIpAddr2);
    if (iResult != OPLK_OK) {
        goto ERR_LABEL;
    }
    iResult = UTIL_TransIpAddr(pcNetmask2, &ulNetMaskTmp2);
    if (iResult != OPLK_OK) {
        goto ERR_LABEL;
    }

    ulNetMaskMin = ulNetMaskTmp1 > ulNetMaskTmp2 ? ulNetMaskTmp2 : ulNetMaskTmp1;

    if ((ulIpAddr1 & ulNetMaskMin) == (ulIpAddr2 & ulNetMaskMin)) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

/* OPLK_OK: NOT in same segment */
/* OPLK_ERR: error or in same segment */

int
UTIL_IsIpv6NetworkSegmentSame(char *pcIpaddr1, uint8_t ucPrefixLen1, char *pcIpaddr2, uint8_t ucPrefixLen2)
{
    int iResult = OPLK_OK;
    uint8_t aucIpaddr1[16];
    uint8_t aucIpaddr2[16];
    uint8_t ucPrefixLenMin = 0;
    uint8_t ucBytes = 0;
    uint8_t ucBits = 0;
    uint8_t i = 0;

    if (NULL == pcIpaddr1 || NULL == pcIpaddr2 || ucPrefixLen1 > DEF_IPV6_PRE_FIX_MAX_LEN || ucPrefixLen2 > DEF_IPV6_PRE_FIX_MAX_LEN) {
        goto ERR_LABEL;
    }

    memset(aucIpaddr1, 0, sizeof(aucIpaddr1));
    iResult = inet_pton(AF_INET6, pcIpaddr1, aucIpaddr1);
    if (iResult <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6: %s", pcIpaddr1);
        goto ERR_LABEL;
    }

    memset(aucIpaddr2, 0, sizeof(aucIpaddr2));
    iResult = inet_pton(AF_INET6, pcIpaddr2, aucIpaddr2);
    if (iResult <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6: %s", pcIpaddr2);
        goto ERR_LABEL;
    }

    ucPrefixLenMin = (ucPrefixLen1 > ucPrefixLen2) ? ucPrefixLen2 : ucPrefixLen1;
    if (0 == ucPrefixLenMin) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid prefixlen, ucPrefixLen1: %d, ucPrefixLen2: %d", ucPrefixLen1, ucPrefixLen2);
        goto ERR_LABEL;
    }

    ucBytes = ucPrefixLenMin / 8;
    ucBits = ucPrefixLenMin % 8;

    for (i = 0; i < ucBytes; i++) {
        if (aucIpaddr1[i] != aucIpaddr2[i]) {
            break;
        }
    }

    if (i >= ucBytes) {
        for (i = 0; i < ucBits; i++) {
            if ((aucIpaddr1[ucBytes] & (0x1 << (7 - i))) != (aucIpaddr2[ucBytes] & (0x1 << (7 - i)))) {
                break;
            }
        }

        if (i >= ucBits) {
            LOG_WRITE(EN_LOG_ERROR, "Same network, i:%d, ucBytes:%d, ucBits: %d", i, ucBytes, ucBits);
            goto ERR_LABEL;
        }
    }

    /* they are different network segments */
    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}


/* pcIpv6 format: <ipv6-addr>/<prefix>                                                            */
int
UTIL_SeparateIpv6AddrAndPrefix(char *pcIpv6, char *pcAddr, size_t nAddrLen, uint8_t *pucPrefix)
{
    char *pcTmp = NULL;
    size_t nLen = 0, i = 0;
    int iPrefix = 0;

    if (NULL == pcIpv6 || NULL == pcAddr || NULL == pucPrefix) {
        goto ERR_LABEL;
    }

    pcTmp = strchr(pcIpv6, '/');
    if (NULL == pcTmp) {
        goto ERR_LABEL;
    }

    nLen = pcTmp - pcIpv6;

    if (nLen >= nAddrLen) {
        goto ERR_LABEL;
    }

    strncpy(pcAddr, pcIpv6, nLen);
    *(pcAddr + nLen) = '\0';

    if (OPLK_OK != UTIL_ChkIpV6Addr(pcAddr)) {
        goto ERR_LABEL;
    }

    pcTmp++;

    nLen = strlen(pcTmp);
    if (nLen < 1 || nLen > 3) {
        goto ERR_LABEL;
    }

    for (i = 0; i < nLen; i++) {
        if (0 == isdigit(pcTmp[i])) {
            goto ERR_LABEL;
        }
    }

    iPrefix = atoi(pcTmp);
    if (iPrefix < 0 || iPrefix > 128) {
        goto ERR_LABEL;
    }

    *pucPrefix = (uint8_t)iPrefix;

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_SetVlan                                                                          */
/* Descrp : Set vlan in Ethx, such as add vlan id 0x102 in eth0                                   */
/* Input  : pcEthName -- A pointer of Eth name                                                    */
/*        : pcVlanId1 -- A pointer of vlan id 1 string, example 0x112                             */
/*        : pcVlanId2 -- A pointer of vlan id 1 string, example 0x122                             */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_SetVlan(char *pcEthName, char *pcVlanId1, char *pcVlanId2)
{
    char acCmd[DEF_BUF_SIZE_256] = {0};
    int rc = OPLK_ERR;

    /* Check input parameter                                                    */
    if (NULL == pcEthName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: %p", pcEthName);
        goto ERR_LABEL;
    }

    /* Add vlan id 1                                                            */
    if (pcVlanId1 != NULL) {
        /* Delete the vlan id 1                                                 */
        memset(acCmd, 0x00, sizeof(acCmd));
        snprintf(acCmd, sizeof(acCmd) - 1, "ip link del %s.1", pcEthName);
        UTIL_System(acCmd);

        /*  Add vlan id 1                                                       */
        memset(acCmd, 0x00, sizeof(acCmd));
        snprintf(acCmd, sizeof(acCmd), "ip link add link %s name %s.1 type vlan id %s", pcEthName, pcEthName, pcVlanId1);
        rc = UTIL_System(acCmd);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by: %s", acCmd);
            goto ERR_LABEL;
        }
    }

    /* Add vlan id 2                                                            */
    if (pcVlanId2 != NULL) {
        /* Delete the vlan id 1                                                 */
        memset(acCmd, 0x00, sizeof(acCmd));
        snprintf(acCmd, sizeof(acCmd) - 1, "ip link del %s.2", pcEthName);
        UTIL_System(acCmd);

        /*  Add vlan id 1                                                       */
        memset(acCmd, 0x00, sizeof(acCmd));
        snprintf(acCmd, sizeof(acCmd), "ip link add link %s name %s.2 type vlan id %s", pcEthName, pcEthName, pcVlanId2);
        rc = UTIL_System(acCmd);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by: %s", acCmd);
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_ChkIpv4EthNetCfg                                                                 */
/* Descrp : Check the legality of network port configuration parameters                           */
/* Input  : pcIp                                                                                  */
/*        : pcNetmask                                                                             */
/*        : pcGateway -- If pcGateway is NULL, ignore check gateway                               */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_ChkIpv4EthNetCfg(char *pcIp, char *pcNetmask, char *pcGateway)
{
    uint32_t ulIp = 0;
    uint32_t ulNetmask = 0;
    uint32_t ulGateway = 0;

    if (NULL == pcIp || NULL == pcNetmask) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        return OPLK_ERR;
    }

    if (OPLK_OK != UTIL_ChkIpV4Addr(pcIp)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net ip %s error", pcIp);
        goto ERR_LABEL;
    }

    if (OPLK_OK != UTIL_ChkNetmask(pcNetmask)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net netmask %s error", pcNetmask);
        goto ERR_LABEL;
    }

    /* ignore check gateway */
    if (NULL == pcGateway) {
        return OPLK_OK;
    }

    if (OPLK_OK != UTIL_ChkIpV4Addr(pcGateway)) {
        goto ERR_LABEL;
    }

    if (OPLK_OK != UTIL_TransIpAddr(pcIp, &ulIp)) {
        goto ERR_LABEL;
    }

    if (OPLK_OK != UTIL_TransIpAddr(pcNetmask, &ulNetmask)) {
        goto ERR_LABEL;
    }

    if (OPLK_OK != UTIL_TransIpAddr(pcGateway, &ulGateway)) {
        goto ERR_LABEL;
    }

    /* Check if the IP and gateway are on the same network */
    if ((ulIp & ulNetmask) != (ulGateway & ulNetmask)) {
        LOG_WRITE(EN_LOG_ERROR, "IP and gateway are not in the same network segment. ip:%s, netmask: %s, gateway: %s", pcIp, pcNetmask, pcGateway);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_ChkIpv6EthNetCfg                                                                 */
/* Descrp : Check the legality of network port configuration parameters                           */
/* Input  : pcIp                                                                                  */
/*        : pcNetmask                                                                             */
/*        : pcGateway -- If pcGateway is NULL, ignore check gateway                               */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_ChkIpv6EthNetCfg(char *pcIp, uint8_t ucPrefixlen, char *pcGateway)
{
    if (NULL == pcIp) {
        LOG_WRITE(EN_LOG_ERROR, "input para error");
        return OPLK_ERR;
    }

    if (OPLK_OK != UTIL_ChkIpV6Addr(pcIp)) {
        LOG_WRITE(EN_LOG_ERROR, "chk net ipv6 %s error", pcIp);
        goto ERR_LABEL;
    }

    if (0 == strncasecmp("FE80", pcIp, strlen("FE80"))) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 FE80::/16 can not use");
        return OPLK_ERR;
    }

    if (0 == strncasecmp("FF00", pcIp, 2)) {
        LOG_WRITE(EN_LOG_ERROR, "ipv6 FF00::/8 can not use");
        return OPLK_ERR;
    }

    if (OPLK_OK != UTIL_ChkIpv6Prefix(ucPrefixlen) || 0 == ucPrefixlen) {
        LOG_WRITE(EN_LOG_ERROR, "chk net prefixlen %d error", ucPrefixlen);
        goto ERR_LABEL;
    }

    /* ignore check gateway */
    if (NULL == pcGateway) {
        return OPLK_OK;
    }

    if (OPLK_OK != UTIL_ChkIpV6Addr(pcGateway)) {
        goto ERR_LABEL;
    }

    /* Check if the IP and gateway are on the same network */
    if (OPLK_OK == UTIL_IsIpv6NetworkSegmentSame(pcIp, ucPrefixlen, pcGateway, ucPrefixlen)) {
        LOG_WRITE(EN_LOG_ERROR, "IP and gateway are not in the same network segment. ipv6:%s, prefixlen: %s, gateway: %s", pcIp, ucPrefixlen, pcGateway);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

