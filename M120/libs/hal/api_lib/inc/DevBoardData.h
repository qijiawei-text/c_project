/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevBoardData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_BOARD_DATA__
#define _DEV_BOARD_DATA__

#ifdef __cplusplus
extern "C" {
#endif


enum EN_FAN
{
    FAN0 = 0,
    FAN1,
    FAN2,
    FAN3,

    FAN_CNT
};

enum EN_PWR
{
    PWR0 = 0,
    PWR1,

    PWR_CNT
};

/*luowei modify*/
enum
{
    RST_CHASSIS,
    RST_CARD_SCC,
    RST_CARD_OCM,
    RST_CARD_BA,
    RST_CARD_PA,
    RST_CARD_OPTICAL,
    RST_CARD_OTDR,
    RST_CARD_EXTEND,
    RST_CARD_AMP,
    RST_CARD_AMP_P,
    RST_CARD_EDFA,
    RST_CARD_EDFA_P,
    RST_CARD_OCM_P,
    RST_CARD_OTDR_P,
    RST_CARD_OPS,
    RST_CARD_OCS,

    RST_CARD_OPS4_A,
    RST_CARD_OPS4_B,
    RST_MODULE_MAX

};

enum
{
    RST_MODE_POWER,
    RST_MODE_WARM,
    RST_MODE_COLD,
    RST_MODE_WDT,
    RST_MODE_BUTTON,

    RST_MODE_CNT
};

enum EN_TEMPERATURE_SENSOR
{
    TEMP_CHASSIS = 0,
    TEMP_SCC,
    /* TEMP_SLOT1, */
    /* TEMP_SLOT2, */

    TEMP_NUM
};

enum
{
    LED_STATUS_OFF,
    LED_STATUS_RED,
    LED_STATUS_GREEN,
    LED_STATUS_AMBER,
    LED_STATUS_RED_BLINK = 5,
    LED_STATUS_GREEN_BLINK,
    LED_STATUS_AMBER_BLINK
};

enum
{
    MAX1139_AIN0,
    MAX1139_AIN1,
    MAX1139_AIN2,
    MAX1139_AIN3,
    MAX1139_AIN4,
    MAX1139_AIN5,
    MAX1139_CNT
};

enum EN_VOL
{
    VOL_SCC,
    VOL_SLC0,
    VOL_SLC1,
    VOL_CNT
};


#if 0
enum EN_LED
{
    LED_SYS = 0,
    LED_ALM,
    LED_PWR,
    LED_FAN,

    LED_CNT
};

enum EN_BOARD_LED_SUBIDX
{
    BOARD_LED_SYS = 0,
    BOARD_LED_ALM,
    BOARD_LED_PWR,
    BOARD_LED_FAN,

    BORAD_LED_IDX_CNT
};

enum EN_SLOT_LED_SUBIDX
{
    SLOT0_LED_SYS = 0,
    SLOT0_LED_ALM,

    SLOT1_LED_SYS,
    SLOT1_LED_ALM,

    SLOT2_LED_SYS,
    SLOT2_LED_ALM,

    SLOT3_LED_SYS,
    SLOT3_LED_ALM,

    SLOT_LED_IDX_CNT
};

#else
enum EN_LED
{
    LED0 = 0,
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED_CNT
};

#endif

typedef struct
{
    uint32_t uiDuty;
} CFanCfg;

typedef struct
{
    bool bIsOnline;
    bool bIsFailure;
    uint32_t uiSpeed;     /* rpm */
    uint32_t uiLedState;

    /* This is ONLY in ueno project, for the led control register in Ueno is NOT ready, we can    */
    /* use the 0x10 register in FAN to save the reset cause.                                      */
    uint8_t ucResetCause;
} CFanSta;

typedef struct
{
} CPowerCfg;

typedef struct
{
    bool bIsOnline;
    bool bIsFailure;
    uint16_t usInVol;
    uint16_t usOutVol;
    uint32_t uiInCurrent;
    uint32_t uiOutCurrent;
    uint32_t uiWatts;
    int32_t iTemp;
} CPowerSta;

#define FAN_MAX_NUM                          16
#define POWER_MAX_NUM                        16
#define TEMP_MAX_NUM                         8
#define SFP_NUM                              2
#define LTC4215_MAX_NUM                      3
#define MAX1139_MAX_NUM                      3
#define MP5023_MAX_NUM                       3

struct CBoardMfg
{
    CMfg stChassis;
    bool bChassisMfgOk;
    CMfg stScc;
    bool bSccMfgOk;
    CMfg stRpc;
    bool bRpcMfgOk;
    CMfg astFan[FAN_MAX_NUM];
    bool abFanMfgOk[FAN_MAX_NUM];
    CMfg astPower[POWER_MAX_NUM];
    bool abPowerMfgOk[POWER_MAX_NUM];
};

struct CBoardCfgData
{
    CLedCfg astLed[LED_MAX_NUM];
    CFanCfg astFan[FAN_MAX_NUM];
    CPowerCfg astPower[POWER_MAX_NUM];
};

struct CBoardCali
{
    CCali stChassis;
};

typedef struct
{
    int32_t iSense;
    int32_t iSource;
} CLtc4215Sta;

typedef struct
{
    float adVoltage[12];
} CMax1139Sta;

typedef struct
{
    double dVIn;
    double dVOut;
    double dIOut;
} CMp5023Sta;

#define BOARD_PORT_MAX_NUM           (2)
typedef struct
{
    CFanSta astFan[FAN_MAX_NUM];
    CPowerSta astPower[POWER_MAX_NUM];
    CLedSta astLed[LED_MAX_NUM];

    bool abApsdStatus[SFP_NUM];
    bool abApsdEvent[SFP_NUM];
    bool abApsdEnable[SFP_NUM];


    uint8_t aucResetType[16];

    int32_t aiTemp[TEMP_MAX_NUM];
    int32_t aiPortPower[BOARD_PORT_MAX_NUM];

    CLtc4215Sta astLtc4215[LTC4215_MAX_NUM];
    CMp5023Sta astMp5023[MP5023_MAX_NUM];
    CMax1139Sta astMax1139[MAX1139_MAX_NUM];
} CBoardStatusData;

typedef struct
{
    uint8_t aucLogicVal[256];

    uint8_t aucUpgInfo[256];
    uint8_t aucBdlHead[1024];

} CBoardRealTimeData;


#ifdef __cplusplus
};
#endif

#endif



