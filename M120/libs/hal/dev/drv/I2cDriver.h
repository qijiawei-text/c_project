/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      I2cDriver.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevDriver.h"
#include "DevInternal.h"
#include "InterfaceDriver.h"
#include "Ft4222Driver.h"
#include "./thirdparty/Susi4.h"


#define I2C_FPGA_MAX_NUM                1

#define DEF_16B_PAGE_LEN           (16u)
#define DEF_128KB_PAGE_LEN         (128u)
#define DEF_256KB_PAGE_LEN         (256u)



/*************************************************/
class CI2c /*: public CDevDriver */
{
public:
    CI2c()
    {
    }

    virtual
    ~CI2c()
    {
    }

    virtual bool Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) = 0;
    virtual bool Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) = 0;
    virtual bool
    WriteByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW)
    {
        return true;
    }
    virtual bool
    ReadByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR)
    {
        return true;
    }

    virtual uint32_t
    PageLenR(uint32_t uiLen, uint32_t uiAddr)
    {
        return uiLen;
    }

    virtual uint32_t
    PageLenW(uint32_t uiLen, uint32_t uiAddr)
    {
        return uiLen;
    }

protected:

    uint32_t m_uiHwIdx;
    uint32_t m_uiHwSubIdx;

    /* uint32_t m_uiAddr;

       virtual bool
       Switch(uint32_t uiIdx)
       {
        return true;
       }

       virtual bool WriteDev(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) = 0;
       virtual bool ReadDev(uint32_t uiDevIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) = 0;


       private:
       virtual void MutexLock(uint32_t uiIdx) = 0;
       virtual void MutexUnlock(uint32_t uiIdx) = 0;
       virtual uint32_t GetDevMaxNum(void) = 0; */

};

class CFtI2c : public CI2c
{
public:
    CFtI2c()
    {

    }

    virtual
    ~CFtI2c()
    {
        LOG_WRITE(EN_LOG_DEBUG, "~CFtI2c()");
    }

    virtual uint32_t
    PageLenW(uint32_t uiLen, uint32_t uiAddr)
    {
        uint32_t uiPageLen = 0;
        uiPageLen = uiLen >= (DEF_256KB_PAGE_LEN - (uiAddr % DEF_256KB_PAGE_LEN))
                    ? (DEF_256KB_PAGE_LEN - (uiAddr % DEF_256KB_PAGE_LEN))
                    : uiLen;

        return uiPageLen;
    }

protected:

    virtual bool Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) override;
    virtual bool Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) override;

    CI2cFt4222Driver m_stI2cFt;

};


static bool m_bSusiInit = false;
class CSusi4I2c : public CI2c
{
public:
    CSusi4I2c()
    {
        if (!m_bSusiInit) {
            SusiStatus_t status;
            status = SusiLibInitialize();

            if (status == SUSI_STATUS_ERROR) {
                LOG_WRITE(EN_LOG_ERROR, "Your Linux capabilities is not enough, recommond to become ROOT!");
            } else {
                m_bSusiInit = true;
            }
        }
    }

#if 0
    CSusi4I2c(uint32_t uiIdx) : CI2c(uiIdx)
    {
        if (!m_bSusiInit) {
            SusiStatus_t status;
            status = SusiLibInitialize();

            if (status == SUSI_STATUS_ERROR) {
                LOG_WRITE(EN_LOG_ERROR, "Your Linux capabilities is not enough, recommond to become ROOT!");
            } else {
                m_bSusiInit = true;
            }
        }
    }
#endif

    virtual
    ~CSusi4I2c()
    {
        if (m_bSusiInit) {
            /* SusiLibUninitialize(); */
        }
    }

    virtual bool Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) override;
    virtual bool Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) override;
    virtual bool WriteByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) override;
    virtual bool ReadByte(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) override;

    virtual uint32_t
    PageLenR(uint32_t uiLen, uint32_t uiAddr)
    {
        uint32_t uiPageLen = 0;
        uint32_t uiOffset = uiAddr % DEF_128KB_PAGE_LEN;         /* uiAddr offset in page */
        uint32_t uiLeftLen = DEF_128KB_PAGE_LEN - uiOffset;

        uiPageLen = ((uiLen >= uiLeftLen) ? uiLeftLen : uiLen);
        uiPageLen = uiLen >= (DEF_128KB_PAGE_LEN - (uiAddr % DEF_128KB_PAGE_LEN))
                    ? (DEF_128KB_PAGE_LEN - (uiAddr % DEF_128KB_PAGE_LEN))
                    : uiLen;

        return uiPageLen;
    }

    virtual uint32_t
    PageLenW(uint32_t uiLen, uint32_t uiAddr)
    {
        uint32_t uiPageLen = 0;
        uiPageLen = PageLenR(uiLen, uiAddr);

        return uiPageLen;
    }


protected:


private:
    bool AckReset();
    bool CheckAck();
    bool CheckSta();
    bool Ctl(uint8_t ucCtl);
};


