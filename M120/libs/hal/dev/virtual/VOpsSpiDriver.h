/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmMasterSpiDriver.h
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:


*****************************************************************************************************/

/*=========================include head files====================*/
#pragma once

#include "InterfaceDriver.h"
#include <fstream>
#include <jsoncpp/json/json.h>
/*#include <json/json.h> */
#include <unordered_map>
#include "DevData.h"
#include "StringTool.h"
#include "OpsProto.h"
#include "FrameProto.h"
#include "DevOpsData.h"

#define DEF_OPS_FILE_PATH "/var/vevice/SPIOpsConfig.json"
/*==============================function=========================*/
/*supports OPS at present */
class COpsVSpi : public CInterfaceDriver
{
public:
    COpsVSpi()
    {
    }

    virtual
    ~COpsVSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    uint8_t *GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetVer(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    /* get status */
    uint8_t *GetPortPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetWorkMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetWssPortPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetWssAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetOscPd(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetLedStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetAlarmDuration(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    /* get cfg data */
    uint8_t *GetAllCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetWssPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetWssPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetSwitchThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetSwitchHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    uint8_t *SetPortPowerThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetPortPowerHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetSwitchThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetSwitchHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetWtr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetHoldOffTime(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetRevertive(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetRelativeThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetRelativeOffset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    uint8_t *Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    /* Support bi-directional APS */
    uint8_t *GetSwitchingType(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetSwitchingType(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetRequestStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetRemoteStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

private:
    /* map the command code to corresponding commands of VSpi */
    typedef uint8_t * (COpsVSpi::*command)(const std::string&, const std::shared_ptr<CRspHandler> stpRspHandler);
    const std::unordered_map<int, command> CommandsMap = {
        {0x01, &COpsVSpi::GetMfg},
        {0x02, &COpsVSpi::GetVer},

        {0x24, &COpsVSpi::GetPortPower},
        {0x11, &COpsVSpi::GetWorkMode},
        {0x42, &COpsVSpi::GetAlarm},
        {0x27, &COpsVSpi::GetWssPortPower},
        {0x49, &COpsVSpi::GetWssAlarm},
        {0x30, &COpsVSpi::GetOscPd},
        {0x33, &COpsVSpi::GetLedStatus},
        {0x4E, &COpsVSpi::GetAlarmDuration},

        {0x17, &COpsVSpi::GetAllCfg},
        {0x21, &COpsVSpi::GetPortPowerThr},
        {0x23, &COpsVSpi::GetPortPowerHys},
        {0x25, &COpsVSpi::GetWssPortPowerThr},
        {0x26, &COpsVSpi::GetWssPortPowerHys},
        {0x4B, &COpsVSpi::GetSwitchThr},
        {0x4D, &COpsVSpi::GetSwitchHys},

        {0x51, &COpsVSpi::GetRequestStatus},
        {0x53, &COpsVSpi::GetSwitchingType},

        {0x20, &COpsVSpi::SetPortPowerThr},
        {0x22, &COpsVSpi::SetPortPowerHys},
        {0x4A, &COpsVSpi::SetSwitchThr},
        {0x4C, &COpsVSpi::SetSwitchHys},
        {0x10, &COpsVSpi::SetMode},
        {0x14, &COpsVSpi::SetWtr},
        {0x13, &COpsVSpi::SetHoldOffTime},
        {0x12, &COpsVSpi::SetRevertive},
        {0x43, &COpsVSpi::SetRelativeThr},
        {0x45, &COpsVSpi::SetRelativeOffset},
        {0x50, &COpsVSpi::SetRemoteStatus},
        {0x52, &COpsVSpi::SetSwitchingType},
        {0xF0, &COpsVSpi::Reset},
        {0xF1, &COpsVSpi::Upgrade},
    };

    bool
    readConfig()
    {
        std::ifstream config_doc(configFilename, std::ifstream::binary);
        if (config_doc.fail()) {
            LOG_WRITE(EN_LOG_ERROR, "VSpi config/status read/create fail.");
            return false;
        }
        config_doc >> config;
        return true;
    }

    bool
    writeConfig() const
    {
        std::ofstream config_doc(configFilename, std::ofstream::binary);
        if (config_doc.fail()) {
            LOG_WRITE(EN_LOG_ERROR, "VSpi config/status write fail.");
            return false;
        }
        config_doc << config;
        return true;
    }

    Json::Value config;
    const std::string configFilename = DEF_OPS_FILE_PATH;

    const std::string mfg =
        "Configuration: MOLEX\r"
        "Firmware Vers: 0.01.0024\r"
        "Serial Number: E4159XXX\r"
        "HW version: 3.01.01.0\r"
        "IPL version: 1.00.0012 Oct 29 2021-13:56:22\r"
        "PLD version: 6.01.0021\r"
        "EEPROM version: SW_0.01\r"
        "EEPROM checksum: OK\r"
        "WS Lib version: Ver2.0.0\r"
        "Part Number: 1834020XXX\r"
        "Bundle version: 1.00.0001\r"  /*upgrade change this line */
        "SubPart Number: 1834020XXX\r"
        "Mfg date: 2022-04-03\r"
        "CAL date: 2022-04-04\r"
        "Boot version: 1.00.0002\r"
        "ModuleType: VM-OPS\r";
};

