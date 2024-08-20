#ifndef _DSC_PM_H
#define _DSC_PM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define MODULE_NS_OOPM                      "org-openroadm-pm"
#define PM_MODULE_INTERNAL                  "com-oplink-pm"
#define PM_XPATH_CURRENT_LIST               "/org-openroadm-pm:currentPmlist"
#define PM_XPATH_CURRENT_ALL                PM_XPATH_CURRENT_LIST "/currentPm//*"
#define PM_XPATH_HISTORICAL_LIST            "/org-openroadm-pm:historicalPmlist"
#define PM_XPATH_HISTORICAL_ALL             PM_XPATH_HISTORICAL_LIST "//*"
#define PM_XPATH_CURRENT_HEAD_I             "/com-oplink-pm:currentPmlist"
#define PM_XPATH_CURRENT_LIST_I             PM_XPATH_CURRENT_HEAD_I "/currentPm[id='%s']//*"
#define PM_XPATH_HISTORICAL_HEAD_I          "/com-oplink-pm:historicalPmlist/historicalPm"
#define PM_XPATH_HISTORICAL_LIST_I          PM_XPATH_HISTORICAL_HEAD_I "[id='%s']//*"
#define PM_XPATH_CURRENT_ID                 PM_XPATH_CURRENT_HEAD_I "/currentPm/id"
#define PM_XPATH_CURRENT_CIRCUIT_PACK_NAME  PM_XPATH_CURRENT_HEAD_I "/currentPm[id='%s']/resource/resource/port/circuit-pack-name"

#define PM_XPATH_HISTORICAL_ALL_ID          PM_XPATH_HISTORICAL_LIST "/historicalPm/id"
#define PM_XPATH_HISTORICAL_ALL_GRANULARITY PM_XPATH_HISTORICAL_LIST "/historicalPm[id='%s']/measurements[granularity='%s']/binned-pm//*"
#define PM_XPATH_HISTORICAL_ALL_BINNUM      PM_XPATH_HISTORICAL_LIST "/historicalPm[id='%s']/measurements[granularity='%s']/binned-pm[bin-number='%d']//*"
#define PM_XPATH_HISTORICAL_UNIT            PM_XPATH_HISTORICAL_LIST "/historicalPm[id='%s']/measurements[granularity='1min']/binned-pm[bin-number='1']/measurement/pmParameterUnit"


#define PM_XPATH_PM_ID                      "/id"   /* item */
#define PM_XPATH_PM_GRANULARITY             "/granularity"           /* 1 and 15 minutes or 24 hours */
#define PM_XPATH_PM_RETRIEVAL_TIME          "/retrievalTime"

#define PM_XPATH_PM_MIN_VALUE               "/pmParameterValue[type='min']/value"
#define PM_XPATH_PM_MAX_VALUE               "/pmParameterValue[type='max']/value"
#define PM_XPATH_PM_AVERAGE_VALUE           "/pmParameterValue[type='average']/value"
#define PM_XPATH_PM_MIN_TIMESTAMP           "/pmParameterValue[type='min']/com-oplink-pm-ext:timestamp"
#define PM_XPATH_PM_MAX_TIMESTAMP           "/pmParameterValue[type='max']/com-oplink-pm-ext:timestamp"
#define PM_XPATH_PM_AVERAGE_TIMESTAMP       "/pmParameterValue[type='average']/com-oplink-pm-ext:timestamp"

#define PM_XPATH_PM_UNIT                    "/pmParameterUnit"
#define PM_XPATH_PM_COMPLETION_TIME         "/completionTime"
#define PM_XPATH_PM_BIN_NUMBER              "/bin-number"
#define PM_XPATH_PM_VALIDITY                "/validity"


/*******Clear PM*********/
#define RPC_XPATH_CLEAR_PM                  "/org-openroadm-pm:clear-pm"
#define RPC_XPATH_CLEAR_PM_RESOURCE_TYPE    RPC_XPATH_CLEAR_PM "/resourceType/type"
#define PM_TYPE                             "/pm-type"
#define RPC_XPATH_CLEAR_PM_TYPE             RPC_XPATH_CLEAR_PM PM_TYPE
#define PM_GRANULARITY                      "/granularity"
#define RPC_XPATH_CLEAR_PM_GRANULARITY      RPC_XPATH_CLEAR_PM PM_GRANULARITY
#define RPC_XPATH_CLEAR_PM_STATUS           RPC_XPATH_CLEAR_PM "/status"
#define RPC_XPATH_CLEAR_PM_MESSAGE          RPC_XPATH_CLEAR_PM "/status-message"

#define PM_STR_1_MIN                        "1min"
#define PM_STR_15_MIN                       "15min"
#define PM_STR_24_HOUR                      "24Hour"
#define PM_STR_NOT_ACCEPT                   "notApplicable"

#define PM_STR_CURRENT                      "current"
#define PM_STR_ALL                          "all"

