/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "DevApiManager.h"
#include "HalApi.h"


#include "DevBoardCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
BOARD_GetChassisMfg(CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetChassisMfg(*pstMfg);
}

int
BOARD_SetChassisMfg(const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetChassisMfg(pcFilePath);
}

int
BOARD_SetChassisConfigType(const char *pcConfigType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetChassisConfigType(pcConfigType);
}

int
BOARD_GetChassisCali(CCali *pstCali)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetChassisCali(*pstCali);
}

int
BOARD_SetChassisCali(const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetChassisCali(pcFilePath);
}

int
BOARD_GetChassisCfg(uint32_t uiAddrSlice, const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetChassisCfg(uiAddrSlice, pcFilePath);
}

int
BOARD_SetChassisCfg(uint32_t uiAddrSlice, uint32_t uiSize, const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetChassisCfg(uiAddrSlice, uiSize, pcFilePath);
}

int
BOARD_GetUpgInfo(uint8_t *pucBuf, uint32_t ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetUpgInfo(pucBuf, ulLen);
}
int
BOARD_SetUpgInfo(uint8_t *pucBuf, uint32_t ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetUpgInfo(pucBuf, ulLen);
}
int
BOARD_GetBdlHead(uint8_t *pucBuf, uint32_t ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetBdlHead(pucBuf, ulLen);
}
int
BOARD_SetBdlHead(uint8_t *pucBuf, uint32_t ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetBdlHead(pucBuf, ulLen);
}

int
BOARD_GetStatusData(CBoardStatusData *pstBoardStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetStatusData(*pstBoardStatus);
}


int
BOARD_GetSccMfg(CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetSccMfg(*pstMfg);
}

int
BOARD_SetSccMfg(const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetSccMfg(pcFilePath);
}

int
BOARD_SetRpcMfg(const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetRpcMfg(pcFilePath);
}

int
BOARD_GetRpcMfg(CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetRpcMfg(*pstMfg);
}

int
BOARD_SetResetType(uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetResetType(ucResetType);
}

int
BOARD_GetResetType(int iIndex, uint8_t *pucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    int iRet = -1;
    CBoardStatusData stBoardStatusData;

    iRet = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData);
    if (OPLK_OK == iRet) {
        *pucResetType = stBoardStatusData.aucResetType[iIndex];
    }

    return iRet;
}

int
BOARD_CfgWatchDogTime(bool bEnable, uint8_t ucTimeout)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().CfgWatchDog(bEnable, ucTimeout);
}

int
BOARD_GetApsdStatus(int iIndex, bool *pbCfg, bool *pbStatus, bool *pbEvent)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    int iRet = -1;
    CBoardStatusData stBoardStatusData;

    iRet = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData);
    if (OPLK_OK == iRet) {
        *pbCfg = stBoardStatusData.abApsdEnable[iIndex];
        *pbStatus = stBoardStatusData.abApsdStatus[iIndex];
        *pbEvent = stBoardStatusData.abApsdEvent[iIndex];
    }

    return iRet;
}


int
BOARD_SetLedState(uint32_t uiDevIdx, uint32_t uiState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetLedState(uiDevIdx, uiState);
}

int
BOARD_SetLedTestMode(uint32_t uiDevIdx, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetLedTestMode(uiDevIdx, bEnable);
}

int
BOARD_SetLedLocationTestMode(uint32_t uiDevIdx, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetLedLocationTestMode(uiDevIdx, bEnable);
}

int
BOARD_ResetLogic(uint32_t uiDevIdx, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LogicReset(uiDevIdx, ucResetType);
}

int
BOARD_UpgradeLogic(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0;

    if (uiDevType == DEV_TYPE_FPGA) {
        uiDevIdx = 0;
    } else if (uiDevType == DEV_TYPE_CPLD) {
        uiDevIdx = 1;
    }
    return CDevApiManager::GetInstance().GetBoardApi().LogicUpgrade(uiDevIdx, c_pcFilePath);
}

int
BOARD_GetUpgadeStatus()
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetUpgadeStatus();
}



