/*************************************************************************
 *
 *            Copyright (c) 2016 by Microsemi Corp. Inc.
 *
 *  This software is copyrighted by, and is the sole property of Microsemi
 *  Corp. All rights, title, ownership, or other interests in the
 *  software  remain the property of Microsemi Corp. This software
 *  may only be used in accordance with the corresponding license
 *  agreement.  Any unauthorized use, duplication, transmission,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of Microsemi Corp.
 *
 *  Microsemi Corp. reserves the right to modify this software without
 *  notice.
 *
 *************************************************************************
 *
 *  File Revision: 1.0
 *
 *************************************************************************
 *
 *  Description:  Code Encryption and PoE 15bytes protocol communication global types
 *
 *************************************************************************/

#ifndef MSCC_POE_GLOBAL_TYPES_H_
#define MSCC_POE_GLOBAL_TYPES_H_

/*=========================================================================
/ CONSTANTS
/========================================================================*/


#ifndef POE_NULL
#define POE_NULL (void *)0
#endif

#define _OUT     /* used to clarify if func param is in/out */
#define _INOUT   /* used to clarify if func param is in/out */

#define API_VERSION      0x02000F  // 2.0.15

#define MAX_IC_ON_BOARD             8
#define MAX_POWER_BUDGET            16
#define MAX_CH_PER_SYSTEM           48
#define MSG_SIZE                    15
#define RX_BUFF_SIZE                50

#define SOFTWARE_UPDATE_S19_ACK_UART_DELAY_mS  50


/*=========================================================================
/ TYPES
/========================================================================*/



/* -- MACRO to get specific 8 bits from 16 bit var -- */
#define W_L(x) ( (U8)(  (x)      & 0x00FF) )
#define W_H(x) ( (U8)( ((x)>> 8) & 0x00FF) )

/* -- MACRO to get specific 8 bits from 32 bit var -- */
#define L_LL(x) ( (U8)(  (x)       & 0x000000FF) )
#define L_LH(x) ( (U8)( ((x)>> 8 ) & 0x000000FF) )
#define L_HL(x) ( (U8)( ((x)>> 16) & 0x000000FF) )
#define L_HH(x) ( (U8)( ((x)>> 24) & 0x000000FF) )


#define mscc_MIN(a, b)  ((a) < (b) ? (a) : (b))
#define mscc_MAX(a, b)  ((a) > (b) ? (a) : (b))


/* Types */
typedef char               S8   ;
typedef unsigned char      U8   ;
typedef signed short       S16  ;
typedef unsigned short     U16  ;
typedef long               S32  ;
typedef unsigned long      U32  ;

#define DEF_Get_All_Ports_Status_t_NUMBER_OF_ELEMENTS                    11
#define DEF_Get_All_Ports_Power_t_NUMBER_OF_ELEMENTS                     11
#define DEF_Get_UDL_Counters_t_NUMBER_OF_ELEMENTS                        24
#define DEF_Get_All_Ports_Class_t_NUMBER_OF_ELEMENTS                     16
#define DEF_Get_Latches_t_NUMBER_OF_ELEMENTS                             24
#define DEF_Get_Detection_Failure_Counters_t_NUMBER_OF_ELEMENTS          24
#define DEF_Get_All_4Pair_Ports_Power_t_NUMBER_OF_ELEMENTS               11
#define DEF_Get_global_port_counters_t_NUMBER_OF_ELEMENTS                11



#define B_DeratingUserTemp      0
#define B_BPMPrivateLabel       0
#define B_Command               0x00
#define B_Program               0x01
#define B_Paramz4Pair           0x01
#define B_Request               0x02
#define B_EnDis4Pair            0x02
#define B_Telemetry             0x03
#define B_EnDis4Pair2           0x03
#define B_PortCounters          0x04
#define B_Test		           	0x04
#define B_Channel               0x05
#define B_PowerIndication	 	0x05
#define B_E2                    0x06
#define B_Global                0x07
#define B_Priority              0x0A /* 10 */
#define B_Supply                0x0B /* 11 */
#define B_EnDis                 0x0C /* 12 */
#define B_PortStatus            0x0E /* 14 */
#define B_SaveConfig            0x0F /* 15 */
#define B_Main                  0x17 /* 23 */
#define B_PowerManage           0x18 /* 24 */
#define B_Measurementz          0x1A /* 26 */
#define B_SWversion             0x21 /* 33 */
#define B_Versionz              0x1E /* 30 */
#define B_Paramz                0x25 /* 37 */
#define B_Maskz                 0x2B /* 43 */
#define B_RestoreFact           0x2D /* 45 */
#define B_PortsStatus1          0x31 /* 49 */
#define B_PortsStatus2          0x32 /* 50 */
#define B_PortsStatus3          0x33 /* 51 */
#define B_Latches               0x3A /* 58 */
#define B_SystemStatus          0x3D /* 61 */
#define B_UserByte              0x41 /* 65 */
#define B_TmpMatrix             0x43 /* 67 */
#define B_ChannelMatrix         0x44 /* 68 */
#define B_PortsStatus4          0x47 /* 71 */
#define B_PortsStatus5          0x48 /* 72 */
#define B_Latches2              0x49 /* 73 */
#define B_PortFullInit          0x4A /* 74 */
#define B_PortsPower1           0x4B /* 75 */
#define B_PortsPower2           0x4C /* 76 */
#define B_PortsPower3           0x4D /* 77 */
#define B_N                     0x4E /* 78 */
#define B_PortsPower4           0x4F /* 79 */
#define B_PortsPower5           0x50 /* 80 */
#define B_ForcePower            0x51 /* 81 */
#define B_Report                0x52 /* 82 */
#define B_ReadsRon              0x52 /* 82 */
#define B_WriteRons             0x53 /* 83 */
#define B_Reset                 0x55 /* 85 */
#define B_Individual_Mask       0x56 /* 86 */
#define B_PowerBudget           0x57 /* 87 */
#define B_UDLCounter1           0x59 /* 89 */
#define B_UDLCounter2           0x5A /* 90 */
#define B_PoEDeviceVersion      0x5E /* 94 */
#define B_PowerManageMode       0x5F /* 95 */
#define B_ExpendedPowerInfo     0x60 /* 96 */
#define B_AllPortClass          0x61 /* 97 */
#define B_IrqMask               0x63 /* 99 */
#define B_All_channels          0x80 /* 128 */
#define B_SystemStatus2		 	0x84 /* 132 */
#define B_DetCnt1               0x85 /* 133 */
#define B_DetCnt2               0x86 /* 134 */
#define B_DeviceParams          0x87 /* 135 */
#define B_EnDis2                0x88 /* 136 */
#define B_UDLCounter3           0x89 /* 137 */
#define B_UDLCounter4           0x8A /* 138 */
#define B_PortsStatus6          0x8B /* 139 */
#define B_PortsStatus7          0x8C /* 140 */
#define B_PortsStatus8          0x8D /* 141 */
#define B_PortsStatus9          0x8E /* 142 */
#define B_PortsStatus10         0x8F /* 143 */
#define B_PortsPower6           0x90 /* 144 */
#define B_PortsPower7           0x91 /* 145 */
#define B_PortsPower8           0x92 /* 146 */
#define B_PortsPower9           0x93 /* 147 */
#define B_PortsPower10          0x94 /* 148 */
#define B_Latches3              0x95 /* 149 */
#define B_Latches4              0x96 /* 150 */
#define B_NonVolMem             0x97 /* 151 */
#define B_DetCnt3               0x98 /* 152 */
#define B_DetCnt4               0x99 /* 153 */
#define B_AllPortsPower         0x9C /* 156 */
#define B_SystemOKMask          0xA1 /* 161 */
#define B_TemporarySupply       0xA2 /* 162 */
#define B_ExtendDeviceParams    0xA3 /* 163 */
#define B_BackplanePowerData    0xA4 /* 164 */
#define B_BPMReqData            0xA5 /* 165 */
#define B_Layer2_PD             0xA6 /* 166 */
#define B_PowerBudgetSourceType 0xA7 /* 167 */
#define B_Layer2_LLDP_PSE  		0xA8 /* 168 */
#define B_PowerUp_ALT_B         0xA9 /* 169 */
#define B_Layer2_CDP_PD         0xAA /* 170 */
#define B_Layer2_CDP_PSE        0xAB /* 171 */
#define B_Layer2_CDP_PD_2       0xAC /* 172 */
#define B_Supply4Pair           0xAD /* 173 */
#define B_ActualSystemData      0xAD /* 173 */
#define B_TemporarySupply4Pair  0xAE /* 174 */
#define B_PortFullInit4Pair     0xAF /* 175 */
#define B_NewPortStatus         0xB0 /* 176 */
#define B_Ports4PairPower1      0xB0 /* 176 */
#define B_Ports4PairPower2      0xB1 /* 177 */
#define B_Layer2_LLDP_Math      0xB1 /* 177 */
#define B_Ports4PairPower3      0xB2 /* 178 */
#define B_Ports4PairPower4      0xB3 /* 179 */
#define B_Ports4PairPower5      0xB4 /* 180 */
#define B_Ports4PairPower6      0xB5 /* 181 */
#define B_Ports4PairPower7      0xB6 /* 182 */
#define B_Ports4PairPower8      0xB7 /* 183 */
#define B_Ports4PairPower9      0xB8 /* 184 */
#define B_Ports4PairPower10     0xB9 /* 185 */
#define B_Derating              0xBA /* 186 */
#define B_ClassPower            0xBB /* 187 */
#define B_AllPortsDelivering    0xC0 /* 192 */
#define B_Event					0xC1 /* 193 */

