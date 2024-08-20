/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Cmd.h
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

enum EOid
{
    OID_SET_BEGIN = 0x0000,

    /* Hal */
    OID_HAL_REGISTER_DEV = 0x0001,
    OID_HAL_UNREGISTER_DEV,
    OID_HAL_DBG_LOOP,
    OID_HAL_DBG_LOOPINTERVAL,
    OID_HAL_DBG_IDLE,

    /* slot */
    OID_SLOT_SET_LED_STATE = 0x0010,
    OID_SLOT_SET_MFG,
    OID_SLOT_SET_CALI,
    OID_SLOT_CLEAR_PLUG_FLAG,
    OID_SLOT_RESET,
    OID_SLOT_SET_ONLINE,
    OID_SLOT_EEPROM_WRITE,
    OID_SLOT_EEPROM_READ,

    /* BOARD */
    OID_BOARD_SET_LED_STATE = 0x0100,
    OID_BOARD_SET_CHASSIS_MFG,
    OID_BOARD_SET_CHASSIS_CALI,
    OID_BOARD_SET_CHASSIS_CFG,
    OID_BOARD_GET_CHASSIS_CFG,
    OID_BOARD_GET_SCC_CFG,
    OID_BOARD_SET_SCC_MFG,
    OID_BOARD_SET_SCC_CFG,
    OID_BOARD_GET_UPG_INFO,
    OID_BOARD_SET_UPG_INFO,
    OID_BOARD_GET_BDL_HEAD,
    OID_BOARD_SET_BDL_HEAD,
    OID_BOARD_I2C_TEST,
    OID_BOARD_SET_LED_TESTMODE,
    OID_BOARD_SET_LED_LOCATION_TESTMODE,
    /* FAN */
    OID_FAN_SET_MFG,
    OID_FAN_SET_SPEED,
    OID_FAN_SET_LED,
    OID_FAN_SET_CONTROLLEREEP,

    /* Logic */
    OID_LOGIC_READ_REG,
    OID_LOGIC_WRITE_REG,
    OID_LOGIC_READ_REG_SLC,
    OID_LOGIC_WRITE_REG_SLC,
    OID_LOGIC_SET_RESTTYPE,
    OID_LOGIC_CFG_WATCHDOG,
    OID_LOGIC_CFG_APSD,

    OID_LOGIC_RESET,
    OID_LOGIC_UPGRADE,

    /* Spi */
    OID_SPI_READ_WRITE_REG,

    OID_BOARD_SET_CHASSIS_CONFIG_TYPE,
    OID_LOGIC_SET_LOG_LEVEL,
    OID_FUNC_GET_DEV_TYPE,

    OID_FAN_SET_RESET_CAUSE,   /* OC2A-286: ONLY used in ueno project */

