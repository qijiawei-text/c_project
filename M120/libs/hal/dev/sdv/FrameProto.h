/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MolexSpiProto.h
   Author:        Zhu Weian
   Date:          2020-12-02
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-12-02] [1.0] [Creator]


*****************************************************************************************************/
#pragma once

#include "DevInternal.h"
#include "Proto.h"

#define SPI_CRC16_FEED                                                  (0xFFFFu)

#define DEF_FRM_CRC_LEN                                                         (2)

#define DEF_STATUS_OK                                                   (0)
#define DEF_STATUS_IN_PROGRESS                                          (9)

typedef enum {
    MODULE_TPYE_OCM = 0,
    MODULE_TPYE_OA,
    MODULE_TPYE_CNT,
}MODULE_TYPE_INDEX_EN;


typedef struct {

    uint8_t ucPort;
    uint8_t ucCmd;

} __attribute__((packed)) ST_FRM_REQ_HEAD;

typedef struct {

    uint8_t ucStatus;

} __attribute__((packed)) ST_FRM_RSP_HEAD;


typedef struct {
    uint16_t usTid;
    uint16_t usLen;
} __attribute__((packed)) ST_FRM_TID_HEAD;


typedef struct {
    ST_FRM_TID_HEAD stTidHead;
    ST_FRM_REQ_HEAD stFrmHead;

} __attribute__((packed)) ST_FRM_WRITE_HEAD;

typedef struct {
    ST_FRM_TID_HEAD stTidHead;
    ST_FRM_RSP_HEAD stFrmRspHead;
} __attribute__((packed)) ST_FRM_READ_HEAD;

class CFrameProto
{
public:
    static void Pack(uint16_t usOid, std::string &rstaBody, std::string &rstaMsg);
    static bool UnPack(std::string &rstaMsg, bool &rbInProgress);
    static void Pack(uint16_t usTid, uint8_t ucPort, uint16_t usOid, std::string &rstaBody, std::string &rstaMsg, int iType);
    static bool UnPack(std::string &rstaMsg, bool &rbInProgress, int iType);
private:

    static uint16_t __CheckSum(uint16_t usSeed, uint8_t *pucSource, uint32_t uiLen);
    static int32_t Util_CRC16Calculate(uint32_t ulFeed, uint8_t *pucSource, uint32_t uSize, uint16_t *pusCrc);
    static uint16_t UTIL_Crc16IBMCalcWithFeed(uint32_t ulFeed, uint8_t *pucBuf, uint32_t ulLen);

};

