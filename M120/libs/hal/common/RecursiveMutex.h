/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicI2c.h
   Author:        Zhu Weian
   Date:          2020-08-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-15] [1.0] [Creator]


*****************************************************************************************************/
#pragma once


class CRecursiveMutex
{


public:
    CRecursiveMutex()
    {
    }

    ~CRecursiveMutex()
    {

    }

#if 0
    void
    lock()
    {

        m_stMutex.lock();
        pthread_t iThId = pthread_self();
        m_staNumOfLocks[iThId]++;
        LOG_WRITE(EN_LOG_DEBUG, "Lock ++ m_uiNumOfLocks = %d pThId = %d", m_staNumOfLocks[iThId], iThId);
    }

    void
    unlock()
    {

        pthread_t iThId = pthread_self();
        while (m_staNumOfLocks[iThId] > 0) {
            m_staNumOfLocks[iThId]--;
            LOG_WRITE(EN_LOG_DEBUG, "UnLock -- m_uiNumOfLocks = %d pThId = %d", m_staNumOfLocks[iThId], iThId);
            m_stMutex.unlock();
        }
    }
#endif

    void
    lock()
    {
        m_stMutex.lock();

        std::lock_guard<std::mutex> lockGuard(m_stAtomicMutex);

        m_uiNumOfLocks++;
        LOG_WRITE(EN_LOG_DEBUG, "Lock ++ m_uiNumOfLocks = %u pThId = %lu", m_uiNumOfLocks, pthread_self());
    }

    void
    unlock()
    {
        while (m_uiNumOfLocks > 0) {

            m_uiNumOfLocks--;

            std::lock_guard<std::mutex> lockGuard(m_stAtomicMutex);

            m_stMutex.unlock();

            LOG_WRITE(EN_LOG_DEBUG, "unLock ++ m_uiNumOfLocks = %u pThId = %lu", m_uiNumOfLocks, pthread_self());

            if (0 == m_uiNumOfLocks) {
                break;
            }
        }
    }


private:

    /* std::map<pthread_t, uint32_t> m_staNumOfLocks; */
    std::recursive_mutex m_stMutex;
    std::mutex m_stAtomicMutex;
    uint32_t m_uiNumOfLocks = 0;
};

