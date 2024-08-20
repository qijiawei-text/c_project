#ifndef __DSC_RSTP_H__
#define __DSC_RSTP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"


#define RSTP_XPATH_BRIDGE_NAME                        "/bridge-name"
#define RSTP_XPATH_BRIDGE_PRIORITY                    "/bridge-priority"
#define RSTP_XPATH_HOLD_TIME                          "/hold-time"
#define RSTP_XPATH_HELLO_TIME                         "/hello-time"
#define RSTP_XPATH_SHUT_DOWN                          "/shutdown"
#define RSTP_XPATH_MAX_AGE                            "/max-age"
#define RSTP_XPATH_FORWAED_DELAY                      "/forward-delay"
#define RSTP_XPATH_TRANSMIT_HOLD_COUNT                "/transmit-hold-count"

typedef enum en_L2_PORT_IFNAME
{
    L2_PORT_IFNAME_NONE = 0,
    L2_PORT_IFNAME_1,
    L2_PORT_IFNAME_2,
    L2_PORT_IFNAME_CNT
} L2_PORT_IFNAME_EN;

#define  L2_PORT_IFNAME_1_STR                         "OSCA"
#define  L2_PORT_IFNAME_2_STR                         "OSCB"

#define  RSTP_XPATH_BRIDGE_PORT_TABLE                 "/rstp-bridge-port-table"

#define RSTP_XPATH_BRIDGE_IFNAME                      "/ifname"
#define RSTP_XPATH_BRIDGE_PORT_TABLE_PRIORITY         "/priority"
#define RSTP_XPATH_BRIDGE_PORT_TABLE_COST             "/cost"

/**rstp-port-config**/
#define MODULE_NS_RSTP                                "org-openroadm-rstp"
#define RSTP_HEADER_XPATH                             "/" MODULE_NS_RSTP ":rstp"
#define PROTOCOLS_XPATH_RSTP_CONFIG                   RSTP_HEADER_XPATH "/rstp-bridge-instance[bridge-name='oplk-ila']/rstp-config"

#define DEVICE_XPATH_RSTP_BRIDGE_PRIORITY             PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_BRIDGE_PRIORITY
#define DEVICE_XPATH_RSTP_HOLD_TIME                   PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_HOLD_TIME
#define DEVICE_XPATH_RSTP_HELLO_TIME                  PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_HELLO_TIME
#define DEVICE_XPATH_RSTP_SHUT_DOWN                   PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_SHUT_DOWN
#define DEVICE_XPATH_RSTP_MAX_AGE                     PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_MAX_AGE
#define DEVICE_XPATH_RSTP_FORWARD_DELAY               PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_FORWAED_DELAY
#define DEVICE_XPATH_RSTP_TRANS_HOLD_COUNT            PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_TRANSMIT_HOLD_COUNT


#define RSTP_XPATH_BRIDGE_PORT_PRIORITY               "/rstp-bridge-port-table[ifname='%s']/priority"
#define RSTP_XPATH_BRIDGE_PORT_COST                   "/rstp-bridge-port-table[ifname='%s']/cost"
#define DEVICE_XPATH_RSTP_BRIDGE_PORT_PRIORITY        PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_BRIDGE_PORT_PRIORITY
#define DEVICE_XPATH_RSTP_BRIDGE_PORT_COST            PROTOCOLS_XPATH_RSTP_CONFIG RSTP_XPATH_BRIDGE_PORT_COST

/****rstp-state****/
#define DEVICE_XPATH_PROTOCOLS_RSTP_HEADER_ALL        RSTP_HEADER_XPATH "//*"

#define DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER      RSTP_HEADER_XPATH "/rstp-bridge-instance[bridge-name='oplk-ila']/rstp-state"
#define DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER_ALL  DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER "//*"

