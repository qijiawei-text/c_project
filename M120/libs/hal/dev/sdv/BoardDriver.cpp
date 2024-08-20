/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "BoardDriver.h"
#include "BoardCmd.h"
#include "TempDriver.h"
#include "FanDriver.h"
#include "EEprom.h"
#include "DevData.h"
#include "EEpromOperator.h"
#include "PsuDriver.h"
#include "LedDriver.h"
#include "File.h"
#include "Jtag.h"
#include "Ltc4215Driver.h"
#include "Mp5023Driver.h"
#include "Max1139Driver.h"
/*==============================function=========================*/

bool
CBoard::GetMfg(CBoardMfg &rstMfg)
{
#define RETRY_TIMES 3

    uint32_t uiRetryTimes = 0;
    std::shared_ptr<CFan> stpFanDrvPtr;
    std::shared_ptr<CPsu> stpPsuDrvPtr;

    LOG_WRITE(EN_LOG_NOTICE, "CBoard::GetMfg +++");

#if __USB_RELAOD_DEBUG__
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);
#endif

    for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
        if (GetChassicMfg(rstMfg.stChassis)) {
            rstMfg.bChassisMfgOk = true;
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "get chassis mfg error.");
#if __USB_RELAOD_DEBUG__
            ofile << "get chassis mfg error." << std::endl;
#endif
            CDelay::Delay(10, MS);
        }
    }

    for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
        if (GetSccMfg(rstMfg.stScc)) {
            rstMfg.bSccMfgOk = true;
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "get scc mfg error.");
#if __USB_RELAOD_DEBUG__
            ofile << "get scc mfg error." << std::endl;
#endif
            CDelay::Delay(10, MS);
        }
    }

    for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
        if (GetRpcMfg(rstMfg.stRpc)) {
            rstMfg.bRpcMfgOk = true;
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "get rpc mfg error.");
#if __USB_RELAOD_DEBUG__
            ofile << "get rpc mfg error." << std::endl;
#endif
            CDelay::Delay(10, MS);
        }
    }

    uint32_t uiFanIdx = 0;
    stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);

    while (nullptr != stpFanDrvPtr) {
        if (stpFanDrvPtr->IsOnline()) {
            for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
                if (stpFanDrvPtr->GetMfg(rstMfg.astFan[uiFanIdx])) {
                    rstMfg.abFanMfgOk[uiFanIdx] = true;
                    break;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "get fan #%d mfg error.", uiFanIdx);
#if __USB_RELAOD_DEBUG__
                    ofile << "get fan #" << uiFanIdx << " mfg error." << std::endl;
#endif
                    CDelay::Delay(10, MS);
                }
            }
        }

        uiFanIdx++;
        stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);
    }

    uint32_t uiPsuIdx = 0;
    stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);

    while (nullptr != stpPsuDrvPtr) {
        if (stpPsuDrvPtr->IsOnline() && stpPsuDrvPtr->IsOk()) {
            for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
                if (stpPsuDrvPtr->GetMfg(rstMfg.astPower[uiPsuIdx])) {
                    rstMfg.abPowerMfgOk[uiPsuIdx] = true;
                    break;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "get PSU #%d mfg error.", uiPsuIdx);
#if __USB_RELAOD_DEBUG__
                    ofile << "get psu #" << uiPsuIdx << " mfg error." << std::endl;
#endif
                    CDelay::Delay(10, MS);
                }
            }
        }

        uiPsuIdx++;
        stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);
    }

#if __USB_RELAOD_DEBUG__
    ofile.close();
#endif

    LOG_WRITE(EN_LOG_NOTICE, "CBoard::GetMfg --");

    return true;
}

