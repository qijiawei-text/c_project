#ifndef __REST_API_AUTH_H__
#define __REST_API_AUTH_H__

#ifdef __cplusplus
extern "C" {
#endif

void API_Login(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_Logout(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

void API_UserAddOrUpdate(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_UserGetAll(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);
void API_UserDelete(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif
