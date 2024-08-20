/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmMasterSpiProto.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OcmMasterSpiProto.h"
#include "StringTool.h"



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


#define OCM_OPCODE_GET_MFG        (0x18)
#define OCM_OPCODE_GET_CH_ARRAY   (0x25)
#define OCM_OPCODE_SET_CH_ARRAY   (0x26)
#define OCM_OPCODE_GET_TEMP       (0x12)
#define OCM_OPCODE_GET_ALARM      (0x13)
#define OCM_OPCODE_GET_CH_PWRS_FREQS      (0x07)
#define OCM_OPCODE_GET_PWR_DBN    (0x11)
#define OCM_OPCODE_GET_OSA_DATA   (0x14)
#define OCM_OPCODE_GET_SPECTRUM_PWRS_DBN (0x19)
#define OCM_OPCODE_START_UPG      (0xf0)
#define OCM_OPCODE_SEND_DATA_UPG  (0xf1)
#define OCM_OPCODE_END_UPG        (0xf2)
#define OCM_OPCODE_COLD_RESET     (0xf3)
#define OCM_OPCODE_SOFT_RESET     (0xf6)

#define LEN_192                   (192)
#define RSP_HEAD                  (3)

#define OCM_BASE_FREQUENCE        (186237.50)  /*qingy 2022 */
#define BAND_WIDTH_PER_SLICE      (6.25)

#define OCM_BASE_SLICE            (OCM_BASE_FREQUENCE / BAND_WIDTH_PER_SLICE) /* 30588 */

/*==============================function=========================*/

uint8_t
COcmSwMasterSpiFrameProto::CheckSum(uint8_t *pucStream, uint16_t usStreamLen)
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
COcmSwMasterSpiFrameProto::CheckState(uint8_t *pcResponse, bool &rbAgain)
{
    uint8_t ucRespStatus = 0xff;

    if (pcResponse == NULL) {
        /* LOG_WRITE(EN_LOG_ERROR,"Input parameter error!"); */
        return false;
    }

    ucRespStatus = *pcResponse;

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
COcmSwMasterSpiFrameProto::GetCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg /*uint8_t *pucFrameBuf, uint32_t &ruiFrameLen*/)
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
COcmSwMasterSpiFrameProto::ProcRsp(std::string & rstaMsg)
{

    bool bRet = false;
    bool bInProgress = false;

    bRet = CFrameProto::UnPack(rstaMsg, bInProgress);
    LOG_WRITE(EN_LOG_NOTICE, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bInProgress;
}

bool
COcmSwMasterSpiFrameProto::ProcRsp(std::string & rstaMsg, bool &rbInProgress)
{

    bool bRet = false;
    bool bInProgress = false;

    bRet = CFrameProto::UnPack(rstaMsg, bInProgress, MODULE_TPYE_OCM);
    rbInProgress = bInProgress;
    LOG_WRITE(EN_LOG_NOTICE, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bRet;
}

void
COcmSwMasterSpiFrameProto::GetMfgCmd(std::string &rstaMsg)
{
    /* 01 18 00 00 E7 */
    /*GetCmd(1, OCM_OPCODE_GET_MFG, nullptr, 0, rstaMsg); */

    std::string staBody;
    CFrameProto::Pack(22, 1, OCM_OPCODE_GET_MFG, staBody, rstaMsg, MODULE_TPYE_OCM);
}

bool
COcmSwMasterSpiFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
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

    /* PN */
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, MOD_INFO_MODULE_PN, "\n", MFG_INFO_LEN_MAX - 1)) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error.", MOD_INFO_MODULE_PN);
        return false;
    }

    strcpy(rstMfg.acManufacturer, "Molex");
    strcpy(rstMfg.acModuleType, "Molex OCM");
    strncpy(rstMfg.acPcbRev, rstMfg.acHwVer, MFG_INFO_LEN_MAX - 1);

    return true;

}


bool
COcmSwMasterSpiFrameProto::GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    if (!CStringTool::SubStr(rstMfg.acPN, c_rstaMsg, MOD_INFO_MODULE_PN, "\r", MFG_INFO_LEN_MAX - 1)) {
        return false;
    }

    return true;
}


void
COcmSwMasterSpiFrameProto::GetStatusData(std::string &rstaMsg)
{

}


void
COcmSwMasterSpiFrameProto::SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{

    uint32_t i = 0;

    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlanCmd uiPortIdx = %u, OCM_BASE_SLICE = %.2f", uiPortIdx, OCM_BASE_SLICE);
    printf("SetWavePlanCmd uiPortIdx = %u, OCM_BASE_SLICE = %.2f\n", uiPortIdx, OCM_BASE_SLICE);
    if (nullptr == pstWavePlan) {
        LOG_WRITE(EN_LOG_ERROR, "pstWavePlan is nullptr.");
        return;
    }

    for (i = 0; i < uiChCnt; i++) {    /*uiChCnt */
        /*LOG_WRITE(EN_LOG_NOTICE, "Before: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice); */
        pstWavePlan[i].usStartSlice = htons(pstWavePlan[i].usStartSlice - OCM_BASE_SLICE + 1);
        pstWavePlan[i].usEndSlice = htons(pstWavePlan[i].usEndSlice - OCM_BASE_SLICE + 1);
        /*LOG_WRITE(EN_LOG_NOTICE, "After: StartSlice = %d, EndSlice = %d", pstWavePlan[i].usStartSlice, pstWavePlan[i].usEndSlice); */
    }

    std::string staBody;
    staBody.append((char *)pstWavePlan, uiChCnt * sizeof(CWavePlanSlice));

    CFrameProto::Pack(22, uiPortIdx, OCM_OPCODE_SET_CH_ARRAY, staBody, rstaMsg, MODULE_TPYE_OCM);

    return;
}

