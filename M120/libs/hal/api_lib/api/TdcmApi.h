/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TdcmApi.h
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class CTdcmApi : public CDevApi
{
public:
    CTdcmApi()
    {
    }
    ~CTdcmApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t GetStatusData(uint32_t uiDevIdx, CTdcmStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CTdcmCfgData &rstCfg);

    int32_t SetFrequency(uint32_t uiDevIdx, float fFrequency);
    int32_t SetDispersion(uint32_t uiDevIdx, float fDispersion);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_TDCM_STA,
                                                            SHM_SEM_KEY_TDCM_STA,
                                                            SHM_SIZE_TDCM_STA},

                                                           {SHM_KEY_TDCM_CMD,
                                                            SHM_SEM_KEY_TDCM_CMD,
                                                            SHM_SIZE_TDCM_CMD},

                                                           {SHM_KEY_TDCM_DATA,
                                                            SHM_SEM_KEY_TDCM_DATA,
                                                            SHM_SIZE_TDCM_DATA}}};


};






