/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Logic.cpp
   Author:        Zhu Weian
   Date:          2020-10-19
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-06] [1.0] [Creator]


*****************************************************************************************************/
#include "Logic.h"
#include <sstream>
#include "File.h"
#include "DevThread.h"


#define VERSION_FPGA_REG 0x04
#define VERSION_CPLD_REG 0x1000

#define SECTION_ADDR_A           (0x010000)                          /* 64KB                                                    */
#define SECTION_ADDR_B           (0x800000)                          /* 8MB                                                     */



static const uint8_t m_aucOldHead[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 0x99, 0x55, 0x66, 0x31, 0xE1, 0xFF, 0xFF, 0x32, 0x61, 0x00, 0x00,
    0x32, 0x81, 0x03, 0x01, 0x32, 0xA1, 0x00, 0x00, 0x32, 0xC1, 0x03, 0x80, 0x32, 0xE1, 0x00, 0x00,
    0x30, 0xA1, 0x00, 0x00, 0x33, 0x01, 0x21, 0x00, 0x32, 0x01, 0x00, 0x1F, 0x30, 0xA1, 0x00, 0x0E,
    0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};


static const uint8_t m_aucNewHead[] =
{
    0xFF, 0xFF, 0xFF, 0xFF,     /* DUMMYWORD */
    0xAA, 0x99, 0x55, 0x66,     /* SYNCWORD */
    0x20, 0x00, 0x00, 0x00,     /* Type 1 NO OP */
    0x30, 0x02, 0x00, 0x01,     /* Write WBSTAR cmd */
    0x00, 0x01, 0x00, 0x00,     /* Addr in SPI Flash of MultiBoot image address */
    0x30, 0x00, 0x80, 0x01,     /* Write CMD */
    0x00, 0x00, 0x00, 0x0F,     /* Write IPROG */
    0x20, 0x00, 0x00, 0x00,     /* Type 1 NO OP */
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};


bool
CCpld::GetVersion(char *pcVersion)
{


#if 1
    uint8_t aucReadBuf[4];

    memset(aucReadBuf, 0, sizeof(aucReadBuf));


    m_stpLogicDrv->ReadRam(VERSION_CPLD_REG, aucReadBuf, sizeof(aucReadBuf));

    snprintf(pcVersion, MFG_INFO_LEN_MAX - 1, "%01x.%02x.%02x%02x",
             aucReadBuf[3], aucReadBuf[2], aucReadBuf[1], aucReadBuf[0]);

    LOG_WRITE(EN_LOG_NOTICE, "CPLD Version: %s", pcVersion);

    printf("CPLD Version: %s\n", pcVersion);

#else

    uint8_t ucVerH = 0;
    uint8_t ucVerL = 0;
    uint8_t ucVerFst = 0;


    if ( false == m_stpLogicDrv->ReadReg(0x04, &ucVerH)) {
        return false;
    }

    if ( false == m_stpLogicDrv->ReadReg(0x05, &ucVerL)) {
        return false;
    }

    if ( false == m_stpLogicDrv->ReadReg(0x07, &ucVerFst)) {
        return false;
    }

    snprintf(pcVersion, MFG_INFO_LEN_MAX - 1, "%d.%02d.%04d", ucVerFst, ucVerL, ucVerH);

#endif

    return true;
}

bool
CCpld::Upgrade(const char *c_pcFilePath)
{
    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;
#if 1
    LOG_WRITE(EN_LOG_NOTICE, "CPLD Upgrade:c_pcFilePath:%s", c_pcFilePath);
    pucBuffer = (uint8_t *)CFile::Malloc(c_pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", c_pcFilePath, iFileLen);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CPLD::Upgrade +++,m_uiHwSubIdx:%d", m_uiHwSubIdx);
#if 0
    /* fpc cpld,need enable */
    if (1 == m_uiHwSubIdx) {
        LOG_WRITE(EN_LOG_NOTICE, "CPLD::Upgrade set jtag enable +++");
        m_stpLogicDrv->WriteBit(RESET_REG, BIT_0, 1);
        LOG_WRITE(EN_LOG_NOTICE, "CPLD::Upgrade set jtag enable ---");
    }
#endif

    m_stpJtag->Upgrade(pucBuffer, iFileLen);

#if 0
    if (1 == m_uiHwSubIdx) {
        LOG_WRITE(EN_LOG_NOTICE, "CPLD::Upgrade set jtag disable +++");
        m_stpLogicDrv->WriteBit(RESET_REG, BIT_0, 0);
        LOG_WRITE(EN_LOG_NOTICE, "CPLD::Upgrade set jtag disable ---");
    }
#endif
#else
    /* for debug */
    LOG_WRITE(EN_LOG_NOTICE, "CPLD Upgrade:m_stpJtag:%p, c_pcFilePath:%s,m_uiBaseAddr:%d,", m_stpJtag, c_pcFilePath, m_uiBaseAddr);


#endif
    return true;
}

bool
CCpld::Reset()
{
    m_stpJtag->Reset();

    return true;
}

bool
CFpga::GetVersion(char *pcVersion)
{
    uint8_t aucReadBuf[4];

    memset(aucReadBuf, 0, sizeof(aucReadBuf));


    m_stpLogicDrv->ReadRam(VERSION_FPGA_REG, aucReadBuf, sizeof(aucReadBuf));

    snprintf(pcVersion, MFG_INFO_LEN_MAX - 1, "%01x.%02x.%02x%02x",
             aucReadBuf[3], aucReadBuf[2], aucReadBuf[1], aucReadBuf[0]);

    LOG_WRITE(EN_LOG_NOTICE, "CPLD Version: %s", pcVersion);

    printf("FPGA Version: %s\n", pcVersion);

    /* add here temporary for debug: enable pci int */
    uint8_t ucDataW = 0x11;
    m_stpLogicDrv->Write(0x8, &ucDataW, 1);

    return true;
}

bool
CFpga::Upgrade(const char *c_pcFilePath)
{
    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;

    m_stpLogicDrv->WriteBit(CLK_BUF_REG, BIT_4, 1);

    LOG_WRITE(EN_LOG_NOTICE, "c_pcFilePath = %s", c_pcFilePath);

    pucBuffer = (uint8_t *)CFile::Malloc(c_pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", c_pcFilePath, iFileLen);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "iFileLen = %d", iFileLen);

    if (!m_stpFlash->WriteImage(pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) write to flash error. iFileLen = %d", c_pcFilePath, iFileLen);
        return false;
    }

    return true;
}

/* #define UPGRADE_REG (0x64) */

bool
CFpga::PreUpgrade()
{

    bool bRet = true;
    uint8_t aucBuf[4] = {0};
    uint32_t uiPrimaryAddr = 0, uiBackupAddr = 0;

    bRet = m_stpLogicDrv->ReadRam(VERSION_FPGA_REG, aucBuf, sizeof(aucBuf));
    if (!bRet) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "aucBuf[0] = %d, aucBuf[1] = %d, aucBuf[2] = %d, aucBuf[3] = %d", aucBuf[0], aucBuf[1], aucBuf[2], aucBuf[3]);

    m_bNewFpga = (aucBuf[0] == 1) ? true : false;

    if (m_bNewFpga) {

        /* if new FPGA, open upgrade buffer first */
        /* m_stpLogicDrv->WriteBit(UPGRADE_REG, BIT_0, 1); */
        m_stpLogicDrv->WriteBit(CLK_BUF_REG, BIT_4, 1);

        m_ucHeadLen = sizeof(m_aucNewHead);
        memcpy(m_aucHead, m_aucNewHead, m_ucHeadLen);

        m_ucPrimaryOffset = 17;
        m_ucBackupOffset = 17;

        LOG_WRITE(EN_LOG_NOTICE, "m_ucPrimaryOffset = %d, m_ucBackupOffset = %d", m_ucPrimaryOffset, m_ucBackupOffset);
    } else {
        m_ucHeadLen = sizeof(m_aucOldHead);
        memcpy(m_aucHead, m_aucOldHead, m_ucHeadLen);

        m_ucPrimaryOffset = 19;
        m_ucBackupOffset = 27;
    }

    m_bIsPrimary = IsPrimary();

    if (m_bIsPrimary) {
        m_uiPrimaryAddr = SECTION_ADDR_B;
        m_uiBackupAddr = SECTION_ADDR_A;
    } else {
        m_uiPrimaryAddr = SECTION_ADDR_A;
        m_uiBackupAddr = SECTION_ADDR_B;
    }


    LOG_WRITE(EN_LOG_NOTICE, "m_bIsPrimary = %d, m_bNewFpga = %d", m_bIsPrimary, m_bNewFpga);

    LOG_WRITE(EN_LOG_NOTICE, "m_uiPrimaryAddr = %d, m_uiBackupAddr = %d", m_uiPrimaryAddr, m_uiBackupAddr);

    LOG_WRITE(EN_LOG_NOTICE, "m_ucPrimaryOffset = %d, m_ucBackupOffset = %d", m_ucPrimaryOffset, m_ucBackupOffset);

    if (m_bNewFpga) {
        uiPrimaryAddr = m_uiPrimaryAddr;
        m_aucHead[m_ucPrimaryOffset] = uiPrimaryAddr >> 16;

        printf("uiPrimaryAddr = %d\n", uiPrimaryAddr);
    } else {

        uiPrimaryAddr = m_uiPrimaryAddr;
        uiBackupAddr = m_uiBackupAddr;
        m_aucHead[m_ucPrimaryOffset] = uiPrimaryAddr >> 16;
        m_aucHead[m_ucBackupOffset] = uiBackupAddr >> 16;
    }

    return true;
}
#if 0
bool
CFpga::Upgrade(const char *c_pcFilePath)
{
    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;
    bool bRet = false;

    PreUpgrade();

    pucBuffer = (uint8_t *)CFile::Malloc(c_pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", c_pcFilePath, iFileLen);
        goto END_LABEL;
    }

    bRet = WriteImage(pucBuffer, iFileLen);
    if (!bRet) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) Write to flash error. iFileLen = %d", c_pcFilePath, iFileLen);
        goto END_LABEL;
    }

END_LABEL:

    PostUpgrade();
    DEF_SAFE_FREE(pucBuffer);

    return bRet;
}
#endif
bool
CFpga::PostUpgrade()
{
    /* if new FPGA, close upgrade buffer in the end */
    if (m_bNewFpga) {
        LOG_WRITE(EN_LOG_NOTICE, "close buffer.");
        /* m_stpLogicDrv->WriteBit(UPGRADE_REG, BIT_0, 0); */
        m_stpLogicDrv->WriteBit(CLK_BUF_REG, BIT_4, 0);
    }

    return true;
}


bool
CFpga::IsPrimary()
{
    uint8_t aucHeadInFlash[128] = {0};
    uint8_t aucTmp[1024] = {0};

    /* Read the fpga image header from spi flash                                */
    if (!m_stpFlash->ReadData(0, aucHeadInFlash, m_ucHeadLen)) {

        LOG_WRITE(EN_LOG_ERROR, "Failed to read fpga image header from spi flash");
        return false;
    }

    for (int i = 0; i < m_ucHeadLen; i++) {
        sprintf((char *)aucTmp + i * 5, "0x%02x ", aucHeadInFlash[i]);
    }
    LOG_WRITE(EN_LOG_NOTICE, "%s", aucTmp);


    for (int i = 0; i < m_ucHeadLen; i++) {
        printf("0x%02x ", aucHeadInFlash[i]);
    }

    printf("\n");


    if (m_bNewFpga) {

        LOG_WRITE(EN_LOG_NOTICE, "m_ucPrimaryOffset = %d, m_ucBackupOffset = %d", m_ucPrimaryOffset, m_ucBackupOffset);
        LOG_WRITE(EN_LOG_NOTICE, "aucHeadInFlash[m_ucPrimaryOffset] = %d, m_aucHead[m_ucPrimaryOffset] = %d", aucHeadInFlash[m_ucPrimaryOffset], m_aucHead[m_ucPrimaryOffset]);
        if (aucHeadInFlash[m_ucPrimaryOffset] == m_aucHead[m_ucPrimaryOffset]) {
            return true;
        }
    } else {
        if ((aucHeadInFlash[m_ucPrimaryOffset] == m_aucHead[m_ucPrimaryOffset]) &&
            (aucHeadInFlash[m_ucBackupOffset] == m_aucHead[m_ucBackupOffset])) {
            return true;
        }

    }



    return false;

}


