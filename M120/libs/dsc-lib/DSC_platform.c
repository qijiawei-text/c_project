#include "DSC_common.h"
#include "DSC_circuitpack.h"
#include "DSC_module.h"
#include "DSC_platform.h"
#include "DSC_ops.h"
#include "DSC_ocs.h"

#define __set_component_mfginfo(path, data_value) do {          \
        memset(acXpath, 0x00, sizeof(acXpath));              \
        snprintf(acXpath, sizeof(acXpath), path, pcCpName);  \
        rc = set_string_item(pstSess, acXpath, data_value, strlen(data_value)); \
        if (rc != SR_ERR_OK) { \
            LOG_WRITE(EN_LOG_ERROR, "set_string_item: %s, %s, %s", acXpath, data_value, sr_strerror(rc)); \
            goto cleanup; \
        } \
        LOG_WRITE(EN_LOG_DEBUG, "%s %s", acXpath, data_value); \
} while (0)

#define __del_component_mfginfo(path) do {          \
        memset(acXpath, 0x00, sizeof(acXpath));                          \
        snprintf(acXpath, sizeof(acXpath), path, pcCpName);  \
        rc = sr_oper_delete_item_str(pstSess, acXpath, NULL, SR_EDIT_DEFAULT); \
        if (rc != SR_ERR_OK) { \
            LOG_WRITE(EN_LOG_ERROR, "sr_oper_delete_item_str: %s, %s", acXpath, sr_strerror(rc)); \
            goto cleanup; \
        } \
        LOG_WRITE(EN_LOG_DEBUG, "sr_delete_item:%s", acXpath); \
} while (0)



/* Struction for LED name and index for a component. */
typedef struct st_COMPONENT_LED_INFO
{
    const char *pcComponentName;
    const char *pcItemName;
    uint32_t ulItemIndex;
} ST_COMPONENT_LED_INFO;

/* Enum for LED index. */
typedef enum en_DSC_LED_IDX
{
    DSC_LED_IDX0 = 0,
    DSC_LED_IDX1,
    DSC_LED_IDX2,
    DSC_LED_IDX3,
    DSC_LED_IDX4,
    DSC_LED_IDX5,
    DSC_LED_IDX6,
    DSC_LED_IDX7,
    DSC_LED_IDX8,
    DSC_LED_IDX9,
    DSC_LED_IDX10,
    DSC_LED_IDX11,
    DSC_LED_IDX12,
    DSC_LED_IDX13,
    DSC_LED_IDX14,
    DSC_LED_IDX15,
    DSC_LED_IDX16,
    DSC_LED_IDX17
} EN_DSC_LED_IDX;

/* Enum for FAN index. */
typedef enum en_DSC_FAN_IDX
{
    DSC_FAN_IDX0 = 0,
    DSC_FAN_IDX1,
    DSC_FAN_IDX2,
    DSC_FAN_IDX3,
    DSC_FAN_CNT
} EN_DSC_FAN_IDX;

/* Enum for PSU index. */
typedef enum en_DSC_PSU_IDX
{
    DSC_PSU_IDX0 = 0,
    DSC_PSU_IDX1,
    DSC_PSU_CNT
} EN_DSC_PSU_IDX;

/* LED count define for every component. */
#ifdef _NR1004_
#define DSC_AMP_LED_CNT     (4)
#define DSC_AMP_P_LED_CNT   (4)
#endif
#if defined __OLSP__ || defined __OTDR__
#define DSC_AMP_LED_CNT     (6)
#define DSC_AMP_P_LED_CNT   (6)
#endif
#ifdef __HCF__
#define DSC_AMP_LED_CNT     (6)
#define DSC_AMP_P_LED_CNT   (6)
#endif
#ifdef _OCS_
#define DSC_AMP_LED_CNT     (6)
#define DSC_AMP_P_LED_CNT   (6)
#endif
#define DSC_OPS_LED_CNT     (6)
#define DSC_OCS_LED_CNT     (2)
#define DSC_SCC_LED_CNT     (3)
#define DSC_FAN_LED_CNT     (1)
#define DIABLO_DSC_AMP_LED_CNT     (2)
#define DSC_OPS4_LED_CNT     (2 + 4 * 4)

/* None of item */
#define DSC_ITEM_NONE       (0)

typedef enum {
    EN_CHASSIS_TYPE_AMP = 0,     /* eg. OLSP, NR1004_AMP(OPS + AMP * 2) */
    EN_CHASSIS_TYPE_OPS,         /* eg. OLSP, NR1004_OSP(OPS + OPS4 * 2) */
    EN_CHASSIS_TYPE_OCS,         /* eg. OCS 16X16 or 32x32 */
    EN_CHASSIS_TYPE_CNT
} CHASSIS_TYPE_EN;

static uint8_t m_ucChassisType = EN_CHASSIS_TYPE_CNT; /* refer to CHASSIS_TYPE_EN */

/*
   Configuration for every LED name and index.
 */
static const ST_COMPONENT_LED_INFO m_stComponentLedInfo[] = {
    /* SCC */
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_SYS, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_ALM, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_FAN, DSC_LED_IDX2},
#ifdef _OCS_
    {DEF_OCP_COMPONENT_KEY_LC_OCS, LED_NAME_ACT, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_LC_OCS, LED_NAME_ALM, DSC_LED_IDX1},
#else
    /* OPS */
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_CLIENTW, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_CLIENTP, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_MUX, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_WP, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_ACT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_ALM, DSC_LED_IDX5},
    /* AMP */
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_ACT, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_ALM, DSC_LED_IDX1},
#endif
#ifdef _NR1004_
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_LINE, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_CLIENT, DSC_LED_IDX3},
#endif
#ifdef __OLSP__
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_OTDR, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_OCM, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_CLIENT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_LINE, DSC_LED_IDX5},
#endif
#ifdef __HCF__
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_OTDR, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_MUX, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_LINE, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_AMP, LED_NAME_LINE_SAFETY, DSC_LED_IDX5},
#endif
    /* AMP-P */
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_ACT, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_ALM, DSC_LED_IDX1},
#ifdef _NR1004_
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_LINE, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_CLIENT, DSC_LED_IDX3},
#endif
#ifdef __OLSP__
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_OTDR, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_OCM, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_CLIENT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_AMP_P, LED_NAME_LINE, DSC_LED_IDX5},
#endif

    /* FAN */
    {DEF_OCP_COMPONENT_KEY_FAN0, LED_NAME_FAN0, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN1, LED_NAME_FAN1, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN2, LED_NAME_FAN2, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN3, LED_NAME_FAN3, DSC_LED_IDX0}
};

/* define led list for OPS1 + OPS4 * 2 */
static const ST_COMPONENT_LED_INFO m_stOps4ComponentLedInfo[] = {
    /* SCC */
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_SYS, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_ALM, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_SCC, LED_NAME_FAN, DSC_LED_IDX2},
    /* OPS */
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_CLIENTW, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_CLIENTP, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_MUX, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_WP, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_ACT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_OPS, LED_NAME_ALM, DSC_LED_IDX5},

    /* OPS-4-A */
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PRI_1, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_SEC_1, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_COM_1, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PS_1, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_ACT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_ALM, DSC_LED_IDX5},

    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PRI_2, DSC_LED_IDX6},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_SEC_2, DSC_LED_IDX7},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_COM_2, DSC_LED_IDX8},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PS_2, DSC_LED_IDX9},

    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PRI_3, DSC_LED_IDX10},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_SEC_3, DSC_LED_IDX11},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_COM_3, DSC_LED_IDX12},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PS_3, DSC_LED_IDX13},

    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PRI_4, DSC_LED_IDX14},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_SEC_4, DSC_LED_IDX15},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_COM_4, DSC_LED_IDX16},
    {DEF_OCP_COMPONENT_KEY_OPS_4_A, LED_NAME_PS_4, DSC_LED_IDX17},

    /* OPS-4-B */
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PRI_1, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_SEC_1, DSC_LED_IDX1},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_COM_1, DSC_LED_IDX2},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PS_1, DSC_LED_IDX3},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_ACT, DSC_LED_IDX4},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_ALM, DSC_LED_IDX5},

    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PRI_2, DSC_LED_IDX6},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_SEC_2, DSC_LED_IDX7},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_COM_2, DSC_LED_IDX8},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PS_2, DSC_LED_IDX9},

    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PRI_3, DSC_LED_IDX10},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_SEC_3, DSC_LED_IDX11},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_COM_3, DSC_LED_IDX12},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PS_3, DSC_LED_IDX13},

    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PRI_4, DSC_LED_IDX14},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_SEC_4, DSC_LED_IDX15},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_COM_4, DSC_LED_IDX16},
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, LED_NAME_PS_4, DSC_LED_IDX17},

    /* FAN */
    {DEF_OCP_COMPONENT_KEY_FAN0, LED_NAME_FAN0, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN1, LED_NAME_FAN1, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN2, LED_NAME_FAN2, DSC_LED_IDX0},
    {DEF_OCP_COMPONENT_KEY_FAN3, LED_NAME_FAN3, DSC_LED_IDX0}
};

/*
   Configuration for every component information.
 */
