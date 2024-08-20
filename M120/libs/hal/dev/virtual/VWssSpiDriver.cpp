/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      VWssSpiDriver.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:



*****************************************************************************************************/

/*=========================include head files====================*/
#include "VWssSpiDriver.h"

#define LEN_192                   (192)

#define DEF_WRITE             (1)
#define DEF_READ              (2)
/*==============================function=========================*/

bool
CWssVSpi::ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                    uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    return true;
}

static uint16_t
_WssNis_Crc16(uint8_t *pucBuf, uint32_t ulLen)
{
    /* CRC-16, polynomial = 0x8005                                              */
    const uint16_t ausCrc16Tbl[256] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0,
        0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41,
        0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0,
        0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
        0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341,
        0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1,
        0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
        0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940,
        0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1,
        0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0,
        0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
        0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0,
        0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0,
        0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
        0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0,
        0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0,
        0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40,
        0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
        0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0,
        0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    uint16_t usCrc16 = 0;

    /* Check parameters                                                         */
    if ((NULL == pucBuf) ||
        (0 == ulLen)) {
        goto ERR_LABEL;
    }

    while (ulLen--) {
        usCrc16 = ausCrc16Tbl[(uint8_t)(usCrc16) ^ (*pucBuf++)] ^ (usCrc16 >> 8);
    }

    return usCrc16;

ERR_LABEL:
    return 0;
}


/*| Long Command Format:                                                                                 |*/
/*|------------------------------------------------------------------------------------------------------|*/
/*|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  |13 ?? n+12| n+13 | n+14 |*/
/*|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----------|------|------|*/
/*| MID |0x00 |0xFF |    Len    |    Cmd    | Oid | Tbl | Row |Inst1|Para |InstN|   Data   |    CRC16    |*/
/*|------------------------------------------------------------------------------------------------------|*/
/*|                                               | Optional  |           |Array|                        |*/
/*|------------------------------------------------------------------------------------------------------|*/

bool
CWssVSpi::Write(uint32_t uiHwSubIdx, uint8_t *pucBufW, uint32_t usLenW)
{
#if 0
    for (int i = 0; i < (int)100; i++) {
        std::cout << "index" << i;
        printf(": 0x%02x\n", (uint8_t)pucBufW[i]);
    }
#endif

    LOG_WRITE(EN_LOG_NOTICE, "VSPI Start ++ ");
    /*init */
    memset(&m_uscmd, 0, sizeof(m_uscmd));
    memset(&m_uslen, 0, sizeof(m_uslen));
    memset(&m_ucOid, 0, sizeof(m_ucOid));
    memset(&m_ucMid, 0, sizeof(m_ucMid));
    memset(m_sRhead, 0, sizeof(m_sRhead));

    /*get OID */
    m_ucMid = pucBufW[0];

    /*cmd type */
    uint16_t uscmd = 0;
    memcpy(&uscmd, pucBufW + 5, sizeof(uscmd));
    uscmd = htons(uscmd);
    LOG_WRITE(EN_LOG_NOTICE, "VSPI uscmd:  %d", uscmd);

    /*get W/R cmd */
    uscmd &= 31;
    m_uscmd = uscmd;
    LOG_WRITE(EN_LOG_NOTICE, "VSPI m_uscmd:  %d", m_uscmd);

    /*check cmd */
    uint8_t usWorR = 0;
    switch (m_uscmd) {
    case EN_WSS_NIS_CMD_RD:
        usWorR = DEF_READ;
        break;

    case EN_WSS_NIS_CMD_ARRAY_RD:
        usWorR = DEF_READ;
        break;

    case EN_WSS_NIS_CMD_WR:
        usWorR = DEF_WRITE;
        break;

    case EN_WSS_NIS_CMD_ARRAY_WR:
        usWorR = DEF_WRITE;
        break;

    case EN_WSS_NIS_CMD_MULTI_WR:
        return MultiWrite(pucBufW, usLenW);

    default:
        LOG_WRITE(EN_LOG_ERROR, "Wss VSPI Unkown Cmd");
        return false;
    }

    /*get comm and receive write */
    std::string Wcommand((char *)pucBufW, usLenW);
    uint8_t comm = (uint8_t)pucBufW[7];
    LOG_WRITE(EN_LOG_NOTICE, "VWssSPI OID: %d", comm);
    /*save OID(comm) local */
    m_ucOid = comm;

    /*check Read cmd */
    if (RCommandsMap.find(comm) != RCommandsMap.end() && DEF_READ == usWorR) {
        LOG_WRITE(EN_LOG_NOTICE, "VSPI Read cmd");
        memcpy(m_sRhead, pucBufW, usLenW);
        return true;
    }

    /*check Write cmd */
    if (WCommandsMap.find(comm) == WCommandsMap.end() && DEF_WRITE == usWorR) {
        LOG_WRITE(EN_LOG_ERROR, "VSPI received unrecognized command!");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "VSPI Write cmd");

    /*execute cmd */
    (this->*WCommandsMap.at (m_ucOid))(Wcommand);


    return true;
}

