/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevThread.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DevThread.h"

#include "Delay.h"

#include "Register.h"
#include "UpgdManager.h"
#include "DevData.h"

#include "DevDrvManager.h"
#include "DevDriver.h"
#include "BoardDriver.h"
#include "LogicDriver.h"
#include "LedDriver.h"
#include "FanDriver.h"
#include "Event.h"


bool CDevThread::m_bLoopEnable = true;
uint32 CDevThread::m_uiInterval = 20;
bool CDevThread::m_bForceIdle = false;


/*==============================function=========================*/

void
CDevThread::CreateShm(const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg, uint32_t uiKeyOffset)
{

    for (int32_t i = 0; i < SHM_TYPE_MAX; i++) {
        if (!m_staShm.at(i).Get(c_rstaShmCfg.at(i).eShmKey + uiKeyOffset)) {
            m_staShm.at(i).CreateAttache(c_rstaShmCfg.at(i).eShmKey + uiKeyOffset, c_rstaShmCfg.at(i).eShmSize);
            LOG_WRITE(EN_LOG_DEBUG, "shm create");
        } else {
            m_staShm.at(i).Attache();
            m_staShm.at(i).Clear();
            LOG_WRITE(EN_LOG_DEBUG, "shm Attache");
        }

        if (!m_staSem.at(i).Get(c_rstaShmCfg.at(i).eSemKey + uiKeyOffset)) {
            m_staSem.at(i).Create(c_rstaShmCfg.at(i).eSemKey + uiKeyOffset);
            LOG_WRITE(EN_LOG_DEBUG, "sem create");
        }
    }



}

bool
CDevThread::ClearShm(void)
{

    for (int32_t i = 0; i < SHM_TYPE_MAX; i++) {
        if (!m_staSem.at(i).P()) {
            return false;
        }

        if (!m_staShm.at(i).Clear()) {
            return false;
        }

        if (!m_staSem.at(i).V()) {
            return false;
        }
    }

    return true;

}

bool
CDevThread::WaitInitOk(void)
{
    int iTimes = 0;
    bool bRet = false;

    /* set sub dev num to shm */
    SetSubDevNum();

#if 0
    while (true) {
#else
    while (iTimes < 15) {
#endif
        /* get mfg and set mfg to shm */
        if (GetMfg()) {
            /* get and set status data to shm */
            if (GetStatusData()) {
                /* init OK, set state to shm */
                CShmDevSta stState = {SHM_STATUS_OK, SHM_STATUS_NONE, SHM_STATUS_NONE};
                CShmOperator::Set(m_staShm.at(SHM_TYPE_STA), m_staSem.at(SHM_TYPE_STA), (uint8_t *)&stState,
                                  CShmOperator::GetStaDevStaPos(), sizeof(stState));
                LOG_WRITE(EN_LOG_NOTICE, "WaitInitOk.");
                bRet = true;
                break;
            }
        }

        ++iTimes;

        CDelay::Delay(1000, MS);
    }

    return bRet;
}

#if 0
void
CDevThread::__StartTime()
{
    gettimeofday(&m_stStartTime, NULL);
}

bool
CDevThread::__IsNeedRefresh()
{
    gettimeofday(&m_stStopTime, NULL);

    long elapsed = (m_stStopTime.tv_sec - m_stStartTime.tv_sec) * 1000000 + (m_stStopTime.tv_usec - m_stStartTime.tv_usec);

    if (elapsed > 1000 * m_uiStatusInterval) {
        return true;
    } else if (elapsed < 0) {
        return true;
    } else {
        return false;
    }
}
#else
void
CDevThread::__StartTime()
{
    clock_gettime(CLOCK_MONOTONIC, &m_stStartTimeSpec);
}

bool
CDevThread::__IsNeedRefresh()
{
    clock_gettime(CLOCK_MONOTONIC, &m_stStopTimeSpec);

    long elapsed = (m_stStopTimeSpec.tv_sec - m_stStartTimeSpec.tv_sec) * 1000000 + ((m_stStopTimeSpec.tv_nsec - m_stStartTimeSpec.tv_nsec) / 1000);

    if (elapsed > 1000 * m_uiStatusInterval) {
        return true;
    } else {
        return false;
    }
}
#endif

void
CDevThread::LoopProc(void)
{
#define DEF_COMM_FAIL_COUNT 3

    CQueueNode stQNode;

    __StartTime();

    while (true) {
        if (m_bForceIdle) {
            CDelay::Delay(1, S);
            continue;
        }

        /* check if upgrade */
        if ((UPGD_BUSY != CUpgdManager::GetInstance().GetState()) &&
            (UPGD_LOAD_BUSY != CUpgdManager::GetInstance().GetLoadState())) {
            /* check if has cmd and process */
            if (CShmOperator::GetCmd(m_staShm.at(SHM_TYPE_CMD), m_staSem.at(SHM_TYPE_CMD), stQNode)) {
                CDevCmdProcesser::GetInstance().CmdProc(m_staShm.at(SHM_TYPE_CMD), m_staSem.at(SHM_TYPE_CMD), stQNode);
                /* CDelay::Delay(20, MS); */
            }
            /* get status info */
            else {
                if (!m_bLoopEnable) {
                    /* for debug module */
                    CDelay::Delay(1, S);
                    continue;
                }

                if (__IsNeedRefresh()) {
                    if (GetStatusData()) {

                        m_uiCommFailCnt = 0;

                        if (m_bCommFailFlag && ClearCommFailAlarm()) {
                            m_bCommFailFlag = false;
                        }
                    } else {
                        m_uiCommFailCnt++;
                        if (m_uiCommFailCnt >= DEF_COMM_FAIL_COUNT) {
                            if (!m_bCommFailFlag) {
                                m_bCommFailFlag = SendCommFailAlarm();
                            }
                            m_uiCommFailCnt = DEF_COMM_FAIL_COUNT;
                        }
                    }
                    __StartTime();
                }

                /* get other info */
                DoSomething();

                CDelay::Delay(m_uiInterval, MS);
            }
        }

        if (m_bExitFlg) {
            break;
        }
    }

}

void
CDevThread::Run(void)
{

/* 1. create shared memory */
    CreateShm();

/* 2. get driver instance by device type */
    GetDriver();


/* 3. init cmd instance */
    InitCmd();

/* 4. wait until Mfg and status data is prepared OK */
    m_bReady = m_bReadyEventFlg = WaitInitOk();

    m_bInitCmpltFlg = true;

/* 5. loop for process set cmd and get status data */
    LoopProc();

/* 6. clear Shm and unregister cmd instance */
    ClearShm();

    DeleCmd();

    Exit(nullptr);

}

/* ---------------------------------Board--------------------------- */

bool
CBoardThread::WaitInitOk(void)
{
    /* set sub dev num to shm */
    SetSubDevNum();

    while (true) {
        /* get reset info and set it to shm */
        if (GetResetInfo()) {
            /* get mfg and set mfg to shm */
            if (GetMfg()) {
                /* get and set status data to shm */
                if (GetStatusData()) {
                    /* init OK, set state to shm */
                    CShmDevSta stState = {SHM_STATUS_OK, SHM_STATUS_NONE, SHM_STATUS_NONE};
                    CShmOperator::Set(m_staShm.at(SHM_TYPE_STA), m_staSem.at(SHM_TYPE_STA), (uint8_t *)&stState,
                                      CShmOperator::GetStaDevStaPos(), sizeof(stState));

                    break;

                }
            }
        }
        CDelay::Delay(1000, MS);
    }

    return true;

}


bool
CBoardThread::GetStatusData(void)
{

    CBoardStatusData stSta;
    memset(&stSta, 0, sizeof(stSta));
    /*return true; */

    if (m_bExitFlg) {
        return true;
    }

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {

        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataBoardStaPos(), sizeof(stSta));


        return true;
    }

    return false;

}

