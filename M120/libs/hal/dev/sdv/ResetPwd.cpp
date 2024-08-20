/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ResetPwd.cpp
   Author:        Zhu Weian
   Date:          2020-10-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "ResetPwd.h"

#if __RAINIER__

enum EResetPwdReg
{
    RESET_PWD_REG = 0x14,
};


#else


enum EResetPwdReg
{
    RESET_PWD_REG = 0x14,
};

#endif


bool
CResetPwd::IsNeed()
{
    uint8_t ucVal = 0;

    m_stpLogicDrv->ReadBit(RESET_PWD_REG, BIT_3, ucVal);
    /* rbReset = ((1 == ucVal) ? true : false); */

    m_stpLogicDrv->WriteBit(RESET_PWD_REG, BIT_3, 1);
    m_stpLogicDrv->WriteBit(RESET_PWD_REG, BIT_3, 0);

    return true;
}