/*| Multi-Object Commands in Long Format:                                                                |*/
/*|------------------------------------------------------------------------------------------------------|*/
/*|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  |13 ?? n+12| n+13 | n+14 |*/
/*|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----------|------|------|*/
/*| MID |0x00 |0xFF |    Len    | Cmd(0x12) |Oid1 |Tbl1 |Inst1|Para1|   Data1   |                        |*/
/*|                                         |Oid2 |Tbl2 |Inst2|Para2|   Data2   |                        |*/
/*|                                                      ...                                             |*/
/*|                                         |OidN |TblN |InstN|ParaN|   DataN   |          |    CRC16    |*/
/*|------------------------------------------------------------------------------------------------------|*/

bool
CWssVSpi::MultiWrite(uint8_t *pucBufW, uint32_t usLenW)
{
    uint16_t ulDatabuf[2] = {0};
    uint16_t usData = 0;
    uint16_t ucChCnt = 0;

    LOG_WRITE(EN_LOG_NOTICE, "VSPI MultiWrite ++");
    LOG_WRITE(EN_LOG_NOTICE, "usLenW: %d", usLenW);

    for (uint16_t i = 7; i < (uint16_t)usLenW - 2; i += 6) {
        memset(&ulDatabuf, 0, sizeof(ulDatabuf));

        uint8_t m_ucOid = (uint8_t)pucBufW[i];
        /*channnel index */
        ucChCnt = (i - 7) / 12 + 1;

        /*ChCnt + Data */
        memcpy(&ulDatabuf[0], &ucChCnt, sizeof(ucChCnt));
        memcpy(&usData, &pucBufW[i + 4], sizeof(usData));
        ucChCnt = ntohs(ucChCnt);
        usData = ntohs(usData);

        memcpy(&ulDatabuf[1], &usData, sizeof(usData));

        std::string Wcommand((char *)ulDatabuf, sizeof(ucChCnt) + sizeof(usData));

        /*execute cmd */
        (this->*WCommandsMap.at (m_ucOid))(Wcommand);

    }
    memset(&m_ucOid, 0, sizeof(m_ucOid));
    LOG_WRITE(EN_LOG_NOTICE, "VSPI MultiWrite --");

    return true;
}

/*| Long Command Response Format:                       |                   */
/*|-----------------------------------------------------|                   */
/*|  0  |  1  |  2  |  3  |  4  |  5 ?? n+4 | n+5 | n+6 |                   */
/*|-----|-----------|-----|-----|-----------|-----------|                   */
/*| MID |    Len    |Rslt | Cnt |    Data   |   Crc16   |                   */
/*|-----------------------------------------------------|                   */
/*|                       |Array|                       |                   */
/*|-----------------------------------------------------|                   */

