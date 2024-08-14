/***MODU+******************************************************************************************
* FileName    : dsc_acl.c
* Description : acl set and get functions.
* History     :
*     [Author]          [Date]          [Version]        [Description]
* [1] bengangs          2019/12/06      Ver 1.0.0        Initial file.
* [2] Chun Ye           2022/11/08      Ver 1.0.1        Format and optimization
***MODU-******************************************************************************************/
#ifndef _DSC_ACL_H
#define _DSC_ACL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
/* #include "DSC_inc.h" */

/**************************************************************************************************/
/* definition of acl module                                                                       */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_DATA_MODULE_ACL          "openconfig-acl"
#define DEF_ACL_XPATH_HEADER         "/openconfig-acl:acl/acl-sets"
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of acl list xpath                                                                   */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_ACL_SET_KEY_IPV4           "ACL_IPV4"
#define DEF_ACL_SET_KEY_IPV6           "ACL_IPV6"

#define DEF_ACL_LEAF_SEQUENCE_ID       "sequence-id"
#define DEF_ACL_LEAF_SRC_ADDR          "source-address"
#define DEF_ACL_LEAF_FW_ACTION         "forwarding-action"

#define DEF_ACL_XPATH_NAME             DEF_ACL_XPATH_HEADER "/acl-set[name='%s'][type='%s']/config/name"
#define DEF_ACL_XPATH_TYPE             DEF_ACL_XPATH_HEADER "/acl-set[name='%s'][type='%s']/config/type"

#define DEF_ACL_XPATH_SET              DEF_ACL_XPATH_HEADER "/acl-set[name='user-defined'][type='openconfig-acl:%s']/acl-entries"
#define DEF_ACL_XPATH_SET_ALL          DEF_ACL_XPATH_SET "/acl-entry"

#define DEF_ACL_XPATH_ENTRY            DEF_ACL_XPATH_SET "/acl-entry[sequence-id='%u']"
#define DEF_ACL_XPATH_ENTRY_ID         DEF_ACL_XPATH_SET "/acl-entry[sequence-id='%u']/config/sequence-id"

#define DEF_ACL_XPATH_ENTRY_IPV4       DEF_ACL_XPATH_ENTRY "/ipv4/config/source-address"
#define DEF_ACL_XPATH_ENTRY_IPV6       DEF_ACL_XPATH_ENTRY "/ipv6/config/source-address"

#define DEF_ACL_XPATH_ENTRY_ACTION     DEF_ACL_XPATH_ENTRY "/actions/config/forwarding-action"

#define XPATH_NEW_ACL_LIST             DEF_ACL_XPATH_HEADER "/acl-set[name='%s'][type='%s']/acl-entries/acl-entry"

#define XPATH_ACL_LIST                 DEF_ACL_XPATH_HEADER "/acl-set[name='%s'][type='%s']/acl-entries"

#define XPATH_ACL_ENTRY_ID             XPATH_ACL_LIST "/acl-entry[sequence-id='%d']/config/sequence-id"

#define XPATH_ACL_ENTRY                XPATH_ACL_LIST "/acl-entry[sequence-id='%d']"

#define XPATH_ACL_ENTRY_IPV4           XPATH_ACL_ENTRY "/ipv4/config/source-address"
#define XPATH_ACL_ENTRY_IPV6           XPATH_ACL_ENTRY "/ipv6/config/source-address"

#define XPATH_ACL_ENTRY_DESTINATION_IPV4 XPATH_ACL_ENTRY "/ipv4/config/destination-address"
#define XPATH_ACL_ENTRY_DESTINATION_IPV6 XPATH_ACL_ENTRY "/ipv6/config/destination-address"

#define XPATH_ACL_ENTRY_IPV4_PROTOCOL       XPATH_ACL_ENTRY "/ipv4/config/protocol"
#define XPATH_ACL_ENTRY_IPV6_PROTOCOL       XPATH_ACL_ENTRY "/ipv6/config/protocol"

#define XPATH_ACL_ENTRY_ACTION              XPATH_ACL_ENTRY "/actions/config/forwarding-action"
#define XPATH_ACL_ENTRY_SOURCE_PORT         XPATH_ACL_ENTRY "/transport/config/source-port"
#define XPATH_ACL_ENTRY_DESTINATION_PORT    XPATH_ACL_ENTRY "/transport/config/destination-port"

/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of acl common len                                                                   */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_ACL_ENTRY_NUM_MAX   (20)
#define DEF_ACL_IPV4_MAX_LEN    (32)
#define DEF_ACL_IP_MAX_LEN      (48)
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of acl structure                                                                    */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    uint32_t ulId;
    char acSrcAddr[DEF_ACL_IP_MAX_LEN];
    char acAction[DEF_BUF_SIZE_64];
} ST_ACL;

typedef struct {
    ST_ACL *pstIpv4Acl;
    uint32_t uiIpv4Cnt;
    ST_ACL *pstIpv6Acl;
    uint32_t uiIpv6Cnt;
} ST_ACL_LIST;

typedef struct {
    uint32_t uiSequenceId;
    char acSrcAddr[40];
    uint16_t uiSrcPort;
    char acDestAddr[40];
    uint16_t uiDestPort;
    char acAction[64];
    uint32_t uiProtocol;
}ACL_ENTRY_ST;

typedef struct {
    char acAclName[32];
    char acAclType[32];
    ACL_ENTRY_ST astAclList[256];
    uint32_t uiAclCnt;
}ACL_TABLE_ST;
/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of acl function                                                                     */
/**************************************************************************************************/
/* @ {                                                                                            */
int DSC_GetAclList(bool bIsIpv4, ST_ACL_LIST *pstAclList, sr_session_ctx_t *pstSession);
int DSC_DelAclList(void);
int DSC_SetAclList(ST_ACL_LIST *pstAclList);
int DSC_ResetAclList(char *acAclljson);
int DSC_SetAclIpv6Type(void);

extern int DSC_GetAclListByName(char *pacAclName, char *pacAclType, ACL_TABLE_ST *pstAclList);
extern int DSC_AddAclEntry(char *pacAclName, char *pacAclType, ACL_ENTRY_ST *pstAclEntry, bool bCommit, bool bRefresh);
extern int DSC_GetAclListAll(ACL_TABLE_ST *pstAclList, size_t *puiCnt);
extern int DSC_DelAclEntry(char *pacAclName, char *pacAclType, uint32_t uiSequenceId);
extern int DSC_DelAclEntryAll(char *pacAclName, char *pacAclType);
/* @ }                                                                                            */


#ifdef __cplusplus
}
#endif

#endif