bool
CBoardThread::GetResetInfo(void)
{
    return true;
}

bool
CBoardThread::GetMfg(void)
{
    CBoardMfg stMfg;
    memset(&stMfg, 0x00, sizeof(stMfg));

    /* return true; */

    std::shared_ptr<CFan> stpFanDrvPtr;
    std::shared_ptr<CPsu> stpPsuDrvPtr;

    bool bFanMfgOk = false, bPsuMfgOk = false;

#if __USB_RELAOD_DEBUG__
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);
#endif

    /* get mfg */
    for (uint32_t uiRetryTimes = 0; uiRetryTimes < 3; uiRetryTimes++) {
        if (m_stpDrvPtr->GetMfg(stMfg)) {

            uint32_t uiFanIdx = 0;
            stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);

            while (nullptr != stpFanDrvPtr) {
                if (stpFanDrvPtr->IsOnline()) {
                    bFanMfgOk |= stMfg.abFanMfgOk[uiFanIdx];
                }

                uiFanIdx++;
                stpFanDrvPtr = CBoardCmd::GetFanPtr(uiFanIdx);
            }

            uint32_t uiPsuIdx = 0;
            stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);

            while (nullptr != stpPsuDrvPtr) {
                if (stpPsuDrvPtr->IsOnline() && stpPsuDrvPtr->IsOk()) {
                    bPsuMfgOk |= stMfg.abPowerMfgOk[uiPsuIdx];
                }

                uiPsuIdx++;
                stpPsuDrvPtr = CBoardCmd::GetPsuPtr(uiPsuIdx);
            }

            if (stMfg.bSccMfgOk || bFanMfgOk || bPsuMfgOk) {
                LOG_WRITE(EN_LOG_NOTICE, "CBoardThread::GetMfg ok!");

#if __USB_RELAOD_DEBUG__
                ofile << "CBoardThread::GetMfg ok!" << std::endl;
#endif
                break;
            } else {

                LOG_WRITE(EN_LOG_ERROR, "CBoardThread::GetMfg fail!");

#if __USB_RELAOD_DEBUG__
                ofile << "CBoardThreofad::GetMfg fail!" << std::endl;
                system("lsusb >> /home/oplink/log/hal-usb.log");
                ofile << std::endl;
                ofile << "reload usb!!!" << std::endl;
#endif
                system("echo -n 0000:00:1d.0|tee /sys/bus/pci/drivers/ehci-pci/unbind");
                CDelay::Delay(1, S);
                system("echo -n 0000:00:1d.0|tee /sys/bus/pci/drivers/ehci-pci/bind");
                CDelay::Delay(1, S);

#if __USB_RELAOD_DEBUG__
                system("lsusb >> /home/oplink/log/hal-usb.log");
                ofile << std::endl;
#endif

            }
        }
    }

#if __USB_RELAOD_DEBUG__
    ofile.close();
