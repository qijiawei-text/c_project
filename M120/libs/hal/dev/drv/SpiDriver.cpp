/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SpiDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SpiDriver.h"
pthread_mutex_t m_spi_mutex[4] = PTHREAD_MUTEX_INITIALIZER;

#define SPI_BUFFER_SIZE               (16384)

#if 0
#define SPI_M_CTRL_REG 0x49
#define SPI_M_ADDR_REG 0x4A
#define SPI_M_LEN_REG  0x4C
/* #define SPI_M_DATA_REG 0x14000 */
#define SPI_M_DATA_REG 0x2000
#endif

#define SPI_M_CTRL_OFFSET  (12)
#define SPI_M_DATA_OFFSET  (0x4000)
#if 1
#define SPI_M_CTRL_REG 0x80
#define SPI_M_ADDR_REG 0x84
#define SPI_M_LEN_REG  0x88
#define SPI_M_DATA_REG 0x100000
#endif

#if 0
#define SPI_M_CTRL_REG 0x8C
#define SPI_M_ADDR_REG 0x90
#define SPI_M_LEN_REG  0x94
#define SPI_M_DATA_REG 0x104000
#endif

#if 0
#define SPI_M_CTRL_REG 0x98
#define SPI_M_ADDR_REG 0x9C
#define SPI_M_LEN_REG  0xA0
#define SPI_M_DATA_REG 0x108000
#endif

#if 0
#define SPI_M_CTRL_REG 0xA4
#define SPI_M_ADDR_REG 0xA8
#define SPI_M_LEN_REG  0xAC
#define SPI_M_DATA_REG 0x10C000
#endif

/**
 * @brief define FPGA EDFA/OTDR control register
 */
#define SPI_M_CTRL_CS_EN(value) (value | 0x08)
#define SPI_M_CTRL_CS_DIS(value) (value & 0xFFFFFFF7)
#define SPI_M_CTRL_SET_W(value) (value | 0x04)
#define SPI_M_CTRL_SET_R(value) (value & 0xFFFFFFFB)
#define SPI_M_CTRL_SET_START_SEND(value) (value | 0x01)
#define SPI_M_CTRL_SET_STOP_SEND(value) (value & 0xFFFFFFFE)
#define SPI_M_CTRL_GET_SEND_FLAG(value) (value & 0x02)
#define SPI_M_CTRL_GET_STOP_SEND_CHECK(value) (value & 0x03)

#define SPI_M_CTRL_CHECK_ACK_COUNT   (2)
#define SPI_M_CTRL_ACK_W_FINISH      (1)
#define SPI_M_CTRL_ACK_R_READY       (1)

#define SPI_M_CTRL_READ_RETRY_MAX_COUNT  (6)


/**
 * @brief define FPGA EDFA/OTDR data ram write/read max len
 */
#define SPI_M_DATA_RAM_W_MAX_LEN    (0x2000)
#define SPI_M_DATA_RAM_R_MAX_LEN    (0x2000)
#define SPI_M_DATA_RAM_WRITE_OFFSET (0x0000)
#define SPI_M_DATA_RAM_READ_OFFSET  (0x2000)


/**
 * @brief define slave spi
 */

enum EN_FPGA_SPI_CHAR_LEN
{
    FPGA_SPI_CHAR_LEN_1BYTE = 0,
    FPGA_SPI_CHAR_LEN_2BYTE,
    FPGA_SPI_CHAR_LEN_3BYTE,
    FPGA_SPI_CHAR_LEN_4BYTE

};
#if 0
#define FPGA_CTRL_REG_SPI_M_VER                  0xA0
#define FPGA_CTRL_REG_SPI_M_SEL                  0xA2
#define FPGA_CTRL_REG_SPI_M_PROTO                0xA4
#define FPGA_CTRL_REG_SPI_M_TX_DATA1             0xA8
#define FPGA_CTRL_REG_SPI_M_TX_DATA2             0xA9
#define FPGA_CTRL_REG_SPI_M_TX_DATA3             0xAA
#define FPGA_CTRL_REG_SPI_M_TX_DATA4             0xAB
#define FPGA_CTRL_REG_SPI_M_RX_DATA1             0xAC
#define FPGA_CTRL_REG_SPI_M_RX_DATA2             0xAD
#define FPGA_CTRL_REG_SPI_M_RX_DATA3             0xAE
#define FPGA_CTRL_REG_SPI_M_RX_DATA4             0xAF


