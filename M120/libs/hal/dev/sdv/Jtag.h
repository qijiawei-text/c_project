/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Jtag.h
   Author:        Zhu Weian
   Date:          2020-10-09
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-10-09] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "LogicDriver.h"

class CJtag
{
public:

    CJtag(std::shared_ptr<CLogicDriver> &c_stpLogicDrv) : m_stpLogicDrv(c_stpLogicDrv)
    {
    }
    CJtag(std::shared_ptr<CLogicDriver> &c_stpLogicDrv, uint32_t uiBaseAddr) : m_stpLogicDrv(c_stpLogicDrv), m_uiBaseAddr(uiBaseAddr)
    {
    }
    CJtag(std::shared_ptr<CLogicDriver> &c_stpLogicDrv, uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiBaseAddr) : m_stpLogicDrv(c_stpLogicDrv), m_uiHwIdx(uiHwIdx), m_uiHwSubIdx(uiHwSubIdx), m_uiBaseAddr(uiBaseAddr)
    {
    }

    ~CJtag()
    {
    }

    bool Upgrade(uint8_t *pucImg, uint32_t uiLen);
    bool Reset();

    bool WritePin(uint8_t ucPin, uint8_t ucValue);
    bool ReadTDO(uint8_t &ucValue);
    bool GetImg(uint8_t *pucData);


    uint32_t m_uiIdx;
    std::shared_ptr<CLogicDriver> m_stpLogicDrv;

private:
    uint8_t *ms_pucImg;
    uint32_t ms_uiImgLen;
    uint32_t ms_uiImgPos;
    uint32_t m_uiHwIdx;
    uint32_t m_uiHwSubIdx;
    uint32_t m_uiBaseAddr;


};

