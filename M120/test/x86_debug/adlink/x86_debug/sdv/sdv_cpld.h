
#ifndef __SDV_CPLD_H__
#define __SDV_CPLD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "oplk_datatype.h"

#define MV6390X_PHY_ADDR        0x1E

/* #define MV_NO_CPU */

#define MV6390X_PORT0_ADDR  0x00
#define MV6390X_PORT1_ADDR  0x01
#define MV6390X_PORT2_ADDR  0x02
#define MV6390X_PORT3_ADDR  0x03
#define MV6390X_PORT4_ADDR  0x04
#define MV6390X_PORT5_ADDR  0x05
#define MV6390X_PORT6_ADDR  0x06
#define MV6390X_PORT7_ADDR  0x07
#define MV6390X_PORT8_ADDR  0x08
#define MV6390X_PORT9_ADDR  0x09
#define MV6390X_PORTA_ADDR  0x0A

#define MV6390X_GlOB1_ADDR  0x1B
#define MV6390X_GlOB2_ADDR  0x1C

#define MV6390X_INCPU_ADDR  0x1E
#define MV6390X_TCAM_ADDR   0x1F

#define MV6390X_SMI_CMD_ADDR    0x18
#define MV6390X_SMI_DATA_ADDR   0x19


#define MSD_REG_SMI_COMMAND     0x0
#define MSD_REG_SMI_DATA        0x1


/* Bit definition for MSD_REG_SMI_COMMAND */
#define MSD_SMI_BUSY                0x8000
#define MSD_SMI_MODE                0x1000
#define MSD_SMI_MODE_BIT            12
#define MSD_SMI_FUNC_BIT            13
#define MSD_SMI_FUNC_SIZE            2
#define MSD_SMI_OP_BIT              10
#define MSD_SMI_OP_SIZE              2
#define MSD_SMI_DEV_ADDR_BIT         5
#define MSD_SMI_DEV_ADDR_SIZE        5
#define MSD_SMI_REG_ADDR_BIT         0
#define MSD_SMI_REG_ADDR_SIZE        5

#define MSD_SMI_CLAUSE45            0
#define MSD_SMI_CLAUSE22            1

#define MSD_SMI_WRITE_ADDR          0x00
#define MSD_SMI_WRITE               0x01
#define MSD_SMI_READ_22             0x02
#define MSD_SMI_READ_45             0x03
#define MSD_SMI_READ_INC            0x02

#define MSD_SMI_ACCESS_LOOP        1000
#define MSD_SMI_TIMEOUT            2

#define MSD_PHY_MMD_ADDR          0x0000
#define MSD_PHY_MMD_DATA_NO_INC   0x4000
#define MSD_PHY_MMD_DATA_RW_INC   0x8000
#define MSD_PHY_MMD_DATA_WR_INC   0xc000

#define MSD_INVALID_PHY             0xFF
#define MSD_INVALID_PORT            0xFF
#define MSD_INVALID_PORT_VEC        0xFFFFFFFF

#define VALIDPORTVEC    0x1FE
#define BASEPHYREGADDR  0


#define MSD_LPORT_2_PHY(_lport)       msdlport2phy(_lport)
#define MSD_LPORT_2_PORT(_lport)      msdlport2port(VALIDPORTVEC, _lport)


#define NVM_MAX_RETRY_SPI           10 /* Max wait of 5ms, for RDY signal */
#define NVM_READ_OPCODE_SPI         0x03 /* NVM read opcode */
#define NVM_WRITE_OPCODE_SPI    0x02 /* NVM write opcode */
#define NVM_A8_OPCODE_SPI           0x08 /* opcode bit-3 = address bit-8 */
#define NVM_WREN_OPCODE_SPI         0x06 /* NVM set Write Enable latch */
#define NVM_RDSR_OPCODE_SPI         0x05 /* NVM read Status register */

