/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevApi.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"
#include "Queue.h"

/*************************************************/

class CDevApi
{
public:
    CDevApi()
    {
    }
    virtual
    ~CDevApi()
    {
    }

    void
    SetApiMode(int32_t iMode)
    {
        m_iSyncFlag = iMode;
    }

protected:
    int32_t m_iSyncFlag = SYNC;

    bool IsShmCreated(uint32_t uiDevIdx, uint32_t uiShmIdx, const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg,
                      CShareMemory &rstShm, CSemaphore &rstSem);
    int32_t GetStatus(uint32_t uiDevIdx, const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg);

    bool SetCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem,
                uint32_t uiOid, uint8_t *pucData, uint32_t uiLen, int32_t iSyncFlag, int32_t iPriority = QUEUE_PRIO_COMMON);
};






