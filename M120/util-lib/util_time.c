/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_time.c
* Description:   get or set date and time
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_time.h"

#define DEF_GET_TIMEZONE                        ("date +'%:z'")

int64_t
UTIL_GetUptime(void)
{
    struct sysinfo stSysInfo;
    int64_t llUptime = 0;

    if (0 != sysinfo(&stSysInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "0 != sysinfo");
        goto ERR_LABEL;
    }

    llUptime = stSysInfo.uptime;

    return llUptime;

ERR_LABEL:
    return OPLK_ERR;
}

uint64_t
UTIL_GetTimeMs(void)
{
    struct timeval stTv;

    gettimeofday(&stTv, NULL);

    return (uint64_t)stTv.tv_sec * 1000 + stTv.tv_usec / 1000;
}


int
UTIL_GetSysTime(char *pcTime, uint32_t ulLen)
{
    struct timeval tv;
    struct tm *pstSysTime = NULL;
    char acTimeZone[DEF_BUF_SIZE_32] = {0};

    if (NULL == pcTime || 0 == ulLen) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcTime || 0 == ulLen");
        goto ERR_LABEL;
    }

    gettimeofday(&tv, NULL);

    pstSysTime = localtime(&tv.tv_sec);

    /*
     * get system time with timezone
     * example: 2018-06-20T14:05:12.123456+08:00
     */
    if (OPLK_OK != UTIL_GetTimeZoneString(acTimeZone, sizeof(acTimeZone))) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_GetTimeZoneString error");
        goto ERR_LABEL;
    }

    snprintf(pcTime, ulLen, "%4d-%02d-%02dT%02d:%02d:%02d.%d%s",
             pstSysTime->tm_year + 1900, pstSysTime->tm_mon + 1, pstSysTime->tm_mday,
             pstSysTime->tm_hour,
             pstSysTime->tm_min, pstSysTime->tm_sec, (int)tv.tv_usec, acTimeZone);

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SetSysTime(struct tm *pstSysTime)
{
    /*char   acBuf[DEF_BUF_SIZE_128] = {0};
       struct tm stTime;*/
    struct timeval stTv;

    if (NULL == pstSysTime) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstSysTime");
        goto ERR_LABEL;
    }

    /*memset(acBuf, 0, sizeof(acBuf));
       snprintf(acBuf, sizeof(acBuf), "%d-%d-%d %d:%d:%d",
             pstSysTime->tm_year, pstSysTime->tm_mon, pstSysTime->tm_mday,
             pstSysTime->tm_hour, pstSysTime->tm_min, pstSysTime->tm_sec);*/
    /*<time.h>, implicit declaration of function 'strptime*/
    /*strptime(acBuf, "%Y-%m-%d %H:%M:%S", &stTime);*/

    memset(&stTv, 0, sizeof(stTv));
    stTv.tv_sec = mktime(pstSysTime);

    if (OPLK_OK != settimeofday(&stTv, NULL)) {
        LOG_WRITE(EN_LOG_ERROR, "Set real time to system error");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_GetTimeZoneString(char *pcTimeZone, uint32_t ulLen)
{
    char acTimeZone[DEF_BUF_SIZE_32] = {0};
    uint32_t ulLength = 0;
    int iRet = OPLK_ERR;

    if (NULL == pcTimeZone || 0 == ulLen) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pcTimeZone");
        goto ERR_LABEL;
    }

    iRet = UTIL_ExecuteCmd(DEF_GET_TIMEZONE, acTimeZone, DEF_BUF_SIZE_32, 500);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "get Timezone error:%s", acTimeZone);
        goto ERR_LABEL;
    }

    ulLength = strlen(acTimeZone);
    if ((ulLength > 0) && ('\n' == acTimeZone[ulLength - 1])) {
        acTimeZone[ulLength - 1] = '\0';
    }
    snprintf(pcTimeZone, ulLen, "%s", acTimeZone);

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_CheckTime(struct tm *plocaltime)
{
    int iIsLeapYear = 0;

    if (NULL == plocaltime) {
        goto ERR_LABEL;
    }

    if (plocaltime->tm_year < 2000
        || plocaltime->tm_year > 2050
        || plocaltime->tm_mon < 1
        || plocaltime->tm_mon > 12
        || plocaltime->tm_mday < 1
        || plocaltime->tm_mday > 31) {
        goto ERR_LABEL;
    }

    /**check year                                                               */
    if ((plocaltime->tm_year % 4) == 0) {
        if ((plocaltime->tm_year % 100) == 0) {
            if ((plocaltime->tm_year % 400) == 0) {
                /*Leap Year                                                */
                iIsLeapYear = 1;
            }
        } else {
            /*Leap Year                                                */
            iIsLeapYear = 1;
        }
    } else {
        iIsLeapYear = 0;
    }

    if (plocaltime->tm_mon == 4
        || plocaltime->tm_mon == 6
        || plocaltime->tm_mon == 9
        || plocaltime->tm_mon == 11) {
        if ((plocaltime->tm_mday > 30) || (plocaltime->tm_mday < 1)) {
            goto ERR_LABEL;
        }
    } else if (plocaltime->tm_mon == 2 && !iIsLeapYear) {
        if ((plocaltime->tm_mday > 28) || (plocaltime->tm_mday < 1)) {
            goto ERR_LABEL;
        }
    } else if (plocaltime->tm_mon == 2 && iIsLeapYear) {
        if ((plocaltime->tm_mday > 29) || (plocaltime->tm_mday < 1)) {
            goto ERR_LABEL;
        }
    } else {
        if ((plocaltime->tm_mday > 31) ||
            (plocaltime->tm_mday < 1)) {
            goto ERR_LABEL;
        }
    }

    if (plocaltime->tm_hour < 0 || plocaltime->tm_hour > 23
        || plocaltime->tm_min < 0 || plocaltime->tm_min > 59
        || plocaltime->tm_sec < 0 || plocaltime->tm_sec > 59) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    if (NULL == plocaltime) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == plocaltime");
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Check Time error %d-%d-%d %d:%d%d",
                  plocaltime->tm_year, plocaltime->tm_mon, plocaltime->tm_mday,
                  plocaltime->tm_hour, plocaltime->tm_min, plocaltime->tm_sec);
    }

    return OPLK_ERR;
}