#endif

    /* set mfg to shm */
    CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                      CShmOperator::GetDataBoardMfgPos(), sizeof(stMfg));

    CCali stCali;
    memset(&stCali, 0x00, sizeof(stCali));

    if (m_stpDrvPtr->GetCali(EEPROM_CHASSIS, stCali)) {
        /* set cali to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stCali,
                          CShmOperator::GetDataChassisCaliPos(), sizeof(stCali));
    }

    return true;

}

void
CBoardThread::DoSomething(void)
{
    CMfg stMfg;

    uint32_t uiIdx = 0;
    static bool bOkOld[2] = {false, false};

    std::shared_ptr<CLogicDriver> stpLogicDrv = std::static_pointer_cast<CLogicDriver>(
        std::make_shared<CPcieBusLogic>(0, 0, 0, DATA_WIDTH_1B));

    if (m_bExitFlg) {
        return;
    }

    /* Fan */
    std::shared_ptr<CFan> stpFanDrvPtr;

    for (uiIdx = 0; uiIdx < m_staFanPtrs.size(); uiIdx++) {
        stpFanDrvPtr = m_staFanPtrs.at(uiIdx);
        if (nullptr == stpFanDrvPtr) {
            continue;
        }

        memset(&stMfg, 0x00, sizeof(CMfg));
        if (stpFanDrvPtr->IsPlug()) {
            /* EQA-22 set scc sys led amber, blink */
            stpLogicDrv->WriteReg(0x1068, 3);
            stpLogicDrv->WriteReg(0x106c, 1);

            CDelay::Delay(50, MS);
            if (stpFanDrvPtr->IsOnline()) {
                for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
                    if (stpFanDrvPtr->GetMfg(stMfg)) {
                        break;
                    }
                    CDelay::Delay(50, MS);
                }
                LOG_WRITE(EN_LOG_NOTICE, "fan #%d plug in.", uiIdx);
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "fan #%d plug out.", uiIdx);
            }

            stpFanDrvPtr->ClearPlug();

            CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                              CShmOperator::GetDataFanMfgPos(uiIdx), sizeof(stMfg));

            CEvent::SendEvent((char *)DEF_CMPTCTRLD_EVENT_SOCKET_PATH, DEF_EVENT_ID_FAN, 0);
            /* EQA-22 set scc sys led green */
            stpLogicDrv->WriteReg(0x1068, 2);
            stpLogicDrv->WriteReg(0x106c, 0);
        }
    }


    /* Psu */
    std::shared_ptr<CPsu> stpPsuDrvPtr;


    for (uiIdx = 0; uiIdx < m_staPsuPtrs.size(); uiIdx++) {
        stpPsuDrvPtr = m_staPsuPtrs.at(uiIdx);
        if (nullptr == stpPsuDrvPtr) {
            continue;
        }

        memset(&stMfg, 0x00, sizeof(CMfg));
        if (stpPsuDrvPtr->IsPlug()) {
            /* EQA-22 set scc sys led amber, blink */
            stpLogicDrv->WriteReg(0x1068, 3);
            stpLogicDrv->WriteReg(0x106c, 1);

            CDelay::Delay(1, S);
            if (stpPsuDrvPtr->IsOnline() && stpPsuDrvPtr->IsOk()) {
                for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
                    if (stpPsuDrvPtr->GetMfg(stMfg)) {
                        break;
                    }
                    CDelay::Delay(50, MS);
                }
                /* LOG_WRITE(EN_LOG_NOTICE, "psu #%d plug in.", uiIdx); */
                /* printf("psu #%d plug in.\n", uiIdx); */
            } else {
                /* LOG_WRITE(EN_LOG_NOTICE, "psu #%d plug out.", uiIdx); */
                /* printf("psu #%d plug out.\n", uiIdx); */
            }

            stpPsuDrvPtr->ClearPlug();

            CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                              CShmOperator::GetDataPowerMfgPos(uiIdx), sizeof(stMfg));

            CEvent::SendEvent((char *)DEF_CMPTCTRLD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PWR, 0);

            /* EQA-22 set scc sys led green */
            stpLogicDrv->WriteReg(0x1068, 2);
            stpLogicDrv->WriteReg(0x106c, 0);
        }


        if ((uiIdx < sizeof(bOkOld)) && stpPsuDrvPtr->IsOnline() && stpPsuDrvPtr->IsOk()) {
            if ((bOkOld[uiIdx] != stpPsuDrvPtr->IsOk())) {
                LOG_WRITE(EN_LOG_NOTICE, "Psu[%d] of %d IsOk, isOkOld %d, %lu, need GetMfg", uiIdx, stpPsuDrvPtr->IsOk(), bOkOld[uiIdx], sizeof(bOkOld));
                for (uint32_t uiTimes = 0; uiTimes < 3; uiTimes++) {
                    if (stpPsuDrvPtr->GetMfg(stMfg)) {
                        break;
                    }
                    CDelay::Delay(50, MS);
                }
                CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                                  CShmOperator::GetDataPowerMfgPos(uiIdx), sizeof(stMfg));

                CEvent::SendEvent((char *)DEF_CMPTCTRLD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PWR, 0);
            }
        }

        bOkOld[uiIdx] = stpPsuDrvPtr->IsOk();
    }

#if 0
    /* default password */
    bool bReset = false;
    std::shared_ptr<CLogic> stpLogicPtr = m_staLogicPtrs.at(0);

    stpLogicPtr->ResetPwd(bReset);
    if (bReset) {
        CEvent::SendEvent((char *)DEF_PERSISTD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PWD_RESET, 0);
    }
#endif

}

void
CBoardThread::SetSubDevNum(void)
{

#if 0
    CShmSubSta astSta[] =
    {
        {DEV_TYPE_FPGA, (uint32_t)m_stSubDevInfo.staLogicInfo.size()},
        /* {DEV_TYPE_CPLD, (uint32_t)m_stSubDevInfo.staCpldInfo.size()}, */
        {DEV_TYPE_LED, (uint32_t)m_stSubDevInfo.staLedInfo.size()}
    };

    CShmOperator::Set(m_staShm.at(SHM_TYPE_STA), m_staSem.at(SHM_TYPE_STA), (uint8_t *)&astSta,
                      CShmOperator::GetStaSubStaPos(0), sizeof(astSta));
#endif
}

