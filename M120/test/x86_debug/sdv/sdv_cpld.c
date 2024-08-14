
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sdv_cpld.h"
#include "sdv_usbspi.h"

#define READ(_a)        (*((volatile UCHAR *)(_a)))
#define WRITE(_a, _v)   (*(volatile UCHAR *)(_a) = (_v))

#define CPLD_BASE       (0xfed50000)
#define ALLOC_SIZE      (0x10000)

#define CPLD_SEM_KEY        (0xFFEEDDCC)
#define CPLD_I2C_SEM_KEY    (0xFFEEDD00)

static int g_fd = -1;
static void *map_base = NULL;

static INT m_iCpldLock = -1;
static INT m_iCpldI2cLock = -1;

static int
_phy_map(void)
{
    if (g_fd < 0) {
        g_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (g_fd < 0) {
            OPLK_LOG_PRINT_ERR("Fail to open /dev/mem fd=%08x\n", g_fd);
            return -1;
        }
    }
    map_base = mmap(0, ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, g_fd, CPLD_BASE);
    if (!map_base) {
        OPLK_LOG_PRINT_ERR("mmap phyaddr %#x failed ! \n", CPLD_BASE);
        close(g_fd);
        g_fd = -1;
        return -1;
    }

    return 0;
}

static void
_phy_umap(void)
{
    if (map_base)
        munmap(map_base, ALLOC_SIZE);

    if (g_fd > 0)
        close(g_fd);

    map_base = NULL;
    g_fd = -1;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_EdfaFlashBufEn                                                                   */
/* Descrp : Enable the E1&E2 Flash's buffer or not                                                */
/* Input  : bEn-- enable or not                                                                   */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_TURE :                                                                  */
/*              @arg OPLK_FALSE:                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_EdfaFlashBufEn(ULONG ulChip, BOOL bEn)
{
    CHAR acResp[512] = {0};
    INT i;

    #if 0
    if (EDFA1 == ulChip) {
        cmd_eable = SDV_EDFA_SET_E1_OE_ENDABLE
                cmd_disable = SDV_EDFA_SET_E1_OE_FREE
    } else {
        cmd_eable = SDV_EDFA_SET_E2_OE_ENDABLE
                cmd_disable = SDV_EDFA_SET_E2_OE_FREE
    }

    #endif
    for (i = 0; i < 5; i++) {
        memset(acResp, 0x00, sizeof(acResp));

        if (OPLK_FALSE != bEn) {
            /* if (OPLK_OK == SDV_EdfaSendCommand(SDV_EDFA_SET_E1_OE_ENDABLE, acResp, EDFA_READ_NUM)) */
            {
                if (NULL == strstr(acResp, "Enabled")) {
                    OPLK_LOG_PRINT_ERR("Invalid return value for edfa set oe 0");
                    continue;
                }

                /* OPLK_LOG_PRINT_DBG("Set EDFA OE enable success"); */
                return OPLK_OK;
            }
        } else {
            /* if (OPLK_OK == SDV_EdfaSendCommand(SDV_EDFA_SET_E1_OE_FREE, acResp, EDFA_READ_NUM)) */
            {
                if (NULL == strstr(acResp, "Disabled")) {
                    OPLK_LOG_PRINT_ERR("Invalid return value for edfa set oe 1");
                    continue;
                }

                /* OPLK_LOG_PRINT_DBG("Set EDFA OE free success"); */
                return OPLK_OK;
            }
        }

        OPLK_LOG_PRINT_ERR("Invalid return value for edfa set oe");
        usleep(20 * 1000);
    }

    OPLK_LOG_PRINT_ERR("Error: Set EDFA OE failure");
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : _SPI_WriteEnable                                                                      */
/* Descrp : SPI flash write enable                                                                */
/* Input  : ulChip-- the specified spi slave chip to enable                                       */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_WriteEnable(ULONG ulChip)
{
    UCHAR aucBufTx[32] = {0};
    UCHAR aucBufRx[32] = {0};
    UCHAR ucCmd = CMD_WRITE_ENABLE;
    INT i;


    for (i = 0; i < 3; i++) {
        if (OPLK_OK != SDV_UsbSpiWriteRead(&ucCmd, aucBufRx, 1)) {
            goto ERR_LABEL;
        }

        usleep(1);

        aucBufTx[0] = CMD_STATUS_READ;
        if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 2)) {
            goto ERR_LABEL;
        }

        if (0x02 == (aucBufRx[1] & 0x03)) {
            break;
        }
    }

    if (3 == i) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ChkStaReg                                                                        */
/* Descrp : Checks the SPI flash status                                                           */
/* Input  : ulChip-- the specified spi slave chip to check                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_ChkStaReg(ULONG ulChip)
{
    UCHAR aucBufTx[32] = {0};
    UCHAR aucBufRx[32] = {0};


    aucBufTx[0] = CMD_STATUS_READ;

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 2)) {
        goto ERR_LABEL;
    }

    if (aucBufRx[1] & 0x01) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ChkTime                                                                          */
/* Descrp : Waiting the busy of the spi device within the maximum time                            */
/* Input  : ulChip -- the specified spi slave chip to check                                       */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulTimes-- the maximum time to check, unit: ms                                         */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_ChkTime(ULONG ulChip, ULONG ulTimes)
{
    ULONG ulTryTimes = 0;


    while (ulTryTimes < ulTimes) {
        if (OPLK_OK == _SPI_ChkStaReg(ulChip)) {
            return OPLK_OK;
        }

        usleep(1 * 1000);
        ulTryTimes++;
    }

    OPLK_LOG_PRINT_ERR("flash is always busy.");
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ReadChipId                                                                       */
/* Descrp : Read the chip id of the specified spi device                                          */
/* Input  : ulChip-- the specified spi slave chip to write                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/* Output : pulId -- pointer to the returned chip id                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_ReadChipId(ULONG ulChip, ULONG *pulId)
{
    UCHAR aucBufTx[CHIP_PAGE_BYTES] = {0};
    UCHAR aucBufRx[CHIP_PAGE_BYTES] = {0};
    UCHAR ucChipId = FPGA_SPI_FLASH_ID_N25Q128;


    /* Read the spi flash chip id                                               */
    aucBufTx[0] = CMD_RDID;
    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 4)) {
        OPLK_LOG_PRINT_ERR("spi read chip id error");
        goto ERR_LABEL;
    }

    if ((0x01 == aucBufRx[1]) &&
            (0x20 == aucBufRx[2]) &&
            (0x18 == aucBufRx[3])) {
        ucChipId = FPGA_SPI_FLASH_ID_S25FL129P;
    } else if ((0x20 == aucBufRx[1]) &&
            (0xBA == aucBufRx[2]) &&
            (0x18 == aucBufRx[3])) {
        ucChipId = FPGA_SPI_FLASH_ID_N25Q128;
    } else {
        OPLK_LOG_PRINT_ERR("spi chip id error, no recognised flash:%02x%02x%02x", aucBufRx[1], aucBufRx[2], aucBufRx[3]);
        goto ERR_LABEL;
    }

    *pulId = ucChipId;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ReadLoadBit                                                                      */
