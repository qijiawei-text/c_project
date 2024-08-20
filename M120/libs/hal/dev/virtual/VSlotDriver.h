#pragma once
#include "SlotDriver.h"
#include "VDriver.h"

class CSlotVirtual : public CSlotDriver, public CVirtualDriver
{
public:
    CSlotVirtual(uint32_t uiHwIdx, uint32_t uiHwSubIdx);
    virtual
    ~CSlotVirtual();

    virtual bool IsOnline(void);
    virtual bool IsPlug(void);
    virtual bool ClearPlug(void);
    virtual bool GetCardType(uint32_t &ruiType);

    virtual bool GetTemp(int32_t &riTemp);
    virtual bool GetMfg(CMfg &rstMfg);

private:
    uint32_t m_uiCardType;
};