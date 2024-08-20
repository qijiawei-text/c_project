#include "EEprom.h"
#include "RspHandler.h"

#if 0
bool
CEEprom::Read(uint32_t uiAddrR, uint8_t *pucBufR, uint32_t uiLenR)
{
    uint8_t aucCmdBuf[4] = {0};
    uint32_t uiPageLen = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Read +++");

    if (nullptr == pucBufR) {
        LOG_WRITE(EN_LOG_ERROR, "pucBuf is nullptr.");
        return false;
    }

    do {
        LOG_WRITE(EN_LOG_DEBUG, "Read uiAddrR = 0x%02X uiLen = %d.", uiAddrR, uiLenR);

        uiPageLen = __PageLenR(uiLenR, uiAddrR);

        m_stpProtoPtr->ReadCmd(aucCmdBuf, uiAddrR);

        __Read(uiAddrR, aucCmdBuf, pucBufR, uiPageLen);

        uiAddrR += uiPageLen;
        pucBufR += uiPageLen;
        uiLenR -= uiPageLen;

        /* LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen); */

    } while (uiLenR);

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Read ---");

    return true;
}

bool
CEEprom::Write(uint32_t uiAddrW, uint8_t *pucBufW, uint32_t uiLenW)
{
    uint8_t aucCmdBuf[2] = {0};
    uint8_t aucBufW[DEF_EE_128KB_PAGE_LEN + 4] = {0};
    uint32_t uiPageLenW = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write +++");

    do {

        LOG_WRITE(EN_LOG_DEBUG, "Write uiAddrW = 0x%02X uiLen = %d.", uiAddrW, uiLenW);
        uiPageLenW = __PageLenW(uiLenW, uiAddrW);

        m_stpProtoPtr->WriteCmd(aucCmdBuf, uiAddrW);

        memcpy(aucBufW, aucCmdBuf, 2);
        memcpy(&aucBufW[2], pucBufW, uiPageLenW);

        __Write(uiAddrW, aucBufW, uiPageLenW + 2);

        uiAddrW += uiPageLenW;
        pucBufW += uiPageLenW;
        uiLenW -= uiPageLenW;

        CDelay::Delay(50, MS);
        /* LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen); */
    } while (uiLenW);

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write ---");

    return true;
}


bool
CI2cEEprom::__Read(uint32_t uiAddrR, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t uiLenR)
{
    uint8_t ucDevAddr = 0;
    uint8_t ucLenW = 2;


    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    ucDevAddr = (m_uiDevAddr & 0xFD) | ((uiAddrR >> 15) & 0x02);

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, ucDevAddr, pucBufW, ucLenW, pucBufR, uiLenR)) {
        LOG_WRITE(EN_LOG_ERROR, "Read m_uiHwSubIdx = %d, ucDevAddr = %02x, uiLenR = %d error.", m_uiHwSubIdx, ucDevAddr, uiLenR);
        return false;
    }

    return true;
}

bool
CI2cEEprom::__Write(uint32_t uiAddrW, uint8_t *pucBufW, uint32_t uiLenW)
{
    uint8_t ucDevAddr = 0;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    ucDevAddr = (m_uiDevAddr & 0xFD) | ((uiAddrW >> 15) & 0x02);

    if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, ucDevAddr, pucBufW, uiLenW)) {
        return false;
    }


    return true;
}

uint32_t
CI2cEEprom::__PageLenR(uint32_t uiLenR, uint32_t uiAddrR)
{
    uint32_t uiPageLenR = 0;

    uiPageLenR = m_stpI2cPtr->PageLenR(uiLenR, uiAddrR);

    return uiPageLenR;
}

uint32_t
CI2cEEprom::__PageLenW(uint32_t uiLenW, uint32_t uiAddrW)
{
    uint32_t uiPageLenW = 0;

    uiPageLenW = m_stpI2cPtr->PageLenR(uiLenW, uiAddrW);

    return uiPageLenW;
}
#endif

#if 1

bool
CI2cEEprom::Read(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBuf[4] = {0};
    uint8_t ucLen = 0;
    uint8_t ucDevAddr = 0;
    uint32_t uiPageLen = 0;

    /* LOG_WRITE(EN_LOG_NOTICE, "CEEprom::Read +++"); */

    if (nullptr == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "pucBuf is nullptr.");
        return false;
    }

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    do {

        uiPageLen = m_stpI2cPtr->PageLenR(uiLen, uiAddr);
        /* LOG_WRITE(EN_LOG_NOTICE, "Read m_uiDevAddr = 0x%02X uiAddrR = 0x%02X uiLen = %d.", m_uiDevAddr, uiAddr, uiLen); */

        ucDevAddr = (m_uiDevAddr & 0xFD) | ((uiAddr >> 15) & 0x02);

        aucBuf[0] = (uiAddr >> 8) & 0xFF;
        aucBuf[1] = uiAddr & 0xFF;
        ucLen = 2;

        /* LOG_WRITE(EN_LOG_NOTICE, "m_uiHwSubIdx = 0x%02X, ucDevAddr = 0x%02X", m_uiHwSubIdx, ucDevAddr); */
        if (!m_stpI2cPtr->Read(m_uiHwSubIdx, ucDevAddr, aucBuf, ucLen, pucBuf, uiPageLen)) {
            LOG_WRITE(EN_LOG_ERROR, "Read m_uiHwSubIdx = %d, ucDevAddr=%02x, uiPageLen=%d error.", m_uiHwSubIdx, ucDevAddr, uiPageLen);
            return false;
        }

        uiAddr += uiPageLen;
        pucBuf += uiPageLen;
        uiLen -= uiPageLen;

        /* LOG_WRITE(EN_LOG_NOTICE, "uiLen = %d", uiLen); */

    } while (uiLen);

    /* LOG_WRITE(EN_LOG_NOTICE, "CEEprom::Read ---"); */

    return true;
}


