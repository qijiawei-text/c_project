/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DevApi.h"
#include "Cmd.h"
#include "DevData.h"

/*==============================function=========================*/

bool
CDevApi::IsShmCreated(uint32_t uiDevIdx, uint32_t uiShmIdx, const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg,
                      CShareMemory &rstShm, CSemaphore &rstSem)
{

    if ((uiShmIdx >= SHM_TYPE_MAX) || (uiDevIdx >= DEV_MAX_NUM)) {
        LOG_WRITE(EN_LOG_ERROR, "uiShmIdx = %d, uiDevIdx = %d", uiShmIdx, uiDevIdx);
        return false;
    }

    /* check if this shm has been created OK */
    if (!rstShm.GetAttache(c_rstaShmCfg.at(uiShmIdx).eShmKey + uiDevIdx * SHM_TYPE_MAX)) {
        /* LOG_WRITE(EN_LOG_ERROR, "shm GetAttache error"); */
        return false;
    }

    if (!rstSem.Get(c_rstaShmCfg.at(uiShmIdx).eSemKey + uiDevIdx * SHM_TYPE_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "sem Get error");
        return false;
    }

    return true;

}

int32_t
CDevApi::GetStatus(uint32_t uiDevIdx, const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg)
{

    CShareMemory stShmSta;
    CSemaphore stSemSta;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_STA, c_rstaShmCfg, stShmSta, stSemSta)) {
        /* LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx); */
        return ERR_SHM_NOT_ACCESS;
    }

    /* get status from shm */
    CShmDevSta stSta;
    CShmOperator::Get(stShmSta, stSemSta, (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    if (stSta.iInitSta != SHM_STATUS_OK) {
        /* LOG_WRITE(EN_LOG_ERROR, "stSta.iInitSta = %d", stSta.iInitSta); */
        return ERR_OPERATION_FAILED;
    }

    if (SHM_STATUS_BUSY == stSta.iUpgdSta) {
        return ERR_UPGRADING;
    }

    if (SHM_STATUS_BUSY == stSta.iLoadSta) {
        return ERR_LOADING;
    }

    return OPLK_OK;

}

bool
CDevApi::SetCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem,
                uint32_t uiOid, uint8_t *pucData, uint32_t uiLen, int32_t iSyncFlag, int32_t iPriority)
{

    CQueueNode stNode;
    stNode.uiOid = uiOid;
    stNode.uiPri = iPriority;
    stNode.iSyncFlag = iSyncFlag;
    stNode.uiLen = uiLen;
    memcpy(stNode.aucBuf, pucData, uiLen);

    /* set cmd to shm */
    if (SYNC == iSyncFlag) {
        return CShmOperator::SetSyncCmd(c_rstShm, c_rstSem, stNode,
                                        nullptr, TIMEOUT_PERIOD, TIMEOUT_CNT);
    }

    return CShmOperator::SetAsyncCmd(c_rstShm, c_rstSem, stNode);

}


