#include "PsuDriver.h"
#include <math.h>

#define PWR_ID_REG_ADDR_VIN             0x88
#define PWR_ID_REG_ADDR_IIN             0x89
#define PWR_ID_REG_ADDR_VOUT            0x8B
#define PWR_ID_REG_ADDR_IOUT            0x8C
#define PWR_ID_REG_ADDR_TEMP                    0x8D
#define PWR_ID_REG_ADDR_POUT            0x96
#define PWR_ID_REG_ADDR_PIN             0x97

#define DEF_WEIGHT_HUNDREDTH            (100)
#define DEF_WEIGHT_TENTH                (10)

#define PWR_ID_E2ROM_ADDR_MANUFACTURER_NAME       (0x0C)   /*length: 5*/
#define PWR_ID_E2ROM_ADDR_PRODUCT_NAME            (0x12)   /*length: 14*/
#define PWR_ID_E2ROM_ADDR_PN                      (0x21)   /*length: 14*/
#define PWR_ID_E2ROM_ADDR_PRODUCT_VERSION         (0x30)   /*length: 4*/
#define PWR_ID_E2ROM_ADDR_SN                      (0x35)   /*length: 14*/


typedef struct PWR_MFG_FIELD_LIST_st
{
    uint32_t uiAddr;
    uint32_t uiReadLen;
    char *pcValue;
} PWR_MFG_FIELD_LIST_ST;

typedef enum PSU_TYPE_en {
    TYPE_ACDC,
    TYPE_DCDC,
    TYPE_MAX
} PSU_TYPE_EN;

struct CPsuMfgExtra
{
    char acUnitName[MFG_INFO_LEN_MAX];
    char acFCPN[MFG_INFO_LEN_MAX];
    char acFCIss[MFG_INFO_LEN_MAX];
    char acFujitsuPN[MFG_INFO_LEN_MAX];
    char acModel[MFG_INFO_LEN_MAX];
};
CPsuMfgExtra msc_staPsuMfgExtra[TYPE_MAX] =
{
    {"PIL3-4PP3", "FC95594PP3", "1", "TA09068-C418", "NR10-PSU550AC-01"},               /* ACDC */
    {"PIL3-4PP4", "FC95594PP4", "1", "TA09068-C417", "NR10-PSU550DC-01"}                /* DCDC */
};

bool
CPsu::GetMfg(CMfg &rstMfg)
{
    uint8_t aucTmp[MFG_INFO_LEN_MAX] = {0};
    uint8_t ucMfgCmd = 0x9D;
    uint8_t aucMfgDate[16] = {0};
    int32_t iNum;
    uint32_t uiManuLen = 8, uiModelLen = 14, uiPnLen = 20, uiVerLen = 3, uiSnLen = 19;     /* GosPower Sn len is 19 */
    uint32_t uiManuAddr = 0x0C;
    uint32_t uiModelAddr = uiManuAddr + uiManuLen + 1;
    uint32_t uiPnAddr = uiModelAddr + uiModelLen + 1;
    uint32_t uiVerAddr = uiPnAddr + uiPnLen + 1;
    uint32_t uiSnAddr = uiVerAddr + uiVerLen + 1;
    uint8_t ucAcdc = TYPE_ACDC;

    memset(&rstMfg, 0, sizeof(rstMfg));


    LOG_WRITE(EN_LOG_NOTICE, "PSU #%d GetMfg +++", m_uiPsuBoardIdx);
    if (!GetMfgItem(PWR_ID_E2ROM_ADDR_MANUFACTURER_NAME, (uint8_t *)rstMfg.acManufacturer, uiManuLen)) {
        LOG_WRITE(EN_LOG_NOTICE, "Read psu manufacture error.");
        return false;
    }

    std::string staManufacture((char *)rstMfg.acManufacturer, uiManuLen);

    LOG_WRITE(EN_LOG_NOTICE, "mfg: %s", staManufacture.c_str());
    std::size_t found = staManufacture.find("DELTA");
    if (found != std::string::npos) {
        memset(rstMfg.acManufacturer, 0, MFG_INFO_LEN_MAX);
        memcpy(rstMfg.acManufacturer, "DELTA", 5);

        uiManuLen = 5;
        uiModelLen = 14;
        uiPnLen = 14;
        uiVerLen = 4;
        uiSnLen = 14;

        uiModelAddr = PWR_ID_E2ROM_ADDR_PRODUCT_NAME;
        uiPnAddr = PWR_ID_E2ROM_ADDR_PN;
        uiVerAddr = PWR_ID_E2ROM_ADDR_PRODUCT_VERSION;
        uiSnAddr = PWR_ID_E2ROM_ADDR_SN;
    }

    found = staManufacture.find("BOCO");
    if (found != std::string::npos) {
        memset(rstMfg.acManufacturer, 0, MFG_INFO_LEN_MAX);
        memcpy(rstMfg.acManufacturer, "BOCO ", 5);

        uiManuLen = 5;
        uiModelLen = 10;
        uiPnLen = 15;
        uiVerLen = 2;
        uiSnLen = 15;

        uiModelAddr = uiManuAddr + uiManuLen + 1;
        uiPnAddr = uiModelAddr + uiModelLen + 1;
        uiVerAddr = uiPnAddr + uiPnLen + 1;
        uiSnAddr = uiVerAddr + uiVerLen + 1;
    }

    PWR_MFG_FIELD_LIST_ST stFieldList[] = {{uiModelAddr, uiModelLen, rstMfg.acProductName},
                                           {uiPnAddr, uiPnLen, rstMfg.acPN},
                                           {uiVerAddr, uiVerLen, rstMfg.acHwVer},
                                           {uiSnAddr, uiSnLen, rstMfg.acSN}};

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d m_stpI2cPtr is nullptr. ", m_uiPsuBoardIdx);
        return false;
    }

    iNum = sizeof(stFieldList) / sizeof(stFieldList[0]);
    for (int32_t i = 0; i < iNum; i++) {
        memset(aucTmp, 0, sizeof(aucTmp));

        if (false == GetMfgItem(stFieldList[i].uiAddr, aucTmp, stFieldList[i].uiReadLen)) {
            LOG_WRITE(EN_LOG_ERROR, "Get PSU mfg item error. uiAddr = 0X%02x, uiReadLen = %d", stFieldList[i].uiAddr, stFieldList[i].uiReadLen);
            return false;
        }

        snprintf(stFieldList[i].pcValue, sizeof(aucTmp), "%s", aucTmp);

        LOG_WRITE(EN_LOG_NOTICE, "stFieldList[%d].pcValue = %s", i, stFieldList[i].pcValue);
    }

    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, (uint8_t *)&ucMfgCmd, 1, aucMfgDate, 9)) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = %d", m_uiDevAddr);
        /* return false; */
    }

    snprintf(rstMfg.acMfgDate, sizeof(aucMfgDate) - 1, "%s", aucMfgDate + 1);

    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acManufacturer = %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acProductName = %s", rstMfg.acProductName);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acPN = %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSN = %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acHwVer = %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acMfgDate = %s", rstMfg.acMfgDate);
#ifdef __NR1004__
    /*  rstMfg.acManufacturer = GOSPOWER
        rstMfg.acProductName = G1232-0550WNA
        rstMfg.acPN = XXXXXXXXXXXXXXXXXXXX
        rstMfg.acSN = G1232551NA30600004
        rstMfg.acHwVer = R00
        rstMfg.acMfgDate = 20230704 */
    if (0 == strcmp(rstMfg.acManufacturer, "GOSPOWER")) {
        if ( 0 == strncmp(rstMfg.acProductName, "G1232", strlen("G1232"))) {          /* dcdc G1232-550WNA */
            ucAcdc = TYPE_DCDC;

            /* OC2A-358 the PartNo of DC power is 'PN0D', just use the rstMfg.acProductName  */
            memcpy(rstMfg.acPN, rstMfg.acProductName, sizeof(rstMfg.acProductName));
        } else if ( 0 == strncmp(rstMfg.acProductName, "G1251", strlen("G1251"))) {   /* acdc G1251-0550WNJ */
            ucAcdc = TYPE_ACDC;
        }
    } else if (0 == strcmp(rstMfg.acManufacturer, "DELTA")) {
        if ( 0 == strncmp(rstMfg.acProductName, "DPS-550AB-37", strlen("DPS-550AB-37"))) {          /* dcdc DPS-550AB-37A */
            ucAcdc = TYPE_DCDC;
        } else if ( 0 == strncmp(rstMfg.acProductName, "DPS-550AB-22", strlen("DPS-550AB-22"))) {   /* acdc DPS-550AB-22 J */
            ucAcdc = TYPE_ACDC;
        }
    }

    memcpy(rstMfg.acUnitName, msc_staPsuMfgExtra[ucAcdc].acUnitName, sizeof(rstMfg.acUnitName));
    memcpy(rstMfg.acFCPN, msc_staPsuMfgExtra[ucAcdc].acFCPN, sizeof(rstMfg.acFCPN));
    memcpy(rstMfg.acFCIss, msc_staPsuMfgExtra[ucAcdc].acFCIss, sizeof(rstMfg.acFCIss));
    memcpy(rstMfg.acFujitsuPN, msc_staPsuMfgExtra[ucAcdc].acFujitsuPN, sizeof(rstMfg.acFujitsuPN));
    memcpy(rstMfg.acProductName, msc_staPsuMfgExtra[ucAcdc].acModel, sizeof(rstMfg.acProductName));
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acUnitName = %s", rstMfg.acUnitName);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFCPN = %s", rstMfg.acFCPN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFCIss = %s", rstMfg.acFCIss);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFujitsuPN = %s", rstMfg.acFujitsuPN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acProductName = %s", rstMfg.acProductName);
#endif
    LOG_WRITE(EN_LOG_NOTICE, "PSU #%d acdc=%u GetMfg ---", m_uiPsuBoardIdx, ucAcdc);

    return true;
}

