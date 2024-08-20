/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsProto.cpp
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OpsProto.h"
#include "FrameProto.h"
#include "StringTool.h"
/*==============================function=========================*/

uint16_t
COpsSwProto::CheckSum(uint16_t usSeed, uint8_t *pucSource, uint32_t uiLen)
{

    uint16_t usIndex;

    while (uiLen--) {
        usSeed ^= (uint16_t)(*pucSource++);

        for (usIndex = 0; usIndex < 8; usIndex++) {
            if (usSeed & 1) {
                usSeed = (usSeed >> 1) ^ 0xA001;
            } else {
                usSeed = usSeed >> 1;
            }
        }
    }

    return usSeed;

}

bool
COpsSwFrameProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    uint16_t usChkSum = 0, usRecvChkSum = 0;
    uint8_t *pucMsg = nullptr;
    uint16_t usLen = 0; /* not include TID head */
    uint32_t uiFrmLen = 0;     /* not include crc */
    uint32_t uiCrcOffset = 0;
    ST_FRM_READ_HEAD *pstHead = nullptr;
    uint8_t ucRspStatus = DEF_STATUS_OK;

    pucMsg = (uint8_t *)rstaMsg.data();
    pstHead = (ST_FRM_READ_HEAD *)pucMsg;
    usLen = ntohs(pstHead->stTidHead.usLen);

    uiFrmLen = sizeof(ST_FRM_TID_HEAD) + usLen - DEF_FRM_CRC_LEN;
    usChkSum = CheckSum(SPI_CRC16_FEED, pucMsg, uiFrmLen);

    uiCrcOffset = uiFrmLen;
    memcpy(&usRecvChkSum, pucMsg + uiCrcOffset, sizeof(usRecvChkSum));

    usRecvChkSum = ntohs(usRecvChkSum);

    if (usRecvChkSum != usChkSum) {
        LOG_WRITE(EN_LOG_ERROR, "Ops read checksum error, usChkSum = %x, usLen = %d, usRecvChkSum = %x",
                  usChkSum,
                  usLen,
                  usRecvChkSum);
        return false;
    }

    ucRspStatus = pstHead->stFrmRspHead.ucStatus;
    if (DEF_STATUS_OK != ucRspStatus) {
        if (DEF_STATUS_IN_PROGRESS == ucRspStatus) {
            rbInProgress = true;
        }
        LOG_WRITE(EN_LOG_ERROR, "status wrong %d", ucRspStatus);
        return false;
    }

    return true;
}


bool
COpsSwFrameProto::GeneralCmd(uint16_t usOid, std::string staPayload, std::string &rstaMsg, uint32_t uiSubIdx)
{
    ST_FRM_WRITE_HEAD stHead;
    uint8_t *pucMsg = nullptr;
    uint8_t *pucData = nullptr;
    uint16_t usPayloadLen = 0;

    usPayloadLen = (uint16_t)staPayload.size();

    if (usPayloadLen > 0) {
        if ( NULL == (pucData = (uint8_t *)staPayload.data())) {
            return false;
        }
    }

    rstaMsg.resize(sizeof(ST_FRM_WRITE_HEAD) + usPayloadLen + DEF_FRM_CRC_LEN);
    pucMsg = (uint8_t *)rstaMsg.data();

    /* Header   */
    stHead.stTidHead.usTid = htons(22);
    stHead.stTidHead.usLen = htons(sizeof(ST_FRM_REQ_HEAD) + usPayloadLen + DEF_FRM_CRC_LEN);

    stHead.stFrmHead.ucPort = uiSubIdx;
    stHead.stFrmHead.ucCmd = usOid;

    memcpy(pucMsg, &stHead, sizeof(ST_FRM_WRITE_HEAD));

    /* Payload  */
    if (usPayloadLen > 0) {
        memcpy(pucMsg + sizeof(ST_FRM_WRITE_HEAD), pucData, usPayloadLen);
    }

    uint16_t usCrc = CheckSum(SPI_CRC16_FEED, pucMsg, sizeof(ST_FRM_WRITE_HEAD) + usPayloadLen);
    usCrc = htons(usCrc);

    /* Tail  */
    memcpy(pucMsg + sizeof(ST_FRM_WRITE_HEAD) + usPayloadLen, &usCrc, DEF_FRM_CRC_LEN);

    return true;
}


void
COpsSwFrameProto::GetMfgCmd(std::string &rstaMsg)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_MFGINFO, staPayload, rstaMsg);
}

void
COpsSwFrameProto::GetVerCmd(std::string &rstaMsg)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_BOOT_VER, staPayload, rstaMsg);
}

bool
COpsSwFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    uint8_t *pucMsg = nullptr;
    ST_MFG_INFO *pstOpsInfo = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstOpsInfo = (ST_MFG_INFO *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    strncpy(rstMfg.acManufacturer, pstOpsInfo->acMfgName, sizeof(rstMfg.acManufacturer) - 1);
    /* strncpy(rstMfg.acManufacturer, "Molex", sizeof(rstMfg.acManufacturer) - 1); */
    strncpy(rstMfg.acPN, pstOpsInfo->acPN, sizeof(rstMfg.acPN) - 1);
    strncpy(rstMfg.acSN, pstOpsInfo->acSN, sizeof(rstMfg.acSN) - 1);
    strncpy(rstMfg.acMfgDate, pstOpsInfo->acMfgDate, sizeof(rstMfg.acMfgDate) - 1);
    strncpy(rstMfg.acCaliDate, pstOpsInfo->acCalDate, sizeof(rstMfg.acCaliDate) - 1);
    /* strncpy(rstMfg.acProductName, "OLS-P", sizeof(rstMfg.acProductName) - 1); */

#ifdef __NR1004__
    strncpy(rstMfg.acUnitName, pstOpsInfo->acUnitName, sizeof(rstMfg.acUnitName) - 1);
    strncpy(rstMfg.acFCPN, pstOpsInfo->acFCPN, sizeof(rstMfg.acFCPN) - 1);
    strncpy(rstMfg.acFCIss, pstOpsInfo->acFCIss, sizeof(rstMfg.acFCIss) - 1);
    strncpy(rstMfg.acFujitsuPN, pstOpsInfo->acFujitsuPN, sizeof(rstMfg.acFujitsuPN) - 1);
    strncpy(rstMfg.acCleiCode, pstOpsInfo->acCleiCode, sizeof(rstMfg.acCleiCode) - 1);
    strncpy(rstMfg.acProductName, pstOpsInfo->acModeNo, sizeof(rstMfg.acProductName) - 1);
    /*strncpy(rstMfg.acFjtSn, pstOpsInfo->acFjtSn, sizeof(rstMfg.acFjtSn) - 1); */
#endif

    return true;
}

