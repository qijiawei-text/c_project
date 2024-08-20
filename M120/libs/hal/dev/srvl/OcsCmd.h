/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsCmd.h
   Author:        Guo Guangjun
   Date:          2023-01-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2023-01-05] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"

#include "OcsDriver.h"

/*************************************************/

class COcsCmd
{
public:
    static void
    SetDevPtr(const std::shared_ptr<COcsDriver> &c_rstpDrvPtr, uint32_t uiDevIdx)
    {
        ms_staDrvPtrs[uiDevIdx] = c_rstpDrvPtr;
    }

    static void
    SetShm(const std::array<CShareMemory, SHM_TYPE_MAX> &c_rstaShm,
           const std::array<CSemaphore, SHM_TYPE_MAX> &c_rstaSem, uint32_t uiDevIdx)
    {
        ms_staShm[uiDevIdx] = c_rstaShm;
        ms_staSem[uiDevIdx] = c_rstaSem;
    }

    static void
    DeleteDevPtr(uint32_t uiDevIdx)
    {
        ms_staDrvPtrs.erase(uiDevIdx);
    }

    static void
    DeleteShm(uint32_t uiDevIdx)
    {
        ms_staShm.erase(uiDevIdx);
        ms_staSem.erase(uiDevIdx);
    }

    /* static bool GetMfg(uint32_t uiLen, const uint8_t *cv_pucBuf); */

    /* static bool GetSwitchChannels(uint32_t uiLen, const uint8_t *cv_pucBuf); */
    static bool SetSwitchChannels(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOneSwitchChannel(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool Reset(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool Commit(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetLampTestState(uint32_t uiLen, const uint8_t *cv_pucBuf);
    /* static bool GetLedStatus(uint32_t uiLen, const uint8_t *cv_pucBuf); */

private:
    static std::map<uint32_t, std::shared_ptr<COcsDriver> > ms_staDrvPtrs;
    static std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > ms_staShm;
    static std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > ms_staSem;
};







