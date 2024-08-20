/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      CPcaI2c.cpp
   Author:         Zhu Weian
   Date:		   2020-05-18
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-05-18] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SwitchableI2c.h"

/*=============================variables=========================*/


/*==============================function=========================*/

#if 0
bool
CSwitchableI2c::Read(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    LOG_WRITE(EN_LOG_DEBUG, "CSwitchableI2c::Read +++");

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    /* LOG_WRITE(EN_LOG_NOTICE, "uiChIdx = %d", uiChIdx); */
    if (false == __Switch(uiChIdx)) {
        /* LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", uiChIdx); */
        goto ERR_LABLE;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "Read m_uiI2cSubIdx = %d, uiAddr = 0X%02x", m_uiI2cSubIdx, uiAddr); */
    if (false == m_stpI2cPtr->Read(m_uiI2cSubIdx, uiAddr, pucBufW, uiLenW, pucBufR, uiLenR)) {
        LOG_WRITE(EN_LOG_DEBUG, "read error. m_uiHwSubIdx=%d, uiAddr = 0X%02x", m_uiHwSubIdx, uiAddr);
        goto ERR_LABLE;
    }

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", uiChIdx);
        goto ERR_LABLE;
    }

    m_staMutex.unlock();

    /* LOG_WRITE(EN_LOG_DEBUG, "CPcaI2c::Read ---."); */

    return true;

ERR_LABLE:
    m_staMutex.unlock();
    return false;

}

bool
CSwitchableI2c::Write(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    LOG_WRITE(EN_LOG_DEBUG, "CSwitchableI2c::Write +++");

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    LOG_WRITE(EN_LOG_DEBUG, "Switch uiChIdx = %d", uiChIdx);
    if (false == __Switch(uiChIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", uiChIdx);
        goto ERR_LABLE;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write m_uiI2cSubIdx = %d uiAddr = 0X%02x", m_uiI2cSubIdx, uiAddr);
    if (false == m_stpI2cPtr->Write(m_uiI2cSubIdx, uiAddr, pucBufW, uiLenW)) {
        /* LOG_WRITE(EN_LOG_ERROR, "write error."); */
        goto ERR_LABLE;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write uiAddr = 0X%02x", uiAddr);

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", uiChIdx);
        goto ERR_LABLE;
    }

    m_staMutex.unlock();

    LOG_WRITE(EN_LOG_DEBUG, "CSwitchableI2c::Write ---.");

    return true;

ERR_LABLE:
    m_staMutex.unlock();
    return false;
}
#endif

bool
CSwitchableI2c::Read(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    int32_t iRet = true;

    /* LOG_WRITE(EN_LOG_NOTICE, "CSwitchableI2c::Read +++"); */

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    /* LOG_WRITE(EN_LOG_NOTICE, "uiChIdx = %d", uiChIdx); */
    if (false == __Switch(uiChIdx)) {
        /* LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", uiChIdx); */
        goto ERR_LABLE;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "Read m_uiI2cSubIdx = %d, uiAddr = 0X%02x", m_uiI2cSubIdx, uiAddr); */
    if (false == m_stpI2cPtr->Read(m_uiI2cSubIdx, uiAddr, pucBufW, uiLenW, pucBufR, uiLenR)) {
        LOG_WRITE(EN_LOG_DEBUG, "read error. m_uiHwSubIdx=%d, uiAddr = 0X%02x", m_uiHwSubIdx, uiAddr);
        iRet &= false;
    }

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", uiChIdx);
        iRet &= false;
    }

    /*m_staMutex.unlock(); */

    /*LOG_WRITE(EN_LOG_DEBUG, "CSwitchableI2c::Read ---."); */

    /*return true; */

ERR_LABLE:
    m_staMutex.unlock();
    return iRet;

}

bool
CSwitchableI2c::Write(uint32_t uiChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{

    /* LOG_WRITE(EN_LOG_NOTICE, "CSwitchableI2c::Write +++"); */

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    /* LOG_WRITE(EN_LOG_NOTICE, "Switch uiChIdx = %d", uiChIdx); */
    if (false == __Switch(uiChIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", uiChIdx);
        goto ERR_LABLE;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write m_ui2cSubIdx = %d uiAddr = 0X%02x", m_uiI2cSubIdx, uiAddr);
    if (false == m_stpI2cPtr->Write(m_uiI2cSubIdx, uiAddr, pucBufW, uiLenW)) {
        /* LOG_WRITE(EN_LOG_ERROR, "write error."); */
        goto ERR_LABLE;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write uiAddr = 0X%02x", uiAddr);

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", uiChIdx);
        goto ERR_LABLE;
    }

    m_staMutex.unlock();

    /* LOG_WRITE(EN_LOG_NOTICE, "CSwitchableI2c::Write ---."); */

    return true;

ERR_LABLE:
    m_staMutex.unlock();
    return false;
}

#if 0

bool
CPcaI2c::Read(uint8_t ucChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    /* LOG_WRITE(EN_LOG_DEBUG, "CPcaI2c::Read +++"); */

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    if (false == __Switch(ucChIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", ucChIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Read uiAddr = 0X%02x", uiAddr);
    if (false == m_stpI2cPtr->Read(uiAddr, pucBufW, uiLenW, pucBufR, uiLenR)) {
        LOG_WRITE(EN_LOG_ERROR, "read error.");
        return false;
    }

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", ucChIdx);
        return false;
    }

    m_staMutex.unlock();

    /* LOG_WRITE(EN_LOG_DEBUG, "CPcaI2c::Read ---."); */

    return true;
}

bool
CPcaI2c::Write(uint8_t ucChIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{
    LOG_WRITE(EN_LOG_DEBUG, "CPcaI2c::Write +++");

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    m_staMutex.lock();

    if (false == __Switch(ucChIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch ucChIdx = %d error.", ucChIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "uiAddr = 0X%02x", uiAddr);
    if (false == m_stpI2cPtr->Write(uiAddr, pucBufW, uiLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "write error.");
        return false;
    }

    if (false == __SwitchClose()) {
        LOG_WRITE(EN_LOG_ERROR, "Switch close ucChIdx = %d error.", ucChIdx);
        return false;
    }

    m_staMutex.unlock();

    LOG_WRITE(EN_LOG_DEBUG, "CPcaI2c::Write ---.");

    return true;
}

#endif

bool
CPcaI2c::__Switch(uint32_t uiChIdx)
{
    uint8_t ucChIdx = uiChIdx;

    /* LOG_WRITE(EN_LOG_NOTICE, "CPcaI2c::__Switch +++"); */


    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "m_uiI2cSubIdx = 0X%02X, ucDevAddr = 0X%02X, ucChIdx = %x", m_uiI2cSubIdx, m_uiDevAddr, ucChIdx);
    if (false == m_stpI2cPtr->Write(m_uiI2cSubIdx, m_uiDevAddr, &ucChIdx, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "PCA switch error. m_uiI2cSubIdx = 0X%02x m_uiDevAddr = 0X%02x, ucChIdx = 0X%02X",
                  m_uiI2cSubIdx, m_uiDevAddr, ucChIdx);
        return false;
    }


    /* LOG_WRITE(EN_LOG_NOTICE, "CPcaI2c::__Switch ---"); */

    return true;
}

bool
CPcaI2c::__SwitchClose()
{
    uint8_t ucChIdx = 0;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr.");
        return false;
    }

    if (false == m_stpI2cPtr->Write(m_uiI2cSubIdx, m_uiDevAddr, &ucChIdx, 1)) {
        LOG_WRITE(EN_LOG_ERROR, "PCA switch error.");
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "write m_uiAddr = 0X%02x ucChIdx = 0X%02x", m_uiDevAddr, ucChIdx);

    return true;
}

const std::vector<uint32_t> CLogicSwitchI2c::msc_staValue = {0x00, 0x10};

bool
CLogicSwitchI2c::__Switch(uint32_t uiChIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__Switch +++");

    if (nullptr == m_stpI2cSwitch) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__Switch +++ uiChIdx = %d", uiChIdx);
    /* m_stpI2cSwitch->Switch(uiChIdx); */
    m_stpI2cSwitch->Switch(m_uiLogicSubIdx, uiChIdx);

    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__Switch ---");

    return true;
}

bool
CLogicSwitchI2c::__SwitchClose()
{
    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__SwitchClose +++");

    if (nullptr == m_stpI2cSwitch) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicPtr is nullptr.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__SwitchClose +++ m_uiLogicSubIdx = %d", m_uiLogicSubIdx);
    /* m_stpI2cSwitch->Switch(uiChIdx); */
    m_stpI2cSwitch->SwitchClose(m_uiLogicSubIdx);

    LOG_WRITE(EN_LOG_NOTICE, "CLogicSwitchI2c::__SwitchClose ---");

    return true;
}

