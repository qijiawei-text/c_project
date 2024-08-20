
/**
 * Copyright (c) Oplink Communications, LLC
 */

#include "DSC_snmp.h"

static char *pacGroupList[DEF_GROUP_NAME_LEN] = {
    DSC_SNMP_GROUP_STRING_V1_RO,
    DSC_SNMP_GROUP_STRING_V1_RW,
    DSC_SNMP_GROUP_STRING_V1_TRAP,

    DSC_SNMP_GROUP_STRING_V2_RO,
    DSC_SNMP_GROUP_STRING_V2_RW,
    DSC_SNMP_GROUP_STRING_V2_TRAP,

    DSC_SNMP_GROUP_STRING_V3_RO_NOAUTH_NOPRIV,
    DSC_SNMP_GROUP_STRING_V3_RW_NOAUTH_NOPRIV,
    DSC_SNMP_GROUP_STRING_V3_TRAP_NOAUTH_NOPRIV,

    DSC_SNMP_GROUP_STRING_V3_RO_AUTH_NOPRIV,
    DSC_SNMP_GROUP_STRING_V3_RW_AUTH_NOPRIV,
    DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_NOPRIV,

    DSC_SNMP_GROUP_STRING_V3_RO_AUTH_PRIV,
    DSC_SNMP_GROUP_STRING_V3_RW_AUTH_PRIV,
    DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_PRIV,

};


static void
__VersionConvert(char *pcVersion, uint8_t *pucVersion)
{
    if ( NULL == pcVersion || NULL == pucVersion ) {
        return;
    }

    if (0 == strcmp(pcVersion, "usm")) {
        *pucVersion = EN_SNMP_VER_V3;
    } else if ( 0 == strcmp(pcVersion, "v1")) {
        *pucVersion = EN_SNMP_VER_V1;
    } else if (0 == strcmp(pcVersion, "v2c")) {
        *pucVersion = EN_SNMP_VER_V2;
    }
}


static int
DSC_DelSnmpNode(char *pcXPath)
{

    signed int rc = SR_ERR_OK;
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

END_LABEL:

    return rc;

}

#if 0

static int
__UserNameCompare(sr_node_t *pstNode, DSC_SNMP_USER_ST *pstUser)
{
    sr_node_t *pstLeaf = NULL;

    if ( NULL == pstNode || NULL == pstUser ) {
        return -1;
    }
#if 0
    pstLeaf = (sr_node_t *)pstNode;
    if (0 == strcmp(lyd_get_value(pstLeaf), pstUser->acDestName)) {

        pstNode = pstNode->next;
        pstLeaf = (sr_node_t *)pstNode;
        switch (pstUser->ucVersion) {
        case EN_SNMP_VER_V1:
            return strcmp(lyd_get_value(pstLeaf), DSC_SNMP_V1_STRING);
        case EN_SNMP_VER_V2:
            if (0 == strcmp(lyd_get_value(pstLeaf), DSC_SNMP_V1_STRING)) {
                pstUser->ucVersion = EN_SNMP_VER_V1;
                return 0;
            }
            return strcmp(lyd_get_value(pstLeaf), DSC_SNMP_V2C_STRING);
        case EN_SNMP_VER_V3:
            return strcmp(lyd_get_value(pstLeaf), DSC_SNMP_V3_USM_STRING);

        default:
            return -1;
        }
    }
#endif
    return -1;
}


static int
__GroupIdRangeV1(EN_GROUP_ID *piMin, EN_GROUP_ID *piMax)
{
    if (NULL == piMin || NULL == piMax ) {
        return SR_ERR_INVAL_ARG;
    }

    *piMin = EN_GROUP_ID_V1_RO;
    *piMax = EN_GROUP_ID_V2_TRAP;

    return SR_ERR_OK;
}

static int
__GroupIdRangeV2(EN_GROUP_ID *piMin, EN_GROUP_ID *piMax)
{
    if (NULL == piMin || NULL == piMax ) {
        return SR_ERR_INVAL_ARG;
    }

    *piMin = EN_GROUP_ID_V1_RO;
    *piMax = EN_GROUP_ID_V2_TRAP;

    return SR_ERR_OK;
}


static int
__GroupIdRangeV3(uint8_t ucSecurityLevel, EN_GROUP_ID *piMin, EN_GROUP_ID *piMax)
{
    if (NULL == piMin || NULL == piMax ) {
        return SR_ERR_INVAL_ARG;
    }

    switch (ucSecurityLevel) {
    case EN_SNMP_SECU_AUTH_PRIV:
        *piMin = EN_GROUP_ID_V3_RO_AUTH_PRIV;
        *piMax = EN_GROUP_ID_V3_TRAP_AUTH_PRIV;
        break;

    case EN_SNMP_SECU_AUTH_NOPRIV:
        *piMin = EN_GROUP_ID_V3_RO_AUTH_NOPRIV;
        *piMax = EN_GROUP_ID_V3_TRAP_AUTH_NOPRIV;
        break;

    case EN_SNMP_SECU_NOAUTH_NOPRIV:
        *piMin = EN_GROUP_ID_V3_RO_NOAUTH_NOPRIV;
        *piMax = EN_GROUP_ID_V3_TRAP_NOAUTH_NOPRIV;
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "__GroupIdxRange error. ucSecurityLevel = %d", ucSecurityLevel);
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}


static int
__GroupIdxRange(uint8_t ucVersion, uint8_t ucSecurityLevel, EN_GROUP_ID *piMin, EN_GROUP_ID *piMax)
{
    int rc = SR_ERR_OK;

    if (NULL == piMin || NULL == piMax ) {
        return SR_ERR_INVAL_ARG;
    }

    switch (ucVersion) {
    case EN_SNMP_VER_V1:
        rc = __GroupIdRangeV1(piMin, piMax);
        break;

    case EN_SNMP_VER_V2:
        rc = __GroupIdRangeV2(piMin, piMax);
        break;

    case EN_SNMP_VER_V3:
        rc = __GroupIdRangeV3(ucSecurityLevel, piMin, piMax);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "__GroupIdxRange error. ucVersion = %d, ucSecurityLevel = %d", ucVersion, ucSecurityLevel);
        return SR_ERR_INVAL_ARG;
    }

    return rc;
}



static bool
__FindUserNameInGroup(sr_node_t *pstSubTrees, DSC_SNMP_USER_ST *pstUser)
{
    int j = 0;
    bool rc = true;
    size_t uiCnt;
    sr_node_t *pstNode = NULL;
    struct ly_set *nodeSet = NULL;

    if ( NULL == pstSubTrees || NULL == pstUser ) {
        return false;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, xpath, &nodeSet);
    if (LY_SUCCESS == rc) {
        uiCnt = nodeSet->count;
    }

    for (j = 0; j < uiCnt; ++j) {

        pstNode = nodeSet->dnodes[j];
        pstNode = pstNode->child;

        if (0 == __UserNameCompare(pstNode, pstUser)) {
            goto END_LABEL;
        }
    }

    rc = false;
END_LABEL:
    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    return rc;
}


static EN_GROUP_ID
__GroupIdx(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser,
           EN_GROUP_ID enGroupIdxMin, EN_GROUP_ID enGroupIdxMax)
{
    unsigned int i = 0;
    size_t nCnt = 0;
    int rc = SR_ERR_OK;
    EN_GROUP_ID enGourpId = EN_GROUP_ID_CNT;
    bool bFound = false;
    sr_data_t *pstSubTrees = NULL;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char *pcGroupList[DEF_GROUP_NAME_LEN] = {
        DSC_SNMP_GROUP_STRING_V1_RO,
        DSC_SNMP_GROUP_STRING_V1_RW,
        DSC_SNMP_GROUP_STRING_V1_TRAP,

        DSC_SNMP_GROUP_STRING_V2_RO,
        DSC_SNMP_GROUP_STRING_V2_RW,
        DSC_SNMP_GROUP_STRING_V2_TRAP,

        DSC_SNMP_GROUP_STRING_V3_RO_AUTH_PRIV,
        DSC_SNMP_GROUP_STRING_V3_RW_AUTH_PRIV,
        DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_PRIV,

        DSC_SNMP_GROUP_STRING_V3_RO_AUTH_NOPRIV,
        DSC_SNMP_GROUP_STRING_V3_RW_AUTH_NOPRIV,
        DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_NOPRIV,

        DSC_SNMP_GROUP_STRING_V3_RO_NOAUTH_NOPRIV,
        DSC_SNMP_GROUP_STRING_V3_RW_NOAUTH_NOPRIV,
        DSC_SNMP_GROUP_STRING_V3_TRAP_NOAUTH_NOPRIV,
    };

    if (NULL == pstSess || NULL == pstUser) {
        return EN_GROUP_ID_CNT;
    }


    for (i = enGroupIdxMin; i <= enGroupIdxMax; i++) {
        memset(xpath, 0, sizeof(xpath));
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_LIST_MEMBER, pcGroupList[i]);

        rc = get_subtrees(pstSess, xpath, 0, 0, &pstSubTrees);
        if (SR_ERR_OK != rc) {
            if (SR_ERR_NOT_FOUND == rc) {
                LOG_WRITE(EN_LOG_ERROR, "xpath = %s", xpath);
                continue;
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
                enGourpId = EN_GROUP_ID_CNT;
                goto END_LABLE;
            }
        }

        if (true == __FindUserNameInGroup(pstSubTrees, pstUser)) {

            /* true == bFound, meams user name find in another group before,
               user name MUST NOT be present in more than one group.*/
            if (true == bFound) {
                LOG_WRITE(EN_LOG_ERROR, "%s is present in more than one group.", pstUser->acDestName);
                enGourpId = EN_GROUP_ID_CNT;
                goto END_LABLE;
            } else {
                bFound = true;
                enGourpId = i;
            }
        }

END_LABLE:
        if (pstSubTrees) {
            sr_release_data(pstSubtrees);
            pstSubtrees = NULL;
        }
    }

    return enGourpId;
}

int
DSC_GetAccess(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    EN_GROUP_ID enGroupId = EN_GROUP_ID_CNT;
    EN_GROUP_ID enGroupIdxMin = EN_GROUP_ID_V1_RO, enGroupIdxMax = EN_GROUP_ID_V3_TRAP_NOAUTH_NOPRIV;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = __GroupIdxRange(pstUser->ucVersion, pstUser->ucSecurityLevel, &enGroupIdxMin, &enGroupIdxMax);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    enGroupId = __GroupIdx(pstSess, pstUser, enGroupIdxMin, enGroupIdxMax);

    if (enGroupId >= enGroupIdxMin && enGroupId <= enGroupIdxMax) {
        switch (enGroupId % 3) {
        case 0:
            pstUser->ucAccess = EN_SNMP_ACCESS_MODE_READ_ONLY;
            break;

        case 1:
            pstUser->ucAccess = EN_SNMP_ACCESS_MODE_READ_WRITE;
            break;

        case 2:
            pstUser->ucAccess = EN_SNMP_ACCESS_MODE_TRAP;
            break;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "%s access error. enGroupId = %d", pstUser->acDestName, enGroupId);
        rc = SR_ERR_NOT_FOUND;
    }

    return rc;
}

#endif


int
DSC_GroupV1(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucAccess) {
    case EN_SNMP_ACCESS_MODE_READ_ONLY:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V1_RO);
        break;

    case EN_SNMP_ACCESS_MODE_READ_WRITE:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V1_RW);
        break;

    case EN_SNMP_ACCESS_MODE_TRAP:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V1_TRAP);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

int
DSC_GroupV2(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucAccess) {
    case EN_SNMP_ACCESS_MODE_READ_ONLY:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V2_RO);
        break;

    case EN_SNMP_ACCESS_MODE_READ_WRITE:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V2_RW);
        break;

    case EN_SNMP_ACCESS_MODE_TRAP:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V2_TRAP);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

static int
__GroupV3RO(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucSecurityLevel) {
    case EN_SNMP_SECU_NOAUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RO_NOAUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RO_AUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_PRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RO_AUTH_PRIV);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

static int
__GroupV3RW(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucSecurityLevel) {
    case EN_SNMP_SECU_NOAUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RW_NOAUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RW_AUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_PRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_RW_AUTH_PRIV);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

static int
__GroupV3Trap(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucSecurityLevel) {
    case EN_SNMP_SECU_NOAUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_TRAP_NOAUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_NOPRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_NOPRIV);
        break;

    case EN_SNMP_SECU_AUTH_PRIV:
        strcpy(pcGroup, DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_PRIV);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}


int
DSC_GroupV3(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucAccess) {
    case EN_SNMP_ACCESS_MODE_READ_ONLY:
        __GroupV3RO(pstUser, pcGroup);
        break;

    case EN_SNMP_ACCESS_MODE_READ_WRITE:
        __GroupV3RW(pstUser, pcGroup);
        break;

    case EN_SNMP_ACCESS_MODE_TRAP:
        __GroupV3Trap(pstUser, pcGroup);
        break;

    default:
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

int
DSC_GetGroupName(DSC_SNMP_USER_ST *pstUser, char *pcGroup)
{
    if (NULL == pstUser || NULL == pcGroup) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucVersion) {

    case EN_SNMP_VER_V1:
        DSC_GroupV1(pstUser, pcGroup);
        break;

    case EN_SNMP_VER_V2:
        DSC_GroupV2(pstUser, pcGroup);
        break;

    case EN_SNMP_VER_V3:
        DSC_GroupV3(pstUser, pcGroup);
        break;

    default:
        break;
    }

    return SR_ERR_OK;
}


static int
__SetSecurityName(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_COMMUNITY_SECURITY_NAME, pstUser->acDestName);
    rc = set_string_item(pstSess, xpath, pstUser->acDestName, sizeof(pstUser->acDestName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-security-name: %s\n", sr_strerror(rc));
    }

    return rc;
}

static int
__SetTextName(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_COMMUNITY_TEXT_NAME, pstUser->acDestName);
    rc = set_string_item(pstSess, xpath, pstUser->stCommunity.acTextName, sizeof(pstUser->stCommunity.acTextName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-text-name: %s\n", sr_strerror(rc));
    }

    return rc;
}

static int
__SetSecurityModel(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char acGroupName[DEF_GROUP_NAME_LEN] = {0};

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }
#if 1
    switch (pstUser->ucVersion) {
    case EN_SNMP_VER_V1:
        DSC_GroupV1(pstUser, acGroupName);
        break;

    case EN_SNMP_VER_V2:
        DSC_GroupV2(pstUser, acGroupName);
        break;

    case EN_SNMP_VER_V3:
        DSC_GroupV3(pstUser, acGroupName);
        break;

    default:
        break;
    }
#endif

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_MEMBER_SECURITY_MODEL, acGroupName, pstUser->acDestName);

    rc = set_enum_item(pstSess, xpath, pstUser->acSecurityModel);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-security-model: %s\n", sr_strerror(rc));
    }

    return rc;
}

static int
__SetAuthKey(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->stUsmUser.ucAuthProto) {
    case EN_V3_AUTH_MD5:
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_AUTH_MD5_KEY, pstUser->acDestName);
        break;

    case EN_V3_AUTH_SHA:
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_AUTH_SHA_KEY, pstUser->acDestName);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "pstUser Auth error %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_string_item(pstSess, xpath, pstUser->stUsmUser.acAuthKey, sizeof(pstUser->stUsmUser.acAuthKey));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-auth-proto: %s\n", sr_strerror(rc));
    }

    return rc;
}

static int
__SetPrivKey(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->stUsmUser.ucPrivProto) {
    case EN_V3_PRIV_DES:
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_PRIV_DES_KEY, pstUser->acDestName);
        break;

    case EN_V3_PRIV_AES:
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_PRIV_AES_KEY, pstUser->acDestName);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "pstUser Proto error %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = set_string_item(pstSess, xpath, pstUser->stUsmUser.acPrivKey, sizeof(pstUser->stUsmUser.acPrivKey));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-priv-proto: %s\n", sr_strerror(rc));
    }

    return rc;
}



static int
__SetAuthPrivKey(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    rc = __SetAuthKey(pstSess, pstUser);
    if (rc != SR_ERR_OK) {
        return rc;
    }

    rc = __SetPrivKey(pstSess, pstUser);
    if (rc != SR_ERR_OK) {
        return rc;
    }

    return rc;
}

static int
__SetKey(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstUser->ucSecurityLevel) {
    case EN_SNMP_SECU_AUTH_NOPRIV:
        rc = __SetAuthKey(pstSess, pstUser);
        break;

    case EN_SNMP_SECU_AUTH_PRIV:
        rc = __SetAuthPrivKey(pstSess, pstUser);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "pstUser SecurityLevel error %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    return rc;
}

#if 0
static int
__GetUsmSecLevel(sr_session_ctx_t *pstSess, char *pcUserName, char *pcSecurityLevel, size_t nSize)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    char acSecurityMode[16] = {0};

    if (NULL == pstSess || NULL == pcUserName || NULL == pcSecurityLevel) {
        return SR_ERR_INVAL_ARG;
    }

    char *pcGroupList[DEF_GROUP_NAME_LEN] = {
        DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_PRIV,
        DSC_SNMP_GROUP_STRING_V3_TRAP_AUTH_NOPRIV,
        DSC_SNMP_GROUP_STRING_V3_TRAP_NOAUTH_NOPRIV,
    };

    for (int i = 0; i < 3; ++i) {

        memset(xpath, 0, OPLK_PATH_MAX_LEN);
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_MEMBER_SECURITY_MODEL, pcGroupList[i], pcUserName);

        rc = get_enum_item(pstSess, xpath, acSecurityMode, sizeof(acSecurityMode) - 1);
        if (rc != SR_ERR_OK) {
            if ( SR_ERR_NOT_FOUND == rc ) {
                continue;
            }
            LOG_WRITE(EN_LOG_ERROR, "get_enum_item %s error. %s", xpath, sr_strerror(rc));
            return rc;
        }

        LOG_WRITE(EN_LOG_NOTICE, "acSecurityMode = %s", acSecurityMode);

        memset(xpath, 0, OPLK_PATH_MAX_LEN);
        snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_ACCESS_SECURITY_LEVEL, pcGroupList[i]);
        rc = get_string_item(pstSess, xpath, pcSecurityLevel, nSize);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get_string_item %s error. %s", xpath, sr_strerror(rc));
            return rc;
        }

        LOG_WRITE(EN_LOG_NOTICE, "pcSecurityLevel = %s", pcSecurityLevel);
        break;
    }

    return rc;
}
#endif

static uint16_t __GetSnmpUserCnt(sr_session_ctx_t *pstSess);

int
DSC_AddSnmpUser(DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    DSC_SNMP_USER_ST stOldUser;
    uint16_t usSnmpCurUserCnt = 0;

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    usSnmpCurUserCnt = __GetSnmpUserCnt(pstSess);

    memset(&stOldUser, 0x00, sizeof(DSC_SNMP_USER_ST));
    memcpy(stOldUser.acDestName, pstUser->acDestName, sizeof(stOldUser.acDestName) - 1);
    stOldUser.acDestName[sizeof(stOldUser.acDestName) - 1] = '\0';

    stOldUser.ucVersion = pstUser->ucVersion;
    memcpy(stOldUser.acSecurityModel, pstUser->acSecurityModel, sizeof(stOldUser.acSecurityModel) - 1);
    stOldUser.acSecurityModel[sizeof(stOldUser.acSecurityModel) - 1] = '\0';

    rc = DSC_GetUser(pstSess, &stOldUser);
    if ( SR_ERR_OK == rc) {
        LOG_WRITE(EN_LOG_NOTICE, "delete user %s first.", stOldUser.acDestName);
        DSC_DelSnmpUser(&stOldUser, false);
    } else {
        /* NOT in the current user list, just add a new ONE. */
        if (usSnmpCurUserCnt >= DEF_SNMP_USER_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "Already has %hu user, max: %d", usSnmpCurUserCnt, DEF_SNMP_USER_MAX);
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }
    }

    /* 2.  add or modify a list */
    if (EN_SNMP_VER_V1 == pstUser->ucVersion ||
        EN_SNMP_VER_V2 == pstUser->ucVersion) {

        /* community */
        rc = __SetSecurityName(pstSess, pstUser);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

        rc = __SetTextName(pstSess, pstUser);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

        /* vacm */
        rc = __SetSecurityModel(pstSess, pstUser);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

    } else if (EN_SNMP_VER_V3 == pstUser->ucVersion) {

        /* usm */
        rc = __SetKey(pstSess, pstUser);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

        /*vacm*/
        rc = __SetSecurityModel(pstSess, pstUser);
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }

    } else {
        LOG_WRITE(EN_LOG_ERROR, "pstUser version error %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    return rc;

ERR_LABEL:

    sr_discard_changes(pstSess);
    return rc;
}

static int
__DelCommunity(DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_COMMUNITY_INDEX, pstUser->acDestName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode error %s", sr_strerror(rc));
        return rc;
    }

    return rc;
}

