/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaDriver.cpp
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaMasterSpiDriver.h"
#include "Event.h"
#include "File.h"
#include "UartDriver.h"
#include "RspHandler.h"
#include "ElapseTimer.h"
#include "OaComm.h"

/*#include <iostream> */
/*#include <vector> */
using namespace std;


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

typedef struct
{
    uint32_t ulDbgIdx;
    uint8_t aucData[4608];

} COaMcuDebugInfo;

/*==============================function=========================*/
bool
COaMasterSpiDriver::IsOnline(void)
{

    return true;
}

bool
COaMasterSpiDriver::GetMfg(CMfg &rstMfg)
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

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + 512 + OA_SPI_CRC_LEN);   /*qingy 2022 add len parameter */

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
    LOG_WRITE(EN_LOG_NOTICE, "acCpldVer %s", rstMfg.acCpld1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acSwVer %s", rstMfg.acSwVer);

    /* EQA-59 the communication is normal, but the payload is null string. */
    if (strlen(rstMfg.acSwVer) <= 4) {
        LOG_WRITE(EN_LOG_WARNING, "OA swver is null, %d", strlen(rstMfg.acSwVer));
        return false;
    }

#ifdef __NR1004__
    LOG_WRITE(EN_LOG_NOTICE, "acUnitName %s", rstMfg.acUnitName);
    LOG_WRITE(EN_LOG_NOTICE, "acFCPN %s", rstMfg.acFCPN);
    LOG_WRITE(EN_LOG_NOTICE, "acFCIss %s", rstMfg.acFCIss);
    LOG_WRITE(EN_LOG_NOTICE, "acFujitsuPN %s", rstMfg.acFujitsuPN);
    LOG_WRITE(EN_LOG_NOTICE, "acCleiCode %s", rstMfg.acCleiCode);
    LOG_WRITE(EN_LOG_NOTICE, "acMac %s", rstMfg.acEth0Mac);
#endif

    printf("OA[%d] acFwVer %s, acSwVer %s\n", m_uiHwSubIdx, rstMfg.acFwVer, rstMfg.acSwVer);

    /* CElapseTimer::Stop(); */

    return true;


}

bool
COaMasterSpiDriver::GetStatusData(std::vector<COaStatusData> &rstaData)
{
    EDFA_DATA_ST stEdfaData;

    /* return true; */

    if (m_bCommFailDebug) {
        return false;
    }

    /* CElapseTimer::Start(); */

    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData +++ pthread = %lu", pthread_self());

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    memset(&stEdfaData, 0, sizeof(stEdfaData));

    m_stpProtoPtr->GetStatusData(staMsg);

    /* std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(ST_EDFA_OPER) + OA_SPI_CRC_LEN); */
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(ST_EDFA_OPER) + OA_SPI_CRC_LEN, 5);   /*qingy 2022 add len parameter */

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetStatusDataRsp(staRsp, rstaData)) {
        LOG_WRITE(EN_LOG_ERROR, "OA GetStatusDataRsp error.");
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "sInPower %d", rstaData[0].stOaCurData.sInPower); */
    /* LOG_WRITE(EN_LOG_NOTICE, "sGainActual %d", rstaData[0].stOaCurData.sGainActual); */
    GetAlarmData(rstaData);

    vector<COaStatusData>::iterator iter;
    for (iter = rstaData.begin(); iter != rstaData.end(); iter++) {
        /* cout << iter->stOaCurData.asCaseTemp[0] << endl; */
    }

    /*
        CMfg stMfg = {0};
        GetMfg(stMfg);
        for(int i=0;i<5;i++){
                SetGain(i,0);
                usleep(3000*1000);
        }*/


    LOG_WRITE(EN_LOG_DEBUG, "GetStatusData --- pthread = %lu", pthread_self());

    /* CElapseTimer::Stop(); */
#if 1
    uint8_t *pucMsg = (uint8_t *)staRsp.data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy((uint8_t *)rstaData[0].aucDbgBuf, pucMsg, MIN(sizeof(ST_EDFA_OPER), sizeof(rstaData[0].aucDbgBuf)));
#endif

    return true;
}

bool
COaMasterSpiDriver::GetCfgData(COaCfgData &rstData, uint32_t uiSubIdx)
{


    return true;

}

