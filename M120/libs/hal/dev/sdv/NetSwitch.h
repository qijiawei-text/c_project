/*****************************************************************************************************
   FileName:      NetSwitch.h
   Date:          2019-07-22
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2019-07-22] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevDriver.h"
#include "MdioDriver.h"
#include "ResetBoard.h"

class CNetSwitch : public CDevDriver
{
public:

    CNetSwitch(const std::shared_ptr<CMdio> &c_rstpDrvPtr, const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) :
        m_stpDrvPtr(c_rstpDrvPtr), m_stpLogicDrv(c_rstpLogicDrv)
    {

    }

    virtual
    ~CNetSwitch()
    {
    }

    bool Init();
    bool Reset();
    bool ReadReg(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusValue);
    bool WriteReg(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usValue);


    bool ReadSMIC45PhyReg(uint32_t ulDevAddr, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusValue);
    bool WriteSMIC45PhyReg(uint32_t ulDevAddr, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usValue);
    bool Init88e1512(uint32_t ulIndex);
private:
    std::shared_ptr<CMdio> m_stpDrvPtr = nullptr;
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    uint32_t m_uiResetIdx;

    bool ReadSMIC22PhyReg(uint32_t ulDevAddr, uint32_t ulRegAddr, uint16_t *pusValue);
    bool WriteSMIC22PhyReg(uint32_t ulDevAddr, uint32_t ulRegAddr, uint16_t usValue);
    bool WriteSMIC22PhyPage(uint32_t ulPortNum, uint32_t ulPageNum, uint32_t ulRegAddr, uint16_t usValue);
    bool ReadSMIC22PhyPage(uint32_t ulPortNum, uint32_t ulPageNum, uint32_t ulRegAddr, uint16_t *pusValue);

    bool CfgNoCpuMode();
};


