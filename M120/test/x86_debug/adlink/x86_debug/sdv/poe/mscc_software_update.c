

/*========================================================================
   INCLUDES
   ======================================================================== */



/*#include <fstream> */
/*#include <sstream>*/
/*#include <string>*/
/*#include <cerrno>*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stdio.h"
#include "string.h"
#include "mscc_arch_functions.h"
#include "mscc_poe_communication_protocol.h"
#include "mscc_poe_device_communication.h"
#include "mscc_poe_util.h"
#include "mscc_poe_global_internal.h"







/****************************************************************************
                         DEFINITIONS
****************************************************************************/


/****************************************************************************
                         GLOBAL VARS
****************************************************************************/

U8 byArr_GET_SYSTEM_STATUS[] = {0x02, 0x02, 0x07, 0x3D, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x03, 0x06};
U8 byArr_DOWNLOAD[] = {0x01, 0x01, 0xFF, 0x99, 0x15, 0x16, 0x16, 0x99, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x03, 0xFA};



/****************************************************************************
                       L O C A L      F U N C T I O N S
****************************************************************************/



S32
iRead_entire_file(char *sFile_name, char buffer[], S32 iFile_size)
{
    FILE *f = fopen(sFile_name, "r");
    if (!f)
        return -1;

    S32 iSize;
    /* read entire file and write its content to buffer lines array */
    while ((iSize = fread(buffer, iFile_size, sizeof(char), f)) > 0) {
        /* MSCC_PRINTF_INFO("%s \n\r",buffer); */
        fwrite(buffer, iSize, sizeof(char), stdout);
    }

    fclose(f);
    return 0;
}




U16
ClearLeadingZeroesFromData(U8 bRplyBuf[], U16 usRplySize, int iMaxBufferSize)
{
    S16 sStr_index = 0;
    S32 iBuf_index = 0;

    if (usRplySize > iMaxBufferSize)
        usRplySize = (U16)(iMaxBufferSize - 1);

    for (iBuf_index = 0; iBuf_index < usRplySize; iBuf_index++) {
        if (bRplyBuf[iBuf_index] != 0) {
            bRplyBuf[sStr_index] = bRplyBuf[iBuf_index];
            sStr_index++;
        }
    }

    return sStr_index;
}




int
PrintBytesBuffer(const char *pcTitle, U8 byArrBuffer[], int iDataLen, int iMaxBufferSize)
{
    int i = 0;

    if (iDataLen > iMaxBufferSize)
        iDataLen = iMaxBufferSize - 1;

    MSCC_PRINTF_INFO(" Datalen : %d , ", iDataLen);

    if (iDataLen > 0) {   /* check if there is data to print */
        MSCC_PRINTF_INFO(" %s ", pcTitle);

        for (i = 0; i < iDataLen; i++)
            MSCC_PRINTF_INFO("%02x ", byArrBuffer[i]);               /* display reply rec from PoE MCU */

        MSCC_PRINTF_INFO("\n\r");
    }

    return 0;
}



int
BytesToString(U8 byArrBuffer[], _OUT char cStr[], int iDataLen, int iMaxBufferSize)
{
    int i = 0;
    cStr[0] = (char)0;

    if (iDataLen > iMaxBufferSize)
        iDataLen = iMaxBufferSize - 1;

    memset(cStr, 0, iMaxBufferSize * sizeof(char));

    while (i < iDataLen) {
        cStr[i] = (char)byArrBuffer[i];
        if (byArrBuffer[i] == 0)
            return i;

        i++;
    }

    return 0;
}






/*---------------------------------------------------------------------
 *    description: This function check if communication with PoE Device is OK (it send GET_SYSTEM_STATUS telemetry and expect proper response)
 *
 *    input :   byI2C_Address           - device I2C address
 *    output:   *pePOE_BOOL_Is_system_status - indicate if system_status telemetry detected or not
 *    return:   ePOE_STATUS_OK          - operation succeed
 *              < ePOE_STATUS_OK        - operation failed
 *---------------------------------------------------------------------*/
