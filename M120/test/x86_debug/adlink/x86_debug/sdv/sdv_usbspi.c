
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

#include "libft4222.h"

/*
   SPI Master can assert SS0O in single mode
   SS0O and SS1O in dual mode, and
   SS0O, SS1O, SS2O and SS3O in quad mode.
 */
#define SLAVE_SELECT(x) (1 << (x))
#define CPLD1_CS SLAVE_SELECT(0)  /* only use single mode, and select 0 */


FT_DEVICE_LIST_INFO_NODE *devInfo = NULL;
FT_HANDLE ftHandle = (FT_HANDLE)NULL;
int modeDevID = 0;

int g_UsbInit = 0;

int
_SDV_GetUSBList(void)
{
    FT_STATUS ftStatus;
    int i;
    DWORD numDevs = 0;

    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus != FT_OK) {
        printf("FT_CreateDeviceInfoList failed (error code %d)\n", (int)ftStatus);
        goto exit;
    }

    if (numDevs == 0) {
        printf("No devices connected.\n");
        goto exit;
    }

    /* Allocate storage */
    devInfo = calloc((size_t)numDevs, sizeof(FT_DEVICE_LIST_INFO_NODE));
    if (devInfo == NULL) {
        printf("Allocation failure.\n");
        goto exit;
    }

    /* Populate the list of info nodes */
    ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
    if (ftStatus != FT_OK) {
        printf("FT_GetDeviceInfoList failed (error code %d)\n", (int)ftStatus);
        goto exit;
    }

    for (i = 0; i < (int)numDevs; i++) {
        unsigned int devType = devInfo[i].Type;
        size_t descLen;

        if (devType == FT_DEVICE_4222H_0) {
            /* printf("In mode 0. i %d numDevs %d\n",i,numDevs); */
            /* In mode 0, the FT4222H presents two interfaces: A and B. */

            descLen = strlen(devInfo[i].Description);

            if ('A' == devInfo[i].Description[descLen - 1]) {
                /* Interface A may be configured as an I2C master. */
                /* printf("Interface A may be configured as an I2C master.i %d numDevs %d\n",i,numDevs); */
            } else {
                /* Interface B of mode 0 is reserved for GPIO. */
                /* printf("Skipping interface B of mode-0 FT4222H.i %d numDevs %d\n",i,numDevs); */
            }
        }
        if (devType == FT_DEVICE_4222H_1_2) {
            /* printf("In modes 1 and 2.i %d numDevs %d\n",i,numDevs); */
            /* In modes 1 and 2, the FT4222H presents four interfaces but */
            /* none is suitable for I2C. */
        }
        if (devType == FT_DEVICE_4222H_3) {
            /* printf("In mode 3.i %d numDevs %d\n",i,numDevs); */
            /* In mode 3, the FT4222H presents a single interface. */
            /* It may be configured as an I2C Master. */
        }
    }

    /* 4 spi slave,use SS3O[0-3] cs */
    i = 0;
    modeDevID = devInfo[i].LocId;

    return 0;

exit:
    if (NULL != devInfo) {
        free(devInfo);
        devInfo = NULL;
    }
    return -1;

}

int
_SDV_ConfigSPI(void)
{
    FT_STATUS ftStatus;
    FT4222_STATUS ft4222Status;

    ftStatus = FT_OpenEx((PVOID)(uintptr_t)modeDevID,
                         FT_OPEN_BY_LOCATION,
                         &ftHandle);
    if (ftStatus != FT_OK) {
        printf("FT_OpenEx failed (error %d)\n", (int)ftStatus);
        goto exit;
    }

    /* Configure the FT4222 as an SPI Master. */
    ft4222Status = FT4222_SPIMaster_Init(
        ftHandle,
        SPI_IO_SINGLE,                       /* 1 channel */
        CLK_DIV_32,                        /* 60 MHz / 32 == 1.875 MHz */
        CLK_IDLE_LOW,                      /* clock idles at logic 0 */
        CLK_LEADING,                       /* data captured on rising edge */
        CPLD1_CS);                         /* Use SS3O for slave-select */
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_SPIMaster_Init failed (error %d)\n", (int)ft4222Status);
        goto exit;
    }

    ft4222Status = FT4222_SPI_SetDrivingStrength(ftHandle,
                                                 DS_8MA,
                                                 DS_8MA,
                                                 DS_8MA);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_SPI_SetDrivingStrength failed (error %d)\n", (int)ft4222Status);
        goto exit;
    }

    return 0;

