/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscLogic.cpp
   Author:        Zhu Weian
   Date:          2020-08-21
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-08-21] [1.0] [Creator]


*****************************************************************************************************/
#include "OscLogic.h"

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

enum EOscRegApsd
{
    SFP0_LINKUP_BIT = BIT_2,

    SFP1_LINKUP_BIT = BIT_3,
};

const std::vector<CSfpRegDef> COscLogic::msc_staSfpRegDef =
{
    {SFP0_ENBALE_BIT,
     SFP0_ONLINE_BIT,
     SFP0_RX_LOS_BIT,
     SFP1_LINKUP_BIT}, /* TODO: logic bug */

    {SFP1_ENBALE_BIT,
     SFP1_ONLINE_BIT,
     SFP1_RX_LOS_BIT,
     SFP1_LINKUP_BIT},
};



bool
COscLogic::SetEnable(uint32_t uiIdx, bool bEnable)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetEnable +++");
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiEnableBit;

    LOG_WRITE(EN_LOG_NOTICE, "m_uiSfpReg = 0X%02x, uiBit = %d, bEnable = %d", m_uiSfpReg, uiBit, bEnable);

    return __SetEnable(m_uiSfpReg, uiBit, bEnable);
}


bool
COscLogic::IsEnable(uint32_t uiIdx)
{
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiEnableBit;

    return __IsEnable(m_uiSfpReg, uiBit);
}


bool
COscLogic::IsOnline(uint32_t uiIdx)
{
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiOnlineBit;

    return __IsOnline(m_uiSfpReg, uiBit);
}

bool
COscLogic::IsRxLos(uint32_t uiIdx)
{
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiRxLosBit;

    return __IsRxLos(m_uiSfpReg, uiBit);
}


bool
COscLogic::IsLinkup(uint32_t uiIdx)
{
    uint32_t uiBit = msc_staSfpRegDef[uiIdx].uiLinkUpBit;

    return __IsLinkup(m_uiApsdReg, uiBit);
}

bool
COscLogic::__IsEnable(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    /* LOG_WRITE(EN_LOG_NOTICE, "uiReg = 0X%02x, uiBit = %d, ucVal = %d", uiReg, uiBit, ucVal); */

    return (0 == ucVal) ? true : false;
}



bool
COscLogic::__IsOnline(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);
    return (0 == ucVal) ? true : false;
}


bool
COscLogic::__IsRxLos(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    return (1 == ucVal) ? true : false;
}


bool
COscLogic::__IsLinkup(uint32_t uiReg, uint32_t uiBit)
{
    uint8_t ucVal = 0;

    m_stpLogicPtr->ReadBit(uiReg, uiBit, ucVal);

    return (0 == ucVal) ? true : false;
}


bool
COscLogic::__SetEnable(uint32_t uiReg, uint32_t uiBit, bool bEnable)
{
    if (true == bEnable) {
        /* 0 - ENABLE */
        m_stpLogicPtr->WriteBit(uiReg, uiBit, 0);
    } else {
        m_stpLogicPtr->WriteBit(uiReg, uiBit, 1);
    }


    LOG_WRITE(EN_LOG_NOTICE, "SetEnable ---");

    return true;
}


