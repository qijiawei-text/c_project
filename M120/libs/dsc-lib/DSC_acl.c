/***MODU+******************************************************************************************
* FileName    : dsc_acl.c
* Description : acl set and get functions.
* History     :
*     [Author]          [Date]          [Version]        [Description]
* [1] bengangs          2019/12/06      Ver 1.0.0        Initial file.
* [2] Chun Ye           2022/11/08      Ver 1.0.1        Format and optimization
***MODU-******************************************************************************************/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "DSC_acl.h"
#include "cJSON.h"
#include "DSC_snmp.h"
/**************************************************************************************************/
/* definition of support acl leaf number max                                                      */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_ACL_LEAF_NUM_MAX      (64)
/* @ }                                                                                            */


/***FUNC+******************************************************************************************/
/* Name   : __DelAclNode                                                                          */
/* Descrp : Delete the acl node in db. This function is used in reset acl                         */
/* Input  : pcXpath  -- A pointer of path to be deleted                                           */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_XXX :                                                                 */
/***FUNC-******************************************************************************************/
static int
__DelAclNode(char *pcXpath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check session                                                            */
    if (NULL == pstSess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Session null: 0x%p", pstSess);
        goto cleanup;
    }

    /* Check parameter                                                          */
    if (NULL == pcXpath) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcXpath);
        goto cleanup;
    }

    /* Using the flag SR_EDIT_STRICT, the identified element must exist         */
    rc = sr_delete_item(pstSess, pcXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Delete: %s error: %s", pcXpath, sr_strerror(rc));
        goto cleanup;
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", pcXpath, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __GetAclIp                                                                            */
/* Descrp : Get acl ip by 'xxx.xxx.xxx.xxx/mask', this function will modidfy pcIpAndPrefix        */
/* Input  : pcIpAndPrefix  -- A pointer of path of ip and mask                                    */
/* Output : pcStr -- A pointer of ip                                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg NULL :                                                                       */
/*              @arg A pointer of IP :                                                            */
/***FUNC-******************************************************************************************/
static char *
__GetAclIp(char *pcIpAndPrefix)
{
    char *pcStr = NULL;

    /* This is from man user manual.
       If  *stringp is NULL, the strsep() function returns NULL and does nothing else.  Otherwise, this function finds the first
       token in the string *stringp, that is delimited by one of the bytes in the string delim.  This  token  is  terminated  by
       overwriting the delimiter with a null byte ('\0'), and *stringp is updated to point past the token.  In case no delimiter
       was found, the token is taken to be the entire string *stringp, and *stringp is made NULL.
     */
    pcStr = strsep(&pcIpAndPrefix, "/");

    return pcStr;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_GetAclList                                                                        */
/* Descrp : Get ACL list ipv4/ipv6                                                                */
/* Input  : bIsIpv4  -- ipv4/ipv6 flag                                                            */
/* Input  : pstSession  -- A pointer of session                                                   */
/* Output : pstAclList -- A pointer of acl list                                                   */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_xxx :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_GetAclList(bool bIsIpv4, ST_ACL_LIST *pstAclList, sr_session_ctx_t *pstSession)
{
    sr_data_t *pstSubtrees = NULL;
    size_t uiCnt = 0;
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    sr_session_ctx_t *pstSess = NULL;
    unsigned int uiLeafCnt;
    sr_node_t *apstLeaf[DEF_ACL_LEAF_NUM_MAX];
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    ST_ACL *pstAcl = NULL;
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    /* Check parameter                                                          */
    if (NULL == pstAclList) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pstAclList);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* Check session                                                            */
    if (pstSession != NULL) {
        pstSess = pstSession;
    } else {
        pstSess = DSC_GetRunningSession();
    }

    /* Check ipv4/ipv6 flag                                                     */
    memset(acXpath, 0, sizeof(acXpath));
    if (true == bIsIpv4) {
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET_ALL, DEF_ACL_SET_KEY_IPV4);
    } else {
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET_ALL, DEF_ACL_SET_KEY_IPV6);
    }
    /* get user defined ACL entries                                             */
    rc = get_subtrees(pstSess, acXpath, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_NOT_FOUND == rc) {
        uiCnt = 0;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees: %s error: %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, acXpath, &pstNodeSet);
        if (LY_SUCCESS == rc) {
            uiCnt = pstNodeSet->count;
        }
    }

    /* if found at least one entry                                              */
    if (uiCnt != 0) {
        pstAcl = (ST_ACL *)malloc(uiCnt * sizeof(ST_ACL));
        if (NULL == pstAcl) {
            LOG_WRITE(EN_LOG_ERROR, "pstAcl malloc error: %s", strerror(errno));
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }

        memset(pstAcl, 0x00, uiCnt * sizeof(ST_ACL));
        for (i = 0; i < uiCnt; i++) {
            uiLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);

            for (j = 0; j < uiLeafCnt; j++) {
                pstLeaf = (sr_node_t *)apstLeaf[j];
                if (0 == strcmp(pstLeaf->schema->name, DEF_ACL_LEAF_SEQUENCE_ID)) {
                    pstAcl[i].ulId = ((sr_node_term_t *)pstLeaf)->value.uint32;
                } else if (0 == strcmp(pstLeaf->schema->name, DEF_ACL_LEAF_SRC_ADDR)) {
                    strncpy(pstAcl[i].acSrcAddr, lyd_get_value(pstLeaf), sizeof(pstAcl[i].acSrcAddr) - 1);
                } else if (0 == strcmp(pstLeaf->schema->name, DEF_ACL_LEAF_FW_ACTION)) {
                    strncpy(pstAcl[i].acAction, lyd_get_value(pstLeaf), sizeof(pstAcl[i].acAction) - 1);

                }
            }
        }
    }

    /* Set the output value, if the uiCnt is 0, the pointer is NULL             */
    if (true == bIsIpv4) {
        pstAclList->pstIpv4Acl = pstAcl;
        pstAclList->uiIpv4Cnt = uiCnt;
    } else {
        pstAclList->pstIpv6Acl = pstAcl;
        pstAclList->uiIpv6Cnt = uiCnt;
    }

