/***MODU+******************************************************************************************
* Copyright(C), 2023, OPLINK Tech.Co.,Ltd
* FileName    : UTIL_log_hash.c
* Description : Prevent the syslog floods
* History     :
*     [Author]      [Date]          [Version]        [Description]
* [1] Chun Ye       2023/03/20      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <pthread.h>

#include "util_log_hash.h"

/* Using hash to look up, add and delete the key                                                  */
/* Source: https://github.com/troydhanson/uthash                                                  */
/* Document: https://troydhanson.github.io/uthash/userguide.html                                  */
#include "uthash.h"

/**************************************************************************************************/
/* LOG message format:                                                                            */
/*     [OPT][command][cmd_task.c:375] admin Enter linux shell and enter admin user                */
/* Using the 'filename:linenumber' as the key                                                     */
/* @ {                                                                                            */
#define DEF_LOG_FILTER_KEY_MAX_LEN       (64)
#define DEF_CONTENT_MAX_LEN             (256)
typedef struct {
    char acKey[DEF_LOG_FILTER_KEY_MAX_LEN];
    char acContent[DEF_CONTENT_MAX_LEN];
    UT_hash_handle hh;
} ST_LOG_HASH;
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of the global pointer to the log structure, must initialize to NULL!!!              */
/* @ {                                                                                            */
ST_LOG_HASH *m_pstLogs = NULL;
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of mutex for log hash table and global value m_lLastTime. The API 'UTIL_LogHash'    */
/* will be called by muti threads                                                                 */
/* @ {                                                                                            */
static pthread_mutex_t m_stLogHashMutex;
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of log hash table update timestamp                                                  */
/* @ {                                                                                            */
static int64_t m_lLastTime = 0;
#define DEF_HASH_TABLE_CLR_INTERVAL         (5 * 60)                        /* unit: s                                                 */
/* @ }                                                                                            */


static int64_t
__GetUptime()
{
    struct sysinfo stSysInfo;
    int64_t lUptime = 0;

    memset(&stSysInfo, 0x00, sizeof(stSysInfo));
    if (0 != sysinfo(&stSysInfo)) {
        goto ERR_LABEL;
    }

    lUptime = stSysInfo.uptime;

    return lUptime;

ERR_LABEL:
    return -1;
}

int
__add_log(char *pcInputKey, char *pcContent)
{
    ST_LOG_HASH *pstTmp = NULL;

    if (NULL == pcInputKey || NULL == pcContent) {
        return -1;
    }

    HASH_FIND_STR(m_pstLogs, pcInputKey, pstTmp);                           /* id already in the hash?                                 */
    if (NULL == pstTmp) {
        pstTmp = (ST_LOG_HASH *)malloc(sizeof(ST_LOG_HASH));
        if (NULL == pstTmp) {
            return -1;
        }

        strncpy(pstTmp->acKey, pcInputKey, sizeof(pstTmp->acKey) - 1);
        strncpy(pstTmp->acContent, pcContent, sizeof(pstTmp->acContent) - 1);
        HASH_ADD_STR(m_pstLogs, acKey, pstTmp);                             /* id is the key field                                     */
    }

    strncpy(pstTmp->acContent, pcContent, sizeof(pstTmp->acContent) - 1);
    pstTmp->acContent[DEF_CONTENT_MAX_LEN - 1] = '\0';

    return 0;
}

int
__delete_log(ST_LOG_HASH *pstLog)
{
    if (NULL == pstLog) {
        return -1;
    }

    HASH_DEL(m_pstLogs, pstLog);                                            /* user: pointer to delete                                 */
    free(pstLog);

    return 0;
}

void
__delete_all()
{
    ST_LOG_HASH *pstCurLog = NULL;
    ST_LOG_HASH *pstTmp = NULL;

    HASH_ITER(hh, m_pstLogs, pstCurLog, pstTmp) {
        HASH_DEL(m_pstLogs, pstCurLog);                                     /* delete it (users advances to next)                      */
        free(pstCurLog);                                                    /* free it                                                 */
    }
}

ST_LOG_HASH *
__find_log_by_key(char *pcInputKey)
{
    ST_LOG_HASH *pstTmp = NULL;

    HASH_FIND_STR(m_pstLogs, pcInputKey, pstTmp);
    return pstTmp;
}

void
__print_logs()
{
    ST_LOG_HASH *pstTmp = NULL;

    for (pstTmp = m_pstLogs; pstTmp != NULL; pstTmp = (ST_LOG_HASH *)(pstTmp->hh.next)) {
        printf("[%s] %s\n", pstTmp->acKey, pstTmp->acContent);
    }
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_LogHash                                                                          */
/* Descrp : Check the log in the hash table or NOT. If NOT, add in hash table                     */
/* Input  : pcKey -- A pointer of key                                                             */
/*        : pcContent -- A pointer of content                                                     */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg  0: need to write syslog                                                     */
/*              @arg -1: no need to write syslog                                                  */
/***FUNC-******************************************************************************************/
int
UTIL_LogHash(char *pcKey, char *pcContent)
{
    int64_t lCurTime = 0;

    if (NULL == pcKey) {
        return -1;
    }

    lCurTime = __GetUptime();

    if (0 == m_lLastTime) {
        /* Initilization table                                                  */
        pthread_mutex_init(&m_stLogHashMutex, NULL);

        pthread_mutex_lock(&m_stLogHashMutex);
        m_lLastTime = lCurTime;

        __add_log(pcKey, pcContent);
        pthread_mutex_unlock(&m_stLogHashMutex);

        return 0;
    }

    pthread_mutex_lock(&m_stLogHashMutex);
    if (lCurTime - m_lLastTime >= DEF_HASH_TABLE_CLR_INTERVAL) {
        /* delete log hash table each 5 minutes                                 */
        m_lLastTime = lCurTime;

        __delete_all();

        __add_log(pcKey, pcContent);

        m_lLastTime = lCurTime;
        pthread_mutex_unlock(&m_stLogHashMutex);

        return 0;
    }

    if (__find_log_by_key(pcKey) != NULL) {
        /* the log had been in hash table                                       */
        pthread_mutex_unlock(&m_stLogHashMutex);
        return -1;
    }

    __add_log(pcKey, pcContent);

    pthread_mutex_unlock(&m_stLogHashMutex);

    return 0;
}

/* gcc -o log_hash_test UTIL_log_hash.c                                                           */
