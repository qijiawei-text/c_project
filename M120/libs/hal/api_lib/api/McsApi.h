/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      McsApi.h
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

class CMcsApi : public CDevApi
{
public:
    CMcsApi()
    {
    }
    ~CMcsApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, CMcsStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CMcsCfgData &rstCfg);

    int32_t SetPortSwPos(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiPos);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_MCS_STA,
                                                            SHM_SEM_KEY_MCS_STA,
                                                            SHM_SIZE_MCS_STA},

                                                           {SHM_KEY_MCS_CMD,
                                                            SHM_SEM_KEY_MCS_CMD,
                                                            SHM_SIZE_MCS_CMD},

                                                           {SHM_KEY_MCS_DATA,
                                                            SHM_SEM_KEY_MCS_DATA,
                                                            SHM_SIZE_MCS_DATA}}};


};






