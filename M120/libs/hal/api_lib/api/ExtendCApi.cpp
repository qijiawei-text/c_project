/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "DevApiManager.h"
#include "HalApi.h"


#include "DevExtendCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif



int
EXTEND_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().GetMfg(uiDevIdx, *pstMfg);
}

int
EXTEND_GetStatusData(uint32_t uiDevIdx, CExtendStatusData *pstStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().GetStatusData(uiDevIdx, *pstStatus);
}

int
EXTEND_Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().Reset(uiDevIdx, ucResetType);
}

int
EXTEND_Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().Upgrade(uiDevIdx, c_pcFilePath);
}

int
EXTEND_GetUpgadeStatus(uint32_t uiDevIdx)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().GetUpgadeStatus(uiDevIdx);
}

int
EXTEND_DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().DbgRead(uiDevIdx, pucBuf, uiLen);
}

int
EXTEND_DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().DbgWrite(uiDevIdx, pucBuf, uiLen);
}



int
EXTEND_SetSfp(uint32_t uiDevIdx, uint32_t ulVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().SetSfp(0, ulVal, uiDevIdx);
}


int
MIRROR_GetStatusData(uint32_t uiDevIdx, CMirrorData *pstStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    int rc = OPLK_OK;

    CExtendStatusData stStatus;

    memset(&stStatus, 0, sizeof(stStatus));
    rc = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(uiDevIdx, stStatus);
    if (OPLK_OK == rc) {
        pstStatus->sTxPower = stStatus.asPD[0];
        pstStatus->sMuxInPower = stStatus.asPD[2];
        pstStatus->sDemuxInPower = stStatus.asPD[1];
    }

    return rc;
}

int
SWITCH_Pos(uint32_t uiDevIdx, uint32_t uiPort)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().SetSwitchPos(0, uiPort, uiDevIdx);
}

#ifdef __cplusplus
};
#endif






