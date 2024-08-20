/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevDrvManager.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DevDrvManager.h"
#include "DevDriver.h"
#include "InterfaceDriver.h"
#include "UartDriver.h"
#include "I2cDriver.h"
#include "LogicDriver.h"
#include "SwitchableI2c.h"
#include "MultiUart.h"
#include "SpiDriver.h"
#include "LogicSpi.h"
#include "VOaSpiDriver.h"
#include "VOcmSpiDriver.h"
#include "VWssSpiDriver.h"
#include "VOpsSpiDriver.h"
#include "VOtdrSpiDriver.h"
#include "VOcsDriver.h"
#include "VMolexOtdrDriver.h"

#include "OaProto.h"
#include "OtdrProto.h"
#include "ExtendProto.h"
#include "OpsProto.h"
#include "OcmProto.h"
#include "DgeProto.h"
#include "MolexOtdr.h"
#include "OcmMasterSpiProto.h"
#include "OcmMasterSpiDriver.h"
#include "OaProto.h"
#include "OaMasterSpiProto.h"
#include "OaMasterSpiDriver.h"
#include "WssProto.h"
#include "WssMasterSpiProto.h"
#include "WssMasterSpiDriver.h"
#include "OcsProto.h"
#include "OcsDriver.h"


#include "RegisterFile.h"
#include "VFanDriver.h"
#include "VPsuDriver.h"
#include "VLedDriver.h"
#include "VThermalDriver.h"
#include "VMuxDriver.h"
#include "VBoardDriver.h"
#include "VSlotDriver.h"
/*==============================function=========================*/

enum REGISTER_INFO_IDX
{
    PRES_REG_INFO,
    SCEN_REG_INFO,
    TREB_REG_INFO,
    FOUR_REG_INFO,
    FIVE_REG_INFO
};

CDevDrvManager &
CDevDrvManager::GetInstance(void)
{
    static CDevDrvManager s_stDevDrvManager;

    return s_stDevDrvManager;

}


std::shared_ptr<CBoard>
CDevDrvManager::GetBoardDrv()
{
    if (HW_TYPE_VIRTUAL == CRegisterFile::ms_stBoardRegInfo.staEEpromInfo[0][0].uiHwType) {
        return std::dynamic_pointer_cast<CBoard>(std::make_shared<CBoardVirtual>());
    }
    return std::make_shared<CBoard>();
}

std::shared_ptr<CLogicDriver>
CDevDrvManager::GetLogicDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_LOCAL_BUS == stRegInfo.uiHwType) {

        LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);

        /* A logic device drived by local-bus */
        return std::static_pointer_cast<CLogicDriver>(
                   /* std::make_shared<CLclBusLogic>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr, DATA_WIDTH_1B)); */
                   std::make_shared<CPcieBusLogic>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr, DATA_WIDTH_1B));

    } else if (HW_TYPE_LOCAL_BUS_BITMAP == stRegInfo.uiHwType) {

        LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);

        /* A logic device drived by local-bus */
        return std::static_pointer_cast<CLogicDriver>(
                   /* std::make_shared<CLclBusLogic>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr, DATA_WIDTH_1B)); */
                   std::make_shared<CPcieBusLogicBitmap>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr, DATA_WIDTH_1B));

    } else if ( HW_TYPE_I2C == stRegInfo.uiHwType) {

        /* A logic device drived by i2c */
        std::shared_ptr<CI2c> stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);
        return std::static_pointer_cast<CLogicDriver>(
                   std::make_shared<CI2cLogic>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stpI2cPtr, stRegInfo.uiDevAddr));
    }
    return nullptr;
}

std::shared_ptr<CI2cSwitch>
CDevDrvManager::GetI2cSwitch(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CI2cSwitch>(stpLogicDrvPtr);
}

std::shared_ptr<CUartSwitch>
CDevDrvManager::GetUartSwitch(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CUartSwitch>(stRegInfo.uiDevAddr, stpLogicDrvPtr);
}

std::shared_ptr<CI2c>
CDevDrvManager::GetI2cDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetI2cDrv +++");

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);


    LOG_WRITE(EN_LOG_NOTICE, "uiHwType = %d", stRegInfo.uiHwType);

#if 0
    std::shared_ptr<CI2c> stpFtPtr = std::static_pointer_cast<CI2c>(std::make_shared<CFtI2c>());
    stpFtPtr->Write(0, 0, nullptr, 0);
#endif

    if (HW_TYPE_FT_I2C == stRegInfo.uiHwType) {

        /* std::shared_ptr<CI2c> stpI2cPtr = std::static_pointer_cast<CI2c>(std::make_shared<CFtI2c>()); */

        return std::static_pointer_cast<CI2c>(std::make_shared<CFtI2c>());

    } else if (HW_TYPE_PCA_I2C == stRegInfo.uiHwType) {

        std::shared_ptr<CI2c> stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

        LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
        return std::static_pointer_cast<CI2c>(
                   std::make_shared<CPcaI2c>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));

    } else if (HW_TYPE_LOGIC_I2C == stRegInfo.uiHwType) {

        std::shared_ptr<CLogicDriver> stpLogicPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);

        return std::static_pointer_cast<CI2c>(
                   std::make_shared<CLogicI2c>(stpLogicPtr, stRegInfo.uiDevAddr));

    } else if (HW_TYPE_LOGIC_SWITCH_I2C == stRegInfo.uiHwType) {

        CRegisterInfo stI2cSwitchInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
        std::shared_ptr<CI2c> stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);
        std::shared_ptr<CI2cSwitch> stpI2cSwtichPtr = CBoardCmd::GetI2cSwitchPtr(stI2cSwitchInfo.uiHwIdx);

        if (nullptr == stpI2cPtr) {
            LOG_WRITE(EN_LOG_ERROR, "GetI2cPtr error. stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
            return nullptr;
        }

        if (nullptr == stpI2cSwtichPtr) {
            LOG_WRITE(EN_LOG_ERROR, "GetI2cSwitchPtr error. stI2cSwitchInfo.uiHwIdx = %d", stI2cSwitchInfo.uiHwIdx);
            return nullptr;
        }

        return std::static_pointer_cast<CI2c>(
                   std::make_shared<CLogicSwitchI2c>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr,
                                                     stpI2cSwtichPtr, stI2cSwitchInfo.uiHwSubIdx));

    } else if (HW_TYPE_SUSI4_I2C == stRegInfo.uiHwType) {

        return std::static_pointer_cast<CI2c>(std::make_shared<CSusi4I2c>());
    }

    return nullptr;
}

