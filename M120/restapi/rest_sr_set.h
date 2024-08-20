#ifndef __REST_SR_SET_H__
#define __REST_SR_SET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "rest_sr_util.h"

int REST_Set(char *pcUrl, cJSON *pstBody);
int REST_Rpc(char *pcUrl, const cJSON *pstInputJson, cJSON **ppstOutputJson);
int REST_Delete(char *pcUrl);

#ifdef __cplusplus
}
#endif
#endif

