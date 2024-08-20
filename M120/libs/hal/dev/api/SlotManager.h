/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotManager.h
   Author:        Zhen Zhang
   Date:          2019-02-20
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-02-20] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevThread.h"
#include "SlotDriver.h"
#include "SlotCmd.h"


/*************************************************/

struct CHwInfo
{
    uint32_t uiHwType;     /* e.g:UART,I2C,SPI */
    uint32_t uiHwIdx;      /* e.g:SPI0,SPI1 */
    uint32_t uiHwSubIdx;
};

struct CSlotHwInfo
{
    CHwInfo stHwInfo;
    /* std::map<uint32_t, std::pair<uint32_t, uint32_t> > staModuleSupported;  / * CardType : <Module Func : Dev Func> * / */
    std::map<uint32_t, uint32_t> staModules;  /* CardType : Dev Type */
    std::map<uint32_t, uint32_t> staDevs;  /* CardType : Dev Func */
};


struct CSlotRegInfo
{
    std::vector<CRegisterInfo> staRegInfo;
    std::vector<uint32_t> staType;
    std::vector<CSlotHwInfo> staSlotHwInfo;
};

#if 0
struct CCardDevInfoDetail
{
    uint32_t uiDevAddr;
    uint32_t uiProto;
};


struct CCardDevInfo
{
    uint32_t uiDevType;
    uint32_t uiFuncType;

    std::vector<CCardDevInfoDetail> staDetailInfo;
};
#endif

struct CDevInfo
{
    uint32_t uiType;
    uint32_t uiFunc;
    uint32_t uiDevAddr;
    uint32_t uiProto;
};

struct CModuleInfo
{
    uint32_t uiType;
    /* uint32_t uiFunc;*/

    std::vector<CDevInfo> staDevInfo;
};

struct CCardInfo
{
    std::vector<CModuleInfo> staModuleInfo;
    std::vector<CDevInfo> staDevInfo;
};


struct CDevIdxInfo
{
    uint32_t uiDevType;
    uint32_t uiDevIdx;
};

class CSlotManager
{
public:
    static CSlotManager &GetInstance(void);

    void CreateShm(void);
    void Register(const std::vector<CSlotRegInfo> &c_rstaRegInfo);
    void Manage(bool bExit);



    void
    InitCmd(void)
    {
        CSlotCmd::SetSlotPtr(m_staSlotDrvs);
        CSlotCmd::SetShm(ms_staShm, ms_staSem);
    }

private:
    CSlotManager()
    {
        uint32_t iVal = 0;

        for (uint32_t i = 0; i < EEPROM_NUM; i++) {
            iVal = m_staDevRegIdx[DEV_TYPE_E2PROM];
            m_staDevRegIdx[DEV_TYPE_E2PROM] = iVal | (0x01 << i);
        }
    }
    CSlotManager(const CSlotManager &) = delete;
    CSlotManager &operator=(const CSlotManager &) = delete;

    bool GetSlotMfg();
    bool IsCardMatch(uint32_t uiIdx, uint32_t uiType);
    bool IsCardRegister(uint32_t uiIdx);

    void RegisterCard(uint32_t uiIdx, uint32_t uiCardType, CSlotInfo &rstSlotInfo);

    bool RegisterModule(uint32_t uiSlotIdx, uint32_t uiCardType, uint32_t &ruiDevNum, CSlotDevInfo *pastDevInfo);
    bool RegisterDev(uint32_t uiSlotIdx, uint32_t uiCardType);

    bool GetModuleRegInfo(uint32_t uiSlotIdx, uint32_t uiCardType, CDevInfo &stDevInfo, std::vector<CRegisterInfo> &rstaRegInfo, std::vector<uint32_t> &rstaDevType);
    bool GetDevRegInfo(uint32_t uiSlotIdx, uint32_t uiCardType, CDevInfo &stDevInfo, std::vector<CRegisterInfo> &rstaRegInfo);

    void UnregisterCard(uint32_t uiIdx, CSlotInfo &rstSlotInfo);
    bool UnregisterDev(uint32_t uiIdx);

    void OnlineProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo);
    void OfflineProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo);
    void MatchProc(uint32_t uiIdx, uint32_t uiCardType, CSlotInfo &rstSlotInfo);
    void MisMatchProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo);

    void UpdateStatusData(uint32_t uiIdx);

    void GetSlotMfg(uint32_t uiIdx);
    bool SendEvent(uint32_t uiIdx);
    void SetSlotNum(uint32_t uiNum);
    void SetShmStatusOk();
    void CmdProc();
    void UpdateSlotInfo(uint32_t uiIdx, CSlotInfo &rstSlotInfo);

    bool GetDevIdx(uint32_t uiType, uint32_t &ruiIdx);
    bool GetRegisterInfo(uint32_t uiIdx, uint32_t uiCardType, uint32_t uiFuncType, CHwInfo &rstInterface);

    void ClearDevIdx(uint32_t uiType, uint32_t uiIdx);


    uint32_t m_uiSlotNum = 0;
    std::vector<uint32_t> m_staSlotRegType;
    std::vector<std::shared_ptr<CSlotDriver> > m_staSlotDrvs;
    std::vector<std::vector<uint32_t> > m_staCardTypes;
    std::vector<std::vector<CSlotHwInfo> > m_staSlotHwInfo;
    std::vector<std::vector<CDevIdxInfo> > m_staDevIdxInfos;
    std::vector<CSlotInfo> m_staSlotInfos;

    std::map<uint32_t, uint32_t> m_staDevRegIdx;  /* dev type:dev idx in each bit */

    static const std::map<uint32_t, CCardInfo> msc_staCardsInfo;  /* card type:dev info ... */

    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg
    {
        {{SHM_KEY_SLOT_STA,
          SHM_SEM_KEY_SLOT_STA,
          SHM_SIZE_SLOT_STA},

         {SHM_KEY_SLOT_CMD,
          SHM_SEM_KEY_SLOT_CMD,
          SHM_SIZE_SLOT_CMD},

         {SHM_KEY_SLOT_DATA,
          SHM_SEM_KEY_SLOT_DATA,
          SHM_SIZE_SLOT_DATA}}
    };

    static std::array<CShareMemory, SHM_TYPE_MAX> ms_staShm;
    static std::array<CSemaphore, SHM_TYPE_MAX> ms_staSem;
};








