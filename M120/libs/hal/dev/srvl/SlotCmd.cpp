/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotCmd.cpp
   Author:        Zhu Weian
   Date:          2019-08-26
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-08-26] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SlotCmd.h"
#include "DevCmd.h"
#include "DevData.h"

/*=============================variables=========================*/

std::vector<std::shared_ptr<CSlotDriver> > CSlotCmd::ms_staDrvPtrs;
std::array<CShareMemory, SHM_TYPE_MAX> CSlotCmd::ms_staShm;
std::array<CSemaphore, SHM_TYPE_MAX> CSlotCmd::ms_staSem;

/*==============================function=========================*/

bool
CSlotCmd::SetOnline(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::SetOnline +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->SetOnline(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::SetOnline ---");
    return true;

}


bool
CSlotCmd::SetMfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::SetMfg +++");
    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)c_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d  acFilePath = %s", pstData->uiIdx, pstData->acFilePath);

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->SetMfg(pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::SetMfg ---");
    return true;

}

bool
CSlotCmd::SetCali(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CSlotCmd::SetChassisCali +++");

    if ((uiLen != sizeof(CCmdCaliData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCaliData *pstData = (CCmdCaliData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (!ms_staDrvPtrs.at(pstData->uiIdx)->SetCali(pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set slot cali error. FileName = %s", pstData->acFilePath);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CSlotCmd::SetChassisCali ---");

    return true;
}

bool
CSlotCmd::ClearPlugFlag(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::ClearPlugFlag +++");
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->ClearPlug()) {
        LOG_WRITE(EN_LOG_ERROR, "Clear plug flag error. uiIdx = %d", pstData->uiIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::ClearPlugFlag ---");
    return true;

}


bool
CSlotCmd::SetLedState(uint32_t uiLen, const uint8_t *c_pucBuf)
{

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!ms_staDrvPtrs.at(pstData->uiIdx)->SetLedState(pstData->uiSubIdx, pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Set led state error. uiSubIdx = %d, iVal = %d", pstData->uiSubIdx, pstData->iVal);
        return false;
    }

    return true;

}

bool
CSlotCmd::EepromWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::EepromWrite +++");
    if ((uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->EepromWrite(pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write Eeprom error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::EepromWrite ---");
    return true;

}


bool
CSlotCmd::EepromRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::EepromRead +++");
    if ((uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {

        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->EepromRead(pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read Eeprom error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);
        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucVal,
                      CShmOperator::GetDataSlotDbgStaPos(pstData->uiIdx), pstData->ulLen);

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::EepromRead ---");
    return true;

}

bool
CSlotCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::Reset +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    if (pstData->uiIdx >= ms_staDrvPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx >= ms_staDrvPtrs.size()");
        return false;
    }

    if (nullptr == ms_staDrvPtrs.at(pstData->uiIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == ms_staDrvPtrs.at(pstData->uiIdx)");
        return false;
    }

    if (false == ms_staDrvPtrs.at(pstData->uiIdx)->Reset(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotCmd::Reset ---");
    return true;

}


