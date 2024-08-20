#ifndef __OPS_COMMON_H__
#define __OPS_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <math.h>

#include "util_log.h"
#include "DSC_inc.h"

#ifdef __cplusplus
extern "C" {
#endif

int OPS_GetNames(char (**names)[DEF_BUF_SIZE_64], size_t * count);

uint32_t OPS_GetNumber(uint32_t uiDevType, uint32_t uiPortId);

int OPS_CreateUDPServer(uint16_t usPort);

void OPS_SetRecvfromNonblocking(int iSockFd);

void OPS_SetRunningThreadName(const char *pcThreadName);

#ifdef __cplusplus
}
#endif
#endif
