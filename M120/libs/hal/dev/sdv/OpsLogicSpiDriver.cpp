/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "File.h"

#include "OpsDriver.h"
#include "Event.h"
#include "FrameProto.h"

#define CHECK_INTERRUPT() do { \
        if (m_bInterruptFlag) { \
            return false; \
        } \
} while (0)

#define SET_INTERRUPT() do { \
        m_bInterruptFlag = true; \
} while (0)

#define RELEASE_INTERRUPT() do { \
        m_bInterruptFlag = false; \
} while (0)

/*==============================function=========================*/

bool
COpsLogicSpiDriver::IsOnline(void)
{

    return true;
}

bool
COpsLogicSpiDriver::GetMfg(CMfg &rstMfg)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    printf("%s ++\n", __func__);

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::GetMfg ++");
    memset(&rstMfg, 0, sizeof(rstMfg));

    std::string staMsg;

    /* Get mfginfo */
    m_stpProtoPtr->GetMfgCmd(staMsg);

    std::shared_ptr<CRspHandler> stpMfgRspHandler = __RspHandler(sizeof(ST_MFG_INFO));
    if (!__SendCmd(staMsg, stpMfgRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetMfg error.");
        return false;
    }

    std::string staRsp;
    stpMfgRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg);

    std::string staMsgEx;

    /* Get mfginfo */
    m_stpProtoPtr->GetMfgExCmd(staMsgEx);

    std::shared_ptr<CRspHandler> stpMfgRspExHandler = __RspHandler(512);
    if (!__SendCmd(staMsgEx, stpMfgRspExHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetMfgEx error.");
        return false;
    }

    std::string staRspEx;
    stpMfgRspExHandler->Copy(staRspEx);
    m_stpProtoPtr->GetMfgExRsp(staRspEx, rstMfg);

    /* Get version */
    m_stpProtoPtr->GetVerCmd(staMsg);


    std::shared_ptr<CRspHandler> stpVerRspHandler = __RspHandler(sizeof(ST_VER));

    if (!__SendCmd(staMsg, stpVerRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Ver error.");
        return false;
    }

    std::string staVerRsp;
    stpVerRspHandler->Copy(staVerRsp);
    m_stpProtoPtr->GetVerRsp(staVerRsp, rstMfg);

    printf("acFwVer = %s\n", rstMfg.acFwVer);


    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "acFpgaRun %s", rstMfg.acFpgaRunVer);
    LOG_WRITE(EN_LOG_NOTICE, "acSwVer %s", rstMfg.acSwVer);
#ifdef __NR1004__
    LOG_WRITE(EN_LOG_NOTICE, "acUnitName %s", rstMfg.acUnitName);
    LOG_WRITE(EN_LOG_NOTICE, "acFCPN %s", rstMfg.acFCPN);
    LOG_WRITE(EN_LOG_NOTICE, "acFCIss %s", rstMfg.acFCIss);
    LOG_WRITE(EN_LOG_NOTICE, "acFujitsuPN %s", rstMfg.acFujitsuPN);
    LOG_WRITE(EN_LOG_NOTICE, "acCleiCode %s", rstMfg.acCleiCode);
    /*LOG_WRITE(EN_LOG_NOTICE, "acFjtSn %s", rstMfg.acFjtSn); */
#endif
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::GetMfg --");

    printf("%s --\n", __func__);

    return true;
}

