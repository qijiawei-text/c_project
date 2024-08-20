#ifndef _DSC_TCA_H
#define _DSC_TCA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
#include "DSC_alarm.h"
#include "DSC_ops.h"

/*
 * EdfaAB, EDFABA, EDFA1, EDFA2, EDFA,
 * Input/output
 *
 * OSCA, OSCB, OSC1, OSC2
 * Rx/Tx
 */

typedef enum en_TCA_IDX
{
    TCA_IDX_EDFA1_INPUT = 0x01,
    TCA_IDX_EDFA1_OUTPUT,
    TCA_IDX_EDFA1_MSA_INPUT,
    TCA_IDX_EDFA1_MSA_OUTPUT,
    TCA_IDX_EDFA1_GAIN,
    TCA_IDX_EDFA1_TILT,
    TCA_IDX_EDFA2_INPUT,
    TCA_IDX_EDFA2_OUTPUT,
    TCA_IDX_EDFA2_MSA_INPUT,
    TCA_IDX_EDFA2_MSA_OUTPUT,
    TCA_IDX_EDFA2_GAIN,
    TCA_IDX_EDFA2_TILT,
    TCA_IDX_EDFA3_INPUT,
    TCA_IDX_EDFA3_OUTPUT,
    TCA_IDX_EDFA3_MSA_INPUT,
    TCA_IDX_EDFA3_MSA_OUTPUT,
    TCA_IDX_EDFA3_GAIN,
    TCA_IDX_EDFA3_TILT,
    TCA_IDX_EDFA4_INPUT,
    TCA_IDX_EDFA4_OUTPUT,
    TCA_IDX_EDFA4_MSA_INPUT,
    TCA_IDX_EDFA4_MSA_OUTPUT,
    TCA_IDX_EDFA4_GAIN,
    TCA_IDX_EDFA4_TILT,
    TCA_IDX_OSC1_ADD,
    TCA_IDX_OSC1_DROP,
    TCA_IDX_OSC1_RX,
    TCA_IDX_OSC1_TX,
    TCA_IDX_OSC2_ADD,
    TCA_IDX_OSC2_DROP,
    TCA_IDX_OSC2_RX,
    TCA_IDX_OSC2_TX,
    TCA_IDX_PAM4_RX,
    TCA_IDX_PAM4_TX,
    TCA_IDX_PAM4_BER,
    TCA_IDX_PORT_LINE_IN = 0x40,
    TCA_IDX_PORT_LINE_OUT,
    TCA_IDX_PORT_3,
    TCA_IDX_PORT_4,
    TCA_IDX_PORT_5,
    TCA_IDX_PORT_6,
    TCA_IDX_PORT_MAX,

    TCA_IDX_CV_REF,
    TCA_IDX_CV_MUX_R,
    TCA_IDX_CV_MUX_T,
    TCA_IDX_CV_CLIENT_W_R,
    TCA_IDX_CV_CLIENT_W_T,
    TCA_IDX_CV_CLIENT_P_R,
    TCA_IDX_CV_CLIENT_P_T,

    TCA_IDX_SPANLOSS,
    TCA_IDX_SPANLOS_P,


    TCA_IDX_BA_IN_CHANNEL,
    TCA_IDX_PA_OUT_CHANNEL,
    TCA_IDX_BA_P_IN_CHANNEL,
    TCA_IDX_PA_P_OUT_CHANNEL,

    TCA_IDX_TEMP_HIGH_ALM,
    TCA_IDX_TEMP_HIGH_WARN,
    TCA_IDX_TEMP_LOW_ALM,
    TCA_IDX_TEMP_LOW_WARN,

    TCA_IDX_CPU_LOAD_HIGH,
    TCA_IDX_MEMORY_RESIDUAL_LOW,

    TCA_IDX_AC_VOL_LOW,
    TCA_IDX_AC_VOL_HIGH,
    TCA_IDX_DC_VOL_LOW,
    TCA_IDX_DC_VOL_HIGH,

    TCA_IDX_LINEIN_LOS,
    TCA_IDX_LINEIN_LOW,
    TCA_IDX_LINEIN_HIGH,
    TCA_IDX_LINEIN_P_LOS,
    TCA_IDX_LINEIN_P_LOW,
    TCA_IDX_LINEIN_P_HIGH,

    TCA_IDX_LINET_LOS,
    TCA_IDX_LINET_P_LOS,

    TCA_IDX_CLIENTT_LOS,
    TCA_IDX_CLIENTT_P_LOS,

    TCA_IDX_CLIENT_RX_LOW,
    TCA_IDX_CLIENT_RX_HIGH,
    TCA_IDX_CLIENT_P_RX_LOW,
    TCA_IDX_CLIENT_P_RX_HIGH,

    TCA_IDX_CLIENT_TX_LOW,
    TCA_IDX_CLIENT_TX_HIGH,
    TCA_IDX_CLIENT_P_TX_LOW,
    TCA_IDX_CLIENT_P_TX_HIGH,

    TCA_IDX_LINE_RX_LOW,
    TCA_IDX_LINE_RX_HIGH,
    TCA_IDX_LINE_P_RX_LOW,
    TCA_IDX_LINE_P_RX_HIGH,

    TCA_IDX_LINE_TX_LOW,
    TCA_IDX_LINE_TX_HIGH,
    TCA_IDX_LINE_P_TX_LOW,
    TCA_IDX_LINE_P_TX_HIGH,

    TCA_IDX_BA_CHANNELIN_LOW,
    TCA_IDX_BA_CHANNELIN_HIGH,
    TCA_IDX_BA_P_CHANNELIN_LOW,
    TCA_IDX_BA_P_CHANNELIN_HIGH,

    TCA_IDX_BA_CHANNELOUT_LOW,
    TCA_IDX_BA_CHANNELOUT_HIGH,
    TCA_IDX_BA_P_CHANNELOUT_LOW,
    TCA_IDX_BA_P_CHANNELOUT_HIGH,

    TCA_IDX_PA_CHANNELIN_LOW,
    TCA_IDX_PA_CHANNELIN_HIGH,
    TCA_IDX_PA_P_CHANNELIN_LOW,
    TCA_IDX_PA_P_CHANNELIN_HIGH,

    TCA_IDX_PA_CHANNELOUT_LOW,
    TCA_IDX_PA_CHANNELOUT_HIGH,
    TCA_IDX_PA_P_CHANNELOUT_LOW,
    TCA_IDX_PA_P_CHANNELOUT_HIGH,

    TCA_IDX_OSC_TX_LOW,
    TCA_IDX_OSC_TX_HIGH,
    TCA_IDX_OSC_P_TX_LOW,
    TCA_IDX_OSC_P_TX_HIGH,

    TCA_IDX_OSC_RX_LOW,
    TCA_IDX_OSC_RX_HIGH,
    TCA_IDX_OSC_P_RX_LOW,
    TCA_IDX_OSC_P_RX_HIGH,

    TCA_IDX_OSC_ADD_LOW,
    TCA_IDX_OSC_ADD_HIGH,
    TCA_IDX_OSC_P_ADD_LOW,
    TCA_IDX_OSC_P_ADD_HIGH,

    TCA_IDX_OSC_DROP_LOW,
    TCA_IDX_OSC_DROP_HIGH,
    TCA_IDX_OSC_P_DROP_LOW,
    TCA_IDX_OSC_P_DROP_HIGH,

    TCA_IDX_SPANLOSS_VARIATION_LOW,
    TCA_IDX_SPANLOSS_VARIATION_HIGH,
    TCA_IDX_SPANLOSS_P_VARIATION_LOW,
    TCA_IDX_SPANLOSS_P_VARIATION_HIGH,

    TCA_IDX_BA_PUMP1_CURRENT_HIGH,
    TCA_IDX_BA_PUMP2_CURRENT_HIGH,
    TCA_IDX_BA_PUMP3_CURRENT_HIGH,
    TCA_IDX_BA_P_PUMP1_CURRENT_HIGH,
    TCA_IDX_BA_P_PUMP2_CURRENT_HIGH,
    TCA_IDX_BA_P_PUMP3_CURRENT_HIGH,

    TCA_IDX_PA_PUMP1_CURRENT_HIGH,
    TCA_IDX_PA_PUMP2_CURRENT_HIGH,
    TCA_IDX_PA_P_PUMP1_CURRENT_HIGH,
    TCA_IDX_PA_P_PUMP2_CURRENT_HIGH,

    TCA_IDX_LASERSAFETY_RX_LOS,
    TCA_IDX_LASERSAFETY_P_RX_LOS,
    TCA_IDX_LASERSAFETY_TX_LOS,
    TCA_IDX_LASERSAFETY_P_TX_LOS,
    TCA_IDX_PA_RFL,
    TCA_IDX_BA_RFL,
    /* Support multiple OPS here. max count is 10 */
    TCA_IDX_OPS_START,
    TCA_IDX_OPS_COM_IN_LOS = TCA_IDX_OPS_START,
    TCA_IDX_OPS_POWER_DIFFER,
    TCA_IDX_OPS_COM_IN_LOW,
    TCA_IDX_OPS_COM_IN_HIGH,
    TCA_IDX_OPS_PRI_IN_LOW,
    TCA_IDX_OPS_PRI_IN_HIGH,
    TCA_IDX_OPS_SEC_IN_LOW,
    TCA_IDX_OPS_SEC_IN_HIGH,
    TCA_IDX_OPS_END = TCA_IDX_OPS_SEC_IN_HIGH + OPS_DEV_NUM_MAX * (TCA_IDX_OPS_SEC_IN_HIGH - TCA_IDX_OPS_START + 1),

    TCA_IDX_MAX
}EN_TCA_IDX;

