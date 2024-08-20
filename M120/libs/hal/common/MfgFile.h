/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MfgFile.h
   Author:        Zhu Weian
   Date:          2019-08-09
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-08-09] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include <unistd.h>

#if 0


#define LEN_FILE_DESCR      (64)
typedef struct FILE_INFO_st
{
    uint32_t uiAddr;
    uint32_t uiLen;
    char acDescr[LEN_FILE_DESCR];
}FILE_INFO_ST;

#define LEN_HEADER          (2 * 1024)
#define LEN_HEADER_KEYWORD  (16)
#define LEN_HEADER_VERSION  (16)
#define MAX_FILE_COUNT      (20)
#define LEN_HEADER_RES      (LEN_HEADER - LEN_HEADER_KEYWORD \
                             - LEN_HEADER_VERSION \
                             - 4 * sizeof(uint32_t)        \
                             - MAX_FILE_COUNT * sizeof(FILE_INFO_ST))


typedef struct CONF_FILE_HEADER_st
{
    char acKeyWord[LEN_HEADER_KEYWORD];
    char acVersion[LEN_HEADER_VERSION];
    uint32_t uiFileLen;                                   /* File total len.                          */
    uint32_t uiFileCount;
    uint32_t uiType;
    FILE_INFO_ST astInfo[MAX_FILE_COUNT];
    char acResv[LEN_HEADER_RES];
    uint32_t uiHeaderCrc;                               /* Header crc32.                            */
}CONF_FILE_HEADER_ST;
#endif

class CMfgFile
{
public:
    CMfgFile()
    {
    }
    ~CMfgFile()
    {
    }

    bool ReadMfg(const char *c_pcFilePath, CMfg &stMfg);
    bool ReadCali(const char *c_pcFilePath, CCali &stCali);

private:
    bool FindKeyWordValue(FILE *fpFile, const char *pacInKeyWord, char *pacOutValue);
    bool FindKeyWordShortValue(FILE *fpFile, const char *pacInKeyWord, short *psOutValue);
    bool CheckMacAddr(const char *pcMac);

};

