/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssApi.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "WssApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CWssApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataWssMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
CWssApi::GetStatusData(uint32_t uiDevIdx, uint32_t uiSubIdx, CWssStatusData &rstStatus)
{



    return OPLK_OK;
}

int32_t
CWssApi::GetCfgData(uint32_t uiDevIdx, CWssAllCfg &rstCfg)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData;
    stData.uiIdx = uiDevIdx;
    stData.uiSubIdx = 0;

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_GET_ALL_CFG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCfg, CShmOperator::GetDataWssCfgPos(), sizeof(rstCfg))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get cfg data error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    for (uint32_t uiIdx = 0; uiIdx < WSS_CH_CNT_BUSINESS; uiIdx++ ) {
        memcpy(rstCfg.astWavePlan + uiIdx, (uint8_t *)(rstCfg.astWavePlan + uiIdx + 1), sizeof(CWavePlan));
        memcpy(rstCfg.astWssChCfgAdd + uiIdx, (uint8_t *)(rstCfg.astWssChCfgAdd + uiIdx + 1), sizeof(CWssCfg));
        memcpy(rstCfg.astWssChCfgDrop + uiIdx, (uint8_t *)(rstCfg.astWssChCfgDrop + uiIdx + 1), sizeof(CWssCfg));
    }

    return OPLK_OK;
}


int32_t
CWssApi::GetOscCfgData(uint32_t uiDevIdx, CWssAllCfg &rstCfg)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData;
    stData.uiIdx = uiDevIdx;
    stData.uiSubIdx = 0;

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_GET_ALL_CFG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCfg, CShmOperator::GetDataWssCfgPos(), sizeof(rstCfg))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get cfg data error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;
}




int32_t
CWssApi::SetAllChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (0 == uiChCnt) {
        return ERR_INVAL_ARG;
    }

    /* generate cmd frame and set to shm */
    uint32_t uiSize = sizeof(CCmdWssCfgData);
    uint8_t *pucData = new uint8_t[uiSize];

    CCmdWssCfgData *pstData = (CCmdWssCfgData *)pucData;
    pstData->uiIdx = uiDevIdx;
    pstData->uiSubIdx = uiSubIdx;
    pstData->uiInstBegin = WSS_BUSINESS_CH_IDX_BEGIN;
    pstData->uiInstEnd = WSS_BUSINESS_CH_IDX_END;

    memcpy(pstData->astVal, pstWssCfg, uiChCnt * sizeof(CWssCfg));

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_SET_CFG, pucData, uiSize, SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    delete [] pucData;

    return iErr;
}


int32_t
CWssApi::SetOscChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (0 == uiChCnt) {
        return ERR_INVAL_ARG;
    }

    /* generate cmd frame and set to shm */
    uint32_t uiSize = sizeof(CCmdWssCfgData);
    uint8_t *pucData = new uint8_t[uiSize];

    CCmdWssCfgData *pstData = (CCmdWssCfgData *)pucData;
    pstData->uiIdx = uiDevIdx;
    pstData->uiSubIdx = uiSubIdx;
    pstData->uiInstBegin = WSS_OSC_CH_IDX_BEGIN;
    pstData->uiInstEnd = WSS_OSC_CH_IDX_END;

    memcpy(pstData->astVal, pstWssCfg, uiChCnt * sizeof(CWssCfg));

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_SET_CFG, pucData, uiSize, SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    delete [] pucData;

    return iErr;
}


int32_t
CWssApi::SetWavePlan(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (0 == uiChCnt) {
        return ERR_INVAL_ARG;
    }

    /* generate cmd frame and set to shm */
    uint32_t uiSize = sizeof(CCmdWavePlanData);
    uint8_t *pucData = new uint8_t[uiSize];

    CCmdWavePlanData *pstData = (CCmdWavePlanData *)pucData;
    pstData->uiIdx = uiDevIdx;
    pstData->uiSubIdx = uiSubIdx;
    pstData->uiChCnt = uiChCnt;

    memcpy(&(pstData->astVal), pstWavePlan, uiChCnt * sizeof(CWavePlan));

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_SET_WAVEPLAN, pucData, uiSize, SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    delete [] pucData;

    return iErr;

}


int32_t
CWssApi::Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset ++ uiDevIdx = %u, ucResetType = %hhu", uiDevIdx, ucResetType);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)ucResetType, uiDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}


int32_t
CWssApi::Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK && iErr != ERR_UPGRADING) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdMfgData stData;
    stData.uiIdx = uiDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}

int32_t
CWssApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    return GetStatus(uiDevIdx, mc_staShmCfg);
}

int32_t
CWssApi::GetDataForUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucData, CShmOperator::GetDataWssUserDataPos(), nLen);

    return OPLK_OK;
}


int32_t
CWssApi::SetDataForUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr); */
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Set(stShmData, stSemData, (uint8_t *)pucData, CShmOperator::GetDataWssUserDataPos(), nLen);

    return OPLK_OK;
}

int32_t
CWssApi::GetDataForPmUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr); */
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucData, CShmOperator::GetDataPmUserDataPos(), nLen);

    return OPLK_OK;
}


int32_t
CWssApi::SetDataForPmUser(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /*LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);*/
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Set(stShmData, stSemData, (uint8_t *)pucData, CShmOperator::GetDataPmUserDataPos(), nLen);

    return OPLK_OK;
}

int32_t
CWssApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
CWssApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CWssApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_WSS_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CWssApi::DbgClearCommFail --");
    return OPLK_OK;
}

