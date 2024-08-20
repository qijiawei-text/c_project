/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmProto.cpp
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcmProto.h"
#include "StringTool.h"
#include "OcmComm.h"


#define MOD_INFO_HW_VER           ("HW version: ")
#define MOD_INFO_BOOT_VER         ("Boot version: ")
#define MOD_INFO_MAIN_VER         ("Main version: ")
#define MOD_INFO_MODULE_SN        ("Module SN: ")
#define MOD_INFO_MANU_DATE        ("Manufacturing Date: ")
#define MOD_INFO_CALI_DATE        ("Calibration Date: ")
#define MOD_INFO_MODULE_PN        ("Module PN: ")

typedef enum OCM_CMD_IDX_en
{
    OCM_CMD_IDX_PORT = 0,
    OCM_CMD_IDX_OPCODE,
    OCM_CMD_IDX_LEN,
    OCM_CMD_IDX_DATA = 4,
    OCM_CMD_IDX_CNT
}OCM_CMD_IDX_EN;

#define USHORT_GET_HIGH_BYTE(usVal)    ((usVal >> 8) & 0x00FF)
#define USHORT_GET_LOW_BYTE(usVal)     (usVal & 0x00FF)

#define OCM_OPCODE_GET_PN         (0x0A)
#define OCM_OPCODE_GET_CID_PWR    (0x0B)
#define OCM_OPCODE_GET_MFG        (0x18)
#define OCM_OPCODE_GET_CH_ARRAY   (0x25)
#define OCM_OPCODE_SET_CH_ARRAY   (0x26)
#define OCM_OPCODE_GET_CH_SINGLE  (0x27)
#define OCM_OPCODE_SET_CH_SINGLE  (0x28)
#define OCM_OPCODE_GET_PWR_DBN    (0x11)
#define OCM_OPCODE_GET_OSA_DATA   (0x14)
#define OCM_OPCODE_GET_TEMP       (0x12)
#define OCM_OPCODE_GET_ALARM      (0x13)
#if 0
#define OCM_OPCODE_START_UPG      (0xf0)
#define OCM_OPCODE_SEND_DATA_UPG  (0xf1)
#define OCM_OPCODE_END_UPG        (0xf2)
#define OCM_OPCODE_WDG_RESET      (0xf3)                                    /* Do not used it at present                               */
#define OCM_OPCODE_SOFT_RESET     (0xf6)
#else
#define OCM_OPCODE_START_UPG      (0xf0)
#define OCM_OPCODE_SEND_DATA_UPG  (0xf1)
#define OCM_OPCODE_END_UPG        (0xf2)
#define OCM_OPCODE_WDG_RESET      (0xf3)                                    /* Do not used it at present                               */
#define OCM_OPCODE_VERIFY_UPG     (0xf4)
#define OCM_OPCODE_SWITCH_UPG     (0xf5)
#define OCM_OPCODE_SOFT_RESET     (0xf6)
#endif

#define LEN_192 (192)
#define RSP_HEAD (3)

#define OCM_BASE_FREQUENCE        (191175ll)
#define BAND_WIDTH_PER_SLICE      (6.25)
#define OCM_BASE_SLICE            OCM_BASE_FREQUENCE / BAND_WIDTH_PER_SLICE /* 30588 */

/*==============================function=========================*/

uint8_t
COcmSwFrameProto::CheckSum(uint8_t *pucStream, uint16_t usStreamLen)
{
    uint8_t ucThisByte = 0x00;
    uint16_t usIndex;

    if (NULL == pucStream) {
        /* LOG_WRITE(EN_LOG_ERROR, "Null pointer in SDV_OcmChksumGenOplk."); */

        /* If return 0, caller must check whether the parameter is right.       */
        return 0;
    }

    for (usIndex = 0; usIndex < usStreamLen; usIndex++) {
        ucThisByte += pucStream[usIndex];
    }

    /* The check sum is 0x00.                                                   */
    ucThisByte = (uint8_t)(0x00 - ucThisByte);

    /* Return Checksum.                                                         */
    return ucThisByte;

}

