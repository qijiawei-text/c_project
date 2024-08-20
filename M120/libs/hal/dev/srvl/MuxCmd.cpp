/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MuxCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "MuxCmd.h"
#include "DevCmd.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CMux> > CMuxCmd::ms_staDrvPtrs;
std::map<uint32_t, std::array<CShareMemory, SHM_TYPE_MAX> > CMuxCmd::ms_staShm;
std::map<uint32_t, std::array<CSemaphore, SHM_TYPE_MAX> > CMuxCmd::ms_staSem;


/*==============================function=========================*/