/* Descrp : Read the load bit                                                                     */
/* Input  : pucBuf    -- pointer to the image file to check                                       */
/* Output : pulLoadBit-- pointer to the returned load bit                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_LOAD_BIT_1:                                                         */
/*              @arg FPGA_SPI_LOAD_BIT_4:                                                         */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_ReadLoadBit(UCHAR *pucBuf, ULONG *pulLoadBit)
{
    ULONG ulLoadBit = FPGA_SPI_LOAD_BIT_1;
    ULONG ulTagCnt = 0;
    INT i;


    if ((NULL == pucBuf) ||
            (NULL == pulLoadBit)) {
        goto ERR_LABEL;
    }

    for (i = 0; i < 1024; i++) {
        if (0xAA == pucBuf[i]) {
            if ((0x99 == pucBuf[i + 1]) &&
                    (0x55 == pucBuf[i + 2]) &&
                    (0x66 == pucBuf[i + 3])) {
                i += 4;
                ulTagCnt++;
            }
        }

        if (ulTagCnt > 1) {
            break;
        }
    }

    if (0 == ulTagCnt) {
        goto ERR_LABEL;
    } else if (1 == ulTagCnt) {
        ulLoadBit = FPGA_SPI_LOAD_BIT_1;
    } else {
        ulLoadBit = FPGA_SPI_LOAD_BIT_4;
    }

    *pulLoadBit = ulLoadBit;

    /* OPLK_LOG_PRINT_TRC("spi load bit is %ld", ulLoadBit); */
    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_FlashProtect                                                                     */
/* Descrp : SPI flash device protect                                                              */
/* Input  : ulChip-- the specified spi slave chip to protect                                      */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulId  -- the specified spi flash chip id to unprotect                                 */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FLASH_ID_N25Q128  :                                                 */
/*              @arg FPGA_SPI_FLASH_ID_S25FL129P:                                                 */
/*        : ulBit -- the specified spi flash load mode                                            */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_LOAD_BIT_1:                                                         */
/*              @arg FPGA_SPI_LOAD_BIT_4:                                                         */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_FlashProtect(ULONG ulChip, ULONG ulId, ULONG ulBit)
{
    UCHAR aucBufTx[32] = {0};
    UCHAR aucBufRx[32] = {0};
    UCHAR ucLen = 0;


    if (OPLK_OK != _SPI_WriteEnable(ulChip)) {
        OPLK_LOG_PRINT_ERR("write enable fail.");
        return OPLK_ERROR;
    }

    switch (ulId) {
    case FPGA_SPI_FLASH_ID_N25Q128:
        aucBufTx[0] = CMD_STATUS_WRITE;
        aucBufTx[1] = 0x00;
        aucBufTx[2] = 0x00;

        ucLen = 2;
        break;

    case FPGA_SPI_FLASH_ID_S25FL129P:
        aucBufTx[0] = CMD_STATUS_WRITE;
        aucBufTx[1] = 0x00;
        if (FPGA_SPI_LOAD_BIT_1 == ulBit) {
            aucBufTx[2] = 0x00;
        } else if (FPGA_SPI_LOAD_BIT_4 == ulBit) {
            aucBufTx[2] = 0x02;
        } else {
            goto ERR_LABEL;
        }

        ucLen = 3;
        break;

    default:
        OPLK_LOG_PRINT_ERR("chip id error");
        goto ERR_LABEL;
    }

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, ucLen)) {
        OPLK_LOG_PRINT_ERR("spi write and read interface error");
        goto ERR_LABEL;
    }

    if (_SPI_ChkTime(ulChip, PROTECT_TRY_TIMES)) {
        OPLK_LOG_PRINT_ERR("time out");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_FlashUnProtect                                                                   */
/* Descrp : SPI flash device unprotect                                                            */
/* Input  : ulChip-- the specified spi slave chip to unprotect                                    */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulId  -- the specified spi flash chip id to unprotect                                 */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FLASH_ID_N25Q128  :                                                 */
/*              @arg FPGA_SPI_FLASH_ID_S25FL129P:                                                 */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_FlashUnProtect(ULONG ulChip, ULONG ulId)
{
    UCHAR aucBufTx[32] = {0};
    UCHAR aucBufRx[32] = {0};
    UCHAR ucLen = 0;


    if (OPLK_OK != _SPI_WriteEnable(ulChip)) {
        OPLK_LOG_PRINT_ERR("write enable fail.");
        return OPLK_ERROR;
    }

    aucBufTx[0] = CMD_STATUS_WRITE;
    aucBufTx[1] = 0x00;
    aucBufTx[2] = 0x00;

    switch (ulId) {
    case FPGA_SPI_FLASH_ID_N25Q128:
        ucLen = 2;
        break;

    case FPGA_SPI_FLASH_ID_S25FL129P:
        ucLen = 3;
        break;

    default:
        OPLK_LOG_PRINT_ERR("chip id error");
        goto ERR_LABEL;
    }

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, ucLen)) {
        OPLK_LOG_PRINT_ERR("SPI Write error.");
        goto ERR_LABEL;
    }

    if (_SPI_ChkTime(ulChip, PROTECT_TRY_TIMES)) {
        OPLK_LOG_PRINT_ERR("time out.");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_EraseBy64K                                                                       */
/* Descrp : Erase the specified SPI Flash by 64KB                                                 */
/* Input  : ulChip      -- the specified spi slave chip to erase                                  */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulSectorAddr-- the specified sector address to erase, must be 64KB align              */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_EraseBy64K(ULONG ulChip, UINT ulSectorAddr)
{
    UCHAR aucBufTx[32] = {0};
    UCHAR aucBufRx[32] = {0};


    if (OPLK_OK != _SPI_WriteEnable(ulChip)) {
        OPLK_LOG_PRINT_ERR("write enable fail.");
        goto ERR_LABEL;
    }

    aucBufTx[0] = CMD_ERASE_64K;
    aucBufTx[1] = ulSectorAddr >> 16;
    aucBufTx[2] = ulSectorAddr >> 8;
    aucBufTx[3] = ulSectorAddr;

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 4)) {
        OPLK_LOG_PRINT_ERR("spi write and read interface error");
        goto ERR_LABEL;
    }

    if (_SPI_ChkTime(ulChip, ERASE64K_TRY_TIMES)) {
        OPLK_LOG_PRINT_ERR("time out.");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_WriteOnePage                                                                     */
/* Descrp : Writea a page of data to spi flash device                                             */
/* Input  : ulChip    -- the specified spi slave chip to write                                    */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulPageAddr-- the specified page address to write, must be page(256B) align            */
/*        : pucBuf    -- pointer to the source data buffer to write                               */
/*        : ulLen     -- the length of data to write, unit: byte                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_WriteOnePage(ULONG ulChip, ULONG ulPageAddr, UCHAR *pucBuf, ULONG ulLen)
{
    UCHAR aucBufTx[512] = {0};
    UCHAR aucBufRx[512] = {0};


    if ((ulPageAddr > CHIP_TOTAL_BYTES) ||
            (ulLen > CHIP_PAGE_BYTES) ||
            (NULL == pucBuf)) {
        OPLK_LOG_PRINT_ERR("input parameter wrong!");
        return OPLK_ERROR;
    }

    if (OPLK_OK != _SPI_WriteEnable(ulChip)) {
        OPLK_LOG_PRINT_ERR("write enable fail.");
        return OPLK_ERROR;
    }

    aucBufTx[0] = CMD_WRITE;
    aucBufTx[1] = ulPageAddr >> 16;
    aucBufTx[2] = ulPageAddr >> 8;
    aucBufTx[3] = ulPageAddr;
    memcpy(aucBufTx + 4, pucBuf, ulLen);

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 4 + ulLen)) {
        OPLK_LOG_PRINT_ERR("spi write and read interface error");
        goto ERR_LABEL;
    }

    if (_SPI_ChkTime(ulChip, PAGE_WRITE_TRY_TIMES)) {
        OPLK_LOG_PRINT_ERR("time out.");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ReadOnePage                                                                      */
/* Descrp : Read a page of data from spi flash device                                             */
/* Input  : ulChip    -- the specified spi slave chip to read                                     */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulPageAddr-- the specified page address to read, must be page(256B) align             */
/*        : ulLen     -- the length of data to read, unit: byte                                   */
/* Output : pucBuf    -- pointer to the returned data buffer                                      */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
_SPI_ReadOnePage(ULONG ulChip, ULONG ulPageAddr, UCHAR *pucBuf, ULONG ulLen)
{
    UCHAR aucBufTx[512] = {0};
    UCHAR aucBufRx[512] = {0};


    if ((ulPageAddr > CHIP_TOTAL_BYTES) ||
            (ulLen > CHIP_PAGE_BYTES) ||
            (NULL == pucBuf)) {
        OPLK_LOG_PRINT_ERR("input parameter wrong!");
        return OPLK_ERROR;
    }

    aucBufTx[0] = CMD_READ;
    aucBufTx[1] = ulPageAddr >> 16;
    aucBufTx[2] = ulPageAddr >> 8;
    aucBufTx[3] = ulPageAddr;

    if (OPLK_OK != SDV_UsbSpiWriteRead(aucBufTx, aucBufRx, 4 + ulLen)) {
        OPLK_LOG_PRINT_ERR("spi write and read interface error");
        goto ERR_LABEL;
    }

    if (_SPI_ChkTime(ulChip, PAGE_WRITE_TRY_TIMES)) {
        OPLK_LOG_PRINT_ERR("time out.");
        goto ERR_LABEL;
    }

    memcpy(pucBuf, aucBufRx + 4, ulLen);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_ReadDataFromFlash                                                                */
/* Descrp : Read a mount of data from the spi flash device                                        */
/* Input  : ulChip-- the specified spi slave chip to read                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulAddr-- the start address in spi flash to read                                       */
/*        : ulLen -- the length of data to read, unit: byte                                       */
/* Output : pucBuf-- pointer to the source data buffer to read                                    */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
_SPI_ReadDataFromFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    UINT nDataPageLen = 0;
    UINT nDataIndex = 0;

    if ((NULL == pucBuf) ||
            (ulAddr > CHIP_TOTAL_BYTES) ||
            ((ulAddr + ulLen) > CHIP_TOTAL_BYTES)) {
        OPLK_LOG_PRINT_ERR("input parameter wrong!");
        goto ERR_LABEL;
    }

    /* Enable the spi flash buffer chip firstly, then ppc can handle the spi    */
    /* flash chip                                                               */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_TRUE)) {
        OPLK_LOG_PRINT_ERR("spi buffer enable error!");
        goto ERR_LABEL;
    }

    while (0 != ulLen) {
        if (ulLen < CHIP_PAGE_BYTES) {
            nDataPageLen = ulLen;
        } else {
            nDataPageLen = CHIP_PAGE_BYTES;
        }

        if ((_SPI_ReadOnePage(ulChip, ulAddr + nDataIndex, pucBuf + nDataIndex, nDataPageLen))) {
            OPLK_LOG_PRINT_ERR("read one page error.");
            goto ERR_LABEL;
        }

        nDataIndex += nDataPageLen;
        ulLen -= nDataPageLen;
    }

    /* Free the spi flash buffer chip after the spi flash operation             */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE)) {
        OPLK_LOG_PRINT_ERR("spi buffer disable error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE);
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_WriteDataToFlash                                                                 */
/* Descrp : Writes a mount of data to the spi flash device                                        */
/* Input  : ulChip-- the specified spi slave chip to write                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/*        : ulAddr-- the specified address to write, page(256B) align                             */
/*        : pucBuf-- pointer to the source data buffer to write                                   */
/*        : ulLen -- the length of data to write, unit: byte                                      */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
_SPI_WriteDataToFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    UCHAR aucBufRx[CHIP_PAGE_BYTES] = {0};
    UINT nDataPageLen = 0;
    UINT nDataIndex = 0;
    UINT nBlockEraseAddr = 0;
    UINT nBlockEraseNum = 0;

    ULONG ulId = FPGA_SPI_FLASH_ID_N25Q128;
    ULONG ulLoadBit = FPGA_SPI_LOAD_BIT_1;
    INT iRetVal = OPLK_ERROR;


    if ((NULL == pucBuf) ||
            (ulAddr > CHIP_TOTAL_BYTES) ||
            ((ulAddr + ulLen) > CHIP_TOTAL_BYTES)) {
        OPLK_LOG_PRINT_ERR("input parameter wrong!");
        goto ERR_LABEL;
    }

    /* Enable the spi flash buffer chip firstly, then ppc can handle the spi    */
    /* flash chip                                                               */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_TRUE)) {
        OPLK_LOG_PRINT_ERR("spi buffer enable error!");
        goto ERR_LABEL;
    }
    sleep(1);
    /* Read the spi flash chip id                                               */
    iRetVal = _SPI_ReadChipId(ulChip, &ulId);
    if (OPLK_OK != iRetVal) {
        OPLK_LOG_PRINT_ERR("spi read chip id error");
        goto ERR_LABEL;
    }

    /* Read spi bus load mode                                                   */
    if (OPLK_OK != _SPI_ReadLoadBit(pucBuf, &ulLoadBit)) {
        /* OPLK_LOG_PRINT_OPT("spi read load mode error, use 1 bit load mode"); */
    }

    iRetVal = _SPI_FlashUnProtect(ulChip, ulId);
    if (OPLK_OK != iRetVal) {
        OPLK_LOG_PRINT_ERR("spi flash unprotect error!");
        goto ERR_LABEL;
    }

    nBlockEraseAddr = ulAddr;
    nBlockEraseNum = (ulLen + CHIP_BLOCK64K_LEN - 1) / CHIP_BLOCK64K_LEN;

    do {
        iRetVal = _SPI_EraseBy64K(ulChip, nBlockEraseAddr);
        if (OPLK_OK != iRetVal) {
            OPLK_LOG_PRINT_ERR("spi device erase by 64KB error.");
            goto ERR_LABEL;
        }

        nBlockEraseAddr += CHIP_BLOCK64K_LEN;
    }
    while (--nBlockEraseNum);

    /* OPLK_LOG_PRINT_OPT("spi device %ld write data to %s flash addr 0x%06lX", ulChip, pcFlashType, ulAddr); */

    while (0 != ulLen) {
        if (ulLen < CHIP_PAGE_BYTES) {
            nDataPageLen = ulLen;
        } else {
            nDataPageLen = CHIP_PAGE_BYTES;
        }

        iRetVal = _SPI_WriteOnePage(ulChip, ulAddr + nDataIndex, pucBuf + nDataIndex, nDataPageLen);
        if (OPLK_OK != iRetVal) {
            OPLK_LOG_PRINT_ERR("spi write one page error.");
            goto ERR_LABEL;
        }

        memset(aucBufRx, 0, sizeof(aucBufRx));

        iRetVal = _SPI_ReadOnePage(ulChip, ulAddr + nDataIndex, aucBufRx, nDataPageLen);
        if (OPLK_OK != iRetVal) {
            OPLK_LOG_PRINT_ERR("spi read one page error.");
            goto ERR_LABEL;
        }

        /* Check                                                                */
        if (0 != memcmp(aucBufRx, pucBuf + nDataIndex, nDataPageLen)) {
            OPLK_LOG_PRINT_ERR("spi read one page cmp error, index = %d.", nDataIndex);
            goto ERR_LABEL;
        }

        nDataIndex += nDataPageLen;
        ulLen -= nDataPageLen;
    }

    iRetVal = _SPI_FlashProtect(ulChip, ulId, ulLoadBit);
    if (OPLK_OK != iRetVal) {
        OPLK_LOG_PRINT_ERR("spi flash protect error!");
        goto ERR_LABEL;
    }

    /* Free the spi flash buffer chip after the spi flash operation             */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE)) {
        OPLK_LOG_PRINT_ERR("spi buffer disable error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE);
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : _SPI_EraseFlash                                                                       */
/* Descrp : Erases a spi flash                                                                    */
/* Input  : ulChip-- the specified spi slave chip to erase                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_CLK_16M:                                                            */
/*              @arg FPGA_SPI_CLK_4M :                                                            */
/*              @arg FPGA_SPI_CLK_2M :                                                            */
/*              @arg FPGA_SPI_CLK_1M :                                                            */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
_SPI_EraseFlash(ULONG ulChip)
{
    ULONG ulId = 0;
    INT i;

    sleep(1);

    /* Enable the spi flash buffer chip firstly, then ppc can handle the spi    */
    /* flash chip                                                               */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_TRUE)) {
        OPLK_LOG_PRINT_ERR("spi buf enable error!");
        goto ERR_LABEL;
    }

    /* Read the spi flash chip id                                               */
    if (OPLK_OK != _SPI_ReadChipId(ulChip, &ulId)) {
        OPLK_LOG_PRINT_ERR("spi read chip id error");
        goto ERR_LABEL;
    }

    if (OPLK_OK != _SPI_FlashUnProtect(ulChip, ulId)) {
        OPLK_LOG_PRINT_ERR("UnProtect error!");
        goto ERR_LABEL;
    }

    for (i = 0; i < CHIP_TOTAL_BYTES / CHIP_BLOCK64K_LEN; i++) {
        if (OPLK_OK != _SPI_EraseBy64K(ulChip, CHIP_BLOCK64K_LEN * i)) {
            OPLK_LOG_PRINT_ERR("erase by 64k error.");
            goto ERR_LABEL;
        }
    }

    /* Free the spi flash buffer chip after the spi flash operation             */
    if (OPLK_OK != _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE)) {
        OPLK_LOG_PRINT_ERR("spi buf disable error!");
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    _SPI_EdfaFlashBufEn(ulChip, OPLK_FALSE);
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_SpiWriteDataToFlash                                                               */
/* Descrp : Writes a mount of data to the spi flash device                                        */
/* Input  : ulChip-- the specified spi slave chip to write                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*        : ulAddr-- the specified address to write, page(256B) align                             */
/*        : pucBuf-- pointer to the source data buffer to write                                   */
/*        : ulLen -- the length of data to write, unit: byte                                      */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_SpiWriteDataToFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{

    return _SPI_WriteDataToFlash(ulChip, ulAddr, pucBuf, ulLen);
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_SpiReadDataFromFlash                                                              */
/* Descrp : Read a mount of data from the spi flash device                                        */
/* Input  : ulChip-- the specified spi slave chip to read                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/*        : ulAddr-- the start address in spi flash to read                                       */
/*        : ulLen -- the length of data to read, unit: byte                                       */
/* Output : pucBuf-- pointer to the source data buffer to read                                    */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_SpiReadDataFromFlash(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{

    return _SPI_ReadDataFromFlash(ulChip, ulAddr, pucBuf, ulLen);
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_WriteEdfaFpgaImg                                                                  */
/* Descrp : Writes a image to the edfa fpga flash                                                 */
/* Input  : ulAddr-- the start address of the spi flash to write                                  */
/*        : pucBuf-- pointer to the source image buffer to write                                  */
/*        : ulLen -- the length of data to write, unit: byte                                      */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_WriteEdfaFpgaImg(ULONG ulChip, ULONG ulAddr, UCHAR *pucBuf, ULONG ulLen)
{
    return SDV_SpiWriteDataToFlash(ulChip, ulAddr, pucBuf, ulLen);
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_EraseSpiChip                                                                      */
/* Descrp : Erases a spi flash                                                                    */
/* Input  : ulChip-- the specified spi slave chip to erase                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg FPGA_SPI_FPGA_FLASH:                                                         */
/*              @arg FPGA_SPI_EDFA_FLASH:                                                         */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_EraseSpiChip(ULONG ulChip)
{

    return _SPI_EraseFlash(ulChip);
}

INT SDV_CpldLock(void)
{
    struct sembuf stSembuf = {0,-1,SEM_UNDO};
    
    if(m_iCpldLock < 0)
    {
        printf("cpld lock need init.\n");
        return -1;
    }
    
    if (semop(m_iCpldLock, &stSembuf, 1) < 0)
    {
        printf("SDV_CpldLock error.\n");
        return -1;
    }

    return 0;
}

INT SDV_CpldUnLock(void)
{
    struct sembuf stSembuf = {0,1,SEM_UNDO};

    if(m_iCpldLock < 0)
    {
        printf("cpld lock need init.\n");
        return -1;
    }
        
    if (semop(m_iCpldLock, &stSembuf, 1) < 0)
    {
        printf("SDV_CpldUnLock error.\n");
        return -1;
    }

    return 0;
}


INT
SDV_CpldInit(void)
{
    union semun arg;

    if(m_iCpldLock >= 0)
        return 0;

    m_iCpldLock = semget((key_t)CPLD_SEM_KEY, 1, IPC_CREAT | 0666);

    if(m_iCpldLock < 0)
    {
        OPLK_LOG_PRINT_ERR("semget error.");
        return -1;
    }

    arg.setval = 1;
    if(-1 == semctl(m_iCpldLock,0,SETVAL,arg))
    {
        OPLK_LOG_PRINT_ERR("semctl error.");
        return -1;
    }

    if (_phy_map())
        return -1;

    return 0;
}

INT
SDV_CpldClose(void)
{
    _phy_umap();

    if(m_iCpldLock < 0)
    {
        printf("cpld lock need init.\n");
        return -1;
    }

    if(-1 == semctl(m_iCpldLock,0,IPC_RMID,0))
    {
       OPLK_LOG_PRINT_ERR("semctl error.");
       return -1;
    }

    m_iCpldLock= -1;
    
    return 0;
}

INT
SDV_GetCpldValue(ULONG ulAddress, ULONG ulReadLen, UCHAR *pucBuf)
{
    INT i = 0;

    if (NULL == map_base) {
        if (SDV_CpldInit())
            return -1;
    }

    if (NULL == pucBuf)
        return -1;

    if ((ulAddress + ulReadLen) >= ALLOC_SIZE) {
        OPLK_LOG_PRINT_ERR("address over length \n");
        return -1;
    }

    SDV_CpldLock();

    for (i = 0; i < ulReadLen; i++) {
        pucBuf[i] = READ(map_base + ulAddress + i);
    }
    
    SDV_CpldUnLock();
    
    return 0;
}


INT
SDV_SetCpldValue(ULONG ulAddress, ULONG ulWriteLen, UCHAR *pucBuf)
{
    INT i = 0;

    if (NULL == map_base) {
        if (SDV_CpldInit())
            return -1;
    }

    if (NULL == pucBuf)
        return -1;

    if ((ulAddress + ulWriteLen) >= ALLOC_SIZE) {
        OPLK_LOG_PRINT_ERR("address over length \n");
        return -1;
    }

    SDV_CpldLock();

    for (i = 0; i < ulWriteLen; i++) {
        WRITE(map_base + ulAddress + i, pucBuf[i]);
    }

    SDV_CpldUnLock();

    return 0;
}

INT
SDV_MdioReadReg(unsigned int phyAddr, unsigned int regAddr, unsigned short *usData)
{
    UCHAR ucDataH = 0;
    UCHAR ucDataL = 0;
    UCHAR ucSta = 0;
    INT iTimeOut = 0;
    UCHAR ucDataW = 0;

    ucDataW = 0;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);
    usleep(1000);

    SDV_SetCpldValue(CPLD1_MDIO_PHYAD, 1, (UCHAR *)(&phyAddr));
    usleep(1000);
    SDV_SetCpldValue(CPLD1_MDIO_REGAD, 1, (UCHAR *)(&regAddr));
    usleep(1000);

    ucDataW = 0x02;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);

    usleep(10);

    SDV_GetCpldValue(CPLD1_MDIO_OP, 1, &ucSta);
    while (!(ucSta & 0x80)) {
        usleep(1000);
        if (iTimeOut++ > 1000) {
            OPLK_LOG_PRINT_ERR("mdio read timeout!\n");
            return -1;
        }

        SDV_GetCpldValue(CPLD1_MDIO_OP, 1, &ucSta);
    }

    SDV_GetCpldValue(CPLD1_MDIO_RX_DATA_H, 1, &ucDataH);
    usleep(1000);
    SDV_GetCpldValue(CPLD1_MDIO_RX_DATA_L, 1, &ucDataL);

    ucDataW = 0;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);

    *usData = (ucDataH << 8 | ucDataL);

    return 0;
}

INT
SDV_MdioWriteReg(unsigned int phyAddr, unsigned int regAddr, unsigned int data)
{
    UCHAR ucSta = 0;
    INT iTimeOut = 0;
    UCHAR ucDataW = 0;

    ucDataW = 0;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);
    usleep(1000);

    SDV_SetCpldValue(CPLD1_MDIO_PHYAD, 1, (UCHAR *)(&phyAddr));
    usleep(1000);
    SDV_SetCpldValue(CPLD1_MDIO_REGAD, 1, (UCHAR *)(&regAddr));
    usleep(1000);
    ucDataW = (UCHAR)((data & 0xff00) >> 8);
    SDV_SetCpldValue(CPLD1_MDIO_TX_DATA_H, 1, &ucDataW);
    usleep(1000);
    ucDataW = (data & 0xff);
    SDV_SetCpldValue(CPLD1_MDIO_TX_DATA_L, 1, &ucDataW);

    ucDataW = 0x03;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);
    usleep(1000);

    SDV_GetCpldValue(CPLD1_MDIO_OP, 1, &ucSta);
    while (!(ucSta & 0x80)) {
        usleep(1000);
        if (iTimeOut++ > 1000) {
            OPLK_LOG_PRINT_ERR("\nmdio write timeout!\n");
            return -1;
        }

        SDV_GetCpldValue(CPLD1_MDIO_OP, 1, &ucSta);
    }

    ucDataW = 0;
    SDV_SetCpldValue(CPLD1_MDIO_OP, 1, &ucDataW);

    return 0;
}

