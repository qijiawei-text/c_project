/***MODU+*******************************************************************************************
   Copyright(C), 2022, Molex Tech.Co.,Ltd
 * FileName    : dsc_bgp.h
 * Description : Define the bgp control functions.
 * History     :
 *     [Author]      [Date]          [Version]        [Description]
 * [1] Chun Ye       2022/09/29      Ver 1.0.0        Initial file.
 ***MODU-******************************************************************************************/

#ifndef _DSC_CONNECTIVITY_H_
#define _DSC_CONNECTIVITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

/**************************************************************************************************/
/* definition of reflection PD state and config                                                   */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    bool bOnline;
    double dDifferPower;    /* dOutPower - dRefPower                                   */
    double dOutputPower;    /* PDb                                                     */
    double dRefPower;       /* PDa                                                     */
} ST_REFLECT_STATE;

typedef struct {
    double dDifferHighThr;
    double dDifferLowThr;
} ST_REFLECT_CONFIG;
/* } @                                                                                            */


/**************************************************************************************************/
/* definition of mux PD state and config                                                          */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    bool bOnline;
    double dOutputPowerR;
    double dInputPowerR;
    double dDifferPowerR;
    double dOutputPowerT;
    double dInputPowerT;
    double dDifferPowerT;
} ST_MUX_STATE;

typedef struct {
    double dDifferHighThrR;
    double dDifferLowThrR;
    double dDifferHighThrT;
    double dDifferLowThrT;
} ST_MUX_CONFIG;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of mux client state and config                                                      */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    bool bOnline;
    double dDfbOutputPower;
    double dOutputPowerR;
    double dInputPowerR;
    double dDifferPowerR;
    double dOutputPowerT;
    double dInputPowerT;
    double dDifferPowerT;
} ST_CLIENT_STATE;

typedef struct {
    double dDifferHighThrR;
    double dDifferLowThrR;
    double dDifferHighThrT;
    double dDifferLowThrT;
} ST_CLIENT_CONFIG;
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of connectivity validation state and config                                         */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_CLIENT_W = 0,
    EN_CLIENT_P = 1,
    EN_CLIENT_MAX = 2
} EN_CLIENT;

typedef enum {
    EN_REFLECT_MUX = 0,
    EN_REFLECT_LINE_T = 1,
    EN_REFLECT_CLIENT_T = 2,
    EN_REFLECT_LINE_T_P = 3,
    EN_REFLECT_CLIENT_T_P = 4,
    EN_REFLECT_MAX
} EN_REFLECT;

typedef enum {
    EN_DIFFER_REF = 0,
    EN_DIFFER_MUX_R = 1,
    EN_DIFFER_MUX_T = 2,
    EN_DIFFER_CLIENT_W_R = 3,
    EN_DIFFER_CLIENT_W_T = 4,
    EN_DIFFER_CLIENT_P_R = 5,
    EN_DIFFER_CLIENT_P_T = 6,

    EN_DIFFER_MAX
} EN_DIFFER;

#define DEF_CV_DIFF_THR_IVALID_VAL     (-99.99)

typedef struct {
    double dHighThr;
    double dLowThr;
} ST_DIFFER_THR;

typedef struct {
    ST_REFLECT_STATE astReflect[EN_REFLECT_MAX];
    ST_MUX_STATE stMux;
    ST_CLIENT_STATE astClient[EN_CLIENT_MAX];
} ST_CV_STATE;

typedef struct {
    ST_DIFFER_THR astThr[EN_DIFFER_MAX];
} ST_CV_CONFIG;
/* } @                                                                                            */


/**************************************************************************************************/
/* definition of connectivity validation state Xpath                                              */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_MODULE_NS_CV              "com-oplink-connectivity-validation"
#define DEF_CV_XPATH_HEADER           COO_XPATH_PREFIX "/optical-modules/com-oplink-connectivity-validation:optical-cv"
#define DEF_CV_I_XPATH_HEADER         COO_I_XPATH_PREFIX "/optical-modules/com-oplink-connectivity-validation:optical-cv"

#define DEF_CV_XPATH_STATE_HEADER    DEF_CV_XPATH_HEADER "/state"
#define DEF_CV_XPATH_STATE_ALL       DEF_CV_XPATH_STATE_HEADER "//*"

