/*
    This file is to define by org-openroadm-device.yang
 */

#ifndef _DSC_OOD_H_
#define _DSC_OOD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "org_openroadm_physical_types.h"
#include "org_openroadm_equipment_states_types.h"

/* org-openroadm-device:reset-type enumeration */
typedef enum en_OOD_RESET_TYPE
{
    OOD_RESET_TYPE_POWER = 0,
    OOD_RESET_TYPE_COLD,
    OOD_RESET_TYPE_WARM,
    OOD_RESET_TYPE_BUTTON,
    OOD_RESET_TYPE_WATCHDOG,
    OOD_RESET_TYPE_MAX
} EN_OOD_RESET_TYPE;

/* org-openroadm-device:system-mode enumeration */
typedef enum en_OOD_SYSTEM_MODE
{
    OOD_SYSTEM_MODE_AUTO = 0,
    OOD_SYSTEM_MODE_MANUAL,
    OOD_SYSTEM_MODE_DEBUG,
    OOD_SYSTEM_MODE_MAX
} EN_OOD_SYSTEM_MODE;

/* org-openroadm-device:slot-status enumeration */
typedef enum en_OOD_SLOT_STATUS
{
    OOD_SLOT_STATUS_NONE = 0,
    OOD_SLOT_STATUS_EMPTY_NOT_PROV,
    OOD_SLOT_STATUS_EMPTY_PROV_MATCH,
    OOD_SLOT_STATUS_EMPTY_PROV_MISMATCH,
    OOD_SLOT_STATUS_INSTALLED_NOT_PROV,
    OOD_SLOT_STATUS_INSTALLED_PROV_MATCH,
    OOD_SLOT_STATUS_INSTALLED_PROV_MISMATCH,
    OOD_SLOT_STATUS_MAX
} EN_OOD_SLOT_STATUS;

#define MODULE_OOD        "org-openroadm-device"
#define OOD_XPATH_PREFIX  "/" MODULE_OOD ":org-openroadm-device"

/*
 * /org-openroadm-device:org-openroadm-device/info
 */
#define OOD_XPATH_INFO    OOD_XPATH_PREFIX "/info"
#define LEAF_NODE_ID      "/node-id"
#define LEAF_NODE_NUMBER  "/node-number"
#define LEAF_NODE_TYPE    "/node-type"
#define LEAF_CLLI         "/clli"  /* Common Language Location Identifier */
#define LEAF_VENDOR       NODE_INFO_VENDOR
#define LEAF_MODEL        NODE_INFO_MODEL
#define LEAF_SERIAL_ID    NODE_INFO_SERIAL_ID
#define LEAF_SW_VER       "/softwareVersion"  /* bundle version */
#define LEAF_SW_VER_BUILD "/software-build"
#define LEAF_LIFECYCLE    "/lifecycle-state"
#define LEAF_SYS_MODE     "/system-mode"
#define LEAF_RESET_CAUSE  "/reset-cause"
#define LEAF_GETLOCATION_LATITUDE "/geoLocation/latitude"
#define LEAF_GETLOCATION_LONGITUDE "/geoLocation/longitude"
#define OOD_XPATH_NODE_ID      OOD_XPATH_INFO LEAF_NODE_ID
#define OOD_XPATH_NODE_NUMBER  OOD_XPATH_INFO LEAF_NODE_NUMBER
#define OOD_XPATH_NODE_TYPE    OOD_XPATH_INFO LEAF_NODE_TYPE
#define OOD_XPATH_CLLI         OOD_XPATH_INFO LEAF_CLLI
#define OOD_XPATH_VENDOR       OOD_XPATH_INFO LEAF_VENDOR
#define OOD_XPATH_MODEL        OOD_XPATH_INFO LEAF_MODEL
#define OOD_XPATH_SERIAL_ID    OOD_XPATH_INFO LEAF_SERIAL_ID
#define OOD_XPATH_SW_VER       OOD_XPATH_INFO LEAF_SW_VER
#define OOD_XPATH_SW_VER_BUILD OOD_XPATH_INFO LEAF_SW_VER_BUILD
#define OOD_XPATH_SW_LIFECYCLE OOD_XPATH_INFO LEAF_LIFECYCLE
#define OOD_XPATH_SW_SYS_MODE  OOD_XPATH_INFO LEAF_SYS_MODE
#define OOD_XPATH_RESET_CAUSE  OOD_XPATH_INFO LEAF_RESET_CAUSE
#define OOD_XPATH_GETLOCATION_LATITUDE  OOD_XPATH_INFO LEAF_GETLOCATION_LATITUDE
#define OOD_XPATH_GETLOCATION_LONGITUDE  OOD_XPATH_INFO LEAF_GETLOCATION_LONGITUDE

#define OOD_XPATH_LIST_INTERFACE OOD_XPATH_INFO "/ip-interfaces/interface"
#define OOD_XPATH_INTERFACE_ALL  OOD_XPATH_LIST_INTERFACE "//*"
#define KEY_INTERFACE            "[name"
#define LEAF_INTERFACE_NAME      "/name"
#define LEAF_INTERFACE_ENABLED   "/enabled"
#define LEAF_V4_IP_ADDR          "/ipv4/ipAddress"
#define LEAF_V4_NETMASK          "/ipv4/netmask"
#define LEAF_V4_GATEWAY          "/ipv4/defaultGateway"
#define LEAF_V6_IP_ADDR          "/ipv6/ipAddress"
#define LEAF_V6_PREFIX_LEN       "/ipv6/prefix-length"
#define LEAF_V6_GATEWAY          "/ipv6/defaultGateway"
#define OOD_XPATH_V4_IP_ADDR     OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V4_IP_ADDR
#define OOD_XPATH_V4_NETMASK     OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V4_NETMASK
#define OOD_XPATH_V4_GATEWAY     OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V4_GATEWAY
#define OOD_XPATH_V6_IP_ADDR     OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V6_IP_ADDR
#define OOD_XPATH_V6_PREFIX_LEN  OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V6_PREFIX_LEN
#define OOD_XPATH_V6_GATEWAY     OOD_XPATH_LIST_INTERFACE "[name='eth%d']" LEAF_V6_GATEWAY

#define OOD_XPATH_LIST_MAC     OOD_XPATH_INFO "/macAddrs"
#define OOD_XPATH_LIST_MAC_ALL OOD_XPATH_LIST_MAC "//*"
#define KEY_MAC            "[name"
#define LEAF_MAC_NAME      "/name"
#define LEAF_MAC_ADDR      "/macAddress"
#define OOD_XPATH_MAC_ADDR OOD_XPATH_LIST_MAC "[name='eth%d']" LEAF_MAC_ADDR

#define OOD_XPATH_GEOLOCATION OOD_XPATH_INFO "/geoLocation"
#define LEAF_LATITUDE         "/latitude"
#define LEAF_LONGITUDE        "/longitude"
#define OOD_XPATH_LATITUDE    OOD_XPATH_INFO LEAF_LATITUDE
#define OOD_XPATH_LONGITUDE   OOD_XPATH_INFO LEAF_LONGITUDE

/*
 * /org-openroadm-device:org-openroadm-device/pending-sw
 */
#define OOD_XPATH_PEND_SW               OOD_XPATH_PREFIX "/pending-sw"
#define LEAF_PEND_SW_VER                "/sw-version"
#define LEAF_PEND_SW_VALIDTN_TIMER      "/sw-validation-timer"
#define LEAF_PEND_SW_ACTIVATE_DT        "/activation-date-time"
#define OOD_XPATH_PEND_SW_VER           OOD_XPATH_PEND_SW LEAF_PEND_SW_VER
#define OOD_XPATH_PEND_SW_VALIDTN_TIMER OOD_XPATH_PEND_SW LEAF_PEND_SW_VALIDTN_TIMER
#define OOD_XPATH_PEND_SW_ACTIVATE_DT   OOD_XPATH_PEND_SW LEAF_PEND_SW_ACTIVATE_DT

/*
 * /org-openroadm-device:org-openroadm-device/database-info
 */
#define OOD_XPATH_DB_INFO             OOD_XPATH_PREFIX "/database-info"
#define LEAF_LAST_RESTORE_TIME        "/last-restored-time"
#define LEAF_ROLLBACK_TIMER           "/rollback-timer"
#define LEAF_DB_INFO_ACTIVATE_DT      "/activation-date-time"
#define OOD_XPATH_LAST_RESTORE_TIME   OOD_XPATH_DB_INFO LEAF_LAST_RESTORE_TIME
#define OOD_XPATH_ROLLBACK_TIMER      OOD_XPATH_DB_INFO LEAF_ROLLBACK_TIMER
#define OOD_XPATH_DB_INFO_ACTIVATE_DT OOD_XPATH_DB_INFO LEAF_DB_INFO_ACTIVATE_DT

