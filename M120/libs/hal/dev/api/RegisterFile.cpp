/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      RegisterFile.cpp
   Author:        Zhu Weian
   Date:          2020-09-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-28] [1.0] [Creator]


*****************************************************************************************************/

#include "RegisterFile.h"
#include "util_inc.h"

std::map<std::string, uint32_t> CRegisterFile::ms_staHwTypeMap = {
#if 1
    {"HW_TYPE_UART", HW_TYPE_UART},
    {"HW_TYPE_UART_USB", HW_TYPE_UART_USB},
    {"HW_TYPE_UART_CPLD", HW_TYPE_UART_CPLD},
    {"HW_TYPE_UART_FPGA", HW_TYPE_UART_FPGA},
    {"HW_TYPE_UART_LOGIC", HW_TYPE_UART_LOGIC},
    {"HW_TYPE_UART_LOGIC_USB", HW_TYPE_UART_LOGIC_USB},
    {"HW_TYPE_MULTI_UART", HW_TYPE_MULTI_UART},
    {"HW_TYPE_FT", HW_TYPE_FT},
    {"HW_TYPE_I2C", HW_TYPE_I2C},
    {"HW_TYPE_LOGIC_I2C", HW_TYPE_LOGIC_I2C},
    {"HW_TYPE_LOGIC_SWITCH_I2C", HW_TYPE_LOGIC_SWITCH_I2C},
    {"HW_TYPE_FT_I2C", HW_TYPE_FT_I2C},
    {"HW_TYPE_PCA_I2C", HW_TYPE_PCA_I2C},
    {"HW_TYPE_SUSI4_I2C", HW_TYPE_SUSI4_I2C},
    {"HW_TYPE_SPI", HW_TYPE_SPI},
    {"HW_TYPE_VSPI", HW_TYPE_VSPI},
    {"HW_TYPE_LOGIC_SPI", HW_TYPE_LOGIC_SPI},
    {"HW_TYPE_LOGIC_SLAVE_SPI", HW_TYPE_LOGIC_SLAVE_SPI},
    {"HW_TYPE_LOGIC_MASTER_SPI", HW_TYPE_LOGIC_MASTER_SPI},
    {"HW_TYPE_SPI_USB_FT", HW_TYPE_SPI_USB_FT},
    {"HW_TYPE_UART_LOGIC", HW_TYPE_UART_LOGIC},
    {"HW_TYPE_LOCAL_BUS", HW_TYPE_LOCAL_BUS},
    {"HW_TYPE_LOCAL_BUS_BITMAP", HW_TYPE_LOCAL_BUS_BITMAP},
    {"HW_TYPE_GPIO", HW_TYPE_GPIO},
    {"HW_TYPE_MDIO", HW_TYPE_MDIO},
    {"HW_TYPE_MDIO_FPGA", HW_TYPE_MDIO_FPGA},
    {"HW_TYPE_JTAG", HW_TYPE_JTAG},
    {"HW_TYPE_FPGA", HW_TYPE_FPGA},
    {"HW_TYPE_CPLD", HW_TYPE_CPLD},
    {"HW_TYPE_LOGIC_DRIVER", HW_TYPE_LOGIC_DRIVER},
    {"HW_TYPE_LOGIC", HW_TYPE_LOGIC},
    {"HW_TYPE_OSC_BOARD", HW_TYPE_OSC_BOARD},
    {"HW_TYPE_LED_BOARD", HW_TYPE_LED_BOARD},
    {"HW_TYPE_FAN_BOARD", HW_TYPE_FAN_BOARD},
    {"HW_TYPE_PSU_BOARD", HW_TYPE_PSU_BOARD},
    {"HW_TYPE_OSC_BOARD", HW_TYPE_OSC_BOARD},
    {"HW_TYPE_SLOT_BOARD", HW_TYPE_SLOT_BOARD},
    {"HW_TYPE_RESET_MODULE", HW_TYPE_RESET_MODULE},
    {"HW_TYPE_I2C_SWITCH", HW_TYPE_I2C_SWITCH},
    {"HW_TYPE_USB", HW_TYPE_USB},
#endif
    {"HW_TYPE_VIRTUAL", HW_TYPE_VIRTUAL},
    {"HW_TYPE_NONE", HW_TYPE_NONE},
    {"HW_TYPE_FIXED_OPS", HW_TYPE_FIXED_OPS},
    {"HW_TYPE_FIXED_AMP", HW_TYPE_FIXED_AMP},
};

std::map<std::string, uint32_t> CRegisterFile::ms_staCardTypeMap = {
    {"CARD_TYPE_OCM", CARD_TYPE_OCM},
    {"CARD_TYPE_PA", CARD_TYPE_PA},
    {"CARD_TYPE_BA", CARD_TYPE_BA},
    {"CARD_TYPE_OPS", CARD_TYPE_OPS},
    {"CARD_TYPE_OTA", CARD_TYPE_OTA},
    {"CARD_TYPE_400GZR", CARD_TYPE_400GZR},
    {"CARD_TYPE_OPTICAL", CARD_TYPE_OPTICAL},
    {"CARD_TYPE_400GZR_BETA", CARD_TYPE_400GZR_BETA},
    {"CARD_TYPE_FAN", CARD_TYPE_FAN},
    {"CARD_TYPE_POWER", CARD_TYPE_POWER},
    {"CARD_TYPE_SCC", CARD_TYPE_SCC},
    {"CARD_TYPE_CLASSIS", CARD_TYPE_CLASSIS},
    {"CARD_TYPE_FIX", CARD_TYPE_FIX},
    {"CARD_TYPE_MUX", CARD_TYPE_MUX},
    {"CARD_TYPE_OCS", CARD_TYPE_OCS},
    {"CARD_TYPE_UNKOWN", CARD_TYPE_UNKOWN}
};

std::map<std::string, uint32_t> CRegisterFile::ms_staModuleTypeMap = {
    {"MODULE_TYPE_NONE", MODULE_TYPE_NONE},
    {"MODULE_TYPE_CHASSIS", MODULE_TYPE_CHASSIS},
    {"MODULE_TYPE_SCC", MODULE_TYPE_SCC},
    {"MODULE_TYPE_SLOT_INFO", MODULE_TYPE_SLOT_INFO},

    {"MODULE_TYPE_OA", MODULE_TYPE_OA},
    {"MODULE_TYPE_VOA", MODULE_TYPE_VOA},
    {"MODULE_TYPE_WSS", MODULE_TYPE_WSS},
    {"MODULE_TYPE_OCM", MODULE_TYPE_OCM},
    {"MODULE_TYPE_TDCM", MODULE_TYPE_TDCM},
    {"MODULE_TYPE_PAM4", MODULE_TYPE_PAM4},
    {"MODULE_TYPE_OPS", MODULE_TYPE_OPS},
    {"MODULE_TYPE_DCE", MODULE_TYPE_DCE},
    {"MODULE_TYPE_MCS", MODULE_TYPE_MCS},
    {"MODULE_TYPE_OSC", MODULE_TYPE_OSC},
    {"MODULE_TYPE_DGE", MODULE_TYPE_DGE},
    {"MODULE_TYPE_OTDR", MODULE_TYPE_OTDR},
    {"MODULE_TYPE_EXTEND", MODULE_TYPE_EXTEND},
    {"MODULE_TYPE_MUX", MODULE_TYPE_MUX},
    {"MODULE_TYPE_ADC", MODULE_TYPE_ADC},
    {"MODULE_TYPE_OCS", MODULE_TYPE_OCS},

    {"MODULE_TYPE_FPGA", MODULE_TYPE_FPGA},
    {"MODULE_TYPE_CPLD", MODULE_TYPE_CPLD},
    {"MODULE_TYPE_FAN", MODULE_TYPE_FAN},
    {"MODULE_TYPE_LED", MODULE_TYPE_LED},
    {"MODULE_TYPE_TEMP", MODULE_TYPE_TEMP},
    {"MODULE_TYPE_POWER", MODULE_TYPE_POWER},
    {"MODULE_TYPE_E2PROM", MODULE_TYPE_E2PROM},
    {"MODULE_TYPE_FLASH", MODULE_TYPE_FLASH},
    {"MODULE_TYPE_RESET", MODULE_TYPE_RESET},
    {"MODULE_TYPE_UPGRADE", MODULE_TYPE_UPGRADE},
};

std::map<std::string, uint32_t> CRegisterFile::ms_staModuleFuncMap = {
    {"MODULE_FUNC_TYPE_NONE", MODULE_FUNC_TYPE_NONE},
#if 0
    {"MODULE_FUNC_TYPE_OA", MODULE_FUNC_TYPE_OA},
    {"MODULE_FUNC_TYPE_OA_PA", MODULE_FUNC_TYPE_OA_PA},
    {"MODULE_FUNC_TYPE_OA_BA", MODULE_FUNC_TYPE_OA_BA},
    {"MODULE_FUNC_TYPE_OTDR", MODULE_FUNC_TYPE_OTDR},
    {"MODULE_FUNC_TYPE_OCM", MODULE_FUNC_TYPE_OCM},
    {"MODULE_FUNC_TYPE_OPS", MODULE_FUNC_TYPE_OPS},
    {"MODULE_FUNC_TYPE_EXTEND", MODULE_FUNC_TYPE_EXTEND},
    {"MODULE_FUNC_TYPE_MUX", MODULE_FUNC_TYPE_MUX},
    {"MODULE_FUNC_TYPE_TDCM", MODULE_FUNC_TYPE_TDCM},
    {"MODULE_FUNC_TYPE_OSC", MODULE_FUNC_TYPE_OSC},
    {"MODULE_FUNC_TYPE_WSS0", MODULE_FUNC_TYPE_WSS0},
    {"MODULE_FUNC_TYPE_WSS1", MODULE_FUNC_TYPE_WSS1}
#endif
};

/*
   DEV_TYPE_NONE = 0,

        DEV_TYPE_OA,
        DEV_TYPE_BA,
        DEV_TYPE_PA,
        DEV_TYPE_BA_PA,
        DEV_TYPE_ASE,
        DEV_TYPE_ILA,

        DEV_TYPE_WSS,
        DEV_TYPE_OCM,
        DEV_TYPE_OPS,
        DEV_TYPE_OSC,
        DEV_TYPE_VOA,

        DEV_TYPE_P_OA = (DEV_FUNC_TYPE_P_OA << 16) + DEV_TYPE_OA,
        DEV_TYPE_P_BA = (DEV_FUNC_TYPE_P_OA << 16) + DEV_TYPE_BA,
        DEV_TYPE_P_PA = (DEV_FUNC_TYPE_P_OA << 16) + DEV_TYPE_PA,
        DEV_TYPE_P_BA_PA = (DEV_FUNC_TYPE_P_OA << 16) + DEV_TYPE_BA_PA,
        DEV_TYPE_W_OA = (DEV_FUNC_TYPE_W_OA << 16) + DEV_TYPE_OA,
        DEV_TYPE_E_OA = (DEV_FUNC_TYPE_E_OA << 16) + DEV_TYPE_OA,

        DEV_TYPE_W_OSC = (DEV_FUNC_TYPE_W_OSC << 16) + DEV_TYPE_OSC,
        DEV_TYPE_E_OSC = (DEV_FUNC_TYPE_E_OSC << 16) + DEV_TYPE_OSC,
        DEV_TYPE_P_OSC = (DEV_FUNC_TYPE_P_OSC << 16) + DEV_TYPE_OSC,
        DEV_TYPE_SFP = (DEV_FUNC_TYPE_SFP << 16) + DEV_TYPE_OSC,

        DEV_TYPE_PADDING_VOA = (DEV_FUNC_TYPE_PADDING_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_PA_VOA =  (DEV_FUNC_TYPE_PA_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_OSC_VOA = (DEV_FUNC_TYPE_OSC_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_OCM_VOA = (DEV_FUNC_TYPE_OCM_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_P_PADDING_VOA = (DEV_FUNC_TYPE_P_PADDING_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_P_PA_VOA = (DEV_FUNC_TYPE_P_PA_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_P_OSC_VOA = (DEV_FUNC_TYPE_P_OSC_VOA << 16) + DEV_TYPE_VOA,
        DEV_TYPE_P_OCM_VOA = (DEV_FUNC_TYPE_P_OCM_VOA << 16) + DEV_TYPE_VOA,

        DEV_TYPE_P_OCM = (DEV_FUNC_TYPE_P_OCM << 16) + DEV_TYPE_OCM,
        DEV_TYPE_WSS_OCM = (DEV_FUNC_TYPE_WSS_OCM << 16) + DEV_TYPE_OCM,

        DEV_TYPE_TDCM,
        DEV_TYPE_PAM4,
        DEV_TYPE_DCE,
        DEV_TYPE_OTDR,
        DEV_TYPE_MUX,
        DEV_TYPE_EXTEND,

        DEV_TYPE_RESET,
        DEV_TYPE_E2PROM,
        DEV_TYPE_TEMP,
        DEV_TYPE_LED,
        DEV_TYPE_PHY,

        DEV_TYPE_SCC,
        DEV_TYPE_FPGA,
        DEV_TYPE_CPLD,

 */

