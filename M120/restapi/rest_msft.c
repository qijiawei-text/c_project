/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_msft.c
* Description:   only used for MSFT, parse requested json data and
*                      get business data for reponsing http/https request
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/17  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "util_inc.h"
#include "rest_edfa.h"


int
REST_ParseSystem(const cJSON *pstJson, REST_SYSTEM_ST *pstSystem)
{
    REST_SYSTEM_ST stSystem;
    char aacFiberTypeStr[2][32] = {{0}, {0}};
    char aacLongSpanStr[2][32] = {{0}, {0}};
    FIBER_TYPE_EN enFiberType = FIBER_TYPE_CNT;
    int i = 0;
    JSON_OBJ_ST astJsonObj[] = {{STR_NODE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acNodeName), sizeof(stSystem.acNodeName)},
                                {STR_IP_ADDRESS, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIPV4), sizeof(stSystem.acIPV4)},
                                {STR_NETMASK, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acNetmask), sizeof(stSystem.acNetmask)},
                                {STR_GATEWAY, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acGatewayV4), sizeof(stSystem.acGatewayV4)},
                                {STR_NTP_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacNtpIp[0]), sizeof(stSystem.aacNtpIp[0])},
                                {STR_NTP_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacNtpIp[1]), sizeof(stSystem.aacNtpIp[1])},
                                {STR_SYSLOG_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacSyslogIp[0]), sizeof(stSystem.aacSyslogIp[0])},
                                {STR_SYSLOG_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacSyslogIp[1]), sizeof(stSystem.aacSyslogIp[1])},
                                {STR_TACACS_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[0].acIp), sizeof(stSystem.astTacacsServer[0].acIp)},
                                {STR_TACACS_KEY_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[0].acKey), sizeof(stSystem.astTacacsServer[0].acKey)},
                                /*{STR_TACACS_PORT_1, EN_DATA_TYPE_UINT, TAKE_VOID_ADDRESS(stSystem.stTacacsServer.uiPort), sizeof(stSystem.stTacacsServer.uiPort)},*/ /*fixed port: 49*/
                                {STR_TACACS_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[1].acIp), sizeof(stSystem.astTacacsServer[1].acIp)},
                                {STR_TACACS_KEY_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[1].acKey), sizeof(stSystem.astTacacsServer[1].acKey)},
#if defined(__OCS__)

#else
                                /* auto control parameters of working line */
                                {STR_FIBER_TYPE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacFiberTypeStr[0]), sizeof(aacFiberTypeStr[0])},
                                {STR_LAUNCHING_POWER, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adLauchingPower[0]), sizeof(stSystem.adLauchingPower[0])},
                                {STR_DROP_POWER_PER_CHANNEL, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adDropPower[0]), sizeof(stSystem.adDropPower[0])},
                                {STR_NO_OF_CARRIERS, EN_DATA_TYPE_UCHAR, TAKE_VOID_ADDRESS(stSystem.aucCarries[0]), sizeof(stSystem.aucCarries[0])},
                                {STR_LONG_SPAN, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacLongSpanStr[0]), sizeof(aacLongSpanStr[0])},
#if defined(__OLSP__)
                                /* auto control parameters of protect line */
                                {STR_FIBER_TYPE_P, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacFiberTypeStr[1]), sizeof(aacFiberTypeStr[1])},
                                {STR_LAUNCHING_POWER_P, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adLauchingPower[1]), sizeof(stSystem.adLauchingPower[1])},
                                {STR_DROP_POWER_PER_CHANNEL_P, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adDropPower[1]), sizeof(stSystem.adDropPower[1])},
                                {STR_NO_OF_CARRIERS_P, EN_DATA_TYPE_UCHAR, TAKE_VOID_ADDRESS(stSystem.aucCarries[1]), sizeof(stSystem.aucCarries[1])},
                                {STR_LONG_SPAN_P, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacLongSpanStr[1]), sizeof(aacLongSpanStr[1])},
#endif
#endif
                                {STR_TIMEZONE, EN_DATA_TYPE_SHORT, TAKE_VOID_ADDRESS(stSystem.sTimezone), sizeof(stSystem.sTimezone)},
                                {STR_IPV6_MODE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIpv6Mode), sizeof(stSystem.acIpv6Mode)},
                                {STR_IPV6_ADDRESS, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIPv6), sizeof(stSystem.acIPv6)},
                                {STR_IPV6_GATEWAY, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acGatewayV6), sizeof(stSystem.acGatewayV6)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stSystem, 0, sizeof(stSystem));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

#if defined(__OCS__)
    /* clear compile waring */
    memset(aacFiberTypeStr, 0, sizeof(aacFiberTypeStr));
    memset(aacLongSpanStr, 0, sizeof(aacLongSpanStr));
    enFiberType = enFiberType;
    i = i;
#else
    /* parse auto control parameters */
    for (i = 0; i < REST_GetLinePathMax(); i++) {
        enFiberType = DSC_FiberTypeToEnum(aacFiberTypeStr[i]);
        if (FIBER_TYPE_CNT == enFiberType) {
            LOG_WRITE(EN_LOG_ERROR, "Unknown fiber-type[%d]:%s", i, aacFiberTypeStr[i]);
            goto ERR_LABEL;
        }

        stSystem.aenFiberType[i] = enFiberType;

        if (0 == strcmp(aacLongSpanStr[i], STR_ENABLE)) {
            stSystem.abLongSpan[i] = true;
        } else if (0 == strcmp(aacLongSpanStr[i], STR_DISABLE)) {
            stSystem.abLongSpan[i] = false;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Invalid long-span[%d]:%s", i, aacLongSpanStr[i]);
            goto ERR_LABEL;
        }
    }
#endif

    memcpy(pstSystem, &stSystem, sizeof(stSystem));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_SetSystem(REST_SYSTEM_ST *pstSystem)
{
    int rc = SR_ERR_OK;
    REST_SYSTEM_FLAG_ST stExistFlag;

    memset(&stExistFlag, 0, sizeof(stExistFlag));

    if (OPLK_OK != REST_SetEachFlag(&stExistFlag, sizeof(stExistFlag), true)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = REST_SetSystemOptional(pstSystem, &stExistFlag);

END_LABEL:

    return rc;
}

int
REST_GetSystem(REST_SYSTEM_ST *pstSystem)
{
    int rc = SR_ERR_OK;
    REST_SYSTEM_FLAG_ST stExistFlag;

    memset(&stExistFlag, 0, sizeof(stExistFlag));

    if (OPLK_OK != REST_SetEachFlag(&stExistFlag, sizeof(stExistFlag), true)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = REST_GetSystemOptional(pstSystem, &stExistFlag);

END_LABEL:

    return rc;
}

int
REST_ParseFullConfig(const cJSON *pstJson, REST_FULLCONFIG_ST *pstFullConfig)
{
    cJSON *pstObjJson = NULL;
    REST_AMP_CARD_TYPE_EN enCardType = EN_REST_AMP_CARD_MAX;
    char *pcEdfaNodeName = NULL;

    /*parse "system" node*/
    pstObjJson = cJSON_GetObjectItem(pstJson, STR_SYSTEM);
    if (NULL != pstObjJson && cJSON_IsObject(pstObjJson)) {
        /*parse data*/
        if (OPLK_OK != REST_ParseSystem((const cJSON *)pstObjJson, &(pstFullConfig->stSystem))) {
            LOG_WRITE(EN_LOG_ERROR, "parse system node failed");
            goto END_LABEL;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "not find system node");
        goto END_LABEL;
    }
#if defined(__OCS__)
    /* clear compile waring */
    enCardType = enCardType;
    pcEdfaNodeName = pcEdfaNodeName;
#else
    for (enCardType = EN_REST_AMP_W_CARD; enCardType < REST_GetAmpCardCount(); enCardType++) {


        /*parse "booster-amp" node*/
        pcEdfaNodeName = (EN_REST_AMP_W_CARD == enCardType) ? STR_BOOSTER_AMP : STR_BOOSTER_AMP_P;
        pstObjJson = cJSON_GetObjectItem(pstJson, pcEdfaNodeName);
        if (NULL != pstObjJson && cJSON_IsObject(pstObjJson)) {
            /*parse data*/
            if (OPLK_OK != REST_ParseEdfaBA((const cJSON *)pstObjJson, &(pstFullConfig->astEdfaBA[enCardType]))) {
                LOG_WRITE(EN_LOG_ERROR, "parse %s node failed", pcEdfaNodeName);
                goto END_LABEL;
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "not find %s node", pcEdfaNodeName);
            goto END_LABEL;
        }

        /*parse "pre-amp" node*/
        pcEdfaNodeName = (EN_REST_AMP_W_CARD == enCardType) ? STR_PRE_AMP : STR_PRE_AMP_P;
        pstObjJson = cJSON_GetObjectItem(pstJson, pcEdfaNodeName);
        if (NULL != pstObjJson && cJSON_IsObject(pstObjJson)) {
            /*parse data*/
            if (OPLK_OK != REST_ParseEdfaPA((const cJSON *)pstObjJson, &(pstFullConfig->astEdfaPA[enCardType]))) {
                LOG_WRITE(EN_LOG_ERROR, "parse %s node failed", pcEdfaNodeName);
                goto END_LABEL;
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "not find %s node", pcEdfaNodeName);
            goto END_LABEL;
        }
    }
#endif
    return OPLK_OK;

END_LABEL:
    return OPLK_ERR;
}

int
REST_SetFullConfig(REST_FULLCONFIG_ST *pstFullConfig)
{
    int rc = SR_ERR_OK;
    REST_FULLCONFIG_ST stLastFullConfig;
    REST_AMP_CARD_TYPE_EN enCardType = EN_REST_AMP_CARD_MAX;
    bool abCardOnline[EN_REST_AMP_CARD_MAX];
    bool abCardConfig[EN_REST_AMP_CARD_MAX];

    memset(&stLastFullConfig, 0, sizeof(stLastFullConfig));
    memset(&abCardOnline, 0, sizeof(abCardOnline));
    memset(&abCardConfig, 0, sizeof(abCardConfig));

#if defined(__OCS__)
    /* clear compile warning */
    enCardType = enCardType;
#else

    /* NOTE: When the card is in place, set the configuration, otherwise ignore */

    for (enCardType = EN_REST_AMP_W_CARD; enCardType < REST_GetAmpCardCount(); enCardType++) {

        abCardOnline[enCardType] = REST_IsAmpCardOnline(enCardType);
        if (false == abCardOnline[enCardType]) {
            abCardConfig[enCardType] = false;
            continue;
        }

        /* pre-amp data */
        rc = REST_GetEdfaPA(enCardType, &(stLastFullConfig.astEdfaPA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get pre-amp node failed, enCardType:%d", enCardType);
            goto cleanup;
        }

        abCardConfig[enCardType] = true;
        rc = REST_SetEdfaPA(enCardType, &(pstFullConfig->astEdfaPA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "set pre-amp node failed, enCardType:%d", enCardType);
            goto cleanup;
        }

        /* booster-amp data */
        rc = REST_GetEdfaBA(enCardType, &(stLastFullConfig.astEdfaBA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get booster-amp node failed, enCardType:%d", enCardType);
            goto cleanup;
        }

        rc = REST_SetEdfaBA(enCardType, &(pstFullConfig->astEdfaBA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "set booster-amp node failed, enCardType:%d", enCardType);
            goto cleanup;
        }
    }
#endif
    /* system data */
    rc = REST_SetSystem(&(pstFullConfig->stSystem));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "set system node failed");
        goto cleanup;
    }

    return SR_ERR_OK;

cleanup:

#if defined(__OCS__)

#else
    /* AMP card data Rollback */
    for (enCardType = EN_REST_AMP_W_CARD; enCardType < REST_GetAmpCardCount(); enCardType++) {
        if (abCardOnline[enCardType] && abCardConfig[enCardType]) {
            REST_SetEdfaPA(enCardType, &(stLastFullConfig.astEdfaPA[enCardType]));
            REST_SetEdfaBA(enCardType, &(stLastFullConfig.astEdfaBA[enCardType]));
        }
    }
#endif
    return rc;
}

int
REST_GetFullConfig(REST_FULLCONFIG_ST *pstFullConfig)
{
    int rc = SR_ERR_OK;
    REST_AMP_CARD_TYPE_EN enCardType = EN_REST_AMP_CARD_MAX;

    /* get system data */
    rc = REST_GetSystem(&(pstFullConfig->stSystem));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get system data failed");
        goto ERR_LABEL;
    }
#if defined(__OCS__)
    /* clear compile warning */
    enCardType = enCardType;
#else
    for (enCardType = EN_REST_AMP_W_CARD; enCardType < REST_GetAmpCardCount(); enCardType++) {
        /* get pre-amp data */
        rc = REST_GetEdfaPA(enCardType, &(pstFullConfig->astEdfaPA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get pre-amp data failed, enCardType:%d", enCardType);
            goto ERR_LABEL;
        }

        /* get booster-amp data */
        rc = REST_GetEdfaBA(enCardType, &(pstFullConfig->astEdfaBA[enCardType]));
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "parse booster-amp node failed, enCardType:%d", enCardType);
            goto ERR_LABEL;
        }
    }
#endif

ERR_LABEL:
    return rc;
}

