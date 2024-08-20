/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      HalCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Cmd.h"
#include "HalCmd.h"
#include "DevData.h"
#include "Register.h"

/*=============================variables=========================*/

std::array<CShareMemory, SHM_TYPE_MAX> CHalCmd::ms_staShm;
std::array<CSemaphore, SHM_TYPE_MAX> CHalCmd::ms_staSem;


/*==============================function=========================*/

void
CHalCmd::CreateShm(void)
{

    for (int32_t i; i < SHM_TYPE_MAX; i++) {
        ms_staShm.at(i).CreateAttache(mc_staShmCfg.at(i).eShmKey, mc_staShmCfg.at(i).eShmSize);
        ms_staSem.at(i).Create(mc_staShmCfg.at(i).eSemKey);
    }

}

bool
CHalCmd::RegisterDev(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CRegisterDev)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CRegisterDev *pstData = (CRegisterDev *)cv_pucBuf;

    std::vector<CRegisterInfo> staRegInfo = { /*pstData->uiDevIdx, pstData->uiHwType, pstData->uiHwIdx,
                                                 pstData->uiHwSubIdx, pstData->uiSubType*/
    };

    bool bRegFlg = false;
    CRegisterFactory stRegisterFactory;
    std::shared_ptr<CRegister> stpRegister = stRegisterFactory.GetRegister(pstData->uiDevType, staRegInfo, 0);

    if (stpRegister != nullptr) {
        bRegFlg = stpRegister->Register();
    }

    return bRegFlg;

}

bool
CHalCmd::UnregisterDev(uint32_t uiLen, const uint8_t *cv_pucBuf)
{

    if ((uiLen != sizeof(CCmdUnRegDev)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdUnRegDev *pstData = (CCmdUnRegDev *)cv_pucBuf;

    bool bUnregFlg = false;
    CRegisterFactory stRegisterFactory;
    std::shared_ptr<CRegister> stpRegister = stRegisterFactory.GetRegister(pstData->uiDevType);

    if (stpRegister != nullptr) {
        bUnregFlg = stpRegister->Unregister(pstData->uiDevIdx);
    }

    return bUnregFlg;

}



bool
CHalCmd::LoopEnable(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::LoopEnable +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;
    bool bEnable = (1 == pstData->iVal) ? true : false;

    CDevThread::SetLoopEnable(bEnable);

    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::LoopEnable %d", bEnable);
    return true;
}

bool
CHalCmd::LoopInterval(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::LoopInterval +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;
    uint32_t uiInterval = pstData->iVal;

    CDevThread::SetLoopInterval(uiInterval);

    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::LoopInterval %d", uiInterval);
    return true;
}

bool
CHalCmd::IdleEnable(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::IdleEnable +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;
    bool bEnable = (1 == pstData->iVal) ? true : false;

    CDevThread::SetForceIdle(bEnable);

    LOG_WRITE(EN_LOG_NOTICE, "CHalCmd::IdleEnable %d", bEnable);
    return true;
}

