/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DgeCmd.h"
#include "DevCmd.h"
#include "DevData.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CDgeDriver> > CDgeCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CDgeCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CDgeCmd::ms_staSem;


/*==============================function=========================*/






