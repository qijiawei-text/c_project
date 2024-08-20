/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MoldxOtdr.cpp
   Author:        Zhu Weian
   Date:          2021-02-14
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

#include "MolexOtdr.h"
#include "File.h"

bool
CMolexOtdr::__Scan()
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::__Scan ++");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->StartScan(staMsg);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Start Scan Error.");
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::__Scan --");

    return true;

}


bool
CMolexOtdr::__GetState(bool &rbIdle)
{
    LOG_WRITE(EN_LOG_TRACE, "CMolexOtdr::__GetState ++");


    std::string staMsg;
    m_stpProtoPtr->GetState(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_ACQ_STATUS));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__GetState error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetStateRsp(staRsp, rbIdle)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStateRsp error.");
        return false;
    }

    return true;
}



bool
CMolexOtdr::GetEvent(COtdrStatusData &rstData)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetEvent +++");

    std::string staMsg;
    m_stpProtoPtr->GetEventInfo(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_EVENTS_INFO));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetEvent error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetEventInfoRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetEvent ---");

    return true;
}



bool
CMolexOtdr::IsOnline(void)
{

    return true;
}

bool
CMolexOtdr::GetVol(uint16 &rusVol)
{
    std::string staMsg;
    m_stpProtoPtr->GetVolCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(uint16_t), 5);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetVol error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetVolRsp(staRsp, rusVol)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetVol rsp error.");
        return false;
    }

    return true;
}

bool
CMolexOtdr::GetAlarm(uint16_t &rusAlarm)
{
    std::string staMsg;
    m_stpProtoPtr->GetAlarmCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(uint16_t), 5);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetAlarmRsp(staRsp, rusAlarm)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetAlarm rsp error.");
        return false;
    }

    return true;
}


bool
CMolexOtdr::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetMfg +++");

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    memset(&rstMfg, 0, sizeof(rstMfg));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_MOLEX_OTDR_MFG_INFO));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetMfg error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetMfg rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSN = %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acPN = %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acModuleType = %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acBootVer = %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSwVer = %s", rstMfg.acSwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFwVer = %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acHwVer = %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acCaliDate = %s", rstMfg.acCaliDate);
    printf("Molex acFwVer %s\n", rstMfg.acFwVer);
#if 1
    COtdrCfgData stCfg;
    memset(&stCfg, 0, sizeof(stCfg));
    GetCfg(stCfg);

#endif
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetMfg ---");
    return true;

}

bool
CMolexOtdr::GetCfg(COtdrCfgData &rstCfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetCfg +++");

    COtdrCfgData stCfg = {0};
    if (!__GetCfg(stCfg)) {
        LOG_WRITE(EN_LOG_ERROR, "__GetCfg error.");
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetCfg ---");
    return true;
}

bool
CMolexOtdr::SetCfg(COtdrCfgData &rstCfg)
{
    COtdrCfgData stCfg = {0};
    if (!__GetCfg(stCfg)) {
        LOG_WRITE(EN_LOG_ERROR, "__GetCfg error.");
        return false;
    }

    /* Set Cfg */
    stCfg.ulScanEndPos = rstCfg.ulScanEndPos;
    stCfg.ulExpPulseWidth = rstCfg.ulExpPulseWidth;
    /* rstCfg.dSamplingResolution; */

    stCfg.fReflIdx = rstCfg.fReflIdx;
    stCfg.ulExpScanTime = rstCfg.ulExpScanTime;

    stCfg.fReflEventThr = rstCfg.fReflEventThr;
    stCfg.fNonReflEventThr = rstCfg.fNonReflEventThr;
    /* rstCfg.dFiberEndThr */

    __SetCfg(stCfg);


    return true;
}

bool
CMolexOtdr::GetStatusData(COtdrStatusData &rstData)
{
#if 0
    if (m_bModuleAlarmDebug) {
        return false;
    }

    CMfg stMfg = {0};
    return GetMfg(stMfg);
#endif

    return true;
}


bool
CMolexOtdr::GetSor()
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetSor ++");

    uint32_t uiPackId = 1;
    uint32_t uiTotalPackId = 1;

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staSorFile;

    for (uiPackId = 1; uiPackId <= uiTotalPackId; uiPackId++) {

        std::string staMsg;
        m_stpProtoPtr->GetSorTemp(staMsg, uiPackId);

        std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(4004);
        __SendCmd(staMsg, stpRspHandler);

        std::string staRsp;
        stpRspHandler->Copy(staRsp);

        m_stpProtoPtr->GetSorRsp(staRsp, uiTotalPackId);

        LOG_WRITE(EN_LOG_NOTICE, "staRsp.size() = %lu", staRsp.size());
        staSorFile.append(staRsp.data() + 4, staRsp.size() - 4);

    }

#if 1
    if (!CFile::Write(OTDR_SOR_FILE, (uint8_t *)staSorFile.data(), staSorFile.size())) {
        LOG_WRITE(EN_LOG_ERROR, "write file error. len %lu", staSorFile.size());
        goto ERR_LABEL;
    }
#endif
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetSor --");
    return true;

ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "CMolexOtdr::GetSor --");

    return false;

}


