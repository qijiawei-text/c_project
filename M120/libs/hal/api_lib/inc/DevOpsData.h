/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOpsData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_OPS_DATA__
#define _DEV_OPS_DATA__

#include "DevWssData.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OPS_SUB_INDEX_DEFAULT   1
#define OPS1_OPS_CNT_MAX        1
#define OPS4_OPS_CNT_MAX        4
#define OPS_HW_IDX(idx)         ((idx) & 0xFF)
#define OPS_HW_TYPE(idx)        ((idx) >> 8 & 0xFF)
#define OPS_ID_PACK(type, port) (((type) << 4 & 0xF0) | ((port) & 0x0F))
#define OPS_ID_UNPACK(id, type, port) \
    do { \
        (port) = (id) & 0x0F; \
        (type) = (id) >> 4 & 0x0F; \
    } while (0)

enum
{
    OPS_IDX_1,
    OPS_IDX_2,

    OPS_CNT
};

enum
{
    OPS_PATH_SECOENDARY,
    OPS_PATH_PRIMARY,

    OPS_PATH_CNT
};

enum
{
    OPS_STATUS_INIT = 0,
    OPS_STATUS_BLOCK, /* 1 reserved */

    OPS_STATUS_NO_REQUEST_WORK, /* 2 */
    OPS_STATUS_NO_REQUEST_PROTECT, /* 3 */

    OPS_STATUS_MANUAL2WORK, /* 4 */
    OPS_STATUS_MANUAL2PROTECT, /* 5 */

    OPS_STATUS_SIGNAL_FAIL_WORK, /* 6 */
    OPS_STATUS_SIGNAL_FAIL_PROTECT, /* 7 */

    OPS_STATUS_FORCE2WORK, /* 8 */
    OPS_STATUS_FORCE2PROTECT, /* 9 */

    OPS_STATUS_WTR, /* 10 */
    OPS_STATUS_LOCK_OUT, /* 11 */

    OPS_STATUS_RELATIVE_SWITCH_WORK,     /* 12 */
    OPS_STATUS_RELATIVE_SWITCH_PROTECT,     /* 13 */

    OPS_STATUS_CNT
};

#if 0
enum
{
    OPS_MODE_CLEAR,
    OPS_MODE_LOCKOUT_PROTECT,
    OPS_MODE_FORCE_S,
    OPS_MODE_MAN_P,
    OPS_MODE_MAN_S,

};
#endif

enum
{
    OPS_MODE_CLEAR,
    OPS_MODE_FORCE_S,
    OPS_MODE_FORCE_P,
    OPS_MODE_CLEAR_BLOCKED,
    OPS_MODE_MFG_TEXT_MODE,
    OPS_MODE_MAN_S,
    OPS_MODE_MAN_P,
    OPS_MODE_CLEAR_WTR,
    OPS_MODE_LOCKOUT_PROTECT,

};

enum
{
    OPS_THR_TYPE_LOS,
    OPS_THR_TYPE_DIFFER,
    OPS_THR_TYPE_LOW,
    OPS_THR_TYPE_HIGH,
    OPS_THR_TYPE_CNT
};

enum
{
    OPS_ALARM_TYPE_LOS,
#if 0
    OPS_ALARM_TYPE_LOW,
    OPS_ALARM_TYPE_HIGH,
#endif
    OPS_ALARM_TYPE_CNT
};

enum
{
    OPS_PORT_LINE1_IN,
    OPS_PORT_LINE2_IN,
    OPS_PORT_LINE_IN_CNT = 2,
#if 0
    OPS_PORT_LINE1_OUT = 2,
    OPS_PORT_LINE2_OUT,
#endif
    OPS_PORT_CNT = 2
};


/*
   0 - off
   1 - red
   2 - green
   3 - shine
   4 - horse race lamp
 */

enum
{
    OPS_LED_OFF = 0,
    OPS_LED_RED,
    OPS_LED_GREEN,
    OPS_LED_SHINE,
    OPS_LED_LAMP
};


/*
    APS switching type define:
        0 - unidirectional
        1 - bidirectional
 */
enum
{
    OPS_SWITCHING_TYPE_UNI = 0,
    OPS_SWITCHING_TYPE_BI = 1
};