bool
CWssVSpi::Read(uint32_t uiHwSubIdx, uint8_t *paucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI Read");

    /*Head */
    ST_WSS_NIS_RESP_L_HEAD_CMN stVhead;
    memset(&stVhead, 0, sizeof(stVhead));

    /*CRC */
    uint16_t usCrc = 0;

    /*get OID */
    uint8_t comm = m_ucOid;
    LOG_WRITE(EN_LOG_NOTICE, "VWssSPI OID: %d", comm);

    /*check Cnt */
    uint8_t usCntExist = 0;

    /*check cmd */
    switch (m_uscmd) {
    case EN_WSS_NIS_CMD_RD:
        usCntExist = 0;
        break;

    case EN_WSS_NIS_CMD_ARRAY_RD:
        usCntExist = 1;
        break;

    case EN_WSS_NIS_CMD_WR:
        usCntExist = 0;
        return WRead(usCntExist, paucBufR, stpRspHandler);

    case EN_WSS_NIS_CMD_ARRAY_WR:
        usCntExist = 1;
        return WRead(usCntExist, paucBufR, stpRspHandler);

    case EN_WSS_NIS_CMD_MULTI_WR:
        usCntExist = 1;
        return WRead(usCntExist, paucBufR, stpRspHandler);

    default:
        LOG_WRITE(EN_LOG_ERROR, "Wss VSPI Unkown Cmd");
        return false;
    }

    if (!readConfig()) {
        return false;
    }

    /*get len */
    /*uint32_t usLenW = stpRspHandler->Len(); */

    /*excute cmd */
    std::string Rcommand((char *)paucBufR, DEF_WSS_NIS_BUF_LEN);
    uint8_t *pucBuf = (this->*RCommandsMap.at (m_ucOid))(Rcommand, stpRspHandler);

    /*pack head */
    stVhead.ucMid = m_ucMid;
    stVhead.usLen = ntohs(sizeof(stVhead.ucRslt) + m_uslen + usCntExist + sizeof(usCrc));
    stVhead.ucRslt = EN_WSS_NIS_ERR_NONE;
    memcpy(paucBufR, &stVhead, sizeof(stVhead));

    LOG_WRITE(EN_LOG_NOTICE, "VWssSPI usLen: %d", stVhead.usLen);

    /*Success Count */
    if (1 == usCntExist) {
        uint8_t ucCnt = (uint8_t)m_sRhead[10];
        memcpy(paucBufR + sizeof(stVhead), &ucCnt, sizeof(ucCnt));
    }

    /*pack data */
    memcpy(paucBufR + sizeof(stVhead) + usCntExist, (char *)pucBuf, m_uslen);

    /*pack CRC */
    usCrc = ntohs(_WssNis_Crc16(paucBufR, m_uslen + usCntExist + sizeof(stVhead)));
    memcpy(paucBufR + m_uslen + sizeof(stVhead) + usCntExist, &usCrc, sizeof(usCrc));

#if 0
    for (int i = 0; i < htons(stVhead.usLen) + 5; i++) {
        printf("%dindex: ", i);
        printf("0x%2x\n", (int8_t)paucBufR[i]);
    }
#endif

    LOG_WRITE(EN_LOG_NOTICE, "VSPI End -- ");

    return true;
}

bool
CWssVSpi::WRead(uint32_t usCntExist, uint8_t *paucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*get len */
    /*uint32_t usLenW = stpRspHandler->Len(); */

    /*Head */
    ST_WSS_NIS_RESP_L_HEAD_CMN stVhead;
    memset(&stVhead, 0, sizeof(stVhead));

    /*CRC */
    uint16_t usCrc = 0;

    /*pack head */
    stVhead.ucMid = m_ucMid;
    stVhead.usLen = ntohs(sizeof(stVhead.ucRslt) + sizeof(usCrc));
    stVhead.ucRslt = EN_WSS_NIS_ERR_NONE;
    memcpy(paucBufR, &stVhead, sizeof(stVhead));

    /*pack CRC */

    usCrc = ntohs(_WssNis_Crc16(paucBufR, sizeof(stVhead)));
    memcpy(paucBufR + sizeof(stVhead), &usCrc, sizeof(usCrc));

    return true;
}

