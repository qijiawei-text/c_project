/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevInternal.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/


#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <memory>
#include <mutex>


#include <cstring>
#include <cstdio>

#include <stdint.h>

#include "util_inc.h"
#include "Delay.h"
#include "ByteOrder.h"

#include "DevData.h"
#include "DevBoardData.h"
#include "DevSlotData.h"
#include "DevEdfaData.h"
#include "DevTdcmData.h"
#include "DevWssData.h"
#include "DevOcmData.h"
#include "DevPam4Data.h"
#include "DevOtdrData.h"
#include "DevDceData.h"
#include "DevDgeData.h"
#include "DevOscData.h"
#include "DevOpsData.h"
#include "DevMcsData.h"
#include "DevMuxData.h"
#include "DevExtendData.h"
#include "DevOcsData.h"

#ifndef REGION_DES
#define REGION_DES(x)   (1)
#endif

/**************************************************************************************************/
/* definition of block hal log, cause hal log floods                                              */
/**************************************************************************************************/
/* @ {                                                                                            */
/* #define __block_hal_log */
#ifdef __block_hal_log
    #ifdef LOG_WRITE
        #undef LOG_WRITE
    #endif

    #define LOG_WRITE(enSeverity, pcStr, args ...) \
    do {                                       \
        printf(pcStr, ## args);                \
        printf("\n");                          \
    } while (0)
#endif
/* } @                                                                                            */


#define __RAINIER__     (0)

#define DEV_MAX_NUM                          32

struct CRegisterDev
{
    uint32_t uiDevType;            /* e.g:EDFA, OCM */
    uint32_t uiDevIdx;             /* e.g:EDFA0, EDFA1 */
    uint32_t uiHwType;    /* e.g:SPI, UART */
    uint32_t uiHwIdx;     /* e.g:SPI0,SPI1 */
    uint32_t uiHwSubIdx;  /* e.g:SPI0 CS1,I2C0 addr */
    uint32_t uiSubType;
};


struct CRegisterInfo
{
    uint32_t uiHwType;             /* e.g:UART,I2C,SPI */
    uint32_t uiHwIdx;
    uint32_t uiHwSubIdx;
    uint32_t uiDevAddr;
    uint32_t uiProto;

};


enum EN_LOGIC_DRIVER
{
    LOGIC_DRIVER0 = 0,
    LOGIC_DRIVER1,
    LOGIC_DRIVER2,

    LOGIC_DRIVER_NUM
};

enum EN_I2C_SWITCH
{
    I2C_SWITCH0 = 0,

    I2C_SWITCH_NUM
};

enum EN_I2C
{
    I2C0 = 0,
    I2C1,
    I2C2,
    I2C3,
    I2C4,
    I2C5,
    I2C6,

    I2C_NUM
};

enum EN_LOGIC
{
    LOGIC0 = 0,
    LOGIC1,
    LOGIC2,

    LOGIC_NUM
};


enum EN_OSC_BOARD
{
    OSC_BOARD0,

    OSC_BOARD_NUM
};


enum EN_LOGIC_I2C
{
    LOGIC_I2C0 = 0,

    LOGIC_I2C_NUM
};


enum EN_PCA
{
    PCA_DEV0 = 0,
    PCA_DEV1,
    PCA_DEV2,
    PCA_DEV3,

    PCA_NUM
};

enum EN_PCA_CHANNEL
{
    PCA_CH0 = 0x04,
    PCA_CH1 = 0x05,
    PCA_CH2 = 0x06,
    PCA_CH3 = 0x07,
};


enum EN_UART
{
    UART0 = 0,
    UART1,
    UART2,
    UART3,

    UART_NUM
};

enum EN_LOGIC_UART
{
    LOGIC_UART0 = 0,
    LOGIC_UART1,
    LOGIC_UART2,
    LOGIC_UART3,
    LOGIC_UART4,
    LOGIC_UART5,
    LOGIC_UART6,

    LOGIC_UART_CNT

};

enum EN_UART_USB
{
    UART_USB0 = 0,
    UART_USB1,

    UART_USB_NUM
};

enum EN_LOCAL_BUS
{
    LOCAL_BUS0 = 0,

    LOCAL_BUS_NUM
};

enum EN_FPGA
{
    FPGA_MAIN = 0,
    FPGA_CTRL,

    FPGA_NUM
};


enum EN_EEPROM
{
#if 0
    EEPROM_CHASSIS = 0,
    EEPROM_SCC,
#else
    EEPROM_CHASSIS = 0,
    EEPROM_SCC,
    EEPROM_SCC_BAK,
#endif
    /* EEPROM_PA, */
    /* EEPROM_BA, */
    EEPROM_FAN0,
    EEPROM_FAN1,
    EEPROM_FAN2,
    EEPROM_FAN3,

    EEPROM_NUM
};

enum EN_PSU
{
    PSU0 = 0,
    PSU1 = 1,

    PSU_NUM
};

enum EN_FAN_BOARD
{
    FAN_BOARD0 = 0,

    FAN_BOARD_CNT
};
enum EN_PSU_BOARD
{
    PSU_BOARD0 = 0,

    PSU_BOARD_NUM
};

enum EN_LED_BOARD
{
    LED_BOARD0 = 0,
    LED_BOARD1 = 4,
    LEN_BOARD_NUM = 7,
};

enum EN_CTRL_FAN
{
    CTRL_FAN0 = 0,
    CTRL_FAN1,
    CTRL_FAN2,
    CTRL_FAN3,

    CTRL_FAN_CNT
};




enum EN_SPI_SLAVE_SEL
{
    SPI_FLASH_NONE = 0,

    SPI_CTRL_FLASH = 1,
    SPI_MAIN_FLASH = 7,
    SPI_PA_FLASH = 3,
    SPI_BA_FLASH = 4,

    SPI_SLAVE_MAX
};

enum EN_MDIO
{
    MDIO0 = 0,

    MDIO_NUM
};

enum EN_NET_SWITCH
{
    NET_SWITCH_MV6390 = 0,

    NET_SWITCH_NUM
};

enum EN_MUX_DEMUX
{
    MUX_DEMUX = 0,

    MUX_DEMUX_NUM
};


enum EHwType
{
    HW_TYPE_DEF  = 0,
    HW_TYPE_UART,
    HW_TYPE_UART_USB,
    HW_TYPE_UART_CPLD,
    HW_TYPE_UART_FPGA,
    HW_TYPE_UART_LOGIC,
    HW_TYPE_UART_LOGIC_USB,
    HW_TYPE_MULTI_UART,

    HW_TYPE_FT,

    HW_TYPE_I2C,
    HW_TYPE_LOGIC_I2C,
    HW_TYPE_LOGIC_SWITCH_I2C,
    HW_TYPE_FT_I2C,
    HW_TYPE_PCA_I2C,
    HW_TYPE_SUSI4_I2C,

    HW_TYPE_SPI,
    HW_TYPE_LOGIC_SPI,
    HW_TYPE_LOGIC_SLAVE_SPI,
    HW_TYPE_LOGIC_MASTER_SPI,
    HW_TYPE_SPI_USB_FT,
    HW_TYPE_LOCAL_BUS,
    HW_TYPE_LOCAL_BUS_BITMAP,
    HW_TYPE_GPIO,
    HW_TYPE_MDIO,
    HW_TYPE_MDIO_FPGA,
    HW_TYPE_JTAG,

    HW_TYPE_FPGA,
    HW_TYPE_CPLD,
    HW_TYPE_LOGIC_DRIVER,
    HW_TYPE_LOGIC,
    HW_TYPE_OSC_BOARD,
    HW_TYPE_LED_BOARD,
    HW_TYPE_FAN_BOARD,
    HW_TYPE_PSU_BOARD,
    HW_TYPE_SLOT_BOARD,
    HW_TYPE_RESET_MODULE,

    HW_TYPE_I2C_SWITCH,

    HW_TYPE_USB,

    /* for simulation */
    HW_TYPE_VSPI,
    HW_TYPE_VIRTUAL,

    HW_TYPE_NONE,

    HW_TYPE_FIXED_OPS,
    HW_TYPE_FIXED_AMP
};

enum EProtoType  /* sub type */
{
    PROTO_TYPE_DEF  = 0,
    PROTO_TYPE_OA_CLI_SIG,
    PROTO_TYPE_OA_CLI_MUL,
    PROTO_TYPE_OA_CLI_K20,
    PROTO_TYPE_OA_OOP_SIG,
    PROTO_TYPE_OA_OOP_MUL,


    PROTO_TYPE_WSS,
    PROTO_TYPE_OCM,
    PROTO_TYPE_TDCM,
    PROTO_TYPE_PAM4,
    PROTO_TYPE_OPS,
    PROTO_TYPE_DGE,
    PROTO_TYPE_OSC,

    PROTO_TYPE_OCS,

    PROTO_TYPE_FPGA,
    PROTO_TYPE_CPLD,
    PROTO_TYPE_FAN_CPLD,

    PROTO_TYPE_E2PROM,
    PROTO_TYPE_TINY_E2PROM,
    PROTO_TYPE_LED
};


#define DEV_ADDR_NONE   0

#define ADDR_NULL                                0xFF
#define CH_NONE                                  0xFF

#define SUB_IDX_NONE                         0xFF
#define SWITCH_NONE                          0xFF

extern pthread_mutex_t m_spi_mutex[4];
#define   SLC_SPI_SELECT  0x34

