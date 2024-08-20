/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Thread.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Thread.h"

/*==============================function=========================*/

void *
CThread::Entry(void *pvArg)
{

    CThread *pstThread = static_cast<CThread *>(pvArg);

    pstThread->Run();

    return pvArg;

}

void
CThread::Start(void)
{

    pthread_create(&m_iThId, nullptr, Entry, this);

}

void
CThread::Join(void **ppvPtr)
{

    if (m_iThId > 0) {
        pthread_join(m_iThId, ppvPtr);
    }

}

void
CThread::Exit(void *pvPtr)
{

    if (m_iThId > 0) {
        pthread_exit(pvPtr);
    }

}


