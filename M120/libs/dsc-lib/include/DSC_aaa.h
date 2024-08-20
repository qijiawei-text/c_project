#ifndef _DSC_AAA_H
#define _DSC_AAA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
#include "DSC_auth.h"


#define DATA_MODULE_SYSTEM                     "ietf-system"
#define XPATH_SYSTEM_ALL                       "/ietf-system:system"
#define USER_SHELL_FILE                        "/usr/local/bin/user.sh"

/* auth */
#define XPATH_AUTH                             "/ietf-system:system/authentication"
#define XPATH_AUTH_USER_LIST                   XPATH_AUTH "/user"
#define XPATH_AUTH_ORDER_LIST                  XPATH_AUTH "/user-authentication-order"
#define EXT_TACACS_TYPE_STR                    "ietf-aaa-tacacs:tacacs"

#define USER_NAME_NODE                         "/name"
#define XPATH_AUTH_USER_LIST_NAME              XPATH_AUTH_USER_LIST USER_NAME_NODE
#define USER_PWD_NODE                          "/password"
#define XPATH_AUTH_USER_LIST_PASSWORD          XPATH_AUTH_USER_LIST USER_PWD_NODE
#define USER_ROLE_NODE                         "/com-oplink-auth:user-role"

#define XPATH_AUTH_USER                        XPATH_AUTH "/user[name='%s']"
#define XPATH_AUTH_USER_PASSWORD               XPATH_AUTH_USER USER_PWD_NODE
#define XPATH_AUTH_USER_ROLE                   XPATH_AUTH_USER USER_ROLE_NODE

#define LOWER_LIMIT_USER_NAME_LEN              (4)
#define UPPER_LIMIT_USER_NAME_LEN              (31)
#define LOWER_LIMIT_PASSWORD_LEN               (4)
#define UPPER_LIMIT_PASSWORD_LEN               (63)

#define HASH_MD5_SALT_MAX_LEN                  (8)
#define HASH_SHA256_SALT_MAX_LEN               (16)
#define MAX_XPATH_LEN                          (256)
/*default hash salt*/
#define HASH_COMMON_SALT                       "79EJ8YKe"

#define NACM_GROUP_READ_WRITE_EXEC             "write-user"
#define NACM_GROUP_READ_EXEC                   "read-user"
#define NACM_GROUP_SUPER_USER                  "super-user"

#define LOCAL_HOST_IP                          "127.0.0.1"


typedef enum USER_ROLE_en {
    READ_ONLY_ROLE = 0x01,
    READ_WRITE_ROLE = 0x02,
    FULL_ROLE = 0x04
} USER_ROLE_EN;

typedef struct
{
    char acName[USER_NAME_MAX_LEN];
    char acPassword[USER_PASSWORD_MAX_LEN];
    int iUserRole;
} USER_ST;

typedef struct
{
    USER_ST *pstUser;
    unsigned int uiTotal;
} USER_LIST_ST;

typedef struct XPATH_PWD_st
{
    char acXpath[MAX_XPATH_LEN];
    char acPwd[USER_PASSWORD_MAX_LEN];
} XPATH_PWD_ST;

typedef enum HASH_ALGORITHM_en
{
    EN_CLEAR_TEXT = 0,
    EN_HASH_MD5 = 1,
    EN_HASH_SHA256 = 5,
    EN_HASH_SHA512 = 6,
    EN_HASH_MAX
} HASH_ALGORITHM_EN;

typedef enum {
    AUTH_RADIUS = 0,
    AUTH_TACACS = 1,
    AUTH_LOCAL = 2
} AUTH_TYPE_EN;

typedef enum {
    EN_AUTH_ORDER_ONLY_LOCAL = 0,
    EN_AUTH_ORDER_LOCAL_THEN_TACACS,
    EN_AUTH_ORDER_TACACS_THEN_LOCAL,        /* TACACS+ first, then local-user. It is not related to whether TACACS is reachable or not */
    EN_AUTH_ORDER_TACACSONLY_THEN_LOCAL,    /* TACACS+ first, then local-user. when TACACS+ is reachable, all local user can not login */
    EN_AUTH_ORDER_ONLY_TACACS,
    EN_AUTH_ORDER_MAX
} USER_AUTH_ORDER_EN;

#define ONLY_LOCAL_STR                "only-local"
#define LOCAL_THEN_TACACS_STR         "local-then-tacacs"
#define TACACS_THEN_LOCAL_STR         "tacacs-then-local"
#define TACACSONLY_THEN_LOCAL_STR     "tacacsonly-then-local"

#define AUTH_RADIUS_STR   "radius"
#define AUTH_TACACS_STR   "tacacs"
#define AUTH_LOCAL_STR    "local-users"
#define DEF_USER_AUTHENTICATION_ORDER_MAX_ELEMENTS           (2)

