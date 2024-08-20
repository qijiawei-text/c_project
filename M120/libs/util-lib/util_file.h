/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_file.h
* Description:   file operation
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/

#ifndef __UTIL_FILE_H__
#define __UTIL_FILE_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif


extern int32_t UTIL_GetFileLen(const char *pcFileName);
extern uint8_t *UTIL_MallocFromFile(char *pcFileName, uint32_t *pulFileLen);

extern int UTIL_CreatePidFile(const char *pcPidFile, int *piFd);
extern int UTIL_DelPidFile(const char *pcPidFile, int iFd);
extern int UTIL_ValidateJsonFile(char *pcFileName);
extern int UTIL_GetFileName(char *pcPath, char *pcName, size_t nLen);
extern int UTIL_CopyFile(const char *pcSrcFilePath, const char *pcDestFilePath);
extern int UTIL_DeleteFile(const char *pcFile);
extern int UTIL_CreateDir(const char *pcDir);
extern int UTIL_LockFile(const char *pcFileName, const char *pcFunc);
extern int UTIL_UnlockFile(const char *pcFileName, const char *pcFunc);

#ifdef __cplusplus
}
#endif

#endif
