/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      VSpiDriver.cp     Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:



*****************************************************************************************************/

/*=========================include head files====================*/
#include "VOaSpiDriver.h"
#include "StringTool.h"
#include "OaComm.h"

#define DEF_RW_START    (5)
#define DEF_MID_START   (8)
#define DEF_DID_START   (10)
#define DEF_DATA_START  (16)

/*==============================function=========================*/
bool
COaVSpi::ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                   uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    std::string command((char *)pucBufW, uiLenW);
    /*pucBufW TID, Length, Status, R/W, OID, MID, DID, Instance, address, Length, Data, CRC */
    /*bytes    2     2        1     1    2    2    1      1         2        2      M    2 */
    uint8_t comm = pucBufW[DEF_DID_START];
    LOG_WRITE(EN_LOG_DEBUG, "VOaSpi comm: 0x%02x", comm);
    if (CommandsMap.find(comm) == CommandsMap.end()) {
        /*LOG_WRITE(EN_LOG_ERROR, "VSpi received unrecognized command!");*/
        LOG_WRITE(EN_LOG_NOTICE, "VOaSpi comm: 0x%02x", comm);
        return false;
    }

    if (!readConfig()) {
        return false;
    }

    /* The return value is not checked at present */
    (this->*CommandsMap.at(comm))(command, stpRspHandler);
    /* ? does it need to check whether what's in use, pucBufR or stpRspHandler */

    /* SPI driver processes the header and CRC, which means VSpi can and should skip them */

    return true;
}

uint8_t *
COaVSpi::GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetMfg +++");
    ST_MODULE_INFO stOaModuleInfo;
    CStringTool::SubStr(stOaModuleInfo.acModuleName, mfg, "Configuration: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucFwMainVer, mfg, "Firmware Vers: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucFwBootVer, mfg, "IPL version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucHwVer, mfg, "HW version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucBundleVer, mfg, "SW version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucSerialNum, mfg, "Serial Number: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucFpgaVer, mfg, "PLD version: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucPartNum, mfg, "Part Number: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucMfgDate, mfg, "MAN_DATE: ", "\n", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOaModuleInfo.aucCaliDate, mfg, "CAL_DATE: ", "\n", MFG_INFO_LEN_MAX - 1);

    /* LOG_WRITE(EN_LOG_NOTICE, "config[OA][0][Swversion]: %s", config["OA"][0]["SWversion"].asString()); */
    if (config["OA"][0].isMember("SWversion")) {
        LOG_WRITE(EN_LOG_NOTICE, "config[OA][0][Swversion]: %s", config["OA"][0]["SWversion"].asCString());
        strcpy(stOaModuleInfo.aucBundleVer, config["OA"][0]["SWversion"].asCString());
    }

#if 0
    std::cout << stOaModuleInfo.acModuleName << std::endl;
    std::cout << stOaModuleInfo.aucFwMainVer << std::endl;
    std::cout << stOaModuleInfo.aucFwBootVer << std::endl;
    std::cout << stOaModuleInfo.aucHwVer << std::endl;
    std::cout << stOaModuleInfo.aucBundleVer << std::endl;
    std::cout << stOaModuleInfo.aucSerialNum << std::endl;
    std::cout << stOaModuleInfo.aucFpgaVer << std::endl;
    std::cout << stOaModuleInfo.aucPartNum << std::endl;
    std::cout << stOaModuleInfo.aucMfgDate << std::endl;
    std::cout << stOaModuleInfo.aucCaliDate << std::endl;
    std::cout << stOaModuleInfo.acModuleName << std::endl;
#endif

    uint8_t *pucBuf = stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy(pucBuf, &stOaModuleInfo, sizeof(ST_MODULE_INFO));

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetMfg ---");

    return (uint8_t *)"";
}