std::shared_ptr<CUartDriver>
CDevDrvManager::GetUartDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    std::shared_ptr<CUartDriver> stpDrvPtr = nullptr;
    std::shared_ptr<CUartDriver> stpUartDrvPtr = nullptr;

    LOG_WRITE(EN_LOG_NOTICE, "GetUartDrv +++");


    if (HW_TYPE_UART == stRegInfo.uiHwType) {

        stpDrvPtr = std::make_shared<CUartHwDriver>(stRegInfo.uiHwIdx);

    } else if (HW_TYPE_UART_USB == stRegInfo.uiHwType ) {
        LOG_WRITE(EN_LOG_NOTICE, "HW_TYPE_UART_USB");
        LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
        stpDrvPtr = std::make_shared<CUartUsbDriver>(stRegInfo.uiHwIdx);

    } else if (HW_TYPE_MULTI_UART == stRegInfo.uiHwType) {

        LOG_WRITE(EN_LOG_NOTICE, "HW_TYPE_MULTI_UART");
        LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
        stpUartDrvPtr = CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx);
        if (nullptr == stpUartDrvPtr) {
            LOG_WRITE(EN_LOG_NOTICE, "nullptr == stpUartDrvPtr");
        }

        CRegisterInfo stUartSwitchRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
        std::shared_ptr<CUartSwitch> stpUartSwitch = CBoardCmd::GetUartSwitchPtr(stUartSwitchRegInfo.uiHwIdx);
        LOG_WRITE(EN_LOG_NOTICE, "stUartSwitchRegInfo.uiHwIdx = %d", stUartSwitchRegInfo.uiHwIdx);
        if (nullptr == stpUartSwitch) {
            LOG_WRITE(EN_LOG_NOTICE, "nullptr == stpUartSwitch");
        }

        std::shared_ptr<CMultiUart> stpMultiUart =
            std::make_shared<CMultiUart>(stpUartDrvPtr, stRegInfo.uiHwSubIdx, stpUartSwitch);
        stpDrvPtr = std::static_pointer_cast<CUartDriver>(stpMultiUart);
    }


    LOG_WRITE(EN_LOG_NOTICE, "GetUartDrv ---");

    return stpDrvPtr;
}


std::shared_ptr<CApsd>
CDevDrvManager::GetApsd(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CApsd>(stpLogicDrvPtr);
}

std::shared_ptr<CWatchDog>
CDevDrvManager::GetWatchDog(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CWatchDog>(stpLogicDrvPtr);
}

std::shared_ptr<CResetBoard>
CDevDrvManager::GetResetBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CResetBoard>(stpLogicDrvPtr);
}

std::shared_ptr<CResetTypeRegister>
CDevDrvManager::GetResetTypeRegister(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CResetTypeRegister>(stpLogicDrvPtr);
}


std::shared_ptr<CFanBoard>
CDevDrvManager::GetFanBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    LOG_WRITE(EN_LOG_ERROR, "stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    if (nullptr == stpLogicDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrvPtr is nullptr.");
        return nullptr;
    }
    return std::make_shared<CFanBoard>(stpLogicDrvPtr);
}

std::shared_ptr<CPsuBoard>
CDevDrvManager::GetPsuBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CPsuBoard>(stpLogicDrvPtr);
}

std::shared_ptr<CLedBoard>
CDevDrvManager::GetLedBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CLedBoard>(stpLogicDrvPtr);
}


std::shared_ptr<COscBoard>
CDevDrvManager::GetOscBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    if ( nullptr == stpLogicDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrvPtr is nullptr. stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
        return nullptr;
    }
    return std::make_shared<COscBoard>(stpLogicDrvPtr);
}

std::shared_ptr<CSlotBoard>
CDevDrvManager::GetSlotBoard(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    return std::make_shared<CSlotBoard>(stpLogicDrvPtr);
}

