/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetBoard.h
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

struct CResetReg
{
    uint32_t uiReg;
    uint32_t uiBit;

};


class CResetBoard
{
public:
    CResetBoard(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CResetBoard()
    {
    }

    virtual bool Reset(uint32_t uiModIndex);

protected:

    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    static const std::vector<CResetReg> msc_staResetRegs;

};
