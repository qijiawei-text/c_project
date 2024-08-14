#ifndef _DSC_OTDR_H
#define _DSC_OTDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define OTDR_KEY_MAX (10) /* ports */
#define OTDR_TRACE_MAX (10)

typedef enum {
    EN_OTDR_IDX_W = 0,
    EN_OTDR_IDX_P,
    EN_OTDR_IDX_MAX
} OTDR_IDX_EN;

typedef enum {
    OTDR_NONE_PORT = 0,
    OTDR_LINE_IN_PORT,
    OTDR_LINE_OUT_PORT,
    OTDR_EXTERNAL_PORT,
    OTDR_LINE_IN_P_PORT,
    OTDR_LINE_OUT_P_PORT,
    OTDR_EXTERNAL_P_PORT,
    OTDR_PORT_CNT
} OTDR_PORT_EN;

typedef struct  st_otdr_key {
    int32_t iCnt;
    char aacOtdrKeys[OTDR_KEY_MAX][OPLK_NAME_MAX_LEN];
} OTDR_KEY_ST;

#define OTDR_CFG_KEY    "OTDR"
#define OTDR_P_CFG_KEY    "OTDR-P"

#define OTDR_STR_EXTERNAL_PORT  "otdr-out"
#define OTDR_STR_LINE_IN_PORT   "line-in"
#define OTDR_STR_LINE_OUT_PORT  "line-out"
#define OTDR_STR_EXTERNAL_P_PORT  "otdr-out-p"
#define OTDR_STR_LINE_IN_P_PORT   "line-in-p"
#define OTDR_STR_LINE_OUT_P_PORT  "line-out-p"

#define OTDR_SOR                "-SORS"
#define OTDR_SOR_KEY            OTDR_CFG_KEY OTDR_SOR
#define OTDR_P_SOR_KEY          OTDR_P_CFG_KEY OTDR_SOR
typedef enum {
    OTDR_MODE_IDLE = 0,
    OTDR_MODE_MEASURING,
    OTDR_MODE_PENDING,
    OTDR_MODE_UNKNOWN,
    OTDR_MODE_CNT
} OTDR_MODE_EN;

#define OTDR_STR_MODE_IDLE        "idle"
#define OTDR_STR_MODE_MEASURING   "measuring"
#define OTDR_STR_MODE_PENDING     "pending"
#define OTDR_STR_MODE_UNKNOWN     "unknown"


typedef enum {
    OTDR_EVENT_RET_NON_REF = 0,
    OTDR_EVENT_RET_REF,
    OTDR_EVENT_RET_FIBER_END,

    OTDR_EVENT_RET_CNT
} OTDR_EVENT_RET_EN;
#define OTDR_STR_EVENT_TYPE_NO_REF     "non-reflective"
#define OTDR_STR_EVENT_TYPE_REF        "reflective"
#define OTDR_STR_EVENT_TYPE_FIBER_END  "fiber-end"

typedef struct {
    uint32_t ulDistRange;
    uint32_t ulPulseWidth;
    double dSampleResolution;
} OTDR_PARA_ST;

typedef enum {
    OTDR_SCAN_TYPE_SHORT = 0,
    OTDR_SCAN_TYPE_MED,
    OTDR_SCAN_TYPE_LONG,
    OTDR_SCAN_TYPE_CUSTOM,
    OTDR_SCAN_TYPE_AUTO,
    OTDR_SCAN_TYPE_CNT
} OTDR_SCAN_TYPE_EN;

#define OTDR_STR_SCAN_TYPE_CUSTOM    "customized"
#define OTDR_STR_SCAN_TYPE_SHORT     "short"
#define OTDR_STR_SCAN_TYPE_MED       "medium"
#define OTDR_STR_SCAN_TYPE_LONG      "long"
#define OTDR_STR_SCAN_TYPE_AUTO      "auto"

typedef enum {
    OTDR_SCAN_OPT_START = 0,
    OTDR_SCAN_OPT_STOP,
    OTDR_SCAN_OPT_CNT
} OTDR_SCAN_OPT_EN;
#define OTDR_STR_SCAN_OPT_START     "start"
#define OTDR_STR_SCAN_OPT_STOP      "stop"

typedef enum {
    OTDR_TRIGGER_MANUAL = 0,
    OTDR_TRIGGER_LOF_SET,
    OTDR_TRIGGER_LOF_CLEAR,
    OTDR_TRIGGER_EXTERNAL,
    OTDR_TRIGGER_CNT
} OTDR_TRIGGER_EN;

