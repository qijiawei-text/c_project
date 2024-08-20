/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      StringTool.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-15] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "StringTool.h"

/*==============================function=========================*/

bool
CStringTool::SubStr(char *pcStr, const std::string &c_rstaStr, const std::string &c_rstaStartStr,
                    const std::string &c_rstaEndStr, uint32_t uiLen)
{

    auto uiStartPos = c_rstaStr.find(c_rstaStartStr);

    if (std::string::npos == uiStartPos) {
        return false;
    }

    auto uiEndPos = c_rstaStr.find(c_rstaEndStr, uiStartPos);

    if (std::string::npos == uiEndPos) {
        return false;
    }

    if (uiEndPos - uiStartPos > uiLen) {
        return false;
    }

    std::string staSubStr = c_rstaStr.substr(uiStartPos + c_rstaStartStr.size(), uiEndPos - uiStartPos - c_rstaStartStr.size());

    /* skip ' ' */
    int i = 0;
    char *pcBuf = (char *)staSubStr.c_str();
    while (' ' == pcBuf[i]) {
        i++;
    }

    sprintf(pcStr, "%s", pcBuf + i);

    return true;

}


