/*****************************************************************************************************
   FileName:      EEprom.h
   Date:          2019-07-22
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-22] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "I2cDriver.h"
#include "SpiDriver.h"

#define DEF_EE_128KB_SIZE               (128 * DEF_1KB)
#define DEF_EE_128KB_PAGE_LEN           (256u)
/* #define DEF_128KB_PAGE_LEN         (128u) */

class CEEprom /*: public CDevDriver*/
{
public:
    /* CEEprom(uint32_t uiHwIdx, uint32_t uiHwSubIdx, const std::shared_ptr<CI2c> &c_rstpDrvPtr) :
        CDevDriver(uiHwIdx, uiHwSubIdx), m_stpDrvPtr(c_rstpDrvPtr)
       {
       } */

    CEEprom()
    {
    }

    virtual
    ~CEEprom()
    {
    }

    virtual bool Read(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool Write(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) = 0;
#if 0
protected:

    virtual bool
    __Read(uint32_t uiAddrW, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t uiLenR)
    {
        return false;
    }

    virtual bool
    __Write(uint32_t uiAddrW, uint8_t *pucBufW, uint32_t uiLenW)
    {
        return false;
    }

    virtual uint32_t
    __PageLenR(uint32_t uiLenR, uint32_t uiAddrR)
    {
        return uiLenR;
    }

    virtual uint32_t
    __PageLenW(uint32_t uiLenW, uint32_t uiAddrW)
    {
        return uiLenW;
    }

private:
    std::shared_ptr<CEEpromProto> m_stpProtoPtr = nullptr;
#endif

};

class CI2cEEprom : public CEEprom
{
public:

    CI2cEEprom(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr) :
        m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
    }

    virtual
    ~CI2cEEprom()
    {
    }

    virtual bool Read(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool Write(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) override;


private:

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;
};


class CSpiEEprom : public CEEprom
{
public:

    CSpiEEprom(const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, uint32_t uiChipIdx)
        : m_stpDrvPtr(c_rstpDrvPtr), m_uiChipIdx(uiChipIdx)
    {
    }

    virtual
    ~CSpiEEprom()
    {
    }

    virtual bool Read(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool Write(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen) override;


private:

    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    uint32_t m_uiChipIdx;
};




