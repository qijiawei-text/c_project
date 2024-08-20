/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MolexOtdrProto.cpp
   Author:        Zhu Weian
   Date:          2021-02-15
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2021-02-15] [1.0] [Creator]


*****************************************************************************************************/
#include "MolexOtdrProto.h"
#include "string.h"
#define DEF_CMD_GET_VOL         (0x11)
#define DEF_CMD_GET_ALARM               (0x13)

#define DEF_CMD_SCAN_STATUS     (0x21)
#define DEF_CMD_EVENT_INFO      (0x22)

#define DEF_CMD_GET_MFG                 (0x50)
#define DEF_CMD_SET_CFG                 (0x51)
#define DEF_CMD_GET_CFG                 (0x52)

#define DEF_CMD_START_SCAN              (0x58)
#define DEF_CMD_READ_SOR                (0x2C)

#define DEF_CMD_SET_START_OFFSET        (0x5B)
#define DEF_CMD_GET_START_OFFSET        (0x5C)

#define DEF_CMD_START_UPGRADE   (0x80)
#define DEF_CMD_LOAD_FIRMWARE   (0x81)
#define DEF_CMD_END_UPGRADE     (0x82)

#define DEF_CMD_COLD_RESET              (0x83)
#define DEF_CMD_WARM_RESET              (0x84)
#define DEF_CMD_TEMP                                    (0xAE)

/* molex otdr distance unit is cm, openconfig unit is m */
#define DEF_DISTANCE_UNIT       (100)



float
SWAPFLOAT(uint32_t uiVal)
{
    int32_t iTemp = ntohl(uiVal);

    return (float)iTemp;
}



bool
COtdrProtoMolex::ProcRsp(std::string & rstaMsg, bool &rbInProgress)
{

    bool bRet = false;
    bool bInProgress = false;

    bRet = CFrameProto::UnPack(rstaMsg, bInProgress);
    rbInProgress = bInProgress;
    /* LOG_WRITE(EN_LOG_NOTICE, "bRet = %d, bInProgress = %d", bRet, bInProgress); */

    return bRet;
}

void
COtdrProtoMolex::GetVolCmd(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_GET_VOL, staBody, rstaMsg);
}


bool
COtdrProtoMolex::GetVolRsp(const std::string &c_rstaMsg, uint16_t &rusVol)
{
    memcpy(&rusVol, c_rstaMsg.data(), sizeof(uint16_t));
    rusVol = ntohs(rusVol);

    return true;
}



void
COtdrProtoMolex::GetAlarmCmd(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_GET_ALARM, staBody, rstaMsg);
}

#if 0
typedef struct {

    union {
        uint8_t ucAlarm1;
        struct CAlarm1 {
            uint8_t : 1;
            uint8_t ucTemp : 1;
            uint8_t : 1;
            uint8_t ucMemory : 1;
            uint8_t ucCali : 1;
            uint8_t : 3;
        };
    };

    union {
        uint8_t ucAlarm2;
        struct CAlarm2 {
            uint8_t : 1;
            uint8_t ucPower : 1;
            uint8_t : 6;
        };
    };
}CAlarm;

#endif

bool
COtdrProtoMolex::GetAlarmRsp(const std::string &c_rstaMsg, uint16_t &rusAlarm)
{
    memcpy(&rusAlarm, c_rstaMsg.data(), sizeof(uint16_t));
    rusAlarm = ntohs(rusAlarm);

    return true;
}

