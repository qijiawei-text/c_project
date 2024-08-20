/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrDriver.h
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
#include "UartDriver.h"
#include "DevDriver.h"
#include "BoardCmd.h"

#include "OtdrProto.h"

/*************************************************/

class COtdrDriver  : public CDevDriver
{
public:
    COtdrDriver()
    {
    }

    COtdrDriver(uint32_t uiHwIdx,
                uint32_t uiHwSubIdx,
                uint32_t uiResetHwIdx,
                uint32_t uiResetHwSubIdx,
                const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
                const std::shared_ptr<COtdrProto> &c_rstpProtoPtr) :
        CDevDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx),
        m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        m_stpResetBoard = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }

    virtual
    ~COtdrDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool
    GetVol(uint16_t &rusVol)
    {
        return false;
    }
    virtual bool
    GetAlarm(uint16_t &rusAlarm)
    {
        return false;
    }
    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(COtdrStatusData &rstData) = 0;
    virtual bool GetCfg(COtdrCfgData &rstCfgData) = 0;
    virtual bool GetEvent(COtdrStatusData &rstData) = 0;
    virtual bool GetSor() = 0;

    virtual bool StartScan(void) = 0;
    virtual bool SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution) = 0;
    virtual bool SetUserIOR(double dUserIOR) = 0;
    virtual bool SetMeasuringTime(uint32_t ulMeasuringTime) = 0;
    virtual bool SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr) = 0;
    virtual bool SetScanMode(uint32_t ulScanMode) = 0;
    virtual bool GetTemp(COtdrTemp &rstTemp) = 0;

    virtual bool Reset(uint8_t ucResetType) = 0;
    virtual bool Upgrade(const char *pcFilePath) = 0;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool SetStartOffset(COtdrStartOffset *pstStartOffset) = 0;
    virtual bool GetStartOffset(COtdrStartOffset &rstStartOffset) = 0;

protected:

    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<COtdrProto> m_stpProtoPtr = nullptr;

    std::shared_ptr<CResetBoard> m_stpResetBoard = nullptr;

};

class COtdrUartDriver : public COtdrDriver
{
public:
    COtdrUartDriver(uint32_t uiHwIdx,
                    uint32_t uiHwSubIdx,
                    uint32_t uiResetHwIdx,
                    uint32_t uiResetHwSubIdx,
                    const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
                    const std::shared_ptr<COtdrProto> &c_rstpProtoPtr) :
        COtdrDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx, c_rstpDrvPtr, c_rstpProtoPtr)
    {

    }
    ~COtdrUartDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COtdrStatusData &rstData) override;
    virtual bool GetCfg(COtdrCfgData &rstCfgData) override;
    virtual bool GetEvent(COtdrStatusData &rstData) override;
    virtual bool GetSor() override;

    virtual bool StartScan(void) override;
    virtual bool SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution) override;
    virtual bool SetUserIOR(double dUserIOR) override;
    virtual bool SetMeasuringTime(uint32_t ulMeasuringTime) override;
    virtual bool SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr) override;
    virtual bool SetScanMode(uint32_t ulScanMode) override;
    virtual bool GetTemp(COtdrTemp &rstTemp) override;

    virtual bool Reset(uint8_t ucResetType) override;
    virtual bool Upgrade(const char *pcFilePath) override;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool SetStartOffset(COtdrStartOffset *pstStartOffset) override;
    virtual bool GetStartOffset(COtdrStartOffset &rstStartOffset) override;

private:

    bool __Scan();
    bool __GetState(bool &rbIdle);
    bool __GetEventTop(COtdrEventTop &rstEventTop);
    bool __GetEventLine(uint32_t ulEventId, COtdrEventLine &rstEventLine);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg, uint32_t uiTimeout = 50);

};







