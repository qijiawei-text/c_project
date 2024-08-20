#ifndef _DSC_SYSTEM_H
#define _DSC_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

/*****************************
   // config
*****************************/
#define CONFIG_DSC_API       1
#if CONFIG_DSC_API


typedef struct st_DEVICE_CONFIG
{
    char acNodeId[128];
    char acContact[128];
    char acLocation[128];
    char acHostname[128];
    uint32_t u32NodeNumber;
    int32_t i32Timezone;
    double dTemperatureHighThr;
    double dTemperatureLowThr;
} DEVICE_CONFIG_ST;

typedef enum en_NODE_TYPE
{
    NODE_TYPE_ROADM = 1,
    NODE_TYPE_XPDR,
    NODE_TYPE_EDFA,
    NODE_TYPE_OCM,
    NODE_TYPE_OPS,
    NODE_TYPE_TOA,
    NODE_TYPE_OCMLE,

    NODE_TYPE_MAX
} NODE_TYPE_EN;

typedef enum en_ROADM_MODE
{
    ROADM_MODE_2D = 1,
    ROADM_MODE_8D,
    ROADM_MODE_MAX
} ROADM_MODE_EN;

typedef enum en_EDFA_SUB_TYPE
{
    SUB_EDFA = 1,
    SUB_ILA,
    SUB_MAX
} EDFA_SUB_TYPE_EN;

typedef enum en_SYSTEM_MODE
{
    SYSTEM_AUTO,
    SYSTEM_MANUAL,
    SYSTEM_DEBUG
} SYSTEM_MODE_EN;

typedef enum en_TOA_MODE
{
    TOA_MODE_PAM4 = 1,
    TOA_MODE_MSFT_PAM4,
    TOA_MODE_PAM4_50G,
    TOA_MODE_PAM4_VMUX,
    TOA_MODE_RAINIER,
    TOA_MODE_MAX
} TOA_MODE_EN;

#define ETH4_DEFAULT_IP           "192.168.2.100"
#define ETH4_DEFAULT_MASK         "255.255.255.0"
#define ETH4_DEFAULT_GW           "192.168.2.100"
#define ETH4_DEFAULT_SUBNET       "192.168.2."

#define SYSTEM_MODE_AUTO           "auto"
#define SYSTEM_MODE_MANUAL         "manual"
#define SYSTEM_MODE_DEBUG          "debug"

#define NODE_TYPE_ROADM_STR        "rdm"
#define NODE_TYPE_XPDR_STR         "xpdr"
#define NODE_TYPE_EDFA_STR         "edfa"
#define NODE_TYPE_OCM_STR          "ocm"
#define NODE_TYPE_OPS_STR          "ops"
#define NODE_TYPE_TOA_STR          "toa" /* terminal optical amplifer*/
#define NODE_TYPE_OCMLE_STR        "ocmle"

/* roadm */
#define MODEL_TYPE_ROADM_2D_STR    "2DROADM"
#define MODEL_TYPE_ROADM_8D_STR    "8DROADM"
/* edfa */
#define MODEL_TYPE_EDFA_ILA_JUNIPER_STR "ILA"  /* in line edfa for juniper */
#define MODEL_TYPE_EDFA_COM_STR         "EDFA"
/* toa */
#define MODEL_TYPE_TOA_PAM4_STR     "PAM4"
#define MODEL_TYPE_TOA_MSFTPAM4_STR "MSFT-PAM4"
#define MODEL_TYPE_TOA_PAM4_50G_STR "PAM4-50G"
#define MODEL_TYPE_TOA_PAM4_VMUX_STR "PAM4-VMUX"
#define MODEL_TYPE_TOA_COM_STR      "TOA"
#define MODEL_TYPE_TOA_MSFT_STR     "MSFT-TOA"
#define MODEL_TYPE_TOA_ABB_STR      "ABB-TOA"
#define MODEL_TYPE_TOA_RAINIER_STR  "RAINIER"
/* ops */
#define MODEL_TYPE_OPS_8CH_STR      "8CH-OPS"

#define DATA_MODULE_DEVICE          "org-openroadm-device"
#define DATA_MODULE_DEVICE_INTERNAL "com-oplink-device"
#define DATA_MODULE_WAVEPLAN        "com-oplink-waveplan"

#define SYSTEM_PROC_DIR             "/usr/local/bin/"
#define SYSTEM_BDL_1_DIR            "/home/oplink/bundleprimary/"
#define SYSTEM_BDL_2_DIR            "/home/oplink/bundlebackup/"
#define SYSTEM_LOG_DIR              "/home/oplink/log/"
#define SYSTEM_CFG_1_DIR            "/home/oplink/datastore1/"
#define SYSTEM_CFG_2_DIR            "/home/oplink/datastore2/"

#define DEVICE_XPATH_CONFIG        OOD_XPATH_PREFIX "/info"
#define DEVICE_XPATH_CONFIG_ALL    DEVICE_XPATH_CONFIG "//*"
#define DEVICE_XPATH_NODE_ID       DEVICE_XPATH_CONFIG "/node-id"
#define DEVICE_XPATH_NODE_NO       DEVICE_XPATH_CONFIG "/node-number"
#define DEVICE_XPATH_TEMP_HIGH_THR DEVICE_XPATH_STATE "/temperature-up-thr"
#define DEVICE_XPATH_TEMP_LOW_THR  DEVICE_XPATH_STATE "/temperature-low-thr"


#define DEVICE_XPATH_ADMIN_STATE   DEVICE_XPATH_CONFIG "/administrative-state"
#define DEVICE_XPATH_WORK_MODE     DEVICE_XPATH_CONFIG "/work-mode"

#define SYSTEM_XPATH               "/ietf-system:system"
#define DEVICE_XPATH_HOSTNAME      SYSTEM_XPATH "/hostname"
#define DEVICE_XPATH_CONTACT       SYSTEM_XPATH "/contact"
#define DEVICE_XPATH_LOCATION      SYSTEM_XPATH "/location"


extern int DSC_GetConfigInfo(DEVICE_CONFIG_ST *pstDevCfg, size_t size);
extern int DSC_GetNodeId(char *pcNodeId, size_t nMaxSize);
extern int DSC_SetNodeId(char *pcNodeId, size_t size);
extern int DSC_SetNodeNumber(uint32_t u32NodeNumber);

extern int DSC_SetTemperatureThr(double dHighThr, double dLowThr);
extern int DSC_SetNodeSwVersion(char *pcSwVersion, size_t nSize);
extern int DSC_SetBuildSwVersion(char *pcSwVersion, size_t nSize);

#endif

