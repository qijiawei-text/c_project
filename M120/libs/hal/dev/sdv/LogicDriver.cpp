/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "LogicDriver.h"
#include "DevData.h"
/*=============================variables=========================*/

std::mutex CLclBusLogic::ms_stMutex;
std::mutex CI2cLogic::ms_stMutex;
std::mutex CPcieBusLogic::ms_stMutex;
std::mutex CPcieBusLogicBitmap::ms_stMutex;
/*==============================function=========================*/
/* ---------------------------CLogicDriver-------------------------- */

bool
CLogicDriver::Write(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen, uint32_t uiType)
{

    bool bRet = true;

    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    GetMutex().lock();

    if (LOGIC_MEM_TYPE_RAM == uiType) {
        if (!WriteRam(uiAddr, pucData, uiLen)) {
            LOG_WRITE(EN_LOG_ERROR, "write ram error. uiAddr = 0x%0x, uiLen = %d", uiAddr, uiLen);
            bRet = false;
        }
    } else {
        if (!WriteFifo(uiAddr, pucData, uiLen)) {
            LOG_WRITE(EN_LOG_ERROR, "write fifo error. uiAddr = 0x%0x, uiLen = %d", uiAddr, uiLen);
            bRet = false;
        }
    }

    GetMutex().unlock();

    return bRet;

}

bool
CLogicDriver::Read(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen, uint32_t uiType)
{

    bool bRet = true;

    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    GetMutex().lock();

    if (LOGIC_MEM_TYPE_RAM == uiType) {
        if (!ReadRam(uiAddr, pucData, uiLen)) {
            LOG_WRITE(EN_LOG_ERROR, "read ram error. uiAddr = 0x%0x, uiLen = %d", uiAddr, uiLen);
            bRet = false;
        }
    } else {
        if (!ReadFifo(uiAddr, pucData, uiLen)) {
            LOG_WRITE(EN_LOG_ERROR, "read fifo error. uiAddr = 0x%0x, uiLen = %d", uiAddr, uiLen);
            bRet = false;
        }
    }

    GetMutex().unlock();

    return bRet;

}

bool
CLogicDriver::WriteBit(uint32_t uiReg, uint8_t ucBitOffset, uint8_t ucVal)
{
    uint8_t ucTmp = 0;

    GetMutex().lock();
    if ( false == ReadReg(uiReg, &ucTmp)) {
        GetMutex().unlock();
        LOG_WRITE(EN_LOG_ERROR, "read reg error. uiReg = 0X%02x, ucBitOffset = %d", uiReg, ucBitOffset);
        return false;
    }

    if ( 0 == ucVal) {
        ucTmp &= (~(0x01 << ucBitOffset));
    } else {
        ucTmp |= (0x01 << ucBitOffset);
    }

    if ( false == WriteReg(uiReg, ucTmp)) {
        GetMutex().unlock();
        LOG_WRITE(EN_LOG_ERROR, "write reg error. uiReg = 0X%02x, ucBitOffset = %d", uiReg, ucBitOffset);
        return false;
    }

    GetMutex().unlock();
    return true;
}


bool
CLogicDriver::ReadBit(uint32_t uiReg, uint8_t ucBitOffset, uint8_t &rucVal)
{
    uint8_t ucTmp = 0;

    LOG_WRITE(EN_LOG_DEBUG, "ReadBit +++");
    if ( false == ReadReg(uiReg, &ucTmp)) {
        LOG_WRITE(EN_LOG_ERROR, "read reg error. uiReg = 0X%02x, ucBitOffset = %d", uiReg, ucBitOffset);
        return false;
    }

    ucTmp = (ucTmp & (0x01 << ucBitOffset));
    rucVal = (ucTmp >> ucBitOffset);


    LOG_WRITE(EN_LOG_DEBUG, "ReadBit ---");

    return true;
}

/* ------------------------CLclBusLogic----------------------- */