    /* OA */
    OID_OA_MIN = 0x0200,
    OID_OA_CHECK_INDEX,
    OID_OA_SET_GAINRANG,
    OID_OA_SET_GAIN,
    OID_OA_SET_ATTN,
    OID_OA_SET_TILT,
    OID_OA_GET_STATUS,
    OID_OA_SET_RX_LOS_THR,
    OID_OA_SET_OUTPUT_LOS_THRESHOLD,
    OID_OA_SET_MSA_INPUT_LOS_THR,
    OID_OA_SET_MSA_OUTPUT_LOS_THR,
    OID_OA_SET_PUMP_ENABLE,
    OID_OA_SET_AUTO_LOS_ENABLE,
    OID_OA_SET_MODE,
    OID_OA_SET_PLIM_ENABLE,
    OID_OA_SET_PLIM_POWER,
    OID_OA_SET_GLIM_POWER,
    OID_OA_SET_APSD_ENABLE,
    OID_OA_SET_APC_POWER,
    OID_OA_GET_MFG,
    OID_OA_GET_ALL_CFG,
    OID_OA_SET_APR_ENABLE,
    OID_OA_RESET,
    OID_OA_UPGRADE,
    OID_OA_SWAP,
    OID_OA_COMMIT,
    OID_OA_DBG_READ,
    OID_OA_DBG_WRITE,
    OID_OA_DBG_SET_COMM_FAIL,
    OID_OA_DBG_CLEAR_COMM_FAIL,
    OID_OA_DBG_SET_MODULE_ALARM,
    OID_OA_DBG_CLEAR_MODULE_ALARM,
    OID_OA_GET_DEBUG_INFO,
    OID_OA_SET_DFB_ENABLE,
    OID_OA_SET_OSC_ENABLE,
    OID_OA_SET_OTDR_SWITCH_POS,
    OID_OA_SET_OSC_ADD_THR,
    OID_OA_SET_OSC_DROP_THR,
    OID_OA_SET_OSC_RX_THR,
    OID_OA_SET_UPG_STATE,
    OID_OA_SET_LED_STATE,
    OID_OA_GET_CALI,
    OID_OA_SET_OTDR_CALI,
    OID_OA_SET_OCM_CALI,
    OID_OA_SET_LED_ALM,
    OID_OA_SET_MANUAL_VOA_ENABLE,
    OID_OA_SET_PADDING_VOA_CONFIG_POWER,
    OID_OA_SET_MANUAL_GAIN_ENABLE,
    OID_OA_SET_THR,
    OID_OA_SET_PUMP_CURRENT,
    OID_OA_SET_ALM_MASK,
    OID_OA_GET_EVENTLOG,
    OID_OA_GET_MCU_DEBUG_INFO,
    OID_OA_SET_SHUTTER_STATE,
    OID_OA_GET_SHUTTER_STATE,
    OID_OA_SET_EDFA_READY_TRIGGER,
    OID_OA_SET_EDFA_AUTOGAIN_MODE,
    OID_OA_SET_REMOTE_INTERLOCK_ENABLE,
    OID_OA_SET_DUMMY_ADMIN_KEY_ENABLE,
    OID_OA_SET_PANEL_SAFE_CMPT_ENABLE,
    OID_OA_SET_OSC_AMP_ENABLE,
    OID_OA_SET_EDFA_READY_POWER,
    /* OCM */
    OID_OCM_GET_POWER = 0x0300,
    OID_OCM_SET_WAVEPLAN_FORMAT_CB,
    /* OID_OCM_GET_PORT_ENABLE, */
    /* OID_OCM_SET_PORT_ENABLE, */
    OID_OCM_RESET,
    OID_OCM_UPGRADE,
    OID_OCM_GET_REALTIME_CH_POWER,
    OID_OCM_DBG_SET_COMM_FAIL,
    OID_OCM_DBG_CLEAR_COMM_FAIL,
    OID_OCM_COMMIT,
    OID_OCM_SET_WAVEPLAN,

    /* WSS */
    OID_WSS_SET_ONE_CH_CFG = 0x0400,
    OID_WSS_SET_CFG,
    OID_WSS_SET_TEMP_ALARM_THR,
    OID_WSS_SET_BAUD_RATE,
    OID_WSS_SET_WAVEPLAN,
    OID_WSS_GET_ALL_CFG,
    OID_WSS_RESET,
    OID_WSS_UPGRADE,
    OID_WSS_DBG_SET_COMM_FAIL,
    OID_WSS_DBG_CLEAR_COMM_FAIL,

    /* DGE */
    OID_DGE_SET_CURVER = 0x0500,

    /* OSC */
    OID_OSC_GET_POWER = 0x0600,
    OID_OSC_SET_LASER_ENABLED,
    OID_OSC_SET_TX_THRESHOLD,
    OID_OSC_SET_RX_THRESHOLD,
    OID_OSC_SET_DROP_THRESHOLD,
    OID_OSC_RESET,
    OID_OSC_SET_TX_TARGET_POWER,

    /*VOA*/
    OID_VOA_SET_ATTENUATION = 0x0700,
    OID_VOA_GET_INFO,

    /*OPS*/
    OID_OPS_SET_MODE = 0x0800,
    OID_OPS_SET_WTR,
    OID_OPS_SET_REVERTIVE,
    OID_OPS_SET_HOLDOFFTIME,
    OID_OPS_SET_RELATIVE_THR,
    OID_OPS_SET_RELATIVE_OFFSET,
    OID_OPS_SET_SWITCH_THR,
    OID_OPS_GET_SWITCH_THR,
    OID_OPS_SET_SWITCH_HYS,
    OID_OPS_GET_SWITCH_HYS,
    OID_OPS_SET_PORT_POWER_THR,
    OID_OPS_SET_PORT_POWER_HYS,
    OID_OPS_GET_HISTORY_DATA,
    OID_OPS_RESET,
    OID_OPS_UPGRADE,

