/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : dsc_parameter.h
   Description : System parameter range.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/06/24   V1.0.0       Initial file.
**********************************************************************************/

#ifndef __DSC_PARAMETER_H
#define __DSC_PARAMETER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char acParaName[64];
    double dHighVal;
    double dLowVal;
} ST_PARAMETER;


/* DEF_MAX_PARA for com-oplink-parameter-range-startup-ROADM.xml list length */
#define DEF_MAX_PARA              (200)
#define DEF_PARA_XPATH            "/com-oplink-parameter-range:parameter-range/parameters//*"

#define COP_PARA_HEAD             "/com-oplink-parameter-range:parameter-range/parameters"

#define COP_RANGE_VALUE                   "/range"
#define COP_LOW_VALUE_LEAF                "/low-thr"
#define COP_XPATH_COP_LOW                 COP_PARA_HEAD "[name='%s']" COP_RANGE_VALUE COP_LOW_VALUE_LEAF
#define COP_HIGH_VALUE_LEAF               "/high-thr"
#define COP_XPATH_COP_HIGH                COP_PARA_HEAD "[name='%s']" COP_RANGE_VALUE COP_HIGH_VALUE_LEAF


/* edfa1 -> pa, edfa2 -> ba, edfa3 -> pa-p, edfa4 -> ba-p*/
#define EDFA1_RANGE_GAIN            "edfa1_gain"
#define EDFA1_RANGE_LOW_GAIN        "edfa1_low_gain"
#define EDFA1_RANGE_HIGH_GAIN       "edfa1_high_gain"
#define EDFA1_RANGE_TILT            "edfa1_tilt"
#define EDFA1_RANGE_INLOS_HIGH      "edfa1_threshold_inlos_high"
#define EDFA1_RANGE_INLOS_LOW       "edfa1_threshold_inlos_low"
#define EDFA1_RANGE_INLOS_DIFFER    "edfa1_threshold_inlos_differ"
#define EDFA1_RANGE_RFL             "edfa1_threshold_rfl"
#define EDFA1_RANGE_CUR_THR         "edfa1_threshold_current"
#define EDFA1_RANGE_CUR_HYS         "edfa1_hysteresis_current"
#define EDFA1_RANGE_ORL_HIGH        "edfa1_threshold_orl_high"
#define EDFA1_RANGE_ORL_LOW         "edfa1_threshold_orl_low"
#define EDFA1_RANGE_ORL_DIFFER      "edfa1_threshold_orl_differ"

#define EDFA2_RANGE_GAIN            "edfa2_gain"
#define EDFA2_RANGE_LOW_GAIN        "edfa2_low_gain"
#define EDFA2_RANGE_HIGH_GAIN       "edfa2_high_gain"
#define EDFA2_RANGE_TILT            "edfa2_tilt"
#define EDFA2_RANGE_INLOS_HIGH      "edfa2_threshold_inlos_high"
#define EDFA2_RANGE_INLOS_LOW       "edfa2_threshold_inlos_low"
#define EDFA2_RANGE_INLOS_DIFFER    "edfa2_threshold_inlos_differ"
#define EDFA2_RANGE_RFL             "edfa2_threshold_rfl"
#define EDFA2_RANGE_CUR_THR         "edfa2_threshold_current"
#define EDFA2_RANGE_CUR_HYS         "edfa2_hysteresis_current"
#define EDFA2_RANGE_ORL_HIGH        "edfa2_threshold_orl_high"
#define EDFA2_RANGE_ORL_LOW         "edfa2_threshold_orl_low"
#define EDFA2_RANGE_ORL_DIFFER      "edfa2_threshold_orl_differ"

