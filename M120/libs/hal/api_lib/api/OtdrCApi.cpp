/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrCApi.cpp
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


#include "DevOtdrCApi.h"
#include "Cmd.h"
#include "Delay.h"
#include "DevSlotCApi.h"
#include "DevEdfaCApi.h"

#include <map>

#ifdef __cplusplus
extern "C" {
#endif



int
OTDR_GetMfg(uint32_t uiDevType, CMfg *pstMfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().GetMfg(uiDevIdx, *pstMfg);
}

int
OTDR_GetStatusData(uint32_t uiDevType, COtdrStatusData *pstStatus)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().GetStatusData(uiDevIdx, *pstStatus);
}

int
OTDR_GetCfgData(uint32_t uiDevType, COtdrCfgData *pstCfg)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().GetCfgData(uiDevIdx, *pstCfg);
}

int
OTDR_StartScan(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().StartScan(uiDevIdx, uiDevType);
}

int
OTDR_StartScanPort(uint32_t uiDevType, uint32_t ulPortId)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    /* TODO
       Hard code for EDFA type.
       Should be fixed to general code in the future.
     */
    uint32_t uiOaDevType = uiDevType == DEV_TYPE_OTDR ? DEV_TYPE_PA : DEV_TYPE_P_PA;
    uint32_t uiPortIndex = ulPortId - 1;
    if (uiPortIndex < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid OTDR port id %u", ulPortId);
        return OPLK_ERR;
    }

    /* Cache the EDFA calibration data */
    static std::map<uint32_t, COaCaliData> m_mapCali;

    /* switch to OTDR port */
#ifdef __HCF__
    int rc = EDFA_SetOtdrSwitchPos(uiOaDevType, (1 << 16) | ulPortId);
#else
    int rc = EDFA_SetOtdrSwitchPos(uiOaDevType, ulPortId);
#endif
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Switch OTDR port %u error. dev %u, rc %d", ulPortId, uiOaDevType, rc);
        return rc;
    }

    if (m_mapCali.find(uiOaDevType) == m_mapCali.end()) {
        /* Get the OTDR port IL from EDFA calibration data */
        COaCaliData stCali = {0};
        memset(&stCali, 0, sizeof(stCali));
        rc = EDFA_GetCali(uiOaDevType, &stCali);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get EDFA calibration data error. dev %u, rc %d", uiOaDevType, rc);
            return rc;
        }

        /* Check the calibration data CRC32 */
        uint32_t uiCrcCalc = UTIL_Crc32Calc((uint8_t *)stCali.stOtdrCaliInfo.stOtdrStartOffsetPort, sizeof(COtdrStartOffsetAll) - sizeof(uiCrcCalc));
        if (uiCrcCalc != stCali.stOtdrCaliInfo.ulCrc32) {
            LOG_WRITE(EN_LOG_ERROR, "OTDR switch calibration data CRC32 error. dev %u, calc crc %u, recv crc %u",
                      uiOaDevType, uiCrcCalc, stCali.stOtdrCaliInfo.ulCrc32);
            return OPLK_ERR;
        }

        /* Check the calibration data is valid or not */
        if (uiPortIndex >= sizeof(stCali.stOtdrCaliInfo.stOtdrStartOffsetPort) / sizeof(COtdrStartOffsetPort) ||
            stCali.stOtdrCaliInfo.stOtdrStartOffsetPort[uiPortIndex].uiPositionOffset <= 0 ||
            isnan(stCali.stOtdrCaliInfo.stOtdrStartOffsetPort[uiPortIndex].fLossOffset) == 1) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid OTDR switch calibration data. dev %u, port %u", uiOaDevType, ulPortId);
            return OPLK_ERR;
        }

        m_mapCali[uiOaDevType] = stCali;
    }

    /* Set the OTDR port offset */
    const auto &it = m_mapCali.find(uiOaDevType);
    COtdrStartOffset stStartOffset = {it->second.stOtdrCaliInfo.stOtdrStartOffsetPort[uiPortIndex].uiPositionOffset,
                                      it->second.stOtdrCaliInfo.stOtdrStartOffsetPort[uiPortIndex].fLossOffset};
    rc = OTDR_SetStartOffset(uiDevType, &stStartOffset);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set OTDR port %u offset error. rc = %d", ulPortId, rc);
        return rc;
    }

    return OTDR_StartScan(uiDevType);
}

int
OTDR_SetPara(uint32_t uiDevType, uint32_t ulDistRang, uint32_t ulPulseWidth, double dSamplingResolution)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().SetPara(uiDevIdx, ulDistRang, ulPulseWidth, dSamplingResolution);
}

int
OTDR_SetMeasuringTime(uint32_t uiDevType, uint32_t ulMeasuringTime)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().SetMeasuringTime(uiDevIdx, ulMeasuringTime);
}

int
OTDR_SetUserIOR(uint32_t uiDevType, double dUserIOR)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);

    return CDevApiManager::GetInstance().GetOtdrApi().SetUserIOR(uiDevIdx, dUserIOR);
}

int
OTDR_SetThr(uint32_t uiDevType, double dRefThr, double dSpliceLosThr, double dFiberEndThr)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().SetThr(uiDevIdx, dRefThr, dSpliceLosThr, dFiberEndThr);
}


int
OTDR_Reset(uint32_t uiDevType, uint8_t ucResetType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().Reset(uiDevIdx, ucResetType);
}

int
OTDR_Upgrade(uint32_t uiDevType, const char *c_pcFilePath)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().Upgrade(uiDevIdx, c_pcFilePath);
}

int
OTDR_GetUpgadeStatus(uint32_t uiDevType)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().GetUpgadeStatus(uiDevIdx);
}

int
OTDR_DbgRead(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().DbgRead(uiDevIdx, pucBuf, uiLen);
}

int
OTDR_DbgWrite(uint32_t uiDevType, uint8_t *pucBuf, uint32_t uiLen)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().DbgWrite(uiDevIdx, pucBuf, uiLen);
}

int
OTDR_SetScanMode(uint32_t uiDevType, uint32_t ulScanMode)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().SetScanMode(uiDevIdx, ulScanMode);
}

int
OTDR_SetStartOffset(uint32_t uiDevType, COtdrStartOffset *pstStartOffset)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().SetStartOffset(uiDevIdx, pstStartOffset);
}

int
OTDR_GetStartOffset(uint32_t uiDevType, COtdrStartOffset *pstStartOffset)
{
#ifdef _DEBUG_SIMULATE_
    return 0;
#endif
    uint32_t uiDevIdx = 0, uiSubIdx = 0;
    GET_DEV_IDX(uiDevType, &uiDevIdx, &uiSubIdx);
    return CDevApiManager::GetInstance().GetOtdrApi().GetStartOffset(uiDevIdx, *pstStartOffset);
}

#ifdef __cplusplus
};
#endif



