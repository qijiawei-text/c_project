/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MultiUart.h
   Author:        Zhu Weian
   Date:          2020-12-02
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-12-02] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "UartDriver.h"
#include "UartSwitch.h"

#define MULTI_UART_MAX_NUM 7

class CMultiUart : public CUartDriver
{
public:

    CMultiUart(std::shared_ptr<CUartDriver> &c_rstpUartDrv, uint32_t uiUartSubIdx, std::shared_ptr<CUartSwitch > &c_rstpUartSwitch)
        : m_stpUartDrv(c_rstpUartDrv), m_uiUartSubIdx(uiUartSubIdx), m_stpUartSwitch(c_rstpUartSwitch)
    {

    }

    virtual
    ~CMultiUart()
    {
    }


    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                           uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos) override;


    virtual bool Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos) override;

    virtual bool Lock() override;

    virtual void Unlock() override;

    virtual bool Switch(uint32_t uiIdx) override;

    virtual bool ReadWriteUnlock(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                                 uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos) override;

    virtual bool WriteUnlock(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen) override;

private:

    virtual uint32_t
    GetUartMaxNum(void) override
    {
        return MULTI_UART_MAX_NUM;
    }

    std::mutex m_staMutex;

    std::shared_ptr<CUartDriver> m_stpUartDrv = nullptr;
    uint32_t m_uiUartSubIdx = 0;

    std::shared_ptr<CUartSwitch> m_stpUartSwitch = nullptr;
};