bool
COaMasterSpiDriver::GetDebugInfo(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_DEBUG, "GetDebugInfo +++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetDebugInfoCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_OA_SPI_MSG_HEADER) + DEBUG_INFO_LEN_TOTAL + OA_SPI_CRC_LEN);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    CDebugInfo stDebugInfo = {0};

    memcpy((uint8_t *)&stDebugInfo, stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY), sizeof(stDebugInfo));

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
COaMasterSpiDriver::GetMcuDebugInfo(const char *pcFilePath, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "GetMcuDebugInfo +++");

    COaMcuDebugInfo stMcuDbgData;
    int32_t ulDbgIdx = 0;
    int iFd = -1;
    int32_t iLen = 0;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    iFd = open(pcFilePath, O_RDWR | O_CREAT | O_TRUNC | O_SYNC);
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Open file '%s' error: %s", pcFilePath, strerror(errno));
        goto ERR_LABEL;
    }

    if ( 0 == uiSubIdx || 2 == uiSubIdx ) {
        for (ulDbgIdx = 0; ulDbgIdx < 1; ulDbgIdx++) {
            std::string staMsg;
            m_stpProtoPtr->GetMcuDebugInfoCmd(staMsg, ulDbgIdx);

            std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(COaMcuDebugInfo) + OA_SPI_CRC_LEN);

            if (!__SendCmd(staMsg, stpRspHandler)) {
                LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
                printf("__SendCmd error.\n");
                goto ERR_LABEL;
            }

            memset((uint8_t *)&stMcuDbgData, 0x00, sizeof(stMcuDbgData));
            memcpy((uint8_t *)&stMcuDbgData, stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY), sizeof(stMcuDbgData));
            iLen = write(iFd, (void *)&stMcuDbgData.aucData, sizeof(stMcuDbgData.aucData));
            if (iLen != (int32_t)sizeof(stMcuDbgData.aucData)) {
                LOG_WRITE(EN_LOG_ERROR, "Write file '%s' error: %s, ilen: %d", pcFilePath, strerror(errno), iLen);
                goto ERR_LABEL;
            }

            write(iFd, "\n", strlen("\n"));
        }
    }

    if ( 1 == uiSubIdx || 2 == uiSubIdx) {
        write(iFd, "Fpga info: \r\n", strlen("Fpga info: \r\n"));

        for (ulDbgIdx = 0; ulDbgIdx < 8; ulDbgIdx++) {
            std::string staMsg;
            m_stpProtoPtr->GetFpgaInfoCmd(staMsg, ulDbgIdx);

            std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(COaMcuDebugInfo) + OA_SPI_CRC_LEN);

            if (!__SendCmd(staMsg, stpRspHandler)) {
                LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
                continue;
            }

            memset((uint8_t *)&stMcuDbgData, 0x00, sizeof(stMcuDbgData));
            memcpy((uint8_t *)&stMcuDbgData, stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY), sizeof(stMcuDbgData));
            iLen = write(iFd, (void *)&stMcuDbgData.aucData, sizeof(stMcuDbgData.aucData));
            if (iLen != (int32_t)sizeof(stMcuDbgData.aucData)) {
                LOG_WRITE(EN_LOG_ERROR, "Write file '%s' error: %s, ilen: %d", pcFilePath, strerror(errno), iLen);
                continue;
            }

            DEF_SLEEP_MS(50);
        }
    }
    LOG_WRITE(EN_LOG_TRACE, "GetMcuDebugInfo ---");

    DEF_SAFE_CLOSE(iFd);
    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "GetMcuDebugInfo ---");
    DEF_SAFE_CLOSE(iFd);
    return false;
}

bool
COaMasterSpiDriver::SetGainRange(uint32_t uiGainRange, uint32_t uiSubIdx)
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
COaMasterSpiDriver::SetGain(uint32_t uiGain, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }
#if 0
    std::string staMsg;
    m_stpProtoPtr->SetGain(uiSubIdx, staMsg, uiGain);

    return __SendSetCmd(staMsg);
#endif
    return SetMode(2, uiGain, uiSubIdx);

}

bool
COaMasterSpiDriver::SetRxLosThr(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }
#if 0
    std::string staMsg;
    m_stpProtoPtr->SetRxLosThr(uiSubIdx, staMsg, sThr);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    m_stpProtoPtr->SetRxLosHys(uiSubIdx, staMsg, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }
#else
    std::string staMsg;
    m_stpProtoPtr->SetRxLosThrHys(uiSubIdx, staMsg, sThr, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }
#endif
    return true;

}

bool
COaMasterSpiDriver::SetPumpEnable(bool bEnable, uint32_t uiSubIdx)
{
    bool bEnableToModule = false;
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    if (true == bEnable) {
        bEnableToModule = false;
    } else if (false == bEnable) {
        bEnableToModule = true;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "SetPumpEnable para err");
        return false;
    }
    std::string staMsg;
    m_stpProtoPtr->SetPumpEnable(uiSubIdx, staMsg, bEnableToModule);
    LOG_WRITE(EN_LOG_NOTICE, "SetPumpEnable %s", staMsg.c_str());

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetAprEnable(bool bEnable, uint32_t uiSubIdx)
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
COaMasterSpiDriver::SetAutolosEnable(bool bEnable, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "SetAutolosEnable +++ bEnable = %d, uiSubIdx = %d", bEnable, uiSubIdx);

    std::string staMsg;
    m_stpProtoPtr->SetAutolosEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}


bool
COaMasterSpiDriver::SetTilt(int16_t sTilt, uint32_t uiSubIdx)
{
    std::string staMsg;
    m_stpProtoPtr->SetTilt(uiSubIdx, staMsg, sTilt);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetMode(uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx)
{
    bool bRet = false;
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "SetMode +++ ucMode = %d, sVal = %d, uiSubIdx = %d", ucMode, sVal, uiSubIdx);
    m_stpProtoPtr->SetMode(uiSubIdx, staMsg, ucMode, sVal);

    bRet = __SendSetCmd(staMsg);
    if (false == bRet) {
        LOG_WRITE(EN_LOG_ERROR, "SetMode err");
        return false;
    }
    if ( 2 == ucMode) {
        LOG_WRITE(EN_LOG_NOTICE, "Agc mode ,set gain, sVal = %d, uiSubIdx = %d", sVal, uiSubIdx);
        m_stpProtoPtr->SetGain(uiSubIdx, staMsg, sVal);
        bRet = __SendSetCmd(staMsg);
        if (false == bRet) {
            LOG_WRITE(EN_LOG_ERROR, "SetMode succ,SetGain err");
            return false;
        }
    }
    return true;
}