ERR_LABEL:
    if (NULL != pstNodeSet) {
        ly_set_free(pstNodeSet, NULL);
        pstNodeSet = NULL;
    }

    if (NULL != pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetAclList                                                                        */
/* Descrp : Set ACL list ipv4/ipv6                                                                */
/* Input  : pstAclList  -- A pointer of acl list                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_xxx :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetAclList(ST_ACL_LIST *pstAclList)
{
    int rc = SR_ERR_OK;
    int i = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    for (i = 0; i < pstAclList->uiIpv4Cnt; i++) {
        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ID, DEF_ACL_SET_KEY_IPV4, pstAclList->pstIpv4Acl[i].ulId);
        rc = set_uint32_item(pstSess, acXpath, pstAclList->pstIpv4Acl[i].ulId);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_IPV4, DEF_ACL_SET_KEY_IPV4, pstAclList->pstIpv4Acl[i].ulId);
        rc = set_string_item(pstSess, acXpath, pstAclList->pstIpv4Acl[i].acSrcAddr, strlen(pstAclList->pstIpv4Acl[i].acSrcAddr));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ACTION, DEF_ACL_SET_KEY_IPV4, pstAclList->pstIpv4Acl[i].ulId);
        rc = set_identityref_item(pstSess, acXpath, pstAclList->pstIpv4Acl[i].acAction, strlen(pstAclList->pstIpv4Acl[i].acAction));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }
    }

    for (i = 0; i < pstAclList->uiIpv6Cnt; i++) {
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ID, DEF_ACL_SET_KEY_IPV6, pstAclList->pstIpv6Acl[i].ulId);
        rc = set_uint32_item(pstSess, acXpath, pstAclList->pstIpv6Acl[i].ulId);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_IPV6, DEF_ACL_SET_KEY_IPV6, pstAclList->pstIpv6Acl[i].ulId);
        rc = set_string_item(pstSess, acXpath, pstAclList->pstIpv6Acl[i].acSrcAddr, strlen(pstAclList->pstIpv6Acl[i].acSrcAddr));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ACTION, DEF_ACL_SET_KEY_IPV6, pstAclList->pstIpv6Acl[i].ulId);
        rc = set_identityref_item(pstSess, acXpath, pstAclList->pstIpv6Acl[i].acAction, strlen(pstAclList->pstIpv6Acl[i].acAction));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }
    }

    /* Commit operation                                                         */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", "Set acl", sr_strerror(rc));
    }
