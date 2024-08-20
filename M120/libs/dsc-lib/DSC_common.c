/**
 * Copyright (c) Oplink Communications, LLC
 * fandy <Dongyang.Fan@Molex.com>
 * Jun. 28th, 2016
 */
#include <signal.h>
#include <limits.h>
#include <ctype.h>


#include "DSC_common.h"
#include "DSC_system.h"
#include "DSC_waveplan.h"
#include "DSC_tca.h"
#include "DSC_aaa.h"
#include "DSC_ocsystem.h"
#include "DSC_syslog.h"

static sr_conn_ctx_t *m_pstConn = NULL;
static sr_session_ctx_t *m_pstStartupSess = NULL;
static sr_session_ctx_t *m_pstRunningSess = NULL;
static sr_session_ctx_t *m_pstOperSess = NULL;
static bool m_bUsedRunning = false;
static bool m_bIsDeamon = false;
uint32_t timeout_ms = 0;
uint32_t priority = 0;

int
sr_get_items_vargs(sr_session_ctx_t *session, sr_val_t **values, size_t *value_cnt, const char *xpath_fmt, ...)
{
    va_list ap;
    char buf[512];
    int rc = 0;

    if (strlen(xpath_fmt) > 512)
        return ERR_NOMEM;

    va_start(ap, xpath_fmt);
    vsnprintf(buf, sizeof(buf), xpath_fmt, ap);
    va_end(ap);

    rc = get_items(session, buf, values, value_cnt);
    if (rc == SR_ERR_NOT_FOUND) {
        *values = NULL;
        *value_cnt = 0;
        return SR_ERR_OK;
    } else if (rc != SR_ERR_OK) {
        *values = NULL;
        *value_cnt = 0;
        return rc;
    } else {
        return rc;
    }
}

int
sr_get_item_vargs(sr_session_ctx_t *session, sr_val_t **value, const char *xpath_fmt, ...)
{
    va_list ap;
    char buf[512];
    int rc = 0;

    if (strlen(xpath_fmt) > 512)
        return ERR_NOMEM;

    va_start(ap, xpath_fmt);
    vsnprintf(buf, sizeof(buf), xpath_fmt, ap);
    va_end(ap);

    rc = sr_get_item(session, buf, timeout_ms, value);
    if (rc == SR_ERR_NOT_FOUND) {
        *value = NULL;
        return SR_ERR_OK;
    } else if (rc != SR_ERR_OK) {
        *value = NULL;
        return rc;
    } else {
        return rc;
    }
}

sr_val_t *
sr_values_find_by_xpath(sr_val_t *values, size_t values_count, const char *xpath)
{
    int i = 0;

    if (values == NULL || values_count <= 0)
        return NULL;

    for (i = 0; i < values_count; i++) {
        if (strcmp(values[i].xpath, xpath) == 0)
            return &values[i];
    }

    return NULL;
}

sr_val_t *
sr_values_find_by_xpath_vargs(sr_val_t *values, size_t values_count, const char *xpath_fmt, ...)
{
    va_list ap;
    char buf[512];

    if (strlen(xpath_fmt) > 512)
        return NULL;

    va_start(ap, xpath_fmt);
    vsnprintf(buf, sizeof(buf), xpath_fmt, ap);
    va_end(ap);

    return sr_values_find_by_xpath(values, values_count, buf);
}

int
set_string_val_by_xpath(sr_val_t *input, const char *xpath, const char *string_val)
{
    int rc = SR_ERR_OK;

    if (NULL == input || NULL == xpath || NULL == string_val) {
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_val_set_xpath(input, xpath);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    /* input->type = SR_STRING_T; */
    /* input->data.string_val = strdup(pcStr); */

    rc = sr_val_set_str_data(input, SR_STRING_T, string_val);

    return rc;
}

int
set_uint32_val_by_xpath(sr_val_t *input, const char *xpath, uint32_t uiValue)
{
    int rc = SR_ERR_OK;

    if (NULL == input || NULL == xpath) {
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_val_set_xpath(input, xpath);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input->type = SR_UINT32_T;
    input->data.uint32_val = uiValue;

    return rc;
}

int
set_int32_val_by_xpath(sr_val_t *input, const char *xpath, int32_t iValue)
{
    int rc = SR_ERR_OK;

    if (NULL == input || NULL == xpath) {
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_val_set_xpath(input, xpath);
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input->type = SR_INT32_T;
    input->data.int32_val = iValue;

    return rc;
}

int
get_string_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        strncpy(lhs, (_v ? _v->data.string_val : SR_DEFAULT_STRING_VAL), size);
        sr_free_val(_v);
    }
    return rc;
}

int
get_string_item_log_trace(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_TRACE, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        strncpy(lhs, (_v ? _v->data.string_val : SR_DEFAULT_STRING_VAL), size);
        sr_free_val(_v);
    }
    return rc;
}

int
get_identityref_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        strncpy(lhs, (_v ? _v->data.identityref_val : SR_DEFAULT_STRING_VAL), size);
        sr_free_val(_v);
    }
    return rc;
}


