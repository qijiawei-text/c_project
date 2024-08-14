#ifndef __REST_API_EDFA_H__
#define __REST_API_EDFA_H__
#ifdef __cplusplus
extern "C" {
#endif

void API_EdfaSetPA(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_EdfaSetBA(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_EdfaSetPaOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_EdfaSetBaOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif

