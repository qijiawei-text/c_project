#ifndef __DSC_LLDP_H__
#define __DSC_LLDP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define MAX_LLDP_NBRS       (3)
#define MAX_LLDP_TLVS       (3)
#define L2_PORT_CNT         (3)
#define LLDP_STR_LEN        (64)


/*Len defined by LLDP protocol */
#define LLDP_MAX_OUI_VAL_LEN    (507)
#define LLDP_OUI_LEN    (3)

typedef enum en_LLDP_ADMINSTATUS
{
    LLDP_ADMINSTATUS_DISABLE = 0,
    LLDP_ADMINSTATUS_ENABLE,              /* enable, txandrx */
} LLDP_ADMINSTATUS_EN;


typedef struct st_LLDP_CUSTOM_TLV
{
    int32_t type;
    char oui[LLDP_OUI_LEN];
    char ouiSubType[LLDP_STR_LEN];
    char value[LLDP_MAX_OUI_VAL_LEN];
} LLDP_CUSTOM_TLV_ST;

typedef struct st_LLDP_NBR_INFO
{
    char neighbor_id[LLDP_STR_LEN];
    char remoteSysName[LLDP_STR_LEN];      /* string */
    char remoteSysDesc[LLDP_STR_LEN];      /* string */
    char remoteMgmtAddressSubType[LLDP_STR_LEN];      /* ipv4 or ipv6, ipv6 first */
    char remoteMgmtAddress[LLDP_STR_LEN];      /* ipv4 or ipv6, ipv6 first */
    char remotePortIdSubType[LLDP_STR_LEN];      /* enumeration */
    char remotePortId[LLDP_STR_LEN];      /* string */
    char remoteChassisIdSubType[LLDP_STR_LEN];      /* remoteChassisIdSubType */
    char remoteChassisId[LLDP_STR_LEN];        /* string */
    char remoteLastUpdate[LLDP_STR_LEN];        /* string */
} LLDP_NBR_ST;

typedef struct st_LLDP_PORT_CONFIG
{
    char acPortName[LLDP_STR_LEN];  /*Linux IfName, (ethx, ethx.x) */
    bool enabled;
    char acPortDescription[LLDP_STR_LEN]; /*device port name (OSCx, CV) */
    char acPortIdSubType[LLDP_STR_LEN];
    char acLocalValue[LLDP_STR_LEN];
    uint32_t tlv_count;
    LLDP_CUSTOM_TLV_ST customTlv[MAX_LLDP_TLVS];
} LLDP_PORT_CONFIG_ST;

typedef struct st_LLDP_COUNTER
{
    uint64_t frame_in;
    uint64_t frame_out;
    uint64_t frame_error_in;
    uint64_t frame_disgard;
    uint64_t frame_error_out;
    uint64_t tlv_discard;
    uint64_t tlv_unknow;
    char last_clear[LLDP_STR_LEN];     /*data_and_time*/
} LLDP_COUNTER_ST;

typedef struct st_LLDP_PORT_STATE
{
    char port_name[LLDP_STR_LEN];
    uint32_t neighbor_cnt;
    LLDP_NBR_ST neighbor[MAX_LLDP_NBRS];
    LLDP_COUNTER_ST counters;
} LLDP_PORT_STATE_ST;

typedef struct st_LLDP_CONFIG
{
    bool enabled;
    uint64_t txInterval;
    uint64_t holdMultiplier;
    char acSysName[LLDP_STR_LEN];
    char acSysDescription[LLDP_STR_LEN];
    char acAgentType[LLDP_STR_LEN];
    char acMgmtAddress[LLDP_STR_LEN];
    char acChassisIdSubType[LLDP_STR_LEN];
    char acChassisId[LLDP_STR_LEN];
    LLDP_PORT_CONFIG_ST stPortConfig[L2_PORT_CNT];
} LLDP_CONFIG_ST;

typedef struct st_LLDP_STATE
{
    LLDP_COUNTER_ST counters;
    LLDP_PORT_STATE_ST portState[L2_PORT_CNT];
} LLDP_STATE_ST;

#define MODULE_NS_LLDP                     "openconfig-lldp"
#define LLDP_HEADER_XPATH                  "/" MODULE_NS_LLDP ":lldp"
#define LLDP_HEADER_XPATH_ALL              LLDP_HEADER_XPATH "//*"

#define XPATH_LLDP_CONFIG                  LLDP_HEADER_XPATH "/config"
#define XPATH_LLDP_CONFIG_ALL              XPATH_LLDP_CONFIG "//*"