void
COtdrProtoMolex::GetMfgCmd(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_GET_MFG, staBody, rstaMsg);

}
bool
COtdrProtoMolex::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    ST_MOLEX_OTDR_MFG_INFO *pstOpsInfo = (ST_MOLEX_OTDR_MFG_INFO *)c_rstaMsg.data();

    strncpy(rstMfg.acManufacturer, "Molex Otdr", sizeof(rstMfg.acManufacturer) - 1);
    strncpy(rstMfg.acPN, pstOpsInfo->acPN, sizeof(rstMfg.acPN) - 1);
    strncpy(rstMfg.acSN, pstOpsInfo->acSN, sizeof(rstMfg.acSN) - 1);
    strncpy(rstMfg.acBootVer, pstOpsInfo->acBootVer, sizeof(rstMfg.acBootVer) - 1);
    strncpy(rstMfg.acHwVer, pstOpsInfo->acHWVer, sizeof(rstMfg.acHwVer) - 1);
    strncpy(rstMfg.acSwVer, pstOpsInfo->acBundleVer, sizeof(rstMfg.acSwVer) - 1);
    strncpy(rstMfg.acMfgDate, pstOpsInfo->acMfgDate, sizeof(rstMfg.acMfgDate) - 1);
    strncpy(rstMfg.acCaliDate, pstOpsInfo->acCalDate, sizeof(rstMfg.acCaliDate) - 1);
    strncpy(rstMfg.acFwVer, pstOpsInfo->acBundleVer, sizeof(rstMfg.acFwVer) - 1);

    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acMfgDate = %s", rstMfg.acMfgDate);

    printf("Otdr Sw Version: %s\n", rstMfg.acSwVer);

    return true;
}

void
COtdrProtoMolex::GetCfgCmd(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_GET_CFG, staBody, rstaMsg);
}

bool
COtdrProtoMolex::GetCfgRsp(const std::string &c_rstaMsg, COtdrCfgData &rstCfg)
{
    ST_MOLEX_OTDR_CONFIG *pstCfg = (ST_MOLEX_OTDR_CONFIG *)c_rstaMsg.data();

    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetCfgRsp +++");

    rstCfg.ulScanCtrl = ntohl(pstCfg->ulScanCtrl);
    rstCfg.ulScanMode = ntohl(pstCfg->ulScanMode);
    rstCfg.ulExpScanTime = ntohl(pstCfg->ulExpScanTime);
    rstCfg.ulExpPulseWidth = ntohl(pstCfg->ulExpPulseWidth);
    rstCfg.ulScanStartPos = ntohl(pstCfg->ulScanStartPos) / DEF_DISTANCE_UNIT;
    rstCfg.ulScanEndPos = ntohl(pstCfg->ulScanEndPos) / DEF_DISTANCE_UNIT;

    rstCfg.fReflIdx = UTIL_Ntohf(pstCfg->fReflIdx);
    rstCfg.fReflEventThr = UTIL_Ntohf(pstCfg->fReflEventThr);
    rstCfg.fNonReflEventThr = UTIL_Ntohf(pstCfg->fNonReflEventThr);
    rstCfg.fBackCoef = UTIL_Ntohf(pstCfg->fBackCoef);
    rstCfg.fOrlThr = UTIL_Ntohf(pstCfg->fOrlThr);
    rstCfg.fInputPwrThr = UTIL_Ntohf(pstCfg->fInputPwrThr);
    rstCfg.fReflEventFhThr = UTIL_Ntohf(pstCfg->fReflEventFhThr);
    rstCfg.fRamanSetpont = UTIL_Ntohf(pstCfg->fRamanSetpont);


    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanCtrl = %d", rstCfg.ulScanCtrl);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanMode = %d", rstCfg.ulScanMode);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulExpScanTime = %d", rstCfg.ulExpScanTime);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulExpPulseWidth = %d", rstCfg.ulExpPulseWidth);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanStartPos = %d", rstCfg.ulScanStartPos);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanEndPos = %d", rstCfg.ulScanEndPos);


    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflIdx = %f", rstCfg.fReflIdx);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflEventTh = %f", rstCfg.fReflEventThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fNonReflEventTh = %f", rstCfg.fNonReflEventThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fBackCoef = %f", rstCfg.fBackCoef);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fOrlTh = %f", rstCfg.fOrlThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fInputPwrTh = %f", rstCfg.fInputPwrThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflEventFhTh = %f", rstCfg.fReflEventFhThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fRamanSetpont = %f", rstCfg.fRamanSetpont);


    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetCfgRsp ---");

    return true;
}

