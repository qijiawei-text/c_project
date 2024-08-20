/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscBoard.cpp
   Author:        Zhu Weian
   Date:          2020-08-21
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-08-21] [1.0] [Creator]


*****************************************************************************************************/
#include "OscBoard.h"


#ifdef __400GZR__

#define SFP_REG         (0xB8)
#define LINK_REG        (0xB0)

enum EOscRegApsd
{
    SFP0_LINKUP_BIT = BIT_1,
    SFP1_LINKUP_BIT = BIT_0,
};

#else

#define SFP_REG         (0x30)
#define LINK_REG        (0x34)


enum EOscRegApsd
{
    SFP0_LINKUP_BIT = BIT_3, /* TODO: logic bug */
    SFP1_LINKUP_BIT = BIT_3,
};

#endif

enum EOscRegDef
{
    SFP0_ENBALE_BIT = BIT_0,
    SFP0_ONLINE_BIT = BIT_1,
    SFP0_RX_LOS_BIT = BIT_2,
    SFP0_TX_FUALT_BIT = BIT_3,

    SFP1_ENBALE_BIT = BIT_4,
    SFP1_ONLINE_BIT = BIT_5,
    SFP1_RX_LOS_BIT = BIT_6,
    SFP1_TX_FUALT_BIT = BIT_7,
};



const std::vector<CSfpRegDef> COscBoard::msc_staSfpRegDef =
{
    {SFP0_ENBALE_BIT,
     SFP0_ONLINE_BIT,
     SFP0_RX_LOS_BIT,
     SFP0_LINKUP_BIT},

    {SFP1_ENBALE_BIT,
     SFP1_ONLINE_BIT,
     SFP1_RX_LOS_BIT,
     SFP1_LINKUP_BIT},
};



bool
COscBoard::SetEnable(uint32_t uiIdx, bool bEnable)
{
    LOG_WRITE(EN_LOG_DEBUG, "SetEnable +++");
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiEnableBit;

    LOG_WRITE(EN_LOG_NOTICE, "m_uiSfpReg = 0X%02x, uiBit = %d, bEnable = %d", SFP_REG, uiBit, bEnable);

    return __SetEnable(SFP_REG, uiBit, bEnable);
}


bool
COscBoard::IsEnable(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::IsEnable +++");

    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiEnableBit;

    return __IsEnable(SFP_REG, uiBit);
}


bool
COscBoard::IsOnline(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::IsOnline +++");

    if (uiIdx >= msc_staSfpRegDef.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx >= msc_staSfpRegDef.size(). uiIdx = %d", uiIdx);
        return false;
    }
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiOnlineBit;

    return __IsOnline(SFP_REG, uiBit);
}

bool
COscBoard::IsRxLos(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::IsRxLos +++");

    if (uiIdx >= msc_staSfpRegDef.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx >= msc_staSfpRegDef.size(). uiIdx = %d", uiIdx);
        return false;
    }

    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiRxLosBit;

    return __IsRxLos(SFP_REG, uiBit);
}


bool
COscBoard::IsLinkup(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::IsLinkup +++");

    if (uiIdx >= msc_staSfpRegDef.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx >= msc_staSfpRegDef.size(). uiIdx = %d", uiIdx);
        return false;
    }

    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiLinkUpBit;

    return __IsLinkup(LINK_REG, uiBit);
}

bool
COscBoard::__IsEnable(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;


    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::__IsEnable +++");

    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    /* LOG_WRITE(EN_LOG_DEBUG, "uiReg = 0X%02x, uiBit = %d, ucVal = %d", uiReg, uiBit, ucVal); */

    return (0 == ucVal) ? true : false;
}



bool
COscBoard::__IsOnline(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::__IsOnline +++");

    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::__IsOnline ---");
    return (0 == ucVal) ? true : false;
}


bool
COscBoard::__IsRxLos(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;


    LOG_WRITE(EN_LOG_DEBUG, "COscBoard::__IsRxLos +++");

    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    return (1 == ucVal) ? true : false;
}


bool
COscBoard::__IsLinkup(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    return (0 == ucVal) ? true : false;
}


bool
COscBoard::__SetEnable(uint32_t uiReg, uint32_t uiBit, bool bEnable)
{
    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    if (true == bEnable) {
        /* 0 - ENABLE */
        m_stpLogicPtr->WriteBit(uiReg, uiBit, 0);
    } else {
        m_stpLogicPtr->WriteBit(uiReg, uiBit, 1);
    }

    LOG_WRITE(EN_LOG_DEBUG, "SetEnable ---");

    return true;
}