uint8_t *
COaVSpi::GetStatusData(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_DEBUG, "VSpi OA GetStatusData +++");

    ST_EDFA_OPER stOaOper;

    for (uint32_t i = 0; i < 2; i++) {
        /* uint32_t uiSubIdx = i; */
        uint32_t k = 0;
        if (i == 0) {
            k = 0;
        } else if (i == 1) {
            k = 1;
        }

        uint32_t index = i < config["OA"].size() ? i : 0;

        stOaOper.astEdfaCfg[k].uiGainRangeMode = config["OA"][index]["Config"]["GainRange"].asInt();
        stOaOper.astEdfaCfg[k].uiCtrlMode = config["OA"][index]["Config"]["Mode"].asInt();
        stOaOper.astEdfaCfg[k].uiGain = config["OA"][index]["Config"]["Gain"].asInt();
        stOaOper.astEdfaCfg[k].iGainTilt = config["OA"][index]["Config"]["Tilt"].asInt();
        stOaOper.astEdfaCfg[k].iOutPwr = config["OA"][index]["Config"]["OutPwr"].asInt();

        stOaOper.auiPortStatusCfg[k] = config["OA"][index]["Config"]["PortStatusCfg"].asInt();

        if (config["OA"][index]["Config"]["PumpEnable"].asString() == "off") {
            stOaOper.astSafetyCfg[k].uiOsriCtrlMode = 1;
        } else {
            stOaOper.astSafetyCfg[k].uiOsriCtrlMode = 0;
        }

        stOaOper.astSafetyCfg[k].uiCtrlMode = config["OA"][index]["Config"]["SafeMode"].asInt();
        stOaOper.astSafetyCfg[k].uiAprPwr = config["OA"][index]["Config"]["AprPwr"].asInt();
        stOaOper.astSafetyCfg[k].uiForceAPR = config["OA"][index]["Config"]["ForceAPR"].asInt();
        stOaOper.astSafetyCfg[k].uiOverride = config["OA"][index]["Config"]["Override"].asInt();
        stOaOper.astSafetyCfg[k].uiOverTimer = config["OA"][index]["Config"]["OverTimer"].asInt();
        stOaOper.astSafetyCfg[k].uiRestartEn = config["OA"][index]["Config"]["RestartEn"].asInt();
        stOaOper.astSafetyCfg[k].uiBackREn = config["OA"][index]["Config"]["BackREn"].asInt();
        stOaOper.astSafetyCfg[k].uiALSAlarm = config["OA"][index]["Config"]["ALSAlarm"].asInt();
        stOaOper.astSafetyCfg[k].uiAlmCorrelationDis = config["OA"][index]["Config"]["AlmCorrelationDis"].asInt();

        stOaOper.astEdfaStatus[k].uiEdfaStatus = config["OA"][index]["Config"]["sEdfaStatus"].asInt();
        stOaOper.astEdfaStatus[k].iFiberTemp = config["OA"][index]["Config"]["sFiberTemp"].asInt();
        stOaOper.astEdfaStatus[k].aiLaserTemp[0] = config["OA"][index]["Config"]["sLaserTemp"][0].asInt();

        for (uint32_t j = 0; j < EN_EDFA_SUB_PUMP_MAX; j++) {
            stOaOper.astEdfaStatus[k].auiLaserCurr[j] = config["OA"][index]["Config"]["sLaserCurr"][j].asInt();
        }

        stOaOper.astEdfaStatus[k].auiBoosterLaserPwr[0] = config["OA"][index]["Config"]["sBoosterLaserPwr"].asInt();

        /* set actual gain to config value */
        stOaOper.astEdfaStatus[k].iActualGain = config["OA"][index]["Config"]["Gain"].asInt();
        /*stOaOper.astEdfaStatus.iRsv[] = */

        stOaOper.astVoaStatus[i].iAtten = config["VOA"]["Config"]["Attn"][i].asInt();

        stOaOper.stWbaStatus.uiStatus = config["OA"][index]["Config"]["wbaStatus"].asInt();
        stOaOper.stWbaStatus.uiDGE6NoiseLoaderOnOff = config["OA"][index]["Config"]["PDPwuiDGE6NoiseLoaderOnOffr"].asInt();

        stOaOper.astSysStatus[k].uiRstType = config["OA"][index]["Config"]["RstType"].asInt();

        stOaOper.stNoiseLoader.ulNoiseLoader = config["OA"][index]["Config"]["NoiseLoader"].asInt();
        /*stOaOper.stNoiseLoader.aucResv[] = */

        stOaOper.auiLimitedPwrMode[0] = config["OA"][index]["Config"]["LimitedPwrMode"][0].asInt();
        /*stOaOper.aucResv[] = */

        stOaOper.uiOscAddPower = config["OSC"]["AddPower"].asInt();
        stOaOper.uiOscDropPower = config["OSC"]["DropPower"].asInt();
        stOaOper.uiDFBOnOff = config["DFB"]["OnOff"].asInt();
        stOaOper.uiOscOnOff = config["OSC"]["OnOff"].asInt();
    }