#define EDFA3_RANGE_GAIN            "edfa3_gain"
#define EDFA3_RANGE_LOW_GAIN        "edfa3_low_gain"
#define EDFA3_RANGE_HIGH_GAIN       "edfa3_high_gain"
#define EDFA3_RANGE_TILT            "edfa3_tilt"
#define EDFA3_RANGE_INLOS_HIGH      "edfa3_threshold_inlos_high"
#define EDFA3_RANGE_INLOS_LOW       "edfa3_threshold_inlos_low"
#define EDFA3_RANGE_INLOS_DIFFER    "edfa3_threshold_inlos_differ"
#define EDFA3_RANGE_RFL             "edfa3_threshold_rfl"
#define EDFA3_RANGE_CUR_THR         "edfa3_threshold_current"
#define EDFA3_RANGE_CUR_HYS         "edfa3_hysteresis_current"
#define EDFA3_RANGE_ORL_HIGH        "edfa3_threshold_orl_high"
#define EDFA3_RANGE_ORL_LOW         "edfa3_threshold_orl_low"
#define EDFA3_RANGE_ORL_DIFFER      "edfa3_threshold_orl_differ"

#define EDFA4_RANGE_GAIN            "edfa4_gain"
#define EDFA4_RANGE_LOW_GAIN        "edfa4_low_gain"
#define EDFA4_RANGE_HIGH_GAIN       "edfa4_high_gain"
#define EDFA4_RANGE_TILT            "edfa4_tilt"
#define EDFA4_RANGE_INLOS_HIGH      "edfa4_threshold_inlos_high"
#define EDFA4_RANGE_INLOS_LOW       "edfa4_threshold_inlos_low"
#define EDFA4_RANGE_INLOS_DIFFER    "edfa4_threshold_inlos_differ"
#define EDFA4_RANGE_RFL             "edfa4_threshold_rfl"
#define EDFA4_RANGE_CUR_THR         "edfa4_threshold_current"
#define EDFA4_RANGE_CUR_HYS         "edfa4_hysteresis_current"
#define EDFA4_RANGE_ORL_HIGH        "edfa4_threshold_orl_high"
#define EDFA4_RANGE_ORL_LOW         "edfa4_threshold_orl_low"
#define EDFA4_RANGE_ORL_DIFFER      "edfa4_threshold_orl_differ"

#define PADDING_VOA_ATTN            "padding-voa_atten"
#define PA_VOA_ATTN                 "pa-voa_atten"
#define PA_VOA_P_ATTN               "pa-voa-p_atten"
#define PADDING_VOA_P_ATTN          "padding-voa-p_atten"
#define ASE_VOA_ATTN                "ase-voa_atten"
#define OSC_TX_VOA_ATTN             "osc-tx-voa_atten"
#define OSC_RX_VOA_ATTN             "osc-rx-voa_atten"
#define OSC_TX_VOA_P_ATTN           "osc-tx-voa-p_atten"
#define OSC_RX_VOA_P_ATTN           "osc-rx-voa-p_atten"

#define DEF_ATTN_VOA_PADDING        "padding-voa_atten"
#define DEF_ATTN_VOA_PA             "pa-voa_atten"
#define DEF_ATTN_VOA_OSC_TX         "osc-tx-voa_atten"
#define DEF_ATTN_VOA_OSC_RX         "osc-rx-voa_atten"
#define DEF_ATTN_VOA_OCM            "ocm-voa_atten"
#define DEF_ATTN_VOA_PADDING_P      "padding-voa-p_atten"
#define DEF_ATTN_VOA_PA_P           "pa-voa-p_atten"
#define DEF_ATTN_VOA_OSC_TX_P       "osc-tx-voa-p_atten"
#define DEF_ATTN_VOA_OSC_RX_P       "osc-rx-voa-p_atten"
#define DEF_ATTN_VOA_OCM_P          "ocm-voa-p_atten"
#define DEF_TARGET_VOA_OSC_TX       "osc-tx-voa_target"
#define DEF_TARGET_VOA_OSC_TX_P     "osc-tx-voa-p_target"

#define DEF_LINE_RANGE_OUTLOS_HIGH      "line_threshold_outlos_high"
#define DEF_LINE_RANGE_OUTLOS_LOW       "line_threshold_outlos_low"
#define DEF_LINE_RANGE_OUTLOS_DIFFER    "line_threshold_outlos_differ"
#define DEF_LINE_RANGE_RXLOW_HIGH       "line_threshold_rxlow_high"
#define DEF_LINE_RANGE_RXLOW_LOW        "line_threshold_rxlow_low"
#define DEF_LINE_RANGE_RXLOW_DIFFER     "line_threshold_rxlow_differ"
#define DEF_LINE_RANGE_RXHIGH_HIGH      "line_threshold_rxhigh_high"
#define DEF_LINE_RANGE_RXHIGH_LOW       "line_threshold_rxhigh_low"
#define DEF_LINE_RANGE_RXHIGH_DIFFER    "line_threshold_rxhigh_differ"
#define DEF_LINE_RANGE_TXLOW_HIGH       "line_threshold_txlow_high"
#define DEF_LINE_RANGE_TXLOW_LOW        "line_threshold_txlow_low"
#define DEF_LINE_RANGE_TXLOW_DIFFER     "line_threshold_txlow_differ"
#define DEF_LINE_RANGE_TXHIGH_HIGH      "line_threshold_txhigh_high"
#define DEF_LINE_RANGE_TXHIGH_LOW       "line_threshold_txhigh_low"
#define DEF_LINE_RANGE_TXHIGH_DIFFER    "line_threshold_txhigh_differ"

#define DEF_CLIENT_RANGE_OUTLOS_HIGH    "client_threshold_outlos_high"
#define DEF_CLIENT_RANGE_OUTLOS_LOW     "client_threshold_outlos_low"
#define DEF_CLIENT_RANGE_OUTLOS_DIFFER  "client_threshold_outlos_differ"
#define DEF_CLIENT_RANGE_RXLOW_HIGH     "client_threshold_rxlow_high"
#define DEF_CLIENT_RANGE_RXLOW_LOW      "client_threshold_rxlow_low"
#define DEF_CLIENT_RANGE_RXLOW_DIFFER   "client_threshold_rxlow_differ"
#define DEF_CLIENT_RANGE_RXHIGH_HIGH    "client_threshold_rxhigh_high"
#define DEF_CLIENT_RANGE_RXHIGH_LOW     "client_threshold_rxhigh_low"
#define DEF_CLIENT_RANGE_RXHIGH_DIFFER  "client_threshold_rxhigh_differ"
#define DEF_CLIENT_RANGE_TXLOW_HIGH     "client_threshold_txlow_high"
#define DEF_CLIENT_RANGE_TXLOW_LOW      "client_threshold_txlow_low"
#define DEF_CLIENT_RANGE_TXLOW_DIFFER   "client_threshold_txlow_differ"
#define DEF_CLIENT_RANGE_TXHIGH_HIGH    "client_threshold_txhigh_high"
#define DEF_CLIENT_RANGE_TXHIGH_LOW     "client_threshold_txhigh_low"
#define DEF_CLIENT_RANGE_TXHIGH_DIFFER  "client_threshold_txhigh_differ"

#define OLP_RANGE_INLOS_LOW         "In65-low"
#define OLP_RANGE_HYS               "ops-hysteresis"

