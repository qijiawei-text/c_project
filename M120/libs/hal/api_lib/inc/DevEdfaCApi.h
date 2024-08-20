/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevEdfaCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_EDFA_C_API__
#define _DEV_EDFA_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevEdfaData.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 400GZR : uiSubIdx always is 0 */
/* MEIWU  : uiSubIdx 0 -- OA0 1 -- OA1 */
/* uiDevIdx get from SLOT_GetInfo */

int EDFA_GetMfg(uint32_t uiDevType, CMfg *pstMfg);
int EDFA_GetStatusData(uint32_t uiDevType, COaStatusData *pstEdfaStatus);
int EDFA_GetCfgData(uint32_t uiDevType, COaCfgData *pstEdfaCfg);
int EDFA_GetDebugInfo(uint32_t uiDevType, const char *pcFilePath);
int EDFA_GetMcuDebugInfo(uint32_t uiDevType, const char *pcFilePath, uint32_t uiVal);
int EDFA_SetGainRange(uint32_t uiDevType, int16_t sGainRange);
int EDFA_SetGain(uint32_t uiDevType, int16_t sGain);
int EDFA_SetTilt(uint32_t uiDevType, int16_t sTilt);
int EDFA_SetMode(uint32_t uiDevType, uint8_t ucMode, int16_t sVal);
int EDFA_SetPumpEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetAprEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetAutolosEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetRxLosThr(uint32_t uiDevType, int16_t sThr, int16_t sHys);
int EDFA_SetAttn(uint32_t uiDevType, int16_t sAttn);
int EDFA_GetAttn(uint32_t uiDevType, int16_t *psAttn);
int EDFA_Reset(uint32_t uiDevType, uint8_t ucResetType);
int EDFA_Upgrade(uint32_t uiDevType, const char *c_pcFilePath);
int EDFA_Swap(uint32_t uiDevType);
int EDFA_Commit(uint32_t uiDevType);
int EDFA_GetUpgadeStatus(uint32_t uiDevType);
int EDFA_DbgRead(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen);
int EDFA_DbgWrite(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen);
int EDFA_SetDfbEnable(uint32_t uiDevType, bool bEnable, int32_t iPower);
int EDFA_SetOscEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetOtdrSwitchPos(uint32_t uiDevType, uint32_t uiPos);
int EDFA_SetOscAddThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys);
int EDFA_SetOscDropThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys);
int EDFA_SetOscRxThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys);
int EDFA_SetApsdEnable(uint32_t uiDevType, bool bEnable);
int EDFA_GetApsdStatus(uint32_t uiDevType, bool *pbCfg, bool *pbStatus, bool *pbEvent);
int EDFA_GetOcmAttn(uint32_t uiDevType, int16_t *psAttn);
int EDFA_SetUpgState(uint32_t uiDevType, int16_t sState);
int EDFA_SetLedState(uint32_t uiDevType, int16_t sState);
int EDFA_SetLedAlm(uint32_t uiDevType, int16_t sState);
int EDFA_GetCali(uint32_t uiDevType, COaCaliData *pstCaliInfo);
int EDFA_SetOtdrCali(uint32_t uiDevType, char *pcFilePath);
int EDFA_SetOcmCali(uint32_t uiDevType, char *pcFilePath);
int EDFA_SetManualVoaEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetPaddingVoaConfigPower(uint32_t uiDevType, int16_t sOutPower);
int EDFA_SetManualGainEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetThr(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys);
int EDFA_SetPumpCurrent(uint32_t uiDevType, uint32_t uiPumpIdx, int16_t sCurrent);
int EDFA_SetAlmMask(uint32_t uiDevType, int32_t iMaskIdx, int32_t iMaskVal);
int EDFA_GetEventLog(uint32_t uiDevType, COaEventLogAllData *pstEventLogAll);

/* add API for shutter component */
int EDFA_SetShutterState(uint32_t uiDevType, bool block);
int EDFA_GetShutterState(uint32_t uiDevType, bool *block);
int EDFA_SetRemoteInterlockEnable(uint32_t uiDevType, bool bEnable);
int EDFA_SetDummyAdminKey(uint32_t uiDevType, bool bEnable);
int EDFA_SetPanelSafeCmpt(uint32_t uiDevType, bool bEnable);
int EDFA_SetReadyTrigger(uint32_t uiDevType, uint32_t uiState);
int EDFA_SetFsmMode(uint32_t uiDevType, uint32_t uiMode);
int EDFA_SetReadyPower(uint32_t uiDevType, uint32_t uiPower);
int EDFA_SetTargetPower(uint32_t uiDevType, uint32_t uiPower);
#ifdef __cplusplus
};
#endif

#endif
