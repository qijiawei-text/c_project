/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : util_inotify.c
   Description : Linux monitor file.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/06/18   V1.0.0       Initial file.
**********************************************************************************/
#include "util_inotify.h"

static bool m_bInotifyExit = false;


static void *
__UTIL_InotifyLoopThread(void *arg)
{
    int iFd = -1;
    char acBuf[DEF_BUF_SIZE_256];
    size_t nLen = 0;
    ST_INOTIFY_ARG stInotifyArg;

    if (NULL == arg) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == arg");
        return NULL;
    }

    memset(&stInotifyArg, 0, sizeof(stInotifyArg));
    memcpy(&stInotifyArg, arg, sizeof(stInotifyArg));

    prctl(PR_SET_NAME, "inotify_pthread");
    LOG_WRITE(EN_LOG_TRACE, "Inotify fd %d, fp %p", stInotifyArg.iFd,
              stInotifyArg.fpInotifyCb);
    iFd = stInotifyArg.iFd;
    if (stInotifyArg.iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "iFd error");
        return NULL;
    }

    while (!m_bInotifyExit) {
        memset(acBuf, 0, sizeof(acBuf));
        nLen = read(iFd, acBuf, sizeof(acBuf));
        if (nLen <= 0) {
            sleep(1);
            continue;
        }

        if (0 != stInotifyArg.fpInotifyCb(stInotifyArg.acFile)) {
            LOG_WRITE(EN_LOG_ERROR, "inotify cb error");
        }
    }

    if (iFd > 0) {
        close(iFd);
    }

    LOG_WRITE(EN_LOG_TRACE, "__InotifyLoop exit");
    return NULL;
}

int
UTIL_RegInotifyCb(char *pcFile, InotifyCb fpInotifyCb)
{
    int iRet = OPLK_ERR;
    int iFd = -1;
    ST_INOTIFY_ARG stInotifyArg;
    pthread_t tInotifyThread;

    if (NULL == pcFile || NULL == fpInotifyCb) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFile");
        iRet = -1;
        goto cleanup;
    }

    if (sizeof(stInotifyArg.acFile) <= strlen(pcFile)) {
        LOG_WRITE(EN_LOG_ERROR, "sizeof(stInotifyArg.acFile) <= strlen(pcFile) %s", pcFile);
        iRet = -1;
        goto cleanup;
    }

    iFd = inotify_init();
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "inotify_init error");
        iRet = -2;
        goto cleanup;
    }

    memset(&stInotifyArg, 0x00, sizeof(stInotifyArg));
    stInotifyArg.iFd = iFd;
    stInotifyArg.fpInotifyCb = fpInotifyCb;
    strcpy(stInotifyArg.acFile, pcFile);

    if (0 != pthread_create(&tInotifyThread, NULL, __UTIL_InotifyLoopThread,
                            (void *)&stInotifyArg)) {
        LOG_WRITE(EN_LOG_ERROR, "Start inotify thread fail!");
        iRet = -3;
        goto cleanup;
    }

    DEF_SLEEP_S(1);

cleanup:
    return iRet;
}

void
UTIL_UnRegInotifyCb(void)
{
    m_bInotifyExit = true;
    LOG_WRITE(EN_LOG_TRACE, "inotify thread exit!");
}