std::shared_ptr<CLogic>
CDevDrvManager::GetLogic(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetLogic +++");

    std::shared_ptr<CLogicDriver> stpLogicDrv = nullptr;
    std::shared_ptr<CLogicDriver> stpUpgardeLogicDrv = nullptr;

    std::shared_ptr<CLogic> stpLogic = nullptr;

    if (c_rstaRegInfo.size() <= SCEN_REG_INFO) {
        LOG_WRITE(EN_LOG_ERROR, "Logic Register Info Count(%lu) error.", c_rstaRegInfo.size());
        return nullptr;
    }

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stRegUpgradeInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "HwIdx = %d, UpgradeHwIdx = %d, Proto = %d",
              stRegInfo.uiHwIdx,
              stRegUpgradeInfo.uiHwIdx,
              stRegInfo.uiProto);

    stpLogicDrv = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    if ( nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrvPtr is nullptr. stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
        return nullptr;
    }

    stpUpgardeLogicDrv = CBoardCmd::GetLogicDrvPtr(stRegUpgradeInfo.uiHwIdx);
    if ( nullptr == stpUpgardeLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicPtr is nullptr. stRegUpgradeInfo.uiHwIdx = %d", stRegUpgradeInfo.uiHwIdx);
        return nullptr;
    }

    if (PROTO_TYPE_CPLD == stRegUpgradeInfo.uiProto) {

        /*std::shared_ptr<CJtag> stpJtagPtr = std::make_shared<CJtag>(stpUpgardeLogicDrv); */
        /*stpLogic = std::static_pointer_cast<CLogic>(std::make_shared<CCpld>(stpLogicDrv, stpJtagPtr)); */

        std::shared_ptr<CJtag> stpJtagPtr = std::make_shared<CJtag>(stpUpgardeLogicDrv, stRegUpgradeInfo.uiDevAddr);
        stpLogic = std::static_pointer_cast<CLogic>(std::make_shared<CCpld>(stpLogicDrv, stpJtagPtr, stRegUpgradeInfo.uiHwSubIdx));
    } else {
        std::shared_ptr<CInterfaceDriver> stpSpi = std::static_pointer_cast<CInterfaceDriver>(
            std::make_shared<CLogicSlaveSpi>(stpUpgardeLogicDrv));

        std::shared_ptr<CFlash> stpFlash = std::make_shared<CFlash>(stRegUpgradeInfo.uiHwSubIdx, stpSpi, stpLogicDrv);
        stpLogic = std::static_pointer_cast<CLogic>(std::make_shared<CFpga>(stpLogicDrv, stpFlash, stpUpgardeLogicDrv));
    }

    LOG_WRITE(EN_LOG_NOTICE, "GetLogic ---");

    return stpLogic;
}


std::shared_ptr<CFan>
CDevDrvManager::GetFanDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        return std::dynamic_pointer_cast<CFan>(std::make_shared<CFanVirtual>());
    }

    std::vector<CRegisterInfo> staMax31760RegInfo = {c_rstaRegInfo.at(SCEN_REG_INFO)};
    std::vector<CRegisterInfo> staEEpromRegInfo = {c_rstaRegInfo.at(TREB_REG_INFO)};

    std::shared_ptr<CFanBoard> stpFanBoard = nullptr;
    std::shared_ptr<CMax31760> stpMax31760Ptr = nullptr;
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;

    LOG_WRITE(EN_LOG_NOTICE, "Board Logic idx = %d", stRegInfo.uiHwIdx);
    stpFanBoard = CBoardCmd::GetFanBoardPtr(stRegInfo.uiHwIdx);
    stpMax31760Ptr = GetMax31760Drv(staMax31760RegInfo);
    stpEEpromPtr = GetEEpromDrv(staEEpromRegInfo);

    return std::make_shared<CFan>(stpFanBoard, stRegInfo.uiHwSubIdx, stpEEpromPtr, stpMax31760Ptr);
}


std::shared_ptr<CPsu>
CDevDrvManager::GetPsuDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stPsuBoardInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_VIRTUAL == stPsuBoardInfo.uiHwType) {
        return std::dynamic_pointer_cast<CPsu>(std::make_shared<CPsuVirtual>());
    }

    CRegisterInfo stEEpromRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
    CRegisterInfo stVolRegInfo = c_rstaRegInfo.at(TREB_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "c_rstRegInfo.uiDevAddr = 0X%02x", stVolRegInfo.uiDevAddr);

    std::shared_ptr<CI2c> stpI2cPtr = CBoardCmd::GetI2cPtr(stVolRegInfo.uiHwIdx);
    std::shared_ptr<CPsuBoard> stpPsuBoardPtr = CBoardCmd::GetPsuBoardPtr(stPsuBoardInfo.uiHwIdx);
    return std::make_shared<CPsu>(stpI2cPtr,
                                  stVolRegInfo.uiHwSubIdx,
                                  stVolRegInfo.uiDevAddr,
                                  stEEpromRegInfo.uiDevAddr,
                                  stpPsuBoardPtr,
                                  stPsuBoardInfo.uiHwSubIdx);
}

std::shared_ptr<CLed>
CDevDrvManager::GetLedDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    /* TODO: other HW interface */
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        return std::dynamic_pointer_cast<CLed>(std::make_shared<CLedVirtual>());
    }

    std::shared_ptr<CLedBoard> stpLedBoardPtr = CBoardCmd::GetLedBoardPtr(stRegInfo.uiHwIdx);

    return std::make_shared<CLed>(stpLedBoardPtr, stRegInfo.uiHwSubIdx);
}

std::shared_ptr<CTemp>
CDevDrvManager::GetTempDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        return std::dynamic_pointer_cast<CTemp>(std::make_shared<CThermalVirtual>());
    }

    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

    return std::static_pointer_cast<CTemp>(
               std::make_shared<CI2cTemp>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
}
std::shared_ptr<CLtc4215>
CDevDrvManager::GetLtc4215Drv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetLtc4215Drv +++");
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "GetLtc4215Drv ---");

    return std::static_pointer_cast<CLtc4215>(
               std::make_shared<CLtc4215>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
}

std::shared_ptr<CMp5023>
CDevDrvManager::GetMp5023Drv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetMp5023Drv +++");
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "GetMp5023Drv ---");

    return std::static_pointer_cast<CMp5023>(
               std::make_shared<CMp5023>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
}

std::shared_ptr<CMax1139>
CDevDrvManager::GetMax1139Drv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "GetMax1139Drv +++");
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "GetMax1139Drv ---");

    return std::static_pointer_cast<CMax1139>(
               std::make_shared<CMax1139>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
#else
    LOG_WRITE(EN_LOG_NOTICE, "GetMax1139Drv +++");
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrv = nullptr;
    stpLogicDrv = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
    if ( nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrvPtr is nullptr. stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);
        return nullptr;
    }

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "GetMax1139Drv ---");

    return std::static_pointer_cast<CMax1139>(
               std::make_shared<CMax1139>(stpLogicDrv, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));