bool
CBoard::GetStatusData(CBoardStatusData &rstData)
{
    std::shared_ptr<CTemp> stpTempDrvPtr;
    std::shared_ptr<CFan> stpFanDrvPtr;
    std::shared_ptr<CPsu> stpPsuDrvPtr;
    std::shared_ptr<CLed> stpLedDrvPtr;
    std::shared_ptr<CResetTypeRegister> stpResetTypeResgister;
    std::shared_ptr<CApsd> stpApsdPtr;
    std::shared_ptr<CLtc4215> stpLtc4215DrvPtr;
    std::shared_ptr<CMax1139> stpMax1139DrvPtr;
    std::shared_ptr<CMp5023> stpMp5023DrvPtr;

    /* every (500 * 20)ms times get temp */
    uint32_t uiTempIdx = 0;
    stpTempDrvPtr = CBoardCmd::GetTempPtr(uiTempIdx);

    while (nullptr != stpTempDrvPtr) {
        /* loop time is modified from 20ms to 200ms, also need change mod num from 500 to 50 */
        /* still every (50 * 200)ms get temp */
        if (0 == (m_ullCnt % 50)) {
            if (true == stpTempDrvPtr->GetTemp(rstData.aiTemp[uiTempIdx])) {
                m_aiTemp[uiTempIdx] = rstData.aiTemp[uiTempIdx];
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get temp(%d) error.", uiTempIdx);
                /* printf("get temp(%d) error.", uiTempIdx); */
            }
        } else {
            rstData.aiTemp[uiTempIdx] = m_aiTemp[uiTempIdx];
        }

        uiTempIdx++;
        stpTempDrvPtr = CBoardCmd::GetTempPtr(uiTempIdx);
    }


    uint32_t uiFanIdx = 0;
    stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);

    while (nullptr != stpFanDrvPtr) {
        if (false == stpFanDrvPtr->GetStatusData(rstData.astFan[uiFanIdx])) {
            LOG_WRITE(EN_LOG_ERROR, "get fan #%d status data error.", uiFanIdx);
        }

        uiFanIdx++;
        stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);
    }


    uint32_t uiPsuIdx = 0;
    stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);

    /* psu status */
    while (nullptr != stpPsuDrvPtr) {
        stpPsuDrvPtr->GetStatusData(rstData.astPower[uiPsuIdx]);

        uiPsuIdx++;
        stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);
    }

    m_ullCnt++;


    uint32_t uiLedIdx = 0;
    stpLedDrvPtr = CBoardCmd::GetLedPtr(uiLedIdx);

    while (nullptr != stpLedDrvPtr) {
        if (false == stpLedDrvPtr->GetState(rstData.astLed[uiLedIdx].uiState)) {
            LOG_WRITE(EN_LOG_ERROR, "get led #%d status data error.", uiLedIdx);
        }

        uiLedIdx++;
        stpLedDrvPtr = CBoardCmd::GetLedPtr(uiLedIdx);
    }

    /* get fpga function */
    uint32_t uiResetTpyeIdx = 0;
    stpResetTypeResgister = CBoardCmd::GetResetTypeRegister(uiResetTpyeIdx);

    while (nullptr != stpResetTypeResgister) {
        if (false == stpResetTypeResgister->GetResetType(rstData.aucResetType[uiResetTpyeIdx])) {
            LOG_WRITE(EN_LOG_ERROR, "get reset type error. uiResetTpyeIdx = %d", uiResetTpyeIdx);
        }

        uiResetTpyeIdx++;
        stpResetTypeResgister = CBoardCmd::GetResetTypeRegister(uiResetTpyeIdx);
    }