/* SPI NVM Status Register */
#define NVM_STATUS_RDY_SPI          0x01

#define NVM_PAGE_SZIE       (64)
#define NVM_TATOL_SZIE      (32 * 1024)



#define CHIP_PAGE_BYTES                 (0x100)
#define CHIP_PAGE_NUMS                  (0x10000)
#define CHIP_TOTAL_BYTES                (CHIP_PAGE_BYTES * CHIP_PAGE_NUMS)

#define CMD_WRITE_ENABLE                (0x06)
#define CMD_WRITE_DISABLE               (0x04)

#define CMD_RDID                        (0x9F)                              /* Read Identification                                     */

#define CMD_WRITE                       (0x02)
#define CMD_READ                        (0x03)

#define CMD_STATUS_READ                 (0x05)
#define CMD_STATUS_WRITE                (0x01)

#define CMD_ERASE_4K                    (0x20)
#define CMD_ERASE_64K                   (0xD8)

#define CHIP_BLOCK4K_LEN                (4 * 1024)
#define CHIP_BLOCK64K_LEN               (64 * 1024)

#define PROTECT_TRY_TIMES               (1000)
#define PAGE_WRITE_TRY_TIMES            (10000)
#define ERASE4K_TRY_TIMES               (5000)
#define ERASE64K_TRY_TIMES              (10000)

/**
 * @defgroup  SPI Flash Chip ID
 * @{
 */
typedef enum
{
    FPGA_SPI_FLASH_ID_N25Q128 = 0,                                          /* N25Q128                                                 */
    FPGA_SPI_FLASH_ID_S25FL129P,                                            /* S25FL129P                                               */

    FPGA_SPI_FLASH_ID_MAX

} FPGA_SPI_FLASH_ID;
/**
 * @}
 */

/**
 * @defgroup  SPI Flash Load Mode
 * @{
 */
typedef enum
{
    FPGA_SPI_LOAD_BIT_1 = 1,
    FPGA_SPI_LOAD_BIT_2,
    FPGA_SPI_LOAD_BIT_4 = 4,

    FPGA_SPI_LOAD_BIT_MAX

} FPGA_SPI_LOAD_BIT;
/**
 * @}
 */


/**
 * @}
 */

/**
 * @defgroup  SPI Slave Selection
 * @{
 */
typedef enum
{
    FPGA_SPI_NONE = 0,
    FPGA_SPI_EDFA1_FLASH = 1,
    FPGA_SPI_EDFA2_FLASH = 2,

    FPGA_SPI_SLAVE_MAX

} EN_FPGA_SPI_SLAVE_SEL;
/**
 * @}
 */


#define CPLD1_OFFSET(x)    (0x8000 + x)

