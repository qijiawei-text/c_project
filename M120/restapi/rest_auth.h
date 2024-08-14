#ifndef __REST_AUTH_H__
#define __REST_AUTH_H__
#ifdef __cplusplus
extern "C" {
#endif

int REST_ParseLoginUser(const cJSON *pstJson, USER_ST *pstUser);
int REST_AuthLogin(LOGIN_REQ_ST *pstLoginReq, LOGIN_RESP_ST *pstLoginResp);
int REST_AuthLogout(const char *pcToken);

int REST_ParseUser(const cJSON *pstJson, USER_ST *pstUser);

int REST_AuthGetLoginTokenRecord(const char *pcToken);
TOKEN_ST *REST_GetCurTokenRecord();

void REST_ClearCurTokenRecord();
void REST_SetUsernameToCurToken(char *pcUsername);

int REST_Auth(AUTH_ST *pstAuth);

int REST_DeleteTokensByUsername(char *pcUserName);

#ifdef __cplusplus
}
#endif

#endif
