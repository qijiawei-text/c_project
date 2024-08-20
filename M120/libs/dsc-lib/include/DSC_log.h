/*
 * @Author: your name
 * @Date: 2021-02-11 16:53:14
 * @LastEditTime: 2021-02-17 10:35:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /dsc-lib/include/DSC_log.h
 */
/*print utility for testing*/
#include <stdio.h>
#include <stdlib.h>
#include "sysrepo.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "util_inc.h"

extern const char *ev_to_str(sr_event_t ev);

extern void dsc_print_value(sr_val_t *value);

extern void print_change(sr_change_oper_t op, sr_val_t *old_val, sr_val_t *new_val);

extern void print_current_config(sr_session_ctx_t *session, const char *module_name);


#define libdsc_err_begin(default_rc) int rc = (default_rc); char error_string[512] = {'\0', }

#define libdsc_err_return(where_to_jmp, ret_code, err_str_fmt, ...) do {   \
        rc = ret_code;  \
        snprintf((error_string), 512, (err_str_fmt), ## __VA_ARGS__);    \
        goto where_to_jmp; \
} while (0)

#define libdsc_err_cond_return(where_to_jmp, ret_code, err_str_fmt, ...) \
    if (rc != SR_ERR_OK) libdsc_err_return(where_to_jmp, ret_code, err_str_fmt, ## __VA_ARGS__)

#define libdsc_err_skip(err_str_fmt, ...) do {  \
        LOG_WRITE(EN_LOG_TRACE, (err_str_fmt), ## __VA_ARGS__);    \
} while (0)

#define libdsc_err_cond_skip(err_str_fmt, ...)  \
    if (rc != SR_ERR_OK) libdsc_err_skip(err_str_fmt, ## __VA_ARGS__)

#define libdsc_err_end()  ({   \
                               LOG_WRITE(EN_LOG_ERROR, "%d:%s %s", rc, sr_strerror(rc), error_string);   \
                               rc;  \
                           })


