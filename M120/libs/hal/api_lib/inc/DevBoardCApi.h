/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevBoardCApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#ifndef _DEV_BOARD_C_API__
#define _DEV_BOARD_C_API__

#include <stdint.h>
#include <stdbool.h>

#include "DevData.h"
#include "DevBoardData.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EN_PRODUCT_TYPE_OLSP = 0,
    EN_PRODUCT_TYPE_NR1004,
    EN_PRODUCT_TYPE_NR1004_OPS_4,
    EN_PRODUCT_TYPE_HCF,
    EN_PRODUCT_TYPE_OCS,
    EN_PRODUCT_TYPE_MAX
} PRODUCT_TYPE_EN;

int BOARD_GetChassisMfg(CMfg *pstMfg);
int BOARD_SetChassisMfg(const char *pcFilePath);
int BOARD_SetChassisConfigType(const char *pcConfigType);
int BOARD_GetChassisCali(CCali *pstCali);
int BOARD_SetChassisCali(const char *pcFilePath);
int BOARD_GetChassisCfg(uint32_t uiAddrSlice, const char *pcFilePath);
int BOARD_SetChassisCfg(uint32_t uiAddrSlice, uint32_t uiSize, const char *pcFilePath);
int BOARD_GetUpgInfo(uint8_t *pucBuf, uint32_t ulLen);
int BOARD_SetUpgInfo(uint8_t *pucBuf, uint32_t ulLen);
int BOARD_GetBdlHead(uint8_t *pucBuf, uint32_t ulLen);
int BOARD_SetBdlHead(uint8_t *pucBuf, uint32_t ulLen);

int BOARD_GetStatusData(CBoardStatusData *pstBoardStatus);
int BOARD_GetSccMfg(CMfg *pstMfg);
int BOARD_SetSccMfg(const char *pcFilePath);
int BOARD_GetRpcMfg(CMfg *pstMfg);
int BOARD_SetRpcMfg(const char *pcFilePath);
int BOARD_SetResetType(uint8_t ucResetType);
int BOARD_GetResetType(int iIndex, uint8_t *pucResetType);
int BOARD_CfgWatchDogTime(bool bEnable, uint8_t ucTimeout);
int BOARD_GetApsdStatus(int iIndex, bool *pbCfg, bool *pbStatus, bool *pbEvent);

int BOARD_SetLedState(uint32_t uiDevIdx, uint32_t uiState);

int BOARD_ResetLogic(uint32_t uiDevIdx, uint8_t ucResetType);
int BOARD_UpgradeLogic(uint32_t uiDevIdx, const char *c_pcFilePath);
int BOARD_GetUpgadeStatus();


int FAN_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);
int FAN_SetMfg(uint32_t uiDevIdx, const char *pcFilePath);
int FAN_SetSpeed(uint32_t uiDevIdx, uint32_t uiDuty);
int FAN_GetSpeedCfg(uint32_t uiDevIdx, uint32_t *uiCfgSpeed);
int FAN_SetResetCauseInFan(uint8_t ucResetCause);
int FAN_GetResetCauseInFan(uint8_t *pucResetCause);
int FAN_SetControllerEEP(uint32_t uiDevIdx);
int FAN_SetLed(uint32_t uiDevIdx, uint32_t uiValue);
int POWER_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg);

char *BOARD_GetProductName(char *pcBuf, size_t nSize);
char *BOARD_GetConfigType(char *pcBuf, size_t nSize);
PRODUCT_TYPE_EN BOARD_GetProductType();

int BOARD_SetLogLevel(uint32_t uiLogLevel);

int BOARD_ConvertDevType(const char *pcName, uint32_t *puiDevType);

#ifdef __cplusplus
};
#endif

#endif