bool
CI2cEEprom::Write(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBuf[DEF_EE_128KB_PAGE_LEN + 4] = {0};
    uint32_t uiPageLen = 0;
    uint8_t ucDevAddr = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write +++ uiLen  =%d", uiLen);

    if (nullptr == m_stpI2cPtr) {
        return false;
    }

    do {

        uiPageLen = m_stpI2cPtr->PageLenW(uiLen, uiAddr);
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiDevAddr = 0x%02X uiAddrW = 0x%02X uiLen = %d uiPageLen = %d.", m_uiDevAddr, uiAddr, uiLen, uiPageLen);

        /* Access bit1                                                          */
        ucDevAddr = (m_uiDevAddr & 0xFD) | ((uiAddr >> 15) & 0x02);

        aucBuf[0] = (uiAddr >> 8) & 0xFF;
        aucBuf[1] = uiAddr & 0xFF;
        memcpy(&aucBuf[2], pucBuf, uiPageLen);

        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, ucDevAddr, aucBuf, uiPageLen + 2)) {
            LOG_WRITE(EN_LOG_ERROR, "I2c Write error. m_uiHwSubIdx = %d, ucDevAddr = %0x", m_uiHwSubIdx, ucDevAddr);
            return false;
        }

        uiAddr += uiPageLen;
        pucBuf += uiPageLen;
        uiLen -= uiPageLen;

        CDelay::Delay(50, MS);
        LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen);
    } while (uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write ---");

    return true;
}
#endif


bool
CSpiEEprom::Read(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBuf[2] = {0};
    uint32_t uiPageLen = 0;
    LOG_WRITE(EN_LOG_NOTICE, "CSpiEEprom::Read +++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpSpiPtr is nullptr.");
        return false;
    }

    do {

        uiPageLen = uiLen >= (DEF_16B_PAGE_LEN - (uiAddr % DEF_16B_PAGE_LEN))
                    ? (DEF_16B_PAGE_LEN - (uiAddr % DEF_16B_PAGE_LEN))
                    : uiLen;

        aucBuf[0] = (((uiAddr >> 8) & 0x01) << 3) + 0x03;
        aucBuf[1] = uiAddr & 0xFF;

        if (!m_stpDrvPtr->ReadWrite(m_uiChipIdx, aucBuf, 2, pucBuf, std::make_shared<CRspHandler>(uiLen))) {
            LOG_WRITE(EN_LOG_ERROR, "Read m_uiChipIdx = %d, uiAddrR = 0X%02x, uiLenR = %d error.", m_uiChipIdx, uiAddr, uiLen);
            return false;
        }

        uiAddr += uiPageLen;
        pucBuf += uiPageLen;
        uiLen -= uiPageLen;

        /* LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen); */

    } while (uiLen);

    return true;
}

bool
CSpiEEprom::Write(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBuf[DEF_EE_128KB_PAGE_LEN + 4] = {0};
    uint32_t uiPageLen = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write +++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpSpiPtr is nullptr.");
        return false;
    }

    do {
        uint8_t ucCmd = 0x06;
        m_stpDrvPtr->Write(2, &ucCmd, 1);

        CDelay::Delay(10, MS);

        uiPageLen = uiLen >= (DEF_16B_PAGE_LEN - (uiAddr % DEF_16B_PAGE_LEN))
                    ? (DEF_16B_PAGE_LEN - (uiAddr % DEF_16B_PAGE_LEN))
                    : uiLen;

        aucBuf[0] = (((uiAddr >> 8) & 0x01) << 3) + 0x02;
        aucBuf[1] = uiAddr & 0xFF;
        memcpy(&aucBuf[2], pucBuf, uiPageLen);

        /* printf("aucBuf[0]: 0X%0x ", aucBuf[0]); */
        /* printf("aucBuf[1]: 0X%0x\n", aucBuf[1]); */

        if (!m_stpDrvPtr->Write(m_uiChipIdx, aucBuf, uiPageLen + 2)) {
            LOG_WRITE(EN_LOG_ERROR, "Read m_uiChipIdx = %d, uiAddrR = 0X%02x, uiLenR = %d error.", m_uiChipIdx, uiAddr, uiLen);
            return false;
        }

        uiAddr += uiPageLen;
        pucBuf += uiPageLen;
        uiLen -= uiPageLen;

        CDelay::Delay(50, MS);
        /* LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen); */
    } while (uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "CEEprom::Write ---");

    return true;
}

