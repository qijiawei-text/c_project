/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaDriver.h"
#include "Event.h"
#include "File.h"
#include "UartDriver.h"
#include "RspHandler.h"
#include "ElapseTimer.h"

/*==============================function=========================*/
bool
COaIoDriver::IsOnline(void)
{

    return true;
}

bool
COaIoDriver::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetMfg +++");

    /* CElapseTimer::Start(); */

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    memset(&rstMfg, 0, sizeof(rstMfg));

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "Get mfg rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFpgaRunVer %s", rstMfg.acFpgaRunVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

    /* printf("OA[%d] acFwVer %s\n", m_uiHwSubIdx, rstMfg.acFwVer); */

    /* CElapseTimer::Stop(); */

    return true;


}

bool
COaIoDriver::GetStatusData(std::vector<COaStatusData> &rstaData)
{
    if (m_bCommFailDebug) {
        return false;
    }

    uint32_t uiLen = 0;
    int16_t sCheckSum = 0;
    int16_t *psStatusData = NULL;
    EDFA_DATA_ST stEdfaData;
    COaStatusData stEdfaStatus;

    /* CElapseTimer::Start(); */

    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData +++ pthread = %lu", pthread_self());

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    memset(&stEdfaData, 0, sizeof(stEdfaData));

    m_stpProtoPtr->GetStatusData(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(stEdfaData) + 1);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }


    /* ("OA rsp len: %d\n", stpRspHandler->Pos()); */

    memcpy((uint8_t *)&stEdfaData, stpRspHandler->Data(), sizeof(stEdfaData));


    psStatusData = (int16_t *)&stEdfaData;
    uiLen = sizeof(stEdfaData) / sizeof(int16_t);

    sCheckSum = (int16_t)ntohs(stEdfaData.sCheckSum);
    if (!CByteOrder::CheckSumOk(psStatusData, uiLen, sCheckSum)) {
        LOG_WRITE(EN_LOG_ERROR, "checksum error. m_uiHwSubIdx = %u", m_uiHwSubIdx);
        printf("checksum error. m_uiHwSubIdx = %u\n", m_uiHwSubIdx);
        return false;
    }

    CByteOrder::ArrayNtohs(psStatusData, uiLen);

    for (int32_t i = 0; i < 2; i++) {
        uint32_t uiSubIdx = i;

        /*****************************************************/
        /***  get status ***/
        /*****************************************************/
        memset(&stEdfaStatus, 0, sizeof(stEdfaStatus));
        /**********************************
           BIT0- 1��OK	  0����״̬
           BIT1- 1: DIS  0����DIS
           BIT2- 1: LIM  0����LIM
           BIT3- 1: ES   0����ES
           BIT4- 1: EOP  0����EOP
           BIT5- 1: NOT READY   0: OK
        **********************************/
        stEdfaStatus.stOaCurData.bIsInService = (stEdfaData.stEDFA.asStatus[uiSubIdx] & 0x02) ? false : true;

        /* AGC:1, APC:2 */
        stEdfaStatus.stOaCurData.ucModeActual = stEdfaData.stEDFA.asModeSet[uiSubIdx];

        /* In PD */
        stEdfaStatus.stOaCurData.sInPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_IN];

        /* Out Sig Power */
        stEdfaStatus.stOaCurData.sOutSigCPower = stEdfaData.stEDFA.asOsp[uiSubIdx];
        stEdfaStatus.stOaCurData.sOutSigLPower = NO_POWER_6000;

        /* Out PD */
        stEdfaStatus.stOaCurData.sOutTotalPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_OUT];

        /* Port Power */
        int16_t sPower = stEdfaData.stEDFA.asOsp[uiSubIdx] - stEdfaData.stEDFA.asVOAAct[1];
        stEdfaStatus.stOaCurData.sPortOutSigCPower = ((sPower <= NO_POWER_6000) ? NO_POWER_6000 : sPower);

#if 0
        if ((stEdfaData.stEDFA.asOsp[0] - stEdfaData.stEDFA.asVOAAct[1]) <= NO_POWER_6000) {
            stEdfaStatus.stOaCurData.sPortOutSigCPower = NO_POWER_6000;
        } else {
            stEdfaStatus.stOaCurData.sPortOutSigCPower = stEdfaData.stEDFA.asOsp[0] - stEdfaData.stEDFA.asVOAAct[1];
        }
