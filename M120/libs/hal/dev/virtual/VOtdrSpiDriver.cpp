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
#include "VOtdrSpiDriver.h"


#define DEF_OTDR_DATA_START (6)
#define DEF_SOR_FILE_SIZE   (4000)
#define DEF_ACQ_DONE        (1)


/*==============================function=========================*/
bool
COtdrVSpi::ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                     uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
#if 0
    for (int i = 0; i < (int)10; i++) {
        std::cout << "index" << i;
        printf(": 0x%02x\n", (uint8_t)pucBufW[i]);
    }
#endif

    std::string command((char *)pucBufW, uiLenW);
    /*pucBufW TID, Length, Port, OID, Data, CRC */
    /*bytes    2      2      1    1     M    2  */
    uint8_t comm = pucBufW[5];
    /* LOG_WRITE(EN_LOG_NOTICE, "VOtdrSpi comm: 0x%02x", comm); */
    if (CommandsMap.find(comm) == CommandsMap.end()) {
        LOG_WRITE(EN_LOG_ERROR, "VSPI received unrecognized command!");
        return false;
    }

    if (!readConfig()) {
        return false;
    }

    /* The return value is not checked at present */
    (this->*CommandsMap.at (comm))(command, stpRspHandler);

    /* ? does it need to check whether what's in use, pucBufR or stpRspHandler */

    /* SPI driver processes the header and CRC, which means VSPI can and should skip them */

    return true;
}

uint8_t *
COtdrVSpi::GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_MOLEX_OTDR_MFG_INFO stOtdrMfg;
    memset(&stOtdrMfg, 0, sizeof(stOtdrMfg));

    CStringTool::SubStr(stOtdrMfg.acPN, mfg, "Part Number: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acSN, mfg, "Serial Number: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acHWVer, mfg, "HW version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acBundleVer, mfg, "Bundle version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acCalDate, mfg, "CAL_DATE: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acBootVer, mfg, "Boot version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOtdrMfg.acMfgDate, mfg, "Mfg date: ", "\n", MFG_INFO_LEN_MAX - 1);

#if 0
    std::cout << stOtdrMfg.acPN << std::endl;
    std::cout << stOtdrMfg.acSN << std::endl;
    std::cout << stOtdrMfg.acHWVer << std::endl;
    std::cout << stOtdrMfg.acBundleVer << std::endl;
    std::cout << stOtdrMfg.acCalDate << std::endl;
    std::cout << stOtdrMfg.acBootVer << std::endl;
    std::cout << stOtdrMfg.acMfgDate << std::endl;
#endif

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &stOtdrMfg, sizeof(ST_MOLEX_OTDR_MFG_INFO));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetMfg end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_MOLEX_OTDR_CONFIG stOtdrcfg;
    memset(&stOtdrcfg, 0, sizeof(stOtdrcfg));

    stOtdrcfg.ulScanCtrl = ntohl(config["OTDR"]["Scan control"].asInt());
    stOtdrcfg.ulScanMode = ntohl(config["OTDR"]["Scan mode"].asInt());
    stOtdrcfg.ulExpScanTime = ntohl(config["OTDR"]["Expert scan time"].asInt());
    stOtdrcfg.ulExpPulseWidth = ntohl(config["OTDR"]["Expert pulse width"].asInt());
    stOtdrcfg.ulScanStartPos = ntohl(config["OTDR"]["Expert Scan start position"].asInt());
    stOtdrcfg.ulScanEndPos = ntohl(config["OTDR"]["Expert Scan end position"].asInt());

    stOtdrcfg.fReflIdx = UTIL_Ntohf(float(config["OTDR"]["Refraction index"].asInt()) / 1000000.0);
    stOtdrcfg.fReflEventThr = UTIL_Ntohf(float(config["OTDR"]["Reflection event threshold"].asInt()) / 100.0);
    stOtdrcfg.fNonReflEventThr = UTIL_Ntohf(float(config["OTDR"]["Non-reflection event threshold"].asInt()) / 100.0);
    stOtdrcfg.fBackCoef = UTIL_Ntohf(float(config["OTDR"]["Backscatter coefficient"].asInt()) / 100.0);
    stOtdrcfg.fOrlThr = UTIL_Ntohf(float(config["OTDR"]["Orl threshold"].asInt()) / 100.0);
    stOtdrcfg.fInputPwrThr = UTIL_Ntohf(float(config["OTDR"]["Input power threshold"].asInt()) / 100.0);
    stOtdrcfg.fReflEventFhThr = UTIL_Ntohf(float(config["OTDR"]["Reflection threshold"].asInt()) / 100.0);
    stOtdrcfg.fRamanSetpont = UTIL_Ntohf(float(config["OTDR"]["Raman Setpoint"].asInt()) / 100.0);

