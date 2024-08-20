/***MODU+******************************************************************************************/
/* Copyright(C), 2017, OPLINK Tech.Co.,Ltd                                                        */
/* FileName    : OaComm.h                                                                   */
/* Description : The header file of sdv_edfa_cmd.c.                                               */
/* History     :                                                                                  */
/*     [Author]      [Date]          [Version]        [Description]                               */
/* [1]                                                            .                               */
/***MODU-******************************************************************************************/

#ifndef __SDV_EDFA_CMD_H__
#define __SDV_EDFA_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif


#define OA_SPI_CRC16_FEED                       (0xFFFFu)

#define OA_SPI_BUFFER_SIZE                      (8192)
#define OA_SPI_OVERHEAD                         (21)        /* 7 Bytes Trans header + 12 Bytes protocol header + 2 Bytes CRC */
#define OA_SPI_PAYLOAD_MAX                      (4075)      /* OA_SPI_BUFFER_SIZE - OA_SPI_OVERHEAD */

/* opcode in SPI-bus */
#define OA_SPI_OPCODE_WR                        (0x02)
#define OA_SPI_OPCODE_RD                        (0x0B)

/* magic word */
#define OA_SPI_MAGIC_WORD                       (0xFFFF)

/* protocol layer */
/* command */
#define OA_SPI_CMD_READ                         (0xA5)
#define OA_SPI_CMD_WRITE                        (0x5A)
/* oid */
#define OA_SPI_OID_FIX                          (0x0101)
/* mid */
#define OA_SPI_MID_EDFA1                        (0x0000)
#define OA_SPI_MID_EDFA2                        (0x0001)
#define OA_SPI_MID_EDFA3                        (0x0002)
#define OA_SPI_MID_EDFA4                        (0x0003)
/* did */
#define OA_SPI_DID_GET_MODULE_INFO              (0x10)
#define OA_SPI_DID_EDFA_CONFIG                  (0x11)
#define OA_SPI_DID_EDFA_THR_CONFIG              (0x12)
#define OA_SPI_DID_PADDING_VOA_CONFIG           (0x13)
#define OA_SPI_DID_PORT_STATUS_CONFIG           (0x14)
#define OA_SPI_DID_PD_THR_CONFIG                (0x15)
#define OA_SPI_DID_WBA_CONFIG                   (0x16)
#define OA_SPI_DID_SET_DGE_FLATNESS             (0x17)
#define OA_SPI_DID_SET_SAFETY_DATA              (0x18)
#define OA_SPI_DID_TONE_CONFIG_DATA             (0x19)
#define OA_SPI_DID_SET_TONE_TRIG_THR            (0x1A)
#define OA_SPI_DID_GET_MODULE_STATUS            (0x1B)
#define OA_SPI_DID_GET_DGE_FLATNESS             (0x1C)
#define OA_SPI_DID_GET_PD_POWER                 (0x1D)
#define OA_SPI_DID_GET_TONE_STATUS              (0x1E)
#define OA_SPI_DID_GET_MISC_STATUS              (0x1F)
#define OA_SPI_DID_GET_HASH_DATA                (0x20)
#define OA_SPI_DID_GET_EDFA_WBA_DATA            (0x21)
#define OA_SPI_DID_GET_TONE_OPER_DATA           (0x23)
#define OA_SPI_DID_GET_CAPABILITY               (0x24)
#define OA_SPI_DID_EDFA_CONFIG_ALL              (0x25)
#define OA_SPI_DID_DBG_EDFA_FPGA                (0x26)
#define OA_SPI_DID_GET_EDFA_EVENT               (0x27)
#define OA_SPI_DID_GET_EDFA_ALARM               (0x28)
#define OA_SPI_DID_RESTORE_DEFAULT              (0x29)
#define OA_SPI_DID_GET_EVENT_STATUS             (0x2A)
#define OA_SPI_DID_GET_ALARM_STATUS             (0x2B)

#define OA_SPI_DID_SWITCH_PORT                  (0x30)
#define OA_SPI_DID_DBG_EDFA_CPLD                (0x31)

#define OA_SPI_DID_DBG_BAUDRATE                 (0x32)

#define OA_SPI_DID_SPE_CONFIG                   (0x33)

#define OA_SPI_DID_GET_UPG_INFO                 (0x40)
/* Get all debug information from OA module. */
#define DEF_OA_SPI_DID_GET_MCU_DBG_INFO         (0x41)

#define DEF_OA_SPI_DID_GET_FPGA_INFO            (0x42)

#define OA_SPI_DID_GAIN_RANGE_MODE              (0x50)
#define OA_SPI_DID_GAIN                         (0x51)
#define OA_SPI_DID_CTRL_MODE                    (0x52)
#define OA_SPI_DID_OSRI_CTRL_MODE               (0x53)
#define OA_SPI_DID_TILT                         (0x54)
#define OA_SPI_DID_PADDING_VOA                  (0x55)
#define OA_SPI_DID_APR_ENABLE                   (0x56)
#define OA_SPI_DID_AUTOLOS_ENABLE               (0x57)
#define OA_SPI_DID_RX_LOS_THR                   (0x58)
#define OA_SPI_DID_RX_LOS_HYS                   (0x59)
#define OA_SPI_DID_OSC_TX_VOA                   (0x59)
#define OA_Set_DID_EDFA_READY_TRIGGER           (0x5A)
#define OA_Set_DID_EDFA_AUTOGAIN_MODE           (0x5B)

#define OA_SPI_DID_OSC_TX_VOA_POWER             (0x60)
#define OA_SPI_DID_MANUAL_VOA_CONTROL           (0x61)
#define OA_SPI_DID_PADDING_VOA_POWER            (0x62)
#define OA_SPI_DID_MANUAL_GAIN_CONTROL          (0x63)
#define OA_SPI_DID_TX_LOS_THR                   (0x64)
#define OA_SPI_DID_PUMP_CURRENT                 (0x65)

#define OA_SPI_DID_GET_REAL_GAIN                (0x70)
#define OA_SPI_DID_GET_REAL_TILT                (0x71)
#define OA_SPI_DID_GET_REAL_VOA                 (0x72)

#define OA_SPI_DID_READY_POWER                  (0x80)

#define OA_SPI_DID_DFB_ONFF                     (0xA0)
#define OA_SPI_DID_OSC_ONOFF                    (0xA1)
#define OA_SPI_DID_OSC_POWER                    (0xA2)
#define OA_SPI_DID_OSC_ADD_THR_HYS              (0xA3)
#define OA_SPI_DID_OSC_DROP_THR_HYS             (0xA4)
#define OA_SPI_DID_OSC_RX_THR_HYS               (0xA5)
#define OA_SPI_DID_OSC_ALM                      (0xA6)
#define OA_SPI_DID_OTDR_SWITCH                  (0xA7)
#define OA_SPI_DID_APSD_ONOFF                   (0xA8)
#define OA_SPI_DID_SET_SLC_UPG_STATE            (0xA9)
#define OA_SPI_DID_SET_SLC_LED_STATE            (0xAA)
#define OA_SPI_DID_SLC_OTDR_CALI                (0xAB)
#define OA_SPI_DID_SLC_OCM_CALI                 (0xAC)
#define OA_SPI_DID_SET_SLC_LED_ALM              (0xAD)
#define OA_SPI_DID_OSC_TX_THR_HYS               (0xAE)
#define OA_SPI_DID_THR_HYS                      (0xAF)
#define OA_SPI_DID_PORT_ALM_MASK                (0xB0)
#define OA_SPI_DID_EVENT_LOG                    (0xB1)
#define OA_SPI_DID_SHUTTER_STATE                (0xB2)
#define OA_SPI_DID_REMOTE_INTERLOCK_ENABLE      (0x5C)
#define OA_SPI_DID_DUMMY_ADMIN_KEY_ENABLE       (0x5D)
#define OA_SPI_DID_EDFA_APC_TAR                 (0x5E)
#define OA_SPI_DID_PIF_SAFE_EN                  (0x5F)

#define OA_SPI_DID_DFB_ONFF_STATE               (0xC0)
#define OA_SPI_DID_OSC_ONOFF_STATE              (0xC1)
#define OA_SPI_DID_GET_REAL_APSD                (0xC2)
#define OA_SPI_DID_GET_REAL_OTDR_SWITCH         (0xC3)

#define OA_SPI_DID_PWR_CONSUMPTION              (0xE0)

#define OA_SPI_DID_FW_RESET                     (0xF1)
#define OA_SPI_DID_FW_UPGD_START                (0xF2)
#define OA_SPI_DID_FW_UPGD_WRITE                (0xF3)
#define OA_SPI_DID_FW_UPGD_VERIFY               (0xF4)
#define OA_SPI_DID_FW_UPGD_SWITCH               (0xF5)
#define OA_SPI_DID_FW_UPGD_COMMIT               (0xF6)
#define OA_SPI_DID_FW_UPGD_ABORT                (0xF7)

/* instance */
#define OA_SPI_INST_FIX                         (0x0000)
/* address */
#define OA_SPI_ADDR_FIX                         (0x0000)

/* length */
#define OA_SPI_LEN_GET_MODULE_INFO              (416)
#define OA_SPI_LEN_EDFA_CONFIG                  (128)
#define OA_SPI_LEN_EDFA_THR_CONFIG              (1536)
#define OA_SPI_LEN_PADDING_VOA_CONFIG           (8)
#define OA_SPI_LEN_PORT_STATUS_CONFIG           (56)
#define OA_SPI_LEN_PD_THR_CONFIG                (480)
#define OA_SPI_LEN_WBA_CONFIG                   (1548)      /* 774*2 */
#define OA_SPI_LEN_SET_DGE_FLATNESS             (1536)      /* 2*96*8 */
#define OA_SPI_LEN_SET_SAFETY_DATA              (160)
#define OA_SPI_LEN_TONE_CONFIG_DATA             (8)
#define OA_SPI_LEN_SET_TONE_TRIG_THR            (64)
#define OA_SPI_LEN_GET_MODULE_STATUS            (224)
#define OA_SPI_LEN_GET_DGE_FLATNESS             (1536)      /* 2*96*8 */
#define OA_SPI_LEN_GET_PD_POWER                 (100)
#define OA_SPI_LEN_GET_TONE_STATUS              (2052)      /* 2048+4 */
#define OA_SPI_LEN_GET_MISC_STATUS              (720)
#define OA_SPI_LEN_GET_HASH_DATA                (64)        /* ?????? 64/128 */
#define OA_SPI_LEN_GET_EDFA_WBA_DATA            (3604)
#define OA_SPI_LEN_GET_TONE_OPER_DATA           (2124)
#define OA_SPI_LEN_GET_CAPABILITY               (1136)

