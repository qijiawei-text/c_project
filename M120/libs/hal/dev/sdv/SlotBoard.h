/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotBoard.h
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

struct CSlotReg
{
    uint32_t uiOnlineReg;
    uint32_t uiOnlineBit;
    uint32_t uiPlugReg;
    uint32_t uiPlugBit;
    uint32_t uiCardTypeReg;
};


class CSlotBoard
{
public:
    CSlotBoard(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrvPtr) : m_stpLogicDrv(c_rstpLogicDrvPtr)
    {
    }

    ~CSlotBoard()
    {
    }

    /* slot */
    virtual bool IsSlotOnline(uint32_t uiIdx);

    virtual bool IsSlotPlug(uint32_t uiIdx);

    virtual bool ClearSlotPlug(uint32_t uiIdx);

    virtual bool GetCardType(uint32_t uiIdx, uint32_t &ruiType);

protected:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    static const std::vector<CSlotReg> msc_staSlotRegs;

};



