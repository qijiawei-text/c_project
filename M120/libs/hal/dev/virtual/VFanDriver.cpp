#include "VFanDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/FanConfig.json")

CFanVirtual::CFanVirtual(void) : CFan(nullptr, 0, nullptr), CVirtualDriver("fan", CONFIG_FILE_PATH)
{
}

CFanVirtual::~CFanVirtual()
{

}

bool
CFanVirtual::IsOnline(void)
{
    return true;
}

bool
CFanVirtual::IsPlug(void)
{
    return false;
}

bool
CFanVirtual::ClearPlug(void)
{
    return false;
}

bool
CFanVirtual::GetMfg(CMfg &rstMfg)
{
    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-FAN", /* acProductName */
        "VM-FAN", /* acModuleType */
        "N/A", /* acBootVer */
        "N/A", /* acSwVer */
        "N/A", /* acFwVer */
        "N/A", /* acFpgaRunVer */
        "N/A", /* acFpga1Ver */
        "N/A", /* acFpga2Ver */
        "N/A", /* acCpldRunVer */
        "N/A", /* acCpld1Ver */
        "N/A", /* acCpld2Ver */
        "1.01", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.01", /* acPcbRev */
        "C6791XXX", /* acSN */
        "VM-FAN", /* acPN */
        "2023-02-15T08:00:00+08:00", /* acMfgDate */
        "2023-02-11T08:00:00+08:00", /* acCaliDate */
        "N/A", /* acCleiCode */
        "N/A", /* acSubBdlVer */
        "N/A", /* acEepromVer */
        "N/A", /* acEth0Mac */
        "N/A", /* acEth1Mac */
        "N/A", /* acEth2Mac */
        "N/A", /* acEth3Mac */
        "N/A", /* acEth4Mac */
        "N/A", /* acEth5Mac */
        "N/A", /* acEth6Mac */
        "N/A", /* acEth7Mac */
        "N/A", /* acEth8Mac */
        "N/A" /* acEth9Mac */
    };

    memcpy(&rstMfg, &mfg, sizeof(mfg));

    return true;
}

bool
CFanVirtual::GetSpeed(uint32_t &uiSpeed)
{
    if (m_data.isMember("speed")) {
        uiSpeed = m_data["speed"].asUInt();
        return true;
    }

    return false;
}

bool
CFanVirtual::SetSpeed(uint32_t uiSpeed)
{
    m_data["speed"] = uiSpeed;
    return true;
}

bool
CFanVirtual::SetFanInitFanController()
{
    return true;
}

bool
CFanVirtual::SetFanControllerEEP()
{
    return true;
}

bool
CFanVirtual::GetLed(uint32_t &uiLed)
{
    if (m_data.isMember("led_state")) {
        uiLed = m_data["led_state"].asUInt();
        return true;
    }

    return false;
}

bool
CFanVirtual::SetLed(uint32_t uiLed)
{
    if (m_data.isMember("led_set") && m_data["led_set"].asBool()) {
        m_data["led_state"] = uiLed;
        return true;
    }

    return false;
}

bool
CFanVirtual::GetResetCause(uint8_t &ucResetCause)
{
    if (m_data.isMember("reset")) {
        ucResetCause = (uint8_t)m_data["reset"].asUInt();
        return true;
    }

    ucResetCause = 0;
    return true;
}

bool
CFanVirtual::SetResetCause(uint8_t ucResetCause)
{
    m_data["reset"] = ucResetCause;
    return true;
}