#define OA_SPI_LEN_PWR_CONSUMPTION              (8)

#define OA_SPI_LEN_FW_RESET                     (0)
#define OA_SPI_LEN_FW_UPGD_START                (0)
#define OA_SPI_LEN_FW_UPGD_WRITE                (0)
#define OA_SPI_LEN_FW_UPGD_VERIFY               (0)
#define OA_SPI_LEN_FW_UPGD_SWITCH               (0)
#define OA_SPI_LEN_FW_UPGD_COMMIT               (0)
#define OA_SPI_LEN_FW_UPGD_ABORT                (0)

/* status */
#define OA_SPI_RSP_SUCCESS                      (0x00)
#define OA_SPI_RSP_CHECKSUM_ERR                 (0x01)
#define OA_SPI_RSP_SYNTAX_ERR                   (0x02)
#define OA_SPI_RSP_CMDCODE_NOT_SUPPORT          (0x03)
#define OA_SPI_RSP_REJECT                       (0x04)
/*#define OA_SPI_RSP_OSW_PORT_INVALID             (0x05) */
#define OA_SPI_RSP_NVM_READ_FAIL                (0x06)
#define OA_SPI_RSP_NVM_WRITE_FAIL               (0x07)
#define OA_SPI_RSP_OUT_OF_RANGE                 (0x08)
#define OA_SPI_RSP_COMMAND_IN_PROGRESS          (0x09)
#define OA_SPI_RSP_RSV                          (0x0A)
#define OA_SPI_RSP_FW_FORMAT_ERR                (0x0B)
#define OA_SPI_RSP_FW_CHECKSUM_ERR              (0x0C)
#define OA_SPI_RSP_DEVICE_NOT_READY             (0x0D)
#define OA_SPI_RSP_TIMEOUT                      (0x0E)

#define OA_SPI_CRC_LEN                          (2)

#define OA_SPI_CMD_PAYLOAD_LEN_WITHOUT_DATA     (14)        /* OA protocol layer overhead + CRC */

#define OA_SPI_BUSY_RSP_LEN                     (3)         /* just status & CRC */

#define OA_SPI_SLEEP_TIME                       (5000)      /* 5ms */
#define OA_SPI_RETRY                            (500)       /* 2.5s */

/* EDFA DFMEA */
#define EDFA_SPI_GET_TONE_STATE_BIT             (SPI_COMMON_CMD_MAX_BIT + 0)
#define EDFA_SPI_SET_DGE_CFG_BIT                (SPI_COMMON_CMD_MAX_BIT + 1)
#define EDFA_SPI_GET_DGE_CFG_BIT                (SPI_COMMON_CMD_MAX_BIT + 2)
#define EDFA_SPI_GET_SWITCH_BIT                 (SPI_COMMON_CMD_MAX_BIT + 3)
#define EDFA_SPI_SET_SWITCH_BIT                 (SPI_COMMON_CMD_MAX_BIT + 4)
#define EDFA_SPI_SET_PWR_CONSUM_BIT             (SPI_COMMON_CMD_MAX_BIT + 5)
#define EDFA_SPI_SET_FPGA_BIT                   (SPI_COMMON_CMD_MAX_BIT + 6)
#define EDFA_SPI_GET_FPGA_BIT                   (SPI_COMMON_CMD_MAX_BIT + 7)
#define EDFA_SPI_SET_CPLD_BIT                   (SPI_COMMON_CMD_MAX_BIT + 8)
#define EDFA_SPI_GET_CPLD_BIT                   (SPI_COMMON_CMD_MAX_BIT + 9)
#define EDFA_SPI_SET_DBG_BAUD_BIT               (SPI_COMMON_CMD_MAX_BIT + 10)
#define EDFA_SPI_GET_DBG_BAUD_BIT               (SPI_COMMON_CMD_MAX_BIT + 11)
#define EDFA_SPI_SET_SPE_CFG                    (SPI_COMMON_CMD_MAX_BIT + 12)
#define EDFA_SPI_GET_UPG_INFO                   (SPI_COMMON_CMD_MAX_BIT + 13)
#define EDFA_SPI_GET_AUTH_CODE                  (SPI_COMMON_CMD_MAX_BIT + 14)
#define EDFA_SPI_GET_EVENT_STATUS_BIT           (SPI_COMMON_CMD_MAX_BIT + 15)

#define EDFA_UPG_FRAGMENT_MAX_LEN                (4096)

typedef enum
{
    EDFA_ILA,
    EDFA_OLT14 = 0,
    EDFA_OLT23,
    EDFA_TYPE_MAX
}EDFA_MODULE_TYPE;

typedef enum
{
    PWRLOAD_ILA,
    PWRLOAD_OLT14,
    PWRLOAD_OLT23,
    PWRLOAD_RAMAN
}EDFA_PWRLOAD_MODULE_TYPE;

typedef enum
{
    EDFA_PWRLOAD_LVL_1,
    EDFA_PWRLOAD_LVL_2,
    EDFA_PWRLOAD_LVL_3,
    EDFA_PWRLOAD_LVL_4
}EDFA_PWRLOAD_LEVEL;

/* any OA SPI communication = command operation       +       response operation */
/*                                 /   \                           /    \        */
/*                       write cmd + read rsp(opt)          read cmd + read rsp  */
/* write command frame header */
typedef struct
{
    uint8_t ucOpCode;

    uint16_t usReserve;
    uint16_t usTID;
    uint16_t usDatalen;     /* message length including message body and the CRC 2 bytes */
}__attribute__((packed)) ST_OA_SPI_WR_CMD_HEADER;

/* read command frame header */
typedef struct
{
    uint8_t ucOpCode;
}__attribute__((packed)) ST_OA_SPI_RD_CMD_HEADER;

/* read response frame header */
typedef struct
{
    uint8_t ucDummy;
    uint16_t usMagicWord;
    uint16_t usTID;
    uint16_t usDatalen;     /* message length including message body and the CRC 2 bytes */
}__attribute__((packed)) ST_OA_SPI_RD_RSP_HEADER;

/* OA SPI protocol: protocol layer format */
/* ------------------------------------------------------------------------------------------ */
/*     0    |   1   |  2-3  |  4-5  |   6   |      7     |    8-9    |  10-11   |  12-(M+11)  */
/*  status  |  R/W  |  OID  |  MID  |  DID  |  instance  |  address  |  length  |    data     */
/* ------------------------------------------------------------------------------------------ */
typedef struct
{
    uint8_t ucStatus;
    uint8_t ucCmd;
    uint16_t usOid;
    uint16_t usMid;
    uint8_t ucDid;
    uint8_t ucInst;
    uint16_t usAddr;
    uint16_t usDataLen;
}__attribute__((packed)) ST_OA_SPI_MSG_HEADER;

typedef struct
{
    ST_OA_SPI_WR_CMD_HEADER stWrCmdHeader;
    ST_OA_SPI_MSG_HEADER stMsgHeader;
}__attribute__((packed)) ST_OA_SPI_CMD_HEADER;

typedef struct
{
    ST_OA_SPI_RD_RSP_HEADER stRdCmdHeader;
    ST_OA_SPI_MSG_HEADER stMsgHeader;
}__attribute__((packed)) ST_OA_SPI_RSP_HEADER;

typedef struct
{
    uint16_t usOid;
    uint16_t usMid;
    uint8_t ucDid;
    uint8_t ucInst;
    uint16_t usAddr;
    uint16_t usDataLen;
}__attribute__((packed)) ST_OA_SPI_MSG_BODY;


#define DEV_DEF_VER_MAX_LEN                 (16u)
#define DEV_DEF_UPGD_MOD_VER_MAX_LEN        (64u)
#define DEV_DEF_UPGD_MOD_VER_RESV_LEN       (DEV_DEF_UPGD_MOD_VER_MAX_LEN - (2 * DEV_DEF_VER_MAX_LEN) - sizeof(uint8_t))


typedef enum
{
    EN_DEV_PARA_VALUE,
    EN_DEV_PARA_SETPOINT,
    EN_DEV_PARA_THR_DL,
    EN_DEV_PARA_THR_DH,
    EN_DEV_PARA_THR_FL,
    EN_DEV_PARA_THR_FH,
    EN_DEV_PARA_THR_DIG,
    EN_DEV_PARA_MAX
} EN_DEV_PARA;

typedef struct
{
    char aucUnitName[32];
    char aucFCPN[32];
    char aucFCIss[32];
    char aucFujitsuPN[32];
    char aucCleiCode[32];
    char aucMac[32];
    char aucFSN[16];
    char acRev[48];
} ST_EXTEN_MODULE_INFO;

typedef struct
{
    char aucSerialNum[32];
    char aucPartNum[32];
    char aucSubPartNum[32];
    char aucHwVer[16];
    char aucBundleVer[16];
    char aucFwBootVer[16];
    char aucFwMainVer[16];
    char aucFpgaVer[16];
    char aucMfgDate[16];
    char aucCaliDate[16];
    char acModuleName[32];
    char aucRsv[16];
#ifdef __NR1004__
    ST_EXTEN_MODULE_INFO stExtModInf;
#endif
} ST_MODULE_INFO;


typedef struct
{
    uint16_t usEventId;
    uint8_t ucPhaseType;
    uint8_t ucPhaseSubType;
    uint8_t ucEventType;
    uint8_t ucEventClass;
    uint16_t usEventNumber;
    uint32_t ulTimestamp;
}ST_EVENT_INFO;

typedef struct
{
    uint16_t usObjID;
    uint16_t usInst;
    uint16_t usTimer;
    uint8_t ucAlarmStatus;
    uint8_t ucRsv;
}ST_ALARM_INFO;

typedef struct
{
    int8_t acActiveBdlVer[DEV_DEF_VER_MAX_LEN];         /* Version Format: XX.YY.ZZZZ */
    int8_t acStandbyBdlVer[DEV_DEF_VER_MAX_LEN];        /* Version Format: XX.YY.ZZZZ */
    uint8_t ucState;                                    /* Upgrade State */
    uint8_t aucReserved[DEV_DEF_UPGD_MOD_VER_RESV_LEN];
}ST_UPGRADE_INFO;

/*
   *@brief define the var resolution of API, SRVL layer in BOARD, EDFA, RAMAN
 * Unit | API  |  EDFA_SRVL  | RAMAN_SRVL | SIM_FILE
 * C    | 0.1  |     0.1     |     0.1    |   1
 * mA   | 0.1  |     0.1     |     0.1    |   1
 * mW   | 0.1  |     0.1     |     0.1    |   1
 * dB   | 0.1  |     0.01    |     0.01   |   1
 * dBm  | 0.1  |     0.01    |     0.01   |   1
 * Use the SRVL layer as the standard:
 *   1)When API set C, mA, mW to SRVL, keep same
     2)When API set dB or dBm to SRVL, need *10. When SRVL return dB/dBm to API, need %10
     3)When SRVL read simlulator C/mA/mW from file, need *10
     4)When SRVL read simlulator dB/dBm from file, need *100
 **/

#define MUTI_10(value)   (10 * value)
#define MUTI_100(value)  (100 * value)
#define DIV_10(value)    (value / 10)
#define DIV_100(value)   (value / 100)
#define DIV_10_ROUND(value)    ((value >= 0) ? ((value + 5) / 10) : ((value - 5) / 10))

/*
   *@brief define the port
 */
#define PORT_NUM_MAX           (128)    /*(70) */

/*
   *@brief define the VOA
 */
#define VOA_NUM_MAX            (4)

/*
   *@brief define the pd
 */
#define OSC_NUM_MAX   (2)

/*
   *@brief define the time
 */
#define TIME_NUM            (6)

/*
   *@brief define the osc direction
 */
#define OSC_DIR_NUM_MAX     (4)

/*
   *@brief define the pd
 */
/*#define PD_MAX_NUM   (80) */

/*
   *@brief define the alm
 */
#define ALM_MAX_NUM   (80)

/*
   *@brief define the alm
 */
#define EVENT_MAX_NUM  (32)

/*
   *@brief define the tempture
 */
#define BOARD_CASE_TEMP_NUM  (4)


#define EDFA_GET_MFG_TRY_MAX_NUM      (16)
#define EDFA_NF_POINT_MAX_NUM         (80)
#define EDFA_IL_PATH_MAX_NUM          (10)

#define EDFA_INVALID_DATA             (-9999)

/**
   *@brief Each loop oper is 50ms * 3 = 150ms
 * Cause DGE data update needs 500ms, just set update time to 600ms
 * The loopdge index is 600/150 = 4
 * This time is TBD.
 */
#define EDFA_LOOP_DGE_INTERVAL        (4)

#define EDFA_PD23_SW_PORT_MAX_NUM     (37)

#define EDFA_DBG_LOGIC_FPGA           (0)
#define EDFA_DBG_LOGIC_CPLD           (1)

#define EDFA_DBG_LOGIC_READ           (0)
#define EDFA_DBG_LOGIC_WRITE          (1)

#define EDFA_DBG_BAUDRATE_READ        (0)
#define EDFA_DBG_BAUDRATE_WRITE       (1)

#define EDFA_SET_COMMON_CFG(value)    (value | 0x00000001)
#define EDFA_SET_THR_CFG(value)       (value | 0x00000002)
#define EDFA_SET_PD_THR_CFG(value)    (value | 0x00000004)

#define EDFA_GET_COMMON_CFG(value)    ((value & 0x00000001) >> 0)
#define EDFA_GET_THR_CFG(value)       ((value & 0x00000002) >> 1)
#define EDFA_GET_PD_THR_CFG(value)    ((value & 0x00000004) >> 2)


/**
 * @brief definition of EDFA index.
 * OLT 0~3 EDFA1/2/3/4
 * ILA 0~3 EDFA_1_PRE, EDFA_1_BOOSTER, EDFA_2_PRE, EDFA_2_BOOSTER
 */
typedef enum {
    EN_EDFA_1 = 0,
    EN_EDFA_2,
    EN_EDFA_3,
    EN_EDFA_4,
    EN_EDFA_MAX
}EN_EDFA_INDEX;

typedef enum {
    EN_EDFA_SUB_PUMP_1 = 0,                            /* 0 */
    EN_EDFA_SUB_PUMP_2,                                /* 1 */
    EN_EDFA23_PUMP_NUM_OLT_L,                          /* 2 */
    EN_EDFA_SUB_PUMP_3 = EN_EDFA23_PUMP_NUM_OLT_L,     /* 2 */
    EN_EDFA_PUMP_NUM_OLT_C,                            /* 3 */
    EN_EDFA_SUB_PUMP_4 = EN_EDFA_PUMP_NUM_OLT_C,       /* 3 */
    EN_EDFA14_PUMP_NUM_OLT_L,                          /* 4 */
    EN_EDFA_SUB_PUMP_5 = EN_EDFA14_PUMP_NUM_OLT_L,     /* 4 */
    EN_EDFA_SUB_PUMP_6,                                /* 5 */
    EN_EDFA_PUMP_NUM_ILA,                              /* 6 */
#ifdef __HCF__
    EN_EDFA_SUB_PUMP_MAX = 8,                          /* 8 */
#else
    EN_EDFA_SUB_PUMP_MAX = EN_EDFA_PUMP_NUM_ILA        /* 6 */
#endif
}EN_EDFA_SUB_PUMP;

typedef enum {
    EN_EDFA_VOA_1 = 0,  /* OLT-C-L OBJID31:Instance 1 Line-TX        ILA-C-L OBJID33:Instance 1 Line-1-TX */
    EN_EDFA_VOA_2,      /* OLT-C-L OBJID31:Instance 3 Noise Loader   ILA-C-L OBJID33:Instance 3 Line-2-TX */
    EN_EDFA_VOA_3,      /* OLT-C-L OBJID33&34:Instance 1 OSC-TX      ILA-C-L OBJID34&35:Instance 1 OSC-1-TX */
    EN_EDFA_VOA_4,      /* OLT-C-L rsv                               ILA-C-L OBJID34&35:Instance 1 OSC-2-TX */
    EN_EDFA_VOA_5,
    EN_EDFA_VOA_6,
    EN_EDFA_VOA_7,
    EN_EDFA_VOA_8,
    EN_EDFA_VOA_MAX
}EN_EDFA_VOA_NUM;

/* line num */
typedef enum
{
    EN_LINE_1 = 0,
    EN_LINE_2,
    EN_LINE_NUM_MAX
}EN_LINE_ENUM;


typedef enum
{
    ILA_C_PORT_LINE_1_RX,
    ILA_C_PORT_LINE_1_TX,
    ILA_C_PORT_LINE_2_RX,
    ILA_C_PORT_LINE_2_TX,
    ILA_C_PORT_LBAND_1_RX,
    ILA_C_PORT_LBAND_1_TX,
    ILA_C_PORT_LBAND_2_RX,
    ILA_C_PORT_LBAND_2_TX,
    ILA_C_EDFA_SUPPORT_PORT_NUM
}ILA_C_EDFA_SUPPORT_PORT_ENUM;

typedef enum
{
    ILA_L_PORT_LINE_1_RX,
    ILA_L_PORT_LINE_1_TX,
    ILA_L_PORT_LINE_2_RX,
    ILA_L_PORT_LINE_2_TX,
    ILA_L_EDFA_SUPPORT_PORT_NUM
}ILA_L_EDFA_SUPPORT_PORT_ENUM;

typedef enum
{
    OLT_PORT_LINE_RX,
    OLT_PORT_LINE_TX,
    OLT_PORT_MON_1_RX,
    OLT_PORT_MON_1_TX,
    OLT_PORT_L_BAND_RX,
    OLT_PORT_L_BAND_TX,
    OLT_PORT_ADD_1_RX,
    OLT_PORT_DROP_1_TX,
    OLT_PORT_ADD_2_RX,
    OLT_PORT_DROP_2_TX,
    OLT_EDFA_SUPPORT_PORT_NUM
}OLT_EDFA_SUPPORT_PORT_ENUM;


/**
 * @brief The following struct is from all_0.01.0003.h(by Lei Mao and Xiang Yan)
 */

/***********************************DGE*************************************/

typedef struct
{
    uint32_t ulSwitch1ChN;
    uint32_t ulSwitch2ChN;
}EDFA_SWITCH_PORT_ST;

typedef struct
{
    EDFA_SWITCH_PORT_ST stEdfaSwitchPort;
    int32_t alMPOPower[48];
}ST_EDFA_SWITCH_PORT_POWER;

/*3.Config data*/
typedef struct {
    /*uint32_t uiChannelsAtten[5][64]; / * DGE1 ADD-Rx Even Ch Attenuation, the linecard need transfer than send to module* / */
    /*uint32_t uiDGE6NoiseLoaderOnOff; / * DGE6 NoiseLoader ON-OFF * / */
    uint16_t usChannelsAtten[774]; /* send one wave block data per time */
}WBA_CFG_ST;


typedef struct {
    uint32_t uiOsriCtrlMode; /* OSRI Control Mode EDFA[1], EDFA2[4], */
    uint32_t uiCtrlMode; /* Safety Control Mode EDFA[1], EDFA[4], */
    uint32_t uiAprPwr; /*APR Power Value,  infomdel mean E1, E4. only E1 E4 have meaning now */
    uint32_t uiForceAPR; /*Safety Force APR EDFA[1], EDFA[4] */
    uint32_t uiOverride; /*Safety Override EDFA[1], EDFA[4], setpoint will different with read back value, set value != read back value */
    uint32_t uiOverTimer; /*Safety Ovr Timer EDFA[1], EDFA[4], setpoint will different with read back value, set value != read back value */
    uint32_t uiRestartEn; /* 2 instance in infomodel, mcu only care EDFA_SAFETY_DATA_ST[0].uiRestartEn, and EDFA_SAFETY_DATA_ST[1].uiRestartEn */
    /*Amplifier Restart Enable setpoint will different with read back value, set value != read back value */
    /*Set to 1 to allowed amplifirer restar, reset to 0 and latched in case of LOS/Safety conditions
       0 -> Laser OFF
       1 -> Allow Laser ON if there's no conditons triggering laser off (LOS or Safety)*/
    uint32_t uiBackREn; /*Enable Back-R as safety condition, */
    uint32_t uiALSAlarm; /*EDFA ALS  Alarm EDFA[1], EDFA[2], */
    uint32_t uiAlmCorrelationDis;
} EDFA_SAFETY_DATA_ST;

typedef struct {
    /*                                                                 OLT */
    uint32_t uiGainRangeMode; /* EDFA1 Gain Range Mode.          only E1, index see EDFA_EDFA_ENUM */
    uint32_t uiCtrlMode;      /* EDFA-n Control Mode.                     index see EDFA_EDFA_ENUM */
    uint32_t uiGain;          /* EDFA-n Gain. OLT:3  ILA :2               index see EDFA_EDFA_ENUM */
    int32_t iGainTilt;        /* EDFA-n Gain Tilt. OLT:3  ILA :2          index see EDFA_EDFA_ENUM */
    int32_t iOutPwr;         /* EDFA-n Output Power  OLT:4  ILA :2       index see EDFA_EDFA_ENUM */
    uint32_t auiLaserCurr[EN_EDFA_SUB_PUMP_MAX];     /* EDFA Laser Current. for ACC??? index mean 4 edfa, per edfa most have 3 pump OLT:4x3  ILA :2x2*/
    /* this struct only put optical set parameter, like gain ...*/
} EDFA_OPTICAL_ST;

typedef struct
{
    int32_t iDegLow;
    int32_t iDegHigh;
    int32_t iFailLow;
    int32_t iFailHigh;

}CMN_THR_ST;

typedef struct
{
    int32_t iDegLow;
    int32_t iDegHigh;
    int32_t iFailLow;
    int32_t iFailHigh;

}CMN_HYS_ST;


typedef struct
{
    CMN_THR_ST stOutOfGainThr; /* EDFA-n Gain, OLT:3  ILA :2 , index see EDFA_EDFA_ENUM */
    CMN_HYS_ST stOutOfGainHys;

    CMN_THR_ST stFiberTempThr;    /* RO, EDFA Fiber Temperature */
    CMN_HYS_ST stFiberTempHys;

    CMN_THR_ST astLaserTempThr[EN_EDFA_SUB_PUMP_MAX];    /* RO, EDFA Laser Temperature */
    CMN_HYS_ST astLaserTempHys[EN_EDFA_SUB_PUMP_MAX];

    CMN_THR_ST astLaserCurrThr[EN_EDFA_SUB_PUMP_MAX];    /* RO, EDFA Laser Current */
    CMN_HYS_ST astLaserCurrHys[EN_EDFA_SUB_PUMP_MAX];

    CMN_THR_ST astBoosterLaserPwrThr[EN_EDFA_SUB_PUMP_MAX];    /* RO, EDFA Laser Current */
    CMN_HYS_ST astBoosterLaserPwrHys[EN_EDFA_SUB_PUMP_MAX];

    CMN_THR_ST stEdfaOutPwrThr;        /* infomodel say thr could wr, but default value is - ??? * / */
    CMN_HYS_ST stEdfaOutPwrHys;

}EDFA_CFG_THR_ST;

typedef struct
{
    int32_t iAtten; /* index see EDFA_VOA_NUM_ENUM, infomodel : VOA-n attenuation */

}VOA_CFG_ST;


typedef struct
{
    uint32_t uiEdfaStatus; /* Module Status: Instance [1-4] EDFA[1-4] OLT:4  ILA :2, index see EDFA_EDFA_ENUM*/
    int32_t iFiberTemp; /* EDFA Fiber Temperature */
    int32_t aiLaserTemp[EN_EDFA_SUB_PUMP_MAX]; /* EDFA Laser Temperature. pump temperature. 4 EDFA, per edfa may have 3 pump */
    uint32_t auiLaserCurr[EN_EDFA_SUB_PUMP_MAX]; /* EDFA Laser Current. pump current */
    uint32_t auiBoosterLaserPwr[EN_EDFA_SUB_PUMP_MAX]; /*  */
    int32_t iActualGain;
    int32_t iCaseTemp;
    int32_t iRsv[1];

}EDFA_STATUS_ST;

/*
   typedef enum
   {
    OLT_PD1_PD1_EDFA1InputPwr = 0,           // E14  C + L
    OLT_PD2_PD2_EDFA1OutputTotalPwr,         // E14  C + L
    OLT_PD2_PD2_EDFA1OutputSignalPwr,        // E14  C + L
    OLT_PD2b_PD2b_EDFA1OutputPwr,            // E14  C + L
    OLT_PD7_PD7_EDFA4InputPwr,               // E14  C + L
    OLT_PD8_PD8_EDFA4OutputTotalPwr = 5,     // E14  C + L
    OLT_PD8_PD8_EDFA4OutputSignalPwr,        // E14  C + L
    OLT_PD8b_PD8b_EDFA4OutputPwr,            // E14  C + L

    OLT_PD14_PD11_EDFA4VoaOutputPwr,         // E14  C + L
    OLT_PD11_PDX_LBandTxPwr,                 // E14  Only C
    OLT_PD12_PDX_LBandRxPwr = 10,            // E14  Only C
    OLT_PD13_PDX_LBandTxInputPwr,            // E14  Only C
    OLT_PD15_PD12_LineTxBackReflectedPwr,    // E14  C + L

    OLT_PD3_PD3_EDFA2InputPwr,               // E23  C + L
    OLT_PD4_PD4_EDFA2OutputTotalPwr,         // E23  C + L
    OLT_PD4_PD4_EDFA2OutputSignalPwr = 15,   // E23  C + L
    OLT_PD5_PD5_EDFA3InputPwr,               // E23  C + L
    OLT_PD6_PD6_EDFA3OutputTotalPwr,         // E23  C + L
    OLT_PD6_PD6_EDFA3OutputSignalPwr,        // E23  C + L
    OLT_PD21_PD16_COMTx2Pwr,                 // E23  C + L

    OLT_PD23_PD18_OCM_Sw1OutPwr = 20,        // E14  C + L
    OLT_PD23_PD18_OCM_Sw2OutPwr,             // E14  C + L

    OLT_PD24_PD19_ADD1RxPwr,                 // E23  C + L
    OLT_PD25_PD20_ADD2RxPwr,                 // E23  C + L

    OLT_LineTxBackRef,                       // E14  C + L, RFL threshold is treated as PD threshold

    OLT_PD_PWR_MAX = 25
   }OLT_PD_PWR_E;
 */
#if 0
typedef enum
{
    PD1_EDFA1_InPwr,              /*e14.oa1.in */
    PD2_EDFA1_OutPwr,             /*e14.oa1.out */
    PD2_EDFA1_OutSig,             /*e14.oa1.outSig */
    PD2b_EDFA1_OutPwr,            /*e14.oa1.out_b */

    PD7_EDFA4_InPwr,              /*e14.oa2.in */
    PD8_EDFA4_OutPwr,             /*e14.oa2.out */
    PD8_EDFA4_OutSig,             /*e14.oa2.outSig */
    PD8b_EDFA4_OutPwr,            /*e14.oa2.out_b */

    PD11_EDFA4_VoAOutPwr,     /*e14.oa2.voa1out */
    PD12_Line_TxRFLPwr,       /*e14.oa2.rfl */

    PD3_EDFA2_InPwr,              /*e23.oa1.in */
    PD4_EDFA2_OutPwr,             /*e23.oa1.out */
    PD4_EDFA2_OutSig,             /*e23.oa1.outSig */
    PD5_EDFA3_InPwr,              /*e23.oa2.in */
    PD6_EDFA3_OutPwr,             /*e23.oa2.out */
    PD6_EDFA3_OutSig,             /*e23.oa2.outSig */
    PD16_COM_TX2Pwr,              /*e23.oa1.pm4 */

    PD18_OCM_Sw1OutPwr,       /*e23.oa1.pm2 <-- 37item */
    PD18_OCM_Sw2OutPwr,       /*e23.oa1.pm3 <-- 37item */
    PD19_ADD1_RXPwr,              /*e23.oa1.pm1 */
    PD20_ADD2_RXPwr,              /*e23.oa2.pm1 */

    RFL4_DeltaPwr,                      /*e14.oa2.DeltaRfl */

    MAX_OLT_PD_THR,
}OLT_PD_PWR_E;

#endif

typedef enum
{
    ILA_PD1_LINE1RX,
    ILA_PD2_EDFA1OutputPwrTotal,
    ILA_PD2_EDFA1OutputPwrSignal,
    ILA_PD2b_EDFA1OutputPwrTotal,
    ILA_PD3_LINE2RX,
    ILA_PD4_EDFA2OutputPwrTotal,
    ILA_PD4_EDFA2OutputPwrSignal,
    ILA_PD4b_EDFA2OutputPwrTotal,

    /* C Band */
    ILA_C_PD9_LBand2TX_Pwr,
    ILA_C_PD10_LBand1TX_Pwr,
    ILA_C_PD11_LBand2RX,
    ILA_C_PD12_LBand1RX,
    ILA_C_PD13_Line2VOAOut_Pwr,
    ILA_C_PD14_Line1VOAOut_Pwr,
    ILA_C_PD15_Line2TXBackRefPwr,
    ILA_C_PD16_Line1TXBackRefPwr,
    ILA_C_PD23_L_Band2TxPwr,
    ILA_C_PD24_L_Band1TxPwr,
    ILA_C_Line2TxBackRef,
    ILA_C_Line1TxBackRef,
    ILA_C_PD_PWR_MAX,
    ILA_PD_RESERVED_1 = ILA_C_PD_PWR_MAX,
    ILA_PD_RESERVED_2,
    ILA_PD_RESERVED_3,
    ILA_PD_RESERVED_4,
    ILA_PD_RESERVED_5,
    ILA_PD_RESERVED_6,
    ILA_PD_RESERVED_7,
    ILA_PD_RESERVED_8,
    ILA_PD_RESERVED_9,
    ILA_PD_RESERVED_10,
    ILA_PD_RESERVED_11,
    ILA_PD_RESERVED_12
}ILA_C_PD_PWR_E;

typedef enum
{
    /*ILA_PD1_LINE1RX, */
    /*ILA_PD2_EDFA1OutputPwrTotal, */
    /*ILA_PD2_EDFA1OutputPwrSignal, */
    /*ILA_PD2b_EDFA1OutputPwrTotal, */
    /*ILA_PD3_LINE2RX, */
    /*ILA_PD4_EDFA2OutputPwrTotal, */
    /*ILA_PD4_EDFA2OutputPwrSignal, */
    /*ILA_PD4b_EDFA2OutputPwrTotal, */

    /* L Band */
    ILA_L_PD9_Line2VOAOut_Pwr = ILA_C_PD9_LBand2TX_Pwr,
    ILA_L_PD10_Line1VOAOut_Pwr = ILA_C_PD10_LBand1TX_Pwr,
    ILA_L_PD11_Line2TXBackRefPwr = ILA_C_PD11_LBand2RX,
    ILA_L_PD12_Line1TXBackRefPwr = ILA_C_PD12_LBand1RX,
    ILA_L_Line2TxBackRef = ILA_C_PD13_Line2VOAOut_Pwr,
    ILA_L_Line1TxBackRef = ILA_C_PD14_Line1VOAOut_Pwr,
    ILA_L_PD_PWR_MAX
}ILA_L_PD_PWR_E;

/* same with OLT_PD_PWR_E */
typedef enum
{
    OLT_PD1_PD1_EDFA1InputPwrThr = 0,           /* E14  C + L */
    OLT_PD2_PD2_EDFA1OutputTotalPwrThr,         /* E14  C + L */
    OLT_PD2_PD2_EDFA1OutputSignalPwrThr,        /* E14  C + L */
    OLT_PD2b_PD2b_EDFA1OutputPwrThr,            /* E14  C + L */
    OLT_PD7_PD7_EDFA4InputPwrThr,               /* E14  C + L */
    OLT_PD8_PD8_EDFA4OutputTotalPwrThr = 5,     /* E14  C + L */
    OLT_PD8_PD8_EDFA4OutputSignalPwrThr,        /* E14  C + L */
    OLT_PD8b_PD8b_EDFA4OutputPwrThr,            /* E14  C + L */

    OLT_PD14_PD11_EDFA4VoaOutputPwrThr,         /* E14  C + L */
    OLT_PD11_PDX_LBandTxPwrThr,                 /* E14  Only C */
    OLT_PD12_PDX_LBandRxPwrThr = 10,            /* E14  Only C */
    OLT_PD13_PDX_LBandTxInputPwrThr,            /* E14  Only C */
    OLT_PD15_PD12_LineTxBackReflectedPwrThr,    /* E14  C + L */

    OLT_PD3_PD3_EDFA2InputPwrThr,               /* E23  C + L */
    OLT_PD4_PD4_EDFA2OutputTotalPwrThr,         /* E23  C + L */
    OLT_PD4_PD4_EDFA2OutputSignalPwrThr = 15,   /* E23  C + L */
    OLT_PD5_PD5_EDFA3InputPwrThr,               /* E23  C + L */
    OLT_PD6_PD6_EDFA3OutputTotalPwrThr,         /* E23  C + L */
    OLT_PD6_PD6_EDFA3OutputSignalPwrThr,        /* E23  C + L */
    OLT_PD21_PD16_COMTx2PwrThr,                 /* E23  C + L */

    OLT_PD23_PD18_OCM_Sw1OutPwrThr = 20,        /* E14  C + L */
    OLT_PD23_PD18_OCM_Sw2OutPwrThr,             /* E14  C + L */

    OLT_PD24_PD19_ADD1RxPwrThr,                 /* E23  C + L */
    OLT_PD25_PD20_ADD2RxPwrThr,                 /* E23  C + L */

    OLT_LineTxBackRefThr,                       /* E14  C + L, RFL threshold is treated as PD threshold */

    OLT_PD_THR_MAX = 25
}OLT_PD_THR_IDX_E;

typedef enum {
    ILA_PD1_Line1RxThr,
    ILA_PD3_Line2RxThr,

    /* C Band */
    ILA_C_PD9_LBand2TxPwrThr,
    ILA_C_PD10_LBand1TxPwrThr,
    ILA_C_PD11_LBand2RxThr,
    ILA_C_PD12_LBand1RxThr,
    ILA_C_PD13_Line2VoaOutPwrThr,
    ILA_C_PD14_Line1VoaOutPwrThr,
    ILA_C_PD15_Line2TxBackRefPowerThr,
    ILA_C_PD16_Line1TxBackRefPowerThr,
    ILA_C_PD23_L_Band2TxThr,
    ILA_C_PD24_L_Band1TxThr,
    ILA_C_Line2TxBackRefThr,    /* RFL threshold is treated as PD threshold */
    ILA_C_Line1TxBackRefThr,    /* RFL threshold is treated as PD threshold */
    ILA_C_PD_THR_MAX,
    ILA_PD_THR_RESERVED_1 = ILA_C_PD_THR_MAX,
    ILA_PD_THR_RESERVED_2,
    ILA_PD_THR_RESERVED_3,
    ILA_PD_THR_RESERVED_4,
    ILA_PD_THR_RESERVED_5,
    ILA_PD_THR_RESERVED_6,
    ILA_PD_THR_RESERVED_7,
    ILA_PD_THR_RESERVED_8,
    ILA_PD_THR_RESERVED_9,
    ILA_PD_THR_RESERVED_10,
    ILA_PD_THR_RESERVED_11,
    ILA_PD_THR_RESERVED_12,
    ILA_PD_THR_RESERVED_13,
    ILA_PD_THR_RESERVED_14,
    ILA_PD_THR_RESERVED_15,
    ILA_PD_THR_RESERVED_16,
    ILA_PD_THR_RESERVED_17,
    ILA_PD_THR_RESERVED_18
}ILA_C_PD_THR_IDX_E;

typedef enum {
    /*ILA_PD1_Line1RxThr, */
    /*ILA_PD3_Line2RxThr, */

    /* L Band */
    ILA_L_PD9_Line2VoaOutPwrThr = ILA_C_PD9_LBand2TxPwrThr,
    ILA_L_PD10_Line1VoaOutPwrThr = ILA_C_PD10_LBand1TxPwrThr,
    ILA_L_PD11_Line2TxBackRefPowerThr = ILA_C_PD11_LBand2RxThr,
    ILA_L_PD12_Line1TxBackRefPowerThr = ILA_C_PD12_LBand1RxThr,
    ILA_L_Line2TxBackRefThr = ILA_C_PD13_Line2VoaOutPwrThr,
    ILA_L_Line1TxBackRefThr = ILA_C_PD14_Line1VoaOutPwrThr,
    ILA_L_PD_THR_MAX
}ILA_L_PD_THR_IDX_E;

typedef enum
{
    EDFA_TONE_DET_PD11,
    EDFA_TONE_DET_PD12,
    EDFA_TONE_DET_PD23,
    EDFA_TONE_DET_PD24,
    EDFA_TONE_DET_PD_NUM
}EDFA_ILA_TONE_DET_PD_IDX_E;

typedef enum
{
    EDFA14_TONE_DET_PD12,
    EDFA14_TONE_DET_PD13,
    EDFA14_OLT_TONE_DET_PD_NUM
}EDFA14_OLT_TONE_DET_PD_IDX_E;

typedef enum
{
    EDFA23_TONE_DET_PD3,
    EDFA23_TONE_DET_PD5,
    EDFA23_TONE_DET_PD21,
    EDFA23_TONE_DET_PD24,
    EDFA23_TONE_DET_PD25,
    EDFA23_OLT_TONE_DET_PD_NUM
}EDFA23_OLT_TONE_DET_PD_IDX_E;

typedef struct
{
    CMN_THR_ST stPDThr; /* index see OLT_PD_THR_IDX_E  ILA_PD_THR_IDX_E*/
    CMN_HYS_ST stPDHys; /* index see OLT_PD_THR_IDX_E  ILA_PD_THR_IDX_E*/
}PDS_THR_HYS_T;

typedef struct
{
    uint32_t auiToneDetectCtrl[5];              /* Connectivity Check Tone Detection Control, 1 */
    uint32_t auiPattern[5];                     /*Connectivity Check Size of Pattern, 1~32  defualt 4 */
    uint32_t auiNumOfSamplesNon[5];             /*Connectivity Check Number of Samples N-on, 8~32  defualt 8 */
    uint32_t auiNumOfSamplesNoff[5];            /*Connectivity Check Number of Samples N-off, 8~32  defualt 8 */
    int32_t aiToneDetTriggerThr[5];
    uint32_t auiSamplingRate[5];                /* Connectivity Check Sampling Rate, 2~25  defualt 2 */
}TONE_CONFIG_ST;

typedef struct
{
    uint32_t auiToneDetectCtrlStatus[5];        /* Connectivity Check Tone Detection Control, 0~1  default 0 */
    uint32_t auiPattern[5];                     /*Connectivity Check Size of Pattern, 1~32  defualt 4 */
    uint32_t auiNumOfSamplesNon[5];             /*Connectivity Check Number of Samples N-on, 8~32  defualt 8 */
    uint32_t auiNumOfSamplesNoff[5];            /*Connectivity Check Number of Samples N-off, 8~32  defualt 8 */
    int32_t aiToneDetTriggerThr[5];
    uint32_t auiSamplingRate[5];                /* Connectivity Check Sampling Rate, 2~25  defualt 2 */
    uint32_t auiToneDetectSamplePwr[5][8];      /*Connectivity Check Tone Detection Sampled Power */
}TONE_STATUS_ST;

typedef struct
{
    TONE_STATUS_ST astToneState[2];
}ST_EDFA_TONE_STATE;

typedef struct {
    uint32_t ulNoiseLoader;
    uint8_t aucResv[60];
} EDFA_NOISE_LOADER_ST;

#define EDFA_PORT_MAX             (14)
#define EDFA_PD_MAX_NUM           (32)   /* (25) */

#if 0
typedef struct
{
    /* 1. EDFA_ALL_CFG_ST Config struct, like voa attenuation, this is write only     */
    EDFA_OPTICAL_ST astEdfaCfg[EN_EDFA_MAX];
    EDFA_CFG_THR_ST astEdfaThrCfg[EN_EDFA_MAX];
    VOA_CFG_ST astVoaCfg[EN_EDFA_VOA_MAX];              /* index see EDFA_VOA_NUM_ENUM, infomodel : VOA-n attenuation
                                                           in olt, all evoa in E1E4  */

    uint32_t auiPortStatusCfg[EDFA_PORT_MAX];  /* Port Status , OLT's index see OLT_EDFA_SUPPORT_PORT_ENUM, ILA see ILA_EDFA_SUPPORT_PORT_ENUM
                                                  in olt, need both send E1E4 and E2E3 for now */

    PDS_THR_HYS_T astPdsThrCfg[EDFA_PD_MAX_NUM];       /* index see OLT_PD_THR_IDX_E  ILA_PD_THR_IDX_E,  and in OLT_PD_THR_IDX_E indicate which need send E1E4 */

    EDFA_SAFETY_DATA_ST astSafetyCfg[EN_EDFA_MAX];

    TONE_CONFIG_ST astToneCfg[2];

    EDFA_NOISE_LOADER_ST stNoiseLoader;
} ST_EDFA_CONFIG;
#else
typedef struct
{
    EDFA_OPTICAL_ST astEdfaCfg[EN_EDFA_MAX];                 /* EDFA_ALL_CFG_ST Config struct, like voa attenuation, this is write only     */
    VOA_CFG_ST astVoaCfg[EN_EDFA_VOA_MAX];                   /* index see EDFA_VOA_NUM_ENUM, infomodel : VOA-n attenuation.  in olt, all evoa in E1E4  */
    uint32_t auiPortStatusCfg[EDFA_PORT_MAX];                /* Port Status , OLT's index see OLT_EDFA_SUPPORT_PORT_ENUM, ILA see ILA_EDFA_SUPPORT_PORT_ENUM. in olt, need both send E1E4 and E2E3 for now */
    EDFA_SAFETY_DATA_ST astSafetyCfg[EN_EDFA_MAX];
    TONE_CONFIG_ST astToneCfg[2];
    EDFA_NOISE_LOADER_ST stNoiseLoader;
    uint32_t auiLimitedPwrMode[2];
    uint8_t aucResv[288];
} ST_EDFA_COMMON_CONFIG;
#endif

typedef struct
{
    EDFA_CFG_THR_ST astEdfaThrCfg[EN_EDFA_MAX];
    uint8_t aucResv[384];
}ST_EDFA_CFG_THR;

typedef struct
{
    PDS_THR_HYS_T astPdsThrCfg[EDFA_PD_MAX_NUM];
    uint8_t aucResv[512];
}ST_EDFA_PD_THR;

typedef struct
{
    ST_EDFA_COMMON_CONFIG stEdfaCommonCfg;
    ST_EDFA_CFG_THR stEdfaThrCfg;
    ST_EDFA_PD_THR stPdsThrCfg;
}ST_EDFA_CONFIG;

typedef enum
{
    EDFA_CFG_TYPE_NULL,
    EDFA_CFG_TYPE_MODE,
    EDFA_CFG_TYPE_GAIN,
    EDFA_CFG_TYPE_POWER,
    EDFA_CFG_TYPE_LASER_CURR_1,
    EDFA_CFG_TYPE_LASER_CURR_2,
    EDFA_CFG_TYPE_LASER_CURR_3,
    EDFA_CFG_TYPE_LASER_CURR_4,
    EDFA_CFG_TYPE_NUM
}EDFA_CFG_TYPE_E;

typedef struct
{
    uint32_t uiEdfaIdx;
    uint32_t uiType;
    int32_t iValue;
} ST_EDFA_SPE_CONFIG;

typedef struct
{
    int32_t aaiNoiseFigure[EN_EDFA_MAX][EDFA_NF_POINT_MAX_NUM];   /* Noise Figure EDFA1-4  OLT:4x40  ILA :2x40*/

    int32_t aalIL[EN_EDFA_MAX][EDFA_IL_PATH_MAX_NUM]; /* 1. OLT: index see OLT_EDFA_PORT_IL_ENUM, and only alIL[0][10] need, */
    /* 2. ILA: index see ILA_EDFA_PORT_IL_ENUM */

}MISC_STAUTS_ST;

typedef struct
{
    uint32_t uiStatus; /* 0 = Module Ready - Provisioning Completed, */
                       /* 1 = Module Busy - Provisioning Ongoing, */
    uint32_t uiDGE6NoiseLoaderOnOff; /* DGE6 NoiseLoader ON-OFF , only for OLT, and send E2E3*/
}OLT_ALL_WBA_STATUS;

typedef enum  {
    OLT_ALARM_EDFA1_GAIN,
    OLT_ALARM_EDFA2_GAIN,
    OLT_ALARM_EDFA3_GAIN,
    OLT_ALARM_EDFA4_GAIN,
    OLT_ALARM_EDFA1_O_PWR,
    OLT_ALARM_EDFA2_O_PWR,
    OLT_ALARM_EDFA3_O_PWR,
    OLT_ALARM_EDFA4_O_PWR,
    OLT_ALARM_EDFA1_FIBER_TEMP,
    OLT_ALARM_EDFA2_FIBER_TEMP,
    OLT_ALARM_EDFA3_FIBER_TEMP,
    OLT_ALARM_EDFA4_FIBER_TEMP,

    /* pump temp(C band) */
    OLT_ALARM_C_EDFA1_LASER1_TEMP,
    OLT_ALARM_C_EDFA1_LASER2_TEMP,
    OLT_ALARM_C_EDFA1_LASER3_TEMP,
    OLT_ALARM_C_EDFA2_LASER1_TEMP,
    OLT_ALARM_C_EDFA2_LASER2_TEMP,
    OLT_ALARM_C_EDFA2_LASER3_TEMP,
    OLT_ALARM_C_EDFA3_LASER1_TEMP,
    OLT_ALARM_C_EDFA3_LASER2_TEMP,
    OLT_ALARM_C_EDFA3_LASER3_TEMP,
    OLT_ALARM_C_EDFA4_LASER1_TEMP,
    OLT_ALARM_C_EDFA4_LASER2_TEMP,
    OLT_ALARM_C_EDFA4_LASER3_TEMP,

    /* pump temp(L band) */
    OLT_ALARM_L_EDFA1_LASER1_TEMP = OLT_ALARM_C_EDFA1_LASER1_TEMP,
    OLT_ALARM_L_EDFA1_LASER2_TEMP,
    OLT_ALARM_L_EDFA1_LASER3_TEMP,
    OLT_ALARM_L_EDFA1_LASER4_TEMP,
    OLT_ALARM_L_EDFA2_LASER1_TEMP,
    OLT_ALARM_L_EDFA2_LASER2_TEMP,
    OLT_ALARM_L_EDFA3_LASER1_TEMP,
    OLT_ALARM_L_EDFA3_LASER2_TEMP,
    OLT_ALARM_L_EDFA4_LASER1_TEMP,
    OLT_ALARM_L_EDFA4_LASER2_TEMP,
    OLT_ALARM_L_EDFA4_LASER3_TEMP,
    OLT_ALARM_L_EDFA4_LASER4_TEMP,

    /* pump current(C band) */
    OLT_ALARM_C_EDFA1_LASER1_CURR,
    OLT_ALARM_C_EDFA1_LASER2_CURR,
    OLT_ALARM_C_EDFA1_LASER3_CURR,
    OLT_ALARM_C_EDFA2_LASER1_CURR,
    OLT_ALARM_C_EDFA2_LASER2_CURR,
    OLT_ALARM_C_EDFA2_LASER3_CURR,
    OLT_ALARM_C_EDFA3_LASER1_CURR,
    OLT_ALARM_C_EDFA3_LASER2_CURR,
    OLT_ALARM_C_EDFA3_LASER3_CURR,
    OLT_ALARM_C_EDFA4_LASER1_CURR,
    OLT_ALARM_C_EDFA4_LASER2_CURR,
    OLT_ALARM_C_EDFA4_LASER3_CURR,

    /* pump current(L band) */
    OLT_ALARM_L_EDFA1_LASER1_CURR = OLT_ALARM_C_EDFA1_LASER1_CURR,
    OLT_ALARM_L_EDFA1_LASER2_CURR,
    OLT_ALARM_L_EDFA1_LASER3_CURR,
    OLT_ALARM_L_EDFA1_LASER4_CURR,
    OLT_ALARM_L_EDFA2_LASER1_CURR,
    OLT_ALARM_L_EDFA2_LASER2_CURR,
    OLT_ALARM_L_EDFA3_LASER1_CURR,
    OLT_ALARM_L_EDFA3_LASER2_CURR,
    OLT_ALARM_L_EDFA4_LASER1_CURR,
    OLT_ALARM_L_EDFA4_LASER2_CURR,
    OLT_ALARM_L_EDFA4_LASER3_CURR,
    OLT_ALARM_L_EDFA4_LASER4_CURR,

    /* pump power(C band) */
    OLT_ALARM_C_EDFA1_BST_LASER1_PWR,
    OLT_ALARM_C_EDFA1_BST_LASER2_PWR,
    OLT_ALARM_C_EDFA1_BST_LASER3_PWR,
    OLT_ALARM_C_EDFA2_BST_LASER1_PWR,
    OLT_ALARM_C_EDFA2_BST_LASER2_PWR,
    OLT_ALARM_C_EDFA2_BST_LASER3_PWR,
    OLT_ALARM_C_EDFA3_BST_LASER1_PWR,
    OLT_ALARM_C_EDFA3_BST_LASER2_PWR,
    OLT_ALARM_C_EDFA3_BST_LASER3_PWR,
    OLT_ALARM_C_EDFA4_BST_LASER1_PWR,
    OLT_ALARM_C_EDFA4_BST_LASER2_PWR,
    OLT_ALARM_C_EDFA4_BST_LASER3_PWR,

    /* pump power(L band) */
    OLT_ALARM_L_EDFA1_BST_LASER1_PWR = OLT_ALARM_C_EDFA1_BST_LASER1_PWR,
    OLT_ALARM_L_EDFA1_BST_LASER2_PWR,
    OLT_ALARM_L_EDFA1_BST_LASER3_PWR,
    OLT_ALARM_L_EDFA1_BST_LASER4_PWR,
    OLT_ALARM_L_EDFA2_BST_LASER1_PWR,
    OLT_ALARM_L_EDFA2_BST_LASER2_PWR,
    OLT_ALARM_L_EDFA3_BST_LASER1_PWR,
    OLT_ALARM_L_EDFA3_BST_LASER2_PWR,
    OLT_ALARM_L_EDFA4_BST_LASER1_PWR,
    OLT_ALARM_L_EDFA4_BST_LASER2_PWR,
    OLT_ALARM_L_EDFA4_BST_LASER3_PWR,
    OLT_ALARM_L_EDFA4_BST_LASER4_PWR,

    OLT_ALARM_PD1_EDFA1_IN_PWR,
    OLT_ALARM_PD2_EDFA1_OUT_TOTAL_PWR,
    OLT_ALARM_PD2_EDFA1_OUT_SIGNAL_PWR,
    OLT_ALARM_PD2B_EDFA1_OUT_PWR,
    OLT_ALARM_PD3_EDFA2_IN_PWR,
    OLT_ALARM_PD4_EDFA2_OUT_TOTAL_PWR,
    OLT_ALARM_PD4_EDFA2_OUT_SIGNAL_PWR,
    OLT_ALARM_PD5_EDFA3_IN_PWR,
    OLT_ALARM_PD6_EDFA3_OUT_TOTAL_PWR,
    OLT_ALARM_PD6_EDFA3_OUT_SIGNAL_PWR,
    OLT_ALARM_PD7_EDFA4_IN_PWR,
    OLT_ALARM_PD8_EDFA4_OUT_TOTAL_PWR,
    OLT_ALARM_PD8_EDFA4_OUT_SIGNAL_PWR,
    OLT_ALARM_PD8B_EDFA4_OUT_PWR,
    OLT_ALARM_PD14_PD11_EDFA4_VOA_OUT_PWR,

    /* EDFA14 */
    OLT_ALARM_L_PD12_TX_BR_PWR,

    /* EDFA23 */
    OLT_ALARM_L_PD16_COM_TX2_PWR,
    OLT_ALARM_L_PD19_ADD1_RX_PWR,
    OLT_ALARM_L_PD20_ADD2_RX_PWR,

    /* EDFA14 */
    OLT_ALARM_L_EDFA1_ALS,
    OLT_ALARM_L_EDFA4_ALS,
    OLT_ALARM_L_L_TX_BR,

    /* EDFA14 */
    OLT_ALARM_C_PD11_LB_TX_PWR = OLT_ALARM_L_PD12_TX_BR_PWR,
    OLT_ALARM_C_PD12_LB_RX_PWR,
    OLT_ALARM_C_PD13_LB_TX_IN_PWR,
    OLT_ALARM_C_PD15_TX_BR_PWR,

    /* EDFA23 */
    OLT_ALARM_C_PD21_COM_TX2_PWR,
    OLT_ALARM_C_PD24_ADD1_RX_PWR,
    OLT_ALARM_C_PD25_ADD2_RX_PWR,

    /* EDFA14 */
    OLT_ALARM_C_EDFA1_ALS,
    OLT_ALARM_C_EDFA4_ALS,
    OLT_ALARM_C_L_TX_BR,
    OLT_ALARM_CNT

}OLT_EDFA_ALM_ID_E;