#if 0
    uint32_t uiApsdIdx = 0;
    stpApsdPtr = CBoardCmd::GetApsdPtr(uiApsdIdx);

    while (nullptr != stpApsdPtr) {
        if (false == stpApsdPtr->GetApsdStatus(uiApsdIdx, rstData.abApsdEnable[uiApsdIdx], rstData.abApsdStatus[uiApsdIdx], rstData.abApsdEvent[uiApsdIdx])) {
            LOG_WRITE(EN_LOG_ERROR, "get apsd status error. uiApsdIdx = %d", uiApsdIdx);
        }

        uiApsdIdx++;
        stpApsdPtr = CBoardCmd::GetApsdPtr(uiApsdIdx);
    }
    uint32_t uiLtc4215Idx = 0;
    stpLtc4215DrvPtr = CBoardCmd::GetLtc4215Ptr(uiLtc4215Idx);
    while (nullptr != stpLtc4215DrvPtr) { /*if(nullptr != stpLtc4215DrvPtr) {//while (nullptr != stpLtc4215DrvPtr) { */
        if (0 == (m_ullCnt % 500)) {
            if (true == stpLtc4215DrvPtr->GetSenseCode(rstData.astLtc4215[uiLtc4215Idx].iSense)) {
                m_aiLtc4215Sense[uiLtc4215Idx] = rstData.astLtc4215[uiLtc4215Idx].iSense;
                LOG_WRITE(EN_LOG_DEBUG, "get ltc4215(%d) succ,m_aiLtc4215Sense:%d", uiLtc4215Idx, rstData.astLtc4215[uiLtc4215Idx].iSense);
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get m_aiLtc4215Sense(%d) error.", uiLtc4215Idx);
            }

            if (true == stpLtc4215DrvPtr->GetSourceCode(rstData.astLtc4215[uiLtc4215Idx].iSource)) {
                m_aiLtc4215Source[uiLtc4215Idx] = rstData.astLtc4215[uiLtc4215Idx].iSource;
                LOG_WRITE(EN_LOG_DEBUG, "get ltc4215(%d) succ,aiLtc4215Source:%d", uiLtc4215Idx, rstData.astLtc4215[uiLtc4215Idx].iSource);
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get m_aiLtc4215Source(%d) error.", uiLtc4215Idx);
            }
        } else {
            rstData.astLtc4215[uiLtc4215Idx].iSense = m_aiLtc4215Sense[uiLtc4215Idx];
            rstData.astLtc4215[uiLtc4215Idx].iSource = m_aiLtc4215Source[uiLtc4215Idx];
        }

        uiLtc4215Idx++;
        stpLtc4215DrvPtr = CBoardCmd::GetLtc4215Ptr(uiLtc4215Idx);
    }

    uint32_t uiMp5023Idx = 0;
    stpMp5023DrvPtr = CBoardCmd::GetMp5023Ptr(uiMp5023Idx);
    while (nullptr != stpMp5023DrvPtr) {
        if (0 == (m_ullCnt % 500)) {
            if (true == stpMp5023DrvPtr->GetStatus(rstData.astMp5023[uiMp5023Idx])) {
                memcpy(&m_astMp5023[uiMp5023Idx], &rstData.astMp5023[uiMp5023Idx], sizeof(m_astMp5023[uiMp5023Idx]));
                LOG_WRITE(EN_LOG_NOTICE, "get Mp5023(%d) succ, (%.2f), (%.2f), (%.2f)", uiMp5023Idx, rstData.astMp5023[uiMp5023Idx].dVIn, rstData.astMp5023[uiMp5023Idx].dVOut, rstData.astMp5023[uiMp5023Idx].dIOut);
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get Mp5023(%d) error.", uiMp5023Idx);
            }
        } else {
            memcpy(&rstData.astMp5023[uiMp5023Idx], &m_astMp5023[uiMp5023Idx], sizeof(rstData.astMp5023[uiMp5023Idx]));
        }

        uiMp5023Idx++;
        stpMp5023DrvPtr = CBoardCmd::GetMp5023Ptr(uiMp5023Idx);
    }
#endif
    uint32_t uiMax1139Idx = 0;
    stpMax1139DrvPtr = CBoardCmd::GetMax1139Ptr(uiMax1139Idx);
    while (nullptr != stpMax1139DrvPtr) {
        if (0 == (m_ullCnt % 500)) {
            if (true == stpMax1139DrvPtr->GetVoltage(rstData.astMax1139[uiMax1139Idx].adVoltage, 12)) {
                memcpy(m_adVoltage[uiMax1139Idx], rstData.astMax1139[uiMax1139Idx].adVoltage, 12 * sizeof(float));
                LOG_WRITE(EN_LOG_DEBUG, "get Max1139(%u) succ,m_dVoltage[0]:%.2f", uiMax1139Idx, m_adVoltage[uiMax1139Idx][0]);
            } else {
                LOG_WRITE(EN_LOG_ERROR, "get m_aiMax1139Sense(%u) error.", uiMax1139Idx);
            }

        } else {
            memcpy(rstData.astMax1139[uiMax1139Idx].adVoltage, m_adVoltage[uiMax1139Idx], 12 * sizeof(float));
        }

        uiMax1139Idx++;
        stpMax1139DrvPtr = CBoardCmd::GetMax1139Ptr(uiMax1139Idx);
    }

    return true;
}

