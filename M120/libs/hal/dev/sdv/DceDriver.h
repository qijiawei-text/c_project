/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DceDriver.h
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

/*************************************************/

class CDceDriver : public CDevDriver
{
public:
    CDceDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CDceDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~CDceDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CDceStatusData &rstData) = 0;
    virtual bool GetCfgData(CDceCfgData &rstData) = 0;

protected:

};

class CDceUartDriver : public CDceDriver
{
public:
    CDceUartDriver(uint32_t uiUartIdx) : CDceDriver(uiUartIdx)
    {
    }
    ~CDceUartDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CDceStatusData &rstData) override;
    virtual bool GetCfgData(CDceCfgData &rstData) override;

};










