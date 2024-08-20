/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DceApi.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DceApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CDceApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataDceMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}

int32_t
CDceApi::GetStatusData(uint32_t uiDevIdx, CDceStatusData &rstStatus)
{



    return OPLK_OK;

}


int32_t
CDceApi::GetCfgData(uint32_t uiDevIdx, CDceCfgData &rstCfg)
{



    return OPLK_OK;

}


