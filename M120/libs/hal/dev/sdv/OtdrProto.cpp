
/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrProto.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "OtdrProto.h"
#include "StringTool.h"

/*==============================function=========================*/

bool
COtdrAcclinkSwProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    LOG_WRITE(EN_LOG_DEBUG, "%s", rstaMsg.c_str());

    /* find if has '>' */
    if (rstaMsg.find("ERR0") == std::string::npos) {
        LOG_WRITE(EN_LOG_ERROR, "%s", rstaMsg.c_str());
        return false;
    }

    return true;
}

void
COtdrAcclinkSwProto::GetMfgCmd(std::string & rstaMsg)
{
    rstaMsg = "VER?\r\n";
}

std::vector<std::string>
__Split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    uint32_t size = str.size();

    for (uint32_t i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if ( pos < size ) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}



bool
COtdrAcclinkSwProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{

#if 0
    char acHead[32];
    char acData[256];

    memset(acHead, 0, sizeof(acHead));
    memset(acData, 0, sizeof(acData));

    if (2 != sscanf(c_rstaMsg.data(), "%s %s", acHead, acData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetMfgRsp acHead error");
        return false;
    }

    if (6 != sscanf(acData, "%[^','],%[^','],%[^','],%[^','],%[^','],%s",
                    rstMfg.acManufacturer,
                    rstMfg.acPN,
                    rstMfg.acSN,
                    rstMfg.acEth0Mac,
                    rstMfg.acHwVer,
                    rstMfg.acFwVer)) {
        LOG_WRITE(EN_LOG_ERROR, "GetMfgRsp acData error");
        return false;
    }


#endif


    std::size_t foundStart = c_rstaMsg.find("<VER");
    std::size_t foundEnd = c_rstaMsg.find(">");

    if (foundStart == std::string::npos || foundEnd == std::string::npos) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "foundStart: %lu, foundEnd = %lu", foundStart, foundEnd);

    LOG_WRITE(EN_LOG_NOTICE, "c_rstaMsg: %s, size = %lu", c_rstaMsg.c_str(), c_rstaMsg.size());

    std::string staMfg = c_rstaMsg.substr(foundStart + 5, foundEnd - 7);

    LOG_WRITE(EN_LOG_NOTICE, "staMfg: %s", staMfg.c_str());
    std::vector<std::string> staResult = __Split(staMfg, ",");

    if (staResult.size() == 6) {
        strncpy(rstMfg.acManufacturer, staResult[0].c_str(), sizeof(rstMfg.acManufacturer));
        strncpy(rstMfg.acPN, staResult[1].c_str(), sizeof(rstMfg.acPN));
        strncpy(rstMfg.acSN, staResult[2].c_str(), sizeof(rstMfg.acSN));
        strncpy(rstMfg.acEth0Mac, staResult[3].c_str(), sizeof(rstMfg.acEth0Mac));
        strncpy(rstMfg.acHwVer, staResult[4].c_str(), sizeof(rstMfg.acHwVer));
        strncpy(rstMfg.acFwVer, staResult[5].c_str(), sizeof(rstMfg.acFwVer));
    } else {
        return false;
    }

    strcpy(rstMfg.acModuleType, "ACCELINK OTDR");

    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

    return true;
}

void
COtdrAcclinkSwProto::GetCfgCmd(std::string &rstaMsg)
{
    return;
}

bool
COtdrAcclinkSwProto::GetCfgRsp(const std::string &c_rstaMsg, COtdrCfgData &rstCfg)
{
    return true;
}

void
COtdrAcclinkSwProto::SetCfgCmd(std::string &rstaMsg, COtdrCfgData &rstCfg)
{
    return;
}


