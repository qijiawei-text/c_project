/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Apsd.h
   Author:        Zhu Weian
   Date:          2020-10-06
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-06] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"

struct CApsdReg
{
    uint32_t uiReg;
    uint32_t uiEnableBit;
    uint32_t uiPumpBit;
};


class CApsd
{
public:
    CApsd(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CApsd()
    {
    }

/* apsd */
    virtual bool SetApsdEnable(uint32_t uiIdx, bool bEnable);
    virtual bool GetApsdStatus(uint32_t uiIdx, bool &rbEnable, bool &rbStatus, bool &rbEvent);

protected:

    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    static const std::vector<CApsdReg> msc_staApsdRegs;

private:
    /* apsd */
    virtual bool __SetApsdEnable(uint32_t uiReg, uint32_t uiEnableBit, uint32_t uiPumpBit, bool bEnable);
    virtual bool __GetApsdEnable(uint32_t uiReg, uint32_t uiBit, bool &rbEnable);
    virtual bool __GetApsdStatus(uint32_t uiReg, uint32_t uiBit, bool &rbStatus);
    virtual bool __GetApsdAlarm(uint32_t uiReg, uint32_t uiBit, bool &rbAlarm);
};
