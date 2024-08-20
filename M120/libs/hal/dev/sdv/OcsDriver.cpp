/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsDriver.cpp
   Author:        Guo Guangjun
   Date:          2024-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-04] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "File.h"
#include "OcsDriver.h"

/*==============================macro============================*/
#define DEF_OCS_16X16_SWITCH_STR    "16X16"
#define DEF_OCS_32X32_SWITCH_STR    "32X32"
#define DEF_OCS_64X64_SWITCH_STR    "64X64"
#define DEF_OCS_SWITCH_OUTPUT_CHANNEL_VALUE    (0xFF)
#define DEF_OCS_SWITCH_OUTPUT_CHANNEL_BLOCK    (0x0)
/*==============================function=========================*/

static struct timeval m_stStartTime, m_stStopTime;

static void
__StartTime()
{
    gettimeofday(&m_stStartTime, NULL);
}

static bool
__StopTime()
{
    gettimeofday(&m_stStopTime, NULL);

    long elapsed = (m_stStopTime.tv_sec - m_stStartTime.tv_sec) * 1000000 + (m_stStopTime.tv_usec - m_stStartTime.tv_usec);
    elapsed /= 1000;

    LOG_WRITE(EN_LOG_DEBUG, "elapsed time : %ld", elapsed);
    return true;
}


bool
COcsSpiDriver::IsOnline(void)
{

    return true;
}

bool
COcsSpiDriver::GetMfg(CMfg &rstMfg)
{
    uint32_t ulLenR = DEF_OCS_CMD_RESP_MAX_LEN;
    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::GetMfg +++");
    __StartTime();


    memset(&rstMfg, 0, sizeof(rstMfg));

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    std::string staRsp(ulLenR, 0);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(),
                                (uint8_t *)staRsp.data(), ulLenR)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg error. m_uiHwIdx = %u", m_uiHwIdx);

        return false;
    }

    if (false == m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg rsp error. m_uiHwIdx = %u", m_uiHwIdx);

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

    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acProductName = %s", rstMfg.acProductName);

    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::GetMfg ---");

    /* parse switch type by ModuleType */
    ParseSwitchType((const char *)rstMfg.acModuleType);

    __StopTime();

    return true;

}

bool
COcsSpiDriver::GetStatusData(COcsStatusData &rstData)
{
    COcsStatusData stStatusData;

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    if (m_bCommFailDebug) {
        return false;
    }

    memset(&stStatusData, 0, sizeof(stStatusData));

    if (!GetAlarm(stStatusData.stAlm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }

    if (!GetSwitchChannels(stStatusData.stConnsSta)) {
        LOG_WRITE(EN_LOG_ERROR, "GetSwitchChannels error.");
        return false;
    }

    if (!GetLedState(stStatusData.stLedSta)) {
        LOG_WRITE(EN_LOG_ERROR, "GetLedState error.");
        return false;
    }

    if (!GetCaseTemperature(stStatusData.stTempSta)) {
        LOG_WRITE(EN_LOG_ERROR, "GetCaseTemperature error.");
        return false;
    }

    memcpy(&rstData, &stStatusData, sizeof(stStatusData));

    return true;
}

bool
COcsSpiDriver::GetCfgData(COcsCfgData &rstData)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::GetCfgData is NOT implemented.");
    return true;
}

bool
COcsSpiDriver::GetSwitchChannels(COcsConnections &rstData)
{
    uint32_t ulLenR = DEF_OCS_CMD_RESP_MAX_LEN;
    int rc = OPLK_ERR;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetSwitchChannelsCmd(staMsg);

    std::string staRsp(ulLenR, 0);
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), ulLenR);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get switch channels cmd error. m_uiHwIdx: %d", m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }

    if (false == m_stpProtoPtr->GetSwitchChannelsRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get switch channels rsp error.");
        return false;
    }

    /* Replace with actual channels count by switch type */
    rstData.uiCount = GetSwitchChannelsCount();

    LOG_WRITE(EN_LOG_DEBUG, "GetSwitchChannels --");

    return true;
}

bool
COcsSpiDriver::SetSwitchChannels(const COcsConnections &rstData)
{
    COcsConnections stConns;
    char acConnsStr[DEF_BUF_SIZE_512];
    uint32_t i = 0, j = 0;
    uint32_t uiActualChannelMax = GetSwitchChannelsCount();
    uint32_t uiSourceSameCnt = 0;
    uint32_t uiDestSameCnt = 0;

    LOG_WRITE(EN_LOG_DEBUG, "SetSwitchChannels ++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    memset(&stConns, 0, sizeof(stConns));

    /* check data */
    if (rstData.uiCount > uiActualChannelMax) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid switch channels! uiCount: %d, uiActualChannelMax: %d", rstData.uiCount, uiActualChannelMax);
        return false;
    }

    for (i = 0; i < rstData.uiCount; i++) {
        if ((rstData.astConn[i].ucSource > uiActualChannelMax || 0 == rstData.astConn[i].ucSource) ||
            (rstData.astConn[i].ucDestination > uiActualChannelMax || 0 == rstData.astConn[i].ucDestination)) {
            LOG_WRITE(EN_LOG_ERROR, "Invalid switch channels! uiCount: %d, uiActualChannelMax: %d, i: %d, ucSource: %d, ucDestination: %d",
                      rstData.uiCount, uiActualChannelMax, i, rstData.astConn[i].ucSource, rstData.astConn[i].ucDestination);
            return false;
        }

        uiSourceSameCnt = 0;
        uiDestSameCnt = 0;
        for (j = 0; j < rstData.uiCount; j++) {
            if (rstData.astConn[i].ucSource == rstData.astConn[j].ucSource) {
                uiSourceSameCnt++;
            }

            if ((rstData.astConn[i].ucDestination == rstData.astConn[j].ucDestination) && (DEF_OCS_SWITCH_OUTPUT_CHANNEL_VALUE != rstData.astConn[i].ucDestination)) {
                uiDestSameCnt++;
            }

            if (uiSourceSameCnt > 1 || uiDestSameCnt > 1) {
                LOG_WRITE(EN_LOG_ERROR, "Invalid switch channels! (%d, %d, %d) and (%d, %d, %d) are conflicting.",
                          i, rstData.astConn[i].ucSource, rstData.astConn[i].ucDestination,
                          j, rstData.astConn[j].ucSource, rstData.astConn[j].ucDestination);
                return false;
            }
        }
    }

    /* Currently only 16X16 and 32X32 are supported. TODO for others (e.g. 64X64) */
    for (i = 0; i < uiActualChannelMax; i++) {
        stConns.astConn[i].ucId = i + 1;
        stConns.astConn[i].ucSource = i + 1;
        stConns.astConn[i].ucDestination = DEF_OCS_SWITCH_OUTPUT_CHANNEL_BLOCK;                 /* block channel */
        for (j = 0; j < rstData.uiCount; j++) {
            if (stConns.astConn[i].ucSource == rstData.astConn[j].ucSource) {
                stConns.astConn[i].ucDestination = rstData.astConn[j].ucDestination;
                break;
            }
        }
    }

    /* Follow the protocol requirements of OCS module Spec */
    for (i = uiActualChannelMax; i < OCS_CONNECTION_MAX; i++) {
        stConns.astConn[i].ucId = i + 1;
        stConns.astConn[i].ucSource = i + 1;
        stConns.astConn[i].ucDestination = DEF_OCS_SWITCH_OUTPUT_CHANNEL_VALUE;         /*channel keep previous state */
    }

    stConns.uiCount = OCS_CONNECTION_MAX;

    /* Print setting values */
    memset(&acConnsStr, 0, sizeof(acConnsStr));
    for (i = 0, j = 0; i < stConns.uiCount; i++) {
        if (DEF_OCS_SWITCH_OUTPUT_CHANNEL_VALUE == stConns.astConn[i].ucDestination) {
            continue;
        }

        snprintf(acConnsStr + strlen(acConnsStr), sizeof(acConnsStr) - strlen(acConnsStr), "(%d,%d)", stConns.astConn[i].ucSource, stConns.astConn[i].ucDestination);

        if ((++j) % DEF_OCS_16X16_SWITCH_CHANNES_MAX == 0) {
            LOG_WRITE(EN_LOG_NOTICE, "SetSwitchChannels: %s", acConnsStr);
            memset(acConnsStr, 0, sizeof(acConnsStr));
        }
    }

    if (strlen(acConnsStr) > 0) {
        LOG_WRITE(EN_LOG_NOTICE, "SetSwitchChannels: %s", acConnsStr);
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchChannelsCmd(staMsg, stConns);

    std::string staRsp(DEF_OCS_CMD_RESP_MAX_LEN, 0);

    if (OPLK_OK != __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), DEF_OCS_CMD_RESP_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __ReadWrite!");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "SetSwitchChannels --");

    return true;
}


bool
COcsSpiDriver::SetOneSwitchChannel(const COcsConnection &rstData)
{
    COcsConnections stConns;
    bool bIsFound = false;
    bool bResult = false;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    memset(&stConns, 0, sizeof(stConns));
    stConns.uiCount = OCS_CONNECTION_MAX;
    for (uint32_t i = 0; i < stConns.uiCount; i++) {
        stConns.astConn[i].ucId = i + 1;
        stConns.astConn[i].ucSource = i + 1;
        stConns.astConn[i].ucDestination = DEF_OCS_SWITCH_OUTPUT_CHANNEL_VALUE;         /*channel keep previous state */

        if (rstData.ucSource == stConns.astConn[i].ucSource) {
            stConns.astConn[i].ucDestination = rstData.ucDestination;
            bIsFound = true;
        }
    }
    if (false == bIsFound) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid input! ucId: %d, ucSource: %d, ucDestination: %d", rstData.ucId, rstData.ucSource, rstData.ucDestination);
        return false;
    }

    bResult = SetSwitchChannels(stConns);

    LOG_WRITE(EN_LOG_NOTICE, "SetOneSwitchChannel ucId: %d, ucSource: %d, ucDestination: %d, bResult: %d", rstData.ucId, rstData.ucSource, rstData.ucDestination, bResult);

    return bResult;
}