void
COtdrAcclinkSwProto::SetPara(std::string &rstaMsg, uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution)
{
    rstaMsg = "SPARA "
              + std::to_string(ulDistRange)
              + "," + std::to_string(ulPulseWidth)
              + "," + std::to_string(dSamplingResolution)
              + ",0\r\n";

}
void
COtdrAcclinkSwProto::SetUior(std::string &rstaMsg, double dUIOR)
{
    rstaMsg = "UIOR "
              + std::to_string(dUIOR)
              + "\r\n";

}
void
COtdrAcclinkSwProto::SetAcqt(std::string &rstaMsg, uint32_t ulMTime)
{
    rstaMsg = "ACQT "
              + std::to_string(ulMTime)
              + "\r\n";

}
void
COtdrAcclinkSwProto::SetTref(std::string &rstaMsg, double dTREF)
{

    rstaMsg = "TREF "
              + std::to_string(dTREF)
              + "\r\n";


}
void
COtdrAcclinkSwProto::SetTlos(std::string &rstaMsg, double dTLOS)
{

    rstaMsg = "TLOS "
              + std::to_string(dTLOS)
              + "\r\n";


}
void
COtdrAcclinkSwProto::SetTeof(std::string &rstaMsg, uint32_t ulEOF)
{
    rstaMsg = "TEOF "
              + std::to_string(ulEOF)
              + "\r\n";

}
void
COtdrAcclinkSwProto::StartScan(std::string &rstaMsg)
{
    rstaMsg = "SCAN\r\n";

}
void
COtdrAcclinkSwProto::GetState(std::string &rstaMsg)
{
    rstaMsg = "STATE?\r\n";

}
bool
COtdrAcclinkSwProto::GetStateRsp(const std::string &c_rstaMsg, bool &rbState)
{
    if (c_rstaMsg.find("STATE 0") != std::string::npos) {
        rbState = true;
    } else if (c_rstaMsg.find("STATE 1") != std::string::npos) {
        rbState = false;
    } else {
        return false;
    }

    return true;
}
void
COtdrAcclinkSwProto::GetSor(std::string &rstaMsg)
{
    rstaMsg = "GETSOR?\r\n";

}

void
COtdrAcclinkSwProto::GetSorTemp(std::string &rstaMsg, uint32_t uiPackId)
{
    return;
}

void
COtdrAcclinkSwProto::GetSorRsp(const std::string &c_rstaMsg, uint32_t &ruiPackId)
{
    return;
}


bool
COtdrAcclinkSwProto::GetSorHeaderRsp(const std::string &c_rstaMsg, uint32_t &ruiSize)
{
    if (c_rstaMsg.size() < 4) {
        return false;
    }

    ruiSize = ntohl(*(uint32_t *)c_rstaMsg.data());
    LOG_WRITE(EN_LOG_NOTICE, "ruiSize = %d", ruiSize);

    return true;
}

void
COtdrAcclinkSwProto::GetSorBodyRsp(const std::string &c_rstaMsg)
{
    return;
}

void
COtdrAcclinkSwProto::GetWaveform(std::string &rstaMsg)
{
    rstaMsg = "WAVEFORM?\r\n";

}
void
COtdrAcclinkSwProto::GetEventTop(std::string &rstaMsg)
{
    rstaMsg = "EVENT? TOP\r\n";

}

