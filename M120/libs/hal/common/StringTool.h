/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      StringTool.h
   Author:        Zhen Zhang
   Date:          2018-11-15
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-15] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"

/*************************************************/

class CStringTool
{
public:

    static bool SubStr(char *pcStr, const std::string &c_rstaStr, const std::string &c_rstaStartStr,
                       const std::string &c_rstaEndStr, uint32_t uiLen);

private:

};




