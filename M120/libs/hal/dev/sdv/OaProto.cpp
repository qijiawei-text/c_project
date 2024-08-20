
/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaProto.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaProto.h"
#include "StringTool.h"

/*==============================function=========================*/


bool
COaSwCliProto::ProcRsp(std::string &rstaMsg)
{
    LOG_WRITE(EN_LOG_TRACE, "%lu %s", rstaMsg.size(), (char *)rstaMsg.c_str());
    if (0 == strlen((char *)rstaMsg.c_str())) {
        return false;
    }

    /* find if has '>' */
    if (rstaMsg.find(">") == std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Not found >.");
        return false;
    }

    if (rstaMsg.find("Unknow") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Find Unknow.");
        return false;
    }

    /* check if error res message */
    if (rstaMsg.find("invalid") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Find invalid.");
        return false;
    }

    if (rstaMsg.find("missing") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Find missing.");
        return false;
    }

    if (rstaMsg.find("out of rang") != std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "Find out of rang.");
        return false;
    }

    return true;

}

bool
COaSwCliProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    return true;
}


void
COaSwCliProto::GetMfgCmd(std::string &rstaMsg)
{
    rstaMsg = "privinfo\r\n";

}


bool
COaSwCliProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{

    /* manufacture info */
    sprintf(rstMfg.acManufacturer, "%s", "Molex");

    /* module type */
    if (!CStringTool::SubStr(rstMfg.acModuleType, c_rstaMsg, "Configuration: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Configuration not found.");
        return false;
    }

    /* fw version */
    if (!CStringTool::SubStr(rstMfg.acFwVer, c_rstaMsg, "Firmware Vers: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Firmware Vers not found.");
        return false;
    }

    if (0 == strlen(rstMfg.acFwVer)) {
        LOG_WRITE(EN_LOG_ERROR, "acFwVer strLen is zero");
        return false;
    }

    /* boot version */
    if (!CStringTool::SubStr(rstMfg.acBootVer, c_rstaMsg, "IPL version: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "IPL version not found.");
        return false;
    }

    /* HW version */
    if (!CStringTool::SubStr(rstMfg.acHwVer, c_rstaMsg, "HW version: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "HW version not found.");
        return false;
    }

    /* SN */
    if (!CStringTool::SubStr(rstMfg.acSN, c_rstaMsg, "Serial Number: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Serial Number not found.");
        return false;
    }

    /* PN */
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, "Part Number: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Part Number not found.");
        return false;
    }

    /* Product Name */
    sprintf(rstMfg.acProductName, "%s", "EDFA");

    /* FPGA ver */
    if (!CStringTool::SubStr(rstMfg.acFpgaRunVer, c_rstaMsg, "PLD version: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "PLD version not found.");
        return false;
    }

    /* MAN_DATE */
    if (!CStringTool::SubStr(rstMfg.acMfgDate, c_rstaMsg, "MAN_DATE: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "MAN_DATE: not found.");
    }

    /* CAL_DATE */
    if (!CStringTool::SubStr(rstMfg.acCaliDate, c_rstaMsg, "CAL_DATE: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "CAL_DATE: not found.");
    }


    return true;

}

void
COaSwCliProto::GetStatusData(std::string &rstaMsg)
{
    rstaMsg = "allinfo\r\n";

}

bool
COaSwCliProto::GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    return true;
}

void
COaSwCliProto::GetDebugInfoCmd(std::string &rstaMsg)
{
    rstaMsg = "DBGINFO\r\n";
    LOG_WRITE(EN_LOG_NOTICE, "rstaMsg = %s", rstaMsg.c_str());
}


void
COaSwCliProto::GetMcuDebugInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx)
{
    return;
}

void
COaSwCliProto::GetFpgaInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx)
{
    return;
}

/* ---------------------------Singl Proto-------------------------------------*/


void
COaSwCliSingleProto::SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange)
{

    rstaMsg = "gtype " + std::to_string(uiGainRange) + "\r\n";

}

void
COaSwCliSingleProto::SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain)
{

    float fData = (float)uiGain / 100;

    rstaMsg = "mode g " + std::to_string(fData) + "\r\n";

}

void
COaSwCliSingleProto::GetGain(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "mode\r\n";
}