int DSC_CheckUser(USER_ST *pstUser);
extern signed int DSC_AddUser(USER_ST *pstUser, USER_ST *pstCurUser);
extern signed int DSC_SetHashPassword(XPATH_PWD_ST *pstXpathPwd);
extern signed int DSC_SetHashPassword_Session(sr_session_ctx_t *sess, XPATH_PWD_ST *pstXpathPwd);
extern signed int DSC_DelUser(char *pcUserName, USER_ST *pstCurUser);
extern signed int DSC_GetUsers(USER_LIST_ST *pstUsers);
extern signed int DSC_GetHashPwdByUsername(sr_session_ctx_t *pstSess, const char *pcUsername, char *pcHashPwd, unsigned int uiLen);
extern signed int DSC_GetAuthOrder(int *piAuthOrder, int *piAuthCnt);
extern int DSC_SetUserAuthOrder(USER_AUTH_ORDER_EN enUserAuthOrder);
extern char *DSC_AuthToString(AUTH_TYPE_EN enAuthType, char *pcAuthStr);
extern signed int DSC_GetUserRole(const char *pcUsername, int *iUserRole);

/* radius */
#define XPATH_RADIUS                           "/ietf-system:system/radius"
#define XPATH_RADIUS_SERVER_LIST               XPATH_RADIUS "/server"
#define XPATH_RADIUS_SERVER_LIST_NAME          XPATH_RADIUS_SERVER_LIST "/name"
#define XPATH_RADIUS_SERVER_LIST_UDP           XPATH_RADIUS_SERVER_LIST "/udp"
#define XPATH_RADIUS_SERVER_LIST_UDP_ADDR      XPATH_RADIUS_SERVER_LIST_UDP "/address"
#define XPATH_RADIUS_SERVER_LIST_UDP_PORT      XPATH_RADIUS_SERVER_LIST_UDP "/authentication-port"
#define XPATH_RADIUS_SERVER_LIST_UDP_KEY       XPATH_RADIUS_SERVER_LIST_UDP "/shared-secret"
#define XPATH_RADIUS_SERVER_LIST_AUTHTYPE      XPATH_RADIUS_SERVER_LIST "/authentication-type"
#define XPATH_RADIUS_OPTIONS                   XPATH_RADIUS "/options"
#define XPATH_RADIUS_OPTIONS_TIMEOUT           XPATH_RADIUS_OPTIONS "/timeout"
#define XPATH_RADIUS_OPTIONS_ATTEMPTS          XPATH_RADIUS_OPTIONS "/attempts"
#define XPATH_RADIUS_SERVER                    XPATH_RADIUS "/server[name='%s']"
#define XPATH_RADIUS_SERVER_UDP_ADDR           XPATH_RADIUS_SERVER "/udp/address"
#define XPATH_RADIUS_SERVER_UDP_PORT           XPATH_RADIUS_SERVER "/udp/authentication-port"
#define XPATH_RADIUS_SERVER_UDP_KEY            XPATH_RADIUS_SERVER "/udp/shared-secret"

#define RADIUS_NAME_MAX_LEN                    36
#define RADIUS_IP_MAX_LEN                      40
#define RADIUS_KEY_MAX_LEN                     36



typedef struct
{
    char acName[RADIUS_NAME_MAX_LEN];
    char acIp[RADIUS_IP_MAX_LEN];
    char acKey[RADIUS_KEY_MAX_LEN];
    unsigned int uiPort;
} RADIUS_SERVER_ST;

typedef struct
{
    unsigned int uiTimeout;
    unsigned int uiRetryTimes;

} RADIUS_PARA_ST;


typedef struct RADIUS_CONFIG_LIST_st
{
    RADIUS_SERVER_ST *pstRadiusServer;
    RADIUS_PARA_ST stRadiusPara;
    unsigned int uiTotal;
} RADIUS_SERVER_LIST_ST;


extern signed int DSC_AddRadiusServer(RADIUS_SERVER_ST *pstServer);
extern signed int DSC_SetRadiusPara(RADIUS_PARA_ST *pstPara);
extern signed int DSC_DelRadiusServer(char *pcServerName);
extern signed int DSC_GetRadiusServers(RADIUS_SERVER_LIST_ST *pstServers);

/* nacm */
#define DATA_MODULE_ACM                             "ietf-netconf-acm"

#define XPATH_NACM                                  "/ietf-netconf-acm:nacm"
#define XPATH_NACM_CHILD_NODE                       XPATH_NACM "//*"
#define XPATH_NACM_ENABLE                           XPATH_NACM "/enable-nacm"
#define XPATH_NACM_READDEF                          XPATH_NACM "/read-default"
#define XPATH_NACM_WRITEDEF                         XPATH_NACM "/write-default"
#define XPATH_NACM_EXECDEF                          XPATH_NACM "/exec-default"
#define XPATH_NACM_ENABLEEXTGROUPS                  XPATH_NACM "/enable-external-groups"
#define XPATH_NACM_GROUPS                           XPATH_NACM "/groups"
#define XPATH_NACM_GROUPS_GROUP_LIST                XPATH_NACM_GROUPS "/group"
#define XPATH_NACM_GROUPS_GROUP_LIST_NAME           XPATH_NACM_GROUPS_GROUP_LIST "/name"
#define XPATH_NACM_GROUPS_GROUP_LIST_USERNAME_LIST  XPATH_NACM_GROUPS_GROUP_LIST "/user-name"
#define XPATH_NACM_GROUPS_GROUP                     XPATH_NACM_GROUPS "/group[name='%s']"
#define XPATH_NACM_GROUPS_GROUP_USERNAME_LIST       XPATH_NACM_GROUPS_GROUP "/user-name"
#define XPATH_NACM_GROUPS_GROUP_USERNAME            XPATH_NACM_GROUPS_GROUP "/user-name[.='%s']"
#define XPATH_NACM_RULELIST_LIST                    XPATH_NACM "/rule-list"
#define XPATH_NACM_RULELIST_LIST_NAME               XPATH_NACM_RULELIST_LIST "/name"
#define XPATH_NACM_RULELIST_LIST_GROUP_LIST         XPATH_NACM_RULELIST_LIST "/group"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST          XPATH_NACM_RULELIST_LIST "/rule"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST_NAME     XPATH_NACM_RULELIST_LIST_RULE_LIST "/name"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST_MODNAME  XPATH_NACM_RULELIST_LIST_RULE_LIST "/module-name"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST_ACCOP    XPATH_NACM_RULELIST_LIST_RULE_LIST "/access-operations"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST_ACTION   XPATH_NACM_RULELIST_LIST_RULE_LIST "/action"
#define XPATH_NACM_RULELIST_LIST_RULE_LIST_COMMENT  XPATH_NACM_RULELIST_LIST_RULE_LIST "/comment"
#define XPATH_NACM_RULELIST_LIST_RULE               XPATH_NACM_RULELIST_LIST "/rule[name='%s']"
#define XPATH_NACM_RULELIST                         XPATH_NACM "/rule-list[name='%s']"
#define XPATH_NACM_RULELIST_GROUP_LIST              XPATH_NACM_RULELIST "/group"


#define NACM_NAME_MAX_LEN                           64
#define NACM_RULE_MAX_LEN                           128


typedef enum
{
    ACTION_TYPE_PERMIT = 0,
    ACTION_TYPE_DENY,
    ACTION_TYPE_CNT

} ACTION_TYPE_EN;

typedef enum
{
    RULE_TYPE_PROTOCOL_OPERATION = 0,
    RULE_TYPE_NOTIFICATION,
    RULE_TYPE_DATA_NODE,
    RULE_TYPE_CNT

} RULE_TYPE_EN;

typedef struct
{
    bool bEnable;
    ACTION_TYPE_EN enReadActionType;
    ACTION_TYPE_EN enWriteActionType;
    ACTION_TYPE_EN enExecActionType;
    bool bExtGroupEnable;
    int iDeniedOpers;      /* ro */
    int iDeniedWrites;     /* ro */
    int iDeniedNotifs;     /* ro */
} NACM_BASIC_ST;

typedef struct
{
    char acGroupName[NACM_NAME_MAX_LEN];
    char (*pacUserName)[NACM_NAME_MAX_LEN];
    unsigned int uiUserCnt;
} NACM_GROUP_ST;

typedef struct
{
    NACM_GROUP_ST *pstGroup;
    unsigned int uiGroupCnt;
} NACM_GROUP_LIST_ST;


typedef struct
{
    char acRuleName[NACM_NAME_MAX_LEN];
    char acModName[NACM_NAME_MAX_LEN];
    RULE_TYPE_EN enRuleType;
    char acRule[NACM_RULE_MAX_LEN];     /* protocol-operation:rpc-name; notification:notification-name; data-node:path */
    char acAccessOp[NACM_RULE_MAX_LEN];
    ACTION_TYPE_EN enActionType;
    char acComment[NACM_RULE_MAX_LEN];
} RULE_ST;

typedef struct
{
    RULE_ST *pstRule;
    unsigned int uiRuleCnt;
} RULE_LIST_ST;

typedef struct
{
    char acRuleListName[NACM_NAME_MAX_LEN];
    char (*pacGroupName)[NACM_NAME_MAX_LEN];
    unsigned int uiGroupCnt;
    RULE_LIST_ST stRuleList;
} RULELIST_ST;