static int
__DelUsm(DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_USER_NAME, pstUser->acDestName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode error %s", sr_strerror(rc));
        return rc;
    }

    return rc;
}


static int
__DelMember(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_MEMBER, pstUser->acGroupName, pstUser->acDestName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode error %s", sr_strerror(rc));
        return rc;
    }

    return rc;
}

#if 0

static int
__DelTargetParam(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (pstUser->ucAccess != EN_SNMP_ACCESS_MODE_TRAP) {
        return SR_ERR_OK;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS, pstUser->acDestName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete target params(%s) error %s", pstUser->acDestName, sr_strerror(rc));
        return rc;
    }

    return rc;
}
#endif

int
DSC_DelSnmpUser(DSC_SNMP_USER_ST *pstUser, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();


    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (EN_SNMP_VER_V1 == pstUser->ucVersion ||
        EN_SNMP_VER_V2 == pstUser->ucVersion) {

        /* community */
        rc = __DelCommunity(pstUser);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "delete user(%s) community.", pstUser->acDestName);
            goto ERR_LABEL;
        }

        /* member */
        rc = __DelMember(pstSess, pstUser);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "delete user(%s) vacm.", pstUser->acDestName);
            goto ERR_LABEL;
        }

    } else if (EN_SNMP_VER_V3 == pstUser->ucVersion) {

        /* usm */
        rc = __DelUsm(pstUser);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "delete user(%s) usm.", pstUser->acDestName);
            goto ERR_LABEL;
        }

        /* member */
        rc = __DelMember(pstSess, pstUser);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "delete user(%s) vacm.", pstUser->acDestName);
            goto ERR_LABEL;
        }

    } else {
        LOG_WRITE(EN_LOG_ERROR, "pstUser version error %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if ( true == bCommit) {
        rc = DSC_Commit(pstSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    return rc;

ERR_LABEL:

    sr_discard_changes(pstSess);
    return rc;
}

#if 0

static void
__GetNameAndPassword(sr_node_t *pstSubTrees, DSC_SNMP_USER_ST *pstUser)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstSubTrees) {
        LOG_WRITE(EN_LOG_ERROR, "pstSubTrees is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    pstNode = pstSubTrees;
    pstNode = pstNode->child;

    while (pstNode) {
        sr_node_t *pstLeaf = NULL;
        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(pstNode->schema->name, SECURITY_NAME_STRING)) {
            strncpy(pstUser->acDestName, lyd_get_value(pstLeaf), sizeof(pstUser->acDestName) - 1);
        } else if (0 == strcmp(pstNode->schema->name, TEXT_NAME_STRING)) {
            strncpy(pstUser->stCommunity.acTextName, lyd_get_value(pstLeaf), sizeof(pstUser->stCommunity.acTextName) - 1);
        }
        pstNode = pstNode->next;
    }

    return;
}

#define INIT_V1V2_USER(stUser)  memset(&stUser, 0, sizeof(stUser)); \
    stUser.ucVersion = EN_SNMP_VER_V2;


static int
__GetV1V2User(sr_session_ctx_t *pstSess, sr_node_t *pstSubTrees, SNMP_USER_LIST_ST *pstUsers)
{
    int rc = SR_ERR_OK;
    DSC_SNMP_USER_ST stUser = {0};

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstSubTrees) {
        LOG_WRITE(EN_LOG_ERROR, "pstSubTrees is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUsers) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    INIT_V1V2_USER(stUser);

    __GetNameAndPassword(pstSubTrees, &stUser);

    /* vacm check the user in which group */
    rc = DSC_GetAccess(pstSess, &stUser);
    if (SR_ERR_OK == rc) {
        if ( pstUsers->usCount + 1 < DEF_SECU_SNMPSERVER_MAX) {
            memcpy(&pstUsers->astList[pstUsers->usCount++], &stUser, sizeof(stUser));
        }
    }

    return rc;
}

int
DSC_GetV1V2UserList(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUsers)
{
    sr_node_t *pstSubTrees = NULL;

    size_t nCnt = 0;
    int rc = SR_ERR_OK;
    unsigned int i = 0;

    /* 1. check parameters */
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUsers) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* 2. get all users */
    /* V1/V2C */
    rc = get_subtrees(pstSess, XPATH_COMMUNITY, 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        /*if (SR_ERR_NOT_FOUND != rc) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
            return SR_ERR_OK;
           }*/

        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto END_LABLE;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_COMMUNITY, &nodeSet);
    if (LY_SUCCESS == rc) {
        nCnt = nodeSet->count;
    }

    for (i = 0; i < nCnt; i++) {
        rc |= __GetV1V2User(pstSess, nodeSet->dnodes[i], pstUsers);
    }

END_LABLE:

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    if (pstSubTrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    return rc;

}

static void
__GetAuthKey(sr_node_t *pstNode, DSC_SNMP_USER_ST *pstUser)
{
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstNode) {
        LOG_WRITE(EN_LOG_ERROR, "pstNode is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    /* key's grandpa is auth */
    pstLeaf = (sr_node_t *)pstNode;
    strncpy(pstUser->stUsmUser.acAuthKey, lyd_get_value(pstLeaf), sizeof(pstUser->stUsmUser.acAuthKey) - 1);

    /* key's parent is ucAuthProto: sha/md5 */
    if (0 == strcmp(pstNode->parent->name, USM_SHA_STRING)) {
        pstUser->stUsmUser.ucAuthProto = EN_V3_AUTH_SHA;
        strcpy(pstUser->stUsmUser.acAuthProto, USM_SHA_STRING);
    } else if (0 == strcmp(pstNode->parent->name, USM_MD5_STRING)) {
        pstUser->stUsmUser.ucAuthProto = EN_V3_AUTH_MD5;
        strcpy(pstUser->stUsmUser.acAuthProto, USM_MD5_STRING);
    }

    return;
}

static void
__GetPrivKey(sr_node_t *pstNode, DSC_SNMP_USER_ST *pstUser)
{
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstNode) {
        LOG_WRITE(EN_LOG_ERROR, "pstNode is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    /* key's grandpa is priv */
    pstLeaf = (sr_node_t *)pstNode;
    strncpy(pstUser->stUsmUser.acPrivKey, lyd_get_value(pstLeaf), sizeof(pstUser->stUsmUser.acPrivKey));

    /* key's parent is ucPrivProto: aes/des */
    if (0 == strcmp(pstNode->parent->name, USM_AES_STRING)) {
        pstUser->stUsmUser.ucPrivProto = EN_V3_PRIV_AES;
        strcpy(pstUser->stUsmUser.acPrivProto, USM_AES_STRING);
    } else if (0 == strcmp(pstNode->parent->name, USM_DES_STRING)) {
        pstUser->stUsmUser.ucPrivProto = EN_V3_PRIV_DES;
        strcpy(pstUser->stUsmUser.acPrivProto, USM_DES_STRING);
    }

    return;
}


static void
__GetKeys(sr_node_t *pstNode, DSC_SNMP_USER_ST *pstUser)
{
    if (NULL == pstNode) {
        LOG_WRITE(EN_LOG_ERROR, "pstNode is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (0 == strcmp(pstNode->parent->parent->name, USM_AUTH_STRING)) {
        __GetAuthKey(pstNode, pstUser);
        LOG_WRITE(EN_LOG_NOTICE, "pstUser->stUsmUser.ucAuthProto = %d", pstUser->stUsmUser.ucAuthProto);
    } else if (0 == strcmp(pstNode->parent->parent->name, USM_PRIV_STRING)) {
        __GetPrivKey(pstNode, pstUser);
        LOG_WRITE(EN_LOG_NOTICE, "pstUser->ucPrivProto = %d", pstUser->stUsmUser.ucPrivProto);
    }

    return;
}

static void
__GetNameAndKey(sr_node_t *pstSubTrees, DSC_SNMP_USER_ST *pstUser)
{
    unsigned int uiLeafCnt = 0;
    sr_node_t *apstLeaf[20];
    sr_node_t *pstLeaf = NULL;

    int i = 0;

    if (NULL == pstSubTrees) {
        LOG_WRITE(EN_LOG_ERROR, "pstSubTrees is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    uiLeafCnt = DSC_GetTreeLeaf(pstSubTrees, apstLeaf);

    for (i = 0; i < uiLeafCnt; i++) {
        pstLeaf = (sr_node_t *)apstLeaf[j];

        if (0 == strcmp(apstLeaf[i]->name, USM_NAME_STRING)) {
            strncpy(pstUser->acDestName, lyd_get_value(pstLeaf), sizeof(pstUser->acDestName) - 1);
        } else if (0 == strcmp(apstLeaf[i]->name, USM_KEY_STRING)) {
            __GetKeys(apstLeaf[i], pstUser);
        }
    }
}

static void
__GetSecurityLevel(DSC_SNMP_USER_ST *pstUser)
{
    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUser is null %d", SR_ERR_INVAL_ARG);
        return;
    }

    if (EN_V3_AUTH_NULL == pstUser->stUsmUser.ucAuthProto && EN_V3_PRIV_NULL == pstUser->stUsmUser.ucPrivProto) {
        pstUser->ucSecurityLevel = EN_SNMP_SECU_NOAUTH_NOPRIV;
        strcpy(pstUser->acSecurityLevel, "noauth");
    } else if (EN_V3_AUTH_NULL != pstUser->stUsmUser.ucAuthProto && EN_V3_PRIV_NULL == pstUser->stUsmUser.ucPrivProto) {
        pstUser->ucSecurityLevel = EN_SNMP_SECU_AUTH_NOPRIV;
        strcpy(pstUser->acSecurityLevel, "authnopriv");
    } else if (EN_V3_AUTH_NULL != pstUser->stUsmUser.ucAuthProto && EN_V3_PRIV_NULL != pstUser->stUsmUser.ucPrivProto) {
        pstUser->ucSecurityLevel = EN_SNMP_SECU_AUTH_PRIV;
        strcpy(pstUser->acSecurityLevel, "authpriv");
    }
}

#define INIT_V3_USER(stUser)  memset(&stUser, 0, sizeof(stUser)); \
    stUser.ucVersion = EN_SNMP_VER_V3;

static int
__GetV3User(sr_session_ctx_t *pstSess, sr_node_t *pstSubTrees, SNMP_USER_LIST_ST *pstUsers)
{
    int rc = SR_ERR_OK;
    DSC_SNMP_USER_ST stUser = {0};

    if (NULL == pstSess || NULL == pstSubTrees || NULL == pstUsers) {
        return SR_ERR_INVAL_ARG;
    }

    INIT_V3_USER(stUser);

    __GetNameAndKey(pstSubTrees, &stUser);
    __GetSecurityLevel(&stUser);

    rc = DSC_GetAccess(pstSess, &stUser);
    if (SR_ERR_OK == rc) {
        if ( pstUsers->usCount + 1 < DEF_SECU_SNMPSERVER_MAX) {
            memcpy(&pstUsers->astList[pstUsers->usCount++], &stUser, sizeof(stUser));
        }
    }

    return rc;
}

int
DSC_GetV3UserList(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUsers)
{
    sr_data_t *pstSubTrees = NULL;
    size_t nCnt = 0;
    int rc = SR_ERR_OK;
    int i = 0;
    struct ly_set *nodeSet = NULL;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUsers) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }


    /* V3 */
    c = get_subtrees(pstSess, XPATH_USM_USER, 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        /*if (SR_ERR_NOT_FOUND != rc) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
            return SR_ERR_OK;
           }*/
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        return rc;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_USM_USER, &nodeSet);
    if (LY_SUCCESS == rc) {
        nCnt = nodeSet->count;
    }

    for (i = 0; i < nCnt; ++i) {
        rc |= __GetV3User(pstSess, nodeSet->dnodes[i], pstUsers);
    }

    if (pstSubTrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }

    return rc;

}


int
DSC_GetSnmpUsers(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUsers)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstUsers) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetV1V2UserList(pstSess, pstUsers);

    rc |= DSC_GetV3UserList(pstSess, pstUsers);

    return rc;
}


static bool
__FindV1V2User(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    sr_node_t *pstNode = NULL, *pstSubTrees = NULL;
    size_t nCnt = 0;
    int i = 0;
    bool bFound = false;
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return false;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return false;
    }

    /* community list */
    rc = get_subtrees(pstSess, XPATH_COMMUNITY, 0, 0, &pstSubTrees);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto END_LABLE;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_COMMUNITY, &nodeSet);
    if (LY_SUCCESS == rc) {
        nCnt = nodeSet->count;
    }

    for (i = 0; i < nCnt; i++) {
        pstNode = nodeSet->dnodes[i];
        pstNode = pstNode->child;

        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(lyd_get_value(pstLeaf), pstUser->acDestName)) {
            pstNode = pstNode->next;
            strncpy(pstUser->stCommunity.acTextName, lyd_get_value(pstLeaf), sizeof(pstUser->stCommunity.acTextName) - 1);

            /* vacm check the user in which group */
            if (SR_ERR_OK != DSC_GetAccess(pstSess, pstUser)) {
                continue;
            }

            bFound = true;
            break;
        }
    }