int
get_binary_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        memset(lhs, 0, size);
        memcpy(lhs, _v->data.binary_val, size);
        sr_free_val(_v);
    }
    return rc;
}



int
get_uint32_item(sr_session_ctx_t *session, const char *xpath, uint32_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.uint32_val : 0;
        sr_free_val(_v);
    }
    return rc;
}

int
get_int32_item(sr_session_ctx_t *session, const char *xpath, int32_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.int32_val : 0;
        sr_free_val(_v);
    }
    return rc;
}



int
get_uint16_item(sr_session_ctx_t *session, const char *xpath, uint16_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.uint16_val : 0;
        sr_free_val(_v);
    }
    return rc;
}

int
get_int16_item(sr_session_ctx_t *session, const char *xpath, int16_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.int16_val : 0;
        sr_free_val(_v);
    }
    return rc;
}


int
get_enum_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        strncpy(lhs, (_v ? _v->data.enum_val : SR_DEFAULT_STRING_VAL), size);
        sr_free_val(_v);
    }
    return rc;
}

int
get_enum_item_log_trace(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_TRACE, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        strncpy(lhs, (_v ? _v->data.enum_val : SR_DEFAULT_STRING_VAL), size);
        sr_free_val(_v);
    }
    return rc;
}

int
get_uint8_item(sr_session_ctx_t *session, const char *xpath, uint8_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.int8_val : 0;
        sr_free_val(_v);
    }
    return rc;
}

int
get_uint64_item(sr_session_ctx_t *session, const char *xpath, uint64_t *lhs)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.uint64_val : 0;
        sr_free_val(_v);
    }
    return rc;
}

int
get_decimal64_item(sr_session_ctx_t *session, const char *xpath, double *lhs)
{

    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *lhs = _v ? _v->data.decimal64_val : 0;
        sr_free_val(_v);
    }
    return rc;
}


int
get_bool_item(sr_session_ctx_t *session, const char *xpath, bool *state)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *state = _v ? _v->data.bool_val : false;
        sr_free_val(_v);
    }

    return rc;
}

int
get_bool_item_log_trace(sr_session_ctx_t *session, const char *xpath, bool *state)
{
    int rc = 0;
    sr_val_t *_v;

    rc = sr_get_item(session, xpath, timeout_ms, &_v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_TRACE, "sr_get_item: %s %s", xpath, sr_strerror(rc));
    } else {
        *state = _v ? _v->data.bool_val : false;
        sr_free_val(_v);
    }

    return rc;
}

int
get_items(sr_session_ctx_t *session, const char *xpath, sr_val_t **values, size_t *count)
{
    int rc = 0;

    rc = sr_get_items(session, xpath, timeout_ms, SR_OPER_DEFAULT, values, count);
    /* ? what options */
    return rc;
}

int
get_subtree(sr_session_ctx_t *session, const char *xpath, uint32_t timeout_ms,
            sr_data_t **subtree)
{
    int rc = 0;

    rc = sr_get_subtree(session, xpath, timeout_ms, subtree);
    return rc;
}

int
get_subtrees(sr_session_ctx_t *session, const char *xpath, uint32_t timeout_ms,
             sr_get_options_t opt, sr_data_t **data)
{
    int rc = 0;

    rc = sr_get_data(session, xpath, 0, timeout_ms, opt, data);
    return rc;
}

int
set_string_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t _v = {0};
    char acBuf[1024];

    if (NULL == session || NULL == xpath || NULL == lhs) {
        return SR_ERR_INVAL_ARG;
    }

    if (0 == strlen(xpath)) {
        return SR_ERR_INVAL_ARG;
    }

    /* check length */
    if ((size >= sizeof(acBuf)) || (strlen(lhs) > size)) {
        LOG_WRITE(EN_LOG_ERROR, "string length overflow: %zu %zu", strlen(lhs), size);
        return SR_ERR_INVAL_ARG;
    }

    /* skip if not ascii */
    memset(acBuf, 0, sizeof(acBuf));
    int j = 0;
    for (int i = 0; i < strlen(lhs); i++) {
        if (!isprint(lhs[i])) {  /*if (!isascii(lhs[i])) {*/
            LOG_WRITE(EN_LOG_ERROR, "not ascii char: 0x%02X", lhs[i]);
            continue;
        }

        acBuf[j] = lhs[i];
        j++;
    }

    _v.type = SR_STRING_T;
    _v.data.string_val = acBuf;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_identityref_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_IDENTITYREF_T;
    _v.data.identityref_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_binary_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_BINARY_T;
    _v.data.binary_val = lhs;
    /* memcpy(_v.data.binary_val, lhs, size); */

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}



int
set_uint32_item(sr_session_ctx_t *session, const char *xpath, uint32_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_UINT32_T;
    _v.data.uint32_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }

    return rc;
}

