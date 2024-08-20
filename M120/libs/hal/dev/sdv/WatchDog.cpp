/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WatchDog.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "WatchDog.h"


#ifdef __400GZR__

enum EWatchDogReg
{
    WATCHDOG_FEED_REG = 0x12,
    WATCHDOG_CTRL_REG = 0x13,
};


#else


enum EWatchDogReg
{
    WATCHDOG_FEED_REG = 0x44,
    WATCHDOG_CTRL_REG = 0x48,
};

#endif


bool
CWatchDog::Config(bool bEnable, uint8_t ucTimeout)
{
    uint32_t uiCtrlReg = WATCHDOG_CTRL_REG;
    uint32_t uiFeedReg = WATCHDOG_FEED_REG;
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    /* get watchdog status */
    if (!m_stpLogicDrv->ReadReg(uiCtrlReg, &ucVal)) {
        return false;
    }

    if (bEnable) {
        /* disable watchdog */
        ucVal |= 0x40;         /* bit6 diable */
        if (!m_stpLogicDrv->WriteReg(uiCtrlReg, ucVal)) {
            return false;
        }

        /* change timeout */
        ucVal &= 0xc0;           /*change watchdogdata,bit5-bit0*/
        ucVal |= (ucTimeout & 0x3f); /*0 30s,1 60s,2 90s, ...*/
        if (!m_stpLogicDrv->WriteReg(uiCtrlReg, ucVal)) {
            return false;
        }

        /* enable watchdog */
        ucVal &= 0xbf;         /*bit6  1011 1111*/
        if (!m_stpLogicDrv->WriteReg(uiCtrlReg, ucVal)) {
            return false;
        }

        /* feed watchdog */
        if (!m_stpLogicDrv->WriteReg(uiFeedReg, 0x01)) {
            return false;
        }
    } else {
        /* disable watchdog */
        ucVal |= 0x40;         /* bit6 diable */
        if (!m_stpLogicDrv->WriteReg(uiCtrlReg, ucVal)) {
            return false;
        }
    }

    return true;
}

