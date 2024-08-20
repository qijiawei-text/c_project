/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LedBoard.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "LedBoard.h"
#if 0
#if 0
#ifdef __400GZR__

enum ELedReg
{
    LED_REG0 = 0x00030,     /* SYS_ALM */
    LED_REG1 = 0x00034,     /* FAN_LED */
    LED_REG2 = 0x1B020,     /* SLOT0 */
    LED_REG3 = 0x16020,     /* SLOT1 */
    LED_REG4 = 0x10210,     /* SLOT2 */
    LED_REG5 = 0x10310,     /* SLOT3 */
};

const std::vector<CLedReg> CLedBoard::msc_staLedRegs =
{
    {LED_REG0, 0x0F},
    {LED_REG0, 0xF0},
    {LED_REG1, 0x0F},
    {LED_REG1, 0xF0},
    {LED_REG2, 0x0F},
    {LED_REG2, 0xF0},
    {LED_REG3, 0x0F},
    {LED_REG3, 0xF0},
    {LED_REG4, 0x0F},
    {LED_REG4, 0xF0},
    {LED_REG5, 0x0F},
    {LED_REG5, 0xF0},
};


#else

enum ELegReg
{
    LED_REG0 = 0x3D,     /* SYS_ALM */
    LED_REG1 = 0x3E,     /* PWR_OSC */
    LED_REG2 = 0x3F,     /* LIN_FAN */
};

const std::vector<CLedReg> CLedBoard::msc_staLedRegs =
{
    {LED_REG0, 0x0F},
    {LED_REG0, 0xF0},
    {LED_REG1, 0x0F},
    {LED_REG1, 0xF0},
    {LED_REG2, 0x0F},
    {LED_REG2, 0xF0},

};

#endif
#else
/*
   8.6.11	Register 0x2028 -FPC LED  00000222
   Bit	Type	Reset	Description
   31:16
   RW	-	Reserved
   15:12	RW	0	ALM LED
   11:8	RW	2	FAN LED
   7:4	    RW	2	PSU LED
   3:0	    RW	2	SYS LED
 */
enum ELedReg
{
    LED_REG0 = 0x2028,
    LED_REG1 = 0x2029,
    LED_REG2 = 0x1B020,         /* SLOT0 */
    LED_REG3 = 0x16020,         /* SLOT1 */
    LED_REG4 = 0x10210,         /* SLOT2 */
    LED_REG5 = 0x10310,         /* SLOT3 */
};

const std::vector<CLedReg> CLedBoard::msc_staLedRegs =
{
    {LED_REG0, 0x0F},    /* SYS_LED */
    {LED_REG0, 0xF0},    /* FAN_LED */
    {LED_REG1, 0x0F},    /* PSU_LED */
    {LED_REG1, 0xF0},    /* ALM_LED */
    {LED_REG2, 0x0F},
    {LED_REG2, 0xF0},
    {LED_REG3, 0x0F},
    {LED_REG3, 0xF0},
    {LED_REG4, 0x0F},
    {LED_REG4, 0xF0},
    {LED_REG5, 0x0F},
    {LED_REG5, 0xF0},
};

#endif


struct CSlotLedReg
{
    std::vector<CLedReg> staLedRegSlot0;
    std::vector<CLedReg> staLedRegSlot1;
    std::vector<CLedReg> staLedRegSlot2;
    std::vector<CLedReg> staLedRegSlot3;
};


