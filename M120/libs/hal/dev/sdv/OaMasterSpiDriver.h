/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaMasterSpiDriver.h
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

#pragma once


#include "OaDriver.h"
#include "OaMasterSpiProto.h"
/*************************************************/

class COaMasterSpiDriver : public COaDriver
{
public:

    COaMasterSpiDriver(uint32_t uiHwIdx,
                       uint32_t uiHwSubIdx,
                       uint32_t uiResetHwIdx,
                       uint32_t uiResetHwSubIdx,
                       const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
                       const std::shared_ptr<COaSwProto> &c_rstpProtoPtr) :
        COaDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx), m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        /* TBD: Logic Driver */
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
        m_stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
#if 0
        Switch(2);      /*Switch(uiHwSubIdx); */
        usleep(10 * 1000);
#endif
    }
    virtual
    ~COaMasterSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg & rstMfg) override;
    virtual bool GetStatusData(std::vector<COaStatusData> &rstaData) override;
    virtual bool GetCfgData(COaCfgData & rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetDebugInfo(const char *pcFilePath) override;
    virtual bool GetMcuDebugInfo(const char *pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool GetGain(int16_t & rsGain, uint32_t uiSubIdx = 0) override;
    virtual bool GetTilt(int16_t & rsTilt, uint32_t uiSubIdx = 0) override;
    virtual bool GetRxLosThr(int16_t & rsThr, int16_t & rsHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetAutolosEnable(bool &rbEnable, uint32_t uiSubIdx = 0) override;
    virtual bool GetPumpEnable(bool &rbEnable, uint32_t uiSubIdx = 0) override;
    /* OA */
    virtual bool SetGainRange(uint32_t uiGainRange, uint32_t uiSubIdx = 0) override;
    virtual bool SetGain(uint32_t uiGain, uint32_t uiSubIdx = 0) override;
    virtual bool SetTilt(int16_t sTilt, uint32_t uiSubIdx = 0) override;
    virtual bool SetMode(uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx = 0) override;
    virtual bool SetRxLosThr(int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) override;
    virtual bool SetPumpEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetAprEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetAutolosEnable(bool bEnable, uint32_t uiSubIdx = 0) override;

    /* VOA */
    virtual bool SetAttn(uint32_t uiAttn, uint32_t uiSubIdx = 0) override;
    virtual bool SetTarget(uint32_t uiTarget, uint32_t uiSubIdx = 0) override;

    /* VOA get */
    virtual bool GetAttn(int16_t & rsAttn, uint32_t uiSubIdx = 0) override;

    /* Shutter */
    virtual bool SetShutterState(bool block, uint32_t uiSubIdx = 0) override;
    virtual bool GetShutterState(bool &block, uint32_t uiSubIdx = 0) override;

    /* reset */
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;

    /* upgrade */
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;
    virtual bool Swap(uint32_t uiSubIdx = 0) override;
    virtual bool Commit(uint32_t uiSubIdx = 0) override;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool SetDfbEnable(bool bEnable, int32_t iPower, uint32_t uiSubIdx) override;
    virtual bool SetOscEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetOtdrSwitchPos(uint32_t ulPos, uint32_t uiSubIdx) override;
    virtual bool SetOscAddThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetOscDropThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetOscRxThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetApsdEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetUpgState(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetLedState(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetLedAlm(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool GetCaliInfo(COaCaliData & rstCaliData, uint32_t uiSubIdx = 0) override;
    virtual bool SetOtdrCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool SetOcmCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool SetManualVoaEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetPaddingVoaConfigPower(uint32_t uiPower, uint32_t uiSubIdx = 0) override;
    virtual bool SetManualGainEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetThr(uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) override;
    virtual bool SetPumpCurrent(uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx = 0) override;
    virtual bool SetAlmMask(int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx = 0) override;
    virtual bool GetEventLog(COaEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 0) override;
    virtual bool SetReadyTrigger(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetAutoGainMode(uint32_t uiMode, uint32_t uiSubIdx = 0) override;
    virtual bool SetRemoteInterlockEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetDummyAdminKey(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetPanelSafeCmpt(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetOscAmpEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetReadyPower(uint32_t uiPower, uint32_t uiSubIdx) override;
private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<COaSwProto> m_stpProtoPtr = nullptr;
    std::shared_ptr<CLogicDriver> m_stpLogicDrvPtr = nullptr;

    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen, uint32_t uiTimeOut = 50);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg, uint32_t uiTimeOut = 50);
    int Switch(uint32_t uiChip);

    bool __UpgradeEnd();
    bool GetAlarmData(std::vector<COaStatusData> &rstaData);
    int __GetOtdrTab(char *c_pcFilePath, COtdrStartOffsetAll *pstOtdrCaliInfo);
    int __GetOcmTab(char *c_pcFilePath, COcmCali *pstOcmCaliInfo);
    bool __GetOtdrCali(COtdrStartOffsetAll & rstCaliData);
    bool __GetOcmCali(COcmCali & rstCaliData);
    EDFA_DATA_ST m_stOaData;
    std::mutex ms_stMutex;
    std::mutex &
    GetMutex(void)
    {
        return ms_stMutex;
    }
};