#define PORT_POWER_HISTORY_MAX_NUM   (81)

#define OPS_LED_CNT (18)
#define OPS_LED_CNT_PERCHAN (4)

typedef struct
{
    uint8_t ucW;
    uint8_t ucP;
    uint8_t ucMux;
    uint8_t ucWP;
} COpsLedChannel;

typedef struct
{
    uint8_t ucAct;
    uint8_t ucAlm;
    COpsLedChannel stOpsLedChannel[OPS4_OPS_CNT_MAX];
} COpsLedAll;

typedef union {
    uint8_t aucStatus[OPS_LED_CNT];
    COpsLedAll stOpsLedAll;
} COpsLed;


typedef struct
{
    uint8_t ucWorkMode;
    uint8_t ucSwitchPos;
    uint8_t ucFswStatus;

    /* Switching type, 0–unidirectional, 1–bidirectional */
    uint8_t ucSwitchingType;

    int16_t asPortPower[OPS_PORT_CNT];

    int16_t sTxPower;

    bool bFailure;
    bool bInnerErr;
    bool bOffLine;

} COpsCurData;


typedef struct
{
    uint8_t aaucPortAlarm[PORT_ALARM_TYPE_CNT][OPS_PORT_CNT];
    uint8_t ucSwitchFailed;
    uint8_t ucRelative;
    bool abSwitchAlarm[OPS_PATH_CNT];
    uint8_t ucMfgFailed;
    uint8_t ucComInLoss;
    uint8_t ucPowerDiffer;
    uint32_t auiDuration[PORT_ALARM_TYPE_CNT][OPS_PORT_CNT];
}COpsAlarm;

typedef struct
{
    uint32_t uiHour;
    uint32_t uiMinute;
    uint32_t uiSecond;
    uint32_t uiMillisecond;
    int16_t asPortPowerHistory[OPS_PORT_LINE_IN_CNT][PORT_POWER_HISTORY_MAX_NUM];

} COpsHistoryUnitData;

typedef struct
{
    uint32_t uiSwitchTimes;     /* [Switch times]: 4 bytes, unsigned int, the max is 10. */
    COpsHistoryUnitData astOpsHisUnitData[10];
} COpsHistoryData;


#define OSC_PD_CNT 2

typedef struct {
    int16_t asPd[OSC_PD_CNT];
} COscPd;

typedef struct {
    uint32_t uiOut;
    uint32_t uiIn;
} COscSwitchPos;

typedef struct {
    uint32_t uiSwitchIdx; /* 0 -- TX 1 -- RX */
    uint32_t uiPos; /* 0 -- WSS1 1 -- WSS2 */
} COscSwitchPosSet;

#define OPS_VOLTAGE_CNT  11
typedef struct {
    int16_t asVoltage[OPS_VOLTAGE_CNT];
} COpsVoltage;

typedef struct {
    int16_t asInsertionLoss[4];
} COpsIL;

typedef struct {
    int16_t sStatus;
} COpsUpgStat;
#if 0
enum
{
    OPS_CONN_PD_W_RCV_IN,       /* OPS_CONN_CLIENT_W_R */ /* PD3 to Client-W R */
    /* OPS_CONN_MUX_T_W */   /* PD3 to MUX T */
    OPS_CONN_PD_P_RCV_IN,       /* OPS_CONN_CLINET_P_R */ /* PD4 to Client-P R */
    OPS_CONN_MUX_T_CV_OUT,  /*OPS*//* PD3 to MUX T */
    OPS_CONN_MUX_T_CV_IN,   /* PD5 to MUX T */
    OPS_CONN_W_T_CV_IN, /* PD6 to Client-W T */
    OPS_CONN_P_T_CV_IN, /* PD6 to Client-P T */
    OPS_CONN_MUX_R_CV_OUT,      /* P7 to MUX R */
    OPS_CONN_MUX_R_CV_IN,   /* P6 to MUX R */
    OPS_OPS_CONN_CNT
};
#else
enum
{
    OPS_CONN_CLIENT_W_R,        /* PD3 to Client-W R */
    OPS_CONN_MUX_T_W_OUT,       /* PD3 to MUX T */
    OPS_CONN_CLIENT_P_R,        /* PD4 to Client-P R */
    OPS_CONN_MUX_T_P_OUT,       /* PD4 to MUX T */
    OPS_CONN_MUX_T_IN,          /* PD5 to MUX T */
    OPS_CONN_CLIENT_W_T,        /* PD6 to Client-W T */
    OPS_CONN_CLIENT_P_T,        /* PD6 to Client-P T */
    OPS_CONN_MUX_R_OUT,         /* PD6 to MUX R */
    OPS_CONN_MUX_R_IN,          /* PD7 to MUX R */
    OPS_CONN_CNT
};

/* other PDs power use for fnc */
enum
{
    OPS_LINE1_T,
    OPS_LINE2_T,
    OPS_T,
    OPS_R,
    OPS1_CONN_CNT
};

#endif
typedef struct {
    int16_t asPd[OPS1_CONN_CNT];
} COpsConnectionPd;

#define OPS_PD_ADC_CNT  7
typedef struct {
    int32_t asPdAdc[OPS_PD_ADC_CNT];
} COpsPdAdc;

typedef struct
{
    int16_t asPdThr[OPS_CONN_CNT];
} COpsConnectionPdThr;

typedef struct
{
    int16_t asPdHys[OPS_CONN_CNT];
} COpsConnectionPdHys;

/************************************************************************************************/
/* Data define for 1+1 bidirectional switching.                                                 */
/************************************************************************************************/
/* @ {                                                                                          */
typedef enum {
    APS_STATE_CODE_NR = 0,
    APS_STATE_CODE_DNR = 1,
    APS_STATE_CODE_RR = 2,
    APS_STATE_CODE_EXER = 4,
    APS_STATE_CODE_WTR = 5,
    APS_STATE_CODE_MS = 7,
    APS_STATE_CODE_SD = 9,
    APS_STATE_CODE_SF = 11,
    APS_STATE_CODE_FS = 13,
    APS_STATE_CODE_SFP = 14,
    APS_STATE_CODE_LO = 15
} APS_STATE_CODE_EN;

typedef struct ST_OPS_REQ_STATUS {
    /* APS-Specific Information, 4 bytes                                                        */
    /* R: 0(no-revertive) 1(revertive)                                                          */
    uint8_t ucProtTypeR : 1;
    /* D: 0(Unidirectional) 1(Bidirecrtional)                                                   */
    uint8_t ucProtTypeD : 1;
    /* B: 0(1+1) 1(1:1)                                                                         */
    uint8_t ucProtTypeB : 1;
    /* A: 0(no APS channel) 1(APS channel)                                                      */
    uint8_t ucProtTypeA : 1;
    /* APS Request/State                                                                        */
    uint8_t ucApsState : 4;
    /* Requested signal: 0(Null signal) 1(Normal traffic signal)                                */
    uint8_t ucRequestedSignal;
    /* Bridged signal: 0(Null signal) 1(Normal traffic signal)                                  */
    uint8_t ucBridgedSignal;
    uint8_t ucReserved;
} COpsReqStatus;

typedef struct ST_OPS_BIDIRECTION_STATUS {
    /* Working mode of local switch.                                                            */
    uint8_t ucLocalWorkMode;
    /* Position of local switch.                                                                */
    uint8_t ucLocalPosition;
    /* Status of local switch.                                                                  */
    uint8_t ucLocalStatus;
    /* Requested information of far end switch                                                  */
    COpsReqStatus stRemoteStatus;
} COpsBidirectionStatus;

/* @ }                                                                                          */
typedef enum {
    ALM_MASK_LINE1INLOSS = 0,
    ALM_MASK_LINE2INLOSS,
    ALM_MASK_SWITCHFAILED,
    ALM_MASK_LUT,
    ALM_MASK_RELATIVEBTWL1L2,
    ALM_MASK_VOLTAGE,
    ALM_MASK_COMINLOSS,
    ALM_MASK_RCVPOWERDIFF
} OPS_ALM_MASK_IDX_EN;

typedef struct
{
    uint8_t Line1InLossAlarm;
    uint8_t Line2InLossAlarm;
    uint8_t SwitchFailedAlarm;
    uint8_t LutAlarm;
    uint8_t RelativeAlarmbetweenLine1andLine2;
    uint8_t PDVoltageAlarm;
    uint8_t ComInLossAlarm;
    uint8_t ReceivePowerDifferBetween;
}COpsAlmMask;

