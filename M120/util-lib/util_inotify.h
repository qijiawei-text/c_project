/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : util_inotify.h
   Description : Linux monitor file.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/06/18   V1.0.0       Initial file.
**********************************************************************************/
#ifndef __UTIL_INOTIFY_H__
#define __UTIL_INOTIFY_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*InotifyCb)(char *pcFile);
typedef struct {
    InotifyCb fpInotifyCb;
    int iFd;
    char acFile[DEF_BUF_SIZE_128];
} ST_INOTIFY_ARG;

extern int UTIL_RegInotifyCb(char *pcFile, InotifyCb fpInotifyCb);
extern void UTIL_UnRegInotifyCb(void);

#ifdef __cplusplus
}
#endif

#endif