#define OTDR_STR_TRIGGER_MANUAL       "manual"
#define OTDR_STR_TRIGGER_LOF_SET      "olf-set"
#define OTDR_STR_TRIGGER_LOF_CLEAR    "olf-clear"
#define OTDR_STR_TRIGGER_EXTERNAL     "external"

typedef struct {
    double dRefLosThr;
    double dSpliceLosThr;
    double dFiberEndThr;
    double reflectiveIndex;
    double backscatterIndex;
} OTDR_THR_ST;

typedef struct {
    OTDR_SCAN_TYPE_EN enScanType;
    uint32_t ulMeasuringTime;
    OTDR_PARA_ST stPara;
    OTDR_THR_ST stThr;
} OTDR_CFG_ST;

typedef struct {
    OTDR_EVENT_RET_EN enType;
    double dEventLoss;
    double dReflectance;
    uint32_t ulFiberLength;      /* location */
} OTDR_EVENT_ST;

#define OTDR_EVENT_MAX_CNT    (99)
typedef struct {
    char acCompleteTime[OPLK_NAME_MAX_LEN];
    uint32_t ulFiberLength;
    double dTotalLoss;
    double dReturnLoss;
    double dAverageLoss;
    double dLoss;
    OTDR_TRIGGER_EN trigger;
    OTDR_SCAN_TYPE_EN scanType;
    char acSorfile[OPLK_BUF_MAX_LEN];
    bool baseline;
    uint32_t ulEventCnt;
    OTDR_EVENT_ST astEvent[OTDR_EVENT_MAX_CNT];
} OTDR_TRACE_ST;

typedef struct {
    char acPortName[OPLK_NAME_MAX_LEN];
    OTDR_MODE_EN portMode;
    int traceCnt;
    OTDR_TRACE_ST traces[OTDR_TRACE_MAX];
} OTDR_STATE_ST;

#define MODULE_NS_OTDR                "openconfig-gnoi-otdr"
#define MODULE_NS_OTDR_I              "com-oplink-gnoi-otdr"
#define OTDR_XPATH_ROOT               "/openconfig-gnoi-otdr:optical-time-domain-reflectometer"
#define OTDR_XPATH_ROOT_I             "/com-oplink-gnoi-otdr:optical-time-domain-reflectometer"

/*Common config for all ports */
#define OTDR_XPATH_COMMON             OTDR_XPATH_ROOT "/config-common"
#define OTDR_XPATH_COMMON_ALL         OTDR_XPATH_COMMON "//*"
#define OTDR_XPATH_SCAN_TYPES         OTDR_XPATH_COMMON "/scan-types[scan-type='%s']"
#define OTDR_XPATH_FIBER_PROFILE      OTDR_XPATH_COMMON "/fiber-profile"

#define OTDR_SCAN_TYPE_NODE           "scan-type"
#define OTDR_SCAN_TYPE_XPATH          OTDR_XPATH_SCAN_TYPES "/" OTDR_SCAN_TYPE_NODE
#define OTDR_MEASURING_TIME_NODE      "acquisition-time-s"
#define OTDR_MEASURING_TIME_XPATH     OTDR_XPATH_SCAN_TYPES "/" OTDR_MEASURING_TIME_NODE
#define OTDR_PULSE_WIDTH_NODE         "pulse-width-ns"
#define OTDR_PULSE_WIDTH_XPATH        OTDR_XPATH_SCAN_TYPES "/" OTDR_PULSE_WIDTH_NODE
#define OTDR_WAVELENGTH_NODE          "wavelength-mhz"
#define OTDR_WAVELENGTH_XPATH         OTDR_XPATH_SCAN_TYPES "/" OTDR_WAVELENGTH_NODE
#define OTDR_DIST_RANGE_NODE          "range-m"
#define OTDR_DIST_RANGE_XPATH         OTDR_XPATH_SCAN_TYPES "/" OTDR_DIST_RANGE_NODE
#define OTDR_SAMP_RESOL_NODE          "sampling-resolution-m"
#define OTDR_SAMP_RESOL_XPATH         OTDR_XPATH_SCAN_TYPES "/" OTDR_SAMP_RESOL_NODE
#define OTDR_FIBER_TYPE_NODE          "fiber-type"
#define OTDR_FIBER_TYPE_XPATH         OTDR_XPATH_SCAN_TYPES "/" OTDR_FIBER_TYPE_NODE

