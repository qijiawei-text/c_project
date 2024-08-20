#include "Ltc4215Driver.h"
#include "BoardCmd.h"


bool
CLtc4215::GetSenseCode(int32_t &riSenseCode)
{
    uint8_t ucSenseCode = 0;
    uint16_t usI = 0;

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegSense, sizeof(m_ucRegSense), (uint8_t *)&ucSenseCode, sizeof(ucSenseCode))) {
        return false;
    }

    usI = (uint32_t)ucSenseCode * 151 / 20;

    riSenseCode = usI;
    LOG_WRITE(EN_LOG_DEBUG, "ucSenseCode = %d, usI = %d", ucSenseCode, usI);


    return true;

}

bool
CLtc4215::GetSourceCode(int32_t &riSourceCode)
{
    uint8_t ucSourceCode = 0;
    uint16_t usV = 0;

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegSource, sizeof(m_ucRegSource), (uint8_t *)&ucSourceCode, sizeof(ucSourceCode))) {
        return false;
    }

    usV = (uint32_t)ucSourceCode * 121 / 20;

    riSourceCode = usV;
    LOG_WRITE(EN_LOG_DEBUG, "ucSourceCode = %d, usv = %d", ucSourceCode, usV);


    return true;

}

