#ifndef __REST_API_GENERIC_H__
#define __REST_API_GENERIC_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_sr_get(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_sr_set(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_sr_delete(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_sr_rpc(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif
