/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      VSpiDriver.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:



*****************************************************************************************************/

/*=========================include head files====================*/
#include "VOcmSpiDriver.h"
#include "VBoardDriver.h"
#define LEN_192                   (192)


/*==============================function=========================*/
bool
COcmVSpi::ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                    uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    std::string command((char *)pucBufW, uiLenW);
    /* omitting TID, length, port, and CRC */
    uint8_t comm = pucBufW[5];
    /* LOG_WRITE(EN_LOG_NOTICE, "VOcmSpi comm: 0x%02x", comm); */
    if (CommandsMap.find(comm) == CommandsMap.end()) {
        /* LOG_WRITE(EN_LOG_NOTICE, "VOcmSpi comm: 0x%02x", comm); */
        LOG_WRITE(EN_LOG_ERROR, "VSPI received unrecognized command!");
        return false;
    }

    if (!readConfig()) {
        return false;
    }

    /* The return value is not checked at present */
    (this->*CommandsMap.at(comm))(command, stpRspHandler);
    /* ? does it need to check whether what's in use, pucBufR or stpRspHandler */

    /* SPI driver processes the header and CRC, which means VSPI can and should skip them */

    return true;
}

uint8_t *
COcmVSpi::GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_DEBUG, "COcmVSpi::GetMfg +++");
    /* std::string new_main_version = config["OCM"]["SWversion"].asString() + "\n"; */
    /* LOG_WRITE(EN_LOG_NOTICE, "new_main_version: %s", config["OCM"]["SWversion"].asCString()); */

    std::size_t pos = mfg.find("Main version:");
    if (pos != std::string::npos) {
        std::size_t end_pos = mfg.find("\n", pos);
        if (end_pos != std::string::npos) {
            std::string new_version = "Main version: " + config["OCM"]["SWversion"].asString() + "\n";
            mfg.replace(pos, end_pos - pos + 1, new_version);
        }
    }
    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, mfg.data(), mfg.length());
    LOG_WRITE(EN_LOG_DEBUG, "COcmVSpi::GetMfg ---");
    return (uint8_t *)"";

}

uint8_t *
COcmVSpi::GetChPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COcmChPower channelPowers;
    memset(&channelPowers, 0, sizeof(COcmChPower));
    /* TODO len follow SetWavePlan */
    for (uint i = 0; i < (LEN_192 / sizeof(int16_t)); i++) {
        channelPowers.asPower[i] = (((double)rand() / (RAND_MAX)) - 0.375) * 40 + 6000;
    } /* normal power value varies between -15 to 25 */
    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &channelPowers, sizeof(COcmChPower));
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::GetOsa(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COcmOsa slicePowers;
    /* int isliceType = staMsg[6]; / * hard-coded to be 4 in MasterSpiProto * / */
    memset(&slicePowers, 0, sizeof(COcmOsa));
    for (int i = 0; i < OCM_SLICE_MAX_NUM; i++) {  /* see details in spec */
        slicePowers.asOsa[i] = (((double)rand() / (RAND_MAX)) - 0.375) * 40 + 6000;
    } /* normal power value varies between -15 to 25 */
    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &slicePowers, sizeof(COcmOsa));
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::SetWavePlan(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /* Observe need of input validation in future */
    config["OCM"]["sliceStart"].clear();
    config["OCM"]["sliceEnd"].clear();

    for (uint32_t i = 6; i < staMsg.length() - 2; i += 5) {
        /* each number is two bytes in big endian */
        config["OCM"]["sliceStart"].append(ntohs((uint8_t(staMsg[i + 2]) << 8) + uint8_t(staMsg[i + 1])));
        config["OCM"]["sliceEnd"].append(ntohs((uint8_t(staMsg[i + 4]) << 8) + uint8_t(staMsg[i + 3])));
#if 0
        /*show every bytes */
        LOG_WRITE(EN_LOG_ERROR, "VSPI OCM    0x%02x", uint8_t(staMsg[i]));
        LOG_WRITE(EN_LOG_ERROR, "VSPI OCM    0x%02x", uint8_t(staMsg[i + 1]));
        LOG_WRITE(EN_LOG_ERROR, "VSPI OCM    0x%02x", uint8_t(staMsg[i + 2]));
        LOG_WRITE(EN_LOG_ERROR, "VSPI OCM    0x%02x", uint8_t(staMsg[i + 3]));
        LOG_WRITE(EN_LOG_ERROR, "VSPI OCM    0x%02x", uint8_t(staMsg[i + 4]));
        printf("\r\n");
#endif
    }
    writeConfig();
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::GetTemp(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t *pucBuf = stpRspHandler->Data();
    int16_t stemp = 0;
    /* TODO temp value varies between 30 to 45 */
    stemp = (((double)rand() / (RAND_MAX)) + 2) * 15;
    stemp = htons(stemp);
    memcpy(pucBuf, &stemp, sizeof(stemp));
    return (uint8_t *)"";
}

/* reset */
uint8_t *
COcmVSpi::Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_TRACE, "VSpiSpiDriver::Reset");
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmVSpi::Upgrade +++");
    ST_UPGD_BDL_HEAD *pstBdl = NULL;
    ST_UPGD_IMG_HEAD *pstImgHead = NULL;
    char pcFileName[128] = "/tmp/download/FirmwareBundle";
    char version[128] = {0};

    char acBdlName[256] = {0};
    uint32_t ulBdlLen = 0;

    memset(acBdlName, 0, sizeof(acBdlName));
    if (NULL == fopen(pcFileName, "r")) {
        LOG_WRITE(EN_LOG_ERROR, "CBoardVirtual::GetUpgInfo fopen is error");
        goto cleanup;
    }

    pstBdl = (ST_UPGD_BDL_HEAD *)UTIL_MallocFromFile(pcFileName, &ulBdlLen);

    for (int j = 0; j < pstBdl->ucImgCnt; j++) {
        pstImgHead = &pstBdl->astImgHead[j];

        /* There is no more valid image */
        if (0 == pstImgHead->ucModType) {
            break;
        }
        _VER_BCD_TO_XYZ_(ntohl(pstImgHead->ulImgVer), version);

        if (pstImgHead->ucModType == 17) {
            LOG_WRITE(EN_LOG_DEBUG, "version of OCM: %s", version);
            config["OCM"]["SWversion"] = version;
        }
        memset(version, 0, sizeof(version));
    }

    writeConfig();

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "COcmVSpi::Upgrade ---");
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::GetUpgadeStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    return (uint8_t *)"";
}

uint8_t *
COcmVSpi::Swap(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    return (uint8_t *)"";
}