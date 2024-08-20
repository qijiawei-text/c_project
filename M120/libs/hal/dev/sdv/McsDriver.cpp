/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      McsDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "McsDriver.h"

/*==============================function=========================*/

bool
CMcsFpgaDriver::IsOnline(void)
{

    return true;
}


bool
CMcsFpgaDriver::GetMfg(CMfg &rstMfg)
{

    strcpy(rstMfg.acManufacturer, "oplink");

    return true;
}

bool
CMcsFpgaDriver::GetStatusData(CMcsStatusData &rstData)
{

    return true;
}

bool
CMcsFpgaDriver::SetPortSwPos(uint32_t uiPortIdx, uint32_t uiPos)
{

    return true;
}



