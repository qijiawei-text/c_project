/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCApi.cpp
   Author:        Zhu Weian
   Date:          2020-11-09
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-11-09] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "DevApiManager.h"
#include "HalApi.h"

#include "DevSlotCApi.h"
#include "DevWssCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
WSS_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0xFF, uiDevSubIdx = 0xFF;
    SLOT_GetDevIdx(uiDevType, &uiDevIdx, &uiDevSubIdx);
    return CDevApiManager::GetInstance().GetWssApi().GetMfg(uiDevIdx, *pstMfg);

}

int
WSS_GetCfg(uint32_t uiDevIdx, CWssAllCfg *pstCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().GetCfgData(uiDevIdx, *pstCfg);

}


int
WSS_GetOscCfg(uint32_t uiDevIdx, CWssAllCfg *pstCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    return CDevApiManager::GetInstance().GetWssApi().GetOscCfgData(uiDevIdx, *pstCfg);
}

int
WSS_GetStatusData(uint32_t uiDevIdx, CWssStatusData *pstStatus)
{

#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().GetWssStatusData(0, *pstStatus);

}


int
WSS_SetAllChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().SetAllChCfg(uiDevIdx, uiSubIdx, uiChCnt, pstWssCfg);
}

int
WSS_SetOscChCfg(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWssCfg *pstWssCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().SetOscChCfg(uiDevIdx, uiSubIdx, uiChCnt, pstWssCfg);
}


int
WSS_SetWaveplan(uint32_t uiDevIdx, uint32_t uiSubIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().SetWavePlan(uiDevIdx, uiSubIdx, uiChCnt, pstWavePlan);
}


int
WSS_SetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().SetWssPortPowerThr(uiDevIdx, uiPortIdx, uiType, sThr);
}


int
WSS_SetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPortIdx, uint32_t uiType, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().SetWssPortPowerHys(uiDevIdx, uiPortIdx, uiType, sHys);
}

int
WSS_GetPortPowerHistory(uint32_t uiDevIdx)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().GetWssPortPowerHistory(0);
}

int
WSS_SetLedMode(uint32_t uiDevIdx, uint32_t uiMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().SetWssLedMode(uiDevIdx, uiMode);
}


int
WSS_SetLedDebugMode(uint32_t uiDevIdx, uint32_t uiMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().SetLedMode(uiDevIdx, uiMode);
}

int
WSS_SetLedStatus(uint32_t uiDevIdx, uint32_t uiLedIdx, uint32_t uiStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetOpsApi().SetLedStatus(uiDevIdx, uiLedIdx, uiStatus);
}

int
WSS_Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().Reset(uiDevIdx, ucResetType);
}


int
WSS_Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().Upgrade(uiDevIdx, c_pcFilePath);
}

#ifdef __cplusplus
};
#endif



int
WSS_GetUpgradeStatus(uint32_t uiDevIdx)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().GetUpgadeStatus(uiDevIdx);
}


int
WSS_SetUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().SetDataForUser(uiDevIdx, pucData, nLen);
}


int
WSS_GetUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().GetDataForUser(uiDevIdx, pucData, nLen);
}

int
WSS_SetPmUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().SetDataForPmUser(uiDevIdx, pucData, nLen);
}


int
WSS_GetPmUserData(uint32_t uiDevIdx, uint8_t *pucData, size_t nLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetWssApi().GetDataForPmUser(uiDevIdx, pucData, nLen);
}