#endif
}

std::shared_ptr<CMax31760>
CDevDrvManager::GetMax31760Drv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetMax31760Drv +++");
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);
    LOG_WRITE(EN_LOG_NOTICE, "GetMax31760Drv ---");

    return std::static_pointer_cast<CMax31760>(
               std::make_shared<CMax31760>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
}

std::shared_ptr<CEEprom>
CDevDrvManager::GetEEpromDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwType = %d, uiHwIdx = %d", stRegInfo.uiHwType, stRegInfo.uiHwIdx);

    if (HW_TYPE_I2C == stRegInfo.uiHwType) {

        std::shared_ptr<CI2c> stpI2cPtr = CBoardCmd::GetI2cPtr(stRegInfo.uiHwIdx);
        return std::static_pointer_cast<CEEprom>
                   (std::make_shared<CI2cEEprom>(stpI2cPtr, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));

    } else if (HW_TYPE_SPI_USB_FT == stRegInfo.uiHwType ) {

        printf("GetEEpromDrv CFtSpi\n");
        std::shared_ptr<CFtSpi> stpFtSpi = std::make_shared<CFtSpi>();
        /* stpFtSpi->Init(); */
        std::shared_ptr<CInterfaceDriver> stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(stpFtSpi);

        return std::static_pointer_cast<CEEprom>
                   (std::make_shared<CSpiEEprom>(stpDrvPtr, stRegInfo.uiHwSubIdx));
    } else {
        return nullptr;
    }
}

std::shared_ptr<CFlash>
CDevDrvManager::GetFlashDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    std::shared_ptr<CLogicDriver> stpLogicPtr;
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;

    LOG_WRITE(EN_LOG_NOTICE, "GetFlashDrv +++");

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "[HwType = %d, HwIdx = %d, HwSubIdx = %d]",
              stRegInfo.uiHwType,
              stRegInfo.uiHwIdx,
              stRegInfo.uiHwSubIdx);

    if (HW_TYPE_LOGIC_SLAVE_SPI == stRegInfo.uiHwType) {
        stpLogicPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicSlaveSpi>(stpLogicPtr));
    }

    LOG_WRITE(EN_LOG_NOTICE, "GetFlashDrv ---");

    return std::make_shared<CFlash>(stRegInfo.uiHwSubIdx, stpDrvPtr);
}



std::shared_ptr<CMdio>
CDevDrvManager::GetMdioDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);

    LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiDevAddr = %d", stRegInfo.uiDevAddr);
    return std::static_pointer_cast<CMdio>(
               std::make_shared<CMdioFpgaDriver>(stRegInfo.uiDevAddr, stpLogicDrvPtr));
}

std::shared_ptr<CPhy>
CDevDrvManager::GetPhyDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    if (c_rstaRegInfo.size() < 1) {
        LOG_WRITE(EN_LOG_ERROR, "Phy Register Info. c_rstaRegInfo.size() = %lu", c_rstaRegInfo.size());
        return nullptr;
    }

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwIdx = %u, stRegInfo.uiHwSubIdx = %u", stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx);

    std::shared_ptr<CMdio> stpMdio = CBoardCmd::GetMdioPtr(stRegInfo.uiHwIdx);
    if (stpMdio == nullptr) {
        LOG_WRITE(EN_LOG_ERROR, "stpMdio is nullptr. HwIdx = %d", stRegInfo.uiHwIdx);
        return nullptr;
    }

    CRegisterInfo stLogicRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
    LOG_WRITE(EN_LOG_NOTICE, "stLogicRegInfo.uiHwIdx = %d, stLogicRegInfo.uiHwSubIdx = %d",
              stLogicRegInfo.uiHwIdx, stLogicRegInfo.uiHwSubIdx);
    std::shared_ptr<CLogicDriver> stpLogicDrv = nullptr;
    stpLogicDrv = CBoardCmd::GetLogicDrvPtr(stLogicRegInfo.uiHwIdx);

    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(TREB_REG_INFO);
    LOG_WRITE(EN_LOG_NOTICE, "stResetRegInfo.uiHwIdx = %d, stResetRegInfo.uiHwSubIdx = %d",
              stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx);

    /*std::shared_ptr<CResetBoard> stpResetBoard = nullptr; */
    /*stpResetBoard = CBoardCmd::GetResetBoardPtr(stResetRegInfo.uiHwIdx); */

    return std::make_shared<CPhy>(stpMdio, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpLogicDrv);
    /*return std::make_shared<CPhy>(stpMdio, stpResetBoard, stpLogicDrv); */
}

std::shared_ptr<CNetSwitch>
CDevDrvManager::GetNetSwitchDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    if (c_rstaRegInfo.size() < 2) {
        LOG_WRITE(EN_LOG_ERROR, "Net Switch Register Info error. c_rstaRegInfo.size() = %lu", c_rstaRegInfo.size());
        return nullptr;
    }
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
    std::shared_ptr<CLogicDriver> stpLogicDrv = nullptr;

    std::shared_ptr<CMdio> stpMdioDrvPtr = CBoardCmd::GetMdioPtr(stRegInfo.uiHwIdx);

    stpLogicDrv = CBoardCmd::GetLogicDrvPtr(stResetRegInfo.uiHwIdx);

    return std::make_shared<CNetSwitch>(stpMdioDrvPtr, stpLogicDrv);
}


