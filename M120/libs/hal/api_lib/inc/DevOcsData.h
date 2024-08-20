/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevOcsData.h
   Author:        Lu Mao
   Date:          2024-01-02
   Version:       1.0
   Description:
   Function List:

   History:
   [Lu Mao] [2024-01-02] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_OCS_DATA__
#define _DEV_OCS_DATA__

#ifdef __cplusplus
extern "C" {
#endif

#define DEF_OCS_16X16_SWITCH_CHANNES_MAX             (16)
#define DEF_OCS_32X32_SWITCH_CHANNES_MAX             (32)
#define DEF_OCS_64X64_SWITCH_CHANNES_MAX             (64)

#define OCS_CONNECTION_MAX                           (128)

typedef enum
{
    OCS_ALARM_PS_VOL_HIGH,
    OCS_ALARM_PS_VOL_LOW,
    OCS_ALARM_SW_VOL_HIGH,
    OCS_ALARM_SW_VOL_LOW,
    OCS_ALARM_CALI_DATA,
    OCS_ALARM_MAX
} EN_OCS_ALARM;

typedef enum
{
    OCS_LED_OFF = 0,
    OCS_LED_RED,
    OCS_LED_GREEN,
    OCS_LED_AMBER,
    OCS_LED_RED_BLINK,              /* Red blink, 0.5s  ON, 0.5s OFF */
    OCS_LED_GREEN_BLINK,            /* Green blink, 0.5s ON, 0.5s OFF */
    OCS_LED_AMBER_BLINK,            /* Amber blink, 0.5s ON, 0.5s OFF */
    OCS_LED_RED_GREEN_BLINK,        /* Red / Green blink, 0.5s red, 0.5s green */
    OCS_LED_RED_AMBER_BLINK,        /* Red / Amber blink, 0.5s red, 0.5s amber */
    OCS_LED_GREEN_AMBER_BLINK,      /* Green / Amber blink, 0.5s green, 0.5s amber */
    OCS_LED_GREEN_RED_AMBER_BLINK,  /* Green / Off / Red / Off / Amber / OFF blink, 0.5s interval */
    OCS_LED_MAX
} EN_OCS_LED_STA;

typedef enum
{
    OCS_LED_NORMAL_WORK_STATE = 0,
    OCS_LED_LAMP_TEST_STATE,
} EN_CS_LED_WORK_STATE;

typedef enum
{
    OCS_TYPE_16X16_SWITCH = 0,
    OCS_TYPE_32X32_SWITCH,
    OCS_TYPE_64X64_SWITCH,
    OCS_TYPE_SWITCH_MAX
} EN_OCS_TYPE_SWITCH;

typedef struct ST_OCS_CONNECTION
{
    uint8_t ucId;
    uint8_t ucSource;
    uint8_t ucDestination;
    uint8_t ucReserve;
} COcsConnection;

typedef struct ST_OCS_CONNECTIONS
{
    /* Actual count of connections */
    uint32_t uiCount;
    /* The connections for all ports */
    COcsConnection astConn[OCS_CONNECTION_MAX];
} COcsConnections;

typedef struct
{
    bool abAlarms[OCS_ALARM_MAX];
    uint8_t aucReserve[3];
} COcsAlarm;

typedef struct
{
    uint8_t aucLedSta[LED_MAX_NUM];       /*refer to EN_OCS_LED_STA */
    uint8_t ucLedWorkSta;                 /*refer to EN_CS_LED_WORK_STATE */
    uint8_t aucReserve[3];
} COcsLedSta;

typedef struct
{
    int16_t sCaseTemp;              /* Unit: 0.01 */
    uint8_t aucReserve[2];
} COcsTempSta;


typedef struct ST_OCS_STATUS
{
    /* All alarms */
    COcsAlarm stAlm;
    /* All connections state */
    COcsConnections stConnsSta;
    /* All LEDs state */
    COcsLedSta stLedSta;
    COcsTempSta stTempSta;
} COcsStatusData;

typedef struct ST_OCS_CFG
{
    COcsConnections stConns;
} COcsCfgData;

#ifdef __cplusplus
};
#endif

#endif
