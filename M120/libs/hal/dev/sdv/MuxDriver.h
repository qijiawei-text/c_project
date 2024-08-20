/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MuxDriver.h
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
#include "SpiDriver.h"
#include "EEprom.h"

/*************************************************/

class CMux : public CDevDriver
{
public:
    CMux(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    virtual
    ~CMux()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CMuxStatusData &rstData) = 0;
    virtual bool GetCali(CMuxCaliData &rstData) = 0;

};

class CMuxSpiDriver : public CMux
{
public:

    CMuxSpiDriver(uint32_t uiHwIdx,
                  const std::shared_ptr<CInterfaceDriver> &c_stpDrvPtr,
                  const std::shared_ptr<CEEprom> &c_stpEEpromPtr) :
        CMux(uiHwIdx), m_stpDrvPtr(c_stpDrvPtr), m_stpEEpromPtr(c_stpEEpromPtr)
    {

    }
    ~CMuxSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CMuxStatusData &rstData) override;
    virtual bool GetCali(CMuxCaliData &rstData) override;

private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CEEprom> m_stpEEpromPtr = nullptr;

};







