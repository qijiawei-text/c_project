/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssDriver.h
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "DevDriver.h"
#include "WssProto.h"
#include "UartDriver.h"

/*************************************************/

enum EN_WSS_NIS_CMD {
    /* Single command                                                           */
    EN_WSS_NIS_CMD_WR = 1,
    EN_WSS_NIS_CMD_RD,

    EN_WSS_NIS_CMD_UPGD_ABORT,
    EN_WSS_NIS_CMD_UPGD_VERIFY,
    EN_WSS_NIS_CMD_UPGD_SWITCH,
    EN_WSS_NIS_CMD_UPGD_COMMIT,
    EN_WSS_NIS_CMD_UPGD_START,

    /* Array command                                                            */
    EN_WSS_NIS_CMD_ARRAY_WR = 16,
    EN_WSS_NIS_CMD_ARRAY_RD,

    /* Multi command                                                            */
    EN_WSS_NIS_CMD_MULTI_WR,
    EN_WSS_NIS_CMD_MULTI_RD,

    /* Optional additional parameters                                           */
    EN_WSS_NIS_CMD_ADD_TABLE   = 0x0020,
    EN_WSS_NIS_CMD_ADD_ROW     = 0x0040,
    EN_WSS_NIS_CMD_ADD_ALTCONF = 0x0100,

};

enum EN_WSS_NIS_OID {
    /* General Module Status and Feature Queries                                */
    EN_WSS_NIS_OID_MOUDLE_INFO  = 6,                                        /* Module Info                                             */
    EN_WSS_NIS_OID_DEFAULT      = 8,                                        /* Restore Factory Defaults & Reload                       */
    EN_WSS_NIS_OID_SW_RESET     = 10,                                       /* Reload Module Software                                  */
    EN_WSS_NIS_OID_FLASH_INFO   = 11,                                       /* Module Flash Info                                       */
    EN_WSS_NIS_OID_CUST_INFO    = 14,                                       /* Customer Configurable Memory                            */
    EN_WSS_NIS_OID_FEATURE      = 128,                                      /* Module Supports Feature                                 */

    /* Environmental Monitors                                                   */
    EN_WSS_NIS_OID_TEMP = 150,                                              /* Temperature Sensors                                     */
    EN_WSS_NIS_OID_VOLT,                                                    /* Voltage Sensors                                         */
    EN_WSS_NIS_OID_PRES,                                                    /* Barometric Pressure                                     */

    /* Flexible Waveplan Management                                             */
    EN_WSS_NIS_OID_WP_CH_FREQ_MHz = 156,                                    /* Waveplan Channel - Center Freq in MHz                   */
    EN_WSS_NIS_OID_WP_CH_BW_MHz,                                            /* Waveplan Channel - Bandwidth in MHz                     */
    EN_WSS_NIS_OID_WP_CH1 = 160,                                            /* Waveplan First Chan Center                              */
    EN_WSS_NIS_OID_WP_BW,                                                   /* Waveplan Per-Channel Bandwidth                          */
    EN_WSS_NIS_OID_WP_OPTION,                                               /* Waveplan Options                                        */
    EN_WSS_NIS_OID_WP_CH_NUM,                                               /* Waveplan Channel Count                                  */
    EN_WSS_NIS_OID_WP_CLEAR,                                                /* Clear To Waveplan                                       */
    EN_WSS_NIS_OID_WP_CH_FREQ,                                              /* Waveplan Channel - Center Freq                          */
    EN_WSS_NIS_OID_WP_CH_BW,                                                /* Waveplan Channel - Bandwidth                            */

    /* WSS Configuration and Provisioning                                       */
    EN_WSS_NIS_OID_CH_PORT_ATTN = 169,                                      /* Channel Switch Port & Atten                             */
    EN_WSS_NIS_OID_CH_PORT,                                                 /* Channel Switch Port                                     */
    EN_WSS_NIS_OID_CH_ATTN,                                                 /* Channel Atten                                           */
    EN_WSS_NIS_OID_PACK_CH_PORT_ATTN = 173,                                 /* Packed Channel Switch Port & Atten                      */
    EN_WSS_NIS_OID_PACK_CH_PORT      = 178,                                 /* Packed Channel Switch Port                              */
    EN_WSS_NIS_OID_PACK_CH_ATTN,                                            /* Packed Channel Atten                                    */

