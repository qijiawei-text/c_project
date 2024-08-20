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
#ifndef __UTIL_QUEUE_H__
#define __UTIL_QUEUE_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup  define Max node number of monitor queue
 * @{
 */
#define   DEF_QUEUE_NODE_MAX_SIZE                (256)
#define   DEF_QUEUE_MSG_MAX_LEN                  (1024)
/**
 * @}
 */
typedef struct {
    uint32_t ulMsgOid;                                                                                                  /* Oid,which is index of queue member.			   */
    uint32_t ulMsgLen;                                                                                                  /* length of  message.							   */
    char acMsgBuf[DEF_QUEUE_MSG_MAX_LEN];                                               /* buffer holds message.			                           */
} ST_UTIL_QUEUE_MSG;

typedef int (*QuqueCb)(ST_UTIL_QUEUE_MSG *pstQueueNode);
#define QUEUE_FILE  "/var/tmp/eventQueueThread"

extern int8_t UTIL_InitQueue(uint32_t ulMsgLen, uint32_t ulSize);
extern int UTIL_CloseQueue(int8_t cIndex);

extern int UTIL_AddQueueNode(int8_t cIndex, char *pcMsg, uint32_t ulMsgLen);
extern int UTIL_GetQueueNode(int8_t cIndex, char *pcMsg, uint32_t *pulMsgLen);

int UTIL_RegQueueCb(QuqueCb fpQueueCb);
int UTIL_UnRegQueueCb();

#ifdef __cplusplus
}
#endif

#endif