/* access switch register */
INT
msdMultiAddrRead(UCHAR phyAddr, UCHAR regAddr, USHORT *value)
{
    USHORT smiReg;
    volatile unsigned int timeOut; /* in 100MS units */

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if ((SDV_MdioReadReg(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &smiReg)) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);

    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_22 << MSD_SMI_OP_BIT) |
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

    if ((SDV_MdioWriteReg(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, smiReg)) != 0) {
        return -1;
    }

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if ((SDV_MdioReadReg(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &smiReg)) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);

    if ((SDV_MdioReadReg(MV6390X_PHY_ADDR, MSD_REG_SMI_DATA, &smiReg)) != 0) {
        return -1;
    }
    *value = smiReg;

    return 0;
}

INT
msdMultiAddrWrite(UCHAR phyAddr, UCHAR regAddr, USHORT value)
{
    USHORT smiReg;
    volatile unsigned int timeOut; /* in 100MS units */

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if ((SDV_MdioReadReg(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &smiReg)) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);

    if ((SDV_MdioWriteReg(MV6390X_PHY_ADDR, MSD_REG_SMI_DATA, value)) != 0) {
        return -1;
    }
    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

    if ((SDV_MdioWriteReg(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, smiReg)) != 0) {
        return -1;
    }

    return 0;
}