/* define cpld register                                                                           */
typedef enum
{
    /*cpld0*/
    CPLD0_VERH = 0x02,
    CPLD0_VERL = 0x03,
    CPLD0_VER_FST = 0x07,
    CPLD0_JTAG_UPG = 0x20,
    CPLD0_RESET_CTL1 = 0x28,
    CPLD0_RESET_CTL2 = 0x29,
    CPLD0_BIOS_CTL = 0x2C,
    CPLD0_POW12V_CTL = 0x30,
    CPLD0_POWBRD_CTL = 0x31,
    CPLD0_POW_STA1 = 0x34,
    CPLD0_POW_STA2 = 0x35,
    CPLD0_POE_CTLSTA = 0x38,
    CPLD0_LED01_CTL = 0x3D,
    CPLD0_LED23_CTL = 0x3E,
    CPLD0_LED45_CTL = 0x3F,
    CPLD0_BRD_CTLSTA = 0x48,

    /*cpld1*/
    CPLD1_VERH = CPLD1_OFFSET(0x02),
    CPLD1_VERL = CPLD1_OFFSET(0x03),
    CPLD1_VER_FST = CPLD1_OFFSET(0x07),
    CPLD1_JTAG_UPG = CPLD1_OFFSET(0x20),
    CPLD1_RESET_CTL1 = CPLD1_OFFSET(0x28),
    CPLD1_RESET_CTL2 = CPLD1_OFFSET(0x29),
    CPLD1_SFP_CTLSTA = CPLD1_OFFSET(0x30),
    CPLD1_APSD_STA = CPLD1_OFFSET(0x34),
    CPLD1_UART0_SEL = CPLD1_OFFSET(0x38),
    CPLD1_UART1_SEL = CPLD1_OFFSET(0x39),
    CPLD1_PHY_CTLSTA = CPLD1_OFFSET(0x40),
    CPLD1_I2CSPI_SEL = CPLD1_OFFSET(0x42),
    CPLD1_BRD_CTLSTA = CPLD1_OFFSET(0x44),
    CPLD1_EXTBRD_CTL = CPLD1_OFFSET(0x48),


    CPLD1_MDIO_OP = CPLD1_OFFSET(0x81),
    CPLD1_MDIO_PHYAD = CPLD1_OFFSET(0x82),
    CPLD1_MDIO_REGAD = CPLD1_OFFSET(0x83),
    CPLD1_MDIO_TX_DATA_H = CPLD1_OFFSET(0x84),
    CPLD1_MDIO_TX_DATA_L = CPLD1_OFFSET(0x85),
    CPLD1_MDIO_RX_DATA_H = CPLD1_OFFSET(0x86),
    CPLD1_MDIO_RX_DATA_L = CPLD1_OFFSET(0x87),

    CPLD1_I2C_CTL = CPLD1_OFFSET(0x92),
    CPLD1_I2C_DEVADDR = CPLD1_OFFSET(0x94),
    CPLD1_I2C_TXDATA = CPLD1_OFFSET(0x96),
    CPLD1_I2C_RXDATA = CPLD1_OFFSET(0x97),

    /*cpldFan*/
    CPLDFAN_VERH = 0x02,
    CPLDFAN_VERL = 0x03,
    CPLDFAN_VER_FST = 0x07,
    CPLDFAN_ONLINE = 0x0F,
    CPLDFAN_FAN1PWM = 0x10,
    CPLDFAN_FAN2PWM = 0x11,
    CPLDFAN_FAN3PWM = 0x12,
    CPLDFAN_FAN4PWM = 0x13,
    CPLDFAN_FAN1SPEED = 0x14,
    CPLDFAN_FAN2SPEED = 0x15,
    CPLDFAN_FAN3SPEED = 0x16,
    CPLDFAN_FAN4SPEED = 0x17,


    CPLD_REG_MAX

} EN_CPLD_REGS;


typedef enum
{
    CPLD_RST_EDFA_RESET = 0,
    CPLD_RST_EDFA_SOFT_RESET,
    CPLD_RST_RESET1,
    CPLD_RST_RESET2,
    CPLD_RST_MAIN_FPGA_RESET,
    CPLD_RST_CPLD_RESET,

    CPLD_RST_MODLE_MAX

} EN_CPLD_MODLE_RST;

typedef enum
{
    CPLD_USBSPI_CS_I350 = 0,
    CPLD_USBSPI_CS_EDFA1,
    CPLD_USBSPI_CS_EDFA2,

    CPLD_I2C_POWER,
    CPLD_I2C_FAN,

    CPLD_I2CSPI__MAX

} EN_CPLD_I2CSPI_CHOOSE;


typedef struct ST_CPLD_SPI_CTL_t
{
    union
    {
        struct
        {
            UINT b1START : 1;
            UINT           : 2;
            UINT b1CS    : 1;
            UINT b4SHIT  : 4;
            UINT b1LOOP  : 1;
            UINT           : 7;
            UINT b3MUX   : 3;
            UINT           : 13;
        };
        UINT uiCpldSpiCtl;
    };
} __attribute__((packed)) ST_CPLD_SPI_CTL;

