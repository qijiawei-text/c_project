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

#ifndef _MSCC_POE_COMMUNICATION_PROTOCOL_H_
#define _MSCC_POE_COMMUNICATION_PROTOCOL_H_


/*=========================================================================
   / INCLUDES
   /========================================================================*/
#include "mscc_poe_global_types.h"
#include "mscc_arch_functions.h"


/*=========================================================================
   / PROTOTYPES
   /========================================================================*/
PoE_Status_e ePOE_COMMUNICATION_PROTOCOL_AnalysisAndExecutionCommand(Protocol_15Bytes_Msg_Type_e MsgType, _INOUT PoE_Msg_t *ptPoeMsg, U8 bIC_Address);
PoE_Status_e ePOE_COMMUNICATION_PROTOCOL_InitSoftware(Init_Info_t *ptInitInfo);
PoE_Status_e ePOE_COMMUNICATION_PROTOCOL_Clear_PoE_Device_Buffer(U8 bIC_Address);

#endif  /* _MSCC_POE_COMMUNICATION_PROTOCOL_H_ */

