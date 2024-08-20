/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WatchDog.h
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

class CWatchDog
{
public:
    CWatchDog(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CWatchDog()
    {
    }

    virtual bool Config(bool bEnable, uint8_t ucTimeout);

protected:

    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
};