#define DEF_CV_LEAF_ATTR_NUM         (6 + (2 * 7) + (3 * 5))
/* } @                                                                                            */


/**************************************************************************************************/
/* definition of reflection                                                                       */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_CV_XPATH_REFLECT_HEADER  DEF_CV_XPATH_HEADER "/state/reflection[name='%s']"

#define DEF_CV_REF_MUX              "mux"
#define DEF_CV_REF_LINE_T           "line-t"
#define DEF_CV_REF_CLIENT_T         "client-t"
#define DEF_CV_REF_LINE_T_P         "line-t-p"
#define DEF_CV_REF_CLIENT_T_P       "client-t-p"

#define DEF_CV_LEAF_REF_DIFFER_POWER "differ-power"
#define DEF_CV_LEAF_REF_OUTPUT_POWER "output-power"
#define DEF_CV_LEAF_REF_REF_POWER    "reflect-power"

#define DEF_CV_XPATH_REF_DIFFER_POWER DEF_CV_XPATH_REFLECT_HEADER "/" DEF_CV_LEAF_REF_DIFFER_POWER
#define DEF_CV_XPATH_REF_OUTPUT_POWER DEF_CV_XPATH_REFLECT_HEADER "/" DEF_CV_LEAF_REF_OUTPUT_POWER
#define DEF_CV_XPATH_REF_REF_POWER    DEF_CV_XPATH_REFLECT_HEADER "/" DEF_CV_LEAF_REF_REF_POWER

/* } @                                                                                            */

/**************************************************************************************************/
/* definition of mux connectivity                                                                 */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_CV_XPATH_MUX_HEADER      DEF_CV_XPATH_HEADER "/state/mux-connectivity"

#define DEF_CV_LEAF_MUX_R_OUT_POWER     "mux-r-out-power"
#define DEF_CV_LEAF_MUX_R_IN_POWER      "mux-r-in-power"
#define DEF_CV_LEAF_MUX_R_DIFFER_POWER  "mux-r-differ-power"
#define DEF_CV_LEAF_MUX_T_OUT_POWER     "mux-t-out-power"
#define DEF_CV_LEAF_MUX_T_IN_POWER      "mux-t-in-power"
#define DEF_CV_LEAF_MUX_T_DIFFER_POWER  "mux-t-differ-power"

#define DEF_CV_XPATH_MUX_R_OUT_POWER    DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_R_OUT_POWER
#define DEF_CV_XPATH_MUX_R_IN_POWER     DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_R_IN_POWER
#define DEF_CV_XPATH_MUX_R_DIFFER_POWER DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_R_DIFFER_POWER
#define DEF_CV_XPATH_MUX_T_OUT_POWER    DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_T_OUT_POWER
#define DEF_CV_XPATH_MUX_T_IN_POWER     DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_T_IN_POWER
#define DEF_CV_XPATH_MUX_T_DIFFER_POWER DEF_CV_XPATH_MUX_HEADER "/" DEF_CV_LEAF_MUX_T_DIFFER_POWER
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of mux connectivity                                                                 */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_CV_XPATH_CLIENT_HEADER      DEF_CV_XPATH_HEADER "/state/client-connectivity[name='%s']"

#define DEF_CV_CLIENT_NAME_CLIENT_W        "client-w"
#define DEF_CV_CLIENT_NAME_CLIENT_P        "client-p"

#define DEF_CV_LEAF_CLIENT_NAME            "name"
#define DEF_CV_LEAF_CLIENT_DFB_POWER       "dfb-output-power"
#define DEF_CV_LEAF_CLIENT_R_OUT_POWER     "client-r-out-power"
#define DEF_CV_LEAF_CLIENT_R_IN_POWER      "client-r-in-power"
#define DEF_CV_LEAF_CLIENT_R_DIFFER_POWER  "client-r-differ-power"
#define DEF_CV_LEAF_CLIENT_T_OUT_POWER     "client-t-out-power"
#define DEF_CV_LEAF_CLIENT_T_IN_POWER      "client-t-in-power"
#define DEF_CV_LEAF_CLIENT_T_DIFFER_POWER  "client-t-differ-power"

