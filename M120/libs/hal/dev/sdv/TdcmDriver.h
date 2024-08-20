/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TdcmDriver.h
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "DevData.h"
#include "BoardCmd.h"
#include "TdcmProto.h"
#include "I2cDriver.h"

/*************************************************/

class CTdcm : public CDevDriver
{
public:
    CTdcm(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }

    virtual
    ~CTdcm()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<CTdcmSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CTdcmStatusData &rstData) = 0;
    virtual bool GetCfgData(CTdcmCfgData &rstData) = 0;
    virtual bool SetFrequency(float fFrequency) = 0;
    virtual bool SetDispersion(float fDispersion) = 0;
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) = 0;

protected:
    std::shared_ptr<CTdcmSwProto> m_stpProtoPtr = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;

    virtual bool GetFrequency(float &fFrequency) = 0;
    virtual bool GetDispersion(float &fDispersion) = 0;

};


class CI2cTdcm : public CTdcm
{
public:
    CI2cTdcm(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiDevAddr) :
        CTdcm(uiHwIdx, uiHwSubIdx), m_uiDevAddr(uiDevAddr)
    {
        /* TBD: Logic driver */
        m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx);
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(0);
    }
    ~CI2cTdcm()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CTdcmStatusData &rstData) override;
    virtual bool GetCfgData(CTdcmCfgData &rstData) override;

    virtual bool SetFrequency(float fFrequency) override;
    virtual bool SetDispersion(float fDispersion) override;
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;

protected:
    bool GetMfgItem(uint8_t ucReg, char *pcItem);

    virtual bool GetFrequency(float &fFrequency) override;
    virtual bool GetDispersion(float &fDispersion) override;

private:
    uint32_t m_uiDevAddr;
    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;



};










