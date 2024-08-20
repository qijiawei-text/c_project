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

#include "RspHandler.h"

class COtdrRspHandler : public CRspHandler
{
public:

    COtdrRspHandler(const std::shared_ptr<COtdrProto> &c_rstpProto) : m_stpProto(c_rstpProto)
    {
    }

    COtdrRspHandler(uint32_t uiLen, const std::shared_ptr<COtdrProto> &c_rstpProto)
        : CRspHandler(uiLen), m_stpProto(c_rstpProto)
    {
    }

    ~COtdrRspHandler()
    {
    }

protected:
    std::shared_ptr<COtdrProto> m_stpProto = nullptr;
};


class COtdrMfgRspHandler : public COtdrRspHandler
{

public:

    COtdrMfgRspHandler(uint32_t uiLen, const std::shared_ptr<COtdrProto> &c_rstpProto, CMfg *pstMfg)
        : COtdrRspHandler(uiLen, c_rstpProto), m_pstMfg(pstMfg)
    {
    }

    ~COtdrMfgRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        if (!m_stpProto->GetMfgRsp(m_staRsp, *m_pstMfg)) {
            return RSP_STATUS_AGAIN;
        }

        return RSP_STATUS_OK;
    }

private:

    CMfg *m_pstMfg = nullptr;
};

class COtdrEventTopRspHandler : public COtdrRspHandler
{

public:

    COtdrEventTopRspHandler(const std::shared_ptr<COtdrProto> &c_rstpProto, COtdrEventTop *pstEventTop)
        : COtdrRspHandler(c_rstpProto), m_pstEventTop(pstEventTop)
    {
    }

    ~COtdrEventTopRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        if (!m_stpProto->GetEventTopRsp(m_staRsp, *m_pstEventTop)) {
            return RSP_STATUS_AGAIN;
        }

        return RSP_STATUS_OK;
    }

private:
    COtdrEventTop *m_pstEventTop = nullptr;
};


class COtdrEventLineRspHandler : public COtdrRspHandler
{

public:

    COtdrEventLineRspHandler(const std::shared_ptr<COtdrProto> &c_rstpProto, COtdrEventLine *pstEventLine)
        : COtdrRspHandler(c_rstpProto), m_pstEventLine(pstEventLine)
    {
    }

    ~COtdrEventLineRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        if (!m_stpProto->GetEventLineRsp(m_staRsp, *m_pstEventLine)) {
            return RSP_STATUS_AGAIN;
        }

        return RSP_STATUS_OK;
    }

private:
    COtdrEventLine *m_pstEventLine = nullptr;
};


class COtdrSorRspHandler : public COtdrRspHandler
{

#define STATUS_HEADER (0)
#define STATUS_BODY   (1)

public:
    COtdrSorRspHandler(uint32_t uiLen, const std::shared_ptr<COtdrProto> &c_rstpProto, std::string stpFilePath)
        : COtdrRspHandler(uiLen, c_rstpProto), m_stpFilePath(stpFilePath)
    {

    }

    ~COtdrSorRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        m_bInProgress = false;

        if (STATUS_HEADER == m_uiState) {        /* header handle */
            if (m_uiPos >= sizeof(m_uiSorSize)) {
                m_stpProto->GetSorHeaderRsp(m_staRsp, m_uiSorSize);

                std::size_t errFound = m_staRsp.find(">ERR");
                if (errFound != std::string::npos) {
                    LOG_WRITE(EN_LOG_ERROR, "Sor Rsp: %s", m_staRsp.c_str());
                    return RSP_STATUS_ERROR;
                }

                __Resize(m_uiSorSize);
                m_staSorFile.resize(m_uiSorSize);
                m_uiState = STATUS_BODY;

                m_bInProgress = true;
                return RSP_STATUS_ERROR;
            }
        }

        if (m_iLen <= 0 && STATUS_BODY == m_uiState) {

            LOG_WRITE(EN_LOG_NOTICE, "m_iLen <= 0");
            if (!CFile::Write(OTDR_SOR_FILE, (uint8_t *)m_staSorFile.data(), m_staSorFile.size())) {
                LOG_WRITE(EN_LOG_ERROR, "write file error. len %lu", m_staSorFile.size());
                return RSP_STATUS_ERROR;
            }

            return RSP_STATUS_OK;
        }

        return RSP_STATUS_AGAIN;
    }

    virtual uint8_t *
    Data()
    {
        if (STATUS_HEADER == m_uiState) {
            return (uint8_t *)m_staRsp.data();
        }
        LOG_WRITE(EN_LOG_NOTICE, "*Data");
        return (uint8_t *)m_staSorFile.data();
        /* return m_staSorFile.size(); */
    }

private:
    uint32_t m_uiState = 0;
    std::string m_stpFilePath;
    uint32_t m_uiSorSize = 0;
    uint8_t *m_pucBuf = nullptr;
    std::string m_staSorFile;

};

class COtdrStateRspHandler : public COtdrRspHandler
{

public:

    COtdrStateRspHandler(const std::shared_ptr<COtdrProto> &c_rstpProto, bool *pbState)
        : COtdrRspHandler(c_rstpProto), m_pbState(pbState)
    {
    }

    ~COtdrStateRspHandler()
    {
    }

    virtual int32_t
    Proc()
    {
        if (!m_stpProto->GetStateRsp(m_staRsp, *m_pbState)) {
            return RSP_STATUS_AGAIN;
        }

        return RSP_STATUS_OK;
    }

private:
    bool *m_pbState = nullptr;
};

