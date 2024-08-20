
#ifndef __SDV_IIC_H__
#define __SDV_IIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "oplk_datatype.h"

#define EEPROM_128KB_SIZE               (128 * 1024)
#define EEPROM_64KB_SIZE                (64 * 1024)

#define EEPROM_2Kb_SIZE                 (256)

#define EEPROM_128KB_PAGE_LEN           256

#define EEPROM_128KB_CFG_WRITE_LEN      128


#define I2C_MAX_LEN                     (256)


/* DEF: PCA9544 channel ID */
#define PCA9544_CH_0                    0x04
#define PCA9544_CH_1                    0x05
#define PCA9544_CH_2                    0x06
#define PCA9544_CH_3                    0x07
#define PCA9544_CH_CLOSE                0x00

/* DEF: PCA9544 addr */
#define CHIP0_PCA9544_ADDR              0xE0
#define CHIP1_PCA9544_ADDR              0xE2
#define CHIP2_PCA9544_ADDR              0xE8


#define CHIP_ADDR_TMP                   (0x90)

#define CHIP_ADDR_CFGEEPROM             (0xAC)
#define CHIP_ADDR_IDEEPROM              (0xAA)

#define CHIP_ADDR_FAN0EEPROM            (0xA4)
#define CHIP_ADDR_FAN1EEPROM            (0xA4)
#define CHIP_ADDR_FAN2EEPROM            (0xA4)
#define CHIP_ADDR_FAN3EEPROM            (0xA4)

#define CHIP_ADDR_POWER0EEPROM          (0xA0)
#define CHIP_ADDR_POWER1EEPROM          (0xA2)



#define DS1721_CMD_READ_TEMP        0xAA
#define DS1721_CMD_ACCESS_TH        0xA1
#define DS1721_CMD_ACCESS_TL        0xA2
#define DS1721_CMD_ACCESS_CFG       0xAC
#define DS1721_CMD_START_CONVERT    0x51
#define DS1721_CMD_STOP_CONVERT     0x22


enum addrtypeRank {
    addrtype7,
    addrtype10,
    NumAddrType,
};

enum cmdtypeRank {
    cmdtypeByte,
    cmdtypeWord,
    cmdtypeNone,
    NumCmdType,
};

struct I2CConf {
    enum addrtypeRank addrType;
    UINT addr;
    enum cmdtypeRank cmdType;
    UINT cmd;
    UCHAR *wdata;
    UINT wlen;
    UCHAR *rdata;
    UINT rlen;

};

/**************************************************************************************************/
/* @defgroup sdv_iic_Exported_Functions                                                          */
/**************************************************************************************************/
extern INT SDV_PCA9544_SelectCH(UCHAR ucAddress, UCHAR ucChannel);
extern INT SDV_ReadBoardTemp(INT *piBuf);
extern INT SDV_Pam4IicRead(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen);
extern INT SDV_Pam4IicWrite(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen);
extern INT SDV_TdcmIicRead(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen);
extern INT SDV_TdcmIicWrite(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen);

extern INT SDV_TdcmIicWrAndChk(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWrBuf, ULONG ulWrLen, UCHAR *pucRdBuf, ULONG ulRdLen);

extern INT SDV_EEpromCfgWrite(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen);
extern INT SDV_EEpromCfgRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen);
extern INT SDV_EEpromFanRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo);
extern INT SDV_EEpromFanWrite(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo);

extern INT SDV_PowerInfoRead(ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen, UCHAR ucNo);

extern INT SDV_ReadBoardTemp(INT *piBuf);
extern INT SDV_IicInit(VOID);
extern INT SDV_IicClose(VOID);

extern INT SDV_Pam4IicRd(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucReadBuf, ULONG ulLen);
extern INT SDV_Pam4IicWr(UCHAR ucSlaveAddr, UINT ulCmd, UCHAR *pucWriteBuf, ULONG ulLen);

extern INT SDV_GetIicFreq(UINT *puiFrequency);

#ifdef __cplusplus
}
#endif

#endif

