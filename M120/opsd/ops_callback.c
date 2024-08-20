
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "sysrepo.h"
#include "DSC_inc.h"
#include "oplkipc.h"
#include "oplkalm.h"
#include "ops_callback.h"
#include "ops_common.h"
#include "ops_protocol.h"
#include "DevOpsCApi.h"
#include "DevOpsData.h"
#include "DevBoardData.h"
#include "DevBoardCApi.h"

static OPS_RANGE_ST m_stOpsRange;

typedef enum {
    OPS_IDX_ALM_SWITCH = 0,
    OPS_IDX_ALM_SWITCH_WAVEFORM_REPORT,
    OPS_IDX_ALM_SWITCH_PROTOCOL_FAILURE,
    OPS_IDX_ALM_MFG_FAIL,
    OPS_IDX_ALM_SWITCH_FAIL,
    OPS_IDX_ALM_POWER_DIFF,
    OPS_IDX_ALM_COM_RLOS,
    OPS_IDX_ALM_COM_TLOS,
    OPS_IDX_ALM_CNT
} OPS_ALM_INDEX_EN;

/**************************************************************************************************/
/* Alarm unit define for single OPS                                                               */
/* @ {                                                                                            */
#define OPS_ALARM_UNIT(default, number)         OPS_ALARM_UNIT_ ## default (number)
#define OPS_PORT_ALARM_UNIT(default, number)    OPS_PORT_ALARM_UNIT_ ## default (number)

#define OPS_ALARM_UNIT_1(number)                                                                   \
    {"OPS_SWITCH", "", "", 0, false, false},                                                       \
    {"OPS_SWITCH_WAVEFORM_REPORT", "", "", 0, false, false},                                       \
    {"OPS_SWITCH_COMM_FAIL", "", "", 0, false, false},                                             \
    {"OPS_FAIL", "", "", 0, false, false},                                                         \
    {"OPS_SWITCH_FAIL", "", "", 0, false, false},                                                  \
    {"OPS_RECEIVE_POWER_DIFF", "", "", 0, false, false},                                           \
    {"OPS_OPS_R_LOS", "", "", 0, false, false},                                                    \
    {"OPS_OPS_T_LOS", "", "", 0, false, false}


#define OPS_ALARM_UNIT_0(number) \
    {"OPS" # number "_SWITCH", "", "", 0, false, false},                                           \
    {"OPS" # number "_SWITCH_WAVEFORM_REPORT", "", "", 0, false, false},                           \
    {"OPS" # number "_SWITCH_COMM_FAIL", "", "", 0, false, false},                                 \
    {"OPS" # number "_FAIL", "", "", 0, false, false},                                             \
    {"OPS" # number "_SWITCH_FAIL", "", "", 0, false, false},                                      \
    {"OPS" # number "_RECEIVE_POWER_DIFF", "", "", 0, false, false},                               \
    {"OPS" # number "_OPS_R_LOS", "", "", 0, false, false},                                        \
    {"OPS" # number "_OPS_T_LOS", "", "", 0, false, false}

#define OPS_PORT_ALARM_UNIT_1(number)                                                              \
    {                                                                                              \
        {"OPS_LINE1R_LOS", "", "", 0, false, false},                                               \
    },                                                                                             \
    {                                                                                              \
        {"OPS_LINE2R_LOS", "", "", 0, false, false},                                               \
    }

#define OPS_PORT_ALARM_UNIT_0(number)                                                              \
    {                                                                                              \
        {"OPS" # number "_LINE1R_LOS", "", "", 0, false, false},                                   \
    },                                                                                             \
    {                                                                                              \
        {"OPS" # number "_LINE2R_LOS", "", "", 0, false, false},                                   \
    }
/* @ }                                                                                            */

/**************************************************************************************************/
/* Alarm list for multiple ops module                                                             */
/* Note: The single OPS alarms are defined in m_astOpsAlm[0].                                     */
/*       Meanwhile for Multiple OPS, they start at m_astOpsAlm[1].                                */
/*       This note is also applicable for port alarms.                                            */
/**************************************************************************************************/
/* @ {                                                                                            */
static ALM_UNIT_ST m_astOpsAlm[OPS_DEV_NUM_MAX + 1][OPS_IDX_ALM_CNT] = {
    {OPS_ALARM_UNIT(1, 0)},
    {OPS_ALARM_UNIT(0, 1)},
    {OPS_ALARM_UNIT(0, 2)},
    {OPS_ALARM_UNIT(0, 3)},
    {OPS_ALARM_UNIT(0, 4)},
    {OPS_ALARM_UNIT(0, 5)},
    {OPS_ALARM_UNIT(0, 6)},
    {OPS_ALARM_UNIT(0, 7)},
    {OPS_ALARM_UNIT(0, 8)},
    {OPS_ALARM_UNIT(0, 9)}
};
static ALM_UNIT_ST m_aastOpsPortAlm[OPS_DEV_NUM_MAX + 1][OPS_PORT_LINE_IN_CNT][PORT_ALARM_TYPE_CNT] = {
    {OPS_PORT_ALARM_UNIT(1, 0)},
    {OPS_PORT_ALARM_UNIT(0, 1)},
    {OPS_PORT_ALARM_UNIT(0, 2)},
    {OPS_PORT_ALARM_UNIT(0, 3)},
    {OPS_PORT_ALARM_UNIT(0, 4)},
    {OPS_PORT_ALARM_UNIT(0, 5)},
    {OPS_PORT_ALARM_UNIT(0, 6)},
    {OPS_PORT_ALARM_UNIT(0, 7)},
    {OPS_PORT_ALARM_UNIT(0, 8)},
    {OPS_PORT_ALARM_UNIT(0, 9)}
};
/* @ }                                                                                            */

/**************************************************************************************************/
/* Alarm bit define for OPS with one or more ports                                                */
/**************************************************************************************************/
/* @ {                                                                                            */
#define ALARM_BYTE_UNPACK(byte, port) ((byte) >> ((port) - OPS_SUB_INDEX_DEFAULT) & 0x01)
/* @ }                                                                                            */

#define __OPS_SET_VALUE_DOUBLE(pcXpath, dValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        pstVal[i].type = SR_DECIMAL64_T; \
        pstVal[i++].data.decimal64_val = (dValue); \
}

#define __OPS_SET_VALUE_UINT32(pcXpath, uiValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        pstVal[i].type = SR_UINT32_T; \
        pstVal[i++].data.uint32_val = (uiValue); \
}

#define __OPS_SET_VALUE_UINT64(pcXpath, ulValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        pstVal[i].type = SR_UINT64_T; \
        pstVal[i++].data.uint64_val = (ulValue); \
}

#define __OPS_SET_VALUE_ENUM(pcXpath, pcValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        sr_val_set_str_data(&pstVal[i++], SR_ENUM_T, (pcValue)); \
}

#define __OPS_SET_VALUE_STRING(pcXpath, pcValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        sr_val_set_str_data(&pstVal[i++], SR_STRING_T, (pcValue)); \
}

#define __OPS_SET_VALUE_BOOL(pcXpath, bValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        pstVal[i].type = SR_BOOL_T; \
        pstVal[i++].data.uint64_val = (bValue); \
}

#define __OPS_SET_VALUE_IDENTITYREF(pcXpath, pcValue)  { \
        sr_val_set_xpath(&pstVal[i], (pcXpath)); \
        sr_val_set_str_data(&pstVal[i++], SR_IDENTITYREF_T, (pcValue)); \
}

/**************************************************************************************************/
/* definition of function to check threshold                                                      */
/**************************************************************************************************/
/* @ {                                                                                            */
#define CHECK_TCA_RANGE(type, high, low, thr_high, thr_low, hys_high, hys_low) do {                    \
        if ((high) < (low)) {                                                                          \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "high < low : %d %d", (high), (low));                              \
            return rc;                                                                                 \
        }                                                                                              \
        sThr = (OPS_THR_TYPE_HIGH == type || OPS_THR_TYPE_DIFFER == type) ? (high) : (low);            \
        if ((sThr) < (thr_low)) {                                                                      \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "range: %d %d, low: %d", (thr_low), (thr_high), (sThr));           \
            return rc;                                                                                 \
        }                                                                                              \
        if ((sThr) > (thr_high)) {                                                                     \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "range: %d %d, high: %d", (thr_low), (thr_high), (sThr));          \
            return rc;                                                                                 \
        }                                                                                              \
        if ((high) - (low) > (hys_high) || (high) - (low) < (hys_low)) {                               \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "range: %d %d, diff: %d", (hys_low), (hys_high), (high) - (low));  \
            return rc;                                                                                 \
        }                                                                                              \
} while (0)

#define CHECK_TCA_INTERSECTION(toolow_lowthr, toolow_highthr, toohigh_lowthr, toohigh_highthr) do {    \
        if ((toolow_lowthr) >= (toohigh_highthr)) {                                                    \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "toolow_lowthr(%d) >= toohigh_highthr(%d)", (toolow_lowthr), (toohigh_highthr)); \
            return rc;                                                                                 \
        }                                                                                              \
        if ((toolow_highthr) >= (toohigh_lowthr)) {                                                    \
            rc = SR_ERR_INVAL_ARG;                                                                     \
            LOG_WRITE(EN_LOG_ERROR, "toolow_highthr(%d) >= toohigh_lowthr(%d)", (toolow_highthr), (toohigh_lowthr)); \
            return rc;                                                                                 \
        }                                                                                              \
} while (0)
/* @ }                                                                                            */

#define OPS_TCA_SIZE (TCA_IDX_OPS_END - TCA_IDX_OPS_START + 1)