#define OTDR_REF_THR_NODE             "reflectance-threshold"
#define OTDR_REF_THR_XPATH            OTDR_XPATH_FIBER_PROFILE "/" OTDR_REF_THR_NODE
#define OTDR_SPLICE_THR_NODE          "splice-loss-threshold"
#define OTDR_SPLICE_THR_XPATH         OTDR_XPATH_FIBER_PROFILE "/" OTDR_SPLICE_THR_NODE
#define OTDR_FIBER_END_THR_NODE       "fiber-end-threshold"
#define OTDR_FIBER_END_THR_XPATH      OTDR_XPATH_FIBER_PROFILE "/" OTDR_FIBER_END_THR_NODE
#define OTDR_REFRACTIVE_INDEX_NODE    "refractive-index"
#define OTDR_REFRACTIVE_INDEX_XPATH   OTDR_XPATH_FIBER_PROFILE "/" OTDR_REFRACTIVE_INDEX_NODE
#define OTDR_BACKSCATTER_INDEX_NODE   "backscatter-index"
#define OTDR_BACKSCATTER_INDEX_XPATH  OTDR_XPATH_FIBER_PROFILE "/" OTDR_BACKSCATTER_INDEX_NODE

/*OTDR port level info */
/* config */
#define OTDR_XPATH_OTDRS               OTDR_XPATH_ROOT "/otdrs"
#define OTDR_XPATH_HEADER              OTDR_XPATH_OTDRS "/otdr[name='%s']"
#define OTDR_XPATH_ALL                 OTDR_XPATH_OTDRS "//*"
#define OTDR_XPATH_NAME_ALL            OTDR_XPATH_OTDRS "/otdr/name"

#define OTDR_CFG_HEADER                OTDR_XPATH_HEADER "/config"
#define OTDR_CFG_HEADER_ALL            OTDR_CFG_HEADER "//*"
#define OTDR_PORT_MEASURING_TIME_XPATH OTDR_CFG_HEADER "/" OTDR_MEASURING_TIME_NODE
#define OTDR_PORT_DIST_RANGE_XPATH     OTDR_CFG_HEADER "/" OTDR_DIST_RANGE_NODE
#define OTDR_PORT_PULSE_WIDTH_XPATH    OTDR_CFG_HEADER "/" OTDR_PULSE_WIDTH_NODE
#define OTDR_PORT_SAMP_RESOL_XPATH     OTDR_CFG_HEADER "/" OTDR_SAMP_RESOL_NODE
#define OTDR_PORT_REF_THR_XPATH        OTDR_CFG_HEADER "/" OTDR_REF_THR_NODE
#define OTDR_PORT_SPLICE_THR_XPATH     OTDR_CFG_HEADER "/" OTDR_SPLICE_THR_NODE
#define OTDR_PORT_FIBER_END_THR_XPATH  OTDR_CFG_HEADER "/" OTDR_FIBER_END_THR_NODE
#define OTDR_PORT_REFRACTIVE_INDEX_XPATH   OTDR_CFG_HEADER "/" OTDR_REFRACTIVE_INDEX_NODE
#define OTDR_PORT_BACKSCATTER_INDEX_XPATH  OTDR_CFG_HEADER "/" OTDR_BACKSCATTER_INDEX_NODE

/* state*/
#define OTDR_STATE_HEADER_ALL               OTDR_XPATH_HEADER "/state//*"
#define OTDR_STATE_HEADER                   OTDR_XPATH_HEADER "/state"
#define OTDR_MODE_NODE                      "status"
#define OTDR_MODE_XPATH                     OTDR_STATE_HEADER "/" OTDR_MODE_NODE