int
set_int32_item(sr_session_ctx_t *session, const char *xpath, int32_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_INT32_T;
    _v.data.int32_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_uint16_item(sr_session_ctx_t *session, const char *xpath, uint16_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};


    _v.type = SR_UINT16_T;
    _v.data.uint16_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_int16_item(sr_session_ctx_t *session, const char *xpath, int16_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};


    _v.type = SR_INT16_T;
    _v.data.int16_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_uint8_item(sr_session_ctx_t *session, const char *xpath, uint8_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_UINT8_T;
    _v.data.uint8_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_uint64_item(sr_session_ctx_t *session, const char *xpath, uint64_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_UINT64_T;
    _v.data.uint64_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}


/* enum_val is a string */
int
set_enum_item(sr_session_ctx_t *session, const char *xpath, char *lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_ENUM_T;
    _v.data.enum_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}

int
set_bool_item(sr_session_ctx_t *session, const char *xpath, bool state)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_BOOL_T;
    _v.data.bool_val = state;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }
    return rc;
}
int
set_bits_item(sr_session_ctx_t *session, const char *xpath, char *lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_BITS_T;
    _v.data.bits_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s bits: %s %s", xpath, lhs, sr_strerror(rc));
    }
    return rc;
}

int
set_decimal64_item(sr_session_ctx_t *session, const char *xpath, double lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_DECIMAL64_T;
    _v.data.decimal64_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s decimal: %.02f, %s", xpath, lhs, sr_strerror(rc));
    }
    return rc;
}

int
set_uint32_item_strict(sr_session_ctx_t *session, const char *xpath, uint32_t lhs)
{
    int rc = 0;
    sr_val_t _v = {0};

    _v.type = SR_UINT32_T;
    _v.data.uint32_val = lhs;

    rc = sr_set_item(session, xpath, &_v, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", xpath, sr_strerror(rc));
    }

    return rc;
}

int
del_item(sr_session_ctx_t *session, const char *xpath)
{
    int rc = 0;

    rc = sr_delete_item(session, xpath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete is error,%s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_RpcSend(sr_session_ctx_t *pstSess, const char *pcXpath,
            sr_val_t *pstInput, size_t nInputCnt,
            sr_val_t **ppstOutput, size_t *pnOutputCnt)
{
    int rc = 0;

    rc = sr_rpc_send(pstSess, pcXpath, pstInput, nInputCnt, timeout_ms, ppstOutput, pnOutputCnt);
    if (SR_ERR_OK != rc) {
#if 0
        if (pstSess->err_info != NULL) {
            if ((pstSess->err_info->err_count > 0) && (pstSess->err_info->err != NULL)) {
                LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send error: %s, message: %s",
                          sr_strerror(pstSess->err_info->err->err_code),
                          pstSess->err_info->err->message);
            }
        }
#endif
        sr_discard_changes(pstSess);
    }
    return rc;
}


int
DSC_Commit(sr_session_ctx_t *pstSess)
{
    int rc = 0;

    if (NULL == pstSess) {
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_apply_changes(pstSess, timeout_ms);
    if (SR_ERR_OK != rc) {
#if 0
        if (pstSess->err_info != NULL) {
            if ((pstSess->err_info->err_count > 0) && (pstSess->err_info->err != NULL)) {
                LOG_WRITE(EN_LOG_ERROR, "sr_rpc_send error: %s, message: %s",
                          sr_strerror(pstSess->err_info->err->err_code),
                          pstSess->err_info->err->message);
            }
        }
#endif
        sr_discard_changes(pstSess);
    }

    return rc;
}

int
DSC_GetItems(sr_session_ctx_t *sess, char *path, ANONYMOUS_ST *pstAnonymous, size_t size)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    int found = 0;
    sr_val_t *values;
    size_t values_count;

    if (NULL == sess || NULL == path || NULL == pstAnonymous) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(sess, path, &values, &values_count);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s %s", path, sr_strerror(rc));
        return rc;
    }

    for (i = 0; i < values_count; i++) {
        for (j = 0; j < size / sizeof(ANONYMOUS_ST); j++) {
            if (strcmp(pstAnonymous[j].xpath, values[i].xpath) == 0) {
                found = 1;
                break;
            }

        }

        if (1 != found) {
            continue;
        }

        switch (pstAnonymous[j].type) {
        case SR_BINARY_T:
            memcpy(pstAnonymous[j].buff, values[i].data.binary_val, pstAnonymous[j].buff_size);
            break;

        case SR_STRING_T:
            memcpy(pstAnonymous[j].buff, values[i].data.string_val, pstAnonymous[j].buff_size);
            break;

        case SR_UINT16_T:
            *((uint16_t *)pstAnonymous[j].buff) = values[i].data.uint16_val;
            break;

        case SR_UINT8_T:
            *((uint8_t *)pstAnonymous[j].buff) = values[i].data.uint8_val;
            break;

        case SR_UINT32_T:
            *((uint32_t *)pstAnonymous[j].buff) = values[i].data.uint32_val;
            break;

        case SR_INT32_T:
            *((int32_t *)pstAnonymous[j].buff) = values[i].data.int32_val;
            break;

        case SR_DECIMAL64_T:
            *((double *)pstAnonymous[j].buff) = values[i].data.decimal64_val;
            break;

        case SR_ENUM_T:
            memcpy(pstAnonymous[j].buff, values[i].data.enum_val, pstAnonymous[j].buff_size);
            break;

        case SR_IDENTITYREF_T:
            memcpy(pstAnonymous[j].buff, values[i].data.identityref_val, pstAnonymous[j].buff_size);
            break;

        case SR_BOOL_T:
            *((bool *)pstAnonymous[j].buff) = values[i].data.bool_val;
            break;

        case SR_UINT64_T:
            *((uint64_t *)pstAnonymous[j].buff) = values[i].data.uint64_val;
            break;

        default:
            break;
        }

        found = 0;
    }

    sr_free_values(values, values_count);

    return rc;
}

