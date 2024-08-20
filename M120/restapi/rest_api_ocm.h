#ifndef __REST_API_OCM_H__
#define __REST_API_OCM_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_GetOcmAllData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetOcmOnePortData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SetOcmOnePortData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetOcmOsaData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetOcmModeState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif

