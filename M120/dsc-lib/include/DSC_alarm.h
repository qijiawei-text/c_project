#ifndef _DSC_ALARM_H
#define _DSC_ALARM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

/* alarm type yang model
    leaf type {
      description
        "Type of alarm. Based on X.733 event Type.";
      mandatory false;
      type enumeration {
        enum "communication" {
          value 1;
        }
        enum "qualityOfService" {
          value 2;
        }
        enum "processingError" {
          value 3;
        }
        enum "equipment" {
          value 4;
        }
        enum "environmental" {
          value 5;
        }
      }
    }
 */

#define ALARM_TYPE_COMM_STR      "communication"
#define ALARM_TYPE_QOS_STR       "qualityOfService"
#define ALARM_TYPE_PROC_STR      "processingError"
#define ALARM_TYPE_EQUIP_STR     "equipment"
#define ALARM_TYPE_ENV_STR       "environmental"

typedef enum en_ALARM_TYPE
{
    ALARM_TYPE_UNKNOWN = 0,
    ALARM_TYPE_COMMUNICATION,
    ALARM_TYPE_QOS,
    ALARM_TYPE_PROCESSING,
    ALARM_TYPE_EQUIPMENT,
    ALARM_TYPE_ENVIRONMENTAL
} ALARM_TYPE_EN;
/* resource type yamg model
   typedef resource-type-enum {
    type enumeration {
      enum "other" {
        value 1;
        description
          "Resource of type not found in list.";
      }
      enum "device" {
        value 2;
        description
          "device - ROAMD, Xponder, etc";
      }
      enum "degree" {
        value 3;
        description
          "degree";
      }
      enum "shared-risk-group" {
        value 4;
        description
          "shared-risk-group";
      }
      enum "connection" {
        value 5;
        description
          "connection";
      }
      enum "connection-map" {
        value 6;
        description
          "connection-map";
      }
      enum "connection-map" {
        value 6;
        description
          "connection-map";
      }
      enum "port" {
        value 7;
        description
          "port";
      }
      enum "circuit-pack" {
        value 8;
        description
          "circuit pack";
      }
      enum "internal-link" {
        value 9;
        description
          "internal-link";
      }
      enum "physical-link" {
        value 10;
        description
          "physical-link";
      }
      enum "interface" {
        value 11;
        description
          "interface";
      }
      enum "shelf" {
        value 12;
        description
          "shelf";
      }
      enum "service" {
        value 13;
        description
          "service";
      }
    }
   }
 */

#define ALARM_RSC_DEVICE_STR    "device"
#define ALARM_RSC_PORT_STR      "port"
#define ALARM_RSC_SERVICE_STR   "service"
#define ALARM_RSC_CPACK_STR     "circuit-pack"

typedef enum en_RESOURCE_TYPE
{
    /* TODO: add more */
    RESOURCE_TYPE_OTHER = 0,
    RESOURCE_TYPE_DEVICE,
    RESOURCE_TYPE_PORT,
    RESOURCE_TYPE_SERVICE,
    RESOURCE_TYPE_CIRCUIT_PACK
} REOURCE_TYPE_EN;

/* yang model
   typedef severity {
    description
      "Severity, based on X.733 perceived severity";
    type enumeration {
      enum "critical" {
        value 1;
      }
      enum "major" {
        value 2;
      }
      enum "minor" {
        value 3;
      }
      enum "warning" {
        value 4;
      }
      enum "clear" {
        value 5;
      }
      enum "indeterminate" {
        value 6;
      }
    }
   }
 */
#define ALARM_CRITICAL_STR    "critical"
#define ALARM_MAJOR_STR       "major"
#define ALARM_MINOR_STR       "minor"
#define ALARM_WARNING_STR     "warning"
#define ALARM_CLEAR_STR       "clear"
#define ALARM_INDETERMINATE_STR    "indeterminate"

typedef enum en_SEVERITY
{
    SEVERITY_CRITICAL = 1,
    SEVERITY_MAJOR,
    SEVERITY_MINOR,
    SEVERITY_WARNING,
    SEVERITY_CLEAR,
    SEVERITY_INDETERMINATE,
} SEVERITY_EN;

/* see org-openroadm-probable-cause.yang */
typedef enum en_PROPABLE_CAUSE
{
    LOSS_OF_SIGNAL = 4,
    PORT_LOSS_OF_LIGHT = 17,
    FAN_COOLING_FAIL = 53,
    POWER_PROBLEM_A = 56,
    POWER_PROBLEM_B = 57,
} PROBABLE_CAUSE_EN;

#define MAX_ALM              (512)
#define MAX_ALM_ID_LEN       (64)
typedef struct st_ALARM_INFO
{
    char acAlarmId[MAX_ALM_ID_LEN];
    char acCaseExt[128];
    char acDetail[512];
    REOURCE_TYPE_EN enResourceType;
    char acResource[32];   /* device-id, port-name */
    PROBABLE_CAUSE_EN enCause;
    ALARM_TYPE_EN enAlarmType;
    char acRaiseTime[64];   /* 2016-12-10T11:11:11Z */
    char acCircuitPack[32];
    char acSeverity[32];
    SEVERITY_EN enSeverity;
    bool bMask;
    char acExternalId[MAX_ALM_ID_LEN];
    char acEntity[MAX_ALM_ID_LEN];
} ALARM_INFO_ST;

