/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      PcieBusDriver.h
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
#include <sys/mman.h>

/*************************************************/


struct CPcieBusRsc
{
    void *pvStartAddr;
    uint32_t uiSize;
};

class CPcieBusDriver
{
public:
    CPcieBusDriver()
    {
    }
    ~CPcieBusDriver()
    {
    }

    bool CreateVirtualAddr(uint32_t uiCsIdx);
    bool DestroyVirtualAddr(uint32_t uiCsIdx);
/*    void *
    GetAddr(void)
    {
    }*/

    uint8_t Read(uint32_t uiAddr);

    void Write(uint32_t uiAddr, uint8_t ucData);

    bool Read(uint32_t uiAddr, uint32_t uiLen, uint8_t *pucBuf);

    bool Write(uint32_t uiAddr, uint32_t uiLen, uint8_t *pucBuf);
private:
    int32_t m_iFd = -1;
    pthread_mutex_t m_stMutexLock;

};








