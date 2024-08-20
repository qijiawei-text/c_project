/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevThread.h
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
#include "Thread.h"
#include "ShmOperator.h"
#include "ShareMemory.h"
#include "Semaphore.h"

#include "OaDriver.h"
#include "WssDriver.h"
#include "OcmDriver.h"
#include "TdcmDriver.h"
#include "Pam4Driver.h"
#include "OpsDriver.h"
#include "DceDriver.h"
#include "DgeDriver.h"
#include "McsDriver.h"
#include "OscDriver.h"
#include "OtdrDriver.h"
#include "MuxDriver.h"
#include "ExtendDriver.h"
#include "Flash.h"
#include "OcsDriver.h"

#include "DevCmd.h"
#include "OaCmd.h"
#include "WssCmd.h"
#include "OcmCmd.h"
#include "TdcmCmd.h"
#include "OpsCmd.h"
#include "Pam4Cmd.h"
#include "DceCmd.h"

#include "DgeCmd.h"
#include "McsCmd.h"
#include "OscCmd.h"
#include "OtdrCmd.h"
#include "MuxCmd.h"
#include "ExtendCmd.h"
#include "OcsCmd.h"

#include "Event.h"
#include "NetLink.h"

/*************************************************/


#if 0
union CRegisterInfo
{
    uint32_t uiDevIdx;              /* e.g:EDFA0, EDFA1 */
    uint32_t uiHwType;         /* e.g:UART,I2C,SPI */

    struct {
        uint32_t uiUartIdx;      /* e.g:SPI0,SPI1 */
        uint32_t uiProto;
        uint32_t auiReserved[3];
    }    /* HW_TYPE_UART */

    struct {
        uint32_t uiUartIdx;      /* e.g:SPI0,SPI1 */
        uint32_t uiFpgaIdx;
        uint32_t uiChIdx;
        uint32_t uiProto;
        uint32_t auiReserved[3];
    }    /* HW_TYPE_UART */

    struct {
        uint32_t uiI2cIdx;
        uint32_t uiDevAddr;
        uint32_t auiReserved[3];
    }    /* HW_TYPE_I2C */

    struct {
        uint32_t uiI2cFtIdx;           /* e.g:SPI0 CS1,I2C0 addr */
        uint32_t uiDevAddr;
        uint32_t auiReserved[3];
    }    /* HW_TYPE_I2C_FT */

    struct {
        uint32_t uiI2cFtIdx;           /* e.g:SPI0 CS1,I2C0 addr */
        uint32_t uiDevAddr;
        uint32_t uiPcaAddr;
        uint32_t uiPcaChIdx;
        uint32_t uiReserved;
    }    /* HW_TYPE_I2C_FT_PCA */

    struct {
        uint32_t
    }

};

#endif

struct CBoardRegisterInfo
{
    std::vector<std::vector<CRegisterInfo> > staLogicDrvInfo;
    std::vector<std::vector<CRegisterInfo> > staI2cSwitchInfo;
    std::vector<std::vector<CRegisterInfo> > staUartSwitchInfo;

    std::vector<std::vector<CRegisterInfo> > staI2cInfo;
    std::vector<std::vector<CRegisterInfo> > staUartInfo;
    std::vector<std::vector<CRegisterInfo> > staFanLogicDrvInfo;

    std::vector<std::vector<CRegisterInfo> > staApsdInfo;
    std::vector<std::vector<CRegisterInfo> > staWatchDogInfo;
    std::vector<std::vector<CRegisterInfo> > staResetBoardInfo;
    std::vector<std::vector<CRegisterInfo> > staResetTypeInfo;

    std::vector<std::vector<CRegisterInfo> > staFanBoardInfo;
    std::vector<std::vector<CRegisterInfo> > staPsuBoardInfo;
    std::vector<std::vector<CRegisterInfo> > staLedBoardInfo;
    std::vector<std::vector<CRegisterInfo> > staOscBoardInfo;
    std::vector<std::vector<CRegisterInfo> > staSlotBoardInfo;

    std::vector<std::vector<CRegisterInfo> > staLogicInfo;

    /* std::vector<std::vector<CRegisterInfo>> staBoardInfo; */

