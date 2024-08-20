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
#include "OtdrProto.h"
#include "FrameProto.h"
#include "DevOpsData.h"
#include "MolexOtdrProto.h"

#define DEF_OTDR_FILE_PATH "/var/vevice/SPIOtdrConfig.json"
#define DEF_OTDR_P_FILE_PATH "/var/vevice/SPIOtdr-pConfig.json"
/*==============================function=========================*/
/*supports OPS at present */
class COtdrVSpi : public CInterfaceDriver
{
public:
    COtdrVSpi(uint32_t m_uiHwSubIdx)
    {
        if (0 == m_uiHwSubIdx) {
            configFilename = DEF_OTDR_FILE_PATH;
        } else {
            configFilename = DEF_OTDR_P_FILE_PATH;
        }
    }

    virtual
    ~COtdrVSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    uint8_t *GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetAlarm(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetTemp(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetVol(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetCfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetEvent(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetSor(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    uint8_t *Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
private:
    /* map the command code to corresponding commands of VSpi */
    typedef uint8_t * (COtdrVSpi::*command)(const std::string&, const std::shared_ptr<CRspHandler> stpRspHandler);
    const std::unordered_map<int, command> CommandsMap = {
        {0x50, &COtdrVSpi::GetMfg},
        {0x52, &COtdrVSpi::GetCfg},
        {0x13, &COtdrVSpi::GetAlarm},
        {0xAE, &COtdrVSpi::GetTemp},
        {0x11, &COtdrVSpi::GetVol},
        {0x51, &COtdrVSpi::SetCfg},
        {0x22, &COtdrVSpi::GetEvent},
        {0x2c, &COtdrVSpi::GetSor},
        {0x21, &COtdrVSpi::GetState},
        {0x84, &COtdrVSpi::Reset}
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
    std::string configFilename;
    const std::string staOrdrSorFilename = "/var/OTDR_external_3km_X.sor";

    const std::string mfg =
        "Configuration: ACCELINK\n"
        "Firmware Vers: 1.00.0004\n"
        "IPL version: 1.01.0001\n"
        "HW version: 1.0\n"
        "SW version: 1.0.0\n"
        "Serial Number: SN9900000XXX\n"
        "Part Number: OTDR-W1510-D40-P15-LC/UPC\n"
        "PLD version: 1.01.0001\n"
        "MAN_DATE: 2022-04-04\n"
        "Mfg date: 2022-04-03\n"
        "CAL_DATE: 2022-04-04\n"
        "MAC address: 01-02-03-04-05-06 "
        "CAL_DATE: 2022-04-04\n"
        "Boot version: 1.00.0002\n"
        "Bundle version: 1.00.0004\n"
        "ModuleType: VM-OTDR\n";

};