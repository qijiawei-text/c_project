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
#include "VOpsSpiDriver.h"
#include "VBoardDriver.h"

#define DEF_OPS_DATA_START (6)

/*==============================function=========================*/
bool
COpsVSpi::ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
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
    /* LOG_WRITE(EN_LOG_NOTICE, "VOpsSpi comm: 0x%02x", comm); */
    if (CommandsMap.find(comm) == CommandsMap.end()) {
        /* LOG_WRITE(EN_LOG_NOTICE, "VOpsSpi comm: 0x%02x", comm); */
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
COpsVSpi::GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_MFG_INFO stOpsMfg;
    memset(&stOpsMfg, 0, sizeof(stOpsMfg));
    CStringTool::SubStr(stOpsMfg.acMfgName, mfg, "Configuration: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsMfg.acPN, mfg, "Part Number: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsMfg.acSN, mfg, "Serial Number: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsMfg.acMfgDate, mfg, "Mfg date: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsMfg.acCalDate, mfg, "CAL date: ", "\r", MFG_INFO_LEN_MAX - 1);
    uint8_t *pucBuf = stpRspHandler->Data();

    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsMfg, sizeof(ST_MFG_INFO));
    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetMfg end ");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetVer(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_VER stOpsVer;
    memset(&stOpsVer, 0, sizeof(stOpsVer));
    CStringTool::SubStr(stOpsVer.acBootVer, mfg, "Boot version: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsVer.acBundleVer, mfg, "Bundle version: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsVer.acHwVer, mfg, "HW version: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsVer.acModType, mfg, "ModuleType: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsVer.acFpgaVer, mfg, "PLD version: ", "\r", MFG_INFO_LEN_MAX - 1);
    CStringTool::SubStr(stOpsVer.acMainVer, mfg, "Firmware Vers: ", "\r", MFG_INFO_LEN_MAX - 1);

    if (config["OPS"].isMember("SWversion")) {
        LOG_WRITE(EN_LOG_NOTICE, "config OPS Swversion: %s", config["OPS"]["SWversion"].asCString());
        strcpy(stOpsVer.acBundleVer, config["OPS"]["SWversion"].asCString());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsVer, sizeof(ST_VER));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetVer end");

    return (uint8_t *)"";
}

/* GetStatusData */
uint8_t *
COpsVSpi::GetPortPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_PORT_POWER stOpsPortPower;
    memset(&stOpsPortPower, 0, sizeof(stOpsPortPower));

    stOpsPortPower.asPortPower[0] = ntohs(config["OPS"]["PortPower"]["LINE1_IN"].asInt());
    stOpsPortPower.asPortPower[1] = ntohs(config["OPS"]["PortPower"]["LINE2_IN"].asInt());
    /* stOpsPortPower.asPortPower[2] = ntohs(config["OPS"]["PortPower"]["LINE1_OUT"].asInt()); */
    /* stOpsPortPower.asPortPower[3] = ntohs(config["OPS"]["PortPower"]["LINE2_OUT"].asInt()); */

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsPortPower, sizeof(ST_PORT_POWER));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetPortPower end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetWorkMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_WORK_MODE stOpsMode;
    memset(&stOpsMode, 0, sizeof(stOpsMode));

    stOpsMode.ucWorkMode = (uint8_t)(config["OPS"]["WorkMode"].asInt());
    stOpsMode.ucSwitchPos = (uint8_t)(config["OPS"]["SwitchPos"].asInt());
    stOpsMode.ucFswStatus = (uint8_t)(config["OPS"]["FswStatus"].asInt());

    if (stOpsMode.ucWorkMode == OPS_MODE_FORCE_P || stOpsMode.ucWorkMode == OPS_MODE_MAN_P) {
        stOpsMode.ucSwitchPos = OPS_PATH_SECOENDARY;
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsMode, sizeof(ST_WORK_MODE));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetWorkMode end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_ALARM stOpsAlarm;
    memset(&stOpsAlarm, 0, sizeof(ST_ALARM));

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsAlarm, sizeof(ST_ALARM));

    LOG_WRITE(EN_LOG_TRACE, "VSpi GetAlarm GetWorkMode end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetWssPortPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_WSS_PORT_POWER stWssCur;
    memset(&stWssCur, 0, sizeof(ST_WSS_PORT_POWER));

    for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
        uint32_t index = i < config["OPS"]["WssPd"].size() ? i : 0;
        stWssCur.asPortPower[i] = ntohs(config["OPS"]["WssPd"][index].asInt());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stWssCur, sizeof(ST_WSS_PORT_POWER));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetWssPortPower end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetWssAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_WSS_ALARM stWssAlrm;
    memset(&stWssAlrm, 0, sizeof(stWssAlrm));

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stWssAlrm, sizeof(stWssAlrm));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetWssAlarm end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetOscPd(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COscPd stOscPd;
    memset(&stOscPd, 0, sizeof(stOscPd));

    for (uint32_t i = 0; i < OSC_PD_CNT; i++) {
        uint32_t index = i < config["OPS"]["OSCPD"].size() ? i : 0;
        stOscPd.asPd[i] = ntohs(config["OPS"]["OSCPD"][index].asInt());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOscPd, sizeof(COscPd));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetOscPd end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetLedStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsLed stOpsLed;
    memset(&stOpsLed, 0, sizeof(stOpsLed));


    for (uint32_t i = 0; i < OPS_LED_CNT; i++) {
        uint32_t index = i < config["OPS"]["LedStatus"].size() ? i : 0;
        stOpsLed.aucStatus[i] = ntohs(config["OPS"]["LedStatus"][index].asInt());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsLed, sizeof(COpsLed));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetLedStatus end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetAlarmDuration(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_PORT_ALARM_DURATION stOpsAlarmDur;
    memset(&stOpsAlarmDur, 0, sizeof(stOpsAlarmDur));

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsAlarmDur, sizeof(ST_PORT_ALARM_DURATION));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetAlarmDuration end");

    return (uint8_t *)"";
}