void
COpsSwFrameProto::GetMfgExCmd(std::string &rstaMsg)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_MFGEX, staPayload, rstaMsg);
}
/*
   PN: xxxxx
   SN: xxxxx
   HW Ver: xxxxx
   Boot Ver: xxxxx
   FW Ver: xxxxx
   CPLD Ver: xxxxxx
   Manufacture Date: xxxxx
   Calibration Date: xxxxx
   App0 Ver: xxxxx
   HW inside Ver: xxxxx
   Bundle Ver: xxxxx
   Active Ver: xxxxx
   Stdby Ver: xxxxx
   UN:xxxxx
   FCPN:xxxxx
   FCIss:xxxxx
   FujitsuPN:xxxxx
   CLEI:xxxxx
   ModelNo:xxxxx
   FujitsuSN:xxxxx
 */
bool
COpsSwFrameProto::GetMfgExRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    /* manufacture info */
    sprintf(rstMfg.acManufacturer, "%s", "Molex");

    /* PN */
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, "PN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops PN not found.");
    }
    /* SN */
    if (!CStringTool::SubStr(rstMfg.acSN, c_rstaMsg, "SN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SN not found.");
    }

    /* Manufacture Date */
    if (!CStringTool::SubStr(rstMfg.acMfgDate, c_rstaMsg, "Manufacture Date: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Manufacture Date: not found.");
    }

    /* Calibration Date */
    if (!CStringTool::SubStr(rstMfg.acCaliDate, c_rstaMsg, "Calibration Date: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Calibration Date: not found.");
    }

    /* UN */
    if (!CStringTool::SubStr(rstMfg.acUnitName, c_rstaMsg, "UN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops UN not found.");
    }

    /* FCPN */
    if (!CStringTool::SubStr(rstMfg.acFCPN, c_rstaMsg, "FCPN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops FCPN not found.");
    }

    /* FCIss */
    if (!CStringTool::SubStr(rstMfg.acFCIss, c_rstaMsg, "FCIss: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops FCIss not found.");
    }

    /* FujitsuPN */
    if (!CStringTool::SubStr(rstMfg.acFujitsuPN, c_rstaMsg, "FujitsuPN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops FujitsuPN not found.");
    }

    /* CLEI */
    if (!CStringTool::SubStr(rstMfg.acCleiCode, c_rstaMsg, "CLEI: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops CLEI not found.");
    }

    /* ModelNo */
    if (!CStringTool::SubStr(rstMfg.acProductName, c_rstaMsg, "ModelNo: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops ModelNo not found.");
    }

    /* FujitsuSN */
    if (!CStringTool::SubStr(rstMfg.acFjtSn, c_rstaMsg, "FujitsuSN: ", "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops FujitsuSN not found.");
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
#ifdef __NR1004__
    LOG_WRITE(EN_LOG_NOTICE, "acUnitName %s", rstMfg.acUnitName);
    LOG_WRITE(EN_LOG_NOTICE, "acFCPN %s", rstMfg.acFCPN);
    LOG_WRITE(EN_LOG_NOTICE, "acFCIss %s", rstMfg.acFCIss);
    LOG_WRITE(EN_LOG_NOTICE, "acFujitsuPN %s", rstMfg.acFujitsuPN);
    LOG_WRITE(EN_LOG_NOTICE, "acCleiCode %s", rstMfg.acCleiCode);
    LOG_WRITE(EN_LOG_NOTICE, "acFjtSn %s", rstMfg.acFjtSn);
#endif

    return true;
}


bool
COpsSwFrameProto::GetVerRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    uint8_t *pucMsg = nullptr;
    ST_VER *pstOpsVer = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstOpsVer = (ST_VER *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    strncpy(rstMfg.acBootVer, pstOpsVer->acBootVer, sizeof(rstMfg.acBootVer) - 1);
    strncpy(rstMfg.acFwVer, pstOpsVer->acBundleVer, sizeof(rstMfg.acFwVer) - 1);  /* bundle version */
    strncpy(rstMfg.acHwVer, pstOpsVer->acHwVer, sizeof(rstMfg.acHwVer) - 1);
    strncpy(rstMfg.acModuleType, pstOpsVer->acModType, sizeof(rstMfg.acModuleType) - 1);
    strncpy(rstMfg.acFpgaRunVer, pstOpsVer->acFpgaVer, sizeof(rstMfg.acFpgaRunVer) - 1);
    strncpy(rstMfg.acSwVer, pstOpsVer->acMainVer, sizeof(rstMfg.acSwVer) - 1); /* mcu sw */

    return true;
}

void
COpsSwFrameProto::GetStatusDataCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    /* GeneralCmd(DEF_OPS_CMD_GET_PD_POWER, staPayload, rstaMsg); */
}

