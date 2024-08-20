/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotApi.cpp
   Author:        Zhen Zhang
   Date:          2019-02-20
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-02-20] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SlotApi.h"
#include "Cmd.h"

/*==============================function=========================*/
int32_t
CSlotApi::SetOnline(uint32_t uiSlotIdx, bool bOnline)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CSlotApi::SetOnline +++");

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }


    /* generate cmd frame and set to shm */
    CCmdData stData;
    stData.uiIdx = uiSlotIdx;
    stData.iVal = bOnline;

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_SET_ONLINE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo error.");
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotApi::SetOnline ---");

    return OPLK_OK;

}

int32_t
CSlotApi::GetSlotNum(uint32_t &ruiSlotNum)
{

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiSlotIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&ruiSlotNum, CShmOperator::GetDataSlotNumPos(), sizeof(ruiSlotNum))) {
        LOG_WRITE(EN_LOG_ERROR, "Slot get num error.");
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CSlotApi::GetInfo(uint32_t uiSlotIdx, CSlotInfo &rstInfo)
{

    if (uiSlotIdx > SLOT_MAX_NUM) {
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiSlotIdx = %d", uiSlotIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstInfo, CShmOperator::GetDataSlotInfoPos(uiSlotIdx), sizeof(rstInfo))) {
        LOG_WRITE(EN_LOG_ERROR, "Slot get info error. uiSlotIdx = %d", uiSlotIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
CSlotApi::GetMfg(uint32_t uiSlotIdx, CMfg &rstMfg)
{

    if (uiSlotIdx > SLOT_MAX_NUM) {
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiSlotIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataSlotMfgPos(uiSlotIdx), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Slot get mfginfo error. uiSlotIdx = %d", uiSlotIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CSlotApi::SetMfg(uint32_t uiSlotIdx, const char *c_pcFilePath)
{

    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }


    /* generate cmd frame and set to shm */
    CCmdMfgData stData;
    stData.uiIdx = uiSlotIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_SET_MFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CSlotApi::ClearPlugFlag(uint32_t uiSlotIdx)
{

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }


    /* generate cmd frame and set to shm */
    CCmdNoData stData;
    stData.uiIdx = uiSlotIdx;

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_CLEAR_PLUG_FLAG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Clear plug flag error. uiSlotIdx = %d", uiSlotIdx);
        return ERR_OPERATION_FAILED;
    }


    return OPLK_OK;

}



int32_t
CSlotApi::GetCaliInfo(uint32_t uiSlotIdx, CCali &rstCaliInfo)
{

    if (uiSlotIdx > SLOT_MAX_NUM) {
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiSlotIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCaliInfo, CShmOperator::GetDataSlotCaliPos(uiSlotIdx), sizeof(rstCaliInfo))) {
        LOG_WRITE(EN_LOG_ERROR, "Slot get cali error. uiSlotIdx = %d", uiSlotIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CSlotApi::SetCali(uint32_t uiSlotIdx, const char *c_pcFilePath)
{
    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdCaliData stData = {0};
    stData.uiIdx = uiSlotIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_SET_CALI, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set caliinfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CSlotApi::SetLedState(uint32_t uiSlotIdx, uint32_t uiLedIdx, uint32_t uiState)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);


    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiSlotIdx, (int32_t)uiState, uiLedIdx};


    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_SET_LED_STATE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set led state error. uiDevIdx = %d", uiLedIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}
int32_t
CSlotApi::EepromRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromRead ++ Addr = 0x%lX, len = %lu", ulAddr, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulAddr, ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_EEPROM_READ, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom read 0x%lx error", ulAddr);
        return ERR_OPERATION_FAILED;
    }

    /* get from memeory */
    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucVal, CShmOperator::GetDataSlotDbgStaPos(uiIdx), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromRead -- Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

    return OPLK_OK;

}

int32_t
CSlotApi::EepromWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromWrite ++ Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulAddr, ulLen};
    memcpy(stData.aucVal, pucVal, ulLen);

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_EEPROM_WRITE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom write 0x%lx error", ulAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromWrite --");

    return OPLK_OK;

}


int32_t
CSlotApi::GetStatusData(uint32_t uiSlotIdx, CSlotStatusData &rstStatus)
{

    if (uiSlotIdx > SLOT_MAX_NUM) {
        return ERR_INVAL_ARG;
    }

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiSlotIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataSlotStatusPos(uiSlotIdx), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "Slot get status error. uiSlotIdx = %d", uiSlotIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
CSlotApi::Reset(uint32_t uiDevIdx, uint32_t uiResetType, uint32_t uiSubIdx)
{

    int32_t iErr = GetStatus(uiDevIdx, mc_staShmCfg);


    LOG_WRITE(EN_LOG_NOTICE, "Reset +++");

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int32_t)uiResetType, uiSubIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_SLOT_RESET, (uint8_t *)&stData, sizeof(stData), m_iSyncFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "Osc reset error. uiDevIdx = %d, uiResetType = %d", uiDevIdx, uiResetType);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Reset ---");

    return OPLK_OK;

}

