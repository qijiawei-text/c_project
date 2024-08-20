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

#include "Cmd.h"
#include "Delay.h"
#include "DevOpsCApi.h"
#include "DevSlotCApi.h"

#ifdef __cplusplus
extern "C" {
#endif
/* for olsp uiDevType : DEV_TYPE_OPS
   for ops4 uiDevType : DEV_TYPE_OPS/DEV_TYPE_OPS4A/DEV_TYPE_OPS4B
 */
int
OPS_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetMfg(uiDevIdx, *pstMfg);
}

int
OPS_GetCommStatusData(uint32_t uiDevType, COpsCommStatusData *pstOpsStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetCommStatusData(uiDevIdx, *pstOpsStatus);
}

int
OPS_GetCfgData(uint32_t uiDevType, COpsCfgData *pstOpsCfg, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetCfgData(uiDevIdx, *pstOpsCfg, uiPortId);
}

int
OPS_GetStatusData(uint32_t uiDevType, COpsStatusData *pstOpsStatus, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetStatusData(uiDevIdx, *pstOpsStatus, uiPortId);
}

int
OPS_GetHistoryData(uint32_t uiDevType, COpsHistoryData *pstData, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOpsApi().GetHistoryData(uiDevIdx, *pstData, uiPortId);
}

/*
   0 - off
   1 - red
   2 - green
   3 - shine
   4 - horse race lamp
 */

int
OPS_GetLedStatus(uint32_t uiDevType, COpsLed *pstData)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOpsApi().GetLedStatus(uiDevIdx, *pstData);
}

int
OPS_ClearAlarmDuration(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOpsApi().ClearAlarmDuration(uiDevIdx);
}


int
OPS_SetThrPwrDiff(uint32_t uiDevType, int16_t sThr, int16_t sHys, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    return OPS_SetPortPowerThr(uiDevType, OPS_PORT_LINE1_IN, OPS_THR_TYPE_DIFFER, sThr, uiPortId);
}

int
OPS_SetThrLine(uint32_t uiDevType, uint32_t uiPortIdx, int16_t sThr, int16_t sHys, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    int rc = 0;

    /* rc = CDevApiManager::GetInstance().GetOpsApi().SetThr(uiDevIdx, uiPortIdx, sThr); */
    if (rc != OPLK_OK) {
        return rc;
    }

    /* return CDevApiManager::GetInstance().GetOpsApi().SetHys(uiDevIdx, uiPortIdx, sHys); */

    return rc;
}





int
OPS_SetPortPowerThr(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOpsApi().SetPortPowerThr(uiDevIdx, uiPortIdx, uiType, sThr, uiPortId);
}


int
OPS_SetPortPowerHys(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sHys, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOpsApi().SetPortPowerHys(uiDevIdx, uiPortIdx, uiType, sHys, uiPortId);
}



int
OPS_SetWtr(uint32_t uiDevType, uint32_t uiWtr, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetWtr(uiDevIdx, uiWtr, uiPortId);
}
int
OPS_SetHoldOffTime(uint32_t uiDevType, uint32_t uiHoldOffTime, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetHoldOffTime(uiDevIdx, uiHoldOffTime, uiPortId);
}
int
OPS_SetRevertive(uint32_t uiDevType, bool bRevertive, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetRevertive(uiDevIdx, bRevertive ? 1 : 0, uiPortId);
}
int
OPS_SetMode(uint32_t uiDevType, uint32_t uiMode, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetMode(uiDevIdx, uiMode, uiPortId);
}

int
OPS_SetRelativeThr(uint32_t uiDevType, int16_t sThr, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetRelativeThr(uiDevIdx, sThr, uiPortId);
}

int
OPS_SetRelativeOffset(uint32_t uiDevType, int16_t sOffset, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetRelativeOffset(uiDevIdx, sOffset, uiPortId);
}

int
OPS_SetSwitchThr(uint32_t uiDevType, uint32_t uiPos, int32_t iThr, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetSwitchThr(uiDevIdx, uiPos, iThr, uiPortId);
}


