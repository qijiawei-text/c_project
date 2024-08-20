/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LedBoard.h
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"

struct CLedReg
{
    uint32_t uiReg;
    uint32_t uiRegMask;
};


class CLedBoard
{
public:
    CLedBoard(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CLedBoard()
    {
    }

    virtual bool SetLedState(uint32_t uiIdx, uint32_t uiState);

    virtual bool GetLedState(uint32_t uiIdx, uint32_t &ruiState);

    virtual bool SetLedTestMode(uint32_t uiIdx, bool bTestMode);
    virtual bool SetLedLocationTestMode(uint32_t uiIdx, bool bTestMode);

protected:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    static const std::vector<CLedReg> msc_staLedRegs;
    bool m_bReadyFlag = false;
    /* led */
    virtual bool __GetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t &ruiState);
    virtual bool __SetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t uiState);
};


