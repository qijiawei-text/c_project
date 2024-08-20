/*************************************************************************
*
*            Copyright (c) 2016 by Microsemi Corp. Inc.
*
*  This software is copyrighted by, and is the sole property of Microsemi
*  Corp. All rights, title, ownership, or other interests in the
*  software  remain the property of Microsemi Corp. This software
*  may only be used in accordance with the corresponding license
*  agreement.  Any unauthorized use, duplication, transmission,
*  distribution, or disclosure of this software is expressly forbidden.
*
*  This Copyright notice may not be removed or modified without prior
*  written consent of Microsemi Corp.
*
*  Microsemi Corp. reserves the right to modify this software without notice.
*
*************************************************************************
*
*  File Revision: 1.0
*
*************************************************************************
*
*  Description: Analysis And Execution of the Commands
*
*
*************************************************************************/

/*=========================================================================
   / INCLUDES
   /========================================================================*/

#include "stdio.h"
#include "string.h"
#include "mscc_arch_functions.h"
#include "mscc_poe_communication_protocol.h"
#include "mscc_poe_device_communication.h"
#include "mscc_poe_util.h"
#include "mscc_poe_global_internal.h"


/*=========================================================================
   / TYPES
   /========================================================================*/
typedef enum
{
    e_MSG_TYPE_NONE, e_MSG_VALID_TELEMETRY, e_MSG_VALID_REPLY,
} TX_REPLY_TYPE_e;

/*=========================================================================
   / LOCALS
   /========================================================================*/

static U8 bTxMsg[MSG_SIZE];
static U8 bRxMsg[RX_BUFF_SIZE];

/*=========================================================================
   / LOCAL PROTOTYPES
   /==========================================================================*/

#define B_CKSM_L     0  /* soft need to place correct value */
#define B_CKSM_H     0  /* soft need to place correct value */

/*=========================================================================
   / FUNCTIONS
   /==========================================================================*/

PoE_Status_e poe_communication_protocol_Build_Protocol_TxMsg(
    Protocol_15Bytes_Msg_Type_e MsgType,
    _INOUT U8 *pDataArr,
    PoE_Msg_t *ptPoeMsg,
    _OUT U16 *wCommandDelay_mSec);

PoE_Status_e poe_communication_protocol_POE_Validate_Report_RxMsg(U8 *pRxMsg);

PoE_Status_e poe_communication_protocol_POE_Decode_Msg(
    Protocol_15Bytes_Msg_Type_e MsgType,
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 *pRxMsg);

PoE_Status_e poe_communication_protocol_wait_for_rx(
    U8 bIC_Address,
    _OUT TX_REPLY_TYPE_e *eTxReplyType,
    U8 byEcho,
    U16 wCommandDelay_mSec);

/*---------------------------------------------------------------------
 *    description:     This function Initialize the PoE API Software
 *                     assign two ptr to function that given from the Host for I2C write and read operations.
 *
 *    input :   pInitInfo           - pointer to mscc_InitInfo_t struct which contain initialization information data
 *    output:   none
 *    return:   ePOE_STATUS_OK     - operation succeed
 *              != ePOE_STATUS_OK  - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
ePOE_COMMUNICATION_PROTOCOL_InitSoftware(Init_Info_t *ptInitInfo)
{
    /* Host driver functions */
    ptMscc_fptr_write = ptInitInfo->fptr_write;
    ptMscc_fptr_read = ptInitInfo->fptr_read;

    ePOE_BOOL_Printf_Enable = ptInitInfo->ePOE_BOOL_Printf_Enable;

    /* limit Communication Timeout Offset value to 10 seconds*/
    if (ptInitInfo->wExtra_Timeout_mSec > 10000)
        ptInitInfo->wExtra_Timeout_mSec = 10000;

    wExtra_Timeout_mSec = ptInitInfo->wExtra_Timeout_mSec;
    eDevice_Method = ptInitInfo->eDevice_Method;

    /* zeroing counters struct */
    memset(&tCountersInternal, 0, sizeof(Counters_t));

    return ePOE_STATUS_OK;
}


/* its the first message let's clean the MCU output buffer - system status telemetry in case he had hardware reset event */
PoE_Status_e
ePOE_COMMUNICATION_PROTOCOL_Clear_PoE_Device_Buffer(U8 bIC_Address)
{
    PoE_Status_e eResult;

    /* Try to read 15bytes - Get system status with ECHO 255 - from PoE Device */
    U8 byArrLocalBuffer[RX_BUFF_SIZE];
    POE_BOOL_e ePOE_BOOL_Is_system_status = ePOE_FALSE;

    eResult = eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(bIC_Address, B_SYSTEM_STATUS_ECHO, byArrLocalBuffer, &ePOE_BOOL_Is_system_status);

    /* Empty all data from RX buffer */
    ClearRxBuffer(bIC_Address);

    /* check if we got hardware reset - system status with ECHO 255 */
    if ((bRxMsg[0] == B_Telemetry) && (bRxMsg[1] == B_SYSTEM_STATUS_ECHO) && (eResult == ePOE_STATUS_OK)) {
        /* check system validation
           eHardwareResetDetected_ERR_PoE_controller_error
           eHardwareResetDetected_ERR_firmware_download_is_required
           eHardwareResetDetected_ERR_PoE_controller_memory_error       */
        if (((bRxMsg[2] & 1) == 0) && ((bRxMsg[2] & 2) == 0) && ((bRxMsg[3] & 1) == 0)) {
            return ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED;
        } else {
            MSCC_PRINTF_INFO(" POE MCU status: CPU status1: %02x \n\r", bRxMsg[2]);             /* error */
            if (bRxMsg[3] != 0x4E)
                MSCC_PRINTF_INFO("CPU status2: %02x \n\r", bRxMsg[3]);                 /* error */

            if ((bRxMsg[2] & 1) == 1)
                return ePOE_STATUS_ERR_POE_MCU_FIRMWARE_ERROR;
            else if ((bRxMsg[2] & 2) == 2)
                return ePOE_STATUS_ERR_POE_MCU_FIRMWARE_DOWNLOAD_IS_REQUIRED;
        }
    }

    return eResult;
}



/*---------------------------------------------------------------------
 *    description:    This function  Analysis And Execution Command  at the Communication
 *
 *    input :   eMsgType          - the specific 15bytes communication protocol command/telemetry
 *              bIC_Address       - the PoE device I2C address
 *    output:   none
 *
 *    inout:    ptPoeMsg          - pointer to generic message struct - must be zeroed before useing this function
 *                                  this struct conatain the arguments to be send by the specific protocol command.
 *
 *    return:   ePOE_STATUS_OK   - operation succeed
 *              < ePOE_STATUS_OK - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
ePOE_COMMUNICATION_PROTOCOL_AnalysisAndExecutionCommand(Protocol_15Bytes_Msg_Type_e eMsgType, _INOUT PoE_Msg_t *ptPoeMsg, U8 bIC_Address)
{
    PoE_Status_e eResult;
    TX_REPLY_TYPE_e eMsgReplyType = e_MSG_TYPE_NONE;
    U16 wCommandDelay_mSec = 50;

    if ((eResult = poe_communication_protocol_Build_Protocol_TxMsg(eMsgType, bTxMsg, ptPoeMsg, &wCommandDelay_mSec)) < 0)
        if (eResult != ePOE_STATUS_OK)
            return eResult;

    U8 byEcho = bTxMsg[1];

    /* send message to serial port */
    eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, MSG_SIZE);
    if (eResult != ePOE_STATUS_OK)
        return eResult;

    /* adding delay for Rx delay */
    U16 wTotalCommandDelay_mSec = wCommandDelay_mSec + wExtra_Timeout_mSec;

    eResult = poe_communication_protocol_wait_for_rx(bIC_Address, &eMsgReplyType, byEcho, wTotalCommandDelay_mSec);
    if (eResult != ePOE_STATUS_OK)
        return eResult;

    /* check if we got hardware reset - system status with ECHO 255 */
    if ((bRxMsg[0] == B_Telemetry) && (bRxMsg[1] == B_SYSTEM_STATUS_ECHO)) {
        /* check system valid */
        if ((bRxMsg[2] & 1) != 0)
            ptPoeMsg->eHardwareResetDetected = eHardwareResetDetected_ERR_PoE_controller_error;
        else if ((bRxMsg[2] & 2) != 0)
            ptPoeMsg->eHardwareResetDetected = eHardwareResetDetected_ERR_firmware_download_is_required;
        else if ((bRxMsg[3] & 1) != 0)
            ptPoeMsg->eHardwareResetDetected = eHardwareResetDetected_ERR_PoE_controller_memory_error;

        /* read the original message reply/telemetry to clean the Host buffer - Empty all data from RX buffer */
        ClearRxBuffer(bIC_Address);

        /* if we are here - we pass the get system status and clear the original reply/tele from the host buffer */
        return ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED;         /* return status that hardware reset occured !!!!! */
    }

    /* in case that ECHO number in incorrect lets clean the MCU buffer */
    if (bTxMsg[1] != bRxMsg[1]) {
        /* clear device I2C buffer - Read 15bytes reply from PoE Device - Empty all data from RX buffer */
        ClearRxBuffer(bIC_Address);

        return ePOE_STATUS_ERR_RX_MSG_ECHO_MISMATCH;
    }

    if ((bTxMsg[0] == B_Command) || (bTxMsg[0] == B_Program)) {
        if (eMsgReplyType == e_MSG_VALID_REPLY) {
            if (eMsgType == eReset_Command) {
                /* Wait for reply from PoE device */
                eResult = OS_Sleep_mS(4000);
                if (eResult != ePOE_STATUS_OK)
                    return eResult;

                POE_BOOL_e ePOE_BOOL_Is_system_status = ePOE_FALSE;

                /* Read 15bytes reply from PoE device */
                eResult = eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(bIC_Address, B_SYSTEM_STATUS_ECHO, bRxMsg, &ePOE_BOOL_Is_system_status);
                if (eResult != ePOE_STATUS_OK) {
                    /* if checksum failed return as error */
                    if (eResult == ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR)
                        return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;

                    tCountersInternal.tErrorCounters.ulCommunicationTimeout++;                     /* increment time out counter */
                    return eResult;
                }


                /* if recived data is zero and not ready yet - lets try to read again */
                if (CheckIfArrayHasTheSameContent(bRxMsg, 0, RX_BUFF_SIZE) == ePOE_TRUE)
                    return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;

                /* recived data Check Sum test - if checksum failed return as error */
                eResult = eMscc_POE_UTIL_CheckCommCheckSumValidity(bRxMsg);
                if (eResult != ePOE_STATUS_OK)
                    return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;

                /* check if we rec get system status Telemetry RX message  ( result of reset command ) */
                if ((bRxMsg[0] == B_Telemetry) && (bRxMsg[1] == 255)) {               /* CorrectReply = 1 */
                    tCountersInternal.ulSucceedMsgsCntr++;                     /* inc cntr that count how many messages were send before func() exit */
                    /* *eTxReplyType = e_MSG_VALID_TELEMETRY;*/
                } else
                    return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES;                     /* We rec valid telemetry, exit retry loop */

                /* extract data from 15bytes and build the Get struct */
                eResult = poe_communication_protocol_POE_Decode_Msg(eGet_System_Status, ptPoeMsg, bRxMsg);
                if (eResult != ePOE_STATUS_OK)
                    return eResult;
            } else
                return ePOE_STATUS_OK;
        } else
            return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES;
    } else if (bTxMsg[0] == B_Request) {
        if (eMsgReplyType == e_MSG_VALID_TELEMETRY) {
            /* extract data from 15bytes and build the Get struct */
            eResult = poe_communication_protocol_POE_Decode_Msg(eMsgType, ptPoeMsg, bRxMsg);
            if (eResult != ePOE_STATUS_OK)
                return eResult;
        } else
            return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES;
    }

    return ePOE_STATUS_OK;
}

