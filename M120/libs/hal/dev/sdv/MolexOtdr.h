/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MoldxOtdr.h
   Author:        Zhu Weian
   Date:          2021-02-14
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2021-02-14] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "OtdrDriver.h"
#include "MolexOtdrProto.h"

class CMolexOtdr : public COtdrDriver
{
public:
    CMolexOtdr(uint32_t uiHwIdx,
               uint32_t uiHwSubIdx,
               uint32_t uiResetHwIdx,
               uint32_t uiResetHwSubIdx,
               const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
               const std::shared_ptr<COtdrProto> &c_rstpProtoPtr) :
        COtdrDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx, c_rstpDrvPtr, c_rstpProtoPtr)
    {
        m_stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
    }
    ~CMolexOtdr()
    {
    }

    virtual bool IsOnline(void) override;
    virtual bool GetVol(uint16_t &rusVol) override;
    virtual bool GetAlarm(uint16_t &rusAlarm) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COtdrStatusData &rstData) override;
    virtual bool GetCfg(COtdrCfgData &rstCfgData) override;
    bool SetCfg(COtdrCfgData &rstCfg);
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
    int Switch(uint32_t uiChip);
    int SwitchClose(uint32_t uiChip);
private:
    bool __Scan();
    bool __GetState(bool &rbIdle);
    bool __GetEvent();

    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen, uint32_t uiTimeOut = 50);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg, uint32_t uiTimeOut = 50);
    bool __GetCfg(COtdrCfgData &rstCfg);
    bool __SetCfg(COtdrCfgData &rstCfg);
    std::shared_ptr<CLogicDriver> m_stpLogicDrvPtr = nullptr;
    std::mutex ms_stMutex;
    std::mutex &
    GetMutex(void)
    {
        return ms_stMutex;
    }
};

