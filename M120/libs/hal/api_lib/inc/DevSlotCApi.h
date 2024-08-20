/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevSlotCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_SLOT_C_API__
#define _DEV_SLOT_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevSlotData.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GET_DEV_IDX(uiDevType, puiDevIdx, puiDevSubIdx) \
    if (SLOT_GetDevIdx(uiDevType, puiDevIdx, puiDevSubIdx) != OPLK_OK) { \
        return -1; \
    } \

int SLOT_GetDevIdx(uint32_t uiDevType, uint32_t *puiDevIdx, uint32_t *puiDevSubIdx);

int SLOT_SetOnline(uint32_t uiSlotIdx, bool bOnline);
int SLOT_GetSlotNum(uint32_t *puiSlotNum);
int SLOT_SetMfg(uint32_t uiSlotIdx, const char *c_pcFilePath);
int SLOT_GetMfg(uint32_t uiSlotIdx, CMfg *pstMfg);
int SLOT_GetStatusData(uint32_t uiSlotIdx, CSlotStatusData *pstStatus);
int SLOT_ClearPlugFlag(uint32_t uiSlotIdx);
int SLOT_GetInfo(uint32_t uiSlotIdx, CSlotInfo *pstInfo);
int SLOT_GetCali(uint32_t uiSlotIdx, CCali *pstCaliInfo);
int SLOT_SetCali(uint32_t uiSlotIdx, const char *pcFilePath);
int SLOT_Reset(uint32_t uiSlotIdx, uint8_t ucResetType);
int SLOT_SetLed(uint32_t uiSlotIdx, uint32_t uiLedIdx, uint32_t uiState);

int SLOT_ReadEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
int SLOT_WriteEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);


#ifdef __cplusplus
};
#endif

#endif
