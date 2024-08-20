/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotApi.h
   Author:        Zhen Zhang
   Date:          2019-02-20
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-02-20] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class CSlotApi : public CDevApi
{
public:
    CSlotApi()
    {
    }
    ~CSlotApi()
    {
    }

    int32_t SetOnline(uint32_t uiSlotIdx, bool bOnline);
    int32_t GetSlotNum(uint32_t &ruiSlotNum);
    int32_t GetMfg(uint32_t uiSlotIdx, CMfg &rstMfg);
    int32_t GetInfo(uint32_t uiSlotIdx, CSlotInfo &rstInfo);
    int32_t GetCaliInfo(uint32_t uiSlotIdx, CCali &rstCaliInfo);
    int32_t SetCali(uint32_t uiSlotIdx, const char *c_pcFilePath);
    int32_t GetStatusData(uint32_t uiSlotIdx, CSlotStatusData &rstStatus);

    int32_t SetMfg(uint32_t uiSlotIdx, const char *c_pcFilePath);
    int32_t ClearPlugFlag(uint32_t uiSlotIdx);
    int32_t SetCaliInfo(uint32_t uiSlotIdx, const char *c_pcFileName);

    /* LED */
    int32_t SetLedState(uint32_t uiSlotIdx, uint32_t uiLedIdx, uint32_t uiState);

    /* EEPROM */
    int32_t EepromRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t EepromWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);

    int32_t Reset(uint32_t uiDevIdx, uint32_t uiResetType, uint32_t uiSubIdx = 0);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_SLOT_STA,
          SHM_SEM_KEY_SLOT_STA,
          SHM_SIZE_SLOT_STA},

         {SHM_KEY_SLOT_CMD,
          SHM_SEM_KEY_SLOT_CMD,
          SHM_SIZE_SLOT_CMD},

         {SHM_KEY_SLOT_DATA,
          SHM_SEM_KEY_SLOT_DATA,
          SHM_SIZE_SLOT_DATA}}
    };

};






