/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Phy.h
   Author:        Zhu Weian
   Date:          2021-01-21
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "MdioDriver.h"
#include "ResetBoard.h"
#include "BoardCmd.h"
class CPhy
{
public:
    CPhy(std::shared_ptr<CMdio> &c_rstpMdio,
         uint32_t uiResetHwIdx,
         uint32_t uiResetHwSubIdx,
         const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpMdio(c_rstpMdio),
        m_uiResetHwIdx(uiResetHwIdx),
        m_uiResetHwSubIdx(uiResetHwSubIdx),
        m_stpLogicDrv(c_rstpLogicDrv)
    {
        m_stpResetBoard = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }

/*    CPhy(std::shared_ptr<CMdio> &c_rstpMdio,
         //uint32_t uiResetHwIdx,
         //uint32_t uiResetHwSubIdx,
         const std::shared_ptr<CResetBoard> &c_rstpResetBoard,
         const std::shared_ptr<CLogicDriver> &c_rstpLogicDrv) : m_stpMdio(c_rstpMdio),
        //m_uiResetHwIdx(uiResetHwIdx),
        //m_uiResetHwSubIdx(uiResetHwSubIdx),
        m_stpResetBoard(c_rstpResetBoard),
        m_stpLogicDrv(c_rstpLogicDrv)
    {
        // m_stpResetBoard = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }
 */
    ~CPhy()
    {

    }

    bool
    Config()
    {

        /* OSC 88e1322 4 ports cross connect */
        if (!m_stpMdio->Write(8, 0x16, 0x04)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 8 0x16 0x04 error.");
            return false;
        }
        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 8 0x16 0x04.");

        if (!m_stpMdio->Write(8, 0x1b, 0x7e03)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 8 0x1b 0x7e03 error.");
            return false;
        }
        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 8 0x1b 0x7e03.");

        /* OSC1 88e1322 led control */
        if (!m_stpMdio->Write(9, 0x16, 0x03)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 9 0x16 0x03 error.");
            return false;
        }
        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 9 0x16 0x03.");

        if (!m_stpMdio->Write(9, 0x10, 0x0)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 9 0x10 0x0 error.");
            return false;
        }

        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 9 0x10 0x0.");

        /* 8E1322 port0 SGMII */
        if (!m_stpMdio->Write(0x08, 0x16, 0x01)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 0x08 0x16 0x01 error.");
            return false;
        }

        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 0x08 0x16 0x01.");

        if (!m_stpMdio->Write(0x08, 0x1a, 0x47)) {
            LOG_WRITE(EN_LOG_ERROR, "Mdio Write 0x08 0x1a 0x47 error.");
            return false;
        }

        LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 0x08 0x1a 0x47.");

        LOG_WRITE(EN_LOG_NOTICE, "Config Phy Done!");

        return true;
    }

    bool
    Reset()
    {
        LOG_WRITE(EN_LOG_NOTICE, "Reset +++");

        if (nullptr == m_stpResetBoard) {
            LOG_WRITE(EN_LOG_ERROR, "nullptr == m_stpLogicPtr");
            return false;
        }


        if (false == m_stpResetBoard->Reset(m_uiResetHwSubIdx)) {
            LOG_WRITE(EN_LOG_ERROR, "Reset phy error. m_uiDevIdx = %d", m_uiResetHwSubIdx);
            return false;
        }


        LOG_WRITE(EN_LOG_NOTICE, "Reset ---");

        return true;
    }

    bool Init();

private:
    std::shared_ptr<CMdio> m_stpMdio = nullptr;
    uint32_t m_uiResetHwIdx = 0;
    uint32_t m_uiResetHwSubIdx = 0;
    std::shared_ptr<CLogicDriver> m_stpLogicDrv = nullptr;
    std::shared_ptr<CResetBoard> m_stpResetBoard = nullptr;

};