void
COpsSwFrameProto::SetWorkModeCmd(uint32_t uiMode, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));
    pucData[0] = uiMode;

    GeneralCmd(DEF_OPS_CMD_SET_WORKMODE, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetWorkModeCmd(std::string & rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_WORKMODE, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetWorkModeRsp(const std::string &c_rstaMsg, COpsStatusData &rstData)
{
    uint8_t *pucMsg = nullptr;
    ST_WORK_MODE *pstWorkMode = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstWorkMode = (ST_WORK_MODE *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    rstData.stCurData.ucWorkMode = pstWorkMode->ucWorkMode;
    rstData.stCurData.ucSwitchPos = pstWorkMode->ucSwitchPos;
    rstData.stCurData.ucFswStatus = pstWorkMode->ucFswStatus;

    LOG_WRITE(EN_LOG_DEBUG, "ucWorkMode = %d", rstData.stCurData.ucWorkMode);
    LOG_WRITE(EN_LOG_DEBUG, "ucSwitchPos = %d", rstData.stCurData.ucSwitchPos);
    LOG_WRITE(EN_LOG_DEBUG, "ucFswStatus = %d", rstData.stCurData.ucFswStatus);

    return true;
}

void
COpsSwFrameProto::GetAllCfgCmd(std::string & rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_ALL_CFG, staPayload, rstaMsg, uiSubIdx);
}


bool
COpsSwFrameProto::GetAllCfgRsp(const std::string & c_rstaMsg, COpsCfgData &rstData)
{
    uint8_t *pucMsg = nullptr;
    ST_ALL_CFG *pstCfg = nullptr;
    int16_t asPowerLosThr[OPS_PORT_CNT] = {0};
    int16_t asPowerLosHys[OPS_PORT_CNT] = {0};

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstCfg = (ST_ALL_CFG *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    rstData.usRelativeEnable = ntohs(pstCfg->usRelativeEnable);
    rstData.usHoldTime = ntohs(pstCfg->usHoldTime);
    rstData.usRelativeSwitchThr = ntohs(pstCfg->usRelativeSwitchThr);
    rstData.sRelativeOffset = ntohs(pstCfg->sRelativeOffset);
    rstData.uiWtrTime = ntohl(pstCfg->uiWtrTime);
    rstData.uiHour = ntohl(pstCfg->uiHour);
    rstData.uiMinute = ntohl(pstCfg->uiMinute);
    rstData.uiSecond = ntohl(pstCfg->uiSecond);
    memcpy(asPowerLosThr, pstCfg->sPowerLosThr, sizeof(asPowerLosThr));
    memcpy(asPowerLosHys, pstCfg->sPowerLosHys, sizeof(asPowerLosHys));
    UTIL_ntohs((uint16_t *)&rstData.stPortPowerThr, (uint16_t *)asPowerLosThr, OPS_PORT_CNT);
    UTIL_ntohs((uint16_t *)&rstData.stPortPowerHys, (uint16_t *)asPowerLosHys, OPS_PORT_CNT);

    rstData.stConnectionPdThr.asPdThr[OPS_R] = ntohs(pstCfg->sComInLosThr);
    rstData.stPortPowerThr.sDifferBetweenLine1Line2 = ntohs(pstCfg->sDifferBetweenLine1Line2);
    rstData.stConnectionPdHys.asPdHys[OPS_R] = ntohs(pstCfg->sComInLosHys);

    return true;
}

void
COpsSwFrameProto::GetAlarmCmd(std::string & rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_ALRM, staPayload, rstaMsg);
}


bool
COpsSwFrameProto::GetAlarmRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData)
{
    uint8_t *pucMsg = nullptr;
    ST_ALARM *pstAlarm = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstAlarm = (ST_ALARM *)(pucMsg + sizeof(ST_FRM_READ_HEAD));


    for (uint32_t uiPortCnt = 0; uiPortCnt < OPS_PORT_CNT; uiPortCnt++) {
        rstData.stAlarm.aaucPortAlarm[PORT_ALARM_TYPE_LOS][uiPortCnt] = pstAlarm->abLos[uiPortCnt];
    }
#if 0
    for (uint32_t uiPortCnt = 0; uiPortCnt < OPS_PORT_CNT; uiPortCnt++) {
        rstData.stAlarm.aaucPortAlarm[PORT_ALARM_TYPE_LOW][uiPortCnt] = pstAlarm->abLow[uiPortCnt];
    }

    for (uint32_t uiPortCnt = 0; uiPortCnt < OPS_PORT_CNT; uiPortCnt++) {
        rstData.stAlarm.aaucPortAlarm[PORT_ALARM_TYPE_HIGH][uiPortCnt] = pstAlarm->abHigh[uiPortCnt];
    }
#endif
    rstData.stAlarm.ucSwitchFailed = pstAlarm->bSwitchFailed;
    rstData.stAlarm.ucRelative = pstAlarm->bRelative;
#if 0
    rstData.stAlarm.abSwitchAlarm[OPS_PATH_PRIMARY] = pstAlarm->abSwitchAlarm[OPS_PATH_PRIMARY];
    rstData.stAlarm.abSwitchAlarm[OPS_PATH_SECOENDARY] = pstAlarm->abSwitchAlarm[OPS_PATH_SECOENDARY];
#endif
    rstData.stAlarm.ucMfgFailed = pstAlarm->bLut;
    rstData.stAlarm.ucComInLoss = pstAlarm->bComInLoss;
    rstData.stAlarm.ucPowerDiffer = pstAlarm->bReceivePowerDifferBetween;
    return true;
}


void
COpsSwFrameProto::SetWtrCmd(uint32_t uiWtr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uiWtr));
    uiWtr = htonl((uint32_t)uiWtr);
    memcpy(pucData, (char *)&uiWtr, sizeof(uiWtr));

    GeneralCmd(DEF_OPS_CMD_SET_WTR, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::SetHoldOffTimeCmd(uint32_t uiHoldOffTime, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    uint16_t usHoldOffTime = 0;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(usHoldOffTime));
    usHoldOffTime = htons((uint16_t)uiHoldOffTime);
    memcpy(pucData, (char *)&usHoldOffTime, sizeof(usHoldOffTime));

    GeneralCmd(DEF_OPS_CMD_SET_HOLDOFF, staPayload, rstaMsg, uiSubIdx);
}


void
COpsSwFrameProto::SetRevertiveCmd(uint32_t uiRevertive, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    uint16_t usRevertive = 0;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(usRevertive));
    usRevertive = htons((uint16_t)uiRevertive);
    memcpy(pucData, (char *)&usRevertive, sizeof(usRevertive));

    GeneralCmd(DEF_OPS_CMD_SET_REVERTIVE, staPayload, rstaMsg, uiSubIdx);
}


