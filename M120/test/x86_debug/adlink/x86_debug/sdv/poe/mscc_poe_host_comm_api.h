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
*  Description: API for POE software
*
*
*************************************************************************/

#ifndef _MSCC_POE_API_H_
#define _MSCC_POE_API_H_

/*=========================================================================
   / INCLUDES
   /========================================================================*/

#include "mscc_poe_global_types.h"


/*=========================================================================
   / PROTOTYPES
   /========================================================================*/
PoE_Status_e MSCC_POE_Init(Init_Info_t *ptMscc_InitInfo);
PoE_Status_e MSCC_POE_Exit();
PoE_Status_e MSCC_POE_Msg(Protocol_15Bytes_Msg_Type_e eMessageType, _INOUT PoE_Msg_t *ptPoeMsg, U8 bIC_Address);
void         MSCC_POE_Get_Counters(_OUT Counters_t *ptCounters);
PoE_Status_e MSCC_POE_Clear_POE_Device_Buffer(U8 bIC_Address);
PoE_Status_e MSCC_POE_Firmware_Update(U8 bIC_Address, char S19FileName[]);


/***    microsemi communication protocol functions api    ***/

/**********************************     System messages      *****************************************/

/* 4.1.1 Reset Command */
TRANSMIT_MSG_STATUS_e Reset_Command(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.2 Restore Factory Default */
TRANSMIT_MSG_STATUS_e Restore_Factory_Defaults(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.3 Save System Settings */
TRANSMIT_MSG_STATUS_e Save_System_Settings(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.4 Set User Byte */
TRANSMIT_MSG_STATUS_e Set_User_Byte(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bUserByte);

/* 4.1.5 Set System Status */
TRANSMIT_MSG_STATUS_e Set_System_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bPrivate_Label);

/* 4.1.6 Get System Status */
TRANSMIT_MSG_STATUS_e Get_System_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.7 Get System Status2 */
TRANSMIT_MSG_STATUS_e Get_System_Status2(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.8 Set Interrupt Mask */
TRANSMIT_MSG_STATUS_e Set_Interrupt_Mask(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Protocol_interrupt_register_t tInterrutMaskRegister);

/* 4.1.9 Get Interrupt Mask */
TRANSMIT_MSG_STATUS_e Get_Interrupt_Mask(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.10 Set Individual Mask */
TRANSMIT_MSG_STATUS_e Set_Individual_Mask(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Individual_Mask_e eINDV_MASK_OPT,
    POE_BOOL_e ePOE_BOOL_EnDis);

/* 4.1.11 Get Individual Mask */
TRANSMIT_MSG_STATUS_e Get_Individual_Mask(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Individual_Mask_e eIndividual_Mask);

/* 4.1.12 Set System OK LED Mask Registers */
TRANSMIT_MSG_STATUS_e Set_System_OK_LED_Mask_Registers(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Protocol_interrupt_register_t tBlink_Register,
    Protocol_interrupt_register_t tLight_Register);

/* 4.1.13 Get System OK LED Mask Registers */
TRANSMIT_MSG_STATUS_e Get_System_OK_LED_Mask_Registers(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.14 Set Power Indication LED */
TRANSMIT_MSG_STATUS_e Set_Power_Indication_LED(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bIndicationOn_Percent,
    U8 bIndicationOff_Percent,
    U8 bIndicationOn_W,
    U8 bIndicationOff_W);

/* 4.1.15 Get Power Indication LED */
TRANSMIT_MSG_STATUS_e Get_Power_Indication_LED(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.1.16 Set System Masks - obsolete - please use 'Set_Individual_Mask' instead */

/* 4.1.17 Get Masks Status - obsolete - please use 'Get_Individual_Mask' instead */


/* 4.1.18 Get Software Version */
TRANSMIT_MSG_STATUS_e Get_Software_Version(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/**********************************     4.2 PoE Device messages      *****************************************/

/* 4.2.1 Set PoE Device Parameters */
TRANSMIT_MSG_STATUS_e Set_PoE_Device_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCS_Num,
    U8 bTSH_C);

/* 4.2.2 Get PoE Device Status */
TRANSMIT_MSG_STATUS_e Get_PoE_Device_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCS_Num);

/* 4.2.3 Set PoE Device Register */
TRANSMIT_MSG_STATUS_e Set_PoE_Device_Register(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U16 wRegister_Address,
    U16 wRegister_Data,
    U8 bDeviceNumber);

/* 4.2.4 Get PoE Device Register */
TRANSMIT_MSG_STATUS_e Get_PoE_Device_Register(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U16 wRegister_Address,
    U8 bDeviceNumber);




/**********************************     4.3 PoE Port messages      *****************************************/

/* 4.3.1 Set Temporary Matrix */
TRANSMIT_MSG_STATUS_e Set_Temporary_Matrix(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U8 bPhysical_Number_A,
    U8 bPhysical_Number_B);

/* 4.3.2 Get Physical Port Number from Temporary Matrix */
TRANSMIT_MSG_STATUS_e Get_Physical_Port_Number_from_Temporary_Matrix(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.3 Program Global Matrix */
TRANSMIT_MSG_STATUS_e Program_Global_Matrix(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.3.4 Get Physical Port Number from Active Matrix */
TRANSMIT_MSG_STATUS_e Get_Physical_Port_Number_from_Active_Matrix(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.5 Set Enable/Disable Channels */
TRANSMIT_MSG_STATUS_e Set_Enable_Disable_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis,
    Type_Mask_e eTYPE_MASK);

/* 4.3.6 Get All Ports Enable/Disable */
TRANSMIT_MSG_STATUS_e Get_All_Ports_Enable_Disable(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.3.7 Set Force Power */
TRANSMIT_MSG_STATUS_e Set_Force_Power(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    POE_BOOL_e ePOE_BOOL_EnDis);

/* 4.3.8 Set Port Parameters */
TRANSMIT_MSG_STATUS_e Set_Port_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis,
    U32 ulPPL_mW,
    Port_Priority_e ePortPriority);

/* 4.3.9 Set Power Limit for Channels */
TRANSMIT_MSG_STATUS_e Set_Power_Limit_for_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U32 ulPPL_mW);

/* 4.3.10 Set Temporary Power Limit for Channels */
TRANSMIT_MSG_STATUS_e Set_Temporary_Power_Limit_for_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U32 ulTPPL_mW);

/* 4.3.11 Get Port Power Limit */
TRANSMIT_MSG_STATUS_e Get_Port_Power_Limit(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.12 Set 4-Pair Port Parameters */
TRANSMIT_MSG_STATUS_e Set_4Pair_Port_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis,
    U32 ulPPL4Pair_mW,
    Port_Priority_e ePortPriority,
    Type_Mask_e ePort_Type,
    Sum_as_TPPL_PM_algorithm_e eSum_as_TPPL_PM_algorithm,
    PortPM2_e ePortPM2,
    PortPM3_e ePortPM3);

/* 4.3.13 Get 4-Pair Port Parameters */
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.14 Set Power Limits for 4-Pair Channels */
TRANSMIT_MSG_STATUS_e Set_Power_Limits_for_4_Pair_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U32 ulPPL4Pair_mW);

/* 4.3.15 Set Temporary Power Limits for 4-Pair Channels */
TRANSMIT_MSG_STATUS_e Set_Temporary_Power_Limits_for_4_Pair_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U32 ulTPPL4Pair_mW);

/* 4.3.16 Get 4-Pair Port Power Limit */
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_Power_Limit(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.17 Set Port Priority */
TRANSMIT_MSG_STATUS_e Set_Port_Priority(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Port_Priority_e ePortPriority);

/* 4.3.18 Get Port Priority */
TRANSMIT_MSG_STATUS_e Get_Port_Priority(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.19 Set Enable/Disable 4-Pair for Channels */
TRANSMIT_MSG_STATUS_e Set_Enable_Disable_4_Pair_for_Channels(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Feature_EnDis_Type_e eFeature_EnDis_Type,
    POE_BOOL_e ePOE_BOOL_Feature_EnDis);

/* 4.3.20 Get All Ports Enable/Disable 4-Pair Mode */
TRANSMIT_MSG_STATUS_e Get_All_Ports_Enable_Disable_4pair(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.3.21 Get Single Port Status */
TRANSMIT_MSG_STATUS_e Get_Single_Port_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.22 Get All Ports Status */
TRANSMIT_MSG_STATUS_e Get_All_Ports_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Ports_Status_Group_e ePortsStatusGroup);

/* 4.3.23 Get Extended Port Status */
TRANSMIT_MSG_STATUS_e Get_Extended_Port_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.24	Get Internal Port Status (For Debug usage) */
TRANSMIT_MSG_STATUS_e Get_Internal_Port_Status(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.3.25	Get UDL Counters */
TRANSMIT_MSG_STATUS_e Get_UDL_Counters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    UDL_Counter_Group_e eUDL_CounterGroup);

/* 4.3.26	Get Detection Failure Counters */
TRANSMIT_MSG_STATUS_e Get_Detection_Failure_Counters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Detection_Counter_Group_e eDetCntGroup);

/* 4.3.27	Get Latches (Old Request to support backwards compatibility) */
TRANSMIT_MSG_STATUS_e Get_Latches(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Latches_Group_e eLatchesGroup);

/* 4.3.28	Get Global Port Counters */
TRANSMIT_MSG_STATUS_e Get_global_port_counters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Counter_kind_e eCounter_kind,
    Counter_Group_e eCounter_Group);

/* 4.3.29	Get All Ports Class */
TRANSMIT_MSG_STATUS_e Get_All_Ports_Class(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Ports_Class_Group_e ePortsClassGroup);

/* 4.3.30	Get All Ports delivering power state */
TRANSMIT_MSG_STATUS_e Get_All_Ports_delivering_power_state(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/*4.3.31	Get Port Event Cause */
TRANSMIT_MSG_STATUS_e Get_Port_Event_Cause(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/**********************************     4.4	Power Management Related Messages      *****************************************/

/* 4.4.1	Set PM Method */
TRANSMIT_MSG_STATUS_e Set_PM_Method(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    PM1_System_Power_Calculated_Method_e ePM1_system_power_calculated_method,
    PM2_Port_Power_Limit_e ePM2_Port_Power_Limit,
    PM3_Start_Up_Condition_e ePM3_Start_up_conditions);

/* 4.4.2	Get PM Method */
TRANSMIT_MSG_STATUS_e Get_PM_Method(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.4.3	Get Total Power */
TRANSMIT_MSG_STATUS_e Get_Total_Power(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.4.4	Set Power Banks */
TRANSMIT_MSG_STATUS_e Set_Power_Banks(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bBank, U32 ulPowerLimit_mW,
    U32 ulMaxShutdownVoltage_mV,
    U32 ulMinShutdownVoltage_mV,
    U8 bGuardBand);

/* 4.4.5	Get Power Banks */
TRANSMIT_MSG_STATUS_e Get_Power_Banks(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bBank);

/* 4.4.6	Get Power Supply Parameters */
TRANSMIT_MSG_STATUS_e Get_Power_Supply_Parameters(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.4.7	Get Power Supply Voltage */
TRANSMIT_MSG_STATUS_e Get_Power_Supply_Voltage(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.4.8	Get All Ports Power */
TRANSMIT_MSG_STATUS_e Get_All_Ports_Power(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Ports_Power_Group_e ePortsPowerGroup);

/* 4.4.9	Get All 4-Pair Ports Power */
TRANSMIT_MSG_STATUS_e Get_All_4Pair_Ports_Power(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    Ports_4Pair_Power_Group_e ePorts4PairPowerGroup);

/* 4.4.10	Get All HIP Ports Power - obsolete - Please use the "Get All 4-Pair Ports Power " instead */

/* 4.4.11	Get Port Measurements */
TRANSMIT_MSG_STATUS_e Get_Port_Measurements(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.4.12	Get 4-Pair Port Measurements */
TRANSMIT_MSG_STATUS_e Get_4Pair_Port_measurements(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/**********************************     4.5	Power Derating, Related Messages      *****************************************/

/* 4.5.1	Set Derating Data */
TRANSMIT_MSG_STATUS_e Set_Derating_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bBank,
    U32 ulMax_PS_Power_mW,
    U32 ulTstart_mC,
    U32 ulTshutdown_mC,
    U32 ulDerating_Delta_Power_mW);

/* 4.5.2	Get Derating Data */
TRANSMIT_MSG_STATUS_e Get_Derating_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bBank);

/* 4.5.3	Set Derating User Temperature */
TRANSMIT_MSG_STATUS_e Set_Derating_User_Temperature(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U32 ulTemperature_mC);

/* 4.5.4	Get Derating User Temperature */
TRANSMIT_MSG_STATUS_e Get_Derating_User_Temperature(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/* 4.5.5	Get System Measurements */
TRANSMIT_MSG_STATUS_e Get_System_Measurements(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address);

/**********************************     4.6	Laye2 Related Messages      *****************************************/

/* 4.6.1	Set Port Layer2 LLDP PD Data */
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_LLDP_PD_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Set_Layer2_PD_Type_t tSet_Layer2_PD_Type,
    U32 ulPD_Requested_Power_mW,
    U32 ulPSE_Allocated_Power_mW,
    U8 bCable_Length_meters,
    Execute_LLDP_t tExecute_LLDP);

/* 4.6.2	Get Port Layer2 LLDP PSE Data */
TRANSMIT_MSG_STATUS_e Get_Port_Layer2_LLDP_PSE_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.6.3	Set Power Bank Power Source Type */
TRANSMIT_MSG_STATUS_e Set_Power_Bank_Power_Source_Type(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bBank,
    Power_Source_Type_e eMscc_PSE_power_source);

/* 4.6.4	Set Port PowerUp ALT-B */
TRANSMIT_MSG_STATUS_e Set_Port_PowerUp_ALT_B(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U8 bit_0_PuP_CFG_EN,
    U8 PuP_CMD);

/* 4.6.5	Get Port PowerUp ALT-B */
TRANSMIT_MSG_STATUS_e Get_Port_PowerUp_ALT_B(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);

/* 4.6.6	Set Port Layer2 CDP PD Data */
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_CDP_PD_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Set_Layer2_PD_Type_t tSet_Layer2_PD_Type,
    U32 ulPD_Requested_Power0_mW,
    U32 ulPD_Requested_Power1_mW,
    U32 ulPD_Requested_Power2_mW,
    U32 ulPD_Requested_Power3_mW,
    U8 bCable_Length_meters,
    Execute_LLDP_t tExecute_LLDP);

/* 4.6.7	Get Port Layer2 CDP PSE Data */
TRANSMIT_MSG_STATUS_e Get_Port_Layer2_CDP_PSE_Data(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num);


/* 4.6.8	Set Port Layer2 CDP PD Data 2 */
TRANSMIT_MSG_STATUS_e Set_Port_Layer2_CDP_PD_Data_2(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    Type_and_Exe_CDP_t tType_and_Exe_CDP,
    U32 ulPD_Requested_Power0_mW,
    U32 ulPD_Requested_Power1_mW,
    U32 ulPD_Requested_Power2_mW,
    U32 ulPD_Requested_Power3_mW,
    U32 ulPD_Requested_Power4_mW);


/* 4.6.9	Get LLDP Math Conversion */
TRANSMIT_MSG_STATUS_e Get_LLDP_Math_Conversion(
    _INOUT PoE_Msg_t *ptPoeMsg,
    U8 bI2C_Device_Address,
    U8 bCh_Num,
    U16 PowerToConvert_dW,
    CalcOpt_e eCalcOpt,
    PortType_e ePortType,
    Deliver4P_e eDeliver4P,
    U8 bCable_Length_meters);


/*********************************************************************************************************************/

TRANSMIT_MSG_STATUS_e TransmitMsg(char *pcFunctionName,
                                  Protocol_15Bytes_Msg_Type_e eMsgType,
                                  _INOUT PoE_Msg_t *ptPoeMsg,
                                  U8 bI2C_Device_Address);

#endif /* _MSCC_POE_API_H_ */

