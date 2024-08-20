/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetType.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "ResetTypeRegister.h"


#ifdef __400GZR__

enum EResetTypeReg
{
    RESET_TYPE_REG = 0x04,
};

#else


enum EResetTypeReg
{
    RESET_TYPE_REG_WRABLE = 0xC4,
    RESET_TYPE_REG = 0x0440,
};

#endif


bool
CResetTypeRegister::SetResetType(uint32_t uiType)
{
    uint32_t uiReg = RESET_TYPE_REG;
    uint8_t ucType = uiType;

    m_stpLogicDrv->WriteReg(RESET_TYPE_REG_WRABLE, 2);
    m_stpLogicDrv->WriteReg(uiReg, ucType);
    usleep(100 * 1000);
    m_stpLogicDrv->WriteReg(RESET_TYPE_REG_WRABLE, 0);
    LOG_WRITE(EN_LOG_DEBUG, "SetResetType, uiType = %d", uiType);

    return true;
}

bool
CResetTypeRegister::GetResetType(uint8_t &rucType)
{
    uint32_t uiReg = RESET_TYPE_REG;
    return m_stpLogicDrv->ReadReg(uiReg, &rucType);
}