int
DSC_ProcessRPCResponse(sr_val_t *output, int output_cnt)
{
    int rc = SR_ERR_OK;

    /* print output values */
    /* printf("\n>>> Received an RPC response:\n\n"); */
    /* for (size_t i = 0; i < output_cnt; ++i) { */
    /*    sr_print_val(output+i); */
    /* } */
    /* printf("\n"); */

    /* expect at least 2 return value */
    if (output_cnt < 2) {
        rc = SR_ERR_OPERATION_FAILED;
    } else if (strcmp(output[0].data.enum_val, RPC_STATUS_INPROGRESS) == 0) {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[0].xpath, output[1].data.string_val);
        rc = SR_ERR_EXISTS;
    } else if (strcmp(output[0].data.enum_val, RPC_STATUS_SUCCESS) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "rpc %s, failed: %s", output[0].xpath, output[1].data.string_val);
        rc = SR_ERR_OPERATION_FAILED;
    }


    /* don't forget to de-allocate the output values */
    sr_free_values(output, output_cnt);
    return rc;
}

char *
__sprintfInt(char *pcBuf, char *pcStr, int i)
{
    sprintf(pcBuf, pcStr, i);
    return pcBuf;
}

char *
__sprintfStr(char *pcBuf, char *pcStr, char *pcSrc)
{
    sprintf(pcBuf, pcStr, pcSrc);
    return pcBuf;
}

char *
__sprintfStr2(char *pcBuf, char *pcStr, char *pcSrc1, char *pcSrc2)
{
    sprintf(pcBuf, pcStr, pcSrc1, pcSrc2);
    return pcBuf;
}

int
DSC_GetKeyValue(char *pcXpath, char *pcListName, char *pcKeyStr, int iLen, char *pcKeyValue)
{
    char *pcKeyTmp = NULL;
    sr_xpath_ctx_t stState = {0};

    if (NULL == pcXpath || NULL == pcListName || NULL == pcKeyStr || NULL == pcKeyValue) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetKeyValue input ptr is null!");
        return SR_ERR_INVAL_ARG;
    }

    pcKeyTmp = sr_xpath_key_value(pcXpath, pcListName, pcKeyStr, &stState);
    if (NULL == pcKeyTmp) {
        return SR_ERR_OPERATION_FAILED;
    }
    strncpy(pcKeyValue, pcKeyTmp, iLen - 1);
    sr_xpath_recover(&stState);

    return SR_ERR_OK;
}

int
DSC_DelNode(sr_session_ctx_t *pstSess, char *pcXPath)
{
    int rc = SR_ERR_OK;


    if (NULL == pstSess || NULL == pcXPath) {
        LOG_WRITE(EN_LOG_ERROR, "input is null.");
        return SR_ERR_INVAL_ARG;
    }

    rc = sr_delete_item(pstSess, pcXPath, SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "delete is error,%s", sr_strerror(rc));
        goto END_LABEL;
    }

    rc = DSC_Commit(pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "commit is error,%s", sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:

    return rc;
}

unsigned int
DSC_GetTreeLeaf(sr_node_t *pstTree, sr_node_t **ppstLeafArray)
{
    unsigned int uiCnt = 0;
    unsigned int uiChildLeafCnt = 0;
    sr_node_t *pstChildNode = NULL;

    if (NULL == pstTree) {
        LOG_WRITE(EN_LOG_ERROR, "pstTree is null");
        return 0;
    }

    pstChildNode = lyd_node_to_inner(pstTree)->child;

    /* check if this node is a leaf */
    if (NULL == pstChildNode) {
        *ppstLeafArray = pstTree;

        uiCnt++;

        return uiCnt;
    }

    /* find out all leaves */
    while (pstChildNode != NULL) {
        /* find out all child leaves */
        if (pstChildNode->schema->nodetype & (LYS_LEAF | LYS_LEAFLIST)) {
            *ppstLeafArray = pstChildNode;
            ppstLeafArray += 1;
            uiCnt++;
        } else {
            uiChildLeafCnt = DSC_GetTreeLeaf(pstChildNode, ppstLeafArray);
            ppstLeafArray += uiChildLeafCnt;
            uiCnt += uiChildLeafCnt;
        }

        pstChildNode = pstChildNode->next;

    }

    return uiCnt;

}