uint8_t *
CWssVSpi::GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetMfg ++");

    /*tmp buffer */
    static uint8_t GetmfgRsp[1024] = {0};
    memcpy(GetmfgRsp, &staMsg, sizeof(GetmfgRsp));
    char tmp[64] = {0};

    /*get mfg info */
    CStringTool::SubStr(tmp, mfg, "acManufacturer: ", "\n", MFG_INFO_LEN_MAX - 1);
    memcpy(GetmfgRsp, tmp, strlen(tmp));
    uint16_t uslen = strlen(tmp);
    memset(tmp, 0, sizeof(tmp));

    CStringTool::SubStr(tmp, mfg, "Module PN: ", "\n", MFG_INFO_LEN_MAX - 1);
    memcpy(GetmfgRsp + uslen, tmp, strlen(tmp));
    uslen += strlen(tmp);
    memset(tmp, 0, sizeof(tmp));

    CStringTool::SubStr(tmp, mfg, "Module SN: ", "\n", MFG_INFO_LEN_MAX - 1);
    memcpy(GetmfgRsp + uslen, tmp, strlen(tmp));
    uslen += strlen(tmp);
    memset(tmp, 0, sizeof(tmp));

    CStringTool::SubStr(tmp, mfg, "FW version: ", "\n", MFG_INFO_LEN_MAX - 1);
    memcpy(GetmfgRsp + uslen, tmp, strlen(tmp));
    uslen += strlen(tmp);
    memset(tmp, 0, sizeof(tmp));

    CStringTool::SubStr(tmp, mfg, "HW version: ", "\n", MFG_INFO_LEN_MAX - 1);
    memcpy(GetmfgRsp + uslen, tmp, strlen(tmp) - 1);
    uslen += strlen(tmp) - 1;
    memset(tmp, 0, sizeof(tmp));

    /*end of string */
    memcpy(GetmfgRsp + uslen, "\0", 1);
    m_uslen = strlen((char *)GetmfgRsp);

    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetMfg --");

    return GetmfgRsp;
}

uint8_t *
CWssVSpi::GetChanCenter(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetChanCenter ++");

    uint16_t ausCentFre[WSS_CH_CNT_TOTAL] = {0};
    static uint8_t aucTmpbuf[WSS_CH_CNT_TOTAL * 2] = {0};
    uint8_t usinstance = (uint8_t)m_sRhead[10];
    uint16_t uclen = 0;
    uint16_t usBuff = 0;

    for (int16_t i = 0; i < usinstance; i++) {
        memset(&usBuff, 0, sizeof(usBuff));

        /*get data */
        usBuff = ntohs(config["WSS"]["Center_Freq"][i].asInt());
        memcpy(&ausCentFre[i], &usBuff, sizeof(usBuff));

        uclen += sizeof(usBuff);
    }

    m_uslen = uclen;
    memcpy(aucTmpbuf, ausCentFre, m_uslen);

    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetChanCenter --");

    return aucTmpbuf;
}

uint8_t *
CWssVSpi::GetBandwidth(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetBandwidth ++");

    uint16_t ausBandWith[WSS_CH_CNT_TOTAL] = {0};
    static uint8_t aucTmpbuf[WSS_CH_CNT_TOTAL * 2] = {0};
    uint8_t usinstance = (uint8_t)m_sRhead[10];
    uint16_t uclen = 0;
    uint16_t usBuff = 0;

    for (int i = 0; i < usinstance; i++) {
        memset(&usBuff, 0, sizeof(usBuff));

        /*get data */
        usBuff = ntohs(config["WSS"]["Bandwidth"][i].asInt());
        memcpy(&ausBandWith[i], &usBuff, sizeof(usBuff));

        uclen += sizeof(usBuff);
    }

    m_uslen = uclen;
    memcpy(aucTmpbuf, ausBandWith, m_uslen);

    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetBandwidth --");

    return aucTmpbuf;
}

uint8_t *
CWssVSpi::GetChPortAttn(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetChPortAttn ++");

    uint16_t ausChPortAttn[WSS_CH_CNT_TOTAL] = {0};
    static uint8_t aucTmpbuf[WSS_CH_CNT_TOTAL * 2] = {0};

    uint8_t usinstance = (uint8_t)m_sRhead[11];
    uint8_t tabl = (uint8_t)m_sRhead[8];
    uint16_t uclen = 0;
    ST_PORT_ATTN stBuff;

    /*add */
    if (2 == tabl) {
        for (int i = 0; i < usinstance; i++) {
            memset(&stBuff, 0, sizeof(stBuff));

            /*get data */
            stBuff.ucPort = config["WSS"]["Add"]["Port"][i].asInt();
            stBuff.ucAttn = config["WSS"]["Add"]["Attn"][i].asInt();
            memcpy(&ausChPortAttn[i], &stBuff, sizeof(stBuff));

            uclen += sizeof(stBuff);
        }
    }
    /*drop */
    else if (1 == tabl) {
        for (int i = 0; i < usinstance; i++) {
            memset(&stBuff, 0, sizeof(stBuff));

            /*get data */
            stBuff.ucPort = config["WSS"]["Drop"]["Port"][i].asInt();
            stBuff.ucAttn = config["WSS"]["Drop"]["Attn"][i].asInt();
            memcpy(&ausChPortAttn[i], &stBuff, sizeof(stBuff));

            uclen += sizeof(stBuff);
        }
    } else {
        return (uint8_t *)"";
    }

    m_uslen = uclen;
    memcpy(aucTmpbuf, ausChPortAttn, m_uslen);

    LOG_WRITE(EN_LOG_NOTICE, "VSPI GetChPortAttn --");
    return aucTmpbuf;
}

