/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscApi.h
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

class COscApi : public CDevApi
{
public:
    COscApi()
    {
    }
    ~COscApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, COscStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, COscCfgData &rstCfg);

    int32_t SetLaserEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx = 0);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OSC_STA,
                                                            SHM_SEM_KEY_OSC_STA,
                                                            SHM_SIZE_OSC_STA},

                                                           {SHM_KEY_OSC_CMD,
                                                            SHM_SEM_KEY_OSC_CMD,
                                                            SHM_SIZE_OSC_CMD},

                                                           {SHM_KEY_OSC_DATA,
                                                            SHM_SEM_KEY_OSC_DATA,
                                                            SHM_SIZE_OSC_DATA}}};


};






