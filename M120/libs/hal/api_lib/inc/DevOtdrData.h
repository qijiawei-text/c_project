/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOtdrData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_OTDR_DATA__
#define _DEV_OTDR_DATA__

#ifdef __cplusplus
extern "C" {
#endif

#define OTDR_SOR_FILE           "/tmp/OTDR.sor"
#define OTDR_P_SOR_FILE         "/tmp/OTDR_P.sor"

typedef struct {
    uint32_t ulScanCtrl;
    uint32_t ulScanMode;
    uint32_t ulExpScanTime;
    uint32_t ulExpPulseWidth;           /* ns */
    uint32_t ulScanStartPos;            /* cm */
    uint32_t ulScanEndPos;              /* cm */

    float fReflIdx;
    float fReflEventThr;          /* dB */
    float fNonReflEventThr;               /* dB */
    float fFiberEndThr;
    float fBackCoef;             /* dB */
    float fOrlThr;                       /* dB */
    float fInputPwrThr;            /* dB */
    float fReflEventFhThr;                      /* dB */
    float fRamanSetpont;         /* dB */
} COtdrCfgData;

#if 0

typedef struct
{
    uint32_t ulDistRange;
    uint32_t ulPulseWidth;
    double dSamplingResolution;

    double dUserIOR;
    uint32_t ulMeasuringTime;

    double dRefThr;
    double dSpliceThr;
    double dFiberEndThr;

} COtdrCfgData;
#endif

typedef struct
{
    uint32_t ulCnt;
    double dFiberLen;
    double dTotalLoss;
    double dReturnLoss;
} COtdrEventTop;

typedef enum
{
    OTDR_EVENT_TYPE_NON_REF = 0,
    OTDR_EVENT_TYPE_REF,
    OTDR_EVENT_TYPE_FIBER_END,

    OTDR_EVENT_TYPE_CNT
} EN_OTDR_EVENT_TYPE;

typedef enum
{
    OTDR_SCAN_STATUS_IDLE = 0,
    OTDR_SCAN_STATUS_MEASURING,
    OTDR_SCAN_STATUS_ERR,

    OTDR_SCAN_STATUS_CNT
} EN_OTDR_SCAN_STATUS;

#define OTDR_EVENT_CNT   (99)
typedef struct
{
    uint32_t ulEventId;
    double dLocation;
    double dEventLoss;
    double dReflectance;
    double dTotalLoss;
    EN_OTDR_EVENT_TYPE enEventType;
} COtdrEventLine;

typedef struct
{
    float fTecTemp;
    float fModuleTemp;
    float fFpgaTemp;
} COtdrTemp;

typedef struct {
    uint32_t uiPositionOffset;
    float fLossOffset;
} COtdrStartOffset;

#if 0
typedef struct {
    float fPositionOffsetLineR;
    float fRevLineR0[2];
    float fLossOffsetLineR;
    float fRevLineR1[2];
    float fPositionOffsetOtdr;
    float fRevOtdr0[2];
    float fLossOffsetOtdr;
    float fRevOtdr1[2];
    float fPositionOffsetLineT;
    float fRevLineT0[2];
    float fLossOffsetLineT;
    float fRevLineT1[2];
} COtdrStartOffsetAll;
#else
typedef struct {
    uint32_t uiPositionOffset;
    float fRev0[2];
    float fLossOffset;
    float fRev1[2];
} COtdrStartOffsetPort;
#define OLSP_OTDR_PORT_NUM  3
typedef struct
{
    COtdrStartOffsetPort stOtdrStartOffsetPort[OLSP_OTDR_PORT_NUM];
    float fRev[8];
    uint32_t ulCrc32;
} COtdrStartOffsetAll;
#endif

typedef struct
{
    uint32_t ulScanStatus;

    COtdrEventTop stEventTop;
    COtdrEventLine astEventLine[OTDR_EVENT_CNT];

    COtdrTemp stTemp;
    COtdrStartOffset stStartOffset;

    uint8_t aucDbgBuf[1024];
} COtdrStatusData;

typedef enum {
    OTDR_SCAN_MODE_AUTO = 1,
    OTDR_SCAN_MODE_EXPERT = 2,
    OTDR_SCAN_MODE_FORWARDDIRECTIONCO = 4,
    OTDR_SCAN_MODE_OPPOSITEDIRECTIONCTP = 8
} OTDR_SCAN_MODE_EN;

#ifdef __cplusplus
};
#endif

#endif



