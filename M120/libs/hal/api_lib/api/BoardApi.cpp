/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardApi.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "BoardApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CBoardApi::GetChassisMfg(CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataChassisMfgPos(), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get mfginfo from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::GetRpcMfg(CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataRpcMfgPos(), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get mfginfo from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::GetChassisCali(CCali &rstCali)
{
    /* When upgrading, also obtain chassis calibration data */
#if 0
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }
#endif
    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCali, CShmOperator::GetDataChassisCaliPos(), sizeof(rstCali))) {
        LOG_WRITE(EN_LOG_ERROR, "Get chassis cali from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
CBoardApi::GetChassisCfg(uint32_t uiAddrSlice, const char *c_pcFilePath)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdCfgData stData;
    stData.uiAddrSlice = uiAddrSlice;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_GET_CHASSIS_CFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "get chassis cfg error. uiAddrSlice = %d, c_pcFilePath = %s", uiAddrSlice, c_pcFilePath);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
CBoardApi::SetChassisMfg(const char *c_pcFilePath)
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
    CCmdMfgData stData;
    stData.uiIdx = 0;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_CHASSIS_MFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetChassisConfigType(const char *pcConfigType)
{
    if (NULL == pcConfigType) {
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

    CCmdMfgData stData;
    stData.uiIdx = 0;
    memcpy(stData.acFilePath, pcConfigType, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_CHASSIS_CONFIG_TYPE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo config type error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetChassisCali(const char *c_pcFilePath)
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
    stData.uiIdx = 0;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_CHASSIS_CALI, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set caliinfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetChassisCfg(uint32_t uiAddrSlice, uint32_t uiSize, const char *c_pcFilePath)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdCfgData stData;
    stData.uiAddrSlice = uiAddrSlice;
    stData.uiSize = uiSize;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_CHASSIS_CFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "set chassis cfg error. uiAddrSlice = %d, c_pcFilePath = %s", uiAddrSlice, c_pcFilePath);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
CBoardApi::SetRpcMfg(const char *c_pcFilePath)
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
    CCmdMfgData stData;
    stData.uiIdx = 1;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_CHASSIS_MFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::GetUpgInfo(uint8_t *pucBuf, uint32_t ulLen)
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
    CCmdData stData = {0, (int32_t)ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_GET_UPG_INFO, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Get upg info error");
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

    memset(pucBuf, 0, ulLen);
    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBuf, CShmOperator::GetDataUpgInfoPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}
int32_t
CBoardApi::SetUpgInfo(uint8_t *pucBuf, uint32_t ulLen)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmd1KBufData stData;
    stData.uiLen = ulLen;
    memcpy(stData.aucBuf, pucBuf, ulLen);

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_UPG_INFO, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "set upg info error. ulLen = %d", ulLen);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
CBoardApi::GetBdlHead(uint8_t *pucBuf, uint32_t ulLen)
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
    CCmdData stData = {0, (int32_t)ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_GET_BDL_HEAD, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Get bundle head error");
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBuf, CShmOperator::GetDataBdlHeadPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}
int32_t
CBoardApi::SetBdlHead(uint8_t *pucBuf, uint32_t ulLen)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmd1KBufData stData;
    stData.uiLen = ulLen;
    memcpy(stData.aucBuf, pucBuf, ulLen);

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_BDL_HEAD, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "set bundle head error. ulLen = %d", ulLen);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
CBoardApi::GetSccMfg(CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataSccMfgPos(), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get mfginfo from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::GetSccCfg(uint32_t uiAddrSlice, const char *c_pcFilePath)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdCfgData stData;
    stData.uiAddrSlice = uiAddrSlice;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_GET_SCC_CFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "get chassis cfg error. uiAddrSlice = %d, c_pcFilePath = %s", uiAddrSlice, c_pcFilePath);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}


int32_t
CBoardApi::SetSccMfg(const char *c_pcFilePath)
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
    CCmdMfgData stData;
    stData.uiIdx = 0;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_SCC_MFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set mfginfo error.");
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetSccCfg(uint32_t uiAddrSlice, const char *c_pcFilePath)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdCfgData stData;
    stData.uiAddrSlice = uiAddrSlice;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_SCC_CFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "set scc cfg error. uiAddrSlice = %d, c_pcFilePath = %s", uiAddrSlice, c_pcFilePath);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}