void
COtdrProtoMolex::SetCfgCmd(std::string &rstaMsg, COtdrCfgData &rstCfg)
{
    std::string staBody;
    ST_MOLEX_OTDR_CONFIG stCfg = {0};

    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::SetCfgCmd +++");

    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanCtrl = %d", rstCfg.ulScanCtrl);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanMode = %d", rstCfg.ulScanMode);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulExpScanTime = %d", rstCfg.ulExpScanTime);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulExpPulseWidth = %d", rstCfg.ulExpPulseWidth);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanStartPos = %d", rstCfg.ulScanStartPos);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.ulScanEndPos = %d", rstCfg.ulScanEndPos);


    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflIdx = %f", rstCfg.fReflIdx);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflEventTh = %f", rstCfg.fReflEventThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fNonReflEventTh = %f", rstCfg.fNonReflEventThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fBackCoef = %f", rstCfg.fBackCoef);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fOrlTh = %f", rstCfg.fOrlThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fInputPwrTh = %f", rstCfg.fInputPwrThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fReflEventFhTh = %f", rstCfg.fReflEventFhThr);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fRamanSetpont = %f", rstCfg.fRamanSetpont);

    stCfg.ulScanCtrl = htonl(rstCfg.ulScanCtrl);
    stCfg.ulScanMode = htonl(rstCfg.ulScanMode);
    stCfg.ulExpScanTime = htonl(rstCfg.ulExpScanTime);
    stCfg.ulExpPulseWidth = htonl(rstCfg.ulExpPulseWidth);
    stCfg.ulScanStartPos = htonl(rstCfg.ulScanStartPos * DEF_DISTANCE_UNIT);
    stCfg.ulScanEndPos = htonl(rstCfg.ulScanEndPos * DEF_DISTANCE_UNIT);

    /* to do ntohf->htonf */
    stCfg.fReflIdx = UTIL_Ntohf(rstCfg.fReflIdx);
    stCfg.fReflEventThr = UTIL_Ntohf(rstCfg.fReflEventThr);
    stCfg.fNonReflEventThr = UTIL_Ntohf(rstCfg.fNonReflEventThr);
    stCfg.fBackCoef = UTIL_Ntohf(rstCfg.fBackCoef);
    stCfg.fOrlThr = UTIL_Ntohf(rstCfg.fOrlThr);
    stCfg.fInputPwrThr = UTIL_Ntohf(rstCfg.fInputPwrThr);
    stCfg.fReflEventFhThr = UTIL_Ntohf(rstCfg.fReflEventFhThr);
    stCfg.fRamanSetpont = UTIL_Ntohf(rstCfg.fRamanSetpont);

    staBody.append((char *)&stCfg, sizeof(stCfg));

    CFrameProto::Pack(DEF_CMD_SET_CFG, staBody, rstaMsg);


    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::SetCfgCmd ---");

    return;
}



void
COtdrProtoMolex::SetPara(std::string &rstaMsg, uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution)
{
    return;

}
void
COtdrProtoMolex::SetUior(std::string &rstaMsg, double dUIOR)
{
    return;
}

void
COtdrProtoMolex::SetAcqt(std::string &rstaMsg, uint32_t ulMTime)
{
    return;
}

void
COtdrProtoMolex::SetTref(std::string &rstaMsg, double dTREF)
{
    return;
}
void
COtdrProtoMolex::SetTlos(std::string &rstaMsg, double dTLOS)
{
    return;

}
void
COtdrProtoMolex::SetTeof(std::string &rstaMsg, uint32_t ulEOF)
{
    return;
}

void
COtdrProtoMolex::StartScan(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_START_SCAN, staBody, rstaMsg);

}
void
COtdrProtoMolex::GetState(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_SCAN_STATUS, staBody, rstaMsg);
}

bool
COtdrProtoMolex::GetStateRsp(const std::string &c_rstaMsg, bool &rbState)
{
    ST_ACQ_STATUS *pstStatus = (ST_ACQ_STATUS *)c_rstaMsg.data();
    rbState = ((0 == pstStatus->ucStatus) ? true : false);
    /* LOG_WRITE(EN_LOG_NOTICE, "rbState = %s", (rbState ? "true" : "false" )); */
    printf("rbState = %s\n", (rbState ? "true" : "false"));

    return true;
}

