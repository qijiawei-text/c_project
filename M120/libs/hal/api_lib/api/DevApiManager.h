/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevApiManager.h
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
#include "HalApi.h"
#include "SlotApi.h"
#include "OaApi.h"
#include "WssApi.h"
#include "OcmApi.h"
#include "TdcmApi.h"
#include "Pam4Api.h"
#include "OpsApi.h"
#include "DceApi.h"

#include "DgeApi.h"
#include "McsApi.h"
#include "OscApi.h"
#include "OtdrApi.h"
#include "MuxApi.h"
#include "ExtendApi.h"
#include "BoardApi.h"
#include "OcsApi.h"

/*************************************************/

class CDevApiManager
{
public:
    static CDevApiManager &
    GetInstance(void)
    {
        static CDevApiManager s_stDevApiManager;
        return s_stDevApiManager;
    }

    CHalApi &
    GetHalApi(void)
    {
        return ms_stHalApi;
    }
    CSlotApi &
    GetSlotApi(void)
    {
        return ms_stSlotApi;
    }
    COaApi &
    GetOaApi(void)
    {
        return ms_stOaApi;
    }
    CWssApi &
    GetWssApi(void)
    {
        return ms_stWssApi;
    }
    COcmApi &
    GetOcmApi(void)
    {
        return ms_stOcmApi;
    }
    CTdcmApi &
    GetTdcmApi(void)
    {
        return ms_stTdcmApi;
    }
    CPam4Api &
    GetPam4Api(void)
    {
        return ms_stPam4Api;
    }
    COpsApi &
    GetOpsApi(void)
    {
        return ms_stOpsApi;
    }
    CDceApi &
    GetDceApi(void)
    {
        return ms_stDceApi;
    }

    CDgeApi &
    GetDgeApi(void)
    {
        return ms_stDgeApi;
    }
    CMcsApi &
    GetMcsApi(void)
    {
        return ms_stMcsApi;
    }
    COscApi &
    GetOscApi(void)
    {
        return ms_stOscApi;
    }
    COtdrApi &
    GetOtdrApi(void)
    {
        return ms_stOtdrApi;
    }
    CMuxApi &
    GetMuxApi(void)
    {
        return ms_stMuxApi;
    }
    CExtendApi &
    GetExtendApi(void)
    {
        return ms_stExtendApi;
    }
    CBoardApi &
    GetBoardApi(void)
    {
        return ms_stBoardApi;
    }
    COcsApi &
    GetOcsApi(void)
    {
        return ms_stOcsApi;
    }

private:
    CDevApiManager()
    {
    }
    CDevApiManager(const CDevApiManager &) = delete;
    CDevApiManager &operator=(const CDevApiManager &) = delete;

    static CHalApi ms_stHalApi;
    static CSlotApi ms_stSlotApi;
    static COaApi ms_stOaApi;
    static CWssApi ms_stWssApi;
    static COcmApi ms_stOcmApi;
    static CTdcmApi ms_stTdcmApi;
    static CPam4Api ms_stPam4Api;
    static COpsApi ms_stOpsApi;
    static CDceApi ms_stDceApi;

    static CDgeApi ms_stDgeApi;
    static CMcsApi ms_stMcsApi;
    static COscApi ms_stOscApi;
    static COtdrApi ms_stOtdrApi;
    static CBoardApi ms_stBoardApi;
    static CMuxApi ms_stMuxApi;
    static CExtendApi ms_stExtendApi;

    static COcsApi ms_stOcsApi;
};