bool
COcsSpiDriver::GetLedState(COcsLedSta &rstData)
{
    uint32_t ulLenR = DEF_OCS_CMD_RESP_MAX_LEN;
    int rc = OPLK_ERR;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetLedStateCmd(staMsg);

    std::string staRsp(ulLenR, 0);
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), ulLenR);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get led state cmd error. m_uiHwIdx: %d", m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }

    if (false == m_stpProtoPtr->GetLedStateRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get led state rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "GetLedState --");

    return true;
}

bool
COcsSpiDriver::SetLampTestState(bool bEnable)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetLampTestStateCmd(staMsg, bEnable);

    std::string staRsp(DEF_OCS_CMD_RESP_MAX_LEN, 0);

    if (OPLK_OK != __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), DEF_OCS_CMD_RESP_MAX_LEN)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetLampTestState bEnable: %d", bEnable);

    return true;
}

bool
COcsSpiDriver::GetAlarm(COcsAlarm &rstData)
{
    uint32_t ulLenR = DEF_OCS_CMD_RESP_MAX_LEN;
    int rc = OPLK_ERR;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetAlarmCmd(staMsg);

    std::string staRsp(ulLenR, 0);
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), ulLenR);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get alarms cmd error. m_uiHwIdx: %d", m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }

    if (false == m_stpProtoPtr->GetAlarmRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get alarms rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COcsSpiDriver::GetAlarm --");

    return true;
}

bool
COcsSpiDriver::GetCaseTemperature(COcsTempSta &rstData)
{
    uint32_t ulLenR = DEF_OCS_CMD_RESP_MAX_LEN;
    int rc = OPLK_ERR;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetCaseTemperatureCmd(staMsg);

    std::string staRsp(ulLenR, 0);
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), ulLenR);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get case temperature cmd error. m_uiHwIdx: %d", m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }

    if (false == m_stpProtoPtr->GetCaseTemperatureRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get case temperature rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COcsSpiDriver::GetCaseTemperature --");

    return true;
}

bool
COcsSpiDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COcsSpiDriver::Reset ++ %d %d", uiSubIdx, ucResetType);

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        if (nullptr != m_stpResetBoardPtr) {
            m_stpResetBoardPtr->Reset(m_uiResetHwSubIdx);
        }
    } else {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(staMsg);

        std::string staRsp(DEF_OCS_CMD_RESP_MAX_LEN, 0);
        if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), DEF_OCS_CMD_RESP_MAX_LEN)) {
            LOG_WRITE(EN_LOG_ERROR, "set soft reset set cmd error. uiSubIdx = %d base addr = 0X%02x", uiSubIdx, m_uiHwIdx);
            return false;
        }
    }

    LOG_WRITE(EN_LOG_TRACE, "COcsSpiDriver::Reset --");

    return true;
}