void
CBoardThread::InitSubDev(void)
{
    /* CPLD and FPGA driver must be registered to CBoardCmd first */
    LOG_WRITE(EN_LOG_NOTICE, "InitSubDev +++");

    for (uint32_t i = 0; i < m_stSubDevInfo.staLogicDrvInfo.size(); i++) {

        std::shared_ptr<CLogicDriver> stpLogicDrvPtr =
            CDevDrvManager::GetInstance().GetLogicDrv(m_stSubDevInfo.staLogicDrvInfo.at(i));

        m_staLogicDrvPtrs.push_back(stpLogicDrvPtr);
        CBoardCmd::AddLogicDrvPtr(stpLogicDrvPtr);
    }

#if 0
    /* reset usb link to pa, a workaround for usb-to-i2c can not connect, it may cause main thread block. */
    m_staLogicDrvPtrs[0]->WriteBit(0x58, BIT_1, 0);

    CDelay::Delay(1, S);

    m_staLogicDrvPtrs[0]->WriteBit(0x58, BIT_1, 1);
    LOG_WRITE(EN_LOG_NOTICE, "Reset Usb PA.");
#endif


    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) logic driver.", m_staLogicDrvPtrs.size());
    printf("Init (%d) logic driver.\n", (int)m_staLogicDrvPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staI2cSwitchInfo.size(); i++) {
        m_staI2cSwitchPtrs.push_back(
            CDevDrvManager::GetInstance().GetI2cSwitch(m_stSubDevInfo.staI2cSwitchInfo.at(i)));
    }

    CBoardCmd::SetI2cSwitchPtr(m_staI2cSwitchPtrs);


    LOG_WRITE(EN_LOG_NOTICE, "Init (%d) I2c Switch.", (int)m_staI2cSwitchPtrs.size());

    printf("Init (%d) I2c Switch.\n", (int)m_staI2cSwitchPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staUartSwitchInfo.size(); i++) {
        m_staUartSwitchPtrs.push_back(
            CDevDrvManager::GetInstance().GetUartSwitch(m_stSubDevInfo.staUartSwitchInfo.at(i)));
    }

    CBoardCmd::SetUartSwitchPtr(m_staUartSwitchPtrs);


    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Uart Switch.", m_staUartSwitchPtrs.size());

    printf("Init (%d) Uart Switch.\n", (int)m_staUartSwitchPtrs.size());

    /* I2c */
    for (uint32_t i = 0; i < m_stSubDevInfo.staI2cInfo.size(); i++) {
        std::shared_ptr<CI2c> stpI2cPtr = nullptr;

        stpI2cPtr = CDevDrvManager::GetInstance().GetI2cDrv(m_stSubDevInfo.staI2cInfo.at(i));
        m_staI2cPtrs.push_back(stpI2cPtr);

        CBoardCmd::AddI2cPtr(stpI2cPtr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) I2c Device.", m_staI2cPtrs.size());

    printf("Init (%d) I2c Device.\n", (int)m_staI2cPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staUartInfo.size(); i++) {
        std::shared_ptr<CUartDriver> stpUartPtr = nullptr;

        stpUartPtr = CDevDrvManager::GetInstance().GetUartDrv(m_stSubDevInfo.staUartInfo.at(i));
        m_staUartPtrs.push_back(stpUartPtr);

        CBoardCmd::AddUartPtr(stpUartPtr);
    }

    /* Fan-Logic Driver */
    for (uint32_t i = 0; i < m_stSubDevInfo.staFanLogicDrvInfo.size(); i++) {
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr =
            CDevDrvManager::GetInstance().GetLogicDrv(m_stSubDevInfo.staFanLogicDrvInfo.at(i));

        m_staLogicDrvPtrs.push_back(stpLogicDrvPtr);
        CBoardCmd::AddLogicDrvPtr(stpLogicDrvPtr);
    }
    LOG_WRITE(EN_LOG_NOTICE, "logic driver info (%lu)", m_staLogicDrvPtrs.size());
    printf("logic driver info (%d).\n", (int)m_staLogicDrvPtrs.size());


    /* Apsd */
    for (uint32_t i = 0; i < m_stSubDevInfo.staApsdInfo.size(); i++) {
        m_staApsdPtrs.push_back(
            CDevDrvManager::GetInstance().GetApsd(m_stSubDevInfo.staApsdInfo.at(i)));
    }

    CBoardCmd::SetApsdPtr(m_staApsdPtrs);

    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Apsd Board.", m_staApsdPtrs.size());

    printf("Init (%d) Apsd Board.\n", (int)m_staApsdPtrs.size());

    /* WatchDog */
    for (uint32_t i = 0; i < m_stSubDevInfo.staWatchDogInfo.size(); i++) {
        m_staWatchDogPtrs.push_back(
            CDevDrvManager::GetInstance().GetWatchDog(m_stSubDevInfo.staWatchDogInfo.at(i)));
    }

    CBoardCmd::SetWatchDogPtr(m_staWatchDogPtrs);

    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Watch Dog Board.", m_staWatchDogPtrs.size());

    printf("Init (%d) Watch Dog Board.\n", (int)m_staWatchDogPtrs.size());

    /* Reset Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staResetBoardInfo.size(); i++) {
        m_staResetBoardPtrs.push_back(
            CDevDrvManager::GetInstance().GetResetBoard(m_stSubDevInfo.staResetBoardInfo.at(i)));
    }
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Reset Board.", m_staResetBoardPtrs.size());


    printf("Init (%d) Reset Board.\n", (int)m_staResetBoardPtrs.size());

    CBoardCmd::SetResetBoardPtr(m_staResetBoardPtrs);

    /* ResetType Register*/
    for (uint32_t i = 0; i < m_stSubDevInfo.staResetTypeInfo.size(); i++) {
        m_staResetTypeRegisterPtrs.push_back(
            CDevDrvManager::GetInstance().GetResetTypeRegister(m_stSubDevInfo.staResetTypeInfo.at(i)));
    }

    CBoardCmd::SetResetTypeRegister(m_staResetTypeRegisterPtrs);

    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Reset Board.", m_staResetTypeRegisterPtrs.size());

    /* Fan Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staFanBoardInfo.size(); i++) {
        std::shared_ptr<CFanBoard> stpFanBoardPtr =
            CDevDrvManager::GetInstance().GetFanBoard(m_stSubDevInfo.staFanBoardInfo.at(i));
        if (stpFanBoardPtr != nullptr) {
            m_staFanBoardPtrs.push_back(stpFanBoardPtr);
        }
    }
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Fan Board.", m_staFanBoardPtrs.size());

    CBoardCmd::SetFanBoardPtr(m_staFanBoardPtrs);

    /* Psu Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staPsuBoardInfo.size(); i++) {
        m_staPsuBoardPtrs.push_back(
            CDevDrvManager::GetInstance().GetPsuBoard(m_stSubDevInfo.staPsuBoardInfo.at(i)));
    }

    CBoardCmd::SetPsuBoardPtr(m_staPsuBoardPtrs);

    /* Led Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staLedBoardInfo.size(); i++) {
        m_staLedBoardPtrs.push_back(
            CDevDrvManager::GetInstance().GetLedBoard(m_stSubDevInfo.staLedBoardInfo.at(i)));
    }

    CBoardCmd::SetLedBoardPtr(m_staLedBoardPtrs);

    /* Osc Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staOscBoardInfo.size(); i++) {
        m_staOscBoardPtrs.push_back(
            CDevDrvManager::GetInstance().GetOscBoard(m_stSubDevInfo.staOscBoardInfo.at(i)));
    }

    CBoardCmd::SetOscBoardPtr(m_staOscBoardPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Osc Board.", m_staOscBoardPtrs.size());

    /* Slot Board */
    for (uint32_t i = 0; i < m_stSubDevInfo.staSlotBoardInfo.size(); i++) {
        m_staSlotBoardPtrs.push_back(
            CDevDrvManager::GetInstance().GetSlotBoard(m_stSubDevInfo.staSlotBoardInfo.at(i)));
    }

    CBoardCmd::SetSlotBoardPtr(m_staSlotBoardPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Slot Board.", m_staSlotBoardPtrs.size());

    /* Logic */
    for (uint32_t i = 0; i < m_stSubDevInfo.staLogicInfo.size(); i++) {
        std::shared_ptr<CLogic> stpLogicPtr = CDevDrvManager::GetInstance().GetLogic(m_stSubDevInfo.staLogicInfo.at(i));
        m_staLogicPtrs.push_back(stpLogicPtr);
        CBoardCmd::AddLogicPtr(stpLogicPtr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Logic.", m_staLogicPtrs.size());

    /* EEprom */
    for (uint32_t i = 0; i < m_stSubDevInfo.staEEpromInfo.size(); i++) {
        m_staEEpromPtrs.push_back(
            CDevDrvManager::GetInstance().GetEEpromDrv(m_stSubDevInfo.staEEpromInfo.at(i)));
    }

    CBoardCmd::SetEEpromPtr(m_staEEpromPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) EEPROM.", m_staEEpromPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staFlashInfo.size(); i++) {
        m_staFlashPtrs.push_back(CDevDrvManager::GetInstance().GetFlashDrv(m_stSubDevInfo.staFlashInfo.at(i)));
    }

    CBoardCmd::SetFlashPtr(m_staFlashPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Flash.", m_staFlashPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staLedInfo.size(); i++) {
        m_staLedPtrs.push_back(CDevDrvManager::GetInstance().GetLedDrv(m_stSubDevInfo.staLedInfo.at(i)));
    }

    CBoardCmd::SetLedPtr(m_staLedPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Led.", m_staLedPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staTempInfo.size(); i++) {
        m_staTempPtrs.push_back(CDevDrvManager::GetInstance().GetTempDrv(m_stSubDevInfo.staTempInfo.at(i)));
    }

    CBoardCmd::SetTempPtr(m_staTempPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Temperatrue Sensor.", m_staTempPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staFanInfo.size(); i++) {
        m_staFanPtrs.push_back(CDevDrvManager::GetInstance().GetFanDrv(m_stSubDevInfo.staFanInfo.at(i)));
    }

    CBoardCmd::SetFanPtr(m_staFanPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Fan.", m_staFanPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staPsuInfo.size(); i++) {
        m_staPsuPtrs.push_back(CDevDrvManager::GetInstance()
                               .GetPsuDrv(m_stSubDevInfo.staPsuInfo.at(i) /*, m_stSubDevInfo.staPsuEeInfo.at(i), m_stSubDevInfo.staPsuInfo.at(i)*/));
    }

    CBoardCmd::SetPsuPtr(m_staPsuPtrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) PSU.", m_staPsuPtrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staMdioInfo.size(); i++) {
        m_stpMdioDrvPtr.push_back(CDevDrvManager::GetInstance().GetMdioDrv(m_stSubDevInfo.staMdioInfo.at(i)));
    }
    CBoardCmd::SetMdioDrvPtr(m_stpMdioDrvPtr);

    for (uint32_t i = 0; i < m_stSubDevInfo.staNetSwitchInfo.size(); i++) {
        m_stpNetSwitchPtr.push_back(CDevDrvManager::GetInstance().GetNetSwitchDrv(m_stSubDevInfo.staNetSwitchInfo.at(i)));
    }
    CBoardCmd::SetNetSwitchPtr(m_stpNetSwitchPtr);
    LOG_WRITE(EN_LOG_NOTICE, "Init Net Switch.");

    for (uint32_t i = 0; i < m_stSubDevInfo.staPhyInfo.size(); i++) {
        m_stpPhyPtr.push_back(CDevDrvManager::GetInstance().GetPhyDrv(m_stSubDevInfo.staPhyInfo.at(i)));
    }
    CBoardCmd::SetPhyPtr(m_stpPhyPtr);

    LOG_WRITE(EN_LOG_NOTICE, "Init Ltc4215 size: %d", m_stSubDevInfo.staLtc4215Info.size());
    for (uint32_t i = 0; i < m_stSubDevInfo.staLtc4215Info.size(); i++) {
        m_staLtc4215Ptrs.push_back(CDevDrvManager::GetInstance().GetLtc4215Drv(m_stSubDevInfo.staLtc4215Info.at(i)));
    }
    CBoardCmd::SetLtc4215Ptr(m_staLtc4215Ptrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%d) Ltc4215 Sensor.", m_staLtc4215Ptrs.size());

    LOG_WRITE(EN_LOG_NOTICE, "Init Mp5023 size: %d", m_stSubDevInfo.staMp5023Info.size());
    for (uint32_t i = 0; i < m_stSubDevInfo.staMp5023Info.size(); i++) {
        m_staMp5023Ptrs.push_back(CDevDrvManager::GetInstance().GetMp5023Drv(m_stSubDevInfo.staMp5023Info.at(i)));
    }
    CBoardCmd::SetMp5023Ptr(m_staMp5023Ptrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%d) Mp5023 Sensor.", m_staMp5023Ptrs.size());

    for (uint32_t i = 0; i < m_stSubDevInfo.staMax1139Info.size(); i++) {
        m_staMax1139Ptrs.push_back(CDevDrvManager::GetInstance().GetMax1139Drv(m_stSubDevInfo.staMax1139Info.at(i)));
    }
    CBoardCmd::SetMax1139Ptr(m_staMax1139Ptrs);
    LOG_WRITE(EN_LOG_NOTICE, "Init (%lu) Max1139 Sensor.", m_staMax1139Ptrs.size());

    m_stpDrvPtr = CDevDrvManager::GetInstance().GetBoardDrv();
    CBoardCmd::SetBoardPtr(m_stpDrvPtr);
    LOG_WRITE(EN_LOG_NOTICE, "Init Board Driver.");

}

void
CBoardThread::Run(void)
{
    prctl(PR_SET_NAME, "hal_BoardThread");
/* 1. create shared memory for Board */
    CreateShm(mc_staShmCfg);
    LOG_WRITE(EN_LOG_NOTICE, "Create board shared memory.");

/* 2. init board and sub dev info */

    InitSubDev();
    LOG_WRITE(EN_LOG_NOTICE, "Init sub dev.");

    m_stpDrvPtr = CDevDrvManager::GetInstance().GetBoardDrv();

    LOG_WRITE(EN_LOG_NOTICE, "Set Board Driver.");

/* 3. init Board cmd instance */

    /* CBoardCmd::SetDevPtr(m_stpDrvPtr); */
    CBoardCmd::SetShm(m_staShm, m_staSem);


#if 1
/* 4. config network switch */
    LOG_WRITE(EN_LOG_NOTICE, "Net Switch Init +++");
    CBoardCmd::GetNetSwitchPtr(0)->Init();
    LOG_WRITE(EN_LOG_NOTICE, "Net Switch Init ---");
#endif

    /*CBoardCmd::GetPhyPtr(0)->Config(); */

/* 5. wait until Mfg and status data is prepared OK */
    m_bReady = WaitInitOk();
    m_bInitCmpltFlg = true;

/* 6. loop for process set cmd and get status data */
    LOG_WRITE(EN_LOG_NOTICE, "Board Loop Start.");
    LoopProc();

}

/* ---------------------------------OA------------------------------ */
bool
COaThread::GetStatusData(void)
{
    /*CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PA, 1);*/
    LOG_WRITE(EN_LOG_DEBUG, "COaThread::GetStatusData +++ %lu", pthread_self());
    std::vector<COaStatusData> staSta;

    if (m_bExitFlg) {
        return true;
    }

    /* get status data */
    if (!m_stpDrvPtr->GetStatusData(staSta)) {
        return false;
    }

    for (uint32_t i = 0; i < staSta.size(); i++) {
        COaAlarm stOaAlarm = {0};
        CShmOperator::Get(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stOaAlarm,
                          CShmOperator::GetDataOaAlmPos(i), sizeof(COaAlarm));


        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&(staSta.at(i)),
                          CShmOperator::GetDataOaStaPos(i), sizeof(COaStatusData));


        int32_t iCmpResult = memcmp((void *)&staSta.at(i).stOaAlm, (void *)&stOaAlarm, sizeof(COaAlarm));

        if (0 != iCmpResult) {
            /* LOG_WRITE(EN_LOG_WARNING, "Oa(%d) Send Alarm.", m_uiDevIdx); */
            if (staSta.at(i).stOaAlm.bInLos) {
                /* LOG_WRITE(EN_LOG_WARNING, "sInPower = %d.", staSta.at(i).stOaCurData.sInPower); */
            }

            if (staSta.at(i).stOaAlm.bRFL) {
                /* LOG_WRITE(EN_LOG_WARNING, "sBackRefPower = %d.", staSta.at(i).stOaCurData.sBackRefPower); */
            }

            if (staSta.at(i).stOaAlm.abPumpTemp[0]) {
                /* LOG_WRITE(EN_LOG_WARNING, "asPumpTemp = %d.", staSta.at(i).stOaCurData.asPumpTemp[0]); */
            }

            if (staSta.at(i).stOaAlm.bCaseTemp) {
                /* LOG_WRITE(EN_LOG_WARNING, "asCaseTemp = %d.", staSta.at(i).stOaCurData.asCaseTemp[0]); */
            }

            CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PA, i);
        }
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaThread::GetStatusData --- %lu", pthread_self());

    return true;

}