bool
COaSwCliSingleProto::GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain)
{
    std::string staGain;
    std::string staStartStr = "MODE: G ";
    if (!CStringTool::SubStr((char *)staGain.c_str(), c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    rsGain = std::stof(staGain) * 100;

    return true;
}


void
COaSwCliSingleProto::SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt)
{
    float fData = (float)sTilt / 100;

    rstaMsg = "tilt " + std::to_string(fData) + "\r\n";
}

void
COaSwCliSingleProto::GetTilt(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "tilt\r\n";
}

bool
COaSwCliSingleProto::GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt)
{
    std::string staTilt;
    std::string staStartStr = "TILT: ";
    if (!CStringTool::SubStr((char *)staTilt.c_str(), c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    rsTilt = std::stof(staTilt) * 100;

    return true;
}

void
COaSwCliSingleProto::SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal)
{
    float fData = (float)sVal / 100;
    if (2 == ucMode) {
        rstaMsg = "mode p " + std::to_string(fData) + "\r\n";
    } else {
        rstaMsg = "mode g " + std::to_string(fData) + "\r\n";
    }
}


void
COaSwCliSingleProto::SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr)
{
    float fData = (float)sThr / 100;

    rstaMsg = "alrm los thr " + std::to_string(fData) + "\r\n";
}

void
COaSwCliSingleProto::SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys)
{
    float fData = (float)sHys / 100;

    rstaMsg = "alrm los hys " + std::to_string(fData) + "\r\n";
}

void
COaSwCliSingleProto::SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliSingleProto::GetRxLosThr(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "alrm los\r\n";
}

bool
COaSwCliSingleProto::GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys)
{
    std::string staThr, staHys;
    if (!CStringTool::SubStr((char *)staThr.c_str(), c_rstaMsg, "THR: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "THR not found.");
        return false;
    }

    rsThr = std::stof(staThr) * 100;

    if (!CStringTool::SubStr((char *)staHys.c_str(), c_rstaMsg, "HYS: ", "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "HYS not found.");
        return false;
    }

    rsHys = std::stof(staHys) * 100;

    return true;

}


void
COaSwCliSingleProto::SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = bEnable ? "off" : "on";

    rstaMsg = "dis " + staEnable + "\r\n";
}

void
COaSwCliSingleProto::GetPumpEnable(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "dis\r\n";
}

bool
COaSwCliSingleProto::GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{
    char acEnable[DEF_BUF_SIZE_16] = {0};
    std::string staStartStr = "DIS: ";
    if (!CStringTool::SubStr(acEnable, c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    if ( 0 == strcmp(acEnable, "OFF")) {
        rbEnable = true;
    } else if (0 == strcmp(acEnable, "ON")) {
        rbEnable = false;
    } else {
        return false;
    }

    return true;
}


void
COaSwCliSingleProto::SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = (bEnable ? "e" : "n");

    rstaMsg = "rflm " + staEnable + "\r\n";
}

void
COaSwCliSingleProto::SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = bEnable ? "a" : "n";

    rstaMsg = "los " + staEnable + "\r\n";
}

void
COaSwCliSingleProto::GetAutolosEnable(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "los\r\n";
}


bool
COaSwCliSingleProto::GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{
    char acEnable[DEF_BUF_SIZE_16] = {0};
    std::string staStartStr = "LOS: ";
    if (!CStringTool::SubStr(acEnable, c_rstaMsg, staStartStr, "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    if ( 0 == strcmp(acEnable, "A")) {
        rbEnable = true;
    } else if (0 == strcmp(acEnable, "N")) {
        rbEnable = false;
    } else {
        return false;
    }

    return true;
}


void
COaSwCliSingleProto::SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn)
{

    float fData = (float)uiAttn / 100;

    /* rstaMsg = "voa 0 set " + std::to_string(fData) + "\r\n"; */
    rstaMsg = "voa " + std::to_string(uiId) + " set " + std::to_string(fData) + "\r\n";
}

void
COaSwCliSingleProto::SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget)
{

    float fData = (float)uiTarget / 100;

    /* rstaMsg = "voa 0 set " + std::to_string(fData) + "\r\n"; */
    rstaMsg = "voa " + std::to_string(uiId) + " set " + std::to_string(fData) + "\r\n";
}

void
COaSwCliSingleProto::GetAttn(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "voa\r\n";
}

