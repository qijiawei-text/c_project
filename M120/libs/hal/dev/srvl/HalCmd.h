/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      HalCmd.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"

/*************************************************/

class CHalCmd
{
public:
    void CreateShm(void);

    static bool RegisterDev(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool UnregisterDev(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool LoopEnable(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool LoopInterval(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool IdleEnable(uint32_t uiLen, const uint8_t *c_pucBuf);

    CShareMemory &
    GetShm(uint32_t uiShmIdx)
    {
        return ms_staShm.at(uiShmIdx >= SHM_TYPE_MAX ? 0 : uiShmIdx);
    }
    CSemaphore &
    GetSem(uint32_t uiSemIdx)
    {
        return ms_staSem.at(uiSemIdx >= SHM_TYPE_MAX ? 0 : uiSemIdx);
    }

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_HAL_STA,
          SHM_SEM_KEY_HAL_STA,
          SHM_SIZE_HAL_STA},

         {SHM_KEY_HAL_CMD,
          SHM_SEM_KEY_HAL_CMD,
          SHM_SIZE_HAL_CMD},

         {SHM_KEY_HAL_DATA,
          SHM_SEM_KEY_HAL_DATA,
          SHM_SIZE_HAL_DATA}}
    };

    static std::array<CShareMemory, SHM_TYPE_MAX> ms_staShm;
    static std::array<CSemaphore, SHM_TYPE_MAX> ms_staSem;

};