std::shared_ptr<CSlotDriver>
CDevDrvManager::GetSlotDrv(const std::vector<CRegisterInfo> & c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        return std::static_pointer_cast<CSlotDriver>(
                   std::make_shared<CSlotVirtual>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    }

    LOG_WRITE(EN_LOG_NOTICE, "GetSlotDrv +++");

    if (HW_TYPE_USB == stRegInfo.uiHwType) {
        return std::static_pointer_cast<CSlotDriver>(
                   std::make_shared<CUsbSlotDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    } else if (HW_TYPE_NONE == stRegInfo.uiHwType ) {
        return std::static_pointer_cast<CSlotDriver>(
                   std::make_shared<CFixedSlotDriver>(stRegInfo.uiHwIdx));
    } else if (HW_TYPE_FIXED_OPS == stRegInfo.uiHwType ) {
        return std::static_pointer_cast<CSlotDriver>(
                   std::make_shared<CFixedOpsSlotDriver>(stRegInfo.uiHwIdx));
    } else if (HW_TYPE_FIXED_AMP == stRegInfo.uiHwType ) {
        return std::static_pointer_cast<CSlotDriver>(
                   std::make_shared<CFixedAmpSlotDriver>(stRegInfo.uiHwIdx));
    }


    LOG_WRITE(EN_LOG_NOTICE, "GetSlotDrv ---");
    return std::static_pointer_cast<CSlotDriver>(
               std::make_shared<CLogicSlotDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
}

std::shared_ptr<COaDriver>
CDevDrvManager::GetOaDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;
    std::shared_ptr<COaSwProto> stpProtoPtr;
    LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv +++");

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "HwIdx = %d, HwType = %d", stRegInfo.uiHwIdx, stRegInfo.uiHwType);
    LOG_WRITE(EN_LOG_NOTICE, "Reset Info: HwIdx = %d, HwSubIdx = %d", stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx);

    if (HW_TYPE_UART == stRegInfo.uiHwType) {
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx));
    } else if (HW_TYPE_LOGIC_MASTER_SPI == stRegInfo.uiHwType) {
        std::shared_ptr<COaDriver> stpOaPtr = nullptr;
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv HW_TYPE_LOGIC_MASTER_SPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx); /* stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0); */
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterSpi> (stpLogicDrvPtr));
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>());
        stpOaPtr = std::static_pointer_cast<COaDriver>(
            std::make_shared<COaMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));
        /*stpOaPtr->SetProtoPtr(std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>())); */
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv ---");
        return stpOaPtr;
    } else if (HW_TYPE_SPI == stRegInfo.uiHwType) {
        std::shared_ptr<COaDriver> stpOaPtr = nullptr;
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv HW_TYPE_LOGIC_MASTER_SPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterSpi> (stpLogicDrvPtr));
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>());
        stpOaPtr = std::static_pointer_cast<COaDriver>(
            std::make_shared<COaMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));
        /*stpOaPtr->SetProtoPtr(std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>())); */
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv ---");
        return stpOaPtr;
    } else if (HW_TYPE_VSPI == stRegInfo.uiHwType || HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        std::shared_ptr<COaDriver> stpOaPtr = nullptr;
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv HW_TYPE_LOGIC_VSPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<COaVSpi> (stRegInfo.uiHwSubIdx));
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>());
        stpOaPtr = std::static_pointer_cast<COaDriver>(
            std::make_shared<COaMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));
        /*stpOaPtr->SetProtoPtr(std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwMasterSpiProto>())); */
        LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv ---");
        return stpOaPtr;
    } else {
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx));
    }

    if (PROTO_TYPE_OA_CLI_SIG == stRegInfo.uiProto) {
        /* single OA */
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwCliSingleProto>());
    } else if (PROTO_TYPE_OA_CLI_K20 == stRegInfo.uiProto) {
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwK20Proto>());
    } else {
        /* multiple OA */
        stpProtoPtr = std::static_pointer_cast<COaSwProto>(std::make_shared<COaSwCliMultProto>());
    }


    LOG_WRITE(EN_LOG_NOTICE, "GetOaDrv ---");

    return std::static_pointer_cast<COaDriver>(std::make_shared<COaIoDriver>(
                                                   stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));

}