static int
__InitOpsParaRangeFromSysrepo(void)
{
    int rc = SR_ERR_INVAL_ARG;
    sr_session_ctx_t *pstStartSess = NULL;
    pstStartSess = DSC_GetStartupSession();
    if (NULL == pstStartSess) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetStartupSession fail");
        return rc;
    }

    double dMax = 0.0;
    double dMin = 0.0;

    /* OC2A-304 walkround: can NOT read correct value of "ops-comr-high-threshold" range from sysrepo, */
    /* the reason is unknown, so use "ops-comr-low-threshold" instead of "ops-comr-high-threshold" */
    /* because they are equal at present. */
    #define OFFSET_OPS_RANGE(p) offsetof(struct OPS_RANGE_st, p)
    struct ST_PARAS_MAP
    {
        const char *name;
        int pos;
    } astNameMaps[] = {{"ops-wtr", OFFSET_OPS_RANGE(uiWtrMax)},
                       {"ops-holdoff-time", OFFSET_OPS_RANGE(uiHoldOffMax)},
                       {"ops-threshold", OFFSET_OPS_RANGE(dPrimaryThrMax)},
                       {"ops-hysteresis", OFFSET_OPS_RANGE(dPrimaryHysMax)},
                       {"ops-threshold", OFFSET_OPS_RANGE(dSecondaryThrMax)},
                       {"ops-hysteresis", OFFSET_OPS_RANGE(dSecondaryHysMax)},
                       {"relative-switch-threshold", OFFSET_OPS_RANGE(dRelativeThrMax)},
                       {"relative-switch-threshold-offset", OFFSET_OPS_RANGE(dRelativeOffsetMax)},
                       {"ops-comr-los-threshold", OFFSET_OPS_RANGE(dComRLosThrMax)},
                       {"ops-comr-los-hysteresis", OFFSET_OPS_RANGE(dComRLosHysMax)},
                       {"ops-differ-threshold", OFFSET_OPS_RANGE(dDifferThrMax)},
                       {"ops-differ-hysteresis", OFFSET_OPS_RANGE(dDifferHysMax)},
                       {"ops-comr-low-threshold", OFFSET_OPS_RANGE(dComRLowThrMax)},
                       {"ops-comr-low-hysteresis", OFFSET_OPS_RANGE(dComRLowHysMax)},
                       /* {"ops-comr-high-threshold", OFFSET_OPS_RANGE(dComRHighThrMax)}, */
                       {"ops-comr-low-threshold", OFFSET_OPS_RANGE(dComRHighThrMax)},
                       {"ops-comr-high-hysteresis", OFFSET_OPS_RANGE(dComRHighHysMax)},
                       {"ops-prir-low-threshold", OFFSET_OPS_RANGE(dPriRLowThrMax)},
                       {"ops-prir-low-hysteresis", OFFSET_OPS_RANGE(dPriRLowHysMax)},
                       /* {"ops-prir-high-threshold", OFFSET_OPS_RANGE(dPriRHighThrMax)}, */
                       {"ops-prir-low-threshold", OFFSET_OPS_RANGE(dPriRHighThrMax)},
                       {"ops-prir-high-hysteresis", OFFSET_OPS_RANGE(dPriRHighHysMax)},
                       {"ops-secr-low-threshold", OFFSET_OPS_RANGE(dSecRLowThrMax)},
                       {"ops-secr-low-hysteresis", OFFSET_OPS_RANGE(dSecRLowHysMax)},
                       /* {"ops-secr-high-threshold", OFFSET_OPS_RANGE(dSecRHighThrMax)}, */
                       {"ops-secr-low-threshold", OFFSET_OPS_RANGE(dSecRHighThrMax)},
                       {"ops-secr-high-hysteresis", OFFSET_OPS_RANGE(dSecRHighHysMax)}};

    size_t nCount = sizeof(astNameMaps) / sizeof(astNameMaps[0]);
    size_t nItem = 0;
    char *pcItem = (char *)&m_stOpsRange;

    for (int i = 0; i < nCount; i++) {
        dMax = dMin = 0.0;
        rc = DSC_GetParameterByName(pstStartSess, astNameMaps[i].name, &dMax, &dMin);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get %s range fail", astNameMaps[i].name);
            return rc;
        }

        nItem = i == (nCount - 1) ? (sizeof(OPS_RANGE_ST) - astNameMaps[i].pos) : (astNameMaps[i + 1].pos - astNameMaps[i].pos);
        nItem /= 2;

        if (nItem == sizeof(uint32_t)) {
            *((uint32_t *)(pcItem + astNameMaps[i].pos)) = (uint32_t)dMax;
            *((uint32_t *)(pcItem + astNameMaps[i].pos + nItem)) = (uint32_t)dMin;
        } else {
            *((double *)(pcItem + astNameMaps[i].pos)) = dMax;
            *((double *)(pcItem + astNameMaps[i].pos + nItem)) = dMin;
        }
    }

    return SR_ERR_OK;
}

int
OPS_OpsGetParaRange(OPS_RANGE_ST *pstRange)
{
    static bool m_bFlag = false;

    if (NULL == pstRange) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr is null");
        return SR_ERR_INVAL_ARG;
    }

    if (false == m_bFlag) {
        if (SR_ERR_OK != __InitOpsParaRangeFromSysrepo()) {
            LOG_WRITE(EN_LOG_ERROR, "init get ops range fail");
            goto ERR_LABEL;
        }
        m_bFlag = true;

    }

    memcpy(pstRange, &m_stOpsRange, sizeof(*pstRange));
    return SR_ERR_OK;
ERR_LABEL:
    return SR_ERR_INVAL_ARG;
}

int
__OpsSwitchingTypeEnumToStr(int iOpsType, char *pcStr)
{
    if (NULL == pcStr) {
        LOG_WRITE(EN_LOG_ERROR, "Arg is null");
        return SR_ERR_INVAL_ARG;
    }

    switch (iOpsType) {
    case OPS_SWITCHING_TYPE_UNI:
    {
        strcpy(pcStr, STR_UNIDIRECTIONAL);
        break;
    }
    case OPS_SWITCHING_TYPE_BI:
    {
        strcpy(pcStr, STR_BIDIRECTIONAL);
        break;
    }
    default:
    {
        LOG_WRITE(EN_LOG_ERROR, "unsupport switching type %d", iOpsType);
        strcpy(pcStr, STR_UNIDIRECTIONAL);
    }
    } /* end of switch */

    return OPLK_OK;
}

/*
   [OLSP OPS FSM status]:
   0 - Init
   1 - Blocked
   2 - No request -Worker selected(NR-W)
   3 - No request -Protection selected(DNR)
   4 - Manual switch -Worker selected(MS-W)
   5 - Manual switch -Protection selected(MS-P)
   6 - Signal fail -Worker selected(SF-P)
   7 - Signal fail -Protection selected(SF-W)
   8 - Forced switch -Worker selected(FS-W)
   9 - Forced switch -Protection selected(FS-P)
   10 - Wait to restore(WtR)
   11- Lock out
   12-Relative switch - Worker selected(RS-W)
   13-Relative switch - Protection selected(RS-P)
 */

int
__OpsFsmEnumToStr(int iOpsStatus, char *pcStr)
{
    if (NULL == pcStr) {
        LOG_WRITE(EN_LOG_ERROR, "Arg is null");
        return SR_ERR_INVAL_ARG;
    }

    switch (iOpsStatus) {
    case OPS_STATUS_LOCK_OUT:
    case OPS_STATUS_FORCE2WORK:
    {
        strcpy(pcStr, STR_LOP);
        break;
    }

    case OPS_STATUS_FORCE2PROTECT:
    {
        strcpy(pcStr, STR_FS);
        break;
    }

    case OPS_STATUS_SIGNAL_FAIL_WORK:
    {
        strcpy(pcStr, STR_SF_P);
        break;
    }

    case OPS_STATUS_SIGNAL_FAIL_PROTECT:
    {
        strcpy(pcStr, STR_SF);
        break;
    }

    case OPS_STATUS_MANUAL2WORK:
    {
        strcpy(pcStr, STR_MS);
        break;
    }

    case OPS_STATUS_MANUAL2PROTECT:
    {
        strcpy(pcStr, STR_MS_P);
        break;
    }

    case OPS_STATUS_WTR:
    {
        strcpy(pcStr, STR_WTR);
        break;
    }

    case OPS_STATUS_INIT:
    case OPS_STATUS_NO_REQUEST_WORK:
    {
        strcpy(pcStr, STR_NR);
        break;
    }
    case OPS_STATUS_NO_REQUEST_PROTECT:
    {
        strcpy(pcStr, STR_DNR);
        break;
    }

    case OPS_STATUS_RELATIVE_SWITCH_WORK:
    {
        strcpy(pcStr, STR_RS);
        break;
    }
    case OPS_STATUS_RELATIVE_SWITCH_PROTECT:
    {
        strcpy(pcStr, STR_RS_P);
        break;
    }

    default:
    {
        LOG_WRITE(EN_LOG_ERROR, "unsupport fsm %d", iOpsStatus);
        strcpy(pcStr, STR_NR);
        /* return RETURN_UNEXECUTE; */
    }

    } /* end of switch */

    return OPLK_OK;

}

/**
 * @brief
 *
 * @note we ask this:
 *  /com-oplink-optical:optical-control/optical-modules
    /openconfig-transport-line-protection:aps/aps-modules
    /aps-module[name='OPS']/ports/line-primary-in/state/optical-power/instant
    but sysrepo do this
    /com-oplink-optical:optical-control/optical-modules
    /openconfig-transport-line-protection:aps/aps-modules
    /aps-module[name='OPS']/ports/line-primary-in/state
    and do this
    /com-oplink-optical:optical-control/optical-modules
    /openconfig-transport-line-protection:aps/aps-modules
    /aps-module[name='OPS']/ports/line-primary-in/state/optical-power

    so sr_xpath_node_name_eq(xpath_in, "optical-power") this, we do nothing,

 * @param [in]
 * @param [in]
 * @return
 */
