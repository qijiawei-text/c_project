#pragma once
#include "FanDriver.h"
#include "VDriver.h"

class CFanVirtual : public CFan, public CVirtualDriver
{
public:
    CFanVirtual(void);
    virtual
    ~CFanVirtual();

    virtual bool IsOnline(void);
    virtual bool IsPlug(void);
    virtual bool ClearPlug(void);

    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetSpeed(uint32_t &uiSpeed);
    virtual bool SetSpeed(uint32_t uiSpeed);
    virtual bool SetFanInitFanController();
    virtual bool SetFanControllerEEP();
    virtual bool GetLed(uint32_t &uiLed);
    virtual bool SetLed(uint32_t uiLed);
    virtual bool GetResetCause(uint8_t &ucResetCause);
    virtual bool SetResetCause(uint8_t ucResetCause);
};