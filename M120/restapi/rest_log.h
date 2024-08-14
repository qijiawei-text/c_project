#ifndef __REST_LOG_H__
#define __REST_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REST_USER_LOG(pstReq, pstResp) \
    do { \
        REST_UserLog((const REST_REQ_ST *)pstReq, (const REST_RESP_ST *)pstResp); \
    } while (0)

void REST_SetNodeName(char *pcNodeName);
void REST_EventLog(char *pcEvent, int iResult);
void REST_UserLog(const REST_REQ_ST *pstReq, const REST_RESP_ST *pstResp);

#ifdef __cplusplus
}
#endif

#endif
