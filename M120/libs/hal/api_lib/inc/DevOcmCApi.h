/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOcmCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_OCM_C_API__
#define _DEV_OCM_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevWssData.h"
#include "DevOcmData.h"

#ifdef __cplusplus
extern "C" {
#endif

int OCM_GetMfg(uint32_t uiDevType, CMfg *pstMfg);
int OCM_GetStatusData(uint32_t uiDevType, COcmStatusData *pstOcmStatus);
int OCM_GetChPower(uint32_t uiDevType, uint32_t uiPortId, COcmChPower *pstOcmChPower);
int OCM_GetOsaData(uint32_t uiDevType, uint32_t uiPortId, COcmOsa *pstOcmOsa);
int OCM_GetRealTimeOsaData(uint32_t uiDevType, uint32_t uiPortId, COcmOsa *pstOcmOsa);
int OCM_SetWaveplan(uint32_t uiDevType, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan);
int OCM_SetOcmWaveplan(uint32_t uiDevType, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan);
int OCM_Reset(uint32_t uiDevType, uint8_t ucResetType);
int OCM_Upgrade(uint32_t uiDevType, const char *c_pcFilePath);
int OCM_Commit(uint32_t uiDevType);
int OCM_GetUpgradeStatus(uint32_t uiDevType);
int OCM_SetVoa(uint32_t uiDevType, int16_t sAttn);
int OCM_GetVoa(uint32_t uiDevType, int16_t *psAttn);
int OCM_GetVoaPd(uint32_t uiDevType, int16_t *psVoaIn, int16_t *psVoaOut);


#ifdef __cplusplus
};
#endif

#endif
