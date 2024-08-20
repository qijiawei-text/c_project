/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotBoard.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "SlotBoard.h"
#if 0
enum ESlotReg
{
    SLOT_PLUG_REG = 0x20,

    SLOT0_ONLINE_REG = 0xB010,
    SLOT1_ONLINE_REG = 0x6010,
    SLOT2_ONLINE_REG = 0x0204,
    SLOT3_ONLINE_REG = 0x0304,

    SLOT0_CARDTYPE_REG = 0xB000,
    SLOT1_CARDTYPE_REG = 0x6000,
    SLOT2_CARDTYPE_REG = 0x0200,
    SLOT3_CARDTYPE_REG = 0x0300,
};


const std::vector<CSlotReg> CSlotBoard::msc_staSlotRegs =
{
    {SLOT0_ONLINE_REG, BIT_2, SLOT_PLUG_REG, BIT_3, SLOT0_CARDTYPE_REG},
    {SLOT1_ONLINE_REG, BIT_2, SLOT_PLUG_REG, BIT_2, SLOT1_CARDTYPE_REG},
    {SLOT2_ONLINE_REG, BIT_0, SLOT_PLUG_REG, BIT_0, SLOT2_CARDTYPE_REG},
    {SLOT3_ONLINE_REG, BIT_1, SLOT_PLUG_REG, BIT_1, SLOT3_CARDTYPE_REG},

};
#endif
enum ESlotReg
{
    SLOT_PLUG_REG0 = 0x4C,
    SLOT_PLUG_REG1 = 0x4D,

    SLOT0_ONLINE_REG = 0x1009,
    SLOT1_ONLINE_REG = 0x1009,
    SLOT2_ONLINE_REG = 0x1008,
    SLOT3_ONLINE_REG = 0x1008,

    SLOT0_CARDTYPE_REG = 0xB0,
    SLOT1_CARDTYPE_REG = 0xB4,
    SLOT2_CARDTYPE_REG = 0xB8,
    SLOT3_CARDTYPE_REG = 0xBC,
};


const std::vector<CSlotReg> CSlotBoard::msc_staSlotRegs =
{
#ifdef __HCF__
    {SLOT0_ONLINE_REG, BIT_1, SLOT_PLUG_REG1, BIT_0, SLOT0_CARDTYPE_REG},        /* SLC0 */
    {SLOT3_ONLINE_REG, BIT_6, SLOT_PLUG_REG0, BIT_6, SLOT3_CARDTYPE_REG},    /* RPC */
#else
    {SLOT0_ONLINE_REG, BIT_1, SLOT_PLUG_REG1, BIT_0, SLOT0_CARDTYPE_REG},    /* SLC0 */
    {SLOT1_ONLINE_REG, BIT_2, SLOT_PLUG_REG1, BIT_1, SLOT1_CARDTYPE_REG},    /* SLC1 */
    {SLOT2_ONLINE_REG, BIT_7, SLOT_PLUG_REG0, BIT_7, SLOT2_CARDTYPE_REG},    /* OPS */
    {SLOT3_ONLINE_REG, BIT_6, SLOT_PLUG_REG0, BIT_6, SLOT3_CARDTYPE_REG},    /* RPC */
#endif
};


bool
CSlotBoard::IsSlotOnline(uint32_t uiIdx)
{
    uint8_t ucOnline = false;

    if (uiIdx >= msc_staSlotRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%d) >= msc_staSlotRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staSlotRegs[uiIdx].uiOnlineReg;
    uint32_t uiBit = msc_staSlotRegs[uiIdx].uiOnlineBit;

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucOnline)) {
        return false;
    }

    return (0 == ucOnline) ? true : false;
}

bool
CSlotBoard::IsSlotPlug(uint32_t uiIdx)
{
    uint8_t ucPlug = false;

    if (uiIdx >= msc_staSlotRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%d) >= msc_staSlotRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staSlotRegs[uiIdx].uiPlugReg;
    uint32_t uiBit = msc_staSlotRegs[uiIdx].uiPlugBit;

    if (false == m_stpLogicDrv->ReadBit(uiReg, uiBit, ucPlug)) {
        return false;
    }

    return (1 == ucPlug) ? true : false;
}

bool
CSlotBoard::ClearSlotPlug(uint32_t uiIdx)
{
    if (uiIdx >= msc_staSlotRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%d) >= msc_staSlotRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staSlotRegs[uiIdx].uiPlugReg;
    uint32_t uiBit = msc_staSlotRegs[uiIdx].uiPlugBit;
    uint32_t uiVal = (1 << uiBit);

    if (false == m_stpLogicDrv->WriteReg(uiReg, uiVal)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }

#if 0
    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }

    if (false == m_stpLogicDrv->WriteBit(uiReg, uiBit, 0)) {
        LOG_WRITE(EN_LOG_ERROR, "write plug flag error. reg = 0X%02x uiBit = 0X%02x", uiReg, uiBit);
        return false;
    }
#endif
    return true;
}


bool
CSlotBoard::GetCardType(uint32_t uiIdx, uint32_t &ruiType)
{
    uint8_t ucData;
    uint32_t uiReg = msc_staSlotRegs.at(uiIdx).uiCardTypeReg;

    if (false == m_stpLogicDrv->ReadReg(uiReg, &ucData)) {
        return false;
    }

    ruiType = (uint32_t)ucData;

    LOG_WRITE(EN_LOG_DEBUG, "uiIdx = %d uiReg = 0X%02x ruiType = %d", uiIdx, uiReg, ruiType);

    return true;
}