#define LLDP_ENABLED                       "enabled"
#define XPATH_LLDP_CONFIG_ENABLED          XPATH_LLDP_CONFIG "/" LLDP_ENABLED
#define LLDP_TXINTERVAL                    "hello-timer"
#define XPATH_LLDP_CONFIG_TXINTERVAL       XPATH_LLDP_CONFIG "/"  LLDP_TXINTERVAL
#define LLDP_HOLDMULTIPLIER                "com-oplink-lldp-ext:hold-timer"
#define XPATH_LLDP_CONFIG_HOLDMULTIPLIER   XPATH_LLDP_CONFIG "/" LLDP_HOLDMULTIPLIER

#define LLDP_AGENT_TYPE                    "com-oplink-lldp-ext:agent-type"
#define XPATH_LLDP_CONFIG_AGENT_TYPE   XPATH_LLDP_CONFIG "/" LLDP_AGENT_TYPE

#define LLDP_SYSTEM_NAME                   "system-name"
#define XPATH_LLDP_CONFIG_SYSTEM_NAME      XPATH_LLDP_CONFIG "/" LLDP_SYSTEM_NAME
#define LLDP_CHASSIS_ID                    "chassis-id"
#define XPATH_LLDP_CONFIG_CHASSIS_ID       XPATH_LLDP_CONFIG "/" LLDP_CHASSIS_ID
#define LLDP_CHASSIS_ID_TYPE               "chassis-id-type"
#define XPATH_LLDP_CONFIG_CHASSIS_ID_TYPE  XPATH_LLDP_CONFIG "/" LLDP_CHASSIS_ID_TYPE
#define LLDP_MANAGEMENT_IP                 "management-address"
#define XPATH_LLDP_CONFIG_MANAGEMENT_IP    XPATH_LLDP_CONFIG "/" LLDP_MANAGEMENT_IP
#define LLDP_MANAGEMENT_IP_TYPE            "management-address-type"
#define XPATH_LLDP_CONFIG_MANAGEMENT_IP_TYPE XPATH_LLDP_CONFIG "/" LLDP_MANAGEMENT_IP_TYPE
#define LLDP_SYSTEM_DESC                   "system-description"
#define XPATH_LLDP_CONFIG_SYSTEM_DESC      XPATH_LLDP_CONFIG "/" LLDP_SYSTEM_DESC

#define XPATH_LLDP_STATE                   LLDP_HEADER_XPATH "/state"
#define XPATH_LLDP_STATE_CHASSIS_ID        XPATH_LLDP_STATE "/" LLDP_CHASSIS_ID
#define XPATH_LLDP_STATE_SYSTEM_NAME       XPATH_LLDP_STATE "/" LLDP_SYSTEM_NAME
#define XPATH_LLDP_COUNTERS                XPATH_LLDP_STATE "/counters"
#define LLDP_FRAME_IN                      "frame-in"
#define XPATH_LLDP_FRAME_IN                XPATH_LLDP_COUNTERS "/" LLDP_FRAME_IN
#define LLDP_FRAME_OUT                     "frame-out"
#define XPATH_LLDP_FRAME_OUT               XPATH_LLDP_COUNTERS "/" LLDP_FRAME_OUT
#define LLDP_FRAME_DISGARD                 "frame-discard"
#define XPATH_LLDP_FRAME_DISGARD           XPATH_LLDP_COUNTERS "/" LLDP_FRAME_DISGARD
#define LLDP_FRAME_ERROR_IN                "frame-error-in"
#define XPATH_LLDP_FRAME_ERROR_IN          XPATH_LLDP_COUNTERS "/" LLDP_FRAME_ERROR_IN
#define LLDP_TLV_DISGARD                   "tlv-disgard"
#define XPATH_LLDP_TLV_DISGARD             XPATH_LLDP_COUNTERS "/" LLDP_TLV_DISGARD
#define LLDP_TLV_UNKNOWN                   "tlv-unknown"
#define XPATH_LLDP_TLV_UNKNOWN             XPATH_LLDP_COUNTERS "/" LLDP_TLV_UNKNOWN
#define LLDP_LAST_CLEAR                    "last-clear"
#define XPATH_LLDP_LAST_CLEAR              XPATH_LLDP_COUNTERS "/" LLDP_LAST_CLEAR

