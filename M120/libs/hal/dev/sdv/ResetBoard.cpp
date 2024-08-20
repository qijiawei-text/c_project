/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetBoard.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "ResetBoard.h"


#ifdef __400GZR__

enum EResetReg
{
    RESET_REG0 = 0xB4,       /* OTDR */
    RESET_REG1 = 0x6030,     /* OCM */
    RESET_REG2 = 0x0240,     /* PA */
    RESET_REG3 = 0x0340,     /* BA */
    RESET_PHY  = 0xB1,
};


const std::vector<CResetReg> CResetBoard::msc_staResetRegs =
{
    {RESET_REG0, BIT_0},  /* OTDR */
    {RESET_REG1, BIT_4},  /* OCM */
    {RESET_REG2, BIT_4},  /* PA */
    {RESET_REG3, BIT_4},  /* BA */
    {RESET_REG0, BIT_3},  /* Extend */
    {RESET_PHY, BIT_0},   /* PHY */
};


#elif __MEIWU__

enum EResetReg
{
    RESET_REG0 = 0x36,
    RESET_REG1 = 0x37,
    RESET_REG2 = 0x29,
};

const std::vector<CResetReg> CResetBoard::msc_staResetRegs =
{
    {RESET_REG0, BIT_1}, /* 0 -- MEIWU : OA */
    {RESET_REG1, BIT_1}, /* 1 -- MEIWU : no use */
    {RESET_REG2, BIT_0}, /* 2 -- MEIWU : Wss0 Hard Reset */
    {RESET_REG2, BIT_1}, /* 3 -- MEIWU : Wss0 Warm Reset */
    {RESET_REG2, BIT_2}, /* 4 -- MEIWU : Ops Hard Reset */
    {RESET_REG2, BIT_3}, /* 5 -- MEIWU : Ops Warm Reset */
    {RESET_REG2, BIT_4}, /* 6 -- MEIWU : OCM */
    {RESET_REG2, BIT_5}, /* 7 -- MEIWU : Wss1 Hard Reset */
    {RESET_REG2, BIT_6}, /* 8 -- MEIWU : Wss1 Warm Reset */
};

#elif __LBAND__
enum EResetReg
{
    RESET_REG0 = 0x1014,     /* SCC CPLD */
    RESET_REG1 = 0x2008,     /* OCM */
};


const std::vector<CResetReg> CResetBoard::msc_staResetRegs =
{
    {RESET_REG0, BIT_0},  /* SLC0 */
    {RESET_REG0, BIT_1},  /* SLC1 */
    {RESET_REG0, BIT_2},  /* SLC2 */
    {RESET_REG0, BIT_3},  /* SLC3 */
    {RESET_REG0, BIT_4},  /* FPC */
    {RESET_REG1, BIT_0},  /* 9541 */
    {RESET_REG1, BIT_1},      /* PHY0 */
    {RESET_REG1, BIT_2},  /* PHY1 */
    {RESET_REG1, BIT_3},  /* PHY2 */
};
#else
enum EResetReg
{
    RESET_REG0 = 0x38,       /* SCC  */
    RESET_REG1 = 0x1010,     /* SCC CPLD SLC Reset and Interrupt */
    RESET_REG2 = 0x304C,     /* SLC0 OCM/OTDR Reset */
    RESET_REG3 = 0x3050,     /* SLC0 EDFA Ctrl */
    RESET_REG4 = 0x314C,     /* SLC1 OCM/OTDR Reset */
    RESET_REG5 = 0x3150,     /* SLC1 EDFA Ctrl */
    RESET_REG6 = 0x1014,     /* SCC CPLD SLC Power Enable, for mantis-40210 temparory*/
    RESET_REG7 = 0xe8,       /* SLC Interface SPI and UART Disable Control */

};

#define OPS4_A_REST_INDEX         16
#define OPS4_B_REST_INDEX         17

