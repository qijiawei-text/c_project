/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ShareMemory.h
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
#include <sys/shm.h>

/*************************************************/

class CShareMemory
{
public:
    CShareMemory()
    {
    }
    ~CShareMemory()
    {
    }

    bool Create(uint32_t uiKey, uint32_t uiSize);
    bool Attache(void);
    bool CreateAttache(uint32_t uiKey, uint32_t uiSize);
    bool Get(uint32_t uiKey);
    bool GetAttache(uint32_t uiKey);
    bool Clear(void);
    bool Delete(void);
    bool Disconnect(void);
    bool DtDelete(void);

    uint32_t
    GetSize(void) const
    {
        return m_uiSize;
    }
    int32_t
    GetId(void) const
    {
        return m_iShmId;
    }
    void *
    GetAddr(void) const
    {
        return m_pvAddr;
    }


private:
    uint32_t m_uiKey;
    uint32_t m_uiSize = 0;
    int32_t m_iShmId = -1;
    void *m_pvAddr = (void *)(-1);
    static std::map<int32_t, void *> ms_staAttaches;
};





