#include "VLedDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/LedConfig.json")

CLedVirtual::CLedVirtual(void) : CLed(nullptr, 0), CVirtualDriver("led", CONFIG_FILE_PATH)
{
}

CLedVirtual::~CLedVirtual()
{
}

bool
CLedVirtual::SetState(uint32_t uiState)
{
    if (m_data.isMember("led_set") && m_data["led_set"].asBool()) {
        m_data["led_state"] = uiState;
        return true;
    }

    return false;
}

bool
CLedVirtual::GetState(uint32_t &ruiState)
{
    if (m_data.isMember("led_state")) {
        ruiState = m_data["led_state"].asUInt();
        return true;
    }

    return false;
}

bool
CLedVirtual::SetTestMode(bool bTestMode)
{
    return true;
}

bool
CLedVirtual::SetLocationTestMode(bool bTestMode)
{
    return true;
}