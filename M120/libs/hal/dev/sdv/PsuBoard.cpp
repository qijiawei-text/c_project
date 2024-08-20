/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      PsuBoard.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "PsuBoard.h"
/*
   #ifdef __400GZR__

   enum PsuPlugReg
   {
    PSU0_PLUG_REG = 0x38,
    PSU1_PLUG_REG = 0x38,
   };


   enum PsuOnlineReg
   {
    PSU0_ONLINE_REG = 0x30,
    PSU1_ONLINE_REG = 0x34,
   };

   const std::vector<CPsuReg> CPsuBoard::msc_staPsuRegs =
   {
    {PSU0_PLUG_REG, BIT_0, PSU0_ONLINE_REG, BIT_0, BIT_1},
    {PSU1_PLUG_REG, BIT_4, PSU1_ONLINE_REG, BIT_0, BIT_1}
   };


   #else

   enum PsuPlugReg
   {
    PSU0_PLUG_REG = 0x32,
    PSU1_PLUG_REG = 0x32,
   };

   enum PsuOnlineReg
   {
    PSU0_ONLINE_REG = 0x30,
    PSU1_ONLINE_REG = 0x30,
   };

   const std::vector<CPsuReg> CPsuBoard::msc_staPsuRegs =
   {
    {PSU0_PLUG_REG, BIT_0, PSU0_ONLINE_REG, BIT_1, BIT_2},
    {PSU1_PLUG_REG, BIT_1, PSU1_ONLINE_REG, BIT_5, BIT_6}
   };


   #endif
 */

/*
   0x2018
   1	R		PWR1, 0: Presence 1: Not Insert
   0	R		PWR0, 0: Presence 1: Not Insert

   0x201C
   3	R		PWR1 OK ,    1:Good, 0:Fail
   2	R		PWR1 PRSNT , 1:Absent,0:Present
   1	R		PWR0 OK ,    1:Good, 0:Fail
   0	R		PWR0 PRSNT , 1:Absent,0:Presents
 */
enum PsuPlugReg
{
    PSU0_PLUG_REG = 0x4C,
    PSU1_PLUG_REG = 0x4C,
};


enum PsuOnlineReg
{
    PSU0_ONLINE_REG = 0x1008,
    PSU1_ONLINE_REG = 0x1008,
};

enum PsuOkReg
{
    PSU0_OK_REG = 0x1009,
    PSU1_OK_REG = 0x1009,
};

const std::vector<CPsuReg> CPsuBoard::msc_staPsuRegs =
{
    {PSU0_PLUG_REG, BIT_0, PSU0_ONLINE_REG, BIT_0, PSU0_OK_REG, BIT_6},
    {PSU1_PLUG_REG, BIT_1, PSU1_ONLINE_REG, BIT_1, PSU1_OK_REG, BIT_7}
};



bool
CPsuBoard::IsPlug(uint32_t uiIdx)
{
    uint8_t ucPlug = 0;

    uint32_t uiReg = msc_staPsuRegs[uiIdx].uiPlugReg;
    uint32_t uiBit = msc_staPsuRegs[uiIdx].uiPlugBit;

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucPlug)) {
        return false;
    }

    return (1 == ucPlug) ? true : false;
}

bool
CPsuBoard::ClearPlug(uint32_t uiIdx)
{
    uint32_t uiReg = msc_staPsuRegs[uiIdx].uiPlugReg;
    uint32_t uiBit = msc_staPsuRegs[uiIdx].uiPlugBit;
#if 0
    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Psu(%d) Plug Reg(%x) Bit(%d) Write 1 error.", uiIdx, uiReg, uiBit);
        return false;
    }
#else
    /* OC2A-266 INT - mount/umount card optimization, fix issue when multiple psus plug simultaneous  */
    uint32_t uiVal = (1 << uiBit);
    if (false == m_stpLogicDrv->WriteReg(uiReg, uiVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Psu(%d) Plug Reg(%x) Bit(%d) Write 1 error.", uiIdx, uiReg, uiBit);
        return false;
    }
#endif
    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 0)) {
        LOG_WRITE(EN_LOG_ERROR, "Psu(%d) Plug Reg(%x) Bit(%d) Write 0 error.", uiIdx, uiReg, uiBit);
        return false;
    }

    return true;
}




bool
CPsuBoard::IsOnline(uint32_t uiIdx)
{
    uint8_t ucOnline = 0;
    uint32_t uiReg = msc_staPsuRegs[uiIdx].uiOnlineReg;
    uint32_t uiBit = msc_staPsuRegs[uiIdx].uiOnlineBit;

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucOnline)) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "uiReg = %x, uiBit = %d, ucOnline = %d", uiReg, uiBit, ucOnline);

    return (0 == ucOnline) ? true : false;
}

bool
CPsuBoard::IsOk(uint32_t uiIdx)
{
    uint8_t ucOk = 0;
    uint32_t uiReg = msc_staPsuRegs[uiIdx].uiOkReg;
    uint32_t uiBit = msc_staPsuRegs[uiIdx].uiOkBit;

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucOk)) {
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "Psu(%d), ucOk = %d", uiIdx, ucOk); */

#ifdef __400GZR__
    return (0 == ucOk) ? true : false;
#else
    return (1 == ucOk) ? true : false;
#endif
}

