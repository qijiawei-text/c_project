/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaCmd.h
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
#include "OaDriver.h"

/*************************************************/

class COaCmd
{
public:
    static void
    SetDevPtr(const std::shared_ptr<COaDriver> &c_rstpDrvPtr, uint32_t uiDevIdx)
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

    static bool GetMfg(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetCfg(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetDebugInfo(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetMcuDebugInfo(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* OA */
    static bool SetGainRange(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetGain(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetTilt(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetRxLosThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPumpEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetAprEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetAutolosEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* VOA */
    static bool SetAttn(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetTarget(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* Shutter */
    static bool SetShutterState(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetShutterState(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* reset */
    static bool Reset(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* Upgrade */
    static bool Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool Swap(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool Commit(uint32_t uiLen, const uint8_t *cv_pucBuf);

    /* Debug */
    static bool DbgRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgWrite(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool DbgSetModuleAlarm(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool DbgClearModuleAlarm(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetDfbEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOtdrSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscAddThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscDropThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscRxThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetApsdEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetUpgState(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetLedState(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetLedAlm(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetCaliInfo(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOtdrCali(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOcmCali(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetManualVoaEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPaddingVoaConfigPower(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetManualGainEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetThr(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPumpCurrent(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetAlmMask(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool GetEventLog(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetReadyTrigger(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetAutoGainMode(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetRemoteInterlockEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetDummyAdminKey(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetPanelSafeCmpt(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetOscAmpEnable(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool SetReadyPower(uint32_t uiLen, const uint8_t *cv_pucBuf);
private:
    static std::map<uint32_t, std::shared_ptr<COaDriver> > ms_staDrvPtrs;
    static std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > ms_staShm;
    static std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > ms_staSem;
};







