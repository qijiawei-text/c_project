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

#include "DevSlotCApi.h"
#include "DevEdfaCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif





int
EDFA_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

#if 0
    uint32_t uiDevIdx = 0;
    if (EDFA_GetDevIdx(uiDevType, uiDevIdx) != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Unknown DevType = %d", uiDevType);
        return -1;
    }
#endif

    uint32_t uiDevIdx = 0, uiDevSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetMfg(uiDevIdx, *pstMfg);
}

int
EDFA_GetStatusData(uint32_t uiDevType, COaStatusData *pstEdfaStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, *pstEdfaStatus, uiSubIdx);
}

int
EDFA_GetCfgData(uint32_t uiDevType, COaCfgData *pstEdfaCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetCfgData(uiDevIdx, *pstEdfaCfg);
}

int
EDFA_GetDebugInfo(uint32_t uiDevType, const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetDebugInfo(uiDevIdx, pcFilePath);
}

/* All EDFA information will be written into pcFilePath */
int
EDFA_GetMcuDebugInfo(uint32_t uiDevType, const char *pcFilePath, uint32_t uiVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetMcuDebugInfo(uiDevIdx, pcFilePath, uiVal);
}

int
EDFA_SetGainRange(uint32_t uiDevType, int16_t sGainRange)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetGainRange(uiDevIdx, sGainRange, uiSubIdx);
}

int
EDFA_SetGain(uint32_t uiDevType, int16_t sGain)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetGain(uiDevIdx, sGain, uiSubIdx);
}

int
EDFA_SetTilt(uint32_t uiDevType, int16_t sTilt)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetTilt(uiDevIdx, sTilt, uiSubIdx);
}

int
EDFA_SetMode(uint32_t uiDevType, uint8_t ucMode, int16_t sVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetMode(uiDevIdx, ucMode, sVal, uiSubIdx);
}

int
EDFA_SetPumpEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetPumpEnable(uiDevIdx, bEnable, uiSubIdx);

}
int
EDFA_SetAprEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetAprEnable(uiDevIdx, bEnable, uiSubIdx);

}
int
EDFA_SetAutolosEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetAutolosEnable(uiDevIdx, bEnable, uiSubIdx);

}
int
EDFA_SetRxLosThr(uint32_t uiDevType, int16_t sThr, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetRxLosThr(uiDevIdx, sThr, sHys, uiSubIdx);
}

int
EDFA_SetAttn(uint32_t uiDevType, int16_t sAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetAttn(uiDevIdx, sAttn, uiSubIdx);


}

int
EDFA_GetAttn(uint32_t uiDevType, int16_t *psAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif


    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    COaStatusData stOaData;
    int rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaData, uiSubIdx);

    if (OPLK_OK == rc) {
        if ( 0 == uiSubIdx) {
            *psAttn = stOaData.stOaCurData.asVoaAttnActual[VOA_E1_PADDING];
        } else if ( 1 == uiSubIdx) {
            *psAttn = stOaData.stOaCurData.asVoaAttnActual[VOA_E2_PADDING];
        }
    }

    return rc;
}

int
EDFA_Reset(uint32_t uiDevType, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().Reset(uiDevIdx, ucResetType);
}

int
EDFA_Upgrade(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().Upgrade(uiDevIdx, c_pcFilePath);
}

int
EDFA_Swap(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().Swap(uiDevIdx);
}

int
EDFA_Commit(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().Commit(uiDevIdx);
}

int
EDFA_GetUpgadeStatus(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetUpgadeStatus(uiDevIdx);
}

int
EDFA_DbgRead(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().DbgRead(uiDevIdx, pucBuf, uiLen);
}

int
EDFA_DbgWrite(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().DbgWrite(uiDevIdx, pucBuf, uiLen);
}

int
EDFA_SetDfbEnable(uint32_t uiDevType, bool bEnable, int32_t iPower)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetDfbEnable(uiDevIdx, bEnable, iPower, uiSubIdx);

}

int
EDFA_SetOscEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscEnable(uiDevIdx, bEnable, uiSubIdx);

}

int
EDFA_SetOtdrSwitchPos(uint32_t uiDevType, uint32_t uiPos)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOtdrSwitchPos(uiDevIdx, uiPos, uiSubIdx);
}

int
EDFA_SetOscAddThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscAddThrHys(uiDevIdx, sThr, sHys, uiSubIdx);
}

int
EDFA_SetOscDropThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscDropThrHys(uiDevIdx, sThr, sHys, uiSubIdx);
}

int
EDFA_SetOscRxThrHys(uint32_t uiDevType, int16_t sThr, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscRxThrHys(uiDevIdx, sThr, sHys, uiSubIdx);
}

int
EDFA_SetApsdEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetApsdEnable(uiDevIdx, bEnable, uiSubIdx);

}

