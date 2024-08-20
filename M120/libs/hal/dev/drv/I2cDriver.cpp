/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      I2cDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "I2cDriver.h"


/*==============================function=========================*/

bool
CFtI2c::Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    /*if ( nullptr == pucBufW ) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
       }*/

    /* LOG_WRITE(EN_LOG_DEBUG, "Write m_uiIdx = %d uiAddr = 0x%02X", m_uiIdx, uiAddr); */
    uiAddr = uiAddr >> 1;
    if (!m_stI2cFt.Write(uiIdx, uiAddr, pucBufW, uiLenW)) {
        return false;
    }

    return true;
}


bool
CFtI2c::Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    if ( nullptr == pucBufW ) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
    }

    if ( nullptr == pucBufR ) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufR is nullptr");
        return false;
    }

    uiAddr = uiAddr >> 1;
    if (!m_stI2cFt.Read(uiIdx, uiAddr, pucBufW, uiLenW, pucBufR, uiLenR)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiIdx = %d", uiIdx);
        return false;
    }

    return true;
}


/*bool
   CI2cFtPcaDriver::Switch(uint32_t uiDevIdx)
   {
    uint32_t uiChIdx = m_uiChIdx;

    if (!WriteDev(uiDevIdx, m_uiPcaAddr, &uiChIdx, 1)) {
        return false;
    }

    return true;
   }*/



#if 0

typedef struct fpga_i2creg_ctl_t
{
    union
    {
        struct
        {
            uint8_t bOpRestart            : 1;
            uint8_t bOpType               : 3;
            uint8_t brsv                  : 2;
            uint8_t bAckErr               : 1;
            uint8_t bFinishFlag           : 1;
        };
        uint8_t ucCtl;
    };

}FPGA_I2CREG_CTL_T;

typedef enum
{
    FPGA_OP_TYPE_INIT = 0,
    FPGA_OP_TYPE_WRITE,
    FPGA_OP_TYPE_READ,
    FPGA_OP_TYPE_TX,
    FPGA_OP_TYPE_LAST_TX,
    FPGA_OP_TYPE_RX,
    FPGA_OP_TYPE_LAST_RX

} EN_FPGA_I2C_CTL;


bool
CI2cLogicDriver::AckReset(void)
{
    FPGA_I2CREG_CTL_T stFpgaI2cCtl;

    if (nullptr == m_stpFpgaDrvPtr) {
        return false;
    }

    if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
        return false;
    }

    stFpgaI2cCtl.ucCtl |= 0x40;
    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
        return false;
    }

    return true;
}


bool
CI2cLogicDriver::CheckAck(void)
{
    FPGA_I2CREG_CTL_T stFpgaI2cCtl;

    if (nullptr == m_stpFpgaDrvPtr) {
        return false;
    }

    if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
        return false;
    }

    if (stFpgaI2cCtl.ucCtl & 0x40) {
        LOG_WRITE(EN_LOG_ERROR, "cpld i2c ack error");
        return false;
    }

    return true;
}

bool
CI2cLogicDriver::CheckSta(void)
{
    uint32_t uiRetryTimes = 0;
    FPGA_I2CREG_CTL_T stFpgaI2cCtl;

    if (nullptr == m_stpFpgaDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpFpgaDrvPtr.");
        return false;
    }

    while (1) {
        if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Read error.");
            return false;
        }

        if (stFpgaI2cCtl.ucCtl & 0x80)
            break;

        if (uiRetryTimes++ > 1000) {
            LOG_WRITE(EN_LOG_ERROR, "uiRetryTimes++ > 1000.");
            return false;
        }
    }

    return true;
}

bool
CI2cLogicDriver::Ctl(uint8_t ucCtl)
{
    if (nullptr == m_stpFpgaDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr is nullptr");
        return false;
    }

    ucCtl &= ~0x01;
    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_CTRL, &ucCtl, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Write(%d) error.", ucCtl);
        return false;
    }


    ucCtl |= 0x01;
    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_CTRL, &ucCtl, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Write(%d) error.", ucCtl);
        return false;
    }

    if (false == CheckSta()) {
        LOG_WRITE(EN_LOG_ERROR, "CheckSta error.");
        return false;
    }

    ucCtl &= ~0x01;
    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_CTRL, &ucCtl, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Write error.");
        return false;
    }

    return true;
}

bool
CI2cLogicDriver::Read(uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    uint8_t ucDataW = 0;
    uint32_t i = 0;
    FPGA_I2CREG_CTL_T stFpgaI2cCtl;

    if (uiLenR < 1) {
        LOG_WRITE(EN_LOG_ERROR, "uiLenR(%d) < 1", uiLenR);
        return false;
    }

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_INIT;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        return false;
    }

    if (false == AckReset()) {
        LOG_WRITE(EN_LOG_ERROR, "AckReset error.");
        return false;
    }

    if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Read error.");
        return false;
    }

    ucDataW = uiAddr;
    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_DEV_ADDR, &ucDataW, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Write error.");
        return false;
    }

    CDelay::Delay(1, MS);

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_WRITE;
    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        return false;
    }

    for (i = 0; i < uiLenW; i++) {
        ucDataW = pucBufW[i];

        if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_TX_DATA, &ucDataW, 1)) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Write error.");
            return false;
        }

        stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_TX;
        if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
            LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
            return false;
        }
    }

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_READ;
    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        return false;
    }

    for (i = 0; i < (uiLenR - 1); i++) {
        stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_RX;

        if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
            return false;
        }

        if (!m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_RX_DATA, &pucBufR[i], 1)) {
            return false;
        }
    }

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_LAST_RX;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        return false;
    }

    if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_RX_DATA, &pucBufR[i], 1)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpFpgaDrvPtr->Read error.");
        return false;
    }
    CDelay::Delay(1, MS);

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_INIT;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        LOG_WRITE(EN_LOG_ERROR, "Ctl error.");
        return false;
    }

    if (false == CheckAck()) {
        LOG_WRITE(EN_LOG_ERROR, "CheckAck error.");
        return false;
    }

    return true;
}


