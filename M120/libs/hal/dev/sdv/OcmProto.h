/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmProto.h
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "RspHandler.h"
#include "Proto.h"

/*************************************************/

enum EOcmProto
{
    OCM_RSP_MAX_LEN = 1024,
    OCM_READ_NUM    = 500,


};

#define DEF_OCM_UPGD_PKG_LEN_MAX       (1024)
#define OCM_CMD_MAX_LEN                (OCM_SLICE_MAX_NUM * 2 + 64)  /* (4096) */
#define OCM_CMD_MIN_LEN                (64)

#define OCM_RESP_STATUS_OK              (0x0)
#define OCM_RESP_STATUS_NOVALID_DATA    (0xa)



class COcmSwProto : public CProto
{
public:
    COcmSwProto()
    {
    }
    virtual
    ~COcmSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg) = 0;
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual bool GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetStatusData(std::string &rstaMsg) = 0;
    virtual bool CheckState(uint8_t *pcResponse, bool &rbTryAgain) = 0;
    virtual uint8_t CheckSum(uint8_t *pucStream, uint16_t usStreamLen) = 0;

    virtual void SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan) = 0;
    virtual void SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan) = 0;
    virtual void GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortId) = 0;
    virtual bool GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData) = 0;
    virtual void GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx) = 0;
    virtual bool GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData) = 0;
    virtual void SoftReset(std::string &rstaMsg, uint32_t uiSubIdx) = 0;
    virtual void StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) = 0;
    virtual void SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) = 0;
    virtual void Verify(std::string &rstaMsg) = 0;
    virtual void Switch(std::string &rstaMsg) = 0;
    virtual void EndUpg(std::string &rstaMsg) = 0;
    virtual void GetTempCmd(std::string &rstaMsg, uint32_t uiPortId) = 0;
    virtual bool GetTempRsp(std::string rstaRsp, int16_t &rstData) = 0;
    virtual void GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId) = 0;
    virtual bool GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstWavePlan) = 0;
};

class COcmSwFrameProto : public COcmSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual bool GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;
    virtual bool CheckState(uint8_t *pcResponse, bool &rbTryAgain) override;
    virtual uint8_t CheckSum(uint8_t *pucStream, uint16_t usStreamLen) override;

    virtual void SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan) override;
    virtual void SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan) override;
    virtual void GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData) override;
    virtual void GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx) override;
    virtual bool GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData) override;
    virtual void SoftReset(std::string &rstaMsg, uint32_t uiSubIdx) override;
    virtual void StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) override;
    virtual void SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) override;
    virtual void Verify(std::string &rstaMsg) override;
    virtual void Switch(std::string &rstaMsg) override;
    virtual void EndUpg(std::string &rstaMsg) override;
    virtual void GetTempCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetTempRsp(std::string rstaRsp, int16_t &rstData) override;
    virtual void GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstWavePlan) override;
private:

    void GetCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg /*uint8_t *pucFrameBuf, uint32_t &ruiFrameLen*/);
};

class COcmSwOlspFrameProto : public COcmSwProto
{
public:
    virtual bool ProcRsp(std::string &rstaMsg) override;
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;
    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual bool GetPnRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetStatusData(std::string &rstaMsg) override;
    virtual bool CheckState(uint8_t *pcResponse, bool &rbTryAgain) override;
    virtual uint8_t CheckSum(uint8_t *pucStream, uint16_t usStreamLen) override;

    virtual void SetWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan) override;
    virtual void SetOcmWavePlanCmd(std::string &rstaMsg, uint32_t uiPortId, uint32_t uiChCnt, STChannelGrid *pstWavePlan) override;
    virtual void GetChPowerCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetChPowerRsp(std::string rstaRsp, COcmChPower &rstData) override;
    virtual void GetChRawDataCmd(std::string &rstaMsg, uint32_t uiPortIdx) override;
    virtual bool GetChRawDataRsp(std::string rstaRsp, COcmOsa &rstData) override;
    virtual void SoftReset(std::string &rstaMsg, uint32_t uiSubIdx) override;
    virtual void StartUpg(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) override;
    virtual void SendData(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) override;
    virtual void Verify(std::string &rstaMsg) override;
    virtual void Switch(std::string &rstaMsg) override;
    virtual void EndUpg(std::string &rstaMsg) override;
    virtual void GetTempCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetTempRsp(std::string rstaRsp, int16_t &rstData) override;
    virtual void GetWaveplanCmd(std::string &rstaMsg, uint32_t uiPortId) override;
    virtual bool GetWaveplanRsp(std::string rstaRsp, STChannelGrid &rstWavePlan) override;
private:

    void GetCmd(uint8_t ucPort, uint8_t ucOpcode, uint8_t *pucDataBuf, uint16_t usDataLen, std::string &rstaMsg /*uint8_t *pucFrameBuf, uint32_t &ruiFrameLen*/);
};









