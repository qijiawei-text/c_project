/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmApi.h
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

class COcmApi : public CDevApi
{
public:
    COcmApi()
    {
    }
    ~COcmApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t Commit(uint32_t uiDevIdx);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);

    int32_t GetStatusData(uint32_t uiDevIdx, COcmStatusData &rstStatus);
    int32_t GetChPower(uint32_t uiDevIdx, uint32_t uiPortId, COcmChPower &rstReport);
    int32_t GetOsaData(uint32_t uiDevIdx, uint32_t uiPortId, COcmOsa &rstOsa);
    int32_t GetCfgData(uint32_t uiDevIdx, COcmCfgData &rstCfg);

    int32_t SetWavePlan(uint32_t uiDevIdx, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan);
    int32_t SetOcmWavePlan(uint32_t uiDevIdx, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan);
    int32_t SetSchedule(uint32_t uiDevIdx, uint32_t uiPortCnt, uint32_t *puiSchedule);
    int32_t SetMode(uint32_t uiDevIdx, uint32_t uiPortCnt, COcmMode *pstMode);
    int32_t GetRealTimeOsa(uint32_t uiDevIdx, uint32_t uiPortId, COcmOsa &rstReport);

    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OCM_STA,
                                                            SHM_SEM_KEY_OCM_STA,
                                                            SHM_SIZE_OCM_STA},

                                                           {SHM_KEY_OCM_CMD,
                                                            SHM_SEM_KEY_OCM_CMD,
                                                            SHM_SIZE_OCM_CMD},

                                                           {SHM_KEY_OCM_DATA,
                                                            SHM_SEM_KEY_OCM_DATA,
                                                            SHM_SIZE_OCM_DATA}}};


};






