/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "WssCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CWssDriver> > CWssCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CWssCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CWssCmd::ms_staSem;


/*==============================function=========================*/
bool
CWssCmd::GetCfgData(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "GetCfgData ++");

    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    CWssAllCfg stCfg;
    memset(&stCfg, 0, sizeof(stCfg));

    if (!itIter->second->GetCfgData(stCfg)) {
        return false;
    }


    auto itShmIter = ms_staShm.find(pstData->uiIdx);
    if (ms_staShm.end() == itShmIter) {
        return false;
    }

    auto itSemIter = ms_staSem.find(pstData->uiIdx);
    if (ms_staSem.end() == itSemIter) {
        return false;
    }

    /* set cfg data to shm */
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stCfg,
                      CShmOperator::GetDataWssCfgPos(), sizeof(stCfg));

    LOG_WRITE(EN_LOG_TRACE, "GetCfgData --");

    return true;

}


bool
CWssCmd::SetWavePlan(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan ++");

    if ((uiLen != sizeof(CCmdWavePlanData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdWavePlanData *pstData = (CCmdWavePlanData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetWavePlan(pstData->uiChCnt, pstData->astVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan --");

    return true;

}



bool
CWssCmd::SetCfg(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetAllChCfg ++");

    if ((uiLen != sizeof(CCmdWssCfgData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdWssCfgData *pstData = (CCmdWssCfgData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetCfg(pstData->uiInstBegin, pstData->uiInstEnd, pstData->astVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetAllChCfg --");

    return true;

}



bool
CWssCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "CWssCmd::Reset %d doing", pstData->uiIdx);
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

    if ( RST_MODE_COLD == pstData->iVal) {
        CDelay::Delay(35, S);
    } else {
        CDelay::Delay(20, S);
    }

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
CWssCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "CWssCmd::Upgrade %d doing", pstData->uiIdx);
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
    return bRet;
}


bool
CWssCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CWssCmd::DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "CWssCmd::DbgSetCommFail ---");
    return true;
}

bool
CWssCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CWssCmd::DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "CWssCmd::DbgClearCommFail ---");
    return true;
}