/* result */
#define OTDR_RESULT_HEADER                  OTDR_XPATH_HEADER "/scan-results"
#define OTDR_RESULTS_ALL                    OTDR_RESULTS_HEADER "//*"
/* trace */
#define OTDR_PORT_COMPLETION_TIME_NODE      "completion-time"
#define OTDR_PORT_COMPLETION_TIME_ALL       OTDR_RESULT_HEADER "/traces/" OTDR_PORT_COMPLETION_TIME_NODE
#define OTDR_TRACE_HEADER                   OTDR_RESULT_HEADER "/traces[completion-time='%s']"
#define OTDR_TRACE_SUBTREE_XPATH            OTDR_TRACE_HEADER "//*"
#define OTDR_PORT_COMPLETION_TIME_XPATH     OTDR_TRACE_HEADER "/" OTDR_PORT_COMPLETION_TIME_NODE
#define OTDR_PORT_EVENT_CNT_NODE            "event-count"
#define OTDR_PORT_EVENT_CNT_XPATH           OTDR_TRACE_HEADER "/" OTDR_PORT_EVENT_CNT_NODE
#define OTDR_PORT_FIBER_LEN_NODE            "total-length-m"
#define OTDR_PORT_FIBER_LEN_XPATH           OTDR_TRACE_HEADER "/" OTDR_PORT_FIBER_LEN_NODE
#define OTDR_PORT_LOSS_NODE                 "total-loss-db"
#define OTDR_PORT_LOSS_XPATH                OTDR_TRACE_HEADER "/" OTDR_PORT_LOSS_NODE
#define OTDR_PORT_REF_NODE                  "optical-return-loss-db"
#define OTDR_PORT_RETURN_LOSS_XPATH         OTDR_TRACE_HEADER "/" OTDR_PORT_REF_NODE
#define OTDR_PORT_AVG_LOSS_NODE             "average-loss-db-km"
#define OTDR_PORT_AVG_LOSS_XPATH            OTDR_TRACE_HEADER "/" OTDR_PORT_AVG_LOSS_NODE
#define OTDR_PORT_FIBER_TYPE_NODE           "discovered-fiber-type"
#define OTDR_PORT_FIBER_TYPE_XPATH          OTDR_TRACE_HEADER "/" OTDR_PORT_AVG_LOSS_NODE
#define OTDR_PORT_TRIGGER_NODE              "trigger"
#define OTDR_PORT_TRIGGER_XPATH             OTDR_TRACE_HEADER "/" OTDR_PORT_TRIGGER_NODE
#define OTDR_PORT_SCAN_TYPE_NODE            "scan-type"
#define OTDR_PORT_SCAN_TYPE_XPATH           OTDR_TRACE_HEADER "/" OTDR_PORT_SCAN_TYPE_NODE
#define OTDR_PORT_SOR_FILE_NODE             "sor-file"
#define OTDR_PORT_SOR_FILE_XPATH            OTDR_TRACE_HEADER "/" OTDR_PORT_SOR_FILE_NODE
#define OTDR_PORT_BASELINE_NODE            "baseline"
#define OTDR_PORT_BASELINE_XPATH           OTDR_TRACE_HEADER "/" OTDR_PORT_BASELINE_NODE

/* event*/
#define OTDR_PORT_EVENT_HEADER              OTDR_TRACE_HEADER "/events[event-id='%d']"
#define OTDR_EVENT_ID_NODE                  "event-id"
#define OTDR_EVENT_ID_XPATH                 OTDR_PORT_EVENT_HEADER "/" OTDR_EVENT_ID_NODE
#define OTDR_EVENT_TYPE_NODE                "event-type"
#define OTDR_EVENT_TYPE_XPATH               OTDR_PORT_EVENT_HEADER "/" OTDR_EVENT_TYPE_NODE
#define OTDR_EVENT_DISTANCE_NODE            "distance-m"
#define OTDR_EVENT_DISTANCE_XPATH           OTDR_PORT_EVENT_HEADER "/" OTDR_EVENT_DISTANCE_NODE
#define OTDR_EVENT_LOSS_NODE                "loss-db"
#define OTDR_EVENT_LOSS_XPATH               OTDR_PORT_EVENT_HEADER "/" OTDR_EVENT_LOSS_NODE
#define OTDR_EVENT_REFLECTION_NODE          "reflection-db"
#define OTDR_EVENT_REFLECTION_XPATH         OTDR_PORT_EVENT_HEADER "/" OTDR_EVENT_REFLECTION_NODE

/*rpc */
#define OTDR_RPC_SCAN_XPATH                 "/" MODULE_NS_OTDR ":otdr-scan"
#define OTDR_RPC_SCAN_NAME_XPATH            OTDR_RPC_SCAN_XPATH "/name"
#define OTDR_RPC_SCAN_OPERATION_XPATH       OTDR_RPC_SCAN_XPATH "/operation"
#define OTDR_RPC_SCAN_TYPE_XPATH            OTDR_RPC_SCAN_XPATH "/otdr-scan-type"

#define OTDR_RPC_BASELINE_XPATH             "/" MODULE_NS_OTDR ":set-baseline"
#define OTDR_RPC_BASELINE_NAME_XPATH        OTDR_RPC_BASELINE_XPATH "/name"
#define OTDR_RPC_BASELINE_TIME_XPATH        OTDR_RPC_BASELINE_XPATH "/scan-time"

