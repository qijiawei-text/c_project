/*****************************************************************************************************
   FileName:      I2cSwitch.cpp
   Date:          2020-10-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-08] [1.0] [Creator]


*****************************************************************************************************/

#include "I2cSwitch.h"
#if 0
enum EI2cSwitchReg
{
    I2C_SWITCH_REG = 0x42,
};

enum EI2cSwitchVal
{
    I2C_SWITCH_VAL0 = 0x00,
    I2C_SWITCH_VAL1 = 0x10,
    I2C_SWITCH_VAL2 = 0x20,
    I2C_SWITCH_VAL3 = 0x30,
};

const std::vector<uint8_t> CI2cSwitch::msc_staVal =
{
    I2C_SWITCH_VAL0,
    I2C_SWITCH_VAL1,
};

bool
CI2cSwitch::Switch(uint32_t uiIdx)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "I2c Switch +++");
    m_stpLogicDrv->ReadReg(I2C_SWITCH_REG, &ucVal);
    LOG_WRITE(EN_LOG_DEBUG, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

    ucVal &= ~0x30;
    ucVal |= msc_staVal[uiIdx];
    LOG_WRITE(EN_LOG_DEBUG, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

    m_stpLogicDrv->WriteReg(I2C_SWITCH_REG, ucVal);
    LOG_WRITE(EN_LOG_DEBUG, "I2c Switch ---");

    return true;
}

enum EI2cSwitchReg
{
    I2C_SWITCH_REG = 0x2050,
    I2C_SWITCH_REG_SLC0 = 0x3030,
    I2C_SWITCH_REG_SLC1 = 0x3130,
    SLC_SPI_SELECT = 0x44,
};

#define I2C_SWITH_NUM_MAX 3
uint16_t I2cSwitchReg[I2C_SWITH_NUM_MAX] = {I2C_SWITCH_REG, I2C_SWITCH_REG_SLC0, I2C_SWITCH_REG_SLC1};
/*3'h0	                    3'h1	3'h2	3'h3	        3'h4	3'h5 */
/*Temperature   EEPRAM	*/
enum EI2cSwitchVal
{
    I2C_SWITCH_VAL0 = 0x0,
    I2C_SWITCH_VAL1 = 0x1,
    I2C_SWITCH_VAL2 = 0x2,
    I2C_SWITCH_VAL3 = 0x3,
    I2C_SWITCH_VAL4 = 0x4,
    I2C_SWITCH_VAL5 = 0x5,
};

const std::vector<uint8_t> CI2cSwitch::msc_staVal =
{
    I2C_SWITCH_VAL0,
    I2C_SWITCH_VAL1,
    I2C_SWITCH_VAL2,
    I2C_SWITCH_VAL3,
    I2C_SWITCH_VAL4,
    I2C_SWITCH_VAL5,
};

bool
CI2cSwitch::Switch(uint32_t uiIdx)
{
    uint8_t ucVal = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "I2c Switch +++,0x%x", I2C_SWITCH_REG);
    m_stpLogicDrv->ReadReg(I2C_SWITCH_REG, &ucVal);
    LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

    ucVal &= ~0x7;
    ucVal |= msc_staVal[uiIdx];
    LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

    m_stpLogicDrv->WriteReg(I2C_SWITCH_REG, ucVal);
    LOG_WRITE(EN_LOG_NOTICE, "I2c Switch ---");

    return true;
}

