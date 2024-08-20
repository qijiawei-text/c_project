#ifndef __REST_API_OCS_H__
#define __REST_API_OCS_H__
#ifdef __cplusplus
extern "C" {
#endif

void API_OcsAddConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_OcsUpdateConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_OcsReplaceConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_OcsDeleteAllConnections(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_OcsDeleteConnection(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_OcsSetOneConnection(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_OcsSetOneConnectionConfig(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif

