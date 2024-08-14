#ifndef _DSC_SNMP_H
#define _DSC_SNMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
#include "DSC_acl.h"

#define DATA_MODULE_SNMP                                "ietf-snmp"
#define XPATH_SNMP_ALL                                  "/ietf-snmp:snmp"

/* community */
#define XPATH_COMMUNITY                                 "/ietf-snmp:snmp/community"
#define COMMUNITY_INDEX_NODE                            "/index"
#define XPATH_COMMUNITY_LIST_INDEX                      XPATH_COMMUNITY COMMUNITY_INDEX_NODE
#define COMMUNITY_SECURITY_NAME_NODE                    "/security-name"
#define XPATH_COMMUNITY_LIST_SECURITY_NAME              XPATH_COMMUNITY COMMUNITY_SECURITY_NAME_NODE
#define COMMUNITY_TEXT_NAME_NODE                        "/text-name"
#define XPATH_COMMUNITY_TEXT_NAME_LIST_ROLE             XPATH_COMMUNITY COMMUNITY_TEXT_NAME_NODE

#define XPATH_COMMUNITY_INDEX                           XPATH_COMMUNITY "[index='%s']"
#define XPATH_COMMUNITY_SECURITY_NAME                   XPATH_COMMUNITY_INDEX COMMUNITY_SECURITY_NAME_NODE
#define XPATH_COMMUNITY_TEXT_NAME                       XPATH_COMMUNITY_INDEX COMMUNITY_TEXT_NAME_NODE

#define XPATH_COMMUNITY_ACL                             XPATH_COMMUNITY_INDEX "/com-oplink-snmp-ext:acl"
#define XPATH_COMMUNITY_ACL_IPV4                        XPATH_COMMUNITY_ACL "/acl-ipv4"
#define XPATH_COMMUNITY_ACL_IPV6                        XPATH_COMMUNITY_ACL "/acl-ipv6"

#define INDEX_STRING                                    "index"
#define SECURITY_NAME_STRING                            "security-name"
#define TEXT_NAME_STRING                                "text-name"
#define SECURITY_MODEL_STRING                           "security-model"

/* usm */
#define XPATH_USM                                       "/ietf-snmp:snmp/usm"
#define XPATH_USM_USER                                  XPATH_USM "/local/user"
#define USM_NAME_NODE                                   "/name"
#define XPATH_USM_LIST_NAME                             XPATH_USM_USER USM_NAME_NODE
#define USM_AUTH_NODE                                   "/auth"
#define XPATH_USM_LIST_AUTH                             XPATH_USM_USER USM_AUTH_NODE
#define USM_PRIV_NODE                                   "/priv"
#define XPATH_USM_LIST_PRIV                             XPATH_USM_USER USM_PRIV_NODE

#define XPATH_USM_USER_NAME                             XPATH_USM_USER "[name='%s']"
#define TEST                                            XPATH_USM_USER "/name"
#define XPATH_USM_AUTH_SHA_KEY                          XPATH_USM_USER_NAME "/auth/sha/key"
#define XPATH_USM_AUTH_MD5_KEY                          XPATH_USM_USER_NAME "/auth/md5/key"
#define XPATH_USM_PRIV_DES_KEY                          XPATH_USM_USER_NAME "/priv/des/key"
#define XPATH_USM_PRIV_AES_KEY                          XPATH_USM_USER_NAME "/priv/aes/key"

#define USM_NAME_STRING                                 "name"
#define USM_AUTH_STRING                                 "auth"
#define USM_PRIV_STRING                                 "priv"
#define USM_SHA_STRING                                  "sha"
#define USM_MD5_STRING                                  "md5"
#define USM_DES_STRING                                  "des"
#define USM_AES_STRING                                  "aes"
#define USM_KEY_STRING                                  "key"


/* vacm */
#define XPATH_VACM                                      "/ietf-snmp:snmp/vacm"
#define XPATH_GROP_NODE                                 "/group"
#define XPATH_VACM_LIST_GROUP                           XPATH_VACM XPATH_GROP_NODE
#define XPATH_VACM_LIST_GROUP_ALL                       XPATH_VACM XPATH_GROP_NODE "//*"


#define XPATH_VACM_GROUP                                XPATH_VACM_LIST_GROUP "[name='%s']"
#define GROUP_NAME_NODE                                 "/name"
#define XPATH_VACM_GROUP_NAME                           XPATH_VACM_GROUP GROUP_NAME_NODE
#define GROUP_MEMBER_NODE                               "/member"
#define XPATH_VACM_GROUP_LIST_MEMBER                    XPATH_VACM_GROUP GROUP_MEMBER_NODE

/* /ietf-snmp:snmp/vacm/group[name='%s']/access/security-level */
#define XPATH_VACM_GROUP_ACCESS_SECURITY_LEVEL          XPATH_VACM_GROUP "/access/security-level"

#define XPATH_VACM_GROUP_MEMBER                         XPATH_VACM_GROUP_LIST_MEMBER "[security-name='%s']"
#define GROUP_MEMBER_SECURITY_NAME_NODE                 "/security-name"
#define XPATH_VACM_GROUP_MEMBER_SECURITY_NAME           XPATH_VACM_GROUP_MEMBER GROUP_MEMBER_SECURITY_NAME_NODE
#define GROUP_MEMBER_SECURITY_MODEL_NODE                "/security-model"
#define XPATH_VACM_GROUP_MEMBER_SECURITY_MODEL          XPATH_VACM_GROUP_MEMBER GROUP_MEMBER_SECURITY_MODEL_NODE



#define XPATH_VIEW_NODE                                 "/view"
#define XPATH_VACM_LIST_VIEW                            XPATH_VACM XPATH_VIEW_NODE



/* target parameter */
#define TARGET_PARAMS                                    "/ietf-snmp:snmp/target-params"
#define TARGET_PARAMS_USM_USER_NAME_NODE                 "/usm/user-name"
#define TARGET_PARAMS_USM_SECURITY_LEVEL_NODE            "/usm/security-level"
#define TARGET_PARAMS_V1_SECURITY_NAME_NODE              "/v1/security-name"
#define TARGET_PARAMS_V2C_SECURITY_NAME_NODE             "/v2c/security-name"


/* /ietf-snmp:snmp/target-params[name='%s'] */
#define XPATH_TARGET_PARAMS                              TARGET_PARAMS "[name='%s']"

/* /ietf-snmp:snmp/target-params[name='%s']/usm/user-name */
#define XPATH_TARGET_PARAMS_USM_USER_NAME                XPATH_TARGET_PARAMS TARGET_PARAMS_USM_USER_NAME_NODE

/* /ietf-snmp:snmp/target-params[name='%s']/usm/security-level */
#define XPATH_TARGET_PARAMS_USM_SECURITY_LEVEL           XPATH_TARGET_PARAMS TARGET_PARAMS_USM_SECURITY_LEVEL_NODE

/* /ietf-snmp:snmp/target-params[name='%s']/v1/security-name */
#define XPATH_TARGET_PARAMS_V1_SECURITY_NAME             XPATH_TARGET_PARAMS TARGET_PARAMS_V1_SECURITY_NAME_NODE

/* /ietf-snmp:snmp/target-params[name='%s']/v2c/security-name */
#define XPATH_TARGET_PARAMS_V2C_SECURITY_NAME            XPATH_TARGET_PARAMS TARGET_PARAMS_V2C_SECURITY_NAME_NODE

#define XPATH_TARGET                                    "/ietf-snmp:snmp/target"

/* /ietf-snmp:snmp/target[name='%s'] */
#define XPATH_TARGET_NAME                               XPATH_TARGET "[name='%s']"

/* /ietf-snmp:snmp/target[name='%s']/udp/ip */
#define XPATH_TARGET_USM_IP                             XPATH_TARGET_NAME "/udp/ip"

/* /ietf-snmp:snmp/target[name='%s']/udp/port */
#define XPATH_TARGET_USM_PORT                           XPATH_TARGET_NAME "/udp/port"

/* /ietf-snmp:snmp/target[name=%s]/target-params */
#define XPATH_TARGET_TARGET_PARAMS                      XPATH_TARGET_NAME "/target-params"



