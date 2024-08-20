#include "VPsuDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/PsuConfig.json")

CPsuVirtual::CPsuVirtual(void) :
    CPsu(nullptr, 0, 0, 0, nullptr, 0),
    CVirtualDriver("psu", CONFIG_FILE_PATH)
{
}

CPsuVirtual::~CPsuVirtual()
{
}

bool
CPsuVirtual::IsOnline(void)
{
    return true;
}

bool
CPsuVirtual::IsOk(void)
{
    return true;
}

bool
CPsuVirtual::IsPlug(void)
{
    return false;
}

bool
CPsuVirtual::ClearPlug(void)
{
    return false;
}

bool
CPsuVirtual::GetMfg(CMfg &rstMfg)
{
    static CMfg mfg = {
        "GOSPOWER", /* acManufacturer */
        "VM-PSU", /* acProductName */
        "VM-PSU", /* acModuleType */
        "N/A", /* acBootVer */
        "N/A", /* acSwVer */
        "N/A", /* acFwVer */
        "N/A", /* acFpgaRunVer */
        "N/A", /* acFpga1Ver */
        "N/A", /* acFpga2Ver */
        "N/A", /* acCpldRunVer */
        "N/A", /* acCpld1Ver */
        "N/A", /* acCpld2Ver */
        "R00", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "R00", /* acPcbRev */
        "G1251551NJ220600XXX", /* acSN */
        "G1251-0550WNJ", /* acPN */
        "2023-02-11T08:00:00+08:00", /* acMfgDate */
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
CPsuVirtual::GetVol(uint16_t &rusInV, uint16_t &rusOutV)
{
    if (m_data.isMember("voltage") && m_data.isMember("voltage_in")) {
        rusInV = (uint16_t)m_data["voltage_in"].asUInt();
        rusOutV = (uint16_t)m_data["voltage"].asUInt();
        return true;
    }
    return false;
}

bool
CPsuVirtual::GetCurrent(uint32_t &ruiInCurrent, uint32_t &ruiOutCurrent)
{
    if (m_data.isMember("current") && m_data.isMember("current_in")) {
        ruiInCurrent = m_data["current_in"].asUInt();
        ruiOutCurrent = m_data["current"].asUInt();
        return true;
    }
    return false;
}

bool
CPsuVirtual::GetWatts(uint32_t &ruiWatts)
{
    if (m_data.isMember("power")) {
        ruiWatts = m_data["power"].asUInt();
        return true;
    }
    return false;
}

bool
CPsuVirtual::GetTemp(int32_t &riTemp)
{
    if (m_data.isMember("temperature")) {
        riTemp = m_data["temperature"].asInt();
        return true;
    }
    return false;
}