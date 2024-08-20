/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssProto.cpp
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "WssProto.h"


/*==============================function=========================*/




bool
CWssSwFrameProto::ProcRsp(std::string &rstaMsg)
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

bool
CWssSwFrameProto::ProcRsp(std::string & rstaMsg, bool &rbInProgress)
{
    bool bRet = true;
    bool bInProgress = false;

    rbInProgress = bInProgress;
    LOG_WRITE(EN_LOG_NOTICE, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bRet;

}

void
CWssSwFrameProto::GetMfgCmd(std::string &rstaMsg)
{

    rstaMsg = "privinfo\r\n";

}

bool
CWssSwFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{



    return true;

}

void
CWssSwFrameProto::GetStatusData(std::string &rstaMsg)
{

    rstaMsg = "allinfo\r\n";

}

void
CWssSwFrameProto::SetOneChCfg(std::string &rstaMsg, const CWssCfg &c_rstData)
{

    rstaMsg = "gtype " + std::to_string(c_rstData.uiAtten) + "\r\n";

}


