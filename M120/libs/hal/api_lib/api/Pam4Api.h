/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Api.h
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

class CPam4Api : public CDevApi
{
public:
    CPam4Api()
    {
    }
    ~CPam4Api()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, CPam4StatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CPam4CfgData &rstCfg);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_PAM4_STA,
                                                            SHM_SEM_KEY_PAM4_STA,
                                                            SHM_SIZE_PAM4_STA},

                                                           {SHM_KEY_PAM4_CMD,
                                                            SHM_SEM_KEY_PAM4_CMD,
                                                            SHM_SIZE_PAM4_CMD},

                                                           {SHM_KEY_PAM4_DATA,
                                                            SHM_SEM_KEY_PAM4_DATA,
                                                            SHM_SIZE_PAM4_DATA}}};


};






