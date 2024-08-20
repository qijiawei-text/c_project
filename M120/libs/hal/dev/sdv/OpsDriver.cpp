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

/*==============================function=========================*/

bool
COpsUartDriver::IsOnline(void)
{

    return true;
}

bool
COpsUartDriver::GetMfg(CMfg &rstMfg)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::GetMfg ++");
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


    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "acFpga1Ver %s", rstMfg.acFpga1Ver);
    LOG_WRITE(EN_LOG_NOTICE, "acSwVer %s", rstMfg.acSwVer);

    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::GetMfg --");

    return true;
}

bool
COpsUartDriver::GetCommonStatusData(COpsCommStatusData &rstData)
{
    COpsCommStatusData data;
    bool bAlarm = false;
    memset(&data, 0, sizeof(data));
    if (!GetAlarm(data, bAlarm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }
    if (!GetLedStatus(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetLedStatus error.");
        return false;
    }
    if (!GetVoltages(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetVoltages error.");
        return false;
    }
    memcpy(&rstData, &data, sizeof(data));
    return true;
}
bool
COpsUartDriver::GetStatusData(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if (m_bCommFailDebug) {
        return false;
    }

    bool bWorkModeAlarm = false, /* bAlarm = false, */ bWssAlarm = false;
    COpsStatusData data;
    memset(&data, 0, sizeof(data));
#if 1
    if (!GetPortPower(data)) {
        LOG_WRITE(EN_LOG_NOTICE, "GetPortPower error.");
        return false;
    }
#endif

#if 1
    if (!GetWorkMode(data, bWorkModeAlarm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWorkMode error.");
        return false;
    }
#endif

#if 0
    if (!GetAlarm(data, bAlarm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarm error.");
        return false;
    }
#endif

#if 1
    if (!GetWssPortPower(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssPortPower error.");
        return false;
    }
#endif

#if 1
    if (!GetWssAlarm(data, bWssAlarm)) {
        LOG_WRITE(EN_LOG_ERROR, "GetWssAlarm error.");
        return false;
    }
#endif

#if 1
    if (!GetOscPd(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetOscPd error.");
        return false;
    }
#endif
#if 0
    if (!GetLedStatus(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetLedStatus error.");
    }

    if (!GetAlarmDuration(data)) {
        LOG_WRITE(EN_LOG_ERROR, "GetAlarmDuration error.");
    }

    if (!GetSwitchingType(data.stCurData.ucSwitchingType)) {
        LOG_WRITE(EN_LOG_ERROR, "GetSwitchingType error.");
        return false;
    }
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
COpsUartDriver::GetPortPower(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetPortPower +++");

    std::string staMsg;

    /* Get mfginfo */
    m_stpProtoPtr->GetPortPowerCmd(staMsg);

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
COpsUartDriver::GetConnectionPds(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }
    return true;
}

bool
COpsUartDriver::GetConnectionPdThr(COpsConnectionPdThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }
    return true;
}

bool
COpsUartDriver::SetConnectionPdThr(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::GetConnectionPdHys(COpsConnectionPdHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }
    return true;
}

bool
COpsUartDriver::SetConnectionPdHys(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::GetPortPowerHistory(COpsHistoryData &rstData, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "GetPortPowerHistory +++");
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetPortPowerHistoryCmd(staMsg);

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
COpsUartDriver::GetAlarm(COpsCommStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
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
COpsUartDriver::SetPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetThr ++  %d, %d, %d", uiPort, iThr, uiSubIdx);

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
COpsUartDriver::SetPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetHys ++  %d, %d, %d", uiPort, iHys, uiSubIdx);

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
COpsUartDriver::GetPortPowerThr(CPortPowerThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetPortPowerThrCmd(staMsg);

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
COpsUartDriver::GetPortPowerHys(CPortPowerHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetPortPowerHysCmd(staMsg);

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
COpsUartDriver::SetPortPowerDiffer(int32_t iDiffer, uint32_t uiSubIdx)
{
    return SetPortPowerThr(0, OPS_THR_TYPE_DIFFER, iDiffer, uiSubIdx);
}

bool
COpsUartDriver::GetCfgData(COpsCfgData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetAllCfgCmd(staMsg);
    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COpsCfgData));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    if (!m_stpProtoPtr->GetAllCfgRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "GetCfgDataRsp error.");
        return false;
    }

    if (!GetPortPowerThr(rstData.stPortPowerThr)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerThr error.");
        return false;
    }

    if (!GetPortPowerHys(rstData.stPortPowerHys)) {
        LOG_WRITE(EN_LOG_ERROR, "GetPortPowerHys error.");
        return false;
    }


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

    return true;
}


bool
COpsUartDriver::GetWorkMode(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetWorkMode +++");

    std::string staMsg;

    /* Get WrokMode */
    m_stpProtoPtr->GetWorkModeCmd(staMsg);

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
COpsUartDriver::SetMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetMode ++ %d, %d", uiMode, uiSubIdx);

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
COpsUartDriver::SetWtr(uint32_t uiWtr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetWtr ++  %d, %d", uiWtr, uiSubIdx);

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
COpsUartDriver::SetHoldOffTime(uint32_t uiHoldOffTime, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetHoldOffTime ++  %d, %d", uiHoldOffTime, uiSubIdx);

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
COpsUartDriver::SetRevertive(uint32_t uiRevertive, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetRevertive +++  %d, %d", uiRevertive, uiSubIdx);

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetRevertiveCmd(uiRevertive, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Revertive error. uiRevertive = %d", uiRevertive);
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetRevertive ---  %d, %d", uiRevertive, uiSubIdx);

    return true;
}



bool
COpsUartDriver::SetRelativeThr(int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetRelativeThr ++  %d, %d", iThr, uiSubIdx);

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
COpsUartDriver::GetRelativeThr(CRelativeThr &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    return true;

}

bool
COpsUartDriver::SetRelativeOffset(int32_t iOffset, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetRelativeOffset ++  %d, %d", iOffset, uiSubIdx);

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
COpsUartDriver::GetRelativeOffset(CRelativeOffset &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    return true;

}

bool
COpsUartDriver::SetSwitchThr(uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetSwitchThr +++  uiPos = %d, iThr = %d, uiSubIdx = %d", uiPos, iThr, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchThrCmd(uiPos, iThr, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Offset error.");
        return false;
    }


    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetSwitchThr --- uiPos = %d, iThr = %d, uiSubIdx = %d", uiPos, iThr, uiSubIdx);

    return true;

}

bool
COpsUartDriver::GetSwitchThr(COpsSwitchThr &rstThr, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetSwitchThr +++");

    std::string staMsg;

    m_stpProtoPtr->GetSwitchThrCmd(staMsg);


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
COpsUartDriver::SetSwitchHys(int32_t uiPos, int32_t iHys, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetSwitchHys +++  uiPos = %d, iHys = %d, uiSubIdx = %d", uiPos, iHys, uiSubIdx);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetSwitchHysCmd(uiPos, iHys, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Relative Offset error.");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::SetSwitchHys --- uiPos = %d, iHys = %d, uiSubIdx = %d", uiPos, iHys, uiSubIdx);

    return true;
}


bool
COpsUartDriver::GetSwitchHys(COpsSwitchHys &rstHys, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetSwithHys +++");

    std::string staMsg;

    m_stpProtoPtr->GetSwitchHysCmd(staMsg);

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
COpsUartDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::Reset ++ %d %d", uiSubIdx, ucResetType);

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
        m_stpProtoPtr->SoftResetCmd(staMsg, uiSubIdx);

        if (!__SendSetCmd(staMsg)) {
            LOG_WRITE(EN_LOG_ERROR, "Ops Rset error.");
            return false;
        }
    }

    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::Reset --");

    return true;
}

bool
COpsUartDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

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

        LOG_WRITE(EN_LOG_NOTICE, "ulLeaveLen = %d", ulLeaveLen);


        if (ulLeaveLen > DEF_PER_PACKET_LEN) {
            ulWriteLen = DEF_PER_PACKET_LEN;
        } else {
            ulWriteLen = ulLeaveLen;
        }

        LOG_WRITE(EN_LOG_NOTICE, "ulWriteLen = %d", ulWriteLen);

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

    LOG_WRITE(EN_LOG_NOTICE, "COpsUartDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COpsUartDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;
}

bool
COpsUartDriver::GetUpgStat(COpsUpgStat &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::GetWssAlarm(COpsStatusData &rstData, bool &rbAlarm, uint32_t uiSubIdx)
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
COpsUartDriver::GetWssPortPower(COpsStatusData &rstData, uint32_t uiSubIdx)
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
COpsUartDriver::GetWssPortPowerHistory(uint32_t uiSubIdx)
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
COpsUartDriver::SetWssPortPowerThr(uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetThr ++  %d, %d, %u", uiPort, iThr, uiSubIdx);

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
COpsUartDriver::SetWssPortPowerHys(uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx)
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
COpsUartDriver::GetWssPortPowerThr(CWssPortPowerThr &rstData, uint32_t uiSubIdx)
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
COpsUartDriver::GetWssPortPowerHys(CWssPortPowerHys &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;

    m_stpProtoPtr->GetWssPortPowerHysCmd(staMsg);


    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(CWssPortPowerHys));

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
COpsUartDriver::SetWssLedMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetLedMode +++ uiMode = %d, uiSubIdx = %d", uiMode, uiSubIdx);
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    uint8_t ucMode = (uint8_t)uiMode;

    std::string staMsg;
    m_stpProtoPtr->SetWssLedModeCmd(ucMode, staMsg, uiSubIdx);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }


    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetLedMode --- uiMode = %d, uiSubIdx = %d", uiMode, uiSubIdx);
    return true;
}


bool
COpsUartDriver::SetCurrentRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetCurrentRealTime +++ uiSubIdx = %d", uiSubIdx);
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

    LOG_WRITE(EN_LOG_TRACE, "COpsUartDriver::SetCurrentRealTime --- uiSubIdx = %d", uiSubIdx);
    return true;
}


bool
COpsUartDriver::GetOscPd(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetOscPdCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(rstData.stOscPd));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetOscPdRsp(staRsp, rstData);

    return true;
}

bool
COpsUartDriver::GetOscSwitchPos(COscSwitchPos &rstPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetOscSwitchPosCmd(staMsg, uiSubIdx);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COscSwitchPos));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetOscSwitchPosRsp(staRsp, rstPos);

    return true;
}

bool
COpsUartDriver::SetOscSwitchPos(uint32_t uiSwitchIdx, uint32_t uiPos, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->SetOscSwitchPosCmd(uiSwitchIdx, uiPos, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }


    return true;
}

bool
COpsUartDriver::SetLedStatus(uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "uiLedIdx = %d, uiStatus= %d", uiLedIdx, uiStatus);

    std::string staMsg;
    m_stpProtoPtr->SetLedStatusCmd(uiLedIdx, uiStatus, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }


    return true;
}

bool
COpsUartDriver::SetLedMode(uint32_t uiMode, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "uiMode = %d", uiMode);

    std::string staMsg;
    m_stpProtoPtr->SetLedModeCmd(uiMode, staMsg, uiSubIdx);

    std::string staRsp(sizeof(ST_FRM_READ_HEAD) + DEF_FRM_CRC_LEN, 0);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }

    return true;
}

bool
COpsUartDriver::GetLedStatus(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetLedStatus +++");
    std::string staMsg;

    m_stpProtoPtr->GetLedStatusCmd(staMsg);
    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(COscSwitchPos));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetLedStatusRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetLedStatus ---");

    return true;
}

bool
COpsUartDriver::GetAlarmDuration(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "GetAlarmDurtaion +++");

    std::string staMsg;
    m_stpProtoPtr->GetAlarmDurationCmd(staMsg);

    std::shared_ptr<CRspHandler> stpRsp = __RspHandler(sizeof(ST_PORT_ALARM_DURATION));

    if (!__SendCmd(staMsg, stpRsp)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops GetPortPowerThr error.");
        return false;
    }

    std::string staRsp;
    stpRsp->Copy(staRsp);

    m_stpProtoPtr->GetAlarmDurationRsp(staRsp, rstData);

    LOG_WRITE(EN_LOG_TRACE, "GetAlarmDurtaion ---");

    return true;
}

bool
COpsUartDriver::ClearAlarmDuration(uint32_t uiSubIdx)
{
    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDurtaion +++");

    std::string staMsg;
    m_stpProtoPtr->ClearAlarmDurationCmd(staMsg);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "Ops Set Hys error.");
        return false;
    }

    LOG_WRITE(EN_LOG_TRACE, "ClearAlarmDurtaion ---");

    return true;
}

bool
COpsUartDriver::GetVoltages(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::SetAlmMask(uint32_t uiIdx, int32_t iMask, uint32_t uiSubIdx)
{

    if (nullptr == m_stpProtoPtr) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::GetAlmMask(COpsCommStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }
    return true;
}

bool
COpsUartDriver::GetIl(COpsStatusData &rstData, uint32_t uiSubIdx)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    return true;
}

bool
COpsUartDriver::SetRemoteStatus(COpsReqStatus &stStatus, uint32_t uiSubIdx)
{
    return true;
}

bool
COpsUartDriver::GetRequestStatus(COpsBidirectionStatus &rstData, uint32_t uiSubIdx)
{
    return true;
}

bool
COpsUartDriver::SetSwitchingType(uint8_t ucType, uint32_t uiSubIdx)
{
    return true;
}

bool
COpsUartDriver::GetSwitchingType(uint8_t &ucType, uint32_t uiSubIdx)
{
    return true;
}

bool
COpsUartDriver::GetEventLog(COpsEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx)
{
    return true;
}

bool
COpsUartDriver::SendAlarm(COpsCommStatusData &rstCommData, COpsStatusData *rstData, uint32_t uiSubIdx)
{
    for (uint32_t i = 1; i <= m_uiPortCnt; i++) {
        if (m_ucSwitchPos[i - 1] != rstData[i - 1].stCurData.ucSwitchPos) {
            CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS_SW_HAPPEND, 0);
            m_ucSwitchPos[i - 1] = rstData[i - 1].stCurData.ucSwitchPos;
            LOG_WRITE(EN_LOG_NOTICE, "Ops Switched. SwitchPos = %d", rstData[i - 1].stCurData.ucSwitchPos);
        }
    }
    if (memcmp(&rstCommData.stAlarm, &m_stAlarm, sizeof(COpsAlarm)) != 0) {
        CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_OPS, 0);
        memcpy(&m_stAlarm, &rstCommData.stAlarm, sizeof(COpsAlarm));
        LOG_WRITE(EN_LOG_NOTICE, "Ops Alarm!");
    }
#if 0
    if (memcmp(&rstData.stWssStaData.stAlarm, &m_stWssAlarm, sizeof(CWssAlarm)) != 0) {
        CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_WSS_PORT, 0);
        memcpy(&m_stWssAlarm, &rstData.stWssStaData.stAlarm, sizeof(CWssAlarm));
        LOG_WRITE(EN_LOG_NOTICE, "Wss Alarm.");
    }
#endif
    return true;

}

std::shared_ptr<CRspHandler>
COpsUartDriver::__RspHandler(uint32_t uiLen)
{
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_FRM_READ_HEAD) + uiLen + DEF_FRM_CRC_LEN);
    stpRspHandler->SetProto(m_stpProtoPtr);

    return stpRspHandler;
}

bool
COpsUartDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
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

bool
COpsUartDriver::__SendSetCmd(std::string staMsg)
{
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(0);

    return __SendCmd(staMsg, stpRspHandler);
}

uint32_t
COpsUartDriver::GetPortCnt()
{
    return m_uiPortCnt;
}


