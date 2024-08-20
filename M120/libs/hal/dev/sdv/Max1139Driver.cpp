#include "Max1139Driver.h"
#include "BoardCmd.h"


#if 0
bool
CMax1139::GetVoltage(float *pdVoltage, uint32_t uiAvinNum)
{
    uint8_t ucCmd[64] = {0};
    uint16_t ucData[MAX1139_AIN_NUM] = {0};
    double dVoltage[MAX1139_AIN_NUM] = {0.0};
    /* unsigned char *pcTmp = NULL; */
    uint32_t i = 0;

    if (uiAvinNum > MAX1139_AIN_NUM) {
        uiAvinNum = MAX1139_AIN_NUM;
    }
    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    ucCmd[0] = m_ucSetup;
    ucCmd[1] = m_ucConfig;

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, ucCmd, 2, (uint8_t *)&ucData, sizeof(ucData))) {
        return false;
    }
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "GetVoltage : 0x%0x, 0x%x", ucData[0], ucData[1]);
    pcTmp = (unsigned char *)&ucData[0];
    for (i = 0; i < MAX1139_AIN_NUM; i++) {
        printf("ucData0[%d] :0x%x!\n", i, *pcTmp++);
    }
#endif
    for (i = 0; i < MAX1139_AIN_NUM; i++) {
        ucData[i] = htons(ucData[i]);
    }

    for (i = 0; i < uiAvinNum; i++) {
        ucData[i] &= 0x3FF;
        dVoltage[i] = (float)((float)ucData[i] / MAX1139_VREF_COEF) * MAX1139_VREF_VOL;
        pdVoltage[i] = dVoltage[i];
    }

    return true;

}
#else
const std::vector<CVolReg> CMax1139::msc_staVolRegs =
{
    {REG_SCC_CPLD_V1P1_ADC},    /* SCC */
    {REG_SLC0_OTDR_V5_ADC},     /* SLC1 */
    {REG_SLC1_OTDR_V5_ADC},     /* SLC2 */
};

bool
CMax1139::GetVoltage(float *pdVoltage, uint32_t uiAvinNum)
{
    uint32_t ucData[MAX1139_CNT] = {0};
    double dVoltage[MAX1139_CNT] = {0.0};
    uint32_t i = 0;

    if (uiAvinNum > MAX1139_CNT) {
        uiAvinNum = MAX1139_CNT;
    }

    if (nullptr == m_stpLogicDrvPtr || m_uiHwSubIdx > VOL_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrvPtr is nullptr or param invalid");
        return false;
    }
#if 0
    if ( VOL_SLC0 == m_uiHwSubIdx || VOL_SLC1 == m_uiHwSubIdx ) {
        Switch(m_uiHwSubIdx, 0);
        if (!m_stpLogicDrvPtr->ReadRam(msc_staVolRegs[m_uiHwSubIdx].uiReg, (uint8_t *)ucData, sizeof(ucData))) {
            SwitchClose(m_uiHwSubIdx);
            return false;
        }
        SwitchClose(m_uiHwSubIdx);
    } else {
        if (!m_stpLogicDrvPtr->ReadRam(msc_staVolRegs[m_uiHwSubIdx].uiReg, (uint8_t *)ucData, sizeof(ucData))) {
            return false;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "GetVoltage : 0x%0x, 0x%x", ucData[0], ucData[1]);
    pcTmp = (unsigned char *)&ucData[0];
    for (i = 0; i < MAX1139_CNT; i++) {
        printf("ucData0[%d] :0x%x!\n", i, *pcTmp++);
    }
#endif
    if (!m_stpLogicDrvPtr->ReadRam(msc_staVolRegs[m_uiHwSubIdx].uiReg, (uint8_t *)ucData, sizeof(ucData))) {
        return false;
    }

    for (i = 0; i < MAX1139_CNT; i++) {
        /* ucData[i] = htons(ucData[i]); */
        dVoltage[i] = ucData[i];
        pdVoltage[i] = dVoltage[i];
    }

    return true;

}


#endif
bool
CMax1139::GetVoltage(CMax1139Sta &astMax1139, uint32_t uiAvinNum)
{
    uint8_t ucCmd[64] = {0};
    uint16_t ucData[MAX1139_AIN_NUM] = {0};
    float dVoltage[MAX1139_AIN_NUM] = {0.0};
    /* unsigned char *pcTmp = NULL; */
    uint32_t i = 0;

    if (uiAvinNum > MAX1139_AIN_NUM) {
        uiAvinNum = MAX1139_AIN_NUM;
    }
    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    ucCmd[0] = m_ucSetup;
    ucCmd[1] = m_ucConfig;

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, ucCmd, 2, (uint8_t *)&ucData, sizeof(ucData))) {
        return false;
    }
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "GetVoltage : 0x%0x, 0x%x", ucData[0], ucData[1]);
    pcTmp = (unsigned char *)&ucData[0];
    for (i = 0; i < MAX1139_AIN_NUM; i++) {
        printf("ucData0[%d] :0x%x!\n", i, *pcTmp++);
    }
#endif
    for (i = 0; i < MAX1139_AIN_NUM; i++) {
        ucData[i] = htons(ucData[i]);
    }

    for (i = 0; i < uiAvinNum; i++) {
        ucData[i] &= 0x3FF;
        dVoltage[i] = (float)((float)ucData[i] / MAX1139_VREF_COEF) * MAX1139_VREF_VOL;
        astMax1139.adVoltage[i] = dVoltage[i];
        printf("ucData2[%d] :0x%x!, %.2f, %.2f\n", i, ucData[i], dVoltage[i], astMax1139.adVoltage[i]);
    }

    return true;

}

bool
CMax1139::Switch(uint32_t uiCardIdx, uint32_t uiIdx)
{
    uint8_t ucDataW = 0;

    if (nullptr == m_stpLogicDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx >= VOL_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d,uiIdx:%d", uiCardIdx, uiIdx);
        return false;
    }

    /* slc uiCardIdx :1/2/ */
    pthread_mutex_lock(&m_spi_mutex[uiCardIdx - 1]);
#if 0
    m_stpLogicDrvPtr->ReadReg(SLC_SPI_SELECT, &ucDataW);
    ucDataW &= ~(1 << (uiCardIdx - 1));
    m_stpLogicDrvPtr->WriteReg(SLC_SPI_SELECT, ucDataW);
#else
    m_stpLogicDrvPtr->WriteReg(SLC_SPI_SELECT + uiCardIdx - 1, ucDataW);
#endif
    return true;
}


bool
CMax1139::SwitchClose(uint32_t uiCardIdx)
{
    if (nullptr == m_stpLogicDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx >= VOL_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%u", uiCardIdx);
        return false;
    }

    if (0 != uiCardIdx) {
        /* slc uiCardIdx :1/2/ */
        pthread_mutex_unlock(&m_spi_mutex[uiCardIdx - 1]);
    }

    return true;
}

