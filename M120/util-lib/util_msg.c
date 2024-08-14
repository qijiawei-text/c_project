/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_msg.c
* Description:   This file provides common message interface for other processes(tl1,scm,snmp etc) to send
*                    OOP message to Monitor process through Unix socket message, it will be compiled to libmsg.so
* Author:        Subsystem Dev team
* History:
*                [Author]    [Date]         [Version]    [Description]
*                Yuan Xia   2013/11/06   Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_msg.h"


static void
__UTIL_CtorMsgSrv(struct sockaddr_un *pSrvAddr, char *pcPath)
{
    if (NULL == pSrvAddr || NULL == pcPath) {
        printf("input NULL == pSrvAddr || NULL == pcPath");
        return;
    }

    memset(pSrvAddr, 0x00, sizeof(struct sockaddr_un));
    pSrvAddr->sun_family = AF_UNIX;
    strncpy(pSrvAddr->sun_path, pcPath, sizeof(pSrvAddr->sun_path) - 1);

    return;
}

static int
__UTIL_SendMsg(int iFd, char *pcMsgBuf, int32_t lBufLen,
               struct sockaddr_un *pSrvAddr)
{
    int32_t lBytesSent = 0;
    int32_t lLen = 0;

    if (NULL == pcMsgBuf || NULL == pSrvAddr) {
        printf("NULL == pcMsgBuf || NULL == pSrvAddr");
        goto ERR_LABEL;
    }

    lLen = offsetof(struct sockaddr_un, sun_path) + strlen(pSrvAddr->sun_path);

