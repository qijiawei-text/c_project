
#ifndef DSC_TELEMETRY_H
#define DSC_TELEMETRY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "DSC_common.h"


#define TELEMETRY_MODE_ALL                    "ALL"
#define TELEMETRY_MODE_STRICT_PRIORITY        "STRICT_PRIORITY"
#define TELEMETRY_MODE_TARGET_DEFINED         "TARGET_DEFINED"


#define GRPC_STATE_CONNECTING                 "CONNECTING"
#define GRPC_STATE_READY                      "READY"
#define GRPC_STATE_TRANSIENT_FAILURE          "TRANSIENT_FAILURE"
#define GRPC_STATE_IDLE                       "IDLE"
#define GRPC_STATE_SHUTDOWN                   "SHUTDOWN"



#define SUBSCRIPTION_NAME         "sub-all"
#define DEST_GROUP_NAME           "dest-all"
#define SENSOR_GROUP_NAME         "sensor-all"


#define SUBSCRIPTION_GROUP_LIST_NAME                 "persistent-subscription"
#define SUBSCRIPTION_SENSOR_GROUP_LIST_NAME          "sensor-profile"
#define SUBSCRIPTION_DEST_GROUP_LIST_NAME            "destination-group"
#define SENSOR_GROUP_LIST_NAME                       "sensor-group"
#define DEST_GROUP_LIST_NAME                         "destination-group"
#define DEST_LIST_NAME                               "destination"

#define SENSOR_LIST_NAME                             "sensor-path"
#define SENSOR_PATH_KEY_NAME                         "path"

#define SUBSCRIPTION_GROUP_KEY_NAME                  "name"
#define SUBSCRIPTION_SENSOR_GROUP_KEY_NAME           "sensor-group"
#define SUBSCRIPTION_DEST_GROUP_KEY_NAME             "group-id"
#define SENSOR_GROUP_KEY_NAME                        "sensor-group-id"
#define DEST_GROUP_KEY_NAME                          "group-id"
#define DEST_IP_KEY_NAME                             "destination-address"
#define DEST_PORT_KEY_NAME                           "destination-port"
#define INTERVAL_TIME_LEAF                           "sample-interval"




#define TELEMETRY_MODULE                                                "openconfig-telemetry"
#define TELEMETRY_SYSTEM                                                "/openconfig-telemetry:telemetry-system"

#define TELEMETRY_SENSOR_GROUPS                                         TELEMETRY_SYSTEM "/sensor-groups"
#define TELEMETRY_SENSOR_GROUPS_LIST_ALL                                TELEMETRY_SENSOR_GROUPS "//*"
#define TELEMETRY_SENSOR_GROUPS_LIST                                    TELEMETRY_SENSOR_GROUPS "/sensor-group[sensor-group-id='%s']"
#define TELEMETRY_SENSOR_GROUPS_LIST_ALL_CONFIG                         TELEMETRY_SENSOR_GROUPS "/sensor-group/config//*"
#define TELEMETRY_SENSOR_GROUPS_LIST_CONFIG_GROUP_ID                    TELEMETRY_SENSOR_GROUPS_LIST "/config/sensor-group-id"
#define TELEMETRY_SENSOR_GROUPS_LIST_SENSOR                             TELEMETRY_SENSOR_GROUPS_LIST "/sensor-paths/sensor-path"
#define TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST                        TELEMETRY_SENSOR_GROUPS_LIST "/sensor-paths/sensor-path[path='%s']"
#define TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST_CONFIG_PATH            TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST "/config/path"
#define TELEMETRY_SENSOR_GROUPS_LIST_SENSOR_LIST_CONFIG_PATH_ALL        TELEMETRY_SENSOR_GROUPS "/sensor-group/sensor-paths/sensor-path/config//*"


