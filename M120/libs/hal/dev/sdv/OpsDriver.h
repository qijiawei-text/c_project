/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsDriver.h
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
#include "OpsProto.h"

/*************************************************/

class COpsDriver : public CDevDriver
{
public:
    COpsDriver(uint32_t uiHwIdx, const std::shared_ptr<CResetBoard> &c_rstpResetBoard, uint32_t uiResetIdx)
        : CDevDriver(uiHwIdx), m_stpResetBoardPtr(c_rstpResetBoard), m_uiResetIdx(uiResetIdx)
    {
    }
    COpsDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~COpsDriver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<COpsSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetCommonStatusData(COpsCommStatusData &rstData) = 0;
    virtual bool GetStatusData(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetCfgData(COpsCfgData &rstData, uint32_t uiSubIdx = 0) = 0;

    virtual bool GetWorkMode(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) = 0;

    virtual bool GetPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPds(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPdThr(COpsConnectionPdThr &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetConnectionPdThr(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPdHys(COpsConnectionPdHys &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetConnectionPdHys(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerHistory(COpsHistoryData &rstData, uint32_t uiSubIdx) = 0;

    virtual bool GetAlarm(COpsCommStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerThr(CPortPowerThr &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerHys(CPortPowerHys &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetPortPowerDiffer(int32_t iDiffer, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetMode(uint32_t uiMode, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetWtr(uint32_t uiWtr, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetHoldOffTime(uint32_t uiHoldOffTime, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetRevertive(uint32_t uiRevertive, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetRelativeThr(int32_t iThr, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRelativeThr(CRelativeThr &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetRelativeOffset(int32_t iOffset, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRelativeOffset(CRelativeOffset &rstData, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetSwitchThr(uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchThr(COpsSwitchThr &rstThr, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetSwitchHys(int32_t uiPos, int32_t iHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchHys(COpsSwitchHys &rstHys, uint32_t uiSubIdx = 0) = 0;


    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) = 0;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) = 0;
    virtual bool GetUpgStat(COpsUpgStat &rstData, uint32_t uiSubIdx = 0) = 0;

    /* WSS */

    virtual bool GetWssAlarm(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerHistory(uint32_t uiSubIdx = 0) = 0;

    virtual bool SetWssPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetWssPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerThr(CWssPortPowerThr &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerHys(CWssPortPowerHys &rstData, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetWssLedMode(uint32_t uiMode, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetCurrentRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, uint32_t uiSubIdx = 0) = 0;

    virtual bool SendAlarm(COpsCommStatusData &rstCommData, COpsStatusData *rstData, uint32_t uiSubIdx = 0) = 0;

    virtual bool GetOscPd(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetOscSwitchPos(COscSwitchPos &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetOscSwitchPos(uint32_t uiSwitchIdx, uint32_t uiPos, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetLedStatus(uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetLedMode(uint32_t uiMode, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetLedStatus(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAlarmDuration(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool ClearAlarmDuration(uint32_t uiSubIdx = 0) = 0;
    virtual bool GetVoltages(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetIl(COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAlmMask(uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAlmMask(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetEventLog(COpsEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 0) = 0;
    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                                      */
    /********************************************************************************************/
    /* @ {                                                                                      */
    virtual bool SetRemoteStatus(COpsReqStatus &stStatus, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRequestStatus(COpsBidirectionStatus &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetSwitchingType(uint8_t ucType, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchingType(uint8_t &ucType, uint32_t uiSubIdx = 0) = 0;
    /* @ }                                                                                      */
    virtual uint32_t GetPortCnt() = 0;
    volatile bool m_bInterruptFlag = false;
protected:
    std::shared_ptr<COpsSwProto> m_stpProtoPtr = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;
    uint32_t m_uiResetIdx = 0;

};


class COpsUartDriver : public COpsDriver
{
public:

    COpsUartDriver(uint32_t uiHwIdx, const std::shared_ptr<CResetBoard> &c_rstpResetBoard, uint32_t uiResetIdx,
                   const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::shared_ptr<COpsSwProto> &c_rstpProtoPtr) :
        COpsDriver(uiHwIdx, c_rstpResetBoard, uiResetIdx), m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        memset(&m_stAlarm, 0, sizeof(COpsAlarm));
        memset(&m_stWssAlarm, 0, sizeof(CWssAlarm));
        memset(&m_ucSwitchPos, 0, sizeof(m_ucSwitchPos));
    }


    ~COpsUartDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetCommonStatusData(COpsCommStatusData &rstData) override;
    virtual bool GetStatusData(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetCfgData(COpsCfgData &rstData, uint32_t uiSubIdx = 0) override;

    virtual bool GetPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPds(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPdThr(COpsConnectionPdThr &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetConnectionPdThr(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPdHys(COpsConnectionPdHys &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetConnectionPdHys(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerHistory(COpsHistoryData &rstData, uint32_t uiSubIdx) override;
    virtual bool GetAlarm(COpsCommStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) override;

    virtual bool SetPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 0) override;
    virtual bool SetPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerThr(CPortPowerThr &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerHys(CPortPowerHys &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetPortPowerDiffer(int32_t iDiffer, uint32_t uiSubIdx = 0) override;

    virtual bool GetWorkMode(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) override;
    virtual bool SetMode(uint32_t uiMode, uint32_t uiSubIdx = 0) override;
    virtual bool SetWtr(uint32_t uiWtr, uint32_t uiSubIdx = 0) override;
    virtual bool SetHoldOffTime(uint32_t uiHoldOffTime, uint32_t uiSubIdx = 0) override;
    virtual bool SetRevertive(uint32_t uiRevertive, uint32_t uiSubIdx = 0) override;

    virtual bool SetRelativeThr(int32_t iThr, uint32_t uiSubIdx = 0) override;
    virtual bool GetRelativeThr(CRelativeThr &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetRelativeOffset(int32_t iOffset, uint32_t uiSubIdx = 0) override;
    virtual bool GetRelativeOffset(CRelativeOffset &rstData, uint32_t uiSubIdx = 0) override;

    virtual bool SetSwitchThr(uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchThr(COpsSwitchThr &rstThr, uint32_t uiSubIdx = 0) override;
    virtual bool SetSwitchHys(int32_t uiPos, int32_t iHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchHys(COpsSwitchHys &rstHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetUpgStat(COpsUpgStat &rstData, uint32_t uiSubIdx = 0) override;

    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;

    virtual bool GetWssAlarm(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerHistory(uint32_t uiSubIdx = 0) override;

    virtual bool SetWssPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 0) override;
    virtual bool SetWssPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerThr(CWssPortPowerThr &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerHys(CWssPortPowerHys &rstData, uint32_t uiSubIdx = 0) override;

    virtual bool SetWssLedMode(uint32_t uiMode, uint32_t uiSubIdx = 0) override;
    virtual bool SetCurrentRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, uint32_t uiSubIdx = 0) override;

    virtual bool SendAlarm(COpsCommStatusData &rstCommData, COpsStatusData *rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetOscPd(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetOscSwitchPos(COscSwitchPos &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetOscSwitchPos(uint32_t uiSwitchIdx, uint32_t uiPos, uint32_t uiSubIdx = 0) override;

    virtual bool SetLedStatus(uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx = 0) override;
    virtual bool SetLedMode(uint32_t uiMode, uint32_t uiSubIdx = 0) override;
    virtual bool GetLedStatus(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetAlarmDuration(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool ClearAlarmDuration(uint32_t uiSubIdx = 0) override;
    virtual bool GetVoltages(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetIl(COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetAlmMask(uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx = 0) override;
    virtual bool GetAlmMask(COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetEventLog(COpsEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 0) override;
    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                                      */
    /********************************************************************************************/
    /* @ {                                                                                      */
    virtual bool SetRemoteStatus(COpsReqStatus &stStatus, uint32_t uiSubIdx = 0) override;
    virtual bool GetRequestStatus(COpsBidirectionStatus &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetSwitchingType(uint8_t ucType, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchingType(uint8_t &ucType, uint32_t uiSubIdx = 0) override;
    /* @ }                                                                                      */
    virtual uint32_t GetPortCnt() override;
private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<COpsSwProto> m_stpProtoPtr = nullptr;

    uint8_t m_ucSwitchPos[OPS4_OPS_CNT_MAX];
    COpsAlarm m_stAlarm;
    CWssAlarm m_stWssAlarm;

    uint32_t m_uiHour = 0xFF;
    uint32_t m_uiPortCnt = OPS1_OPS_CNT_MAX; /* ops1:1, ops-4:4  */
    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg);
};


class COpsLogicSpiDriver : public COpsDriver
{
public:

    COpsLogicSpiDriver(uint32_t uiHwIdx, const std::shared_ptr<CResetBoard> &c_rstpResetBoard, uint32_t uiResetIdx,
                       const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::shared_ptr<COpsSwProto> &c_rstpProtoPtr) :
        COpsDriver(OPS_HW_IDX(uiHwIdx), c_rstpResetBoard, uiResetIdx), m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        memset(&m_stAlarm, 0, sizeof(COpsAlarm));
        memset(&m_stWssAlarm, 0, sizeof(CWssAlarm));
        memset(&m_ucSwitchPos, 0, sizeof(m_ucSwitchPos));
        memset(&m_ucBidiSwPos, 0, sizeof(m_ucBidiSwPos));
        memset(&m_ucBidiSwSta, 0, sizeof(m_ucBidiSwSta));
        LOG_WRITE(EN_LOG_NOTICE, "COpsLogicSpiDriver, uiHwIdx:%d", OPS_HW_IDX(uiHwIdx));
        std::shared_ptr<CLogicDriver> stpLogicDrvPtr = CBoardCmd::GetLogicDrvPtr(0);
        if (nullptr != stpLogicDrvPtr) {
            LOG_WRITE(EN_LOG_NOTICE, "enable pcie int");
            uint8_t ucDataW = 0x11;
            stpLogicDrvPtr->Write(0x8, &ucDataW, 1);
        }
        if (OPS_HW_TYPE(uiHwIdx) > 0) {
            m_uiPortCnt = OPS4_OPS_CNT_MAX;
        }
    }


    ~COpsLogicSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetCommonStatusData(COpsCommStatusData &rstData) override;
    virtual bool GetStatusData(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetCfgData(COpsCfgData &rstData, uint32_t uiSubIdx = 1) override;

    virtual bool GetPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetConnectionPds(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetConnectionPdThr(COpsConnectionPdThr &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetConnectionPdThr(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx = 1) override;
    virtual bool GetConnectionPdHys(COpsConnectionPdHys &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetConnectionPdHys(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx = 1) override;

    virtual bool GetPortPowerHistory(COpsHistoryData &rstData, uint32_t uiSubIdx) override;
    virtual bool GetAlarm(COpsCommStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 1) override;

    virtual bool SetPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 1) override;
    virtual bool SetPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 1) override;
    virtual bool GetPortPowerThr(CPortPowerThr &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetPortPowerHys(CPortPowerHys &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetPortPowerDiffer(int32_t iDiffer, uint32_t uiSubIdx = 1) override;

    virtual bool GetWorkMode(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 1) override;
    virtual bool SetMode(uint32_t uiMode, uint32_t uiSubIdx = 1) override;
    virtual bool SetWtr(uint32_t uiWtr, uint32_t uiSubIdx = 1) override;
    virtual bool SetHoldOffTime(uint32_t uiHoldOffTime, uint32_t uiSubIdx = 1) override;
    virtual bool SetRevertive(uint32_t uiRevertive, uint32_t uiSubIdx = 1) override;

    virtual bool SetRelativeThr(int32_t iThr, uint32_t uiSubIdx = 1) override;
    virtual bool GetRelativeThr(CRelativeThr &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetRelativeOffset(int32_t iOffset, uint32_t uiSubIdx = 1) override;
    virtual bool GetRelativeOffset(CRelativeOffset &rstData, uint32_t uiSubIdx = 1) override;

    virtual bool SetSwitchThr(uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx = 1) override;
    virtual bool GetSwitchThr(COpsSwitchThr &rstThr, uint32_t uiSubIdx = 1) override;
    virtual bool SetSwitchHys(int32_t uiPos, int32_t iHys, uint32_t uiSubIdx = 1) override;
    virtual bool GetSwitchHys(COpsSwitchHys &rstHys, uint32_t uiSubIdx = 1) override;

    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 1) override;
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;
    virtual bool GetUpgStat(COpsUpgStat &rstData, uint32_t uiSubIdx = 1) override;

    virtual bool GetWssAlarm(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx = 1) override;
    virtual bool GetWssPortPower(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetWssPortPowerHistory(uint32_t uiSubIdx = 1) override;

    virtual bool SetWssPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 1) override;
    virtual bool SetWssPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 1) override;
    virtual bool GetWssPortPowerThr(CWssPortPowerThr &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetWssPortPowerHys(CWssPortPowerHys &rstData, uint32_t uiSubIdx = 1) override;

    virtual bool SetWssLedMode(uint32_t uiMode, uint32_t uiSubIdx = 1) override;
    virtual bool SetCurrentRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, uint32_t uiSubIdx = 1) override;

    virtual bool SendAlarm(COpsCommStatusData &rstCommData, COpsStatusData *rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetOscPd(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetOscSwitchPos(COscSwitchPos &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetOscSwitchPos(uint32_t uiSwitchIdx, uint32_t uiPos, uint32_t uiSubIdx = 1) override;

    virtual bool SetLedStatus(uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx = 1) override;
    virtual bool SetLedMode(uint32_t uiMode, uint32_t uiSubIdx = 1) override;
    virtual bool GetLedStatus(COpsCommStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetAlarmDuration(COpsCommStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool ClearAlarmDuration(uint32_t uiSubIdx = 1) override;
    virtual bool GetVoltages(COpsCommStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetIl(COpsStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetAlmMask(uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx = 1) override;
    virtual bool GetAlmMask(COpsCommStatusData &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool GetEventLog(COpsEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 1) override;
    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                                      */
    /********************************************************************************************/
    /* @ {                                                                                      */
    virtual bool SetRemoteStatus(COpsReqStatus &stStatus, uint32_t uiSubIdx = 1) override;
    virtual bool GetRequestStatus(COpsBidirectionStatus &rstData, uint32_t uiSubIdx = 1) override;
    virtual bool SetSwitchingType(uint8_t ucType, uint32_t uiSubIdx = 1) override;
    virtual bool GetSwitchingType(uint8_t &ucType, uint32_t uiSubIdx = 1) override;
    /* @ }                                                                                      */
    virtual uint32_t GetPortCnt() override;
private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<COpsSwProto> m_stpProtoPtr = nullptr;

    uint8_t m_ucSwitchPos[OPS4_OPS_CNT_MAX];
    uint8_t m_ucBidiSwPos[OPS4_OPS_CNT_MAX];
    uint8_t m_ucBidiSwSta[OPS4_OPS_CNT_MAX];
    COpsAlarm m_stAlarm;
    CWssAlarm m_stWssAlarm;

    uint32_t m_uiHour = 0xFF;
    uint32_t m_uiPortCnt = OPS1_OPS_CNT_MAX; /* ops1:1, ops-4:4  */
    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen, uint32_t uiTimeOut = 1);
    bool __SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string staMsg, uint32_t uiTimeOut = 1);
};




