/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class CBoardApi : public CDevApi
{
public:
    CBoardApi()
    {
    }
    ~CBoardApi()
    {
    }

    int32_t GetChassisMfg(CMfg &rstMfg);
    int32_t SetChassisMfg(const char *c_pcFilePath);
    int32_t SetChassisConfigType(const char *pcConfigType);

    int32_t GetChassisCali(CCali &rstCali);
    int32_t SetChassisCali(const char *c_pcFilePath);

    int32_t GetChassisCfg(uint32_t uiAddrSlice, const char *c_pcFilePath);
    int32_t SetChassisCfg(uint32_t uiAddrSlice, uint32_t uiSize, const char *c_pcFilePath);


    int32_t GetUpgInfo(uint8_t *pucBuf, uint32_t ulLen);
    int32_t SetUpgInfo(uint8_t *pucBuf, uint32_t ulLen);
    int32_t GetBdlHead(uint8_t *pucBuf, uint32_t ulLen);
    int32_t SetBdlHead(uint8_t *pucBuf, uint32_t ulLen);

    int32_t GetSccMfg(CMfg &rstMfg);
    int32_t SetSccMfg(const char *c_pcFilePath);

    int32_t GetRpcMfg(CMfg &rstMfg);
    int32_t SetRpcMfg(const char *c_pcFilePath);

    int32_t GetSccCfg(uint32_t uiAddrSlice, const char *c_pcFilePath);
    int32_t SetSccCfg(uint32_t uiAddrSlice, const char *c_pcFilePath);

    int32_t GetStatusData(CBoardStatusData &rstStatus);
    int32_t GetCfgData(CBoardCfgData &rstCfg);

    /* LED */
    int32_t SetLedState(uint32_t uiDevIdx, uint32_t uiState);
    int32_t SetLedTestMode(uint32_t uiDevIdx, bool bEnable);
    int32_t SetLedLocationTestMode(uint32_t uiDevIdx, bool bEnable);
    /* Fan */
    int32_t GetFanMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t SetFanMfg(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t SetFanSpeed(uint32_t uiDevIdx, uint32_t uiDuty);
    int32_t GetFanSpeedCfg(uint32_t uiDevIdx, uint32_t *uiCfgSpeed);
    int32_t SetFanControllerEEP(uint32_t uiDevIdx);
    int32_t SetFanLed(uint32_t uiDevIdx, uint32_t uiValue);
    int32_t SetResetCauseInFan(uint32_t uiDevIdx, uint32_t uiResetCause);

    /* Power */
    int32_t GetPowerMfg(uint32_t uiDevIdx, CMfg &rstMfg);

    /* Logic */
    int32_t LogicRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t LogicWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t LogicReadSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t LogicWriteSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t LogicReset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t LogicUpgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t GetUpgadeStatus();

    /* other */
    int32_t SetResetType(uint32_t uiType);
    int32_t CfgWatchDog(bool bEnable, uint8_t ucTimeout);
    int32_t SetApsdEnable(uint32_t uiIdx, bool bCfg);
    int32_t SetLogLevel(uint32_t uiLogLevel);

    /* spi & mdio & flash & net-switch & eeprom */
    int32_t SpiSendCommand(uint32_t ulChip, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t ulLen);
    int32_t SpiFlashRead(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t SpiFlashWrite(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t MdioRead(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
    int32_t MdioWrite(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
    int32_t NetSwitchRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
    int32_t NetSwitchWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
    int32_t SwitchPhyRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData);
    int32_t SwitchPhyWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData);
    int32_t EepromRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);
    int32_t EepromWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal);

    /* debug */
    int32_t LoopEnable(bool bEnable);
    int32_t LoopInterval(uint32_t uiInterval);
    int32_t IdleEnable(bool bEnable);
    int32_t I2cTest(uint32_t uiPsuIdx, uint32_t uiI2cIdx);

    int32_t ConvertDevType(const std::string &strName, uint32_t &uiDevType);
private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_BOARD_STA,
                                                            SHM_SEM_KEY_BOARD_STA,
                                                            SHM_SIZE_BOARD_STA},

                                                           {SHM_KEY_BOARD_CMD,
                                                            SHM_SEM_KEY_BOARD_CMD,
                                                            SHM_SIZE_BOARD_CMD},

                                                           {SHM_KEY_BOARD_DATA,
                                                            SHM_SEM_KEY_BOARD_DATA,
                                                            SHM_SIZE_BOARD_DATA}}};


};






