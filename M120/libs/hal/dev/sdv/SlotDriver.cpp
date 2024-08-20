/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotDriver.cpp
   Author:        Zhen Zhang
   Date:          2019-03-21
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-03-21] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "SlotDriver.h"
#include "DevDrvManager.h"
#include "EEpromOperator.h"

/*=============================variables=========================*/


/*==============================function=========================*/

void
CSlotDriver::RegisterSubDev(uint32_t uiDevType, std::vector<CRegisterInfo> &staRegisterInfo)
{

    LOG_WRITE(EN_LOG_NOTICE, "RegisterSubDev +++ uiDevType = %d", uiDevType);

    LOG_WRITE(EN_LOG_NOTICE, "uiHwIdx = %d, uiHwSubIdx = %d",
              staRegisterInfo.at(0).uiHwIdx, staRegisterInfo.at(0).uiHwSubIdx);

    switch (uiDevType) {
    case DEV_TYPE_TEMP:
        m_stpTempDrvPtr = CDevDrvManager::GetInstance().GetTempDrv(staRegisterInfo);
        break;
    case DEV_TYPE_E2PROM:
        m_stpEEpromPtr = CDevDrvManager::GetInstance().GetEEpromDrv(staRegisterInfo);
        break;
    case DEV_TYPE_LED:
        m_staLedPtrs.push_back(CDevDrvManager::GetInstance().GetLedDrv(staRegisterInfo));
        break;
    case DEV_TYPE_PHY:
        m_stpPhy = CDevDrvManager::GetInstance().GetPhyDrv(staRegisterInfo);
        m_stpPhy->Config();
        break;
    }

    LOG_WRITE(EN_LOG_NOTICE, "RegisterSubDev --- uiDevType = %d", uiDevType);

    return;
}

void
CSlotDriver::UnregisterSubDevs()
{
    m_stpTempDrvPtr = nullptr;
    m_stpEEpromPtr = nullptr;
    m_stpPhy = nullptr;

    return;
}


bool
CSlotDriver::GetTemp(int32_t &riTemp)
{

    if (nullptr == m_stpTempDrvPtr) {

        LOG_WRITE(EN_LOG_ERROR, "slot #%d temp driver is nullptr.", m_uiHwSubIdx);
        return false;
    }

    if (!m_stpTempDrvPtr->GetTemp(riTemp)) {
        LOG_WRITE(EN_LOG_ERROR, "get slot #%d temperature error.", m_uiHwSubIdx);
        return false;
    }
    return true;
}

bool
CSlotDriver::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::GetMfg +++");

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot #%d eeprom driver is nullptr.", m_uiHwSubIdx);
        return false;
    }

    if ( false == CEEpromOperator::ReadMfg(m_stpEEpromPtr, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "Read slot mfg error.");
        bEEpromAlarm = true;
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::GetMfg ---");

    bEEpromAlarm = false;
    return true;
}

bool
CSlotDriver::SetMfg(const char *c_pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::SetMfg +++");

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot eeprom driver is nullptr.");
        return false;
    }

    if ( false == CEEpromOperator::WriteMfg(m_stpEEpromPtr, c_pcFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "write slot mfg error: %u.", m_uiHwSubIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::SetMfg ---");

    return true;
}


bool
CSlotDriver::GetCali(CCali &rstCali)
{

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot eeprom driver is nullptr.");
        return false;
    }

    if ( false == CEEpromOperator::ReadComCali(m_stpEEpromPtr, rstCali)) {
        LOG_WRITE(EN_LOG_ERROR, "Read EEprom cali error.");
        bEEpromAlarm = true;
        return false;
    }

    bEEpromAlarm = false;
    return true;
}

bool
CSlotDriver::SetCali(const char *c_pcFileName)
{
    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot eeprom driver is nullptr.");
        return false;
    }

    if ( false == CEEpromOperator::WriteComCali(m_stpEEpromPtr, c_pcFileName)) {
        LOG_WRITE(EN_LOG_ERROR, "write EEprom cali error. c_pcFileName = %s", c_pcFileName);
        return false;
    }

    return true;
}