bool
COpsLogicSpiDriver::GetCommonStatusData(COpsCommStatusData &rstData)
{
    COpsCommStatusData data;
    bool bAlarm = false;
    memset(&data, 0, sizeof(data));
    if (!GetAlarm(data, bAlarm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }
    CHECK_INTERRUPT();
    if (!GetLedStatus(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetLedStatus error.");
        return false;
    }
    CHECK_INTERRUPT();
#if 0
    if (!GetVoltages(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetVoltages error.");
        return false;
    }
    CHECK_INTERRUPT();
#endif


#ifdef _NR1004_
#if 0
    if (!GetAlmMask(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlmMask error.");
        /* return false;  comment for compatible reason */
    }
    CHECK_INTERRUPT();
#endif
#endif

    memcpy(&rstData, &data, sizeof(data));
    return true;
}
bool
COpsLogicSpiDriver::GetStatusData(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if (m_bCommFailDebug) {
        return false;
    }

    CHECK_INTERRUPT();

    bool bWorkModeAlarm = false; /* bAlarm = false;   bWssAlarm = false; */
    COpsStatusData data;
    memset(&data, 0, sizeof(data));
#if 1
    if (!GetPortPower(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPower error.");
        return false;
    }
#endif

    CHECK_INTERRUPT();

#if 1
    if (!GetConnectionPds(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetConnectionPds error.");
        return false;
    }
#endif

    CHECK_INTERRUPT();

#if 1
    if (!GetWorkMode(data, bWorkModeAlarm, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWorkMode error.");
        return false;
    }
#endif

    CHECK_INTERRUPT();

#if 0
    if (!GetAlarm(data, bAlarm, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }

    CHECK_INTERRUPT();

    if (!GetWssPortPower(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPower error.");
        return false;
    }

    if (!GetWssAlarm(data, bWssAlarm, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssAlarm error.");
        return false;
    }

    if (!GetOscPd(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetOscPd error.");
        return false;
    }

    if (!GetLedStatus(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetLedStatus error.");
        return false;
    }

    CHECK_INTERRUPT();

    if (!GetAlarmDuration(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarmDuration error.");
        return false;
    }

    CHECK_INTERRUPT();

    if (!GetVoltages(data, , uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetVoltages error.");
        return false;
    }
#endif
    if (!GetIl(data, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetIl error.");
        return false;
    }


    if (!GetSwitchingType(data.stCurData.ucSwitchingType, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetSwitchingType error.");
        return false;
    }

    CHECK_INTERRUPT();
#ifdef __OLSP__
    /* Note: This command will return error for unidirectional OPS. */
    GetRequestStatus(data.stBidiStatus, uiSubIdx);

    CHECK_INTERRUPT();
#endif
    memcpy(&rstData, &data, sizeof(data));

    /* Update time to ops */
    time_t tNow = time(NULL);
    struct tm tm;
    tm = *localtime(&tNow);
    if (m_uiHour != (uint32_t)tm.tm_hour) {
        SetCurrentRealTime(tm.tm_hour, tm.tm_min, tm.tm_sec);
        m_uiHour = tm.tm_hour;
    }

    return true;
}

bool
COpsLogicSpiDriver::GetPortPower(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetPortPower +++");

    std::string staMsg;

    /* Get mfginfo */
    m_stpProtoPtr->GetPortPowerCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_PORT_POWER));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetPortPower error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetPortPowerRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetPortPower ---");

    return true;
}

bool
COpsLogicSpiDriver::GetConnectionPds(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetConnectionPds +++");

    std::string staMsg;

    /* Get connection pds */
    m_stpProtoPtr->GetConnectionPdsPowerCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(COpsConnectionPd));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetConnectionPds error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetConnectionPdsPowerRsp(staRsp, rstData);


    for (int i = 0; i < OPS_CONN_CNT; i++) {
        /* LOG_WRITE(EN_LOG_NOTICE, "%d %d", i, rstData.stConnectionPds.asPd[i]); */
    }

    LOG_WRITE(EN_LOG_TRACE, "GetConnectionPds ---");

    return true;
}

bool
COpsLogicSpiDriver::GetConnectionPdThr(COpsConnectionPdThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetConnectionPdsPowerThrCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(COpsConnectionPd));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "Get connection pd thr error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetConnectionPdsPowerThrRsp(staRsp, rstData);
}

bool
COpsLogicSpiDriver::SetConnectionPdThr(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetConnectionPdPowerThrCmd(uiPdType, uiThrType, iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "SetConnectionPdThr error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::GetConnectionPdHys(COpsConnectionPdHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetConnectionPdsPowerHysCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(COpsConnectionPd));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "Get connection pd thr error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    return m_stpProtoPtr->GetConnectionPdsPowerHysRsp(staRsp, rstData);
}

bool
COpsLogicSpiDriver::SetConnectionPdHys(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetConnectionPdPowerHysCmd(uiPdType, uiHysType, iHys, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "SetConnectionPdHys error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::GetPortPowerHistory(COpsHistoryData &rstData, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "GetPortPowerHistory +++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetPortPowerHistoryCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(COpsHistoryData));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHistory cmd error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetPortPowerHistoryRsp(staRsp, rstData)) {

        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHistory rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetPortPowerHistory ---");
    return true;
}

bool
COpsLogicSpiDriver::GetAlarm(COpsCommStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetAlarm +++");

    std::string staMsg;

    m_stpProtoPtr->GetAlarmCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_ALARM));

    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetPortPower error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetAlarmRsp(staRsp, rstData);

    /* memcpy(&m_stAlarm, &rstData.stAlarm, sizeof(COpsAlarm)); */

    LOG_WRITE(EN_LOG_TRACE, "GetAlarm ---");

    return true;

}

bool
COpsLogicSpiDriver::SetPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetThr ++  %d, %d, %d", uiPort, iThr, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetPortPowerThrCmd(uiPort, uiType, iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Thr error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::SetPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetHys ++  %d, %d, %d", uiPort, iHys, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetPortPowerHysCmd(uiPort, uiType, iHys, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetPortPowerThr(CPortPowerThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetPortPowerThrCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CPortPowerThr));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetPortPowerThrRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerThrRsp error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetPortPowerHys(CPortPowerHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetPortPowerHysCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CPortPowerHys));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetPortPowerHysRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHysRsp error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::SetPortPowerDiffer(int32_t iDiffer, uint32_t uiSubIdx)
{
    return SetPortPowerThr(0, OPS_THR_TYPE_DIFFER, iDiffer, uiSubIdx);
}

bool
COpsLogicSpiDriver::GetCfgData(COpsCfgData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetAllCfgCmd(staMsg, uiSubIdx);
    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(ST_ALL_CFG));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetCfgData error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetAllCfgRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetCfgDataRsp error.");
        return false;
    }
#if 0
    if (!GetPortPowerThr(rstData.stPortPowerThr, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerThr error.");
        return false;
    }

    if (!GetPortPowerHys(rstData.stPortPowerHys, uiSubIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHys error.");
        return false;
    }
#endif
#if 0
    if (!GetWssPortPowerThr(rstData.stWssPortPowerThr)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPowerThr error.");
        return false;
    }

    if (!GetWssPortPowerHys(rstData.stWssPortPowerHys)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPowerHys error.");
        return false;
    }

    if (!GetSwitchThr(rstData.stSwitchThr)) {
        LOG_WRITE(EN_LOG_ERROR, "GetSwitchThr error.");
        return false;
    }

    if (!GetSwitchHys(rstData.stSwitchHys)) {
        LOG_WRITE(EN_LOG_ERROR, "GetSwitchHys error.");
        return false;
    }
#endif
    return true;
}


bool
COpsLogicSpiDriver::GetWorkMode(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWorkMode +++");

    std::string staMsg;

    /* Get WrokMode */
    m_stpProtoPtr->GetWorkModeCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpMfgRspHandler = __RspHandler(sizeof(ST_WORK_MODE));

    if (!__SendCmd(staMsg, stpMfgRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetWorkMode error.");
        return false;
    }

    std::string staRsp;
    stpMfgRspHandler->Copy(staRsp);
    if (!m_stpProtoPtr->GetWorkModeRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWorkModeRsp error.");
        return false;
    }

    return true;
}


bool
COpsLogicSpiDriver::SetMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetMode ++ %d, %d", uiMode, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetWorkModeCmd(uiMode, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Mode error.");
        return false;
    }

    return true;
}



bool
COpsLogicSpiDriver::SetWtr(uint32_t uiWtr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetWtr ++  %d, %d", uiWtr, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetWtrCmd(uiWtr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Wtr error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::SetHoldOffTime(uint32_t uiHoldOffTime, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetHoldOffTime ++  %d, %d", uiHoldOffTime, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetHoldOffTimeCmd(uiHoldOffTime, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hold Off Time error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::SetRevertive(uint32_t uiRevertive, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetRevertive +++  %d, %d", uiRevertive, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRevertiveCmd(uiRevertive, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Revertive error. uiRevertive = %d", uiRevertive);
        return false;
    }


    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetRevertive ---  %d, %d", uiRevertive, uiSubIdx);

    return true;
}



bool
COpsLogicSpiDriver::SetRelativeThr(int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetRelativeThr ++  %d, %d", iThr, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRelativeThrCmd(iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Thr error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetRelativeThr(CRelativeThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetRelativeThrCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CRelativeThr));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetRelativeThrCmd error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetRelativeThrRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetRelativeThrRsp error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::SetRelativeOffset(int32_t iOffset, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetRelativeOffset ++  %d, %d", iOffset, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRelativeOffsetCmd(iOffset, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Offset error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetRelativeOffset(CRelativeOffset &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetRelativeOffsetCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CRelativeOffset));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetRelativeOffsetCmd error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetRelativeOffsetRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetRelativeOffsetRsp error.");
        return false;
    }

    return true;

}


bool
COpsLogicSpiDriver::SetSwitchThr(uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetSwitchThr +++  uiPos = %d, iThr = %d, uiSubIdx = %d", uiPos, iThr, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchThrCmd(uiPos, iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetSetSwitchThr error.");
        return false;
    }


    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetSwitchThr --- uiPos = %d, iThr = %d, uiSubIdx = %d", uiPos, iThr, uiSubIdx);

    return true;

}

bool
COpsLogicSpiDriver::GetSwitchThr(COpsSwitchThr &rstThr, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetSwitchThr +++");

    std::string staMsg;

    m_stpProtoPtr->GetSwitchThrCmd(staMsg, uiSubIdx);


    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CPortPowerThr));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);


    m_stpProtoPtr->GetSwitchThrRsp(staRsp, rstThr);
    return true;
}


bool
COpsLogicSpiDriver::SetSwitchHys(int32_t uiPos, int32_t iHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetSwitchHys +++  uiPos = %d, iHys = %d, uiSubIdx = %d", uiPos, iHys, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchHysCmd(uiPos, iHys, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Offset error.");
        return false;
    }
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetSwitchHys --- uiPos = %d, iHys = %d, uiSubIdx = %d", uiPos, iHys, uiSubIdx);

    return true;
}