/* access internel serdes */
INT
msdSetSMIC45PhyReg(UCHAR devAddr, UCHAR phyAddr, USHORT regAddr, USHORT data)
{
    unsigned int timeOut;
    USHORT smiReg;

    /* first check that it is not busy */
    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);


    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, regAddr) != 0) {
        return -1;
    }
    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE_ADDR << MSD_SMI_OP_BIT) |
            (devAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);


    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, data) != 0) {
        return -1;
    }
    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
            (devAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    return 0;
}

INT
msdGetSMIC45PhyReg(UCHAR devAddr, UCHAR phyAddr, USHORT regAddr, USHORT *data)
{
    unsigned int timeOut;
    USHORT smiReg;
    USHORT tmp;

    /* first check that it is not busy */
    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);


    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, regAddr) != 0) {
        return -1;
    }
    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE_ADDR << MSD_SMI_OP_BIT) |
            (devAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_45 << MSD_SMI_OP_BIT) |
            (devAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);


    if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, &tmp) != 0) {
        return -1;
    }

    *data = tmp;

    return 0;
}

/* access internel phy */
INT
msdSetSMIC22PhyReg(UCHAR devAddr, USHORT regAddr, USHORT data)
{
    unsigned int timeOut;
    USHORT smiReg;


    /* first check that it is not busy */
    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);


    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, data) != 0) {
        return -1;
    }
    smiReg = MSD_SMI_BUSY | (devAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    return 0;
}

