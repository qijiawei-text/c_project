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

#include "MdioDriver.h"
/*
 #define REG_MDIO_VER          0x80
 #define REG_MDIO_CTRL         0x81
 #define REG_MDIO_DEV_ADDR     0x82
 #define REG_MDIO_REG_ADDR     0x83
 #define REG_MDIO_TX_DATA_HI   0x84
 #define REG_MDIO_TX_DATA_LO   0x85
 #define REG_MDIO_RX_DATA_HI   0x86
 #define REG_MDIO_RX_DATA_LO   0x87
 */
#define REG_MDIO_VER          0x100
#define REG_MDIO_CTRL         0x104
#define REG_MDIO_DEV_ADDR     0x108
#define REG_MDIO_REG_ADDR     0x10C
#define REG_MDIO_TX_DATA_HI   0x110
#define REG_MDIO_TX_DATA_LO   0x114
#define REG_MDIO_RX_DATA_HI   0x118
#define REG_MDIO_RX_DATA_LO   0x11C


/*
   `define FPC_PHY_SEL 16'h202C

   `define FPC_MDIO_CTRL 16'h2064
   `define FPC_MDIO_DEV_ADDR 16'h2068
   `define FPC_MDIO_REG_ADDR 16'h206C
   `define FPC_MDIO_TX_DATA_H 16'h2070
   `define FPC_MDIO_TX_DATA_L 16'h2074
   `define FPC_MDIO_RX_DATA_H 16'h2078
   `define FPC_MDIO_RX_DATA_L 16'h207C
 */


bool
CMdioFpgaDriver::Read(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
    uint8_t ucDataH = 0;
    uint8_t ucDataL = 0;
    uint8_t ucSta = 0;
    int32_t iTimeOut = 0;
    uint8_t ucDataW = 0;

    ucDataW = 0;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);
    CDelay::Delay(1, MS);

    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_DEV_ADDR, (uint8_t *)&ulPhyAddr, 1);
    CDelay::Delay(1, MS);

    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_REG_ADDR, (uint8_t *)&ulRegAddr, 1);
    CDelay::Delay(1, MS);

    ucDataW = 0x02;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);
    CDelay::Delay(10, US);

    m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_CTRL, &ucSta, 1);
    while (!(ucSta & 0x80)) {
        CDelay::Delay(1, MS);
        if (iTimeOut++ > 1000) {
            LOG_WRITE(EN_LOG_ERROR, "mdio read timeout!");
            return false;
        }

        m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_CTRL, &ucSta, 1);
    }

    m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_RX_DATA_HI, &ucDataH, 1);
    CDelay::Delay(1, MS);
    m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_RX_DATA_LO, &ucDataL, 1);

    ucDataW = 0;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);

    *pusData = (ucDataH << 8 | ucDataL);

    return true;
}
bool
CMdioFpgaDriver::Write(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
    uint8_t ucSta = 0;
    int32_t iTimeOut = 0;
    uint8_t ucDataW = 0;

    ucDataW = 0;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);
    CDelay::Delay(1, MS);

    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_DEV_ADDR, (uint8_t *)&ulPhyAddr, 1);
    CDelay::Delay(1, MS);

    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_REG_ADDR, (uint8_t *)&ulRegAddr, 1);
    CDelay::Delay(1, MS);

    ucDataW = (uint8_t)((usData & 0xff00) >> 8);
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_TX_DATA_HI, &ucDataW, 1);
    CDelay::Delay(1, MS);

    ucDataW = (usData & 0xff);
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_TX_DATA_LO, &ucDataW, 1);

    ucDataW = 0x03;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);
    CDelay::Delay(1, MS);

    m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_CTRL, &ucSta, 1);
    while (!(ucSta & 0x80)) {
        CDelay::Delay(1, MS);
        if (iTimeOut++ > 1000) {
            LOG_WRITE(EN_LOG_ERROR, "mdio write timeout!");
            return false;
        }

        m_stpDrvPtr->Read(m_uiBaseAddr + REG_MDIO_CTRL, &ucSta, 1);
    }

    ucDataW = 0;
    m_stpDrvPtr->Write(m_uiBaseAddr + REG_MDIO_CTRL, &ucDataW, 1);

    return true;
}

