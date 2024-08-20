/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TdcmCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "TdcmCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CTdcm> > CTdcmCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CTdcmCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CTdcmCmd::ms_staSem;


/*==============================function=========================*/

bool
CTdcmCmd::SetFrequency(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdFloatData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdFloatData *pstData = (CCmdFloatData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetFrequency(pstData->fVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set tdcm frequency error. frequency = %.2f", pstData->fVal);
        return false;
    }

    return true;

}


bool
CTdcmCmd::SetDispersion(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdFloatData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdFloatData *pstData = (CCmdFloatData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetDispersion(pstData->fVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set tdcm frequency error. dispersion = %.2f", pstData->fVal);
        return false;
    }

    return true;

}


bool
CTdcmCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->Reset(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    CDelay::Delay(3, S);

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));
    if (itIter->second->GetMfg(stMfg)) {
        auto itShmIter = ms_staShm.find(pstData->uiIdx);
        if (ms_staShm.end() == itShmIter) {
            return false;
        }

        auto itSemIter = ms_staSem.find(pstData->uiIdx);
        if (ms_staSem.end() == itSemIter) {
            return false;
        }

        /* set data to shm */
        CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                          (uint8_t *)&stMfg, CShmOperator::GetDataTdcmMfgPos(), sizeof(stMfg));
        return true;
    } else {
        return false;
    }

    return true;
}




