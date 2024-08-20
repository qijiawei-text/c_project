/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendApi.h
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

class CExtendApi : public CDevApi
{
public:
    CExtendApi()
    {
    }
    ~CExtendApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t uiDevIdx, CExtendStatusData &rstStatus);
    int32_t GetCfgData(uint32_t uiDevIdx, CExtendCfgData &rstCfg);

    int32_t SetSwitchPos(uint32_t ulDevIdx, uint32_t ulPos, uint32_t ulSubIdx);
    int32_t SetSfp(uint32_t ulDevIdx, uint32_t ulVal, uint32_t ulSubIdx = 0);
    int32_t SetVoaAttn(uint32_t ulDevIdx, uint32_t ulVal, uint32_t ulSubIdx);

    int32_t Reset(uint32_t ulDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t ulDevIdx, const char *c_pcFilePath);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);

    /* DBG */
    int32_t DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);


private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_EXTEND_STA,
                                                            SHM_SEM_KEY_EXTEND_STA,
                                                            SHM_SIZE_EXTEND_STA},

                                                           {SHM_KEY_EXTEND_CMD,
                                                            SHM_SEM_KEY_EXTEND_CMD,
                                                            SHM_SIZE_EXTEND_CMD},

                                                           {SHM_KEY_EXTEND_DATA,
                                                            SHM_SEM_KEY_EXTEND_DATA,
                                                            SHM_SIZE_EXTEND_DATA}}};


};






