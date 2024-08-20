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
#include "VBoardDriver.h"
#include <fstream>
#include <jsoncpp/json/json.h>
/*#include <json/json.h> */
#include <unordered_map>
#include "OaComm.h"

#define DEF_OA_FILE_PATH "/var/vevice/SPIOaConfig.json"
#define DEF_OA_P_FILE_PATH "/var/vevice/SPIOa-pConfig.json"
/*==============================function=========================*/

/* supports OA at present */
class COaVSpi : public CInterfaceDriver
{
public:
    COaVSpi(uint32_t m_uiHwSubIdx)
    {
        if (0 == m_uiHwSubIdx) {
            configFilename = DEF_OA_FILE_PATH;
        } else {
            configFilename = DEF_OA_P_FILE_PATH;
        }
    }

    virtual
    ~COaVSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

private:
    /* OA */
    uint8_t *GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetStatusData(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetGainRange(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetGain(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetTilt(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetMode(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    /* uint8_t *SetRxLosThr(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler); */
    /* uint8_t *SetRxLosHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler); */
    uint8_t *SetRxLosThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetPumpEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetAprEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetAutolosEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetAlarmData(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetCurrents(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    /* OSC */
    uint8_t *SetDfbEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    /* OSC */
    uint8_t *SetOscEnable(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetOscAddThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetOscDropThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetOscRxThrHys(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    /* OTDR */
    uint8_t *SetOtdrSwitchPos(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetOtdrCali(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    /* VOA */
    uint8_t *SetAttn(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    /* RESET */
    uint8_t *Reset(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Upgrade(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetUpgradeStatus(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetUpgState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *Swap(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    uint8_t *ShutterState(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

    /* OCM */
    uint8_t *GetOcmCali(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);

private:
    /* map the command code to corresponding commands of VSpi */
    typedef uint8_t * (COaVSpi::*command)(const std::string&, const std::shared_ptr<CRspHandler> stpRspHandler);
    const std::unordered_map<int, command> CommandsMap = {
        /* OA */
        {0x10, &COaVSpi::GetMfg},
        {0x21, &COaVSpi::GetStatusData},
        {0x50, &COaVSpi::SetGainRange},
        {0x51, &COaVSpi::SetGain},
        {0x54, &COaVSpi::SetTilt},
        {0x52, &COaVSpi::SetMode},
#if defined __HCF__ || defined __OCS__
        {0x65, &COaVSpi::SetCurrents},
#endif
        /* {0x58, &COaVSpi::SetRxLosThr}, */
        /* {0x59, &COaVSpi::SetRxLosHys}, */
        {0x58, &COaVSpi::SetRxLosThrHys},

        {0x53, &COaVSpi::SetPumpEnable},
        {0x56, &COaVSpi::SetAprEnable},
        {0x57, &COaVSpi::SetAutolosEnable},

        {0x2B, &COaVSpi::GetAlarmData},
        {0xF1, &COaVSpi::Reset},
        {0xAD, &COaVSpi::GetUpgradeStatus},
        {0xF5, &COaVSpi::Swap},
        {0xA9, &COaVSpi::SetUpgState},
        /* DFB */
        {0xA0, &COaVSpi::SetDfbEnable},
        /* OSC */
        {0xA1, &COaVSpi::SetOscEnable},
        {0xA3, &COaVSpi::SetOscAddThrHys},
        {0xA4, &COaVSpi::SetOscDropThrHys},
        {0xA5, &COaVSpi::SetOscRxThrHys},

        /* OTDR */
        {0XAB, &COaVSpi::GetOtdrCali},
        {0xA7, &COaVSpi::SetOtdrSwitchPos},

        /* VOA */
        {0x55, &COaVSpi::SetAttn},

        /* Shutter */
        {0xB2, &COaVSpi::ShutterState},

        /* OCM */
        {0XAC, &COaVSpi::GetOcmCali}

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

    const std::string mfg =
        "Configuration: VM-OA\n"
        "Firmware Vers: 1.00.0002\n"
        "IPL version: 1.01.0001\n"
        "HW version: 2.01\n"
        "SW version: 1.00.0002\n"    /*set in Json file */
        "Serial Number: C8028XXX\n"
        "Part Number: 1834110XXX\n"
        "PLD version: 1.01.0001\n"
        "MAN_DATE: 2022-04-04\n"
        "CAL_DATE: 2022-04-04\n";

};
