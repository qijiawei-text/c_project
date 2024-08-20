/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicSpi.cpp
   Author:        Zhu Weian
   Date:          2020-09-30
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-30] [1.0] [Creator]


*****************************************************************************************************/

#include "LogicSpi.h"


enum ESpiReg
{
    SPI_ACK_REG = 0x50,
    SPI_REQ_REG = 0x54,
    SPI_STA_REG = 0x58,
    SPI_CMD_REG = 0x110000,
    SPI_RSP_REG = 0x111800,
};

enum ESpiChannel
{
    SLAVE_SPI_OCM0,
    SLAVE_SPI_OCM1,
    SLAVE_SPI_OPS,
    SLAVE_SPI_CNT,
};

enum ESpiProto
{
    SPI_TIME_OUT_CNT          = 1,
    SPI_WAIT_ACK_TIME_OUT     = 1000,    /* 600ms */
    SPI_DEV_NACK              = 0,
    SPI_DEV_ACK               = 1,
    SPI_CMD_NREQ              = 0,
    SPI_CMD_REQ               = 1,
    SPI_STATUS_NONE           = 0,
    SPI_STATUS_SYNC           = 0xA5,
    SPI_RSP_DATA_LEN_IDX      = 1,
    SPI_RSP_DATA_LEN_LEN      = 2,
    SPI_REG_RSP_SIZE          = 0x2800,
};


bool
CLogicSpi::ReadWrite(uint32_t uiChipIdx, uint8_t *pucBufW, uint32_t uiLenW,
                     uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{

    uint32_t uiTimeoutCnt = 0;
    uint8_t ucData = 0;
    bool bRet = true;
    uint32_t uiOffsetReg = 0;
    uint32_t uiOffsetRam = 0;
    if (uiChipIdx > SLAVE_SPI_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "CLogicSpi::ReadWrite uiChipIdx:%d error", uiChipIdx);
        return false;
    }
    ms_stMutex.lock();
    uiOffsetReg = uiChipIdx * 12;
    uiOffsetRam = uiChipIdx * 0x4000;

    LOG_WRITE(EN_LOG_DEBUG, "CLogicSpi::ReadWrite +++");

    LOG_WRITE(EN_LOG_DEBUG, "SPI_REQ_REG = 0X%02X, SPI_STA_REG = 0X%02X, uiLenR = %d ", SPI_REQ_REG + uiOffsetReg, SPI_STA_REG + uiOffsetReg, stpRspHandler->Len());

    /* init req and status */
    m_stpLogicDrv->WriteReg(SPI_REQ_REG + uiOffsetReg, SPI_CMD_NREQ);
    m_stpLogicDrv->WriteReg(SPI_STA_REG + uiOffsetReg, SPI_STATUS_NONE);

    /* send the cmd frame */
    for (uiTimeoutCnt = 0; uiTimeoutCnt < SPI_TIME_OUT_CNT; uiTimeoutCnt++) {
        m_stpLogicDrv->ReadReg(SPI_ACK_REG + uiOffsetReg, &ucData);

        if (SPI_DEV_NACK == (ucData & 0x01)) {
            /* write cmd into cmd region */
            m_stpLogicDrv->WriteRam(SPI_CMD_REG + uiOffsetRam, pucBufW, uiLenW);

            /* req = 1 */
            m_stpLogicDrv->WriteReg(SPI_REQ_REG + uiOffsetReg, SPI_CMD_REQ);

            /* wait ack */
            uint32_t uiCnt = 0;
            m_stpLogicDrv->ReadReg(SPI_ACK_REG + uiOffsetReg, &ucData);
            while (SPI_DEV_NACK == (ucData & 0x01)) {
                if (SPI_WAIT_ACK_TIME_OUT == uiCnt) {
                    LOG_WRITE(EN_LOG_ERROR, "time out. uiChipIdx:%d, uiTimeoutCnt:%d, uiCnt:%d", uiChipIdx, uiTimeoutCnt, uiCnt);
                    goto TIME_OUT;
                }

                CDelay::Delay(1, MS);

                uiCnt++;

                m_stpLogicDrv->ReadReg(SPI_ACK_REG + uiOffsetReg, &ucData);
            }


            /* LOG_WRITE(EN_LOG_NOTICE, "ack = %d", ucData); */

            /* read response from rsp region */

#if 0
            ReadReg(m_uiBaseAddr + MAIN_BOARD_LOGIC_REG_OCM_RSP_OFFSET + MAIN_BOARD_LOGIC_RSP_DATA_LEN_IDX, &ucData);
            uiRspLen = (uint32_t)ucData;
            LOG_WRITE(EN_LOG_ERROR, "dataLen[1] = %d", ucData);

            ReadReg(m_uiBaseAddr + MAIN_BOARD_LOGIC_REG_OCM_RSP_OFFSET + MAIN_BOARD_LOGIC_RSP_DATA_LEN_IDX + 1, &ucData);
            uiRspLen = (uiRspLen << 8) | (uint32_t)ucData;
            LOG_WRITE(EN_LOG_ERROR, "dataLen[0] = %d", ucData);

            LOG_WRITE(EN_LOG_ERROR, "uiRspLen = %d", uiRspLen);

            if (uiRspLen > MAIN_BOARD_LOGIC_REG_RSP_SIZE) {
                goto TIME_OUT;
            }
#endif

            m_stpLogicDrv->ReadRam(SPI_RSP_REG + uiOffsetRam, pucBufR, stpRspHandler->Len());

            /* ReadRam(m_uiBaseAddr + MAIN_BOARD_LOGIC_REG_OCM_RSP_OFFSET, pucRead, uiRspLen); */

            /* req = 0 */
            m_stpLogicDrv->WriteReg(SPI_REQ_REG + uiOffsetReg, SPI_CMD_NREQ);

            /* wait ack to be cleared */
            uiCnt = 0;
            m_stpLogicDrv->ReadReg(SPI_ACK_REG + uiOffsetReg, &ucData);
            while (SPI_DEV_ACK == (ucData & 0x01)) {
                if (SPI_WAIT_ACK_TIME_OUT == uiCnt) {
                    LOG_WRITE(EN_LOG_NOTICE, "TIME OUT, uiChipIdx:%d, uiTimeoutCnt:%d, uiCnt:%d", uiChipIdx, uiTimeoutCnt, uiCnt);
                    goto TIME_OUT;
                }

                CDelay::Delay(1, MS);

                uiCnt++;

                m_stpLogicDrv->ReadReg(SPI_ACK_REG + uiOffsetReg, &ucData);
            }

            break;
        }

TIME_OUT:

        /* Device should keep sync */
        m_stpLogicDrv->WriteReg(SPI_RSP_REG + uiOffsetRam, SPI_CMD_NREQ);
        m_stpLogicDrv->WriteReg(SPI_STA_REG + uiOffsetReg, SPI_STATUS_SYNC);
        /* wait sync */
        CDelay::Delay(1, S);
        /* clear status */
        m_stpLogicDrv->WriteReg(SPI_STA_REG + uiOffsetReg, SPI_STATUS_NONE);
    }

    if (SPI_TIME_OUT_CNT == uiTimeoutCnt) {
        /* req = 0 */
        m_stpLogicDrv->WriteReg(SPI_REQ_REG + uiOffsetReg, SPI_CMD_NREQ);

        bRet = false;
    }

    ms_stMutex.unlock();
    return bRet;

}