    std::vector<std::vector<CRegisterInfo> > staFanInfo;
    std::vector<std::vector<CRegisterInfo> > staPsuInfo;
    std::vector<std::vector<CRegisterInfo> > staLedInfo;
    std::vector<std::vector<CRegisterInfo> > staTempInfo;

    std::vector<std::vector<CRegisterInfo> > staEEpromInfo;
    std::vector<std::vector<CRegisterInfo> > staFlashInfo;

    std::vector<std::vector<CRegisterInfo> > staMdioInfo;
    std::vector<std::vector<CRegisterInfo> > staNetSwitchInfo;
    std::vector<std::vector<CRegisterInfo> > staPhyInfo;
    std::vector<std::vector<CRegisterInfo> > staLtc4215Info;
    std::vector<std::vector<CRegisterInfo> > staMax1139Info;
    std::vector<std::vector<CRegisterInfo> > staMp5023Info;
};

class CDevThread : public CThread
{
public:
    CDevThread() : m_uiStatusInterval(200)
    {
    }
    virtual
    ~CDevThread()
    {
    }

    virtual void Run(void);

    void
    SetDevIdx(uint32_t uiDevIdx)
    {
        m_uiDevIdx = uiDevIdx;
    }

    void
    SetDevInfo(const std::vector<CRegisterInfo> &c_rstaDevInfo)
    {
        m_staDevInfo = c_rstaDevInfo;
    };

    void
    ExitThread(void)
    {
        m_bExitFlg = true;
        Join();
    }

    bool
    GetInitCmpltFlg(void)
    {
        return m_bInitCmpltFlg;
    }

    static void
    SetLoopEnable(bool bEnable)
    {
        m_bLoopEnable = bEnable;
    }

    static void
    SetForceIdle(bool bForceIdle)
    {
        m_bForceIdle = bForceIdle;
    }

    static void
    SetLoopInterval(uint32_t uiInterval)
    {
        m_uiInterval = uiInterval;
    }

    static void
    GetLoopInterval(uint32 &ruiInterval)
    {
        ruiInterval = m_uiInterval;
    }

    void
    SetStatusInterval(uint32_t interval)
    {
        m_uiStatusInterval = interval;
    }

private:

    virtual void
    CreateShm(void)
    {
    }
    virtual void
    GetDriver(void)
    {
    }
    virtual void
    InitCmd(void)
    {
    }
    virtual void
    DeleCmd(void)
    {
    }

    virtual void
    SetSubDevNum(void)
    {
    }
    virtual void
    DoSomething(void)
    {
    }

    virtual bool
    SendCommFailAlarm(void)
    {
        std::cout << "SendCommFailAlarm." << std::endl;
        return true;
    }

    virtual bool
    ClearCommFailAlarm(void)
    {
        std::cout << "ClearCommFailAlarm." << std::endl;
        return true;
    }

    virtual bool WaitInitOk(void);
    virtual bool GetStatusData(void) = 0;
    virtual bool GetMfg(void) = 0;

protected:
    bool m_bExitFlg = false;
    static bool m_bLoopEnable;
    static bool m_bForceIdle;
    static uint32 m_uiInterval;
    std::array<CShareMemory, SHM_TYPE_MAX> m_staShm;
    std::array<CSemaphore, SHM_TYPE_MAX> m_staSem;

    uint32_t m_uiDevIdx;
    std::vector<CRegisterInfo> m_staDevInfo;

    bool m_bInitCmpltFlg = false;
    bool m_bReady = false;
    bool m_bReadyEventFlg = false;

    uint32_t m_uiCommFailCnt = 0;
    bool m_bCommFailFlag = false;
#if 0
    struct timeval m_stStartTime, m_stStopTime;
#else
    struct timespec m_stStartTimeSpec, m_stStopTimeSpec;
#endif

    void CreateShm(const std::array<CShmCfg, SHM_TYPE_MAX> &c_rstaShmCfg, uint32_t uiKeyOffset = 0);

    bool ClearShm(void);

    void LoopProc(void);

    void __StartTime();

    bool __IsNeedRefresh();

private:
    /* The default interval time of get status data loop is 200ms. */
    uint32_t m_uiStatusInterval;

};

/* Board */
class CBoardThread : public CDevThread
{
public:

    virtual void Run(void) override;