#define B_SYSTEM_STATUS_ECHO	0xFF /* 255 */



/*}MSCC_PROTOCOL_CMD_e;*/


/*
    * typedef: enum POE_BOOL
    *
    * Description: Enumeration of boolean.
    *
    * Enumerations:
    *    POE_FALSE - false.
    *    POE_TRUE  - true.
    */
typedef enum
{
	ePOE_FALSE = 0,
	ePOE_TRUE
} POE_BOOL_e;


typedef enum
{
	eRead_WithTimeout = 0,
	eRead_immediate
} READ_MODE_e;



typedef enum
{
	eTransmitMsg_OK = 0,          /* operation succeed */
	eTransmitMsg_RestartManager,  /* new sync is required */
	eTransmitMsg_ExitManager      /* fatal error occurred – exit from poe task */

} TRANSMIT_MSG_STATUS_e;


/*====================================================
/---- 15bytes Protocol messages that can be send to PoE Device ----
/====================================================*/
typedef enum
{
	eReset_Command = 0,
	eRestore_Factory_Defaults,
	eSave_System_Settings,
	eSet_System_Status,
	eSet_User_Byte,
	eSet_PoE_Device_Parameters,
	eSet_PoE_Device_Register,
	eSet_Power_Banks,
	eSet_PM_Method,
	eSet_Power_Indication_LED,
	eSet_Temporary_Matrix,
	eProgram_Global_Matrix,
	eSet_Enable_Disable_Channels,
	eSet_Power_Limit_for_Channels,
	eSet_Temporary_Power_Limit_for_Channels,
	eSet_Port_Priority,
	eSet_Port_Parameters,
	eSet_Force_Power,
	eSet_Interrupt_Mask,
	eSet_Individual_Mask,
	eSet_System_OK_LED_Mask_Registers,
	eSet_Port_Layer2_LLDP_PD_Data,
	eSet_Power_Bank_Power_Source_Type,
	eSet_Port_PowerUp_ALT_B,
	eSet_Port_Layer2_CDP_PD_Data,
	eSet_Port_Layer2_CDP_PD_Data_2,
	eSet_Power_Limits_for_4_Pair_Channels,
	eSet_Temporary_Power_Limits_for_4_Pair_Channels,
	eSet_4Pair_Port_Parameters,
	eGet_4Pair_Port_Parameters,
	eSet_Derating_Data,
	eSet_Derating_User_Temperature,
	eSet_Enable_Disable_4_Pair_for_Channels,

	eGet_Software_Version,
	eGet_PoE_Device_Status,
	eGet_PoE_Device_Register,
	eGet_System_Status,
	eGet_System_Status2,
	eGet_Power_Supply_Parameters,
	eGet_Power_Supply_Voltage,
	eGet_Power_Banks,
	eGet_All_Ports_Status,
	eGet_All_Ports_Power,
	eGet_All_Ports_Enable_Disable,
	eGet_Single_Port_Status,
	eGet_Port_Power_Limit,
	eGet_Port_Priority,
	eGet_Port_Measurements,
	eGet_Physical_Port_Number_from_Temporary_Matrix,
	eGet_Physical_Port_Number_from_Active_Matrix,
	eGet_UDL_Counters,
	eGet_PM_Method,
	eGet_Total_Power,
	eGet_All_Ports_Class,
	eGet_All_Ports_delivering_power_state,
	eGet_Port_Event_Cause,
	eGet_Latches,
	eGet_Interrupt_Mask,
	eGet_System_OK_LED_Mask_Registers,
	eGet_Power_Indication_LED,
	eGet_Detection_Failure_Counters,
	eGet_Individual_Mask,
	eGet_Port_Layer2_LLDP_PSE_Data,
	eGet_Port_PowerUp_ALT_B,
	eGet_Port_Layer2_CDP_PSE_Data,
	eGet_All_4Pair_Ports_Power,
	eGet_4Pair_Port_Power_Limit,
	eGet_4Pair_Port_measurements,
	eGet_Derating_Data,
	eGet_System_Measurements,
	eGet_Derating_User_Temperature,
	eGet_All_Ports_Enable_Disable_4pair,
	eGet_Extended_Port_Status,
	eGet_Internal_Port_Status,
	eGet_global_port_counters,
	eGet_LLDP_Math_Conversion
} Protocol_15Bytes_Msg_Type_e;



/* possible values rec by 'GET_SINGLE_PORT_STATUS' or 'GET_ALL_PORTS_STATUS' */
typedef enum
{
	ePort_is_on__Valid_capacitor_detected                = 0x00,
	ePort_is_on__Valid_resistor_detected                 = 0x01,
	ePort_is_on__4pair                                   = 0x02,
	ePort_is_on__2pair_Power_Sequence                    = 0x03,
	ePort_is_on__4pair_Power_Sequence                    = 0x04,
	ePort_is_off__Main_supply_voltage_is_high            = 0x06,
	ePort_is_off__Main_supply_voltage_is_low             = 0x07,
	ePort_is_off__Disable_all_ports_pin_is_active        = 0x08,
	ePort_is_off__Non_existing_port_number               = 0x0C,
	ePort_is_yet_undefined                               = 0x11,
	ePort_is_off__Internal_hardware_fault_1              = 0x12,
	ePort_is_off__User_setting                           = 0x1A,
	ePort_is_off__Detection_is_in_process                = 0x1B,
	ePort_is_off__Non_802_3af_powered_device             = 0x1C,
	ePort_is_off__Overload_and_Underload_states          = 0x1D,
	ePort_is_off__Underload_state                        = 0x1E,
	ePort_is_off__Overload_state                         = 0x1F,
	ePort_is_off__Power_budget_exceeded                  = 0x20,
	ePort_is_off__Internal_hardware_fault                = 0x21,
	ePort_is_off__Voltage_injection_into_the_port        = 0x24,
	ePort_is_off__Detection_failed_due_to_short_on_port  = 0x25,
	ePort_is_off__Discharged_load                        = 0x26,
	ePort_is_on__Detection_regardless__Force_On          = 0x2B,
	eUndefined_error_during_Force_On                     = 0x2C,
	eSupply_voltage_higher_than_settings                 = 0x2D,
	eSupply_voltage_lower_than_settings                  = 0x2E,
	eDisable_PDU_flag_raised_during_Force_On             = 0x2F,
	ePort_is_off__Forced_power_error_due_to_Overload     = 0x31,
	ePort_is_off__Out_of_power_budget__during_Force_On   = 0x32,
	eCommunication_error_with_PoE_devices_after_Force_On = 0x33,
	ePort_is_off__Short_condition                        = 0x34,
	ePort_is_off__Over_temperature_at_the_port           = 0x35,
	ePort_is_off__Device_is_too_hot                      = 0x36,
	eUnknown_device_port_status                          = 0x37,
	eForce_Power_Error_Short_Circuit                     = 0x38,
	eForce_Power_Error_Channel_Over_Temperature          = 0x39,
	eForce_Power_Error_Chip_Over_Temperature             = 0x3A,
	ePower_Management_Static                             = 0x3C,
	ePower_Management_Static_ovl                         = 0x3D,
	eForce_Power_Error_Management_Static                 = 0x3E,
	eForce_Power_Error_Management_Static_ovl             = 0x3F,
	ePort_is_off__Class_Error                            = 0x43,

	ePORT_STATUS_UNKNOWN                                 = 0xFF  /* internal software state which mark that port status was never read and updated*/
} Port_Status_e;



