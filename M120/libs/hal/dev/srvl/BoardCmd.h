/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      BoardCmd.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Semaphore.h"
#include "ShmOperator.h"

#include "LogicDriver.h"
#include "I2cSwitch.h"
#include "UartSwitch.h"

#include "ResetTypeRegister.h"
#include "Apsd.h"
#include "WatchDog.h"
#include "ResetBoard.h"

#include "FanBoard.h"
#include "PsuBoard.h"
#include "LedBoard.h"
#include "OscBoard.h"
#include "SlotBoard.h"

#include "Logic.h"

#include "FanDriver.h"
#include "PsuDriver.h"
#include "LedDriver.h"
#include "TempDriver.h"
#include "Ltc4215Driver.h"
#include "Mp5023Driver.h"
#include "Max1139Driver.h"
#include "Max31760Driver.h"
#include "BoardDriver.h"
#include "Flash.h"

#include "MuxDriver.h"
#include "NetSwitch.h"
/*#include "Phy.h" */

/*************************************************/
class CUartDriver;
class COaDriver;
class CLed;
class CPhy;

class CBoardCmd
{
public:

    static void
    AddLogicDrvPtr(const std::shared_ptr<CLogicDriver> c_staLogicDrvPtr)
    {
        ms_staLogicDrvPtrs.push_back(c_staLogicDrvPtr);
    }

    static void
    SetLogicDrvPtr(const std::shared_ptr<CLogic> c_staLogicPtr)
    {
        ms_staLogicPtrs.push_back(c_staLogicPtr);
    }

    static void
    SetI2cSwitchPtr(const std::vector<std::shared_ptr<CI2cSwitch> > &c_staI2cSwitchPtr)
    {
        ms_staI2cSwitchPtrs = c_staI2cSwitchPtr;
    }

    static void
    SetUartSwitchPtr(const std::vector<std::shared_ptr<CUartSwitch> > &c_staUartSwitchPtr)
    {
        ms_staUartSwitchPtrs = c_staUartSwitchPtr;
    }

    static void
    AddI2cPtr(const std::shared_ptr<CI2c> c_staI2cPtr)
    {
        ms_staI2cPtrs.push_back(c_staI2cPtr);
    }

    static void
    AddUartPtr(const std::shared_ptr<CUartDriver> c_staUartPtr)
    {
        ms_staUartPtrs.push_back(c_staUartPtr);
    }

    static void
    SetResetTypeRegister(const std::vector<std::shared_ptr<CResetTypeRegister> > &c_rstaResetTypeRegister)
    {
        ms_staResetTypeRegisters = c_rstaResetTypeRegister;
    }

    static void
    SetApsdPtr(const std::vector<std::shared_ptr<CApsd> > &c_rstaApsdPtrs)
    {
        ms_staApsdPtrs = c_rstaApsdPtrs;
    }

    static void
    SetWatchDogPtr(const std::vector<std::shared_ptr<CWatchDog> > &c_rstaWatchDogPtrs)
    {
        ms_staWatchDogPtrs = c_rstaWatchDogPtrs;
    }

    static void
    SetResetBoardPtr(const std::vector<std::shared_ptr<CResetBoard> > &c_rstaResetBoardPtrs)
    {
        ms_staResetBoardPtrs = c_rstaResetBoardPtrs;
    }

    static void
    SetFanBoardPtr(const std::vector<std::shared_ptr<CFanBoard> > &c_rstaFanBoardPtrs)
    {
        ms_staFanBoardPtrs = c_rstaFanBoardPtrs;
    }

    static void
    SetPsuBoardPtr(const std::vector<std::shared_ptr<CPsuBoard> > &c_rstaPsuBoardPtrs)
    {
        ms_staPsuBoardPtrs = c_rstaPsuBoardPtrs;
    }

    static void
    SetLedBoardPtr(const std::vector<std::shared_ptr<CLedBoard> > &c_rstaLedBoardPtrs)
    {
        ms_staLedBoardPtrs = c_rstaLedBoardPtrs;
    }

    static void
    SetOscBoardPtr(const std::vector<std::shared_ptr<COscBoard> > &c_rstaOscBoardPtrs)
    {
        ms_staOscBoardPtrs = c_rstaOscBoardPtrs;
    }

    static void
    SetSlotBoardPtr(const std::vector<std::shared_ptr<CSlotBoard> > &c_rstaSlotBoardPtrs)
    {
        ms_staSlotBoardPtrs = c_rstaSlotBoardPtrs;
    }

