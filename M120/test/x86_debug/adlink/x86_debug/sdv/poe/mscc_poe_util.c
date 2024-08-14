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
*  Microsemi Corp. reserves the right to modify this software without
*  notice.
*
*************************************************************************
*
*  File Revision: 1.0
*
*************************************************************************
*
*  Description: The utilty functions used by the host and the poe api software
*
*
*************************************************************************/

/*=========================================================================
   / INCLUDES
   /========================================================================*/
#include <stdio.h>
#include "mscc_poe_global_internal.h"
#include "mscc_poe_util.h"



/*=========================================================================
   / Local VARIABLES
   /========================================================================*/

static U8 byLocalEcho = 10;



/*=========================================================================
   / Local FUNCTIONS PROTOTYPES
   /========================================================================*/


/*=========================================================================
   / FUNCTIONS
   /========================================================================*/


U8
GetEcho()
{
    return (U8)((++byLocalEcho) % 250);
}

/*---------------------------------------------------------------------
 *    description: Check If the array has same content in all of its fields
 *
 *    input :  bDataArr    - data bytes array
 *             bData       - data byte which should be compared to all data array bytes
 *             msg_len     - length of the buffer
 *    output:  none
 *    return:  POE_TRUE  - has the same content
 *                 POE_FALSE - content is not equal
 *---------------------------------------------------------------------*/
POE_BOOL_e
CheckIfArrayHasTheSameContent(U8 bDataArr[], U8 bData, U16 msg_len)
{
    int i;
    for (i = 0; i < msg_len; i++) {
        if (bDataArr[i] != bData)
            return ePOE_FALSE;
    }

    return ePOE_TRUE;
}



/*---------------------------------------------------------------------
 *    description: calculate checksum and insert result into the array
 *
 *    input :  dataArr    - data bytes array
 *    output:  none
 *    return:  update the check sum in message array
 *---------------------------------------------------------------------*/
void
mscc_POE_UTIL_SetCheckSum(U8 byArrData[])
{
    int sum = 0;
    int i;

    for (i = 0; i < (MSG_SIZE - 2); i++) {
        sum = sum + byArrData[i];
    }

    byArrData[13] = (U8)(sum / 256);
    byArrData[14] = (U8)(sum % 256);
}



/*---------------------------------------------------------------------
 *    description: calculate the checkSum from data and compare it to the CheckSum fields.
 *
 *    input :   byArrData - data bytes array
 *    output:   none
 *    return:   POE_TRUE  - if Check sum OK
 *              POE_FALSE - if check sum error
 *---------------------------------------------------------------------*/
POE_BOOL_e
mscc_POE_UTIL_CheckSumTest(U8 byArrData[])
{
    U16 wCheckSum_Calculated = 0;
    U16 wCheckSum = 0;
    U8 i = 0;

    for (i = 0; i < (MSG_SIZE - 2); i++)
        wCheckSum_Calculated += byArrData[i];

    wCheckSum = (U16)((byArrData[MSG_SIZE - 2] << 8) + byArrData[MSG_SIZE - 1]);

    if (wCheckSum_Calculated == wCheckSum)
        return ePOE_TRUE;

    return ePOE_FALSE;
}


/*---------------------------------------------------------------------
 *    description: calculate the checkSum from data and compare it to the CheckSum fields.
 *
 *    input :   byArrData - data bytes array
 *    output:   none
 *    return:   ePOE_STATUS_OK                      - if Check sum OK
 *              ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR - if check sum error
 *---------------------------------------------------------------------*/
PoE_Status_e
eMscc_POE_UTIL_CheckCommCheckSumValidity(U8 byArrData[])
{
    U16 wCheckSum_Calculated = 0;
    U16 wCheckSum = 0;
    U8 i = 0;

    for (i = 0; i < (MSG_SIZE - 2); i++)
        wCheckSum_Calculated += byArrData[i];

    wCheckSum = (U16)((byArrData[MSG_SIZE - 2] << 8) + byArrData[MSG_SIZE - 1]);

    if (wCheckSum_Calculated != wCheckSum)
        return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;

    return ePOE_STATUS_OK;
}



