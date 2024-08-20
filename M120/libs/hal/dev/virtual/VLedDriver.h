#pragma once
#include "LedDriver.h"
#include "VDriver.h"

class CLedVirtual : public CLed, public CVirtualDriver
{
public:
    CLedVirtual(void);
    virtual
    ~CLedVirtual();

    virtual bool SetState(uint32_t uiState);
    virtual bool GetState(uint32_t &ruiState);
    virtual bool SetTestMode(bool bTestMode);
    virtual bool SetLocationTestMode(bool bTestMode);
};