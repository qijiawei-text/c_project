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


#define DEF_OCM_FILE_PATH "/var/vevice/SPIOcmConfig.json"
#define DEF_OCM_P_FILE_PATH "/var/vevice/SPIOcm-pConfig.json"
/*==============================function=========================*/

/* supports OCM at present */
class COcmVSpi : public CInterfaceDriver
{
public:
    COcmVSpi(uint32_t m_uiHwSubIdx)
    {
        if (0 == m_uiHwSubIdx) {
            configFilename = DEF_OCM_FILE_PATH;
        } else {
            configFilename = DEF_OCM_P_FILE_PATH;
        }
    }

    virtual
    ~COcmVSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    uint8_t *GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetChPower(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetOsa(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetWavePlan(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetTemp(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetUpgadeStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Swap(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);


private:
    /* map the command code to corresponding commands of VSpi */
    typedef uint8_t * (COcmVSpi::*command)(const std::string&, const std::shared_ptr<CRspHandler> stpRspHandler);
    const std::unordered_map<int, command> CommandsMap = {
        {0x18, &COcmVSpi::GetMfg},
        {0x11, &COcmVSpi::GetChPower},
        {0x14, &COcmVSpi::GetOsa},
        {0x26, &COcmVSpi::SetWavePlan},
        {0xf6, &COcmVSpi::Reset},
        {0xf3, &COcmVSpi::Reset},
        {0xF0, &COcmVSpi::Upgrade},
        {0x12, &COcmVSpi::GetTemp}
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
    uint8_t sliceCounter = 0;

    std::string mfg =
        "Boot version: 1.00.0002\n"
        "Main version: 1.00.0003\n"  /*upgrade change this line */
        "HW version: 2.0.0\n"
        "Module SN: D8824XXX\n"
        "Module PN: 1834110XXX\n"
        "Manufacturing Date: 2022-03-04\n"
        "Calibration Date: 2022-03-04\n"
        "Configuration: VM-OCM\n"
        "Firmware Vers: 0.01.0003\n";
};
