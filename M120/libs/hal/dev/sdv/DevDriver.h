/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevDriver.h
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

/*************************************************/

class CDevDriver
{
public:
    CDevDriver()
    {
    }

    CDevDriver(uint32_t uiHwIdx) : m_uiHwIdx(uiHwIdx)
    {
    }

    CDevDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) :
        m_uiHwIdx(uiHwIdx), m_uiHwSubIdx(uiHwSubIdx)
    {
    }

    CDevDriver(uint32_t uiHwIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        m_uiHwIdx(uiHwIdx), m_uiResetHwIdx(uiResetHwIdx), m_uiResetHwSubIdx(uiResetHwSubIdx)
    {
    }

    CDevDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        m_uiHwIdx(uiHwIdx), m_uiHwSubIdx(uiHwSubIdx), m_uiResetHwIdx(uiResetHwIdx), m_uiResetHwSubIdx(uiResetHwSubIdx)
    {
    }


    /* CDevDriver(uint32_t uiAddr) : m_uiAddr(uiAddr)
       {
       } */


    virtual
    ~CDevDriver()
    {
    }

    bool
    DbgSetCommFail(uint32_t uiSubIdx = 0)
    {
        std::cout << "DbgSetCommFail" << std::endl;
        m_bCommFailDebug = true;
        return true;
    }

    bool
    DbgClearCommFail(uint32_t uiSubIdx = 0)
    {

        std::cout << "DbgClearCommFail" << std::endl;
        m_bCommFailDebug = false;
        return true;
    }

    bool
    DbgSetModuleAlarm(uint32_t uiSubIdx = 0)
    {
        std::cout << "DbgSetModuleAlarm !" << std::endl;
        m_bModuleAlarmDebug = true;
        return true;
    }

    bool
    DbgClearModuleAlarm(uint32_t uiSubIdx = 0)
    {

        std::cout << "DbgClearModuleAlarm" << std::endl;
        m_bModuleAlarmDebug = false;
        return true;
    }

protected:
    /* uint32_t m_uiAddr; */
    uint32_t m_uiHwIdx;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiResetHwIdx;
    uint32_t m_uiResetHwSubIdx;

    bool m_bCommFailDebug = false;
    bool m_bModuleAlarmDebug = false;
};