bool
COcmSwFrameProto::CheckState(uint8_t *pcResponse, bool &rbAgain)
{
    uint8_t ucRespStatus = 0xff;

    if (pcResponse == NULL) {
        /* LOG_WRITE(EN_LOG_ERROR,"Input parameter error!"); */
        return false;
    }

    ucRespStatus = *pcResponse;

#if 0
    /* find end flag from response                                              */
    if ((OCM_RESP_STATUS_OK != ucRespStatus) && (OCM_RESP_STATUS_NOVALID_DATA != ucRespStatus)) {
        LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus);
        return false;
    }
#endif

    if (OCM_RESP_STATUS_OK != ucRespStatus) {
        if (OCM_RESP_STATUS_NOVALID_DATA == ucRespStatus) {
            rbAgain = true;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus);
        }
        return false;
    }

    return true;
}


void
COcmSwFrameProto::GetCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg /*uint8_t *pucFrameBuf, uint32_t &ruiFrameLen*/)
{
    uint8_t *pucMsg = nullptr;

    if ((0 != usDataLen) && (NULL == pucDataBuf)) {
        /* LOG_WRITE(EN_LOG_ERROR,"Ocm opcode error in _OCM_FrameCreate."); */
        goto ERR_LABEL;
    }

    rstaMsg.resize(usDataLen + 5);

    pucMsg = (uint8_t *)rstaMsg.data();
    pucMsg[OCM_CMD_IDX_PORT] = ucPort;
    pucMsg[OCM_CMD_IDX_OPCODE] = ucOpcode;
    pucMsg[OCM_CMD_IDX_LEN] = USHORT_GET_LOW_BYTE(usDataLen);
    pucMsg[OCM_CMD_IDX_LEN + 1] = USHORT_GET_HIGH_BYTE(usDataLen);

    if (0 != usDataLen) {
        memcpy(&pucMsg[OCM_CMD_IDX_DATA], pucDataBuf, usDataLen);
    }

#if 0
    for (int i = 0; i < usDataLen; ++i) {
        printf("%02x ", *((uint8_t *)pucDataBuf + i));
        if (((i + 1) % 40) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    pucMsg[OCM_CMD_IDX_DATA + usDataLen] = CheckSum(pucMsg, 4 + usDataLen);

    return;

ERR_LABEL:
    return;
}


bool
COcmSwFrameProto::ProcRsp(std::string &rstaMsg)
{

    return true;

}

bool
COcmSwFrameProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    return true;
}

void
COcmSwFrameProto::GetMfgCmd(std::string &rstaMsg)
{
    /* 01 18 00 00 E7 */
    GetCmd(1, OCM_OPCODE_GET_MFG, nullptr, 0, rstaMsg);
}

bool
COcmSwFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    /* manufacture info */
    sprintf(rstMfg.acManufacturer, "%s", "OPLINK");

    /* Product Name */
    sprintf(rstMfg.acProductName, "%s", "OCM");


    /* HW version */
    if (!CStringTool::SubStr(rstMfg.acHwVer, c_rstaMsg, MOD_INFO_HW_VER, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_HW_VER);
        return false;
    }

    /* Boot version */
    if (!CStringTool::SubStr(rstMfg.acBootVer, c_rstaMsg, MOD_INFO_BOOT_VER, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_BOOT_VER);
        return false;
    }

    /* main version */
    if (!CStringTool::SubStr(rstMfg.acFwVer, c_rstaMsg, MOD_INFO_MAIN_VER, "\n", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    /* SN */
    if (!CStringTool::SubStr(rstMfg.acSN, c_rstaMsg, MOD_INFO_MODULE_SN, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_MODULE_SN);
        return false;
    }

    /* manufacture date */
    if (!CStringTool::SubStr(rstMfg.acMfgDate, c_rstaMsg, MOD_INFO_MANU_DATE, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_MANU_DATE);
        return false;
    }


    /* calibration date */
    if (!CStringTool::SubStr(rstMfg.acCaliDate, c_rstaMsg, MOD_INFO_CALI_DATE, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_CALI_DATE);
        return false;
    }

    strncpy(rstMfg.acPcbRev, rstMfg.acHwVer, MFG_INFO_LEN_MAX - 1);
    strcpy(rstMfg.acModuleType, "OCM");
    strcpy(rstMfg.acManufacturer, "Molex");

    return true;

}


bool
COcmSwFrameProto::GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, MOD_INFO_MODULE_PN, "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    return true;
}


void
COcmSwFrameProto::GetStatusData(std::string &rstaMsg)
{

}

#if 0
void
COcmSwFrameProto::SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan)
{
    /* get */
    /* 01 25 00 00 DA */
    /* 02 25 00 00 D9 */
    /* 03 25 00 00 D8 */
    /* 04 25 00 00 D7 */
    /* 05 25 00 00 D6 */

    uint32_t i = 0;
    uint8_t aucData[OCM_CMD_MAX_LEN] = {0xFF};
    CWavePlanSlice *pstWavePlanSlice = nullptr;
    uint32_t uiCenterFreq = 0;
    double dStartFreq = 0;
    uint16_t usStartSlice = 0;
    uint16_t usSliceCount = 0;


    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    pstWavePlanSlice = (CWavePlanSlice *)aucData;

    for (i = 0; i < uiChCnt; i++) {

        uiCenterFreq = pstWavePlan[i].uiCenterFreq;
        usSliceCount = pstWavePlan[i].usSliceCount;
        dStartFreq = uiCenterFreq - ((BAND_WIDTH_PER_SLICE * usSliceCount) / 2);
        usStartSlice = (uint16_t)((dStartFreq - OCM_BASE_FREQUENCE) / BAND_WIDTH_PER_SLICE + 1);

        pstWavePlanSlice[i].ucChId = pstWavePlan[i].uiChId;
        pstWavePlanSlice[i].usStartSlice = usStartSlice; /* htons(usStartSlice); */
        pstWavePlanSlice[i].usEndSlice = usStartSlice + usSliceCount - 1; /* htons(usStartSlice + usSliceCount - 1); */
    }

    for (i = 0; i < uiChCnt * sizeof(CWavePlanSlice); i++) {
        LOG_WRITE(EN_LOG_DEBUG, "0x%x", aucData[i]);
    }

    GetCmd(uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, aucData, uiChCnt * sizeof(CWavePlanSlice), rstaMsg);

    return;
}

