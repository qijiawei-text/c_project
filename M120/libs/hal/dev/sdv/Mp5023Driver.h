/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Mp5023Driver.h
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

#include "I2cDriver.h"

#define OPERATION                           0x01
#define CLEAR_FAULTS                        0x03
#define STORE_USER_ALL                      0x15
#define RESTORE_USER_ALL                    0x16
#define CAPABILITY                          0x19
#define IOUT_CAL_GAIN                       0x38
#define IOUT_CAL_OFFSET                     0x39
#define IOUT_OC_WARN_LIMIT                  0x4A
#define OT_FAULT_LIMIT                      0x4F
#define OT_WARN_LIMIT                       0x51
#define VIN_OV_WARN_LIMIT                   0x57
#define VIN_UV_WARN_LIMIT                   0x58
#define POWER_GOOD_ON                       0x5E
#define POWER_GOOD_OFF                      0x5F
#define STATUS_BYTE                         0x78
#define STATUS_WORD                         0x79
#define STATUS_ INPUT                       0x7C
#define STATUS_TEMPERATURE                  0x7D
#define STATUS_CML                          0x7E
#define READ_EIN                            0x86
#define READ_VIN                            0x88
#define READ_VOUT                           0x8B
#define READ_IOUT                           0x8C
#define READ_TEMPERATURE                    0x8D
#define READ_PIN                            0x97
#define PMBUS_REVISION                      0x98
#define MFR_ID                              0x99
#define MFR_MODEL                           0x9A
#define MFR_REVISION                        0x9B
#define MFR_DATE                            0x9D
#define MFR_CTRL                            0xF0
#define MFR_ADDR_PMBUS                      0xF1
#define CONFIG_ID                           0xF2
#define MFR_SPECIFIC_STARTUP_CURRENT_LIMIT  0xF6
#define MFR_OTP_LEFT                        0xFE


class CMp5023
{
public:

    CMp5023(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
    }

    virtual
    ~CMp5023()
    {
    }

    bool GetStatus(CMp5023Sta &rstMp5023Sta);

private:
    bool m_bInit = false;

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;

    uint8_t m_ucVin = READ_VIN;
    uint8_t m_ucVOut = READ_VOUT;
    uint8_t m_ucIOut = READ_IOUT;
    uint8_t m_ucTemp = READ_TEMPERATURE;

};



