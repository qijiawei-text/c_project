/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      HalApi.h
   Author:        Zhen Zhang
   Date:          2018-11-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-05] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class CHalApi : public CDevApi
{
public:
    CHalApi()
    {
    }
    ~CHalApi()
    {
    }

    int32_t RegisterDev(const CRegisterDev &c_rstRegister);
    int32_t UnregisterDev(uint32_t uiDevType, uint32_t uiDevIdx);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_HAL_STA,
          SHM_SEM_KEY_HAL_STA,
          SHM_SIZE_HAL_STA},

         {SHM_KEY_HAL_CMD,
          SHM_SEM_KEY_HAL_CMD,
          SHM_SIZE_HAL_CMD},

         {SHM_KEY_HAL_DATA,
          SHM_SEM_KEY_HAL_DATA,
          SHM_SIZE_HAL_DATA}}
    };

};