bool
COaMasterSpiDriver::SetAttn(uint32_t uiAttn, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "uiSubIdx = %d", uiSubIdx);
    m_stpProtoPtr->SetAttn(uiSubIdx, staMsg, uiAttn);

    return __SendSetCmd(staMsg);
}

#ifdef __HCF__
bool
COaMasterSpiDriver::SetTarget(uint32_t uiTarget, uint32_t uiSubIdx)
{
    bool bRet = false;
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "SetMode APC uiSubIdx = %d", uiTarget, uiSubIdx);
    m_stpProtoPtr->SetMode(2, staMsg, 1, uiTarget);

    bRet = __SendSetCmd(staMsg);
    if (false == bRet) {
        LOG_WRITE(EN_LOG_ERROR, "SetMode err");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Apc mode ,set power, uiTarget = %d, uiSubIdx = %d", uiTarget, uiSubIdx);
    m_stpProtoPtr->SetApcTargetPower(2, staMsg, uiTarget);
    bRet = __SendSetCmd(staMsg);
    if (false == bRet) {
        LOG_WRITE(EN_LOG_ERROR, "SetMode succ,SetTarget err");
        return false;
    }
    return true;
}
#else
bool
COaMasterSpiDriver::SetTarget(uint32_t uiTarget, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "uiSubIdx = %d", uiSubIdx);
    LOG_WRITE(EN_LOG_DEBUG, "uiTarget = %d", uiTarget);
    m_stpProtoPtr->SetTarget(uiSubIdx, staMsg, uiTarget);

    return __SendSetCmd(staMsg);
}
#endif

bool
COaMasterSpiDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Reset ++ m_uiResetHwSubIdx = %d, ucResetType = %d", m_uiResetHwSubIdx, ucResetType);
    if ((nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "COaIoDriver param err");
        return false;
    }

    if ((nullptr == m_stpResetBoardPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "COaIoDriver nullptr == m_stpResetBoardPtr");
    }


    if (RST_MODE_COLD == ucResetType) {
        if ((nullptr == m_stpResetBoardPtr)) {
            LOG_WRITE(EN_LOG_ERROR, "COaIoDriver reset param err");
            return false;
        }
        LOG_WRITE(EN_LOG_NOTICE, "m_uiResetHwSubIdx = %d", m_uiResetHwSubIdx);
        m_stpResetBoardPtr->Reset(m_uiResetHwSubIdx);
    } else if (RST_MODE_WARM == ucResetType) {

        LOG_WRITE(EN_LOG_NOTICE, "Oa WarmReset");
        std::string staMsg;
        m_stpProtoPtr->SoftReset(uiSubIdx, staMsg);

        return __SendSetCmd(staMsg);
    } else {
        LOG_WRITE(EN_LOG_ERROR, "ResetType(%d) error.", ucResetType);
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    uint32_t iRet = true;
    uint32_t uiFragmentCnt = 0;
    uint32_t i = 0;
    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;
    uint8_t *pucBufferTemp = nullptr;

    std::string staMsg;
    std::string staRsp(OA_READ_NUM, 0);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        iRet = false;
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        iRet = false;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade start, pcFilePath:%s, pcFilePath:%d", pcFilePath, iFileLen);
    m_stpProtoPtr->Upgrade(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());
    if (false == __SendSetCmd(staMsg)) {
        iRet = false;
        goto ERR_LABEL;
    }
    CElapseTimer::Start();


    uiFragmentCnt = iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN ? (iFileLen / EDFA_UPG_FRAGMENT_MAX_LEN) + 1 : (iFileLen / EDFA_UPG_FRAGMENT_MAX_LEN);

    LOG_WRITE(EN_LOG_NOTICE, "uiFragmentCnt: %d, packetfragment:%d", uiFragmentCnt, iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN);
    pucBufferTemp = pucBuffer;
    if (0 < uiFragmentCnt) {
        for (i = 0; i < uiFragmentCnt - 1; i++) {
            /* LOG_WRITE(EN_LOG_NOTICE, "Upgrade write,  i:%d", i); */

            m_stpProtoPtr->Write(0, pucBufferTemp, EDFA_UPG_FRAGMENT_MAX_LEN, staMsg);
            pucBufferTemp += EDFA_UPG_FRAGMENT_MAX_LEN;

            /*_PrintfByte((uint8_t *)staMsg.data(), staMsg.size()); */
            if (false == __SendSetCmd(staMsg)) {
                LOG_WRITE(EN_LOG_ERROR, "__SendSetCmd failed");
                iRet = false;
                goto ERR_LABEL;
            }
            usleep(10 * 1000);
            /*if(i == 10){
                    sleep(5);
               }*/

        }
        m_stpProtoPtr->Write(0, pucBufferTemp, (iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN) ? (iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN) : EDFA_UPG_FRAGMENT_MAX_LEN, staMsg);
        LOG_WRITE(EN_LOG_DEBUG, "Upgrade write,last fragment, i:%d, len:%d, pucBuffer:%p", i, ((iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN) ? (iFileLen % EDFA_UPG_FRAGMENT_MAX_LEN) : EDFA_UPG_FRAGMENT_MAX_LEN), pucBufferTemp);
        pucBufferTemp += EDFA_UPG_FRAGMENT_MAX_LEN;
        if (false == __SendSetCmd(staMsg)) {
            iRet = false;
            goto ERR_LABEL;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "Upgrade Verify, pcFilePath:%s, pcFilePath:%d", pcFilePath, iFileLen);
    m_stpProtoPtr->Verify(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());
    if (false == __SendSetCmd(staMsg)) {
        iRet = false;
        goto ERR_LABEL;
    }
#if 0
    LOG_WRITE(EN_LOG_ERROR, "Upgrade Swap, pcFilePath:%s, pcFilePath:%d", pcFilePath, iFileLen);
    m_stpProtoPtr->Swap(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());
    if (false == __SendSetCmd(staMsg)) {
        iRet = false;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_ERROR, "Upgrade SoftReset, pcFilePath:%s, pcFilePath:%d", pcFilePath, iFileLen);
    m_stpProtoPtr->SoftReset(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());
    if (false == __SendSetCmd(staMsg)) {
        iRet = false;
        goto ERR_LABEL;
    }
    sleep(15);

    LOG_WRITE(EN_LOG_ERROR, "Upgrade Commit, pcFilePath:%s, pcFilePath:%d", pcFilePath, iFileLen);
    m_stpProtoPtr->Commit(0, staMsg);
    LOG_WRITE(EN_LOG_NOTICE, "%s", staMsg.c_str());
    if (false == __SendSetCmd(staMsg, 1000)) {
        iRet = false;
        goto ERR_LABEL;
    }
#endif

    CElapseTimer::Stop();

    LOG_WRITE(EN_LOG_NOTICE, "COaIoDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return iRet;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COaIoDriver::Upgrade error ---");
    DEF_SAFE_FREE(pucBuffer);
    return iRet;

}

bool
COaMasterSpiDriver::__UpgradeEnd()
{
    return true;
}


bool
COaMasterSpiDriver::Swap(uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "Swap +++");
    std::string staMsg;
    m_stpProtoPtr->Swap(uiSubIdx, staMsg);

    return __SendSetCmd(staMsg, 500);
}