ERR_LABEL:

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_DelAclList                                                                        */
/* Descrp : Del ACL list ipv4/ipv6                                                                */
/* Input  : NONE                                                                                  */
/* Output  : NONE                                                                                 */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_xxx :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_DelAclList(void)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    ST_ACL_LIST stAclListIpv4;
    ST_ACL_LIST stAclListIpv6;

    /* Get the current acl list ipv4                                            */
    memset(&stAclListIpv4, 0x00, sizeof(stAclListIpv4));
    memset(&stAclListIpv6, 0x00, sizeof(stAclListIpv6));
    rc = DSC_GetAclList(true, &stAclListIpv4, NULL);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get acl list ipv4 error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* If ipv4 exist, delete them                                               */
    if (stAclListIpv4.uiIpv4Cnt > 0) {
        /*Delete exiting entries                                                */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET, DEF_ACL_SET_KEY_IPV4);
        rc = __DelAclNode(acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Del acl list ipv4 error: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    /* Get the current acl list ipv6                                            */
    rc = DSC_GetAclList(false, &stAclListIpv6, NULL);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get acl list ipv6 error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* Get the current acl list ipv6                                            */
    if (stAclListIpv6.uiIpv6Cnt > 0) {
        /*Delete exiting entries                                                */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET, DEF_ACL_SET_KEY_IPV6);
        rc = __DelAclNode(acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Del acl list ipv4 error: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

ERR_LABEL:
    DEF_SAFE_FREE(stAclListIpv4.pstIpv4Acl);
    DEF_SAFE_FREE(stAclListIpv6.pstIpv6Acl);

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_ResetAclList                                                                      */
/* Descrp : Reset acl list.                                                                       */
/*          Del the exist acl list and write the new acl list. If re-write failure, restore the   */
/*          old acl list                                                                          */
/* Input  : acAclJson -- A pointer of acl json file                                               */
/* Output  : NONE                                                                                 */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_xxx :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_ResetAclList(char *pcAclContext)
{
    int rc = SR_ERR_OK;
    int iIndexIpv4 = 0;
    int iIndexIpv6 = 0;
    cJSON *cjsonAcl = NULL, *pstCurItem = NULL, *pstAclEntry = NULL, *pstLeaf = NULL;
    const char *pcReturnParseEnd = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    ST_ACL_LIST stAclListIpv4;
    ST_ACL_LIST stAclListIpv6;
    char acBufTmp[64] = {0};
    char acBufTmp1[64] = {0};
    char *pcIpTmp = NULL;
    bool bFlagIp = false;
    bool bFlagTarget = false;
    struct in_addr stV4Addr;
    struct in6_addr stIpV6Addr;
    bool bRestoreIpv4 = false;
    bool bRestoreIpv6 = false;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* Check parameter                                                          */
    if (NULL == pcAclContext) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: 0x%p", pcAclContext);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* Read the old acl ipv4 data.                                              */
    memset(&stAclListIpv4, 0x00, sizeof(stAclListIpv4));
    rc = DSC_GetAclList(true, &stAclListIpv4, NULL);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetAclList Error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (stAclListIpv4.uiIpv4Cnt > 0) {
        /* Delete exiting entries                                               */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET, "ACL_IPV4");
        rc = __DelAclNode(acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode Error: %d", (rc));
            goto ERR_LABEL;
        }
    }
    bRestoreIpv4 = true;

    /* Read the old acl ipv6 data.                                              */
    memset(&stAclListIpv6, 0x00, sizeof(stAclListIpv6));
    rc = DSC_GetAclList(false, &stAclListIpv6, NULL);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetAclList Error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (stAclListIpv6.uiIpv6Cnt > 0) {
        /* Delete exiting entries                                               */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_SET, "ACL_IPV6");
        rc = __DelAclNode(acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode Error: %d", (rc));
            goto ERR_LABEL;
        }
    }
    bRestoreIpv6 = true;

    /* Parse the acl context first.                                             */
    cjsonAcl = cJSON_ParseWithOpts(pcAclContext, (const char **)&pcReturnParseEnd, 1);
    if (NULL == cjsonAcl) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Json not valid");
        goto ERR_LABEL;
    }

    /* Parse the acl context first.                                             */
    pstCurItem = cjsonAcl->child;
    if (NULL == pstCurItem) {
        /* Delete the old values                                                */
        goto ERR_LABEL;
    }

    /* Format                                                                   */
    /*  {
          "protocol": "ipv4",
          "rules": [{"source-ip": "172.17.166.6/24","target": "ACCEPT"},
                    {"source-ip": "172.16.166.6/24","target": "ACCEPT"},
          ],

          "rules-ipv6": [{"source-ip": "2001::1/64","target": "ACCEPT"},
                        {"source-ip": "2002::1/64","target": "ACCEPT"},
          ]
        }                                                                       */
    while (pstCurItem != NULL && pstCurItem->string != NULL) {

        if (0 == strcmp(pstCurItem->string, "rules")) {
            pstAclEntry = pstCurItem->child;
            while (pstAclEntry != NULL) {
                iIndexIpv4++;
                if (iIndexIpv4 > DEF_ACL_ENTRY_NUM_MAX) {
                    rc = SR_ERR_INVAL_ARG;
                    sr_discard_changes(pstSess);
                    LOG_WRITE(EN_LOG_ERROR, "acl rules num: %d error", iIndexIpv4);
                    goto ERR_LABEL;
                }

                memset(acXpath, 0x00, sizeof(acXpath));
                snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ID, DEF_ACL_SET_KEY_IPV4, iIndexIpv4);
                rc = set_uint32_item(pstSess, acXpath, iIndexIpv4);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                    sr_discard_changes(pstSess);
                    goto ERR_LABEL;
                }

                /* Set sysrepo for one acl entry                                */
                pstLeaf = pstAclEntry->child;
                while (pstLeaf != NULL) {
                    if (0 == strcmp(pstLeaf->string, "source-ip")) {

                        memset(acBufTmp, 0, sizeof(acBufTmp));
                        strncpy(acBufTmp, pstLeaf->valuestring, sizeof(acBufTmp) - 1);
                        memset(acBufTmp1, 0, sizeof(acBufTmp1));
                        strncpy(acBufTmp1, pstLeaf->valuestring, sizeof(acBufTmp1) - 1);

                        /* Get ip by '172.16.166.6/24'                          */
                        pcIpTmp = __GetAclIp(acBufTmp);
                        if (NULL == pcIpTmp) {
                            LOG_WRITE(EN_LOG_ERROR, "Ip error: %s, %s", acBufTmp, strerror(errno));
                            sr_discard_changes(pstSess);
                            rc = SR_ERR_INVAL_ARG;
                            goto ERR_LABEL;
                        }

                        /* Check the ip address                                 */
                        memset(acXpath, 0x00, sizeof(acXpath));
                        rc = inet_pton(AF_INET, pcIpTmp, &stV4Addr);
                        if (rc > 0) {
                            snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_IPV4, DEF_ACL_SET_KEY_IPV4, iIndexIpv4);
                        } else {
                            LOG_WRITE(EN_LOG_ERROR, "Ip error: %s, %s", pcIpTmp, strerror(errno));
                            sr_discard_changes(pstSess);
                            rc = SR_ERR_INVAL_ARG;
                            goto ERR_LABEL;
                        }

                        /* Set the value to DB                                  */
                        rc = set_string_item(pstSess, acXpath, acBufTmp1, strlen(acBufTmp1));
                        if (rc != SR_ERR_OK) {
                            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        bFlagIp = true;
                    } else if (0 == strcmp(pstLeaf->string, "target")) {
                        memset(acXpath, 0, sizeof(acXpath));
                        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ACTION, DEF_ACL_SET_KEY_IPV4, iIndexIpv4);
                        memset(acBufTmp, 0, sizeof(acBufTmp));
                        strcpy(acBufTmp, "openconfig-acl:");
                        if (strlen(pstLeaf->valuestring) > DEF_ACL_IPV4_MAX_LEN) {
                            LOG_WRITE(EN_LOG_ERROR, "Ipv4 len error: %s", pstLeaf->string);
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        strcat(acBufTmp, pstLeaf->valuestring);
                        rc = set_identityref_item(pstSess, acXpath, acBufTmp, strlen(acBufTmp));
                        if (rc != SR_ERR_OK) {
                            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        bFlagTarget = true;
                    } else {
                        LOG_WRITE(EN_LOG_ERROR, "invalid json input: %s", pstLeaf->string);
                        sr_discard_changes(pstSess);
                        rc = SR_ERR_INVAL_ARG;
                        goto ERR_LABEL;
                    }

                    pstLeaf = pstLeaf->next;
                }
                pstAclEntry = pstAclEntry->next;
            }
        } else if (strcmp(pstCurItem->string, "rules-ipv6") == 0) {
            pstAclEntry = pstCurItem->child;

            while (pstAclEntry != NULL) {
                iIndexIpv6++;
                if (iIndexIpv6 > DEF_ACL_ENTRY_NUM_MAX) {
                    rc = SR_ERR_INVAL_ARG;
                    sr_discard_changes(pstSess);
                    LOG_WRITE(EN_LOG_ERROR, "acl rules-ipv6 num: %d error", iIndexIpv6);
                    goto ERR_LABEL;
                }

                memset(acXpath, 0, sizeof(acXpath));
                snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ID, DEF_ACL_SET_KEY_IPV6, iIndexIpv6);
                rc = set_uint32_item(pstSess, acXpath, iIndexIpv6);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                    sr_discard_changes(pstSess);
                    goto ERR_LABEL;
                }

                /* Set sysrepo for one acl entry                                */
                pstLeaf = pstAclEntry->child;
                while (pstLeaf != NULL) {
                    if (0 == strcmp(pstLeaf->string, "source-ip")) {

                        memset(acBufTmp, 0, sizeof(acBufTmp));
                        strncpy(acBufTmp, pstLeaf->valuestring, sizeof(acBufTmp) - 1);
                        memset(acBufTmp1, 0, sizeof(acBufTmp1));
                        strncpy(acBufTmp1, pstLeaf->valuestring, sizeof(acBufTmp1) - 1);

                        pcIpTmp = __GetAclIp(acBufTmp);
                        if (NULL == pcIpTmp) {
                            LOG_WRITE(EN_LOG_ERROR, "ip error %s", acBufTmp);
                            sr_discard_changes(pstSess);
                            rc = SR_ERR_INVAL_ARG;
                            goto ERR_LABEL;
                        }

                        memset(acXpath, 0, sizeof(acXpath));
                        rc = inet_pton(AF_INET6, pcIpTmp, &stIpV6Addr);
                        if (rc > 0) {
                            snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_IPV6, DEF_ACL_SET_KEY_IPV6, iIndexIpv6);
                        } else {
                            LOG_WRITE(EN_LOG_ERROR, "ip error %s, %s", pcIpTmp, strerror(errno));
                            sr_discard_changes(pstSess);
                            rc = SR_ERR_INVAL_ARG;
                            goto ERR_LABEL;
                        }

                        rc = set_string_item(pstSess, acXpath, acBufTmp1, strlen(acBufTmp1));
                        if (rc != SR_ERR_OK) {
                            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        bFlagIp = true;
                    } else if (0 == strcmp(pstLeaf->string, "target")) {
                        memset(acXpath, 0, sizeof(acXpath));
                        snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_ENTRY_ACTION, DEF_ACL_SET_KEY_IPV6, iIndexIpv6);
                        memset(acBufTmp, 0, sizeof(acBufTmp));
                        strcpy(acBufTmp, "openconfig-acl:");
                        if (strlen(pstLeaf->valuestring) > DEF_ACL_IP_MAX_LEN) {
                            LOG_WRITE(EN_LOG_ERROR, "Ipv6 len error: %s", pstLeaf->string);
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        strcat(acBufTmp, pstLeaf->valuestring);
                        rc = set_identityref_item(pstSess, acXpath, acBufTmp, strlen(acBufTmp));
                        if (rc != SR_ERR_OK) {
                            LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
                            sr_discard_changes(pstSess);
                            goto ERR_LABEL;
                        }
                        bFlagTarget = true;
                    } else {
                        LOG_WRITE(EN_LOG_ERROR, "invalid json input: %s", pstLeaf->string);
                        rc = SR_ERR_INVAL_ARG;
                        sr_discard_changes(pstSess);
                        goto ERR_LABEL;
                    }

                    pstLeaf = pstLeaf->next;
                }
                pstAclEntry = pstAclEntry->next;
            }
        } else if (strcmp(pstCurItem->string, "protocol") == 0) {
            /* no checking, do nothing                                          */
        } else {
            LOG_WRITE(EN_LOG_ERROR, "invalid json object");
            rc = SR_ERR_INVAL_ARG;
            sr_discard_changes(pstSess);
            goto ERR_LABEL;
        }

        pstCurItem = pstCurItem->next;
    }

    /* Check whether there is config change or NOT                              */
    if (true == bFlagIp && true == bFlagTarget) {
        /* Commit operation                                                     */
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Commit: %s error: %s", "Reset acl", sr_strerror(rc));
        }
    } else if (false == bFlagIp && false == bFlagTarget) {
        rc = SR_ERR_OK;
    } else {
        rc = SR_ERR_INVAL_ARG;
    }