bool
CFpga::WriteImage(uint8_t *pucBuf, uint32_t ulLen)
{
    uint8_t aucTmp[1024] = {0};


    printf("m_uiPrimaryAddr = 0x%x\n", m_uiPrimaryAddr);
    if (!m_stpFlash->WriteData(m_uiPrimaryAddr, pucBuf, ulLen)) {
        goto ERR_LABEL;
    }

    if (!m_stpFlash->WriteData(0, m_aucHead, m_ucHeadLen)) {
        goto ERR_LABEL;
    }

    for (int i = 0; i < m_ucHeadLen; i++) {
        sprintf((char *)aucTmp + i * 5, "0x%02x ", m_aucHead[i]);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s", aucTmp);

    return true;

ERR_LABEL:

    return false;
}

#if 0
bool
CFpga::Reset()
{
#define RESET_FPGA_REG 0x40

    LOG_WRITE(EN_LOG_NOTICE, "CFpga::Reset +++");

    m_stpLogicDrv->WriteBit(RESET_FPGA_REG, BIT_4, 0);

    CDelay::Delay(1, S);

    m_stpLogicDrv->WriteBit(RESET_FPGA_REG, BIT_4, 1);

    LOG_WRITE(EN_LOG_NOTICE, "CFpga::Reset ---");

    return true;
}
#else
bool
CFpga::Reset()
{
#define RESET_FPGA_REG 0x7C

    LOG_WRITE(EN_LOG_NOTICE, "CFpga::Reset +++");

    LOG_WRITE(EN_LOG_NOTICE, "SetForceIdle true ---");
    CDevThread::SetForceIdle(true);
    LOG_WRITE(EN_LOG_NOTICE, "SendEvent to monitord true ---");
    if (CEvent::SendEvent((char *)DEF_MONITORD_EVENT_SOCKET_PATH, DEF_EVENT_ID_FORCEIDLE, 1)) {
        LOG_WRITE(EN_LOG_NOTICE, "Send Monitord  event ok.");
    }
    sleep(1);

    LOG_WRITE(EN_LOG_NOTICE, "reset fpga ---");
    m_stpLogicDrv->WriteBit(CLK_BUF_REG, BIT_4, 0);
    m_stpLogicDrv->WriteBit(RESET_FPGA_REG, BIT_0, 0);

    LOG_WRITE(EN_LOG_NOTICE, "closepcie ---");
    m_stpLogicDrv->close();

    for (int i = 0; i < 15; i++) {
        CDelay::Delay(1, S);
        LOG_WRITE(EN_LOG_NOTICE, "CFpga::Reset ---,sleep %d s", i);
    }

    LOG_WRITE(EN_LOG_NOTICE, "reopenpcie ---");
    m_stpLogicDrv->reopen();
    LOG_WRITE(EN_LOG_NOTICE, "SendEvent to monitord false ---");
    if (CEvent::SendEvent((char *)DEF_MONITORD_EVENT_SOCKET_PATH, DEF_EVENT_ID_FORCEIDLE, 0)) {
        LOG_WRITE(EN_LOG_NOTICE, "Send Monitord  event ok.");
    }
    LOG_WRITE(EN_LOG_NOTICE, "SetForceIdle false ---");
    CDevThread::SetForceIdle(false);

    LOG_WRITE(EN_LOG_NOTICE, "CFpga::Reset ---");

    return true;
}
#endif

