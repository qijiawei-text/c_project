#ifndef __DSC_AUTH_H
#define __DSC_AUTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define TOKEN_MAX_LEN              (256)
#define USER_NAME_MAX_LEN          (64)
#define USER_PASSWORD_MAX_LEN      (256)

/* SSL */

#define SSL_CERT_WORK_PATH                     "/usr/local/apache2/conf/"
/* #define SSL_CERT_UPLOAD_PATH                   "/tmp/" */
#define SSL_CERT_DEF_PATH                      "/usr/local/apache2/conf/"
#define SSL_CERT_SAVE_PATH                     "/home/oplink/datastore1/https-certs/"

#define SSL_CERT_FILE                          "server.crt"
#define SSL_KEY_FILE                           "server.key"

#define SSL_CERT_WORK_CERT_FILE                SSL_CERT_WORK_PATH SSL_CERT_FILE
#define SSL_CERT_WORK_KEY_FILE                 SSL_CERT_WORK_PATH SSL_KEY_FILE

/* #define SSL_CERT_UPLOAD_CERT_FILE              SSL_CERT_UPLOAD_PATH SSL_CERT_FILE */
/* #define SSL_CERT_UPLOAD_KEY_FILE               SSL_CERT_UPLOAD_PATH SSL_KEY_FILE */

#define SSL_CERT_SAVE_CERT_FILE                SSL_CERT_SAVE_PATH SSL_CERT_FILE
#define SSL_CERT_SAVE_KEY_FILE                 SSL_CERT_SAVE_PATH SSL_KEY_FILE

#define SSL_CERT_DEF_CERT_FILE                 SSL_CERT_DEF_PATH SSL_CERT_FILE
#define SSL_CERT_DEF_KEY_FILE                  SSL_CERT_DEF_PATH SSL_KEY_FILE

#define SSL_CHECK_SCRIPT                       "/usr/local/bin/checkSslCert.exp"
#define SSL_OPENSSL                            "/usr/bin/openssl"

#define SHELL_CMD_ADD_USER_GROUP               "/usr/sbin/groupadd \"%s\" >/dev/null 2>&1"
#define SHELL_CMD_FIND_USER_GROUP              "cat /etc/group | grep \"^%s:\""

/* GNMI Cert */
#define GNMI_CERT_WORK_PATH                     "/var/"
#define GNMI_CERT_DEF_PATH                      "/var/"
#define GNMI_CERT_SAVE_PATH                     "/home/oplink/datastore1/gnmi-certs/"

#define GNMI_CERT_FILE                          "gnmi-server.crt"
#define GNMI_KEY_FILE                           "gnmi-server.key"
#define GNMI_CA_FILE                            "gnmi-ca.crt"

#define GNMI_CERT_WORK_CERT_FILE                GNMI_CERT_WORK_PATH GNMI_CERT_FILE
#define GNMI_CERT_WORK_KEY_FILE                 GNMI_CERT_WORK_PATH GNMI_KEY_FILE
#define GNMI_CERT_WORK_CA_FILE                  GNMI_CERT_WORK_PATH GNMI_CA_FILE

#define GNMI_CERT_SAVE_CERT_FILE                GNMI_CERT_SAVE_PATH GNMI_CERT_FILE
#define GNMI_CERT_SAVE_KEY_FILE                 GNMI_CERT_SAVE_PATH GNMI_KEY_FILE
#define GNMI_CERT_SAVE_CA_FILE                  GNMI_CERT_SAVE_PATH GNMI_CA_FILE

#define GNMI_CERT_DEF_CERT_FILE                 GNMI_CERT_DEF_PATH GNMI_CERT_FILE
#define GNMI_CERT_DEF_KEY_FILE                  GNMI_CERT_DEF_PATH GNMI_KEY_FILE

/* log file */
#define GET_GNMI_CLIENT_CERT_FILE               "/var/gnmi-client-cert.tar.gz"