#define DEF_OSC_RANGE_ADDLOS_HIGH   "osc_threshold_addlos_high"
#define DEF_OSC_RANGE_ADDLOS_LOW    "osc_threshold_addlos_low"
#define DEF_OSC_RANGE_ADDLOS_DIFF   "osc_threshold_addlos_differ"
#define DEF_OSC_RANGE_DROPLOS_HIGH  "osc_threshold_droplos_high"
#define DEF_OSC_RANGE_DROPLOS_LOW   "osc_threshold_droplos_low"
#define DEF_OSC_RANGE_DROPLOS_DIFF  "osc_threshold_droplos_differ"
#define DEF_OSC_RANGE_TXLOS_HIGH    "osc_threshold_txlos_high"
#define DEF_OSC_RANGE_TXLOS_LOW     "osc_threshold_txlos_low"
#define DEF_OSC_RANGE_TXLOS_DIFF    "osc_threshold_txlos_differ"
#define DEF_OSC_RANGE_RXLOS_HIGH    "osc_threshold_rxlos_high"
#define DEF_OSC_RANGE_RXLOS_LOW     "osc_threshold_rxlos_low"
#define DEF_OSC_RANGE_RXLOS_DIFF    "osc_threshold_rxlos_differ"
#define DEF_OSC_RANGE_TXLOW_HIGH    "osc_threshold_txlow_high"
#define DEF_OSC_RANGE_TXLOW_LOW     "osc_threshold_txlow_low"
#define DEF_OSC_RANGE_TXLOW_DIFF    "osc_threshold_txlow_differ"
#define DEF_OSC_RANGE_TXHIGH_HIGH   "osc_threshold_txhigh_high"
#define DEF_OSC_RANGE_TXHIGH_LOW    "osc_threshold_txhigh_low"
#define DEF_OSC_RANGE_TXHIGH_DIFF   "osc_threshold_txhigh_differ"
#define DEF_OSC_RANGE_RXLOW_HIGH    "osc_threshold_rxlow_high"
#define DEF_OSC_RANGE_RXLOW_LOW     "osc_threshold_rxlow_low"
#define DEF_OSC_RANGE_RXLOW_DIFF    "osc_threshold_rxlow_differ"
#define DEF_OSC_RANGE_RXHIGH_HIGH   "osc_threshold_rxhigh_high"
#define DEF_OSC_RANGE_RXHIGH_LOW    "osc_threshold_rxhigh_low"
#define DEF_OSC_RANGE_RXHIGH_DIFF   "osc_threshold_rxhigh_differ"
#define DEF_OSC_RANGE_ADDLOW_HIGH   "osc_threshold_addlow_high"
#define DEF_OSC_RANGE_ADDLOW_LOW    "osc_threshold_addlow_low"
#define DEF_OSC_RANGE_ADDLOW_DIFF   "osc_threshold_addlow_differ"
#define DEF_OSC_RANGE_ADDHIGH_HIGH  "osc_threshold_addhigh_high"
#define DEF_OSC_RANGE_ADDHIGH_LOW   "osc_threshold_addhigh_low"
#define DEF_OSC_RANGE_ADDHIGH_DIFF  "osc_threshold_addhigh_differ"
#define DEF_OSC_RANGE_DROPLOW_HIGH  "osc_threshold_droplow_high"
#define DEF_OSC_RANGE_DROPLOW_LOW   "osc_threshold_droplow_low"
#define DEF_OSC_RANGE_DROPLOW_DIFF  "osc_threshold_droplow_differ"
#define DEF_OSC_RANGE_DROPHIGH_HIGH "osc_threshold_drophigh_high"
#define DEF_OSC_RANGE_DROPHIGH_LOW  "osc_threshold_drophigh_low"
#define DEF_OSC_RANGE_DROPHIGH_DIFF "osc_threshold_drophigh_differ"
#define DEF_SPANLOSS_RANGE_VARIATION_LOW    "spanloss_variation_threshold_low"
#define DEF_SPANLOSS_RANGE_VARIATION_HIGH   "spanloss_variation_threshold_high"

#define DEF_OCM_RANGE_BA_CH_INLOW_HIGH      "ocm-ba-ch_threshold_inlow_high"
#define DEF_OCM_RANGE_BA_CH_INLOW_LOW       "ocm-ba-ch_threshold_inlow_low"
#define DEF_OCM_RANGE_BA_CH_INLOW_DIFF      "ocm-ba-ch_threshold_inlow_differ"
#define DEF_OCM_RANGE_BA_CH_INHIGH_HIGH     "ocm-ba-ch_threshold_inhigh_high"
#define DEF_OCM_RANGE_BA_CH_INHIGH_LOW      "ocm-ba-ch_threshold_inhigh_low"
#define DEF_OCM_RANGE_BA_CH_INHIGH_DIFF     "ocm-ba-ch_threshold_inhigh_differ"