bool
COaMasterSpiDriver::Commit(uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_NOTICE, "Commit +++");
    std::string staMsg;
    m_stpProtoPtr->Commit(uiSubIdx, staMsg);

    return __SendSetCmd(staMsg, 1000);
}

bool
COaMasterSpiDriver::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
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
COaMasterSpiDriver::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
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

int
COaMasterSpiDriver::Switch(uint32_t uiChip)
{
    uint8_t ucDataW = 0;
    uint8_t ucDataSubW = 0;
    uint32_t uiOffset = 0;

    m_stpLogicDrvPtr->Read(0x44, &ucDataW, 1);
    ucDataW &= ~(1 << uiChip);
    m_stpLogicDrvPtr->Write(0x44, &ucDataW, 1);

    usleep(10 * 1000);
#if 1
    ucDataSubW = 0;
    uiOffset = uiChip * 0x100;
    m_stpLogicDrvPtr->Read(0x3150 + uiOffset, &ucDataSubW, 1);
    ucDataSubW |= 0x30;                                           /*bit4 bit5 11  00110000 */
    m_stpLogicDrvPtr->Write(0x3150 + uiOffset, &ucDataSubW, 1);
#endif
    usleep(10 * 1000);

    ucDataW |= 1 << uiChip;
    m_stpLogicDrvPtr->Write(0x44, &ucDataW, 1);

    return OPLK_OK;
}

std::shared_ptr<CRspHandler>
COaMasterSpiDriver::__RspHandler(uint32_t uiLen, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_FRM_READ_HEAD) + uiLen + DEF_FRM_CRC_LEN);
    stpRspHandler->SetProto(m_stpProtoPtr);
    stpRspHandler->SetRetryTimes(uiTimeOut);

    return stpRspHandler;
}


bool
COaMasterSpiDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{



    /* todo  get hwidx */
    GetMutex().lock();
    /* pthread_mutex_lock(&m_spi_mutex[m_uiHwSubIdx]); */
    /* Switch(m_uiHwSubIdx); */

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");

        GetMutex().unlock();
        /* pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]); */
        return false;
    }
    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, (uint8_t *)staMsg.data(), staMsg.size(), nullptr, stpRspHandler)) {
        /*LOG_WRITE(EN_LOG_ERROR, "ReadWrite error: %u.", m_uiHwSubIdx);*/
        GetMutex().unlock();
        /* pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]); */
        return false;
    }
    GetMutex().unlock();
    /* pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]); */

    return true;
}

bool
COaMasterSpiDriver::__SendSetCmd(std::string staMsg, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN + 1 + 3, uiTimeOut);

    return __SendCmd(staMsg, stpRspHandler);
}

