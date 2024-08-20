#pragma once
#include "PsuDriver.h"
#include "VDriver.h"

class CPsuVirtual : public CPsu, public CVirtualDriver
{
public:
    CPsuVirtual(void);
    virtual
    ~CPsuVirtual();

    virtual bool IsOnline(void);
    virtual bool IsOk(void);
    virtual bool IsPlug(void);
    virtual bool ClearPlug(void);

    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetVol(uint16_t &rusInV, uint16_t &rusOutV);
    virtual bool GetCurrent(uint32_t &ruiInCurrent, uint32_t &ruiOutCurrent);
    virtual bool GetWatts(uint32_t &ruiWatts);
    virtual bool GetTemp(int32_t &riTemp);
};