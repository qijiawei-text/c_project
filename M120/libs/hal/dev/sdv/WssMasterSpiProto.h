/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName: WssMasterSpiProto.h
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "WssProto.h"

/*************************************************/



class CWssSwMasterSpiFrameProto : public CWssSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;

    virtual void SetOneChCfg(std::string &rstaMsg, const CWssCfg &c_rstData) override;
};










