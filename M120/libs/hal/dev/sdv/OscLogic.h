/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicOsc.h
   Author:        Zhu Weian
   Date:          2020-08-21
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-08-21] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"

struct CSfpRegDef
{
    uint32_t uiEnableBit;
    uint32_t uiOnlineBit;
    uint32_t uiRxLosBit;
    uint32_t uiLinkUpBit;
};

class COscLogic
{
public:
    COscLogic(const std::shared_ptr<CLogicDriver> &c_rstpLogicPtr, uint32_t uiSfpReg)
        : m_stpLogicPtr(c_rstpLogicPtr), m_uiSfpReg(uiSfpReg)
    {

    }

    ~COscLogic()
    {

    }

    virtual bool IsLinkup(uint32_t uiIdx);
    virtual bool IsOnline(uint32_t uiIdx);
    virtual bool IsRxLos(uint32_t uiIdx);
    virtual bool IsEnable(uint32_t uiIdx);
    virtual bool SetEnable(uint32_t uiIdx, bool bEnable);


private:
    std::shared_ptr<CLogicDriver> m_stpLogicPtr = nullptr;

    uint32_t m_uiSfpReg;
    uint32_t m_uiApsdReg = 0x8034;
    static const std::vector<CSfpRegDef> msc_staSfpRegDef;

    /* osc */
    virtual bool __IsEnable(uint32_t uiReg, uint32_t uiBit);
    virtual bool __IsOnline(uint32_t uiReg, uint32_t uiBit);
    virtual bool __IsRxLos(uint32_t uiReg, uint32_t uiBit);
    virtual bool __IsLinkup(uint32_t uiReg, uint32_t uiBit);
    virtual bool __SetEnable(uint32_t uiReg, uint32_t uiBit, bool bEnable);

};

