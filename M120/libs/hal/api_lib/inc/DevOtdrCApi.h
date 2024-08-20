/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOtdrCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_OTDR_C_API__
#define _DEV_OTDR_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevOtdrData.h"

#ifdef __cplusplus
extern "C" {
#endif

int OTDR_GetMfg(uint32_t ulDevIdx, CMfg *pstMfg);
int OTDR_GetStatusData(uint32_t ulDevIdx, COtdrStatusData *pstStatus);
int OTDR_GetCfgData(uint32_t ulDevIdx, COtdrCfgData *pstCfg);
int OTDR_StartScan(uint32_t ulDevIdx);
int OTDR_SetPara(uint32_t ulDevIdx, uint32_t ulDistRang, uint32_t ulPulseWidth, double dSamplingResolution);
int OTDR_SetMeasuringTime(uint32_t ulDevIdx, uint32_t ulMeasuringTime);
int OTDR_SetUserIOR(uint32_t ulDevIdx, double dUserIOR);
int OTDR_SetThr(uint32_t ulDevIdx, double dRefThr, double dSpliceLosThr, double dFiberEndThr);
int OTDR_Reset(uint32_t ulDevIdx, uint8_t ucResetType);
int OTDR_Upgrade(uint32_t ulDevIdx, const char *c_pcFilePath);
int OTDR_GetUpgadeStatus(uint32_t uiDevIdx);
int OTDR_DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
int OTDR_DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
int OTDR_SetScanMode(uint32_t uiDevType, uint32_t ulScanMode);
int OTDR_SetStartOffset(uint32_t uiDevType, COtdrStartOffset *pstStartOffset);
int OTDR_GetStartOffset(uint32_t uiDevType, COtdrStartOffset *pstStartOffset);

/* Switch the port, compensate the port IL, then make a scan */
int OTDR_StartScanPort(uint32_t ulDevIdx, uint32_t ulPortId);

#ifdef __cplusplus
};
#endif

#endif
