#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "BoardCmd.h"
#include "OcsProto.h"
#include "VDriver.h"
#include "OtdrDriver.h"
#define OTDR_FILE_PATH ("/var/vevice/MolexOtdrConfig.json")
#define OTDR_P_FILE_PATH ("/var/vevice/MolexOtdr-pConfig.json")

/*************************************************/
class COtdrVirtual : public COtdrDriver, public CVirtualDriver {
public:
    COtdrVirtual(uint32_t m_uiHwSubIdx)
        : COtdrDriver(), CVirtualDriver("OTDR", determineConfigFilename(m_uiHwSubIdx)), configFilename(determineConfigFilename(m_uiHwSubIdx))
    {
    }
    virtual
    ~COtdrVirtual();
    virtual bool IsOnline(void);
    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetStatusData(COtdrStatusData &rstData);
    virtual bool GetCfg(COtdrCfgData &rstCfgData);
    virtual bool GetEvent(COtdrStatusData &rstData);
    virtual bool GetSor();

    virtual bool StartScan(void);
    virtual bool SetPara(uint32_t ulDistRange, uint32_t ulPulseWidth, double dSamplingResolution);
    virtual bool SetUserIOR(double dUserIOR);
    virtual bool SetMeasuringTime(uint32_t ulMeasuringTime);
    virtual bool SetThr(double dRefThr, double dSpliceThr, double dFiberEndThr);
    virtual bool SetScanMode(uint32_t ulScanMode);
    virtual bool GetTemp(COtdrTemp &rstTemp);

    virtual bool Reset(uint8_t ucResetType);
    virtual bool Upgrade(const char *pcFilePath);

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen);
    virtual bool SetStartOffset(COtdrStartOffset *pstStartOffset);
    virtual bool GetStartOffset(COtdrStartOffset &rstStartOffset);

private:
    std::string configFilename;
    static std::string
    determineConfigFilename(uint32_t m_uiHwSubIdx)
    {
        return (m_uiHwSubIdx == 0) ? OTDR_FILE_PATH : OTDR_P_FILE_PATH;
    }
};