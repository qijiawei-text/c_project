/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_event.c
* Description:   event
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                dongyangf  2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_event.h"


static pthread_t m_tEventThread;
static bool m_bEventExit = false;

static void *
__UTIL_EventLoopThread(void *arg)
{
    int iRet = OPLK_ERR;
    int iFd = -1;
    fd_set iReadFds;
    struct timeval stTimeout;
    int iLen = 0;
    char acRecvBuf[DEF_OOP_MSG_MAX_LEN];
    ST_EVENT_ARG stEventArg;
    ST_OOP_MSG_HEADER *pstMsgBuf = NULL;

    if (NULL == arg) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == arg");
        goto ERR_LABEL;
    }

    memset(&stEventArg, 0, sizeof(stEventArg));
    memcpy(&stEventArg, arg, sizeof(stEventArg));

    prctl(PR_SET_NAME, "event_pthread");
    LOG_WRITE(EN_LOG_NOTICE, "Socket fd %d, fp %p", stEventArg.iFd, stEventArg.fpEventCb);
    iFd = stEventArg.iFd;
    if (stEventArg.iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iFd error");
        goto ERR_LABEL;
    }

    while (!m_bEventExit) {
        FD_ZERO(&iReadFds);
        FD_SET(iFd, &iReadFds);

        stTimeout.tv_sec = 30;
        stTimeout.tv_usec = 0;

        iRet = select(iFd + 1, &iReadFds, NULL, NULL, &stTimeout);
        if (0 > iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_EventLoopThread select error : %d", iRet);
            goto ERR_LABEL;
        } else if (0 == iRet) {
            continue;     /*timeout*/
        }

        /*select return > 0*/
        /* if received message from command socket */
        if (FD_ISSET(iFd, &iReadFds)) {
            memset(acRecvBuf, 0, sizeof(acRecvBuf));
            iLen = recvfrom(iFd, acRecvBuf, DEF_OOP_MSG_MAX_LEN, MSG_WAITALL, NULL, NULL);
            if (iLen > 0) {
                if (OPLK_OK != UTIL_ChkOopMsg(acRecvBuf, iLen)) {
                    LOG_WRITE(EN_LOG_ERROR, "Recv OOP msg check sum error");
                }

                pstMsgBuf = (ST_OOP_MSG_HEADER *)&(acRecvBuf[0]);
                switch (ntohl(pstMsgBuf->ulOid)) {
                case DEF_OID_DEV_EVENT:
                {
                    if (OPLK_OK != stEventArg.fpEventCb(*((int *)(acRecvBuf + sizeof(ST_OOP_MSG_HEADER))),
                                                        *((int *)(acRecvBuf + sizeof(ST_OOP_MSG_HEADER) + sizeof(int))))) {
                        LOG_WRITE(EN_LOG_ERROR, "event cb error");
                    }

                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
    }

ERR_LABEL:

    pthread_join(m_tEventThread, NULL);
    LOG_WRITE(EN_LOG_NOTICE, "__UTIL_EventLoop exit");

    return NULL;
}

int
UTIL_RegEventCb(const char *pcSoketPath, EventCb fpEventCb)
{
    int iFd = -1;
    ST_EVENT_ARG stEventArg;

    if (NULL == pcSoketPath || NULL == fpEventCb) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcSoketPath fpEventCb");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Start dev event thread!");

    iFd = UTIL_CreateMsgSock((char *)pcSoketPath);
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_MSGCreateSock fail!");
        goto ERR_LABEL;
    }

    memset(&stEventArg, 0, sizeof(stEventArg));
    stEventArg.iFd = iFd;
    stEventArg.fpEventCb = fpEventCb;

    LOG_WRITE(EN_LOG_NOTICE, "Socket iFd %d, fp %p, path %s", stEventArg.iFd, stEventArg.fpEventCb, pcSoketPath);
    if (0 != pthread_create(&m_tEventThread, NULL, __UTIL_EventLoopThread, (void *)&stEventArg)) {
        LOG_WRITE(EN_LOG_ERROR, "Start event thread fail!");
        goto ERR_LABEL;
    }

    /* pthread_join(m_tEventThread, NULL); */
    DEF_SLEEP_S(1);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


void
UTIL_UnRegEventCb()
{
    m_bEventExit = true;

    if (m_tEventThread > 0) {
        pthread_cancel(m_tEventThread);
    }

    LOG_WRITE(EN_LOG_TRACE, "event thread exit!");
}

int
UTIL_SendEvent(int iFd, char *pcDestPath, int iEventId, int iSubEventId)
{
    int iRet = OPLK_ERR;
    int32_t lLen;
    const int32_t lEventLens = sizeof(int) * 2;
    char acEventId[lEventLens];
    char acBuffer[DEF_OOP_MSG_MAX_LEN] = {0};

    if (iFd < 0 || NULL == pcDestPath) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcDestPath, iFd = %d", iFd);
        iRet = OPLK_ERR;
        goto cleanup;
    }

    memcpy(acEventId, &iEventId, sizeof(int));
    memcpy(acEventId + sizeof(int), &iSubEventId, sizeof(int));

    lLen = UTIL_CreateOopMsg(acBuffer, DEF_OOP_MSG_TYPE_REQ, 0, 0, DEF_OID_DEV_EVENT, 0, (char *)&acEventId, lEventLens);
    if (0 > lLen) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateOopMsg failure %d", lLen);
        iRet = OPLK_ERR;
        goto cleanup;
    }

    iRet = UTIL_SendOopMsg(iFd, acBuffer, lLen, pcDestPath);
    if (OPLK_OK != iRet) {
/*        LOG_WRITE(EN_LOG_ERROR, "send event to %s failure", pcDestPath); */
    }

cleanup:

    return iRet;
}