void
COtdrProtoMolex::GetSor(std::string &rstaMsg)
{
    return;
}


void
COtdrProtoMolex::GetSorTemp(std::string &rstaMsg, uint32_t uiPackIdx)
{
    std::string staBody;
    LOG_WRITE(EN_LOG_NOTICE, "uiPackIdx = %d", uiPackIdx);

    uiPackIdx = htonl(uiPackIdx);
    staBody.append((char *)&uiPackIdx, sizeof(uint32_t));
    CFrameProto::Pack(DEF_CMD_READ_SOR, staBody, rstaMsg);
}

void
COtdrProtoMolex::GetSorRsp(const std::string &c_rstaMsg, uint32_t &ruiPackId)
{
    memcpy(&ruiPackId, (uint8_t *)c_rstaMsg.data(), sizeof(uint32_t));
    ruiPackId = ntohl(ruiPackId);
    LOG_WRITE(EN_LOG_NOTICE, "ruiPackId = %d", ruiPackId);
}

bool
COtdrProtoMolex::GetSorHeaderRsp(const std::string &c_rstaMsg, uint32_t &ruiSize)
{
    return true;
}

void
COtdrProtoMolex::GetSorBodyRsp(const std::string &c_rstaMsg)
{
    return;
}


void
COtdrProtoMolex::GetWaveform(std::string &rstaMsg)
{
    return;
}

void
COtdrProtoMolex::GetEventTop(std::string &rstaMsg)
{
    return;
}

bool
COtdrProtoMolex::GetEventTopRsp(const std::string &c_rstaMsg, COtdrEventTop &rstEventTop)
{
    return true;
}

void
COtdrProtoMolex::GetEventLine(std::string &rstaMsg, uint32_t ulLine)
{
    return;
}

bool
COtdrProtoMolex::GetEventLineRsp(const std::string &c_rstaMsg, COtdrEventLine &rstEventTop)
{
    return true;
}


void
COtdrProtoMolex::GetEventInfo(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_EVENT_INFO, staBody, rstaMsg);
}

