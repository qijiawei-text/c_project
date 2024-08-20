#include "VBoardDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/MfgInfo.json")

CBoardVirtual::CBoardVirtual(void) : CVirtualDriver("scc", CONFIG_FILE_PATH)
{
}

CBoardVirtual::~CBoardVirtual()
{
}

bool
CBoardVirtual::GetChassicMfg(CMfg &rstMfg)
{
    /* Chassic Mfg does not change during upgrade */
    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-CHASSIS", /* acProductName */
        "VM-CHASSIS", /* acModuleType */
        "1.01.0001", /* acBootVer */
        "1.01.0002", /* acSwVer */
        "1.01.0003", /* acFwVer */
        "1.01.0004", /* acFpgaRunVer */
        "1.01.0004", /* acFpga1Ver */
        "1.01.0004", /* acFpga2Ver */
        "1.01.0005", /* acCpldRunVer */
        "1.01.0005", /* acCpld1Ver */
        "1.01.0005", /* acCpld2Ver */
        "1.01", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.01", /* acPcbRev */
        "D9401XXX", /* acSN */
        "1835260XXX", /* acPN */
        "2023-02-17T08:00:00+08:00", /* acMfgDate */
        "2023-02-16T08:00:00+08:00", /* acCaliDate */
        "N/A", /* acCleiCode */
        "0.01.0006", /* acSubBdlVer */
        "0.01.0007", /* acEepromVer */
        "00:01:22:53:11:11", /* acEth0Mac */
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

#ifdef __OLSP__

    memset(rstMfg.acProductName, 0, sizeof(rstMfg.acProductName));
    strncpy(rstMfg.acProductName, "OLSP", sizeof(rstMfg.acProductName) - 1);

    memset(rstMfg.acModuleType, 0, sizeof(rstMfg.acModuleType));
    strncpy(rstMfg.acModuleType, "VM-OLSP", sizeof(rstMfg.acModuleType) - 1);

    memset(rstMfg.acConfigType, 0, sizeof(rstMfg.acConfigType));
    strncpy(rstMfg.acConfigType, "AMP", sizeof(rstMfg.acConfigType) - 1);
#endif

#if defined __HCF__ || defined __OCS__

    memset(rstMfg.acProductName, 0, sizeof(rstMfg.acProductName));
    strncpy(rstMfg.acProductName, "HCF", sizeof(rstMfg.acProductName) - 1);

    memset(rstMfg.acModuleType, 0, sizeof(rstMfg.acModuleType));
    strncpy(rstMfg.acModuleType, "VM-HCF", sizeof(rstMfg.acModuleType) - 1);

    memset(rstMfg.acConfigType, 0, sizeof(rstMfg.acConfigType));
    strncpy(rstMfg.acConfigType, "AMP", sizeof(rstMfg.acConfigType) - 1);
#endif
    return true;
}

