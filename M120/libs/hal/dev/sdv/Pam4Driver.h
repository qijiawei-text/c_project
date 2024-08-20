/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Driver.h
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "DevData.h"
#include "Pam4Proto.h"
#include "I2cDriver.h"

/*************************************************/

class CPam4Driver : public CDevDriver
{
public:
    CPam4Driver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CPam4Driver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~CPam4Driver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<CPam4SwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CPam4StatusData &rstData) = 0;
    virtual bool GetCfgData(CPam4CfgData &rstData) = 0;

protected:
    std::shared_ptr<CPam4SwProto> m_stpProtoPtr = nullptr;

};


class CPam4I2cDriver : public CPam4Driver
{
public:
    CPam4I2cDriver(uint32_t uiI2cIdx) : CPam4Driver(uiI2cIdx)
    {
    }
    CPam4I2cDriver(uint32_t uiI2cIdx, uint32_t uiAddr) : CPam4Driver(uiI2cIdx, uiAddr)
    {
    }
    ~CPam4I2cDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CPam4StatusData &rstData) override;
    virtual bool GetCfgData(CPam4CfgData &rstData) override;

private:
    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;


};








