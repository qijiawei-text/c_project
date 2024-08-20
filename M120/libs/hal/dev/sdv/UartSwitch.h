/*****************************************************************************************************
   FileName:      UartSwitch.h
   Date:          2020-09-18
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-18] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"


class CUartSwitch
{
public:
    CUartSwitch(uint32_t uiReg, const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv)
        : m_uiReg(uiReg), m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CUartSwitch()
    {
    }

    bool Switch(uint32_t uiIdx);

private:
    uint32_t m_uiReg = 0;
    std::shared_ptr<CLogicDriver> m_stpLogicDrv;

    static const std::vector<uint8_t> msc_staVal;

};