static ST_DSC_COMPONENT_INFO m_stComponentDevInfo[] = {
    /*Component                 Type      Slot           Sub-index      LED count  */
#if defined(_OCS_)
    {DEF_OCP_COMPONENT_KEY_LC_OCS, CARD_OCS, DSC_SLOT_IDX0, DSC_ITEM_NONE, DSC_OCS_LED_CNT},
#elif defined(__HCF__)
    {DEF_OCP_COMPONENT_KEY_AMP, CARD_AMP, DSC_SLOT_IDX0, DSC_ITEM_NONE, DSC_AMP_LED_CNT},
#else
    {DEF_OCP_COMPONENT_KEY_OPS, CARD_OPS, DSC_SLOT_IDX0, DSC_ITEM_NONE, DSC_OPS_LED_CNT},

    {DEF_OCP_COMPONENT_KEY_AMP, CARD_AMP, DSC_SLOT_IDX1, DSC_ITEM_NONE, DSC_AMP_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_AMP_P, CARD_AMP, DSC_SLOT_IDX2, DSC_ITEM_NONE, DSC_AMP_P_LED_CNT},
#endif

    {DEF_OCP_COMPONENT_KEY_SCC, CARD_SCC, DSC_SLOT_IDX3, DSC_ITEM_NONE, DSC_SCC_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN0, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX0, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN1, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX1, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN2, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX2, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN3, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX3, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_RPC, CARD_UNKOWN, DSC_SLOT_NONE, DSC_ITEM_NONE, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_PSU1, CARD_POWER, DSC_SLOT_NONE, DSC_PSU_IDX0, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_PSU2, CARD_POWER, DSC_SLOT_NONE, DSC_PSU_IDX1, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_CHASSIS, CARD_CLASSIS, DSC_SLOT_NONE, DSC_ITEM_NONE, DSC_ITEM_NONE}
};

static ST_DSC_COMPONENT_INFO m_stOps4ComponentDevInfo[] = {
    /*Component                 Type      Slot           Sub-index      LED count  */
    {DEF_OCP_COMPONENT_KEY_OPS, CARD_OPS, DSC_SLOT_IDX0, DSC_ITEM_NONE, DSC_OPS_LED_CNT},

    {DEF_OCP_COMPONENT_KEY_OPS_4_A, CARD_OPS, DSC_SLOT_IDX1, DSC_ITEM_NONE, DSC_OPS4_LED_CNT},  /* OPS-4 card */
    {DEF_OCP_COMPONENT_KEY_OPS_4_B, CARD_OPS, DSC_SLOT_IDX2, DSC_ITEM_NONE, DSC_OPS4_LED_CNT},  /* OPS-4 card */

    {DEF_OCP_COMPONENT_KEY_SCC, CARD_SCC, DSC_SLOT_IDX3, DSC_ITEM_NONE, DSC_SCC_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN0, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX0, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN1, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX1, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN2, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX2, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_FAN3, CARD_FAN, DSC_SLOT_NONE, DSC_FAN_IDX3, DSC_FAN_LED_CNT},
    {DEF_OCP_COMPONENT_KEY_RPC, CARD_UNKOWN, DSC_SLOT_NONE, DSC_ITEM_NONE, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_PSU1, CARD_POWER, DSC_SLOT_NONE, DSC_PSU_IDX0, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_PSU2, CARD_POWER, DSC_SLOT_NONE, DSC_PSU_IDX1, DSC_ITEM_NONE},
    {DEF_OCP_COMPONENT_KEY_CHASSIS, CARD_CLASSIS, DSC_SLOT_NONE, DSC_ITEM_NONE, DSC_ITEM_NONE}
};


static int
__SetComponentSlotStatus(sr_session_ctx_t *pstSess, char *pcCpName, bool bEmpty, char *pcOperStatus, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acXpath[DEF_BUF_SIZE_1024] = {0};

    if (NULL == pstSess || NULL == pcCpName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OCP_XPATH_COMPONENT_STATE_EMPTY, pcCpName);

    rc = set_bool_item(pstSess, acXpath, bEmpty);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set xpath:%s, value: %d error: %s", acXpath, bEmpty, sr_strerror(rc));
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), OCP_XPATH_COMPONENT_STATE_OPER_STATUS, pcCpName);

    rc = set_identityref_item(pstSess, acXpath, pcOperStatus, strlen(pcOperStatus));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Set xpath:%s, value: %s error: %s", acXpath, pcOperStatus, sr_strerror(rc));
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
        }
    }

cleanup:
    return rc;
}

int
DSC_OCP_SetComponentSlotStatus(char *pcCpName, bool bSlotOnline)
{
    char *pcOperStatus = NULL;
    bool bEmpty = true;

#if __OPER__
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
#else
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
#endif

    bEmpty = !bSlotOnline;
    pcOperStatus = bSlotOnline ? COMPONENT_OPER_STATUS_ACTIVE : COMPONENT_OPER_STATUS_INACTIVE;

    return __SetComponentSlotStatus(pstSess, pcCpName, bEmpty, pcOperStatus, true);
}

static int
__SetComponentMfgInfo(sr_session_ctx_t *pstSess, char *pcCpName, ST_OC_COMPONENT_STATE *pstComponentState, bool bCommit)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pstSess || NULL == pcCpName || NULL == pstComponentState) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

#if 0
    LOG_WRITE(EN_LOG_NOTICE, "++++++++++pcCpName:%s", pcCpName);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgName:%s, acSerialNo:%s, acPartNo:%s, acHardwareVersion:%s,", pstComponentState->acMfgName,
              pstComponentState->acSerialNo, pstComponentState->acPartNo, pstComponentState->acHardwareVersion);

    LOG_WRITE(EN_LOG_NOTICE, "SwVer:%s, acFwVer:%s, acMfgDate:%s,", pstComponentState->acSoftwareVersion,
              pstComponentState->acFirmwareVersion, pstComponentState->acMfgDate);
#endif

    __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_MFG_NAME, pstComponentState->acMfgName);
    /* __set_component_mfginfo(COP_XPATH_COMPONENT_STATE_LOCATION, pstComponentState->acLocation); */
    if (0 < strlen(pstComponentState->acSerialNo)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_SN, pstComponentState->acSerialNo);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_SN);
    }

    if (0 < strlen(pstComponentState->acPartNo)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_PN, pstComponentState->acPartNo);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_PN);
    }

    if (0 < strlen(pstComponentState->acHardwareVersion)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_HW_VER, pstComponentState->acHardwareVersion);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_HW_VER);
    }

    if (0 < strlen(pstComponentState->acSoftwareVersion)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_SW_VER, pstComponentState->acSoftwareVersion);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_SW_VER);
    }

    if (0 < strlen(pstComponentState->acFirmwareVersion)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FW_VER, pstComponentState->acFirmwareVersion);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FW_VER);
    }

    if ( strlen(pstComponentState->acMfgDate) > 0) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_MFG_DATE, pstComponentState->acMfgDate);
    } else {
        /* NOTE: set default value because of deleting node failed, TODO */
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_MFG_DATE, DEF_OCP_MFG_DATE);
        /* __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_MFG_DATE);*/
    }

    if (0 < strlen(pstComponentState->acUnitName)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_UNIT_NAME, pstComponentState->acUnitName);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_UNIT_NAME);
    }

    if (0 < strlen(pstComponentState->acFcNumber)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FC_NUMBER, pstComponentState->acFcNumber);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FC_NUMBER);
    }

    if (0 < strlen(pstComponentState->acPiuIssueNumber)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_PIU_ISSUE_NUMBER, pstComponentState->acPiuIssueNumber);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_PIU_ISSUE_NUMBER);
    }

    if (0 < strlen(pstComponentState->acFujitsuPartNumber)) {
        __set_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FUJITSU_PART_NUMBER, pstComponentState->acFujitsuPartNumber);
    } else {
        __del_component_mfginfo(OCP_XPATH_COMPONENT_STATE_FUJITSU_PART_NUMBER);
    }

    if (0 < strlen(pstComponentState->acFjtVendorName)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_VENDOR, pstComponentState->acFjtVendorName);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_VENDOR);
    }

    if (0 < strlen(pstComponentState->acFjtSn)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_SN, pstComponentState->acFjtSn);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_SN);
    }

    if (0 < strlen(pstComponentState->acClei)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CLEI, pstComponentState->acClei);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CLEI);
    }

    if (0 < strlen(pstComponentState->acModel)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_MODEL, pstComponentState->acModel);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_MODEL);
    }

    if (0 < strlen(pstComponentState->acFpgaVer)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FPGA_VER, pstComponentState->acFpgaVer);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_FPGA_VER);
    }

    if (0 < strlen(pstComponentState->acCpldVer)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CPLD_VER, pstComponentState->acCpldVer);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CPLD_VER);
    }

    if (0 < strlen(pstComponentState->acConfigType)) {
        __set_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CONFIG_TYPE, pstComponentState->acConfigType);
    } else {
        __del_component_mfginfo(DEF_OCP_XPATH_COMPONENT_STATE_CONFIG_TYPE);
    }

    if (bCommit) {
        rc = DSC_Commit(pstSess);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "commit error: %s", sr_strerror(rc));
        }
    }

cleanup:
    /* LOG_WRITE(EN_LOG_NOTICE, "---------pcCpName:%s, rc:%d", pcCpName, rc); */
    return rc;
}

int
DSC_OCP_SetComponentMfgInfo(char *pcCpName, ST_OC_COMPONENT_STATE *pstComponentState)
{
#if __OPER__
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
#else
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
#endif

    return __SetComponentMfgInfo(pstSess, pcCpName, pstComponentState, true);
}