std::shared_ptr<CWssDriver>
CDevDrvManager::GetWssDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CWssDriver> stpWssPtr;

    std::shared_ptr<CUartDriver> stpUartPtr;
    std::shared_ptr<CWssUartDriver> stpUartWssPtr;

    std::shared_ptr<CResetBoard> stpWarmResetBoard;
    std::shared_ptr<CResetBoard> stpColdResetBoard;

    for (uint32_t uiIdx = 0; uiIdx < c_rstaRegInfo.size(); uiIdx++) {
        LOG_WRITE(EN_LOG_NOTICE, "HwType = %u, HwIdx = %u, HwSubIdx = %u",
                  c_rstaRegInfo[uiIdx].uiHwType, c_rstaRegInfo[uiIdx].uiHwIdx, c_rstaRegInfo[uiIdx].uiHwSubIdx);
    }

    if (c_rstaRegInfo.size() < (TREB_REG_INFO + 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Wss Register Info error. Register Info Cnt : %lu", c_rstaRegInfo.size());
        return nullptr;
    }


    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stColdResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);
    CRegisterInfo stWarmResetRegInfo = c_rstaRegInfo.at(TREB_REG_INFO);

    if (HW_TYPE_UART == stRegInfo.uiHwType) {
        stpUartPtr = CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx);
    } else if (HW_TYPE_LOGIC_MASTER_SPI == stRegInfo.uiHwType) {
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
        std::shared_ptr<CInterfaceDriver> stpDrvPtr = nullptr;
        stpColdResetBoard = CBoardCmd::GetResetBoardPtr(stColdResetRegInfo.uiHwIdx);
        stpWarmResetBoard = CBoardCmd::GetResetBoardPtr(stWarmResetRegInfo.uiHwIdx);
        LOG_WRITE(EN_LOG_NOTICE, "GetWss DrvHW_TYPE_LOGIC_MASTER_SPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterWssSpi> (stpLogicDrvPtr));
        stpWssPtr = std::static_pointer_cast<CWssDriver>(
            std::make_shared<CWssMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stpColdResetBoard, stColdResetRegInfo.uiHwSubIdx, stpWarmResetBoard, stWarmResetRegInfo.uiHwSubIdx, stpDrvPtr));
        stpWssPtr->SetProtoPtr(std::static_pointer_cast<CWssSwProto>(std::make_shared<CWssSwMasterSpiFrameProto>()));
        return stpWssPtr;
    } else if (HW_TYPE_VSPI == stRegInfo.uiHwType || HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
        std::shared_ptr<CInterfaceDriver> stpDrvPtr = nullptr;
        stpColdResetBoard = CBoardCmd::GetResetBoardPtr(stColdResetRegInfo.uiHwIdx);
        stpWarmResetBoard = CBoardCmd::GetResetBoardPtr(stWarmResetRegInfo.uiHwIdx);
        LOG_WRITE(EN_LOG_NOTICE, "GetWss DrvHW_TYPE_VSPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CWssVSpi> ());
        stpWssPtr = std::static_pointer_cast<CWssDriver>(
            std::make_shared<CWssMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stpColdResetBoard, stColdResetRegInfo.uiHwSubIdx, stpWarmResetBoard, stWarmResetRegInfo.uiHwSubIdx, stpDrvPtr));
        stpWssPtr->SetProtoPtr(std::static_pointer_cast<CWssSwProto>(std::make_shared<CWssSwMasterSpiFrameProto>()));
        return stpWssPtr;
    } else {
        stpUartPtr = CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx);
    }
    stpColdResetBoard = CBoardCmd::GetResetBoardPtr(stColdResetRegInfo.uiHwIdx);
    stpWarmResetBoard = CBoardCmd::GetResetBoardPtr(stWarmResetRegInfo.uiHwIdx);


    stpUartWssPtr = std::make_shared<CWssUartDriver>(stRegInfo.uiHwIdx,
                                                     stpUartPtr,
                                                     stpColdResetBoard,
                                                     stColdResetRegInfo.uiHwSubIdx,
                                                     stpWarmResetBoard,
                                                     stWarmResetRegInfo.uiHwSubIdx);

    stpWssPtr = std::static_pointer_cast<CWssDriver>(stpUartWssPtr);
    stpWssPtr->SetProtoPtr(std::static_pointer_cast<CWssSwProto>(std::make_shared<CWssSwFrameProto>()));

    return stpWssPtr;

}

std::shared_ptr<COcmDriver>
CDevDrvManager::GetOcmDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<COcmDriver> stpOcmPtr = nullptr;
    std::shared_ptr<CResetBoard> stpResetBoardPtr = nullptr;
    std::shared_ptr<CInterfaceDriver> stpDrvPtr = nullptr;
    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetBoardRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    if (HW_TYPE_LOGIC_SPI == stRegInfo.uiHwType) {

        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicSpi> (stpLogicDrvPtr));
        stpOcmPtr = std::static_pointer_cast<COcmDriver>(
            std::make_shared<COcmSpiDriver>(stRegInfo.uiHwSubIdx, stResetBoardRegInfo.uiHwIdx, stResetBoardRegInfo.uiHwSubIdx, stpDrvPtr));
        /* stpOcmPtr->SetProtoPtr(std::static_pointer_cast<COcmSwProto>(std::make_shared<COcmSwFrameProto>())); */
        stpOcmPtr->SetProtoPtr(std::static_pointer_cast<COcmSwProto>(std::make_shared<COcmSwOlspFrameProto>()));
    } else if (HW_TYPE_LOGIC_MASTER_SPI == stRegInfo.uiHwType) {
        LOG_WRITE(EN_LOG_NOTICE, "HW_TYPE_LOGIC_MASTER_SPI");
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterSpi> (stpLogicDrvPtr));
        stpOcmPtr = std::static_pointer_cast<COcmDriver>(
            std::make_shared<COcmMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetBoardRegInfo.uiHwIdx, stResetBoardRegInfo.uiHwSubIdx, stpDrvPtr));
        stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(stResetBoardRegInfo.uiHwIdx);
        stpOcmPtr->SetProtoPtr(std::static_pointer_cast<COcmSwProto>(std::make_shared<COcmSwMasterSpiFrameProto>()));
    } else if (HW_TYPE_VSPI == stRegInfo.uiHwType || HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<COcmVSpi>(stRegInfo.uiHwSubIdx));
        stpOcmPtr = std::static_pointer_cast<COcmDriver>(
            std::make_shared<COcmMasterSpiDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stResetBoardRegInfo.uiHwIdx, stResetBoardRegInfo.uiHwSubIdx, stpDrvPtr));
        stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(stResetBoardRegInfo.uiHwIdx);
        stpOcmPtr->SetProtoPtr(std::static_pointer_cast<COcmSwProto>(std::make_shared<COcmSwMasterSpiFrameProto>()));
    }

    return stpOcmPtr;

}

std::shared_ptr<CTdcm>
CDevDrvManager::GetTdcmDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    return std::static_pointer_cast<CTdcm>(
               std::make_shared<CI2cTdcm>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx, stRegInfo.uiDevAddr));
}

