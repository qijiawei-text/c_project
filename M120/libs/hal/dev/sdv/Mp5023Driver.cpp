#include "Mp5023Driver.h"
#include "BoardCmd.h"

bool
CMp5023::GetStatus(CMp5023Sta &rstMp5023Sta)
{
    uint16_t sVin = 0;
    uint16_t sVOut = 0;
    uint16_t sIOut = 0;

    LOG_WRITE(EN_LOG_DEBUG, "Mp5023 GetStatus, m_uiHwSubIdx = %d, m_uiDevAddr = %d", m_uiHwSubIdx, m_uiDevAddr);

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, 0x80, &m_ucVin, sizeof(m_ucVin), (uint8_t *)&sVin, sizeof(sVin))) {
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, 0x80, &m_ucVOut, sizeof(m_ucVOut), (uint8_t *)&sVOut, sizeof(sVOut))) {
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, 0x80, &m_ucIOut, sizeof(m_ucIOut), (uint8_t *)&sIOut, sizeof(sIOut))) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "sVin = [0x%0x]  ", sVin);
    LOG_WRITE(EN_LOG_DEBUG, "sVOut = [0x%0x] ", sVOut);
    LOG_WRITE(EN_LOG_DEBUG, "sIOut = [0x%0x]", sIOut);

    /* TODO here need calc real voltage current etc. */
    rstMp5023Sta.dVIn = (double)sVin * 31.25;
    rstMp5023Sta.dVOut = (double)sVOut * 31.25;
    rstMp5023Sta.dIOut = (double)sIOut * 62.5;

    return true;

}