typedef enum {
    ILA_ALARM_EDFA1_GAIN,
    ILA_ALARM_EDFA2_GAIN,
    ILA_ALARM_EDFA1_O_PWR,
    ILA_ALARM_EDFA2_O_PWR,
    ILA_ALARM_EDFA1_PRE_FIBER_TEMP,
    ILA_ALARM_EDFA1_BST_FIBER_TEMP,
    ILA_ALARM_EDFA2_PRE_FIBER_TEMP,
    ILA_ALARM_EDFA2_BST_FIBER_TEMP,
    ILA_ALARM_EDFA1_LASER1_TEMP,
    ILA_ALARM_EDFA1_LASER2_TEMP,
    ILA_ALARM_EDFA1_LASER3_TEMP,
    ILA_ALARM_EDFA2_LASER1_TEMP,
    ILA_ALARM_EDFA2_LASER2_TEMP,
    ILA_ALARM_EDFA2_LASER3_TEMP,
    ILA_ALARM_EDFA3_LASER1_TEMP,
    ILA_ALARM_EDFA3_LASER2_TEMP,
    ILA_ALARM_EDFA3_LASER3_TEMP,
    ILA_ALARM_EDFA4_LASER1_TEMP,
    ILA_ALARM_EDFA4_LASER2_TEMP,
    ILA_ALARM_EDFA4_LASER3_TEMP,
    ILA_ALARM_EDFA1_LASER1_CURR,
    ILA_ALARM_EDFA1_LASER2_CURR,
    ILA_ALARM_EDFA1_LASER3_CURR,
    ILA_ALARM_EDFA2_LASER1_CURR,
    ILA_ALARM_EDFA2_LASER2_CURR,
    ILA_ALARM_EDFA2_LASER3_CURR,
    ILA_ALARM_EDFA3_LASER1_CURR,
    ILA_ALARM_EDFA3_LASER2_CURR,
    ILA_ALARM_EDFA3_LASER3_CURR,
    ILA_ALARM_EDFA4_LASER1_CURR,
    ILA_ALARM_EDFA4_LASER2_CURR,
    ILA_ALARM_EDFA4_LASER3_CURR,
    ILA_ALARM_EDFA1_BST_LASER1_PWR,
    ILA_ALARM_EDFA1_BST_LASER2_PWR,
    ILA_ALARM_EDFA1_BST_LASER3_PWR,
    ILA_ALARM_EDFA2_BST_LASER1_PWR,
    ILA_ALARM_EDFA2_BST_LASER2_PWR,
    ILA_ALARM_EDFA2_BST_LASER3_PWR,
    ILA_ALARM_EDFA3_BST_LASER1_PWR,
    ILA_ALARM_EDFA3_BST_LASER2_PWR,
    ILA_ALARM_EDFA3_BST_LASER3_PWR,
    ILA_ALARM_EDFA4_BST_LASER1_PWR,
    ILA_ALARM_EDFA4_BST_LASER2_PWR,
    ILA_ALARM_EDFA4_BST_LASER3_PWR,
    ILA_ALARM_PD1_L1RX,
    ILA_ALARM_PD3_L2RX,
    ILA_ALARM_PD9_LB2_TX,
    ILA_ALARM_PD10_LB1_TX,
    ILA_ALARM_PD11_LB2_RX,
    ILA_ALARM_PD12_LB1_RX,
    ILA_ALARM_PD13_L2_VOA,
    ILA_ALARM_PD14_L1_VOA,
    ILA_ALARM_PD15_L2_TX_BR,
    ILA_ALARM_PD16_L1_TX_BR,
    ILA_ALARM_PD23_LB2_TX_RCVDIR,
    ILA_ALARM_PD24_LB1_TX_RCVDIR,
    ILA_ALARM_EDFA1_ALS,
    ILA_ALARM_EDFA2_ALS,
    ILA_ALARM_CNT

}ILA_EDFA_ALM_ID_E;

typedef struct {

    uint32_t uiRstType; /* 0: TA 1:NTA */
}SYSTEM_STATUS_T;

#if 0
typedef struct
{
    EDFA_OPTICAL_ST astEdfaCfg[EN_EDFA_MAX];
    EDFA_CFG_THR_ST astEdfaThrCfg[EN_EDFA_MAX];
    VOA_CFG_ST astVoaCfg[EN_EDFA_VOA_MAX];
    uint32_t auiPortStatusCfg[EDFA_PORT_MAX];
    PDS_THR_HYS_T astPdsThrCfg[EDFA_PD_MAX_NUM];

    EDFA_SAFETY_DATA_ST astSafetyCfg[EN_EDFA_MAX];

    /* 2. status struct , like actual voa value, case temperatrue */
    EDFA_STATUS_ST astEdfaStatus[EN_EDFA_MAX];
    VOA_CFG_ST astVoaStatus[EN_EDFA_VOA_MAX];
    int32_t aiPDPwr[EDFA_PD_MAX_NUM];  /* INDEX:  OLT_PD_PWR_E   ILA_PD_PWR_E */

    OLT_ALL_WBA_STATUS stWbaStatus;
    ST_UPGRADE_INFO astUpg[EN_EDFA_MAX];
    SYSTEM_STATUS_T astSysStatus[EN_EDFA_MAX];
    ST_EVENT_INFO astEvent[EVENT_MAX_NUM];
    ST_ALARM_INFO astEdfaAlm[ALM_MAX_NUM]; /* INDEX: see OLT_ALM_ID_E  ILA_ALM_ID_E */
    EDFA_NOISE_LOADER_ST stNoiseLoader;
}ST_EDFA_OPER;
#else
typedef struct
{
    EDFA_OPTICAL_ST astEdfaCfg[EN_EDFA_MAX];
    uint32_t auiPortStatusCfg[EDFA_PORT_MAX];
    EDFA_SAFETY_DATA_ST astSafetyCfg[EN_EDFA_MAX];
    EDFA_STATUS_ST astEdfaStatus[EN_EDFA_MAX];             /* 2. status struct , like actual voa value, case temperatrue */
    VOA_CFG_ST astVoaStatus[EN_EDFA_VOA_MAX];
    int32_t aiPDPwr[EDFA_PD_MAX_NUM];                      /* INDEX:  OLT_PD_PWR_E   ILA_PD_PWR_E */
    OLT_ALL_WBA_STATUS stWbaStatus;
    SYSTEM_STATUS_T astSysStatus[EN_EDFA_MAX];
    EDFA_NOISE_LOADER_ST stNoiseLoader;
    uint32_t auiLimitedPwrMode[2];
    uint32_t uiOscAddPower;
    uint32_t uiOscDropPower;
    uint32_t uiDFBOnOff;
    uint32_t uiDFBPwr;
    uint32_t uiOscOnOff;
    uint32_t uiApsdOnOff;
    /* uint32_t uiApsdStatus; */
#ifdef __HCF__
    uint16_t uiOtdrSwitchInEx;
    uint16_t uiOtdrSwitch;
#else
    uint32_t uiOtdrSwitch;
#endif
    uint32_t uiSccUpgSta;
    uint32_t uiTestLedSta;
    uint32_t uiLedAlmSta;
    uint8_t aucLedSta[OA_LED_MAX_NUM];  /* LED0 ACT, LED1 ALM, LED2 OTDR, LED3  OCM, LED4 CLINT(Mux R), LED5 LINE(Line R) */

    EDFA_LASER_SAFETY_ST stLaserSafety;
#ifdef __HCF__
    uint8_t aucResv0[8];
    uint32_t auiAutogainSt[4];
    uint32_t auiAutogainMode[4];
    /* uint8_t aucResv1[40]; */    /* alhpa 162 beta 40 */
    uint16_t usRemoteLockEn;
    uint16_t usAdminKey;
    uint32_t uiPanelSafe;
    uint32_t auiApcTargetPwr[4];
    uint32_t auiReadyPwr[4];
#else
    CMN_THR_HYS_INFO_ST astThrHys[OA_THR_PORT_MAX][OA_THR_TYPE_MAX];
#endif
    uint32_t aucAlmMask[2];
    uint8_t aucResv[8];           /* 264 */
}ST_EDFA_OPER;
#endif

typedef struct
{
    int32_t aiOcmSwPDPwr[2];                      /* OLT_PD23_PD18_OCM_Sw1OutPwr  OLT_PD23_PD18_OCM_Sw2OutPwr */
    uint64_t ullTimestamp;                        /* ms */
}ST_EDFA_OCM_SWITCH_PD_PWR;

typedef struct {
    ST_UPGRADE_INFO astEdfaInfo[EN_EDFA_MAX];
}ST_EDFA_UPG_INFO;

typedef struct {
    ST_ALARM_INFO astAlm[ALM_MAX_NUM];
} ST_EDFA_ALARM; /* TBD */

typedef struct {
    uint16_t usWriteIdx;
    uint16_t usReadIdx;
    ST_EVENT_INFO astEvent[EVENT_MAX_NUM];
} ST_EDFA_EVENT_FIFO; /* TBD */

typedef struct {
    uint32_t ulSnapSpot;
    uint32_t ulModuleAlarm;
    uint32_t ulLogicPumpTemp;
    uint32_t ulLogicPumpILD;
    uint32_t ulLogicPumpBfm;
    uint32_t ulHWPump;
    uint32_t ulVoaAlarm;
    uint32_t ulPdLosAlarm;
    uint32_t aulRes[2];
    uint32_t ulPowerVolt;
    uint32_t ulVoaDac;
    uint32_t ulPumpDacDiff;
    uint32_t ulPumpSLD;
    uint32_t ulSwitch;
    uint32_t ulTableCheck;
} ST_EDFA_EVENT_SNAPSHOT;

