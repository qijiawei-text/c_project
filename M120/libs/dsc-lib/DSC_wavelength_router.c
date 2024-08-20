/******************************************************************************
*
* Copyright(C), 2020, OPLINK Tech.Co., Ltd
*
* Filename:      dsc_wavelength_router.c
* Description:   dsc interfaces of openconfig-wavelength-router.yang
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo        2020/11/09       Ver 1.0.0    Initial file.
*
******************************************************************************/

#include "DSC_inc.h"

WR_ASE_STATUS_EN
DSC_WrAseStatusToEnum(char *pcAseStatus)
{
    WR_ASE_STATUS_EN enAseStatus = WR_ASE_STATUS_MAX;

    if (0 == strcmp(pcAseStatus, WR_STR_PRESENT)) {
        enAseStatus = WR_ASE_STATUS_PRESENT;
    } else if (0 == strcmp(pcAseStatus, WR_STR_NOT_PRESENT)) {
        enAseStatus = WR_ASE_STATUS_NOT_PRESENT;
    } else {
        enAseStatus = WR_ASE_STATUS_MAX;
    }

    return enAseStatus;
}

char *
DSC_WrAseStatusToStr(WR_ASE_STATUS_EN enAseStatus, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }
    switch (enAseStatus) {
    case WR_ASE_STATUS_PRESENT:
        snprintf(pcBuf, nSize, "%s", WR_STR_PRESENT);
        break;
    case WR_ASE_STATUS_NOT_PRESENT:
        snprintf(pcBuf, nSize, "%s", WR_STR_NOT_PRESENT);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}

WR_ATTN_CONTROL_MODE_EN
DSC_WrAttnCtrlModeToEnum(char *pcAttnCtrlMode)
{
    WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode = WR_ATTN_CONTROL_MODE_MAX;

    if (NULL != strstr(pcAttnCtrlMode, WR_STR_ATTN_FIXED_LOSS)) {
        enAttnCtrlMode = WR_ATTENUATION_FIXED_LOSS;
    } else if (NULL != strstr(pcAttnCtrlMode, WR_STR_ATTN_DYNAMIC_LOSS)) {
        enAttnCtrlMode = WR_ATTENUATION_DYNAMIC_LOSS;
    } else if (0 != strstr(pcAttnCtrlMode, WR_STR_SET_ATTENUATION)) {
        enAttnCtrlMode = WR_SET_ATTENUATION;
    } else {
        enAttnCtrlMode = WR_ATTN_CONTROL_MODE_MAX;
    }

    return enAttnCtrlMode;
}

char *
DSC_WrAttnCtrlModeToStr(WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enAttnCtrlMode) {
    case WR_ATTENUATION_FIXED_LOSS:
        snprintf(pcBuf, nSize, "%s", WR_STR_ATTN_FIXED_LOSS);
        break;
    case WR_ATTENUATION_DYNAMIC_LOSS:
        snprintf(pcBuf, nSize, "%s", WR_STR_ATTN_DYNAMIC_LOSS);
        break;
    case WR_SET_ATTENUATION:
        snprintf(pcBuf, nSize, "%s", "com-alibaba-wavelength-router-ext:" WR_STR_SET_ATTENUATION);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}


WR_ADMIN_STATUS_EN
DSC_WrAdminStatusToEnum(char *pcAdminStatus)
{
    WR_ADMIN_STATUS_EN enAdminStatus = WR_ADMIN_STATUS_MAX;

    if (0 == strcmp(pcAdminStatus, WR_STR_ENABLED)) {
        enAdminStatus = WR_ADMIN_STATUS_ENABLED;
    } else if (0 == strcmp(pcAdminStatus, WR_STR_DISABLED)) {
        enAdminStatus = WR_ADMIN_STATUS_DISABLED;
    } else if (0 == strcmp(pcAdminStatus, WR_STR_MAINT)) {
        enAdminStatus = WR_ADMIN_STATUS_MAINT;
    } else {
        enAdminStatus = WR_ADMIN_STATUS_MAX;
    }

    return enAdminStatus;
}

char *
DSC_WrAdminStatusToStr(WR_ADMIN_STATUS_EN enAdminStatus, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enAdminStatus) {
    case WR_ADMIN_STATUS_ENABLED:
        snprintf(pcBuf, nSize, "%s", WR_STR_ENABLED);
        break;
    case WR_ADMIN_STATUS_DISABLED:
        snprintf(pcBuf, nSize, "%s", WR_STR_DISABLED);
        break;
    case WR_ADMIN_STATUS_MAINT:
        snprintf(pcBuf, nSize, "%s", WR_STR_MAINT);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}

WR_ASE_CONTROL_MODE_EN
DSC_WrAseCtrlModeToEnum(char *pcAseCtrlMode)
{
    WR_ASE_CONTROL_MODE_EN enAseCtrlMode = WR_ASE_CONTROL_MODE_MAX;

    if (0 == strcmp(pcAseCtrlMode, WR_STR_ASE_ENABLED)) {
        enAseCtrlMode = WR_ASE_ENABLED;
    } else if (0 == strcmp(pcAseCtrlMode, WR_STR_ASE_DISABLED)) {
        enAseCtrlMode = WR_ASE_DISABLED;
    } else if (0 == strcmp(pcAseCtrlMode, WR_STR_AUTO_ASE_ON_FAILURE)) {
        enAseCtrlMode = WR_AUTO_ASE_ON_FAILURE;
    } else if (0 == strcmp(pcAseCtrlMode, WR_STR_AUTO_ASE_FAILURE_AND_RESTORE)) {
        enAseCtrlMode = WR_AUTO_ASE_FAILURE_AND_RESTORE;
    } else {
        enAseCtrlMode = WR_ASE_CONTROL_MODE_MAX;
    }

    return enAseCtrlMode;
}

char *
DSC_WrAseCtrlModeToStr(WR_ASE_CONTROL_MODE_EN enAseCtrlMode, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enAseCtrlMode) {
    case WR_ASE_ENABLED:
        snprintf(pcBuf, nSize, "%s", WR_STR_ASE_ENABLED);
        break;
    case WR_ASE_DISABLED:
        snprintf(pcBuf, nSize, "%s", WR_STR_ASE_DISABLED);
        break;
    case WR_AUTO_ASE_ON_FAILURE:
        snprintf(pcBuf, nSize, "%s", WR_STR_AUTO_ASE_ON_FAILURE);
        break;
    case WR_AUTO_ASE_FAILURE_AND_RESTORE:
        snprintf(pcBuf, nSize, "%s", WR_STR_AUTO_ASE_FAILURE_AND_RESTORE);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}

WR_OPER_STATUS_EN
DSC_WrOperStatusToEnum(char *pcOperStatus)
{
    WR_OPER_STATUS_EN enOperStatus = WR_OPER_STATUS_MAX;

    if (0 == strcmp(pcOperStatus, WR_STR_UP)) {
        enOperStatus = WR_OPER_STATUS_UP;
    } else if (0 == strcmp(pcOperStatus, WR_STR_DOWN)) {
        enOperStatus = WR_OPER_STATUS_DOWN;
    } else {
        enOperStatus = WR_OPER_STATUS_MAX;
    }

    return enOperStatus;
}

char *
DSC_WrOperStatusToStr(WR_OPER_STATUS_EN enOperStatus, char *pcBuf, size_t nSize)
{
    if (NULL == pcBuf) {
        return NULL;
    }

    switch (enOperStatus) {
    case WR_OPER_STATUS_UP:
        snprintf(pcBuf, nSize, "%s", WR_STR_UP);
        break;
    case WR_OPER_STATUS_DOWN:
        snprintf(pcBuf, nSize, "%s", WR_STR_DOWN);
        break;
    default:
        return NULL;
    }

    return pcBuf;
}

int
DSC_WrInitChannelDefaultVal(CHANNEL_ST *pstChannel, uint32_t uiIndex)
{

    if (NULL == pstChannel) {
        return SR_ERR_INVAL_ARG;
    }

    memset(pstChannel, 0, sizeof(CHANNEL_ST));
    pstChannel->stCfg.uiIndex = uiIndex;
    pstChannel->stCfg.bSupperChannel = false;
    pstChannel->stCfg.enAdminStatus = WR_ADMIN_STATUS_ENABLED;
    pstChannel->stCfg.enAttnCtrlMode = WR_SET_ATTENUATION;

    return SR_ERR_OK;
}