ERR_LABEL:

    /* Restore configuration                                                    */
    if (rc != SR_ERR_OK) {
        if (true == bRestoreIpv4) {
            if (stAclListIpv4.uiIpv4Cnt > 0) {
                DSC_SetAclList(&stAclListIpv4);
            }
        }

        if (true == bRestoreIpv6) {
            if (stAclListIpv6.uiIpv6Cnt > 0) {
                DSC_SetAclList(&stAclListIpv6);
            }
        }
    }

    DEF_SAFE_FREE(stAclListIpv4.pstIpv4Acl);
    DEF_SAFE_FREE(stAclListIpv6.pstIpv6Acl);
    cJSON_Delete(cjsonAcl);

    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_SetAclIpv6Type, this will set to startup file                                     */
/* Descrp : Set ACL ipv6 type                                                                     */
/* Input  : NONE                                                                                  */
/* Output  : NONE                                                                                 */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg SR_ERR_xxx :                                                                 */
/***FUNC-******************************************************************************************/
int
DSC_SetAclIpv6Type(void)
{
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetStartupSession();

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_TYPE, "user-defined", "openconfig-acl:ACL_IPV6");
    rc = set_identityref_item(pstSess, acXpath, "openconfig-acl:ACL_IPV6", strlen("openconfig-acl:ACL_IPV6"));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
        sr_discard_changes(pstSess);
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_ACL_XPATH_NAME, "user-defined", "openconfig-acl:ACL_IPV6");
    rc = set_string_item(pstSess, acXpath, "user-defined", strlen("user-defined"));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set %s error: %s", acXpath, sr_strerror(rc));
        sr_discard_changes(pstSess);
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Commit error: %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

