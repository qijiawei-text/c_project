/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_queue.c
* Description:   msg queue
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_queue.h"

static pthread_t m_tQueueThread;
static bool m_bQueueThreadExit = false;
static int m_iQueueFd = -1;

#define   DEF_MSG_QUEUE_NUM                (4)

typedef struct {
    uint32_t ulPosIn;                                        /* the position at which to be put message                */
    uint32_t ulPosOut;                                       /* the position at which to be pop out message            */
    uint32_t ulNum;                                          /* the count of unprocessed member                        */
    uint32_t ulMsgLen;                                       /* the queue message len                                  */

    uint32_t ulMaxSize;                                          /* the limit of queue member's number                     */
    uint32_t ulUsedFlag;                                         /* Is it used */
    char *pcMsgInfor;
    pthread_mutex_t stMutexLock;                             /* Mutex lock.    */
} ST_UTIL_QUEUE;

static ST_UTIL_QUEUE m_stMsgQueue[DEF_MSG_QUEUE_NUM] = {
    {0, 0, 0, 0, 0, false, NULL, {}},
    {0, 0, 0, 0, 0, false, NULL, {}},
    {0, 0, 0, 0, 0, false, NULL, {}},
    {0, 0, 0, 0, 0, false, NULL, {}}
};


int8_t
UTIL_InitQueue(uint32_t ulMsgLen, uint32_t ulSize)
{
    uint8_t ucIndex = 0;
    char *pcBuf = NULL;

    if ((ulMsgLen > sizeof(ST_UTIL_QUEUE_MSG)) || (ulSize > DEF_QUEUE_NODE_MAX_SIZE)) {
        /* No spare queue */
        LOG_WRITE(EN_LOG_ERROR, "UTIL_InitQueue size too large MsgLen %u Size %u", ulMsgLen, ulSize);
        goto ERR_LABEL;
    }

    /* default value */
    if (0 == ulMsgLen) {
        ulMsgLen = sizeof(ST_UTIL_QUEUE_MSG);
    }

    if (0 == ulSize) {
        ulSize = DEF_QUEUE_NODE_MAX_SIZE;
    }

    for (ucIndex = 0; ucIndex < DEF_MSG_QUEUE_NUM; ucIndex++) {

        if ((true == m_stMsgQueue[ucIndex].ulUsedFlag) || (NULL != m_stMsgQueue[ucIndex].pcMsgInfor)) {
            /* the queue have been used */
            continue;
        }

        /* malloc buffer for queue, init queue and return the queue index */
        pcBuf = (char *)malloc(ulMsgLen * ulSize);
        if (NULL == pcBuf) {
            LOG_WRITE(EN_LOG_ERROR, "malloc pcBuf is NULL");
            goto ERR_LABEL;
        }

        m_stMsgQueue[ucIndex].pcMsgInfor = pcBuf;
        m_stMsgQueue[ucIndex].ulPosIn = 0;
        m_stMsgQueue[ucIndex].ulPosOut = 0;
        m_stMsgQueue[ucIndex].ulNum = 0;
        m_stMsgQueue[ucIndex].ulMsgLen = ulMsgLen;
        m_stMsgQueue[ucIndex].ulMaxSize = ulSize;
        m_stMsgQueue[ucIndex].ulUsedFlag = true;

        pthread_mutex_init(&m_stMsgQueue[ucIndex].stMutexLock, NULL);

        break;
    }

    if (ucIndex >= DEF_MSG_QUEUE_NUM) {
        /* No spare queue */
        LOG_WRITE(EN_LOG_ERROR, "No spare queue %d", ucIndex);
        goto ERR_LABEL;
    }


    return ucIndex + 1;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_CloseQueue(int8_t cIndex)
{
    uint8_t ucQueueIndex = 0;

    if (0 >= cIndex || cIndex > DEF_MSG_QUEUE_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstQueueMsg or queue index %d error", cIndex);
        goto ERR_LABEL;
    }

    ucQueueIndex = cIndex - 1;

    pthread_mutex_lock(&m_stMsgQueue[ucQueueIndex].stMutexLock);

    DEF_SAFE_FREE(m_stMsgQueue[ucQueueIndex].pcMsgInfor);
    m_stMsgQueue[ucQueueIndex].ulPosIn = 0;
    m_stMsgQueue[ucQueueIndex].ulPosOut = 0;
    m_stMsgQueue[ucQueueIndex].ulNum = 0;
    m_stMsgQueue[ucQueueIndex].ulMsgLen = 0;
    m_stMsgQueue[ucQueueIndex].ulMaxSize = 0;
    m_stMsgQueue[ucQueueIndex].ulUsedFlag = false;

    pthread_mutex_unlock(&m_stMsgQueue[ucQueueIndex].stMutexLock);
    pthread_mutex_destroy(&m_stMsgQueue[ucQueueIndex].stMutexLock);

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


int
UTIL_AddQueueNode(int8_t cIndex, char *pcMsg, uint32_t ulMsgLen)
{
    uint32_t ulPos = 0;
    uint8_t ucQueueIndex = 0;

    if (NULL == pcMsg || 0 >= cIndex || cIndex > DEF_MSG_QUEUE_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstQueueMsg or queue index %d error", cIndex);
        goto INPUT_ERR_LABEL;
    }

    ucQueueIndex = cIndex - 1;

    pthread_mutex_lock(&m_stMsgQueue[ucQueueIndex].stMutexLock);

    if (false == m_stMsgQueue[ucQueueIndex].ulUsedFlag) {
        LOG_WRITE(EN_LOG_ERROR, "queue used flag error %d", m_stMsgQueue[ucQueueIndex].ulUsedFlag);
        goto ERR_LABEL;
    }

    if (m_stMsgQueue[ucQueueIndex].ulMsgLen < ulMsgLen) {
        LOG_WRITE(EN_LOG_ERROR, "queue define MsgLen %d < add MsgLen %d", m_stMsgQueue[ucQueueIndex].ulMsgLen, ulMsgLen);
        goto ERR_LABEL;
    }

    if (NULL == m_stMsgQueue[ucQueueIndex].pcMsgInfor) {
        LOG_WRITE(EN_LOG_ERROR, "m_stMsgQueue MsgInfor is NULL");
        goto ERR_LABEL;
    }

    if (m_stMsgQueue[ucQueueIndex].ulNum >= m_stMsgQueue[ucQueueIndex].ulMaxSize) {
        /* if ulNum >= MON_QUEUE_NODE_MAX_SIZE, the msg will be discarded */
        /* printf("add queue num error %d\r\n", m_stLogMsgQueue.ulNum); */
        LOG_WRITE(EN_LOG_ERROR, "add queue num error %d", m_stMsgQueue[ucQueueIndex].ulNum);
        goto ERR_LABEL;
    } else {
        m_stMsgQueue[ucQueueIndex].ulNum++;
    }

    if (m_stMsgQueue[ucQueueIndex].ulPosIn >= m_stMsgQueue[ucQueueIndex].ulMaxSize) {
        /* if set inpos is end of queue, then set inpos to be first.            */
        m_stMsgQueue[ucQueueIndex].ulPosIn = 0;
    }

    ulPos = m_stMsgQueue[ucQueueIndex].ulPosIn * m_stMsgQueue[ucQueueIndex].ulMsgLen;
    memcpy(m_stMsgQueue[ucQueueIndex].pcMsgInfor + ulPos, pcMsg, ulMsgLen);
    m_stMsgQueue[ucQueueIndex].ulPosIn++;

    /* event driver */
    if (m_iQueueFd > 0) {
        lseek(m_iQueueFd, 0, SEEK_SET);
        if (write(m_iQueueFd, "a", 2)) ;
    }

    pthread_mutex_unlock(&m_stMsgQueue[ucQueueIndex].stMutexLock);
    return OPLK_OK;

INPUT_ERR_LABEL:
    return OPLK_ERR;

ERR_LABEL:

    pthread_mutex_unlock(&m_stMsgQueue[ucQueueIndex].stMutexLock);
    return OPLK_ERR;
}

int
UTIL_GetQueueNode(int8_t cIndex, char *pcMsg, uint32_t *pulMsgLen)
{
    uint32_t ulPos = 0;
    uint8_t ucQueueIndex = 0;

    if (NULL == pcMsg || 0 >= cIndex || cIndex > DEF_MSG_QUEUE_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstQueueMsg or queue index %d error", cIndex);
        goto INPUT_ERR_LABEL;
    }

    ucQueueIndex = cIndex - 1;

    pthread_mutex_lock(&m_stMsgQueue[ucQueueIndex].stMutexLock);

    if (false == m_stMsgQueue[ucQueueIndex].ulUsedFlag) {
        /* LOG_WRITE(EN_LOG_ERROR, "queue used flag error %d", m_stMsgQueue[ucQueueIndex].ulUsedFlag); */
        goto ERR_LABEL;
    }

    if (0 == m_stMsgQueue[ucQueueIndex].ulNum ) {
        /* queue msg is empty.														*/
        goto ERR_LABEL;
    }

    if (m_stMsgQueue[ucQueueIndex].ulMsgLen > *pulMsgLen) {
        LOG_WRITE(EN_LOG_ERROR, "queue define MsgLen %d > get MsgLen %d", m_stMsgQueue[ucQueueIndex].ulMsgLen, *pulMsgLen);
        goto ERR_LABEL;
    }

    if (NULL == m_stMsgQueue[ucQueueIndex].pcMsgInfor) {
        LOG_WRITE(EN_LOG_ERROR, "m_stMsgQueue MsgInfor is NULL");
        goto ERR_LABEL;
    }

    /* Get node from queue.                                                                                                     */
    if (m_stMsgQueue[ucQueueIndex].ulPosOut >= m_stMsgQueue[ucQueueIndex].ulMaxSize) {
        m_stMsgQueue[ucQueueIndex].ulPosOut = 0;
    }

    ulPos = m_stMsgQueue[ucQueueIndex].ulPosOut * m_stMsgQueue[ucQueueIndex].ulMsgLen;
    memcpy(pcMsg, m_stMsgQueue[ucQueueIndex].pcMsgInfor + ulPos, m_stMsgQueue[ucQueueIndex].ulMsgLen);
    *pulMsgLen = m_stMsgQueue[ucQueueIndex].ulMsgLen;
    memset(m_stMsgQueue[ucQueueIndex].pcMsgInfor + ulPos, 0x00, m_stMsgQueue[ucQueueIndex].ulMsgLen);

    m_stMsgQueue[ucQueueIndex].ulPosOut++;
    m_stMsgQueue[ucQueueIndex].ulNum--;

    pthread_mutex_unlock(&m_stMsgQueue[ucQueueIndex].stMutexLock);
    return OPLK_OK;

INPUT_ERR_LABEL:
    return OPLK_ERR;

ERR_LABEL:

    pthread_mutex_unlock(&m_stMsgQueue[ucQueueIndex].stMutexLock);
    return OPLK_ERR;

}


static void *
__UTIL_QueueLoopThread(void *arg)
{
    int iRet = OPLK_ERR;
    fd_set iReadFds;
    struct timeval stTimeout;
    QuqueCb fpQueueCb;

    uint8_t ucIndex = 0;
    ST_UTIL_QUEUE_MSG stQueueNode;
    uint32_t ulLen = sizeof(stQueueNode);

    if (NULL == arg) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == arg");
        goto ERR_LABEL;
    }

    fpQueueCb = (QuqueCb)arg;

    prctl(PR_SET_NAME, "queue_pthread");
    while (!m_bQueueThreadExit) {
        FD_ZERO(&iReadFds);
        FD_SET(m_iQueueFd, &iReadFds);

        stTimeout.tv_sec = 30;
        stTimeout.tv_usec = 0;

        iRet = select(m_iQueueFd + 1, &iReadFds, NULL, NULL, &stTimeout);
        if (0 > iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_QueueLoopThread select error : %d", iRet);
            goto ERR_LABEL;
        } else if (0 == iRet) {
            continue;     /*timeout*/
        }

        /* select return > 0 */
        /* if received message from command socket */
        if (FD_ISSET(m_iQueueFd, &iReadFds)) {
            for (ucIndex = 1; ucIndex <= DEF_MSG_QUEUE_NUM; ucIndex++) {
                while (0 == UTIL_GetQueueNode(ucIndex, (char *)&stQueueNode, &ulLen)) {
                    if (OPLK_OK != fpQueueCb(&stQueueNode)) {
                        LOG_WRITE(EN_LOG_ERROR, "queue cb error");
                    }
                }
            }
        }
    }

ERR_LABEL:

    pthread_join(m_tQueueThread, NULL);
    LOG_WRITE(EN_LOG_NOTICE, "__UTIL_QueueLoopThread exit");

    return NULL;
}


int
UTIL_RegQueueCb(QuqueCb fpQueueCb)
{
    if (NULL == fpQueueCb) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == fpQueueCb");
        goto ERR_LABEL;
    }

    m_iQueueFd = open(QUEUE_FILE, O_RDWR | O_CREAT, 0644);
    if (m_iQueueFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Open %s error", QUEUE_FILE);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Start Queue thread!");

    if (0 != pthread_create(&m_tQueueThread, NULL, __UTIL_QueueLoopThread, (void *)fpQueueCb)) {
        LOG_WRITE(EN_LOG_ERROR, "Start Queue thread fail!");
        goto ERR_LABEL;
    }

    /* pthread_join(m_tQueueThread, NULL); */
    DEF_SLEEP_S(1);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


int
UTIL_UnRegQueueCb()
{
    m_bQueueThreadExit = true;

    if (m_tQueueThread > 0) {
        pthread_cancel(m_tQueueThread);
    }

    DEF_SAFE_CLOSE(m_iQueueFd);

    LOG_WRITE(EN_LOG_TRACE, "Queue thread exit!");

    return OPLK_OK;
}