int
DSC_WrGetMediaChannelsInfo(sr_session_ctx_t *pstSession, MEDIA_CHANNELS_ST *pstMediaChannels)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t i = 0, nCnt = 0;
    sr_session_ctx_t *pstSess = NULL;
    char *pcNodeName = NULL;
    char acXpath[DEF_BUF_SIZE_512] = {0};
    uint32_t uiDistributionIdx = 0;
    CHANNEL_ST *pstChannel = NULL;
    DISTRIBUTION_ST *pstDist = NULL;

    pstSess = (NULL == pstSession) ? DSC_GetOperSession() : pstSession;

    if (NULL == pstSess || NULL == pstMediaChannels) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), "%s", WR_XPATH_MEDIA_CHANNELS "//*");
    rc = get_items(pstSess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        if (SR_ERR_NOT_FOUND == rc) {
            pstMediaChannels->uiTotal = 0;
            LOG_WRITE(EN_LOG_NOTICE, "Not Find: %s", acXpath);
            return SR_ERR_OK;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    }

    memset(pstMediaChannels, 0, sizeof(MEDIA_CHANNELS_ST));

    for (i = 0; i < nCnt; i++) {
        /* sr_print_val(pstVal + i); */
        if (SR_LIST_T == pstVal[i].type) {
            pcNodeName = sr_xpath_node_name(pstVal[i].xpath);
            if (0 == strncmp(pcNodeName, WR_LIST_CHANNEL, strlen(WR_LIST_CHANNEL))) {
                pstMediaChannels->uiTotal++;
                pstMediaChannels->pastChanList = realloc(pstMediaChannels->pastChanList, pstMediaChannels->uiTotal * sizeof(CHANNEL_ST));
                if (NULL == pstMediaChannels->pastChanList) {
                    rc = ERR_NOMEM;
                    LOG_WRITE(EN_LOG_ERROR, "no enough memory");
                    goto END_LABEL;
                }
                pstChannel = &(pstMediaChannels->pastChanList[pstMediaChannels->uiTotal - 1]);
                memset(pstChannel, 0, sizeof(*pstChannel));
                /* Init default value */
                rc = DSC_WrInitChannelDefaultVal(pstChannel, 0);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by init channel");
                    goto END_LABEL;
                }

            } else if (0 == strncmp(pcNodeName, WR_LIST_DISTRIBUTION, strlen(WR_LIST_DISTRIBUTION))) {
                uiDistributionIdx = pstChannel->stSpecPwrProfile.uiTotal++;
                if (uiDistributionIdx >= WR_DISTRIBUTION_NUM_MAX) {
                    LOG_WRITE(EN_LOG_ERROR, "Not enough for distribution. id: %d", uiDistributionIdx);
                    rc = ERR_NOMEM;
                    goto END_LABEL;
                }
                /* Init default value */
                pstChannel->stSpecPwrProfile.astDist[uiDistributionIdx].stSpecPwrCfg.dTargetPwr = WR_INVALID_POWER;
                pstChannel->stSpecPwrProfile.astDist[uiDistributionIdx].stSpecPwrCfg.dAttn = WR_CHANNEL_ATTN_MAX;
            } else {
                /* ignore */
            }
            continue;
        }
        /* parse channel list */
        if (NULL != strstr(pstVal[i].xpath, WR_NODE_SPECTRUM_POWER_PROFILE)) {
            pstDist = &(pstChannel->stSpecPwrProfile.astDist[uiDistributionIdx]);
            /* spectrum-power-profile container */
            if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_LOW_FREQUENCY)) {
                pstDist->stSpecPwrCfg.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_UPPER_FREQUENCY)) {
                pstDist->stSpecPwrCfg.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_TARGET_POWER)) {
                pstDist->stSpecPwrCfg.dTargetPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ATTN)) {
                pstDist->stSpecPwrCfg.dAttn = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_LOW_FREQUENCY)) {
                pstDist->stSpecPwrState.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_UPPER_FREQUENCY)) {
                pstDist->stSpecPwrState.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_TARGET_POWER)) {
                pstDist->stSpecPwrState.dTargetPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ATTN)) {
                pstDist->stSpecPwrState.dAttn = pstVal[i].data.decimal64_val;
            } else {
                /* ignore */
            }

        } else {
            /* config/state/source/dest container */
            if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_INDEX)) {
                pstChannel->stCfg.uiIndex = pstVal[i].data.uint32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_CHANNEL_NAME)) {
                snprintf(pstChannel->stCfg.acName, sizeof(pstChannel->stCfg.acName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_LOW_FREQUENCY)) {
                pstChannel->stCfg.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_UPPER_FREQUENCY)) {
                pstChannel->stCfg.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ADMIN_STATUS)) {
                pstChannel->stCfg.enAdminStatus = DSC_WrAdminStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_SUPPER_CHANNEL)) {
                pstChannel->stCfg.bSupperChannel = pstVal[i].data.bool_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_SUPPER_CHANNEL_PARENT)) {
                pstChannel->stCfg.uiSupperChanIdx = pstVal[i].data.int32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ASE_CONTROL_MODE)) {
                pstChannel->stCfg.enAseCtrlMode = DSC_WrAseCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ASE_INJECTION_THRESHOLD)) {
                pstChannel->stCfg.dAseInjectThr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ATTN_CONTROL_MODE)) {
                pstChannel->stCfg.enAttnCtrlMode = DSC_WrAttnCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_SOURCE WR_NODE_CONFIG WR_NODE_PORT_NAME)) {
                snprintf(pstChannel->stSrcConnPort.acPortName, sizeof(pstChannel->stSrcConnPort.acPortName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_DEST WR_NODE_CONFIG WR_NODE_PORT_NAME)) {
                snprintf(pstChannel->stDestConnPort.acPortName, sizeof(pstChannel->stDestConnPort.acPortName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_INDEX)) {
                pstChannel->stState.uiIndex = pstVal[i].data.uint32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_CHANNEL_NAME)) {
                snprintf(pstChannel->stState.acName, sizeof(pstChannel->stState.acName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_LOW_FREQUENCY)) {
                pstChannel->stState.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_UPPER_FREQUENCY)) {
                pstChannel->stState.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ADMIN_STATUS)) {
                pstChannel->stState.enAdminStatus = DSC_WrAdminStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_SUPPER_CHANNEL)) {
                pstChannel->stState.bSupperChannel = pstVal[i].data.bool_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_SUPPER_CHANNEL_PARENT)) {
                pstChannel->stState.uiSupperChanIdx = pstVal[i].data.int32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ASE_CONTROL_MODE)) {
                pstChannel->stState.enAseCtrlMode = DSC_WrAseCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ASE_INJECTION_THRESHOLD)) {
                pstChannel->stState.dAseInjectThr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ATTN_CONTROL_MODE)) {
                pstChannel->stState.enAttnCtrlMode = DSC_WrAttnCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_OPER_STATUS)) {
                pstChannel->stState.enOperStatus = DSC_WrOperStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_OUTPUT_POWER)) {
                pstChannel->stState.dOutputPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ACTUAL_ATTN)) {
                pstChannel->stState.dActualAttn = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_SOURCE WR_NODE_STATE WR_NODE_PORT_NAME)) {
                /* ingnore */
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_DEST WR_NODE_STATE WR_NODE_PORT_NAME)) {
                /* ingnore */
            } else {
                /* ignore */
            }
        }

    }

END_LABEL:

    sr_free_values(pstVal, nCnt);

    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstMediaChannels->pastChanList);
    }

    return rc;
}


bool
__IsValidChannelFreq(uint64_t ulLowerFreq, uint64_t ulUpperFreq)
{
    uint64_t ulCenterFreq = 0;
    uint64_t ulChannelWidth = 0;
    uint64_t ulOffset = 0;

    if (ulLowerFreq >= ulUpperFreq) {
        goto ERR_LABEL;
    }

    if (ulLowerFreq < WR_LOWER_FREQUENCY_MIN || ulUpperFreq > WR_UPPER_FREQUENCY_MAX) {
        goto ERR_LABEL;
    }

    /* check center frequency */
    if ((ulLowerFreq + ulUpperFreq) % 2 != 0) {
        goto ERR_LABEL;
    }

    ulCenterFreq = (ulLowerFreq + ulUpperFreq) / 2;
    ulOffset = (ulCenterFreq > WR_CENTER_FREQUENCY_BASE) ? ulCenterFreq - WR_CENTER_FREQUENCY_BASE : WR_CENTER_FREQUENCY_BASE - ulCenterFreq;

    if (ulOffset % WR_CENTER_FREQ_OFFSET_UNIT != 0) {
        goto ERR_LABEL;
    }

    /* check channel width */
    ulChannelWidth = ulUpperFreq - ulLowerFreq;
    if ((ulChannelWidth % WR_CHANNEL_WIDTH_MIN) != 0) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;
}

int
__AddChannelToWaveplanList(bool bAllowSameChannel, uint64_t ulLowerFreq, uint64_t ulUpperFreq, WR_WAVEPLAN_LIST_ST *pstWaveplanList)
{
    /* int rc = SR_ERR_OK; */
    uint32_t i = 0, j = 0;
    bool bExistedFlag = false;

    if (NULL == pstWaveplanList) {
        goto ERR_LABEL;
    }

#if 0
    /* check channel frequency */
    if (false == __IsValidChannelFreq(ulLowerFreq, ulUpperFreq)) {
        goto ERR_LABEL;
    }
#endif
    /* LOG_WRITE(EN_LOG_NOTICE, "+++ lower: %ld, upper: %ld, uiTotal: %d", ulLowerFreq, ulUpperFreq, pstWaveplanList->uiTotal); */
    /* insert channel to list from small to large */
    for (i = 0; i < pstWaveplanList->uiTotal; i++) {
        if (ulUpperFreq <= pstWaveplanList->astWaveplan[i].ulLowerFreq) {
            /* LOG_WRITE(EN_LOG_NOTICE, "ok [%d] lower: %ld, upper: %ld, uiTotal: %d", i, pstWaveplanList->astWaveplan[i].ulLowerFreq, pstWaveplanList->astWaveplan[i].ulUpperFreq, pstWaveplanList->uiTotal); */
            /* OK, find insert position */
            break;
        } else if (ulLowerFreq >= pstWaveplanList->astWaveplan[i].ulUpperFreq) {
            /* LOG_WRITE(EN_LOG_NOTICE, "continue [%d] lower: %ld, upper: %ld, uiTotal: %d", i, pstWaveplanList->astWaveplan[i].ulLowerFreq, pstWaveplanList->astWaveplan[i].ulUpperFreq, pstWaveplanList->uiTotal); */
            continue;
        } else if ((ulLowerFreq == pstWaveplanList->astWaveplan[i].ulLowerFreq) &&
                   (ulUpperFreq == pstWaveplanList->astWaveplan[i].ulUpperFreq)) {
            bExistedFlag = true;
            /* LOG_WRITE(EN_LOG_NOTICE, "existed [%d] lower: %ld, upper: %ld, uiTotal: %d", i, pstWaveplanList->astWaveplan[i].ulLowerFreq, pstWaveplanList->astWaveplan[i].ulUpperFreq, pstWaveplanList->uiTotal); */
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Conflict channel. lower: %ld, upper: %ld", ulLowerFreq, ulUpperFreq);
            goto ERR_LABEL;
        }
    }

    /* the same channel has already existed, ignore */
    if (bExistedFlag) {
        if (bAllowSameChannel) {
            goto OK_LABEL;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "channel already existed. lower: %ld, upper: %ld", ulLowerFreq, ulUpperFreq);
            goto ERR_LABEL;
        }
    }

    if (pstWaveplanList->uiTotal >= WR_WAVEPLAN_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "the waveplan is more than %d", WR_WAVEPLAN_NUM_MAX);
        goto ERR_LABEL;
    }

    /* insert new channel to list */
    for (j = pstWaveplanList->uiTotal; j > i; j--) {
        memcpy(&(pstWaveplanList->astWaveplan[j]), &(pstWaveplanList->astWaveplan[j - 1]), sizeof(WR_WAVEPLAN_ST));
    }

    pstWaveplanList->astWaveplan[i].ulLowerFreq = ulLowerFreq;
    pstWaveplanList->astWaveplan[i].ulUpperFreq = ulUpperFreq;
    pstWaveplanList->uiTotal++;

OK_LABEL:

    return SR_ERR_OK;

ERR_LABEL:

    return SR_ERR_INVAL_ARG;
}

int
DSC_WrParseWaveplan(const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WAVEPLAN_LIST_ST *pstWaveplanList)
{
    uint32_t i = 0;
    WR_WAVEPLAN_LIST_ST stWaveplanList;
    uint64_t ulLowerFreq;
    uint64_t ulUpperFreq;
    int rc = SR_ERR_OK;

    if (NULL == pstMediaChannels || NULL == pstWaveplanList) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    memset(&stWaveplanList, 0, sizeof(stWaveplanList));

    for (i = 0; i < pstMediaChannels->uiTotal; i++) {
        ulLowerFreq = pstMediaChannels->pastChanList[i].stCfg.ulLowerFreq;
        ulUpperFreq = pstMediaChannels->pastChanList[i].stCfg.ulUpperFreq;

        /* check channel frequency */
        if (false == __IsValidChannelFreq(ulLowerFreq, ulUpperFreq)) {
            LOG_WRITE(EN_LOG_ERROR, "Invald frequency. index: %d, low: %ld, upper: %ld", pstMediaChannels->pastChanList[i].stCfg.uiIndex, ulLowerFreq, ulUpperFreq);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        /* add channel to list */
        rc = __AddChannelToWaveplanList(true, ulLowerFreq, ulUpperFreq, &stWaveplanList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. index: %d, low: %ld, upper: %ld",
                      pstMediaChannels->pastChanList[i].stCfg.uiIndex, ulLowerFreq, ulUpperFreq);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }
    }

    memcpy(pstWaveplanList, &stWaveplanList, sizeof(stWaveplanList));

END_LABEL:

    return rc;
}