bool
CBoardVirtual::GetSccMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::GetSccMfg +++");

    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-SCC", /* acProductName */
        "VM-SCC", /* acModuleType */
        "1.02.0001", /* acBootVer */
        "1.02.0002", /* acSwVer */
        "1.02.0003", /* acFwVer */
        "1.01.0004", /* acFpgaRunVer changed */
        "1.01.0004", /* acFpga1Ver   changed */
        "1.01.0004", /* acFpga2Ver   changed */
        "1.01.0005", /* acCpldRunVer changed */
        "1.01.0005", /* acCpld1Ver   changed */
        "1.01.0005", /* acCpld2Ver   changed */
        "1.02", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.02", /* acPcbRev */
        "D17135XX", /* acSN */
        "1835260XXX", /* acPN */
        "2023-02-17T08:00:00+08:00", /* acMfgDate */
        "2023-02-16T08:00:00+08:00", /* acCaliDate */
        "N/A", /* acCleiCode */
        "0.02.0006", /* acSubBdlVer */
        "0.02.0007", /* acEepromVer */
        "00:01:22:53:11:12", /* acEth0Mac */
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

    if (m_data.isMember("FpgaRunVer")) {

        LOG_WRITE(EN_LOG_NOTICE, " m_data[FpgaRunVer].asCString(): %s", m_data["FpgaRunVer"].asCString());
        memset(rstMfg.acFpgaRunVer, 0, sizeof(rstMfg.acFpgaRunVer));
        memset(rstMfg.acFpga1Ver, 0, sizeof(rstMfg.acFpga1Ver));
        memset(rstMfg.acFpga2Ver, 0, sizeof(rstMfg.acFpga2Ver));

        strncpy(rstMfg.acFpgaRunVer, m_data["FpgaRunVer"].asCString(), sizeof(rstMfg.acFpgaRunVer) - 1);
        strncpy(rstMfg.acFpga1Ver, m_data["FpgaRunVer"].asCString(), sizeof(rstMfg.acFpga1Ver) - 1);
        strncpy(rstMfg.acFpga2Ver, m_data["FpgaRunVer"].asCString(), sizeof(rstMfg.acFpga2Ver) - 1);

        LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFpgaRunVer: %s", rstMfg.acFpgaRunVer);

    }

    if (m_data.isMember("CpldRunVer")) {

        LOG_WRITE(EN_LOG_NOTICE, " m_data[CpldRunVer].asCString(): %s", m_data["CpldRunVer"].asCString());
        memset(rstMfg.acCpldRunVer, 0, sizeof(rstMfg.acCpldRunVer));
        memset(rstMfg.acCpld1Ver, 0, sizeof(rstMfg.acCpld1Ver));
        memset(rstMfg.acCpldRunVer, 0, sizeof(rstMfg.acCpldRunVer));

        strncpy(rstMfg.acCpldRunVer, m_data["CpldRunVer"].asCString(), sizeof(rstMfg.acCpldRunVer) - 1);
        strncpy(rstMfg.acCpld1Ver, m_data["CpldRunVer"].asCString(), sizeof(rstMfg.acCpld1Ver) - 1);
        strncpy(rstMfg.acCpld2Ver, m_data["CpldRunVer"].asCString(), sizeof(rstMfg.acCpld2Ver) - 1);

        LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acCpldRunVer: %s", rstMfg.acCpldRunVer);

    }


    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::GetSccMfg ---");
    return true;
}

bool
CBoardVirtual::GetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::GetUpgInfo +++");
    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::GetUpgInfo ---");
    return true;

}


bool
CBoardVirtual::SetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::SetUpgInfo +++");

    char acBdlName[256] = {0};
    ST_UPGD_BDL_HEAD *pstBdl = NULL;
    uint32_t ulBdlLen = 0;
    ST_UPGD_IMG_HEAD *pstImgHead = NULL;
    char pcFileName[128] = "/tmp/download/FirmwareBundle";
    char version[128] = {0};

    memset(acBdlName, 0, sizeof(acBdlName));
    if (NULL == fopen(pcFileName, "r")) {
        LOG_WRITE(EN_LOG_ERROR, "CBoardVirtual::GetUpgInfo fopen is error");
        goto cleanup;
    }

    pstBdl = (ST_UPGD_BDL_HEAD *)UTIL_MallocFromFile(pcFileName, &ulBdlLen);

    for (int j = 0; j < pstBdl->ucImgCnt; j++) {
        pstImgHead = &pstBdl->astImgHead[j];

        /* There is no more valid image */
        if (0 == pstImgHead->ucModType) {
            break;
        }
        LOG_WRITE(EN_LOG_DEBUG, "pstImgHead->ucModType %u", pstImgHead->ucModType);
        _VER_BCD_TO_XYZ_(ntohl(pstImgHead->ulImgVer), version);
        LOG_WRITE(EN_LOG_DEBUG, "pstImgHead->ucModType %s", version);

        if (pstImgHead->ucModType == 10 && m_data.isMember("FpgaRunVer")) {
            m_data["FpgaRunVer"] = version;
        }
        if (pstImgHead->ucModType == 11 && m_data.isMember("CpldRunVer")) {
            m_data["CpldRunVer"] = version;
        }
        memset(version, 0, sizeof(version));
    }
    UpdateData();
    SaveCfg();

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "CBoardVirtual::SetUpgInfo ---");
    return true;

}

bool
getBundleVer(char *pcFileName, ST_UPGD_BDL_HEAD *pstBdl)
{
    LOG_WRITE(EN_LOG_NOTICE, "getBundleVer +++");

    char acBdlName[256] = {0};
    uint32_t ulBdlLen = 0;

    memset(acBdlName, 0, sizeof(acBdlName));
    if (NULL == fopen(pcFileName, "r")) {
        LOG_WRITE(EN_LOG_ERROR, "CBoardVirtual::GetUpgInfo fopen is error");
        return false;
    }

    pstBdl = (ST_UPGD_BDL_HEAD *)UTIL_MallocFromFile(pcFileName, &ulBdlLen);
    LOG_WRITE(EN_LOG_NOTICE, "getBundleVer ---");

    return true;
}