#define FPGA_MAIN_REG_BASE 0x10000

#define FPGA_MAIN_REG_SPI_M_VER                  FPGA_MAIN_REG_BASE + 0xA0
#define FPGA_MAIN_REG_SPI_M_SEL                  FPGA_MAIN_REG_BASE + 0xA2
#define FPGA_MAIN_REG_SPI_M_PRO                  FPGA_MAIN_REG_BASE + 0xA4
#define FPGA_MAIN_REG_SPI_M_OUT_DATA1            FPGA_MAIN_REG_BASE + 0xA8
#define FPGA_MAIN_REG_SPI_M_OUT_DATA2            FPGA_MAIN_REG_BASE + 0xA9
#define FPGA_MAIN_REG_SPI_M_OUT_DATA3            FPGA_MAIN_REG_BASE + 0xAA
#define FPGA_MAIN_REG_SPI_M_OUT_DATA4            FPGA_MAIN_REG_BASE + 0xAB
#define FPGA_MAIN_REG_SPI_M_IN_DATA1             FPGA_MAIN_REG_BASE + 0xAC
#define FPGA_MAIN_REG_SPI_M_IN_DATA2             FPGA_MAIN_REG_BASE + 0xAD
#define FPGA_MAIN_REG_SPI_M_IN_DATA3             FPGA_MAIN_REG_BASE + 0xAE
#define FPGA_MAIN_REG_SPI_M_IN_DATA4             FPGA_MAIN_REG_BASE + 0xAF
#else
#define FPGA_CTRL_REG_SPI_M_VER                  0x300
#define FPGA_CTRL_REG_SPI_M_SEL                  0x304
#define FPGA_CTRL_REG_SPI_M_PROTO                0x308
#define FPGA_CTRL_REG_SPI_M_TX_DATA1             0x30C
#define FPGA_CTRL_REG_SPI_M_TX_DATA2             0x310
#define FPGA_CTRL_REG_SPI_M_TX_DATA3             0x314
#define FPGA_CTRL_REG_SPI_M_TX_DATA4             0x318
#define FPGA_CTRL_REG_SPI_M_RX_DATA1             0x31C
#define FPGA_CTRL_REG_SPI_M_RX_DATA2             0x320
#define FPGA_CTRL_REG_SPI_M_RX_DATA3             0x324
#define FPGA_CTRL_REG_SPI_M_RX_DATA4             0x328


#define FPGA_MAIN_REG_BASE 0x0

#define FPGA_MAIN_REG_SPI_M_VER                  FPGA_MAIN_REG_BASE + 0x300
#define FPGA_MAIN_REG_SPI_M_SEL                  FPGA_MAIN_REG_BASE + 0x304
#define FPGA_MAIN_REG_SPI_M_PRO                  FPGA_MAIN_REG_BASE + 0x308
#define FPGA_MAIN_REG_SPI_M_OUT_DATA1            FPGA_MAIN_REG_BASE + 0x30C
#define FPGA_MAIN_REG_SPI_M_OUT_DATA2            FPGA_MAIN_REG_BASE + 0x310
#define FPGA_MAIN_REG_SPI_M_OUT_DATA3            FPGA_MAIN_REG_BASE + 0x314
#define FPGA_MAIN_REG_SPI_M_OUT_DATA4            FPGA_MAIN_REG_BASE + 0x318
#define FPGA_MAIN_REG_SPI_M_IN_DATA1             FPGA_MAIN_REG_BASE + 0x31C
#define FPGA_MAIN_REG_SPI_M_IN_DATA2             FPGA_MAIN_REG_BASE + 0x320
#define FPGA_MAIN_REG_SPI_M_IN_DATA3             FPGA_MAIN_REG_BASE + 0x324
#define FPGA_MAIN_REG_SPI_M_IN_DATA4             FPGA_MAIN_REG_BASE + 0x328
#endif

/**
 * @brief define slave spi retry time
 */

#define DEF_SPI_WR_RETRY_MAX                (512)


std::mutex CLogicSlaveSpi::ms_stMutex;
std::mutex CLogicMasterSpi::ms_stMutex;
std::mutex CLogicMasterWssSpi::ms_stMutex;

