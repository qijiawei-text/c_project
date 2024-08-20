/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendDriver.h
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
#include "UartDriver.h"

#include "ExtendProto.h"

/*************************************************/

class CExtendDriver : public CDevDriver
{
public:
    CExtendDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CExtendDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    CExtendDriver(uint32_t uiHwIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }
    virtual
    ~CExtendDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CExtendStatusData &rstData) = 0;

    virtual bool SetSwitchPos(uint32_t ulSubIdx, uint32_t ulPos) = 0;
    virtual bool SetSfp(uint32_t ulSubIdx, uint32_t ulVal) = 0;
    virtual bool SetVoaAttn(uint32_t ulSubIdx, uint32_t ulVal) = 0;
    virtual bool SetVoaMode(uint32_t ulSubIdx, uint32_t ulMode) = 0;

    virtual bool Reset(uint8_t ucResetType) = 0;
    virtual bool Upgrade(const char *pcFilePath) = 0;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;

protected:
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;
};

typedef struct
{
    int16_t sAlarm;
    int16_t asPdPower[12];
    int16_t asVoaMode[4];
    int16_t asVoaSet[4];
    int16_t asVoaAct[4];
    int16_t asVoaStatus[4];
    int16_t asSwSet[1];
    int16_t asSwAct[1];
    int16_t asReserve[32 - 31];                /*Total 64 bytes*/
} EXTEND_ALLINFO_ST;

typedef struct
{
    int16_t sHead;                          /*Frame head=0xFEFE */
    int16_t sStVer;                         /*struct version=0x0001 */
    EXTEND_ALLINFO_ST stInfo;
    int16_t asReserve[128 - 35];              /* for word Data alignment */
    int16_t sCheckSum;                      /*check sum */
} EXTEND_GETALLINFO_ST;


class CExtendUartDriver : public CExtendDriver
{
public:
    CExtendUartDriver(uint32_t uiUartIdx) : CExtendDriver(uiUartIdx)
    {

    }
    CExtendUartDriver(uint32_t uiHwIdx,
                      uint32_t uiResetHwIdx,
                      uint32_t uiResetHwSubIdx,
                      const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
                      const std::shared_ptr<CExtendSwProto> &c_rstpProtoPtr) :
        CExtendDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx), m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }
    ~CExtendUartDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CExtendStatusData &rstData) override;

    virtual bool SetSwitchPos(uint32_t ulSubIdx, uint32_t ulPos) override;
    virtual bool SetSfp(uint32_t ulSubIdx, uint32_t ulVal) override;
    virtual bool SetVoaAttn(uint32_t ulSubIdx, uint32_t ulVal) override;
    virtual bool SetVoaMode(uint32_t ulSubIdx, uint32_t ulMode) override;

    virtual bool Reset(uint8_t ucResetType) override;
    virtual bool Upgrade(const char *pcFilePath) override;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;

private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CExtendSwProto> m_stpProtoPtr = nullptr;

    std::shared_ptr<CRspHandler> __RspHandler();
    bool __SendCmd(std::string &rstaMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string &rstaMsg);
    bool __UpgradeEnd();


};







