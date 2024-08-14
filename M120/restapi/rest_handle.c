/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_json_handle.c
* Description:   REST APIs of handling JSON data
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2016/12/08  Ver 1.0.0    Initial file.
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "rest_common.h"
#include "rest_handle.h"
#include "rest_msft.h"
#include "rest_api_auth.h"
#include "rest_api_alarm.h"
#include "rest_api_pm.h"
#include "rest_api_msft.h"
#include "rest_api_generic.h"
#include "rest_api_edfa.h"
#include "rest_api_ocm.h"
#include "rest_api_upg.h"
#include "rest_api_autoctrl.h"
#include "rest_api_ocs.h"

typedef struct {
    REST_HANDLE_ST *pstHandle;
    size_t nTotal;
} REST_API_LIST_ST;

static REST_API_LIST_ST m_stApiList = {.pstHandle = NULL, .nTotal = 0};

#define DEF_COMMON_API(method, url, bUrlMatchExactly, role, func) \
    {method, url, bUrlMatchExactly, role, func}

/* GET */
#define DEF_RO_GET_API(url, func) \
    DEF_COMMON_API(METHOD_GET, url, true, READ_ONLY_ROLE, func)

#define DEF_RO_GET_API_TRUNCATED_MATCH(url, func) \
    DEF_COMMON_API(METHOD_GET, url, false, READ_ONLY_ROLE, func)

/* POST */
#define DEF_RO_POST_API(url, func) \
    DEF_COMMON_API(METHOD_POST, url, true, READ_ONLY_ROLE, func)

#define DEF_RW_POST_API(url, func) \
    DEF_COMMON_API(METHOD_POST, url, true, READ_WRITE_ROLE, func)

#define DEF_RW_POST_API_TRUNCATED_MATCH(url, func) \
    DEF_COMMON_API(METHOD_POST, url, false, READ_WRITE_ROLE, func)

/* PUT */
#define DEF_RW_PUT_API(url, func) \
    DEF_COMMON_API(METHOD_PUT, url, true, READ_WRITE_ROLE, func)

#define DEF_RW_PUT_API_TRUNCATED_MATCH(url, func) \
    DEF_COMMON_API(METHOD_PUT, url, false, READ_WRITE_ROLE, func)

/* DELETE */
#define DEF_RW_DELETE_API(url, func) \
    DEF_COMMON_API(METHOD_DELETE, url, true, READ_WRITE_ROLE, func)

#define DEF_RW_DELETE_API_TRUNCATED_MATCH(url, func) \
    DEF_COMMON_API(METHOD_DELETE, url, false, READ_WRITE_ROLE, func)

/* END */
#define DEF_END_API \
    DEF_COMMON_API(METHOD_INVALID, {0}, true, READ_ONLY_ROLE, NULL)


