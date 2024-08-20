/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_log.h
* Description:   log
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EN_OUTPUT_FILE = 0,                                                     /* output the log to internal file                         */
    EN_OUTPUT_SYSLOG,                                                       /* output the log by Syslog                                */

} EN_UTIL_LOG_OUTPUT;

typedef enum {
    EN_LOG_EMERGENCY = 0,
    EN_LOG_ALERT,
    EN_LOG_CRITICAL,
    EN_LOG_ERROR,                                                           /* error, SW running error or HW failure, Log infor will be saved into the file of high.log*/
    EN_LOG_WARNING,                                                         /* alarm, Log infor will be saved into the file of middle.log                       */
    EN_LOG_NOTICE,                                                          /* default severity for record log, set operation log, Log infor will be saved into the file of low.log*/
    EN_LOG_TRACE,                                                           /* running log, Log infor will be saved into the file of low.log                    */
    EN_LOG_DEBUG,                                                           /* debug log, Log infor will be saved into the file of low.log                      */

    EN_LOG_SEVERITY_ALL = 0xFF                                              /* list log, list all log not distinct severity level      */

} EN_UTIL_LOG_SEVERITY;

typedef enum {
    EN_LOG_SAVE_MSATA = 0,                                                  /* save log to msata                                       */
    EN_LOG_SAVE_FLASH,                                                      /* save log to flash                                       */

} EN_UTIL_LOG_SAVE_DEVICE;

#define __FILENAME__                          (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#define DEF_OPLK_LOG_CSV_FILE                "./oplklog.csv"

/* define len for internal log file body                                                          */
#define DEF_LOG_MSG_MAX_LEN                  (512)                          /* log body 256 Bytes                                      */
#define DEF_LOG_FILE_NAME_LEN                (32)
#define DEF_LOG_USER_NAME_LEN                (32)
#define DEF_OPLK_LOG_MSG_LEN                 (192)
#define DEF_USER_LOG_MSG_LEN                 (152)
#define DEF_LOG_IP_ADDRESS_LEN               (40)

/* define len for internal log file header                                                        */
#define DEF_LOG_HEADER_KEYWORD_LEN           (32)
#define DEF_LOG_HERADR_RESV_LEN              (84)

#define DEF_OPLK_LOG_MAX_NUM                 (10000)
#define DEF_USER_LOG_MAX_NUM                 (10000)
#define DEF_OPLK_LOG_BODY_LEN                (256)                          /* sizeof  ST_OPLK_LOG_BODY                                */
#define DEF_USER_LOG_BODY_LEN                (256)                          /* sizeof  ST_USER_LOG_BODY                                */
#define DEF_SYSLOG_LOG_MSG_LEN               (512)

/* 128 Bytes                                                                                      */
typedef struct {
    char acKeyWord[DEF_LOG_HEADER_KEYWORD_LEN];
    uint32_t ulStartIndex;
    uint32_t ulEndIndex;
    char acResv[DEF_LOG_HERADR_RESV_LEN];                                   /* size 84 Byts                                            */
    uint32_t ulHeaderCrc32;

} ST_OPLK_LOG_HEADER;

/* 256 Bytes                                                                                      */
typedef struct {
    uint64_t ullTimeMs;                                                     /* Generate Log Time, Unit:ms                              */
    uint32_t ulLineNum;                                                     /* File Line Num                                           */
    uint8_t ucBank;                                                         /* Low, Middle, High                                       */
    uint8_t ucProcess;                                                      /* SCM, CLI, SNMP, WEB                                     */
    uint8_t ucSeverity;                                                     /* Error, Warning, Notice, Trace,, Debug                   */
    uint8_t aucResv[17];                                                    /* Reserve                                                 */
    char acFileName[DEF_LOG_FILE_NAME_LEN];                                 /* Source Code File Name                                   */
    char acRecordMsg[DEF_OPLK_LOG_MSG_LEN];                                 /* Log Information                                         */

} ST_OPLK_LOG_BODY;

