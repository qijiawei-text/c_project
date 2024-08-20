/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCmd.h
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
#include "Queue.h"

#include "Cmd.h"
#include "HalCmd.h"
#include "SlotCmd.h"
#include "BoardCmd.h"
#include "OaCmd.h"
#include "WssCmd.h"
#include "OcmCmd.h"
#include "DgeCmd.h"
#include "McsCmd.h"
#include "OscCmd.h"
#include "OtdrCmd.h"
#include "TdcmCmd.h"
#include "OpsCmd.h"
#include "MuxCmd.h"
#include "ExtendCmd.h"
#include "OcsCmd.h"
/*************************************************/

typedef bool (*PFSETFUNC)(uint32_t uiLen, const uint8_t *cv_pucBuf);

class CDevCmdProcesser
{
public:
    static CDevCmdProcesser &GetInstance(void);

    bool CmdProc(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, const CQueueNode &c_rstNode);

private:
    const std::map<uint32_t, PFSETFUNC> mc_staCmdList
    {
        /* 0. hal cmd */
        {OID_HAL_REGISTER_DEV, CHalCmd::RegisterDev},
        {OID_HAL_UNREGISTER_DEV, CHalCmd::UnregisterDev},
        {OID_HAL_DBG_LOOP, CHalCmd::LoopEnable},
        {OID_HAL_DBG_LOOPINTERVAL, CHalCmd::LoopInterval},
        {OID_HAL_DBG_IDLE, CHalCmd::IdleEnable},

        /* 1. SLOT set cmd */
        {OID_SLOT_SET_ONLINE, CSlotCmd::SetOnline},
        {OID_SLOT_SET_LED_STATE, CSlotCmd::SetLedState},
        {OID_SLOT_SET_MFG, CSlotCmd::SetMfg},
        {OID_SLOT_SET_CALI, CSlotCmd::SetCali},
        {OID_SLOT_CLEAR_PLUG_FLAG, CSlotCmd::ClearPlugFlag},
        {OID_SLOT_EEPROM_WRITE, CSlotCmd::EepromWrite},
        {OID_SLOT_EEPROM_READ, CSlotCmd::EepromRead},
        {OID_SLOT_RESET, CSlotCmd::Reset},

        /* 2. board set cmd */
        {OID_BOARD_GET_CHASSIS_CFG, CBoardCmd::GetChassisCfg},
        {OID_BOARD_SET_CHASSIS_CFG, CBoardCmd::SetChassisCfg},
        {OID_BOARD_SET_CHASSIS_MFG, CBoardCmd::SetChassisMfg},
        {OID_BOARD_SET_CHASSIS_CONFIG_TYPE, CBoardCmd::SetChassisConfigType},

        {OID_BOARD_SET_CHASSIS_CALI, CBoardCmd::SetChassisCali},

        {OID_BOARD_GET_UPG_INFO, CBoardCmd::GetUpgInfo},
        {OID_BOARD_SET_UPG_INFO, CBoardCmd::SetUpgInfo},
        {OID_BOARD_GET_BDL_HEAD, CBoardCmd::GetBdlHead},
        {OID_BOARD_SET_BDL_HEAD, CBoardCmd::SetBdlHead},

        {OID_BOARD_GET_SCC_CFG, CBoardCmd::GetSccCfg},
        {OID_BOARD_SET_SCC_CFG, CBoardCmd::SetSccCfg},
        {OID_BOARD_SET_SCC_MFG, CBoardCmd::SetSccMfg},

        {OID_BOARD_SET_LED_STATE, CBoardCmd::SetLedState},
        {OID_BOARD_SET_LED_TESTMODE, CBoardCmd::SetLedTestMode},
        {OID_BOARD_SET_LED_LOCATION_TESTMODE, CBoardCmd::SetLedTestMode},
        {OID_FAN_SET_MFG, CBoardCmd::SetFanMfg},
        {OID_FAN_SET_SPEED, CBoardCmd::SetFanSpeed},
        {OID_FAN_SET_LED, CBoardCmd::SetFanLed},
        {OID_FAN_SET_CONTROLLEREEP, CBoardCmd::SetFanControllerEEP},
        {OID_FAN_SET_RESET_CAUSE, CBoardCmd::SetResetCauseInFan},

        /* logic */
        {OID_LOGIC_READ_REG, CBoardCmd::LogicRead},
        {OID_LOGIC_WRITE_REG, CBoardCmd::LogicWrite},
        {OID_LOGIC_READ_REG_SLC, CBoardCmd::LogicReadSlc},
        {OID_LOGIC_WRITE_REG_SLC, CBoardCmd::LogicWriteSlc},
        {OID_LOGIC_RESET, CBoardCmd::LogicReset},
        {OID_LOGIC_UPGRADE, CBoardCmd::LogicUpgrade},


        {OID_LOGIC_SET_RESTTYPE, CBoardCmd::SetResetType},
        {OID_LOGIC_CFG_WATCHDOG, CBoardCmd::CfgWatchDog},
        {OID_LOGIC_CFG_APSD, CBoardCmd::SetApsdEnable},
        {OID_LOGIC_SET_LOG_LEVEL, CBoardCmd::SetLogLevel},


        /* spi & mdio & flash & eeprom & net-switch */
        {OID_SPICOM_WRITE_READ_DATA, CBoardCmd::SpiReadWrite},
        {OID_SPIFLASH_READ_DATA, CBoardCmd::SpiFlashRead},
        {OID_SPIFLASH_WRITE_DATA, CBoardCmd::SpiFlashWrite},
        {OID_MDIO_READ_DATA, CBoardCmd::MdioRead},
        {OID_MDIO_WRITE_DATA, CBoardCmd::MdioWrite},
        {OID_NET_SWITCH_READ_DATA, CBoardCmd::NetSwitchRead},
        {OID_NET_SWITCH_WRITE_DATA, CBoardCmd::NetSwitchWrite},
        {OID_NET_SWITCH_PHY_READ_DATA, CBoardCmd::SwitchPhyRead},
        {OID_NET_SWITCH_PHY_WRITE_DATA, CBoardCmd::SwitchPhyWrite},
        {OID_EEPROM_READ_DATA, CBoardCmd::EepromRead},
        {OID_EEPROM_WRITE_DATA, CBoardCmd::EepromWrite},

        /* hardware test */
        {OID_BOARD_I2C_TEST, CBoardCmd::I2cTest},
        {OID_FUNC_GET_DEV_TYPE, CBoardCmd::GetDevType},

        /* 3. OA set cmd */
        {OID_OA_GET_MFG, COaCmd::GetMfg},
        {OID_OA_GET_ALL_CFG, COaCmd::GetCfg},
        {OID_OA_SET_GAINRANG, COaCmd::SetGainRange},
        {OID_OA_SET_GAIN, COaCmd::SetGain},
        {OID_OA_SET_TILT, COaCmd::SetTilt},
        {OID_OA_SET_MODE, COaCmd::SetMode},
        {OID_OA_SET_RX_LOS_THR, COaCmd::SetRxLosThr},
        {OID_OA_SET_PUMP_ENABLE, COaCmd::SetPumpEnable},
        {OID_OA_SET_APR_ENABLE, COaCmd::SetAprEnable},
        {OID_OA_SET_AUTO_LOS_ENABLE, COaCmd::SetAutolosEnable},
        {OID_OA_RESET, COaCmd::Reset},
        {OID_OA_UPGRADE, COaCmd::Upgrade},
        {OID_OA_SWAP, COaCmd::Swap},
        {OID_OA_COMMIT, COaCmd::Commit},
        {OID_OA_DBG_READ, COaCmd::DbgRead},
        {OID_OA_DBG_WRITE, COaCmd::DbgWrite},
        {OID_OA_DBG_SET_COMM_FAIL, COaCmd::DbgSetCommFail},
        {OID_OA_DBG_CLEAR_COMM_FAIL, COaCmd::DbgClearCommFail},
        {OID_OA_DBG_SET_MODULE_ALARM, COaCmd::DbgSetModuleAlarm},
        {OID_OA_DBG_CLEAR_MODULE_ALARM, COaCmd::DbgClearModuleAlarm},

        {OID_OA_SET_ATTN, COaCmd::SetAttn},
        {OID_OSC_SET_TX_TARGET_POWER, COaCmd::SetTarget},
        {OID_OA_GET_DEBUG_INFO, COaCmd::GetDebugInfo},
        {OID_OA_SET_DFB_ENABLE, COaCmd::SetDfbEnable},
        {OID_OA_SET_OSC_ENABLE, COaCmd::SetOscEnable},
        {OID_OA_SET_OTDR_SWITCH_POS, COaCmd::SetOtdrSwitchPos},
        {OID_OA_SET_OSC_ADD_THR, COaCmd::SetOscAddThrHys},
        {OID_OA_SET_OSC_DROP_THR, COaCmd::SetOscDropThrHys},
        {OID_OA_SET_OSC_RX_THR, COaCmd::SetOscRxThrHys},
        {OID_OA_SET_APSD_ENABLE, COaCmd::SetApsdEnable},
        {OID_OA_SET_UPG_STATE, COaCmd::SetUpgState},
        {OID_OA_SET_LED_STATE, COaCmd::SetLedState},
        {OID_OA_GET_CALI, COaCmd::GetCaliInfo},
        {OID_OA_SET_OTDR_CALI, COaCmd::SetOtdrCali},
        {OID_OA_SET_OCM_CALI, COaCmd::SetOcmCali},
        {OID_OA_SET_LED_ALM, COaCmd::SetLedAlm},
        {OID_OA_SET_MANUAL_VOA_ENABLE, COaCmd::SetManualVoaEnable},
        {OID_OA_SET_PADDING_VOA_CONFIG_POWER, COaCmd::SetPaddingVoaConfigPower},
        {OID_OA_SET_MANUAL_GAIN_ENABLE, COaCmd::SetManualGainEnable},
        {OID_OA_SET_THR, COaCmd::SetThr},
        {OID_OA_SET_PUMP_CURRENT, COaCmd::SetPumpCurrent},
        {OID_OA_SET_ALM_MASK, COaCmd::SetAlmMask},
        {OID_OA_GET_EVENTLOG, COaCmd::GetEventLog},

        {OID_OA_SET_SHUTTER_STATE, COaCmd::SetShutterState},
        {OID_OA_GET_SHUTTER_STATE, COaCmd::GetShutterState},
        {OID_OA_GET_MCU_DEBUG_INFO, COaCmd::GetMcuDebugInfo},
        {OID_OA_SET_EDFA_READY_TRIGGER, COaCmd::SetReadyTrigger},
        {OID_OA_SET_EDFA_AUTOGAIN_MODE, COaCmd::SetAutoGainMode},
        {OID_OA_SET_REMOTE_INTERLOCK_ENABLE, COaCmd::SetRemoteInterlockEnable},
        {OID_OA_SET_DUMMY_ADMIN_KEY_ENABLE, COaCmd::SetDummyAdminKey},
        {OID_OA_SET_PANEL_SAFE_CMPT_ENABLE, COaCmd::SetPanelSafeCmpt},
        {OID_OA_SET_OSC_AMP_ENABLE, COaCmd::SetOscAmpEnable},
        {OID_OA_SET_EDFA_READY_POWER, COaCmd::SetReadyPower},

        /* 4. WSS set cmd */
        {OID_WSS_SET_CFG, CWssCmd::SetCfg},
        {OID_WSS_SET_WAVEPLAN, CWssCmd::SetWavePlan},
        {OID_WSS_GET_ALL_CFG, CWssCmd::GetCfgData},
        {OID_WSS_RESET, CWssCmd::Reset},
        {OID_WSS_UPGRADE, CWssCmd::Upgrade},
        {OID_WSS_DBG_SET_COMM_FAIL, CWssCmd::DbgSetCommFail},
        {OID_WSS_DBG_CLEAR_COMM_FAIL, CWssCmd::DbgClearCommFail},

        /* 5. OCM set cmd */
        {OID_OCM_SET_WAVEPLAN_FORMAT_CB, COcmCmd::SetOcmWavePlan},
        {OID_OCM_RESET, COcmCmd::Reset},
        {OID_OCM_UPGRADE, COcmCmd::Upgrade},
        {OID_OCM_GET_REALTIME_CH_POWER, COcmCmd::GetRealTimeOsa},
        {OID_OCM_DBG_SET_COMM_FAIL, COcmCmd::DbgSetCommFail},
        {OID_OCM_DBG_CLEAR_COMM_FAIL, COcmCmd::DbgClearCommFail},
        {OID_OCM_COMMIT, COcmCmd::Commit},
        {OID_OCM_SET_WAVEPLAN, COcmCmd::SetWavePlan},

        /* 6. DGE set cmd */
        /* {OID_DGE_SET_CURVER, CDgeCmd::SetCurve}, */

        /* 7. MCS set cmd */
        /* {OID_MCS_SET_PORT_SW_POS, CMcsCmd::SetPortSwPos}, */

        /* 8. OSC set cmd */
        {OID_OSC_SET_LASER_ENABLED, COscCmd::SetLaserEnable},

        /* 9. OTDR set cmd */
        {OID_OTDR_START_SCAN, COtdrCmd::StartScan},
        {OID_OTDR_SET_PARA, COtdrCmd::SetPara},
        {OID_OTDR_SET_UIOR, COtdrCmd::SetUserIOR},
        {OID_OTDR_SET_MTIME, COtdrCmd::SetMeasuringTime},
        {OID_OTDR_SET_THR, COtdrCmd::SetThr},
        {OID_OTDR_GET_TEMP, COtdrCmd::GetTemp},
        {OID_OTDR_RESET, COtdrCmd::Reset},
        {OID_OTDR_UPGRADE, COtdrCmd::Upgrade},
        {OID_OTDR_DBG_READ, COtdrCmd::DbgRead},
        {OID_OTDR_DBG_WRITE, COtdrCmd::DbgWrite},
        {OID_OTDR_DBG_SET_COMM_FAIL, COtdrCmd::DbgSetCommFail},
        {OID_OTDR_DBG_CLEAR_COMM_FAIL, COtdrCmd::DbgClearCommFail},
        {OID_OTDR_SET_SCAN_MODE, COtdrCmd::SetScanMode},
        {OID_OTDR_SET_START_OFFSET, COtdrCmd::SetStartOffset},
        {OID_OTDR_GET_START_OFFSET, COtdrCmd::GetStartOffset},

        /* 10. TDCM set cmd */
        {OID_TDCM_SET_FREQUENCY, CTdcmCmd::SetFrequency},
        {OID_TDCM_SET_DISPERSION, CTdcmCmd::SetDispersion},
        {OID_TDCM_RESET, CTdcmCmd::Reset},

        /* 11. OPS set cmd */
        {OID_OPS_SET_MODE, COpsCmd::SetMode},
        {OID_OPS_SET_WTR, COpsCmd::SetWtr},
        {OID_OPS_SET_HOLDOFFTIME, COpsCmd::SetHoldOffTime},
        {OID_OPS_SET_REVERTIVE, COpsCmd::SetRevertive},
        {OID_OPS_SET_RELATIVE_THR, COpsCmd::SetRelativeThr},
        {OID_OPS_SET_RELATIVE_OFFSET, COpsCmd::SetRelativeOffset},
        {OID_OPS_SET_PORT_POWER_THR, COpsCmd::SetPortPowerThr},
        {OID_OPS_SET_PORT_POWER_HYS, COpsCmd::SetPortPowerHys},
        {OID_OPS_GET_HISTORY_DATA, COpsCmd::GetPortPowerHistory},
        {OID_OPS_SET_SWITCH_THR, COpsCmd::SetSwitchThr},
        {OID_OPS_GET_SWITCH_THR, COpsCmd::GetSwitchThr},
        {OID_OPS_SET_SWITCH_HYS, COpsCmd::SetSwitchHys},
        {OID_OPS_GET_SWITCH_HYS, COpsCmd::GetSwitchHys},
        {OID_OPS_SET_LED_STATUS, COpsCmd::SetLedStatus},
        {OID_OPS_SET_LED_MODE, COpsCmd::SetLedMode},
        {OID_OPS_CLEAR_ALARM_DURATION, COpsCmd::ClearAlarmDuration},
        {OID_OPS_RESET, COpsCmd::Reset},
        {OID_OPS_UPGRADE, COpsCmd::Upgrade},
        {OID_OPS_GET_ALL_CFG, COpsCmd::GetCfgData},
        {OID_OPS_SET_REMOTE_STATUS, COpsCmd::SetRemoteStatus},
        {OID_OPS_GET_REQUEST_STATUS, COpsCmd::GetRequestStatus},
        {OID_OPS_SET_SWITCHING_TYPE, COpsCmd::SetSwitchingType},
        {OID_OPS_SET_CONN_PD_THR, COpsCmd::SetConnectionPdThr},
        {OID_OPS_SET_CONN_PD_HYS, COpsCmd::SetConnectionPdHys},
        {OID_OPS_SET_ALM_MASK, COpsCmd::SetAlmMask},
        {OID_OPS_GET_EVENTLOG, COpsCmd::GetEventLog},

        {OID_OPS_SET_WSS_PORT_POWER_THR, COpsCmd::SetWssPortPowerThr},
        {OID_OPS_SET_WSS_PORT_POWER_HYS, COpsCmd::SetWssPortPowerHys},
        {OID_OPS_GET_WSS_PORT_HISTORY_POWER, COpsCmd::GetWssPortPowerHirtory},
        {OID_OPS_SET_WSS_LED_MODE, COpsCmd::SetWssLedMode},
        {OID_OPS_GET_OSC_SWITCH_POS, COpsCmd::GetOscSwitchPos},
        {OID_OPS_SET_OSC_SWITCH_POS, COpsCmd::SetOscSwitchPos},
        {OID_OPS_DBG_SET_COMM_FAIL, COpsCmd::DbgSetCommFail},
        {OID_OPS_DBG_CLEAR_COMM_FAIL, COpsCmd::DbgClearCommFail},

        /* 12. Extend set cmd */
        {OID_EXTEND_SWITCH_POS, CExtendCmd::SetSwitchPos},
        {OID_EXTEND_SFP, CExtendCmd::SetSfp},
        {OID_EXTEND_SET_VOA, CExtendCmd::SetVoaAttn},
        {OID_EXTEND_RESET, CExtendCmd::Reset},
        {OID_EXTEND_UPGRADE, CExtendCmd::Upgrade},
        {OID_EXTEND_DBG_READ, CExtendCmd::DbgRead},
        {OID_EXTEND_DBG_WRITE, CExtendCmd::DbgWrite},


        /* 13. Mux set cmd */

        /* 14. OCS set cmd */
        {OID_OCS_SET_ONE_SWITCH_CHANNEL, COcsCmd::SetOneSwitchChannel},
        {OID_OCS_SET_SWITCH_CHANNELS, COcsCmd::SetSwitchChannels},
        {OID_OCS_SET_LAMP_TEST_STATE, COcsCmd::SetLampTestState},
        {OID_OCS_RESET, COcsCmd::Reset},
        {OID_OCS_UPGRADE, COcsCmd::Upgrade},
        {OID_OCS_DBG_SET_COMM_FAIL, COcsCmd::DbgSetCommFail},
        {OID_OCS_DBG_CLEAR_COMM_FAIL, COcsCmd::DbgClearCommFail},
        {OID_OCS_COMMIT, COcsCmd::Commit},
    };

    CDevCmdProcesser()
    {
    }
    CDevCmdProcesser(const CDevCmdProcesser &) = delete;
    CDevCmdProcesser &operator=(const CDevCmdProcesser &) = delete;

};


