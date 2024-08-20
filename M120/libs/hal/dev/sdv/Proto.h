/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      RspHandler.h
   Author:        Zhu Weian
   Date:          2021-02-12
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2021-02-12] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#define RSP_STATUS_OK    (0)
#define RSP_STATUS_ERROR (1)
#define RSP_STATUS_AGAIN (2)
#define RSP_STATUS_TRY_AGAIN (3)

class CProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) = 0;
};