    void
    SetSubDevInfo(const CBoardRegisterInfo &c_rstInfo)
    {
        m_stSubDevInfo = c_rstInfo;
    };

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_BOARD_STA,
          SHM_SEM_KEY_BOARD_STA,
          SHM_SIZE_BOARD_STA},

         {SHM_KEY_BOARD_CMD,
          SHM_SEM_KEY_BOARD_CMD,
          SHM_SIZE_BOARD_CMD},

         {SHM_KEY_BOARD_DATA,
          SHM_SEM_KEY_BOARD_DATA,
          SHM_SIZE_BOARD_DATA}}
    };

    CBoardRegisterInfo m_stSubDevInfo;

    std::shared_ptr<CBoard> m_stpDrvPtr;

    std::vector<std::shared_ptr<CLogicDriver> > m_staLogicDrvPtrs;
    std::vector<std::shared_ptr<CI2cSwitch> > m_staI2cSwitchPtrs;
    std::vector<std::shared_ptr<CUartSwitch> > m_staUartSwitchPtrs;

    std::vector<std::shared_ptr<CI2c> > m_staI2cPtrs;
    std::vector<std::shared_ptr<CInterfaceDriver> > m_staUartPtrs;
    std::vector<std::shared_ptr<CLogicDriver> > m_staFanLogicDrvPtrs;

    std::vector<std::shared_ptr<CApsd> > m_staApsdPtrs;
    std::vector<std::shared_ptr<CWatchDog> > m_staWatchDogPtrs;
    std::vector<std::shared_ptr<CResetBoard> > m_staResetBoardPtrs;
    std::vector<std::shared_ptr<CResetTypeRegister> > m_staResetTypeRegisterPtrs;

    std::vector<std::shared_ptr<CFanBoard> > m_staFanBoardPtrs;
    std::vector<std::shared_ptr<CPsuBoard> > m_staPsuBoardPtrs;
    std::vector<std::shared_ptr<CLedBoard> > m_staLedBoardPtrs;
    std::vector<std::shared_ptr<COscBoard> > m_staOscBoardPtrs;
    std::vector<std::shared_ptr<CSlotBoard> > m_staSlotBoardPtrs;

    std::vector<std::shared_ptr<CLogic> > m_staLogicPtrs;

    std::vector<std::shared_ptr<CFan> > m_staFanPtrs;
    std::vector<std::shared_ptr<CPsu> > m_staPsuPtrs;
    std::vector<std::shared_ptr<CLed> > m_staLedPtrs;
    std::vector<std::shared_ptr<CTemp> > m_staTempPtrs;

    std::vector<std::shared_ptr<CEEprom> > m_staEEpromPtrs;
    std::vector<std::shared_ptr<CFlash> > m_staFlashPtrs;

    std::vector<std::shared_ptr<CMdio> > m_stpMdioDrvPtr;
    std::vector<std::shared_ptr<CNetSwitch> > m_stpNetSwitchPtr;
    std::vector<std::shared_ptr<CPhy> > m_stpPhyPtr;
    std::vector<std::shared_ptr<CLtc4215> > m_staLtc4215Ptrs;
    std::vector<std::shared_ptr<CMp5023> > m_staMp5023Ptrs;
    std::vector<std::shared_ptr<CMax1139> > m_staMax1139Ptrs;

    virtual void SetSubDevNum(void) override;
    virtual bool WaitInitOk(void) override;
    virtual bool GetStatusData(void) override;
    virtual bool GetMfg(void) override;
    virtual void DoSomething(void) override;
    bool GetResetInfo(void);

    void InitSubDev(void);
};