#endif

        stEdfaStatus.stOaCurData.sPortOutSigLPower = NO_POWER_6000;
        stEdfaStatus.stOaCurData.sPortOutTotalPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_VOA2_OUT];

        /* Msa Power */
        stEdfaStatus.stOaCurData.sMsaInPower = NO_POWER_6000;
        stEdfaStatus.stOaCurData.sMsaOutPower = NO_POWER_6000;

        /* Back Ref Power */
        stEdfaStatus.stOaCurData.sBackRefPower = stEdfaData.stEDFA.asPD[PD_REF];
        stEdfaStatus.stOaCurData.sBackRefRatio = stEdfaData.stEDFA.asPD[PD_RATIO];

        /* Gain & Tilt */
        stEdfaStatus.stOaCurData.sGainActual = stEdfaData.stEDFA.asGain[uiSubIdx];
        if (NO_POWER_6000 == stEdfaStatus.stOaCurData.sInPower) {
            stEdfaStatus.stOaCurData.sGainActual = NO_POWER_6000;
        }
        stEdfaStatus.stOaCurData.sTiltActual = stEdfaData.stEDFA.asTiltAct[uiSubIdx];

        /* Moduel & Heater Temperature */
        stEdfaStatus.stOaCurData.asCaseTemp[0] = stEdfaData.stEDFA.sCtValue;
        stEdfaStatus.stOaCurData.asHeaterTemp[0] = stEdfaData.stEDFA.sHtValue;

        /* Pump Temperature */
        stEdfaStatus.stOaCurData.asPumpTemp[0] = stEdfaData.stEDFA.asPtValue[uiSubIdx];
        /* stEdfaStatus.stOaCurData.asPumpTemp[1] = stEdfaData.stEDFA.asPtValue[1]; */

        /* Pump Current */
        stEdfaStatus.stOaCurData.asPumpCurrent[0] = stEdfaData.stEDFA.asPcValue[uiSubIdx];
        /* stEdfaStatus.stOaCurData.asPumpCurrent[1] = stEdfaData.stEDFA.asPcValue[1]; */

        /* Voa Attn */
        stEdfaStatus.stOaCurData.asVoaAttnActual[0] = stEdfaData.stEDFA.asVOAAct[uiSubIdx];


        memcpy(stEdfaStatus.stOaCurData.asExtPdPower, stEdfaData.stEDFA.asPD, sizeof(stEdfaStatus.stOaCurData.asExtPdPower));

        /*****************************************************/
        /***  debug alarms ***/
        /*****************************************************/
        if (m_bModuleAlarmDebug) {
            /* OOG debug --- alarm mdoule unsupport */
            SET_BIT_2_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);

            /* EOL debug --- alarm module unsupport */
            SET_BIT_4_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

            /* Case Temp debug --- alarm module unsupport */
            SET_BIT_8_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

            /* Disable debug */
            stEdfaStatus.stOaCurData.bIsInService = false;

            /* Pump Temp debug */
            SET_BIT_0_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        }

#if 0
        /* compare alarm */

        if (m_bModuleAlarmDebug) {
            /* OOG debug */
            SET_BIT_2_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
            /* EOL debug */
            SET_BIT_4_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
            /* Limited debug */
            SET_BIT_2_VALUE(stEdfaData.stEDFA.asStatus[uiSubIdx]);
            /* Pump High debug */
            SET_BIT_0_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
            /* Pump Low debug */
            SET_BIT_1_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

            LOG_WRITE(EN_LOG_NOTICE, "alarm1 = %d", stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
            LOG_WRITE(EN_LOG_NOTICE, "alarm2 = %d", stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
            LOG_WRITE(EN_LOG_NOTICE, "status = %d", stEdfaData.stEDFA.asStatus[uiSubIdx]);
        }

        if ((0 != memcmp(m_stOaData.stEDFA.asAlarm1, stEdfaData.stEDFA.asAlarm1, sizeof(m_stOaData.stEDFA.asAlarm1))) ||
            (0 != memcmp(m_stOaData.stEDFA.asAlarm2, stEdfaData.stEDFA.asAlarm2, sizeof(m_stOaData.stEDFA.asAlarm2))) ||
            (0 != memcmp(m_stOaData.stEDFA.asEXTAlarm, stEdfaData.stEDFA.asEXTAlarm, sizeof(m_stOaData.stEDFA.asEXTAlarm))) ||
            (0 != memcmp(m_stOaData.stEDFA.asStatus, stEdfaData.stEDFA.asStatus, sizeof(m_stOaData.stEDFA.asStatus)))) {
            CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PA, 0);
            LOG_WRITE(EN_LOG_WARNING, "OA alarm hal SendEvent");
        }
#endif

        stEdfaStatus.stOaAlm.bFailure = false;
        stEdfaStatus.stOaAlm.bDisable = !stEdfaStatus.stOaCurData.bIsInService;
        stEdfaStatus.stOaAlm.bInnerErr = false;

        stEdfaStatus.stOaAlm.bInLos = BIT_0_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bOutLos = false;
        stEdfaStatus.stOaAlm.bMsaInLos = BIT_6_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bMsaOutLos = BIT_5_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);

        stEdfaStatus.stOaAlm.bRFL = BIT_4_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bOOG = BIT_2_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);


        stEdfaStatus.stOaAlm.bOOP = false;

        stEdfaStatus.stOaAlm.abPumpEOL[0] = BIT_4_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        stEdfaStatus.stOaAlm.bPumpTempHigh = BIT_0_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        stEdfaStatus.stOaAlm.bPumpTempLow = BIT_1_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        stEdfaStatus.stOaAlm.bCaseTemp = BIT_8_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]) | BIT_9_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

        stEdfaStatus.stOaAlm.bLimited = BIT_2_VALUE(stEdfaData.stEDFA.asStatus[uiSubIdx]);

        memcpy((uint8_t *)&m_stOaData, (uint8_t *)&(stEdfaData), sizeof(m_stOaData));

        rstaData.push_back(stEdfaStatus);
    }


    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData --- pthread = %lu", pthread_self());

    /* CElapseTimer::Stop(); */

    return true;
}

