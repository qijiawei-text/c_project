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


#include "DevMuxCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
MUX_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetMuxApi().GetMfg(uiDevIdx, *pstMfg);
}

int
MUX_GetCali(uint32_t uiDevIdx, CMuxCaliData *pstCali)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetMuxApi().GetCali(uiDevIdx, *pstCali);
}


int
MUX_GetStatusData(uint32_t uiDevIdx, CMuxStatusData *pstStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    return CDevApiManager::GetInstance().GetMuxApi().GetStatusData(uiDevIdx, *pstStatus);
}


#ifdef __cplusplus
};
#endif