int
DSC_WrGetWaveplan(WR_WAVEPLAN_LIST_ST *pstWaveplanList)
{
    MEDIA_CHANNELS_ST stMediaChannels = {0, NULL};
    int rc = SR_ERR_OK;

    if (NULL == pstWaveplanList) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    memset(&stMediaChannels, 0, sizeof(stMediaChannels));
    rc = DSC_WrGetMediaChannelsInfo(DSC_GetRunningSession(), &stMediaChannels);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get meta-channels");
        goto END_LABEL;
    }

    rc = DSC_WrParseWaveplan((const MEDIA_CHANNELS_ST *)&stMediaChannels, pstWaveplanList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get meta-channels");
        goto END_LABEL;
    }
END_LABEL:

    DEF_SAFE_FREE(stMediaChannels.pastChanList);

    return rc;
}

int
__AddChannelToWssCfgList(bool bAllowSameChannel, const WR_WSS_CFG_ST *pstWssCfg, WR_WSS_CFG_LIST_ST *pstWssCfgList)
{
    /* int rc = SR_ERR_OK; */
    uint32_t i = 0, j = 0;
    bool bSameChannelFlag = false;

    if (NULL == pstWssCfg || NULL == pstWssCfgList) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para.");
        goto ERR_LABEL;
    }

#if 0
    /* check channel frequency */
    if (false == __IsValidChannelFreq(ulLowerFreq, ulUpperFreq)) {
        goto ERR_LABEL;
    }
#endif

    /* insert channel to list from small to large */
    for (i = 0; i < pstWssCfgList->uiTotal; i++) {
        if (pstWssCfg->ulUpperFreq <= pstWssCfgList->astWssCfg[i].ulLowerFreq) {
            /* OK, find insert position */
            break;
        } else if (pstWssCfg->ulLowerFreq >= pstWssCfgList->astWssCfg[i].ulUpperFreq) {
            continue;
        } else if ((pstWssCfg->ulLowerFreq == pstWssCfgList->astWssCfg[i].ulLowerFreq) && (pstWssCfg->ulUpperFreq == pstWssCfgList->astWssCfg[i].ulUpperFreq)) {
            /* same channel */
            bSameChannelFlag = true;
            break;
        } else {
            /* invalid channel */
        }
    }

    /* the same channel has already existed */
    if (bSameChannelFlag) {
        if (bAllowSameChannel) {
            goto OK_LABEL;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "conflict channel(lower:%ld, upper:%ld)", pstWssCfg->ulLowerFreq, pstWssCfg->ulUpperFreq);
            goto ERR_LABEL;
        }
    }

    /* add new channel */
    if (pstWssCfgList->uiTotal >= WR_WAVEPLAN_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "the waveplan is more than %d", WR_WAVEPLAN_NUM_MAX);
        goto ERR_LABEL;
    }

    /* insert new channel to list */
    for (j = pstWssCfgList->uiTotal; j > i; j--) {
        memcpy(&(pstWssCfgList->astWssCfg[j]), &(pstWssCfgList->astWssCfg[j - 1]), sizeof(WR_WSS_CFG_ST));
    }
    memcpy(pstWssCfgList->astWssCfg + i, pstWssCfg, sizeof(*pstWssCfg));
    pstWssCfgList->uiTotal++;

OK_LABEL:

    return SR_ERR_OK;

ERR_LABEL:

    return SR_ERR_INVAL_ARG;
}


int
__ParseChannelPortName(const char *pcSrcPortName, const char *pcDestPortName, uint32_t *puiExpPort, uint32_t *puiComPort, uint32_t *puiWssIdx, uint32_t *puiWssDirection)
{
#if 0
    size_t nSrcLen = 0, nDestLen = 0;
    int iExpPort = 0, iComPort = 0;
    bool bParseOkFlag = false;
    WR_WSS_DIRECTION_EN enWssDirection = WR_WSS_DIRECTION_MAX;
    uint32_t uiWssIdx = WR_WSS_IDX_MAX;

    if (NULL == pcSrcPortName || NULL == pcDestPortName || NULL == puiExpPort || NULL == puiComPort) {
        return SR_ERR_INVAL_ARG;
    }

    /* Both Source/Dest PortName must be empty or valid matched port at the same time */

    nSrcLen = strlen(pcSrcPortName);
    nDestLen = strlen(pcDestPortName);
    if (0 == nSrcLen || 0 == nDestLen) {

        return SR_ERR_INVAL_ARG;
    }
    /*
       wss1:
            egress:                In01~In32 --> ADD1_COM1/ADD1_COM2
            ingress:  DROP1_COM1/DROP1_COM2  --> Out01~Out32

       wss2:
            egress:                In33~In64 --> ADD2_COM1/ADD2_COM2
            ingress:  DROP2_COM1/DROP2_COM2  --> Out33~Out64
       Note: NOT support XXX_COM2 on meiwu proc
     */


    if ((0 == strncmp(pcSrcPortName, WR_STR_PREFIX_IN, 2)) && (4 == nSrcLen)) {
        /* parse igress direction */
        enWssDirection = WR_WSS_DIRECTION_EGRESS;
        if (isdigit(pcSrcPortName[2]) && isdigit(pcSrcPortName[3])) {
            iExpPort = atoi(pcSrcPortName + 2);
#if WR_MEIWU_PROC_FEATURE
            if (0 == strcmp(pcDestPortName, WR_STR_ADD1_COM1)) {
                iComPort = WR_ADD1_COM1;
            } else if (0 == strcmp(pcDestPortName, WR_STR_ADD2_COM1)) {
                iComPort = WR_ADD2_COM1;
            } else {
                /* invalid */
                iComPort = WR_COMMON_PORT_MAX;
            }
#else
            if (0 == strcmp(pcDestPortName, WR_STR_ADD1_COM1)) {
                iComPort = WR_ADD1_COM1;
            } else if (0 == strcmp(pcDestPortName, WR_STR_ADD1_COM2)) {
                iComPort = WR_ADD1_COM2;
            } else if (0 == strcmp(pcDestPortName, WR_STR_ADD2_COM1)) {
                iComPort = WR_ADD2_COM1;
            } else if (0 == strcmp(pcDestPortName, WR_STR_ADD2_COM2)) {
                iComPort = WR_ADD2_COM2;
            } else {
                /* invalid */
                iComPort = WR_COMMON_PORT_MAX;
            }
#endif
            /*  */
            if (iExpPort >= WR_WSS1_PORT_MIN && iExpPort <= WR_WSS1_PORT_MAX) {
                uiWssIdx = WR_WSS_IDX_1;
                if (WR_ADD1_COM1 == iComPort || WR_ADD1_COM2 == iComPort) {
                    bParseOkFlag = true;
                }
            } else if (iExpPort >= WR_WSS2_PORT_MIN && iExpPort <= WR_WSS2_PORT_MAX) {
                uiWssIdx = WR_WSS_IDX_2;
                if (WR_ADD2_COM1 == iComPort || WR_ADD2_COM2 == iComPort) {
                    bParseOkFlag = true;
                }
            } else {
                /* Not matched source port and dest port */
            }

        }
    } else if (NULL != strstr(pcSrcPortName, WR_STR_PREFIX_DROP)) {
        /* parse egress direction */
        enWssDirection = WR_WSS_DIRECTION_INGRESS;
        if ((0 == strncmp(pcDestPortName, WR_STR_PREFIX_OUT, 3)) && (5 == nDestLen)) {
            if (isdigit(pcDestPortName[3]) && isdigit(pcDestPortName[4])) {
                iExpPort = atoi(pcDestPortName + 3);
            }
        }
        /* LOG_WRITE(EN_LOG_NOTICE, "iExpPort: %d", iExpPort); */
#if WR_MEIWU_PROC_FEATURE
        if (0 == strcmp(pcSrcPortName, WR_STR_DROP1_COM1)) {
            iComPort = WR_DROP1_COM1;
        } else if (0 == strcmp(pcSrcPortName, WR_STR_DROP2_COM1)) {
            iComPort = WR_DROP2_COM1;
        } else {
            /* invalid */
            iComPort = WR_COMMON_PORT_MAX;
        }
#else
        if (0 == strcmp(pcSrcPortName, WR_STR_DROP1_COM1)) {
            iComPort = WR_DROP1_COM1;
        } else if (0 == strcmp(pcSrcPortName, WR_STR_DROP1_COM2)) {
            iComPort = WR_DROP1_COM2;
        } else if (0 == strcmp(pcSrcPortName, WR_STR_DROP2_COM1)) {
            iComPort = WR_DROP2_COM1;
        } else if (0 == strcmp(pcSrcPortName, WR_STR_DROP2_COM2)) {
            iComPort = WR_DROP2_COM2;
        } else {
            /* invalid */
            iComPort = WR_COMMON_PORT_MAX;
        }
#endif
        /* LOG_WRITE(EN_LOG_NOTICE, "iComPort: %d, pcSrcPortName: %s, len:%d", iComPort, pcSrcPortName, strlen(pcDestPortName)); */
        /*  */
        if (iExpPort >= WR_WSS1_PORT_MIN && iExpPort <= WR_WSS1_PORT_MAX) {
            uiWssIdx = WR_WSS_IDX_1;
            if (WR_DROP1_COM1 == iComPort || WR_DROP1_COM2 == iComPort) {
                bParseOkFlag = true;
            }
        } else if (iExpPort >= WR_WSS2_PORT_MIN && iExpPort <= WR_WSS2_PORT_MAX) {
            uiWssIdx = WR_WSS_IDX_2;
            if (WR_DROP2_COM1 == iComPort || WR_DROP2_COM2 == iComPort) {
                bParseOkFlag = true;
            }
        } else {
            /* Not matched source port and dest port */
            /* LOG_WRITE(EN_LOG_NOTICE, "iExpPort: %d, iComPort: %d", iExpPort, iComPort); */
        }
    } else {
        /* error port-name */
        /* LOG_WRITE(EN_LOG_NOTICE, "error name, pcSrcPortName: %s", pcSrcPortName); */
    }

    /* parse ok */
    if (bParseOkFlag) {
        *puiExpPort = iExpPort;
        *puiComPort = iComPort;
        *puiWssIdx = uiWssIdx;
        *puiWssDirection = enWssDirection;

        return SR_ERR_OK;
    }

    return SR_ERR_INVAL_ARG;
#endif
    return SR_ERR_INVAL_ARG;
}

