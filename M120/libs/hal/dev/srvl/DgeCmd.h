/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeCmd.h
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"

#include "DgeDriver.h"



/*************************************************/

class CDgeCmd
{
public:
    static void
    SetDevPtr(const std::shared_ptr<CDgeDriver> &c_rstpDrvPtr, uint32_t uiDevIdx)
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

private:
    static std::map<uint32_t, std::shared_ptr<CDgeDriver> > ms_staDrvPtrs;
    static std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > ms_staShm;
    static std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > ms_staSem;
};







