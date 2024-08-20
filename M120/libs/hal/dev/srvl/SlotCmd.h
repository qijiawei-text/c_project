/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardCmd.h
   Author:        Zhu Weian
   Date:          2019-08-16
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian ] [2019-08-16] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "SlotDriver.h"

#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"



/*************************************************/

class CSlotCmd
{
public:
    static void
    SetSlotPtr(const std::vector<std::shared_ptr<CSlotDriver> > &c_rstaDrvPtrs)
    {
        ms_staDrvPtrs = c_rstaDrvPtrs;
    }

    static void
    SetShm(const std::array<CShareMemory, SHM_TYPE_MAX> &c_rstaShm,
           const std::array<CSemaphore, SHM_TYPE_MAX> &c_rstaSem)
    {
        ms_staShm = c_rstaShm;
        ms_staSem = c_rstaSem;
    }

    static std::shared_ptr<CSlotDriver>
    GetDrvPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staDrvPtrs.size()) ? nullptr : ms_staDrvPtrs.at(uiIdx);
    }

    static bool SetOnline(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetMfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetCali(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool ClearPlugFlag(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetLedState(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool EepromWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool EepromRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool Reset(uint32_t uiLen, const uint8_t *c_pucBuf);


private:
    static std::vector<std::shared_ptr<CSlotDriver> > ms_staDrvPtrs;

    static std::array<CShareMemory, SHM_TYPE_MAX> ms_staShm;
    static std::array<CSemaphore, SHM_TYPE_MAX> ms_staSem;
};