bool
COpsLogicSpiDriver::GetSwitchHys(COpsSwitchHys &rstHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetSwithHys +++");

    std::string staMsg;

    m_stpProtoPtr->GetSwitchHysCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(COpsSwitchHys));
    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPower error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    m_stpProtoPtr->GetSwitchHysRsp(staRsp, rstHys);

    LOG_WRITE(EN_LOG_TRACE, "GetSwithHys ---");

    return true;
}


bool
COpsLogicSpiDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::Reset ++ %d %d", uiSubIdx, ucResetType);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        LOG_WRITE(EN_LOG_NOTICE, "Ops Cold Reset. ResetIdx = %d", m_uiResetIdx);
        if (nullptr != m_stpResetBoardPtr) {
            m_stpResetBoardPtr->Reset(m_uiResetIdx);
        }
    } else {

        std::string staMsg;
        m_stpProtoPtr->SoftResetCmd(staMsg);

        if (!__SendSetCmd(staMsg)) {
            LOG_WRITE(EN_LOG_ERROR, "Ops Rset error.");
            return false;
        }
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::Reset --");

    return true;
}

bool
COpsLogicSpiDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = nullptr;
    uint8_t *pucDataPos = nullptr;
    uint32_t ulWriteLen = 0;
    uint32_t ulLeaveLen = 0;
    uint32_t ulCrc = 0;

    std::string staMsg;

    if ((nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    /* start upgrade */

    ulCrc = UTIL_Crc32Calc(pucBuffer, iFileLen);
    m_stpProtoPtr->StartUpgCmd(staMsg, iFileLen, ulCrc);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Thr error.");
        return false;
    }

    /* send data */
    pucDataPos = pucBuffer;
    ulLeaveLen = iFileLen;

    while (ulLeaveLen > 0) {

        if (ulLeaveLen > DEF_PER_PACKET_LEN) {
            ulWriteLen = DEF_PER_PACKET_LEN;
        } else {
            ulWriteLen = ulLeaveLen;
        }

/*        LOG_WRITE(EN_LOG_NOTICE, "ulWriteLen = %d,ulLeaveLen = %d", ulWriteLen, ulLeaveLen); */

        std::string staDataMsg;
        m_stpProtoPtr->SendDataCmd(staDataMsg, pucDataPos, ulWriteLen);

        if (!__SendSetCmd(staDataMsg)) {
            LOG_WRITE(EN_LOG_ERROR, "Ops Send Data error.");
            return false;
        }

        pucDataPos += ulWriteLen;
        ulLeaveLen -= ulWriteLen;
    }

    /* end upgrade */
    m_stpProtoPtr->EndUpgCmd(staMsg);

    if (!__SendSetCmd(staMsg)) {

        LOG_WRITE(EN_LOG_ERROR, "Ops end upgrade cmd error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COpsLogicSpiDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;
}

bool
COpsLogicSpiDriver::GetUpgStat(COpsUpgStat &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetUpgStatCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COpsUpgStat));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetUpgStat error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetUpgStatCmdRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetUpgStatRsp error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetWssAlarm(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPower +++");

    std::string staMsg;
    m_stpProtoPtr->GetWssAlarmCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_WSS_ALARM));
    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPower error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetWssAlarmRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPower ---");

    return true;
}