bool
CMolexOtdr::StartScan(void)
{
    uint32_t i = 0;
    bool bIdle = false;

    LOG_WRITE(EN_LOG_NOTICE, "StartScan ++");

    /* send scan command to otdr */
    if (!__Scan()) {
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "Check scan state start");

    /* loop check otdr state to idle */
    CDelay::Delay(10, S);

    for (i = 0; i < 240; i++) {

        if (!__GetState(bIdle)) {
            LOG_WRITE(EN_LOG_ERROR, "__GetState error.");
            CDelay::Delay(1, S);
            continue;
        }

        if (bIdle) {
            break;
        }
        CDelay::Delay(1, S);
    }

    LOG_WRITE(EN_LOG_NOTICE, "StartScan --");

    return true;
}

bool
CMolexOtdr::SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetPara ++ %d %d %f", ulDistRange, ulPulseWidth, dSamplingResolution);

    COtdrCfgData stCfg = {0};
    __GetCfg(stCfg);

    stCfg.ulScanEndPos = ulDistRange;
    stCfg.ulExpPulseWidth = ulPulseWidth;

    __SetCfg(stCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetPara --");

    return true;

}


bool
CMolexOtdr::SetUserIOR(double dUserIOR)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetUserIOR ++ %f", dUserIOR);

    COtdrCfgData stCfg = {0};
    __GetCfg(stCfg);

    stCfg.fReflIdx = dUserIOR;

    __SetCfg(stCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetUserIOR --");

    return true;

}


bool
CMolexOtdr::SetMeasuringTime(uint32_t ulMeasuringTime)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetMeasuringTime ++ %d", ulMeasuringTime);

    COtdrCfgData stCfg = {0};
    __GetCfg(stCfg);

    stCfg.ulExpScanTime = ulMeasuringTime;

    __SetCfg(stCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetMeasuringTime --");

    return true;

}


bool
CMolexOtdr::SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetThr ++ %f %f %f", dRefThr, dSpliceThr, dFiberEndThr);

    COtdrCfgData stCfg = {0};
    __GetCfg(stCfg);

    stCfg.fReflEventThr = dRefThr;
    stCfg.fNonReflEventThr = dSpliceThr;

    __SetCfg(stCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetThr --");

    return true;

}

bool
CMolexOtdr::SetScanMode(uint32_t ulScanMode)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetScanMode ++ %d", ulScanMode);

    COtdrCfgData stCfg = {0};
    __GetCfg(stCfg);

    stCfg.ulScanMode = ulScanMode;

    __SetCfg(stCfg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetScanMode --");

    return true;

}

bool
CMolexOtdr::GetTemp(COtdrTemp & rstTemp)
{
    std::string staMsg;
    m_stpProtoPtr->GetTemp(staMsg);

    memset(&rstTemp, 0, sizeof(rstTemp));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_TEMP), 5);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetTemp error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetTempRsp(staRsp, rstTemp)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetTemp rsp error.");
        return false;
    }

    return true;
}

bool
CMolexOtdr::Reset(uint8_t ucResetType)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::Reset ++ %d", ucResetType);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == m_stpResetBoard)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {

        LOG_WRITE(EN_LOG_NOTICE, "m_uiResetHwSubIdx = %d", m_uiResetHwSubIdx);
        return m_stpResetBoard->Reset(m_uiResetHwSubIdx);

    } else {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(staMsg);

        __SendSetCmd(staMsg);
    }

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::Reset --");

    return true;

}