void
COpsSwFrameProto::SetPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_PORT_POWER_THR stPowerThr;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stPowerThr));

    stPowerThr.usPort = htons((uint16_t)uiPort);
    stPowerThr.usType = htons((uint16_t)uiType);
    stPowerThr.sThr = htons((int16_t)iThr);

    memcpy(pucData, &stPowerThr, sizeof(stPowerThr));

    GeneralCmd(DEF_OPS_CMD_SET_PORT_POWER_THR, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_PORT_POWER_THR, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetPortPowerThrRsp(const std::string & c_rstaMsg, CPortPowerThr &rstData)
{
    uint8_t *pucMsg = nullptr;
    CPortPowerThr *pstThr = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstThr = (CPortPowerThr *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstThr->asThr, (uint16_t *)(rstData.asThr), OPS_ALARM_TYPE_CNT * OPS_PORT_CNT + 1);
    return true;
}

void
COpsSwFrameProto::GetPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_PORT_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetPortPowerHysRsp(const std::string & c_rstaMsg, CPortPowerHys &rstData)
{
    uint8_t *pucMsg = nullptr;
    CPortPowerHys *pstHys = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstHys = (CPortPowerHys *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstHys->asHys, (uint16_t *)(rstData.asHys), OPS_ALARM_TYPE_CNT * OPS_PORT_CNT);
    return true;
}


void
COpsSwFrameProto::SetPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_PORT_POWER_HYS stPowerHys;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stPowerHys));

    stPowerHys.usPort = htons((uint16_t)uiPort);
    stPowerHys.usType = htons((uint16_t)uiType);
    stPowerHys.sHys = htons((int16_t)iHys);

    memcpy(pucData, &stPowerHys, sizeof(stPowerHys));

    GeneralCmd(DEF_OPS_CMD_SET_PORT_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}


void
COpsSwFrameProto::GetPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_PORT_POWER, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    ST_PORT_POWER *pstPower = (ST_PORT_POWER *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstPower->asPortPower, (uint16_t *)pstPower->asPortPower, sizeof(ST_PORT_POWER) / sizeof(int16_t));

    memcpy(rstData.stCurData.asPortPower, pstPower->asPortPower, sizeof(ST_PORT_POWER));

    return true;
}

void
COpsSwFrameProto::GetConnectionPdsPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_CONNECTION_PDS_POWER, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetConnectionPdsPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsConnectionPd *pstPower = (COpsConnectionPd *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstPower->asPd, (uint16_t *)pstPower->asPd, sizeof(COpsConnectionPd) / sizeof(int16_t));

    memcpy(rstData.stConnectionPds.asPd, pstPower->asPd, sizeof(COpsConnectionPd));

    return true;
}

void
COpsSwFrameProto::SetConnectionPdPowerThrCmd(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    ST_PORT_POWER_THR stPowerThr;
    stPowerThr.usPort = htons((uint16_t)uiPdType);
    stPowerThr.usType = htons((uint16_t)uiThrType);
    stPowerThr.sThr = htons((int16_t)iThr);
    std::string staPayload((const char *)&stPowerThr, sizeof(stPowerThr));

    GeneralCmd(DEF_OPS_CMD_SET_CONNECTION_PDS_POWER_THR, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetConnectionPdsPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_CONNECTION_PDS_POWER_THR, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetConnectionPdsPowerThrRsp(std::string &rstaMsg, COpsConnectionPdThr &rstData)
{
    uint8_t *pucMsg = (uint8_t *)(rstaMsg.data() + sizeof(ST_FRM_READ_HEAD));
    COpsConnectionPdThr *pThr = (COpsConnectionPdThr *)pucMsg;

    UTIL_ntohs((uint16_t *)pThr->asPdThr, (uint16_t *)pThr->asPdThr, sizeof(COpsConnectionPdThr) / sizeof(int16_t));

    memcpy(rstData.asPdThr, pThr->asPdThr, sizeof(COpsConnectionPdThr));

    return true;
}

void
COpsSwFrameProto::SetConnectionPdPowerHysCmd(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    ST_PORT_POWER_THR stPowerThr;
    stPowerThr.usPort = htons((uint16_t)uiPdType);
    stPowerThr.usType = htons((uint16_t)uiThrType);
    stPowerThr.sThr = htons((int16_t)iThr);
    std::string staPayload((const char *)&stPowerThr, sizeof(stPowerThr));

    GeneralCmd(DEF_OPS_CMD_SET_CONNECTION_PDS_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetConnectionPdsPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_CONNECTION_PDS_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetConnectionPdsPowerHysRsp(std::string &rstaMsg, COpsConnectionPdHys &rstData)
{
    uint8_t *pucMsg = (uint8_t *)(rstaMsg.data() + sizeof(ST_FRM_READ_HEAD));
    COpsConnectionPdHys *pHys = (COpsConnectionPdHys *)pucMsg;

    UTIL_ntohs((uint16_t *)pHys->asPdHys, (uint16_t *)pHys->asPdHys, sizeof(COpsConnectionPdHys) / sizeof(int16_t));

    memcpy(rstData.asPdHys, pHys->asPdHys, sizeof(COpsConnectionPdHys));

    return true;
}

void
COpsSwFrameProto::GetPortPowerHistoryCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_PORT_POWER_HISTORY, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetPortPowerHistoryRsp(const std::string &c_rstaMsg, COpsHistoryData &rstData)
{

    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsHistoryData *pstPower = (COpsHistoryData *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    uint32_t uiSwitchTimes = 0;
    UTIL_ntohl(&(pstPower->uiSwitchTimes), &uiSwitchTimes, 1);

    if (uiSwitchTimes > 10) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHistoryRsp uiSwitchTimes is over %x, %x", pstPower->uiSwitchTimes, uiSwitchTimes);
        return false;
    }

    for (int i = 0; i < (int)uiSwitchTimes; i++) {

        UTIL_ntohl(&(pstPower->astOpsHisUnitData[i].uiHour), &(pstPower->astOpsHisUnitData[i].uiHour), 1);
        UTIL_ntohl(&(pstPower->astOpsHisUnitData[i].uiMinute), &(pstPower->astOpsHisUnitData[i].uiMinute), 1);
        UTIL_ntohl(&(pstPower->astOpsHisUnitData[i].uiSecond), &(pstPower->astOpsHisUnitData[i].uiSecond), 1);
        UTIL_ntohl(&(pstPower->astOpsHisUnitData[i].uiMillisecond), &(pstPower->astOpsHisUnitData[i].uiMillisecond), 1);

        UTIL_ntohs((uint16_t *)(pstPower->astOpsHisUnitData[i].asPortPowerHistory),
                   (uint16_t *)(pstPower->astOpsHisUnitData[i].asPortPowerHistory),
                   PORT_POWER_HISTORY_MAX_NUM * OPS_PORT_LINE_IN_CNT);
    }

    pstPower->uiSwitchTimes = uiSwitchTimes;

    memcpy(&rstData, pstPower, sizeof(COpsHistoryData));

    return true;
}

void
COpsSwFrameProto::SetRelativeThrCmd(int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    int16_t sThr = 0;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(int16_t));
    sThr = htons((int16_t)iThr);
    memcpy(pucData, (char *)&sThr, sizeof(sThr));

    GeneralCmd(DEF_OPS_CMD_SET_RELATIVE_THR, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetRelativeThrCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_RELATIVE_THR, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetRelativeThrRsp(const std::string & c_rstaMsg, CRelativeThr &rstData)
{
    uint8_t *pucMsg = nullptr;
    CRelativeThr *pstThr = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstThr = (CRelativeThr *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)&pstThr->sThr, (uint16_t *)(&rstData.sThr), sizeof(CRelativeThr) / sizeof(int16_t));
    return true;
}

void
COpsSwFrameProto::SetRelativeOffsetCmd(int32_t iOffset, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    int16_t sOffset = (int16_t)iOffset;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(int16_t));
    sOffset = htons((int16_t)sOffset);
    memcpy(pucData, (char *)&sOffset, sizeof(sOffset));

    GeneralCmd(DEF_OPS_CMD_SET_RELATIVE_OFFSET, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetRelativeOffsetCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_RELATIVE_OFFSET, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetRelativeOffsetRsp(const std::string & c_rstaMsg, CRelativeOffset &rstData)
{
    uint8_t *pucMsg = nullptr;
    CRelativeOffset *pstOffset = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstOffset = (CRelativeOffset *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)&pstOffset->sOffset, (uint16_t *)(&rstData.sOffset), sizeof(CRelativeOffset) / sizeof(int16_t));
    return true;
}

void
COpsSwFrameProto::SetSwitchThrCmd(uint32_t uiPos, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_OPS_SWITCH_THR_SET stSwitchThrSet = {0};

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stSwitchThrSet));

    stSwitchThrSet.usPos = htons((uint16_t)uiPos);
    stSwitchThrSet.sThr = htons((int16_t)iThr);

    memcpy(pucData, (uint8_t *)&stSwitchThrSet, sizeof(stSwitchThrSet));

    GeneralCmd(DEF_OPS_CMD_SET_SWITCH_THR, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetSwitchThrCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_SWITCH_THR, staPayload, rstaMsg, uiSubIdx);
}


bool
COpsSwFrameProto::GetSwitchThrRsp(const std::string &c_rstaMsg, COpsSwitchThr &rstThr, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsSwitchThr *pstThr = (COpsSwitchThr *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstThr->asThr, (uint16_t *)pstThr->asThr, sizeof(COpsSwitchThr) / sizeof(int16_t));

    memcpy(&rstThr, pstThr, sizeof(rstThr));

    return true;
}


void
COpsSwFrameProto::SetSwitchHysCmd(uint32_t uiPos, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_OPS_SWITCH_HYS_SET stSwitchHysSet = {0};

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stSwitchHysSet));

    stSwitchHysSet.usPos = htons((uint16_t)uiPos);
    stSwitchHysSet.sHys = htons((int16_t)iHys);

    memcpy(pucData, (uint8_t *)&stSwitchHysSet, sizeof(stSwitchHysSet));

    GeneralCmd(DEF_OPS_CMD_SET_SWITCH_HYS, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetSwitchHysCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_SWITCH_HYS, staPayload, rstaMsg, uiSubIdx);
}


