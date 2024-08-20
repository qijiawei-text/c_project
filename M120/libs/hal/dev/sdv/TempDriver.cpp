#include "TempDriver.h"
#include "BoardCmd.h"

bool
CI2cTemp::InitTemp()
{
    if (false == m_bInit) {
        if (nullptr == m_stpI2cPtr) {
            LOG_WRITE(EN_LOG_ERROR, "Temp i2c ptr is nullptr.");
            return false;
        }

        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegConv, sizeof(m_ucRegConv))) {
            LOG_WRITE(EN_LOG_ERROR, "I2c write error. m_uiDevAddr = 0X%02x", m_uiDevAddr);
            return false;
        }

        m_bInit = true;
    }

    return true;

}


bool
CI2cTemp::GetTemp(int32_t &riTemp)
{
    UN_2B un2B;
    int16_t sTemp = 0;
    float fTemp = 0;

    memset((uint8_t *)&un2B, 0, sizeof(un2B));

    if (false == InitTemp()) {
        LOG_WRITE(EN_LOG_ERROR, "temperature sensor init fail.");
        return false;
    }

    /*if(!m_stpI2cPtr->Write(m_uiHwIdx, &m_ucRegTemp, sizeof(m_ucRegTemp)))
       {
            return false;
       }*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegTemp, sizeof(m_ucRegTemp), (uint8_t *)&un2B.usData, sizeof(un2B.usData))) {
        return false;
    }

    sTemp = (uint16_t)((un2B.ucByteH << 8) | (un2B.ucByteL & 0xff));
    sTemp = sTemp / 16;
    fTemp = (float)sTemp * 0.0625;
    fTemp = fTemp * 10;

    riTemp = (int32_t)fTemp;

    LOG_WRITE(EN_LOG_DEBUG, "riTemp = %d", riTemp);

    return true;

}
