/***MODU+******************************************************************************************/

#ifndef __SDV_USB_H__
#define __SDV_USB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "oplk_datatype.h"

/**************************************************************************************************/
/* @defgroup sdv_usb_Exported_Functions                                                          */
/**************************************************************************************************/

extern int SDV_UsbSpiInit(void);
extern INT SDV_UsbSpiWriteRead(U8 *ucDateTx, U8 *ucDataRx, U16 iLen);

extern int SDV_UsbI2cInit(void);
extern int SDV_UsbI2CRead(U8 ucSlaveAddr, U8 *ucDataRx, U16 iLen);
extern int SDV_UsbI2CWrite(U8 ucSlaveAddr, U8 *ucDataTx, U16 iLen);

extern int SDV_UsbClose(void);


#ifdef __cplusplus
}
#endif

#endif