int
OPS_OpsDataProviderCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath_in,
                      const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_data)
{
    int rc = SR_ERR_OK;
    int final = rc;
    /* vals to be set. */
    int i = 0;
    sr_val_t *pstVal = NULL;

    /* temp var */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acFsm[DEF_BUF_SIZE_32] = {0};
    char acCfgKey[DEF_BUF_SIZE_32] = {0};
    char acSwitchingType[DEF_BUF_SIZE_32] = {0};

    /* OPS status */
    double dPwr = 0.0;
    COpsStatusData stOpsStatus;

    /* Device id to be called by HAL, fixed to be 0 */
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;
    char (*pacNames)[DEF_BUF_SIZE_64] = NULL;
    char *pacKey = NULL;
    size_t nCount = 0, nActCount = 0, nIndex = 0;

    /* check input valid */
    if (NULL == xpath_in) {
        LOG_WRITE(EN_LOG_ERROR, "Input 0x%p", xpath_in);
        final = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /* Get all existing OLP component names */
    final = OPS_GetNames(&pacNames, &nCount);
    if (SR_ERR_OK != final) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetNames");
        goto cleanup;
    }

    /* Check user input the whole path or NOT:                                              */
    /* /openconfig-transport-line-protection:aps/aps-modules//.                             */
    /* /openconfig-transport-line-protection:aps/aps-modules/aps-module[name='OLP']//.      */
    memset(acCfgKey, 0x00, sizeof(acCfgKey));
    snprintf(acXpath, sizeof(acXpath), "%s", request_xpath);
    rc = DSC_GetOpsCfgKey(acXpath, acCfgKey);
    if (rc != SR_ERR_OK) {
        /* Using the whole path                                                             */
        nActCount = nCount;
    } else {
        /* Single path, and will check the key name                                         */
        for (nIndex = 0; nIndex < nCount; nIndex++) {
            if (0 == strcmp(acCfgKey, *(pacNames + nIndex))) {
                break;
            }
        }
        if (nIndex == nCount) {
            LOG_WRITE(EN_LOG_ERROR, "OLP key error: %s", acCfgKey);
            final = SR_ERR_NOT_FOUND;
            goto cleanup;
        }
        nActCount = 1;
    }

    for (nIndex = 0; nIndex < nActCount; nIndex++) {
        pacKey = (nActCount == nCount ? *(pacNames + nIndex) : acCfgKey);

        rc = OPS_GetOpsDevIdByKey(pacKey, &uiDevType, &uiPortId);
        if (SR_ERR_OK != rc) {
            final |= rc;
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey key : %s", pacKey);
            continue;
        }

        LOG_WRITE(EN_LOG_TRACE, "%s, key: %s, uiDevType: %u, uiPortId: %u", __func__, pacKey, uiDevType, uiPortId);

        memset(&stOpsStatus, 0, sizeof(stOpsStatus));
        if (OPLK_OK != OPS_GetStatusData(uiDevType, &stOpsStatus, uiPortId)) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetStatusData, uiDevType: %u, uiPortId: %u", uiDevType, uiPortId);
            final |= SR_ERR_OPERATION_FAILED;
            continue;
        }

        LOG_WRITE(EN_LOG_TRACE, "OPS_GetStatusData ucWorkMode:%hhu ucSwitchPos:%hhu ucFswStatus:%hhu ucSwitchingType:%hhu",
                  stOpsStatus.stCurData.ucWorkMode, stOpsStatus.stCurData.ucSwitchPos, stOpsStatus.stCurData.ucFswStatus, stOpsStatus.stCurData.ucSwitchingType);

        LOG_WRITE(EN_LOG_TRACE, "OPS_GetStatusData line1-in(primary-in):%hd line2-in(secondary-in):%hd",
                  stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE1_IN], stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE2_IN]);

        memset(acFsm, 0, sizeof(acFsm));
        rc = __OpsFsmEnumToStr(stOpsStatus.stCurData.ucFswStatus, acFsm);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OpsFsmEnumToStr %hhu", stOpsStatus.stCurData.ucFswStatus);
            rc = SR_ERR_OPERATION_FAILED;
            continue;
        }

        memset(acSwitchingType, 0, sizeof(acSwitchingType));
        rc = __OpsSwitchingTypeEnumToStr(stOpsStatus.stCurData.ucSwitchingType, acSwitchingType);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __OpsSwitchingTypeEnumToStr %hhu", stOpsStatus.stCurData.ucSwitchingType);
            rc = SR_ERR_OPERATION_FAILED;
            continue;
        }

        /* each ops malloc it's own values                       */
        final = sr_new_values(7, &pstVal);
        if (SR_ERR_OK != final) {
            LOG_WRITE(EN_LOG_ERROR, "sr_new_values: %s", sr_strerror(rc));
            goto cleanup;
        }

        i = 0;
        /* actual state data */
        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_ACTIVE_PATH, pacKey);
        /* Note: stOpsStatus.stCurData.ucSwitchPos, 1: primary, 0: secondary */
        __OPS_SET_VALUE_IDENTITYREF(acXpath, (1 == stOpsStatus.stCurData.ucSwitchPos) ? STR_PRIMARY : STR_SECONDARY);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_SWITCH_STATE, pacKey);
        LOG_WRITE(EN_LOG_DEBUG, "OPS_OpsDataProviderCb xpath:%s acFsm:%s", acXpath, acFsm);
        __OPS_SET_VALUE_ENUM(acXpath, acFsm);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_SWITCHING_TYPE, pacKey);
        LOG_WRITE(EN_LOG_DEBUG, "OPS_OpsDataProviderCb xpath:%s acSwitchingType:%s", acXpath, acSwitchingType);
        __OPS_SET_VALUE_ENUM(acXpath, acSwitchingType);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_PORT_POWER_INSTANT, pacKey, STR_LINE_PRIMARY_IN);
        dPwr = ((double)stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE1_IN]) / WEIGHT_HUNDREDTH;
        LOG_WRITE(EN_LOG_DEBUG, "OPS_OpsDataProviderCb xpath:%s primary-in:%lf", acXpath, dPwr);
        __OPS_SET_VALUE_DOUBLE(acXpath, dPwr);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_PORT_POWER_INSTANT, pacKey, STR_LINE_SECONDARY_IN);
        dPwr = ((double)stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE2_IN]) / WEIGHT_HUNDREDTH;
        LOG_WRITE(EN_LOG_DEBUG, "OPS_OpsDataProviderCb xpath:%s secondary-in:%lf", acXpath, dPwr);
        __OPS_SET_VALUE_DOUBLE(acXpath, dPwr);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_PORT_POWER_INSTANT, pacKey, STR_COMMON_IN);
        dPwr = ((double)stOpsStatus.stConnectionPds.asPd[OPS_R]) / WEIGHT_HUNDREDTH;
        __OPS_SET_VALUE_DOUBLE(acXpath, dPwr);

        snprintf(acXpath, OPLK_PATH_MAX_LEN, OPS_PORT_POWER_INSTANT, pacKey, STR_COMMON_OUTPUT);
        dPwr = ((double)stOpsStatus.stConnectionPds.asPd[OPS_T]) / WEIGHT_HUNDREDTH;
        __OPS_SET_VALUE_DOUBLE(acXpath, dPwr);

        rc = DSC_SrValues2LydNode(session, xpath_in, pstVal, 7, parent);

        final |= rc;

        if (pstVal != NULL) {
            sr_free_values(pstVal, 7);
            pstVal = NULL;
        }
    }

cleanup:

    if (pstVal != NULL) {
        sr_free_values(pstVal, 7);
        pstVal = NULL;
    }
    return final;
}

/**
 * @brief
 *
 * @note the input array like below, it is yang modle define
 *        1. leaf manual-to-port
 * @param [in]
 * @param [in]
 * @return
 */
int
OPS_RpcOpsCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
             sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_data)
{
    uint8_t ucMode = 0;
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;
    int rc = SR_ERR_OK;
    COpsStatusData stOpsStatus;
    char acUserLog[DEF_BUF_SIZE_64] = {0};

    sr_new_values(1, output);
    sr_val_set_xpath(&(*output)[0], RPC_XPATH_OPS_MESSAGE);
    *output_cnt = 1;

    /* Currently we put OPS name in message leaf for extended using. */
    if (input_cnt != 2) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_RpcOpsCb, input count %lu < 2", input_cnt);
        goto ERR_LABEL;
    }

    if (0 == strcmp(input[0].data.enum_val, STR_PRIMARY)) {
        ucMode = OPS_MAN_P;
    } else if (0 == strcmp(input[0].data.enum_val, STR_SECONDARY)) {
        ucMode = OPS_MAN_S;
    } else if (0 == strcmp(input[0].data.enum_val, STR_NONE)) {
        ucMode = OPS_CLEAR;
        /* goto OK_LABEL; */
    }

    rc = OPS_GetOpsDevIdByKey(input[1].data.string_val, &uiDevType, &uiPortId);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey key : %s", input[1].data.string_val);
        goto ERR_LABEL;
    }

    /* Not manual switch in force mode. */
    if (OPS_CLEAR == ucMode) {
        memset(&stOpsStatus, 0, sizeof(stOpsStatus));
        rc = OPS_GetStatusData(uiDevType, &stOpsStatus, uiPortId);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "ops get status failed, uiDevType: %u, uiPortId: %u", uiDevType, uiPortId);
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }

        if (OPS_FORCE_P == stOpsStatus.stCurData.ucWorkMode || OPS_FORCE_S == stOpsStatus.stCurData.ucWorkMode) {
            LOG_WRITE(EN_LOG_ERROR, "NOT allow to manual clear in force mode.");
            rc = SR_ERR_OPERATION_FAILED;
            goto ERR_LABEL;
        }
    }

    /* uiDevType == 0, TBD */
    LOG_WRITE(EN_LOG_NOTICE, "Set ops mode: %hu, device type: %u, port id: %u", ucMode, uiDevType, uiPortId);
    rc = OPS_SetMode(uiDevType, ucMode, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by setting ops mode:%hhd, rc:%d", ucMode, rc);
        goto ERR_LABEL;
    }

    /* OK_LABEL: */
    /* LOG_WRITE(EN_LOG_NOTICE, "Set ops ok. Manual to port: %s, ucMode", input[0].data.enum_val, ucMode); */
    snprintf(acUserLog, sizeof(acUserLog), "%s", input[0].data.enum_val);
    DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_MANUAL_TO_PORT, acUserLog);

    sr_val_set_str_data(&(*output)[0], SR_STRING_T, RPC_STATUS_SUCCESS);
    return SR_ERR_OK;

ERR_LABEL:
    sr_val_set_str_data(&(*output)[0], SR_STRING_T, RPC_STATUS_FAILED);
    return rc;
}

