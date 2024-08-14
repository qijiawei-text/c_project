/*************************************************************************
 *
 *            Copyright (c) 2016 by Microsemi Corp. Inc.
 *
 *  This software is copyrighted by, and is the sole property of Microsemi
 *  Corp. All rights, title, ownership, or other interests in the
 *  software remain the property of Microsemi Corp. This software
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
 *  Description: API software for communication between host and poe devices.
 *               the communication is based on microsemi 15bytes communication protocl.
 *
 *************************************************************************/


/*=========================================================================
/ INCLUDES
/========================================================================*/

#include <string.h>
#include "mscc_poe_communication_protocol.h"
#include "mscc_poe_util.h"
#include "mscc_poe_host_comm_api.h"
#include "mscc_poe_global_internal.h"
#include "mscc_software_update.h"


static U8 bPoEInitialized = ePOE_FALSE;

/*=========================================================================
/ FUNCTIONS
/========================================================================*/


/*---------------------------------------------------------------------
 *    description:     This command initialize the PoE software
 *
 *    input :   pMscc_InitInfo    - pointer to struct InitInfo_t which contain data required for
 *                                  PoE API software initialization.
 *
 *    return:   ePOE_STATUS_OK       - operation succeed
 *              < ePOE_STATUS_OK     - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e MSCC_POE_Init(Init_Info_t *ptMscc_InitInfo)
{
	PoE_Status_e eResult = ePOE_STATUS_OK;
	PoE_Status_e ulTmplResult = ePOE_STATUS_OK;

	if (bPoEInitialized)
		return ePOE_STATUS_ERR_MUTEX_ALREADY_INITIALIZED; /* already initalized */

	eResult = OS_mutex_init();
	if (eResult != ePOE_STATUS_OK)
		return eResult;

	/* try to take and lock the mutex */
	ulTmplResult = OS_mutex_lock(); /* down semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	/* START CRITICAL REGION */

	eResult = ePOE_COMMUNICATION_PROTOCOL_InitSoftware(ptMscc_InitInfo); /* INIT POE SW  */

	/* END CRITICAL REGION */

	ulTmplResult = OS_mutex_unlock(); /* up semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	bPoEInitialized = ePOE_TRUE;

	return eResult;
}












/*---------------------------------------------------------------------
 *    description:     This command initialize the PoE software
 *
 *    input :   pMscc_InitInfo    - pointer to struct InitInfo_t which contain data required for
 *                                  PoE API software initialization.
 *    output:   pDevice_error     - contain the error code of the host driver in case of errors
 *                                  in i2c read or i2c write operations.
 *
 *    return:   ePOE_STATUS_OK       - operation succeed
 *              < ePOE_STATUS_OK     - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e MSCC_POE_Clear_POE_Device_Buffer(U8 bIC_Address)
{
	PoE_Status_e ulResult = ePOE_STATUS_OK;
	PoE_Status_e ulTmplResult = ePOE_STATUS_OK;

	/* try to take and lock the mutex */
	ulTmplResult = OS_mutex_lock(); /* down semaphore */
	
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	/* START CRITICAL REGION */

	/* decode 15 bytes Tx cmd ,access PoE IC's and place wResult in reply buffer */
	ulResult = ePOE_COMMUNICATION_PROTOCOL_Clear_PoE_Device_Buffer(bIC_Address);

	/* END CRITICAL REGION */

	ulTmplResult = OS_mutex_unlock(); /* up semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	return ulResult;
}


/*---------------------------------------------------------------------
 *    description:     This command Close the operation of the PoE software.
 *                     the command is FFU.
 *
 *    input :   mscc_CloseInfo_t  - pointer to struct mscc_CloseInfo_t which contain data required for
 *                                  closing the PoE API software.
 *    output:   pDevice_error     - contain the error code of the host driver in case of errors
 *                                  in read or write operations.
 *
 *    return:   ePOE_STATUS_OK       - operation succeed
 *              < ePOE_STATUS_OK    - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e MSCC_POE_Exit()
{
	if (!bPoEInitialized)
		return ePOE_STATUS_ERR_MUTEX_ALREADY_DISPOSED; /* already disposed */

	bPoEInitialized = ePOE_FALSE;

	return ePOE_STATUS_OK;
}




/*---------------------------------------------------------------------
 *    description: Operate communication between the host and the PoE device
 *
 *    input :   eMessageType     - the specific 15bytes communication protocol command/telemetry
 *              bIC_Address      - the PoE device I2C address
 *
 *    inout :   ptMessage        - pointer to generic message struct - must be zeroed before useing this function
 *                                 this struct conatain the arguments to be send by the specific protocol command.
 *
 *    return:   ePOE_STATUS_OK       - operation succeed
 *              < ePOE_STATUS_OK     - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e MSCC_POE_Msg  (Protocol_15Bytes_Msg_Type_e eMessageType, _INOUT PoE_Msg_t *ptPoeMsg, U8 bIC_Address)
{
	PoE_Status_e ulResult = ePOE_STATUS_OK;
	PoE_Status_e ulTmplResult = ePOE_STATUS_OK;

	/* try to take and lock the mutex */
	ulTmplResult = OS_mutex_lock(); /* down semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	/* START CRITICAL REGION */

	/* decode 15 bytes Tx cmd ,access PoE IC's and place wResult in reply buffer */
	ulResult = ePOE_COMMUNICATION_PROTOCOL_AnalysisAndExecutionCommand(eMessageType, ptPoeMsg, bIC_Address);

	/* END CRITICAL REGION */

	ulTmplResult = OS_mutex_unlock(); /* up semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	return ulResult;
}


/*---------------------------------------------------------------------
 *    description:     Get all counters composed in struct type Counters_t
 *
 *    input :   None
 *    output:   ptCounters - pointer to struct of type Counters_t
 *
 *    return:   None
 *---------------------------------------------------------------------*/