#else

void
COcmSwFrameProto::SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{
    /* get */
    /* 01 25 00 00 DA */
    /* 02 25 00 00 D9 */
    /* 03 25 00 00 D8 */
    /* 04 25 00 00 D7 */
    /* 05 25 00 00 D6 */



    uint32_t i = 0;

    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    for (i = 0; i < uiChCnt; i++) {
        LOG_WRITE(EN_LOG_TRACE, "Before: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice);

        pstWavePlan[i].usStartSlice = pstWavePlan[i].usStartSlice - OCM_BASE_SLICE + 1;
        pstWavePlan[i].usEndSlice = pstWavePlan[i].usEndSlice - OCM_BASE_SLICE + 1;

        LOG_WRITE(EN_LOG_TRACE, "After: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice);
    }

    GetCmd(uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, (uint8_t *)pstWavePlan, uiChCnt * sizeof(CWavePlanSlice), rstaMsg);

    return;
}

#endif
void
COcmSwFrameProto::SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
    return;
}


void
COcmSwFrameProto::GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    /* 01 11 00 00 EE */
    /* 02 11 00 00 ED */
    /* 03 11 00 00 EC */
    /* 04 11 00 00 EB */
    /* 05 11 00 00 EA */
    GetCmd(uiPortIdx, OCM_OPCODE_GET_PWR_DBN, nullptr, 0, rstaMsg);
}

bool
COcmSwFrameProto::GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data() + RSP_HEAD, LEN_192);

    for (i = 0; i < (LEN_192 / sizeof(int16_t)); i++) {
        sTmp = rstData.asPower[i];
        sTmp = sTmp - 6000;   /* the power got from ocm is dbN = dbm * 100 + 6000 */

        memcpy(&(rstData.asPower[i]), &sTmp, sizeof(int16_t));
    }

    return true;
}