POE_BOOL_e
iSerComm_verifyComm_with_poe(U8 byIC_Address, POE_BOOL_e *pePOE_BOOL_Is_system_status)
{
    U8 byArrRxBuff[RX_BUFF_SIZE];
    PoE_Status_e eResult;

    U8 byLocalEcho = GetEcho();
    byArr_GET_SYSTEM_STATUS[1] = byLocalEcho;      /* update ECHO */

    eResult = Send_General_15bytesCommProtocol_Command_or_Telemetry(byIC_Address, byArr_GET_SYSTEM_STATUS, byArrRxBuff, pePOE_BOOL_Is_system_status);
    if (eResult != ePOE_STATUS_OK) {
        /* ETIMEDOUT - our time is up or just an error */
        MSCC_PRINTF_INFO("No responde for GetSystemStatus telemetry \n\r");         /* error */
        return ePOE_FALSE;
    }

    /* verify that subject is telemetry (checksum and echo already verified)  */
    if ((eResult == ePOE_STATUS_OK) && (byArrRxBuff[0] == B_Telemetry)) {
        return ePOE_TRUE;
    } else {
        /* ETIMEDOUT - our time is up or just an error */
        MSCC_PRINTF_INFO("GetSystemStatus telemetry answered with reply error. Fail !!\n\r");         /* error */
        return ePOE_FALSE;
    }
}