    OID_OPS_GET_WSS_PORT_POWER,
    OID_OPS_SET_WSS_PORT_POWER_THR,
    OID_OPS_SET_WSS_PORT_POWER_HYS,
    OID_OPS_GET_WSS_PORT_HISTORY_POWER,
    OID_OPS_SET_WSS_LED_MODE,
    OID_OPS_GET_OSC_SWITCH_POS,
    OID_OPS_SET_OSC_SWITCH_POS,
    OID_OPS_SET_LED_STATUS,
    OID_OPS_SET_LED_MODE,
    OID_OPS_CLEAR_ALARM_DURATION,
    OID_OPS_DBG_SET_COMM_FAIL,
    OID_OPS_DBG_CLEAR_COMM_FAIL,
    OID_OPS_GET_ALL_CFG,

    OID_OPS_SET_REMOTE_STATUS,
    OID_OPS_GET_REQUEST_STATUS,
    OID_OPS_SET_SWITCHING_TYPE,
    OID_OPS_SET_CONN_PD_THR,
    OID_OPS_SET_CONN_PD_HYS,
    OID_OPS_SET_ALM_MASK,
    OID_OPS_GET_EVENTLOG,
    /* MCS */
    OID_MCS_SET_PORT_SW_POS = 0x0900,

    /*TDCM*/
    OID_TDCM_SET_FREQUENCY = 0x0A00,
    OID_TDCM_SET_DISPERSION,
    OID_TDCM_SET_ENABLE,
    OID_TDCM_RESET,

    /* OTDR */
    OID_OTDR_START_SCAN = 0x0B00,
    OID_OTDR_SET_PARA,
    OID_OTDR_SET_UIOR,
    OID_OTDR_SET_MTIME,
    OID_OTDR_SET_THR,
    OID_OTDR_GET_TEMP,
    OID_OTDR_RESET,
    OID_OTDR_UPGRADE,
    OID_OTDR_DBG_READ,
    OID_OTDR_DBG_WRITE,
    OID_OTDR_DBG_SET_COMM_FAIL,
    OID_OTDR_DBG_CLEAR_COMM_FAIL,
    OID_OTDR_SET_SCAN_MODE,
    OID_OTDR_SET_START_OFFSET,
    OID_OTDR_GET_START_OFFSET,

    /* EXTEND */
    OID_EXTEND_SWITCH_POS = 0x0C00,
    OID_EXTEND_SFP,
    OID_EXTEND_SET_VOA,
    OID_EXTEND_RESET,
    OID_EXTEND_UPGRADE,
    OID_EXTEND_DBG_READ,
    OID_EXTEND_DBG_WRITE,
    OID_EXTEND_DBG_SET_COMM_FAIL,
    OID_EXTEND_DBG_CLEAR_COMM_FAIL,

    /* OCS */
    OID_OCS_SET_ONE_SWITCH_CHANNEL = 0x0D00,
    OID_OCS_SET_SWITCH_CHANNELS,
    OID_OCS_SET_LAMP_TEST_STATE,
    OID_OCS_RESET,
    OID_OCS_UPGRADE,
    OID_OCS_DBG_SET_COMM_FAIL,
    OID_OCS_DBG_CLEAR_COMM_FAIL,
    OID_OCS_COMMIT,

    /*Debug*/
    OID_UART_READ_DATA = 0x0F00,
    OID_UART_WRITE_DATA,
    OID_SPICOM_WRITE_READ_DATA,
    OID_SPIFLASH_READ_DATA,
    OID_SPIFLASH_WRITE_DATA,
    OID_IICCOM_READ_DATA,
    OID_IICCOM_WRITE_DATA,
    OID_MDIO_READ_DATA,
    OID_MDIO_WRITE_DATA,
    OID_NET_SWITCH_READ_DATA,
    OID_NET_SWITCH_WRITE_DATA,
    OID_NET_SWITCH_PHY_READ_DATA,
    OID_NET_SWITCH_PHY_WRITE_DATA,
    OID_EEPROM_READ_DATA,
    OID_EEPROM_WRITE_DATA,
    OID_BIOS_READ_DATA,
    OID_BIOS_WRITE_DATA,
    OID_I2C_TEST,

    DBG_OID_STOP_LOOP = 0xEE00,

    OID_SET_END = 0xFFFF,

};

struct CCmdNoData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
};

struct CCmd1KBufData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiLen;
    uint8_t aucBuf[1024];
};


struct CCmdData
{
    uint32_t uiIdx;
    int32_t iVal;
    uint32_t uiSubIdx;
};

