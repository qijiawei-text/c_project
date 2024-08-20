/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOcsCApi.h
   Author:        Lu Mao
   Date:          2024-01-02
   Version:       1.0
   Description:
   Function List:

   History:
   [Lu Mao] [2024-01-02] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_OCS_C_API__
#define _DEV_OCS_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevOcsData.h"

#ifdef __cplusplus
extern "C" {
#endif

int OCS_GetMfg(uint32_t uiDevType, CMfg *pstMfg);
int OCS_GetStatusData(uint32_t uiDevType, COcsStatusData *pstStatus);
int OCS_SetConnection(uint32_t uiDevType, const COcsConnection *pstConnection);
int OCS_SetConnections(uint32_t uiDevType, const COcsConnections *pstConnections);
int OCS_SetLedState(uint32_t uiDevType, uint32_t index, uint32_t uiState);
int OCS_SetLedMode(uint32_t uiDevType, uint32_t uiMode);
int OCS_Reset(uint32_t uiDevType, uint8_t ucResetType);
int OCS_Upgrade(uint32_t uiDevType, const char *pcPath);
int OCS_GetUpgradeStatus(uint32_t uiDevType);

#ifdef __cplusplus
};
#endif

#endif