/* all devices thread class */
#define CLASS_DEV_THREAD(ClassName, DEV, DevDriver, Cmd)       \
                                                               \
    class ClassName : public CDevThread                            \
    {                                                              \
private:                                                       \
        const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg       \
        {                                                          \
            {{SHM_KEY_ ## DEV ## _STA,                                 \
              SHM_SEM_KEY_ ## DEV ## _STA,                             \
              SHM_SIZE_ ## DEV ## _STA},                               \
                                                               \
             {SHM_KEY_ ## DEV ## _CMD,                                 \
              SHM_SEM_KEY_ ## DEV ## _CMD,                             \
              SHM_SIZE_ ## DEV ## _CMD},                               \
                                                               \
             {SHM_KEY_ ## DEV ## _DATA,                                \
              SHM_SEM_KEY_ ## DEV ## _DATA,                            \
              SHM_SIZE_ ## DEV ## _DATA}}                              \
        };                                                         \
                                                               \
        std::shared_ptr<DevDriver> m_stpDrvPtr;                    \
                                                               \
        virtual bool GetStatusData(void) override;              \
        virtual bool GetMfg(void) override;                     \
                                                                                                                        \
        virtual bool SendCommFailAlarm(void) override                        \
        {                                                                                       \
            return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_ ## DEV ## _COMM_FAIL, m_uiDevIdx); \
        }                                                                                       \
        virtual bool ClearCommFailAlarm(void) override                        \
        {                                                                                       \
            return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_CLEAR_ ## DEV ## _COMM_FAIL, m_uiDevIdx); \
        }                                                                                       \
        virtual void CreateShm(void) override                      \
        {                                                          \
            CDevThread::CreateShm(mc_staShmCfg, m_uiDevIdx * SHM_TYPE_MAX); \
        }                                                          \
        virtual void GetDriver(void) override;                     \
        virtual void InitCmd(void) override                        \
        {                                                          \
            Cmd::SetDevPtr(m_stpDrvPtr, m_uiDevIdx);     \
            Cmd::SetShm(m_staShm, m_staSem, m_uiDevIdx); \
        }                                                          \
        virtual void DeleCmd(void) override                        \
        {                                                          \
            Cmd::DeleteDevPtr(m_uiDevIdx);               \
            Cmd::DeleteShm(m_uiDevIdx);                  \
        }                                                          \
    }

CLASS_DEV_THREAD(COaThread, OA, COaDriver, COaCmd);
CLASS_DEV_THREAD(CWssThread, WSS, CWssDriver, CWssCmd);
CLASS_DEV_THREAD(CTdcmThread, TDCM, CTdcm, CTdcmCmd);
CLASS_DEV_THREAD(CPam4Thread, PAM4, CPam4Driver, CPam4Cmd);
/* CLASS_DEV_THREAD(COpsThread, OPS, COpsDriver, COpsCmd); */
CLASS_DEV_THREAD(CDceThread, DCE, CDceDriver, CDceCmd);

CLASS_DEV_THREAD(CDgeThread, DGE, CDgeDriver, CDgeCmd);
CLASS_DEV_THREAD(CMcsThread, MCS, CMcsDriver, CMcsCmd);
CLASS_DEV_THREAD(COtdrThread, OTDR, COtdrDriver, COtdrCmd);
CLASS_DEV_THREAD(COscThread, OSC, COscDriver, COscCmd);
CLASS_DEV_THREAD(CMuxThread, MUX, CMux, CMuxCmd);
CLASS_DEV_THREAD(CExtendThread, EXTEND, CExtendDriver, CExtendCmd);

class COcmThread : public CDevThread
{
private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_OCM_STA,
          SHM_SEM_KEY_OCM_STA,
          SHM_SIZE_OCM_STA},

         {SHM_KEY_OCM_CMD,
          SHM_SEM_KEY_OCM_CMD,
          SHM_SIZE_OCM_CMD},

         {SHM_KEY_OCM_DATA,
          SHM_SEM_KEY_OCM_DATA,
          SHM_SIZE_OCM_DATA}}
    };

    std::shared_ptr<COcmDriver> m_stpDrvPtr;
    uint32_t m_uiPortId = 1;

    virtual void DoSomething(void) override;
    virtual bool GetStatusData(void) override;
    virtual bool GetMfg(void) override;
    virtual void
    CreateShm(void) override
    {
        CDevThread::CreateShm(mc_staShmCfg, m_uiDevIdx * SHM_TYPE_MAX);
    }
    virtual void GetDriver(void) override;

    virtual void
    InitCmd(void) override
    {
        COcmCmd::SetDevPtr(m_stpDrvPtr, m_uiDevIdx);
        COcmCmd::SetShm(m_staShm, m_staSem, m_uiDevIdx);
    }
    virtual void
    DeleCmd(void) override
    {
        COcmCmd::DeleteDevPtr(m_uiDevIdx);
        COcmCmd::DeleteShm(m_uiDevIdx);
    }

    virtual bool
    SendCommFailAlarm(void) override
    {
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OCM_COMM_FAIL, m_uiDevIdx);
    }
    virtual bool
    ClearCommFailAlarm(void) override
    {
        std::cout << "Clear OCM CommFailAlarm." << std::endl;
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_CLEAR_OCM_COMM_FAIL, m_uiDevIdx);
    }
};

