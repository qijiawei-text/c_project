/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardDriver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"

#define __USB_RELAOD_DEBUG__ 0

/*************************************************/

class CBoard : public CDevDriver
{
public:

    CBoard()
    {
    }

    virtual
    ~CBoard()
    {
    }

    virtual bool GetMfg(CBoardMfg &rstMfg);
    virtual bool GetStatusData(CBoardStatusData &rstData);

/* mfg */
    virtual bool GetChassicMfg(CMfg &rstMfg);
    virtual bool GetSccMfg(CMfg &rstMfg);
    virtual bool GetRpcMfg(CMfg &rstMfg);

    virtual bool GetLogicVersion(uint32_t uiIdx, char *pcVersion);

    virtual bool SetChassicMfg(const char *c_pcFileName);
    virtual bool SetChassicMfg(const char *c_pcFileName, uint32_t uiIdx);
    virtual bool SetChassicConfigType(const char *c_pcConfigType);
    virtual bool SetSccMfg(const char *c_pcFileName);


/* cali */
    virtual bool GetCali(uint32_t uiIdx, CCali &rstCali);
    virtual bool SetCali(uint32_t uiIdx, const char *c_pcFileName);

/* cfg */
    virtual bool GetCfg(uint32_t uiIdx, uint32_t uiAddrSlice, const char *c_pcFileName);
    virtual bool SetCfg(uint32_t uiIdx, uint32_t uiAddrSlice, uint32_t uiSize, const char *c_pcFileName);

/* upgrade info */
    virtual bool GetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);
    virtual bool SetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);

/* bundle info */
    virtual bool GetBdlHead(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);
    virtual bool SetBdlHead(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);

/* temperature */
    virtual bool GetTemp(uint32_t uiIdx, int32_t &riTemp);

/* led */
    virtual bool SetLedState(uint32_t uiIdx, uint32_t uiState);

/* logic */
    virtual bool LogicRead(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool LogicWrite(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool LogicReadSlc(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool LogicWriteSlc(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool LogicReset(uint8_t ucResetType, uint32_t uiSubIdx);
    virtual bool LogicUpgrade(uint32_t uiIdx, const char *pcFilePath);

/* reset type */
    virtual bool SetResetType(uint32_t uiType);

/* watch dog */
    virtual bool CfgWatchDog(bool bEnable, uint8_t ucTimeout);

/* apsd */
    virtual bool SetApsdEnable(uint32_t uiIdx, bool bEnable);


/* eeprom */
    virtual bool EepromRead(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool EepromWrite(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);

/* I2c test*/
    virtual bool I2cTest(uint32_t uiPsuIdx, uint32_t uiI2cIdx);

/* ltc4215 */
    virtual bool GetLtc4215(uint32_t uiIdx, int32_t &riLtc4215Sense, int32_t &riLtc4215Source);

/* mp5023 */
    virtual bool GetMp5023(uint32_t uiIdx, CMp5023Sta &rstMp5023);

private:
#if 0
    std::vector<std::shared_ptr<CLogic> > m_staLogicPtrs;
    std::shared_ptr<CApsd> m_stpApsdPtr = nullptr;
    std::shared_ptr<CWatchDog> m_stpWatchDogPtr = nullptr;
#endif
    uint64_t m_ullCnt = 0;
    int32_t m_aiTemp[TEMP_MAX_NUM];
    int32_t m_aiLtc4215Sense[LTC4215_MAX_NUM];
    int32_t m_aiLtc4215Source[LTC4215_MAX_NUM];
    float m_adVoltage[MAX1139_MAX_NUM][12];
    CMp5023Sta m_astMp5023[LTC4215_MAX_NUM];

};