/*---------------------------------------------------------------------
 *    description: This function program new PoE firmware into the PoE Device
 *
 *    input :   bI2C_Address            - device I2C address
 *              S19FileName             - S19 file name (include absolute path)
 *    output:   none
 *    return:   ePOE_STATUS_OK          - operation succeed
 *              < ePOE_STATUS_OK        - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e
eCmnd_do_poe_firmware_update(U8 bIC_Address, char S19FileName[])
{
    U8 bDotCntr = 0;
    U16 usRplySize;
    PoE_Status_e eResult;

    U8 bRxMsg[RX_BUFF_SIZE];
    U8 bTxMsg[RX_BUFF_SIZE];
    char cStr[RX_BUFF_SIZE];

    ClearRxBuffer(bIC_Address);
    OS_Sleep_mS(1000);

    POE_BOOL_e ePOE_BOOL_Is_system_status = ePOE_FALSE;

    /* remove get system status messages if exist ... */
    int iCount = 0;
    POE_BOOL_e ePOE_BOOL;
    do {
        iCount++;
        ePOE_BOOL = iSerComm_verifyComm_with_poe(bIC_Address, &ePOE_BOOL_Is_system_status);
    }
    while ((ePOE_BOOL == ePOE_TRUE) && (ePOE_BOOL_Is_system_status == ePOE_TRUE) && (iCount < 3));


    if (iCount >= 3) {
        return ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED;
    }

    /* if comm with PoE MCU is OK, send DOWNLOAD command */
    if ( ePOE_BOOL == ePOE_TRUE ) {
        MSCC_PRINTF_INFO("POE has an operational firmware, Lets erase it \n\r");
        MSCC_PRINTF_INFO("Send DOWNLOAD command. Rec:");

        eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, byArr_DOWNLOAD, MSG_SIZE);

        OS_Sleep_mS(200);

        /* Read 15bytes response + 5 bytes TPE\n\r from PoE Device */
        /* in case of UART - it will send ack+TPE at once */
        /* in case of I2C - it will send ack and then the TPE */


        eResult = eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(bIC_Address, byArr_DOWNLOAD[1], bRxMsg, &ePOE_BOOL_Is_system_status);
        if (eResult != ePOE_STATUS_OK) {
            tCountersInternal.tErrorCounters.ulCommunicationTimeout++;             /* increment time out counter */

            /* ETIMEDOUT - our time is up or just an error */
            MSCC_PRINTF_INFO("No responde for DOWNLOAD command. Fail - exit update !!\n\r");             /* error */

            /* if checksum failed return as error */
            if (eResult == ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR)
                return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;

            return eResult;
        }

        /* in case of I2C - now we can obtain the TPE */
        OS_Sleep_mS(200);


        eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 5, RX_BUFF_SIZE, &usRplySize, eRead_immediate);
        if (eResult != ePOE_STATUS_OK) {
            /* ETIMEDOUT - our time is up or just an error */
            MSCC_PRINTF_INFO("No responde for DOWNLOAD command. Fail - exit update !!\n\r");             /* error */
            PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                /* display reply rec from PoE MCU */
            return eResult;
        }

        /* convert databytes to string */
        usRplySize = ClearLeadingZeroesFromData(bRxMsg, usRplySize, RX_BUFF_SIZE);
        BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);

        MSCC_PRINTF_INFO(" Download Report OK. Expect to rec TPE. Rec:");

        if ((!strstr(cStr, "TPE")) && (!strstr(cStr, "TOE"))) {
            MSCC_PRINTF_INFO(" [FAIL] - No TPE received - Exit update\n\r");             /* error */
            PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                /* display reply rec from PoE MCU */
            return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TPE;;
        }

        MSCC_PRINTF_INFO(" TPE [OK]\n\r");
    } else {  /* PoE MCU seem not to have valid firmware */
        MSCC_PRINTF_INFO("PoE Unit seem not to have valid firmware!!\n\r");

        MSCC_PRINTF_INFO("If fails to operate download - please Reset Unit is needed prior to next try...\n\r");

        OS_Sleep_mS(5000);
        MSCC_PRINTF_INFO("Send ENTR. Expect to rec TPE. Rec:");

        /* Empty all data from RX buffer */
        ClearRxBuffer(bIC_Address);

        OS_Sleep_mS(10);

        bTxMsg[0] = 'E';
        eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);   OS_Sleep_mS(50);
        bTxMsg[0] = 'N';
        eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);   OS_Sleep_mS(50);
        bTxMsg[0] = 'T';
        eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);   OS_Sleep_mS(50);
        bTxMsg[0] = 'R';
        eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);   OS_Sleep_mS(50);

        /* --- As a result of DOWNLOAD command or ENTR string, we expect to read TPE from PoE MCU ------- */
        OS_Sleep_mS(400);;               /* wait 400ms before trying to read "TPE\r\n\r" from PoE device */

        /* expect to read TPE\n\r */
        eResult = ReadAllRxBuffer(bIC_Address, _OUT bRxMsg, &usRplySize);
        if ((usRplySize == 0) || (eResult != ePOE_STATUS_OK)) {
            MSCC_PRINTF_INFO("\n\rDEVICE_COMMUNICATION_Read() failed to read read TPE\r\n\r");             /* error */
            PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                /* display reply rec from PoE MCU */
            return eResult;
        }

        usRplySize = ClearLeadingZeroesFromData(bRxMsg, usRplySize, RX_BUFF_SIZE);
        BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);
        PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);            /* display reply rec from PoE MCU */
        if ((!strstr(cStr, "TPE")) && (!strstr(cStr, "TOE"))) {
            MSCC_PRINTF_INFO(" [FAIL] - No TPE received - Exit update\n\r");             /* error */
            PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                /* display reply rec from PoE MCU */
            return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TPE;;
        }

        MSCC_PRINTF_INFO(" [OK]\n\r");
    }



    /* --- Send E , and expect to read TOE from PoE MCU ------- */
    MSCC_PRINTF_INFO("Send E, Expect to rec TOE. Rec:");


    bTxMsg[0] = 'E';
    eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);

    OS_Sleep_mS(100);     /* send 'E" and wait 5 sec */

    eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 10, RX_BUFF_SIZE, &usRplySize, eRead_immediate);       /* expect to read TOE\r\n or TE or TPE   */
    usRplySize = ClearLeadingZeroesFromData(bRxMsg, usRplySize, RX_BUFF_SIZE);
    BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);

    MSCC_PRINTF_INFO("%s", cStr);
    if (!strstr(cStr, "TOE")) {   /* rx recieved TOE  */
        MSCC_PRINTF_INFO(" [FAIL] - No TOE received - Exit update\n\r");         /* error */
        PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);            /* display reply rec from PoE MCU */
        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TE_TPE;
    }

    MSCC_PRINTF_INFO("wait 5Sec. Expect to rec TE TPE. Rec:");
    OS_Sleep_mS(5000);     /* send 'E" and wait 5 sec */


    eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 20, RX_BUFF_SIZE, &usRplySize, eRead_immediate);       /* expect to read TOE\r\n or TE or TPE   */
    usRplySize = ClearLeadingZeroesFromData(bRxMsg, usRplySize, RX_BUFF_SIZE);
    BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);

    MSCC_PRINTF_INFO("%s", cStr);
    if ((!strstr(cStr, "TE")) || (!strstr(cStr, "TPE"))) {    /* all 3 must be recieved TE TPE */
        MSCC_PRINTF_INFO(" [FAIL] - No TOE TE TPE received - Exit update\n\r");         /* error */
        PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);            /* display reply rec from PoE MCU */
        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TE_TPE;
    }

    MSCC_PRINTF_INFO(" [OK]\n\r");

    /* --- Send P , and expect to read TOP from PoE MCU ------- */
    bTxMsg[0] = 'P';
    eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 1);

    OS_Sleep_mS(500);     /* send 'P" and wait 100 mSec */

    MSCC_PRINTF_INFO("Send P. Expect to rec TOP. Rec:");

    eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, 30, RX_BUFF_SIZE, &usRplySize, eRead_immediate);         /* expect to read TOP\r\n */
    usRplySize = ClearLeadingZeroesFromData(bRxMsg, usRplySize, RX_BUFF_SIZE);
    BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);

    MSCC_PRINTF_INFO("%s", cStr);

    if ( !strstr(cStr, "TOP")) {
        MSCC_PRINTF_INFO(" [FAIL] - No TOP received - Exit update\n\r");          /* error */
        PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);            /* display reply rec from PoE MCU */
        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_TOP;
    }

    MSCC_PRINTF_INFO(" [OK]\n\r");
    MSCC_PRINTF_INFO("Start S19 transfer (a few minutes duration)\n\r");


