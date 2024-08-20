/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OtdrCmd.h"
#include "DevCmd.h"
#include "Event.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COtdrDriver> > COtdrCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COtdrCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COtdrCmd::ms_staSem;


/*==============================function=========================*/

bool
COtdrCmd::StartScan(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool rc = true;
    int i = 0;
    const int iRetry = 5;

    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "OTDR StartScan err1 arg");
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;
    COtdrStatusData stOtdrStatus = {0};

    auto itShmIter = ms_staShm.find(pstData->uiIdx);
    if (ms_staShm.end() == itShmIter) {
        LOG_WRITE(EN_LOG_ERROR, "OTDR StartScan err2 shm");
        return false;
    }

    auto itSemIter = ms_staSem.find(pstData->uiIdx);
    if (ms_staSem.end() == itSemIter) {
        LOG_WRITE(EN_LOG_ERROR, "OTDR StartScan  err3 sem");
        return false;
    }

    /* set measuring */
    stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_MEASURING;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stOtdrStatus,
                      CShmOperator::GetDataOtdrStaPos(), sizeof(stOtdrStatus));

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        LOG_WRITE(EN_LOG_ERROR, "OTDR StartScan  err4 not found");
        return false;
    }

    if (nullptr == itIter->second) {
        LOG_WRITE(EN_LOG_ERROR, "StartScan err5 nullptr == itIter->second");
        return false;
    }

    if (!itIter->second->StartScan()) {
        stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_ERR;
        LOG_WRITE(EN_LOG_ERROR, "StartScan err6 OTDR_SCAN_STATUS_ERR");
        rc = false;
        goto cleanup;
    }

    /* retry times */
    for (i = 0; i < iRetry; i++) {
        if (itIter->second->GetEvent(stOtdrStatus)) {
            LOG_WRITE(EN_LOG_NOTICE, "StartScan GetEvent, i:%d", i);
            break;
        }
    }

    if (i >= iRetry) {
        stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_ERR;
        LOG_WRITE(EN_LOG_ERROR, "StartScan err7 i >= iRetry, OTDR_SCAN_STATUS_ERR, i:%d", i);
        rc = false;
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Event cnt %d", stOtdrStatus.stEventTop.ulCnt);

    CDelay::Delay(3, S);

    /* retry times */
    for (i = 0; i < iRetry; i++) {
        if (itIter->second->GetSor()) {
            LOG_WRITE(EN_LOG_NOTICE, "GetSor i:%d", i);
            break;
        }
    }

    if (i >= iRetry) {
        stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_ERR;
        LOG_WRITE(EN_LOG_ERROR, "StartScan err %d >= iRetry, stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_ERR", i);
        rc = false;
        goto cleanup;
    }

    stOtdrStatus.ulScanStatus = OTDR_SCAN_STATUS_IDLE;
    rc = true;

cleanup:
    /* set idle */
    LOG_WRITE(EN_LOG_NOTICE, "HalCmd event top %u %u %u %lu %u %u",
              CShmOperator::GetDataOtdrStaPos(),
              stOtdrStatus.ulScanStatus,
              stOtdrStatus.stEventTop.ulCnt,
              sizeof(stOtdrStatus),
              pstData->uiIdx,
              pstData->uiSubIdx);

    for (int i = 0; i < (int)stOtdrStatus.stEventTop.ulCnt; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "HalCmd event line %d %f %f %f %f %d",
                  i,
                  stOtdrStatus.astEventLine[i].dEventLoss,
                  stOtdrStatus.astEventLine[i].dLocation,
                  stOtdrStatus.astEventLine[i].dReflectance,
                  stOtdrStatus.astEventLine[i].dTotalLoss,
                  stOtdrStatus.astEventLine[i].enEventType);
    }

    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stOtdrStatus,
                      CShmOperator::GetDataOtdrStaPos(), sizeof(stOtdrStatus));


    /* send event to csm */
    if (OPLK_OK != CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, (pstData->uiSubIdx == DEV_TYPE_OTDR) ? (DEF_EVENT_ID_OTDR) : (DEF_EVENT_ID_OTDR_P), 0)) {
        /* TBD */
        LOG_WRITE(EN_LOG_ERROR, "StartScan err9 i >= iRetry, SendEvent");
    }

    return rc;
}

bool
COtdrCmd::SetPara(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrPara)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrPara *pstData = (CCmdOtdrPara *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetPara(pstData->uiDistRange, pstData->uiPulseWidth, pstData->dSampling)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::SetMeasuringTime(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrPara)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrPara *pstData = (CCmdOtdrPara *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetMeasuringTime(pstData->uiMTime)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::SetUserIOR(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrPara)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrPara *pstData = (CCmdOtdrPara *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetUserIOR(pstData->dUIOR)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::SetThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrPara)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrPara *pstData = (CCmdOtdrPara *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetThr(pstData->dRef, pstData->dTLos, pstData->dTEof)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::SetScanMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrPara)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrPara *pstData = (CCmdOtdrPara *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetScanMode(pstData->uiScanMode)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::GetTemp(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    COtdrTemp stTemp = {0};

    if (!itIter->second->GetTemp(stTemp)) {
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

    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stTemp,
                      CShmOperator::GetDataOtdrTempPos(), sizeof(stTemp));

    return true;
}



bool
COtdrCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::Reset +++");

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
        LOG_WRITE(EN_LOG_WARNING, "COtdrCmd::Reset %d doing", pstData->uiIdx);
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

    CDelay::Delay(6, S);

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));
    for (int i = 0; i < 40; i++) {
        bRet = itIter->second->GetMfg(stMfg);
        if (bRet) {
            /* CEvent::SendEvent((char *)DEF_OPERATION_EVENT_SOCKET_PATH, (DEF_EVENT_ID_OTDR), pstData->iVal); */
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
                          (uint8_t *)&stMfg, CShmOperator::GetDataOtdrMfgPos(), sizeof(stMfg));
        bRet = true;
        /* CEvent::SendEvent((char *)DEF_OPERATION_EVENT_SOCKET_PATH, (2 + DEF_EVENT_ID_SLOT0), 0); */
    }

cleanup:
    stSta.iLoadSta = SHM_STATUS_FREE;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::Reset ---");

    return bRet;
}



bool
COtdrCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
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
        LOG_WRITE(EN_LOG_WARNING, "COtdrCmd::Upgrade %d doing", pstData->uiIdx);
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
COtdrCmd::DbgRead(uint32_t uiLen, const uint8_t *c_pucBuf)
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
                      CShmOperator::GetDataOtdrDbgPos(), pstData->uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "DbgRead --- %d %s", CShmOperator::GetDataOtdrDbgPos(), pstData->aucBuf);
    return true;
}


bool
COtdrCmd::DbgWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
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
COtdrCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::DbgSetCommFail ---");
    return true;
}

bool
COtdrCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::DbgClearCommFail ---");
    return true;
}

bool
COtdrCmd::SetStartOffset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOtdrStartOffsetData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOtdrStartOffsetData *pstData = (CCmdOtdrStartOffsetData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetStartOffset(&pstData->stVal)) {
        return false;
    }

    return true;
}

bool
COtdrCmd::GetStartOffset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::GetStartOffset +++");
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

    COtdrStartOffset stStartOffset = {0};

    if (!itIter->second->GetStartOffset(stStartOffset)) {
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

    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stStartOffset,
                      CShmOperator::GetDataOtdrStartOffsetPos(), sizeof(stStartOffset));
    LOG_WRITE(EN_LOG_NOTICE, "COtdrCmd::GetStartOffset ---");

    return true;
}