#define DSC_SNMP_V1_STRING                              "v1"
#define DSC_SNMP_V2C_STRING                             "v2c"
#define DSC_SNMP_V3_USM_STRING                          "usm"

#define DEF_GROUP_NAME_LEN  32

#if 0

#define DSC_SNMP_V1_RO_GROUP_STRING                     "group100"
#define DSC_SNMP_V1_RW_GROUP_STRING                     "group101"
#define DSC_SNMP_V2C_RO_GROUP_STRING                    "group200"
#define DSC_SNMP_V2C_RW_GROUP_STRING                    "group201"
#define DSC_SNMP_V3_NOAUTH_NOPRIV_RO_GROUP_STRING       "group300"
#define DSC_SNMP_V3_NOAUTH_NOPRIV_RW_GROUP_STRING       "group301"
#define DSC_SNMP_V3_AUTH_NOPRIV_RO_GROUP_STRING         "group310"
#define DSC_SNMP_V3_AUTH_NOPRIV_RW_GROUP_STRING         "group311"
#define DSC_SNMP_V3_AUTH_PRIV_RO_GROUP_STRING           "group320"
#define DSC_SNMP_V3_AUTH_PRIV_RW_GROUP_STRING           "group321"


typedef enum {
    EN_GROUP_ID_V1_RO = 0,
    EN_GROUP_ID_V1_RW,
    EN_GROUP_ID_V2C_RO,
    EN_GROUP_ID_V2C_RW,
    EN_GROUP_ID_V3_NOAUTH_NOPRIV_RO,
    EN_GROUP_ID_V3_NOAUTH_NOPRIV_RW,
    EN_GROUP_ID_V3_AUTH_NOPRIV_RO,
    EN_GROUP_ID_V3_AUTH_NOPRIV_RW,
    EN_GROUP_ID_V3_AUTH_PRIV_RO,
    EN_GROUP_ID_V3_AUTH_PRIV_RW,

    EN_GROUP_ID_CNT
}EN_GROUP_ID;
#endif


#if 0
#define DSC_SNMP_GROUP_STRING_V1V2_RO                   "group-v1v2-ro"
#define DSC_SNMP_GROUP_STRING_V1V2_RW                   "group-v1v2-rw"

#define DSC_SNMP_GROUP_STRING_V3_RO_AUTH_PRIV           "group-v3-ro-auth-priv"
#define DSC_SNMP_GROUP_STRING_V3_RW_AUTH_PRIV           "group-v3-rw-auth-priv"
#define DSC_SNMP_GROUP_STRING_V3_RO_AUTH_NOPRIV         "group-v3-ro-auth-nopriv"
#define DSC_SNMP_GROUP_STRING_V3_RW_AUTH_NOPRIV         "group-v3-rw-auth-nopriv"
#define DSC_SNMP_GROUP_STRING_V3_RO_NOAUTH_NOPRIV       "group-v3-ro-noauth-nopriv"
#define DSC_SNMP_GROUP_STRING_V3_RW_NOAUTH_NOPRIV       "group-v3-rw-noauth-nopriv"

#else

#define DSC_SNMP_GROUP_STRING_V1_RO                     "group100"
#define DSC_SNMP_GROUP_STRING_V1_RW                     "group101"
#define DSC_SNMP_GROUP_STRING_V1_TRAP                   "group-v1-trap"

#define DSC_SNMP_GROUP_STRING_V2_RO                     "group200"
#define DSC_SNMP_GROUP_STRING_V2_RW                     "group201"
#define DSC_SNMP_GROUP_STRING_V2_TRAP                   "group-v2-trap"

#define DSC_SNMP_GROUP_STRING_V3_RO_AUTH_PRIV           "group320"
#define DSC_SNMP_GROUP_STRING_V3_RW_AUTH_PRIV           "group321"
#define DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_PRIV         "group-v3-trap-auth-priv"

#define DSC_SNMP_GROUP_STRING_V3_RO_AUTH_NOPRIV         "group310"
#define DSC_SNMP_GROUP_STRING_V3_RW_AUTH_NOPRIV         "group311"
#define DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_NOPRIV       "group-v3-trap-auth-nopriv"