bool
COcsSpiDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;
    uint8_t *pucDataPos = nullptr;
    uint32_t ulWriteLen = 0;
    uint32_t ulLeaveLen = 0;
    uint32_t ulCrc = 0;

    std::string staMsg;
    std::string staRsp(DEF_OCS_CMD_RESP_MAX_LEN, 0);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    /* start upg command */
    ulCrc = UTIL_Crc32Calc(pucBuffer, iFileLen);
    m_stpProtoPtr->StartUpg(staMsg, iFileLen, ulCrc);

    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade start cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }


    /* send data */
    pucDataPos = pucBuffer;
    ulLeaveLen = iFileLen;
    ulWriteLen = 0;
    while (ulLeaveLen > 0) {
        if (ulLeaveLen > DEF_OCS_UPGD_PKG_LEN_MAX) {
            ulWriteLen = DEF_OCS_UPGD_PKG_LEN_MAX;
        } else {
            ulWriteLen = ulLeaveLen;
        }

        m_stpProtoPtr->DownloadData(staMsg, pucDataPos, ulWriteLen);
        if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
            LOG_WRITE(EN_LOG_ERROR, "upgrade send data cmd error. base addr = 0X%02x", m_uiHwIdx);
            goto ERR_LABEL;
        }

        ulLeaveLen -= ulWriteLen;
        pucDataPos += ulWriteLen;
    }

    LOG_WRITE(EN_LOG_NOTICE, "end command ulLeaveLen %d ulWriteLen %d", ulLeaveLen, ulWriteLen);

    CDelay::Delay(5, MS);

    m_stpProtoPtr->Verify(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade verify cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Switch(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade switch cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Commit(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade commit cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }

    CDelay::Delay(5, S);

    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COcsSpiDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
COcsSpiDriver::Commit(uint32_t uiDevIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcsSpiDriver::Commit +++");
    __StartTime();

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->Commit(staMsg);

    std::string staRsp(DEF_OCS_CMD_RESP_MAX_LEN, 0);

    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(),
                                (uint8_t *)staRsp.data(), DEF_OCS_CMD_RESP_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "Commit error. m_uiHwIdx = %u", m_uiHwIdx);

        return false;
    }

    __StopTime();

    return true;

}

int
COcsSpiDriver::__ReadWrite(uint32_t uiChip, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    bool bTryAgain = false;
    uint16_t usLen = 0;
    uint16_t usFrameLen = 0;
    uint8_t ucCheckSum = 0;
    uint8_t ucMsgId = 0;

    if (nullptr == pucBufW || nullptr == pucBufR) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW or pucBufR is nullptr.");
        goto ERR_LABEL;
    }

    /* Write frame cmd into ocs                                                                                         */
    if (false == m_stpDrvPtr->ReadWrite(m_uiHwIdx, pucBufW, uiLenW, pucBufR, std::make_shared<CRspHandler>(uiLenR))) {
        goto ERR_LABEL;
    }

    /* Check the busy state                                                                                             */
    if (false == m_stpProtoPtr->CheckState(pucBufR, bTryAgain)) {
        if (bTryAgain) {
            CDelay::Delay(200, MS);
            LOG_WRITE(EN_LOG_NOTICE, "Ocs ReadWrite Try Again.");
            if (false == m_stpDrvPtr->ReadWrite(m_uiHwIdx, pucBufW, uiLenW, pucBufR, std::make_shared<CRspHandler>(uiLenR))) {
                goto ERR_LABEL;
            } else {
                if (false == m_stpProtoPtr->CheckState(pucBufR, bTryAgain)) {
                    if (bTryAgain) {
                        /* Next loop try again */
                        return ERR_CMD_DELAY;
                    } else {
                        goto ERR_LABEL;
                    }
                }
            }
        } else {
            goto ERR_LABEL;
        }
    }

    /***********************************Response format*******************************************************************
    **  Byte 0 |  Byte 1 | Byte 2    | Byte 3   | Byte 4 ~ n-1     |   Byte n  |
    **         |         |       Data Length    |                  |           |
    **---------|---------|----------------------|------------------|-----------|
    **  Msg ID |  Status | High Byte | Low Byte |    Data          |  Checksum |
    *********************************************************************************************************************/
    ucMsgId = pucBufW[0];
    if (ucMsgId != pucBufR[0]) {
        LOG_WRITE(EN_LOG_ERROR, "Mismatched MsgId! send: %d, recv: %d, opcode: %x", ucMsgId, pucBufR[0], pucBufW[1]);
        goto ERR_LABEL;
    }

