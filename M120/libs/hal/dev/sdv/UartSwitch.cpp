/*****************************************************************************************************
   FileName:      UartSwitch.cpp
   Date:          2020-09-18
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-18] [1.0] [Creator]


*****************************************************************************************************/

#include "UartSwitch.h"

enum EUartSwitchVal
{
    UART_SWITCH_VAL0 = 0x00,            /* PA*/
    UART_SWITCH_VAL1 = 0x01,            /* BA*/
    UART_SWITCH_VAL2 = 0x02,            /* WSS1*/
    UART_SWITCH_VAL3 = 0x03,            /* OCM */
    UART_SWITCH_VAL4 = 0x04,            /* EXTEND */
    UART_SWITCH_VAL5 = 0x05,            /* OTDR */
    UART_SWITCH_VAL6 = 0x06,            /* WSS2 */
    UART_SWITCH_VAL7 = 0x07,
};

const std::vector<uint8_t> CUartSwitch::msc_staVal =
{
    UART_SWITCH_VAL0,
    UART_SWITCH_VAL1,
    UART_SWITCH_VAL2,
    UART_SWITCH_VAL3,
    UART_SWITCH_VAL4,
    UART_SWITCH_VAL5,
    UART_SWITCH_VAL6,
    UART_SWITCH_VAL7,
};

bool
CUartSwitch::Switch(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "Uart Switch +++");

    uint8_t ucVal = 0;

    if (uiIdx >= msc_staVal.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx >= msc_staVal.size(). uiIdx = %d", uiIdx);
        return false;
    }

    m_stpLogicDrv->ReadReg(m_uiReg, &ucVal);
    /* LOG_WRITE(EN_LOG_NOTICE, "b:Switch Reg = %0x, Val = %d", UART_SWITCH_REG, ucVal); */

    ucVal &= ~0x07;
    ucVal |= msc_staVal[uiIdx];

    LOG_WRITE(EN_LOG_DEBUG, "Switch Reg = %0x, Val = %d", m_uiReg, ucVal);

    if (!m_stpLogicDrv->WriteReg(m_uiReg, ucVal)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch Reg = %0x, Val = %d", m_uiReg, ucVal);
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "a:Switch Reg = %0x, Val = %d", UART_SWITCH_REG, ucVal); */
    LOG_WRITE(EN_LOG_DEBUG, "Uart Switch ---");

    return true;
}