bool
CBoard::GetChassicMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetChassicMfg +++");
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    memset(&rstMfg, 0, sizeof(rstMfg));

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_CHASSIS);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", EEPROM_CHASSIS);
        return false;
    }

    if (false == CEEpromOperator::ReadMfg(stpEEpromPtr, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) mfg error.", EEPROM_CHASSIS);
        return false;
    }

    GetLogicVersion(0, rstMfg.acFpga1Ver);
    GetLogicVersion(0, rstMfg.acFpga2Ver);

    GetLogicVersion(1, rstMfg.acCpld1Ver);
    GetLogicVersion(1, rstMfg.acCpld2Ver);

    /* If NOT find the key, just set default value */
    if (0 == strlen(rstMfg.acConfigType)) {
#if defined(__OCS__)
        strncpy(rstMfg.acConfigType, "OCS", sizeof(rstMfg.acConfigType) - 1);
#elif defined(__NR1004__)
        strncpy(rstMfg.acConfigType, "AMP", sizeof(rstMfg.acConfigType) - 1);
#endif
        LOG_WRITE(EN_LOG_NOTICE, "Using default config type '%s'", rstMfg.acConfigType);
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFpga1Ver %s", rstMfg.acFpga1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acCpld1Ver %s", rstMfg.acCpld1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acCpld2Ver %s", rstMfg.acCpld2Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "acEth0Mac %s", rstMfg.acEth0Mac);
    LOG_WRITE(EN_LOG_NOTICE, "acEth1Mac %s", rstMfg.acEth1Mac);
    LOG_WRITE(EN_LOG_NOTICE, "acEth2Mac %s", rstMfg.acEth2Mac);
    LOG_WRITE(EN_LOG_NOTICE, "acEth3Mac %s", rstMfg.acEth3Mac);
    LOG_WRITE(EN_LOG_NOTICE, "acEth4Mac %s", rstMfg.acEth4Mac);
    LOG_WRITE(EN_LOG_NOTICE, "acEth5Mac %s", rstMfg.acEth5Mac);
    LOG_WRITE(EN_LOG_NOTICE, "GetChassicMfg ---");

    return true;
}

bool
CBoard::GetSccMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetSccMfg +++");
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;
    std::string staFpgaVer;

    memset(&rstMfg, 0, sizeof(rstMfg));
    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_SCC);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", EEPROM_SCC);
        return false;
    }

    if (false == CEEpromOperator::ReadMfg(stpEEpromPtr, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) mfg error.", EEPROM_SCC);
        return false;
    }

    GetLogicVersion(0, rstMfg.acFpga1Ver);
    GetLogicVersion(0, rstMfg.acFpga2Ver);

    GetLogicVersion(1, rstMfg.acCpld1Ver);
    GetLogicVersion(1, rstMfg.acCpld2Ver);

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFpga1Ver %s", rstMfg.acFpga1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acCpld1Ver %s", rstMfg.acCpld1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

    LOG_WRITE(EN_LOG_NOTICE, "GetSccMfg ---");

    return true;
}

bool
CBoard::GetRpcMfg(CMfg &rstMfg)
{
    uint32_t uiIdx = 1;
    LOG_WRITE(EN_LOG_NOTICE, "GetRpcMfg +++");
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    memset(&rstMfg, 0, sizeof(rstMfg));

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_CHASSIS);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", EEPROM_CHASSIS);
        return false;
    }

    if (false == CEEpromOperator::ReadMfg(stpEEpromPtr, rstMfg, uiIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) mfg error.", EEPROM_CHASSIS);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acProductName %s", rstMfg.acProductName);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acPcbRev %s", rstMfg.acPcbRev);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCleiCode %s", rstMfg.acCleiCode);

    LOG_WRITE(EN_LOG_NOTICE, "GetRpcMfg ---");

    return true;
}

bool
CBoard::GetLogicVersion(uint32_t uiIdx, char *pcVersion)
{
    std::shared_ptr<CLogic> stpLogicDrv = CBoardCmd::GetLogicPtr(uiIdx);
    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    stpLogicDrv->GetVersion(pcVersion);

    return true;
}

bool
CBoard::SetChassicMfg(const char *c_pcFileName)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    if (NULL == c_pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFileName is NULL.");
    }

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_CHASSIS);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get EEprom driver error. uiEEpromIdx = %d", EEPROM_CHASSIS);
        return false;
    }

    if ( false == CEEpromOperator::WriteMfg(stpEEpromPtr, c_pcFileName)) {
        LOG_WRITE(EN_LOG_ERROR, "Write EEprom(%d) mfg error.", EEPROM_CHASSIS);
        return false;
    }

    return true;
}

bool
CBoard::SetChassicMfg(const char *c_pcFileName, uint32_t uiIdx)
{
    /* for RPC MFG */
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    if (NULL == c_pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFileName is NULL.");
    }

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_CHASSIS);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get EEprom driver error. uiEEpromIdx = %d", EEPROM_CHASSIS);
        return false;
    }

    if ( false == CEEpromOperator::WriteMfg(stpEEpromPtr, c_pcFileName, uiIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Write EEprom(%d) mfg error when write rpc mfg.", EEPROM_CHASSIS);
        return false;
    }

    return true;
}