typedef struct {
    COpsAlarm stAlarm;
    COpsLed stLed;
    COpsVoltage stVoltage;
    COpsAlmMask stAlmMask;
} COpsCommStatusData;

/* OPS */
typedef struct
{
    COpsCurData stCurData;
    /* COpsAlarm stAlarm; */

    CWssStatusData stWssStaData;

    COscPd stOscPd;

    /* COpsLed stLed; */

    /* COpsVoltage stVoltage; */

    COpsIL stIl;
    COpsConnectionPd stConnectionPds;
    /* COpsPdAdc stPdAdc; */

    /* Status data for bi-directional OPS */
    COpsBidirectionStatus stBidiStatus;
} COpsStatusData;

typedef struct
{
    int16_t asThr[OPS_PATH_CNT];

} COpsSwitchThr;

typedef struct
{
    int16_t asHys[OPS_PATH_CNT];

} COpsSwitchHys;

#if 0
typedef struct
{
    uint32_t uiWtr;

    int16_t sLine1Thr;
    int16_t sLine1Hys;
    int16_t sLine2Thr;
    int16_t sLine2Hys;
    int16_t sPwrDiffThr;
    int16_t sPwrDiffHys;

    COpsSwitchThr stThr;
    COpsSwitchHys stHys;
} COpsCfgData;

#endif

typedef struct {
    int16_t asThr[OPS_ALARM_TYPE_CNT][OPS_PORT_CNT];
    int16_t sDifferBetweenLine1Line2;
} CPortPowerThr;

typedef struct {
    int16_t asHys[OPS_ALARM_TYPE_CNT][OPS_PORT_CNT];
} CPortPowerHys;


typedef struct {
    int16_t asThr[PORT_ALARM_TYPE_CNT][WSS_PORT_CNT];
} CWssPortPowerThr;

typedef struct {
    int16_t asHys[PORT_ALARM_TYPE_CNT][WSS_PORT_CNT];
} CWssPortPowerHys;

typedef struct {
    int16_t sThr;
} CRelativeThr;

typedef struct {
    int16_t sOffset;
} CRelativeOffset;

typedef struct {
    uint8_t ucWorkMode;
    uint16_t usRelativeEnable;     /* 0--Disable 1--Enable */
    uint16_t usHoldTime;     /* 10ms, [0, 1000] */
    uint16_t usRelativeSwitchThr;     /* 0.01dB, {0, [300, 1000]} */
    int16_t sRelativeOffset;     /* 0.01dB */
    uint32_t uiWtrTime;     /* s, [300, 7200] */
    uint32_t uiHour;
    uint32_t uiMinute;
    uint32_t uiSecond;


    CPortPowerThr stPortPowerThr;
    CPortPowerHys stPortPowerHys;

    CWssPortPowerThr stWssPortPowerThr;
    CWssPortPowerHys stWssPortPowerHys;

    COpsSwitchThr stSwitchThr;
    COpsSwitchHys stSwitchHys;

    COpsConnectionPdThr stConnectionPdThr;
    COpsConnectionPdHys stConnectionPdHys;

} COpsCfgData;

typedef enum OPS_EVENT_LOG_INDEX_en
{
    OPS_EVENT_LOG_SWITCH_TIME = 0,
    OPS_EVENT_LOG_ALARM_TIME,
    OPS_EVENT_LOG_ALARM_W_POWER,
    OPS_EVENT_LOG_ALARM_P_POWER,
    OPS_EVENT_LOG_SWITCH_STATE,
    OPS_EVENT_LOG_MAX
}OPS_EVENT_LOG_INDEX_EN;

typedef struct
{
    uint32_t uiSwitchOffsetTime;
    uint32_t uiAlarmOffsetTime;
    int16_t sAlarmWPower;
    int16_t sAlarmPPower;
    uint32_t uiSwitchState;
} COpsEventLogData;

typedef struct
{
    COpsEventLogData stOpsEventLogData[4];
    uint8_t rev[192];
} COpsEventLogAllData;

#ifdef __cplusplus
};
#endif

#endif