#if 0
    std::cout << htonl(stOtdrcfg.ulScanCtrl) << std::endl;
    std::cout << htonl(stOtdrcfg.ulScanMode) << std::endl;
    std::cout << htonl(stOtdrcfg.ulExpScanTime) << std::endl;
    std::cout << htonl(stOtdrcfg.ulExpPulseWidth) << std::endl;
    std::cout << htonl(stOtdrcfg.ulScanStartPos) << std::endl;
    std::cout << htonl(stOtdrcfg.ulScanEndPos) << std::endl;

    std::cout << UTIL_Ntohf(stOtdrcfg.fReflIdx) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fReflEventThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fNonReflEventThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fBackCoef) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fOrlThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fInputPwrThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fReflEventFhThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrcfg.fRamanSetpont) << std::endl;
#endif

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &stOtdrcfg, sizeof(ST_MOLEX_OTDR_CONFIG));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetCfg end");

    return (uint8_t *)"";

}

uint8_t *
COtdrVSpi::GetAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint16_t usAlarm;
    memset(&usAlarm, 0, sizeof(usAlarm));

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &usAlarm, sizeof(usAlarm));
    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetAlarm end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetTemp(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_TEMP stTmep;
    memset(&stTmep, 0, sizeof(stTmep));

    /* temp value varies (between 30 to 45) * 100 */
    stTmep.sTecTemp = ntohs((((double)rand() / (RAND_MAX)) + 2) * 15 * 100);
    stTmep.sModuleTemp = ntohs((((double)rand() / (RAND_MAX)) + 2) * 15 * 100);
    stTmep.sFpgaTemp = ntohs((((double)rand() / (RAND_MAX)) + 2) * 15 * 100);

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &stTmep, sizeof(stTmep));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetTemp end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetVol(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint16_t usVol;
    memset(&usVol, 0, sizeof(usVol));

    /* PSU power voltage value varies between 240 to 100 */
    usVol = ntohs(((double)rand() / (RAND_MAX)) * 100 + 140);

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &usVol, sizeof(usVol));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetVol end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::SetCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_MOLEX_OTDR_CONFIG stOtdrCfg;
    memset(&stOtdrCfg, 0, sizeof(stOtdrCfg));
    memcpy(&stOtdrCfg, (uint8_t *)staMsg.data() + DEF_OTDR_DATA_START, sizeof(stOtdrCfg));

#if 0
    std::cout << htonl(stOtdrCfg.ulScanCtrl) << std::endl;
    std::cout << htonl(stOtdrCfg.ulScanMode) << std::endl;
    std::cout << htonl(stOtdrCfg.ulExpScanTime) << std::endl;
    std::cout << htonl(stOtdrCfg.ulExpPulseWidth) << std::endl;
    std::cout << htonl(stOtdrCfg.ulScanStartPos) << std::endl;
    std::cout << htonl(stOtdrCfg.ulScanEndPos) << std::endl;

    std::cout << UTIL_Ntohf(stOtdrCfg.fReflIdx) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fReflEventThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fNonReflEventThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fBackCoef) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fOrlThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fInputPwrThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fReflEventFhThr) << std::endl;
    std::cout << UTIL_Ntohf(stOtdrCfg.fRamanSetpont) << std::endl;