bool
COaThread::GetMfg(void)
{
    CMfg stMfg;

    std::cout << "COaThread::GetMfg" << std::endl;

    if (m_stpDrvPtr == nullptr) {

        std::cout << "m_stpDrvPtr == nullptr" << std::endl;
    }

    memset(&stMfg, 0x00, sizeof(stMfg));

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOaMfgPos(), sizeof(stMfg));

        return true;
    }

#if __USB_RELAOD_DEBUG__
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);

    ofile << "COaThread::GetMfg fail!" << std::endl;
    system("lsusb >> /home/oplink/log/hal-usb.log");
    ofile << std::endl;
    ofile.close();
#endif

    return false;

}

void
COaThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OaThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOaDrv(m_staDevInfo);
}

/* --------------------------------WSS------------------------------ */
bool
CWssThread::GetStatusData(void)
{

    CWssStatusData stSta;

    if (m_bExitFlg) {
        return true;
    }

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_NOTICE, "wss drv is nullptr.");
        return false;
    }

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataWssStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CWssThread::GetMfg(void)
{

    CMfg stMfg;

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_NOTICE, "wss drv is nullptr.");
        return false;
    }

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataWssMfgPos(), sizeof(stMfg));

        CWssAllCfg stCfgData;
        if (m_stpDrvPtr->GetCfgData(stCfgData)) {
            CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stCfgData,
                              CShmOperator::GetDataWssCfgPos(), sizeof(stCfgData));
        }

        return true;
    }

