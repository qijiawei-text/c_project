/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      PcaDriver.h
   Author:        Zhu Weian
   Date:          2020-05-18
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-05-18] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "I2cDriver.h"
#include "I2cSwitch.h"

/*************************************************/

class CSwitchableI2c : public CI2c
{
public:
    CSwitchableI2c(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiI2cSubIdx)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiI2cSubIdx(uiI2cSubIdx)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }

    ~CSwitchableI2c()
    {
    }

    uint32_t
    PageLenR(uint32_t uiLen, uint32_t uiAddr)
    {
        return m_stpI2cPtr->PageLenR(uiLen, uiAddr);
    }

    uint32_t
    PageLenW(uint32_t uiLen, uint32_t uiAddr)
    {
        return m_stpI2cPtr->PageLenW(uiLen, uiAddr);
    }

    virtual bool Read(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) override;
    virtual bool Write(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) override;

protected:

    std::mutex m_staMutex;
    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiI2cSubIdx;

    virtual bool __Switch(uint32_t uiChIdx) = 0;
    virtual bool
    __SwitchClose()
    {
        return true;
    }

};

class CPcaI2c : public CSwitchableI2c
{
public:
    CPcaI2c(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr) :
        CSwitchableI2c(c_rstpI2cPtr, uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        LOG_WRITE(EN_LOG_NOTICE, "uiHwSubIdx = %d", uiHwSubIdx);
    }
    ~CPcaI2c()
    {
    }

private:
    uint32_t m_uiDevAddr;

    virtual bool __Switch(uint32_t uiChIdx) override;
    virtual bool __SwitchClose() override;
};

class CLogicSwitchI2c : public CSwitchableI2c
{
public:
    CLogicSwitchI2c(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiI2cSubIdx, uint32_t uiDevAddr, const std::shared_ptr<CI2cSwitch> &c_rstpI2cSwitch, uint32_t uiLogicSubIdx)
        : CSwitchableI2c(c_rstpI2cPtr, uiI2cSubIdx), m_uiDevAddr(uiDevAddr), m_stpI2cSwitch(c_rstpI2cSwitch), m_uiLogicSubIdx(uiLogicSubIdx)
    {
        /* m_stpLogicPtr = CBoardCmd::GetLogicDrvPtr(LOGIC1); */
    }
    ~CLogicSwitchI2c()
    {

    }

private:
    uint32_t m_uiDevAddr;
    std::shared_ptr<CI2cSwitch> m_stpI2cSwitch = nullptr;
    uint32_t m_uiLogicSubIdx;
    virtual bool __Switch(uint32_t uiChIdx) override;
    virtual bool __SwitchClose() override;
    static const std::vector<uint32_t> msc_staValue;
};


