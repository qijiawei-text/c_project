/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Proto.h
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

enum EPam4Proto
{
    PAM4_RSP_MAX_LEN = 1024,
    PAM4_READ_NUM    = 500,


};

class CPam4SwProto
{
public:
    CPam4SwProto()
    {
    }
    virtual
    ~CPam4SwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetStatusData(std::string &rstaMsg) = 0;

};

class CPam4SwFrameProto : public CPam4SwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;

    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;

};










