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
*  Description: the interface for the I2C communication between the host and the POE devices
*
*
*************************************************************************/


/*=========================================================================
   / INCLUDES
   /========================================================================*/

#include <stdio.h>
#include <string.h>
#include "mscc_arch_functions.h"
#include "mscc_poe_device_communication.h"
#include "mscc_poe_util.h"
#include "mscc_poe_global_internal.h"


/*=========================================================================
   / FUNCTIONS
   /========================================================================*/

/*---------------------------------------------------------------------
 *    description: Write data byte array to the PoE device
 *
 *    input :   bI2C_Address            - device I2C address
 *              pbyTxdata               - pointer to transmit data byte array
 *              num_write_length		- number of bytes to write
 *    output:   none
 *    return:   ePOE_STATUS_OK          - operation succeed
 *              < ePOE_STATUS_OK        - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
eDEVICE_COMMUNICATION_Write(U8 bI2C_Address, const U8 *pbyTxdata, U16 num_write_length)
{
    return ptMscc_fptr_write(bI2C_Address, pbyTxdata, num_write_length);
}




/*---------------------------------------------------------------------
 *    description: Read data byte array from PoE device
 *
 *    input :   bI2C_Address                            - device I2C address
 *                      bytes_to_read_length                    - requested number of bytes to read
 *    output:   pRxdata                                 - pointer to recived data byte array
 *                      *pNumber_of_bytes_actually_read - pointer to contain number of bytes actually read
 *                      READ_MODE_e eReadMode           - applicable for Async UART - read immidiate or with read timeout.
 *    return:   ePOE_STATUS_OK                          - operation succeed
 *              < ePOE_STATUS_OK                        - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
eDEVICE_COMMUNICATION_Read(U8 bI2C_Address, _OUT U8 *pRxdata, U16 bytes_to_read_length, U16 buffer_size, _OUT U16 *pNumber_of_bytes_actually_read, READ_MODE_e eReadMode)
{
    /* clear buffer */
    memset(pRxdata, 0, buffer_size * sizeof(U8));
    *pNumber_of_bytes_actually_read = 0;

    /* device error must return 0 if succeed and !=0 if failed */
    PoE_Status_e ePoE_Status = ptMscc_fptr_read(bI2C_Address, pRxdata, bytes_to_read_length, pNumber_of_bytes_actually_read, eReadMode);
    if (ePoE_Status != ePOE_STATUS_OK)
        return ePoE_Status;

    if (*pNumber_of_bytes_actually_read != bytes_to_read_length)
        return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;

    return ePoE_Status;
}



/*---------------------------------------------------------------------
 *    description: read legal 15 bytes prtocol message from PoE device buffer. includes sunc mechnism and message checksum test.
 *                         in case of message system status telemetry with echo 255 - it checks for any firmware damage.
 *                         if no legal 15bytes command found - buffer with be empty at the end of function.
 *
 *    input :   bI2C_Address                 - device I2C address
 *              U8 byEcho                    - messgae Echo
 *    output:   byArrRxBuffer[]              - pointer to recieve data byte array
 *              *pePOE_BOOL_Is_system_status - parameter to inform calling function if special system status (with echo 255) message was detected.
 *    return:   ePOE_STATUS_OK               - operation succeed
 *              < ePOE_STATUS_OK             - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(U8 bIC_Address, U8 byTxEcho, _OUT U8 byArrRxBuff[], _OUT POE_BOOL_e *pePOE_BOOL_Is_system_status)
{
    U8 bRxMsg[RX_BUFF_SIZE];
    U16 usRplySize;
    PoE_Status_e eResult;

    memset(byArrRxBuff, 0, sizeof(RX_BUFF_SIZE));

    int iZeroByteCount = 0;
    while (iZeroByteCount < 15) {   /* 15 bytes with value 0 means that I2C driver has no data to send */
        /* Read the 1 first byte (SUB0) from PoE Device */
        eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 1, RX_BUFF_SIZE, &usRplySize, eRead_WithTimeout);
        if ((eResult != ePOE_STATUS_OK) || (usRplySize != 1)) {
            /* ETIMEDOUT - our time is up or just an error */
            /*MSCC_PRINTF_INFO("No system status message \n\r");*/ /* error */
            if (eResult != ePOE_STATUS_OK)
                return eResult;
            else
                return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
        }

        if (bRxMsg[0] == 0)
            iZeroByteCount++;
        else
            iZeroByteCount = 0;

        if (bRxMsg[0] == B_Telemetry || bRxMsg[0] == B_Report) {       /* first msg byte - telemetry or response */
            byArrRxBuff[0] = bRxMsg[0];              /* store telemetry/response */

            /* Read the second byte (ECHO) from PoE Device */
            eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 1, RX_BUFF_SIZE, &usRplySize, ePOE_FALSE);
            if ((eResult != ePOE_STATUS_OK) || (usRplySize != 1)) {
                /* ETIMEDOUT - our time is up or just an error */
                MSCC_PRINTF_INFO("No responde for DOWNLOAD command. Fail \n\r");                 /* error */
                if (eResult != ePOE_STATUS_OK)
                    return eResult;
                else
                    return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
            }

            if (bRxMsg[0] == 0)
                iZeroByteCount++;
            else
                iZeroByteCount = 0;

            /* second msg byte - echo */
            if ((bRxMsg[0] == byTxEcho) ||                                                                                                      /* original messsage */
                ((byArrRxBuff[0] == B_Telemetry) && (bRxMsg[0] == B_SYSTEM_STATUS_ECHO))) {                 /* system status on startup */
                byArrRxBuff[1] = bRxMsg[0];                     /* store echo */

                /* Read the last 13 bytes from PoE Device */
                eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, MSG_SIZE - 2, RX_BUFF_SIZE, &usRplySize, eRead_immediate);
                if ((eResult != ePOE_STATUS_OK) || (usRplySize != (MSG_SIZE - 2))) {
                    /* ETIMEDOUT - our time is up or just an error */
                    MSCC_PRINTF_INFO("No responde for DOWNLOAD command. Fail \n\r");                     /* error */
                    if (eResult != ePOE_STATUS_OK)
                        return eResult;
                    else
                        return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
                }

                memcpy(byArrRxBuff + 2, bRxMsg, MSG_SIZE - 2);

                /* recived data Check Sum test - if checksum failed return as error */
                eResult = eMscc_POE_UTIL_CheckCommCheckSumValidity(byArrRxBuff);
                if (eResult != ePOE_STATUS_OK) {
                    return eResult;
                }

                if (byArrRxBuff[1] == byTxEcho) {
                    return ePOE_STATUS_OK;
                } else {             /* its system status telemetry with echo 255 - lets check for any firmware damage... */
                    *pePOE_BOOL_Is_system_status = ePOE_TRUE;

                    if ((byArrRxBuff[2] & 1) != 0) {
                        MSCC_PRINTF_INFO("Error- Get system status telemetry : PoE controller error !!\n\r");                         /* error */
                        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_controller_error;
                    } else if ((byArrRxBuff[2] & 2) != 0) {
                        MSCC_PRINTF_INFO("Error- Get system status telemetry : firmware download is required !!\n\r");                         /* error */
                        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_firmware_download_is_required;
                    } else {
                        return ePOE_STATUS_OK;
                    }
                }

            }
        }
    }

    return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
}







