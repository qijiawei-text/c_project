#ifndef _DSC_TDCM_H
#define _DSC_TDCM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"



#define TDCM_DEV_NUM_MAX        1
#define TDCM_CFG_KEY           "TDCM"


typedef struct st_TDCM_INFO
{
    bool bActive;           /* RONLY */
    bool bReady;            /* RONLY */
    uint32_t u32ChlNumber;  /* RONLY */
    double dChlSpacing;     /* RONLY */
    double dDispersnActual; /* RONLY */
    double dFirstFreqncy;   /* RONLY */
    double dMaxDispersion;  /* RONLY */
    double dMinDispersion;  /* RONLY */
    double dFrequency;    /* WR    */
    double dDispersn;     /* WR    */
    double dDispsnRoughStep;  /* WR    */
    double dDispsnSmoothStep; /* WR    */

}ST_TDCM_INFO;

#define MODULE_NS_TDCM                      "com-oplink-tdcm"

#define TDCM_MAX_MON_ATTR                   16
#define TDCM_XPATH_HEADER                   COO_XPATH_PREFIX "/optical-modules/com-oplink-tdcm:optical-tdcm"
#define TDCM_XPATH_ALL                      TDCM_XPATH_HEADER "//*"


#define TDCM_XPATH_CONFIG                   TDCM_XPATH_HEADER "/tdcms[name='%s']/tdcm" "/tdcm-config"
#define TDCM_LEAF_DISPERSION                "/dispersion"
#define TDCM_XPATH_DISPERSION               TDCM_XPATH_CONFIG TDCM_LEAF_DISPERSION

#define TDCM_LEAF_FREQUENCY                 "/frequency"
#define TDCM_XPATH_FREQUENCY                TDCM_XPATH_CONFIG TDCM_LEAF_FREQUENCY

#define TDCM_LEAF_DISPERSION_ROUGH_STEP     "/disprsn-rough-step"
#define TDCM_XPATH_DISPERSION_ROUGH_STEP    TDCM_XPATH_CONFIG TDCM_LEAF_DISPERSION_ROUGH_STEP

#define TDCM_LEAF_DISPERSION_SMOOTH_STEP    "/disprsn-step"
#define TDCM_XPATH_DISPERSION_SMOOTH_STEP   TDCM_XPATH_CONFIG TDCM_LEAF_DISPERSION_SMOOTH_STEP

#define TDCM_XPATH_MONITOR                  TDCM_XPATH_HEADER "/tdcms[name='%s']/tdcm/tdcm-monitor"
#define TDCM_XPATH_MONITOR_ALL              TDCM_XPATH_MONITOR "//*"

#define TDCM_LEAF_CHANNEL_SPACING           "/channel-spacing"
#define TDCM_XPATH_CHANNEL_SPACING          TDCM_XPATH_MONITOR TDCM_LEAF_CHANNEL_SPACING

#define TDCM_LEAF_CHANNEL_NUMBER            "/channel-number"
#define TDCM_XPATH_CHANNEL_NUMBER           TDCM_XPATH_MONITOR TDCM_LEAF_CHANNEL_NUMBER

#define TDCM_LEAF_FIRST_FREQNCY             "/first-frequency"
#define TDCM_XPATH_FIRST_FREQNCY           TDCM_XPATH_MONITOR TDCM_LEAF_FIRST_FREQNCY

#define TDCM_LEAF_DISPERSION_ACTUAL        "/dispersion-actual"
#define TDCM_XPATH_DISPERSION_ACTUAL       TDCM_XPATH_MONITOR TDCM_LEAF_DISPERSION_ACTUAL

#define TDCM_LEAF_DISPERSION_MIN           "/min-dispersion"
#define TDCM_XPATH_DISPERSION_MIN          TDCM_XPATH_MONITOR TDCM_LEAF_DISPERSION_MIN

#define TDCM_LEAF_DISPERSION_MAX           "/max-dispersion"
#define TDCM_XPATH_DISPERSION_MAX          TDCM_XPATH_MONITOR TDCM_LEAF_DISPERSION_MAX

#define TDCM_LEAF_ACTIVE                   "/active"
#define TDCM_XPATH_ACTIVE                  TDCM_XPATH_MONITOR TDCM_LEAF_ACTIVE

#define TDCM_LEAF_READY                    "/ready"
#define TDCM_XPATH_READY                   TDCM_XPATH_MONITOR TDCM_LEAF_READY

#define RPC_XPATH_TDCM_SCAN                "/com-oplink-tdcm:tdcm-scan"
#define RPC_XPATH_TDCM_SCAN_STOP_DISPERSN  RPC_XPATH_TDCM_SCAN "/stop-dispersion"
#define RPC_XPATH_TDCM_SCAN_START_DISPERSN RPC_XPATH_TDCM_SCAN "/start-dispersion"
#define RPC_XPATH_TDCM_SCAN_STEP           RPC_XPATH_TDCM_SCAN "/disprsn-step"
#define RPC_XPATH_TDCM_SCAN_SPANLOSS       RPC_XPATH_TDCM_SCAN "/span-loss"


extern int DSC_GetTdcmCfgKey(char *pcXpath, char *pcKeyValue);
extern int DSC_SetTdcmFrequency(char *pcCfgKey, double dFrequency);
extern int DSC_SetTdcmDispersn(char *pcCfgKey, double dDispersn);
extern int DSC_SetTdcmDispersnStep(char *pcCfgKey, double dRoughStep, double dSmoothStep);
extern int DSC_GetTdcmDispersnStep(char *pcCfgKey, double *pdRoughStep, double *pdSmoothStep);
extern int DSC_GetTdcmInfo(char *pcCfgKey, ST_TDCM_INFO *psTdcmInfo, size_t size);
extern int DSC_GetTdcmDispersn(char *pcCfgKey, double *pdDispersn);
extern int DSC_GetTdcmFrequency(char *pcCfgKey, double *pdFrequency);
extern int DSC_TdcmScan(double dStart, double dStop, double dStep);
extern int DSC_TdcmScanBySpanloss(double dSpanloss);
extern int DSC_SetTdcmDispersn_Commit(char *pcCfgKey, double d32Dispersn, bool bCommit);
#ifdef __cplusplus
}
#endif

#endif