/*
   "/home/oplink/datastore1/otdr-sors/NODE_Port_ScanType_Time.sor"
 */
#define OTDR_SOR_INNER_PATH           "/home/oplink/datastore1/otdr-sors/"

#define OTDR_SOR_INNER_FILE    OTDR_SOR_INNER_PATH "%s_%s_%s_%s.sor"

/* for com-oplink-optical-i */
#define COO_I_XPATH_OTDR_MODULE_ALL         COO_I_XPATH_PREFIX "/optical-modules/optical-time-domain-reflectometer/otdrs/otdr[name='%s']//*"
#define COO_I_XPATH_OTDR_NAME_COMPONENT_ALL COO_I_XPATH_PREFIX "/optical-modules/optical-time-domain-reflectometer/otdrs/otdr[component-name='%s']/name"

typedef struct
{
    char acBaseLineFile[OTDR_PORT_CNT][256];
} ST_OTDR_BASE_LINE_FILE;

OTDR_PORT_EN DSC_GetOtdrPortNameEn(char *pcPortName);
char *DSC_GetOtdrPortNameStr(char *pcStr, OTDR_PORT_EN enVal);
OTDR_MODE_EN DSC_GetOtdrModeEn(char *pcMode);
char *DSC_GetOtdrModeStr(char *pcStr, OTDR_MODE_EN enVal);
OTDR_EVENT_RET_EN DSC_GetOtdrEventTypeEn(char *pcEventType);
char *DSC_GetOtdrEventTypeStr(char *pcStr, OTDR_EVENT_RET_EN enVal);
OTDR_SCAN_TYPE_EN DSC_GetOtdrScanTypeEn(char *pcScanType);
char *DSC_GetOtdrScanTypeStr(char *pcStr, OTDR_SCAN_TYPE_EN enVal);

int DSC_OtdrGetCfgKeyFromDb(OTDR_KEY_ST *pstOtdrKey);
int DSC_OtdrGetPortKey(char *pcXpath, char *pcKeyValue);
int DSC_OtdrGetCfg(sr_session_ctx_t *pstSess, char *pcKey, OTDR_CFG_ST *pstCfg);
int DSC_OtdrSetMTime(sr_session_ctx_t *pstSess, char *pcKey, uint32_t ulMTime, bool isCommit);
int DSC_OtdrSetPara(sr_session_ctx_t *pstSess, char *pcKey, OTDR_PARA_ST *pstPara, bool isCommit);
int DSC_OtdrSetThr(sr_session_ctx_t *pstSess, char *pcKey, OTDR_THR_ST *pstThr, bool isCommit);

int DSC_OtdrGetCommonCfg(sr_session_ctx_t *pstSess, char *pcScanType, OTDR_CFG_ST *pstCfg);

int DSC_OtdrGetTrace(char *pcPortName, char *pcTime, OTDR_TRACE_ST *pstTrace);
int DSC_OtdrGetTraceI(char *pcPortName, char *pcTime, OTDR_TRACE_ST *pstTrace);
int DSC_OtdrGetTraceAll(char *pcPortName, char (*time)[OPLK_NAME_MAX_LEN], uint32_t *cnt);
int DSC_OtdrGetTraceAllI(char *pcPortName, char (*time)[OPLK_NAME_MAX_LEN], uint32_t *cnt);
int DSC_OtdrGetPortState(char *pcPortName, OTDR_STATE_ST *pstState);
int DSC_OtdrSetPortModeI(char *pcPortName, OTDR_MODE_EN enPortState);
int DSC_OtdrSetBaselineI(char *pcPortName, char *completionTime);
int DSC_OtdrSetTraceI(char *pcPortNumber, char *pcTime, OTDR_TRACE_ST *pstState);
int DSC_OtdrDeleteTraceI(char *pcPortNumber, char *pcTime);
int DSC_OtdrStartScan(char *pcPortName, char *pcScanType, char *pcOperation);
int DSC_OtdrSetTraceBaseline(char *pcPortName, char *pcCompleteionTime);

int DSC_OtdrGetPortDesc(const char *pcPortName, OPTICAL_MODULE_DESC_ST *pstDesc);
int DSC_OtdrGetPortCnt(const char *pcComponentKey);

#ifdef __cplusplus
}
#endif

#endif

