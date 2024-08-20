/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCmd.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DevCmd.h"

/*==============================function=========================*/

CDevCmdProcesser &
CDevCmdProcesser::GetInstance(void)
{

    static CDevCmdProcesser s_stDevCmdProcesser;

    return s_stDevCmdProcesser;

}


bool
CDevCmdProcesser::CmdProc(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, const CQueueNode &c_rstNode)
{

    uint32_t uiCmdSta;
    bool bRet = true;

    /* find cmd by oid from cmd list */
    auto itIter = mc_staCmdList.find(c_rstNode.uiOid);
    if (mc_staCmdList.end() == itIter) {
        uiCmdSta = ERR_OPERATION_FAILED;
        bRet = false;
        goto LABEL_END;
    }

    if (nullptr == itIter->second) {
        uiCmdSta = ERR_OPERATION_FAILED;
        bRet = false;
        goto LABEL_END;
    }

    /* excute cmd */
    if ((itIter->second)(c_rstNode.uiLen, c_rstNode.aucBuf)) {
        uiCmdSta = OPLK_OK;
    } else {
        /* Asynchronous processing */
        if (ASYNC == c_rstNode.iSyncFlag) {
            uiCmdSta = OPLK_OK;
        } else {
            uiCmdSta = ERR_OPERATION_FAILED;
            bRet = false;
        }
    }

LABEL_END:
    CShmOperator::Set(c_rstShm, c_rstSem, (uint8_t *)&uiCmdSta,
                      CShmOperator::GetCmdApiStaPos(c_rstNode.ucIdxInQ), sizeof(uiCmdSta));
    return bRet;

}




