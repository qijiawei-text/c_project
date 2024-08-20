/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OtdrApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
COtdrApi::GetMfg(uint32_t ulDevIdx, CMfg &rstMfg)
{

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetMfg +++");
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataOtdrMfgPos(), sizeof(rstMfg));

    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);


    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetMfg ---");

    return OPLK_OK;

}

int32_t
COtdrApi::GetStatusData(uint32_t ulDevIdx, COtdrStatusData &rstStatus)
{

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataOtdrStaPos(), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_DEBUG, "start scan %u %u %u %lu",
              CShmOperator::GetDataOtdrStaPos(),
              rstStatus.ulScanStatus,
              rstStatus.stEventTop.ulCnt,
              sizeof(rstStatus));

    return OPLK_OK;

}



int32_t
COtdrApi::GetCfgData(uint32_t ulDevIdx, COtdrCfgData &rstCfg)
{



    return OPLK_OK;

}



int32_t
COtdrApi::StartScan(uint32_t ulDevIdx, uint32_t uiDevType)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "StartScan ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {ulDevIdx, uiDevType};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_START_SCAN, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Start scan error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);
    LOG_WRITE(EN_LOG_NOTICE, "StartScan --");
    return OPLK_OK;
}


int32_t
COtdrApi::SetPara(uint32_t ulDevIdx, uint32_t ulDistRang, uint32_t ulPulseWidth, double dSamplingResolution)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetPara ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrPara stData = {ulDevIdx, ulDevIdx,
                           ulDistRang, ulPulseWidth, dSamplingResolution};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_PARA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set para error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetPara --");
    return OPLK_OK;
}


int32_t
COtdrApi::SetMeasuringTime(uint32_t ulDevIdx, uint32_t ulMeasuringTime)
{

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetMeasuringTime ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrPara stData = {ulDevIdx, ulDevIdx,
                           0, 0, 0,
                           ulMeasuringTime};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_MTIME, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set measuring time error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetMeasuringTime --");
    return OPLK_OK;

}

int32_t
COtdrApi::SetUserIOR(uint32_t ulDevIdx, double dUserIOR)
{

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetUserIOR ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrPara stData = {ulDevIdx, ulDevIdx,
                           0, 0, 0,
                           0,
                           dUserIOR};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_UIOR, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set user ior error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetUserIOR --");
    return OPLK_OK;

}


int32_t
COtdrApi::SetThr(uint32_t ulDevIdx, double dRefThr, double dSpliceLosThr, double dFiberEndThr)
{

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetThr ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrPara stData = {ulDevIdx, ulDevIdx,
                           0, 0, 0,
                           0,
                           0,
                           dRefThr,
                           dSpliceLosThr,
                           dFiberEndThr};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_THR, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set thr error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetThr --");
    return OPLK_OK;

}

int32_t
COtdrApi::GetTemp(uint32_t ulDevIdx, COtdrTemp &rstTemp)
{

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetTemp +++");

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {ulDevIdx, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_GET_TEMP, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstTemp, CShmOperator::GetDataOtdrTempPos(), sizeof(rstTemp))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetTemp ---");
    return OPLK_OK;
}


int32_t
COtdrApi::Reset(uint32_t ulDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        /* let cold reset go */
        if (!(RST_MODE_COLD == ucResetType && ERR_OPERATION_FAILED == iErr)) {
            LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
            return iErr;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {ulDevIdx, (int)ucResetType, ulDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}

int32_t
COtdrApi::Upgrade(uint32_t ulDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdMfgData stData;
    stData.uiIdx = ulDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Upgrade error. uiDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}




int32_t
COtdrApi::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgRead ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdDbgData stData = {uiDevIdx, uiLen};
    if (uiLen > sizeof(stData.aucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "Buffer overflow. uiDevIdx = %d", 0);
        return ERR_OVERFLOW_ARG;
    }

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_DBG_READ, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Dbg read uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* get from memeory */
    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBuf, CShmOperator::GetDataOtdrDbgPos(), uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DbgRead --");
    return OPLK_OK;
}

int32_t
COtdrApi::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdDbgData stData = {uiDevIdx, uiLen};
    if (uiLen > sizeof(stData.aucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "Buffer overflow. uiDevIdx = %d", 0);
        return ERR_OVERFLOW_ARG;
    }

    memcpy(stData.aucBuf, pucBuf, uiLen);
    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_DBG_WRITE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Dbg write uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DbgWrite --");
    return OPLK_OK;
}

int32_t
COtdrApi::DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::DbgSetCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_DBG_SET_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgSetCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::DbgSetCommFail --");
    return OPLK_OK;
}

int32_t
COtdrApi::DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::DbgClearCommFail ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {uiDevIdx, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_DBG_CLEAR_COMM_FAIL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "DbgClearCommFail uiDevIdx = %d error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::DbgClearCommFail --");
    return OPLK_OK;
}



int32_t
COtdrApi::GetUpgadeStatus(uint32_t uiDevIdx)
{
    return GetStatus(uiDevIdx, mc_staShmCfg);
}

int32_t
COtdrApi::SetScanMode(uint32_t ulDevIdx, uint32_t ulScanMode)
{

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetScanMode ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrPara stData = {ulDevIdx, ulDevIdx,
                           0, 0, 0, 0, 0, 0, 0, 0,
                           ulScanMode};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_SCAN_MODE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set scan mode error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetScanMode --");
    return OPLK_OK;

}

int32_t
COtdrApi::SetStartOffset(uint32_t ulDevIdx, COtdrStartOffset *pstStartOffset)
{
    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetStartOffset ++ ulDevIdx = %d", ulDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdOtdrStartOffsetData stData;
    stData.uiIdx = ulDevIdx;
    stData.uiSubIdx = ulDevIdx;
    stData.uiPort = 0;
    memcpy(&stData.stVal, pstStartOffset, sizeof(COtdrStartOffset));

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_SET_START_OFFSET, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set para error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetStartOffset --");
    return OPLK_OK;
}

int32_t
COtdrApi::GetStartOffset(uint32_t ulDevIdx, COtdrStartOffset &rstStartOffset)
{

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetStartOffset +++");

    int32_t iErr = GetStatus(ulDevIdx, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. ulDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdNoData stData = {ulDevIdx, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_OTDR_GET_START_OFFSET, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error. ulDevIdx = %d", ulDevIdx);
        return ERR_OPERATION_FAILED;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(ulDevIdx, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", ulDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStartOffset, CShmOperator::GetDataOtdrStartOffsetPos(), sizeof(rstStartOffset))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status error. uiDevIdx = %d", ulDevIdx);
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrApi::GetStartOffset ---");
    return OPLK_OK;
}