bool
COaIoDriver::GetCfgData(COaCfgData &rstData, uint32_t uiSubIdx)
{


    return true;

}


typedef struct
{
    int16_t sFrameID;
    int16_t sPayLoadLen;
    int16_t sFramRsv;
    int16_t sFramChkSum;

}CDebugInfoHeader;

#define DEBUG_INFO_LEN_HEADER   sizeof(CDebugInfoHeader)
#define DEBUG_INFO_LEN_TOTAL    (1560)


typedef struct
{
    CDebugInfoHeader stHeader;
    int16_t asData[(DEBUG_INFO_LEN_TOTAL - DEBUG_INFO_LEN_HEADER - 4) / 2];
    int16_t sReserve;
    int16_t sDataCheckSum;

} CDebugInfo;

bool
COaIoDriver::GetDebugInfo(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_DEBUG, "GetDebugInfo +++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetDebugInfoCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(DEBUG_INFO_LEN_TOTAL + 1);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    CDebugInfo stDebugInfo = {0};

    memcpy((uint8_t *)&stDebugInfo, stpRspHandler->Data(), sizeof(stDebugInfo));

    /* int16_t sCheckSum = *(int16_t *)(stpRspHandler->Data() + 6); */
    int16_t sCheckSum = stDebugInfo.stHeader.sFramChkSum;

    if (!CByteOrder::CheckSumOkN((int16_t *)&stDebugInfo, DEBUG_INFO_LEN_HEADER / 2, sCheckSum)) {
        LOG_WRITE(EN_LOG_ERROR, "FramChkSum error. ");
        return false;
    }

    sCheckSum = stDebugInfo.sDataCheckSum;

    if (!CByteOrder::CheckSumOkN((int16_t *)stDebugInfo.asData, (DEBUG_INFO_LEN_TOTAL - DEBUG_INFO_LEN_HEADER) / 2, sCheckSum)) {
        LOG_WRITE(EN_LOG_ERROR, "DataCheckSum error. ");
        return false;
    }

    if (!CFile::Write(pcFilePath, stpRspHandler->Data(), stpRspHandler->Pos())) {
        LOG_WRITE(EN_LOG_ERROR, "write file error. len %d", stpRspHandler->Pos());
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "GetDebugInfo ---");

    return true;
}

bool
COaIoDriver::GetMcuDebugInfo(const char *pcFilePath, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "GetMcuDebugInfo +++");

    return true;
}


bool
COaIoDriver::SetGainRange(uint32_t uiGainRange, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetGainRange(uiSubIdx, staMsg, uiGainRange);

    return __SendSetCmd(staMsg);

}