#define PM_STR_RETRIEVAL_TIME               "retrievalTime"
#define PM_STR_GRANULARITY                  "granularity"
#define PM_STR_CIRCUIT_PACK_NAME            "circuit-pack-name"
#define PM_STR_VALUE                        "value"
#define PM_STR_ID                           "id"
#define PM_STR_MIN                          "min"
#define PM_STR_MAX                          "max"
#define PM_STR_AVERAGE                      "average"
#define PM_STR_OPR_LOW_SECOND               "OPR-low-second"
#define PM_STR_OPR_LOS_SECOND               "OPR-los-second"
#define PM_STR_SECOND                       "-sec"          /* change second to sec for downgrade for 1.07.0001 to 1.04.0002, history.bin issue */

#define UNIT_NAME_UKNOWN                    "unknown"
#define UNIT_NAME_DBM                       "dBm"
#define UNIT_NAME_DB                        "dB"
#define UNIT_NAME_MA                        "mA"
#define UNIT_NAME_CENTIGRADE                "DegreeCentigrade"
#define UNIT_NAME_SECOND                    "s"
#define UNIT_NAME_AMPS                      "amps"
#define UNIT_NAME_VOLTS                     "volts"
#define UNIT_NAME_RPM                       "rpm"
#define UNIT_NAME_WATTS                     "watts"
typedef enum {
    EN_PARA_UNIT_UNKNOWN = 0,
    EN_PARA_UNIT_DB,
    EN_PARA_UNIT_DBM,
    EN_PARA_UNIT_MA,
    EN_PARA_UNIT_CENTIGRADE,
    EN_PARA_UNIT_SECOND,
    EN_PARA_UNIT_AMPS,
    EN_PARA_UNIT_VOLTS,
    EN_PARA_UNIT_RPM,
    EN_PARA_UNIT_WATTS,
    EN_PARA_UNIT_MAX
} EN_PM_PARA_UNIT;

/******pm collection*****/
#define RPC_XPATH_PM_COLLECTION             "/org-openroadm-pm:pm-collection"
#define RPC_XPATH_PM_COLLECTION_FILENAME    RPC_XPATH_PM_COLLECTION "/collections[1]/filename"
#define RPC_XPATH_PM_COLLECTION_TYPE        RPC_XPATH_PM_COLLECTION "/collections[1]/collection-type"
#define RPC_XPATH_PM_COLLECTION_INTERVAL    RPC_XPATH_PM_COLLECTION "/collections[1]/interval"

#define RPC_XPATH_PM_COLLECTION_STATUS      RPC_XPATH_PM_COLLECTION "/status"
#define RPC_XPATH_PM_COLLECTION_MESSAGE     RPC_XPATH_PM_COLLECTION "/status-message"

#define PM_COLLECTION_CURRENT_STR           "current"
#define PM_COLLECTION_HISTORY_STR           "historical"

#define PM_STRING_LEN 64
typedef struct {
    char acPmId[PM_STRING_LEN];
    char acTime[PM_STRING_LEN];
    char acUnit[PM_STRING_LEN];
    double dMin;
    double dMax;
    double dAvg;
    char acMinTime[PM_STRING_LEN];
    char acMaxTime[PM_STRING_LEN];
} ST_DSC_PM_DATA;


#define DSC_PM_1MIN_HIS_CNT        15
#define DSC_PM_15MIN_HIS_CNT       96
#define DSC_PM_24HOUR_HIS_CNT      7

/* 9*(15+96+7)*/
#define PM_HIS_ATTR_MAX_NUM        (9 * (DSC_PM_15MIN_HIS_CNT + DSC_PM_24HOUR_HIS_CNT))

typedef struct {
    ST_DSC_PM_DATA ast1MinHisPm[DSC_PM_1MIN_HIS_CNT];
    ST_DSC_PM_DATA ast15MinHisPm[DSC_PM_15MIN_HIS_CNT];
    ST_DSC_PM_DATA ast24HourHisPm[DSC_PM_24HOUR_HIS_CNT];
} ST_DSC_PM_HIS;

typedef enum {
    EN_DSC_PM_1MIN = 0,
    EN_DSC_PM_15MIN,
    EN_DSC_PM_24HOUR,
    EN_DSC_PM_GRANULARITIES_MAX
} EN_DSC_PM_GRANULARITIES;

extern int DSC_GetCurrentPM(ST_DSC_PM_DATA aastPmUnit[][EN_DSC_PM_GRANULARITIES_MAX], size_t * pnCnt);
extern int DSC_GetHistoryPM(EN_DSC_PM_GRANULARITIES gran, ST_DSC_PM_HIS **ppstPmHis, size_t *pnCnt);
extern int DSC_GetPMData(char *acPmId, char *acGran, uint32_t uiBinNum, ST_DSC_PM_DATA *pstPmUnit);
extern int DSC_PmCollection(char *pcFileName, char *pcType, char *pcInterval);
extern char *DSC_PmGetUnitString(char *pcString, EN_PM_PARA_UNIT enUnit);
extern EN_PM_PARA_UNIT DSC_PmGetUnitEnum(char *pcString);
extern int DSC_PmGetIdKey(char *pcXpath, char *pcKey, size_t nMaxLen);