#if __USB_RELAOD_DEBUG__

    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);
    ofile << "CWssThread::GetMfg fail!" << std::endl;
    ofile.close();
#endif

    return false;

}

void
CWssThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_WssThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetWssDrv(m_staDevInfo);
}

/* --------------------------------OCM------------------------------ */
bool
COcmThread::GetStatusData(void)
{

    COcmStatusData stSta;

    if (m_bExitFlg) {
        return true;
    }

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataOcmStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
COcmThread::GetMfg(void)
{

    CMfg stMfg;

    std::cout << "COcmThread::GetMfg" << std::endl;

    if (m_stpDrvPtr == nullptr) {

        std::cout << "m_stpDrvPtr == nullptr" << std::endl;
    }

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOcmMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
COcmThread::DoSomething(void)
{
    COcmChPower stReport;
    COcmOsa stOsa;

    uint32_t uiPortCnt = m_stpDrvPtr->GetPortCnt();

    LOG_WRITE(EN_LOG_DEBUG, "DoSomething +++");

    if (m_uiPortId > uiPortCnt || m_uiPortId < 1) {
        m_uiPortId = 1;
    }

    /* osa data */
    memset(&stOsa, 0, sizeof(stOsa));
    if (m_stpDrvPtr->GetOsa(m_uiPortId, stOsa)) {
        /* set osa data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stOsa,
                          CShmOperator::GetDataOcmOsaPos(m_uiPortId), sizeof(stOsa));
    }

    if (m_bExitFlg) {
        return;
    }

    /* channel power data */
    memset(&stReport, 0, sizeof(stReport));
    if (m_stpDrvPtr->GetChPower(m_uiPortId, stReport)) {
        /*for(int i = 0;i < 65;i++){
           LOG_WRITE(EN_LOG_NOTICE, "stReport.asPower[%d]: %d", i+1, stReport.asPower[i]);
           }*/
        /* set report data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stReport,
                          CShmOperator::GetDataOcmRptPos(m_uiPortId), sizeof(stReport));
    }

    m_uiPortId++;

    LOG_WRITE(EN_LOG_DEBUG, "DoSomething ---");
}

void
COcmThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OcmThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOcmDrv(m_staDevInfo);
}

/* -------------------------------TDCM------------------------------ */
bool
CTdcmThread::GetStatusData(void)
{

    CTdcmStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataTdcmStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CTdcmThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataTdcmMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}


void
CTdcmThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_TdcmThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetTdcmDrv(m_staDevInfo);
}

/* -------------------------------PAM4------------------------------ */

bool
CPam4Thread::GetStatusData(void)
{

    CPam4StatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataPam4StaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CPam4Thread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataPam4MfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
CPam4Thread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_Pam4Thread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetPam4Drv(m_staDevInfo);
}

/* --------------------------------OPS------------------------------ */
COpsThread::COpsThread() : m_threadQueue(0), m_threadInt(0)
{

}

COpsThread::~COpsThread()
{
    m_nl.Close();
    pthread_join(m_threadQueue, NULL);
    pthread_join(m_threadInt, NULL);
}

void *
COpsThread::FastCmdProc(void *pvArg)
{
    COpsThread *p = (COpsThread *)pvArg;
    CShareMemory &shm = p->m_staShm.at(SHM_TYPE_CMD);
    CSemaphore &sem = p->m_staSem.at(SHM_TYPE_CMD);
    CQueueNode stQNode;

    while (!p->m_bExitFlg) {
        /* get the fast cmd node from shm queue */
        if (CShmOperator::GetFastCmd(shm, sem, stQNode)) {
            CDevCmdProcesser::GetInstance().CmdProc(shm, sem, stQNode);
        }
        usleep(200);
    }

    return NULL;
}

void *
COpsThread::InterruptProc(void *pvArg)
{
    COpsThread *p = (COpsThread *)pvArg;
    const int NETLINK_GROUP = 21;

    if (!p->m_nl.Open(NETLINK_USERSOCK, NETLINK_GROUP)) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to open netlink.");
        return NULL;
    }

    int lens = 0;
    while (!p->m_bExitFlg) {
        if (p->m_nl.Read(NULL, 0, lens)) {
            p->UpdateBidiStatus();
        }
    }
    p->m_nl.Close();
    return NULL;
}

bool
COpsThread::UpdateBidiStatus(void)
{
    m_stpDrvPtr->m_bInterruptFlag = true;

    COpsStatusData sta[OPS4_OPS_CNT_MAX];     /* todo here need to update for fnc */
    /*memset(&stSta, 0, sizeof(stSta)); */
    if (!m_stpDrvPtr->GetRequestStatus(sta[0].stBidiStatus)) {
        LOG_WRITE(EN_LOG_ERROR, "COpsThread::UpdateBidiStatus error");
        m_stpDrvPtr->m_bInterruptFlag = false;
        return false;
    }

    m_stpDrvPtr->m_bInterruptFlag = false;

    LOG_WRITE(EN_LOG_NOTICE, "COpsThread::UpdateBidiStatus, pos = %u, sta = %u",
              sta[0].stBidiStatus.ucLocalPosition, sta[0].stBidiStatus.ucLocalStatus);

    /*OC2A-278: invalid sta.stCurData.ucSwitchPos results in incorrect switch pos for calling SendAlarm() as following. */
    /*when opsd receive DEF_EVENT_ID_OPS_SW_HAPPEND event due to COpsThread::UpdateBidiStatus(), */
    /*COpsStatusData is not updated from OPS module in real time. */
    /*so skip sending DEF_EVENT_ID_OPS_SW_HAPPEND when COpsThread::UpdateBidiStatus() is called. */
    /*ONLY send DEF_EVENT_ID_OPS_SW_HAPPEND event by COpsThread::GetStatusData(), in that case COpsStatusData is ready. */
    uint32_t uiPortCnt = m_stpDrvPtr->GetPortCnt();
    for (uint32_t i = 1; i <= uiPortCnt; i++) {
        sta[i - 1].stCurData.ucSwitchPos = OPS_PATH_CNT;
    }

    COpsCommStatusData stCommData;
    memset(&stCommData, 0, sizeof(COpsCommStatusData));
    if (!m_stpDrvPtr->GetCommonStatusData(stCommData)) {
        return false;
    }
    m_stpDrvPtr->SendAlarm(stCommData, sta);
    return true;
}

bool
COpsThread::GetStatusData(void)
{
    uint32_t uiPortCnt = m_stpDrvPtr->GetPortCnt();

    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData +++");

    if (m_bExitFlg) {
        return true;
    }

    if (OPS1_OPS_CNT_MAX != uiPortCnt && OPS4_OPS_CNT_MAX != uiPortCnt) {
        return false;
    }

    /* get status data */
    COpsStatusData stSta[OPS4_OPS_CNT_MAX];
    memset(&stSta, 0, sizeof(stSta));
    for (uint32_t i = 1; i <= uiPortCnt; i++) {
        /* EQA-22 optimize the time when plugout OPS4 */
        if (m_bExitFlg) {
            break;
        }
        if (!m_stpDrvPtr->GetStatusData(stSta[i - 1], i)) {
            continue;
        }
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta[i - 1],
                          CShmOperator::GetDataOpsStaPos(i), sizeof(COpsStatusData));
        LOG_WRITE(EN_LOG_DEBUG, "%d, pri: %d, sec: %d, com-in: %d, com-out: %d",
                  i,
                  stSta[i - 1].stCurData.asPortPower[OPS_PORT_LINE1_IN],
                  stSta[i - 1].stCurData.asPortPower[OPS_PORT_LINE2_IN],
                  stSta[i - 1].stConnectionPds.asPd[OPS_R],
                  stSta[i - 1].stConnectionPds.asPd[OPS_T]);
    }

    /* EQA-22 optimize the time when plugout OPS4 */
    if (m_bExitFlg) {
        return false;
    }

    COpsCommStatusData stCommData;
    memset(&stCommData, 0, sizeof(COpsCommStatusData));
    if (!m_stpDrvPtr->GetCommonStatusData(stCommData)) {
        return false;
    }

    /* set common status data to shm */
    CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stCommData,
                      CShmOperator::GetCommonDataOpsStaPos(), sizeof(COpsCommStatusData));
    m_stpDrvPtr->SendAlarm(stCommData, stSta);

    return true;


}

bool
COpsThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOpsMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
COpsThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OpsThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOpsDrv(m_staDevInfo);

    /* Set the OPS status loop interval time to 20ms. */
    /* CDevThread::SetStatusInterval(20) */

    /* A special thread to processing high priority command. */
    InitCmd();
    LOG_WRITE(EN_LOG_NOTICE, "Create COpsThread::FastCmdProc thread.");
    if (0 != pthread_create(&m_threadQueue, NULL, COpsThread::FastCmdProc, (void *)this)) {
        LOG_WRITE(EN_LOG_ERROR, "create COpsThread::FastCmdProc thread failed.");
    }

    /* A special thread to processing switch interrupt command. */
    LOG_WRITE(EN_LOG_NOTICE, "Create COpsThread::InterruptProc thread.");
    if (0 != pthread_create(&m_threadInt, NULL, COpsThread::InterruptProc, (void *)this)) {
        LOG_WRITE(EN_LOG_ERROR, "create COpsThread::InterruptProc thread failed.");
    }
}