END_LABLE:

    if (pstSubTrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }


    return bFound;
}


static bool
__FindV3User(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    bool bFound = false;
    sr_data_t *pstSubTrees = NULL;
    size_t nCnt = 0;
    unsigned int uiLeafCnt = 0;
    sr_node_t *apstLeaf[20];
    struct ly_set *nodeSet = NULL;
    sr_node_t *pstLeaf = NULL;

    int i = 0, j = 0;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return false;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return false;
    }


    /* usm list */
    rc = get_subtrees(pstSess, XPATH_USM_USER, 0, 0, &pstSubTrees);
    if ( rc != SR_ERR_OK) {
        if ( rc != SR_ERR_NOT_FOUND ) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        }
        goto END_LABLE;
    }

    rc = lyd_find_xpath(pstSubtrees->tree, XPATH_USM_USER, &nodeSet);
    if (LY_SUCCESS == rc) {
        nCnt = nodeSet->count;
    }

    for (i = 0; i < nCnt; ++i) {
        uiLeafCnt = DSC_GetTreeLeaf(nodeSet->dnodes[i], apstLeaf);

        pstLeaf = (sr_node_t *)apstLeaf[0];
        /* usm name */
        if (0 == strcmp(lyd_get_value(pstLeaf), pstUser->acDestName)) {
            for (j = 1; j < uiLeafCnt; j++) {
                __GetKeys(apstLeaf[j], pstUser);
            }

            __GetSecurityLevel(pstUser);

            LOG_WRITE(EN_LOG_NOTICE, "user(%s) pstUser->ucSecurityLevel = %d.", pstUser->acDestName, pstUser->ucSecurityLevel);

            if (SR_ERR_OK != DSC_GetAccess(pstSess, pstUser)) {
                LOG_WRITE(EN_LOG_ERROR, "Get V3 user:%s access error.", pstUser->acDestName);
                continue;
            }

            bFound = true;
            break;
        }
    }

END_LABLE:

    if (pstSubTrees) {
        sr_release_data(pstSubtrees);
        pstSubtrees = NULL;
    }

    if (nodeSet) {
        ly_set_free(nodeSet, NULL);
    }


    return bFound;

}

bool
DSC_FindSnmpUser(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    bool bFound = false;

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return false;
    }

    if (NULL == pstUser) {
        LOG_WRITE(EN_LOG_ERROR, "pstUsers is null %d", SR_ERR_INVAL_ARG);
        return false;
    }

    strcpy(pstUser->acEngineId, "0x0123456789");

    if (EN_SNMP_VER_V1 == pstUser->ucVersion
        || EN_SNMP_VER_V2 == pstUser->ucVersion) {
        bFound = __FindV1V2User(pstSess, pstUser);
    } else if (EN_SNMP_VER_V3 == pstUser->ucVersion ) {
        bFound = __FindV3User(pstSess, pstUser);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "find v%d user(%s) error.", pstUser->ucVersion, pstUser->acDestName);
    }

    return bFound;
}

#endif

/* target params from subtree */
/* note that this function is highly coupling with MEIWU fmd project. */
static void
__TargetParamsFromSubtree(sr_node_t *pstSubtree, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstSubtree || NULL == pstTargetParams) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: %p, %p", pstSubtree, pstTargetParams);
        goto cleanup;
    }

    /* example:                                                                    */
    /*   /ietf-snmp:snmp/target-params[name='para1']/name = para1                  */
    /*   /ietf-snmp:snmp/target-params[name='para1']/v2c (container)               */
    /*   /ietf-snmp:snmp/target-params[name='para1']/v2c/security-name = v2-trap-1 */

    /* name node */
    pstNode = lyd_node_to_inner(pstSubtree)->child;
    if (NULL == pstNode) {
        goto cleanup;
    }

    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        strncpy(pstTargetParams->acName, lyd_get_value(pstLeaf), sizeof(pstTargetParams->acName) - 1);
    }
    /* version */
    pstNode = pstNode->next;
    if (NULL == pstNode) {
        goto cleanup;
    }

    pstLeaf = (sr_node_t *)pstNode;
    if (pstNode->schema->name != NULL) {
        strncpy(pstTargetParams->acVersion, pstNode->schema->name, sizeof(pstTargetParams->acVersion) - 1);
    }

    __VersionConvert(pstTargetParams->acVersion, &pstTargetParams->ucVersion);

    /* user name */
    pstNode = lyd_node_to_inner(pstNode)->child;
    if (NULL == pstNode) {
        goto cleanup;
    }

    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        strncpy(pstTargetParams->acUserName, lyd_get_value(pstLeaf), sizeof(pstTargetParams->acUserName) - 1);
    }

    /* security level */
    if (0 == strcmp(pstTargetParams->acVersion, "usm")) {
        pstNode = pstNode->next;
        pstLeaf = (sr_node_t *)pstNode;
        if (lyd_get_value(pstLeaf) != NULL) {
            strncpy(pstTargetParams->acSecurityLevel, lyd_get_value(pstLeaf), sizeof(pstTargetParams->acSecurityLevel) - 1);
        }
    } else {
        strcpy(pstTargetParams->acSecurityLevel, "no-auth-no-priv");
    }

    return;
cleanup:
    return;
}

/* get target params by target params name */
static int
__TargetParams(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_data_t *pstTree = NULL;

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS, pstTargetParams->acName);

    rc = sr_get_subtree(pstSess, xpath, SR_OPER_DEFAULT, &pstTree);
    if ( rc != SR_ERR_OK ) {
        LOG_WRITE(EN_LOG_ERROR, "get_subtees error(%s). xpath = %s", sr_strerror(rc), xpath);
        goto cleanup;
    }

    if (pstTree != NULL) {
        __TargetParamsFromSubtree(pstTree->tree, pstTargetParams);
    }

