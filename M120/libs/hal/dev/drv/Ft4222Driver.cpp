/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Ft4222Driver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Ft4222Driver.h"

/*=============================variables=========================*/

std::mutex CFt4222Driver::ms_stMutex;

uint32_t CI2cFt4222Driver::ms_uiDevNum = 0;
std::array<std::mutex, EN_I2C_SLAVE_MAX> CI2cFt4222Driver::ms_staMutexs;
std::vector<FT_HANDLE> CI2cFt4222Driver::ms_staHandles;

uint32_t CSpiFt4222Driver::ms_uiDevNum = 0;
std::array<std::mutex, EN_SPI_MASTER_MAX> CSpiFt4222Driver::ms_staMutexs;
std::vector<FT_HANDLE> CSpiFt4222Driver::ms_staHandles;




/*==============================function=========================*/

bool
CFt4222Driver::Register()
{
    bool bRet = false;
    uint32_t uiIdx = 0;
    uint32_t uiDevNum = 0;
    FT_DEVICE_LIST_INFO_NODE *pstDevInfo = NULL;

    ms_stMutex.lock();

    if (__GetDevNum() > 0) {
        bRet = true;
        goto LABEL_END;
    }

    uiDevNum = __GetUsbList();

    if (0 == uiDevNum) {
        bRet = false;
        goto LABEL_END;
    }

    /* Allocate storage */
    pstDevInfo = new FT_DEVICE_LIST_INFO_NODE[uiDevNum];

    /* Populate the list of info nodes */
    if (FT_GetDeviceInfoList(pstDevInfo, &uiDevNum) != FT_OK) {
        bRet = false;
        goto LABEL_END;
    }

    for (uint32_t i = 0; i < uiDevNum; i++) {
        if (__Config(pstDevInfo[i].Type, pstDevInfo[i].LocId)) {
            uiIdx++;
        }
    }

    if (uiIdx > 0) {
        bRet = true;
    }

    __SetDevNum(uiIdx);

LABEL_END:

    if (pstDevInfo) {
        delete[] pstDevInfo;
    }

    ms_stMutex.unlock();

    return bRet;

}


uint32_t
CFt4222Driver::__GetUsbList()
{

    uint32_t uiDevNum = 0;

    /* ms_stMutex.lock(); */

    if (FT_CreateDeviceInfoList(&uiDevNum) != FT_OK) {
        LOG_WRITE(EN_LOG_ERROR, "FT_CreateDeviceInfoList error.");
    }

    /* ms_stMutex.unlock(); */

    return uiDevNum;

}


bool
CI2cFt4222Driver::__Config(uint32_t uiType, uint32_t uiLocationId)
{
    FT_HANDLE pvHandle = (FT_HANDLE)NULL;

    if (uiType != FT_DEVICE_4222H_3) {
        return false;
    }

    if (FT_OpenEx((PVOID)(uintptr_t)uiLocationId, FT_OPEN_BY_LOCATION, &pvHandle) != FT_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "FT_OpenEx error. uiLocationId = %d", uiLocationId);
        std::cout << "FT_OpenEx error. LocationId = " << uiLocationId << std::endl;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Usb to i2c config. LocationId = %d", uiLocationId);
    std::cout << "Usb to i2c config. LocationId = " << uiLocationId << " pvHandle = " << pvHandle << std::endl;

    ms_staHandles.push_back(pvHandle);

    /* Configure the FT4222 as an I2C Master */
    if (FT4222_I2CMaster_Init(pvHandle, 100) != FT4222_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "FT4222_I2CMaster_Init error.");
        return false;
    }

    /* Reset the I2CM registers to a known state. */
    if (FT4222_I2CMaster_Reset(pvHandle) != FT4222_OK) {
        LOG_WRITE(EN_LOG_NOTICE, "FT4222_I2CMaster_Reset error.");
        return false;
    }

    return true;

}



