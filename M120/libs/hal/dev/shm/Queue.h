/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Queue.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"

/*************************************************/

#define QUEUE_NODE_NUM_MAX              (128)
#define QUEUE_NODE_BUF_LEN              (8 * 1024)

enum ESyncFlag
{
    SYNC = 0,
    ASYNC
};

enum EQueuePriority
{
    QUEUE_PRIO_COMMON = 0,
    QUEUE_PRIO_HIGH
};

struct CQueueNode
{
    uint32_t uiOid;
    uint32_t uiPri;
    int32_t iSyncFlag;
    uint8_t ucIdxInQ;

    uint32_t uiLen;
    uint8_t aucBuf[QUEUE_NODE_BUF_LEN];
};

struct CQueue
{
    uint8_t ucInIdx;     /* the position where the cmd should be put */
    uint8_t ucOutIdx;    /* the position where the cmd should be read out */
    uint8_t ucFastOutIdx;
    CQueueNode astQNode[QUEUE_NODE_NUM_MAX];
};

struct CCmdQStatus
{
    uint32_t uiCmdOID;
    uint32_t uiCmdCnt;
    uint32_t uiCmdStatus;
    uint32_t uiErrCode;
};

struct CCmdQueue
{
    CCmdQStatus astCmdStatus[QUEUE_NODE_NUM_MAX];
    CQueue stQueue;
    CQueueNode stResp;
};

class CQueueOperator
{
public:
    static bool AddNode(CQueueNode &rstNode, CCmdQueue &rstQueue);
    static bool GetNode(CQueueNode &rstNode, CCmdQueue &rstQueue);
    static bool DeleteNode(CCmdQueue &rstQueue);
    static bool GetFastNode(CQueueNode &rstNode, CCmdQueue &rstQueue);
};






