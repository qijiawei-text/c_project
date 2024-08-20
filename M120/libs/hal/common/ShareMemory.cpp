/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ShareMemory.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "ShareMemory.h"

std::map<int32_t, void *> CShareMemory::ms_staAttaches;


/*==============================function=========================*/

bool
CShareMemory::Create(uint32_t uiKey, uint32_t uiSize)
{

    m_uiKey = uiKey;

    m_iShmId = shmget(m_uiKey, uiSize, IPC_CREAT | 0666);
    if (m_iShmId < 0) {
        return false;
    }

    m_uiSize = uiSize;

    return true;

}

bool
CShareMemory::Get(uint32_t uiKey)
{

    m_uiKey = uiKey;

    m_iShmId = shmget(m_uiKey, 0, IPC_CREAT | 0666);
    if (m_iShmId < 0) {
        /* LOG_WRITE(EN_LOG_ERROR, "shmget error = %d", m_iShmId); */
        return false;
    }

    struct shmid_ds stBuf;
    if (shmctl(m_iShmId, IPC_STAT, &stBuf) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "shmctl error = %d", m_iShmId);
        return false;
    }

    m_uiSize = stBuf.shm_segsz;

    return true;

}

bool
CShareMemory::Clear(void)
{

    if ((void *)(-1) == m_pvAddr) {
        return false;
    }

    memset(m_pvAddr, 0, m_uiSize);

    return true;

}

bool
CShareMemory::Attache(void)
{

    if (m_iShmId < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Attache = %d", m_iShmId);
        return false;
    }


    /* find Attache flag in map */
    auto itIter = ms_staAttaches.find(m_iShmId);

    if (ms_staAttaches.end() == itIter) {
        m_pvAddr = shmat(m_iShmId, nullptr, 0);

        if ((void *)(-1) == m_pvAddr) {
            LOG_WRITE(EN_LOG_ERROR, "shmat = %d", m_iShmId);
            return false;
        }

        ms_staAttaches[m_iShmId] = m_pvAddr;
    } else {
        m_pvAddr = itIter->second;
    }

    return true;

}

bool
CShareMemory::CreateAttache(uint32_t uiKey, uint32_t uiSize)
{

    if (Create(uiKey, uiSize)) {
        if (Attache()) {
            Clear();
            return true;
        }
    }

    return false;

}

bool
CShareMemory::GetAttache(uint32_t uiKey)
{

    if (Get(uiKey)) {
        if (Attache()) {
            return true;
        }
    }

    /* LOG_WRITE(EN_LOG_ERROR, "GetAttache = %d", uiKey); */
    return false;

}

bool
CShareMemory::Delete(void)
{

    if (m_iShmId < 0) {
        return false;
    }

    if (shmctl(m_iShmId, IPC_RMID, nullptr) < 0) {
        return false;
    }

    m_iShmId = -1;

    return true;

}

bool
CShareMemory::Disconnect(void)
{

    if ((void *)(-1) == m_pvAddr) {
        return false;
    }

    if (shmdt(m_pvAddr) < 0) {
        return false;
    }

    m_pvAddr = (void *)(-1);

    return true;

}

bool
CShareMemory::DtDelete(void)
{

    if (Disconnect()) {
        if (Delete()) {
            return true;
        }
    }

    return false;

}



