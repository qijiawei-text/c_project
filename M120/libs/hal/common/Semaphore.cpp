/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Semaphore.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Semaphore.h"

/*==============================function=========================*/

bool
CSemaphore::Create(uint32_t uiKey)
{

    m_uiKey = uiKey;

    m_iSemId = semget(m_uiKey, 1, IPC_CREAT | 0666);
    if (m_iSemId < 0) {
        return false;
    }

    if (!SetValue(m_iSemId, 1)) {
        return false;
    }

    return true;

}

bool
CSemaphore::SetValue(int32_t iSemId, int32_t iVal)
{

    union semun uSemArgs;

    uSemArgs.setval = iVal;

    if (semctl(iSemId, 0, SETVAL, uSemArgs) < 0) {
        return false;
    }

    return true;

}

int32_t
CSemaphore::GetValue(int32_t iSemId) const
{

    union semun uSemArgs;

    return semctl(iSemId, 0, GETVAL, uSemArgs);

}

bool
CSemaphore::Get(uint32_t uiKey)
{

    m_uiKey = uiKey;

    m_iSemId = semget(m_uiKey, 0, IPC_CREAT | 0666);
    if (m_iSemId < 0) {
        return false;
    }

    return true;

}

bool
CSemaphore::P(void) const
{

    if (m_iSemId < 0) {
        return false;
    }

    struct sembuf stSembuf;

    stSembuf.sem_num = 0;
    stSembuf.sem_op = -1;
    stSembuf.sem_flg = SEM_UNDO;

    if (semop(m_iSemId, &stSembuf, 1) < 0) {
        return false;
    }

    return true;

}

bool
CSemaphore::V(void) const
{

    if (m_iSemId < 0) {
        return false;
    }

    int32_t iValue = GetValue(m_iSemId);

    if (iValue < 0) {
        return false;
    } else if (0 == iValue) {
        struct sembuf stSembuf;

        stSembuf.sem_num = 0;
        stSembuf.sem_op = 1;
        stSembuf.sem_flg = SEM_UNDO;

        if (semop(m_iSemId, &stSembuf, 1) < 0) {
            return false;
        }
    }

    return true;

}

bool
CSemaphore::Delete(void)
{

    if (m_iSemId < 0) {
        return false;
    }

    if (semctl(m_iSemId, 0, IPC_RMID) < 0) {
        return false;
    }

    m_iSemId = -1;

    return true;

}


