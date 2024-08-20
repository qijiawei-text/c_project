#ifndef _DSC_WAVEPLAN_H
#define _DSC_WAVEPLAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"



#define WSS1_CFG_KEY    "WSS1"
#define WSS2_CFG_KEY    "WSS2"


typedef enum {
    EN_WAVEPLAN_TYPE_FIX = 0,
    EN_WAVEPLAN_TYPE_FLEX,
    EN_WAVEPLAN_TYPE_MAX
} WAVEPLAN_TYPE_EN;

#define MAX_CHL                 512
typedef struct WAVEPLAN_st
{
    uint32_t uiChannelId;
    char acChannelName[32];
    double dCenterFreq;        /* GHZ */
    double dWavelength;
    double dBandwidth;         /* GHZ */
} WAVEPLAN_ST;
/* usCenterFrequency = (Center Frequency - 191 THz) / 3.125G; */
/* usBandWidth = Band Width / 3.125G; */

/* The yang-model define is THZ, here need to translate to GHz*/
#define FREQUENCY_GHZ(dFrequency) (dFrequency * 1000)

/* Maybe need to check this formula */
#define BANDWIDTH_GHZ(uiBandwidthSlice) (uiBandwidthSlice * 6.25)

/* Wavelplan management */
#define MODULE_NS_WAVEPLAN                "com-oplink-waveplan"
#define WAVEPLAN_I_PREFIX                 "/" MODULE_NS_WAVEPLAN ":com-oplink-waveplan/default-waveplan"
#define WAVEPLAN_XPATH_DEFAULT_COM        COO_XPATH_PREFIX  "/optical-modules/com-oplink-waveplan"
#define WAVEPLAN_XPATH_DEFAULT_HEADER     WAVEPLAN_XPATH_DEFAULT_COM  ":default-waveplan"
#define WAVEPLAN_XPATH_DEFAULT_ALL        WAVEPLAN_XPATH_DEFAULT_HEADER "//*"
#define WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID WAVEPLAN_XPATH_DEFAULT_HEADER "/wavelengths/wavelength-number"
#define WAVEPLAN_XPATH_HEADER             OOD_XPATH_PREFIX "/flex-waveplan"
#define WAVEPLAN_XPATH_ALL                WAVEPLAN_XPATH_HEADER "//*"
#define WAVEPLAN_XPATH                    WAVEPLAN_XPATH_HEADER "/wavelengths[wavelength-number='%d']"
#define WAVEPLAN_XPATH_CHANNEL_ID_ALL     WAVEPLAN_XPATH_HEADER "/wavelength-number"
#define CHANNEL_ID_XPATH                  "/wavelength-number"
#define WAVEPLAN_XPATH_CHANNEL_ID         WAVEPLAN_XPATH CHANNEL_ID_XPATH
#define CHANNEL_NAME_XPATH                "/wavelength-name"
#define WAVEPLAN_XPATH_CHANNEL_NAME       WAVEPLAN_XPATH CHANNEL_NAME_XPATH
#define FREQUENCY_XPATH                   "/center-frequency"
#define WAVEPLAN_XPATH_FREQUENCY          WAVEPLAN_XPATH FREQUENCY_XPATH
#define WAVELENGTH_XPATH                  "/wavelength"
#define WAVELENGTH_NODE                  "wavelength"
#define WAVEPLAN_XPATH_WAVELENGTH         WAVEPLAN_XPATH WAVELENGTH_XPATH
#define BANDWIDTH_XPATH                   "/bandwidth"
#define WAVEPLAN_XPATH_BANDWIDTH          WAVEPLAN_XPATH BANDWIDTH_XPATH

#define WAVEPLAN_XPATH_DEFAULT_ID_I         "/com-oplink-waveplan:com-oplink-waveplan/default-waveplan/wavelengths/wavelength-number"
#define WAVEPLAN_XPATH_DEFAULT_ALL_I        "/com-oplink-waveplan:com-oplink-waveplan/default-waveplan//*"
#define WAVEPLAN_XPATH_DEFAULT_CHANNEL_ID_I "/com-oplink-waveplan:com-oplink-waveplan/default-waveplan/wavelengths/wavelength-number"

/* set wavepan config agents */
/* extern int DSC_GetWaveplansCnt(size_t *count); */
/* extern int DSC_GetDefaultWaveplansCnt(size_t *count); */
/* extern int DSC_GetWaveplansInfo(WAVEPLAN_ST **waveplan, size_t *count); */
/* extern int DSC_GetDefaultWaveplansInfo(WAVEPLAN_ST **ppstWaveplan, size_t *count); */
/* extern int DSC_SetWaveplansInfo(WAVEPLAN_ST stWaveplan[], size_t size); */
extern int DSC_GetInnerWaveplansCnt(size_t *count);
/* extern int DSC_GetInnerDefaultWaveplansInfo(WAVEPLAN_ST **ppstWaveplan, size_t *count); */

extern int DSC_GetOcmWaveplanInfo(WAVEPLAN_ST **ppstWaveplan, size_t *pnCount);
extern int DSC_GetOcmWaveplanNumber(size_t *pnCount);
extern WAVEPLAN_TYPE_EN DSC_GetWaveplanType();

#define DEF_COO_WAVEPLAN_XPATH    "/com-oplink-optical:optical-control/optical-modules/com-oplink-waveplan:default-waveplan/"

#ifdef __cplusplus
}
#endif

#endif