int
FAN_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetFanMfg(uiDevIdx, *pstMfg);
}

int
FAN_SetMfg(uint32_t uiDevIdx, const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetFanMfg(uiDevIdx, pcFilePath);
}

int
FAN_SetSpeed(uint32_t uiDevIdx, uint32_t uiDuty)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetFanSpeed(uiDevIdx, uiDuty);
}

int
FAN_GetSpeedCfg(uint32_t uiDevIdx, uint32_t *uiCfgSpeed)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetFanSpeedCfg(uiDevIdx, uiCfgSpeed);
}

int
FAN_SetResetCauseInFan(uint8_t ucResetCause)
{
    uint32_t ulValue = 0;

    ulValue = (uint32_t)ucResetCause;
    for (uint32_t uiDevIdx = 0; uiDevIdx < FAN_CNT; uiDevIdx++) {
        CDevApiManager::GetInstance().GetBoardApi().SetResetCauseInFan(uiDevIdx, ulValue);
    }

    return OPLK_OK;
}

int
FAN_GetResetCauseInFan(uint8_t *pucResetCause)
{
    int iResult = OPLK_ERR;
    CBoardStatusData stBoardStatus;
    memset(&stBoardStatus, 0x00, sizeof(stBoardStatus));
    uint8_t ucValue = 0;

    if (NULL == pucResetCause) {
        return OPLK_ERR;
    }

    iResult = BOARD_GetStatusData(&stBoardStatus);
    if (iResult != OPLK_OK) {
        return OPLK_ERR;
    }

    for (int i = 0; i < FAN_CNT; i++) {
        ucValue |= stBoardStatus.astFan[i].ucResetCause;
    }

    *pucResetCause = ucValue;

    return OPLK_OK;
}

int
FAN_SetControllerEEP(uint32_t uiDevIdx)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetFanControllerEEP(uiDevIdx);
}

int
FAN_SetLed(uint32_t uiDevIdx, uint32_t uiValue)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetFanLed(uiDevIdx, uiValue);
}



int
POWER_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().GetPowerMfg(uiDevIdx, *pstMfg);
}

char *
BOARD_GetProductName(char *pcBuf, size_t nSize)
{
    int rc = OPLK_OK;
    CMfg stMfgInfo;

    if (NULL == pcBuf) {
        return NULL;
    }

    /* Get mfginfo from CHASSIS/RPC EEPROM                                      */
    memset(&stMfgInfo, 0, sizeof(stMfgInfo));
    rc = BOARD_GetChassisMfg(&stMfgInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetChassisMfg");
        return NULL;
    }

    /* set product name                                                         */
    memset(pcBuf, 0x00, nSize);
    snprintf(pcBuf, nSize, "%s", stMfgInfo.acProductName);

    return pcBuf;
}

/* This function need to be merge with 'BOARD_GetProductName', just TBD          */
char *
BOARD_GetConfigType(char *pcBuf, size_t nSize)
{
    int rc = OPLK_OK;
    CMfg stMfgInfo;

    if (NULL == pcBuf) {
        return NULL;
    }

    /* Get mfginfo from CHASSIS/RPC EEPROM                                      */
    memset(&stMfgInfo, 0, sizeof(stMfgInfo));
    rc = BOARD_GetChassisMfg(&stMfgInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetChassisMfg");
        return NULL;
    }

    /* set product name                                                         */
    memset(pcBuf, 0x00, nSize);
    snprintf(pcBuf, nSize, "%s", stMfgInfo.acConfigType);

    return pcBuf;
}

