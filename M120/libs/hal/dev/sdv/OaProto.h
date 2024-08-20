/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaProto.h
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
#include "RspHandler.h"
#include "Proto.h"

/*************************************************/

enum EOaCliProto
{
    OA_CLI_RSP_MAX_LEN = 1024,
    OA_READ_NUM        = 500,
    OA_CMD_RSP_LEN         = 32,


};



class COaSwProto : public CProto
{
public:
    COaSwProto()
    {
    }
    virtual
    ~COaSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetStatusData(std::string &rstaMsg) = 0;
    virtual bool GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData) = 0;
    virtual void GetGain(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain) = 0;
    virtual void GetTilt(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt) = 0;
    virtual void GetRxLosThr(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys) = 0;
    virtual void GetDebugInfoCmd(std::string &rstaMsg) = 0;
    virtual void GetMcuDebugInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx) = 0;

    virtual void GetFpgaInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx) = 0;
    virtual bool GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) = 0;
    virtual void GetAutolosEnable(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) = 0;
    virtual void GetPumpEnable(uint32_t uiId, std::string &rstaMsg) = 0;
    /* OA */
    virtual void SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange) = 0;
    virtual void SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain) = 0;
    virtual void SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt) = 0;
    virtual void SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal) = 0;
    virtual void SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr) = 0;
    virtual void SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys) = 0;
    virtual void SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) = 0;
    virtual void SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;

    /* VOA */
    virtual void SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn) = 0;
    virtual void SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget) = 0;

    /* VOA get */
    virtual void GetAttn(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn) = 0;

    /* Shutter */
    virtual bool SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block) = 0;
    virtual bool GetShutterState(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual bool GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block) = 0;

    /* Upgarde */
    virtual void Upgrade(uint32_t uiId, std::string &rstaMsg) = 0;

    /* Reset */
    virtual void SoftReset(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual void Swap(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual void Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg) = 0;
    virtual void Verify(uint32_t uiId, std::string &rstaMsg) = 0;
    virtual void Commit(uint32_t uiId, std::string &rstaMsg) = 0;

    virtual void GetAlarmData(std::string &rstaMsg) = 0;
    virtual bool GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData) = 0;
    virtual void SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower) = 0;
    virtual void SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos) = 0;
    virtual void SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) = 0;
    virtual void SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) = 0;
    virtual void SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) = 0;
    virtual void SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) = 0;
    virtual void SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) = 0;
    virtual void SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) = 0;
    virtual void GetOtdrCaliCmd(std::string &rstaMsg) = 0;
    virtual bool GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstCali) = 0;
    virtual void GetOcmCaliCmd(std::string &rstaMsg) = 0;
    virtual bool GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstCali) = 0;
    virtual void SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali) = 0;
    virtual void SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali) = 0;
    virtual void SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) = 0;
    virtual void SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys) = 0;
    virtual void SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent) = 0;
    virtual void SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal) = 0;
    virtual void GetEventLogCmd(std::string &rstaMsg) = 0;
    virtual bool GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstCali) = 0;
    virtual void SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) = 0;
    virtual void SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode) = 0;
    virtual bool SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual bool SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual bool SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable) = 0;
    virtual void SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) = 0;
    virtual void SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) = 0;
};

class COaSwCliProto : public COaSwProto
{
public:
    COaSwCliProto()
    {
    }
    virtual
    ~COaSwCliProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;
    virtual bool GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData) override;
    virtual void GetDebugInfoCmd(std::string &rstaMsg) override;
    virtual void GetMcuDebugInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx) override;
    virtual void GetFpgaInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx) override;

};

class COaSwCliSingleProto : public COaSwCliProto
{
public:
    /* OA */
    virtual void SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange) override;
    virtual void SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain) override;
    virtual void SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt) override;
    virtual void SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal) override;
    virtual void SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr) override;
    virtual void SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys) override;
    virtual void SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;

    /* OA get */
    virtual void GetGain(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain) override;
    virtual void GetTilt(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt) override;
    virtual void GetRxLosThr(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys) override;
    virtual void GetPumpEnable(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) override;
    virtual void GetAutolosEnable(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) override;

    /* VOA */
    virtual void SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn) override;
    virtual void SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget) override;

    /* VOA get */
    virtual void GetAttn(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn) override;

    /* Shutter */
    virtual bool SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block) override;
    virtual bool GetShutterState(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block) override;

    /* reset */
    virtual void SoftReset(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Upgrade(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Swap(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg) override;
    virtual void Verify(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Commit(uint32_t uiId, std::string &rstaMsg) override;
    virtual void GetAlarmData(std::string &rstaMsg) override;
    virtual bool GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData) override;
    virtual void SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower) override;
    virtual void SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos) override;
    virtual void SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void GetOtdrCaliCmd(std::string &rstaMsg) override;
    virtual bool GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstCali) override;
    virtual void GetOcmCaliCmd(std::string &rstaMsg) override;
    virtual bool GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstCali) override;
    virtual void SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali) override;
    virtual void SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali) override;
    virtual void SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
    virtual void SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys) override;
    virtual void SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent) override;
    virtual void SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal) override;
    virtual void GetEventLogCmd(std::string &rstaMsg) override;
    virtual bool GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstCali) override;
    virtual void SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode) override;
    virtual bool SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual bool SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual bool SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
    virtual void SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
};

class COaSwCliMultProto : public COaSwCliProto
{
public:
    /* OA */
    virtual void SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange) override;
    virtual void SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain) override;
    virtual void SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt) override;
    virtual void SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal) override;
    virtual void SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr) override;
    virtual void SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys) override;
    virtual void SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;

    /* OA get */
    virtual void GetGain(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain) override;
    virtual void GetTilt(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt) override;
    virtual void GetRxLosThr(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys) override;
    virtual void GetPumpEnable(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) override;
    virtual void GetAutolosEnable(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable) override;

    /* VOA */
    virtual void SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn) override;
    virtual void SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget) override;

    /* VOA get */
    virtual void GetAttn(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn) override;

    /* Shutter */
    virtual bool SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block) override;
    virtual bool GetShutterState(uint32_t uiId, std::string &rstaMsg) override;
    virtual bool GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block) override;

    /* reset */
    virtual void SoftReset(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Upgrade(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Swap(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg) override;
    virtual void Verify(uint32_t uiId, std::string &rstaMsg) override;
    virtual void Commit(uint32_t uiId, std::string &rstaMsg) override;
    virtual void GetAlarmData(std::string &rstaMsg) override;
    virtual bool GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData) override;
    virtual void SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower) override;
    virtual void SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos) override;
    virtual void SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys) override;
    virtual void SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void GetOtdrCaliCmd(std::string &rstaMsg) override;
    virtual bool GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstCali) override;
    virtual void GetOcmCaliCmd(std::string &rstaMsg) override;
    virtual bool GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstCali) override;
    virtual void SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali) override;
    virtual void SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali) override;
    virtual void SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
    virtual void SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys) override;
    virtual void SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent) override;
    virtual void SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal) override;
    virtual void GetEventLogCmd(std::string &rstaMsg) override;
    virtual bool GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstCali) override;
    virtual void SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState) override;
    virtual void SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode) override;
    virtual bool SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual bool SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual bool SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable) override;
    virtual void SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
    virtual void SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower) override;
};



class COaSwK20Proto : public COaSwCliMultProto
{
public:

    /* reset */
    virtual void Upgrade(uint32_t uiId, std::string &rstaMsg) override;
};