bool
CLogicSlaveSpi::__Ready(uint32_t ulChip)
{
    int32_t i = 0;
    uint8_t ucSpiRegHand = 0;
    uint32_t ulOffset = 0;

    if ((SPI_CTRL_FLASH == ulChip)
        || ((SPI_PA_FLASH == ulChip))
        || ((SPI_BA_FLASH == ulChip))) {
        ulOffset = 0x0;
    }

    if (!m_stpDrvPtr->Read(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
        goto ERR_LABEL;
    }

    ucSpiRegHand &= 0xFE;
    if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
        goto ERR_LABEL;
    }

    for (i = 0; i < DEF_SPI_WR_RETRY_MAX; i++) {
        if (!m_stpDrvPtr->Read(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
            goto ERR_LABEL;
        }

        if (0 == (ucSpiRegHand & 0x02)) {
            break;
        }
    }

    if (i >= DEF_SPI_WR_RETRY_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "spi ready time out error");
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;

}


bool
CLogicSlaveSpi::ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
                          uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t ucData = 0;
    uint8_t ucSpiRegHand = 0;
    uint8_t ucSpiRegCtl = 0;
    uint32_t ulOffset = 0;
    int32_t i = 0, j = 0;
    int iErrno = 0;

    ms_stMutex.lock();

    if (nullptr == m_stpDrvPtr || nullptr == pucBufW || nullptr == pucBufR) {
        iErrno = 1;
        goto ERR_LABEL;
    }

    /* SPI chip selection                                                       */
    if (SPI_MAIN_FLASH == uiChip) {
        ucSpiRegCtl = uiChip;
        if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } else if (SPI_CTRL_FLASH == uiChip) {
        ucSpiRegCtl = uiChip;
        if (!m_stpDrvPtr->Write(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 3;
            goto ERR_LABEL;
        }

        ulOffset = 0x0;
    } else if (SPI_PA_FLASH == uiChip) {
        if (!m_stpDrvPtr->Read(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 4;
            goto ERR_LABEL;
        }
        ucSpiRegCtl &= 0xF8;
        ucSpiRegCtl |= 0x02;
        if (!m_stpDrvPtr->Write(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 5;
            goto ERR_LABEL;
        }

        ulOffset = 0x0;
    } else if (SPI_BA_FLASH == uiChip) {
        if (!m_stpDrvPtr->Read(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 6;
            goto ERR_LABEL;
        }
        ucSpiRegCtl &= 0xF8;
        ucSpiRegCtl |= 0x03;
        if (!m_stpDrvPtr->Write(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        ulOffset = 0x0;
    }

    /* SPI character length                                                                                                     */
    if (!m_stpDrvPtr->Read(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
        iErrno = 8;
        goto ERR_LABEL;
    }

    ucSpiRegHand &= 0x0F;
    ucSpiRegHand |= FPGA_SPI_CHAR_LEN_1BYTE << 4;

    if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
        iErrno = 9;
        goto ERR_LABEL;
    }

    if (!__Ready(uiChip)) {
        iErrno = 10;
        goto ERR_LABEL;
    }

    for (i = 0; i < (int)stpRspHandler->Len(); i++) {
        /* Loads the transmission data											*/
        ucData = *(pucBufW + i);
        if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_TX_DATA1 + ulOffset, &ucData, 1)) {
            iErrno = 11;
            goto ERR_LABEL;
        }

        /* Start transmission													*/
        ucSpiRegHand |= 0x01;
        if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
            iErrno = 12;
            goto ERR_LABEL;
        }

        for (j = 0; j < DEF_SPI_WR_RETRY_MAX; j++) {
            if (!m_stpDrvPtr->Read(FPGA_CTRL_REG_SPI_M_PROTO + ulOffset, &ucSpiRegHand, 1)) {
                iErrno = 13;
                goto ERR_LABEL;
            }

            if (0 != (ucSpiRegHand & 0x02)) {
                break;
            }
        }

        if (j >= DEF_SPI_WR_RETRY_MAX) {
            iErrno = 14;
            LOG_WRITE(EN_LOG_ERROR, "spi read/write time out error");
            goto ERR_LABEL;
        }
        /* Gets the read data													*/
        ucData = 0;
        if (!m_stpDrvPtr->Read(FPGA_CTRL_REG_SPI_M_RX_DATA4 + ulOffset, &ucData, 1)) {
            iErrno = 15;
            goto ERR_LABEL;
        }

        if (NULL != pucBufR) {
            *(pucBufR + i) = ucData;
        }

        if (!__Ready(uiChip)) {
            iErrno = 16;
            goto ERR_LABEL;
        }
    }

    /* Disable CS															   */
    ucSpiRegCtl = 0;

    if (SPI_MAIN_FLASH == uiChip) {
        if (!m_stpDrvPtr->Write(FPGA_CTRL_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 17;
            goto ERR_LABEL;
        }

    } else {
        if (!m_stpDrvPtr->Write(FPGA_MAIN_REG_SPI_M_SEL, &ucSpiRegCtl, 1)) {
            iErrno = 18;
            goto ERR_LABEL;
        }
    }

    ms_stMutex.unlock();

    return true;

ERR_LABEL:
    ms_stMutex.unlock();
    LOG_WRITE(EN_LOG_ERROR, "spi logic send command error %d", iErrno);
    return false;

}


bool
CLogicMasterSpi::__SetDirR(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    if (!m_stpDrvPtr->ReadReg(SPI_M_CTRL_REG + uiOffset, &m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error: 0x%08x", SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    /* step1: enable contrl register direction to write or read*/
    m_uiCtrlValue = SPI_M_CTRL_CS_EN(m_uiCtrlValue);
    m_uiCtrlValue = SPI_M_CTRL_SET_R(m_uiCtrlValue);
    if (!m_stpDrvPtr->WriteReg(SPI_M_CTRL_REG + uiOffset, m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__SetDirW(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    if (!m_stpDrvPtr->ReadReg(SPI_M_CTRL_REG + uiOffset, &m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error: 0x%08x", SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    /* step1: enable contrl register direction to write or read*/
    m_uiCtrlValue = SPI_M_CTRL_CS_EN(m_uiCtrlValue);
    m_uiCtrlValue = SPI_M_CTRL_SET_W(m_uiCtrlValue);
    if (!m_stpDrvPtr->WriteReg(SPI_M_CTRL_REG + uiOffset, m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    return true;
}


bool
CLogicMasterSpi::__SetAddress(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    uint32_t uiValue = SPI_M_DATA_RAM_READ_OFFSET;

    if (!m_stpDrvPtr->Write(SPI_M_ADDR_REG + uiOffset, (uint8_t *)&uiValue, 4)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_ADDR_REG + uiOffset);
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__SetLen(uint32_t uiChip, uint32_t uiLen)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    if (!m_stpDrvPtr->Write(SPI_M_LEN_REG + uiOffset, (uint8_t *)&uiLen, 4)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_LEN_REG + uiOffset);
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__ReadLen(uint32_t uiChip, uint32_t &ruiLen)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    if (!m_stpDrvPtr->Read(SPI_M_LEN_REG + uiOffset, (uint8_t *)&ruiLen, 4)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_LEN_REG + uiOffset);
        return false;
    }

    return true;
}


bool
CLogicMasterSpi::__WriteData(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    uint32_t uiOffset = uiChip * SPI_M_DATA_OFFSET;
    if (!m_stpDrvPtr->Write(SPI_M_DATA_REG + uiOffset, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_DATA_REG + uiOffset);
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__StartSend(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    /* step 5: write control reg, FPGA start to send message.*/
    m_uiCtrlValue = SPI_M_CTRL_SET_START_SEND(m_uiCtrlValue);
    if (!m_stpDrvPtr->WriteReg(SPI_M_CTRL_REG + uiOffset, m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__WaitSendDone(uint32_t uiChip)
{
#define SEND_DONE_FLAG 1
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
#if 0
    CDelay::Delay(10, MS);
    if (!m_stpDrvPtr->ReadReg(SPI_M_CTRL_REG, &m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error: 0x%08x", SPI_M_CTRL_REG);
        return false;
    }

    if (0 == SPI_M_CTRL_GET_SEND_FLAG(m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Send Flag error. 0x%02x", m_uiCtrlValue);
        return false;
    }
#else
    uint32_t uiCnt = 0;
    uint8_t ucValue = 0;


    while (uiCnt <= 3) {

        m_stpDrvPtr->ReadBit(SPI_M_CTRL_REG + uiOffset, BIT_1, ucValue);

        if (SEND_DONE_FLAG == ucValue) {
            /* LOG_WRITE(EN_LOG_NOTICE, "break"); */
            break;
        }

        uiCnt++;

        if ( uiCnt >= 3) {
            LOG_WRITE(EN_LOG_ERROR, "Send Flag error. 0x%02x", ucValue);
            return false;
        }
        CDelay::Delay(10, MS);
    }


#endif
    return true;

}

bool
CLogicMasterSpi::__StopSend(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    m_uiCtrlValue = SPI_M_CTRL_SET_STOP_SEND(m_uiCtrlValue);
    m_uiCtrlValue = SPI_M_CTRL_CS_DIS(m_uiCtrlValue);

    if (!m_stpDrvPtr->WriteReg(SPI_M_CTRL_REG + uiOffset, m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    return true;

}

bool
CLogicMasterSpi::__CheckStatus(uint32_t uiChip)
{
    uint32_t uiOffset = uiChip * SPI_M_CTRL_OFFSET;
    /* step 8: read control[0] == 0 check . */
    if (!m_stpDrvPtr->ReadReg(SPI_M_CTRL_REG + uiOffset, &m_uiCtrlValue)) {
        LOG_WRITE(EN_LOG_ERROR, "Write error: 0x%08x", (uint32_t)SPI_M_CTRL_REG + uiOffset);
        return false;
    }

    if (SPI_M_CTRL_GET_STOP_SEND_CHECK(m_uiCtrlValue) != 0x00) {
        LOG_WRITE(EN_LOG_ERROR, "Data transfer error");
        return false;
    }

    return true;
}

bool
CLogicMasterSpi::__ReadData(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    uint32_t uiOffset = uiChip * SPI_M_DATA_OFFSET;
    /* step 9: write in buffer to data ram*/
    if (!m_stpDrvPtr->Read(SPI_M_DATA_REG + uiOffset, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error");
        return false;
    }

    return true;
}


bool
CLogicMasterSpi::__Read(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t aucReadCmd[3] = {0x0b, 0x01, 0x02}; /* uint8_t aucReadCmd[3] = {0x0b, 0xff, 0xff}; */
    uint8_t aucBuf[SPI_BUFFER_SIZE] = {0};
    uint32_t uiReadCmdLen = sizeof(aucReadCmd);
    uint32_t uiTotalLen = uiLen + 3;

    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return false;
    }

    if (uiTotalLen > SPI_BUFFER_SIZE) {
        LOG_WRITE(EN_LOG_ERROR, "Input len: %u out of range: %u", uiLen, SPI_M_DATA_RAM_R_MAX_LEN);
        return false;
    }

    if (!__SetDirR(uiChip)) {
        return false;
    }

    if (!__SetAddress(uiChip)) {
        return false;
    }

    if (!__SetLen(uiChip, uiTotalLen)) {
        return false;
    }

    if (!__WriteData(uiChip, aucReadCmd, uiReadCmdLen)) {
        return false;
    }

    if (!__StartSend(uiChip)) {
        return false;
    }

    if (!__WaitSendDone(uiChip)) {
        __StopSend(uiChip);
        return false;
    }

    if (!__StopSend(uiChip)) {
        return false;
    }

    if (!__CheckStatus(uiChip)) {
        return false;
    }


    if (!__ReadData(uiChip, aucBuf, uiTotalLen)) {
        return false;
    }

    memcpy(pucBuf, aucBuf + uiReadCmdLen, uiLen);

    stpRspHandler->Update(uiLen);

    return true;
}

bool
CLogicMasterSpi::__Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucWriteCmd[3] = {0x02, 0x01, 0x02};
    uint8_t aucBuf[SPI_BUFFER_SIZE] = {0};
    uint32_t uiWriteCmdLen = sizeof(aucWriteCmd);
    uint32_t uiTotalLen = uiLen + uiWriteCmdLen;

    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return false;
    }

    if (uiTotalLen > SPI_BUFFER_SIZE) {
        LOG_WRITE(EN_LOG_ERROR, "Input len: %u out of range: %u", uiLen, SPI_M_DATA_RAM_W_MAX_LEN);
        return false;
    }

    memcpy(aucBuf, aucWriteCmd, uiWriteCmdLen);
    memcpy(aucBuf + uiWriteCmdLen, pucBuf, uiLen);

    if (!__SetDirW(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetDirW error.");
        return false;
    }

    if (!__SetAddress(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetAddress error.");
        return false;
    }

    if (!__SetLen(uiChip, uiTotalLen)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetLen error.");
        return false;
    }
#if 0
    if (uiChip == 2) {
        for (int i = 0; i < 32; ++i) {
            printf("%02x ", *((uint8_t *)aucBuf + i));
            if (((i + 1) % 30) == 0) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
#endif
    if (!__WriteData(uiChip, aucBuf, uiTotalLen)) {
        LOG_WRITE(EN_LOG_ERROR, "__WriteData error.");
        return false;
    }

    if (!__StartSend(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__StartSend error.");
        return false;
    }

    if (!__WaitSendDone(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__WaitSendDone error.");
        __StopSend(uiChip);
        return false;
    }

    if (!__StopSend(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__StopSend error.");
        return false;
    }

    if (!__CheckStatus(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__CheckStatus error.");
        return false;
    }

    return true;

}

bool
CLogicMasterSpi::__ReadRaw(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t aucReadCmd[3] = {0x0b, 0x01, 0x02};
    uint8_t aucBuf[SPI_BUFFER_SIZE] = {0};
    uint32_t uiReadCmdLen = sizeof(aucReadCmd);
    uint32_t uiTotalLen = uiLen;

    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return false;
    }

    if (uiTotalLen > SPI_BUFFER_SIZE) {
        LOG_WRITE(EN_LOG_ERROR, "Input len: %u out of range: %u", uiLen, SPI_M_DATA_RAM_R_MAX_LEN);
        return false;
    }

    if (!__SetDirR(uiChip)) {
        return false;
    }

    if (!__SetAddress(uiChip)) {
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "uiLen:0x%x, uiTotalLen: 0x%08x", uiLen, uiTotalLen);
    if (!__SetLen(uiChip, uiTotalLen)) {
        return false;
    }

    if (!__WriteData(uiChip, aucReadCmd, uiReadCmdLen)) {
        return false;
    }

    if (!__StartSend(uiChip)) {
        return false;
    }

    if (!__WaitSendDone(uiChip)) {
        __StopSend(uiChip);
        return false;
    }

    if (!__StopSend(uiChip)) {
        return false;
    }

    if (!__CheckStatus(uiChip)) {
        return false;
    }


    if (!__ReadData(uiChip, aucBuf, uiTotalLen)) {
        return false;
    }
    memcpy(pucBuf, aucBuf + uiReadCmdLen, uiLen);

    stpRspHandler->Update(uiLen);

    return true;
}

bool
CLogicMasterSpi::__WriteRaw(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBuf[SPI_BUFFER_SIZE] = {0};
    uint32_t uiTotalLen = uiLen;
    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return false;
    }

    if (uiTotalLen > SPI_BUFFER_SIZE) {
        LOG_WRITE(EN_LOG_ERROR, "Input len: %u out of range: %u", uiLen, SPI_M_DATA_RAM_W_MAX_LEN);
        return false;
    }

    memcpy(aucBuf, pucBuf, uiLen);
    if (!__SetDirW(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetDirW error.");
        return false;
    }

    if (!__SetAddress(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetAddress error.");
        return false;
    }

    if (!__SetLen(uiChip, uiTotalLen)) {
        LOG_WRITE(EN_LOG_ERROR, "__SetLen error.");
        return false;
    }

    if (!__WriteData(uiChip, aucBuf, uiTotalLen)) {
        LOG_WRITE(EN_LOG_ERROR, "__WriteData error.");
        return false;
    }

    if (!__StartSend(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__StartSend error.");
        return false;
    }

    if (!__WaitSendDone(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__WaitSendDone error.");
        __StopSend(uiChip);
        return false;
    }

    if (!__StopSend(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__StopSend error.");
        return false;
    }

    if (!__CheckStatus(uiChip)) {
        LOG_WRITE(EN_LOG_ERROR, "__CheckStatus error.");
        return false;
    }

    return true;

}

bool
CLogicMasterSpi::ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint32_t uiRetryTimes = 0;
    GetMutex().lock();
    if (!__Write(uiChip, pucBufW, uiLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi write error. uiChip=%d,len = %d", uiChip, uiLenW);
        GetMutex().unlock();
        return false;
    }

    CDelay::Delay(5, MS);

    for (uiRetryTimes = 0; uiRetryTimes < stpRspHandler->RetryTimes(); uiRetryTimes++) {
        if (__Read(uiChip, (uint8_t *)stpRspHandler->Data(), stpRspHandler->Len(), stpRspHandler)) {
            if (stpRspHandler->Proc() != RSP_STATUS_OK) {
                if (stpRspHandler->InProgress()) {
                    LOG_WRITE(EN_LOG_DEBUG, "In Progress...");
                    CDelay::Delay(20, MS);
                    continue;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "Spi Read error. uiChip=%d", uiChip);
                    GetMutex().unlock();
                    return false;
                }
            }
            GetMutex().unlock();
            return true;
        }
    }

    if (uiRetryTimes >= stpRspHandler->RetryTimes()) {
        LOG_WRITE(EN_LOG_ERROR, "Peer device is busy. uiRetryTimes = %d, uiChip=%d", uiRetryTimes, uiChip);
        GetMutex().unlock();
        return false;
    }
    GetMutex().unlock();
    return true;
}

bool
CLogicMasterSpi::Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    GetMutex().lock();
    if (!__Write(uiChip, pucBuf, uiLen)) {
        GetMutex().unlock();
        return false;
    }
    GetMutex().unlock();

    return true;
}


bool
CLogicMasterSpi::Read(uint32_t uiChip, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler)
{
    GetMutex().lock();
    if (!__Read(uiChip, pucBuf, stpRspHandler->Len(), stpRspHandler)) {
        GetMutex().unlock();
        return false;
    }
    GetMutex().unlock();

    return true;
}



bool
CFtSpi::Read(uint32_t uiChip, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler)
{

    return m_stpSpiFt.Read(uiChip, pucBuf, stpRspHandler->Len());
}

bool
CFtSpi::Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{

    return m_stpSpiFt.Write(uiChip, pucBuf, uiLen);
}


bool
CFtSpi::ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
                  uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    return m_stpSpiFt.ReadWrite(uiChip, pucBufW, uiLenW, pucBufR, stpRspHandler->Len());
}

bool
CLogicMasterWssSpi::ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW,
                              uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint32_t uiRetryTimes = 0;
    LOG_WRITE(EN_LOG_NOTICE, "__Write...uiChip:%d", uiChip);

    ms_stMutex.lock();
    if (!__WriteRaw(uiChip, pucBufW, uiLenW)) {
        ms_stMutex.unlock();
        LOG_WRITE(EN_LOG_ERROR, "__Write error...");
        return false;
    }

    CDelay::Delay(5, MS);
    LOG_WRITE(EN_LOG_NOTICE, "__Read...uiChip:%d", uiChip);
    for (uiRetryTimes = 0; uiRetryTimes < stpRspHandler->RetryTimes(); uiRetryTimes++) {
        if (__ReadRaw(uiChip, (uint8_t *)stpRspHandler->Data(), stpRspHandler->Len(), stpRspHandler)) {
            if (stpRspHandler->Proc() != RSP_STATUS_OK) {
                if (stpRspHandler->InProgress()) {
                    LOG_WRITE(EN_LOG_NOTICE, "In Progress...");
                    CDelay::Delay(20, MS);
                    continue;
                } else {
                    ms_stMutex.unlock();
                    LOG_WRITE(EN_LOG_ERROR, "Spi Read error.");
                    return false;
                }
            }
            ms_stMutex.unlock();
            return true;
        }
    }
    ms_stMutex.unlock();

    if (uiRetryTimes >= stpRspHandler->RetryTimes()) {
        LOG_WRITE(EN_LOG_ERROR, "Peer device is busy. uiRetryTimes = %d", uiRetryTimes);
        return false;
    }

    return true;
}

bool
CLogicMasterWssSpi::Write(uint32_t uiChip, uint8_t *pucBuf, uint32_t uiLen)
{
    if (!__WriteRaw(uiChip, pucBuf, uiLen)) {
        return false;
    }

    return true;
}


bool
CLogicMasterWssSpi::Read(uint32_t uiChip, uint8_t *pucBuf, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if (!__ReadRaw(uiChip, pucBuf, stpRspHandler->Len(), stpRspHandler)) {
        return false;
    }

    return true;
}