int
DSC_OCP_SetComponentState(char *pcCpName, ST_OC_COMPONENT_STATE *pstComponentState)
{
    int rc = SR_ERR_OK;

#if __OPER__
    sr_session_ctx_t *pstSess = DSC_GetOperSession();
#else
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
#endif

    if (NULL == pstSess || NULL == pstComponentState) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __SetComponentMfgInfo(pstSess, pcCpName, pstComponentState, false);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__SetComponentMfgInfo error: %s, pcCpName: %s", sr_strerror(rc), pcCpName ? pcCpName : "NULL");
        goto cleanup;
    }

    rc = __SetComponentSlotStatus(pstSess, pcCpName, pstComponentState->bEmpty, pstComponentState->acOperStatus, true);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__SetComponentSlotStatus error: %s, pcCpName: %s", sr_strerror(rc), pcCpName ? pcCpName : "NULL");
        goto cleanup;
    }

cleanup:
    return rc;
}

int
DSC_OCP_GetComponentState(char *pcComponentName, ST_OC_COMPONENT_STATE *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_STATE_NAME, pcComponentName), SR_STRING_T, pstComponentInfo->acName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OCP_XPATH_COMPONENT_STATE_TYPE, pcComponentName), SR_STRING_T, pstComponentInfo->acType),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OCP_XPATH_COMPONENT_STATE_ID, pcComponentName), SR_STRING_T, pstComponentInfo->acId),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OCP_XPATH_COMPONENT_STATE_LOCATION, pcComponentName), SR_STRING_T, pstComponentInfo->acLocation),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OCP_XPATH_COMPONENT_STATE_DESCRIPTION, pcComponentName), SR_STRING_T, pstComponentInfo->acDescription),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], OCP_XPATH_COMPONENT_STATE_MFG_NAME, pcComponentName), SR_STRING_T, pstComponentInfo->acMfgName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[6], OCP_XPATH_COMPONENT_STATE_MFG_DATE, pcComponentName), SR_STRING_T, pstComponentInfo->acMfgDate),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[7], OCP_XPATH_COMPONENT_STATE_HW_VER, pcComponentName), SR_STRING_T, pstComponentInfo->acHardwareVersion),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[8], OCP_XPATH_COMPONENT_STATE_FW_VER, pcComponentName), SR_STRING_T, pstComponentInfo->acFirmwareVersion),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[9], OCP_XPATH_COMPONENT_STATE_SW_VER, pcComponentName), SR_STRING_T, pstComponentInfo->acSoftwareVersion),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[10], OCP_XPATH_COMPONENT_STATE_SN, pcComponentName), SR_STRING_T, pstComponentInfo->acSerialNo),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[11], OCP_XPATH_COMPONENT_STATE_PN, pcComponentName), SR_STRING_T, pstComponentInfo->acPartNo),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[12], OCP_XPATH_COMPONENT_STATE_REMOVABLE, pcComponentName), SR_BOOL_T, (char *)(&pstComponentInfo->bRemovable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[13], OCP_XPATH_COMPONENT_STATE_OPER_STATUS, pcComponentName), SR_STRING_T, pstComponentInfo->acOperStatus),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[14], OCP_XPATH_COMPONENT_STATE_EMPTY, pcComponentName), SR_BOOL_T, (char *)(&pstComponentInfo->bEmpty)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], OCP_XPATH_COMPONENT_STATE_PARENT, pcComponentName), SR_STRING_T, pstComponentInfo->acParent),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[15], OCP_XPATH_COMPONENT_STATE_UNIT_NAME, pcComponentName), SR_STRING_T, pstComponentInfo->acUnitName),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[16], OCP_XPATH_COMPONENT_STATE_FC_NUMBER, pcComponentName), SR_STRING_T, pstComponentInfo->acFcNumber),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[17], OCP_XPATH_COMPONENT_STATE_PIU_ISSUE_NUMBER, pcComponentName), SR_STRING_T, pstComponentInfo->acPiuIssueNumber),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[18], OCP_XPATH_COMPONENT_STATE_FUJITSU_PART_NUMBER, pcComponentName), SR_STRING_T, pstComponentInfo->acFujitsuPartNumber),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_STATE));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_STATE_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_STATE_ALL, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentStateInfo->acName=%s \n", __func__,__LINE__,pstComponentInfo->acName); */
    /* printf("%s:%d pstComponentStateInfo->acType=%s \n", __func__,__LINE__,pstComponentInfo->acType); */
    /* printf("%s:%d pstComponentStateInfo->acId=%s \n", __func__,__LINE__,pstComponentInfo->acId); */
    /* printf("%s:%d pstComponentStateInfo->acLocation=%s \n", __func__,__LINE__,pstComponentInfo->acLocation); */
    /* printf("%s:%d pstComponentStateInfo->acDescription=%s \n", __func__,__LINE__,pstComponentInfo->acDescription); */
    /* printf("%s:%d pstComponentStateInfo->acMfgName=%s \n", __func__,__LINE__,pstComponentInfo->acMfgName); */
    /* printf("%s:%d pstComponentStateInfo->acMfgDate=%s \n", __func__,__LINE__,pstComponentInfo->acMfgDate); */
    /* printf("%s:%d pstComponentStateInfo->acHardwareVersion=%s \n", __func__,__LINE__,pstComponentInfo->acHardwareVersion); */
    /* printf("%s:%d pstComponentStateInfo->acFirmwareVersion=%s \n", __func__,__LINE__,pstComponentInfo->acFirmwareVersion); */
    /* printf("%s:%d pstComponentStateInfo->acSoftwareVersion=%s \n", __func__,__LINE__,pstComponentInfo->acSoftwareVersion); */
    /* printf("%s:%d pstComponentStateInfo->acSerialNo=%s \n", __func__,__LINE__,pstComponentInfo->acSerialNo); */
    /* printf("%s:%d pstComponentStateInfo->acPartNo=%s \n", __func__,__LINE__,pstComponentInfo->acPartNo); */
    /* printf("%s:%d pstComponentStateInfo->bRemovable=%d \n", __func__,__LINE__,pstComponentInfo->bRemovable); */
    /* printf("%s:%d pstComponentStateInfo->acOperStatus=%s \n", __func__,__LINE__,pstComponentInfo->acOperStatus); */
    /* printf("%s:%d pstComponentStateInfo->bEmpty=%d \n", __func__,__LINE__,pstComponentInfo->bEmpty); */
    /* printf("%s:%d pstComponentStateInfo->acParent=%s \n", __func__,__LINE__,pstComponentInfo->acParent); */
    if ( 0 == strcmp(pcComponentName, COMPONENT_KEY_OLSP)) {
        DSC_OCP_GetMemByName(pcComponentName, &pstComponentInfo->stMem);
        DSC_OCP_GetTempByName(pcComponentName, &pstComponentInfo->stTemper);
    } else if ( NULL != strstr(pcComponentName, "FGA")) {
        DSC_OCP_GetTempByName(pcComponentName, &pstComponentInfo->stTemper);
    }
cleanup:
    return rc;
}

int
DSC_OCP_GetCpuByName(char *pcComponentName, ST_OC_COMPONENT_CPU *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_CPU_STATE_INSTANT, pcComponentName), SR_UINT8_T, (char *)(&pstComponentInfo->ucInstant)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OCP_XPATH_COMPONENT_CPU_STATE_AVG, pcComponentName), SR_UINT8_T, (char *)(&pstComponentInfo->ucAvg)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OCP_XPATH_COMPONENT_CPU_STATE_MIN, pcComponentName), SR_UINT8_T, (char *)(&pstComponentInfo->ucMin)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], OCP_XPATH_COMPONENT_CPU_STATE_MAX, pcComponentName), SR_UINT8_T, (char *)(&pstComponentInfo->ucMax)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], OCP_XPATH_COMPONENT_CPU_STATE_INTERVAL, pcComponentName), SR_UINT64_T, (char *)(&pstComponentInfo->ullInterval)),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_CPU));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_CPU_STATE_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_CPU_STATE, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentStateCpuInfo->ucInstant=%d \n", __func__,__LINE__,pstComponentInfo->ucInstant); */
    /* printf("%s:%d pstComponentStateCpuInfo->ucAvg=%d \n", __func__,__LINE__,pstComponentInfo->ucAvg); */
    /* printf("%s:%d pstComponentStateCpuInfo->ucInstant=%d \n", __func__,__LINE__,pstComponentInfo->ucMin); */
    /* printf("%s:%d pstComponentStateCpuInfo->ucMax=%d \n", __func__,__LINE__,pstComponentInfo->ucMax); */
    /* printf("%s:%d pstComponentStateCpuInfo->ullInterval=%lu \n", __func__,__LINE__,pstComponentInfo->ullInterval); */

cleanup:
    return rc;
}


int
DSC_OCP_GetFanByName(char *pcComponentName, ST_OC_COMPONENT_FAN *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_FAN_STATE_SPEED, pcComponentName), SR_UINT32_T, (char *)(&pstComponentInfo->ulSpeed)),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_FAN));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_FAN_STATE_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_FAN_STATE_ALL, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentInfo->speed=%d \n", __func__,__LINE__,pstComponentInfo->ulSpeed); */

cleanup:
    return rc;
}


