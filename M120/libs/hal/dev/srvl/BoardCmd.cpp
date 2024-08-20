/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "BoardCmd.h"
#include "DevCmd.h"
#include "LedDriver.h"
#include "LogicSpi.h"
#include "RspHandler.h"
#include "DevThread.h"
#include "RegisterFile.h"
/*=============================variables=========================*/

std::shared_ptr<CBoard> CBoardCmd::ms_stpBoardPtr;
std::vector<std::shared_ptr<CLogicDriver> > CBoardCmd::ms_staLogicDrvPtrs;
std::vector<std::shared_ptr<CI2cSwitch> > CBoardCmd::ms_staI2cSwitchPtrs;
std::vector<std::shared_ptr<CUartSwitch> > CBoardCmd::ms_staUartSwitchPtrs;
std::vector<std::shared_ptr<CI2c> > CBoardCmd::ms_staI2cPtrs;
std::vector<std::shared_ptr<CUartDriver> > CBoardCmd::ms_staUartPtrs;

std::vector<std::shared_ptr<CResetTypeRegister> > CBoardCmd::ms_staResetTypeRegisters;
std::vector<std::shared_ptr<CApsd> > CBoardCmd::ms_staApsdPtrs;
std::vector<std::shared_ptr<CWatchDog> > CBoardCmd::ms_staWatchDogPtrs;
std::vector<std::shared_ptr<CResetBoard> > CBoardCmd::ms_staResetBoardPtrs;

std::vector<std::shared_ptr<CFanBoard> > CBoardCmd::ms_staFanBoardPtrs;
std::vector<std::shared_ptr<CPsuBoard> > CBoardCmd::ms_staPsuBoardPtrs;
std::vector<std::shared_ptr<CLedBoard> > CBoardCmd::ms_staLedBoardPtrs;
std::vector<std::shared_ptr<COscBoard> > CBoardCmd::ms_staOscBoardPtrs;
std::vector<std::shared_ptr<CSlotBoard> > CBoardCmd::ms_staSlotBoardPtrs;

std::vector<std::shared_ptr<CLogic> > CBoardCmd::ms_staLogicPtrs;

std::vector<std::shared_ptr<CFan> > CBoardCmd::ms_staFanPtrs;
std::vector<std::shared_ptr<CPsu> > CBoardCmd::ms_staPsuPtrs;
std::vector<std::shared_ptr<CLed> > CBoardCmd::ms_staLedPtrs;
std::vector<std::shared_ptr<CTemp> > CBoardCmd::ms_staTempPtrs;

std::vector<std::shared_ptr<CEEprom> > CBoardCmd::ms_staEEpromPtrs;
std::vector<std::shared_ptr<CFlash> > CBoardCmd::ms_staFlashPtrs;

std::vector<std::shared_ptr<CMdio> > CBoardCmd::ms_staMdioPtrs;
std::vector<std::shared_ptr<CNetSwitch> > CBoardCmd::ms_staNetSwitchPtrs;
std::vector<std::shared_ptr<CPhy> > CBoardCmd::ms_staPhyPtrs;
std::vector<std::shared_ptr<CLtc4215> > CBoardCmd::ms_staLtc4215Ptrs;
std::vector<std::shared_ptr<CMax1139> > CBoardCmd::ms_staMax1139Ptrs;
std::vector<std::shared_ptr<CMax31760> > CBoardCmd::ms_staMax31760Ptrs;
std::vector<std::shared_ptr<CMp5023> > CBoardCmd::ms_staMp5023Ptrs;

std::array<CShareMemory, SHM_TYPE_MAX> CBoardCmd::ms_staShm;
std::array<CSemaphore, SHM_TYPE_MAX> CBoardCmd::ms_staSem;

