/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetPwd.h
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

class CResetPwd
{
public:
    CResetPwd()
    {
    }

    ~CResetPwd()
    {
    }

    virtual bool IsNeed();

protected:

    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
};