cleanup:
    if (pstTree != NULL) {
        sr_release_data(pstTree);
        pstTree = NULL;
    }

    return rc;

}

/* get target params list */
int
DSC_GetTargetParamsList(sr_session_ctx_t *pstSess, SNMP_TARGET_PARAMS_LIST_ST *pstTargetParamsList)
{
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    sr_data_t *pstSubTrees = NULL;
    DSC_SNMP_TARGET_PARAMS_ST stTargetParams = {0};
    struct ly_set *nodeSet = NULL;

    if (NULL == pstSess || NULL == pstTargetParamsList) {
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(pstSess, TARGET_PARAMS, 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        if ( SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
            return SR_ERR_OK;
        }
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        return rc;
    }

    if (pstSubTrees != NULL) {
        rc = lyd_find_xpath(pstSubTrees->tree, TARGET_PARAMS, &nodeSet);
        if (LY_SUCCESS == rc) {
            nCnt = nodeSet->count;
        }
    }

    for (int i = 0; i < nCnt; ++i) {
        memset(&stTargetParams, 0x00, sizeof(stTargetParams));
        __TargetParamsFromSubtree(nodeSet->dnodes[i], &stTargetParams);
        if ((pstTargetParamsList->usCount + 1) < DEF_SNMP_TARGET_PARAMS_MAX) {
            memcpy(&pstTargetParamsList->astList[pstTargetParamsList->usCount++],
                   &stTargetParams, sizeof(stTargetParams));
        }
    }

    if (pstSubTrees != NULL) {
        sr_release_data(pstSubTrees);
        pstSubTrees = NULL;
    }

    if (nodeSet != NULL) {
        ly_set_free(nodeSet, NULL);
    }

    return rc;
}

int
DSC_DelTargetParams(DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();


    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstTargetParams) {
        LOG_WRITE(EN_LOG_ERROR, "pstTargetParams is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS, pstTargetParams->acName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
        goto ERR_LABEL;
    }


    return rc;

ERR_LABEL:

    sr_discard_changes(pstSess);
    return rc;
}



static int
__AddTargetParamsUsm(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    char *pcSecurityLevel[] = {"-", "no-auth-no-priv", "auth-no-priv", "auth-priv"};

    if (NULL == pstSess || NULL == pstTargetParams) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS_USM_USER_NAME, pstTargetParams->acName);
    rc = set_string_item(pstSess, xpath, pstTargetParams->acUserName, sizeof(pstTargetParams->acUserName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-user-name: %s\n", sr_strerror(rc));
        return rc;
    }

    memset(xpath, 0, sizeof(xpath));
    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS_USM_SECURITY_LEVEL, pstTargetParams->acName);

    rc = set_enum_item(pstSess, xpath, pcSecurityLevel[pstTargetParams->ucSecurityLevel]);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-security-level: %s\n", sr_strerror(rc));
        return rc;
    }

    return rc;
}

static int
__AddTargetParamsV1(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstTargetParams) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS_V1_SECURITY_NAME, pstTargetParams->acName);
    rc = set_string_item(pstSess, xpath, pstTargetParams->acUserName, sizeof(pstTargetParams->acUserName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-security-name: %s\n", sr_strerror(rc));
    }

    return rc;
}

static int
__AddTargetParamsV2C(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstTargetParams) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_PARAMS_V2C_SECURITY_NAME, pstTargetParams->acName);
    rc = set_string_item(pstSess, xpath, pstTargetParams->acUserName, sizeof(pstTargetParams->acUserName));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-security-name: %s\n", sr_strerror(rc));
    }

    return rc;
}


/* create a target params */
static int
__AddTargetParams(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;

    if (NULL == pstSess || NULL == pstTargetParams) {
        return SR_ERR_INVAL_ARG;
    }

    switch (pstTargetParams->ucVersion) {

    case EN_SNMP_VER_V1:
        rc = __AddTargetParamsV1(pstSess, pstTargetParams);
        break;

    case EN_SNMP_VER_V2:
        rc = __AddTargetParamsV2C(pstSess, pstTargetParams);
        break;

    case EN_SNMP_VER_V3:
        rc = __AddTargetParamsUsm(pstSess, pstTargetParams);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "__TargetParam error. ucVersion = %d", pstTargetParams->ucVersion);
        return SR_ERR_INVAL_ARG;

    }

    return rc;

}


int
DSC_AddTargetParams(DSC_SNMP_TARGET_PARAMS_ST *pstTargetParams)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    DSC_SNMP_USER_ST stUser;

    if (NULL == pstSess || NULL == pstTargetParams) {
        return SR_ERR_INVAL_ARG;
    }

#if 0
    rc = __GetUsmSecLevel(pstSess, pstTargetParams->acUserName,
                          pstTargetParams->acSecurityLevel, sizeof(pstTargetParams->acSecurityLevel));
    if ( SR_ERR_OK != rc ) {
        LOG_WRITE(EN_LOG_ERROR, "get trap user(%s) security level error. %s",
                  pstTargetParams->acUserName, sr_strerror(rc));
        goto ERR_LABEL;
    }
#endif
    memset(&stUser, 0x00, sizeof(DSC_SNMP_USER_ST));

    memcpy(stUser.acDestName, pstTargetParams->acUserName, sizeof(stUser.acDestName) - 1);
    stUser.acDestName[sizeof(stUser.acDestName) - 1] = '\0';

    memcpy(stUser.acSecurityModel, pstTargetParams->acSecurityModel, sizeof(stUser.acSecurityModel) - 1);
    stUser.acSecurityModel[sizeof(stUser.acSecurityModel) - 1] = '\0';

    rc = DSC_GetUser(pstSess, &stUser);
    if ( SR_ERR_OK != rc ) {
        LOG_WRITE(EN_LOG_ERROR, "get trap user(%s) security level error. %s",
                  pstTargetParams->acUserName, sr_strerror(rc));
        goto ERR_LABEL;
    }

    pstTargetParams->ucSecurityLevel = stUser.ucSecurityLevel;
    rc = __AddTargetParams(pstSess, pstTargetParams);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "add trap user(%s) target params error. %s",
                  pstTargetParams->acUserName, sr_strerror(rc));
        goto ERR_LABEL;
    }

    /* 3. commit */
    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    return rc;

ERR_LABEL:
    sr_discard_changes(pstSess);
    return rc;
}

static int
__SetTargetIp(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_ST *pstTarget)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstTarget) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_USM_IP, pstTarget->acName);

    rc = set_string_item(pstSess, xpath, pstTarget->acIP, sizeof(pstTarget->acIP));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item error. %s, ip = %s", sr_strerror(rc), pstTarget->acIP);
    }

    return rc;
}

static int
__SetTargetPort(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_ST *pstTarget)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstTarget) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_USM_PORT, pstTarget->acName);
    rc = set_uint16_item(pstSess, xpath, pstTarget->usPort);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-target-port: %s", sr_strerror(rc));
    }

    return rc;
}

static int
__SetTargetParams(sr_session_ctx_t *pstSess, DSC_SNMP_TARGET_ST *pstTarget)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pstTarget) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_TARGET_PARAMS, pstTarget->acName);
    rc = set_string_item(pstSess, xpath, pstTarget->acTargetParams, sizeof(pstTarget->acTargetParams));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set_item-target-params: %s", sr_strerror(rc));
    }

    return rc;
}


int
DSC_AddTarget(DSC_SNMP_TARGET_ST *pstTarget)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pstSess || NULL == pstTarget) {
        return SR_ERR_INVAL_ARG;
    }

    rc = __SetTargetIp(pstSess, pstTarget);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__TargetIp error. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = __SetTargetPort(pstSess, pstTarget);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__TargetPort error. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = __SetTargetParams(pstSess, pstTarget);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "__TargetUser error. %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_Commit error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    return rc;

ERR_LABEL:

    sr_discard_changes(pstSess);
    return rc;

}

static void
__TargetFromSubtree(sr_node_t *pstSubtree, DSC_SNMP_TARGET_ST *pstTarget)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstUdpNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstSubtree || NULL == pstTarget) {
        LOG_WRITE(EN_LOG_ERROR, "Input null: %p, %p", pstSubtree, pstTarget);
        return;
    }

    /* example (NOT support report timeout and retries now):            */
    /*   /ietf-snmp:snmp/target[name='test1']/name = test1              */
    /*   /ietf-snmp:snmp/target[name='test1']/udp (container)           */
    /*   /ietf-snmp:snmp/target[name='test1']/udp/ip = 172.16.166.52    */
    /*   /ietf-snmp:snmp/target[name='test1']/udp/port = 162            */
    /*   /ietf-snmp:snmp/target[name='test1']/target-params = para1     */
    /*   /ietf-snmp:snmp/target[name='test1']/timeout = 1500 [default]  */
    /*   /ietf-snmp:snmp/target[name='test1']/retries = 3 [default]     */

    pstNode = lyd_node_to_inner(pstSubtree)->child;
    while (pstNode) {
        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(pstNode->schema->name, "name")) {
            strncpy(pstTarget->acName, lyd_get_value(pstLeaf), sizeof(pstTarget->acName) - 1);
        } else if (0 == strcmp(pstNode->schema->name, "udp")) {
            pstUdpNode = lyd_node_to_inner(pstNode)->child;
            strncpy(pstTarget->acIP, lyd_get_value(pstUdpNode), sizeof(pstTarget->acIP) - 1);

            pstUdpNode = pstUdpNode->next;
            pstTarget->usPort = (uint16_t)atoi(lyd_get_value(pstUdpNode));
        } else if (0 == strcmp(pstNode->schema->name, "target-params")) {
            strncpy(pstTarget->acTargetParams, lyd_get_value(pstLeaf), sizeof(pstTarget->acTargetParams) - 1);
        }
        pstNode = pstNode->next;
    }

