/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:  WssMasterSpiProto.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:


*****************************************************************************************************/

/*=========================include head files====================*/

#include "WssMasterSpiProto.h"


/*==============================function=========================*/



#if 0
bool
CWssSwMasterSpiFrameProto::ProcRsp(std::string &rstaMsg)
{

    /* find if has '>' */
    if (rstaMsg.find(">") == std::string::npos) {
        return false;
    }

    /* check if error res message */
    if (rstaMsg.find("invalid") != std::string::npos) {
        return false;
    }

    if (rstaMsg.find("Unknow") != std::string::npos) {
        return false;
    }

    if (rstaMsg.find("missing") != std::string::npos) {
        return false;
    }

    if (rstaMsg.find("out of rang") != std::string::npos) {
        return false;
    }

    return true;

}
#else
bool
CWssSwMasterSpiFrameProto::ProcRsp(std::string & rstaMsg)
{

    bool bInProgress = false;

    return bInProgress;
}

bool
CWssSwMasterSpiFrameProto::ProcRsp(std::string & rstaMsg, bool &rbInProgress)
{

    bool bRet = false;
    bool bInProgress = false;

    rbInProgress = bInProgress;
    LOG_WRITE(EN_LOG_NOTICE, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bRet;
}

#endif

void
CWssSwMasterSpiFrameProto::GetMfgCmd(std::string &rstaMsg)
{

    rstaMsg = "privinfo\r\n";

}

bool
CWssSwMasterSpiFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{



    return true;

}

void
CWssSwMasterSpiFrameProto::GetStatusData(std::string &rstaMsg)
{

    rstaMsg = "allinfo\r\n";

}

void
CWssSwMasterSpiFrameProto::SetOneChCfg(std::string &rstaMsg, const CWssCfg &c_rstData)
{

    rstaMsg = "gtype " + std::to_string(c_rstData.uiAtten) + "\r\n";

}