bool
CI2cFt4222Driver::Read(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    FT4222_STATUS ft4222Status;
    uint16_t bytesRead = 0;
    uint16 bytesWritten = 0;


    if (false == Register()) {
        LOG_WRITE(EN_LOG_ERROR, "read usb ft init fail!");
        return false;
    }


    if (uiDevIdx >= ms_staHandles.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiDevIdx:%u >= ms_staHandles.size():%lu", uiDevIdx, ms_staHandles.size());
        return false;
    }

    ms_staMutexs[uiDevIdx].lock();

    ft4222Status = FT4222_I2CMaster_Write(ms_staHandles[uiDevIdx],
                                          uiAddr,
                                          pucBufW,
                                          uiLenW,
                                          &bytesWritten);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_I2CMaster_Write 1 failed (error %d)", (int)ft4222Status);
        goto ERR_LABEL;
    }

    ft4222Status = FT4222_I2CMaster_Read(ms_staHandles[uiDevIdx],
                                         uiAddr,
                                         pucBufR,
                                         uiLenR,
                                         &bytesRead);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_I2CMaster_Read failed (error %d)", (int)ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesRead != uiLenR) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_I2CMaster_Read read %u of %u bytes", bytesRead, uiLenR);
        goto ERR_LABEL;
    }

    ms_staMutexs[uiDevIdx].unlock();
    return true;

ERR_LABEL:

    ms_staMutexs[uiDevIdx].unlock();
    return false;
}

bool
CI2cFt4222Driver::Write(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
{

    FT4222_STATUS ft4222Status;
    uint16 bytesWritten = 0;

    if (false == Register()) {
        LOG_WRITE(EN_LOG_ERROR, "write usb ft init fail!");
        return false;
    }


    if (uiDevIdx >= ms_staHandles.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiDevIdx:%u >= ms_staHandles.size():%lu", uiDevIdx, ms_staHandles.size());
        return false;
    }

    ms_staMutexs[uiDevIdx].lock();

    ft4222Status = FT4222_I2CMaster_Write(ms_staHandles[uiDevIdx],
                                          uiAddr,
                                          pucBufW,
                                          uiLenW,
                                          &bytesWritten);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_I2CMaster_Write error.");
        LOG_WRITE(EN_LOG_ERROR, "error = %d", (int)ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesWritten != uiLenW) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_I2CMaster_Write wrote %u of %u bytes", bytesWritten, uiLenW);
        goto ERR_LABEL;
    }

    ms_staMutexs[uiDevIdx].unlock();
    return true;

ERR_LABEL:
    ms_staMutexs[uiDevIdx].unlock();
    return false;
}



bool
CSpiFt4222Driver::__Config(uint32_t uiType, uint32_t uiLocationId)
{
    FT_HANDLE pvHandle = (FT_HANDLE)NULL;

    if (uiType != FT_DEVICE_4222H_1_2) {
        return false;
    }

    if (FT_OpenEx((PVOID)(uintptr_t)uiLocationId, FT_OPEN_BY_LOCATION, &pvHandle) != FT_OK) {
        LOG_WRITE(EN_LOG_ERROR, "FT_OpenEx error. uiLocationId = %d", uiLocationId);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Usb to spi config. LocationId = %d", uiLocationId);

    /* ms_staLocalIds.push_back(uiLocationId); */
    ms_staHandles.push_back(pvHandle);

    /* TBD:
       SPI Master can assert SS0O in single mode
       SS0O and SS1O in dual mode, and
       SS0O, SS1O, SS2O and SS3O in quad mode.
     */
#define SLAVE_SELECT(x) (1 << (x))
/* #define CPLD1_CS SLAVE_SELECT(0)  / * only use single mode, and select 0 * / */

    /* Configure the FT4222 as an SPI Master. */
    if (FT4222_OK != FT4222_SPIMaster_Init(
            pvHandle,
            SPI_IO_SINGLE,                 /* 1 channel */
            CLK_DIV_32,                    /* 60 MHz / 32 == 1.875 MHz */
            CLK_IDLE_LOW,                  /* clock idles at logic 0 */
            CLK_LEADING,                   /* data captured on rising edge */
            2)) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_Init failed");
        return false;
    }

    if (FT4222_OK != FT4222_SPI_SetDrivingStrength(pvHandle,
                                                   DS_8MA,
                                                   DS_8MA,
                                                   DS_8MA)) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPI_SetDrivingStrength failed");
        return false;
    }

    return true;

}


