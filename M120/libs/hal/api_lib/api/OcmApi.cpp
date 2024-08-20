/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmApi.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcmApi.h"
#include "Cmd.h"


/*==============================function=========================*/


int32_t
COcmApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    memset(&rstMfg, 0, sizeof(rstMfg));

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOcmMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
COcmApi::GetStatusData(uint32_t uiDevIdx, COcmStatusData &rstStatus)
{

    return OPLK_OK;

}

int32_t
COcmApi::GetChPower(uint32_t uiDevIdx, uint32_t uiPortId, COcmChPower &rstReport)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstReport, CShmOperator::GetDataOcmRptPos(uiPortId), sizeof(rstReport))) {
        LOG_WRITE(EN_LOG_ERROR, "OCM get power error. uiDevIdx = %d, uiPortId = %d", uiDevIdx, uiPortId);
        return OPLK_ERR;
    }

    return OPLK_OK;


}

int32_t
COcmApi::GetRealTimeOsa(uint32_t uiDevIdx, uint32_t uiPortId, COcmOsa &rstReport)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData;
    stData.uiIdx = uiDevIdx;
    stData.iVal = uiPortId;
    stData.uiSubIdx = 0;

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_GET_REALTIME_CH_POWER, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OCM reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstReport, CShmOperator::GetDataOcmOsaPos(uiPortId), sizeof(rstReport))) {
        LOG_WRITE(EN_LOG_ERROR, "OCM get power error. uiDevIdx = %d, uiPortId = %d", uiDevIdx, uiPortId);
        return OPLK_ERR;
    }

    return OPLK_OK;


}

int32_t
COcmApi::GetOsaData(uint32_t uiDevIdx, uint32_t uiPortId, COcmOsa &rstOsa)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstOsa, CShmOperator::GetDataOcmOsaPos(uiPortId), sizeof(rstOsa))) {
        LOG_WRITE(EN_LOG_ERROR, "OCM get osa data error. uiDevIdx = %d, uiPortId = %d", uiDevIdx, uiPortId);
        return OPLK_ERR;
    }

    return OPLK_OK;


}


int32_t
COcmApi::GetCfgData(uint32_t uiDevIdx, COcmCfgData &rstCfg)
{



    return OPLK_OK;

}

int32_t
COcmApi::SetWavePlan(uint32_t uiDevIdx, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
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

#if 0

    /* generate cmd frame and set to shm */
    uint32_t uiSize = sizeof(CCmdWavePlanSliceData);
    uint8_t *pucData = new uint8_t[uiSize];

    CCmdWavePlanSliceData *pstData = (CCmdWavePlanSliceData *)pucData;
    pstData->uiIdx = uiDevIdx;
    pstData->uiPortId = uiPortId;
    pstData->uiChCnt = uiChCnt;

    memcpy(pstData->astVal, pstWavePlan, uiChCnt * sizeof(CWavePlanSlice));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_SET_WAVEPLAN, pucData, uiSize, SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    delete [] pucData;

#else

    /* generate cmd frame and set to shm */
    CCmdWavePlanSliceData stData;

    stData.uiIdx = uiDevIdx;
    stData.uiPortId = uiPortId;
    stData.uiChCnt = uiChCnt;

    memcpy(stData.astVal, pstWavePlan, uiChCnt * sizeof(CWavePlanSlice));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_SET_WAVEPLAN, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

#endif

    return iErr;

}

int32_t
COcmApi::SetOcmWavePlan(uint32_t uiDevIdx, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
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
    CCmdOcmWavePlanData stData;

    stData.uiIdx = uiDevIdx;
    stData.uiPortId = uiPortId;
    stData.uiChCnt = uiChCnt;

    memcpy(stData.astVal, pstWavePlan, uiChCnt * sizeof(STChannelGrid));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_SET_WAVEPLAN_FORMAT_CB, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }


    return iErr;

}


int32_t
COcmApi::Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* let cold reset go */
        if (!(RST_MODE_COLD == ucResetType && ERR_OPERATION_FAILED == iErr)) {
            LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
            return iErr;
        }
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OCM reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}


int32_t
COcmApi::Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(4, S);

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}

int32_t
COcmApi::Commit(uint32_t uiDevIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Commit ++ uiDevIdx = %u", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, 0, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_COMMIT, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OCM Commit error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}


int32_t
COcmApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcmApi::DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcmApi::DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
COcmApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcmApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCM_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcmApi::DbgClearCommFail --");
    return OPLK_OK;
}


int32_t
COcmApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    return GetStatus(uiDevIdx, mc_staShmCfg);
}