bool
CI2cSwitch::Switch(uint32_t uiCardIdx, uint32_t uiIdx)
{
    uint8_t ucVal = 0;
    uint8_t ucDataW = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx >= I2C_SWITH_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d,uiIdx:%d", uiCardIdx, uiIdx);
        return false;
    }

    if (0 == uiCardIdx) {
        /* fpc */
        LOG_WRITE(EN_LOG_NOTICE, "I2c Switch +++,0x%x", I2C_SWITCH_REG);
        m_stpLogicDrv->ReadReg(I2cSwitchReg[uiCardIdx], &ucVal);
        LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

        ucVal &= ~0x7;
        ucVal |= msc_staVal[uiIdx];
        LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

        m_stpLogicDrv->WriteReg(I2cSwitchReg[uiCardIdx], ucVal);
        LOG_WRITE(EN_LOG_NOTICE, "I2c Switch ---");

    } else {
        /* slc uiCardIdx :1/2/3/4 */
        pthread_mutex_lock(&m_spi_mutex[uiCardIdx - 1]);
        m_stpLogicDrv->ReadReg(SLC_SPI_SELECT, &ucDataW);
        ucDataW &= ~(1 << (uiCardIdx - 1));
        m_stpLogicDrv->WriteReg(SLC_SPI_SELECT, ucDataW);
        usleep(10 * 1000);

        LOG_WRITE(EN_LOG_NOTICE, "I2c Switch +++,0x%x", I2C_SWITCH_REG);
        m_stpLogicDrv->ReadReg(I2cSwitchReg[uiCardIdx], &ucVal);
        LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

        ucVal &= ~0x7;
        ucVal |= msc_staVal[uiIdx];
        LOG_WRITE(EN_LOG_NOTICE, "uiIdx = %d ucVal = 0X%02X", uiIdx, ucVal);

        m_stpLogicDrv->WriteReg(I2cSwitchReg[uiCardIdx], ucVal);
        LOG_WRITE(EN_LOG_NOTICE, "I2c Switch ---");

    }

    return true;
}


bool
CI2cSwitch::SwitchClose(uint32_t uiCardIdx)
{
    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx >= I2C_SWITH_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%u", uiCardIdx);
        return false;
    }

    if (0 != uiCardIdx) {
        /* slc uiCardIdx :1/2/3/4 */
        pthread_mutex_unlock(&m_spi_mutex[uiCardIdx - 1]);
    }

    return true;
}
#else
#define I2C_SWITH_NUM_MAX 2
bool
CI2cSwitch::Switch(uint32_t uiIdx)
{

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    return true;
}

bool
CI2cSwitch::Switch(uint32_t uiCardIdx, uint32_t uiIdx)
{
    uint8_t ucDataW = 0;

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx > I2C_SWITH_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%d,uiIdx:%d", uiCardIdx, uiIdx);
        return false;
    }

    if (0 == uiCardIdx) {
        /* scc */
        LOG_WRITE(EN_LOG_NOTICE, "SCC I2c Switch +++");
        LOG_WRITE(EN_LOG_NOTICE, "SCC I2c Switch ---");
    } else {
        /* slc uiCardIdx :1/2 */
        LOG_WRITE(EN_LOG_NOTICE, "SLC I2c Switch +++,0x%x", uiCardIdx);
        pthread_mutex_lock(&m_spi_mutex[uiCardIdx - 1]);
#if 0		
        m_stpLogicDrv->ReadReg(SLC_SPI_SELECT, &ucDataW);
        ucDataW &= ~(1 << (uiCardIdx - 1));
        m_stpLogicDrv->WriteReg(SLC_SPI_SELECT, ucDataW);
#else
        m_stpLogicDrv->WriteReg(SLC_SPI_SELECT + uiCardIdx - 1, ucDataW);
#endif
        usleep(10 * 1000);
        LOG_WRITE(EN_LOG_NOTICE, "SLC I2c Switch ---,0x%x", uiCardIdx);
    }

    return true;
}


bool
CI2cSwitch::SwitchClose(uint32_t uiCardIdx)
{
    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr");
        return false;
    }

    if (uiCardIdx > I2C_SWITH_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "para err,uiCardIdx:%u", uiCardIdx);
        return false;
    }

    if (0 != uiCardIdx) {
        /* slc uiCardIdx :1/2 */
        pthread_mutex_unlock(&m_spi_mutex[uiCardIdx - 1]);
    }

    return true;
}

#endif
