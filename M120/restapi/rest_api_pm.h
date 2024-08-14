#ifndef __REST_API_PM_H__
#define __REST_API_PM_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_GetCurrentPmValues(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetPastPmValues(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetCurrentPm15min(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetCurrentPm24hour(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif


#endif