struct CCmdKeyData
{
    uint32_t uiIdx;
    int32_t iKey;
    int32_t iVal;
    uint32_t uiSubIdx;
};

struct CCmdSVal
{
    uint32_t uiIdx;
    int16_t sVal;
    uint32_t uiSubIdx;
};

struct CCmdFloatData
{
    uint32_t uiIdx;
    float fVal;
    uint32_t uiSubIdx;
};

struct CCmdBufData
{
    uint32_t uiIdx;
    uint8_t aucBuf[128];
    uint32_t uiSubIdx;
};

struct CCmdUnRegDev
{
    uint32_t uiDevType;
    uint32_t uiDevIdx;
};

struct CCmdThrData
{
    uint32_t uiIdx;
    int16_t sThr;
    int16_t sHys;
    uint32_t uiSubIdx;
};

struct CCmdThrCommonData
{
    uint32_t uiIdx;
    uint32_t uiPortIdx;
    uint32_t uiType;
    int16_t sThr;
    int16_t sHys;
    uint32_t uiSubIdx;
};

struct CCmdPowerOnFlag
{
    uint32_t uiFlag;
};

struct CCmdMfgData
{
    uint32_t uiIdx;
    char acFilePath[128];
};

struct CCmdCfgData
{
    uint32_t uiAddrSlice;
    uint32_t uiSize;
    char acFilePath[128];
};


struct CCmdCaliData
{
    uint32_t uiIdx;
    char acFilePath[128];
};

struct CCmdLogicData
{
    uint32_t uiIdx;
    uint64_t ulAddr;
    uint64_t ulLen;
    uint64_t ulSubAddr;
    uint8_t aucVal[256];
};


struct CCmdDbgData
{
    uint32_t uiIdx;
    uint32_t uiLen;
    uint8_t aucBuf[1024];
};

struct CCmdWssCfgData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiInstBegin;
    uint32_t uiInstEnd;
    CWssCfg astVal[WSS_CH_CNT_TOTAL];
};

struct CCmdWssAllCfgData
{
    uint32_t uiIdx;
    CWssAllCfg stVal;
};


struct CCmdWavePlanData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiPortId;
    uint32_t uiChCnt;
    CWavePlan astVal[WSS_CH_CNT_TOTAL];
};

struct CCmdWavePlanSliceData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiPortId;
    uint32_t uiChCnt;
    CWavePlanSlice astVal[WSS_CH_CNT_BUSINESS];
};

struct CCmdOcmWavePlanData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiPortId;
    uint32_t uiChCnt;
    STChannelGrid astVal[WSS_CH_CNT_BUSINESS];
};

struct CCmdDgeCurveData
{
    uint32_t uiIdx;
    uint32_t uiPointCnt;
    CDgePoint stVal;
};

struct CCmdMcsSwData
{
    uint32_t uiIdx;
    uint32_t uiPortIdx;
    uint32_t uiPos;
};



struct CCmdOpsData
{
    uint32_t uiIdx;
    uint32_t uiPort;
    int32_t iVal;
    uint32_t uiSubIdx;
};

struct CCmdThrHysData
{
    uint32_t uiIdx;
    uint32_t uiPort;
    uint32_t uiType;
    int32_t iVal;
    uint32_t uiSubIdx;

};

struct CCmdOtdrPara
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;

    uint32_t uiDistRange;
    uint32_t uiPulseWidth;
    double dSampling;

    uint32_t uiMTime;

    double dUIOR;

    double dRef;
    double dTLos;
    double dTEof;
    uint32_t uiScanMode;
};

struct CCmdOtdrStartOffsetData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    uint32_t uiPort;
    COtdrStartOffset stVal;
};

struct CCmdOscSwitchPos
{
    uint32_t uiIdx;
    COscSwitchPos stSwitchPos;
    uint32_t uiSubIdx;
};

struct CCmdOscSwitchPosSet
{
    uint32_t uiIdx;
    COscSwitchPosSet stSwitchPosSet;
    uint32_t uiSubIdx;
};

struct CCmdOpsRemoteStatusSet
{
    uint32_t uiIdx;
    COpsReqStatus stStatus;
    uint32_t uiSubIdx;
};

struct CCmdOcsConnectionsData
{
    uint32_t uiIdx;
    uint32_t uiSubIdx;
    COcsConnections stVal;
};

#define TIMEOUT_PERIOD            2     /* 2ms */
#define TIMEOUT_CNT               5000