/* --------------------------------DCE------------------------------ */

bool
CDceThread::GetStatusData(void)
{

    CDceStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataDceStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CDceThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataDceMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
CDceThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_DceThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetDceDrv(m_staDevInfo);
}

/* --------------------------------DGE------------------------------ */

bool
CDgeThread::GetStatusData(void)
{

    CDgeStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataDgeStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}


bool
CDgeThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataDgeMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
CDgeThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_DgeThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetDgeDrv(m_staDevInfo);
}

/* --------------------------------MCS------------------------------ */
bool
CMcsThread::GetStatusData(void)
{

    CMcsStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataMcsStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CMcsThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataMcsMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
CMcsThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_McsThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetMcsDrv(m_staDevInfo);
}

/* --------------------------------OSC------------------------------ */
bool
COscThread::GetStatusData(void)
{

    COscStatusData stSta;

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpDrvPtr");
        return false;
    }

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataOscStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
COscThread::GetMfg(void)
{

    CMfg stMfg;

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpDrvPtr");
        return false;
    }

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOscMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
COscThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OscThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOscDrv(m_staDevInfo);
}

/* -------------------------------OTDR------------------------------ */
bool
COtdrThread::GetStatusData(void)
{
#if 0 /* no need loop status */
    COtdrStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataOtdrStaPos(), sizeof(stSta));

        return true;
    }

    return false;