    /* Alarms and Alarm Events                                                  */
    EN_WSS_NIS_OID_ALM_STAT     = 5,                                        /* Current Alarms                                          */
    EN_WSS_NIS_OID_FIFO_R       = 9,                                        /* Alarm Event History                                     */
    EN_WSS_NIS_OID_ALM_STAT_TR  = 12,                                       /* Current Alarms with Table/Row                           */
    EN_WSS_NIS_OID_FIFO_TR,                                                 /* Alarm Event History with Table/Row                      */

    /* Firmware Download                                                        */
    EN_WSS_NIS_OID_UPGD = 255,

    EN_WSS_NIS_OID_MAX

};

enum EN_WSS_NIS_PARA {
    EN_WSS_NIS_PARA_MEAS_VAL = 0,                                           /* 0 : Measured value                                      */
    EN_WSS_NIS_PARA_SET_POINT,                                              /* 1 : Setpoint (provisioned value or control target)      */
    EN_WSS_NIS_PARA_D_ALM_THR_L,                                            /* 2 : Degrade-Low alarm threshold                         */
    EN_WSS_NIS_PARA_D_ALM_THR_H,                                            /* 3 : Degrade-High alarm threshold                        */
    EN_WSS_NIS_PARA_F_ALM_THR_L,                                            /* 4 : Fail-Low alarm threshold                            */
    EN_WSS_NIS_PARA_F_ALM_THR_H,                                            /* 5 : Fail-High alarm threshold                           */
    EN_WSS_NIS_PARA_ALM,                                                    /* 6 : Boolean / Digital alarm (0 or 1)                    */
    EN_WSS_NIS_PARA_D_ALM_HYS_L,                                            /* 7 : Degrade-Low alarm hysteresis                        */
    EN_WSS_NIS_PARA_D_ALM_HYS_H,                                            /* 8 : Degrade-High alarm hysteresis                       */
    EN_WSS_NIS_PARA_F_ALM_HYS_L,                                            /* 9 : Fail-Low alarm hysteresis                           */
    EN_WSS_NIS_PARA_F_ALM_HYS_H,                                            /* 10: Fail-High alarm hysteresis                          */

    EN_WSS_NIS_PARA_MAX
};

enum EN_WSS_NIS_FRM_TYPE {
    EN_WSS_NIS_FRM_TYPE_S = 0,                                              /* Short frame                                             */
    EN_WSS_NIS_FRM_TYPE_L,                                                  /* Long frame                                              */

    EN_WSS_NIS_FRM_TYPE_MAX

};
#define __packed                        __attribute__((__packed__))
typedef struct
{
    uint8_t ucOpCode;
}__packed ST_WSS_SPI_RD_CMD_HEADER;

typedef struct
{
    uint8_t ucRdOper;
    uint16_t usMagic;
}__packed ST_WSS_SPI_RD_HEADER;


typedef struct
{
    uint8_t ucWrOper;
    uint16_t usReserve;
}__packed ST_WSS_SPI_WR_HEADER;

struct ST_WSS_NIS_CMD_S_HEAD_CMN {
    uint8_t ucMid;                                                          /* MID                                                     */
    uint8_t ucLen;                                                          /* Length, from command to checksum                        */
    uint8_t ucCmd;                                                          /* Command, refer to EN_WSS_NIS_CMD                        */
};

/* Single Instance Command Format                                                                 */
struct ST_WSS_NIS_CMD_S_HEAD_SINGLE {
    ST_WSS_NIS_CMD_S_HEAD_CMN stHeadS;

    uint8_t ucOid;                                                          /* Object ID, refer to EN_WSS_NIS_OID                      */
    uint8_t ucInst;                                                         /* Instance                                                */
    uint8_t ucPara;                                                         /* Parameter                                               */

};

/* Array Command Format                                                                           */
typedef struct {
    ST_WSS_NIS_CMD_S_HEAD_CMN stHeadS;

    uint8_t ucOid;                                                          /* Object ID, refer to EN_WSS_NIS_OID                      */
    uint8_t ucInst1;                                                        /* Instance 1                                              */
    uint8_t ucPara;                                                         /* Parameter                                               */
    uint8_t ucInstN;                                                        /* Instance N                                              */

} ST_WSS_NIS_CMD_S_HEAD_ARRAY;