#define TELEMETRY_DEST_GROUPS                                           TELEMETRY_SYSTEM "/destination-groups"
#define TELEMETRY_DEST_GROUPS_LIST_ALL                                  TELEMETRY_DEST_GROUPS "//*"
#define TELEMETRY_DEST_GROUPS_LIST                                      TELEMETRY_DEST_GROUPS "/destination-group[group-id='%s']"
#define TELEMETRY_DEST_GROUPS_LIST_ALL_CONFIG                           TELEMETRY_DEST_GROUPS "/destination-group/config//*"
#define TELEMETRY_DEST_GROUPS_LIST_CONFIG_GROUP_ID                      TELEMETRY_DEST_GROUPS_LIST "/config/group-id"
#define TELEMETRY_DEST_GROUPS_LIST_DEST                                 TELEMETRY_DEST_GROUPS_LIST "/destinations/destination"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST                            TELEMETRY_DEST_GROUPS_LIST "/destinations/destination[destination-address='%s'][destination-port='%d']"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_IP                  TELEMETRY_DEST_GROUPS_LIST_DEST_LIST "/config/destination-address"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_PORT                TELEMETRY_DEST_GROUPS_LIST_DEST_LIST "/config/destination-port"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_PRIORITY            TELEMETRY_DEST_GROUPS_LIST_DEST_LIST "/config/com-alibaba-telemetry-ext:priority"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_CONFIG_ALL                 TELEMETRY_DEST_GROUPS "/destination-group/destinations/destination/config//*"

#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_STATE_CONN_STATE           TELEMETRY_DEST_GROUPS_LIST_DEST_LIST "/state/com-alibaba-telemetry-ext:conn-state"
#define TELEMETRY_DEST_GROUPS_LIST_DEST_LIST_STATE_IS_SELECTED          TELEMETRY_DEST_GROUPS_LIST_DEST_LIST "/state/com-alibaba-telemetry-ext:is-selected"


#define TELEMETRY_SUBSCRIPTION_GROUPS                                   TELEMETRY_SYSTEM "/subscriptions"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT                        TELEMETRY_SUBSCRIPTION_GROUPS "/persistent-subscriptions"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST_ALL               TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT "//*"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST                   TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT "/persistent-subscription[name='%s']"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST_ALL_CONFIG        TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT "/persistent-subscription/config//*"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_NAME            TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/config/name"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_MODE            TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/config/com-alibaba-telemetry-ext:telemetry-mode"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_PROTOCOL        TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/config/protocol"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_CONFIG_ENCODING        TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/config/encoding"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR                 TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/sensor-profiles"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_PROFILE         TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR "/sensor-profile"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST            TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR "/sensor-profile[sensor-group='%s']"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_ALL_CONFIG TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT "/persistent-subscription/sensor-profiles/sensor-profile/config//*"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_CONFIG     TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST "/config"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_GROUP_IP   TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_CONFIG "/sensor-group"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_SAMPLE     TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_CONFIG "/sample-interval"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_HEARTBEAT  TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_SENSOR_LIST_CONFIG "/heartbeat-interval"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST                   TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_LIST "/destination-groups"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_GROUP             TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST "/destination-group"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST              TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST "/destination-group[group-id='%s']"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ID    TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST "/config/group-id"
#define TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT_DEST_LIST_CONFIG_ALL   TELEMETRY_SUBSCRIPTION_GROUPS_PERSISTENT "/persistent-subscription/destination-groups/destination-group/config//*"




#define ALARM_CHANGE_NUM_MAX                     (96)


#define IP_PRIORITY_NUM_MAX                      (32)
#define DSC_IN_SUBSCRIPTION_IP_NUM_MAX           (16)


#define DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN      (32)
#define DSC_ADDR_GROUP_NAME_MAX_LEN              (32)
#define DSC_SENSOR_GROUP_NAME_MAX_LEN            (32)
#define DSC_IP_ADDRESS_MAX_LEN                   (40)
#define DSC_L4_PORT_MAX_LEN                      (8)

