
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <stdint.h>

#include "EApi.h"
#include "mscc_poe_host_comm_api.h"
#include "mscc_arch_functions.h"
#include "mscc_poe_util.h"
#include "sdv_poe.h"
#include "Susi4.h"

extern int8_t SDV_PCA9544_SelectCH(uint8_t ucAddress, uint8_t ucChannel);

/* DEF: PCA9544 channel ID */
#define PCA9544_CH_0                    0x04
#define PCA9544_CH_1                    0x05
#define PCA9544_CH_2                    0x06
#define PCA9544_CH_3                    0x07
#define PCA9544_CH_CLOSE                0x00

/* DEF: PCA9544 addr */
#define CHIP0_PCA9544_ADDR              0xE0
#define CHIP1_PCA9544_ADDR              0xE2

#define POE_I2C_MODE

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
    uint32_t addr;
    enum cmdtypeRank cmdType;
    uint32_t cmd;
    uint8_t *wdata;
    uint32_t wlen;
    uint8_t *rdata;
    uint32_t rlen;

};

U8 bI2C_Device_Address = 0x78;

int
SDV_PoeIICWrite(U8 I2C_Address, const U8 *Txdata, U16 num_write_length)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    struct I2CConf config;

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_3);

    memset(&config, 0, sizeof(config));

    config.addr = bI2C_Device_Address;
    config.cmd = SUSI_I2C_NO_CMD;
    config.wlen = num_write_length;
    config.wdata = (U8 *)Txdata;

    status = EApiI2CWriteTransfer(id, config.addr, config.cmd, config.wdata, config.wlen);
    if (status != SUSI_STATUS_SUCCESS) {
        printf("Write transfer failed. (0x%08X)\n", status);
        SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
        return -1;
    }

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);

    return 0;

}

int
SDV_PoeIICRead(U8 I2C_Address, _OUT U8 *Rxdata, U16 number_of_bytes_to_read, _OUT U16 *number_of_bytes_actually_read, READ_MODE_e eReadMode)
{
    SusiStatus_t status;
    SusiId_t id = SUSI_ID_I2C_EXTERNAL;
    struct I2CConf config;

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_3);

    memset(&config, 0, sizeof(config));

    config.addr = bI2C_Device_Address;
    config.cmd = SUSI_I2C_NO_CMD;
    config.rlen = number_of_bytes_to_read;
    config.rdata = Rxdata;

    status = EApiI2CReadTransfer(id, config.addr, config.cmd, config.rdata, config.rlen, config.rlen);
    if (status != SUSI_STATUS_SUCCESS) {
        printf("Read transfer failed. (0x%08X)\n", status);

        SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);
        return -1;
    }

    *number_of_bytes_actually_read = number_of_bytes_to_read;

    SDV_PCA9544_SelectCH(CHIP0_PCA9544_ADDR, PCA9544_CH_CLOSE);

    return 0;

}


int
SDV_PoeInit(void)
{
    PoE_Status_e ePoE_Status;
    /* Counters_t tCounters; */
    Init_Info_t tMscc_InitInfo;

#ifdef POE_I2C_MODE
    tMscc_InitInfo.fptr_write = SDV_PoeIICWrite;
    tMscc_InitInfo.fptr_read = SDV_PoeIICRead;
    tMscc_InitInfo.eDevice_Method = eDevice_Method_I2C;
#else
    tMscc_InitInfo.fptr_write = SDV_PoeUartWrite;
    tMscc_InitInfo.fptr_read = SDV_PoeUartRead;
    tMscc_InitInfo.eDevice_Method = eDevice_Method_UART_Sync;
#endif
    tMscc_InitInfo.wExtra_Timeout_mSec = 0;
    tMscc_InitInfo.ePOE_BOOL_Printf_Enable = ePOE_TRUE;

    ePoE_Status = MSCC_POE_Init(&tMscc_InitInfo);
    if (ePoE_Status != ePOE_STATUS_OK) {
        MSCC_PRINTF_INFO("Init ,status: %s \n\r", MSCC_POE_UTIL_GetErrDescription(ePoE_Status));
        goto PRE_FINISH;
    }
    /* clear PoE device Transmit buffer */
    ePoE_Status = MSCC_POE_Clear_POE_Device_Buffer(bI2C_Device_Address);

    /* in case RX buffer clear */
    if ((ePoE_Status == ePOE_STATUS_OK) || (ePoE_Status == ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT)) {
        printf("POE device: 0x%X OK, PoE device communicate correctly\n\r", bI2C_Device_Address);
    } else if (ePoE_Status == ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED) {
        printf("POE device: 0x%X OK, PoE device reset detected ,buffer cleaned and communicate correctly\n\r", bI2C_Device_Address);
    } else {
        MSCC_PRINTF_INFO("CLEAR_POE_DEVICE_BUFFER ,status: %s \n\r", MSCC_POE_UTIL_GetErrDescription(ePoE_Status));
        MSCC_PRINTF_INFO("\n\rIn case the PoE device firmware is damage ,please operate firmware update process !!!\n\r");
    }

#if 0
/* 4.4.7 Get Power Supply Voltage   **************************************************************** */
    printf("\n\r4.4.7 Get_Power_Supply_Voltage \n\r");

    eTransmitMsgStatus = Get_Power_Supply_Voltage(_INOUT & tPoeMsg, bI2C_Device_Address);
    if (eTransmitMsgStatus != eTransmitMsg_OK)
        continue;

    db.ulVmain_Voltage_mV = tPoeMsg.PoE_Msg_Prm_e.Get_Power_Supply_Voltage_t.Get.ulVmain_Voltage_mV;
    db.ulImain_Current_mA = tPoeMsg.PoE_Msg_Prm_e.Get_Power_Supply_Voltage_t.Get.ulImain_Current_mA;

    printf("Vmain Voltage %lf [V] \n\r", (double)db.ulVmain_Voltage_mV / 1000);
    printf("Imain current %lf [mA] \n\r", (double)db.ulImain_Current_mA);

#endif

PRE_FINISH:

    /* MSCC_POE_Get_Counters(_OUT & tCounters); */
    /* PrintCounters(&tCounters); */

    /* Close PoE API software operation */
    ePoE_Status = MSCC_POE_Exit();
    if (ePoE_Status != ePOE_STATUS_OK) {
        MSCC_PRINTF_INFO("Exit PoE API operation failed ,status: %s \n\r", MSCC_POE_UTIL_GetErrDescription(ePoE_Status));
    }

    return 0;
}

int
SDV_PoeClose(void)
{
    PoE_Status_e ePoE_Status;

    /* Close PoE API software operation */
    ePoE_Status = MSCC_POE_Exit();
    if (ePoE_Status != ePOE_STATUS_OK) {
        MSCC_PRINTF_INFO("Exit PoE API operation failed ,status: %s \n\r", MSCC_POE_UTIL_GetErrDescription(ePoE_Status));
    }

    return 0;
}