/* Multi-Object Command Format                                                                    */
typedef struct {
    ST_WSS_NIS_CMD_S_HEAD_CMN stHeadS;

    uint8_t ucOid;
    uint8_t ucInst;
    uint8_t ucPara;

}  ST_WSS_NIS_CMD_S_HEAD_MULTI;

struct ST_PORT_ATTN {
    uint8_t ucPort;
    uint8_t ucAttn;
};

typedef struct {
    uint16_t ausFreq[WSS_CH_CNT_TOTAL];
    uint16_t ausBand[WSS_CH_CNT_TOTAL];

    uint16_t ausPortAttn1[WSS_CH_CNT_TOTAL];
    uint16_t ausPortAttn2[WSS_CH_CNT_TOTAL];

} ST_WSS_NIS_VAL;


#define DEF_WSS_NIS_LOOG_FRM_TAG      0x00FF

#define __packed                        __attribute__((__packed__))


typedef struct
{
    uint8_t ucMid;                                                          /* MID                                                     */
    uint16_t usTag;                                                         /* Long frame tag                                          */
    uint16_t usLen;                                                         /* Length, from command to crc                             */
    uint16_t usCmd;                                                         /* Command, refer to EN_WSS_NIS_CMD                        */

} __packed ST_WSS_NIS_CMD_L_HEAD_CMN;



/* Single Instance Command Format                                                                 */
typedef struct {
    ST_WSS_NIS_CMD_L_HEAD_CMN stHeadL;

    uint8_t ucOid;                                                          /* Object ID, refer to EN_WSS_NIS_OID                      */
    uint8_t ucInst;                                                         /* Instance                                                */
    uint8_t ucPara;                                                         /* Parameter                                               */

} __packed ST_WSS_NIS_CMD_L_HEAD_SINGLE;

/* Array Command Format                                                                           */
typedef struct {
    ST_WSS_NIS_CMD_L_HEAD_CMN stHeadL;

    uint8_t ucOid;                                                          /* Object ID, refer to EN_WSS_NIS_OID                      */
    uint8_t ucInst1;                                                        /* Instance 1                                              */
    uint8_t ucPara;                                                         /* Parameter                                               */
    uint8_t ucInstN;                                                        /* Instance N                                              */

} __packed ST_WSS_NIS_CMD_L_HEAD_ARRAY;

/* Multi-Object Command Format                                                                    */
typedef struct {
    ST_WSS_NIS_CMD_L_HEAD_CMN stHeadL;

    uint8_t ucOid;
    uint8_t ucInst;
    uint8_t ucPara;

} __packed ST_WSS_NIS_CMD_L_HEAD_MULTI;

/* Multi-Object Command Format                                                                    */
typedef struct {
    uint8_t ucOid;
    uint8_t ucTbl;
    uint8_t ucInst;
    uint8_t ucPara;

    uint16_t usData;

} __packed ST_WSS_NIS_CMD_L_MULTI;

typedef struct {
    uint8_t ucMid;                                                          /* MID                                                     */
    uint8_t ucLen;                                                          /* Length, from result to checksum                         */
    uint8_t ucRslt;                                                         /* Result, refer to EN_WSS_NIS_ERR_CODE                    */

} __packed ST_WSS_NIS_RESP_S_HEAD_CMN;

/* Single-Command Response Format                                                                 */
typedef struct {
    ST_WSS_NIS_RESP_S_HEAD_CMN stRespHeadS;

}  ST_WSS_NIS_RESP_S_HEAD_SINGLE;

/* Array Response Format                                                                          */
typedef struct {
    ST_WSS_NIS_RESP_S_HEAD_CMN stRespHeadS;

    uint8_t ucCnt;                                                          /* Success Count                                           */

} __packed ST_WSS_NIS_RESP_S_HEAD_ARRAY;