#define STR_ID_SET_ENABELED          "SET_ENABELED"
#define STR_ID_SET_AUTOLOS           "SET_AUTOLOS"
#define STR_ID_SET_TCA_INPUT         "SET_TCA_INPUT"
#define STR_ID_SET_TCA_OUTPUT        "SET_TCA_OUTPUT"
#define STR_ID_SET_ATTN              "SET_ATTN"
#define STR_ID_SET_MONITOR_PORT      "SET_MONITOR_PORT"
#define STR_ID_SET_MANUAL_TO_PORT    "SET_MANUAL_TO_PORT"
#define STR_ID_SET_REVERTIVE         "SET_REVERTIVE"
#define STR_ID_SET_WTR               "SET_WTR"
#define STR_ID_SET_HOLD_OFF_TIME     "SET_HOLD_OFF_TIME"
#define STR_ID_SET_PRIMARY_THR       "SET_PRIMARY_THR"
#define STR_ID_SET_PRIMARY_HYS       "SET_PRIMARY_HYS"
#define STR_ID_SET_SECONDARY_THR     "SET_SECONDARY_THR"
#define STR_ID_SET_SECONDARY_HYS     "SET_SECONDARY_HYS"
#define STR_ID_SET_RELATIVE_THR      "SET_RELATIVE_THR"
#define STR_ID_SET_RELATIVE_OFFSET   "SET_RELATIVE_OFFSET"
#define STR_ID_SET_FORCE_TO_PORT     "SET_FORCE_TO_PORT"
#define STR_ID_LOGIN                 "LOGIN"
#define STR_ID_LOGOUT                "LOGOUT"
#define STR_ID_SET_USER              "SET_USER"
#define STR_ID_DELETE_USER           "DELETE_USER"
#define STR_ID_SET_TACACS            "SET_TACACS"
#define STR_ID_DELETE_TACACS         "DELETE_TACACS"
#define STR_ID_SET_USER_AUTH_ORDER      "SET_USER_AUTH_ORDER"
#define STR_ID_SET_SESSION_TIMEOUT      "SET_SESSION_TIMEOUT"
#define STR_ID_SET_SESSION_MAX_COUNT    "SET_SESSION_MAX_COUNT"
#define STR_ID_SET_PER_COMMAND_AUTHORIZATION     "SET_PER_COMMAND_AUTHORIZATION"
#define STR_ID_ADD_MEDIA_CHANNEL        "ADD_MEDIA_CHANNEL"
#define STR_ID_MODIFY_MEDIA_CHANNEL     "MODIFY_MEDIA_CHANNEL"
#define STR_ID_DELETE_MEDIA_CHANNEL     "DELETE_MEDIA_CHANNEL"
#define STR_ID_SET_NETWORK_IPV4         "SET_NETWORK_IPV4"
#define STR_ID_SET_TIME              "SET_TIME"
#define STR_ID_SET_NTP_ENABELD       "SET_NTP_ENABELED"
#define STR_ID_SET_NTP_ADDRESS       "SET_NTP_ADDRESS"
#define STR_ID_DELETE_NTP            "DELETE_NTP"
#define STR_ID_DB_RESTORE_DEFAULT    "DB_RESTORE_DEFAULT"
#define STR_ID_UPGRADE_SOFTWARE      "UPGRADE_SOFTWARE"
#define STR_ID_RESTART               "RESTART"
#define STR_ID_FILE_TRANSFER         "FILE_TRANSFER"
#define STR_ID_SET_ALARMS_ON         "SET_ALARMS_ON"
#define STR_ID_SET_ALARMS_OFF        "SET_ALARMS_OFF"
#define STR_ID_SET_ALARMS_MASK_ON         "SET_MASK_ON"
#define STR_ID_SET_ALARMS_MASK_OFF        "SET_MASK_OFF"
#define DEF_STR_ID_SET_TCA_OSC_TX           "SET_TCA_OSC_TX"
#define DEF_STR_ID_SET_TCA_OSC_ADD          "SET_TCA_OSC_ADD"
#define DEF_STR_ID_SET_TCA_OSC_DROP         "SET_TCA_OSC_DROP"
#define DEF_STR_ID_SET_TCA_OSC_RX           "SET_TCA_OSC_RX"
#define DEF_STR_ID_SET_TCA_LINET_LOS        "SET_TCA_LINET_LOS"
#define DEF_STR_ID_SET_TCA_CLIENTT_LOS      "SET_TCA_CLIENTT_LOS"
#define DEF_STR_ID_SET_TCA_CLIENT_RX_LOW    "SET_TCA_CLIENT_RX_LOW"
#define DEF_STR_ID_SET_TCA_CLIENT_RX_HIGH   "SET_TCA_CLIENT_RX_HIGH"
#define DEF_STR_ID_SET_TCA_CLIENT_TX_LOW    "SET_TCA_CLIENT_TX_LOW"
#define DEF_STR_ID_SET_TCA_CLIENT_TX_HIGH   "SET_TCA_CLIENT_TX_HIGH"
#define DEF_STR_ID_SET_TCA_LINE_RX_LOW      "SET_TCA_LINE_RX_LOW"
#define DEF_STR_ID_SET_TCA_LINE_RX_HIGH     "SET_TCA_LINE_RX_HIGH"
#define DEF_STR_ID_SET_TCA_LINE_TX_LOW      "SET_TCA_LINE_TX_LOW"
#define DEF_STR_ID_SET_TCA_LINE_TX_HIGH     "SET_TCA_LINE_TX_HIGH"
#define DEF_STR_ID_SET_TCA_BA_CH_IN_LOW     "SET_TCA_BA_CH_IN_LOW"
#define DEF_STR_ID_SET_TCA_BA_CH_IN_HIGH    "SET_TCA_BA_CH_IN_HIGH"
#define DEF_STR_ID_SET_TCA_BA_CH_OUT_LOW    "SET_TCA_BA_CH_OUT_LOW"
#define DEF_STR_ID_SET_TCA_BA_CH_OUT_HIGH   "SET_TCA_BA_CH_OUT_HIGH"
#define DEF_STR_ID_SET_TCA_PA_CH_IN_LOW     "SET_TCA_PA_CH_IN_LOW"
#define DEF_STR_ID_SET_TCA_PA_CH_IN_HIGH    "SET_TCA_PA_CH_IN_HIGH"
#define DEF_STR_ID_SET_TCA_PA_CH_OUT_LOW    "SET_TCA_PA_CH_OUT_LOW"
#define DEF_STR_ID_SET_TCA_PA_CH_OUT_HIGH   "SET_TCA_PA_CH_OUT_HIGH"
#define DEF_STR_ID_SET_TCA_OSC_TX_LOW       "SET_TCA_OSC_TX_LOW"
#define DEF_STR_ID_SET_TCA_OSC_TX_HIGH      "SET_TCA_OSC_TX_HIGH"
#define DEF_STR_ID_SET_TCA_OSC_RX_LOW       "SET_TCA_OSC_RX_LOW"
#define DEF_STR_ID_SET_TCA_OSC_RX_HIGH      "SET_TCA_OSC_RX_HIGH"
#define DEF_STR_ID_SET_TCA_OSC_ADD_LOW      "SET_TCA_OSC_ADD_LOW"
#define DEF_STR_ID_SET_TCA_OSC_ADD_HIGH     "SET_TCA_OSC_ADD_HIGH"
#define DEF_STR_ID_SET_TCA_OSC_DROP_LOW     "SET_TCA_OSC_DROP_LOW"
#define DEF_STR_ID_SET_TCA_OSC_DROP_HIGH    "SET_TCA_OSC_DROP_HIGH"
#define DEF_STR_ID_SET_TCA_SPANLOSS_VARIATION_LOW   "SET_TCA_SPANLOSS_VARIATION_LOW"
#define DEF_STR_ID_SET_TCA_SPANLOSS_VARIATION_HIGH  "SET_TCA_SPANLOSS_VARIATION_HIGH"

/* #define USER_LOG_MSG "{  \"id\": \"%s\",  \"resource\": \"%s\",  \"text\": \"%s\",  \"time-created\": \"%s\",  \"severity\": \"%s\",  \"type-id\": \"%s\"}" */

/* #define USER_LOG_MSG "{\"id\":\"%s\", \"resource\":\"%s\", \"text\":\"%s\", \"time-created\":\"%s\", \"severity\":\"%s\", \"type-id\":\"%s\"}" */
#define USER_LOG_MSG "{\"id\":\"%s\", \"resource\":\"%s\", \"time-created\":\"%s\", \"severity\":\"%s\", \"type-id\":\"%s\", \"text\":\"%s\"}"
#define USER_LOG_MSG_MIN "{\"id\":\"\", \"resource\":\"\", \"time-created\":\"\", \"severity\":\"\", \"type-id\":\"\", \"text\":\"\"}"


int DSC_LogInit();

int DSC_WriteUserLog(const char *pcResouce, const char *pcTypeId, char *pcText);

#ifdef __cplusplus
}
#endif


