/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Queue.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Queue.h"
#include "Cmd.h"

/*==============================function=========================*/

bool
CQueueOperator::AddNode(CQueueNode &rstNode, CCmdQueue &rstQueue)
{

    if (rstNode.uiLen > QUEUE_NODE_BUF_LEN) {
        return false;
    }

    uint32_t uiNodeIdx = rstQueue.stQueue.ucInIdx;
    CQueueNode &rstNodeInQ = rstQueue.stQueue.astQNode[uiNodeIdx];

    rstNode.ucIdxInQ = uiNodeIdx;
    memcpy(&rstNodeInQ, &rstNode, sizeof(CQueueNode));

    rstQueue.astCmdStatus[uiNodeIdx].uiCmdStatus = ERR_CMD_DELAY;

    rstQueue.stQueue.ucInIdx++;
    if (rstQueue.stQueue.ucInIdx >= QUEUE_NODE_NUM_MAX) {
        rstQueue.stQueue.ucInIdx = 0;
    }

    return true;

}

bool
CQueueOperator::GetNode(CQueueNode &rstNode, CCmdQueue &rstQueue)
{
    uint32_t uiNodeIdx = rstQueue.stQueue.ucOutIdx;

    while (uiNodeIdx < rstQueue.stQueue.ucInIdx) {
        if (QUEUE_PRIO_COMMON == rstQueue.stQueue.astQNode[uiNodeIdx].uiPri) {
            break;
        }

        if (uiNodeIdx >= rstQueue.stQueue.ucFastOutIdx) {
            if (++rstQueue.stQueue.ucFastOutIdx >= QUEUE_NODE_NUM_MAX) {
                rstQueue.stQueue.ucFastOutIdx = 0;
            }
            break;
        }

        if (++rstQueue.stQueue.ucOutIdx >= QUEUE_NODE_NUM_MAX) {
            rstQueue.stQueue.ucOutIdx = 0;
        }

        uiNodeIdx = rstQueue.stQueue.ucOutIdx;
    }

    if (uiNodeIdx == rstQueue.stQueue.ucInIdx) {
        return false;
    }

    CQueueNode &rstNodeInQ = rstQueue.stQueue.astQNode[uiNodeIdx];

    rstNodeInQ.ucIdxInQ = uiNodeIdx;
    memcpy(&rstNode, &rstNodeInQ, sizeof(CQueueNode));

    rstQueue.stQueue.ucOutIdx++;
    if (rstQueue.stQueue.ucOutIdx >= QUEUE_NODE_NUM_MAX) {
        rstQueue.stQueue.ucOutIdx = 0;
    }

    return true;

}

bool
CQueueOperator::DeleteNode(CCmdQueue &rstQueue)
{

    if (rstQueue.stQueue.ucOutIdx == rstQueue.stQueue.ucInIdx) {
        return false;
    }

    if (rstQueue.stQueue.ucInIdx == 0) {
        rstQueue.stQueue.ucInIdx = QUEUE_NODE_NUM_MAX - 1;
    } else {
        rstQueue.stQueue.ucInIdx--;
    }

    return true;

}

bool
CQueueOperator::GetFastNode(CQueueNode &rstNode, CCmdQueue &rstQueue)
{
    uint32_t uiNodeIdx = rstQueue.stQueue.ucFastOutIdx;

    while (uiNodeIdx < rstQueue.stQueue.ucInIdx) {
        if (QUEUE_PRIO_HIGH == rstQueue.stQueue.astQNode[uiNodeIdx].uiPri) {
            break;
        }

        if (++rstQueue.stQueue.ucFastOutIdx >= QUEUE_NODE_NUM_MAX) {
            rstQueue.stQueue.ucFastOutIdx = 0;
        }

        uiNodeIdx = rstQueue.stQueue.ucFastOutIdx;
    }

    if (uiNodeIdx == rstQueue.stQueue.ucInIdx) {
        return false;
    }

    CQueueNode &rstNodeInQ = rstQueue.stQueue.astQNode[uiNodeIdx];
    rstNodeInQ.ucIdxInQ = uiNodeIdx;
    memcpy(&rstNode, &rstNodeInQ, sizeof(CQueueNode));

    if (++rstQueue.stQueue.ucFastOutIdx >= QUEUE_NODE_NUM_MAX) {
        rstQueue.stQueue.ucFastOutIdx = 0;
    }

    return true;
}