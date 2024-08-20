/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicI2c.cpp
   Author:        Zhu Weian
   Date:          2020-08-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-08-05] [1.0] [Creator]


*****************************************************************************************************/
#include "LogicI2c.h"

typedef struct logic_i2creg_ctl_t
{
    union
    {
        struct
        {
            uint8_t bOpRestart                        : 1;
            uint8_t bOpType                           : 3;
            uint8_t brsv                              : 2;
            uint8_t bAckErr                           : 1;
            uint8_t bFinishFlag               : 1;
        };
        uint8_t ucCtl;
    };

}LOGIC_I2CREG_CTL_T;

typedef enum
{
    LOGIC_OP_TYPE_INIT = 0,
    LOGIC_OP_TYPE_WRITE,
    LOGIC_OP_TYPE_READ,
    LOGIC_OP_TYPE_TX,
    LOGIC_OP_TYPE_LAST_TX,
    LOGIC_OP_TYPE_RX,
    LOGIC_OP_TYPE_LAST_RX

} EN_LOGIC_I2C_CTL;
/*
   #define LOGIC_REG_I2C_VER                            0x90
   #define LOGIC_REG_I2C_CTRL                           0x92
   #define LOGIC_REG_I2C_DEV_ADDR                       0x94
   #define LOGIC_REG_I2C_TX_DATA                        0x96
   #define LOGIC_REG_I2C_RX_DATA                        0x97
 */
#define LOGIC_REG_I2C_VER                            0x180
#define LOGIC_REG_I2C_CTRL                           0x184
#define LOGIC_REG_I2C_DEV_ADDR                       0x188
#define LOGIC_REG_I2C_TX_DATA                        0x18C
#define LOGIC_REG_I2C_RX_DATA                        0x190
#define LOGIC_REG_I2C_SPEED_MODE                     0x194

std::mutex CLogicI2c::ms_stMutex;

bool
CLogicI2c::__AckReset(void)
{
    LOGIC_I2CREG_CTL_T stLogicI2cCtl;

    if (nullptr == m_stpLogicPtr) {
        return false;
    }

    if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, &stLogicI2cCtl.ucCtl)) {
        return false;
    }

    stLogicI2cCtl.ucCtl |= 0x40;

    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, stLogicI2cCtl.ucCtl)) {
        return false;
    }

    CDelay::Delay(500, US);
    return true;
}


bool
CLogicI2c::__CheckAck(void)
{
    LOGIC_I2CREG_CTL_T stLogicI2cCtl;

    if (nullptr == m_stpLogicPtr) {
        return false;
    }

    if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, &stLogicI2cCtl.ucCtl)) {
        return false;
    }

    if (stLogicI2cCtl.ucCtl & 0x40) {
        LOG_WRITE(EN_LOG_DEBUG, "cpld i2c ack error");
        return false;
    }

    return true;
}

bool
CLogicI2c::__CheckSta(void)
{
    uint32_t uiRetryTimes = 0;
    LOGIC_I2CREG_CTL_T stLogicI2cCtl;

    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpLogicPtr.");
        return false;
    }

    while (1) {
        if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, &stLogicI2cCtl.ucCtl)) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Read error.");
            return false;
        }

        if (stLogicI2cCtl.ucCtl & 0x80)
            break;

        if (uiRetryTimes++ > 1000) {
            LOG_WRITE(EN_LOG_ERROR, "uiRetryTimes++ > 1000. m_uiBaseAddr = 0X%02x, Addr = 0X%02x", m_uiBaseAddr, LOGIC_REG_I2C_CTRL + m_uiBaseAddr);
            return false;
        }
    }

    return true;
}

bool
CLogicI2c::__Ctl(uint8_t ucCtl)
{
    if (nullptr == m_stpLogicPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr");
        return false;
    }

    ucCtl &= ~0x01;
    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->WriteReg(%d) error.", ucCtl);
        return false;
    }


    ucCtl |= 0x01;
    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->WriteReg(%d) error.", ucCtl);
        return false;
    }

    if (false == __CheckSta()) {
        LOG_WRITE(EN_LOG_ERROR, "CheckSta error.");
        return false;
    }

    ucCtl &= ~0x01;
    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Write error.");
        return false;
    }

    CDelay::Delay(500, US);
    return true;
}