std::map<std::string, uint32_t> CRegisterFile::ms_staDevTypeMap = {
    {"DEV_TYPE_NONE", DEV_TYPE_NONE},
    {"DEV_TYPE_OA", DEV_TYPE_OA},
    {"DEV_TYPE_BA", DEV_TYPE_BA},
    {"DEV_TYPE_PA", DEV_TYPE_PA},
    {"DEV_TYPE_BA_PA", DEV_TYPE_BA_PA},
    {"DEV_TYPE_ASE", DEV_TYPE_ASE},
    {"DEV_TYPE_W_OA", DEV_TYPE_W_OA},
    {"DEV_TYPE_E_OA", DEV_TYPE_E_OA},
    {"DEV_TYPE_P_BA", DEV_TYPE_P_BA},
    {"DEV_TYPE_P_PA", DEV_TYPE_P_PA},
    {"DEV_TYPE_P_BA_PA", DEV_TYPE_P_BA_PA},


    {"DEV_TYPE_WSS", DEV_TYPE_WSS},
    {"DEV_TYPE_WSS1", DEV_TYPE_WSS1},
    {"DEV_TYPE_WSS2", DEV_TYPE_WSS2},


    {"DEV_TYPE_OCM", DEV_TYPE_OCM},
    {"DEV_TYPE_P_OCM", DEV_TYPE_P_OCM},
    {"DEV_TYPE_WSS_OCM", DEV_TYPE_WSS_OCM},

    {"DEV_TYPE_VOA", DEV_TYPE_VOA},
    {"DEV_TYPE_PADDING_VOA", DEV_TYPE_PADDING_VOA},
    {"DEV_TYPE_OSC_VOA", DEV_TYPE_OSC_VOA},
    {"DEV_TYPE_P_PADDING_VOA", DEV_TYPE_P_PADDING_VOA},
    {"DEV_TYPE_P_PA_VOA", DEV_TYPE_P_PA_VOA},
    {"DEV_TYPE_P_OSC_VOA", DEV_TYPE_P_OSC_VOA},

    {"DEV_TYPE_TDCM", DEV_TYPE_TDCM},
    {"DEV_TYPE_PAM4", DEV_TYPE_PAM4},
    {"DEV_TYPE_OPS", DEV_TYPE_OPS},
    {"DEV_TYPE_OPS4A", DEV_TYPE_OPS4A},
    {"DEV_TYPE_OPS4B", DEV_TYPE_OPS4B},
    {"DEV_TYPE_OSC", DEV_TYPE_OSC},
    {"DEV_TYPE_OTDR", DEV_TYPE_OTDR},
    {"DEV_TYPE_P_OTDR", DEV_TYPE_P_OTDR},
    {"DEV_TYPE_EXTEND", DEV_TYPE_EXTEND},
    {"DEV_TYPE_MUX", DEV_TYPE_MUX},
    {"DEV_TYPE_OCS", DEV_TYPE_OCS},

    {"DEV_TYPE_FPGA", DEV_TYPE_FPGA},
    {"DEV_TYPE_CPLD", DEV_TYPE_CPLD},
    {"DEV_TYPE_LED", DEV_TYPE_LED},

    {"DEV_TYPE_COLD_RESET", DEV_TYPE_COLD_RESET},

    {"DEV_TYPE_OA_COLD_RESET", DEV_TYPE_OA_COLD_RESET},
    {"DEV_TYPE_BA_COLD_RESET", DEV_TYPE_BA_COLD_RESET},
    {"DEV_TYPE_PA_COLD_RESET", DEV_TYPE_PA_COLD_RESET},
    {"DEV_TYPE_BA_PA_COLD_RESET", DEV_TYPE_BA_PA_COLD_RESET},
    {"DEV_TYPE_ASE_COLD_RESET", DEV_TYPE_ASE_COLD_RESET},

    {"DEV_TYPE_WSS_COLD_RESET", DEV_TYPE_WSS_COLD_RESET},
    {"DEV_TYPE_WSS1_COLD_RESET", DEV_TYPE_WSS1_COLD_RESET},
    {"DEV_TYPE_WSS2_COLD_RESET", DEV_TYPE_WSS2_COLD_RESET},
    {"DEV_TYPE_WSS1_WARM_RESET", DEV_TYPE_WSS1_WARM_RESET},
    {"DEV_TYPE_WSS2_WARM_RESET", DEV_TYPE_WSS2_WARM_RESET},

    {"DEV_TYPE_OPS_COLD_RESET", DEV_TYPE_OPS_COLD_RESET},

    {"DEV_TYPE_OCM_COLD_RESET", DEV_TYPE_OCM_COLD_RESET},
    {"DEV_TYPE_WSS_OCM_COLD_RESET", DEV_TYPE_WSS_OCM_COLD_RESET},
    {"DEV_TYPE_OTDR_COLD_RESET", DEV_TYPE_OTDR_COLD_RESET},

    {"DEV_TYPE_TEMP", DEV_TYPE_TEMP},
    {"DEV_TYPE_E2PROM", DEV_TYPE_E2PROM},
    {"DEV_TYPE_COLD_RESET", DEV_TYPE_COLD_RESET},
    {"DEV_TYPE_PHY", DEV_TYPE_PHY},


    {"DEV_TYPE_P_BA_COLD_RESET", DEV_TYPE_P_BA_COLD_RESET},
    {"DEV_TYPE_P_PA_COLD_RESET", DEV_TYPE_P_PA_COLD_RESET},
    {"DEV_TYPE_P_BA_PA_COLD_RESET", DEV_TYPE_P_BA_PA_COLD_RESET},
    {"DEV_TYPE_P_OCM_COLD_RESET", DEV_TYPE_P_OCM_COLD_RESET},
    {"DEV_TYPE_P_OSC_COLD_RESET", DEV_TYPE_P_OSC_COLD_RESET},
    {"DEV_TYPE_P_OTDR_COLD_RESET", DEV_TYPE_P_OTDR_COLD_RESET},

    {"DEV_TYPE_OCS_COLD_RESET", DEV_TYPE_OCS_COLD_RESET},


#if 0
    /* OA */

    DEV_TYPE_W_OA,     /* west oa in ILA or ILE*/
    DEV_TYPE_E_OA,     /* east oa in ILA or ILE */

    DEV_TYPE_P_BA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_BA,     /* use in P-OTA */
    DEV_TYPE_P_PA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_PA,     /* use in P-OTA */
    DEV_TYPE_P_BA_PA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_BA_PA,     /* internal type for hal */

    /* WSS */
    DEV_TYPE_WSS,
    DEV_TYPE_WSS1,
    DEV_TYPE_WSS2,

    /* OCM */
    DEV_TYPE_OCM,

    DEV_TYPE_P_OCM = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_OCM,
    DEV_TYPE_WSS_OCM = (FUNC_TYPE_WSS << FUNC_FLAG_OFFSET) + DEV_TYPE_OCM,

    /* OPS */
    DEV_TYPE_OPS,

    /* OSC */
    DEV_TYPE_OSC,

    DEV_TYPE_W_OSC,
    DEV_TYPE_E_OSC,

    DEV_TYPE_P_OSC = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_OSC,
    DEV_TYPE_WSS_OSC = (FUNC_TYPE_WSS << FUNC_FLAG_OFFSET) + DEV_TYPE_OSC,

    /* VOA */
    DEV_TYPE_VOA,

    DEV_TYPE_PADDING_VOA,
    DEV_TYPE_PA_VOA,
    DEV_TYPE_OSC_VOA,

    DEV_TYPE_P_PADDING_VOA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_PADDING_VOA,
    DEV_TYPE_P_PA_VOA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_PA_VOA,
    DEV_TYPE_P_OSC_VOA = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_OSC_VOA,

    /* TDCM */
    DEV_TYPE_TDCM,

    /* PAM4 */
    DEV_TYPE_PAM4,

    /* DCE */
    DEV_TYPE_DCE,

    /* OTDR */
    DEV_TYPE_OTDR,

    /* MUX */
    DEV_TYPE_MUX,

    /* EXTEND */
    DEV_TYPE_EXTEND,

    /* LED */
    DEV_TYPE_LED,
    DEV_TYPE_SYS_LED,
    DEV_TYPE_ALM_LED,

    DEV_TYPE_P_OTA_SYS_LED = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_SYS_LED,
    DEV_TYPE_P_OTA_ALM_LED = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_ALM_LED,
    DEV_TYPE_WSS_SYS_LED = (FUNC_TYPE_WSS << FUNC_FLAG_OFFSET) + DEV_TYPE_SYS_LED,
    DEV_TYPE_WSS_ALM_LED = (FUNC_TYPE_WSS << FUNC_FLAG_OFFSET) + DEV_TYPE_ALM_LED,
    DEV_TYPE_OPS_SYS_LED = (FUNC_TYPE_OPS << FUNC_FLAG_OFFSET) + DEV_TYPE_SYS_LED,
    DEV_TYPE_OPS_ALM_LED = (FUNC_TYPE_OPS << FUNC_FLAG_OFFSET) + DEV_TYPE_ALM_LED,

    /* COLD_RESET */
    DEV_TYPE_COLD_RESET,

    DEV_TYPE_OA_COLD_RESET,
    DEV_TYPE_BA_COLD_RESET,
    DEV_TYPE_PA_COLD_RESET,
    DEV_TYPE_BA_PA_COLD_RESET,
    DEV_TYPE_ASE_COLD_RESET,

    DEV_TYPE_WSS_COLD_RESET,
    DEV_TYPE_WSS1_COLD_RESET,
    DEV_TYPE_WSS2_COLD_RESET,

    DEV_TYPE_OPS_COLD_RESET,

    DEV_TYPE_OCM_COLD_RESET,
    DEV_TYPE_WSS_OCM_COLD_RESET,

    DEV_TYPE_OSC_COLD_RESET,
    DEV_TYPE_W_OSC_COLD_RESET,
    DEV_TYPE_E_OSC_COLD_RESET,


    DEV_TYPE_P_BA_COLD_RESET = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_BA_COLD_RESET,
    DEV_TYPE_P_PA_COLD_RESET = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_PA_COLD_RESET,
    DEV_TYPE_P_BA_PA_COLD_RESET = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_BA_PA_COLD_RESET,
    DEV_TYPE_P_OCM_COLD_RESET = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_OCM_COLD_RESET,
    DEV_TYPE_P_OSC_COLD_RESET = (FUNC_TYPE_P_OTA << FUNC_FLAG_OFFSET) + DEV_TYPE_OSC_COLD_RESET,

    DEV_TYPE_E2PROM,
    DEV_TYPE_TEMP,
    DEV_TYPE_PHY,

    DEV_TYPE_SCC,
    DEV_TYPE_FPGA,
    DEV_TYPE_CPLD,

    DEV_TYPE_NUM
#endif
};