#endif

    if (m_bInitCmpltFlg && !m_bReady) {
        m_bReady = m_bReadyEventFlg = GetMfg();
    } else {

#if 1
        COtdrStatusData stSta;
        m_stpDrvPtr->GetStatusData(stSta);
#endif
    }

    if (m_bReadyEventFlg) {
        if (CEvent::SendEvent((char *)DEF_OPERATION_EVENT_SOCKET_PATH, DEF_EVENT_ID_OTDR, 0)) {
            m_bReadyEventFlg = false;
            LOG_WRITE(EN_LOG_NOTICE, "Send OTDR ready event ok.");
        }
    }

    return true;
}

bool
COtdrThread::GetMfg(void)
{
    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOtdrMfgPos(), sizeof(stMfg));

        CShmDevSta stState = {SHM_STATUS_OK, SHM_STATUS_NONE, SHM_STATUS_NONE};
        CShmOperator::Set(m_staShm.at(SHM_TYPE_STA), m_staSem.at(SHM_TYPE_STA), (uint8_t *)&stState,
                          CShmOperator::GetStaDevStaPos(), sizeof(stState));

        return true;
    }

    return false;
}

void
COtdrThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OtdrThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOtdrDrv(m_staDevInfo);
}


/* -------------------------------MUX------------------------------ */
bool
CMuxThread::GetStatusData(void)
{

    CMuxStatusData stSta;

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataMuxStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CMuxThread::GetMfg(void)
{

    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    /* get mfg */
    if (false == m_stpDrvPtr->GetMfg(stMfg)) {
        return false;
    }

    /* set mfg to shm */
    CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                      CShmOperator::GetDataMuxMfgPos(), sizeof(stMfg));

    CMuxCaliData stCali;
    memset(&stCali, 0, sizeof(stCali));

    if (false == m_stpDrvPtr->GetCali(stCali)) {
        return false;
    }

    /* set cali to shm */
    CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stCali,
                      CShmOperator::GetDataMuxCaliPos(), sizeof(stCali));

    return true;

}

void
CMuxThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_MuxThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetMuxDrv(m_staDevInfo);
}


/* -------------------------------EXTEND------------------------------ */
bool
CExtendThread::GetStatusData(void)
{

    CExtendStatusData stSta;

    if (m_bExitFlg) {
        return true;
    }

    /* get status data */
    if (m_stpDrvPtr->GetStatusData(stSta)) {
        /* set status data to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                          CShmOperator::GetDataExtendStaPos(), sizeof(stSta));

        return true;
    }

    return false;

}

bool
CExtendThread::GetMfg(void)
{

    CMfg stMfg;

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataExtendMfgPos(), sizeof(stMfg));
        /* to Resolve Extend Bug */
        /* m_stpDrvPtr->SetVoaMode(2, 0); */
        /* m_stpDrvPtr->SetVoaMode(4, 0); */

        return true;
    }

#if __USB_RELAOD_DEBUG__
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);

    ofile << "CExtendThread::GetMfg fail!" << std::endl;
    system("lsusb >> /home/oplink/log/hal-usb.log");
    ofile << std::endl;
    ofile.close();
#endif

    return false;

}

void
CExtendThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_ExtendThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetExtendDrv(m_staDevInfo);
}

/* --------------------------------OCS------------------------------ */
bool
COcsThread::GetStatusData(void)
{
    COcsStatusData stSta;
    COcsAlarm stAlm;

    if (m_bExitFlg) {
        return true;
    }

    /* get status data */
    memset(&stSta, 0, sizeof(stSta));
    if (!m_stpDrvPtr->GetStatusData(stSta)) {
        return false;
    }

    memset(&stAlm, 0, sizeof(stAlm));
    CShmOperator::Get(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stAlm,
                      CShmOperator::GetDataOcsAlmPos(), sizeof(stAlm));

    /* set status data to shm */
    CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stSta,
                      CShmOperator::GetDataOcsStaPos(), sizeof(stSta));

    if (0 != memcmp(&stAlm, &stSta.stAlm, sizeof(stAlm))) {
        LOG_WRITE(EN_LOG_DEBUG, "old(%hhd, %hhd, %hhd, %hhd, %hhd), new(%hhd, %hhd, %hhd, %hhd, %hhd)",
                  stAlm.abAlarms[OCS_ALARM_PS_VOL_HIGH], stAlm.abAlarms[OCS_ALARM_PS_VOL_LOW], stAlm.abAlarms[OCS_ALARM_SW_VOL_HIGH],
                  stAlm.abAlarms[OCS_ALARM_SW_VOL_LOW], stAlm.abAlarms[OCS_ALARM_CALI_DATA],
                  stSta.stAlm.abAlarms[OCS_ALARM_PS_VOL_HIGH], stSta.stAlm.abAlarms[OCS_ALARM_PS_VOL_LOW], stSta.stAlm.abAlarms[OCS_ALARM_SW_VOL_HIGH],
                  stSta.stAlm.abAlarms[OCS_ALARM_SW_VOL_LOW], stSta.stAlm.abAlarms[OCS_ALARM_CALI_DATA]);

        CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OCS, 0);
    }

    return true;

}

bool
COcsThread::GetMfg(void)
{
    CMfg stMfg;

    if (m_stpDrvPtr == nullptr) {
        LOG_WRITE(EN_LOG_ERROR, "%s, m_stpDrvPtr == nullptr", __func__);
        return false;
    }

    /* get mfg */
    if (m_stpDrvPtr->GetMfg(stMfg)) {
        /* set mfg to shm */
        CShmOperator::Set(m_staShm.at(SHM_TYPE_DATA), m_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                          CShmOperator::GetDataOcsMfgPos(), sizeof(stMfg));

        return true;
    }

    return false;

}

void
COcsThread::DoSomething(void)
{
    LOG_WRITE(EN_LOG_DEBUG, "DoSomething ---");
}

void
COcsThread::GetDriver(void)
{
    prctl(PR_SET_NAME, "hal_OcsThread");
    m_stpDrvPtr = CDevDrvManager::GetInstance().GetOcsDrv(m_staDevInfo);
}