/*
 * /org-openroadm-device:org-openroadm-device/shelves[shelf-name]
 */
#define OOD_XPATH_LIST_SHELVES        OOD_XPATH_PREFIX "/shelves"
#define OOD_XPATH_LIST_SHELVES_ALL    OOD_XPATH_LIST_SHELVES "//*"
#define KEY_SHELF                     "[shelf-name"
#define LEAF_SHELF_NAME               "/shelf-name"
#define LEAF_SHELF_TYPE               "/shelf-type"
#define LEAF_SHELF_RACK               "/rack"
#define LEAF_SHELF_POSITION           "/shelf-position"
#define LEAF_SHELF_LIFECYCLE          LEAF_LIFECYCLE
#define LEAF_SHELF_ADMIN_STATE        "/administrative-state"
#define LEAF_SHELF_VENDOR             NODE_INFO_VENDOR
#define LEAF_SHELF_MODEL              NODE_INFO_MODEL
#define LEAF_SHELF_SERIAL_ID          NODE_INFO_SERIAL_ID
#define LEAF_SHELF_COMMON_TYPE        COMMON_INFO_TYPE
#define LEAF_SHELF_PRODUCT_CODE       COMMON_INFO_PRODUCT_CODE
#define LEAF_SHELF_MFG_DATE           COMMON_INFO_MFG_DATE
#define LEAF_SHELF_CLEI               COMMON_INFO_CLEI
#define LEAF_SHELF_HW_VER             COMMON_INFO_HW_VER
#define LEAF_SHELF_OPRATNAL_STATE     COMMON_INFO_OPRATNAL_STATE
#define LEAF_SHELF_RESET_CAUSE        COMMON_INFO_RESET_CAUSE
#define LEAF_SHELF_EQPMT_STATE        "/equipment-state"
#define LEAF_SHELF_IS_PHYSICAL        "/is-physical"
#define LEAF_SHELF_IS_PASSIVE         "/is-passive"
#define LEAF_SHELF_FACEPTE_LABEL      "faceplate-label"
#define LEAF_SHELF_DUE_DATE           "due-date"

#define OOD_XPATH_SHELF_NAME           OOD_XPATH_LIST_SHELVES LEAF_SHELF_NAME
#define OOD_XPATH_SHELF_ALL            OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "//*"
#define OOD_XPATH_SHELF_TYPE           OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_TYPE
#define OOD_XPATH_SHELF_RACK           OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_RACK
#define OOD_XPATH_SHELF_POSITION       OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_POSITION
#define OOD_XPATH_SHELF_LIFECYCLE      OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_LIFECYCLE
#define OOD_XPATH_SHELF_ADMIN_STATE    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_ADMIN_STATE
#define OOD_XPATH_SHELF_VENDOR         OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_VENDOR
#define OOD_XPATH_SHELF_MODEL          OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_MODEL
#define OOD_XPATH_SHELF_SERIAL_ID      OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_SERIAL_ID
#define OOD_XPATH_SHELF_COMMON_TYPE    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_COMMON_TYPE
#define OOD_XPATH_SHELF_PRODUCT_CODE   OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_PRODUCT_CODE
#define OOD_XPATH_SHELF_MFG_DATE       OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_MFG_DATE
#define OOD_XPATH_SHELF_CLEI           OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_CLEI
#define OOD_XPATH_SHELF_HW_VER         OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_HW_VER
#define OOD_XPATH_SHELF_OPRATNAL_STATE OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_OPRATNAL_STATE
#define OOD_XPATH_SHELF_EQPMT_STATE    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_EQPMT_STATE
#define OOD_XPATH_SHELF_IS_PHYSICAL    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_IS_PHYSICAL
#define OOD_XPATH_SHELF_IS_PASSIVE     OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_IS_PASSIVE
#define OOD_XPATH_SHELF_FACEPTE_LABEL  OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_FACEPTE_LABEL
#define OOD_XPATH_SHELF_DUE_DATE       OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" LEAF_SHELF_DUE_DATE
#define OOD_XPATH_SHELF_RESET_CAUSE    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:reset-cause"
#define OOD_XPATH_SHELF_TEMPERATURE    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:temperature"
#define OOD_XPATH_SHELF_TEMP_UP_THR    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:temperature-up-thr"
#define OOD_XPATH_SHELF_TEMP_LOW_THR   OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:temperature-low-thr"

