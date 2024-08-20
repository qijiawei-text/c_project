/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      VWssSpiDriver.h
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
#include "StringTool.h"
#include "WssDriver.h"

#define DEF_WSS_FILE_PATH "/var/vevice/SPIWssConfig.json"
/*==============================function=========================*/
#define DEF_WSS_NIS_UART_RECV_LEN_MAX   (4096)
#define DEF_WSS_NIS_BUF_LEN             (DEF_WSS_NIS_UART_RECV_LEN_MAX)

/*supports WSS at present */
class CWssVSpi : public CInterfaceDriver
{
public:
    CWssVSpi()
    {
    }

    virtual
    ~CWssVSpi()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucBufW, uint32_t uiLenW,
                           uint8_t *pucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;

    virtual bool Write(uint32_t uiHwSubIdx, uint8_t *pucBufW, uint32_t usLenW) override;
    virtual bool Read(uint32_t uiHwSubIdx, uint8_t *paucBufR, std::shared_ptr<CRspHandler> stpRspHandler) override;
    virtual bool MultiWrite(uint8_t *pucBufW, uint32_t usLenW);
    virtual bool WRead(uint32_t usCntExist, uint8_t *paucBufR, std::shared_ptr<CRspHandler> stpRspHandler);


private:
    uint8_t *GetMfg(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetChanCenter(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetBandwidth(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *GetChPortAttn(const std::string& staMsg, const std::shared_ptr<CRspHandler> stpRspHandler);
    uint8_t *SetChCout(const std::string& staMsg);
    uint8_t *SetChPortAttn(const std::string& staMsg);
    uint8_t *SetChanCenter(const std::string& staMsg);
    uint8_t *SetBandwidth(const std::string& staMsg);



private:
    /* map the command code to corresponding commands of VSpi */
    typedef uint8_t * (CWssVSpi::*Wcommand)(const std::string&);
    const std::unordered_map<int, Wcommand> WCommandsMap =
    {
        {163, &CWssVSpi::SetChCout},
        {173, &CWssVSpi::SetChPortAttn},
        {165, &CWssVSpi::SetChanCenter},
        {166, &CWssVSpi::SetBandwidth}
    };

    typedef uint8_t * (CWssVSpi::*Rcommand)(const std::string&, const std::shared_ptr<CRspHandler> stpRspHandler);
    const std::unordered_map<int, Rcommand> RCommandsMap =
    {
        {6, &CWssVSpi::GetMfg},
        {165, &CWssVSpi::GetChanCenter},
        {166, &CWssVSpi::GetBandwidth},
        {173, &CWssVSpi::GetChPortAttn}
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
    const std::string configFilename = DEF_WSS_FILE_PATH;

    /*local data */
    uint8_t m_ucOid = 0;
    uint8_t m_ucMid = 0;
    uint16_t m_uslen = 0;
    uint16_t m_uscmd = 0;
    uint8_t m_sRhead[4096] = {0};


    const std::string mfg =
        "acManufacturer: Molex;\n"
        "Module PN: FFDB-C58L020-WAB;\n"
        "Module SN: N020K57XXX;\n"
        "FW version: 8.0.2;\n"
        "HW version: 5.1.0;\n"
        "SW version: 8.0.2;\n"
        "Manufacturing Date: 2020-10-26;\n"
        "Calibration Date: 2020-10-26;\n"
        "Configuration: VM-WSS;\n";
};