bool
CSlotDriver::GetStatusData(CSlotStatusData &rstData)
{
    rstData.bEEpromAlarm = bEEpromAlarm;

    GetAllLedState(rstData.astLed);

#if	defined __OCS__
	/* For OCS, ignore get card temperature */
#else
	GetTemp(rstData.iTemp);
#endif	

    return true;
}


bool
CSlotDriver::GetLedState(uint32_t uiLedIdx, uint32_t &uiState)
{

    if (uiLedIdx >= m_staLedPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLedIdx >= m_staLedPtrs.size(). uiLedIdx = %d", uiLedIdx);
        return false;
    }

    if (false == m_staLedPtrs.at(uiLedIdx)->GetState(uiState)) {
        LOG_WRITE(EN_LOG_ERROR, "get led state error. m_uiHwSubIdx = %u, uiLedIdx: %u, uiState = %u", m_uiHwSubIdx, uiLedIdx, uiState);
        return false;
    }


    return true;

}

bool
CSlotDriver::SetLedState(uint32_t uiLedIdx, uint32_t uiState)
{

    if (uiLedIdx >= m_staLedPtrs.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLedIdx >= m_staLedPtrs.size(). uiLedIdx = %d", uiLedIdx);
        return false;
    }

    if (false == m_staLedPtrs.at(uiLedIdx)->SetState(uiState)) {
        LOG_WRITE(EN_LOG_ERROR, "set led state error. m_uiHwSubIdx = %u, uiLedIdx: %u, uiState = %u", m_uiHwSubIdx, uiLedIdx, uiState);
        return false;
    }

    return true;
}

bool
CSlotDriver::GetAllLedState(CLedSta *pastLedsta)
{
    int i = 0;

    for (i = 0; i < (int)m_staLedPtrs.size(); ++i) {
        if (false == m_staLedPtrs.at(i)->GetState(pastLedsta[i].uiState)) {
            LOG_WRITE(EN_LOG_ERROR, "get led state error. m_uiHwSubIdx = %u, uiLedIdx: %d, uiState = %u", m_uiHwSubIdx, i, pastLedsta[i].uiState);
            continue;
        }
    }

    return true;
}


bool
CSlotDriver::EepromRead(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::Read +++");
    std::cout << "CSlotDriver::EepromRead +++" << std::endl;

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot eeprom driver is nullptr.");
        return false;
    }

    std::cout << "CSlotDriver::EepromRead +++ 1" << std::endl;

    if (false == m_stpEEpromPtr->Read(uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read slot eeprom error: %u.", m_uiHwSubIdx);
        return false;
    }


    for (uint32_t uiIdx = 0; uiIdx < uiLen; uiIdx++) {
        printf("%02x ", pucData[uiIdx]);

        if ((uiIdx + 1) % 32 == 0) {
            printf("\n");
        }
    }

    printf("\n");

    std::cout << "CSlotDriver::EepromRead ---" << std::endl;

    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::Read ---");

    return true;
}


bool
CSlotDriver::EepromWrite(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::Write +++");

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "slot eeprom driver is nullptr.");
        return false;
    }


    if ( false == CEEpromOperator::Write(m_stpEEpromPtr, uiAddr, pucData, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write slot eeprom error: %u.", m_uiHwSubIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CSlotDriver::Write ---");

    return true;
}


bool
CLogicSlotDriver::IsOnline(void)
{
    bool bOnline = false;

    if (nullptr == m_stpSlotBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpSlotBoardPtr is nullptr.");
        return false;
    }

    bOnline = m_stpSlotBoardPtr->IsSlotOnline(m_uiHwSubIdx);

    return bOnline;

}

bool
CLogicSlotDriver::IsPlug(void)
{
    bool bPlug = false;

    if (nullptr == m_stpSlotBoardPtr) {
        return false;
    }

    bPlug = m_stpSlotBoardPtr->IsSlotPlug(m_uiHwSubIdx);

    return bPlug;

}