int
DSC_WrCheckChannelCfg(const CHANNEL_ST *pstChanList, WR_WSS_CFG_ST *pstWssCfg)
{
    uint32_t i = 0, uiIndex = 0;
    WR_WSS_CFG_ST stWssCfg;
    int rc = SR_ERR_OK;

    if (NULL == pstChanList || NULL == pstWssCfg) {
        return SR_ERR_INVAL_ARG;
    }

    memset(&stWssCfg, 0, sizeof(stWssCfg));
    stWssCfg.ulLowerFreq = pstChanList->stCfg.ulLowerFreq;
    stWssCfg.ulUpperFreq = pstChanList->stCfg.ulUpperFreq;

    /* check index */
    uiIndex = pstChanList->stCfg.uiIndex;
    if (uiIndex < WR_INDEX_MIN || uiIndex > WR_INDEX_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid index: %d. low: %ld, upper: %ld", uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    stWssCfg.uiIndex = uiIndex;

    /* check name */
    if ((0 != strlen(pstChanList->stCfg.acName)) &&
        (0 != UTIL_CheckStrWithAllSpecChars((char *)(pstChanList->stCfg.acName)))) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid channel-name. index: %d, %lu, %lu", uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /* check channel frequency */
    if (false == __IsValidChannelFreq(stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid frequency. index: %d, low: %ld, upper: %ld", pstChanList->stCfg.uiIndex,
                  stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /* check source port and dest port */
    rc = __ParseChannelPortName(pstChanList->stSrcConnPort.acPortName,
                                pstChanList->stDestConnPort.acPortName,
                                &stWssCfg.uiExpPort, &stWssCfg.uiComPort,
                                &stWssCfg.uiWssIdx, &stWssCfg.uiWssDirection);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port-name. src-port: %s, dest-port: %s, index: %d",
                  pstChanList->stSrcConnPort.acPortName, pstChanList->stDestConnPort.acPortName, uiIndex);
        goto END_LABEL;
    }



    /* copy admin-status and attn-ctrl-mode */
    stWssCfg.enAdminStatus = pstChanList->stCfg.enAdminStatus;
    stWssCfg.enAttnCtrlMode = pstChanList->stCfg.enAttnCtrlMode;


    if ((WR_WSS_DIRECTION_INGRESS == stWssCfg.uiWssDirection) && (WR_ATTENUATION_DYNAMIC_LOSS == stWssCfg.enAttnCtrlMode)) {
        LOG_WRITE(EN_LOG_ERROR, "Not support apc mode in drop direction. index: %d", uiIndex);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /* check the count of distribution */
    if (pstChanList->stSpecPwrProfile.uiTotal != WR_DISTRIBUTION_NUM_UPPER_LIMIT) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid distribution number. only support one. index: %d, distribution-total: %d",
                  uiIndex, pstChanList->stSpecPwrProfile.uiTotal);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    /*  parse target power and attn*/
    /* !!! only support one distribution */
    if (0 == pstChanList->stSpecPwrProfile.uiTotal) {
        stWssCfg.dTargetPwr = WR_INVALID_POWER;      /* default value */
        stWssCfg.dAttn = WR_CHANNEL_ATTN_MAX;        /* default value */
    } else if (WR_DISTRIBUTION_NUM_UPPER_LIMIT == pstChanList->stSpecPwrProfile.uiTotal) {

        stWssCfg.dTargetPwr = pstChanList->stSpecPwrProfile.astDist[0].stSpecPwrCfg.dTargetPwr;
        stWssCfg.dAttn = pstChanList->stSpecPwrProfile.astDist[0].stSpecPwrCfg.dAttn;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid distribution number. index: %d, distribution-total: %d",
                  uiIndex, pstChanList->stSpecPwrProfile.uiTotal);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;

    }

    /* check target-power and attn, TBD */
    for (i = 0; i < pstChanList->stSpecPwrProfile.uiTotal; i++) {

        /* TODO, spectrum frequency == channel frequency */
        if (stWssCfg.ulLowerFreq != pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulLowerFreq ||
            stWssCfg.ulUpperFreq != pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulUpperFreq) {
            LOG_WRITE(EN_LOG_ERROR, "Not matched frequency. index: %d, lower: %ld, upper: %ld, lower: %ld, upper: %ld",
                      uiIndex,
                      stWssCfg.ulLowerFreq,
                      stWssCfg.ulUpperFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulLowerFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulUpperFreq);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        /* attn */
        if ((pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dAttn < 0) ||
            (pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dAttn > WR_CHANNEL_ATTN_MAX)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid attenuation. index: %d, lower: %ld, upper: %ld, dAttn: %.2f",
                      uiIndex,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulLowerFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulUpperFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dAttn);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        /* target-power */
        if (UTIL_IsDoubleEqual(pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dTargetPwr, WR_INVALID_POWER)) {
            /* default value */
            continue;
        }
        if ((pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dTargetPwr < WR_TARGET_POWER_MIN) ||
            (pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dTargetPwr > WR_TARGET_POWER_MAX)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid target-power. index: %d, lower: %ld, upper: %ld, dTargetPwr: %.2f",
                      uiIndex,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulLowerFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.ulUpperFreq,
                      pstChanList->stSpecPwrProfile.astDist[i].stSpecPwrCfg.dTargetPwr);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }
    }


    memcpy(pstWssCfg, &stWssCfg, sizeof(stWssCfg));

END_LABEL:

    return rc;
}

/* parse one channel */
static int
__ParseMediaChannelData(const CHANNEL_ST *pstChannel, WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WAVEPLAN_LIST_ST *pastWssWaveplanList, WR_WSS_CFG_LIST_ST *pstEgressCfgList, WR_WSS_CFG_LIST_ST *pastIngressWssCfgList)
{
    uint32_t uiIndex = 0;
    WR_WSS_CFG_ST stWssCfg;
    int rc = SR_ERR_OK;

    uiIndex = pstChannel->stCfg.uiIndex;

    /* check channel config data */
    memset(&stWssCfg, 0, sizeof(stWssCfg));
    rc = DSC_WrCheckChannelCfg(pstChannel, &stWssCfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid channel. index: %d, lower: %ld, upper: %ld, admin-status: %d, attn-ctrl-mode: %d, src-port: %s, dest-post: %s",
                  uiIndex,
                  pstChannel->stCfg.ulLowerFreq,
                  pstChannel->stCfg.ulUpperFreq,
                  pstChannel->stCfg.enAdminStatus,
                  pstChannel->stCfg.enAttnCtrlMode,
                  pstChannel->stSrcConnPort.acPortName,
                  pstChannel->stDestConnPort.acPortName);
        goto END_LABEL;
    }


    /* check wavelplan by WR_WAVEPLAN_LIST_ST */
    rc = __AddChannelToWaveplanList(true, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq, pstWaveplanList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. index: %d, low: %ld, upper: %ld",
                  uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
        goto END_LABEL;
    }

    /* check channel conflict and use wss1 and wss2 as one wss, not allow same channel on single direction. */
    if (WR_WSS_DIRECTION_INGRESS == stWssCfg.uiWssDirection) {
        /* check channel conflict on__AddChannelToWaveplanList ingress direction */
        rc = __AddChannelToWaveplanList(true, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq, &pastWssWaveplanList[WR_WSS_DIRECTION_INGRESS]);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. direction: %d, index: %d, low: %ld, upper: %ld", stWssCfg.uiWssDirection,
                      uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }
    } else if (WR_WSS_DIRECTION_EGRESS == stWssCfg.uiWssDirection) {
        /* check channel conflict on egress direction */
        rc = __AddChannelToWaveplanList(false, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq, &pastWssWaveplanList[WR_WSS_DIRECTION_EGRESS]);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. direction: %d, index: %d, low: %ld, upper: %ld", stWssCfg.uiWssDirection,
                      uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }
    } else {
#if 0
        /* check channel conflict on no direction (both source and dest portname are empty) */
        rc = __AddChannelToWaveplanList(false, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq, &astWssWaveplanList[WR_WSS_DIRECTION_MAX]);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. direction: %d, index: %d, low: %ld, upper: %ld", stWssCfg.uiWssDirection,
                      uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }
#else
        LOG_WRITE(EN_LOG_ERROR, "Invalid port mapping. direction: %d, index: %d, low: %ld, upper: %ld",
                  stWssCfg.uiWssDirection, uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
        goto END_LABEL;
#endif
    }

    /* check channels conflict  */
    if (WR_WSS_DIRECTION_INGRESS == stWssCfg.uiWssDirection) {
        /* check all channels in ingress direction:
                (1) channel frequency is different in WSS1 (or WSS2)
                (2) one channel transfer in unique port Out01~32 (src-port and dest-port),
                    and allow multiple channels to transfer in same port Out01~32
                (3) one channel transfer in unique port Out33~64 (src-port and dest-port),
                    and allow multiple channels to transfer in same port Out33~64
                (4) wss1 and wss2 may have same channel, because wss1 and wss2 backup eche other
         */
        if (stWssCfg.uiWssIdx != WR_WSS_IDX_1 && stWssCfg.uiWssIdx != WR_WSS_IDX_2) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid wssid. direction: %d, uiWssIdx: %d, index: %d, low: %ld, upper: %ld",
                      stWssCfg.uiWssDirection, stWssCfg.uiWssIdx, uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }

        /* wss1 or wss2 */
        rc = __AddChannelToWssCfgList(false, (const WR_WSS_CFG_ST *)&stWssCfg, pastIngressWssCfgList + stWssCfg.uiWssIdx);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid wss%d config of ingress direction. index: %d, low: %ld, upper: %ld",
                      stWssCfg.uiWssIdx + 1, uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }

    } else if (WR_WSS_DIRECTION_EGRESS == stWssCfg.uiWssDirection) {
        /* check all channels in egress direction, and make two wss as one wss:
                        (1) channel frequency is different
                        (2) one channel transfer in unique port InXX (src-port and dest-port),
                            and allow multiple channels to transfer in sampe port InXX
         */

        rc = __AddChannelToWssCfgList(false, (const WR_WSS_CFG_ST *)&stWssCfg, pstEgressCfgList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid wss config of egress direction. index: %d, low: %ld, upper: %ld",
                      uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }
    }

END_LABEL:

    return rc;
}

int
DSC_WrCheckMediaChannels(const MEDIA_CHANNELS_ST *pstMediaChannels)
{
    int rc = SR_ERR_OK;
    uint32_t i = 0;
    /* uint32_t uiIndex = 0; */
    /* WR_WSS_CFG_ST stWssCfg; */
    WR_WAVEPLAN_LIST_ST stWaveplanList;      /* ocm/wss waveplan */
    WR_WAVEPLAN_LIST_ST astWssWaveplanList[WR_WSS_DIRECTION_MAX];     /* egress direction waveplan and ingress ingress waveplan*/
    WR_WSS_CFG_LIST_ST stEgressWssCfgList;
    WR_WSS_CFG_LIST_ST astIngressWssCfgList[WR_WSS_IDX_MAX];

    memset(&stWaveplanList, 0, sizeof(stWaveplanList));
    memset(&astWssWaveplanList, 0, sizeof(astWssWaveplanList));
    memset(&stEgressWssCfgList, 0, sizeof(stEgressWssCfgList));
    memset(&astIngressWssCfgList, 0, sizeof(astIngressWssCfgList));

    for (i = 0; i < pstMediaChannels->uiTotal; i++) {

        /* uiIndex = pstMediaChannels->pastChanList[i].stCfg.uiIndex; */
        rc = __ParseMediaChannelData((const CHANNEL_ST *)(pstMediaChannels->pastChanList + i),
                                     &stWaveplanList, astWssWaveplanList, &stEgressWssCfgList, astIngressWssCfgList);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }

    }

    /* LOG_WRITE(EN_LOG_NOTICE, "channel total: %d", stWaveplanList.uiTotal); */