#define DSC_SNMP_GROUP_STRING_V3_RO_NOAUTH_NOPRIV       "group300"
#define DSC_SNMP_GROUP_STRING_V3_RW_NOAUTH_NOPRIV       "group301"
#define DSC_SNMP_GROUP_STRING_V3_TRAP_NOAUTH_NOPRIV     "group-v3-trap-noauth-nopriv"

#endif


typedef enum {
    EN_GROUP_ID_V1_RO = 0,
    EN_GROUP_ID_V1_RW,
    EN_GROUP_ID_V1_TRAP,

    EN_GROUP_ID_V2_RO,
    EN_GROUP_ID_V2_RW,
    EN_GROUP_ID_V2_TRAP,

    EN_GROUP_ID_V3_RO_NOAUTH_NOPRIV,
    EN_GROUP_ID_V3_RW_NOAUTH_NOPRIV,
    EN_GROUP_ID_V3_TRAP_NOAUTH_NOPRIV,

    EN_GROUP_ID_V3_RO_AUTH_NOPRIV,
    EN_GROUP_ID_V3_RW_AUTH_NOPRIV,
    EN_GROUP_ID_V3_TRAP_AUTH_NOPRIV,

    EN_GROUP_ID_V3_RO_AUTH_PRIV,
    EN_GROUP_ID_V3_RW_AUTH_PRIV,
    EN_GROUP_ID_V3_TRAP_AUTH_PRIV,


    EN_GROUP_ID_CNT
}EN_GROUP_ID;


#define DEF_AUTH_MODE_LEN               (9)
#define DEF_PRIV_MODE_LEN               (9)
#define DEF_AUTH_KEY_LEN                (16)
#define DEF_PRIV_KEY_LEN                (16)

#define DEF_SECU_SNMPSERVER_MAX         (40)

#define DEF_SNMP_USER_MAX               (10)
#define DEF_SNMP_TARGET_PARAMS_MAX      (10)
#define DEF_SNMP_TARGET_MAX             (10)


#define DEF_LEN_MAX_IP                  (40)

#define MIN_AUTH_KEY_LEN                (8)
#define MIN_PRIV_KEY_LEN                (8)


typedef enum EN_SNMP_ACCESS_en {
    EN_SNMP_ACCESS_MODE_NONE = 0,
    EN_SNMP_ACCESS_MODE_READ_ONLY = 1,
    EN_SNMP_ACCESS_MODE_READ_WRITE = 2,
    EN_SNMP_ACCESS_MODE_TRAP = 3,
} EN_SNMP_ACCESS;

typedef enum {
    EN_SNMP_VER_NONE = 0,
    EN_SNMP_VER_V1 = 1,
    EN_SNMP_VER_V2 = 2,
    EN_SNMP_VER_V3 = 3,

} EN_SNMP_VER;

typedef enum {
    EN_SNMP_TRAP_OFF = 0,
    EN_SNMP_TRAP_TRAP,
    EN_SNMP_TRAP_INFORM,
    EN_SNMP_TRAP_TRAP_ONLY,
    EN_SNMP_TRAP_CNT
} EN_SNMP_TRAP_MODE;

typedef enum {
    EN_SNMP_SECU_NOAUTH_NOPRIV = 1,
    EN_SNMP_SECU_AUTH_NOPRIV = 2,
    EN_SNMP_SECU_AUTH_PRIV = 3,
    EN_SNMP_SECU_CNT
} EN_SNMP_SECU_MODE;

typedef enum {
    EN_V3_AUTH_NULL = 0,
    EN_V3_AUTH_MD5,
    EN_V3_AUTH_SHA,
    EN_V3_AUTH_SHA256,
    EN_V3_AUTH_CNT
} EN_V3_AUTH;

typedef enum {
    EN_V3_PRIV_NULL = 0,
    EN_V3_PRIV_DES,
    EN_V3_PRIV_AES,
    EN_V3_PRIV_AES256,
    EN_V3_PRIV_CNT
} EN_V3_PRIV;

typedef struct {
    char acSecurityName[32];
    char acTextName[32];
    ACL_TABLE_ST stAclTableIpv4;
    ACL_TABLE_ST stAclTableIpv6;
}COMMUNITY_ST;


