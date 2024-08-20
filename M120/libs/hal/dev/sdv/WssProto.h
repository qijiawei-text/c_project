/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssProto.h
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "Proto.h"
/*************************************************/

enum EWssProto
{
    WSS_RSP_MAX_LEN = 1024,
    WSS_READ_NUM    = 500,


};


class CWssSwProto : public CProto
{
public:
    CWssSwProto()
    {
    }
    virtual
    ~CWssSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetStatusData(std::string &rstaMsg) = 0;

    virtual void SetOneChCfg(std::string &rstaMsg, const CWssCfg &c_rstData) = 0;

};

class CWssSwFrameProto : public CWssSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;

    virtual void SetOneChCfg(std::string &rstaMsg, const CWssCfg &c_rstData) override;
};