/* GetCfgData */
uint8_t *
COpsVSpi::GetAllCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsCfgData stOpsCfg;
    memset(&stOpsCfg, 0, sizeof(stOpsCfg));

    stOpsCfg.usRelativeEnable = ntohs(config["OPS"]["RelativeEnable"].asInt());
    stOpsCfg.usHoldTime = ntohs(config["OPS"]["HoldTime"].asInt());
    stOpsCfg.usRelativeSwitchThr = ntohs(config["OPS"]["RelativeSwitchThr"].asInt());
    stOpsCfg.sRelativeOffset = ntohs(config["OPS"]["RelativeOffset"].asInt());
    stOpsCfg.uiWtrTime = ntohs(config["OPS"]["WtrTime"].asInt());

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsCfg, sizeof(stOpsCfg));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetAllCfg end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    CPortPowerThr stPortPowerThr;
    memset(&stPortPowerThr, 0, sizeof(stPortPowerThr));
    for (uint32_t i = 0; i < OPS_PORT_CNT; i++) {
        uint32_t index1 = i < config["OPS"]["PD"]["Thr"].size() ? i : 0;
        for (uint32_t j = 0; j < OPS_ALARM_TYPE_CNT; j++) {
            uint32_t index2 = i < config["OPS"]["PD"]["Thr"][index1].size() ? j : 0;
            stPortPowerThr.asThr[j][i] = ntohs(config["OPS"]["PD"]["Thr"][index1][index2].asInt());
        }
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stPortPowerThr, sizeof(CPortPowerThr));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetPortPowerThr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    CPortPowerHys stPortPowerHys;
    memset(&stPortPowerHys, 0, sizeof(stPortPowerHys));
    for (uint32_t i = 0; i < OPS_PORT_CNT; i++) {
        uint32_t index1 = i < config["OPS"]["PD"]["Hys"].size() ? i : 0;
        for (uint32_t j = 0; j < OPS_ALARM_TYPE_CNT; j++) {
            uint32_t index2 = i < config["OPS"]["PD"]["Hys"][index1].size() ? j : 0;
            stPortPowerHys.asHys[j][i] = ntohs(config["OPS"]["PD"]["Hys"][index1][index2].asInt());
        }
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stPortPowerHys, sizeof(CPortPowerHys));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetPortPowerHys end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetWssPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    CWssPortPowerThr stWssPortPowerThr;
    memset(&stWssPortPowerThr, 0, sizeof(stWssPortPowerThr));
    for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
        uint32_t index1 = i < config["OPS"]["WSSPD"]["Thr"].size() ? i : 0;
        for (uint32_t j = 0; j < OPS_ALARM_TYPE_CNT; j++) {
            uint32_t index2 = i < config["OPS"]["WSSPD"]["Thr"][index1].size() ? j : 0;
            stWssPortPowerThr.asThr[j][i] = ntohs(config["OPS"]["WSSPD"]["Thr"][index1][index2].asInt());
        }
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stWssPortPowerThr, sizeof(CWssPortPowerThr));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetWssPortPowerThr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetWssPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    CWssPortPowerHys stWssPortPowerHys;
    memset(&stWssPortPowerHys, 0, sizeof(stWssPortPowerHys));
    for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
        uint32_t index1 = i < config["OPS"]["WSSPD"]["Hys"].size() ? i : 0;
        for (uint32_t j = 0; j < OPS_ALARM_TYPE_CNT; j++) {
            uint32_t index2 = i < config["OPS"]["WSSPD"]["Hys"][index1].size() ? j : 0;
            stWssPortPowerHys.asHys[j][i] = ntohs(config["OPS"]["WSSPD"]["Hys"][index1][index2].asInt());
        }
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stWssPortPowerHys, sizeof(CWssPortPowerHys));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetWssPortPowerHys end");

    return (uint8_t *)"";
}