bool
CBoard::SetChassicConfigType(const char *c_pcConfigType)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    if (NULL == c_pcConfigType) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFileName is NULL.");
    }

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_CHASSIS);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get EEprom driver error. uiEEpromIdx = %d", EEPROM_CHASSIS);
        return false;
    }

    if ( false == CEEpromOperator::WriteMfgConfigType(stpEEpromPtr, c_pcConfigType)) {
        LOG_WRITE(EN_LOG_ERROR, "Write EEprom(%d) mfg error.", EEPROM_CHASSIS);
        return false;
    }

    return true;
}

bool
CBoard::SetSccMfg(const char *c_pcFileName)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    if (NULL == c_pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFileName is NULL.");
    }

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(EEPROM_SCC);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get EEprom driver error. uiEEpromIdx = %d", EEPROM_SCC);
        return false;
    }

    if ( false == CEEpromOperator::WriteMfg(stpEEpromPtr, c_pcFileName)) {
        LOG_WRITE(EN_LOG_ERROR, "Write EEprom(%d) mfg error.", EEPROM_SCC);
        return false;
    }

    return true;
}

bool
CBoard::GetCfg(uint32_t uiIdx, uint32_t uiAddrSlice, const char *c_pcFileName)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;
    int iResult = OPLK_ERR;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        iResult = CEEpromOperator::ReadCfg(stpEEpromPtr, uiAddrSlice, c_pcFileName);
        if (OPLK_OK == iResult) {
            return true;
        } else if (ERR_OPERATION_FAILED == iResult) {
            CDelay::Delay(10, MS);
            continue;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "read EEprom(%d) cfg error. uiAddrSlice = %d , c_pcFileName = %s", uiIdx, uiAddrSlice, c_pcFileName);
            return false;
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "read EEprom(%d) cfg error. uiAddrSlice = %d , c_pcFileName = %s", uiIdx, uiAddrSlice, c_pcFileName);
    return false;
}

bool
CBoard::SetCfg(uint32_t uiIdx, uint32_t uiAddrSlice, uint32_t uiSize, const char *c_pcFileName)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;
    int iResult = OPLK_ERR;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        iResult = CEEpromOperator::WriteCfg(stpEEpromPtr, uiAddrSlice, uiSize, c_pcFileName);
        if (OPLK_OK == iResult) {
            return true;
        } else if (ERR_OPERATION_FAILED == iResult) {
            CDelay::Delay(10, MS);
            continue;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "write EEprom(%d) cfg error. uiAddrSlice = %d , uiSize = %u, c_pcFileName = %s", uiIdx, uiAddrSlice, uiSize, c_pcFileName);
            return false;
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "write EEprom(%d) cfg error. uiAddrSlice = %d , uiSize = %u, c_pcFileName = %s", uiIdx, uiAddrSlice, uiSize, c_pcFileName);
    return false;
}



bool
CBoard::GetCali(uint32_t uiIdx, CCali &rstCali)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::ReadComCali(stpEEpromPtr, rstCali)) {
            LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) cali error.", uiIdx);
            CDelay::Delay(10, MS);
            continue;
        }

        return true;
    }

    return false;
}

bool
CBoard::SetCali(uint32_t uiIdx, const char *c_pcFileName)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::WriteComCali(stpEEpromPtr, c_pcFileName)) {
            LOG_WRITE(EN_LOG_ERROR, "write EEprom(%d) cali error. c_pcFileName = %s", uiIdx, c_pcFileName);
            CDelay::Delay(10, MS);
            continue;
        }
        return true;
    }

    return false;
}


bool
CBoard::GetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::ReadUpgInfo(stpEEpromPtr, pucBuf, ulLen)) {
            LOG_WRITE(EN_LOG_ERROR, "read EEprom(%d) upg info error. len = %d", uiIdx, ulLen);
            CDelay::Delay(10, MS);
            continue;
        }

        return true;
    }

    return false;
}
bool
CBoard::SetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::WriteUpgInfo(stpEEpromPtr, pucBuf, ulLen)) {
            LOG_WRITE(EN_LOG_ERROR, "write EEprom(%d) upg info error. len = %d", uiIdx, ulLen);
            CDelay::Delay(10, MS);
            continue;
        }

        return true;
    }

    return false;
}


