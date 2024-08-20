/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OscApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
COscApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOscMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
COscApi::GetStatusData(uint32_t uiDevIdx, COscStatusData &rstStatus)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOscStaPos(), sizeof(rstStatus));
    return OPLK_OK;

}

int32_t
COscApi::GetCfgData(uint32_t uiDevIdx, COscCfgData &rstCfg)
{



    return OPLK_OK;

}

int32_t
COscApi::SetLaserEnable(uint32_t uiDevIdx, bool bEnable, uint32_t uiSubIdx)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);


    LOG_WRITE(EN_LOG_NOTICE, "SetLaserEnable +++");

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_OSC_SET_LASER_ENABLED, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "OA set attn %d error. uiDevIdx = %d", bEnable, uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "SetLaserEnable ---");

    return OPLK_OK;

}