void MSCC_POE_Get_Counters (_OUT Counters_t *ptCounters)
{
	if (ptCounters != NULL)
		*ptCounters = tCountersInternal;
}






/*---------------------------------------------------------------------
 *    description: Operate communication between the host and the PoE device
 *
 *    input :   eMessageType     - the specific 15bytes communication protocol command/telemetry
 *              bIC_Address      - the PoE device I2C address
 *
 *    inout :   ptMessage        - pointer to generic message struct - must be zeroed before useing this function
 *                                 this struct conatain the arguments to be send by the specific protocol command.
 *
 *    return:   ePOE_STATUS_OK       - operation succeed
 *              < ePOE_STATUS_OK     - operation failed
 *---------------------------------------------------------------------*/
PoE_Status_e MSCC_POE_Firmware_Update  (U8 bIC_Address, char S19FileName[])
{
	PoE_Status_e ulResult = ePOE_STATUS_OK;
	PoE_Status_e ulTmplResult = ePOE_STATUS_OK;

	/* try to take and lock the mutex */
	ulTmplResult = OS_mutex_lock(); /* down semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	/* START CRITICAL REGION */

	/* decode 15 bytes Tx cmd ,access PoE IC's and place wResult in reply buffer */
	ulResult = eCmnd_do_poe_firmware_update(bIC_Address, S19FileName);

	/* END CRITICAL REGION */

	ulTmplResult = OS_mutex_unlock(); /* up semaphore */
	if (ulTmplResult != ePOE_STATUS_OK)
		return ulTmplResult;

	return ulResult;
}


/*=========================================================================
/ FUNCTIONS
/========================================================================*/



/*---------------------------------------------------------------------
 *    description:    Operate transaction with PoE device and handle in case of situations of non succeed transactions.
 *
 *    input :   pcFunctionName  - Function Name string
 *              eMsgType        - enum which message we deal with.
 *    inout :   ptMessage       - ptr to struct that conatain the arguments to be send by the specific protocol command.
 *    output:   none
 *    return:   TransmitMsgStatus_e - enum that instruct the host what to do next
 *---------------------------------------------------------------------*/
TRANSMIT_MSG_STATUS_e TransmitMsg(char *pcFunctionName, Protocol_15Bytes_Msg_Type_e eMsgType, _INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	U8 bMsgSendIndex = 0; /* rechecking data 3 times before announcement of communication timeout */
	PoE_Status_e eMscc_poe_status; /* poe software status */

	do
	{
		/* operate message transaction */
		eMscc_poe_status = MSCC_POE_Msg(eMsgType, _INOUT ptPoeMsg, bI2C_Device_Address);
		if (eMscc_poe_status == ePOE_STATUS_OK)
			return eTransmitMsg_OK;

		if (ePOE_BOOL_Printf_Enable == ePOE_TRUE)
		{
			MSCC_PRINTF_INFO("%s ,status: %s \n\r", pcFunctionName, MSCC_POE_UTIL_GetErrDescription(eMscc_poe_status));
		}

		/*** checking fault options ***/

		if (eMscc_poe_status == ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT)
		{
			OS_Sleep_mS(2000);      /* wait 1 second after timeout */
		}
		else if (eMscc_poe_status == ePOE_STATUS_ERR_COMMUNICATION_DEVICE_ERROR)
		{
			OS_Sleep_mS(3000);      /* wait 3 seconds for Unit startup after hardware reset */
			return eTransmitMsg_RestartManager;
		}
		else if (eMscc_poe_status == ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED)
		{
			/* check the status of the hardware after hardware reset event */
			if ( ((*ptPoeMsg).eHardwareResetDetected != eHardwareResetDetected_PoE_controller_OK) &&
			        ((*ptPoeMsg).eHardwareResetDetected != eNO_HardwareResetDetected_was_detected))
			{
				/* print the hardware error description and mark to exit from the manager loop */
				MSCC_POE_UTIL_PrintPoEDeviceHardwareError((*ptPoeMsg).eHardwareResetDetected);
				return eTransmitMsg_ExitManager;
			}
			else
			{
				return eTransmitMsg_RestartManager;
			}
		}
		else if ((eMscc_poe_status <= ePOE_STATUS_ERR_RX_MSG_ECHO_MISMATCH) &&
		         (eMscc_poe_status >= ePOE_STATUS_ERR_REPLY_COMMAND_RECEIVED_WRONG_CHECKSUM))
		{
			bMsgSendIndex++;
		}
		else
			return eTransmitMsg_ExitManager;

	}
	while (bMsgSendIndex < 3);

	return eTransmitMsg_ExitManager; /* no response from PoE device */
}








