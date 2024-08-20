#pragma once
#include "TempDriver.h"
#include "VDriver.h"

class CThermalVirtual : public CTemp, public CVirtualDriver
{
public:
    CThermalVirtual(void);
    virtual
    ~CThermalVirtual();

    virtual bool GetTemp(int32_t &riTemp);
};