/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmMasterSpiProto.h
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"

#include "OcmProto.h"
#include "FrameProto.h"

/*************************************************/
#if 0
#define DEF_OCM_UPGD_PKG_LEN_MAX        (1024)
#define OCM_CMD_MAX_LEN                 (4096)

#define OCM_RESP_STATUS_OK              (0x0)
#define OCM_RESP_STATUS_NOVALID_DATA    (0xa)
#endif
/*0x01 - 3.125GHz Slice; 0x02 - 6.25GHz Slice; 0x04 -12.5GHz slice */
#define DEF_OCM_Slice_Type1             1
#define DEF_OCM_Slice_Type2             2
#define DEF_OCM_Slice_Type4             4

class COcmSwMasterSpiFrameProto : public COcmSwProto
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