/***FUNC+******************************************************************************************/
/* Name   : DSC_IterateTreeLeaf                                                                   */
/* Descrp : Interate via all elements in a tree                                                   */
/* Input  : pstRoot -- A pointer of root node                                                     */
/* Output : pstOut -- A pointer of array                                                          */
/* Input  : nMaxCnt -- pstOut array max count                                                     */
/* Return : The leaf count of tree                                                                */
/***FUNC-******************************************************************************************/
size_t
DSC_IterateTreeLeaf(const sr_node_t *pstRoot, sr_node_term_t *pstOut, size_t nMaxCnt)
{
    size_t nIndex = 0;
    struct lyd_node *subroot = NULL;
    struct lyd_node *node = NULL;

    subroot = (struct lyd_node *)pstRoot;

    LYD_TREE_DFS_BEGIN(subroot, node) {
        if (LYS_LEAF == node->schema->nodetype ||
            LYS_LEAFLIST == node->schema->nodetype) {
            memcpy(pstOut + nIndex, node, sizeof(sr_node_term_t));
            nIndex++;
            if (nIndex >= nMaxCnt) {
                break;
            }
        }
        LYD_TREE_DFS_END(subroot, node)
    }

    return nIndex;
}

/* ----begin----add by guangjung */
int
DSC_ParseValuesByXpath(const sr_val_t *values, size_t values_count, ANONYMOUS_ST *pstAnonymous, size_t size)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    int found = 0;
    size_t items_count = 0;

    if (NULL == values || NULL == pstAnonymous) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return SR_ERR_INVAL_ARG;
    }

    items_count = size / sizeof(ANONYMOUS_ST);
    for (i = 0; i < items_count; i++) {
        for (j = 0; j < values_count; j++) {
            if (0 == strcmp(pstAnonymous[i].xpath, values[j].xpath)) {
                found = 1;
                break;
            }
        }

        if (1 != found) {
            /* rc = SR_ERR_NOT_FOUND; */
            LOG_WRITE(EN_LOG_ERROR, "not find item %s", pstAnonymous[i].xpath);
            continue;
        }

        switch (pstAnonymous[i].type) {
        case SR_BINARY_T:
            memcpy(pstAnonymous[i].buff, values[j].data.binary_val, pstAnonymous[i].buff_size);
            break;

        case SR_STRING_T:
            memcpy(pstAnonymous[i].buff, values[j].data.string_val, pstAnonymous[i].buff_size);
            break;

        case SR_UINT16_T:
            *((uint16_t *)pstAnonymous[i].buff) = values[j].data.uint16_val;
            break;

        case SR_UINT32_T:
            *((uint32_t *)pstAnonymous[i].buff) = values[j].data.uint32_val;
            break;

        case SR_INT32_T:
            *((int32_t *)pstAnonymous[i].buff) = values[j].data.int32_val;
            break;

        case SR_DECIMAL64_T:
            *((double *)pstAnonymous[i].buff) = values[j].data.decimal64_val;
            break;

        case SR_ENUM_T:
            memcpy(pstAnonymous[i].buff, values[j].data.enum_val, pstAnonymous[i].buff_size);
            break;

        case SR_IDENTITYREF_T:
            memcpy(pstAnonymous[i].buff, values[j].data.identityref_val, pstAnonymous[i].buff_size);
            break;

        case SR_BOOL_T:
            *((bool *)pstAnonymous[i].buff) = values[j].data.bool_val;
            break;

        default:
            LOG_WRITE(EN_LOG_ERROR, "not match type(%d). %s", pstAnonymous[i].type, pstAnonymous[i].xpath);
            break;
        }

        found = 0;
    }

    return rc;
}

/* ----end----add by guangjung */

int
DSC_GetNamespace(const char *pcInXpath, char *pcNameSpace, size_t tMaxSize)
{
    int rc = SR_ERR_OK;
    char *pcColonPosition = NULL;
    size_t tCopyLen = 0;

    if (NULL == pcInXpath ||
        NULL == pcNameSpace) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pcColonPosition = strchr(pcInXpath, ':');
    if (pcInXpath[0] != '/' || NULL == pcColonPosition) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    pcInXpath++;
    if ('/' == pcInXpath[0]) {
        pcInXpath++;
    }

    tCopyLen = pcColonPosition - pcInXpath;
    tCopyLen = (tCopyLen < tMaxSize ? tCopyLen : tMaxSize);
    strncpy(pcNameSpace, pcInXpath, tCopyLen);

cleanup:
    return rc;
}

bool
DSC_IsSchemaInstalled(sr_session_ctx_t *pstSess, const char *pcModuleName)
{
    const struct ly_ctx *ly_ctx;
    const struct lys_module *ly_mod = NULL;

    sr_conn_ctx_t *pstConn = NULL;

    pstConn = sr_session_get_connection(pstSess);
    if (NULL == pstConn) {
        return false;
    }

    ly_ctx = sr_acquire_context(pstConn);
    ly_mod = ly_ctx_get_module_implemented(ly_ctx, pcModuleName);
    sr_release_context(pstConn);
    if (NULL == ly_mod) {
        return false;
    }

    return true;
}

/*Note: unintsall may not be performed immediately */
int
DSC_UninstallModule(const char *pcModuleName)
{
    return sr_remove_module(DSC_GetConn(), pcModuleName, 0);
}

