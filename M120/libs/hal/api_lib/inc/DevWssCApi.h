/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOcmCApi.h
   Author:        Weian Zhu
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_WSS_C_API__
#define _DEV_WSS_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevWssData.h"

#ifdef __cplusplus
extern "C" {
#endif

int WSS_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int WSS_GetCfg(uint32_t uiDevIdx, CWssAllCfg *pstAllCfg);
int WSS_GetOscCfg(uint32_t uiDevIdx, CWssAllCfg *pstCfg);
int WSS_GetStatusData(uint32_t uiDevIdx, CWssStatusData *pstStatus);
int WSS_SetAllChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg);
int WSS_SetOscChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg);
int WSS_SetWaveplan(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan);
int WSS_SetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr);
int WSS_SetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sHys);
int WSS_GetPortPowerHistory(uint32_t uiDevIdx);

/*uiMode : 0 ¨C normal 1 ¨C horse race lamp mode */
int WSS_SetLedMode(uint32_t uiDevIdx, uint32_t uiMode);

/* uiMode
   0 - normal
   1 - enter test mode
   2 - enter LED debug mode
 */
int WSS_SetLedDebugMode(uint32_t uiDevIdx, uint32_t uiMode);


/* uiStatus
   0 - off
   1 - red
   2 - green
   3 - shine
 */
int WSS_SetLedStatus(uint32_t uiDevIdx, uint32_t uiLedIdx, uint32_t uiStatus);



int WSS_Reset(uint32_t uiDevIdx, uint8_t ucResetType);
int WSS_Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
int WSS_GetUpgradeStatus(uint32_t uiDevIdx);
int WSS_SetUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
int WSS_GetUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
int WSS_SetPmUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);
int WSS_GetPmUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen);



#ifdef __cplusplus
};
#endif

#endif
