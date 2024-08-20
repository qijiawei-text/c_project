/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssMasterSpiDriver.h
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "WssProto.h"
#include "UartDriver.h"
#include "WssDriver.h"
#include "WssMasterSpiProto.h"
/*************************************************/

class CWssMasterSpiDriver : public CWssDriver
{
public:
#if 0
    CWssMasterSpiDriver(uint32_t uiUartIdx,
                        const std::shared_ptr<CUartDriver> &c_rstpPtr,
                        const std::shared_ptr<CResetBoard> &c_rstpColdResetBoard,
                        uint32_t uiColdResetIdx,
                        const std::shared_ptr<CResetBoard> &c_rstpWarmResetBoard,
                        uint32_t uiWarmResetIdx)
        : CWssDriver(uiUartIdx),
        m_stpUartDrvPtr(c_rstpPtr),
        m_stpColdResetBoard(c_rstpColdResetBoard),
        m_uiColdResetIdx(uiColdResetIdx),
        m_stpWarmResetBoard(c_rstpWarmResetBoard),
        m_uiWarmResetIdx(uiWarmResetIdx)
    {

    }
#endif
    CWssMasterSpiDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx,
                        const std::shared_ptr<CResetBoard> &c_rstpColdResetBoard,
                        uint32_t uiColdResetIdx,
                        const std::shared_ptr<CResetBoard> &c_rstpWarmResetBoard,
                        uint32_t uiWarmResetIdx,
                        const std::shared_ptr<CInterfaceDriver> &c_rstpPtr)
        : CWssDriver(uiHwIdx, uiHwSubIdx),
        m_stpColdResetBoard(c_rstpColdResetBoard),
        m_uiColdResetIdx(uiColdResetIdx),
        m_stpWarmResetBoard(c_rstpWarmResetBoard),
        m_uiWarmResetIdx(uiWarmResetIdx),
        m_stpDrvPtr(c_rstpPtr)
    {

    }


    ~CWssMasterSpiDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CWssStatusData &rstData) override;
    virtual bool GetCfgData(CWssAllCfg &rstData) override;

    virtual bool SetCfg(uint32_t uiInstBegin, uint32_t uiInstEnd, CWssCfg *pstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetOneChCfg(const CWssCfg &c_rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetAllChCfg(uint32_t uiChCnt, CWssCfg *pstData, uint32_t uiSubIdx = 0) override;

    virtual bool SetWavePlan(uint32_t uiChCnt, CWavePlan *pstWavePlan, uint32_t uiSubIdx = 0) override;

    virtual bool Reset(uint8_t ucResetType) override;
    virtual bool Upgrade(const char *pcFilePath) override;


private:

    std::shared_ptr<CResetBoard> m_stpColdResetBoard = nullptr;
    uint32_t m_uiColdResetIdx = 0;

    std::shared_ptr<CResetBoard> m_stpWarmResetBoard = nullptr;
    uint32_t m_uiWarmResetIdx = 0;

    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    uint32_t m_uiPortCnt = WSS_PORT_MAX_NUM;
    int __SendRecv(uint8_t *pucBufW, uint16_t usLenW,
                   uint8_t *pucBufR, uint16_t *pusLenR,
                   uint8_t ucFrmType, uint32_t uiTimeOut = 100);

    int __WssNisCmd(uint8_t *pucBufW, uint16_t usLenW,
                    uint8_t *pucBufR, uint16_t *pusLenR,
                    uint16_t usCmd, uint8_t uOid,
                    bool bTbl, uint8_t ucTbl,
                    bool bRow, uint8_t ucRow,
                    uint16_t usInst1, uint8_t ucPara,
                    bool bArray, uint16_t usInstN,
                    uint8_t ucFrmType, uint32_t uiTimeOut = 100);

    int __WssNisCmdMultiW(ST_WSS_NIS_MULTI *pstMulti, uint16_t usCnt);
    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiLen, uint32_t uiTimeOut = 50);
};