#define OOD_XPATH_SHELF_UNIT_NAME                    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:unit-name"
#define OOD_XPATH_SHELF_FC_NUMBER                    OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:fc-number"
#define OOD_XPATH_SHELF_PIU_ISSUE_NUMBER             OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:piu-issue-number"
#define OOD_XPATH_SHELF_FUJITSU_PART_NUMBER          OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/com-oplink-physical-ext:fujitsu-part-number"

/*
 * /org-openroadm-device:org-openroadm-device/shelves[shelf-name='%s']/slots[slot-name]
 */
#define OOD_XPATH_LIST_SHELVE_SLOTS      OOD_XPATH_LIST_SHELVES "[shelf-name='%s']" "/slots"
#define OOD_XPATH_LIST_SHELVE_SLOTS_ALL  OOD_XPATH_LIST_SHELVE_SLOTS "//*"
#define KEY_SLOT_NAME                    "[slot-name"
#define LEAF_SLOT_NAME                   "/slot-name"
#define LEAF_SLOT_LABEL                  "/label"
#define LEAF_SLOT_PRVSN_CIRCUIT_PAC      "/provisioned-circuit-pack"
#define LEAF_SLOT_RESET_CAUSE            "/reset-cause"
#define LEAF_SLOT_STATUS                 "/slot-status"

#define OOD_XPATH_SLOT_NAME              OOD_XPATH_LIST_SHELVE_SLOTS LEAF_SLOT_NAME
#define OOD_XPATH_SLOT_LABEL             OOD_XPATH_LIST_SHELVE_SLOTS "[slot-name='%s']" LEAF_SLOT_LABEL
#define OOD_XPATH_SLOT_PRVSN_CIRCUIT_PAC OOD_XPATH_LIST_SHELVE_SLOTS "[slot-name='%s']" LEAF_SLOT_PRVSN_CIRCUIT_PAC
#define OOD_XPATH_SLOT_STATUS            OOD_XPATH_LIST_SHELVE_SLOTS "[slot-name='%s']" LEAF_SLOT_STATUS

#define SLOT_NAME_CHASSIS            "chassis"
#define SLOT_NAME_PM_1               "pm-1"
#define SLOT_NAME_PM_2               "pm-2"
#define SLOT_NAME_FAN_1              "fan-1"
#define SLOT_NAME_FAN_2              "fan-2"
#define SLOT_NAME_FAN_3              "fan-3"
#define SLOT_NAME_FAN_4              "fan-4"
#define SLOT_NAME_SCC                "scc"
#define SLOT_NAME_BA                 "ba"
#define SLOT_NAME_PA                 "pa"
#define SLOT_NAME_OCM                "ocm"

/*
 * /org-openroadm-device:org-openroadm-device/circuit-packs[circuit-pack-name]
 */
#define OOD_XPATH_LIST_CIRCUIT_PACKS     OOD_XPATH_PREFIX "/circuit-packs"
#define OOD_XPATH_LIST_CIRCUIT_PACKS_ALL OOD_XPATH_LIST_CIRCUIT_PACKS "//*"
#define KEY_CP_NAME             "[circuit-pack-name"
#define LEAF_CP_NAME            "/circuit-pack-name"
#define LEAF_CP_TYPE            "/circuit-pack-type"
#define LEAF_CP_PRODUCT_CODE    "/circuit-pack-product-code"
#define LEAF_CP_LIFECYCLE_STATE "/lifecycle-state"
#define LEAF_CP_ADMIN_STATE     "/administrative-state"
#define LEAF_CP_VENDOR          NODE_INFO_VENDOR
#define LEAF_CP_MODEL           NODE_INFO_MODEL
#define LEAF_CP_SERIAL_ID       NODE_INFO_SERIAL_ID
#define LEAF_CP_COMMON_TYPE     COMMON_INFO_TYPE
#define LEAF_CP_COMMON_PRODUCT_CODE COMMON_INFO_PRODUCT_CODE
#define LEAF_CP_MFG_DATE        COMMON_INFO_MFG_DATE
#define LEAF_CP_CLEI            COMMON_INFO_CLEI
#define LEAF_CP_HW_VER          COMMON_INFO_HW_VER
#define LEAF_CP_OPRATNAL_STATE  COMMON_INFO_OPRATNAL_STATE
#define LEAF_CP_RESET_CAUSE     COMMON_INFO_RESET_CAUSE
#define LEAF_CP_CATEGORY_TYPE   "/circuit-pack-category/equipment-type-enum"
#define LEAF_CP_CATEGORYE_EXTSN "/circuit-pack-category/extension"
#define LEAF_CP_EQPMT_STATE     "/equipment-state"
#define LEAF_CP_MODE            "/circuit-pack-mode"
#define LEAF_CP_REF_SHELF_NAME  "/shelf"  /* leafref /org-openroadm-device/shelves/shelf-name */
#define LEAF_CP_REF_SLOT        "/slot"
#define LEAF_CP_SERVICE_AFFECT  "/com-oplink-physical-ext:service-affect"