int
DSC_OCP_GetPsuByName(char *pcComponentName, ST_OC_COMPONENT_POWER_SUPPLY *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_IN_CUR, pcComponentName), SR_BINARY_T, (char *)(&pstComponentInfo->acInputCurrent)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_IN_VOL, pcComponentName), SR_BINARY_T, (char *)(&pstComponentInfo->acInputVoltage)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_OUT_VOL, pcComponentName), SR_BINARY_T, (char *)(&pstComponentInfo->acOutputVoltage)),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_POWER_SUPPLY));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_ALL, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentInfo->InputCurrent=%s \n", __func__,__LINE__,pstComponentInfo->acInputCurrent); */
    /* printf("%s:%d pstComponentInfo->InputVoltage=%s \n", __func__,__LINE__,pstComponentInfo->acInputVoltage); */
    /* printf("%s:%d pstComponentInfo->OutputVoltage=%s \n", __func__,__LINE__,pstComponentInfo->acOutputVoltage); */

cleanup:
    return rc;
}


int
DSC_OCP_GetMemByName(char *pcComponentName, ST_OC_COMPONENT_STATE_MEMORY *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_STATE_MEMORY_AVAILABLE, pcComponentName), SR_UINT64_T, (char *)(&pstComponentInfo->ullAvailable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], OCP_XPATH_COMPONENT_STATE_MEMORY_UTILIZED, pcComponentName), SR_UINT64_T, (char *)(&pstComponentInfo->ullUtilized)),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_STATE_MEMORY));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_STATE_MEMORY_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_STATE_MEMORY, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentStateMemInfo->ullAvailable=%ld \n", __func__,__LINE__,pstComponentInfo->ullAvailable); */
    /* printf("%s:%d pstComponentStateMemInfo->ullUtilized=%ld \n", __func__,__LINE__,pstComponentInfo->ullUtilized); */

cleanup:
    return rc;
}

int
DSC_OCP_GetTempByName(char *pcComponentName, ST_OC_COMPONENT_STATE_TEMPERATURE *pstComponentInfo)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN] = {{0}};
    char acSelectXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName ||
        NULL == pstComponentInfo) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d SR_ERR_INVAL_ARG \n", __func__,__LINE__); */
        goto cleanup;
    }

    ANONYMOUS_ST astMaps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[0], OCP_XPATH_COMPONENT_STATE_TEMP_INSTANT, pcComponentName), SR_DECIMAL64_T, (char *)(&pstComponentInfo->dInstant)),
    };

    pstSess = DSC_GetOperSession();

    memset((void *)pstComponentInfo, 0x00, sizeof(ST_OC_COMPONENT_STATE_TEMPERATURE));
    memset(acSelectXpath, 0x00, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_STATE_TEMP_ALL, pcComponentName);                    /* COP_XPATH_COMPONENT_ALL */
    /* printf("%s:%d acSelectXpath=%s \n", __func__,__LINE__,acSelectXpath); */
    rc = DSC_GetItems(pstSess, acSelectXpath, astMaps, sizeof(astMaps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s, %s", acSelectXpath, sr_strerror(rc));
        /* printf("Error by DSC_GetItems: %s, %s\n", OCP_XPATH_COMPONENT_STATE_TEMP_ALL, sr_strerror(rc)); */
        return rc;
    }

    /* printf("%s:%d pstComponentStateTempInfo->dInstant=%f \n", __func__,__LINE__,pstComponentInfo->dInstant); */

cleanup:
    return rc;
}

static int
__GetSubComponentCount(char *pcComponentName, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcComponentName || NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_SUBCOMPINENT_ALL, pcComponentName);

    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    *ptCount = (tValuesCount > OCP_SUBCOMPONENT_NUM_MAX) ? OCP_SUBCOMPONENT_NUM_MAX : tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_OCP_GetSubComponentsCountAndName(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT *pstSubComponent, size_t *ptSubCompntCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;

    if (NULL == pcComponentName || NULL == pstSubComponent) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d rc = SR_ERR_INVAL_ARG; \n", __func__,__LINE__); */
        goto cleanup;
    }

    /*
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='LINECARD-1-1']/name = LINECARD-1-1
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='PSU1']/name = PSU1
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='PSU2']/name = PSU2
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='FAN1']/name = FAN1
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='FAN2']/name = FAN2
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='FAN3']/name = FAN3
        /openconfig-platform:components/component[name='FMD']/subcomponents/subcomponent[name='In01']/name = In01
     */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_SUBCOMPINENT_NAME, pcComponentName);

    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if (SR_ERR_NOT_FOUND == rc) {
        /* LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath); */
        /* printf("%s:%d,Not found item: %s, xpath: %s\n", __func__,__LINE__,sr_strerror(rc), ALARM_XPATH_LIST_I); */
        if (pstValues != NULL)
            sr_free_values(pstValues, tValuesCount);

        *ptSubCompntCount = 0;
        return SR_ERR_OK;
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        /* printf("%s:%d get_items: %s name, error: %s \n", __func__,__LINE__,acXpath, sr_strerror(rc)); */
        goto cleanup;
    }
    /* printf("subcomponent of:%s, tValuesCount=%d,tMaxCount=%d\n",pcComponentName,(int)tValuesCount,(int)tMaxCount); */


    for (i = 0; i < tValuesCount; i++) {
        /* printf("##### j=%d,pstValues[%d].xpath=%s\n",j,(int)i,pstValues[i].xpath); */
        j++;
        if ( j >= OCP_SUBCOMPONENT_NUM_MAX) {
            /* printf("j >= OCP_SUBCOMPONENT_NUM_MAX!!!,j=%d\n",j); */
            goto cleanup;
        }
        strncpy(pstSubComponent[j].acName, pstValues[i].data.string_val, sizeof(pstSubComponent[j].acName) - 1);
    }
    *ptSubCompntCount = j + 1;
cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetSubComponentInfo(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT *pstSubComponent, size_t tMaxCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;
    /* printf("%s:%d pcComponentName1=%s \n", __func__,__LINE__,pcComponentName); */
    if (NULL == pstSubComponent) {
        rc = SR_ERR_INVAL_ARG;
        /* printf("%s:%d rc = SR_ERR_INVAL_ARG; \n", __func__,__LINE__); */
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_SUBCOMPINENTS, pcComponentName);
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        /* printf("%s:%d get_items: %s name, error: %s \n", __func__,__LINE__,acXpath, sr_strerror(rc)); */
        goto cleanup;
    }
    /* printf("subcomponent of:%s, tValuesCount=%d,tMaxCount=%d\n",pcComponentName,(int)tValuesCount,(int)tMaxCount); */
    for (i = 0; i < tValuesCount; i++) {
        /* printf("j=%d,pstValues[%d].xpath=%s\n",j,(int)i,pstValues[i].xpath); */
        if (NULL != strstr(pstValues[i].xpath, LEAF_SUBCOMPONENT_NAME)) {
            j++;
            if (j >= tMaxCount || j >= OCP_SUBCOMPONENT_NUM_MAX) {
                goto cleanup;
            }
            strncpy(pstSubComponent[j].acName, pstValues[i].data.string_val, sizeof(pstSubComponent[j].acName) - 1);
        }
    }

cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

static int
__GetSubComponents(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT **ppstSubComponent, size_t *ptSubCompntCount)
{
    int rc = SR_ERR_OK;
    ST_OC_COMPONENT_SUBCOMPONENT *pstSubComponent = NULL;

    if (NULL == pcComponentName ||
        NULL == ppstSubComponent ||
        NULL == ptSubCompntCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetSubComponentCount(pcComponentName, ptSubCompntCount);
    if (rc != SR_ERR_OK) {
        /* printf("Get component count error1: %s\n", sr_strerror(rc)); */
        if (SR_ERR_NOT_FOUND == rc) {
            rc = SR_ERR_OK;
            *ptSubCompntCount = 0;
            /* printf("SR_ERR_NOT_FOUND\n"); */
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get component count error: %s", sr_strerror(rc));
            /* printf("Get component count error2: %s\n", sr_strerror(rc)); */
        }
        goto cleanup;
    }
    /* printf("ptSubCompntCount = %d\n",(int)*ptSubCompntCount); */
    if ( *ptSubCompntCount > OCP_SUBCOMPONENT_NUM_MAX) {
        LOG_WRITE(EN_LOG_WARNING, "SubCompntCount[%d] > OCP_SUBCOMPONENT_NUM_MAX", (int)*ptSubCompntCount);
        *ptSubCompntCount = OCP_SUBCOMPONENT_NUM_MAX;
    }
    pstSubComponent = (ST_OC_COMPONENT_SUBCOMPONENT *)malloc(sizeof(ST_OC_COMPONENT_SUBCOMPONENT) * (*ptSubCompntCount));
    if (NULL == pstSubComponent) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstSubComponent, 0x00, (sizeof(ST_OC_COMPONENT_SUBCOMPONENT) * (*ptSubCompntCount)));

    rc = __GetSubComponentInfo(pcComponentName, pstSubComponent, *ptSubCompntCount);
    if (rc != SR_ERR_OK) {
        /* printf("__GetSubComponentInfo error: %s", sr_strerror(rc)); */
        DEF_SAFE_FREE(pstSubComponent);
        goto cleanup;
    }

    *ppstSubComponent = pstSubComponent;

cleanup:
    return rc;
}

int
DSC_OCP_GetSubComponents(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT **ppstSubComponent, size_t *ptSubCompntCount)
{
    return __GetSubComponents(pcComponentName, ppstSubComponent, ptSubCompntCount);
}

