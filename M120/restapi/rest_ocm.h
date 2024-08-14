#ifndef __REST_OCM_H__
#define __REST_OCM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REST_MAX_OSA_CHANNEL_NUM      (2048)
typedef struct REST_OSA_DATA_st {
    uint32_t uiTotal;
    double adPwr[REST_MAX_OSA_CHANNEL_NUM];
} REST_OSA_DATA_ST;

int REST_GetOneChannelState(char *pcPortName, char *pcLowerFreq, char *pcUpperFreq, OCM_CHANNEL_ST *pstChannel);

int REST_GetPortDescObjJson(const OCM_PORT_DESC_ST *pstPortDesc, cJSON *pstObjJson);
int REST_GetChannelArrayJson(const OCM_CHANNELS_LIST_ST *pstList, cJSON *pstArrayJson);
int REST_GetOnePortObjJson(const OCM_PORT_DESC_ST *pstPortDesc, const OCM_CHANNELS_LIST_ST *pstList, cJSON *pstObjJson);

int REST_ParseOcmPortConfig(const cJSON *pstJson, const char *pcPortName, OCM_PORT_DESC_ST *pstOcmPortDesc);

int REST_GetOcmOsaData(char *pcPortName, REST_OSA_DATA_ST *pstOsaData);
int REST_GetOcmModeCfg(OCM_MODE_CONFIG_ST *pstModeCfg);
int REST_ParseOcmModeCfg(const cJSON *pstJson, OCM_MODE_CONFIG_ST *pstModeCfg);

#ifdef __cplusplus
}
#endif

#endif