std::map<std::string, uint32_t> CRegisterFile::ms_staDevFuncMap = {
#if 0
    {"DEV_FUNC_TYPE_NONE", DEV_FUNC_TYPE_NONE},
    {"DEV_FUNC_TYPE_OA", DEV_FUNC_TYPE_OA},
    {"DEV_FUNC_TYPE_OA_PA", DEV_FUNC_TYPE_OA_PA},
    {"DEV_FUNC_TYPE_OA_BA", DEV_FUNC_TYPE_OA_BA},
    {"DEV_FUNC_TYPE_OCM", DEV_FUNC_TYPE_OCM},
    {"DEV_FUNC_TYPE_OPS", DEV_FUNC_TYPE_OPS},
    {"DEV_FUNC_TYPE_OSC", DEV_FUNC_TYPE_OSC},
    {"DEV_FUNC_TYPE_TDCM", DEV_FUNC_TYPE_TDCM},
    {"DEV_FUNC_TYPE_TEMP", DEV_FUNC_TYPE_TEMP},
    {"DEV_FUNC_TYPE_MEM", DEV_FUNC_TYPE_MEM},
    {"DEV_FUNC_TYPE_OTDR", DEV_FUNC_TYPE_OTDR},
    {"DEV_FUNC_TYPE_EXTEND", DEV_FUNC_TYPE_EXTEND},
    {"DEV_FUNC_TYPE_MUX", DEV_FUNC_TYPE_MUX},
    {"DEV_FUNC_TYPE_ADC_POSITIVE", DEV_FUNC_TYPE_ADC_POSITIVE},
    {"DEV_FUNC_TYPE_ADC_OPPOSITE", DEV_FUNC_TYPE_ADC_OPPOSITE},
    {"DEV_FUNC_TYPE_RESET_WARM", DEV_FUNC_TYPE_RESET_WARM},
    {"DEV_FUNC_TYPE_RESET_COLD", DEV_FUNC_TYPE_RESET_COLD},
    {"DEV_FUNC_TYPE_UPGRADE", DEV_FUNC_TYPE_UPGRADE},
    {"DEV_FUNC_TYPE_STORE", DEV_FUNC_TYPE_STORE},
    {"DEV_FUNC_TYPE_LED_SYS", DEV_FUNC_TYPE_LED_SYS},
    {"DEV_FUNC_TYPE_LED_ALM", DEV_FUNC_TYPE_LED_ALM},
    {"DEV_FUNC_TYPE_LEN_PWR", DEV_FUNC_TYPE_LEN_PWR},
    {"DEV_FUNC_TYPE_LED_FAN", DEV_FUNC_TYPE_LED_FAN},
    {"DEV_FUNC_TYPE_WSS", DEV_FUNC_TYPE_WSS},
    {"DEV_FUNC_TYPE_PHY", DEV_FUNC_TYPE_PHY}
#endif
};

