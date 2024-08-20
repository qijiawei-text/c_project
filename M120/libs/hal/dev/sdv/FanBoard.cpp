/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      FanBoard.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "FanBoard.h"



/*
   enum EFanReg
   {

   #ifdef __400GZR__

    FAN_ONLINE_REG = 0x70,
    FAN_PLUG_REG = 0x72,

    FAN1_SET_SPEED_REG = 0x50,
    FAN2_SET_SPEED_REG = 0x51,
    FAN3_SET_SPEED_REG = 0x52,
    FAN4_SET_SPEED_REG = 0x53,

    FAN1_GET_SPEED_REG = 0x60,
    FAN2_GET_SPEED_REG = 0x61,
    FAN3_GET_SPEED_REG = 0x62,
    FAN4_GET_SPEED_REG = 0x63,

   #else

    FAN_PLUG_REG = 0x0E,
    FAN_ONLINE_REG = 0x0F,


    FAN1_SET_SPEED_REG = 0x10,
    FAN2_SET_SPEED_REG = 0x11,
    FAN3_SET_SPEED_REG = 0x12,
    FAN4_SET_SPEED_REG = 0x13,

    FAN1_GET_SPEED_REG = 0x14,
    FAN2_GET_SPEED_REG = 0x15,
    FAN3_GET_SPEED_REG = 0x16,
    FAN4_GET_SPEED_REG = 0x17,

   #endif

   };

   const std::vector<CFanReg> CFanBoard::msc_staFanRegs =
   {
    {FAN_ONLINE_REG, BIT_0, FAN_PLUG_REG, BIT_0, FAN1_SET_SPEED_REG, FAN1_GET_SPEED_REG},
    {FAN_ONLINE_REG, BIT_1, FAN_PLUG_REG, BIT_1, FAN2_SET_SPEED_REG, FAN2_GET_SPEED_REG},
    {FAN_ONLINE_REG, BIT_2, FAN_PLUG_REG, BIT_2, FAN3_SET_SPEED_REG, FAN3_GET_SPEED_REG},
    {FAN_ONLINE_REG, BIT_3, FAN_PLUG_REG, BIT_3, FAN4_SET_SPEED_REG, FAN4_GET_SPEED_REG},
   };
 */

/*
   0x2018
   6	R	1	FAN2, 0: Presence 1: Not Insert
   5	R		FAN1, 0: Presence 1: Not Insert
   4	R		FAN0, 0: Presence 1: Not Insert


   0x2040
   31:     RW	-	Reserved
   23:16	RW	50	FAN3 PWM Ratio
   15:8	RW	50	FAN2 PWM Ratio
   7:0	    RW	50	FAN1 PWM Ratio


   0x2044
   31:24
   R	-	Reserved
   23:16	R	-	FAN3 Speed
   15:8	R	-	FAN2 Speed
   7:0          R	-   FAN1 Speed
 */
enum EFanReg
{

    FAN_ONLINE_REG = 0x1008,
    FAN_PLUG_REG = 0x4C,
#if 0
    FAN1_SET_SPEED_REG = 0x2040,
    FAN2_SET_SPEED_REG = 0x2041,
    FAN3_SET_SPEED_REG = 0x2042,
    FAN4_SET_SPEED_REG = 0x1020,

    FAN1_GET_SPEED_REG = 0x2044,
    FAN2_GET_SPEED_REG = 0x2045,
    FAN3_GET_SPEED_REG = 0x2046,
    FAN4_GET_SPEED_REG = 0x1024,

    FAN1_GET_LED_REG = 0x2048,
    FAN2_GET_LED_REG = 0x2048,
    FAN3_GET_LED_REG = 0x2048,
    FAN4_GET_LED_REG = 0xffff,

    FAN1_SET_LED_REG = 0x2048,
    FAN2_SET_LED_REG = 0x2048,
    FAN3_SET_LED_REG = 0x2048,
    FAN4_SET_LED_REG = 0xffff,
#endif
};