int32_t
CBoardApi::GetStatusData(CBoardStatusData &rstStatus)
{

    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstStatus, CShmOperator::GetDataBoardStaPos(), sizeof(rstStatus))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::GetCfgData(CBoardCfgData &rstCfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstCfg, CShmOperator::GetDataBoardCfgPos(), sizeof(rstCfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get cfg data from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}



int32_t
CBoardApi::SetLedState(uint32_t uiDevIdx, uint32_t uiState)
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
    CCmdData stData = {uiDevIdx, (int32_t)uiState};

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_LED_STATE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set led state error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetLedTestMode(uint32_t uiDevIdx, bool bEnable)
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
    CCmdData stData = {uiDevIdx, (int32_t)bEnable};

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_LED_TESTMODE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set led state error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetLedLocationTestMode(uint32_t uiDevIdx, bool bEnable)
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
    CCmdData stData = {uiDevIdx, (int32_t)bEnable};

    if (!SetCmd(stShmCmd, stSemCmd, OID_BOARD_SET_LED_LOCATION_TESTMODE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set led state error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}


int32_t
CBoardApi::GetFanMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataFanMfgPos(uiDevIdx), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get fan%d mfginfo from share memory error", uiDevIdx);
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
CBoardApi::SetFanMfg(uint32_t uiDevIdx, const char *c_pcFilePath)
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
    stData.uiIdx = uiDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_FAN_SET_MFG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set fan%d mfginfo error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CBoardApi::SetFanSpeed(uint32_t uiDevIdx, uint32_t uiDuty)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Speed(%d) +++", uiDevIdx, uiDuty);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiDuty};

    if (!SetCmd(stShmCmd, stSemCmd, OID_FAN_SET_SPEED, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set fan%d speed error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Speed(%d) ---", uiDevIdx, uiDuty);

    return OPLK_OK;

}

int32_t
CBoardApi::GetFanSpeedCfg(uint32_t uiDevIdx, uint32_t *uiCfgSpeed)
{
    int32_t iErr = CDevApi::GetStatus(0, mc_staShmCfg);

    if ((iErr != OPLK_OK) && (iErr != ERR_UPGRADING) && (iErr != ERR_LOADING)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    /* check if shm of data is created OK */
    CShareMemory stShmData;
    CSemaphore stSemData;

    if (!CDevApi::IsShmCreated(0, SHM_TYPE_DATA, mc_staShmCfg, stShmData, stSemData)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error.");
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)uiCfgSpeed, CShmOperator::GetDataFanCfgPos(uiDevIdx), sizeof(uint32_t))) {
        LOG_WRITE(EN_LOG_ERROR, "Get fan speed from share memory error");
        return OPLK_ERR;
    }
    return OPLK_OK;
}

int32_t
CBoardApi::SetResetCauseInFan(uint32_t uiDevIdx, uint32_t uiResetCause)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) resetcause(%d) +++", uiDevIdx, uiResetCause);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiResetCause};

    if (!SetCmd(stShmCmd, stSemCmd, OID_FAN_SET_RESET_CAUSE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set fan%d resetacuase error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) resetcause(%d) ---", uiDevIdx, uiResetCause);

    return OPLK_OK;

}

int32_t
CBoardApi::SetFanControllerEEP(uint32_t uiDevIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) SetFanControllerEEP +++", uiDevIdx);

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
    CCmdData stData = {uiDevIdx, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_FAN_SET_CONTROLLEREEP, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set fan%d ControllerEEP error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) SetFanControllerEEP ---", uiDevIdx);

    return OPLK_OK;

}


int32_t
CBoardApi::SetFanLed(uint32_t uiDevIdx, uint32_t uiValue)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Led(%d) +++", uiDevIdx, uiValue);

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
    CCmdData stData = {uiDevIdx, (int32_t)uiValue};

    if (!SetCmd(stShmCmd, stSemCmd, OID_FAN_SET_LED, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Set fan%d speed error", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Led(%d) ---", uiDevIdx, uiValue);

    return OPLK_OK;

}


int32_t
CBoardApi::GetPowerMfg(uint32_t uiDevIdx, CMfg &rstMfg)
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
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&rstMfg, CShmOperator::GetDataPowerMfgPos(uiDevIdx), sizeof(rstMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Get power mfginfo from share memory error");
        return OPLK_ERR;
    }

    return OPLK_OK;

}


