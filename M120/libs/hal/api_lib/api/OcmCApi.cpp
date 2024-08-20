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


#include "DevOcmCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
OCM_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().GetMfg(uiDevIdx, *pstMfg);
}

int
OCM_GetStatusData(uint32_t uiDevType, COcmStatusData *pstOcmStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return 0;
}

int
OCM_GetChPower(uint32_t uiDevType, uint32_t uiPortId, COcmChPower *pstOcmChPower)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().GetChPower(uiDevIdx, uiPortId, *pstOcmChPower);
}

int
OCM_GetOsaData(uint32_t uiDevType, uint32_t uiPortId, COcmOsa *pstOcmOsa)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().GetOsaData(uiDevIdx, uiPortId, *pstOcmOsa);
}

int
OCM_GetRealTimeOsaData(uint32_t uiDevType, uint32_t uiPortId, COcmOsa *pstOcmOsa)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().GetRealTimeOsa(uiDevIdx, uiPortId, *pstOcmOsa);
}



int
OCM_SetWaveplan(uint32_t uiDevType, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().SetWavePlan(uiDevIdx, uiPortId, uiChCnt, pstWavePlan);
}

int
OCM_SetOcmWaveplan(uint32_t uiDevType, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().SetOcmWavePlan(uiDevIdx, uiPortId, uiChCnt, pstWavePlan);
}


int
OCM_Reset(uint32_t uiDevType, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().Reset(uiDevIdx, ucResetType);
}


int
OCM_Upgrade(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().Upgrade(uiDevIdx, c_pcFilePath);
}

#ifdef __cplusplus
};
#endif

int
OCM_Commit(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().Commit(uiDevIdx);
}

int
OCM_GetUpgradeStatus(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiDevSubIdx);

    return CDevApiManager::GetInstance().GetOcmApi().GetUpgadeStatus(uiDevIdx);
}



int
OCM_SetVoa(uint32_t uiDevType, int16_t sAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetExtendApi().SetVoaAttn(0, sAttn, 4);
}

int
OCM_GetVoa(uint32_t uiDevType, int16_t *psAttn)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevTypeOfOa = 0;
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    if (DEV_TYPE_OCM == uiDevType) {
        uiDevTypeOfOa = DEV_TYPE_PA;
    } else if (DEV_TYPE_P_OCM == uiDevType) {
        uiDevTypeOfOa = DEV_TYPE_P_PA;
    } else {
        return ERR_INVAL_ARG;
    }
    GET_DEV_IDX(uiDevTypeOfOa, &uiDevIdx, &uiDevSubIdx);

    COaStatusData stData;
    memset(&stData, 0, sizeof(COaStatusData));
    int rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stData, uiDevSubIdx);
    if (OPLK_OK == rc) {
        *psAttn = stData.stOaCurData.asVoaAttnActual[VOA_OCM];
    }

    return rc;
}

int
OCM_GetVoaPd(uint32_t uiDevType, int16_t *psVoaIn, int16_t *psVoaOut)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevTypeOfOa = 0;
    uint32_t uiDevIdx = 0xFFFF, uiDevSubIdx = 0xFFFF;
    if (DEV_TYPE_OCM == uiDevType) {
        uiDevTypeOfOa = DEV_TYPE_PA;
    } else if (DEV_TYPE_P_OCM == uiDevType) {
        uiDevTypeOfOa = DEV_TYPE_P_PA;
    } else {
        return ERR_INVAL_ARG;
    }
    GET_DEV_IDX(uiDevTypeOfOa, &uiDevIdx, &uiDevSubIdx);

    COaStatusData stData;
    memset(&stData, 0, sizeof(COaStatusData));
    int rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stData, uiDevSubIdx);
    if (OPLK_OK == rc) {
#ifdef __OLSP__
        *psVoaIn = stData.stOaCurData.asExtPdPower[PD16_EDFA2_OCM_VOA_IN_PWR];
        *psVoaOut = stData.stOaCurData.asExtPdPower[PD17_EDFA2_OCM_VOA_OUT_PWR];
#endif
#ifdef __HCF__
        *psVoaIn = stData.stOaCurData.asExtPdPower[OCM_VOA_IN_PWR];
        *psVoaOut = stData.stOaCurData.asExtPdPower[OCM_VOA_OUT_PWR];
#endif

    }

    return rc;
}