bool
CLedBoard::GetLedState(uint32_t uiIdx, uint32_t &ruiState)
{
    if (uiIdx >= msc_staLedRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staLedRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staLedRegs[uiIdx].uiReg;
    uint32_t uiRegMask = msc_staLedRegs[uiIdx].uiRegMask;

    return __GetLedState(uiReg, uiRegMask, ruiState);
}

bool
CLedBoard::SetLedState(uint32_t uiIdx, uint32_t uiState)
{
    if (uiIdx >= msc_staLedRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staLedRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staLedRegs[uiIdx].uiReg;
    uint32_t uiRegMask = msc_staLedRegs[uiIdx].uiRegMask;

    return __SetLedState(uiReg, uiRegMask, uiState);
}

bool
CLedBoard::__GetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t &ruiState)
{
    uint8_t ucTmp = 0;

    if (false == m_stpLogicDrv->Read(uiReg, &ucTmp, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "read reg[0x%x] fail. ucTmp = 0x%0x", uiReg, ucTmp);
        return false;
    }

    ucTmp &= uiRegMask;

    if (0xF0 == uiRegMask) {
        ucTmp = (ucTmp >> 4);
    }

    ruiState = ucTmp;

    return true;
}


bool
CLedBoard::__SetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t uiState)
{
    uint8_t ucTmp = 0;

    if (false == m_stpLogicDrv->Read(uiReg, &ucTmp, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "read reg[0x%x] fail. ucTmp = 0x%0x", uiReg, ucTmp);
        return false;
    }

    ucTmp &= ~uiRegMask;

    if (0x0F == uiRegMask) {
        ucTmp |= uiState;
    } else if (0xF0 == uiRegMask) {
        ucTmp |= (uiState << 4);
    }

    if (false == m_stpLogicDrv->Write(uiReg, &ucTmp, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write reg[0x%x] fail. ucTmp = 0x%0x", uiReg, ucTmp);
        return false;
    }

    return true;
}
#endif

enum ELedReg
{
    LED_RPC_GREEN  = 0x400,
    LED_RPC_RED    = 0x404,
    LED_RPC_AMBERG = 0x408,
    LED_RPC_AMBERR = 0x40c,
    LED_RPC_MODE0  = 0x410,
    LED_RPC_BLINK0 = 0x414,
    LED_RPC_MODE1  = 0x418,
    LED_RPC_BLINK1 = 0x41c,
    LED_RPC_MODE2  = 0x420,
    LED_RPC_BLINK2 = 0x424,
    LED_RPC_MODE3  = 0x428,
    LED_RPC_BLINK3 = 0x42c,
    LED_RPC_ACCESS_MODE = 0x430,
    LED_RPC_TEST_MODE = 0x434,
    LED_RPC_LOCATION_TEST_MODE = 0x438,

    LED_SCC_GREEN  = 0x1048,
    LED_SCC_RED    = 0x104C,
    LED_SCC_AMBERG = 0x1050,
    LED_SCC_AMBERR = 0x1054,
    LED_SCC_MODE0  = 0x1058,
    LED_SCC_BLINK0 = 0x105C,
    LED_SCC_MODE1  = 0x1060,
    LED_SCC_BLINK1 = 0x1064,
    LED_SCC_MODE2  = 0x1068,
    LED_SCC_BLINK2 = 0x106C,
    LED_SCC_MODE3  = 0x1070,
    LED_SCC_BLINK3 = 0x1074,
    LED_SCC_ACCESS_MODE = 0x1078,
    LED_SCC_TEST_MODE = 0x107C,
    LED_SCC_LOCATION_TEST_MODE = 0x1080,
};

const std::vector<CLedReg> CLedBoard::msc_staLedRegs =
{
    {LED_SCC_MODE2, 0x0F},    /* SCC_LED0 SYS */
    {LED_SCC_MODE1, 0x0F},    /* SCC_LED1 ALM */
    {LED_SCC_MODE0, 0x0F},    /* SCC_LED2 FAN */
    /* {LED_SCC_MODE3, 0x0F}, */    /* SCC_LED3 */

    {LED_RPC_MODE0, 0x0F},    /* FAN_LED0 */
    {LED_RPC_MODE1, 0x0F},    /* FAN_LED1 */
    {LED_RPC_MODE2, 0x0F},    /* FAN_LED2 */
    {LED_RPC_MODE3, 0x0F},    /* FAN_LED3 */

};

bool
CLedBoard::GetLedState(uint32_t uiIdx, uint32_t &ruiState)
{
    if (uiIdx >= msc_staLedRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staLedRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staLedRegs[uiIdx].uiReg;
    uint32_t uiRegMask = uiIdx;

    return __GetLedState(uiReg, uiRegMask, ruiState);
}

bool
CLedBoard::SetLedState(uint32_t uiIdx, uint32_t uiState)
{
    if (uiIdx >= msc_staLedRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staLedRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staLedRegs[uiIdx].uiReg;
    uint32_t uiRegMask = uiIdx;
    if (uiIdx > 2 && false == m_bReadyFlag) {
        /* notify cpld SCC CPU is ready */
        uint8_t ucDataW = 1;
        m_stpLogicDrv->Write(0xC0, &ucDataW, 1);
        m_stpLogicDrv->Write(0x10A0, &ucDataW, 1);
        m_bReadyFlag = true;
        LOG_WRITE(EN_LOG_NOTICE, "notify cpld SCC CPU is ready,uiIdx:%d", uiIdx);
    }
    return __SetLedState(uiReg, uiRegMask, uiState);
}

bool
CLedBoard::SetLedTestMode(uint32_t uiIdx, bool bTestMode)
{
    uint32_t uiReg = 0;
    uint8_t ucTestMode = bTestMode;

    if ( uiIdx > LED_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx[0x%x]  LED_CNT", uiIdx);
        return false;
    }

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if ( LED0 < uiIdx && LED3 > uiIdx) {
        uiReg = LED_SCC_TEST_MODE;
    } else if ( LED4 < uiIdx && LED_CNT > uiIdx) {
        uiReg = LED_RPC_TEST_MODE;
    }

    if (false == m_stpLogicDrv->Write(uiReg, &ucTestMode, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write reg[0x%x] fail. bTestMode = 0x%0x", uiReg, bTestMode);
        return false;
    }

    return true;
}

bool
CLedBoard::SetLedLocationTestMode(uint32_t uiIdx, bool bTestMode)
{
    uint32_t uiReg = 0;
    uint8_t ucTestMode = bTestMode;

    if ( uiIdx > LED_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx[0x%x]  LED_CNT", uiIdx);
        return false;
    }

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if ( LED0 < uiIdx && LED3 > uiIdx) {
        uiReg = LED_SCC_LOCATION_TEST_MODE;
    } else if ( LED4 < uiIdx && LED_CNT > uiIdx) {
        uiReg = LED_RPC_LOCATION_TEST_MODE;
    }

    if (false == m_stpLogicDrv->Write(uiReg, &ucTestMode, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write reg[0x%x] fail. bTestMode = 0x%0x", uiReg, bTestMode);
        return false;
    }

    return true;
}

/*
   enum
   {
    LED_STATUS_OFF,
    LED_STATUS_RED,
    LED_STATUS_GREEN,
    LED_STATUS_AMBER,
    LED_STATUS_RED_BLINK = 5,
    LED_STATUS_GREEN_BLINK,
    LED_STATUS_AMBER_BLINK
   };

   LED color Mode.
   0: off
   1: red
   2: green
   3: amber

   LED blink Mode.
   0: always on
   1: blink
 */
bool
CLedBoard::__GetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t &ruiState)
{
    uint8_t ucTmpMode = 0;
    uint8_t ucTmpBlink = 0;
    uint8_t ucTmp = 0;
    if (false == m_stpLogicDrv->Read(uiReg, &ucTmpMode, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "read reg[0x%x] fail. ucTmpMode = 0x%0x", uiReg, ucTmpMode);
        return false;
    }

    if (false == m_stpLogicDrv->Read(uiReg + 4, &ucTmpBlink, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "read reg[0x%x] fail. ucTmpBlink = 0x%0x", uiReg, ucTmpBlink);
        return false;
    }

    if ( 0 == ucTmpMode ) {
        ucTmp = LED_STATUS_OFF;
    } else if ( 1 == ucTmpMode && 0 == ucTmpBlink) {
        ucTmp = LED_STATUS_RED;
    } else if ( 2 == ucTmpMode && 0 == ucTmpBlink) {
        ucTmp = LED_STATUS_GREEN;
    } else if ( 3 == ucTmpMode && 0 == ucTmpBlink) {
        ucTmp = LED_STATUS_AMBER;
    } else if ( 1 == ucTmpMode && 1 == ucTmpBlink) {
        ucTmp = LED_STATUS_RED_BLINK;
    } else if ( 2 == ucTmpMode && 1 == ucTmpBlink) {
        ucTmp = LED_STATUS_GREEN_BLINK;
    } else if ( 3 == ucTmpMode && 1 == ucTmpBlink) {
        ucTmp = LED_STATUS_AMBER_BLINK;
    }

    ruiState = ucTmp;

    return true;
}


bool
CLedBoard::__SetLedState(uint32_t uiReg, uint32_t uiRegMask, uint32_t uiState)
{
    uint8_t ucTmpMode = 0;
    uint8_t ucTmpBlink = 0;

    if ( LED_STATUS_OFF == uiState ) {
        ucTmpMode = 0;
        ucTmpBlink = 0;
    } else if ( LED_STATUS_RED == uiState ) {
        ucTmpMode = 1;
        ucTmpBlink = 0;
    } else if ( LED_STATUS_GREEN == uiState ) {
        ucTmpMode = 2;
        ucTmpBlink = 0;
    } else if ( LED_STATUS_AMBER == uiState ) {
        ucTmpMode = 3;
        ucTmpBlink = 0;
    } else if ( LED_STATUS_RED_BLINK == uiState ) {
        ucTmpMode = 1;
        ucTmpBlink = 1;
    } else if ( LED_STATUS_GREEN_BLINK == uiState ) {
        ucTmpMode = 2;
        ucTmpBlink = 1;
    } else if ( LED_STATUS_AMBER_BLINK == uiState ) {
        ucTmpMode = 3;
        ucTmpBlink = 1;
    }

    if (false == m_stpLogicDrv->Write(uiReg, &ucTmpMode, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write reg[0x%x] fail. ucTmp = 0x%0x", uiReg, ucTmpMode);
        return false;
    }

    if (false == m_stpLogicDrv->Write(uiReg + 4, &ucTmpBlink, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write reg[0x%x] fail. ucTmp = 0x%0x", uiReg, ucTmpBlink);
        return false;
    }

    return true;
}
