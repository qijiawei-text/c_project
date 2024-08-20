/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsProto.cpp
   Author:        Guo Guangjun
   Date:          2024-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-04] [1.0] [Creator]


*****************************************************************************************************/


/*=========================include head files====================*/

#include "OcsProto.h"
#include "StringTool.h"

/*==============================macro============================*/

#define USHORT_GET_HIGH_BYTE(usVal)    ((usVal >> 8) & 0x00FF)
#define USHORT_GET_LOW_BYTE(usVal)     (usVal & 0x00FF)

#define OCS_OPCODE_GET_MFG                   (0x01)
#define OCS_OPCODE_GET_SWITCH_CHANNELS       (0x10)
#define OCS_OPCODE_SET_SWITCH_CHANNELS       (0x11)
#define OCS_OPCODE_GET_ALARM                 (0x20)
#define OCS_OPCODE_GET_CASE_TEMPERATURE      (0x21)
#define OCS_OPCODE_GET_LED_STATE             (0x30)
#define OCS_OPCODE_SET_LAMP_TEST_STATE       (0x31)

#define OCS_OPCODE_SOFT_RESET                (0xF0)

#define OCS_OPCODE_START_UPG                 (0xF1)
#define OCS_OPCODE_DOWNLOAD_DATA_UPG         (0xF2)
#define OCS_OPCODE_VERIFY_UPG                (0xF3)
#define OCS_OPCODE_SWITCH_UPG                (0xF4)
#define OCS_OPCODE_COMMIT_UPG                (0xF5)
#define OCS_OPCODE_ABORT_DOWNLOAD_UPG        (0xF6)

/*==============================enum=========================*/
typedef enum OCM_CMD_IDX_en
{
    OCS_CMD_IDX_MSGID = 0,
    OCS_CMD_IDX_OPCODE,
    OCS_CMD_IDX_LEN,
    OCS_CMD_IDX_DATA = 4,
    OCS_CMD_IDX_CNT
} OCS_CMD_IDX_EN;

/*==============================internal structure=========================*/
typedef struct
{
    uint8_t ucMsgId;
    uint8_t ucStatus;
    uint16_t usDataLen;
}__attribute__((packed)) ST_OCS_SPI_MSG_HEADER;

typedef struct {
    char acVendor[DEF_OCS_MFG_INFO_LEN_MAX];
    char acType[DEF_OCS_MFG_INFO_LEN_MAX];
    char acSN[DEF_OCS_MFG_INFO_LEN_MAX];
    char acPN[DEF_OCS_MFG_INFO_LEN_MAX];
    char acHwVer[DEF_OCS_MFG_INFO_LEN_MAX];
    char acBootVer[DEF_OCS_MFG_INFO_LEN_MAX];
    char acFwVer[DEF_OCS_MFG_INFO_LEN_MAX];
    char acMfgDate[DEF_OCS_MFG_INFO_LEN_MAX];
    char acCalDate[DEF_OCS_MFG_INFO_LEN_MAX];
} ST_OCS_MFG_INFO;

/*==============================function=========================*/

uint8_t COcsSwFrameProto::m_ucGlobalMsgId = 0;

uint8_t
COcsSwFrameProto::GenerateFrameMsgId()
{
    uint8_t ucMsgId = m_ucGlobalMsgId++;
    return ucMsgId;
}

uint8_t
COcsSwFrameProto::CheckSum(uint8_t *pucStream, uint16_t usStreamLen)
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
COcsSwFrameProto::CheckState(uint8_t *pcResponse, bool &rbAgain)
{
    uint8_t ucRespStatus = OCS_RESP_STATUS_UNKNOWN_FAILURE;

    if (pcResponse == NULL) {
        /* LOG_WRITE(EN_LOG_ERROR,"Input parameter error!"); */
        return false;
    }

    ucRespStatus = *(pcResponse + 1);

    if (OCS_RESP_STATUS_OK != ucRespStatus) {
        if (OCS_RESP_STATUS_INVALID_DATA == ucRespStatus) {
            rbAgain = true;
            /*LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus); */
        } else {
            LOG_WRITE(EN_LOG_ERROR, "check status error! ucRespStatus = %d", ucRespStatus);
        }
        return false;
    }

    return true;
}

