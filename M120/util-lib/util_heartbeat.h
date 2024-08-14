/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_heartbeat.h
* Description:   heart beat
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                dongyangf  2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_HEART_BEAT_H__
#define __UTIL_HEART_BEAT_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* monitor cfg cmd id */
#define DEF_MON_CFG_PROC_VALID                   (1)
#define DEF_MON_CFG_CHK_INTERVAL                 (2)
#define DEF_MON_CFG_CHK_TIMEOUT                  (3)
#define DEF_MON_CFG_RESET_PROCESS                (4)


/* monitor cfg  information*/
typedef struct tMONITOR_CFG_INFO
{
    int iCmdId;
    int iValue;
    char acProcName[DEF_PROC_NAME_LEN];
    char auBufExt[DEF_BUF_SIZE_512];
} ST_MON_CFG_INFO;

typedef struct
{
    int iFd;
    char acProcName[DEF_PROC_NAME_LEN];
    char acSoketPath[DEF_BUF_SIZE_128];
} ST_HEART_BEAT;

#define MONITOR_RECV_SOCKET_PATH    "/var/tmp/monRecvSocket"
#define HEARTBEAT_SEND_SOCKET_PATH  "/var/tmp/monSend%sSocket"

#define CSM_SEND_SOCKET_PATH        "/var/tmp/monCsmdSendSocket"
#define AUTHD_SEND_SOCKET_PATH      "/var/tmp/monAuthdSendSocket"
#define PERSISTD_SEND_SOCKET_PATH   "/var/tmp/monPersistdSendSocket"
#define CMPTCTRLD_SEND_SOCKET_PATH  "/var/tmp/monCmptctrldSendSocket"
#define TIMED_SEND_SOCKET_PATH      "/var/tmp/monTimedSendSocket"
#define NETWORKD_SEND_SOCKET_PATH   "/var/tmp/monNetworkdSendSocket"
#define SYSLOGALM_SEND_SOCKET_PATH  "/var/tmp/monSyslogalmSendSocket"
#define RECORDLOGD_SEND_SOCKET_PATH "/var/tmp/monRecordlogdSendSocket"
#define DEVHAL_SEND_SOCKET_PATH     "/var/tmp/monHalSendSocket"
#define ALMPROVIDED_SEND_SOCKET_PATH  "/var/tmp/monAlmProvidedSendSocket"
#define AUTOCTRLD_SEND_SOCKET_PATH    "/var/tmp/monAutoCtrldSendSocket"
#define PLUGD_SEND_SOCKET_PATH        "/var/tmp/monPlugdSendSocket"
#define SNMPD_SEND_SOCKET_PATH        "/var/tmp/monSnmpdSendSocket"
#define REST_SEND_SOCKET_PATH         "/var/tmp/monRestSendSocket"
#define CLI_SEND_SOCKET_PATH          "/var/tmp/monCliSendSocket"
#define OPERATIOND_SEND_SOCKET_PATH   "/var/tmp/monOperationdSendSocket"
#define PERFORMANCED_SEND_SOCKET_PATH "/var/tmp/monPerformancedSendSocket"
#define SNMP_SEND_SOCKET_PATH         "/var/tmp/monSnmpSendSocket"


extern int UTIL_SendHeartbeat();
extern int UTIL_HeartbeatCfgEnalbe(int iEnable);
extern int UTIL_HeartbeatCfgEnalbeByName(int iEnable, char *pcProcName);
extern int UTIL_HeartbeatCfgChkInterval(int iInterval);
extern int UTIL_HeartbeatCfgChkTimeout(int iTimeout);
extern int UTIL_HeartbeatCfgResetProcess(char *pcProName);
extern int UTIL_CloseHeartBeat();

#define THREAD_HEART_BEAT_TIMEOUT  15
#define THREAD_HEART_BEAT_MAX_CNT  8
typedef struct
{
    bool bIsUsed;
    int iTimeoutCnt;
    uint64_t ullCount;
} ST_THREAD_HEART_BEAT;

int UTIL_InitThreadHeartBeart();
int UTIL_IncreaseThreadHeartBeat(int iId);
int UTIL_EnableThreadHeartBeat(int iId, bool bEnable);
bool UTIL_CheckThreadHeartBeart();

#ifdef __cplusplus
}
#endif

#endif
