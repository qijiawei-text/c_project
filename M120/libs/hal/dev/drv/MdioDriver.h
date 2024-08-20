/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MdioDriver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"

/*************************************************/

class CMdio
{
public:
    CMdio()
    {
    }
    ~CMdio()
    {
    }

    virtual bool Read(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData) = 0;
    virtual bool Write(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData) = 0;

protected:

};

class CMdioFpgaDriver : public CMdio
{
public:

    CMdioFpgaDriver(uint32_t uiBaseAddr, const std::shared_ptr<CLogicDriver> &c_rstpDrvPtr)
        : m_uiBaseAddr(uiBaseAddr), m_stpDrvPtr(c_rstpDrvPtr)
    {
    }
    ~CMdioFpgaDriver()
    {
    }

    virtual bool Read(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData) override;
    virtual bool Write(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData) override;

private:

    uint32_t m_uiBaseAddr;
    std::shared_ptr<CLogicDriver> m_stpDrvPtr = nullptr;

    static std::mutex ms_stMutex;

};