typedef struct
{
    RULELIST_ST *pstRulelist;
    unsigned int uiRulelistCnt;
} RULELIST_LIST_ST;

extern signed int DSC_SetNacmEnable(bool bState);
extern signed int DSC_AddNacmGroupUser(char *pcGroupName, char *pcUserName);
extern signed int DSC_DelNacmGroupUser(char *pcGroupName, char *pcUserName);
extern signed int DSC_GetNacmBasic(NACM_BASIC_ST *pstNacmBasic);
extern signed int DSC_GetNacmGroups(NACM_GROUP_LIST_ST *pstNacmGroups);
extern signed int DSC_GetNacmRulelists(RULELIST_LIST_ST *pstNacmrulelists);
int DSC_UserNacmDelete(char *pcUserName);
int DSC_UserNacmAdd(char *pcUserName, ACCESS_EN iUserRole);

char *DSC_GetNacmGroupNameByRole(int iRole);

extern int DSC_CryptHash(HASH_ALGORITHM_EN enHashMethod, char *pcSalt, char *pcPwd, char *pcHashPwd, unsigned int uiLen);

#define XPATH_TACACS                          "/ietf-system:system/ietf-aaa-tacacs:tacacs"

#define XPATH_TACACS_SERVER_LIST               XPATH_TACACS "/ietf-aaa-tacacs:server"
#define XPATH_TACACS_SERVER_LIST_NAME          XPATH_TACACS_SERVER_LIST "/name"
#define XPATH_TACACS_SERVER_LIST_TYPE          XPATH_TACACS_SERVER_LIST "/server-type"
#define XPATH_TACACS_SERVER_LIST_ADDR          XPATH_TACACS_SERVER_LIST "/address"
#define XPATH_TACACS_SERVER_LIST_PORT          XPATH_TACACS_SERVER_LIST "/port"
#define XPATH_TACACS_SERVER_LIST_KEY           XPATH_TACACS_SERVER_LIST "/shared-secret"

#define XPATH_TACACS_SERVER                    XPATH_TACACS "/server[name='%s']"
#define XPATH_TACACS_SERVER_TYPE               XPATH_TACACS_SERVER "/server-type"
#define XPATH_TACACS_SERVER_ADDR               XPATH_TACACS_SERVER "/address"
#define XPATH_TACACS_SERVER_PORT               XPATH_TACACS_SERVER "/port"
#define XPATH_TACACS_SERVER_KEY                XPATH_TACACS_SERVER "/shared-secret"

#define XPATH_TACACS_OPTIONS                   XPATH_TACACS "/options"
#define XPATH_TACACS_OPTIONS_TIMEOUT           XPATH_TACACS_OPTIONS "/timeout"

#define PASSWORD_NOT_CLEAR_TEXT                "********"

#define TACACS_NAME_MAX_LEN                    (36)
#define TACACS_IP_MAX_LEN                      (40)
#define TACACS_KEY_MAX_LEN                     (128)
#define TACACS_SERVER_DEFAULT_PORT             (49)
#define TACACS_DEFAULT_TIMEOUT                 (3)

#define FIRST_PRIORITY_TACACS_NAME             "1"
#define SECOND_PRIORITY_TACACS_NAME            "2"

typedef enum OPERATE_OPTIONS_e {
    ADD_OPERATE,
    MODIFY_OPERATE
}OPERATE_OPTIONS_E;

typedef struct {
    char acName[TACACS_NAME_MAX_LEN];
    char acIp[TACACS_IP_MAX_LEN];
    unsigned int uiTimeOut;
    unsigned int uiPort;
    char acKey[TACACS_KEY_MAX_LEN + 1];
} TACACS_SERVER_ST;

typedef struct TACACS_CONFIG_LIST_st {
    TACACS_SERVER_ST *pstTacacsServer;
    unsigned int uiTotal;
} TACACS_SERVER_LIST_ST;

#define TACACS_SERVER_MAX_NUM                 2
#define TACACS_ACCOUNTING_FEATURE_ENABLE      1

int DSC_AddTacServer(TACACS_SERVER_ST *pstTacacsServer);
int DSC_UpdateTacServer(TACACS_SERVER_ST *pstTacacsServer);
int DSC_DelTacServer(char *pcServerIp);
int DSC_GetTacServers(TACACS_SERVER_LIST_ST *pstTacacsList);

extern int DSC_ParseUsernameFromXpath(const char *pcXpath, char *pcUsername, unsigned int uiLen);

char *DSC_ConvertRoleToString(int iRole);
int DSC_ConvertRoleToInt(const char *pcRole, int *piRole);

#ifdef __cplusplus
}
#endif

#endif