/*---------------------------------------------------------------------
 *    description:    This function wait and read 15bytes of data from the PoE device
 *
 *    input :   bTxMsg - 15 bytes communication protocol command/request
 *              bIC_Address      - the PoE device I2C address
 *
 *    output:   bRxMsg - 15 bytes communication protocol response/telemetry
 *              eTxReplyType - pointer for enum which indicate about the type of the recived 15byes data,
 *                             whether it reply or telemetry
 *
 *    return:   e_POE_STATUS_OK_VALID_REPLY     - operation succeed
 *                      e_POE_STATUS_OK_VALID_TELEMETRY - operation succeed
 *
 *              < ePOE_STATUS_OK               - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
poe_communication_protocol_wait_for_rx(U8 bIC_Address, _OUT TX_REPLY_TYPE_e *eTxReplyType, U8 byEcho, U16 wCommandDelay_mSec)
{
#define MAX_TX_RETRY_NUM 3
    U8 Retry;
    PoE_Status_e eResult;

    for (Retry = 0; Retry < MAX_TX_RETRY_NUM; Retry++) {
        if (eDevice_Method != eDevice_Method_UART_Async) {
            /* Wait for reply from PoE device */
            eResult = OS_Sleep_mS(wCommandDelay_mSec);
            if (eResult != ePOE_STATUS_OK)
                return eResult;
        }

        /* MSCC_PRINTF_INFO("wait: %d\n\r",wCommandDelay_mSec);  */

        wCommandDelay_mSec = 40;         /* second and third transmission has 40mSec delay */

        POE_BOOL_e ePOE_BOOL_Is_system_status = ePOE_FALSE;

        /* Read 15bytes reply from PoE device */
        eResult = eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(bIC_Address, byEcho, bRxMsg, &ePOE_BOOL_Is_system_status);
        if (eResult != ePOE_STATUS_OK) {
            /* if checksum failed return as error */
            if (eResult == ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR)
                return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;

            tCountersInternal.tErrorCounters.ulCommunicationTimeout++;             /* increment time out counter */
            continue;
            /*return eResult;*/
        }

        /* if received data is zero and not ready yet - lets try to read again */
        if (CheckIfArrayHasTheSameContent(bRxMsg, 0, RX_BUFF_SIZE) == ePOE_TRUE) {
            tCountersInternal.tErrorCounters.ulRxRetry++;
            continue;
        }


        /* check if we rec REPORT RX message  ( result of SET message, or error for 'REQUEST' message ) */
        if (bRxMsg[0] == B_Report) {
            eResult = poe_communication_protocol_POE_Validate_Report_RxMsg(bRxMsg);
            if (eResult == ePOE_STATUS_OK) {           /* Correct Reply */
                tCountersInternal.ulSucceedMsgsCntr++;                 /* inc cntr that count how many messages were send before func() exit */
                *eTxReplyType = e_MSG_VALID_REPLY;
            }

            return eResult;             /* no need to retry, since command was rec by PoE device */
        }        /* check if we rec B_Telemetry RX message  ( result of REQUEST message ) */
        else if (bRxMsg[0] == B_Telemetry) {       /* CorrectReply = 1 */
            tCountersInternal.ulSucceedMsgsCntr++;             /* inc cntr that count how many messages were send before func() exit */
            *eTxReplyType = e_MSG_VALID_TELEMETRY;
            return ePOE_STATUS_OK;             /* We rec valid telemetry, exit retry loop */
        } else
            tCountersInternal.tErrorCounters.tUnitErrors.ulFailed_Execution_Undefined_Key_Value++;

        return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES;
    }     /* end for loop */

    return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
}

/********************************************************************
 *    description:    This function is responsible to build 15 byte TX message to be send
 *                    to PoE Device. The function doen't realy on ony specific HW.
 *                    It get an enum which define which message to build, ptr were to place
 *                    15 byte message, and a ptr to a union which contain the various parameters
 *                    which are required for each command.
 *
 *    input :   eMsgType          - enum which message has to be constructed
 *              ptPoeMsg        - ptr to a message struct which conatain the arguments to be send
 *                                  by the specific protocol command/telemetry.
 *
 *    output:   pDataArr          - ptr were to place the 15 bytes communication protocol command/request
 *              wCommandDelay_mSec- contain the delay of the specific command/request between the write and read
 *                                  I2C transaction
 *
 *    return:   ePOE_STATUS_OK   - operation succeed
 *              < ePOE_STATUS_OK - operation failed
 **********************************************************************/
