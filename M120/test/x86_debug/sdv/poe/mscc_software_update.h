#ifndef _SOFTWARE_UPDATE_H_
#define	_SOFTWARE_UPDATE_H_


#include "mscc_poe_global_types.h"

#define NX_HTTP_MAX_BINARY_MD5              16
#define NX_HTTP_MAX_ASCII_MD5               33
#define MAX_FILE_NAME_LEN                   100

/****************************************************************************
                          DEFINITIONS
****************************************************************************/


PoE_Status_e    eTftp_process_verification(char *sFolderLocation);
PoE_Status_e    eCmnd_do_poe_firmware_update(U8 bIC_Address, char S19FileName[]);

char            mS19_file_name[ MAX_FILE_NAME_LEN ]; /* file name to be loaded from TFTP Server (leave space for string zero termination) */


/****************************************************************************
*                  EXPORTED FUNCTIONS PROTOTYPES                            *
*****************************************************************************/

#endif	/* _SOFTWARE_UPDATE_H_ */

