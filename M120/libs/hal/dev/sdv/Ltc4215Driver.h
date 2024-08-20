/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Ltc4215Driver.h
   Author:        Qing Yan
   Date:
   Version:       1.0
   Description:
   Function List:

   History:
   [Qing Yan] [2022-04-11] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevDriver.h"

#include "I2cDriver.h"

#define LTC4215_CTRL_REG         0x00
#define LTC4215_SENSE_REG        0x04
#define LTC4215_SOURCE_REG       0x05
#define LTC4215_ADIN_REG         0x06



class CLtc4215
{
public:

    CLtc4215(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }

    virtual
    ~CLtc4215()
    {
    }

    bool GetSenseCode(int32_t &riSenseCode);
    bool GetSourceCode(int32_t &riSourceCode);

private:
    bool m_bInit = false;

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;

    uint8_t m_ucRegCtrl = LTC4215_CTRL_REG;
    uint8_t m_ucRegSense = LTC4215_SENSE_REG;
    uint8_t m_ucRegSource = LTC4215_SOURCE_REG;
    uint8_t m_ucRegAdin = LTC4215_ADIN_REG;


};



