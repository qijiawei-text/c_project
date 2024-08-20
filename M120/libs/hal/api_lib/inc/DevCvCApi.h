/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCVCApi.h
   Author:        Zhu WeiAn
   Date:          2022-10-10
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu WeiAn] [2022-10-10] [1.0] [Creator]


*****************************************************************************************************/
#include "DevData.h"
#include "DevCvData.h"

#ifdef __cplusplus
extern "C" {
#endif

int Cv_GetMuxStatusData(EnCvLine enCvLine, CCvMuxStatusData *pstMuxStatus);
int Cv_GetClientStatusData(EnCvLine enCvLine, CCvClientStatusData *pstClientStatus);
int Cv_GetRef(CCvRef *pstRef);

#ifdef __cplusplus
};
#endif

