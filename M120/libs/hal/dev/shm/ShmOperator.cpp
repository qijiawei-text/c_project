/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ShmOperator.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "ShmOperator.h"
#include "Cmd.h"
#include "Delay.h"

/*==============================function=========================*/

bool
CShmOperator::Get(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem,
                  uint8_t *pucBuf, uint32_t uiPos, uint32_t uiLen)
{

    if ((uiPos + uiLen > c_rstShm.GetSize()) || (nullptr == pucBuf) ||
        ((void *)(-1) == c_rstShm.GetAddr())) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    memcpy(pucBuf, (uint8_t *)c_rstShm.GetAddr() + uiPos, uiLen);

    if (!c_rstSem.V()) {
        return false;
    }

    return true;

}

bool
CShmOperator::Set(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem,
                  uint8_t *pucBuf, uint32_t uiPos, uint32_t uiLen)
{

    if ((uiPos + uiLen > c_rstShm.GetSize()) || (nullptr == pucBuf) ||
        ((void *)(-1) == c_rstShm.GetAddr())) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    memcpy((uint8_t *)c_rstShm.GetAddr() + uiPos, pucBuf, uiLen);

    if (!c_rstSem.V()) {
        return false;
    }

    return true;

}

bool
CShmOperator::SetSyncCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNodeReq,
                         CQueueNode *pstQNodeRsp, uint32_t uiPrd, uint32_t uiTimeout)
{

    if (((void *)(-1) == c_rstShm.GetAddr()) || (0 == uiPrd) || (0 == uiTimeout)) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    /* put the cmd node into the queue */
    CCmdQueue *pstShmQ = (CCmdQueue *)c_rstShm.GetAddr();

    if (!CQueueOperator::AddNode(rstQNodeReq, *pstShmQ)) {
        c_rstSem.V();
        return false;
    }

    if (!c_rstSem.V()) {
        return false;
    }

    /* check the excute status */
    uint32_t i;
    for (i = 0; i < uiTimeout; i++) {
        uint32_t uiCmdSta;

        if (!CShmOperator::Get(c_rstShm, c_rstSem, (uint8_t *)&uiCmdSta,
                               CShmOperator::GetCmdApiStaPos(rstQNodeReq.ucIdxInQ), sizeof(uiCmdSta))) {
            return false;
        }

        if ((ERR_CMD_INPROGRESS == uiCmdSta) || (ERR_CMD_DELAY == uiCmdSta)) {
            CDelay::Delay(uiPrd, MS);
        } else if (OPLK_OK == uiCmdSta) {
            break;
        } else {
            return false;
        }
    }

    if (i >= uiTimeout) {
        return false;
    }

    /* response process */
    if (pstQNodeRsp) {
        if (!CShmOperator::Get(c_rstShm, c_rstSem, (uint8_t *)pstQNodeRsp,
                               CShmOperator::GetCmdRespPos(), sizeof(CQueueNode))) {
            return false;
        }
    }

    return true;

}

bool
CShmOperator::SetAsyncCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNodeReq)
{

    if ((void *)(-1) == c_rstShm.GetAddr()) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    /* put the cmd node into the queue */
    CCmdQueue *pstShmQ = (CCmdQueue *)c_rstShm.GetAddr();

    if (!CQueueOperator::AddNode(rstQNodeReq, *pstShmQ)) {
        c_rstSem.V();
        return false;
    }

    if (!c_rstSem.V()) {
        return false;
    }

    return true;

}

bool
CShmOperator::GetCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNode)
{

    if ((void *)(-1) == c_rstShm.GetAddr()) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    /* get the cmd node from shm queue */
    CCmdQueue *pstShmQ = (CCmdQueue *)c_rstShm.GetAddr();

    if (!CQueueOperator::GetNode(rstQNode, *pstShmQ)) {
        c_rstSem.V();
        return false;
    }

    if (!c_rstSem.V()) {
        return false;
    }

    return true;

}

bool
CShmOperator::GetFastCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNode)
{

    if ((void *)(-1) == c_rstShm.GetAddr()) {
        return false;
    }

    if (!c_rstSem.P()) {
        return false;
    }

    /* get the cmd node from shm queue */
    CCmdQueue *pstShmQ = (CCmdQueue *)c_rstShm.GetAddr();

    if (!CQueueOperator::GetFastNode(rstQNode, *pstShmQ)) {
        c_rstSem.V();
        return false;
    }

    if (!c_rstSem.V()) {
        return false;
    }

    return true;

}


