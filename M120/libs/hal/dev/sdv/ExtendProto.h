/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendProto.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "RspHandler.h"

/*************************************************/

enum EExtendProto
{
    EXTEND_RSP_MAX_LEN = 1024,
    EXTEND_READ_NUM    = 500,

};


class CExtendSwProto
{
public:
    CExtendSwProto()
    {
    }
    virtual
    ~CExtendSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg);
    virtual void GetMfgCmd(std::string &rstaMsg);
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg);
    virtual void GetStatusData(std::string &rstaMsg);

    /* SWITCH */
    virtual void SetSwitchPos(uint32_t ulId, std::string &rstaMsg, uint32_t ulPos);

    /* VOA */
    virtual void SetVoaAttn(uint32_t ulId, std::string &rstaMsg, uint32_t ulAttn);


    /* VOA Mode */
    virtual void SetVoaMode(uint32_t ulId, std::string &rstaMsg, uint32_t ulMode);

    /* SFP */
    virtual void SetSfpEnable(uint32_t ulId, std::string &rstaMsg, uint32_t ulEnable);

    /* Reset */
    virtual void SoftReset(std::string &rstaMsg);

    /* Upgard */
    virtual void Upgrade(std::string &rstaMsg);
};


class CExtendK20SwProto : public CExtendSwProto
{
public:
    /* Upgard */
    virtual void Upgrade(std::string &rstaMsg);
};




