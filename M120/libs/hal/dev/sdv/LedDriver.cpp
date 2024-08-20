/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LedDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "LedDriver.h"

/*==============================function=========================*/
#if 1

bool
CLed::SetState(uint32_t uiState)
{
    if (nullptr == m_stpLedBoardPtr) {
        return false;
    }


    if (false == m_stpLedBoardPtr->SetLedState(m_uiHwSubIdx, uiState)) {
        return false;
    }


    return true;
}

bool
CLed::GetState(uint32_t &ruiState)
{
    if (nullptr == m_stpLedBoardPtr) {
        return false;
    }


    if (false == m_stpLedBoardPtr->GetLedState(m_uiHwSubIdx, ruiState)) {
        return false;
    }


    return true;
}

bool
CLed::SetTestMode(bool bTestMode)
{
    if (nullptr == m_stpLedBoardPtr) {
        return false;
    }


    if (false == m_stpLedBoardPtr->SetLedTestMode(m_uiHwSubIdx, bTestMode)) {
        return false;
    }


    return true;
}

bool
CLed::SetLocationTestMode(bool bTestMode)
{
    if (nullptr == m_stpLedBoardPtr) {
        return false;
    }


    if (false == m_stpLedBoardPtr->SetLedLocationTestMode(m_uiHwSubIdx, bTestMode)) {
        return false;
    }


    return true;
}
#endif


#if 0
bool
CLedCpldDriver::SetState(uint32_t uiState)
{

    if (nullptr == m_stpCpldDrvPtr) {
        return false;
    }

    return true;

}



/*==============================function=========================*/
bool
CLedFpgaDriver::SetState(uint32_t uiState)
{
    if (nullptr == m_stpFpgaDrvPtr) {
        return false;
    }

#if 1
    if (false == m_stpLogicDrvPtr->SetLedStatus(m_uiHwSubIdx, uiState)) {
        return false;
    }
#endif

    return true;

}

#endif