class COpsThread : public CDevThread
{
public:
    COpsThread();
    ~COpsThread();
private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_OPS_STA,
          SHM_SEM_KEY_OPS_STA,
          SHM_SIZE_OPS_STA},

         {SHM_KEY_OPS_CMD,
          SHM_SEM_KEY_OPS_CMD,
          SHM_SIZE_OPS_CMD},

         {SHM_KEY_OPS_DATA,
          SHM_SEM_KEY_OPS_DATA,
          SHM_SIZE_OPS_DATA}}
    };

    std::shared_ptr<COpsDriver> m_stpDrvPtr;
    pthread_t m_threadQueue;
    pthread_t m_threadInt;
    CNetLink m_nl;

    virtual bool GetStatusData(void) override;
    virtual bool GetMfg(void) override;
    virtual void GetDriver(void) override;

    virtual bool
    SendCommFailAlarm(void) override
    {
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS_COMM_FAIL, m_uiDevIdx);
    }
    virtual bool
    ClearCommFailAlarm(void) override
    {
        std::cout << "Clear OPS CommFailAlarm." << std::endl;
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_CLEAR_OPS_COMM_FAIL, m_uiDevIdx);
    }
    virtual void
    CreateShm(void) override
    {
        CDevThread::CreateShm(mc_staShmCfg, m_uiDevIdx * SHM_TYPE_MAX);
    }
    virtual void
    InitCmd(void) override
    {
        COpsCmd::SetDevPtr(m_stpDrvPtr, m_uiDevIdx);
        COpsCmd::SetShm(m_staShm, m_staSem, m_uiDevIdx);
    }
    virtual void
    DeleCmd(void) override
    {
        COpsCmd::DeleteDevPtr(m_uiDevIdx);
        COpsCmd::DeleteShm(m_uiDevIdx);
    }

    bool UpdateBidiStatus(void);

    static void *FastCmdProc(void *pvArg);
    static void *InterruptProc(void *pvArg);
};

class COcsThread : public CDevThread
{
private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_OCS_STA,
          SHM_SEM_KEY_OCS_STA,
          SHM_SIZE_OCS_STA},

         {SHM_KEY_OCS_CMD,
          SHM_SEM_KEY_OCS_CMD,
          SHM_SIZE_OCS_CMD},

         {SHM_KEY_OCS_DATA,
          SHM_SEM_KEY_OCS_DATA,
          SHM_SIZE_OCS_DATA}}
    };

    std::shared_ptr<COcsDriver> m_stpDrvPtr;
    uint32_t m_uiPortId = 1;

    virtual void DoSomething(void) override;
    virtual bool GetStatusData(void) override;
    virtual bool GetMfg(void) override;
    virtual void
    CreateShm(void) override
    {
        CDevThread::CreateShm(mc_staShmCfg, m_uiDevIdx * SHM_TYPE_MAX);
    }
    virtual void GetDriver(void) override;

    virtual void
    InitCmd(void) override
    {
        COcsCmd::SetDevPtr(m_stpDrvPtr, m_uiDevIdx);
        COcsCmd::SetShm(m_staShm, m_staSem, m_uiDevIdx);
    }
    virtual void
    DeleCmd(void) override
    {
        COcsCmd::DeleteDevPtr(m_uiDevIdx);
        COcsCmd::DeleteShm(m_uiDevIdx);
    }

    virtual bool
    SendCommFailAlarm(void) override
    {
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OCS_COMM_FAIL, m_uiDevIdx);
    }
    virtual bool
    ClearCommFailAlarm(void) override
    {
        std::cout << "Clear OCM CommFailAlarm." << std::endl;
        return CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_CLEAR_OCS_COMM_FAIL, m_uiDevIdx);
    }
};