int
DSC_OCP_GetComponentsCountAndName(ST_OC_COMPONENT *pstComponent, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    int i;

    if (NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, OCP_XPATH_COMPONENTS_NAME, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    *ptCount = tValuesCount;
    /* printf("tValuesCount = %d\n",(int)tValuesCount); */
    for (i = 0; i < tValuesCount; i++) {
        /* printf("pstValues[%d].xpath=%s\n",(int)i,pstValues[i].xpath); */
        /* printf("pstValues[%d].data.string_val = %s\n",(int)i,pstValues[i].data.string_val); */
        strncpy(pstComponent[i].acName, pstValues[i].data.string_val, sizeof(pstComponent[i].acName) - 1);
    }

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}


static int
__GetComponentCount(size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;

    if (NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, OCP_XPATH_COMPONENTS_ALL, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    *ptCount = tValuesCount;

cleanup:

    sr_free_values(pstValues, tValuesCount);
    return rc;
}


static int
__GetComponentInfo(ST_OC_COMPONENT *pstComponent, size_t tMaxCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    size_t i = 0;
    int j = -1;
    int k = -1;

    if (NULL == pstComponent) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    memcpy(acXpath, OCP_XPATH_COMPONENTLIST, strlen(OCP_XPATH_COMPONENTLIST));
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if (SR_ERR_TIME_OUT == rc) {
        LOG_WRITE(EN_LOG_ERROR, "SR_ERR_TIME_OUT: %s, xpath: %s", sr_strerror(rc), acXpath);
        goto cleanup;
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s , error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }
    /* printf("%s:%d ######## tValuesCount=%d,tMaxCount=%d \n", __func__,__LINE__,(int)tValuesCount,(int)tMaxCount); */

    for (i = 0; i < tValuesCount; i++) {
        /* printf("j=%d,pstValues[%d].xpath=%s\n",j,(int)i,pstValues[i].xpath); */
        /* LOG_WRITE(EN_LOG_NOTICE, "j=%d,pstValues[%d].xpath=%s", j,(int)i,pstValues[i].xpath); */
        if (NULL != strstr(pstValues[i].xpath, OCP_EXT_LEDS)) {
            /* ignore */
            continue;
        }

        if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_NAME) && strlen(pstValues[i].xpath) < OCP_COMPONENT_NAME_XPATH_LEN_MAX
            && NULL == strstr(pstValues[i].xpath, OCP_PLATFORM_LEAF_SUBCOMPONENTS)) {                    /* openconfig-platform:components/component[name='FMD']/name */
            j++;
            k = 0;
            if (j >= tMaxCount || j >= OCP_COMPONENT_NUM_MAX) {
                /* printf("j >= tMaxCount,j=%d\n",j); */
                break;                    /* goto cleanup; */
            }
            strncpy(pstComponent[j].acName, pstValues[i].data.string_val, sizeof(pstComponent[j].acName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TYPE)) {
            strncpy(pstComponent[j].stState.acType, pstValues[i].data.identityref_val, sizeof(pstComponent[j].stState.acType) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_ID)) {
            strncpy(pstComponent[j].stState.acId, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acId) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_LOCATION)) {
            strncpy(pstComponent[j].stState.acLocation, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acLocation) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_DESC)) {
            strncpy(pstComponent[j].stState.acDescription, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acDescription) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_MFG_NAME)) {
            strncpy(pstComponent[j].stState.acMfgName, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acMfgName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_MFG_DATA)) {
            strncpy(pstComponent[j].stState.acMfgDate, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acMfgDate) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_HW_VER)) {
            strncpy(pstComponent[j].stState.acHardwareVersion, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acHardwareVersion) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_FW_VER)) {
            strncpy(pstComponent[j].stState.acFirmwareVersion, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFirmwareVersion) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_SW_VER)) {
            strncpy(pstComponent[j].stState.acSoftwareVersion, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acSoftwareVersion) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_SN)) {
            strncpy(pstComponent[j].stState.acSerialNo, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acSerialNo) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PN)) {
            strncpy(pstComponent[j].stState.acPartNo, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acPartNo) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_UNIT_NAME)) {
            strncpy(pstComponent[j].stState.acUnitName, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acUnitName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_FC_NUMBER)) {
            strncpy(pstComponent[j].stState.acFcNumber, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFcNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PIU_ISSUE_NUMBER)) {
            strncpy(pstComponent[j].stState.acPiuIssueNumber, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acPiuIssueNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_FUJITSU_PART_NUMBER)) {
            strncpy(pstComponent[j].stState.acFujitsuPartNumber, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFujitsuPartNumber) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_REMOVE)) {
            pstComponent[j].stState.bRemovable = pstValues[i].data.bool_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_OPER_STATUS)) {
            strncpy(pstComponent[j].stState.acOperStatus, pstValues[i].data.identityref_val, sizeof(pstComponent[j].stState.acOperStatus) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_EMPTY)) {
            pstComponent[j].stState.bEmpty = pstValues[i].data.bool_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PARENT)) {
            strncpy(pstComponent[j].stState.acParent, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acParent) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TEMP_INSTANT)) {
            pstComponent[j].stState.stTemper.dInstant = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TEMP_AVG)) {
            pstComponent[j].stState.stTemper.dAvg = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TEMP_MIN)) {
            pstComponent[j].stState.stTemper.dMax = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TEMP_MAX)) {
            pstComponent[j].stState.stTemper.dMin = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TEMP_INTERVAL)) {
            pstComponent[j].stState.stTemper.ullInterval = pstValues[i].data.uint64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_MEM_AVAIL)) {
            pstComponent[j].stState.stMem.ullAvailable = pstValues[i].data.uint64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_MEM_UTIL)) {
            pstComponent[j].stState.stMem.ullUtilized = pstValues[i].data.uint64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_CPU_INSTANT)) {
            pstComponent[j].stCpu.ucInstant = pstValues[i].data.uint8_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_CPU_INTERVAL)) {
            pstComponent[j].stCpu.ullInterval = pstValues[i].data.uint64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_FAN_SPEED)) {
            pstComponent[j].stFan.ulSpeed = pstValues[i].data.uint32_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_CAP)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acCapacity, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acCapacity) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_IN_CUR)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acInputCurrent, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acInputCurrent) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_IN_VOL)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acInputVoltage, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acInputVoltage) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_OUT_CUR)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acOutputCurrent, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acOutputCurrent) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_OUT_VOL)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acOutputVoltage, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acOutputVoltage) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_PSU_OUT_POWER)) {
            strncpy((char *)&pstComponent[j].stPowerSupply.acOutputPower, pstValues[i].data.binary_val, sizeof(pstComponent[j].stPowerSupply.acOutputPower) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, OCP_PLATFORM_LEAF_SUBCOMPONENTS) && NULL != strstr(pstValues[i].xpath, "]/name")) {
            if ( k < OCP_SUBCOMPONENT_NUM_MAX ) {
                strncpy((char *)&pstComponent[j].stSubcomponent[k].acName, pstValues[i].data.string_val, sizeof(pstComponent[j].stSubcomponent[k].acName) - 1);
                k++;
                pstComponent[j].iSubCount = k;
            }
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_ENABLED)) {
            pstComponent[j].stTransceiver.bEnabled = pstValues[i].data.bool_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_IN_POWER_INST)) {
            pstComponent[j].stTransceiver.dInputPowerInstant = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_OUT_POWER_INST)) {
            pstComponent[j].stTransceiver.dOutputPowerInstant = pstValues[i].data.decimal64_val;
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_FPGA_VER)) {
            strncpy(pstComponent[j].stState.acFpgaVer, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFpgaVer) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_FUJITSU_VENDOR)) {
            strncpy(pstComponent[j].stState.acFjtVendorName, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFjtVendorName) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_FUJITSU_SN)) {
            strncpy(pstComponent[j].stState.acFjtSn, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acFjtSn) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_CLEI)) {
            strncpy(pstComponent[j].stState.acClei, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acClei) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_MODEL)) {
            strncpy(pstComponent[j].stState.acModel, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acModel) - 1);
        } else if (NULL != strstr(pstValues[i].xpath, DEF_LEAF_COMPONENT_CPLD_VER)) {
            strncpy(pstComponent[j].stState.acCpldVer, pstValues[i].data.string_val, sizeof(pstComponent[j].stState.acCpldVer) - 1);
        }
    }

#if 0
    /* size_t tSubCompntCount=0; */
    for (i = 0; i < tMaxCount; i++) {
        if (true == bIsInternal) {
            printf("true == bIsInternal\n");
            DSC_OCP_GetStateByName_I(pstComponent[i].acName, &pstComponent[i].stState);
            /* DSC_OCP_GetSubComponents_I(pstComponent[i].acName,(ST_OC_COMPONENT_SUBCOMPONENT **)(&pstComponent[i].stSubcomponent),&tSubCompntCount); */
        } else {
            DSC_OCP_GetComponentState(pstComponent[i].acName, &pstComponent[i].stState);
            DSC_OCP_GetCpuByName(pstComponent[i].acName, &pstComponent[i].stCpu);
            DSC_OCP_GetFanByName(pstComponent[i].acName, &pstComponent[i].stFan);
            DSC_OCP_GetPsuByName(pstComponent[i].acName, &pstComponent[i].stPowerSupply);
            /* DSC_OCP_GetSubComponents(pstComponent[i].acName,(ST_OC_COMPONENT_SUBCOMPONENT **)(&pstComponent[i].stSubcomponent),&tSubCompntCount); */
        }
    }
#endif
cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}


