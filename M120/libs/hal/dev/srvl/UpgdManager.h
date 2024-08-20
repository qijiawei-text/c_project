/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      UpgdManager.h
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

/*************************************************/

enum EUpgdStatus
{
    UPGD_SUCCESS = 0,
    UPGD_BUSY,
    UPGD_FAILED
};

enum EUpgdLoadStatus
{
    UPGD_LOAD_SUCCESS = 0,
    UPGD_LOAD_BUSY,
    UPGD_LOAD_FAILED
};


class CUpgdManager
{
public:
    static CUpgdManager &GetInstance(void);

    uint32_t
    GetState(void) const
    {
        return m_uiState;
    }
    uint32_t
    GetLoadState(void) const
    {
        return m_uiLoadSta;
    }

    void
    SetState(uint32_t uiState)
    {
        m_uiState = uiState;
    }

private:
    CUpgdManager()
    {
    }
    CUpgdManager(const CUpgdManager &) = delete;
    CUpgdManager &operator=(const CUpgdManager &) = delete;

    uint32_t m_uiState = UPGD_SUCCESS;
    uint32_t m_uiLoadSta = UPGD_LOAD_SUCCESS;
    uint32_t m_uiPercent = 0;
};








