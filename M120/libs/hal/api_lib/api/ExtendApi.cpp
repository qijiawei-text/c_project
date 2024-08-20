/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "ExtendApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CExtendApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataExtendMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
CExtendApi::GetStatusData(uint32_t uiDevIdx, CExtendStatusData &rstStatus)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataExtendStaPos(), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}



int32_t
CExtendApi::GetCfgData(uint32_t uiDevIdx, CExtendCfgData &rstCfg)
{



    return OPLK_OK;

}

int32_t
CExtendApi::SetSwitchPos(uint32_t ulDevIdx, uint32_t ulPos, uint32_t ulSubIdx)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetSwitchPos ++ ulDevIdx = %d, ulPos = %d", ulDevIdx, ulPos);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {ulDevIdx, (int)ulPos, ulSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_SWITCH_POS, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch pos error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetSwitchPos --");
    return OPLK_OK;
}


int32_t
CExtendApi::SetSfp(uint32_t ulDevIdx, uint32_t ulVal, uint32_t ulSubIdx)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetSfp ++ ulDevIdx = %d, ulval = %d", ulDevIdx, ulVal);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {ulDevIdx, (int)ulVal, ulSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_SFP, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set sfp error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetSfp --");
    return OPLK_OK;
}


int32_t
CExtendApi::SetVoaAttn(uint32_t ulDevIdx, uint32_t ulVal, uint32_t ulSubIdx)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetVoaAttn ++ ulDevIdx = %d, val = %d", ulDevIdx, ulVal);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {ulDevIdx, (int)ulVal, ulSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_SET_VOA, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set voa error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetVoaAttn --");
    return OPLK_OK;
}


int32_t
CExtendApi::Reset(uint32_t ulDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* let cold reset go */
        if (!(RST_MODE_COLD == ucResetType && ERR_OPERATION_FAILED == iErr)) {
            LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
            return iErr;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {ulDevIdx, (int)ucResetType, ulDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}

int32_t
CExtendApi::Upgrade(uint32_t ulDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdMfgData stData;
    stData.uiIdx = ulDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}

int32_t
CExtendApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    return GetStatus(uiDevIdx, mc_staShmCfg);
}



int32_t
CExtendApi::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_DBG_READ, (uint8_t *)&stData, sizeof(stData), SYNC)) {
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBuf, CShmOperator::GetDataExtendDbgPos(), uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgRead --");
    return OPLK_OK;
}

int32_t
CExtendApi::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
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
    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_DBG_WRITE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Dbg write uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite --");
    return OPLK_OK;
}

int32_t
CExtendApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendApi::DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendApi::DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
CExtendApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EXTEND_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CExtendApi::DbgClearCommFail --");
    return OPLK_OK;
}


