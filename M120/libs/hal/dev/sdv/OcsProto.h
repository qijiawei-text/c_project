/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcsProto.h
   Author:        Guo Guangjun
   Date:          2024-01-04
   Version:       1.0
   Description:
   Function List:

   History:
   [Guo Guangjun] [2024-01-04] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "RspHandler.h"
#include "Proto.h"


#define OCS_RESP_STATUS_OK                       (0x00)
#define OCS_RESP_STATUS_BUSY                     (0x01)
#define OCS_RESP_STATUS_INVALID_OPCODE           (0x02)
#define OCS_RESP_STATUS_INVALID_LENGTH           (0x03)
#define OCS_RESP_STATUS_INVALID_DATA             (0x04)
#define OCS_RESP_STATUS_INVALID_CHECHSUM         (0x05)
#define OCS_RESP_STATUS_EXECUTE_FAILED           (0x06)
#define OCS_RESP_STATUS_UNKNOWN_FAILURE          (0xFF)

#define DEF_OCS_UPGD_PKG_LEN_MAX                 (1024)
#define DEF_OCS_CMD_RESP_MAX_LEN                 (512)
#define DEF_OCS_MFG_INFO_LEN_MAX                 (16)

#define OCS_CMD_RESP_DATA_LENGTH(highByte, lowByte)      (((0x00FF && (highByte)) << 8) | (lowByte))

class COcsSwProto : public CProto
{
public:
    COcsSwProto()
    {
    }
    virtual
    ~COcsSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) = 0;
    virtual bool CheckState(uint8_t *pcResponse, bool &rbTryAgain) = 0;
    virtual uint8_t CheckSum(uint8_t *pucStream, uint16_t usStreamLen) = 0;

    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;

    virtual void GetSwitchChannelsCmd(std::string &rstaMsg) = 0;
    virtual bool GetSwitchChannelsRsp(const std::string &rstaRsp, COcsConnections &rstData) = 0;

    virtual void SetSwitchChannelsCmd(std::string &rstaMsg, const COcsConnections &rstData) = 0;

    virtual void GetAlarmCmd(std::string &rstaMsg) = 0;
    virtual bool GetAlarmRsp(const std::string &c_rstaMsg, COcsAlarm &rstData) = 0;

    virtual void GetCaseTemperatureCmd(std::string &rstaMsg) = 0;
    virtual bool GetCaseTemperatureRsp(const std::string &c_rstaMsg, COcsTempSta &rstData) = 0;

    virtual void GetLedStateCmd(std::string &rstaMsg) = 0;
    virtual bool GetLedStateRsp(const std::string &c_rstaMsg, COcsLedSta &rstData) = 0;

    virtual void SetLampTestStateCmd(std::string &rstaMsg, bool bEnable) = 0;

    /* Reset */
    virtual void SoftReset(std::string &rstaMsg) = 0;


    /* Upgarde */
    virtual void StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) = 0;
    virtual void DownloadData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) = 0;
    virtual void Verify(std::string &rstaMsg) = 0;
    virtual void Switch(std::string &rstaMsg) = 0;
    virtual void Commit(std::string &rstaMsg) = 0;

    virtual void AbortDownload(std::string &rstaMsg) = 0;
};

class COcsSwFrameProto : public COcsSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual bool CheckState(uint8_t *pcResponse, bool &rbTryAgain) override;
    virtual uint8_t CheckSum(uint8_t *pucStream, uint16_t usStreamLen) override;

    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;

    virtual void GetSwitchChannelsCmd(std::string &rstaMsg) override;
    virtual bool GetSwitchChannelsRsp(const std::string &rstaRsp, COcsConnections &rstData) override;

    virtual void SetSwitchChannelsCmd(std::string &rstaMsg, const COcsConnections &rstData) override;

    virtual void GetAlarmCmd(std::string &rstaMsg) override;
    virtual bool GetAlarmRsp(const std::string &c_rstaMsg, COcsAlarm &rstData) override;

    virtual void GetCaseTemperatureCmd(std::string &rstaMsg) override;
    virtual bool GetCaseTemperatureRsp(const std::string &c_rstaMsg, COcsTempSta &rstData) override;

    virtual void GetLedStateCmd(std::string &rstaMsg) override;
    virtual bool GetLedStateRsp(const std::string &c_rstaMsg, COcsLedSta &rstData) override;

    virtual void SetLampTestStateCmd(std::string &rstaMsg, bool bEnable) override;

    /* Reset */
    virtual void SoftReset(std::string &rstaMsg) override;


    /* Upgarde */
    virtual void StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) override;
    virtual void DownloadData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) override;
    virtual void Verify(std::string &rstaMsg) override;
    virtual void Switch(std::string &rstaMsg) override;
    virtual void Commit(std::string &rstaMsg) override;

    virtual void AbortDownload(std::string &rstaMsg) override;

public:
    static uint8_t GenerateFrameMsgId();

private:
    static uint8_t m_ucGlobalMsgId;
    void BuildFrameCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg);

};