/* Multi-Object Response Format                                                                   */
typedef struct {
    ST_WSS_NIS_RESP_S_HEAD_CMN stRespHeadS;

    uint8_t ucCnt;                                                          /* Success Count                                           */

}__packed ST_WSS_NIS_RESP_S_HEAD_MULTI;

typedef struct {
    uint8_t ucMid;                                                          /* MID                                                     */
    uint16_t usLen;                                                         /* Length, from result to crc                              */
    uint8_t ucRslt;                                                         /* Result, refer to EN_WSS_NIS_ERR_CODE                    */

}__packed ST_WSS_NIS_RESP_L_HEAD_CMN;

/* Single-Command Response Format                                                                 */
typedef struct {
    ST_WSS_NIS_RESP_L_HEAD_CMN stRespHeadL;

}  ST_WSS_NIS_RESP_L_HEAD_SINGLE;

/* Array Response Format                                                                          */
typedef struct {
    ST_WSS_NIS_RESP_L_HEAD_CMN stRespHeadL;

    uint8_t ucCnt;                                                          /* Success Count                                           */

}__packed ST_WSS_NIS_RESP_L_HEAD_ARRAY;

/* Multi-Object Response Format                                                                   */
typedef struct {
    ST_WSS_NIS_RESP_L_HEAD_CMN stRespHeadL;

    uint8_t ucCnt;                                                          /* Success Count                                           */

} __packed ST_WSS_NIS_RESP_L_HEAD_MULTI;

typedef enum {
    EN_WSS_NIS_ERR_NONE = 0,                                                /* 0 : No Error                                            */
    EN_WSS_NIS_ERR_INVALID_CMD,                                             /* 1 : Invalid Command                                     */
    EN_WSS_NIS_ERR_OUT_OF_RANGE,                                            /* 2 : Value out of Range                                  */
    EN_WSS_NIS_ERR_CMD_FAILED,                                              /* 3 : Command Failed                                      */
    EN_WSS_NIS_ERR_CHKSUM_FAILED,                                           /* 4 : Checksum Failed                                     */
    EN_WSS_NIS_ERR_NOTALLW,                                                 /* 5 : Not Allowed                                         */
    EN_WSS_NIS_ERR_CMD_INPROGRESS,                                          /* 6 : Command in Progress                                 */
    EN_WSS_NIS_ERR_ENDOFDATA,                                               /* 7 : End of Data                                         */
    EN_WSS_NIS_ERR_CMD_TOO_LONG = 10,                                       /* 10: Command Too Long                                    */

    EN_WSS_NIS_ERR_MAX

} EN_WSS_NIS_ERR_CODE;

typedef struct {
    uint16_t usCh;                                                          /* Channel number, range: [1, 384]                         */

    uint16_t usFreq;                                                        /* Center frequency                                        */
    uint16_t usBand;                                                        /* Bandwidth                                               */

}__packed ST_WSS_NIS_MULTI;

typedef struct {
    uint8_t ucPort;                                                         /* Channel port                                            */
    uint8_t ucAttn;                                                         /* Channel attn                                            */
}   ST_WSS_CH_INFO, *PST_WSS_CH_INFO;


#ifdef __SOURCE_INSIGHT__
#define int8_t
#define uint8_t
#define int16_t
#define uint16_t
#define int32_t
#define uint32_t
#define int64_t
#define uint64_t
#define size_t
#define ssize_t
#endif

#define DEF_WSS_NIS_UART_RECV_LEN_MAX   (4096)
#define DEF_WSS_NIS_BUF_LEN             (DEF_WSS_NIS_UART_RECV_LEN_MAX)
#define DEF_WSS_NIS_FW_LEN              (1024)

#define DEF_WSS_NIS_MULTI_CNT_FRM       (128)                               /* Unit: channel                                           */
#define DEF_WSS_NIS_MULTI_CNT_MAX       (1024)                              /* Unit: channel                                           */

#define DEF_WSS_NIS_INST                (1)

