/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsApi.cpp
   Author:        Guo Guangjun
   Date:          2024-01-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-05] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcsApi.h"
#include "Cmd.h"


/*==============================function=========================*/


int32_t
COcsApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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
COcsApi::GetStatusData(uint32_t uiDevIdx, COcsStatusData &rstStatus)
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOcsStaPos(), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "OCS get status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;
}

int32_t
COcsApi::GetCfgData(uint32_t uiDevIdx, COcsCfgData &rstCfg)
{
    return OPLK_OK;

}

int32_t
COcsApi::GetSwitchChannels(uint32_t uiDevIdx, COcsConnections &rstConns)
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstConns, CShmOperator::GetDataOcsStaConnsPos(), sizeof(rstConns))) {
        LOG_WRITE(EN_LOG_ERROR, "OCS get connections status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;
}


int32_t
COcsApi::SetSwitchChannels(uint32_t uiDevIdx, const COcsConnections &rstConns)
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
    CCmdOcsConnectionsData stData;

    memset(&stData, 0, sizeof(stData));
    stData.uiIdx = uiDevIdx;

    memcpy(&stData.stVal, &rstConns, sizeof(rstConns));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_SET_SWITCH_CHANNELS, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    return iErr;
}

int32_t
COcsApi::SetSwitchChannel(uint32_t uiDevIdx, const COcsConnection &rstConn)
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
    CCmdOcsConnectionsData stData;

    memset(&stData, 0, sizeof(stData));
    stData.uiIdx = uiDevIdx;
    stData.stVal.uiCount = 1;
    memcpy(stData.stVal.astConn, &rstConn, sizeof(COcsConnection));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_SET_ONE_SWITCH_CHANNEL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    return iErr;
}

int32_t
COcsApi::GetLedState(uint32_t uiDevIdx, COcsLedSta &rstLedSta)
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstLedSta, CShmOperator::GetDataOcsStaLedPos(), sizeof(rstLedSta))) {
        LOG_WRITE(EN_LOG_ERROR, "OCS get led status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;
}

int32_t
COcsApi::SetLampTestState(uint32_t uiDevIdx, bool bEnable)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::SetLampTestState +++");

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
    CCmdData stData = {uiDevIdx, bEnable, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_SET_LAMP_TEST_STATE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::SetLampTestState ---");

    return iErr;
}

int32_t
COcsApi::Reset(uint32_t uiDevIdx, uint8_t ucResetType)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OCS reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}

int32_t
COcsApi::Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(4, S);

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}

int32_t
COcsApi::Commit(uint32_t uiDevIdx)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_COMMIT, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OCS Commit error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}


int32_t
COcsApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
COcsApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OCS_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COcsApi::DbgClearCommFail --");
    return OPLK_OK;
}


int32_t
COcsApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    return GetStatus(uiDevIdx, mc_staShmCfg);
}