bool
CSpiFt4222Driver::Read(uint32_t uiChipIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesTransceived = 0;

    if (false == Register()) {
        LOG_WRITE(EN_LOG_ERROR, "write usb ft init fail!");
        return false;
    }


    if (uiChipIdx >= ms_staHandles.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiDevIdx:%u >= ms_staHandles.size():%lu", uiChipIdx, ms_staMutexs.size());
        return false;
    }

    ms_staMutexs[uiChipIdx].lock();

    ft4222Status = FT4222_SPIMaster_SingleRead(
        ms_staHandles[uiChipIdx],
        pucBuf,
        uiLen,
        &bytesTransceived,
        TRUE);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleRead failed (error %d)!", ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesTransceived != uiLen) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleRead "
                  "transceived %u bytes instead of %u.",
                  bytesTransceived,
                  uiLen);
        goto ERR_LABEL;
    }

    ms_staMutexs[uiChipIdx].unlock();
    return true;

ERR_LABEL:

    ms_staMutexs[uiChipIdx].unlock();
    return false;
}

bool
CSpiFt4222Driver::Write(uint32_t uiChipIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesTransceived = 0;


    if (false == Register()) {
        LOG_WRITE(EN_LOG_ERROR, "write usb ft init fail!");
        return false;
    }


    if (uiChipIdx >= ms_staHandles.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiDevIdx:%u >= ms_staHandles.size():%lu", uiChipIdx, ms_staHandles.size());
        return false;
    }

    ms_staMutexs[uiChipIdx].lock();

    ft4222Status = FT4222_SPIMaster_SingleWrite(
        ms_staHandles[uiChipIdx],
        pucBuf,
        uiLen,
        &bytesTransceived,
        TRUE);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleWrite failed (error %d)!", ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesTransceived != uiLen) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleWrite "
                  "transceived %u bytes instead of %u.",
                  bytesTransceived,
                  uiLen);
        goto ERR_LABEL;
    }

    ms_staMutexs[uiChipIdx].unlock();
    return true;

ERR_LABEL:
    ms_staMutexs[uiChipIdx].unlock();
    return false;
}


bool
CSpiFt4222Driver::ReadWrite(uint32_t uiChipIdx, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
{
    FT4222_STATUS ft4222Status;
    uint16 bytesTransceived = 0;

    if (false == Register()) {
        LOG_WRITE(EN_LOG_ERROR, "write usb ft init fail!");
        return false;
    }

    if (uiChipIdx >= ms_staHandles.size()) {
        LOG_WRITE(EN_LOG_ERROR, "uiDevIdx:%u >= ms_staHandles.size():%lu", uiChipIdx, ms_staHandles.size());
        return false;
    }

    ms_staMutexs[uiChipIdx].lock();

    ft4222Status = FT4222_SPIMaster_SingleWrite(
        ms_staHandles[uiChipIdx],
        pucBufW,
        uiLenW,
        &bytesTransceived,
        FALSE);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleWrite failed (error %d)!", ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesTransceived != uiLenW) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleWrite "
                  "transceived %u bytes instead of %u.",
                  bytesTransceived,
                  uiLenW);
        goto ERR_LABEL;
    }

    ft4222Status = FT4222_SPIMaster_SingleRead(
        ms_staHandles[uiChipIdx],
        pucBufR,
        uiLenR,
        &bytesTransceived,
        TRUE);
    if (FT4222_OK != ft4222Status) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleRead failed (error %d)!", ft4222Status);
        goto ERR_LABEL;
    }

    if (bytesTransceived != uiLenR) {
        LOG_WRITE(EN_LOG_ERROR, "FT4222_SPIMaster_SingleRead "
                  "transceived %u bytes instead of %u.",
                  bytesTransceived,
                  uiLenW);
        goto ERR_LABEL;
    }

    ms_staMutexs[uiChipIdx].unlock();
    return true;

ERR_LABEL:

    ms_staMutexs[uiChipIdx].unlock();
    return false;
}


