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

#ifndef _DEV_MCS_DATA__
#define _DEV_MCS_DATA__

#ifdef __cplusplus
extern "C" {
#endif

/* MCS */
#define MCS_PORT_MAX_NUM                     16
#define MCS_PD_MAX_NUM                       16

struct CMcsCfgData
{
    uint8_t aucPortSwPos[MCS_PORT_MAX_NUM];
    uint8_t aucPortStatus[MCS_PORT_MAX_NUM];
};

struct CMcsStatusData
{
    uint8_t aucAlm[8];
    uint8_t aucPortPos[MCS_PORT_MAX_NUM];
    int16_t asPdPower[MCS_PD_MAX_NUM];
};


#ifdef __cplusplus
};
#endif

#endif