int
DSC_PrintVal(sr_session_ctx_t *pstSess, const char *pcXpath)
{
    int rc = SR_ERR_OK;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;

    if (NULL == pstSess || NULL == pcXpath) {
        LOG_WRITE(EN_LOG_ERROR, "pstServers is null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    rc = get_items(pstSess, pcXpath, &pstVal, &nCnt);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get %s error: %s", pcXpath, sr_strerror(rc));
        return SR_ERR_OK;
    }

#if 1
    printf("xpath: %s, nCnt=%lu\n", pcXpath, nCnt);
    for (size_t i = 0; i < nCnt; i++) {
        sr_print_val(pstVal + i);
    }
#endif

    if (pstVal != NULL) {
        sr_free_values(pstVal, nCnt);
        pstVal = NULL;
    }

    return rc;
}

int
DSC_StartAgentComSess(const char *pcAppName, int iTimeout)
{
    int rc = SR_ERR_OK;

    if (NULL == pcAppName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_StartAgentComSess %s", pcAppName);

    /* register log callback for libyang */
    DSC_SetLibyangLogCb();

    if (NULL == m_pstConn) {
        do {
            rc = sr_connect(SR_CONN_DEFAULT, &m_pstConn);
            if (SR_ERR_OK != rc) {
                sleep(1);
            } else {
                break;
            }
        } while (iTimeout-- > 0);

        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_connect error : %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstRunningSess) {
        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstRunningSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstOperSess) {
        rc = sr_session_start(m_pstConn, SR_DS_OPERATIONAL, &m_pstOperSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_StartAgentComSess");
    return rc;
ERR_LABEL:
    DSC_StopSess();
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_StartAgentComSess");
    return rc;
}

int
DSC_StartAgentUserSess(const char *pcAppName, const char *pcUserName, int iTimeout)
{
    int rc = SR_ERR_OK;

    if (NULL == pcAppName || NULL == pcUserName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_StartAgentUserSess %s %s", pcAppName, pcUserName);

    /* register log callback for libyang */
    DSC_SetLibyangLogCb();

    if (NULL != m_pstConn) {
        DSC_StopSess();
    }

    if (NULL == m_pstConn) {
        do {
            rc = sr_connect(SR_CONN_DEFAULT, &m_pstConn);
            /* ? what options */
            if (SR_ERR_OK != rc) {
                sleep(1);
            } else {
                break;
            }
        } while (iTimeout-- > 0);

        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_connect error : %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstRunningSess) {
        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstRunningSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start_user %s error: %s", pcAppName, pcUserName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstOperSess) {
        rc = sr_session_start(m_pstConn, SR_DS_OPERATIONAL, &m_pstOperSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_StartAgentUserSess");
    return rc;
ERR_LABEL:
    DSC_StopSess();
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_StartAgentUserSess");
    return rc;
}

int
DSC_StartDeamonSess(const char *pcAppName, int iTimeoutMs, sr_session_ctx_t **ppstStartupSess, sr_session_ctx_t **ppstRunningSess, sr_session_ctx_t **ppstOperSess)
{
    int rc = SR_ERR_OK;

    if (NULL == pcAppName || NULL == ppstStartupSess || NULL == ppstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_StartDeamonSess %s", pcAppName);

    if (NULL == m_pstConn) {
        do {
            rc = sr_connect(SR_CONN_DEFAULT, &m_pstConn);
            /* ? what options */
            if (SR_ERR_OK != rc) {
                sleep(1);
            } else {
                break;
            }
        } while (iTimeoutMs-- > 0);

        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_connect error : %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstStartupSess) {
        rc = sr_session_start(m_pstConn, SR_DS_STARTUP, &m_pstStartupSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start startup error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    if (NULL == m_pstRunningSess) {
        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstRunningSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start running error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }

    }

    if (NULL == m_pstOperSess) {
        rc = sr_session_start(m_pstConn, SR_DS_OPERATIONAL, &m_pstOperSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s sr_session_start operational error: %s", pcAppName, sr_strerror(rc));
            goto ERR_LABEL;
        }
    }

    *ppstStartupSess = m_pstStartupSess;
    *ppstRunningSess = m_pstRunningSess;
    *ppstOperSess = m_pstOperSess;
    m_bIsDeamon = true;
    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_StartDeamonSess");

    DSC_SubLogLevel(m_pstRunningSess);

    return rc;
ERR_LABEL:
    DSC_StopSess();
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_StartDeamonSess");
    return rc;
}

/* only for deamon start */
int
DSC_SwitchToRunningSess()
{
    int rc = SR_ERR_OK;

    if (m_bIsDeamon) {
#if 0
        if (NULL != m_pstRunningSess) {
            sr_session_stop(m_pstRunningSess);
            m_pstRunningSess = NULL;
        }

        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstRunningSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "restart running sr_session_start error: %s", sr_strerror(rc));
        }
#endif
        m_bUsedRunning = true;
    }

    return rc;
}

sr_session_ctx_t *
DSC_GetStartupSession()
{
    return m_pstStartupSess;
}

sr_session_ctx_t *
DSC_GetRunningSession()
{
    return m_pstRunningSess;
}

sr_session_ctx_t *
DSC_GetOperSession()
{
    return m_pstOperSess;
}

/* only for deamon end */

sr_session_ctx_t *
DSC_GetSession()
{
    sr_session_ctx_t *pstSess = NULL;

    /* Agent only operational session */
    if (!m_bIsDeamon) {
        pstSess = m_pstOperSess;
        goto cleanup;
    }

    /* Deamon */
    if (m_bUsedRunning) {
        pstSess = m_pstRunningSess;
    } else {
        pstSess = m_pstStartupSess;
    }

cleanup:
    return pstSess;
}

sr_conn_ctx_t *
DSC_GetConn()
{
    return m_pstConn;
}

int
DSC_ResetSession()
{
    int rc = SR_ERR_OK;

    if (NULL == m_pstConn) {
        return SR_ERR_INVAL_ARG;
    }

    if (NULL != m_pstRunningSess) {
        sr_session_stop(m_pstRunningSess);
        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstRunningSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "restart running sr_session_start error: %s", sr_strerror(rc));
        }
    }

    if (NULL != m_pstStartupSess) {
        sr_session_stop(m_pstStartupSess);
        rc = sr_session_start(m_pstConn, SR_DS_RUNNING, &m_pstStartupSess);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "restart startup sr_session_start error: %s", sr_strerror(rc));
        }
    }

    return rc;
}

int
DSC_StopSess()
{
    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_StopSess");

    if (NULL != m_pstStartupSess) {
        sr_session_stop(m_pstStartupSess);
        m_pstStartupSess = NULL;
    }

    if (NULL != m_pstRunningSess) {
        sr_session_stop(m_pstRunningSess);
        m_pstRunningSess = NULL;
    }

    if (NULL != m_pstOperSess) {
        sr_session_stop(m_pstOperSess);
        m_pstOperSess = NULL;
    }

    if (NULL != m_pstConn) {
        sr_disconnect(m_pstConn);
        m_pstConn = NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_StopSess");

    return SR_ERR_OK;
}


static sr_subscription_ctx_t *m_pstConfSubscription = NULL;
static sr_subscription_ctx_t *m_pstRpcSubscription = NULL;
static sr_subscription_ctx_t *m_pstStateSubscription = NULL;

int
DSC_RpcSubscribe(const char *pcXpath, sr_rpc_cb fCallback, void *pvPrivateCtx)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == fCallback || NULL == m_pstStartupSess || NULL == m_pstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " ++ DSC_RpcSubscribe %s", pcXpath);

    rc = sr_rpc_subscribe(m_pstRunningSess, pcXpath, fCallback,
                          (void *)pvPrivateCtx, priority, 0, &m_pstRpcSubscription);
    /* ? PrivateCtx, SUBSCR_CTX_REUSE */
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_rpc_subscribe: %s: %s", pcXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " -- DSC_RpcSubscribe");

    return rc;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_RpcSubscribe");
    return rc;
}

int
DSC_GetItemsSubscribe(const char *module, const char *pcXpath, sr_oper_get_items_cb fCallback, void *pvPrivateCtx)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == fCallback || NULL == m_pstStartupSess || NULL == m_pstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " ++ DSC_GetItemsSubscribe %s", pcXpath);

    rc = sr_oper_get_subscribe(m_pstRunningSess, module, pcXpath, fCallback,
                               pvPrivateCtx, 0 | SR_SUBSCR_OPER_MERGE, &m_pstStateSubscription);
    /* ? REUSE */
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_dp_get_items_subscribe: %s: %s", pcXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " -- DSC_GetItemsSubscribe");

    return rc;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_GetItemsSubscribe");
    return rc;
}

int
DSC_ChangeSubscribe(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == fCallback || NULL == m_pstStartupSess || NULL == m_pstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " ++ DSC_ChangeSubscribe %s", pcXpath);

    rc = sr_module_change_subscribe(m_pstRunningSess, module, pcXpath, fCallback,
                                    (void *)pvPrivateCtx, priority,
                                    0, &m_pstConfSubscription);
    /* ? PrivateCtx, SUBSCR_CTX_REUSE */
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error conf subscribe %s, %s", pcXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " -- DSC_ChangeSubscribe");

    return rc;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_ChangeSubscribe");
    return rc;
}

int
DSC_ChangeSubscribePassive(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == fCallback || NULL == m_pstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " ++ DSC_ChangeSubscribePassive %s", pcXpath);

    rc = sr_module_change_subscribe(m_pstRunningSess, module, pcXpath, fCallback, NULL, 0,
                                    SR_SUBSCR_PASSIVE | SR_SUBSCR_DONE_ONLY, &m_pstConfSubscription);
    /* ? what options */
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error conf subscribe %s, %s", pcXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " -- DSC_ChangeSubscribePassive");

    return rc;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_ChangeSubscribePassive");
    return rc;
}

int
DSC_ModuleChangeSubscribe(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx)
{
    int rc = SR_ERR_OK;

    if (NULL == pcXpath || NULL == fCallback || NULL == m_pstStartupSess || NULL == m_pstRunningSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " ++ DSC_ModuleChangeSubscribe %s", pcXpath);

    rc = sr_module_change_subscribe(m_pstRunningSess,
                                    module,
                                    pcXpath,
                                    fCallback,
                                    (void *)m_pstRunningSess, 0,
                                    0,
                                    &m_pstConfSubscription);
    /* ? what options */
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error module conf subscribe %s, %s", pcXpath, sr_strerror(rc));
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_TRACE, " -- DSC_ModuleChangeSubscribe");

    return rc;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, " ** DSC_ModuleChangeSubscribe");
    return rc;
}