bool
COpsLogicSpiDriver::GetWssPortPower(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPower +++");

    std::string staMsg;
    m_stpProtoPtr->GetWssPortPowerCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(ST_WSS_PORT_POWER));
    if (!__SendCmd(staMsg, stpRspHandler)) {

        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPower error.");
        return false;
    }

    std::string staRsp;
    stpRspHandler->Copy(staRsp);
    m_stpProtoPtr->GetWssPortPowerRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPower ---");

    return true;
}

bool
COpsLogicSpiDriver::GetWssPortPowerHistory(uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPowerHirtory +++");

    std::string staMsg;
    uint32_t uiTotalLen = DEF_WSS_PORT_HISTORY_POWER_DATA_TOTAL_LEN;
    uint32_t uiReadLen = DEF_PER_PACKET_LEN;
    uint32_t uiPacketId = 0;
    CWssPortPowerHistory stHistoryPower = {0};

    uint8_t *pucBuf = (uint8_t *)&stHistoryPower;

    while (uiTotalLen > 0) {

        uiPacketId++;
        m_stpProtoPtr->GetWssPortPowerHistoryCmd(uiPacketId, staMsg);

        uiReadLen = (uiTotalLen > 4000 ? 4000 : uiTotalLen);
        std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(sizeof(uiPacketId) + uiReadLen);

        if (!__SendCmd(staMsg, stpRspHandler)) {
            return false;
        }

        std::string staRsp;
        stpRspHandler->Copy(staRsp);
        if (!m_stpProtoPtr->GetWssPortPowerHistoryRsp(staRsp, pucBuf, uiReadLen)) {
            return false;
        }

        uiTotalLen -= uiReadLen;
        pucBuf += uiReadLen;

        CDelay::Delay(5, MS);
    }

    if (!CFile::Write("/tmp/WssHistoryPortPower", (uint8_t *)&stHistoryPower, sizeof(stHistoryPower))) {
        LOG_WRITE(EN_LOG_ERROR, "write file error. len %lu", sizeof(stHistoryPower));
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWssPortPowerHirtory ---");

    return true;
}

bool
COpsLogicSpiDriver::SetWssPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetThr ++  %d, %d, %u", uiPort, iThr, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetWssPortPowerThrCmd(uiPort, uiType, iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Thr error.");
        return false;
    }

    return true;

}


