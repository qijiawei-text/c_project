/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeApi.h
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class CDgeApi : public CDevApi
{
public:
    CDgeApi()
    {
    }
    ~CDgeApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, CDgeStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CDgeCfgData &rstCfg);

    int32_t SetCurve(uint32_t uiDevIdx, uint32_t uiPointCnt, CDgePoint *pstPoint);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_DGE_STA,
                                                            SHM_SEM_KEY_DGE_STA,
                                                            SHM_SIZE_DGE_STA},

                                                           {SHM_KEY_DGE_CMD,
                                                            SHM_SEM_KEY_DGE_CMD,
                                                            SHM_SIZE_DGE_CMD},

                                                           {SHM_KEY_DGE_DATA,
                                                            SHM_SEM_KEY_DGE_DATA,
                                                            SHM_SIZE_DGE_DATA}}};


};