#ifdef __OLSP__
    for (uint32_t j = 0; j < EDFA_PD_MAX_NUM; j++) {
        uint32_t index = j < config["PD"]["PDPwr"].size() ? j : 0;
        if (index == 1 || index == 2) {
            /* BA */
            stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][0].asInt() + config["OA"][1]["Config"]["Gain"].asInt();
            continue;
        }
        if (index == 6 || index == 7) {
            /* PA */
            stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][5].asInt() + config["OA"][0]["Config"]["Gain"].asInt();
            continue;
        }
        stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][index].asInt();
    }
#endif

#if defined __HCF__ || defined __OCS__
    for (uint32_t j = 0; j < EDFA_PD_MAX_NUM; j++) {
        uint32_t index = j < config["PD"]["PDPwr"].size() ? j : 0;
        if (index == 1 || index == 2) {
            /* BA */
            stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][0].asInt() + config["OA"][0]["Config"]["Gain"].asInt();
            continue;
        }
        if (index == 6 || index == 7) {
            /* PA */
            stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][5].asInt() + config["OA"][1]["Config"]["Gain"].asInt();
            continue;
        }
        stOaOper.aiPDPwr[j] = config["PD"]["PDPwr"][index].asInt();
    }
#endif

    for (uint32_t k = 0; k < EN_EDFA_VOA_MAX; k++ ) {
        uint32_t index = k < config["VOA"]["Config"]["Attn"].size() ? k : 0;
        stOaOper.astVoaStatus[k].iAtten = config["VOA"]["Config"]["Attn"][index].asInt();
    }
    uint8_t *pucBuf = stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy(pucBuf, &stOaOper, sizeof(ST_EDFA_OPER));

    LOG_WRITE(EN_LOG_DEBUG, "VSpi OA GetStatusData ---");

    return (uint8_t *)"";
}

/* OA */
uint8_t *
COaVSpi::SetGainRange(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetGainRange +++");

    /* GainRange value and uild(MID) */
    uint32_t uiGainRange = 0;
    memcpy(&uiGainRange, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiGainRange));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, GainRange = %d", uiID, uiGainRange);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write GainRange value */
    config["OA"][uiID]["Config"]["GainRange"] = uiGainRange;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetGainRange ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetGain(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetGain +++");

    /* Gain value and uild(MID) */
    uint32_t uiGain = 0;
    memcpy(&uiGain, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiGain));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, Gain = %d", uiID, uiGain);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write Gain value */
    config["OA"][uiID]["Config"]["Gain"] = uiGain;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetGain ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetTilt(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     int32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetTilt +++");

    /* Tilt value and uild(MID) */
    int32_t iTilt = 0;
    memcpy(&iTilt, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(iTilt));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);
    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, Tilt = %d", uiID, iTilt);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write Tilt value */
    config["OA"][uiID]["Config"]["Tilt"] = iTilt;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetTilt ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetMode +++");

    /* Mode value and uild(MID) */
    uint32_t uiMode = 0;
    memcpy(&uiMode, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiMode));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, Mode = %d", uiID, uiMode);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write Mode value */
    config["OA"][uiID]["Config"]["Mode"] = uiMode;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetMode ---");

    return (uint8_t *)"";
}

#if 0
uint8_t *
COaVSpi::SetRxLosThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint16_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosThr +++");

    /* RxLosThr value and uild(MID) */
    uint16_t sRxLosThr = 0;
    memcpy(&sRxLosThr, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(sRxLosThr));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, data = %d", uiID, sRxLosThr);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write RxLosThr value */
    config["OA"][uiID]["Config"]["RxLosThr"] = sRxLosThr;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosThr ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetRxLosHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint16_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosHys +++");

    /* RxLosHys value and uild(MID) */
    uint16_t sRxLosHys = 0;
    memcpy(&sRxLosHys, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(sRxLosHys));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, data = %d", uiID, sRxLosHys);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write RxLosHys value */
    config["OA"][uiID]["Config"]["RxLosHys"] = sRxLosHys;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosHys ---");

    return (uint8_t *)"";
}
#endif

uint8_t *
COaVSpi::SetRxLosThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head       Data        CRC */
    /*bytes   16    CMN_THR_HYS_ST   2  */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosThrHys +++");

    /* RxLosThr value and uild(MID) */
    CMN_THR_HYS_ST stCmnThrHys;
    memcpy(&stCmnThrHys, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(CMN_THR_HYS_ST));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, OaRxLosThr = %d, OaRxLosHys = %d", uiID, stCmnThrHys.stCmnThr.iDegLow, stCmnThrHys.stCmnHys.iDegLow);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write RxLosThr and RxLosHys value */
    config["OA"][uiID]["Config"]["RxLosThr"] = (int)(stCmnThrHys.stCmnThr.iDegLow);
    config["OA"][uiID]["Config"]["RxLosHys"] = (int)(stCmnThrHys.stCmnHys.iDegLow);
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetRxLosThrHys ---");

    return (uint8_t *)"";
}