PoE_Status_e
poe_communication_protocol_Build_Protocol_TxMsg(
    Protocol_15Bytes_Msg_Type_e eMsgType, _OUT U8 *pDataArr,
    PoE_Msg_t *ptPoeMsg, _OUT U16 *wCommandDelay_mSec)
{
    static U8 Echo_Id = 0;     /* is kept & increment internaly by the function */
    U16 chksum = 0;
    U16 x;

    switch (eMsgType) {

    /******************************  4.1 System messages  ******************************/

    /* 4.1.1 Reset Command */
    case eReset_Command:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Reset, 0, B_Reset, 0, B_Reset, B_N, B_N,
                B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.2 Restore Factory Default */
    case eRestore_Factory_Defaults:
    {
        *wCommandDelay_mSec = 3000;
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Program,
                Echo_Id, B_RestoreFact, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N,
                B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.3 Save System Settings */
    case eSave_System_Settings:
    {
        *wCommandDelay_mSec = 400;
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Program,
                Echo_Id, B_E2, B_SaveConfig, B_N, B_N, B_N, B_N, B_N, B_N, B_N,
                B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.4 Save User Byte */
    case eSet_User_Byte:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Program,
                Echo_Id, B_UserByte,
                ptPoeMsg->PoE_Msg_Prm_e.Set_User_Byte_t.Set.bUserByte, B_N,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.5 Set System Status */
    case eSet_System_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_SystemStatus,
                ptPoeMsg->PoE_Msg_Prm_e.Set_System_Status_t.Set.bPrivate_Label,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.6 Get System Status */
    case eGet_System_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_SystemStatus,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.1.7 Get System Status2 */
    case eGet_System_Status2:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_SystemStatus2,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }


    /* 4.1.8 Set Interrupt Mask */
    case eSet_Interrupt_Mask:
    {
        U16 wRegister_Mask = 0;

        /* 0xFF: unmask all Blink_Register events */
        wRegister_Mask = ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortTurnedOn;                                    /* 0 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortTurnedOff << 1;                             /* 1 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bDetectionUnsuccessful << 2;                     /* 2 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortFault << 3;                                     /* 3 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInUnderLoad << 4;                        /* 4 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInOverLoad << 5;                             /* 5 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInPM << 6;                               /* 6 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPort_Power_Up_denied << 7;                      /* 7 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bDisconnectionTemperature << 8;                  /* 8 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bUserDefinedTemperature << 9;                    /* 9 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPoEDeviceFault << 10;                           /* 10 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bNoMoreConnect << 12;                            /* 12 */
        wRegister_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bVmainFault << 13;                               /* 13 */

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_IrqMask, W_H(wRegister_Mask),
                W_L(wRegister_Mask),
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.9 Get Interrupt Mask */
    case eGet_Interrupt_Mask:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_IrqMask,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.1.10 Set Individual Mask */
    case eSet_Individual_Mask:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Individual_Mask,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Individual_Mask_t.Set.eINDV_MASK_OPT,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Individual_Mask_t.Set.ePOE_BOOL_EnDis,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.11 Get Individual Mask */
    case eGet_Individual_Mask:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Individual_Mask,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Individual_Mask_t.Set.eINDV_MASK_OPT,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.12 Set System OK LED Mask Registers */
    case eSet_System_OK_LED_Mask_Registers:
    {
        U16 wBlink_Register_Mask = 0;
        U16 wLight_Register_Mask = 0;

        /* 0xFF: unmask all Blink_Register events */
        wBlink_Register_Mask = ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortTurnedOn;                                    /* 0 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortTurnedOff << 1;                             /* 1 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bDetectionUnsuccessful << 2;                     /* 2 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortFault << 3;                                 /* 3 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInUnderLoad << 4;                        /* 4 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInOverLoad << 5;                         /* 5 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInPM << 6;                               /* 6 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPort_Power_Up_denied << 7;                      /* 7 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bDisconnectionTemperature << 8;                  /* 8 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bUserDefinedTemperature << 9;                    /* 9 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPoEDeviceFault << 10;                           /* 10 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bNoMoreConnect << 12;                            /* 12 */
        wBlink_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bVmainFault << 13;                               /* 13 */

        /* 0xFF: unmask all Light_Register events */
        wLight_Register_Mask = ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortTurnedOn;                                    /* 0 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortTurnedOff << 1;                             /* 1 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bDetectionUnsuccessful << 2;                     /* 2 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortFault << 3;                                 /* 3 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInUnderLoad << 4;                            /* 4 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInOverLoad << 5;                         /* 5 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInPM << 6;                               /* 6 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPort_Power_Up_denied << 7;                      /* 7 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bDisconnectionTemperature << 8;                  /* 8 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bUserDefinedTemperature << 9;                    /* 9 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPoEDeviceFault << 10;                           /* 10 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bNoMoreConnect << 12;                            /* 12 */
        wLight_Register_Mask |= ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bVmainFault << 13;                               /* 13 */

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_SystemOKMask, W_H(wBlink_Register_Mask),
                W_L(wBlink_Register_Mask), W_H(wLight_Register_Mask),
                W_L(wLight_Register_Mask),
                B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.1.13 Get System OK LED Mask Registers */
    case eGet_System_OK_LED_Mask_Registers:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_SystemOKMask,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }


    /* 4.1.14 Set Power Indication LED */
    case eSet_Power_Indication_LED:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_PowerIndication,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOn_Percent,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOff_Percent,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOn_W,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOff_W,
                B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }




    /* 4.1.15 Get Power Indication LED */
    case eGet_Power_Indication_LED:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_PowerIndication,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }


    /* 4.1.16 Set System Masks - obsolete - please use individual mask instead */


    /* 4.1.17 Get Masks Status - obsolete - please use individual mask instead */


    /* 4.1.18 Get Software Version */
    case eGet_Software_Version:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Versionz, B_SWversion,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /******************************  4.2 PoE Device messages  ******************************/

    /* 4.2.1 Set PoE Device Parameters */
    case eSet_PoE_Device_Parameters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_DeviceParams,
                ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Parameters_t.Set.bCS_Num,
                B_N,
                (U8)((ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Parameters_t.Set.bTSH_C)),
                B_N,
                B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.2.2 Get PoE Device Status */
    case eGet_PoE_Device_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_DeviceParams,
                ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Set.bCS_Num, 0,
                0, 0, 0, 0, 0, 0, 0, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.2.3 Set PoE Device Register */
    case eSet_PoE_Device_Register:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Test,
                Echo_Id, B_Test, B_WriteRons,
                W_H(ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Address),
                W_L(ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Address),

                W_H(ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Data),
                W_L(ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Data),

                ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.bDeviceNumber,
                B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.2.4 Get PoE Device Register */
    case eGet_PoE_Device_Register:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Test, B_ReadsRon,
                W_H(ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Set.wRegister_Address),
                W_L(ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Set.wRegister_Address),
                ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Set.bDeviceNumber,
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }



    /******************************  4.3 PoE Port messages  ******************************/

    /* 4.3.1 Set Temporary Matrix */
    case eSet_Temporary_Matrix:
    {
        *wCommandDelay_mSec = 70;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_TmpMatrix,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bPhysical_Number_A,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bPhysical_Number_B,
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.2 Get Physical Port Number from Temporary Matrix */
    case eGet_Physical_Port_Number_from_Temporary_Matrix:
    {
        *wCommandDelay_mSec = 70;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_TmpMatrix,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Temporary_Matrix_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.3 Program Global Matrix */
    case eProgram_Global_Matrix:
    {
        *wCommandDelay_mSec = 1000;
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_TmpMatrix,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.3.4 Get Physical Port Number from Active Matrix */
    case eGet_Physical_Port_Number_from_Active_Matrix:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_ChannelMatrix,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Active_Matrix_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.5 Set Enable/Disable Channels */
    case eSet_Enable_Disable_Channels:
    {
        *wCommandDelay_mSec = 150;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_EnDis,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.byPoeCmd,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.eTYPE_MASK,
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.6 Get All Ports Enable/Disable */
    case eGet_All_Ports_Enable_Disable:
    {
        *wCommandDelay_mSec = 100;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_EnDis,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }


    /* 4.3.7 Set Force Power */
    case eSet_Force_Power:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_ForcePower,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Force_Power_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Force_Power_t.Set.ePOE_BOOL_EnDis,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.8 Set Port Parameters */
    case eSet_Port_Parameters:
    {
        *wCommandDelay_mSec = 150;

        /* convert from mWatt to deciWatt */
        U16 wPPL_mW =
            (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.ulPPL_mW);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_PortFullInit,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.byPoeCmd,
                W_H(wPPL_mW), W_L(wPPL_mW),
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.ePortPriority,
                B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.9 Set Power Limit for Channels */
    case eSet_Power_Limit_for_Channels:
    {
        /* convert from mWatt to miliWatt */
        U16 wPPL_mW =
            (U16)ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limit_for_Channels_t.Set.ulPPL_mW;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Supply,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limit_for_Channels_t.Set.bCh_Num,
                W_H(wPPL_mW), W_L(wPPL_mW),
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.10 Set Temporary Power Limit for Channels */
    case eSet_Temporary_Power_Limit_for_Channels:
    {
        /* convert from mWatt to miliWatt */
        U16 wTPPL_mW =
            (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limit_for_Channels_t.Set.ulTPPL_mW);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_TemporarySupply,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limit_for_Channels_t.Set.bCh_Num,
                W_H(wTPPL_mW), W_L(wTPPL_mW),
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.11 Get Port Power Limit */
    case eGet_Port_Power_Limit:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Supply,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Power_Limit_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.12 Set 4-Pair Port Parameters */
    case eSet_4Pair_Port_Parameters:
    {
        *wCommandDelay_mSec = 150;

        /* convert from mWatt to 5mW units */
        U16 wPPL_4Pair_5mW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ulPPL4Pair_mW / 5);

        U8 bPortPM2PM3 = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPM2 |
                              (ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPM3 << 4));

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_PortFullInit4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.byPoeCmd,
                W_H(wPPL_4Pair_5mW),
                W_L(wPPL_4Pair_5mW),
                (U8)ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPriority,
                (U8)ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePort_Type,
                (U8)ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.eSum_as_TPPL_PM_algorithm,
                bPortPM2PM3,
                B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.3.13 Get 4-Pair Port Parameters */
    case eGet_4Pair_Port_Parameters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_PortFullInit4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }



    /* 4.3.14 Set Power Limits for 4-Pair Channels */
    case eSet_Power_Limits_for_4_Pair_Channels:
    {
        /* convert from mWatt to 5mW units */
        U16 wPPL_4Pair_5mW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limits_for_4_Pair_Channels_t.Set.ulPPL4Pair_mW / 5);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Supply4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limits_for_4_Pair_Channels_t.Set.bCh_Num,
                W_H(wPPL_4Pair_5mW), W_L(wPPL_4Pair_5mW),
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.15 Set Temporary Power Limits for 4-Pair Channels */
    case eSet_Temporary_Power_Limits_for_4_Pair_Channels:
    {
        /* convert from mWatt to 5mW units */
        U16 wTPPL_4Pair_5mW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limits_for_4_Pair_Channels_t.Set.ulTPPL4Pair_mW / 5);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_TemporarySupply4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limits_for_4_Pair_Channels_t.Set.bCh_Num,
                W_H(wTPPL_4Pair_5mW), W_L(wTPPL_4Pair_5mW),
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.16 Get 4-Pair Port Power Limit */
    case eGet_4Pair_Port_Power_Limit:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Supply4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Power_Limit_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.17 Set Port Priority */
    case eSet_Port_Priority:
    {
        *wCommandDelay_mSec = 150;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Priority,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Priority_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Priority_t.Set.ePortPriority,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.18 Get Port Priority */
    case eGet_Port_Priority:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Priority,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Priority_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.19 Set Enable/Disable 4-Pair for Channels */
    case eSet_Enable_Disable_4_Pair_for_Channels:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_EnDis4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_4_Pair_for_Channels_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_4_Pair_for_Channels_t.Set.byPoeCmd,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.20 Get All Ports Enable/Disable 4-Pair */
    case eGet_All_Ports_Enable_Disable_4pair:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_EnDis4Pair,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.3.21 Get Single Port Status */
    case eGet_Single_Port_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_PortStatus,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.22 Get All Ports Status */
    case eGet_All_Ports_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Set.ePortsStatusGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }


    /* 4.3.23 Get Extended Port Status */
    case eGet_Extended_Port_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_NewPortStatus,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.3.24 Get Internal Port Status (For Debug usage) */
    case eGet_Internal_Port_Status:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_PortStatus,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.3.25 Get UDL Counters */
    case eGet_UDL_Counters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Set.eCounterGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.3.26 Get Detection Failure Counters */
    case eGet_Detection_Failure_Counters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Set.eDetCntGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.3.27 Get Latches (Old Request to support backwards compatibility) */
    case eGet_Latches:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Set.eLatchesGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.3.28 Get Global Port Counters */
    case eGet_global_port_counters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_PortCounters,
                ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterKind,
                ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.29 Get All Ports Class */
    case eGet_All_Ports_Class:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_AllPortClass,
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Set.ePortsClassGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.3.30 Get All Ports delivering power state */
    case eGet_All_Ports_delivering_power_state:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_AllPortsDelivering,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.3.31 Get Port Event Cause  */
    case eGet_Port_Event_Cause:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Event,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }




    /******************************  4.4 Power Management Related Messages  ******************************/

    /* 4.4.1 Set PM Method */
    case eSet_PM_Method:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Supply, B_PowerManageMode,
                ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM1_system_power_calculated_method,
                ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM2_Port_Power_Limit,
                ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM3_Start_up_conditions,
                B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.2 Get PM Method */
    case eGet_PM_Method:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_PowerManageMode,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.3 Get Total Power */
    case eGet_Total_Power:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_ExpendedPowerInfo,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.4 Set Power Banks */
    case eSet_Power_Banks:
    {
        /* convert from miliWatt to Watt */
        U16 wPowerLimit_W = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulPowerLimit_mW / 1000);

        /* convert from miliVolt to deciVolt */
        U16 wMaxShutdownVoltage_dV = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulMaxShutdownVoltage_mV / 100);
        U16 wMinShutdownVoltage_dV = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulMinShutdownVoltage_mV / 100);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Supply, B_PowerBudget,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.bBank,
                W_H(wPowerLimit_W), W_L(wPowerLimit_W),
                W_H(wMaxShutdownVoltage_dV), W_L(wMaxShutdownVoltage_dV),
                W_H(wMinShutdownVoltage_dV), W_L(wMinShutdownVoltage_dV),
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.bGuardBand,
                B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.5 Get Power Banks */
    case eGet_Power_Banks:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_PowerBudget,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Set.bBank,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.6 Get Power Supply Parameters */
    case eGet_Power_Supply_Parameters:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_Main,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.7 Get Power Supply Voltage */
    case eGet_Power_Supply_Voltage:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_Measurementz,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.8 Get All Ports Power */
    case eGet_All_Ports_Power:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Set.ePortsPowerGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.4.9 Get All 4-Pair Ports Power */
    case eGet_All_4Pair_Ports_Power:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global,
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Set.ePorts4PairPowerGroup,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H,
                B_CKSM_L);
        break;
    }

    /* 4.4.10 Get All HIP Ports Power - obsolete - Please use the "Get All 4-Pair Ports Power" instead */

    /* 4.4.11 Get Port Measurements */
    case eGet_Port_Measurements:
    {
        *wCommandDelay_mSec = 60;

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Paramz,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Measurements_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.4.12 Get 4-Pair Port Measurements */
    case eGet_4Pair_Port_measurements:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Paramz4Pair,
                ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /******************************  4.5 Power Derating, Related Messages  ******************************/

    /* 4.5.1 Set Derating Data */
    case eSet_Derating_Data:
    {
        /* convert from mWatt to deciWatt */
        U16 wMax_PS_Power_W = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulMax_PS_Power_mW / 1000);
        U16 wDerating_Delta_Power_dW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulDerating_Delta_Power_mW / 100);

        U8 bTstart_C = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulTstart_mC / 1000);
        U8 bTshutdown_C = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulTshutdown_mC / 1000);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Derating, B_PowerBudget,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.bBank,
                W_H(wMax_PS_Power_W), W_L(wMax_PS_Power_W), bTstart_C,
                bTshutdown_C, W_H(wDerating_Delta_Power_dW),
                W_L(wDerating_Delta_Power_dW),
                B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.5.2 Get Derating Data */
    case eGet_Derating_Data:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Derating, B_PowerBudget,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Set.bBank,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.5.3 Set Derating User Temperature */
    case eSet_Derating_User_Temperature:
    {
        /* convert from mC to deciC */
        U16 wTemperature_dC = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_User_Temperature_t.Set.ulTemperature_mC / 100);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Derating, B_DeratingUserTemp,
                W_H(wTemperature_dC), W_L(wTemperature_dC),
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.5.4 Get Derating User Temperature */
    case eGet_Derating_User_Temperature:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Derating, B_DeratingUserTemp,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.5.5 Get System Measurements */
    case eGet_System_Measurements:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Global, B_Supply, B_ActualSystemData,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /******************************  4.6 Laye2 Related Messages  ******************************/

    /* 4.6.1 Set Port Layer2 LLDP PD Data */
    case eSet_Port_Layer2_LLDP_PD_Data:
    {
        /* convert from mWatt to deciWatt */
        U16 wPD_Requested_Power_dW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.ulPD_Requested_Power_mW / 100);
        U16 wPSE_Allocated_Power_dW = (U16)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.ulPSE_Allocated_Power_mW / 100);

        ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.byReserved = 0;

        U8 bSet_Layer2_PD_Type = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePortPriority;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.byReserved << 2;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePower_source << 4;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePD_Power_type << 6;

        U8 bExecute_LLDP = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tExecute_LLDP.Bit0_Execute_LLDP;
        bExecute_LLDP |= (ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tExecute_LLDP.Bit1_LLDP_functionality_Disabled << 1);
        bExecute_LLDP |= (ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tExecute_LLDP.Bit2_Reserve_Power_Mode << 2);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Layer2_PD,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.bCh_Num,
                bSet_Layer2_PD_Type, W_H(wPD_Requested_Power_dW),
                W_L(wPD_Requested_Power_dW), W_H(wPSE_Allocated_Power_dW),
                W_L(wPSE_Allocated_Power_dW),
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.bCable_Length_meters,
                bExecute_LLDP,
                B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.6.2 Get Port Layer2 LLDP PSE Data */
    case eGet_Port_Layer2_LLDP_PSE_Data:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Layer2_LLDP_PSE,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.6.3 Set Power Bank Power Source Type */
    case eSet_Power_Bank_Power_Source_Type:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Global, B_Supply, B_PowerBudgetSourceType,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Bank_Power_Source_Type_t.Set.bBank,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Bank_Power_Source_Type_t.Set.eMscc_PSE_power_source,
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.6.4 Set Port PowerUp ALT-B */
    case eSet_Port_PowerUp_ALT_B:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_PowerUp_ALT_B,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.bCh_Num,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.bit_0_PuP_CFG_EN,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.PuP_CMD,
                B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.6.5 Get Port PowerUp ALT-B */
    case eGet_Port_PowerUp_ALT_B:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_PowerUp_ALT_B,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_PowerUp_ALT_B_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }


    /* 4.6.6 Set Port Layer2 CDP PD Data */
    case eSet_Port_Layer2_CDP_PD_Data:
    {
        /* convert from mWatt to half Watt units */
        U8 bPD_Requested_Power0_hW = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power0_mW / 500);
        U8 bPD_Requested_Power1_hW = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power1_mW / 500);
        U8 bPD_Requested_Power2_hW = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power2_mW / 500);
        U8 bPD_Requested_Power3_hW = (U8)(ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power3_mW / 500);

        ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.byReserved = 0;

        U8 bSet_Layer2_PD_Type = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePortPriority;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.byReserved << 2;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePower_source << 4;
        bSet_Layer2_PD_Type |= ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePD_Power_type << 6;

        U8 bExecute_LLDP = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tExecute_LLDP.Bit0_Execute_LLDP;
        bExecute_LLDP |= (ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tExecute_LLDP.Bit1_LLDP_functionality_Disabled << 1);
        bExecute_LLDP |= (ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tExecute_LLDP.Bit2_Reserve_Power_Mode << 2);


        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Layer2_CDP_PD,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.bCh_Num,
                bSet_Layer2_PD_Type,
                bPD_Requested_Power0_hW,
                bPD_Requested_Power1_hW,
                bPD_Requested_Power2_hW,
                bPD_Requested_Power3_hW,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.bCable_Length_meters,
                bExecute_LLDP,
                B_N, B_CKSM_H, B_CKSM_L);
        break;
    }



    /* 4.6.7 Get Port Layer2 CDP PSE Data */
    case eGet_Port_Layer2_CDP_PSE_Data:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Layer2_CDP_PSE,
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Set.bCh_Num,
                B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_N, B_CKSM_H, B_CKSM_L);
        break;
    }



    /* 4.6.8 Set Port Layer2 CDP PD Data 2*/
    case eSet_Port_Layer2_CDP_PD_Data_2:
    {
        U8 bType_and_Exe_CDP = (U8)ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP.ePortPriority;                        /* :2 bit */
        bType_and_Exe_CDP |= ((U8)ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP.bExecute_CDP) << 2;                      /* :1 bit */
        bType_and_Exe_CDP |= ((U8)ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP.bDisable_CDP) << 3;                      /* :1 bit */
        bType_and_Exe_CDP |= ((U8)ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP.ePower_source) << 4;                     /* :2 bit */
        bType_and_Exe_CDP |= ((U8)ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP.ePD_Power_type) << 6;                    /* :2 bit */


        /* convert from Watt to half Watt units */
        U32 ulPD_Requested_Power0_dW = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power0_dW;
        U32 ulPD_Requested_Power1_dW = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power1_dW;
        U32 ulPD_Requested_Power2_dW = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power2_dW;
        U32 ulPD_Requested_Power3_dW = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power3_dW;
        U32 ulPD_Requested_Power4_dW = ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power4_dW;

        U8 Pw0 = (U8)((ulPD_Requested_Power0_dW >> 8) & 3);
        U8 Pw1 = (U8)((ulPD_Requested_Power1_dW >> 8) & 3);
        U8 Pw2 = (U8)((ulPD_Requested_Power2_dW >> 8) & 3);
        U8 Pw3 = (U8)((ulPD_Requested_Power3_dW >> 8) & 3);

        U8 Pw0_1_2_3 = (U8)(Pw0 + (Pw1 << 2) + (Pw2 << 4) + (Pw3 << 6));

        U8 Pw4 = (U8)((ulPD_Requested_Power4_dW >> 8) & 3);

        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Command,
                Echo_Id, B_Channel, B_Layer2_CDP_PD_2,
                ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.bCh_Num,
                bType_and_Exe_CDP,
                (U8)ulPD_Requested_Power0_dW,
                (U8)ulPD_Requested_Power1_dW,
                (U8)ulPD_Requested_Power2_dW,
                (U8)ulPD_Requested_Power3_dW,
                (U8)ulPD_Requested_Power4_dW,
                Pw4,
                Pw0_1_2_3,
                B_CKSM_H, B_CKSM_L);
        break;
    }

    /* 4.6.9	Get LLDP Math Conversion */
    case eGet_LLDP_Math_Conversion:
    {
        sprintf((char *)pDataArr, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", B_Request,
                Echo_Id, B_Channel, B_Layer2_LLDP_Math,
                ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.bCh_Num,
                W_H(ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.PowerToConvert_dW),
                W_L(ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.PowerToConvert_dW),
                ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.eCalcOpt,
                ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.ePortType,
                ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.eDeliver4P,
                ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.bCable_Length_meters,
                B_N, B_N, B_CKSM_H, B_CKSM_L);

        break;
    }


    default:
        return ePOE_STATUS_ERR_MSG_TYPE_NOT_EXIST;
    }

    /* calculate checksum , and place it inside message offset 13,14 */
    for (x = 0; x < 13; x++)
        chksum += pDataArr[x];

    pDataArr[13] = (chksum >> 8) & 0xFF;     /* update checksum MBS */
    pDataArr[14] = chksum & 0xFF;     /* update checksum LSB */

    Echo_Id++;     /* remember what Echo_Id value was placed inside the message, and Inc Echo_Id */
    if (Echo_Id == 255)     /* in case last echo-id number was 254, next one should be zero */
        Echo_Id = 0;

    return ePOE_STATUS_OK;     /* return echo-id that was placed inside the message */
}

