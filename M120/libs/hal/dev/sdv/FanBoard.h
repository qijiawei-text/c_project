/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      FanBoard.h
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

struct CFanReg
{
    uint32_t uiOnlineReg;
    uint32_t uiOnlineBit;
    uint32_t uiPlugReg;
    uint32_t uiPlugBit;
#if 0
    uint32_t uiSetSpeedReg;
    uint32_t uiGetSpeedReg;
    uint32_t uiSetLedReg;
    uint32_t uiGetLedReg;
#endif
};


class CFanBoard
{
public:
    CFanBoard(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) :
        m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CFanBoard()
    {
    }

    virtual bool IsOnline(uint32_t uiIdx);
    virtual bool IsPlug(uint32_t uiIdx);
    virtual bool ClearPlug(uint32_t uiIdx);
#if 0
    virtual bool GetFanSpeed(uint32_t uiIdx, uint32_t &uiSpeed);
    virtual bool SetFanSpeed(uint32_t uiIdx, uint32_t uiSpeed);

    virtual bool SetFanLedState(uint32_t uiIdx, uint32_t uiState);

    virtual bool GetFanLedState(uint32_t uiIdx, uint32_t &ruiState);
#endif
protected:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    const static std::vector<CFanReg> msc_staFanRegs;


};