#define XPATH_LLDP_PORT                    LLDP_HEADER_XPATH "/interfaces"
#define XPATH_LLDP_PORT_LIST               XPATH_LLDP_PORT "/interface"
#define XPATH_LLDP_PORT_NAME_ALL           XPATH_LLDP_PORT_LIST "/name"
#define XPATH_LLDP_PORT_LIST_CONFIG        XPATH_LLDP_PORT_LIST "/config"
#define XPATH_LLDP_PORT_CONFIG             XPATH_LLDP_PORT "/interface[name='%s']/config"
#define XPATH_LLDP_PORT_CONFIG_ENABLED     XPATH_LLDP_PORT_CONFIG "/" LLDP_ENABLED
#define LLDP_PORT_NAME        "name"
#define XPATH_LLDP_PORT_CONFIG_NAME        XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_NAME
#define LLDP_PORT_LOCAL "com-oplink-lldp-ext:local-value"
#define XPATH_LLDP_PORT_CONFIG_LOCAL       XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_LOCAL
#define LLDP_PORT_ID_TYPE_EXT "com-oplink-lldp-ext:port-id-type"
#define XPATH_LLDP_PORT_CONFIG_PORTID_TYPE XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_ID_TYPE_EXT
#define LLDP_PORT_DESC "com-oplink-lldp-ext:port-description"
#define XPATH_LLDP_PORT_CONFIG_PORT_DESC   XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_DESC
#define LLDP_PORT_TLV "com-oplink-lldp-ext:custom-tlvs/tlv[%d]"
#define XPATH_LLDP_PORT_CONFIG_TLV   XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_TLV
#define LLDP_PORT_TLV_ALL "com-oplink-lldp-ext:custom-tlvs/tlv"
#define XPATH_LLDP_PORT_CONFIG_TLV_ALL   XPATH_LLDP_PORT_CONFIG "/" LLDP_PORT_TLV_ALL
#define LLDP_PORT_TLV_TYPE "com-oplink-lldp-ext:type"
#define XPATH_LLDP_PORT_CONFIG_TLV_TYPE   XPATH_LLDP_PORT_CONFIG_TLV "/" LLDP_PORT_TLV_TYPE
#define LLDP_PORT_TLV_OUI "com-oplink-lldp-ext:oui"
#define XPATH_LLDP_PORT_CONFIG_TLV_OUI   XPATH_LLDP_PORT_CONFIG_TLV "/" LLDP_PORT_TLV_OUI
#define LLDP_PORT_TLV_OUITYPE "com-oplink-lldp-ext:oui-subtype"
#define XPATH_LLDP_PORT_CONFIG_TLV_OUITYPE   XPATH_LLDP_PORT_CONFIG_TLV "/" LLDP_PORT_TLV_OUITYPE
#define LLDP_PORT_TLV_VALUE "com-oplink-lldp-ext:value"
#define XPATH_LLDP_PORT_CONFIG_TLV_VALUE   XPATH_LLDP_PORT_CONFIG_TLV "/" LLDP_PORT_TLV_VALUE


#define XPATH_LLDP_PORT_STATE              XPATH_LLDP_PORT "/interface[name='%s']/state"
#define XPATH_LLDP_PORT_LIST_STATE         XPATH_LLDP_PORT_LIST "/state"
#define XPATH_LLDP_PORT_STATE_NAME         XPATH_LLDP_PORT_STATE "/" LLDP_PORT_NAME
#define XPATH_LLDP_PORT_STATE_ENABLED      XPATH_LLDP_PORT_STATE "/" LLDP_ENABLED
#define XPATH_LLDP_PORT_COUNTERS           XPATH_LLDP_PORT_STATE "/counters"
#define XPATH_LLDP_PORT_FRAME_IN           XPATH_LLDP_PORT_COUNTERS "/" LLDP_FRAME_IN
#define XPATH_LLDP_PORT_FRAME_OUT          XPATH_LLDP_PORT_COUNTERS  "/" LLDP_FRAME_OUT
#define XPATH_LLDP_PORT_FRAME_DISGARD      XPATH_LLDP_PORT_COUNTERS "/" LLDP_FRAME_DISGARD
#define XPATH_LLDP_PORT_FRAME_ERROR_IN     XPATH_LLDP_PORT_COUNTERS "/" LLDP_FRAME_ERROR_IN
#define XPATH_LLDP_PORT_TLV_DISGARD        XPATH_LLDP_PORT_COUNTERS "/" LLDP_TLV_DISGARD
#define XPATH_LLDP_PORT_TLV_UNKNOWN        XPATH_LLDP_PORT_COUNTERS "/" LLDP_TLV_UNKNOWN
#define XPATH_LLDP_PORT_LAST_CLEAR         XPATH_LLDP_PORT_COUNTERS "/" LLDP_LAST_CLEAR

