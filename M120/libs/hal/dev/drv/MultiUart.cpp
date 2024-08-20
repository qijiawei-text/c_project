/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MultiUart.cpp
   Author:         Zhu Weian
   Date:		   2020-12-02
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-12-02] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "MultiUart.h"


bool
CMultiUart::ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                      uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos)
{

    /* m_staMutex.lock(); */
    if (!Lock()) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%d) Lock error.", uiIdx);
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "CMultiUart::ReadWrite +++");

    /* Fpga switch uart */
    LOG_WRITE(EN_LOG_DEBUG, "uiIdx = %d", uiIdx);
    m_stpUartSwitch->Switch(uiIdx);

    LOG_WRITE(EN_LOG_DEBUG, "m_uiUartSubIdx = %d", m_uiUartSubIdx);
    if (!m_stpUartDrv->ReadWrite(m_uiUartSubIdx, pucDataW, uiLenW, pucDataR, stpEndPos)) {
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CMultiUart::ReadWrite ---");
    /* m_staMutex.unlock(); */
    Unlock();

    return true;

ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "ReadWrite ---");
    /* m_staMutex.unlock(); */
    Unlock();


    return false;
}


bool
CMultiUart::Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
{

    /* m_staMutex.lock(); */
    if (!Lock()) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%d) Lock error.", uiIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write +++");

    /* Fpga switch uart */
    m_stpUartSwitch->Switch(uiIdx);

    if (!m_stpUartDrv->Write(m_uiIdx, pucData, uiLen)) {
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write ---");
    /* m_staMutex.unlock(); */
    Unlock();

    return true;

ERR_LABEL:

    /* m_staMutex.unlock(); */
    Unlock();

    return false;
}

bool
CMultiUart::Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos)
{

    LOG_WRITE(EN_LOG_DEBUG, "Read +++");

    /* m_staMutex.lock(); */
    if (!Lock()) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%d) Lock error.", uiIdx);
        return false;
    }

    /* Fpga switch uart */
    m_stpUartSwitch->Switch(uiIdx);

    if (!m_stpUartDrv->Read(m_uiIdx, pucData, stpEndPos)) {
        goto ERR_LABEL;
    }

    /* m_staMutex.unlock(); */
    Unlock();

    LOG_WRITE(EN_LOG_DEBUG, "Read ---");

    return true;

ERR_LABEL:

    /* m_staMutex.unlock(); */
    Unlock();

    return false;

}

#if 0
bool
CMultiUart::Lock()
{
    for (uint32_t uiTimes = 0; uiTimes < 800; uiTimes++) {
        if (m_staMutex.try_lock()) {
            return true;
        }
        CDelay::Delay(10, MS);
    }

    return false;
}
#else

bool
CMultiUart::Lock()
{
    m_staMutex.lock();
    return true;
}
#endif

void
CMultiUart::Unlock()
{
    m_staMutex.unlock();
}


bool
CMultiUart::Switch(uint32_t uiIdx)
{
    m_stpUartSwitch->Switch(uiIdx);
    return true;
}


bool
CMultiUart::ReadWriteUnlock(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                            uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos)
{
    LOG_WRITE(EN_LOG_DEBUG, "m_uiUartSubIdx = %d", m_uiUartSubIdx);
    if (!m_stpUartDrv->ReadWrite(m_uiUartSubIdx, pucDataW, uiLenW, pucDataR, stpEndPos)) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "ReadWrite ---");
    return false;
}

bool
CMultiUart::WriteUnlock(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
{

    if (!m_stpUartDrv->Write(m_uiIdx, pucData, uiLen)) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:

    return false;
}



