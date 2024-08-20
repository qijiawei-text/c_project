/******************************************************************************
*
* Copyright(C), 2020, OPLINK Tech.Co.,Ltd
*
* Filename:      util_cali.c
* Description:   Optical cali
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                fandy      2020/12/02  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_CALI_H__
#define __UTIL_CALI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WSS_CALI_ROW_MAX_CNT   96  /* row channel */
#define WSS_CALI_COL_MAX_CNT   33  /* column 0:freq, 1~32:port */

typedef struct
{
    double aadWssRawData[WSS_CALI_ROW_MAX_CNT][WSS_CALI_COL_MAX_CNT];
} ST_WSS_CALI;


typedef enum
{
    /*
       EN_K_VAL_OUT65,
       EN_K_VAL_OUT66,
       EN_K_VAL_DROP1,
       EN_K_VAL_DROP2,
       EN_K_VAL_ADD1,
       EN_K_VAL_ADD2,
     */

    EN_K_VAL_LINEIN,
    EN_K_VAL_LINEOUT,
    EN_K_VAL_LINEINP,
    EN_K_VAL_LINEOUTP,
    EN_K_VAL_COMIN,
    EN_K_VAL_COMOUT,

    EN_K_VAL_CNT


}EN_K_VAL_PORT;




typedef struct
{
    double aadKVal[EN_K_VAL_CNT][4];
    double adWssLoss[EN_K_VAL_CNT];
} ST_K_VAL;

#define COM_K_VAL_DATA            "/home/oplink/datastore1/COM_K_VAL_DATA.csv"

#define WSS1_DROP_T0_CALI_DATA    "/home/oplink/datastore1/WSS1_DROP_T0_CALI_DATA.csv"
#define WSS1_DROP_T30_CALI_DATA   "/home/oplink/datastore1/WSS1_DROP_T30_CALI_DATA.csv"
#define WSS1_DROP_T65_CALI_DATA   "/home/oplink/datastore1/WSS1_DROP_T65_CALI_DATA.csv"
#define WSS2_DROP_T0_CALI_DATA    "/home/oplink/datastore1/WSS2_DROP_T0_CALI_DATA.csv"
#define WSS2_DROP_T30_CALI_DATA   "/home/oplink/datastore1/WSS2_DROP_T30_CALI_DATA.csv"
#define WSS2_DROP_T65_CALI_DATA   "/home/oplink/datastore1/WSS2_DROP_T65_CALI_DATA.csv"

#define WSS1_ADD_T0_CALI_DATA     "/home/oplink/datastore1/WSS1_ADD_T0_CALI_DATA.csv"
#define WSS1_ADD_T30_CALI_DATA    "/home/oplink/datastore1/WSS1_ADD_T30_CALI_DATA.csv"
#define WSS1_ADD_T65_CALI_DATA    "/home/oplink/datastore1/WSS1_ADD_T65_CALI_DATA.csv"
#define WSS2_ADD_T0_CALI_DATA     "/home/oplink/datastore1/WSS2_ADD_T0_CALI_DATA.csv"
#define WSS2_ADD_T30_CALI_DATA    "/home/oplink/datastore1/WSS2_ADD_T30_CALI_DATA.csv"
#define WSS2_ADD_T65_CALI_DATA    "/home/oplink/datastore1/WSS2_ADD_T65_CALI_DATA.csv"

double UTIL_GetComPortCali(EN_K_VAL_PORT enPort, double dStartFreq, double dEndFreq);
double UTIL_GetDropPortCali(int iPort, double dStartFreq, double dEndFreq, double dTemperature);
double UTIL_GetDropPortCaliNoTab(int iPort, double dStartFreq, double dEndFreq, double dTemperature);
double UTIL_GetAddPortCali(int iPort, double dStartFreq, double dEndFreq, double dTemperature);
double UTIL_GetTotalPower(double *pdDbm, int iLen);
double UTIL_GetChanPowerByRawdata(int16_t *psSliceRawData, uint32_t uiSliceCnt, double dLosThr);

/**************************************************************************************************/
/* definition for OCM DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration              */
/**************************************************************************************************/
/* @ { */
#define DEF_OFS_FGA_VAL_DATA            "/home/oplink/datastore1/OCM_FGA_DATA.csv"
#define DEF_OCM_DROP1COM1_TO_FGA1       "OCM_DROP1COM1_TO_FGA1"
#define DEF_OCM_DROP2COM1_TO_FGA2       "OCM_DROP2COM1_TO_FGA2"
typedef struct {
    double dFga1;
    double dFga2;
} ST_OFS_FGA;
/* @ } */

/**************************************************************************************************/
/* definition for SFP port tx/rx calibration                                                      */
/**************************************************************************************************/
/* @ { */
#define DEF_SFP_WSS_PANEL_VAL_DATA      "/home/oplink/datastore1/SFP_WSS_PANEL_CALI_DATA.csv"
#define DEF_SFP_WSS1_ADD_PANEL          "SFP_WSS1_ADD_PANEL"
#define DEF_SFP_WSS1_DROP_PANEL         "SFP_WSS1_DROP_PANEL"
#define DEF_SFP_WSS2_ADD_PANEL          "SFP_WSS2_ADD_PANEL"
#define DEF_SFP_WSS2_DROP_PANEL         "SFP_WSS2_DROP_PANEL"

typedef struct {
    double dWss1Add;
    double dWss1Drop;
    double dWss2Add;
    double dWss2Drop;
} ST_SFP_WSS_PANEL_CALI;

typedef enum {
    EN_CALI_SFP_DIR_TX = 0,
    EN_CALI_SFP_DIR_RX = 1,
    EN_CALI_SFP_DIR_MAX
} EN_CALI_SFP_DIR;

#define DEF_CALI_WSS1_PORT_MIN      (1)
#define DEF_CALI_WSS1_PORT_MAX      (32)
#define DEF_CALI_WSS2_PORT_MIN      (33)
#define DEF_CALI_WSS2_PORT_MAX      (64)

#define DEF_SFP_NO_POWER            (-60.00)

/* The calibration frequency is 191200 GHZ, but there is no calibration data in database */
/* just use the nearly frequency */
#define DEF_CALI_SFP_FREQ               (191250)

/***FUNC+******************************************************************************************/
/* Name   : Util_CaliCalcSfpPower                                                                 */
/* Descrp : Cali the sfp panel power by calibration                                               */
/* Input  : dPower -- the sfp module tx/rx power                                                  */
/* Input  : ucDir  -- the sfp power is TX or RX(refer to EN_CALI_SFP_DIR)                         */
/* Input  : iWssPortIdx -- the wss port idx (from 1 to 64)                                        */
/* Input  : dAttn -- the WSS attn                                                                 */
/* Input  : dTemperature -- system temperature                                                    */
/* Output : NONE                                                                                  */
/* Return : the panel sfp power. If the calibrate panel sfp power <= -60dBm, just return -60dBm   */
/***FUNC-******************************************************************************************/
double Util_CaliCalcSfpPower(double dPower, uint8_t ucDir, int32_t iWssPortIdx, double dAttn, double dTemperature);

/***FUNC+******************************************************************************************/
/* Name   : UTIL_GetSfpWssPanelCali                                                               */
/* Descrp : get the sfp panel wss calibration data                                                */
/* Input  : iIndex -- wss index                                                                   */
/* Output : NONE                                                                                  */
/* Return : the panel sfp calibrate data                                                          */
/***FUNC-******************************************************************************************/
double UTIL_GetSfpWssPanelCali(int iIndex);

#ifdef __cplusplus
}
#endif

#endif