void
COcmSwMasterSpiFrameProto::SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
    return;
}

void
COcmSwMasterSpiFrameProto::GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    std::string staBody;
    CFrameProto::Pack(22, uiPortIdx, OCM_OPCODE_GET_PWR_DBN, staBody, rstaMsg, MODULE_TPYE_OCM);
}

bool
COcmSwMasterSpiFrameProto::GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data(), LEN_192);         /*qingy 2022 */

    for (i = 0; i < (LEN_192 / sizeof(int16_t)); i++) {
        sTmp = rstData.asPower[i];
        sTmp = sTmp - 6000;   /* the power got from ocm is dbN = dbm * 100 + 6000 */

        memcpy(&(rstData.asPower[i]), &sTmp, sizeof(int16_t));
    }

    return true;
}

void
COcmSwMasterSpiFrameProto::GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    unsigned char ucSliceType = DEF_OCM_Slice_Type4;
    std::string staBody;
    staBody.append((char *)&ucSliceType, sizeof(ucSliceType));
    CFrameProto::Pack(22, uiPortIdx, OCM_OPCODE_GET_OSA_DATA, staBody, rstaMsg, MODULE_TPYE_OCM);
}

bool
COcmSwMasterSpiFrameProto::GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData)
{
    uint32_t i = 0;
    int16_t sTmp = 0;
    memcpy(&rstData, (uint8_t *)rstaRsp.data(), OCM_SLICE_MAX_NUM * sizeof(int16_t));

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
COcmSwMasterSpiFrameProto::SoftReset(std::string &rstaMsg, uint32_t uiSubIdx)
{
    std::string staBody;
    CFrameProto::Pack(22, 1, OCM_OPCODE_SOFT_RESET, staBody, rstaMsg, MODULE_TPYE_OCM);
}

void
COcmSwMasterSpiFrameProto::StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc)
{
    std::string staBody;
    CFrameProto::Pack(22, 1, OCM_OPCODE_START_UPG, staBody, rstaMsg, MODULE_TPYE_OCM);
}
void
COcmSwMasterSpiFrameProto::SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    std::string staBody;
    staBody.append((char *)pcBuf, ulLen);
    CFrameProto::Pack(22, 1, OCM_OPCODE_SEND_DATA_UPG, staBody, rstaMsg, MODULE_TPYE_OCM);
}

void
COcmSwMasterSpiFrameProto::Verify(std::string &rstaMsg)
{
}

void
COcmSwMasterSpiFrameProto::Switch(std::string &rstaMsg)
{
}

void
COcmSwMasterSpiFrameProto::EndUpg(std::string &rstaMsg)
{
    std::string staBody;
    CFrameProto::Pack(22, 1, OCM_OPCODE_END_UPG, staBody, rstaMsg, MODULE_TPYE_OCM);
}

void
COcmSwMasterSpiFrameProto::GetTempCmd(std::string &rstaMsg, uint32_t uiPortIdx)
{
    std::string staBody;
    CFrameProto::Pack(0x0a, uiPortIdx, OCM_OPCODE_GET_TEMP, staBody, rstaMsg, MODULE_TPYE_OCM);
}

bool
COcmSwMasterSpiFrameProto::GetTempRsp(std::string rstaRsp, int16_t &usTemp)
{
    int16_t sTmp = 0;
    uint8_t *pucData = nullptr;
    uint16_t usBodyLen = 0;

    usBodyLen = (uint16_t)rstaRsp.size();
    pucData = (uint8_t *)rstaRsp.data();
    LOG_WRITE(EN_LOG_NOTICE, "GetTempRsp,usBodyLen:%d", usBodyLen);
    for (uint32_t uiIdx = 0; uiIdx < rstaRsp.size(); uiIdx++) {
        LOG_WRITE(EN_LOG_NOTICE, "%0x", pucData[uiIdx]);
    }

    /*memcpy(sTmp, (uint8_t *)rstaRsp.data() + RSP_HEAD, 2); //qingy 2022 */
    memcpy(&sTmp, (uint8_t *)rstaRsp.data(), 2);
    sTmp = ntohs(sTmp);
    usTemp = sTmp;
    LOG_WRITE(EN_LOG_NOTICE, "sTmp = %d,usTemp = %d", sTmp, usTemp);
    return true;
}

void
COcmSwMasterSpiFrameProto::GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId)
{
}

bool
COcmSwMasterSpiFrameProto::GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstWavePlan)
{
    return true;
}

