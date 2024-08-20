/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevExtendData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_EXTEND_DATA__
#define _DEV_EXTEND_DATA__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int16_t asSwitchPos[4];

    int16_t asVoaAttn[4];
} CExtendCfgData;

typedef struct
{
    int16_t asPD[16];

    int16_t asSwitchPosActual[4];

    int16_t asVoaAttnActual[4];

    uint8_t aucDbgBuf[1024];
} CExtendStatusData;

typedef struct
{
    int16_t sTxPower;
    int16_t sMuxInPower;
    int16_t sDemuxInPower;
} CMirrorData;

#ifdef __cplusplus
};
#endif

#endif



