
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "CmdTypes.h"
#include "CmdCommon.h"
#include "CmdTask.h"
#include "CmdTree.h"
#include "CmdLex.h"

#include "mscc_poe_host_comm_api.h"
#include "mscc_arch_functions.h"
#include "mscc_poe_util.h"

typedef enum
{
    CPLD_UART_EDFA1 = 0,
    CPLD_UART_EDFA2,
    CPLD_UART_WSS,
    CPLD_UART_OCM,
    CPLD_UART_EXTEND,
    CPLD_UART_POE,

} EN_CPLD_UART_CHOOSE;


/* #include "sdv_cpldspi.h" */
typedef enum
{
    FPGA_SPI_NONE = 0,
    FPGA_SPI_BCM5389,
    FPGA_SPI_EDFA1_FLASH = 1,
    FPGA_SPI_EDFA2_FLASH = 2,
    FPGA_SPI_FPGA_FLASH = 4,

    FPGA_SPI_SLAVE_MAX

} EN_FPGA_SPI_SLAVE_SEL;

#define CHIP_PAGE_BYTES                 (0x100)
#define CHIP_PAGE_NUMS                  (0x10000)
#define CHIP_TOTAL_BYTES                (CHIP_PAGE_BYTES * CHIP_PAGE_NUMS)


