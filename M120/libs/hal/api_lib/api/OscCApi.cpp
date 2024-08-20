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
#include "OaApi.h"
#include "ExtendApi.h"
#include "DevSlotCApi.h"
#include "DevOscCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif


int
OSC_GetStatusData(uint32_t uiDevType, COscStatusData *pstOscStatus)
{
    int iRet = OPLK_OK;

#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    COaStatusData stOaStatus;
    memset(&stOaStatus, 0, sizeof(stOaStatus));

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    iRet = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaStatus);
    if ( OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_DEBUG, "Get osc status data error. uiDevIdx = %d", uiDevIdx);
        return iRet;
    }
#ifdef __OLSP__
    pstOscStatus->sRxPower = stOaStatus.stOaCurData.asExtPdPower[PD15_EDFA1_OSC_RX_VOA_OUT_PWR];
    pstOscStatus->sTxPower = stOaStatus.stOaCurData.asExtPdPower[PD13_EDFA2_OSC_TX_OUT_PWR];
    pstOscStatus->sAddPower = stOaStatus.stOaCurData.asExtPdPower[PD13_EDFA2_OSC_TX_OUT_PWR];
    pstOscStatus->sDropPower = stOaStatus.stOaCurData.asExtPdPower[PD14_EDFA1_OSC_RX_VOA_IN_PWR];
#endif
#ifdef __NR1004__
    pstOscStatus->sRxPower = stOaStatus.stOaCurData.asExtPdPower[E1_VOA3_OUT];
    pstOscStatus->sTxPower = stOaStatus.stOaCurData.asExtPdPower[E2_VOA3_IN];
    pstOscStatus->sAddPower = stOaStatus.stOaCurData.asExtPdPower[E2_OSCADD];
    pstOscStatus->sDropPower = stOaStatus.stOaCurData.asExtPdPower[E1_OSCDROP];
    pstOscStatus->bMisMatch = stOaStatus.stOscCurData.bMisMatch;
    pstOscStatus->bRxLow = stOaStatus.stOscCurData.bRxLow;
    pstOscStatus->bTxLow = stOaStatus.stOscCurData.bTxLow;
    pstOscStatus->bAddLow = stOaStatus.stOscCurData.bAddLow;
    pstOscStatus->bDropLow = stOaStatus.stOscCurData.bDropLow;
    pstOscStatus->bRxHigh = stOaStatus.stOscCurData.bRxHigh;
    pstOscStatus->bTxHigh = stOaStatus.stOscCurData.bTxHigh;
    pstOscStatus->bAddHigh = stOaStatus.stOscCurData.bAddHigh;
    pstOscStatus->bDropHigh = stOaStatus.stOscCurData.bDropHigh;
#endif
#ifdef __HCF__
    pstOscStatus->sRxPower = stOaStatus.stOaCurData.asExtPdPower[OSC_RX_PWR];
    pstOscStatus->sTxPower = stOaStatus.stOaCurData.asExtPdPower[OSC_TX_PWR];
    pstOscStatus->sAddPower = stOaStatus.stOaCurData.asExtPdPower[OSC_ADD_PWR];
    pstOscStatus->sDropPower = stOaStatus.stOaCurData.asExtPdPower[OSC_DROP_PWR];
    pstOscStatus->bLaserSafetyRxLos = stOaStatus.stOscCurData.bLaserSafetyRxLos;
#endif
    pstOscStatus->bFailure = stOaStatus.stOscCurData.bFailure;
    pstOscStatus->bInnerErr = stOaStatus.stOscCurData.bInnerErr;
    pstOscStatus->bOffLine = stOaStatus.stOscCurData.bOffLine;
    pstOscStatus->bRxLos = stOaStatus.stOscCurData.bRxLos;
    pstOscStatus->bTxLos = stOaStatus.stOscCurData.bTxLos;
    pstOscStatus->bAddLos = stOaStatus.stOscCurData.bAddLos;
    pstOscStatus->bDropLos = stOaStatus.stOscCurData.bDropLos;
    pstOscStatus->bLinkDown = stOaStatus.stOscCurData.bLinkDown;
    pstOscStatus->ucModStatus = stOaStatus.stOscCurData.ucModStatus;

    return OPLK_OK;
}
int
OSC_GetCfgData(uint32_t uiDevIdx, COscCfgData *pstOscCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return 0;
}
int
OSC_SetEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscEnable(uiDevIdx, bEnable, uiSubIdx);

}

int
OSC_SetVoa(uint32_t uiDevType, uint32_t uiTxRx, int16_t sAttn)
{
    int rc = OPLK_OK;
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    /* return CDevApiManager::GetInstance().GetExtendApi().SetVoaAttn(0, sAttn, 2); */
    /* olsp&fnc osc rx voa control by edfa module, there is no external interface
           fnc osc tx voa can be setting , uiDevIdx use DEV_TYPE_BA  or DEV_TYPE_P_BA */
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
#ifdef __NR1004__
    if (OSC_TX == uiTxRx) {
        rc = CDevApiManager::GetInstance().GetOaApi().SetAttn(uiDevIdx, sAttn, VOA_OSC_TX);
    }
#endif
    return rc;
}

int
OSC_SetTarget(uint32_t uiDevType, uint32_t uiTxRx, int16_t sTarget)
{
    int rc = OPLK_OK;
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    /*fnc osc tx voa can be setting target power , uiDevIdx use DEV_TYPE_BA  or DEV_TYPE_P_BA */
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    if (OSC_TX == uiTxRx) {
        rc = CDevApiManager::GetInstance().GetOaApi().SetTarget(uiDevIdx, sTarget, VOA_IDX_6);
    }

    return rc;
}

int
OSC_SetAmpEnable(uint32_t uiDevType, bool bEnable)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOaApi().SetOscAmpEnable(uiDevIdx, bEnable, uiSubIdx);

}

int
OSC_GetVoa(uint32_t uiDevType, uint32_t uiTxRx, int16_t *psAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    COaStatusData stOaData;
    int rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaData, uiSubIdx);

    if (OPLK_OK == rc) {
        if (OSC_RX == uiTxRx) {
            *psAttn = stOaData.stOaCurData.asVoaAttnActual[VOA_OSC];
        } else if (OSC_TX == uiTxRx) {
            *psAttn = stOaData.stOaCurData.asVoaAttnActual[VOA_OSC_TX];
        }
    }
    return rc;
}

int
OSC_SetSwitchPos(uint32_t uiDevIdx, COscSwitchPosSet stSwitchPosSet)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    int rc = CDevApiManager::GetInstance().GetOpsApi().SetOscSwitchPos(uiDevIdx, stSwitchPosSet);
    return rc;
}

int
OSC_GetSwitchPos(uint32_t uiDevIdx, COscSwitchPos *pstSwitchPos)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    int rc = CDevApiManager::GetInstance().GetOpsApi().GetOscSwitchPos(uiDevIdx, *pstSwitchPos);
    return rc;
}


#ifdef __cplusplus
};
#endif