bool
COpsSwFrameProto::GetSwitchHysRsp(const std::string &c_rstaMsg, COpsSwitchHys &rstHys, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsSwitchHys *pstHys = (COpsSwitchHys *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstHys->asHys, (uint16_t *)pstHys->asHys, sizeof(COpsSwitchHys) / sizeof(int16_t));
    memcpy(&rstHys, pstHys, sizeof(rstHys));

    return true;
}


void
COpsSwFrameProto::SoftResetCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));
    pucData[0] = 1;

    GeneralCmd(DEF_OPS_CMD_RESET, staPayload, rstaMsg);
}


void
COpsSwFrameProto::StartUpgCmd(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint32_t) * 2);
    memcpy(pucData, &ulLen, sizeof(ulLen));
    memcpy(pucData + sizeof(ulLen), &ulCrc, sizeof(ulCrc));

    GeneralCmd(DEF_OPS_CMD_UPGD_START, staPayload, rstaMsg);
}
#if 0
void
COpsSwFrameProto::SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(ulLen);
    memcpy(pucData, pcBuf, ulLen);

    LOG_WRITE(EN_LOG_NOTICE, "ulLen = %d", ulLen);

    GeneralCmd(DEF_OPS_CMD_UPGD_DWNLD, staPayload, rstaMsg);
    LOG_WRITE(EN_LOG_NOTICE, "rstaMsg.size() = %lu", rstaMsg.size());
}
#else
void
COpsSwFrameProto::SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    uint8_t *pucMsg = nullptr;
    pucMsg = (uint8_t *)rstaMsg.data();

    LOG_WRITE(EN_LOG_DEBUG, "COpsSwFrameProto SendDataCmd ulLen = %d", ulLen);

    ST_FRM_WRITE_HEAD stHead;

    /* Header   */
    stHead.stTidHead.usTid = htons(22);
    stHead.stTidHead.usLen = htons(sizeof(ST_FRM_REQ_HEAD) + ulLen + DEF_FRM_CRC_LEN);
    stHead.stFrmHead.ucPort = 1;
    stHead.stFrmHead.ucCmd = DEF_OPS_CMD_UPGD_DWNLD;

    rstaMsg.append((char *)&stHead, sizeof(stHead));
    rstaMsg.append((char *)pcBuf, ulLen);
    uint16_t usCrc = CheckSum(SPI_CRC16_FEED, pucMsg, sizeof(ST_FRM_WRITE_HEAD) + ulLen);
    usCrc = htons(usCrc);
    rstaMsg.append((char *)&usCrc, 2);

    LOG_WRITE(EN_LOG_DEBUG, "SendDataCmd rstaMsg.size() = %lu", rstaMsg.size());
}

#endif

void
COpsSwFrameProto::EndUpgCmd(std::string &rstaMsg)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));
    pucData[0] = 0x69;

    GeneralCmd(DEF_OPS_CMD_UPGD_END, staPayload, rstaMsg);
}

