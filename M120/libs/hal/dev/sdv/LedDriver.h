/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LedDriver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "BoardCmd.h"

/*************************************************/



#if 0

struct CLedReg
{
    uint32_t uiReg;
    uint32_t uiRegMask;
};

struct CSlotLedReg
{
    std::vector<CLedReg> staLedRegSlot0;
    std::vector<CLedReg> staLedRegSlot1;
    std::vector<CLedReg> staLedRegSlot2;
    std::vector<CLedReg> staLedRegSlotBoard;
};

#endif


class CLed /* : public CDevDriver */
{
public:

    CLed(const std::shared_ptr<CLedBoard> &c_rstpLedBoardPtr, uint32_t uiHwSubIdx) : m_stpLedBoardPtr(c_rstpLedBoardPtr), m_uiHwSubIdx(uiHwSubIdx)
    {
    }

    virtual
    ~CLed()
    {
    }

    virtual bool SetState(uint32_t uiState);
    virtual bool GetState(uint32_t &ruiState);
    virtual bool SetTestMode(bool bTestMode);
    virtual bool SetLocationTestMode(bool bTestMode);
private:

    std::shared_ptr<CLedBoard> m_stpLedBoardPtr = nullptr;
    uint32_t m_uiHwSubIdx = 0;

};

#if 0
class CLedCpldDriver : public CLed
{
public:
    CLedCpldDriver(uint32_t uiCpldIdx, uint32_t uiIdx) : CLed(uiCpldIdx, uiIdx)
    {
        m_stpLogicDrvPtr = CBoardCmd::GetCpldDrvPtr(uiCpldIdx);
    }
    ~CLedCpldDriver()
    {
    }

    /* virtual bool SetState(uint32_t uiState) override; */

#if 0
private:
    std::shared_ptr<CLogicDriver> m_stpCpldDrvPtr;
#endif

};




class CLedFpgaDriver : public CLed
{
public:
    CLedFpgaDriver(uint32_t uiFpgaIdx, uint32_t uiIdx) : CLed(uiFpgaIdx, uiIdx)
    {
        m_stpLogicDrvPtr = CBoardCmd::GetCpldDrvPtr(uiFpgaIdx);
    }
    ~CLedFpgaDriver()
    {
    }

    /* virtual bool SetState(uint32_t uiState) override; */

#if 0
private:
    std::shared_ptr<CLogicDriver> m_stpFpgaDrvPtr;
#endif
};

#endif