#define SPI_WR_RETRY_MAX                (512)


#define FANCPLD_SLAVE_ADDR              (0x04)
#define FANCPLD_RW_RETRY                (5)


typedef enum
{
    CPLD_UART_EDFA1 = 0,
    CPLD_UART_EDFA2,
    CPLD_UART_WSS,
    CPLD_UART_OCM,
    CPLD_UART_EXTEND,
    CPLD_UART_POE,

    CPLD_UART_CNT

} EN_CPLD_UART_CHOOSE;

typedef enum
{
    CPLD_OP_TYPE_INIT = 0,
    CPLD_OP_TYPE_WRITE,
    CPLD_OP_TYPE_READ,
    CPLD_OP_TYPE_TX,
    CPLD_OP_TYPE_LAST_TX,
    CPLD_OP_TYPE_RX,
    CPLD_OP_TYPE_LAST_RX

} EN_CPLD_I2C_CTL;


typedef struct cpld_i2creg_ctl_t
{
    union
    {
        struct
        {
            UCHAR bOpRestart            : 1;
            UCHAR bOpType               : 3;
            UCHAR brsv                  : 2;
            UCHAR bAckErr               : 1;
            UCHAR bFinishFlag           : 1;
        };
        UCHAR ucCtl;
    };

}CPLD_I2CREG_CTL_T;


typedef enum
{
    CPLD0_VER = 0,
    CPLD1_VER,
    CPLDFAN_VER,

} EN_CPLD_VER_CHOOSE;

typedef enum
{
    SFP1_INDEX = 0,
    SFP2_INDEX,
    SFP_INDEX_MAX,

} EN_SFP_INDEX;

typedef enum
{
    SFP_LINKDOWN = 0,
    SFP_LINKUP,
    SFP_LINK_MAX,

} EN_SFP_LINK;


typedef enum
{
    SFP_ENABLE = 0,
    SFP_DISABLE,
    SFP_TX_NORMAL = 0,
    SFP_TX_FAULT,
    SFP_LOS_NORMAL = 0,
    SFP_LOS_ASSERT,
    SFP_ON_LINE = 0,
    SFP_OFF_LINE,

} EN_SFP_STATE;


typedef enum
{
    WSS_MODULE = 0,
    EXT_BROD_MODULE,
    EXT_MCU_MODULE,
    OCM_MODULE,

} EN_MODULE_INDEX;


typedef enum
{
    WSS_HARD_RESET = 0,
    WSS_HARD_NORMAL,
    WSS_SOFT_RESET = 0,
    WSS_SOFT_NORMAL,
    EXT_BROD_RESET = 0,
    EXT_BROD_NORMAL,
    EXT_MCU_RESET = 0,
    EXT_MCU_NORMAL,
    OCM_HARD_RESET = 0,
    OCM_HARD_NORMAL,

} EN_RESET_CTL;

typedef enum
{
    FAN0_IDX = 0,
    FAN1_IDX,
    FAN2_IDX,
    FAN3_IDX,

} EN_FAN_INDEX;

typedef enum
{
    POWER0_IDX = 0,
    POWER1_IDX,

} EN_POWER_INDEX;


typedef struct cpld_reg_pwr_online_watchdog_t
{
    union
    {
        struct
        {
            UCHAR bX86Wdg           : 1;         /* Bit0:  */
            UCHAR bDcPwrOnline      : 1;         /* Bit1:  */
            UCHAR bAcPwrOnline      : 1;         /* Bit2:  */
            UCHAR brsv              : 5;         /* Bit3-7:  */


        };
        UCHAR ucAlm;
    };

}CPLD_REG_PWR_ONLINE_WATCHDOG_T;

