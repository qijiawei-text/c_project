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
*  Description:  Code Encryption and PoE 15bytes protocol communication global types
*
*************************************************************************/

#ifndef MSCC_POE_GLOBAL_INTERNAL_H_
#define MSCC_POE_GLOBAL_INTERNAL_H_

#include "mscc_poe_global_types.h"

/*=========================================================================
   / CONSTANTS
   /========================================================================*/


/*=========================================================================
   / TYPES
   /========================================================================*/

FPTR_Write ptMscc_fptr_write;
FPTR_Read ptMscc_fptr_read;
U16 wExtra_Timeout_mSec;
Device_Method_e eDevice_Method;
Counters_t tCountersInternal;
POE_BOOL_e ePOE_BOOL_Printf_Enable;        /* enable API to print to stdout */





#endif /* MSCC_POE_GLOBAL_INTERNAL_H_ */

