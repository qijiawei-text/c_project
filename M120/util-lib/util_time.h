/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_time.h
* Description:   get or set date and time
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_TIME_H__
#define __UTIL_TIME_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif


extern int UTIL_CheckTime(struct tm *plocaltime);
extern int64_t UTIL_GetUptime(void);
extern uint64_t UTIL_GetTimeMs(void);
extern int UTIL_GetSysTime(char *pcTime, uint32_t ulLen);
extern int UTIL_SetSysTime(struct tm *pstSysTime);
extern int UTIL_GetTimeZoneString(char *pcTimeZone, uint32_t ulLen);
extern char *UTIL_LocalTimeStr(time_t timeT, char *pcTimeStr);

extern int UTIL_GetSysTimeUTC(char *pcTime, unsigned int uiLen);
extern int UTIL_GetSysTimeLocal(char *pcTime, unsigned int uiLen);
extern int UTIL_GetSysTimeLocalMicroSecond(char *pcTime, unsigned int uiLen);
extern int UTIL_GetLocalTimeWithZone(char *pcTime, unsigned int uiLen);
extern int UTIL_FormatWithTimeZone(time_t time, long fractions, char *pcTime, unsigned int uiLen);
extern int UTIL_GetLocalTimeWithZone_Z(char *pcTime, unsigned int uiLen);
extern int UTIL_FormatWithTimeZone_Z(time_t time, long fractions, char *pcTime, unsigned int uiLen);

extern void UTIL_GetClockNow(struct timespec *tsp);
extern uint64_t UTIL_GetLatencyMs(const struct timespec ts);
extern void UTIL_MakeTimeout(struct timespec *tsp, double seconds);

#ifdef __cplusplus
}
#endif

#endif