exit:
    if (ftHandle != (FT_HANDLE)NULL) {
        (void)FT4222_UnInitialize(ftHandle);
        (void)FT_Close(ftHandle);
        ftHandle = (FT_HANDLE)NULL;
    }

    return -1;
}
int
_SDV_ConfigI2C(void)
{
    FT_STATUS ftStatus;
    FT4222_STATUS ft4222Status;

    ftStatus = FT_OpenEx((PVOID)(uintptr_t)modeDevID,
                         FT_OPEN_BY_LOCATION,
                         &ftHandle);
    if (ftStatus != FT_OK) {
        printf("FT_OpenEx failed (error %d)\n", (int)ftStatus);
        goto exit;
    }

    /* Configure the FT4222 as an I2C Master */
    ft4222Status = FT4222_I2CMaster_Init(ftHandle, 400);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_I2CMaster_Init failed (error %d)!\n",
               ft4222Status);
        goto exit;
    }

    /* Reset the I2CM registers to a known state. */
    ft4222Status = FT4222_I2CMaster_Reset(ftHandle);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_I2CMaster_Reset failed (error %d)!\n",
               ft4222Status);
        goto exit;
    }

    return 0;

exit:
    if (ftHandle != (FT_HANDLE)NULL) {
        (void)FT4222_UnInitialize(ftHandle);
        (void)FT_Close(ftHandle);
        ftHandle = (FT_HANDLE)NULL;
    }

    return -1;
}
int
SDV_UsbSpiWriteRead(uint8 *ucDateTx, uint8 *ucDataRx, uint16 iLen)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesTransceived;

    ft4222Status = FT4222_SPIMaster_SingleReadWrite(
        ftHandle,
        ucDataRx,
        ucDateTx,
        iLen,
        &bytesTransceived,
        TRUE);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_SPIMaster_SingleReadWrite failed (error %d)!\n", ft4222Status);
        return -1;
    }

    if (bytesTransceived != iLen) {
        printf("FT4222_SPIMaster_SingleReadWrite "
               "transceived %u bytes instead of %u.\n",
               bytesTransceived,
               iLen);
        return -1;
    }

    return 0;
}

int
SDV_UsbI2CRead(uint8 ucSlaveAddr, uint8 *ucDataRx, uint16 iLen)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesRead = 0;

    ft4222Status = FT4222_I2CMaster_Read(ftHandle,
                                         ucSlaveAddr,
                                         ucDataRx,
                                         iLen,
                                         &bytesRead);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_I2CMaster_Read failed (error %d)\n",
               (int)ft4222Status);
        return -1;
    }

    if (bytesRead != iLen) {
        printf("FT4222_I2CMaster_Read read %u of %u bytes.\n",
               bytesRead,
               iLen);
        return -1;
    }

    return 0;
}

int
SDV_UsbI2CWrite(uint8 ucSlaveAddr, uint8 *ucDataTx, uint16 iLen)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesWritten = 0;

    ft4222Status = FT4222_I2CMaster_Write(ftHandle,
                                          ucSlaveAddr,
                                          ucDataTx,
                                          iLen,
                                          &bytesWritten);
    if (FT4222_OK != ft4222Status) {
        printf("FT4222_I2CMaster_Write failed (error %d)\n",
               (int)ft4222Status);
        return -1;
    }

    if (bytesWritten != iLen) {
        printf("FT4222_I2CMaster_Write wrote %u of %u bytes.\n",
               bytesWritten,
               iLen);
        return -1;
    }

    return 0;
}

int
SDV_UsbSpiInit(void)
{
    if (1 == g_UsbInit)
        return 0;

    if (_SDV_GetUSBList()) {
        printf("get usb list err!\n");
        return -1;
    }
    if (_SDV_ConfigSPI()) {
        printf("config spi err!\n");
        return -1;
    }

    g_UsbInit = 1;

    return 0;
}

int
SDV_UsbI2cInit(void)
{
    if (1 == g_UsbInit)
        return 0;

    if (_SDV_GetUSBList()) {
        printf("get usb list err!\n");
        return -1;
    }
    if (_SDV_ConfigI2C()) {
        printf("config i2c err!\n");
        return -1;
    }

    g_UsbInit = 1;

    return 0;
}


int
SDV_UsbClose(void)
{
    if (0 == g_UsbInit)
        return 0;

    if (NULL != devInfo)
        free(devInfo);

    if (ftHandle != (FT_HANDLE)NULL) {
        (void)FT4222_UnInitialize(ftHandle);
        (void)FT_Close(ftHandle);
    }

    g_UsbInit = 0;

    return 0;
}

