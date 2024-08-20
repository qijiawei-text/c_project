/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      InterfaceDriver.h
   Author:        Zhen Zhang
   Date:          2019-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-01-04] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "RspHandler.h"

/*************************************************/

class CInterfaceDriver
{
public:
    CInterfaceDriver()
    {
    }
    virtual
    ~CInterfaceDriver()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW, uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos) = 0;


    virtual bool
    Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
    {
        return false;
    }
    virtual bool
    Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos)
    {
        return false;
    }

    virtual bool
    Lock()
    {
        return true;
    }

    virtual void
    Unlock()
    {
        return;
    }

    virtual bool
    Switch(uint32_t uiIdx)
    {
        return true;
    }

    virtual bool
    ReadWriteUnlock(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW, uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos)
    {
        return true;
    }

    virtual bool
    WriteUnlock(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
    {
        return true;
    }

private:

};