bool
COtdrProtoMolex::GetEventInfoRsp(const std::string &c_rstaMsg, COtdrStatusData &rstaData)
{

#if 0
    ST_EVENTS_INFO *pstData = (ST_EVENTS_INFO *)c_rstaMsg.data();
    ST_MOLEX_OTDR_EVENT_INFO *pstEventIn = pstData->staEventInfo;
    COtdrEventLine *pstEventOut = rstaData.astEventLine;
    COtdrEventTop *pstEventTop = &rstaData.stEventTop;

    float fUnit = 0.001;
    uint32_t uiEventNum = ntohl(pstData->uiEventNum);

    LOG_WRITE(EN_LOG_NOTICE, "uiEventNum = %d", uiEventNum);

    printf("uiEventNum = %d\n", uiEventNum);

    for (uint32_t uiIdx = 0; uiIdx < uiEventNum; uiIdx++) {
        pstEventOut->ulEventId = ntohl(pstEventIn->uiIdx);
        pstEventOut->dLocation = UTIL_Ntohf(pstEventIn->fDistance);
        pstEventOut->dEventLoss = SWAPFLOAT(pstEventIn->iLoss) * fUnit;
        pstEventOut->dTotalLoss = SWAPFLOAT(pstEventIn->iCumulatedLoss) * fUnit;
        pstEventOut->dReflectance = SWAPFLOAT(pstEventIn->iReflectance) * fUnit;
        pstEventOut->enEventType = (EN_OTDR_EVENT_TYPE)ntohl(pstEventIn->uiType);

        LOG_WRITE(EN_LOG_NOTICE, "ulEventId = %d", pstEventOut->ulEventId);
        LOG_WRITE(EN_LOG_NOTICE, "dEventLoss = %f", pstEventOut->dEventLoss);
        LOG_WRITE(EN_LOG_NOTICE, "dTotalLoss = %f", pstEventOut->dTotalLoss);
        LOG_WRITE(EN_LOG_NOTICE, "dLocation = %f", pstEventOut->dLocation);
        LOG_WRITE(EN_LOG_NOTICE, "dReflectance = %f", pstEventOut->dReflectance);
        LOG_WRITE(EN_LOG_NOTICE, "enEventType = %d", pstEventOut->enEventType);

        pstEventIn++;
        pstEventOut++;
    }

    if (uiEventNum > 0) {
        pstEventOut--;

        pstEventTop->ulCnt = uiEventNum;
        pstEventTop->dFiberLen = pstEventOut->dLocation;
        pstEventTop->dTotalLoss = pstEventOut->dTotalLoss;
        /* pstEventTop->dReturnLoss = ? */
    }

#endif

    ST_EVENTS_INFO *pstData = (ST_EVENTS_INFO *)c_rstaMsg.data();

    float fUnit = 0.001;
    uint32_t uiEventNum = ntohl(pstData->uiEventNum);

    LOG_WRITE(EN_LOG_NOTICE, "uiEventNum = %d", uiEventNum);

    printf("uiEventNum = %d\n", uiEventNum);

    for (uint32_t uiIdx = 0; uiIdx < uiEventNum; uiIdx++) {
        rstaData.astEventLine[uiIdx].ulEventId = ntohl(pstData->staEventInfo[uiIdx].uiIdx);
        rstaData.astEventLine[uiIdx].dLocation = UTIL_Ntohf(pstData->staEventInfo[uiIdx].fDistance);
        rstaData.astEventLine[uiIdx].dEventLoss = SWAPFLOAT(pstData->staEventInfo[uiIdx].iLoss) * fUnit;
        rstaData.astEventLine[uiIdx].dTotalLoss = SWAPFLOAT(pstData->staEventInfo[uiIdx].iCumulatedLoss) * fUnit;
        rstaData.astEventLine[uiIdx].dReflectance = SWAPFLOAT(pstData->staEventInfo[uiIdx].iReflectance) * fUnit;

        uint32_t uiType = ntohl(pstData->staEventInfo[uiIdx].uiType);

        if ( 1 == uiType) {
            rstaData.astEventLine[uiIdx].enEventType = OTDR_EVENT_TYPE_REF;
        } else if ( 2 == uiType ) {
            rstaData.astEventLine[uiIdx].enEventType = OTDR_EVENT_TYPE_NON_REF;
        } else if ( 3 == uiType ) {
            rstaData.astEventLine[uiIdx].enEventType = OTDR_EVENT_TYPE_FIBER_END;
        } else {
            rstaData.astEventLine[uiIdx].enEventType = OTDR_EVENT_TYPE_NON_REF;
            LOG_WRITE(EN_LOG_ERROR, "error event type. %d", uiType);
        }

        LOG_WRITE(EN_LOG_NOTICE, "ulEventId = %d", rstaData.astEventLine[uiIdx].ulEventId);
        LOG_WRITE(EN_LOG_NOTICE, "dEventLoss = %f", rstaData.astEventLine[uiIdx].dEventLoss);
        LOG_WRITE(EN_LOG_NOTICE, "dTotalLoss = %f", rstaData.astEventLine[uiIdx].dTotalLoss);
        LOG_WRITE(EN_LOG_NOTICE, "dLocation = %f", rstaData.astEventLine[uiIdx].dLocation);
        LOG_WRITE(EN_LOG_NOTICE, "dReflectance = %f", rstaData.astEventLine[uiIdx].dReflectance);
        LOG_WRITE(EN_LOG_NOTICE, "enEventType = %d", rstaData.astEventLine[uiIdx].enEventType);
    }

    if (uiEventNum > 0) {
        rstaData.stEventTop.ulCnt = uiEventNum;
        rstaData.stEventTop.dFiberLen = rstaData.astEventLine[uiEventNum - 1].dLocation;
        rstaData.stEventTop.dTotalLoss = rstaData.astEventLine[uiEventNum - 1].dTotalLoss;

        LOG_WRITE(EN_LOG_NOTICE, "dFiberLen = %f", rstaData.stEventTop.dFiberLen);
        LOG_WRITE(EN_LOG_NOTICE, "dTotalLoss = %f", rstaData.stEventTop.dTotalLoss);


        /* pstEventTop->dReturnLoss = ? */
    }


    return true;
}