#define LEAF_CP_SERVICE_UNIT_NAMET               "/com-oplink-physical-ext:unit-name"
#define LEAF_CP_SERVICE_PACK_FC_NUMBER           "/com-oplink-physical-ext:fc-number"
#define LEAF_CP_SERVICE_PACK_PIU_ISSUE_NUMBER    "/com-oplink-physical-ext:piu-issue-number"
#define LEAF_CP_SERVICE_PACK_FUJITSU_PART_NUMBER "/com-oplink-physical-ext:fujitsu-part-number"


#define OOD_XPATH_CIRCUIT_PACK_NAME_ALL  OOD_XPATH_LIST_CIRCUIT_PACKS LEAF_CP_NAME

#define OOD_XPATH_CIRCUIT_PACK_ADMIN_STATE     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_ADMIN_STATE
#define OOD_XPATH_CIRCUIT_PACK_OPRATNAL_STATE  OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_OPRATNAL_STATE


#define OOD_XPATH_CIRCUIT_PACK_ALL             OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" "//*"
#define OOD_XPATH_CIRCUIT_PACK_NAME            OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_NAME
#define OOD_XPATH_CIRCUIT_PACK_PRODUCT_CODE    OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_PRODUCT_CODE
#define OOD_XPATH_CIRCUIT_PACK_LIFECYCLE_STATE OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_LIFECYCLE_STATE
#define OOD_XPATH_CIRCUIT_PACK_VENDOR          OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_VENDOR
#define OOD_XPATH_CIRCUIT_PACK_MODEL           OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_MODEL
#define OOD_XPATH_CIRCUIT_PACK_SERIAL_ID       OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERIAL_ID
#define OOD_XPATH_CIRCUIT_PACK_COMMON_TYPE     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_COMMON_TYPE
#define OOD_XPATH_CIRCUIT_PACK_COMMON_PRODUCT_CODE    OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_COMMON_PRODUCT_CODE
#define OOD_XPATH_CIRCUIT_PACK_MFG_DATE        OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_MFG_DATE
#define OOD_XPATH_CIRCUIT_PACK_CLEI            OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_CLEI
#define OOD_XPATH_CIRCUIT_PACK_HW_VER          OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_HW_VER

#define OOD_XPATH_CIRCUIT_PACK_UNIT_NAME             OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERVICE_UNIT_NAMET
#define OOD_XPATH_CIRCUIT_PACK_FC_NUMBER             OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERVICE_PACK_FC_NUMBER
#define OOD_XPATH_CIRCUIT_PACK_PIU_ISSUE_NUMBER      OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERVICE_PACK_PIU_ISSUE_NUMBER
#define OOD_XPATH_CIRCUIT_PACK_FUJITSU_PART_NUMBER   OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERVICE_PACK_FUJITSU_PART_NUMBER

#define OOD_XPATH_CIRCUIT_PACK_EQPMT_STATE     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_EQPMT_STATE
#define OOD_XPATH_CIRCUIT_PACK_MODE            OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_MODE
#define OOD_XPATH_CIRCUIT_PACK_REF_SHELF_NAME  OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_REF_SHELF_NAME
#define OOD_XPATH_CIRCUIT_PACK_REF_SLOT        OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_REF_SLOT
#define OOD_XPATH_CIRCUIT_PACK_SERVICE_AFFECT  OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" LEAF_CP_SERVICE_AFFECT

#define OOD_XPATH_CPS_LIST_FEATURES     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" "/circuit-pack-features"
#define OOD_XPATH_CPS_LIST_FEATURES_ALL OOD_XPATH_CPS_LIST_FEATURES "//*"

