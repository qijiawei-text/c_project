/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OpsProto.h
   Author:        Zhen Zhang
   Date:          2018-12-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-12-28] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevData.h"
#include "Proto.h"

/*************************************************/

enum EOpsProto
{
    OPS_RSP_MAX_LEN = 1024,
    OPS_READ_NUM    = 500,


};

class COpsSwProto : public CProto
{
public:
    COpsSwProto()
    {
    }
    virtual
    ~COpsSwProto()
    {
    }


    uint16_t CheckSum(uint16_t usSeed, uint8_t *pucSource, uint32_t uiLen);

    /* virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) = 0; */

    virtual void GetMfgCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;

    virtual void GetVerCmd(std::string &rstaMsg) = 0;
    virtual bool GetVerRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;

    virtual void GetStatusDataCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;

    virtual void GetWorkModeCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWorkModeRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) = 0;

    virtual void GetAllCfgCmd(std::string & rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAllCfgRsp(const std::string & c_rstaMsg, COpsCfgData &rstData) = 0;

    virtual void GetAlarmCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAlarmRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData) = 0;

    virtual void GetPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) = 0;

    virtual void GetConnectionPdsPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPdsPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) = 0;
    virtual void SetConnectionPdPowerThrCmd(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetConnectionPdsPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPdsPowerThrRsp(std::string &rstaMsg, COpsConnectionPdThr &rstData) = 0;
    virtual void SetConnectionPdPowerHysCmd(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetConnectionPdsPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetConnectionPdsPowerHysRsp(std::string &rstaMsg, COpsConnectionPdHys &rstData) = 0;

    virtual void GetPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerThrRsp(const std::string & c_rstaMsg, CPortPowerThr &rstData) = 0;
    virtual void GetPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerHysRsp(const std::string & c_rstaMsg, CPortPowerHys &rstData) = 0;

    virtual void GetPortPowerHistoryCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPortPowerHistoryRsp(const std::string &c_rstaMsg, COpsHistoryData &rstData) = 0;

    virtual void SetPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;

    virtual void SetWorkModeCmd(uint32_t uiMode, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetWtrCmd(uint32_t uiWtr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetHoldOffTimeCmd(uint32_t uiHoldOffTime, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetRevertiveCmd(uint32_t uiRevertive, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetRelativeThrCmd(int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetRelativeThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRelativeThrRsp(const std::string & c_rstaMsg, CRelativeThr &rstData) = 0;
    virtual void SetRelativeOffsetCmd(int32_t iOffset, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetRelativeOffsetCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRelativeOffsetRsp(const std::string & c_rstaMsg, CRelativeOffset &rstData) = 0;

    virtual void SetSwitchThrCmd(uint32_t uiPos, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetSwitchThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchThrRsp(const std::string &c_rstaMsg, COpsSwitchThr &rstThr, uint32_t uiSubIdx = 0) = 0;

    virtual void SetSwitchHysCmd(uint32_t uiPos, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetSwitchHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchHysRsp(const std::string &c_rstaMsg, COpsSwitchHys &rstHys, uint32_t uiSubIdx = 0) = 0;

    virtual void SoftResetCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void StartUpgCmd(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) = 0;
    virtual void SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) = 0;
    virtual void EndUpgCmd(std::string &rstaMsg) = 0;

    virtual void GetWssPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx = 0) = 0;

    virtual void GetWssPortPowerHistoryCmd(uint32_t uiPacketId, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerHistoryRsp(const std::string &c_rstaMsg, uint8_t *pucBuf, uint32_t uiLen, uint32_t uiSubIdx = 0) = 0;

    virtual void SetWssPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetWssPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetWssPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerThrRsp(const std::string & c_rstaMsg, CWssPortPowerThr &rstData) = 0;
    virtual void GetWssPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssPortPowerHysRsp(const std::string & c_rstaMsg, CWssPortPowerHys &rstData) = 0;

    virtual void GetWssAlarmCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetWssAlarmRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) = 0;

    virtual void SetWssLedModeCmd(uint8_t ucMode, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, std::string & rstaMsg, uint32_t uiSubIdx = 0) = 0;

    virtual void GetOscPdCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetOscPdRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) = 0;

    virtual void GetOscSwitchPosCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetOscSwitchPosRsp(const std::string &c_rstaMsg, COscSwitchPos &rstData) = 0;

    virtual void SetOscSwitchPosCmd(uint32_t uiSwitchIdx, uint32_t uiPos, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetLedStatusCmd(uint32_t usLedIdx, uint32_t usStatus, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void SetLedModeCmd(uint8_t ucMode, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetLedStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetLedStatusRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual void GetAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetAlarmDurationRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual void ClearAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetVoltageCmd(std::string &rstaMsg, uint32_t usPdIdx, uint32_t uiSubIdx) = 0;
    virtual bool GetVoltageRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t usPdIdx, uint32_t uiSubIdx) = 0;
    virtual void GetILCmd(std::string &rstaMsg, uint32_t uiSubIdx) = 0;
    virtual bool GetILRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx) = 0;
    virtual void GetUpgStatCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetUpgStatCmdRsp(const std::string &c_rstaMsg, COpsUpgStat &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual void GetMfgExCmd(std::string &rstaMsg) = 0;
    virtual bool GetMfgExRsp(const std::string &c_rstaMsg, CMfg &rstMfg) = 0;
    virtual void SetAlmMaskCmd(uint32_t uiIndex, uint32_t uiMask, std::string &rstaMsg, uint32_t uiSubIdx) = 0;
    virtual void GetAlmMaskCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAlmMaskRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual void GetEventLogCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetEventLogRsp(const std::string &c_rstaMsg, COpsEventLogAllData &rstCali) = 0;

    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                     */
    /********************************************************************************************/
    /* @ {                                                                                      */
    virtual void SetRemoteStatusCmd(COpsReqStatus& stStatus, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetRequestStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRequestStatusRsp(std::string &rstaMsg, COpsBidirectionStatus &rstData) = 0;
    virtual void SetSwitchingTypeCmd(uint8_t ucType, std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual void GetSwitchingTypeCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetSwitchingTypeRsp(std::string &rstaMsg, uint8_t &ucType) = 0;
    /* @ }                                                                                      */

};

#if 0
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

#endif


#define DEF_OPS_VER_LEN                 (20)
typedef struct {
    char acMfgName  [DEF_OPS_VER_LEN];
    char acPN       [DEF_OPS_VER_LEN];
    char acSN       [DEF_OPS_VER_LEN];
    char acMfgDate  [DEF_OPS_VER_LEN];
    char acCalDate  [DEF_OPS_VER_LEN];

    char acUnitName[DEF_OPS_VER_LEN];
    char acFCPN[DEF_OPS_VER_LEN];
    char acFCIss[DEF_OPS_VER_LEN];
    char acFujitsuPN[DEF_OPS_VER_LEN];
    char acCleiCode[DEF_OPS_VER_LEN];
    char acModeNo[DEF_OPS_VER_LEN];
    /*char acFjtSn[DEF_OPS_VER_LEN]; */
} __attribute__((packed)) ST_MFG_INFO;

typedef struct {
    char acVendorId[DEF_OPS_VER_LEN];
    char acModType[DEF_OPS_VER_LEN];
    char acHwVer[DEF_OPS_VER_LEN];
    char acMainVer[DEF_OPS_VER_LEN];
    char acBootVer[DEF_OPS_VER_LEN];
    char acFpgaVer[DEF_OPS_VER_LEN];
    char acBundleVer[DEF_OPS_VER_LEN];
    char acLedHwVer[DEF_OPS_VER_LEN];
    char acLedFwVer[DEF_OPS_VER_LEN];

} __attribute__((packed)) ST_VER;

typedef struct {
    uint8_t ucWorkMode;
    uint8_t ucSwitchPos;
    uint8_t ucFswStatus;
} __attribute__((packed)) ST_WORK_MODE;


typedef struct
{
    uint8_t abLos[OPS_PORT_CNT];
#if 0
    uint8_t bLine1InHTimeLos;
    uint8_t bLine2InHTimeLos;
#endif
    uint8_t bSwitchFailed;
    uint8_t bLut;
    uint8_t bRelative;
#if 0
    uint8_t abLow[OPS_PORT_CNT];
    uint8_t abHigh[OPS_PORT_CNT];

    uint8_t ucReserved;
    uint8_t abSwitchAlarm[OPS_PATH_CNT];
#endif
    uint8_t bVoltage;
    uint8_t bComInLoss;
    uint8_t bReceivePowerDifferBetween;
}__attribute__((packed)) ST_ALARM;


typedef struct {
    int16_t asPortPower[OPS_PORT_CNT];
} ST_PORT_POWER;

typedef struct
{
    uint32_t auiAlarm[9];
}__attribute__((packed)) ST_WSS_ALARM;

typedef struct {
    int16_t asPortPower[WSS_PORT_CNT];
} __attribute__((packed)) ST_WSS_PORT_POWER;



typedef struct {
    uint16_t usPos;
    int16_t sThr;
} ST_OPS_SWITCH_THR_SET;

typedef struct {
    uint16_t usPos;
    int16_t sHys;
} ST_OPS_SWITCH_HYS_SET;

typedef struct {
    uint16_t usPort;
    uint16_t usType;
    int16_t sThr;
} __attribute__((packed)) ST_PORT_POWER_THR;

typedef struct {
    uint16_t usPort;
    uint16_t usType;
    int16_t sHys;
} __attribute__((packed)) ST_PORT_POWER_HYS;

typedef struct {
    uint32_t auiOpsInLos[OPS_PORT_LINE_IN_CNT];
    uint32_t auiWssLos[WSS_PANEL_PORT_CNT];
    uint32_t auiOpsInLow[OPS_PORT_LINE_IN_CNT];
    uint32_t auiWssLow[WSS_PANEL_PORT_CNT];
} __attribute__((packed)) ST_PORT_ALARM_DURATION;

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
#if 0
    /*
       [Line1 In Los/Low/High]
       [Line2 In Los/Low/High]
       [Line1 Out Los/Low/High]
       [Line2 Out Los/Low/High]
       [WSS IN1 Los/Low/High]
       ?-?-
       [WSS IN64 Los/Low/High]
       [WSS COM1 Los/Low/High]
       [WSS COM2 Los/Low/High]
     */

    int16_t sPowerLosThr[OPS_PORT_CNT + WSS_PORT_CNT + 2];
    int16_t sPowerLowThr[OPS_PORT_CNT + WSS_PORT_CNT + 2];
    int16_t sPowerHighThr[OPS_PORT_CNT + WSS_PORT_CNT + 2];

    int16_t sPowerLosHys[OPS_PORT_CNT + WSS_PORT_CNT + 2];
    int16_t sPowerLowHys[OPS_PORT_CNT + WSS_PORT_CNT + 2];
    int16_t sPowerHighHys[OPS_PORT_CNT + WSS_PORT_CNT + 2];
#endif
    int16_t sPowerLosThr[OPS_PORT_CNT];
    int16_t sPowerLosHys[OPS_PORT_CNT];
    int16_t sComInLosThr;
    int16_t sDifferBetweenLine1Line2;
    int16_t sComInLosHys;
}__attribute__((packed)) ST_ALL_CFG;

typedef struct {
    uint16_t usIdx;
    uint16_t usMask;
} __attribute__((packed)) ST_ALM_MASK;


#define SPI_CRC16_FEED                                                  (0xFFFFu)

#define DEF_FRM_CRC_LEN                                                         (2)
#define DEF_STATUS_OK                                                   (0)
#define DEF_STATUS_IN_PROGRESS                                          (9)

/**
 * @defgroup  OPS Command
 * @{
 */
#define DEF_OPS_CMD_GET_MFGINFO                         (0x01)
#define DEF_OPS_CMD_GET_BOOT_VER                        (0x02)

#define DEF_OPS_CMD_SET_WORKMODE                        (0x10)
#define DEF_OPS_CMD_GET_WORKMODE                        (0x11)

#define DEF_OPS_CMD_SET_REVERTIVE                       (0x12)
#define DEF_OPS_CMD_SET_HOLDOFF                         (0x13)
#define DEF_OPS_CMD_SET_WTR                             (0x14)

#define DEF_OPS_CMD_ALL_CFG                             (0x17)

#define DEF_OPS_CMD_SET_PORT_POWER_THR                  (0x20)
#define DEF_OPS_CMD_GET_PORT_POWER_THR                  (0x21)
#define DEF_OPS_CMD_SET_PORT_POWER_HYS                  (0x22)
#define DEF_OPS_CMD_GET_PORT_POWER_HYS                  (0x23)

#define DEF_OPS_CMD_GET_PORT_POWER                      (0x24)
#define DEF_OPS_CMD_GET_CONNECTION_PDS_POWER            (0x25)
#define DEF_OPS_CMD_SET_CONNECTION_PDS_POWER_THR        (0x26)
#define DEF_OPS_CMD_GET_CONNECTION_PDS_POWER_THR        (0x27)
#define DEF_OPS_CMD_SET_CONNECTION_PDS_POWER_HYS        (0x28)
#define DEF_OPS_CMD_GET_CONNECTION_PDS_POWER_HYS        (0x29)
#define DEF_OPS_CMD_GET_PORT_POWER_HISTORY              (0x47)

#define DEF_OPS_CMD_GET_IL                              (0x41)
#define DEF_OPS_CMD_GET_ALRM                            (0x42)
#define DEF_OPS_CMD_SET_RELATIVE_THR                    (0x43)
#define DEF_OPS_CMD_GET_RELATIVE_THR                    (0x44)
#define DEF_OPS_CMD_SET_RELATIVE_OFFSET                 (0x45)
#define DEF_OPS_CMD_GET_RELATIVE_OFFSET                 (0x46)

#define DEF_OPS_CMD_GET_WSS_PORT_POWER_THR              (0x25)
#define DEF_OPS_CMD_GET_WSS_PORT_POWER_HYS              (0x26)
#define DEF_OPS_CMD_GET_WSS_PORT_POWER                  (0x27)
#define DEF_OPS_CMD_SET_WSS_PORT_POWER_THR              (0x28)
#define DEF_OPS_CMD_SET_WSS_PORT_POWER_HYS              (0x29)
#define DEF_OPS_CMD_GET_OSC_PD                          (0x30)
#define DEF_OPS_CMD_SET_OSC_SWITCH_POS                  (0x31)
#define DEF_OPS_CMD_GET_OSC_SWITCH_POS                  (0x32)
#define DEF_OPS_CMD_GET_LED_STATUS                      (0x33)

#define DEF_OPS_CMD_SET_ALM_MASK                        (0x47)
#define DEF_OPS_CMD_GET_ALM_MASK                        (0x48)

#define DEF_OPS_CMD_GET_WSS_PORT_POWER_HISTORY          (0x48)
#define DEF_OPS_CMD_GET_WSS_ALRM                        (0x49)
#define DEF_OPS_CMD_SET_WSS_LED_MODE                    (0x2A)
#define DEF_OPS_CMD_SET_REAL_TIME                       (0x2B)

#define DEF_OPS_CMD_SET_SWITCH_THR                      (0x4A)
#define DEF_OPS_CMD_GET_SWITCH_THR                      (0x4B)
#define DEF_OPS_CMD_SET_SWITCH_HYS                      (0x4C)
#define DEF_OPS_CMD_GET_SWITCH_HYS                      (0x4D)
#define DEF_OPS_CMD_GET_ALARM_DURATION                  (0x4E)
#define DEF_OPS_CMD_CLEAR_ALARM_DURATION                (0x4F)

#define DEF_OPS_CMD_SET_REMOTE_STATUS                   (0x50)
#define DEF_OPS_CMD_GET_ENDS_STATUS                     (0x51)
#define DEF_OPS_CMD_SET_SWITCHING_TYPE                  (0x52)
#define DEF_OPS_CMD_GET_SWITCHING_TYPE                  (0x53)

#define DEF_OPS_CMD_SET_LED_STUTAS                      (0xE0)
#define DEF_OPS_CMD_SET_LED_MODE                        (0xE1)
#define DEF_OPS_CMD_GET_VOLTAGE                         (0xE2)
#define DEF_OPS_CMD_GET_MFGEX                           (0xE3)
#define DEF_OPS_CMD_GET_EVENT_LOG                       (0xE7)

#define DEF_OPS_CMD_RESET                               (0xF0)
#define DEF_OPS_CMD_UPGD_START                          (0xF1)
#define DEF_OPS_CMD_UPGD_DWNLD                          (0xF2)
#define DEF_OPS_CMD_UPGD_END                            (0xF3)
#define DEF_OPS_CMD_GET_UPGD_STAT                       (0xF4)

#define DEF_OPS_CMD_GET_PD_ADC                          (0xA0)
#define DEF_OPS_CMD_GET_PD_POWER                        (0xA1)
#define DEF_OPS_CMD_GET_TEMP                            (0xA3)

#define DEF_UPGD_PKG_LEN_MAX                            (1024)
#define DEF_WSS_PORT_HISTORY_POWER_DATA_TOTAL_LEN       (600 * 2 * 66) /* 100ms internal, total 1s */
#define DEF_PER_PACKET_LEN                              (1 * 1024)

class COpsSwFrameProto : public COpsSwProto
{
private:
    bool GeneralCmd(uint16_t usOid, std::string staData, std::string &rstaMsg, uint32_t uiSubIdx = 1);
public:
    virtual bool ProcRsp(std::string &rstaMsg, bool &rbInProgress) override;

    virtual void GetMfgCmd(std::string &rstaMsg) override;
    virtual bool GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;

    virtual void GetVerCmd(std::string &rstaMsg) override;
    virtual bool GetVerRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;

    virtual void GetStatusDataCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;

    virtual void GetWorkModeCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWorkModeRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) override;

    virtual void GetAllCfgCmd(std::string & rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetAllCfgRsp(const std::string & c_rstaMsg, COpsCfgData &rstData) override;

    virtual void GetAlarmCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetAlarmRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData) override;

    virtual void GetPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) override;

    virtual void GetConnectionPdsPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPdsPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) override;
    virtual void SetConnectionPdPowerThrCmd(uint32_t uiPdType, uint32_t uiThrType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetConnectionPdsPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPdsPowerThrRsp(std::string &rstaMsg, COpsConnectionPdThr &rstData) override;
    virtual void SetConnectionPdPowerHysCmd(uint32_t uiPdType, uint32_t uiHysType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetConnectionPdsPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetConnectionPdsPowerHysRsp(std::string &rstaMsg, COpsConnectionPdHys &rstData) override;


    virtual void GetPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerThrRsp(const std::string & c_rstaMsg, CPortPowerThr &rstData) override;
    virtual void GetPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerHysRsp(const std::string & c_rstaMsg, CPortPowerHys &rstData) override;

    virtual void GetPortPowerHistoryCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetPortPowerHistoryRsp(const std::string &c_rstaMsg, COpsHistoryData &rstData) override;

    virtual void SetPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;

    virtual void SetWorkModeCmd(uint32_t uiMode, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetWtrCmd(uint32_t uiWtr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetHoldOffTimeCmd(uint32_t uiHoldOffTime, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetRevertiveCmd(uint32_t uiRevertive, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;

    virtual void SetRelativeThrCmd(int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetRelativeThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetRelativeThrRsp(const std::string & c_rstaMsg, CRelativeThr &rstData) override;
    virtual void SetRelativeOffsetCmd(int32_t iOffset, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetRelativeOffsetCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetRelativeOffsetRsp(const std::string & c_rstaMsg, CRelativeOffset &rstData) override;

    virtual void SetSwitchThrCmd(uint32_t uiPos, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetSwitchThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchThrRsp(const std::string &c_rstaMsg, COpsSwitchThr &rstThr, uint32_t uiSubIdx = 0) override;

    virtual void SetSwitchHysCmd(uint32_t uiPos, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetSwitchHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchHysRsp(const std::string &c_rstaMsg, COpsSwitchHys &rstHys, uint32_t uiSubIdx = 0) override;

    virtual void SoftResetCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void StartUpgCmd(std::string &rstaMsg, uint32_t ulLen, uint32_t ulCrc) override;
    virtual void SendDataCmd(std::string &rstaMsg, uint8_t *pcBuf, uint32_t ulLen) override;
    virtual void EndUpgCmd(std::string &rstaMsg) override;

    virtual void GetWssPortPowerCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual void GetWssPortPowerHistoryCmd(uint32_t uiPacketId, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerHistoryRsp(const std::string &c_rstaMsg, uint8_t *pucBuf, uint32_t uiLen, uint32_t uiSubIdx = 0) override;

    virtual void SetWssPortPowerThrCmd(uint32_t uiPort, uint32_t uiType, int32_t iThr, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetWssPortPowerHysCmd(uint32_t uiPort, uint32_t uiType, int32_t iHys, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetWssPortPowerThrCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerThrRsp(const std::string & c_rstaMsg, CWssPortPowerThr &rstData) override;
    virtual void GetWssPortPowerHysCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssPortPowerHysRsp(const std::string & c_rstaMsg, CWssPortPowerHys &rstData) override;

    virtual void GetWssAlarmCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetWssAlarmRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) override;

    virtual void SetWssLedModeCmd(uint8_t ucMode, std::string &rstaMsg, uint32_t uiSubIdx) override;
    virtual void SetRealTime(uint32_t uiHour, uint32_t uiMinute, uint32_t uiSecond, std::string & rstaMsg, uint32_t uiSubIdx) override;

    virtual void GetOscPdCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetOscPdRsp(const std::string &c_rstaMsg, COpsStatusData &rstData) override;

    virtual void GetOscSwitchPosCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetOscSwitchPosRsp(const std::string &c_rstaMsg, COscSwitchPos &rstData) override;

    virtual void SetOscSwitchPosCmd(uint32_t uiSwitchIdx, uint32_t uiPos, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetLedStatusCmd(uint32_t usLedIdx, uint32_t usStatus, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void SetLedModeCmd(uint8_t ucMode, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetLedStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetLedStatusRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual void GetAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetAlarmDurationRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual void ClearAlarmDurationCmd(std::string &rstaMsg, uint32_t uiSubIdx);
    virtual void GetVoltageCmd(std::string &rstaMsg, uint32_t usPdIdx, uint32_t uiSubIdx) override;
    virtual bool GetVoltageRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t usPdIdx, uint32_t uiSubIdx = 0) override;
    virtual void GetILCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetILRsp(const std::string &c_rstaMsg, COpsStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual void GetUpgStatCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetUpgStatCmdRsp(const std::string &c_rstaMsg, COpsUpgStat &rstData, uint32_t uiSubIdx = 0) override;
    virtual void GetMfgExCmd(std::string &rstaMsg) override;
    virtual bool GetMfgExRsp(const std::string &c_rstaMsg, CMfg &rstMfg) override;
    virtual void SetAlmMaskCmd(uint32_t uiIndex, uint32_t uiMask, std::string &rstaMsg, uint32_t uiSubIdx) override;
    virtual void GetAlmMaskCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetAlmMaskRsp(const std::string &c_rstaMsg, COpsCommStatusData &rstData, uint32_t uiSubIdx = 0) override;
    virtual void GetEventLogCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetEventLogRsp(const std::string &c_rstaMsg, COpsEventLogAllData &rstCali) override;

    /********************************************************************************************/
    /* Operation commands for 1+1 bidirectional switching.                     */
    /********************************************************************************************/
    /* @ {                                                                                      */
    virtual void SetRemoteStatusCmd(COpsReqStatus& stStatus, std::string &rstMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetRequestStatusCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetRequestStatusRsp(std::string &rstaMsg, COpsBidirectionStatus &rstData) override;
    virtual void SetSwitchingTypeCmd(uint8_t ucType, std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual void GetSwitchingTypeCmd(std::string &rstaMsg, uint32_t uiSubIdx = 0) override;
    virtual bool GetSwitchingTypeRsp(std::string &rstaMsg, uint8_t &ucType) override;
    /* @ }                                                                                      */
};