static int
__DelNode(char *pcXPath, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pcXPath) {
        LOG_WRITE(EN_LOG_ERROR, "pcXPath is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. delete a node through Xpath */
    rc = sr_delete_item(pstSess, pcXPath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete is error,%s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* 3.  commit the changes */
    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
            goto END_LABEL;
        }
    }


END_LABEL:

    return rc;
}

static void
__IpCpy(char *pcSourceIp, char *pcDestIp)
{
    int iMask = 0;
    char *pcToken = NULL;

    if (NULL == pcSourceIp || NULL == pcDestIp) {
        return;
    }

    pcToken = strtok(pcSourceIp, "/");
    strcpy(pcDestIp, pcToken);

    pcToken = NULL;
    pcToken = strtok(NULL, "/");
    if (pcToken != NULL) {
        iMask = atoi(pcToken);
        if (iMask == 32 || iMask == 128) {
            return;
        }

        strcat(pcDestIp, "/");
        strcat(pcDestIp, pcToken);
    }
}

int
DSC_GetAclListByName(char *pacAclName, char *pacAclType, ACL_TABLE_ST *pstAclList)
{
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_data_t *pstSubtrees = NULL;
    size_t uiCnt = 0, uiLeafCnt = 0;
    sr_node_t *apstLeaf[128];
    int rc = SR_ERR_OK;
    pstAclList->uiAclCnt = 0;
    struct ly_set *pstNodeSet = NULL;
    sr_node_t *pstLeaf = NULL;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    char acBuffer[512] = {0};

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_NEW_ACL_LIST, pacAclName, pacAclType);
    rc = get_subtrees(pstSess, acXpath, 0, SR_OPER_DEFAULT, &pstSubtrees);
    if (SR_ERR_OK != rc) {
        if (rc != SR_ERR_NOT_FOUND) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        }
        pstAclList->uiAclCnt = 0;
        goto END_LABEL;
    }

    if (pstSubtrees != NULL) {
        rc = lyd_find_xpath(pstSubtrees->tree, acXpath, &pstNodeSet);
        if (LY_SUCCESS == rc) {
            uiCnt = pstNodeSet->count;
        }
    }
    pstAclList->uiAclCnt = uiCnt;
    for (int i = 0; i < uiCnt; i++) {
        uiLeafCnt = DSC_GetTreeLeaf(pstNodeSet->dnodes[i], apstLeaf);
        for (int j = 0; j < uiLeafCnt; j++) {
            pstLeaf = (sr_node_t *)apstLeaf[j];
            if (0 == strcmp(pstLeaf->schema->name, "sequence-id")) {
                pstAclList->astAclList[i].uiSequenceId = ((sr_node_term_t *)pstLeaf)->value.uint32;
            } else if (0 == strcmp(pstLeaf->schema->name, "source-address")) {
                memset(acBuffer, 0, sizeof(acBuffer));
                strncpy(acBuffer, lyd_get_value(pstLeaf), sizeof(acBuffer) - 1);
                __IpCpy(acBuffer, pstAclList->astAclList[i].acSrcAddr);
            } else if (0 == strcmp(pstLeaf->schema->name, "destination-address")) {
                memset(acBuffer, 0, sizeof(acBuffer));
                strncpy(acBuffer, lyd_get_value(pstLeaf), sizeof(acBuffer) - 1);
                __IpCpy(acBuffer, pstAclList->astAclList[i].acDestAddr);
            } else if (0 == strcmp(pstLeaf->schema->name, "forwarding-action")) {
                strcpy(pstAclList->astAclList[i].acAction, lyd_get_value(pstLeaf));
            } else if (0 == strcmp(pstLeaf->schema->name, "source-port")) {
                pstAclList->astAclList[i].uiSrcPort = ((sr_node_term_t *)pstLeaf)->value.uint16;
            } else if (0 == strcmp(pstLeaf->schema->name, "destination-port")) {
                pstAclList->astAclList[i].uiDestPort = ((sr_node_term_t *)pstLeaf)->value.uint16;
            } else if (0 == strcmp(pstLeaf->schema->name, "protocol")) {
                pstAclList->astAclList[i].uiProtocol = ((sr_node_term_t *)pstLeaf)->value.uint8;

            }
        }
    }