#define WssNisCmdSglW(pucBufW, usLenW, uOid, usInst)    \
    __WssNisCmd(pucBufW, usLenW,                          \
                NULL, NULL,                            \
                EN_WSS_NIS_CMD_WR, uOid,                \
                false, 0,                            \
                false, 0,                            \
                usInst, EN_WSS_NIS_PARA_SET_POINT,       \
                false, 0,                            \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdSglR(pucBufR, pusLenR, uOid, usInst)   \
    __WssNisCmd(NULL, 0,                               \
                pucBufR, pusLenR,                         \
                EN_WSS_NIS_CMD_RD, uOid,                \
                false, 0,                            \
                false, 0,                            \
                usInst, EN_WSS_NIS_PARA_MEAS_VAL,        \
                false, 0,                            \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdSglTblW(pucBufW, usLenW, uOid, ucTbl, usInst)  \
    __WssNisCmd(pucBufW, usLenW,                          \
                NULL, NULL,                            \
                EN_WSS_NIS_CMD_WR, uOid,                \
                true, ucTbl,                        \
                false, 0,                            \
                usInst, EN_WSS_NIS_PARA_SET_POINT,       \
                false, 0,                            \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdSglTblR(pucBufR, pusLenR, uOid, ucTbl, usInst) \
    __WssNisCmd(NULL, 0,                               \
                pucBufR, pusLenR,                         \
                EN_WSS_NIS_CMD_RD, uOid,                \
                true, ucTbl,                        \
                false, 0,                            \
                usInst, EN_WSS_NIS_PARA_MEAS_VAL,        \
                false, 0,                            \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdUpgdW(pucBufW, usLenW)                         \
    __WssNisCmd(pucBufW, usLenW,                                  \
                NULL, NULL,                                    \
                EN_WSS_NIS_CMD_WR, EN_WSS_NIS_OID_UPGD,         \
                false, 0,                                    \
                false, 0,                                    \
                DEF_WSS_NIS_INST, EN_WSS_NIS_PARA_SET_POINT,      \
                false, 0,                                    \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdUpgdR(ucCmd, uiTimeOut)                                   \
    __WssNisCmd(NULL, 0,                                       \
                NULL, 0,                                       \
                ucCmd, EN_WSS_NIS_OID_UPGD,                     \
                false, 0,                                    \
                false, 0,                                    \
                DEF_WSS_NIS_INST, EN_WSS_NIS_PARA_MEAS_VAL,       \
                false, 0,                                    \
                EN_WSS_NIS_FRM_TYPE_L, uiTimeOut)

#define WssNisCmdArrW(pucBufW, uOid, usInst1, usInstN) \
    __WssNisCmd(pucBufW, (usInstN - usInst1 + 1) * 2,       \
                NULL, NULL,                        \
                EN_WSS_NIS_CMD_ARRAY_WR, uOid,      \
                false, 0,                        \
                false, 0,                        \
                usInst1, EN_WSS_NIS_PARA_SET_POINT,   \
                true, usInstN,                   \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdArrR(pucBufR, pusLenR, uOid, usInst1, usInstN) \
    __WssNisCmd(NULL, 0,                           \
                pucBufR, pusLenR,                     \
                EN_WSS_NIS_CMD_ARRAY_RD, uOid,      \
                false, 0,                        \
                false, 0,                        \
                usInst1, EN_WSS_NIS_PARA_MEAS_VAL,    \
                true, usInstN,                   \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdArrTblW(pucBufW, uOid, ucTbl, usInst1, usInstN) \
    __WssNisCmd(pucBufW, (usInstN - usInst1 + 1) * 2,       \
                NULL, NULL,                        \
                EN_WSS_NIS_CMD_ARRAY_WR, uOid,      \
                true, ucTbl,                    \
                false, 0,                        \
                usInst1, EN_WSS_NIS_PARA_SET_POINT,   \
                true, usInstN,                   \
                EN_WSS_NIS_FRM_TYPE_L)

#define WssNisCmdArrTblR(pucBufR, pusLenR, uOid, ucTbl, usInst1, usInstN) \
    __WssNisCmd(NULL, 0,                           \
                pucBufR, pusLenR,                     \
                EN_WSS_NIS_CMD_ARRAY_RD, uOid,      \
                true, ucTbl,                    \
                false, 0,                        \
                usInst1, EN_WSS_NIS_PARA_MEAS_VAL,    \
                true, usInstN,                   \
                EN_WSS_NIS_FRM_TYPE_L)
/**
 * @}
 */

/**
 * @}
 */


class CWssDriver : public CDevDriver
{
public:
    CWssDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
    {
    }
    CWssDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
    {
    }
    virtual
    ~CWssDriver()
    {
    }

    void
    SetProtoPtr(const std::shared_ptr<CWssSwProto> &c_rstpPtr)
    {
        m_stpProtoPtr = c_rstpPtr;
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(CWssStatusData &rstData) = 0;
    virtual bool GetCfgData(CWssAllCfg &rstData) = 0;

    virtual bool SetCfg(uint32_t uiInstBegin, uint32_t uiInstEnd, CWssCfg *pstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetOneChCfg(const CWssCfg &c_rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAllChCfg(uint32_t uiChCnt, CWssCfg *pstData, uint32_t uiSubIdx = 0) = 0;

    virtual bool SetWavePlan(uint32_t uiChCnt, CWavePlan *pstWavePlan, uint32_t uiSubIdx = 0) = 0;

    virtual bool Reset(uint8_t ucResetType) = 0;
    virtual bool Upgrade(const char *pcFilePath) = 0;

protected:
    std::shared_ptr<CWssSwProto> m_stpProtoPtr = nullptr;

};

class CWssUartDriver : public CWssDriver
{
public:
    CWssUartDriver(uint32_t uiUartIdx,
                   const std::shared_ptr<CUartDriver> &c_rstpPtr,
                   const std::shared_ptr<CResetBoard> &c_rstpColdResetBoard,
                   uint32_t uiColdResetIdx,
                   const std::shared_ptr<CResetBoard> &c_rstpWarmResetBoard,
                   uint32_t uiWarmResetIdx)
        : CWssDriver(uiUartIdx),
        m_stpUartDrvPtr(c_rstpPtr),
        m_stpColdResetBoard(c_rstpColdResetBoard),
        m_uiColdResetIdx(uiColdResetIdx),
        m_stpWarmResetBoard(c_rstpWarmResetBoard),
        m_uiWarmResetIdx(uiWarmResetIdx)
    {

    }

    ~CWssUartDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(CWssStatusData &rstData) override;
    virtual bool GetCfgData(CWssAllCfg &rstData) override;

    virtual bool SetCfg(uint32_t uiInstBegin, uint32_t uiInstEnd, CWssCfg *pstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetOneChCfg(const CWssCfg &c_rstData, uint32_t uiSubIdx = 0) override;
    virtual bool SetAllChCfg(uint32_t uiChCnt, CWssCfg *pstData, uint32_t uiSubIdx = 0) override;

    virtual bool SetWavePlan(uint32_t uiChCnt, CWavePlan *pstWavePlan, uint32_t uiSubIdx = 0) override;

    virtual bool Reset(uint8_t ucResetType) override;
    virtual bool Upgrade(const char *pcFilePath) override;


private:
    std::shared_ptr<CUartDriver> m_stpUartDrvPtr = nullptr;

    std::shared_ptr<CResetBoard> m_stpColdResetBoard = nullptr;
    uint32_t m_uiColdResetIdx = 0;

    std::shared_ptr<CResetBoard> m_stpWarmResetBoard = nullptr;
    uint32_t m_uiWarmResetIdx = 0;

    uint32_t m_uiPortCnt = WSS_PORT_MAX_NUM;
    int __SendRecv(uint8_t *pucBufW, uint16_t usLenW,
                   uint8_t *pucBufR, uint16_t *pusLenR,
                   uint8_t ucFrmType, uint32_t uiTimeOut = 100);

    int __WssNisCmd(uint8_t *pucBufW, uint16_t usLenW,
                    uint8_t *pucBufR, uint16_t *pusLenR,
                    uint16_t usCmd, uint8_t uOid,
                    bool bTbl, uint8_t ucTbl,
                    bool bRow, uint8_t ucRow,
                    uint16_t usInst1, uint8_t ucPara,
                    bool bArray, uint16_t usInstN,
                    uint8_t ucFrmType, uint32_t uiTimeOut = 100);

    int __WssNisCmdMultiW(ST_WSS_NIS_MULTI *pstMulti, uint16_t usCnt);


    bool __GetCfgData(uint32_t uiInst1, uint32_t uiInstN, CWssAllCfg &rstData);

};