END_LABEL:

    return rc;
}

/* add blocked channel to ingress (or egress) channels */
int
__AddBlockChannelToWssCfgList(uint32_t uiWssDirection, const WR_WSS_CFG_ST *pstWssCfg, WR_WSS_CFG_LIST_ST *pstWssCfgList)
{
    /* int rc = SR_ERR_OK; */
    uint32_t i = 0, j = 0;
    bool bSameChannelFlag = false;

    if (NULL == pstWssCfg || NULL == pstWssCfgList || uiWssDirection >= WR_WSS_DIRECTION_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para.");
        goto ERR_LABEL;
    }


#if 0
    /* check channel frequency */
    if (false == __IsValidChannelFreq(ulLowerFreq, ulUpperFreq)) {
        goto ERR_LABEL;
    }
#endif

    /* insert channel to list from small to large */
    for (i = 0; i < pstWssCfgList->uiTotal; i++) {
        if (pstWssCfg->ulUpperFreq <= pstWssCfgList->astWssCfg[i].ulLowerFreq) {
            /* OK, find insert position */
            break;
        } else if (pstWssCfg->ulLowerFreq >= pstWssCfgList->astWssCfg[i].ulUpperFreq) {
            continue;
        } else if ((pstWssCfg->ulLowerFreq == pstWssCfgList->astWssCfg[i].ulLowerFreq) && (pstWssCfg->ulUpperFreq == pstWssCfgList->astWssCfg[i].ulUpperFreq)) {
            /* same channel */
            bSameChannelFlag = true;
            break;
        } else {
            /* invalid channel */
            LOG_WRITE(EN_LOG_ERROR, "invalid channel(lower:%ld, upper:%ld)", pstWssCfg->ulLowerFreq, pstWssCfg->ulUpperFreq);
            goto ERR_LABEL;
        }
    }

    /* the same channel has already existed */
    if (bSameChannelFlag) {
        goto OK_LABEL;
    }

    /* add new channel */
    if (pstWssCfgList->uiTotal >= WR_WAVEPLAN_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "the waveplan is more than %d", WR_WAVEPLAN_NUM_MAX);
        goto ERR_LABEL;
    }

    /* insert new channel to list */
    for (j = pstWssCfgList->uiTotal; j > i; j--) {
        memcpy(&(pstWssCfgList->astWssCfg[j]), &(pstWssCfgList->astWssCfg[j - 1]), sizeof(WR_WSS_CFG_ST));
    }
    memcpy(pstWssCfgList->astWssCfg + i, pstWssCfg, sizeof(*pstWssCfg));
    pstWssCfgList->uiTotal++;

OK_LABEL:

    return SR_ERR_OK;

ERR_LABEL:

    return SR_ERR_INVAL_ARG;
}
#if 0
int
DSC_WrGetChannelsByDirection(WR_WSS_DIRECTION_EN enWssDirection, const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WSS_CFG_LIST_ST *pstWssCfgList)
{
    uint32_t i = 0;
    WR_WSS_CFG_LIST_ST stWssCfgList;
    WR_WSS_CFG_ST stWssCfg;
    int rc = SR_ERR_OK;
    WR_WAVEPLAN_LIST_ST stWaveplanList;      /*  */
    /* CHANNEL_ST astChanList[WR_WAVEPLAN_NUM_MAX]; */
    memset(&stWaveplanList, 0, sizeof(stWaveplanList));
    memset(&stWssCfgList, 0, sizeof(stWssCfgList));
    memset(&stWssCfg, 0, sizeof(stWssCfg));

    if (NULL == pstMediaChannels || NULL == pstWssCfgList || enWssDirection >= WR_WSS_DIRECTION_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para.");
        return SR_ERR_INVAL_ARG;
    }

    for (i = 0; i < pstMediaChannels->uiTotal; i++) {

        memset(&stWssCfg, 0, sizeof(stWssCfg));

        stWssCfg.ulLowerFreq = pstMediaChannels->pastChanList[i].stCfg.ulLowerFreq;
        stWssCfg.ulUpperFreq = pstMediaChannels->pastChanList[i].stCfg.ulUpperFreq;

        /* check channel frequency */
        if (false == __IsValidChannelFreq(stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid frequency. index: %d, low: %ld, upper: %ld", pstMediaChannels->pastChanList[i].stCfg.uiIndex,
                      stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        /* check source port and dest port */
        rc = __ParseChannelPortName(pstMediaChannels->pastChanList[i].stSrcConnPort.acPortName,
                                    pstMediaChannels->pastChanList[i].stDestConnPort.acPortName,
                                    &stWssCfg.uiExpPort, &stWssCfg.uiComPort,
                                    &stWssCfg.uiWssIdx, &stWssCfg.uiWssDirection);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid port-name. src-port: %s, dest-port: %s, index: %d, low: %ld, upper: %ld",
                      pstMediaChannels->pastChanList[i].stSrcConnPort.acPortName, pstMediaChannels->pastChanList[i].stDestConnPort.acPortName,
                      pstMediaChannels->pastChanList[i].stCfg.uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }

        stWssCfg.enAdminStatus = pstMediaChannels->pastChanList[i].stCfg.enAdminStatus;
        stWssCfg.enAttnCtrlMode = pstMediaChannels->pastChanList[i].stCfg.enAttnCtrlMode;

        if (0 == pstMediaChannels->pastChanList[i].stSpecPwrProfile.uiTotal) {
            stWssCfg.dTargetPwr = -60.00;             /* TBD, default value */
            stWssCfg.dAttn = 25.5;                    /* TBD, default value */
        } else {
            stWssCfg.dTargetPwr = pstMediaChannels->pastChanList[i].stSpecPwrProfile.astDist[0].stSpecPwrCfg.dTargetPwr;
            stWssCfg.dAttn = pstMediaChannels->pastChanList[i].stSpecPwrProfile.astDist[0].stSpecPwrCfg.dAttn;
        }
        /* check target power and attn, TBD */

        /* check wavelplan by WR_WAVEPLAN_LIST_ST */
        rc = __AddChannelToWaveplanList(true, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq, &stWaveplanList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan. index: %d, low: %ld, upper: %ld",
                      pstMediaChannels->pastChanList[i].stCfg.uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
            goto END_LABEL;
        }

        /* ingress direction, channel : InXX is 1 : 1 */
        /* egress direction, channel : OutXX is 1 : 1 */
        if (enWssDirection == stWssCfg.uiWssDirection) {
            rc = __AddChannelToWssCfgList(enWssDirection, (const WR_WSS_CFG_ST *)&stWssCfg, &stWssCfgList);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Invalid wss config. direction: %d, index: %d, low: %ld, upper: %ld", stWssCfg.uiWssDirection,
                          pstMediaChannels->pastChanList[i].stCfg.uiIndex, stWssCfg.ulLowerFreq, stWssCfg.ulUpperFreq);
                goto END_LABEL;
            }
        } else {
            /* ignore */
        }

    }

    memcpy(pstWssCfgList, &stWssCfgList, sizeof(stWssCfgList));

    LOG_WRITE(EN_LOG_NOTICE, "channel total: %d", stWaveplanList.uiTotal);

END_LABEL:

    return rc;
}
#endif

int
DSC_WrGetSingleMediaChannel(sr_session_ctx_t *pstSession, uint32_t uiIndex, CHANNEL_ST *pstChannel)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t i = 0, j = 0, nCnt = 0 /*, nActualNum = -1, nChannelTotal = 0*/;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[512] = {0};
    DISTRIBUTION_ST *pstDist = NULL;
    char acKeyValue[DEF_BUF_SIZE_32] = {0};
    uint64_t ulLowerFreq = 0, ulUpperFreq = 0;

    pstSess = (NULL == pstSession) ? DSC_GetOperSession() : pstSession;

    if (NULL == pstChannel || NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL "//*", uiIndex);
    rc = get_items(pstSess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    if (NULL == pstVal) {
        LOG_WRITE(EN_LOG_ERROR, "Not find channel index: %d", uiIndex);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }


    rc = DSC_WrInitChannelDefaultVal(pstChannel, 0);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_WrInitChannelDefaultVal");
        goto END_LABEL;
    }

    for (i = 0; i < nCnt; i++) {
        /* sr_print_val(pstVal + i); */
        /* parse channel list */
        if (NULL != strstr(pstVal[i].xpath, WR_NODE_SPECTRUM_POWER_PROFILE)) {
            if (sr_xpath_node_name_eq(pstVal[i].xpath, "spectrum-power-profile")) {
                /* ignore */
                continue;
            }

            memset(acKeyValue, 0, sizeof(acKeyValue));
            rc = DSC_GetKeyValue(pstVal[i].xpath, WR_LIST_DISTRIBUTION, WR_KEY_LOWER_FREQUENCY, sizeof(acKeyValue), acKeyValue);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Not find key: lower-frequency");
                goto END_LABEL;
            }
            ulLowerFreq = atol(acKeyValue);

            memset(acKeyValue, 0, sizeof(acKeyValue));
            rc = DSC_GetKeyValue(pstVal[i].xpath, WR_LIST_DISTRIBUTION, WR_KEY_UPPER_FREQUENCY, sizeof(acKeyValue), acKeyValue);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Not find key: upper-frequency");
                goto END_LABEL;
            }

            ulUpperFreq = atol(acKeyValue);

            for (j = 0; j < pstChannel->stSpecPwrProfile.uiTotal; j++) {
                if (ulLowerFreq == pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.ulLowerFreq &&
                    ulUpperFreq == pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.ulUpperFreq) {
                    break;
                }
            }
            if (j < pstChannel->stSpecPwrProfile.uiTotal) {
                pstDist = &(pstChannel->stSpecPwrProfile.astDist[j]);
            } else {
                pstDist = &(pstChannel->stSpecPwrProfile.astDist[pstChannel->stSpecPwrProfile.uiTotal]);
                pstChannel->stSpecPwrProfile.uiTotal++;
                /* init default value */
                pstDist->stSpecPwrCfg.ulLowerFreq = ulLowerFreq;
                pstDist->stSpecPwrCfg.ulUpperFreq = ulUpperFreq;
                pstDist->stSpecPwrCfg.dTargetPwr = WR_INVALID_POWER;
                pstDist->stSpecPwrCfg.dAttn = WR_CHANNEL_ATTN_MAX;
            }

            /* spectrum-power-profile container */
            if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_LOW_FREQUENCY)) {
                pstDist->stSpecPwrCfg.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_UPPER_FREQUENCY)) {
                pstDist->stSpecPwrCfg.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_TARGET_POWER)) {
                pstDist->stSpecPwrCfg.dTargetPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ATTN)) {
                pstDist->stSpecPwrCfg.dAttn = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_LOW_FREQUENCY)) {
                pstDist->stSpecPwrState.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_UPPER_FREQUENCY)) {
                pstDist->stSpecPwrState.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_TARGET_POWER)) {
                pstDist->stSpecPwrState.dTargetPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ATTN)) {
                pstDist->stSpecPwrState.dAttn = pstVal[i].data.decimal64_val;
            } else {
                /* ignore */
            }

        } else {
            /* config/state/source/dest container */
            if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_INDEX)) {
                pstChannel->stCfg.uiIndex = pstVal[i].data.uint32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_CHANNEL_NAME)) {
                snprintf(pstChannel->stCfg.acName, sizeof(pstChannel->stCfg.acName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_LOW_FREQUENCY)) {
                pstChannel->stCfg.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_UPPER_FREQUENCY)) {
                pstChannel->stCfg.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ADMIN_STATUS)) {
                pstChannel->stCfg.enAdminStatus = DSC_WrAdminStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_SUPPER_CHANNEL)) {
                pstChannel->stCfg.bSupperChannel = pstVal[i].data.bool_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_SUPPER_CHANNEL_PARENT)) {
                pstChannel->stCfg.uiSupperChanIdx = pstVal[i].data.int32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ASE_CONTROL_MODE)) {
                pstChannel->stCfg.enAseCtrlMode = DSC_WrAseCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ASE_INJECTION_THRESHOLD)) {
                pstChannel->stCfg.dAseInjectThr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_CONFIG WR_NODE_ATTN_CONTROL_MODE)) {
                pstChannel->stCfg.enAttnCtrlMode = DSC_WrAttnCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_SOURCE WR_NODE_CONFIG WR_NODE_PORT_NAME)) {
                snprintf(pstChannel->stSrcConnPort.acPortName, sizeof(pstChannel->stSrcConnPort.acPortName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_DEST WR_NODE_CONFIG WR_NODE_PORT_NAME)) {
                snprintf(pstChannel->stDestConnPort.acPortName, sizeof(pstChannel->stDestConnPort.acPortName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_INDEX)) {
                pstChannel->stState.uiIndex = pstVal[i].data.uint32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_CHANNEL_NAME)) {
                snprintf(pstChannel->stState.acName, sizeof(pstChannel->stState.acName), "%s", pstVal[i].data.string_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_LOW_FREQUENCY)) {
                pstChannel->stCfg.ulLowerFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_UPPER_FREQUENCY)) {
                pstChannel->stState.ulUpperFreq = pstVal[i].data.int64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ADMIN_STATUS)) {
                pstChannel->stState.enAdminStatus = DSC_WrAdminStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_SUPPER_CHANNEL)) {
                pstChannel->stState.bSupperChannel = pstVal[i].data.bool_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_SUPPER_CHANNEL_PARENT)) {
                pstChannel->stState.uiSupperChanIdx = pstVal[i].data.int32_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ASE_CONTROL_MODE)) {
                pstChannel->stState.enAseCtrlMode = DSC_WrAseCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ASE_INJECTION_THRESHOLD)) {
                pstChannel->stState.dAseInjectThr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ATTN_CONTROL_MODE)) {
                pstChannel->stState.enAttnCtrlMode = DSC_WrAttnCtrlModeToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_OPER_STATUS)) {
                pstChannel->stState.enOperStatus = DSC_WrOperStatusToEnum(pstVal[i].data.enum_val);
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_OUTPUT_POWER)) {
                pstChannel->stState.dOutputPwr = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_STATE WR_NODE_ACTUAL_ATTN)) {
                pstChannel->stState.dActualAttn = pstVal[i].data.decimal64_val;
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_SOURCE WR_NODE_STATE WR_NODE_PORT_NAME)) {
                /* ingnore */
            } else if (NULL != strstr(pstVal[i].xpath, WR_NODE_DEST WR_NODE_STATE WR_NODE_PORT_NAME)) {
                /* ingnore */
            } else {
                /* ignore */
            }
        }

    }