std::map<std::string, uint32_t> CRegisterFile::ms_staProtoMap = {
    {"PROTO_TYPE_DEF", PROTO_TYPE_DEF},
    {"PROTO_TYPE_OA_CLI_SIG", PROTO_TYPE_OA_CLI_SIG},
    {"PROTO_TYPE_OA_CLI_MUL", PROTO_TYPE_OA_CLI_MUL},
    {"PROTO_TYPE_OA_CLI_K20", PROTO_TYPE_OA_CLI_K20},
    {"PROTO_TYPE_OA_OOP_SIG", PROTO_TYPE_OA_OOP_SIG},
    {"PROTO_TYPE_OA_OOP_MUL", PROTO_TYPE_OA_OOP_MUL},

    {"PROTO_TYPE_WSS", PROTO_TYPE_WSS},
    {"PROTO_TYPE_OCM", PROTO_TYPE_OCM},
    {"PROTO_TYPE_TDCM", PROTO_TYPE_TDCM},
    {"PROTO_TYPE_PAM4", PROTO_TYPE_PAM4},
    {"PROTO_TYPE_OPS", PROTO_TYPE_OPS},
    {"PROTO_TYPE_DGE", PROTO_TYPE_DGE},
    {"PROTO_TYPE_OSC", PROTO_TYPE_OSC},

    {"PROTO_TYPE_FPGA", PROTO_TYPE_FPGA},
    {"PROTO_TYPE_CPLD", PROTO_TYPE_CPLD},
    {"PROTO_TYPE_FAN_CPLD", PROTO_TYPE_FAN_CPLD},

    {"PROTO_TYPE_E2PROM", PROTO_TYPE_E2PROM},
    {"PROTO_TYPE_TINY_E2PROM", PROTO_TYPE_TINY_E2PROM},
    {"PROTO_TYPE_LED", PROTO_TYPE_LED},
};

CBoardRegisterInfo CRegisterFile::ms_stBoardRegInfo;
std::vector<CSlotRegInfo> CRegisterFile::ms_staSlotRegInfo;
std::map<uint32_t, CCardInfo> CRegisterFile::ms_staCardRegInfo;

bool
CRegisterFile::__ParseRegInfo(cJSON *pstJson, std::vector<CRegisterInfo> &rstaRegInfo)
{
    if (nullptr == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "pstJson is nullptr.");
        return false;
    }

    int iSize = cJSON_GetArraySize(pstJson);

    for (int i = 0; i < iSize; ++i) {
        CRegisterInfo stRegInfo = {0};

        cJSON *pstRegInfo = cJSON_GetArrayItem(pstJson, i);
        if (nullptr == pstRegInfo) {
            LOG_WRITE(EN_LOG_ERROR, "get reg info error. i = %d", i);
            return false;
        }



        cJSON *pstHwType = cJSON_GetObjectItem(pstRegInfo, "HwType");
        cJSON *pstHwIdx = cJSON_GetObjectItem(pstRegInfo, "HwIdx");

        cJSON *pstHwSubIdx = cJSON_GetObjectItem(pstRegInfo, "HwSubIdx");
        cJSON *pstDevAddr = cJSON_GetObjectItem(pstRegInfo, "DevAddr");

        cJSON *pstProto = cJSON_GetObjectItem(pstRegInfo, "Proto");


        std::string staHwType(pstHwType->valuestring);
        std::string staProto(pstProto->valuestring);


        stRegInfo.uiHwType = ms_staHwTypeMap[staHwType];
        stRegInfo.uiHwIdx = pstHwIdx->valueint;
        stRegInfo.uiHwSubIdx = pstHwSubIdx->valueint;
        stRegInfo.uiDevAddr = pstDevAddr->valueint;
        stRegInfo.uiProto = ms_staProtoMap[staProto];

        LOG_WRITE(EN_LOG_TRACE, "[HwType = %s:%d, HwIdx = %d, HwSubIdx = %d, DevAddr = %d, Proto = %s:%d]",
                  pstHwType->valuestring,
                  stRegInfo.uiHwType,
                  stRegInfo.uiHwIdx,
                  stRegInfo.uiHwIdx,
                  stRegInfo.uiDevAddr,
                  pstProto->valuestring,
                  stRegInfo.uiProto);

        rstaRegInfo.push_back(stRegInfo);
    }

    return true;
}

bool
CRegisterFile::__ParseSupportedTypeArray(cJSON *pstJson, std::vector<uint32_t> &rstaSupportedType)
{
    LOG_WRITE(EN_LOG_DEBUG, "Supported Type +++");
    int iTypeSize = cJSON_GetArraySize(pstJson);

    for (int i = 0; i < iTypeSize; ++i) {

        cJSON *pstTypeObj = cJSON_GetArrayItem(pstJson, i);
        std::string staCardType(pstTypeObj->valuestring);
        uint32_t uiCardType = ms_staCardTypeMap[staCardType];
        rstaSupportedType.push_back(uiCardType);
        LOG_WRITE(EN_LOG_TRACE, "%s", pstTypeObj->valuestring);
    }
    LOG_WRITE(EN_LOG_DEBUG, "Supported Type ---");

    return true;
}