END_LABEL:
    if (NULL != pstNodeSet) {
        ly_set_free(pstNodeSet, NULL);
        pstNodeSet = NULL;
    }

    if (NULL != pstSubtrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    return rc;
}

/*return code: 0 is ipv4, 1 is ipv6*/
static int
__IpType(char *pcIp)
{
    struct in_addr ipv4;
    struct in6_addr ipv6;

    if (inet_pton(AF_INET, pcIp, &ipv4) == 1) {
        return 0;
    } else if (inet_pton(AF_INET6, pcIp, &ipv6) == 1) {
        return 1;
    } else {
        return -1;
    }
}

static void
__AddMask(char *pcIp)
{
    char *pcMask = NULL;
    char acIp[40] = {0};
    strcpy(acIp, pcIp);
    strtok(acIp, "/");

    pcMask = strtok(NULL, "/");
    if (pcMask == NULL) {
        if (__IpType(acIp) == 0) {
            strcat(pcIp, "/32");
        } else if (__IpType(acIp) == 1) {
            strcat(pcIp, "/128");
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Invalid ip source: %s", pcIp);
        }
    }
}

int
DSC_AddAclEntry(char *pacAclName, char *pacAclType, ACL_ENTRY_ST *pstAclEntry, bool bCommit, bool bRefresh)
{
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int rc = SR_ERR_OK;
    int totalCnt = 0;
    ACL_TABLE_ST stAclList = {0};
    uint32_t uiSequenceId = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    ACL_TABLE_ST astAclAll[32];
    size_t uiCnt = 0;
    char acIp[64] = {0};

    memset(&astAclAll, 0, sizeof(astAclAll));

    /* if (bRefresh) { */
    /*     rc = DSC_ManuRefeshSession(); */
    /*     if (rc != SR_ERR_OK) { */
    /*         LOG_WRITE(EN_LOG_ERROR, "DSC_ManuRefeshSession Error: %s", sr_strerror(rc)); */
    /*         goto ERR_LABEL; */
    /*     } */
    /* } */
    rc = DSC_GetAclListByName(pacAclName, pacAclType, &stAclList);
    if ((rc != SR_ERR_OK) && (rc != SR_ERR_NOT_FOUND)) {
        return rc;
    }

    if (rc == SR_ERR_NOT_FOUND) {
        rc = DSC_GetAclListAll(astAclAll, &uiCnt);
        if (rc != SR_ERR_OK) {
            return rc;
        }
        if (uiCnt >= 32) {
            LOG_WRITE(EN_LOG_ERROR, "Acl table max num is 32.");
            return SR_ERR_INVAL_ARG;
        }
    }


    if (stAclList.uiAclCnt >= 128) {
        LOG_WRITE(EN_LOG_ERROR, "Acl entry max num is 128.");
        return SR_ERR_INVAL_ARG;
    }
    rc = DSC_GetAclListAll(astAclAll, &uiCnt);
    if (rc != SR_ERR_OK) {
        return rc;
    }

    for (int i = 0; i < uiCnt; i++) {
        if (0 == strcasecmp(astAclAll[i].acAclName, "internal-access")) {
            continue;
        }
        totalCnt += astAclAll[i].uiAclCnt;
    }

    LOG_WRITE(EN_LOG_NOTICE, "totalCnt is: %d", totalCnt);

    if (totalCnt >= 128) {
        LOG_WRITE(EN_LOG_ERROR, "All Acl entry max num is 128.");
        return SR_ERR_INVAL_ARG;
    }

    uiSequenceId = stAclList.uiAclCnt;

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, DEF_ACL_XPATH_TYPE, pacAclName, pacAclType);
    rc = set_identityref_item(pstSess, acXpath, pacAclType, strlen(pacAclType));
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, DEF_ACL_XPATH_NAME, pacAclName, pacAclType);
    rc = set_string_item(pstSess, acXpath, pacAclName, strlen(pacAclName));
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_ACL_ENTRY_ID, pacAclName, pacAclType, uiSequenceId);
    rc = set_uint32_item(pstSess, acXpath, uiSequenceId);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    if (strstr(pacAclType, "IPV4")) {
        sprintf(acXpath, XPATH_ACL_ENTRY_IPV4, pacAclName, pacAclType, uiSequenceId);
    } else {
        sprintf(acXpath, XPATH_ACL_ENTRY_IPV6, pacAclName, pacAclType, uiSequenceId);
    }

    memset(acIp, 0, sizeof(acIp));
    strcpy(acIp, pstAclEntry->acSrcAddr);
    __AddMask(acIp);
    rc = set_string_item(pstSess, acXpath, acIp, strlen(acIp));
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    if (strlen(pstAclEntry->acDestAddr) > 0) {
        memset(acXpath, 0, sizeof(acXpath));
        if (strstr(pacAclType, "IPV4")) {
            sprintf(acXpath, XPATH_ACL_ENTRY_DESTINATION_IPV4, pacAclName, pacAclType, uiSequenceId);
        } else {
            sprintf(acXpath, XPATH_ACL_ENTRY_DESTINATION_IPV6, pacAclName, pacAclType, uiSequenceId);
        }

        memset(acIp, 0, sizeof(acIp));
        strcpy(acIp, pstAclEntry->acDestAddr);
        __AddMask(acIp);
        rc = set_string_item(pstSess, acXpath, acIp, strlen(acIp));
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }

    if (pstAclEntry->uiProtocol > 0) {
        memset(acXpath, 0, sizeof(acXpath));
        if (strstr(pacAclType, "IPV4")) {
            sprintf(acXpath, XPATH_ACL_ENTRY_IPV4_PROTOCOL, pacAclName, pacAclType, uiSequenceId);
        } else {
            sprintf(acXpath, XPATH_ACL_ENTRY_IPV6_PROTOCOL, pacAclName, pacAclType, uiSequenceId);
        }
        rc = set_uint8_item(pstSess, acXpath, pstAclEntry->uiProtocol);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }


    if (pstAclEntry->uiSrcPort > 0) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, XPATH_ACL_ENTRY_SOURCE_PORT, pacAclName, pacAclType, uiSequenceId);
        rc = set_uint16_item(pstSess, acXpath, pstAclEntry->uiSrcPort);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }

    if (pstAclEntry->uiDestPort > 0) {
        memset(acXpath, 0, sizeof(acXpath));
        sprintf(acXpath, XPATH_ACL_ENTRY_DESTINATION_PORT, pacAclName, pacAclType, uiSequenceId);
        rc = set_uint16_item(pstSess, acXpath, pstAclEntry->uiDestPort);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }

    memset(acXpath, 0, sizeof(acXpath));
    sprintf(acXpath, XPATH_ACL_ENTRY_ACTION, pacAclName, pacAclType, uiSequenceId);
    rc = set_identityref_item(pstSess, acXpath, pstAclEntry->acAction, strlen(pstAclEntry->acAction));
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            goto ERR_LABEL;
        }
    }

    return rc;

