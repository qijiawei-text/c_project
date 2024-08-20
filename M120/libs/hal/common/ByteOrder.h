/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Delay.h
   Author:        Zhu Weian
   Date:          2019-07-31
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-31] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include <unistd.h>

/*************************************************/

class CByteOrder
{
public:
    CByteOrder()
    {
    }
    ~CByteOrder()
    {
    }

    static void
    ArrayNtohs(int16_t *psData, uint32_t uiLen)
    {
        for (uint32_t i = 0; i < uiLen; i++) {
            psData[i] = (int16_t)ntohs(*(psData + i));
        }
    }

    static void
    ArrayNtohl(int32_t *psData, uint32_t uiLen)
    {
        for (uint32_t i = 0; i < uiLen; i++) {
            psData[i] = (int32_t)ntohs(*(psData + i));
        }
    }

    static bool
    CheckSumOk(int16_t *psData, uint32_t uiLen, int16_t sCheckSum)
    {
        int16_t __sCheckSum = 0;
        for (uint32_t i = 0; i < (uiLen - 1); i++) {
            __sCheckSum += (int16_t)ntohs(*(psData + i));
        }

        if (__sCheckSum != sCheckSum) {
            LOG_WRITE(EN_LOG_DEBUG, "checksum error. __sCheckSum 0x%x, 0x%x", __sCheckSum, sCheckSum);
            return false;
        }

        return true;
    }

    static bool
    CheckSumOkN(int16_t *psData, uint32_t uiLen, int16_t sCheckSum)
    {
        int16_t __sCheckSum = 0;
        for (uint32_t i = 0; i < (uiLen - 1); i++) {
            __sCheckSum += *(psData + i);
        }

        if (__sCheckSum != sCheckSum) {
            LOG_WRITE(EN_LOG_DEBUG, "checksum error. __sCheckSum 0x%x, 0x%x", __sCheckSum, sCheckSum);
            return false;
        }

        return true;
    }
};

