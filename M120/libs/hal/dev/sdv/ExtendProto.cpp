
/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendProto.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "ExtendProto.h"
#include "StringTool.h"

/*==============================function=========================*/


bool
CExtendSwProto::ProcRsp(std::string &rstaMsg)
{
    /* LOG_WRITE(EN_LOG_NOTICE, "%d %s", strlen((char *)rstaMsg.c_str()), (char *)rstaMsg.c_str()); */
    if (0 == strlen((char *)rstaMsg.c_str())) {
        LOG_WRITE(EN_LOG_NOTICE, "len = 0.");
        return false;
    }

    /* find if has '>' */
    if (rstaMsg.find(">") == std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "not find >.");
        return false;
    }


    /* check if error res message */
    if (rstaMsg.find("invalid") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "invalid.");
        return false;
    }

    if (rstaMsg.find("Unknow") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Unknow.");
        return false;
    }

    if (rstaMsg.find("missing") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "missing.");
        return false;
    }

    if (rstaMsg.find("out of rang") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "out of rang.");
        return false;
    }


    return true;

}


void
CExtendSwProto::GetMfgCmd(std::string &rstaMsg)
{
    rstaMsg = "INFO\r\n";

}


bool
CExtendSwProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    /* manufacture info */
    sprintf(rstMfg.acManufacturer, "%s", "OPLINK");

    /* module type Name */
    sprintf(rstMfg.acModuleType, "%s", "EXTEND");

    /* product Name */
    sprintf(rstMfg.acProductName, "%s", "EXTEND");

    /* fw version */
    if (!CStringTool::SubStr(rstMfg.acFwVer, c_rstaMsg, "Main Ver: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    /* HW version */
#if 0
    if (!CStringTool::SubStr(rstMfg.acHwVer, c_rstaMsg, "HW ver: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }
#else
    sprintf(rstMfg.acHwVer, "%s", "1.01");
#endif

    /* SN */
    if (!CStringTool::SubStr(rstMfg.acSN, c_rstaMsg, "SN: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    /* PN */
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, "PN: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    /* mfg date */
    if (!CStringTool::SubStr(rstMfg.acMfgDate, c_rstaMsg, "Mfg Date: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    /* cali date */
    if (!CStringTool::SubStr(rstMfg.acCaliDate, c_rstaMsg, "Cal Date: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }


    return true;

}

void
CExtendSwProto::GetStatusData(std::string &rstaMsg)
{
    rstaMsg = "allinfo\r\n";

}



void
CExtendSwProto::SetSwitchPos(uint32_t ulId, std::string &rstaMsg, uint32_t ulPos)
{
    rstaMsg = "SW " + std::to_string(ulId) + " SPOS " + std::to_string(ulPos) + "\r\n";
}


void
CExtendSwProto::SetVoaAttn(uint32_t ulId, std::string &rstaMsg, uint32_t ulAttn)
{
    float fData = (float)ulAttn / 100;

    rstaMsg = "VOA " + std::to_string(ulId) + " SATT " + std::to_string(fData) + "\r\n";
}

void
CExtendSwProto::SetVoaMode(uint32_t ulId, std::string &rstaMsg, uint32_t ulMode)
{
    rstaMsg = "VOA " + std::to_string(ulId) + " SMOD " + std::to_string(ulMode) + "\r\n";
    LOG_WRITE(EN_LOG_NOTICE, "%s", rstaMsg.c_str());
}


void
CExtendSwProto::SetSfpEnable(uint32_t ulId, std::string &rstaMsg, uint32_t ulEnable)
{
    rstaMsg = "SFPSTA " + std::to_string(ulEnable) + "\r\n";
}


void
CExtendSwProto::SoftReset(std::string &rstaMsg)
{

    rstaMsg = "RST\r\n";
}

void
CExtendSwProto::Upgrade(std::string &rstaMsg)
{

    rstaMsg = "upgdfw\r\n";
}

void
CExtendK20SwProto::Upgrade(std::string &rstaMsg)
{
    rstaMsg = "recv fw\r\n";
}