/*****************************
   // service
*****************************/
#define SERVICE_DSC_API      1
#if SERVICE_DSC_API
typedef enum en_SERVICE
{
    SERVICE_NONE = 0,
    SERVICE_SNMP,
    SERVICE_NETCONF,
    SERVICE_REST,
    SERVICE_WEB,
    SERVICE_SFTP,
    SERVICE_BGP,
    SERVICE_GNMI,
    SERVICE_LLDP,
    SERVICE_OSPF,
    SERVICE_PROXYARP,
    SERVICE_KLISH,
    SERIVCE_MAX
} SERVICE_EN;

#define SERVICE_SNMP_STR                "snmp"
#define SERVICE_NETCONF_STR             "netconf"
#define SERVICE_REST_STR                "rest"
#define SERVICE_WEB_STR                 "web"
#define SERVICE_SFTP_STR                "sftp"
#define SERVICE_BGP_STR                 "bgp"
#define SERVICE_GNMI_STR                "gnmi-server"
#define SERVICE_LLDP_STR                "lldp"
#define SERVICE_OSPF_STR                "ospf"
#define SERVICE_PROXYARP_STR            "proxyarp"
#define SERVICE_KLISH_SERVER_STR        "klish-server"

#define MODULE_SERVICE                  "com-oplink-service"
#define SERVICE_XPATH_HEADER            "/com-oplink-service:support-service/services"
#define SERVICE_XPATH_ALL               SERVICE_XPATH_HEADER "//*"
#define SERVICE_NAME                    "/service-name"
#define SERVICE_XPATH                   SERVICE_XPATH_HEADER "[service-name='%s']"
#define SERVICE_ENABLE                  "/service-enable"
#define SERVICE_XPATH_ENABLE            SERVICE_XPATH SERVICE_ENABLE


extern SERVICE_EN DSC_ServiceToEnum(char *pcService);
extern char *DSC_ServiceToString(SERVICE_EN pcService, char *pcBuf);
extern bool DSC_GetServiceState(SERVICE_EN enService);
extern int DSC_SetServiceEnable(SERVICE_EN enService, bool bState);
extern int DSC_GetServiceCtrlIp(SERVICE_EN enService, char *pcCtrlIp);
#endif

/*****************************
   // client
*****************************/
#define CLIENT_DSC_API      1
#if CLIENT_DSC_API
typedef enum en_CLIENT
{
    CLIENT_NONE = 0,
    CLIENT_NTP,
    CLIENT_SYSLOG,
    CLIENT_TACACS,
    CLIENT_OPENFLOW,
    CLIENT_SNMPTRAP
} CLIENT_EN;
typedef enum en_ROLE
{
    ROLE_EQUAL = 0,
    ROLE_MASTER,
    ROLE_SLAVE,
} ROLE_EN;
#define ROLE_EQUAL_STR         "equal"
#define ROLE_MASTER_STR        "master"
#define ROLE_SLAVE_STR         "slave"

typedef enum en_HOST_TYPE
{
    HOST_TYPE_IPV4 = 0,
    HOST_TYPE_IPV6,
    HOST_TYPE_DOMAIN,
    HOST_TYPE_UNKOWN
} HOST_TYPE_EN;

typedef struct st_CLIENT_SERVER
{
    uint32_t u32ServerId;
    ROLE_EN enRole;
    char acServerIp[32];
} CLIENT_SERVER_ST;

#define CLIENT_SNMPTRAP_STR            "snmptrap"

#define CLIENT_SYSLOG_SERVER_NUM       2 /* max 2 */
#define CLIENT_NTP_SERVER_NUM          2 /* max 2 */

#define CLIENT_NTP_SERVER_ID_1         "server-id='1'" /* max 2 */
#define CLIENT_NTP_SERVER_ID_2         "server-id='2'" /* max 2 */

#define CLIENT_NTP_SERVER_HOST_LEN     128

#define CLIENT_XPATH_HEADER             OOD_XPATH_PREFIX "/config/clients"
#define CLIENT_XPATH_ALL                CLIENT_XPATH_HEADER "//*"
#define CLIENT_XPATH_NAME               CLIENT_XPATH_HEADER "/client-name"
#define CLIENT_XPATH                    CLIENT_XPATH_HEADER "[client-name='%s']"
#define CLIENT_XPATH_ENABLE             CLIENT_XPATH "/enable"
#define CLIENT_XPATH_SERVERS_HEADER     CLIENT_XPATH "/servers"
#define CLIENT_XPATH_SERVERS_ALL        CLIENT_XPATH_SERVERS_HEADER "//*"
#define SERVER_ID                       "/server-id"
#define CLIENT_XPATH_SERVERS_ID         CLIENT_XPATH_SERVERS_HEADER SERVER_ID
#define CLIENT_XPATH_SERVERS            CLIENT_XPATH_SERVERS_HEADER "[server-id='%d']"
#define SERVER_IP                       "/server-ip"
#define CLIENT_XPATH_SERVERS_IP         CLIENT_XPATH_SERVERS SERVER_IP
#define SERVER_ROLE                     "/server-role"
#define CLIENT_XPATH_SERVERS_ROLE       CLIENT_XPATH_SERVERS SERVER_ROLE
extern CLIENT_EN DSC_ClientToEnum(char *pcClient);
extern char *DSC_ClientToString(CLIENT_EN enClient, char *pcBuf);
extern ROLE_EN DSC_RoleToEnum(char *pcRole);
extern char *DSC_RoleToString(ROLE_EN enRole, char *pcBuf);
extern int DSC_GetClientServerInfo(CLIENT_EN enClient, CLIENT_SERVER_ST **ppstServerInfo, size_t *count);
extern int DSC_AddClientServerIp(CLIENT_EN enClient, int uiId, char *pcIpAddress);
extern int DSC_DeleteClientServerIp(CLIENT_EN enClient, int uiId);
extern HOST_TYPE_EN DSC_GetHostType(char *pcHost);

/* openflow */
typedef struct st_OF_CTRL
{
    char acCtrlId[4];
    char acRole[8];
    char acProtocol[8];
    char acCtrlIp[32];
    uint16_t u16CtrlPort;
} OF_CTRL_ST;

#define MAX_CTRL         8
typedef struct st_OF_CONFIG
{
    char cOnlineCtrl;
    char acRev1[3];
    OF_CTRL_ST astOfCtrl[MAX_CTRL];
} OF_CONFIG_ST;

#define OF_PROTOCO_TCP            "tcp"
#define OF_PROTOCO_TLS            "tls"
#define OF_PORT_DEF               6633
extern int DSC_GetOfConfig(OF_CONFIG_ST *pstOfConfig, size_t size);