/* Application name */
typedef enum {
    WEB = 0x00000001,
    CMD = 0x00000002,
    TL1 = 0x00000004,
    SNMP = 0x00000008,
    SHELL = 0x00000010,
    REST = 0x00000020,
    NETCONF = 0x00000040
} APP_EN;

/* User role */
typedef enum {
    QUERY = 0x00000001,                 /*readonly*/
    OPERATOR = 0x00000002,
    ADMIN = 0x00000004                  /*full*/
} ACCESS_EN;

typedef enum {
    EN_LOGIN_NORMAL_AUTH = 0,                 /* username/passowrd authentication*/
    EN_LOGIN_BASIC_AUTH = 1,                  /* HTTP Basic Authentication */
    EN_LOGIN_TOKEN_AUTH = 2,                  /* Token Authentication (return token) */
    EN_LOGIN_MAX
} AUTH_LOGIN_TYPE_EN;

typedef struct LOGIN_REQUEST_st {
    char acUserName[USER_NAME_MAX_LEN];
    char acPassword[USER_PASSWORD_MAX_LEN];
    uint32_t uiApp;                        /* refer to APP_EN */
    uint32_t uiLoginType;                  /* refer to AUTH_LOGIN_TYPE_EN */
    char acRemoteAddr[64];
    char acOldToken[TOKEN_MAX_LEN];        /* special requirement for MSFT's REST API login */
} LOGIN_REQ_ST;

typedef struct LOGIN_RESPONSE_st {
    int iResult;
    uint32_t uiAccess;                /* refer to ACCESS_EN */
    uint32_t uiPriv;
    uint32_t uiAuthChannel;           /* refer to AUTH_TYPE_EN */
    char acToken[TOKEN_MAX_LEN];
} LOGIN_RESP_ST;

typedef struct LOGOUT_st {
    char acUserName[USER_NAME_MAX_LEN];       /* reserve, TBD */
    char acToken[TOKEN_MAX_LEN];
    uint32_t uiApp;                       /* refer to APP_EN */
    uint32_t uiAuthLoginType;             /* refer to AUTH_LOGIN_TYPE_EN ,  reserve, TBD */
} LOGOUT_ST;

#define AAA_IP_MAX_LEN           (40)
#define AAA_KEY_MAX_LEN          (256)

typedef struct {
    uint32_t uiUptimeOfLogin;          /* system uptime of login, used to cali elapsed_time until session logout   */
    char acIp[AAA_IP_MAX_LEN];         /* AAA server ip (tacacs+ or radius) */
    char acKey[AAA_KEY_MAX_LEN];       /* AAA server key (tacacs+ or radius) */
    uint32_t uiPort;
} AAA_TOKEN_ST;

typedef struct {
    char acToken[TOKEN_MAX_LEN];
    char acUsername[USER_NAME_MAX_LEN];
    uint32_t uiExpiration;                /* Session expiration UNIX timestamp */
    uint32_t uiLastLoginTime;             /* User last login UNIX timestamp  */
    uint32_t uiLastOperationTime;         /* User last operation UNIX timestamp */
    uint32_t uiApp;                       /* refer to APP_EN */
    uint32_t uiAuthLoginType;                 /* Refer to AUTH_LOGIN_TYPE_EN */
    uint32_t uiAccess;                        /* refer to ACCESS_EN */
    uint32_t uiPriv;
    uint32_t uiAuthChannel;                   /* refer to AUTH_TYPE_EN */
    char acRemoteAddr[64];
    AAA_TOKEN_ST stAaaToken;
    char acResv[16];
} TOKEN_ST;

#define AUTH_OPER_CMD_MAX_LEN             (256)

typedef struct {
    char acToken[TOKEN_MAX_LEN];
    uint32_t uiApp;                               /* refer to APP_EN */
    char acOperCmd[AUTH_OPER_CMD_MAX_LEN];
    int iStatus;
} AUTH_ST;