static REST_HANDLE_ST m_astSysBaseHandle[] = {
    /* user */
    DEF_RO_POST_API(URL_OMT_USER_LOGIN, API_Login),
    DEF_RO_GET_API(URL_OMT_USER_LOGOUT, API_Logout),
    DEF_RO_GET_API(URL_OMT_USER, API_UserGetAll),
    DEF_RO_POST_API(URL_OMT_USER, API_UserAddOrUpdate),     /* readonly user may modify own password */
    DEF_RW_DELETE_API_TRUNCATED_MATCH(URL_OMT_USER_ROOT, API_UserDelete),

    /* system */
    DEF_RO_GET_API(URL_OMT_CONFIG, API_GetFullConfig),
    DEF_RW_POST_API(URL_OMT_CONFIG, API_SetFullConfig),
    DEF_RW_POST_API(URL_OMT_CONFIG_SYSTEM, API_SetSystem),
    DEF_RW_PUT_API(URL_OMT_CONFIG_SYSTEM, API_SetSystemOptional),
    DEF_RO_GET_API(URL_OMT_INVENTORY, API_GetInventory),

    /* upgrade */
    DEF_RO_GET_API(URL_OMT_SOFTWARE_UPGRADE, API_GetSoftwareState),
    DEF_RO_GET_API(URL_OMT_SOFTWARE_ACTIVATE, API_GetSoftwareState),
    DEF_RW_POST_API(URL_OMT_SOFTWARE_UPGRADE, API_SoftwareUpgrade),
    DEF_RW_POST_API(URL_OMT_SOFTWARE_DOWNLOAD, API_SoftwareDownLoad),
    DEF_RW_POST_API(URL_OMT_SOFTWARE_ACTIVATE, API_SoftwareActivate),

    /* alarm */
    DEF_RO_GET_API(URL_OMT_CURRENT_ALARMS, API_GetCurrentAlarms),

    DEF_RO_GET_API(URL_OMT_SLOTS, API_GetAllSlotsStatus),

    /* pm */
    DEF_RO_GET_API(URL_OMT_CURRENT_PM_VALUES, API_GetCurrentPmValues),
    DEF_RO_GET_API(URL_OMT_CURRENT_PM_15MIN, API_GetCurrentPm15min),
    DEF_RO_GET_API(URL_OMT_CURRENT_PM_24HOUR, API_GetCurrentPm24hour),

    /* led */
    DEF_RW_POST_API(URL_OMT_OPERATIONS_LAMP_TEST, API_LampTest),

    /* restart */
    DEF_RW_POST_API(URL_OMT_OPERATIONS_WARMRESTART, API_Restart),
    DEF_RW_POST_API(URL_OMT_OPERATIONS_COLDRESTART, API_Restart),

    /* transfer */
    DEF_RW_POST_API(URL_OMT_OPERATIONS_TRANSFER_FILE, API_TransferFile),
    DEF_RW_POST_API(URL_OMT_OPERATIONS_TRANSFER_LOG, API_TransferLog),

    /* security */
    DEF_RO_GET_API(URL_OMT_IPACL, API_GetIpacl),
    DEF_RW_POST_API(URL_OMT_IPACL, API_SetIpacl),
    DEF_END_API
};

static REST_HANDLE_ST m_astAutoCtrlHandle[EN_REST_AMP_CARD_MAX][10] = {
    {
        DEF_RO_GET_API(URL_OMT_OPERATIONS_AUTO_GAIN_W, API_GetAutoGainCtrlState),
        DEF_RO_GET_API(URL_OMT_CONFIG_THRESHOLD, API_GetSpanlossThreshold),
        DEF_RO_GET_API(URL_OMT_REMOTE_NODE_INFO, API_GetRemoteNodeInfo),
        DEF_RW_POST_API(URL_OMT_OPERATIONS_AUTO_GAIN_W, API_StartAutoGainCtrl),
        DEF_RW_POST_API(URL_OMT_CONFIG_THRESHOLD, API_SetSpanlossThreshold),
        DEF_RO_GET_API_TRUNCATED_MATCH(URL_OMT_GET_FIBER_CONFIG_BY_TYPE, API_GetFiberCfgByType),
        DEF_RW_POST_API(URL_OMT_CLEAR_AUTO_ALARMS, API_ClearAutoAlarms),
        DEF_END_API

    }, {
        DEF_RO_GET_API(URL_OMT_OPERATIONS_AUTO_GAIN_P, API_GetAutoGainCtrlState),
        DEF_RO_GET_API(URL_OMT_CONFIG_THRESHOLD_P, API_GetSpanlossThreshold),
        DEF_RO_GET_API(URL_OMT_REMOTE_NODE_INFO_P, API_GetRemoteNodeInfo),
        DEF_RW_POST_API(URL_OMT_OPERATIONS_AUTO_GAIN_P, API_StartAutoGainCtrl),
        DEF_RW_POST_API(URL_OMT_CONFIG_THRESHOLD_P, API_SetSpanlossThreshold),
        DEF_RO_GET_API_TRUNCATED_MATCH(URL_OMT_GET_FIBER_CONFIG_BY_TYPE_P, API_GetFiberCfgByType),
        DEF_END_API
    }
};

