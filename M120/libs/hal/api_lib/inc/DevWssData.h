/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_WSS_DATA__
#define _DEV_WSS_DATA__

#include "DevData.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WSS */
#define WSS_CH_CNT_BUSINESS                 (387)
#define WSS_CH_CNT_OSC                                          (1)
#define WSS_CH_CNT_TOTAL                                        (WSS_CH_CNT_BUSINESS + WSS_CH_CNT_OSC) /* add one osc channel */


#define WSS_CH_IDX_MAX                      (WSS_CH_CNT_TOTAL - 1)

#define WSS_OSC_CH_IDX_BEGIN                    (0)
#define WSS_OSC_CH_IDX_END                      (0)

#define WSS_BUSINESS_CH_IDX_BEGIN           (1)
#define WSS_BUSINESS_CH_IDX_END             (WSS_CH_IDX_MAX)


#define WSS_PORT_MAX_NUM                    (32)
#define WSS_LOOPBACK_PORT                   (1)


enum EN_WSS_DIR
{
    EN_WSS_DIR_DROP = 1,
    EN_WSS_DIR_ADD = 2,

    EN_WSS_DIR_CNT
};


typedef struct
{
    uint32_t uiChId;          /* [1, max] */
    uint32_t uiCenterFreq;   /* [1, max-1] */
    uint16_t usSliceCount;    /* [2, max] */
} __attribute__((packed)) CWavePlan;

#if 0
typedef struct
{
    uint32_t uiChId;          /* [1, max] */
    uint16_t usStartSlice;   /* [1, max-1] */
    uint16_t usEndSlice;     /* [2, max] */
} __attribute__((packed)) CWavePlanSlice;
#endif


typedef struct
{
    uint32_t uiId;           /* [1, max] ch or slice */
    uint32_t uiComPort;      /* [1, max] */
    uint32_t uiExpPort;      /* [1, max] */
    uint32_t uiAtten;        /* 0.1dB */
} CWssCfg;


typedef struct
{
    CWavePlan astWavePlan[WSS_CH_CNT_TOTAL];
    CWssCfg astWssChCfgAdd[WSS_CH_CNT_TOTAL];
    CWssCfg astWssChCfgDrop[WSS_CH_CNT_TOTAL];

} CWssAllCfg;


#define WSS_PORT_CNT  66
#define WSS_PANEL_PORT_CNT 64
#define WSS_PD_HISTORY_CNT 60000 / 100 /* Total: 1Min, Interval: 100ms */

typedef struct
{
    int16_t asPortPower[WSS_PORT_CNT][WSS_PD_HISTORY_CNT];

} CWssPortPowerHistory;

typedef struct
{
    int16_t asPortPower[WSS_PORT_CNT];
    int16_t asPortPowerOut[WSS_PORT_CNT];

} CWssCurData;


typedef struct
{
    bool abPortPower[PORT_ALARM_TYPE_CNT][WSS_PORT_CNT];
    uint32_t auiDuration[PORT_ALARM_TYPE_CNT][WSS_PORT_CNT];

} CWssAlarm;

typedef struct
{
    CWssCurData stCurData;
    CWssAlarm stAlarm;

} CWssStatusData;

typedef struct
{
    int16_t asPortPowerThrLos[WSS_PORT_CNT];
    int16_t asPortPowerHysLos[WSS_PORT_CNT];

    int16_t asPortPowerThrLow[WSS_PORT_CNT];
    int16_t asPortPowerHysLow[WSS_PORT_CNT];

    int16_t asPortPowerThrHigh[WSS_PORT_CNT];
    int16_t asPortPowerHysHigh[WSS_PORT_CNT];

}CPortPowerCfg;



#ifdef __cplusplus
};
#endif

#endif