/******NBR_LIST********/
#define  XPATH_LLDP_NBRS                   XPATH_LLDP_PORT "/interface[name='%s']/neighbors"
#define  XPATH_LLDP_NBR_ALL                XPATH_LLDP_NBRS "//*"
#define  XPATH_LLDP_NBR_LIST               XPATH_LLDP_NBRS "/neighbor"
#define  XPATH_LLDP_NBR                    XPATH_LLDP_NBRS "/neighbor[id='%d']"
#define  LLDP_NBR_ID                       "id"
#define  XPATH_LLDP_NBR_ID                 XPATH_LLDP_NBR "/" LLDP_NBR_ID
#define  XPATH_LLDP_NBR_STATE              XPATH_LLDP_NBR "/state"
#define  XPATH_LLDP_NBR_SYSTEM_NAME        XPATH_LLDP_NBR_STATE "/" LLDP_SYSTEM_NAME
#define  XPATH_LLDP_NBR_SYSTEM_DESC        XPATH_LLDP_NBR_STATE "/" LLDP_SYSTEM_DESC
#define  XPATH_LLDP_NBR_CHASSIS_ID         XPATH_LLDP_NBR_STATE "/" LLDP_CHASSIS_ID
#define  XPATH_LLDP_NBR_CHASSIS_ID_TYPE    XPATH_LLDP_NBR_STATE "/" LLDP_CHASSIS_ID_TYPE
#define  XPATH_LLDP_NBR_STATE_ID           XPATH_LLDP_NBR_STATE "/" LLDP_NBR_ID
#define  LLDP_NBR_TTL                      "ttl"
#define  XPATH_LLDP_NBR_TTL                XPATH_LLDP_NBR_STATE "/" LLDP_NBR_TTL
#define  LLDP_LAST_UPDATE                  "last-update"
#define  XPATH_LLDP_NBR_LAST_UPDATE        XPATH_LLDP_NBR_STATE "/" LLDP_NBR_LAST_UPDATE
#define  LLDP_PORT_ID                      "port-id"
#define  LLDP_PORT_ID_TYPE                 "port-id-type"
#define  XPATH_LLDP_NBR_PORT_ID            XPATH_LLDP_NBR_STATE "/" LLDP_PORT_ID
#define  XPATH_LLDP_NBR_PORT_ID_TYPE       XPATH_LLDP_NBR_STATE "/" LLDP_PORT_ID_TYPE
#define  XPATH_LLDP_NBR_MANAGEMENT_IP      XPATH_LLDP_NBR_STATE "/" LLDP_MANAGEMENT_IP
#define  XPATH_LLDP_NBR_MANAGEMENT_IP_TYPE XPATH_LLDP_NBR_STATE "/" LLDP_MANAGEMENT_IP_TYPE
#define  XPATH_LLDP_NBR_TLVS               XPATH_LLDP_NBR "/custom-tlvs"
#define  XPATH_LLDP_NBR_TLVS_ALL           XPATH_LLDP_NBR_TLVS "//*"
#define  XPATH_LLDP_NBR_TLV                XPATH_LLDP_NBR_TLVS "/tlv[type='%s'][oui='%s'][oui-subtype='%d']"
#define  XPATH_LLDP_NBR_TLV_STATE          XPATH_LLDP_NBR_TLV "/state"
#define  LLDP_TLV_TYPE                     "type"
#define  XPATH_LLDP_NBR_TLV_TYPE           XPATH_LLDP_NBR_TLV_STATE "/" LLDP_TLV_TYPE
#define  LLDP_TLV_OUI                      "oui"
#define  XPATH_LLDP_NBR_TLV_OUI            XPATH_LLDP_NBR_TLV_STATE "/" LLDP_TLV_OUI
#define  LLDP_TLV_OUI_TYPE                 "oui-subtype"
#define  XPATH_LLDP_NBR_TLV_OUI_TYPE        XPATH_LLDP_NBR_TLV_STATE "/" LLDP_TLV_OUI_TYPE
#define  LLDP_TLV_VALUE                     "value"
#define  XPATH_LLDP_NBR_TLV_VALUE           XPATH_LLDP_NBR_TLV_STATE "/" LLDP_TLV_VALUE

extern int DSC_GetLldpState(LLDP_STATE_ST *pstLldpState);
extern int DSC_GetLldpConfig(LLDP_CONFIG_ST *pstLldpConfig);
extern bool DSC_IsLldpdRunning();
extern int DSC_SetLldpPortEnable(char *portName, bool enable);

extern bool DSC_GetLldpGlobalAdminStatus();
extern int DSC_SetLldpGlobalAdminStatus(bool enabled);
extern int DSC_SetLldpGlobalMsgTxInterval(uint32_t u32MsgTxInterval);
extern int DSC_SetLldpGlobalMsgTxHoldMultiplier(uint32_t u32MsgTxHoldMultiplier);
extern int DSC_SetLldpPortAdminStatus(char *portName, bool enabled);


#ifdef __cplusplus
}
#endif

#endif