static REST_HANDLE_ST m_astOaHandle[EN_REST_AMP_CARD_MAX][10] = {
    {
        DEF_RW_POST_API(URL_OMT_CONFIG_PREAMP, API_EdfaSetPA),
        DEF_RW_POST_API(URL_OMT_CONFIG_BOOSTERAMP, API_EdfaSetBA),
        DEF_RW_PUT_API(URL_OMT_CONFIG_PREAMP, API_EdfaSetPaOptional),
        DEF_RW_PUT_API(URL_OMT_CONFIG_BOOSTERAMP, API_EdfaSetBaOptional),
        DEF_END_API
    }, {
        DEF_RW_POST_API(URL_OMT_CONFIG_PREAMP_P, API_EdfaSetPA),
        DEF_RW_POST_API(URL_OMT_CONFIG_BOOSTERAMP_P, API_EdfaSetBA),
        DEF_RW_PUT_API(URL_OMT_CONFIG_PREAMP_P, API_EdfaSetPaOptional),
        DEF_RW_PUT_API(URL_OMT_CONFIG_BOOSTERAMP_P, API_EdfaSetBaOptional),
        DEF_END_API
    }
};

static REST_HANDLE_ST m_astOcmHandle[] = {

#if 0
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_CHANNEL_MONITORS_ALL, API_sr_get),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_CHANNEL_MONITORS_ALL, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_CHANNEL_MONITORS_ALL, API_sr_set),
#else
    DEF_RO_GET_API(URL_DATA_CHANNEL_MONITORS_ALL, API_GetOcmAllData),
    DEF_RO_GET_API(URL_DATA_OCM_CONFIG, API_GetOcmModeCfg),
    DEF_RO_GET_API(URL_DATA_OCM_STATE, API_GetOcmModeState),
    DEF_RW_POST_API(URL_DATA_OCM_CONFIG, API_SetOcmModeCfg),

    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_CHANNEL_MONITOR_ONE, API_GetOcmOnePortData),
    /* DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_CHANNEL_MONITOR_ONE, API_SetOcmOnePortData), */ /* NOT support for M120 */
#endif
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OCM_OSA_ONE, API_GetOcmOsaData),

    DEF_END_API
};

static REST_HANDLE_ST m_astOpsHandle[] = {
    DEF_RO_GET_API(URL_DATA_OPS_ALL, API_sr_get),
    DEF_RO_GET_API(URL_DATA_OPS_CONFIG, API_sr_get),
    DEF_RO_GET_API(URL_DATA_OPS_STATE, API_sr_get),
    DEF_RO_GET_API(URL_DATA_OPS_PORTS, API_sr_get),

    DEF_RW_POST_API(URL_DATA_OPS_CONFIG, API_sr_set),
    DEF_RW_PUT_API(URL_DATA_OPS_CONFIG, API_sr_set),

    DEF_END_API
};

static REST_HANDLE_ST m_astOtdrHandle[] = {
    DEF_RO_GET_API(URL_DATA_OPENCONFIG_OTDR_SOR_FILES, API_GetOtdrSorFiles),

    DEF_RW_POST_API_TRUNCATED_MATCH(URL_OPT_OTDR_SCAN, API_sr_rpc),

    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OPENCONFIG_OTDR, API_sr_get),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_OPENCONFIG_OTDR, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_OPENCONFIG_OTDR, API_sr_set),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_OPT_SET_TRACE_BASELINE, API_sr_rpc),

    DEF_END_API

};

static REST_HANDLE_ST m_astMuxHandle[] = {
    DEF_RO_GET_API(URL_OMT_MUX_CONFIG, API_GetMuxConfig),
    DEF_RW_POST_API(URL_OMT_MUX_CONFIG, API_SetMuxConfig),
    DEF_RO_GET_API(URL_OMT_MUX_STATE, API_GetMuxState),
    DEF_END_API
};

static REST_HANDLE_ST m_astApsdHandle[EN_REST_AMP_CARD_MAX][6] = {
    {
        DEF_RO_GET_API(URL_OMT_OPERATIONS_APSD, API_GetApsd),
        /* DEF_RW_POST_API(URL_OMT_OPERATIONS_APSD, API_SetApsd), */
        DEF_END_API
    }, {
        DEF_RO_GET_API(URL_OMT_OPERATIONS_APSD_P, API_GetApsd),
        /* DEF_RW_POST_API(URL_OMT_OPERATIONS_APSD_P, API_SetApsd), */
        DEF_END_API
    }
};