bool
COaSwCliSingleProto::GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn)
{
    std::string staAttn;
    std::string staStartStr = "VOA SET: ";
    if (!CStringTool::SubStr((char *)staAttn.c_str(), c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    rsAttn = std::stof(staAttn) * 100;

    return true;
}

bool
COaSwCliSingleProto::SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block)
{
    /* TODO */
    return false;
}

bool
COaSwCliSingleProto::GetShutterState(uint32_t uiId, std::string &rstaMsg)
{
    /* TODO */
    return false;
}

bool
COaSwCliSingleProto::GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block)
{
    /* TODO */
    return false;
}

void
COaSwCliSingleProto::SoftReset(uint32_t uiId, std::string &rstaMsg)
{

    rstaMsg = "boot\r\n";
}

void
COaSwCliSingleProto::Upgrade(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "upgdfw\r\n";
}


void
COaSwCliSingleProto::Swap(uint32_t uiId, std::string &rstaMsg)
{

    rstaMsg = "swab\r\n";
}

void
COaSwCliSingleProto::Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg)
{

}

void
COaSwCliSingleProto::Verify(uint32_t uiId, std::string &rstaMsg)
{

}

void
COaSwCliSingleProto::Commit(uint32_t uiId, std::string &rstaMsg)
{

}

void
COaSwCliSingleProto::GetAlarmData(std::string &rstaMsg)
{

}

bool
COaSwCliSingleProto::GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    return true;
}

void
COaSwCliSingleProto::SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower)
{
}
void
COaSwCliSingleProto::SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}
void
COaSwCliSingleProto::SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos)
{
}
void
COaSwCliSingleProto::SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliSingleProto::SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliSingleProto::SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliSingleProto::SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}
void
COaSwCliSingleProto::SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}
void
COaSwCliSingleProto::SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}
void
COaSwCliSingleProto::SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}

void
COaSwCliSingleProto::GetOtdrCaliCmd(std::string &rstaMsg)
{
}

bool
COaSwCliSingleProto::GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstMfg)
{
    return true;
}

void
COaSwCliSingleProto::GetOcmCaliCmd(std::string &rstaMsg)
{
}

bool
COaSwCliSingleProto::GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstMfg)
{
    return true;
}

void
COaSwCliSingleProto::SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali)
{
}

void
COaSwCliSingleProto::SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali)
{
}
void
COaSwCliSingleProto::SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}
void
COaSwCliSingleProto::SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}
void
COaSwCliSingleProto::SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}
void
COaSwCliSingleProto:: SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliSingleProto::SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent)
{
}

void
COaSwCliSingleProto::SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal)
{
}

void
COaSwCliSingleProto::GetEventLogCmd(std::string &rstaMsg)
{
}

bool
COaSwCliSingleProto::GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstMfg)
{
    return true;
}

void
COaSwCliSingleProto::SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}

void
COaSwCliSingleProto::SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode)
{
}

bool
COaSwCliSingleProto::SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

bool
COaSwCliSingleProto::SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

bool
COaSwCliSingleProto::SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

void
COaSwCliSingleProto::SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}

void
COaSwCliSingleProto::SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}

/* ---------------------------Mult Proto-------------------------------------*/

void
COaSwCliMultProto::SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange)
{

    rstaMsg = "sgtype " + std::to_string(uiId + 1) + " " + std::to_string(uiGainRange) + "\r\n";

}

void
COaSwCliMultProto::SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain)
{

    float fData = (float)uiGain / 100;

    rstaMsg = "mode g " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n";
}

void
COaSwCliMultProto::GetGain(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "smode " + std::to_string(uiId + 1) + "\r\n";
}

bool
COaSwCliMultProto::GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain)
{
    char acGain[DEF_BUF_SIZE_32] = {0};
    std::string staStartStr = "SMODE " + std::to_string(uiId + 1) + ": G ";

    if (!CStringTool::SubStr(acGain, c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    std::string staGain(acGain);
    std::size_t nPos = staGain.find(std::string("."));
    if (nPos != std::string::npos) {
        staGain.erase(nPos, 1);
    }

    rsGain = std::stof(staGain);
    return true;
}


void
COaSwCliMultProto::SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt)
{
    float fData = (float)sTilt / 100;

    rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n";
}

void
COaSwCliMultProto::GetTilt(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "tilt\r\n";
}

bool
COaSwCliMultProto::GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt)
{
    char acTilt[DEF_BUF_SIZE_32] = {0};
    std::string staStartStr = "TILT: ";

    if (!CStringTool::SubStr(acTilt, c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    std::string staTilt(acTilt);
    std::size_t nPos = staTilt.find(std::string("."));
    if (nPos != std::string::npos) {
        staTilt.erase(nPos, 1);
    }

    rsTilt = std::stof(staTilt);

    return true;
}


void
COaSwCliMultProto::SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal)
{
    float fData = (float)sVal / 100;
    if (1 == ucMode) {
        rstaMsg = "mode " + std::to_string(uiId) + " p " + std::to_string(fData) + "\r\n";
    } else {
        rstaMsg = "mode " + std::to_string(uiId) + " g " + std::to_string(fData) + "\r\n";
    }
}

void
COaSwCliMultProto::SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr)
{
    float fData = (float)sThr / 100;

    rstaMsg = "alrm los" + std::to_string(uiId + 1) + " thr " + std::to_string(fData) + "\r\n";
}

