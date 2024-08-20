/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      PsuBoard.h
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

struct CPsuReg
{
    uint32_t uiPlugReg;
    uint32_t uiPlugBit;
    uint32_t uiOnlineReg;
    uint32_t uiOnlineBit;
    uint32_t uiOkReg;
    uint32_t uiOkBit;
};


class CPsuBoard
{
public:
    CPsuBoard(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CPsuBoard()
    {
    }

    /* power */
    virtual bool IsOnline(uint32_t uiIdx);

    virtual bool IsOk(uint32_t uiIdx);

    virtual bool IsPlug(uint32_t uiIdx);

    virtual bool ClearPlug(uint32_t uiIdx);


private:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    static const std::vector<CPsuReg> msc_staPsuRegs;
};