bool
CBoard::GetBdlHead(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::ReadBdlHead(stpEEpromPtr, pucBuf, ulLen)) {
            LOG_WRITE(EN_LOG_ERROR, "read EEprom(%d) bundle head error. len = %d", uiIdx, ulLen);
            CDelay::Delay(10, MS);
            continue;
        }

        return true;
    }

    return false;
}
bool
CBoard::SetBdlHead(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
        if ( false == CEEpromOperator::WriteBdlHead(stpEEpromPtr, pucBuf, ulLen)) {
            LOG_WRITE(EN_LOG_ERROR, "write EEprom(%d) bundle head error. len = %d", uiIdx, ulLen);
            CDelay::Delay(10, MS);
            continue;
        }

        return true;
    }

    return false;
}


bool
CBoard::GetTemp(uint32_t uiIdx, int32_t &riTemp)
{
    std::shared_ptr<CTemp> stpTempDrvPtr;
    stpTempDrvPtr = CBoardCmd::GetTempPtr(uiIdx);

    if (nullptr == stpTempDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpTempDrvPtr is nullptr");
        return false;
    }

    if (!stpTempDrvPtr->GetTemp(riTemp)) {
        LOG_WRITE(EN_LOG_ERROR, "get temperature error. uiTempIdx = %d", uiIdx);
        return false;
    }

    return true;
}

bool
CBoard::GetLtc4215(uint32_t uiIdx, int32_t &riLtc4215Sense, int32_t &riLtc4215Source)
{
    std::shared_ptr<CLtc4215> stpLtc4215DrvPtr;
    stpLtc4215DrvPtr = CBoardCmd::GetLtc4215Ptr(uiIdx);

    if (nullptr == stpLtc4215DrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpLtc4215DrvPtr is nullptr");
        return false;
    }


    if (true == stpLtc4215DrvPtr->GetSenseCode(riLtc4215Sense)) {
        LOG_WRITE(EN_LOG_DEBUG, "get ltc4215(%d) succ,m_aiLtc4215Sense:%d", uiIdx, riLtc4215Sense);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get m_aiLtc4215Sense(%d) error.", uiIdx);
    }

    if (true == stpLtc4215DrvPtr->GetSourceCode(riLtc4215Source)) {
        LOG_WRITE(EN_LOG_DEBUG, "get ltc4215(%d) succ,aiLtc4215Source:%d", uiIdx, riLtc4215Source);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get m_aiLtc4215Source(%d) error.", uiIdx);
    }

    return true;
}

bool
CBoard::GetMp5023(uint32_t uiIdx, CMp5023Sta &rstMp5023Sta)
{
    std::shared_ptr<CMp5023> stpMp5023DrvPtr;
    stpMp5023DrvPtr = CBoardCmd::GetMp5023Ptr(uiIdx);

    if (nullptr == stpMp5023DrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpMp5023rvPtr is nullptr");
        return false;
    }

    if (true == stpMp5023DrvPtr->GetStatus(rstMp5023Sta)) {
        LOG_WRITE(EN_LOG_DEBUG, "get mp5023(%d) status succ, Vin:%f, Vout:%f, Iout:%f,", uiIdx, rstMp5023Sta.dVIn, rstMp5023Sta.dVOut, rstMp5023Sta.dIOut);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "get mp5023(%d) status error.", uiIdx);
    }

    return true;
}

bool
CBoard::SetLedState(uint32_t uiIdx, uint32_t uiState)
{
    std::shared_ptr<CLed> ptr = CBoardCmd::GetLedPtr(uiIdx);
    if (nullptr == ptr) {
        LOG_WRITE(EN_LOG_ERROR, "None of led dirver found, uiIdx = %u", uiIdx);
        return false;
    }

    if (!ptr->SetState(uiState)) {
        LOG_WRITE(EN_LOG_ERROR, "Set board led state error. uiIdx = %u, uiState = %u", uiIdx, uiState);
        return false;
    }

    return true;
}

