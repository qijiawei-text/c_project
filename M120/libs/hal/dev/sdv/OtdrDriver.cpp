/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OtdrDriver.h"
#include "File.h"
#include "OtdrRsp.h"
#include "ElapseTimer.h"
#include "DevThread.h"

/* accerlink otdr distance unit is km, openconfig unit is m */
#define DEF_DISTANCE_UNIT       (1000)

/*==============================function=========================*/

bool
COtdrUartDriver::__Scan()
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::__Scan ++");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->StartScan(staMsg);

    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::__Scan --");
    return true;

}


bool
COtdrUartDriver::__GetState(bool &rbIdle)
{
    LOG_WRITE(EN_LOG_TRACE, "COtdrUartDriver::__GetState +++");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetState(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<COtdrStateRspHandler>(m_stpProtoPtr, &rbIdle);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "COtdrUartDriver::__GetState ---");
    return true;
}



bool
COtdrUartDriver::__GetEventTop(COtdrEventTop &rstEventTop)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::__GetEventTop ++");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetEventTop(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<COtdrEventTopRspHandler>(m_stpProtoPtr, &rstEventTop);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    return true;
}


bool
COtdrUartDriver::__GetEventLine(uint32_t ulEventId, COtdrEventLine &rstEventLine)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::__GetEventLine ++");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetEventLine(staMsg, ulEventId);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<COtdrEventLineRspHandler>(m_stpProtoPtr, &rstEventLine);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::__GetEventLine --");
    return true;
}


bool
COtdrUartDriver::IsOnline(void)
{

    return true;
}

bool
COtdrUartDriver::GetMfg(CMfg &rstMfg)
{

#define ACCELINK_OTDR_MFG_MAX_LEN (1024 * 5)

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::GetMfg +++");
    CElapseTimer::Start();

    if (nullptr == m_stpProtoPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    memset(&rstMfg, 0, sizeof(rstMfg));
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<COtdrMfgRspHandler>(
        ACCELINK_OTDR_MFG_MAX_LEN, m_stpProtoPtr, &rstMfg);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "COtdrUartDriver::__SendCmd error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

    printf("Accelink acFwVer %s\n", rstMfg.acFwVer);

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::GetMfg ---");

    CElapseTimer::Stop();
    return true;

}


bool
COtdrUartDriver::GetStatusData(COtdrStatusData &rstData)
{
    return true;
}

bool
COtdrUartDriver::GetCfg(COtdrCfgData &rstData)
{

    return true;
}


bool
COtdrUartDriver::GetEvent(COtdrStatusData &rstData)
{
    uint32_t i = 0;
    int j = 0;
    const int iRetry = 5;

    LOG_WRITE(EN_LOG_NOTICE, "GetEvent ++");

    /* get event from otdr, retry times */
    for (j = 0; j < iRetry; j++) {
        if (__GetEventTop(rstData.stEventTop)) {
            break;
        }
    }

    if (j >= iRetry) {
        LOG_WRITE(EN_LOG_ERROR, "__GetEventTop error");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Event cnt %d", rstData.stEventTop.ulCnt);

    if (rstData.stEventTop.ulCnt > 99) {
        return false;
    }

    for (i = 0; i < rstData.stEventTop.ulCnt; i++) {
        if (!__GetEventLine(i + 1, rstData.astEventLine[i])) {
            return false;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "GetEvent --");

    return true;
}


bool
COtdrUartDriver::GetSor()
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::GetSor ++");

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetSor(staMsg);

    CElapseTimer::Start();

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<COtdrSorRspHandler>(4, m_stpProtoPtr, OTDR_SOR_FILE);
    stpRspHandler->SetTimeOut(1000);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::GetSor --");
    return true;

}


bool
COtdrUartDriver::StartScan(void)
{
    uint32_t i = 0;
    bool bIdle = false;

    LOG_WRITE(EN_LOG_NOTICE, "StartScan ++");

    /* send scan command to otdr */
    if (!__Scan()) {
        LOG_WRITE(EN_LOG_ERROR, "__Scan error.");
        return false;
    }

    /* loop check otdr state to idle */
    CDelay::Delay(10, S);

    LOG_WRITE(EN_LOG_NOTICE, "Check scan state start");

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
COtdrUartDriver::SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    /* openconfig otdr distance unit is meter */
    ulDistRange = (uint32_t)round((double)ulDistRange / DEF_DISTANCE_UNIT);

    if (ulDistRange > 0 && ulDistRange <= 25) {
        dSamplingResolution = 0.64;
    } else if (ulDistRange > 25 && ulDistRange <= 50) {
        dSamplingResolution = 5.12;
    } else if (ulDistRange > 50 && ulDistRange <= 75) {
        dSamplingResolution = 25.6;
    } else if (ulDistRange > 75 && ulDistRange <= 150) {
        dSamplingResolution = 106.24;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "DistRange = %d error. ", ulDistRange);
    }


    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::SetPara ++ %d %d %f", ulDistRange, ulPulseWidth, dSamplingResolution);

    std::string staMsg;
    m_stpProtoPtr->SetPara(staMsg, ulDistRange, ulPulseWidth, dSamplingResolution);

    return __SendSetCmd(staMsg);

}


bool
COtdrUartDriver::SetUserIOR(double dUserIOR)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::SetUserIOR ++ %f", dUserIOR);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    std::string staRsp(OTDR_READ_NUM, 0);
    m_stpProtoPtr->SetUior(staMsg, dUserIOR);

    return __SendSetCmd(staMsg);

}