typedef enum
{
    TCA_TX = 0x01,
    TCA_RX,
    TCA_ADD,
    TCA_DROP,

    TCA_DIR_MAX
}EN_TCA_DIR;

#define TCA_NAME_MAX_LEN                  (64)
typedef struct st_TCA_TRANS
{
    int iIdx;                             /* For TCA-id str are different. */
    char acId[TCA_NAME_MAX_LEN];
}ST_TCA_TRANS;

typedef struct st_TCA_INFO
{
    int iIdx;                             /* For TCA-id str are different. */
    char acId[TCA_NAME_MAX_LEN];
    double dLowThr;
    double dHighThr;
    SEVERITY_EN enSeverity;
    char acCpName[32];
    char acPortName[32];
} TCA_INFO_ST;

#define DATA_MODULE_TCA                   "org-openroadm-tca"
#define TCA_XPATH_HEADER                  "/org-openroadm-tca:potential-tca-list/tca"
#define TCA_XPATH_ALL                     TCA_XPATH_HEADER "//*"
#define TCA_ID_LEAF                       "/id"
#define TCA_XPATH_ID                      TCA_XPATH_HEADER TCA_ID_LEAF
#define TCA_LOW_VALUE_LEAF                "/lowThresholdValue"
#define TCA_XPATH_TCA_LOW                 TCA_XPATH_HEADER "[id='%s']" TCA_LOW_VALUE_LEAF
#define TCA_HIGH_VALUE_LEAF               "/highThresholdValue"
#define TCA_XPATH_TCA_HIGH                TCA_XPATH_HEADER "[id='%s']" TCA_HIGH_VALUE_LEAF
#define TCA_CP_NAME_LEAF                  "/circuit-pack-name"
#define TCA_PORT_NAME_LEAF                "/port-name"



extern int DSC_GetTcaIdx(char *xpath);
extern char *DSC_GetTcaName(EN_TCA_IDX enIdx);
extern int DSC_GetTcaList(sr_session_ctx_t *sess, TCA_INFO_ST **ppstTca, size_t *count);
extern int DSC_GetTcaThresh(EN_TCA_IDX enIdx, TCA_INFO_ST *pstInfo, double *pdHighThr, double *pdLowThr);
extern int DSC_SetTcaThreshold(sr_session_ctx_t *pstSess, EN_TCA_IDX enIdx, double dHighThr, double dLowThr);
extern int DSC_SetTcaThresh(sr_session_ctx_t *pstSess, const char *pcTcaName, double dHighThr, double dLowThr);
extern int DSC_GetTcaCpNamePortName(int iTcaId, char *pcCpName, char *pcPortName);
extern int DSC_GetTcaIdxFromName(char *pcId, int *iIdx);

#ifdef __cplusplus
}
#endif

#endif

