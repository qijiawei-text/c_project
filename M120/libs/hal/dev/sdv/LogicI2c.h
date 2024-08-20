/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicI2c.h
   Author:        Zhu Weian
   Date:          2020-08-05
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-08-05] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "I2cDriver.h"
#include "LogicDriver.h"


class CLogicI2c : public CI2c
{
public:

    CLogicI2c(const std::shared_ptr<CLogicDriver> &c_rstpLogicPtr, uint32_t uiBaseAddr)
        : m_stpLogicPtr(c_rstpLogicPtr), m_uiBaseAddr(uiBaseAddr)
    {
    }


    virtual
    ~CLogicI2c()
    {
    }

    virtual bool Write(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW) override;
    virtual bool Read(uint32_t uiIdx, uint32_t uiAddr, uint8_t *pucBufW, uint32_t uiLenW, uint8_t *pucBufR, uint32_t uiLenR) override;

    virtual uint32_t
    PageLenR(uint32_t uiLen, uint32_t uiAddr)
    {
        uint32_t uiPageLen = 0;
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

private:

    std::shared_ptr<CLogicDriver> m_stpLogicPtr;
    uint32_t m_uiBaseAddr;
    /* static std::array<std::mutex, I2C_FPGA_MAX_NUM> ms_staMutexs; */

    static std::mutex ms_stMutex;

    bool __AckReset();
    bool __CheckAck();
    bool __CheckSta();
    bool __Ctl(uint8_t ucCtl);

};


