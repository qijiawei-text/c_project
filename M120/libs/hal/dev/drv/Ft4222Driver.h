/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Ft4222Driver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once



#include "DevInternal.h"
#include "./thirdparty/libft4222.h"



#define EN_I2C_SLAVE_MAX        (10)
#define EN_SPI_MASTER_MAX       (10)



#define I2C_FT4222_MAX_NUM      (EN_I2C_SLAVE_MAX + EN_SPI_MASTER_MAX)


/*************************************************/
class CFt4222Driver
{
public:

    CFt4222Driver()
    {

    }

    virtual
    ~CFt4222Driver()
    {

#if 0
        LOG_WRITE(EN_LOG_NOTICE, "~CI2cFt4222Driver +++");
        std::vector<FT_HANDLE> staHandles = __HandlerVec();

        for (uint32_t i = 0; i < staHandles.size(); i++) {
            if (staHandles[i] != nullptr) {

                (void)FT4222_UnInitialize(staHandles[i]);
                (void)FT_Close(staHandles[i]);

                staHandles[i] = nullptr;
            }
        }

        staHandles.clear();
        __SetDevNum(0);

        LOG_WRITE(EN_LOG_NOTICE, "~CI2cFt4222Driver ---");
#endif
    }

    bool Register();

protected:
    uint32_t __GetUsbList();
    virtual uint32_t __GetDevNum() = 0;
    virtual void __SetDevNum(uint32_t uiDevNum) = 0;
    virtual std::vector<FT_HANDLE> & __HandlerVec() = 0;
    virtual bool __Config(uint32_t uiType, uint32_t uiLocationId) = 0;

    static std::mutex ms_stMutex;
};


class CI2cFt4222Driver : public CFt4222Driver
{
public:

    virtual
    ~CI2cFt4222Driver()
    {
        LOG_WRITE(EN_LOG_DEBUG, "~CI2cFt4222Driver +++");
        printf("~CI2cFt4222Driver +++\n");

        ms_stMutex.lock();
        for (uint32_t i = 0; i < ms_staHandles.size(); i++) {
            if (ms_staHandles[i] != nullptr) {

                (void)FT4222_UnInitialize(ms_staHandles[i]);
                (void)FT_Close(ms_staHandles[i]);

                ms_staHandles[i] = nullptr;
            }
        }

        ms_staHandles.clear();
        ms_uiDevNum = 0;

        ms_stMutex.unlock();

        printf("~CI2cFt4222Driver ---\n");
        LOG_WRITE(EN_LOG_DEBUG, "~CI2cFt4222Driver ---");
    }

    bool Write(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t ulLenW);
    bool Read(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t ulLenW, uint8_t *pucBufR, uint32_t uiLenR);


private:

    static uint32_t ms_uiDevNum;
    static std::array<std::mutex, EN_I2C_SLAVE_MAX> ms_staMutexs;

    static std::vector<FT_HANDLE> ms_staHandles;

    virtual uint32_t
    __GetDevNum()
    {
        return ms_uiDevNum;
    }

    virtual void
    __SetDevNum(uint32_t uiDevNum)
    {
        ms_uiDevNum = uiDevNum;
    }

    virtual std::vector<FT_HANDLE> &
    __HandlerVec()
    {
        return ms_staHandles;
    }
    virtual bool __Config(uint32_t uiType, uint32_t uiLocationId) override;

};


class CSpiFt4222Driver : public CFt4222Driver
{
public:
    ~CSpiFt4222Driver()
    {
        LOG_WRITE(EN_LOG_DEBUG, "~CSpiFt4222Driver +++");

        for (uint32_t i = 0; i < ms_staHandles.size(); i++) {
            printf("i = %u +++", i);
            if (ms_staHandles[i] != nullptr) {

                (void)FT4222_UnInitialize(ms_staHandles[i]);
                (void)FT_Close(ms_staHandles[i]);

                ms_staHandles[i] = nullptr;
            }

            printf("i = %u ---", i);
        }

        ms_staHandles.clear();
        ms_uiDevNum = 0;
        LOG_WRITE(EN_LOG_DEBUG, "~CSpiFt4222Driver ---");
    }
    bool Read(uint32_t uiChipIdx, uint8_t *pucBuf, uint32_t uiLen);
    bool Write(uint32_t uiChipIdx, uint8_t *pucBuf, uint32_t uiLen);
    bool ReadWrite(uint32_t uiChipIdx, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR);

private:

    static uint32_t ms_uiDevNum;
    static std::array<std::mutex, EN_SPI_MASTER_MAX> ms_staMutexs;
    static std::vector<FT_HANDLE> ms_staHandles;

    virtual uint32_t
    __GetDevNum()
    {
        return ms_uiDevNum;
    }

    virtual void
    __SetDevNum(uint32_t uiDevNum)
    {
        ms_uiDevNum = uiDevNum;
    }

    virtual std::vector<FT_HANDLE> &
    __HandlerVec()
    {
        return ms_staHandles;
    }

    virtual bool __Config(uint32_t uiType, uint32_t uiLocationId) override;

};









