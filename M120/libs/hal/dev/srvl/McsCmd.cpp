/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      McsCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "McsCmd.h"
#include "DevCmd.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CMcsDriver> > CMcsCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CMcsCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CMcsCmd::ms_staSem;


/*==============================function=========================*/