#define LINE_SIZE 120
    char line [LINE_SIZE];

    S32 iNumber_of_bytes_to_write;

    FILE *file = fopen(S19FileName, "r");
    if (file == NULL)
        return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_FAILED_TO_OPEN_FILE;         /*print the error message on stderr.*/


    POE_BOOL_e ePOE_BOOL_write_S19_lines_succeed = ePOE_TRUE;     /* flag to sign if writing S19 lines operation succeed  */

    while (fgets(line, sizeof line, file) != NULL) {   /* read a line from a file */
        if (line[1] == 0x31          /* characters 5- 8 in the line specify a 16-bit address.  */
            || line[1] == 0x32             /* characters 5-10 in the line specify a 24-bit address.  */
            || line[1] == 0x33             /* characters 5-12 in the line specify a 32-bit address.  */
            || line[1] == 0x37             /* end of file. A checksum follows but if generally ignored. */
            || line[1] == 0x38             /* end of file. A checksum follows but if generally ignored. */
            || line[1] == 0x39             /* end of file. A checksum follows but if generally ignored. */
            ) {
            iNumber_of_bytes_to_write = strlen(line);

            eResult = eDEVICE_COMMUNICATION_Write(bIC_Address, (U8 *)line, iNumber_of_bytes_to_write);
            if (eResult != ePOE_STATUS_OK) {
                ePOE_BOOL_write_S19_lines_succeed = ePOE_FALSE;
                MSCC_PRINTF_INFO("\n\rFailed to write byte to PoE device ,status: %s - Exit update\n\r", MSCC_POE_UTIL_GetErrDescription(eResult));
                break;
            }

            memset(line, 0, sizeof(line));

            if ((eDevice_Method == eDevice_Method_UART_Sync) || (eDevice_Method == eDevice_Method_UART_Async))
                OS_Sleep_mS(SOFTWARE_UPDATE_S19_ACK_UART_DELAY_mS);                  /* uart sync minimum delay is 45 (as tested on lab)*/

            eResult = eDEVICE_COMMUNICATION_ReadPattern(bIC_Address, _OUT bRxMsg, 4, &usRplySize);
            if ((eResult != ePOE_STATUS_OK) || (usRplySize == 0)) {
                ePOE_BOOL_write_S19_lines_succeed = ePOE_FALSE;
                MSCC_PRINTF_INFO("\n\rS19 line , status: %s , Reply size error: %d - Exit update\n\r", MSCC_POE_UTIL_GetErrDescription(eResult), usRplySize);                     /* error */
                PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                    /* display reply rec from PoE MCU */
                break;
            }

            BytesToString(bRxMsg, _OUT cStr, usRplySize, RX_BUFF_SIZE);

            if ((usRplySize > 0) && (!strstr(cStr, "T*")) && (!strstr(cStr, "TP"))) {
                ePOE_BOOL_write_S19_lines_succeed = ePOE_FALSE;
                MSCC_PRINTF_INFO("\n\rReply for S19 line error, Exit update\n\r", cStr);
                PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);                    /* display reply rec from PoE MCU */
                break;
            }

            /* show progress by printing '.' for every S19 line. After 74 dots start new line */
            MSCC_PRINTF_INFO(".");
            if (bDotCntr++ > 74) {
                MSCC_PRINTF_INFO("\n\r");
                bDotCntr = 0;
            }
        }
    }

    MSCC_PRINTF_INFO("END\n\r");
    fclose(file);

    bTxMsg[0] = 'R';
    bTxMsg[1] = 'S';
    bTxMsg[2] = 'T';
    PoE_Status_e eResultWrite = eDEVICE_COMMUNICATION_Write(bIC_Address, bTxMsg, 3);        /* Command PoE MCU to RESET itself */

    /* check if writing S19 lines operation succeed  */
    if (ePOE_BOOL_write_S19_lines_succeed == ePOE_FALSE) {
        if (eResult == ePOE_STATUS_OK)
            return ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT;             /* in case we read single byte with value '0' from PoE device in I2C mode */
        else
            return eResult;
    }

    if (eResultWrite != ePOE_STATUS_OK)
        return eResultWrite;


    MSCC_PRINTF_INFO("waiting for Get system status telemetry... ");

    OS_Sleep_mS(5000);

    /* Read 15bytes from PoE Device */
    eResult = eDEVICE_COMMUNICATION_Read(bIC_Address, _OUT bRxMsg, MSG_SIZE, RX_BUFF_SIZE, &usRplySize, eRead_WithTimeout);
    if (eResult != ePOE_STATUS_OK) {
        /* ETIMEDOUT - our time is up or just an error */
        MSCC_PRINTF_INFO("Error: No System status telemetry received. Fail - exit update !!\n\r");         /* error */
        PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);            /* display reply rec from PoE MCU */
        return eResult;
    }

    /* verify that telemetry checksum ,subject and echo are correct   */
    ePOE_BOOL = mscc_POE_UTIL_CheckSumTest(bRxMsg);
    if ((ePOE_BOOL == ePOE_TRUE) && (bRxMsg[0] == B_Telemetry) && (bRxMsg[1] == B_SYSTEM_STATUS_ECHO)) {
        if ((bRxMsg[2] & 1) != 0) {
            MSCC_PRINTF_INFO("Error- Get system status telemetry : PoE controller error !!\n\r");             /* error */
            return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_controller_error;
        } else if ((bRxMsg[2] & 2) != 0) {
            MSCC_PRINTF_INFO("Error- Get system status telemetry : firmware download is required !!\n\r");             /* error */
            return ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_firmware_download_is_required;
        }

        MSCC_PRINTF_INFO("[OK] \n\rProgramming finished successfully \n\r");
        return ePOE_STATUS_OK;
    }

    /* ETIMEDOUT - our time is up or just an error */
    MSCC_PRINTF_INFO("Error: No System status telemetry received. Fail - exit update !!\n\r");     /* error */
    PrintBytesBuffer("Rec:", bRxMsg, usRplySize, RX_BUFF_SIZE);        /* display reply rec from PoE MCU */

    if (ePOE_BOOL == ePOE_FALSE)
        return ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR;
    else
        return ePOE_STATUS_ERR_POE_MCU_FIRMWARE_ERROR;
}
