/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaApi.h"
#include "Cmd.h"

/*==============================function=========================*/
#if 0
int32_t
COaApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOaMfgPos(), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get mfginfo error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}
#endif

int32_t
COaApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

#if 0
    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_MFG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }
#endif

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    memset(&rstMfg, 0, sizeof(rstMfg));

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOaMfgPos(), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get mfginfo error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
COaApi::GetStatusData(uint32_t uiDevIdx, COaStatusData &rstStatus, uint32_t uiSubIdx)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        LOG_WRITE(EN_LOG_TRACE, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOaStaPos(uiSubIdx), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
COaApi::GetCfgData(uint32_t uiDevIdx, COaCfgData &rstCfg, uint32_t uiSubIdx)
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
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_ALL_CFG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCfg, CShmOperator::GetDataOaCfgPos(uiSubIdx), sizeof(rstCfg))) {
        LOG_WRITE(EN_LOG_ERROR, "OA get cfg data error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
COaApi::GetDebugInfo(uint32_t uiDevIdx, const char *pcFilePath)
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
    CCmdBufData stData = {0};

    stData.uiIdx = uiDevIdx;
    strncpy((char *)stData.aucBuf, pcFilePath, sizeof(stData.aucBuf) - 1);

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_DEBUG_INFO, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::GetMcuDebugInfo(uint32_t uiDevIdx, const char *pcFilePath, uint32_t uiSubIdx)
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
    CCmdBufData stData = {0};

    stData.uiIdx = uiDevIdx;
    strncpy((char *)stData.aucBuf, pcFilePath, sizeof(stData.aucBuf) - 1);
    stData.uiSubIdx = uiSubIdx;
    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_MCU_DEBUG_INFO, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetGainRange(uint32_t uiDevIdx, uint32_t uiGainRange, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, (int)uiGainRange, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_GAINRANG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set gain range %d error. uiDevIdx = %d", uiGainRange, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}


int32_t
COaApi::SetGain(uint32_t uiDevIdx, uint32_t uiGain, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetGain +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiGain = %d, uiSubIdx = %d", uiDevIdx, uiGain, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiGain, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_GAIN, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set gain %d error. uiDevIdx = %d", uiGain, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetGain ---");

    return OPLK_OK;

}

int32_t
COaApi::SetTilt(uint32_t uiDevIdx, int16_t sTilt, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetTilt +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, sTilt = %hd, uiSubIdx = %d", uiDevIdx, sTilt, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)sTilt, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_TILT, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set tilt %d error. uiDevIdx = %d", sTilt, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetTilt +++");

    return OPLK_OK;

}
int32_t
COaApi::SetMode(uint32_t uiDevIdx, uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx)
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
    CCmdKeyData stData = {uiDevIdx, (int32_t)ucMode, (int32_t)sVal, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_MODE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set mode %d error. uiDevIdx = %d", ucMode, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetAprEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAprEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_APR_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set apr %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAprEnable ---");

    return OPLK_OK;

}


int32_t
COaApi::SetPumpEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_PUMP_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set pump enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetAutolosEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAutolosEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_AUTO_LOS_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set auto los %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAutolosEnable ---");

    return OPLK_OK;

}

int32_t
COaApi::SetRxLosThr(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
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
    CCmdThrData stData = {uiDevIdx, sThr, sHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_RX_LOS_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set rx los thr [%d, %d] error. uiDevIdx = %d", sThr, sHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetAttn(uint32_t uiDevIdx, int16_t sAttn, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, (int32_t)sAttn, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_ATTN, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set attn %d error. uiDevIdx = %d", sAttn, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetTarget(uint32_t uiDevIdx, int16_t sTarget, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, (int32_t)sTarget, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OSC_SET_TX_TARGET_POWER, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set target power %d error. uiDevIdx = %d", sTarget, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgRead ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdDbgData stData = {uiDevIdx, uiLen};
    if (uiLen > sizeof(stData.aucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "Buffer overflow. uiDevIdx = %d", 0);
        return ERR_OVERFLOW_ARG;
    }

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_READ, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Dbg read uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* get from memeory */
    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBuf, CShmOperator::GetDataOaDbgPos(0), uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgRead --");
    return OPLK_OK;
}

int32_t
COaApi::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdDbgData stData = {uiDevIdx, uiLen};
    if (uiLen > sizeof(stData.aucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "Buffer overflow. uiDevIdx = %d", 0);
        return ERR_OVERFLOW_ARG;
    }

    memcpy(stData.aucBuf, pucBuf, uiLen);
    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_WRITE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Dbg write uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite --");
    return OPLK_OK;
}


int32_t
COaApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
COaApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearCommFail --");
    return OPLK_OK;
}

int32_t
COaApi::DbgSetModuleAlarm(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetModuleAlarm ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_SET_MODULE_ALARM, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetModuleAlarm uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgSetModuleAlarm --");
    return OPLK_OK;
}

int32_t
COaApi::DbgClearModuleAlarm(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearModuleAlarm ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_DBG_CLEAR_MODULE_ALARM, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearModuleAlarm uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgClearModuleAlarm --");
    return OPLK_OK;
}



int32_t
COaApi::Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* let cold reset go */
        if (!(RST_MODE_COLD == ucResetType && ERR_OPERATION_FAILED == iErr)) {
            LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
            return iErr;
        }

        printf("COaApi::Reset : %d\n", ucResetType);
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)ucResetType, uiDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OA reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}



