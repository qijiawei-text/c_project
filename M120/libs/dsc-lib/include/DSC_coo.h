/*
    This file is to define by com-oplink-optical.yang
 */

#ifndef _DSC_COO_H_
#define _DSC_COO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define OPTICAL_CFG_PARENT_MAX_NUM    16

#define MODULE_COO        "com-oplink-optical"
#define COO_XPATH_PREFIX  "/" MODULE_COO ":optical-control"

/*
 * /com-oplink-optical:optical-control/work-mode
 */
#define LEAF_WORK_MODE       "/work-mode"
#define COO_XPATH_WORKMODE   COO_XPATH_PREFIX LEAF_WORK_MODE

#define LEAF_RESET_CAUSE     "/reset-cause"

#define OPTICAL_CFG_PARENT_KEY                   "/config/name"
#define OPTICAL_CFG_PARENT_CP_NAME               "/circuit-pack-name"
#define OPTICAL_CFG_PARENT_COMPONENT_NAME        "/component-name"


#define OPS_CFG_PARENT_XPATH_HEADER     COO_XPATH_PREFIX "/optical-modules/aps"
#define OPS_CFG_PARENT_CP_PATH          OPS_CFG_PARENT_XPATH_HEADER "/aps-modules/aps-module[name='%s']" OPTICAL_CFG_PARENT_CP_NAME
#define OPS_CFG_PARENT_COMPONENT_PATH   OPS_CFG_PARENT_XPATH_HEADER "/aps-modules/aps-module[name='%s']" OPTICAL_CFG_PARENT_COMPONENT_NAME


#define EDFA_CFG_PARENT_XPATH_HEADER    COO_XPATH_PREFIX "/optical-modules/optical-amplifier"
#define EDFA_CFG_PARENT_CP_PATH         EDFA_CFG_PARENT_XPATH_HEADER "/amplifiers/amplifier[name='%s']" OPTICAL_CFG_PARENT_CP_NAME
#define EDFA_CFG_PARENT_COMPONENT_PATH  EDFA_CFG_PARENT_XPATH_HEADER "/amplifiers/amplifier[name='%s']" OPTICAL_CFG_PARENT_COMPONENT_NAME


#define WSS_CFG_PARENT_XPATH_HEADER     COO_XPATH_PREFIX "/optical-modules/wavelength-router"
#define WSS_CFG_PARENT_CP_PATH          WSS_CFG_PARENT_XPATH_HEADER "/wss-modules/wss-module[name='%s']" OPTICAL_CFG_PARENT_CP_NAME
#define WSS_CFG_PARENT_COMPONENT_PATH   WSS_CFG_PARENT_XPATH_HEADER "/wss-modules/wss-module[name='%s']" OPTICAL_CFG_PARENT_COMPONENT_NAME


#define TDCM_CFG_PARENT_XPATH_HEADER    COO_XPATH_PREFIX "/optical-modules/com-oplink-tdcm:optical-tdcm"
#define TDCM_CFG_PARENT_CP_PATH         TDCM_CFG_PARENT_XPATH_HEADER "/tdcms[name='%s']" OPTICAL_CFG_PARENT_CP_NAME
#define TDCM_CFG_PARENT_COMPONENT_PATH  TDCM_CFG_PARENT_XPATH_HEADER "/tdcms[name='%s']" OPTICAL_CFG_PARENT_COMPONENT_NAME


#define OCM_CFG_PARENT_XPATH_HEADER     COO_XPATH_PREFIX "/optical-modules/channel-monitors"
#define OCM_CFG_PARENT_CP_PATH          OCM_CFG_PARENT_XPATH_HEADER "/ocm-modules/ocm-module[name='%s']" OPTICAL_CFG_PARENT_CP_NAME
#define OCM_CFG_PARENT_COMPONENT_PATH   OCM_CFG_PARENT_XPATH_HEADER "/ocm-modules/ocm-module[name='%s']" OPTICAL_CFG_PARENT_COMPONENT_NAME

#define MODULE_COO_I        "com-oplink-optical-i"
#define COO_I_XPATH_PREFIX  "/com-oplink-optical-i:optical-control"

#define COO_I_OPTICAL_MODULE_CIRCUIT_PACK_NAME   "circuit-pack-name"
#define COO_I_OPTICAL_MODULE_COMPONENT_NAME      "component-name"
#define COO_I_OPTICAL_MODULE_COMPONENT_TYPE      "component-type"
#define COO_I_OPTICAL_MODULE_COMPONENT_INDEX     "component-index"


#define SHUTTER_ON                              "/shutter-on"
#define INTER_LOCK_ON                           "/inter-lock-on"
#define ADMIN_KEY_ON                            "/admin-key-on"
#define LC_PRESENT                              "/lc-present"
#define LC_DOOR_LOCK                            "/lc-door-lock"
#define REMOTE_INTERLOCK_ENABLE                 "/remote-interlock-enable"
#define DUMMY_ADMIN_KEY_ENABLE                  "/dummy-admin-key-enable"
#define PANEL_SAFE_CMPT_ENABLE                  "/panel-safe-cmpt-enable"

#if 0
/* move to dsc_lasersafety*/
#define LASERSAFETY_CFG_PARENT_XPATH_HEADER     COO_XPATH_PREFIX "/laser-safty"
#define LASERSAFETY_CFG_CONFIG                  LASERSAFETY_CFG_PARENT_XPATH_HEADER "/config"
#define LASERSAFETY_XPATH_CFG_ALL               LASERSAFETY_CFG_CONFIG "//*"

#define LASERSAFETY_CFG_SHUTTER_ON              LASERSAFETY_CFG_CONFIG SHUTTER_ON
#define LASERSAFETY_CFG_REMOTE_INTERLOCK_ENABLE LASERSAFETY_CFG_CONFIG REMOTE_INTERLOCK_ENABLE
#define LASERSAFETY_CFG_DUMMY_ADMIN_KEY_ENABLE  LASERSAFETY_CFG_CONFIG DUMMY_ADMIN_KEY_ENABLE
#define LASERSAFETY_CFG_PANEL_SAFE_CMPT_ENABLE  LASERSAFETY_CFG_CONFIG PANEL_SAFE_CMPT_ENABLE

#define LASERSAFETY_STATE                       LASERSAFETY_CFG_PARENT_XPATH_HEADER "/state"
#define LASERSAFETY_XPATH_STATE_ALL             LASERSAFETY_STATE "//*"
#define LASERSAFETY_STATE_INTER_LOCK_ON         LASERSAFETY_STATE INTER_LOCK_ON
#define LASERSAFETY_STATE_ADMIN_KEY_ON          LASERSAFETY_STATE ADMIN_KEY_ON
#define LASERSAFETY_STATE_LC_PRESENT            LASERSAFETY_STATE LC_PRESENT
#define LASERSAFETY_STATE_LC_DOOR_LOCK          LASERSAFETY_STATE LC_DOOR_LOCK
#define LASERSAFETY_STATE_SHUTTER_ON            LASERSAFETY_STATE SHUTTER_ON
#endif

/*
 * /com-oplink-optical:optical-control/com-oplink-optical:optical-modules
 */
#define COO_XPATH_OPTICAL_MOD COO_XPATH_PREFIX "/optical-modules"

typedef struct {
    char acCircuitPackName[32];
    char acComponentName[32];
    char acComponentType[64];
    uint32_t uiComponentIndex;
} OPTICAL_MODULE_DESC_ST;


#ifdef __cplusplus
}
#endif

#endif