/* GET/SET individual Mask command is capable to do Get/Set to various single parameters. This enum define to which parameetr we should do Get/Set */
typedef enum ind_mask_opt_e
{
	eIgnore_higher_priority                      = 0x00,
	eSupports_Legacy_detection                   = 0x01,
	eResDetectShort                              = 0x04,
	eLegacy_4pair_class_mismatch_equal_0         = 0x08,
	eIgnoreClass4Pair                            = 0x0A,
	eIgnore_higher_priority_grouping             = 0x0C, /*  (Can be used only when Ignore higher priority = '0') */
	eIRQ_COR_by_Get_System_Status                = 0x0D,
	eLayer2_ignore_Hi_Pri_at_pwr_change_Req 	 = 0x0E,
	eSupports_backoff                            = 0x11,
	eClass4At_AF_Reported_as_class4				 = 0x12,
	eSupport_12_5K_detection                     = 0x13,
	eHardware_reset_on_ASIC_error                = 0x14,
	eCalculated_power_disconnect_flag            = 0x15,
	eLED_stream_type                             = 0x16,
	eLED_reverse                                 = 0x18,
	eLED_stream_negative                         = 0x19,
	eI2C_restart_enable                          = 0x1B,
	eMessage_ready_notify                        = 0x1E,
	ePSE_powering_PSE_checking                   = 0x1F,
	exSystem_pin_behavior                        = 0x28,
	eIgnore_high_priority_at_flexible_guard_band = 0x29,
	eEnable_ASIC_Refresh                         = 0x2A,
	eCheck_inrush                                = 0x2B,
	eLayer2_LLDP_enable                          = 0x2E,
	eLayer2_priority_by_PD                       = 0x2F,
	eExtra_power_enable                          = 0x30,
	eTemperature_Derating_enable                 = 0x32,
	eTemperature_Derating_Negative_Delta         = 0x33,
	eUse_new_matrix_command_4pair                = 0x34,
	eSystem_support_reduced_cap                  = 0x37,
	eClass_0_equal_AF                            = 0x38,
	eClass_1_2_3_equal_AF                        = 0x39,
	eClass_bypass_2nd_finger_error               = 0x3A,
	eClass_error_equal_0                         = 0x3B,
	eClass_error_equal_4                         = 0x3C,
	eSystem_POH_enable                           = 0x3D,
	ePower_forwarding                            = 0x43,
	eSupport_4pair_AF                            = 0x44,
	eMax_power_check                             = 0x45,
	eSingleDetectionFaileEvent                   = 0x46,
	eLLDP_Best_Effort                   		 = 0x47,
	eIgnore_Dynamic_for_Port_PowerUp             = 0x48,
} Individual_Mask_e;



typedef enum
{
	Counter_kind_InvalidSigCnt = 0,
	PowerDeniedCnt,
	PoE_OVLCnt,
	PoE_UDL_Cnt,
	PoE_SC_Cnt
} Counter_kind_e;


typedef enum
{
	Counters_group_0 = 0, /* port0 -  10 */
	Counters_group_1,     /* port11 - 21 */
	Counters_group_2,     /* port22 - 32 */
	Counters_group_3,     /* Port33 - 43 */
	Counters_group_4      /* Port44 - 47 */
} Counter_Group_e;


typedef enum
{
	eTYPE_MASK_AF = 0,
	eTYPE_MASK_AT,
	eTYPE_MASK_POH,
	eTYPE_MASK_stay_with_last_mode = B_N
} Type_Mask_e;


typedef enum
{
	e_AF_Mode = 0,
	e_AT_Mode,
	e_POH_Mode,
} Operation_Mode_e;



typedef enum
{
	eIC_Exp_0 = 0,
	eIC_Exp_4 = 4,
	eIC_Exp_8 = 8,
	eIC_Exp_12 = 12,
	eIC_Exp_Unknown = 15
} IC_Exp_e;


typedef enum
{
	eFeature_EnDis_Type_None       = 0,
	eFeature_EnDis_Type_EnDisCh    = 1,
	eFeature_EnDis_Type_CapDis     = 2,
	eFeature_EnDis_Type_IcutDis    = 3,
	eFeature_EnDis_Type_ConfigAll  = 7
} Feature_EnDis_Type_e;


typedef enum
{
	eIC_Status_None = 0,                           /* No PoE Device is connected and also no PoE Device was expected to be connected */
	eIC_Status_Ok,                                 /* The Configured PoE device is the one that is connected */
	eIC_Status_Unexpected_PoE_detection1,          /* Unexpected PoE Device was connected to the Sys and NO Save was done */
	eIC_Status_Unexpected_PoE_detection2,          /* Unexpected PoE Device was connected to the Sys and Save was done */
	eIC_Status_Fail_Missing_PoE_Device,            /* Missing PoE Device, No Save Done */
	eIC_Status_Different_PoE_device_was_detected1, /* Wrong PoE Device was connected to the Sys and NO Save was done */
	eIC_Status_Different_PoE_device_was_detected2  /* Wrong PoE Device was connected to the Sys and Save was done */
} Asic_Status_e;


typedef enum
{
	ePOE_STATUS_OK                                                      =  0,
	ePOE_STATUS_ERR_POE_DEVICE_RESET_DETECTED                           = -1,
	ePOE_STATUS_ERR_COMMUNICATION_DEVICE_ERROR  						= -2,
	ePOE_STATUS_ERR_COMMUNICATION_TIMEOUT                               = -3,
	ePOE_STATUS_ERR_REPLY_COMMAND_RECEIVED_WRONG_CHECKSUM               = -4,
	ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_CONFLICT_IN_SUBJECT_BYTES    = -5,
	ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_WRONG_DATA_BYTE_VALUE        = -6,
	ePOE_STATUS_ERR_REPLY_FAILED_EXECUTION_UNDEFINED_KEY_VALUE          = -7,
	ePOE_STATUS_ERR_REPLY_CODE_OUT_OF_RANGE                             = -8,
	ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_DATA_BYTES                       = -9,
	ePOE_STATUS_ERR_RX_MSG_CONFLICT_IN_KEY_BYTES                        = -10,
	ePOE_STATUS_ERR_RX_MSG_CHECKSUM_ERR                                 = -11,
	ePOE_STATUS_ERR_RX_MSG_ECHO_MISMATCH                                = -12,
	ePOE_STATUS_ERR_MSG_TYPE_NOT_EXIST                                  = -13,
	ePOE_STATUS_ERR_MUTEX_INIT_ERROR                                    = -14,
	ePOE_STATUS_ERR_MUTEX_DESTROY_ERROR                                 = -15,
	ePOE_STATUS_ERR_MUTEX_LOCK_ERROR                                    = -16,
	ePOE_STATUS_ERR_MUTEX_UNLOCK_ERROR                                  = -17,
	ePOE_STATUS_ERR_SLEEP_FUNCTION_ERROR                                = -18,
	ePOE_STATUS_ERR_MUTEX_ALREADY_INITIALIZED                           = -19,
	ePOE_STATUS_ERR_MUTEX_ALREADY_DISPOSED                              = -20,
	ePOE_STATUS_ERR_OPEN_COMM_DEVICE_ERROR                            	= -21,
	ePOE_STATUS_ERR_CLOSE_COMM_DEVICE_ERROR                             = -22,
	ePOE_STATUS_ERR_INITIALIZATION_ERROR                                = -23,
	ePOE_STATUS_ERR_UART_DRIVER_ERROR                                   = -24,
	ePOE_STATUS_ERR_DEVICE_SELECTION_ERROR                              = -25,
	ePOE_STATUS_ERR_OPEN_QUEUE                                          = -26,
	ePOE_STATUS_ERR_QUEUE_TX_MSG_FAILED                                 = -27,
	ePOE_STATUS_ERR_QUEUE_MSG_SIZE_ERROR                                = -28,
	ePOE_STATUS_ERR_QUEUE_RX_MSG_ERROR                                  = -29,
	ePOE_STATUS_ERR_QUEUE_NOT_INITIALIZED                               = -30,
	ePOE_STATUS_ERR_OPEN_UART_DEVICE_ERROR                              = -31,
	ePOE_STATUS_ERR_POE_MCU_FIRMWARE_ERROR              				= -32,
	ePOE_STATUS_ERR_POE_MCU_FIRMWARE_DOWNLOAD_IS_REQUIRED  				= -33,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TPE            				= -34,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_NO_TE_TPE                         = -35,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_TOP                               = -36,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_controller_error              = -37,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_PoE_firmware_download_is_required = -38,
	ePOE_STATUS_ERR_FIRMWARE_DOWNLOAD_FAILED_TO_OPEN_FILE               = -39,
	ePOE_STATUS_MAX						                                = -40,
} PoE_Status_e;