bool
CPsu::GetMfgItem(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    uint8_t aucBufW[2] = {0};
    uint8_t ucLenW = 0;

    /* Access bit1                                                              */
    aucBufW[0] = uiAddr & 0xff;
    ucLenW = 1;

    LOG_WRITE(EN_LOG_DEBUG, "I2c read m_uiDevEeAddr = 0X%02x uiAddr = 0X%02x", m_uiDevEeAddr, uiAddr);

    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevEeAddr, aucBufW, ucLenW, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "I2c read error. m_uiDevEeAddr = 0X%02x, uiAddr = 0X%02x", m_uiDevAddr, uiAddr);
        return false;
    }

    return true;
}

bool
CPsu::IsOnline(void)
{
    bool bOnline = false;

    LOG_WRITE(EN_LOG_DEBUG, "CPsu::IsOnline +++");

    if (nullptr == m_stpPsuBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d driver ptr is nullptr", m_uiPsuBoardIdx);
        return false;
    }

    bOnline = m_stpPsuBoardPtr->IsOnline(m_uiPsuBoardIdx);


    LOG_WRITE(EN_LOG_DEBUG, "CPsu::IsOnline +++");

    return bOnline;
}

bool
CPsu::IsOk(void)
{
    if (nullptr == m_stpPsuBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d driver ptr is nullptr", m_uiPsuBoardIdx);
        return false;
    }

    return m_stpPsuBoardPtr->IsOk(m_uiPsuBoardIdx);
}

bool
CPsu::IsPlug(void)
{
    if (nullptr == m_stpPsuBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d driver ptr is nullptr", m_uiPsuBoardIdx);
        return false;
    }


    if (false == m_stpPsuBoardPtr->IsPlug(m_uiPsuBoardIdx)) {
        return false;
    }

    return true;

}

bool
CPsu::ClearPlug(void)
{
    if (nullptr == m_stpPsuBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d driver ptr is nullptr", m_uiPsuBoardIdx);
        return false;
    }

    if (false == m_stpPsuBoardPtr->ClearPlug(m_uiPsuBoardIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "PSU #%d clear plug flag error.", m_uiPsuBoardIdx);
        return false;
    }

    return true;
}


bool
CPsu::GetStatusData(CPowerSta & rstData)
{
    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetStatusData +++ ", m_uiPsuBoardIdx);
    bool bOnline = IsOnline();
    bool bOk = IsOk();

    rstData.bIsOnline = bOnline;
    rstData.bIsFailure = !bOk;

    if (bOnline && bOk) {
        if (!GetVol(rstData.usInVol, rstData.usOutVol)) {

            LOG_WRITE(EN_LOG_ERROR, "GetVol error");
            return false;
        }

        if (!GetCurrent(rstData.uiInCurrent, rstData.uiOutCurrent)) {

            LOG_WRITE(EN_LOG_ERROR, "GetCurrent error");
            return false;
        }

        if (!GetWatts(rstData.uiWatts)) {

            LOG_WRITE(EN_LOG_ERROR, "GetWatts error");
            return false;
        }

        if (!GetTemp(rstData.iTemp)) {

            LOG_WRITE(EN_LOG_ERROR, "GetTemp error");
            return false;
        }
    }

    return true;
}

