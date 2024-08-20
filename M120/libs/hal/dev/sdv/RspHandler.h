/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      RspHandler.h
   Author:        Zhu Weian
   Date:          2020-11-16
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-11-16] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "OtdrProto.h"
#include "EndToken.h"
#include "Proto.h"

#define RSP_LEN_DEF    (1024)
#define RSP_TIMEOUT_DEF     (50)
#define RSP_RETRY_TIMES_DEF (50)

class CRspHandler
{
public:
    CRspHandler()
    {
        m_staRsp.resize(RSP_LEN_DEF);
    }

    /* oa status */
    CRspHandler(uint32_t uiLen) : m_iLen(uiLen)
    {
        m_staRsp.resize(uiLen);
    }

    ~CRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        int32_t iRet = RSP_STATUS_AGAIN;

        if (m_stpEndToken && m_stpEndToken->Find((uint8_t *)m_staRsp.data(), m_uiPos)) {
            iRet = RSP_STATUS_OK;
        } else if ( m_iLen <= 0) {
            iRet = RSP_STATUS_OK;
        }

        /* Check RSP if RSP_STATUS_OK  */
        if (RSP_STATUS_OK == iRet && m_stpProto) {
            if (!m_stpProto->ProcRsp(m_staRsp, m_bInProgress)) {
                if (m_bInProgress) {
                    LOG_WRITE(EN_LOG_DEBUG, "In Progress...");
                    Reset();
                }
                iRet = RSP_STATUS_ERROR;
            } else {
                iRet = RSP_STATUS_OK;
            }
        }

        return iRet;

    }

    void
    SetEndToken(const std::shared_ptr<CEndToken> &c_rstpEndToken)
    {
        m_stpEndToken = c_rstpEndToken;
    }

    void
    SetProto(const std::shared_ptr<CProto> &c_rstpProto)
    {
        m_stpProto = c_rstpProto;
    }

    int32_t
    Len()
    {
        return m_iLen;
    }

    uint32_t
    Pos()
    {
        return m_uiPos;
    }

    uint32_t
    TimeOut()
    {
        return m_uiTimeOut;
    }

    void
    SetTimeOut(uint32_t uiTimeOut)
    {
        m_uiTimeOut = uiTimeOut;
    }

    void
    Update(uint32_t uiLen)
    {
        m_iLen -= uiLen;
        m_uiPos += uiLen;
    }

    bool
    InProgress()
    {
        return m_bInProgress;
    }

    void
    Copy(std::string &rstaRsp)
    {
        rstaRsp = m_staRsp;
    }

    virtual uint8_t *
    Data()
    {
        return (uint8_t *)m_staRsp.data();
    }

    uint32_t
    RetryTimes()
    {
        return m_uiRetryTimes;
    }

    void
    SetRetryTimes(uint32_t uiRetryTimes)
    {
        m_uiRetryTimes = uiRetryTimes;
    }


    void
    Reset()
    {
        m_iLen = m_staRsp.size();
        m_uiPos = 0;
    }


protected:

    int32_t m_iLen = RSP_LEN_DEF;
    uint32_t m_uiPos = 0;

    bool m_bInProgress = false;
    uint32_t m_uiRetryTimes = RSP_RETRY_TIMES_DEF;
    uint32_t m_uiTimeOut = RSP_TIMEOUT_DEF;

    std::shared_ptr<CEndToken> m_stpEndToken = nullptr;
    std::shared_ptr<CProto> m_stpProto = nullptr;

    std::string m_staRsp;

    void
    __Resize(uint32_t uiLen)
    {
        m_iLen = uiLen;
        m_uiPos = 0;
    }
};