    lBytesSent = sendto(iFd, pcMsgBuf, lBufLen, 0, (struct sockaddr *)pSrvAddr, lLen);
    if (lBytesSent != lBufLen) {
        /* printf("Error: data lost, expected datalen %d, acutal datalen is %d. iFd=%d. error=%s\n", lBufLen, lBytesSent, iFd, strerror(errno)); */
        /* perror("send msg sendto message failure" ); */
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_CreateMsgSock(char *pcPath)
{
    int iFd = -1;
    struct sockaddr_un sockAddr;

    if (NULL == pcPath) {
        printf("NULL == pcPath");
        goto ERR_LABEL;
    }

    __UTIL_CtorMsgSrv(&sockAddr, pcPath);

    iFd = socket(AF_UNIX, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    /* iFd = socket(AF_UNIX, SOCK_DGRAM, 0); */
    if (iFd < 0) {
        printf("socket error=%s\n", strerror(errno));
        goto ERR_LABEL;
    }

    unlink(pcPath);
    if (0 > bind(iFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr))) {
        printf("bind error=%s\n", strerror(errno));
        goto ERR_LABEL;
    }

    return iFd;

ERR_LABEL:

    DEF_SAFE_CLOSE(iFd);

    return OPLK_ERR;
}

void
UTIL_CloseMsgSock(char *pcPath, int iFd)
{
    char acCmd[DEF_BUF_SIZE_128] = {0};
    int iRet = -1;

    unlink(pcPath);
    DEF_SAFE_CLOSE(iFd);

    /*rm the socket file*/
    snprintf(acCmd, DEF_BUF_SIZE_128, "rm -rf %s", pcPath);
    iRet = UTIL_System(acCmd);
    if (OPLK_OK != iRet) {
        printf("system cmd error:%s\r\n", acCmd);
        return;
    }

    return;
}

int
UTIL_SendAndRecvOopMsg(int iFd, char *pcMsgBuf, int32_t lBufLen,
                       struct sockaddr_un *pSrvAddr,
                       char *pcRecvBuf, int32_t *plRecvLen)
{
    int32_t lBytesSent = 0;
    int32_t lLen = 0;
    struct timeval stTimeout;
    fd_set readFds;
    int iRet = OPLK_ERR;
    uint32_t ulRecvLen = 0;

    if (NULL == pcMsgBuf || NULL == pSrvAddr || NULL == pcRecvBuf || NULL == plRecvLen) {
        printf("Error: input parameter error\r\n");
        goto ERR_LABEL;
    }

    lLen = offsetof(struct sockaddr_un, sun_path) + strlen(pSrvAddr->sun_path);

    /* send data out */
    lBytesSent = sendto(iFd, pcMsgBuf, lBufLen, 0, (struct sockaddr *)pSrvAddr,
                        lLen);

    if (lBytesSent != lBufLen) {
        /* printf("Error: data lost, expected datalen %d, acutal datalen is %d\r\n", lBufLen, lBytesSent); */
        goto ERR_LABEL;
    }

    FD_ZERO(&readFds);
    FD_SET(iFd, &readFds);
    stTimeout.tv_sec = DEF_OOP_MSG_TIME_OUT;
    stTimeout.tv_usec = 0;

    iRet = select(iFd + 1, &readFds, NULL, NULL, &stTimeout);
    if (0 >= iRet) {
        goto ERR_LABEL;
    }

    /* select return > 0, receive response from server */
    if (FD_ISSET(iFd, &readFds)) {
        ulRecvLen = *plRecvLen;
        *plRecvLen = recvfrom(iFd, pcRecvBuf, ulRecvLen, MSG_WAITALL, NULL, NULL);
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


int
UTIL_SendOopMsg(int iFd, char *pcMsgBuf, int32_t lBufLen, char *pcPath)
{
    int iRet = OPLK_ERR;
    struct sockaddr_un stSrvAddr;

    if (NULL == pcMsgBuf || NULL == pcPath) {
        printf("Error: input parameter error\r\n");
        goto ERR_LABEL;
    }

    if (0 > lBufLen) {
        printf("Error: input parameter error\r\n");
        goto ERR_LABEL;
    }

    __UTIL_CtorMsgSrv(&stSrvAddr, pcPath);

    iRet = __UTIL_SendMsg(iFd, pcMsgBuf, lBufLen, &stSrvAddr);
    if (OPLK_OK != iRet) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static uint8_t
__UTIL_CalcMsgSum(char *pcMsgBuf, int32_t lBufLen)
{
    int32_t lIndex = 0;
    uint8_t ucCheckSum = 0;

    if (NULL == pcMsgBuf) {
        printf("Error: input parameter error");
        return 0xFF;
    }

    for (lIndex = 0; lIndex < lBufLen; lIndex++) {
        ucCheckSum += pcMsgBuf[lIndex];
    }

    return 0xFF - ucCheckSum;
}

int
UTIL_ChkOopMsg(char *pcMsgBuf, int32_t lBufLen)
{
    ST_OOP_MSG_HEADER *pstHeader = NULL;

    if (NULL == pcMsgBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input parameter error");
        goto ERR_LABEL;
    }

    pstHeader = (ST_OOP_MSG_HEADER *)pcMsgBuf;
    if (ntohs(pstHeader->usPayloadLen) + sizeof(ST_OOP_MSG_HEADER) + 1 != lBufLen) {
        LOG_WRITE(EN_LOG_ERROR, "input lBufLen error");
        goto ERR_LABEL;
    }

    if ((uint8_t)pcMsgBuf[lBufLen - 1] != __UTIL_CalcMsgSum(pcMsgBuf, lBufLen - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "__UTIL_CalcMsgSum error");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int32_t
UTIL_CreateOopMsg(char *pcOopMsg, uint32_t ulSyncHdr, uint8_t ucSyncNo,
                  uint8_t ucSlotId,
                  uint32_t ulOid, uint16_t usMsgStat, char *pcPayload, uint16_t usPayloadSize)
{
    int32_t lMsgLen = 0;
    char *pcCheckSum = NULL;
    ST_OOP_MSG_HEADER *pstMsgHeader = NULL;

    if (NULL == pcOopMsg || usPayloadSize > (DEF_OOP_MSG_MAX_LEN - sizeof(ST_OOP_MSG_HEADER) - 1)) {
        printf("Error: input parameter error");
        goto ERR_LABEL;
    }

    if (NULL == pcPayload && usPayloadSize != 0) {
        printf("Error: input PayloadSize error");
        goto ERR_LABEL;
    }

    pstMsgHeader = (ST_OOP_MSG_HEADER *)pcOopMsg;
    memset(pstMsgHeader, 0x00, sizeof(ST_OOP_MSG_HEADER));

    pstMsgHeader->ulSyncHeader = htonl(ulSyncHdr);
    pstMsgHeader->ucProtoVer = DEF_OOP_MSG_CURRENT_VER;
    pstMsgHeader->ucSyncNo = ucSyncNo;
    pstMsgHeader->ucSlotId = ucSlotId;
    pstMsgHeader->ulOid = htonl(ulOid);
    pstMsgHeader->usMsgStat = htons(usMsgStat);
    pstMsgHeader->usPayloadLen = htons(usPayloadSize);

    if (pcPayload != NULL && usPayloadSize > 0) {
        memcpy(pcOopMsg + sizeof(ST_OOP_MSG_HEADER), pcPayload, usPayloadSize);
    }

    pcCheckSum = pcOopMsg + sizeof(ST_OOP_MSG_HEADER) + usPayloadSize;
    *pcCheckSum = __UTIL_CalcMsgSum(pcOopMsg, sizeof(ST_OOP_MSG_HEADER) + usPayloadSize);

    lMsgLen = sizeof(ST_OOP_MSG_HEADER) + usPayloadSize + 1;

    return lMsgLen;

ERR_LABEL:

    return OPLK_ERR;
}


