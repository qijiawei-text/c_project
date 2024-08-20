#ifndef _DSC_OPER_H
#define _DSC_OPER_H

#ifdef __cplusplus
extern "C" {
#endif

/*convert a string to lyd_node and attach to the root. */
sr_error_info_t *DSC_val_sr2ly(struct ly_ctx *ctx, const char *xpath, const char *val_str, int dflt, int output, struct lyd_node **root);

/*Conver list of sr_value to a lyd_node tree and attach to the parent. */
int DSC_SrValues2LydNode(sr_session_ctx_t *session, const char *path, sr_val_t *values, size_t value_cnt, struct lyd_node **parent);

/*Load data in internal running datastore to external operational datastore. */
/*Load data in openconfig config to state. */
int DSC_LoadOperData(const char *source_xpath, const char *dest_xpath, const char *func_trace);

/*push data operational datastore. */
int DSC_PushOperData(const char *xpath, sr_val_t *value);

/*update openconfig state in operational datastore when config changes.*/
int DSC_UpdateOpenconfigState(sr_session_ctx_t *session, const char *change_xpath);

/*TODO: be obsoleted later. */
int DSC_GeneralDataProviderCb(sr_session_ctx_t *session, const char *module_name, const char *xpath,
                              const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_data, const char *func_trace);

int DSC_LoadOperDataOfSingleLeafNode(const char *pcSrcXpath, const char *pcDestXpath);

int DSC_ReplaceStr(char *result, const char *str, const char *substr, const char *replace);

#ifdef __cplusplus
}
#endif

#endif