INT
msdGetSMIC22PhyReg(UCHAR devAddr, USHORT regAddr, USHORT *data)
{
    unsigned int timeOut;
    USHORT smiReg;

    /* first check that it is not busy */
    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);

    smiReg = MSD_SMI_BUSY | (devAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_22 << MSD_SMI_OP_BIT) |
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

    if (msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, smiReg) != 0) {
        return -1;
    }

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &smiReg) != 0) {
            return -1;
        }
        if (timeOut-- < 1 ) {
            return -1;
        }
    } while (smiReg & MSD_SMI_BUSY);

    if (msdMultiAddrRead(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, &smiReg) != 0) {
        return -1;
    }
    *data = smiReg;

    return 0;
}

INT
msdWritePagedPhyReg(UCHAR portNum, UCHAR pageNum, UCHAR regAddr, USHORT data)
{
    int retVal;
    retVal = msdSetSMIC22PhyReg(portNum, 22, pageNum);
    if (retVal != 0)
        return retVal;
    retVal = msdSetSMIC22PhyReg(portNum, regAddr, data);
    return retVal;
}

INT
msdReadPagedPhyReg(UCHAR portNum, UCHAR pageNum, UCHAR regAddr, USHORT *data)
{
    int retVal;
    retVal = msdSetSMIC22PhyReg(portNum, 22, pageNum);
    if (retVal != 0)
        return retVal;
    retVal = msdGetSMIC22PhyReg(portNum, regAddr, data);
    return retVal;
}

UCHAR
msdlport2port(USHORT portVec, UINT port)
{
    UCHAR hwPort, tmpPort;

    tmpPort = hwPort = 0;

    while (portVec) {
        if (portVec & 0x1) {
            if ((UINT)tmpPort == port)
                break;
            tmpPort++;
        }
        hwPort++;
        portVec >>= 1;
    }

    if (!portVec)
        hwPort = MSD_INVALID_PORT;

    return hwPort;
}


UCHAR
msdlport2phy(UINT port)
{
    UCHAR hwPort;

    if (VALIDPORTVEC & (1 << port)) {
        hwPort = MSD_LPORT_2_PORT(port);
        hwPort += BASEPHYREGADDR;
    } else
        hwPort = (UCHAR)MSD_INVALID_PHY;

    return hwPort;
}


