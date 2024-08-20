/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmDriver.h
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
#include "DevData.h"
#include "DevDriver.h"
#include "BoardCmd.h"
#include "OcmProto.h"

/*************************************************/

class COcmDriver : public CDevDriver
{
public:
    COcmDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    COcmDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }

    COcmDriver(uint32_t uiHwIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    COcmDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    virtual
    ~COcmDriver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<COcmSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(COcmStatusData &rstData) = 0;
    virtual bool GetCfgData(COcmCfgData &rstData) = 0;

    virtual uint32_t GetPortCnt() = 0;
    virtual bool GetChPower(uint32_t uiPortIdx, COcmChPower &rstData) = 0;
    virtual bool SetWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan) = 0;
    virtual bool SetOcmWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan) = 0;
    virtual bool GetOsa(uint32_t uiPortIdx, COcmOsa &rstData) = 0;
    virtual bool GetRealTimeOsa(uint32_t uiPortIdx, COcmOsa &rstData) = 0;
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) = 0;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) = 0;
    virtual bool Commit(uint32_t uiDevIdx) = 0;
protected:
    std::shared_ptr<COcmSwProto> m_stpProtoPtr = nullptr;
    uint32_t m_uiPortCnt = 5; /* TODO: 400GZR 5 port */

};

class COcmSpiDriver : public COcmDriver
{
public:
    COcmSpiDriver(uint32_t uiHwIdx,
                  uint32_t uiResetHwIdx,
                  uint32_t uiResetHwSubIdx,
                  const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr) :
        COcmDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx),
        m_stpDrvPtr(c_rstpDrvPtr)
    {
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }

    virtual
    ~COcmSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COcmStatusData &rstData) override;
    virtual bool GetCfgData(COcmCfgData &rstData) override;

    virtual uint32_t GetPortCnt() override;
    virtual bool GetChPower(uint32_t uiPortIdx, COcmChPower &rstData) override;
    virtual bool GetOsa(uint32_t uiPortIdx, COcmOsa &rstData) override;
    virtual bool GetRealTimeOsa(uint32_t uiPortIdx, COcmOsa &rstData) override;

    virtual bool SetWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan) override;
    virtual bool SetOcmWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan) override;
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;
    virtual bool Commit(uint32_t uiDevIdx) override;
protected:

    int __ReadWrite(uint32_t uiChip, uint8_t *pucBufW, int32_t uiLenW, uint8_t *pucBufR, uint32_t ulLenR);
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;
};







