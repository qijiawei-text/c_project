/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DgeDriver.h"

/*==============================function=========================*/

bool
CDgeFpgaDriver::IsOnline(void)
{

    return true;
}

bool
CDgeFpgaDriver::GetMfg(CMfg &rstMfg)
{

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    /* std::string staMsg; */
    /* m_stpProtoPtr->GetMfgCmd(staMsg); */
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
    sprintf(rstMfg.acModuleType, "%s", "DGE");
    sprintf(rstMfg.acBootVer, "0x%02X%02X", 0x12, 0x34);
    sprintf(rstMfg.acFwVer, "0x%02X%02X", 0x1, 0x2);
    sprintf(rstMfg.acHwVer, "%d.%d.%d", 1, 1, 1);
    sprintf(rstMfg.acCaliDate, "%02d%02d.%d.%d", 20, 18, 11, 22);

    return true;

}

bool
CDgeFpgaDriver::GetStatusData(CDgeStatusData &rstData)
{



    return true;

}

bool
CDgeFpgaDriver::SetCurve(uint32_t uiPointCnt, CDgePoint *pstPoint)
{



    return true;

}



