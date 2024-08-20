/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrCmd.h
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

#include "OtdrDriver.h"

/*************************************************/

class COtdrCmd
{
public:
    static void
    SetDevPtr(const std::shared_ptr<COtdrDriver> &c_rstpDrvPtr, uint32_t uiDevIdx)
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

    static bool StartScan(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPara(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetMeasuringTime(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetUserIOR(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetTemp(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* reset */
    static bool Reset(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* Upgrade */
    static bool Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* Debug */
    static bool DbgRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetScanMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetStartOffset(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetStartOffset(uint32_t uiLen, const uint8_t *cv_pucBuf);

private:
    static std::map<uint32_t, std::shared_ptr<COtdrDriver> > ms_staDrvPtrs;
    static std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > ms_staShm;
    static std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > ms_staSem;
};







