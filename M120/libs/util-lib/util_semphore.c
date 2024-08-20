/*********************************************************************************************
* Copyright(C), 2018, Molex Tech.Co.,Ltd
* FileName :       util_semphore.c
* Description :    util_semphore.c
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*********************************************************************************************/
#include "util_semphore.h"

/* this is from the man semctl, just keep the source format */
union semun {
    int val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;   /* Array for GETALL, SETALL */
    struct seminfo *__buf;   /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int
UTIL_SemInit(SEMID *ptSem, uint32_t ulInitNum)
{
    if (NULL == ptSem) {
        LOG_WRITE(EN_LOG_ERROR, "SemInit input para error!");
        goto ERR_LABEL;
    }

    /* Init linux semaphore                                                     */
    if (OPLK_OK != sem_init(ptSem, 0, ulInitNum)) {
        LOG_WRITE(EN_LOG_ERROR, "SemInit error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SemObtain(SEMID *ptSem, uint32_t ulTimeoutMs)
{
    struct timespec stTs;
    struct timeval stTv;
    int iRet = OPLK_ERR;
    uint32_t ulBefore = 0;
    uint32_t ulLater = 0;
    uint32_t ulLeft = 0;

    if (NULL == ptSem) {
        LOG_WRITE(EN_LOG_ERROR, "SemObatain input para error!");
        goto ERR_LABEL;
    }

    if (WAIT_FOREVER == ulTimeoutMs) {
        iRet = sem_wait(ptSem);
        /* A signal handler always interrupts a blocked call to one of these    */
        /* functions, regardless of the use of the sigaction(2) SA_RESTART flag */
        while (OPLK_OK != iRet && errno == EINTR) {
            iRet = sem_wait(ptSem);
        }
    } else {
        gettimeofday(&stTv, NULL);
        ulBefore = stTv.tv_sec * 1000 + stTv.tv_usec / 1000;

        stTv.tv_usec += ulTimeoutMs * 1000;
        if (stTv.tv_usec > 1000000) {
            stTv.tv_sec += stTv.tv_usec / 1000000;
            stTv.tv_usec %= 1000000;
        }

        stTs.tv_sec = stTv.tv_sec;
        stTs.tv_nsec = stTv.tv_usec * 1000;

        iRet = sem_timedwait(ptSem, &stTs);

        /* A signal handler always interrupts a blocked call to one of these    */
        /* functions, regardless of the use of the sigaction(2) SA_RESTART flag */
        while (OPLK_OK != iRet && errno == EINTR) {
            gettimeofday(&stTv, NULL);
            ulLater = stTv.tv_sec * 1000 + stTv.tv_usec / 1000;
            ulLeft = ulTimeoutMs - (ulLater - ulBefore);

            stTv.tv_usec += ulLeft * 1000;
            if (stTv.tv_usec > 1000000) {
                stTv.tv_sec += stTv.tv_usec / 1000000;
                stTv.tv_usec %= 1000000;
            }

            stTs.tv_sec = stTv.tv_sec;
            stTs.tv_nsec = stTv.tv_usec * 1000;

            ulBefore = ulLater;
            ulTimeoutMs = ulLeft;
            iRet = sem_timedwait(ptSem, &stTs);
        }

        if (iRet != OPLK_OK) {
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SemRelease(SEMID *ptSem)
{
    if (NULL == ptSem) {
        LOG_WRITE(EN_LOG_ERROR, "SemRelease input para error!");
        goto ERR_LABEL;
    }

    if (OPLK_OK != sem_post(ptSem)) {
        LOG_WRITE(EN_LOG_ERROR, "sem_post error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


int
UTIL_SemDestory(SEMID *ptSem)
{
    if (NULL == ptSem) {
        LOG_WRITE(EN_LOG_ERROR, "SemDestory input para error!");
        goto ERR_LABEL;
    }

    if (OPLK_OK != sem_destroy(ptSem)) {
        LOG_WRITE(EN_LOG_ERROR, "SemDestory error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
__GetValue(int iSemId)
{
    int iSemaphore = -1;
    union semun uSemArgs;

    iSemaphore = semctl(iSemId, 0, GETVAL, uSemArgs);
    if (0 > iSemaphore) {
        LOG_WRITE(EN_LOG_ERROR, "semctl error: %s", strerror(errno));
        goto ERR_LABEL;
    }

    return iSemaphore;

ERR_LABEL:
    return -1;
}

int
__SetValue(int iSemId, int iValue)
{
    int iSemaphore = -1;
    union semun uSemArgs;


    uSemArgs.val = iValue;
    iSemaphore = semctl(iSemId, 0, SETVAL, uSemArgs);
    if (0 > iSemaphore) {
        LOG_WRITE(EN_LOG_ERROR, "semctl error: %s", strerror(errno));
        goto ERR_LABEL;
    }

    return iSemaphore;

ERR_LABEL:
    return -1;
}

int
UTIL_SemPNoWait(int iSemId)
{
    struct sembuf stSembuf;


    stSembuf.sem_num = 0;
    stSembuf.sem_op = -1;
    stSembuf.sem_flg = IPC_NOWAIT;

    if (semop(iSemId, &stSembuf, 1) < 0) {
        /* LOG_WRITE(EN_LOG_DEBUG, "SEM_P fd = %d, error: %s", iSemId, strerror(errno)); */
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SemP(int iSemId)
{
    struct sembuf stSembuf;


    stSembuf.sem_num = 0;
    stSembuf.sem_op = -1;
    stSembuf.sem_flg = SEM_UNDO;

    if (semop(iSemId, &stSembuf, 1) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "SEM_P fd = %d, error: %s", iSemId, strerror(errno));
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SemV(int iSemId)
{
    int iSemaphore = -1;
    struct sembuf stSembuf;


    stSembuf.sem_num = 0;
    stSembuf.sem_op = 1;
    stSembuf.sem_flg = SEM_UNDO;

    iSemaphore = __GetValue(iSemId);
    if (iSemaphore < 0) {
        goto ERR_LABEL;
    } else if (iSemaphore >= 1) {
        goto END_LABEL;
    } else {
        if (semop(iSemId, &stSembuf, 1) < 0) {
            LOG_WRITE(EN_LOG_ERROR, "SEM_V fd=%d error, %d, %s", iSemId, __GetValue(iSemId), strerror(errno));
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

END_LABEL:
    LOG_WRITE(EN_LOG_TRACE, "EM_V fd=%d succ, %d", iSemId, __GetValue(iSemId));
    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_SemCreate(int32_t ulKey)
{
    int iSemId = -1;

    iSemId = semget((key_t)ulKey, 1, IPC_CREAT | 0666);
    if (iSemId < 0) {
        LOG_WRITE(EN_LOG_ERROR, "semget create error: %s", strerror(errno));
        goto ERR_LABEL;
    }

    __SetValue(iSemId, 1);

    return iSemId;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_SemQuote(int32_t ulKey)
{
    int iSemId = -1;


    iSemId = semget((key_t)ulKey, 0, IPC_CREAT | 0666);
    if (iSemId < 0) {
        LOG_WRITE(EN_LOG_ERROR, "semget quote error: %s", strerror(errno));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "SEM_Quote iSemaphore = %d", __GetValue(iSemId));

    return iSemId;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_SemClose(int iSemId)
{
    if (iSemId > 0) {
        close(iSemId);
    }

    return OPLK_OK;
}

int
UTIL_SemGet(int32_t ulKey)
{
    int iSemId = -1;

    iSemId = semget((key_t)ulKey, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (iSemId > 0) {
        /* A new semphore */
        LOG_WRITE(EN_LOG_NOTICE, "Create Semaphore succ");
        goto END_LABEL;
    }

    /* The semphore is already exist.*/
    if (EEXIST == errno) {
        iSemId = semget((key_t)ulKey, 0, IPC_CREAT | 0666);
        if (iSemId < 0) {
            LOG_WRITE(EN_LOG_ERROR, "semget error: %s", strerror(errno));
            goto ERR_LABEL;
        }
        LOG_WRITE(EN_LOG_NOTICE, "Quote Semaphore succ");
    } else {
        LOG_WRITE(EN_LOG_ERROR, "semget error: %s", strerror(errno));
        goto ERR_LABEL;
    }

END_LABEL:
    __SetValue(iSemId, 1);
    LOG_WRITE(EN_LOG_NOTICE, "SEM_Create iSemaphore = %d, value = %d", iSemId, __GetValue(iSemId));
    return iSemId;

ERR_LABEL:
    return OPLK_ERR;
}