typedef enum
{
	ePM1_Full_Dynamics_Consumption = 0,
	ePM1_Classes_1_to_3_TPPL_and_classes_0_and_4_Dynamic,
	ePM1_Classes_0_to_3_TPPL_and_class_4_Dynamic,
	ePM1_Classes_1_to_4_TPPL_and_class_0_Dynamic,
	ePM1_Classes_0_to_4_TPPL,
	ePM1_LLDP_TPPL_and_NonLLDP_Dynamic,
	ePM1_Classes_0_to_3_Dynamic_and_class_4_TPPL,
	ePM1_User_defined_per_port_Sum_as_TPPL = 0x80
} PM1_System_Power_Calculated_Method_e;


typedef enum
{
	ePM2_PPL = 0,
	ePM2_Class_Limit,
	ePM2_ICUT_Max,
	ePM2_User_defined_per_port_PM2_per_Port = 0x80
} PM2_Port_Power_Limit_e;


typedef enum
{
	ePM3_No_Condition = 0,
	ePM3_Condition_on_Classes_1_to_3,
	ePM3_Condition_on_Classes_0_to_3,
	ePM3_Condition_on_Classes_1_to_4,
	ePM3_Condition_on_Classes_0_to_4,
	ePM3_User_defined_per_port = 0x80
} PM3_Start_Up_Condition_e;


typedef enum
{
	ePoE_Device_is_OK = 0,
	eFail_or_missing_PoE_Device
} Device_Fail_e;


typedef enum
{
	eTemp_Dis_Temperature_is_OK = 0,
	eTemp_Dis_PoE_device_caused_disconnection_due_to_high_temperature
} Temperature_Disconnect_e;


typedef enum
{
	eTemp_Alarm_Temperature_is_OK = 0,
	eTemp_Alarm_High_temperature
} Temperature_Alarm_e;


typedef enum
{
	eClass0 = 0,
	eClass1,
	eClass2,
	eClass3,
	eClass4,
	eClassUnknown
} Class_Number_e;


typedef enum
{
	eTemperature_zone_unknown = 0,
	eTemperature_zone_1,
	eTemperature_zone_2,
	eTemperature_zone_3,
	eTemperature_zone_4
} Temperature_Zone_e;


typedef enum
{
	ePower_Type_2_PSE = 0,
	ePower_Type_2_PD ,
	ePower_Type_1_PSE ,
	ePower_Type_1_PD
} Power_Type_e;


typedef enum
{
	ePD_Power_Type_2_PD = 1 ,
	ePD_Power_Type_1_PD = 3
} Set_PD_Power_Type_e;



typedef enum
{
	eSet_Power_source_Unknown = 0,
	eSet_Power_source_PSE ,
	eSet_Power_source_LOCAL ,
	eSet_Power_source_PSE_and_LOCAL
} Set_Power_Source_e;


typedef enum
{
	ePower_source_Unknown = 0,
	ePower_source_Primary_power_source ,
	ePower_source_backup_source ,
	ePower_source_Reserved
} Power_Source_Type_e;


typedef enum
{
	ePortPriority_Unknown = 0,
	ePortPriority_Critical ,
	ePortPriority_High ,
	ePortPriority_Low
} Port_Priority_e;


typedef enum
{
	ePowerClass_Unknown = 0,
	ePowerClass_0,
	ePowerClass_1,
	ePowerClass_2,
	ePowerClass_3,
	ePowerClass_4,
} Power_Class_e;


typedef enum
{
	eSparePair = 0,
	eDataPair,
} PSE_Power_Pair_e;


typedef enum
{
	eLayer2_LLDP_Port_is_off     = 0, /* 00 */
	eLayer2_LLDP_Port_is_On      = 1, /* 01 */
	eLayer2_LLDP_Port_is_On_CDP  = 2, /* 10 */
	eLayer2_LLDP_Port_is_On_LLDP = 3  /* 11 */
} Layer2_LLDP_Port_State_e;



typedef enum
{
	eLayer2_request_was_executed = 0, /* 00 */
	eLayer2_request_in_process      , /* 01 */
} Layer2_request_Pending_e;



typedef enum
{
	ePortsStatusGroup1_0_to_10   = B_PortsStatus1,
	ePortsStatusGroup2_11_to_21  = B_PortsStatus2,
	ePortsStatusGroup3_22_to_25  = B_PortsStatus3,
	ePortsStatusGroup4_26_to_36  = B_PortsStatus4,
	ePortsStatusGroup5_37_to_47  = B_PortsStatus5
} Ports_Status_Group_e;


typedef enum
{
	ePortsPowerGroup1_0_to_10   = B_PortsPower1,
	ePortsPowerGroup2_11_to_21  = B_PortsPower2,
	ePortsPowerGroup3_22_to_25  = B_PortsPower3,
	ePortsPowerGroup4_26_to_36  = B_PortsPower4,
	ePortsPowerGroup5_37_to_47  = B_PortsPower5
} Ports_Power_Group_e;


typedef enum
{
	eUDL_CounterGroup1_0_to_23  = B_UDLCounter1,
	eUDL_CounterGroup2_24_to_47 = B_UDLCounter2
} UDL_Counter_Group_e;


typedef enum
{
	ePortsClassGroup0_0_to_15 = 0,
	ePortsClassGroup1_16_to_31,
	ePortsClassGroup2_32_to_47
} Ports_Class_Group_e;


typedef enum
{
	eLatchesGroup1_0_to_23  = B_Latches,
	eLatchesGroup2_24_to_47 = B_Latches2
} Latches_Group_e;


typedef enum
{
	eDetCntGroup1_0_to_23  = B_DetCnt1,
	eDetCntGroup2_24_to_47 = B_DetCnt2
} Detection_Counter_Group_e;


typedef enum
{
	ePorts4PairPowerGroup1_0_to_10   = B_Ports4PairPower1,
	ePorts4PairPowerGroup2_11_to_21  = B_Ports4PairPower2,
	ePorts4PairPowerGroup3_22_to_25  = B_Ports4PairPower3,
	ePorts4PairPowerGroup4_26_to_36  = B_Ports4PairPower4,
	ePorts4PairPowerGroup5_37_to_47  = B_Ports4PairPower5
} Ports_4Pair_Power_Group_e;


typedef enum
{
	eNO_HardwareResetDetected_was_detected = 0,
	eHardwareResetDetected_PoE_controller_OK,
	eHardwareResetDetected_ERR_PoE_controller_error,
	eHardwareResetDetected_ERR_firmware_download_is_required,
	eHardwareResetDetected_ERR_PoE_controller_memory_error,
	eHardwareResetDetected_ERR_PoE_firmware_error
} Hardware_Reset_Detected_e;


typedef enum
{
	eClass_power_for_class_0 = 0,
	eClass_power_for_class_1,
	eClass_power_for_class_2,
	eClass_power_for_class_3,
	eClass_power_for_class_4_AF,
	eClass_power_for_class_4_AT
} Class_Type_e;


typedef enum
{
	ePPL = 0,
	eClass,
	eICut,
	ePPL_Class_Max,
	ePortPM2_NO_CHANGE = 0xF
} PortPM2_e;


typedef enum
{
	eNo_Condition,
	Condition_on_Classes_1_to_3,
	eCondition_on_Classes_0_to_3 ,
	eCondition_on_Classes_1_to_4,
	eCondition_on_Classes_0_to_4,
	ePortPM3_NO_CHANGE = 0xF
} PortPM3_e;

typedef enum
{
	ePM_algorithm_Dynamic_0 = 0,
	ePM_algorithm_TPPL,
	ePM_algorithm_NO_CHANGE = 78
} Sum_as_TPPL_PM_algorithm_e;

typedef enum
{
	eDevice_Method_I2C,
	eDevice_Method_UART_Sync,
	eDevice_Method_UART_Async,
	eDevice_Method_Queue,
} Device_Method_e;


typedef enum
{
	eCalcOpt_PD_Request_power_converted_to_TPPL = 0,
	eCalcOpt_TPPL_converted_to_PD_Request_power
} CalcOpt_e;


typedef enum
{
	ePortType_AF = 0,
	ePortType_AT
} PortType_e;


typedef enum
{
	eDeliver_2P = 0,
	eDeliver_4P
} Deliver4P_e;



typedef PoE_Status_e (*FPTR_Write)(U8 I2C_Address, const U8* Txdata, U16 num_write_length);
typedef PoE_Status_e (*FPTR_Read) (U8 I2C_Address, _OUT U8* Rxdata, U16 number_of_bytes_to_read, _OUT U16 *number_of_bytes_actually_read, READ_MODE_e eReadMode);



typedef struct
{
	U32 ulCommand_Received_Wrong_Checksum;
	U32 ulFailed_Execution_Conflict_in_Subject_Bytes;
	U32 ulFailed_Execution_Wrong_Data_Byte_Value;
	U32 ulFailed_Execution_Undefined_Key_Value;
	U32 ulUnknownError;
} PoE_Device_Errors_t;


