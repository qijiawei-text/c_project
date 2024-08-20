/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      McsDriver.h
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
#include "BoardCmd.h"

/*************************************************/

class CMcsDriver : public CDevDriver
{
public:
    CMcsDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CMcsDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~CMcsDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CMcsStatusData &rstData) = 0;
    virtual bool SetPortSwPos(uint32_t uiPortIdx, uint32_t uiPos) = 0;
};

class CMcsFpgaDriver : public CMcsDriver
{
public:
    CMcsFpgaDriver(uint32_t uiFpgaIdx, uint32_t uiIdx) : CMcsDriver(uiFpgaIdx, uiIdx)
    {
        /* TBD: Fpga Driver */
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiFpgaIdx);
    }
    ~CMcsFpgaDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CMcsStatusData &rstData) override;
    virtual bool SetPortSwPos(uint32_t uiPortIdx, uint32_t uiPos) override;

private:
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;

};






