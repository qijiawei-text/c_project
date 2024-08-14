
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdint.h>

#include "EApi.h"
#include "sdv_iic.h"
#include "sdv_cpld.h"

#define IIC_SEM_KEY    (0xFFEEDD11)

static BOOL m_bIicInit = OPLK_FALSE;
static INT m_iIicLock = -1;

INT
SDV_IicLock(void)
{
    struct sembuf stSembuf = {0, -1, SEM_UNDO};

    if (m_iIicLock < 0) {
        printf("Iic_Lock need init .\n");
        return -1;
    }

    if (semop(m_iIicLock, &stSembuf, 1) < 0) {
        printf("SDV_IicLock error.\n");
        return -1;
    }

    return 0;
}

INT
SDV_IicUnLock(void)
{
    struct sembuf stSembuf = {0, 1, SEM_UNDO};

    if (m_iIicLock < 0) {
        printf("Iic_Lock need init .\n");
        return -1;
    }

    if (semop(m_iIicLock, &stSembuf, 1) < 0) {
        printf("SDV_IicUnLock error.\n");
        return -1;
    }

    return 0;
}

INT
_DS1721_Init(void)
{
    EApiStatus_t status;
    UCHAR ucData = 0;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct I2CConf config;

    memset(&config, 0, sizeof(config));

    ucData = 0x0C;
    config.addr = CHIP_ADDR_TMP;
    config.cmd = DS1721_CMD_ACCESS_CFG;
    config.wlen = 1;
    config.wdata = &ucData;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        return OPLK_ERROR;
    }

    ucData = DS1721_CMD_START_CONVERT;
    config.addr = CHIP_ADDR_TMP;
    config.cmd = EAPI_I2C_NO_CMD;
    config.wlen = 1;
    config.wdata = &ucData;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        return OPLK_ERROR;
    }

    return OPLK_OK;
}