void
COcsSwFrameProto::BuildFrameCmd(uint8_t ucMsgId, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg)
{
    uint8_t *pucMsg = nullptr;

    if ((0 != usDataLen) && (NULL == pucDataBuf)) {
        /* LOG_WRITE(EN_LOG_ERROR,"Ocm opcode error in _OCM_FrameCreate."); */
        goto ERR_LABEL;
    }

    rstaMsg.resize(usDataLen + 5);

    pucMsg = (uint8_t *)rstaMsg.data();
    pucMsg[OCS_CMD_IDX_MSGID] = ucMsgId;
    pucMsg[OCS_CMD_IDX_OPCODE] = ucOpcode;
    pucMsg[OCS_CMD_IDX_LEN] = USHORT_GET_HIGH_BYTE(usDataLen);
    pucMsg[OCS_CMD_IDX_LEN + 1] = USHORT_GET_LOW_BYTE(usDataLen);

    if (0 != usDataLen) {
        memcpy(&pucMsg[OCS_CMD_IDX_DATA], pucDataBuf, usDataLen);
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

    pucMsg[OCS_CMD_IDX_DATA + usDataLen] = CheckSum(pucMsg, OCS_CMD_IDX_DATA + usDataLen);

    return;

ERR_LABEL:
    return;
}

bool
COcsSwFrameProto::ProcRsp(std::string &rstaMsg)
{
    return true;
}

bool
COcsSwFrameProto::ProcRsp(std::string &rstaMsg, bool &rbInProgress)
{
    return true;
}

void
COcsSwFrameProto::GetMfgCmd(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0x01><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_GET_MFG, nullptr, 0, rstaMsg);
}

bool
COcsSwFrameProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{
    size_t nMsgLen = c_rstaMsg.size();
    size_t nExpectMsgLen = sizeof(ST_OCS_SPI_MSG_HEADER) + sizeof(ST_OCS_MFG_INFO) + 1;
    char *pcDataBuf = NULL;
    ST_OCS_MFG_INFO stOcsMfg;

    if (nMsgLen < nExpectMsgLen) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid msg length! nMsgLen: %ld, nExpectMsgLen: %ld", nMsgLen, nExpectMsgLen);
        return false;
    }

    /* Response: <IDX><Status><0x01><0x00><D0~D255><CS> */

    pcDataBuf = (char *)c_rstaMsg.data() + sizeof(ST_OCS_SPI_MSG_HEADER);
    memset(&stOcsMfg, 0, sizeof(stOcsMfg));
    memcpy(&stOcsMfg, pcDataBuf, sizeof(stOcsMfg));

    /* memcpy(rstMfg.acManufacturer, stOcsMfg.acVendor, sizeof(stOcsMfg.acVendor)); */
    snprintf(rstMfg.acManufacturer, sizeof(rstMfg.acManufacturer), "%s", "Molex");
    memcpy(rstMfg.acModuleType, stOcsMfg.acType, sizeof(stOcsMfg.acType));
    memcpy(rstMfg.acSN, stOcsMfg.acSN, sizeof(stOcsMfg.acSN));
    memcpy(rstMfg.acPN, stOcsMfg.acPN, sizeof(stOcsMfg.acPN));
    memcpy(rstMfg.acHwVer, stOcsMfg.acHwVer, sizeof(stOcsMfg.acHwVer));
    memcpy(rstMfg.acBootVer, stOcsMfg.acBootVer, sizeof(stOcsMfg.acBootVer));
    memcpy(rstMfg.acFwVer, stOcsMfg.acFwVer, sizeof(stOcsMfg.acFwVer));
    memcpy(rstMfg.acMfgDate, stOcsMfg.acMfgDate, sizeof(stOcsMfg.acMfgDate));
    memcpy(rstMfg.acCaliDate, stOcsMfg.acCalDate, sizeof(stOcsMfg.acCalDate));

    /* special hander */
    memcpy(rstMfg.acProductName, stOcsMfg.acType, sizeof(stOcsMfg.acType));

    return true;

}


void
COcsSwFrameProto::GetSwitchChannelsCmd(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0x10><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_GET_SWITCH_CHANNELS, nullptr, 0, rstaMsg);
}