bool
COpsLogicSpiDriver::SetWssPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "SetWssPortPowerHys +++  %d, %d, %u", uiPort, iHys, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetWssPortPowerHysCmd(uiPort, uiType, iHys, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetWssPortPowerHys ---  %d, %d, %d", uiPort, iHys, uiSubIdx);

    return true;

}

bool
COpsLogicSpiDriver::GetWssPortPowerThr(CWssPortPowerThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetWssPortPowerThrCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CWssPortPowerThr));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetWssPortPowerThrRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPowerThrRsp error.");
        return false;
    }

    return true;

}

bool
COpsLogicSpiDriver::GetWssPortPowerHys(CWssPortPowerHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetWssPortPowerHysCmd(staMsg);


    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CPortPowerThr));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetWssPortPowerHysRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPowerHysRsp error.");
        return false;
    }

    return true;

}


bool
COpsLogicSpiDriver::SetWssLedMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetLedMode +++ uiMode = %d, uiSubIdx = %d", uiMode, uiSubIdx);
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    uint8_t ucMode = (uint8_t)uiMode;

    std::string staMsg;
    m_stpProtoPtr->SetWssLedModeCmd(ucMode, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetLedMode error.");
        return false;
    }


    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetLedMode --- uiMode = %d, uiSubIdx = %d", uiMode, uiSubIdx);
    return true;
}