static int
__SetOpsModule(uint32_t uiDevType, uint32_t uiPortId, OPS_CONFIG_ST *pstOpsCfg, OPS_CFG_CHANGE_FLAG *pstCfgChangeFlag)
{
    int rc = SR_ERR_OK;
    /* COpsCfgData stHalOpsCfg; */
    uint32_t uiMode = 0;
    int16_t sThr = 0;
    int16_t sHys = 0;
    char acUserLog[DEF_BUF_SIZE_64] = {0};

    if (NULL == pstOpsCfg || NULL == pstCfgChangeFlag) {
        LOG_WRITE(EN_LOG_ERROR, "Input NULL");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }
    /*
       //stHalOpsCfg is not used, except for special check
       memset(&stHalOpsCfg, 0, sizeof(stHalOpsCfg));
       rc = OPS_GetCfgData(uiDevType, &stHalOpsCfg);
       if (SR_ERR_OK != rc) {
           LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetCfgData, uiDevType : %d", uiDevType);
           goto END_LABEL;
       }
     */
    if (pstCfgChangeFlag->bRevertive) {
        rc = OPS_SetRevertive(uiDevType, pstOpsCfg->bRevertive, uiPortId);
        if (rc != OPLK_OK) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set ops revertive: %d, uiDevType: %u, uiPortId: %u", \
                      pstOpsCfg->bRevertive, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetRevertive bRevertive:%d", pstOpsCfg->bRevertive); */

        snprintf(acUserLog, sizeof(acUserLog), "%s", pstOpsCfg->bRevertive ? "true" : "false");
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_REVERTIVE, acUserLog);
    }

    if (pstCfgChangeFlag->bWtr) {
        rc = OPS_SetWtr(uiDevType, pstOpsCfg->uiWtr / 1000, uiPortId); /* ms => s */
        if (rc != OPLK_OK) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set wtr %u, uiDevType: %u, uiPortId: %u", pstOpsCfg->uiWtr, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetWtr uiWtr:%d", pstOpsCfg->uiWtr); */
        snprintf(acUserLog, sizeof(acUserLog), "%d", pstOpsCfg->uiWtr);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_WTR, acUserLog);
    }

    if (pstCfgChangeFlag->bHoldOffTime) {
        rc = OPS_SetHoldOffTime(uiDevType, pstOpsCfg->uiHoldOffTime, uiPortId); /* Unit: us for OPS module */
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set hold off time: %hd, uiDevType: %u, uiPortId: %u", \
                      pstOpsCfg->uiHoldOffTime, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetHoldOffTime uiHoldOffTime:%d", pstOpsCfg->uiHoldOffTime); */
        snprintf(acUserLog, sizeof(acUserLog), "%d", pstOpsCfg->uiHoldOffTime);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_HOLD_OFF_TIME, acUserLog);
    }

    if (pstCfgChangeFlag->bPrimaryThr) {
        sThr = ROUND_MUTI_100(pstOpsCfg->dPrimarySwitchThr);    /* Unit: 0.01dBm for OPS module */
        rc = OPS_SetPortPowerThr(uiDevType, OPS_PORT_LINE1_IN, OPS_THR_TYPE_LOS, sThr, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set primary thr: %hd, uiDevType: %u, uiPortId: %u", sThr, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetPortPowerThr dPrimarySwitchThr:%.2f", pstOpsCfg->dPrimarySwitchThr); */
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dPrimarySwitchThr);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_PRIMARY_THR, acUserLog);
    }

    if (pstCfgChangeFlag->bPrimaryHys) {
        sHys = ROUND_MUTI_100(pstOpsCfg->dPrimaryHys);
        rc = OPS_SetPortPowerHys(uiDevType, OPS_PORT_LINE1_IN, OPS_THR_TYPE_LOS, sHys, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set primary hys: %hd, uiDevType: %u, uiPortId: %u", sHys, uiDevType, uiPortId);
            goto END_LABEL;
        }
        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetPortPowerHys dPrimaryHys:%.2f", pstOpsCfg->dPrimaryHys); */
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dPrimaryHys);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_PRIMARY_HYS, acUserLog);
#if 0
        /* for meiwu, secondary hys is 0*/
        rc = OPS_SetSwitchHys(uiDevType, 1, sHys);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "SetOps%d secondary hys: %hd", uiDevType, sHys);
            goto END_LABEL;
        }
        LOG_WRITE(EN_LOG_NOTICE, "OPS_SetSwitchHys dSecondaryHys:%.2f", pstOpsCfg->dPrimaryHys);
#endif
    }

    if (pstCfgChangeFlag->bSecondaryThr) {
        sThr = ROUND_MUTI_100(pstOpsCfg->dSecondarySwitchThr); /* Unit: 0.01dBm for OPS module */
        rc = OPS_SetPortPowerThr(uiDevType, OPS_PORT_LINE2_IN, OPS_THR_TYPE_LOS, sThr, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set seconday thr: %hd, uiDevType: %u, uiPortId: %u", sThr, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetPortPowerThr dSecondarySwitchThr:%.2f", pstOpsCfg->dSecondarySwitchThr); */
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dSecondarySwitchThr);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_SECONDARY_THR, acUserLog);
    }

    if (pstCfgChangeFlag->bSecondaryHys) {
        sHys = ROUND_MUTI_100(pstOpsCfg->dSecondaryHys);
        rc = OPS_SetPortPowerHys(uiDevType, OPS_PORT_LINE2_IN, OPS_THR_TYPE_LOS, sHys, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set secondary hys: %hd, uiDevType: %u, uiPortId: %u", sHys, uiDevType, uiPortId);
            goto END_LABEL;
        }

        memset(&acUserLog, 0, sizeof(acUserLog));
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dSecondaryHys);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_SECONDARY_HYS, acUserLog);
    }

    if (pstCfgChangeFlag->bRelativeThr) {
        sThr = ROUND_MUTI_100(pstOpsCfg->dRelativeThr);
        rc = OPS_SetRelativeThr(uiDevType, sThr, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set relative thr: %hd, uiDevType: %u, uiPortId: %u", sThr, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetRelativeThr dRelativeThr:%.2f", pstOpsCfg->dRelativeThr); */
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dRelativeThr);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_RELATIVE_THR, acUserLog);
    }

    if (pstCfgChangeFlag->bRelativeOffset) {
        sHys = ROUND_MUTI_100(pstOpsCfg->dRelativeThrOffset);

        rc = OPS_SetRelativeOffset(uiDevType, sHys, uiPortId);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Error by set relative offset: %hd, uiDevType: %u, uiPortId: %u", sHys, uiDevType, uiPortId);
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetRelativeOffset dRelativeThrOffset:%.2f", pstOpsCfg->dRelativeThrOffset); */
        snprintf(acUserLog, sizeof(acUserLog), "%.2f", pstOpsCfg->dRelativeThrOffset);
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_RELATIVE_OFFSET, acUserLog);
    }

    if (pstCfgChangeFlag->bForceToPort) {
        switch (pstOpsCfg->enForceToPort) {
        case SWITCH_TO_NONE:
            uiMode = OPS_CLEAR;
            snprintf(acUserLog, sizeof(acUserLog), "%s", STR_NONE);
            break;

        case SWITCH_TO_PRIMARY:
            uiMode = OPS_FORCE_P;
            snprintf(acUserLog, sizeof(acUserLog), "%s", STR_PRIMARY);
            break;

        case SWITCH_TO_SECONDARY:
            uiMode = OPS_FORCE_S;
            snprintf(acUserLog, sizeof(acUserLog), "%s", STR_SECONDARY);
            break;

        default:
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "Invalid enForceToPort: %d", pstOpsCfg->enForceToPort);
            goto END_LABEL;
        }

        LOG_WRITE(EN_LOG_NOTICE, "set mode:%u, uiDevType: %u, uiPortId: %u", uiMode, uiDevType, uiPortId);
        rc = OPS_SetMode(uiDevType, uiMode, uiPortId);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by set ops mode:%u rc:%d", uiMode, rc);
            rc = SR_ERR_OPERATION_FAILED;
            goto END_LABEL;
        }

        /* LOG_WRITE(EN_LOG_NOTICE, "OPS_SetMode uiMode:%d", uiMode); */
        DSC_WriteUserLog(COMPONENT_KEY_OPS, STR_ID_SET_FORCE_TO_PORT, acUserLog);

        /* mod led */
#if 0
        if (OPS_FORCE_P == uiMode || OPS_FORCE_S == uiMode) {
            BOARD_SetLedState(LED5, LED_STATUS_RED);
        } else {
            BOARD_SetLedState(LED5, LED_STATUS_GREEN);
        }
#endif
    }

END_LABEL:
    return rc;
}


/**
 * @brief
 *
 * @note  noppop only have one ops, so idx fixed be one
 * @param [in]
 * @param [in]
 * @return
 */
