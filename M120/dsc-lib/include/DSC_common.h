#ifndef _DSC_COMMON_H
#define _DSC_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
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

#include "sysrepo.h"
#include "sysrepo_types.h"
#include "sysrepo/values.h"
#include "sysrepo/xpath.h"
#include "libyang/tree_data.h"
#include "libyang/log.h"

#include "DSC_log.h"
#include "DSC_ood.h"
#include "DSC_cod.h"
#include "DSC_coo.h"

typedef enum en_DIRECTION
{
    DIRECTION_TX = 1,
    DIRECTION_RX,
    BIDIRECTIONAL
} DIRECTION_EN;


#define ANONYMOUS_MAP_ENTRY(_xpath, _type, _buff) \
    { \
        .xpath = _xpath, \
        .type = _type, \
        .buff = _buff, \
        .buff_size = sizeof(_buff) \
    }

typedef struct st_ANONYMOUS {
    const char *xpath;
    sr_val_type_t type;
    char *buff;
    size_t buff_size;
} ANONYMOUS_ST;

#define OPLK_PATH_MAX_LEN  512
#define OPLK_BUF_MAX_LEN   512
#define OPLK_NAME_MAX_LEN  64

#define SR_DEFAULT_STRING_VAL    ""

#ifndef SR_SAFE_FREE_VAL
#define SR_SAFE_FREE_VAL(pstVal)      do {if ((pstVal) != NULL) {sr_free_val(pstVal); (pstVal) = NULL; }} while (0)
#endif

/**
 * @brief Generic structure for a data node.
 */
typedef struct lyd_node sr_node_t;

/**
 * @brief Data node structure for the inner data tree nodes - containers, lists, RPCs, actions and Notifications.
 */
typedef struct lyd_node_inner sr_node_inner_t;

/**
 * @brief Data node structure for the terminal data tree nodes - leaves and leaf-lists.
 */
typedef struct lyd_node_term sr_node_term_t;

#define lyd_node_to_inner(lyd_node) ((sr_node_inner_t *)(lyd_node))

/* This function is from sysrepo log.h for complication. */
extern void sr_errinfo_new_ly(sr_error_info_t **err_info, const struct ly_ctx *ly_ctx);
extern int sr_api_ret(sr_session_ctx_t *session, sr_error_info_t *err_info);

typedef struct {
    const char *pcModuleName;
    sr_module_change_cb pFCallback;
    const char *pcXpath;
}ST_CONF_SUB;

typedef struct
{
    const char *pcModuleName;
    sr_oper_get_items_cb pFCallback;
    const char *pcXpath;
    void *pvPrivateCtx;
}ST_PROVIDER_SUB;

typedef struct
{
    const char *pcModuleName;
    sr_rpc_cb pFCallback;
    const char *pcXpath;
}ST_RPC_SUB;

#define RPC_STATUS_SUCCESS         "Successful"
#define RPC_STATUS_INPROGRESS      "In-progress"
#define RPC_STATUS_FAILED          "Failed"

#define RPC_TYPE_ALL               "all"
#define RPC_TYPE_OPTICAL           "optical"

#define DEF_LEAF_NAX_CNT           (512)

#define DEBUG_PRINT    do { \
        printf("%s, %d\n", __func__, __LINE__); \
} while (0)

extern int sr_dup_val_data(sr_val_t *dest, const sr_val_t *source);
extern int sr_get_items_vargs(sr_session_ctx_t *session, sr_val_t **values, size_t *value_cnt, const char *xpath_fmt, ...);
extern int sr_get_item_vargs(sr_session_ctx_t *session, sr_val_t **value, const char *xpath_fmt, ...);
extern sr_val_t *sr_values_find_by_xpath(sr_val_t *values, size_t values_count, const char *xpath);
extern sr_val_t *sr_values_find_by_xpath_vargs(sr_val_t *values, size_t values_count, const char *xpath_fmt, ...);

