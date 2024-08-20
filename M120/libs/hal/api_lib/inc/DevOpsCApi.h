/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOpsCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_OPS_C_API__
#define _DEV_OPS_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevOpsData.h"

#ifdef __cplusplus
extern "C" {
#endif

int OPS_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int OPS_GetCommStatusData(uint32_t uiDevType, COpsCommStatusData *pstOpsStatus);
int OPS_GetCfgData(uint32_t uiDevIdx, COpsCfgData *pstOpsCfg, uint32_t uiPortId);
int OPS_GetStatusData(uint32_t uiDevIdx, COpsStatusData *pstOpsStatus, uint32_t uiPortId);
int OPS_GetHistoryData(uint32_t uiDevIdx, COpsHistoryData *pstData, uint32_t uiPortId);
int OPS_GetLedStatus(uint32_t uiDevIdx, COpsLed *pstData);
int OPS_ClearAlarmDuration(uint32_t uiDevIdx);

int OPS_SetThrPwrDiff(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiPortId);
int OPS_SetThrLine(uint32_t uiDevIdx, uint32_t uiPortIdx, int16_t sThr, int16_t sHys, uint32_t uiPortId);

int OPS_SetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, uint32_t uiPortId);
int OPS_SetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sHys, uint32_t uiPortId);

int OPS_SetWtr(uint32_t uiDevIdx, uint32_t uiWtr, uint32_t uiPortId);
int OPS_SetHoldOffTime(uint32_t uiDevIdx, uint32_t uiHoldOffTime, uint32_t uiPortId);
int OPS_SetRevertive(uint32_t uiDevIdx, bool bRevertive, uint32_t uiPortId);
int OPS_SetMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiPortId);
int OPS_SetRelativeThr(uint32_t uiDevIdx, int16_t sThr, uint32_t uiPortId);
int OPS_SetRelativeOffset(uint32_t uiDevIdx, int16_t sOffset, uint32_t uiPortId);

/*
   uiPos
   0-	W
   1-	P

   iThr: 0.01dBm, [-1000, 500]
 */
int OPS_SetSwitchThr(uint32_t uiDevIdx, uint32_t uiPos, int32_t iThr, uint32_t uiPortId);

/*
   uiPos
   0-	W
   1-	P

   iHys: 0.01dBm, [0, 500]
 */

int OPS_SetSwitchHys(uint32_t uiDevIdx, uint32_t uiPos, int32_t iHys, uint32_t uiPortId);


int OPS_Reset(uint32_t uiDevIdx, uint8_t ucResetType);
int OPS_Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
int OPS_GetUpgadeStatus(uint32_t uiDevIdx);
int OPS_SetLedDebugMode(uint32_t uiDevType, uint32_t uiMode);
int OPS_SetLedStatus(uint32_t uiDevType, uint32_t uiLedIdx, uint32_t uiStatus);

int OPS_SetConnectionPdThr(uint32_t uiDevIdx, uint32_t uiPdIdx, uint32_t uiType, int16_t sThr, uint32_t uiPortId);
int OPS_SetConnectionPdHys(uint32_t uiDevIdx, uint32_t uiPdIdx, uint32_t uiType, int16_t sHys, uint32_t uiPortId);
int OPS_SetAlmMask(uint32_t uiDevType, int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiPortId);
int OPS_GetEventLog(uint32_t uiDevType, COpsEventLogAllData *pstEventLogAll, uint32_t uiPortId);
/********************************************************************************************/
/* Operation commands for 1+1 bidirectional switching.                        */
/********************************************************************************************/
/* @ {                                                                                      */
int OPS_SetRemoteStatus(uint32_t uiDevIdx, COpsReqStatus *pstStatus, uint32_t uiPortId);
int OPS_GetRequestStatus(uint32_t uiDevIdx, COpsBidirectionStatus *pstStatus, uint32_t uiPortId);
int OPS_SetSwitchingType(uint32_t uiDevIdx, uint8_t ucType, uint32_t uiPortId);
/* @ }                                                                                      */

int OPS_GetOpsDevIdByKey(char *pcName, uint32_t *piDevId, uint32_t *piPortId);

#ifdef __cplusplus
};
#endif

#endif