    static void
    SetLogicPtr(const std::vector<std::shared_ptr<CLogic> > &c_rstaLogicPtrs)
    {
        ms_staLogicPtrs = c_rstaLogicPtrs;
    }

    static void
    AddLogicPtr(const std::shared_ptr<CLogic> c_staLogicPtr)
    {
        ms_staLogicPtrs.push_back(c_staLogicPtr);
    }

    static void
    SetBoardPtr(const std::shared_ptr<CBoard> &c_rstpBoardPtr)
    {
        ms_stpBoardPtr = c_rstpBoardPtr;
    }

    static void
    SetI2cDrvPtr(const std::vector<std::shared_ptr<CI2c> > &c_rstaI2cDrvPtrs)
    {
        ms_staI2cPtrs = c_rstaI2cDrvPtrs;
    }

    static void
    SetFanPtr(const std::vector<std::shared_ptr<CFan> > &c_rstaFanPtrs)
    {
        ms_staFanPtrs = c_rstaFanPtrs;
    }

    static void
    SetPsuPtr(const std::vector<std::shared_ptr<CPsu> > &c_rstaPsuPtrs)
    {
        ms_staPsuPtrs = c_rstaPsuPtrs;
    }

    static void
    SetLedPtr(const std::vector<std::shared_ptr<CLed> > &c_rstaLedPtrs)
    {
        ms_staLedPtrs = c_rstaLedPtrs;
    }

    static void
    SetTempPtr(const std::vector<std::shared_ptr<CTemp> > &c_rstaTempPtrs)
    {
        ms_staTempPtrs = c_rstaTempPtrs;
    }

    static void
    SetEEpromPtr(const std::vector<std::shared_ptr<CEEprom> > &c_rstaEEpromPtrs)
    {
        ms_staEEpromPtrs = c_rstaEEpromPtrs;
    }

    static void
    SetFlashPtr(const std::vector<std::shared_ptr<CFlash> > &c_rstaFlashPtrs)
    {
        ms_staFlashPtrs = c_rstaFlashPtrs;
    }

    static void
    SetMdioDrvPtr(const std::vector<std::shared_ptr<CMdio> > &c_rstaMdioDrvPtr)
    {
        ms_staMdioPtrs = c_rstaMdioDrvPtr;
    }

    static void
    SetNetSwitchPtr(const std::vector<std::shared_ptr<CNetSwitch> > &c_rstaNetSwitchPtr)
    {
        ms_staNetSwitchPtrs = c_rstaNetSwitchPtr;
    }

    static void
    SetPhyPtr(const std::vector<std::shared_ptr<CPhy> > &c_rstaPhyPtr)
    {
        ms_staPhyPtrs = c_rstaPhyPtr;
    }

    static void
    SetLtc4215Ptr(const std::vector<std::shared_ptr<CLtc4215> > &c_rstaLtc4215Ptrs)
    {
        ms_staLtc4215Ptrs = c_rstaLtc4215Ptrs;
    }

    static void
    SetMp5023Ptr(const std::vector<std::shared_ptr<CMp5023> > &c_rstaMp5023Ptrs)
    {
        ms_staMp5023Ptrs = c_rstaMp5023Ptrs;
    }

    static void
    SetMax1139Ptr(const std::vector<std::shared_ptr<CMax1139> > &c_rstaMax1139Ptrs)
    {
        ms_staMax1139Ptrs = c_rstaMax1139Ptrs;
    }

    static void
    SetMax31760Ptr(const std::vector<std::shared_ptr<CMax31760> > &c_rstaMax31760Ptrs)
    {
        ms_staMax31760Ptrs = c_rstaMax31760Ptrs;
    }

    static void
    SetShm(const std::array<CShareMemory, SHM_TYPE_MAX> &c_rstaShm,
           const std::array<CSemaphore, SHM_TYPE_MAX> &c_rstaSem)
    {
        ms_staShm = c_rstaShm;
        ms_staSem = c_rstaSem;
    }