/* support all urls in openconfig-transport-line-connectivity.yaml */
static REST_HANDLE_ST m_astOcsHandle[] = {
#if 0
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTIONS_ALL, API_sr_get),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTIONS_ALL, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTIONS_ALL, API_sr_set),
#else
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTIONS_ALL, API_sr_get),
    DEF_RW_POST_API(URL_DATA_OCS_CONNECTIONS_ALL, API_OcsAddConnections),
    DEF_RW_PUT_API(URL_DATA_OCS_CONNECTIONS_ALL, API_OcsReplaceConnections),
    DEF_RW_DELETE_API(URL_DATA_OCS_CONNECTIONS_ALL, API_OcsDeleteAllConnections),

    DEF_RW_POST_API(URL_DATA_OCS_CONNECTION, API_OcsSetOneConnection),
    /* DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTION_ONE, API_OcsSetOneConnection), */

    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTION_ONE, API_OcsSetOneConnectionConfig),
    DEF_RW_DELETE_API_TRUNCATED_MATCH(URL_DATA_OCS_CONNECTION_ONE, API_OcsDeleteConnection),

#endif
    DEF_END_API
};


/* support all urls in com-oplink-dfb.yaml */
static REST_HANDLE_ST m_astDfbHandle[] = {
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_DFB, API_sr_get),

    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_DFB, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_DFB, API_sr_set),

    DEF_END_API
};

/* support all urls in openconfig-optical-amplifier.yaml */
static REST_HANDLE_ST m_astOcAmplifierHandle[] = {
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OC_AMPLIFIER_ROOT, API_sr_get),

    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_OC_AMPLIFIER_ROOT, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_OC_AMPLIFIER_ROOT, API_sr_set),

    DEF_END_API
};

/* support all urls in openconfig-optical-attenuator.yaml */
static REST_HANDLE_ST m_astOcAttenuatorHandle[] = {
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_OC_ATTENUATOR_ROOT, API_sr_get),

    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_OC_ATTENUATOR_ROOT, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_OC_ATTENUATOR_ROOT, API_sr_set),

    DEF_END_API
};

/* support all urls in com-oplink-laser-safty.yaml */
static REST_HANDLE_ST m_astLaserSaftyHandle[] = {
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_LASER_SAFTY_ROOT, API_sr_get),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_LASER_SAFTY_ROOT, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_LASER_SAFTY_ROOT, API_sr_set),

    DEF_END_API
};


static REST_HANDLE_ST m_stRestGenericHandle[] = {
    DEF_RO_GET_API_TRUNCATED_MATCH(URL_DATA_PREFIX, API_sr_get),
    DEF_RW_DELETE_API_TRUNCATED_MATCH(URL_DATA_PREFIX, API_sr_delete),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_PREFIX, API_sr_set),
    DEF_RW_PUT_API_TRUNCATED_MATCH(URL_DATA_PREFIX, API_sr_set),
    DEF_RW_POST_API_TRUNCATED_MATCH(URL_DATA_PREFIX, API_sr_rpc),
    DEF_END_API
};


REST_HANDLE_ST *
REST_GetHandle(REQ_METHOD_EN enMethod, const char *pcUrl)
{
    size_t i = 0;
    REST_HANDLE_ST *pstHandle = NULL;

    for (i = 0; i < m_stApiList.nTotal; i++) {
        pstHandle = m_stApiList.pstHandle + i;

        if (pstHandle->enMethod != enMethod) {
            continue;
        }

        if (pstHandle->bUrlMatchExactly) {
            if (0 == strcasecmp(pstHandle->acUrlPath, pcUrl)) {
                return pstHandle;
            }
        } else {
            if (0 == strncasecmp(pstHandle->acUrlPath, pcUrl, strlen(pstHandle->acUrlPath))) {
                return pstHandle;
            }
        }
    }

    return NULL;
}

