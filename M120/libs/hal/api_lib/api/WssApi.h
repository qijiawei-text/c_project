/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssApi.h
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

class CWssApi : public CDevApi
{
public:
    CWssApi()
    {
    }
    ~CWssApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, uint32_t uiSubIdx, CWssStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CWssAllCfg &rstCfg);
    int32_t GetOscCfgData(uint32_t uiDevIdx, CWssAllCfg &rstCfg);

    int32_t SetAllChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg);
    int32_t SetOscChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg);
    int32_t SetWavePlan(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan);

    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);
    int32_t GetDataForUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
    int32_t SetDataForUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
    int32_t GetDataForPmUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
    int32_t SetDataForPmUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);

    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_WSS_STA,
                                                            SHM_SEM_KEY_WSS_STA,
                                                            SHM_SIZE_WSS_STA},

                                                           {SHM_KEY_WSS_CMD,
                                                            SHM_SEM_KEY_WSS_CMD,
                                                            SHM_SIZE_WSS_CMD},

                                                           {SHM_KEY_WSS_DATA,
                                                            SHM_SEM_KEY_WSS_DATA,
                                                            SHM_SIZE_WSS_DATA}}};


};