const std::vector<CResetReg> CResetBoard::msc_staResetRegs =
{
    {RESET_REG0, BIT_0},  /* 0 PCIe reset, 0: reset, 1: normal */
    {RESET_REG0, BIT_1},  /* 1 I350 reset, 0: reset, 1: normal */
    {RESET_REG0, BIT_2},  /* 2 PHY reset, 0: reset, 1: normal */
    {RESET_REG0, BIT_3},  /* 3 88E6390X Switch reset, 0: reset, 1: normal */
    {RESET_REG1, BIT_4},  /* 4 RPC_RESET_N_CPLD. L?active */
    {RESET_REG6, BIT_1},  /* 5 OPS_RESET_N_CPLD. L?active */   /* for mantis-40210  0x1010 bit5 to 0x1014 bit1, 20031128 recovery for xujie request as fpga/cpld change */
    {RESET_REG1, BIT_6},  /* 6 SLC0_RESET_N_CPLD. L?active */
    {RESET_REG1, BIT_7},  /* 7 SLC1_RESET_N_CPLD. L?active */
    {RESET_REG2, BIT_0},  /* 8 SLC0 OTDR hard, L: reset H: normal */
    {RESET_REG2, BIT_1},  /* 9 SLC0 OCM hard, L: reset H: normal */
    {RESET_REG3, BIT_4},  /*10 SLC0 EDFA hard L: reset*/
    {RESET_REG3, BIT_5},  /*11 SLC0 EDFA soft L: reset*/
    {RESET_REG4, BIT_0},  /*12 SLC1 OTDR hard, L: reset H: normal */
    {RESET_REG4, BIT_1},  /*13 SLC1 COM hard, L: reset H: normal */
    {RESET_REG5, BIT_4},  /*14 SLC1 EDFA hard L: reset*/
    {RESET_REG5, BIT_5},  /*15 SLC1 EDFA soft L: reset*/
    {RESET_REG6, BIT_2},  /*16 SLC0 OPS4 RESET_N_CPLD L: reset*/
    {RESET_REG6, BIT_3},  /*17 SLC1 OPS4 RESET_N_CPLD L: reset*/
};

#endif

/* Keep EQA 171 in sequence                                        */
static pthread_mutex_t m_stResetBoardMutexLock = PTHREAD_MUTEX_INITIALIZER;