bool
COtdrAcclinkSwProto::GetEventTopRsp(const std::string &c_rstaMsg, COtdrEventTop &rstEventTop)
{
    char acHead[32];
    char acTop[32];
    char acData[64];
    char aacTmp[4][64];

    LOG_WRITE(EN_LOG_NOTICE, "GetEventTopRsp %s", c_rstaMsg.c_str());

    memset(acData, 0, sizeof(acData));
    if (3 != sscanf(c_rstaMsg.data(), "%s %s %s", acHead, acTop, acData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetEventTopRsp acHead error");
        return false;
    }

    memset(aacTmp, 0, sizeof(aacTmp));
    if (4 != sscanf(acData, "%[^','],%[^','],%[^','],%s",
                    aacTmp[0], aacTmp[1], aacTmp[2], aacTmp[3])) {
        LOG_WRITE(EN_LOG_ERROR, "GetEventTopRsp acData error");
        return false;
    }

    rstEventTop.ulCnt = atoi(aacTmp[0]);
    rstEventTop.dFiberLen = atof(aacTmp[1]);
    rstEventTop.dTotalLoss = atof(aacTmp[2]);
    rstEventTop.dReturnLoss = atof(aacTmp[3]);

    LOG_WRITE(EN_LOG_NOTICE, "__GetEventTopRsp -- %d, %f, %f, %f",
              rstEventTop.ulCnt,
              rstEventTop.dFiberLen,
              rstEventTop.dTotalLoss,
              rstEventTop.dReturnLoss);

    return true;

}

void
COtdrAcclinkSwProto::GetEventLine(std::string &rstaMsg, uint32_t ulLine)
{
    rstaMsg = "EVENT? LINE "
              + std::to_string(ulLine)
              + "\r\n";

}

bool
COtdrAcclinkSwProto::GetEventLineRsp(const std::string &c_rstaMsg, COtdrEventLine &rstEvetnLine)
{
    char acHead[32];
    char acLine[32];
    char acData[64];
    char acEventType[4];
    char aacTmp[5][64];

    LOG_WRITE(EN_LOG_NOTICE, "GetEventLineRsp %s", c_rstaMsg.data());

    memset(acData, 0, sizeof(acData));
    if (3 != sscanf(c_rstaMsg.data(), "%s %s %s", acHead, acLine, acData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetEventLineRsp acHead error");
        return false;
    }

    memset(aacTmp, 0, sizeof(aacTmp));
    memset(acEventType, 0, sizeof(acEventType));
    if (6 != sscanf(acData, "%[^','],%[^','],%[^','],%[^','],%[^','],%s",
                    aacTmp[0],
                    aacTmp[1],
                    aacTmp[2],
                    aacTmp[3],
                    aacTmp[4],
                    acEventType)) {
        LOG_WRITE(EN_LOG_ERROR, "GetEventLineRsp acData error");
        return false;
    }

    rstEvetnLine.ulEventId = atoi(aacTmp[0]);
    rstEvetnLine.dLocation = atof(aacTmp[1]);
    rstEvetnLine.dEventLoss = atof(aacTmp[2]);
    rstEvetnLine.dReflectance = atof(aacTmp[3]);
    rstEvetnLine.dTotalLoss = atof(aacTmp[4]);

    if (nullptr != strstr(acEventType, "E")) {
        rstEvetnLine.enEventType = OTDR_EVENT_TYPE_FIBER_END;
    } else if (nullptr != strstr(acEventType, "R")) {
        rstEvetnLine.enEventType = OTDR_EVENT_TYPE_REF;
    } else {
        rstEvetnLine.enEventType = OTDR_EVENT_TYPE_NON_REF;
    }

    return true;
}

void
COtdrAcclinkSwProto::GetEventInfo(std::string &rstaMsg)
{
    return;
}

bool
COtdrAcclinkSwProto::GetEventInfoRsp(const std::string &c_rstaMsg, COtdrStatusData &rstaData)
{
    return true;
}

void
COtdrAcclinkSwProto::GetTemp(std::string & rstaMsg)
{
    return;
}

bool
COtdrAcclinkSwProto::GetTempRsp(const std::string &c_rstaMsg, COtdrTemp &rstaData)
{
    return true;
}

void
COtdrAcclinkSwProto::SoftReset(std::string &rstaMsg)
{
    rstaMsg = "RST\r\n";

}
void
COtdrAcclinkSwProto::UpgStart(std::string &rstaMsg)
{
    rstaMsg = "UPGMODE 1\r\n";

}

void
COtdrAcclinkSwProto::SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    return;
}

void
COtdrAcclinkSwProto::UpgEnd(std::string &rstaMsg)
{
    rstaMsg = "UPGMODE 0\r\n";

}

void
COtdrAcclinkSwProto::GetStartOffsetCmd(std::string &rstaMsg)
{
    return;
}

bool
COtdrAcclinkSwProto::GetStartOffsetRsp(const std::string &c_rstaMsg, COtdrStartOffset &rstCfg)
{
    return true;
}

void
COtdrAcclinkSwProto::SetStartOffsetCmd(std::string &rstaMsg, COtdrStartOffset &rstCfg)
{
    return;
}