char *
UTIL_LocalTimeStr(time_t timeT, char *pcLocalTimeStr)
{
    struct tm tm;

    if (NULL == pcLocalTimeStr) {
        LOG_WRITE(EN_LOG_ERROR, "input pcTimeStr == NULL");
        return NULL;
    }

    tm = *localtime(&timeT);
    sprintf(pcLocalTimeStr, "%d-%02d-%02dT%02d:%02d:%02dZ", tm.tm_year + 1900,
            tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    return pcLocalTimeStr;
}


/*
 * get system time with timezone
 * example: 2018-06-20 14:05:12.123456
 */
int
UTIL_GetSysTimeUTC(char *pcTime, unsigned int uiLen)
{
    struct timeval tv;
    struct tm *pstSysTime = NULL;

    if (NULL == pcTime || 0 == uiLen) {
        goto ERR_LABEL;
    }

    gettimeofday(&tv, NULL);

    pstSysTime = gmtime(&tv.tv_sec);

    snprintf(pcTime, uiLen, "%4d-%02d-%02d %02d:%02d:%02d.%d",
             pstSysTime->tm_year + 1900, pstSysTime->tm_mon + 1, pstSysTime->tm_mday, pstSysTime->tm_hour,
             pstSysTime->tm_min, pstSysTime->tm_sec, (int)tv.tv_usec);

    return 0;

ERR_LABEL:

    return -1;
}

/*
 * get system time with timezone
 * example: 2018-06-20 14:05:12.123456 UTC
 */
int
UTIL_GetSysTimeLocal(char *pcTime, unsigned int uiLen)
{
    struct timeval tv;
    struct tm *pstSysTime = NULL;

    if (NULL == pcTime || 0 == uiLen) {
        goto ERR_LABEL;
    }

    gettimeofday(&tv, NULL);

    pstSysTime = localtime(&tv.tv_sec);

    snprintf(pcTime, uiLen, "%4d-%02d-%02d %02d:%02d:%02d.%d",
             pstSysTime->tm_year + 1900, pstSysTime->tm_mon + 1, pstSysTime->tm_mday, pstSysTime->tm_hour,
             pstSysTime->tm_min, pstSysTime->tm_sec, (int)tv.tv_usec);

    return 0;

ERR_LABEL:

    return -1;
}

int
UTIL_GetSysTimeLocalMicroSecond(char *pcTime, unsigned int uiLen)
{
    struct timeval tv;
    struct tm *pstSysTime = NULL;

    if (NULL == pcTime || 0 == uiLen) {
        goto ERR_LABEL;
    }

    gettimeofday(&tv, NULL);

    pstSysTime = gmtime(&tv.tv_sec);

    snprintf(pcTime, uiLen, "%4d-%02d-%02dT%02d:%02d:%02d.%06dZ",
             pstSysTime->tm_year + 1900, pstSysTime->tm_mon + 1, pstSysTime->tm_mday, pstSysTime->tm_hour,
             pstSysTime->tm_min, pstSysTime->tm_sec, (int)tv.tv_usec);

    return 0;

ERR_LABEL:

    return -1;
}

int
UTIL_GetLocalTimeWithZone(char *pcTime, unsigned int uiLen)
{
    time_t t;
    time(&t);

    return UTIL_FormatWithTimeZone(t, -1, pcTime, uiLen);
}

int
UTIL_FormatWithTimeZone(time_t time, long fractions, char *pcTime, unsigned int uiLen)
{
    struct tm tm;
    int32_t zonediff_h, zonediff_m;

    /* convert */
    if (!localtime_r(&time, &tm)) {
        return -1;
    }

    /* get timezone offset */
    if (tm.tm_gmtoff == 0) {
        /* time is Zulu (UTC) */
        zonediff_h = 0;
        zonediff_m = 0;
    } else {
        /* timezone offset */
        zonediff_h = tm.tm_gmtoff / 60 / 60;
        zonediff_m = tm.tm_gmtoff / 60 % 60;
    }

    if (fractions < 0) {
        snprintf(pcTime, uiLen, "%04d-%02d-%02dT%02d:%02d:%02d%+03d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                 zonediff_h, zonediff_m);
    } else {
        snprintf(pcTime, uiLen, "%04d-%02d-%02dT%02d:%02d:%02d.%06ld%+03d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                 fractions, zonediff_h, zonediff_m);
    }

    return 0;
}

int
UTIL_GetLocalTimeWithZone_Z(char *pcTime, unsigned int uiLen)
{
    time_t t;

    time(&t);

    return UTIL_FormatWithTimeZone_Z(t, -1, pcTime, uiLen);
}

int
UTIL_FormatWithTimeZone_Z(time_t time, long fractions, char *pcTime, unsigned int uiLen)
{
    struct tm tm;
    int32_t zonediff_h, zonediff_m;

    /* convert */
    if (!localtime_r(&time, &tm)) {
        return -1;
    }

    /* get timezone offset */
    if (tm.tm_gmtoff == 0) {
        /* time is Zulu (UTC) */
        zonediff_h = 0;
        zonediff_m = 0;
    } else {
        /* timezone offset */
        zonediff_h = tm.tm_gmtoff / 60 / 60;
        zonediff_m = tm.tm_gmtoff / 60 % 60;
    }

    if (fractions < 0) {
        snprintf(pcTime, uiLen, "%04d-%02d-%02dT%02d:%02d:%02dZ%+03d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                 zonediff_h, zonediff_m);
    } else {
        snprintf(pcTime, uiLen, "%04d-%02d-%02dT%02d:%02d:%02d.%06ldZ%+03d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                 fractions, zonediff_h, zonediff_m);
    }

    return 0;
}

void
UTIL_GetClockNow(struct timespec *tsp)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tsp->tv_sec = tv.tv_sec;
    tsp->tv_nsec = tv.tv_usec * 1000;
}

uint64_t
UTIL_GetLatencyMs(const struct timespec ts)
{
    struct timespec ts2;
    struct timeval now;

    gettimeofday(&now, NULL);
    ts2.tv_sec = now.tv_sec;
    ts2.tv_nsec = now.tv_usec * 1000;

    ts2.tv_sec -= ts.tv_sec;
    if (ts2.tv_nsec >= ts.tv_nsec) {
        ts2.tv_nsec -= ts.tv_nsec;
    } else {
        ts2.tv_nsec += 1000000000L - ts.tv_nsec;
        ts2.tv_sec--;
    }

    return (uint64_t)ts2.tv_nsec / 1000000ull + (uint64_t)ts2.tv_sec * 1000ull;
}

#include <math.h> /* for trunc */

void
UTIL_MakeTimeout(struct timespec *tsp, double seconds)
{
    struct timeval now;

    gettimeofday(&now, NULL);
    tsp->tv_sec = now.tv_sec;
    tsp->tv_nsec = now.tv_usec * 1000;

    double fs = trunc(seconds);
    double ns = (seconds - fs) * (double)1000000000.0;

    tsp->tv_sec += (long)fs;
    tsp->tv_nsec += (long)ns;

    if (tsp->tv_nsec > 1000000000ul) {
        tsp->tv_sec += tsp->tv_nsec / (1000000000ul);
        tsp->tv_nsec %= 1000000000ul;
    }
}