#endif

/*****************************
   // status
*****************************/
#define STATUS_DSC_API      1
#if STATUS_DSC_API

#define DEVICE_STATE_COUNT             33
#define DEVICE_XPATH_STATE             OOD_XPATH_PREFIX "/info"
#define DEVICE_XPATH_TEMPERATURE       DEVICE_XPATH_STATE "/temperature"
#define UPG_STATE_XPATH                "/upg-state"
#define DEVICE_XPATH_UPG_STATE         OOD_XPATH_PREFIX "/pending-sw/com-oplink-swdl-ext:sw-upgrade-state"

#define DEVICE_XPATH_UPG_STATE_ALL     DEVICE_XPATH_UPG_STATE "//*"
#define DEVICE_XPATH_UPG_STATE_STAGE   DEVICE_XPATH_UPG_STATE "/upgrade-state"
#define DEVICE_XPATH_UPG_STATE_SYS_STATUS  DEVICE_XPATH_UPG_STATE "/sys-status"
#define DEVICE_XPATH_UPG_STATE_STEP        DEVICE_XPATH_UPG_STATE "/upgrade-step"
#define DEVICE_XPATH_UPG_STATE_PERCENT     DEVICE_XPATH_UPG_STATE "/percent"
#define DEVICE_XPATH_UPG_STATE_HITLESS     DEVICE_XPATH_UPG_STATE "/hitless"
#define DEVICE_XPATH_DEVICE_STATE          DEVICE_XPATH_STATE "/operational-state"
#define DEVICE_XPATH_RUNTIME               DEVICE_XPATH_STATE "/run-time"
#define DEVICE_XPATH_RST_CAUSE             DEVICE_XPATH_STATE "/reset-cause"
#define DEVICE_XPATH_UPG_STATE_NUM     (4)

#define SLOT_ID_NODE                   "/slot-id"
#define SLOT_NAME_NODE                 "/slot-name"
#define LABEL_NODE                     "/label"
#define SLOT_STATUS_NODE               "/slot-status"
#define DEVICE_XPATH_SLOTS_STATE       OOD_XPATH_PREFIX "/slots-state"
#define SLOTS_XPATH_HEARD              DEVICE_XPATH_SLOTS_STATE "/slots"
#define SLOTS_XPATH_ALL                SLOTS_XPATH_HEARD "//*"

#define SLOTS_XPATH_SLOT_NAME          SLOTS_XPATH_HEARD "[slot-id='%d']" SLOT_NAME_NODE
#define SLOTS_XPATH_LABEL              SLOTS_XPATH_HEARD "[slot-id='%d']" LABEL_NODE
#define SLOTS_XPATH_SLOT_STATUS        SLOTS_XPATH_HEARD "[slot-id='%d']" SLOT_STATUS_NODE

#define DSC_SLOT_STATUS_EMPTY_NOT_PROV_STR               "empty-not-prov"
#define DSC_SLOT_STATUS_EMPTY_PROV_MATCH_STR             "empty-prov-match"
#define DSC_SLOT_STATUS_EMPTY_PROV_MISMATCH_STR          "empty-prov-mismatch"
#define DSC_SLOT_STATUS_INSTALLED_NOT_PROV_STR           "installed-not-prov"
#define DSC_SLOT_STATUS_INSTALLED_PROV_MATCH_STR         "installed-prov-match"
#define DSC_SLOT_STATUS_INSTALLED_PROV_MISMATCH_STR      "installed-prov-mismatch"


#define DEVICE_XPATH_LEDS                   OOD_XPATH_LIST_CIRCUIT_PACKS "/com-oplink-physical-ext:leds"
#define DEVICE_XPATH_LEDS_LIST              OOD_XPATH_LIST_CIRCUIT_PACKS "[circuit-pack-name='%s']" "/com-oplink-physical-ext:leds"
#define DEVICE_XPATH_LEDS_LIST_ALL          DEVICE_XPATH_LEDS_LIST "//*"

#define LED_LIST_KEY                        "/com-oplink-physical-ext:name"
#define DEVICE_XPATH_LED_KEY                DEVICE_XPATH_LEDS_LIST LED_LIST_KEY

#define LED_CONTAINER_NAME                  "/com-oplink-physical-ext:led"
#define DEVICE_XPATH_LED_CONTAINER          DEVICE_XPATH_LEDS_LIST LED_CONTAINER_NAME
#define DEVICE_XPATH_LED_CONTAINER_ALL      DEVICE_XPATH_LED_CONTAINER "//*"


#define LED_NAME                            "/led-name"
#define DEVICE_XPATH_LED_NAME               DEVICE_XPATH_LED_CONTAINER LED_NAME
#define LED_STATE                           "/status"
#define DEVICE_XPATH_LED_STATE              DEVICE_XPATH_LED_CONTAINER LED_STATE

#define DEVICE_XPATH_LED_STATUS             DEVICE_XPATH_LEDS_LIST "[name='%s']/led/status"

#define DSC_LED_STATE_OFF_STR          "off"
#define DSC_LED_STATE_GREEN_STR        "green"
#define DSC_LED_STATE_GREEN_BLINK_STR  "green-blink"
#define DSC_LED_STATE_AMBER_STR        "amber"
#define DSC_LED_STATE_AMBER_BLINK_STR  "amber-blink"
#define DSC_LED_STATE_RED_STR          "red"
#define DSC_LED_STATE_RED_BLINK_STR    "red-blink"



enum DSC_EN_LED
{
    DSC_LED_SYS = 0,
    DSC_LED_PRI,
    DSC_LED_PWR,
    DSC_LED_FAN,
    DSC_LED_SEC,
    DSC_LED_MOD,

    DSC_LED_CNT
};

/*   Slot table of pluggable card for OLS-P
 * ------------------------------------------
 *	 slotId  |  CARD
 * ------------------------------------------
 *   1 ~ 2   |  PSU
 * ------------------------------------------
 *	 3 ~ 6   |  FAN
 * ------------------------------------------
 *	   7	 |  RPC
 * ------------------------------------------
 *	   8	 |  OPS
 * ------------------------------------------
 *	   9     |  AMP
 * ------------------------------------------
 *	   10    |  AMP-P
 * ------------------------------------------
 *	   11	 |  SCC
 * ------------------------------------------
 */