int
EDFA_GetApsdStatus(uint32_t uiDevType, bool *pbCfg, bool *pbStatus, bool *pbEvent)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    int iRet = -1;
    COaStatusData stOaStatusData;
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    memset(&stOaStatusData, 0x00, sizeof(stOaStatusData));
    iRet = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaStatusData, uiSubIdx);
    if (OPLK_OK == iRet) {
        *pbCfg = stOaStatusData.stOaCurData.abApsdEnable;
        *pbStatus = stOaStatusData.stOaCurData.abApsdStatus;
        *pbEvent = stOaStatusData.stOaCurData.abApsdEvent;
    }

    return iRet;
}

int
EDFA_GetOcmAttn(uint32_t uiDevType, int16_t *psAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    COaStatusData stOaData;
    int rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaData, uiSubIdx);

    if (OPLK_OK == rc) {
        *psAttn = stOaData.stOaCurData.asVoaAttnActual[VOA_OCM];
    }

    return rc;
}

int
EDFA_SetUpgState(uint32_t uiDevType, int16_t sState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetUpgState(uiDevIdx, sState, uiSubIdx);
}

int
EDFA_SetLedState(uint32_t uiDevType, int16_t sState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetLedState(uiDevIdx, sState, uiSubIdx);
}

int
EDFA_SetLedAlm(uint32_t uiDevType, int16_t sState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetLedAlm(uiDevIdx, sState, uiSubIdx);
}

int
EDFA_GetCali(uint32_t uiDevType, COaCaliData *pstCaliInfo)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOaApi().GetCaliInfo(uiDevIdx, *pstCaliInfo, uiSubIdx);
}

int
EDFA_SetOtdrCali(uint32_t uiDevType, char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "EDFA_SetOtdrCali DevType = %d,uiDevIdx:%d,uiSubIdx:%d", uiDevType, uiDevIdx, uiSubIdx);
    return CDevApiManager::GetInstance().GetOaApi().SetOtdrCali(uiDevIdx, pcFilePath, uiSubIdx);
}

int
EDFA_SetOcmCali(uint32_t uiDevType, char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "EDFA_SetOcmCali DevType = %d,uiDevIdx:%d,uiSubIdx:%d", uiDevType, uiDevIdx, uiSubIdx);
    return CDevApiManager::GetInstance().GetOaApi().SetOcmCali(uiDevIdx, pcFilePath, uiSubIdx);
}

int
EDFA_SetManualVoaEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetManualVoaEnable(uiDevIdx, bEnable, uiSubIdx);
}

int
EDFA_SetPaddingVoaConfigPower(uint32_t uiDevType, int16_t sOutPower)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetPaddingVoaConfigPower(uiDevIdx, sOutPower, uiSubIdx);
}

int
EDFA_SetManualGainEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetManualGainEnable(uiDevIdx, bEnable, uiSubIdx);
}

int
EDFA_SetThr(uint32_t uiDevType, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetThrHys(uiDevIdx, uiPortIdx, uiType, sThr, sHys, uiSubIdx);
}

int
EDFA_SetPumpCurrent(uint32_t uiDevType, uint32_t uiPumpIdx, int16_t sCurrent)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetPumpCurrent(uiDevIdx, uiPumpIdx, sCurrent, uiSubIdx);
}

int
EDFA_SetAlmMask(uint32_t uiDevType, int32_t iMaskIdx, int32_t iMaskVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetAlmMask(uiDevIdx, iMaskIdx, iMaskVal, uiSubIdx);
}

int
EDFA_GetEventLog(uint32_t uiDevType, COaEventLogAllData *pstEventLogAll)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOaApi().GetEventLog(uiDevIdx, *pstEventLogAll, uiSubIdx);
}

int
EDFA_SetShutterState(uint32_t uiDevType, bool block)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetShutterState(uiDevIdx, block, uiSubIdx);
}

int
EDFA_GetShutterState(uint32_t uiDevType, bool *block)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().GetShutterState(uiDevIdx, *block, uiSubIdx);
}

int
EDFA_SetRemoteInterlockEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetRemoteInterlockEnable(uiDevIdx, bEnable, uiSubIdx);
}

int
EDFA_SetDummyAdminKey(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetDummyAdminKey(uiDevIdx, bEnable, uiSubIdx);
}

int
EDFA_SetPanelSafeCmpt(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetPanelSafeCmpt(uiDevIdx, bEnable, uiSubIdx);
}

int
EDFA_SetReadyTrigger(uint32_t uiDevType, uint32_t uiState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetReadyTrigger(uiDevIdx, uiState, uiSubIdx);
}

int
EDFA_SetFsmMode(uint32_t uiDevType, uint32_t uiMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetAutogainMode(uiDevIdx, uiMode, uiSubIdx);
}

int
EDFA_SetReadyPower(uint32_t uiDevType, uint32_t uiPower)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetReadyPower(uiDevIdx, uiPower, uiSubIdx);
}

int
EDFA_SetTargetPower(uint32_t uiDevType, uint32_t uiPower)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetTarget(uiDevIdx, uiPower, uiSubIdx);
}

#ifdef __cplusplus
};
#endif