static int
__GetComponents(ST_OC_COMPONENT **ppstCpComponent, size_t *ptCompntCount)
{
    int rc = SR_ERR_OK;
    ST_OC_COMPONENT *pstComponent = NULL;

    if (NULL == ppstCpComponent ||
        NULL == ptCompntCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetComponentCount(ptCompntCount);
    if (rc != SR_ERR_OK) {
        if (SR_ERR_NOT_FOUND == rc) {
            rc = SR_ERR_OK;
            *ptCompntCount = 0;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Get component count error: %s", sr_strerror(rc));
        }
        goto cleanup;
    }
    /* printf("%s:%d ######## ptCompntCount=%d \n", __func__,__LINE__,(int)*ptCompntCount); */

    pstComponent = (ST_OC_COMPONENT *)malloc(sizeof(ST_OC_COMPONENT) * (*ptCompntCount));
    if (NULL == pstComponent) {
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    memset(pstComponent, 0x00, (sizeof(ST_OC_COMPONENT) * (*ptCompntCount)));

    rc = __GetComponentInfo(pstComponent, *ptCompntCount);
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstComponent);
        goto cleanup;
    }

    *ppstCpComponent = pstComponent;

cleanup:
    return rc;
}

int
DSC_OCP_GetComponents(ST_OC_COMPONENT **ppstCpComponent, size_t *ptCompntCount)
{
    return __GetComponents(ppstCpComponent, ptCompntCount);
}

static int
__GetAll(ST_OC_COMPONENT **ppCompnent, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    ST_OC_COMPONENT *pstComponentInfos = NULL;
    size_t tComponentsCount = 0;

    if (NULL == ppCompnent ||
        NULL == ptCount) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    rc = __GetComponents(&pstComponentInfos, &tComponentsCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_DEBUG, "component count: %lu", tComponentsCount);

    *ppCompnent = pstComponentInfos;
    *ptCount = tComponentsCount;
    return rc;
cleanup:
    DEF_SAFE_FREE(pstComponentInfos);
    return rc;
}

int
DSC_OCP_GetAll(ST_OC_COMPONENT **ppCompnent, size_t *ptCount)
{
    return __GetAll(ppCompnent, ptCount);
}

int
DSC_OCP_SetPsuEnable(char *pcComponentName, bool enabled)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == pcComponentName) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, OCP_XPATH_COMPONENT_POWER_SUPPLY_CONFIG_ENABLED, pcComponentName);
    rc = set_bool_item(sess, acXpath, enabled);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetBoolItem config psu: %s\n", sr_strerror(rc));

        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_OCP_Set_Psu_Enable DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:

    return rc;
}

int
DSC_OCP_GetSfpConfig(char *pcComponentName, bool *pEnabled)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    bool bEnabled = false;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pEnabled || NULL == pcComponentName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }
    if (sess != NULL) {
        /* sr_session_refresh(sess); */
    }

    snprintf(acXpath, sizeof(acXpath) - 1, OCP_XPATH_COMPONENT_TRANSCEIVER_CONFIG_ENABLED, pcComponentName);
    /* printf("acXpath:%s\n", acXpath); */
    rc = get_bool_item(sess, acXpath, &bEnabled);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }
    /* printf("bEnabled:%d\n", bEnabled); */
    *pEnabled = bEnabled;
    /* printf("enabled:%d\n", *pEnabled); */
ERR_LABEL:
    return rc;
}

int
DSC_OCP_SetSfpConfig(char *pcComponentName, bool bEnabled)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null, or sess is null %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    snprintf(acXpath, sizeof(acXpath) - 1, OCP_XPATH_COMPONENT_TRANSCEIVER_CONFIG_ENABLED, pcComponentName);
    rc = set_bool_item(sess, acXpath, bEnabled);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "set_bool_item: %s %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_SetSfpConfig DSC_Commit error %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_OCP_GetSfpAll(char *pcComponentName, ST_OC_COMPONENT_TRANSCEIVER *pstTransceiver)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    int i = 0;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == pcComponentName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }
    if (sess != NULL) {
        /* sr_session_refresh(sess); */
    }

    snprintf(acXpath, sizeof(acXpath) - 1, OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_ALL, pcComponentName);
    /* printf("acXpath:%s\n", acXpath); */
    rc = get_items(sess, acXpath, &pstValues, &tValuesCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_ENABLED)) {
            pstTransceiver->bEnabled = pstValues[i].data.bool_val;
            /* printf("pstTransceiver->bEnabled:%d\n", pstTransceiver->bEnabled); */
            LOG_WRITE(EN_LOG_DEBUG, "pstTransceiver->bEnabled:%d", pstTransceiver->bEnabled);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_IN_POWER_INST)) {
            pstTransceiver->dInputPowerInstant = pstValues[i].data.decimal64_val;
            /* printf("pstTransceiver->dInputPowerInstant:%f\n", pstTransceiver->dInputPowerInstant); */
            LOG_WRITE(EN_LOG_DEBUG, "pstTransceiver->dInputPowerInstant:%f", pstTransceiver->dInputPowerInstant);
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_COMPONENT_TRANSCEIVER_OUT_POWER_INST)) {
            pstTransceiver->dOutputPowerInstant = pstValues[i].data.decimal64_val;
            /* printf("pstTransceiver->dOutputPowerInstant:%f\n", pstTransceiver->dOutputPowerInstant); */
            LOG_WRITE(EN_LOG_DEBUG, "pstTransceiver->dOutputPowerInstant:%f", pstTransceiver->dOutputPowerInstant);
        }
    }

ERR_LABEL:
    sr_free_values(pstValues, tValuesCount);
    return rc;

}


int
DSC_OCP_GetOpticalPortPowerInfo(ST_OC_COMPONENT_PORT *pstPorts, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    int i;
    int j = -1;
    /* int iPortCount=0; */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acCfgKey[32] = {0};
    if (NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
#if 0
    pstSess = DSC_GetOperSession();
    if (true == bIsInternal) {
        rc = get_items(pstSess, COP_XPATH_COMPONENT_NAME_I, &pstValues, &tValuesCount);                    /* COP_XPATH_COMPONENT_ALL_I */
    } else {
        rc = get_items(pstSess, OCP_XPATH_COMPONENTS_NAME, &pstValues, &tValuesCount);                    /* OCP_XPATH_COMPONENTS_ALL */
    }
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    printf("tValuesCount = %d\n", (int)tValuesCount);
    for (i = 0; i < tValuesCount; i++) {
        printf("pstValues[%d].xpath=%s\n", (int)i, pstValues[i].xpath);
        printf("pstValues[%d].data.string_val = %s\n", (int)i, pstValues[i].data.string_val);
        if ( NULL != strstr(pstValues[i].data.string_val, COMPONENT_KEY_PORT_IN) ||
             NULL != strstr(pstValues[i].data.string_val, COMPONENT_KEY_PORT_OUT)) {
            strncpy(pstPorts[iPortCount].acName, pstValues[i].data.string_val, sizeof(pstPorts[iPortCount].acName) - 1);
            iPortCount++;
        }
    }
    printf("%s:%d ######## PortCount=%d \n", __func__, __LINE__, (int)iPortCount);
#endif
    memset(acXpath, 0x00, sizeof(acXpath));
    /* memcpy(acXpath, OCP_XPATH_COMPONENTLIST, strlen(OCP_XPATH_COMPONENTLIST)); */
    memcpy(acXpath, OCP_XPATH_COMPONENT_OPTICAL_PORT_LIST, strlen(OCP_XPATH_COMPONENT_OPTICAL_PORT_LIST));
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if ( SR_ERR_NOT_FOUND == rc) {                    /* if (SR_ERR_TIME_OUT == rc || SR_ERR_NOT_FOUND == rc) { */
        LOG_WRITE(EN_LOG_ERROR, "SR_ERR_TIME_OUT: %s, xpath: %s", sr_strerror(rc), acXpath);
        goto cleanup;
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {
        if (NULL != strstr(pstValues[i].xpath, LEAF_OPTICAL_PORT_INPUT_POWER_INSTANT)) {                    /* LEAF_OPTICAL_PORT_POWER_INSTANT */
            j++;
            if ( j >= OCP_COMPONENT_NUM_MAX) {
                break;                    /* goto cleanup; */
            }
            memset(acCfgKey, 0, sizeof(acCfgKey));
            rc = DSC_GetKeyValue((char *)pstValues[i].xpath, "component", "name", 32, acCfgKey);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_GetComponentCfgKey xpath: %s", (char *)pstValues[i].xpath);

                goto cleanup;
            }

            strncpy(pstPorts[j].acName, acCfgKey, sizeof(pstPorts[j].acName) - 1);
            pstPorts[j].acName[sizeof(pstPorts[j].acName) - 1] = '\0';

            pstPorts[j].stOptiaclPower.dInstant = pstValues[i].data.decimal64_val;

        } else if (NULL != strstr(pstValues[i].xpath, LEAF_OPTICAL_PORT_INPUT_POWER_LOW_SEC)) {
            pstPorts[j].stOptiaclPower.ulLowSecond = pstValues[i].data.uint64_val;
        } else if (NULL != strstr(pstValues[i].xpath, LEAF_OPTICAL_PORT_INPUT_POWER_LOS_SEC)) {
            pstPorts[j].stOptiaclPower.ulLosSecond = pstValues[i].data.uint64_val;
        }
    }
    *ptCount = j + 1;
cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

int
DSC_OCP_GetOpticalPortPowerOutInfo(ST_OC_COMPONENT_PORT *pstPorts, size_t *ptCount)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    sr_val_t *pstValues = NULL;
    size_t tValuesCount = 0;
    int i;
    int j = -1;
    /* int iPortCount=0; */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    char acCfgKey[32] = {0};
    if (NULL == ptCount) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
#if 0
    pstSess = DSC_GetOperSession();
    if (true == bIsInternal) {
        rc = get_items(pstSess, COP_XPATH_COMPONENT_NAME_I, &pstValues, &tValuesCount);                    /* COP_XPATH_COMPONENT_ALL_I */
    } else {
        rc = get_items(pstSess, OCP_XPATH_COMPONENTS_NAME, &pstValues, &tValuesCount);                    /* OCP_XPATH_COMPONENTS_ALL */
    }
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    printf("tValuesCount = %d\n", (int)tValuesCount);
    for (i = 0; i < tValuesCount; i++) {
        printf("pstValues[%d].xpath=%s\n", (int)i, pstValues[i].xpath);
        printf("pstValues[%d].data.string_val = %s\n", (int)i, pstValues[i].data.string_val);
        if ( NULL != strstr(pstValues[i].data.string_val, COMPONENT_KEY_PORT_IN) ||
             NULL != strstr(pstValues[i].data.string_val, COMPONENT_KEY_PORT_OUT)) {
            strncpy(pstPorts[iPortCount].acName, pstValues[i].data.string_val, sizeof(pstPorts[iPortCount].acName) - 1);
            iPortCount++;
        }
    }
    printf("%s:%d ######## PortCount=%d \n", __func__, __LINE__, (int)iPortCount);
#endif
    memset(acXpath, 0x00, sizeof(acXpath));
    /* memcpy(acXpath, OCP_XPATH_COMPONENTLIST, strlen(OCP_XPATH_COMPONENTLIST)); */
    memcpy(acXpath, OCP_XPATH_COMPONENT_OPTICAL_PORT_LIST, strlen(OCP_XPATH_COMPONENT_OPTICAL_PORT_LIST));
    pstSess = DSC_GetOperSession();
    rc = get_items(pstSess, acXpath, &pstValues, &tValuesCount);
    if ( SR_ERR_NOT_FOUND == rc) {                    /* { if (SR_ERR_TIME_OUT == rc || SR_ERR_NOT_FOUND == rc) { */
        LOG_WRITE(EN_LOG_ERROR, "SR_ERR_TIME_OUT: %s, xpath: %s", sr_strerror(rc), acXpath);
        goto cleanup;
    }
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s name, error: %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < tValuesCount; i++) {

        if (NULL != strstr(pstValues[i].xpath, LEAF_OPTICAL_PORT_OUTPUT_POWER_INSTANT)) {                    /* LEAF_OPTICAL_PORT_POWER_INSTANT */
            j++;
            if ( j >= OCP_COMPONENT_NUM_MAX) {
                break;                    /* goto cleanup; */
            }
            memset(acCfgKey, 0, sizeof(acCfgKey));
            rc = DSC_GetKeyValue((char *)pstValues[i].xpath, "component", "name", 32, acCfgKey);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_GetComponentCfgKey xpath: %s", (char *)pstValues[i].xpath);

                goto cleanup;
            }

            strncpy(pstPorts[j].acName, acCfgKey, sizeof(pstPorts[j].acName) - 1);
            pstPorts[j].acName[sizeof(pstPorts[j].acName) - 1] = '\0';
            pstPorts[j].stOptiaclPower.dInstant = pstValues[i].data.decimal64_val;

        }
    }
    *ptCount = j + 1;
