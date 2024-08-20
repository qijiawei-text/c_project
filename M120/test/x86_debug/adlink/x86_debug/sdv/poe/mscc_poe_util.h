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
*  Description:
*
*
*************************************************************************/

#ifndef _MSCC_POE_UTIL_H_
#define _MSCC_POE_UTIL_H_

/*=========================================================================
   / INCLUDES
   /========================================================================*/

#include <stdarg.h>
#include "mscc_poe_global_types.h"


/*=========================================================================
   / PROTOTYPES
   /========================================================================*/

U8                GetEcho();
PoE_Status_e eMscc_POE_UTIL_CheckCommCheckSumValidity(U8 byDataArr[]);
U16 mscc_POE_UTIL_CommCalculateCheckSum(U8 byDataArr[]);
void mscc_POE_UTIL_SetCheckSum(U8 byArrData[]);
POE_BOOL_e        mscc_POE_UTIL_CheckSumTest(U8 *byArrData);
char *MSCC_POE_UTIL_GetErrDescription(PoE_Status_e ePOE_STATUS);
POE_BOOL_e CheckIfArrayHasTheSameContent(U8 bArr[], U8 bData, U16 msg_len);
void              PrintCounters(Counters_t *ptCounters);
void                      MSCC_POE_UTIL_PrintPoEDeviceHardwareError(Hardware_Reset_Detected_e eHardwareResetDetected);
void              mscc_printf(const char *fmt, ...);

/* You might want to replace the printfs with calls ... */
#define MSCC_PRINTF_INFO(fmt, args ...)   mscc_printf(fmt, ## args)
#define MSCC_PRINTF_ERROR(fmt, args ...)  mscc_printf(__FILE__ " %s, %d: " fmt "\n", __func__, __LINE__, ## args)

#endif /* _MSCC_POE_UTIL_H_ */

