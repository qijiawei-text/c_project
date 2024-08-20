/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsCmd.cpp
   Author:        Guo Guangjun
   Date:          2023-01-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2023-01-05] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcsCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COcsDriver> > COcsCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COcsCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COcsCmd::ms_staSem;


/*==============================function=========================*/

bool
COcsCmd::SetOneSwitchChannel(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "SetOneSwitchChannel ++");

    if ((uiLen != sizeof(CCmdOcsConnectionsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOcsConnectionsData *pstData = (CCmdOcsConnectionsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOneSwitchChannel(pstData->stVal.astConn[0])) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "SetOneSwitchChannel --");

    return true;

}


bool
COcsCmd::SetSwitchChannels(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetSwitchChannels ++");

    if ((uiLen != sizeof(CCmdOcsConnectionsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOcsConnectionsData *pstData = (CCmdOcsConnectionsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetSwitchChannels(pstData->stVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetSwitchChannels --");

    return true;

}


bool
COcsCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "COcsCmd::Reset %d doing", pstData->uiIdx);
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
COcsCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "COcsCmd::Upgrade %d doing", pstData->uiIdx);
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
COcsCmd::Commit(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::Commit +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::Commit ---");
    return true;

}

bool
COcsCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::DbgSetCommFail ---");
    return true;
}

bool
COcsCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COcsCmd::DbgClearCommFail ---");
    return true;
}


bool
COcsCmd::SetLampTestState(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetLampTestState ++");

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

    if (!itIter->second->SetLampTestState((0 == pstData->iVal) ? false : true)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetLampTestState --");

    return true;

}