typedef struct {
    uint8_t ucAuthProto;
    uint8_t ucPrivProto;
    char acName[32];
    char acAuthProto[8];
    char acPrivProto[8];
    char acAuthKey[32];
    char acPrivKey[32];
}USM_USER_ST;


typedef struct {
    uint8_t ucVersion;
    uint8_t ucTrapMode;
    uint8_t ucAccess;
    uint8_t ucSecurityLevel;

    char acDestName[32];

    COMMUNITY_ST stCommunity;
    USM_USER_ST stUsmUser;

    char acSecurityModel[8];
    char acSecurityLevel[16];
    char acGroupName[DEF_GROUP_NAME_LEN];
    char acEngineId[128];
    char acUniqueId[33];  /* ID, not useful, using acDestName */

} __attribute__((packed)) DSC_SNMP_USER_ST;

typedef struct {
    uint8_t ucVersion;
    uint8_t ucSecurityLevel;
    char acName[32];
    char acVersion[16];

    char acUserName[32];
    char acSecurityModel[16];
    char acSecurityLevel[16];
}DSC_SNMP_TARGET_PARAMS_ST;


typedef struct {
    uint16_t usPort;
    uint8_t ucVersion;
    uint8_t ucSecurityLevel;
    char acName[32];
    char acIP[DEF_LEN_MAX_IP];
    char acTargetParams[32];

    COMMUNITY_ST stCommunity;
    DSC_SNMP_USER_ST stUser;
}DSC_SNMP_TARGET_ST;



typedef struct {
    char acSecurityName[32];
    char acSecurityModel[32];
}MEMBER_ST;


typedef struct {
    uint16_t usCount;
    COMMUNITY_ST astList[DEF_SNMP_USER_MAX];
}COMMUNITY_LIST_ST;




typedef struct {
    uint16_t usCount;
    USM_USER_ST astList[DEF_SNMP_USER_MAX];
}USM_USER_LIST_ST;



typedef struct {
    uint16_t usCount;
    DSC_SNMP_USER_ST astList[DEF_SNMP_USER_MAX];
} __attribute__((packed)) SNMP_USER_LIST_ST;

typedef struct {
    uint16_t usCount;
    DSC_SNMP_TARGET_PARAMS_ST astList[DEF_SNMP_TARGET_PARAMS_MAX];
} __attribute__((packed)) SNMP_TARGET_PARAMS_LIST_ST;


typedef struct {
    uint16_t usCount;
    DSC_SNMP_TARGET_ST astList[DEF_SNMP_TARGET_MAX];
} __attribute__((packed)) SNMP_TARGET_LIST_ST;

int DSC_GetUser(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser);

extern int DSC_GetUserList(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUserList);


extern int DSC_AddSnmpUser(DSC_SNMP_USER_ST *pstUser);
extern int DSC_DelSnmpUser(DSC_SNMP_USER_ST *pstUser, bool bCommit);
extern int DSC_GetSnmpUsers(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUsers);
extern bool DSC_FindSnmpUser(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser);

extern int DSC_AddTargetParams(DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams);
extern int DSC_DelTargetParams(DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams);
extern int DSC_GetTargetParamsList(sr_session_ctx_t *pstSess, SNMP_TARGET_PARAMS_LIST_ST *pstTargetParamsList);




extern int DSC_AddTarget(DSC_SNMP_TARGET_ST *pstTarget);
extern int DSC_DelSnmpTarget(DSC_SNMP_TARGET_ST *pstTarget);
extern int DSC_GetTargetList(sr_session_ctx_t *pstSess, SNMP_TARGET_LIST_ST *pstTargetList);


extern int DSC_GroupV1(DSC_SNMP_USER_ST *pstUser, char *pcGroup);
extern int DSC_GroupV2(DSC_SNMP_USER_ST *pstUser, char *pcGroup);
extern int DSC_GroupV3(DSC_SNMP_USER_ST *pstUser, char *pcGroup);

extern int DSC_AddAcl(DSC_SNMP_USER_ST *pstUser, char *pcAclName, char *pcAclType);
extern int DSC_DelAcl(DSC_SNMP_USER_ST *pstUser, char *pcAclName, char *pcAclType);

#ifdef __cplusplus
}
#endif

#endif