#define DSC_IN_SUBSCRIPTION_SENSOR_GROUP_NUM_MAX (4)
#define DSC_IN_SUBSCRIPTION_ADDR_GROUP_NUM_MAX   (4)

#define DSC_SUBSCRIPTIONS_NUM_MAX                (4)
#define DSC_ADDR_GROUP_NUM_MAX                   (4)
#define DSC_IN_ADDR_GROUP_IP_NUM_MAX             (4)
#define DSC_SENSOR_GROUP_NUM_MAX                 (4)
#define DSC_IN_SENSOR_GROUP_XPATH_NUM_MAX        (128)
#define DSC_XPATH_LEN_MAX                        (512)

typedef enum en_TELEMETRY_SUBS_MODE
{
    TELEMETRY_SUBS_MODE_TARGET_DEFINED = 0,
    TELEMETRY_SUBS_MODE_STRICT_PRIORITY,
    TELEMETRY_SUBS_MODE_ALL,
} TELEMETRY_SUBS_MODE_TYPE_EN;


typedef struct
{
    bool bChange;
    bool bIsAlarm;
    bool bIsCreate;
    char acAlarmId[32];
}TELEMETRY_ALARM_CHANGE_INFO_ST;

typedef struct
{
    char acSubsName[DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN];
    char acDestName[DSC_ADDR_GROUP_NAME_MAX_LEN];
    char acIp[DSC_IP_ADDRESS_MAX_LEN];
    uint16_t usPort;
    bool bConOk;
    bool bSendState;
    uint32_t uiRandPri;
    int iConState;
    bool bIsSelected;
}TELEMETRY_SUBS_INFO_ST;

typedef struct
{
    char acIpAddress[DSC_IP_ADDRESS_MAX_LEN];
    uint16_t usPort;
    uint16_t usPri;
    char acConnState[32];
    bool bIsSelected;
}TELEMETRY_ADDR_ENTRY_ST;

typedef struct
{
    char acName[DSC_ADDR_GROUP_NAME_MAX_LEN];
    TELEMETRY_ADDR_ENTRY_ST astRemoteAddr[DSC_IN_ADDR_GROUP_IP_NUM_MAX];
}TELEMETRY_ADDR_GROUP_ST;

typedef struct
{
    char acPath[DSC_XPATH_LEN_MAX];
}TELEMETRY_PATH_ST;

typedef struct
{
    char acName[DSC_SENSOR_GROUP_NAME_MAX_LEN];
    TELEMETRY_PATH_ST astPath[DSC_IN_SENSOR_GROUP_XPATH_NUM_MAX];
}TELEMETRY_SENSOR_INFO_ST;

typedef struct
{
    char acName[DSC_SENSOR_GROUP_NAME_MAX_LEN];
    uint64_t ullIntervalTime;
    uint64_t ullHeartBeat;
    bool bHasHeartbeat;
    bool bPathValChangeTopFlag;
    TELEMETRY_ALARM_CHANGE_INFO_ST astChangeInfo[ALARM_CHANGE_NUM_MAX];
}TELEMETRY_SENSOR_GROUP_ST;

typedef struct
{
    char acSubsName[DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN];
    char aacAddrGroupName[DSC_IN_SUBSCRIPTION_ADDR_GROUP_NUM_MAX][DSC_ADDR_GROUP_NAME_MAX_LEN];
    TELEMETRY_SENSOR_GROUP_ST astSensorGroupInfo[DSC_IN_SUBSCRIPTION_SENSOR_GROUP_NUM_MAX];
    int iMode;
}TELEMETRY_SUBSCRIPTION_GROUP_ST;

typedef struct
{
    TELEMETRY_SUBSCRIPTION_GROUP_ST astSubScriptionGroup[DSC_SUBSCRIPTIONS_NUM_MAX];
}TELEMETRY_SUBSCRIPTION_GROUPS_ST;