int
OPS_OpsChangeCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath, sr_event_t event, uint32_t request_id, void *private_ctx)
{
    sr_change_iter_t *pstIter = NULL;
    int rc = SR_ERR_OK;
    sr_change_oper_t enOper;
    sr_val_t *pstOldValue = NULL;
    sr_val_t *pstNewValue = NULL;
    bool bConfEnable = true;
    char acCfgKey[32] = {0};
    OPS_RANGE_ST stOpsRange;
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT, uiItemIndex = 0;
    OPS_CFG_CHANGE_FLAG astOpsCfgChangeFlag[OPS_DEV_NUM_MAX];
    OPS_CONFIG_ST astOpsCfg[OPS_DEV_NUM_MAX];
    uint32_t auiDevType[OPS_DEV_NUM_MAX];
    uint32_t auiPortId[OPS_DEV_NUM_MAX];
    bool abFlag[OPS_DEV_NUM_MAX];

    LOG_WRITE(EN_LOG_NOTICE, "OPS Config Changes, event: %s", ev_to_str(event));

    /* SR_EV_CHANGE:                                                            */
    /*   Occurs just before the changes are committed to the datastore          */
    /* SR_EV_ABORT:                                                             */
    /*   Occurs in case that the commit transaction has failed because          */
    /*   one of the verifiershas denied the change (returned an error)          */
    /* SR_EV_DONE:                                                              */
    /*   Occurs just after the changes have been successfully committed to DB   */
    switch (event) {
    case SR_EV_CHANGE:
    case SR_EV_ABORT:
        break;
    case SR_EV_DONE:
        rc = DSC_UpdateOpenconfigState(session, OPS_XPATH_HEADER "//.");
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Update openconfig failed for xpath %s", OPS_XPATH_HEADER);
        }
        return rc;
    default:
        return SR_ERR_OK;
    }

    rc = sr_get_changes_iter(session, OPS_XPATH_HEADER "//.", &pstIter);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get changes iter failed for xpath %s", OPS_XPATH_HEADER);
        goto cleanup;
    }


    memset(&stOpsRange, 0, sizeof(stOpsRange));
    rc = OPS_OpsGetParaRange(&stOpsRange);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "OPS_GetParaRange fail");
        goto cleanup;
    }

    memset(astOpsCfgChangeFlag, 0, sizeof(astOpsCfgChangeFlag));
    memset(astOpsCfg, 0, sizeof(astOpsCfg));
    memset(auiDevType, 0, sizeof(auiDevType));
    memset(auiPortId, 0, sizeof(auiPortId));
    memset(abFlag, 0, sizeof(abFlag));

    while (SR_ERR_OK == sr_get_change_next(session, pstIter,
                                           &enOper, &pstOldValue, &pstNewValue)) {

        /* assume only add or modify, so we always use the new value */
        if (NULL == pstNewValue) {
            if (pstOldValue != NULL) {
                LOG_WRITE(EN_LOG_ERROR, "xpath: %s", pstOldValue->xpath);
            }
            LOG_WRITE(EN_LOG_ERROR, "Get ops changes iter failed for xpath %s", OPS_XPATH_HEADER);
            goto cleanup;
        }

        /* Decide whether OPS can be configured according to admin state */
        rc = DSC_GetConfEnable(OPTICAL_OPS_EN, pstNewValue->xpath, &bConfEnable);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetConfEnable, xpath : %s", pstNewValue->xpath);
        }

        if (false == bConfEnable) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_NOTICE, "ops admin state is oos, can not conf!");
            goto cleanup;
        }

        memset(acCfgKey, 0, sizeof(acCfgKey));
        rc = DSC_GetOpsCfgKey(pstNewValue->xpath, acCfgKey);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsCfgKey, xpath: %s", pstNewValue->xpath);
            goto cleanup;
        }

        rc = OPS_GetOpsDevIdByKey(acCfgKey, &uiDevType, &uiPortId);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey key: %s", acCfgKey);
            goto cleanup;
        }

        uiItemIndex = OPS_GetNumber(uiDevType, uiPortId) - OPS_SUB_INDEX_DEFAULT;
        if (uiItemIndex < 0 || uiItemIndex >= OPS_DEV_NUM_MAX) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid OPS number: %u, should be [1, %d]", uiItemIndex + OPS_SUB_INDEX_DEFAULT, OPS_DEV_NUM_MAX);
            rc = SR_ERR_INVAL_ARG;
            goto cleanup;
        }

        abFlag[uiItemIndex] = true;
        auiDevType[uiItemIndex] = uiDevType;
        auiPortId[uiItemIndex] = uiPortId;

        /* LOG_WRITE(EN_LOG_NOTICE, "### %s", pstNewValue->xpath); */
        if (sr_xpath_node_name_eq(pstNewValue->xpath, "revertive")) {
            astOpsCfgChangeFlag[uiItemIndex].bRevertive = true;

            astOpsCfg[uiItemIndex].bRevertive = pstNewValue->data.bool_val;

        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "wait-to-restore-time")) {
            astOpsCfgChangeFlag[uiItemIndex].bWtr = true;

            astOpsCfg[uiItemIndex].uiWtr = pstNewValue->data.uint32_val;

            if (astOpsCfg[uiItemIndex].uiWtr < stOpsRange.uiWtrMin || astOpsCfg[uiItemIndex].uiWtr > stOpsRange.uiWtrMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d wtr %d, range: [%d, %d]",
                          uiDevType, astOpsCfg[uiItemIndex].uiWtr, stOpsRange.uiWtrMin, stOpsRange.uiWtrMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "primary-switch-threshold")) {
            astOpsCfgChangeFlag[uiItemIndex].bPrimaryThr = true;

            astOpsCfg[uiItemIndex].dPrimarySwitchThr = pstNewValue->data.decimal64_val;
            if (astOpsCfg[uiItemIndex].dPrimarySwitchThr < stOpsRange.dPrimaryThrMin || astOpsCfg[uiItemIndex].dPrimarySwitchThr > stOpsRange.dPrimaryThrMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d primary thr: %.2f, range: [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dPrimarySwitchThr, stOpsRange.dPrimaryThrMin, stOpsRange.dPrimaryThrMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "primary-switch-hysteresis")) {

            astOpsCfgChangeFlag[uiItemIndex].bPrimaryHys = true;

            astOpsCfg[uiItemIndex].dPrimaryHys = pstNewValue->data.decimal64_val;

            if (astOpsCfg[uiItemIndex].dPrimaryHys < stOpsRange.dPrimaryHysMin || astOpsCfg[uiItemIndex].dPrimaryHys > stOpsRange.dPrimaryHysMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d primary Hys: %.2f, range: [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dPrimaryHys, stOpsRange.dPrimaryHysMin, stOpsRange.dPrimaryHysMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "secondary-switch-threshold")) {
            astOpsCfgChangeFlag[uiItemIndex].bSecondaryThr = true;

            astOpsCfg[uiItemIndex].dSecondarySwitchThr = pstNewValue->data.decimal64_val;

            if (astOpsCfg[uiItemIndex].dSecondarySwitchThr < stOpsRange.dSecondaryThrMin || astOpsCfg[uiItemIndex].dSecondarySwitchThr > stOpsRange.dSecondaryThrMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d secondary thr: %.2f, range: [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dSecondarySwitchThr, stOpsRange.dSecondaryThrMin, stOpsRange.dSecondaryThrMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "secondary-switch-hysteresis")) {
            astOpsCfgChangeFlag[uiItemIndex].bSecondaryHys = true;

            astOpsCfg[uiItemIndex].dSecondaryHys = pstNewValue->data.decimal64_val;

            if (astOpsCfg[uiItemIndex].dSecondaryHys < stOpsRange.dSecondaryHysMin || astOpsCfg[uiItemIndex].dSecondaryHys > stOpsRange.dSecondaryHysMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d secondary Hys: %.2f, range: [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dSecondaryHys, stOpsRange.dSecondaryHysMin, stOpsRange.dSecondaryHysMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "hold-off-time")) {
            astOpsCfgChangeFlag[uiItemIndex].bHoldOffTime = true;

            astOpsCfg[uiItemIndex].uiHoldOffTime = pstNewValue->data.uint32_val;

            if (astOpsCfg[uiItemIndex].uiHoldOffTime < stOpsRange.uiHoldOffMin || astOpsCfg[uiItemIndex].uiHoldOffTime > stOpsRange.uiHoldOffMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "SetOps%d uiHoldOff %d, range:%d %d",
                          uiDevType, astOpsCfg[uiItemIndex].uiHoldOffTime, stOpsRange.uiHoldOffMin, stOpsRange.uiHoldOffMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "relative-switch-threshold")) {
            astOpsCfgChangeFlag[uiItemIndex].bRelativeThr = true;
            astOpsCfg[uiItemIndex].dRelativeThr = pstNewValue->data.decimal64_val;
            if (UTIL_IsDoubleEqual(astOpsCfg[uiItemIndex].dRelativeThr, 0)) {
                /* absolute threshold mode */
            } else if (astOpsCfg[uiItemIndex].dRelativeThr < stOpsRange.dRelativeThrMin || astOpsCfg[uiItemIndex].dRelativeThr > stOpsRange.dRelativeThrMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d relative thr: %.2f, range: 0 or [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dRelativeThr, stOpsRange.dRelativeThrMin, stOpsRange.dRelativeThrMax);
                goto cleanup;
            }

        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "relative-switch-threshold-offset")) {
            astOpsCfgChangeFlag[uiItemIndex].bRelativeOffset = true;
            astOpsCfg[uiItemIndex].dRelativeThrOffset = pstNewValue->data.decimal64_val;

            if (astOpsCfg[uiItemIndex].dRelativeThrOffset < stOpsRange.dRelativeOffsetMin || astOpsCfg[uiItemIndex].dRelativeThrOffset > stOpsRange.dRelativeOffsetMax) {
                rc = SR_ERR_INVAL_ARG;
                LOG_WRITE(EN_LOG_ERROR, "ChkOps%d relative offset: %.2f, range: [%.2f, %.2f]",
                          uiDevType, astOpsCfg[uiItemIndex].dRelativeThrOffset, stOpsRange.dRelativeOffsetMin, stOpsRange.dRelativeOffsetMax);
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "force-to-port")) {
            astOpsCfgChangeFlag[uiItemIndex].bForceToPort = true;

            astOpsCfg[uiItemIndex].enForceToPort = DSC_OpsSwitchPortStrToEnum(pstNewValue->data.enum_val);
            if (SWITCH_TO_PORT_MAX == astOpsCfg[uiItemIndex].enForceToPort) {
                LOG_WRITE(EN_LOG_ERROR, "Error by force-to-port: %s", pstNewValue->data.enum_val);
                rc = SR_ERR_INVAL_ARG;
                goto cleanup;
            }
        } else if (sr_xpath_node_name_eq(pstNewValue->xpath, "switching-type")) {
            LOG_WRITE(EN_LOG_ERROR, "not support %s", pstNewValue->xpath);
            rc = SR_ERR_UNSUPPORTED;
            goto cleanup;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "not support %s", pstNewValue->xpath);
            rc = SR_ERR_INVAL_ARG;
            goto cleanup;
        }

        SR_SAFE_FREE_VAL(pstOldValue);
        SR_SAFE_FREE_VAL(pstNewValue);
    }

    for (int i = 0; i < OPS_DEV_NUM_MAX; i++) {
        if (!abFlag[i]) {
            continue;
        }
        rc |= __SetOpsModule(auiDevType[i], auiPortId[i], astOpsCfg + i, astOpsCfgChangeFlag + i);
    }

cleanup:
    SR_SAFE_FREE_VAL(pstOldValue);
    SR_SAFE_FREE_VAL(pstNewValue);
    sr_free_change_iter(pstIter);
    LOG_WRITE(EN_LOG_TRACE, "Set ops, rc: %d", rc);
    return rc;
}

