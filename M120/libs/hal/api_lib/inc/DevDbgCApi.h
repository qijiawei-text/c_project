/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevTdcmCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_DBG_C_API__
#define _DEV_DBG_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"

#ifdef __cplusplus
extern "C" {
#endif

int DBG_ReadLogicReg(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_WriteLogicReg(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_ReadLogicRegSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_WriteLogicRegSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_LoopEnable(bool bEnable);
int DBG_LoopInterval(uint32_t uiInterval);
int DBG_IdleEnable(bool bEnable);
int DBG_ApsdEnable(int iIndex, bool bEnable);
int DBG_GetAprStatus(uint32_t uiIndex, uint8_t *pucVal);

int DBG_SpiSendCommand(uint32_t ulChip, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t ulLen);
int DBG_SpiFlashRead(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_SpiFlashWrite(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_MdioRead(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
int DBG_MdioWrite(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
int DBG_NetSwitchRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
int DBG_NetSwitchWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
int DBG_SwitchPhyRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
int DBG_SwitchPhyWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
int DBG_ReadEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int DBG_WriteEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);



#ifdef __cplusplus
};
#endif

#endif