/*---------------------------------------------------------------------
 *    description:     This function return the port status
 *
 *    input :   bPortStatus         - data byte which represent the port status
 *    output:   none
 *    return:   Port_Status_e       - port status type
 *---------------------------------------------------------------------*/
Port_Status_e
GetActualPortsStatus(U8 bPortStatus)
{
    if (((bPortStatus >= 3) && (bPortStatus <= 5))
        || ((bPortStatus >= 10) && (bPortStatus <= 11))
        || ((bPortStatus >= 13) && (bPortStatus <= 16))
        || ((bPortStatus >= 19) && (bPortStatus <= 25))
        || ((bPortStatus >= 34) && (bPortStatus <= 35))
        || ((bPortStatus >= 39) && (bPortStatus <= 42))
        || (bPortStatus == 59) || (bPortStatus == 255)
        || ((bPortStatus >= 69) && (bPortStatus <= 254)))
        return ePORT_STATUS_UNKNOWN;
    else
        return (Port_Status_e)bPortStatus;
}

/********************************************************************
 *    description:    This function is responsible to update the appropriate fields/structures in the
 *                    message struct according to reply/telemetry messages which are received from host.
 *
 *    input :   eMsgType          - which type of message was send to PoE controller
 *              pRxMsg            - 15 bytes communication protocol response/telemetry
 *    output:   ptPoeMsg          - ptr to a message struct which conatain the appropriate fields/structures
 *                                  of the received command/telemetry.
 *
 *    return:   ePOE_STATUS_OK   - operation succeed
 *              < ePOE_STATUS_OK - operation failed
 **********************************************************************/