void
COcmSwFrameProto::GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    /* 01 11 00 00 EE */
    /* 02 11 00 00 ED */
    /* 03 11 00 00 EC */
    /* 04 11 00 00 EB */
    /* 05 11 00 00 EA */
    GetCmd(uiPortIdx, OCM_OPCODE_GET_OSA_DATA, nullptr, 0, rstaMsg);
}

bool
COcmSwFrameProto::GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data() + RSP_HEAD, OCM_SLICE_MAX_NUM * sizeof(int16_t));

    for (i = 0; i < OCM_SLICE_MAX_NUM; i++) {
        sTmp = rstData.asOsa[i];
        sTmp = sTmp - 6000;   /* the power got from ocm is dbN = dbm * 100 + 6000 */

        memcpy(&(rstData.asOsa[i]), &sTmp, sizeof(int16_t));
    }

#if 0
    for (int i = 0; i < sizeof(rstData); ++i) {
        printf("%02x ", *((uint8_t *)&rstData + i));
        if (((i + 1) % 40) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif


    return true;
}

void
COcmSwFrameProto::SoftReset(std::string &rstaMsg, uint32_t uiSubIdx)
{
    GetCmd(1, OCM_OPCODE_SOFT_RESET, nullptr, 0, rstaMsg);
}


void
COcmSwFrameProto::StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc)
{
    uint8_t aucBuf[8] = {0};

    aucBuf[0] = ulLen >> 24;
    aucBuf[1] = ulLen >> 16;
    aucBuf[2] = ulLen >> 8;
    aucBuf[3] = ulLen;

    aucBuf[4] = ulCrc >> 24;
    aucBuf[5] = ulCrc >> 16;
    aucBuf[6] = ulCrc >> 8;
    aucBuf[7] = ulCrc;

    GetCmd(1, OCM_OPCODE_START_UPG, aucBuf, sizeof(aucBuf), rstaMsg);
}
void
COcmSwFrameProto::SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    GetCmd(1, OCM_OPCODE_SEND_DATA_UPG, pcBuf, ulLen, rstaMsg);
}

void
COcmSwFrameProto::Verify(std::string &rstaMsg)
{
}

void
COcmSwFrameProto::Switch(std::string &rstaMsg)
{
}

void
COcmSwFrameProto::EndUpg(std::string &rstaMsg)
{
    GetCmd(1, OCM_OPCODE_END_UPG, nullptr, 0, rstaMsg);
}

void
COcmSwFrameProto::GetTempCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
}

bool
COcmSwFrameProto::GetTempRsp(std::string rstaRsp, int16_t &usTemp)
{
    return false;
}

void
COcmSwFrameProto::GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId)
{
}

bool
COcmSwFrameProto::GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstWavePlan)
{
    return true;
}
/*==============================Olsp=========================*/

uint8_t
COcmSwOlspFrameProto::CheckSum(uint8_t *pucStream, uint16_t usStreamLen)
{
    uint8_t ucThisByte = 0x00;
    uint16_t usIndex;

    if (NULL == pucStream) {
        /* LOG_WRITE(EN_LOG_ERROR, "Null pointer in SDV_OcmChksumGenOplk."); */

        /* If return 0, caller must check whether the parameter is right.       */
        return 0;
    }

    for (usIndex = 0; usIndex < usStreamLen; usIndex++) {
        ucThisByte += pucStream[usIndex];
    }

    /* The check sum is 0x00.                                                   */
    ucThisByte = (uint8_t)(0x00 - ucThisByte);

    /* Return Checksum.                                                         */
    return ucThisByte;

}

bool
COcmSwOlspFrameProto::CheckState(uint8_t *pcResponse, bool &rbAgain)
{
    uint8_t ucRespStatus = 0xff;

    if (pcResponse == NULL) {
        /* LOG_WRITE(EN_LOG_ERROR,"Input parameter error!"); */
        return false;
    }

    ucRespStatus = *pcResponse;

#if 0
    /* find end flag from response                                              */
    if ((OCM_RESP_STATUS_OK != ucRespStatus) && (OCM_RESP_STATUS_NOVALID_DATA != ucRespStatus)) {
        LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus);
        return false;
    }
