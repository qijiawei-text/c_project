/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Driver.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Pam4Driver.h"

/*==============================function=========================*/

bool
CPam4I2cDriver::IsOnline(void)
{

    return true;
}

bool
CPam4I2cDriver::GetMfg(CMfg &rstMfg)
{

    /* if ((nullptr == m_stpI2cPtr) || (nullptr == m_stpProtoPtr)) */
    /* { */
    /* return false; */
    /* } */

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);
    /*
        m_stpUartDrvPtr->Write(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size());

        std::string staRsp(OA_CLI_RSP_MAX_LEN, 0);
        m_stpUartDrvPtr->Read(m_uiHwIdx, (uint8_t *)staRsp.data(), OA_READ_NUM);

        if (m_stpProtoPtr->ProcRsp(staRsp))
        {
            m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg);

            return true;
        }

        return false;*/



    sprintf(rstMfg.acSN, "%s", "SN:123456");
    sprintf(rstMfg.acPN, "%s", "PN:abcd");
    sprintf(rstMfg.acModuleType, "%s", "PAM4");
    sprintf(rstMfg.acBootVer, "0x%02X%02X", 0x12, 0x34);
    sprintf(rstMfg.acFwVer, "0x%02X%02X", 0x1, 0x2);
    sprintf(rstMfg.acHwVer, "%d.%d.%d", 1, 1, 1);
    sprintf(rstMfg.acCaliDate, "%02d%02d.%d.%d", 20, 18, 11, 22);

    return true;

}


bool
CPam4I2cDriver::GetStatusData(CPam4StatusData &rstData)
{
    CDelay::Delay(1, S);

    return true;
}

bool
CPam4I2cDriver::GetCfgData(CPam4CfgData &rstData)
{

    return true;
}


