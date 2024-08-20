/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TempDriver.h
   Author:        Zhu Weian
   Date:          2019-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-01] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevDriver.h"

#include "I2cDriver.h"

#define DEF_DS1721_REG_TEMP             0xAA
#define DEF_DS1721_REG_CONV             0x51


class CTemp /*: public CDevDriver*/
{
public:
    CTemp()
    {
    }
    virtual
    ~CTemp()
    {
    }

    virtual bool GetTemp(int32_t &riTemp) = 0;
};

class CI2cTemp : public CTemp
{
public:

    CI2cTemp(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }

    virtual
    ~CI2cTemp()
    {
    }

    virtual bool GetTemp(int32_t &riTemp) override;

private:
    bool m_bInit = false;

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;

    uint8_t m_ucRegTemp = DEF_DS1721_REG_TEMP;
    uint8_t m_ucRegConv = DEF_DS1721_REG_CONV;


    bool InitTemp();
};