#if 0
    /* name node */
    pstNode = lyd_node_to_inner(pstSubtree)->child;
    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        strncpy(pstTarget->acName, lyd_get_value(pstLeaf), sizeof(pstTarget->acName) - 1);
    }

    /* /udp/ip */
    pstNode = lyd_node_to_inner(pstNode->next)->child;
    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        strncpy(pstTarget->acIP, lyd_get_value(pstLeaf), sizeof(pstTarget->acIP) - 1);
    }

    /* /udp/port */
    pstNode = pstNode->next;
    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        pstTarget->usPort = (uint16_t)atoi(lyd_get_value(pstLeaf));
    }
    /* target params */
    pstNode = pstNode->parent->next;
    pstLeaf = (sr_node_t *)pstNode;
    if (lyd_get_value(pstLeaf) != NULL) {
        strncpy(pstTarget->acTargetParams, lyd_get_value(pstLeaf), sizeof(pstTarget->acTargetParams) - 1);
    }
#endif
    return;

}


int
DSC_GetTargetList(sr_session_ctx_t *pstSess, SNMP_TARGET_LIST_ST *pstTargetAll)
{
    sr_data_t *pstSubTrees = NULL;
    DSC_SNMP_TARGET_ST stTarget = {0};
    DSC_SNMP_TARGET_PARAMS_ST stTargetParams = {0};
    DSC_SNMP_USER_ST stUser = {0};

    size_t nCnt = 0;
    int rc = SR_ERR_OK;
    int i = 0;
    uint16_t usTargetIdx = 0;
    struct ly_set *nodeSet = NULL;

/*    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetTargetList ++"); */

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstTargetAll) {
        LOG_WRITE(EN_LOG_ERROR, "pstTargetAll is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_subtrees(pstSess, XPATH_TARGET, 0, 0, &pstSubTrees);
    if (SR_ERR_OK != rc) {
        if ( SR_ERR_NOT_FOUND == rc) {
            return SR_ERR_OK;
        }
        LOG_WRITE(EN_LOG_ERROR, "get xpath(%s) subtrees error. %s", XPATH_TARGET, sr_strerror(rc));
        return rc;
    }

    if (pstSubTrees != NULL) {
        rc = lyd_find_xpath(pstSubTrees->tree, XPATH_TARGET, &nodeSet);
        if (LY_SUCCESS == rc) {
            nCnt = nodeSet->count;
        }
    }

/*    LOG_WRITE(EN_LOG_NOTICE, "nCnt = %d", nCnt); */
    for (i = 0; i < nCnt; ++i) {
        memset(&stTarget, 0x00, sizeof(stTarget));
        __TargetFromSubtree(nodeSet->dnodes[i], &stTarget);

        /* check target params if exist */
        memset(&stTargetParams, 0x00, sizeof(stTargetParams));
        memcpy(stTargetParams.acName, stTarget.acTargetParams, sizeof(stTargetParams.acName) - 1);
        stTargetParams.acName[sizeof(stTargetParams.acName) - 1] = '\0';

        rc = __TargetParams(pstSess, &stTargetParams);
        if (rc != SR_ERR_OK ) {
            LOG_WRITE(EN_LOG_ERROR, "target params(%s) is not exist.", stTargetParams.acName);
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        memset(&stUser, 0x00, sizeof(DSC_SNMP_USER_ST));
        __VersionConvert(stTargetParams.acVersion, &(stUser.ucVersion));

        /* check user if exist */
        memcpy(stUser.acDestName, stTargetParams.acUserName, sizeof(stUser.acDestName) - 1);
        stUser.acDestName[sizeof(stUser.acDestName) - 1] = '\0';
        memcpy(stUser.acSecurityModel, stTargetParams.acVersion, sizeof(stUser.acSecurityModel) - 1);
        stUser.acSecurityModel[sizeof(stUser.acSecurityModel) - 1] = '\0';

        rc = DSC_GetUser(pstSess, &stUser);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "user(%s) is not exist.", stUser.acDestName);
            rc = SR_ERR_INVAL_ARG;
            goto ERR_LABEL;
        }

        /* copy user to target */
        memcpy(&stTarget.stUser, &stUser, sizeof(stUser));

        if (usTargetIdx >= DEF_SNMP_TARGET_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "user count: %hu out of range(%d)",
                      pstTargetAll->usCount, DEF_SNMP_TARGET_MAX);
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }

        memcpy(&pstTargetAll->astList[usTargetIdx], &stTarget, sizeof(stTarget));
        usTargetIdx++;
        pstTargetAll->usCount = usTargetIdx;

    }

ERR_LABEL:
    if (nodeSet != NULL) {
        ly_set_free(nodeSet, NULL);
        nodeSet = NULL;
    }

    if (pstSubTrees != NULL) {
        sr_release_data(pstSubTrees);
        pstSubTrees = NULL;
    }
/*    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetTargetList --"); */
    return rc;

}


int
DSC_DelSnmpTarget(DSC_SNMP_TARGET_ST *pstTarget)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();


    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "sess is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    if (NULL == pstTarget) {
        LOG_WRITE(EN_LOG_ERROR, "pstTarget is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    snprintf(xpath, OPLK_PATH_MAX_LEN - 1, XPATH_TARGET_NAME, pstTarget->acName);
    rc = DSC_DelSnmpNode(xpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_DelNode error %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
        goto ERR_LABEL;
    }


    return rc;

ERR_LABEL:

    sr_discard_changes(pstSess);
    return rc;
}

static void
__SecurityLevel(uint8_t uiGroupIdx, DSC_SNMP_USER_ST *pstUser)
{
    switch (uiGroupIdx) {
    case EN_GROUP_ID_V3_RO_NOAUTH_NOPRIV:
    case EN_GROUP_ID_V3_RW_NOAUTH_NOPRIV:
    case EN_GROUP_ID_V3_TRAP_NOAUTH_NOPRIV:
        pstUser->ucSecurityLevel = EN_SNMP_SECU_NOAUTH_NOPRIV;
        strcpy(pstUser->acSecurityLevel, "no-auth-no-auth");
        break;

    case EN_GROUP_ID_V3_RO_AUTH_NOPRIV:
    case EN_GROUP_ID_V3_RW_AUTH_NOPRIV:
    case EN_GROUP_ID_V3_TRAP_AUTH_NOPRIV:
        pstUser->ucSecurityLevel = EN_SNMP_SECU_AUTH_NOPRIV;
        strcpy(pstUser->acSecurityLevel, "auth-no-priv");
        break;

    case EN_GROUP_ID_V3_RO_AUTH_PRIV:
    case EN_GROUP_ID_V3_RW_AUTH_PRIV:
    case EN_GROUP_ID_V3_TRAP_AUTH_PRIV:
        pstUser->ucSecurityLevel = EN_SNMP_SECU_AUTH_PRIV;
        strcpy(pstUser->acSecurityLevel, "auth-priv");
        break;

    default:
        pstUser->ucSecurityLevel = EN_SNMP_SECU_NOAUTH_NOPRIV;
        strcpy(pstUser->acSecurityLevel, "no-auth-no-priv");
        break;
    }
}



static void
__Access(uint8_t uiGroupIdx, DSC_SNMP_USER_ST *pstUser)
{
    switch (uiGroupIdx % 3) {
    case 0:
        pstUser->ucAccess = EN_SNMP_ACCESS_MODE_READ_ONLY;
        break;

    case 1:
        pstUser->ucAccess = EN_SNMP_ACCESS_MODE_READ_WRITE;
        break;

    case 2:
        pstUser->ucAccess = EN_SNMP_ACCESS_MODE_TRAP;
        break;
    }
}


static int
__CommunityTree(sr_session_ctx_t *pstSess, char *pcUserName, sr_data_t **ppTree)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == ppTree || NULL == pcUserName) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, OPLK_PATH_MAX_LEN - 1, XPATH_COMMUNITY_INDEX, pcUserName);
    rc = sr_get_subtree(pstSess, acXpath, SR_OPER_DEFAULT, ppTree);
    if (rc != SR_ERR_OK) {
        if (rc != SR_ERR_NOT_FOUND) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        }
        return rc;
    }

    return rc;
}

static void
__Community(sr_node_t *pstTree, COMMUNITY_ST *pstCommunity)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;
    sr_node_t *pstSubNode = NULL;
    sr_node_t *pstSubLeaf = NULL;

    if (NULL == pstTree || NULL == pstCommunity) {
        return;
    }

    pstNode = lyd_node_to_inner(pstTree)->child;

    while (pstNode) {
        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(pstNode->schema->name, SECURITY_NAME_STRING)) {
            strncpy(pstCommunity->acSecurityName, lyd_get_value(pstLeaf), sizeof(pstCommunity->acSecurityName) - 1);

        } else if (0 == strcmp(pstNode->schema->name, TEXT_NAME_STRING)) {
            strncpy(pstCommunity->acTextName, lyd_get_value(pstLeaf), sizeof(pstCommunity->acTextName) - 1);

        } else if (0 == strcmp(pstNode->schema->name, "acl")) {
            pstSubNode = lyd_node_to_inner(pstNode)->child;
            pstSubLeaf = (sr_node_t *)pstSubNode;
            while (pstSubNode) {
                if (0 == strcmp(pstSubNode->schema->name, "acl-ipv4")) {
                    strncpy(pstCommunity->stAclTableIpv4.acAclName, lyd_get_value(pstSubLeaf), sizeof(pstCommunity->stAclTableIpv4.acAclName) - 1);
                    strncpy(pstCommunity->stAclTableIpv4.acAclType, "openconfig-acl:ACL_IPV4", sizeof(pstCommunity->stAclTableIpv4.acAclType) - 1);
                    DSC_GetAclListByName(pstCommunity->stAclTableIpv4.acAclName, "openconfig-acl:ACL_IPV4", &(pstCommunity->stAclTableIpv4));
                } else if (0 == strcmp(pstSubNode->schema->name, "acl-ipv6")) {
                    strncpy(pstCommunity->stAclTableIpv6.acAclName, lyd_get_value(pstSubLeaf), sizeof(pstCommunity->stAclTableIpv6.acAclName) - 1);
                    strncpy(pstCommunity->stAclTableIpv6.acAclType, "openconfig-acl:ACL_IPV6", sizeof(pstCommunity->stAclTableIpv6.acAclType) - 1);
                    DSC_GetAclListByName(pstCommunity->stAclTableIpv6.acAclName, "openconfig-acl:ACL_IPV6", &(pstCommunity->stAclTableIpv6));
                }
                pstSubNode = pstSubNode->next;
            }
        }
        pstNode = pstNode->next;
    }
}