int32_t
COaApi::Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK && iErr != ERR_UPGRADING) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    printf("Upgrade ++ uiDevIdx = %d\r\n", uiDevIdx);

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OA Upgrade error. uiDevIdx = %d", uiDevIdx);
        printf("OA Upgrade error. uiDevIdx = %d, file path = %s", uiDevIdx, c_pcFilePath);
        return ERR_OPERATION_FAILED;
    }

    /* CDelay::Delay(1, S); */

    printf("Upgrade --\r\n");
    return OPLK_OK;
}

int32_t
COaApi::Swap(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Swap +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SWAP, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA Swap error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Swap ---");
    return OPLK_OK;
}

int32_t
COaApi::Commit(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Commit +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_COMMIT, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA Commit error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Commit ---");
    return OPLK_OK;
}

int32_t
COaApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    int32_t iStatus = GetStatus(uiDevIdx, mc_staShmCfg);
    LOG_WRITE(EN_LOG_DEBUG, "Upgrade Status = %d", iStatus);
    return iStatus;
}

int32_t
COaApi::SetDfbEnable(uint32_t uiDevIdx, bool bEnable, int32_t iPower, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COaApi::SetDfbEnable +++");

    LOG_WRITE(EN_LOG_TRACE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdKeyData stData = {uiDevIdx, bEnable, iPower, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_DFB_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set dfb enable %d error. uiDevIdx = %d, iPower = %d", bEnable, iPower, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetOscEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOscEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OSC_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set osc enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetOscAmpEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOscAmpEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OSC_AMP_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set osc amp enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetOtdrSwitchPos(uint32_t uiDevIdx, uint32_t uiPos, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOtdrSwitchPos +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiPos = %d, uiSubIdx = %d", uiDevIdx, uiPos, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiPos, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OTDR_SWITCH_POS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set uiPos %d error. uiDevIdx = %d", uiPos, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOtdrSwitchPos ---");

    return OPLK_OK;

}

int32_t
COaApi::SetOscAddThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetSetOscAddThrHys +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, sThr = %d, sHys = %d, uiSubIdx = %d", uiDevIdx, sThr, sHys, uiSubIdx);
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
    CCmdThrData stData = {uiDevIdx, sThr, sHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OSC_ADD_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set osc add thr [%d, %d] error. uiDevIdx = %d", sThr, sHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetOscDropThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetSetOscDropThrHys +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, sThr = %d, sHys = %d, uiSubIdx = %d", uiDevIdx, sThr, sHys, uiSubIdx);
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
    CCmdThrData stData = {uiDevIdx, sThr, sHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OSC_DROP_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set osc drop thr [%d, %d] error. uiDevIdx = %d", sThr, sHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetOscRxThrHys(uint32_t uiDevIdx, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetSetOscRxThrHys +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, sThr = %d, sHys = %d, uiSubIdx = %d", uiDevIdx, sThr, sHys, uiSubIdx);
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
    CCmdThrData stData = {uiDevIdx, sThr, sHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OSC_RX_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set osc rx thr [%d, %d] error. uiDevIdx = %d", sThr, sHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetApsdEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetApsdEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_APSD_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set apsd enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetUpgState(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetUpgState +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiState = %d, uiSubIdx = %d", uiDevIdx, uiState, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiState, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_UPG_STATE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set gain %d error. uiDevIdx = %d", uiState, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetUpgState ---");

    return OPLK_OK;

}

