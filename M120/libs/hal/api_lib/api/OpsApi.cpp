/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsApi.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OpsApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
COpsApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOpsMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}


int32_t
COpsApi::GetCommStatusData(uint32_t uiDevIdx, COpsCommStatusData &rstStatus)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetCommonDataOpsStaPos(), sizeof(rstStatus));


    return OPLK_OK;
}

int32_t
COpsApi::GetStatusData(uint32_t uiDevIdx, COpsStatusData &rstStatus, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        /* To avoid frequent log flushing,  do not record error logs when upgrading and reseting */
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOpsStaPos(uiSubIdx), sizeof(rstStatus));


    return OPLK_OK;
}

int32_t
COpsApi::GetHistoryData(uint32_t uiDevIdx, COpsHistoryData &rstData, uint32_t uiSubIdx)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
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
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_HISTORY_DATA, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS GetPortPowerHistory error.");
        return ERR_OPERATION_FAILED;
    }


    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstData, CShmOperator::GetDataOpsHistoryPos(uiSubIdx), sizeof(rstData));


    return OPLK_OK;
}


int32_t
COpsApi::GetCfgData(uint32_t uiDevIdx, COpsCfgData &rstCfg, uint32_t uiSubIdx)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_ALL_CFG, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops get all cfg data error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {

        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCfg, CShmOperator::GetDataOpsCfgPos(uiSubIdx), sizeof(rstCfg));

    return OPLK_OK;
}

int32_t
COpsApi::GetLedStatus(uint32_t uiDevIdx, COpsLed &rstStatus, uint32_t uiSubIdx)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOpsLedStaPos(), sizeof(rstStatus));


    return OPLK_OK;
}



int32_t
COpsApi::SetMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetMode ++ uiDevIdx = %d, uiMode = %d", uiDevIdx, uiMode);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)uiMode, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_MODE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS set mode %d error. uiDevIdx = %d", uiMode, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetMode --");

    return OPLK_OK;
}

int32_t
COpsApi::SetWtr(uint32_t uiDevIdx, uint32_t uiWtr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetWtr ++ uiDevIdx = %d, uiWtr = %d", uiDevIdx, uiWtr);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)uiWtr, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_WTR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set wtr %d error. uiDevIdx = %d", uiWtr, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetWtr --");

    return OPLK_OK;
}

int32_t
COpsApi::SetHoldOffTime(uint32_t uiDevIdx, uint32_t uiHoldOffTime, uint32_t uiSubIdx)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHoldOffTime ++ uiDevIdx = %d, uiHoldOffTime = %d", uiDevIdx, uiHoldOffTime);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)uiHoldOffTime, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_HOLDOFFTIME, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set hold off time %d error. uiDevIdx = %d", uiHoldOffTime, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHoldOffTime --");

    return OPLK_OK;
}

int32_t
COpsApi::SetRevertive(uint32_t uiDevIdx, uint32_t uiRevertive, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRevertive ++ uiDevIdx = %d, uiRevertive = %d", uiDevIdx, uiRevertive);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)uiRevertive, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_REVERTIVE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set revertive %d error. uiDevIdx = %d", uiRevertive, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRevertive --");

    return OPLK_OK;
}

int32_t
COpsApi::SetRelativeThr(uint32_t uiDevIdx, int32_t iThr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRelativeThr ++ uiDevIdx = %d, iThr = %d", uiDevIdx, iThr);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)iThr, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_RELATIVE_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set relative thr = %d error. uiDevIdx = %d", iThr, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRelativeThr --");

    return OPLK_OK;
}

int32_t
COpsApi::SetRelativeOffset(uint32_t uiDevIdx, int32_t iOffset, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRelativeOffset ++ uiDevIdx = %d, iOffset = %d", uiDevIdx, iOffset);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)iOffset, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_RELATIVE_OFFSET, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set relative iOffset = %d error. uiDevIdx = %d", iOffset, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetRelativeOffset --");

    return OPLK_OK;
}



int32_t
COpsApi::SetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetThr ++ uiDevIdx = %d, uiPort = %d, uiType = %d, iThr = %d", uiDevIdx, uiPort, uiType, iThr);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdThrHysData stData = {uiDevIdx, uiPort, uiType, iThr, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_PORT_POWER_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set port%d threshold %d error. uiDevIdx = %d", uiPort, iThr, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetThr --");

    return OPLK_OK;
}


int32_t
COpsApi::SetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHys ++ uiDevIdx = %d, uiPort = %d, uiType = %d, iHys = %d", uiDevIdx, uiPort, uiType, iHys);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdThrHysData stData = {uiDevIdx, uiPort, uiType, iHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_PORT_POWER_HYS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set port%d hys %d error. uiDevIdx = %d", uiPort, iHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHys --");

    return OPLK_OK;
}