bool
CI2cLogicDriver::Write(uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    uint8_t ucDataW = 0;
    uint32_t i = 0;
    FPGA_I2CREG_CTL_T stFpgaI2cCtl;

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_INIT;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        return false;
    }

    if (false == AckReset()) {
        return false;
    }

    if (false == m_stpFpgaDrvPtr->Read(FPGA_CTRL_REG_I2C_CTRL, &stFpgaI2cCtl.ucCtl, 1)) {
        return false;
    }

    ucDataW = uiAddr;

    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_DEV_ADDR, &ucDataW, 1)) {
        return false;
    }
    CDelay::Delay(1, MS);

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_WRITE;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        return false;
    }

    for (i = 0; i < (uiLenW - 1); i++) {
        if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_TX_DATA, &pucBufW[i], 1)) {
            return false;
        }

        stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_TX;

        if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
            return false;
        }
    }

    if (false == m_stpFpgaDrvPtr->Write(FPGA_CTRL_REG_I2C_TX_DATA, &pucBufW[i], 1)) {
        return false;
    }

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_LAST_TX;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        return false;
    }

    stFpgaI2cCtl.bOpType = FPGA_OP_TYPE_INIT;

    if (false == Ctl(stFpgaI2cCtl.ucCtl)) {
        return false;
    }

    if (false == CheckAck()) {
        return false;
    }

    return true;
}




bool
CI2cLogicDriver::Read(uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    return m_stpBoardLogicPtr->I2cRead(uiAddr, pucBufW, uiLenW, pucBufR, uiLenR);
}

bool
CI2cLogicDriver::Write(uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    return m_stpBoardLogicPtr->I2cWrite(uiAddr, pucBufW, uiLenW);
}
#endif


bool
CSusi4I2c::Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    uint32_t uiCmd = 0;


    if (nullptr == pucBufW) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
    }

    uiCmd = pucBufW[0] << 8 | pucBufW[1];
    uiCmd = SUSI_I2C_ENC_EXT_CMD(uiCmd);

    status = SusiI2CWriteTransfer(id, (uint8_t)uiAddr, uiCmd, (uint8_t *)&(pucBufW[2]), uiLenW - 2);
    if (status != SUSI_STATUS_SUCCESS) {
        LOG_WRITE(EN_LOG_ERROR, "Write transfer failed. (0x%08X)", status);
        return false;
    }

    return true;
}


bool
CSusi4I2c::Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    uint32_t uiCmd = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CI2cSusi4Driver::Read ++");

    if (nullptr == pucBufW) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
    }

    if (nullptr == pucBufR) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufR is nullptr");
        return false;
    }

    if (2 != uiLenW) {
        LOG_WRITE(EN_LOG_ERROR, "i2c read word len error %d!", uiLenW);
        return false;
    }

    uiCmd = pucBufW[0] << 8 | pucBufW[1];
    uiCmd = SUSI_I2C_ENC_EXT_CMD(uiCmd);

    status = SusiI2CReadTransfer(id, (uint8_t)uiAddr, uiCmd, pucBufR, uiLenR);
    if (status != SUSI_STATUS_SUCCESS) {
        LOG_WRITE(EN_LOG_ERROR, "Read transfer failed. (0x%08X)", status);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CI2cSusi4Driver::Read --");

    return true;
}



bool
CSusi4I2c::WriteByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    uint32_t uiCmd = 0;
    uint8_t *pucData = NULL;

    if (nullptr == pucBufW) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
    }

    if (0 == uiLenW) {
        LOG_WRITE(EN_LOG_ERROR, "uiLenW is 0");
        return false;
    }

    if (1 == uiLenW) {
        uiCmd = SUSI_I2C_NO_CMD;
        pucData = pucBufW;
    } else {
        uiCmd = pucBufW[0];
        pucData = &pucBufW[1];
        uiLenW = uiLenW - 1;
    }

    status = SusiI2CWriteTransfer(id, (uint8_t)uiAddr, uiCmd, (uint8_t *)pucData, uiLenW);
    if (status != SUSI_STATUS_SUCCESS) {
        LOG_WRITE(EN_LOG_ERROR, "Write byte transfer failed. (0x%08X)", status);
        return false;
    }

    return true;
}


bool
CSusi4I2c::ReadByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    uint32_t uiCmd = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CI2cSusi4Driver::Read ++");

    if (nullptr == pucBufW) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr");
        return false;
    }

    if (nullptr == pucBufR) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufR is nullptr");
        return false;
    }

    if (1 != uiLenW) {
        LOG_WRITE(EN_LOG_ERROR, "i2c read byte len error %d!", uiLenW);
        return false;
    }

    uiCmd = pucBufW[0];

    status = SusiI2CReadTransfer(id, (uint8_t)uiAddr, uiCmd, pucBufR, uiLenR);
    if (status != SUSI_STATUS_SUCCESS) {
        LOG_WRITE(EN_LOG_ERROR, "Read byte transfer failed. (0x%08X)", status);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CI2cSusi4Driver::Read --");

    return true;
}