uint8_t *
CWssVSpi::SetChCout(const std::string& staMsg)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI SetChCout ++");

    uint8_t usChmax = 0;
    usChmax = (uint8_t)staMsg[10];

    /*set data */
    config["WSS"]["channelMax"] = usChmax;

    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSPI SetChCout --");

    return (uint8_t *)"";
}


uint8_t *
CWssVSpi::SetChPortAttn(const std::string& staMsg)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSPI SetChPortAttn ++");

    uint8_t usinstance = (uint8_t)staMsg[11];
    uint8_t ausChPortAttn[WSS_CH_CNT_TOTAL * 2] = {0};
    memcpy(ausChPortAttn, &staMsg[12], usinstance * 2);

    /*tmp buf */
    uint8_t usBuffP = 0;
    uint8_t usBuffA = 0;

    /*tbl add or drop */
    uint8_t usCSubid = 0;
    usCSubid = (uint8_t)staMsg[8];

#if 0
    for (int i = 0; i < (int)(usinstance * 2); i++) {
        std::cout << "index" << i;
        printf(": 0x%02x\n", (uint8_t)ausChPortAttn[i]);
    }
#endif

    /*add */
    if (2 == usCSubid) {
        for (int i = 0; i < (usinstance * 2); i += 2) {
            memcpy(&usBuffP, &ausChPortAttn[i], sizeof(usBuffP));
            memcpy(&usBuffA, &ausChPortAttn[i + 1], sizeof(usBuffP));

            /*set data */
            config["WSS"]["Add"]["Port"][i / 2 + 1] = ausChPortAttn[i];
            config["WSS"]["Add"]["Attn"][i / 2 + 1] = ausChPortAttn[i + 1];
            memset(&usBuffP, 0, sizeof(usBuffP));
            memset(&usBuffA, 0, sizeof(usBuffA));
        }
    }
    /*drop */
    else if (1 == usCSubid) {
        for (int i = 0; i < usinstance * 2; i += 2) {
            memcpy(&usBuffP, &ausChPortAttn[i], sizeof(usBuffP));
            memcpy(&usBuffA, &ausChPortAttn[i + 1], sizeof(usBuffP));

            /*set data */
            config["WSS"]["Drop"]["Port"][i / 2 + 1] = ausChPortAttn[i];
            config["WSS"]["Drop"]["Attn"][i / 2 + 1] = ausChPortAttn[i + 1];
            memset(&usBuffP, 0, sizeof(usBuffP));
            memset(&usBuffA, 0, sizeof(usBuffA));
        }
    } else {
        return (uint8_t *)"";
    }

    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSPI SetChPortAttn --");

    return (uint8_t *)"";
}


uint8_t *
CWssVSpi::SetChanCenter(const std::string& staMsg)
{
    uint16_t ucChCnt = 0;
    uint16_t usData = 0;

    /*set data */
    memcpy(&ucChCnt, &staMsg[0], sizeof(ucChCnt));
    memcpy(&usData, &staMsg[sizeof(ucChCnt)], sizeof(usData));
    config["WSS"]["Center_Freq"][ucChCnt] = usData;

    writeConfig();

    return (uint8_t *)true;
}

uint8_t *
CWssVSpi::SetBandwidth(const std::string& staMsg)
{
    uint16_t ucChCnt = 0;
    uint16_t usData = 0;

    /*set data */
    memcpy(&ucChCnt, &staMsg[0], sizeof(ucChCnt));
    memcpy(&usData, &staMsg[sizeof(ucChCnt)], sizeof(usData));
    config["WSS"]["Bandwidth"][ucChCnt] = usData;

    writeConfig();

    return (uint8_t *)true;
}