/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      LogicDriver.h
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
#include <mutex>
#include "DevDriver.h"
#include "LclBusDriver.h"
#include "I2cDriver.h"
#include "PcieBusDriver.h"

/*************************************************/

enum BIT_DEF
{
    BIT_0 = 0,
    BIT_1,
    BIT_2,
    BIT_3,
    BIT_4,
    BIT_5,
    BIT_6,
    BIT_7,
};


enum ELogicMem
{
    LOGIC_MEM_TYPE_RAM = 0,
    LOGIC_MEM_TYPE_FIFO,
};


class CLogicDriver /*: public CDevDriver*/
{
public:
    CLogicDriver(uint32_t uiHwIdx) : m_uiHwIdx(uiHwIdx)
    {
    }
    CLogicDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) :
        m_uiHwIdx(uiHwIdx), m_uiHwSubIdx(uiHwSubIdx)
    {
    }
    CLogicDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiBaseAddr) :
        m_uiHwIdx(uiHwIdx), m_uiHwSubIdx(uiHwSubIdx), m_uiBaseAddr(uiBaseAddr)
    {
    }
    virtual
    ~CLogicDriver()
    {
    }

    bool Write(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen, uint32_t uiType = LOGIC_MEM_TYPE_RAM);
    bool Read(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen, uint32_t uiType = LOGIC_MEM_TYPE_RAM);

    bool WriteBit(uint32_t uiReg, uint8_t ucBitOffset, uint8_t ucVal);
    bool ReadBit(uint32_t uiReg, uint8_t ucBitOffset, uint8_t &rucVal);


    virtual bool WriteReg(uint32_t uiReg, uint8_t ucData) = 0;
    virtual bool ReadReg(uint32_t uiReg, uint8_t *pucData) = 0;
    virtual bool WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) = 0;
    virtual bool ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) = 0;
    virtual bool WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) = 0;
    virtual bool ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) = 0;
    virtual bool close() = 0;
    virtual bool reopen() = 0;
protected:
    uint32_t m_uiHwIdx;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiBaseAddr;
    virtual std::mutex &GetMutex(void) = 0;

};

enum EDataWidth
{
    DATA_WIDTH_1B = 1,
    DATA_WIDTH_2B = 2,
    DATA_WIDTH_4B = 4,
};


class CLclBusLogic : public CLogicDriver

{
public:
    CLclBusLogic(uint32_t uiLclBusIdx, uint32_t uiCsIdx, uint32_t uiBaseAddr, uint32_t uiDataWidth) :
        CLogicDriver(uiLclBusIdx, uiCsIdx, uiBaseAddr), m_uiDataWidth(uiDataWidth)
    {
        m_stLclBusDrv.CreateVirtualAddr(uiCsIdx);
    }
    virtual
    ~CLclBusLogic()
    {
    }

    virtual bool WriteReg(uint32_t uiReg, uint8_t ucData) override;
    virtual bool ReadReg(uint32_t uiReg, uint8_t *pucData) override;
    virtual bool WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool
    close()
    {
        return false;
    }
    virtual bool
    reopen()
    {
        return false;
    }
private:

    static std::mutex ms_stMutex;

    virtual std::mutex &
    GetMutex(void) override
    {
        return ms_stMutex;
    }

    CLclBusDriver m_stLclBusDrv;
    uint32_t m_uiDataWidth;

};


class CI2cLogic : public CLogicDriver
{
public:


    CI2cLogic(uint32_t uiHwIdx, uint32_t uiHwSubIdx, const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiDevAddr) :
        CLogicDriver(uiHwIdx, uiHwSubIdx), m_stpI2cPtr(c_rstpI2cPtr), m_uiDevAddr(uiDevAddr)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
    }
    virtual
    ~CI2cLogic()
    {
    }


    virtual bool WriteReg(uint32_t uiReg, uint8_t ucData) override;
    virtual bool ReadReg(uint32_t uiReg, uint8_t *pucData) override;
    virtual bool WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;

    virtual bool
    WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
    {
        return false;
    }
    virtual bool
    ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen)
    {
        return false;
    }
    virtual bool
    close()
    {
        return false;
    }
    virtual bool
    reopen()
    {
        return false;
    }

private:

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiDevAddr;

    static std::mutex ms_stMutex;

    virtual std::mutex &
    GetMutex(void) override
    {
        return ms_stMutex;
    }

};


class CPcieBusLogic : public CLogicDriver

{
public:
    CPcieBusLogic(uint32_t uiPcieBusIdx, uint32_t uiCsIdx, uint32_t uiBaseAddr, uint32_t uiDataWidth) :
        CLogicDriver(uiPcieBusIdx, uiCsIdx, uiBaseAddr), m_uiDataWidth(uiDataWidth)
    {
        m_stPcieBusDrv.CreateVirtualAddr(uiCsIdx);
    }
    virtual
    ~CPcieBusLogic()
    {
        m_stPcieBusDrv.DestroyVirtualAddr(0);
    }

    virtual bool WriteReg(uint32_t uiReg, uint8_t ucData) override;
    virtual bool ReadReg(uint32_t uiReg, uint8_t *pucData) override;
    virtual bool WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool
    close()
    {
        m_stPcieBusDrv.DestroyVirtualAddr(0);
        return true;
    }
    virtual bool
    reopen()
    {
        m_stPcieBusDrv.CreateVirtualAddr(0);
        return true;
    }
private:

    static std::mutex ms_stMutex;

    virtual std::mutex &
    GetMutex(void) override
    {
        return ms_stMutex;
    }

    CPcieBusDriver m_stPcieBusDrv;
    uint32_t m_uiDataWidth;

};

class CPcieBusLogicBitmap : public CLogicDriver
{
public:
    CPcieBusLogicBitmap(uint32_t uiPcieBusIdx, uint32_t uiCsIdx, uint32_t uiBaseAddr, uint32_t uiDataWidth) :
        CLogicDriver(uiPcieBusIdx, uiCsIdx, uiBaseAddr), m_uiDataWidth(uiDataWidth)
    {
        m_stPcieBusDrv.CreateVirtualAddr(uiCsIdx);
    }
    virtual
    ~CPcieBusLogicBitmap()
    {
        m_stPcieBusDrv.DestroyVirtualAddr(0);
    }

    virtual bool WriteReg(uint32_t uiReg, uint8_t ucData) override;
    virtual bool ReadReg(uint32_t uiReg, uint8_t *pucData) override;
    virtual bool WriteFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadFifo(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool WriteRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool ReadRam(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool
    close()
    {
        m_stPcieBusDrv.DestroyVirtualAddr(0);
        return true;
    }
    virtual bool
    reopen()
    {
        m_stPcieBusDrv.CreateVirtualAddr(0);
        return true;
    }
private:

    static std::mutex ms_stMutex;

    virtual std::mutex &
    GetMutex(void) override
    {
        return ms_stMutex;
    }

    CPcieBusDriver m_stPcieBusDrv;
    uint32_t m_uiDataWidth;

};







