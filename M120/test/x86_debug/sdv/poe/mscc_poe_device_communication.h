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

#ifndef _MSCC_POE_DEVICE_COMM_H_
#define _MSCC_POE_DEVICE_COMM_H_

/*=========================================================================
/ INCLUDES
/========================================================================*/
#include "mscc_poe_global_types.h"

/*=========================================================================
/ PROTOTYPES
/========================================================================*/

PoE_Status_e eDEVICE_COMMUNICATION_Get_15Bytes_CommProtocol_Reply(U8 bIC_Address, U8 byEcho, _OUT U8 byArrRxBuffer[], _OUT POE_BOOL_e *pePOE_BOOL_Is_system_status);
PoE_Status_e eDEVICE_COMMUNICATION_Write(U8 bI2C_Address, const U8* pbyTxdata, U16 num_write_length);
PoE_Status_e eDEVICE_COMMUNICATION_Read (U8 bI2C_Address, _OUT U8* pRxdata, U16 bytes_to_read_length, U16 buffer_size, _OUT U16* pNumber_of_bytes_actually_read, READ_MODE_e eReadMode);
PoE_Status_e ReadAllRxBuffer(U8 bIC_Address, _OUT U8 byArrRxBuff[] , _OUT U16* pNumber_of_bytes_actually_read);
void         ClearRxBuffer(U8 bIC_Address);
PoE_Status_e eDEVICE_COMMUNICATION_ReadPattern(U8 bIC_Address, _OUT U8 byArrRxBuff[], U16 bytes_to_read_length, _OUT U16* pNumber_of_bytes_actually_read);
PoE_Status_e Send_General_15bytesCommProtocol_Command_or_Telemetry(U8 byIC_Address, U8 byTxArr[], U8 byArrRxBuff[], POE_BOOL_e *pePOE_BOOL_Is_system_status);

#endif /* _MSCC_POE_DEVICE_COMM_H_ */