bool
COcsSwFrameProto::GetSwitchChannelsRsp(const std::string &rstaRsp, COcsConnections &rstData)
{
    uint16_t usDataLen = 0;
    uint8_t *pucBuf = NULL;
    uint16_t usExpDataLen = 0;
    uint8_t aucChannels[OCS_CONNECTION_MAX] = {0};
    uint32_t i = 0;

    memset(aucChannels, 0, sizeof(aucChannels));

    /*Response: <IDX><Status><0x00><0x20><D0~D63><CS> */

    pucBuf = (uint8_t *)rstaRsp.data();

    usDataLen = OCS_CMD_RESP_DATA_LENGTH(pucBuf[OCS_CMD_IDX_LEN], pucBuf[OCS_CMD_IDX_LEN + 1]);
    usExpDataLen = sizeof(aucChannels);

    if (usDataLen < usExpDataLen) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid data length! DataLen: %d, ExpectedDataLen: %d", usDataLen, usExpDataLen);
        return false;
    }

    memcpy(aucChannels, pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER), sizeof(aucChannels));

    memset(&rstData, 0, sizeof(rstData));
    rstData.uiCount = OCS_CONNECTION_MAX;

    for (i = 0; i < rstData.uiCount; i++) {
        rstData.astConn[i].ucId = i;
        rstData.astConn[i].ucSource = i + 1;
        rstData.astConn[i].ucDestination = aucChannels[i];
    }

    return true;
}

void
COcsSwFrameProto::SetSwitchChannelsCmd(std::string &rstaMsg, const COcsConnections &rstData)
{

    uint8_t ucMsgId = GenerateFrameMsgId();
    uint8_t aucChannels[OCS_CONNECTION_MAX] = {0};
    uint32_t uiCnt = 0;
    uint32_t i = 0;

    uiCnt = rstData.uiCount > OCS_CONNECTION_MAX ? OCS_CONNECTION_MAX : rstData.uiCount;

    memset(aucChannels, 0, sizeof(aucChannels));
    for (i = 0; i < uiCnt; i++) {
        aucChannels[i] = rstData.astConn[i].ucDestination;
    }

    /*Command: <IDX><0x11><0x00><0x80><D0~D127><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_SET_SWITCH_CHANNELS, aucChannels, sizeof(aucChannels), rstaMsg);

    return;
}


void
COcsSwFrameProto::GetAlarmCmd(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0x20><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_GET_ALARM, nullptr, 0, rstaMsg);
}

bool
COcsSwFrameProto::GetAlarmRsp(const std::string &rstaRsp, COcsAlarm &rstData)
{
    uint16_t usDataLen = 0;
    uint8_t *pucBuf = NULL;
    uint16_t usExpDataLen = 0;

    /*****************************************************************
    **	Response: <IDX><Status><0x00><0x08><D0~D7><CS>
    **	D0~~D7: 0x00 - alarm off; 0x01 - alarm on
    **	D0: Power supply voltage high alarm
    **	D1: Power supply voltage low alarm
    **	D2: Switch driver voltage high alarm
    **	D3: Switch driver voltage low alarm
    **	D4: Calibration data alarm
    **	D5~D7: Reserve
    *****************************************************************/

    pucBuf = (uint8_t *)rstaRsp.data();

    usDataLen = OCS_CMD_RESP_DATA_LENGTH(pucBuf[OCS_CMD_IDX_LEN], pucBuf[OCS_CMD_IDX_LEN + 1]);
    usExpDataLen = sizeof(COcsAlarm);

    if (usDataLen < usExpDataLen) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid data length! DataLen: %d, ExpectedDataLen: %d", usDataLen, usExpDataLen);
        return false;
    }

    memcpy(&rstData, pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER), sizeof(rstData));

    return true;
}

void
COcsSwFrameProto::GetCaseTemperatureCmd(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /* Command: <IDX><0x21><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_GET_CASE_TEMPERATURE, nullptr, 0, rstaMsg);
}

bool
COcsSwFrameProto::GetCaseTemperatureRsp(const std::string &rstaRsp, COcsTempSta &rstData)
{
    uint16_t usDataLen = 0;
    uint8_t *pucBuf = NULL;
    uint16_t usExpDataLen = 0;

    /****************************************************************************************************
    **	Response:  <IDX><Status><0x00><0x02><D0~D1><CS>
    **	D0~D1: Case temperature, MSB first.
    **	Temperature is represented by a signed integer (2 bytes).
    **	The temperature shall be represented by a minimum resolution which is 0.01 degrees Celsius here.
    **	For example, temperature -1.2 degrees Celsius will be as 0xFF88 (MSB)
    **	      -1.2dBm   -120 (in 0.01 degrees Celsius)   0xFF88 (D0 is 0xFF and D1 is 0x88)
    ******************************************************************************************************/

    pucBuf = (uint8_t *)rstaRsp.data();

    usDataLen = OCS_CMD_RESP_DATA_LENGTH(pucBuf[OCS_CMD_IDX_LEN], pucBuf[OCS_CMD_IDX_LEN + 1]);
    usExpDataLen = sizeof(int16_t);

    if (usDataLen < usExpDataLen) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid data length! DataLen: %d, ExpectedDataLen: %d", usDataLen, usExpDataLen);
        return false;
    }

    pucBuf = pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER);