int32_t
CBoardApi::LogicRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "LogicRead ++ Addr = 0x%lX, len = 0x%lX", ulAddr, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulAddr, ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_READ_REG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic read 0x%lx error", ulAddr);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucVal, CShmOperator::GetDataLogicStaPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_TRACE, "LogicRead -- Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

    return OPLK_OK;

}

int32_t
CBoardApi::LogicWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicWrite ++ Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_WRITE_REG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic write 0x%lx error", ulAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicWrite --");

    return OPLK_OK;

}

int32_t
CBoardApi::LogicReadSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicReadSlc ++ Addr = 0x%lX, len = 0x%lX", ulAddr, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulAddr, ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_READ_REG_SLC, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic read slc 0x%lx error", ulAddr);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucVal, CShmOperator::GetDataLogicStaPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicReadSlc -- Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

    return OPLK_OK;

}

int32_t
CBoardApi::LogicWriteSlc(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicWriteSlc ++ Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

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

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_WRITE_REG_SLC, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic write 0x%lx error", ulAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LogicWriteSlc --");

    return OPLK_OK;

}

int32_t
CBoardApi::SetResetType(uint32_t uiType)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetResetType ++ uiType = %d", uiType);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {0, (int32_t)uiType};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_SET_RESTTYPE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "SetResetType %d error", uiType);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetResetType --");

    return OPLK_OK;

}

int32_t
CBoardApi::SetLogLevel(uint32_t uiLogLevel)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetResetType ++ uiType = %u", uiLogLevel);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {0, (int32_t)uiLogLevel};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_SET_LOG_LEVEL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "SetLogLevel %u error", uiLogLevel);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetResetType --");

    return OPLK_OK;

}

int32_t
CBoardApi::CfgWatchDog(bool bEnable, uint8_t ucTimeout)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CfgWatchDog ++ bEnable = %d, ucTimeout = %d", bEnable, ucTimeout);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {(uint8_t)bEnable, ucTimeout};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_CFG_WATCHDOG, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "CfgWatchDog %d error", bEnable);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CfgWatchDog --");

    return OPLK_OK;

}

int32_t
CBoardApi::SetApsdEnable(uint32_t uiIdx, bool bCfg)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetApsdEnable ++ uiType = %d, uiType = %d", uiIdx, bCfg);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiIdx, (int32_t)bCfg};
    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_CFG_APSD, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "SetApsdEnable %d error", uiIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetApsdEnable --");

    return OPLK_OK;

}


int32_t
CBoardApi::LogicReset(uint32_t uiDevIdx, uint8_t ucResetType)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdData stData = {uiDevIdx, (int)ucResetType, uiDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_RESET, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic reset error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset --");
    return OPLK_OK;
}



int32_t
CBoardApi::LogicUpgrade(uint32_t uiDevIdx, const char *c_pcFilePath)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK && iErr != ERR_UPGRADING) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade ++ uiDevIdx = %d", uiDevIdx);

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdMfgData stData;
    stData.uiIdx = uiDevIdx;
    memcpy(stData.acFilePath, c_pcFilePath, sizeof(stData.acFilePath));

    if (!SetCmd(stShmCmd, stSemCmd, OID_LOGIC_UPGRADE, (uint8_t *)&stData, sizeof(stData), ASYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Logic Upgrade error. uiDevIdx = %d", uiDevIdx);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade --");
    return OPLK_OK;
}



int32_t
CBoardApi::SpiSendCommand(uint32_t ulChip, uint8_t *pucBufW, uint8_t *pucBufR, uint32_t ulLen)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiSendCommand ++ ulChip = 0x%X, len = 0x%X", ulChip, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulChip, ulLen};
    memcpy(stData.aucVal, pucBufW, ulLen);

    if (!SetCmd(stShmCmd, stSemCmd, OID_SPICOM_WRITE_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "spi read write 0x%x error", ulChip);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucBufR, CShmOperator::GetDataLogicStaPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiSendCommand -- ");

    return OPLK_OK;
}