void
COtdrProtoMolex::GetTemp(std::string &rstaMsg)
{
    std::string staBody;

    CFrameProto::Pack(DEF_CMD_TEMP, staBody, rstaMsg);
}

bool
COtdrProtoMolex::__TempConvert(int16_t sTemp, float &rfTemp)
{
    sTemp = ntohs(sTemp);
    rfTemp = (float)sTemp * 0.01;

    return true;
}

bool
COtdrProtoMolex::GetTempRsp(const std::string &c_rstaMsg, COtdrTemp &rstaData)
{
    ST_TEMP *pstData = (ST_TEMP *)c_rstaMsg.data();

    __TempConvert(pstData->sTecTemp, rstaData.fTecTemp);
    __TempConvert(pstData->sModuleTemp, rstaData.fModuleTemp);
    __TempConvert(pstData->sFpgaTemp, rstaData.fFpgaTemp);

    return true;
}


void
COtdrProtoMolex::SoftReset(std::string &rstaMsg)
{
    std::string staBody;

    CFrameProto::Pack(DEF_CMD_WARM_RESET, staBody, rstaMsg);

}
void
COtdrProtoMolex::UpgStart(std::string &rstaMsg)
{
    std::string staBody;

    CFrameProto::Pack(DEF_CMD_START_UPGRADE, staBody, rstaMsg);

}

void
COtdrProtoMolex::SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    std::string staBody;
    staBody.append((char *)pcBuf, ulLen);
    CFrameProto::Pack(DEF_CMD_LOAD_FIRMWARE, staBody, rstaMsg);
}



void
COtdrProtoMolex::UpgEnd(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(DEF_CMD_END_UPGRADE, staBody, rstaMsg);

}

void
COtdrProtoMolex::GetStartOffsetCmd(std::string &rstaMsg)
{
    std::string staBody;
    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetStartOffsetCmd +++");

    CFrameProto::Pack(DEF_CMD_GET_START_OFFSET, staBody, rstaMsg);
    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetStartOffsetCmd ---");
}

bool
COtdrProtoMolex::GetStartOffsetRsp(const std::string &c_rstaMsg, COtdrStartOffset &rstCfg)
{
    COtdrStartOffset *pstCfg = (COtdrStartOffset *)c_rstaMsg.data();

    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetStartOffsetRsp +++");

    rstCfg.uiPositionOffset = ntohl(pstCfg->uiPositionOffset);
    rstCfg.fLossOffset = UTIL_Ntohf(pstCfg->fLossOffset);

    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.uiPositionOffset = %d", rstCfg.uiPositionOffset);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fLossOffset = %f", rstCfg.fLossOffset);


    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::GetStartOffsetRsp ---");

    return true;
}

void
COtdrProtoMolex::SetStartOffsetCmd(std::string &rstaMsg, COtdrStartOffset &rstCfg)
{
    std::string staBody;
    COtdrStartOffset stCfg = {0};

    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::SetStartOffsetCmd +++");


    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.uiPositionOffset = %d", rstCfg.uiPositionOffset);
    LOG_WRITE(EN_LOG_NOTICE, "rstCfg.fLossOffset = %f", rstCfg.fLossOffset);

    /* to do ntohf->htonf */
    stCfg.uiPositionOffset = htonl(rstCfg.uiPositionOffset);
    stCfg.fLossOffset = UTIL_Ntohf(rstCfg.fLossOffset);

    staBody.append((char *)&stCfg, sizeof(stCfg));

    CFrameProto::Pack(DEF_CMD_SET_START_OFFSET, staBody, rstaMsg);


    LOG_WRITE(EN_LOG_NOTICE, "COtdrProtoMolex::SetStartOffsetCmd ---");

    return;
}