int32_t
COaApi::SetLedState(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaApi::SetLedState +++");

    LOG_WRITE(EN_LOG_DEBUG, "uiDevIdx = %d, uiState = %d, uiSubIdx = %d", uiDevIdx, uiState, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiState, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_LED_STATE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        /*LOG_WRITE(EN_LOG_ERROR, "OA set gain %d error. uiDevIdx = %d", uiState, uiDevIdx);*/
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetLedState ---");

    return OPLK_OK;

}

int32_t
COaApi::SetLedAlm(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaApi::SetLedAlm +++");

    LOG_WRITE(EN_LOG_DEBUG, "uiDevIdx = %d, uiState = %d, uiSubIdx = %d", uiDevIdx, uiState, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiState, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_LED_ALM, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        /*LOG_WRITE(EN_LOG_ERROR, "OA set gain %d error. uiDevIdx = %d", uiState, uiDevIdx);*/
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_DEBUG, "COaApi::SetLedAlm ---");

    return OPLK_OK;

}


int32_t
COaApi::GetCaliInfo(uint32_t ulDevIdx, COaCaliData &rstCaliData, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_DEBUG, "COaApi::GetCaliInfo +++");

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {ulDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_CALI, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OID_OA_GET_CALI error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCaliData, CShmOperator::GetDataOaCaliPos(), sizeof(rstCaliData))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaApi::GetCaliInfo ---");
    return OPLK_OK;
}



int32_t
COaApi::SetOtdrCali(uint32_t uiDevIdx, char *c_pcFilePath, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaApi::SetOtdrCali +++");

    LOG_WRITE(EN_LOG_DEBUG, "uiDevIdx = %d, pcFilePath = %s, uiSubIdx = %d", uiDevIdx, c_pcFilePath, uiSubIdx);

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
    CCmdCaliData stData = {0};
    stData.uiIdx = uiDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OTDR_CALI, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set caliinfo error.");
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOtdrCali ---");

    return OPLK_OK;

}


int32_t
COaApi::SetOcmCali(uint32_t uiDevIdx, char *c_pcFilePath, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOcmCali +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, pcFilePath = %s, uiSubIdx = %d", uiDevIdx, c_pcFilePath, uiSubIdx);

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
    CCmdCaliData stData = {0};
    stData.uiIdx = uiDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_OCM_CALI, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set caliinfo error.");
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetOcmCali ---");

    return OPLK_OK;

}

int32_t
COaApi::SetManualVoaEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetManualVoaEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_MANUAL_VOA_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set manual voa enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}


