#include "Max31760Driver.h"
#include "BoardCmd.h"

bool m_bSaveToEEP = false;


bool
CMax31760::InitFanController()
{
    uint8_t aucBuf[2] = {0};
#if 1
    if (false == m_bInit) {
        if (nullptr == m_stpI2cPtr) {
            LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
            return false;
        }

        aucBuf[0] = m_ucRegCtrl1;
        aucBuf[1] = 0x1d; /* 0x1d;  0x19; */
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

        aucBuf[0] = m_ucRegCtrl2;
        aucBuf[1] = 0x39; /* 0x39; */
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

        aucBuf[0] = m_ucRegCtrl3;
        aucBuf[1] = 0x71; /* 0x00  bit 5:4  0 default(slow) 1 middle slow, 2 nomal 3 fast */
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);


#if 0
        aucBuf[0] = MAX31760_FFDC;
        aucBuf[1] = 0xff;
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

        aucBuf[0] = MAX31760_MASK;
        aucBuf[1] = 0xff;
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);
#endif
        /* m_bInit = true; */
    }
#endif
    return true;
}


bool
CMax31760::SetFanController()
{
    uint8_t aucBuf[2] = {0};
    std::shared_ptr<CLogicDriver> stpLogicDrv = CBoardCmd::GetLogicDrvPtr(0);
    /* uint8_t ucData = 0; */
    /* uint8_t ucRegCtrl1 = 0; */
    uint8_t ucRegCtrl2 = 0;
    /* uint8_t ucRegCtrl3 = 0; */
    uint8_t ucResetValue = false;
    bool bResetReadResult = false;

    if (nullptr == stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrv is nullptr");
        return false;
    }

    #if 0
    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegCtrl1, sizeof(m_ucRegCtrl1), (uint8_t *)&ucRegCtrl1, sizeof(ucRegCtrl1))) {
        LOG_WRITE(EN_LOG_ERROR, "fan ucRegCtrl1 Read error");
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "fan ucRegCtrl1 Read,ucRegCtrl1 = 0x%0x", ucRegCtrl1);
    /*printf("fan ucRegCtrl1 Read,ucRegCtrl1 = 0x%0x\n", ucRegCtrl1);*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegCtrl2, sizeof(m_ucRegCtrl2), (uint8_t *)&ucRegCtrl2, sizeof(ucRegCtrl2))) {
        LOG_WRITE(EN_LOG_ERROR, "fan ucRegCtrl2 Read error");
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "fan ucRegCtrl2 Read,ucRegCtrl2 = 0x%0x", ucRegCtrl2);
    /*printf("fan ucRegCtrl2 Read,ucRegCtrl2 = 0x%0x\n", ucRegCtrl2);*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegCtrl3, sizeof(m_ucRegCtrl3), (uint8_t *)&ucRegCtrl3, sizeof(ucRegCtrl3))) {
        LOG_WRITE(EN_LOG_ERROR, "fan ucRegCtrl3 Read error");
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "fan ucRegCtrl3 Read,ucRegCtrl3 = 0x%0x", ucRegCtrl3);
    /*printf("fan ucRegCtrl3 Read,ucRegCtrl3 = 0x%0x\n", ucRegCtrl3);*/
    #endif

    if ( m_bSaveToEEP == false) {
        LOG_WRITE(EN_LOG_NOTICE, "SetFanController++");

        /* write 'm_ucRegCtrl1' to 0x5d(bit6=1) will cause registers default. */
        /* Software POR. Returns the MAX31760 to POR conditions. */
        /* Loads EEPROM data into EEPROM-backed registers, returns all other registers to their default states. */
        /* When set to 1, all other bits are ignored. Self-clears to 0. */
        bResetReadResult = m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegUser0, sizeof(m_ucRegUser0), (uint8_t *)&ucResetValue, sizeof(ucResetValue));

        LOG_WRITE(EN_LOG_DEBUG, "fan SetFanController,ucRegCtrl2 = %d", ucRegCtrl2);
        /*printf("fan SetFanController,ucRegCtrl2 = 0x%0x\n", ucRegCtrl2);*/

        aucBuf[0] = MAX31760_EEX;
        aucBuf[1] = 0x9f;
        if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
            LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "fan  MAX31760_EEX Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

        if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegCtrl2, sizeof(m_ucRegCtrl2), (uint8_t *)&ucRegCtrl2, sizeof(ucRegCtrl2))) {
            LOG_WRITE(EN_LOG_ERROR, "fan ucRegCtrl2 Read error");
            return false;
        }
        LOG_WRITE(EN_LOG_DEBUG, "fan ucRegCtrl2 Read,ucRegCtrl2 = 0x%0x", ucRegCtrl2);
        /* printf("fan ucRegCtrl2 Read,ucRegCtrl2 = %x\n", ucRegCtrl2); */
        if (0x18 != ucRegCtrl2) {
            aucBuf[0] = m_ucRegCtrl1;
            aucBuf[1] = 0x1d; /* 0x1d */
            if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
                LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
                return false;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

            aucBuf[0] = m_ucRegCtrl2;
            aucBuf[1] = 0x18;
            if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
                LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
                return false;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);

            #if 0
            /*debug reg2*/
            if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegCtrl2, sizeof(m_ucRegCtrl2), (uint8_t *)&ucRegCtrl2, sizeof(ucRegCtrl2))) {
                LOG_WRITE(EN_LOG_ERROR, "fan ucRegCtrl2 Read error");
                return false;
            }
            printf("%s(%d):%s  fan ucRegCtrl2 Read,ucRegCtrl2 = 0x%0x\n", __FILE__, __LINE__, __func__, ucRegCtrl2);
            LOG_WRITE(EN_LOG_DEBUG, "fan ucRegCtrl2 Read,ucRegCtrl2 = 0x%0x", ucRegCtrl2);
            #endif

            aucBuf[0] = m_ucRegCtrl3;
            aucBuf[1] = 0x03;
            if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
                LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
                return false;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);


            aucBuf[0] = MAX31760_EEX;
            aucBuf[1] = 0x1f;
            if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
                LOG_WRITE(EN_LOG_ERROR, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
                return false;
            }
            LOG_WRITE(EN_LOG_DEBUG, "fan  MAX31760_EEX Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);
#if 0
            ucData = (1 << m_uiHwSubIdx) | 0x0f;
            stpLogicDrv->Write(0x100c, &ucData, 1);
            sleep(5);
            ucData = 0x0f;
            stpLogicDrv->Write(0x100c, &ucData, 1);
#else

            aucBuf[0] = m_ucRegCtrl1;
            aucBuf[1] = 0x5d; /* bit6 to 1 POR */
            if (false == m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
                LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
            }
            LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d.", m_uiHwSubIdx, m_uiDevAddr);
            sleep(1);
#endif
        }

        if (true == bResetReadResult) {
            aucBuf[0] = m_ucRegUser0;
            aucBuf[1] = ucResetValue;
            m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf));
        }
    }
    return true;
}