bool
COtdrUartDriver::SetMeasuringTime(uint32_t ulMeasuringTime)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::SetMeasuringTime ++ %d", ulMeasuringTime);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    std::string staRsp(OTDR_READ_NUM, 0);
    m_stpProtoPtr->SetAcqt(staMsg, ulMeasuringTime);

    return __SendSetCmd(staMsg);
}


bool
COtdrUartDriver::SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::SetThr ++ %f %f %f", dRefThr, dSpliceThr, dFiberEndThr);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetTref(staMsg, dRefThr);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    m_stpProtoPtr->SetTlos(staMsg, dSpliceThr);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    dFiberEndThr = 5.5;
    m_stpProtoPtr->SetTeof(staMsg, (uint32_t)dFiberEndThr);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::SetThr --");

    return true;

}

bool
COtdrUartDriver::SetScanMode(uint32_t ulScanMode)
{
    return true;
}

bool
COtdrUartDriver::GetTemp(COtdrTemp &rstTemp)
{
    return true;
}

bool
COtdrUartDriver::Reset(uint8_t ucResetType)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::Reset ++ %d", ucResetType);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        LOG_WRITE(EN_LOG_NOTICE, "m_uiResetHwSubIdx = %d", m_uiResetHwSubIdx);
        return m_stpResetBoard->Reset(m_uiResetHwSubIdx);

    } else {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(staMsg);
        if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)staMsg.data(), staMsg.size())) {
            LOG_WRITE(EN_LOG_ERROR, "Reset write error.");
            return false;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::Reset --");
    return true;

}

bool
COtdrUartDriver::Upgrade(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::Upgrade ++ %s", pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;
    uint32_t uiLen = 0;

    std::string staStartMsg, staEndMsg;
    std::string staRsp(OTDR_READ_NUM, 0);
    const char *pcSendCmd = "TRANSF ";
    /* uint32_t uiPos = 0; */

    const std::shared_ptr<CEndToken> stpEndToken = std::static_pointer_cast<CEndToken>(
        std::make_shared<COtdrEndToken>());

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>();
    stpRspHandler->SetEndToken(stpEndToken);
    stpRspHandler->SetTimeOut(50 * 20);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    /* start upg */
    m_stpProtoPtr->UpgStart(staStartMsg);
    if (!__SendSetCmd(staStartMsg)) {
        goto ERR_LABEL;
    }

    /* send data */
    if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)pcSendCmd, strlen(pcSendCmd))) {
        LOG_WRITE(EN_LOG_ERROR, "send cmd write error.");
        goto ERR_LABEL;
    }

    /* length (Big endian) */
    uiLen = htonl(iFileLen);
    if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)&uiLen, sizeof(uint32_t))) {
        LOG_WRITE(EN_LOG_ERROR, "write length error.");
        goto ERR_LABEL;
    }

