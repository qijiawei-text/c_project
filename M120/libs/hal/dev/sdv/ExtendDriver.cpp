/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ExtendDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "File.h"
#include "ExtendDriver.h"
#include "ElapseTimer.h"

/*==============================function=========================*/
bool
CExtendUartDriver::IsOnline(void)
{

    return true;
}

bool
CExtendUartDriver::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::GetMfg +++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "CExtendUartDriver::GetMfg error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    LOG_WRITE(EN_LOG_NOTICE, "extend: %s", staRsp.c_str());

    if (m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {

        LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
        LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
        LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
        LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
        LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
        LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
        LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
        LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
        LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

        printf("externd acFwVer %s\n", rstMfg.acFwVer);

        LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::GetMfg ---");
        return true;
    }

    LOG_WRITE(EN_LOG_ERROR, "CExtendUartDriver::GetMfg ---");

    return false;

}

bool
CExtendUartDriver::GetStatusData(CExtendStatusData &rstData)
{
    EXTEND_GETALLINFO_ST stAllInfo;
    uint32_t uiLen = 0;
    int16_t sCheckSum = 0;
    int16_t *psStatusData = NULL;

    LOG_WRITE(EN_LOG_DEBUG, "CExtendUartDriver::GetStatusData +++");
    /* CElapseTimer::Start(); */

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetStatusData(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(stAllInfo));
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "CExtendUartDriver::GetStatusData error.");
        return false;
    }

    /* printf("Extend rsp len: %d\n", stpRspHandler->Pos()); */

#if 0
    for (uint32_t uiIdx = 0; uiIdx < sizeof(stAllInfo) + 3; uiIdx++) {
        printf("%02x ", *((uint8_t *)stpRspHandler->Data() + uiIdx));

        if ((uiIdx + 1) % 32 == 0) {
            printf("\n");
        }
    }
    printf("\n");
#endif

    memset(&stAllInfo, 0, sizeof(stAllInfo));
    memcpy((uint8_t *)&stAllInfo, (uint8_t *)stpRspHandler->Data(), sizeof(stAllInfo));

    psStatusData = (int16_t *)&stAllInfo;
    uiLen = sizeof(stAllInfo) / sizeof(int16_t);

    for (int i = 0; i < (int)uiLen - 1; i++) {
        sCheckSum += (int16_t)(*(psStatusData + i));
    }

    if (sCheckSum != stAllInfo.sCheckSum) {
        LOG_WRITE(EN_LOG_ERROR, "checksum error. sCheckSum = %x, stAllInfo.sCheckSum = %x", sCheckSum, stAllInfo.sCheckSum);
        return false;
    }

    /* CByteOrder::ArrayNtohs(psStatusData, uiLen); */
#if 0
    for (int i = 0; i < 16; i++) {
        printf("%d ", rstData.asPD[i]);
    }
    printf("\n");
#endif

    memcpy(&rstData.asPD, &stAllInfo.stInfo.asPdPower, sizeof(stAllInfo.stInfo.asPdPower));
    memcpy(&rstData.asSwitchPosActual, &stAllInfo.stInfo.asSwAct, sizeof(stAllInfo.stInfo.asSwAct));
    memcpy(&rstData.asVoaAttnActual, &stAllInfo.stInfo.asVoaAct, sizeof(stAllInfo.stInfo.asVoaAct));

    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData ---");

    /* CElapseTimer::Stop(); */

    return true;
}

bool
CExtendUartDriver::SetSwitchPos(uint32_t ulSubIdx, uint32_t ulPos)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchPos(ulSubIdx, staMsg, ulPos);

    return __SendSetCmd(staMsg);
}

bool
CExtendUartDriver::SetSfp(uint32_t ulSubIdx, uint32_t ulVal)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    /* 0 Enable, 1 Disable ?? */
    m_stpProtoPtr->SetSfpEnable(ulSubIdx, staMsg, ulVal);
    LOG_WRITE(EN_LOG_NOTICE, "SetSfp %d %s", ulSubIdx, staMsg.c_str());

    return __SendSetCmd(staMsg);
}

bool
CExtendUartDriver::SetVoaAttn(uint32_t ulSubIdx, uint32_t ulVal)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetVoaAttn(ulSubIdx, staMsg, ulVal);
    LOG_WRITE(EN_LOG_NOTICE, "Set VOA(%d) Attn = %d", ulSubIdx, ulVal);

    return __SendSetCmd(staMsg);
}

bool
CExtendUartDriver::SetVoaMode(uint32_t ulSubIdx, uint32_t ulMode)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetVoaMode(ulSubIdx, staMsg, ulMode);
    LOG_WRITE(EN_LOG_NOTICE, "Set VOA(%d) Mode = %d", ulSubIdx, ulMode);

    return __SendSetCmd(staMsg);
}



bool
CExtendUartDriver::Reset(uint8_t ucResetType)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::Reset ++ %d", ucResetType);

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        return m_stpResetBoardPtr->Reset(m_uiResetHwSubIdx);
    } else if (RST_MODE_WARM == ucResetType) {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(staMsg);

        return __SendSetCmd(staMsg);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "ResetType(%d) error.", ucResetType);
        return false;
    }
}

#if 0
bool
CExtendUartDriver::Upgrade(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::Upgrade ++ %s", pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;

    std::string staMsg;
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }


    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Upgrade(staMsg);

    m_stpDrvPtr->Lock();
    m_stpDrvPtr->Switch(m_uiHwSubIdx);

    /* Xmodem */
    if (!CFile::XModem(m_stpDrvPtr, staMsg, m_uiHwIdx, pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Xmodem %d error, len %d", m_uiHwIdx, iFileLen);
        m_stpDrvPtr->Unlock();
        goto ERR_LABEL;
    }

    if (!__UpgradeEnd()) {
        LOG_WRITE(EN_LOG_ERROR, "__UpgradeEnd error.");
        m_stpDrvPtr->Unlock();
        goto ERR_LABEL;
    }

    m_stpDrvPtr->Unlock();

    /* CDelay::Delay(3, S); */

    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CExtendUartDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
CExtendUartDriver::__UpgradeEnd()
{
    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};

    /* Sends EOT to the module finally                                          */
    aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;

    for (uint32_t uiIdx = 0; uiIdx < XMD_RETRY_MAX; uiIdx++) {

        std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(1);

        if (!m_stpDrvPtr->ReadWriteUnlock(m_uiHwIdx, aucXmdPkg, XMD_EOT_IDX_MAX, nullptr, stpRspHandler)) {
            LOG_WRITE(EN_LOG_ERROR, "Send Command error.");
            return false;
        }

        uint8_t *pucRspData = (uint8_t *)stpRspHandler->Data();

        if (XMD_ACK == pucRspData[0]) {
            LOG_WRITE(EN_LOG_NOTICE, "Exit Xmode.");
            return true;
        } else if (XMD_CAN == pucRspData[0]) {
            LOG_WRITE(EN_LOG_ERROR, "Response CAN.");
            return false;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Invalid response [%X]", (uint32_t)pucRspData[0]);
            CDelay::Delay(20, MS);
            continue;
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "Exit xmode error.");
    return false;
}

#else
bool
CExtendUartDriver::Upgrade(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::Upgrade ++ %s", pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;

    std::string staMsg;
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }


    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Upgrade(staMsg);

    /* Xmodem */
    if (!CFile::XModem(m_stpDrvPtr, staMsg, m_uiHwIdx, pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Xmodem %d error, len %d", m_uiHwIdx, iFileLen);
        goto ERR_LABEL;
    }

    if (!__UpgradeEnd()) {
        LOG_WRITE(EN_LOG_ERROR, "__UpgradeEnd error.");
        goto ERR_LABEL;
    }

    /* CDelay::Delay(3, S); */

    LOG_WRITE(EN_LOG_NOTICE, "CExtendUartDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CExtendUartDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}


bool
CExtendUartDriver::__UpgradeEnd()
{
    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};

    /* Sends EOT to the module finally                                          */
    aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;

    for (uint32_t uiIdx = 0; uiIdx < XMD_RETRY_MAX; uiIdx++) {

        std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(1);

        if (!m_stpDrvPtr->ReadWrite(m_uiHwIdx, aucXmdPkg, XMD_EOT_IDX_MAX, nullptr, stpRspHandler)) {
            LOG_WRITE(EN_LOG_ERROR, "Send Command error.");
            return false;
        }

        uint8_t *pucRspData = (uint8_t *)stpRspHandler->Data();

        if (XMD_ACK == pucRspData[0]) {
            LOG_WRITE(EN_LOG_NOTICE, "Exit Xmode.");
            return true;
        } else if (XMD_CAN == pucRspData[0]) {
            LOG_WRITE(EN_LOG_ERROR, "Response CAN.");
            return false;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Invalid response [%X]", (uint32_t)pucRspData[0]);
            CDelay::Delay(20, MS);
            continue;
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "Exit xmode error.");
    return false;
}


#endif





bool
CExtendUartDriver::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CExtendUartDriver::DbgRead ++ %d %d %d", uiDevIdx, m_uiHwIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Read(m_uiHwIdx, (uint8_t *)pucBuf, std::make_shared<CRspHandler>(uiLen))) {
        LOG_WRITE(EN_LOG_ERROR, "Extend read error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CExtendUartDriver::DbgRead -- %s", pucBuf);

    return true;
}

bool
CExtendUartDriver::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CExtendUartDriver::DbgWrite ++ %d %d %d", uiDevIdx, m_uiHwIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Write(m_uiHwIdx, (uint8_t *)pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Extend write error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CExtendUartDriver::DbgWrite --");

    return true;
}


std::shared_ptr<CRspHandler>
CExtendUartDriver::__RspHandler()
{
    std::shared_ptr<CEndToken> stpEndToken = std::static_pointer_cast<CEndToken>(std::make_shared<CMolexEndToken>());
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>();
    stpRspHandler->SetEndToken(stpEndToken);

    return stpRspHandler;
}

bool
CExtendUartDriver::__SendCmd(std::string &rstaMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if (!m_stpDrvPtr->ReadWrite(m_uiHwIdx, (uint8_t *)rstaMsg.data(), rstaMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch Uart(%u) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    return true;
}


bool
CExtendUartDriver::__SendSetCmd(std::string &rstaMsg)
{

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(rstaMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->ProcRsp(staRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "__SendSetCmd done.");
    return true;
}



