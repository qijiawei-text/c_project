/*********************************************************************************************
* Copyright(C), 2018, Molex Tech.Co.,Ltd
* FileName :       util_semphore.h
* Description :    util_semphore.h
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*********************************************************************************************/

#ifndef __UTIL_SEMPHORE_H__
#define __UTIL_SEMPHORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "util_internalinc.h"

typedef sem_t SEMID;


#define WAIT_FOREVER                    (0xFFFFFFFF)


extern int UTIL_SemCreate(int32_t ulKey);
extern int UTIL_SemQuote(int32_t ulKey);
extern int UTIL_SemV(int iSemId);
extern int UTIL_SemP(int iSemId);
extern int UTIL_SemPNoWait(int iSemId);
extern int UTIL_SemClose(int iSemId);
extern int UTIL_SemGet(int32_t ulKey);

extern int UTIL_SemInit(SEMID *ptSem, uint32_t lInitNum);
extern int UTIL_SemObtain(SEMID *ptSem, uint32_t ulTimeoutMs);
extern int UTIL_SemRelease(SEMID *ptSem);
extern int UTIL_SemDestory(SEMID *ptSem);


#ifdef __cplusplus
}
#endif

#endif /* __UTIL_SEMPHORE_H__                                                   */


