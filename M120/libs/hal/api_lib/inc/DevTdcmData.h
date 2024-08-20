/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevTdcmData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_TDCM_DATA__
#define _DEV_TDCM_DATA__

#ifdef __cplusplus
extern "C" {
#endif


/* TDCM */
typedef struct
{
    float fFrequency;
    float fDispersion;

} CTdcmStatusData;

typedef struct
{

} CTdcmCfgData;


#ifdef __cplusplus
};
#endif

#endif