ULONG
CMD_ReadCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulStartAddr = 0;
    ULONG ulReadLen = 0;
    UCHAR *pucBuf = NULL;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);
    INT i = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulStartAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulReadLen);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    pucBuf = (UCHAR *)malloc(ulReadLen + 1);
    if (NULL == pucBuf) {
        goto ERR_LABEL;
    }

    cReturn = SDV_GetCpldValue(ulStartAddr, ulReadLen, pucBuf);
    if (CMD_OK != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\nCPLD VAL:\r\n");

    for (i = 0; i < ulReadLen; i++) {
        printf("%02X ", pucBuf[i]);

        if ((0 == (i + 1) % 8) && (0 != (i + 1) % 16)) {
            printf("- ");
        }

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    CMD_PrintfStr(ulSes, "\r\nread cpld ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread cpld error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddress = 0;
    UCHAR ucVal = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);
    CHAR acBufWr[512] = {0};
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nIndex = 0;
    UINT i;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulAddress);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    memset(acBufWr, 0x00, sizeof(acBufWr));
    sprintf(acBufWr, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufWr, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nIndex = 0;
    acBufWr[nIndex++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufWr[nIndex++] = nData;
    }

    printf("\r\nCpld Write:\r\n");

    for (i = 0; i < nIndex; i++) {
        printf("%02X ", (UCHAR)acBufWr[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    cReturn = SDV_SetCpldValue(ulAddress, nIndex, acBufWr);
    if (CMD_OK != cReturn) {
        goto ERR_LABEL;
    }
    CMD_PrintfStr(ulSes, "\r\nwrite cpld ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite cpld error!");
    return CMD_ERROR;
}

ULONG
CMD_ReadFanCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulStartAddr = 0;
    ULONG ulReadLen = 0;
    UCHAR ucBuf = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);
    INT i = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulStartAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    if (0xFFFF <= ulStartAddr) {
        printf("\r\nAddress is overflow\r\n");
        goto ERR_LABEL;
    }

    cReturn = SDV_FanCpldRead(ulStartAddr, &ucBuf);
    if (CMD_OK != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\nCPLD VAL:\r\n");

    printf("%02X ", ucBuf);

    printf("\r\n");

    CMD_PrintfStr(ulSes, "\r\nread cpld ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread cpld error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteFanCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddress = 0;
    UCHAR ucVal = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulAddress);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    if (0xFFFF <= ulAddress) {
        printf("\r\nAddress is overflow\r\n");
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%hhx", &ucVal);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    cReturn = SDV_FanCpldWrite(ulAddress, &ucVal);
    if (CMD_OK != cReturn) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nwrite cpld ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite cpld error!");
    return CMD_ERROR;
}


ULONG
CMD_ReadMdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulReadData = 0;
    INT lRetVal = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    SDV_MdioReadReg(ulPhyAddr, ulRegAddr, &ulReadData);

    printf("\r\nphy(%x)reg(%x) value:%x\r\n", ulPhyAddr, ulRegAddr, ulReadData);

    CMD_PrintfStr(ulSes, "\r\nread mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteMdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulWriteData);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    SDV_MdioWriteReg(ulPhyAddr, ulRegAddr, ulWriteData);

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_ReadMvSwitch(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulReadData = 0;
    INT lRetVal = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdMultiAddrRead(ulPhyAddr, ulRegAddr, &ulReadData);

    printf("\r\nphy(%x)reg(%x) value:%x\r\n", ulPhyAddr, ulRegAddr, ulReadData);

    CMD_PrintfStr(ulSes, "\r\nread mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteMvSwitch(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulWriteData);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdMultiAddrWrite(ulPhyAddr, ulRegAddr, ulWriteData);

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_MvInit(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    SDV_Mv6390Init();

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_ReadMvPhy(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulDevAddr = 0;
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulReadData = 0;
    INT lRetVal = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulDevAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdGetSMIC45PhyReg(ulDevAddr, ulPhyAddr, ulRegAddr, &ulReadData);

    printf("\r\ndev(%x)phy(%x)reg(%x) value:%x\r\n", ulDevAddr, ulPhyAddr, ulRegAddr, ulReadData);

    CMD_PrintfStr(ulSes, "\r\nread mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteMvPhy(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulDevAddr = 0;
    ULONG ulPhyAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulDevAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulPhyAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 3], "%lx", &ulWriteData);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdSetSMIC45PhyReg(ulDevAddr, ulPhyAddr, ulRegAddr, ulWriteData);

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_ReadMvXmdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulPortNum = 0;
    ULONG ulDevAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulReadData = 0;
    INT lRetVal = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPortNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulDevAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdGetSMIPhyXMDIOReg(ulPortNum, ulDevAddr, ulRegAddr, &ulReadData);

    printf("\r\nPort(%x)dev(%x)reg(%x) value:%x\r\n", ulPortNum, ulDevAddr, ulRegAddr, ulReadData);

    CMD_PrintfStr(ulSes, "\r\nread mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteMvXmdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulPortNum = 0;
    ULONG ulDevAddr = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPortNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulDevAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 3], "%lx", &ulWriteData);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdSetSMIPhyXMDIOReg(ulPortNum, ulDevAddr, ulRegAddr, ulWriteData);

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_ReadMvPagemdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulPortNum = 0;
    ULONG ulPageNum = 0;
    ULONG ulRegAddr = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulReadData = 0;
    INT lRetVal = 0;

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPortNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulPageNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdReadPagedPhyReg(ulPortNum, ulPageNum, ulRegAddr, &ulReadData);

    printf("\r\nPort(%x)page(%x)reg(%x) value:%x\r\n", ulPortNum, ulPageNum, ulRegAddr, ulReadData);

    CMD_PrintfStr(ulSes, "\r\nread mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nread mdio error!");
    return CMD_ERROR;
}

ULONG
CMD_WriteMvPagemdio(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulPortNum = 0;
    ULONG ulPageNum = 0;
    ULONG ulRegAddr = 0;
    ULONG ulWriteData = 0;
    INT lRetVal = 0;
    ULONG ulSes = atoi(argv[0]);

    lRetVal = sscanf(argv[PRES_ARG], "%lx", &ulPortNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 1], "%lx", &ulPageNum);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 2], "%lx", &ulRegAddr);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    lRetVal = sscanf(argv[PRES_ARG + 3], "%lx", &ulWriteData);
    if (1 != lRetVal) {
        goto ERR_LABEL;
    }

    msdWritePagedPhyReg(ulPortNum, ulPageNum, ulRegAddr, ulWriteData);

    CMD_PrintfStr(ulSes, "\r\nwrite mdio ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nwrite mdio error!");
    return CMD_ERROR;
}


ULONG
CMD_UsbSpiCom(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR acBufWr[512] = {0};
    CHAR acBufR[512] = {0};
    INT ICs = 0;
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nIndex = 0;
    ULONG ulSes = atoi(argv[0]);
    UINT i;

    printf("\r\n");

    memset(acBufWr, 0x00, sizeof(acBufWr));
    sprintf(acBufWr, "%s", argv[PRES_ARG]);

    pcStrTok = strtok(acBufWr, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nIndex = 0;
    acBufWr[nIndex++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufWr[nIndex++] = nData;
    }

    printf("\r\nspi write:\r\n");

    for (i = 0; i < nIndex; i++) {
        printf("%02X ", (UCHAR)acBufWr[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    if (SDV_UsbSpiInit()) {
        goto ERR_LABEL;
    }

    if (ERROR == SDV_UsbSpiWriteRead((UCHAR *)&acBufWr, (UCHAR *)&acBufR, nIndex)) {
        goto ERR_LABEL;
    }

    printf("\r\nspi read:\r\n");

    for (i = 0; i < nIndex; i++) {
        printf("%02X ", (UCHAR)acBufR[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    SDV_UsbClose();

    CMD_PrintfStr(ulSes, "\r\nspicom execute ok!");
    return CMD_OK;

ERR_LABEL:
    SDV_UsbClose();

    CMD_PrintfStr(ulSes, "\r\nspicom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_JtagWriteCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR *pcFileName = NULL;
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet;
    INT iBoardType = 0;
    UCHAR *pucDataBuf = NULL;
    ULONG ulLength;
    struct stat stFileInfo;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];

    iRet = sscanf(argv[PRES_ARG + 1], "%x", &iBoardType);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }
    ulLength = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulLength + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucDataBuf, ulLength);
    if (ulLength != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_JtagCpld(iBoardType, pucDataBuf, ulLength);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nwirte cpld image ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nwirte cpld image error!");
    return CMD_ERROR;
}

ULONG
CMD_JtagResetCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    INT iRet;
    INT iBoardType = 0;
    ULONG ulSes = atoi(argv[0]);


    iRet = sscanf(argv[PRES_ARG], "%x", &iBoardType);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nreseting...");

    cReturn = SDV_JtagCpldReset(iBoardType);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nreset cpld ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nrest cpld error!");
    return CMD_ERROR;
}


ULONG
CMD_ReadSpiE1Flash(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    ULONG ulReadLen = 0;
    UINT nLenDisp = 0;
    INT iAddr;
    UCHAR *pucDataBuf = NULL;
    ULONG ulSes = atoi(argv[0]);
    INT i;

    iRet = sscanf(argv[PRES_ARG], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulReadLen);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulReadLen + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    memset(pucDataBuf, 0, ulReadLen);
    cReturn = SDV_SpiReadDataFromFlash(FPGA_SPI_EDFA1_FLASH, iAddr, pucDataBuf, ulReadLen);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    nLenDisp = ulReadLen < 256 ? ulReadLen : 256;

    printf(" display lenth is: %d\n\n", nLenDisp);

    for (i = 0; i < nLenDisp; i++) {
        printf("%02X ", pucDataBuf[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    /*creat  new  file                                                          */
    iSourceFd = open("/tmp/EDFAflashfile.bin", O_RDWR | O_CREAT, 0);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    /*restore data to file                                                      */
    iRet = write(iSourceFd, pucDataBuf, ulReadLen);
    if (ulReadLen != iRet) {
        goto ERR_LABEL;

    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nRead EDFA1 flash ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nRead EDFA1 flash error!");
    return CMD_ERROR;

}

ULONG
CMD_WriteSpiE1Flash(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR *pcFileName = NULL;
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet;
    INT iAddr = 0;
    UCHAR *pucDataBuf = NULL;
    ULONG ulLength;
    struct stat stFileInfo;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];

    iRet = sscanf(argv[PRES_ARG + 1], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }
    ulLength = stFileInfo.st_size;

    if ((iAddr + ulLength) > CHIP_TOTAL_BYTES) {
        goto ERR_LABEL;
    }

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulLength + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucDataBuf, ulLength);
    if (ulLength != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_WriteEdfaFpgaImg(FPGA_SPI_EDFA1_FLASH, iAddr, pucDataBuf, ulLength);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite spi EDFA1 flash ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite spi EDFA1 flash error!");
    return CMD_ERROR;
}

ULONG
CMD_SpiFlashEraseE1(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulSes = atoi(argv[0]);

    CMD_PrintfStr(ulSes, "\r\nErase...");

    cReturn = SDV_EraseSpiChip(FPGA_SPI_EDFA1_FLASH);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nErase EDFA1 flash execute ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nErase EDFA1 flash execute error!");
    return CMD_ERROR;

}


ULONG
CMD_ReadSpiE2Flash(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    ULONG ulReadLen = 0;
    UINT nLenDisp = 0;
    INT iAddr;
    UCHAR *pucDataBuf = NULL;
    ULONG ulSes = atoi(argv[0]);
    INT i;

    iRet = sscanf(argv[PRES_ARG], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulReadLen);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulReadLen + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    memset(pucDataBuf, 0, ulReadLen);
    cReturn = SDV_SpiReadDataFromFlash(FPGA_SPI_EDFA2_FLASH, iAddr, pucDataBuf, ulReadLen);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    nLenDisp = ulReadLen < 256 ? ulReadLen : 256;

    printf(" display lenth is: %d\n\n", nLenDisp);

    for (i = 0; i < nLenDisp; i++) {
        printf("%02X ", pucDataBuf[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    /*creat  new  file                                                          */
    iSourceFd = open("/tmp/EDFAflashfile.bin", O_RDWR | O_CREAT, 0);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    /*restore data to file                                                      */
    iRet = write(iSourceFd, pucDataBuf, ulReadLen);
    if (ulReadLen != iRet) {
        goto ERR_LABEL;

    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nRead EDFA2 flash ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nRead EDFA2 flash error!");
    return CMD_ERROR;

}

ULONG
CMD_WriteSpiE2Flash(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR *pcFileName = NULL;
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet;
    INT iAddr = 0;
    UCHAR *pucDataBuf = NULL;
    ULONG ulLength;
    struct stat stFileInfo;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];

    iRet = sscanf(argv[PRES_ARG + 1], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }
    ulLength = stFileInfo.st_size;

    if ((iAddr + ulLength) > CHIP_TOTAL_BYTES) {
        goto ERR_LABEL;
    }

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulLength + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucDataBuf, ulLength);
    if (ulLength != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_WriteEdfaFpgaImg(FPGA_SPI_EDFA2_FLASH, iAddr, pucDataBuf, ulLength);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite spi EDFA2 flash ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite spi EDFA2 flash error!");
    return CMD_ERROR;
}

ULONG
CMD_SpiFlashEraseE2(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulSes = atoi(argv[0]);

    CMD_PrintfStr(ulSes, "\r\nErase...");

    cReturn = SDV_EraseSpiChip(FPGA_SPI_EDFA2_FLASH);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nErase EDFA2 flash execute ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nErase EDFA2 flash execute error!");
    return CMD_ERROR;

}

ULONG
CMD_I350EEPROMWrite(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR *pcFileName = NULL;
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet;
    INT iAddr = 0;
    UCHAR *pucDataBuf = NULL;
    ULONG ulLength;
    struct stat stFileInfo;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];

    iRet = sscanf(argv[PRES_ARG + 1], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }
    ulLength = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulLength + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucDataBuf, ulLength);
    if (ulLength != iRet) {
        goto ERR_LABEL;
    }

    /* cReturn = SDV_I350EepromWrite(iAddr,pucDataBuf, ulLength); */
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i350 eeprom ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i350 eeprom error!");
    return CMD_ERROR;
}

ULONG
CMD_I350EEPROMRead(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    ULONG ulReadLen = 0;
    UINT nLenDisp = 0;
    INT iAddr;
    UCHAR *pucDataBuf = NULL;
    ULONG ulSes = atoi(argv[0]);
    INT i;

    iRet = sscanf(argv[PRES_ARG], "%x", &iAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulReadLen);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    pucDataBuf = (UCHAR *)malloc(ulReadLen + 1);
    if (NULL == pucDataBuf) {
        goto ERR_LABEL;
    }

    if (SDV_UsbSpiInit()) {
        goto ERR_LABEL;
    }

    memset(pucDataBuf, 0, ulReadLen);
    cReturn = SDV_I350EepromRead(iAddr, pucDataBuf, ulReadLen);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    nLenDisp = ulReadLen < 256 ? ulReadLen : 256;

    printf(" display lenth is: %d\n\n", nLenDisp);

    for (i = 0; i < nLenDisp; i++) {
        printf("%02X ", pucDataBuf[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    /*creat  new  file                                                          */
    iSourceFd = open("/tmp/i350.eep", O_RDWR | O_CREAT, 0);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    /*restore data to file                                                      */
    iRet = write(iSourceFd, pucDataBuf, ulReadLen);
    if (ulReadLen != iRet) {
        goto ERR_LABEL;

    }

    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    SDV_UsbClose();
    CMD_PrintfStr(ulSes, "\r\nRead i350 eeprom ok!");
    return CMD_OK;

ERR_LABEL:
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    if (NULL != pucDataBuf) {
        free(pucDataBuf);
    }
    SDV_UsbClose();
    CMD_PrintfStr(ulSes, "\r\nRead i350 eeprom error!");
    return CMD_ERROR;

}

ULONG
CMD_IICReadTemp(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    UCHAR ucTarget = 0;
    INT iTemp;
    ULONG ulSes = atoi(argv[0]);

    SDV_IicInit();

    cReturn = SDV_ReadBoardTemp(&iTemp);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    SDV_IicClose();

    CMD_PrintfStr(ulSes, "\r\nTemp(unit:0.1 C): %d\n", iTemp);

    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();

    CMD_PrintfStr(ulSes, "\r\nRead i2c temp execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICReadEEPROM(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    INT iReadMode = 0;
    UCHAR *pucBuffer = NULL;
    ULONG ulSes = atoi(argv[0]);

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iReadMode = atoi(argv[PRES_ARG + 2]);

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    if (SDV_IicInit()) {
        goto ERR_LABEL;
    }

    cReturn = SDV_EEpromCfgRead(ulAddr, pucBuffer, ulLenRd);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    if (0 == iReadMode) {
        /*create  new  file                                                     */
        iSourceFd = open("/tmp/Eeprom1file.bin", O_RDWR | O_CREAT, 0);
        if (iSourceFd < 0) {
            goto ERR_LABEL;
        }

        /*restore data to file                                                  */
        iRet = write(iSourceFd, pucBuffer, ulLenRd);
        if (ulLenRd != iRet) {
            goto ERR_LABEL;

        }
    }

    if (1 == iReadMode) {
        for (i = 0; i < ulLenRd; i++) {
            printf("%02X ", pucBuffer[i]);

            if (0 == (i + 1) % 16) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
    printf("\r\n");

    SDV_IicClose();

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICWriteEEPROM(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    INT iSourceFd = 0;
    struct stat stFileInfo;
    CHAR *pcFileName = NULL;
    UCHAR *pucBuf = NULL;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    /* file information.                                                        */
    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }

    ulFileLen = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucBuf = (UCHAR *)malloc(ulFileLen + 1);
    if (NULL == pcBuf) {
        goto ERR_LABEL;
    }

    memset(pucBuf, 0, ulFileLen + 1);

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    if (SDV_IicInit()) {
        goto ERR_LABEL;
    }

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucBuf, ulFileLen);
    if (ulFileLen != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_EEpromCfgWrite(ulAddr, pucBuf, ulFileLen);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    SDV_IicClose();

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICReadEEPROMID(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    INT iReadMode = 0;
    UCHAR *pucBuffer = NULL;
    ULONG ulSes = atoi(argv[0]);

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iReadMode = atoi(argv[PRES_ARG + 2]);

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    if (SDV_IicInit()) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    cReturn = SDV_EEpromIDRead(ulAddr, pucBuffer, ulLenRd);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    if (0 == iReadMode) {
        /*create  new  file                                                     */
        iSourceFd = open("/tmp/Eeprom2file.bin", O_RDWR | O_CREAT, 0);
        if (iSourceFd < 0) {
            goto ERR_LABEL;
        }

        /*restore data to file                                                  */
        iRet = write(iSourceFd, pucBuffer, ulLenRd);
        if (ulLenRd != iRet) {
            goto ERR_LABEL;

        }
    }

    if (1 == iReadMode) {
        for (i = 0; i < ulLenRd; i++) {
            printf("%02X ", pucBuffer[i]);

            if (0 == (i + 1) % 16) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
    printf("\r\n");
    SDV_IicClose();

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICWriteEEPROMID(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    INT iSourceFd = 0;
    struct stat stFileInfo;
    CHAR *pcFileName = NULL;
    UCHAR *pucBuf = NULL;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    /* file information.                                                        */
    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }

    ulFileLen = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucBuf = (UCHAR *)malloc(ulFileLen + 1);
    if (NULL == pcBuf) {
        goto ERR_LABEL;
    }

    if (SDV_IicInit()) {
        goto ERR_LABEL;
    }

    memset(pucBuf, 0, ulFileLen + 1);

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucBuf, ulFileLen);
    if (ulFileLen != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_EEpromIDWrite(ulAddr, pucBuf, ulFileLen);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }

    SDV_IicClose();

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute error!");
    return CMD_ERROR;
}


ULONG
CMD_IICReadEEPROMIDByCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    INT iReadMode = 0;
    UCHAR *pucBuffer = NULL;
    ULONG ulSes = atoi(argv[0]);

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iReadMode = atoi(argv[PRES_ARG + 2]);

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    cReturn = SDV_EEpromIDReadByCpldI2c(ulAddr, pucBuffer, ulLenRd);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    if (0 == iReadMode) {
        /*create  new  file                                                     */
        iSourceFd = open("/tmp/Eeprom2file.bin", O_RDWR | O_CREAT, 0);
        if (iSourceFd < 0) {
            goto ERR_LABEL;
        }

        /*restore data to file                                                  */
        iRet = write(iSourceFd, pucBuffer, ulLenRd);
        if (ulLenRd != iRet) {
            goto ERR_LABEL;

        }
    }

    if (1 == iReadMode) {
        for (i = 0; i < ulLenRd; i++) {
            printf("%02X ", pucBuffer[i]);

            if (0 == (i + 1) % 16) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
    printf("\r\n");

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICWriteEEPROMIDByCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    INT iSourceFd = 0;
    struct stat stFileInfo;
    CHAR *pcFileName = NULL;
    UCHAR *pucBuf = NULL;
    ULONG ulSes = atoi(argv[0]);

    pcFileName = argv[PRES_ARG];
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    /* file information.                                                        */
    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }

    ulFileLen = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucBuf = (UCHAR *)malloc(ulFileLen + 1);
    if (NULL == pcBuf) {
        goto ERR_LABEL;
    }


    memset(pucBuf, 0, ulFileLen + 1);

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucBuf, ulFileLen);
    if (ulFileLen != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_EEpromIDWriteByCpldI2c(ulAddr, pucBuf, ulFileLen);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICReadEEPROMFanByCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    INT iIdx = 0;
    UCHAR *pucBuffer = NULL;
    ULONG ulSes = atoi(argv[0]);

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iIdx = atoi(argv[PRES_ARG + 2]);

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    cReturn = SDV_EEpromFanRead(ulAddr, pucBuffer, ulLenRd, iIdx);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    for (i = 0; i < ulLenRd; i++) {
        printf("%02X ", pucBuffer[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c eeprom execute error!");
    return CMD_ERROR;
}

ULONG
CMD_IICWriteEEPROMFanByCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    INT iSourceFd = 0;
    struct stat stFileInfo;
    CHAR *pcFileName = NULL;
    UCHAR *pucBuf = NULL;
    ULONG ulSes = atoi(argv[0]);
    INT iIdx = 0;

    pcFileName = argv[PRES_ARG];
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    iIdx = atoi(argv[PRES_ARG + 2]);

    /* file information.                                                        */
    if (stat(pcFileName, &stFileInfo) < 0) {
        goto ERR_LABEL;
    }

    ulFileLen = stFileInfo.st_size;

    /* File                                                                     */
    iSourceFd = open(pcFileName, O_RDWR);
    if (iSourceFd < 0) {
        goto ERR_LABEL;
    }

    pucBuf = (UCHAR *)malloc(ulFileLen + 1);
    if (NULL == pcBuf) {
        goto ERR_LABEL;
    }


    memset(pucBuf, 0, ulFileLen + 1);

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    /* read data from file.                                                     */
    iRet = read(iSourceFd, pucBuf, ulFileLen);
    if (ulFileLen != iRet) {
        goto ERR_LABEL;
    }

    cReturn = SDV_EEpromFanWrite(ulAddr, pucBuf, ulFileLen, iIdx);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuf) {
        free(pucBuf);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c eeprom execute error!");
    return CMD_ERROR;
}


ULONG
CMD_IICReadPowerByCpld(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iSourceFd = 0;
    INT iRet = 0;
    INT iIdx = 0;
    UCHAR *pucBuffer = NULL;
    ULONG ulSes = atoi(argv[0]);

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 1], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iIdx = atoi(argv[PRES_ARG + 2]);

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    cReturn = SDV_PowerInfoRead(ulAddr, pucBuffer, ulLenRd, iIdx);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    for (i = 0; i < ulLenRd; i++) {
        printf("%02X ", pucBuffer[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead power info execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    if (0 != iSourceFd) {
        close(iSourceFd);
    }
    CMD_PrintfStr(ulSes, "\r\nRead power info execute error!");
    return CMD_ERROR;
}


ULONG
CMD_I2CCpldRead(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iRet = 0;
    UCHAR *pucBuffer = NULL;
    CHAR acBufWr[512] = {0};
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nIndex = 0;
    ULONG ulSes = atoi(argv[0]);


    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 2], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);

    memset(acBufWr, 0x00, sizeof(acBufWr));
    sprintf(acBufWr, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufWr, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nIndex = 0;
    acBufWr[nIndex++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufWr[nIndex++] = nData;
    }

    ulAddr = ulAddr << 1;

    cReturn = SDV_CpldI2cRead(ulAddr, pucBuffer, ulLenRd, acBufWr, nIndex);
    if (0 != cReturn) {
        goto ERR_LABEL;
    }

    printf("\r\n");

    for (i = 0; i < ulLenRd; i++) {
        printf("%02X ", pucBuffer[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");


    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c cpld execute ok!");
    return CMD_OK;

ERR_LABEL:

    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    CMD_PrintfStr(ulSes, "\r\nRead i2c cpld execute error!");
    return CMD_ERROR;
}


ULONG
CMD_I2CCpldWrite(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    ULONG ulSes = atoi(argv[0]);
    CHAR acBufWr[512] = {0};
    CHAR acBufCmd[512] = {0};
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nCmdLen = 0;
    UINT nDataLen = 0;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    memset(acBufCmd, 0x00, sizeof(acBufCmd));
    sprintf(acBufCmd, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufCmd, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nCmdLen = 0;
    acBufCmd[nCmdLen++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufCmd[nCmdLen++] = nData;
    }

    memset(acBufWr, 0x00, sizeof(acBufWr));
    sprintf(acBufWr, "%s", argv[PRES_ARG + 2]);

    pcStrTok = strtok(acBufWr, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nDataLen = 0;
    acBufWr[nDataLen++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufWr[nDataLen++] = nData;
    }


    CMD_PrintfStr(ulSes, "\r\nWriting...");

    ulAddr = ulAddr << 1;

    cReturn = SDV_CpldI2cWrite(ulAddr, acBufWr, nDataLen, acBufCmd, nCmdLen);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute error!");
    return CMD_ERROR;
}

ULONG
CMD_I2CCpldDebug1Read(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulLenRd = 0;
    INT i = 0;
    INT iRet = 0;
    UCHAR *pucBuffer = NULL;
    CHAR acBufWr[512] = {0};;
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nIndex = 0;
    ULONG ulSes = atoi(argv[0]);
    ULONG ulLen = 0;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    iRet = sscanf(argv[PRES_ARG + 2], "%lx", &ulLenRd);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    pucBuffer = (UCHAR *)malloc(ulLenRd + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, 0, ulLenRd + 1);


    memset(acBufWr, 0x00, sizeof(acBufWr));
    sprintf(acBufWr, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufWr, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nIndex = 0;
    acBufWr[nIndex++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufWr[nIndex++] = nData;
    }

    ulAddr = ulAddr << 1;
    ulLen = ulLenRd;
    i = 0;

    while (ulLenRd--) {
        if (SDV_CpldI2cRead(ulAddr, (UCHAR *)&pucBuffer[i], 1, acBufWr, nIndex)) {
            break;
        }

        i++;
    }

    printf("\r\n");

    for (i = 0; i < ulLen; i++) {
        printf("%02X ", pucBuffer[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");


    if (NULL != pucBuffer) {
        free(pucBuffer);
    }

    printf("\r\n");

    CMD_PrintfStr(ulSes, "\r\nRead i2c cpld execute ok!");
    return CMD_OK;

ERR_LABEL:
    if (NULL != pucBuffer) {
        free(pucBuffer);
    }

    CMD_PrintfStr(ulSes, "\r\nRead i2c cpld execute error!");
    return CMD_ERROR;
}

ULONG
CMD_I2CCpldDebug1Write(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    ULONG ulSes = atoi(argv[0]);
    CHAR acBufCmd[512] = {0};
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nCmdLen = 0;
    UINT nDataLen = 0;
    ULONG uLData = 0;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }
    sscanf(argv[PRES_ARG + 2], "%lx", &uLData);
    sscanf(argv[PRES_ARG + 3], "%lx", &nDataLen);
    ulAddr = 2;

    memset(acBufCmd, 0x00, sizeof(acBufCmd));
    sprintf(acBufCmd, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufCmd, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nCmdLen = 0;
    acBufCmd[nCmdLen++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufCmd[nCmdLen++] = nData;
    }

    CMD_PrintfStr(ulSes, "\r\nWriting...");

    ulAddr = ulAddr << 1;

    while (nDataLen--) {
        cReturn = SDV_CpldI2cWrite(ulAddr, (UCHAR *)&uLData, 1, acBufCmd, nCmdLen);
        if (-1 == cReturn) {
            goto ERR_LABEL;
        }
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute ok!");
    return CMD_OK;

ERR_LABEL:
    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute error!");
    return CMD_ERROR;
}

ULONG
CMD_I2CCpldDebug2Write(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    CHAR cReturn = 0;
    ULONG ulAddr = 0;
    ULONG ulFileLen = 0;
    INT iRet;
    ULONG ulSes = atoi(argv[0]);
    CHAR acBufCmd[512] = {0};
    CHAR *pcDelim = ",";
    CHAR *pcStrTok = NULL;
    UINT nData = 0;
    UINT nCmdLen = 0;
    UINT nDataLen = 0;
    ULONG uLData = 0;
    UCHAR *pucBuffer = NULL;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulAddr);
    if (1 != iRet) {
        goto ERR_LABEL;
    }


    memset(acBufCmd, 0x00, sizeof(acBufCmd));
    sprintf(acBufCmd, "%s", argv[PRES_ARG + 1]);

    pcStrTok = strtok(acBufCmd, pcDelim);
    if (NULL == pcStrTok) {
        printf("strtok error\r\n");
        goto ERR_LABEL;
    }

    sscanf(pcStrTok, "%x", &nData);
    if (nData > 255) {
        printf("data > 255 error\r\n");
        goto ERR_LABEL;
    }

    nCmdLen = 0;
    acBufCmd[nCmdLen++] = nData;

    while ((pcStrTok = strtok(NULL, pcDelim))) {
        sscanf(pcStrTok, "%x", &nData);

        if (nData > 255) {
            printf("data > 255 error\r\n");
            goto ERR_LABEL;
        }

        acBufCmd[nCmdLen++] = nData;
    }

    sscanf(argv[PRES_ARG + 2], "%lx", &uLData);
    sscanf(argv[PRES_ARG + 3], "%lx", &nDataLen);

    pucBuffer = (UCHAR *)malloc(nDataLen + 1);
    if (NULL == pucBuffer) {
        goto ERR_LABEL;
    }

    memset(pucBuffer, uLData, nDataLen + 1);

    CMD_PrintfStr(ulSes, "\r\nWriting...");
    ulAddr = 0x52;
    ulAddr = ulAddr << 1;


    cReturn = SDV_CpldI2cWrite(ulAddr, pucBuffer, nDataLen, acBufCmd, nCmdLen);
    if (-1 == cReturn) {
        goto ERR_LABEL;
    }
    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute ok!");
    return CMD_OK;

ERR_LABEL:
    if (NULL != pucBuffer) {
        free(pucBuffer);
    }
    CMD_PrintfStr(ulSes, "\r\nWrite i2c cpld execute error!");
    return CMD_ERROR;
}



ULONG
CMD_PoeGetSoftVer(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulSes = atoi(argv[0]);
    TRANSMIT_MSG_STATUS_e eTransmitMsgStatus;
    PoE_Msg_t tPoeMsg;
    U8 bI2C_Device_Address = 0x78;

    if (SDV_IicInit()) {
        goto ERR_LABEL;
    }

    if (SDV_PoeInit()) {
        printf("poe init failed !\n");
        goto ERR_LABEL;
    }

    printf("\n\r4.6.7 Get_Port_Layer2_CDP_PSE_Data \n\r");
    U8 bSelected_Ch = 1; /* general index varible */

    eTransmitMsgStatus = Get_Port_Layer2_CDP_PSE_Data(
        _INOUT & tPoeMsg,
        bI2C_Device_Address,
        bSelected_Ch);

    if (eTransmitMsgStatus != eTransmitMsg_OK)
        return tPoeMsg.eMscc_poe_status;


    printf("  Port: %d\n\r", bSelected_Ch);

    printf("    PSE Allocated Power[mW]      : %lu\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ulPSE_Allocated_Power_mW);
    printf("    PD Requested Power[mW]       : %lu\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ulPD_Requested_Power_mW);

    printf("    PSE Power Type:\n\r");
    printf("      Port Priority              : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePortPriority);
    printf("      Power source type          : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_source_type);
    printf("      Power type                 : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.tLayer2_PD_Type.ePower_type);

    printf("    Power Class                  : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ePower_Class);
    printf("    PSE Power Pair               : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.ePSE_Power_Pair);

    printf("    MDI POWER STATUS:\n\r");
    printf("      PSE                        : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.PSE);
    printf("      SUPPORT_PoE                : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.SUPPORT_PoE);
    printf("      Port EnDis                 : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.Port_EnDis);
    printf("      Pairs Control Capability   : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.MDI_Power_Status_t.PairsControlCapability);

    printf("    Cable Length                 : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.bCable_Length);

    printf("    Power consumption:\n\r");
    printf("      Port power consumption[mW] : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.wPort_power_consumption);
    printf("      Port state                 : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_LLDP_Port_State);
    printf("      Layer2 request Pending     : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eLayer2_request_Pending);
    printf("      eReserved power mode active: %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Port_Layer2_CDP_PSE_Data_t.Get.PortIndicationAndPowerConsumption_t.eReserved_power_mode_active);

    #if 0
    printf("\n\r4.1.18 Get_Software_Version \n\r");

    eTransmitMsgStatus = Get_Software_Version(_INOUT & tPoeMsg, bI2C_Device_Address);

    if (eTransmitMsgStatus != eTransmitMsg_OK) {
        printf("eTransmitMsgStatus %d\n", eTransmitMsgStatus);
        goto ERR_LABEL;
    }


    printf("  Hardware version   : 0x%x\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.bHW_Version);
    printf("  Product number     : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.bProd);
    printf("  Software version   : %02d.%1d.%1d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMajor,
           tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMinor,
           tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bPatch);
    printf("    Ma:              : %02d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMajor);
    printf("    Mi:              : %1d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bMinor);
    printf("    Pa:              : %1d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.SW_VERSION_t.bPatch);

    printf("  Parameter number   : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.bParamNumber);
    printf("  Build number       : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.bBuild_Num);
    printf("  Internal SW number : %d\n\r", tPoeMsg.PoE_Msg_Prm_e.Get_Software_Version_t.Get.wInternalSWNumber);
    #endif

    SDV_IicClose();
    SDV_PoeClose();

    return CMD_OK;

ERR_LABEL:
    SDV_IicClose();
    SDV_PoeClose();

    CMD_PrintfStr(ulSes, "\r\nget software version %d error!");
    return CMD_ERROR;
}

/* #define OOP_MAX_LEN           (15*1024) */
#define OOP_MAX_LEN           (256)
#define CMD_EXIT_COUNT        (10)
#define CMD_DBG_RETRY_MAX     (3)

ULONG
CMD_DebugExceptOcm(ULONG ulSes, CHAR ucModIndex)
{
    CHAR acReadBuf[OOP_MAX_LEN] = {0};
    CHAR *pcReadBuf = NULL;
    UCHAR ucCount = 0;
    INT nDataLen = 0;
    INT nWriteLen = 0;
    INT iResult = 0;

    while (1) {
        memset(acReadBuf, 0x00, sizeof(acReadBuf));
        nDataLen = read(CmdCtrl[ulSes].BaseIo.ulIFd, acReadBuf, OOP_MAX_LEN);
        if (nDataLen > 0) {
            /* input is CTRL+ D = 4 */
            if (acReadBuf[0] == 4) {
                ucCount++;
            } else {
                ucCount = 0;
            }

            if (CMD_EXIT_COUNT == ucCount) {
                #if 0
                if (CPLD_UART_EDFA1 == ucModIndex) {
                    SDV_UartWrite(ucModIndex, "e1 echo off\r", strlen("e1 echo off\r"), CMD_DBG_RETRY_MAX);
                    SDV_UartWrite(ucModIndex, "e1 echo off\r", strlen("e1 echo off\r"), CMD_DBG_RETRY_MAX);
                }
                if (CPLD_UART_EDFA2 == ucModIndex) {
                    SDV_UartWrite(ucModIndex, "e2 echo off\r", strlen("e2 echo off\r"), CMD_DBG_RETRY_MAX);
                    SDV_UartWrite(ucModIndex, "e2 echo off\r", strlen("e2 echo off\r"), CMD_DBG_RETRY_MAX);
                }
                #endif
                break;
            }

            /* send data to module                                              */
            iResult = SDV_UartWrite(ucModIndex, acReadBuf, nDataLen, CMD_DBG_RETRY_MAX);
            if (-1 == iResult) {
                printf("SDV_UartWrite ERROR\n\r");
                goto ERR_LABEL;
            }
        }

        /* read data from module                                                */
        memset(acReadBuf, 0x00, sizeof(acReadBuf));
        nDataLen = SDV_UartRead(ucModIndex, acReadBuf, sizeof(acReadBuf), CMD_DBG_RETRY_MAX);
        pcReadBuf = acReadBuf;

        while (nDataLen > 0) {
            nWriteLen = write(CmdCtrl[ulSes].BaseIo.ulIFd, pcReadBuf, nDataLen);
            if (nWriteLen > 0) {
                nDataLen -= nWriteLen;
                pcReadBuf += nWriteLen;
            }
        }

        usleep(10 * 1000);
    }

    SDV_UartClose();
    return 0;

ERR_LABEL:
    SDV_UartClose();
    return -1;
}


ULONG
CMD_DebugToModule(ULONG ulSes, CHAR ucModIndex)
{
    BOOL bGetAttrSucFlag = 0;
    struct termios stTcFlagNew;
    struct termios stTcFlagOld;

    /* Get ttyS0 attributes        */
    if (tcgetattr(CmdCtrl[ulSes].BaseIo.ulIFd, &stTcFlagOld) < 0) {
        bGetAttrSucFlag = 0;

        CMD_OutputStr(ulSes, "Failed to get attributes! \r\n");

        goto ERR_LABEL;
    } else {
        bGetAttrSucFlag = 1;
    }

    stTcFlagNew = stTcFlagOld;
    stTcFlagNew.c_iflag &= ~(ICRNL | INLCR);
    /* ????,?Xmodem??????,???????? */
    stTcFlagNew.c_iflag &= ~(IXON | IXOFF | IXANY);
    stTcFlagNew.c_oflag &= ~(OCRNL | ONLCR);

    if (tcsetattr(CmdCtrl[ulSes].BaseIo.ulIFd, TCSAFLUSH, &stTcFlagNew) < 0) {
        CMD_OutputStr(ulSes, "Failed to change attributes! \r\n");

        tcsetattr(CmdCtrl[ulSes].BaseIo.ulIFd, TCSANOW, &stTcFlagOld);
        goto ERR_LABEL;
    }

    /* Set read attribute with none block                                       */
    fcntl(CmdCtrl[ulSes].BaseIo.ulIFd, F_SETFL, O_NONBLOCK);
    /* SETLOOP_SetLoopPriority(LOOP_PRIORITY_DEBUG); */
    sleep(1);

    if (-1 == CMD_DebugExceptOcm(ulSes, ucModIndex)) {
        goto ERR_LABEL;
    }
    /* Set read attribute back                                                  */
    fcntl(CmdCtrl[ulSes].BaseIo.ulIFd, F_SETFL, 0);

    if (1 == bGetAttrSucFlag) {
        tcsetattr(CmdCtrl[ulSes].BaseIo.ulIFd, TCSANOW, &stTcFlagOld);
    }

    /* CMD_PrintfStr(ulSes, "\r\nDebug to a module execute ok!"); */
    tcflush(CmdCtrl[ulSes].BaseIo.ulIFd, TCIFLUSH);
    return 0;

ERR_LABEL:
    fcntl(CmdCtrl[ulSes].BaseIo.ulIFd, F_SETFL, 0);
    /* CMD_PrintfStr(ulSes, "\r\nDebug to a module execute error!"); */
    tcflush(CmdCtrl[ulSes].BaseIo.ulIFd, TCIFLUSH);
    if (1 == bGetAttrSucFlag) {
        tcsetattr(CmdCtrl[ulSes].BaseIo.ulIFd, TCSANOW, &stTcFlagOld);
    }
    return -1;
}


ULONG
CMD_UartDebugMode(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulSes = atoi(argv[0]);
    UCHAR ucModeIndex = 0;
    INT iRet = 0;

    iRet = sscanf(argv[PRES_ARG], "%x", &ucModeIndex);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    if (CMD_DebugToModule(ulSes, ucModeIndex)) {
        goto ERR_LABEL;
    }

    CMD_PrintfStr(ulSes, "\r\ndebug to mode ok!\r\n");

    sleep(1);

    return CMD_OK;

ERR_LABEL:

    CMD_PrintfStr(ulSes, "\r\ndebug to mode fail!");
    return CMD_ERROR;
}

void
_PrintfByte(CHAR *pcMsgBuffer, INT iMsgLen)
{
    INT i;
    INT j;
    INT k = 0;

    INT line16 = iMsgLen / 16;
    INT rest = iMsgLen % 16;

    for (i = 0; i < 16; i++) {
        printf("%02X ", i);
    }
    printf("\n");

    for (i = 0; i < line16; i++) {
        for (j = 0; j < 16; j++) {
            printf("%02hhX ", pcMsgBuffer[k++]);
        }
        printf("\n");
    }

    for (i = 0; i < rest; i++) {
        printf("%02hhX ", pcMsgBuffer[k++]);
    }
    printf("\n");

}

ULONG
CMD_UartLoop1(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulSes = atoi(argv[0]);
    ULONG ulTimes = 0;
    INT iRet = 0;

#define TST_BUF_LENGTH 96

    UINT uiAllSendTimes = 0;

    INT i = 0;
    INT iResult = 0;
    UCHAR ucUartIndex = 0;
    UCHAR aucWrData[TST_BUF_LENGTH];
    UCHAR aucRdData[TST_BUF_LENGTH];
    UCHAR ucSumCurernt;
    UCHAR ucEnd = 0xAA;
    UCHAR ucModIndex = 0;

    UCHAR ucSum = 0;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulTimes);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    for (i = 0; i < TST_BUF_LENGTH; i++) {
        aucWrData[i] = i;
    }

    for (i = 0; i < TST_BUF_LENGTH; i++) {
        ucSum += aucWrData[i];
    }

    /*send */
    SDV_UartLock();
    printf("\n");
    while (ulTimes--) {
        uiAllSendTimes++;

        ucModIndex = 2;

        /* send command to edfa                                                     */
        SDV_UartWrite(ucModIndex, (CHAR *)aucWrData, sizeof(aucWrData), 1);

        if (uiAllSendTimes % 0x50 == 0) {
            printf("%x...\n", uiAllSendTimes);
        }

        usleep(10 * 1000);
        memset(aucRdData, 0, sizeof(aucRdData));
        /* read data from edfa                                                      */
        iResult = SDV_UartRead(ucModIndex, (CHAR *)aucRdData, TST_BUF_LENGTH, 20);
        if (iResult <= 0) {
            printf("don't receive response from %d!\n", ucModIndex);
            goto ERR_LABEL;
        }


        ucSumCurernt = 0;
        for (i = 0; i < TST_BUF_LENGTH; i++) {
            ucSumCurernt += aucRdData[i];
        }

        if (ucSumCurernt != ucSum) {
            printf("### total send time:%d,", uiAllSendTimes);

            _PrintfByte((CHAR *)aucRdData, sizeof(aucRdData));
            sleep(1);
            goto ERR_LABEL;
        }

        usleep(10 * 1000);

    }

    SDV_UartUnLock();
    CMD_PrintfStr(ulSes, "\r\nend !\r\n");
    return CMD_OK;

ERR_LABEL:
    SDV_UartWrite(ucModIndex, (CHAR *)&ucEnd, 1, 1);

    SDV_UartUnLock();
    CMD_PrintfStr(ulSes, "\r\nend error!");
    return CMD_ERROR;
}


ULONG
CMD_UartLoop2(CHAR *pcBuf, INT Len, CHAR **argv, INT argc)
{
    ULONG ulSes = atoi(argv[0]);
    ULONG ulTimes = 0;
    INT iRet = 0;

#define TST_BUF_LENGTH 96

    UINT uiAllSendTimes = 0;

    INT i = 0;
    INT iResult = 0;
    UCHAR ucUartIndex = 0;
    UCHAR aucWrData[TST_BUF_LENGTH];
    UCHAR aucRdData[TST_BUF_LENGTH];
    UCHAR ucSumCurernt;
    UCHAR ucEnd = 0xAA;
    UCHAR ucModIndex = 0;

    UCHAR ucSum = 0;

    iRet = sscanf(argv[PRES_ARG], "%lx", &ulTimes);
    if (1 != iRet) {
        goto ERR_LABEL;
    }

    for (i = 0; i < TST_BUF_LENGTH; i++) {
        aucWrData[i] = i;
    }

    for (i = 0; i < TST_BUF_LENGTH; i++) {
        ucSum += aucWrData[i];
    }

    /*send */
    SDV_UartLock();
    printf("\n");
    while (ulTimes--) {
        uiAllSendTimes++;
        if (0 == uiAllSendTimes % 3) {
            ucModIndex = 0;
        } else if (1 == uiAllSendTimes % 3) {
            ucModIndex = 1;
        } else {
            ucModIndex = 4;
        }

        /* send command to edfa                                                     */
        SDV_UartWrite(ucModIndex, (CHAR *)aucWrData, sizeof(aucWrData), 1);

        if (uiAllSendTimes % 0x50 == 0) {
            printf("%x...\n", uiAllSendTimes);
        }

        usleep(10 * 1000);
        memset(aucRdData, 0, sizeof(aucRdData));
        /* read data from edfa                                                      */
        iResult = SDV_UartRead(ucModIndex, (CHAR *)aucRdData, TST_BUF_LENGTH, 20);
        if (iResult <= 0) {
            /* printf("don't receive response from %d!\n",ucModIndex); */
            goto ERR_LABEL;
        }


        ucSumCurernt = 0;
        for (i = 0; i < TST_BUF_LENGTH; i++) {
            ucSumCurernt += aucRdData[i];
        }

        if (ucSumCurernt != ucSum) {
            printf("### total send time:%d,", uiAllSendTimes);

            _PrintfByte((CHAR *)aucRdData, sizeof(aucRdData));
            sleep(1);
            goto ERR_LABEL;
        }

        usleep(10 * 1000);

    }

    SDV_UartUnLock();
    CMD_PrintfStr(ulSes, "\r\nend !\r\n");
    return CMD_OK;

ERR_LABEL:
    SDV_UartWrite(ucModIndex, (CHAR *)&ucEnd, 1, 1);
    if (0 == ucModIndex) {
        CMD_PrintfStr(ulSes, "\r\nedfa1 ");
    }
    if (1 == ucModIndex) {
        CMD_PrintfStr(ulSes, "\r\nedfa2 ");
    }
    if (4 == ucModIndex) {
        CMD_PrintfStr(ulSes, "\r\nextend ");
    }
    SDV_UartUnLock();
    CMD_PrintfStr(ulSes, "\r\nend error!");
    return CMD_ERROR;
}


CMD_TreeRecd
    CmdSdvTestTable[] =
{

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "cpld", "Test CPLD"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "Read register of CPLD"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x0000-0x10000>", "Address of CPLD"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<length>", CMD_ReadCpld, NULL, "Length of the data"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "cpld", "Test CPLD"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "Write register of CPLD"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x0000-0x10000>", "Address of CPLD"),
    CMD_VLEAF(CMD_STR, "[0-255]", "<xx,xx,...,xx>", CMD_WriteCpld, NULL, "Value into the register"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "cpldfan", "Test FAN CPLD"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "Read register of CPLD"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x0000-0xffff>", CMD_ReadFanCpld, NULL, "Address of CPLD"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "cpldfan", "Test FAN CPLD"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "Write register of CPLD"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x0000-0xffff>", "Address of CPLD"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteFanCpld, NULL, "Value into the register"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "mdio", "Test mdio"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "Read of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x00-0xff>", CMD_ReadMdio, NULL, "reg addr of mdio"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "mdio", "Test mdio"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "Write of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "reg addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteMdio, NULL, "Value into the register"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "Read of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x00-0xff>", CMD_ReadMvSwitch, NULL, "reg addr of mdio"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "Write of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "reg addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteMvSwitch, NULL, "Value into the register"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "modeinit", CMD_MvInit, NULL, "switch init"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "phyread", "phy Read of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "dev addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x00-0xff>", CMD_ReadMvPhy, NULL, "reg addr of mdio"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "phywrite", "phy Write of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "dev addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "reg addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteMvPhy, NULL, "Value into the register"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "xmdioread", "xmdio Read of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "portnum addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x00-0xff>", CMD_ReadMvXmdio, NULL, "reg addr of mdio"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "xmdiowrite", "xmdio Write of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "portnum addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "phy addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "reg addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteMvXmdio, NULL, "Value into the register"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "pagemdioread", "pagemdio Read of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "portnum addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "pagenum of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<0x00-0xff>", CMD_ReadMvPagemdio, NULL, "reg addr of mdio"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "MV6390", "Test MV6390 Multi Add"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "pagemdiowrite", "pagemdio Write of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "portnum addr of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "pagenum of mdio"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0x00-0xff>", "reg addr of mdio"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<value>", CMD_WriteMvPagemdio, NULL, "Value into the register"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "usb-spi", "Test usb spi com"),
    CMD_VLEAF(CMD_STR, "[0-255]", "<xx,xx,...,xx>", CMD_UsbSpiCom, NULL, "the data stream to write"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "jtag", "Test cpld jtag"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "upgadecpld", "upgade cpld"),
    CMD_VALUE(CMD_STR, "[1-128]", "<filename>", "file to cpld "),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-1>", CMD_JtagWriteCpld, NULL, "0-cpld0,1-cpld1"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "jtag", "Test cpld jtag"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "reset", "reset cpld"),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-1>", CMD_JtagResetCpld, NULL, "0-cpld0,1-cpld1"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "readedfa1fpga", "Read SPI E1E2 FPGA flash to /tmp/EDFAflashfile.bin"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0-FFFFFF>", "address of the EDFA flash, in hex format"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-1000000>", CMD_ReadSpiE1Flash, NULL, "Size of reading out, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "writeedfa1fpga", "Write SPI E1 FPGA flash"),
    CMD_VALUE(CMD_STR, "[1-128]", "<filename>", "file to FPGA flash"),
    CMD_VLEAF(CMD_STR, "[1-64]", "<addr>", CMD_WriteSpiE1Flash, NULL, "The address of spi flash to write, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eraseedfa1fpga", CMD_SpiFlashEraseE1, NULL, "Erase E1 Fpga flash"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "readedfa2fpga", "Read SPI E1E2 FPGA flash to /tmp/EDFAflashfile.bin"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0-FFFFFF>", "address of the EDFA flash, in hex format"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-1000000>", CMD_ReadSpiE2Flash, NULL, "Size of reading out, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "writeedfa2fpga", "Write SPI E2 FPGA flash"),
    CMD_VALUE(CMD_STR, "[1-128]", "<filename>", "file to FPGA flash"),
    CMD_VLEAF(CMD_STR, "[1-64]", "<addr>", CMD_WriteSpiE2Flash, NULL, "The address of spi flash to write, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "spiflash", "Test SPI flash"),
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eraseedfa2fpga", CMD_SpiFlashEraseE2, NULL, "Erase E2 Fpga flash"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i350", "Test i350"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "upgeeprom", "upgade eeprom"),
    CMD_VALUE(CMD_STR, "[1-128]", "<filename>", "file to i350"),
    CMD_VLEAF(CMD_STR, "[1-64]", "<addr>", CMD_I350EEPROMWrite, NULL, "The address of eeprom to write, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i350", "Test i350"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "readeeprom", "upgade eeprom"),
    CMD_VALUE(CMD_STR, "[1-10]", "<0-FFFFFF>", "address of the eeprom, in hex format"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-1000000>", CMD_I350EEPROMRead, NULL, "Size of reading out, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-x86", "Test I2C BUS"),
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "gettemp", CMD_IICReadTemp, NULL, "Read temp"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-x86", "Test I2C BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepcfgread", "Read I2C EEPROM to /tmp/Eeprom1file.bin"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VALUE(CMD_STR, "[1-10]", "<1-20000>", "Size of reading out, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-1>", CMD_IICReadEEPROM, NULL, "Read mode:0-restore to file,1-print to screen"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-x86", "Test I2C BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepcfgwrite", "Write I2C EEPROM"),
    CMD_VALUE(CMD_STR, "[1-64]", "<filename>", "File for storing the reading out"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<addr>", CMD_IICWriteEEPROM, NULL, "Address of the eeprom, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-x86", "Test I2C BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepidread", "Read I2C EEPROM to /tmp/Eeprom2file.bin"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VALUE(CMD_STR, "[1-10]", "<1-20000>", "Size of reading out, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-1>", CMD_IICReadEEPROMID, NULL, "Read mode:0-restore to file,1-print to screen"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-x86", "Test I2C BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepidwrite", "Write I2C EEPROM"),
    CMD_VALUE(CMD_STR, "[1-64]", "<filename>", "File for storing the reading out"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<addr>", CMD_IICWriteEEPROMID, NULL, "Address of the eeprom, in hex format"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepidread", "Read I2C EEPROM to /tmp/Eeprom2file.bin"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VALUE(CMD_STR, "[1-10]", "<1-20000>", "Size of reading out, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-1>", CMD_IICReadEEPROMIDByCpld, NULL, "Read mode:0-restore to file,1-print to screen"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepidwrite", "Write I2C EEPROM"),
    CMD_VALUE(CMD_STR, "[1-64]", "<filename>", "File for storing the reading out"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<addr>", CMD_IICWriteEEPROMIDByCpld, NULL, "Address of the eeprom, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepfanread", "Read I2C EEPROM"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VALUE(CMD_STR, "[1-10]", "<1-20000>", "Size of reading out, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-4]", "<0-4>", CMD_IICReadEEPROMFanByCpld, NULL, "0-fan0,1-fan1,2-fan2,3-fan3"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "eepfanwrite", "Write I2C EEPROM"),
    CMD_VALUE(CMD_STR, "[1-64]", "<filename>", "File for storing the reading out"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-4]", "<0-4>", CMD_IICWriteEEPROMFanByCpld, NULL, "0-fan0,1-fan1,2-fan2,3-fan3"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "powerinforead", "Read power info"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the eeprom, in hex format"),
    CMD_VALUE(CMD_STR, "[1-10]", "<1-20000>", "Size of reading out, in hex format"),
    CMD_VLEAF(CMD_INT, "[0-2]", "<0-2>", CMD_IICReadPowerByCpld, NULL, "0-power0,1-power1"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "cpld i2c read"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the slave"),
    CMD_VALUE(CMD_STR, "[0-255]", "<xx,xx,...,xx>", "i2c command"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_I2CCpldRead, NULL, "Size of reading out, in hex format"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld", "Test I2C-cpld BUS"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "cpld i2c write"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the slave"),
    CMD_VALUE(CMD_STR, "[0-255]", "<xx,xx,...,xx>", "i2c command"),
    CMD_VLEAF(CMD_STR, "[0-255]", "<xx,xx,...,xx>", CMD_I2CCpldWrite, NULL, "i2c data"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld-debug1", "Test I2C-cpld BUS debug1"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "read", "cpld i2c"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the slave"),
    CMD_VALUE(CMD_STR, "[0-255]", "<xx,xx,...,xx>", "i2c command"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_I2CCpldDebug1Read, NULL, "i2c times"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld-debug1", "Test I2C-cpld BUS debug1"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "write", "cpld i2c debug"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the slave"),
    CMD_VALUE(CMD_STR, "[0-255]", "<xx,xx,...,xx>", "i2c command"),
    CMD_VALUE(CMD_STR, "[0-255]", "<255>", "i2c data"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_I2CCpldDebug1Write, NULL, "i2c times"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "i2c-cpld-debug2", "Test I2C-cpld BUS debug2"),
    CMD_VALUE(CMD_STR, "[1-10]", "<addr>", "Address of the slave"),
    CMD_VALUE(CMD_STR, "[0-255]", "<xx,xx,...,xx>", "i2c command"),
    CMD_VALUE(CMD_STR, "[0-255]", "<255>", "i2c data"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_I2CCpldDebug2Write, NULL, "i2c times"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "uartdebug", "Test uart"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "mode", "mode"),
    CMD_VLEAF(CMD_INT, "[0-5]", "<0-5>", CMD_UartDebugMode, NULL, "0-edfa0,1-edfa1,2-wss"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "uartdebug", "Test uart"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "loopback1", "loop test"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_UartLoop1, NULL, "times of loop"),
    END_RECORD,

    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "uartdebug", "Test uart"),
    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "loopback2", "loop test"),
    CMD_VLEAF(CMD_STR, "[1-10]", "<1-100000>", CMD_UartLoop2, NULL, "times of loop"),
    END_RECORD,


    CMD_TOKEN(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "poe", "Test poe func"),
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "getSoftVer", CMD_PoeGetSoftVer, NULL, "get software version"),
    END_RECORD,



    {END_TREE, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL}

};