uint8_t *
COaVSpi::SetPumpEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head       Data     CRC */
    /*bytes   16      uint32_t     2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetPumpEnable +++");

    /* PumpEnable value and uild(MID) */
    uint32_t uiPumpEnable = 0;
    memcpy(&uiPumpEnable, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiPumpEnable));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, PumpEnable = %d", uiID, uiPumpEnable);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write PumpEnable value */
    switch (uiPumpEnable) {
    case 0:
        config["OA"][uiID]["Config"]["PumpEnable"] = "off";
        break;
    default:
        config["OA"][uiID]["Config"]["PumpEnable"] = "on";
        break;
    }
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetPumpEnable ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetAprEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint8_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAprEnable +++");

    /* AprEnable value and uild(MID) */
    uint8_t ucAprEnable = 0;
    memcpy(&ucAprEnable, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(ucAprEnable));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, AprEnable = %d", uiID, (int)ucAprEnable);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write AprEnable value  */
    switch (ucAprEnable) {
    case 0:
        config["OA"][uiID]["Config"]["AprEnable"] = "off";
        break;
    default:
        config["OA"][uiID]["Config"]["AprEnable"] = "on";
        break;
    }
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAprEnable ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetAutolosEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint8_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAutolosEnable +++");

    /* AutolosEnable value and uild(MID) */
    uint8_t ucAutolosEnable = 0;
    memcpy(&ucAutolosEnable, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(ucAutolosEnable));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, AutolosEnable = %d", uiID, ucAutolosEnable);

    if (uiID >= config["OA"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, out of range.", uiID);
        return NULL;
    }

    /* write AutolosEnable value  */
    switch (ucAutolosEnable) {
    case 0:
        config["OA"][uiID]["Config"]["AutolosEnable"] = "off";
        break;
    default:
        config["OA"][uiID]["Config"]["AutolosEnable"] = "on";
        break;
    }
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAutolosEnable ---");

    return (uint8_t *)"";
}


uint8_t *
COaVSpi::GetAlarmData(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_DEBUG, "VSpi OA GetAlarmData +++");
    ST_EDFA_ALARM1 stEdfaAlarm;
    memset(&stEdfaAlarm, 0, sizeof(ST_EDFA_ALARM1) - sizeof(ST_EDFA_RESERVE_ALM));


    stEdfaAlarm.stPdLosAlm.aucPdLosAlarm[0] = config["OA"][0]["Alarm"]["PA_INPUTLOS"].asInt();
    if (config["OA"].size() > 1) {
        stEdfaAlarm.stPdLosAlm.aucPdLosAlarm[2] = config["OA"][1]["Alarm"]["PA_INPUTLOS"].asInt();
    } else {
        stEdfaAlarm.stPdLosAlm.aucPdLosAlarm[2] = stEdfaAlarm.stPdLosAlm.aucPdLosAlarm[0];
    }

    uint8_t *pucBuf = stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy(pucBuf, &stEdfaAlarm, sizeof(ST_EDFA_ALARM1) - sizeof(ST_EDFA_RESERVE_ALM));

    LOG_WRITE(EN_LOG_DEBUG, "VSpi OA GetAlarmData ---");

    return (uint8_t *)"";
}

/* DFB */
uint8_t *
COaVSpi::SetDfbEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetDfbEnable +++");

    /* DfbEnable value and uild(MID) */
    uint32_t uiDfbEnable = 0;
    memcpy(&uiDfbEnable, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiDfbEnable));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, DfbEnable = %d", uiID, uiDfbEnable);

    /* write PumpEnable value */
    config["DFB"]["OnOff"] = uiDfbEnable;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetDfbEnable ---");

    return (uint8_t *)"";
}