std::shared_ptr<CPam4Driver>
CDevDrvManager::GetPam4Drv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CPam4Driver> stpPtr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_I2C == stRegInfo.uiHwType) {
        stpPtr = std::static_pointer_cast<CPam4Driver>(std::make_shared<CPam4I2cDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    } else {
        stpPtr = std::static_pointer_cast<CPam4Driver>(std::make_shared<CPam4I2cDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    }

    stpPtr->SetProtoPtr(std::static_pointer_cast<CPam4SwProto>(std::make_shared<CPam4SwFrameProto>()));

    return stpPtr;

}

std::shared_ptr<COpsDriver>
CDevDrvManager::GetOpsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;
    std::shared_ptr<CLogicDriver> stpLogicDrvPtr;
    std::shared_ptr<CResetBoard> stpResetBoardPtr;
    std::shared_ptr<COpsSwProto> stpProtoPtr;

    for (uint32_t uiIdx = 0; uiIdx < c_rstaRegInfo.size(); uiIdx++) {
        LOG_WRITE(EN_LOG_NOTICE, "HwType = %u, HwIdx = %u, HwSubIdx = %u",
                  c_rstaRegInfo[uiIdx].uiHwType, c_rstaRegInfo[uiIdx].uiHwIdx, c_rstaRegInfo[uiIdx].uiHwSubIdx);
    }

    if (c_rstaRegInfo.size() < SCEN_REG_INFO + 1) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Register Info Count (%lu) error.", c_rstaRegInfo.size());
        return nullptr;
    }

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    if (HW_TYPE_UART_USB == stRegInfo.uiHwType) {
        /* stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CUartUsbDriver>()); */
    } else if (HW_TYPE_LOGIC_SPI == stRegInfo.uiHwType) {
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx); /*stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0); */
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicSpi> (stpLogicDrvPtr));
        stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(stResetRegInfo.uiHwIdx);
        stpProtoPtr = std::static_pointer_cast<COpsSwProto>(std::make_shared<COpsSwFrameProto>());
        return std::static_pointer_cast<COpsDriver>(
                   std::make_shared<COpsLogicSpiDriver>(stRegInfo.uiHwSubIdx, stpResetBoardPtr, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));
    } else if (HW_TYPE_LOGIC_MASTER_SPI == stRegInfo.uiHwType) {
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(1);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterSpi>(stpLogicDrvPtr));
        stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(stResetRegInfo.uiHwIdx);
    } else if (HW_TYPE_VSPI == stRegInfo.uiHwType || HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(1);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<COpsVSpi>());
        stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(stResetRegInfo.uiHwIdx);
    } else {
        /* stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CUartHwDriver>()); */
    }


    stpProtoPtr = std::static_pointer_cast<COpsSwProto>(std::make_shared<COpsSwFrameProto>());

    return std::static_pointer_cast<COpsDriver>(
               std::make_shared<COpsUartDriver>(stRegInfo.uiHwIdx, stpResetBoardPtr, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));

}

std::shared_ptr<CDceDriver>
CDevDrvManager::GetDceDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_FT_I2C == stRegInfo.uiHwType) {
        return std::static_pointer_cast<CDceDriver>(std::make_shared<CDceUartDriver>(stRegInfo.uiHwIdx));
    } else {
        return std::static_pointer_cast<CDceDriver>(std::make_shared<CDceUartDriver>(stRegInfo.uiHwIdx));
    }

}

std::shared_ptr<CDgeDriver>
CDevDrvManager::GetDgeDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{

    std::shared_ptr<CDgeDriver> stpPtr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_FT_I2C == stRegInfo.uiHwType) {
        stpPtr = std::static_pointer_cast<CDgeDriver>(std::make_shared<CDgeFpgaDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    } else {
        stpPtr = std::static_pointer_cast<CDgeDriver>(std::make_shared<CDgeFpgaDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    }

    stpPtr->SetProtoPtr(std::static_pointer_cast<CDgeSwProto>(std::make_shared<CDgeSwFrameProto>()));

    return stpPtr;

}

std::shared_ptr<CMcsDriver>
CDevDrvManager::GetMcsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_FT_I2C == stRegInfo.uiHwType) {
        return std::static_pointer_cast<CMcsDriver>(std::make_shared<CMcsFpgaDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    } else {
        return std::static_pointer_cast<CMcsDriver>(std::make_shared<CMcsFpgaDriver>(stRegInfo.uiHwIdx, stRegInfo.uiHwSubIdx));
    }

}

std::shared_ptr<COscDriver>
CDevDrvManager::GetOscDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<COscBoard> stpOscBoardPtr = nullptr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);

    if (HW_TYPE_I2C == stRegInfo.uiHwType) {
        return std::static_pointer_cast<COscDriver>(std::make_shared<CI2cOscDriver>(stRegInfo.uiHwIdx));
    } else if (HW_TYPE_OSC_BOARD == stRegInfo.uiHwType) {
        LOG_WRITE(EN_LOG_NOTICE, "Register Logic Osc. uiHwSubIdx = %d", stRegInfo.uiHwSubIdx);
        stpOscBoardPtr = CBoardCmd::GetOscBoardPtr(stRegInfo.uiHwIdx);
        return std::static_pointer_cast<COscDriver>(std::make_shared<CLogicOscDriver>(
                                                        stRegInfo.uiHwSubIdx, stpOscBoardPtr));
    } else {
        return std::static_pointer_cast<COscDriver>(std::make_shared<CI2cOscDriver>(stRegInfo.uiHwIdx));
    }
}

