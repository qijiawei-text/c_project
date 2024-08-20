/*
    This file is to define by org-openroadm-physical-types.yang
 */

#ifndef _ORG_OPENROADM_PHYSICAL_TYPES_H_
#define _ORG_OPENROADM_PHYSICAL_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "org_openroadm_common_state_types.h"

/*
 * /org-openroadm-physical-types:node-info
 */
#define NODE_INFO_VENDOR          "/vendor"
#define NODE_INFO_MODEL           "/model"
#define NODE_INFO_SERIAL_ID       "/serial-id"

typedef struct
{
    char acVendor[128];
    char acModel[128];
    char acSN[128];
}ST_PHYSICAL_NODE_INFO;

/*
 * /org-openroadm-physical-types:common-info
 */
#define COMMON_INFO_TYPE           "/type"
#define COMMON_INFO_PRODUCT_CODE   "/product-code"
#define COMMON_INFO_MFG_DATE       "/manufacture-date"
#define COMMON_INFO_CLEI           "/clei"
#define COMMON_INFO_HW_VER         "/hardware-version"
#define COMMON_INFO_OPRATNAL_STATE "/operational-state"
#define COMMON_INFO_RESET_CAUSE    "/reset-cause"

/* org-openroadm-physical-types:common-info/reset-cause enumeration */
#define EN_COMMON_INFO_RESET_CAUSE_POWER_STR  "power"
#define EN_COMMON_INFO_RESET_CAUSE_WARM_STR   "warm"
#define EN_COMMON_INFO_RESET_CAUSE_COLD_STR   "cold"
#define EN_COMMON_INFO_RESET_CAUSE_WATCHDOG_STR "watchdog"
#define EN_COMMON_INFO_RESET_CAUSE_BUTTON_STR "button"

typedef enum
{
    EN_COMMON_INFO_RESET_CAUSE_POWER = 0,
    EN_COMMON_INFO_RESET_CAUSE_WARM,
    EN_COMMON_INFO_RESET_CAUSE_COLD,
    EN_COMMON_INFO_RESET_CAUSE_WATCHDOG,
    EN_COMMON_INFO_RESET_CAUSE_BUTTON,
    EN_COMMON_INFO_RESET_CAUSE_MAX,
} EN_COMMON_INFO_RESET_CAUSE;

typedef struct
{
    char acType[32];
    char acProductCode[128];
    char acMfgDate[128];
    char acClei[128];
    char acHwVer[128];
    EN_STATE enOperationalState;
    EN_COMMON_INFO_RESET_CAUSE enResetCause;
}ST_PHYSICAL_COMMON_INFO;

typedef struct
{
    char acUnitName[128];
    char acFcNumber[128];
    char acPiuIssueNumber[128];
    char acFujitsuPartNumber[128];
}ST_PHYSICAL_FUJITSU_INFO;

typedef struct
{
    ST_PHYSICAL_NODE_INFO stNode;
    ST_PHYSICAL_COMMON_INFO stCommon;
    ST_PHYSICAL_FUJITSU_INFO stFujitsu;
}ST_PHYSICAL_INFO;

#ifdef __cplusplus
}
#endif

#endif
