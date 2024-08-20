/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevTdcmCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_TDCM_C_API__
#define _DEV_TDCM_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevTdcmData.h"

#ifdef __cplusplus
extern "C" {
#endif

int TDCM_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int TDCM_GetStatusData(uint32_t uiDevIdx, CTdcmStatusData *pstTdcmStatus);
int TDCM_GetCfgData(uint32_t uiDevIdx, CTdcmCfgData *pstTdcmCfg);
int TDCM_SetFrequency(uint32_t uiDevIdx, float fFrequency);
int TDCM_SetDispersion(uint32_t uiDevIdx, float fDispersion);
int TDCM_Reset(uint32_t uiDevIdx, uint8_t ucResetType);

#ifdef __cplusplus
};
#endif

#endif
