/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrApi.h
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

class COtdrApi : public CDevApi
{
public:
    COtdrApi()
    {
    }
    ~COtdrApi()
    {
    }

    int32_t GetMfg(uint32_t ulDevIdx, CMfg &rstMfg);
    int32_t GetStatusData(uint32_t ulDevIdx, COtdrStatusData &rstStatus);
    int32_t GetCfgData(uint32_t ulDevIdx, COtdrCfgData &rstCfg);

    int32_t StartScan(uint32_t ulDevIdx, uint32_t uiDevType);
    int32_t SetPara(uint32_t ulDevIdx, uint32_t ulDistRang, uint32_t ulPulseWidth, double dSamplingResolution);
    int32_t SetMeasuringTime(uint32_t ulDevIdx, uint32_t ulMeasuringTime);
    int32_t SetUserIOR(uint32_t ulDevIdx, double dUserIOR);
    int32_t SetThr(uint32_t ulDevIdx, double dRefThr, double dSpliceLosThr, double dFiberEndThr);
    int32_t GetTemp(uint32_t ulDevIdx, COtdrTemp &rstTemp);

    int32_t Reset(uint32_t ulDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t ulDevIdx, const char *c_pcFilePath);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);

    /* DBG */
    int32_t DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 0);
    int32_t SetScanMode(uint32_t ulDevIdx, uint32_t ulScanMode);
    int32_t SetStartOffset(uint32_t ulDevIdx, COtdrStartOffset *pstStartOffset);
    int32_t GetStartOffset(uint32_t ulDevIdx, COtdrStartOffset &rstStartOffset);

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OTDR_STA,
                                                            SHM_SEM_KEY_OTDR_STA,
                                                            SHM_SIZE_OTDR_STA},

                                                           {SHM_KEY_OTDR_CMD,
                                                            SHM_SEM_KEY_OTDR_CMD,
                                                            SHM_SIZE_OTDR_CMD},

                                                           {SHM_KEY_OTDR_DATA,
                                                            SHM_SEM_KEY_OTDR_DATA,
                                                            SHM_SIZE_OTDR_DATA}}};


};






