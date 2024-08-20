/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevMuxData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_MUX_DATA__
#define _DEV_MUX_DATA__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{

} CMuxCfgData;


typedef struct
{
    int16_t asPdPower[2];

} CMuxStatusData;

typedef struct
{
    double dRefMuxTCali;
    double dRefDemuxRCali;

} CMuxCaliData;


#ifdef __cplusplus
};
#endif

#endif



