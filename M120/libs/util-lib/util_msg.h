/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_msg.h
* Description:   message API declarations for the message(libmsg.so) shared library
* Author:         Subsystem Dev team
* History:
*                    [Author]    [Date]         [Version]    [Description]
*                    Yuan Xia   2013/11/06   Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_MSG_H__
#define __UTIL_MSG_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DEF_OOP_MSG_TIME_OUT                     (15)
#define DEF_OOP_MSG_CURRENT_VER                  (0x01)

#define DEF_OOP_MSG_TYPE_REQ                     (0xAA55EE77)
#define DEF_OOP_MSG_TYPE_RESP                    (0xCC33BB55)

#define DEF_OOP_MSG_MAX_LEN                      (1024)
#define DEF_OOP_MSG_HDR_RESV_LEN                 (16)

/**
 * @defgroup  define oop msg oid
 * @{
 */
/* Monitor OID */
#define DEF_OID_MON_CFG                              (0x00000001)                       /* to MONITOR                                              */
#define DEF_OID_MON_HEART_BEAT                       (0x00000002)                       /* to MONITOR (NO ACL)                                     */

/* Log OID */
#define DEF_OID_LOG_ADD_OPLK_LOG                     (0x0F000001)                       /* to Log host   */
#define DEF_OID_LOG_ADD_USER_LOG                     (0x0F000002)                       /* to Log host   */
#define DEF_OID_LOG_SAVE_LOG                         (0x0F000003)                       /* to Log host   */

/* Event OID */
#define DEF_OID_DEV_EVENT                        (0x00010001)
/**
 * @}
 */

/**
 * @defgroup  OOP message header structure(32B)
 * @{
 */
typedef struct {
    uint32_t ulSyncHeader;
    uint8_t ucProtoVer;
    uint8_t ucSyncNo;
    uint8_t ucSlotId;
    uint8_t ucResv;
    uint32_t ulOid;
    uint16_t usMsgStat;
    uint16_t usPayloadLen;
    uint8_t aucResv[DEF_OOP_MSG_HDR_RESV_LEN];

} ST_OOP_MSG_HEADER;
/**
 * @}
 */


extern int     UTIL_CreateMsgSock(char *pcPath);
extern void    UTIL_CloseMsgSock(char *pcPath, int iFd);

extern int     UTIL_SendAndRecvOopMsg(int iFd, char *pcMsgBuf, int32_t lBufLen, struct sockaddr_un *pSrvAddr,
                                      char *pcRecvBuf, int32_t *plRecvLen);
extern int     UTIL_SendOopMsg(int iFd, char *pcMsgBuf, int32_t lBufLen, char *pcPath);

extern int32_t UTIL_CreateOopMsg(char *pcOopMsg, uint32_t ulSyncHdr, uint8_t ucSyncNo, uint8_t ucSlotId,
                                 uint32_t ulOid, uint16_t usMsgStat, char *pcPayload, uint16_t usPayloadSize);

extern int     UTIL_ChkOopMsg(char *pcMsgBuf, int32_t lBufLen);


#ifdef __cplusplus
}
#endif

#endif