bool
COaIoDriver::SetGain(uint32_t uiGain, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetGain(uiSubIdx, staMsg, uiGain);

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::SetRxLosThr(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRxLosThr(uiSubIdx, staMsg, sThr);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    m_stpProtoPtr->SetRxLosHys(uiSubIdx, staMsg, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    return true;

}

bool
COaIoDriver::GetGain(int16_t &rsGain, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetGain(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetGainRsp(uiSubIdx, staRsp, rsGain);
}

bool
COaIoDriver::GetTilt(int16_t &rsTilt, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetTilt(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();
    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetTiltRsp(uiSubIdx, staRsp, rsTilt);
}

bool
COaIoDriver::GetRxLosThr(int16_t &rsThr, int16_t &rsHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetRxLosThr(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetRxLosThrRsp(uiSubIdx, staRsp, rsThr, rsHys);
}


bool
COaIoDriver::SetPumpEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetPumpEnable(uiSubIdx, staMsg, bEnable);
    LOG_WRITE(EN_LOG_NOTICE, "SetPumpEnable %s", staMsg.c_str());

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::GetPumpEnable(bool &rbEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetPumpEnable(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetPumpEnableRsp(uiSubIdx, staRsp, rbEnable);
}

bool
COaIoDriver::GetAutolosEnable(bool &rbEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetAutolosEnable(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();
    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetAutolosEnableRsp(uiSubIdx, staRsp, rbEnable);
}




bool
COaIoDriver::SetAprEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetAprEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::SetAutolosEnable(bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetAutolosEnable +++ bEnable = %d, uiSubIdx = %d", bEnable, uiSubIdx);

    std::string staMsg;
    m_stpProtoPtr->SetAutolosEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}


bool
COaIoDriver::SetTilt(int16_t sTilt, uint32_t uiSubIdx)
{
    std::string staMsg;
    m_stpProtoPtr->SetTilt(uiSubIdx, staMsg, sTilt);

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::SetMode(uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx)
{
    std::string staMsg;
    m_stpProtoPtr->SetMode(uiSubIdx, staMsg, ucMode, sVal);

    return __SendSetCmd(staMsg);
}


bool
COaIoDriver::SetAttn(uint32_t uiAttn, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "uiSubIdx = %d", uiSubIdx);
    m_stpProtoPtr->SetAttn(uiSubIdx, staMsg, uiAttn);

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::SetTarget(uint32_t uiTarget, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "uiSubIdx = %d", uiSubIdx);
    m_stpProtoPtr->SetTarget(uiSubIdx, staMsg, uiTarget);

    return __SendSetCmd(staMsg);
}

bool
COaIoDriver::GetAttn(int16_t &rsAttn, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!__SendCmd(staMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetAttnRsp(uiSubIdx, staRsp, rsAttn);
}

bool
COaIoDriver::SetShutterState(bool block, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetShutterState");
    return false;
}

bool
COaIoDriver::GetShutterState(bool &block, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: GetShutterState");
    return false;
}

bool
COaIoDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Reset ++ m_uiResetHwSubIdx = %d, ucResetType = %d", m_uiResetHwSubIdx, ucResetType);

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        LOG_WRITE(EN_LOG_NOTICE, "m_uiResetHwSubIdx = %d", m_uiResetHwSubIdx);
        m_stpResetBoardPtr->Reset(m_uiResetHwSubIdx);
    } else if (RST_MODE_WARM == ucResetType) {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(uiSubIdx, staMsg);

        return __SendSetCmd(staMsg);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "ResetType(%d) error.", ucResetType);
        return false;
    }

    return true;
}
#if 0
bool
COaIoDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;

    std::string staMsg;
    std::string staRsp(OA_READ_NUM, 0);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Upgrade(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());

    CElapseTimer::Start();

    m_stpDrvPtr->Lock();
    m_stpDrvPtr->Switch(m_uiHwSubIdx);

    /* Xmodem */
    if (!CFile::XModem(m_stpDrvPtr, staMsg, m_uiHwSubIdx, pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Xmodem %d error, len %d", m_uiHwSubIdx, iFileLen);

        m_stpDrvPtr->Unlock();
        goto ERR_LABEL;
    }

    if (!__UpgradeEnd()) {
        LOG_WRITE(EN_LOG_ERROR, "__UpgradeEnd error.");

        m_stpDrvPtr->Unlock();
        goto ERR_LABEL;

    }

    m_stpDrvPtr->Unlock();

    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COaIoDriver::Upgrade error ---");
    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
COaIoDriver::__UpgradeEnd()
{
    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};

    /* Sends EOT to the module finally                                          */
    aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!m_stpDrvPtr->ReadWriteUnlock(m_uiHwSubIdx, aucXmdPkg, XMD_EOT_IDX_MAX, nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%d) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    LOG_WRITE(EN_LOG_NOTICE, "%s", staRsp.c_str());

    std::size_t found = staRsp.find("ok");
    if (found != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

#else

bool
COaIoDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;

    std::string staMsg;
    std::string staRsp(OA_READ_NUM, 0);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Upgrade(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());

    CElapseTimer::Start();

    /* Xmodem */
    if (!CFile::XModem(m_stpDrvPtr, staMsg, m_uiHwSubIdx, pucBuffer, iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Xmodem %d error, len %d", m_uiHwSubIdx, iFileLen);
        goto ERR_LABEL;
    }

    if (!__UpgradeEnd()) {
        LOG_WRITE(EN_LOG_ERROR, "__UpgradeEnd error.");
        goto ERR_LABEL;

    }

    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COaIoDriver::Upgrade error ---");
    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
COaIoDriver::__UpgradeEnd()
{
    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};

    /* Sends EOT to the module finally                                          */
    aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
    aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler();

    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, aucXmdPkg, XMD_EOT_IDX_MAX, nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%d) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    LOG_WRITE(EN_LOG_NOTICE, "%s", staRsp.c_str());

    std::size_t found = staRsp.find("ok");
    if (found != std::string::npos) {
        return true;
    } else {
        return false;
    }
}


#endif




bool
COaIoDriver::Swap(uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "Swap +++");
    std::string staMsg;
    m_stpProtoPtr->Swap(uiSubIdx, staMsg);

    return __SendSetCmd(staMsg, 500);
}

bool
COaIoDriver::Commit(uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "Commit +++");
    std::string staMsg;
    m_stpProtoPtr->Commit(uiSubIdx, staMsg);

    return __SendSetCmd(staMsg, 500);
}

bool
COaIoDriver::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaIoDriver::DbgRead ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Read(m_uiHwSubIdx, (uint8_t *)pucBuf, std::make_shared<CRspHandler>(uiLen))) {
        LOG_WRITE(EN_LOG_ERROR, "Oa read error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaIoDriver::DbgRead -- %s", pucBuf);

    return true;
}

bool
COaIoDriver::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "COaIoDriver::DbgWrite ++ %d %d %d", uiDevIdx, m_uiHwSubIdx, uiLen);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->Write(m_uiHwSubIdx, (uint8_t *)pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Oa write error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COaIoDriver::DbgWrite --");

    return true;
}

std::shared_ptr<CRspHandler>
COaIoDriver::__RspHandler(uint32_t uiTimeOut)
{
    std::shared_ptr<CEndToken> stpEndToken = std::static_pointer_cast<CEndToken>(std::make_shared<CMolexEndToken>());
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>();

    stpRspHandler->SetEndToken(stpEndToken);
    stpRspHandler->SetTimeOut(uiTimeOut);

    return stpRspHandler;
}

bool
COaIoDriver::__SendCmd(std::string &rstaMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, (uint8_t *)rstaMsg.data(), rstaMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Uart(%u) ReadWrite error.", m_uiHwSubIdx);
        return false;
    }

    return true;
}


bool
COaIoDriver::__SendSetCmd(std::string &rstaMsg, uint32_t uiTimeOut)
{

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(uiTimeOut);

    if (!__SendCmd(rstaMsg, stpRspHandler)) {
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->ProcRsp(staRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "__SendSetCmd rsp ok.");
    return true;
}


bool
COaIoDriver::SetDfbEnable(bool bEnable, int32_t iPower, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;

}

bool
COaIoDriver::SetOscEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetOtdrSwitchPos(uint32_t ulPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetOscAddThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetOscDropThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetOscRxThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetApsdEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;
}

bool
COaIoDriver::SetUpgState(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;

}

bool
COaIoDriver::SetLedState(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;

}

bool
COaIoDriver::SetLedAlm(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    return true;

}

bool
COaIoDriver::GetCaliInfo(COaCaliData &rstCaliData, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetOtdrCali(char *c_pcFilePath, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetOcmCali(char *c_pcFilePath, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetManualVoaEnable(bool bEnable, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetPaddingVoaConfigPower(uint32_t uiPower, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetManualGainEnable(bool bEnable, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetThr(uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetPumpCurrent(uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetAlmMask(int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::GetEventLog(COaEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetReadyTrigger(uint32_t uiState, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetAutoGainMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    return true;
}

bool
COaIoDriver::SetRemoteInterlockEnable(bool bEnable, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetRemoteInterlockEnable");
    return false;
}

bool
COaIoDriver::SetDummyAdminKey(bool bEnable, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetDummyAdminKey");
    return false;
}

bool
COaIoDriver::SetPanelSafeCmpt(bool bEnable, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetRemoteInterlockEnable");
    return false;
}

bool
COaIoDriver::SetOscAmpEnable(bool bEnable, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetOscEnable");
    return false;
}

bool
COaIoDriver::SetReadyPower(uint32_t uiPower, uint32_t uiSubIdx)
{
    /* TODO */
    LOG_WRITE(EN_LOG_ERROR, "Not support the command: SetOscEnable");
    return false;
}

