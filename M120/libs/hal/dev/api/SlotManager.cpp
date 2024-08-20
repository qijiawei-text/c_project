/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotManager.cpp
   Author:        Zhen Zhang
   Date:          2019-02-20
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-02-20] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SlotManager.h"
#include "DevDrvManager.h"
#include "DevData.h"
#include "SlotCmd.h"
#include "Event.h"
#include "util_inc.h"
#include "RegisterFile.h"
#include "ElapseTimer.h"
#include "BoardDriver.h"

/*=============================variables=========================*/
#if 0
const std::map<uint32_t, CCardInfo> CSlotManager::msc_staCardsInfo =
{

#if __RAINIER__
    {
        CARD_TYPE_OCM,
        {
            {DEV_TYPE_OCM, 0, MODULE_FUNC_TYPE_OCM, PROTO_TYPE_OCM},
            {DEV_TYPE_E2PROM, 0xAC, DEV_FUNC_TYPE_MEM, PROTO_TYPE_E2PROM}
        }
    },

    {
        CARD_TYPE_OPS,
        {
            {DEV_TYPE_OPS, DEV_TYPE_OPS, DEV_FUNC_TYPE_OPS, PROTO_TYPE_OPS},
            /* TBD */
            {DEV_TYPE_E2PROM, DEV_TYPE_OPS, DEV_FUNC_TYPE_MEM, PROTO_TYPE_E2PROM}
        }
    },

    {
        CARD_TYPE_PA,
        {
            {DEV_TYPE_OA, 0, DEV_FUNC_TYPE_OA_PA, PROTO_TYPE_OA_CLI_SIG},
            {DEV_TYPE_TDCM, 0x60, DEV_FUNC_TYPE_TDCM, PROTO_TYPE_TDCM},
            {DEV_TYPE_E2PROM, 0xAC, DEV_FUNC_TYPE_MEM, PROTO_TYPE_E2PROM},
            {DEV_TYPE_LED, 0, DEV_FUNC_TYPE_LED_SYS, PROTO_TYPE_LED},
            {DEV_TYPE_LED, 0, DEV_FUNC_TYPE_LED_ALM, PROTO_TYPE_LED},
            {DEV_TYPE_TEMP, 0x90, DEV_FUNC_TYPE_TEMP, PROTO_TYPE_DEF}
        }
    },

    {
        CARD_TYPE_BA,
        {
            {DEV_TYPE_OA, 0, DEV_FUNC_TYPE_OA_BA, PROTO_TYPE_OA_CLI_SIG},
            {DEV_TYPE_E2PROM, 0xAC, DEV_FUNC_TYPE_MEM, PROTO_TYPE_E2PROM},
            {DEV_TYPE_LED, 0, DEV_FUNC_TYPE_LED_SYS, PROTO_TYPE_LED},
            {DEV_TYPE_LED, 0, DEV_FUNC_TYPE_LED_ALM, PROTO_TYPE_LED},
            {DEV_TYPE_TEMP, 0x90, DEV_FUNC_TYPE_TEMP, PROTO_TYPE_DEF}
        }
    },
#else /* 400GZR */

    {
        CARD_TYPE_FIX,
        {
            {
                {
                    MODULE_TYPE_OA,
                    MODULE_FUNC_TYPE_OA_PA,
                    {
                        {DEV_TYPE_OA, DEV_FUNC_TYPE_OA, DEV_ADDR_NONE, PROTO_TYPE_OA_CLI_SIG},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_UPGRADE, DEV_FUNC_TYPE_UPGRADE, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },

                {
                    MODULE_TYPE_OA,
                    MODULE_FUNC_TYPE_OA_BA,
                    {
                        {DEV_TYPE_OA, DEV_FUNC_TYPE_OA, DEV_ADDR_NONE, PROTO_TYPE_OA_CLI_SIG},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_UPGRADE, DEV_FUNC_TYPE_UPGRADE, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
                {
                    MODULE_TYPE_OTDR,
                    MODULE_FUNC_TYPE_OTDR,
                    {
                        {DEV_TYPE_OTDR, DEV_FUNC_TYPE_OTDR, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
                {
                    MODULE_TYPE_OCM,
                    MODULE_FUNC_TYPE_OCM,
                    {
                        {DEV_TYPE_OCM, DEV_FUNC_TYPE_OCM, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
                {
                    MODULE_TYPE_EXTEND,
                    MODULE_FUNC_TYPE_EXTEND,
                    {
                        {DEV_TYPE_EXTEND, DEV_FUNC_TYPE_EXTEND, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
            },
        },

        {},
    },
#if 0
    {
        CARD_TYPE_MUX,
        {
            {
                MODULE_TYPE_MUX,
                MODULE_FUNC_TYPE_MUX,
                {
                    {DEV_TYPE_ADC, DEV_FUNC_TYPE_ADC_POSITIVE, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    {DEV_TYPE_ADC, DEV_FUNC_TYPE_ADC_OPPOSITE, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    {DEV_TYPE_FLASH, DEV_FUNC_TYPE_MEM, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                }
            },
        },
    },
#endif

#endif

};

#endif


#if 0
const std::map<uint32_t, CCardInfo> CSlotManager::msc_staCardsInfo =
{
    {
        CARD_TYPE_FIX,
        {
            {
                {
                    MODULE_TYPE_OA,
                    MODULE_FUNC_TYPE_OA_PA,
                    {
                        {DEV_TYPE_OA, DEV_FUNC_TYPE_OA, DEV_ADDR_NONE, PROTO_TYPE_OA_CLI_SIG},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },

                {
                    MODULE_TYPE_OA,
                    MODULE_FUNC_TYPE_OA_BA,
                    {
                        {DEV_TYPE_OA, DEV_FUNC_TYPE_OA, DEV_ADDR_NONE, PROTO_TYPE_OA_CLI_SIG},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
#if 0

                {
                    MODULE_TYPE_OTDR,
                    MODULE_FUNC_TYPE_OTDR,
                    {
                        {DEV_TYPE_OTDR, DEV_FUNC_TYPE_OTDR, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },


                {
                    MODULE_TYPE_OCM,
                    MODULE_FUNC_TYPE_OCM,
                    {
                        {DEV_TYPE_OCM, DEV_FUNC_TYPE_OCM, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },

                {
                    MODULE_TYPE_EXTEND,
                    MODULE_FUNC_TYPE_EXTEND,
                    {
                        {DEV_TYPE_EXTEND, DEV_FUNC_TYPE_EXTEND, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                        {DEV_TYPE_RESET, DEV_FUNC_TYPE_RESET_COLD, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
#endif

            },
        },
    },

    {
        CARD_TYPE_MUX,
        {
            {
                {
                    MODULE_TYPE_MUX,
                    MODULE_FUNC_TYPE_MUX,
                    {
                        {DEV_TYPE_E2PROM, DEV_FUNC_TYPE_STORE, DEV_ADDR_NONE, PROTO_TYPE_DEF},
                    },
                },
            },
        },
    }
};
#endif

std::array<CShareMemory, SHM_TYPE_MAX> CSlotManager::ms_staShm;
std::array<CSemaphore, SHM_TYPE_MAX> CSlotManager::ms_staSem;


/*==============================function=========================*/

CSlotManager &
CSlotManager::GetInstance(void)
{

    static CSlotManager s_stSlotManager;

    return s_stSlotManager;

}

void
CSlotManager::CreateShm(void)
{
    for (int32_t i = 0; i < SHM_TYPE_MAX; i++) {
        ms_staShm.at(i).CreateAttache(mc_staShmCfg.at(i).eShmKey, mc_staShmCfg.at(i).eShmSize);
        ms_staSem.at(i).Create(mc_staShmCfg.at(i).eSemKey);
    }

    CSlotCmd::SetShm(ms_staShm, ms_staSem);
}


bool
CSlotManager::SendEvent(uint32_t uiIdx)
{
    bool bRet = false;

    uint32_t uiEventIdx = uiIdx + DEF_EVENT_ID_SLOT0;
    LOG_WRITE(EN_LOG_NOTICE, "Slot%d online event send to DEF_OPERATION_EVENT_SOCKET_PATH, uiEventIdx: %u", uiIdx, uiEventIdx);
    bRet = CEvent::SendEvent((char *)DEF_OPERATION_EVENT_SOCKET_PATH, uiEventIdx, 0);
    CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, uiEventIdx, 0);
    CEvent::SendEvent((char *)DEF_PMD_EVENT_SOCKET_PATH, uiEventIdx, 0);
    /* mux plug/unplug event also need send to csmd*/
#ifdef __HCF__
    if (DEF_EVENT_ID_SLOT1 == uiEventIdx) {
#else
    if (DEF_EVENT_ID_SLOT3 == uiEventIdx) {
#endif
        LOG_WRITE(EN_LOG_NOTICE, "SendEvent to DEF_CSMD_EVENT_SOCKET_PATH, uiIdx: %d", uiIdx);
        CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, uiEventIdx, 0);
    }
    return bRet;
}


void
CSlotManager::Register(const std::vector<CSlotRegInfo> &c_rstaRegInfo)
{

    for (auto stSlotRegInfo : c_rstaRegInfo) {
        CSlotInfo stSlotInfo;
        memset(&stSlotInfo, 0, sizeof(stSlotInfo));

        m_staSlotDrvs.push_back(CDevDrvManager::GetInstance().GetSlotDrv(stSlotRegInfo.staRegInfo));
        m_staCardTypes.push_back(stSlotRegInfo.staType);
        m_staSlotRegType.push_back(CARD_TYPE_UNKOWN);
        m_staSlotHwInfo.push_back(stSlotRegInfo.staSlotHwInfo);
        m_staSlotInfos.push_back(stSlotInfo);
    }

    CSlotCmd::SetSlotPtr(m_staSlotDrvs);
    m_uiSlotNum = c_rstaRegInfo.size();
    m_staDevIdxInfos.resize(m_uiSlotNum);

    LOG_WRITE(EN_LOG_NOTICE, "m_uiSlotNum = %d", m_uiSlotNum);

    SetSlotNum(m_uiSlotNum);
    SetShmStatusOk();

}



void
CSlotManager::Manage(bool bExit)
{
    bool bRet = false;
    std::shared_ptr<CLogicDriver> stpLogicDrv = std::static_pointer_cast<CLogicDriver>(
        std::make_shared<CPcieBusLogic>(0, 0, 0, DATA_WIDTH_1B));

    /* CSlotInfo stSlotInfo;
       memset(&stSlotInfo, 0, sizeof(stSlotInfo)); */

    /* get each slot info */
    for (uint32_t i = 0; i < m_uiSlotNum; i++) {

        if (bExit) {
            OfflineProc(i, m_staSlotInfos[i]);
            continue;
        }

        m_staSlotInfos[i].uiPlugFlag = m_staSlotDrvs.at(i)->IsPlug();

        if (true == m_staSlotInfos[i].uiPlugFlag) {
            /* EQA-22 set scc sys led amber, blink */
            stpLogicDrv->WriteReg(0x1068, 3);
            stpLogicDrv->WriteReg(0x106c, 1);
        }

        if (m_staSlotDrvs.at(i)->IsOnline()) {
            OnlineProc(i, m_staSlotInfos[i]);
        } else {
            OfflineProc(i, m_staSlotInfos[i]);
        }

        /* update slot info to shm */
        UpdateSlotInfo(i, m_staSlotInfos[i]);

        if (true == m_staSlotInfos[i].uiPlugFlag) {

            LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d uiSlotState = %d.", i, m_staSlotInfos[i].uiSlotState);
            LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d uiCardType = %d.", i, m_staSlotInfos[i].uiCardType);
            LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d uiPlugFlag = %d.", i, m_staSlotInfos[i].uiPlugFlag);

            /* send online event */
            bRet = SendEvent(i);
            if (true == bRet ) {
                LOG_WRITE(EN_LOG_DEBUG, "Clear plug flag.");
                m_staSlotDrvs.at(i)->ClearPlug();

                /* EQA-22 set scc sys led green */
                stpLogicDrv->WriteReg(0x1068, 2);
                stpLogicDrv->WriteReg(0x106c, 0);
            }
        }
    }

    if (bExit) {
        return;
    }

    CmdProc();

    LOG_WRITE(EN_LOG_DEBUG, "CSlotManager::Manage ---");
}

void
CSlotManager::OnlineProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo)
{
    uint32_t uiCardType = CRegisterFile::ms_staSlotRegInfo[uiIdx].staType[0];

    m_staSlotDrvs.at(uiIdx)->GetCardType(uiCardType);
    rstSlotInfo.uiCardType = uiCardType;

    /* LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d uiCardType = %d.", uiIdx, uiCardType); */

    if (IsCardMatch(uiIdx, uiCardType)) {

        MatchProc(uiIdx, uiCardType, rstSlotInfo);

        /* update status data. e.g : temperature */
        UpdateStatusData(uiIdx);

    } else {

        MisMatchProc(uiIdx, rstSlotInfo);
    }
}


void
CSlotManager::OfflineProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo)
{
    rstSlotInfo.uiCardType = CARD_TYPE_UNKOWN;
    rstSlotInfo.uiSlotState = SLOT_STATE_OFFLINE;

    if (IsCardRegister(uiIdx)) {
        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d offline +++.", uiIdx);
        UnregisterCard(uiIdx, rstSlotInfo);
        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d offline ---.", uiIdx);
    }
}

void
CSlotManager::MatchProc(uint32_t uiIdx, uint32_t uiCardType, CSlotInfo &rstSlotInfo)
{
    /* set slot state */
    rstSlotInfo.uiSlotState = SLOT_STATE_ONLINE;

    if (false == IsCardRegister(uiIdx)) {
        RegisterCard(uiIdx, uiCardType, rstSlotInfo);

        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d match. uiCardType = %d", uiIdx, rstSlotInfo.uiCardType);
        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d match. uiSlotState = %d", uiIdx, rstSlotInfo.uiSlotState);
        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d match. uiPlugFlag = %d", uiIdx, rstSlotInfo.uiPlugFlag);
    }
}

void
CSlotManager::MisMatchProc(uint32_t uiIdx, CSlotInfo &rstSlotInfo)
{
    /* set slot state */
    rstSlotInfo.uiSlotState = SLOT_STATE_MISMATCH;

    /* check if register */
    if (IsCardRegister(uiIdx)) {
        LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d mismatch. uiCardType = %d", uiIdx, rstSlotInfo.uiCardType);
        UnregisterCard(uiIdx, rstSlotInfo);
    }
}

void
CSlotManager::GetSlotMfg(uint32_t uiIdx)
{

#define RETRY_TIMES  3
    int32_t uiRetryTimes = 0;

    /* get card mfg */
    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

#if __USB_RELAOD_DEBUG__
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);
#endif

    for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
        if (m_staSlotDrvs.at(uiIdx)->GetMfg(stMfg)) {
#if __USB_RELAOD_DEBUG__
            ofile << "CSlotManager::GetSlotMfg ok!" << std::endl;
#endif
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Slot %d get mfg error.", uiIdx);
#if __USB_RELAOD_DEBUG__
            ofile << "CSlotManager::GetSlotMfg fail!!!!" << std::endl;
#endif
            CDelay::Delay(10, MS);
        }
    }
#if __USB_RELAOD_DEBUG__
    ofile.close();
#endif

    LOG_WRITE(EN_LOG_NOTICE, "Slot(%d) GetMfg +++", uiIdx);

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", stMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", stMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", stMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", stMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", stMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", stMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", stMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", stMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", stMfg.acModuleType);

    LOG_WRITE(EN_LOG_NOTICE, "Slot(%d) GetMfg ---", uiIdx);

    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stMfg,
                      CShmOperator::GetDataSlotMfgPos(uiIdx), sizeof(stMfg));

    /* get card cali */
    CCali stCali;
    memset(&stCali, 0, sizeof(stCali));

    for (uiRetryTimes = 0; uiRetryTimes < RETRY_TIMES; uiRetryTimes++) {
        if (m_staSlotDrvs.at(uiIdx)->GetCali(stCali)) {
            break;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Slot %d get cali error.", uiIdx);
            CDelay::Delay(10, MS);
        }
    }

    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stCali,
                      CShmOperator::GetDataSlotCaliPos(uiIdx), sizeof(stCali));

}


void
CSlotManager::UpdateSlotInfo(uint32_t uiIdx, CSlotInfo &rstSlotInfo)
{
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&rstSlotInfo,
                      CShmOperator::GetDataSlotInfoPos(uiIdx), sizeof(rstSlotInfo));
}

void
CSlotManager::SetSlotNum(uint32_t uiNum)
{
    if ( false == CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&m_uiSlotNum,
                                    CShmOperator::GetDataSlotNumPos(), sizeof(m_uiSlotNum))) {
        LOG_WRITE(EN_LOG_ERROR, "set slot num error.");
    }
}

void
CSlotManager::SetShmStatusOk()
{
    CShmDevSta stState = {SHM_STATUS_OK, SHM_STATUS_NONE, SHM_STATUS_NONE};
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_STA), ms_staSem.at(SHM_TYPE_STA), (uint8_t *)&stState,
                      CShmOperator::GetStaDevStaPos(), sizeof(stState));
}

void
CSlotManager::CmdProc()
{
    CQueueNode stQNode;
    if (CShmOperator::GetCmd(ms_staShm.at(SHM_TYPE_CMD), ms_staSem.at(SHM_TYPE_CMD), stQNode)) {
        LOG_WRITE(EN_LOG_DEBUG, "slot get cmd");
        CDevCmdProcesser::GetInstance().CmdProc(ms_staShm.at(SHM_TYPE_CMD), ms_staSem.at(SHM_TYPE_CMD), stQNode);
    }
}

void
CSlotManager::UpdateStatusData(uint32_t uiIdx)
{
    CSlotStatusData stData = {0};
    /* CLedSta astLedSta[LED_MAX_NUM] = {0}; */

#if 0 /* usb to i2c may cause main thread block*/
    int32_t iTemp = 0;
    /* every 10 * 1 times get temp */
    m_staSlotDrvs.at(uiIdx)->m_ullCnt++;
    if (0 == (m_staSlotDrvs.at(uiIdx)->m_ullCnt % 10)) {

        LOG_WRITE(EN_LOG_NOTICE, "Slot(%d) Get Temp.", uiIdx);
        m_staSlotDrvs.at(uiIdx)->GetTemp(iTemp);
        LOG_WRITE(EN_LOG_NOTICE, "Slot(%d) iTemp = %d", uiIdx, iTemp);
        CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&iTemp,
                          CShmOperator::GetDataSlotTempStatusPos(uiIdx), sizeof(iTemp));
    }
#endif
    m_staSlotDrvs.at(uiIdx)->GetStatusData(stData);
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)&stData,
                      CShmOperator::GetDataSlotStatusPos(uiIdx), sizeof(stData));

#if 0
    /* Get Led */
    m_staSlotDrvs.at(uiIdx)->GetAllLedState(astLedSta);
    CShmOperator::Set(ms_staShm.at(SHM_TYPE_DATA), ms_staSem.at(SHM_TYPE_DATA), (uint8_t *)astLedSta,
                      CShmOperator::GetDataSlotLedStatusPos(uiIdx), sizeof(astLedSta));
#endif

}


bool
CSlotManager::IsCardMatch(uint32_t uiIdx, uint32_t uiType)
{

    if (uiIdx >= m_uiSlotNum) {
        return false;
    }
#if 1
    for (auto uiValidType : m_staCardTypes.at(uiIdx)) {
        /* LOG_WRITE(EN_LOG_NOTICE, "Valide Type = %d", uiValidType); */
        if (uiValidType == uiType) {
            return true;
        }
    }
#endif

    return false;

}

bool
CSlotManager::IsCardRegister(uint32_t uiIdx)
{
    if (CARD_TYPE_UNKOWN == m_staSlotRegType.at(uiIdx)) {
        return false;
    } else {
        return true;
    }
}

void
CSlotManager::RegisterCard(uint32_t uiSlotIdx, uint32_t uiCardType, CSlotInfo &rstSlotInfo)
{
    /* register devices and get their info */
    RegisterModule(uiSlotIdx, rstSlotInfo.uiCardType, rstSlotInfo.uiDevNum, rstSlotInfo.astDevInfo);

    LOG_WRITE(EN_LOG_NOTICE, "SLOT #%d register. uiDevNum = %d", uiSlotIdx, rstSlotInfo.uiDevNum);

    RegisterDev(uiSlotIdx, rstSlotInfo.uiCardType);

    /* get slot mfg */
    /* if (m_staSlotRegType.at(uiSlotIdx) <= CARD_TYPE_OPTICAL) { */
    if ( uiCardType < CARD_TYPE_FAN) {

        GetSlotMfg(uiSlotIdx);
    }

    /* set reg type */
    m_staSlotRegType.at(uiSlotIdx) = uiCardType;
}

void
CSlotManager::UnregisterCard(uint32_t uiIdx, CSlotInfo &rstSlotInfo)
{
    /* unregister devices */
    UnregisterDev(uiIdx);

    /* unregister sub devices */
    m_staSlotDrvs.at(uiIdx)->UnregisterSubDevs();

    /* set reg type CARD_TYPE_UNKOWN */
    m_staSlotRegType.at(uiIdx) = CARD_TYPE_UNKOWN;

    /* clear dev num */
    rstSlotInfo.uiDevNum = 0;

    /* clear dev info */
    memset(rstSlotInfo.astDevInfo, 0, sizeof(rstSlotInfo.astDevInfo));
}

bool
CSlotManager::RegisterModule(uint32_t uiSlotIdx, uint32_t uiCardType, uint32_t &ruiDevNum, CSlotDevInfo *pastDevInfo)
{
    ruiDevNum = 0;

    if ((uiSlotIdx >= m_uiSlotNum) || (nullptr == pastDevInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == pastDevInfo. uiSlotIdx = %d", uiSlotIdx);
        return false;
    }

    /* find card type in map */
    auto itIter = CRegisterFile::ms_staCardRegInfo.find(uiCardType);

    if (CRegisterFile::ms_staCardRegInfo.end() == itIter) {
        LOG_WRITE(EN_LOG_ERROR, "uiCardType error. uiCardType = %d", uiCardType);
        return false;
    }

    /* register each dev on this card */
    uint32_t uiDevRegNum = 0;

    for (auto stModuleInfo : itIter->second.staModuleInfo) {
        std::vector<CRegisterInfo> staRegisterInfo;
        std::vector<uint32_t> staDevType;

        for (auto stDevInfo : stModuleInfo.staDevInfo) {
            /* get slot interface */
            if (false == GetModuleRegInfo(uiSlotIdx, uiCardType, stDevInfo, staRegisterInfo, staDevType)) {
                LOG_WRITE(EN_LOG_ERROR, "get module register info error. SlotIdx = %d, CardType = %d",
                          uiSlotIdx, uiCardType);
                continue;
            }
        }

        LOG_WRITE(EN_LOG_TRACE, "staRegisterInfo.size() = %lu", staRegisterInfo.size());

        if (staRegisterInfo.size() <= 0) {
            continue;
        }

        /* get Dev index */
        uint32_t uiDevIdx;
        if (!GetDevIdx(stModuleInfo.uiType, uiDevIdx)) {
            /* get dev index error */
            LOG_WRITE(EN_LOG_ERROR, "get dev index error. uiDevType = %u", stModuleInfo.uiType);
            continue;
        }

        bool bRegFlg = false;

        /* get register object */
        CRegisterFactory stRegisterFactory;
        std::shared_ptr<CRegister> stpRegister = stRegisterFactory.GetRegister(stModuleInfo.uiType, staRegisterInfo, uiDevIdx);

        if (stpRegister != nullptr) {
            LOG_WRITE(EN_LOG_DEBUG, "Register DevType = %u", stModuleInfo.uiType);
            bRegFlg = stpRegister->Register();
        }

        if (!bRegFlg) {
            ClearDevIdx(stModuleInfo.uiType, uiDevIdx);
        } else {
            CDevIdxInfo stDevIdxInfo = {stModuleInfo.uiType, uiDevIdx};
            m_staDevIdxInfos.at(uiSlotIdx).push_back(stDevIdxInfo);

            pastDevInfo->uiDevType = staDevType[0];
            pastDevInfo->uiDevIdx = uiDevIdx;


            LOG_WRITE(EN_LOG_NOTICE, "pastDevInfo->uiDevType = %x", pastDevInfo->uiDevType);
            LOG_WRITE(EN_LOG_NOTICE, "pastDevInfo->uiDevIdx = %d", pastDevInfo->uiDevIdx);

            pastDevInfo++;
            uiDevRegNum++;


        }
    }

    ruiDevNum = uiDevRegNum;

    return true;

}

bool
CSlotManager::RegisterDev(uint32_t uiSlotIdx, uint32_t uiCardType)
{

    LOG_WRITE(EN_LOG_TRACE, "RegisterDev +++ Slot Idx = %d, Card Type = %d", uiSlotIdx, uiCardType);
    if (uiSlotIdx >= m_uiSlotNum) {
        LOG_WRITE(EN_LOG_ERROR, "nullptr == pastDevInfo. uiSlotIdx = %d", uiSlotIdx);
        return false;
    }

    /* find card type in map */
    auto itIter = CRegisterFile::ms_staCardRegInfo.find(uiCardType);

    if (CRegisterFile::ms_staCardRegInfo.end() == itIter) {
        LOG_WRITE(EN_LOG_ERROR, "uiCardType error. uiCardType = %d", uiCardType);
        return false;
    }

    for (auto stDevInfo : itIter->second.staDevInfo) {
        std::vector<CRegisterInfo> staRegInfo;
        /* get slot interface */
        if (!GetDevRegInfo(uiSlotIdx, uiCardType, stDevInfo, staRegInfo)) {
            continue;
        }

        /* get Dev index */
        uint32_t uiDevIdx;
        if (!GetDevIdx(stDevInfo.uiType, uiDevIdx)) {
            /* get dev index error */
            LOG_WRITE(EN_LOG_ERROR, "get dev index error. uiDevType = %d", stDevInfo.uiType);
            continue;
        }

        m_staSlotDrvs.at(uiSlotIdx)->RegisterSubDev(stDevInfo.uiType, staRegInfo);
    }

    LOG_WRITE(EN_LOG_TRACE, "RegisterDev --- Slot Idx = %d, Card Type = %d", uiSlotIdx, uiCardType);

    return true;

}


bool
CSlotManager::UnregisterDev(uint32_t uiIdx)
{

    if (uiIdx >= m_uiSlotNum) {
        return false;
    }


    /* delete each dev on this card */
    std::vector<CDevIdxInfo> &rstaDevIdxInfos = m_staDevIdxInfos.at(uiIdx);

    LOG_WRITE(EN_LOG_DEBUG, "rstaDevIdxInfos.size() = %lu", rstaDevIdxInfos.size());

    auto itIter = rstaDevIdxInfos.begin();
    while (itIter != rstaDevIdxInfos.end()) {
        bool bUnregFlg = false;

        /* get register object */
        CRegisterFactory stRegisterFactory;
        std::shared_ptr<CRegister> stpRegister = stRegisterFactory.GetRegister(itIter->uiDevType);

        /* LOG_WRITE(EN_LOG_NOTICE, "CSlotManager::UnregisterDev(%d) +++", itIter->uiDevType); */
        /*std::cout << "CSlotManager::UnregisterDev(" << itIter->uiDevType << ") +++." << std::endl; */

        if (stpRegister != nullptr) {

            CElapseTimer::Start();
            LOG_WRITE(EN_LOG_DEBUG, "stpRegister != nullptr");
            /*std::cout << "stpRegister != nullptr" << std::endl; */
            bUnregFlg = stpRegister->Unregister(itIter->uiDevIdx);

            CElapseTimer::Stop();
        }

        if (bUnregFlg) {
            ClearDevIdx(itIter->uiDevType, itIter->uiDevIdx);

            /* delete this dev */
            itIter = rstaDevIdxInfos.erase(itIter);
            /*std::cout << "delete " << itIter->uiDevType << std::endl; */
        } else {

            /*std::cout << "itIter++ " << std::endl; */
            itIter++;
        }

        /*std::cout << "CSlotManager::UnregisterDev(" << itIter->uiDevType << ") ---." << std::endl; */
        /* LOG_WRITE(EN_LOG_NOTICE, "CSlotManager::UnregisterDev(%d) ---", itIter->uiDevType); */
    }

    return true;

}

bool
CSlotManager::GetDevIdx(uint32_t uiType, uint32_t &ruiIdx)
{

    /* check if find this type of dev in map */
    auto itIter = m_staDevRegIdx.find(uiType);

    if (m_staDevRegIdx.end() == itIter) {
        m_staDevRegIdx[uiType] = 1;

        ruiIdx = 0;
    } else {
        uint32_t i;

        /* check each bit */
        for (i = 0; i < DEV_MAX_NUM; i++) {
            if ( 0 == ((itIter->second >> i) & 0x01)) {
                ruiIdx = i;

                m_staDevRegIdx[uiType] = itIter->second | (0x01 << i);

                break;
            }
        }

        if (DEV_MAX_NUM == i) {
            return false;
        }
    }

    return true;

}


bool
CSlotManager::GetModuleRegInfo(uint32_t uiSlotIdx, uint32_t uiCardType, CDevInfo &stDevInfo, std::vector<CRegisterInfo> &rstaRegInfo, std::vector<uint32_t> &rstaDevType)
{

    if (uiSlotIdx >= m_uiSlotNum) {
        return false;
    }

    for (auto stSlotHwInfo : m_staSlotHwInfo.at(uiSlotIdx)) {

        /* check if find this type of card in map */
        auto staModule = stSlotHwInfo.staModules.find(uiCardType);
        LOG_WRITE(EN_LOG_TRACE, "uiSlotIdx = %d uiCardType = %d", uiSlotIdx, uiCardType);

        if (stSlotHwInfo.staModules.end() != staModule) {
            uint32_t uiDevType = stDevInfo.uiType;
            uint32_t uiDevTypeInModule = staModule->second & 0xFFFF;
            uint32_t uiDevTypeInSys = staModule->second;
            uint32_t uiDevFunc = stDevInfo.uiType >> FUNC_FLAG_OFFSET;

            LOG_WRITE(EN_LOG_NOTICE, "uiFunc = %d staModuleMap->second = %x", uiDevFunc, staModule->second);
            if ((uiDevType == uiDevTypeInSys)
                || (DEV_TYPE_NONE == uiDevFunc && uiDevType == uiDevTypeInModule)
                || (DEV_TYPE_OPS == uiDevType && DEV_TYPE_OPS4A == uiDevTypeInSys)
                || (DEV_TYPE_OPS == uiDevType && DEV_TYPE_OPS4B == uiDevTypeInSys)) {

                /* LOG_WRITE(EN_LOG_NOTICE, "stDevInfo.uiFunc = %d DevFunc = %d", stDevInfo.uiFunc, itIter->second.second); */
                CRegisterInfo stInfo = {stSlotHwInfo.stHwInfo.uiHwType,
                                        stSlotHwInfo.stHwInfo.uiHwIdx,
                                        stSlotHwInfo.stHwInfo.uiHwSubIdx,
                                        stDevInfo.uiDevAddr,
                                        stDevInfo.uiProto};


                LOG_WRITE(EN_LOG_NOTICE, "uiHwType = %d, uiHwIdx = %d, uiHwSubIdx = %d, uiDevAddr = %d, uiProto = %d",
                          stInfo.uiHwType, stInfo.uiHwIdx, stInfo.uiHwSubIdx, stInfo.uiDevAddr, stInfo.uiProto);

                rstaRegInfo.push_back(stInfo);
                rstaDevType.push_back(uiDevTypeInSys);
                return true;
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Not Found. CardType = %d", uiCardType);
        }
    }

    return false;

}



bool
CSlotManager::GetDevRegInfo(uint32_t uiSlotIdx, uint32_t uiCardType, CDevInfo &stDevInfo, std::vector<CRegisterInfo> &rstaRegInfo)
{

    if (uiSlotIdx >= m_uiSlotNum) {
        return false;
    }

    for (auto stSlotHwInfo : m_staSlotHwInfo.at(uiSlotIdx)) {
        /* check if find this type of card in map */
        auto staDevMap = stSlotHwInfo.staDevs.find(uiCardType);

        if (stSlotHwInfo.staDevs.end() != staDevMap) {
            /* match dev type */
            if (stDevInfo.uiType == staDevMap->second) {
                CRegisterInfo stInfo = {stSlotHwInfo.stHwInfo.uiHwType,
                                        stSlotHwInfo.stHwInfo.uiHwIdx,
                                        stSlotHwInfo.stHwInfo.uiHwSubIdx,
                                        stDevInfo.uiDevAddr,
                                        stDevInfo.uiProto};
                rstaRegInfo.push_back(stInfo);
                continue;
            }
        }
    }

    if (rstaRegInfo.size() > 0) {
        return true;
    } else {
        return false;
    }

}


void
CSlotManager::ClearDevIdx(uint32_t uiType, uint32_t uiIdx)
{

    /* check if find this type of dev in map */
    auto itIter = m_staDevRegIdx.find(uiType);

    if (m_staDevRegIdx.end() != itIter) {
        m_staDevRegIdx.at(uiType) = itIter->second & (~(0x01 << uiIdx));
    }

}

