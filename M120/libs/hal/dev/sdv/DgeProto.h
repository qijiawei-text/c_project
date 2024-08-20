/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeProto.h
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
#include "DevData.h"

/*************************************************/

/* frame format: object ID, data length, data, checksum */
/* total byte:      1           2        ?       1 */

/* response:      status, data length, data, checksum */
/* total byte:      1           2        ?       1 */

enum EDgeFrameProto
{
    DGE_FRM_EXCEPT_DATA_LENGTH = 4,

    DGE_FRM_OID_SET_CURVE      = 0x15,

    DGE_FRM_MIN_POINT_NUM      = 2,
    DGE_FRM_MAX_POINT_NUM      = 4800,
    DGE_FRM_FREQ_BASE          = 180000,

    DGE_RSP_STATUS_IDX         = 0,
    DGE_RSP_DATA_LEN_IDX       = 1,
    DGE_RSP_DATA_IDX           = 3,
    DGE_RSP_EXCEPT_DATA_LENGTH = 4,

    DGE_STATUS_OK              = 0x00,
    DGE_STATUS_CHK_ERR         = 0x01,
    DGE_STATUS_SYNTAX_ERR      = 0x02,
    DGE_STATUS_DATA_INVALID    = 0x04,
};

class CDgeSwProto
{
public:
    CDgeSwProto()
    {
    }
    virtual
    ~CDgeSwProto()
    {
    }

    virtual bool SetCurve(std::string &rstaMsg, uint32_t uiPointCnt, CDgePoint *pstPoint) = 0;

    virtual bool ProcRsp(uint8_t *pucData) = 0;
};

class CDgeSwFrameProto : public CDgeSwProto
{
public:
    virtual bool SetCurve(std::string &rstaMsg, uint32_t uiPointCnt, CDgePoint *pstPoint) override;

    virtual bool ProcRsp(uint8_t *pucData) override;

private:
    uint8_t GenerateChecksum(uint8_t *pucData, uint32_t uiLen);
    bool ChkChecksum(uint8_t *pucData, uint32_t uiLen);
};