bool
CBoard::LogicRead(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    std::shared_ptr<CLogicDriver> stpLogicDrv = CBoardCmd::GetLogicDrvPtr(uiIdx);
    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    if (false == stpLogicDrv->Read(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read register error. %d", uiAddr);
        return false;
    }
    return true;
}

bool
CBoard::LogicWrite(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    std::shared_ptr<CLogicDriver> stpLogicDrv = CBoardCmd::GetLogicDrvPtr(uiIdx);

    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    if (false == stpLogicDrv->Write(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write register error. %d", uiAddr);
        return false;
    }

    return true;
}

bool
CBoard::LogicReadSlc(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;

    if (uiIdx != 1 && uiIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", uiIdx);
        return false;
    }
    std::shared_ptr<CLogicDriver> stpLogicDrv = CBoardCmd::GetLogicDrvPtr(0);
    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    pthread_mutex_lock(&m_spi_mutex[uiIdx - 1]);
#if 0
    stpLogicDrv->ReadReg(SLC_SPI_SELECT, &ucDataW);
    ucDataW &= ~(1 << (uiIdx - 1));
    stpLogicDrv->WriteReg(SLC_SPI_SELECT, ucDataW);
#else
    stpLogicDrv->WriteReg(SLC_SPI_SELECT + uiIdx - 1, ucDataW);
#endif

    if (false == stpLogicDrv->Read(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read register error. %d", uiAddr);
        pthread_mutex_unlock(&m_spi_mutex[uiIdx - 1]);
        return false;
    }
    pthread_mutex_unlock(&m_spi_mutex[uiIdx - 1]);

    return true;
}

bool
CBoard::LogicWriteSlc(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t ucDataW = 0;

    if (uiIdx != 1 && uiIdx != 2 ) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d", uiIdx);
        return false;
    }
    std::shared_ptr<CLogicDriver> stpLogicDrv = CBoardCmd::GetLogicDrvPtr(0);
    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    pthread_mutex_lock(&m_spi_mutex[uiIdx - 1]);
#if 0
    stpLogicDrv->ReadReg(SLC_SPI_SELECT, &ucDataW);
    ucDataW &= ~(1 << (uiIdx - 1));
    stpLogicDrv->WriteReg(SLC_SPI_SELECT, ucDataW);
#else
    stpLogicDrv->WriteReg(SLC_SPI_SELECT + uiIdx - 1, ucDataW);
#endif
    if (false == stpLogicDrv->Write(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write register error. %d", uiAddr);
        pthread_mutex_unlock(&m_spi_mutex[uiIdx - 1]);
        return false;
    }
    pthread_mutex_unlock(&m_spi_mutex[uiIdx - 1]);

    return true;
}

bool
CBoard::SetResetType(uint32_t uiType)
{
    std::shared_ptr<CResetTypeRegister> stpResetTypeRegister = CBoardCmd::GetResetTypeRegister(0);

    if (nullptr == stpResetTypeRegister) {
        LOG_WRITE(EN_LOG_ERROR, "stpResetTypeRegister is nullptr");
        return false;
    }

    if (false == stpResetTypeRegister->SetResetType(uiType)) {
        LOG_WRITE(EN_LOG_ERROR, "Set reset type %d error. ", uiType);
        return false;
    }

    return true;
}

bool
CBoard::CfgWatchDog(bool bEnable, uint8_t ucTimeout)
{
    std::shared_ptr<CWatchDog> stpWatchDogPtr = CBoardCmd::GetWatchDogPtr(0);
    if (nullptr == stpWatchDogPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpWatchDogPtr is nullptr");
        return false;
    }

    if (false == stpWatchDogPtr->Config(bEnable, ucTimeout)) {
        LOG_WRITE(EN_LOG_ERROR, "Config watch dog error. %d %d", bEnable, ucTimeout);
        return false;
    }

    return true;
}

bool
CBoard::SetApsdEnable(uint32_t uiIdx, bool bEnable)
{
    std::shared_ptr<CApsd> stpApsdPtr = CBoardCmd::GetApsdPtr(uiIdx);

    if (nullptr == stpApsdPtr) {

        LOG_WRITE(EN_LOG_ERROR, "stpApsdPtr is nullptr. uiIdx = %d", uiIdx);
        return false;
    }

    if (false == stpApsdPtr->SetApsdEnable(uiIdx, bEnable)) {
        LOG_WRITE(EN_LOG_ERROR, "Set apsd %u enable %d error", uiIdx, bEnable);
        return false;
    }

    return true;
}


bool
CBoard::LogicReset(uint8_t ucResetType, uint32_t uiSubIdx)
{
#if 0
    if (FPGA_MAIN == uiSubIdx) {
        if (nullptr == m_stpMainLogicPtr) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpLogicMainDrvPtr is nullptr");
            return false;
        }

        if (false == m_stpMainLogicPtr->Reset()) {
            LOG_WRITE(EN_LOG_ERROR, "Logic %d reset error.", uiSubIdx);
            return false;
        }

    } else {

        if (nullptr == m_stpMainLogicPtr) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpLogicCtrlDrvPtr is nullptr");
            return false;
        }

        if (false == m_stpMainLogicPtr->Reset()) {
            LOG_WRITE(EN_LOG_ERROR, "Logic %d reset error.", uiSubIdx);
            return false;
        }
    }
#endif

    return true;
}

bool
CBoard::LogicUpgrade(uint32_t uiIdx, const char *pcFilePath)
{
#if __RAINIER__
    std::shared_ptr<CFlash> stpFlashDrvPtr = nullptr;
    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "CBoard::LogicUpgrade %d %s", (int)uiIdx, pcFilePath);

    stpFlashDrvPtr = CBoardCmd::GetFlashPtr(uiIdx);
    if (nullptr == stpFlashDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) Flash driver error.", uiIdx);
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    if (!stpFlashDrvPtr->WriteImage(pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write flash error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CBoard::LogicUpgrade --");

    DEF_SAFE_FREE(pucBuffer);

    return true;
ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "CBoard::LogicUpgrade **");
    DEF_SAFE_FREE(pucBuffer);

    return false;

#endif

    return true;

}



bool
CBoard::EepromRead(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    if (false == stpEEpromPtr->Read(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) error.", uiIdx);
        return false;
    }

    return true;
}

bool
CBoard::EepromWrite(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    stpEEpromPtr = CBoardCmd::GetEEpromPtr(uiIdx);
    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Get uiIdx(%d) EEprom driver error.", uiIdx);
        return false;
    }

    if (false == stpEEpromPtr->Write(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom(%d) error.", uiIdx);
        return false;
    }

    return true;
}


bool
CBoard::I2cTest(uint32_t uiPsuIdx, uint32_t uiI2cIdx)
{
    std::shared_ptr<CPsu> stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);

    if (nullptr == stpPsuDrvPtr) {
        LOG_WRITE(EN_LOG_NOTICE, "Invalid id. uiIdx = %d", uiPsuIdx);
        return false;
    }
    CPowerSta astData[POWER_MAX_NUM] = {0};

    stpPsuDrvPtr->GetStatusData(astData[uiPsuIdx]);
    LOG_WRITE(EN_LOG_NOTICE, "PSU(%d) bIsOnline %d", uiPsuIdx, astData[uiPsuIdx].bIsOnline);
    LOG_WRITE(EN_LOG_NOTICE, "PSU(%d) bIsFailure %d", uiPsuIdx, astData[uiPsuIdx].bIsFailure);

    /* printf("PSU(%d) bIsOnline = %d\n", uiPsuIdx, astData[uiPsuIdx].bIsOnline); */
    /* printf("PSU(%d) bIsFailure = %d\n", uiPsuIdx, astData[uiPsuIdx].bIsFailure); */

    if ((astData[uiPsuIdx].bIsOnline)) {
        if ( 0 == uiI2cIdx ) {
            if (false == stpPsuDrvPtr->GetVol(astData[uiPsuIdx].usInVol, astData[uiPsuIdx].usOutVol)) {
                LOG_WRITE(EN_LOG_ERROR, "get psu #%d vol error.", uiPsuIdx);
                return false;
            }

            std::cout << "PSU #" << uiPsuIdx << " Out Vol = " << (int16_t)astData[uiPsuIdx].usOutVol << std::endl;
            LOG_WRITE(EN_LOG_NOTICE, "PSU(%d) InVol %d", uiPsuIdx, (int16_t)astData[uiPsuIdx].usInVol);
            LOG_WRITE(EN_LOG_NOTICE, "PSU(%d) OutVol %d", uiPsuIdx, (int16_t)astData[uiPsuIdx].usOutVol);

        } else {

            uint8_t aucTmp[MFG_INFO_LEN_MAX] = {0};
            if (false == stpPsuDrvPtr->GetMfgItem(0x0C, aucTmp, 8)) {
                LOG_WRITE(EN_LOG_ERROR, "get psu #%d mfg error.", uiPsuIdx);
                return false;
            }

            std::cout << "Manufacture: " << aucTmp << std::endl;

        }
    } else {
        std::cout << "PSU #" << uiPsuIdx << " not online." << std::endl;
    }
    return true;
}


