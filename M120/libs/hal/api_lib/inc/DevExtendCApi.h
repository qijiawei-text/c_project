/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevExtendCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_EXTEND_C_API__
#define _DEV_EXTEND_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevExtendData.h"

#ifdef __cplusplus
extern "C" {
#endif

int EXTEND_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int EXTEND_GetStatusData(uint32_t uiDevIdx, CExtendStatusData *pstStatus);
int EXTEND_Reset(uint32_t uiDevIdx, uint8_t ucResetType);
int EXTEND_Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
int EXTEND_GetUpgadeStatus(uint32_t uiDevIdx);
int EXTEND_DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
int EXTEND_DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
int EXTEND_SetSfp(uint32_t uiDevIdx, uint32_t ulVal);

int MIRROR_GetStatusData(uint32_t uiDevIdx, CMirrorData *pstStatus);

int SWITCH_Pos(uint32_t uiDevIdx, uint32_t uiPort);

#ifdef __cplusplus
};
#endif

#endif