/* MUST BE MATCH to enum PoE_Status_e */
char *ErrListArr[] =
{
    "ePOE_STATUS_OK",                                                            /* =  0,   */
    "ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED",                                 /* =  -1,  */
    "ePOE_STATUS_ERR_COMMUNICATION_DEVICE_ERROR",                                            /* =  -2,  */
    "ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT",                                     /* =  -3,  */
    "ePOE_STATUS_ERR_REPLY_COMMAND_RECEIVED_WRONG_CHECKSUM",                     /* =  -4,  */
    "ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_CONFLICT_IN_SUBJECT_BYTES",          /* =  -5,  */
    "ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_WRONG_DATA_BYTE_VALUE",              /* =  -6,  */
    "ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_UNDEFINED_KEY_VALUE",                /* =  -7,  */
    "ePOE_STATUS_ERR_REPLY_CODE_OUT_OF_RANGE",                                   /* =  -8,  */
    "ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_DATA_BYTES",                             /* =  -9,  */
    "ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES",                              /* =  -10, */
    "ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR",                                       /* =  -11, */
    "ePOE_STATUS_ERR_RX_MSG_ECHO_MISMATCH",                                      /* =  -12, */
    "ePOE_STATUS_ERR_MSG_TYPE_NOT_EXIST",                                        /* =  -13, */
    "ePOE_STATUS_ERR_MUTEX_INIT_ERROR",                                          /* =  -14, */
    "ePOE_STATUS_ERR_MUTEX_DESTROY_ERROR",                                       /* =  -15, */
    "ePOE_STATUS_ERR_MUTEX_LOCK_ERROR",                                          /* =  -16, */
    "ePOE_STATUS_ERR_MUTEX_UNLOCK_ERROR",                                        /* =  -17, */
    "ePOE_STATUS_ERR_SLEEP_FUNCTION_ERROR",                                      /* =  -18, */
    "ePOE_STATUS_ERR_MUTEX_ALREADY_INITIALIZED",                                 /* =  -19, */
    "ePOE_STATUS_ERR_MUTEX_ALREADY_DISPOSED",                                    /* =  -20, */
    "ePOE_STATUS_ERR_OPEN_I2C_DEVICE_ERROR",                                     /* =  -21, */
    "ePOE_STATUS_ERR_CLOSE_I2C_DEVICE_ERROR",                                    /* =  -22, */
    "ePOE_STATUS_ERR_INITIALIZATION_ERROR",                                      /* =  -23, */
    "ePOE_STATUS_ERR_UART_DRIVER_ERROR",                                         /* =  -24, */
    "ePOE_STATUS_ERR_DEVICE_SELECTION_ERROR",                                    /* =  -25, */
    "ePOE_STATUS_ERR_OPEN_QUEUE",                                                /* =  -26, */
    "ePOE_STATUS_ERR_QUEUE_TX_MSG_FAILED",                                       /* =  -27, */
    "ePOE_STATUS_ERR_QUEUE_MSG_SIZE_ERROR",                                      /* =  -28, */
    "ePOE_STATUS_ERR_QUEUE_RX_MSG_ERROR",                                        /* =  -29, */
    "ePOE_STATUS_ERR_QUEUE_NOT_INITIALIZED",                                     /* =  -30, */
    "ePOE_STATUS_ERR_OPEN_UART_DEVICE_ERROR",                                                                /* =  -31, */
    "ePOE_STATUS_ERR_POE_MCU_FIRMWARE_ERROR",                                    /* =  -32, */
    "ePOE_STATUS_ERR_POE_MCU_FIRMWARE_DOWNLOAD_IS_REQUIRED",                     /* =  -33, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TPE",                                  /* =  -34, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TE_TPE",                               /* =  -35, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_TOP",                                     /* =  -36, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_controller_error",                    /* =  -37, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_firmware_download_is_required",       /* =  -38, */
    "ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_FAILED_TO_OPEN_FILE",                     /* =  -39, */
    "ePOE_STATUS_MAX"                                                            /* =  -40, */
};