void
COaSwCliMultProto::SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys)
{
    float fData = (float)sHys / 100;

    rstaMsg = "alrm los" + std::to_string(uiId + 1) + " hys " + std::to_string(fData) + "\r\n";
}

void
COaSwCliMultProto::SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{

}

void
COaSwCliMultProto::GetRxLosThr(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "alrm los" + std::to_string(uiId + 1) + "\r\n";
}

bool
COaSwCliMultProto::GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys)
{
    char acThr[DEF_BUF_SIZE_64];
    char acHys[DEF_BUF_SIZE_64];
    std::string staThrStartStr = "ALRM LOS" + std::to_string(uiId + 1) + " THR: ";
    std::string staHysStartStr = "ALRM LOS" + std::to_string(uiId + 1) + " HYS: ";

    if (!CStringTool::SubStr(acThr, c_rstaMsg, staThrStartStr.c_str(), "\r", DEF_BUF_SIZE_64 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staThrStartStr.c_str());
        return false;
    }

    std::string staThr(acThr);
    std::size_t nPos = staThr.find(std::string("."));
    if (nPos != std::string::npos) {
        staThr.erase(nPos, 1);
    }
    rsThr = std::stof(staThr);

    if (!CStringTool::SubStr(acHys, c_rstaMsg, staHysStartStr, "\r", DEF_BUF_SIZE_64 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staHysStartStr.c_str());
        return false;
    }

    std::string staHys(acHys);
    nPos = staHys.find(std::string("."));
    if (nPos != std::string::npos) {
        staHys.erase(nPos, 1);
    }

    rsHys = std::stof(staHys);

    return true;

}



void
COaSwCliMultProto::SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = bEnable ? "off" : "on";

    rstaMsg = "sdis " + std::to_string(uiId + 1) + " " + staEnable + "\r\n";
}

void
COaSwCliMultProto::GetPumpEnable(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "sdis " + std::to_string(uiId + 1) + "\r\n";
}

bool
COaSwCliMultProto::GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{
    char acEnable[DEF_BUF_SIZE_16] = {0};
    std::string staStartStr = "SDIS " + std::to_string(uiId + 1) + ": ";
    if (!CStringTool::SubStr(acEnable, c_rstaMsg, staStartStr, "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    if ( 0 == strcmp(acEnable, "OFF")) {
        rbEnable = true;
    } else if (0 == strcmp(acEnable, "ON")) {
        rbEnable = false;
    } else {
        return false;
    }

    return true;

}



void
COaSwCliMultProto::SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = (bEnable ? "e" : "n");

    rstaMsg = "srflm " + std::to_string(uiId + 1) + " " + staEnable + "\r\n";
}

void
COaSwCliMultProto::SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = bEnable ? "a" : "n";

    rstaMsg = "slos " + std::to_string(uiId + 1) + " " + staEnable + "\r\n";

    LOG_WRITE(EN_LOG_NOTICE, "rstaMsg = %s", rstaMsg.c_str());
}

void
COaSwCliMultProto::GetAutolosEnable(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "slos\r\n";
}


bool
COaSwCliMultProto::GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{
    char acEnable[DEF_BUF_SIZE_16] = {0};
    std::string staStartStr = "SLOS " + std::to_string(uiId + 1) + ": ";
    if (!CStringTool::SubStr(acEnable, c_rstaMsg, staStartStr, "\r", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    if ( 0 == strcmp(acEnable, "A")) {
        rbEnable = true;
    } else if (0 == strcmp(acEnable, "N")) {
        rbEnable = false;
    } else {
        return false;
    }

    return true;
}

void
COaSwCliMultProto::SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn)
{
    float fData = (float)uiAttn / 100;

    rstaMsg = "voa " + std::to_string(uiId + 1) + " set " + std::to_string(fData) + "\r\n";


    LOG_WRITE(EN_LOG_NOTICE, "Send1: %s", rstaMsg.c_str());
}

void
COaSwCliMultProto::SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget)
{
    float fData = (float)uiTarget / 100;

    rstaMsg = "voa " + std::to_string(uiId + 1) + " set " + std::to_string(fData) + "\r\n";


    LOG_WRITE(EN_LOG_NOTICE, "Send1: %s", rstaMsg.c_str());
}