bool
CLclBusLogic::WriteReg(uint32_t uiReg, uint8_t ucData)
{

    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "m_uiBaseAddr(%d) + uiReg(%d), ucData = %d", m_uiBaseAddr, uiReg, ucData); */
    m_stLclBusDrv.Write(m_uiBaseAddr + uiReg, ucData);

    return true;

}

bool
CLclBusLogic::ReadReg(uint32_t uiReg, uint8_t *pucData)
{
    if (nullptr == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "ReadReg +++ uiReg = 0X%02X", uiReg);

    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        LOG_WRITE(EN_LOG_ERROR, "m_stLclBusDrv.GetAddr() is MAP_FAILED.");
        return false;
    }

    *pucData = m_stLclBusDrv.Read(m_uiBaseAddr + uiReg);


    LOG_WRITE(EN_LOG_DEBUG, "ReadReg ---");

    return true;

}

bool
CLclBusLogic::WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }


    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        LOG_WRITE(EN_LOG_ERROR, "m_stLclBusDrv.GetAddr() is MAP_FAILED.");
        return false;
    }

    while (uiLen--) {
        m_stLclBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);
    }

    return true;

}

bool
CLclBusLogic::ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }


    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        LOG_WRITE(EN_LOG_ERROR, "m_stLclBusDrv.GetAddr() is MAP_FAILED.");
        return false;
    }

    while (uiLen--) {
        *pucData++ = m_stLclBusDrv.Read(m_uiBaseAddr + uiAddr);
    }

    return true;

}

bool
CLclBusLogic::WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }


    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        LOG_WRITE(EN_LOG_ERROR, "m_stLclBusDrv.GetAddr() is MAP_FAILED");
        return false;
    }

    while (uiLen--) {
        m_stLclBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);

        uiAddr += m_uiDataWidth;
    }

    return true;

}


bool
CLclBusLogic::ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }


    if (MAP_FAILED == m_stLclBusDrv.GetAddr()) {
        LOG_WRITE(EN_LOG_ERROR, "m_stLclBusDrv.GetAddr() is MAP_FAILED");
        return false;
    }

    while (uiLen--) {
        *pucData++ = m_stLclBusDrv.Read(m_uiBaseAddr + uiAddr);
        uiAddr += m_uiDataWidth;
    }

    return true;

}

/* ------------------------CI2cLogic----------------------- */

bool
CI2cLogic::WriteReg(uint32_t uiReg, uint8_t ucData)
{
    if (nullptr == m_stpI2cPtr) {
        return false;
    }

    UCHAR aucBuf[2];
    uint8_t ucReg = uiReg;

    aucBuf[0] = ucReg;
    aucBuf[1] = ucData;

    if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

    return true;

}

bool
CI2cLogic::ReadReg(uint32_t uiReg, uint8_t *pucData)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    if (nullptr == m_stpI2cPtr) {
        return false;
    }

    uint8_t ucReg = uiReg;

    m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &ucReg, 1, pucData, 1);

    return true;

}

bool
CI2cLogic::WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (nullptr == m_stpI2cPtr) {
        return false;
    }

    WriteReg(uiAddr, *pucData);

    return true;

}

bool
CI2cLogic::ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    ReadReg(uiAddr, pucData);

    return true;

}

/* ------------------------CPcieBusLogic----------------------- */

bool
CPcieBusLogic::WriteReg(uint32_t uiReg, uint8_t ucData)
{

    /* LOG_WRITE(EN_LOG_NOTICE, "uiReg(0x%0x), ucData = 0x%0x",  uiReg, ucData); */
    m_stPcieBusDrv.Write(m_uiBaseAddr + uiReg, ucData);

    return true;

}

bool
CPcieBusLogic::ReadReg(uint32_t uiReg, uint8_t *pucData)
{
    if (nullptr == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    *pucData = m_stPcieBusDrv.Read(m_uiBaseAddr + uiReg);
    /* LOG_WRITE(EN_LOG_NOTICE, "ReadReg +++ uiReg = 0X%02X,*pucData=0x%x", uiReg,*pucData); */

    LOG_WRITE(EN_LOG_DEBUG, "ReadReg ---");

    return true;

}

bool
CPcieBusLogic::WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    while (uiLen--) {
        m_stPcieBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);
    }

    return true;

}

