#ifndef __REST_API_UPG_H__
#define __REST_API_UPG_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_SoftwareUpgrade(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SoftwareDownLoad(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SoftwareActivate(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetSoftwareState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif

