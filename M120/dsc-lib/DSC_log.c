/*print utility for testing*/
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <syslog.h>

#include "sysrepo.h"

#include "DSC_common.h"
#include "DSC_log.h"
#include "DSC_system.h"

const char *
ev_to_str(sr_event_t ev)
{
    switch (ev) {
    /* case SR_EV_VERIFY: */
    case SR_EV_CHANGE:
        return "change";
    /* case SR_EV_APPLY: */
    case SR_EV_DONE:
        return "done";
    case SR_EV_ENABLED:
        return "enabled";
    case SR_EV_ABORT:
    default:
        return "abort";
    }
}

void
dsc_print_value(sr_val_t *value)
{
    printf("%s ", value->xpath);

    switch (value->type) {
    case SR_CONTAINER_T:
    case SR_CONTAINER_PRESENCE_T:
        printf("(container)\n");
        break;
    case SR_LIST_T:
        printf("(list instance)\n");
        break;
    case SR_STRING_T:
        printf("= %s\n", value->data.string_val);
        break;
    case SR_BOOL_T:
        printf("= %s\n", value->data.bool_val ? "true" : "false");
        break;
    case SR_ENUM_T:
        printf("= %s\n", value->data.enum_val);
        break;
    case SR_DECIMAL64_T:
        printf("= %g\n", value->data.decimal64_val);
        break;
    case SR_INT8_T:
        printf("= %" PRId8 "\n", value->data.int8_val);
        break;
    case SR_INT16_T:
        printf("= %" PRId16 "\n", value->data.int16_val);
        break;
    case SR_INT32_T:
        printf("= %" PRId32 "\n", value->data.int32_val);
        break;
    case SR_INT64_T:
        printf("= %" PRId64 "\n", value->data.int64_val);
        break;
    case SR_UINT8_T:
        printf("= %" PRIu8 "\n", value->data.uint8_val);
        break;
    case SR_UINT16_T:
        printf("= %" PRIu16 "\n", value->data.uint16_val);
        break;
    case SR_UINT32_T:
        printf("= %" PRIu32 "\n", value->data.uint32_val);
        break;
    case SR_UINT64_T:
        printf("= %" PRIu64 "\n", value->data.uint64_val);
        break;
    case SR_IDENTITYREF_T:
        printf("= %s\n", value->data.identityref_val);
        break;
    case SR_BITS_T:
        printf("= %s\n", value->data.bits_val);
        break;
    case SR_BINARY_T:
        printf("= %s\n", value->data.binary_val);
        break;
    default:
        printf("(unprintable)\n");
    }
}


void
print_change(sr_change_oper_t op, sr_val_t *old_val, sr_val_t *new_val)
{
    switch (op) {
    case SR_OP_CREATED:
        if (NULL != new_val) {
            printf("CREATED: ");
            dsc_print_value(new_val);
        }
        break;
    case SR_OP_DELETED:
        if (NULL != old_val) {
            printf("DELETED: ");
            dsc_print_value(old_val);
        }
        break;
    case SR_OP_MODIFIED:
        if (NULL != old_val && NULL != new_val) {
            printf("MODIFIED: ");
            printf("old value");
            dsc_print_value(old_val);
            printf("new value");
            dsc_print_value(new_val);
        }
        break;
    case SR_OP_MOVED:
        if (NULL != new_val) {
            printf("MOVED: %s after %s", new_val->xpath, NULL != old_val ? old_val->xpath : NULL);
        }
        break;
    }
}

void
print_current_config(sr_session_ctx_t *session, const char *module_name)
{
    sr_val_t *values = NULL;
    size_t count = 0;
    int rc = SR_ERR_OK;
    char select_xpath[OPLK_PATH_MAX_LEN];
    snprintf(select_xpath, OPLK_PATH_MAX_LEN, "/%s:*//*", module_name);

    rc = sr_get_items(session, select_xpath, 0, 0, &values, &count);
    if (SR_ERR_OK != rc) {
        printf("Error by get_items: %s", sr_strerror(rc));
        return;
    }
    for (size_t i = 0; i < count; i++) {
        dsc_print_value(&values[i]);
    }
    sr_free_values(values, count);
}

int
DSC_LogInit(const char *appName)
{
    sr_log_syslog(appName, SR_LL_WRN);
    openlog("APPS", LOG_PID, LOG_SYSLOG);

    return SR_ERR_OK;
}

int
DSC_WriteUserLog(const char *pcResouce, const char *pcTypeId, char *pcText)
{
    int rc = SR_ERR_OK;
    time_t tNow;
    char acId[128] = {0};
    char acTimeStr[128] = {0};
    char acUserLogStr[512] = {0};
    char acText[512] = {0};
    int iLen = 0;
    int iTextValidLen = 0;
#define DEF_MEIWU_LOG_MSG_LEN                 (384)
    if (NULL == pcResouce || NULL == pcTypeId || NULL == pcText) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(acId, 0, sizeof(acId));
    snprintf(acId, sizeof(acId), "%s_%s", pcResouce, pcTypeId);

    tNow = time(NULL);
    memset(acTimeStr, 0, sizeof(acTimeStr));
    UTIL_TimeToStr(tNow, acTimeStr);
    /* UTIL_GetSysTimeLocalMicroSecond(acTimeStr, sizeof(acTimeStr) - 1); */

    memset(acUserLogStr, 0, sizeof(acUserLogStr));
    rc = snprintf(acUserLogStr, sizeof(acUserLogStr) - 1,
                  USER_LOG_MSG,
                  acId,
                  pcResouce,
                  acTimeStr,
                  "NA",
                  pcTypeId,
                  pcText);
    if (rc < 0) {
        LOG_WRITE(EN_LOG_DEBUG, "directive output may be truncated");
    }

    /* exception handling */
    if ( DEF_MEIWU_LOG_MSG_LEN - 1 < strlen(acUserLogStr)) {
        /* LOG_WRITE(EN_LOG_DEBUG, "Text is too longm,len:%d,%s",strlen(pcText),pcText); */
        /* LOG_WRITE(EN_LOG_DEBUG, "Text is too longm,jsonlen:%d,%s",strlen(acUserLogStr),acUserLogStr); */
        iLen = strlen(USER_LOG_MSG_MIN) + strlen(acId) + strlen(pcResouce) + strlen(acTimeStr) + strlen("NA") + strlen(pcTypeId);
        iTextValidLen = DEF_MEIWU_LOG_MSG_LEN - 1 - iLen - 1;
        strncpy(acText, pcText, iTextValidLen);
        memset(acUserLogStr, 0, sizeof(acUserLogStr));
        rc = snprintf(acUserLogStr, sizeof(acUserLogStr) - 1,
                      USER_LOG_MSG,
                      acId,
                      pcResouce,
                      acTimeStr,
                      "NA",
                      pcTypeId,
                      acText);
        if (rc < 0) {
            LOG_WRITE(EN_LOG_DEBUG, "directive output may be truncated");
        }
        LOG_WRITE(EN_LOG_DEBUG, "exception handling acUserLogStr,len:%lu,%s", strlen(acUserLogStr), acUserLogStr);

    }

    USER_LOG(EN_LOG_NOTICE, acUserLogStr);

    return SR_ERR_OK;
cleanup:
    return rc;
}

