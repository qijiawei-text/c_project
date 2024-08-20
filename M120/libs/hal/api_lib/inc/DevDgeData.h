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

#ifndef _DEV_DGE_DATA__
#define _DEV_DGE_DATA__

#ifdef __cplusplus
extern "C" {
#endif

/* DGE */
#define DGE_POINT_MAX_NUM                    776

typedef struct
{
    uint32_t uiFreq;   /* GHz */
    uint32_t uiAtten;  /* 0.01dB */
} CDgePoint;

struct CDgeCfgData
{
    CDgePoint astCurve[DGE_POINT_MAX_NUM];
};

struct CDgeStatusData
{
    uint8_t aucAlm[8];
};

#define DGE_MFG_INFO_STR_LEN    32
#define DGE_MFG_VER_LEN         4
#define DGE_MFG_HW_DATE_LEN     2
#define DGE_MFG_CALI_DATE_LEN   4

struct CDgeMfg
{
    char acSN[DGE_MFG_INFO_STR_LEN];
    char acPN[DGE_MFG_INFO_STR_LEN];
    char acModType[DGE_MFG_INFO_STR_LEN];

    uint8_t aucBootVer[DGE_MFG_VER_LEN];
    uint8_t aucMainVer[DGE_MFG_VER_LEN];
    uint8_t aucHwVer[DGE_MFG_VER_LEN];
    uint8_t aucHwDate[DGE_MFG_HW_DATE_LEN];
    uint8_t aucCaliDate[DGE_MFG_CALI_DATE_LEN];
};



#ifdef __cplusplus
};
#endif

#endif