#define MOUDLE_NS_OC_PM               "openconfig-transport-pms"
#define OC_PM_XPATH_PM_POINTS         "/openconfig-transport-pms:pm-points"
#define OC_PM_XPATH_PM_POINT          OC_PM_XPATH_PM_POINTS "/pm-point"
#define OC_PM_XPATH_PM_POINT_LIST     OC_PM_XPATH_PM_POINTS "/pm-point[entity-name=\"%s\"]"
#define OC_PM_XPATH_PM_SETS           OC_PM_XPATH_PM_POINT_LIST "/config/pm-sets"
#define OC_PM_XPATH_PM_SET_LIST       OC_PM_XPATH_PM_SETS "/pm-set[pm-time-period=\"%s\"][pmp-type=\"%s\"]"
#define OC_PM_XPATH_PM_THR_LIST       OC_PM_XPATH_PM_SET_LIST "/pm-thresholds/pm-threshold[pm-parameter=\"%s\"]"

#define OC_PM_XPATH_STATE_PM_THR_LIST  OC_PM_XPATH_PM_POINT_LIST "/state/pm-sets/pm-set[pm-time-period=\"%s\"][pmp-type=\"%s\"]/pm-thresholds/pm-threshold[pm-parameter=\"%s\"]"

#define OC_PM_NODE_PM_POINT           "pm-point"
#define OC_PM_NODE_PM_SET             "pm-set"
#define OC_PM_NODE_PM_THR             "pm-threshold"

#define OC_PM_NODE_ENTITY_NAME        "entity-name"
#define OC_PM_NODE_PM_TIME_PERIOD     "pm-time-period"
#define OC_PM_NODE_PMP_TYPE           "pmp-type"

#define OC_PM_NODE_PM_PARAMETER       "pm-parameter"
#define OC_PM_NODE_PM_HIGH_THR        "pm-high-threshold"
#define OC_PM_NODE_PM_LOW_THR         "pm-low-threshold"
#define OC_PM_NODE_PM_UINT            "pm-unit"

#define OC_PM_XPATH_STATE             OC_PM_XPATH_PM_POINT "/state"

/* pm-time-period */
#define OC_PM_NOT_APPLICABLE_STR      "not-applicable"
#define OC_PM_15MIN_STR               "15min"
#define OC_PM_24H_STR                 "24h"

#define OC_PM_TYPE_OPTICAL_POWER_STR  "optical-power"
#define OC_PM_PARA_OPR_INST_STR       "OPR-inst"

#define PM_THR_NUMBER_MAX         (3)
#define PM_SET_NUMBER_MAX         (3)

#define PM_ENTITY_NAME_IN65_STR       "In65"
#define PM_ENTITY_NAME_IN66_STR       "In66"

typedef enum ENTITY_IDX_en {
    EN_ENTITY_IN65 = 0,
    EN_ENTITY_IN66,
    EN_ENTITY_MAX
} ENTITY_IDX_EN;

typedef struct {
    ENTITY_IDX_EN enIdx;
    char acName[32];
} ENTITY_TABLE_ST;

typedef struct OC_PM_THR_st {
    char acPara[16];
    double dHighThr;
    double dLowThr;
    char acUnit[16];
} OC_PM_THR_ST;

typedef struct {
    OC_PM_THR_ST astThr[PM_THR_NUMBER_MAX];
    uint32_t uiTotal;
} OC_PM_THR_LIST_ST;

typedef struct OC_PM_SET_st {
    char acTimePeriod[32];
    char acType[32];
    bool bSuperSwitch;
    bool bThrSwitch;
    bool bHisRecording;
    OC_PM_THR_LIST_ST stThrList;
} OC_PM_SET_ST;

typedef struct {
    OC_PM_SET_ST astPmSet[PM_SET_NUMBER_MAX];
    uint32_t uiTotal;
} OC_PM_SET_LIST_ST;

typedef struct {
    char acEntityName[32];
    OC_PM_SET_LIST_ST stSetList;
} OC_PM_POINT_ST;

typedef struct {
    OC_PM_POINT_ST *pstPmPoint;
    uint32_t uiTotal;
} OC_PM_POINT_LIST_ST;

#define MAX_ID_CNT (128)

typedef struct {
    uint32_t uiCnt;
    char aacIdList[MAX_ID_CNT][64];
} PM_ID_LIST_ST;

int DSC_SetOcPmThreshold(sr_session_ctx_t *pstSess, OC_PM_POINT_ST *pstPmPoint);

int DSC_GetEntityIdx(char *pcEntityName);

int DSC_SetOcPmThresholdOfOlp(sr_session_ctx_t *pstSess, ENTITY_IDX_EN enEntityIdx, double dHighThr, double dLowThr);

int DSC_GetPmIdList(char *pcMoudleName, char *pcSubName, PM_ID_LIST_ST *pstPmIdList, PM_ID_LIST_ST *pstSubPmIdList);

int DSC_ClearPm(char *pcType, char *pcGranularity);

#ifdef __cplusplus
}
#endif

#endif

