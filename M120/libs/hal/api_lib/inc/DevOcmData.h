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

#ifndef _DEV_OCM_DATA__
#define _DEV_OCM_DATA__

#ifdef __cplusplus
extern "C" {
#endif

/* OCM */
#define OCM_CH_MAX_NUM                       194   /* 96  */
#ifdef __HCF__
#define OCM_SLICE_MAX_NUM                    1629  /* 800 */
#else
#define OCM_SLICE_MAX_NUM                    1621  /* 800 */
#endif
#define OCM_PORT_MAX_NUM                     24

typedef struct
{
    uint8_t ucChId;          /* [1, max] */
    uint16_t usStartSlice;   /* [1, max-1] */
    uint16_t usEndSlice;     /* [2, max] */
} __attribute__((packed)) CWavePlanSlice;

typedef struct tagChannelGrid
{
    /* Resolution is 0.001 GHz */
    uint32_t uCentralFreq;
    /* Resolution is 0.001 GHz */
    uint32_t uBandwidth;
}STChannelGrid;

typedef struct
{
    CWavePlan aastWavePlan[OCM_PORT_MAX_NUM][OCM_CH_MAX_NUM];

    uint32_t auiSchdlList[OCM_PORT_MAX_NUM];
} COcmCfgData;

typedef struct
{
    /* uint32_t uiPortId; */
    int16_t asPower[OCM_CH_MAX_NUM];   /* 0.01dBm */
    int16_t asOsnr [OCM_CH_MAX_NUM];      /* 0.01dBm */
} COcmChPower;

typedef struct
{
    /* uint32_t uiPortId; */
    int16_t asOsa[OCM_SLICE_MAX_NUM];   /* 0.01dBm */
} COcmOsa;


typedef struct
{
    uint8_t aucAlm[8];
    uint8_t aucEvent[8];

    int32_t iCvPower;        /* 0.01dBm */
} COcmStatusData;

struct COcmMode
{
    uint32_t uiPortId;
    uint32_t uiMode;
};

/*
   #PortName,Rev,K3_L,K2_L,K1_L,C_L,Rev,K3_N,K2_N,K1_N,C_N,Rev,K3_H,K2_H,K1_H,C_H
   OcmToBaIn,0,0,0.00001,0.0002,10.1,0,0,0.00001,0.0002,10.1,0,0,0.00001,0.0002,10.1
   OcmToBaOut,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1
   OcmToPaIn,0,0,0.00001,0.0002,18.1,0,0,0.00001,0.0002,18.1,0,0,0.00001,0.0002,18.1
   OcmToPaOut,0,0,0.00001,0.0002,17.1,0,0,0.00001,0.0002,17.1,0,0,0.00001,0.0002,17.1
   OcmToExternal,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1
 */
typedef enum
{
    OcmToPaIn,
    OcmToBaOut,
    OcmToBaIn,
    OcmToPaOut,
    OcmToExternal,
    EN_OCM_PORT_CNT
} EN_OCM_PORT;

typedef struct
{
    double dRevL;
    double dK3L;
    double dK2L;
    double dK1L;
    double dCL;
    double dRevN;
    double dK3N;
    double dK2N;
    double dK1N;
    double dCN;
    double dRevH;
    double dK3H;
    double dK2H;
    double dK1H;
    double dCH;
} COcmPortCali;

#define   OTDR_OCM_USE_CRC  1
typedef struct
{
    /* double aOcmCali[EN_OCM_PORT_CNT][15]; */
    COcmPortCali stOcmPortCali[EN_OCM_PORT_CNT];
#if OTDR_OCM_USE_CRC
    uint8_t aucRev[28];
    uint32_t ulCrc32;
#endif
} COcmCali;

#ifdef __cplusplus
};
#endif

#endif



