/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      UartDriver.h
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
#include <unistd.h>
#include <mutex>
#include "BoardCmd.h"
#include "InterfaceDriver.h"
#include "RecursiveMutex.h"

/*************************************************/

#define UART_HW_FILE                             "/dev/ttyS"
#define UART_USB_FILE                            "/dev/ttyUSB"


enum EUartAttr
{
    UART_HW_MAX_NUM  = 4,
    UART_USB_MAX_NUM = 2,

    UART_BD_2400     = 2400,
    UART_BD_4800     = 4800,
    UART_BD_9600     = 9600,
    UART_BD_19200    = 19200,
    UART_BD_38400    = 38400,
    UART_BD_115200   = 115200,

    UART_DATA_BITS_7 = 7,
    UART_DATA_BITS_8 = 8,

    UART_STOP_BITS_1 = 1,
    UART_STOP_BITS_2 = 2,

    UART_PARITY_NO   = 0,
    UART_PARITY_ODD  = 1,
    UART_PARITY_EVEN = 2,

};

#define UART_SINGLE_READ 64

#define MACRO_UART_TCFLUSH(fd)          \
    do                                  \
    {                                   \
        tcflush((fd), TCIFLUSH);        \
        tcflush((fd), TCOFLUSH);        \
    } while (0)

#define MACRO_UART_TCDRAIN(fd)          \
    do                                  \
    {                                   \
        tcdrain((fd));                  \
    } while (0)

class CUartDriver : public CInterfaceDriver
{
public:
    CUartDriver()
    {

    }

    CUartDriver(uint32_t uiIdx) : m_uiIdx(uiIdx)
    {

    }

    CUartDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        m_uiBd(uiBd), m_uiDataBits(uiDataBits), m_uiStopBits(uiStopBits), m_uiParity(uiParity)
    {
    }
    virtual
    ~CUartDriver()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos) override;

    virtual bool Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen) override;
    virtual bool Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos) override;


    bool Close();

protected:
    uint32_t m_uiIdx = 0;
    int32_t m_iFd = -1;
    uint32_t m_uiBd = UART_BD_115200;
    uint32_t m_uiDataBits = UART_DATA_BITS_8;
    uint32_t m_uiStopBits = UART_STOP_BITS_1;
    uint32_t m_uiParity = UART_PARITY_NO;

    bool Open();
    bool SetCfg(void);

    bool WriteBytes(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen);
    bool ReadBytes(uint32_t uiIdx, std::shared_ptr<CRspHandler> stpEndPos);

private:
    virtual uint32_t GetUartMaxNum(void) = 0;
    virtual std::string
    GetFilePath()
    {
        return "";
    }
};

class CUartHwDriver : public CUartDriver
{
public:
    CUartHwDriver(uint32_t uiIdx) : CUartDriver(uiIdx)
    {
    }
    CUartHwDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    virtual
    ~CUartHwDriver()
    {
    }

private:

    virtual uint32_t
    GetUartMaxNum(void) override
    {
        return UART_HW_MAX_NUM;
    }

    virtual std::string
    GetFilePath() override
    {
        return UART_HW_FILE + std::to_string(m_uiIdx);
    }

protected:
    static std::array<CRecursiveMutex, UART_HW_MAX_NUM> ms_staMutexs;
    static std::vector<int32_t> ms_staFds;

};

class CUartUsbDriver : public CUartDriver
{
public:
    CUartUsbDriver(uint32_t uiIdx) : CUartDriver(uiIdx)
    {
    }
    CUartUsbDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    virtual
    ~CUartUsbDriver()
    {
    }

private:

    virtual uint32_t
    GetUartMaxNum(void) override
    {
        return UART_USB_MAX_NUM;
    }

    virtual std::string
    GetFilePath() override
    {
        return UART_USB_FILE + std::to_string(m_uiIdx);
    }

protected:
    static std::array<CRecursiveMutex, UART_USB_MAX_NUM> ms_staMutexs;
    static std::vector<int32_t> ms_staFds;

};

#if 0

struct CUartLogicMap
{
    uint32_t uiUartCpuIdx;
    uint32_t uiLogicIdx;
    uint32_t uiLogicSel;
};

class CUartLogicDriver : public CUartHwDriver
{
public:
    CUartLogicDriver()
    {
    }
    CUartLogicDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartHwDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    virtual
    ~CUartLogicDriver()
    {
    }

    /*virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                                                            uint8_t *pucDataR, uint32_t uiLen, uint32_t uiTimeOut = 20) override;*/

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                           uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos, uint32_t uiTimeOut = 100, bool bEnd = true) override;


    virtual bool ReadWriteMutil(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                                uint8_t *pucDataR, uint32_t uiLenR, bool bEnd = true) override;


    virtual bool Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen, bool bEnd = true) override;
    virtual bool Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos, uint32_t uiTimeOut = 100, bool bEnd = true) override;
    virtual bool ReadMulti(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen, bool bEnd = true) override;