/*---------------------------------------------------------------------
 *    description: This function check if communication with PoE Device is OK (it send GET_SYSTEM_STATUS telemetry and expect proper response)
 *
 *    input :   byI2C_Address           - device I2C address
 *    output:   *pePOE_BOOL_Is_system_status - indicate if system_status telemetry detected or not
 *    return:   ePOE_STATUS_OK          - operation succeed
 *              < ePOE_STATUS_OK        - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
Send_General_15bytesCommProtocol_Command_or_Telemetry(U8 byIC_Address, U8 byTxArr[], U8 byArrRxBuff[], POE_BOOL_e *pePOE_BOOL_Is_system_status)
{
    U8 byEcho = byTxArr[1];
    mscc_POE_UTIL_SetCheckSum(byTxArr);

    PoE_Status_e eResult = eDEVICE_COMMUNICATION_Write(byIC_Address, byTxArr, MSG_SIZE);
    if (eResult != ePOE_STATUS_OK) {
        /* ETIMEDOUT - our time is up or just an error */
        MSCC_PRINTF_INFO("No responde for Get system status telemetry. Fail - exit update !!\n\r");         /* error */
        return eResult;
    }

    OS_Sleep_mS(200);

    return eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(byIC_Address, byEcho, byArrRxBuff, pePOE_BOOL_Is_system_status);
}