static int
__OpsProcessTca(int iId, int16_t psTcaThr[][2], bool pbFlag[][2])
{
    int rc = SR_ERR_OK;
    int iVid = TCA_IDX_OPS_START + (iId - TCA_IDX_OPS_START) % (OPS_TCA_SIZE / (OPS_DEV_NUM_MAX + 1));
    OPS_RANGE_ST stOpsRange;
    char *pcTcaName = NULL;
    char *pcTemp = NULL;
    char acCfgKey[DEF_BUF_SIZE_64];
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t nCount = 0;
    double dblHighThr = 0.0, dblLowThr = 0.0;
    int16_t sHighThr = 0, sLowThr = 0, sThr = 0;
    double dblCmpHighThr = 0.0, dblCmpLowThr = 0.0;
    int16_t sCmpHighThr = 0, sCmpLowThr = 0;
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;

    memset(&stOpsRange, 0, sizeof(stOpsRange));
    rc = OPS_OpsGetParaRange(&stOpsRange);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "OPS_GetParaRange fail");
        return rc;
    }

    pcTcaName = DSC_GetTcaName(iId);
    if (NULL == pcTcaName) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_GetTcaName id: %d", iId);
        return SR_ERR_INVAL_ARG;
    }

    pcTemp = strchr(pcTcaName, '_');
    if (NULL == pcTemp || NULL == strstr(pcTcaName, OPS_CFG_KEY)) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid TCA name: %s, id: %d", pcTcaName, iId);
        return SR_ERR_INVAL_ARG;
    }

    memset(acCfgKey, 0, sizeof(acCfgKey));
    memcpy(acCfgKey, pcTcaName, pcTemp - pcTcaName);

    rc = OPS_GetOpsDevIdByKey(acCfgKey, &uiDevType, &uiPortId);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey, invalid ops key: %s", acCfgKey);
        return rc;
    }

    /* LOW and HIGH TCA need to check each other, DSC_GetTcaList is essential in this case */
    if ((iVid >= TCA_IDX_OPS_COM_IN_LOW && iVid <= TCA_IDX_OPS_SEC_IN_HIGH) ||
        (0 == pbFlag[iId - TCA_IDX_OPS_START][0] || 0 == pbFlag[iId - TCA_IDX_OPS_START][1])) {
        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &nCount);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
            return rc;
        }
    }

    if (pbFlag[iId - TCA_IDX_OPS_START][0] && pbFlag[iId - TCA_IDX_OPS_START][1]) {
        sHighThr = psTcaThr[iId - TCA_IDX_OPS_START][0];
        sLowThr = psTcaThr[iId - TCA_IDX_OPS_START][1];
    } else {
        rc = DSC_GetTcaThresh(iId, pstTcaInfo, &dblHighThr, &dblLowThr);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get tca threshold error, tca id: %d", iId);
        }

        if (pbFlag[iId - TCA_IDX_OPS_START][0]) {
            sHighThr = psTcaThr[iId - TCA_IDX_OPS_START][0];
            sLowThr = (int16_t)ROUND_MUTI_100(dblLowThr);
        } else {
            sHighThr = (int16_t)ROUND_MUTI_100(dblHighThr);
            sLowThr = psTcaThr[iId - TCA_IDX_OPS_START][1];
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "key: %s, id: %d, name: %s, vid: %d, high: %d, low: %d", acCfgKey, iId, pcTcaName, iVid, sHighThr, sLowThr);

    switch (iVid) {
    case TCA_IDX_OPS_COM_IN_LOS:
        CHECK_TCA_RANGE(OPS_THR_TYPE_LOS, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dComRLosThrMax), ROUND_MUTI_100(stOpsRange.dComRLosThrMin), \
                        ROUND_MUTI_100(stOpsRange.dComRLosHysMax), ROUND_MUTI_100(stOpsRange.dComRLosHysMin));
        rc = OPS_SetConnectionPdThr(uiDevType, OPS_R, OPS_THR_TYPE_LOS, sLowThr, uiPortId);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetConnectionPdThr, tca name:%s, thr: %d, component: %s, rc:%d", \
                      pcTcaName, sLowThr, acCfgKey, rc);
            return rc;
        }
        rc = OPS_SetConnectionPdHys(uiDevType, OPS_R, OPS_THR_TYPE_LOS, sHighThr - sLowThr, uiPortId);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetConnectionPdHys, tca name:%s, hys: %d, component: %s, rc:%d", \
                      pcTcaName, sHighThr - sLowThr, acCfgKey, rc);
            return rc;
        }
        break;

    case TCA_IDX_OPS_POWER_DIFFER:
        CHECK_TCA_RANGE(OPS_THR_TYPE_DIFFER, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dDifferThrMax), ROUND_MUTI_100(stOpsRange.dDifferThrMin), \
                        ROUND_MUTI_100(stOpsRange.dDifferHysMax), ROUND_MUTI_100(stOpsRange.dDifferHysMin));
        rc = OPS_SetThrPwrDiff(uiDevType, sHighThr, sHighThr - sLowThr, uiPortId);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetThrPwrDiff, tca name:%s, thr: %d, hys: %d, component: %s, rc:%d", \
                      pcTcaName, sHighThr, sHighThr - sLowThr, acCfgKey, rc);
            return rc;
        }
        break;

#define __high_thr(id, high) (true == pbFlag[id - TCA_IDX_OPS_START][0] ? psTcaThr[id - TCA_IDX_OPS_START][0] : high)
#define __low_thr(id, low)  (true == pbFlag[id - TCA_IDX_OPS_START][1] ? psTcaThr[id - TCA_IDX_OPS_START][1] : low)

    case TCA_IDX_OPS_COM_IN_LOW:
        CHECK_TCA_RANGE(OPS_THR_TYPE_LOW, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dComRLowThrMax), ROUND_MUTI_100(stOpsRange.dComRLowThrMin), \
                        ROUND_MUTI_100(stOpsRange.dComRLowHysMax), ROUND_MUTI_100(stOpsRange.dComRLowHysMin));

        rc = DSC_GetTcaThresh(iId + 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sLowThr, sHighThr, sCmpLowThr, sCmpHighThr);
        }
        break;

    case TCA_IDX_OPS_COM_IN_HIGH:
        CHECK_TCA_RANGE(OPS_THR_TYPE_HIGH, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dComRHighThrMax), ROUND_MUTI_100(stOpsRange.dComRHighThrMin), \
                        ROUND_MUTI_100(stOpsRange.dComRHighHysMax), ROUND_MUTI_100(stOpsRange.dComRHighHysMin));

        rc = DSC_GetTcaThresh(iId - 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sCmpLowThr, sCmpHighThr, sLowThr, sHighThr);
        }
        break;

    case TCA_IDX_OPS_PRI_IN_LOW:
        CHECK_TCA_RANGE(OPS_THR_TYPE_LOW, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dPriRLowThrMax), ROUND_MUTI_100(stOpsRange.dPriRLowThrMin), \
                        ROUND_MUTI_100(stOpsRange.dPriRLowHysMax), ROUND_MUTI_100(stOpsRange.dPriRLowHysMin));

        rc = DSC_GetTcaThresh(iId + 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sLowThr, sHighThr, sCmpLowThr, sCmpHighThr);
        }
        break;

    case TCA_IDX_OPS_PRI_IN_HIGH:
        CHECK_TCA_RANGE(OPS_THR_TYPE_HIGH, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dPriRHighThrMax), ROUND_MUTI_100(stOpsRange.dPriRHighThrMin), \
                        ROUND_MUTI_100(stOpsRange.dPriRHighHysMax), ROUND_MUTI_100(stOpsRange.dPriRHighHysMin));

        rc = DSC_GetTcaThresh(iId - 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sCmpLowThr, sCmpHighThr, sLowThr, sHighThr);
        }
        break;

    case TCA_IDX_OPS_SEC_IN_LOW:
        CHECK_TCA_RANGE(OPS_THR_TYPE_LOW, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dSecRLowThrMax), ROUND_MUTI_100(stOpsRange.dSecRLowThrMin), \
                        ROUND_MUTI_100(stOpsRange.dSecRLowHysMax), ROUND_MUTI_100(stOpsRange.dSecRLowHysMin));

        rc = DSC_GetTcaThresh(iId + 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId + 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sLowThr, sHighThr, sCmpLowThr, sCmpHighThr);
        }
        break;

    case TCA_IDX_OPS_SEC_IN_HIGH:
        CHECK_TCA_RANGE(OPS_THR_TYPE_HIGH, sHighThr, sLowThr, ROUND_MUTI_100(stOpsRange.dSecRHighThrMax), ROUND_MUTI_100(stOpsRange.dSecRHighThrMin), \
                        ROUND_MUTI_100(stOpsRange.dSecRHighHysMax), ROUND_MUTI_100(stOpsRange.dSecRHighHysMin));

        rc = DSC_GetTcaThresh(iId - 1, pstTcaInfo, &dblCmpHighThr, &dblCmpLowThr);
        if (SR_ERR_OK == rc) {
            sCmpLowThr = __low_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpLowThr));
            sCmpHighThr = __high_thr(iId - 1, (int16_t)ROUND_MUTI_100(dblCmpHighThr));
            CHECK_TCA_INTERSECTION(sCmpLowThr, sCmpHighThr, sLowThr, sHighThr);
        }
        break;

#undef __high_thr
#undef __low_thr

    default:
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Not support the tca id: %d, ops key: %s", iId, acCfgKey);
        break;
    }

    return rc;
}

static int
__OpsGetNextChanges(sr_session_ctx_t *session, sr_change_iter_t *iter, sr_change_oper_t *operation, sr_val_t **old_value, sr_val_t **new_value)
{
    SR_SAFE_FREE_VAL(*old_value);
    SR_SAFE_FREE_VAL(*new_value);
    return sr_get_change_next(session, iter, operation, old_value, new_value);
}

