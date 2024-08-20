/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MAX31760Driver.h
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
#include "LogicDriver.h"

#if 0
00h Read / Write CR1 Yes 01h Control Register 1
01h Read / Write CR2 Yes 10h Control Register 2
02h Read / Write CR3 Yes 03h Control Register 3
03h Read / Write FFDC Yes FFh Fan Fault Duty Cycle
04h Read / Write MASK Yes C0h Alert Mask Register
05h Read / Write IFR Yes 18h Ideality Factor Register
06h Read / Write RHSH Yes 55h Remote High Set - point MSB
07h Read / Write RHSL Yes 00h Remote High Set - point LSB
08h Read / Write LOTSH Yes 55h Local Overtemperature Set - point MSB
09h Read / Write LOTSL Yes 00h Local Overtemperature Set - point LSB
0Ah Read / Write ROTSH Yes 6Eh Remote Overtemperature Set - point MSB
0Bh Read / Write ROTSL Yes 00h Remote Overtemperature Set - point LSB
0Ch Read / Write LHSH Yes 46h Local High Set - point MSB
0Dh Read / Write LHSL Yes 00h Local High Set - point LSB
0Eh Read / Write TCTH Yes FFh TACH Count Threshold Register, MSB
0Fh Read / Write TCTL Yes FEh TACH Count Threshold Register, LSB
10h ~17h Read / Write USER Yes 00h 8 Bytes of General - Purpose User Memory
20h ~4Fh Read / Write LUT Yes FFh 48 - Byte Lookup Table(LUT)
50h Read / Write PWMR No 00h Direct Duty - Cycle Control Register
51h Read Only PWMV No 00h Current PWM Duty - Cycle Register
52h Read Only TC1H No 00h TACH1 Count Register, MSB
53h Read Only TC1L No 00h TACH1 Count Register, LSB
54h Read Only TC2H No 00h TACH2 Count Register, MSB
55h Read Only TC2L No 00h TACH2 Count Register, LSB
56h Read Only RTH No 00h Remote Temperature Reading Register, MSB
57h Read Only RTL No 00h Remote Temperature Reading Register, LSB
58h Read Only LTH No 00h Local Temperature Reading Register, MSB
59h Read Only LTL No 00h Local Temperature Reading Register, LSB
5Ah Read Only SR No 00h Status Register
5Bh Write Only EEX No 00h Load EEPROM to RAM; Write RAM to EEPROM
#endif

#define MAX31760_CTRL_REG1       0x00
#define MAX31760_CTRL_REG2       0x01
#define MAX31760_CTRL_REG3       0x02
#define MAX31760_FFDC            0x03
#define MAX31760_MASK            0x04
#define MAX31760_IFR             0x05
#define MAX31760_RHSH            0x06
#define MAX31760_RHSL            0x07
#define MAX31760_LOTSH           0x08
#define MAX31760_LOTSL           0x09
#define MAX31760_ROTSH           0x0A
#define MAX31760_ROTSL           0x0B
#define MAX31760_LHSH            0x0C
#define MAX31760_LHSL            0x0D
#define MAX31760_TCTH            0x0E
#define MAX31760_TCTL            0x0F
#define MAX31760_USER0           0x10
#define MAX31760_USER1           0x11
#define MAX31760_USER2           0x12
#define MAX31760_USER3           0x13
#define MAX31760_USER4           0x14
#define MAX31760_USER5           0x15
#define MAX31760_USER6           0x16
#define MAX31760_USER7           0x17
#define MAX31760_PWMR            0x50
#define MAX31760_PWMV            0x51
#define MAX31760_TC1H            0x52
#define MAX31760_TC1L            0x53
#define MAX31760_TC2H            0x54
#define MAX31760_TC2L            0x55
#define MAX31760_RTH             0x56
#define MAX31760_RTL             0x57
#define MAX31760_LTH             0x58
#define MAX31760_LTL             0x59
#define MAX31760_SR              0x5A
#define MAX31760_EEX             0x5B

#define PULSE_PER_CIRCLE         0x04
class CMax31760
{
public:

    CMax31760(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiHwSubIdx, uint32_t uiDevAddr)
        : m_stpI2cPtr(c_rstpI2cPtr), m_uiHwSubIdx(uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
        SetFanController();
        SetFanSpeed(99);
    }

    virtual
    ~CMax31760()
    {
    }

    bool InitFanController();
    bool SetFanController();
    bool GetFanRPM(uint32_t &uiSpeed);
    bool SetFanSpeed(uint8_t uiSpeed);
    bool GetFanSpeed(uint32_t &uiSpeed);

    bool SetResetCause(uint8_t ucResetCause);
    bool GetResetCause(uint8_t &ucResetCause);

private:
    bool m_bInit = false;

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiDevAddr;

    uint8_t m_ucRegCtrl1 = MAX31760_CTRL_REG1;
    uint8_t m_ucRegCtrl2 = MAX31760_CTRL_REG2;
    uint8_t m_ucRegCtrl3 = MAX31760_CTRL_REG3;
    uint8_t m_ucRegPwmr = MAX31760_PWMR;
    uint8_t m_ucRegPwmv = MAX31760_PWMV;
    uint8_t m_ucRegTc1h = MAX31760_TC1H;
    uint8_t m_ucRegTc1l = MAX31760_TC1L;
    uint8_t m_ucRegUser0 = MAX31760_USER0;
};