/*---------------------------------------------------------------------
 *    description: print description according to the input POE_STATUS_e number
 *
 *    input :  msg           - message to print
 *             ePOE_STATUS   - poe status
 *             ulDeviceError - the error number as returned from the Host I2C read and write device functions
 *    output:  none
 *    return:  none
 *---------------------------------------------------------------------*/
char *
MSCC_POE_UTIL_GetErrDescription(PoE_Status_e ePOE_STATUS)
{
    if ((ePOE_STATUS <= ePOE_STATUS_MAX) || (ePOE_STATUS > 0))
        return NULL;

    int num = (int)ePOE_STATUS;
    num *= -1;
    return ErrListArr[num];
}




/*---------------------------------------------------------------------
 *    description: print Hardware Reset error according to the input mscc_HardwareResetDetected_e number
 *
 *    input :  eHardwareResetDetected   - enum for Hardware error type
 *    output:  none
 *    return:  none
 *---------------------------------------------------------------------*/
void
MSCC_POE_UTIL_PrintPoEDeviceHardwareError(Hardware_Reset_Detected_e eHardwareResetDetected)
{
    char *Msg;        /* message to print */

    switch (eHardwareResetDetected) {
    case eHardwareResetDetected_ERR_PoE_controller_error:
    {
        Msg = "PoE_controller_error";
        break;
    }
    case eHardwareResetDetected_ERR_firmware_download_is_required:
    {
        Msg = "firmware_download_is_required";
        break;
    }

    case eHardwareResetDetected_ERR_PoE_controller_memory_error:
    {
        Msg = "PoE_controller_memory_error";
        break;
    }
    case eHardwareResetDetected_ERR_PoE_firmware_error:
    {
        Msg = "PoE_firmware_error";
        break;
    }
    default:
    {
        return;
    }
    }

    MSCC_PRINTF_INFO("Error-PoE device hardware error detected: %s\n\r ", Msg);
}



/*---------------------------------------------------------------------
 *    description: print counters
 *
 *    input :  ptCounters - Counters_t struct which contain all counters
 *    output:  none
 *    return:  none
 *---------------------------------------------------------------------*/
void
PrintCounters(Counters_t *ptCounters)
{
    printf("\n\rAPI Counters:\n\r");
    printf("Succeed Msgs             : %lu\n\r", (*ptCounters).ulSucceedMsgsCntr);

    printf("\n\rAPI error Counters:\n\r");
    printf("Checksum                 : %lu\n\r", (*ptCounters).tErrorCounters.ulChecksum);
    printf("Communication Timeout    : %lu\n\r", (*ptCounters).tErrorCounters.ulCommunicationTimeout);
    printf("Echo Mismatch            : %lu\n\r", (*ptCounters).tErrorCounters.ulEchoMismatch);
    printf("Rx Retry                 : %lu\n\r", (*ptCounters).tErrorCounters.ulRxRetry);

    printf("\n\rError Counters reported from Unit:\n\r");
    printf("Wrong Checksum           : %lu\n\r", (*ptCounters).tErrorCounters.tUnitErrors.ulCommand_Received_Wrong_Checksum);
    printf("Conflict in Subject Bytes: %lu\n\r", (*ptCounters).tErrorCounters.tUnitErrors.ulFailed_Execution_Conflict_in_Subject_Bytes);
    printf("Undefined Key Value      : %lu\n\r", (*ptCounters).tErrorCounters.tUnitErrors.ulFailed_Execution_Undefined_Key_Value);
    printf("Wrong Data Byte Value    : %lu\n\r", (*ptCounters).tErrorCounters.tUnitErrors.ulFailed_Execution_Wrong_Data_Byte_Value);
    printf("Unknown Error            : %lu\n\r", (*ptCounters).tErrorCounters.tUnitErrors.ulUnknownError);
}



void
mscc_printf(const char *format, ...)
{
    if (ePOE_BOOL_Printf_Enable == ePOE_TRUE) {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        fflush(stdout);
        va_end(args);
    }
}