bool
CPsu::GetVol(uint16_t &rusInV, uint16_t &rusOutV)
{
    uint8_t aucBufW[2] = {0};
    uint16_t usTmp = 0;
    int8_t cN = 0;
    float fVal = 0.0f;

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetVol +++ ", m_uiPsuBoardIdx);

    aucBufW[0] = PWR_ID_REG_ADDR_VIN;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = 0X%02x aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "m_uiI2cIdx = 0X%02x, m_uiDevAddr = 0X%02x", m_uiI2cIdx, m_uiDevAddr);

    rusInV = CalcX(usTmp, DEF_WEIGHT_HUNDREDTH);
    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) rusInV:%d", m_uiPsuBoardIdx, rusInV);

    aucBufW[0] = PWR_ID_REG_ADDR_VOUT;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = %d aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    cN = -9;
    /* X = Y * 2^N                                                              */
    fVal = (float)usTmp * pow(2, cN);
    rusOutV = fVal * DEF_WEIGHT_HUNDREDTH;

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) OutV:%d ", m_uiPsuBoardIdx, rusOutV);

    return true;
}


bool
CPsu::GetCurrent(uint32_t &ruiInCurrent, uint32_t &ruiOutCurrent)
{
    uint8_t aucBufW[2] = {0};
    uint16_t usTmp = 0;

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetCurrent +++ ", m_uiPsuBoardIdx);

    aucBufW[0] = PWR_ID_REG_ADDR_IOUT;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = 0X%02x aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    ruiOutCurrent = CalcX(usTmp, DEF_WEIGHT_HUNDREDTH);

    aucBufW[0] = PWR_ID_REG_ADDR_IIN;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = 0X%02x aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "m_uiI2cIdx = 0X%02x, m_uiDevAddr = 0X%02x", m_uiI2cIdx, m_uiDevAddr);

    ruiInCurrent = CalcX(usTmp, DEF_WEIGHT_HUNDREDTH);
    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) ruiInCurrent = %d, ruiOutCurrent = %d", m_uiPsuBoardIdx, ruiInCurrent, ruiOutCurrent);

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetCurrent ---", m_uiPsuBoardIdx);

    return true;
}

bool
CPsu::GetWatts(uint32_t &ruiWatts)
{
    uint8_t aucBufW[2] = {0};
    uint16_t usTmp = 0;

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetWatts +++ ", m_uiPsuBoardIdx);

    aucBufW[0] = PWR_ID_REG_ADDR_POUT;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = 0X%02x aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "m_uiI2cIdx = 0X%02x, m_uiDevAddr = 0X%02x", m_uiI2cIdx, m_uiDevAddr);

    ruiWatts = CalcX(usTmp, DEF_WEIGHT_HUNDREDTH);
    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) ruiWatts = %d", m_uiPsuBoardIdx, ruiWatts);
    /* printf("PSU(%d) ruiWatts = %d\n", m_uiPsuBoardIdx, ruiWatts); */

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetWatts ---", m_uiPsuBoardIdx);

    return true;
}


bool
CPsu::GetTemp(int32_t &riTemp)
{
    uint8_t aucBufW[2] = {0};
    uint16_t usTmp = 0;

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetTemp +++ ", m_uiPsuBoardIdx);

    aucBufW[0] = PWR_ID_REG_ADDR_TEMP;
    if (false == m_stpI2cPtr->Read(m_uiI2cIdx, m_uiDevAddr, aucBufW, 1, (uint8_t *)&usTmp, sizeof(usTmp))) {
        LOG_WRITE(EN_LOG_ERROR, "Read error. m_uiDevAddr = 0X%02x aucBufW[0] = 0X%02x", m_uiDevAddr, aucBufW[0]);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "m_uiI2cIdx = 0X%02x, m_uiDevAddr = 0X%02x", m_uiI2cIdx, m_uiDevAddr);

    riTemp = CalcX(usTmp, DEF_WEIGHT_TENTH);
    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) riTemp = %d", m_uiPsuBoardIdx, riTemp);

    LOG_WRITE(EN_LOG_DEBUG, "PSU(%d) GetTemp ---", m_uiPsuBoardIdx);

    return true;
}



uint16_t
CPsu::CalcX(uint16_t usVal, uint16_t usRatio)
{
    int baseY = 0;
    int baseN = 0;
    float fVal = 0.0f;

    baseY = usVal & 0x07ff;
    baseN = (usVal >> 11) - 32;

    /* X = Y * 2^N */
    fVal = (float)baseY * pow(2, baseN);

    return fVal * usRatio;
}

