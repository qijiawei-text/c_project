/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicSpi.h
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
#include "SpiDriver.h"

class CLogicSpi : public CSpi
{
public:
    CLogicSpi(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    virtual
    ~CLogicSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiChipIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpEndPos) override;

private:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    std::mutex ms_stMutex;

    std::mutex &
    GetMutex(void)
    {
        return ms_stMutex;
    }
};