static void
__InitInventory(REST_INVENTORY_ST *pstInventory)
{
    size_t i = 0, nSize = sizeof(REST_INVENTORY_ST);
    char *pcStartPos = NULL;

    memset(pstInventory, 0, nSize);
    pcStartPos = (char *)pstInventory;
    while (i < nSize) {
        snprintf(pcStartPos + i, 32, "%s", STR_NA);
        i += 32;
    }

    return;
}

int
REST_GetInventory(REST_INVENTORY_ST *pstInventory)
{
    int rc = SR_ERR_OK;
    REST_INVENTORY_ST stInventory;
    ST_NODE_INFO stNodeInfo;
    ST_SHELF stShelf;
    ST_CIRCUIT_PACK *pstCircuitPack = NULL;
    size_t i = 0, nCircuitPackCount = 0;

    memset(&stInventory, 0, sizeof(stInventory));
    memset(&stShelf, 0, sizeof(stShelf));
    memset(&stShelf, 0, sizeof(stShelf));

    /* init "NA" */
    __InitInventory(&stInventory);

    rc = DSC_SHELF_GetInfo(&stShelf);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

    rc = DSC_GetNodeInfo(&stNodeInfo, sizeof(stNodeInfo));
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

    snprintf(stInventory.acModuleSN, sizeof(stInventory.acModuleSN), "%s", stShelf.stPhysical.stNode.acSN);
    snprintf(stInventory.acModulePN, sizeof(stInventory.acModulePN), "%s", stShelf.stPhysical.stCommon.acProductCode);
    snprintf(stInventory.acVendor, sizeof(stInventory.acVendor), "%s", stShelf.stPhysical.stNode.acVendor);

    snprintf(stInventory.acModuleSoftVer, sizeof(stInventory.acModuleSoftVer), "%s", stNodeInfo.acSwVer);

    rc = DSC_CP_GetAll(&pstCircuitPack, &nCircuitPackCount);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

    for (i = 0; i < nCircuitPackCount; i++) {
        if (0 == strlen(pstCircuitPack[i].acName) ||
            0 == strlen(pstCircuitPack[i].acRefSlotName)) {
            continue;
        }

        if (0 == strcasecmp(pstCircuitPack[i].stPhysical.stNode.acSN, DEF_OOD_SERIAL_ID)) {
            continue;
        }

        if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_SCC)) {
            snprintf(stInventory.stScc.acPN, sizeof(stInventory.stScc.acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.stScc.acSN, sizeof(stInventory.stScc.acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_OPS)) {
            snprintf(stInventory.stOps.acPN, sizeof(stInventory.stOps.acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.stOps.acSN, sizeof(stInventory.stOps.acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_OCS)) {
            snprintf(stInventory.stOcs.acPN, sizeof(stInventory.stOcs.acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.stOcs.acSN, sizeof(stInventory.stOcs.acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_AMP)) {
            snprintf(stInventory.astAmp[EN_REST_AMP_W_CARD].acPN, sizeof(stInventory.astAmp[EN_REST_AMP_W_CARD].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astAmp[EN_REST_AMP_W_CARD].acSN, sizeof(stInventory.astAmp[EN_REST_AMP_W_CARD].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_AMP_P)) {
            snprintf(stInventory.astAmp[EN_REST_AMP_P_CARD].acPN, sizeof(stInventory.astAmp[EN_REST_AMP_P_CARD].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astAmp[EN_REST_AMP_P_CARD].acSN, sizeof(stInventory.astAmp[EN_REST_AMP_P_CARD].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_FAN "0")) {
            snprintf(stInventory.astFAN[FAN_A_INDEX].acPN, sizeof(stInventory.astFAN[FAN_A_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astFAN[FAN_A_INDEX].acSN, sizeof(stInventory.astFAN[FAN_A_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_FAN "1")) {
            snprintf(stInventory.astFAN[FAN_B_INDEX].acPN, sizeof(stInventory.astFAN[FAN_B_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astFAN[FAN_B_INDEX].acSN, sizeof(stInventory.astFAN[FAN_B_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_FAN "2")) {
            snprintf(stInventory.astFAN[FAN_C_INDEX].acPN, sizeof(stInventory.astFAN[FAN_C_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astFAN[FAN_C_INDEX].acSN, sizeof(stInventory.astFAN[FAN_C_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_FAN "3")) {
            snprintf(stInventory.astFAN[FAN_D_INDEX].acPN, sizeof(stInventory.astFAN[FAN_D_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astFAN[FAN_D_INDEX].acSN, sizeof(stInventory.astFAN[FAN_D_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_POWER "1")) {
            snprintf(stInventory.astPower[POWER_A_INDEX].acPN, sizeof(stInventory.astPower[POWER_A_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astPower[POWER_A_INDEX].acSN, sizeof(stInventory.astPower[POWER_A_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_KEY_POWER "2")) {
            snprintf(stInventory.astPower[POWER_B_INDEX].acPN, sizeof(stInventory.astPower[POWER_B_INDEX].acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.astPower[POWER_B_INDEX].acSN, sizeof(stInventory.astPower[POWER_B_INDEX].acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        } else if (0 == strcasecmp(pstCircuitPack[i].acName, CIRCUIT_PACK_MUX)) {
            snprintf(stInventory.stMux.acPN, sizeof(stInventory.stMux.acPN), "%s", pstCircuitPack[i].stPhysical.stCommon.acProductCode);
            snprintf(stInventory.stMux.acSN, sizeof(stInventory.stMux.acSN), "%s", pstCircuitPack[i].stPhysical.stNode.acSN);
        }

    }

    memcpy(pstInventory, &stInventory, sizeof(stInventory));

END_LABEL:
    DEF_SAFE_FREE(pstCircuitPack);
    return rc;
}

int
REST_ParseRestart(const cJSON *pstJson, REST_RESTART_ST *pstRestart)
{
    REST_RESTART_ST stRestart;
    JSON_OBJ_ST astJsonObj[] = {{STR_ENTITY, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stRestart.acEntity), sizeof(stRestart.acEntity)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stRestart, 0, sizeof(stRestart));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstRestart, &stRestart, sizeof(stRestart));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_CommonRestart(REST_RESTART_ST *pstRestart, char *pcResetType)
{
    int rc = SR_ERR_OK;
    char acResetModule[32] = {0};
    ENTITY_TYPE_KV_ST stEntityKV[] = {{STR_ENTITY_MODULE, RESET_DEVICE},
                                      {STR_ENTITY_SCC, RESET_CARD_SCC},
#if defined(__OCS__)
                                      {STR_ENTITY_OCS, RESET_CARD_OCS}
#elif defined(__OLSP__)
                                      {STR_ENTITY_OPTICAL_CARD, RESET_CARD_AMP},
                                      {STR_ENTITY_OPTICAL_CARD_P, RESET_CARD_AMP_P},
                                      {STR_ENTITY_OPS, RESET_CARD_OPS}
#elif defined(__HCF__)
                                      {STR_ENTITY_OPTICAL_CARD, RESET_CARD_OPTICAL}
#else

#endif
    };
    int i = 0;
    int iCount = 0;

    iCount = sizeof(stEntityKV) / sizeof(stEntityKV[0]);

    for (i = 0; i < iCount; i++) {
        if (0 == strcasecmp(pstRestart->acEntity, stEntityKV[i].pcEntity)) {
            snprintf(acResetModule, sizeof(acResetModule), "%s", stEntityKV[i].pcResetModule);
            break;
        }
    }

    if (i >= iCount) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid acEntity: %s", pstRestart->acEntity);
        goto ERR_LABEL;
    }

    rc = DSC_Reset(acResetModule, pcResetType);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

ERR_LABEL:
    return rc;
}

int
REST_ParseTransferFile(const cJSON *pstJson, REST_TRANSFER_FILE_ST *pstTransferFile)
{
    REST_TRANSFER_FILE_ST stTransferFile;
    JSON_OBJ_ST astJsonObj[] = {{STR_USER_ID, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acUserId), sizeof(stTransferFile.acUserId)},
                                {STR_PASSWORD, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acPassword), sizeof(stTransferFile.acPassword)},
                                {STR_SOURCE_FILE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acSrcFileName), sizeof(stTransferFile.acSrcFileName)},
                                {STR_DESTINATION_FILE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acDestFileName), sizeof(stTransferFile.acDestFileName)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stTransferFile, 0, sizeof(stTransferFile));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstTransferFile, &stTransferFile, sizeof(stTransferFile));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_TransferFile(REST_TRANSFER_FILE_ST *pstTransferFile)
{
    int rc = SR_ERR_OK;
    char acRemotePath[512] = {0};

    rc = UTIL_ChkStrSpecChr(pstTransferFile->acUserId);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile UserId error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = UTIL_CheckStrWithAllSpecChars(pstTransferFile->acPassword);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile Password error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = UTIL_ChkStrSpecChr(pstTransferFile->acSrcFileName);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile Src File Name error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = UTIL_ChkStrSpecChr(pstTransferFile->acDestFileName);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile Dest File Name error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    snprintf(acRemotePath, sizeof(acRemotePath), "scp:%s@%s&%s", pstTransferFile->acUserId,
             pstTransferFile->acDestFileName, pstTransferFile->acPassword);

    rc = DSC_TransferFile(FILE_TRANSFER_UPLOAD, pstTransferFile->acSrcFileName, acRemotePath);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}


int
REST_ParseTransferLog(const cJSON *pstJson, REST_TRANSFER_FILE_ST *pstTransferFile)
{
    REST_TRANSFER_FILE_ST stTransferFile;
    JSON_OBJ_ST astJsonObj[] = {{STR_USER_ID, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acUserId), sizeof(stTransferFile.acUserId)},
                                {STR_PASSWORD, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acPassword), sizeof(stTransferFile.acPassword)},
                                /* {STR_SOURCE_FILE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acSrcFileName), sizeof(stTransferFile.acSrcFileName)}, */
                                {STR_DESTINATION_FILE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stTransferFile.acDestFileName), sizeof(stTransferFile.acDestFileName)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stTransferFile, 0, sizeof(stTransferFile));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstTransferFile, &stTransferFile, sizeof(stTransferFile));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_TransferLog(REST_TRANSFER_FILE_ST *pstTransferFile)
{
    int rc = SR_ERR_OK;

    rc = UTIL_ChkStrSpecChr(pstTransferFile->acUserId);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile UserId error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = UTIL_CheckStrWithAllSpecChars(pstTransferFile->acPassword);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile Password error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = UTIL_ChkStrSpecChr(pstTransferFile->acDestFileName);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check TransferFile Dest File Name error!");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }


    rc = DSC_TransferLog(pstTransferFile->acUserId, pstTransferFile->acPassword, pstTransferFile->acDestFileName);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}

int
REST_ParseSystemOptional(const cJSON *pstJson, REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag)
{
    REST_SYSTEM_ST stSystem;
    char aacFiberTypeStr[2][32] = {{0}, {0}};
    char aacLongSpanStr[2][32] = {{0}, {0}};
    FIBER_TYPE_EN enFiberType = FIBER_TYPE_CNT;
    REST_SYSTEM_FLAG_ST stSystemExistFlag;
    int i = 0;
    JSON_OBJ_NEW_ST astJsonObj[] = {{STR_NODE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acNodeName), sizeof(stSystem.acNodeName), &(stSystemExistFlag.bNodeNameFlag)},
                                    {STR_IP_ADDRESS, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIPV4), sizeof(stSystem.acIPV4), &(stSystemExistFlag.bIpv4Flag)},
                                    {STR_NETMASK, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acNetmask), sizeof(stSystem.acNetmask), &(stSystemExistFlag.bNetmaskFlag)},
                                    {STR_GATEWAY, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acGatewayV4), sizeof(stSystem.acGatewayV4), &(stSystemExistFlag.bGatewayFlag)},
                                    {STR_NTP_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacNtpIp[0]), sizeof(stSystem.aacNtpIp[0]), &(stSystemExistFlag.abNtpFlag[0])},
                                    {STR_NTP_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacNtpIp[1]), sizeof(stSystem.aacNtpIp[1]), &(stSystemExistFlag.abNtpFlag[1])},
                                    {STR_SYSLOG_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacSyslogIp[0]), sizeof(stSystem.aacSyslogIp[0]), &(stSystemExistFlag.abSyslogFlag[0])},
                                    {STR_SYSLOG_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.aacSyslogIp[1]), sizeof(stSystem.aacSyslogIp[1]), &(stSystemExistFlag.abSyslogFlag[1])},
                                    {STR_TACACS_IP_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[0].acIp), sizeof(stSystem.astTacacsServer[0].acIp), &(stSystemExistFlag.abTacacsIpFlag[0])},
                                    {STR_TACACS_KEY_1, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[0].acKey), sizeof(stSystem.astTacacsServer[0].acKey), &(stSystemExistFlag.abTacacsKeyFlag[0])},
                                    /*{STR_TACACS_PORT_1, EN_DATA_TYPE_UINT, TAKE_VOID_ADDRESS(stSystem.stTacacsServer.uiPort), sizeof(stSystem.stTacacsServer.uiPort)},*/ /*fixed port: 49*/
                                    {STR_TACACS_IP_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[1].acIp), sizeof(stSystem.astTacacsServer[1].acIp), &(stSystemExistFlag.abTacacsIpFlag[1])},
                                    {STR_TACACS_KEY_2, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.astTacacsServer[1].acKey), sizeof(stSystem.astTacacsServer[1].acKey), &(stSystemExistFlag.abTacacsKeyFlag[1])},
#if defined(__OCS__)
#else
                                    /* auto control parameters of working line */
                                    {STR_FIBER_TYPE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacFiberTypeStr[0]), sizeof(aacFiberTypeStr[0]), &(stSystemExistFlag.abFiberTypeFlag[0])},
                                    {STR_LAUNCHING_POWER, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adLauchingPower[0]), sizeof(stSystem.adLauchingPower[0]), &(stSystemExistFlag.abLaunchPwrFlag[0])},
                                    {STR_DROP_POWER_PER_CHANNEL, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adDropPower[0]), sizeof(stSystem.adDropPower[0]), &(stSystemExistFlag.abDropPwrFlag[0])},
                                    {STR_NO_OF_CARRIERS, EN_DATA_TYPE_UCHAR, TAKE_VOID_ADDRESS(stSystem.aucCarries[0]), sizeof(stSystem.aucCarries[0]), &(stSystemExistFlag.abCarriersFlag[0])},
                                    {STR_LONG_SPAN, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacLongSpanStr[0]), sizeof(aacLongSpanStr[0]), &(stSystemExistFlag.abLongSpanFlag[0])},
#if defined(__OLSP__)

                                    /* auto control parameters of protect line */
                                    {STR_FIBER_TYPE_P, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacFiberTypeStr[1]), sizeof(aacFiberTypeStr[1]), &(stSystemExistFlag.abFiberTypeFlag[1])},
                                    {STR_LAUNCHING_POWER_P, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adLauchingPower[1]), sizeof(stSystem.adLauchingPower[1]), &(stSystemExistFlag.abLaunchPwrFlag[1])},
                                    {STR_DROP_POWER_PER_CHANNEL_P, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stSystem.adDropPower[1]), sizeof(stSystem.adDropPower[1]), &(stSystemExistFlag.abDropPwrFlag[1])},
                                    {STR_NO_OF_CARRIERS_P, EN_DATA_TYPE_UCHAR, TAKE_VOID_ADDRESS(stSystem.aucCarries[1]), sizeof(stSystem.aucCarries[1]), &(stSystemExistFlag.abCarriersFlag[1])},
                                    {STR_LONG_SPAN_P, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacLongSpanStr[1]), sizeof(aacLongSpanStr[1]), &(stSystemExistFlag.abLongSpanFlag[1])},
#endif
#endif
                                    {STR_TIMEZONE, EN_DATA_TYPE_SHORT, TAKE_VOID_ADDRESS(stSystem.sTimezone), sizeof(stSystem.sTimezone), &(stSystemExistFlag.bTimezone)},
                                    {STR_IPV6_MODE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIpv6Mode), sizeof(stSystem.acIpv6Mode), &(stSystemExistFlag.bIpv6ModeFlag)},
                                    {STR_IPV6_ADDRESS, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acIPv6), sizeof(stSystem.acIPv6), &(stSystemExistFlag.bIpv6AddrFlag)},
                                    {STR_IPV6_GATEWAY, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSystem.acGatewayV6), sizeof(stSystem.acGatewayV6), &(stSystemExistFlag.bIpv6GatewayFlag)},

                                    {NULL, EN_DATA_TYPE_MAX, NULL, 0, NULL}};

    memset(&stSystem, 0, sizeof(stSystem));
    memset(&stSystemExistFlag, 0, sizeof(stSystemExistFlag));

    if (OPLK_OK != JSON_GetObjItemsOptional(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }
#if defined(__OCS__)
    /* clear compile waring */
    memset(aacFiberTypeStr, 0, sizeof(aacFiberTypeStr));
    memset(aacLongSpanStr, 0, sizeof(aacLongSpanStr));
    enFiberType = enFiberType;
    i = i;
#else
    /* parse auto control parameters */
    for (i = 0; i < REST_GetLinePathMax(); i++) {
        if (stSystemExistFlag.abFiberTypeFlag[i]) {
            enFiberType = DSC_FiberTypeToEnum(aacFiberTypeStr[i]);
            if (FIBER_TYPE_CNT == enFiberType) {
                LOG_WRITE(EN_LOG_ERROR, "Unknown fiber-type[%d]:%s", i, aacFiberTypeStr[i]);
                goto ERR_LABEL;
            }

            stSystem.aenFiberType[i] = enFiberType;
        }

        if (stSystemExistFlag.abLongSpanFlag[i]) {
            if (0 == strcmp(aacLongSpanStr[i], STR_ENABLE)) {
                stSystem.abLongSpan[i] = true;
            } else if (0 == strcmp(aacLongSpanStr[i], STR_DISABLE)) {
                stSystem.abLongSpan[i] = false;
            } else {
                LOG_WRITE(EN_LOG_ERROR, "Invalid long-span[%d]:%s", i, aacLongSpanStr[i]);
                goto ERR_LABEL;
            }
        }
    }
#endif
    /* ip/netmask/gateway must be existed at the same time */
    if (stSystemExistFlag.bIpv4Flag || stSystemExistFlag.bNetmaskFlag || stSystemExistFlag.bGatewayFlag) {

        if (!stSystemExistFlag.bIpv4Flag || !stSystemExistFlag.bNetmaskFlag || !stSystemExistFlag.bGatewayFlag) {
            LOG_WRITE(EN_LOG_ERROR, "ip/netmask/gateway must be existed at the same time");
            goto ERR_LABEL;
        }
    }

    /* ipv6 */
    if (stSystemExistFlag.bIpv6ModeFlag || stSystemExistFlag.bIpv6AddrFlag || stSystemExistFlag.bIpv6GatewayFlag) {
        if (!stSystemExistFlag.bIpv6ModeFlag) {
            LOG_WRITE(EN_LOG_ERROR, "ipv6-mode must be existed");
            goto ERR_LABEL;
        }
    }

    /* tacacs+ server/key must be existed at the same time */
    for (i = 0; i < TACACS_SERVER_MAX_NUM; i++) {
        if (stSystemExistFlag.abTacacsIpFlag[i] || stSystemExistFlag.abTacacsKeyFlag[i]) {
            if (!stSystemExistFlag.abTacacsIpFlag[i] || !stSystemExistFlag.abTacacsKeyFlag[i]) {
                LOG_WRITE(EN_LOG_ERROR, "tacacs+ server[%d]/key must be existed at the same time", i + i);
                goto ERR_LABEL;
            }
        }
    }

    memcpy(pstSystem, &stSystem, sizeof(stSystem));
    memcpy(pstExistFlag, &stSystemExistFlag, sizeof(stSystemExistFlag));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


static int
__REST_CheckSystemOptional(REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag, REST_SYSTEM_ST *pstOldSystem)
{
    size_t i = 0, nOldIndex = 0;
    EN_NETWORK_IP_ADDRESS_ORIGIN enIpv6Origin = EN_NETWORK_ORIGIN_OTHER;
    char acIpv6Addr[DEF_IPV6_ADDR_MAX_LEN] = {0};
    uint8_t ucPrefixLen = 0;

    if (NULL == pstSystem) {
        goto ERR_LABEL;
    }

    /* check nodename */
    if (pstExistFlag->bNodeNameFlag) {
        if (OPLK_ERR == UTIL_ChkStrSpecChr(pstSystem->acNodeName)) {
            LOG_WRITE(EN_LOG_ERROR, "invalid nodename");
            goto ERR_LABEL;
        }
    }

#define HAVE_SAME_SERVER_IP(acIp1, acIp2) ((strlen(acIp1) > 0) && (strlen(acIp2) > 0) && (0 == strcmp(acIp1, acIp2)))

    /* check ntp1/2 */
    if (pstExistFlag->abNtpFlag[0] && pstExistFlag->abNtpFlag[1]) {
        if (HAVE_SAME_SERVER_IP(pstSystem->aacNtpIp[0], pstSystem->aacNtpIp[1])) {
            LOG_WRITE(EN_LOG_ERROR, "ntp server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    } else if (pstExistFlag->abNtpFlag[0]) {
        if (HAVE_SAME_SERVER_IP(pstSystem->aacNtpIp[0], pstOldSystem->aacNtpIp[1])) {
            LOG_WRITE(EN_LOG_ERROR, "ntp server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    } else if (pstExistFlag->abNtpFlag[1]) {
        if (HAVE_SAME_SERVER_IP(pstSystem->aacNtpIp[1], pstOldSystem->aacNtpIp[0])) {
            LOG_WRITE(EN_LOG_ERROR, "ntp server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    }

    /* check syslog1/2 */
    if (pstExistFlag->abSyslogFlag[0] && pstExistFlag->abSyslogFlag[1]) {
        if (HAVE_SAME_SERVER_IP(pstSystem->aacSyslogIp[0], pstSystem->aacSyslogIp[1])) {
            LOG_WRITE(EN_LOG_ERROR, "syslog server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    } else if (pstExistFlag->abSyslogFlag[0]) {
        if (HAVE_SAME_SERVER_IP(pstSystem->aacSyslogIp[0], pstOldSystem->aacSyslogIp[1])) {
            LOG_WRITE(EN_LOG_ERROR, "syslog server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    } else if (pstExistFlag->abSyslogFlag[1]) {
        if (HAVE_SAME_SERVER_IP(pstOldSystem->aacSyslogIp[0], pstSystem->aacSyslogIp[1])) {
            LOG_WRITE(EN_LOG_ERROR, "syslog server1 and server2 have the same IP!");
            goto ERR_LABEL;
        }
    }

/* #undef HAVE_SAME_SERVER_IP */

    /* check the tacacs+ server (allow to be empty) */
    if (pstExistFlag->abTacacsIpFlag[0] && pstExistFlag->abTacacsIpFlag[1]) {
        for (i = 0; i < TACACS_SERVER_MAX_NUM; i++) {
            if (0 != strlen(pstSystem->astTacacsServer[i].acIp)) {
                if ((i > 0) && (0 == strcmp(pstSystem->astTacacsServer[i - 1].acIp, pstSystem->astTacacsServer[i].acIp))) {
                    LOG_WRITE(EN_LOG_ERROR, "TACACS server1 and server2 must be different");
                    goto ERR_LABEL;
                }

                if (OPLK_ERR == UTIL_ChkStrSpecChr(pstSystem->astTacacsServer[i].acKey)) {
                    LOG_WRITE(EN_LOG_ERROR, "invalid input TACACS server%ld key", i + 1);
                    goto ERR_LABEL;
                }
            } else {
                if (0 != strlen(pstSystem->astTacacsServer[i].acKey)) {
                    LOG_WRITE(EN_LOG_ERROR, "Key and IP of TACACS server%ld must be empty or exist at the same time", i + 1);
                    goto ERR_LABEL;
                }
                /* allow the tacacs+ server is empty. */
            }
        }
    } else if (pstExistFlag->abTacacsIpFlag[0] || pstExistFlag->abTacacsIpFlag[1]) {
        if (pstExistFlag->abTacacsIpFlag[0]) {
            i = 0;
            nOldIndex = 1;
        } else {
            i = 1;
            nOldIndex = 0;
        }

        if (0 != strlen(pstSystem->astTacacsServer[i].acIp)) {
            if (0 == strcmp(pstSystem->astTacacsServer[i].acIp, pstOldSystem->astTacacsServer[nOldIndex].acIp)) {
                LOG_WRITE(EN_LOG_ERROR, "TACACS server1 and server2 must be different. nOldIndex: %lu", nOldIndex);
                goto ERR_LABEL;
            }

            if (OPLK_ERR == UTIL_ChkStrSpecChr(pstSystem->astTacacsServer[i].acKey)) {
                LOG_WRITE(EN_LOG_ERROR, "invalid input TACACS server%ld key", i + 1);
                goto ERR_LABEL;
            }
        } else {
            if (0 != strlen(pstSystem->astTacacsServer[i].acKey)) {
                LOG_WRITE(EN_LOG_ERROR, "Key and IP of TACACS server%ld must be empty or exist at the same time", i + 1);
                goto ERR_LABEL;
            }
            /* allow the tacacs+ server is empty. */
        }
    }

    if (pstExistFlag->bTimezone) {
        if (pstSystem->sTimezone > 12 || pstSystem->sTimezone < -12) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid timezone: %d", pstSystem->sTimezone);
            goto ERR_LABEL;
        }
    }

    /* check launching power by yang model */
#if 0
    if (pstExistFlag->bLaunchPwrFlag) {
        if (pstSystem->dLauchingPower < AUTO_CFG_LAUNCH_POWER_MIN
            || pstSystem->dLauchingPower > AUTO_CFG_LAUNCH_POWER_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "lauching power is out range. dLauchingPower = %.2f", pstSystem->dLauchingPower);
            goto ERR_LABEL;
        }
    }
#endif

    /* check ipv6 */
    if (pstExistFlag->bIpv6ModeFlag || pstExistFlag->bIpv6AddrFlag || pstExistFlag->bIpv6GatewayFlag) {
        if (SR_ERR_OK != DSC_EthOriginStrToValue(pstSystem->acIpv6Mode, &enIpv6Origin)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6-mode: %s", pstSystem->acIpv6Mode);
            goto ERR_LABEL;
        }

        if (EN_NETWORK_ORIGIN_STATIC == enIpv6Origin) {
            /* ipv6-address/ipv6-gateway must be existed at the same time.
               both addr and gateway are either NOT empty or empty.
             */
            if (!pstExistFlag->bIpv6AddrFlag || !pstExistFlag->bIpv6GatewayFlag ||
                (0 == strlen(pstSystem->acIPv6)) || (0 == strlen(pstSystem->acGatewayV6))) {
                LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6: %s,%s", pstSystem->acIPv6, pstSystem->acGatewayV6);
                goto ERR_LABEL;
            }

            if (OPLK_OK != UTIL_SeparateIpv6AddrAndPrefix(pstSystem->acIPv6, acIpv6Addr, sizeof(acIpv6Addr), &ucPrefixLen)) {
                LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6-address: %s", pstSystem->acIPv6);
                goto ERR_LABEL;
            }
        } else if ((EN_NETWORK_ORIGIN_DHCP == enIpv6Origin) || (EN_NETWORK_ORIGIN_LINK_LAYER == enIpv6Origin)) {
            /* only check ipv6-address/ipv6-gateway */
            if (0 != strlen(pstSystem->acIPv6)) {
                if (OPLK_OK != UTIL_SeparateIpv6AddrAndPrefix(pstSystem->acIPv6, acIpv6Addr, sizeof(acIpv6Addr), &ucPrefixLen)) {
                    LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6-address: %s", pstSystem->acIPv6);
                    goto ERR_LABEL;
                }
            }
            if (0 != strlen(pstSystem->acGatewayV6)) {
                if (OPLK_OK != UTIL_ChkIpV6Addr(pstSystem->acGatewayV6)) {
                    LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6-gateway: %s", pstSystem->acGatewayV6);
                    goto ERR_LABEL;
                }
            }

        } else {
            LOG_WRITE(EN_LOG_ERROR, "Invalid ipv6-mode:%s", pstSystem->acIpv6Mode);
            goto ERR_LABEL;
        }
    }

    return SR_ERR_OK;
ERR_LABEL:
    return SR_ERR_INVAL_ARG;
}

static int
__GetNtpServer(REST_SYSTEM_ST *pstSystem)
{
    uint32_t i = 0, j = 0;
    NTP_SERVER_LIST_ST stNtpServerList;
    int rc = SR_ERR_OK;

    memset(&stNtpServerList, 0, sizeof(stNtpServerList));
    rc = DSC_OC_GetNtpServer(true, &stNtpServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get ntp server list %s error!", sr_strerror(rc));
        goto END_LABEL;
    }

    for (i = 0; i < stNtpServerList.ulTotal; i++) {
        if ((NTP_SERVER_MAX_NUM == stNtpServerList.ulTotal) && (!stNtpServerList.stNtpServer[0].bPrefer) && (stNtpServerList.stNtpServer[1].bPrefer)) {
            j = (0 == i) ? i + 1 : i - 1;
            snprintf(pstSystem->aacNtpIp[i], sizeof(pstSystem->aacNtpIp[j]), "%s", stNtpServerList.stNtpServer[j].acIp);
        } else {
            snprintf(pstSystem->aacNtpIp[i], sizeof(pstSystem->aacNtpIp[i]), "%s", stNtpServerList.stNtpServer[i].acIp);
        }
    }

END_LABEL:
    return rc;
}

static int
__GetSyslogServer(REST_SYSTEM_ST *pstSystem)
{
    uint32_t i = 0;
    SYSLOG_SERVER_LIST_ST stSyslogServerList;
    int rc = SR_ERR_OK;

    memset(&stSyslogServerList, 0, sizeof(stSyslogServerList));

    rc = DSC_GetOcSyslogServerConfig(&stSyslogServerList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get oc syslog server list %s error!", sr_strerror(rc));
        goto END_LABEL;
    }

    for (i = 0; i < stSyslogServerList.ulTotal; i++) {
        snprintf(pstSystem->aacSyslogIp[i], sizeof(pstSystem->aacSyslogIp[i]), "%s", stSyslogServerList.astSyslogServer[i].stAction.acIp);
    }

END_LABEL:
    return rc;
}

static int
__SetNtpServer(const REST_SYSTEM_ST *pstSystem, const REST_SYSTEM_FLAG_ST *pstExistFlag, const REST_SYSTEM_ST *pstOldSystem)
{
    int i = 0;
    NTP_SERVER_ST stNtpServerInfo;
    int rc = SR_ERR_OK;
    char aacNtpIp[2][NTP_SERVER_IP_MAX_LEN] = {{0}, {0}};
    bool abChangeFlag[CLIENT_NTP_SERVER_NUM] = {0};
    bool bCfgNtp = false;

    if (!pstExistFlag->abNtpFlag[0] && !pstExistFlag->abNtpFlag[1]) {
        return SR_ERR_OK;
    } else {
        for (i = 0; i < CLIENT_NTP_SERVER_NUM; i++) {
            if (pstExistFlag->abNtpFlag[i]) {
                strncpy(aacNtpIp[i], pstSystem->aacNtpIp[i], sizeof(aacNtpIp[i]) - 1);
            } else {
                strncpy(aacNtpIp[i], pstOldSystem->aacNtpIp[i], sizeof(aacNtpIp[i]) - 1);
            }

            if (0 == strcmp(aacNtpIp[i], pstOldSystem->aacNtpIp[i])) {
                abChangeFlag[i] = false;
            } else {
                abChangeFlag[i] = true;
            }
        }
    }

    if (!abChangeFlag[0] && !abChangeFlag[1]) {
        LOG_WRITE(EN_LOG_NOTICE, "ntp server already existed!");
        return SR_ERR_OK;
    }

    if (HAVE_SAME_SERVER_IP(aacNtpIp[0], aacNtpIp[1])) {
        LOG_WRITE(EN_LOG_ERROR, "ntp server1 and server2 have the same IP(%s)!", aacNtpIp[1]);
        goto END_LABEL;
    }

    /* delete old ntp */
    for (i = 0; i < CLIENT_NTP_SERVER_NUM; i++) {
        if (!abChangeFlag[i]) {
            continue;
        }

        if (strlen(pstOldSystem->aacNtpIp[i]) > 0) {
            rc = DSC_OC_DeleteNtpServer((char *)pstOldSystem->aacNtpIp[i]);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_OC_DeleteNtpServer(%s) error !", pstOldSystem->aacNtpIp[i]);
                goto END_LABEL;
            }
        }
    }


    /* add new ntp */
    for (i = 0; i < CLIENT_NTP_SERVER_NUM; i++) {
        if (!abChangeFlag[i]) {
            continue;
        }

        if (strlen(pstSystem->aacNtpIp[i]) > 0) {
            memset(&stNtpServerInfo, 0, sizeof(NTP_SERVER_ST));
            strncpy(stNtpServerInfo.acIp, pstSystem->aacNtpIp[i], NTP_SERVER_IP_MAX_LEN - 1);
            stNtpServerInfo.usPort = 123;
            stNtpServerInfo.iVersion = 4;
            stNtpServerInfo.bBurst = true;
            stNtpServerInfo.bPrefer = (0 == i) ? true : false;

            rc = DSC_OC_AddNtpServer(&stNtpServerInfo);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_OC_AddNtpServer(%s) error !", pstSystem->aacNtpIp[i]);
                goto END_LABEL;
            }

            bCfgNtp = true;
        }
    }

    if (bCfgNtp) {
        rc = DSC_OC_SetNtpEnable(true);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    }

END_LABEL:
    return rc;
}

static int
__SetSyslogServer(const REST_SYSTEM_ST *pstSystem, const REST_SYSTEM_FLAG_ST *pstExistFlag, const REST_SYSTEM_ST *pstOldSystem)
{
    int i = 0;
    int rc = SR_ERR_OK;
    char aacSyslogIp[2][SYSLOG_REMOTE_IP_MAX_LEN] = {{0}, {0}};
    bool abChangeFlag[CLIENT_SYSLOG_SERVER_NUM] = {0};

    if (!pstExistFlag->abSyslogFlag[0] && !pstExistFlag->abSyslogFlag[1]) {
        return SR_ERR_OK;
    } else {
        for (i = 0; i < CLIENT_SYSLOG_SERVER_NUM; i++) {
            if (pstExistFlag->abSyslogFlag[i]) {
                strncpy(aacSyslogIp[i], pstSystem->aacSyslogIp[i], sizeof(aacSyslogIp[i]) - 1);
            } else {
                strncpy(aacSyslogIp[i], pstOldSystem->aacSyslogIp[i], sizeof(aacSyslogIp[i]) - 1);
            }

            if (0 == strcmp(aacSyslogIp[i], pstOldSystem->aacSyslogIp[i])) {
                abChangeFlag[i] = false;
            } else {
                abChangeFlag[i] = true;
            }
        }
    }

    if (!abChangeFlag[0] && !abChangeFlag[1]) {
        LOG_WRITE(EN_LOG_NOTICE, "syslog server already existed!");
        return SR_ERR_OK;
    }

    if (HAVE_SAME_SERVER_IP(aacSyslogIp[0], aacSyslogIp[1])) {
        LOG_WRITE(EN_LOG_ERROR, "syslog server1 and server2 have the same IP(%s)!", aacSyslogIp[1]);
        goto END_LABEL;
    }

    /* delete old syslog */
    for (i = 0; i < CLIENT_SYSLOG_SERVER_NUM; i++) {
        if (!abChangeFlag[i]) {
            continue;
        }

        if (strlen(pstOldSystem->aacSyslogIp[i]) > 0) {
            rc = DSC_DelOcSyslogServer((char *)pstOldSystem->aacSyslogIp[i]);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_DelOcSyslogServer(%s) error !", pstOldSystem->aacSyslogIp[i]);
                goto END_LABEL;
            }
        }
    }

    /* add new syslog */
    for (i = 0; i < CLIENT_SYSLOG_SERVER_NUM; i++) {
        if (!abChangeFlag[i]) {
            continue;
        }

        if (strlen(pstSystem->aacSyslogIp[i]) > 0) {
            rc = DSC_AddOcSyslogServerSpec((char *)pstSystem->aacSyslogIp[i], "UDP", (char *)pstSystem->aacSyslogIp[i], 514);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_AddOcSyslogServerSpec(%s) error !", pstSystem->aacSyslogIp[i]);
                goto END_LABEL;
            }
        }
    }

END_LABEL:
    return rc;
}

static int
__SetTacacsServer(const REST_SYSTEM_ST *pstSystem, const REST_SYSTEM_FLAG_ST *pstExistFlag, const REST_SYSTEM_ST *pstOldSystem)
{
    int i = 0;
    int rc = SR_ERR_OK;
    TACACS_SERVER_LIST_ST stServerList = {NULL, 0};
    TACACS_SERVER_ST stTacServer;

    if (pstExistFlag->abTacacsIpFlag[0] || pstExistFlag->abTacacsIpFlag[1]) {
        rc = DSC_GetTacServers(&stServerList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetTacServers error!");
            goto END_LABEL;
        }

        for (i = 0; i < stServerList.uiTotal; i++) {
            DSC_DelTacServer(stServerList.pstTacacsServer[i].acIp);
        }

        for (i = 0; i < TACACS_SERVER_MAX_NUM; i++) {
            memset(&stTacServer, 0, sizeof(stTacServer));
            if (pstExistFlag->abTacacsIpFlag[i]) {
                if ((0 == strlen(pstSystem->astTacacsServer[i].acIp)) && (0 == strlen(pstSystem->astTacacsServer[i].acKey))) {
                    /* delete old tacacs+ server */
                    continue;
                } else {
                    /* add new tacacs+ server */
                    snprintf(stTacServer.acIp, sizeof(stTacServer.acIp), "%s", pstSystem->astTacacsServer[i].acIp);
                    snprintf(stTacServer.acKey, sizeof(stTacServer.acKey), "%s", pstSystem->astTacacsServer[i].acKey);
                }
            } else {
                if ((0 != strlen(pstOldSystem->astTacacsServer[i].acIp)) && (0 != strlen(pstOldSystem->astTacacsServer[i].acKey))) {
                    /* restore old tacacs+ server */
                    snprintf(stTacServer.acIp, sizeof(stTacServer.acIp), "%s", pstOldSystem->astTacacsServer[i].acIp);
                    snprintf(stTacServer.acKey, sizeof(stTacServer.acKey), "%s", pstOldSystem->astTacacsServer[i].acKey);
                } else {
                    /* not restore old tacacs+ server */
                    continue;
                }
            }
            snprintf(stTacServer.acName, sizeof(stTacServer.acName), "%d", i + 1);             /*fixed*/
            stTacServer.uiPort = TACACS_SERVER_DEFAULT_PORT;            /*fixed*/
            stTacServer.uiTimeOut = TACACS_DEFAULT_TIMEOUT;          /*fixed*/
            rc = DSC_AddTacServer(&stTacServer);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Add TacacsServer%d failed!", i + 1);
                goto END_LABEL;
            }
        }
    }

END_LABEL:
    DEF_SAFE_FREE(stServerList.pstTacacsServer);
    return rc;
}

static int
__SetIpv6(char *pcEthName, char *pcIpv6Mode, char *pcIpv6Addr, char *pcIpv6Gateway)
{
    int rc = SR_ERR_OK;
    ST_NETWORK_IPV6_STATIC stNetIpv6;
    EN_NETWORK_IP_ADDRESS_ORIGIN enIpv6Origin = EN_NETWORK_ORIGIN_OTHER;

    memset(&stNetIpv6, 0, sizeof(stNetIpv6));

    if (SR_ERR_OK != DSC_EthOriginStrToValue(pcIpv6Mode, &enIpv6Origin)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid %s ipv6-mode: %s", pcEthName, pcIpv6Mode);
        goto END_LABEL;
    }

    if (EN_NETWORK_ORIGIN_STATIC == enIpv6Origin) {
        rc = UTIL_SeparateIpv6AddrAndPrefix(pcIpv6Addr, stNetIpv6.acIpAddress, sizeof(stNetIpv6.acIpAddress), &stNetIpv6.ucPrefixLen);
        if (rc != SR_ERR_OK) {
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "Invalid %s ipv6: %s,%s,%s", pcEthName, pcIpv6Mode, pcIpv6Addr, pcIpv6Gateway);
            goto END_LABEL;
        }

        snprintf(stNetIpv6.acDefaultGateway, sizeof(stNetIpv6.acDefaultGateway), "%s", pcIpv6Gateway);
        rc = DSC_SetEthIpv6Static(pcEthName, &stNetIpv6);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    } else if (EN_NETWORK_ORIGIN_DHCP == enIpv6Origin) {
        rc = DSC_SetEthDhcpCli(pcEthName, EN_NETWORK_TYPE_IPV6);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    } else if (EN_NETWORK_ORIGIN_LINK_LAYER == enIpv6Origin) {
        rc = DSC_SetEthIpv6Linklayer(pcEthName, EN_NETWORK_ORIGIN_LINK_LAYER);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid %s ipv6: %s,%s,%s", pcEthName, pcIpv6Mode, pcIpv6Addr, pcIpv6Gateway);
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}

static void
__REST_SystemDataRollback(REST_SYSTEM_ST *pstOldSystem, REST_SYSTEM_FLAG_ST *pstExistFlag)
{
    int i = 0;
    TACACS_SERVER_LIST_ST stServerList = {NULL, 0};
    TACACS_SERVER_ST stTacServer;
    AUTO_CFG_DATA_ST stAutoCfg;
    ST_NETWORK_IPV4_STATIC stIpv4Cfg;
    REST_SYSTEM_ST stCurSystem;
    char *pcOperLine = NULL;

    if (NULL == pstOldSystem || NULL == pstExistFlag) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter is NULL");
        return;
    }

    if (pstExistFlag->bNodeNameFlag) {
        DSC_SetNodeId(pstOldSystem->acNodeName, sizeof(pstOldSystem->acNodeName));
    }

    if (pstExistFlag->abNtpFlag[0] || pstExistFlag->abNtpFlag[1]) {
        /* get current ntp */
        memset(&stCurSystem, 0, sizeof(stCurSystem));
        if (SR_ERR_OK == __GetNtpServer(&stCurSystem)) {
            __SetNtpServer(pstOldSystem, (const REST_SYSTEM_FLAG_ST *)pstExistFlag, (const REST_SYSTEM_ST *)(&stCurSystem));
        }

    }

    if (pstExistFlag->abSyslogFlag[0] || pstExistFlag->abSyslogFlag[1]) {
        /* get current syslog */
        memset(&stCurSystem, 0, sizeof(stCurSystem));
        if (SR_ERR_OK == __GetSyslogServer(&stCurSystem)) {
            __SetSyslogServer(pstOldSystem, (const REST_SYSTEM_FLAG_ST *)pstExistFlag, (const REST_SYSTEM_ST *)(&stCurSystem));
        }

    }

    if (pstExistFlag->abTacacsIpFlag[0] || pstExistFlag->abTacacsIpFlag[1]) {
        memset(&stServerList, 0x00, sizeof(stServerList));
        stServerList.pstTacacsServer = NULL;
        if (SR_ERR_OK == DSC_GetTacServers(&stServerList)) {
            for (i = 0; i < stServerList.uiTotal; i++) {
                DSC_DelTacServer(stServerList.pstTacacsServer[i].acIp);
            }
        }
        for (i = 0; i < TACACS_SERVER_MAX_NUM; i++) {
            if (0 == strlen(pstOldSystem->astTacacsServer[i].acIp)) {
                continue;
            }

            memset(&stTacServer, 0x00, sizeof(stTacServer));
            snprintf(stTacServer.acIp, sizeof(stTacServer.acIp), "%s", pstOldSystem->astTacacsServer[i].acIp);
            snprintf(stTacServer.acKey, sizeof(stTacServer.acKey), "%s", pstOldSystem->astTacacsServer[i].acKey);
            snprintf(stTacServer.acName, sizeof(stTacServer.acName), "%d", i + 1); /*fixed*/
            stTacServer.uiPort = TACACS_SERVER_DEFAULT_PORT;         /*fixed*/
            stTacServer.uiTimeOut = TACACS_DEFAULT_TIMEOUT;          /*fixed*/
            DSC_AddTacServer(&stTacServer);
        }
        DEF_SAFE_FREE(stServerList.pstTacacsServer);
    }
#if defined(__OCS__)
    /* clear compile warning */
    pcOperLine = pcOperLine;
    memset(&stAutoCfg, 0, sizeof(stAutoCfg));
#else
    for (i = 0; i < REST_GetLinePathMax(); i++) {
        pcOperLine = (EN_REST_OPTICAL_PATH_W == i) ? WORKING_LINE_YANG : PROTECTION_LINE_YANG;
        if (pstExistFlag->abFiberTypeFlag[i] || pstExistFlag->abLaunchPwrFlag[i] || pstExistFlag->abDropPwrFlag[i] || pstExistFlag->abCarriersFlag[i]) {
            memset(&stAutoCfg, 0, sizeof(stAutoCfg));
            stAutoCfg.enFiberType = pstOldSystem->aenFiberType[i];
            stAutoCfg.dLauchingPower = pstOldSystem->adLauchingPower[i];
            stAutoCfg.dTargetPower = pstOldSystem->adDropPower[i];
            stAutoCfg.ucNumOfCarriers = pstOldSystem->aucCarries[i];
            DSC_AutoSetCfg(pcOperLine, &stAutoCfg);
        }

        if (pstExistFlag->abLongSpanFlag[i]) {
            DSC_AutoSetLongSpan(pcOperLine, pstOldSystem->abLongSpan[i]);
        }
    }
#endif

    if (pstExistFlag->bTimezone) {
        DSC_SetTimezoneUtcOffset(pstOldSystem->sTimezone * 60);
    }

    if (pstExistFlag->bIpv6ModeFlag || pstExistFlag->bIpv6AddrFlag || pstExistFlag->bIpv6GatewayFlag) {
        __SetIpv6(DEF_ETH0_NAME_STR, pstOldSystem->acIpv6Mode, pstOldSystem->acIPv6, pstOldSystem->acGatewayV6);
    }

    if (pstExistFlag->bIpv4Flag && pstExistFlag->bNetmaskFlag && pstExistFlag->bGatewayFlag) {
        memset(&stIpv4Cfg, 0, sizeof(stIpv4Cfg));
        memcpy(stIpv4Cfg.acIpAddress, pstOldSystem->acIPV4, sizeof(stIpv4Cfg.acIpAddress) - 1);
        memcpy(stIpv4Cfg.acNetMask, pstOldSystem->acNetmask, sizeof(stIpv4Cfg.acNetMask) - 1);
        memcpy(stIpv4Cfg.acDefaultGateway, pstOldSystem->acGatewayV4, sizeof(stIpv4Cfg.acDefaultGateway) - 1);
        DSC_SetEthIpv4Static(DEF_ETH0_NAME_STR, &stIpv4Cfg);
    }

    return;
}


int
REST_SetSystemOptional(REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag)
{
    int rc = SR_ERR_OK;
    AUTO_CFG_DATA_ST stAutoCfg;
    ST_NETWORK_IPV4_STATIC stIpv4Cfg;
    REST_SYSTEM_ST stOldSystem;
    char *pcOperLine = NULL;
    int i = 0;
    char acTimeZone[8] = {0};

    memset(&stAutoCfg, 0, sizeof(stAutoCfg));
    memset(&stIpv4Cfg, 0, sizeof(stIpv4Cfg));

    /* rollback old data when excuting failed */
    memset(&stOldSystem, 0x00, sizeof(stOldSystem));
    rc = REST_GetSystemOptional(&stOldSystem, pstExistFlag);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    rc = __REST_CheckSystemOptional(pstSystem, pstExistFlag, &stOldSystem);
    if (SR_ERR_OK != rc) {
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* set nodename */
    if (pstExistFlag->bNodeNameFlag) {
        rc = DSC_SetNodeId(pstSystem->acNodeName, sizeof(pstSystem->acNodeName));
        if (rc != SR_ERR_OK) {
            goto ERR_LABEL;
        }
    }

    /*set ntp1/2*/
    rc = __SetNtpServer((const REST_SYSTEM_ST *)pstSystem, (const REST_SYSTEM_FLAG_ST *)pstExistFlag, (const REST_SYSTEM_ST *)&stOldSystem);
    if (rc != SR_ERR_OK) {
        goto DATA_ROLLBACK_LABEL;
    }

    /*set syslog1/2*/
    rc = __SetSyslogServer((const REST_SYSTEM_ST *)pstSystem, (const REST_SYSTEM_FLAG_ST *)pstExistFlag, (const REST_SYSTEM_ST *)&stOldSystem);
    if (rc != SR_ERR_OK) {
        goto DATA_ROLLBACK_LABEL;
    }

    /* set tacacs server1/2 */
    rc = __SetTacacsServer((const REST_SYSTEM_ST *)pstSystem, (const REST_SYSTEM_FLAG_ST *)pstExistFlag, (const REST_SYSTEM_ST *)&stOldSystem);
    if (rc != SR_ERR_OK) {
        goto DATA_ROLLBACK_LABEL;
    }
#if 0
    if (pstExistFlag->bFiberTypeFlag || pstExistFlag->bLaunchPwrFlag || pstExistFlag->bDropPwrFlag || pstExistFlag->bCarriersFlag) {
        stAutoCfg.enFiberType = pstExistFlag->bFiberTypeFlag ? pstSystem->enFiberType : stOldSystem.enFiberType;
        stAutoCfg.dLauchingPower = pstExistFlag->bLaunchPwrFlag ? pstSystem->dLauchingPower : stOldSystem.dLauchingPower;
        stAutoCfg.dTargetPower = pstExistFlag->bDropPwrFlag ? pstSystem->dDropPower : stOldSystem.dDropPower;
        stAutoCfg.ucNumOfCarriers = pstExistFlag->bCarriersFlag ? pstSystem->ucCarries : stOldSystem.ucCarries;
        rc = DSC_AutoSetCfg(&stAutoCfg);
        if (rc != SR_ERR_OK) {
            goto DATA_ROLLBACK_LABEL;
        }
    }
#else
#if defined(__OCS__)
    /* clear compile warning */
    i = i;
    pcOperLine = pcOperLine;
#else

    for (i = 0; i < REST_GetLinePathMax(); i++) {
        pcOperLine = (EN_REST_OPTICAL_PATH_W == i) ? WORKING_LINE_YANG : PROTECTION_LINE_YANG;
        if (pstExistFlag->abFiberTypeFlag[i]) {
            if ( pstSystem->aenFiberType[i] != stOldSystem.aenFiberType[i]) {
                rc = DSC_AutoSetCfgFibertype(pcOperLine, pstSystem->aenFiberType[i]);
                if (rc != SR_ERR_OK) {
                    goto DATA_ROLLBACK_LABEL;
                }
            }
        }

        if (pstExistFlag->abLaunchPwrFlag[i]) {
            rc = DSC_AutoSetCfgLanchingPower(pcOperLine, pstSystem->adLauchingPower[i]);
            if (rc != SR_ERR_OK) {
                goto DATA_ROLLBACK_LABEL;
            }
        }

        if (pstExistFlag->abDropPwrFlag[i]) {
            rc = DSC_AutoSetCfgDropPower(pcOperLine, pstSystem->adDropPower[i]);
            if (rc != SR_ERR_OK) {
                goto DATA_ROLLBACK_LABEL;
            }
        }

        if (pstExistFlag->abCarriersFlag[i]) {
            rc = DSC_AutoSetNumOfCarriers(pcOperLine, pstSystem->aucCarries[i]);
            if (rc != SR_ERR_OK) {
                goto DATA_ROLLBACK_LABEL;
            }
        }
        if (pstExistFlag->abLongSpanFlag[i]) {
            DSC_AutoSetLongSpan(pcOperLine, pstSystem->abLongSpan[i]);
            if (rc != SR_ERR_OK) {
                goto DATA_ROLLBACK_LABEL;
            }
        }

    }
#endif

#endif

    if (pstExistFlag->bTimezone) {
        snprintf(acTimeZone, sizeof(acTimeZone), "%d", pstSystem->sTimezone);
        rc = DSC_SetOcTimezoneName(acTimeZone);
        if (rc != SR_ERR_OK) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    /* set ipv6 */
    if (pstExistFlag->bIpv6ModeFlag || pstExistFlag->bIpv6AddrFlag || pstExistFlag->bIpv6GatewayFlag) {
        rc = __SetIpv6(DEF_ETH0_NAME_STR, pstSystem->acIpv6Mode, pstSystem->acIPv6, pstSystem->acGatewayV6);
        if (rc != SR_ERR_OK) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    /* set ip at last !!! */
    if (pstExistFlag->bIpv4Flag && pstExistFlag->bGatewayFlag && pstExistFlag->bNetmaskFlag) {
        memset(&stIpv4Cfg, 0, sizeof(stIpv4Cfg));
        memcpy(stIpv4Cfg.acIpAddress, pstSystem->acIPV4, sizeof(stIpv4Cfg.acIpAddress) - 1);
        memcpy(stIpv4Cfg.acNetMask, pstSystem->acNetmask, sizeof(stIpv4Cfg.acNetMask) - 1);

        memcpy(stIpv4Cfg.acDefaultGateway, pstSystem->acGatewayV4, sizeof(stIpv4Cfg.acDefaultGateway) - 1);
        rc = DSC_SetEthIpv4Static(DEF_ETH0_NAME_STR, &stIpv4Cfg);
        if (rc != SR_ERR_OK) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    /* fandy: update nodename gloabl for event log */
    if (pstExistFlag->bNodeNameFlag) {
        REST_SetNodeName(pstSystem->acNodeName);
    }

    return SR_ERR_OK;

DATA_ROLLBACK_LABEL:

    __REST_SystemDataRollback(&stOldSystem, pstExistFlag);

ERR_LABEL:
    return rc;
}

int
REST_GetSystemOptional(REST_SYSTEM_ST *pstSystem, REST_SYSTEM_FLAG_ST *pstExistFlag)
{
    int rc = SR_ERR_OK;
    DEVICE_CONFIG_ST stDeviceConfig;
    size_t i = 0, j = 0;
    TACACS_SERVER_LIST_ST stServer = {NULL, 0};
    AUTO_CFG_DATA_ST stAutoConfig;
    ST_NETWORK_IPV4_STATIC stIpv4Static;
    ST_NETWORK_IPV6_STATIC stIpv6State;
    char acTimeZone[8] = {0};

    memset(&stDeviceConfig, 0, sizeof(stDeviceConfig));
    memset(&stAutoConfig, 0, sizeof(stAutoConfig));
    memset(&stIpv4Static, 0, sizeof(stIpv4Static));
    memset(&stIpv6State, 0, sizeof(stIpv6State));

    /* get nodename*/
    if (pstExistFlag->bNodeNameFlag || pstExistFlag->bIpv4Flag || pstExistFlag->bNetmaskFlag || pstExistFlag->bGatewayFlag) {
        rc = DSC_GetNodeId(pstSystem->acNodeName, sizeof(pstSystem->acNodeName) - 1);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    }

    /* get ipv4 */
    if (pstExistFlag->bIpv4Flag || pstExistFlag->bNetmaskFlag || pstExistFlag->bGatewayFlag) {
        rc = DSC_GetEthIpv4State(DEF_ETH0_NAME_STR, &stIpv4Static);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
        snprintf(pstSystem->acGatewayV4, sizeof(pstSystem->acGatewayV4), "%s", stIpv4Static.acDefaultGateway);
        snprintf(pstSystem->acNetmask, sizeof(pstSystem->acNetmask), "%s", stIpv4Static.acNetMask);
        snprintf(pstSystem->acIPV4, sizeof(pstSystem->acIPV4), "%s", stIpv4Static.acIpAddress);
    }

    /* get ipv6 */
    if (pstExistFlag->bIpv6ModeFlag || pstExistFlag->bIpv6AddrFlag || pstExistFlag->bIpv6GatewayFlag) {
        rc = DSC_GetEthIpv6State(DEF_ETH0_NAME_STR, &stIpv6State);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }

        rc = DSC_GetEthIpv6OriginStr(DEF_ETH0_NAME_STR, pstSystem->acIpv6Mode, sizeof(pstSystem->acIpv6Mode));
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }

        if ((0 == strcmp(stIpv6State.acIpAddress, "0::0")) || (0 == strcmp(stIpv6State.acIpAddress, "::"))) {
            memset(pstSystem->acIPv6, 0, sizeof(pstSystem->acIPv6));
        } else {
            snprintf(pstSystem->acIPv6, sizeof(pstSystem->acIPv6), "%s/%d", stIpv6State.acIpAddress, stIpv6State.ucPrefixLen);
        }

        if ((0 == strcmp(stIpv6State.acDefaultGateway, "0::0")) || (0 == strcmp(stIpv6State.acDefaultGateway, "::"))) {
            memset(pstSystem->acGatewayV6, 0, sizeof(pstSystem->acGatewayV6));
        } else {
            snprintf(pstSystem->acGatewayV6, sizeof(pstSystem->acGatewayV6), "%s", stIpv6State.acDefaultGateway);
        }
    }

    /* get ntp server */
    if (pstExistFlag->abNtpFlag[0] || pstExistFlag->abNtpFlag[1]) {
        rc = __GetNtpServer(pstSystem);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    }

    /* get syslog server */
    if (pstExistFlag->abSyslogFlag[0] || pstExistFlag->abSyslogFlag[1]) {
        rc = __GetSyslogServer(pstSystem);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }
    }

    /* get tacacs server */
    if (pstExistFlag->abTacacsIpFlag[0] || pstExistFlag->abTacacsIpFlag[1]) {
        rc = DSC_GetTacServers(&stServer);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_NOTICE, "get tacacs server %d error!", rc);
        }

        for (i = 0; i < stServer.uiTotal; i++) {
            if (i >= 2) {
                break;
            }

            /* sort by priority */
            j = (0 == strcmp(stServer.pstTacacsServer[i].acName, STR_1)) ? 0 : 1;

            snprintf(pstSystem->astTacacsServer[j].acName, sizeof(pstSystem->astTacacsServer[j].acName), "%s", stServer.pstTacacsServer[i].acName);
            snprintf(pstSystem->astTacacsServer[j].acIp, sizeof(pstSystem->astTacacsServer[j].acIp), "%s", stServer.pstTacacsServer[i].acIp);
            snprintf(pstSystem->astTacacsServer[j].acKey, sizeof(pstSystem->astTacacsServer[j].acKey), "%s", stServer.pstTacacsServer[i].acKey);
            pstSystem->astTacacsServer[j].uiPort = stServer.pstTacacsServer[i].uiPort;
        }
    }

#if defined(__OCS__)

#else
    /*get auto control data */
    for (i = 0; i < REST_GetLinePathMax(); i++) {
        if (pstExistFlag->abFiberTypeFlag[i] || pstExistFlag->abLaunchPwrFlag[i] || pstExistFlag->abDropPwrFlag[i] || pstExistFlag->abCarriersFlag[i] ) {
            memset(&stAutoConfig, 0, sizeof(stAutoConfig));
            rc = DSC_AutoGetCfg((i == EN_REST_OPTICAL_PATH_W) ? WORKING_LINE_YANG : PROTECTION_LINE_YANG, &stAutoConfig);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "DSC_AutoGetCfg error, i:%lu", i);
                goto END_LABEL;
            }
            pstSystem->aucCarries[i] = stAutoConfig.ucNumOfCarriers;
            pstSystem->aenFiberType[i] = stAutoConfig.enFiberType;
            pstSystem->adLauchingPower[i] = stAutoConfig.dLauchingPower;
            pstSystem->adDropPower[i] = stAutoConfig.dTargetPower;
            pstSystem->abLongSpan[i] = stAutoConfig.bLongSpan;
        }
    }
#endif

    if (pstExistFlag->bTimezone) {
        rc = DSC_GetOcTimezoneNameConfig(acTimeZone, sizeof(acTimeZone) - 1);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get timezone failed");
            goto END_LABEL;
        }
        pstSystem->sTimezone = (short)atoi(acTimeZone);
    }

    rc = SR_ERR_OK;

END_LABEL:

    DEF_SAFE_FREE(stServer.pstTacacsServer);

    return rc;
}

int
REST_ParseApsd(const cJSON *pstJson, bool *pbApsdEnable)
{
    char acBuf[32] = {0};
    JSON_OBJ_ST astJsonObj[] = {{STR_APSD_ENABLE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(acBuf), sizeof(acBuf)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(acBuf, STR_ENABLE)) {
        *pbApsdEnable = true;
    } else if (0 == strcasecmp(acBuf, STR_DISABLE)) {
        *pbApsdEnable = false;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Error by parsing apsd: %s", acBuf);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static int
__REST_ParseReflectionCfg(const cJSON *pstJson, REST_MUX_CFG_ST *pstMuxCfg)
{
    REST_MUX_CFG_ST stMuxCfg;
    JSON_OBJ_ST astJsonObj[] = {{STR_MUXT_DIFFER_HIGH_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stMuxtDifferThr.dHighThr), sizeof(stMuxCfg.stMuxtDifferThr.dHighThr)},
                                {STR_MUXT_DIFFER_LOW_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stMuxtDifferThr.dLowThr), sizeof(stMuxCfg.stMuxtDifferThr.dLowThr)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stMuxCfg, 0, sizeof(stMuxCfg));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(&(pstMuxCfg->stMuxtDifferThr), &(stMuxCfg.stMuxtDifferThr), sizeof(stMuxCfg.stMuxtDifferThr));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}



static int
__REST_ParseConnectivityCfg(const cJSON *pstJson, REST_MUX_CFG_ST *pstMuxCfg)
{
    REST_MUX_CFG_ST stMuxCfg;
    JSON_OBJ_ST astJsonObj[] = {{STR_MUXR_DIFFER_HIGH_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stMuxrDifferThr.dHighThr), sizeof(stMuxCfg.stMuxrDifferThr.dHighThr)},
                                {STR_MUXR_DIFFER_LOW_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stMuxrDifferThr.dLowThr), sizeof(stMuxCfg.stMuxrDifferThr.dLowThr)},
                                {STR_DEMUXT_DIFFER_HIGH_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stDemuxtDifferThr.dHighThr), sizeof(stMuxCfg.stDemuxtDifferThr.dHighThr)},
                                {STR_DEMUXT_DIFFER_LOW_THR, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stMuxCfg.stDemuxtDifferThr.dLowThr), sizeof(stMuxCfg.stDemuxtDifferThr.dLowThr)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stMuxCfg, 0, sizeof(stMuxCfg));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(&(pstMuxCfg->stMuxrDifferThr), &(stMuxCfg.stMuxrDifferThr), sizeof(stMuxCfg.stMuxrDifferThr));
    memcpy(&(pstMuxCfg->stDemuxtDifferThr), &(stMuxCfg.stDemuxtDifferThr), sizeof(stMuxCfg.stDemuxtDifferThr));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


int
REST_ParseMuxConf(const cJSON *pstJson, REST_MUX_CFG_ST *pstMuxCfg)
{
    cJSON *pstObjJson = NULL;
    REST_MUX_CFG_ST stMuxCfg;

    memset(&stMuxCfg, 0, sizeof(stMuxCfg));

    /*parse "reflection" node*/
    pstObjJson = cJSON_GetObjectItem(pstJson, STR_REFLECTION);
    if (NULL != pstObjJson && cJSON_IsObject(pstObjJson)) {
        /*parse data*/
        if (OPLK_OK != __REST_ParseReflectionCfg((const cJSON *)pstObjJson, &stMuxCfg)) {
            LOG_WRITE(EN_LOG_ERROR, "parse %s node failed", STR_REFLECTION);
            goto ERR_LABEL;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "not find %s node", STR_REFLECTION);
        goto ERR_LABEL;
    }

    /*parse "connectivity" node*/
    pstObjJson = cJSON_GetObjectItem(pstJson, STR_CONNECTIVITY);
    if (NULL != pstObjJson && cJSON_IsObject(pstObjJson)) {
        /*parse data*/
        if (OPLK_OK != __REST_ParseConnectivityCfg((const cJSON *)pstObjJson, &stMuxCfg)) {
            LOG_WRITE(EN_LOG_ERROR, "parse %s node failed", STR_CONNECTIVITY);
            goto ERR_LABEL;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "not find %s node", STR_CONNECTIVITY);
        goto ERR_LABEL;
    }

    memcpy(pstMuxCfg, &stMuxCfg, sizeof(stMuxCfg));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


int
REST_SetMuxCfg(REST_MUX_CFG_ST *pstMuxCfg)
{
    int rc = SR_ERR_OK;
    REST_MUX_CFG_ST stOldMuxCfg;

    /* get current mux config */
    memset(&stOldMuxCfg, 0, sizeof(stOldMuxCfg));
    rc = REST_GetMuxCfg(&stOldMuxCfg);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_GetMuxCfg");
        goto END_LABEL;
    }

    /* set thr to mux */
    rc = DSC_CvSetDifferThr(EN_DIFFER_REF, pstMuxCfg->stMuxtDifferThr.dHighThr, pstMuxCfg->stMuxtDifferThr.dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set cv treshold: %d,  highThr: %.2f, lowThr: %.2f error", EN_DIFFER_REF, pstMuxCfg->stMuxtDifferThr.dHighThr, pstMuxCfg->stMuxtDifferThr.dLowThr);
        goto END_LABEL;
    }

    rc = DSC_CvSetDifferThr(EN_DIFFER_MUX_R, pstMuxCfg->stMuxrDifferThr.dHighThr, pstMuxCfg->stMuxrDifferThr.dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set cv treshold: %d,  highThr: %.2f, lowThr: %.2f error", EN_DIFFER_MUX_R, pstMuxCfg->stMuxrDifferThr.dHighThr, pstMuxCfg->stMuxrDifferThr.dLowThr);
        /* data rollback */
        (void)DSC_CvSetDifferThr(TCA_IDX_CV_REF, stOldMuxCfg.stMuxtDifferThr.dHighThr, stOldMuxCfg.stMuxtDifferThr.dLowThr);
        goto END_LABEL;
    }

    rc = DSC_CvSetDifferThr(EN_DIFFER_MUX_T, pstMuxCfg->stDemuxtDifferThr.dHighThr, pstMuxCfg->stDemuxtDifferThr.dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set cv treshold: %d,  highThr: %.2f, lowThr: %.2f error", EN_DIFFER_MUX_T, pstMuxCfg->stDemuxtDifferThr.dHighThr, pstMuxCfg->stDemuxtDifferThr.dLowThr);
        /* data rollback */
        (void)DSC_CvSetDifferThr(EN_DIFFER_REF, stOldMuxCfg.stMuxtDifferThr.dHighThr, stOldMuxCfg.stMuxtDifferThr.dLowThr);
        (void)DSC_CvSetDifferThr(EN_DIFFER_MUX_R, stOldMuxCfg.stMuxrDifferThr.dHighThr, stOldMuxCfg.stMuxrDifferThr.dLowThr);
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

int
REST_GetMuxCfg(REST_MUX_CFG_ST *pstMuxCfg)
{
    int rc = SR_ERR_OK;
    ST_CV_CONFIG stCfg;

    /* Get connectivity validation all config                                   */
    memset(&stCfg, 0x00, sizeof(stCfg));
    rc = DSC_CvGetConfig(&stCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "CV get config failed!");
        goto END_LABEL;
    }

    memcpy(&(pstMuxCfg->stMuxtDifferThr), &stCfg.astThr[EN_DIFFER_REF], sizeof(ST_DIFFER_THR));
    memcpy(&(pstMuxCfg->stMuxrDifferThr), &stCfg.astThr[EN_DIFFER_MUX_R], sizeof(ST_DIFFER_THR));
    memcpy(&(pstMuxCfg->stDemuxtDifferThr), &stCfg.astThr[EN_DIFFER_MUX_T], sizeof(ST_DIFFER_THR));

END_LABEL:

    return rc;
}

int
REST_GetMuxState(REST_MUX_STATE_ST *pstMuxState)
{
    int rc = SR_ERR_OK;
    ST_CV_STATE stState;

    memset(&stState, 0, sizeof(stState));
    rc = DSC_CvGetState(&stState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "CV get state failed!");
        goto END_LABEL;
    }

    pstMuxState->dMuxtOutPwr = stState.astReflect[EN_REFLECT_MUX].dOutputPower;
    pstMuxState->dMuxtReflectPwr = stState.astReflect[EN_REFLECT_MUX].dRefPower;
    pstMuxState->dMuxtDifferPwr = stState.astReflect[EN_REFLECT_MUX].dDifferPower;

    pstMuxState->dMuxrOutPwr = stState.stMux.dOutputPowerR;
    pstMuxState->dMuxrInputPwr = stState.stMux.dInputPowerR;
    pstMuxState->dMuxrDifferPwr = stState.stMux.dDifferPowerR;

    pstMuxState->dDemuxtOutPwr = stState.stMux.dOutputPowerT;
    pstMuxState->dDemuxtInputPwr = stState.stMux.dInputPowerT;
    pstMuxState->dDemuxtDifferPwr = stState.stMux.dDifferPowerT;

END_LABEL:

    return rc;
}