PRODUCT_TYPE_EN
BOARD_GetProductType()
{
    static PRODUCT_TYPE_EN m_enProductType = EN_PRODUCT_TYPE_MAX;
    CMfg stMfgInfo;
    int rc = 0;

    if (EN_PRODUCT_TYPE_MAX != m_enProductType) {
        return m_enProductType;
    }

    rc = BOARD_GetChassisMfg(&stMfgInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get chassi mfinfo error error");
        return EN_PRODUCT_TYPE_NR1004;
    }

    if (0 == strcasecmp(stMfgInfo.acProductName, "OLS-P")) {
        m_enProductType = EN_PRODUCT_TYPE_OLSP;
    } else if (0 == strcasecmp(stMfgInfo.acProductName, "OLS-HP")) {
        m_enProductType = EN_PRODUCT_TYPE_HCF;
    } else if (0 == strcasecmp(stMfgInfo.acProductName, "NR1002-CHAS-01")) {
        if (0 == strcasecmp(stMfgInfo.acConfigType, "OLS-HP")) {
            m_enProductType = EN_PRODUCT_TYPE_HCF;
        } else {
            /* Default value is 'HCF' */
            m_enProductType = EN_PRODUCT_TYPE_HCF;
        }
    } else if (0 == strcasecmp(stMfgInfo.acProductName, "OCS")) {
        m_enProductType = EN_PRODUCT_TYPE_OCS;
    } else {
        if (0 == strcasecmp(stMfgInfo.acConfigType, "AMP")) {
            m_enProductType = EN_PRODUCT_TYPE_NR1004;
        } else if (0 == strcasecmp(stMfgInfo.acConfigType, "OPS")) {
            m_enProductType = EN_PRODUCT_TYPE_NR1004_OPS_4;
        } else {
            /* Default value is 'NR1004' */
            m_enProductType = EN_PRODUCT_TYPE_NR1004;
        }
    }

    return m_enProductType;
}


#if REGION_DES("dbg")
int
DBG_ReadLogicReg(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LogicRead(uiIdx, ulAddr, ulLen, pucVal);
}

int
DBG_WriteLogicReg(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LogicWrite(uiIdx, ulAddr, ulLen, pucVal);
}

int
DBG_ReadLogicRegSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LogicReadSlc(uiIdx, ulAddr, ulLen, pucVal);
}

int
DBG_WriteLogicRegSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LogicWriteSlc(uiIdx, ulAddr, ulLen, pucVal);
}

int
DBG_LoopEnable(bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LoopEnable(bEnable);
}

int
DBG_LoopInterval(uint32_t uiInterval)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().LoopInterval(uiInterval);
}

int
DBG_IdleEnable(bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().IdleEnable(bEnable);
}

int
DBG_ApsdEnable(int iIndex, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetApsdEnable(iIndex, bEnable);

}

int
DBG_GetAprStatus(uint32_t uiIndex, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return 0;
}

int
DBG_SpiSendCommand(uint32_t ulChip, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SpiSendCommand(ulChip, pucBufW, pucBufR, ulLen);
}
int
DBG_SpiFlashRead(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SpiFlashRead(ulChip, ulAddr, ulLen, pucVal);
}
int
DBG_SpiFlashWrite(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SpiFlashWrite(ulChip, ulAddr, ulLen, pucVal);
}
int
DBG_MdioRead(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().MdioRead(uiIdx, ulPhyAddr, ulRegAddr, pusData);
}
int
DBG_MdioWrite(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().MdioWrite(uiIdx, ulPhyAddr, ulRegAddr, usData);
}
int
DBG_NetSwitchRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().NetSwitchRead(ulPhyAddr, ulRegAddr, pusData);
}
int
DBG_NetSwitchWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().NetSwitchWrite(ulPhyAddr, ulRegAddr, usData);
}

int
DBG_SwitchPhyRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SwitchPhyRead(ulPhyAddr, ulRegAddr, pusData);
}
int
DBG_SwitchPhyWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SwitchPhyWrite(ulPhyAddr, ulRegAddr, usData);
}

int
DBG_ReadEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().EepromRead(uiIdx, ulAddr, ulLen, pucVal);
}

int
DBG_WriteEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().EepromWrite(uiIdx, ulAddr, ulLen, pucVal);
}


int
BOARD_SetLogLevel(uint32_t uiLogLevel)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().SetLogLevel(uiLogLevel);
}

int
BOARD_ConvertDevType(const char *pcName, uint32_t *puiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetBoardApi().ConvertDevType(pcName, *puiDevType);
}

#endif


#ifdef __cplusplus
};
#endif