int
OPS_TcaChangeCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath, sr_event_t event, uint32_t request_id, void *private_data)
{
    sr_change_iter_t *it = NULL;
    int rc = SR_ERR_OK;
    int iId = 0, i = 0;
    sr_change_oper_t oper;
    sr_val_t *pstOldValue = NULL;
    sr_val_t *pstNewValue = NULL;
    /* 0-high, 1-low                                                            */
    int16_t asTca[OPS_TCA_SIZE][2] = {{0, 0}};
    bool abFlag[OPS_TCA_SIZE][2] = {{false, false}};

    LOG_WRITE(EN_LOG_NOTICE, "%s, tca changes, event: %s", __func__, ev_to_str(event));

    /* SR_EV_CHANGE:                                                            */
    /*   Occurs just before the changes are committed to the datastore          */
    /* SR_EV_ABORT:                                                             */
    /*   Occurs in case that the commit transaction has failed because          */
    /*   one of the verifiershas denied the change (returned an error)          */
    /* SR_EV_DONE:                                                              */
    /*   Occurs just after the changes have been successfully committed to DB   */
    switch (event) {
    case SR_EV_CHANGE:
    case SR_EV_ABORT:
        break;
    case SR_EV_DONE:
        return rc;
    default:
        return SR_ERR_OK;
    }

    rc = sr_get_changes_iter(session, TCA_XPATH_ALL, &it);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get changes iter failed for xpath %s", TCA_XPATH_HEADER);
        goto cleanup;
    }

    while (SR_ERR_OK == __OpsGetNextChanges(session, it, &oper, &pstOldValue, &pstNewValue)) {
        /* Only modify, so we always use the new value                         */
        if (NULL == pstNewValue || NULL == pstOldValue) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Get tca changes iter failed for xpath %s", TCA_XPATH_HEADER);
            goto cleanup;
        }

        iId = DSC_GetTcaIdx(pstNewValue->xpath);
        if (iId < 0 || iId >= TCA_IDX_MAX) {
            rc = SR_ERR_INVAL_ARG;
            LOG_WRITE(EN_LOG_ERROR, "Error xpath: %s, id %d", pstNewValue->xpath, iId);
            goto cleanup;
        }

        /* Skip the non-ops tca items, they will be processed in csmd.          */
        if (iId < TCA_IDX_OPS_START || iId > TCA_IDX_OPS_END) {
            LOG_WRITE(EN_LOG_NOTICE, "%s, skip non ops tca items, id: %d", __func__, iId);
            continue;
        }

        i = iId - TCA_IDX_OPS_START;
        if (NULL != strstr(pstNewValue->xpath, TCA_HIGH_VALUE_LEAF)) {
            asTca[i][0] = ROUND_MUTI_100(pstNewValue->data.decimal64_val);
            abFlag[i][0] = true;
            LOG_WRITE(EN_LOG_NOTICE, "%d, TCA_HIGH_VALUE_LEAF: %d", iId, asTca[i][0]);
        } else if (NULL != strstr(pstNewValue->xpath, TCA_LOW_VALUE_LEAF)) {
            asTca[i][1] = ROUND_MUTI_100(pstNewValue->data.decimal64_val);
            abFlag[i][1] = true;
            LOG_WRITE(EN_LOG_NOTICE, "%d, TCA_LOW_VALUE_LEAF: %d", iId, asTca[i][1]);
        } else {
            LOG_WRITE(EN_LOG_ERROR, "%d, not support %s", iId, pstNewValue->xpath);
            continue;
        }
    }

    for (i = 0; i < OPS_TCA_SIZE; i++) {
        if (abFlag[i][0] || abFlag[i][1]) {
            rc = __OpsProcessTca(i + TCA_IDX_OPS_START, asTca, abFlag);
            if (SR_ERR_OK != rc) {
                goto cleanup;
            }
        }
    }

cleanup:
    SR_SAFE_FREE_VAL(pstOldValue);
    SR_SAFE_FREE_VAL(pstNewValue);
    sr_free_change_iter(it);
    return rc;
}

static void
__SetOpsLeds(COpsStatusData *pstOpsStatus)
{
#if 0
    uint8_t ucOpsPos = OPS_PORT_LINE_IN_CNT, ucOpsCurrentPos = OPS_PORT_LINE_IN_CNT;
    uint32_t uiLedIdx = LED1;

    /* mod led */
    if (OPS_FORCE_P == pstOpsStatus->stCurData.ucWorkMode || OPS_FORCE_S == pstOpsStatus->stCurData.ucWorkMode) {
        BOARD_SetLedState(LED5, LED_STATUS_RED);
    } else {
        BOARD_SetLedState(LED5, LED_STATUS_GREEN);
    }

    /* pri/sec led */
    for (ucOpsPos = OPS_PORT_LINE1_IN; ucOpsPos < OPS_PORT_LINE_IN_CNT; ucOpsPos++) {

        ucOpsCurrentPos = (1 == pstOpsStatus->stCurData.ucSwitchPos) ? OPS_PORT_LINE1_IN :  OPS_PORT_LINE2_IN;
        uiLedIdx = (OPS_PORT_LINE1_IN == ucOpsPos) ? LED1 : LED4;

        if (pstOpsStatus->stAlarm.abSwitchAlarm[ucOpsPos]) {
            /* input power is los */
            if (ucOpsCurrentPos == ucOpsPos) {
                BOARD_SetLedState(uiLedIdx, LED_STATUS_RED_BLINK);

            } else {
                BOARD_SetLedState(uiLedIdx, LED_STATUS_RED);
            }
        } else {
            /* input power is ok */
            if (ucOpsCurrentPos == ucOpsPos) {
                BOARD_SetLedState(uiLedIdx, LED_STATUS_GREEN_BLINK);
            } else {
                BOARD_SetLedState(uiLedIdx, LED_STATUS_GREEN);
            }

        }
    }
#endif
    return;
}

/* EQA-98 After csm loop the alarm from OA, before the CSM had create the event. If th OA alarm   */
/*        changes, csm won't receive the alarm. So we need to loop again after creating event     */
/*        In this case, we need to add a mutex to protect.                                        */
/* @ {
 */
static pthread_mutex_t m_stOpsEventLock = PTHREAD_MUTEX_INITIALIZER;
/* }@                                                                                             */