PoE_Status_e
poe_communication_protocol_POE_Decode_Msg(
    Protocol_15Bytes_Msg_Type_e eMsgType, _OUT PoE_Msg_t *ptPoeMsg,
    U8 *pRxMsg)
{
    int x, y;

    switch (eMsgType) {

    /* ---------------------------------------------
       / -------------- SET MESSAGES -----------------
       / ---------------------------------------------*/

    case eReset_Command:
    case eRestore_Factory_Defaults:
    case eSave_System_Settings:
    case eSet_System_Status:
    case eSet_User_Byte:
    case eSet_PoE_Device_Parameters:
    case eSet_PoE_Device_Register:
    case eSet_Power_Banks:
    case eSet_PM_Method:
    case eSet_Power_Indication_LED:
    case eSet_Temporary_Matrix:
    case eProgram_Global_Matrix:
    case eSet_Enable_Disable_Channels:
    case eSet_Power_Limit_for_Channels:
    case eSet_Temporary_Power_Limit_for_Channels:
    case eSet_Port_Priority:
    case eSet_Port_Parameters:
    case eSet_Force_Power:
    case eSet_Interrupt_Mask:
    case eSet_Individual_Mask:
    case eSet_System_OK_LED_Mask_Registers:
    case eSet_Port_Layer2_LLDP_PD_Data:
    case eSet_Power_Bank_Power_Source_Type:
    case eSet_Port_PowerUp_ALT_B:
    case eSet_Port_Layer2_CDP_PD_Data:
    case eSet_Power_Limits_for_4_Pair_Channels:
    case eSet_Temporary_Power_Limits_for_4_Pair_Channels:
    case eSet_4Pair_Port_Parameters:
    case eSet_Derating_Data:
    case eSet_Derating_User_Temperature:
    case eSet_Enable_Disable_4_Pair_for_Channels:
        return ePOE_STATUS_ERR_MSG_TYPE_NOT_EXIST;

    /* ---------------------------------------------
       / -------------- GET MESSAGES -----------------
       / ---------------------------------------------*/

    /*****************   4.1 System messages    *********************/


    /* 4.1.6 Get System Status */
    case eGet_System_Status:
    {
        /* Fields required in Telemetry at Boot Up Error */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.poe_controller_error = pRxMsg[2] & 1;
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.firmware_download_is_required = (pRxMsg[2] >> 1) & 1;
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.bGIE = pRxMsg[5];
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.bPrivate_Label = pRxMsg[6];
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.bUser_Byte = pRxMsg[7];


        if (ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.firmware_download_is_required == 1) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.bTelemetry_at_Boot_Up_Error = ePOE_TRUE;
            break;
        }


        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.poe_controller_memory_error = pRxMsg[3] & 1;
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.devices_error = (pRxMsg[3] >> 1) & 1;
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.bFactory_Default = pRxMsg[4] & 1;


        for (x = 0; x < 8; x++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.eDevice_Fail[x] = (Device_Fail_e)((pRxMsg[8] >> x) & 1);
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.eTemperature_Disconnect[x] = (Temperature_Disconnect_e)((pRxMsg[9] >> x) & 1);
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.eTemperature_Alarm[x] = (Temperature_Alarm_e)((pRxMsg[10] >> x) & 1);
        }

        U16 wRegisterInterrupt = ((U16)pRxMsg[11] << 8) | (U16)pRxMsg[12];

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortTurnedOn = (POE_BOOL_e)(wRegisterInterrupt & 1);                              /* 0 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortTurnedOff = (POE_BOOL_e)((wRegisterInterrupt >> 1) & 1);                             /* 1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bDetectionUnsuccessful = (POE_BOOL_e)((wRegisterInterrupt >> 2) & 1);                     /* 2 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortFault = (POE_BOOL_e)((wRegisterInterrupt >> 3) & 1);                                 /* 3 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortWasInUnderLoad = (POE_BOOL_e)((wRegisterInterrupt >> 4) & 1);                        /* 4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortWasInOverLoad = (POE_BOOL_e)((wRegisterInterrupt >> 5) & 1);                         /* 5 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPortWasInPM = (POE_BOOL_e)((wRegisterInterrupt >> 6) & 1);                               /* 6 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPort_Power_Up_denied = (POE_BOOL_e)((wRegisterInterrupt >> 7) & 1);                      /* 7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bDisconnectionTemperature = (POE_BOOL_e)((wRegisterInterrupt >> 8) & 1);                  /* 8 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bUserDefinedTemperature = (POE_BOOL_e)((wRegisterInterrupt >> 9) & 1);                    /* 9 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPoEDeviceFault = (POE_BOOL_e)((wRegisterInterrupt >> 10) & 1);                            /* 10 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bPoEDeviceSpareEvent = (POE_BOOL_e)((wRegisterInterrupt >> 11) & 1);                       /* 11 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bNoMoreConnect = (POE_BOOL_e)((wRegisterInterrupt >> 12) & 1);                             /* 12 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bVmainFault = (POE_BOOL_e)((wRegisterInterrupt >> 13) & 1);                                /* 13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bSystemSpareEvent1 = (POE_BOOL_e)((wRegisterInterrupt >> 14) & 1);                         /* 14 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status_t.Get.tInterrutMaskRegister.bSystemSpareEvent2 = (POE_BOOL_e)((wRegisterInterrupt >> 15) & 1);                         /* 15 */

        break;
    }


    /* 4.1.7 Get System Status2 */
    case eGet_System_Status2:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_1_LVD = (POE_BOOL_e)((pRxMsg[2] >> 1) & 1);                            /* bit 1 - Low Voltage Detect */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_3_Lockup = (POE_BOOL_e)((pRxMsg[2] >> 3) & 1);                         /* bit 3 - Unrecoverable exception */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_4_ILOP = (POE_BOOL_e)((pRxMsg[2] >> 4) & 1);                           /* bit 4 - Illegal Opcode */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_5_COP = (POE_BOOL_e)((pRxMsg[2] >> 5) & 1);                            /* bit 5 - Computer Operation Properly (COP) Watchdog */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_6_PIN = (POE_BOOL_e)((pRxMsg[2] >> 6) & 1);                            /* bit 6 - External Reset Pin */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.SRS.Bit_7_POR = (POE_BOOL_e)((pRxMsg[2] >> 7) & 1);                            /* bit 7 - Power On Reset */

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_0_Clock_Loss_IRQ_occurred = (POE_BOOL_e)(pRxMsg[3] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_1_UART_IRQ_occurred_at_I2C_mode = (POE_BOOL_e)((pRxMsg[3] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_2_Unauthorized_IRQ_Occurred = (POE_BOOL_e)((pRxMsg[3] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_3_I2C_Arbitration_loss_occurred = (POE_BOOL_e)((pRxMsg[3] >> 3) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_4_External_clock_recovery_failed = (POE_BOOL_e)((pRxMsg[3] >> 4) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_5_POE_Vmain_out_of_range_flag = (POE_BOOL_e)((pRxMsg[3] >> 5) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_6_CPU_voltage_warning_flag = (POE_BOOL_e)((pRxMsg[3] >> 6) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.GIE1.Bit_7_UART_error_IRQ_Occurred = (POE_BOOL_e)((pRxMsg[3] >> 7) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.Reset_Info.Bit_0_communication_reset_command = (POE_BOOL_e)(pRxMsg[4] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.Reset_Info.Bit_1_clock_recovery_failure_for_more_than_5_sec = (POE_BOOL_e)((pRxMsg[4] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.Reset_Info.Bit_2_PoE_Device_failure = (POE_BOOL_e)((pRxMsg[4] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.Reset_Info.Bit_3_I2C_module_was_restarted = (POE_BOOL_e)((pRxMsg[4] >> 3) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.Reset_Info.Bit_4_self_reset = (POE_BOOL_e)((pRxMsg[4] >> 4) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Status2_t.Get.bSave_Command_counter = pRxMsg[10];
        break;
    }


    /* 4.1.9 Get Interrupt Mask */
    case eGet_Interrupt_Mask:
    {
        U16 wRegister_Mask = ((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortTurnedOn = (POE_BOOL_e)(wRegister_Mask & 1);                                             /* 0 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortTurnedOff = (POE_BOOL_e)((wRegister_Mask >> 1) & 1);                                     /* 1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bDetectionUnsuccessful = (POE_BOOL_e)((wRegister_Mask >> 2) & 1);                     /* 2 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortFault = (POE_BOOL_e)((wRegister_Mask >> 3) & 1);                                                 /* 3 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortWasInUnderLoad = (POE_BOOL_e)((wRegister_Mask >> 4) & 1);                                /* 4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortWasInOverLoad = (POE_BOOL_e)((wRegister_Mask >> 5) & 1);                                 /* 5 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPortWasInPM = (POE_BOOL_e)((wRegister_Mask >> 6) & 1);                                       /* 6 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPort_Power_Up_denied = (POE_BOOL_e)((wRegister_Mask >> 7) & 1);                      /* 7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bDisconnectionTemperature = (POE_BOOL_e)((wRegister_Mask >> 8) & 1);                  /* 8 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bUserDefinedTemperature = (POE_BOOL_e)((wRegister_Mask >> 9) & 1);                    /* 9 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPoEDeviceFault = (POE_BOOL_e)((wRegister_Mask >> 10) & 1);                                    /* 10 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bPoEDeviceSpareEvent = (POE_BOOL_e)((wRegister_Mask >> 11) & 1);                       /* 11 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bNoMoreConnect = (POE_BOOL_e)((wRegister_Mask >> 12) & 1);                                     /* 12 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bVmainFault = (POE_BOOL_e)((wRegister_Mask >> 13) & 1);                                                /* 13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bSystemSpareEvent1 = (POE_BOOL_e)((wRegister_Mask >> 14) & 1);                                 /* 14 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Interrupt_Mask_t.Get.tInterrutMaskRegister.bSystemSpareEvent2 = (POE_BOOL_e)((wRegister_Mask >> 15) & 1);                                 /* 15 */
        break;
    }


    /* 4.1.11 Get Individual Mask */
    case eGet_Individual_Mask:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Individual_Mask_t.Get.ePOE_BOOL_EnDis = (POE_BOOL_e)(pRxMsg[2] & 1);
        break;
    }

    /* 4.1.13 Get System OK LED Mask Registers */
    case eGet_System_OK_LED_Mask_Registers:
    {
        U16 wBlink_Register = ((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3];
        U16 wLight_Register = ((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5];

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.wBlink_Register = wBlink_Register;
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.wLight_Register = wLight_Register;

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortTurnedOn = (POE_BOOL_e)(wBlink_Register & 1);                                            /* 0 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortTurnedOff = (POE_BOOL_e)((wBlink_Register >> 1) & 1);                                    /* 1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bDetectionUnsuccessful = (POE_BOOL_e)((wBlink_Register >> 2) & 1);                    /* 2 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortFault = (POE_BOOL_e)((wBlink_Register >> 3) & 1);                                                /* 3 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortWasInUnderLoad = (POE_BOOL_e)((wBlink_Register >> 4) & 1);                               /* 4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortWasInOverLoad = (POE_BOOL_e)((wBlink_Register >> 5) & 1);                                /* 5 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPortWasInPM = (POE_BOOL_e)((wBlink_Register >> 6) & 1);                                      /* 6 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPort_Power_Up_denied = (POE_BOOL_e)((wBlink_Register >> 7) & 1);                     /* 7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bDisconnectionTemperature = (POE_BOOL_e)((wBlink_Register >> 8) & 1);                  /* 8 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bUserDefinedTemperature = (POE_BOOL_e)((wBlink_Register >> 9) & 1);                   /* 9 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPoEDeviceFault = (POE_BOOL_e)((wBlink_Register >> 10) & 1);                                   /* 10 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bPoEDeviceSpareEvent = (POE_BOOL_e)((wBlink_Register >> 11) & 1);                      /* 11 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bNoMoreConnect = (POE_BOOL_e)((wBlink_Register >> 12) & 1);                                    /* 12 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bVmainFault = (POE_BOOL_e)((wBlink_Register >> 13) & 1);                                               /* 13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bSystemSpareEvent1 = (POE_BOOL_e)((wBlink_Register >> 14) & 1);                                /* 14 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tBlink_Register.bSystemSpareEvent2 = (POE_BOOL_e)((wBlink_Register >> 15) & 1);                                /* 15 */


        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortTurnedOn = (POE_BOOL_e)(wLight_Register & 1);                                            /* 0 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortTurnedOff = (POE_BOOL_e)((wLight_Register >> 1) & 1);                                    /* 1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bDetectionUnsuccessful = (POE_BOOL_e)((wLight_Register >> 2) & 1);                    /* 2 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortFault = (POE_BOOL_e)((wLight_Register >> 3) & 1);                                                /* 3 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortWasInUnderLoad = (POE_BOOL_e)((wLight_Register >> 4) & 1);                               /* 4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortWasInOverLoad = (POE_BOOL_e)((wLight_Register >> 5) & 1);                                /* 5 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPortWasInPM = (POE_BOOL_e)((wLight_Register >> 6) & 1);                                      /* 6 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPort_Power_Up_denied = (POE_BOOL_e)((wLight_Register >> 7) & 1);                     /* 7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bDisconnectionTemperature = (POE_BOOL_e)((wLight_Register >> 8) & 1);                  /* 8 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bUserDefinedTemperature = (POE_BOOL_e)((wLight_Register >> 9) & 1);                   /* 9 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPoEDeviceFault = (POE_BOOL_e)((wLight_Register >> 10) & 1);                                   /* 10 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bPoEDeviceSpareEvent = (POE_BOOL_e)((wLight_Register >> 11) & 1);                      /* 11 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bNoMoreConnect = (POE_BOOL_e)((wLight_Register >> 12) & 1);                                    /* 12 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bVmainFault = (POE_BOOL_e)((wLight_Register >> 13) & 1);                                               /* 13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bSystemSpareEvent1 = (POE_BOOL_e)((wLight_Register >> 14) & 1);                                /* 14 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_OK_LED_Mask_Registers_t.Get.tLight_Register.bSystemSpareEvent2 = (POE_BOOL_e)((wLight_Register >> 15) & 1);                                /* 15 */

        break;
    }


    /* 4.1.15 Get Power Indication LED */
    case eGet_Power_Indication_LED:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Indication_LED_t.Get.bIndicationOn_Percent = pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Indication_LED_t.Get.bIndicationOff_Percent = pRxMsg[3];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Indication_LED_t.Get.eLED_Indication_Status = (pRxMsg[4] == 0) ? ePOE_TRUE : ePOE_FALSE;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Indication_LED_t.Get.bIndicationOn_W = pRxMsg[5];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Indication_LED_t.Get.bIndicationOff_W = pRxMsg[6];
        break;
    }


    /* 4.1.17 Get Masks Status - obsolete */



    /* 4.1.18 Get Software Version */
    case eGet_Software_Version:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.bHW_Version = pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.bProd = pRxMsg[4];

        U16 wSW_VERSION = ((U16)pRxMsg[5] << 8) | (U16)pRxMsg[6];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bPatch = (U8)(wSW_VERSION % 10);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMinor = (U8)((wSW_VERSION / 10) % 10);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMajor = (U8)((wSW_VERSION / 100) % 100);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.bParamNumber = pRxMsg[7];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.bBuild_Num = pRxMsg[8];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Software_Version_t.Get.wInternalSWNumber = ((U16)pRxMsg[9] << 8) | (U16)pRxMsg[10];

        break;
    }


    /*****************   4.2 PoE Device messages    *********************/


    /* 4.2.2 Get PoE Device Status */
    case eGet_PoE_Device_Status:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.bCS_Num = pRxMsg[2];
        U16 wDeviceVersion = ((U16)pRxMsg[3] << 8) | (U16)pRxMsg[4];

        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.tDeviceVersion.bSoftwareVersion = wDeviceVersion & 0x7F;                        /* 0-6 :7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.tDeviceVersion.bRTL_Version = (wDeviceVersion >> 7) & 0x7;                      /* 7-9 :3  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.tDeviceVersion.bAnalogVersion = (wDeviceVersion >> 10) & 0x3;                   /* 10-11 :2  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.tDeviceVersion.bFamilyPrefix = (wDeviceVersion >> 12) & 0xF;                    /* 12-15 :4  */

        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.eAsicStatus = (Asic_Status_e)pRxMsg[5];

        if ((pRxMsg[6] == eIC_Exp_0) || (pRxMsg[6] == eIC_Exp_4) || (pRxMsg[6] == eIC_Exp_8) || (pRxMsg[6] == eIC_Exp_12))
            ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.eIC_Exp = (IC_Exp_e)pRxMsg[6];
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.eIC_Exp = eIC_Exp_Unknown;

        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.bIC_HW = pRxMsg[7];
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.bIC_ports = pRxMsg[8];

        /* pos: 0 to 205   neg: -1 to -30 */
        if (pRxMsg[9] > 205)
            ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.lTempr_mC = (pRxMsg[9] - 256) * 1000;                      /* convert C to mC */
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.lTempr_mC = pRxMsg[9] * 1000;                              /* convert C to mC */

        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Get.ulTSH_mC = (U32)(pRxMsg[10] * 1000);                       /* convert C to mC */

        break;
    }

    /* 4.2.4 Get PoE Device Register */
    case eGet_PoE_Device_Register:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Get.wRegister_Data = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]);
        break;
    }



    /*****************   4.3 PoE Port messages    *********************/

    /* 4.3.2 Get Physical Port Number from Temporary Matrix */
    case eGet_Physical_Port_Number_from_Temporary_Matrix:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Temporary_Matrix_t.Get.bPhysical_Number_A = pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Temporary_Matrix_t.Get.bPhysical_Number_B = pRxMsg[3];
        break;
    }


    /* 4.3.4 Get Physical Port Number from Active Matrix */
    case eGet_Physical_Port_Number_from_Active_Matrix:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Active_Matrix_t.Get.bPhysical_Number_A = pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Active_Matrix_t.Get.bPhysical_Number_B = pRxMsg[3];
        break;
    }


    /* 4.3.6 Get All Ports Enable/Disable */
    case eGet_All_Ports_Enable_Disable:
    {
        U8 bPortIndex = 0;

        for (x = 2; x < 9; x++) {
            if (x == 5)                     /* skip on byte 5 which do not contain data */
                x++;

            for (y = 0; y < 8; y++) {
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Enable_Disable_t.Get.tPorts_EnDis[bPortIndex].bPorts_EnDis = (pRxMsg[x] & (1 << y)) >> y;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Enable_Disable_t.Get.tPorts_EnDis[bPortIndex].bPortNumber = bPortIndex;
                bPortIndex++;
            }
        }

        break;
    }


    /* 4.3.11 Get Port Power Limit */
    case eGet_Port_Power_Limit:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Power_Limit_t.Get.ulPPL_mW = ((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Power_Limit_t.Get.ulTPPL_mW = ((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5];
        break;
    }


    /* 4.3.13 Get 4-Pair Port Parameters */
    case eGet_4Pair_Port_Parameters:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortCfg.Bit0_PortEnDis = (POE_BOOL_e)(pRxMsg[2] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortCfg.Bit1_CapDis = (POE_BOOL_e)((pRxMsg[2] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortCfg.Bit2_IcutDis = (POE_BOOL_e)((pRxMsg[2] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.ulPPL4Pair_mW = (((U16)pRxMsg[3] << 8) | (U16)pRxMsg[4]) * 5;                             /* convert 5mW step to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.ePortPriority = (Port_Priority_e)pRxMsg[5];
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.bPort_Type = pRxMsg[6];
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tSum_as_TPPL.tBit0_3_user_configuration.eBit0_PM_algorithm = (Sum_as_TPPL_PM_algorithm_e)(pRxMsg[7] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tSum_as_TPPL.tBit4_7_actual_configuration.eBit4_PM_algorithm = (Sum_as_TPPL_PM_algorithm_e)((pRxMsg[7] >> 4) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortPM2.eBit0_3_PortPM2_user_configuration = (PortPM2_e)(pRxMsg[8] & 0xF);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortPM2.eBit4_7_PortPM2_actual_configuration = (PortPM2_e)((pRxMsg[8] >> 4) & 0xF);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.ulTPPL4Pair_mW = (((U16)pRxMsg[9] << 8) | (U16)pRxMsg[10]) * 5;                   /* convert 5mW step to mW */

        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortPM3.eBit0_3_PortPM3_user_configuration = (PortPM3_e)(pRxMsg[11] & 0xF);
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Get.tPortPM3.eBit4_7_PortPM3_actual_configuration = (PortPM3_e)((pRxMsg[11] >> 4) & 0xF);

        break;
    }


    /* 4.3.16 Get 4-Pair Port Power Limit */
    case eGet_4Pair_Port_Power_Limit:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Power_Limit_t.Get.ulPPL_4Pair_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 5;                    /* convert 5mW to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Power_Limit_t.Get.ulTPPL_4Pair_mW = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 5;                   /* convert 5mW to mW */
        break;
    }


    /* 4.3.18 Get Port Priority */
    case eGet_Port_Priority:
    {
        if (pRxMsg[2] > ePortPriority_Low)
            return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_DATA_BYTES;
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Priority_t.Get.ePortPriority = (Port_Priority_e)pRxMsg[2];

        break;
    }


    /* 4.3.20 Get All Ports Enable/Disable 4-Pair Mode */
    case eGet_All_Ports_Enable_Disable_4pair:
    {
        U8 bPortIndex = 0;

        for (x = 2; x < 9; x++) {
            if (x == 5)                     /* skip on byte 5 which do not contain data */
                x++;

            for (y = 0; y < 8; y++) {
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Enable_Disable_4pair_t.Get.tPorts_EnDis4Pair[bPortIndex].bEnDisValue = (pRxMsg[x] >> y) & 1;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Enable_Disable_4pair_t.Get.tPorts_EnDis4Pair[bPortIndex].bPortNumber = bPortIndex;
                bPortIndex++;
            }
        }

        break;

    }


    /* 4.3.21 Get Single Port Status */
    case eGet_Single_Port_Status:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.EnDis.bit0_Port_Enabled = (POE_BOOL_e)(pRxMsg[2] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.EnDis.bit1_Legacy_detection_Dis = (POE_BOOL_e)((pRxMsg[2] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.EnDis.bit2_Icut_Dis = (POE_BOOL_e)((pRxMsg[2] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.bPort_Status = pRxMsg[3];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.bForce_PowerEn = pRxMsg[4];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.Underload_latch = (POE_BOOL_e)(pRxMsg[5] & 1);                              /*  0  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.Overload_latch = (POE_BOOL_e)((pRxMsg[5] >> 1) & 1);                        /*  1  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.Force_On_current = (POE_BOOL_e)((pRxMsg[5] >> 2) & 1);                      /*  2  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.Underload_counter = (pRxMsg[5] >> 3) & 3;                    /* 3-4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.short_circuit = (POE_BOOL_e)((pRxMsg[5] >> 5) & 1);                         /*  5  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.tPortLatch.detection_failure_counter = (pRxMsg[5] >> 6) & 3;                     /* 6-7 */

        if (pRxMsg[6] > eClass4)
            ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.eMscc_ClassNum = eClassUnknown;
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.eMscc_ClassNum = (Class_Number_e)pRxMsg[6];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.eOperationMode = (Operation_Mode_e)(pRxMsg[10] & 3);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Get.b4PairEn = pRxMsg[11];

        break;
    }


    /* 4.3.22 Get All Ports Status */
    case eGet_All_Ports_Status:
    {
        U8 byVar_a = 0;;                 /* start loop */
        U8 byVar_b = 0;                 /* end of the loop */

        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Set.ePortsStatusGroup) {
        case ePortsStatusGroup1_0_to_10:
        {
            byVar_a = 0;                             /* start of the loop */
            byVar_b = 10;                             /* end loop */
            goto Get_Channels_Status;
        }
        case ePortsStatusGroup2_11_to_21:
        {
            byVar_a = 11;                             /* start of the loop */
            byVar_b = 21;                             /* end loop */
            goto Get_Channels_Status;
        }
        case ePortsStatusGroup3_22_to_25:
        {
            U8 bBytesOffset;

            for (x = 0; x < 4; x++) {
                bBytesOffset = (x < 2) ? 0 : 3;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[x].ePORT_STATUS = GetActualPortsStatus(pRxMsg[2 + x + bBytesOffset]);
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[x].bPortNumber = 22 + x;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[x].bValid = ePOE_TRUE;
            }

            return ePOE_STATUS_OK;
        }
        case ePortsStatusGroup4_26_to_36:
        {
            byVar_a = 26;                             /* start of the loop */
            byVar_b = 36;                             /* end loop */
            goto Get_Channels_Status;
        }
        case ePortsStatusGroup5_37_to_47:
        {
            byVar_a = 37;                             /* start of the loop */
            byVar_b = 47;                             /* end loop */
            goto Get_Channels_Status;
        }
        }

Get_Channels_Status:

        for (x = byVar_a, y = 0; x <= byVar_b; x++, y++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[y].ePORT_STATUS = GetActualPortsStatus(pRxMsg[2 + y]);
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[y].bPortNumber = x;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Get.tPorts_Status[y].bValid = ePOE_TRUE;
        }

        break;
    }


    /* 4.3.23 Get Extended Port Status */
    case eGet_Extended_Port_Status:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Port_standard_configuration_AF_AT_PoH = pRxMsg[2] & 3;                            /* Bits 0:1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Port_force_power_enable = (POE_BOOL_e)((pRxMsg[2] >> 2) & 1);                     /* Bit  2   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Port_4pair_enable = (POE_BOOL_e)((pRxMsg[2] >> 3) & 1);                           /* Bit  3   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Port_Enabled = (POE_BOOL_e)((pRxMsg[2] >> 4) & 1);                                /* Bit  4   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Legacy_detection_Disabled = (POE_BOOL_e)((pRxMsg[2] >> 5) & 1);                   /* Bit  5   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Defined_Port_Configuration_t.Icut_disabled = (POE_BOOL_e)((pRxMsg[2] >> 6) & 1);                               /* Bit  6   */

        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Actual_Port_Configuration_t.Port_standard_behavior_AF_AT_PoH = pRxMsg[3] & 3;                                  /* Bits 0:1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Actual_Port_Configuration_t.Port_force_power_behavior = (POE_BOOL_e)((pRxMsg[3] >> 2) & 1);                    /* Bit  2   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Actual_Port_Configuration_t.Port_4pair_behavior = (POE_BOOL_e)((pRxMsg[3] >> 3) & 1);                          /* Bit  3   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Actual_Port_Configuration_t.Layer2_enabled_on_this_port = (POE_BOOL_e)((pRxMsg[3] >> 4) & 1);                  /* Bit  4   */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.Actual_Port_Configuration_t.Power_reserved_mode_active = (POE_BOOL_e)((pRxMsg[3] >> 5) & 1);                   /* Bit  5   */

        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.ePortStatus = GetActualPortsStatus(pRxMsg[4]);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.eMscc_ClassNum = (Class_Number_e)pRxMsg[5];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.UDL_cnt = pRxMsg[6];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.OVL_cnt = pRxMsg[7];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.SC_cnt = pRxMsg[8];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.InvalidSigCnt = pRxMsg[9];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Get.PowerDeniedCnt = pRxMsg[10];

        break;
    }


    /* 4.3.24 Get Internal Port Status (For Debug usage) */
    case eGet_Internal_Port_Status:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.EnDis.bit0_Port_Enabled = (POE_BOOL_e)(pRxMsg[2] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.EnDis.bit1_Legacy_detection = (POE_BOOL_e)((pRxMsg[2] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.EnDis.bit2_Icut_Dis = (POE_BOOL_e)((pRxMsg[2] >> 2) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.bPortInternalStatus = pRxMsg[3];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.bit0_UDL_Latch = (POE_BOOL_e)(pRxMsg[5] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.bit1_OVL_Latch = (POE_BOOL_e)((pRxMsg[5] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.bit2_FORCE_ON_Latch = (POE_BOOL_e)((pRxMsg[5] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.BIT3_4_UDL_Counter = (pRxMsg[5] >> 3) & 3;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.bit5_SHORT_Latch = (POE_BOOL_e)((pRxMsg[5] >> 5) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Latch.BIT6_7_DET_FAIL_Counter = (pRxMsg[5] >> 6) & 3;

        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.eMscc_ClassNum = (Class_Number_e)pRxMsg[6];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Get.Port_standard_behavior_AF_AT_PoH = pRxMsg[7];

        break;
    }



    /* 4.3.25 Get UDL Counters */
    case eGet_UDL_Counters:
    {
        U8 bFirstPortIndex = 0;
        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Set.eCounterGroup) {
        case eUDL_CounterGroup1_0_to_23:
        {
            bFirstPortIndex = 0;
            break;
        }
        case eUDL_CounterGroup2_24_to_47:
        {
            bFirstPortIndex = 24;
            break;
        }
        }

        for (x = 0, y = 0; x < 24; x += 4, y++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x].bValue = pRxMsg[2 + y] & 0x03;                          /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x].bPortNumber = bFirstPortIndex + x;

            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 1].bValue = (pRxMsg[2 + y] & 0x0C) >> 2;                              /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 1].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 1].bPortNumber = bFirstPortIndex + x + 1;

            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 2].bValue = (pRxMsg[2 + y] & 0x30) >> 4;                              /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 2].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 2].bPortNumber = bFirstPortIndex + x + 2;

            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 3].bValue = (pRxMsg[2 + y] & 0xC0) >> 6;                              /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 3].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Get.tPorts_UDL_Counter[x + 3].bPortNumber = bFirstPortIndex + x + 3;
        }

        break;
    }


    /* 4.3.26 Get Detection Failure Counters */
    case eGet_Detection_Failure_Counters:
    {
        U8 bFirstPortIndex = 0;

        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Set.eDetCntGroup) {
        case eDetCntGroup1_0_to_23:
        {
            bFirstPortIndex = 0;
            break;
        }
        case eDetCntGroup2_24_to_47:
        {
            bFirstPortIndex = 24;
            break;
        }
        }

        for (x = 0, y = 0; x < 24; x += 4, y++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x].bValue = pRxMsg[2 + y] & 0x03;                          /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x].bPortNumber = bFirstPortIndex + x;

            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 1].bValue = (pRxMsg[2 + y] & 0x0C) >> 2;                          /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 1].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 1].bPortNumber = bFirstPortIndex + x + 1;

            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 2].bValue = (pRxMsg[2 + y] & 0x30) >> 4;                              /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 2].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 2].bPortNumber = bFirstPortIndex + x + 2;

            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 3].bValue = (pRxMsg[2 + y] & 0xC0) >> 6;                              /* get 2 bit cntr */
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 3].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Get.tPortsDetFailCounter[x + 3].bPortNumber = bFirstPortIndex + x + 3;
        }

        break;
    }


    /* 4.3.27 Get Latches (Old Request to support backwards compatibility) */
    case eGet_Latches:
    {
        U8 bPortOffset = 0;;
        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Set.eLatchesGroup) {
        case eLatchesGroup1_0_to_23:
        {
            bPortOffset = 0;
            break;
        }
        case eLatchesGroup2_24_to_47:
        {
            bPortOffset = 24;
            break;
        }
        }

        for (x = 0; x < 6; x++)
            for (y = 0; y < 4; y++) {
                ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Get.tPorts_Latches[(x * 4) + y].bUDL = pRxMsg[2 + x] & (1 << (2 * y));
                ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Get.tPorts_Latches[(x * 4) + y].bOVL = pRxMsg[2 + x] & (2 << (2 * y));
                ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Get.tPorts_Latches[(x * 4) + y].bPortNumber = bPortOffset + (x * 4) + y;
                ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Get.tPorts_Latches[(x * 4) + y].bValid = ePOE_TRUE;
            }

        break;
    }



    /* 4.3.28 Get Global Port Counters */
    case eGet_global_port_counters:
    {
        U8 bFirstPortIndex = 0;
        /*
           Counters_group_0, : port0 -  10
           Counters_group_1, : port11 - 21
           Counters_group_2, : port22 - 32
           Counters_group_3, : Port33 - 43
           Counters_group_4  : Port44 - 47 */

        bFirstPortIndex = 11 * ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterGroup;
        U8 Number_Of_Elements = DEF_Get_global_port_counters_t_NUMBER_OF_ELEMENTS;
        if (ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterGroup == Counters_group_4)                 /* Counters_group_4 */
            Number_Of_Elements = 4;

        for (x = 0; x < Number_Of_Elements; x++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Get.tPort_Counter[x].bValue = pRxMsg[2 + x];
            ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Get.tPort_Counter[x].bValid = ePOE_TRUE;
            ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Get.tPort_Counter[x].bPortNumber = bFirstPortIndex + x;
        }

        break;
    }



    /* 4.3.29 Get All Ports Class */
    case eGet_All_Ports_Class:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.bGroup = pRxMsg[2];                 /* which class group(0-2) command was send */

        U8 bByteClassVal;

        for (x = 0; x < 16; x += 2) {
            bByteClassVal = pRxMsg[3 + (x >> 1)] & 0x0F;

            if (bByteClassVal > eClass4)
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x].eMscc_ClassNum = eClassUnknown;
            else
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x].eMscc_ClassNum = (Class_Number_e)bByteClassVal;

            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x].bPortNumber = (pRxMsg[2] * 16) + x;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x].bValid = ePOE_TRUE;

            bByteClassVal = (pRxMsg[3 + (x >> 1)] & 0xF0) >> 4;
            if (bByteClassVal > eClass4)
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x + 1].eMscc_ClassNum = eClassUnknown;
            else
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x + 1].eMscc_ClassNum = (Class_Number_e)bByteClassVal;

            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x + 1].bPortNumber = (pRxMsg[2] * 16) + x + 1;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Get.tPorts_Class[x + 1].bValid = ePOE_TRUE;
        }
        break;
    }



    /* 4.3.30 Get All Ports delivering power state */
    case eGet_All_Ports_delivering_power_state:
    {
        U8 bIndex;
        for (bIndex = 0; bIndex < 6; bIndex++) {
            for (x = 0; x < 8; x++) {
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_delivering_power_state_t.Get.bPortsEnDis[(bIndex * 8) + x] = (pRxMsg[2 + bIndex] >> x) & 1;
            }
        }
        break;
    }



    /* 4.3.31 Get Port Event Cause */
    case eGet_Port_Event_Cause:
    {
        U8 bByteIndex;
        for (bByteIndex = 0; bByteIndex < 6; bByteIndex++) {
            for (x = 0; x < 8; x++) {
                ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.bPortsEvent[(bByteIndex * 8) + x] = (pRxMsg[2 + bByteIndex] >> x) & 1;
            }
        }

        U16 wRegisterInterrupt = ((U16)pRxMsg[8] << 8) | (U16)pRxMsg[9];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortTurnedOn = (POE_BOOL_e)(wRegisterInterrupt & 1);                                     /* 0 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortTurnedOff = (POE_BOOL_e)((wRegisterInterrupt >> 1) & 1);                             /* 1 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bDetectionUnsuccessful = (POE_BOOL_e)((wRegisterInterrupt >> 2) & 1);                     /* 2 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortFault = (POE_BOOL_e)((wRegisterInterrupt >> 3) & 1);                                 /* 3 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortWasInUnderLoad = (POE_BOOL_e)((wRegisterInterrupt >> 4) & 1);                        /* 4 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortWasInOverLoad = (POE_BOOL_e)((wRegisterInterrupt >> 5) & 1);                         /* 5 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPortWasInPM = (POE_BOOL_e)((wRegisterInterrupt >> 6) & 1);                               /* 6 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPort_Power_Up_denied = (POE_BOOL_e)((wRegisterInterrupt >> 7) & 1);                      /* 7 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bDisconnectionTemperature = (POE_BOOL_e)((wRegisterInterrupt >> 8) & 1);                  /* 8 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bUserDefinedTemperature = (POE_BOOL_e)((wRegisterInterrupt >> 9) & 1);                    /* 9 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPoEDeviceFault = (POE_BOOL_e)((wRegisterInterrupt >> 10) & 1);                            /* 10 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bPoEDeviceSpareEvent = (POE_BOOL_e)((wRegisterInterrupt >> 11) & 1);                       /* 11 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bNoMoreConnect = (POE_BOOL_e)((wRegisterInterrupt >> 12) & 1);                             /* 12 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bVmainFault = (POE_BOOL_e)((wRegisterInterrupt >> 13) & 1);                                /* 13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bSystemSpareEvent1 = (POE_BOOL_e)((wRegisterInterrupt >> 14) & 1);                         /* 14 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tInterrutRegister.bSystemSpareEvent2 = (POE_BOOL_e)((wRegisterInterrupt >> 15) & 1);                         /* 15 */

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tSystemOkReg.bit0_Vmain_is_in_range = (POE_BOOL_e)(pRxMsg[10] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tSystemOkReg.bit1_Over_power_indication_in_percent = (POE_BOOL_e)((pRxMsg[10] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Event_Cause_t.Get.tSystemOkReg.bit2_Over_power_indication_in_watts = (POE_BOOL_e)((pRxMsg[10] >> 2) & 1);

        break;
    }



    /*********************    4.4 Power Management Related Messages    ***************************/


    /* 4.4.2 Get PM Method */
    case eGet_PM_Method:
    {
        /* check that input value is in the enum's range*/
        if (((pRxMsg[2] > ePM1_Classes_0_to_3_Dynamic_and_class_4_TPPL) && (pRxMsg[2] != ePM1_User_defined_per_port_Sum_as_TPPL))
            || ((pRxMsg[3] > ePM2_ICUT_Max) && (pRxMsg[3] != ePM2_User_defined_per_port_PM2_per_Port))
            || (pRxMsg[4] > ePM3_Condition_on_Classes_0_to_4))
            return ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_DATA_BYTES;

        ptPoeMsg->PoE_Msg_Prm_e.Get_PM_Method_t.Get.ePM1_system_power_calculated_method = (PM1_System_Power_Calculated_Method_e)pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_PM_Method_t.Get.ePM2_Port_Power_Limit = (PM2_Port_Power_Limit_e)pRxMsg[3];
        ptPoeMsg->PoE_Msg_Prm_e.Get_PM_Method_t.Get.ePM3_Start_up_conditions = (PM3_Start_Up_Condition_e)pRxMsg[4];
        break;
    }


    /* 4.4.3 Get Total Power */
    case eGet_Total_Power:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Total_Power_t.Get.ulPower_Consumption_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Total_Power_t.Get.ulCalculated_Power_mW = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Total_Power_t.Get.ulAvailable_Power_mW = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Total_Power_t.Get.ulPower_Limit_mW = (((U16)pRxMsg[8] << 8) | (U16)pRxMsg[9]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Total_Power_t.Get.bPower_Bank = pRxMsg[10];
        break;
    }


    /* 4.4.5 Get Power Banks */
    case eGet_Power_Banks:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Get.ulPower_Budget_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Get.ulMax_Shutdown_Voltage_mV = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Get.ulMin_Shutdown_Voltage_mV = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Get.bGuardBand = pRxMsg[8];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Get.ePower_source_type = (Power_Source_Type_e)(pRxMsg[9] & 3);

        break;
    }


    /* 4.4.6 Get Power Supply Parameters */
    case eGet_Power_Supply_Parameters:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Parameters_t.Get.ulPower_Consumption_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 1000;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Parameters_t.Get.ulMax_Shutdown_Voltage_mV = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Parameters_t.Get.ulMin_Shutdown_Voltage_mV = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Parameters_t.Get.bPower_Bank = pRxMsg[9];                               /* 1 = internal 220/110VAC , 2 = external DC */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Parameters_t.Get.ulPower_Limit_mW = (((U16)pRxMsg[10] << 8) | (U16)pRxMsg[11]) * 1000;                            /* MAX power OF THE ACTIVE SOURCE (may switch) */
        break;
    }


    /* 4.4.7 Get Power Supply Voltage */
    case eGet_Power_Supply_Voltage:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Voltage_t.Get.ulVmain_Voltage_mV = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Supply_Voltage_t.Get.ulImain_Current_mA = (((U16)pRxMsg[7] << 8) | (U16)pRxMsg[8]);

        break;
    }


    /* 4.4.8 Get All Ports Power */
    case eGet_All_Ports_Power:
    {
        U8 byVar_a = 0;                 /* start loop */
        U8 byVar_b = 0;                 /* end of the loop */

        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Set.ePortsPowerGroup) {
        case ePortsPowerGroup1_0_to_10:
        {
            byVar_a = 0;                             /* start of the loop */
            byVar_b = 10;                             /* end loop */
            goto Get_Channels_Power;
        }
        case ePortsPowerGroup2_11_to_21:
        {
            byVar_a = 11;                             /* start of the loop */
            byVar_b = 21;                             /* end loop */
            goto Get_Channels_Power;
        }
        case ePortsPowerGroup3_22_to_25:
        {
            int iByteIndex = 0;
            for (x = 0; x < 4; x++) {
                iByteIndex = x;
                if (x >= 2)
                    iByteIndex = x + 6;

                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[x].ulPortPower_mW = pRxMsg[2 + iByteIndex];
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[x].bPortNumber = 22 + x;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[x].bValid = ePOE_TRUE;
            }

            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tAdditional_Info.ulVmain_Voltage_mV = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;                                     /* convert deciV to miliV */
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tAdditional_Info.ulPower_Consumption_mW = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 1000;                                 /* convert W to miliW */
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tAdditional_Info.ulMax_Power_Available_mW = (((U16)pRxMsg[8] << 8) | (U16)pRxMsg[9]) * 1000;                               /* convert W to miliW */
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tAdditional_Info.bValid = ePOE_TRUE;

            return ePOE_STATUS_OK;
        }
        case ePortsPowerGroup4_26_to_36:
        {
            byVar_a = 26;                             /* start of the loop */
            byVar_b = 36;                             /* end loop */
            goto Get_Channels_Power;
        }
        case ePortsPowerGroup5_37_to_47:
        {
            byVar_a = 37;                             /* start of the loop */
            byVar_b = 47;                             /* end loop */
            goto Get_Channels_Power;
        }
        }