cleanup:
    sr_free_values(pstValues, tValuesCount);
    return rc;
}

char *
DSC_OCP_LedStatusToString(uint32_t uiLedStatus, char *pcLedStatus, size_t nLen)
{

    if (NULL == pcLedStatus) {
        return NULL;
    }

    switch (uiLedStatus) {
    case DSC_LED_STATE_OFF:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_OFF_STR);
        break;

    case DSC_LED_STATE_RED:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_RED_STR);
        break;

    case DSC_LED_STATE_GREEN:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_GREEN_STR);
        break;

    case DSC_LED_STATE_AMBER:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_AMBER_STR);
        break;

    case DSC_LED_STATE_RED_BLINK:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_RED_FLASH_STR);
        break;

    case DSC_LED_STATE_GREEN_BLINK:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_GREEN_FLASH_STR);
        break;

    case DSC_LED_STATE_AMBER_BLINK:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_AMBER_FLASH_STR);
        break;

    case DSC_LED_STATE_RED_GREEN_BLINK:
#if 0
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_RED_GREEN_FLASH_STR);
#endif
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_LAMPTEST_STR);
        break;

    default:
        snprintf(pcLedStatus, nLen, "%s", DSC_EXT_LED_STATUS_ABNORMAL_STR);
        break;
    }

    return pcLedStatus;
}

int
DSC_RpcLedOperation(OCP_LED_OPERATION_ST *pstLedOper)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 4;

    sr_session_ctx_t *sess = DSC_GetRunningSession();

    if (NULL == pstLedOper) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = strdup(pstLedOper->bEnabled ? DSC_EXT_LED_OPERATION_ENABLED_STR : DSC_EXT_LED_OPERATION_DISABLED_STR);

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_LED_OPER_OPERTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_LED_OPER_OPERTION, sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[1].type = SR_UINT16_T;
    input[1].data.uint16_val = pstLedOper->usInterval;
    rc = sr_val_set_xpath(&input[1], RPC_XPATH_LED_OPER_INTERVAL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_LED_OPER_INTERVAL, sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[2].type = SR_STRING_T;
    input[2].data.string_val = strdup(pstLedOper->acSrcComponent);

    rc = sr_val_set_xpath(&input[2], RPC_XPATH_LED_OPER_SRC_COMPONENT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_LED_OPER_SRC_COMPONENT, sr_strerror(rc));
        goto ERR_LABEL;
    }

    input[3].type = SR_STRING_T;
    input[3].data.string_val = strdup(pstLedOper->acSrcLed);

    rc = sr_val_set_xpath(&input[3], RPC_XPATH_LED_OPER_SRC_LED);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_LED_OPER_SRC_LED, sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_RpcSend(sess, RPC_XPATH_LED_OPERTION, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_LED_OPERTION, sr_strerror(rc));
        goto ERR_LABEL;
    }

    sr_free_values(input, input_cnt);

    if (strcmp(output[0].data.string_val, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[0].xpath, output[0].data.string_val);
        rc = SR_ERR_OPERATION_FAILED;
    }

    sr_free_values(output, output_cnt);

    return rc;

ERR_LABEL:
    sr_free_values(input, input_cnt);
    return rc;

}

/* NOTE: free memory by caller */
int
DSC_OCP_GetAllLedState(OCP_LED_LIST_ST *pstLedList)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0, i = 0;
    int iIdx = -1;
    sr_session_ctx_t *pstSess = DSC_GetOperSession();

    if (NULL == pstLedList) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    memset(pstLedList, 0, sizeof(OCP_LED_LIST_ST));

    rc = get_items(pstSess, OCP_XPATH_LEDS_ALL, &pstVal, &nCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        goto END_LABEL;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "%s, nCnt: %ld", OCP_XPATH_LEDS_ALL, nCnt); */

    if (0 == nCnt || nCnt % 4 != 0) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid nCnt: %ld", nCnt);
        return rc;
    }

    pstLedList->nTotal = nCnt / 4;
    pstLedList->pstLedState = (OCP_LED_STATE_ST *)malloc(pstLedList->nTotal * sizeof(OCP_LED_STATE_ST));

    if (NULL == pstLedList->pstLedState) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid nCnt: %ld", nCnt);
        rc = ERR_NOMEM;
        goto END_LABEL;
    }
    memset(pstLedList->pstLedState, 0, pstLedList->nTotal * sizeof(OCP_LED_STATE_ST));
    iIdx = -1;
    for (i = 0; i < nCnt; i++) {
        if (SR_LIST_T == pstVal[i].type) {
            iIdx++;
            continue;
        }

        if (iIdx < 0 || iIdx >= pstLedList->nTotal) {
            rc = ERR_NOMEM;
            LOG_WRITE(EN_LOG_ERROR, "over range, iIdx:%d, nTotal: %ld", iIdx, pstLedList->nTotal);
            goto END_LABEL;
        }

        if (sr_xpath_node_name_eq(pstVal[i].xpath, COMPONENT_STATE_LED_NAME)) {
            snprintf(pstLedList->pstLedState[iIdx].acName, sizeof(pstLedList->pstLedState[iIdx].acName),
                     "%s", pstVal[i].data.string_val);
        } else if (sr_xpath_node_name_eq(pstVal[i].xpath, COMPONENT_STATE_LED_STATUS)) {

            snprintf(pstLedList->pstLedState[iIdx].acStatus, sizeof(pstLedList->pstLedState[iIdx].acStatus),
                     "%s", pstVal[i].data.enum_val);
        } else if (sr_xpath_node_name_eq(pstVal[i].xpath, COMPONENT_STATE_LED_FUCTION)) {
            snprintf(pstLedList->pstLedState[iIdx].acFuction, sizeof(pstLedList->pstLedState[iIdx].acFuction),
                     "%s", pstVal[i].data.string_val);
        }
    }

END_LABEL:
    if (rc != SR_ERR_OK) {
        DEF_SAFE_FREE(pstLedList->pstLedState);
    }

    sr_free_values(pstVal, nCnt);
    return rc;
}

bool
DSC_OCP_IsLamptestRunning()
{

    if (access(FILE_LAMPTEST_RUNNING_FLAG, F_OK) != 0) {

        return false;
    }

    return true;
}

