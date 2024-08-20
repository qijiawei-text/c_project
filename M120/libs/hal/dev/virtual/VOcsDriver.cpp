#include "VOcsDriver.h"
#define CONFIG_FILE_PATH    ("/var/vevice/OcsConfig.json")

COcsVirtual::COcsVirtual(void) : COcsDriver(0), CVirtualDriver("ocs", CONFIG_FILE_PATH)
{
}

COcsVirtual::~COcsVirtual()
{

}

bool
COcsVirtual::GetSwitchChannels(COcsConnections &rstData)
{
    rstData.uiCount = m_data["source"].size();
    for (uint32_t i = 0; i < rstData.uiCount; i++) {
        rstData.astConn[i].ucId = m_data["index"][i].asInt();
        rstData.astConn[i].ucSource = m_data["source"][i].asInt();
        rstData.astConn[i].ucDestination = m_data["dest"][i].asInt();
    }

    return true;
}

bool
COcsVirtual::SetOneSwitchChannel(const COcsConnection &rstData)
{
    for (uint32_t i = 0; i < m_data["source"].size(); i++) {
        if (rstData.ucSource == m_data["source"][i].asInt()) {
            m_data["index"][i] = rstData.ucId;
            m_data["dest"][i] = rstData.ucDestination;
            break;
        }
    }

    return true;
}


bool
COcsVirtual::IsOnline(void)
{
    return true;
}

bool
COcsVirtual::GetMfg(CMfg &rstMfg)
{
    return true;
}

bool
COcsVirtual::GetStatusData(COcsStatusData &rstData)
{
    /* All connections state */
    memset(&rstData, 0, sizeof(rstData));
    return GetSwitchChannels(rstData.stConnsSta);
}

bool
COcsVirtual::GetCfgData(COcsCfgData &rstData)
{
    return true;
}

bool
COcsVirtual::SetSwitchChannels(const COcsConnections &rstData)
{
    for (uint32_t i = 0; i < rstData.uiCount; i++) {
        if (!SetOneSwitchChannel(rstData.astConn[i])) {
            return false;
        }
    }

    return true;
}

bool
COcsVirtual::GetLedState(COcsLedSta &rstData)
{
    return true;
}

bool
COcsVirtual::SetLampTestState(bool bEnable)
{
    return true;
}

bool
COcsVirtual::GetAlarm(COcsAlarm &rstAlarm)
{
    return true;
}

bool
COcsVirtual::GetCaseTemperature(COcsTempSta &rstData)
{
    return true;
}

bool
COcsVirtual::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    return true;
}

bool
COcsVirtual::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    return true;
}

bool
COcsVirtual::Commit(uint32_t uiDevIdx)
{
    return true;
}
