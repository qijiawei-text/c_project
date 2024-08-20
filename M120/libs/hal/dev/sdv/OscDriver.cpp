/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OscDriver.h"


/*==============================function=========================*/

bool
CLogicOscDriver::IsOnline(void)
{

    return true;
}

bool
CLogicOscDriver::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetMfg");
    strcpy(rstMfg.acManufacturer, "Molex");

    return true;
}

bool
CLogicOscDriver::GetStatusData(COscStatusData &rstData)
{
    LOG_WRITE(EN_LOG_DEBUG, "CLogicOscDriver::GetStatusData +++");

    if (nullptr == m_stpOscBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpOscBoardPtr is nullptr.");
        return false;
    }

    rstData.bOffLine = !m_stpOscBoardPtr->IsOnline(m_uiDevIdx);

    bool bCurrRxLos = m_stpOscBoardPtr->IsRxLos(m_uiDevIdx);
    if (m_bLastRxLos && bCurrRxLos) {
        rstData.bRxLos = true;
    } else {
        rstData.bRxLos = false;
    }
    m_bLastRxLos = bCurrRxLos;

    bool bCurrLinkDown = !m_stpOscBoardPtr->IsLinkup(m_uiDevIdx);
    if (m_bLastLinkdown && bCurrLinkDown) {
        rstData.bLinkDown = true;
    } else {
        rstData.bLinkDown = false;
    }
    m_bLastLinkdown = bCurrLinkDown;

    rstData.bTxLos = !m_stpOscBoardPtr->IsEnable(m_uiDevIdx);

    LOG_WRITE(EN_LOG_DEBUG, "m_uiDevIdx = %d", m_uiDevIdx);
    LOG_WRITE(EN_LOG_DEBUG, "bOffLine = %d", rstData.bOffLine);
    LOG_WRITE(EN_LOG_DEBUG, "bRxLos = %d", rstData.bRxLos);
    LOG_WRITE(EN_LOG_DEBUG, "bLinkDown = %d", rstData.bLinkDown);
    LOG_WRITE(EN_LOG_DEBUG, "bTxLos = %d", rstData.bTxLos);

    LOG_WRITE(EN_LOG_DEBUG, "CLogicOscDriver::GetStatusData ---");

    return true;
}

bool
CLogicOscDriver::SetLaserEnable(bool bEnable)
{

    LOG_WRITE(EN_LOG_NOTICE, "SetLaserEnable +++");

    if (nullptr == m_stpOscBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpLogicPtr");
        return false;
    }


    if (false == m_stpOscBoardPtr->SetEnable(m_uiDevIdx, bEnable)) {
        LOG_WRITE(EN_LOG_ERROR, "Set osc output enanble error. m_uiDevIdx = %d, bEnable = %d", m_uiDevIdx, bEnable);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "m_uiDevIdx = %d, bEnable = %d", m_uiDevIdx, bEnable);

    LOG_WRITE(EN_LOG_NOTICE, "SetLaserEnable ---");

    return true;
}



/*==============================function=========================*/

bool
CI2cOscDriver::IsOnline(void)
{

    return true;
}

bool
CI2cOscDriver::GetMfg(CMfg &rstMfg)
{

    strcpy(rstMfg.acManufacturer, "oplink");

    return true;
}

bool
CI2cOscDriver::GetStatusData(COscStatusData &rstData)
{

    return true;
}


bool
CI2cOscDriver::SetLaserEnable(bool bEnable)
{

    return true;
}




