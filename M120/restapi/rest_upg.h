#ifndef __REST_UPG_H__
#define __REST_UPG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct REST_SOFTWARE_LOAD_st
{
    /* char acEntity[16]; */
    /* char acFileName[128]; */
    char acUserId[64];
    char acPassword[64];
    char acSrcFileName[256];
} REST_SOFTWARE_LOAD_ST;

int REST_ParseSoftwareLoad(const cJSON *pstJson, REST_SOFTWARE_LOAD_ST *pstSoftwareLoad);
int REST_SoftwareDownload(REST_SOFTWARE_LOAD_ST *pstSoftwareLoad, bool bIsOneKeyUpgd);
int REST_SoftwareActivate();

#ifdef __cplusplus
}
#endif

#endif