INT
_DS1721_GetTemp(INT *piTemp)
{
    EApiStatus_t status;
    USHORT usData = 0;
    UCHAR ucDataH, ucDataL;
    SHORT sDs1721Temp = 0;
    FLOAT fTemp = 0;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct I2CConf config;

    memset(&config, 0, sizeof(config));

    config.addr = CHIP_ADDR_TMP;
    config.cmd = DS1721_CMD_READ_TEMP;
    config.rlen = 2;
    config.rdata = (UCHAR *)&usData;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
        return OPLK_ERROR;
    }

    ucDataL = (UCHAR)((usData >> 8) & 0xff);
    ucDataH = (UCHAR)(usData & 0xff);

    sDs1721Temp = (ucDataH << 8) | ucDataL;

    sDs1721Temp = sDs1721Temp / 16;
    fTemp = (FLOAT)sDs1721Temp * 0.0625;
    fTemp = fTemp * 10;

    if (piTemp) {
        *piTemp = (SHORT)fTemp;;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : _EEpromRead                                                                           */
/* Descrp : Read data from eeprom                                                                 */
/* Input  : ucChipAddr-- the EEPROM chip address to read                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg CHIP_ADDR_EEPROM_128KB:                                                      */
/*              @arg CHIP_ADDR_EEPROM_16KB :                                                      */
/*        : ulAddr    -- the start address to read                                                */
/*        : ulLen     -- the specfied length of data to read, unit: byte                          */
/* Output : pucBuf    -- pointer to read buffer                                                   */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_EEpromCfgRead(UCHAR ucChipAddr, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    USHORT usDataLen = 0;
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;
    ULONG ulAddrIndex = 0;
    ULONG ulDataIndex = 0;

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }


    if ((ulAddr + ulLen) > EEPROM_64KB_SIZE) {
        if (ulAddr < EEPROM_64KB_SIZE) {
            OPLK_LOG_PRINT_ERR("not support cross page access.");
            goto ERR_LABEL;
        }
        ucChipAddr |= (0x01 << 1);
    }

    memset(&config, 0, sizeof(config));

    while (0 != ulLen) {
        if (ulLen < EEPROM_128KB_CFG_WRITE_LEN) {
            usDataLen = ulLen;
        } else {
            usDataLen = EEPROM_128KB_CFG_WRITE_LEN;
        }

        memset(&config, 0, sizeof(config));

        config.cmdType = cmdtypeWord;

        config.addr = ucChipAddr;
        config.cmd = ulAddr + ulAddrIndex;
        if (config.cmdType == cmdtypeWord)
            config.cmd = EAPI_I2C_ENC_EXT_CMD(config.cmd);
        config.rlen = usDataLen;
        config.rdata = pucBuf + ulDataIndex;

        status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
        if (status != EAPI_STATUS_SUCCESS) {
            OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
            goto ERR_LABEL;
        }

        ulAddrIndex += usDataLen;
        ulDataIndex += usDataLen;
        ulLen -= usDataLen;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;

}

/***FUNC+******************************************************************************************/
/* Name   : _EEpromWrite                                                                          */
/* Descrp : Write data to eeprom                                                                  */
/* Input  : ucChipAddr-- the EEPROM chip address to write                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg CHIP_ADDR_EEPROM_128KB:                                                      */
/*              @arg CHIP_ADDR_EEPROM_16KB :                                                      */
/*        : ulAddr    -- the start address to write                                               */
/*        : ulLen     -- the specfied length of data to write, unit: byte                         */
/* Output : pucBuf    -- pointer to write buffer                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_EEpromCfgWrite(UCHAR ucChipAddr, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    USHORT usDataLen = 0;
    ULONG ulAddrIndex = 0;
    ULONG ulDataIndex = 0;
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    if (0 != (ulAddr % EEPROM_128KB_PAGE_LEN)) {
        OPLK_LOG_PRINT_ERR("need be a page addr.");
        goto ERR_LABEL;
    }

    if ((ulAddr + ulLen) > EEPROM_64KB_SIZE) {
        if (ulAddr < EEPROM_64KB_SIZE) {
            OPLK_LOG_PRINT_ERR("not support cross page access.");
            goto ERR_LABEL;
        }
        ucChipAddr |= (0x01 << 1);
    }

    memset(&config, 0, sizeof(config));

    while (0 != ulLen) {
        if (ulLen < EEPROM_128KB_CFG_WRITE_LEN) {
            usDataLen = ulLen;
        } else {
            usDataLen = EEPROM_128KB_CFG_WRITE_LEN;
        }

        memset(&config, 0, sizeof(config));

        config.cmdType = cmdtypeWord;

        config.addr = ucChipAddr;
        config.cmd = ulAddr + ulAddrIndex;
        if (config.cmdType == cmdtypeWord)
            config.cmd = EAPI_I2C_ENC_EXT_CMD(config.cmd);
        config.wlen = usDataLen;
        config.wdata = pucBuf + ulDataIndex;
        status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
        if (status != EAPI_STATUS_SUCCESS) {
            OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
            goto ERR_LABEL;
        }

        ulAddrIndex += usDataLen;
        ulDataIndex += usDataLen;
        ulLen -= usDataLen;

        usleep(10 * 1000);
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

static INT
_EEpromFanRead(UCHAR ucChipAddr, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    USHORT usDataLen = 0;

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    if ((ulAddr + ulLen) > EEPROM_64KB_SIZE) {
        ucChipAddr |= 0x01;
    }

    while (0 != ulLen) {
        if (ulLen < EEPROM_128KB_CFG_WRITE_LEN) {
            usDataLen = ulLen;
        } else {
            usDataLen = EEPROM_128KB_CFG_WRITE_LEN;
        }

        if (SDV_CpldI2cRead(ucChipAddr, pucBuf, usDataLen, (UCHAR *)&ulAddr, 2)) {
            OPLK_LOG_PRINT_ERR("Read i2c failed.");
            goto ERR_LABEL;
        }

        ulAddr += usDataLen;
        pucBuf += usDataLen;
        ulLen -= usDataLen;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;

}

static INT
_EEpromFanWrite(UCHAR ucChipAddr, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    USHORT usDataLen = 0;

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    if ((ulAddr + ulLen) > EEPROM_64KB_SIZE) {
        ucChipAddr |= 0x01;
    }

    while (0 != ulLen) {
        if (ulLen < EEPROM_128KB_CFG_WRITE_LEN) {
            usDataLen = ulLen;
        } else {
            usDataLen = EEPROM_128KB_CFG_WRITE_LEN;
        }

        if (OPLK_OK != SDV_CpldI2cWrite(ucChipAddr, pucBuf, usDataLen, (UCHAR *)&(ulAddr), 2)) {
            OPLK_LOG_PRINT_ERR("Write i2c failed.");
            goto ERR_LABEL;
        }

        ulAddr += usDataLen;
        pucBuf += usDataLen;
        ulLen -= usDataLen;

        usleep(10 * 1000);
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}


INT
SDV_FanPCA9544_SelectCH(UCHAR ucAddress, UCHAR ucChannel)
{
    UCHAR ucCmd = 0;

    if (SDV_CpldI2cWrite(ucAddress, &ucChannel, 1, &ucCmd, 0)) {
        OPLK_LOG_PRINT_ERR("Write cpld i2c failed.");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}


INT
SDV_ReadBoardTemp(INT *piBuf)
{
    INT iRetVal = -1;
    INT iTemp = 0;
    static UCHAR sucInit = 1;

    SDV_IicLock();

    if (sucInit) {
        iRetVal = _DS1721_Init();
        if (iRetVal < 0) {
            goto ERR_LABEL;
        }

        sucInit = 0;
        sleep(1);
    }
    iRetVal = _DS1721_GetTemp(&iTemp);
    if (iRetVal < 0) {
        goto ERR_LABEL;
    }

    *piBuf = iTemp;

    SDV_IicUnLock();
    return OPLK_OK;

ERR_LABEL:

    SDV_IicUnLock();
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_EEpromCfgRead                                                                        */
/* Descrp : Read data from eeprom                                                                 */
/* Input  : usPageAddr-- the start page address to read                                           */
/*        : ulLen     -- the specfied length of data to read, unit: byte                          */
/* Output : pucBuf    -- pointer to read buffer                                                   */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_EEpromCfgRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    SDV_IicLock();

    if ((ulAddr + ulLen) > EEPROM_128KB_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (_EEpromCfgRead(CHIP_ADDR_CFGEEPROM, ulAddr, pucBuf, ulLen)) {
        OPLK_LOG_PRINT_ERR("_EEpromCfgRead error.");
        goto ERR_LABEL;
    }

    SDV_IicUnLock();

    return OPLK_OK;
ERR_LABEL:
    SDV_IicUnLock();
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_EEpromCfgWrite                                                                       */
/* Descrp : Write data to eeprom                                                                  */
/* Input  : ulAddr-- the start address to write                                                   */
/*        : ulLen -- the specfied length of data to write, unit: byte                             */
/* Output : pucBuf-- pointer to write buffer                                                      */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_EEpromCfgWrite(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    SDV_IicLock();

    if ((ulAddr + ulLen) > EEPROM_128KB_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (OPLK_OK != _EEpromCfgWrite(CHIP_ADDR_CFGEEPROM, ulAddr, pucBuf, ulLen)) {
        OPLK_LOG_PRINT_ERR("_EEpromCfgWrite error.");
        goto ERR_LABEL;
    }

    SDV_IicUnLock();
    return OPLK_OK;

ERR_LABEL:
    SDV_IicUnLock();
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_EEprom2Read                                                                       */
/* Descrp : Read data from eeprom                                                                 */
/* Input  : ulAddr-- the start page address to read                                               */
/*        : ulLen -- the specfied length of data to read, unit: byte                              */
/* Output : pucBuf-- pointer to read buffer                                                       */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_EEpromIDRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));

    SDV_IicLock();
    if ((ulAddr + ulLen) > EEPROM_2Kb_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    config.addr = CHIP_ADDR_IDEEPROM;
    config.cmd = ulAddr;
    config.rlen = ulLen;
    config.rdata = pucBuf;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    SDV_IicUnLock();

    return OPLK_OK;

ERR_LABEL:
    SDV_IicUnLock();
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_EEprom2Write                                                                      */
/* Descrp : Write data to eeprom                                                                  */
/* Input  : ulAddr-- the start address to write                                                   */
/*        : ulLen -- the specfied length of data to write, unit: byte                             */
/* Output : pucBuf-- pointer to write buffer                                                      */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_EEpromIDWrite(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;
    int i = 0;

    SDV_IicLock();

    if ((ulAddr + ulLen) > EEPROM_2Kb_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    for (i = 0; i < ulLen; i++) {
        memset(&config, 0, sizeof(config));

        config.addr = CHIP_ADDR_IDEEPROM;
        config.cmd = ulAddr;
        config.wlen = 1;
        config.wdata = pucBuf + i;
        status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
        if (status != EAPI_STATUS_SUCCESS) {
            OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
            goto ERR_LABEL;
        }

        ulAddr++;

        usleep(100 * 1000);
    }

    SDV_IicUnLock();
    return OPLK_OK;

ERR_LABEL:
    SDV_IicUnLock();
    return OPLK_ERROR;
}

INT
SDV_EEpromIDReadByCpldI2c(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{

    if ((ulAddr + ulLen) > EEPROM_2Kb_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    if (SDV_CpldI2cRead(CHIP_ADDR_IDEEPROM, pucBuf, ulLen, (UCHAR *)&ulAddr, 1)) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_EEpromIDWriteByCpldI2c(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    int i = 0;

    if ((ulAddr + ulLen) > EEPROM_2Kb_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (NULL == pucBuf) {
        OPLK_LOG_PRINT_ERR("pucBuf error.");
        goto ERR_LABEL;
    }

    for (i = 0; i < ulLen; i++) {
        if (SDV_CpldI2cWrite(CHIP_ADDR_IDEEPROM, pucBuf + i, 1, (UCHAR *)&ulAddr, 1)) {
            goto ERR_LABEL;
        }
        ulAddr++;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_EEpromFanRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo)
{
    UCHAR ucFanChipAddr = 0;
    UCHAR uc9544Ch = 0;

    if ((ulAddr + ulLen) > EEPROM_128KB_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (ucNo == FAN0_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN0EEPROM;
        uc9544Ch = PCA9544_CH_0;
    } else if (ucNo == FAN1_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN1EEPROM;
        uc9544Ch = PCA9544_CH_1;
    } else if (ucNo == FAN2_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN2EEPROM;
        uc9544Ch = PCA9544_CH_2;
    } else if (ucNo == FAN3_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN3EEPROM;
        uc9544Ch = PCA9544_CH_3;
    } else {
        return OPLK_ERROR;
    }

    SDV_CpldI2cLock();

    SDV_CpldI2CSPiChoose(CPLD_I2C_FAN);

    SDV_FanPCA9544_SelectCH(CHIP2_PCA9544_ADDR, uc9544Ch);

    if (_EEpromFanRead(ucFanChipAddr, ulAddr, pucBuf, ulLen)) {
        OPLK_LOG_PRINT_ERR("_EEpromCfgRead error.");
        SDV_CpldI2cUnlock();
        goto ERR_LABEL;
    }

    SDV_FanPCA9544_SelectCH(CHIP2_PCA9544_ADDR, PCA9544_CH_CLOSE);

    SDV_CpldI2cUnlock();

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERROR;

}

INT
SDV_EEpromFanWrite(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo)
{
    UCHAR ucFanChipAddr = 0;
    UCHAR uc9544Ch = 0;

    if ((ulAddr + ulLen) > EEPROM_128KB_SIZE) {
        OPLK_LOG_PRINT_ERR("address or length overflow");
        goto ERR_LABEL;
    }

    if (0 != (ulAddr % EEPROM_128KB_PAGE_LEN)) {
        OPLK_LOG_PRINT_ERR("need be a page addr.");
        goto ERR_LABEL;
    }

    if (ucNo == FAN0_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN0EEPROM;
        uc9544Ch = PCA9544_CH_0;
    } else if (ucNo == FAN1_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN1EEPROM;
        uc9544Ch = PCA9544_CH_1;
    } else if (ucNo == FAN2_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN2EEPROM;
        uc9544Ch = PCA9544_CH_2;
    } else if (ucNo == FAN3_IDX) {
        ucFanChipAddr = CHIP_ADDR_FAN3EEPROM;
        uc9544Ch = PCA9544_CH_3;
    } else {
        return OPLK_ERROR;
    }

    SDV_CpldI2cLock();

    SDV_CpldI2CSPiChoose(CPLD_I2C_FAN);

    SDV_FanPCA9544_SelectCH(CHIP2_PCA9544_ADDR, uc9544Ch);

    if (OPLK_OK != _EEpromFanWrite(ucFanChipAddr, ulAddr, pucBuf, ulLen)) {
        OPLK_LOG_PRINT_ERR("_EEpromFanWrite error.");
        SDV_CpldI2cUnlock();
        goto ERR_LABEL;
    }

    SDV_FanPCA9544_SelectCH(CHIP2_PCA9544_ADDR, PCA9544_CH_CLOSE);

    SDV_CpldI2cUnlock();

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_PowerInfoRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo)
{
    UCHAR ucPowerChipAddr = 0;

    if (ucNo == POWER0_IDX) {
        ucPowerChipAddr = CHIP_ADDR_POWER0EEPROM;
    } else if (ucNo == POWER1_IDX) {
        ucPowerChipAddr = CHIP_ADDR_POWER1EEPROM;
    } else {
        goto ERR_LABEL;
    }

    SDV_CpldI2cLock();

    SDV_CpldI2CSPiChoose(CPLD_I2C_POWER);

    if (SDV_CpldI2cRead(ucPowerChipAddr, pucBuf, ulLen, (UCHAR *)&ulAddr, 1)) {
        OPLK_LOG_PRINT_ERR("Read power info failed.");
        SDV_CpldI2cUnlock();
        goto ERR_LABEL;
    }

    SDV_CpldI2cUnlock();

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/*
   INT
   SDV_Pam4IicRead(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen)
   {
 #ifdef _DEBUG_SIMULATE_
    return OPLK_OK;
 #endif
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_1);

    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }

    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.rlen = ulLen;
    config.rdata = pucReadBuf;

    status = SusiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen);
    if (status != SUSI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)\n", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);

    return OPLK_OK;
   ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    return OPLK_ERROR;

   }

   INT
   SDV_Pam4IicWrite(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen)
   {
 #ifdef _DEBUG_SIMULATE_
    return OPLK_OK;
 #endif
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_1);
    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }



    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.wlen = ulLen;
    config.wdata = pucWriteBuf;

    status = SusiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != SUSI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)\n", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP1_PCA9544_ADDR, PCA9544_CH_CLOSE);

    return OPLK_OK;
   ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP1_PCA9544_ADDR, PCA9544_CH_CLOSE);
    return OPLK_ERROR;
   }*/

INT
SDV_TdcmIicWrAndChk(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWrBuf, ULONG ulWrLen, UCHAR *pucRdBuf, ULONG ulRdLen)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_IicLock();
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_2);
    if (ulWrLen > I2C_MAX_LEN || ulRdLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }



    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.wlen = ulWrLen;
    config.wdata = pucWrBuf;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    memset(&config, 0, sizeof(config));

    config.addr = ucSlaveAddr;
    config.cmd = EAPI_I2C_NO_CMD;
    config.rlen = ulRdLen;
    config.rdata = pucRdBuf;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }


    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_OK;
ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_ERROR;
}

INT
SDV_TdcmIicRead(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_IicLock();
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_2);
    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }



    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.rlen = ulLen;
    config.rdata = pucReadBuf;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_OK;
ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_ERROR;
}

INT
SDV_TdcmIicWrite(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_IicLock();
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_2);
    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }



    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.wlen = ulLen;
    config.wdata = pucWriteBuf;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_OK;
ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_IicInit                                                                           */
/* Descrp : IIC init                                                                              */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_IicInit(VOID)
{
    EApiStatus_t status;
    union semun arg;

    if (OPLK_TRUE == m_bIicInit) {
        return OPLK_OK;
    }

    m_iIicLock = semget((key_t)IIC_SEM_KEY, 1, IPC_CREAT | 0666);

    if (m_iIicLock < 0) {
        OPLK_LOG_PRINT_ERR("semget error.");
        return -1;
    }

    arg.setval = 1;
    if (-1 == semctl(m_iIicLock, 0, SETVAL, arg)) {
        OPLK_LOG_PRINT_ERR("semctl error.");
        return -1;
    }

    status = EApiLibInitialize();

    if (status == EAPI_STATUS_ERROR) {
        /* This error only occurs on Linux distributions */
        OPLK_LOG_PRINT_ERR("Your Linux capabilities is not enough, recommond to become ROOT!");
        return OPLK_ERROR;
    }

    m_bIicInit = OPLK_TRUE;
    return OPLK_OK;

}

/***FUNC+******************************************************************************************/
/* Name   : SDV_IicClose                                                                          */
/* Descrp : IIC close                                                                             */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_IicClose(VOID)
{
    EApiStatus_t status;

    m_bIicInit = OPLK_FALSE;

    if (m_iIicLock < 0) {
        printf("Iic_Lock need init.\n");
        return -1;
    }

    if (-1 == semctl(m_iIicLock, 0, IPC_RMID, 0)) {
        OPLK_LOG_PRINT_ERR("semctl error.");
        return -1;
    }

    m_iIicLock = -1;

    status = EApiLibUnInitialize();

    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("SusiLibUninitialize() failed. (%8X)", status);
        return OPLK_ERROR;
    }

    return OPLK_OK;
}



INT
SDV_PCA9544_SelectCH(UCHAR ucAddress, UCHAR ucChannel)
{
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct I2CConf config;

    memset(&config, 0, sizeof(config));

    config.addr = ucAddress;
    config.cmd = EAPI_I2C_NO_CMD;
    config.wlen = 1;
    config.wdata = &ucChannel;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}


INT
SDV_Pam4IicRd(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return OPLK_OK;
#endif
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));

    SDV_IicLock();

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_1);

    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }

    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.rlen = ulLen;
    config.rdata = pucReadBuf;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Read transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_OK;
ERR_LABEL:

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_ERROR;

}