#endif

    if (OCM_RESP_STATUS_OK != ucRespStatus) {
        if (OCM_RESP_STATUS_NOVALID_DATA == ucRespStatus) {
            rbAgain = true;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus);
        }
        return false;
    }

    return true;
}


void
COcmSwOlspFrameProto::GetCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg /*uint8_t *pucFrameBuf, uint32_t &ruiFrameLen*/)
{
    uint8_t *pucMsg = nullptr;

    if ((0 != usDataLen) && (NULL == pucDataBuf)) {
        /* LOG_WRITE(EN_LOG_ERROR,"Ocm opcode error in _OCM_FrameCreate."); */
        goto ERR_LABEL;
    }

    rstaMsg.resize(usDataLen + 5);

    pucMsg = (uint8_t *)rstaMsg.data();
    pucMsg[OCM_CMD_IDX_PORT] = ucPort;
    pucMsg[OCM_CMD_IDX_OPCODE] = ucOpcode;
    pucMsg[OCM_CMD_IDX_LEN] = USHORT_GET_LOW_BYTE(usDataLen);
    pucMsg[OCM_CMD_IDX_LEN + 1] = USHORT_GET_HIGH_BYTE(usDataLen);

    if (0 != usDataLen) {
        memcpy(&pucMsg[OCM_CMD_IDX_DATA], pucDataBuf, usDataLen);
    }

#if 0
    for (int i = 0; i < usDataLen; ++i) {
        printf("%02x ", *((uint8_t *)pucDataBuf + i));
        if (((i + 1) % 40) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    pucMsg[OCM_CMD_IDX_DATA + usDataLen] = CheckSum(pucMsg, 4 + usDataLen);

    return;

ERR_LABEL:
    return;
}


bool
COcmSwOlspFrameProto::ProcRsp(std::string &rstaMsg)
{

    return true;

}

bool
COcmSwOlspFrameProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    return true;
}

void
COcmSwOlspFrameProto::GetMfgCmd(std::string &rstaMsg)
{
    /* 01 18 00 00 E7 */
    GetCmd(1, OCM_OPCODE_GET_MFG, nullptr, 0, rstaMsg);
}

bool
COcmSwOlspFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    STModuleInfo *pstModuleInfo = nullptr;;
    uint8_t *pucMsg = nullptr;

    /* LOG_WRITE(EN_LOG_NOTICE, "UnPack +++"); */

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OCM_SPI_MSG_HEADER);
    pstModuleInfo = (STModuleInfo *)pucMsg;

    sprintf(rstMfg.acManufacturer, "%s", "Molex");
    sprintf(rstMfg.acModuleType, "%s", "OCM");
    memcpy(rstMfg.acFwVer, pstModuleInfo->acMainVersion, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acSwVer, pstModuleInfo->acBundleVersion, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acBootVer, pstModuleInfo->acBootVersion, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acHwVer, pstModuleInfo->acHwVersion, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acSN, pstModuleInfo->acSerialNumber, MFG_INFO_LEN_MAX - 1);
    sprintf(rstMfg.acProductName, "%s", "EDFA");
    memcpy(rstMfg.acFpgaRunVer, pstModuleInfo->acFpgaVersion, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acPN, pstModuleInfo->acPartNumber, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acMfgDate, pstModuleInfo->acMfgDate, MFG_INFO_LEN_MAX - 1);
    memcpy(rstMfg.acCaliDate, pstModuleInfo->acCalibrationDate, MFG_INFO_LEN_MAX - 1);

    return true;

}


bool
COcmSwOlspFrameProto::GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    return true;
}


void
COcmSwOlspFrameProto::GetStatusData(std::string &rstaMsg)
{

}