/*   Slot table of pluggable card for NR1004
 * ------------------------------------------
 *   slotId  |  CARD
 * ------------------------------------------
 *     1     |  OPS
 * ------------------------------------------
 *     2     |  AMP
 * ------------------------------------------
 *     3     |  AMP-P
 * ------------------------------------------
 *     4     |  SCC
 * ------------------------------------------
 *     5     |  PSU (PSU0)
 * ------------------------------------------
 *     6 ~ 9 |  FAN (FAN0~3)
 * ------------------------------------------
 *     10    |  RPC
 * ------------------------------------------
 *     11    |  PSU (PSU1)
 * ------------------------------------------
 */

typedef enum DSC_SLOT_NO_en {
    DSC_SLOT_MIN = 0,
    DSC_SLOT_1 = 1,
    DSC_SLOT_2,
    DSC_SLOT_3,
    DSC_SLOT_4,
    DSC_SLOT_5,
    DSC_SLOT_6,
    DSC_SLOT_7,
    DSC_SLOT_8,
    DSC_SLOT_9,
    DSC_SLOT_10,
    DSC_SLOT_11,
    DSC_SLOT_MAX
} DSC_SLOT_NO_EN;

/** The solt id of the first SLC card.
    refer to org-openroadm-device:org-openroadm-device/shelves/slots/label
 **/
#define SLOT_ID_SLC_OFFSET              DSC_SLOT_1

typedef enum DSC_SLOT_STATUS_en
{
    DSC_SLOT_STATUS_EMPTY_NOT_PROV = 1,
    DSC_SLOT_STATUS_EMPTY_PROV_MATCH,
    DSC_SLOT_STATUS_EMPTY_PROV_MISMATCH,
    DSC_SLOT_STATUS_INSTALLED_NOT_PROV,
    DSC_SLOT_STATUS_INSTALLED_PROV_MATCH,
    DSC_SLOT_STATUS_INSTALLED_PROV_MISMATCH = 6,
    DSC_SLOT_STATUS_MAX
} DSC_SLOT_STATUS_EN;

typedef struct SLOT_INFO_st
{
    uint32_t uiSlotId;
    char acSlotName[32];
    char acLabel[32];
    DSC_SLOT_STATUS_EN enStatus;
} SLOT_INFO_ST;

typedef struct SHELF_SLOTS_st {
    SLOT_INFO_ST astSlotInfo[DSC_SLOT_MAX];
    uint32_t uiSlotCnt;
} SHELF_SLOTS_ST;

/* Temperature */
typedef struct st_TEMP_INFO
{
    double dBoardTemp;
    double dHighThr;
    double dLowThr;
} TEMP_INFO_ST;


#define SYS_STATE_IN_SERVICE_STR        "inService"
#define SYS_STATE_OUT_OF_SERVICE_STR    "outOfService"
#define SYS_STATE_DEGRADED_STR          "degraded"
typedef enum en_SYS_STATE
{
    SYS_STATE_IN_SERVICE = 1,
    SYS_STATE_OUT_OF_SERVICE,
    SYS_STATE_DEGRADED
} SYS_STATE_EN;
extern SYS_STATE_EN DSC_SysStateToEnum(char *pcSysState);
extern char *DSC_SysStateToString(SYS_STATE_EN enSysState, char *pcBuf);
extern int DSC_GetSystemStatus(SYS_STATE_EN *penSysState);
extern int DSC_GetSystemRstCause(char *pstr);

typedef struct st_UPG_STATE
{
    char acSysState[32];
    char acUpgdStep[32];
    uint8_t u8Percent;             /* range 0..100 */
    bool bHitLess;
}ST_UPG_STATE;

extern int DSC_GetUpgState(ST_UPG_STATE *pstUpgState, size_t size);

/* physical */
#define CPLD_MAX_NUM                  (4)
#define FPGA_MAX_NUM                  (4)
typedef struct st_DEVICE_PHYSICAL
{
    char acNodeType[32];
    char acVendor[32];
    char acModel[32];
    char acProductCode[32];
    char acSerialId[32];
    char acManufactorDate[32];
    char acClei[32];
    char acType[32];
    char acHwVer[32];
    char acSwVer[32];
    char aacCpldVer[CPLD_MAX_NUM][32];
    char aacFpgaVer[FPGA_MAX_NUM][32];
    char acEepromVer[32];
} DEVICE_PHYSICAL_ST;

typedef struct st_DEVICE_PHYSICAL_EXT
{
    char acDate[32];
    char acJdec[32];
    char acAssembly[32];
    char acPN[32];

    char acBoardRevision[32]; /* "hardware-version" in xml*/
    char acSN[32];
    char acModel[32];
    char acModelRevision[32];

    char acClei[32];
    char acEci[32];
    char acCpr[32];
    char acRsv[32];

} EXT_DEVICE_PHYSICAL_ST;

#define NODE_ID_MAX_SIZE 64
/*
 * /org-openroadm-device:org-openroadm-device/info
 */
typedef struct
{
    char acNodeId[NODE_ID_MAX_SIZE];
    uint32_t ulNodeNumber;
    char acNodeType[32];
    char acVendor[32];
    char acModel[32];
    char acSwVer[32];
    char acSwBuild[32];
    char acClli[32];
}ST_NODE_INFO;

/**org-openroadm-device for user***/
/**com-oplink-device for internal***/
#define DEVICE_XPATH_PHYSICAL          OOD_XPATH_PREFIX "/physical"
#define DEVICE_XPATH_PHYSICAL_EXT      DEVICE_XPATH_PHYSICAL "/com-oplink-extension-ILA:physical-ext"
#define DEVICE_XPATH_PHYSICAL_ALL      DEVICE_XPATH_PHYSICAL "//*"
#define DEVICE_XPATH_PHYSICAL_EXT_ALL  DEVICE_XPATH_PHYSICAL_EXT "//*"
#define DEVICE_XPATH_NODE_TYPE         DEVICE_XPATH_PHYSICAL "/node-type"
#define DEVICE_XPATH_VENDOR            DEVICE_XPATH_PHYSICAL "/vendor"
#define MODEL_NODE                     "/model"
#define DEVICE_XPATH_MODEL             DEVICE_XPATH_PHYSICAL MODEL_NODE
#define PRODUCT_CODE_NODE              "/product-code"
#define DEVICE_XPATH_PRODUCT_CODE      DEVICE_XPATH_PHYSICAL PRODUCT_CODE_NODE
#define SERIAL_ID_NODE                 "/serial-id"
#define DEVICE_XPATH_SERIAL_ID         DEVICE_XPATH_PHYSICAL SERIAL_ID_NODE
#define MFR_DATE_NODE                  "/manufactor-date"
#define DEVICE_XPATH_MFR_DATE          DEVICE_XPATH_PHYSICAL MFR_DATE_NODE
#define CLEI_NODE                      "/clei"
#define DEVICE_XPATH_CLLI              DEVICE_XPATH_PHYSICAL CLEI_NODE
#define DEVICE_XPATH_TYPE              DEVICE_XPATH_PHYSICAL "/module-type"
#define HW_VER_NODE                    "/hardware-version"
#define DEVICE_XPATH_HW_VER            DEVICE_XPATH_PHYSICAL HW_VER_NODE
#define SW_VER_NODE                    "/softwareVersion"
#define DEVICE_XPATH_SW_VER            DEVICE_XPATH_PHYSICAL SW_VER_NODE