INT
SDV_Pam4IicWr(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen)
{
#ifdef _DEBUG_SIMULATE_
    return OPLK_OK;
#endif
    EApiStatus_t status;
    EApiId_t id = EAPI_ID_I2C_EXTERNAL;
    struct  I2CConf config;

    memset(&config, 0, sizeof(config));
    SDV_IicLock();
    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_1);
    if (ulLen > I2C_MAX_LEN) {
        OPLK_LOG_PRINT_ERR("length overflow");
        goto ERR_LABEL;
    }



    config.addr = ucSlaveAddr;
    config.cmd = ulCmd;
    config.wlen = ulLen;
    config.wdata = pucWriteBuf;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != EAPI_STATUS_SUCCESS) {
        OPLK_LOG_PRINT_ERR("Write transfer failed. (0x%08X)", status);
        goto ERR_LABEL;
    }

    SDV_PCA9544_SelectCH(CHIP1_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_OK;
ERR_LABEL:
    SDV_PCA9544_SelectCH(CHIP1_PCA9544_ADDR, PCA9544_CH_CLOSE);
    SDV_IicUnLock();
    return OPLK_ERROR;
}
/*
   INT SDV_GetIicFreq(UINT *puiFrequency)
   {
        SusiId_t id = SUSI_ID_I2C_EXTERNAL;
        SusiI2CSetFrequency(id, 400);
        sleep(3);
        SusiI2CGetFrequency(id, puiFrequency);
        OPLK_LOG_PRINT_OPT("susi iic freq:%dKHz", *puiFrequency);
        return OPLK_OK;
   }*/

