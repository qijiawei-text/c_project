/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsDriver.h
   Author:        Guo Guangjun
   Date:          2024-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-04] [1.0] [Creator]

*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "BoardCmd.h"
#include "OcsProto.h"

/*************************************************/

class COcsDriver : public CDevDriver
{
public:
    COcsDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    COcsDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }

    COcsDriver(uint32_t uiHwIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    COcsDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    virtual
    ~COcsDriver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<COcsSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(COcsStatusData &rstData) = 0;
    virtual bool GetCfgData(COcsCfgData &rstData) = 0;

    virtual bool GetSwitchChannels(COcsConnections &rstData) = 0;
    virtual bool SetSwitchChannels(const COcsConnections &rstData) = 0;
    virtual bool SetOneSwitchChannel(const COcsConnection &rstData) = 0;

    virtual bool GetLedState(COcsLedSta &rstData) = 0;
    virtual bool SetLampTestState(bool bEnable) = 0;

    virtual bool GetAlarm(COcsAlarm &rstAlarm) = 0;

    virtual bool GetCaseTemperature(COcsTempSta &rstData) = 0;

    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) = 0;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) = 0;
    virtual bool Commit(uint32_t uiDevIdx) = 0;
protected:
    std::shared_ptr<COcsSwProto> m_stpProtoPtr = nullptr;

};

class COcsSpiDriver : public COcsDriver
{
public:
    COcsSpiDriver(uint32_t uiHwIdx,
                  uint32_t uiResetHwIdx,
                  uint32_t uiResetHwSubIdx,
                  const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr) :
        COcsDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx),
        m_stpDrvPtr(c_rstpDrvPtr)
    {
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);

        m_ucSwitchType = OCS_TYPE_16X16_SWITCH;
    }

    virtual
    ~COcsSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COcsStatusData &rstData) override;
    virtual bool GetCfgData(COcsCfgData &rstData) override;

    virtual bool GetSwitchChannels(COcsConnections &rstData) override;
    virtual bool SetSwitchChannels(const COcsConnections &rstData) override;
    virtual bool SetOneSwitchChannel(const COcsConnection &rstData) override;

    virtual bool GetLedState(COcsLedSta &rstData) override;
    virtual bool SetLampTestState(bool bEnable) override;

    virtual bool GetAlarm(COcsAlarm &rstAlarm) override;

    virtual bool GetCaseTemperature(COcsTempSta &rstData) override;

    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;
    virtual bool Commit(uint32_t uiDevIdx) override;
protected:

    int __ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR);
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;

private:
    uint8_t m_ucSwitchType;                /* refer to EN_OCS_TYPE_SWITCH */
    void ParseSwitchType(const char *pcModuleType);
    uint32_t GetSwitchChannelsCount();
};

