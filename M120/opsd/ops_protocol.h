#ifndef __OPS_PROTOCOL_H__
#define __OPS_PROTOCOL_H__

#include "DevOpsData.h"

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************************************/
/* Definition for APS PDU bytes, opCode and TLV offset                                          */
/************************************************************************************************/
/* @ {                                                                                          */
#define APS_PDU_BYTES       (9)
#define APS_PDU_OPCODE      (39)
#define APS_PDU_TLV_OFFSET  (4)
/* @ }                                                                                          */


/************************************************************************************************/
/* APS Request/State described from G.8031:                                                     */
/*      1111 Lockout of protection (LO)                                                         */
/*      1110 Signal fail on protection (SF-P)                                                   */
/*      1101 Forced switch (FS)                                                                 */
/*      1011 Signal fail for working (SF)                                                       */
/*      1001 Signal degrade (SD)                                                                */
/*      0111 Manual switch (MS)                                                                 */
/*      0110 Deprecated                                                                         */
/*      0101 Wait-to-restore (WTR)                                                              */
/*      0100 Exercise (EXER)                                                                    */
/*      0010 Reverse request (RR)                                                               */
/*      0001 Do not revert (DNR)                                                                */
/*      0000 No request (NR)                                                                    */
/************************************************************************************************/
/* @ {                                                                                          */
#define APS_STATE_LO    0xF
#define APS_STATE_SF_P  0xE
#define APS_STATE_FS    0xD
#define APS_STATE_SF_W  0xB
#define APS_STATE_MS    0x7
#define APS_STATE_WTR   0x5
#define APS_STATE_RR    0x2
#define APS_STATE_DNR   0x1
#define APS_STATE_NR    0x0
/* @ }                                                                                          */


/************************************************************************************************/
/* Definition for APS Protection Type                                                           */
/************************************************************************************************/
/* @ {                                                                                          */
#define APS_PROT_TYPE_A_NO_CHANEL   (0)
#define APS_PROT_TYPE_A_CHANEL      (1)
#define APS_PROT_TYPE_B_1P1         (0)
#define APS_PROT_TYPE_B_1T1         (1)
#define APS_PROT_TYPE_D_UNIDIR      (0)
#define APS_PROT_TYPE_D_BIDIR       (1)
#define APS_PROT_TYPE_R_NO_REVERT   (0)
#define APS_PROT_TYPE_R_REVERT      (1)
/* @ }                                                                                          */


/************************************************************************************************/
/* Definition of Null and Normal traffic signal for Requested/Bridged signal                    */
/************************************************************************************************/
/* @ {                                                                                          */
#define APS_SIGNAL_NULL             (0)
#define APS_SIGNAL_NORMAL           (1)
/* @ }                                                                                          */


/************************************************************************************************/
/* Definition APS PDU frame.                                                                    */
/* The detail description about PDU please see G8031-201803 page 25.                            */
/************************************************************************************************/
/* @ {                                                                                          */
typedef struct st_aps_pdu_fmt {
    uint8_t ver : 5;
    uint8_t mel : 3;
    uint8_t opcode;
    uint8_t flags;
    uint8_t tlv_offset;
    /* APS-Specific Information, 4 bytes                                                        */
    COpsReqStatus status;
    /* END TLV                                                                                  */
    uint8_t end_tlv;
} OPS_PDU_FMT_ST;
/* @ }                                                                                          */


typedef union un_aps_req_sta {
    /* 9 bytes over the wire, per ITUT- G8031                                                   */
    char pchApsFrames[APS_PDU_BYTES];
    OPS_PDU_FMT_ST stApsPduFmt;
} OPS_REQUEST_UN;

typedef struct THREAD_INFO_st {
    /* Process thread id.                                                                       */
    pthread_t tId;
    /* Process thread name.                                                                     */
    /* !!! length is restricted to 16 characters, including the terminating null byte ('\0')    */
    const char *pcName;
    bool bExitLoop;
} OPS_THREAD_ST;

int OPS_Client_StartThreads();
void OPS_Client_StopThreads();
int OPS_Server_StartThreads();
void OPS_Server_StopThreads();
void OPS_SendAPSRapidMessage(int iSubEventId);

#ifdef __cplusplus
}
#endif
#endif

