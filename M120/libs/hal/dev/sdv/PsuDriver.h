/*****************************************************************************************************
   FileName:      PowerDriver.h
   Date:          2019-07-22
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-22] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "EEprom.h"
#include "DevData.h"
#include "PsuBoard.h"

class CPsu /*: public CDevDriver*/
{
public:
    CPsu(const std::shared_ptr<CI2c> &c_rstpI2cPtr, uint32_t uiI2cIdx, uint32_t uiDevAddr, uint32_t uiDevEeAddr,
         const std::shared_ptr<CPsuBoard> &c_rstpPsuBoardPtr, uint32_t uiPsuBoardIdx) :
        m_stpI2cPtr(c_rstpI2cPtr),
        m_uiI2cIdx(uiI2cIdx),
        m_uiDevAddr(uiDevAddr),
        m_uiDevEeAddr(uiDevEeAddr),
        m_stpPsuBoardPtr(c_rstpPsuBoardPtr),
        m_uiPsuBoardIdx(uiPsuBoardIdx)
    {
        /* m_stpI2cPtr = CBoardCmd::GetI2cPtr(uiHwIdx); */
        /* m_stpBoardLogicPtr = CBoardCmd::GetLogicDevPtr(uiLogicIdx); */

    }

    virtual
    ~CPsu()
    {
    }

    virtual bool IsOnline(void);
    virtual bool IsOk(void);
    virtual bool IsPlug(void);
    virtual bool ClearPlug(void);

    virtual bool GetMfg(CMfg &rstMfg);
    virtual bool GetStatusData(CPowerSta &rstData);
    virtual bool GetVol(uint16_t &rusInV, uint16_t &rusOutV);
    virtual bool GetCurrent(uint32_t &ruiInCurrent, uint32_t &ruiOutCurrent);
    virtual bool GetWatts(uint32_t &ruiWatts);
    virtual bool GetTemp(int32_t &riTemp);

    virtual bool GetMfgItem(uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen);

private:

    std::shared_ptr<CI2c> m_stpI2cPtr = nullptr;
    uint32_t m_uiI2cIdx;
    uint32_t m_uiDevAddr;
    uint32_t m_uiDevEeAddr;

    std::shared_ptr<CPsuBoard> m_stpPsuBoardPtr = nullptr;
    uint32_t m_uiPsuBoardIdx;

    uint16_t CalcX(uint16_t usVal, uint16_t usRatio);

};


