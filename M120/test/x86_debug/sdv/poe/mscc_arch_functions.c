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
 *  Description: contain the implementation of system depended functions.
 *
 *************************************************************************/

#include "mscc_arch_functions.h"



/*=========================================================================
/ Define here the Arcitecture
/========================================================================*/


#define _LINUX_PC_ARC_
/*#define _NEW_ARC_*/

#ifdef _LINUX_PC_ARC_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

static pthread_mutex_t sharedVariableMutex = PTHREAD_MUTEX_INITIALIZER;

/*---------------------------------------------------------------------
 *    description:     Sleep function
 *
 *    input :   sleepTime_mS  - sleep value in mili Seconds
 *                              minimum required range is: 20 mili seconds to 50 mili seconds
 *                              with resulution of 10 mili second
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                         - operation succeed
 *              e_POE_STATUS_ERR_SLEEP_FUNCTION_ERROR   - operation failed due to usleep function operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_Sleep_mS(U16 wSleepTime_mS)
{
	S32 lsStatus_number = 0;
	lsStatus_number = usleep(wSleepTime_mS * 1000);
	if (lsStatus_number != ePOE_STATUS_OK)
		return ePOE_STATUS_ERR_SLEEP_FUNCTION_ERROR;

	return ePOE_STATUS_OK;
}


/*---------------------------------------------------------------------
 *    description:     initialize the mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                     - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_INIT_ERROR   - operation failed due to mutex initialize operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_init()
{
	S32 lsStatus_number = 0;

	/* initializes the mutex */
	lsStatus_number = pthread_mutex_init(&sharedVariableMutex, NULL);
	if (lsStatus_number != ePOE_STATUS_OK)
		return ePOE_STATUS_ERR_MUTEX_INIT_ERROR;

	return ePOE_STATUS_OK;
}



/*---------------------------------------------------------------------
 *    description:     destroy the mutex - Clean up a mutex that is no longer needed
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                      - operation succeed
 *              ePOE_STATUS_ERR_MUTEX_DESTROY_ERROR - operation failed due to mutex initialize operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_destroy()
{
	S32 lsStatus_number = 0;

	/* initializes the mutex */
	lsStatus_number = pthread_mutex_destroy(&sharedVariableMutex);
	if (lsStatus_number != ePOE_STATUS_OK)
		return ePOE_STATUS_ERR_MUTEX_DESTROY_ERROR;

	return ePOE_STATUS_OK;
}


/*---------------------------------------------------------------------
 *    description:  locking a mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                       - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_LOCK_ERROR     - operation failed due to mutex lock operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_lock()
{
	S32 lsStatus_number = 0;
	/* lock the mutex. */
	lsStatus_number = pthread_mutex_lock(&sharedVariableMutex);
	if (lsStatus_number != ePOE_STATUS_OK)
		return ePOE_STATUS_ERR_MUTEX_LOCK_ERROR;

	return ePOE_STATUS_OK;
}


/*---------------------------------------------------------------------
 *    description:     Unlocking or releasing a mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                       - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_UNLOCK_ERROR   - operation failed due to mutex unlock operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_unlock()
{
	S32 lsStatus_number = 0;
	/* Release the mutex. */
	lsStatus_number = pthread_mutex_unlock(&sharedVariableMutex);
	if (lsStatus_number != ePOE_STATUS_OK)
		return ePOE_STATUS_ERR_MUTEX_UNLOCK_ERROR;

	return ePOE_STATUS_OK;
}

#elif defined(_NEW_ARC_)

/*---------------------------------------------------------------------
 *    description:     Sleep function
 *
 *    input :   sleepTime_mS  - sleep value in mili Seconds
 *                              minimum required range is: 20 mili seconds to 50 mili seconds
 *                              with resulution of 10 mili second
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                         - operation succeed
 *              e_POE_STATUS_ERR_SLEEP_FUNCTION_ERROR   - operation failed due to usleep function operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_Sleep_mS(U16 wSleepTime_mS)
{
	S32 lsStatus_number = 0;

	/* TODO - implement here the function depending your architecture */
#error OS_Sleep_mS function should be Implement.

	return ePOE_STATUS_OK;
}


/*---------------------------------------------------------------------
 *    description:     initialize the mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                     - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_INIT_ERROR   - operation failed due to mutex initialize operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_init()
{
	S32 lsStatus_number = 0;

	/* TODO - implement here the function depending your architecture */
#error OS_mutex_init function should be Implement.

	return ePOE_STATUS_OK;
}




/*---------------------------------------------------------------------
 *    description:     destroy the mutex - Clean up a mutex that is no longer needed
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                      - operation succeed
 *              ePOE_STATUS_ERR_MUTEX_DESTROY_ERROR - operation failed due to mutex initialize operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_destroy()
{
	S32 lsStatus_number = 0;

	/* TODO - implement here the function depending your architecture */
#error OS_mutex_destroy function should be Implement.

	return ePOE_STATUS_OK;
}



/*---------------------------------------------------------------------
 *    description:  locking a mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                       - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_LOCK_ERROR     - operation failed due to mutex lock operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_lock()
{
	S32 lsStatus_number = 0;

	/* TODO - implement here the function depending your architecture */
#error OS_mutex_lock function should be Implement.

	return ePOE_STATUS_OK;
}


/*---------------------------------------------------------------------
 *    description:     Unlocking or releasing a mutex
 *
 *    input :   none
 *
 *    output:   none
 *
 *    return:   ePOE_STATUS_OK                       - operation succeed
 *              e_POE_STATUS_ERR_MUTEX_UNLOCK_ERROR   - operation failed due to mutex unlock operation error
 *---------------------------------------------------------------------*/
PoE_Status_e OS_mutex_unlock()
{
	S32 lsStatus_number = 0;

	/* TODO - implement here the function depending your architecture */
#error OS_mutex_unlock function should be Implement.

	return ePOE_STATUS_OK;
}

#else
#error UNSUPPORTED PLATFORM
#endif

/*=========================================================================
/ End of Arcitecture Definition
/========================================================================*/