private:

    virtual std::shared_ptr<CUartSwitch> &GetDrvPtr(void) = 0;
    virtual const CUartLogicMap &GetMap(uint32_t uiIdx) = 0;
};



class CUartLogicUsbDriver : public CUartUsbDriver
{
public:
    CUartLogicUsbDriver()
    {
    }
    CUartLogicUsbDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartUsbDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    virtual
    ~CUartLogicUsbDriver()
    {
    }

    virtual bool ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                           uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpEndPos, uint32_t uiTimeOut = 100, bool bEnd = true) override;

    virtual bool ReadWriteMutil(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                                uint8_t *pucDataR, uint32_t uiLenR, bool bEnd = true) override;


    virtual bool Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen, bool bEnd = true) override;
    virtual bool Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpEndPos, uint32_t uiTimeOut = 100, bool bEnd = true) override;
    virtual bool ReadMulti(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen, bool bEnd = true) override;

private:

    virtual std::shared_ptr<CUartSwitch> &GetDrvPtr(void) = 0;
    virtual const CUartLogicMap &GetMap(uint32_t uiIdx) = 0;

};


class CUartFpgaDriver : public CUartLogicDriver
{
public:
    CUartFpgaDriver()
    {
    }
    CUartFpgaDriver(std::shared_ptr<CUartSwitch > &c_rstpUartSwitchPtr, uint32_t uiUartIdx)
    {
        if (uiUartIdx >= LOGIC_UART_CNT) {
            return;
        }

        /* uint32_t uiFpgaIdx = mc_staMap.at(uiUartIdx).uiLogicIdx; */

        /* TBD: Logic Driver */
        m_stpUartSwitchPtr = c_rstpUartSwitchPtr;

    }
    CUartFpgaDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartLogicDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    ~CUartFpgaDriver()
    {
    }

private:
    std::shared_ptr<CUartSwitch> m_stpUartSwitchPtr = nullptr;


    /* TODO */
    const std::array<CUartLogicMap, LOGIC_UART_CNT> mc_staMap =
    {
        {{UART3, 0, LOGIC_UART0}, /* PA */
         {UART3, 0, LOGIC_UART1}, /* BA */
         {UART3, 0, LOGIC_UART2}, /* WSS1*/
         {UART3, 0, LOGIC_UART3}, /* OCM */
         {UART3, 0, LOGIC_UART4}, /* EXTEND */
         {UART3, 0, LOGIC_UART5}, /* OTDR */
         {UART3, 0, LOGIC_UART6}} /* WSS2 */
    };

    virtual std::shared_ptr<CUartSwitch> &
    GetDrvPtr(void) override
    {
        return m_stpUartSwitchPtr;
    }

    virtual const CUartLogicMap &
    GetMap(uint32_t uiIdx) override
    {
        return mc_staMap.at((uiIdx >= LOGIC_UART_CNT) ? 0 : uiIdx);
    }

};

class CUartFpgaUsbDriver : public CUartLogicUsbDriver
{
public:
    CUartFpgaUsbDriver()
    {
    }
    CUartFpgaUsbDriver(std::shared_ptr<CUartSwitch > &c_rstpUartSwitchPtr, uint32_t uiUartIdx)
    {
        if (uiUartIdx >= LOGIC_UART_CNT) {
            return;
        }

        /* uint32_t uiFpgaIdx = mc_staMap.at(uiUartIdx).uiLogicIdx; */

        /* TBD: Logic Driver */
        m_stpUartSwitchPtr = c_rstpUartSwitchPtr;

    }
    CUartFpgaUsbDriver(uint32_t uiBd, uint32_t uiDataBits, uint32_t uiStopBits, uint32_t uiParity) :
        CUartLogicUsbDriver(uiBd, uiDataBits, uiStopBits, uiParity)
    {
    }
    ~CUartFpgaUsbDriver()
    {
    }

private:
    std::shared_ptr<CUartSwitch> m_stpUartSwitchPtr = nullptr;


    /* TODO */
    const std::array<CUartLogicMap, LOGIC_UART_CNT> mc_staMap =
    {
        {{UART0, 0, LOGIC_UART0}, /* PA */
         {UART1, 0, LOGIC_UART1}, /* BA */
         {UART0, 0, LOGIC_UART2}, /* WSS1*/
         {UART1, 0, LOGIC_UART3}, /* OCM */
         {UART0, 0, LOGIC_UART4}, /* EXTEND */
         {UART1, 0, LOGIC_UART5}, /* OTDR */
         {UART0, 0, LOGIC_UART6}} /* WSS2 */
    };

    virtual std::shared_ptr<CUartSwitch> &
    GetDrvPtr(void) override
    {
        return m_stpUartSwitchPtr;
    }

    virtual const CUartLogicMap &
    GetMap(uint32_t uiIdx) override
    {
        return mc_staMap.at((uiIdx >= LOGIC_UART_CNT) ? 0 : uiIdx);
    }

};


#endif