bool
CRegisterFile::__ParseHwInfo(cJSON *pstJson, CHwInfo &rstHwInfo)
{

    LOG_WRITE(EN_LOG_DEBUG, "Hw Info +++");

    cJSON *pstHwType = cJSON_GetObjectItem(pstJson, "HwType");
    cJSON *pstHwIdx = cJSON_GetObjectItem(pstJson, "HwIdx");
    cJSON *pstHwSubIdx = cJSON_GetObjectItem(pstJson, "HwSubIdx");

    std::string staHwType(pstHwType->valuestring);
    rstHwInfo.uiHwType = ms_staHwTypeMap[staHwType];
    rstHwInfo.uiHwIdx = pstHwIdx->valueint;
    rstHwInfo.uiHwSubIdx = pstHwSubIdx->valueint;

    LOG_WRITE(EN_LOG_TRACE, "[HwType = %s, HwIdx = %d, HwSubIdx = %d]",
              pstHwType->valuestring, rstHwInfo.uiHwIdx, rstHwInfo.uiHwSubIdx);

    LOG_WRITE(EN_LOG_DEBUG, "Hw Info ---");

    return true;
}

#if 0
bool
CRegisterFile::__ParseSupportedModule(cJSON *pstJson,
                                      std::map<uint32_t, std::pair<uint32_t, uint32_t> > &rstaMap)
{
    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedModule +++");

    cJSON *pstCardTypeMap = pstJson->child;

    while (pstCardTypeMap != nullptr) {

        std::string staCardType(pstCardTypeMap->string);
        uint32_t uiCardType = ms_staCardTypeMap[staCardType];        /* atoi(pstCardTypeMap->string); */

        cJSON *pstMap = pstCardTypeMap->child;
        if (pstMap != nullptr) {

            std::string staModuleFunc(pstMap->string);
            std::string staDevFunc(pstMap->valuestring);
            uint32_t uiModuleFunc = ms_staModuleFuncMap[staModuleFunc];             /* atoi(pstMap->string); */
            uint32_t uiDevFunc = ms_staDevFuncMap[staDevFunc];            /* pstMap->valueint; */

            std::pair<uint32_t, uint32_t> staModuleDev(uiModuleFunc, uiDevFunc);
            std::pair<uint32_t, std::pair<uint32_t, uint32_t> > staPair(uiCardType, staModuleDev);

            rstaMap.insert(staPair);

            LOG_WRITE(EN_LOG_TRACE, "[CardType : %s, [ModuleFunc : %s, DevFunc : %s]]",
                      pstCardTypeMap->string, pstMap->string, pstMap->valuestring);
        }

        pstCardTypeMap = pstCardTypeMap->next;
    }

    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedModule ---");

    return true;
}

#else

bool
CRegisterFile::__ParseSupportedModule(cJSON *pstJson,
                                      std::map<uint32_t, uint32_t> &rstaMap)
{
    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedModule +++");

    cJSON *pstCardType = pstJson->child;

    while (pstCardType != nullptr) {

        std::string staCardType(pstCardType->string);
        std::string staDevFunc(pstCardType->valuestring);

        uint32_t uiCardType = ms_staCardTypeMap[staCardType];
        uint32_t uiDevType = ms_staDevTypeMap[staDevFunc];

        std::pair<uint32_t, uint32_t> staPair(uiCardType, uiDevType);

        rstaMap.insert(staPair);

        LOG_WRITE(EN_LOG_TRACE, "[CardType = %s : DevFunc = %s]", pstCardType->string, pstCardType->valuestring);

        pstCardType = pstCardType->next;
    }

    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedModule ---");

    return true;
}


#endif

bool
CRegisterFile::__ParseSupportedDev(cJSON *pstJson, std::map<uint32_t, uint32_t> &rstaMap)
{

    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedDev +++");
    cJSON *pstCardType = pstJson->child;

    while (pstCardType != nullptr) {

        std::string staCardType(pstCardType->string);
        std::string staDevFunc(pstCardType->valuestring);
        uint32_t uiCardType = ms_staCardTypeMap[staCardType];
        uint32_t uiDevType = ms_staDevTypeMap[staDevFunc];

        std::pair<uint32_t, uint32_t> staPair(uiCardType, uiDevType);

        rstaMap.insert(staPair);

        LOG_WRITE(EN_LOG_TRACE, "[CardType = %s : DevFunc = %s]", pstCardType->string, pstCardType->valuestring);

        pstCardType = pstCardType->next;
    }


    LOG_WRITE(EN_LOG_DEBUG, "__ParseSupportedDev ---");

    return true;
}

bool
CRegisterFile::__ParseSlotHwInfo(cJSON *pstJson, CSlotHwInfo &rstSlotHwInfo)
{
    cJSON *pstHwInfo = cJSON_GetObjectItem(pstJson, "HwInfo");
    if (nullptr == pstHwInfo) {
        LOG_WRITE(EN_LOG_ERROR, "pstHwInfoObj is nullptr.");
    }
    __ParseHwInfo(pstHwInfo, rstSlotHwInfo.stHwInfo);

    cJSON *pstModules = cJSON_GetObjectItem(pstJson, "Modules");
    if (nullptr == pstModules) {
        LOG_WRITE(EN_LOG_ERROR, "pstModules is nullptr.");
    }

    __ParseSupportedModule(pstModules, rstSlotHwInfo.staModules);

    cJSON *pstDevs = cJSON_GetObjectItem(pstJson, "Devs");
    if (nullptr == pstDevs) {
        LOG_WRITE(EN_LOG_ERROR, "pstDevs is nullptr.");
    }
    __ParseSupportedDev(pstDevs, rstSlotHwInfo.staDevs);

    LOG_WRITE(EN_LOG_DEBUG, "Slot Hw Info ---");

    return true;
}


bool
CRegisterFile::__ParseSlotHwInfoArray(cJSON *pstJson, std::vector<CSlotHwInfo> &rstaSlotHwInfo)
{

    int iSlotHwInfoSize = cJSON_GetArraySize(pstJson);

    LOG_WRITE(EN_LOG_TRACE, "Hw Info +++");
    for (int i = 0; i < iSlotHwInfoSize; ++i) {
        CSlotHwInfo stSlotHwInfo;
        memset(&stSlotHwInfo.stHwInfo, 0, sizeof(CHwInfo));

        cJSON *pstSlotHwInfo = cJSON_GetArrayItem(pstJson, i);
        __ParseSlotHwInfo(pstSlotHwInfo, stSlotHwInfo);

        rstaSlotHwInfo.push_back(stSlotHwInfo);
    }

    LOG_WRITE(EN_LOG_TRACE, "Hw Info ---");

    return true;
}