int32_t
CBoardApi::SpiFlashRead(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiFlashRead ++ chip = 0x%lX, len = 0x%lX", ulChip, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulChip, ulLen, ulAddr};

    if (!SetCmd(stShmCmd, stSemCmd, OID_SPIFLASH_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi flash read 0x%lx error", ulChip);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucVal, CShmOperator::GetDataLogicStaPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiFlashRead -- chip = 0x%lX, len = 0x%lX, val = 0x%02X", ulChip, ulLen, pucVal[0]);

    return OPLK_OK;

}

int32_t
CBoardApi::SpiFlashWrite(uint64_t ulChip, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiFlashWrite ++ chip = 0x%lX, len = 0x%lX, val = 0x%02X", ulChip, ulLen, pucVal[0]);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulChip, ulLen, ulAddr};
    memcpy(stData.aucVal, pucVal, ulLen);

    if (!SetCmd(stShmCmd, stSemCmd, OID_SPIFLASH_WRITE_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi flash write 0x%lx error", ulChip);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SpiFlashWrite --");

    return OPLK_OK;

}


int32_t
CBoardApi::MdioRead(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "MdioRead ++ Addr = 0x%X, reg = 0x%02X", ulPhyAddr, ulRegAddr);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulPhyAddr, sizeof(uint16_t), ulRegAddr};

    if (!SetCmd(stShmCmd, stSemCmd, OID_MDIO_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio read addr 0x%x reg = 0x%02X error", ulPhyAddr, ulRegAddr);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pusData, CShmOperator::GetDataLogicStaPos(), sizeof(uint16_t))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "MdioRead -- Addr = 0x%X, reg = 0x%02X, val = 0x%X", ulPhyAddr, ulRegAddr, *pusData);

    return OPLK_OK;

}

int32_t
CBoardApi::MdioWrite(uint32_t uiIdx, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "MdioWrite ++ Addr = 0x%X, reg = 0x%02X, val = 0x%0X", ulPhyAddr, ulRegAddr, usData);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulPhyAddr, sizeof(usData), ulRegAddr};
    memcpy(stData.aucVal, &usData, sizeof(usData));

    if (!SetCmd(stShmCmd, stSemCmd, OID_MDIO_WRITE_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio write 0x%x 0x%x error", ulPhyAddr, ulRegAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "MdioWrite --");

    return OPLK_OK;

}


int32_t
CBoardApi::NetSwitchRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_DEBUG, "GetStatus error %d", iErr);
        return iErr;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "NetSwitchRead ++ Addr = 0x%X, reg = 0x%X", ulPhyAddr, ulRegAddr); */

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulPhyAddr, sizeof(uint16_t), ulRegAddr};

    if (!SetCmd(stShmCmd, stSemCmd, OID_NET_SWITCH_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Net Switch read addr 0x%x reg = 0x%02X error", ulPhyAddr, ulRegAddr);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pusData, CShmOperator::GetDataLogicStaPos(), sizeof(uint16_t))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "NetSwitchRead -- Addr = 0x%X, reg = 0x%X, val = 0x%04X", ulPhyAddr, ulRegAddr, *pusData); */

    return OPLK_OK;

}

int32_t
CBoardApi::NetSwitchWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "NetSwitchWrite ++ Addr = 0x%X, reg = 0x%X, val = 0x%04X", ulPhyAddr, ulRegAddr, usData);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulPhyAddr, sizeof(usData), ulRegAddr};
    memcpy(stData.aucVal, &usData, sizeof(usData));

    if (!SetCmd(stShmCmd, stSemCmd, OID_NET_SWITCH_WRITE_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch write 0x%x 0x%x error", ulPhyAddr, ulRegAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "NetSwitchWrite --");

    return OPLK_OK;

}


int32_t
CBoardApi::SwitchPhyRead(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SwitchPhyRead ++ Addr = 0x%X, reg = 0x%X", ulPhyAddr, ulRegAddr);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulPhyAddr, sizeof(uint16_t), ulRegAddr};

    if (!SetCmd(stShmCmd, stSemCmd, OID_NET_SWITCH_PHY_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Net Switch phy read addr 0x%x reg = 0x%02X error", ulPhyAddr, ulRegAddr);
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pusData, CShmOperator::GetDataLogicStaPos(), sizeof(uint16_t))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SwitchPhyRead -- Addr = 0x%X, reg = 0x%X, val = 0x%04X", ulPhyAddr, ulRegAddr, *pusData);

    return OPLK_OK;

}

int32_t
CBoardApi::SwitchPhyWrite(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usData)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SwitchPhyWrite ++ Addr = 0x%X, reg = 0x%02X, val = 0x%0X", ulPhyAddr, ulRegAddr, usData);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {0, ulPhyAddr, sizeof(usData), ulRegAddr};
    memcpy(stData.aucVal, &usData, sizeof(usData));

    if (!SetCmd(stShmCmd, stSemCmd, OID_NET_SWITCH_PHY_WRITE_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch phy write 0x%x 0x%x error", ulPhyAddr, ulRegAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SwitchPhyWrite --");

    return OPLK_OK;

}

int32_t
CBoardApi::EepromRead(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromRead ++ Addr = 0x%lX, len = 0x%lX", ulAddr, ulLen);

    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdLogicData stData = {uiIdx, ulAddr, ulLen};

    if (!SetCmd(stShmCmd, stSemCmd, OID_EEPROM_READ_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
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

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)pucVal, CShmOperator::GetDataLogicStaPos(), ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromRead -- Addr = 0x%lX, len = 0x%lX, val = 0x%02X", ulAddr, ulLen, pucVal[0]);

    return OPLK_OK;

}

int32_t
CBoardApi::EepromWrite(uint32_t uiIdx, uint64_t ulAddr, uint64_t ulLen, uint8_t *pucVal)
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

    if (!SetCmd(stShmCmd, stSemCmd, OID_EEPROM_WRITE_DATA, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom write 0x%lx error", ulAddr);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "EepromWrite --");

    return OPLK_OK;

}

int32_t
CBoardApi::LoopEnable(bool bEnable)
{
    LOG_WRITE(EN_LOG_NOTICE, "LoopEnable ++ bEnable = %d", bEnable);

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
    CCmdData stData = {0, bEnable ? 1 : 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_HAL_DBG_LOOP, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "LoopEnable %d error", bEnable);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LoopEnable --");

    return OPLK_OK;

}

int32_t
CBoardApi::LoopInterval(uint32_t uiInterval)
{
    LOG_WRITE(EN_LOG_NOTICE, "LoopInterval ++ uiInterval = %d", uiInterval);

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
    CCmdData stData = {0, (int32_t)uiInterval};

    if (!SetCmd(stShmCmd, stSemCmd, OID_HAL_DBG_LOOPINTERVAL, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "LoopInterval %d error", uiInterval);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "LoopInterval --");

    return OPLK_OK;

}

int32_t
CBoardApi::IdleEnable(bool bEnable)
{
    LOG_WRITE(EN_LOG_NOTICE, "IdleEnable ++ bEnable = %d", bEnable);

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
    CCmdData stData = {0, bEnable ? 1 : 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_HAL_DBG_IDLE, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "LoopEnable %d error", bEnable);
        return ERR_OPERATION_FAILED;
    }

    LOG_WRITE(EN_LOG_NOTICE, "IdleEnable --");

    return OPLK_OK;

}

int32_t
CBoardApi::I2cTest(uint32_t uiPsuIdx, uint32_t uiI2cIdx)
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
    CCmdKeyData stData = {0, (int32_t)uiPsuIdx, (int32_t)uiI2cIdx, 0};

    if (!SetCmd(stShmCmd, stSemCmd, OID_I2C_TEST, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "I2c Test error. uiIdx = %d", uiPsuIdx);
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;
}

int32_t
CBoardApi::GetUpgadeStatus()
{
    int32_t iStatus = GetStatus(0, mc_staShmCfg);
    /*LOG_WRITE(EN_LOG_NOTICE, "Upgrade Status = %d", iStatus); */
    return iStatus;
}

int32_t
CBoardApi::ConvertDevType(const std::string &strName, uint32_t &uiDevType)
{
    int32_t iErr = GetStatus(0, mc_staShmCfg);

    if (iErr != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "GetStatus error %d", iErr);
        return iErr;
    }
    /* check if shm of data is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;
    uint32_t uiDevIdx = 0;

    if (!IsShmCreated(uiDevIdx, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", uiDevIdx);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    if (!SetCmd(stShmCmd, stSemCmd, OID_FUNC_GET_DEV_TYPE, (uint8_t *)strName.c_str(), strName.length() + 1, SYNC)) {
        LOG_WRITE(EN_LOG_ERROR, "ConvertDevType, uiDevIdx = %d error", uiDevIdx);
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

    uint32_t uiType = DEV_TYPE_NONE;

    if (!CShmOperator::Get(stShmData, stSemData, (uint8_t *)&uiType, CShmOperator::GetDataFuncDevType(), sizeof(uint32_t))) {
        LOG_WRITE(EN_LOG_ERROR, "Get status from share memory error");
        return OPLK_ERR;
    }

    uiDevType = uiType;

    return OPLK_OK;
}