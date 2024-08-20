/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      UpgdManager.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "UpgdManager.h"


/*==============================function=========================*/

CUpgdManager &
CUpgdManager::GetInstance(void)
{

    static CUpgdManager s_stUpgdManager;

    return s_stUpgdManager;

}





