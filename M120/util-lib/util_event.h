/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_event.h
* Description:   event
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                dongyangf  2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_EVENT_H__
#define __UTIL_EVENT_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef int (*EventCb)(int iEventId, int iSubEventId);
typedef struct {
    EventCb fpEventCb;
    int iFd;
} ST_EVENT_ARG;

#define DEF_HAL_EVENT_SOCKET_PATH                ("/var/tmp/eventSendHalSocket")
#define DEF_CMPTCTRLD_EVENT_SOCKET_PATH          ("/var/tmp/eventRecvCmptctrldSocket")
#define DEF_CSMD_EVENT_SOCKET_PATH               ("/var/tmp/eventRecvCsmdSocket")
#define DEF_OPERATION_EVENT_SOCKET_PATH          ("/var/tmp/eventRecvOperationSocket")
#define DEF_PERSISTD_EVENT_SOCKET_PATH           ("/var/tmp/eventRecvPersistdSocket")
#define DEF_OPSD_EVENT_SOCKET_PATH               ("/var/tmp/eventRecvOpsdSocket")
#define DEF_MONITORD_EVENT_SOCKET_PATH           ("/var/tmp/eventRecvMonitordSocket")
#define DEF_RECORDLOGD_EVENT_SOCKET_PATH         ("/var/tmp/eventRecvRecordlogdSocket")
#define DEF_CSMD_HASH_EVENT_SOCKET_PATH          ("/var/tmp/eventCsmHashSocket")
#define DEF_PMD_EVENT_SOCKET_PATH                ("/var/tmp/eventRecvPmdSocket")

#define DEF_EVENT_ID_FAN                         (0)
#define DEF_EVENT_ID_PWR                         (1)
#define DEF_EVENT_ID_PA                          (2)
#define DEF_EVENT_ID_BA                          (3)
#define DEF_EVENT_ID_OPS                         (4)
#define DEF_EVENT_ID_OSC                         (5)
#define DEF_EVENT_ID_OPS_SW_HAPPEND              (6)
#define DEF_EVENT_ID_SLOT0                       (7) /* 7 ~ 16 */
#define DEF_EVENT_ID_SLOT1                       (8)
#define DEF_EVENT_ID_SLOT2                       (9)
#define DEF_EVENT_ID_SLOT3                       (10)

#define DEF_EVENT_ID_PWD_RESET                   (17)
#define DEF_EVENT_ID_OCM                         (18)
#define DEF_EVENT_ID_OTDR                        (19)
#define DEF_EVENT_ID_MUX                         (20)
#define DEF_EVENT_ID_MIRROR                      (21)
#define DEF_EVENT_ID_WSS_PORT                    (22)
#define DEF_EVENT_ID_OA_COMM_FAIL                (23)
#define DEF_EVENT_ID_CLEAR_OA_COMM_FAIL          (24)
#define DEF_EVENT_ID_WSS_COMM_FAIL                (25)
#define DEF_EVENT_ID_CLEAR_WSS_COMM_FAIL          (26)
#define DEF_EVENT_ID_DCE_COMM_FAIL                (27)
#define DEF_EVENT_ID_CLEAR_DCE_COMM_FAIL          (28)
#define DEF_EVENT_ID_OSC_COMM_FAIL                (29)
#define DEF_EVENT_ID_CLEAR_OSC_COMM_FAIL          (30)
#define DEF_EVENT_ID_PAM4_COMM_FAIL                (31)
#define DEF_EVENT_ID_CLEAR_PAM4_COMM_FAIL          (32)
#define DEF_EVENT_ID_OTDR_COMM_FAIL                (33)
#define DEF_EVENT_ID_CLEAR_OTDR_COMM_FAIL          (34)
#define DEF_EVENT_ID_OPS_COMM_FAIL                (35)
#define DEF_EVENT_ID_CLEAR_OPS_COMM_FAIL          (36)
#define DEF_EVENT_ID_TDCM_COMM_FAIL               (37)
#define DEF_EVENT_ID_CLEAR_TDCM_COMM_FAIL         (38)
#define DEF_EVENT_ID_DGE_COMM_FAIL                (39)
#define DEF_EVENT_ID_CLEAR_DGE_COMM_FAIL          (40)
#define DEF_EVENT_ID_MCS_COMM_FAIL                (41)
#define DEF_EVENT_ID_CLEAR_MCS_COMM_FAIL          (42)
#define DEF_EVENT_ID_MUX_COMM_FAIL                (43)
#define DEF_EVENT_ID_CLEAR_MUX_COMM_FAIL          (44)
#define DEF_EVENT_ID_EXTEND_COMM_FAIL             (45)
#define DEF_EVENT_ID_CLEAR_EXTEND_COMM_FAIL       (46)
#define DEF_EVENT_ID_OCM_COMM_FAIL                (47)
#define DEF_EVENT_ID_CLEAR_OCM_COMM_FAIL          (48)
#define DEF_EVENT_ID_OTDR_P                       (49)
#define DEF_EVENT_ID_OSC_P                        (50)
#define DEF_EVENT_ID_FORCEIDLE                    (51)
#define DEF_EVENT_ID_OPS_STATE_CHANGE             (52)
#define DEF_EVENT_ID_OCS_COMM_FAIL                (53)
#define DEF_EVENT_ID_CLEAR_OCS_COMM_FAIL          (54)
#define DEF_EVENT_ID_OCS                          (55)
#define DEF_EVENT_ID_RAMAN_COMM_FAIL              (56)
#define DEF_EVENT_ID_CLEAR_RAMAN_COMM_FAIL        (57)




extern int UTIL_RegEventCb(const char *pcSoketPath, EventCb fpEventCb);
extern void UTIL_UnRegEventCb();
extern int UTIL_SendEvent(int iFd, char *pcDestPath, int iEventId, int iSubEventId);



#ifdef __cplusplus
}
#endif

#endif