uint8_t *
COpsVSpi::GetSwitchThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsSwitchThr stOpsSwitchThr;
    memset(&stOpsSwitchThr, 0, sizeof(stOpsSwitchThr));

    for (uint32_t i = 0; i < OPS_PATH_CNT; i++) {
        uint32_t index = i < config["OPS"]["Path"]["Thr"].size() ? i : 0;
        stOpsSwitchThr.asThr[i] = ntohs(config["OPS"]["Path"]["Thr"][index].asInt());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsSwitchThr, sizeof(COpsSwitchThr));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetSwitchThr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetSwitchHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsSwitchHys stOpsSwitchHys;
    memset(&stOpsSwitchHys, 0, sizeof(stOpsSwitchHys));

    for (uint32_t i = 0; i < OPS_PATH_CNT; i++) {
        uint32_t index = i < config["OPS"]["Path"]["Hys"].size() ? i : 0;
        stOpsSwitchHys.asHys[i] = ntohs(config["OPS"]["Path"]["Hys"][index].asInt());
    }

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &stOpsSwitchHys, sizeof(COpsSwitchHys));

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS GetSwitchHys end");

    return (uint8_t *)"";
}


uint8_t *
COpsVSpi::SetPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_PORT_POWER_THR stPortPowerThr;
    memcpy(&stPortPowerThr, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ST_PORT_POWER_THR));

    uint32_t port = htons(stPortPowerThr.usPort);
    if (port >= config["OPS"]["PD"]["Thr"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetPortPowerThr, port out of range, port = %u", port);
        return NULL;
    }

    uint32_t type = htons(stPortPowerThr.usType);
    if (type >= config["OPS"]["PD"]["Thr"][port].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetPortPowerThr, type out of range, type = %u", type);
        return NULL;
    }

    config["OPS"]["PD"]["Thr"][port][type] = int16_t(htons(stPortPowerThr.sThr));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetPortPowerThr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_PORT_POWER_HYS stPortPowerHys;
    memcpy(&stPortPowerHys, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ST_PORT_POWER_HYS));

    uint32_t port = htons(stPortPowerHys.usPort);
    if (port >= config["OPS"]["PD"]["Hys"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetPortPowerHys, port out of range, port = %u", port);
        return NULL;
    }

    uint32_t type = htons(stPortPowerHys.usType);
    if (type >= config["OPS"]["PD"]["Hys"][port].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetPortPowerHys, type out of range, type = %u", type);
        return NULL;
    }


    config["OPS"]["PD"]["Hys"][port][type] = int16_t(htons(stPortPowerHys.sHys));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetPortPowerHys end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetSwitchThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_OPS_SWITCH_THR_SET stOpsSwitchThr;
    memcpy(&stOpsSwitchThr, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ST_OPS_SWITCH_THR_SET));

    uint32_t pos = htons(stOpsSwitchThr.usPos);
    if (pos >= config["OPS"]["Path"]["Thr"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetSwitchThr, pos out of range, pos = %u", pos);
        return NULL;
    }

    config["OPS"]["Path"]["Thr"][pos] = int16_t(htons(stOpsSwitchThr.sThr));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetSwitchThr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetSwitchHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    ST_OPS_SWITCH_HYS_SET stPortPowerHys;
    memcpy(&stPortPowerHys, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ST_PORT_POWER_HYS));

    uint32_t pos = htons(stPortPowerHys.usPos);
    if (pos >= config["OPS"]["Path"]["Hys"].size()) {
        LOG_WRITE(EN_LOG_ERROR, "VSpi OPS SetSwitchHys, pos out of range, pos = %u", pos);
        return NULL;
    }

    config["OPS"]["Path"]["Hys"][pos] = int16_t(htons(stPortPowerHys.sHys));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetSwitchHys end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t ucMode;
    memcpy(&ucMode, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ucMode));

    config["OPS"]["WorkMode"] = ucMode;

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetMode end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetWtr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint32_t uiWtr;
    memcpy(&uiWtr, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(uiWtr));

    config["OPS"]["WtrTime"] = htonl(uiWtr);

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetWtr end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetHoldOffTime(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint16_t usHoldOffTime;
    memcpy(&usHoldOffTime, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(usHoldOffTime));

    config["OPS"]["HoldTime"] = htons(usHoldOffTime);

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetHoldOffTime end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetRevertive(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint16_t usRevertive;
    memcpy(&usRevertive, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(usRevertive));

    config["OPS"]["RelativeEnable"] = htons(usRevertive);

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetRevertive end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetRelativeThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    int16_t sOffset;
    memcpy(&sOffset, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(sOffset));

    config["OPS"]["RelativeSwitchThr"] = int16_t(htons(sOffset));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetRelativeOffset end");

    return (uint8_t *)"";
}


uint8_t *
COpsVSpi::SetRelativeOffset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    int16_t sOffset;
    memcpy(&sOffset, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(sOffset));

    config["OPS"]["RelativeOffset"] = int16_t(htons(sOffset));

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetRelativeOffset end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsVSpi::Upgrade +++");
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

        if (pstImgHead->ucModType == 16) {
            LOG_WRITE(EN_LOG_DEBUG, "version of OPS: %s", version);
            config["OPS"]["SWversion"] = version;
        }
        memset(version, 0, sizeof(version));
    }

    writeConfig();

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "COpsVSpi::Upgrade ---");
    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetSwitchingType(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t ucType = (uint8_t)(config["OPS"]["SwitchingType"].asInt());

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &ucType, sizeof(ucType));

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OPS GetSwitchingType end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetSwitchingType(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint8_t ucType;
    memcpy(&ucType, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(ucType));

    config["OPS"]["SwitchingType"] = ucType;

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetSwitchingType end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::GetRequestStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsBidirectionStatus data;

    data.ucLocalWorkMode = (uint8_t)(config["OPS"]["WorkMode"].asInt());
    data.ucLocalPosition = (uint8_t)(config["OPS"]["SwitchPos"].asInt());
    data.ucLocalStatus = (uint8_t)(config["OPS"]["LocalState"].asInt());
    data.stRemoteStatus.ucProtTypeR = (uint8_t)(config["OPS"]["ProtTypeR"].asInt());
    data.stRemoteStatus.ucProtTypeD = (uint8_t)(config["OPS"]["ProtTypeD"].asInt());
    data.stRemoteStatus.ucProtTypeB = (uint8_t)(config["OPS"]["ProtTypeB"].asInt());
    data.stRemoteStatus.ucProtTypeA = (uint8_t)(config["OPS"]["ProtTypeA"].asInt());
    data.stRemoteStatus.ucApsState = (uint8_t)(config["OPS"]["RemoteState"].asInt());
    data.stRemoteStatus.ucRequestedSignal = (uint8_t)(config["OPS"]["RequestSignal"].asInt());
    data.stRemoteStatus.ucBridgedSignal = 1;
    data.stRemoteStatus.ucReserved = 0;

    uint8_t *pucBuf = stpRspHandler->Data();
    memcpy(pucBuf + sizeof(ST_FRM_READ_HEAD), &data, sizeof(data));

    LOG_WRITE(EN_LOG_NOTICE, "VSpi OPS GetRequestStatus end");

    return (uint8_t *)"";
}

uint8_t *
COpsVSpi::SetRemoteStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler)
{
    COpsReqStatus data;
    memcpy(&data, (uint8_t *)staMsg.data() + DEF_OPS_DATA_START, sizeof(data));

    config["OPS"]["ProtTypeR"] = data.ucProtTypeR;
    config["OPS"]["ProtTypeD"] = data.ucProtTypeD;
    config["OPS"]["ProtTypeB"] = data.ucProtTypeB;
    config["OPS"]["ProtTypeA"] = data.ucProtTypeA;
    config["OPS"]["RemoteState"] = data.ucApsState;
    config["OPS"]["RequestSignal"] = data.ucRequestedSignal;

    writeConfig();

    LOG_WRITE(EN_LOG_TRACE, "VSpi OPS SetRemoteStatus end");

    return (uint8_t *)"";
}