/****rstp-bridge-attr***/
#define RSTP_BRIDGE_ATTR_NODE                         "/rstp-bridge-attr"
#define ROOT_BRIDGE_PORT                              "/rstp-bridge-attr/root-bridge-port"
#define ROOT_PATH_COST                                "/rstp-bridge-attr/root-path-cost"
#define ROOT_BRIDGE_PRIORRITY                         "/rstp-bridge-attr/root-bridge-priority"
#define ROOT_BRIDGE_ID                                "/rstp-bridge-attr/root-bridge-id"
#define ROOT_HOLD_TIME                                "/rstp-bridge-attr/root-hold-time"
#define ROOT_HELLO_TIME                               "/rstp-bridge-attr/root-hello-time"
#define ROOT_MAX_AGE                                  "/rstp-bridge-attr/root-max-age"
#define ROOT_FORWARD_DELAY                            "/rstp-bridge-attr/root-forward-delay"
#define BRIDGE_ID                                     "/rstp-bridge-attr/bridge-id"
#define TOPO_CHANGE_COUNT                             "/rstp-bridge-attr/topo-change-count"
#define TIME_SINCE_TOPO_CHANGE                        "/rstp-bridge-attr/time-since-topo-change"

#define RSTP_STATE_ROOT_BRIDGE_PORT                   DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_BRIDGE_PORT
#define RSTP_STATE_ROOT_PATH_COST                     DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_PATH_COST
#define RSTP_STATE_ROOT_BRIDGE_PRIORRITY              DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_BRIDGE_PRIORRITY
#define RSTP_STATE_ROOT_BRIDGE_ID                     DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_BRIDGE_ID
#define RSTP_STATE_ROOT_HOLD_TIME                     DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_HOLD_TIME
#define RSTP_STATE_ROOT_HELLO_TIME                    DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_HELLO_TIME
#define RSTP_STATE_ROOT_MAX_AGE                       DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_MAX_AGE
#define RSTP_STATE_ROOT_FORWARD_DELAY                 DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER ROOT_FORWARD_DELAY
#define RSTP_STATE_BRIDGE_ID                          DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER BRIDGE_ID
#define RSTP_STATE_TOPO_CHANGE_COUNT                  DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER TOPO_CHANGE_COUNT
#define RSTP_STATE_TIME_SINCE_TOPO_CHANGE             DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER TIME_SINCE_TOPO_CHANGE

/****rstp-bridge-port-attr***/
#define RSTP_BRIDGE_PORT_ATTR                         "/rstp-bridge-port-attr"
#define RSTP_BRIDGE_PORT_STATE                        "/bridge-port-state"  /* enumeration */
#define RSTP_BRIDGE_PORT_ROLE                         "/bridge-port-role"   /* enumeration */
#define RSTP_BRIDGE_PORT_ID                           "/bridge-port-id"
#define RSTP_OPER_EDGE_BRIDGE_PORT_ROLE               "/oper-edge-bridge-port"  /* empty */
#define RSTP_DESIGNATED_BRIDGE_PORT                   "/designated-bridge-port"
#define RSTP_DESIGNATED_BRIDGE_ID                     "/designated-bridgeid"   /* bridge-id-type */

#define BRIDGE_PORT_STATE                             "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/bridge-port-state"  /* enumeration */
#define BRIDGE_PORT_ROLE                              "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/bridge-port-role"   /* enumeration */
#define BRIDGE_PORT_ID                                "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/bridge-port-id"
#define OPER_EDGE_BRIDGE_PORT_ROLE                    "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/oper-edge-bridge-port"  /* empty */
#define DESIGNATED_BRIDGE_PORT                        "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/designated-bridge-port"
#define DESIGNATED_BRIDGE_ID                          "/rstp-bridge-port-attr/rstp-bridge-port-table[ifname='%s']/designated-bridgeid"   /* bridge-id-type */

#define RSTP_PORT_ATTR_BRIDGE_PORT_STATE              DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER BRIDGE_PORT_STATE
#define RSTP_PORT_ATTR_BRIDGE_PORT_ROLE               DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER BRIDGE_PORT_ROLE
#define RSTP_PORT_ATTR_BRIDGE_PORT_ID                 DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER BRIDGE_PORT_ID
#define RSTP_PORT_ATTR_OPER_EDGE_BRIDGE_PORT_ROLE     DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER OPER_EDGE_BRIDGE_PORT_ROLE
#define RSTP_PORT_ATTR_DESIGNATED_BRIDGE_PORT         DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER DESIGNATED_BRIDGE_PORT
#define RSTP_PORT_ATTR_DESIGNATED_BRIDGE_ID           DEVICE_XPATH_PROTOCOLS_RSTP_STATE_HEADER DESIGNATED_BRIDGE_ID