#endif

    config["OTDR"]["Scan control"] = htonl(stOtdrCfg.ulScanCtrl);
    config["OTDR"]["Scan mode"] = htonl(stOtdrCfg.ulScanMode);
    config["OTDR"]["Expert scan time"] = htonl(stOtdrCfg.ulExpScanTime);
    config["OTDR"]["Expert pulse width"] = htonl(stOtdrCfg.ulExpPulseWidth);
    config["OTDR"]["Expert Scan start position"] = htonl(stOtdrCfg.ulScanStartPos);
    config["OTDR"]["Expert Scan end position"] = htonl(stOtdrCfg.ulScanEndPos);

    config["OTDR"]["Refraction index"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fReflIdx) * 1000000);
    config["OTDR"]["Reflection event threshold"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fReflEventThr) * 100);
    config["OTDR"]["Non-reflection event threshold"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fNonReflEventThr) * 100);
    config["OTDR"]["Backscatter coefficient"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fBackCoef) * 100);
    config["OTDR"]["Orl threshold"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fOrlThr) * 100);
    config["OTDR"]["Input power threshold"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fInputPwrThr) * 100);
    config["OTDR"]["Reflection threshold"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fReflEventFhThr) * 100);
    config["OTDR"]["Raman Setpoint"] = (int32_t)(UTIL_Ntohf(stOtdrCfg.fRamanSetpont) * 100);

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR SetCfg end");
    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetEvent(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_EVENTS_INFO stEvent;
    memset(&stEvent, 0, sizeof(stEvent));

    stEvent.uiEventNum = 1;
    stEvent.staEventInfo[0].uiIdx = 1;
    stEvent.staEventInfo[0].fDistance = 50559;
    stEvent.staEventInfo[0].iAttenCoe = 0;
    stEvent.staEventInfo[0].iLoss = 1049;
    stEvent.staEventInfo[0].iReflectance = -1300;
    stEvent.staEventInfo[0].iCumulatedLoss = 1504;
    stEvent.staEventInfo[0].uiType = 1;

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &stEvent, sizeof(ST_EVENTS_INFO));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetEvent end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetSor(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /* get packid*/
    uint32_t uiPackId;
    memset(&uiPackId, 0, sizeof(uiPackId));
    memcpy(&uiPackId, (uint8_t *)staMsg.data() + DEF_OTDR_DATA_START, sizeof(uiPackId));

    char acSor[DEF_SOR_FILE_SIZE] = {0};
    uint32_t uiFileLength;
    memset(&acSor, 0, sizeof(acSor));

    /* open otdr sor file*/
    std::ifstream ifile;
    ifile.open(staOrdrSorFilename, std::ios::in);

    if (ifile.fail()) {
        perror("Failed to open sor file, exiting...");
        LOG_WRITE(EN_LOG_ERROR, "VOtdr SOR file open fail.");
        exit(1);
    }

    ifile.seekg(0, std::ios::end);
    uiFileLength = ifile.tellg();

    if ( uiFileLength >= DEF_SOR_FILE_SIZE * uiPackId ) {
        /* get file data*/
        ifile.getline(acSor + DEF_SOR_FILE_SIZE * (uiPackId - 1), sizeof(acSor));
        uiPackId += 1;
    } else {
        ifile.getline(acSor + DEF_SOR_FILE_SIZE * (uiPackId - 1), sizeof(acSor));
    }

    ifile.close();

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &uiPackId, sizeof(uiPackId));
    memcpy(pucBuf + sizeof(uiPackId), &acSor, sizeof(acSor));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetSor end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::GetState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_ACQ_STATUS stStatus;
    memset(&stStatus, 0, sizeof(stStatus));

    stStatus.ucStatus = DEF_ACQ_DONE;
    stStatus.ucPercent = 100;

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf, &stStatus, sizeof(ST_ACQ_STATUS));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OTDR GetState end");

    return (uint8_t *)"";
}

uint8_t *
COtdrVSpi::Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    return (uint8_t *)"";
}
