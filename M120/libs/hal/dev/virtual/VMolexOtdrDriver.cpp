#include "VMolexOtdrDriver.h"
#include "VBoardDriver.h"
/*need to be changed */

COtdrVirtual::~COtdrVirtual()
{

}
bool
COtdrVirtual::IsOnline(void)
{
    return true;
}
bool
COtdrVirtual::GetMfg(CMfg &rstMfg)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::GetMfg +++");
    static CMfg mfg = {
        "MOLEX", /* acManufacturer */
        "VM-OTDR", /* acProductName */
        "VM-OTDR", /* acModuleType */
        "1.00.0002", /* acBootVer */
        "1.00.0004", /* acSwVer */
        "1.00.0004", /* acFwVer */
        "1.01.0004", /* acFpgaRunVer  */
        "1.01.0004", /* acFpga1Ver    */
        "1.01.0004", /* acFpga2Ver    */
        "1.01.0005", /* acCpldRunVer  */
        "1.01.0005", /* acCpld1Ver    */
        "1.01.0005", /* acCpld2Ver    */
        "1.00.0004", /* acHwVer */
        "N/A", /* acHwFuncRev */
        "1.02", /* acPcbRev */
        "SN9900000XXX", /* acSN */
        "OTDR-W1510-D40-P15-LC/UPC", /* acPN */
        "2022-04-04T08:00:00+08:00", /* acMfgDate */
        "2022-04-04T08:00:00+08:00", /* acCaliDate */
        "N/A", /* acCleiCode */
        "0.02.0006", /* acSubBdlVer */
        "0.02.0007", /* acEepromVer */
        "01:02:03:04:05:06", /* acEth0Mac */
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

    if (m_data.isMember("SWversion")) {

        LOG_WRITE(EN_LOG_DEBUG, " m_data[SWversion].asCString(): %s", m_data["SWversion"].asCString());

        memset(rstMfg.acSwVer, 0, sizeof(rstMfg.acSwVer));
        memset(rstMfg.acBootVer, 0, sizeof(rstMfg.acBootVer));
        memset(rstMfg.acHwVer, 0, sizeof(rstMfg.acHwVer));
        memset(rstMfg.acFwVer, 0, sizeof(rstMfg.acFwVer));

        strncpy(rstMfg.acSwVer, m_data["SWversion"].asCString(), sizeof(rstMfg.acSwVer) - 1);
        strncpy(rstMfg.acBootVer, m_data["SWversion"].asCString(), sizeof(rstMfg.acBootVer) - 1);
        strncpy(rstMfg.acHwVer, m_data["SWversion"].asCString(), sizeof(rstMfg.acHwVer) - 1);
        strncpy(rstMfg.acFwVer, m_data["SWversion"].asCString(), sizeof(rstMfg.acFwVer) - 1);

    }
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::GetMfg ---");
    return true;
}

bool
COtdrVirtual::GetStatusData(COtdrStatusData &rstData)
{
    return true;
}
bool
COtdrVirtual::GetCfg(COtdrCfgData &rstCfgData)
{
    return true;
}
bool
COtdrVirtual::GetEvent(COtdrStatusData &rstData)
{
    /* Fake event info */
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::GetEvent +++");
    rstData.astEventLine[0].ulEventId = 0;
    rstData.astEventLine[0].dEventLoss = 1.0;
    rstData.astEventLine[0].dLocation = 2.0;
    rstData.astEventLine[0].dReflectance = 3.0;
    rstData.astEventLine[0].dTotalLoss = 4.0;
    rstData.astEventLine[0].enEventType = OTDR_EVENT_TYPE_REF;
    rstData.stEventTop.ulCnt = 1;
    rstData.stEventTop.dFiberLen = 2.0;
    rstData.stEventTop.dReturnLoss = 3.0;
    rstData.stEventTop.dTotalLoss = 4.0;
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::GetEvent ---");
    return true;
}
bool
COtdrVirtual::GetSor()
{
    /* Copy true file from cfg/filesystem/var/ Here pass */
    return true;
}

bool
COtdrVirtual::StartScan(void)
{
    /* wait for 30 seconds to see measuring */
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::StartScan +++");
    sleep(30);
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::StartScan ---");
    return true;
}
bool
COtdrVirtual::SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution)
{
    return true;
}
bool
COtdrVirtual::SetUserIOR(double dUserIOR)
{
    return true;
}
bool
COtdrVirtual::SetMeasuringTime(uint32_t ulMeasuringTime)
{
    return true;
}
bool
COtdrVirtual::SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr)
{
    return true;
}
bool
COtdrVirtual::SetScanMode(uint32_t ulScanMode)
{
    return true;
}
bool
COtdrVirtual::GetTemp(COtdrTemp &rstTemp)
{
    return true;
}

bool
COtdrVirtual::Reset(uint8_t ucResetType)
{
    return true;
}

bool
COtdrVirtual::Upgrade(const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::Upgrade +++");

    ST_UPGD_BDL_HEAD *pstBdl = NULL;
    ST_UPGD_IMG_HEAD *pstImgHead = NULL;
    char pcFileName[128] = "/tmp/download/FirmwareBundle";
    char version[128] = {0};

    char acBdlName[256] = {0};
    uint32_t ulBdlLen = 0;

    memset(acBdlName, 0, sizeof(acBdlName));
    if (NULL == fopen(pcFileName, "r")) {
        LOG_WRITE(EN_LOG_ERROR, "COtdrVirtual::Upgrade fopen is error");
        goto cleanup;
    }

    pstBdl = (ST_UPGD_BDL_HEAD *)UTIL_MallocFromFile(pcFileName, &ulBdlLen);

    for (int j = 0; j < pstBdl->ucImgCnt; j++) {
        pstImgHead = &pstBdl->astImgHead[j];

        /* There is no more valid image */
        if (0 == pstImgHead->ucModType) {
            break;
        }
        _VER_BCD_TO_XYZ_(ntohl(pstImgHead->ulImgVer), version);

        if (pstImgHead->ucModType == 20 && m_data.isMember("SWversion")) {
            LOG_WRITE(EN_LOG_DEBUG, "version of OTDR: %s", version);
            m_data["SWversion"] = version;
        }

        memset(version, 0, sizeof(version));
    }
    UpdateData();
    SaveCfg();

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "COtdrVirtual::Upgrade ---");
    return true;
}

/* debug */
bool
COtdrVirtual::DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    return true;
}
bool
COtdrVirtual::DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    return true;
}
bool
COtdrVirtual::SetStartOffset(COtdrStartOffset *pstStartOffset)
{
    return true;
}
bool
COtdrVirtual::GetStartOffset(COtdrStartOffset &rstStartOffset)
{
    return true;
}