#define CPLD_0_VER_NODE                "/cpld0-version"
#define DEVICE_XPATH_CPLD_0_VER        DEVICE_XPATH_PHYSICAL CPLD_0_VER_NODE
#define CPLD_1_VER_NODE                "/cpld1-version"
#define DEVICE_XPATH_CPLD_1_VER        DEVICE_XPATH_PHYSICAL CPLD_1_VER_NODE
#define FPGA_VER_NODE                  "/fpga-version"
#define DEVICE_XPATH_FPGA_VER          DEVICE_XPATH_PHYSICAL FPGA_VER_NODE
#define EEPROM_VER_NODE                "/eeprom-version"
#define DEVICE_XPATH_EEPROM_VER        DEVICE_XPATH_PHYSICAL EEPROM_VER_NODE

/********internal*******/
#define DEVICE_XPATH_SW_VER_I          COD_XPATH_PREFIX "/physical/softwareVersion"
#define DEVICE_XPATH_NODETYPE_I        COD_XPATH_PREFIX "/physical/node-type"
#define DEVICE_XPATH_MODEL_I           COD_XPATH_PREFIX "/physical/model"
#define DEVICE_XPATH_CPLD_0_VER_I      COD_XPATH_PREFIX "/physical/cpld0-version"
#define DEVICE_XPATH_CPLD_1_VER_I      COD_XPATH_PREFIX "/physical/cpld1-version"
#define DEVICE_XPATH_FPGA_VER_I        COD_XPATH_PREFIX "/physical/fpga-version"


#ifndef MOD_IDX_EN_DEFINED
typedef enum en_MOD_IDX
{
    MOD_IDX_BOARD = 0,
    MOD_IDX_EDFA,
    MOD_IDX_PA,
    MOD_IDX_BA,
    MOD_IDX_WSS,
    MOD_IDX_OCM,
    MOD_IDX_OPS,
    MOD_IDX_PAM4,
    MOD_IDX_TDCM,
    MOD_IDX_OSC,
    MOD_IDX_VOA,
    MOD_IDX_APPS,
    MOD_IDX_CPLD,
    MOD_IDX_FPGA,
    MOD_IDX_FAN,
    MOD_IDX_PWR,
    MOD_IDX_MUX,
    MOD_IDX_CARD_CHASSIS,
    MOD_IDX_CARD_OCM,
    MOD_IDX_CARD_BA,
    MOD_IDX_CARD_PA,
    MOD_IDX_CARD_SCC,

    MOD_IDX_MAX
} MOD_IDX_EN;
#define MOD_IDX_EN_DEFINED
#endif

#define MOD_IDX_EDFA_STR               "edfa"
#define MOD_IDX_EDFA_PA                "pa"
#define MOD_IDX_EDFA_BA                "ba"
#define MOD_IDX_WSS_STR                "wss"
#define MOD_IDX_OCM_STR                "ocm"
#define MOD_IDX_OPS_STR                "ops"
#define MOD_IDX_PAM4_STR               "pam4"
#define MOD_IDX_TDCM_STR               "tdcm"
#define MOD_IDX_OSC_STR                "osc"
#define MOD_IDX_VOA_STR                "voa"
#define MOD_IDX_APPS_STR               "apps"
#define MOD_IDX_CPLD_STR               "cpld"
#define MOD_IDX_FPGA_STR               "fpga"
#define MOD_IDX_FAN_STR                "fan"
#define MOD_IDX_PWR_STR                "pwr"
#define MOD_IDX_MUX_STR                "vmux"
#define MOD_IDX_CARD_CHASSIS_STR       "card-chassis"
#define MOD_IDX_CARD_OCM_STR           "card-ocm"
#define MOD_IDX_CARD_BA_STR            "card-ba"
#define MOD_IDX_CARD_PA_STR            "card-pa"
#define MOD_IDX_CARD_SCC_STR           "card-scc"
#define MOD_IDX_CARD_FAN0_STR          "card-fan0"
#define MOD_IDX_CARD_FAN1_STR          "card-fan1"
#define MOD_IDX_CARD_FAN2_STR          "card-fan2"
#define MOD_IDX_CARD_FAN3_STR          "card-fan3"
#define MOD_IDX_CARD_PWR0_STR          "card-pwr0"
#define MOD_IDX_CARD_PWR1_STR          "card-pwr1"

#define MODULE_XPATH_HEADER_I          COD_XPATH_PREFIX "/sub-modules"
#define MODULE_XPATH_HEADER            OOD_XPATH_PREFIX "/sub-modules"
#define MODULE_XPATH_ALL_I             MODULE_XPATH_HEADER_I "//*"
#define MODULE_XPATH_ALL               MODULE_XPATH_HEADER "//*"
#define MOD_ID_NODE                    "/module-id"
#define MODULE_XPATH_MOD_ID_ALL        MODULE_XPATH_HEADER MOD_ID_NODE
#define MODULE_XPATH_MOD_ID_ALL_I      MODULE_XPATH_HEADER_I MOD_ID_NODE
#define MOD_TYPE_NODE                  "/module-type"
#define MODULE_XPATH_I                 MODULE_XPATH_HEADER_I "[module-id='%d']"
#define MODULE_XPATH_MOD_TYPE_I        MODULE_XPATH_I MOD_TYPE_NODE
#define MODULE_XPATH_MODEL_I           MODULE_XPATH_I MODEL_NODE
#define DEVICE_XPATH_MODEL_I           COD_XPATH_PREFIX "/physical/model"
#define DEVICE_XPATH_NODE_TYPE_I       COD_XPATH_PREFIX "/physical/node-type"