#if 0
    printf("[%3ld] %3d %10s %d %10ld %10ld %d %10s %20s\r\n", i + 1, pstChannel->stCfg.uiIndex, pstChannel->stCfg.acName,
           pstChannel->stCfg.bSupperChannel, pstChannel->stCfg.ulLowerFreq, pstChannel->stCfg.ulUpperFreq,
           pstChannel->stCfg.enAttnCtrlMode, pstChannel->stSrcConnPort.acPortName, pstChannel->stDestConnPort.acPortName
           );
    printf("	distribution number: %d\r\n", pstChannel->stSpecPwrProfile.uiTotal);
    for (j = 0; j < pstChannel->stSpecPwrProfile.uiTotal; j++) {
        printf("	[%ld]	[lowerFreq, upperFreq, targetPwr, attn]: [%ld, %ld, %.2f, %.2f]\r\n", j + 1,
               pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.ulLowerFreq,
               pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.ulUpperFreq,
               pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.dTargetPwr,
               pstChannel->stSpecPwrProfile.astDist[j].stSpecPwrCfg.dAttn);
    }
#endif
END_LABEL:

    sr_free_values(pstVal, nCnt);


    return rc;
}



int
DSC_WrParseMetaChannels(const MEDIA_CHANNELS_ST *pstMediaChannels, WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WSS_CFG_LIST_ST *pstEgressCfgList, WR_WSS_CFG_LIST_ST *pstIngressWss1CfgList, WR_WSS_CFG_LIST_ST *pstIngressWss2CfgList)
{
    uint32_t i = 0;
    int rc = SR_ERR_OK;

    WR_WAVEPLAN_LIST_ST stWaveplanList;          /* ocm/wss waveplan */
    WR_WAVEPLAN_LIST_ST astWssWaveplanList[WR_WSS_DIRECTION_MAX];         /* egress direction waveplan and ingress ingress waveplan*/
    WR_WSS_CFG_LIST_ST stEgressWssCfgList;
    WR_WSS_CFG_LIST_ST astIngressWssCfgList[WR_WSS_IDX_MAX];

    memset(&stWaveplanList, 0, sizeof(stWaveplanList));
    memset(&astWssWaveplanList, 0, sizeof(astWssWaveplanList));
    memset(&stEgressWssCfgList, 0, sizeof(stEgressWssCfgList));
    memset(&astIngressWssCfgList, 0, sizeof(astIngressWssCfgList));

    if (NULL == pstMediaChannels || NULL == pstWaveplanList || NULL == pstEgressCfgList ||
        NULL == pstIngressWss1CfgList || NULL == pstIngressWss2CfgList) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid para.");
        return SR_ERR_INVAL_ARG;
    }

    memset(&stWaveplanList, 0, sizeof(stWaveplanList));
    memset(&astWssWaveplanList, 0, sizeof(astWssWaveplanList));
    memset(&stEgressWssCfgList, 0, sizeof(stEgressWssCfgList));
    memset(&astIngressWssCfgList, 0, sizeof(astIngressWssCfgList));

    for (i = 0; i < pstMediaChannels->uiTotal; i++) {

        /* uiIndex = pstMediaChannels->pastChanList[i].stCfg.uiIndex; */
        rc = __ParseMediaChannelData((const CHANNEL_ST *)(pstMediaChannels->pastChanList + i),
                                     &stWaveplanList, astWssWaveplanList, &stEgressWssCfgList, astIngressWssCfgList);
        if (rc != SR_ERR_OK) {
            goto END_LABEL;
        }

    }

    memcpy(pstWaveplanList, &stWaveplanList, sizeof(stWaveplanList));
    memcpy(pstEgressCfgList, &stEgressWssCfgList, sizeof(WR_WSS_CFG_LIST_ST));
    memcpy(pstIngressWss1CfgList, &astIngressWssCfgList[WR_WSS_IDX_1], sizeof(WR_WSS_CFG_LIST_ST));

#ifdef MEIWU_PRO
    memcpy(pstIngressWss2CfgList, &astIngressWssCfgList[WR_WSS_IDX_2], sizeof(WR_WSS_CFG_LIST_ST));
#endif

#if 0
    LOG_WRITE(EN_LOG_NOTICE, "channel total: %d, egress total: %d, ingress wss1 total: %d, ingress wss2 total: %d",
              stWaveplanList.uiTotal,
              pstEgressCfgList->uiTotal,
              pstIngressWss1CfgList->uiTotal,
              pstIngressWss2CfgList->uiTotal);
#endif

END_LABEL:

    return rc;
}


int
DSC_WrAddChannelIndex(uint32_t uiIndex, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_INDEX, uiIndex);

    rc = set_uint32_item_strict(pstSess, acXpath, uiIndex);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetChannelName(uint32_t uiIndex, char *pcChannelName, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess || NULL == pcChannelName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_CHANNEL_NAME, uiIndex);

    rc = set_string_item(pstSess, acXpath, pcChannelName, strlen(pcChannelName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}


int
DSC_WrSetChannelFreqency(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_LOW_FREQUENCY, uiIndex);

    rc = set_uint64_item(pstSess, acXpath, ulLowerFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_UPPER_FREQUENCY, uiIndex);

    rc = set_uint64_item(pstSess, acXpath, ulUpperFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetChannelAdminStatus(uint32_t uiIndex, WR_ADMIN_STATUS_EN enAdminStatus, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];
    char acBuf[DEF_BUF_SIZE_32] = {0};

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_ADMIN_STATUS, uiIndex);

    rc = set_enum_item(pstSess, acXpath, DSC_WrAdminStatusToStr(enAdminStatus, acBuf, sizeof(acBuf)));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetChannelAttnCtrlMode(uint32_t uiIndex, WR_ATTN_CONTROL_MODE_EN enAttnCtrlMode, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];
    char acBuf[DEF_BUF_SIZE_64] = {0};

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_CONFIG WR_NODE_ATTN_CONTROL_MODE, uiIndex);

    rc = set_identityref_item(pstSess, acXpath, DSC_WrAttnCtrlModeToStr(enAttnCtrlMode, acBuf, sizeof(acBuf)), sizeof(acBuf));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s, acBuf: %s", sr_strerror(rc), acBuf);
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetPortConnection(uint32_t uiIndex, char *pcSrcPortName, char *pcDestPortName, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess || NULL == pcSrcPortName || NULL == pcDestPortName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_SOURCE WR_NODE_CONFIG WR_NODE_PORT_NAME, uiIndex);

    rc = set_string_item(pstSess, acXpath, pcSrcPortName, strlen(pcSrcPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL_DEST WR_NODE_CONFIG WR_NODE_PORT_NAME, uiIndex);

    rc = set_string_item(pstSess, acXpath, pcDestPortName, strlen(pcDestPortName));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetDistributionFrequency(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }
#if 0
    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_DISTRIBUTION WR_NODE_LOW_FREQUENCY, uiIndex, ulLowerFreq, ulUpperFreq);
    rc = set_uint64_item(pstSess, acXpath, ulLowerFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_DISTRIBUTION WR_NODE_UPPER_FREQUENCY, uiIndex, ulLowerFreq, ulUpperFreq);
    rc = set_uint64_item(pstSess, acXpath, ulUpperFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }
#endif
    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_CONFIG WR_NODE_LOW_FREQUENCY, uiIndex, ulLowerFreq, ulUpperFreq);
    rc = set_uint64_item(pstSess, acXpath, ulLowerFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_CONFIG WR_NODE_UPPER_FREQUENCY, uiIndex, ulLowerFreq, ulUpperFreq);
    rc = set_uint64_item(pstSess, acXpath, ulUpperFreq);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetDistributionTargetPwr(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, double dTargetPwr, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_CONFIG WR_NODE_TARGET_POWER, uiIndex, ulLowerFreq, ulUpperFreq);

    rc = set_decimal64_item(pstSess, acXpath, dTargetPwr);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrSetDistributionAttn(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, double dAttn, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_CONFIG WR_NODE_ATTN, uiIndex, ulLowerFreq, ulUpperFreq);

    rc = set_decimal64_item(pstSess, acXpath, dAttn);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrAddDistribution(uint32_t uiIndex, SPECTRUM_POWER_ST *pstSpectrumPwrCfg, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess || NULL == pstSpectrumPwrCfg) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_WrSetDistributionFrequency(uiIndex, pstSpectrumPwrCfg->ulLowerFreq, pstSpectrumPwrCfg->ulUpperFreq, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetDistributionTargetPwr(uiIndex, pstSpectrumPwrCfg->ulLowerFreq, pstSpectrumPwrCfg->ulUpperFreq, pstSpectrumPwrCfg->dTargetPwr, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetDistributionAttn(uiIndex, pstSpectrumPwrCfg->ulLowerFreq, pstSpectrumPwrCfg->ulUpperFreq, pstSpectrumPwrCfg->dAttn, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrDeleteDistribution(uint32_t uiIndex, uint64_t ulLowerFreq, uint64_t ulUpperFreq, bool bCommit)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[DEF_BUF_SIZE_512];

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_SPECTRUM_DISTRIBUTION, uiIndex, ulLowerFreq, ulUpperFreq);

    rc = sr_delete_item(pstSess, acXpath, SR_EDIT_STRICT);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (!bCommit) {
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

int
DSC_WrDeleteMediaChannel(uint32_t uiIndex)
{
    int rc = SR_ERR_OK;
    char acXpath[DEF_BUF_SIZE_512];

    sr_session_ctx_t *sess = DSC_GetRunningSession();

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL, uiIndex);
    rc = sr_delete_item(sess, acXpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item %s: %s", acXpath, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

int
DSC_WrDeleteAllMediaChannels()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* NOTE: media-channels node will be deleted */
    rc = sr_delete_item(sess, WR_XPATH_MEDIA_CHANNELS, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item %s: %s", WR_XPATH_MEDIA_CHANNELS, sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

/*
   NOTE: media-channels node NOT will be deleted

   delete /openconfig-wavelength-router:wavelength-router/media-channels/channel[index='*'],
   but reserve /openconfig-wavelength-router:wavelength-router/media-channels
 */

int
DSC_WrDeleteChannelList()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    sr_val_t *pstVal = NULL;
    size_t i = 0, nCnt = 0;
    char acXpath[DEF_BUF_SIZE_512] = {0};
    uint32_t uiIndex = 0;

    snprintf(acXpath, sizeof(acXpath), "%s", WR_XPATH_MEDIA_CHANNELS "/channel/index");
    rc = get_items(pstSess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_NOTICE, "Not Find: %s", acXpath);
            return SR_ERR_OK;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
            goto END_LABEL;
        }
    }

    for (i = 0; i < nCnt; i++) {
        /* sr_print_val(pstVal + i); */
        uiIndex = pstVal[i].data.uint32_val;
        snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL, uiIndex);
        rc = sr_delete_item(pstSess, acXpath, SR_EDIT_STRICT);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_delete_item %s: %s", acXpath, sr_strerror(rc));
            goto END_LABEL;
        }
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    sr_free_values(pstVal, nCnt);

    return rc;
}

bool
DSC_IsPortConnectionWithDirection(WR_WSS_DIRECTION_EN enWssDirection, WR_WSS_IDX_EN enWssIdx, char *pcSrcPortName, char *pcDestPortName)
{

    uint32_t uiExpPort = 0, uiComPort = 0;
    uint32_t uiWssIdx = WR_WSS_IDX_MAX;
    uint32_t uiWssDirection = WR_WSS_DIRECTION_MAX;
    int rc = SR_ERR_OK;

    rc = __ParseChannelPortName((const char *)pcSrcPortName, (const char *)pcDestPortName, &uiExpPort, &uiComPort, &uiWssIdx, &uiWssDirection);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid portname. src-port: %s, detst-port: %s", pcSrcPortName, pcDestPortName);
        goto END_LABEL;
    }

    if ((WR_WSS_DIRECTION_INGRESS == enWssDirection) && (WR_WSS_IDX_1 == enWssIdx || WR_WSS_IDX_2 == enWssIdx)) {
        if ((uiWssDirection == enWssDirection) && (uiWssIdx == enWssIdx)) {
            return true;
        }
    } else {
        if (uiWssDirection == enWssDirection) {
            return true;
        }
    }

END_LABEL:

    return false;
}

int
DSC_WrAddMediaChannel(CHANNEL_ST *pstChannel)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    uint32_t uiIndex = 0;

    pstSess = DSC_GetRunningSession();

    if (NULL == pstSess || NULL == pstChannel) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    uiIndex = pstChannel->stCfg.uiIndex;

    rc = DSC_WrAddChannelIndex(uiIndex, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s, index: %d", sr_strerror(rc), uiIndex);
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_WrSetChannelName(uiIndex, pstChannel->stCfg.acName, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetChannelFreqency(uiIndex, pstChannel->stCfg.ulLowerFreq, pstChannel->stCfg.ulUpperFreq, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetChannelAdminStatus(uiIndex, pstChannel->stCfg.enAdminStatus, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetChannelAttnCtrlMode(uiIndex, pstChannel->stCfg.enAttnCtrlMode, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetPortConnection(uiIndex, pstChannel->stSrcConnPort.acPortName, pstChannel->stDestConnPort.acPortName, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_WrSetDistributionFrequency(uiIndex, pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulLowerFreq,
                                        pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulUpperFreq, false);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    if (WR_ATTENUATION_DYNAMIC_LOSS == pstChannel->stCfg.enAttnCtrlMode) {
        rc = DSC_WrSetDistributionTargetPwr(uiIndex, pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulLowerFreq,
                                            pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulUpperFreq,
                                            pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.dTargetPwr, false);
    } else {
        rc = DSC_WrSetDistributionAttn(uiIndex, pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulLowerFreq,
                                       pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.ulUpperFreq,
                                       pstChannel->stSpecPwrProfile.astDist[0].stSpecPwrCfg.dAttn, false);
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

cleanup:
    return rc;
}

/* covert waveplan from wss to ocm */
int
DSC_WrConvertWaveplanToOcm(const WR_WAVEPLAN_LIST_ST *pstWaveplanList, WR_WAVEPLAN_LIST_ST *pstOcmWaveplanList)
{
    int rc = SR_ERR_OK;
    WR_WAVEPLAN_LIST_ST stOcmWaveplanList;
    uint32_t i = 0;
    uint64_t ulMinCenterFreq = 0, ulTmpLower = 0, ulTmpUpper = 0, ulTmpCenter = 0;
    bool bExistedChan = false;

    memset(&stOcmWaveplanList, 0, sizeof(stOcmWaveplanList));

    /* calc min center-frequency with bandwidth 12.5GHz, frequecy range [191325, 196162.5] GHz */
    i = 0;
    while (1) {
        /* calc lower frequncy */
        ulTmpCenter = WR_CENTER_FREQUENCY_BASE - i * WR_WSS_BAND_WIDTH_PER_SLICE;
        ulTmpLower = ulTmpCenter - WR_WSS_BAND_WIDTH_PER_SLICE;
        if (ulTmpLower < WR_LOWER_FREQUENCY_MIN) {
            break;
        }
        ulMinCenterFreq = ulTmpCenter;
        i++;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "ulMinCenterFreq: %ld, uitoal", ulMinCenterFreq, pstWaveplanList->uiTotal); */
    i = 0;
    /* int j = 0; */
    ulTmpCenter = ulMinCenterFreq;
    while (1) {
        /* 12.5Ghz channel */
        ulTmpLower = ulTmpCenter - WR_WSS_BAND_WIDTH_PER_SLICE;
        ulTmpUpper = ulTmpCenter + WR_WSS_BAND_WIDTH_PER_SLICE;

        if (ulTmpUpper > WR_UPPER_FREQUENCY_MAX) {
            /* over */
            break;
        }

        bExistedChan = false;
        for (; i < pstWaveplanList->uiTotal; i++) {
            if (ulTmpCenter < pstWaveplanList->astWaveplan[i].ulLowerFreq) {
                /* to add 12.5Ghz channel */
                break;
            } else if (ulTmpCenter >= pstWaveplanList->astWaveplan[i].ulLowerFreq &&
                       ulTmpCenter <= pstWaveplanList->astWaveplan[i].ulUpperFreq) {
                bExistedChan = true;
                /*LOG_WRITE(EN_LOG_NOTICE, "222ok: i: %d, j: %d,  lower: %ld, upper: %ld", i, j++,
                        pstWaveplanList->astWaveplan[i].ulLowerFreq, pstWaveplanList->astWaveplan[i].ulUpperFreq);*/
                rc = __AddChannelToWaveplanList(false, pstWaveplanList->astWaveplan[i].ulLowerFreq, pstWaveplanList->astWaveplan[i].ulUpperFreq, &stOcmWaveplanList);
                if (rc != SR_ERR_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by add channel00. i:%d, lower: %ld, upper: %ld", i,
                              pstWaveplanList->astWaveplan[i].ulLowerFreq,
                              pstWaveplanList->astWaveplan[i].ulUpperFreq);
                    rc = SR_ERR_INVAL_ARG;
                    goto END_LABEL;
                }
                break;
            } else {
                continue;
            }
        }

        /* find wss waveplan */
        if (bExistedChan) {
            ulTmpCenter = pstWaveplanList->astWaveplan[i].ulUpperFreq + WR_WSS_BAND_WIDTH_PER_SLICE;
            i++;
            continue;
        }

        /*LOG_WRITE(EN_LOG_NOTICE, "ok: i: %d, j: %d, lower: %ld, upper: %ld", i, j++, ulTmpLower, ulTmpUpper);*/
        rc = __AddChannelToWaveplanList(false, ulTmpLower, ulTmpUpper, &stOcmWaveplanList);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by add channel. lower: %ld, upper: %ld, ", ulTmpLower, ulTmpUpper);
            rc = SR_ERR_INVAL_ARG;
            goto END_LABEL;
        }

        ulTmpCenter += WR_CHANNEL_WIDTH_MIN;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "uiTotal: %d", stOcmWaveplanList.uiTotal); */

    memcpy(pstOcmWaveplanList, &stOcmWaveplanList, sizeof(stOcmWaveplanList));

END_LABEL:

    return rc;
}

/* get current waveplan */
int
DSC_WrGetOcmWaveplan(WR_WAVEPLAN_LIST_ST *pstOcmWaveplanList)
{
    int rc = SR_ERR_OK;
    MEDIA_CHANNELS_ST stMediaChannels;
    WR_WAVEPLAN_LIST_ST stWaveplanList;

    memset(&stMediaChannels, 0, sizeof(stMediaChannels));
    memset(&stWaveplanList, 0, sizeof(stWaveplanList));

    rc = DSC_WrGetMediaChannelsInfo(DSC_GetRunningSession(), &stMediaChannels);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting media-channels");
        goto END_LABEL;
    }

    rc = DSC_WrParseWaveplan((const MEDIA_CHANNELS_ST *)(&stMediaChannels), &stWaveplanList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by getting waveplan");
        goto END_LABEL;
    }

    rc = DSC_WrConvertWaveplanToOcm((const WR_WAVEPLAN_LIST_ST *)&stWaveplanList, pstOcmWaveplanList);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by converting waveplan from wss to ocm");
        goto END_LABEL;
    }

END_LABEL:
    DEF_SAFE_FREE(stMediaChannels.pastChanList);
    return rc;
}

static int
__ParsePortWaveform(char *pcBinaryWaveform, WSS_WAVEFORM_ST *pstWssWaveform)
{
    size_t nLen = 0;
    uint8_t *pucDecode = NULL;
    int rc = SR_ERR_OK;
    size_t i = 0, nPortIdx = 0, nPortPwrCnt = 0;
    int16_t sIntPwr = 0, sFracPwr = 0;

    if (NULL == pcBinaryWaveform || NULL == pstWssWaveform || pstWssWaveform->usTimePeriod <= 0 ||
        pstWssWaveform->usTimePeriod > 60) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid para");
        goto END_LABEL;
    }

    pucDecode = b64_decode_ex(pcBinaryWaveform, strlen(pcBinaryWaveform), &nLen);
    if (NULL == pucDecode) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "Error by b64_decode_ex");
        goto END_LABEL;
    }

    nPortPwrCnt = pstWssWaveform->usTimePeriod * 10;     /* 10 history data every 1s */

    if (nPortPwrCnt * 2 == nLen) {
        /* single port */
        pstWssWaveform->uiPortNum = 1;
    } else if (64 * nPortPwrCnt * 2 == nLen) {
        /* all port (64) */
        pstWssWaveform->uiPortNum = 64;
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid length: %ld", nLen);
        goto END_LABEL;
    }

    for (i = 0; i < nLen; ) {

        if (pucDecode[i] >= 128) {
            sIntPwr = (short)(pucDecode[i] - 128);
            sFracPwr = (short)pucDecode[i + 1];
            sIntPwr = -sIntPwr;
            sFracPwr = -sFracPwr;
        } else {
            sIntPwr = (short)pucDecode[i];
            sFracPwr = (short)pucDecode[i + 1];
        }

        nPortIdx = i / (nPortPwrCnt * 2);
        pstWssWaveform->aadPwr[nPortIdx][(i / 2) % nPortPwrCnt] = (double)sIntPwr + (double)sFracPwr / 100;

        i += 2;
    }

