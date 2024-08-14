#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spi_eeprom.h"
#include "sdv_usbspi.h"

#define SPI_EEPROM_WRITE_CMD         (0x02)
#define SPI_EEPROM_READ_CMD          (0x03)
#define SPI_EEPROM_STA_CMD           (0x05)
#define SPI_EEPROM_WRITENABLE_CMD    (0x06)

#define SPI_EEPROM_CHIP_LEN         (0x8000)
#define SPI_EEPROM_PAGE_LEN         (64)


INT
EESpiWriteEnable(void)
{
    UCHAR ucDateTx[4] = {0};
    UCHAR ucDataRx[4] = {0};

    ucDateTx[0] = SPI_EEPROM_WRITENABLE_CMD;
    if (SDV_UsbSpiWriteRead(ucDateTx, ucDataRx, 1)) {
        return -1;
    }

    return 0;
}

INT
EESpiWriteOnePage(UINT unPageAddr, UCHAR *pucDataBuf, UINT unDataLen)
{
    UCHAR ucDateTx[128] = {0};
    UCHAR ucDataRx[128] = {0};

    UCHAR ucAddrH = 0;
    UCHAR ucAddrL = 0;

    ucAddrH = (unPageAddr >> 8) & 0xFF;
    ucAddrL = unPageAddr & 0xFF;

    ucDateTx[0] = SPI_EEPROM_WRITE_CMD;
    ucDateTx[1] = ucAddrH;
    ucDateTx[2] = ucAddrL;
    memcpy(ucDateTx + 3, pucDataBuf, unDataLen);

    if (SDV_UsbSpiWriteRead(ucDateTx, ucDataRx, unDataLen + 3)) {
        return -1;
    }

    return 0;

}


INT
EESpiWriteData(UINT unPageIndex, UCHAR *pucDataBuf, UINT unDataLen)
{
    UINT unDataPageLen = 0;
    UINT unDataIndex = 0;

    if ((NULL == pucDataBuf) ||
        (unDataLen > SPI_EEPROM_CHIP_LEN)) {
        return -1;
    }

    if (0 != (unPageIndex % SPI_EEPROM_PAGE_LEN)) {
        printf("need be a page addr.\n");
        return -1;
    }

    while (0 != unDataLen) {
        if (unDataLen < SPI_EEPROM_PAGE_LEN) {
            unDataPageLen = unDataLen;
        } else {
            unDataPageLen = SPI_EEPROM_PAGE_LEN;
        }

        if (EESpiWriteEnable()) {
            return -1;
        }

        if (EESpiWriteOnePage(unPageIndex, pucDataBuf + unDataIndex, unDataPageLen)) {
            printf("eeprom spi write one page error. \n");
            return -1;
        }
        /* wait for page write end. */
        usleep(10000);

        unPageIndex += unDataPageLen;
        unDataIndex += unDataPageLen;
        unDataLen -= unDataPageLen;
    }


    return 0;
}

INT
EESpiReadOnePage(UINT unPageAddr, UCHAR *pucDataBuf, UINT unDataLen)
{
    UCHAR ucDateTx[128] = {0};
    UCHAR ucDataRx[128] = {0};

    UCHAR ucAddrH = 0;
    UCHAR ucAddrL = 0;

    ucAddrH = (unPageAddr >> 8) & 0xFF;
    ucAddrL = unPageAddr & 0xFF;

    ucDateTx[0] = SPI_EEPROM_READ_CMD;
    ucDateTx[1] = ucAddrH;
    ucDateTx[2] = ucAddrL;

    if (SDV_UsbSpiWriteRead(ucDateTx, ucDataRx, unDataLen + 3)) {
        return -1;
    }

    memcpy(pucDataBuf, ucDataRx + 3, unDataLen);

    return 0;
}

INT
EESpiReadData(UINT unPageIndex, UCHAR *pucDataBuf, UINT unDataLen)
{
    UINT unDataPageLen = 0;
    UINT unDataIndex = 0;

    if ((NULL == pucDataBuf) ||
        (unDataLen > SPI_EEPROM_CHIP_LEN)) {
        return -1;
    }

    while (0 != unDataLen) {
        if (unDataLen < SPI_EEPROM_PAGE_LEN) {
            unDataPageLen = unDataLen;
        } else {
            unDataPageLen = SPI_EEPROM_PAGE_LEN;
        }

        if (EESpiReadOnePage(unPageIndex, pucDataBuf + unDataIndex, unDataPageLen)) {
            printf("eeprom spi write one page error. \n");
            return -1;
        }

        unPageIndex += unDataPageLen;
        unDataIndex += unDataPageLen;
        unDataLen -= unDataPageLen;
    }

    return 0;
}
