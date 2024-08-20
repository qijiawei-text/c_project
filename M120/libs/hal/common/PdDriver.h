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
typedef struct {
    float fPdA;
    float fPdB;
    float fPdC;
}CPdKcStage;


typedef struct {
    CPdKcStage astPdStage[3];
    float afRsvBuf[3];
}CPdKcLevel;


class CPdDriver
{
public:
    CPdDriver()
    {
    }
    ~CPdDriver()
    {
    }

    static void
    CalcKC(CPdKcLevel &stPdKc, int32_t iTemp, float &fK, float &fC)
    {
        float fCurrTemp = 0.0;

        /*current temprature*/
        fCurrTemp = (float)iTemp / 10;

        fK = stPdKc.astPdStage[0].fPdA * fCurrTemp * fCurrTemp
             + stPdKc.astPdStage[0].fPdB * fCurrTemp
             + stPdKc.astPdStage[0].fPdC;

        LOG_WRITE(EN_LOG_DEBUG, " fPdK = %f, c0 = %f", fK, stPdKc.astPdStage[0].fPdC);

        fC = stPdKc.astPdStage[1].fPdA * fCurrTemp * fCurrTemp
             + stPdKc.astPdStage[1].fPdB * fCurrTemp
             + stPdKc.astPdStage[1].fPdC;

        LOG_WRITE(EN_LOG_DEBUG, "fPdC = %f, c1 = %f", fC, stPdKc.astPdStage[1].fPdC);

    }
};