bool
CPcieBusLogic::ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    while (uiLen--) {
        *pucData++ = m_stPcieBusDrv.Read(m_uiBaseAddr + uiAddr);
    }

    return true;

}

bool
CPcieBusLogic::WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }
#if 1
    while (uiLen--) {
        m_stPcieBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);

        uiAddr += m_uiDataWidth;
    }

    return true;
#else
    return m_stPcieBusDrv.Write(uiAddr, uiLen, pucData);
#endif
}

bool
CPcieBusLogic::ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
#define DATA_BRAM_ADDR                 (0x100000)
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }
    if (uiAddr < DATA_BRAM_ADDR) {
        while (uiLen--) {
            *pucData++ = m_stPcieBusDrv.Read(m_uiBaseAddr + uiAddr);
            uiAddr += m_uiDataWidth;
        }
        return true;
    } else {
        return m_stPcieBusDrv.Read(uiAddr, uiLen, pucData);
    }
}

/* ------------------------CPcieBusLogicBitmap----------------------- */

bool
CPcieBusLogicBitmap::WriteReg(uint32_t uiReg, uint8_t ucData)
{
    uint8_t ucDataW = 0;
    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif
    /* LOG_WRITE(EN_LOG_NOTICE, "uiReg(0x%0x), ucData = 0x%0x",  uiReg, ucData); */
    m_stPcieBusDrv.Write(m_uiBaseAddr + uiReg, ucData);
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);

    return true;

}

bool
CPcieBusLogicBitmap::ReadReg(uint32_t uiReg, uint8_t *pucData)
{
    uint8_t ucDataW = 0;
    if (nullptr == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif

    *pucData = m_stPcieBusDrv.Read(m_uiBaseAddr + uiReg);
    /* LOG_WRITE(EN_LOG_NOTICE, "ReadReg +++ uiReg = 0X%02X,*pucData=0x%x", uiReg,*pucData); */
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    LOG_WRITE(EN_LOG_DEBUG, "ReadReg ---");

    return true;

}

bool
CPcieBusLogicBitmap::WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif

    while (uiLen--) {
        m_stPcieBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);
    }
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    return true;

}

bool
CPcieBusLogicBitmap::ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;
    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif
    while (uiLen--) {
        *pucData++ = m_stPcieBusDrv.Read(m_uiBaseAddr + uiAddr);
    }
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    return true;

}

bool
CPcieBusLogicBitmap::WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;

    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif
#if 1
    while (uiLen--) {
        m_stPcieBusDrv.Write(m_uiBaseAddr + uiAddr, *pucData++);

        uiAddr += m_uiDataWidth;
    }
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    return true;
#else
    m_stPcieBusDrv.Write(uiAddr, uiLen, pucData);
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    return true;
#endif
}

bool
CPcieBusLogicBitmap::ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;

    if (NULL == pucData) {
        LOG_WRITE(EN_LOG_ERROR, "pucData is NULL.");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    if (m_uiHwIdx != 1 && m_uiHwIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", m_uiHwIdx);
        return false;
    }
    pthread_mutex_lock(&m_spi_mutex[m_uiHwIdx - 1]);
#if 0
    ucDataW = m_stPcieBusDrv.Read(m_uiBaseAddr + SLC_SPI_SELECT);
    ucDataW &= ~(1 << (m_uiHwIdx - 1));
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT, ucDataW);
#else
    m_stPcieBusDrv.Write(m_uiBaseAddr + SLC_SPI_SELECT + m_uiHwIdx - 1, ucDataW);
#endif
#if 1
    while (uiLen--) {
        *pucData++ = m_stPcieBusDrv.Read(m_uiBaseAddr + uiAddr);
        uiAddr += m_uiDataWidth;
    }
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
    return true;
#else
    m_stPcieBusDrv.Read(uiAddr, uiLen, pucData);
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwIdx - 1]);
#endif


}