int
OPS_SetSwitchHys(uint32_t uiDevType, uint32_t uiPos, int32_t iHys, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetSwitchHys(uiDevIdx, uiPos, iHys, uiPortId);
}


int
OPS_Reset(uint32_t uiDevType, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().Reset(uiDevIdx, ucResetType);
}


int
OPS_Upgrade(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().Upgrade(uiDevIdx, c_pcFilePath);
}

int
OPS_SetLedDebugMode(uint32_t uiDevType, uint32_t uiMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetLedMode(uiDevIdx, uiMode);
}

int
OPS_SetLedStatus(uint32_t uiDevType, uint32_t uiLedIdx, uint32_t uiStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetLedStatus(uiDevIdx, uiLedIdx, uiStatus);
}

int
OPS_GetUpgadeStatus(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetUpgadeStatus(uiDevIdx);
}


#if 0
int OPS_SetWssPortPowerThr(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr);
int OPS_SetWssPortPowerHys(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sHys);
#endif

int
OPS_SetRemoteStatus(uint32_t uiDevType, COpsReqStatus *pstStatus, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetRemoteStatus(uiDevIdx, *pstStatus, uiPortId);
}

int
OPS_GetRequestStatus(uint32_t uiDevType, COpsBidirectionStatus *pstStatus, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetRequestStatus(uiDevIdx, *pstStatus, uiPortId);
}

int
OPS_SetSwitchingType(uint32_t uiDevType, uint8_t ucType, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetSwitchingType(uiDevIdx, ucType, uiPortId);
}

int
OPS_GetOpsDevIdByKey(char *pcName, uint32_t *piDevId, uint32_t *piPortId)
{
    int cmp = 0, num = 0;
    const char *pcBaseName = "OLP";

    if (NULL == pcName || NULL == piDevId || NULL == piPortId) {
        goto END_LABEL;
    }

    cmp = strcmp(pcBaseName, pcName);
    if (0 == cmp) {
        *piDevId = DEV_TYPE_OPS;
        *piPortId = OPS_SUB_INDEX_DEFAULT;
    } else if (cmp < 0) {
        num = atoi(pcName + strlen(pcBaseName)) - 1;
        if (num < 0) {
            goto END_LABEL;
        }
        if (num < OPS1_OPS_CNT_MAX) {
            *piDevId = DEV_TYPE_OPS + num;
            *piPortId = OPS_SUB_INDEX_DEFAULT;
        } else {
            *piDevId = DEV_TYPE_OPS + OPS1_OPS_CNT_MAX + (num - OPS1_OPS_CNT_MAX) / OPS4_OPS_CNT_MAX;
            *piPortId = OPS_SUB_INDEX_DEFAULT + (num - OPS1_OPS_CNT_MAX) % OPS4_OPS_CNT_MAX;
        }
    } else {
        goto END_LABEL;
    }

    return OPLK_OK;

END_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "OPS_GetOpsDevIdByKey not found, pcKey = %s", pcName);
    *piDevId = 0;
    *piPortId = 0;
    return ERR_INVAL_ARG;
}

int
OPS_SetConnectionPdThr(uint32_t uiDevType, uint32_t uiPdIdx, uint32_t uiType, int16_t sThr, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetConnectionPdThr(uiDevIdx, uiPdIdx, uiType, sThr, uiPortId);
}

int
OPS_SetConnectionPdHys(uint32_t uiDevType, uint32_t uiPdIdx, uint32_t uiType, int16_t sHys, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetConnectionPdHys(uiDevIdx, uiPdIdx, uiType, sHys, uiPortId);
}

int
OPS_SetAlmMask(uint32_t uiDevType, int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().SetAlmMask(uiDevIdx, iMaskIdx, iMaskVal, uiPortId);
}

int
OPS_GetEventLog(uint32_t uiDevType, COpsEventLogAllData *pstEventLogAll, uint32_t uiPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOpsApi().GetEventLog(uiDevIdx, *pstEventLogAll, uiPortId);
}

#ifdef __cplusplus
};
#endif

