/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OscCmd.h"
#include "DevCmd.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<COscDriver> > COscCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > COscCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > COscCmd::ms_staSem;


/*==============================function=========================*/

bool
COscCmd::SetLaserEnable(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    /* find this dev in map */
    auto itIter = ms_staDrvPtrs.find(pstData->uiIdx);

    if (ms_staDrvPtrs.end() == itIter) {
        return false;
    }

    if (nullptr == itIter->second) {
        return false;
    }

    if (!itIter->second->SetLaserEnable(pstData->iVal)) {
        return false;
    }

    return true;

}