bool
CLogicSlotDriver::ClearPlug(void)
{
    if (nullptr == m_stpSlotBoardPtr) {
        return false;
    }

    if ( false == m_stpSlotBoardPtr->ClearSlotPlug(m_uiHwSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "Clear plug flag error. m_uiHwSubIdx = %d", m_uiHwSubIdx);
        return false;
    }

    return true;
}


bool
CLogicSlotDriver::GetCardType(uint32_t &ruiType)
{
    if (nullptr == m_stpSlotBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrvPtr is nullptr.");
        return false;
    }

    if (false == m_stpSlotBoardPtr->GetCardType(m_uiHwSubIdx, ruiType)) {
        return false;
    }

    return true;

}

bool
CLogicSlotDriver::Reset(uint32_t uiResetType)
{
    LOG_WRITE(EN_LOG_NOTICE, "CLogicSlotDriver::Reset +++, uiResetType = %d", uiResetType);

    if (RST_MODE_COLD == uiResetType) {
#if 0
        m_stpPhy->Reset();

        CDelay::Delay(20, MS);

        m_stpPhy->Config();
#endif
    }

    LOG_WRITE(EN_LOG_NOTICE, "CLogicSlotDriver::Reset +++, uiResetType = %d", uiResetType);

    return true;
}

void
CUsbSlotDriver::__Init()
{
    const int buffersize = UEVENT_BUFFER_SIZE;
    int32_t iRet;
    int on = 1;

    if (m_stSpiFt.Register()) {
        LOG_WRITE(EN_LOG_NOTICE, "Plugged!!!");
        SetOnline(true);
    }


    struct sockaddr_nl snl;
    bzero(&snl, sizeof(struct sockaddr_nl));

    snl.nl_family = AF_NETLINK;
    snl.nl_pid = 0;
    snl.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

    m_iSocket = socket(PF_NETLINK, SOCK_DGRAM | SOCK_NONBLOCK, NETLINK_KOBJECT_UEVENT);

    LOG_WRITE(EN_LOG_NOTICE, "m_iSocket = %d", m_iSocket);
    if (m_iSocket == -1) {
        LOG_WRITE(EN_LOG_ERROR, "m_iSocket == -1");
        return;
    }

    iRet = setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
    if ( iRet < 0) {
        LOG_WRITE(EN_LOG_ERROR, "setsockopet error. errno = %d", errno);
        return;
    }

    iRet = setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (iRet < 0) {
        LOG_WRITE(EN_LOG_ERROR, "setsockopet error. errno = %d", errno);
        return;
    }

    iRet = bind(m_iSocket, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));
    if (iRet < 0) {
        LOG_WRITE(EN_LOG_NOTICE, "bind error. errno = %d", errno);
        return;
    }
}

bool
CUsbSlotDriver::IsPlug()
{
    char buf[UEVENT_BUFFER_SIZE * 2] = {0};

    /* Netlink message buffer */
    int32_t iRet = recv(m_iSocket, &buf, sizeof(buf), 0);

    std::string staRecv(buf);
    if (iRet > 0) {

        LOG_WRITE(EN_LOG_NOTICE, "%s", staRecv.c_str());

        std::size_t foundAdd = staRecv.find("add@");
        std::size_t foundRemove = staRecv.find("remove@");
        std::size_t foundUsb = staRecv.find("usb1");

        if ((foundAdd != std::string::npos) && (foundUsb != std::string::npos)) {
            SetOnline(true);
        } else if ((foundRemove != std::string::npos) && (foundUsb != std::string::npos)) {
            SetOnline(false);
        }
    }

    return m_bPlug;
}

bool
CUsbSlotDriver::ClearPlug()
{
    m_bPlug = false;
    return true;
}

bool
CUsbSlotDriver::IsOnline()
{
    return m_bOnline;
}