/*==============================function=========================*/
bool
CBoardCmd::GetChassisCfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetChassisCfg +++");
    if ((uiLen != sizeof(CCmdCfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCfgData *pstData = (CCmdCfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->GetCfg(EEPROM_CHASSIS, pstData->uiAddrSlice, pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "get chassis cfg error. Slice Addr = 0x%02X, FileName = %s", pstData->uiAddrSlice, pstData->acFilePath);
        LOG_WRITE(EN_LOG_ERROR, "CBoardCmd::GetChassisCfg error --- ");

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetChassisCfg ---");
    return true;
}

bool
CBoardCmd::SetChassisCfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisCfg +++");
    if ((uiLen != sizeof(CCmdCfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCfgData *pstData = (CCmdCfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetCfg(EEPROM_CHASSIS, pstData->uiAddrSlice, pstData->uiSize, pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set chassis cfg error. uiAddrSlice = %d, uiSize = %d, FileName = %s", pstData->uiAddrSlice, pstData->uiSize, pstData->acFilePath);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisCfg ---");
    return true;
}


bool
CBoardCmd::GetSccCfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetSccCfg +++");
    if ((uiLen != sizeof(CCmdCfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCfgData *pstData = (CCmdCfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->GetCfg(EEPROM_SCC, pstData->uiAddrSlice, pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "get chassis cfg error. Slice Addr = 0x%02X, FileName = %s", pstData->uiAddrSlice, pstData->acFilePath);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetSccCfg ---");
    return true;
}

bool
CBoardCmd::SetSccCfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetSccCfg +++");
    if ((uiLen != sizeof(CCmdCfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCfgData *pstData = (CCmdCfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetCfg(EEPROM_SCC, pstData->uiAddrSlice, pstData->uiSize, pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set chassis cfg error. uiAddrSlice = %d, FileName = %s", pstData->uiAddrSlice, pstData->acFilePath);
        LOG_WRITE(EN_LOG_ERROR, "CBoardCmd::SetChassisCfg error --- ");

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetSccCfg ---");
    return true;
}


bool
CBoardCmd::SetChassisMfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisMfg +++");

    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)c_pucBuf;
    if (0 == pstData->uiIdx) {
        if (!ms_stpBoardPtr->SetChassicMfg(pstData->acFilePath)) {
            LOG_WRITE(EN_LOG_ERROR, "set chassis mfg error. FileName = %s", pstData->acFilePath);
            return false;
        }
    } else if (1 == pstData->uiIdx) {
        /* for RPC MFG */
        if (!ms_stpBoardPtr->SetChassicMfg(pstData->acFilePath, pstData->uiIdx)) {
            LOG_WRITE(EN_LOG_ERROR, "set chassis mfg error. FileName = %s", pstData->acFilePath);
            return false;
        }        
    } else {
        LOG_WRITE(EN_LOG_ERROR, "set chassis mfg error. FileName = %s idx = %d", pstData->acFilePath, pstData->uiIdx);
    }


    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisMfg ---");

    return true;
}

bool
CBoardCmd::SetChassisConfigType(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisConfigType +++");

    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetChassicConfigType(pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set chassis mfg error. configtype = %s", pstData->acFilePath);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisConfigType ---");

    return true;
}

bool
CBoardCmd::SetChassisCali(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisCali +++");

    if ((uiLen != sizeof(CCmdCaliData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdCaliData *pstData = (CCmdCaliData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetCali(EEPROM_CHASSIS, pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set chassis cali error. FileName = %s", pstData->acFilePath);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetChassisCali ---");

    return true;
}


bool
CBoardCmd::GetUpgInfo(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetUpgInfo +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;
    uint8_t aucBuf[1024] = {0};

    memset(aucBuf, 0, sizeof(aucBuf));
    if (!ms_stpBoardPtr->GetUpgInfo(EEPROM_CHASSIS, aucBuf, pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "get upg info error.");
        return false;
    }

    for (int i = 0; i < 10; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "%d", aucBuf[i]);
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)aucBuf,
                      CShmOperator::GetDataUpgInfoPos(), pstData->iVal);

    memset(aucBuf, 0, sizeof(aucBuf));
    CShmOperator::Get(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)aucBuf,
                      CShmOperator::GetDataUpgInfoPos(), pstData->iVal);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetUpgInfo ---");

    return true;
}
bool
CBoardCmd::SetUpgInfo(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetUpgInfo +++");

    if ((uiLen != sizeof(CCmd1KBufData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmd1KBufData *pstData = (CCmd1KBufData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetUpgInfo(EEPROM_CHASSIS, pstData->aucBuf, pstData->uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "set upg info error. len = %d", pstData->uiLen);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetUpgInfo ---");

    return true;
}
bool
CBoardCmd::SetBdlHead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetBdlHead +++");

    if ((uiLen != sizeof(CCmd1KBufData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmd1KBufData *pstData = (CCmd1KBufData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetBdlHead(EEPROM_CHASSIS, pstData->aucBuf, pstData->uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "set bundle header error. len = %d", pstData->uiLen);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetBdlHead ---");

    return true;
}

bool
CBoardCmd::GetBdlHead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetBdlHead +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;
    uint8_t aucBuf[1024];

    memset(aucBuf, 0, sizeof(aucBuf));
    if (!ms_stpBoardPtr->GetBdlHead(EEPROM_CHASSIS, aucBuf, pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "get bundle header error.");
        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)aucBuf,
                      CShmOperator::GetDataBdlHeadPos(), pstData->iVal);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::GetBdlHead ---");

    return true;
}


bool
CBoardCmd::SetSccMfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetSccCfg +++");
    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetSccMfg(pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set scc mfg error. FileName = %s", pstData->acFilePath);
        LOG_WRITE(EN_LOG_ERROR, "CBoardCmd::SetSccMfg error --- ");

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SetSccMfg ---");
    return true;
}



bool
CBoardCmd::SetFanMfg(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staFanPtrs.size()) {
        return false;
    }

    if (!ms_staFanPtrs.at(pstData->uiIdx)->SetMfg(pstData->acFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "set fan #%d mfg error. FileName = %s", pstData->uiIdx, pstData->acFilePath);
        return false;
    }

    return true;
}



bool
CBoardCmd::SetLedState(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedState +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staLedPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staLedPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!ms_staLedPtrs.at(pstData->uiIdx)->SetState(pstData->iVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedState --");

    return true;

}

bool
CBoardCmd::SetLedTestMode(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedTestMode +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staLedPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staLedPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!ms_staLedPtrs.at(pstData->uiIdx)->SetTestMode(pstData->iVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedState --");

    return true;

}

bool
CBoardCmd::SetLedLocationTestMode(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedLocationTestMode +++");

    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staLedPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staLedPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!ms_staLedPtrs.at(pstData->uiIdx)->SetLocationTestMode(pstData->iVal)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLedState --");

    return true;

}

bool
CBoardCmd::SetFanSpeed(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan Speed +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staFanPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staFanPtrs.at(pstData->uiIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Speed(%d) +++", pstData->uiIdx, pstData->iVal);
    if (!ms_staFanPtrs.at(pstData->uiIdx)->SetSpeed(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set fan%d speed error. speed=%d", pstData->uiIdx, pstData->iVal);
        return false;
    }

    /* set data to shm */
    if (false == CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&(pstData->iVal),
                                   CShmOperator::GetDataFanCfgPos(pstData->uiIdx), sizeof(pstData->iVal))) {
        LOG_WRITE(EN_LOG_ERROR, "set fan%d speed to cfg error. speed=%d", pstData->uiIdx, pstData->iVal);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Set Fan Speed ---");

    return true;

}

bool
CBoardCmd::SetResetCauseInFan(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staFanPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staFanPtrs.at(pstData->uiIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Set reset cause in (%d) fan(%d) +++", (uint8_t)pstData->iVal, pstData->uiIdx);
    if (!ms_staFanPtrs.at(pstData->uiIdx)->SetResetCause(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Set reset cause in (%d) fan(%d) +++", (uint8_t)pstData->iVal, pstData->uiIdx);
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return true;

}

bool
CBoardCmd::SetFanControllerEEP(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "etFanControllerEEP +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staFanPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staFanPtrs.at(pstData->uiIdx)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) SetFanControllerEEP", pstData->uiIdx);
    if (!ms_staFanPtrs.at(pstData->uiIdx)->SetFanControllerEEP()) {
        LOG_WRITE(EN_LOG_ERROR, "set fan%d SetFanControllerEEP error", pstData->uiIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetFanControllerEEP ---");

    return true;

}


bool
CBoardCmd::SetFanLed(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan Speed +++");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (pstData->uiIdx >= ms_staFanPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staFanPtrs.at(pstData->uiIdx)) {
        return false;
    }
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "Set Fan(%d) Led(%d) +++", pstData->uiIdx, pstData->iVal);
    if (!ms_staFanPtrs.at(pstData->uiIdx)->SetLed(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "set fan%d speed error. Led=%d", pstData->uiIdx, pstData->iVal);
        return false;
    }
#endif
    /* set data to shm */
    if (false == CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&(pstData->iVal),
                                   CShmOperator::GetDataFanCfgPos(pstData->uiIdx), sizeof(pstData->iVal))) {
        LOG_WRITE(EN_LOG_ERROR, "set fan%d speed to cfg error. led=%d", pstData->uiIdx, pstData->iVal);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Set Fan Led ---");

    return true;

}


bool
CBoardCmd::LogicRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicRead +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->LogicRead(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "read logic error. idx = %d, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicRead ---");
    return true;
}


bool
CBoardCmd::LogicWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicWrite +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->LogicWrite(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "write logic error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicWrite ---");
    return true;
}

bool
CBoardCmd::LogicReadSlc(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicReadSlc +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->LogicReadSlc(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "read logic slc error. idx = %d, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicReadSlc ---");
    return true;
}

bool
CBoardCmd::LogicWriteSlc(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicWriteSlc +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->LogicWriteSlc(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "write logic slc error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::LogicWriteSlc ---");
    return true;
}

bool
CBoardCmd::SetResetType(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::SetResetType +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetResetType(pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "SetResetType error. val = %d", pstData->iVal);

        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::SetResetType ---");
    return true;
}


bool
CBoardCmd::CfgWatchDog(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::CfgWatchDog +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (!ms_stpBoardPtr->CfgWatchDog((bool)pstData->uiIdx, (uint8_t)pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "CfgWatchDog error. uiIdx = %d, val = %d", pstData->uiIdx, pstData->iVal);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::CfgWatchDog ---");
    return true;
}


bool
CBoardCmd::SetApsdEnable(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::SetApsdEnable +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    if (!ms_stpBoardPtr->SetApsdEnable(pstData->uiIdx, (bool)pstData->iVal)) {
        LOG_WRITE(EN_LOG_ERROR, "SetApsdEnable error. uiIdx = %u, val = %d", pstData->uiIdx, pstData->iVal);

        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::SetApsdEnable ---");
    return true;
}

bool
CBoardCmd::LogicReset(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = true;

    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::LogicReset ++ ");
    if ((uiLen != sizeof(CCmdData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)cv_pucBuf;

    if (pstData->uiIdx >= ms_staLogicPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staLogicPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!__SetShmStatus(SHM_STATUS_BUSY)) {
        return false;
    }

    if (!ms_staLogicPtrs.at(pstData->uiIdx)->Reset()) {
        bRet = false;
        goto cleanup;
    }

    CDelay::Delay(1, S);

    /* TBD: clear plug register */

    /* update mfginfo */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    if (ms_stpBoardPtr->GetSccMfg(stMfg)) {
        CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA),
                          (uint8_t *)&stMfg, CShmOperator::GetDataSccMfgPos(), sizeof(stMfg));

        LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::LogicReset -- ");
    }

    memset(&stMfg, 0, sizeof(stMfg));
    if (ms_stpBoardPtr->GetChassicMfg(stMfg)) {
        CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA),
                          (uint8_t *)&stMfg, CShmOperator::GetDataChassisMfgPos(), sizeof(stMfg));
        LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::LogicReset -- ");
    }

    bRet = true;

cleanup:

    __SetShmStatus(SHM_STATUS_FREE);
    return bRet;

}


bool
CBoardCmd::LogicUpgrade(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    bool bRet = true;

    printf("CBoardCmd::LogicUpgrade ++ \n");

    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::LogicUpgrade ++ ");

    if ((uiLen != sizeof(CCmdMfgData)) || (nullptr == cv_pucBuf)) {
        return false;
    }

    CCmdMfgData *pstData = (CCmdMfgData *)cv_pucBuf;

    if (pstData->uiIdx >= ms_staLogicPtrs.size()) {
        return false;
    }

    if (nullptr == ms_staLogicPtrs.at(pstData->uiIdx)) {
        return false;
    }

    if (!__SetShmStatus(SHM_STATUS_BUSY)) {
        return false;
    }
    /* CDevThread::SetLoopEnable(false); */
    /* CDevThread::SetLoopInterval(1000); */
    if (!ms_staLogicPtrs.at(pstData->uiIdx)->Upgrade(pstData->acFilePath)) {
        bRet = false;
        goto cleanup;
    }

cleanup:
    /* CDevThread::SetLoopInterval(20); */
    /* CDevThread::SetLoopEnable(true); */

    __SetShmStatus(SHM_STATUS_FREE);
    LOG_WRITE(EN_LOG_NOTICE, "CBoardCmd::LogicUpgrade ---");

    return bRet;

}

bool
CBoardCmd::SpiReadWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiReadWrite +++");

    uint8_t aucVal[256];
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    std::shared_ptr<CLogicSpi> stpSpiLogicPtr;
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;

    /* TBD: ms_staFpgaDrvPtrs.at(FPGA_MAIN) */
    stpSpiLogicPtr = std::make_shared<CLogicSpi>(ms_staLogicDrvPtrs.at(FPGA_MAIN));
    stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(stpSpiLogicPtr);

    memset(aucVal, 0, sizeof(aucVal));
    std::shared_ptr<CRspHandler> stpEndPos = std::make_shared<CRspHandler>(pstData->ulLen);
    if (!stpDrvPtr->ReadWrite(pstData->ulAddr, pstData->aucVal, 0, aucVal, stpEndPos)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi read write error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)aucVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiReadWrite ---");
    return true;
}


bool
CBoardCmd::MdioRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::MdioRead +++");

    if ((0 == ms_staMdioPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;


    uint16_t usVal = 0;

    if (!ms_staMdioPtrs.at(pstData->uiIdx)->Read(pstData->ulAddr, pstData->ulSubAddr, &usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio read error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&usVal,
                      CShmOperator::GetDataLogicStaPos(), sizeof(usVal));

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::MdioRead ---");
    return true;
}

bool
CBoardCmd::MdioWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::MdioWrite +++");

    if ((0 == ms_staMdioPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    uint16_t usVal = 0;
    memcpy(&usVal, pstData->aucVal, sizeof(usVal));

    if (!ms_staMdioPtrs.at(pstData->uiIdx)->Write(pstData->ulAddr, pstData->ulSubAddr, usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio write error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::MdioWrite ---");
    return true;
}

bool
CBoardCmd::NetSwitchRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::NetSwitchRead +++");

    if ((0 == ms_staNetSwitchPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    uint16_t usVal = 0;

    if (!ms_staNetSwitchPtrs.at(0)->ReadReg(pstData->ulAddr, pstData->ulSubAddr, &usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch read error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&usVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::NetSwitchRead ---");
    return true;
}

bool
CBoardCmd::NetSwitchWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::NetSwitchWrite +++");

    if ((0 == ms_staNetSwitchPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    uint16_t usVal = 0;
    memcpy(&usVal, pstData->aucVal, sizeof(usVal));

    if (!ms_staNetSwitchPtrs.at(0)->WriteReg(pstData->ulAddr, pstData->ulSubAddr, usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch write error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::NetSwitchWrite ---");
    return true;
}

bool
CBoardCmd::SwitchPhyRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SwitchPhyRead +++");

    if ((0 == ms_staNetSwitchPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    uint16_t usVal = 0;

    if (!ms_staNetSwitchPtrs.at(0)->ReadSMIC45PhyReg(0x04, pstData->ulAddr, pstData->ulSubAddr, &usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch phy read error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&usVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SwitchPhyRead ---");
    return true;
}

bool
CBoardCmd::SwitchPhyWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SwitchPhyWrite +++");

    if ((0 == ms_staNetSwitchPtrs.size()) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    uint16_t usVal = 0;
    memcpy(&usVal, pstData->aucVal, sizeof(usVal));

    if (!ms_staNetSwitchPtrs.at(0)->WriteSMIC45PhyReg(0x04, pstData->ulAddr, pstData->ulSubAddr, usVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Net switch phy write error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SwitchPhyWrite ---");
    return true;
}


bool
CBoardCmd::SpiFlashRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiFlashRead +++");

    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    int iIdx = 0;
#if 0
    if (SPI_MAIN_FLASH == pstData->ulAddr) {
        iIdx = FPGA_MAIN;
    } else if (SPI_CTRL_FLASH == pstData->ulAddr) {
        iIdx = FPGA_CTRL;
    } else {
        return false;
    }
#endif
    if (!ms_staFlashPtrs.at(iIdx)->ReadData(pstData->ulSubAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi Flash read error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    for (int i = 0; i < (int)pstData->ulLen; i++) {
        /* LOG_WRITE(EN_LOG_NOTICE, "Val[%d] = %02X", i, pstData->aucVal[i]); */
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiFlashRead ---");
    return true;
}

bool
CBoardCmd::SpiFlashWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiFlashWrite +++");

    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    int iIdx = 0;
#if 0
    if (SPI_MAIN_FLASH == pstData->ulAddr) {
        iIdx = FPGA_MAIN;
    } else if (SPI_CTRL_FLASH == pstData->ulAddr) {
        iIdx = FPGA_CTRL;
    } else {
        return false;
    }
#endif
    if (!ms_staFlashPtrs.at(iIdx)->WriteData(pstData->ulSubAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Spi flash write error. Addr = 0x%lX, len = %lu", pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::SpiFlashWrite ---");
    return true;
}


bool
CBoardCmd::EepromRead(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::EepromRead +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->EepromRead(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "read Eeprom error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)pstData->aucVal,
                      CShmOperator::GetDataLogicStaPos(), pstData->ulLen);

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::EepromRead ---");
    return true;
}


bool
CBoardCmd::EepromWrite(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::EepromWrite +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdLogicData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdLogicData *pstData = (CCmdLogicData *)c_pucBuf;

    if (!ms_stpBoardPtr->EepromWrite(pstData->uiIdx, pstData->ulAddr, pstData->aucVal, pstData->ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "write Eeprom error. idx = %u, Addr = 0x%lX, len = %lu", pstData->uiIdx, pstData->ulAddr, pstData->ulLen);

        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CBoardCmd::EepromWrite ---");
    return true;
}

bool
CBoardCmd::I2cTest(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    CCmdKeyData *pstData = (CCmdKeyData *)c_pucBuf;

    ms_stpBoardPtr->I2cTest((uint32_t)pstData->iKey, (uint32_t)pstData->iVal);

    return true;
}


bool
CBoardCmd::__SetShmStatus(int32_t iStatus)
{
    CShmDevSta stSta;
    CShmOperator::Get(ms_staShm.at(SHM_TYPE_STA), ms_staSem.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    if (iStatus == stSta.iUpgdSta) {
        LOG_WRITE(EN_LOG_ERROR, "stSta.iUpgdSta = %d", stSta.iUpgdSta);
        return false;
    }

    stSta.iUpgdSta = iStatus;
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_STA), ms_staSem.at(SHM_TYPE_STA),
                      (uint8_t *)&stSta, CShmOperator::GetStaDevStaPos(), sizeof(stSta));

    return true;
}

bool
CBoardCmd::SetLogLevel(uint32_t uiLen, const uint8_t *c_pucBuf)
{
    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLogLevel +++");
    if ((nullptr == ms_stpBoardPtr) || (uiLen != sizeof(CCmdData)) || (nullptr == c_pucBuf)) {
        return false;
    }

    CCmdData *pstData = (CCmdData *)c_pucBuf;

    UTIL_SetClientLogSeverity((EN_UTIL_LOG_SEVERITY)pstData->iVal);

    LOG_WRITE(EN_LOG_TRACE, "CBoardCmd::SetLogLevel ---");
    return true;
}

bool
CBoardCmd::GetDevType(uint32_t uiLen, const uint8_t *cv_pucBuf)
{
    if (uiLen == 0 || nullptr == cv_pucBuf) {
        return false;
    }

    uint32_t type = CRegisterFile::ConvertDevType((const char *)cv_pucBuf);

    /* set data to shm */
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA),
                      (uint8_t *)&type, CShmOperator::GetDataFuncDevType(), sizeof(uint32_t));

    return true;
}