#define ADMIN_USER_NAME               "admin"

#define NODE_NAME                     "name"
#define NODE_PASSWORD                 "password"
#define NODE_APPLICATION              "application"
#define NODE_EXPIRE_TIME              "expire-time"
#define NODE_LAST_LOGIN_TIME          "last-login-time"
#define NODE_LAST_OPT_TIME            "last-operation-time"
#define NODE_USER_ROLE                "user-role"
#define NODE_PRIV_LV                  "priv-lv"
#define NODE_SESSION_ID               "session-id"
#define NODE_STATUS                   "status"
#define NODE_LOGIN_TYPE               "login-type"
#define NODE_REMOTE_ADDRESS           "remote-address"
#define NODE_AUTH_CHANNEL             "auth-channel"
#define NODE_TOKEN                    "token"
#define NODE_OPER_CMD                 "oper-cmd"

#define SLASH                          "/"

/*rpc*/
#define AUTH_MODULE_NAME                   "com-oplink-auth"
#define AUTH_MODULE_HEADER                   "/com-oplink-auth"
#define RPC_XPATH_LOGIN                      AUTH_MODULE_HEADER ":login"
#define RPC_XPATH_AUTH                       AUTH_MODULE_HEADER ":auth"
#define RPC_XPATH_DELETE_TOKENS_BY_USERNAME  AUTH_MODULE_HEADER ":delete-tokens-by-username"
#define RPC_XPATH_LOGOUT                     AUTH_MODULE_HEADER ":logout"

/*login rpc*/
#define XPATH_RPC_LOGIN_NAME           (RPC_XPATH_LOGIN SLASH NODE_NAME)
#define XPATH_RPC_LOGIN_PASSWORD       (RPC_XPATH_LOGIN SLASH NODE_PASSWORD)
#define XPATH_RPC_LOGIN_APPLICATION    (RPC_XPATH_LOGIN SLASH NODE_APPLICATION)
#define XPATH_RPC_LOGIN_SESSION        (RPC_XPATH_LOGIN SLASH NODE_SESSION_ID)
#define XPATH_RPC_LOGIN_TYPE           (RPC_XPATH_LOGIN SLASH NODE_LOGIN_TYPE)
#define XPATH_RPC_LOGIN_REMOTE_ADDRESS (RPC_XPATH_LOGIN SLASH NODE_REMOTE_ADDRESS)

#define XPATH_RPC_LOGIN_STATUS         (RPC_XPATH_LOGIN SLASH NODE_STATUS)
#define XPATH_RPC_LOGIN_USERROLE       (RPC_XPATH_LOGIN SLASH NODE_USER_ROLE)
#define XPATH_RPC_LOGIN_AUTH_CHANNEL   (RPC_XPATH_LOGIN SLASH NODE_AUTH_CHANNEL)
#define XPATH_RPC_LOGIN_TOKEN          (RPC_XPATH_LOGIN SLASH NODE_TOKEN)

/*auth rpc*/
#define XPATH_RPC_AUTH_TOKEN           (RPC_XPATH_AUTH SLASH NODE_TOKEN)
#define XPATH_RPC_AUTH_APPLICATION     (RPC_XPATH_AUTH SLASH NODE_APPLICATION)
#define XPATH_RPC_AUTH_STATUS          (RPC_XPATH_AUTH SLASH NODE_STATUS)
#define XPATH_RPC_AUTH_OPER_CMD        (RPC_XPATH_AUTH SLASH NODE_OPER_CMD)

/*delete-tokens-by-username*/
#define XPATH_RPC_DELETE_TOKES_USERNAME   (RPC_XPATH_DELETE_TOKENS_BY_USERNAME SLASH NODE_NAME)
#define XPATH_RPC_DELETE_TOKES_STATUS     (RPC_XPATH_DELETE_TOKENS_BY_USERNAME SLASH NODE_STATUS)