typedef struct {
    ST_EDFA_EVENT_SNAPSHOT astEventSnapshot[EN_EDFA_MAX];
} ST_EDFA_EVENT_STATUS;

typedef struct
{
    WBA_CFG_ST stOltWbaCfg;         /* only for OLT, and send from E2E3 */
                                    /* there are 6 wave block, but only can send one wave block per time */
}ONE_WBA_CFG_STATUS_T;

typedef struct {
    TONE_CONFIG_ST stToneCfg[2];
    TONE_STATUS_ST stToneStatus[2];
}TONE_OPER_DATA_ST;

typedef struct
{
    ST_MODULE_INFO stEdfaMfg;
    ST_MODULE_INFO stWbaMfg;

}DBG_MFG_T;

typedef struct {
    /* 3. capability , it will not change in application or change a few time */
    MISC_STAUTS_ST stMiscStatus;  /*stMiscStatus only need read once, it will not change in running */
    DBG_MFG_T stMfg;
} CAPABILITY_T;

typedef struct {
} ST_EDFA_DEBUG;

typedef struct {
    ST_MODULE_INFO astInfo[2];
} ST_EDFA_MFGINFO;

typedef struct {
    int32_t aalFiberTemp[EN_EDFA_MAX][EN_DEV_PARA_MAX];         /* ILA - 4, OLT - 4 */
    int32_t aaalLaserTemp[EN_EDFA_MAX][EN_EDFA_SUB_PUMP_MAX][EN_DEV_PARA_MAX];         /* ILA - 4, OLT - 4 */
    int32_t aaalLaserCurrent[EN_EDFA_MAX][EN_EDFA_SUB_PUMP_MAX][EN_DEV_PARA_MAX];      /* ILA - 4, OLT - 4 */
    int32_t aaalBoosterLaserPower[EN_EDFA_MAX][EN_EDFA_SUB_PUMP_MAX][EN_DEV_PARA_MAX]; /* ILA - 4, OLT - 4 */
    int32_t aalNoiseFigure[EN_EDFA_MAX][EDFA_NF_POINT_MAX_NUM];
    int32_t aalIL[EN_EDFA_MAX][EDFA_IL_PATH_MAX_NUM];
} ST_EDFA_CAPA;

typedef struct
{
    ST_EDFA_MFGINFO stMfginfo;
    ST_EDFA_UPG_INFO stUpgInfo;
    ST_EDFA_CONFIG stConfig;
    ST_EDFA_OPER stOper;
    ST_EDFA_TONE_STATE stEdfaToneState;
    ST_EDFA_ALARM stAlm;
    ST_EDFA_EVENT_FIFO stEventFifo;
    ST_EDFA_EVENT_STATUS stEventStatus;
    ST_EDFA_CAPA stCapa;
    ST_EDFA_SWITCH_PORT_POWER stPortPwr;
    ST_EDFA_OCM_SWITCH_PD_PWR stOcmSwPdPwr;
} ST_EDFA_POLLING;

/*ILA DGE Debug data*/
typedef struct {
    uint32_t uiFreq;   /* FREQ-180000 GHz */
    uint32_t uiAtten;  /* 0.01dB */
} ST_EDFA_DGE_ONE_POINT;
#if 0
typedef struct {
    ST_EDFA_DGE_ONE_POINT aastPoints[DGE_PORT_ILA_NUMBER][DGE_EDFA_CHNL_ILA_NUM]; /* board read back, then set to DGE */
    uint32_t aulCusAvgAttn[DGE_PORT_ILA_NUMBER];
    uint32_t aulCusMaxAttn[DGE_PORT_ILA_NUMBER];
}ST_EDFA_DGE_POINTS;

typedef struct {
    ST_DGE_CONFIG stCusConfig;
    ST_DGE_CONFIG stEdfaToDgeOper;
    ST_EDFA_DGE_POINTS stEdfaToDgePointsOper;
} ST_EDFA_DGE_POLLING;
#endif
#define RW_LOGIC_CNT                32
typedef struct
{
    uint32_t uiIdx;                     /* 0: E1 1:E2 */
    uint32_t uiCnt;                     /* how many cnt need read or write */
    uint32_t uiStartAddr;               /* fpga/cpld addr */
    uint32_t auiValue[RW_LOGIC_CNT];    /* fpga/cpld value */
} ST_DEBUG_LOGIC;

typedef struct
{
    uint32_t aulBaudRate[EN_EDFA_MAX];    /* */
} ST_DEBUG_BAUDRATE;

#define VOA_STABLE_THR_MIN              (-10)
#define VOA_STABLE_THR_MAX              (10)

#define EDFA_VOA_ADJUST_TIMEOUT         (1000)    /* timeout = 1s */

typedef enum
{
    COMPENSATE_VOA_1,
    COMPENSATE_VOA_2,
    COMPENSATE_VOA_NUM
} COMPENSATE_VOA_E;

typedef enum
{
    EDFA_VOA_IDLE,
    EDFA_VOA_ADJUSTING,
    EDFA_VOA_ADJUST_DONE,
    EDFA_VOA_STATUS_NUM
} EDFA_VOA_STATUS_E;

typedef struct
{
    uint32_t uiStatus;
    uint32_t uiAttStart;
    uint32_t uiAttTarget;
    int32_t iAttDelta;
    uint64_t ullStartTime;
} ST_EDFA_VOA_CTRL;

/**
 * @brief definition of EDFA type.
 */
typedef enum {
    EN_EDFA_TYPE_OLT_C,
    EN_EDFA_TYPE_OLT_L,
    EN_EDFA_TYPE_ILA_C,
    EN_EDFA_TYPE_ILA_L,
    EN_EDFA_TYPE_MAX
} EN_EDFA_TYPE;

#define ALM_MAX_SIZE  (496)
#define MODULE_COUNT         4
typedef struct {
    uint8_t ucCaseTmp;
    uint8_t FiberTmp;     /* pstAlmFiberTmpObj */
    uint8_t HeaterTmp;    /* pstAlmFiberTmpDfmeaObj */
    uint8_t OOG[MODULE_COUNT];
    uint8_t LOP[MODULE_COUNT];
    uint8_t EOP[MODULE_COUNT];
    uint8_t ALS[MODULE_COUNT];
    uint8_t OTG[MODULE_COUNT];
    uint8_t Fail[MODULE_COUNT];
    uint8_t Disable[MODULE_COUNT];
    uint8_t Rfl[MODULE_COUNT];
    uint8_t Apr[MODULE_COUNT];
    uint8_t Apsd[MODULE_COUNT];
    uint8_t Rsv[29];   /* 13 */
} ST_EDFA_MODULE_ALM;

typedef struct {
    uint8_t aucPumpTemp[MODULE_COUNT][8];
} ST_EDFA_PUMPTEMP_ALM;

typedef struct {
    uint8_t aucPumpIld[MODULE_COUNT][8];
} ST_EDFA_PUMPILD_ALM;

typedef struct {
    uint8_t aucPumpBfm[MODULE_COUNT][8];
} ST_EDFA_PUMPBFM_ALM;

typedef struct {
    uint8_t aucVoaAlarm[MODULE_COUNT][8];
} ST_EDFA_VOA_ALM;

typedef struct {
    uint8_t aucPdLosAlarm[64];
} ST_EDFA_PDLOS_ALM;

typedef struct {
    uint8_t aucVoltageAlarm[32];
} ST_EDFA_VOLTAGE_ALM;

typedef struct {
    uint8_t aucVoaDac[MODULE_COUNT][8];
} ST_EDFA_VOADAC_ALM;

typedef struct {
    uint8_t aucPumpDacDiff[MODULE_COUNT][8];
} ST_EDFA_PUMPDAC_ALM;

typedef struct {
    uint8_t aucPumpSld[MODULE_COUNT][8];
} ST_EDFA_PUMPSLD_ALM;

typedef struct {
    uint8_t aucSwitch[MODULE_COUNT][2];
} ST_EDFA_SWITCH_ALM;

typedef struct {
    uint8_t aucTableCheck[32];
} ST_EDFA_TABLECHECK_ALM;

typedef struct {
    uint8_t aucOsc[8];
} ST_EDFA_OSC_ALM;

typedef struct {
    uint8_t aucReserver[56];
} ST_EDFA_RESERVE_ALM;

typedef struct {
    uint8_t ucApsd;
} ST_EDFA_APSD_ALM;

typedef struct {
    uint32_t uiPumpIdx;
    uint16_t sCurrent;
}EN_PUMP_INFO;

typedef struct {
    ST_EDFA_MODULE_ALM stModuleAlm;
    ST_EDFA_PUMPTEMP_ALM stPumpTempAlm;
    ST_EDFA_PUMPILD_ALM stPumpIldAlm;
    ST_EDFA_PUMPBFM_ALM stPumpBmAlm;
    ST_EDFA_VOA_ALM stVoaAlm;
    ST_EDFA_PDLOS_ALM stPdLosAlm;
    ST_EDFA_VOLTAGE_ALM stVoltageAlm;
    ST_EDFA_VOADAC_ALM stVoaDacAlm;
    ST_EDFA_PUMPDAC_ALM stPumpDacAlm;
    ST_EDFA_PUMPSLD_ALM stPumpSldAlm;
    ST_EDFA_SWITCH_ALM stSwitchAlm;
    ST_EDFA_TABLECHECK_ALM stTableCheckAlm;
    ST_EDFA_OSC_ALM stOscAlm;
    /* ST_EDFA_APSD_ALM stApsdAlm; */
    ST_EDFA_RESERVE_ALM stReserveAlm;
} ST_EDFA_ALARM1;

typedef enum  {
    OSC_IDX_Failure = 0,
    OSC_IDX_InnerErr,
    OSC_IDX_OffLine,
    OSC_IDX_MisMatch,
    OSC_IDX_RxLos = 3,
    OSC_IDX_TxLos,
    OSC_IDX_AddLos,
    OSC_IDX_DropLos,
    OSC_IDX_PhyLinkDown,

    OSC_ALM_COUNT,
}OSC_ALM_EN;

typedef struct
{
    CMN_THR_ST stCmnThr;
    CMN_THR_ST stCmnHys;
}CMN_THR_HYS_ST;

typedef struct
{
    uint32_t uiPortIdx;
    uint32_t uiType;
    int32_t iThr;
    int32_t iHys;
}CMN_GENERIC_THR_HYS_ST;

#ifdef __cplusplus
}
#endif

#endif
