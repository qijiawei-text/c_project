/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DceCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DceCmd.h"
#include "DevCmd.h"


/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CDceDriver> > CDceCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CDceCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CDceCmd::ms_staSem;

/*==============================function=========================*/