#if 0
static int
__CommunityList(sr_session_ctx_t *pstSess, COMMUNITY_LIST_ST *pstCommunityList)
{
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    sr_node_t *pstTrees = NULL;
    COMMUNITY_ST stCommunity;


    LOG_WRITE(EN_LOG_NOTICE, "__CommunityList ++");

    rc = get_subtrees(pstSess, XPATH_COMMUNITY, SR_GET_SUBTREE_DEFAULT, &pstTrees, &nCnt);
    if (rc != SR_ERR_OK) {
        if (rc == SR_ERR_NOT_FOUND) {
            goto END_LABLE;
        }
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto ERR_LABLE;
    }

    for (int i = 0; i < nCnt; ++i) {
        memset(&stCommunity, 0x00, sizeof(stCommunity));
        __Community(pstTrees + i, &stCommunity);

        if (pstCommunityList->usCount < DEF_SNMP_USER_MAX) {
            memcpy(&(pstCommunityList->astList[pstCommunityList->usCount++]), &stCommunity, sizeof(stCommunity));
        }
    }

    sr_release_data(pstSubtrees);
    pstSubtrees = NULL;

END_LABLE:

    LOG_WRITE(EN_LOG_NOTICE, "__CommunityList --");
    return rc;

ERR_LABLE:

    LOG_WRITE(EN_LOG_ERROR, "__CommunityList --");
    return rc;
}

#endif

static void
__Auth(sr_node_t *pstTree, USM_USER_ST *pstUsmUser)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstTree || NULL == pstUsmUser) {
        return;
    }

    pstNode = lyd_node_to_inner(pstTree)->child;

    /* key's grandpa is auth */
    if (0 == strcmp(pstNode->schema->name, USM_SHA_STRING)) {
        pstUsmUser->ucAuthProto = EN_V3_AUTH_SHA;
        strcpy(pstUsmUser->acAuthProto, USM_SHA_STRING);
    } else if (0 == strcmp(pstNode->schema->name, USM_MD5_STRING)) {
        pstUsmUser->ucAuthProto = EN_V3_AUTH_MD5;
        strcpy(pstUsmUser->acAuthProto, USM_MD5_STRING);
    }
    /* LOG_WRITE(EN_LOG_NOTICE, "pstUsmUser->acAuthProto = %s", pstUsmUser->acAuthProto); */

    pstNode = lyd_node_to_inner(pstNode)->child;
    pstLeaf = (sr_node_t *)pstNode;
    strncpy(pstUsmUser->acAuthKey, lyd_get_value(pstLeaf), sizeof(pstUsmUser->acAuthKey) - 1);
    /* (EN_LOG_NOTICE, "pstUsmUser->acAuthKey = %s", pstUsmUser->acAuthKey); */
}

static void
__Priv(sr_node_t *pstTree, USM_USER_ST *pstUsmUser)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstTree || NULL == pstUsmUser) {
        return;
    }

    pstNode = lyd_node_to_inner(pstTree)->child;


    /* key's grandpa is auth */
    if (0 == strcmp(pstNode->schema->name, USM_DES_STRING)) {
        pstUsmUser->ucPrivProto = EN_V3_PRIV_DES;
        strcpy(pstUsmUser->acPrivProto, USM_DES_STRING);
    } else if (0 == strcmp(pstNode->schema->name, USM_AES_STRING)) {
        pstUsmUser->ucPrivProto = EN_V3_PRIV_AES;
        strcpy(pstUsmUser->acPrivProto, USM_AES_STRING);
    }
    /* LOG_WRITE(EN_LOG_NOTICE, "pstUsmUser->acPrivProto = %s", pstUsmUser->acPrivProto); */

    pstNode = lyd_node_to_inner(pstNode)->child;
    pstLeaf = (sr_node_t *)pstNode;
    strncpy(pstUsmUser->acPrivKey, lyd_get_value(pstLeaf), sizeof(pstUsmUser->acPrivKey) - 1);
    /* LOG_WRITE(EN_LOG_NOTICE, "pstUsmUser->acPrivKey = %s", pstUsmUser->acPrivKey); */

}


static void
__UsmUser(sr_node_t *pstTree, USM_USER_ST *pstUsmUser)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstTree || NULL == pstUsmUser) {
        return;
    }

/*    LOG_WRITE(EN_LOG_ERROR, "__UsmUser ++"); */

    pstNode = lyd_node_to_inner(pstTree)->child;

    while (pstNode) {
        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(pstNode->schema->name, USM_NAME_STRING)) {
            strncpy(pstUsmUser->acName, lyd_get_value(pstLeaf), sizeof(pstUsmUser->acName) - 1);
            /* LOG_WRITE(EN_LOG_NOTICE, "pstUsmUser->acName = %s", pstUsmUser->acName); */
        } else if (0 == strcmp(pstNode->schema->name, USM_AUTH_STRING)) {
            __Auth(pstNode, pstUsmUser);
        } else if (0 == strcmp(pstNode->schema->name, USM_PRIV_STRING)) {
            __Priv(pstNode, pstUsmUser);
        }
        pstNode = pstNode->next;
    }

/*    LOG_WRITE(EN_LOG_ERROR, "__UsmUser --"); */
}


#if 0
static int
__UsmUserList(sr_session_ctx_t *pstSess, USM_USER_LIST_ST *pstUsmUserList)
{
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    sr_node_t *pstTrees = NULL;
    USM_USER_ST stUsmUser = {0};

    LOG_WRITE(EN_LOG_NOTICE, "__UsmUserList ++");

    rc = get_subtrees(pstSess, XPATH_USM_USER, SR_GET_SUBTREE_DEFAULT, &pstTrees, &nCnt);
    if (rc != SR_ERR_OK) {
        if (rc == SR_ERR_NOT_FOUND) {
            goto END_LABLE;
        }
        LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        goto ERR_LABLE;
    }

    for (int i = 0; i < nCnt; ++i) {
        memset(&stUsmUser, 0x00, sizeof(stUsmUser));
        __UsmUser(pstTrees + i, &stUsmUser);

        if (pstUsmUserList->usCount < DEF_SNMP_USER_MAX) {
            memcpy(&(pstUsmUserList->astList[pstUsmUserList->usCount++]), &stUsmUser, sizeof(stUsmUser));
        }
    }

    sr_release_data(pstSubtrees);
    pstSubtrees = NULL;


END_LABLE:

    LOG_WRITE(EN_LOG_NOTICE, "__UsmUserList --");
    return rc;

ERR_LABLE:

    LOG_WRITE(EN_LOG_ERROR, "__UsmUserList --");
    return rc;

}

#endif

static void
__Member(sr_node_t *pstTree, MEMBER_ST *pstMember)
{
    sr_node_t *pstNode = NULL;
    sr_node_t *pstLeaf = NULL;

    if (NULL == pstTree || NULL == pstMember) {
        return;
    }

/*    LOG_WRITE(EN_LOG_ERROR, "__Member ++"); */

    pstNode = lyd_node_to_inner(pstTree)->child;

    while (pstNode) {
        pstLeaf = (sr_node_t *)pstNode;
        if (0 == strcmp(pstNode->schema->name, SECURITY_NAME_STRING)) {
            strncpy(pstMember->acSecurityName, lyd_get_value(pstLeaf), sizeof(pstMember->acSecurityName) - 1);
        } else if (0 == strcmp(pstNode->schema->name, SECURITY_MODEL_STRING)) {
            strncpy(pstMember->acSecurityModel, lyd_get_value(pstLeaf), sizeof(pstMember->acSecurityModel) - 1);
        }
        pstNode = pstNode->next;
    }


/*    LOG_WRITE(EN_LOG_ERROR, "__Member --"); */
}

static int
__MemberTrees(sr_session_ctx_t *pstSess, char *pcGroup, sr_data_t **ppTrees)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pcGroup || NULL == ppTrees) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, OPLK_PATH_MAX_LEN - 1, XPATH_VACM_GROUP_LIST_MEMBER, pcGroup);

    rc = get_subtrees(pstSess, acXpath, 0, 0, ppTrees);
    if (rc != SR_ERR_OK) {
        if (rc != SR_ERR_NOT_FOUND) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        }
        return rc;
    }

    return rc;
}

static int
__UsmUserTree(sr_session_ctx_t *pstSess, char *pcUserName, sr_data_t **ppTree)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == ppTree || NULL == pcUserName) {
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, OPLK_PATH_MAX_LEN - 1, XPATH_USM_USER_NAME, pcUserName);
    rc = sr_get_subtree(pstSess, acXpath, SR_OPER_DEFAULT, ppTree);
    if (rc != SR_ERR_OK) {
        if (rc != SR_ERR_NOT_FOUND) {
            LOG_WRITE(EN_LOG_ERROR, "get_subtrees error: %s", sr_strerror(rc));
        }
        return rc;
    }

    return rc;
}