int32_t
COpsApi::SetSwitchThr(uint32_t uiDevIdx, uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetSwitchThr +++ uiDevIdx = %d, uiPos = %d, iThr = %d", uiDevIdx, uiPos, iThr);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOpsData stData = {uiDevIdx, uiPos, iThr, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_SWITCH_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set uiPos = %d, iThr = %d error. uiDevIdx = %d", uiPos, iThr, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetSwitchThr --");

    return OPLK_OK;
}

int32_t
COpsApi::GetSwitchThr(uint32_t uiDevIdx, COpsSwitchThr &rstThr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetSwitchThr +++ uiDevIdx = %d, uiSubIdx = %d", uiDevIdx, uiSubIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_SWITCH_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetSwitchThr error. uiDevIdx = %d, uiSubIdx = %d", uiDevIdx, uiSubIdx);
        return ERR_OPERATION_FAILED;
    }

    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstThr, CShmOperator::GetDataOpsSwitchThrPos(uiSubIdx), sizeof(rstThr));

    LOG_WRITE(EN_LOG_TRACE, "GetSwitchThr ---");

    return OPLK_OK;
}


int32_t
COpsApi::SetSwitchHys(uint32_t uiDevIdx, uint32_t uiPos, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetSwitchHys +++ uiDevIdx = %d, uiPos = %d, iThr = %d", uiDevIdx, uiPos, iHys);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOpsData stData = {uiDevIdx, uiPos, iHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_SWITCH_HYS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set uiPos = %d, iThr = %d error. uiDevIdx = %d", uiPos, iHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetSwitchHys ---");

    return OPLK_OK;
}

int32_t
COpsApi::GetSwitchHys(uint32_t uiDevIdx, COpsSwitchHys &rstHys, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetSwitchHys +++ uiDevIdx = %d, uiSubIdx = %d", uiDevIdx, uiSubIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_SWITCH_HYS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetSwitchHys error. uiDevIdx = %d, uiSubIdx = %d", uiDevIdx, uiSubIdx);
        return ERR_OPERATION_FAILED;
    }

    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstHys, CShmOperator::GetDataOpsSwitchHysPos(uiSubIdx), sizeof(rstHys));

    LOG_WRITE(EN_LOG_TRACE, "GetSwitchHys ---");

    return OPLK_OK;
}


int32_t
COpsApi::GetWssStatusData(uint32_t uiDevIdx, CWssStatusData &rstStatus, uint32_t uiSubIdx)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOpsWssStaPos(uiSubIdx), sizeof(rstStatus));

    return OPLK_OK;

}



int32_t
COpsApi::SetWssPortPowerThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetThr ++ uiDevIdx = %d, uiPort = %d, uiType = %d, iThr = %d", uiDevIdx, uiPort, uiType, iThr);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdThrHysData stData = {uiDevIdx, uiPort, uiType, iThr, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_WSS_PORT_POWER_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set port%d threshold %d error. uiDevIdx = %d", uiPort, iThr, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetThr --");

    return OPLK_OK;
}


int32_t
COpsApi::SetWssPortPowerHys(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHys ++ uiDevIdx = %d, uiPort = %d, uiType = %d, iHys = %d", uiDevIdx, uiPort, uiType, iHys);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdThrHysData stData = {uiDevIdx, uiPort, uiType, iHys, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_WSS_PORT_POWER_HYS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set port%d hys %d error. uiDevIdx = %d", uiPort, iHys, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHys --");

    return OPLK_OK;
}

int32_t
COpsApi::GetWssPortPowerHistory(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPowerHirtory ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_WSS_PORT_HISTORY_POWER, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPowerHirtory error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetHys --");

    return OPLK_OK;
}

int32_t
COpsApi::SetWssLedMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetWssLedMode +++ uiDevIdx = %d, uiMode = %d", uiDevIdx, uiMode);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)uiMode, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_WSS_LED_MODE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops set led mode error. uiDevIdx = %d, uiMode = %d", uiDevIdx, uiMode);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetWssLedMode ---");

    return OPLK_OK;
}

int32_t
COpsApi::GetOscSwitchPos(uint32_t uiDevIdx, COscSwitchPos &rstSwitchPos, uint32_t uiSubIdx)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_OSC_SWITCH_POS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops set led mode error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {

        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstSwitchPos, CShmOperator::GetDataOpsOscSwitchPos(uiSubIdx), sizeof(rstSwitchPos));

    return OPLK_OK;
}


int32_t
COpsApi::SetOscSwitchPos(uint32_t uiDevIdx, COscSwitchPosSet &rstSwitchPosSet, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetOscSwitchPos +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOscSwitchPosSet stData = {0};
    stData.uiIdx = uiDevIdx;

    stData.stSwitchPosSet.uiSwitchIdx = rstSwitchPosSet.uiSwitchIdx;
    stData.stSwitchPosSet.uiPos = rstSwitchPosSet.uiPos;

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_OSC_SWITCH_POS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops set led mode error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetOscSwitchPos ---");

    return OPLK_OK;
}