void
COpsSwFrameProto::SetWssPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_PORT_POWER_THR stPowerThr;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stPowerThr));

    stPowerThr.usPort = htons((uint16_t)uiPort);
    stPowerThr.usType = htons((uint16_t)uiType);
    stPowerThr.sThr = htons((int16_t)iThr);

    memcpy(pucData, &stPowerThr, sizeof(stPowerThr));

    GeneralCmd(DEF_OPS_CMD_SET_WSS_PORT_POWER_THR, staPayload, rstaMsg, uiSubIdx);

}

void
COpsSwFrameProto::SetWssPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_PORT_POWER_HYS stPowerHys;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stPowerHys));

    stPowerHys.usPort = htons((uint16_t)uiPort);
    stPowerHys.usType = htons((uint16_t)uiType);
    stPowerHys.sHys = htons((int16_t)iHys);

    memcpy(pucData, &stPowerHys, sizeof(stPowerHys));

    GeneralCmd(DEF_OPS_CMD_SET_WSS_PORT_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetWssPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_WSS_PORT_POWER_THR, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetWssPortPowerThrRsp(const std::string & c_rstaMsg, CWssPortPowerThr &rstData)
{
    uint8_t *pucMsg = nullptr;
    CWssPortPowerThr *pstThr = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstThr = (CWssPortPowerThr *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstThr->asThr, (uint16_t *)(rstData.asThr), PORT_ALARM_TYPE_CNT * WSS_PORT_CNT);
    return true;
}

void
COpsSwFrameProto::GetWssPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_WSS_PORT_POWER_HYS, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetWssPortPowerHysRsp(const std::string & c_rstaMsg, CWssPortPowerHys &rstData)
{
    uint8_t *pucMsg = nullptr;
    CWssPortPowerHys *pstHys = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstHys = (CWssPortPowerHys *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)pstHys->asHys, (uint16_t *)(rstData.asHys), PORT_ALARM_TYPE_CNT * WSS_PORT_CNT);
    return true;
}



void
COpsSwFrameProto::GetWssPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_WSS_PORT_POWER, staPayload, rstaMsg, uiSubIdx);

}

bool
COpsSwFrameProto::GetWssPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    UTIL_ntohs((uint16_t *)pucMsg, (uint16_t *)pucMsg, sizeof(rstData.stWssStaData.stCurData.asPortPower) / sizeof(uint16_t));
    memcpy(rstData.stWssStaData.stCurData.asPortPower, pucMsg, sizeof(rstData.stWssStaData.stCurData.asPortPower));

    return true;
}

void
COpsSwFrameProto::GetWssPortPowerHistoryCmd(uint32_t uiPacketId, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint32_t));

    uiPacketId = htonl(uiPacketId);
    memcpy(pucData, &uiPacketId, sizeof(uint32_t));

    GeneralCmd(DEF_OPS_CMD_GET_WSS_PORT_POWER_HISTORY, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetWssPortPowerHistoryRsp(const std::string &c_rstaMsg, uint8_t *pucBuf, uint32_t uiLen, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD) + sizeof(uint32_t);

    UTIL_ntohs((uint16_t *)pucMsg, (uint16_t *)pucMsg, uiLen / sizeof(uint16_t));
    memcpy(pucBuf, pucMsg, uiLen);

    return true;
}

void
COpsSwFrameProto::GetWssAlarmCmd(std::string & rstaMsg, uint32_t uiSubIdx)
{
    GeneralCmd(DEF_OPS_CMD_GET_WSS_ALRM, "", rstaMsg, uiSubIdx);
}



bool
COpsSwFrameProto::GetWssAlarmRsp(const std::string &c_rstaMsg, COpsStatusData &rstData)
{

    uint8_t *pucMsg = (uint8_t *)c_rstaMsg.data();
    uint32_t *puiAlarm = (uint32_t *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohl(puiAlarm, puiAlarm, sizeof(ST_WSS_ALARM) / sizeof(uint32_t));

    for (uint32_t uiCnt = 0; uiCnt < PORT_ALARM_TYPE_CNT; uiCnt++) {
        uint32_t j = 0x10;

        for (uint32_t i = 0; i < WSS_PORT_CNT; ++i) {
            rstData.stWssStaData.stAlarm.abPortPower[uiCnt][i] = false;
            if ( *puiAlarm & j) {
                rstData.stWssStaData.stAlarm.abPortPower[uiCnt][i] = true;
            }

            j = j << 1;
            if ( 0 == j) {
                puiAlarm++;
                j = 0x01;
            }
        }

        puiAlarm++;
    }

    return true;
}

void
COpsSwFrameProto::SetWssLedModeCmd(uint8_t ucMode, std::string & rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));
    memcpy(pucData, &ucMode, sizeof(uint8_t));

    GeneralCmd(DEF_OPS_CMD_SET_WSS_LED_MODE, staPayload, rstaMsg, uiSubIdx);
}



void
COpsSwFrameProto::SetRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, std::string & rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint32_t) * 3);

    uiHour = htonl(uiHour);
    uiMinute = htonl(uiMinute);
    uiSecond = htonl(uiSecond);

    memcpy(pucData, &uiHour, sizeof(uint32_t));
    memcpy(pucData + sizeof(uint32_t), &uiMinute, sizeof(uint32_t));
    memcpy(pucData + sizeof(uint32_t) * 2, &uiSecond, sizeof(uint32_t));

    GeneralCmd(DEF_OPS_CMD_SET_REAL_TIME, staPayload, rstaMsg, uiSubIdx);
}


void
COpsSwFrameProto::GetOscPdCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_OSC_PD, staPayload, rstaMsg);
}


bool
COpsSwFrameProto::GetOscPdRsp(const std::string &c_rstaMsg, COpsStatusData &rstData)
{
    uint8_t *pucMsg = nullptr;
    COscPd *pstOscPd = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstOscPd = (COscPd *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    UTIL_ntohs((uint16_t *)&pstOscPd->asPd, (uint16_t *)&pstOscPd->asPd, sizeof(pstOscPd->asPd) / sizeof(int16_t));
    memcpy((uint8_t *)&rstData.stOscPd, (uint8_t *)pstOscPd, sizeof(COscPd));

    return true;
}

void
COpsSwFrameProto::GetOscSwitchPosCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_OSC_SWITCH_POS, staPayload, rstaMsg, uiSubIdx);

}