/* access extern phy */
INT
msdSetSMIPhyXMDIOReg(UCHAR portNum, UCHAR devAddr, UCHAR regAddr, USHORT data)
{
    int retVal;
    USHORT tmpData;
    UCHAR hwPort;

    hwPort = MSD_LPORT_2_PHY(portNum);
    if (hwPort == MSD_INVALID_PORT) {
        return -1;
    }

    /* Set MMD access control address is Address | devAddr */
    tmpData = MSD_PHY_MMD_ADDR | devAddr;
    retVal = msdWritePagedPhyReg(hwPort, 0, 13, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Set MMD access Data is regAddr */
    tmpData = regAddr;
    retVal = msdWritePagedPhyReg(hwPort, 0, 14, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Set MMD access control address is Data no inc | devAddr */
    tmpData = MSD_PHY_MMD_DATA_NO_INC | devAddr;
    retVal = msdWritePagedPhyReg(hwPort, 0, 13, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Write MMD access Data */
    retVal = msdWritePagedPhyReg(hwPort, 0, 14, data);
    if (retVal != 0) {
        return retVal;
    }

    return 0;
}

INT
msdGetSMIPhyXMDIOReg(UCHAR portNum, UCHAR devAddr, UCHAR regAddr, USHORT *data)
{
    int retVal;
    USHORT tmpData;

    /* Set MMD access control address is Address | devAddr */
    tmpData = MSD_PHY_MMD_ADDR | devAddr;
    retVal = msdWritePagedPhyReg(portNum, 0, 13, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Set MMD access Data is regAddr */
    tmpData = regAddr;
    retVal = msdWritePagedPhyReg(portNum, 0, 14, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Set MMD access control address is Data no inc | devAddr */
    tmpData = MSD_PHY_MMD_DATA_NO_INC | devAddr;
    retVal = msdWritePagedPhyReg(portNum, 0, 13, tmpData);
    if (retVal != 0) {
        return retVal;
    }

    /* Read MMD access Data */
    retVal = msdReadPagedPhyReg(portNum, 0, 14, &tmpData);
    if (retVal != 0) {
        return retVal;
    }

    *data = tmpData;

    return 0;
}

INT
SDV_CpldI2cAckReset(void)
{
    CPLD_I2CREG_CTL_T stCpldI2cCtl;

    SDV_GetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);

    stCpldI2cCtl.ucCtl |= 0x40;

    SDV_SetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);

    return 0;
}


INT
SDV_CpldI2cCheckAck(void)
{
    CPLD_I2CREG_CTL_T stCpldI2cCtl;

    SDV_GetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);
    if (stCpldI2cCtl.ucCtl & 0x40) {
        OPLK_LOG_PRINT_ERR("cpld i2c ack error");
        return -1;
    }

    return 0;
}

INT
SDV_CpldI2cCheckSta(void)
{
    int j = 0;
    CPLD_I2CREG_CTL_T stCpldI2cCtl;

    while (1) {
        SDV_GetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);
        usleep(1000);
        if (stCpldI2cCtl.ucCtl & 0x80)
            break;
        if (j++ > 1000) {
            return -1;
        }
    }

    return 0;
}

INT
SDV_CpldI2cCtl(UCHAR ucCtl)
{

    ucCtl &= ~0x01;
    SDV_SetCpldValue(CPLD1_I2C_CTL, 1, &ucCtl);
    usleep(1000);

    ucCtl |= 0x01;
    SDV_SetCpldValue(CPLD1_I2C_CTL, 1, &ucCtl);
    usleep(1000);

    if (SDV_CpldI2cCheckSta()) {
        return -1;
    }

    ucCtl &= ~0x01;
    SDV_SetCpldValue(CPLD1_I2C_CTL, 1, &ucCtl);
    usleep(1000);


    return 0;
}


INT
SDV_CpldI2cRead(unsigned int slaveAddr, unsigned char *pucReadBuf, unsigned int iReadLen, unsigned char *ucCmd, unsigned char ucCmdLen)
{
    UCHAR ucDataW = 0;
    INT i = 0;
    CPLD_I2CREG_CTL_T stCpldI2cCtl;

    if (iReadLen < 1) {
        OPLK_LOG_PRINT_ERR("iReadLen < 1");
        return -1;
    }

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_INIT;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    SDV_CpldI2cAckReset();

    SDV_GetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);

    ucDataW = slaveAddr;
    SDV_SetCpldValue(CPLD1_I2C_DEVADDR, 1, &ucDataW);
    usleep(1000);

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_WRITE;
    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    for (i = 0; i < ucCmdLen; i++) {
        ucDataW = ucCmd[ucCmdLen - i - 1];
        SDV_SetCpldValue(CPLD1_I2C_TXDATA, 1, &ucDataW);
        usleep(1000);

        stCpldI2cCtl.bOpType = CPLD_OP_TYPE_TX;

        if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
            goto TIMEOUT;
        }
    }

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_READ;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    for (i = 0; i < (iReadLen - 1); i++) {
        stCpldI2cCtl.bOpType = CPLD_OP_TYPE_RX;

        if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
            goto TIMEOUT;
        }

        SDV_GetCpldValue(CPLD1_I2C_RXDATA, 1, &pucReadBuf[i]);
        usleep(1000);
    }

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_LAST_RX;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    SDV_GetCpldValue(CPLD1_I2C_RXDATA, 1, &pucReadBuf[i]);
    usleep(1000);

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_INIT;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    if (SDV_CpldI2cCheckAck()) {
        return -1;
    }

    return 0;

TIMEOUT:
    OPLK_LOG_PRINT_ERR("cpld i2c read timeout");

    return -1;
}

INT
SDV_CpldI2cWrite(unsigned int slaveAddr, unsigned char *pucWriteBuf, unsigned int iWriteLen, unsigned char *ucCmd, unsigned char ucCmdLen)
{
    UCHAR ucDataW = 0;
    int i = 0;
    CPLD_I2CREG_CTL_T stCpldI2cCtl;

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_INIT;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    SDV_CpldI2cAckReset();

    SDV_GetCpldValue(CPLD1_I2C_CTL, 1, &stCpldI2cCtl.ucCtl);

    ucDataW = slaveAddr;
    SDV_SetCpldValue(CPLD1_I2C_DEVADDR, 1, &ucDataW);
    usleep(1000);

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_WRITE;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    for (i = 0; i < ucCmdLen; i++) {
        ucDataW = ucCmd[ucCmdLen - i - 1];
        SDV_SetCpldValue(CPLD1_I2C_TXDATA, 1, &ucDataW);
        usleep(1000);

        stCpldI2cCtl.bOpType = CPLD_OP_TYPE_TX;

        if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
            goto TIMEOUT;
        }
    }

    for (i = 0; i < (iWriteLen - 1); i++) {
        SDV_SetCpldValue(CPLD1_I2C_TXDATA, 1, &pucWriteBuf[i]);

        stCpldI2cCtl.bOpType = CPLD_OP_TYPE_TX;

        if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
            goto TIMEOUT;
        }
    }

    SDV_SetCpldValue(CPLD1_I2C_TXDATA, 1, &pucWriteBuf[i]);

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_LAST_TX;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    stCpldI2cCtl.bOpType = CPLD_OP_TYPE_INIT;

    if (SDV_CpldI2cCtl(stCpldI2cCtl.ucCtl)) {
        goto TIMEOUT;
    }

    if (SDV_CpldI2cCheckAck()) {
        return -1;
    }

    return 0;

TIMEOUT:
    OPLK_LOG_PRINT_ERR("cpld i2c write timeout");

    return -1;
}

INT
SDV_FanCpldRead(ULONG ulAddress, UCHAR *pucBuf)
{
    UCHAR ucRetryCnt = 0;

    SDV_CpldI2cLock();

    for (ucRetryCnt = 0; ucRetryCnt < FANCPLD_RW_RETRY; ucRetryCnt++) {
        SDV_CpldI2CSPiChoose(CPLD_I2C_FAN);
        if (0 == SDV_CpldI2cRead(FANCPLD_SLAVE_ADDR, pucBuf, 1, (UCHAR *)&ulAddress, 1)) {
            break;
        }
    }

    if (FANCPLD_RW_RETRY == ucRetryCnt) {
        SDV_CpldI2cUnlock();
        return -1;
    }

    SDV_CpldI2cUnlock();

    return 0;
}

INT
SDV_FanCpldWrite(ULONG ulAddress, UCHAR *pucBuf)
{
    SDV_CpldI2cLock();

    SDV_CpldI2CSPiChoose(CPLD_I2C_FAN);

    if (SDV_CpldI2cWrite(FANCPLD_SLAVE_ADDR, pucBuf, 1, (UCHAR *)&ulAddress, 1)) {
        SDV_CpldI2cUnlock();
        return -1;
    }

    SDV_CpldI2cUnlock();

    return 0;
}

INT
SDV_CpldI2CSPiChoose(INT iDx)
{
    UCHAR ucBuf = 0;

    SDV_GetCpldValue(CPLD1_I2CSPI_SEL, 1, &ucBuf);

    if (CPLD_USBSPI_CS_I350 == iDx) {
        ucBuf &= ~0x03;
    } else if (CPLD_USBSPI_CS_EDFA1 == iDx) {
        ucBuf &= ~0x03;
        ucBuf |= 0x01;
    } else if (CPLD_USBSPI_CS_EDFA2 == iDx) {
        ucBuf &= ~0x03;
        ucBuf |= 0x02;
    } else if (CPLD_I2C_POWER == iDx) {
        ucBuf &= ~0x30;
    } else if (CPLD_I2C_FAN == iDx) {
        ucBuf &= ~0x30;
        ucBuf |= 0x10;
    } else {
        return -1;
    }

    SDV_SetCpldValue(CPLD1_I2CSPI_SEL, 1, &ucBuf);

    return 0;
}

INT
SDV_CpldI2cLock(void)
{    
    struct sembuf stSembuf = {0,-1,SEM_UNDO};
    
    if(m_iCpldI2cLock < 0)
    {
        printf("cpld i2c lock need init.\n");
        return -1;
    }
    
    if (semop(m_iCpldI2cLock, &stSembuf, 1) < 0)
    {
        printf("SDV_CpldI2cLock error.");
        return -1;
    }

    return 0;
}

INT
SDV_CpldI2cUnlock(void)
{
    struct sembuf stSembuf = {0,1,SEM_UNDO};
    
    if(m_iCpldI2cLock < 0)
    {
        printf("cpld i2c lock need init.\n");
        return -1;
    }
        
    if (semop(m_iCpldI2cLock, &stSembuf, 1) < 0)
    {
        printf("SDV_CpldI2cUnlock error.");
        return -1;
    }

    return 0;
}