/* 256 Bytes                                                                                      */
typedef struct {
    uint64_t ullTimeMs;                                                     /* Generate Log Time, Unit:ms                              */
    uint8_t ucProcess;                                                      /* SCM, CLI, SNMP, WEB                                     */
    uint8_t ucRemoteProtocol;                                               /* ssh http https                                          */
    uint8_t ucSeverity;                                                     /* Error, Warning, Notice, Trace, Debug                    */
    uint8_t ucOperStatus;                                                   /* OK or Failed                                            */
    uint8_t ucUserLevel;
    uint8_t aucResv[19];
    char acUserName[DEF_LOG_USER_NAME_LEN];
    char acRemoteAddr[DEF_LOG_IP_ADDRESS_LEN];                              /* IPv4 or IPv6                                            */
    char acRecordMsg[DEF_USER_LOG_MSG_LEN];                                 /* Log Information                                         */

} ST_USER_LOG_BODY;

extern int UTIL_ListOplkLogs(char *pcFileName,
                             EN_UTIL_LOG_SEVERITY enSeverity,
                             uint32_t *pulLogCount,
                             int (*pfCallbackFormatLog)(void *pArg, ST_OPLK_LOG_BODY *pstLogBody));

extern char *UTIL_LogSeverityEnumToStr(uint8_t ucServrity);
extern EN_UTIL_LOG_OUTPUT UTIL_GetHostOplkLogOutput(void);
extern EN_UTIL_LOG_OUTPUT UTIL_GetHostUserLogOutput(void);

/* client interface functions                                                                     */
extern int UTIL_InitClientLog(void);
extern int UTIL_CloseClientLog(void);

/* waiting for the signal SIGUSR1 for save log completely                                         */
extern int UTIL_SaveClientLog(void);

extern void UTIL_SetClientLogSeverity(EN_UTIL_LOG_SEVERITY enSeverity);

extern int UTIL_SendClientOplkLog(EN_UTIL_LOG_SEVERITY enSeverity,
                                  const char *pcFileName,
                                  uint32_t ulLineNum,
                                  const char *pcLogInfo, ...);
extern int UTIL_SendClientUserLog(ST_USER_LOG_BODY *pstUserLog);

/* for debug, write log to syslog directly                                                        */
extern int UTIL_WriteSyslog(int iFacility,
                            EN_UTIL_LOG_SEVERITY enSeverity,
                            const char *pcFileName,
                            uint32_t ulLineNum,
                            const char *pcLogInfo, ...);
/* enable or disable record the process log                                                       */
extern void UTIL_SetProcLogEnable(bool bEnable);

/* internal log                                                                                   */
#ifndef DEF_CHECK_CODE
#define LOG_WRITE(enSeverity, pcStr, args ...)                                      \
    do {                                                                             \
        UTIL_SendClientOplkLog(enSeverity, __FILENAME__, __LINE__, pcStr, ## args);   \
    } while (0)

#else
#define LOG_WRITE(enSeverity, pcStr, args ...)                                     \
    do {                                                                            \
        printf(pcStr, ## args);                                                      \
    } while (0)
#endif

/* record alarm by syslog, log severity is alert                                                  */
#define ALARM_WRITE(pcStr, args ...)                                                \
    do {                                                                             \
        UTIL_WriteSyslog(LOG_LOCAL3, EN_LOG_ALERT, __FILENAME__, __LINE__, pcStr, ## args);       \
    } while (0)

/* for debug, write log to syslog directly or Monitor                                             */
#define LOG_WRITE_SYSLOG(enSeverity, pcStr, args ...)                               \
    do {                                                                             \
        UTIL_WriteSyslog(0, enSeverity, __FILENAME__, __LINE__, pcStr, ## args);         \
    } while (0)

/* record alarm by syslog, log severity is alert												  */
#define USER_LOG(enSeverity, pcStr, args ...)                                                 \
    do {                                                                                                                                                     \
        UTIL_WriteSyslog(LOG_LOCAL6, enSeverity, __FILENAME__, __LINE__, pcStr, ## args);         \
    } while (0)


/* host interface functions                                                                       */
extern int UTIL_SetHostLogOutput(EN_UTIL_LOG_OUTPUT enOplkOutput,
                                 EN_UTIL_LOG_OUTPUT enUserOutput);
extern int UTIL_InitHostLog(void);
extern int UTIL_CloseHostLog(void);

extern int UTIL_RecvHostLog(void);
extern void UTIL_SetHostExitLoop(bool bExitLoop);
extern void UTIL_SetHostLogSaveDevice(EN_UTIL_LOG_SAVE_DEVICE enDevice);
extern int LOG_SetLogHashEnable(bool bEnable);
#ifdef __cplusplus
}
#endif

#endif
