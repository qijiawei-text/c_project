/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TdcmProto.h
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

/*************************************************/

enum ETdcmProto
{
    TDCM_RSP_MAX_LEN = 1024,
    TDCM_READ_NUM    = 500,


};

class CTdcmSwProto
{
public:
    CTdcmSwProto()
    {
    }
    virtual
    ~CTdcmSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetStatusData(std::string &rstaMsg) = 0;

};

class CTdcmSwFrameProto : public CTdcmSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;

    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;

};