bool
COpsLogicSpiDriver::SetCurrentRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetCurrentRealTime +++ uiSubIdx = %d", uiSubIdx);
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRealTime(uiHour, uiMinute, uiSecond, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetCurrentRealTime --- uiSubIdx = %d", uiSubIdx);
    return true;
}


bool
COpsLogicSpiDriver::GetOscPd(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetOscPdCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData.stOscPd));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetOscPd error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetOscPdRsp(staRsp, rstData);

    return true;
}

bool
COpsLogicSpiDriver::GetOscSwitchPos(COscSwitchPos &rstPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetOscSwitchPosCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COscSwitchPos));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetOscSwitchPos error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetOscSwitchPosRsp(staRsp, rstPos);

    return true;
}

bool
COpsLogicSpiDriver::SetOscSwitchPos(uint32_t uiSwitchIdx, uint32_t uiPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscSwitchPosCmd(uiSwitchIdx, uiPos, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetOscSwitchPos error.");
        return false;
    }


    return true;
}

bool
COpsLogicSpiDriver::SetLedStatus(uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedStatus uiLedIdx = %d, uiStatus= %d", uiLedIdx, uiStatus);

    std::string staMsg;
    m_stpProtoPtr->SetLedStatusCmd(uiLedIdx, uiStatus, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetLedStatus error.");
        return false;
    }


    return true;
}

bool
COpsLogicSpiDriver::SetLedMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "SetLedMode uiMode = %d", uiMode);

    std::string staMsg;
    m_stpProtoPtr->SetLedModeCmd(uiMode, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetLedMode error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::GetLedStatus(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetLedStatus +++");
    std::string staMsg;

    m_stpProtoPtr->GetLedStatusCmd(staMsg);
    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COpsLed));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetLedStatus error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetLedStatusRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetLedStatus ---");

    return true;
}

bool
COpsLogicSpiDriver::GetAlarmDuration(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetAlarmDurtaion +++");

    std::string staMsg;
    m_stpProtoPtr->GetAlarmDurationCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(ST_PORT_ALARM_DURATION));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetAlarmDuration error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetAlarmDurationRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetAlarmDurtaion ---");

    return true;
}

bool
COpsLogicSpiDriver::ClearAlarmDuration(uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDurtaion +++");

    std::string staMsg;
    m_stpProtoPtr->ClearAlarmDurationCmd(staMsg);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops ClearAlarmDuration error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDurtaion ---");

    return true;
}

bool
COpsLogicSpiDriver::GetVoltages(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    for (uint32_t i = 0; i < OPS_VOLTAGE_CNT; i++) {
        std::string staMsg;
        m_stpProtoPtr->GetVoltageCmd(staMsg, i, 0);

        std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData.stVoltage));

        if (!__SendCmd(staMsg, stpRsp)) {
            LOG_WRITE(EN_LOG_ERROR, "Ops GetVoltages error.");
            return false;
        }

        std::string staRsp;
        stpRsp->Copy(staRsp);

        m_stpProtoPtr->GetVoltageRsp(staRsp, rstData, i, 0);
    }

    return true;
}

