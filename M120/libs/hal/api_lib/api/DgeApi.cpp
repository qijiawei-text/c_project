/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeApi.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DgeApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CDgeApi::GetMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataDgeMfgPos(), sizeof(rstMfg));

    return OPLK_OK;

}


int32_t
CDgeApi::GetStatusData(uint32_t uiDevIdx, CDgeStatusData &rstStatus)
{



    return OPLK_OK;

}

int32_t
CDgeApi::GetCfgData(uint32_t uiDevIdx, CDgeCfgData &rstCfg)
{



    return OPLK_OK;

}

int32_t
CDgeApi::SetCurve(uint32_t uiDevIdx, uint32_t uiPointCnt, CDgePoint *pstPoint)
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

    if (0 == uiPointCnt) {
        return ERR_INVAL_ARG;
    }

    /* generate cmd frame and set to shm */
    uint32_t uiSize = sizeof(CCmdDgeCurveData) + (uiPointCnt - 1) * sizeof(CDgePoint);
    uint8_t *pucData = new uint8_t[uiSize];

    CCmdDgeCurveData *pstData = (CCmdDgeCurveData *)pucData;
    pstData->uiIdx = uiDevIdx;
    pstData->uiPointCnt = uiPointCnt;

    memcpy(&(pstData->stVal), pstPoint, uiPointCnt * sizeof(CDgePoint));

    if (!SetCmd(stShmCmd, stSemCmd, OID_DGE_SET_CURVER, pucData, uiSize, SYNC)) {
        iErr = ERR_OPERATION_FAILED;
    }

    delete [] pucData;

    return iErr;

}