bool
COpsSwFrameProto::GetOscSwitchPosRsp(const std::string &c_rstaMsg, COscSwitchPos &rstData)
{
    uint8_t *pucMsg = nullptr;
    COscSwitchPos *pstOscSwitchPos = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    pstOscSwitchPos = (COscSwitchPos *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    rstData.uiIn = ntohl(pstOscSwitchPos->uiIn);
    rstData.uiOut = ntohl(pstOscSwitchPos->uiOut);

    return true;
}

void
COpsSwFrameProto::SetOscSwitchPosCmd(uint32_t uiSwitchIdx, uint32_t uiPos, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint32_t) + sizeof(uint32_t));

    uiSwitchIdx = htonl(uiSwitchIdx);
    uiPos = htonl(uiPos);

    memcpy(pucData, (char *)&uiSwitchIdx, sizeof(uint32_t));
    memcpy(pucData + sizeof(uint32_t), (char *)&uiPos, sizeof(uint32_t));


    GeneralCmd(DEF_OPS_CMD_SET_OSC_SWITCH_POS, staPayload, rstaMsg, uiSubIdx);

}

void
COpsSwFrameProto::SetLedStatusCmd(uint32_t usLedIdx, uint32_t usStatus, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(2 * sizeof(uint16_t));

    usLedIdx = htons(usLedIdx);
    usStatus = htons(usStatus);

    memcpy(pucData, (uint8_t *)&usLedIdx, sizeof(uint16_t));
    memcpy(pucData + sizeof(uint16_t), (uint8_t *)&usStatus, sizeof(uint16_t));

    GeneralCmd(DEF_OPS_CMD_SET_LED_STUTAS, staPayload, rstaMsg);
}

void
COpsSwFrameProto::SetLedModeCmd(uint8_t ucMode, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));

    memcpy(pucData, &ucMode, sizeof(uint8_t));

    GeneralCmd(DEF_OPS_CMD_SET_LED_MODE, staPayload, rstaMsg);
}

void
COpsSwFrameProto::GetLedStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_GET_LED_STATUS, staPayLoad, rstaMsg);
}

void
COpsSwFrameProto::GetLedStatusRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsLed *pstLed = (COpsLed *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    memcpy(rstData.stLed.aucStatus, pstLed, sizeof(COpsLed));
}

void
COpsSwFrameProto::GetAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_GET_ALARM_DURATION, staPayLoad, rstaMsg);
}

void
COpsSwFrameProto::GetAlarmDurationRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    UTIL_ntohl((uint32_t *)pucMsg, (uint32_t *)pucMsg, sizeof(ST_PORT_ALARM_DURATION) / sizeof(uint32_t));

    ST_PORT_ALARM_DURATION *pucDuration = (ST_PORT_ALARM_DURATION *)pucMsg;

    memcpy(rstData.stAlarm.auiDuration[PORT_ALARM_TYPE_LOS], pucDuration->auiOpsInLos, OPS_PORT_LINE_IN_CNT * 4);
#if 0
    memcpy(rstData.stAlarm.auiDuration[PORT_ALARM_TYPE_LOW], pucDuration->auiOpsInLow, OPS_PORT_LINE_IN_CNT * 4);

    memcpy(rstData.stWssStaData.stAlarm.auiDuration[PORT_ALARM_TYPE_LOS], pucDuration->auiWssLos, WSS_PANEL_PORT_CNT * 4);
    memcpy(rstData.stWssStaData.stAlarm.auiDuration[PORT_ALARM_TYPE_LOW], pucDuration->auiWssLow, WSS_PANEL_PORT_CNT * 4);
#endif
}

void
COpsSwFrameProto::ClearAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_CLEAR_ALARM_DURATION, staPayLoad, rstaMsg);
}

void
COpsSwFrameProto::GetVoltageCmd(std::string &rstaMsg, uint32_t usPdIdx, uint32_t uiSubIdx)
{
    std::string staPayload;
    uint8_t ucMode = usPdIdx;
    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint8_t));
    memcpy(pucData, &ucMode, sizeof(uint8_t));

    GeneralCmd(DEF_OPS_CMD_GET_VOLTAGE, staPayload, rstaMsg);

}

bool
COpsSwFrameProto::GetVoltageRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t usPdIdx, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

#if 0
    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    UTIL_ntohs((uint16_t *)pucMsg, (uint16_t *)pucMsg, sizeof(rstData.stVoltage) / sizeof(uint16_t));
    memcpy(&rstData.stVoltage, pucMsg, sizeof(rstData.stVoltage));
#else
    int32_t lVoltage = 0;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    memcpy(&lVoltage, pucMsg, sizeof(uint32_t));
    lVoltage = ntohl(lVoltage);
    LOG_WRITE(EN_LOG_DEBUG, "lVoltage = %d", lVoltage);
    rstData.stVoltage.asVoltage[uiSubIdx] = lVoltage;
#endif

    return true;
}

void
COpsSwFrameProto::GetILCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_GET_IL, staPayLoad, rstaMsg, uiSubIdx);

}

bool
COpsSwFrameProto::GetILRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    UTIL_ntohs((uint16_t *)pucMsg, (uint16_t *)pucMsg, sizeof(rstData.stIl) / sizeof(uint16_t));
    memcpy(&rstData.stIl, pucMsg, sizeof(rstData.stIl));

    return true;
}

void
COpsSwFrameProto::GetUpgStatCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;

    GeneralCmd(DEF_OPS_CMD_GET_UPGD_STAT, staPayload, rstaMsg);

}

bool
COpsSwFrameProto::GetUpgStatCmdRsp(const std::string &c_rstaMsg, COpsUpgStat &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);

    UTIL_ntohs((uint16_t *)pucMsg, (uint16_t *)pucMsg, sizeof(rstData.sStatus) / sizeof(uint16_t));
    memcpy(&rstData.sStatus, pucMsg, sizeof(rstData.sStatus));

    return true;
}
#if 0
void
COpsSwFrameProto::GetPdAdcCmd(std::string &rstaMsg, uint32_t uiPdIdx, uint32_t uiSubIdx)
{
    std::string staPayload;
    uint32_t uiId = uiPdIdx;
    uiId = htonl(uiId);
    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(uint32_t));
    memcpy(pucData, &uiId, sizeof(uint32_t));

    GeneralCmd(DEF_OPS_CMD_GET_PD_ADC, staPayLoad, rstaMsg);
}