int
OPS_OpsDevEventCb(int iEventId, int iSubEventId)
{
    int rc = 0;
    COpsStatusData stOpsStatus;
    OPS_CONFIG_ST cfg;
    COpsCommStatusData stOpsCommStatus;
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT, uiItemIndex = 0;
    int i = 0, m = 0, n = 0;
    char (*pacNames)[DEF_BUF_SIZE_64] = NULL;
    char *pacKey = NULL;
    size_t nCount = 0;

    /* It's NOT a nice code, just for solve the ops-4 issue quickly */
    int iOps4IdxStart = 0;
    int iOps4IdxEnd = 0;
    int iOffset = 0;
    COpsStatusData stOps4Status;

    LOG_WRITE(EN_LOG_DEBUG, "++ OPS_OpsDevEventCb, iEventId=%d, payload=%d", iEventId, iSubEventId);

    pthread_mutex_lock(&m_stOpsEventLock);

    /*For bi-directional ASP, send remote APS message when the switch status changed. */
    if (iEventId == DEF_EVENT_ID_OPS_STATE_CHANGE) {
        OPS_SendAPSRapidMessage(iSubEventId);
        pthread_mutex_unlock(&m_stOpsEventLock);
        return rc;
    }

    /* Get all existing OLP component names */
    rc = OPS_GetNames(&pacNames, &nCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetNames");
        pthread_mutex_unlock(&m_stOpsEventLock);
        return rc;
    }

    OPS_ID_UNPACK(iSubEventId, uiDevType, uiPortId);
    uiItemIndex = OPS_GetNumber(uiDevType, uiPortId) - OPS_SUB_INDEX_DEFAULT;
    if (uiItemIndex < 0 || uiItemIndex >= OPS_DEV_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid OPS number: %u, should be [1, %d]", uiItemIndex + OPS_SUB_INDEX_DEFAULT, OPS_DEV_NUM_MAX);
        rc = SR_ERR_INVAL_ARG;
        pthread_mutex_unlock(&m_stOpsEventLock);
        return rc;
    }

    pacKey = *(pacNames + uiItemIndex);
    LOG_WRITE(EN_LOG_DEBUG, "ItemIndex: %u, key: %s, uiDevType: %u, uiPortId: %u", uiItemIndex, pacKey, uiDevType, uiPortId);

    /*
       Note:
        uiItemIndex = 0: OLP, compatible for the old default OPS1
        uiItemIndex > 0: OLP1 ~ OLP9, for multiple OPS
     */
    if (nCount > 1) {
        ++uiItemIndex;
    }

    if (DEF_EVENT_ID_OPS == iEventId) {
        /* wait hal to refresh OPS power */
        usleep(500 * 1000);
    }

    memset(&stOpsStatus, 0, sizeof(stOpsStatus));
    rc = OPS_GetStatusData(uiDevType, &stOpsStatus, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ops get status failed, uiDevType = %u, uiPortId = %u", uiDevType, uiPortId);
        pthread_mutex_unlock(&m_stOpsEventLock);
        return rc;
    }

    memset(&stOpsCommStatus, 0, sizeof(stOpsCommStatus));
    rc = OPS_GetCommStatusData(uiDevType, &stOpsCommStatus);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "ops get commstatus failed, uiDevType = %u, uiPortId = %u", uiDevType, uiPortId);
        pthread_mutex_unlock(&m_stOpsEventLock);
        return rc;
    }

    LOG_WRITE(EN_LOG_DEBUG, "pri: 0x%02x, sec: 0x%02x",
              stOpsCommStatus.stAlarm.aaucPortAlarm[0][0],
              stOpsCommStatus.stAlarm.aaucPortAlarm[0][1]);

    switch (iEventId) {
    case DEF_EVENT_ID_OPS:

        if (DEV_TYPE_OPS4A == uiDevType || DEV_TYPE_OPS4B == uiDevType) {
            /* For OPS4 CARD, this is a fixed code, need to be updated later.*/
            if (DEV_TYPE_OPS4A == uiDevType) {
                iOps4IdxStart = OPS_SUB_INDEX_DEFAULT + OPS1_OPS_CNT_MAX;
                iOps4IdxEnd = OPS_SUB_INDEX_DEFAULT + OPS1_OPS_CNT_MAX + OPS4_OPS_CNT_MAX;
                iOffset = OPS_SUB_INDEX_DEFAULT;
            } else {
                iOps4IdxStart = OPS_SUB_INDEX_DEFAULT + OPS1_OPS_CNT_MAX + OPS4_OPS_CNT_MAX;
                iOps4IdxEnd = OPS_SUB_INDEX_DEFAULT + OPS1_OPS_CNT_MAX + OPS4_OPS_CNT_MAX + OPS4_OPS_CNT_MAX;
                iOffset = OPS_SUB_INDEX_DEFAULT + OPS4_OPS_CNT_MAX;
            }

            for (uiItemIndex = iOps4IdxStart; uiItemIndex < iOps4IdxEnd; uiItemIndex++) {
                m = OPS_IDX_ALM_MFG_FAIL;
                i = m;

                memset(&stOps4Status, 0, sizeof(stOps4Status));
                OPS_GetStatusData(uiDevType, &stOps4Status, (uiItemIndex - iOffset));


                /* OPS_FAIL */
                m_astOpsAlm[uiItemIndex][i++].bCurAlm = (stOpsCommStatus.stAlarm.ucMfgFailed > 0 ? true : false);

                /* OPS_SWITCH_FAIL */
                m_astOpsAlm[uiItemIndex][i++].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucSwitchFailed, uiItemIndex - iOffset);

                /* OPS_RECEIVE_POWER_DIFF */
                m_astOpsAlm[uiItemIndex][i].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucPowerDiffer, uiItemIndex - iOffset);

                snprintf(m_astOpsAlm[uiItemIndex][i].acAlmText, sizeof(m_astOpsAlm[uiItemIndex][i].acAlmText),
                         "%+04.2f",
                         ((float)(abs(stOps4Status.stCurData.asPortPower[OPS_PORT_LINE1_IN] - stOps4Status.stCurData.asPortPower[OPS_PORT_LINE2_IN])) / WEIGHT_HUNDREDTH));
                i++;

                /* OPS_OPS_R_LOS */
                m_astOpsAlm[uiItemIndex][i].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucComInLoss, uiItemIndex - iOffset);
                snprintf(m_astOpsAlm[uiItemIndex][i].acAlmText, sizeof(m_astOpsAlm[uiItemIndex][i].acAlmText),
                         "%+04.2f", ((double)stOps4Status.stConnectionPds.asPd[OPS_R]) / WEIGHT_HUNDREDTH);
                i++;

                ALM_Set(m_astOpsAlm[uiItemIndex] + m, i - m);

                LOG_WRITE(EN_LOG_DEBUG, "%u %s %d", uiItemIndex, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_MFG_FAIL].acAlmStr, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_MFG_FAIL].bCurAlm);
                LOG_WRITE(EN_LOG_DEBUG, "%u %s %d", uiItemIndex, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_SWITCH_FAIL].acAlmStr, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_SWITCH_FAIL].bCurAlm);
                LOG_WRITE(EN_LOG_DEBUG, "%u %s %d", uiItemIndex, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_POWER_DIFF].acAlmStr, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_POWER_DIFF].bCurAlm);
                LOG_WRITE(EN_LOG_DEBUG, "%u %s %d", uiItemIndex, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_COM_RLOS].acAlmStr, m_astOpsAlm[uiItemIndex][OPS_IDX_ALM_COM_RLOS].bCurAlm);

                /* 2. port alarms */
                for (m = 0; m < OPS_PORT_LINE_IN_CNT; m++) {
                    for (n = 0; n < PORT_ALARM_TYPE_CNT; n++) {
                        memset(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, 0, sizeof(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText));
                        snprintf(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, sizeof(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText) - 1, \
                                 "%+04.2f", ((double)stOps4Status.stCurData.asPortPower[m]) / WEIGHT_HUNDREDTH);
                        /* LOG_WRITE(EN_LOG_DEBUG, "m_aastOpsPortAlm[%u][%d][%d]. %s, acAlmText:%s, alarm:%d", \
                                  uiItemIndex, m, n, m_aastOpsPortAlm[uiItemIndex][m][n].acAlmStr, \
                                  m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, \
                                  ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.aaucPortAlarm[n][m], uiItemIndex - iOffset));*/

                        m_aastOpsPortAlm[uiItemIndex][m][n].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.aaucPortAlarm[n][m], uiItemIndex - iOffset);
                    }

                    LOG_WRITE(EN_LOG_DEBUG, "%u %s %d", uiItemIndex,
                              m_aastOpsPortAlm[uiItemIndex][m][0].acAlmStr,
                              m_aastOpsPortAlm[uiItemIndex][m][0].bCurAlm);
                    ALM_Set(m_aastOpsPortAlm[uiItemIndex][m], sizeof(m_aastOpsPortAlm[uiItemIndex][m]) / sizeof(ALM_UNIT_ST));
                }
            }

        } else {
            /* For ops1 card. */
            /* 1. switch alarms */
            m = OPS_IDX_ALM_MFG_FAIL;
            i = m;
            /* OPS_FAIL */
            m_astOpsAlm[uiItemIndex][i++].bCurAlm = (stOpsCommStatus.stAlarm.ucMfgFailed > 0 ? true : false);
            /* OPS_SWITCH_FAIL */
            m_astOpsAlm[uiItemIndex][i++].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucSwitchFailed, uiPortId);

            /* OPS_RECEIVE_POWER_DIFF */
            m_astOpsAlm[uiItemIndex][i].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucPowerDiffer, uiPortId);
            snprintf(m_astOpsAlm[uiItemIndex][i].acAlmText, sizeof(m_astOpsAlm[uiItemIndex][i].acAlmText),
                     "%+04.2f",
                     ((float)(abs(stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE1_IN] - stOpsStatus.stCurData.asPortPower[OPS_PORT_LINE2_IN])) / WEIGHT_HUNDREDTH));
            i++;

            /* OPS_OPS_R_LOS */
            m_astOpsAlm[uiItemIndex][i].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.ucComInLoss, uiPortId);
            snprintf(m_astOpsAlm[uiItemIndex][i].acAlmText, sizeof(m_astOpsAlm[uiItemIndex][i].acAlmText),
                     "%+04.2f", ((double)stOpsStatus.stConnectionPds.asPd[OPS_R]) / WEIGHT_HUNDREDTH);
            i++;

            ALM_Set(m_astOpsAlm[uiItemIndex] + m, i - m);

            /* 2. port alarms */
            for (m = 0; m < OPS_PORT_LINE_IN_CNT; m++) {
                for (n = 0; n < PORT_ALARM_TYPE_CNT; n++) {
                    memset(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, 0, sizeof(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText));
                    snprintf(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, sizeof(m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText) - 1, \
                             "%+04.2f", ((double)stOpsStatus.stCurData.asPortPower[m]) / WEIGHT_HUNDREDTH);
                    LOG_WRITE(EN_LOG_DEBUG, "m_aastOpsPortAlm[%u][%d][%d]. %s, acAlmText:%s, alarm:%d", \
                              uiItemIndex, m, n, m_aastOpsPortAlm[uiItemIndex][m][n].acAlmStr, \
                              m_aastOpsPortAlm[uiItemIndex][m][n].acAlmText, \
                              ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.aaucPortAlarm[n][m], uiPortId));

                    m_aastOpsPortAlm[uiItemIndex][m][n].bCurAlm = ALARM_BYTE_UNPACK(stOpsCommStatus.stAlarm.aaucPortAlarm[n][m], uiPortId);
                }

                ALM_Set(m_aastOpsPortAlm[uiItemIndex][m], sizeof(m_aastOpsPortAlm[uiItemIndex][m]) / sizeof(ALM_UNIT_ST));
            }
        }

        __SetOpsLeds(&stOpsStatus);

        break;
    case DEF_EVENT_ID_OPS_SW_HAPPEND:

        /* when a local SF-P is happened, we need to reset force-to-port to NONE in sysrepo.    */
        /* Exceptionally, do not reset sysrepo when local state is LO.                          */
        if (SR_ERR_OK != DSC_GetOpsConfig(pacKey, &cfg, sizeof(cfg))) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsConfig");
            pthread_mutex_unlock(&m_stOpsEventLock);
            return OPLK_ERR;
        }

        if (SWITCH_TO_SECONDARY == cfg.enForceToPort && OPS_STATUS_SIGNAL_FAIL_WORK == stOpsStatus.stCurData.ucFswStatus) {
            LOG_WRITE(EN_LOG_NOTICE, "Reset force-to-port to NONE in sysrepo, FSM status = %u", stOpsStatus.stCurData.ucFswStatus);
            rc = DSC_SetForceSwitch(pacKey, STR_NONE);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetForceSwitch");
            }
        }

        /* SWITCH event */
        m = OPS_IDX_ALM_SWITCH;
        i = m;
        m_astOpsAlm[uiItemIndex][i].bCurAlm = true;
        snprintf(m_astOpsAlm[uiItemIndex][i].acAlmText, sizeof(m_astOpsAlm[uiItemIndex][i].acAlmText), "%s",
                 (OPS_PATH_PRIMARY == stOpsStatus.stCurData.ucSwitchPos) ? "primary" : "secondary");
        i++;
        /* SWITCH_WAVEFORM_REPORT event */
        m_astOpsAlm[uiItemIndex][i++].bCurAlm = true;

        ALM_Set(m_astOpsAlm[uiItemIndex] + m, i - m);

        __SetOpsLeds(&stOpsStatus);
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "event %d not support!", iEventId);
        rc = SR_ERR_UNSUPPORTED;
        break;
    }

    LOG_WRITE(EN_LOG_DEBUG, "-- OPS_OpsDevEventCb");
    pthread_mutex_unlock(&m_stOpsEventLock);

    return rc;
}

void
OPS_OpsAlmInit(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess, bool bInitAlmTable)
{
    int i = 0;
    size_t nCount = 0, nIndex = 0, nStart = 0;

    if (true == DSC_IsSchemaInstalled(pstRunningSess, MODULE_NS_OPS)) {
        /* init OPS alarm */
        /* Get all existing OLP component names */
        if (OPS_GetNames(NULL, &nCount) != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetNames");
            return;
        }

        LOG_WRITE(EN_LOG_NOTICE, "nCount: %lu", nCount);

        if (nCount > 1) {
            ++nStart;
        }

        if (true == bInitAlmTable) {
            for (nIndex = 0; nIndex < nCount; ++nIndex, ++nStart) {
                ALM_Init(pstStartupSess, pstRunningSess, m_astOpsAlm[nStart], sizeof(m_astOpsAlm[nStart]) / sizeof(ALM_UNIT_ST));

                for (i = 0; i < OPS_PORT_LINE_IN_CNT; i++) {
                    ALM_Init(pstStartupSess, pstRunningSess, m_aastOpsPortAlm[nStart][i], sizeof(m_aastOpsPortAlm[nStart][i]) / sizeof(ALM_UNIT_ST));
                }
            }
        }

        OPS_OpsDevEventCb(DEF_EVENT_ID_OPS, OPS_ID_PACK(DEV_TYPE_OPS, OPS_SUB_INDEX_DEFAULT));

        if (nCount > 1) {
            DEF_SLEEP_S(1);
            OPS_OpsDevEventCb(DEF_EVENT_ID_OPS, OPS_ID_PACK(DEV_TYPE_OPS4A, OPS4_OPS_CNT_MAX));

            DEF_SLEEP_S(1);
            OPS_OpsDevEventCb(DEF_EVENT_ID_OPS, OPS_ID_PACK(DEV_TYPE_OPS4B, OPS4_OPS_CNT_MAX));
        }
    }
}

void
OPS_OpsAlmFree()
{
    ALM_Free();
}

void
OPS_OpsAlmProtoFail(bool bAlmSta)
{
    m_astOpsAlm[0][OPS_IDX_ALM_SWITCH_PROTOCOL_FAILURE].bCurAlm = bAlmSta;
    ALM_Set(m_astOpsAlm[0] + OPS_IDX_ALM_SWITCH_PROTOCOL_FAILURE, 1);
}