typedef struct
{
    char acSensorGroupName[DSC_SENSOR_GROUP_NAME_MAX_LEN];
    uint64_t ullIntervalTime;
    uint64_t ullHeartBeat;
    uint32_t uiPathNum;
    bool bValChange;
    TELEMETRY_ALARM_CHANGE_INFO_ST astChangeInfo[ALARM_CHANGE_NUM_MAX];
    TELEMETRY_PATH_ST astPathVal[DSC_IN_SENSOR_GROUP_XPATH_NUM_MAX];
}TELEMETRY_DIALOUT_SENSOR_ENTRY_ST;

typedef struct
{
    char acSubsName[DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN];
    TELEMETRY_DIALOUT_SENSOR_ENTRY_ST astSensorEntry[DSC_IN_SUBSCRIPTION_SENSOR_GROUP_NUM_MAX];
    int iSensorGroupNum;
}TELEMETRY_DIALOUT_SENSOR_INFO_ST;

typedef struct
{
    char acAddrName[DSC_ADDR_GROUP_NAME_MAX_LEN];
    TELEMETRY_ADDR_ENTRY_ST astAddrInfo[DSC_IN_ADDR_GROUP_IP_NUM_MAX];
    uint32_t uiAddrNum;
}TELEMETRY_DIALOUT_ADDR_ENTRY_ST;

typedef struct
{
    char acSubsName[DSC_SUBSCRIPTION_GROUP_NAME_MAX_LEN];
    TELEMETRY_DIALOUT_ADDR_ENTRY_ST astAddrEntry[DSC_IN_SUBSCRIPTION_ADDR_GROUP_NUM_MAX];
    uint32_t iAddrGroupNum;
}TELEMETRY_DIALOUT_ADDR_INFO_ST;


extern char *DSC_GetModeFromCli(char *pcMode);
extern int DSC_SubsModeToEnum(char *pcMode);
extern char *DSC_SubsModeToStr(int iMode);
extern int DSC_GetTelemetrySubsInfo(TELEMETRY_SUBSCRIPTION_GROUPS_ST *pstSubsInfo);
extern int DSC_GetTelemetrySensorInfo(TELEMETRY_SENSOR_INFO_ST *pstSensorInfo);
extern int DSC_GetTelemetryAddrInfo(bool bIsRunningSess, TELEMETRY_ADDR_GROUP_ST *pstAddrInfo);
extern int DSC_AddPathToSensorGroup(char *pcSensorGroupName, char *pcPath);
extern int DSC_DelPathFromSensorGroup(char *pcSensorGroupName, char *pcPath);
extern int DSC_DelSensorGroup(char *pcSensorGroupName);
extern int DSC_AddAddrToDestGroup(char *pcDestGroupName, char *pcIp, uint16_t usPort, bool bHasPri, uint16_t usPri);
extern int DSC_DelAddrFromDestGroup(char *pcDestGroupName, char *pcIp, uint16_t usPort);
extern int DSC_DelDestGroup(char *pcDestGroupName);
extern int DSC_AddSubscription(char *pcSubsGroupName, char *pcMode, char *pcSensorGroupName, char *pcDestGroupName,
                               uint64_t ullSampleInterval, bool bHasHeartbeat, uint64_t ullHeartbeatInterval);
extern int DSC_DelSubsGroup(char *pcSubsGroupName);
extern int DSC_SetSubsSensor(char *pcSubsGroupName, char *pcSensorGroupName,
                             uint64_t ullSampleInterval, bool bHasHeartbeat, uint64_t ullHeartbeatInterval);
extern int DSC_SetSubsMode(char *pcSubsGroupName, char *pcMode);
extern int DSC_DelSubsSensor(char *pcSubsGroupName, char *pcSensorGroupName);
extern int DSC_SetSubsDest(char *pcSubsGroupName, char *pcDestGroupName);
extern int DSC_DelSubsDest(char *pcSubsGroupName, char *pcDestGroupName);




#ifdef __cplusplus
}
#endif

#endif /* DSC_TELEMETRY_H */