bool
CMax31760::GetFanRPM(uint32_t &uiSpeed)
{
    uint32_t uiSpeedCur = 0;
    uint8_t ucTc1h = 0;
    uint8_t ucTc1l = 0;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
        return false;
    }

    if (false == InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " InitFanController init fail.");
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegTc1h, sizeof(m_ucRegTc1h), (uint8_t *)&ucTc1h, sizeof(ucTc1h))) {
        LOG_WRITE(EN_LOG_ERROR, "fan speed I2c Read error,ucTc1h = %d", ucTc1h);
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegTc1l, sizeof(m_ucRegTc1l), (uint8_t *)&ucTc1l, sizeof(ucTc1l))) {
        LOG_WRITE(EN_LOG_ERROR, "fan speed I2c Read error,ucTc1l = %d", ucTc1l);
        return false;
    }

    /* Fan_RPM = 60 x 100,000/(TC12H_Value x 256 + TC2L_Value)/n */
    uiSpeedCur = (uint32_t)60 * 100000 / (ucTc1h * 256 + ucTc1l) / PULSE_PER_CIRCLE;

    uiSpeed = uiSpeedCur;
    LOG_WRITE(EN_LOG_DEBUG, "ucTc1h = %d , ucTc1l = %d, riSpeed = %d, uiSpeed = %d", ucTc1h, ucTc1l, uiSpeed, uiSpeedCur);

    return true;

}