#define LEAF_CP_FEATURES_SW_LOAD_VER "/software-load-version"
#define LEAF_CP_FEATURES_DESCPTN     "/feature/description"
#define LEAF_CP_FEATURES_BOOT        "/feature/boot"
#define LEAF_CP_FEATURES_ACTIVATED   "/feature/activated"

#define OOD_XPATH_CP_LIST_COMPONENTS     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" "/circuit-pack-components"
#define OOD_XPATH_CP_LIST_COMPONENTS_ALL OOD_XPATH_CP_LIST_COMPONENTS "//*"
#define KEY_CP_COMPONENT               "[component-name"
#define LEAF_CP_COMPONENT              "/component-name"
#define LEAF_CP_COMPONENT_NAME         "/component/name"
#define LEAF_CP_COMPONENT_BOOT         "/component/boot"
#define LEAF_CP_COMPONENT_CURRENT_VER  "/component/current-version"
#define LEAF_CP_COMPONENT_VER_TO_APPLY "/component/version-to-apply"

#define OOD_XPATH_CPS_COMPONENT_NAME_ALL  OOD_XPATH_CP_LIST_COMPONENTS LEAF_CP_COMPONENT
#define OOD_XPATH_CPS_COMPONENT_CURRENT_VER  OOD_XPATH_CP_LIST_COMPONENTS "[component-name='%s']" LEAF_CP_COMPONENT_CURRENT_VER
#define OOD_XPATH_CPS_COMPONENT_NAME         OOD_XPATH_CP_LIST_COMPONENTS "[component-name='%s']" LEAF_CP_COMPONENT
#define OOD_XPATH_CPS_COMPONENT_BOOT         OOD_XPATH_CP_LIST_COMPONENTS "[component-name='%s']" LEAF_CP_COMPONENT_BOOT
#define OOD_XPATH_CPS_COMPONENT_VER_TO_APPLY OOD_XPATH_CP_LIST_COMPONENTS "[component-name='%s']" LEAF_CP_COMPONENT_VER_TO_APPLY

#define OOD_XPATH_CP_LIST_PORTS     OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" "/ports"
#define OOD_XPATH_CP_LIST_PORTS_ALL OOD_XPATH_CP_LIST_PORTS "//*"
#define KEY_CP_PORT                   "[port-id"
#define LEAF_CP_PORT_ID               "/port/port-id"
#define LEAF_CP_PORT_TYPE             "/port/port-type"
#define LEAF_CP_PORT_NAME             "/port/port-name"
#define LEAF_CP_PORT_WAVELENGTH_TYPE  "/port/port-wavelength-type"
#define LEAF_CP_PORT_QUAL             "/port/port-qual"
#define LEAF_CP_PORT_DIRECTION        "/port/port-direction"
#define LEAF_CP_PORT_CURRENT_POWER    "/port/port-current-power"
#define LEAF_CP_PORT_CAPA_MIN_RX      "/port/port-property/port-power-capability-min-rx"
#define LEAF_CP_PORT_CAPA_MAX_RX      "/port/port-property/port-power-capability-max-rx"
#define LEAF_CP_PORT_CAPA_MIN_TX      "/port/port-property/port-power-capability-min-tx"
#define LEAF_CP_PORT_CAPA_MAX_TX      "/port/port-property/port-power-capability-max-tx"
#define LEAF_CP_PORT_OPRATIONAL_STATE "/port/operational-state"
#define LEAF_CP_PORT_FACEPLATE_LABEL  "/port/faceplate-label"
#define LEAF_CP_PORT_ADMIN_STATE      "/port/administrative-state"


#define OOD_XPATH_CP_PORT_ID_ALL          OOD_XPATH_CP_LIST_PORTS "/port-id"
#define OOD_XPATH_CP_PORT_ID              OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_ID
#define OOD_XPATH_CP_PORT_TYPE            OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_TYPE
#define OOD_XPATH_CP_PORT_WAVELENGTH_TYPE OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_WAVELENGTH_TYPE
#define OOD_XPATH_CP_PORT_QUAL            OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_QUAL
#define OOD_XPATH_CP_DIRECTION            OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_DIRECTION

#define OOD_XPATH_CP_CURRENT_POWER        OOD_XPATH_CP_LIST_PORTS "[port-id='%d']" LEAF_CP_PORT_CURRENT_POWER

#ifdef __cplusplus
}
#endif

#endif
