/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MolexOtdrProto.h
   Author:        Zhu Weian
   Date:          2021-02-15
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2021-02-15] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "OtdrProto.h"
#include "FrameProto.h"



typedef struct
{
    uint32_t uiIdx;
    float fDistance;
    int32_t iAttenCoe;
    int32_t iLoss;
    int32_t iReflectance;
    int32_t iCumulatedLoss;
    uint32_t uiType;
}ST_MOLEX_OTDR_EVENT_INFO;

#define DEF_OTDR_VER_LEN        (16)
#define DEF_OTDR_PN_LEN         (32)
#define DEF_OTDR_VER_TOTAL_LEN (256)

typedef struct {
    char acSN        [DEF_OTDR_PN_LEN];
    char acPN        [DEF_OTDR_PN_LEN];
    char acSubPN     [DEF_OTDR_PN_LEN];
    char acHWVer     [DEF_OTDR_VER_LEN];
    char acBundleVer [DEF_OTDR_VER_LEN];
    char acBootVer   [DEF_OTDR_VER_LEN];
    char acMainVer   [DEF_OTDR_VER_LEN];
    char acFpgaVer   [DEF_OTDR_VER_LEN];
    char acMfgDate   [DEF_OTDR_VER_LEN];
    char acCalDate   [DEF_OTDR_VER_LEN];
    char acReserved  [DEF_OTDR_VER_TOTAL_LEN - 3 * DEF_OTDR_PN_LEN - 7 * DEF_OTDR_VER_LEN];

}ST_MOLEX_OTDR_MFG_INFO;

typedef struct
{
    uint32_t uiEventNum;
    ST_MOLEX_OTDR_EVENT_INFO staEventInfo[100];

}ST_EVENTS_INFO;


typedef struct
{
    uint8_t ucStatus;
    uint8_t ucPercent;
}ST_ACQ_STATUS;

typedef struct
{
    int16_t sTecTemp;
    int16_t sModuleTemp;
    int16_t sFpgaTemp;

    uint8_t ucReserved[8];
}ST_TEMP;

class COtdrProtoMolex : public COtdrProto
{
public:
    COtdrProtoMolex()
    {
    }

    virtual
    ~COtdrProtoMolex()
    {
    }

    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;

    virtual void GetVolCmd(std::string &rstaMsg) override;
    virtual bool GetVolRsp(const std::string &c_rstaMsg, uint16_t &rusVol) override;
    virtual void GetAlarmCmd(std::string &rstaMsg) override;
    virtual bool GetAlarmRsp(const std::string &c_rstaMsg, uint16_t &rusAlarm) override;
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

private:
    bool __TempConvert(int16_t sTemp, float &rfTemp);


};

