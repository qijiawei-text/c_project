
#ifndef _SPI_EEPROM_H_
#define _SPI_EEPROM_H_

#include "oplk_datatype.h"

INT EESpiReadData(UINT unPageIndex, UCHAR *pucDataBuf, UINT unDataLen);
INT EESpiWriteData(UINT unPageIndex, UCHAR *pucDataBuf, UINT unDataLen);

#endif

