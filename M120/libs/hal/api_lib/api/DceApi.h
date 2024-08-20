/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DceApi.h
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

class CDceApi : public CDevApi
{
public:
    CDceApi()
    {
    }
    ~CDceApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, CDceStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CDceCfgData &rstCfg);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_DCE_STA,
                                                            SHM_SEM_KEY_DCE_STA,
                                                            SHM_SIZE_DCE_STA},

                                                           {SHM_KEY_DCE_CMD,
                                                            SHM_SEM_KEY_DCE_CMD,
                                                            SHM_SIZE_DCE_CMD},

                                                           {SHM_KEY_DCE_DATA,
                                                            SHM_SEM_KEY_DCE_DATA,
                                                            SHM_SIZE_DCE_DATA}}};


};






