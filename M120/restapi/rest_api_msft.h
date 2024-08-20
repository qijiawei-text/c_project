#ifndef __REST_API_MSFT_H__
#define __REST_API_MSFT_H__


#ifdef __cplusplus
extern "C" {
#endif

void API_GetFullConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SetSystem(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SetFullConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetSystemOptional(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetInventory(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_Restart(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetAllSlotsStatus(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_TransferFile(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_TransferLog(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_LampTest(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetApsd(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetApsd(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetIpacl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetIpacl(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_SetMuxConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetMuxConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetMuxState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_ClearAutoAlarms(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetOtdrSorFiles(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetOcmOsaData(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_SetOcmModeCfg(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_GetOcmModeState(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_GetFiberCfgByType(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif
