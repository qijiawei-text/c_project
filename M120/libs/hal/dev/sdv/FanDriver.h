/*****************************************************************************************************
   FileName:      FanDriver.h
   Date:          2019-07-22
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-22] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "EEprom.h"
#include "DevData.h"
#include "FanBoard.h"
#include "Max31760Driver.h"

class CFan /*: public CDevDriver*/
{
public:

    CFan(const std::shared_ptr<CFanBoard> &c_rstpFanBoardPtr, uint32_t uiFanBoardIdx, const std::shared_ptr<CEEprom> &c_rstpEEpromPtr) :
        m_stpFanBoardPtr(c_rstpFanBoardPtr),
        m_uiFanBoardIdx(uiFanBoardIdx),
        m_stpEEpromPtr(c_rstpEEpromPtr)
    {
    }

    CFan(const std::shared_ptr<CFanBoard> &c_rstpFanBoardPtr, uint32_t uiFanBoardIdx, const std::shared_ptr<CEEprom> &c_rstpEEpromPtr, const std::shared_ptr<CMax31760> &c_rstpMax31760Ptr) :
        m_stpFanBoardPtr(c_rstpFanBoardPtr),
        m_uiFanBoardIdx(uiFanBoardIdx),
        m_stpEEpromPtr(c_rstpEEpromPtr),
        m_stpMax31760Ptr(c_rstpMax31760Ptr)
    {
    }

    virtual
    ~CFan()
    {
    }

    virtual bool IsOnline(void);
    virtual bool IsPlug(void);
    virtual bool ClearPlug(void);

    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool SetMfg(const char *c_pcFileName);
    virtual bool GetStatusData(CFanSta &rstData);

    virtual bool GetSpeed(uint32_t &uiSpeed);
    virtual bool SetSpeed(uint32_t uiSpeed);

    virtual bool GetResetCause(uint8_t &ucResetCause);
    virtual bool SetResetCause(uint8_t ucResetCause);

    virtual bool SetFanInitFanController();
    virtual bool SetFanControllerEEP();
#if 0
    virtual bool GetLed(uint32_t &uiLed);
    virtual bool SetLed(uint32_t uiLed);
#endif
private:
    std::shared_ptr<CFanBoard> m_stpFanBoardPtr = nullptr;
    uint32_t m_uiFanBoardIdx;

    std::shared_ptr<CEEprom> m_stpEEpromPtr = nullptr;
    std::shared_ptr<CMax31760> m_stpMax31760Ptr = nullptr;
};

