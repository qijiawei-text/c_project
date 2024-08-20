/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OtdrProto.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "RspHandler.h"
#include "Proto.h"

/*************************************************/

enum EOtdrProto
{
    OTDR_RSP_MAX_LEN            = 1024,
    OTDR_READ_NUM               = 500,
    OTDR_SOR_FILE_READ_NUM      = 0xFFF,

};

typedef struct
{
    uint32_t ulScanCtrl;
    uint32_t ulScanMode;
    uint32_t ulExpScanTime;
    uint32_t ulExpPulseWidth;   /* ns */
    uint32_t ulScanStartPos;    /* cm */
    uint32_t ulScanEndPos;      /* cm */

    float fReflIdx;
    float fReflEventThr;       /* dB */
    float fNonReflEventThr;        /* dB */
    float fBackCoef;         /* dB */
    float fOrlThr;            /* dB */
    float fInputPwrThr;         /* dB */
    float fReflEventFhThr;           /* dB */
    float fRamanSetpont;     /* dB */
}ST_MOLEX_OTDR_CONFIG;



class COtdrProto : public CProto
{
public:
    COtdrProto()
    {
    }
    virtual
    ~COtdrProto()
    {
    }

    virtual void
    GetAlarmCmd(std::string &rstaMsg)
    {
    };
    virtual bool
    GetAlarmRsp(const std::string &c_rstaMsg, uint16_t &rusAlarm)
    {
        return false;
    }
    virtual void
    GetVolCmd(std::string &rstaMsg)
    {
    };
    virtual bool
    GetVolRsp(const std::string &c_rstaMsg, uint16_t &rusVol)
    {
        return false;
    }
    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void GetCfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetCfgRsp(const std::string &c_rstaMsg, COtdrCfgData &rstCfg) = 0;
    virtual void SetCfgCmd(std::string &rstaMsg, COtdrCfgData &rstCfg) = 0;

    virtual void SetPara(std::string &rstaMsg, uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution) = 0;
    virtual void SetUior(std::string &rstaMsg, double dUIOR) = 0;
    virtual void SetAcqt(std::string &rstaMsg, uint32_t ulMTime) = 0;
    virtual void SetTref(std::string &rstaMsg, double dTREF) = 0;
    virtual void SetTlos(std::string &rstaMsg, double dTLOS) = 0;
    virtual void SetTeof(std::string &rstaMsg, uint32_t ulEOF) = 0;

    virtual void StartScan(std::string &rstaMsg) = 0;
    virtual void GetState(std::string &rstaMsg) = 0;
    virtual bool GetStateRsp(const std::string &rstaMsg, bool &rbState) = 0;
    virtual void GetSor(std::string &rstaMsg) = 0;
    virtual void GetSorTemp(std::string &rstaMsg, uint32_t uiPackId) = 0;
    virtual void GetSorRsp(const std::string &c_rstaMsg, uint32_t &ruiPackId) = 0;
    virtual bool GetSorHeaderRsp(const std::string &rstaMsg, uint32_t &ruiSize) = 0;
    virtual void GetSorBodyRsp(const std::string &rstaMsg) = 0;
    virtual void GetWaveform(std::string &rstaMsg) = 0;
    virtual void GetEventTop(std::string &rstaMsg) = 0;
    virtual bool GetEventTopRsp(const std::string &c_rstaMsg, COtdrEventTop &rstEventTop) = 0;
    virtual void GetEventLine(std::string &rstaMsg, uint32_t ulLine) = 0;
    virtual bool GetEventLineRsp(const std::string &c_rstaMsg, COtdrEventLine &rstEventLine) = 0;
    virtual void GetEventInfo(std::string &rstaMsg) = 0;
    virtual bool GetEventInfoRsp(const std::string &c_rstaMsg, COtdrStatusData &rstaData) = 0;
    virtual void GetTemp(std::string &rstaMsg) = 0;
    virtual bool GetTempRsp(const std::string &c_rstaMsg, COtdrTemp &rstaData) = 0;

    virtual void SoftReset(std::string &rstaMsg) = 0;
    virtual void UpgStart(std::string &rstaMsg) = 0;

    virtual void SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) = 0;
    virtual void UpgEnd(std::string &rstaMsg) = 0;
    virtual void GetStartOffsetCmd(std::string &rstaMsg) = 0;
    virtual bool GetStartOffsetRsp(const std::string &c_rstaMsg, COtdrStartOffset &rstCfg) = 0;
    virtual void SetStartOffsetCmd(std::string &rstaMsg, COtdrStartOffset &rstCfg) = 0;

};

class COtdrAcclinkSwProto : public COtdrProto
{
public:
    COtdrAcclinkSwProto()
    {
    }
    virtual
    ~COtdrAcclinkSwProto()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;

    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void GetCfgCmd(std::string &rstaMsg) override;
    virtual bool GetCfgRsp(const std::string &c_rstaMsg, COtdrCfgData &rstCfg) override;
    virtual void SetCfgCmd(std::string &rstaMsg, COtdrCfgData &rstCfg) override;

    virtual void SetPara(std::string &rstaMsg, uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution) override;
    virtual void SetUior(std::string &rstaMsg, double dUIOR) override;
    virtual void SetAcqt(std::string &rstaMsg, uint32_t ulMTime) override;
    virtual void SetTref(std::string &rstaMsg, double dTREF) override;
    virtual void SetTlos(std::string &rstaMsg, double dTLOS) override;
    virtual void SetTeof(std::string &rstaMsg, uint32_t ulEOF) override;

    virtual void StartScan(std::string &rstaMsg) override;
    virtual void GetState(std::string &rstaMsg) override;
    virtual bool GetStateRsp(const std::string &rstaMsg, bool &rbState) override;
    virtual void GetSor(std::string &rstaMsg) override;
    virtual void GetSorTemp(std::string &rstaMsg, uint32_t uiPackId) override;
    virtual void GetSorRsp(const std::string &c_rstaMsg, uint32_t &ruiPackId) override;
    virtual bool GetSorHeaderRsp(const std::string &rstaMsg, uint32_t &ruiSize) override;
    virtual void GetSorBodyRsp(const std::string &rstaMsg) override;
    virtual void GetWaveform(std::string &rstaMsg) override;
    virtual void GetEventTop(std::string &rstaMsg) override;
    virtual bool GetEventTopRsp(const std::string &c_rstaMsg, COtdrEventTop &rstEventTop) override;
    virtual void GetEventLine(std::string &rstaMsg, uint32_t ulLine) override;
    virtual bool GetEventLineRsp(const std::string &c_rstaMsg, COtdrEventLine &rstEventLine) override;
    virtual void GetEventInfo(std::string &rstaMsg) override;
    virtual bool GetEventInfoRsp(const std::string &c_rstaMsg, COtdrStatusData &rstaData) override;
    virtual void GetTemp(std::string &rstaMsg) override;
    virtual bool GetTempRsp(const std::string &c_rstaMsg, COtdrTemp &rstaData) override;

    virtual void SoftReset(std::string &rstaMsg) override;
    virtual void UpgStart(std::string &rstaMsg) override;

    virtual void SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) override;
    virtual void UpgEnd(std::string &rstaMsg) override;
    virtual void GetStartOffsetCmd(std::string &rstaMsg) override;
    virtual bool GetStartOffsetRsp(const std::string &c_rstaMsg, COtdrStartOffset &rstCfg) override;
    virtual void SetStartOffsetCmd(std::string &rstaMsg, COtdrStartOffset &rstCfg) override;;

};




