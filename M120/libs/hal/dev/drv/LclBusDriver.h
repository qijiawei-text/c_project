/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LclBusDriver.h
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

#define LCL_BUS_CS_NUM          4

struct CLclBusRsc
{
    void *pvStartAddr;
    uint32_t uiSize;
};

class CLclBusDriver
{
public:
    CLclBusDriver()
    {
    }
    ~CLclBusDriver()
    {
    }

    bool CreateVirtualAddr(uint32_t uiCsIdx);
    void *
    GetAddr(void)
    {
        return m_pvAddr;
    }

    uint8_t
    Read(uint32_t uiAddr)
    {
#if 0
        if (uiAddr == 40) {
            LOG_WRITE(EN_LOG_NOTICE, "Read uiAddr = %d", uiAddr);
        }
#else
        /* LOG_WRITE(EN_LOG_NOTICE, "Read uiAddr = %d", uiAddr); */
#endif
        volatile uint8_t *pucData = (volatile uint8_t *)m_pvAddr;
        return *(pucData + uiAddr);
    }

    void
    Write(uint32_t uiAddr, uint8_t ucData)
    {
#if 0
        if (uiAddr == 40) {
            LOG_WRITE(EN_LOG_NOTICE, "Write uiAddr = %d", uiAddr);
        }
#else
        /* LOG_WRITE(EN_LOG_NOTICE, "Write uiAddr = %d, ucData = %d", uiAddr, ucData); */
#endif
        volatile uint8_t *pucData = (volatile uint8_t *)m_pvAddr;
        *(pucData + uiAddr) = ucData;
    }


private:
    int32_t m_iFd = -1;
    void *m_pvAddr = MAP_FAILED;

    const std::array<CLclBusRsc, LCL_BUS_CS_NUM> mc_staBassAddr
    {{
         {reinterpret_cast<void *>(0xfed50000), 0x20000 + 1},
         {reinterpret_cast<void *>(0xfed50000), 0x20000 + 1},
         {reinterpret_cast<void *>(0xFFF800000), 0x10000},
         {reinterpret_cast<void *>(0xFFF800000), 0x10000},
     }};
};








