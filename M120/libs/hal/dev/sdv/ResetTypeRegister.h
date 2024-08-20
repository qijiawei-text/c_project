/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetTypeRegister.h
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

class CResetTypeRegister
{
public:
    CResetTypeRegister(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CResetTypeRegister()
    {
    }

    virtual bool SetResetType(uint32_t uiType);
    virtual bool GetResetType(uint8_t &rucType);

protected:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;

};