/**************************************************************************************/
/***************************   4.1 System messages    *********************************/
/**************************************************************************************/


/* 4.1.1 Reset Command   ****************************************************************/
TRANSMIT_MSG_STATUS_e Reset_Command(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Reset_Command", eReset_Command, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.2 Restore Factory Default   ****************************************************************/
TRANSMIT_MSG_STATUS_e Restore_Factory_Defaults(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Restore_Factory_Defaults", eRestore_Factory_Defaults, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.3 Save System Settings   ****************************************************************/
TRANSMIT_MSG_STATUS_e Save_System_Settings(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Save_System_Settings", eSave_System_Settings, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.4 Set User Byte   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_User_Byte(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bUserByte)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_User_Byte_t.Set.bUserByte = bUserByte;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_User_Byte", eSet_User_Byte, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.5 Set System Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_System_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bPrivate_Label)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_System_Status_t.Set.bPrivate_Label = bPrivate_Label;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_System_Status", eSet_System_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.6 Get System Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_System_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_System_Status", eGet_System_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.7 Get System Status2   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_System_Status2(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_System_Status2", eGet_System_Status2, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.8 Set Interrupt Mask   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Interrupt_Mask(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Protocol_interrupt_register_t tInterrutMaskRegister)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* 0xFF: unmask all Interrupt Register events */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortTurnedOn             = tInterrutMaskRegister.bPortTurnedOn;            /* 0 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortTurnedOff            = tInterrutMaskRegister.bPortTurnedOff;           /* 1 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bDetectionUnsuccessful    = tInterrutMaskRegister.bDetectionUnsuccessful;   /* 2 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortFault                = tInterrutMaskRegister.bPortFault;               /* 3 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInUnderLoad       = tInterrutMaskRegister.bPortWasInUnderLoad;      /* 4 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInOverLoad        = tInterrutMaskRegister.bPortWasInOverLoad;       /* 5 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPortWasInPM              = tInterrutMaskRegister.bPortWasInPM;             /* 6 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPort_Power_Up_denied     = tInterrutMaskRegister.bPort_Power_Up_denied;    /* 7 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bDisconnectionTemperature = tInterrutMaskRegister.bDisconnectionTemperature;/* 8 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bUserDefinedTemperature   = tInterrutMaskRegister.bUserDefinedTemperature;  /* 9 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bPoEDeviceFault           = tInterrutMaskRegister.bPoEDeviceFault;          /* 10 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bNoMoreConnect            = tInterrutMaskRegister.bNoMoreConnect;           /* 12 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Interrupt_Mask_t.Set.tInterrutMaskRegister.bVmainFault               = tInterrutMaskRegister.bVmainFault;              /* 13 */

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Interrupt_Mask", eSet_Interrupt_Mask, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.9 Get Interrupt Mask   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Interrupt_Mask(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Interrupt_Mask", eGet_Interrupt_Mask, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.10 Set Individual Mask   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Individual_Mask(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Individual_Mask_e eINDV_MASK_OPT, POE_BOOL_e ePOE_BOOL_EnDis )
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Individual_Mask_t.Set.eINDV_MASK_OPT = eINDV_MASK_OPT;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Individual_Mask_t.Set.ePOE_BOOL_EnDis = ePOE_BOOL_EnDis;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Individual_Mask", eSet_Individual_Mask, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.11 Get Individual Mask   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Individual_Mask(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Individual_Mask_e eIndividual_Mask)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Individual_Mask_t.Set.eINDV_MASK_OPT = eIndividual_Mask;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Individual_Mask", eGet_Individual_Mask, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.12 Set System OK LED Mask Registers   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_System_OK_LED_Mask_Registers(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Protocol_interrupt_register_t tBlink_Register, Protocol_interrupt_register_t tLight_Register)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* 0xFF: unmask all Blink_Register events */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortTurnedOn             = tBlink_Register.bPortTurnedOn;            /* 0 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortTurnedOff            = tBlink_Register.bPortTurnedOff;           /* 1 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bDetectionUnsuccessful    = tBlink_Register.bDetectionUnsuccessful;   /* 2 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortFault                = tBlink_Register.bPortFault;               /* 3 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInUnderLoad       = tBlink_Register.bPortWasInUnderLoad;      /* 4 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInOverLoad        = tBlink_Register.bPortWasInOverLoad;       /* 5 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPortWasInPM              = tBlink_Register.bPortWasInPM;             /* 6 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPort_Power_Up_denied     = tBlink_Register.bPort_Power_Up_denied;    /* 7 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bDisconnectionTemperature = tBlink_Register.bDisconnectionTemperature;/* 8 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bUserDefinedTemperature   = tBlink_Register.bUserDefinedTemperature;  /* 9 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bPoEDeviceFault           = tBlink_Register.bPoEDeviceFault;          /* 10 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bNoMoreConnect            = tBlink_Register.bNoMoreConnect;           /* 12 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tBlink_Register.bVmainFault               = tBlink_Register.bVmainFault;              /* 13 */

	/* 0xFF: unmask all Light_Register events */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortTurnedOn             = tLight_Register.bPortTurnedOn;            /* 0 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortTurnedOff            = tLight_Register.bPortTurnedOff;           /* 1 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bDetectionUnsuccessful    = tLight_Register.bDetectionUnsuccessful;   /* 2 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortFault                = tLight_Register.bPortFault;               /* 3 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInUnderLoad       = tLight_Register.bPortWasInUnderLoad;      /* 4 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInOverLoad        = tLight_Register.bPortWasInOverLoad;       /* 5 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPortWasInPM              = tLight_Register.bPortWasInPM;             /* 6 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPort_Power_Up_denied     = tLight_Register.bPort_Power_Up_denied;    /* 7 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bDisconnectionTemperature = tLight_Register.bDisconnectionTemperature;/* 8 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bUserDefinedTemperature   = tLight_Register.bUserDefinedTemperature;  /* 9 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bPoEDeviceFault           = tLight_Register.bPoEDeviceFault;          /* 10 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bNoMoreConnect            = tLight_Register.bNoMoreConnect;           /* 12 */
	ptPoeMsg->PoE_Msg_Prm_e.Set_System_OK_LED_Mask_Registers_t.Set.tLight_Register.bVmainFault               = tLight_Register.bVmainFault;              /* 13 */

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_System_OK_LED_Mask_Registers", eSet_System_OK_LED_Mask_Registers, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.13 Get System OK LED Mask Registers   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_System_OK_LED_Mask_Registers(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_System_OK_LED_Mask_Registers", eGet_System_OK_LED_Mask_Registers, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.14 Set Power Indication LED   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Power_Indication_LED(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bIndicationOn_Percent,
    U8 bIndicationOff_Percent,
    U8 bIndicationOn_W,
    U8 bIndicationOff_W)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOn_Percent = bIndicationOn_Percent;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOff_Percent = bIndicationOff_Percent;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOn_W = bIndicationOn_W;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Indication_LED_t.Set.bIndicationOff_W = bIndicationOff_W;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Power_Indication_LED", eSet_Power_Indication_LED, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.15 Get Power Indication LED   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Power_Indication_LED(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Power_Indication_LED", eGet_Power_Indication_LED, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.1.16 Set System Masks - obsolete   ****************************************************************/


/* 4.1.17 Get Masks Status  - obsolete ****************************************************************/


/* 4.1.18 Get Software Version   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Software_Version(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return  TransmitMsg("Get_Software_Version", eGet_Software_Version, _INOUT ptPoeMsg, bI2C_Device_Address);
}



/****************************************************************************************/
/***************************   4.2 PoE Device messages    *******************************/
/****************************************************************************************/


/* 4.2.1 Set PoE Device Parameters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_PoE_Device_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCS_Num,
    U8 bTSH_C)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Parameters_t.Set.bCS_Num = bCS_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Parameters_t.Set.bTSH_C =  bTSH_C;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_PoE_Device_Parameters", eSet_PoE_Device_Parameters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.2.2 Get PoE Device Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_PoE_Device_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCS_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Status_t.Set.bCS_Num = bCS_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_PoE_Device_Status", eGet_PoE_Device_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.2.3 Set PoE Device Register   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_PoE_Device_Register(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U16 wRegister_Address,
    U16 wRegister_Data,
    U8 bDeviceNumber )
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Address = wRegister_Address;
	ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.wRegister_Data =  wRegister_Data;
	ptPoeMsg->PoE_Msg_Prm_e.Set_PoE_Device_Register_t.Set.bDeviceNumber =  bDeviceNumber;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_PoE_Device_Register", eSet_PoE_Device_Register, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.2.4 Get PoE Device Register   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_PoE_Device_Register(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U16 wRegister_Address,
    U8 	bDeviceNumber )
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Set.wRegister_Address = wRegister_Address;
	ptPoeMsg->PoE_Msg_Prm_e.Get_PoE_Device_Register_t.Set.bDeviceNumber = bDeviceNumber;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_PoE_Device_Register", eGet_PoE_Device_Register, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/****************************************************************************************/
/***************************   4.3 PoE Port messages    *********************************/
/******************************************************************************************/



/* 4.3.1 Set Temporary Matrix   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Temporary_Matrix( _INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num, U8 bPhysical_Number_A, U8 bPhysical_Number_B)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bPhysical_Number_A = bPhysical_Number_A;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Matrix_t.Set.bPhysical_Number_B = bPhysical_Number_B;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Temporary_Matrix", eSet_Temporary_Matrix, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.2 Get Physical Port Number from Temporary Matrix   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Physical_Port_Number_from_Temporary_Matrix(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Temporary_Matrix_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Physical_Port_Number_from_Temporary_Matrix", eGet_Physical_Port_Number_from_Temporary_Matrix, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.3 Program Global Matrix   ****************************************************************/
TRANSMIT_MSG_STATUS_e Program_Global_Matrix(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Program_Global_Matrix", eProgram_Global_Matrix, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.4 Get Physical Port Number from Active Matrix   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Physical_Port_Number_from_Active_Matrix(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Physical_Port_Number_from_Active_Matrix_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Physical_Port_Number_from_Active_Matrix", eGet_Physical_Port_Number_from_Active_Matrix, _INOUT ptPoeMsg, bI2C_Device_Address);
}


int Process_Cmd_field(Feature_EnDis_Type_e eFeature_EnDis_Type, U8 byVal)
{

	int iCmd = 0;

	switch (eFeature_EnDis_Type)
	{
		case eFeature_EnDis_Type_None:
			iCmd = 78;  /* ignore command */
			break;

		case eFeature_EnDis_Type_EnDisCh:
			byVal &= 1; /* filter the first bit */
			iCmd = byVal; /* set en dis */
			break;

		case eFeature_EnDis_Type_CapDis:
			byVal &= 1; /* filter the first bit */
			iCmd = 1 << 4; /* feature  bit - value 1 = cap dis */
			iCmd |= (U8)(byVal << 1);  /* set en dis */
			break;

		case eFeature_EnDis_Type_IcutDis:
			byVal &= 1; /* filter the first bit */
			iCmd = 1 << 5; /* feature  bit - value 1 = Icut dis */
			iCmd |= (U8)(byVal << 2);  /* set en dis */
			break;

		case eFeature_EnDis_Type_ConfigAll:
			byVal &= 0x7; /* filter the first 3 bits ( Bits 3 – This bit is reserved and should be set to '0' ) */
			iCmd = 0xF << 4; /* value 0xF = all */
			iCmd |= byVal;  /* set en dis */
			break;

		default:
			return -1;
	}

	return iCmd;
}


/* 4.3.5 Set Enable/Disable Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Enable_Disable_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis,  /* En/Dis the selected feature */
    Type_Mask_e eTYPE_MASK)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	int iCmd = Process_Cmd_field(eFeature_EnDis_Type, ePOE_BOOL_Feature_EnDis);
	if (iCmd < 0)
	{
		/* print the hardware error description and mark to exit from the manager loop */
		return eTransmitMsg_RestartManager;
	}

	ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.byPoeCmd = (U8)iCmd;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_Channels_t.Set.eTYPE_MASK = eTYPE_MASK;


	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Enable_Disable_Channels", eSet_Enable_Disable_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.6 Get All Ports Enable/Disable   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_Enable_Disable(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_Enable_Disable", eGet_All_Ports_Enable_Disable, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.7 Set Force Power   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Force_Power(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num, POE_BOOL_e ePOE_BOOL_EnDis )
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Force_Power_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Force_Power_t.Set.ePOE_BOOL_EnDis = ePOE_BOOL_EnDis;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Force_Power", eSet_Force_Power, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.8 Set Port Parameters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num ,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis,
    U32 ulPPL_mW,
    Port_Priority_e ePortPriority)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	int iCmd = Process_Cmd_field(eFeature_EnDis_Type, ePOE_BOOL_Feature_EnDis);
	if (iCmd < 0)
	{
		/* print the hardware error description and mark to exit from the manager loop */
		return eTransmitMsg_RestartManager;
	}

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.byPoeCmd = (U8)iCmd;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.ulPPL_mW = ulPPL_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Parameters_t.Set.ePortPriority = ePortPriority;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_Parameters", eSet_Port_Parameters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.9 Set Power Limit for Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Power_Limit_for_Channels(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num, U32 ulPPL_mW)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limit_for_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limit_for_Channels_t.Set.ulPPL_mW = ulPPL_mW;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Power_Limit_for_Channels", eSet_Power_Limit_for_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.10 Set Temporary Power Limit for Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Temporary_Power_Limit_for_Channels(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num, U32 ulTPPL_mW)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limit_for_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limit_for_Channels_t.Set.ulTPPL_mW = ulTPPL_mW;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Temporary_Power_Limit_for_Channels", eSet_Temporary_Power_Limit_for_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.11 Get Port Power Limit   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Power_Limit(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Power_Limit_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Power_Limit", eGet_Port_Power_Limit, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.12 Set 4-Pair Port Parameters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_4Pair_Port_Parameters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address,
        U8 bCh_Num,
        Feature_EnDis_Type_e eFeature_EnDis_Type,
        POE_BOOL_e ePOE_BOOL_Feature_EnDis,
        U32 ulPPL4Pair_mW,
        Port_Priority_e ePortPriority,
        Type_Mask_e ePort_Type,
        Sum_as_TPPL_PM_algorithm_e  eSum_as_TPPL_PM_algorithm,
        PortPM2_e ePortPM2,
        PortPM3_e ePortPM3)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));


	int iCmd = Process_Cmd_field(eFeature_EnDis_Type, ePOE_BOOL_Feature_EnDis);
	if (iCmd < 0)
	{
		/* print the hardware error description and mark to exit from the manager loop */
		return eTransmitMsg_RestartManager;
	}


	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.bCh_Num 		= bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.byPoeCmd        = (U8)iCmd;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ulPPL4Pair_mW 	= ulPPL4Pair_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPriority 	= ePortPriority;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePort_Type 		= ePort_Type;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.eSum_as_TPPL_PM_algorithm = eSum_as_TPPL_PM_algorithm;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPM2 		= ePortPM2;
	ptPoeMsg->PoE_Msg_Prm_e.Set_4Pair_Port_Parameters_t.Set.ePortPM3 		= ePortPM3;


	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_4Pair_Port_Parameters", eSet_4Pair_Port_Parameters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.13 Get 4-Pair Port Parameters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_Parameters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Parameters_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get 4Pair Port Parameters", eGet_4Pair_Port_Parameters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.14 Set Power Limits for 4-Pair Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Power_Limits_for_4_Pair_Channels(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8  bCh_Num, U32 ulPPL4Pair_mW)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limits_for_4_Pair_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Limits_for_4_Pair_Channels_t.Set.ulPPL4Pair_mW = ulPPL4Pair_mW;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Power_Limits_for_4_Pair_Channels", eSet_Power_Limits_for_4_Pair_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.15 Set Temporary Power Limits for 4-Pair Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Temporary_Power_Limits_for_4_Pair_Channels(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8  bCh_Num, U32 ulTPPL4Pair_mW)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limits_for_4_Pair_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Temporary_Power_Limits_for_4_Pair_Channels_t.Set.ulTPPL4Pair_mW = ulTPPL4Pair_mW;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Temporary_Power_Limits_for_4_Pair_Channels", eSet_Temporary_Power_Limits_for_4_Pair_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.16 Get 4-Pair Port Power Limit   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_Power_Limit(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_Power_Limit_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_4Pair_Port_Power_Limit", eGet_4Pair_Port_Power_Limit, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.17 Set Port Priority   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_Priority(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num, Port_Priority_e ePortPriority)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Priority_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Priority_t.Set.ePortPriority = ePortPriority;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_Priority", eSet_Port_Priority, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.18 Get Port Priority   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Priority(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Priority_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Priority", eGet_Port_Priority, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.19 Set Enable/Disable 4-Pair for Channels   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Enable_Disable_4_Pair_for_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis)  /* En/Dis the selected feature */
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	int iCmd = Process_Cmd_field(eFeature_EnDis_Type, ePOE_BOOL_Feature_EnDis);
	if (iCmd < 0)
	{
		/* print the hardware error description and mark to exit from the manager loop */
		return eTransmitMsg_RestartManager;
	}

	ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_4_Pair_for_Channels_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Enable_Disable_4_Pair_for_Channels_t.Set.byPoeCmd = (U8)iCmd;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Enable_Disable_4_Pair_for_Channels", eSet_Enable_Disable_4_Pair_for_Channels, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.20 Get All Ports Enable/Disable 4-Pair   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_Enable_Disable_4pair(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_Enable_Disable_4pair", eGet_All_Ports_Enable_Disable_4pair, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.21 Get Single Port Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Single_Port_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Single_Port_Status_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Single_Port_Status", eGet_Single_Port_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.22 Get All Ports Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Ports_Status_Group_e ePortsStatusGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Status_t.Set.ePortsStatusGroup = ePortsStatusGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_Status", eGet_All_Ports_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.23 Get Extended Port Status   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Extended_Port_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Extended_Port_Status_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Extended_Port_Status", eGet_Extended_Port_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.24 Get Internal Port Status (For Debug usage)   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Internal_Port_Status(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Internal_Port_Status_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Internal_Port_Status", eGet_Internal_Port_Status, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.25 Get UDL Counters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_UDL_Counters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, UDL_Counter_Group_e eUDL_CounterGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_UDL_Counters_t.Set.eCounterGroup = eUDL_CounterGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_UDL_Counters", eGet_UDL_Counters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.26 Get Detection Failure Counters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Detection_Failure_Counters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Detection_Counter_Group_e eDetCntGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Detection_Failure_Counters_t.Set.eDetCntGroup = eDetCntGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Detection_Failure_Counters", eGet_Detection_Failure_Counters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.27 Get Latches (Old Request to support backwards compatibility)   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Latches(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Latches_Group_e eLatchesGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Latches_t.Set.eLatchesGroup = eLatchesGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Latches", eGet_Latches, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.28 Get Global Port Counters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_global_port_counters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Counter_kind_e eCounter_kind, Counter_Group_e eCounter_Group)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterKind = eCounter_kind;
	ptPoeMsg->PoE_Msg_Prm_e.Get_global_port_counters_t.Set.eCounterGroup = eCounter_Group;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_global_port_counters", eGet_global_port_counters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.29 Get All Ports Class   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_Class(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Ports_Class_Group_e ePortsClassGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Class_t.Set.ePortsClassGroup = ePortsClassGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_Class", eGet_All_Ports_Class, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.30 Get All Ports delivering power state   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_delivering_power_state(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_delivering_power_state", eGet_All_Ports_delivering_power_state, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.3.31 Get Port Event Cause   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Event_Cause(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Event_Cause", eGet_Port_Event_Cause, _INOUT ptPoeMsg, bI2C_Device_Address);
}



/**********************************************************************************************/
/**********************   4.4 Power Management Related Messages    ****************************/
/**********************************************************************************************/


/* 4.4.1 Set PM Method   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_PM_Method( _INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address,
                                     PM1_System_Power_Calculated_Method_e ePM1_system_power_calculated_method,
                                     PM2_Port_Power_Limit_e               ePM2_Port_Power_Limit,
                                     PM3_Start_Up_Condition_e             ePM3_Start_up_conditions)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM1_system_power_calculated_method = ePM1_system_power_calculated_method;
	ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM2_Port_Power_Limit = ePM2_Port_Power_Limit;
	ptPoeMsg->PoE_Msg_Prm_e.Set_PM_Method_t.Set.ePM3_Start_up_conditions = ePM3_Start_up_conditions;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_PM_Method", eSet_PM_Method, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.2 Get PM Method   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_PM_Method(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get PM Method", eGet_PM_Method, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.3 Get Total Power   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Total_Power(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Total_Power", eGet_Total_Power, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.4 Set Power Banks   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Power_Banks(  _INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address,
                                        U8  bBank,
                                        U32 ulPowerLimit_mW,
                                        U32 ulMaxShutdownVoltage_mV,
                                        U32 ulMinShutdownVoltage_mV,
                                        U8  bGuardBand)
{


	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.bBank = bBank;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulPowerLimit_mW = ulPowerLimit_mW;   /* 1500W */
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulMaxShutdownVoltage_mV = ulMaxShutdownVoltage_mV;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.ulMinShutdownVoltage_mV = ulMinShutdownVoltage_mV;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Banks_t.Set.bGuardBand = bGuardBand;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Power_Banks", eSet_Power_Banks, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.5 Get Power Banks   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Power_Banks(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bBank)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Power_Banks_t.Set.bBank = bBank;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Power_Banks", eGet_Power_Banks, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.6 Get Power Supply Parameters   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Power_Supply_Parameters(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Power_Supply_Parameters", eGet_Power_Supply_Parameters, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.7 Get Power Supply Voltage   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Power_Supply_Voltage(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Power_Supply_Voltage", eGet_Power_Supply_Voltage, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.8 Get All Ports Power   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_Ports_Power(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Ports_Power_Group_e ePortsPowerGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_All_Ports_Power_t.Set.ePortsPowerGroup = ePortsPowerGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_Ports_Power", eGet_All_Ports_Power, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.9 Get All 4-Pair Ports Power   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_All_4Pair_Ports_Power(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, Ports_4Pair_Power_Group_e ePorts4PairPowerGroup)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_All_4Pair_Ports_Power_t.Set.ePorts4PairPowerGroup = ePorts4PairPowerGroup;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_All_4Pair_Ports_Power", eGet_All_4Pair_Ports_Power, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.10 Get All HIP Ports Power - obsolete - Please use the "Get All 4-Pair Ports Power " instead */


/* 4.4.11 Get Port Measurements   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Measurements(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Priority_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Measurements", eGet_Port_Measurements, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.4.12 Get 4-Pair Port Measurements   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_measurements(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_4Pair_Port_measurements_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_4Pair_Port_measurements", eGet_4Pair_Port_measurements, _INOUT ptPoeMsg, bI2C_Device_Address);
}




/****************************************************************************************/
/*********************   4.5 Power Derating, Related Messages    ************************/
/****************************************************************************************/

/* 4.5.1 Set Derating Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Derating_Data(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bBank, U32 ulMax_PS_Power_mW, U32 ulTstart_mC, U32 ulTshutdown_mC, U32 ulDerating_Delta_Power_mW)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.bBank = bBank;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulMax_PS_Power_mW = ulMax_PS_Power_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulTstart_mC = ulTstart_mC;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulTshutdown_mC = ulTshutdown_mC;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_Data_t.Set.ulDerating_Delta_Power_mW = ulDerating_Delta_Power_mW;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Derating_Data", eSet_Derating_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.5.2 Get Derating Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Derating_Data(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bBank)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Derating_Data_t.Set.bBank = bBank;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Derating_Data", eGet_Derating_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.5.3 Set Derating User Temperature   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Derating_User_Temperature(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U32 ulTemperature_mC)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Derating_User_Temperature_t.Set.ulTemperature_mC = ulTemperature_mC;   /* 70 C */

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Derating_User_Temperature", eSet_Derating_User_Temperature, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.5.4 Get Derating User Temperature   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Derating_User_Temperature(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Derating_User_Temperature", eGet_Derating_User_Temperature, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.5.5 Get System Measurements   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_System_Measurements(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_System_Measurements", eGet_System_Measurements, _INOUT ptPoeMsg, bI2C_Device_Address);
}



/****************************************************************************************/
/**************************   4.6 Laye2 Related Messages    *****************************/
/****************************************************************************************/



/* 4.6.1 Set Port Layer2 LLDP PD Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_LLDP_PD_Data(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address,
        U8 bCh_Num,
        Set_Layer2_PD_Type_t tSet_Layer2_PD_Type,
        U32 ulPD_Requested_Power_mW,
        U32 ulPSE_Allocated_Power_mW,
        U8  bCable_Length_meters,
        Execute_LLDP_t tExecute_LLDP)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePortPriority = tSet_Layer2_PD_Type.ePortPriority;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePower_source = tSet_Layer2_PD_Type.ePower_source;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePD_Power_type = tSet_Layer2_PD_Type.ePD_Power_type;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.ulPD_Requested_Power_mW = ulPD_Requested_Power_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.ulPSE_Allocated_Power_mW = ulPSE_Allocated_Power_mW;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.bCable_Length_meters = bCable_Length_meters;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_LLDP_PD_Data_t.Set.tExecute_LLDP = tExecute_LLDP;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_Layer2_LLDP_PD_Data", eSet_Port_Layer2_LLDP_PD_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.2 Get Port Layer2 LLDP PSE Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Layer2_LLDP_PSE_Data(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_LLDP_PSE_Data_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Layer2_LLDP_PSE_Data", eGet_Port_Layer2_LLDP_PSE_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.3 Set Power Bank Power Source Type   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Power_Bank_Power_Source_Type(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bBank, Power_Source_Type_e eMscc_PSE_power_source)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Bank_Power_Source_Type_t.Set.bBank = bBank;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Power_Bank_Power_Source_Type_t.Set.eMscc_PSE_power_source = eMscc_PSE_power_source;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Power_Bank_Power_Source_Type", eSet_Power_Bank_Power_Source_Type, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.4 Set Port PowerUp ALT-B   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_PowerUp_ALT_B(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U8 bit_0_PuP_CFG_EN ,
    U8 PuP_CMD)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.bit_0_PuP_CFG_EN = bit_0_PuP_CFG_EN;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_PowerUp_ALT_B_t.Set.PuP_CMD = PuP_CMD;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_PowerUp_ALT_B", eSet_Port_PowerUp_ALT_B, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.5 Get Port PowerUp ALT-B   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_PowerUp_ALT_B(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_PowerUp_ALT_B_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_PowerUp_ALT_B", eGet_Port_PowerUp_ALT_B, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.6 Set Port Layer2 CDP PD Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_CDP_PD_Data( _INOUT PoE_Msg_t *ptPoeMsg,
        U8 bI2C_Device_Address,
        U8 bCh_Num,
        Set_Layer2_PD_Type_t tSet_Layer2_PD_Type,
        U32 ulPD_Requested_Power0_mW,
        U32 ulPD_Requested_Power1_mW,
        U32 ulPD_Requested_Power2_mW,
        U32 ulPD_Requested_Power3_mW,
        U8 bCable_Length_meters,
        Execute_LLDP_t tExecute_LLDP)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.bCh_Num = bCh_Num;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePortPriority = tSet_Layer2_PD_Type.ePortPriority;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePower_source = tSet_Layer2_PD_Type.ePower_source;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tSet_Layer2_PD_Type.ePD_Power_type = tSet_Layer2_PD_Type.ePD_Power_type;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power0_mW = ulPD_Requested_Power0_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power1_mW = ulPD_Requested_Power1_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power2_mW = ulPD_Requested_Power2_mW;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.ulPD_Requested_Power3_mW = ulPD_Requested_Power3_mW;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.bCable_Length_meters = bCable_Length_meters;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_t.Set.tExecute_LLDP = tExecute_LLDP;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_Layer2_CDP_PD_Data", eSet_Port_Layer2_CDP_PD_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.7 Get Port Layer2 CDP PSE Data   ****************************************************************/
TRANSMIT_MSG_STATUS_e Get_Port_Layer2_CDP_PSE_Data(_INOUT PoE_Msg_t *ptPoeMsg, U8 bI2C_Device_Address, U8 bCh_Num)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Set.bCh_Num = bCh_Num;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_Port_Layer2_CDP_PSE_Data", eGet_Port_Layer2_CDP_PSE_Data, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.8 Set Port Layer2 CDP PD Data 2   ****************************************************************/
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_CDP_PD_Data_2( _INOUT PoE_Msg_t *ptPoeMsg,
        U8 bI2C_Device_Address,
        U8 					bCh_Num,
        Type_and_Exe_CDP_t  tType_and_Exe_CDP,
        U32 				ulPD_Requested_Power0_mW,
        U32 				ulPD_Requested_Power1_mW,
        U32 				ulPD_Requested_Power2_mW,
        U32 				ulPD_Requested_Power3_mW,
        U32 				ulPD_Requested_Power4_mW)

{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.bCh_Num = bCh_Num;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.tType_and_Exe_CDP = tType_and_Exe_CDP;

	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power0_dW = ulPD_Requested_Power0_mW / 100;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power1_dW = ulPD_Requested_Power1_mW / 100;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power2_dW = ulPD_Requested_Power2_mW / 100;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power3_dW = ulPD_Requested_Power3_mW / 100;
	ptPoeMsg->PoE_Msg_Prm_e.Set_Port_Layer2_CDP_PD_Data_2_t.Set.ulPD_Requested_Power4_dW = ulPD_Requested_Power4_mW / 100;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Set_Port_Layer2_CDP_PD_Data_2", eSet_Port_Layer2_CDP_PD_Data_2, _INOUT ptPoeMsg, bI2C_Device_Address);
}


/* 4.6.9	Get LLDP Math Conversion */
TRANSMIT_MSG_STATUS_e Get_LLDP_Math_Conversion(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8          bI2C_Device_Address,
    U8          bCh_Num,
    U16         PowerToConvert_mW,
    CalcOpt_e   eCalcOpt,
    PortType_e  ePortType,
    Deliver4P_e eDeliver4P,
    U8          bCable_Length_meters)
{
	memset(ptPoeMsg, 0, sizeof(PoE_Msg_t));

	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.bCh_Num = bCh_Num;
	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.PowerToConvert_dW = PowerToConvert_mW / 100;  /* convert from mW to dW */
	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.eCalcOpt = eCalcOpt;
	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.ePortType = ePortType;
	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.eDeliver4P = eDeliver4P;
	ptPoeMsg->PoE_Msg_Prm_e.Get_LLDP_Math_Conversion_t.Set.bCable_Length_meters = bCable_Length_meters;

	/* Send 15 byte protocol msg to PoE API */
	return TransmitMsg("Get_LLDP_Math_Conversion", eGet_LLDP_Math_Conversion, _INOUT ptPoeMsg, bI2C_Device_Address);
}
