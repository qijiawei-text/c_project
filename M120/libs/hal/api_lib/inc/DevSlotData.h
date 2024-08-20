/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_SLOT_DATA__
#define _DEV_SLOT_DATA__

#ifdef __cplusplus
extern "C" {
#endif


enum EN_SLOT
{
    SLOT0 = 0,
    SLOT1,
    SLOT2,
    SLOT3,

    SLOT_NUM
};

enum ECardType
{
    CARD_TYPE_OCM = 0,
    CARD_TYPE_PA = 1,
    CARD_TYPE_BA,
    CARD_TYPE_OPS,
    CARD_TYPE_WSS,
    CARD_TYPE_OTA,
    CARD_TYPE_P_OTA,

    CARD_TYPE_400GZR = 5,
    CARD_TYPE_OPTICAL = CARD_TYPE_400GZR,
    CARD_TYPE_OCS = CARD_TYPE_400GZR,

    CARD_TYPE_400GZR_BETA = 7,
    CARD_TYPE_FAN,
    CARD_TYPE_POWER,

    CARD_TYPE_SCC,
    CARD_TYPE_CLASSIS,

    CARD_TYPE_FIX,
    CARD_TYPE_MUX,

    CARD_TYPE_UNKOWN = 0xFF
};


enum EN_SLOT_IO
{
    SLOT_IO = 0,

    SLOT_IO_NUM
};

/* slot */
#define SLOT_MAX_NUM                  (16)
#define SLOT_DEV_MAX_NUM              (16)

typedef struct
{
    uint32_t uiDevType;
    uint32_t uiDevIdx;
    uint32_t uiSubType;         /* e.g 2 pump in on dev(module) */
    uint32_t uiFuncType;    /* e.g 2 oa in one card */
    bool bOnBoard;
} CSlotDevInfo;

enum ESlotEvent
{
    SLOT_EVENT_OFFLINE = 0,
    SLOT_EVENT_ONLINE,
    SLOT_EVENT_MISMATCH
};

enum ESlotState
{
    SLOT_STATE_OFFLINE = 0,
    SLOT_STATE_ONLINE,
    SLOT_STATE_MISMATCH
};


typedef struct
{
    uint32_t uiCardType;
    uint32_t uiSlotState;
    uint32_t uiPlugFlag;
    uint32_t uiResetType;
    uint32_t uiDevNum;
    CSlotDevInfo astDevInfo[SLOT_DEV_MAX_NUM];
} CSlotInfo;

typedef struct
{
    int32_t iTemp;
    CLedSta astLed[LED_MAX_NUM];
    bool bEEpromAlarm;

} CSlotStatusData;


#ifdef __cplusplus
};
#endif

#endif