ERR_LABEL:
    sr_discard_changes(pstSess);
    LOG_WRITE(EN_LOG_ERROR, "Error : %s", sr_strerror(rc));
    return rc;
}

int
DSC_GetAclListAll(ACL_TABLE_ST *pstAclList, size_t *puiCnt)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    ACL_TABLE_ST *pstList = pstAclList;
    *puiCnt = 0;
    int countname = 0;
    int counttype = 0;
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, "/openconfig-acl:acl/acl-sets//*", &pstValues, &tValuesCount);
    if (SR_ERR_TIME_OUT == rc) {
        LOG_WRITE(EN_LOG_ERROR, "SR_ERR_TIME_OUT: %s", sr_strerror(rc));
        goto cleanup;
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items error: %s", sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, "]/name")) {
            strncpy(pstList->acAclName, pstValues[i].data.string_val, sizeof(pstList->acAclName) - 1);
            pstList++;
            (*puiCnt)++;
            countname++;
        }
    }

    pstList = pstAclList;
    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, "]/type")) {
            strncpy(pstList->acAclType, pstValues[i].data.string_val, sizeof(pstList->acAclType) - 1);
            pstList++;
            counttype++;
        }
    }

    pstList = pstAclList;
    for (int i = 0; i < *puiCnt; i++) {
        DSC_GetAclListByName(pstList->acAclName, pstList->acAclType, pstList);
        pstList++;
    }
cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_DelAclEntry(char *pacAclName, char *pacAclType, uint32_t uiSequenceId)
{
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int rc = SR_ERR_OK;
    ACL_TABLE_ST stAclList = {0};
    int j = 0;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    memset(&stAclList, 0, sizeof(stAclList));

    strcpy(stAclList.acAclName, pacAclName);
    strcpy(stAclList.acAclType, pacAclType);

    /* rc = DSC_ManuRefeshSession(); */
    /* if (rc != SR_ERR_OK) { */
    /*     LOG_WRITE(EN_LOG_ERROR, "DSC_ManuRefeshSession Error: %s", sr_strerror(rc)); */
    /*     goto ERR_LABEL; */
    /* } */

    rc = DSC_GetAclListByName(pacAclName, pacAclType, &stAclList);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    if (uiSequenceId >= stAclList.uiAclCnt) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    sprintf(acXpath, "/openconfig-acl:acl/acl-sets/acl-set[name='%s'][type='%s']", pacAclName, pacAclType);
    rc = __DelNode(acXpath, true);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    for (int i = 0; i < stAclList.uiAclCnt; i++) {
        if (stAclList.astAclList[i].uiSequenceId == uiSequenceId) {
            continue;
        }
        stAclList.astAclList[i].uiSequenceId = j++;
        rc = DSC_AddAclEntry(pacAclName, pacAclType, &(stAclList.astAclList[i]), false, false);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    return rc;

ERR_LABEL:
    sr_discard_changes(pstSess);
    LOG_WRITE(EN_LOG_ERROR, "Error : %s", sr_strerror(rc));
    return rc;
}

int
DSC_DelAclEntryAll(char *pacAclName, char *pacAclType)
{
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int rc = SR_ERR_OK;
    ACL_TABLE_ST stAclList = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    SNMP_USER_LIST_ST stUserAll;

    memset(&stAclList, 0, sizeof(stAclList));

    strcpy(stAclList.acAclName, pacAclName);
    strcpy(stAclList.acAclType, pacAclType);

    /* rc = DSC_ManuRefeshSession(); */
    /* if (rc != SR_ERR_OK) { */
    /*     LOG_WRITE(EN_LOG_ERROR, "DSC_ManuRefeshSession Error: %s", sr_strerror(rc)); */
    /*     goto ERR_LABEL; */
    /* } */

    memset(&stUserAll, 0x00, sizeof(stUserAll));
    rc = DSC_GetUserList(DSC_GetRunningSession(), &stUserAll);
    if (rc != SR_ERR_OK) {
        if (rc != SR_ERR_NOT_FOUND) {
            LOG_WRITE(EN_LOG_ERROR, "Error by  DSC_GetUserList Error: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    for (int i = 0; i < stUserAll.usCount; i++) {
        if (strstr(pacAclType, "ACL_IPV4")) {
            if (strcmp(stUserAll.astList[i].stCommunity.stAclTableIpv4.acAclName, pacAclName) == 0) {
                goto ERR_LABEL;
            }
        } else if (strstr(pacAclType, "ACL_IPV6")) {
            if (strcmp(stUserAll.astList[i].stCommunity.stAclTableIpv6.acAclName, pacAclName) == 0) {
                goto ERR_LABEL;
            }
        } else {
            goto ERR_LABEL;
        }
    }

#if 0
    rc = DSC_GetAclListByName(pacAclName, pacAclType, &stAclList);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }
#endif

    sprintf(acXpath, "/openconfig-acl:acl/acl-sets/acl-set[name='%s'][type='%s']", pacAclName, pacAclType);
    rc = __DelNode(acXpath, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __DelNode Error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit Error: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    return rc;

ERR_LABEL:
    sr_discard_changes(pstSess);
    return SR_ERR_OPERATION_FAILED;
}
