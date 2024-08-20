/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOscCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_OSC_C_API__
#define _DEV_OSC_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevOscData.h"
#include "DevOpsData.h"

#ifdef __cplusplus
extern "C" {
#endif

int OSC_GetStatusData(uint32_t uiDevType, COscStatusData *pstOscStatus);
int OSC_GetCfgData(uint32_t uiDevType, COscCfgData *pstOscCfg);
int OSC_SetEnable(uint32_t uiDevType, bool bEnable);
int OSC_SetVoa(uint32_t uiDevType, uint32_t uiTxRx, int16_t sAttn);
int OSC_SetTarget(uint32_t uiDevType, uint32_t uiTxRx, int16_t sTarget);
int OSC_GetVoa(uint32_t uiDevType, uint32_t uiTxRx, int16_t *psAttn);
int OSC_SetSwitchPos(uint32_t uiDevType, COscSwitchPosSet rstSwitchPos);
int OSC_GetSwitchPos(uint32_t uiDevType, COscSwitchPos *pstSwitchPos);
int OSC_SetAmpEnable(uint32_t uiDevType, bool bEnable);

#ifdef __cplusplus
};
#endif

#endif