const std::vector<CFanReg> CFanBoard::msc_staFanRegs =
{
    {FAN_ONLINE_REG, BIT_2, FAN_PLUG_REG, BIT_2},
    {FAN_ONLINE_REG, BIT_3, FAN_PLUG_REG, BIT_3},
    {FAN_ONLINE_REG, BIT_4, FAN_PLUG_REG, BIT_4},
    {FAN_ONLINE_REG, BIT_5, FAN_PLUG_REG, BIT_5},
};



bool
CFanBoard::IsPlug(uint32_t uiIdx)
{
    uint8_t ucPlug = 1;

    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiPlugReg;
    uint32_t uiBit = msc_staFanRegs[uiIdx].uiPlugBit;


    LOG_WRITE(EN_LOG_DEBUG, "CFanBoard::IsFanPlug +++");

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucPlug)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "uiReg = 0X%02x  uiBit = 0X%02x ucPlug = 0X%02x", uiReg, uiBit, ucPlug);

    return (1 == ucPlug) ? true : false;
}

bool
CFanBoard::ClearPlug(uint32_t uiIdx)
{
    uint32_t uiReg = msc_staFanRegs[uiIdx].uiPlugReg;;
    uint32_t uiBit = msc_staFanRegs[uiIdx].uiPlugBit;
#if 0
    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }
#else
    /* OC2A-266 INT - mount/umount card optimization, fix issue when multiple psus plug simultaneous  */
    uint32_t uiVal = (1 << uiBit);
    if (false == m_stpLogicDrv->WriteReg(uiReg, uiVal)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }
#endif
    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 0)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }

    return true;
}



bool
CFanBoard::IsOnline(uint32_t uiIdx)
{
    uint8_t ucOnline = 0;
    LOG_WRITE(EN_LOG_DEBUG, "uiIdx = %d CFanBoard::IsFanOnline +++", uiIdx);

    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiOnlineReg;
    uint32_t uiBit = msc_staFanRegs[uiIdx].uiOnlineBit;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucOnline)) {
        LOG_WRITE(EN_LOG_ERROR, "ReadBit error. uiReg = 0X%02x, uiBit = %d", uiReg, uiBit);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CFanBoard::IsFanOnline ---");

    return (0 == ucOnline) ? true : false;
}

#if 0
bool
CFanBoard::GetFanSpeed(uint32_t uiIdx, uint32_t &uiSpeed)
{
    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiGetSpeedReg;
    uint8_t ucSpeed = 0;

    if (false == m_stpLogicDrv->ReadReg(uiReg, &ucSpeed)) {
        return false;
    }

    uiSpeed = ucSpeed;

    return true;
}

bool
CFanBoard::SetFanSpeed(uint32_t uiIdx, uint32_t uiSpeed)
{
    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiSetSpeedReg;
    uint8_t ucSpeed = uiSpeed;

    if (false == m_stpLogicDrv->WriteReg(uiReg, ucSpeed)) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Fan(%d) speed reg = 0X%02x uiSpeed = %d", uiIdx, uiReg, uiSpeed);

    return true;
}

bool
CFanBoard::GetFanLedState(uint32_t uiIdx, uint32_t &ruiState)
{

    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiGetLedReg;
    uint8_t ucState = 0;

    if (false == m_stpLogicDrv->ReadReg(uiReg, &ucState)) {
        return false;
    }

    ruiState = (ucState >> (uiIdx * 2)) & 0x03;

    return true;

}

bool
CFanBoard::SetFanLedState(uint32_t uiIdx, uint32_t uiState)
{
    uint32_t uiMask[3] = {0x3C, 0x33, 0x0F};

    if (uiIdx >= msc_staFanRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staFanRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staFanRegs[uiIdx].uiGetLedReg;
    if ( 0xffff == uiReg) {
        return true;
    }
    uint8_t ucState = 0;
    if (false == m_stpLogicDrv->ReadReg(uiReg, &ucState)) {
        return false;
    }

    ucState &= uiMask[uiIdx];
    ucState |= uiState << (uiIdx * 2);

    uiReg = msc_staFanRegs[uiIdx].uiSetLedReg;
    if (false == m_stpLogicDrv->WriteReg(uiReg, ucState)) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Fan(%d) speed reg = 0X%02x uiLedState = %d", uiIdx, uiReg, ucState);

    return true;

}
#endif