int32_t
COaApi::SetPaddingVoaConfigPower(uint32_t uiDevIdx, int16_t sOutPower, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, (int32_t)sOutPower, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_PADDING_VOA_CONFIG_POWER, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set PaddingVoaConfigPower %d error. uiDevIdx = %d", sOutPower, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}


int32_t
COaApi::SetManualGainEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetManualGainEnable +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, bEnable = %d, uiSubIdx = %d", uiDevIdx, bEnable, uiSubIdx);
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
    CCmdData stData = {uiDevIdx, bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_MANUAL_GAIN_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set manual gain enable %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetThrHys(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetThrHys +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiPortIdx = %d, uiType = %d, sThr = %d, sHys = %d, uiSubIdx = %d", uiDevIdx, uiPortIdx, uiType, sThr, sHys, uiSubIdx);
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
    CCmdThrCommonData stData = {uiDevIdx, uiPortIdx, uiType, sThr, sHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set oa thr [%d, %d, %d, %d] error. uiDevIdx = %d", uiPortIdx, uiType, sThr, sHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::SetPumpCurrent(uint32_t uiDevIdx, uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetPumpCurrent +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiPumpIdx = %d, sCurrent = %hd, uiSubIdx = %d", uiDevIdx, uiPumpIdx, sCurrent, uiSubIdx);

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
    CCmdKeyData stData = {uiDevIdx, (int32_t)uiPumpIdx, (int32_t)sCurrent, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_PUMP_CURRENT, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set Pump[%d]Current  %d error. uiDevIdx = %d", uiPumpIdx, sCurrent, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetPumpCurrent +++");

    return OPLK_OK;

}

int32_t
COaApi::SetAlmMask(uint32_t uiDevIdx, int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx)
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
    CCmdKeyData stData = {uiDevIdx, (int32_t)iMaskIdx, (int32_t)iMaskVal, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_ALM_MASK, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set alm mask idx%d val%d error. uiDevIdx = %d", iMaskIdx, iMaskVal, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
COaApi::GetEventLog(uint32_t ulDevIdx, COaEventLogAllData &rstEventLogAll, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_DEBUG, "COaApi::GetEventLogAll +++");

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {ulDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_EVENTLOG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OID_OA_GET_EVENTLOG error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstEventLogAll, CShmOperator::GetDataOaEventLogAllPos(uiSubIdx), sizeof(rstEventLogAll))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }
#if 0
    for (int i = 0; i < 4; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostTimeCount	  = 0x%0x", i, rstEventLogAll.stOaEventLogData[i].uiLostTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostPD			  = 0x%0x", i, rstEventLogAll.stOaEventLogData[i].uiLostPD);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLinkDownTimeCount = 0x%0x", i, rstEventLogAll.stOaEventLogData[i].uiLinkDownTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiApsdTimeCount	  = 0x%0x", i, rstEventLogAll.stOaEventLogData[i].uiApsdTimeCount);
    }
#endif

    LOG_WRITE(EN_LOG_DEBUG, "COaApi::GetEventLogAll ---");
    return OPLK_OK;
}

int32_t
COaApi::SetShutterState(uint32_t uiDevIdx, bool block, uint32_t uiSubIdx)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)block, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_SHUTTER_STATE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set shutter state %d error. uiDevIdx = %u", block, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COaApi::GetShutterState(uint32_t uiDevIdx, bool &block, uint32_t uiSubIdx)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_GET_SHUTTER_STATE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OID_OA_GET_SHUTTER_STATE error. ulDevIdx = %u", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    uint8_t trans = 0;
    if (!CShmOperator::Get(stShmData, stSemData, &trans, CShmOperator::GetDataOaShutterPos(uiSubIdx), sizeof(trans))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %u", uiDevIdx);
        return OPLK_ERR;
    }

    block = (bool)trans;

    return OPLK_OK;
}

int32_t
COaApi::SetReadyTrigger(uint32_t uiDevIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetReadyTrigger +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiState = %d, uiSubIdx = %d", uiDevIdx, uiState, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiState, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_EDFA_READY_TRIGGER, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set uiPos %d error. uiDevIdx = %d", uiState, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetReadyTrigger ---");

    return OPLK_OK;

}

int32_t
COaApi::SetAutogainMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAutogainMode +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiMode = %d, uiSubIdx = %d", uiDevIdx, uiMode, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiMode, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_EDFA_AUTOGAIN_MODE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set uiMode %d error. uiDevIdx = %d", uiMode, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetAutogainMode ---");

    return OPLK_OK;

}

int32_t
COaApi::SetRemoteInterlockEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_REMOTE_INTERLOCK_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set remote_interlock bEnable %d error. uiDevIdx = %u", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COaApi::SetDummyAdminKey(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_DUMMY_ADMIN_KEY_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set dummy_admin_key bEnable %d error. uiDevIdx = %u", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COaApi::SetPanelSafeCmpt(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)bEnable, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_PANEL_SAFE_CMPT_ENABLE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set panel safety cmoponents bEnable %d error. uiDevIdx = %u", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COaApi::SetReadyPower(uint32_t uiDevIdx, uint32_t uiPower, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetReadyPower +++");

    LOG_WRITE(EN_LOG_NOTICE, "uiDevIdx = %d, uiMode = %d, uiSubIdx = %d", uiDevIdx, uiPower, uiSubIdx);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiPower, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OA_SET_EDFA_READY_POWER, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set ready power %d error. uiDevIdx = %d", uiPower, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COaApi::SetReadyPower ---");

    return OPLK_OK;

}