std::shared_ptr<COtdrDriver>
CDevDrvManager::GetOtdrDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;
    std::shared_ptr<CLogicDriver> stpLogicDrv;
    std::shared_ptr<COtdrProto> stpProtoPtr;

    LOG_WRITE(EN_LOG_NOTICE, "GetOtdrDrv +++");
    if (HW_TYPE_VIRTUAL == c_rstaRegInfo[PRES_REG_INFO].uiHwType) {
        /* Directly get virtual driver */
        return std::dynamic_pointer_cast<COtdrDriver>(std::make_shared<COtdrVirtual>(c_rstaRegInfo[PRES_REG_INFO].uiHwSubIdx));
    }

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwType = %d", stRegInfo.uiHwType);

    if (HW_TYPE_MULTI_UART == stRegInfo.uiHwType) {
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx));
        stpProtoPtr = std::static_pointer_cast<COtdrProto>(std::make_shared<COtdrAcclinkSwProto>());

        LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwIdx = %d", stRegInfo.uiHwIdx);

        LOG_WRITE(EN_LOG_NOTICE, "GetOtdrDrv ---");
        return std::static_pointer_cast<COtdrDriver>(
                   std::make_shared<COtdrUartDriver>(stRegInfo.uiHwIdx,
                                                     stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));

    } else if (HW_TYPE_VSPI == stRegInfo.uiHwType || HW_TYPE_VIRTUAL == stRegInfo.uiHwType) {
        stpLogicDrv = CBoardCmd::GetLogicDrvPtr(1);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<COtdrVSpi>(stRegInfo.uiHwSubIdx));
        stpProtoPtr = std::static_pointer_cast<COtdrProto>(std::make_shared<COtdrProtoMolex>());

        LOG_WRITE(EN_LOG_NOTICE, "GetVOtdrDrv ---");
        return std::static_pointer_cast<COtdrDriver>(
                   std::make_shared<CMolexOtdr>(stRegInfo.uiHwIdx,
                                                stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));

    } else {
        /* stpLogicDrv = CBoardCmd::GetLogicDrvPtr(1); */
        stpLogicDrv = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicMasterSpi>(stpLogicDrv));
        stpProtoPtr = std::static_pointer_cast<COtdrProto>(std::make_shared<COtdrProtoMolex>());

        LOG_WRITE(EN_LOG_NOTICE, "GetOtdrDrv ---");
        return std::static_pointer_cast<COtdrDriver>(
                   std::make_shared<CMolexOtdr>(stRegInfo.uiHwIdx,
                                                stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));
    }


}

std::shared_ptr<CMux>
CDevDrvManager::GetMuxDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    if (HW_TYPE_VIRTUAL == c_rstaRegInfo.at(PRES_REG_INFO).uiHwType) {
        return std::static_pointer_cast<CMux>(std::make_shared<CMuxVirtual>());
    }

    std::shared_ptr<CSpi> stpSpiPtr;
    std::shared_ptr<CEEprom> stpEEpromPtr;

    LOG_WRITE(EN_LOG_NOTICE, "GetMuxDrv +++");


    /* CRegisterInfo stRegInfo = c_rstaRegInfo.at(0); */
    std::vector<CRegisterInfo> staEEpromRegInfo = {c_rstaRegInfo.at(PRES_REG_INFO)};

    stpEEpromPtr = GetEEpromDrv(staEEpromRegInfo);

    std::shared_ptr<CFtSpi> stpFtSpi = std::make_shared<CFtSpi>();
    /* stpFtSpi->Init(); */
    stpSpiPtr = std::static_pointer_cast<CSpi>(stpFtSpi);

    LOG_WRITE(EN_LOG_NOTICE, "GetMuxDrv ---");
    return std::static_pointer_cast<CMux>(
               std::make_shared<CMuxSpiDriver>(0, stpSpiPtr, stpEEpromPtr));
}

std::shared_ptr<CExtendDriver>
CDevDrvManager::GetExtendDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    std::shared_ptr<CInterfaceDriver> stpDrvPtr;
    std::shared_ptr<CExtendSwProto> stpProtoPtr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    if (HW_TYPE_UART == stRegInfo.uiHwType) {
        stpDrvPtr = CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx);
    } else {
        stpDrvPtr = CBoardCmd::GetUartPtr(stRegInfo.uiHwIdx);
    }

    stpProtoPtr = std::static_pointer_cast<CExtendSwProto>(std::make_shared<CExtendSwProto>());

    LOG_WRITE(EN_LOG_NOTICE, "stRegInfo.uiHwSubIdx = %d", stRegInfo.uiHwSubIdx);
    return std::static_pointer_cast<CExtendDriver>(std::make_shared<CExtendUartDriver>(
                                                       stRegInfo.uiHwSubIdx, stResetRegInfo.uiHwIdx, stResetRegInfo.uiHwSubIdx, stpDrvPtr, stpProtoPtr));

}

std::shared_ptr<COcsDriver>
CDevDrvManager::GetOcsDrv(const std::vector<CRegisterInfo> &c_rstaRegInfo)
{
    if (HW_TYPE_VIRTUAL == c_rstaRegInfo[PRES_REG_INFO].uiHwType) {
        return std::dynamic_pointer_cast<COcsDriver>(std::make_shared<COcsVirtual>());
    }

    std::shared_ptr<COcsDriver> stpOcsPtr = nullptr;
    std::shared_ptr<CResetBoard> stpResetBoardPtr = nullptr;
    std::shared_ptr<CInterfaceDriver> stpDrvPtr = nullptr;
    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;

    CRegisterInfo stRegInfo = c_rstaRegInfo.at(PRES_REG_INFO);
    CRegisterInfo stResetBoardRegInfo = c_rstaRegInfo.at(SCEN_REG_INFO);

    if (HW_TYPE_LOGIC_SPI == stRegInfo.uiHwType) {

        stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(stRegInfo.uiHwIdx);
        stpDrvPtr = std::static_pointer_cast<CInterfaceDriver>(std::make_shared<CLogicSpi> (stpLogicDrvPtr));
        stpOcsPtr = std::static_pointer_cast<COcsDriver>(
            std::make_shared<COcsSpiDriver>(stRegInfo.uiHwSubIdx, stResetBoardRegInfo.uiHwIdx, stResetBoardRegInfo.uiHwSubIdx, stpDrvPtr));
        stpOcsPtr->SetProtoPtr(std::static_pointer_cast<COcsSwProto>(std::make_shared<COcsSwFrameProto>()));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "NOT support uiHwType: %d", stRegInfo.uiHwType);
    }

    return stpOcsPtr;

}