#if 0
void
COcmSwOlspFrameProto::SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlan *pstWavePlan)
{
    /* get */
    /* 01 25 00 00 DA */
    /* 02 25 00 00 D9 */
    /* 03 25 00 00 D8 */
    /* 04 25 00 00 D7 */
    /* 05 25 00 00 D6 */

    uint32_t i = 0;
    uint8_t aucData[OCM_CMD_MAX_LEN] = {0xFF};
    CWavePlanSlice *pstWavePlanSlice = nullptr;
    uint32_t uiCenterFreq = 0;
    double dStartFreq = 0;
    uint16_t usStartSlice = 0;
    uint16_t usSliceCount = 0;


    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    pstWavePlanSlice = (CWavePlanSlice *)aucData;

    for (i = 0; i < uiChCnt; i++) {

        uiCenterFreq = pstWavePlan[i].uiCenterFreq;
        usSliceCount = pstWavePlan[i].usSliceCount;
        dStartFreq = uiCenterFreq - ((BAND_WIDTH_PER_SLICE * usSliceCount) / 2);
        usStartSlice = (uint16_t)((dStartFreq - OCM_BASE_FREQUENCE) / BAND_WIDTH_PER_SLICE + 1);

        pstWavePlanSlice[i].ucChId = pstWavePlan[i].uiChId;
        pstWavePlanSlice[i].usStartSlice = usStartSlice; /* htons(usStartSlice); */
        pstWavePlanSlice[i].usEndSlice = usStartSlice + usSliceCount - 1; /* htons(usStartSlice + usSliceCount - 1); */
    }

    for (i = 0; i < uiChCnt * sizeof(CWavePlanSlice); i++) {
        LOG_WRITE(EN_LOG_DEBUG, "0x%x", aucData[i]);
    }

    GetCmd(uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, aucData, uiChCnt * sizeof(CWavePlanSlice), rstaMsg);

    return;
}

#else

void
COcmSwOlspFrameProto::SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{
    /* get */
    /* 01 25 00 00 DA */
    /* 02 25 00 00 D9 */
    /* 03 25 00 00 D8 */
    /* 04 25 00 00 D7 */
    /* 05 25 00 00 D6 */



    uint32_t i = 0;

    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    for (i = 0; i < uiChCnt; i++) {
        LOG_WRITE(EN_LOG_TRACE, "Before: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice);

        pstWavePlan[i].usStartSlice = pstWavePlan[i].usStartSlice - OCM_BASE_SLICE + 1;
        pstWavePlan[i].usEndSlice = pstWavePlan[i].usEndSlice - OCM_BASE_SLICE + 1;

        LOG_WRITE(EN_LOG_TRACE, "After: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice);
    }

    GetCmd(uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, (uint8_t *)pstWavePlan, uiChCnt * sizeof(CWavePlanSlice), rstaMsg);

    return;
}

#endif
void
COcmSwOlspFrameProto::SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
    /* get */
    /* 01 25 00 00 DA */
    /* 02 25 00 00 D9 */
    /* 03 25 00 00 D8 */
    /* 04 25 00 00 D7 */
    /* 05 25 00 00 D6 */
    uint32_t i = 0;

    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    for (i = 0; i < uiChCnt; i++) {

        LOG_WRITE(EN_LOG_DEBUG, "port[%d]wavplan[%d]: CentralFreq = %d, Bandwidth = %d", uiPortIdx, i, pstWavePlan[i].uCentralFreq, pstWavePlan[i].uBandwidth);
    }

    GetCmd(uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, (uint8_t *)pstWavePlan, uiChCnt * sizeof(STChannelGrid), rstaMsg);

    return;
}

void
COcmSwOlspFrameProto::GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    /* 01 11 00 00 EE */
    /* 02 11 00 00 ED */
    /* 03 11 00 00 EC */
    /* 04 11 00 00 EB */
    /* 05 11 00 00 EA */
    GetCmd(uiPortIdx, OCM_OPCODE_GET_PWR_DBN, nullptr, 0, rstaMsg);
}

bool
COcmSwOlspFrameProto::GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data() + sizeof(ST_OCM_SPI_MSG_HEADER), CHANNEL_NUMBER_THRESHOLD);

    for (i = 0; i < (OCM_CH_MAX_NUM / sizeof(int16_t)); i++) {
        sTmp = rstData.asPower[i];
        /*sTmp = sTmp - 6000;  */ /* the power got from ocm is dbN = dbm * 100 + 6000 */

        memcpy(&(rstData.asPower[i]), &sTmp, sizeof(int16_t));
    }

    return true;
}


