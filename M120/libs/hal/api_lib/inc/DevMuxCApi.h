/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevMuxCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_MUX_C_API__
#define _DEV_MUX_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevMuxData.h"

#ifdef __cplusplus
extern "C" {
#endif

int MUX_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int MUX_GetStatusData(uint32_t uiDevIdx, CMuxStatusData *pstStatus);
int MUX_GetCali(uint32_t uiDevIdx, CMuxCaliData *pstCali);


#ifdef __cplusplus
};
#endif

#endif