bool
CResetBoard::Reset(uint32_t uiIdx)
{
    pthread_mutex_lock(&m_stResetBoardMutexLock);

    LOG_WRITE(EN_LOG_NOTICE, "ResetBoard +++");

    if (uiIdx >= msc_staResetRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staResetRegs.size()(%lu).",
                  uiIdx, msc_staResetRegs.size());
        pthread_mutex_unlock(&m_stResetBoardMutexLock);
        return false;
    }

    uint32_t uiReg = msc_staResetRegs[uiIdx].uiReg;
    uint32_t uiBit = msc_staResetRegs[uiIdx].uiBit;

    uint32_t uiTmp = 0;

    /* pre OPS4 cold reset */
    if (OPS4_A_REST_INDEX == uiIdx) {
        /* set 0xe8 BIT_2 1 make ops4-A SPI low power */
        m_stpLogicDrv->WriteBit(RESET_REG7, BIT_2, 1);
        LOG_WRITE(EN_LOG_NOTICE, "write 1 RESET_REG7 = 0X%02x, BIT_2 = %d", RESET_REG7, BIT_2);

        m_stpLogicDrv->Read(RESET_REG7, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

        /* set 0x1010 BIT_6 0 make ops4-A RESET low power */
        m_stpLogicDrv->WriteBit(RESET_REG1, BIT_6, 0);
        LOG_WRITE(EN_LOG_NOTICE, "write 0 RESET_REG1 = 0X%02x, BIT_6 = %d", RESET_REG1, BIT_6);

        m_stpLogicDrv->Read(RESET_REG1, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);
    } else if (OPS4_B_REST_INDEX == uiIdx) {
        /* set 0xe8 BIT_3 1 make ops4-B SPI low power */
        m_stpLogicDrv->WriteBit(RESET_REG7, BIT_3, 1);
        LOG_WRITE(EN_LOG_NOTICE, "write 1 RESET_REG7 = 0X%02x, BIT_3 = %d", RESET_REG7, BIT_3);

        m_stpLogicDrv->Read(RESET_REG7, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

        /* set 0x1010 BIT_7 0 make ops4-B RESET low power */
        m_stpLogicDrv->WriteBit(RESET_REG1, BIT_7, 0);
        LOG_WRITE(EN_LOG_NOTICE, "write 0 RESET_REG1 = 0X%02x, BIT_7 = %d", RESET_REG1, BIT_7);

        m_stpLogicDrv->Read(RESET_REG1, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);
    }

    m_stpLogicDrv->Read(uiReg, (uint8_t *)&uiTmp, 1);
    LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

    LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d uiReg = 0X%02x, uiBit = %d", uiIdx, uiReg, uiBit);
    m_stpLogicDrv->WriteBit(uiReg, uiBit, 0);

    m_stpLogicDrv->Read(uiReg, (uint8_t *)&uiTmp, 1);
    LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

    if ( RESET_REG0 == uiReg && BIT_0 == uiBit) {
        /* LOG_WRITE(EN_LOG_NOTICE, "CResetBoard otdr reset delay 5ms"); */
        CDelay::Delay(5, MS);
    } else {
        /* LOG_WRITE(EN_LOG_NOTICE, "CResetBoard other reset delay 1s"); */
        CDelay::Delay(1, S);
    }

    m_stpLogicDrv->WriteBit(uiReg, uiBit, 1);

    m_stpLogicDrv->Read(uiReg, (uint8_t *)&uiTmp, 1);

    LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

    /* restore OPS4 cold reset */
    if (OPS4_A_REST_INDEX == uiIdx) {
        /* set 0x1010 BIT_6 1 make ops4-A RESET restore power */
        m_stpLogicDrv->WriteBit(RESET_REG1, BIT_6, 1);
        LOG_WRITE(EN_LOG_NOTICE, "write 1 RESET_REG1 = 0X%02x, BIT_6 = %d", RESET_REG1, BIT_6);

        m_stpLogicDrv->Read(RESET_REG1, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

        /* set 0xe8 BIT_2 0 make ops4-A SPI restore power */
        m_stpLogicDrv->WriteBit(RESET_REG7, BIT_2, 0);
        LOG_WRITE(EN_LOG_NOTICE, "write 0 RESET_REG7 = 0X%02x, BIT_2 = %d", RESET_REG7, BIT_2);

        m_stpLogicDrv->Read(RESET_REG7, (uint8_t *)&uiTmp, 0);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);
    } else if (OPS4_B_REST_INDEX == uiIdx) {
        /* set 0x1010 BIT_7 1 make ops4-B RESET restore power */
        m_stpLogicDrv->WriteBit(RESET_REG1, BIT_7, 1);
        LOG_WRITE(EN_LOG_NOTICE, "write 1 RESET_REG1 = 0X%02x, BIT_7 = %d", RESET_REG1, BIT_7);

        m_stpLogicDrv->Read(RESET_REG1, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);

        /* set 0xe8 BIT_3 0 make ops4-B SPI restore power */
        m_stpLogicDrv->WriteBit(RESET_REG7, BIT_3, 0);
        LOG_WRITE(EN_LOG_NOTICE, "write 0 RESET_REG7 = 0X%02x, BIT_3 = %d", RESET_REG7, BIT_3);

        m_stpLogicDrv->Read(RESET_REG7, (uint8_t *)&uiTmp, 1);
        LOG_WRITE(EN_LOG_NOTICE, "uiTmp = %d", uiTmp);
    }

    LOG_WRITE(EN_LOG_NOTICE, "ResetBoard ---");

    pthread_mutex_unlock(&m_stResetBoardMutexLock);

    return true;
}