static int
__RegisterApi(REST_HANDLE_ST *pstHandle, size_t nCnt)
{
    void *pvMem = NULL;
    size_t nSize = 0;
    size_t nValidCnt = 0;

    if (NULL == pstHandle) {
        LOG_WRITE(EN_LOG_ERROR, "pstHandle is NULL");
        return OPLK_ERR;
    }

    /* count the number of APIs */
    for (nValidCnt = 0; nValidCnt < nCnt; nValidCnt++) {
        if (NULL == (pstHandle + nValidCnt)->pfHandlerFunc) {
            break;
        }
    }

    if (0 == nValidCnt) {
        /* ignore */
        return OPLK_OK;
    }

    nSize = sizeof(REST_HANDLE_ST) * (nValidCnt + m_stApiList.nTotal);
    pvMem = realloc(m_stApiList.pstHandle, nSize);
    if (NULL == pvMem) {
        LOG_WRITE(EN_LOG_ERROR, "Error realloc");
        return OPLK_ERR;
    }

    m_stApiList.pstHandle = (REST_HANDLE_ST *)pvMem;

    /* add new APIs */
    nSize = sizeof(REST_HANDLE_ST) * nValidCnt;
    memset(&m_stApiList.pstHandle[m_stApiList.nTotal], 0, nSize);
    memcpy(&m_stApiList.pstHandle[m_stApiList.nTotal], pstHandle, nSize);

    m_stApiList.nTotal += nValidCnt;

    return OPLK_OK;
}

int
REST_RegisterApi()
{

    __RegisterApi(m_astSysBaseHandle, sizeof(m_astSysBaseHandle));

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_AMP)) {
        if (DSC_OCP_IsComponentExist(COMPONENT_KEY_AMP)) {
            __RegisterApi(m_astOaHandle[EN_REST_AMP_W_CARD], sizeof(m_astOaHandle[EN_REST_AMP_W_CARD]));

            __RegisterApi(m_astAutoCtrlHandle[EN_REST_AMP_W_CARD], sizeof(m_astAutoCtrlHandle[EN_REST_AMP_W_CARD]));

            __RegisterApi(m_astApsdHandle[EN_REST_AMP_W_CARD], sizeof(m_astApsdHandle[EN_REST_AMP_W_CARD]));
        }

        if (DSC_OCP_IsComponentExist(COMPONENT_KEY_AMP_P)) {
            __RegisterApi(m_astOaHandle[EN_REST_AMP_P_CARD], sizeof(m_astOaHandle[EN_REST_AMP_P_CARD]));

            __RegisterApi(m_astAutoCtrlHandle[EN_REST_AMP_P_CARD], sizeof(m_astAutoCtrlHandle[EN_REST_AMP_P_CARD]));

            __RegisterApi(m_astApsdHandle[EN_REST_AMP_P_CARD], sizeof(m_astApsdHandle[EN_REST_AMP_P_CARD]));
        }

        __RegisterApi(m_astOcAmplifierHandle, sizeof(m_astOcAmplifierHandle));

    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OCM_OPENCONFIG)) {
        __RegisterApi(m_astOcmHandle, sizeof(m_astOcmHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OTDR)) {
#if defined(__HCF__)
        /* NOT support OTDR */
        (void)m_astOtdrHandle;         /* Happy compiler */
#else
        __RegisterApi(m_astOtdrHandle, sizeof(m_astOtdrHandle));
#endif
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), DEF_MODULE_NS_CV)) {
        __RegisterApi(m_astMuxHandle, sizeof(m_astMuxHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OPS)) {
        __RegisterApi(m_astOpsHandle, sizeof(m_astOpsHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OCS)) {
        __RegisterApi(m_astOcsHandle, sizeof(m_astOcsHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_DFB)) {
        __RegisterApi(m_astDfbHandle, sizeof(m_astDfbHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_EVOA)) {
        __RegisterApi(m_astOcAttenuatorHandle, sizeof(m_astOcAttenuatorHandle));
    }

    if (true == DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_LASER_SAFTY)) {
        __RegisterApi(m_astLaserSaftyHandle, sizeof(m_astLaserSaftyHandle));
    }

    /* debug */
    __RegisterApi(m_stRestGenericHandle, sizeof(m_stRestGenericHandle));

    return OPLK_OK;

}

int
REST_UnRegisterApi()
{

    DEF_SAFE_FREE(m_stApiList.pstHandle);
    m_stApiList.nTotal = 0;

    return OPLK_OK;
}