    static std::shared_ptr<CLogicDriver>
    GetLogicDrvPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staLogicDrvPtrs.size()) ? nullptr : ms_staLogicDrvPtrs.at(uiIdx);
    }


    static std::shared_ptr<CI2cSwitch>
    GetI2cSwitchPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staI2cSwitchPtrs.size()) ? nullptr : ms_staI2cSwitchPtrs.at(uiIdx);
    }

    static std::shared_ptr<CUartSwitch>
    GetUartSwitchPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staUartSwitchPtrs.size()) ? nullptr : ms_staUartSwitchPtrs.at(uiIdx);
    }

    static std::shared_ptr<CI2c>
    GetI2cPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staI2cPtrs.size()) ? nullptr : ms_staI2cPtrs.at(uiIdx);
    }


    static std::shared_ptr<CUartDriver>
    GetUartPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staUartPtrs.size()) ? nullptr : ms_staUartPtrs.at(uiIdx);
    }

    static std::shared_ptr<CResetTypeRegister>
    GetResetTypeRegister(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staResetTypeRegisters.size()) ? nullptr : ms_staResetTypeRegisters.at(uiIdx);
    }

    static std::shared_ptr<CApsd>
    GetApsdPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staApsdPtrs.size()) ? nullptr : ms_staApsdPtrs.at(uiIdx);
    }

    static std::shared_ptr<CWatchDog>
    GetWatchDogPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staWatchDogPtrs.size()) ? nullptr : ms_staWatchDogPtrs.at(uiIdx);
    }

    static std::shared_ptr<CResetBoard>
    GetResetBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staResetBoardPtrs.size()) ? nullptr : ms_staResetBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<CFanBoard>
    GetFanBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staFanBoardPtrs.size()) ? nullptr : ms_staFanBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<CPsuBoard>
    GetPsuBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staPsuBoardPtrs.size()) ? nullptr : ms_staPsuBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<CLedBoard>
    GetLedBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staLedBoardPtrs.size()) ? nullptr : ms_staLedBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<COscBoard>
    GetOscBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staOscBoardPtrs.size()) ? nullptr : ms_staOscBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<CSlotBoard>
    GetSlotBoardPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staSlotBoardPtrs.size()) ? nullptr : ms_staSlotBoardPtrs.at(uiIdx);
    }

    static std::shared_ptr<CLogic>
    GetLogicPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staLogicPtrs.size()) ? nullptr : ms_staLogicPtrs.at(uiIdx);
    }

    static std::shared_ptr<CFan>
    GetFanPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staFanPtrs.size()) ? nullptr : ms_staFanPtrs.at(uiIdx);
    }

    static std::shared_ptr<CPsu>
    GetPsuPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staPsuPtrs.size()) ? nullptr : ms_staPsuPtrs.at(uiIdx);
    }

    static std::shared_ptr<CLed>
    GetLedPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staLedPtrs.size()) ? nullptr : ms_staLedPtrs.at(uiIdx);
    }

    static std::shared_ptr<CTemp>
    GetTempPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staTempPtrs.size()) ? nullptr : ms_staTempPtrs.at(uiIdx);
    }

    static std::shared_ptr<CEEprom>
    GetEEpromPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staEEpromPtrs.size()) ? nullptr : ms_staEEpromPtrs.at(uiIdx);
    }

    static std::shared_ptr<CFlash>
    GetFlashPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staFlashPtrs.size()) ? nullptr : ms_staFlashPtrs.at(uiIdx);
    }


    static std::shared_ptr<CMdio>
    GetMdioPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staMdioPtrs.size()) ? nullptr : ms_staMdioPtrs.at(uiIdx);
    }

    static std::shared_ptr<CNetSwitch>
    GetNetSwitchPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staNetSwitchPtrs.size()) ? nullptr : ms_staNetSwitchPtrs.at(uiIdx);
    }

    static std::shared_ptr<CPhy>
    GetPhyPtr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staPhyPtrs.size()) ? nullptr : ms_staPhyPtrs.at(uiIdx);
    }

    static std::shared_ptr<CLtc4215>
    GetLtc4215Ptr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staLtc4215Ptrs.size()) ? nullptr : ms_staLtc4215Ptrs.at(uiIdx);
    }

    static std::shared_ptr<CMp5023>
    GetMp5023Ptr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staMp5023Ptrs.size()) ? nullptr : ms_staMp5023Ptrs.at(uiIdx);
    }

    static std::shared_ptr<CMax1139>
    GetMax1139Ptr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staMax1139Ptrs.size()) ? nullptr : ms_staMax1139Ptrs.at(uiIdx);
    }

    static std::shared_ptr<CMax31760>
    GetMax31760Ptr(uint32_t uiIdx)
    {
        return (uiIdx >= ms_staMax31760Ptrs.size()) ? nullptr : ms_staMax31760Ptrs.at(uiIdx);
    }

    static bool GetChassisCfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetChassisCfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool GetSccCfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetSccCfg(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetChassisMfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetChassisConfigType(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetSccMfg(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetChassisCali(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool GetUpgInfo(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetUpgInfo(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetBdlHead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool GetBdlHead(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetLedState(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetLedTestMode(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetLedLocationTestMode(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool SetFanMfg(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetFanSpeed(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetResetCauseInFan(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetFanLed(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetFanControllerEEP(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool LogicRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool LogicWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool LogicReadSlc(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool LogicWriteSlc(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool LogicReset(uint32_t uiLen, const uint8_t *cv_pucBuf);
    static bool LogicUpgrade(uint32_t uiLen, const uint8_t *cv_pucBuf);


    static bool SetResetType(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool CfgWatchDog(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetApsdEnable(uint32_t uiLen, const uint8_t *c_pucBuf);


    /* spi & mdio & flash & net-switch & eeprom */
    static bool MdioWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool MdioRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool NetSwitchWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool NetSwitchRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SwitchPhyRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SwitchPhyWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SpiReadWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SpiFlashRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SpiFlashWrite(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool EepromRead(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool EepromWrite(uint32_t uiLen, const uint8_t *c_pucBuf);

    /* debug api */
    static bool I2cTest(uint32_t uiLen, const uint8_t *c_pucBuf);
    static bool SetLogLevel(uint32_t uiLen, const uint8_t *c_pucBuf);

    static bool GetDevType(uint32_t uiLen, const uint8_t *cv_pucBuf);

private:
    static std::shared_ptr<CBoard> ms_stpBoardPtr;

    static std::vector<std::shared_ptr<CLogicDriver> > ms_staLogicDrvPtrs;
    static std::vector<std::shared_ptr<CI2cSwitch> > ms_staI2cSwitchPtrs;
    static std::vector<std::shared_ptr<CUartSwitch> > ms_staUartSwitchPtrs;
    static std::vector<std::shared_ptr<CI2c> > ms_staI2cPtrs;
    static std::vector<std::shared_ptr<CUartDriver> > ms_staUartPtrs;

    static std::vector<std::shared_ptr<CResetTypeRegister> > ms_staResetTypeRegisters;
    static std::vector<std::shared_ptr<CApsd> > ms_staApsdPtrs;
    static std::vector<std::shared_ptr<CWatchDog> > ms_staWatchDogPtrs;
    static std::vector<std::shared_ptr<CResetBoard> > ms_staResetBoardPtrs;

    static std::vector<std::shared_ptr<CFanBoard> > ms_staFanBoardPtrs;
    static std::vector<std::shared_ptr<CPsuBoard> > ms_staPsuBoardPtrs;
    static std::vector<std::shared_ptr<CLedBoard> > ms_staLedBoardPtrs;
    static std::vector<std::shared_ptr<COscBoard> > ms_staOscBoardPtrs;
    static std::vector<std::shared_ptr<CSlotBoard> > ms_staSlotBoardPtrs;

    static std::vector<std::shared_ptr<CLogic> > ms_staLogicPtrs;

    static std::vector<std::shared_ptr<CFan> > ms_staFanPtrs;
    static std::vector<std::shared_ptr<CPsu> > ms_staPsuPtrs;
    static std::vector<std::shared_ptr<CLed> > ms_staLedPtrs;

    static std::vector<std::shared_ptr<CTemp> > ms_staTempPtrs;
    static std::vector<std::shared_ptr<CEEprom> > ms_staEEpromPtrs;
    static std::vector<std::shared_ptr<CFlash> > ms_staFlashPtrs;

    static std::vector<std::shared_ptr<CMdio> > ms_staMdioPtrs;
    static std::vector<std::shared_ptr<CNetSwitch> > ms_staNetSwitchPtrs;
    static std::vector<std::shared_ptr<CPhy> > ms_staPhyPtrs;
    static std::vector<std::shared_ptr<CLtc4215> > ms_staLtc4215Ptrs;
    static std::vector<std::shared_ptr<CMp5023> > ms_staMp5023Ptrs;
    static std::vector<std::shared_ptr<CMax1139> > ms_staMax1139Ptrs;
    static std::vector<std::shared_ptr<CMax31760> > ms_staMax31760Ptrs;

    static std::array<CShareMemory, SHM_TYPE_MAX> ms_staShm;
    static std::array<CSemaphore, SHM_TYPE_MAX> ms_staSem;

    static bool __SetShmStatus(int32_t iStatus);
};







