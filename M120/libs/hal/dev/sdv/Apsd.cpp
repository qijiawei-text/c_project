/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Apsd.cpp
   Author:        Zhu Weian
   Date:          2020-10-06
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-06] [1.0] [Creator]


*****************************************************************************************************/

#include "Apsd.h"

#ifdef __400GZR__

enum EApsdReg
{
    APSD_REG = 0x340,
};


const std::vector<CApsdReg> CApsd::msc_staApsdRegs =
{
    {APSD_REG, BIT_7, BIT_0},
    /* {APSD_REG, BIT_7, BIT_4}, */
};


#else

enum EApsdReg
{
    APSD_REG = 0x34,
};

const std::vector<CApsdReg> CApsd::msc_staApsdRegs =
{
    {APSD_REG, BIT_1, BIT_7},
    /* {APSD_REG, BIT_1, BIT_3}, */
};


#endif

bool
CApsd::SetApsdEnable(uint32_t uiIdx, bool bEnable)
{
    if (uiIdx >= msc_staApsdRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staApsdRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staApsdRegs.at(uiIdx).uiReg;
    uint32_t uiEnableBit = msc_staApsdRegs.at(uiIdx).uiEnableBit;
    uint32_t uiPumpBit = msc_staApsdRegs.at(uiIdx).uiPumpBit;

    return __SetApsdEnable(uiReg, uiEnableBit, uiPumpBit, bEnable);
}

bool
CApsd::GetApsdStatus(uint32_t uiIdx, bool &rbEnable, bool &rbStatus, bool &rbEvent)
{
    if (uiIdx >= msc_staApsdRegs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%u) >= msc_staApsdRegs.size()", uiIdx);
        return false;
    }

    uint32_t uiReg = msc_staApsdRegs.at(uiIdx).uiReg;
    uint32_t uiEnableBit = msc_staApsdRegs.at(uiIdx).uiEnableBit;
    uint32_t uiPumpBit = msc_staApsdRegs.at(uiIdx).uiPumpBit;

    __GetApsdEnable(uiReg, uiEnableBit, rbEnable);
    __GetApsdStatus(uiReg, uiPumpBit, rbStatus);

    return true;
}

bool
CApsd::__SetApsdEnable(uint32_t uiReg, uint32_t uiEnableBit, uint32_t uiPumpBit, bool bEnable)
{
    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    if (bEnable) {
        /* 0 - ENABLE */
        m_stpLogicDrv->WriteBit(uiReg, uiEnableBit, 0);
    } else {
        /* 1 - DISABLE */
        /* 0 - PUMP ON */
        m_stpLogicDrv->WriteBit(uiReg, uiEnableBit, 1);
        m_stpLogicDrv->WriteBit(uiReg, uiPumpBit, 0);
    }

    return true;
}

bool
CApsd::__GetApsdEnable(uint32_t uiReg, uint32_t uiBit, bool &rbEnable)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }
    /* 0 - ENABLE */
    m_stpLogicDrv->ReadBit(uiReg, uiBit, ucVal);
    rbEnable = ((0 == ucVal) ? true : false);

    return true;
}

bool
CApsd::__GetApsdStatus(uint32_t uiReg, uint32_t uiBit, bool &rbStatus)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    /* 1 - PUMP ShutDown APSD occur */
    m_stpLogicDrv->ReadBit(uiReg, uiBit, ucVal);
    rbStatus = ((1 == ucVal) ? true : false);

    return true;
}

bool
CApsd::__GetApsdAlarm(uint32_t uiReg, uint32_t uiBit, bool &rbAlarm)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    m_stpLogicDrv->ReadBit(uiReg, uiBit, ucVal);
    rbAlarm = ((0 == ucVal) ? true : false);

    return true;
}