bool
CMolexOtdr::Upgrade(const char *pcFilePath)
{
    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;
    uint8_t *pucDataPos = nullptr;
    uint32_t ulWriteLen = 0;
    uint32_t ulLeaveLen = 0;

    std::string staMsg;
    std::string staDataMsg;


#define DEF_OTDR_PER_PACKET_LEN                              (3 * 1024)

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::Upgrade +++");

    if ((nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    /* start upgrade */

    m_stpProtoPtr->UpgStart(staMsg);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Otdr Start Upgrade error.");
        goto ERR_LABEL;
    }

    /* send data */
    pucDataPos = pucBuffer;
    ulLeaveLen = iFileLen;

    while (ulLeaveLen > 0) {

        LOG_WRITE(EN_LOG_DEBUG, "ulLeaveLen = %d", ulLeaveLen);

        printf("ulLeaveLen = %d\n", ulLeaveLen);


        if (ulLeaveLen > DEF_OTDR_PER_PACKET_LEN) {
            ulWriteLen = DEF_OTDR_PER_PACKET_LEN;
        } else {
            ulWriteLen = ulLeaveLen;
        }

        m_stpProtoPtr->SendDataCmd(staDataMsg, pucDataPos, ulWriteLen);

        if (!__SendSetCmd(staDataMsg)) {
            LOG_WRITE(EN_LOG_ERROR, "Otdr SendDataCmd error.");
            goto ERR_LABEL;
        }

        LOG_WRITE(EN_LOG_DEBUG, "ulWriteLen = %d", ulWriteLen);

        pucDataPos += ulWriteLen;
        ulLeaveLen -= ulWriteLen;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::UpgEnd");
    /* end upgrade */
    m_stpProtoPtr->UpgEnd(staMsg);
    if (!__SendSetCmd(staMsg, 500)) {

        LOG_WRITE(EN_LOG_ERROR, "Otdr end upgrade cmd error.");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CMolexOtdr::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}



bool
CMolexOtdr::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CMolexOtdr::DbgRead ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Read(m_uiHwSubIdx, (uint8_t *)pucBuf, std::make_shared<CRspHandler>(uiLen))) {
        LOG_WRITE(EN_LOG_ERROR, "Otdr read error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CMolexOtdr::DbgRead -- %s", pucBuf);

    return true;
}

bool
CMolexOtdr::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CMolexOtdr::DbgWrite ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Otdr write error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CMolexOtdr::DbgWrite --");

    return true;
}

bool
CMolexOtdr::SetStartOffset(COtdrStartOffset *pstStartOffset)
{
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetStartOffset ++ ");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }


    std::string staMsg;
    m_stpProtoPtr->SetStartOffsetCmd(staMsg, *pstStartOffset);

    __SendSetCmd(staMsg);

    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::SetStartOffset --");

    return true;
}

bool
CMolexOtdr::GetStartOffset(COtdrStartOffset &rstStartOffset)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetStartOffset +++");
    m_stpProtoPtr->GetStartOffsetCmd(staMsg);

    memset(&rstStartOffset, 0, sizeof(rstStartOffset));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_TEMP));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetStartOffset error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (!m_stpProtoPtr->GetStartOffsetRsp(staRsp, rstStartOffset)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetStartOffset rsp error.");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "CMolexOtdr::GetStartOffset ---");
    return true;
}

int
CMolexOtdr::Switch(uint32_t uiChip)
{
    uint8_t ucDataW = 1;

    pthread_mutex_lock(&m_spi_mutex[m_uiHwSubIdx]);
#if 0
    ucDataW |= 1 << uiChip;
    m_stpLogicDrvPtr->Write(SLC_SPI_SELECT, &ucDataW, 1);
#else
    m_stpLogicDrvPtr->Write(SLC_SPI_SELECT + m_uiHwSubIdx, &ucDataW, 1);
#endif
    return OPLK_OK;
}

int
CMolexOtdr::SwitchClose(uint32_t uiChip)
{
    uint8_t ucDataW = 0;
#if 0
    m_stpLogicDrvPtr->Read(SLC_SPI_SELECT, &ucDataW, 1);
    ucDataW &= ~(1 << uiChip);
    m_stpLogicDrvPtr->Write(SLC_SPI_SELECT, &ucDataW, 1);
#else
    m_stpLogicDrvPtr->Write(SLC_SPI_SELECT + m_uiHwSubIdx, &ucDataW, 1);
#endif
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]);

    return OPLK_OK;
}


std::shared_ptr<CRspHandler>
CMolexOtdr::__RspHandler(uint32_t uiLen, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_FRM_READ_HEAD) + uiLen + DEF_FRM_CRC_LEN);
    stpRspHandler->SetProto(m_stpProtoPtr);
    stpRspHandler->SetRetryTimes(uiTimeOut);

    return stpRspHandler;
}


bool
CMolexOtdr::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");
        return false;
    }
    GetMutex().lock();
    Switch(m_uiHwSubIdx);

    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, (uint8_t *)staMsg.data(), staMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "ReadWrite error: %u.", m_uiHwSubIdx);
        SwitchClose(m_uiHwSubIdx);
        GetMutex().unlock();
        return false;
    }
    SwitchClose(m_uiHwSubIdx);
    GetMutex().unlock();
    return true;
}

bool
CMolexOtdr::__SendSetCmd(std::string staMsg, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(0, uiTimeOut);

    return __SendCmd(staMsg, stpRspHandler);
}

bool
CMolexOtdr::__GetCfg(COtdrCfgData &rstCfg)
{
    std::string staMsg;
    m_stpProtoPtr->GetCfgCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_MOLEX_OTDR_CONFIG));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetCfg error.");
        return false;
    }

    memset(&rstCfg, 0, sizeof(rstCfg));

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetCfgRsp(staRsp, rstCfg);

    return true;
}

bool
CMolexOtdr::__SetCfg(COtdrCfgData &rstCfg)
{

    std::string staMsg;
    m_stpProtoPtr->SetCfgCmd(staMsg, rstCfg);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "GetCfg error.");
        return false;
    }

    return true;
}