bool
COpsLogicSpiDriver::SetAlmMask(uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx)
{

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetAlmMask +++  uiIdx = %d, iHys = %d, uiSubIdx = %d", uiIdx, iMask, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetAlmMaskCmd(uiIdx, iMask, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops SetAlmMask error.uiIdx = %d, iMask = %d, uiSubIdx = %d", uiIdx, iMask, uiSubIdx);
        return false;
    }
    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetAlmMask --- uiIdx = %d, iMask = %d, uiSubIdx = %d", uiIdx, iMask, uiSubIdx);

    return true;
}

bool
COpsLogicSpiDriver::GetAlmMask(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetAlmMaskCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData.stAlmMask));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetAlmMask error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetAlmMaskRsp(staRsp, rstData, uiSubIdx);

    return true;
}

bool
COpsLogicSpiDriver::GetIl(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetILCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData.stIl));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetIl error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetILRsp(staRsp, rstData, uiSubIdx);

    return true;
}

bool
COpsLogicSpiDriver::SetRemoteStatus(COpsReqStatus &stStatus, uint32_t uiSubIdx)
{
    SET_INTERRUPT();

    LOG_WRITE(EN_LOG_TRACE, "COpsLogicSpiDriver::SetRemoteStatus, sta = 0x%x, sig = 0x%x",
              stStatus.ucApsState, stStatus.ucRequestedSignal);

    if (nullptr == m_stpProtoPtr) {
        RELEASE_INTERRUPT();
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRemoteStatusCmd(stStatus, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        RELEASE_INTERRUPT();
        LOG_WRITE(EN_LOG_ERROR, "Ops set remote status error.");
        return false;
    }

    RELEASE_INTERRUPT();

    return true;
}

bool
COpsLogicSpiDriver::GetRequestStatus(COpsBidirectionStatus &rstData, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetRequestStatusCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops get request status error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);
    m_stpProtoPtr->GetRequestStatusRsp(staRsp, rstData);

    return true;
}

bool
COpsLogicSpiDriver::SetSwitchingType(uint8_t ucType, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchingTypeCmd(ucType, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops set switching type error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::GetSwitchingType(uint8_t &ucType, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetSwitchingTypeCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(ucType));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops get switching type error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);
    m_stpProtoPtr->GetSwitchingTypeRsp(staRsp, ucType);

    return true;
}

bool
COpsLogicSpiDriver::GetEventLog(COpsEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetEventLogCmd(staMsg, uiSubIdx);
    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COpsEventLogAllData));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetEventLog error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetEventLogRsp(staRsp, rstEventLogAllData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetEventLog error.");
        return false;
    }

    return true;
}