typedef struct cpld_reg_fan_online_watchdog_t
{
    union
    {
        struct
        {
            UCHAR bFan1Online      : 1;         /* Bit0:  */
            UCHAR bFan2Online      : 1;         /* Bit1:  */
            UCHAR bFan3Online      : 1;         /* Bit2:  */
            UCHAR brsv             : 5;         /* Bit3-7:  */


        };
        UCHAR ucAlm;
    };

}CPLD_REG_FAN_ONLINE_T;

typedef struct cpld_reg_pwr_t
{
    union
    {
        struct
        {
            UCHAR bCpld2         : 1;         /* Bit0:  */
            UCHAR b3v3           : 1;         /* Bit1:  */
            UCHAR b5vUsbEn1      : 1;         /* Bit2:  */
            UCHAR b5vUsbEn2      : 1;         /* bit3 */
            UCHAR bFan1PwrEn     : 1;         /* bit4 */
            UCHAR bFan2PwrEn     : 1;         /* bit5 */
            UCHAR bFan3PwrEn     : 1;         /* bit6 */
            UCHAR brsv           : 1;         /* Bit7:  */

        };
        UCHAR ucPwrReg;
    };

}CPLD_REG_PWR_T;

union semun {
    INT setval;
    struct semid_ds *buf;
    USHORT *array;
};


/**
 * @}
 */

/**************************************************************************************************/
/* @defgroup sdv_cpld_Exported_Functions                                                          */
/**************************************************************************************************/

extern INT SDV_SpiWriteRead(ULONG ulChip, UCHAR *pucBufWr, UCHAR *pucBufRd, ULONG ulLen);
extern INT SDV_WriteEdfaFpgaImg(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen);
extern INT SDV_SpiWriteDataToFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen);
extern INT SDV_SpiReadDataFromFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen);
extern INT SDV_EraseSpiChip(ULONG ulChip);
extern INT SDV_ProtectSpiChip(ULONG ulChip, BOOL bEn);

extern INT SDV_CpldInit(void);
extern INT SDV_CpldClose(void);

extern INT SDV_GetCpldValue(ULONG ulAddress, ULONG ulReadLen, UCHAR *pucBuf);
extern INT SDV_SetCpldValue(ULONG ulAddress, ULONG ulWriteLen, UCHAR *pucBuf);

extern INT SDV_CpldUartSwitch(int iIndex);
extern INT SDV_CpldI2CSPiChoose(INT iDx);

extern INT SDV_FanCpldRead(ULONG ulAddress, UCHAR *pucBuf);
extern INT SDV_FanCpldWrite(ULONG ulAddress, UCHAR *pucBuf);

extern INT SDV_CpldI2cWrite(unsigned int slaveAddr, unsigned char *pucWriteBuf, unsigned int iWriteLen, unsigned char *ucCmd, unsigned char ucCmdLen);
extern INT SDV_CpldI2cRead(unsigned int slaveAddr, unsigned char *pucReadBuf, unsigned int iReadLen, unsigned char *ucCmd, unsigned char ucCmdLen);

extern INT SDV_GetSfpOnline(INT iIndex, UCHAR *ucState);
extern INT SDV_GetSfpPortLink(INT iIndex, UCHAR *ucState);
extern INT SDV_GetSfpState(INT iIndex, UCHAR *ucState);
extern INT SDV_SetSfpState(INT iIndex, UCHAR ucState);
extern INT SDV_GetSfpPhyLink(INT iIndex, UCHAR *ucState);


extern INT SDV_CpldGetVer(INT iIndex, CHAR *pcVer, UCHAR ucLen);

extern INT SDV_CpldModReset(UCHAR ucModIndex, UCHAR ucRstMode);

extern INT  SDV_Mv6390Init(void);
extern INT SDV_CpldApsdDbg(void);

INT SDV_CpldI2cLock(void);
INT SDV_CpldI2cUnlock(void);
INT SDV_CpldI2cInit(void);
INT SDV_CpldI2cClose(void);


#ifdef __cplusplus
}
#endif

#endif

