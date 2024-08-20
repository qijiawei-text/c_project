/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOscData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_OSC_DATA__
#define _DEV_OSC_DATA__

#ifdef __cplusplus
extern "C" {
#endif


/* OSC */
enum
{
    OSC_IDX_0,
    OSC_IDX_1,

    OSC_CNT
};

enum
{
    OSC_RX,
    OSC_TX,
};
typedef struct
{
    uint8_t ucLaserEnable;
    uint8_t ucSwitch;

    int16_t sRxThr;
    int16_t sRxHys;
    int16_t sTxThr;
    int16_t sTxHys;
    int16_t sAddThr;
    int16_t sAddHys;
    int16_t sDropThr;
    int16_t sDropHys;
} COscCfgData;

typedef struct
{
    uint8_t ucModStatus;
    int16_t sRxPower;
    int16_t sTxPower;
    int16_t sAddPower;
    int16_t sDropPower;

    bool bFailure;
    bool bInnerErr;
    bool bOffLine;
    bool bRxLos;
    bool bTxLos;
    bool bAddLos;
    bool bDropLos;
    bool bLinkDown;

    /* fnc add */
    bool bMisMatch;
    bool bRxLow;
    bool bTxLow;
    bool bAddLow;
    bool bDropLow;
    bool bRxHigh;
    bool bTxHigh;
    bool bAddHigh;
    bool bDropHigh;

    /* hfc add */
    bool bLaserSafetyRxLos;
    bool bLaserSafetyTxLos;
} COscStatusData;

#ifdef __cplusplus
};
#endif

#endif



