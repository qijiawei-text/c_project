/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      HalApi.cpp
   Author:        Zhen Zhang
   Date:          2018-11-06
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-06] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "HalApi.h"
#include "Cmd.h"


/*==============================function=========================*/

int32_t
CHalApi::RegisterDev(const CRegisterDev &c_rstRegister)
{

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    if (!SetCmd(stShmCmd, stSemCmd, OID_HAL_REGISTER_DEV, (uint8_t *)&c_rstRegister, sizeof(c_rstRegister), SYNC)) {
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;

}

int32_t
CHalApi::UnregisterDev(uint32_t uiDevType, uint32_t uiDevIdx)
{

    /* check if shm of cmd is created OK */
    CShareMemory stShmCmd;
    CSemaphore stSemCmd;

    if (!IsShmCreated(0, SHM_TYPE_CMD, mc_staShmCfg, stShmCmd, stSemCmd)) {
        LOG_WRITE(EN_LOG_ERROR, "Share Memory error. uiDevIdx = %d", 0);
        return ERR_SHM_NOT_ACCESS;
    }

    /* generate cmd frame and set to shm */
    CCmdUnRegDev stData = {uiDevType, uiDevIdx};

    if (!SetCmd(stShmCmd, stSemCmd, OID_HAL_UNREGISTER_DEV, (uint8_t *)&stData, sizeof(stData), SYNC)) {
        return ERR_OPERATION_FAILED;
    }

    return OPLK_OK;


}




