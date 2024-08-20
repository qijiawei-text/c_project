/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevEdfaData.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#ifndef _DEV_EDFA_DATA__
#define _DEV_EDFA_DATA__

#ifdef __cplusplus
extern "C" {
#endif
#include "DevOscData.h"
#include "DevOtdrData.h"
#include "DevWssData.h"
#include "DevOcmData.h"

#define PD_MAX_NUM                           32
#define VOA_MAX_NUM                          16

/* OA */
#define OA_PD_MAX_NUM                        PD_MAX_NUM
#define OA_PORT_MAX_NUM                      32
#define OA_PUMP_MAX_NUM                      16
#define OA_FIBER_MAX_NUM                     16
#define OA_VOA_MAX_NUM                       VOA_MAX_NUM
#if defined __HCF__ || defined __OCS__
#define OA_LED_MAX_NUM                       8
#else
#define OA_LED_MAX_NUM                       8
#endif
#define EDFA_TILT_ELE_MAX_NUM                32

typedef struct
{
    COtdrStartOffsetAll stOtdrCaliInfo;
    COcmCali stOcmCaliInfo;
} COaCaliData;

typedef struct
{
    uint8_t ucMode;
    uint8_t ucGainRange;
    uint8_t ucRflAlmEnable;
    uint8_t ucLosAlmMode;
    uint8_t ucMute;
    uint8_t ucMuteMode;

    bool bPumpEnable;
    bool bAlsEnable;
    bool bSafeEnable;
    bool bAprEnable;

    uint8_t ucBandType;        /* for raman C/L switch */
    uint8_t aucPortStatus[OA_PORT_MAX_NUM];

    int16_t sGain;
    int16_t sApcTarget;
    int16_t sTilt;
    int16_t sAprPower;
    int16_t asPumpCurrent[OA_PUMP_MAX_NUM];


    int16_t sInThr;
    int16_t sInHys;
    int16_t sOutThr;
    int16_t sOutHys;
    int16_t sMsaInThr;
    int16_t sMsaInHys;
    int16_t sMsaOutThr;
    int16_t sMsaOutHys;

    int16_t sAttn;
    COaCaliData stCaliData;
} COaCfgData;
enum
{
    OA_IDX_1,
    OA_IDX_2,

    OA_CNT
};

enum
{
    VOA_IDX_1,
    VOA_IDX_2,
    VOA_IDX_3,
    VOA_IDX_4,   /* E2_interVOA */
    VOA_IDX_5,   /* E2_PaddingVOA */
    VOA_IDX_6,   /* OCM VOA IN E2 */
    VOA_IDX_7,
    VOA_IDX_8,
    VOA_CNT
};


typedef struct
{
    bool bFailure;
    bool bDisable;
    bool bInnerErr;

    bool bInLos;
    bool bOutLos;
    bool bMsaInLos;
    bool bMsaOutLos;

    bool bRFL;
    bool bOOG;
    bool bOOP;
    bool abPumpEOL[8];
    bool abPumpTemp[8];
    bool bCaseTemp;
    bool bPumpTempHigh;
    bool bPumpTempLow;
    bool bLimited;
    bool bApsd;

    /* fnc add */
    bool bInLow;
    bool bOutLow;
    bool bInHigh;
    bool bOutHigh;
    bool bFiberTemp;
    bool bHeaterTemp;
    bool bApr;
} COaAlarm;

typedef enum OA_THR_TYPE_en
{
    OA_THR_TYPE_LOS = 0,
    OA_THR_TYPE_LOW,
    OA_THR_TYPE_HIGH,
    OA_THR_TYPE_MAX
}OA_THR_TYPE_EN;


typedef struct
{
    int32_t iThr;
    int32_t iHys;
}CMN_THR_HYS_INFO_ST;

#ifdef __NR1004__
/*
   ParameterName	HWPD    CurPD      Comment
   E1_PDIN	        1	    PD3	       Cus PD3  HW-Mapping   PA
   E1_PDOUT	        2	    PD4	       Cus PD4
   E1_OSCADD	    0
   E1_OSCDROP	    10	    PD8	       Cus PD8
   E1_RFL	        8	    PD10	   Cus PD10
   E1_VOA1 IN	    5	    PD12       Cus PD12  interVOA1
   E1_VOA1 OUT	    6	    PD13       Cus PD13  interVOA1
   E1_VOA2 IN	    0
   E1_VOA2 OUT	    0
   E1_VOA3 IN	    10	    PD8	       Cus PD8
   E1_VOA3 OUT	    0
   E1_PM1	        0
   E1_PM2           0
   E1_PM3           0
   E1_PM4           0                  E1_ DeltaPwr
   E1_PM5           0                  EDFA1_OUT_SIG
   E2_PDIN	        3	    PD1	       cus PD1   E2_PDIN   BA
   E2_PDOUT	        4	    PD5	       cus PD5   E2_PDOUT
   E2_OSCADD	    15	    PD6	       cus PD6
   E2_OSCDROP	    0
   E2_RFL	        14	    PD9	       cus PD9
   E2_VOA1 IN
   E2_VOA1 OUT
   E2_VOA2 IN	    4	    PD5	       cus PD5   E2_padding VOA IN
   E2_VOA2 OUT	    11	    PD2	       cus PD2   E2_padding VOA OUT
   E2_VOA3 IN	    0
   E2_VOA3 OUT	    15	    PD6	       PD6
   E2_PM1	        0
   E2_PM2	        0
   E2_PM3	        0
   E2_PM4	        0                  E2_ DeltaPwr
   E2_PM5	        0                  EDFA2_OUT_SIG
 */
typedef enum
{
    E1_PDIN,           /*  PD3   PA */
    E1_PDOUT,          /*  PD4      */
    E1_OSCADD,
    E1_OSCDROP,        /*  PD8      */
    E1_RFL,            /*  PD10     */
    E1_VOA1_IN,
    E1_VOA1_OUT,
    E1_VOA2_IN,
    E1_VOA2_OUT,
    E1_VOA3_IN,        /*  PD8      */
    E1_VOA3_OUT,
    E1_PM1,
    E1_PM2,
    E1_PM3,
    E1_PM4,
    E1_PM5,
    E2_PDIN,           /*  PD1   BA */
    E2_PDOUT,          /*  PD5      */
    E2_OSCADD,         /*  PD6      */
    E2_OSCDROP,
    E2_RFL,            /*  PD9      */
    E2_VOA1_IN,
    E2_VOA1_OUT,
    E2_VOA2_IN,        /*  PD5      */
    E2_VOA2_OUT,       /*  PD2      */
    E2_VOA3_IN,
    E2_VOA3_OUT,       /*  PD6      */
    E2_PM1,
    E2_PM2,
    E2_PM3,
    E2_PM4,
    E2_PM5
}OLT_PD_PWR_E;
#endif

#ifdef __OLSP__
typedef enum
{
    PD1_EDFA2_IN_PWR,
    PD2_EDFA2_OUT_PWR,
    PD2_EDFA2_OUT_SIG,
    PD3_EDFA2_PADDINGVOA_OUT_PWR,
    PD4_EDFA2_LINE_TX_RFL_PWR,
    PD5_EDFA1_IN_PWR,
    PD6_EDFA1_OUT_PWR,
    PD6_EDFA1_OUT_SIG,
    PD7_EDFA1_PAVOA_OUT_PWR,
    PD8_EDFA1_LINE_TX_RFL_PWR,
    PD9_EDFA2_DFB_IN_PWR,
    PD10_EDFA2_DFB_OUT_PWR,
    PD11_EDFA1_DFB_IN_PWR,
    PD12_EDFA1_DFB_OUT_PWR,
    PD13_EDFA2_OSC_TX_OUT_PWR,       /* E2 PM1 */
    PD14_EDFA1_OSC_RX_VOA_IN_PWR,
    PD15_EDFA1_OSC_RX_VOA_OUT_PWR,
    PD16_EDFA2_OCM_VOA_IN_PWR,
    PD17_EDFA2_OCM_VOA_OUT_PWR,
    EDFA2_INTER_INPUT_PWR,
    EDFA2_INTER_OUTPUT_PWR,
    EDFA1_INTER_INPUT_PWR,
    EDFA1_INTER_OUTPUT_PWR,
    RFL2_DeltaPwr,          /* e2. DeltaRfl */
    RFL1_DeltaPwr,          /* e1. DeltaRfl */
    MAX_OLT_PD_THR,
}OLT_PD_PWR_E;
#endif

#if defined __HCF__ || defined __OCS__ || defined __OTDR__
typedef enum
{
    PD1_EDFA1_IN_PWR,
    PD2_EDFA1_OUT_PWR,
    PD2_EDFA1_OUT_SIG,
    PD3_EDFA1_LINE_OUT_PWR,
    PD4_EDFA1_LINE_RFL_PWR,

    PD5_EDFA2_IN_PWR,
    PD6_EDFA2_OUT_PWR,
    PD6_EDFA2_OUT_SIG,
    PD7_EDFA2_LINE_OUT_PWR,
    PD8_EDFA2_LINE_RFL_PWR,

    PD12_EDFA1_DFB_OUT_PWR,          /* MUX R */
    PD9_EDFA1_DFB_RFL_PWR,            /* MUX R RFL */
    PD10_EDFA2_DFB_OUT_PWR,          /* DEMUX T */
    PD11_EDFA2_DFB_RFL_PWR,           /* DEMUX T RFL */

    PD13_EDFA2_OSC_TX_OUT_PWR,       /* E2 PM1 OSC add */
    PD14_EDFA2_OSC_RX_VOA_IN_PWR,    /* OSC drop */
    PD15_EDFA2_OSC_RX_VOA_OUT_PWR,   /* OSC Rx */

    PD18_SAFTY_RX_PWR,
    PD20_SAFTY_TX_PWR,
    PD19_OTDR_R_PWR,

    RFL1_DeltaPwr,          /* e1. DeltaRfl */
    RFL2_DeltaPwr,          /* e2. DeltaRfl */

    PD3_EDFA1_LINE_OUT_SIG,
    PD7_EDFA2_LINE_OUT_SIG,
    DFA3_IN_PWR,           /* OSC Tx Pwr, OSC EDFA In Pwr */
    MAX_OLT_PD_THR,
}OLT_PD_PWR_E;

typedef enum
{
    BA_IN_PWR = PD1_EDFA1_IN_PWR,
    BA_OUT_PWR = PD2_EDFA1_OUT_PWR,
    BA_OUT_SIG = PD2_EDFA1_OUT_SIG,
    BA_LINE_OUT_PWR = PD3_EDFA1_LINE_OUT_PWR,
    BA_LINE_OUT_SIG = PD3_EDFA1_LINE_OUT_SIG,
    BA_LINE_RFL_PWR = PD4_EDFA1_LINE_RFL_PWR,

    PA_IN_PWR = PD5_EDFA2_IN_PWR,
    PA_OUT_PWR = PD6_EDFA2_OUT_PWR,
    PA_OUT_SIG = PD6_EDFA2_OUT_SIG,
    PA_LINE_OUT_PWR = PD7_EDFA2_LINE_OUT_PWR,
    PA_LINE_OUT_SIG = PD7_EDFA2_LINE_OUT_SIG,
    PA_LINE_RFL_PWR = PD8_EDFA2_LINE_RFL_PWR,

    BA_DFB_OUT_PWR = PD12_EDFA1_DFB_OUT_PWR,
    BA_DFB_RFL_PWR = PD9_EDFA1_DFB_RFL_PWR,
    PA_DFB_OUT_PWR = PD10_EDFA2_DFB_OUT_PWR,
    PA_DFB_RFL_PWR = PD11_EDFA2_DFB_RFL_PWR,

    OSC_TX_PWR = DFA3_IN_PWR,
    OSC_ADD_PWR = PD13_EDFA2_OSC_TX_OUT_PWR,
    OSC_DROP_PWR = PD14_EDFA2_OSC_RX_VOA_IN_PWR,    /* OSC drop */
    OSC_RX_PWR = PD15_EDFA2_OSC_RX_VOA_OUT_PWR,   /* OSC Rx */

    SAFTY_RX_PWR = PD18_SAFTY_RX_PWR,
    SAFTY_TX_PWR = PD20_SAFTY_TX_PWR,
    OTDR_R_PWR = PD19_OTDR_R_PWR,

    BA_LINE_RFL_RATIO = RFL1_DeltaPwr,
    PA_LINE_RFL_RATIO = RFL2_DeltaPwr,          /* e2. DeltaRfl */

    OCM_VOA_IN_PWR,
    OCM_VOA_OUT_PWR,
}OLT_PD_PWR_E_GENERIC;

typedef enum OA_THR_PORT_en {
    OA_THR_PORT_LINE_R = PA_IN_PWR,
    OA_THR_PORT_OSC_DROP = OSC_DROP_PWR,
    OA_THR_PORT_OSC_RX = OSC_RX_PWR,
    OA_THR_PORT_MUX_T = PA_LINE_OUT_PWR,
    OA_THR_PORT_MUX_R = BA_IN_PWR,
    OA_THR_PORT_OSC_ADD = OSC_ADD_PWR,
    OA_THR_PORT_OSC_TX = OSC_TX_PWR,
    OA_THR_PORT_LINE_T = BA_LINE_OUT_PWR,
    OA_THR_PORT_SAFTY_R = SAFTY_RX_PWR,
    OA_THR_PORT_SAFTY_T = SAFTY_TX_PWR,
    OA_THR_PORT_PA_RFL = PA_LINE_RFL_RATIO,
    OA_THR_PORT_BA_RFL = BA_LINE_RFL_RATIO,
    OA_THR_PORT_MAX = 8
}OA_THR_PORT_EN;
#else
typedef enum OA_THR_PORT_en {
    OA_THR_PORT_LINE_R = 0,      /* PA PD1 */
    OA_THR_PORT_OSC_DROP,        /* PA PD10 */
    OA_THR_PORT_OSC_RX,          /* PA READ FROM OSC RX */
    OA_THR_PORT_MUX_T,           /* PA PD2 */
    OA_THR_PORT_MUX_R,           /* BA PD3 */
    OA_THR_PORT_OSC_ADD,         /* BA PD15 */
    OA_THR_PORT_OSC_TX,          /* BA READ FROM OSC TX */
    OA_THR_PORT_LINE_T,          /* BA PD4 */
    OA_THR_PORT_MAX
}OA_THR_PORT_EN;
#endif

typedef enum
{
    LINE_R_LOS,
    LINE_R_POWER_IN_HI,
    LINE_R_POWER_IN_LOW,
    OSC_R_DROP_LOS,
    OSC_R_DROP_POWER_IN_HI,
    OSC_R_DROP_POWER_IN_LOW,
    OSC_R_RX_LOS,
    OSC_R_RX_POWER_IN_HI,
    OSC_R_RX_POWER_IN_LOW,
    MUX_T_LOS,
    MUX_T_POWER_OUT_HI,
    MUX_T_POWER_OUT_LOW,
    MUX_R_LOS,
    MUX_R_POWER_IN_HI,
    MUX_R_POWER_IN_LOW,
    OSC_T_ADD_LOS,
    OSC_T_ADD_POWER_OUT_HI,
    OSC_T_ADD_POWER_OUT_LOW,
    OSC_T_TX_LOS,
    OSC_T_TX_POWER_OUT_HI,
    OSC_T_TX_POWER_OUT_LOW,
    LINE_T_LOS,
    LINE_T_POWER_OUT_HI,
    LINE_T_POWER_OUT_LOW,
    OLT_PD_ALM_MAX
}OLT_PD_ALM_E;

#if 0
typedef union EDFA_LASER_SAFETY_st
{
    struct
    {
        uint8_t bInterLock           : 1;                                               /* Bit15:                                        */
        uint8_t bHcfLcPresent        : 1;                                               /* Bit14:                                        */
        uint8_t bAdminKey            : 1;                                               /* Bit13:                                        */
        uint8_t bPsControl           : 1;                                               /* Bit12:                                        */
        uint8_t bLaserSafetyRxLos    : 1;                                               /* Bit11:                                        */
        uint8_t bBaInLos             : 1;                                               /* Bit10:                                        */
        uint8_t bPaInLos             : 1;                                               /* Bit9:                                         */
        uint8_t bOscLinkDown         : 1;                                               /* Bit8:                                         */

        uint8_t bBaPumpStatus        : 1;                                               /* Bit7: BaPump                                  */
        uint8_t bShutterSatus        : 1;                                               /* Bit6: shutterStatus                           */
        uint8_t rev                  : 6;
    };
    uint16_t usLaserSafety;
}EDFA_LASER_SAFETY_ST;
#else
#if 0
/*
   14.1.16 Register 0x2048  safety_pin_in
   31:20   R   0   Reserved.
   19:18   R   0   hp_admin_key  00: admin key = -1; 01: admin key = 1; 10: admin key = 0
   17:16   R   0   otdr_admin_key  00: admin key = -1; 01: admin key = 1; 10: admin key = 0
   15      R   0   hp_hcf_lc_presence 1: presence
   14      R   0   otdr_hcf_lc_presence 1: presence
   13      R   0   hp_hcf_lc_door_lock 1:lock, 0:unlock
   12      R   0   otdr_hcf_lc_door_lock 1:lock, 0:unlock
   11:0    R   0   otdr_e2s_status_1, otdr_e2s_status_2, e2s_status_1, e2s_status_2,
                otdr_key_status1, otdr_key_status, hp_key_status1, hp_key_status,
                scc_edfa_ba_disable, remote_status, wdt, edfa_hard_rst_n
 */
typedef union PIN_IN_st
{
    struct
    {
        uint16_t edfa_hard_rst_n               : 1;                                     /* Bit0:                                        */
        uint16_t wdt                           : 1;                                     /* Bit1:                                        */
        uint16_t remote_status                 : 1;                                     /* Bit2:                                        */
        uint16_t scc_edfa_ba_disable           : 1;                                     /* Bit3:                                        */

        uint16_t hp_key_status                 : 1;                                     /* Bit4:                                        */
        uint16_t hp_key_status1                : 1;                                     /* Bit5:                                        */
        uint16_t otdr_key_status               : 1;                                     /* Bit6:                                        */
        uint16_t otdr_key_status1              : 1;                                     /* Bit7:                                        */

        uint16_t e2s_status_2                  : 1;                                     /* Bit8:                                        */
        uint16_t e2s_status_1                  : 1;                                     /* Bit9:                                        */
        uint16_t otdr_e2s_status_2             : 1;                                     /* Bit10:                                       */
        uint16_t otdr_e2s_status_1             : 1;                                     /* Bit11:                                       */

        uint16_t otdr_hcf_lc_door_lock         : 1;                                     /* Bit12:                                       */
        uint16_t hp_hcf_lc_door_lock           : 1;                                     /* Bit13:                                       */
        uint16_t otdr_hcf_lc_presence          : 1;                                     /* Bit14:                                       */
        uint16_t hp_hcf_lc_presence            : 1;                                     /* Bit15:                                       */

        uint16_t otdr_admin_key                : 2;                                     /* Bit16~17:                                    */
        uint16_t hp_admin_key                  : 2;                                     /* Bit18~19:                                    */

        uint16_t Reserved                      : 12;                                    /* Bit20~31:                                    */

    };
    uint32_t uiData;
}PIN_IN_ST;
#endif
typedef union PIN_IN_st
{
    struct
    {
        uint16_t wdt                           : 1;                                     /* Bit0:                                        */
        uint16_t edfa_hard_rst_n               : 1;                                     /* Bit1:                                        */
        uint16_t scc_edfa_ba_disable           : 1;                                     /* Bit2:                                        */
        uint16_t reserved3                     : 1;                                     /* Bit3:                                        */

        uint16_t e2s_status_2                  : 1;                                     /* Bit4:                                        */
        uint16_t e2s_status_1                  : 1;                                     /* Bit5:                                        */
        uint16_t hp_hcf_lc_presence            : 1;                                     /* Bit6:                                        */
        uint16_t reserved7                     : 1;                                     /* Bit7:                                        */

        uint16_t remote_status_mid             : 1;                                     /* Bit8:                                        */
        uint16_t remote_status                 : 1;                                     /* Bit9:                                        */
        uint16_t lc_status_mid                 : 1;                                     /* Bit10:                                       */
        uint16_t lc_status_vld                 : 1;                                     /* Bit11: LC Door sensor 1? present             */

        uint16_t hp_key_status                 : 1;                                     /* Bit12:                                       */
        uint16_t hp_key_status1                : 1;                                     /* Bit13:                                       */
        uint16_t hp_admin_key                  : 2;                                     /* Bit14~15:                                       */

        uint16_t reserved                      : 16;                                    /* Bit16~31:                                    */

    };
    uint32_t uiData;
}PIN_IN_ST;

/*
   14.1.17 Register 0x204c  safety_pin_out
   31:12   R   0   Reserved.
   15:4    R   0   motor_2b, motor_2a, motor_1b, motor_1b,
                cpld_pump_en, pump9_cpld_dis, pump8_cpld_dis, pump7_cpld_dis,
                motor_nsleep2, motor_nsleep1, edfa_ba_disable, manual_reset_n?
   3:0     R   0   ba_dis_status_num
 */
#if 0
typedef union PIN_OUT_st
{
    struct
    {
        uint16_t ba_dis_status_num             : 4;                                     /* Bit0~3:                                      */

        uint16_t manual_reset_n                : 1;                                     /* Bit4:                                        */
        uint16_t edfa_ba_disable               : 1;                                     /* Bit5:                                        */
        uint16_t motor_nsleep1                 : 1;                                     /* Bit6:                                        */
        uint16_t motor_nsleep2                 : 1;                                     /* Bit7:                                        */

        uint16_t pump7_cpld_dis                : 1;                                     /* Bit8:                                        */
        uint16_t pump8_cpld_dis                : 1;                                     /* Bit9:                                        */
        uint16_t pump9_cpld_dis                : 1;                                     /* Bit10:                                       */
        uint16_t cpld_pump_en                  : 1;                                     /* Bit11:                                       */

        uint16_t motor_1a                      : 1;                                     /* Bit12:                                       */
        uint16_t motor_1b                      : 1;                                     /* Bit13:                                       */
        uint16_t motor_2a                      : 1;                                     /* Bit14:                                       */
        uint16_t motor_2b                      : 1;                                     /* Bit15:                                       */

        uint16_t Reserved                      : 16;                                    /* Bit16~31:                                    */

    };
    uint32_t uiData;
}PIN_OUT_ST;
#else
typedef union PIN_OUT_st
{
    struct
    {
        uint16_t ba_dis_status_num             : 4;                                     /* Bit0~3:                                      */

        uint16_t manual_reset_n                : 1;                                     /* Bit4:                                        */
        uint16_t edfa_ba_disable               : 1;                                     /* Bit5:                                        */
        uint16_t motor_nsleep1                 : 1;                                     /* Bit6:                                        */
        uint16_t pump10_cpld_dis               : 1;                                     /* Bit7:                                        */

        uint16_t pump7_cpld_dis                : 1;                                     /* Bit8:                                        */
        uint16_t pump8_cpld_dis                : 1;                                     /* Bit9:                                        */
        uint16_t pump9_cpld_dis                : 1;                                     /* Bit10:                                       */
        uint16_t cpld_pump_en                  : 1;                                     /* Bit11:                                       */

        uint16_t motor_1a                      : 1;                                     /* Bit12:                                       */
        uint16_t motor_1b                      : 1;                                     /* Bit13:                                       */
        uint16_t reserved14                    : 1;                                     /* Bit14:                                       */
        uint16_t reserved15                    : 1;                                     /* Bit15:                                       */

        uint16_t safety_led_blue               : 1;                                     /* Bit16:                                       */
        uint16_t safety_led_green              : 1;                                     /* Bit17:                                       */
        uint16_t safety_led_red                : 1;                                     /* Bit18:                                       */

        uint16_t reserved                      : 13;                                    /* Bit19~31:                                    */

    };
    uint32_t uiData;
}PIN_OUT_ST;

#endif
typedef struct EDFA_LASER_SAFETY_st
{
    PIN_IN_ST stPinIn;
    PIN_OUT_ST stPinOut;
}EDFA_LASER_SAFETY_ST;
#endif

typedef struct
{
    bool bIsInService;

    uint8_t ucModeActual;
    int16_t sInPower;
    int16_t sOutSigCPower;
    int16_t sOutSigLPower;
    int16_t sOutTotalPower;

    int16_t sPortOutSigCPower;
    int16_t sPortOutSigLPower;
    int16_t sPortOutTotalPower;

    int16_t sMsaInPower;
    int16_t sMsaOutPower;

    int16_t sBackRefPower;
    int16_t sBackRefRatio;

    int16_t sGainActual;
    int16_t sTiltActual;

    int16_t asVoaAttnActual[8]; /* 4 to 8 for olsp */

    int16_t asCaseTemp[4];
    int16_t asHeaterTemp[4];
    int16_t asPumpTemp[8];
    int32_t asPumpCurrent[8];

    int16_t asExtPdPower[32];  /* 16 to 32 for olsp */
    bool abDfbEnable;
    bool abApsdStatus;
    bool abApsdEvent;
    bool abApsdEnable;
#if defined __HCF__ || defined __OCS__
    uint16_t uiOtdrSwitchInEx;
    uint16_t uiOtdrSwitch;
#else
    uint32_t uiOtdrSwitch;
#endif
    uint32_t uiDFBPwr;
    int16_t sDfbInPower2;
    int16_t sDfbOutPower2;
    int16_t sDfbInPower1;
    int16_t sDfbOutPower1;
    int16_t sLaserSafetyTxPower;
    int16_t sLaserSafetyRxPower;
    EDFA_LASER_SAFETY_ST stLaserSafety;
    CMN_THR_HYS_INFO_ST astThrHys[OA_THR_PORT_MAX][OA_THR_TYPE_MAX];
    uint32_t uiAutogainSt;    /* 0:Disable_Mode,1:Normal_Mode,2:Safety_Mode,3:Ready_Mode */
    uint32_t uiAutogainMode;  /* 1: Manual Mode, 0: Auto Mode Default:BA: Manual Mode PA: Auto Mode */
    uint16_t usRemoteLockEn;
    uint16_t usAdminKey;
    uint32_t uiPanelSafe;
    uint32_t uiApcTargetPwr;
    uint32_t uiReadyPwr;
} COaCurData;

typedef struct
{
    /* uint32_t uiOtdrSwitch; */
    uint32_t uiSccUpgSta;
    uint32_t uiTestLedSta;
    uint32_t uiLedAlmSta;
    uint8_t aucLedSta[OA_LED_MAX_NUM];      /* LED0 ACT, LED1 ALM, LED2 OTDR, LED3  OCM, LED4 CLINT(Mux R), LED5 LINE(Line R) */
    uint32_t aucAlmMask[2];
    /* Add status for shutter component, block/pass */
    uint8_t ucShutterSta;
    uint8_t reserve[3];
} COaAnnexData;


typedef struct
{
    COaAlarm stOaAlm;
    COaCurData stOaCurData;
    COscStatusData stOscCurData;
    uint8_t aucDbgBuf[2048];
    COaAnnexData stOaAnnexData;
} COaStatusData;


typedef enum OA_PUMP_CURRENT_INDEX_en
{
    PUMP_CURRENT_INDEX1 = 0,
    PUMP_CURRENT_INDEX2,
    PUMP_CURRENT_INDEX3,
    PUMP_CURRENT_INDEX4,
    PUMP_CURRENT_MAX
}OA_PUMP_CURRENT_INDEX_EN;

typedef enum OA_MODE_en
{
    OA_MODE_ACC = 0,
    OA_MODE_APC,
    OA_MODE_AGC,
} OA_MODE_EN;


typedef enum OA_EVENT_LOG_INDEX_en
{
    OA_EVENT_LOG_LOST_TIME = 0,
    OA_EVENT_LOG_LOST_PD,
    OA_EVENT_LOG_LINKDOWN_TIME,
    OA_EVENT_LOG_APSD_TIME,
    OA_EVENT_LOG_MAX
}OA_EVENT_LOG_INDEX_EN;

typedef struct
{
    uint32_t uiLostTimeCount;
    uint32_t uiLostPD;
    uint32_t uiLinkDownTimeCount;
    uint32_t uiApsdTimeCount;
} COaEventLogData;

typedef struct
{
    COaEventLogData stOaEventLogData[4];
    uint32_t uiBaLostTimeCount;
    uint8_t rev[188];
} COaEventLogAllData;

/* hp_admin_key  00: admin key = -1; 01: admin key = 1; 10: admin key = 0 */
enum ADMIN_KEY
{
    ADMIN_KEY_00,
    ADMIN_KEY_01,
    ADMIN_KEY_02,
    ADMIN_KEY_MAX
};
#define DFB_OUTPUT_DEFAULT_POWER 5
#ifdef __cplusplus
};
#endif

#endif




