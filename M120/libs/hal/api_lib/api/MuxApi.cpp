/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MuxApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "MuxApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CMuxApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataMuxMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
CMuxApi::GetCali(uint32_t uiDevIdx, CMuxCaliData &rstCali)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCali, CShmOperator::GetDataMuxCaliPos(), sizeof(rstCali));

    return OPLK_OK;

}


int32_t
CMuxApi::GetStatusData(uint32_t uiDevIdx, CMuxStatusData &rstStatus)
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataMuxStaPos(), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}



int32_t
CMuxApi::GetCfgData(uint32_t uiDevIdx, CMuxCfgData &rstCfg)
{



    return OPLK_OK;

}






