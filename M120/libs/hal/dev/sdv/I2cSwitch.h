/*****************************************************************************************************
   FileName:      I2cSwitch.h
   Date:          2020-10-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-08] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"


class CI2cSwitch
{
public:
    CI2cSwitch(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CI2cSwitch()
    {
    }

    bool Switch(uint32_t uiIdx);
    bool Switch(uint32_t uiCardIdx, uint32_t uiIdx);
    bool SwitchClose(uint32_t uiCardIdx);
private:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv;
    const static std::vector<uint8_t> msc_staVal;
};

