/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Semaphore.h
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
#include <sys/sem.h>

/*************************************************/

union semun
{
    int32_t setval;
    struct semid_ds *buf;
    uint16_t *array;
};


class CSemaphore
{
public:
    CSemaphore()
    {
    }
    ~CSemaphore()
    {
    }

    bool Create(uint32_t uiKey);
    bool Get(uint32_t uiKey);
    bool P(void) const;
    bool V(void) const;
    bool Delete(void);

    int32_t
    GetId(void) const
    {
        return m_iSemId;
    }

private:
    uint32_t m_uiKey;
    int32_t m_iSemId = -1;

    bool SetValue(int32_t iSemId, int32_t iVal);
    int32_t  GetValue(int32_t iSemId) const;
};





