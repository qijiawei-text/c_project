#include "VMuxDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/MuxConfig.json")

CMuxVirtual::CMuxVirtual(void) : CMux(0), CVirtualDriver("mux", CONFIG_FILE_PATH)
{
}

CMuxVirtual::~CMuxVirtual()
{
}

bool
CMuxVirtual::IsOnline(void)
{
    return true;
}

bool
CMuxVirtual::GetMfg(CMfg &rstMfg)
{
    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-MUX", /* acProductName */
        "VM-MUX", /* acModuleType */
        "N/A", /* acBootVer */
        "N/A", /* acSwVer */
        "1.00.0005", /* acFwVer */
        "N/A", /* acFpgaRunVer */
        "N/A", /* acFpga1Ver */
        "N/A", /* acFpga2Ver */
        "N/A", /* acCpldRunVer */
        "N/A", /* acCpld1Ver */
        "N/A", /* acCpld2Ver */
        "1.01", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.01", /* acPcbRev */
        "C5237XXX", /* acSN */
        "1834055XXX", /* acPN */
        "2023-02-19T08:00:00+08:00", /* acMfgDate */
        "2023-02-17T08:00:00+08:00", /* acCaliDate */
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
CMuxVirtual::GetStatusData(CMuxStatusData &rstData)
{
    int iNum = sizeof(rstData.asPdPower) / sizeof(int16_t);
    if (!m_data.isMember("pd_power") || iNum > (int)m_data["pd_power"].size()) {
        return false;
    }

    for (int i = 0; i < iNum; i++) {
        rstData.asPdPower[i] = (int16_t)m_data["pd_power"][i].asInt();
    }

    return true;
}

bool
CMuxVirtual::GetCali(CMuxCaliData &rstData)
{
    memset(&rstData, 0, sizeof(rstData));
    return true;
}