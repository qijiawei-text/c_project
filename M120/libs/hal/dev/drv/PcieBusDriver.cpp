/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      PcieBusDriver.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:

*****************************************************************************************************/

/*=========================include head files====================*/

#include "PcieBusDriver.h"
#include <fcntl.h>
/* static int32_t m_iFd = -1; */
/* static pthread_mutex_t m_stMutexLock; */
/*==============================function=========================*/

bool
CPcieBusDriver::CreateVirtualAddr(uint32_t uiCsIdx)
{

    if (m_iFd > 0) {
        return true;
    }

    m_iFd = open("/dev/xilinx", O_RDWR);

    if (m_iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "open /dev/xilinx error.");
        return false;
    }
    pthread_mutex_init(&m_stMutexLock, NULL);
    return true;
}

bool
CPcieBusDriver::DestroyVirtualAddr(uint32_t uiCsIdx)
{

    if (m_iFd) {
        close(m_iFd);
        m_iFd = -1;
    }
    pthread_mutex_destroy(&m_stMutexLock);

    return true;
}

uint8_t
CPcieBusDriver::Read(uint32_t uiAddr)
{

    /* LOG_WRITE(EN_LOG_NOTICE, "Read uiAddr = %d", uiAddr); */

    uint8_t pucData = 0;
    pthread_mutex_lock(&m_stMutexLock);
    if (m_iFd > 0) {
        if (uiAddr != lseek(m_iFd, uiAddr, SEEK_SET)) {
            printf("lseek error\n");
            goto ERR_LABEL;
        }

        if (1 != read(m_iFd, &pucData, 1)) {
            printf("read error,0x%x\n", uiAddr);
            goto ERR_LABEL;
        }
        pthread_mutex_unlock(&m_stMutexLock);
        return pucData;

    }
ERR_LABEL:
    pthread_mutex_unlock(&m_stMutexLock);
    /* printf("CPcieBusDriver read error,m_iFd:%d", m_iFd); */
    return pucData;
}


void
CPcieBusDriver::Write(uint32_t uiAddr, uint8_t ucData)
{

    /*LOG_WRITE(EN_LOG_NOTICE, "Write uiAddr = 0x%0x, ucData = 0x%0x", uiAddr, ucData); */
    pthread_mutex_lock(&m_stMutexLock);
    if (m_iFd > 0) {
        if (uiAddr != lseek(m_iFd, uiAddr, SEEK_SET)) {
            printf("lseek error\n");
            goto ERR_LABEL;
        }

        if (1 != write(m_iFd, &ucData, 1)) {
            printf("write error,0x%x,data:0x%x\n", uiAddr, ucData);
            goto ERR_LABEL;
        }
    }
    pthread_mutex_unlock(&m_stMutexLock);
    return;
ERR_LABEL:
    pthread_mutex_unlock(&m_stMutexLock);
    printf("CPcieBusDriver write error,m_iFd:%d\n", m_iFd);

}


bool
CPcieBusDriver::Read(uint32_t uiAddr, uint32_t uiLen, uint8_t *pucBuf)
{
    pthread_mutex_lock(&m_stMutexLock);
    if (m_iFd > 0) {
        if (uiAddr != lseek(m_iFd, uiAddr, SEEK_SET)) {
            goto ERR_LABEL;
        }

        if (0 > read(m_iFd, pucBuf, uiLen)) {
            goto ERR_LABEL;
        }
        pthread_mutex_unlock(&m_stMutexLock);
        return true;

    }
ERR_LABEL:
    pthread_mutex_unlock(&m_stMutexLock);
    return false;
}

bool
CPcieBusDriver::Write(uint32_t uiAddr, uint32_t uiLen, uint8_t *pucBuf)
{
    pthread_mutex_lock(&m_stMutexLock);
    if (m_iFd > 0) {
        if (uiAddr != lseek(m_iFd, uiAddr, SEEK_SET)) {
            goto ERR_LABEL;
        }

        if (0 > write(m_iFd, pucBuf, uiLen)) {
            goto ERR_LABEL;
        }
    }
    pthread_mutex_unlock(&m_stMutexLock);
    return true;
ERR_LABEL:
    pthread_mutex_unlock(&m_stMutexLock);
    return false;

}