void
COpsSwFrameProto::GetLPdAdcRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiPdIdx, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;
    int32_t lPdAdc = 0;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_FRM_READ_HEAD);
    memcpy(&lPdAdc, pucMsg, sizeof(uint32_t));

    lPdAdc = ntohl(lPdAdc);
    rstData.stPdAdc.asPdAdc[uiPdIdx] = lPdAdc;

}
#endif

void
COpsSwFrameProto::SetRemoteStatusCmd(COpsReqStatus& stStatus, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload((const char *)&stStatus, sizeof(stStatus));
    GeneralCmd(DEF_OPS_CMD_SET_REMOTE_STATUS, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetRequestStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_ENDS_STATUS, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetRequestStatusRsp(std::string &rstaMsg, COpsBidirectionStatus &rstData)
{
    memcpy(&rstData, rstaMsg.data() + sizeof(ST_FRM_READ_HEAD), sizeof(rstData));
    return true;
}

void
COpsSwFrameProto::SetSwitchingTypeCmd(uint8_t ucType, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload(sizeof(uint8_t), ucType);
    GeneralCmd(DEF_OPS_CMD_SET_SWITCHING_TYPE, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetSwitchingTypeCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    GeneralCmd(DEF_OPS_CMD_GET_SWITCHING_TYPE, staPayload, rstaMsg, uiSubIdx);
}

bool
COpsSwFrameProto::GetSwitchingTypeRsp(std::string &rstaMsg, uint8_t &ucType)
{
    ucType = *(rstaMsg.data() + sizeof(ST_FRM_READ_HEAD));
    return true;
}

void
COpsSwFrameProto::SetAlmMaskCmd(uint32_t uiIndex, uint32_t uiMask, std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayload;
    ST_ALM_MASK stAlmMask;

    uint8_t *pucData = (uint8_t *)staPayload.data();
    staPayload.resize(sizeof(stAlmMask));

    stAlmMask.usIdx = htons((uint16_t)uiIndex);
    stAlmMask.usMask = htons((uint16_t)uiMask);

    memcpy(pucData, &stAlmMask, sizeof(stAlmMask));

    GeneralCmd(DEF_OPS_CMD_SET_ALM_MASK, staPayload, rstaMsg, uiSubIdx);
}

void
COpsSwFrameProto::GetAlmMaskCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_GET_ALM_MASK, staPayLoad, rstaMsg);
}

bool
COpsSwFrameProto::GetAlmMaskRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsAlmMask *pstAlmMask = (COpsAlmMask *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

    memcpy((char *)&rstData.stAlmMask, pstAlmMask, sizeof(COpsAlmMask));

    return true;
}

void
COpsSwFrameProto::GetEventLogCmd(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staPayLoad;

    GeneralCmd(DEF_OPS_CMD_GET_EVENT_LOG, staPayLoad, rstaMsg);
}

bool
COpsSwFrameProto::GetEventLogRsp(const std::string &c_rstaMsg, COpsEventLogAllData &rstData)
{
    uint8_t *pucMsg = nullptr;

    pucMsg = (uint8_t *)c_rstaMsg.data();
    COpsEventLogAllData *pstEventLogAllData = (COpsEventLogAllData *)(pucMsg + sizeof(ST_FRM_READ_HEAD));

#if 1
    for (int i = 0; i < 4; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]OPS_SwitchOffsetTime  = 0x%0x", i, pstEventLogAllData->stOpsEventLogData[i].uiSwitchOffsetTime);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]OPS_uiAlarmOffsetTime = 0x%0x", i, pstEventLogAllData->stOpsEventLogData[i].uiAlarmOffsetTime);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]OPS_AlarmWPower      = 0x%0x", i, pstEventLogAllData->stOpsEventLogData[i].sAlarmWPower);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]OPS_AlarmPPower      = 0x%0x", i, pstEventLogAllData->stOpsEventLogData[i].sAlarmPPower);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]OPS_SwitchState       = 0x%0x", i, pstEventLogAllData->stOpsEventLogData[i].uiSwitchState);
    }
#endif

    /* memcpy((char *)&rstData, pstEventLogAllData, sizeof(COpsEventLogAllData)); */
    /* UTIL_ntohl((uint32_t *)pstEventLogAllData, (uint32_t *)&rstData, sizeof(COpsEventLogAllData)/sizeof(uint32_t)); */
#if 1
    for (int i = 0; i < 4; i++) {
        rstData.stOpsEventLogData[i].uiSwitchOffsetTime = ntohl(pstEventLogAllData->stOpsEventLogData[i].uiSwitchOffsetTime);
        rstData.stOpsEventLogData[i].uiAlarmOffsetTime = ntohl(pstEventLogAllData->stOpsEventLogData[i].uiAlarmOffsetTime);
        rstData.stOpsEventLogData[i].sAlarmWPower = ntohs(pstEventLogAllData->stOpsEventLogData[i].sAlarmWPower);
        rstData.stOpsEventLogData[i].sAlarmPPower = ntohs(pstEventLogAllData->stOpsEventLogData[i].sAlarmPPower);
        rstData.stOpsEventLogData[i].uiSwitchState = ntohl(pstEventLogAllData->stOpsEventLogData[i].uiSwitchState);

        LOG_WRITE(EN_LOG_NOTICE, "+[%d]OPS_SwitchOffsetTime  = 0x%0x", i, rstData.stOpsEventLogData[i].uiSwitchOffsetTime);
        LOG_WRITE(EN_LOG_NOTICE, "+[%d]OPS_uiAlarmOffsetTime = 0x%0x", i, rstData.stOpsEventLogData[i].uiAlarmOffsetTime);
        LOG_WRITE(EN_LOG_NOTICE, "+[%d]OPS_AlarmWPPower      = 0x%0x", i, rstData.stOpsEventLogData[i].sAlarmWPower);
        LOG_WRITE(EN_LOG_NOTICE, "+[%d]OPS_AlarmPPPower      = 0x%0x", i, rstData.stOpsEventLogData[i].sAlarmPPower);
        LOG_WRITE(EN_LOG_NOTICE, "+[%d]OPS_SwitchState       = 0x%0x", i, rstData.stOpsEventLogData[i].uiSwitchState);
    }
#endif

    return true;
}