int
DSC_UnSubscribe()
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_UnSubscribe");

    if (NULL != m_pstStartupSess && NULL != m_pstConfSubscription) {
        rc = sr_unsubscribe(m_pstConfSubscription);
        m_pstConfSubscription = NULL;
    }

    if (NULL != m_pstStartupSess && NULL != m_pstRpcSubscription) {
        rc = sr_unsubscribe(m_pstRpcSubscription);
        m_pstRpcSubscription = NULL;
    }

    if (NULL != m_pstRunningSess && NULL != m_pstStateSubscription) {
        rc = sr_unsubscribe(m_pstStateSubscription);
        m_pstStateSubscription = NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_UnSubscribe");

    return rc;
}

int
DSC_RpcCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt)
{
    int rc = SR_ERR_OK;
    char acRespXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == xpath ||
        NULL == status ||
        NULL == message ||
        NULL == output ||
        NULL == output_cnt) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto cleanup;
    }

    rc = sr_new_values(2, output);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_new_values: %s", sr_strerror(rc));
        goto cleanup;
    }

    memset(acRespXpath, 0x00, sizeof(acRespXpath));
    snprintf(acRespXpath, OPLK_PATH_MAX_LEN, "%s/status", xpath);
    rc = sr_val_set_xpath(&(*output)[0], acRespXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acRespXpath, sr_strerror(rc));
        goto cleanup;
    }
    (*output)[0].type = SR_ENUM_T;
    (*output)[0].data.enum_val = strdup((char *)status);

    memset(acRespXpath, 0x00, sizeof(acRespXpath));
    snprintf(acRespXpath, OPLK_PATH_MAX_LEN, "%s/status-message", xpath);
    rc = sr_val_set_xpath(&(*output)[1], acRespXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acRespXpath, sr_strerror(rc));
        goto cleanup;
    }

    rc = sr_val_set_str_data(&(*output)[1], SR_STRING_T, message);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* inform sysrepo about the number of output values */
    *output_cnt = 2;

cleanup:
    return rc;
}

int
DSC_UnStateSubscribe()
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_UnSubStatescribe");


    if (NULL != m_pstRunningSess && NULL != m_pstStateSubscription) {
        rc = sr_unsubscribe(m_pstStateSubscription);
        m_pstStateSubscription = NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_UnStateSubscribe");

    return rc;
}

int
DSC_UnConfSubscribe()
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_UnConfSubscribe");

    if (NULL != m_pstStartupSess && NULL != m_pstConfSubscription) {
        rc = sr_unsubscribe(m_pstConfSubscription);
        m_pstConfSubscription = NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_UnConfSubscribe");

    return rc;
}

int
DSC_UnRpcSubscribe()
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, " ++ DSC_UnRpcSubscribe");


    if (NULL != m_pstStartupSess && NULL != m_pstRpcSubscription) {
        rc = sr_unsubscribe(m_pstRpcSubscription);
        m_pstRpcSubscription = NULL;
    }

    LOG_WRITE(EN_LOG_NOTICE, " -- DSC_UnRpcSubscribe");

    return rc;
}

void
DSC_DisableLibyangLog()
{
    /*********************************************************
       NOTE: By default, libyang will print error log to stderr.
          Now disable logging to stderr, eg: CLI.
    *********************************************************/

    ly_log_options(0);

    return;
}

static void
__libyanglog_cb(LY_LOG_LEVEL level, const char *msg, const char *path)
{
    int iLogLevel = 0;

    if (level > LY_LLWRN) {
        return;
    }

    switch (level) {
    case LY_LLERR:
        iLogLevel = EN_LOG_ERROR;
        break;
    case LY_LLWRN:
        iLogLevel = EN_LOG_WARNING;
        break;
    case LY_LLVRB:
        iLogLevel = EN_LOG_NOTICE;
        break;
    case LY_LLDBG:
        iLogLevel = EN_LOG_DEBUG;
        break;

    default:
        /* silent, just to cover enum, shouldn't be here in real world */
        return;
    }

    /* For compliation */
#ifdef DEF_CHECK_CODE
    iLogLevel = iLogLevel;
#endif

    LOG_WRITE(iLogLevel, "LY: %s path: %s", msg, path);

    return;
}

/*
   logging libyang log to syslog, added on 2021-06-16;
 */
void
DSC_SetLibyangLogCb()
{

    ly_set_log_clb(__libyanglog_cb, 1);

    return;
}
