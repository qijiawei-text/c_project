/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MuxApi.h
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
#include "DevApi.h"

/*************************************************/

class CMuxApi : public CDevApi
{
public:
    CMuxApi()
    {
    }
    ~CMuxApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetCali(uint32_t uiDevIdx, CMuxCaliData &rstCali);
    int32_t GetStatusData(uint32_t uiDevIdx, CMuxStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CMuxCfgData &rstCfg);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_MUX_STA,
                                                            SHM_SEM_KEY_MUX_STA,
                                                            SHM_SIZE_MUX_STA},

                                                           {SHM_KEY_MUX_CMD,
                                                            SHM_SEM_KEY_MUX_CMD,
                                                            SHM_SIZE_MUX_CMD},

                                                           {SHM_KEY_MUX_DATA,
                                                            SHM_SEM_KEY_MUX_DATA,
                                                            SHM_SIZE_MUX_DATA}}};


};






