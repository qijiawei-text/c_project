/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsApi.h
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
#include "DevData.h"
#include "DevApi.h"

/*************************************************/

class COpsApi : public CDevApi
{
public:
    COpsApi()
    {
    }
    ~COpsApi()
    {
    }

    int32_t GetMfg(uint32_t uiDevIdx, CMfg &rstMfg);
    int32_t Reset(uint32_t uiDevIdx, uint8_t ucResetType);
    int32_t Upgrade(uint32_t uiDevIdx, const char *c_pcFilePath);
    int32_t GetUpgadeStatus(uint32_t uiDevIdx);
    int32_t GetCommStatusData(uint32_t uiDevIdx, COpsCommStatusData &rstStatus);
    int32_t GetStatusData(uint32_t uiDevIdx, COpsStatusData &rstStatus, uint32_t uiSubIdx = 1);
    int32_t GetHistoryData(uint32_t uiDevIdx, COpsHistoryData &rstData, uint32_t uiSubIdx = 1);
    int32_t GetCfgData(uint32_t uiDevIdx, COpsCfgData &rstCfg, uint32_t uiSubIdx = 1);

    int32_t SetMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx = 1);
    int32_t SetWtr(uint32_t uiDevIdx, uint32_t uiWtr, uint32_t uiSubIdx = 1);
    int32_t SetHoldOffTime(uint32_t uiDevIdx, uint32_t uiHoldOffTime, uint32_t uiSubIdx = 1);
    int32_t SetRevertive(uint32_t uiDevIdx, uint32_t uiRevertive, uint32_t uiSubIdx = 1);
    int32_t SetRelativeThr(uint32_t uiDevIdx, int32_t iThr, uint32_t uiSubIdx = 1);
    int32_t SetRelativeOffset(uint32_t uiDevIdx, int32_t iOffset, uint32_t uiSubIdx = 1);

    int32_t SetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 1);
    int32_t SetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 1);
    int32_t SetSwitchThr(uint32_t uiDevIdx, uint32_t uiPos, int32_t iThr, uint32_t uiSubIdx = 1);
    int32_t GetSwitchThr(uint32_t uiDevIdx, COpsSwitchThr &rstThr, uint32_t uiSubIdx = 1);
    int32_t SetSwitchHys(uint32_t uiDevIdx, uint32_t uiPos, int32_t iHys, uint32_t uiSubIdx = 1);
    int32_t GetSwitchHys(uint32_t uiDevIdx, COpsSwitchHys &rstHys, uint32_t uiSubIdx = 1);

    int32_t GetWssStatusData(uint32_t uiDevIdx, CWssStatusData &rstStatus, uint32_t uiSubIdx = 1);
    int32_t SetWssPortPowerThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx = 1);
    int32_t SetWssPortPowerHys(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iHys, uint32_t uiSubIdx = 1);
    int32_t GetWssPortPowerHistory(uint32_t uiDevIdx, uint32_t uiSubIdx = 1);
    int32_t SetWssLedMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx = 1);
    int32_t GetOscSwitchPos(uint32_t uiDevIdx, COscSwitchPos &rstSwitchPos, uint32_t uiSubIdx = 1);
    int32_t SetOscSwitchPos(uint32_t uiDevIdx, COscSwitchPosSet &rstSwitchPosSet, uint32_t uiSubIdx = 1);
    int32_t SetLedStatus(uint32_t uiDevIdx, uint32_t uiLedIdx, uint32_t uiStatus, uint32_t uiSubIdx = 1);
    int32_t SetLedMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx = 1);
    int32_t GetLedStatus(uint32_t uiDevIdx, COpsLed &rstStatus, uint32_t uiSubIdx = 1);
    int32_t ClearAlarmDuration(uint32_t uiDevIdx, uint32_t uiSubIdx = 1);

    int32_t DbgSetCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 1);
    int32_t DbgClearCommFail(uint32_t uiDevIdx, uint32_t uiSubIdx = 1);

    int32_t SetConnectionPdThr(uint32_t uiDevIdx, uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, uint32_t uiSubIdx = 1);
    int32_t SetConnectionPdHys(uint32_t uiDevIdx, uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, uint32_t uiSubIdx = 1);
    int32_t SetAlmMask(uint32_t uiDevIdx, uint32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx = 1);
    int32_t GetEventLog(uint32_t uiDevIdx, COpsEventLogAllData &rstEventLogAll, uint32_t uiSubIdx = 1);
    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                                      */
    /********************************************************************************************/
    /* @ {                                                                                      */
    int32_t SetRemoteStatus(uint32_t uiDevIdx, COpsReqStatus &stStatus, uint32_t uiSubIdx = 0);
    int32_t GetRequestStatus(uint32_t uiDevIdx, COpsBidirectionStatus &stStatus, uint32_t uiSubIdx = 0);
    int32_t SetSwitchingType(uint32_t uiDevIdx, uint8_t ucType, uint32_t uiSubIdx = 0);
    /* @ }                                                                                      */

private:
    const std::array<CShmCfg, SHM_TYPE_MAX> mc_staShmCfg {{{SHM_KEY_OPS_STA,
                                                            SHM_SEM_KEY_OPS_STA,
                                                            SHM_SIZE_OPS_STA},

                                                           {SHM_KEY_OPS_CMD,
                                                            SHM_SEM_KEY_OPS_CMD,
                                                            SHM_SIZE_OPS_CMD},

                                                           {SHM_KEY_OPS_DATA,
                                                            SHM_SEM_KEY_OPS_DATA,
                                                            SHM_SIZE_OPS_DATA}}};


};