#if 0
    int16_t sTemp = 0;
    memcpy(&sTemp, pucBuf, sizeof(sTemp));
    rstData.sCaseTemp = (int16_t)ntohl(sTemp);
#else
    rstData.sCaseTemp = (((*pucBuf) & 0x00FF) << 8) | ((*(pucBuf + 1)) & 0x00FF);
#endif

    /* LOG_WRITE(EN_LOG_DEBUG, "sCaseTemp: %d, first: %02x, second: %02x",
                  rstData.sCaseTemp, *(pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER)), *(pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER) + 1)); */
    return true;
}

void
COcsSwFrameProto::GetLedStateCmd(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0x30><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_GET_LED_STATE, nullptr, 0, rstaMsg);
}

bool
COcsSwFrameProto::GetLedStateRsp(const std::string &rstaRsp, COcsLedSta &rstData)
{
    uint16_t usDataLen = 0;
    uint8_t *pucBuf = NULL;
    uint8_t *pucDataBuf = NULL;
    uint16_t usExpDataLen = 3;


    /*****************************************************************
    **	Response: <IDX><Status><0x00><0x03><D0><D1><D2><CS>
    **	D0: LED work state
    **				0 - Normal work state
    **				1 - Lamp test state
    **	D1: ACT LED State, refer to below table.
    **	D2: ALM LED State, refer to below table.
    *****************************************************************/

    pucBuf = (uint8_t *)rstaRsp.data();

    usDataLen = OCS_CMD_RESP_DATA_LENGTH(pucBuf[OCS_CMD_IDX_LEN], pucBuf[OCS_CMD_IDX_LEN + 1]);

    if (usDataLen < usExpDataLen) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid data length! DataLen: %d, ExpectedDataLen: %d", usDataLen, usExpDataLen);
        return false;
    }

    pucDataBuf = pucBuf + sizeof(ST_OCS_SPI_MSG_HEADER);
    rstData.ucLedWorkSta = pucDataBuf[0];
    rstData.aucLedSta[0] = pucDataBuf[1];
    rstData.aucLedSta[1] = pucDataBuf[2];

    return true;
}

void
COcsSwFrameProto::SetLampTestStateCmd(std::string &rstaMsg, bool bEnable)
{
    uint8_t ucMsgId = GenerateFrameMsgId();
    uint8_t ucState = bEnable ? 1 : 0;

    /*Command: <IDX><0x31><0x00><0x01><D0><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_SET_LAMP_TEST_STATE, &ucState, sizeof(ucState), rstaMsg);

    return;
}

void
COcsSwFrameProto::SoftReset(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();
    uint8_t ucData = 0x69;

    /*Command: <IDX><0xF0><0x00><0x01><0x69><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_SOFT_RESET, &ucData, sizeof(ucData), rstaMsg);
}

void
COcsSwFrameProto::StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc)
{
    uint8_t ucMsgId = GenerateFrameMsgId();
    uint8_t aucData[2] = {0xA5, 0xC3};

    /*Command: <IDX><0xF1><0x00><0x02><0xA5><0xC3><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_START_UPG, aucData, sizeof(aucData), rstaMsg);
}

void
COcsSwFrameProto::DownloadData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0xF2><0x00><N><D0-D(N-1)><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_DOWNLOAD_DATA_UPG, pcBuf, ulLen, rstaMsg);

}

void
COcsSwFrameProto::Verify(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0XF3><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_VERIFY_UPG, nullptr, 0, rstaMsg);
}

void
COcsSwFrameProto::Switch(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0XF4><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_SWITCH_UPG, nullptr, 0, rstaMsg);
}

void
COcsSwFrameProto::Commit(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0XF5><0x00><0x00><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_COMMIT_UPG, nullptr, 0, rstaMsg);
}

void
COcsSwFrameProto::AbortDownload(std::string &rstaMsg)
{
    uint8_t ucMsgId = GenerateFrameMsgId();

    /*Command: <IDX><0xF6><0x00><0x02><0xA5><0xC3><CS> */
    BuildFrameCmd(ucMsgId, OCS_OPCODE_ABORT_DOWNLOAD_UPG, nullptr, 0, rstaMsg);
}

