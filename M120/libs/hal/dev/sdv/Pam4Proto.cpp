/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Proto.cpp
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Pam4Proto.h"

/*==============================function=========================*/

bool
CPam4SwFrameProto::ProcRsp(std::string &rstaMsg)
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

void
CPam4SwFrameProto::GetMfgCmd(std::string &rstaMsg)
{

    rstaMsg = "privinfo\r\n";

}


bool
CPam4SwFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{



    return true;

}


void
CPam4SwFrameProto::GetStatusData(std::string &rstaMsg)
{

    rstaMsg = "allinfo\r\n";

}