bool
COpsLogicSpiDriver::SendAlarm(COpsCommStatusData &rstCommData, COpsStatusData *rstData, uint32_t uiSubIdx)
{
    const int iBasePort = OPS_SUB_INDEX_DEFAULT;
    int iType = m_uiPortCnt == OPS1_OPS_CNT_MAX ? DEV_TYPE_OPS : DEV_TYPE_OPS4A + m_uiHwIdx;
    int iPayload = OPS_ID_PACK(iType, iBasePort);

    for (uint32_t i = iBasePort; i < m_uiPortCnt + iBasePort; i++) {
        iPayload = OPS_ID_PACK(iType, i);
        if (OPS_PATH_CNT != rstData[i - iBasePort].stCurData.ucSwitchPos &&
            m_ucSwitchPos[i - iBasePort] != rstData[i - iBasePort].stCurData.ucSwitchPos) {
            CEvent::SendEvent((char *)DEF_OPSD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS_SW_HAPPEND, iPayload);
            m_ucSwitchPos[i - iBasePort] = rstData[i - iBasePort].stCurData.ucSwitchPos;
            LOG_WRITE(EN_LOG_NOTICE, "Ops Switched. SwitchPos = %u", m_ucSwitchPos[i - iBasePort]);
        }

        /* Add DEF_EVENT_ID_OPS_STATE_CHANGE event for bi-direcional OPS, notify a state changed event. */
        if (m_ucBidiSwPos[i - iBasePort] != rstData[i - iBasePort].stBidiStatus.ucLocalPosition || \
            m_ucBidiSwSta[i - iBasePort] != rstData[i - iBasePort].stBidiStatus.ucLocalStatus) {
            /* Note:
                The request status and signal from bi-direcional OPS are saved in sub id to avoid
                duplicate data fetching. This will save about 20~30ms time.
             */
            iPayload += *((int *)&rstData[i - iBasePort].stBidiStatus) << 8;
            CEvent::SendEvent((char *)DEF_OPSD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS_STATE_CHANGE, iPayload);
            m_ucBidiSwPos[i - iBasePort] = rstData[i - iBasePort].stBidiStatus.ucLocalPosition;
            m_ucBidiSwSta[i - iBasePort] = rstData[i - iBasePort].stBidiStatus.ucLocalStatus;
            LOG_WRITE(EN_LOG_NOTICE, "Bidirectional OPS state changed. position = %u, state = %u", m_ucBidiSwPos[i - iBasePort], m_ucBidiSwSta[i - iBasePort]);
        }
    }

    if (memcmp(&rstCommData.stAlarm, &m_stAlarm, sizeof(COpsAlarm)) != 0) {
        CEvent::SendEvent((char *)DEF_OPSD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS, iPayload);
        memcpy(&m_stAlarm, &rstCommData.stAlarm, sizeof(COpsAlarm));
        LOG_WRITE(EN_LOG_NOTICE, "Ops Alarm!");
    }

#if 0
    if (memcmp(&rstData.stWssStaData.stAlarm, &m_stWssAlarm, sizeof(CWssAlarm)) != 0) {
        CEvent::SendEvent((char *)DEF_OPSD_EVENT_SOCKET_PATH, DEF_EVENT_ID_WSS_PORT, 0);
        memcpy(&m_stWssAlarm, &rstData.stWssStaData.stAlarm, sizeof(CWssAlarm));
        LOG_WRITE(EN_LOG_NOTICE, "Wss Alarm.");
    }
#endif

    return true;

}

std::shared_ptr<CRspHandler>
COpsLogicSpiDriver::__RspHandler(uint32_t uiLen, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_FRM_READ_HEAD) + uiLen + DEF_FRM_CRC_LEN + 64 + 3);
    stpRspHandler->SetProto(m_stpProtoPtr);
    stpRspHandler->SetRetryTimes(uiTimeOut);

    return stpRspHandler;
}

#if 0
bool
COpsLogicSpiDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (!m_stpDrvPtr->ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops read write error.");
        return false;
    }

    return true;
}
#else
bool
COpsLogicSpiDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint32_t uiRetryTimes = 0;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    for (uiRetryTimes = 0; uiRetryTimes < stpRspHandler->RetryTimes(); uiRetryTimes++) {
        if (m_stpDrvPtr->ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), stpRspHandler->Data(), stpRspHandler)) {
            stpRspHandler->Update(stpRspHandler->Len());
            if (stpRspHandler->Proc() != RSP_STATUS_OK) {
                if (stpRspHandler->InProgress()) {
                    LOG_WRITE(EN_LOG_DEBUG, "In Progress...,RetryTimes:%d", uiRetryTimes);
                    CDelay::Delay(20, MS);
                    continue;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "Spi Read error.");
                    return false;
                }
            }

            return true;
        }
    }

    if (uiRetryTimes >= stpRspHandler->RetryTimes()) {
        LOG_WRITE(EN_LOG_ERROR, "Ops device is busy. uiRetryTimes = %d", uiRetryTimes);
        return false;
    }

    return true;

}
#endif

bool
COpsLogicSpiDriver::__SendSetCmd(std::string staMsg, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(0, uiTimeOut);

    return __SendCmd(staMsg, stpRspHandler);
}


uint32_t
COpsLogicSpiDriver::GetPortCnt()
{
    return m_uiPortCnt;
}