/*logout rpc*/
#define XPATH_RPC_LOGOUT_NAME          (RPC_XPATH_LOGOUT SLASH NODE_NAME)
#define XPATH_RPC_LOGOUT_APPLICATION   (RPC_XPATH_LOGOUT SLASH NODE_APPLICATION)
#define XPATH_RPC_LOGOUT_STATUS        (RPC_XPATH_LOGOUT SLASH NODE_STATUS)
#define XPATH_RPC_LOGOUT_LOGIN_TYPE    (RPC_XPATH_LOGOUT SLASH NODE_LOGIN_TYPE)
#define XPATH_RPC_LOGOUT_TOKEN         (RPC_XPATH_LOGOUT SLASH NODE_TOKEN)

/* state */
#define XPATH_AUTH_STATE               AUTH_MODULE_HEADER ":auth-mgnt/state"
#define XPATH_AUTH_TOKENS              XPATH_AUTH_STATE "/tokens"
#define XPATH_AUTH_TOKENS_PARA         XPATH_AUTH_TOKENS "[token='%s']/%s"

int DSC_Login(LOGIN_REQ_ST *pstReqLogin, LOGIN_RESP_ST *pstRespLogin);
int DSC_Logout(LOGOUT_ST *pstLogout);
int DSC_Auth(AUTH_ST *pstAuth);
int DSC_LoginWithResetSession(LOGIN_REQ_ST *pstReqLogin, LOGIN_RESP_ST *pstRespLogin);
int DSC_AuthWithResetSession(AUTH_ST *pstAuth);
int DSC_GetCurTokenRecord(TOKEN_ST *pstToken);
int DSC_GetTokenLists(TOKEN_ST **ppstToken, size_t *pnCnt);
int DSC_DeteleTokensByUsername(const char *pcUsername);

int DSC_SetDefaultUserAdmin(void);


/*****************************
   // Security API
*****************************/
#define DSC_SECURITY_API      (1)
#if DSC_SECURITY_API

typedef struct AUTH_SECURITY_st {
    uint32_t uiSessionMaxCount;
    uint32_t uiSessionTimeout;
    bool bPerCmdAuthorization;
} AUTH_SECURITY_ST;


#define NODE_SESSION_MAX_COUNT    "/session-maxcount"
#define NODE_SESSION_TIMEOUT      "/session-timeout"
#define NODE_PER_COMMAND_AUTHORIZATION      "/per-command-authorizaiton"

#define XPATH_SECURITY_CONFIG     AUTH_MODULE_HEADER ":auth-mgnt/config"
#define XPATH_NODE_SESSION_MAX_COUNT XPATH_SECURITY_CONFIG NODE_SESSION_MAX_COUNT
#define XPATH_NODE_SESSION_TIMEOUT XPATH_SECURITY_CONFIG NODE_SESSION_TIMEOUT
#define XPATH_NODE_COMMAND_AUTHORIZATION  XPATH_SECURITY_CONFIG NODE_PER_COMMAND_AUTHORIZATION

extern int DSC_SetSecuritySessionMaxCount(uint32_t uiMaxCount);
extern int DSC_SetSecuritySessionTimeout(uint32_t uiTimeout);
extern int DSC_SetSecurityPerCmdAuthorization(bool bEnable);
extern int DSC_DeletePerCmdAuthorization();
extern int DSC_GetSecurityConfig(AUTH_SECURITY_ST *pstSecurityConfig);

#endif

/* SSL api */
extern int DSC_CheckSslCerts(char *pcCertFile, char *pcKeyFile);
extern int DSC_CheckGnmiCerts(char *pcCertFile, char *pcKeyFile, char *pcCaFile);
/* extern int DSC_LoadSslCerts(char *pcCertFile, char *pcKeyFile); */
/* extern int DSC_RestoreSslCerts(); */

#ifdef __cplusplus
}
#endif

#endif




