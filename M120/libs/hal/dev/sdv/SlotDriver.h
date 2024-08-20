/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      SlotDriver.h
   Author:        Zhen Zhang
   Date:          2019-02-20
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2019-02-20] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "DevData.h"
/* #include "BoardCmd.h" */
#include "TempDriver.h"
#include "EEprom.h"
#include "LedDriver.h"
#include "Phy.h"
/* #include "DevThread.h" */

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <errno.h>
#include <sys/socket.h>




/*************************************************/

#define SLOT_MAX_LED_NUM 2

class CSlotDriver : public CDevDriver
{
public:
    CSlotDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CSlotDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }

    virtual
    ~CSlotDriver()
    {
    }

    virtual bool
    SetOnline(bool bOnline)
    {
        return true;
    }
    virtual bool
    SetPlug(bool bPlug)
    {
        return true;
    }
    virtual bool IsOnline(void) = 0;
    virtual bool IsPlug(void) = 0;
    virtual bool ClearPlug(void) = 0;
    virtual bool GetCardType(uint32_t &ruiType) = 0;

    virtual void RegisterSubDev(uint32_t uiDevType, std::vector<CRegisterInfo> &stRegisterInfo);
    virtual void UnregisterSubDevs();

/* mfg */
    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool SetMfg(const char *c_pcFilePath);

/* cali */
    virtual bool GetCali(CCali &rstCali);
    virtual bool SetCali(const char *c_pcFileName);

/* status */
    virtual bool GetStatusData(CSlotStatusData &rstData);

/* temperature */
    virtual bool GetTemp(int32_t &riTemp);

/* led */
    virtual bool GetLedState(uint32_t uiLedIdx, uint32_t &uiState);
    virtual bool SetLedState(uint32_t uiLedIdx, uint32_t uiState);
    virtual bool GetAllLedState(CLedSta *pastLedsta);


/* eeprom */
    virtual bool EepromRead(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);
    virtual bool EepromWrite(uint32_t uiAddr, uint8_t *pucData, uint32_t uiLen);

    virtual bool
    Reset(uint32_t uiResetType)
    {
        return true;
    };

    uint64_t m_ullCnt = 0;

protected:

    std::shared_ptr<CTemp> m_stpTempDrvPtr = nullptr;
    std::shared_ptr<CEEprom> m_stpEEpromPtr = nullptr;
    std::vector<std::shared_ptr<CLed> > m_staLedPtrs;
    std::shared_ptr<CPhy> m_stpPhy = nullptr;

    bool bEEpromAlarm = false;

};


#define UEVENT_BUFFER_SIZE 1024 * 4

class CUsbSlotDriver : public CSlotDriver
{
public:

    CUsbSlotDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CSlotDriver(uiHwIdx, uiHwSubIdx)
    {
        __Init();
    }

    /* CUsbSlotDriver()
       {
       }*/

    ~CUsbSlotDriver()
    {
    }

    virtual bool
    SetOnline(bool bOnline)
    {
        if (m_bOnline != bOnline) {
            m_bOnline = bOnline;
            m_uiCardType = bOnline ? CARD_TYPE_MUX : CARD_TYPE_UNKOWN;
            m_bPlug = true;
        }

        return true;
    }


    virtual bool
    SetPlug(bool bPlug)
    {
        m_bPlug = bPlug;
        return true;
    }

    virtual bool IsOnline(void) override;

    virtual bool IsPlug(void) override;

    virtual bool ClearPlug(void) override;

    virtual bool
    GetCardType(uint32_t &ruiType)
    {
        ruiType = m_uiCardType;
        return true;
    }

    /* mfg */
    virtual bool
    GetMfg(CMfg &rstMfg)
    {
        return true;
    }
    virtual bool
    SetMfg(const char *c_pcFilePath)
    {
        return true;
    }

/* cali */
    virtual bool
    GetCali(CCali &rstCali)
    {
        return true;
    }
    virtual bool
    SetCali(const char *c_pcFileName)
    {
        return true;
    }

/* temperature */
    virtual bool
    GetTemp(int32_t &riTemp)
    {
        return true;
    }

/* led */
    virtual bool
    GetLedState(uint32_t uiLedIdx, uint32_t &uiState)
    {
        return true;
    }
    virtual bool
    SetLedState(uint32_t uiLedIdx, uint32_t uiState)
    {
        return true;
    }
    virtual bool
    GetAllLedState(CLedSta *pastLedsta)
    {
        return true;
    }


private:

    bool m_bOnline = false;
    bool m_bPlug = false;
    uint32_t m_uiCardType = CARD_TYPE_UNKOWN;
    int32_t m_iSocket = -1;
    CSpiFt4222Driver m_stSpiFt;

    void __Init();
};


class CFixedSlotDriver : public CSlotDriver
{
public:

    CFixedSlotDriver(uint32_t uiHwIdx) : CSlotDriver(uiHwIdx)
    {
    }
    ~CFixedSlotDriver()
    {
    }

    virtual bool
    IsOnline(void)
    {
        return true;
    }
    virtual bool
    IsPlug(void)
    {
        return false;
    }

    virtual bool
    ClearPlug(void)
    {
        return false;
    }

    virtual bool
    GetCardType(uint32_t &ruiType)
    {
        ruiType = CARD_TYPE_FIX;
        return true;
    }
};

class CFixedOpsSlotDriver : public CSlotDriver
{
public:

    CFixedOpsSlotDriver(uint32_t uiHwIdx) : CSlotDriver(uiHwIdx)
    {
    }
    ~CFixedOpsSlotDriver()
    {
    }

    virtual bool
    IsOnline(void)
    {
        return true;
    }
    virtual bool
    IsPlug(void)
    {
        return false;
    }

    virtual bool
    ClearPlug(void)
    {
        return false;
    }

    virtual bool
    GetCardType(uint32_t &ruiType)
    {
        ruiType = CARD_TYPE_OPS;
        return true;
    }
};


class CFixedAmpSlotDriver : public CSlotDriver
{
public:

    CFixedAmpSlotDriver(uint32_t uiHwIdx) : CSlotDriver(uiHwIdx)
    {
    }
    ~CFixedAmpSlotDriver()
    {
    }

    virtual bool
    IsOnline(void)
    {
        return true;
    }
    virtual bool
    IsPlug(void)
    {
        return false;
    }

    virtual bool
    ClearPlug(void)
    {
        return false;
    }

    virtual bool
    GetCardType(uint32_t &ruiType)
    {
        ruiType = CARD_TYPE_OTA;
        return true;
    }
};


class CLogicSlotDriver : public CSlotDriver
{
public:

    CLogicSlotDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CSlotDriver(uiHwIdx, uiHwSubIdx)
    {
        LOG_WRITE(EN_LOG_NOTICE, "CLogicSlotDriver HwIdx = %d", uiHwIdx);
        m_stpSlotBoardPtr = CBoardCmd::GetSlotBoardPtr(uiHwIdx);
    }
    ~CLogicSlotDriver()
    {
    }

    virtual bool IsOnline(void) override;
    virtual bool IsPlug(void) override;
    virtual bool ClearPlug(void) override;

    virtual bool GetCardType(uint32_t &ruiType) override;
    virtual bool Reset(uint32_t uiResetType) override;

private:
    std::shared_ptr<CSlotBoard> m_stpSlotBoardPtr = nullptr;

    /* static const std::vector<CSlotReg> msc_staRegs; */



};