#define DEF_OCM_RANGE_BA_CH_OUTLOW_HIGH     "ocm-ba-ch_threshold_outlow_high"
#define DEF_OCM_RANGE_BA_CH_OUTLOW_LOW      "ocm-ba-ch_threshold_outlow_low"
#define DEF_OCM_RANGE_BA_CH_OUTLOW_DIFF     "ocm-ba-ch_threshold_outlow_differ"
#define DEF_OCM_RANGE_BA_CH_OUTHIGH_HIGH    "ocm-ba-ch_threshold_outhigh_high"
#define DEF_OCM_RANGE_BA_CH_OUTHIGH_LOW     "ocm-ba-ch_threshold_outhigh_low"
#define DEF_OCM_RANGE_BA_CH_OUTHIGH_DIFF    "ocm-ba-ch_threshold_outhigh_differ"

#define DEF_OCM_RANGE_PA_CH_INLOW_HIGH      "ocm-pa-ch_threshold_inlow_high"
#define DEF_OCM_RANGE_PA_CH_INLOW_LOW       "ocm-pa-ch_threshold_inlow_low"
#define DEF_OCM_RANGE_PA_CH_INLOW_DIFF      "ocm-pa-ch_threshold_inlow_differ"
#define DEF_OCM_RANGE_PA_CH_INHIGH_HIGH     "ocm-pa-ch_threshold_inhigh_high"
#define DEF_OCM_RANGE_PA_CH_INHIGH_LOW      "ocm-pa-ch_threshold_inhigh_low"
#define DEF_OCM_RANGE_PA_CH_INHIGH_DIFF     "ocm-pa-ch_threshold_inhigh_differ"

#define DEF_OCM_RANGE_PA_CH_OUTLOW_HIGH     "ocm-pa-ch_threshold_outlow_high"
#define DEF_OCM_RANGE_PA_CH_OUTLOW_LOW      "ocm-pa-ch_threshold_outlow_low"
#define DEF_OCM_RANGE_PA_CH_OUTLOW_DIFF     "ocm-pa-ch_threshold_outlow_differ"
#define DEF_OCM_RANGE_PA_CH_OUTHIGH_HIGH    "ocm-pa-ch_threshold_outhigh_high"
#define DEF_OCM_RANGE_PA_CH_OUTHIGH_LOW     "ocm-pa-ch_threshold_outhigh_low"
#define DEF_OCM_RANGE_PA_CH_OUTHIGH_DIFF    "ocm-pa-ch_threshold_outhigh_differ"

#define DEF_OCM_RANGE_TEMP_HIGH_HIGH        "temperature_high_high"
#define DEF_OCM_RANGE_TEMP_HIGH_LOW         "temperature_high_low"
#define DEF_OCM_RANGE_TEMP_HIGH_DIFF        "temperature_high_differ"
#define DEF_OCM_RANGE_TEMP_LOW_HIGH         "temperature_low_high"
#define DEF_OCM_RANGE_TEMP_LOW_LOW          "temperature_low_low"
#define DEF_OCM_RANGE_TEMP_LOW_DIFF         "temperature_low_differ"

#define DEF_CPU_LOAD_HIGH_HIGH              "cpu_load_high_high"
#define DEF_CPU_LOAD_HIGH_LOW               "cpu_load_high_low"
#define DEF_CPU_LOAD_HIGH_DIFF              "cpu_load_high_differ"

#define DEF_MEMORY_RESIDUAL_LOW_HIGH        "memory_residual_low_high"
#define DEF_MEMORY_RESIDUAL_LOW_LOW         "memory_residual_low_low"
#define DEF_MEMORY_RESIDUAL_LOW_DIFF        "memory_residual_low_differ"

#define DEF_AC_RANGE_SPANLOSS               "spanloss"
#define DEF_AC_RANGE_SPANLOSS_VARIATION     "spanloss_variation"
#define DEF_AC_RANGE_LAUNCHING_POWER        "launching_power_per_channel"
#define DEF_AC_RANGE_TARGET_POWER           "target_power_per_channel"
#define DEF_AC_RANGE_PADDING_VOA            "autoctrl_padding-voa_atten"
#define DEF_AC_RANGE_SATURATION_DROP        "saturation-drop"
#define DEF_AC_RANGE_PA_INPUT_TARGET        "pre-amp-input-target-power-per-channel"

