/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LclBusDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "LclBusDriver.h"
#include <fcntl.h>

/*==============================function=========================*/

bool
CLclBusDriver::CreateVirtualAddr(uint32_t uiCsIdx)
{

    if (uiCsIdx >= LCL_BUS_CS_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "uiCsIdx >= LCL_BUS_CS_NUM, uiCsIdx = %d", uiCsIdx);
        return false;
    }

    if (m_iFd > 0) {
        return true;
    }

    m_iFd = open("/dev/mem", O_RDWR | O_SYNC);

    if (m_iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "open /dev/mem error.");
        return false;
    }

    m_pvAddr = mmap64(nullptr, mc_staBassAddr.at(uiCsIdx).uiSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_iFd, reinterpret_cast<__off64_t>(mc_staBassAddr.at(uiCsIdx).pvStartAddr));

    if (MAP_FAILED == m_pvAddr) {
        return false;
    }

    return true;

}