void
COaSwCliMultProto::GetAttn(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "voa " + std::to_string(uiId + 1) + "\r\n";
}

bool
COaSwCliMultProto::GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn)
{
    char acAttn[DEF_BUF_SIZE_32] = {0};
    std::string staStartStr = "VOA " + std::to_string(uiId + 1) + " SET: ";

    if (!CStringTool::SubStr(acAttn, c_rstaMsg, staStartStr, "\r", DEF_BUF_SIZE_32 - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "%s not found.", staStartStr.c_str());
        return false;
    }

    std::string staAttn(acAttn);
    std::size_t nPos = staAttn.find(std::string("."));
    if (nPos != std::string::npos) {
        staAttn.erase(nPos, 1);
    }

    rsAttn = std::stof(staAttn);

    return true;
}


void
COaSwCliMultProto::SoftReset(uint32_t uiId, std::string &rstaMsg)
{

    rstaMsg = "boot\r\n";
}

void
COaSwCliMultProto::Upgrade(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "upgdfw\r\n";
}


void
COaSwCliMultProto::Swap(uint32_t uiId, std::string &rstaMsg)
{

    rstaMsg = "swab\r\n";
}

void
COaSwCliMultProto::Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg)
{

}

void
COaSwCliMultProto::Verify(uint32_t uiId, std::string &rstaMsg)
{

}

void
COaSwCliMultProto::Commit(uint32_t uiId, std::string &rstaMsg)
{

}

void
COaSwCliMultProto::GetAlarmData(std::string &rstaMsg)
{

}

bool
COaSwCliMultProto::GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    return true;
}

void
COaSwCliMultProto::SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower)
{
}

void
COaSwCliMultProto::SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}

void
COaSwCliMultProto::SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos)
{
}

void
COaSwCliMultProto::SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}

void
COaSwCliMultProto::SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}

void
COaSwCliMultProto::SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
}
void
COaSwCliMultProto::SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}
void
COaSwCliMultProto::SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}
void
COaSwCliMultProto::SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}
void
COaSwCliMultProto::SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}

void
COaSwCliMultProto::GetOtdrCaliCmd(std::string &rstaMsg)
{
}

bool
COaSwCliMultProto::GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstMfg)
{
    return true;
}

void
COaSwCliMultProto::GetOcmCaliCmd(std::string &rstaMsg)
{
}

bool
COaSwCliMultProto::GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstMfg)
{
    return true;
}

void
COaSwCliMultProto::SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali)
{
}

void
COaSwCliMultProto::SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali)
{
}

void
COaSwCliMultProto::SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}

void
COaSwCliMultProto::SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}

void
COaSwCliMultProto::SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
}

void
COaSwCliMultProto:: SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys)
{
}

void
COaSwCliMultProto::SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent)
{
}

void
COaSwCliMultProto::SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal)
{
}

void
COaSwCliMultProto::GetEventLogCmd(std::string &rstaMsg)
{
}

bool
COaSwCliMultProto::GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstMfg)
{
    return true;
}

void
COaSwK20Proto::Upgrade(uint32_t uiId, std::string &rstaMsg)
{
    rstaMsg = "recv fw\r\n";
    LOG_WRITE(EN_LOG_NOTICE, "%s", rstaMsg.c_str());
}


bool
COaSwCliMultProto::SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block)
{
    /* TODO */
    return false;
}

bool
COaSwCliMultProto::GetShutterState(uint32_t uiId, std::string &rstaMsg)
{
    /* TODO */
    return false;
}

bool
COaSwCliMultProto::GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block)
{
    /* TODO */
    return false;
}

void
COaSwCliMultProto::SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{
}

void
COaSwCliMultProto::SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode)
{
}

bool
COaSwCliMultProto::SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

bool
COaSwCliMultProto::SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

bool
COaSwCliMultProto::SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /* TODO */
    return false;
}

void
COaSwCliMultProto::SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}

void
COaSwCliMultProto::SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
}