#define MODULE_XPATH_MAX_ATTR          (9)
#define MODULE_XPATH_MOD_TYPE          MODULE_XPATH_HEADER "[module-id='%d']" MOD_TYPE_NODE
#define MODULE_XPATH_MODEL             MODULE_XPATH_HEADER "[module-id='%d']" MODEL_NODE
#define MODULE_XPATH_PRODUCT_CODE      MODULE_XPATH_HEADER "[module-id='%d']" PRODUCT_CODE_NODE
#define MODULE_XPATH_SERIAL_ID         MODULE_XPATH_HEADER "[module-id='%d']" SERIAL_ID_NODE
#define MODULE_XPATH_MFR_DATE          MODULE_XPATH_HEADER "[module-id='%d']" MFR_DATE_NODE
#define MODULE_XPATH_HW_VER            MODULE_XPATH_HEADER "[module-id='%d']" HW_VER_NODE
#define MODULE_XPATH_FW_VER            MODULE_XPATH_HEADER "[module-id='%d']" SW_VER_NODE
#define MODULE_XPATH_FPGA_VER          MODULE_XPATH_HEADER "[module-id='%d']" FPGA_VER_NODE
#define MODULE_XPATH_CPLD_0_VER        MODULE_XPATH_HEADER "[module-id='%d']" CPLD_0_VER_NODE
#define MODULE_XPATH_CPLD_1_VER        MODULE_XPATH_HEADER "[module-id='%d']" CPLD_1_VER_NODE
#define MODULE_XPATH_CLEI              MODULE_XPATH_HEADER "[module-id='%d']" CLEI_NODE
#define MODULE_XPATH_SW_VER            MODULE_XPATH_HEADER "[module-id='%d']" SW_VER_NODE
#define MODULE_XPATH_EEPROM_VER        MODULE_XPATH_HEADER "[module-id='%d']" EEPROM_VER_NODE

#define DEV_CMN_XPATH_HEADER           "/org-openroadm-device:org-openroadm-device/"
#define DEV_CMN_EXTERNAL_LINK          OOD_XPATH_PREFIX "/external-link"

extern int DSC_GetPhysicalInfo(DEVICE_PHYSICAL_ST *pstDevPhysical, size_t size);
extern int DSC_GetPhysicalInfoTimeout(DEVICE_PHYSICAL_ST *pstDevPhysical, size_t size, int seconds);
extern int DSC_GetPhysicalExtension(EXT_DEVICE_PHYSICAL_ST *pstIdeeprom);
extern int DSC_GetPhysicalExtension_1();
extern int DSC_GetModMfgInfo(DEVICE_PHYSICAL_ST **ppstDevPhysical, size_t *count);
extern MOD_IDX_EN DSC_ModuleTypeStrToEnum(char *pcModuleType);
extern int DSC_ModuleTypeEnumToString(MOD_IDX_EN enModType, char *pcModuleType, size_t len);
extern NODE_TYPE_EN DSC_NodetypeStrToEnum(char *pcNodeType);
extern int DSC_SetContact(char *pcStr, size_t size);
extern int DSC_SetLocation(char *pcStr, size_t size);
extern int DSC_SetHostname(char *pcStr, size_t size);

extern int DSC_GetAllSlotsState(SHELF_SLOTS_ST *pstShelfSlots);
extern int DSC_GetNodeInfo(ST_NODE_INFO *pstNodeInfo, size_t nMaxSize);

#endif

/*****************************
   // operate
*****************************/
#define OPERATE_DSC_API      1
#if OPERATE_DSC_API

/******restart*****/
#define MODULE_NS_OODO             "org-openroadm-de-operations"
#define RPC_XPATH_RESTART          "/org-openroadm-de-operations:restart"
#define RPC_XPATH_RESTART_OPTION   RPC_XPATH_RESTART "/option"
#define RPC_XPATH_RESTART_TYPE     RPC_XPATH_RESTART "/resourceType/type"
#define RPC_XPATH_RESTART_EXTENSION     RPC_XPATH_RESTART "/resourceType/extension"

#define RPC_XPATH_RESTART_STATUS   RPC_XPATH_RESTART "/status"
#define RPC_XPATH_RESTART_MESSAGE  RPC_XPATH_RESTART "/status-message"


#define WARM_RESTART_STR      "warm"
#define COLD_RESTART_STR      "cold"

/*luowei modify*/
#define RESET_DEVICE          "RST_DEVICE"
#define RESET_CARD_OPTICAL    "RST_CARD_OPTICAL"
#define RESET_CARD_OCM        "RST_CARD_OCM"
#define RESET_CARD_OCM_P      "RST_CARD_OCM_P"
#define RESET_CARD_BA         "RST_CARD_BA"
#define RESET_CARD_PA         "RST_CARD_PA"
#define RESET_CARD_SCC        "RST_CARD_SCC"
#define RESET_CARD_OTDR       "RST_CARD_OTDR"
#define RESET_CARD_OTDR_P     "RST_CARD_OTDR_P"
#define RESET_CARD_AMP        "RST_CARD_AMP"
#define RESET_CARD_AMP_P      "RST_CARD_AMP_P"
#define RESET_CARD_EDFA       "RST_CARD_EDFA"
#define RESET_CARD_EDFA_P     "RST_CARD_EDFA_P"
#define RESET_CARD_OPS        "RST_CARD_OPS"

#define RESET_CARD_OCS        "RST_CARD_OCS"
#define RESET_CARD_OPS4_A     "RST_CARD_OPS4_A"
#define RESET_CARD_OPS4_B     "RST_CARD_OPS4_B"


/******upgrade sw-stage*****/
#define MODULE_NS_OOSWDL               "org-openroadm-swdl"
#define RPC_XPATH_SW_STAGE             "/org-openroadm-swdl:sw-stage"
#define RPC_XPATH_SW_STAGE_FILENAME    RPC_XPATH_SW_STAGE "/filename"
#define RPC_XPATH_SW_STAGE_STATUS      RPC_XPATH_SW_STAGE "/status"
#define RPC_XPATH_SW_STAGE_MESSAGE     RPC_XPATH_SW_STAGE "/status-message"

/******upgrade sw-activate*****/
#define RPC_XPATH_SW_ACTIVATE             "/org-openroadm-swdl:sw-activate"
#define RPC_XPATH_SW_ACTIVATE_VERSION     RPC_XPATH_SW_ACTIVATE "/version"
#define RPC_XPATH_SW_ACTIVATE_VALID       RPC_XPATH_SW_ACTIVATE "/validationTimer"
#define RPC_XPATH_SW_ACTIVATE_STATUS      RPC_XPATH_SW_ACTIVATE "/status"
#define RPC_XPATH_SW_ACTIVATE_MESSAGE     RPC_XPATH_SW_ACTIVATE "/status-message"

