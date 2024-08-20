/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsCApi.cpp
   Author:        Guo Guangjun
   Date:          2024-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-04] [1.0] [Creator]



*****************************************************************************************************/

/*=========================include head files====================*/
#include "DevApiManager.h"
#include "HalApi.h"
#include "DevSlotCApi.h"


#include "DevOcsCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
OCS_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().GetMfg(uiDevIdx, *pstMfg);
}

int
OCS_GetStatusData(uint32_t uiDevType, COcsStatusData *pstOcsStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().GetStatusData(uiDevIdx, *pstOcsStatus);
}

int
OCS_GetConnections(uint32_t uiDevType, COcsConnections *pstConnections)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().GetSwitchChannels(uiDevIdx, *pstConnections);
}

int
OCS_SetConnection(uint32_t uiDevType, const COcsConnection *pstConnection)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().SetSwitchChannel(uiDevIdx, *pstConnection);
}


int
OCS_SetConnections(uint32_t uiDevType, const COcsConnections *pstConnections)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().SetSwitchChannels(uiDevIdx, *pstConnections);
}

int
OCS_SetLedMode(uint32_t uiDevType, uint32_t uiMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().SetLampTestState(uiDevIdx, uiMode);
}


int
OCS_Reset(uint32_t uiDevType, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().Reset(uiDevIdx, ucResetType);
}


int
OCS_Upgrade(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().Upgrade(uiDevIdx, c_pcFilePath);
}

#ifdef __cplusplus
};
#endif

int
OCS_Commit(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().Commit(uiDevIdx);
}

int
OCS_GetUpgradeStatus(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcsApi().GetUpgadeStatus(uiDevIdx);
}

