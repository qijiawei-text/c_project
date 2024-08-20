/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Logic.h
   Author:        Zhu Weian
   Date:          2020-10-19
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-06] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "Jtag.h"
#include "Flash.h"

/* #define RESET_REG 0x2014 */

class CLogic
{
public:

    CLogic(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpLogicDrv(c_rstpLogicDrv)
    {
    }

    ~CLogic()
    {
    }

    virtual bool GetVersion(char *pcVersion) = 0;
    virtual bool Upgrade(const char *c_pcFileName) = 0;
    virtual bool Reset() = 0;


protected:
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;

};


class CCpld : public CLogic
{
public:

    CCpld(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv, const std::shared_ptr<CJtag> &c_rstpJtag)
        : CLogic(c_rstpLogicDrv), m_stpJtag(c_rstpJtag)
    {
    }
    CCpld(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv, const std::shared_ptr<CJtag> &c_rstpJtag, uint32_t uiHwSubIdx)
        : CLogic(c_rstpLogicDrv), m_stpJtag(c_rstpJtag), m_uiHwSubIdx(uiHwSubIdx)
    {
    }

    ~CCpld()
    {
    }

    virtual bool GetVersion(char *pcVersion) override;
    virtual bool Upgrade(const char *c_pcFilePath) override;
    virtual bool Reset() override;
    uint32_t
    getHwSubIdx()
    {
        return m_uiHwSubIdx;
    }
private:

    std::shared_ptr<CJtag> m_stpJtag = nullptr;
    uint32_t m_uiHwIdx;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiBaseAddr;


};

class CFpga : public CLogic
{
public:

    CFpga(const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv,
          const std::shared_ptr<CFlash> &c_rstpFlash,
          const std::shared_ptr<CLogicDriver> &c_rstpResetLogicDrv)
        : CLogic(c_rstpLogicDrv), m_stpFlash(c_rstpFlash), m_stpResetLogicDrv(c_rstpResetLogicDrv)
    {
    }

    ~CFpga()
    {
    }

    virtual bool GetVersion(char *pcVersion) override;
    virtual bool Upgrade(const char *c_pcFilePath) override;
    virtual bool Reset() override;

private:

    bool IsPrimary();
    bool PreUpgrade();
    bool PostUpgrade();
    bool WriteImage(uint8_t *pucBuf, uint32_t ulLen);

    bool m_bNewFpga = false;
    bool m_bIsPrimary = true;
    uint8_t m_ucPrimaryOffset = 0;
    uint8_t m_ucBackupOffset = 0;


    uint8_t m_ucHeadLen = 0;
    uint8_t m_aucHead[128] = {0};
    uint32_t m_uiPrimaryAddr = 0x00;
    uint32_t m_uiBackupAddr = 0x00;

    std::shared_ptr<CFlash> m_stpFlash = nullptr;
    std::shared_ptr<CLogicDriver> m_stpResetLogicDrv = nullptr;


};