/******upgrade cancel-validation-timer*****/
#define RPC_XPATH_SW_CANCEL               "/org-openroadm-swdl:cancel-validation-timer"
#define RPC_XPATH_SW_CANCEL_ACCEPT        RPC_XPATH_SW_CANCEL "/accept"
#define RPC_XPATH_SW_CANCEL_STATUS        RPC_XPATH_SW_CANCEL "/status"
#define RPC_XPATH_SW_CANCEL_MESSAGE       RPC_XPATH_SW_CANCEL "/status-message"

/******restore*****/
#define MODULE_NS_CORPC            "com-oplink-rpc"
#define RPC_XPATH_RESTORE          "/com-oplink-rpc:restore-default"
#define RPC_XPATH_RESTORE_TYPE     RPC_XPATH_RESTORE "/restore-type"
#define RPC_XPATH_RESTORE_STATUS   RPC_XPATH_RESTORE "/status"
#define RPC_XPATH_RESTORE_MESSAGE  RPC_XPATH_RESTORE "/status-message"

/******save_conf*****/
#define RPC_XPATH_SAVE_CONF        "/com-oplink-rpc:save-conf"
#define RPC_XPATH_SAVE_TYPE        RPC_XPATH_SAVE_CONF "/save-type"

/**************************************************************************************************/
/* definition of APSD set/get rpc                                                                 */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_RPC_XPATH_SET_APSD         "/com-oplink-rpc:set-apsd"
#define DEF_RPC_NODE_APSD_ENABLED      "/enabled"
#define DEF_RPC_NODE_APSD_PATH         "/path"
#define DEF_RPC_NODE_STATUS            "/status"

#define DEF_RPC_XPATH_GET_APSD         "/com-oplink-rpc:get-apsd-state"
#define DEF_RPC_NODE_APSD_STATUS       "/apsd-status"

typedef enum {
    EN_APSD_PATH_W = 0,
    EN_APSD_PATH_P = 1,
    EN_APSD_PATH_MAX
} EN_APSD_PATH;

typedef struct {
    bool bEnabled;
    bool bStatus;
    bool bEvent;
} ST_APSD_STATE;

/* } @                                                                                            */

/******save_conf*****/
#define RPC_XPATH_NTP_SYNC         "/com-oplink-device-common:ntp-sync"
#define RPC_XPATH_NTP_SYNC_IP      RPC_XPATH_NTP_SYNC "/server-ip"


/******File Transfer*******/
#define MODULE_NS_OOFT                      "org-openroadm-file-transfer"
#define RPC_XPATH_FILE_TRANSFER             "/org-openroadm-file-transfer:transfer"
#define RPC_XPATH_FILE_TRANSFER_ACTION      RPC_XPATH_FILE_TRANSFER "/action"
#define RPC_XPATH_FILE_TRANSFER_LOCAL       RPC_XPATH_FILE_TRANSFER "/local-file-path"
#define RPC_XPATH_FILE_TRANSFER_REMOTE      RPC_XPATH_FILE_TRANSFER "/remote-file-path"
#define RPC_XPATH_FILE_STATUS               RPC_XPATH_FILE_TRANSFER "/status"
#define RPC_XPATH_FILE_MESSAGE              RPC_XPATH_FILE_TRANSFER "/status-message"
/******Show File*******/
#define RPC_XPATH_SHOW_FILE                 "/org-openroadm-file-transfer:show-file"
#define RPC_XPATH_SHOW_FILE_NAME            RPC_XPATH_SHOW_FILE "/filename"
#define  RPC_XPATH_SHOW_FILE_STATUS         RPC_XPATH_SHOW_FILE "/status"
#define  RPC_XPATH_SHOW_FILE_MESSAGE        RPC_XPATH_SHOW_FILE "/status-message"
#define RPC_XPATH_SHOW_OUTPUT                  RPC_XPATH_SHOW_FILE "/file"
#define RPC_XPATH_SHOW_OUTPUT_FILE_NAME        RPC_XPATH_SHOW_OUTPUT "[filename='%s']" "/filename"
#define RPC_XPATH_SHOW_OUTPUT_FILE_SIZE        RPC_XPATH_SHOW_OUTPUT "[filename='%s']" "/file-size"
#define RPC_XPATH_SHOW_OUTPUT_FILE_MODIFY_DATE RPC_XPATH_SHOW_OUTPUT "[filename='%s']" "/modified-date"


/******Del File*******/
#define RPC_XPATH_DEL_FILE                  "/org-openroadm-file-transfer:delete-file"
#define RPC_XPATH_DEL_FILE_NAME             RPC_XPATH_DEL_FILE "/filename"
#define RPC_XPATH_DEL_FILE_STATUS           RPC_XPATH_DEL_FILE "/status"
#define RPC_XPATH_DEL_FILE_MESSAGE          RPC_XPATH_DEL_FILE "/status-message"

#define FILE_TRANSFER_UPLOAD                "upload"
#define FILE_TRANSFER_DOWNLOAD              "download"
#define FILE_TRANSFER_ONEKEY_UPGRADING      "onekey-upgrading"
#define UPGD_IMAGE_TRANSFERRING_DIRECTORY   "/tmp/download"
#define UPGD_IMAGE_EXEC_DIRECTORY           "/tmp"

/* log file */
#define GET_LOG_FILE                        "/log/saved/get_log.tgz"

/******notification*******/
#define FILE_XPATH_TRANSFER_EVENT "/org-openroadm-file-transfer:transfer-notification"
#define FILE_XPANT_EVENT_LOLCAL_PATH FILE_XPATH_TRANSFER_EVENT "/local-file-path"
#define FILE_XPANT_EVENT_STATUS FILE_XPATH_TRANSFER_EVENT "/status"


/******database*****/
#define MODULE_NS_OODS                 "org-openroadm-database"
#define DB_XPATH_BACKUP                "/org-openroadm-database:db-backup"
#define DB_XPATH_BACKUP_FILE           DB_XPATH_BACKUP "/filename"
#define DB_XPATH_BACKUP_NOTICE         "/org-openroadm-database:db-backup-notification"
#define DB_XPATH_BACKUP_NOTICE_STATUS  DB_XPATH_BACKUP_NOTICE "/status"
#define DB_XPATH_RESTORE               "/org-openroadm-database:db-restore"
#define DB_XPATH_RESTORE_FILE          DB_XPATH_RESTORE "/filename"
#define DB_XPATH_RESTORE_NODEID_CHECK  DB_XPATH_RESTORE "/nodeIDCheck"
#define DB_XPATH_RESTORE_NOTICE        "/org-openroadm-database:db-restore-notification"
#define DB_XPATH_RESTORE_NOTICE_STATUS DB_XPATH_RESTORE_NOTICE "/status"
#define DB_XPATH_ACTIVATE              "/org-openroadm-database:db-activate"
#define DB_XPATH_ACTIVATE_NOTICE       "/org-openroadm-database:db-activate-notification"
#define DB_XPATH_ACTIVATE_NOTICE_TYPE  DB_XPATH_ACTIVATE_NOTICE "/db-active-notification-type"
#define DB_XPATH_ACTIVATE_NOTICE_STATUS DB_XPATH_ACTIVATE_NOTICE "/status"
#define DB_XPATH_ROLLBACK_TIMER        DB_XPATH_ACTIVATE "/rollBackTimer"
#define DB_XPATH_CANCEL                "/org-openroadm-database:cancel-rollback-timer"
#define DB_XPATH_CANCEL_ACCEPT         DB_XPATH_CANCEL "/accept"
#define DB_XPATH_INIT                  "/org-openroadm-database:database-init"