#define DEF_MUX_RANGE_ORL_HIGH              "mux_threshold_orl_high"
#define DEF_MUX_RANGE_ORL_LOW               "mux_threshold_orl_low"
#define DEF_MUX_RANGE_ORL_DIFF              "mux_threshold_orl_differ"

#define DEF_CV_RANGE_MUX_R_HIGH             "cv_threshold_mux-r_high"
#define DEF_CV_RANGE_MUX_R_LOW              "cv_threshold_mux-r_low"
#define DEF_CV_RANGE_MUX_R_DIFF             "cv_threshold_mux-r_differ"
#define DEF_CV_RANGE_MUX_T_HIGH             "cv_threshold_mux-t_high"
#define DEF_CV_RANGE_MUX_T_LOW              "cv_threshold_mux-t_low"
#define DEF_CV_RANGE_MUX_T_DIFF             "cv_threshold_mux-t_differ"
#define DEF_CV_RANGE_CLIENT_W_R_HIGH        "cv_threshold_client-w-r_high"
#define DEF_CV_RANGE_CLIENT_W_R_LOW         "cv_threshold_client-w-r_low"
#define DEF_CV_RANGE_CLIENT_W_R_DIFF        "cv_threshold_client-w-r_differ"
#define DEF_CV_RANGE_CLIENT_W_T_HIGH        "cv_threshold_client-w-t_high"
#define DEF_CV_RANGE_CLIENT_W_T_LOW         "cv_threshold_client-w-t_low"
#define DEF_CV_RANGE_CLIENT_W_T_DIFF        "cv_threshold_client-w-t_differ"
#define DEF_CV_RANGE_CLIENT_P_R_HIGH        "cv_threshold_client-p-r_high"
#define DEF_CV_RANGE_CLIENT_P_R_LOW         "cv_threshold_client-p-r_low"
#define DEF_CV_RANGE_CLIENT_P_R_DIFF        "cv_threshold_client-p-r_differ"
#define DEF_CV_RANGE_CLIENT_P_T_HIGH        "cv_threshold_client-p-t_high"
#define DEF_CV_RANGE_CLIENT_P_T_LOW         "cv_threshold_client-p-t_low"
#define DEF_CV_RANGE_CLIENT_P_T_DIFF        "cv_threshold_client-p-t_differ"

#define DEF_LASERSAFETY_RANGE_TARGET_OUT    "lasersafety_target_output_power"
#define DEF_LASERSAFETY_RANGE_R_LOS_HIGH    "lasersafety_threshold_r_los_high"
#define DEF_LASERSAFETY_RANGE_R_LOS_LOW     "lasersafety_threshold_r_los_low"
#define DEF_LASERSAFETY_RANGE_R_LOS_DIFF    "lasersafety_threshold_r_los_differ"
#define DEF_LASERSAFETY_RANGE_T_LOS_HIGH    "lasersafety_threshold_t_los_high"
#define DEF_LASERSAFETY_RANGE_T_LOS_LOW     "lasersafety_threshold_t_los_low"
#define DEF_LASERSAFETY_RANGE_T_LOS_DIFF    "lasersafety_threshold_t_los_differ"
#define DEF_LASERSAFETY_RANGE_T_LOS_DIFF    "lasersafety_threshold_t_los_differ"
#define EDFA1_RANGE_READY_POWER             "edfa1_ready_power"
#define EDFA2_RANGE_READY_POWER             "edfa2_ready_power"

int DSC_GetParameterLists(sr_session_ctx_t *pstStartupSess, ST_PARAMETER **ppstParaList, size_t *pnCnt);
int DSC_GetParameterByName(sr_session_ctx_t *pstStartupSess, const char *pcParaName, double *pdHighVal, double *pdLowVal);

#ifdef __cplusplus
}
#endif

#endif

