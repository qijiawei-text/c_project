#ifndef _DSC_STATE_H_
#define _DSC_STATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_ood.h"
#include "org_openroadm_common_equipment_types.h"
#include "org_openroadm_common_link_types.h"
#include "org_openroadm_common_alarm_pm_types.h"
#include "org_openroadm_port_types.h"
#include "org_openroadm_device_types.h"




#define DSC_SHELF_NAME_MAX_LEN            (128)
#define DSC_CP_NAME_MAX_LEN               (128)
#define DSC_CP_NUM_MAX                    (32)
#define DSC_ALARM_NUM_IN_CP_MAX           (16)
/* #define DSC_ALARM_NUM_IN_SHELF_MAX        (8) */
#define DSC_ALARM_STR_MAX_LEN             (32)
#define DSC_STATE_STR_MAX_LEN             (32)
#define DSC_LIST_NAME_KEY_MAX_LEN         (64)

#define ALARM_SEVERITY_STR_MAJOR      "major"
#define ALARM_SEVERITY_STR_CRITICAL   "critical"


typedef enum
{
    OPTICAL_EDFA_EN,
    OPTICAL_VOA_EN,
    OPTICAL_OPS_EN,
    OPTICAL_END_EN
}DSC_OPITICAL_TYEP_EN;

typedef enum
{
    DSC_ALM_SEVERITY_NONE = 0,
    DSC_ALM_SEVERITY_MAJOR,
    DSC_ALM_SEVERITY_CRITICAL
} DSC_ALM_SEVERITY_TYPE_EN;

typedef struct
{
    DSC_OPITICAL_TYEP_EN enOpiticalType;
    char acListName[DSC_LIST_NAME_KEY_MAX_LEN];
    char acKeyName[DSC_LIST_NAME_KEY_MAX_LEN];
}LIST_KEY_INFO_ST;

typedef struct
{
    char acAlarmId[DSC_ALARM_STR_MAX_LEN];
    DSC_ALM_SEVERITY_TYPE_EN enSeverity;
}ALARM_STATE_INFO_ST;

typedef struct
{
    char acCircuilPackName[DSC_CP_NAME_MAX_LEN];
    ALARM_STATE_INFO_ST astAlarmState[DSC_ALARM_NUM_IN_CP_MAX];
    uint32_t ulSupportAlarmNum;
    uint32_t ulAdminState;
    bool bServiceAffect;
}CIRCUIT_PACK_STATE_INFO_ST;

typedef struct
{
    char acShelfName[DSC_SHELF_NAME_MAX_LEN];
    /* ALARM_STATE_INFO_ST astAlarmState[DSC_ALARM_NUM_IN_SHELF_MAX]; */
    /* uint32_t ulSupportAlarmNum; */
    uint32_t ulAdminState;
}SHELF_STATE_INFO_ST;

typedef struct
{
    CIRCUIT_PACK_STATE_INFO_ST astCircuitPackState[DSC_CP_NUM_MAX];
    uint32_t ulCircuitPackNum;
    SHELF_STATE_INFO_ST stShelfState;
}SYSTEM_STATE_INFO_ST;




extern int DSC_CP_GetListKeyValue(char *pcXpath, char *pcKeyValue);
extern int DSC_SHELF_GetShelfName(char *pcName);
extern int DSC_CP_GetCpName(char aacName[][DSC_CP_NAME_MAX_LEN], size_t * pnCount);
extern char *DSC_AdminStateEnumToStr(EN_EQPT_ADMIN_STATES enAdminState, char *pcBuf);
extern int DSC_AdminStateStrToEnum(char *pcStateString, EN_EQPT_ADMIN_STATES *penAdminState);
extern char *DSC_OpStateEnumToStr(EN_STATE enOpState, char *pcBuf);
extern int DSC_OpStateStrToEnum(char *pcStateString, EN_STATE *penOpState);
extern int DSC_CP_SetAdminState(char *pcName, EN_EQPT_ADMIN_STATES enState);
extern int DSC_CP_GetAllCpInfo(ST_CIRCUIT_PACK *pstCircuitPackInfo, size_t *ptCircuitPackCount);
extern int DSC_SHELF_SetAdminState(char *pcName, EN_EQPT_ADMIN_STATES enState);
extern int DSC_SHELF_GetAdminStateStr(char *pcName, char *pcState);
extern int DSC_SHELF_GetAdminStateEnum(EN_EQPT_ADMIN_STATES *penAdminState);
extern int DSC_SHELF_GetOpState(char *pcName, char *pcState);
extern int DSC_CP_GetAdminState(char *pcName, char *pcState);
extern int DSC_CP_GetServiceAffect(char *pcName, bool *pbAffect);
extern int DSC_GetConfEnable(DSC_OPITICAL_TYEP_EN enType, char *pcXpath, bool *pbEnable);
extern int DSC_GetSystemStateInfo(SYSTEM_STATE_INFO_ST *pstSystemStateInfo);
extern int DSC_SHELF_SetOpState(char *pcName, uint32_t ulOpState);
extern int DSC_CP_GetOpState(char *pcName, char *pcState);
extern int DSC_CP_SetOpState(char *pcName, uint32_t ulOpState);
extern int DSC_GetAlarmEnable(char *pcAlarmIdStr, bool *pbEnable);
extern bool DSC_IsCircuitPackInService(const char *pcCpName);

#ifdef __cplusplus
}
#endif


#endif