#if 0
    if (pucBufW[1] == 0x11) {
        LOG_WRITE(EN_LOG_NOTICE, "send: %02X, %02X, %02X, %02X, %02X, %02X, %02X", ucMsgId, pucBufW[0], pucBufW[1], pucBufW[2], pucBufW[3], pucBufW[4], pucBufW[5]);
        LOG_WRITE(EN_LOG_NOTICE, "recv: %02X, %02X, %02X, %02X, %02X, %02X, %02X", ucMsgId, pucBufR[0], pucBufR[1], pucBufR[2], pucBufR[3], pucBufR[4], pucBufR[5]);
    }
#endif

    /* check status */
    if (OCS_RESP_STATUS_OK != pucBufR[1]) {
        LOG_WRITE(EN_LOG_ERROR, "Execute command failed! ucMsgId: %d, opcode: %x, reponse status: %d", ucMsgId, pucBufW[1], pucBufR[1]);
        goto ERR_LABEL;
    }

    /*usLen = ((0x00FF & pucBufR[2]) << 8) | pucBufR[3]; */
    usLen = OCS_CMD_RESP_DATA_LENGTH(pucBufR[2], pucBufR[3]);
    usFrameLen = usLen + 5;

    /* Frame length must be less then the input buffer length               */
    if ((usLen >= DEF_OCS_CMD_RESP_MAX_LEN) || (usFrameLen > DEF_OCS_CMD_RESP_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "ocs response length %d > buffer %d", usFrameLen, DEF_OCS_CMD_RESP_MAX_LEN);
        return OPLK_ERR;
    }

    /*checksum*/
    ucCheckSum = m_stpProtoPtr->CheckSum(pucBufR, usFrameLen - 1);
    if (ucCheckSum != pucBufR[usFrameLen - 1]) {
        LOG_WRITE(EN_LOG_ERROR, "ocs response check sum error! current is:0x%02X, expect is: 0x%02X",
                  ucCheckSum, pucBufR[usFrameLen - 1]);
        return OPLK_ERR;
    }

    return OPLK_OK;

ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "send command to ocs failed! m_uiHwIdx = %d", m_uiHwIdx);

    return OPLK_ERR;

}


void
COcsSpiDriver::ParseSwitchType(const char *pcModuleType)
{
    if ((NULL == pcModuleType) || (0 == strlen(pcModuleType))) {
        return;
    }

    if (0 == strcasecmp(pcModuleType, DEF_OCS_16X16_SWITCH_STR)) {
        m_ucSwitchType = OCS_TYPE_16X16_SWITCH;
    } else if (0 == strcasecmp(pcModuleType, DEF_OCS_32X32_SWITCH_STR)) {
        m_ucSwitchType = OCS_TYPE_32X32_SWITCH;
    } else if (0 == strcasecmp(pcModuleType, DEF_OCS_64X64_SWITCH_STR)) {
        m_ucSwitchType = OCS_TYPE_64X64_SWITCH;
    } else {
        /* default */
        m_ucSwitchType = OCS_TYPE_16X16_SWITCH;
    }

    LOG_WRITE(EN_LOG_NOTICE, "m_ucSwitchType: %d, pcModuleType: %s", m_ucSwitchType, pcModuleType);

    return;
}

uint32_t
COcsSpiDriver::GetSwitchChannelsCount()
{
    uint32_t uiCount = 0;

    switch (m_ucSwitchType) {
    case OCS_TYPE_16X16_SWITCH:
        uiCount = DEF_OCS_16X16_SWITCH_CHANNES_MAX;
        break;
    case OCS_TYPE_32X32_SWITCH:
        uiCount = DEF_OCS_32X32_SWITCH_CHANNES_MAX;
        break;
    case OCS_TYPE_64X64_SWITCH:
        uiCount = DEF_OCS_64X64_SWITCH_CHANNES_MAX;
        break;
    default:
        uiCount = DEF_OCS_16X16_SWITCH_CHANNES_MAX;
        break;
    }

    return uiCount;
}