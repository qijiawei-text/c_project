/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      VOcsDriver.h
   Author:        Jichen Dou
   Date:          2024-01-17
   Version:       1.0
   Description:
   Function List:

   History:
   [Jichen Dou] [2024-01-17] [1.0] [Creator]

*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "BoardCmd.h"
#include "OcsProto.h"
#include "VDriver.h"
#include "OcsDriver.h"

/*************************************************/
class COcsVirtual : public COcsDriver, public CVirtualDriver
{

public:
    COcsVirtual(void);
    virtual
    ~COcsVirtual();

    virtual bool IsOnline(void);

    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetStatusData(COcsStatusData &rstData);
    virtual bool GetCfgData(COcsCfgData &rstData);

    virtual bool GetSwitchChannels(COcsConnections &rstData);
    virtual bool SetSwitchChannels(const COcsConnections &rstData);
    virtual bool SetOneSwitchChannel(const COcsConnection &rstData);

    virtual bool GetLedState(COcsLedSta &rstData);
    virtual bool SetLampTestState(bool bEnable);

    virtual bool GetAlarm(COcsAlarm &rstAlarm);

    virtual bool GetCaseTemperature(COcsTempSta &rstData);

    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0);
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath);
    virtual bool Commit(uint32_t uiDevIdx);
};
