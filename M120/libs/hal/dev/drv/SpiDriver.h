/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SpiDriver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]
   [Zhu Weian] [2020-11-12] [2.0] [Modify]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "InterfaceDriver.h"
#include "LogicDriver.h"
#include "Ft4222Driver.h"


/*************************************************/

class CSpi : public CInterfaceDriver
{
public:
    CSpi()
    {
    }

    virtual
    ~CSpi()
    {
    }

#if 0

    virtual bool
    Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
    {
        return false;
    }

    virtual bool
    Read(uint32_t uiIdx, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler)
    {
        return false;
    }


    virtual bool
    ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
              uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
    {
        return false;
    }
#endif
};

class CLogicSlaveSpi : public CSpi
{
public:
    CLogicSlaveSpi(const std::shared_ptr<CLogicDriver> &c_rstpDrvPtr) : m_stpDrvPtr(c_rstpDrvPtr)
    {
    }
    ~CLogicSlaveSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    std::shared_ptr<CLogicDriver> m_stpDrvPtr = nullptr;
    bool __Ready(uint32_t uiChip);

    static std::mutex ms_stMutex;

};

class CLogicMasterSpi : public CSpi
{
public:
    CLogicMasterSpi(const std::shared_ptr<CLogicDriver> &c_rstpDrvPtr) : m_stpDrvPtr(c_rstpDrvPtr)
    {
    }
    ~CLogicMasterSpi()
    {
    }

    /* virtual bool Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen) override; */
    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

    virtual bool Write(uint32_t uiIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool Read(uint32_t uiIdx, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler) override;

    bool __ReadRaw(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __WriteRaw(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen);
private:
    bool __Read(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen);
    bool __SetDirR(uint32_t uiChip);
    bool __SetDirW(uint32_t uiChip);
    bool __SetAddress(uint32_t uiChip);
    bool __SetLen(uint32_t uiChip, uint32_t uiLen);
    bool __ReadLen(uint32_t uiChip, uint32_t &ruiLen);
    bool __WriteData(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen);
    bool __StartSend(uint32_t uiChip);
    bool __WaitSendDone(uint32_t uiChip);
    bool __StopSend(uint32_t uiChip);
    bool __CheckStatus(uint32_t uiChip);
    bool __ReadData(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen);

    uint8_t m_uiCtrlValue = 0x00;

    std::shared_ptr<CLogicDriver> m_stpDrvPtr = nullptr;
    static std::mutex ms_stMutex;
    std::mutex &
    GetMutex(void)
    {
        return ms_stMutex;
    }
};


class CFtSpi : public CSpi
{
public:
    CFtSpi()
    {

    }

    ~CFtSpi()
    {
    }

    virtual bool Read(uint32_t uiChip, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler) override;
    virtual bool Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen) override;

    virtual bool ReadWrite(uint32_t ulChip,
                           uint8_t *pucBufW,
                           uint32_t uiLenW,
                           uint8_t *pucBufR,
                           std::shared_ptr<CRspHandler> stpRspHandler) override;


private:
    CSpiFt4222Driver m_stpSpiFt;
};

class CLogicMasterWssSpi : public CLogicMasterSpi
{
public:
    CLogicMasterWssSpi(const std::shared_ptr<CLogicDriver> &c_rstpDrvPtr) : CLogicMasterSpi(c_rstpDrvPtr)
    {
        m_stpDrvPtr = c_rstpDrvPtr;
    }

    ~CLogicMasterWssSpi()
    {
    }

    virtual bool Read(uint32_t uiChip, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler) override;
    virtual bool Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen) override;

    virtual bool ReadWrite(uint32_t ulChip,
                           uint8_t *pucBufW,
                           uint32_t uiLenW,
                           uint8_t *pucBufR,
                           std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    std::shared_ptr<CLogicDriver> m_stpDrvPtr = nullptr;
    static std::mutex ms_stMutex;
};