void
COcmSwOlspFrameProto::GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    /* 01 11 00 00 EE */
    /* 02 11 00 00 ED */
    /* 03 11 00 00 EC */
    /* 04 11 00 00 EB */
    /* 05 11 00 00 EA */
    GetCmd(uiPortIdx, OCM_OPCODE_GET_OSA_DATA, nullptr, 0, rstaMsg);
}

bool
COcmSwOlspFrameProto::GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data() + RSP_HEAD, RAW_DATA_NUMBER_THRESHOLD * sizeof(int16_t));

    for (i = 0; i < OCM_SLICE_MAX_NUM; i++) {
        sTmp = rstData.asOsa[i];
        /* sTmp = sTmp - 6000; */   /* the power got from ocm is dbN = dbm * 100 + 6000 */

        memcpy(&(rstData.asOsa[i]), &sTmp, sizeof(int16_t));
    }

#if 0
    for (int i = 0; i < sizeof(rstData); ++i) {
        printf("%02x ", *((uint8_t *)&rstData + i));
        if (((i + 1) % 40) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif


    return true;
}

void
COcmSwOlspFrameProto::SoftReset(std::string &rstaMsg, uint32_t uiSubIdx)
{
    GetCmd(1, OCM_OPCODE_SOFT_RESET, nullptr, 0, rstaMsg);
}


void
COcmSwOlspFrameProto::StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc)
{
#if 0
    uint8_t aucBuf[8] = {0};

    aucBuf[0] = ulLen >> 24;
    aucBuf[1] = ulLen >> 16;
    aucBuf[2] = ulLen >> 8;
    aucBuf[3] = ulLen;

    aucBuf[4] = ulCrc >> 24;
    aucBuf[5] = ulCrc >> 16;
    aucBuf[6] = ulCrc >> 8;
    aucBuf[7] = ulCrc;

    GetCmd(1, OCM_OPCODE_START_UPG, aucBuf, sizeof(aucBuf), rstaMsg);
#endif
    GetCmd(1, OCM_OPCODE_START_UPG, nullptr, 0, rstaMsg);

}
void
COcmSwOlspFrameProto::SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    GetCmd(1, OCM_OPCODE_SEND_DATA_UPG, pcBuf, ulLen, rstaMsg);
}

void
COcmSwOlspFrameProto::Verify(std::string &rstaMsg)
{
    GetCmd(1, OCM_OPCODE_VERIFY_UPG, nullptr, 0, rstaMsg);
}

void
COcmSwOlspFrameProto::Switch(std::string &rstaMsg)
{
    GetCmd(1, OCM_OPCODE_SWITCH_UPG, nullptr, 0, rstaMsg);
}

void
COcmSwOlspFrameProto::EndUpg(std::string &rstaMsg)
{
    GetCmd(1, OCM_OPCODE_END_UPG, nullptr, 0, rstaMsg);
}

void
COcmSwOlspFrameProto::GetTempCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
}

bool
COcmSwOlspFrameProto::GetTempRsp(std::string rstaRsp, int16_t &usTemp)
{
    return false;
}

void
COcmSwOlspFrameProto::GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId)
{
    GetCmd(uiPortId, OCM_OPCODE_GET_CH_ARRAY, nullptr, 0, rstaMsg);
}

bool
COcmSwOlspFrameProto::GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstData)
{
    uint32_t i = 0;
    STChannelGrid *pstData = (STChannelGrid *)&rstData;
    memcpy(&rstData, (uint8_t *)rstaRsp.data() + RSP_HEAD, CHANNEL_NUMBER_THRESHOLD * sizeof(STChannelGrid));

    for (i = 0; i < CHANNEL_NUMBER_THRESHOLD; i++) {

        LOG_WRITE(EN_LOG_NOTICE, "waveplan[%d]: CentralFreq = %d, Bandwidth = %d", i, pstData[i].uCentralFreq, pstData[i].uBandwidth);
    }


#if 0
    for (int i = 0; i < sizeof(rstData); ++i) {
        printf("%02x ", *((uint8_t *)&rstData + i));
        if (((i + 1) % 40) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif


    return true;

}

