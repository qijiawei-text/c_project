/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Thread.h
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
#include <pthread.h>

/*************************************************/

class CThread
{
public:
    CThread()
    {
    }
    virtual
    ~CThread()
    {
    }

    void Start(void);
    virtual void Run(void) = 0;
    void Join(void **ppvPtr = nullptr);
    void Exit(void *pvPtr);

private:
    pthread_t m_iThId = 0;

    static void *Entry(void *pvArg);
};