/* OSC */
uint8_t *
COaVSpi::SetOscEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscEnable +++");

    /* OscEnable value and uild(MID) */
    uint32_t uiOscEnable = 0;
    memcpy(&uiOscEnable, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiOscEnable));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, OscEnable = %d", uiID, uiOscEnable);

    /* write PumpEnable value */
    config["OSC"]["OnOff"] = uiOscEnable;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscEnable ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetOscAddThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint16_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscAddThrHys +++");

    /* ThrHys value and uild(MID) */
    CMN_THR_HYS_ST stOscCmnThrHys;
    memcpy(&stOscCmnThrHys, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(CMN_THR_HYS_ST));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, AddLosThr = %d, AddLosHys = %d", uiID, stOscCmnThrHys.stCmnThr.iDegLow, stOscCmnThrHys.stCmnHys.iDegLow);

    /* write AddLosThr and AddLosHys value */
    config["OSC"]["Add"]["Thr"] = (int)(stOscCmnThrHys.stCmnThr.iDegLow);
    config["OSC"]["Add"]["Hys"] = (int)(stOscCmnThrHys.stCmnHys.iDegLow);
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscAddThrHys ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetOscDropThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint16_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscDropThrHys +++");

    /* ThrHys value and uild(MID) */
    CMN_THR_HYS_ST stOscCmnThrHys;
    memcpy(&stOscCmnThrHys, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(CMN_THR_HYS_ST));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, DropLosThr = %d, DropLosHys = %d", uiID, stOscCmnThrHys.stCmnThr.iDegLow, stOscCmnThrHys.stCmnHys.iDegLow);

    /* write DropLosThr and DropLosHys value */
    config["OSC"]["Drop"]["Thr"] = (int)(stOscCmnThrHys.stCmnThr.iDegLow);
    config["OSC"]["Drop"]["Hys"] = (int)(stOscCmnThrHys.stCmnHys.iDegLow);
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscDropThrHys ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetOscRxThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16    uint16_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscRxThrHys +++");

    /* ThrHys value and uild(MID) */
    CMN_THR_HYS_ST stOscCmnThrHys;
    memcpy(&stOscCmnThrHys, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(CMN_THR_HYS_ST));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, OscRxLosThr = %d, OscRxLosHys = %d", uiID, stOscCmnThrHys.stCmnThr.iDegLow, stOscCmnThrHys.stCmnHys.iDegLow);

    /* write RxLosThr and RxLosHys value */
    config["OSC"]["Rx"]["Thr"] = (int)(stOscCmnThrHys.stCmnThr.iDegLow);
    config["OSC"]["Rx"]["Hys"] = (int)(stOscCmnThrHys.stCmnHys.iDegLow);
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOscRxThrHys ---");

    return (uint8_t *)"";
}

/* OTDR */
uint8_t *
COaVSpi::SetOtdrSwitchPos(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg Head     Data     CRC */
    /*bytes   16     uint32_t    2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOtdrSwitchPos +++");

    /* OscEnable value and uild(MID) */
    uint32_t uiOtdrPos = 0;
    memcpy(&uiOtdrPos, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiOtdrPos));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, Pos = %d", uiID, uiOtdrPos);

    /* write PumpEnable value */
    config["OTDR"]["Pos"] = uiOtdrPos;
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetOtdrSwitchPos ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::GetOtdrCali(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /* for OTDR scan */

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetOtdrCali +++");
    COtdrStartOffsetAll stOtdrCaliInfo;
    memset(&stOtdrCaliInfo, 0, sizeof(stOtdrCaliInfo));

    for (int i = 0; i < OLSP_OTDR_PORT_NUM; i++) {
        stOtdrCaliInfo.stOtdrStartOffsetPort[i].fLossOffset = 1.0;
        stOtdrCaliInfo.stOtdrStartOffsetPort[i].uiPositionOffset = 2.0;
    }

    stOtdrCaliInfo.ulCrc32 = 3922260722;
    uint8_t *pucBuf = stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy(pucBuf, &stOtdrCaliInfo, sizeof(COtdrStartOffsetAll));
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetOtdrCali ---");
    return (uint8_t *)"";
}

