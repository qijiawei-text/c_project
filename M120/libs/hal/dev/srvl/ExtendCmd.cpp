/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "ExtendCmd.h"
#include "DevCmd.h"
#include "ElapseTimer.h"
/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CExtendDriver> > CExtendCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CExtendCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CExtendCmd::ms_staSem;

/*==============================function=========================*/


bool
CExtendCmd::SetSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetSwitchPos(pstData->uiSubIdx, pstData->iVal)) {
        return false;
    }

    return true;
}



bool
CExtendCmd::SetSfp(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetSfp(pstData->uiSubIdx, pstData->iVal)) {
        return false;
    }

    return true;
}


bool
CExtendCmd::SetVoaAttn(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetVoaAttn(pstData->uiSubIdx, pstData->iVal)) {
        return false;
    }

    return true;
}

bool
CExtendCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::Reset +++");
    CElapseTimer::Start();

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* set data to shm */
    auto itShmIter = ms_staShm.find(pstData->uiIdx);
    if (ms_staShm.end() == itShmIter) {
        return false;
    }

    auto itSemIter = ms_staSem.find(pstData->uiIdx);
    if (ms_staSem.end() == itSemIter) {
        return false;
    }

    CShmDevSta stSta;
    CShmOperator::Get(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    if (SHM_STATUS_BUSY == stSta.iLoadSta) {
        LOG_WRITE(EN_LOG_WARNING, "CExtendCmd::Reset %d doing", pstData->uiIdx);
        return false;
    }

    stSta.iLoadSta = SHM_STATUS_BUSY;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        bRet = false;
        goto cleanup;
    }

    if (nullptr == itIter->second) {
        bRet = false;
        goto cleanup;
    }

    if (!itIter->second->Reset(pstData->iVal)) {
        bRet = false;
        goto cleanup;
    }

    /* CDelay::Delay(3, S); */

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));
    for (int i = 0; i < 3; i++) {
        bRet = itIter->second->GetMfg(stMfg);
        if (bRet) {
            break;
        }
        CDelay::Delay(1, S);
    }

    if (bRet) {
        auto itShmIter = ms_staShm.find(pstData->uiIdx);
        if (ms_staShm.end() == itShmIter) {
            bRet = false;
            goto cleanup;
        }

        auto itSemIter = ms_staSem.find(pstData->uiIdx);
        if (ms_staSem.end() == itSemIter) {
            bRet = false;
            goto cleanup;
        }

        /* set data to shm */
        CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                          (uint8_t *)&stMfg, CShmOperator::GetDataExtendMfgPos(), sizeof(stMfg));
        bRet = true;
    }

cleanup:
    stSta.iLoadSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));


    CElapseTimer::Stop();
    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::Reset ---");

    return bRet;
}



bool
CExtendCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::Upgrade +++");

    CElapseTimer::Start();

    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)cv_pucBuf;

    /* set data to shm */
    auto itShmIter = ms_staShm.find(pstData->uiIdx);
    if (ms_staShm.end() == itShmIter) {
        return false;
    }

    auto itSemIter = ms_staSem.find(pstData->uiIdx);
    if (ms_staSem.end() == itSemIter) {
        return false;
    }

    CShmDevSta stSta;
    CShmOperator::Get(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    if (SHM_STATUS_BUSY == stSta.iUpgdSta) {
        LOG_WRITE(EN_LOG_WARNING, "CExtendCmd::Upgrade %d doing", pstData->uiIdx);
        return false;
    }

    stSta.iUpgdSta = SHM_STATUS_BUSY;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        bRet = false;
        goto cleanup;
    }

    if (nullptr == itIter->second) {
        bRet = false;
        goto cleanup;
    }

    if (!itIter->second->Upgrade(pstData->acFilePath)) {
        bRet = false;
        goto cleanup;
    }

    bRet = true;
cleanup:

    stSta.iUpgdSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    CElapseTimer::Stop();
    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::Upgrade ---");
    return bRet;
}


bool
CExtendCmd::DbgRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "DbgRead +++");
    if ((uiLen != sizeof(CCmdDbgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdDbgData *pstData = (CCmdDbgData *)c_pucBuf;

    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->DbgRead(pstData->uiIdx, pstData->aucBuf, pstData->uiLen)) {
        return false;
    }

    /* set data to shm */
    auto itShmIter = ms_staShm.find(pstData->uiIdx);
    if (ms_staShm.end() == itShmIter) {
        return false;
    }

    auto itSemIter = ms_staSem.find(pstData->uiIdx);
    if (ms_staSem.end() == itSemIter) {
        return false;
    }

    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucBuf,
                      CShmOperator::GetDataExtendDbgPos(), pstData->uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "DbgRead --- %d %s", CShmOperator::GetDataExtendDbgPos(), pstData->aucBuf);
    return true;
}


bool
CExtendCmd::DbgWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite +++");
    if ((uiLen != sizeof(CCmdDbgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdDbgData *pstData = (CCmdDbgData *)c_pucBuf;

    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->DbgWrite(pstData->uiIdx, pstData->aucBuf, pstData->uiLen)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite ---");
    return true;
}

bool
CExtendCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::DbgSetCommFail +++");
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)c_pucBuf;

    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->DbgSetCommFail(pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::DbgSetCommFail ---");
    return true;
}

bool
CExtendCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::DbgClearCommFail +++");
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)c_pucBuf;

    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->DbgClearCommFail(pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendCmd::DbgClearCommFail ---");
    return true;
}

