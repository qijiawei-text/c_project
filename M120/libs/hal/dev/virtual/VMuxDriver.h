#pragma once
#include "MuxDriver.h"
#include "VDriver.h"

class CMuxVirtual : public CMux, public CVirtualDriver
{
public:
    CMuxVirtual(void);
    virtual
    ~CMuxVirtual();

    virtual bool IsOnline(void);
    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetStatusData(CMuxStatusData &rstData);
    virtual bool GetCali(CMuxCaliData &rstData);
};