END_LABEL:

    DEF_SAFE_FREE(pucDecode);

    return rc;
}

int
DSC_RpcCollectPortWaveform(WSS_WAVEFORM_ST *pstWssWaveform)
{
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = DSC_GetRunningSession();
    size_t i = 0;

    if (NULL == pstWssWaveform) {
        rc = SR_ERR_INVAL_ARG;
        return rc;
    }

    input_cnt = 2;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_PORT_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_PORT_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = strdup(pstWssWaveform->acPortName);

    rc = sr_val_set_xpath(&input[1], ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_COLLECTION_TIME_PERIOD);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_COLLECTION_TIME_PERIOD, sr_strerror(rc));
        goto ERR_LABEL;
    }
    input[1].type = SR_UINT16_T;
    input[1].data.uint16_val = pstWssWaveform->usTimePeriod;

    /* set_uint16_item(&input[1]); */
    /* execute RPC */
    rc = DSC_RpcSend(sess, ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH, sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < output_cnt; i++) {
        if (0 == strcmp(output[i].xpath, ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_STATUS)) {
            if (strcmp(output[i].data.string_val, "Success") != 0) {
                LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[i].xpath, output[i].data.string_val);
                rc = SR_ERR_OPERATION_FAILED;
                break;
            }
        } else if (0 == strcmp(output[i].xpath, ALI_RPC_COLLECT_PORT_WAVEFORM_XPATH ALI_NODE_WAVEFORM)) {

            rc = __ParsePortWaveform(output[i].data.string_val, pstWssWaveform);
            if (SR_ERR_OK != rc) {
                break;
            }
        }
    }

ERR_LABEL:
    sr_free_values(input, input_cnt);
    sr_free_values(output, output_cnt);
    return rc;
}

bool
DSC_WrIsChannelExisted(uint32_t uiIndex)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[512] = {0};
    size_t nCnt = 0;

    pstSess = DSC_GetRunningSession();

    snprintf(acXpath, sizeof(acXpath), WR_XPATH_CHANNEL WR_NODE_INDEX, uiIndex);
    rc = get_items(pstSess, acXpath, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    if (NULL == pstVal) {
        rc = SR_ERR_NOT_FOUND;
    }

END_LABEL:

    sr_free_values(pstVal, nCnt);

    if (rc != SR_ERR_OK) {
        return false;
    } else {
        return true;
    }
}

int
DSC_WrGetIdleMinIndex(uint32_t *puiIndex)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    sr_session_ctx_t *pstSess = NULL;
    size_t nCnt = 0, i = 0, j = 0;

    pstSess = DSC_GetRunningSession();

    if (NULL == puiIndex) {
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(pstSess, WR_XPATH_CHANNEL_INDEX_ALL, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    if (0 == nCnt) {
        *puiIndex = 1;
        rc = SR_ERR_OK;
    } else {
        for (i = 1; i <= WR_INDEX_MAX; i++) {
            for (j = 0; j < nCnt; j++) {
                if (pstVal[j].data.uint32_val == i) {
                    break;
                }
            }

            if (j >= nCnt) {
                /* find min idle */
                *puiIndex = (uint32_t)i;
                rc = SR_ERR_OK;
                goto END_LABEL;
            }
        }

        if (i > WR_INDEX_MAX) {
            rc = SR_ERR_NOT_FOUND;
            goto END_LABEL;
        }
    }

END_LABEL:

    sr_free_values(pstVal, nCnt);

    return rc;
}

int
DSC_WRGetOldPortNameByNew(const char *pcPortName, char *pcOldPort, size_t nLen)
{
#if 0
    char cUpperChar;
    int iPortId = 0;

    if ((NULL == pcPortName) || (strlen(pcPortName) != 3) || (NULL == pcOldPort)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid parameters");
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(pcPortName, WR_STR_IN1)) {
        snprintf(pcOldPort, nLen, "%s", WR_STR_IN65);
        return OPLK_OK;
    } else if (0 == strcasecmp(pcPortName, WR_STR_IN2)) {
        snprintf(pcOldPort, nLen, "%s", WR_STR_IN66);
        return OPLK_OK;
    }

    cUpperChar = (char)toupper(pcPortName[0]);

    if (!isdigit(pcPortName[1]) || !isdigit(pcPortName[2])) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    iPortId = atoi(pcPortName + 1);
    if ('R' == cUpperChar || 'T' == cUpperChar) {
        if (iPortId < 1 || iPortId > 16) {
            goto ERR_LABEL;
        }

        if (1 == iPortId % 2) {
            snprintf(pcOldPort, nLen, "%s%02d", \
                     ('R' == cUpperChar) ? WR_STR_PREFIX_IN : WR_STR_PREFIX_OUT, (iPortId + 1) / 2);
        } else {
            snprintf(pcOldPort, nLen, "%s%02d", \
                     ('R' == cUpperChar) ? WR_STR_PREFIX_IN : WR_STR_PREFIX_OUT, 32 + iPortId / 2);
        }
    } else if ('M' == cUpperChar || 'D' == cUpperChar) {
        if (iPortId < 1 || iPortId > 48) {
            goto ERR_LABEL;
        }

        if (1 == iPortId % 2) {
            snprintf(pcOldPort, nLen, "%s%02d", \
                     ('M' == cUpperChar) ? WR_STR_PREFIX_IN : WR_STR_PREFIX_OUT, 32 - (iPortId - 1) / 2);
        } else {
            snprintf(pcOldPort, nLen, "%s%02d", \
                     ('M' == cUpperChar) ? WR_STR_PREFIX_IN : WR_STR_PREFIX_OUT, 64 - (iPortId - 2) / 2);
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
#endif
    return OPLK_ERR;
}
/*
   convert old portname (InXX) to new portname (R01~R16, M01~M48, IN1, IN2)
 */
int
DSC_WRGetNewPortNameByOldIn(const char *pcPortName, char *pcNewPortName, size_t nLen)
{
#if 0

    int iPortId = 0;

    if ((NULL == pcPortName) || (NULL == pcNewPortName)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid parameters");
        goto ERR_LABEL;
    }

    if ((0 != strncasecmp(pcPortName, WR_STR_PREFIX_IN, 2)) || (4 != strlen(pcPortName)) ||
        !isdigit(pcPortName[2]) || !isdigit(pcPortName[3])) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    iPortId = atoi(pcPortName + 2);

    if (iPortId >= 1 && iPortId <= 8) {
        snprintf(pcNewPortName, nLen, "R%02d", 2 * iPortId - 1);
    } else if (iPortId >= 9 && iPortId <= 32) {
        snprintf(pcNewPortName, nLen, "M%02d", 47 - (iPortId - 9) * 2);
    } else if (iPortId >= 33 && iPortId <= 40) {
        snprintf(pcNewPortName, nLen, "R%02d", 2 * (iPortId - 32));
    } else if (iPortId >= 41 && iPortId <= 64) {
        snprintf(pcNewPortName, nLen, "M%02d", 48 - (iPortId - 41) * 2);
    } else if (iPortId >= 65 && iPortId <= 66) {
        snprintf(pcNewPortName, nLen, "IN%d", iPortId - 64);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
#endif
    return OPLK_ERR;
}

/* map new portname to ops led idx, refer to OPS manual 5.3.1 */
int
DSC_WrGetOpsLedIdxByNewInPortName(const char *pcPortName, uint32_t *puiPordLedIdx)
{
#if 0

    int iPortId = 0;
    char cUpperChar;

    if ((NULL == pcPortName) || (NULL == puiPordLedIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid parameters");
        goto ERR_LABEL;
    }

    if (3 != strlen(pcPortName)) {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(WR_STR_IN1, pcPortName)) {
        *puiPordLedIdx = 64;
        return OPLK_OK;
    } else if (0 == strcasecmp(WR_STR_IN2, pcPortName)) {
        *puiPordLedIdx = 65;
        return OPLK_OK;
    }

    cUpperChar = (char)toupper(pcPortName[0]);
    if (!isdigit(pcPortName[1]) || !isdigit(pcPortName[2])) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    iPortId = atoi(pcPortName + 1);

    if (('R' == cUpperChar) && (iPortId >= 1 && iPortId <= 16)) {
        *puiPordLedIdx = (uint32_t)iPortId - 1;
    } else if (('M' == cUpperChar) && (iPortId >= 1 && iPortId <= 48)) {
        *puiPordLedIdx = (uint32_t)iPortId + 15;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Invalid port name: %s", pcPortName);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
#endif
    return OPLK_ERR;
}

