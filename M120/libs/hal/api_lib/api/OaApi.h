/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaApi.h
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
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class COaApi : public CDevApi
{
public:
    COaApi()
    {
    }
    ~COaApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t Swap(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t Commit(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);

    int32_t GetStatusData(uint32_t uiDevIdx, COaStatusData &rstStatus, uint32_t uiSubIdx = 0);
    int32_t GetCfgData(uint32_t uiDevIdx, COaCfgData &rstCfg, uint32_t uiSubIdx = 0);
    int32_t GetDebugInfo(uint32_t uiDevIdx, const char *pcFilePath);
    int32_t GetMcuDebugInfo(uint32_t uiDevIdx, const char *pcFilePath, uint32_t uiSubIdx = 0);

    /* OA */
    int32_t SetGainRange(uint32_t uiDevIdx, uint32_t uiGainRange, uint32_t uiSubIdx = 0);
    int32_t SetGain(uint32_t uiDevIdx, uint32_t uiGain, uint32_t uiSubIdx = 0);
    int32_t SetTilt(uint32_t uiDevIdx, int16_t sTilt, uint32_t uiSubIdx = 0);
    int32_t SetMode(uint32_t uiDevIdx, uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx = 0);
    int32_t SetPumpEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetAprEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetAutolosEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetRxLosThr(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0);

    /* VOA */
    int32_t SetAttn(uint32_t uiDevIdx, int16_t sAttn, uint32_t uiSubIdx = 0);
    int32_t SetTarget(uint32_t uiDevIdx, int16_t sTarget, uint32_t uiSubIdx = 0);

    /* DBG */
    int32_t DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgSetModuleAlarm(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearModuleAlarm(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t SetDfbEnable(uint32_t uiDevIdx, bool bEnable, int32_t iPower, uint32_t uiSubIdx = 0);
    int32_t SetOscEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetOtdrSwitchPos(uint32_t uiDevIdx, uint32_t uiPos, uint32_t uiSubIdx = 0);
    int32_t SetOscAddThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0);
    int32_t SetOscDropThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0);
    int32_t SetOscRxThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0);
    int32_t SetApsdEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetUpgState(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx = 0);
    int32_t SetLedState(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx = 0);
    int32_t SetLedAlm(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx = 0);
    int32_t GetCaliInfo(uint32_t ulDevIdx, COaCaliData &rstCaliData, uint32_t uiSubIdx = 0);
    int32_t SetOtdrCali(uint32_t uiDevIdx, char *c_pcFilePath, uint32_t uiSubIdx = 0);
    int32_t SetOcmCali(uint32_t uiDevIdx, char *c_pcFilePath, uint32_t uiSubIdx = 0);
    int32_t SetManualVoaEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetPaddingVoaConfigPower(uint32_t uiDevIdx, int16_t sOutPower, uint32_t uiSubIdx = 0);
    int32_t SetManualGainEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetThrHys(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0);
    int32_t SetPumpCurrent(uint32_t uiDevIdx, uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx = 0);
    int32_t SetAlmMask(uint32_t uiDevIdx, int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx = 0);
    int32_t GetEventLog(uint32_t ulDevIdx, COaEventLogAllData &rstEventLogAll, uint32_t uiSubIdx = 0);

    int32_t SetShutterState(uint32_t uiDevIdx, bool block, uint32_t uiSubIdx = 0);
    int32_t GetShutterState(uint32_t uiDevIdx, bool &block, uint32_t uiSubIdx = 0);
    int32_t SetReadyTrigger(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx = 0);
    int32_t SetAutogainMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx = 0);
    int32_t SetRemoteInterlockEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetDummyAdminKey(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetPanelSafeCmpt(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetOscAmpEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);
    int32_t SetReadyPower(uint32_t uiDevIdx, uint32_t uiPower, uint32_t uiSubIdx = 0);
private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OA_STA,
                                                            SHM_SEM_KEY_OA_STA,
                                                            SHM_SIZE_OA_STA},

                                                           {SHM_KEY_OA_CMD,
                                                            SHM_SEM_KEY_OA_CMD,
                                                            SHM_SIZE_OA_CMD},

                                                           {SHM_KEY_OA_DATA,
                                                            SHM_SEM_KEY_OA_DATA,
                                                            SHM_SIZE_OA_DATA}}};


};