extern int get_string_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_string_item_log_trace(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_identityref_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_binary_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_uint32_item(sr_session_ctx_t *session, const char *xpath, uint32_t *lhs);
extern int get_int32_item(sr_session_ctx_t *session, const char *xpath, int32_t *lhs);
extern int get_uint16_item(sr_session_ctx_t *session, const char *xpath, uint16_t *lhs);
extern int get_int16_item(sr_session_ctx_t *session, const char *xpath, int16_t *lhs);
extern int get_uint8_item(sr_session_ctx_t *session, const char *xpath, uint8_t *lhs);
extern int get_uint64_item(sr_session_ctx_t *session, const char *xpath, uint64_t *lhs);
extern int get_enum_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_enum_item_log_trace(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int get_decimal64_item(sr_session_ctx_t *session, const char *xpath, double *lhs);
extern int get_bool_item(sr_session_ctx_t *session, const char *xpath, bool *state);
int get_bool_item_log_trace(sr_session_ctx_t *session, const char *xpath, bool *state);
extern int get_items(sr_session_ctx_t *session, const char *xpath, sr_val_t **values, size_t *count);
extern int get_subtree(sr_session_ctx_t *session, const char *xpath, uint32_t timeout_ms,
                       sr_data_t **subtree);
extern int get_subtrees(sr_session_ctx_t *session, const char *xpath, uint32_t timeout_ms,
                        sr_get_options_t opt, sr_data_t **data);
extern int set_string_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int set_identityref_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int set_binary_item(sr_session_ctx_t *session, const char *xpath, char *lhs, size_t size);
extern int set_uint32_item(sr_session_ctx_t *session, const char *xpath, uint32_t lhs);
extern int set_int32_item(sr_session_ctx_t *session, const char *xpath, int32_t lhs);
extern int set_uint16_item(sr_session_ctx_t *session, const char *xpath, uint16_t lhs);
extern int set_int16_item(sr_session_ctx_t *session, const char *xpath, int16_t lhs);
extern int set_uint8_item(sr_session_ctx_t *session, const char *xpath, uint8_t lhs);
extern int set_uint64_item(sr_session_ctx_t *session, const char *xpath, uint64_t lhs);
extern int set_enum_item(sr_session_ctx_t *session, const char *xpath, char *lhs);
extern int set_bool_item(sr_session_ctx_t *session, const char *xpath, bool state);
extern int set_bits_item(sr_session_ctx_t *session, const char *xpath, char *lhs);
extern int set_decimal64_item(sr_session_ctx_t *session, const char *xpath, double lhs);

extern int set_uint32_item_strict(sr_session_ctx_t *session, const char *xpath, uint32_t lhs);
extern int del_item(sr_session_ctx_t *session, const char *xpath);


extern int DSC_GetItems(sr_session_ctx_t *sess, char *path, ANONYMOUS_ST *pstAnonymous, size_t size);
extern int DSC_ProcessRPCResponse(sr_val_t *output, int output_cnt);
extern char *__sprintfInt(char *pcBuf, char *pcStr, int i);
extern char *__sprintfStr(char *pcBuf, char *pcStr, char *pcSrc);
extern char *__sprintfStr2(char *pcBuf, char *pcStr, char *pcSrc1, char *pcSrc2);

extern unsigned int DSC_GetTreeLeaf(sr_node_t *pstTree, sr_node_t **ppstLeafArray);
size_t DSC_IterateTreeLeaf(const sr_node_t *pstRoot, sr_node_term_t *pstOut, size_t nMaxCnt);
extern int DSC_GetKeyValue(char *pcXpath, char *pcListName, char *pcKeyStr, int iLen, char *pcKeyValue);
extern int DSC_DelNode(sr_session_ctx_t *pstSess, char *pcXPath);
extern int DSC_ParseValuesByXpath(const sr_val_t *values, size_t values_count, ANONYMOUS_ST *pstAnonymous, size_t size);
extern int set_string_val_by_xpath(sr_val_t *input, const char *xpath, const char *pcStr);
extern int set_uint32_val_by_xpath(sr_val_t *input, const char *xpath, uint32_t uiValue);
extern int set_int32_val_by_xpath(sr_val_t *input, const char *xpath, int32_t iValue);
extern int DSC_RpcSend(sr_session_ctx_t *pstSess, const char *pcXpath,
                       sr_val_t *pstInput, size_t nInputCnt,
                       sr_val_t **ppstOutput, size_t *pnOutputCnt);

extern int DSC_GetNamespace(const char *pcInXpath, char *pcNameSpace, size_t tMaxSize);
extern bool DSC_IsSchemaInstalled(sr_session_ctx_t *pstSess, const char *pcModuleName);
extern int DSC_UninstallModule(const char *pcModuleName);

int DSC_StartAgentComSess(const char *pcAppName, int iTimeout);
int DSC_StartAgentUserSess(const char *pcAppName, const char *pcUserName, int iTimeout);
int DSC_StartDeamonSess(const char *pcAppName, int iTimeoutMs, sr_session_ctx_t **ppstStartupSess, sr_session_ctx_t **ppstRunningSess, sr_session_ctx_t **ppstOperSess);
int DSC_SwitchToRunningSess();
int DSC_ResetSession();
sr_session_ctx_t *DSC_GetSession();
sr_session_ctx_t *DSC_GetStartupSession();
sr_session_ctx_t *DSC_GetRunningSession();
sr_session_ctx_t *DSC_GetOperSession();
sr_conn_ctx_t *DSC_GetConn();
int DSC_StopSess();
int DSC_Commit(sr_session_ctx_t *sess);
int DSC_RpcSubscribe(const char *pcXpath, sr_rpc_cb fCallback, void *pvPrivateCtx);
int DSC_GetItemsSubscribe(const char *module, const char *pcXpath, sr_oper_get_items_cb fCallback, void *pvPrivateCtx);
int DSC_ChangeSubscribe(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx);
int DSC_ChangeSubscribePassive(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx);
int DSC_ModuleChangeSubscribe(const char *module, const char *pcXpath, sr_module_change_cb fCallback, void *pvPrivateCtx);
int DSC_UnSubscribe();
int DSC_RpcCreateOutput(const char *xpath, const char *status, const char *message, sr_val_t **output, size_t *output_cnt);
int DSC_UnStateSubscribe();
int DSC_UnConfSubscribe();
int DSC_UnRpcSubscribe();
int DSC_PrintVal(sr_session_ctx_t *pstSess, const char *pcXpath);

void DSC_DisableLibyangLog();

void DSC_SetLibyangLogCb();

/* Unit: ms */
#define CONN_DEF_TIMEOUT  10

#ifdef __cplusplus
}
#endif

#endif