static int
__GetUser(sr_session_ctx_t *pstSess, uint8_t uiGroupIdx, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_OK;
    COMMUNITY_ST stCommunity;
    USM_USER_ST stUsmUser;
    sr_data_t *pstTree = NULL;

    if ( NULL == pstSess || NULL == pstUser) {
        return SR_ERR_INVAL_ARG;
    }

/*    LOG_WRITE(EN_LOG_NOTICE, "__GetUser(%s:%s) ++", pstUser->acDestName, pstUser->acSecurityModel); */

    strcpy(pstUser->acEngineId, "0x0123456789");
    strncpy(pstUser->acGroupName, pacGroupList[uiGroupIdx], sizeof(pstUser->acGroupName) - 1);
    __VersionConvert(pstUser->acSecurityModel, &(pstUser->ucVersion));
    __Access(uiGroupIdx, pstUser);
    __SecurityLevel(uiGroupIdx, pstUser);

/*    LOG_WRITE(EN_LOG_NOTICE, "stUser.acGroupName = %s", pstUser->acGroupName); */
/*    LOG_WRITE(EN_LOG_NOTICE, "stUser.ucAccess = %d", pstUser->ucAccess); */
/*    LOG_WRITE(EN_LOG_NOTICE, "stUser.ucSecurityLevel = %d", pstUser->ucSecurityLevel); */

    memset(&stUsmUser, 0x00, sizeof(stUsmUser));

    if ( 0 == strcmp(pstUser->acSecurityModel, DSC_SNMP_V3_USM_STRING)) {
        rc = __UsmUserTree(pstSess, pstUser->acDestName, &pstTree);
        if (SR_ERR_OK == rc) {

            __UsmUser(pstTree->tree, &stUsmUser);
            memcpy(&(pstUser->stUsmUser), &stUsmUser, sizeof(pstUser->stUsmUser));
        }
    } else {
        memset(&stCommunity, 0x00, sizeof(stCommunity));
        rc = __CommunityTree(pstSess, pstUser->acDestName, &pstTree);
        if (SR_ERR_OK == rc) {

            __Community(pstTree->tree, &stCommunity);
            memcpy(&(pstUser->stCommunity), &stCommunity, sizeof(pstUser->stCommunity));
        }
    }

    sr_release_data(pstTree);
    pstTree = NULL;
    /* LOG_WRITE(EN_LOG_NOTICE, "__GetUser(%s:%s) --", pstUser->acDestName, pstUser->acSecurityModel); */

    return rc;
}



int
DSC_GetUserList(sr_session_ctx_t *pstSess, SNMP_USER_LIST_ST *pstUserList)
{
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    sr_data_t *pstTrees = NULL;
    struct ly_set *nodeSet = NULL;

    DSC_SNMP_USER_ST stUser = {0};
    MEMBER_ST stMember;

#if 0
    COMMUNITY_LIST_ST stCommunityList;
    USM_USER_LIST_ST stUsmUserList;

    memset(&stCommunityList, 0x00, sizeof(stCommunityList));
    __CommunityList(pstSess, &stCommunityList);

    memset(&stUsmUserList, 0x00, sizeof(stUsmUserList));
    __UsmUserList(pstSess, &stUsmUserList);
#endif

    for (int i = 0; i < EN_GROUP_ID_CNT; ++i) {
        rc = __MemberTrees(pstSess, pacGroupList[i], &pstTrees);
        if (rc != SR_ERR_OK) {
            continue;
        }

        nCnt = 0;
        if (pstTrees != NULL) {
            rc = lyd_find_xpath(pstTrees->tree, "/ietf-snmp:snmp/vacm/group/member", &nodeSet);
            if (LY_SUCCESS == rc) {
                nCnt = nodeSet->count;
            }
        }

        for (int j = 0; j < nCnt; ++j) {
            memset(&stMember, 0x00, sizeof(stMember));
            __Member(nodeSet->dnodes[j], &stMember);

            memset(&stUser, 0x00, sizeof(stUser));
            strncpy(stUser.acDestName, stMember.acSecurityName, sizeof(stUser.acDestName));
            memcpy(stUser.acSecurityModel, stMember.acSecurityModel, sizeof(stUser.acSecurityModel) - 1);
            stUser.acSecurityModel[sizeof(stUser.acSecurityModel) - 1] = '\0';

            rc = __GetUser(pstSess, i, &stUser);

            if ((SR_ERR_OK == rc) && (pstUserList->usCount < DEF_SNMP_USER_MAX)) {
                memcpy(&(pstUserList->astList[pstUserList->usCount++]), &stUser, sizeof(stUser));
            }
        }

        if (nodeSet != NULL) {
            ly_set_free(nodeSet, NULL);
            nodeSet = NULL;
        }

        if (pstTrees != NULL) {
            sr_release_data(pstTrees);
            pstTrees = NULL;
        }
    }

    return rc;
}

static uint16_t
__GetSnmpUserCnt(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    size_t nCnt = 0;
    sr_data_t *pstTrees = NULL;
    struct ly_set *nodeSet = NULL;

    DSC_SNMP_USER_ST stUser = {0};
    MEMBER_ST stMember;
    uint16_t usUserCnt = 0;

    for (int i = 0; i < EN_GROUP_ID_CNT; ++i) {
        rc = __MemberTrees(pstSess, pacGroupList[i], &pstTrees);
        if (rc != SR_ERR_OK) {
            continue;
        }

        nCnt = 0;
        if (pstTrees != NULL) {
            rc = lyd_find_xpath(pstTrees->tree, "/ietf-snmp:snmp/vacm/group/member", &nodeSet);
            if (LY_SUCCESS == rc) {
                nCnt = nodeSet->count;
            }
        }

        for (int j = 0; j < nCnt; ++j) {
            memset(&stMember, 0x00, sizeof(stMember));
            __Member(nodeSet->dnodes[j], &stMember);

            memset(&stUser, 0x00, sizeof(stUser));
            strncpy(stUser.acDestName, stMember.acSecurityName, sizeof(stUser.acDestName));
            memcpy(stUser.acSecurityModel, stMember.acSecurityModel, sizeof(stUser.acSecurityModel) - 1);
            stUser.acSecurityModel[sizeof(stUser.acSecurityModel) - 1] = '\0';

            rc = __GetUser(pstSess, i, &stUser);
            if (SR_ERR_OK == rc) {
                usUserCnt++;
            }
        }

        if (nodeSet != NULL) {
            ly_set_free(nodeSet, NULL);
            nodeSet = NULL;
        }

        if (pstTrees != NULL) {
            sr_release_data(pstTrees);
            pstTrees = NULL;
        }
    }

    return usUserCnt;
}


static int
__UserNameCompare(MEMBER_ST *pstMember, DSC_SNMP_USER_ST *pstUser)
{
    if (NULL == pstMember || NULL == pstUser) {
        return -1;
    }

    if (0 == strcmp(pstMember->acSecurityName, pstUser->acDestName)
        && 0 == strcmp(pstMember->acSecurityModel, pstUser->acSecurityModel)) {
        return 0;
    }

    return -1;
}


int
DSC_GetUser(sr_session_ctx_t *pstSess, DSC_SNMP_USER_ST *pstUser)
{
    int rc = SR_ERR_NOT_FOUND;
    size_t nCnt = 0;
    sr_data_t *pstTrees = NULL;
    struct ly_set *nodeSet = NULL;

    MEMBER_ST stMember;


/*    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetUser ++"); */

    for (int i = 0; i < EN_GROUP_ID_CNT; ++i) {
        rc = __MemberTrees(pstSess, pacGroupList[i], &pstTrees);
        if (rc != SR_ERR_OK) {
            continue;
        }

        if (NULL == pstTrees) {
            continue;
        }

        nCnt = 0;
        rc = lyd_find_xpath(pstTrees->tree, "/ietf-snmp:snmp/vacm/group/member", &nodeSet);
        if (LY_SUCCESS == rc) {
            nCnt = nodeSet->count;
        }

        for (int j = 0; j < nCnt; ++j) {
            memset(&stMember, 0x00, sizeof(stMember));
            __Member(nodeSet->dnodes[j], &stMember);


/*            LOG_WRITE(EN_LOG_NOTICE, "stMember.acSecurityName = %s", stMember.acSecurityName); */
/*            LOG_WRITE(EN_LOG_NOTICE, "stMember.acSecurityModel = %s", stMember.acSecurityModel); */

/*            LOG_WRITE(EN_LOG_NOTICE, "pstUser->acDestName = %s", pstUser->acDestName); */
/*            LOG_WRITE(EN_LOG_NOTICE, "pstUser->acSecurityModel = %s", pstUser->acSecurityModel); */


            if (0 == __UserNameCompare(&stMember, pstUser)) {
                rc = __GetUser(pstSess, i, pstUser);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_NOTICE, "Get user(%s) error.", pstUser->acDestName);
                }

                if (pstTrees != NULL) {
                    sr_release_data(pstTrees);
                    pstTrees = NULL;
                }

/*                LOG_WRITE(EN_LOG_NOTICE, "DSC_GetUser --"); */
                return rc;
            }
        }

        if (pstTrees != NULL) {
            sr_release_data(pstTrees);
            pstTrees = NULL;
        }
    }

    rc = SR_ERR_NOT_FOUND;
    LOG_WRITE(EN_LOG_NOTICE, "user(%s) is not exist.", pstUser->acDestName);
/*    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetUser --"); */

    return rc;
}

int
DSC_AddAcl(DSC_SNMP_USER_ST *pstUser, char *pcAclName, char *pcAclType)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    memset(xpath, 0, sizeof(xpath));
    if (strstr(pcAclType, "IPV4")) {
        snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_COMMUNITY_ACL_IPV4, pstUser->acDestName);
    } else {
        snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_COMMUNITY_ACL_IPV6, pstUser->acDestName);
    }
    rc = set_string_item(pstSess, xpath, pcAclName, strlen(pcAclName));
    if (rc != SR_ERR_OK) {
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }
    return rc;
}

int
DSC_DelAcl(DSC_SNMP_USER_ST *pstUser, char *pcAclName, char *pcAclType)
{
    int rc = SR_ERR_OK;
    char xpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    memset(xpath, 0, sizeof(xpath));
    if (strstr(pcAclType, "IPV4")) {
        snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_COMMUNITY_ACL_IPV4, pstUser->acDestName);
    } else {
        snprintf(xpath, OPLK_PATH_MAX_LEN, XPATH_COMMUNITY_ACL_IPV6, pstUser->acDestName);
    }
    rc = sr_delete_item(pstSess, xpath, SR_EDIT_STRICT);
    if (rc != SR_ERR_OK) {
        return rc;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

    return rc;
}
