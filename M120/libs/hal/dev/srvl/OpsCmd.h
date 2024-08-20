/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsCmd.h
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"
#include "OpsDriver.h"



/*************************************************/

class COpsCmd
{
public:
    static void
    SetDevPtr(const std::shared_ptr<COpsDriver> &c_rstpDrvPtr, uint32_t uiDevIdx)
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

    static bool GetCfgData(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool SetMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetWtr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetHoldOffTime(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetRevertive(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetRelativeThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetRelativeOffset(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPortPowerThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPortPowerHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetSwitchThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetSwitchThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetSwitchHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetSwitchHys(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool Reset(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool GetPortPowerHistory(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool SetWssPortPowerThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetWssPortPowerHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetWssPortPowerHirtory(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetWssLedMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetOscSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool SetLedStatus(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetLedMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool ClearAlarmDuration(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetRemoteStatus(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetRequestStatus(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetSwitchingType(uint32_t uiLen, const uint8_t *cv_pucBuf);

    static bool SetConnectionPdThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetConnectionPdHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetAlmMask(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetEventLog(uint32_t uiLen, const uint8_t *cv_pucBuf);
private:
    static std::map<uint32_t, std::shared_ptr<COpsDriver> > ms_staDrvPtrs;
    static std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > ms_staShm;
    static std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > ms_staSem;

    static bool __SetShmStatus(uint32_t uiIdx, int32_t uiStatus);
};







