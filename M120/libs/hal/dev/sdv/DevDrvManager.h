/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevDrvManager.h
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
#include "Register.h"

#include "LogicDriver.h"
#include "LogicI2c.h"

#include "LedDriver.h"
#include "TempDriver.h"
#include "Ltc4215Driver.h"
#include "Mp5023Driver.h"
#include "Max1139Driver.h"
#include "Max31760Driver.h"
#include "PsuDriver.h"
#include "Flash.h"

#include "OaDriver.h"
#include "WssDriver.h"
#include "OcmDriver.h"
#include "TdcmDriver.h"
#include "Pam4Driver.h"
#include "OpsDriver.h"
#include "DceDriver.h"
#include "SlotDriver.h"

#include "DgeDriver.h"
#include "McsDriver.h"
#include "OscDriver.h"
#include "OtdrDriver.h"

#include "OcsDriver.h"

/*************************************************/

class CDevDrvManager
{
public:
    static CDevDrvManager &GetInstance(void);

    std::shared_ptr<CLogicDriver> GetLogicDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CI2cSwitch> GetI2cSwitch(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CUartSwitch> GetUartSwitch(const std::vector<CRegisterInfo> &c_rstaRegInfo);

    std::shared_ptr<CBoard> GetBoardDrv();
    std::shared_ptr<CI2c> GetI2cDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CUartDriver> GetUartDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);


    std::shared_ptr<CApsd> GetApsd(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CWatchDog> GetWatchDog(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CResetBoard> GetResetBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo);
    std::shared_ptr<CResetTypeRegister> GetResetTypeRegister(const std::vector<CRegisterInfo> &c_rstaRegInfo);


    std::shared_ptr<CFanBoard> GetFanBoard(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CPsuBoard> GetPsuBoard(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CLedBoard> GetLedBoard(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COscBoard> GetOscBoard(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CSlotBoard> GetSlotBoard(const std::vector<CRegisterInfo> &c_rstaRegInfo);

    std::shared_ptr<CLogic> GetLogic(const std::vector<CRegisterInfo> & c_rstaRegInfo);

    std::shared_ptr<CEEprom> GetEEpromDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CFlash> GetFlashDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CLed> GetLedDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CFan> GetFanDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CTemp> GetTempDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CPsu> GetPsuDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMdio> GetMdioDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CPhy> GetPhyDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CNetSwitch> GetNetSwitchDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CLtc4215> GetLtc4215Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMp5023> GetMp5023Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMax1139> GetMax1139Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMax31760> GetMax31760Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COaDriver> GetOaDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CWssDriver> GetWssDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COcmDriver> GetOcmDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CTdcm> GetTdcmDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CPam4Driver> GetPam4Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COpsDriver> GetOpsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CDceDriver> GetDceDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);


    std::shared_ptr<CDgeDriver> GetDgeDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMcsDriver> GetMcsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COscDriver> GetOscDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<COtdrDriver> GetOtdrDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CMux> GetMuxDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);
    std::shared_ptr<CExtendDriver> GetExtendDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);

    std::shared_ptr<COcsDriver> GetOcsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);

    std::shared_ptr<CSlotDriver> GetSlotDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo);

private:
    CDevDrvManager()
    {
    }
    CDevDrvManager(const CDevDrvManager &) = delete;
    CDevDrvManager &operator=(const CDevDrvManager &) = delete;

};







