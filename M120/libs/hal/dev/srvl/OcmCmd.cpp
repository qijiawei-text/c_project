/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcmCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COcmDriver> > COcmCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COcmCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COcmCmd::ms_staSem;


/*==============================function=========================*/

bool
COcmCmd::SetWavePlan(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan ++");

    if ((uiLen != sizeof(CCmdWavePlanSliceData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdWavePlanSliceData *pstData = (CCmdWavePlanSliceData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetWavePlan(pstData->uiPortId, pstData->uiChCnt, pstData->astVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan --");

    return true;

}

bool
COcmCmd::SetOcmWavePlan(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetOcmWavePlan ++");

    if ((uiLen != sizeof(CCmdOcmWavePlanData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOcmWavePlanData *pstData = (CCmdOcmWavePlanData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOcmWavePlan(pstData->uiPortId, pstData->uiChCnt, pstData->astVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetOcmWavePlan --");

    return true;

}


bool
COcmCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

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
        LOG_WRITE(EN_LOG_WARNING, "COcmCmd::Reset %d doing", pstData->uiIdx);
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

    if (!itIter->second->Reset(pstData->iVal, pstData->uiSubIdx)) {
        bRet = false;
        goto cleanup;
    }

    CDelay::Delay(3, S);

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));
    for (int i = 0; i < 3; i++) {
        bRet = itIter->second->GetMfg(stMfg);
        if (bRet) {
            break;
        }
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
                          (uint8_t *)&stMfg, CShmOperator::GetDataOcmMfgPos(), sizeof(stMfg));
        bRet = true;
    }

cleanup:
    stSta.iLoadSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    return bRet;
}



bool
COcmCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

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
        LOG_WRITE(EN_LOG_WARNING, "COcmCmd::Upgrade %d doing", pstData->uiIdx);
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

    if (!itIter->second->Upgrade(pstData->uiIdx, pstData->acFilePath)) {
        bRet = false;
        goto cleanup;
    }

    bRet = true;
cleanup:

    stSta.iUpgdSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));
    return bRet;
}

bool
COcmCmd::Commit(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::Commit +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->Commit(pstData->uiIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::Commit ---");
    return true;

}

bool
COcmCmd::GetRealTimeOsa(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "GetRealTimeOsa +++");

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

    LOG_WRITE(EN_LOG_DEBUG, "PortId = %d", pstData->iVal);
    COcmOsa stData;
    memset(&stData, 0, sizeof(stData));

    if (!itIter->second->GetRealTimeOsa(pstData->iVal, stData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetRealTimeOsa error. PortId = %d", pstData->iVal);
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

    /* set report data to shm */
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataOcmOsaPos(pstData->iVal), sizeof(stData));

    LOG_WRITE(EN_LOG_DEBUG, "GetRealTimeOsa ---");

    return true;

}

bool
COcmCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::DbgSetCommFail ---");
    return true;
}

bool
COcmCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COcmCmd::DbgClearCommFail ---");
    return true;
}
