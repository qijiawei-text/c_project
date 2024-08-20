/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsApi.h
   Author:        Guo Guangjun
   Date:          2024-01-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-05] [1.0] [Creator]

*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class COcsApi : public CDevApi
{
public:
    COcsApi()
    {
    }
    ~COcsApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t Commit(uint32_t uiDevIdx);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);

    int32_t GetStatusData(uint32_t uiDevIdx, COcsStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, COcsCfgData &rstCfg);

    int32_t GetSwitchChannels(uint32_t uiDevIdx, COcsConnections &rstConns);
    int32_t SetSwitchChannels(uint32_t uiDevIdx, const COcsConnections &rstConns);
    int32_t SetSwitchChannel(uint32_t uiDevIdx, const COcsConnection &rstConn);

    int32_t GetLedState(uint32_t uiDevIdx, COcsLedSta &rstLedSta);
    int32_t SetLampTestState(uint32_t uiDevIdx, bool bEnable);

    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OCS_STA,
                                                            SHM_SEM_KEY_OCS_STA,
                                                            SHM_SIZE_OCS_STA},

                                                           {SHM_KEY_OCS_CMD,
                                                            SHM_SEM_KEY_OCS_CMD,
                                                            SHM_SIZE_OCS_CMD},

                                                           {SHM_KEY_OCS_DATA,
                                                            SHM_SEM_KEY_OCS_DATA,
                                                            SHM_SIZE_OCS_DATA}}};


};