bool
COaMasterSpiDriver::GetGain(int16_t &rsGain, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetTilt(int16_t &rsTilt, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetRxLosThr(int16_t &rsThr, int16_t &rsHys, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetPumpEnable(bool &rbEnable, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetAutolosEnable(bool &rbEnable, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetAttn(int16_t &rsAttn, uint32_t uiSubIdx)
{
    return true;
}

bool
COaMasterSpiDriver::GetAlarmData(std::vector<COaStatusData> &rstaData)
{
    if (m_bCommFailDebug) {
        return false;
    }

    /* CElapseTimer::Start(); */

    LOG_WRITE(EN_LOG_DEBUG, "GetAlarmData +++ pthread = %lu", pthread_self());

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetAlarmData(staMsg);

    /* std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(ALM_MAX_SIZE) + OA_SPI_CRC_LEN); */
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(ST_EDFA_ALARM1) + OA_SPI_CRC_LEN, 5);   /* ALM_MAX_SIZE 496 qingy 2022 add len parameter */

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetAlarmDataRsp(staRsp, rstaData)) {
        LOG_WRITE(EN_LOG_ERROR, "OA GetAlarmData error.");
        return false;
    }

#if 1
    uint8_t *pucMsg = (uint8_t *)staRsp.data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy((uint8_t *)rstaData[0].aucDbgBuf + sizeof(ST_EDFA_OPER), pucMsg, MIN(sizeof(ST_EDFA_ALARM1), sizeof(rstaData[0].aucDbgBuf)));
#endif

    return true;
}
bool
COaMasterSpiDriver::SetDfbEnable(bool bEnable, int32_t iPower, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetDfbEnable(uiSubIdx, staMsg, bEnable, iPower);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetOscEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetOtdrSwitchPos(uint32_t ulPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOtdrSwitchPos(uiSubIdx, staMsg, ulPos);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetOscAddThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscAddThrHys(uiSubIdx, staMsg, sThr, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::SetOscDropThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscDropThrHys(uiSubIdx, staMsg, sThr, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::SetOscRxThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscRxThrHys(uiSubIdx, staMsg, sThr, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::SetApsdEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetApsdEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetUpgState(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetUpgState(uiSubIdx, staMsg, uiState);

    return __SendSetCmd(staMsg);

}

bool
COaMasterSpiDriver::SetLedState(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetLedState(uiSubIdx, staMsg, uiState);

    return __SendSetCmd(staMsg);

}

bool
COaMasterSpiDriver::SetLedAlm(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetLedAlm(uiSubIdx, staMsg, uiState);

    return __SendSetCmd(staMsg);

}


int
COaMasterSpiDriver::__GetOtdrTab(char *c_pcFilePath, COtdrStartOffsetAll *pstOtdrCaliInfo)
{
    int rc = 0;
    int iRow = 0;

    FILE *fpFile = NULL;
    char acLine[256] = {0};
    char acBuf[256] = {0};
    float fRev = 0.0;
    uint32_t ulCrc32 = 0;

    if (NULL == pstOtdrCaliInfo || c_pcFilePath == NULL) {
        return -1;
    }
    LOG_WRITE(EN_LOG_NOTICE, "__GetCali++, FilePath = %s,payloadlen:%lu", c_pcFilePath, sizeof(COtdrStartOffsetAll));
    fpFile = fopen(c_pcFilePath, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", c_pcFilePath);
        rc = -2;
        goto ERR_LABEL;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        LOG_WRITE(EN_LOG_NOTICE, "acLine[0]:0x%x", acLine[0]);
        LOG_WRITE(EN_LOG_NOTICE, "acLine:%s", acLine);

        /* skip first line
           #PortName,Rev,K3_L,K2_L,K1_L,C_L,Rev,K3_N,K2_N,K1_N,C_N,Rev,K3_H,K2_H,K1_H,C_H
           OcmToBaIn,0,0,0.00001,0.0002,10.1,0,0,0.00001,0.0002,10.1,0,0,0.00001,0.0002,10.1
           OcmToBaOut,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1
           OcmToPaIn,0,0,0.00001,0.0002,18.1,0,0,0.00001,0.0002,18.1,0,0,0.00001,0.0002,18.1
           OcmToPaOut,0,0,0.00001,0.0002,17.1,0,0,0.00001,0.0002,17.1,0,0,0.00001,0.0002,17.1
           OcmToExternal,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1,0,0,0.00001,0.0002,19.1

           #OtdrCali,PositionOffset,LossOffset,PortName
           0,0,0,LineR
           0,0,0,OTDR
           0,0,0,LineT

         */
        if ((NULL != strstr(acLine, "PositionOffset"))) {
            LOG_WRITE(EN_LOG_NOTICE, "continuePositionOffset");
            continue;
        }

        sscanf(acLine, "%f,%d,%f,%s",
               &fRev,
               &pstOtdrCaliInfo->stOtdrStartOffsetPort[iRow].uiPositionOffset,
               &pstOtdrCaliInfo->stOtdrStartOffsetPort[iRow].fLossOffset,
               acBuf
               );
        LOG_WRITE(EN_LOG_NOTICE, "Row[%d] portname:%s,uiPositionOffset:%d, fLossOffset:%f", iRow, acBuf, pstOtdrCaliInfo->stOtdrStartOffsetPort[iRow].uiPositionOffset, pstOtdrCaliInfo->stOtdrStartOffsetPort[iRow].fLossOffset);
        iRow++;
        memset(acLine, 0, sizeof(acLine));
        if (iRow >= OLSP_OTDR_PORT_NUM) {
            LOG_WRITE(EN_LOG_ERROR, "Row is overflow. Row = %d", iRow);
            break;
        }
    }
#if OTDR_OCM_USE_CRC
    ulCrc32 = UTIL_Crc32Calc((uint8_t *)pstOtdrCaliInfo->stOtdrStartOffsetPort, (sizeof(COtdrStartOffsetAll) - sizeof(ulCrc32)));
    pstOtdrCaliInfo->ulCrc32 = ulCrc32;
    LOG_WRITE(EN_LOG_NOTICE, "Ocm Otdr ulCrc32 =0x%x", ulCrc32);
#endif
ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}

int
COaMasterSpiDriver::__GetOcmTab(char *c_pcFilePath, COcmCali *pstOcmCaliInfo)
{
    int rc = 0;
    int iRow = 0;

    FILE *fpFile = NULL;
    char acLine[256] = {0};
    char acBuf[256] = {0};
    double dRev = 0.0;
    uint32_t ulCrc32 = 0;

    if (NULL == pstOcmCaliInfo || c_pcFilePath == NULL) {
        return -1;
    }
    LOG_WRITE(EN_LOG_NOTICE, "__GetCali++, FilePath = %s,payloadlen:%lu", c_pcFilePath, sizeof(COcmCali));
    fpFile = fopen(c_pcFilePath, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", c_pcFilePath);
        rc = -2;
        goto ERR_LABEL;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        LOG_WRITE(EN_LOG_NOTICE, "acLine[0]:0x%x", acLine[0]);
        LOG_WRITE(EN_LOG_NOTICE, "acLine:%s", acLine);

        /* skip first line
           #OcmCali,Rev,K3_L,K2_L,K1_L,C_L,Rev,K3_N,K2_N,K1_N,C_N,Rev,K3_H,K2_H,K1_H,C_H,PortName
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,OcmToBaIn
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,OcmToBaOut
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,OcmToPaIn
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,OcmToPaOut
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,OcmToExternal
         */
        if ((NULL != strstr(acLine, "OcmCali"))) {
            LOG_WRITE(EN_LOG_NOTICE, "continuePositionOffset");
            continue;
        }

        sscanf(acLine, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%s",
               &dRev,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dRevL,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK3L,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK2L,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK1L,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dCL,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dRevN,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK3N,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK2N,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK1N,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dCN,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dRevH,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK3H,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK2H,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dK1H,
               &pstOcmCaliInfo->stOcmPortCali[iRow].dCH,
               acBuf
               );

        LOG_WRITE(EN_LOG_NOTICE, "Row[%d] portname:%s,K3L:%f,K2L:%f,K1L:%f,CL:%f", iRow, acBuf, pstOcmCaliInfo->stOcmPortCali[iRow].dK3L, pstOcmCaliInfo->stOcmPortCali[iRow].dK2L, pstOcmCaliInfo->stOcmPortCali[iRow].dK1L, pstOcmCaliInfo->stOcmPortCali[iRow].dCL);
        LOG_WRITE(EN_LOG_NOTICE, "Row[%d] portname:%s,K3N:%f,K2N:%f,K1N:%f,CN:%f", iRow, acBuf, pstOcmCaliInfo->stOcmPortCali[iRow].dK3N, pstOcmCaliInfo->stOcmPortCali[iRow].dK2N, pstOcmCaliInfo->stOcmPortCali[iRow].dK1N, pstOcmCaliInfo->stOcmPortCali[iRow].dCN);
        LOG_WRITE(EN_LOG_NOTICE, "Row[%d] portname:%s,K3H:%f,K2H:%f,K1H:%f,CH:%f", iRow, acBuf, pstOcmCaliInfo->stOcmPortCali[iRow].dK3H, pstOcmCaliInfo->stOcmPortCali[iRow].dK2H, pstOcmCaliInfo->stOcmPortCali[iRow].dK1H, pstOcmCaliInfo->stOcmPortCali[iRow].dCH);

        iRow++;
        memset(acLine, 0, sizeof(acLine));
        if (iRow >= EN_OCM_PORT_CNT) {
            LOG_WRITE(EN_LOG_ERROR, "Row is overflow. Row = %d", iRow);
            break;
        }
    }

#if OTDR_OCM_USE_CRC
    ulCrc32 = UTIL_Crc32Calc((uint8_t *)pstOcmCaliInfo->stOcmPortCali, (sizeof(COcmCali) - sizeof(ulCrc32)));
    pstOcmCaliInfo->ulCrc32 = ulCrc32;
    LOG_WRITE(EN_LOG_NOTICE, "Ocm Cali ulCrc32 =0x%x", ulCrc32);
#endif

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}


bool
COaMasterSpiDriver::__GetOtdrCali(COtdrStartOffsetAll &rstCaliData)
{

    if (m_bCommFailDebug) {
        return false;
    }

    /* CElapseTimer::Start(); */

    LOG_WRITE(EN_LOG_DEBUG, "__GetOtdrCali++ ");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetOtdrCaliCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(COtdrStartOffsetAll) + OA_SPI_CRC_LEN);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetOtdrCaliRsp(staRsp, rstCaliData)) {
        LOG_WRITE(EN_LOG_ERROR, "OA __GetOtdrCali error.");
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::__GetOcmCali(COcmCali &rstCaliData)
{

    if (m_bCommFailDebug) {
        return false;
    }

    /* CElapseTimer::Start(); */

    LOG_WRITE(EN_LOG_DEBUG, "__GetOcmCali++ ");

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetOcmCaliCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(COcmCali) + OA_SPI_CRC_LEN);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetOcmCaliRsp(staRsp, rstCaliData)) {
        LOG_WRITE(EN_LOG_ERROR, "OA __GetOcmCali error.");
        return false;
    }

    return true;
}


bool
COaMasterSpiDriver::GetCaliInfo(COaCaliData &rstCaliData, uint32_t uiSubIdx)
{
#if 0
    uint32_t ulCrc32OtdrCalc = 0;
    uint32_t ulCrc32OtdrRcv = 0;
    uint32_t ulCrc32OcmCalc = 0;
    uint32_t ulCrc32OcmRcv = 0;
#endif
    LOG_WRITE(EN_LOG_DEBUG, "GetCaliInfo +++, uiSubIdx:%d", uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    __GetOtdrCali(rstCaliData.stOtdrCaliInfo);

    __GetOcmCali(rstCaliData.stOcmCaliInfo);

    /* checkcrc */
#if 0
    /* checkcrc ,move to csmd*/
    ulCrc32OtdrCalc = UTIL_Crc32Calc((uint8_t *)rstCaliData.stOtdrCaliInfo.stOtdrStartOffsetPort, sizeof(COtdrStartOffsetAll) - sizeof(ulCrc32OtdrCalc));
    ulCrc32OtdrRcv = rstCaliData.stOtdrCaliInfo.ulCrc32;
    if ( ulCrc32OtdrCalc != ulCrc32OtdrRcv) {
        LOG_WRITE(EN_LOG_ERROR, "GetCaliInfo otdr crc err, ulCrc32OtdrCalc=0x%0x, ulCrc32OtdrRcv=0x%0x", ulCrc32OtdrCalc, ulCrc32OtdrRcv);
        /* todo */
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "GetCaliInfo otdr crc succ, ulCrc32OtdrCalc=0x%0x, ulCrc32OtdrRcv=0x%0x", ulCrc32OtdrCalc, ulCrc32OtdrRcv);

    ulCrc32OcmCalc = UTIL_Crc32Calc((uint8_t *)rstCaliData.stOcmCaliInfo.stOcmPortCali, sizeof(COcmCali) - sizeof(ulCrc32OcmCalc));
    ulCrc32OcmRcv = rstCaliData.stOcmCaliInfo.ulCrc32;
    if ( ulCrc32OcmCalc != ulCrc32OcmRcv) {
        LOG_WRITE(EN_LOG_ERROR, "GetCaliInfo ocm crc err, ulCrc32OcmCalc=0x%0x, ulCrc32OcmRcv=0x%0x", ulCrc32OcmCalc, ulCrc32OcmRcv);
        /* todo */
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "GetCaliInfo ocm crc suc, ulCrc32OcmCalc=0x%0x, ulCrc32OcmRcv=0x%0x", ulCrc32OcmCalc, ulCrc32OcmRcv);
#endif

    return true;
}

bool
COaMasterSpiDriver::SetOtdrCali(char *c_pcFilePath, uint32_t uiSubIdx)
{
    int rc = 0;
    COtdrStartOffsetAll stOtdrCaliInfo;
    std::string staMsg;
    uint8_t *pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetOtdrCali++, FilePath = %s, uiSubIdx = %d", c_pcFilePath, uiSubIdx);

    memset(&stOtdrCaliInfo, 0, sizeof(COtdrStartOffsetAll));
    rc = __GetOtdrTab(c_pcFilePath, &stOtdrCaliInfo);
    if (0 != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to __GetCali. FilePath = %s", c_pcFilePath);
        rc = -2;
        goto ERR_LABEL;
    }

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    m_stpProtoPtr->SetOtdrCali(uiSubIdx, staMsg, stOtdrCaliInfo);
#if 1
    /* printf("staMsg.size():%lu\n",(long unsigned int)staMsg.size()); */
    printf("SetOtdrCali\n");
    pucBuf = (uint8_t *)staMsg.data();
    for (long unsigned int i = 0; i < staMsg.size(); i++) {
        if (0 == (i % 16)) {
            printf("\n");
        }
        printf("%02x ", pucBuf[i]);
    }
    printf("\n");
    printf("\n");
#endif
    return __SendSetCmd(staMsg);

ERR_LABEL:
    return false;

}

bool
COaMasterSpiDriver::SetOcmCali(char *c_pcFilePath, uint32_t uiSubIdx)
{
    int rc = 0;
    COcmCali stOcmCaliInfo;
    std::string staMsg;
    uint8_t *pucBuf;

    LOG_WRITE(EN_LOG_NOTICE, "SetOcmCali++, FilePath = %s, uiSubIdx = %d", c_pcFilePath, uiSubIdx);

    memset(&stOcmCaliInfo, 0, sizeof(stOcmCaliInfo));
    rc = __GetOcmTab(c_pcFilePath, &stOcmCaliInfo);
    if (0 != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to __GetCali. FilePath = %s", c_pcFilePath);
        rc = -2;
        goto ERR_LABEL;
    }

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    m_stpProtoPtr->SetOcmCali(uiSubIdx, staMsg, stOcmCaliInfo);
#if 1
    /* printf("staMsg.size():%lu\n",(long unsigned int)staMsg.size()); */
    printf("SetOcmCali\n");
    pucBuf = (uint8_t *)staMsg.data();
    for (long unsigned int i = 0; i < staMsg.size(); i++) {
        if (0 == (i % 16)) {
            printf("\n");
        }
        printf("%02x ", pucBuf[i]);
    }
    printf("\n");
    printf("\n");
#endif
    return __SendSetCmd(staMsg);

ERR_LABEL:
    return false;
}

bool
COaMasterSpiDriver::SetManualVoaEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetManualVoaEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);

}

bool
COaMasterSpiDriver::SetPaddingVoaConfigPower(uint32_t uiPower, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "uiSubIdx = %d", uiSubIdx);
    m_stpProtoPtr->SetPaddingVoaConfigPower(uiSubIdx, staMsg, uiPower);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetManualGainEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetManualGainEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetThr(uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) /* || uiPortIdx >= OA_THR_PORT_MAX */ || uiType >= OA_THR_TYPE_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "SetThr error, m_stpDrvPtr or m_stpProtoPtr is null or para err, uiPortIdx:%d, uiType:%d", uiPortIdx, uiType);
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetThrHys(uiSubIdx, staMsg, uiPortIdx, uiType, sThr, sHys);
    if (!__SendSetCmd(staMsg)) {
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::SetPumpCurrent(uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx)
{
    std::string staMsg;
    m_stpProtoPtr->SetPumpCurrent(uiSubIdx, staMsg, uiPumpIdx, sCurrent);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetAlmMask(int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx)
{
    bool bRet = false;
    std::string staMsg;
    LOG_WRITE(EN_LOG_NOTICE, "SetAlmMask +++ iMaskIdx = %d, iMaskVal = %d, uiSubIdx = %d", iMaskIdx, iMaskVal, uiSubIdx);
    m_stpProtoPtr->SetAlmMask(uiSubIdx, staMsg, iMaskIdx, iMaskVal);

    bRet = __SendSetCmd(staMsg);
    if (false == bRet) {
        LOG_WRITE(EN_LOG_ERROR, "SetAlmMask err");
        return false;
    }

    return true;
}

bool
COaMasterSpiDriver::GetEventLog(COaEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx)
{
    std::string staMsg;

    LOG_WRITE(EN_LOG_DEBUG, "GetEventLog +++, uiSubIdx:%d", uiSubIdx);

    if (m_bCommFailDebug) {
        return false;
    }

    /* CElapseTimer::Start(); */

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }


    m_stpProtoPtr->GetEventLogCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(COaEventLogAllData) + OA_SPI_CRC_LEN);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetEventLogRsp(staRsp, rstEventLogAllData)) {
        LOG_WRITE(EN_LOG_ERROR, "OA GetEventLog error.");
        return false;
    }
#if 0
    for (int i = 0; i < 4; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostTimeCount     = 0x%0x", i, rstEventLogAllData.stOaEventLogData[i].uiLostTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostPD            = 0x%0x", i, rstEventLogAllData.stOaEventLogData[i].uiLostPD);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLinkDownTimeCount = 0x%0x", i, rstEventLogAllData.stOaEventLogData[i].uiLinkDownTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiApsdTimeCount     = 0x%0x", i, rstEventLogAllData.stOaEventLogData[i].uiApsdTimeCount);
    }
#endif

    return true;
}

bool
COaMasterSpiDriver::SetShutterState(bool block, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "%s, uiSubIdx = %u", __func__, uiSubIdx);
    m_stpProtoPtr->SetShutterState(uiSubIdx, staMsg, block);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::GetShutterState(bool &block, uint32_t uiSubIdx)
{
    if (m_bCommFailDebug) {
        return false;
    }

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "%s, uiSubIdx = %u", __func__, uiSubIdx);

    std::string staMsg;
    m_stpProtoPtr->GetShutterState(uiSubIdx, staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_OA_SPI_MSG_HEADER) + sizeof(uint32_t) + OA_SPI_CRC_LEN);

    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "__SendCmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetShutterStateRsp(uiSubIdx, staRsp, block)) {
        LOG_WRITE(EN_LOG_ERROR, "OA GetShutterStateRsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s, uiSubIdx = %u, state = %d", __func__, uiSubIdx, block);

    return true;
}

