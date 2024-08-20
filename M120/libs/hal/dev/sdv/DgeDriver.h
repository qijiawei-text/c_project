/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeDriver.h
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

#include "DgeProto.h"

/*************************************************/

class CDgeDriver : public CDevDriver
{
public:
    CDgeDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CDgeDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~CDgeDriver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<CDgeSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CDgeStatusData &rstData) = 0;
    virtual bool SetCurve(uint32_t uiPointCnt, CDgePoint *pstPoint) = 0;

protected:
    std::shared_ptr<CDgeSwProto> m_stpProtoPtr = nullptr;

};

class CDgeFpgaDriver : public CDgeDriver
{
public:
    CDgeFpgaDriver(uint32_t uiFpgaIdx, uint32_t uiIdx) : CDgeDriver(uiFpgaIdx, uiIdx)
    {
        /* TBD: Logic Driver */
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiFpgaIdx);
    }
    ~CDgeFpgaDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CDgeStatusData &rstData) override;
    virtual bool SetCurve(uint32_t uiPointCnt, CDgePoint *pstPoint) override;

private:
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;

};







