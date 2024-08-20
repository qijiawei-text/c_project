/***MODU+******************************************************************************************/
/* Copyright(C), 2017, OPLINK Tech.Co.,Ltd                                                        */
/* FileName    : OcmComm.h                                                                        */
/* Description : The header file of ocm.                                                          */
/* History     :                                                                                  */
/*     [Author]      [Date]          [Version]        [Description]                               */
/* [1]                                                            .                               */
/***MODU-******************************************************************************************/

#ifndef __SDV_OCM_CMD_H__
#define __SDV_OCM_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif


#define PORT_SCHEDULE_THRESHOLD                1600
#define PORT_NUMBER_THRESHOLD                  8
#define CHANNEL_NUMBER_THRESHOLD               194
#ifdef __HCF__
#define RAW_DATA_NUMBER_THRESHOLD              1629
#else
#define RAW_DATA_NUMBER_THRESHOLD              1621
#endif
#define EVENT_NUMBER_THRESHOLD                 32
#define EVENT_SNAPSHOT_EQUIPMENT_FAILURE_MASK  ((uint32_t)1 << 31)
#define EVENT_SNAPSHOT_WARNING_MAJOR_MASK      ((uint32_t)1 << 30)
#define EVENT_SNAPSHOT_WARNING_MINOR_MASK      ((uint32_t)1 << 29)


typedef enum tagChPwrMode
{
    SMART_CHANNEL_POWER_MODE = 0,
    DUMB_CHANNEL_POWER_MODE
}ChPwrMode;

typedef enum tagWdlParameter
{
    WDL_COEFFICIENT_A = 0,
    WDL_COEFFICIENT_B,
    WDL_COEFFICIENT_C,
    WDL_COEFFICIENT_D,
    WDL_COEFFICIENT_NUMBER
}WdlParameter;

typedef enum tagUpgradeState
{
    UPGRADE_COMMITTED_STATE = 0,
    UPGRADE_SWITCHED_STATE,
    UPGRADE_PILOT_STATE,
    UPGRADE_STATE_NUMBER
}UpgradeState;


typedef struct tagModuleInfo
{
    char acSerialNumber[32];
    char acPartNumber[32];
    char acSubPartNumber[32];
    char acHwVersion[16];
    char acBundleVersion[16];
    char acBootVersion[16];
    char acMainVersion[16];
    char acFpgaVersion[16];
    char acMfgDate[16];
    char acCalibrationDate[16];
    uint8_t aucReserved[256 - (3 * 32) - (7 * 16)];
}STModuleInfo;


typedef struct tagCapablilityData
{
    uint32_t uRerserved;
}STCapablilityData;


typedef struct tagPortScanInfo
{
    uint8_t aucPortSchedule[PORT_SCHEDULE_THRESHOLD];
}STPortScanInfo;
#if 0
typedef struct tagChannelGrid
{
    /* Resolution is 0.001 GHz */
    uint32_t uCentralFreq;
    /* Resolution is 0.001 GHz */
    uint32_t uBandwidth;
}STChannelGrid;
#endif
typedef struct tagConfigData
{
    uint16_t usMode;
    uint16_t usInterruptEn;
    STChannelGrid astChannelPlan[CHANNEL_NUMBER_THRESHOLD];
    uint8_t ucChPwrMode;
    uint8_t aucReserved1[3];
    float afReserved2[3];
}STConfigData;


typedef struct tagAlarm
{
    uint16_t usObjectId;
    uint16_t usInstance;
    uint16_t usTimestamp;
    uint8_t ucState;
    uint8_t ucReserved;
}STAlarm;

typedef struct tagEvent
{
    uint16_t usEventId;
    uint8_t ucPhaseType;
    uint8_t ucPhaseSubType;
    uint8_t ucEventType;
    uint8_t ucEventClass;
    uint16_t usEventNumber;
    uint32_t uTimestamp;
}STEvent;

typedef struct tagPortInfo
{
    uint8_t ucIdentity;
    uint8_t ucReserved;
    /* Index of the port in scanning schedule */
    uint16_t usIndex;
}STPortInfo;

typedef struct tagModuleStatus
{
    uint32_t uEventSnapshot;
    STPortInfo stLatestRdyPort;
    STPortInfo stLatestSmpOverPort;
}STModuleStatus;


typedef struct tagUpgradeInfo
{
    /* Version Format: XX.YY.ZZZZ */
    char acActiveBdlVer[16];
    /* Version Format: XX.YY.ZZZZ */
    char acStandbyBdlVer[16];
    /* Upgrade State */
    uint8_t ucState;
    uint8_t aucReserved[64 - (2 * 16) - 1];
}STUpgradeInfo;

typedef struct tagOperationData
{
    /* Time unit is milisecond */
    uint32_t uTimestamp;
    /* Resolution is  0.1 ℃ */
    int16_t sTemperature;
    uint8_t aucReserved1[2];
    STChannelGrid astChannelPlan[CHANNEL_NUMBER_THRESHOLD];
    /* Resolution is  0.001 GHz */
    uint32_t auChannelFreq[CHANNEL_NUMBER_THRESHOLD];
    /* Resolution is  0.1 dBm */
    int16_t asChannelPwr[CHANNEL_NUMBER_THRESHOLD];
    /* Resolution is  0.1 dBm */
    int16_t asRawData[RAW_DATA_NUMBER_THRESHOLD];
    uint8_t aucReserved2[2];
    /* Upgrade information */
    STUpgradeInfo stUpgradeInfo;
}STOperationData;


typedef struct tagDebugInfo
{
    char acMiscellaneous[8192];
}STDebugInfo;


typedef struct
{
    uint8_t ucStatus;
    uint16_t usDataLen;
}__attribute__((packed)) ST_OCM_SPI_MSG_HEADER;



#ifdef __cplusplus
}
#endif

#endif
