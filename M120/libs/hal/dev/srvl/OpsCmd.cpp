/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OpsCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COpsDriver> > COpsCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COpsCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COpsCmd::ms_staSem;

/*==============================function=========================*/


bool
COpsCmd::SetMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetMode(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}

bool
COpsCmd::SetWtr(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetWtr(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}
bool
COpsCmd::SetHoldOffTime(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetHoldOffTime(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}

bool
COpsCmd::SetRevertive(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetRevertive(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}

bool
COpsCmd::SetRelativeThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetRelativeThr(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}

bool
COpsCmd::SetRelativeOffset(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetRelativeOffset(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;

}


bool
COpsCmd::SetPortPowerThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetPortPowerThr(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::SetPortPowerHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetPortPowerHys(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::SetSwitchThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOpsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOpsData *pstData = (CCmdOpsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetSwitchThr(pstData->uiPort, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::GetSwitchThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    COpsSwitchThr stThr = {0};
    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetSwitchThr(stThr, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                      (uint8_t *)&stThr, CShmOperator::GetDataOpsSwitchThrPos(), sizeof(COpsSwitchThr));


    return true;
}



bool
COpsCmd::SetSwitchHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOpsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOpsData *pstData = (CCmdOpsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetSwitchHys(pstData->uiPort, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::GetSwitchHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    COpsSwitchHys stHys = {0};
    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetSwitchHys(stHys, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                      (uint8_t *)&stHys, CShmOperator::GetDataOpsSwitchHysPos(), sizeof(COpsSwitchHys));


    return true;
}


bool
COpsCmd::Reset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;

    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::Reset +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (!__SetShmStatus(pstData->uiIdx, SHM_STATUS_BUSY)) {
        return false;
    }

    if (ms_staDrvPtrs.end() == itIter) {
        goto cleanup;
    }

    if (nullptr == itIter->second) {
        goto cleanup;
    }

    if (!itIter->second->Reset(pstData->iVal, pstData->uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Reset error. Reset Type = %d", pstData->iVal);
        goto cleanup;
    }

    CDelay::Delay(3, S);

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    for (int i = 0; i < 40; i++) {
        if (itIter->second->GetMfg(stMfg)) {
            auto itShmIter = ms_staShm.find(pstData->uiIdx);
            if (ms_staShm.end() == itShmIter) {
                goto cleanup;
            }

            auto itSemIter = ms_staSem.find(pstData->uiIdx);
            if (ms_staSem.end() == itSemIter) {
                goto cleanup;
            }

            /* set data to shm */
            CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                              (uint8_t *)&stMfg, CShmOperator::GetDataOpsMfgPos(), sizeof(stMfg));
            bRet = true;
            goto cleanup;
        }

        CDelay::Delay(1, S);
    }

cleanup:

    __SetShmStatus(pstData->uiIdx, SHM_STATUS_FREE);

    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::Reset ---");
    return bRet;

}

bool
COpsCmd::Upgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = false;
    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)cv_pucBuf;

    if (!__SetShmStatus(pstData->uiIdx, SHM_STATUS_BUSY)) {
        return false;
    }

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);
    if (ms_staDrvPtrs.end() == itIter) {
        goto cleanup;
    }

    if (nullptr == itIter->second) {
        goto cleanup;
    }

    if (!itIter->second->Upgrade(pstData->uiIdx, pstData->acFilePath)) {
        goto cleanup;
    }

    bRet = true;

cleanup:

    __SetShmStatus(pstData->uiIdx, SHM_STATUS_FREE);
    return bRet;
}


bool
COpsCmd::GetPortPowerHistory(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;
    COpsHistoryData stHistoryData;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    memset(&stHistoryData, 0, sizeof(stHistoryData));
    if (!itIter->second->GetPortPowerHistory(stHistoryData, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stHistoryData,
                      CShmOperator::GetDataOpsHistoryPos(), sizeof(stHistoryData));


    return true;
}


bool
COpsCmd::SetWssPortPowerThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetWssPortPowerThr(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}
bool
COpsCmd::SetWssPortPowerHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetWssPortPowerHys(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::GetWssPortPowerHirtory(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->GetWssPortPowerHistory(pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::SetWssLedMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetWssLedMode(pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}


bool
COpsCmd::GetOscSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    COscSwitchPos stSwitchPos = {0};
    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetOscSwitchPos(stSwitchPos, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                      (uint8_t *)&stSwitchPos, CShmOperator::GetDataOpsOscSwitchPos(), sizeof(COscSwitchPos));


    return true;
}


bool
COpsCmd::SetOscSwitchPos(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOscSwitchPos)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOscSwitchPosSet *pstData = (CCmdOscSwitchPosSet *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetOscSwitchPos(pstData->stSwitchPosSet.uiSwitchIdx, pstData->stSwitchPosSet.uiPos, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}


bool
COpsCmd::SetLedStatus(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOpsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOpsData *pstData = (CCmdOpsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetLedStatus(pstData->uiPort, (uint32_t)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::SetLedMode(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetWssLedMode((uint32_t)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::ClearAlarmDuration(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->ClearAlarmDuration(pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::GetCfgData(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;
    COpsCfgData stData;

    memset(&stData, 0, sizeof(stData));

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

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

    /* set data to shm */
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                      (uint8_t *)&stData, CShmOperator::GetDataOpsCfgPos(), sizeof(stData));

    return true;
}




bool
COpsCmd::DbgSetCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::DbgSetCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::DbgSetCommFail ---");
    return true;
}

bool
COpsCmd::DbgClearCommFail(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::DbgClearCommFail +++");
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

    LOG_WRITE(EN_LOG_NOTICE, "COpsCmd::DbgClearCommFail ---");
    return true;
}

bool
COpsCmd::SetRemoteStatus(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOpsRemoteStatusSet)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOpsRemoteStatusSet *pstData = (CCmdOpsRemoteStatusSet *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetRemoteStatus(pstData->stStatus, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::GetRequestStatus(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdNoData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    COpsBidirectionStatus stStatus = {0};
    CCmdNoData *pstData = (CCmdNoData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->GetRequestStatus(stStatus, pstData->uiSubIdx)) {
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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA),
                      (uint8_t *)&stStatus, CShmOperator::GetDataOpsBidiStaPos(), sizeof(stStatus));


    return true;
}

bool
COpsCmd::SetSwitchingType(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    if (!itIter->second->SetSwitchingType((uint8_t)pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::SetConnectionPdThr(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetConnectionPdThr(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::SetConnectionPdHys(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdThrHysData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdThrHysData *pstData = (CCmdThrHysData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetConnectionPdHys(pstData->uiPort, pstData->uiType, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }

    return true;
}

bool
COpsCmd::SetAlmMask(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if ((uiLen != sizeof(CCmdOpsData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdOpsData *pstData = (CCmdOpsData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetAlmMask(pstData->uiPort, pstData->iVal, pstData->uiSubIdx)) {
        return false;
    }
    return true;
}

bool
COpsCmd::GetEventLog(uint32_t uiLen, const uint8_t *cv_pucBuf)
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

    COpsEventLogAllData stData;

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
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_DATA), itSemIter->second.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataOpsEventLogAllPos(pstData->uiSubIdx), sizeof(stData));

    return true;

}

bool
COpsCmd::__SetShmStatus(uint32_t uiIdx, int32_t uiStatus)
{

    CShmDevSta stSta;

    auto itShmIter = ms_staShm.find(uiIdx);
    if (ms_staShm.end() == itShmIter) {
        return false;
    }

    auto itSemIter = ms_staSem.find(uiIdx);
    if (ms_staSem.end() == itSemIter) {
        return false;
    }

    CShmOperator::Get(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));


    LOG_WRITE(EN_LOG_NOTICE, "+++ Upgrade Status = %d. +++", stSta.iUpgdSta);

    if (uiStatus == stSta.iUpgdSta) {
        return false;
    }

    stSta.iUpgdSta = uiStatus;
    CShmOperator::Set(itShmIter->second.at(SHM_TYPE_STA), itSemIter->second.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));


    return true;
}