Get_Channels_Power:

        for (x = byVar_a, y = 0; x <= byVar_b; x++, y++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[y].ulPortPower_mW = (pRxMsg[2 + y]) * 100;                     /* convert deciW to miliW */
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[y].bPortNumber = x;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Get.tPorts_Power[y].bValid = ePOE_TRUE;
        }

        break;
    }


    /* 4.4.9 Get All 4-Pair Ports Power */
    case eGet_All_4Pair_Ports_Power:
    {
        U8 byVar_a = 0;                  /* start loop */
        U8 byVar_b = 0;                  /* end of the loop */

        switch (ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Set.ePorts4PairPowerGroup) {
        case ePorts4PairPowerGroup1_0_to_10:
        {
            byVar_a = 0;                             /* start of the loop */
            byVar_b = 10;                             /* end loop */
            goto Get_All_4Pair_Ports_Power;
        }
        case ePorts4PairPowerGroup2_11_to_21:
        {
            byVar_a = 11;                             /* start of the loop */
            byVar_b = 21;                             /* end loop */
            goto Get_All_4Pair_Ports_Power;
        }

        case ePorts4PairPowerGroup3_22_to_25:
        {
            int iByteIndex = 0;
            for (x = 0; x < 4; x++) {
                iByteIndex = x;
                if (x >= 2)
                    iByteIndex = x + 6;

                ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[x].ulPortPower_mW = (pRxMsg[2 + iByteIndex]) * 500;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[x].bPortNumber = 22 + x;
                ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[x].bValid = ePOE_TRUE;
            }

            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tAdditional_Info.ulVmain_Voltage_mV = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tAdditional_Info.ulPower_Consumption_mW = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 1000;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tAdditional_Info.ulMax_Power_Available_mW = (((U16)pRxMsg[8] << 8) | (U16)pRxMsg[9]) * 1000;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tAdditional_Info.bValid = ePOE_TRUE;
            return ePOE_STATUS_OK;
        }

        case ePorts4PairPowerGroup4_26_to_36:
        {
            byVar_a = 26;                             /* start of the loop */
            byVar_b = 36;                             /* end loop */
            goto Get_All_4Pair_Ports_Power;
        }

        case ePorts4PairPowerGroup5_37_to_47:
        {
            byVar_a = 37;                             /* start of the loop */
            byVar_b = 47;                             /* end loop */
            goto Get_All_4Pair_Ports_Power;
        }
        }

Get_All_4Pair_Ports_Power:

        for (x = byVar_a, y = 0; x <= byVar_b; x++, y++) {
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[y].ulPortPower_mW = (pRxMsg[2 + y]) * 500;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[y].bPortNumber = x;
            ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Get.tPorts_Power[y].bValid = ePOE_TRUE;
        }

        break;
    }


    /* 4.4.10 Get All HIP Ports Power - obsolete - Please use the "Get All 4-Pair Ports Power " instead */

    /* 4.4.11 Get Port Measurements */
    case eGet_Port_Measurements:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Measurements_t.Get.ulVmain_Voltage_mV = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Measurements_t.Get.ulCalculated_Current_mA = ((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Measurements_t.Get.ulPower_Consumption_mW = ((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7];
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Measurements_t.Get.ulPort_Voltage_mV = (((U16)pRxMsg[9] << 8) | (U16)pRxMsg[10]) * 100;
        break;
    }


    /* 4.4.12 Get 4-Pair Port Measurements */
    case eGet_4Pair_Port_measurements:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Get.ulVmain_Voltage_mV = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;                             /* convert deciW to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Get.ulCalculated_Current_mA = ((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5];
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Get.ulPower_Consumption_4Pair_mW = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 5;                   /* convert 5mW to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Get.ulPort_Voltage_mV = (((U16)pRxMsg[9] << 8) | (U16)pRxMsg[10]) * 100;                              /* convert deciW to mW */
        break;
    }




    /*********************    4.5 Power Derating, Related Messages    *************************/


    /* 4.5.2 Get Derating Data */
    case eGet_Derating_Data:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Get.ulMax_PS_Power_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 1000;                           /* convert W to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Get.ulTstart_mC = (pRxMsg[4]) * 1000;                                   /* convert C to mC */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Get.ulTshutdown_mC = (pRxMsg[5]) * 1000;                                /* convert C to mC */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Get.ulDerating_Delta_Power_mW = (((U16)pRxMsg[6] << 8) | (U16)pRxMsg[7]) * 1000;                       /* convert W to mW */
        break;
    }


    /* 4.5.4 Get Derating User Temperature */
    case eGet_Derating_User_Temperature:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_User_Temperature_t.Get.ulUser_Temperature_mC = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;                       /* convert deciC to mC */
        break;
    }


    /* 4.5.5 Get System Measurments */
    case eGet_System_Measurements:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.bPower_Budget_Index = pRxMsg[2];
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.ulPower_Budget_mW = (((U16)pRxMsg[3] << 8) | (U16)pRxMsg[4]) * 1000;                         /* LSB = 1 Watt */
        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.ulActual_Budget_mW = (((U16)pRxMsg[5] << 8) | (U16)pRxMsg[6]) * 1000;                        /* LSB = 1 Watt */

        if (pRxMsg[7] > eTemperature_zone_4)
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.eTemperature_zone = eTemperature_zone_unknown;
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.eTemperature_zone = (Temperature_Zone_e)(pRxMsg[7]);

        ptPoeMsg->PoE_Msg_Prm_e.Get_System_Measurements_t.Get.ulDerating_Temperature_mC = ((((U16)pRxMsg[8] << 8) | (U16)pRxMsg[9])) * 100;                   /* convert from dC to mC *//* convert deciC to mC */
        break;
    }



    /************************    4.6 Layer2 Related Messages    ****************************/


    /* 4.6.2 Get Port Layer2 LLDP PSE Data */
    case eGet_Port_Layer2_LLDP_PSE_Data:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.ulPSE_Allocated_Power_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.ulPD_Requested_Power_mW = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.tLayer2_PD_Type.ePortPriority = (Port_Priority_e)(pRxMsg[6] & 3);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_source_type = (Power_Source_Type_e)((pRxMsg[6] >> 4) & 3);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_type = (Power_Type_e)((pRxMsg[6] >> 6) & 3);

        if (pRxMsg[7] > ePowerClass_4)
            ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.ePower_Class = ePowerClass_Unknown;
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.ePower_Class = (Power_Class_e)pRxMsg[7];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.ePSE_Power_Pair = (PSE_Power_Pair_e)(pRxMsg[8] & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.MDI_Power_Status_t.PSE = (POE_BOOL_e)(pRxMsg[9] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.MDI_Power_Status_t.SUPPORT_PoE = (POE_BOOL_e)((pRxMsg[9] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.MDI_Power_Status_t.Port_EnDis = (POE_BOOL_e)((pRxMsg[9] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.MDI_Power_Status_t.PairsControlCapability = (POE_BOOL_e)((pRxMsg[9] >> 3) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.bCable_Length = pRxMsg[10];

        U16 wPortIndicationAndPowerConsumption = ((U16)pRxMsg[11] << 8) | ((U16)pRxMsg[12]);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.wPort_power_consumption = wPortIndicationAndPowerConsumption & 0x0FFF;                                                 /* Bit 11:0  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_LLDP_Port_State = (Layer2_LLDP_Port_State_e)((wPortIndicationAndPowerConsumption >> 12) & 3);                  /* Bit 12:13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_request_Pending = (Layer2_request_Pending_e)((wPortIndicationAndPowerConsumption >> 14) & 1);                  /* Bit 14    */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eReserved_power_mode_active = (POE_BOOL_e)((wPortIndicationAndPowerConsumption >> 15) & 1);                            /* Bit 15    */

        break;
    }


    /* 4.6.5 Get Port PowerUp ALT-B */
    case eGet_Port_PowerUp_ALT_B:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_PowerUp_ALT_B_t.Get.bit_0_PuP_CFG_EN = (POE_BOOL_e)(pRxMsg[2] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_PowerUp_ALT_B_t.Get.PuP_CMD = (POE_BOOL_e)(pRxMsg[3] & 1);

        break;
    }


    /* 4.6.7 Get Port Layer2 CDP PSE Data */
    case eGet_Port_Layer2_CDP_PSE_Data:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ulPSE_Allocated_Power_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ulPD_Requested_Power_mW = (((U16)pRxMsg[4] << 8) | (U16)pRxMsg[5]) * 100;

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePortPriority = (Port_Priority_e)(pRxMsg[6] & 3);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_source_type = (Power_Source_Type_e)((pRxMsg[6] >> 4) & 3);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_type = (Power_Type_e)((pRxMsg[6] >> 6) & 3);

        if (pRxMsg[7] > ePowerClass_4)
            ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ePower_Class = ePowerClass_Unknown;
        else
            ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ePower_Class = (Power_Class_e)pRxMsg[7];

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ePSE_Power_Pair = (PSE_Power_Pair_e)(pRxMsg[8] & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.PSE = (POE_BOOL_e)(pRxMsg[9] & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.SUPPORT_PoE = (POE_BOOL_e)((pRxMsg[9] >> 1) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.Port_EnDis = (POE_BOOL_e)((pRxMsg[9] >> 2) & 1);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.PairsControlCapability = (POE_BOOL_e)((pRxMsg[9] >> 3) & 1);

        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.bCable_Length = pRxMsg[10];

        U16 wPortIndicationAndPowerConsumption = ((U16)pRxMsg[11] << 8) | ((U16)pRxMsg[12]);
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.wPort_power_consumption = wPortIndicationAndPowerConsumption & 0x0FFF;                                                      /* Bit 11:0  */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_LLDP_Port_State = (Layer2_LLDP_Port_State_e)((wPortIndicationAndPowerConsumption >> 12) & 3);                       /* Bit 12:13 */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_request_Pending = (Layer2_request_Pending_e)((wPortIndicationAndPowerConsumption >> 14) & 1);                       /* Bit 14    */
        ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eReserved_power_mode_active = (POE_BOOL_e)((wPortIndicationAndPowerConsumption >> 15) & 1);                                 /* Bit 15    */

        break;
    }


    /* 4.6.9	Get LLDP Math Conversion */
    case eGet_LLDP_Math_Conversion:
    {
        ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Get.PowerConvResult_mW = (((U16)pRxMsg[2] << 8) | (U16)pRxMsg[3]) * 100;                   /* convert from dW to mW */
        ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Get.eCalcOpt = (CalcOpt_e)pRxMsg[4];
        ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Get.CalcErr = pRxMsg[5];

        break;
    }


    /* ---------------------------------------------
       / -------------- GET MESSAGES -----------------
       / ---------------------------------------------*/
    default:
        return ePOE_STATUS_ERR_MSG_TYPE_NOT_EXIST;
    }

    return ePOE_STATUS_OK;
}


/********************************************************************
 *    description:    This function is responsible to check the recieved reply type
 *
 *    input :   pRxMsg            - 15 bytes communication protocol response/telemetry
 *    output:   none
 *    return:   ePOE_STATUS_OK   - operation succeed
 *              < ePOE_STATUS_OK - operation failed
 **********************************************************************/
PoE_Status_e
poe_communication_protocol_POE_Validate_Report_RxMsg(U8 *pRxMsg)
{
    S32 ReplyOk;

    if ((pRxMsg[2] == 0) && (pRxMsg[3] == 0))
        return ePOE_STATUS_OK;         /* valid report message was received */

    if ((pRxMsg[2] == 0xFF) && (pRxMsg[3] == 0xFF) && (pRxMsg[4] == 0xFF) && (pRxMsg[5] == 0xFF)) {
        tCountersInternal.tErrorCounters.tUnitErrors.ulCommand_Received_Wrong_Checksum++;
        return ePOE_STATUS_ERR_REPLY_COMMAND_RECEIVED_WRONG_CHECKSUM;         /* message send to PoE Device contained CRC error */
    }

    ReplyOk = ((S32)pRxMsg[2] << 8) | ((S32)pRxMsg[3]);

    if ((ReplyOk >= 0x1) && (ReplyOk <= 0x7FFF)) {
        tCountersInternal.tErrorCounters.tUnitErrors.ulFailed_Execution_Conflict_in_Subject_Bytes++;
        return ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_CONFLICT_IN_SUBJECT_BYTES;         /* SUBJECT field unknown */
    }

    if ((ReplyOk >= 0x8000) && (ReplyOk <= 0x8FFF)) {
        tCountersInternal.tErrorCounters.tUnitErrors.ulFailed_Execution_Wrong_Data_Byte_Value++;
        return ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_WRONG_DATA_BYTE_VALUE;         /* leagal command, but with illegal value (example: set port power to 100W) */
    }

    if (ReplyOk == 0xFFFF) {
        tCountersInternal.tErrorCounters.tUnitErrors.ulFailed_Execution_Undefined_Key_Value++;
        return ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_UNDEFINED_KEY_VALUE;         /* leagal key command) */
    }

    tCountersInternal.tErrorCounters.tUnitErrors.ulUnknownError++;
    return ePOE_STATUS_ERR_REPLY_CODE_OUT_OF_RANGE;     /* unknown error type */
}
