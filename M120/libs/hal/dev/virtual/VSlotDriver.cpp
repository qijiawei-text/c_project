#include "VSlotDriver.h"


CSlotVirtual::CSlotVirtual(uint32_t uiHwIdx, uint32_t uiHwSubIdx) :
    CSlotDriver(uiHwIdx, uiHwSubIdx),
    m_uiCardType(CARD_TYPE_UNKOWN)
{
}

CSlotVirtual::~CSlotVirtual()
{
}

bool
CSlotVirtual::IsOnline(void)
{
    return true;
}

bool
CSlotVirtual::IsPlug(void)
{
    return false;
}

bool
CSlotVirtual::ClearPlug(void)
{
    return true;
}

bool
CSlotVirtual::GetCardType(uint32_t &ruiType)
{
    if (CARD_TYPE_UNKOWN == m_uiCardType) {
        m_uiCardType = ruiType;
    } else {
        ruiType = m_uiCardType;
    }
    return true;
}

bool
CSlotVirtual::GetMfg(CMfg &rstMfg)
{
    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-LINECARD", /* acProductName */
        "VM-LINECARD", /* acModuleType */
        "1.02.0001", /* acBootVer */
        "1.02.0002", /* acSwVer */
        "1.02.0003", /* acFwVer */
        "1.01.0004", /* acFpgaRunVer */
        "1.01.0004", /* acFpga1Ver */
        "1.01.0004", /* acFpga2Ver */
        "1.01.0005", /* acCpldRunVer */
        "1.01.0005", /* acCpld1Ver */
        "1.01.0005", /* acCpld2Ver */
        "1.02", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.02", /* acPcbRev */
        "D17135XX", /* acSN */
        "1835260XXX", /* acPN */
        "2023-02-15T08:00:00+08:00", /* acMfgDate */
        "2023-02-13T08:00:00+08:00", /* acCaliDate */
        "N/A", /* acCleiCode */
        "0.02.0006", /* acSubBdlVer */
        "0.02.0007", /* acEepromVer */
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

    std::string strType(mfg.acModuleType);
    switch (m_uiCardType) {
    case CARD_TYPE_OPS:
        strType += "-OPS";
        break;
    case CARD_TYPE_PA:
    case CARD_TYPE_BA:
    case CARD_TYPE_OTA:
        strType += "-OA";
        break;
    case CARD_TYPE_OCM:
        strType += "-OCM";
        break;
    case CARD_TYPE_MUX:
        strType += "-MUX";
        break;
    default:
        break;
    }

    strncpy(rstMfg.acProductName, strType.data(), strType.length());
    strncpy(rstMfg.acModuleType, strType.data(), strType.length());

    return true;
}

bool
CSlotVirtual::GetTemp(int32_t &riTemp)
{
    riTemp = 40;
    return true;
}