typedef struct st_RSTP_BRIDGE_CONFIG
{
    char acBridgeName[32];
    uint32_t u32BridgePriority;
    uint32_t u32ShutDown;
    uint32_t u32HelloTime;
    uint32_t u32HoldTime;
    uint32_t u32MaxAge;
    uint32_t u32ForwardDelay;
    uint32_t u32TransmitHoldCount;
    uint32_t u32Cost[L2_PORT_IFNAME_CNT];
    uint32_t u32Priority[L2_PORT_IFNAME_CNT];
} RSTP_BRIDGE_CONFIG_ST;

typedef struct st_RSTP_BRIDGE_ATTRIBUTION
{
    /**** according yang****/
    uint32_t u32RootBridgePort;
    uint32_t u32RootPathCost;
    uint32_t u32RootBridgePriority;
    char acRootBridgeID[32];      /* bridge-id-type  string*/
    uint32_t u32RootHoldTime;
    uint32_t u32RootHelloTime;
    uint32_t u32RootMaxAge;
    uint32_t u32RootForwardDelay;
    char acBridgeID[32];      /* bridge-id-type  string*/
    uint32_t u32TopoChangeCount;
    uint32_t u32TimeSinceTopoChange;
} RSTP_BRIDGE_ATTRIBUTION_ST;

typedef struct st_RSTP_BRIDGE_PORT_ATTRIBUTION
{
    /**** according yang****/
    char acIfName[32];      /* string*/
    char acBridgePortState[32];      /* enumeration*/
    char acBridgePortRole[32];      /* enumeration*/
    uint32_t u32BridgePortID;
    uint32_t u32OperEdgeBridgePort;  /**empty***/
    uint32_t u32DesignatedBridgePort;
    char acDesignatedBridgeID[32];         /* bridge-id-type  string*/
} RSTP_BRIDGE_PORT_ATTRIBUTION_ST;

typedef struct st_RSTP_BRIDGE_STATE
{
    RSTP_BRIDGE_ATTRIBUTION_ST stRstpBridgeAttr;
    RSTP_BRIDGE_PORT_ATTRIBUTION_ST stRstpBridgePortAttr[L2_PORT_IFNAME_CNT];
} RSTP_BRIDGE_STATE_ST;

extern L2_PORT_IFNAME_EN DSC_GetL2PortIfName(char *pcXpath);
extern char *DSC_L2PortToStr(char *pcStr, L2_PORT_IFNAME_EN ifName);
extern int DSC_GetRstpBridgeState(RSTP_BRIDGE_STATE_ST *pstRstpBridgeState);
extern int DSC_GetRstpBridgeConfig(RSTP_BRIDGE_CONFIG_ST *pstRstpBridgeConfig);
extern int DSC_SetRstpBridgePriority(uint32_t u32BridgePriority);
extern int DSC_SetRstpHoldTime(uint32_t u32HoldTime);
extern int DSC_SetRstpHelloTime(uint32_t u32HelloTime);
extern int DSC_SetRstpShutDown(uint32_t u32ShutDown);
extern int DSC_SetRstpMaxAge(uint32_t u32MaxAge);
extern int DSC_SetRstpForwardDelay(uint32_t u32ForwardDelay);
extern int DSC_SetRstpTransmitHoldCount(uint32_t u32TransmitHoldCount);

extern int DSC_SetRstpBridgePortPriority(L2_PORT_IFNAME_EN enPortIfname, uint32_t u32PortPriority);
extern int DSC_SetRstpBridgePortCost(L2_PORT_IFNAME_EN enPortIfname, uint32_t u32PortCost);

#ifdef __cplusplus
}
#endif

#endif

