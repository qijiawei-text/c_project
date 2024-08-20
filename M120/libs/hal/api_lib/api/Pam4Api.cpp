/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Pam4Api.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Pam4Api.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CPam4Api::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataPam4MfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
CPam4Api::GetStatusData(uint32_t uiDevIdx, CPam4StatusData &rstStatus)
{



    return OPLK_OK;

}

int32_t
CPam4Api::GetCfgData(uint32_t uiDevIdx, CPam4CfgData &rstCfg)
{



    return OPLK_OK;

}


