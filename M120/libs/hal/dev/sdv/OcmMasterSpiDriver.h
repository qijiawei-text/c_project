/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmMasterSpiDriver.h
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:


*****************************************************************************************************/

#pragma once

#include "OcmDriver.h"
#include "OcmMasterSpiProto.h"


class COcmMasterSpiDriver : public COcmDriver
{
public:
    COcmMasterSpiDriver(uint32_t uiHwIdx,
                        uint32_t uiHwSubIdx,
                        uint32_t uiResetHwIdx,
                        uint32_t uiResetHwSubIdx,
                        const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr) :
        COcmDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx),
        m_stpDrvPtr(c_rstpDrvPtr)
    {
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
        m_stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        /*Switch(uiHwSubIdx); */
        usleep(10 * 1000);
    }

    virtual
    ~COcmMasterSpiDriver()
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
    bool GetTemp(uint32_t uiPortIdx, int16_t &rsData);
protected:

    int __ReadWrite(uint32_t uiChip, uint8_t *pucBufW, int32_t uiLenW, uint8_t *pucBufR, uint32_t ulLenR);
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;
    std::shared_ptr<CLogicDriver> m_stpLogicDrvPtr = nullptr;

private:
    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen, uint32_t uiTimeOut = 50);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg, uint32_t uiTimeOut = 50);
    int Switch(uint32_t uiChip);
    static std::mutex ms_stMutex;
    std::mutex &
    GetMutex(void)
    {
        return ms_stMutex;
    }
};







