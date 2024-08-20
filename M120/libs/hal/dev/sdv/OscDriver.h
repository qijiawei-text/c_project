/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OscDriver.h
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "OscBoard.h"
#include "BoardCmd.h"
#include "ResetBoard.h"

/*************************************************/

class COscDriver /*: public CDevDriver*/
{
public:

    COscDriver(uint32_t uiDevIdx) : m_uiDevIdx(uiDevIdx)
    {
    }

    virtual
    ~COscDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(COscStatusData &rstData) = 0;
    virtual bool SetLaserEnable(bool bEnable) = 0;

protected:

    uint32_t m_uiDevIdx;

};

class CLogicOscDriver : public COscDriver
{
public:
    CLogicOscDriver(uint32_t uiDevIdx,
                    std::shared_ptr<COscBoard> c_rstpOscBoardPtr)
        : COscDriver(uiDevIdx),
        m_stpOscBoardPtr(c_rstpOscBoardPtr)
    {
    }
    ~CLogicOscDriver()
    {

    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COscStatusData &rstData) override;
    virtual bool SetLaserEnable(bool bEnable) override;


private:
    std::shared_ptr<COscBoard> m_stpOscBoardPtr = nullptr;
    bool m_bLastRxLos = false;
    bool m_bLastLinkdown = false;
};



class CI2cOscDriver : public COscDriver
{
public:
    CI2cOscDriver(uint32_t uiI2cSubIdx) : COscDriver(uiI2cSubIdx)
    {

    }
    ~CI2cOscDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(COscStatusData &rstData) override;
    virtual bool SetLaserEnable(bool bEnable) override;

private:



};

