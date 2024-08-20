/*****************************************************************************************************
   FileName:      Flash.h
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
#include "SpiDriver.h"
#define CLK_BUF_REG (0x1014)

class CFlash : public CDevDriver
{
public:

    CFlash(uint32_t ulChip, const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr) :
        m_ulChip(ulChip), m_stpDrvPtr(c_rstpDrvPtr)
    {
        m_ulId = GetChipId();
    }

    CFlash(uint32_t ulChip, const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) :
        m_ulChip(ulChip), m_stpDrvPtr(c_rstpDrvPtr), m_stpLogicDrv(c_rstpLogicDrv)
    {
        int32_t lId = 0;
        m_stpLogicDrv->WriteBit(CLK_BUF_REG, BIT_4, 1);
        for (uint32_t i = 0; i < 3; i++) {
            usleep(100 * 1000);
            lId = GetChipId();
            if (OPLK_ERR != lId) {
                m_ulId = lId;
                break;
            }
        }
    }

    virtual
    ~CFlash()
    {
    }

    bool WriteData(uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen);
    bool ReadData(uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen);

    bool WriteImage(uint8_t *pucBuf, uint32_t ulLen);
    bool Erase();

private:
    uint32_t m_ulChip;
    int32_t m_ulId;
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;

    int32_t GetChipId();
    bool WriteEnable();
    bool CheckStatus();
    bool CheckTime(uint32_t ulTimes);
    int32_t GetLoadBit(uint8_t *pucBuf);
    bool Protect(uint32_t ulBit);
    bool UnProtect();
    bool EraseBy64K(uint32_t ulSectorAddr);
    bool WriteOnePage(uint32_t ulPageAddr, uint8_t *pucBuf, uint32_t ulLen);
    bool ReadOnePage(uint32_t ulPageAddr, uint8_t *pucBuf, uint32_t ulLen);
    bool IsPrimary();
    bool WriteHead(uint32_t ulAddr1, uint32_t ulAddr2);


};