bool
CRegisterFile::__ParseSlotRegInfo(cJSON *pstJson, CSlotRegInfo &rstSlotRegInfo)
{
    /* RegInfo */
    cJSON *pstRegInfo = cJSON_GetObjectItem(pstJson, "RegInfo");
    if (nullptr == pstRegInfo) {
        LOG_WRITE(EN_LOG_ERROR, "get reg info error.");
        return false;
    }
    __ParseRegInfo(pstRegInfo, rstSlotRegInfo.staRegInfo);

    /* SupportedTypes */
    cJSON *pstSupportedTypeArray = cJSON_GetObjectItem(pstJson, "SupportedType");
    if (nullptr == pstSupportedTypeArray) {
        LOG_WRITE(EN_LOG_ERROR, "get supported type error.");
        return false;
    }
    __ParseSupportedTypeArray(pstSupportedTypeArray, rstSlotRegInfo.staType);

    /* Interfaces */
    cJSON *pstSlotHwInfoArray = cJSON_GetObjectItem(pstJson, "SlotHwInfo");
    if (nullptr == pstSlotHwInfoArray) {
        LOG_WRITE(EN_LOG_ERROR, "get reg info error.");
        return false;
    }
    __ParseSlotHwInfoArray(pstSlotHwInfoArray, rstSlotRegInfo.staSlotHwInfo);

    return true;
}


bool
CRegisterFile::__ParseSlotRegInfoArray(cJSON *pstJson, std::vector<CSlotRegInfo> &rstaSlotRegInfo)
{
    cJSON *pstSlotRegInfoArray = cJSON_GetObjectItem(pstJson, "SlotRegInfo");
    if (nullptr == pstSlotRegInfoArray) {
        LOG_WRITE(EN_LOG_ERROR, "Not find slot reg info.");
        return false;
    }

    int iSize = cJSON_GetArraySize(pstSlotRegInfoArray);

    for (int iIdx = 0; iIdx < iSize; iIdx++) {
        LOG_WRITE(EN_LOG_TRACE, "Slot%d Register Info +++", iIdx);

        CSlotRegInfo stSlotRegInfo;

        cJSON *pstSlotRegInfo = cJSON_GetArrayItem(pstSlotRegInfoArray, iIdx);
        if (nullptr == pstSlotRegInfo) {
            LOG_WRITE(EN_LOG_ERROR, "get slot info error. slot idx = %d", iIdx);
            return false;
        }

        __ParseSlotRegInfo(pstSlotRegInfo, stSlotRegInfo);

        rstaSlotRegInfo.push_back(stSlotRegInfo);
        LOG_WRITE(EN_LOG_TRACE, "Slot%d Register Info ---", iIdx);
    }

    return true;
}

bool
CRegisterFile::__ParseRegInfoArray(cJSON *pstJson, const char *pcRegInfoName, std::vector<std::vector<CRegisterInfo> > &rstaRegInfo)
{
    cJSON *pstRegInfoArray = cJSON_GetObjectItem(pstJson, pcRegInfoName);
    if (nullptr == pstRegInfoArray) {
        LOG_WRITE(EN_LOG_ERROR, "Not find reg info %s.", pcRegInfoName);
        return false;
    }

    int iSize = cJSON_GetArraySize(pstRegInfoArray);

    for (int i = 0; i < iSize; i++) {
        cJSON *pstRegInfo = cJSON_GetArrayItem(pstRegInfoArray, i);
        if (nullptr == pstRegInfo) {
            LOG_WRITE(EN_LOG_ERROR, "No %s reg info.", pcRegInfoName);
            return true;
        }

        std::vector<CRegisterInfo> staRegInfo;
        __ParseRegInfo(pstRegInfo, staRegInfo);
        rstaRegInfo.push_back(staRegInfo);
    }

    return true;
}


bool
CRegisterFile::__ParseBoardRegInfo(cJSON *pstJson, CBoardRegisterInfo &rstBoardRegInfo)
{
    cJSON *pstBoardRegInfo = cJSON_GetObjectItem(pstJson, "BoardRegInfo");
    if (nullptr == pstBoardRegInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Not find board reg info.");
        return false;
    }

    __ParseRegInfoArray(pstBoardRegInfo, "LogicDrv", rstBoardRegInfo.staLogicDrvInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "I2cSwitch", rstBoardRegInfo.staI2cSwitchInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "UartSwitch", rstBoardRegInfo.staUartSwitchInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "I2c", rstBoardRegInfo.staI2cInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Uart", rstBoardRegInfo.staUartInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "FanLogicDrv", rstBoardRegInfo.staFanLogicDrvInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "Apsd", rstBoardRegInfo.staApsdInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "WatchDog", rstBoardRegInfo.staWatchDogInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "ResetBoard", rstBoardRegInfo.staResetBoardInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "ResetTypeRegister", rstBoardRegInfo.staResetTypeInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "FanBoard", rstBoardRegInfo.staFanBoardInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "PsuBoard", rstBoardRegInfo.staPsuBoardInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "LedBoard", rstBoardRegInfo.staLedBoardInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "OscBoard", rstBoardRegInfo.staOscBoardInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "SlotBoard", rstBoardRegInfo.staSlotBoardInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "Logic", rstBoardRegInfo.staLogicInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "Fan", rstBoardRegInfo.staFanInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Psu", rstBoardRegInfo.staPsuInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Led", rstBoardRegInfo.staLedInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Temp", rstBoardRegInfo.staTempInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "EEprom", rstBoardRegInfo.staEEpromInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Flash", rstBoardRegInfo.staFlashInfo);

    __ParseRegInfoArray(pstBoardRegInfo, "Mdio", rstBoardRegInfo.staMdioInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "NetSwitch", rstBoardRegInfo.staNetSwitchInfo);
    __ParseRegInfoArray(pstBoardRegInfo, "Ltc4215", rstBoardRegInfo.staLtc4215Info);
    __ParseRegInfoArray(pstBoardRegInfo, "Mp5023", rstBoardRegInfo.staMp5023Info);
    __ParseRegInfoArray(pstBoardRegInfo, "Max1139", rstBoardRegInfo.staMax1139Info);
    LOG_WRITE(EN_LOG_TRACE, "__ParseBoardRegInfo ---");

    return true;
}

bool
CRegisterFile::__ParseDevInfo(cJSON *pstJson, std::vector<CDevInfo> &rstaDevInfo)
{
    cJSON *pstDevInfoArray = cJSON_GetObjectItem(pstJson, "DevInfo");

    int iSize = cJSON_GetArraySize(pstDevInfoArray);

    for (int i = 0; i < iSize; ++i) {

        CDevInfo stDevInfo = {0};

        cJSON *pstDevInfo = cJSON_GetArrayItem(pstDevInfoArray, i);

        cJSON *pstDevType = cJSON_GetObjectItem(pstDevInfo, "DevType");
        /*cJSON *pstDevFunc = cJSON_GetObjectItem(pstDevInfo, "DevFunc"); */
        cJSON *pstDevAddr = cJSON_GetObjectItem(pstDevInfo, "DevAddr");
        cJSON *pstDevProto = cJSON_GetObjectItem(pstDevInfo, "DevProto");

        stDevInfo.uiType = ms_staDevTypeMap[pstDevType->valuestring];
        /*stDevInfo.uiFunc = ms_staDevFuncMap[pstDevFunc->valuestring]; */
        stDevInfo.uiDevAddr = pstDevAddr->valueint;
        stDevInfo.uiProto = ms_staProtoMap[pstDevProto->valuestring];

        LOG_WRITE(EN_LOG_NOTICE, "DevType = %s:%d, DevAddr = %02x, DevProto = %s:%d",
                  pstDevType->valuestring,
                  stDevInfo.uiType,
                  stDevInfo.uiDevAddr,
                  pstDevProto->valuestring,
                  stDevInfo.uiProto);

        rstaDevInfo.push_back(stDevInfo);
    }

    return true;
}

bool
CRegisterFile::__ParseModuleInfo(cJSON *pstJson, std::vector<CModuleInfo> &rstaModuleInfo)
{
    cJSON *pstModuleInfoArray = cJSON_GetObjectItem(pstJson, "ModuleInfo");

    if (nullptr == pstModuleInfoArray) {
        LOG_WRITE(EN_LOG_ERROR, "Not find module info.");
        return false;
    }

    int iSize = cJSON_GetArraySize(pstModuleInfoArray);

    for (int i = 0; i < iSize; ++i) {
        CModuleInfo stModuleInfo;

        cJSON *pstModuleInfo = cJSON_GetArrayItem(pstModuleInfoArray, i);

        cJSON *pstModuleType = cJSON_GetObjectItem(pstModuleInfo, "ModuleType");

        stModuleInfo.uiType = ms_staModuleTypeMap[pstModuleType->valuestring];

        LOG_WRITE(EN_LOG_TRACE, "[ModuleType = %s:%d]n",
                  pstModuleType->valuestring,
                  stModuleInfo.uiType);

        __ParseDevInfo(pstModuleInfo, stModuleInfo.staDevInfo);

        rstaModuleInfo.push_back(stModuleInfo);
    }

    return true;

}

bool
CRegisterFile::__ParseCardInfo(cJSON *pstJson, std::map<uint32_t, CCardInfo> &rstaCardRegInfo)
{
    LOG_WRITE(EN_LOG_TRACE, "__ParseCardInfo +++");
    cJSON *pstCardInfoMap = cJSON_GetObjectItem(pstJson, "CardRegInfo");
    cJSON *pstCardInfo = pstCardInfoMap->child;

    while (pstCardInfo != nullptr) {

        std::string staCardType(pstCardInfo->string);
        uint32_t uiCardType = ms_staCardTypeMap[staCardType];        /* atoi(pstCardTypeMap->string); */

        LOG_WRITE(EN_LOG_TRACE, "CardType = %s", pstCardInfo->string);

        CCardInfo stCardInfo;
        __ParseModuleInfo(pstCardInfo, stCardInfo.staModuleInfo);
        __ParseDevInfo(pstCardInfo, stCardInfo.staDevInfo);

        rstaCardRegInfo[uiCardType] = stCardInfo;


        pstCardInfo = pstCardInfo->next;
    }

    LOG_WRITE(EN_LOG_TRACE, "__ParseCardInfo ---");

    return true;
}

bool
CRegisterFile::Load()
{
    FILE *pfFile = NULL;
    char *pcBuf = NULL;
    INT iReadCount = 0;
    cJSON *pstJson = NULL;

    LOG_WRITE(EN_LOG_TRACE, "Register File Load +++");

    const char *pcFilePath = "/etc/RegisterFile";

    pfFile = fopen(pcFilePath, "r");
    if (NULL == pfFile) {
        LOG_WRITE(EN_LOG_ERROR, "fopen is error. pcFileName = %s", pcFilePath);
        return false;
    }


    fseek(pfFile, 0, SEEK_END);
    int32_t iPos = ftell(pfFile);
    fseek(pfFile, 0, SEEK_SET);
    if (iPos < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid file. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

#if 0
    if (pos >= MAX_CONF_SIZE) {
        LOG_WRITE(EN_LOG_ERROR, "Too large file. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }
#endif

    pcBuf = (char *)malloc(iPos + 1);
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "No enough memory. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

    iReadCount = fread(pcBuf, 1, iPos, pfFile);
    if (iReadCount <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to read the file. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }
    LOG_WRITE(EN_LOG_TRACE, "iReadCount = %d", iReadCount);

    /* fclose(pfFile); */
    pcBuf[iPos] = '\0'; /* end of string */

    pstJson = cJSON_Parse(pcBuf);
    if (NULL == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "Parse file failed. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

    if (false == __ParseBoardRegInfo(pstJson, ms_stBoardRegInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "Parse board reg info error. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

    if (false == __ParseSlotRegInfoArray(pstJson, ms_staSlotRegInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "Parse slot reg info error. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

    if (false == __ParseCardInfo(pstJson, ms_staCardRegInfo)) {
        LOG_WRITE(EN_LOG_ERROR, "Parse card reg info error. pcFileName = %s", pcFilePath);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, "Register File Load ---");

    return true;

ERR_LABEL:

    DEF_SAFE_FCLOSE(pfFile);
    DEF_SAFE_FREE(pcBuf);

    return false;

}

uint32_t
CRegisterFile::ConvertDevType(const std::string &strTypeName)
{
    const auto &it = ms_staDevTypeMap.find(strTypeName);
    if (it == ms_staDevTypeMap.end()) {
        return DEV_TYPE_NONE;
    }

    return it->second;
}