int32_t
COpsApi::SetLedStatus(uint32_t uiDevIdx, uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedStatus +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOpsData stData = {0};
    stData.uiIdx = uiDevIdx;
    stData.uiPort = uiLedIdx;
    stData.iVal = (int32_t)uiStatus;

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_LED_STATUS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops set led status error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedStatus ---");

    return OPLK_OK;
}

int32_t
COpsApi::SetLedMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedMode +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {0};
    stData.uiIdx = uiDevIdx;
    stData.iVal = (int32_t)uiMode;

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_LED_MODE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops set led mode error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedMode ---");

    return OPLK_OK;
}

int32_t
COpsApi::ClearAlarmDuration(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDuration +++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {0};
    stData.uiIdx = uiDevIdx;

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_CLEAR_ALARM_DURATION, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "ops clear alarm duration time error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDuration ---");

    return OPLK_OK;
}



int32_t
COpsApi::Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "Reset ++ uiDevIdx = %d type = %d", uiDevIdx, ucResetType);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)ucResetType, uiDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_TRACE, "Reset --");
    return OPLK_OK;
}


int32_t
COpsApi::Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "Upgrade ++ uiDevIdx = %d", uiDevIdx);

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_TRACE, "Upgrade --");
    return OPLK_OK;
}

int32_t
COpsApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsApi::DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsApi::DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
COpsApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsApi::DbgClearCommFail --");
    return OPLK_OK;
}


int32_t
COpsApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    int32_t iStatus = GetStatus(uiDevIdx, mc_staShmCfg);

    return iStatus;
}

int32_t
COpsApi::SetRemoteStatus(uint32_t uiDevIdx, COpsReqStatus &stStatus, uint32_t uiSubIdx)
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
    CCmdOpsRemoteStatusSet stData = {uiDevIdx, stStatus, uiSubIdx};
    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_REMOTE_STATUS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag, QUEUE_PRIO_HIGH)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS set remote status error. uiDevIdx = %u, sta = 0x%x, sig = 0x%x",
                  uiDevIdx, stStatus.ucApsState, stStatus.ucRequestedSignal);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COpsApi::GetRequestStatus(uint32_t uiDevIdx, COpsBidirectionStatus &stStatus, uint32_t uiSubIdx)
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
    CCmdNoData stData = {uiDevIdx, uiSubIdx};
    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_REQUEST_STATUS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS get request status error. uiDevIdx = %u", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CShareMemory stShmData;
    CSemaphore stSemData;
    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {

        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %u", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&stStatus, CShmOperator::GetDataOpsBidiStaPos(uiSubIdx), sizeof(stStatus));

    return OPLK_OK;

}

int32_t
COpsApi::SetSwitchingType(uint32_t uiDevIdx, uint8_t ucType, uint32_t uiSubIdx)
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
    CCmdData stData = {uiDevIdx, ucType, uiSubIdx};
    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_SWITCHING_TYPE, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OPS set switching type error. uiDevIdx = %u, uiType = %u", uiDevIdx, ucType);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COpsApi::SetConnectionPdThr(uint32_t uiDevIdx, uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx)
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
    CCmdThrHysData stData = {uiDevIdx, uiPdType, uiThrType, iThr, uiSubIdx};
    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_CONN_PD_THR, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set connection pd thr error. uiDevIdx = %u, uiPdType = %u, uiThrType = %u, uiSubIdx = %u", \
                  uiDevIdx, uiPdType, uiThrType, uiSubIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COpsApi::SetConnectionPdHys(uint32_t uiDevIdx, uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx)
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
    CCmdThrHysData stData = {uiDevIdx, uiPdType, uiHysType, iHys, uiSubIdx};
    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_CONN_PD_HYS, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set connection pd hys error. uiDevIdx = %u, uiPdType = %u, uiHysType = %u, uiSubIdx = %u", \
                  uiDevIdx, uiPdType, uiHysType, uiSubIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
COpsApi::SetAlmMask(uint32_t uiDevIdx, uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetAmMask +++ uiDevIdx = %d, uiIdx = %d, iMask = %d", uiDevIdx, uiIdx, iMask);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOpsData stData = {uiDevIdx, uiIdx, iMask, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_SET_ALM_MASK, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set AmMask uiIdx = %d, iMask = %d error. uiDevIdx = %d", uiIdx, iMask, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetAmMask --");

    return OPLK_OK;
}

int32_t
COpsApi::GetEventLog(uint32_t ulDevIdx, COpsEventLogAllData &rstEventLogAll, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_DEBUG, "COpsApi::GetEventLogAll +++");

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OPS_GET_EVENTLOG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "OID_OPS_GET_EVENTLOG error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstEventLogAll, CShmOperator::GetDataOpsEventLogAllPos(uiSubIdx), sizeof(rstEventLogAll))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COpsApi::GetEventLogAll ---");
    return OPLK_OK;
}

