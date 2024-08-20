/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "DevApiManager.h"
#include "HalApi.h"

#include "DevTdcmCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif


int
TDCM_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().GetMfg(uiDevIdx, *pstMfg);
}

int
TDCM_GetStatusData(uint32_t uiDevIdx, CTdcmStatusData *pstTdcmStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().GetStatusData(uiDevIdx, *pstTdcmStatus);
}

int
TDCM_GetCfgData(uint32_t uiDevIdx, CTdcmCfgData *pstTdcmCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().GetCfgData(uiDevIdx, *pstTdcmCfg);
}

int
TDCM_SetFrequency(uint32_t uiDevIdx, float fFrequency)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().SetFrequency(uiDevIdx, fFrequency);
}

int
TDCM_SetDispersion(uint32_t uiDevIdx, float fDispersion)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().SetDispersion(uiDevIdx, fDispersion);
}

int
TDCM_Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetTdcmApi().Reset(uiDevIdx, ucResetType);
}

#ifdef __cplusplus
};
#endif