INT
SDV_CpldI2cInit(void)
{
    union semun arg;

    if(m_iCpldI2cLock >= 0)
        return 0;   

    m_iCpldI2cLock = semget((key_t)CPLD_I2C_SEM_KEY, 1, IPC_CREAT | 0666);
    
    if(m_iCpldI2cLock < 0)
    {
        OPLK_LOG_PRINT_ERR("semget error.");
        return -1;
    }

    arg.setval = 1;
    if(-1 == semctl(m_iCpldI2cLock,0,SETVAL,arg))
    {
        OPLK_LOG_PRINT_ERR("semctl error.");
        return -1;
    }
    
    return 0;
}

INT
SDV_CpldI2cClose(void)
{
    if(m_iCpldI2cLock < 0)
    {
        printf("cpld i2c lock need init.\n");
        return -1;
    }

    if(-1 == semctl(m_iCpldI2cLock,0,IPC_RMID,0))
    {
       OPLK_LOG_PRINT_ERR("semctl error.");
       return -1;
    }

    m_iCpldI2cLock = -1;

    return 0;
}


INT
SDV_I350BufOeEnable(void)
{
    UCHAR ucBufw = 0;

    SDV_GetCpldValue(CPLD0_BRD_CTLSTA, 1, &ucBufw);

    ucBufw &= ~0x80;
    SDV_SetCpldValue(CPLD0_BRD_CTLSTA, 1, &ucBufw);

    return 0;
}

INT
SDV_I350BufOeDisable(void)
{
    UCHAR ucBufw = 0;

    SDV_GetCpldValue(CPLD0_BRD_CTLSTA, 1, &ucBufw);

    ucBufw |= 0x80;
    SDV_SetCpldValue(CPLD0_BRD_CTLSTA, 1, &ucBufw);

    return 0;
}


INT
SDV_I350EepromReady(void)
{
    UCHAR ucDataR[2] = {0};
    UCHAR ucCmdW[2] = {0};
    UCHAR ucReady = 0xFF;
    /* USHORT timeout = NVM_MAX_RETRY_SPI; */
    USHORT timeout = 1;

    while (timeout) {
        ucCmdW[0] = NVM_RDSR_OPCODE_SPI;
        SDV_UsbSpiWriteRead(ucCmdW, ucDataR, 2);

        ucReady = ucDataR[1];
        OPLK_LOG_PRINT_ERR("ucDataR[0]%x ucDataR[1]%x ", ucDataR[0], ucDataR[1]);
        if (!(ucReady & NVM_STATUS_RDY_SPI))
            break;

        usleep(5);
        timeout--;
    }

    if (!timeout) {
        OPLK_LOG_PRINT_ERR("SPI NVM Status error");
        return -1;
    }

    return 0;
}


INT
SDV_I350EepromRead(ULONG ulOffset, UCHAR *pucDataBuf, ULONG ulLength)
{
    UCHAR ucCmdAndAddr[3] = {0};
    UCHAR *pucBufTx = NULL;

    if ((ulOffset + ulLength) > NVM_TATOL_SZIE) {
        return -1;
    }

    SDV_I350BufOeEnable();
    SDV_CpldI2CSPiChoose(CPLD_USBSPI_CS_I350);

    if (SDV_I350EepromReady()) {
        goto ERR;
    }

    pucBufTx = (UCHAR *)malloc(ulLength + sizeof(ucCmdAndAddr) + 1);
    if (NULL == pucBufTx) {
        goto ERR;
    }

    ucCmdAndAddr[0] = NVM_READ_OPCODE_SPI;
    ucCmdAndAddr[1] = (ulOffset & 0xff00) >> 8;
    ucCmdAndAddr[2] = (ulOffset) & 0xff;

    memcpy(pucBufTx, ucCmdAndAddr, sizeof(ucCmdAndAddr));

    SDV_UsbSpiWriteRead(pucBufTx, pucDataBuf, ulLength + sizeof(ucCmdAndAddr));

    /*   SDV_I350BufOeDisable(); */

    if (NULL != pucBufTx) {
        free(pucBufTx);
        pucBufTx = NULL;
    }

    return 0;
ERR:

    /*   SDV_I350BufOeDisable(); */
    if (NULL != pucBufTx) {
        free(pucBufTx);
        pucBufTx = NULL;
    }

    return -1;
}

INT
SDV_Mv6390NoCpu(void)
{
    /* set port9 lan0/1/2(port9/2/3) sedres power normal */
    msdSetSMIC45PhyReg(0x04,0x12,0x2000,0x1140);
    msdSetSMIC45PhyReg(0x04,0x13,0x2000,0x1140);
    msdSetSMIC45PhyReg(0x04,0x09,0x2000,0x1140);

    /* set port1/4/8 phy power normal */
    msdWritePagedPhyReg(0x01,0x00,0x00,0x1140);
    msdWritePagedPhyReg(0x04,0x00,0x00,0x1140);
    msdWritePagedPhyReg(0x08,0x00,0x00,0x1140);

    /* set port0-10 forwarding */
    msdMultiAddrWrite(0,4,0x007f);
    msdMultiAddrWrite(1,4,0x007f);
    msdMultiAddrWrite(2,4,0x007f);
    msdMultiAddrWrite(3,4,0x007f);
    msdMultiAddrWrite(4,4,0x007f);
    msdMultiAddrWrite(5,4,0x007f);
    msdMultiAddrWrite(6,4,0x007f);
    msdMultiAddrWrite(7,4,0x007f);
    msdMultiAddrWrite(8,4,0x007f);
    msdMultiAddrWrite(9,4,0x007f);
    msdMultiAddrWrite(10,4,0x007f);
    
    return 0;
}

INT
SDV_Mv6390Init(void)
{
    USHORT usDataBuf = 0;
    /* set port9/10 C_MODE 1000base-x */
    msdMultiAddrRead(MV6390X_PORT9_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    msdMultiAddrWrite(MV6390X_PORT9_ADDR, 0, usDataBuf);

    msdMultiAddrRead(MV6390X_PORTA_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    msdMultiAddrWrite(MV6390X_PORTA_ADDR, 0, usDataBuf);

    /* set port10 lan1/2 (sfp port5/6) 1000M */
    /* port5 */
    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, 0x2000);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, 0x82A4);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, 0xa040);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, 0x86A4);
    /* == msdSetSMIC45PhyReg(0x04,0x15,0x2000,0xa040); */

    /* port6 */
    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, 0x2000);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, 0x82C4);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, 0xa040);

    msdMultiAddrWrite(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, 0x86C4);
    /* == msdSetSMIC45PhyReg(0x04,0x16,0x2000,0xa040); */

    /* port5/6 1000base-x nosie */
    msdSetSMIC45PhyReg(0x04, 0x15, 0xa000, 0x2001);
    msdSetSMIC45PhyReg(0x04, 0x16, 0xa000, 0x2001);

    #ifdef MV_NO_CPU
    SDV_Mv6390NoCpu();
    #endif
    
    return 0;
}