bool
COaMasterSpiDriver::SetReadyTrigger(uint32_t uiState, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetReadyTrigger(uiSubIdx, staMsg, uiState);

    return __SendSetCmd(staMsg);

}

bool
COaMasterSpiDriver::SetAutoGainMode(uint32_t uiMode, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetAutoGainMode(uiSubIdx, staMsg, uiMode);

    return __SendSetCmd(staMsg);

}

bool
COaMasterSpiDriver::SetRemoteInterlockEnable(bool bEnable, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "%s, uiSubIdx = %u, bEnable = %d", __func__, uiSubIdx, bEnable);
    m_stpProtoPtr->SetRemoteInterlockEnable(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetDummyAdminKey(bool bEnable, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "%s, uiSubIdx = %u, bEnable = %d", __func__, uiSubIdx, bEnable);
    m_stpProtoPtr->SetDummyAdminKey(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetPanelSafeCmpt(bool bEnable, uint32_t uiSubIdx)
{
    std::string staMsg;
    LOG_WRITE(EN_LOG_DEBUG, "%s, uiSubIdx = %u, bEnable = %d", __func__, uiSubIdx, bEnable);
    m_stpProtoPtr->SetPanelSafeCmpt(uiSubIdx, staMsg, bEnable);

    return __SendSetCmd(staMsg);
}

bool
COaMasterSpiDriver::SetOscAmpEnable(bool bEnable, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "%s, uiSubIdx = 2, bEnable = %d", __func__, bEnable);
    return SetPumpEnable(bEnable, 2);

}

bool
COaMasterSpiDriver::SetReadyPower(uint32_t uiPower, uint32_t uiSubIdx)
{

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetReadyPower(uiSubIdx, staMsg, uiPower);
    LOG_WRITE(EN_LOG_NOTICE, "%s, uiSubIdx = %d, uiPower = %d", __func__, uiSubIdx, uiPower);
    return __SendSetCmd(staMsg);

}