int
DSC_OCP_SetLamptestFlag(bool bEnable)
{
    bool bLamptest = false;
    char acCmdBuf[DEF_BUF_SIZE_256] = {0};

    bLamptest = DSC_OCP_IsLamptestRunning();

    if (bEnable == bLamptest) {

        return OPLK_OK;
    }

    snprintf(acCmdBuf, sizeof(acCmdBuf), "%s %s", bEnable ? "echo > " : "rm -f ",
             FILE_LAMPTEST_RUNNING_FLAG);

    return UTIL_System(acCmdBuf);
}

static uint8_t
__GetChassisType()
{
    size_t nItemsCount = 0;

    if (m_ucChassisType < EN_CHASSIS_TYPE_CNT) {
        return m_ucChassisType;
    }

    /* This is NOT a good way to identy chassis type. TODO: optimize */
    if (DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OPS) && DSC_GetOpsNames(NULL, 0, &nItemsCount) == SR_ERR_OK) {
        m_ucChassisType = nItemsCount > 1 ? EN_CHASSIS_TYPE_OPS : EN_CHASSIS_TYPE_AMP;
    } else if (DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_OCS)) {
        m_ucChassisType = EN_CHASSIS_TYPE_OCS;
    } else {
        m_ucChassisType = EN_CHASSIS_TYPE_AMP;
    }

    LOG_WRITE(EN_LOG_NOTICE, "m_ucChassisType: %d", m_ucChassisType);
    return m_ucChassisType;
}

static int
__GetLedList(ST_COMPONENT_LED_INFO **ppstLedList, size_t *pnLedCnt)
{
    uint8_t ucChassisType = __GetChassisType();

    if (NULL == ppstLedList || NULL == pnLedCnt) {
        return SR_ERR_INVAL_ARG;
    }

    if (EN_CHASSIS_TYPE_AMP == ucChassisType || EN_CHASSIS_TYPE_OCS == ucChassisType) {
        *ppstLedList = (ST_COMPONENT_LED_INFO *)m_stComponentLedInfo;
        *pnLedCnt = sizeof(m_stComponentLedInfo) / sizeof(ST_COMPONENT_LED_INFO);
    } else if (EN_CHASSIS_TYPE_OPS == ucChassisType) {
        *ppstLedList = (ST_COMPONENT_LED_INFO *)m_stOps4ComponentLedInfo;
        *pnLedCnt = sizeof(m_stOps4ComponentLedInfo) / sizeof(ST_COMPONENT_LED_INFO);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Unknown ChassisType: %d", ucChassisType);
        return SR_ERR_INVAL_ARG;
    }

    return SR_ERR_OK;
}

int
DSC_LedNameToIndex(const char *pcComponentName, const char *pcLedStr, uint32_t *pulLedIndex)
{
    size_t i = 0;
    size_t nLedCnt = 0;
    ST_COMPONENT_LED_INFO *pstComponentLedInfo = NULL;

    /* Check input arguments */
    if ( NULL == pcComponentName || NULL == pcLedStr || NULL == pulLedIndex) {
        return SR_ERR_INVAL_ARG;
    }

    if (SR_ERR_OK != __GetLedList(&pstComponentLedInfo, &nLedCnt)) {
        return SR_ERR_INVAL_ARG;
    }

    /* Find index by component name and LED name. */
    for (i = 0; i < nLedCnt; i++) {
        if (strcmp(pstComponentLedInfo[i].pcComponentName, pcComponentName) == 0 &&
            strcmp(pstComponentLedInfo[i].pcItemName, pcLedStr) == 0) {
            *pulLedIndex = pstComponentLedInfo[i].ulItemIndex;
            return SR_ERR_OK;
        }
    }

    /* Error, cannot find the index. */
    return SR_ERR_INVAL_ARG;
}

char *
DSC_LedIndexToName(const char *pcComponentName, uint32_t ulLedIndex, char *pcLedName, size_t nLen)
{
    size_t i = 0;
    size_t nLedCnt = 0;
    ST_COMPONENT_LED_INFO *pstComponentLedInfo = NULL;

    /* Check input arguments */
    if ( NULL == pcComponentName || NULL == pcLedName) {
        return NULL;
    }

    if (SR_ERR_OK != __GetLedList(&pstComponentLedInfo, &nLedCnt)) {
        return NULL;
    }

    /* Find LED name by component name and LED index. */
    for (i = 0; i < nLedCnt; i++) {
        /*LOG_WRITE(EN_LOG_NOTICE, "i: %d", i);*/
        /*LOG_WRITE(EN_LOG_NOTICE, "%s , %s ,  %d , %d  ", m_stComponentLedInfo[i].pcComponentName \
                  , pcComponentName, m_stComponentLedInfo[i].ulItemIndex, ulLedIndex);*/
        if (strcmp(pstComponentLedInfo[i].pcComponentName, pcComponentName) == 0 &&
            pstComponentLedInfo[i].ulItemIndex == ulLedIndex) {
            snprintf(pcLedName, nLen, "%s", pstComponentLedInfo[i].pcItemName);
            return pcLedName;
        }
    }

    /* Error, cannot find the LED. */
    return NULL;
}

static void
__RefreshComponentSlot(ST_DSC_COMPONENT_INFO *list, size_t count)
{
    static bool m_bRefresh = false;
    int iSlotIndex = 0;

    if (m_bRefresh) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        if (DSC_GetSlotIndexByCpKey(list[i].pcComponentName, &iSlotIndex) == OPLK_OK) {
            list[i].ulSlotIndex = iSlotIndex;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "Refresh components slot information finished.");

    m_bRefresh = true;
}

static int
__GetComponentInfoList(ST_DSC_COMPONENT_INFO **ppstComponentInfoList, size_t *pnComponentInfoCnt)
{
    uint8_t ucChassisType = __GetChassisType();

    if (NULL == ppstComponentInfoList || NULL == pnComponentInfoCnt) {
        return SR_ERR_INVAL_ARG;
    }

    if ((EN_CHASSIS_TYPE_AMP == ucChassisType) || (EN_CHASSIS_TYPE_OCS == ucChassisType)) {
        *ppstComponentInfoList = (ST_DSC_COMPONENT_INFO *)m_stComponentDevInfo;
        *pnComponentInfoCnt = sizeof(m_stComponentDevInfo) / sizeof(ST_DSC_COMPONENT_INFO);
    } else if (EN_CHASSIS_TYPE_OPS == ucChassisType) {
        *ppstComponentInfoList = (ST_DSC_COMPONENT_INFO *)m_stOps4ComponentDevInfo;
        *pnComponentInfoCnt = sizeof(m_stOps4ComponentDevInfo) / sizeof(ST_DSC_COMPONENT_INFO);
    } else {
        *pnComponentInfoCnt = 0;
        LOG_WRITE(EN_LOG_ERROR, "Unknown ChassisType: %d", ucChassisType);
        return SR_ERR_INVAL_ARG;
    }

    __RefreshComponentSlot(*ppstComponentInfoList, *pnComponentInfoCnt);

    return SR_ERR_OK;
}

uint32_t
DSC_GetComponentInfoCount()
{
    size_t nComponentInfoCnt = 0;
    ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    __GetComponentInfoList(&pstComponentInfo, &nComponentInfoCnt);

    return (uint32_t)nComponentInfoCnt;
}

const ST_DSC_COMPONENT_INFO *
DSC_GetComponentInfoByName(const char *pcComponentName)
{
    size_t i = 0;
    size_t nComponentInfoCnt = 0;
    ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    /* Check input arguments */
    if ( NULL == pcComponentName) {
        return NULL;
    }

    if (SR_ERR_OK != __GetComponentInfoList(&pstComponentInfo, &nComponentInfoCnt)) {
        return NULL;
    }

    /* Find slot index by component name. */
    for (i = 0; i < nComponentInfoCnt; i++) {
        if (strcmp(pstComponentInfo[i].pcComponentName, pcComponentName) == 0) {
            return pstComponentInfo + i;
        }
    }

    /* Error, cannot find the index. */
    return NULL;
}

const ST_DSC_COMPONENT_INFO *
DSC_GetComponentInfoByIndex(uint32_t ulIndex)
{
    size_t nComponentInfoCnt = 0;
    ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    if (SR_ERR_OK != __GetComponentInfoList(&pstComponentInfo, &nComponentInfoCnt)) {
        return NULL;
    }

    /* Check the index. */
    if (ulIndex >= nComponentInfoCnt) {
        return NULL;
    }

    return pstComponentInfo + ulIndex;
}

const ST_DSC_COMPONENT_INFO *
DSC_GetComponentInfoBySlotIndex(uint32_t ulSlotIndex)
{
    size_t nComponentInfoCnt = 0;
    ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    if (SR_ERR_OK != __GetComponentInfoList(&pstComponentInfo, &nComponentInfoCnt)) {
        return NULL;
    }

    /* Find LED name by component name and LED index. */
    for (uint32_t i = 0; i < nComponentInfoCnt; i++) {
        if (pstComponentInfo[i].ulSlotIndex == ulSlotIndex) {
            return pstComponentInfo + i;
        }
    }

    /* Error, cannot find the LED. */
    return NULL;
}

bool
DSC_OCP_IsComponentExist(const char *pcComponentName)
{
    if (NULL == pcComponentName) {
        return false;
    }

    sr_val_t *_v = NULL;
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    snprintf(acXpath, sizeof(acXpath), OCP_XPATH_COMPONENT_CONFIG_NAME, pcComponentName);
    rc = sr_get_item(DSC_GetRunningSession(), acXpath, 0, &_v);
    sr_free_val(_v);

    return SR_ERR_OK == rc;
}