bool
CLogicI2c::Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    uint8_t ucDataW = 0;
    uint32_t i = 0;
    LOGIC_I2CREG_CTL_T stLogicI2cCtl;
    stLogicI2cCtl.ucCtl = 0x0;
    LOG_WRITE(EN_LOG_DEBUG, "CLogicI2c::Read +++");

    if (uiLenR < 1) {
        LOG_WRITE(EN_LOG_ERROR, "uiLenR(%d) < 1", uiLenR);
        return false;
    }

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_INIT;

    ms_stMutex.lock();

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        goto ERROR_LABEL;
    }

    if (false == __AckReset()) {
        LOG_WRITE(EN_LOG_ERROR, "AckReset error.");
        goto ERROR_LABEL;
    }

    if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, &stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Read error.");
        goto ERROR_LABEL;
    }

    ucDataW = uiAddr;
    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_DEV_ADDR + m_uiBaseAddr, ucDataW)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Write error.");
        goto ERROR_LABEL;
    }

    CDelay::Delay(1, MS);

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_WRITE;
    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        goto ERROR_LABEL;
    }

    for (i = 0; i < uiLenW; i++) {
        ucDataW = pucBufW[i];

        if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_TX_DATA + m_uiBaseAddr, ucDataW)) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Write error.");
            goto ERROR_LABEL;
        }

        stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_TX;
        if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
            LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
            goto ERROR_LABEL;
        }
    }

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_READ;
    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "__Ctl error1.");
        goto ERROR_LABEL;
    }

    for (i = 0; i < (uiLenR - 1); i++) {
        stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_RX;

        if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
            LOG_WRITE(EN_LOG_ERROR, "__Ctl error2.");
            goto ERROR_LABEL;
        }

        if (!m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_RX_DATA + m_uiBaseAddr, &pucBufR[i])) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->ReadReg error.");
            goto ERROR_LABEL;
        }
    }

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_LAST_RX;

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        goto ERROR_LABEL;
    }

    if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_RX_DATA + m_uiBaseAddr, &pucBufR[i])) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr->Read error.");
        goto ERROR_LABEL;
    }
    CDelay::Delay(1, MS);

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_INIT;

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        goto ERROR_LABEL;
    }

    if (false == __CheckAck()) {
        LOG_WRITE(EN_LOG_ERROR, "CheckAck error.");
        goto ERROR_LABEL;
    }

    ms_stMutex.unlock();

    return true;

ERROR_LABEL:
    ms_stMutex.unlock();
    return false;
}


bool
CLogicI2c::Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    uint8_t ucDataW = 0;
    uint32_t i = 0;
    LOGIC_I2CREG_CTL_T stLogicI2cCtl;

    stLogicI2cCtl.ucCtl = 0x0;
    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_INIT;

    ms_stMutex.lock();

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        goto ERROR_LABEL;
    }

    if (false == __AckReset()) {
        goto ERROR_LABEL;
    }

    if (false == m_stpLogicPtr->ReadReg(LOGIC_REG_I2C_CTRL + m_uiBaseAddr, &stLogicI2cCtl.ucCtl)) {
        goto ERROR_LABEL;
    }

    ucDataW = uiAddr;

    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_DEV_ADDR + m_uiBaseAddr, ucDataW)) {
        goto ERROR_LABEL;
    }
    CDelay::Delay(1, MS);

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_WRITE;

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        goto ERROR_LABEL;
    }

    for (i = 0; i < (uiLenW - 1); i++) {
        if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_TX_DATA + m_uiBaseAddr, pucBufW[i])) {
            goto ERROR_LABEL;
        }

        stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_TX;

        if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
            goto ERROR_LABEL;
        }
    }

    if (false == m_stpLogicPtr->WriteReg(LOGIC_REG_I2C_TX_DATA + m_uiBaseAddr, pucBufW[i])) {
        goto ERROR_LABEL;
    }

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_LAST_TX;

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        goto ERROR_LABEL;
    }

    stLogicI2cCtl.bOpType = LOGIC_OP_TYPE_INIT;

    if (false == __Ctl(stLogicI2cCtl.ucCtl)) {
        goto ERROR_LABEL;
    }

    if (false == __CheckAck()) {
        goto ERROR_LABEL;
    }

    ms_stMutex.unlock();

    return true;

ERROR_LABEL:
    ms_stMutex.unlock();
    return false;
}