#define FILE_TRANSSPORT_PROTOCAL  "scp"
/* #define FILE_TRANSSPORT_PROTOCAL  "sftp" */

#define DSC_FILE_NUM_MAX               (10)
#define DSC_FILE_NAME_LEN_MAX          (256)
#define DSC_MODIFY_TIME_LEN_MAX        (64)

typedef enum {
    EN_RETORE_DEFAULT_ALL = 0,
    EN_RETORE_DEFAULT_OPTICAL,
    EN_RETORE_DEFAULT_SYSTEM,
    EN_RETORE_DEFAULT_MAX,
} EN_RESTORE_DEFAULT_TYPE;

typedef struct
{
    char acFileName[DSC_FILE_NAME_LEN_MAX];
    uint64_t ullFileSize;
    char acFileModiyTime[DSC_MODIFY_TIME_LEN_MAX];
} FILE_ATTR_ENTRY_INFO_ST;

typedef struct
{
    FILE_ATTR_ENTRY_INFO_ST astFileEntryInfo[DSC_FILE_NUM_MAX];
    int32_t ulNum;
} FILE_ATTR_ALL_INFO_ST;


extern int DSC_Reset(char *pcResetModule, char *pcResetType);
extern int DSC_RestoreDefault(EN_RESTORE_DEFAULT_TYPE enRestoreType);
extern int DSC_SaveConfig(const char *module);
extern int DSC_NtpSync(char *pcIpAddress);
extern int DSC_NtpSync_Session(sr_session_ctx_t *sess, char *pcIpAddress);
extern int DSC_SaveLog();
extern int DSC_GetSystemFile(char *pcFilePath, FILE_ATTR_ALL_INFO_ST *pstAllFileInfo);
extern int DSC_DelSystemFile(char *pcFilePath);
extern int DSC_TransferFile(char *pcAction, char *pcLocalPath, char *pcRemotePath);
extern int DSC_TransferLog(char *pcUser, char *pcPwd, char *pcDestFile);
extern int DSC_SwStage(char *pcFilename);
extern int DSC_SwActivate(char *pcVersion, char *pcValidationTimer, char *pcStatus);
extern int DSC_SwUpgrade(char *pcFilename, char *pcStatus);
extern int DSC_SwCancel(bool bBoolean);
extern int DSC_SetAdminState(char *pcState);
extern int DSC_GetAdminState(char *pcState, size_t size);
extern int DSC_SetSystemMode(SYSTEM_MODE_EN enSystemMode);
extern int DSC_GetSystemMode(SYSTEM_MODE_EN *penSystemMode);
extern int DSC_BackupDB(char *pcFilePath);
extern int DSC_RestoreDB(char *pcFilePath, bool bNodeIdCheck);
extern int DSC_ActivateDB(char *pcValidationTimer);
extern int DSC_CancelDB(bool bBoolean);
extern int DSC_RpcGetApsdState(EN_APSD_PATH enPath, ST_APSD_STATE *pstApsdState);
extern int DSC_RpcSetApsd(EN_APSD_PATH enPath, bool bEnabled);

/*****************************
   // lamp Test
*****************************/

#define RPC_XPATH_LAMP_TEST            "/org-openroadm-device:led-control"   /* "/com-oplink-control-loop-nopporo:lamptest" */
#define LED_CTRL_XPATH_CP_NAME         RPC_XPATH_LAMP_TEST "/circuit-pack-name"
#define LED_CTRL_XPATH_PORT_NAME       RPC_XPATH_LAMP_TEST "/port-name"
#define LED_CTRL_XPATH_ENABELD         RPC_XPATH_LAMP_TEST "/enabled"

typedef struct {
    bool bEnabled;
    char acCpName[32];
    char acPortName[32];
} LED_CTRL_ST;

typedef enum {
    EN_LED_CTRL_NONE,
    EN_LED_CTRL_ALL,
    EN_LED_CTRL_FMD,        /* all leds of FMD component */
    EN_LED_CTRL_SINGLE_LC,  /* single LC led */
    EN_LED_CTRL_MAX
} LED_CTRL_MODE_EN;

int DSC_LampTest(bool enabled);

int DSC_PortLampTest(bool bEnabled, char *pcPortName);


/* Add SCP service enable/disable function from 400GZR. */
#define DSC_ENABLE_STR                  "enable"
#define DSC_DISABLE_STR                 "disable"
#define SCP_PATH                        "/usr/bin/scp"
#define SCP_OPENSSH_PATH                "/usr/bin/scp.openssh"

bool DSC_IsScpServerEnabled();
int DSC_SetScpServerEnable(bool bEnable);

#endif

#define RPC_XPATH_LOG_HASH         "/com-oplink-rpc:log-hash"
#define RPC_XPATH_LOG_HASH_ENABLE            RPC_XPATH_LOG_HASH "/enable"
#define RPC_XPATH_LOG_HASH_PROCESS_NAME      RPC_XPATH_LOG_HASH "/process-name"
extern int DSC_HashLogEnable(bool bBoolean);

#define RPC_XPATH_SET_AUTO_CTRL_RAMP            "/com-oplink-rpc:set-auto-ctrl-ramp"
#define RPC_XPATH_SET_AUTO_CTRL_RAMP_PATH       RPC_XPATH_SET_AUTO_CTRL_RAMP "/path"
#define RPC_XPATH_SE_AUTO_CTRLT_RAMP_STEP       RPC_XPATH_SET_AUTO_CTRL_RAMP "/step"
#define RPC_XPATH_SET_AUTO_CTRL_RAMP_INTERVAL   RPC_XPATH_SET_AUTO_CTRL_RAMP "/interval"
int DSC_AutoSetAutoCtrlRamp(char *pcLine, double dStep, uint32_t uiInterval);


#ifdef __cplusplus
}
#endif

#endif
