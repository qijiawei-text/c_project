#ifndef __REST_EDFA_H__
#define __REST_EDFA_H__
#ifdef __cplusplus
extern "C" {
#endif

bool REST_IsAmpCardOnline(REST_AMP_CARD_TYPE_EN enAmpCardType);

int REST_GetAmpCardCount();
int REST_GetLinePathMax();

int REST_ParseEdfaPA(const cJSON *pstJson, REST_EDFA_PA_ST *pstEdfaPA);
int REST_SetEdfaPA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA);
int REST_GetEdfaPA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA);

int REST_ParseEdfaBA(const cJSON *pstJson, REST_EDFA_BA_ST *pstEdfaBA);
int REST_SetEdfaBA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA);
int REST_GetEdfaBA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA);

int REST_ParseEdfaPaOptional(const cJSON *pstJson, REST_EDFA_PA_ST *pstEdfaPA, REST_EDFA_FLAG_ST *pstExistFlag);
int REST_SetEdfaPaOptional(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA, REST_EDFA_FLAG_ST *pstExistFlag);

int REST_ParseEdfaBaOptional(const cJSON *pstJson, REST_EDFA_BA_ST *pstEdfaBA, REST_EDFA_FLAG_ST *pstExistFlag);
int REST_SetEdfaBaOptional(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA, REST_EDFA_FLAG_ST *pstExistFlag);

#ifdef __cplusplus
}
#endif

#endif