/* VOA */
uint8_t *
COaVSpi::SetAttn(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg  Head     Data     CRC */
    /*bytes    16      float     2 */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAttn +++");

    /* Attn value and uild(MID) */
    uint32_t uiAttn = 0;

    memcpy(&uiAttn, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(uiAttn));
    LOG_WRITE(EN_LOG_NOTICE, "Attn = %d", uiAttn);
    /* uiAttn = (uint32_t)(fData * 100); */

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    LOG_WRITE(EN_LOG_NOTICE, "uiLd = %d, Attn = %d", uiID, uiAttn);

    /* write Attn value */
#ifdef __OLSP__
    if (EN_EDFA_VOA_2 == uiID && EN_EDFA_VOA_5 < config["VOA"]["Config"]["Attn"].size()) {
        config["VOA"]["Config"]["Attn"][EN_EDFA_VOA_5] = uiAttn;
    } else if (EN_EDFA_VOA_1 == uiID && EN_EDFA_VOA_2 < config["VOA"]["Config"]["Attn"].size()) {
        config["VOA"]["Config"]["Attn"][EN_EDFA_VOA_2] = uiAttn;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, Attn = %d", uiID, uiAttn);
    }
#endif
#if defined __HCF__ || defined __OCS__
    if (EN_EDFA_VOA_1 == uiID && EN_EDFA_VOA_7 < config["VOA"]["Config"]["Attn"].size()) {
        config["VOA"]["Config"]["Attn"][EN_EDFA_VOA_7] = uiAttn;
    } else if (EN_EDFA_VOA_2 == uiID && EN_EDFA_VOA_3 < config["VOA"]["Config"]["Attn"].size()) {
        config["VOA"]["Config"]["Attn"][EN_EDFA_VOA_3] = uiAttn;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "uiLd = %d, Attn = %d", uiID, uiAttn);
    }
#endif
    writeConfig();

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetAttn ---");

    return (uint8_t *)"";
}

/* RESET */
uint8_t *
COaVSpi::Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA Reset +++");

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA Reset ---");

    return (uint8_t *)"";
}

uint8_t *
COaVSpi::Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::Upgrade +++");
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::Upgrade ---");
    return (uint8_t *)"";
}

uint8_t *
COaVSpi::GetUpgradeStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::GetUpgadeStatus +++");
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::GetUpgadeStatus ---");
    return (uint8_t *)"";
}

uint8_t *
COaVSpi::Swap(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::Swap +++");
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::Swap ---");
    return (uint8_t *)"";
}

uint8_t *
COaVSpi::SetUpgState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::SetUpgState +++");
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

        if (pstImgHead->ucModType == 15) {
            LOG_WRITE(EN_LOG_DEBUG, "version of EDFA: %s", version);
            config["OA"][0]["SWversion"] = version;
            config["OA"][1]["SWversion"] = version;
        }
        memset(version, 0, sizeof(version));
    }

    writeConfig();

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "COaVSpi::SetUpgState ---");
    return (uint8_t *)"";
}

/* VOA */
uint8_t *
COaVSpi::ShutterState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /*staMsg  Head     Data     CRC */
    /*bytes    16      float     2 */
    static uint32_t state = 0;
    uint8_t *data = (uint8_t *)staMsg.data();
    uint8_t ucRw = *(data + DEF_RW_START);

    if (ucRw == OA_SPI_CMD_WRITE) {
        state = *((uint32_t *)(data + DEF_DATA_START));
        LOG_WRITE(EN_LOG_NOTICE, "VSpi OA set shutter state %u", state);
    } else {
        *((uint32_t *)(stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY))) = state;
        LOG_WRITE(EN_LOG_NOTICE, "VSpi OA get shutter state %u", state);
    }

    return (uint8_t *)"";
}

/* OCM */
uint8_t *
COaVSpi::GetOcmCali(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    /* for OTDR scan */
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetOcmCali +++");
    COcmCali stOcmCaliInfo;
    memset(&stOcmCaliInfo, 0, sizeof(stOcmCaliInfo));
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA GetOcmCali ---");

    uint8_t *pucBuf = stpRspHandler->Data() + sizeof(ST_OA_SPI_MSG_BODY);
    memcpy(pucBuf, &stOcmCaliInfo, sizeof(COcmCali));

    return (uint8_t *)"";
}


#if defined __HCF__ || defined __OCS__
uint8_t *
COaVSpi::SetCurrents(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetCurrents +++");
    EN_PUMP_INFO pump_info;
    memcpy(&pump_info, (uint8_t *)staMsg.data() + DEF_DATA_START, sizeof(pump_info));

    uint16_t uiID = 0;
    memcpy(&uiID, (uint8_t *)staMsg.data() + DEF_MID_START, sizeof(uiID));
    uiID = ntohs(uiID);

    config["OA"][uiID]["Config"]["LaserCurr"][pump_info.uiPumpIdx] = pump_info.sCurrent;
    config["OA"][uiID]["Config"]["sLaserCurr"][pump_info.uiPumpIdx] = pump_info.sCurrent;

    writeConfig();
    LOG_WRITE(EN_LOG_NOTICE, "VSpi OA SetCurrents ---");

    return (uint8_t *)"";

}
#endif