/* only used to raise all alarm defination in meiwu project. Be convenient for analysis and processing */
typedef struct ALARM_DEFINATION_st {
    char acAlarmId[MAX_ALM_ID_LEN];
    char acCircuitPack[32];
    char acResourceType[32];
    char acResourceTypeExt[32];
    char acCause[32];
    char acCauseExt[32];
    char acAlarmType[32];
    char acRaiseTime[64];
    char acSeverity[32];
    char acExternalId[MAX_ALM_ID_LEN];
    char acEntity[MAX_ALM_ID_LEN];
} ALARM_DEFINATION_ST;


#define MODULE_COA                     "com-oplink-alarm"
#define MODULE_OOA                     "org-openroadm-alarm"
#define ALARM_XPATH                    "/org-openroadm-alarm:active-alarm-list"
#define ALARM_XPATH_LIST               ALARM_XPATH "/activeAlarms//*"
#define ALARM_XPATH_NOTIF_ID           "/id"
#define ALARM_XPATH_NOTIF_RAISETIME    "/raiseTime"
#define ALARM_XPATH_NOTIF_SERVERITY    "/severity"
#define ALARM_XPATH_NOTIF_CIRCUIT_PACK "/circuit-pack-name"
#define ALARM_XPATH_NOTIF_TYPE         "/probableCause/type"
#define ALARM_XPATH_NOTIF_RESOURCE     "/resource/resourceType/type"
#define ALARM_XPATH_NOTIF_CAUSE_EXT    "/probableCause/extension"
#define ALARM_XPATH_I                  "/com-oplink-alarm:active-alarm-list"
#define ALARM_XPATH_LIST_I             ALARM_XPATH_I "/activeAlarms"
#define ALARM_XPATH_CHILD_NODE         "/child-id"
#define ALARM_XPATH_PARENT_NODE        "/parent-id"
#define ALARM_XPATH_MASK_NODE          "/mask"
#define ALARM_XPATH_ADD_DETAIL         "/additional-detail"

#define ALARM_XPATH_NOTIF              "/org-openroadm-alarm:alarm-notification"
#define ALARM_XPATH_SUPPORTED          "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']//*"
#define ALARM_XPATH_SUPPORTED_LIST     "/com-oplink-alarm:supported-alarm-list/alarm-definition//*"
#define ALARM_XPATH_SUPPORTED_LIST_ALL     "/com-oplink-alarm:supported-alarm-list//*"

#define ALARM_XPATH_ACTIVE             "/com-oplink-alarm:active-alarm-list/activeAlarms[id='%s']"
#define ALARM_XPATH_RESOURCE_CP_NAME   "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']/resource/resource/circuit-pack-name"
#define ALARM_XPATH_PARENT_ID          "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']/parents-id" ALARM_XPATH_PARENT_NODE
#define ALARM_XPATH_CHILD_ID           "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']/children-id" ALARM_XPATH_CHILD_NODE
#define ALARM_XPATH_MASK               "/com-oplink-alarm:supported-alarm-list/alarm-definition[id='%s']/mask"
#define ALARM_XPATH_CHILDREN_ID_NODE   "/children-id"
#define ALARM_XPATH_PARENTS_ID_NODE    "/parents-id"
#define ALARM_XPATH_EXT_ID             "/external-id"
#define ALARM_XPATH_EXT_ENTITY         "/entity"
#define ALARM_XPATH_EXT_ID_OOG         "/com-oplink-alarm:external-id"
#define ALARM_XPATH_EXT_ENTITY_OOG     "/com-oplink-alarm:entity"


#define ALARM_XPATH_BACKUP             "/com-oplink-alarm:all-alarm-list/allAlarms[id='%s']"
#define RAINIER_ALARM_MAX_CNT   256

typedef int (*DSC_event_notif_cb)(ALARM_INFO_ST *ppstEvent, size_t count);

extern int DSC_GetAlarmList(ALARM_INFO_ST **ppstAlarm, size_t *count);
extern int DSC_Event_Subscribe(const char *xpath, DSC_event_notif_cb callback);


/* delete the mapping table in dsc_alarm, use the setting in yang file */
extern int DSC_GetRainierAlmList(ALARM_INFO_ST **ppstAlarm, size_t *pnCount, bool bSkipSame, bool bTca);
extern int DSC_WriteCurrentAlarm(sr_session_ctx_t *pstSess, ALARM_INFO_ST *pstAlarm);

extern int DSC_GetSupportAlarmList(ALARM_INFO_ST *pstAlarm, size_t *count);
extern int DSC_BuildAlarmList(const sr_val_t *val, const size_t cnt, ALARM_INFO_ST **ppstAlarm, size_t *count);

/* extern int DSC_NotificationAllAlarm(bool bRaise); */
extern int DSC_RaiseAlarm(char *pcExternId, char *pcEntity, bool bFlag);
extern int DSC_RaiseAllAlarm(bool bRaise);
extern int DSC_MaskAlarm(char *pcAlmId, bool bMask);

#ifdef __cplusplus
}
#endif

#endif