#define DEF_CV_XPATH_CLIENT_DFB_POWER      DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_DFB_POWER
#define DEF_CV_XPATH_CLIENT_R_OUT_POWER    DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_R_OUT_POWER
#define DEF_CV_XPATH_CLIENT_R_IN_POWER     DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_R_IN_POWER
#define DEF_CV_XPATH_CLIENT_R_DIFFER_POWER DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_R_DIFFER_POWER
#define DEF_CV_XPATH_CLIENT_T_OUT_POWER    DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_T_OUT_POWER
#define DEF_CV_XPATH_CLIENT_T_IN_POWER     DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_T_IN_POWER
#define DEF_CV_XPATH_CLIENT_T_DIFFER_POWER DEF_CV_XPATH_CLIENT_HEADER "/" DEF_CV_LEAF_CLIENT_T_DIFFER_POWER
/* } @                                                                                            */

/**************************************************************************************************/
/* definition of connection validation rpc Xpath                                                  */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_CV_RPC_IDX_ALL = 0,
    EN_CV_RPC_IDX_REFLECT = 1,
    EN_CV_RPC_IDX_MUX = 2,
    EN_CV_RPC_IDX_CLIENT_W = 3,
    EN_CV_RPC_IDX_CLIENT_P = 4,
    EN_CV_RPC_IDX_MAX
} EN_CV_RPC_IDX;

#define DEF_CV_RPC_NAME_ALL        "all"
#define DEF_CV_RPC_NAME_REFLECT    "reflection"
#define DEF_CV_RPC_NAME_MUX        "mux"
#define DEF_CV_RPC_NAME_CLIENT_W   "client-w"
#define DEF_CV_RPC_NAME_CLIENT_P   "client-p"

#define DEF_CV_XPATH_RPC_ENABLE    "/" DEF_MODULE_NS_CV ":set-cv-enable"
#define DEF_CV_XPATH_RPC_VAL       DEF_CV_XPATH_RPC_ENABLE "/enable"
#define DEF_CV_XPATH_RPC_NAME      DEF_CV_XPATH_RPC_ENABLE "/name"
/* } @                                                                                            */

#define DEF_CV_XPATH_CONFIG                     DEF_CV_XPATH_HEADER "/config"
#define DEF_CV_LEAF_ENABLE_CV_PROTECTION        "/enable-cv-protection"
#define DEF_CV_XPATH_ENABLE_CV_PROTECTION       DEF_CV_XPATH_CONFIG DEF_CV_LEAF_ENABLE_CV_PROTECTION

#define DEF_CV_EVENT_FSM_INIT                   "fsm-init"
#define DEF_CV_EVENT_CONNECTOR_OPEN             "connector-open"
#define DEF_CV_EVENT_CONNECTOR_CLEAN            "connector-clean"
#define DEF_CV_EVENT_CONNECTOR_SLIGHTLY_DIRTY   "connector-slightly-dirty"
#define DEF_CV_EVENT_CONNECTOR_DIRTY            "connector-dirty"
#define DEF_CV_EVENT_PROTECTION_DISABLE         "protection-disable"
#define DEF_CV_EVENT_PROTECTION_ENABLE          "protection-enable"

#define CONNECTOR_OPEN_THR      (10.00)
#define MAX_PA_VOA_VALUE        (25.50)

/**************************************************************************************************/
/* definition of Function                                                                         */
/**************************************************************************************************/
/* @ {                                                                                            */

extern char m_aacRefName[EN_REFLECT_MAX][16];
int DSC_CvGetState(ST_CV_STATE *pstState);
int DSC_CvGetConfig(ST_CV_CONFIG *pstConfig);
int DSC_CvSetDifferThr(EN_DIFFER enIdx, double dHighThr, double dLowThr);
int DSC_CvRpcEnable(EN_CV_RPC_IDX enIdx, bool bIsEnable);
EN_CV_RPC_IDX DSC_CvRpcNameStrToIdx(char *pcName);
char *DSC_CvRpcNameIdxToStr(EN_CV_RPC_IDX enIdx);
int DSC_SetCvProtectionEnable(bool bEnable);
int DSC_GetCvProtectionEnable(sr_session_ctx_t *sess, bool *pbEnable);
/* } @                                                                                            */


#ifdef __cplusplus
}
#endif

#endif

