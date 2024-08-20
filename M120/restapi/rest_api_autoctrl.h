#ifndef __REST_API_AUTOCTRL_H__
#define __REST_API_AUTOCTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_StartAutoGainCtrl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetAutoGainCtrlState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_StartCdControl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetCdControlState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetAutoConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetAutoConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetRemoteNodeInfo(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetSpanlossThreshold(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetSpanlossThreshold(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetFiberCfgByType(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif

