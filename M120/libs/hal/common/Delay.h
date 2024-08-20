/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Delay.h
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
#include <unistd.h>

/*************************************************/

enum ETimeUnit
{
    S,
    MS,
    US
};

class CDelay
{
public:
    CDelay()
    {
    }
    ~CDelay()
    {
    }

    static void
    Delay(uint32_t uiCnt, ETimeUnit eUnit)
    {
        if (S == eUnit) {
            sleep(uiCnt);
        } else if (MS == eUnit) {
            usleep(1000 * uiCnt);
        } else {
            usleep(uiCnt);
        }
    }
};




