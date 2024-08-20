/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DceDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DceDriver.h"

/*==============================function=========================*/

bool
CDceUartDriver::IsOnline(void)
{

    return true;
}

bool
CDceUartDriver::GetMfg(CMfg &rstMfg)
{

    strcpy(rstMfg.acManufacturer, "oplink");

    return true;
}

bool
CDceUartDriver::GetStatusData(CDceStatusData &rstData)
{

    return true;
}

bool
CDceUartDriver::GetCfgData(CDceCfgData &rstData)
{

    return true;
}