/*---------------------------------------------------------------------
 *    description: read special patterns with special sync mechanism
 *
 *    input :   bI2C_Address                     - device I2C address
 *              bytes_to_read_length             - required number of bytes to read
 *    output:   byArrRxBuff[]                    - pointer to recieve data byte array
 *		        *pNumber_of_bytes_actually_read  - pointer to contain number of bytes actually read
 *    return:   ePOE_STATUS_OK                   - operation succeed
 *              < ePOE_STATUS_OK                 - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
eDEVICE_COMMUNICATION_ReadPattern(U8 bIC_Address, _OUT U8 byArrRxBuff[], U16 bytes_to_read_length, _OUT U16 *pNumber_of_bytes_actually_read)
{
    U8 byArrLocalRxBuff[RX_BUFF_SIZE];
    PoE_Status_e eResult;
    int iCount = 0;
    U16 usRplySize;
    POE_BOOL_e bStartRx = ePOE_FALSE;

    memset(byArrLocalRxBuff, 0, sizeof(RX_BUFF_SIZE));
    memset(byArrRxBuff, 0, sizeof(RX_BUFF_SIZE));

    *pNumber_of_bytes_actually_read = 0;

    /**********    Empty RX buffer
       - for RS232 UART buffer - wait for timeout
       - for I2C buffer - wait for 15 sequenced bytes 0 bytes */

    do {
        /* Read 1 byte each time from PoE Device - to clear buffer from data */
        eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT byArrLocalRxBuff, 1, RX_BUFF_SIZE, &usRplySize, eRead_immediate);

        if ((eResult == ePOE_STATUS_OK) && (usRplySize == 1) && (byArrLocalRxBuff[0] != 0)) {
            byArrRxBuff[0] = byArrLocalRxBuff[0];
            bStartRx = ePOE_TRUE;
            *pNumber_of_bytes_actually_read = 1;
        } else if (((eResult == ePOE_STATUS_OK) && (usRplySize == 1) && (byArrLocalRxBuff[0] == 0)) ||        /* timeout - data not ready - i2c mode */
                   (eResult == ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT)) {                                     /* timeout - data not ready - uart mode */
            MSCC_PRINTF_INFO("T");
            OS_Sleep_mS(5);
            iCount++;
        } else {
            if (eResult == ePOE_STATUS_OK)
                return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
            else
                return eResult;
        }
    }
    while (((eResult == ePOE_STATUS_OK) || (eResult == ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT)) && (iCount < 10) && (bStartRx == ePOE_FALSE));

    if (iCount >= 10)
        return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;

    eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT byArrLocalRxBuff, bytes_to_read_length - 1, RX_BUFF_SIZE, &usRplySize, eRead_immediate);
    if ((eResult == ePOE_STATUS_OK) && (usRplySize == (bytes_to_read_length - 1))) {
        memcpy(byArrRxBuff + 1, byArrLocalRxBuff, (bytes_to_read_length - 1) * sizeof(U8));
        *pNumber_of_bytes_actually_read += bytes_to_read_length - 1;
        return eResult;
    }

    if (eResult == ePOE_STATUS_OK)
        return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;
    else
        return eResult;
}





/*---------------------------------------------------------------------
 *    description: read all existing data from RX buffer
 *
 *    input :   bI2C_Address                     - device I2C address
 *    output:   byArrRxBuff[]                    - pointer to recieve data byte array
 *                              pNumber_of_bytes_actually_read   - pointer to contain number of bytes actually read
 *    return:   ePOE_STATUS_OK                   - operation succeed
 *              < ePOE_STATUS_OK                 - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
ReadAllRxBuffer(U8 bIC_Address, U8 byArrRxBuff[], U16 *pNumber_of_bytes_actually_read)
{
    /**********    Empty RX buffer
       - for RS232 UART buffer - wait for timeout
       - for I2C buffer - wait for 15 sequenced bytes 0 bytes */
    int iSeqZeroByteCount = 0;
    U16 usRplySize;
    U8 bRxClearMsgBuff[RX_BUFF_SIZE];
    PoE_Status_e eResult = ePOE_STATUS_OK;
    int i;

    memset(byArrRxBuff, 0, sizeof(RX_BUFF_SIZE));
    memset(bRxClearMsgBuff, 0, sizeof(RX_BUFF_SIZE));

    (*pNumber_of_bytes_actually_read) = 0;

    for ( i = 0; (i < RX_BUFF_SIZE) && (eResult == ePOE_STATUS_OK) && (iSeqZeroByteCount < 15); i++) {
        /* Read 1 byte each time from PoE Device - to clear buffer from data */
        eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxClearMsgBuff, 1, RX_BUFF_SIZE, &usRplySize, eRead_immediate);
        if ((eResult != ePOE_STATUS_OK) && (eResult != ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT)) {
            return eResult;
        }

        if ((eResult == ePOE_STATUS_OK) && (usRplySize == 1)) {
            byArrRxBuff[*pNumber_of_bytes_actually_read] = bRxClearMsgBuff[0];
            (*pNumber_of_bytes_actually_read)++;
        }

        if ((eResult == ePOE_STATUS_OK) && (bRxClearMsgBuff[0] == 0) && (usRplySize == 1)) {
            iSeqZeroByteCount++;
        } else {
            iSeqZeroByteCount = 0;
        }
    }

    return ePOE_STATUS_OK;
}




/*---------------------------------------------------------------------
 *    description: clear all existing data from RX buffer
 *
 *    input :   bI2C_Address                      - device I2C address *
 *    output:   none
 *    return:   ePOE_STATUS_OK                    - operation succeed
 *              < ePOE_STATUS_OK                  - operation failed
 *---------------------------------------------------------------------*/
void
ClearRxBuffer(U8 bIC_Address)
{
    /**********    Empty RX buffer
       - for RS232 UART buffer - wait for timeout
       - for I2C buffer - wait for 15 sequenced bytes 0 bytes */
    int iZeroByteCount = 0;
    U16 usRplySize;
    U8 bRxClearMsgBuff[RX_BUFF_SIZE];
    PoE_Status_e eClearResult;

    do {
        /* Read 1 byte each time from PoE Device - to clear buffer from data */
        eClearResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxClearMsgBuff, 1, RX_BUFF_SIZE, &usRplySize, eRead_immediate);
        if ((eClearResult == ePOE_STATUS_OK) && (bRxClearMsgBuff[0] == 0) && (usRplySize == 1))
            iZeroByteCount++;
        else
            iZeroByteCount = 0;

    }
    while ((eClearResult == ePOE_STATUS_OK) && (iZeroByteCount < 1000));
}