typedef struct
{
	U32 ulChecksum;
	U32 ulCommunicationTimeout;
	U32 ulRxRetry;
	U32 ulEchoMismatch;
	PoE_Device_Errors_t tUnitErrors;
} Error_Counters_t;


typedef struct
{
	U32 ulSucceedMsgsCntr;
	Error_Counters_t tErrorCounters;
} Counters_t;


typedef struct
{
	POE_BOOL_e bPortTurnedOn             : 1; /* 0 */
	POE_BOOL_e bPortTurnedOff            : 1; /* 1 */
	POE_BOOL_e bDetectionUnsuccessful    : 1; /* 2 */
	POE_BOOL_e bPortFault                : 1; /* 3 */
	POE_BOOL_e bPortWasInUnderLoad       : 1; /* 4 */
	POE_BOOL_e bPortWasInOverLoad        : 1; /* 5 */
	POE_BOOL_e bPortWasInPM              : 1; /* 6 */
	POE_BOOL_e bPort_Power_Up_denied     : 1; /* 7 */
	POE_BOOL_e bDisconnectionTemperature : 1; /* 8 */
	POE_BOOL_e bUserDefinedTemperature   : 1; /* 9 */
	POE_BOOL_e bPoEDeviceFault           : 1; /* 10 */
	POE_BOOL_e bPoEDeviceSpareEvent      : 1; /* 11 */
	POE_BOOL_e bNoMoreConnect            : 1; /* 12 */
	POE_BOOL_e bVmainFault               : 1; /* 13 */
	POE_BOOL_e bSystemSpareEvent1        : 1; /* 14 */
	POE_BOOL_e bSystemSpareEvent2        : 1; /* 15 */
} Protocol_interrupt_register_t;




typedef struct
{
	POE_BOOL_e Underload_latch   : 1; /* 0   */
	POE_BOOL_e Overload_latch    : 1; /* 1   */
	POE_BOOL_e Force_On_current  : 1; /* 2   */
	U8 Underload_counter         : 2; /* 3-4 */
	POE_BOOL_e short_circuit     : 1; /* 5   */
	U8 detection_failure_counter : 2; /* 6-7 */
} Port_Latch_t;



typedef struct
{
	FPTR_Write      fptr_write;
	FPTR_Read       fptr_read;
	U16             wExtra_Timeout_mSec;
	Device_Method_e eDevice_Method ;
	POE_BOOL_e      ePOE_BOOL_Printf_Enable;
} Init_Info_t;




typedef struct
{
	Port_Priority_e     ePortPriority       : 2;
	U8                  byReserved          : 2;
	Power_Source_Type_e ePower_source_type  : 2;
	Power_Type_e        ePower_type         : 2;
} Get_Layer2_PD_Type_t;



typedef struct
{
	Port_Priority_e     ePortPriority    : 2;
	U8                  byReserved       : 2;
	Set_Power_Source_e  ePower_source    : 2;
	Set_PD_Power_Type_e ePD_Power_type   : 2;
} Set_Layer2_PD_Type_t;



typedef struct
{
	Port_Priority_e     ePortPriority    : 2;
	POE_BOOL_e          bExecute_CDP     : 1;
	POE_BOOL_e          bDisable_CDP     : 1;
	Set_Power_Source_e  ePower_source    : 2;
	Set_PD_Power_Type_e ePD_Power_type   : 2;
} Type_and_Exe_CDP_t;


typedef struct
{
	U8    bSoftwareVersion  : 7;
	U8    bRTL_Version      : 3;
	U8    bAnalogVersion    : 2;
	U8    bFamilyPrefix     : 4;
} Device_Version_t;


typedef struct
{
	POE_BOOL_e  Bit0_PortEnDis		: 1;
	POE_BOOL_e  Bit1_CapDis			: 1;
	POE_BOOL_e  Bit2_IcutDis		: 1;
} PortCfg_t;


typedef struct
{
	Sum_as_TPPL_PM_algorithm_e eBit0_PM_algorithm;
} Bit0_3_Sum_as_TPPL_user_configuration_t;

typedef struct
{
	Sum_as_TPPL_PM_algorithm_e eBit4_PM_algorithm;
} Bit4_7_Sum_as_TPPL_actual_configuration_t;


typedef struct
{
	Bit0_3_Sum_as_TPPL_user_configuration_t tBit0_3_user_configuration;
	Bit4_7_Sum_as_TPPL_actual_configuration_t tBit4_7_actual_configuration;
} Sum_as_TPPL_t;


typedef struct
{
	PortPM2_e eBit0_3_PortPM2_user_configuration;
	PortPM2_e eBit4_7_PortPM2_actual_configuration;
} PortPM2_t;



typedef struct
{
	PortPM3_e eBit0_3_PortPM3_user_configuration;
	PortPM3_e eBit4_7_PortPM3_actual_configuration;
} PortPM3_t;


typedef struct
{
	POE_BOOL_e  Bit0_Execute_LLDP	             : 1;
	POE_BOOL_e  Bit1_LLDP_functionality_Disabled : 1;
	POE_BOOL_e  Bit2_Reserve_Power_Mode	         : 1;
} Execute_LLDP_t;



/*================================================================================
/---- union of structures. Each structure contains the TX                  ------
/---- specific parameters which are required to send specific PoE Device command ------
/================================================================================*/

typedef struct
{
	Hardware_Reset_Detected_e eHardwareResetDetected;
	PoE_Status_e              eMscc_poe_status;

	union
	{
		/* ---------------------------------
		/ ---------- TX SET info ----------
		/ ---------------------------------*/

		struct
		{
			U8 PAD;
		} Save_System_Settings_t, Restore_Factory_Defaults_t;

		struct
		{
			struct
			{
				U8 bPrivate_Label;
			} Set;
		} Set_System_Status_t;

		struct
		{
			struct
			{
				U8 bUserByte;
			} Set;
		} Set_User_Byte_t;

		struct
		{
			struct
			{
				U8              bCS_Num;
				U8              bTSH_C;
			} Set;
		}
		Set_PoE_Device_Parameters_t;



		struct
		{
			struct
			{
				U16 wRegister_Address;
				U16 wRegister_Data;
				U8 	bDeviceNumber;
			} Set;
		}
		Set_PoE_Device_Register_t;


		struct
		{
			struct
			{
				U16 wRegister_Address;
				U8 	bDeviceNumber;
			} Set;

			struct
			{
				U16 wRegister_Data;
			} Get;
		}
		Get_PoE_Device_Register_t;



		struct
		{
			struct
			{
				U8  bBank;
				U32 ulPowerLimit_mW;
				U32 ulMaxShutdownVoltage_mV;
				U32 ulMinShutdownVoltage_mV;
				U8  bGuardBand;
			} Set;

		} Set_Power_Banks_t;

		struct
		{
			struct
			{
				PM1_System_Power_Calculated_Method_e ePM1_system_power_calculated_method;
				PM2_Port_Power_Limit_e               ePM2_Port_Power_Limit;
				PM3_Start_Up_Condition_e             ePM3_Start_up_conditions;
			} Set;
		} Set_PM_Method_t;


		struct
		{
			struct
			{
				U8 bIndicationOn_Percent;
				U8 bIndicationOff_Percent;
				U8 bIndicationOn_W;
				U8 bIndicationOff_W;
			} Set;
		} Set_Power_Indication_LED_t;


		struct
		{
			struct
			{
				U8 bCh_Num;
				U8 bPhysical_Number_A;
				U8 bPhysical_Number_B;
			} Set;
		} Set_Temporary_Matrix_t;

		struct
		{
			U8 PAD;
		} Program_Global_Matrix_t;

		struct
		{
			struct
			{
				U8 bCh_Num  ;    /* channel number (0-47) */
				U8 byPoeCmd ;   /* features enable/disable */
				Type_Mask_e eTYPE_MASK;
			} Set;
		} Set_Enable_Disable_Channels_t;

		struct
		{
			struct
			{
				U8  bCh_Num ;     /* channel number (0-47) */
				U32 ulPPL_mW;
			} Set;
		} Set_Power_Limit_for_Channels_t;

		struct
		{
			struct
			{
				U8  bCh_Num  ;   /* channel number (0-47) */
				U32 ulTPPL_mW;
			} Set;
		} Set_Temporary_Power_Limit_for_Channels_t;

		struct
		{
			struct
			{
				U8              bCh_Num ;         /* channel number (0-47) */
				Port_Priority_e ePortPriority;    /* Critical – 1; high – 2; low – 3 (default) */
			} Set;

		} Set_Port_Priority_t;

		struct
		{
			struct
			{
				U8              bCh_Num ;        /* channel number (0-47) */
				U8              byPoeCmd;        /* features enable/disable */
				U32             ulPPL_mW;
				Port_Priority_e ePortPriority;   /* Critical – 1; high – 2; low – 3 (default) */
			} Set;
		} Set_Port_Parameters_t;

		struct
		{
			struct
			{
				U8         bCh_Num         ;     /* channel number (0-47) */
				POE_BOOL_e ePOE_BOOL_EnDis ;     /*  1 - Force connection; 0 (default) - Normal operation */
			} Set;
		} Set_Force_Power_t;

		struct
		{
			struct
			{
				Protocol_interrupt_register_t tInterrutMaskRegister;
			} Set;

		} Set_Interrupt_Mask_t;

		struct
		{
			struct
			{
				Individual_Mask_e eINDV_MASK_OPT ;
				POE_BOOL_e        ePOE_BOOL_EnDis;
			} Set;
		} Set_Individual_Mask_t;

		struct
		{
			struct
			{
				Protocol_interrupt_register_t tBlink_Register;
				Protocol_interrupt_register_t tLight_Register;
			} Set;

		} Set_System_OK_LED_Mask_Registers_t;


		struct
		{
			struct
			{
				U8 bCh_Num ;      /* channel number (0-47) */
				Set_Layer2_PD_Type_t tSet_Layer2_PD_Type;
				U32 ulPD_Requested_Power_mW;
				U32 ulPSE_Allocated_Power_mW;
				U8  bCable_Length_meters;
				Execute_LLDP_t tExecute_LLDP;
			} Set;
		}
		Set_Port_Layer2_LLDP_PD_Data_t;

		struct
		{
			struct
			{
				U8                  bBank;
				Power_Source_Type_e eMscc_PSE_power_source;
			} Set;
		}
		Set_Power_Bank_Power_Source_Type_t;



		struct
		{
			struct
			{
				U8 bCh_Num ;         /* channel number (0-47) */
				U8 bit_0_PuP_CFG_EN;
				U8 PuP_CMD;
			} Set;
		}
		Set_Port_PowerUp_ALT_B_t;





		struct
		{
			struct
			{
				U8 bCh_Num ;      /* channel number (0-47) */
				Set_Layer2_PD_Type_t tSet_Layer2_PD_Type;
				U32 ulPD_Requested_Power0_mW;
				U32 ulPD_Requested_Power1_mW;
				U32 ulPD_Requested_Power2_mW;
				U32 ulPD_Requested_Power3_mW;
				U8  bCable_Length_meters;
				Execute_LLDP_t tExecute_LLDP;
			} Set;
		}
		Set_Port_Layer2_CDP_PD_Data_t;




		struct
		{
			struct
			{
				U8 bCh_Num ;      /* channel number (0-47) */
				Type_and_Exe_CDP_t tType_and_Exe_CDP;
				U32 ulPD_Requested_Power0_dW;
				U32 ulPD_Requested_Power1_dW;
				U32 ulPD_Requested_Power2_dW;
				U32 ulPD_Requested_Power3_dW;
				U32 ulPD_Requested_Power4_dW;
				U8  ulPw4;
				U8  ulPw3_to_0;
			} Set;
		}
		Set_Port_Layer2_CDP_PD_Data_2_t;




		struct
		{
			struct
			{
				U8  bCh_Num ;        /* channel number (0-47) */
				U32 ulPPL4Pair_mW;
			} Set;
		}
		Set_Power_Limits_for_4_Pair_Channels_t;

		struct
		{
			struct
			{
				U8  bCh_Num ;         /* channel number (0-47) */
				U32 ulTPPL4Pair_mW;
			} Set;
		}
		Set_Temporary_Power_Limits_for_4_Pair_Channels_t;

		struct
		{
			struct
			{
				U8              bCh_Num 		;	/* channel number (0-47) */
				U8              byPoeCmd        ;   /* features enable/disable */
				U32             ulPPL4Pair_mW	;
				Port_Priority_e ePortPriority	;   /* Critical – 1; high – 2; low – 3 (default) */
				Type_Mask_e 	ePort_Type 		;   /* 0 – AF - Only IEEE802.3af operation , 1 – AT operation , 2 – POH operation */
				Sum_as_TPPL_PM_algorithm_e  eSum_as_TPPL_PM_algorithm	;
				PortPM2_e 		ePortPM2		; 	/* 0 - Table set by the user (PPL), 1 - Class power Limit, ICUT Max (According to port behavior) */
				PortPM3_e       ePortPM3        ;
			} Set;
		}
		Set_4Pair_Port_Parameters_t;


		struct
		{
			struct
			{
				U8              bCh_Num 		;	/* channel number (0-47) */
			} Set;

			struct
			{
				PortCfg_t       tPortCfg	    ;
				U32             ulPPL4Pair_mW	;
				Port_Priority_e ePortPriority	;   /* Critical – 1; high – 2; low – 3 (default) */
				U8 				bPort_Type 		;   /* 0 – AF - Only IEEE802.3af operation , 1 – AT operation , 2 – POH operation */
				Sum_as_TPPL_t	tSum_as_TPPL	;
				PortPM2_t		tPortPM2		; 	/* 0 - Table set by the user (PPL), 1 - Class power Limit, ICUT Max (According to port behavior) */
				U32             ulTPPL4Pair_mW	;
				PortPM3_t		tPortPM3		;
			} Get;
		}
		Get_4Pair_Port_Parameters_t;


		struct
		{
			struct
			{
				U8  bBank ;
				U32 ulMax_PS_Power_mW;
				U32 ulTstart_mC;
				U32 ulTshutdown_mC;
				U32 ulDerating_Delta_Power_mW;
			} Set;
		}
		Set_Derating_Data_t;

		struct
		{
			struct
			{
				U32 ulTemperature_mC;
			} Set;
		}
		Set_Derating_User_Temperature_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;        /* channel number (0-47) */
				U8 byPoeCmd;        /* features enable/disable */
			} Set;
		}
		Set_Enable_Disable_4_Pair_for_Channels_t;


		struct
		{
			struct
			{
				U8  bHW_Version;
				U8  bProd;

				struct
				{
					U8 bMajor;
					U8 bMinor;
					U8 bPatch;
				} SW_VERSION_t;

				U8  bParamNumber;
				U8  bBuild_Num;       /* Critical – 1; high – 2; low – 3 (default) */
				U16 wInternalSWNumber;
			} Get;
		}
		Get_Software_Version_t;

		struct
		{
			struct
			{
				U8  bCS_Num ;
			} Set;

			struct
			{
				U8               bCS_Num ;
				Device_Version_t tDeviceVersion;
				Asic_Status_e    eAsicStatus;
				IC_Exp_e         eIC_Exp;
				U8               bIC_HW;
				U8               bIC_ports;
				S32              lTempr_mC;
				U32              ulTSH_mC;
			} Get;
		}
		Get_PoE_Device_Status_t;

		struct
		{
			struct
			{
				U8 bTelemetry_at_Boot_Up_Error;

				U8 poe_controller_error;
				U8 firmware_download_is_required;
				U8 poe_controller_memory_error;
				U8 devices_error;

				U8 bFactory_Default;
				U8 bGIE;
				U8 bPrivate_Label;
				U8 bUser_Byte;

				Device_Fail_e            eDevice_Fail[8];
				Temperature_Disconnect_e eTemperature_Disconnect[8];
				Temperature_Alarm_e      eTemperature_Alarm[8];

				Protocol_interrupt_register_t tInterrutMaskRegister;
			} Get;
		} Get_System_Status_t, Reset_Command_t;


		struct
		{
			struct
			{
				struct
				{
					POE_BOOL_e Bit_1_LVD	: 1 ;	/* bit 1 - Low Voltage Detect */
					POE_BOOL_e Bit_3_Lockup : 1 ; 	/* bit 3 - Unrecoverable exception */
					POE_BOOL_e Bit_4_ILOP 	: 1 ; 	/* bit 4 - Illegal Opcode */
					POE_BOOL_e Bit_5_COP 	: 1 ; 	/* bit 5 - Computer Operation Properly (COP) Watchdog */
					POE_BOOL_e Bit_6_PIN 	: 1 ; 	/* bit 6 - External Reset Pin */
					POE_BOOL_e Bit_7_POR 	: 1 ; 	/* bit 7 - Power On Reset */
				} SRS; /* System Reset Status */

				struct
				{
					POE_BOOL_e Bit_0_Clock_Loss_IRQ_occurred 		: 1;
					POE_BOOL_e Bit_1_UART_IRQ_occurred_at_I2C_mode 	: 1;
					POE_BOOL_e Bit_2_Unauthorized_IRQ_Occurred		: 1;
					POE_BOOL_e Bit_3_I2C_Arbitration_loss_occurred	: 1;
					POE_BOOL_e Bit_4_External_clock_recovery_failed	: 1; /*(Not supported MCU runs on Internal clock only)*/
					POE_BOOL_e Bit_5_POE_Vmain_out_of_range_flag	: 1;
					POE_BOOL_e Bit_6_CPU_voltage_warning_flag		: 1; /* (Not supported CPU can operate from 2v)*/
					POE_BOOL_e Bit_7_UART_error_IRQ_Occurred		: 1;
				} GIE1; /* General Internal Error 1 */

				struct
				{
					POE_BOOL_e Bit_0_communication_reset_command 				: 1;
					POE_BOOL_e Bit_1_clock_recovery_failure_for_more_than_5_sec : 1;
					POE_BOOL_e Bit_2_PoE_Device_failure							: 1;
					POE_BOOL_e Bit_3_I2C_module_was_restarted					: 1;
					POE_BOOL_e Bit_4_self_reset									: 1;
				} Reset_Info;

				U8 bSave_Command_counter;
			} Get;
		} Get_System_Status2_t;


		struct
		{
			struct
			{
				U32 ulPower_Consumption_mW;
				U32 ulMax_Shutdown_Voltage_mV;
				U32 ulMin_Shutdown_Voltage_mV;
				U8  bPower_Bank;
				U32 ulPower_Limit_mW;
			} Get;
		}
		Get_Power_Supply_Parameters_t;

		struct
		{
			struct
			{
				U32 ulVmain_Voltage_mV;
				U32 ulImain_Current_mA;
			} Get;
		}
		Get_Power_Supply_Voltage_t;

		struct
		{
			struct
			{
				U8  bBank;
			} Set;

			struct
			{
				U32                 ulPower_Budget_mW;
				U32                 ulMax_Shutdown_Voltage_mV;
				U32                 ulMin_Shutdown_Voltage_mV;
				U8                  bGuardBand;
				Power_Source_Type_e ePower_source_type;
			} Get;
		}
		Get_Power_Banks_t;

		struct
		{
			struct
			{
				Ports_Status_Group_e ePortsStatusGroup;
			} Set;

			struct
			{
				struct
				{
					U8 bPortNumber;
					Port_Status_e ePORT_STATUS;
					U8 bValid;
				} tPorts_Status[DEF_Get_All_Ports_Status_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_All_Ports_Status_t;

		struct
		{
			struct
			{
				Ports_Power_Group_e ePortsPowerGroup;
			} Set;

			struct
			{
				struct
				{
					U8  bValid;
					U8  bPortNumber;
					U32 ulPortPower_mW;
				} tPorts_Power[DEF_Get_All_Ports_Power_t_NUMBER_OF_ELEMENTS];

				struct
				{
					U8  bValid;
					U32 ulVmain_Voltage_mV;
					U32 ulPower_Consumption_mW;
					U32 ulMax_Power_Available_mW;
				} tAdditional_Info;
			} Get;
		}
		Get_All_Ports_Power_t;


		struct
		{
			struct
			{
				struct
				{
					U8 bPortNumber;
					U8 bPorts_EnDis;
				} tPorts_EnDis[MAX_CH_PER_SYSTEM];
			} Get;
		}
		Get_All_Ports_Enable_Disable_t;

		struct
		{
			struct
			{
				U8  bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				struct
				{
					POE_BOOL_e bit0_Port_Enabled  		 : 1;
					POE_BOOL_e bit1_Legacy_detection_Dis : 1;
					POE_BOOL_e bit2_Icut_Dis			 : 1;
				} EnDis;

				U8               bPort_Status;
				U8               bForce_PowerEn;
				Port_Latch_t     tPortLatch;
				Class_Number_e   eMscc_ClassNum;
				Operation_Mode_e eOperationMode;
				U8               b4PairEn;
			} Get;
		}
		Get_Single_Port_Status_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;   /* channel number (0-47) */
			} Set;

			struct
			{
				U32 ulPPL_mW;
				U32 ulTPPL_mW;
			} Get;
		}
		Get_Port_Power_Limit_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				Port_Priority_e ePortPriority;   /* 1 = Critical; 2 = High; 3 = Low */
			} Get;
		}
		Get_Port_Priority_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				U32 ulVmain_Voltage_mV;
				U32 ulCalculated_Current_mA;
				U32 ulPower_Consumption_mW;
				U32 ulPort_Voltage_mV;
			} Get;
		}
		Get_Port_Measurements_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;    /* channel number (0-47) */
			} Set;

			struct
			{
				U8 bPhysical_Number_A;
				U8 bPhysical_Number_B;
			} Get;
		}
		Get_Physical_Port_Number_from_Temporary_Matrix_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;    /* channel number (0-47) */
			} Set;

			struct
			{
				U8 bPhysical_Number_A;
				U8 bPhysical_Number_B;
			} Get;
		}
		Get_Physical_Port_Number_from_Active_Matrix_t;

		struct
		{
			struct
			{
				UDL_Counter_Group_e eCounterGroup;
			} Set;

			struct
			{
				struct
				{
					U8 bPortNumber;
					U8 bValue;
					U8 bValid;
				} tPorts_UDL_Counter[DEF_Get_UDL_Counters_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_UDL_Counters_t;

		struct
		{
			struct
			{
				PM1_System_Power_Calculated_Method_e ePM1_system_power_calculated_method;
				PM2_Port_Power_Limit_e               ePM2_Port_Power_Limit;
				PM3_Start_Up_Condition_e             ePM3_Start_up_conditions;
			} Get;
		}
		Get_PM_Method_t;

		struct
		{
			struct
			{
				U32 ulPower_Consumption_mW;
				U32 ulCalculated_Power_mW;
				U32 ulAvailable_Power_mW;
				U32 ulPower_Limit_mW;
				U8  bPower_Bank;
			} Get;
		}
		Get_Total_Power_t;

		struct
		{
			struct
			{
				Ports_Class_Group_e ePortsClassGroup;
			} Set;

			struct
			{
				U8 bGroup;

				struct
				{
					U8             bPortNumber;
					Class_Number_e eMscc_ClassNum;
					U8             bValid;
				} tPorts_Class[DEF_Get_All_Ports_Class_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_All_Ports_Class_t;


		struct
		{
			struct
			{
				U8 bPortsEnDis[MAX_CH_PER_SYSTEM];
			} Get;
		}
		Get_All_Ports_delivering_power_state_t;



		struct
		{
			struct
			{
				U8 bPortsEvent[MAX_CH_PER_SYSTEM];

				Protocol_interrupt_register_t tInterrutRegister;

				struct
				{
					POE_BOOL_e bit0_Vmain_is_in_range                : 1;
					POE_BOOL_e bit1_Over_power_indication_in_percent : 1;
					POE_BOOL_e bit2_Over_power_indication_in_watts   : 1;
				} tSystemOkReg;

			} Get;
		}
		Get_Port_Event_Cause_t;



		struct
		{
			struct
			{
				Latches_Group_e eLatchesGroup;
			} Set;

			struct
			{
				struct
				{
					U8 bPortNumber;
					U8 bUDL;
					U8 bOVL;
					U8 bValid;
				} tPorts_Latches[DEF_Get_Latches_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_Latches_t;

		struct
		{
			struct
			{
				Protocol_interrupt_register_t tInterrutMaskRegister;
			} Get;
		}
		Get_Interrupt_Mask_t;

		struct
		{
			struct
			{
				U16 wBlink_Register;
				U16 wLight_Register;

				Protocol_interrupt_register_t tBlink_Register;
				Protocol_interrupt_register_t tLight_Register;
			} Get;
		}
		Get_System_OK_LED_Mask_Registers_t;


		struct
		{
			struct
			{
				U8         bIndicationOn_Percent;
				U8         bIndicationOff_Percent;
				POE_BOOL_e eLED_Indication_Status;
				U8         bIndicationOn_W;
				U8         bIndicationOff_W;
			} Get;
		}
		Get_Power_Indication_LED_t;


		struct
		{
			struct
			{
				Detection_Counter_Group_e eDetCntGroup;
			} Set;

			struct
			{
				struct
				{
					U8 bPortNumber;
					U8 bValue;
					U8 bValid;
				} tPortsDetFailCounter[DEF_Get_Detection_Failure_Counters_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_Detection_Failure_Counters_t;

		struct
		{
			struct
			{
				Individual_Mask_e eINDV_MASK_OPT;
			} Set;

			struct
			{
				POE_BOOL_e ePOE_BOOL_EnDis  ;
			} Get;
		}
		Get_Individual_Mask_t;


		struct
		{
			struct
			{
				U8 bCh_Num ;   /* channel number (0-47) */
			} Set;

			struct
			{
				U32                   ulPSE_Allocated_Power_mW;
				U32                   ulPD_Requested_Power_mW;
				Get_Layer2_PD_Type_t  tLayer2_PD_Type;
				Power_Class_e         ePower_Class;
				PSE_Power_Pair_e      ePSE_Power_Pair;

				struct
				{
					POE_BOOL_e PSE                     : 1;
					POE_BOOL_e SUPPORT_PoE             : 1;
					POE_BOOL_e Port_EnDis              : 1;
					POE_BOOL_e PairsControlCapability  : 1;
					U8         spare                   : 4;
				} MDI_Power_Status_t;

				U8 bCable_Length;

				struct
				{
					U16                      wPort_power_consumption ;
					Layer2_LLDP_Port_State_e eLayer2_LLDP_Port_State ;
					Layer2_request_Pending_e eLayer2_request_Pending ;
					POE_BOOL_e               eReserved_power_mode_active;
				} PortIndicationAndPowerConsumption_t;

			} Get;
		}
		Get_Port_Layer2_LLDP_PSE_Data_t;





		struct
		{
			struct
			{
				U8 bCh_Num ;   /* channel number (0-47) */
			} Set;

			struct
			{
				POE_BOOL_e bit_0_PuP_CFG_EN : 1;
				POE_BOOL_e PuP_CMD          : 1;
			} Get;
		}
		Get_Port_PowerUp_ALT_B_t;




		struct
		{
			struct
			{
				U8 bCh_Num ;   /* channel number (0-47) */
			} Set;

			struct
			{
				U32                   ulPSE_Allocated_Power_mW;
				U32                   ulPD_Requested_Power_mW;
				Get_Layer2_PD_Type_t  tLayer2_PD_Type;
				Power_Class_e         ePower_Class;
				PSE_Power_Pair_e      ePSE_Power_Pair;

				struct
				{
					POE_BOOL_e PSE                     : 1;
					POE_BOOL_e SUPPORT_PoE             : 1;
					POE_BOOL_e Port_EnDis              : 1;
					POE_BOOL_e PairsControlCapability  : 1;
					U8 spare                           : 4;
				} MDI_Power_Status_t;

				U8 bCable_Length;

				struct
				{
					U16                      wPort_power_consumption ;
					Layer2_LLDP_Port_State_e eLayer2_LLDP_Port_State ;
					Layer2_request_Pending_e eLayer2_request_Pending ;
					POE_BOOL_e               eReserved_power_mode_active;
				} PortIndicationAndPowerConsumption_t;

			} Get;
		}
		Get_Port_Layer2_CDP_PSE_Data_t;



		struct
		{
			struct
			{
				U8          bCh_Num ;   /* channel number (0-??) */
				U16         PowerToConvert_dW;
				CalcOpt_e   eCalcOpt;
				PortType_e  ePortType;
				Deliver4P_e eDeliver4P;
				U8          bCable_Length_meters;

			} Set;

			struct
			{
				U16         PowerConvResult_mW;
				CalcOpt_e   eCalcOpt;
				U8          CalcErr;
			} Get;
		}
		Get_LLDP_Math_Conversion_t;



		struct
		{
			struct
			{
				Ports_4Pair_Power_Group_e ePorts4PairPowerGroup;
			} Set;

			struct
			{
				struct
				{
					U8  bValid;
					U8  bPortNumber;
					U32 ulPortPower_mW;
				} tPorts_Power[DEF_Get_All_4Pair_Ports_Power_t_NUMBER_OF_ELEMENTS];

				struct
				{
					U8  bValid;
					U32 ulVmain_Voltage_mV;
					U32 ulPower_Consumption_mW;
					U32 ulMax_Power_Available_mW;
				} tAdditional_Info;
			} Get;
		}
		Get_All_4Pair_Ports_Power_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;    /* channel number (0-47) */
			} Set;

			struct
			{
				U32 ulPPL_4Pair_mW;
				U32 ulTPPL_4Pair_mW;
			} Get;
		}
		Get_4Pair_Port_Power_Limit_t;

		struct
		{
			struct
			{
				U8 bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				U32 ulVmain_Voltage_mV;
				U32 ulCalculated_Current_mA;
				U32 ulPower_Consumption_4Pair_mW;
				U32 ulPort_Voltage_mV;
			} Get;
		}
		Get_4Pair_Port_measurements_t;

		struct
		{
			struct
			{
				U8 bBank;
			} Set;

			struct
			{
				U32 ulMax_PS_Power_mW;
				U32 ulTstart_mC;
				U32 ulTshutdown_mC;
				U32 ulDerating_Delta_Power_mW;
			} Get;
		}
		Get_Derating_Data_t;

		struct
		{
			struct
			{
				U8  bPower_Budget_Index;
				U32 ulPower_Budget_mW;
				U32 ulActual_Budget_mW;
				Temperature_Zone_e eTemperature_zone;
				U32 ulDerating_Temperature_mC;
			} Get;
		}
		Get_System_Measurements_t;


		struct
		{
			struct
			{
				U32 ulUser_Temperature_mC;
			} Get;
		}
		Get_Derating_User_Temperature_t;


		struct
		{
			struct
			{
				struct
				{
					U8 bPortNumber;
					U8 bEnDisValue;
				} tPorts_EnDis4Pair[MAX_CH_PER_SYSTEM];
			} Get;
		}
		Get_All_Ports_Enable_Disable_4pair_t;



		struct
		{
			struct
			{
				U8 bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				struct
				{
					U8 Port_standard_configuration_AF_AT_PoH : 2;  /* Bits 0:1 */
					POE_BOOL_e Port_force_power_enable       : 1;  /* Bit  2   */
					POE_BOOL_e Port_4pair_enable             : 1;  /* Bit  3   */
					POE_BOOL_e Port_Enabled                  : 1;  /* Bit  4   */
					POE_BOOL_e Legacy_detection_Disabled     : 1;  /* Bit  5   */
					POE_BOOL_e Icut_disabled                 : 1;  /* Bit  6   */
					POE_BOOL_e Reseved            	         : 1;
				} Defined_Port_Configuration_t;

				struct
				{
					U8 Port_standard_behavior_AF_AT_PoH      : 2;  /* Bits 0:1 */
					POE_BOOL_e Port_force_power_behavior     : 1;  /* Bit  2   */
					POE_BOOL_e Port_4pair_behavior           : 1;  /* Bit  3   */
					POE_BOOL_e Layer2_enabled_on_this_port   : 1;  /* Bit  4   */
					POE_BOOL_e Power_reserved_mode_active    : 1;  /* Bit  5   */
					U8 Reseved                               : 2;
				} Actual_Port_Configuration_t;

				Port_Status_e    ePortStatus;
				Class_Number_e   eMscc_ClassNum;

				U8 UDL_cnt;
				U8 OVL_cnt;
				U8 SC_cnt;
				U8 InvalidSigCnt;
				U8 PowerDeniedCnt;
			} Get;
		}
		Get_Extended_Port_Status_t;



		struct
		{
			struct
			{
				U8 bCh_Num ;     /* channel number (0-47) */
			} Set;

			struct
			{
				struct
				{
					POE_BOOL_e bit0_Port_Enabled  		: 1;
					POE_BOOL_e bit1_Legacy_detection	: 1;
					POE_BOOL_e bit2_Icut_Dis			: 1;
				} EnDis;


				U8 bPortInternalStatus;

				struct
				{
					POE_BOOL_e bit0_UDL_Latch          : 1;
					POE_BOOL_e bit1_OVL_Latch          : 1;
					POE_BOOL_e bit2_FORCE_ON_Latch     : 1;
					U8         BIT3_4_UDL_Counter      : 2;
					POE_BOOL_e bit5_SHORT_Latch        : 1;
					U8         BIT6_7_DET_FAIL_Counter : 2;
				} Latch;

				Class_Number_e   eMscc_ClassNum;

				U8 Port_standard_behavior_AF_AT_PoH    : 2;
			} Get;
		}
		Get_Internal_Port_Status_t;



		struct
		{
			struct
			{
				Counter_kind_e  eCounterKind;
				Counter_Group_e eCounterGroup;
			} Set;

			struct
			{
				struct
				{
					U8 bValid;
					U8 bPortNumber;
					U8 bValue;
				} tPort_Counter[DEF_Get_global_port_counters_t_NUMBER_OF_ELEMENTS];
			} Get;
		}
		Get_global_port_counters_t;


	} PoE_Msg_Prm_e;

} PoE_Msg_t;

#endif /* MSCC_POE_GLOBAL_TYPES_H_ */