bool
CMax31760::SetFanSpeed(uint8_t ucFanSpeed)
{
    uint8_t ucPwm = ucFanSpeed * 255 / 100;
    unsigned char aucBuf[2] = {0};
    aucBuf[0] = m_ucRegPwmr;
    aucBuf[1] = ucPwm;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
        return false;
    }

    if (false == SetFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " SetFanController fail.");
        return false;
    }

    if (false == InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " InitFanController init fail.");
        return false;
    }

    if (!m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "SetPMW Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
        return false;
    }
    LOG_WRITE(EN_LOG_DEBUG, "Write m_uiHwSubIdx = %d, m_uiDevAddr = %d, ucFanSpeed = %d ", m_uiHwSubIdx, m_uiDevAddr, ucFanSpeed);
    return true;

}

bool
CMax31760::GetFanSpeed(uint32_t &uiSpeed)
{
    uint8_t ucTch = 0;
    uint8_t ucTcl = 0;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
        return false;
    }

    if (false == InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " InitFanController init fail.");
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegTc1h, sizeof(m_ucRegTc1h), (uint8_t *)&ucTch, sizeof(ucTch))) {
        LOG_WRITE(EN_LOG_ERROR, "fan speed I2c Read error,ucTch = %", ucTch);
        return false;
    }
    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegTc1l, sizeof(m_ucRegTc1l), (uint8_t *)&ucTcl, sizeof(ucTcl))) {
        LOG_WRITE(EN_LOG_ERROR, "fan speed I2c Read error,ucTcl = %d", ucTcl);
        return false;
    }

    /* RPM = 60 * 100000 / (TCH * 256 + TCL) / 2 */
    /* speed =  RPM / 22000 */
    /* (OC2A-347)[EQA 65]: insert a metal stick to stop the fan from rotating. Report fan speed is FF */
    if (ucTch == 0 && ucTcl == 0) {
        uiSpeed = 99;
        LOG_WRITE(EN_LOG_DEBUG, "ucTch = %d, ucTcl = %d, uiSpeed = %d", ucTch, ucTcl, uiSpeed);
        return true;
    }
    
    if (ucTcl == 255 && ucTch == 255) {
        uiSpeed = 255;
        return true;
    }

    uiSpeed = 60 * 100000 * 100 / (ucTch * 256 + ucTcl) / 2 / 22000;

    /* (M120-1190) INT- Fan speed exceeds 100% when ambient temperature is 40° */
    /* Control fluctuations greater than 100 to 100 */
    if(uiSpeed > 100 && uiSpeed <= 105) {
        uiSpeed = 100;
    }

    LOG_WRITE(EN_LOG_DEBUG, "ucTch = %d, ucTcl = %d, uiSpeed = %d", ucTch, ucTcl, uiSpeed);

    return true;

}

bool
CMax31760::SetResetCause(uint8_t ucResetCause)
{
    unsigned char aucBuf[2] = {0};
    aucBuf[0] = m_ucRegUser0;
    aucBuf[1] = ucResetCause;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
        return false;
    }
/*
    if (false == SetFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " SetFanController fail.");
        return false;
    }
 */
    if (false == InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " InitFanController init fail.");
        return false;
    }

    if (!m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, (uint8_t *)&aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "SetResetCause Write m_uiHwSubIdx = %d, m_uiDevAddr = %d error.", m_uiHwSubIdx, m_uiDevAddr);
        return false;
    }

    return true;

}

bool
CMax31760::GetResetCause(uint8_t &ucResetCause)
{
    uint8_t ucValue = 0;

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Max31760 i2c ptr is nullptr.");
        return false;
    }

    if (false == InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, " InitFanController init fail.");
        return false;
    }

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &m_ucRegUser0, sizeof(m_ucRegUser0), (uint8_t *)&ucValue, sizeof(ucValue))) {
        LOG_WRITE(EN_LOG_ERROR, "fan user0 I2c Read error, ucValue = %d", ucValue);
        return false;
    }

    ucResetCause = ucValue;
    LOG_WRITE(EN_LOG_DEBUG, "ucResetCause = %d, ucValue = %d", ucResetCause, ucValue);

    return true;

}