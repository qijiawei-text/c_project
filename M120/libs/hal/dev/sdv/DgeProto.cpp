/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DgeProto.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DgeProto.h"

/*==============================function=========================*/
uint8_t
CDgeSwFrameProto::GenerateChecksum(uint8_t *pucData, uint32_t uiLen)
{

    uint8_t ucThisByte = 0;

    for (uint32_t i = 0; i < uiLen; i++) {
        ucThisByte += *pucData++;
    }

    ucThisByte = 0x00 - ucThisByte;

    return ucThisByte;

}

bool
CDgeSwFrameProto::ChkChecksum(uint8_t *pucData, uint32_t uiLen)
{

    uint8_t ucSum = 0;

    for (uint32_t i = 0; i < uiLen; i++) {
        ucSum += *pucData++;
    }

    return (ucSum == 0x00) ? true : false;

}

bool
CDgeSwFrameProto::ProcRsp(uint8_t *pucData)
{

    /* check checksum */
    uint32_t uiLen = ((uint32_t)pucData[DGE_RSP_DATA_LEN_IDX] << 8) | pucData[DGE_RSP_DATA_LEN_IDX + 1];

    if (!ChkChecksum(pucData, uiLen + DGE_RSP_EXCEPT_DATA_LENGTH)) {
        return false;
    }

    /* check status */
    if (pucData[DGE_RSP_STATUS_IDX] != DGE_STATUS_OK) {
        return false;
    }

    return true;

}

bool
CDgeSwFrameProto::SetCurve(std::string &rstaMsg, uint32_t uiPointCnt, CDgePoint *pstPoint)
{

    if ((uiPointCnt < DGE_FRM_MIN_POINT_NUM) || (uiPointCnt > DGE_FRM_MAX_POINT_NUM)) {
        return false;
    }

    /* get data length */
    uint32_t uiDataLen = uiPointCnt * 4;

    rstaMsg.resize(uiDataLen + DGE_FRM_EXCEPT_DATA_LENGTH);

    uint8_t *pucData = (uint8_t *)rstaMsg.data();

    /* frame */
    *pucData++ = DGE_FRM_OID_SET_CURVE;
    *pucData++ = (uint8_t)(uiDataLen >> 8);
    *pucData++ = (uint8_t)uiDataLen;

    uint32_t uiFreq;
    for (uint32_t i = 0; i < uiPointCnt; i++, pstPoint++) {
        uiFreq = pstPoint->uiFreq - DGE_FRM_FREQ_BASE;
        *pucData++ = (uint8_t)(uiFreq >> 8);
        *pucData++ = (uint8_t)uiFreq;
        *pucData++ = (uint8_t)(pstPoint->uiAtten >> 8);
        *pucData++ = (uint8_t)(pstPoint->uiAtten);
    }

    *pucData = GenerateChecksum((uint8_t *)rstaMsg.data(), uiDataLen + DGE_FRM_EXCEPT_DATA_LENGTH - 1);

    return true;

}


