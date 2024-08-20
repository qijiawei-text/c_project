/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Max1139Driver.h
   Author:        Qing Yan
   Date:
   Version:       1.0
   Description:
   Function List:

   History:
   [Qing Yan] [2022-04-11] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "LogicDriver.h"
#include "I2cDriver.h"

#define MAX1139_AIN_NUM         (12)
#define MAX1139_SETUP           (0x82)
#define MAX1139_CONFIG          (0x19)
#define MAX1139_VREF_COEF       (1024)
#define MAX1139_VREF_VOL        (3.3)


#define REG_SCC_CPLD_V1P1_ADC   0x1030
#define REG_SCC_CPLD_V1P5_ADC   0x1034
#define REG_SCC_CPLD_V1P2_ADC   0x1038
#define REG_SCC_CPLD_V1P8_ADC   0x103c
#define REG_SCC_CPLD_V1P0_ADC   0x1040
#define REG_SCC_CPLD_V2P5_ADC   0x1044

#define REG_SLC0_OTDR_V5_ADC    0x3010
#define REG_SLC0_SFP_V3P3_ADC   0x3014
#define REG_SLC0_OCM_V5_ADC     0x3018
#define REG_SLC0_BUF_3VP3_ADC   0x301c
#define REG_SLC0_CPLD_V3P3_ADC  0x3020
#define REG_SLC0_V12_ADC        0x3024

#define REG_SLC1_OTDR_V5_ADC    0x3110
#define REG_SLC1_SFP_V3P3_ADC   0x3114
#define REG_SLC1_OCM_V5_ADC     0x3118
#define REG_SLC1_BUF_3VP3_ADC   0x311c
#define REG_SLC1_CPLD_V3P3_ADC  0x3120
#define REG_SLC1_V12_ADC        0x3124

struct CVolReg
{
    uint32_t uiReg;
};

class CMax1139
{
public:
    CMax1139(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrvPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpLogicDrvPtr(c_rstpLogicDrvPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }

    CMax1139(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }

    virtual
    ~CMax1139()
    {
    }

    bool GetVoltage(float *pdVoltage, uint32_t uiAvinNum);
    bool GetVoltage(CMax1139Sta &astMax1139, uint32_t uiAvinNum);
    bool Switch(uint32_t uiCardIdx, uint32_t uiIdx);
    bool SwitchClose(uint32_t uiCardIdx);
private:
    bool m_bInit = false;
    std::shared_ptr<CLogicDriver> m_stpLogicDrvPtr = nullptr;
    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;

    float m_dVoltage[MAX1139_AIN_NUM];
    uint8_t m_ucSetup = MAX1139_SETUP;
    uint8_t m_ucConfig = MAX1139_CONFIG;
    static const std::vector<CVolReg> msc_staVolRegs;
};



