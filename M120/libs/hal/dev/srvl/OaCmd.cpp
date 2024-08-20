/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaCmd.h"
#include "DevCmd.h"
#include "DevData.h"
#include "ElapseTimer.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COaDriver> > COaCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COaCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COaCmd::ms_staSem;

/*==============================function=========================*/

bool
COaCmd::GetMfg(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

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

    CMfg stMfg;

    if (!itIter->second->GetMfg(stMfg)) {
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

    /* set mfg data to shm */
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                      CShmOperator::GetDataOaMfgPos(), sizeof(stMfg));

    return true;

}


bool
COaCmd::GetCfg(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

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

    COaCfgData stData;

    if (!itIter->second->GetCfgData(stData, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataOaCfgPos(pstData->uiSubIdx), sizeof(stData));

    return true;

}

bool
COaCmd::GetDebugInfo(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdBufData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdBufData *pstData = (CCmdBufData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetDebugInfo((const char *)pstData->aucBuf)) {
        return false;
    }

    return true;

}

bool
COaCmd::GetMcuDebugInfo(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdBufData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdBufData *pstData = (CCmdBufData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetMcuDebugInfo((const char *)pstData->aucBuf, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}

bool
COaCmd::SetGainRange(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetGainRange +++");

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

    if (!itIter->second->SetGainRange(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetGainRange ---");

    return true;

}


bool
COaCmd::SetGain(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetGain +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "pstData->iVal = %d, pstData->uiSubIdx: %u", pstData->iVal, pstData->uiSubIdx);

    if (!itIter->second->SetGain(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetGain ---");

    return true;

}

bool
COaCmd::SetTilt(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetTilt +++");

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

    if (!itIter->second->SetTilt(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetTilt ---");

    return true;

}

bool
COaCmd::SetMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetMode +++");

    if ((uiLen != sizeof(CCmdKeyData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdKeyData *pstData = (CCmdKeyData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetMode((uint8_t)pstData->iKey, (int16_t)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetMode ---");

    return true;

}

bool
COaCmd::SetRxLosThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetRxLosThr +++");

    if ((uiLen != sizeof(CCmdThrData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrData *pstData = (CCmdThrData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetRxLosThr(pstData->sThr, pstData->sHys, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetRxLosThr ---");

    return true;

}

bool
COaCmd::SetPumpEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPumpEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetPumpEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetPumpEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPumpEnable ---");

    return true;

}

bool
COaCmd::SetAprEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAprEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);


    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiIdx = %d", pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetAprEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAprEnable ---");

    return true;

}

bool
COaCmd::SetAutolosEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAutolosEnable +++");

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

    if (!itIter->second->SetAutolosEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAutolosEnable ---");

    return true;

}



bool
COaCmd::SetAttn(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAttn +++");

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

    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiSubIdx = %d", pstData->uiSubIdx);
    if (!itIter->second->SetAttn(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAttn ---");

    return true;

}

bool
COaCmd::SetShutterState(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetShutterState +++");

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

    if (!itIter->second->SetShutterState((bool)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetShutterState ---");

    return true;

}

bool
COaCmd::GetShutterState(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

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

    bool bState = false;

    if (!itIter->second->GetShutterState(bState, pstData->uiSubIdx)) {
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

    /* set data to shm */
    uint8_t trans = (uint8_t)bState;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), &trans,
                      CShmOperator::GetDataOaShutterPos(pstData->uiSubIdx), sizeof(trans));

    return true;

}

bool
COaCmd::SetTarget(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetTarget +++");

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

    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiSubIdx = %d", pstData->uiSubIdx);
    if (!itIter->second->SetTarget(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetTarget ---");

    return true;
}


bool
COaCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "COaCmd::Reset %d doing", pstData->uiIdx);
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

    CElapseTimer::Start();
    if (!itIter->second->Reset(pstData->iVal, pstData->uiSubIdx)) {
        bRet = false;
        goto cleanup;
    }

    CDelay::Delay(1, S);

    /* update mfginfo
           NOTE: for edfa upgrade reset, max waiting time is 90s
     */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));
    for (int i = 0; i < 90; i++) {
        bRet = itIter->second->GetMfg(stMfg);
        if (bRet) {
            break;
        }
        CDelay::Delay(1, S);
    }

    CElapseTimer::Stop();

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
                          (uint8_t *)&stMfg, CShmOperator::GetDataOaMfgPos(), sizeof(stMfg));
        bRet = true;
    }

cleanup:
    stSta.iLoadSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    return bRet;
}


bool
COaCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
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


    LOG_WRITE(EN_LOG_NOTICE, "+++ Upgrade Status = %d. +++", stSta.iUpgdSta);

    if (SHM_STATUS_BUSY == stSta.iUpgdSta) {
        LOG_WRITE(EN_LOG_WARNING, "COaCmd::Upgrade %d doing", pstData->uiIdx);
        return false;
    }

    stSta.iUpgdSta = SHM_STATUS_BUSY;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    LOG_WRITE(EN_LOG_NOTICE, "+++ Set Upgrade Status Busy. +++");


    LOG_WRITE(EN_LOG_NOTICE, "+++ Upgrade Status = %d. +++", stSta.iUpgdSta);

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
COaCmd::Swap(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Swap +++");

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx = %d", pstData->uiIdx);
        return false;
    }

    if (nullptr == itIter->second) {
        LOG_WRITE(EN_LOG_ERROR, "drv ptr is nullptr.");
        return false;
    }

    CElapseTimer::Start();
    if (!itIter->second->Swap(pstData->uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Swap error.");
        return false;
    }
    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "Swap ---");

    return true;

}

bool
COaCmd::Commit(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Commit +++");

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        LOG_WRITE(EN_LOG_ERROR, "pstData->uiIdx = %d", pstData->uiIdx);
        return false;
    }

    if (nullptr == itIter->second) {
        LOG_WRITE(EN_LOG_ERROR, "drv ptr is nullptr.");
        return false;
    }

    CElapseTimer::Start();
    if (!itIter->second->Commit(pstData->uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Commit error.");
        return false;
    }
    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "Commit ---");

    return true;

}

bool
COaCmd::DbgRead(uint32_t uiLen, const uint8_t *c_pucBuf)
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
                      CShmOperator::GetDataOaDbgPos(0), pstData->uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "DbgRead --- %d %s", CShmOperator::GetDataOaDbgPos(0), pstData->aucBuf);
    return true;
}


bool
COaCmd::DbgWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
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
COaCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetCommFail ---");
    return true;
}

bool
COaCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearCommFail ---");
    return true;
}

bool
COaCmd::DbgSetModuleAlarm(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "DbgSetModuleAlarm +++");
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

    if (!itIter->second->DbgSetModuleAlarm(pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetModuleAlarm ---");
    return true;
}

bool
COaCmd::DbgClearModuleAlarm(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "DbgClearModuleAlarm +++");
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

    if (!itIter->second->DbgClearModuleAlarm(pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearModuleAlarm ---");
    return true;
}

bool
COaCmd::SetDfbEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "COaCmd::SetDfbEnable +++");

    if ((uiLen != sizeof(CCmdKeyData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdKeyData *pstData = (CCmdKeyData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_TRACE, "SetDfbEnable ++ %d, Power: %d", pstData->iKey, pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetDfbEnable((1 == pstData->iKey) ? true : false, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "COaCmd::SetDfbEnable ---");

    return true;

}

bool
COaCmd::SetOscEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetOscEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscEnable ---");

    return true;

}

bool
COaCmd::SetOtdrSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOtdrSwitchPos +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;
    LOG_WRITE(EN_LOG_NOTICE, "SetOtdrSwitchPos ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOtdrSwitchPos(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOtdrSwitchPos ---");

    return true;
}

bool
COaCmd::SetOscAddThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscAddThrHys +++");

    if ((uiLen != sizeof(CCmdThrData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrData *pstData = (CCmdThrData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscAddThrHys(pstData->sThr, pstData->sHys, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscAddThrHys ---");

    return true;

}

bool
COaCmd::SetOscDropThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscDropThrHys +++");

    if ((uiLen != sizeof(CCmdThrData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrData *pstData = (CCmdThrData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscDropThrHys(pstData->sThr, pstData->sHys, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscDropThrHys ---");

    return true;

}

bool
COaCmd::SetOscRxThrHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscRxThrHys +++");

    if ((uiLen != sizeof(CCmdThrData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrData *pstData = (CCmdThrData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscRxThrHys(pstData->sThr, pstData->sHys, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscRxThrHys ---");

    return true;

}

bool
COaCmd::SetApsdEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetApsdEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetApsdEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetApsdEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetApsdEnable ---");

    return true;

}

bool
COaCmd::SetUpgState(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetUpgState +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_DEBUG, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "pstData->iVal = %d, pstData->uiSubIdx: %u", pstData->iVal, pstData->uiSubIdx);

    if (!itIter->second->SetUpgState(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetUpgState ---");

    return true;

}

bool
COaCmd::SetLedState(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetLedState +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_DEBUG, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "pstData->iVal = %d, pstData->uiSubIdx: %u", pstData->iVal, pstData->uiSubIdx);

    if (!itIter->second->SetLedState(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetLedState ---");

    return true;

}

bool
COaCmd::SetLedAlm(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetLedAlm +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_DEBUG, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "pstData->iVal = %d, pstData->uiSubIdx: %u", pstData->iVal, pstData->uiSubIdx);

    if (!itIter->second->SetLedAlm(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetLedAlm ---");

    return true;

}


bool
COaCmd::GetCaliInfo(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

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

    COaCaliData stData;

    if (!itIter->second->GetCaliInfo(stData, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataOaCaliPos(pstData->uiSubIdx), sizeof(stData));

    return true;

}

bool
COaCmd::SetOtdrCali(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOtdrCali +++");

    if ((uiLen != sizeof(CCmdCaliData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdCaliData *pstData = (CCmdCaliData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_DEBUG, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "pstData->acFilePath = %s, pstData->uiIdx: %u", pstData->acFilePath, pstData->uiIdx);

    if (!itIter->second->SetOtdrCali(pstData->acFilePath, 0)) {
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOtdrCali ---");

    return true;

}

bool
COaCmd::SetOcmCali(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOcmCali +++");

    if ((uiLen != sizeof(CCmdCaliData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdCaliData *pstData = (CCmdCaliData *)cv_pucBuf;


    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiIdx = %d", pstData->uiIdx);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "pstData->acFilePath = %s, pstData->uiIdx: %u", pstData->acFilePath, pstData->uiIdx);

    if (!itIter->second->SetOcmCali(pstData->acFilePath, 0)) {
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOcmCali ---");

    return true;

}

bool
COaCmd::SetManualVoaEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetManualVoaEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetManualVoaEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetManualVoaEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetManualVoaEnable ---");

    return true;

}

bool
COaCmd::SetPaddingVoaConfigPower(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPaddingVoaConfigPower +++");

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

    LOG_WRITE(EN_LOG_NOTICE, "pstData->uiSubIdx = %d", pstData->uiSubIdx);
    if (!itIter->second->SetPaddingVoaConfigPower(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPaddingVoaConfigPower ---");

    return true;

}


bool
COaCmd::SetManualGainEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetManualGainEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetManualGainEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetManualGainEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetManualGainEnable ---");

    return true;

}

bool
COaCmd::SetThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetThr +++");

    if ((uiLen != sizeof(CCmdThrCommonData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrCommonData *pstData = (CCmdThrCommonData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetThr(pstData->uiPortIdx, pstData->uiType, pstData->sThr, pstData->sHys, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetThr ---");

    return true;

}

bool
COaCmd::SetPumpCurrent(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPumpCurrent +++");

    if ((uiLen != sizeof(CCmdKeyData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdKeyData *pstData = (CCmdKeyData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetPumpCurrent(pstData->iKey, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetPumpCurrent ---");

    return true;

}

bool
COaCmd::SetAlmMask(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAlmMask +++");

    if ((uiLen != sizeof(CCmdKeyData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdKeyData *pstData = (CCmdKeyData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetAlmMask((uint8_t)pstData->iKey, (int16_t)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAlmMask ---");

    return true;

}

bool
COaCmd::GetEventLog(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

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

    COaEventLogAllData stData;

    if (!itIter->second->GetEventLog(stData, pstData->uiSubIdx)) {
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
#if 0
    for (int i = 0; i < 4; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostTimeCount     = 0x%0x", i, stData.stOaEventLogData[i].uiLostTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostPD            = 0x%0x", i, stData.stOaEventLogData[i].uiLostPD);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLinkDownTimeCount = 0x%0x", i, stData.stOaEventLogData[i].uiLinkDownTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiApsdTimeCount     = 0x%0x", i, stData.stOaEventLogData[i].uiApsdTimeCount);
    }
#endif

    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataOaEventLogAllPos(pstData->uiSubIdx), sizeof(stData));

    return true;

}

bool
COaCmd::SetReadyTrigger(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetReadyTrigger +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;
    LOG_WRITE(EN_LOG_NOTICE, "SetReadyTrigger ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetReadyTrigger(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetReadyTrigger ---");

    return true;
}

bool
COaCmd::SetAutoGainMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAutoGainMode +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;
    LOG_WRITE(EN_LOG_NOTICE, "SetAutoGainMode ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetAutoGainMode(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetAutoGainMode ---");

    return true;
}

bool
COaCmd::SetRemoteInterlockEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetRemoteInterlockEnable +++");

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

    if (!itIter->second->SetRemoteInterlockEnable((bool)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetRemoteInterlockEnable ---");

    return true;

}

bool
COaCmd::SetDummyAdminKey(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetDummyAdminKey +++");

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

    if (!itIter->second->SetDummyAdminKey((bool)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetDummyAdminKey ---");

    return true;

}

bool
COaCmd::SetPanelSafeCmpt(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetPanelSafeCmpt +++");

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

    if (!itIter->second->SetPanelSafeCmpt((bool)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaCmd::SetPanelSafeCmpt ---");

    return true;

}

bool
COaCmd::SetOscAmpEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscAmpEnable +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }


    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetOscAmpEnable ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscAmpEnable((1 == pstData->iVal) ? true : false, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetOscAmpEnable ---");

    return true;

}

bool
COaCmd::SetReadyPower(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetReadyPower +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;
    LOG_WRITE(EN_LOG_NOTICE, "SetReadyPower ++ %d", pstData->iVal);

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetReadyPower(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaCmd::SetReadyPower ---");

    return true;
}