#if 1
    LOG_WRITE(EN_LOG_NOTICE, "Total Len = %d", iFileLen);
    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, pucBuffer, iFileLen, nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch Uart(%d) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }
#else

#define WRITE_LEN_PER_PACKET (256 * 4)

    LOG_WRITE(EN_LOG_NOTICE, "Total Len = %d", iFileLen);

    /* CDevThread::SetLoopEnable(false); */

    while (iFileLen > WRITE_LEN_PER_PACKET) {
        if (!m_stpDrvPtr->Write(m_uiHwSubIdx, pucBuffer + uiPos, WRITE_LEN_PER_PACKET)) {
            LOG_WRITE(EN_LOG_ERROR, "Switch Uart(%d) ReadWrite error.", m_uiHwSubIdx);
            return false;
        }

        iFileLen -= WRITE_LEN_PER_PACKET;
        uiPos += WRITE_LEN_PER_PACKET;

        LOG_WRITE(EN_LOG_DEBUG, "uiPos = %d", uiPos);
    }

    LOG_WRITE(EN_LOG_DEBUG, "Last Packet Len = %d", iFileLen);
    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, pucBuffer + uiPos, iFileLen, nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch Uart(%d) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    /* CDevThread::SetLoopEnable(true); */
#endif

    /* end upg */
    m_stpProtoPtr->UpgEnd(staEndMsg);
    LOG_WRITE(EN_LOG_NOTICE, "staEndMsg: %s", staEndMsg.c_str());
    if (!__SendSetCmd(staEndMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "UpgEndCmd error.");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COtdrUartDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COtdrUartDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}



bool
COtdrUartDriver::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "COtdrUartDriver::DbgRead ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(uiLen);
    if (!m_stpDrvPtr->Read(m_uiHwSubIdx, (uint8_t *)pucBuf, std::make_shared<CRspHandler>(uiLen))) {
        LOG_WRITE(EN_LOG_ERROR, "Otdr read error.");
        return false;
    }

    memcpy(pucBuf, stpRspHandler->Data(), uiLen);

    LOG_WRITE(EN_LOG_DEBUG, "COtdrUartDriver::DbgRead -- %s", pucBuf);

    return true;
}

bool
COtdrUartDriver::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "COtdrUartDriver::DbgWrite ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Otdr write error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COtdrUartDriver::DbgWrite --");

    return true;
}

bool
COtdrUartDriver::SetStartOffset(COtdrStartOffset *pstStartOffset)
{
    return true;
}

bool
COtdrUartDriver::GetStartOffset(COtdrStartOffset &rstStartOffset)
{
    return true;
}

bool
COtdrUartDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");
        return false;
    }

    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, (uint8_t *)staMsg.data(), staMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch Uart(%d) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    return true;
}

bool
COtdrUartDriver::__SendSetCmd(std::string staMsg, uint32_t uiTimeout)
{
    bool bInProgress = false;
    const std::shared_ptr<CEndToken> stpEndToken = std::static_pointer_cast<CEndToken>(
        std::make_shared<COtdrEndToken>());

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>();
    stpRspHandler->SetEndToken(stpEndToken);
    stpRspHandler->SetTimeOut(uiTimeout);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->ProcRsp(staRsp, bInProgress)) {
        LOG_WRITE(EN_LOG_ERROR, "Otdr Rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "__SendSetCmd done.");
    return true;
}



