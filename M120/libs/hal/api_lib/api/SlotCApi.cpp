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
#include "DevOcmCApi.h"
#include "DevOpsCApi.h"
#include "DevTdcmCApi.h"
#include "DevExtendCApi.h"
#include "DevOtdrCApi.h"
#include "DevWssCApi.h"
#include "DevOcsCApi.h"
#include "Cmd.h"
#include "Delay.h"

#ifdef __cplusplus
extern "C" {
#endif

int
SLOT_GetDevIdx(uint32_t uiDevType, uint32_t *puiDevIdx, uint32_t *puiDevSubIdx)
{
    uint32_t uiSlotNum = 0;
    CSlotInfo stInfo = {0};

    int ret = CDevApiManager::GetInstance().GetSlotApi().GetSlotNum(uiSlotNum);
    if (ret != OPLK_OK) {
        return ret;
    }

    if (DEV_TYPE_OSC == uiDevType) {
        uiDevType = DEV_TYPE_BA;
    } else if (DEV_TYPE_P_OSC == uiDevType) {
        uiDevType = DEV_TYPE_P_BA;
    }

    for (uint32_t uiSlotIdx = 0; uiSlotIdx < uiSlotNum; uiSlotIdx++) {
        CDevApiManager::GetInstance().GetSlotApi().GetInfo(uiSlotIdx, stInfo);

        for (uint32_t uiIdx = 0; uiIdx < stInfo.uiDevNum; uiIdx++) {
            if ( DEV_TYPE_BA == uiDevType || DEV_TYPE_PA == uiDevType || DEV_TYPE_OSC_OA == uiDevType) {
                if (DEV_TYPE_BA_PA == stInfo.astDevInfo[uiIdx].uiDevType) {
                    *puiDevIdx = stInfo.astDevInfo[uiIdx].uiDevIdx;
#ifdef __HCF__
                    *puiDevSubIdx = (DEV_TYPE_BA == uiDevType) ? 0 : 1;
                    if (DEV_TYPE_OSC_OA == uiDevType) {
                        *puiDevSubIdx = 2;
                        LOG_WRITE(EN_LOG_NOTICE, "SLOT_GetInfo uiDevType = %d, uiDevType = %d, DevIdx = %d, DevSubIdx = %d", stInfo.astDevInfo[uiIdx].uiDevType, uiDevType, *puiDevSubIdx, *puiDevSubIdx);
                    }
#else
                    *puiDevSubIdx = (DEV_TYPE_BA == uiDevType) ? 1 : 0;
#endif
                    return 0;
                }
            } else if ( DEV_TYPE_P_BA == uiDevType || DEV_TYPE_P_PA == uiDevType) {
                if (DEV_TYPE_P_BA_PA == stInfo.astDevInfo[uiIdx].uiDevType) {
                    *puiDevIdx = stInfo.astDevInfo[uiIdx].uiDevIdx;
                    *puiDevSubIdx = (DEV_TYPE_P_BA == uiDevType) ? 1 : 0;
                    return 0;
                }
            } else if (stInfo.astDevInfo[uiIdx].uiDevType == uiDevType) {
                *puiDevIdx = stInfo.astDevInfo[uiIdx].uiDevIdx;
                *puiDevSubIdx = 0;
                return 0;
            }
        }
    }

    return -1;
}


int
SLOT_SetOnline(uint32_t uiDevIdx, bool bOnline)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().SetOnline(uiDevIdx, bOnline);
}


int
SLOT_GetSlotNum(uint32_t *puiSlotNum)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().GetSlotNum(*puiSlotNum);
}

int
SLOT_SetMfg(uint32_t uiDevIdx, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().SetMfg(uiDevIdx, c_pcFilePath);
}

int
SLOT_ClearPlugFlag(uint32_t uiDevIdx)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().ClearPlugFlag(uiDevIdx);
}


int
SLOT_GetMfg(uint32_t uiDevIdx, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().GetMfg(uiDevIdx, *pstMfg);
}

int
SLOT_GetStatusData(uint32_t uiSlotIdx, CSlotStatusData *pstStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().GetStatusData(uiSlotIdx, *pstStatus);
}

int
SLOT_GetInfo(uint32_t uiDevIdx, CSlotInfo *pstInfo)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().GetInfo(uiDevIdx, *pstInfo);
}

int
SLOT_GetCali(uint32_t uiDevIdx, CCali *pstCaliInfo)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().GetCaliInfo(uiDevIdx, *pstCaliInfo);
}

int
SLOT_SetCali(uint32_t uiDevIdx, const char *pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().SetCali(uiDevIdx, pcFilePath);
}

int
SLOT_Reset(uint32_t uiDevIdx, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif

    int iRet = 0;
    CSlotInfo stSlotInfo;

    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    iRet = SLOT_GetInfo(uiDevIdx, &stSlotInfo);
    if (iRet != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "SLOT_GetInfo error. uiSlotIdx = %d, ucResetType = %d", uiDevIdx, ucResetType);
        return iRet;
    }

    for (int i = 0; i < (int)stSlotInfo.uiDevNum; i++) {

        switch (stSlotInfo.astDevInfo[i].uiDevType) {
        case DEV_TYPE_OA:
        case DEV_TYPE_BA_PA:
        case DEV_TYPE_P_BA_PA:
            /* iRet = EDFA_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType); */
            iRet = CDevApiManager::GetInstance().GetOaApi().Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_TDCM:
            iRet = TDCM_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_OPS:
        case DEV_TYPE_OPS4A:
        case DEV_TYPE_OPS4B:
            /* iRet = OPS_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType); */
            iRet = CDevApiManager::GetInstance().GetOpsApi().Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_OCM:
        case DEV_TYPE_P_OCM:
            /* iRet = OCM_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType); */
            iRet = CDevApiManager::GetInstance().GetOcmApi().Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_EXTEND:
            iRet = EXTEND_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_OTDR:
        case DEV_TYPE_P_OTDR:
            /* if (CARD_TYPE_400GZR_BETA == stSlotInfo.uiCardType) { */
            /* iRet = OTDR_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType); */
            /* } */
            iRet = CDevApiManager::GetInstance().GetOtdrApi().Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_WSS:
            iRet = WSS_Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        case DEV_TYPE_OCS:
            iRet = CDevApiManager::GetInstance().GetOcsApi().Reset(stSlotInfo.astDevInfo[i].uiDevIdx, ucResetType);
            break;
        default:
            LOG_WRITE(EN_LOG_ERROR, "Unknown device type(%d).", stSlotInfo.astDevInfo[i].uiDevType);
            break;
        }
    }


    CDevApiManager::GetInstance().GetSlotApi().Reset(uiDevIdx, ucResetType);

    return iRet;
}

int
SLOT_SetLed(uint32_t uiSlotIdx, uint32_t uiLedIdx, uint32_t uiState)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().SetLedState(uiSlotIdx, uiLedIdx, uiState);
}


int
SLOT_ReadEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().EepromRead(uiIdx, ulAddr, ulLen, pucVal);
}

int
SLOT_WriteEeprom(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    return CDevApiManager::GetInstance().GetSlotApi().EepromWrite(uiIdx, ulAddr, ulLen, pucVal);
}


#ifdef __cplusplus
};
#endif