INT
SDV_CpldUartSwitch(INT iIndex)
{
    UCHAR ucBufw = 0;

    if ((iIndex < CPLD_UART_EDFA1) || (iIndex > CPLD_UART_POE)) {
        return -1;
    }

    ucBufw = iIndex;

    SDV_SetCpldValue(CPLD1_UART1_SEL, 1, &ucBufw);

    return 0;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_GetSfpOnline                                                                      */
/* Descrp : SFP online status.                                                                    */
/* Input  : iIndex-- sfp index                                                                    */
/* Output : *ucState: 0--on line; 1--off line.                                                    */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_GetSfpOnline(INT iIndex, UCHAR *ucState)
{
    UCHAR ucBufw = 0;

    if (NULL == ucState) {
        return OPLK_ERROR;
    }

    SDV_GetCpldValue(CPLD1_SFP_CTLSTA, 1, &ucBufw);

    if (iIndex == SFP1_INDEX) {
        if (0 != (ucBufw & 0x2)) {
            *ucState = SFP_OFF_LINE;
        } else {
            *ucState = SFP_ON_LINE;
        }
    } else if (iIndex == SFP2_INDEX) {
        if (0 != (ucBufw & 0x20)) {
            *ucState = SFP_OFF_LINE;
        } else {
            *ucState = SFP_ON_LINE;
        }
    } else {
        OPLK_LOG_PRINT_ERR("sfp index error");
        return OPLK_ERROR;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_GetSfpPortLink                                                                    */
/* Descrp : SFP LOS Status.                                                                       */
/* Input  : iIndex-- sfp index                                                                    */
/* Output : *ucState: 0--normal; 1--sfp los.                                                   */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_GetSfpPortLink(INT iIndex, UCHAR *ucState)
{
    UCHAR ucBufw = 0;

    if (NULL == ucState) {
        return OPLK_ERROR;
    }

    SDV_GetCpldValue(CPLD1_SFP_CTLSTA, 1, &ucBufw);

    if (iIndex == SFP1_INDEX) {
        if (0 != (ucBufw & 0x4)) {
            *ucState = SFP_LOS_ASSERT;
        } else {
            *ucState = SFP_LOS_NORMAL;
        }
    } else if (iIndex == SFP2_INDEX) {
        if (0 != (ucBufw & 0x40)) {
            *ucState = SFP_LOS_ASSERT;
        } else {
            *ucState = SFP_LOS_NORMAL;
        }
    } else {
        OPLK_LOG_PRINT_ERR("sfp index error");
        return OPLK_ERROR;
    }

    return OPLK_OK;
}

INT
SDV_GetSfpPhyLink(INT iIndex, UCHAR *ucState)
{
    USHORT usLinkState = 0;

    if (iIndex == SFP1_INDEX) {
        msdGetSMIC45PhyReg(0x04, 0x15, 0xA003, &usLinkState);
    } else if (iIndex == SFP2_INDEX) {
        msdGetSMIC45PhyReg(0x04, 0x16, 0xA003, &usLinkState);
    } else {
        OPLK_LOG_PRINT_ERR("sfp index error");
        return -1;
    }

    *ucState = SFP_LINKDOWN;

    if (usLinkState & 0x0400) {
        *ucState = SFP_LINKUP;
    }

    return 0;
}
/***FUNC+******************************************************************************************/
/* Name   : SDV_SetSfpState                                                                       */
/* Descrp : Set SFP enable.                                                                       */
/* Input  : iIndex-- sfp index                                                                    */
/*          ucState: 0--sfp enable; 1--sfp disable.                                               */
/* Output : none.                                                                                 */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_SetSfpState(INT iIndex, UCHAR ucState)
{
    UCHAR ucBufw = 0;

    SDV_GetCpldValue(CPLD1_SFP_CTLSTA, 1, &ucBufw);

    if (iIndex == SFP1_INDEX) {
        if (ucState == SFP_ENABLE) {
            ucBufw &= ~0x01;
        } else if (ucState == SFP_DISABLE) {
            ucBufw |= 0x01;
        } else {
            OPLK_LOG_PRINT_ERR("sfp state error");
            return -1;
        }
    } else if (iIndex == SFP2_INDEX) {
        if (ucState == SFP_ENABLE) {
            ucBufw &= ~0x10;
        } else if (ucState == SFP_DISABLE) {
            ucBufw |= 0x10;
        } else {
            OPLK_LOG_PRINT_ERR("sfp state error");
            return OPLK_ERROR;
        }
    } else {
        OPLK_LOG_PRINT_ERR("sfp index error");
        return OPLK_ERROR;
    }

    SDV_SetCpldValue(CPLD1_SFP_CTLSTA, 1, &ucBufw);

    return OPLK_OK;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_GetSfpState                                                                       */
/* Descrp : Get SFP enable.                                                                       */
/* Input  : iIndex-- sfp index                                                                    */
/* Output : *ucState: 0--sfp enable; 1--sfp disable.                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_GetSfpState(INT iIndex, UCHAR *ucState)
{
    UCHAR ucBufw = 0;

    SDV_GetCpldValue(CPLD1_SFP_CTLSTA, 1, ucState);

    if (iIndex == SFP1_INDEX) {
        if (0 != (ucBufw & 0x1)) {
            *ucState = SFP_DISABLE;
        } else {
            *ucState = SFP_ENABLE;
        }
    } else if (iIndex == SFP2_INDEX) {
        if (0 != (ucBufw & 0x10)) {
            *ucState = SFP_DISABLE;
        } else {
            *ucState = SFP_ENABLE;
        }
    } else {
        OPLK_LOG_PRINT_ERR("sfp index error");
        return OPLK_ERROR;
    }

    return OPLK_OK;
}


INT
SDV_CpldGetVer(INT iIndex, CHAR *pcVer, UCHAR ucLen)
{
    UCHAR ucVerH = 0;
    UCHAR ucVerL = 0;
    UCHAR ucVerFst = 0;
    CHAR acVersion[32] = {0};

    if ((NULL == pcVer) || (ucLen > 32)) {
        return -1;
    }

    memset(acVersion, 0, sizeof(acVersion));
    if (iIndex == CPLD0_VER) {
        SDV_GetCpldValue(CPLD0_VERH, 1, &ucVerH);
        SDV_GetCpldValue(CPLD0_VERL, 1, &ucVerL);
        SDV_GetCpldValue(CPLD0_VER_FST, 1, &ucVerFst);
    } else if (iIndex == CPLD1_VER) {
        SDV_GetCpldValue(CPLD1_VERH, 1, &ucVerH);
        SDV_GetCpldValue(CPLD1_VERL, 1, &ucVerL);
        SDV_GetCpldValue(CPLD1_VER_FST, 1, &ucVerFst);
    } else if (iIndex == CPLDFAN_VER) {
        SDV_FanCpldRead(CPLDFAN_VERH, &ucVerH);
        SDV_FanCpldRead(CPLDFAN_VERL, &ucVerL);
        SDV_FanCpldRead(CPLDFAN_VER_FST, &ucVerFst);
    }

    snprintf(acVersion, sizeof(acVersion), "%d.%02d.%04d", ucVerFst, ucVerL, ucVerH);
    memcpy(pcVer, acVersion, sizeof(acVersion));

    return 0;
}

INT
SDV_CpldModReset(UCHAR ucModIndex, UCHAR ucRstMode)
{
    UCHAR ucBufw = 0;

    SDV_GetCpldValue(CPLD1_RESET_CTL2, 1, &ucBufw);

    if (ucModIndex == WSS_MODULE) {
        if (ucRstMode == WSS_HARD_RESET) {
            ucBufw &= ~0x01;
        } else if (ucRstMode == WSS_HARD_NORMAL) {
            ucBufw |= 0x01;
        } else if (ucRstMode == WSS_SOFT_RESET) {
            ucBufw &= ~0x02;
        } else if (ucRstMode == WSS_SOFT_NORMAL) {
            ucBufw |= 0x02;
        } else {
            return -1;
        }
    } else if (ucModIndex == EXT_BROD_MODULE) {
        if (ucRstMode == EXT_BROD_RESET) {
            ucBufw &= ~0x04;
        } else if (ucRstMode == EXT_BROD_NORMAL) {
            ucBufw |= 0x04;
        } else {
            return -1;
        }
    } else if (ucModIndex == EXT_MCU_MODULE) {
        if (ucRstMode == EXT_MCU_RESET) {
            ucBufw &= ~0x08;
        } else if (ucRstMode == EXT_MCU_NORMAL) {
            ucBufw |= 0x08;
        } else {
            return -1;
        }
    } else if (ucModIndex == OCM_MODULE) {
        if (ucRstMode == OCM_HARD_RESET) {
            ucBufw &= ~0x10;
        } else if (ucRstMode == OCM_HARD_NORMAL) {
            ucBufw |= 0x10;
        } else {
            return -1;
        }
    } else {
        OPLK_LOG_PRINT_ERR("module index error");
        return -1;
    }

    SDV_SetCpldValue(CPLD1_RESET_CTL2, 1, &ucBufw);

    return 0;
}

INT
SDV_CpldApsdDbg(void)
{
    UCHAR ucData = 0xc1;    /* 0b1100 0001 */

    SDV_SetCpldValue(CPLD1_APSD_STA, 1, &ucData);

    return 0;
}
