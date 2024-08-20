/**
 * Copyright (c) Oplink Communications, LLC
 */
#include "DSC_inc.h"
#include "oplkalm.h"

/*re-use sysrepo code */
static char *
DSC_val_sr2ly_str(struct ly_ctx *ctx, const sr_val_t *sr_val, const char *xpath, char *buf, int output)
{
    struct lysc_node_leaf *sleaf;
    const struct lysc_type *t, *t2;
    LY_ARRAY_COUNT_TYPE u;

    if (!sr_val) {
        return NULL;
    }

    switch (sr_val->type) {
    case SR_STRING_T:
    case SR_BINARY_T:
    case SR_BITS_T:
    case SR_ENUM_T:
    case SR_IDENTITYREF_T:
    case SR_INSTANCEID_T:
    case SR_ANYDATA_T:
    case SR_ANYXML_T:
        return sr_val->data.string_val;
    case SR_LEAF_EMPTY_T:
        return NULL;
    case SR_BOOL_T:
        return sr_val->data.bool_val ? "true" : "false";
    case SR_DECIMAL64_T:
        /* get fraction-digits */
        sleaf = (struct lysc_node_leaf *)lys_find_path(ctx, NULL, xpath, output);
        if (!sleaf) {
            return NULL;
        }
        t = sleaf->type;
        if (t->basetype == LY_TYPE_LEAFREF) {
            t = ((struct lysc_type_leafref *)t)->realtype;
        }
        if (t->basetype == LY_TYPE_UNION) {
            t2 = NULL;
            LY_ARRAY_FOR(((struct lysc_type_union *)t)->types, u) {
                if (((struct lysc_type_union *)t)->types[u]->basetype == LY_TYPE_DEC64) {
                    t2 = ((struct lysc_type_union *)t)->types[u];
                    break;
                }
            }
            t = t2;
        }
        if (!t) {
            return NULL;
        }
        sprintf(buf, "%.*f", ((struct lysc_type_dec *)t)->fraction_digits, sr_val->data.decimal64_val);
        return buf;
    case SR_UINT8_T:
        sprintf(buf, "%" PRIu8, sr_val->data.uint8_val);
        return buf;
    case SR_UINT16_T:
        sprintf(buf, "%" PRIu16, sr_val->data.uint16_val);
        return buf;
    case SR_UINT32_T:
        sprintf(buf, "%" PRIu32, sr_val->data.uint32_val);
        return buf;
    case SR_UINT64_T:
        sprintf(buf, "%" PRIu64, sr_val->data.uint64_val);
        return buf;
    case SR_INT8_T:
        sprintf(buf, "%" PRId8, sr_val->data.int8_val);
        return buf;
    case SR_INT16_T:
        sprintf(buf, "%" PRId16, sr_val->data.int16_val);
        return buf;
    case SR_INT32_T:
        sprintf(buf, "%" PRId32, sr_val->data.int32_val);
        return buf;
    case SR_INT64_T:
        sprintf(buf, "%" PRId64, sr_val->data.int64_val);
        return buf;
    default:
        return NULL;
    }
}

sr_error_info_t *
DSC_val_sr2ly(struct ly_ctx *ctx, const char *xpath, const char *val_str, int dflt, int output, struct lyd_node **root)
{
    sr_error_info_t *err_info = NULL;
    struct lyd_node *node, *parent;
    int opts;

    opts = LYD_NEW_PATH_UPDATE | (output ? LYD_NEW_PATH_OUTPUT : 0);

    if (lyd_new_path2(*root, ctx, xpath, val_str, val_str ? strlen(val_str) : 0, 0, opts, &parent, &node)) {
        return err_info;
    }
    if (dflt) {
        node->flags |= LYD_DEFAULT;
    }

    if (!*root) {
        *root = parent;
    }
    return NULL;
}

/*this function convert sr_value list to tree */
int
DSC_SrValues2LydNode(sr_session_ctx_t *session, const char *path, sr_val_t *values, size_t value_cnt, struct lyd_node **parent)
{
    sr_error_info_t *err_info = NULL;
    struct ly_ctx *ly_ctx = (struct ly_ctx *)sr_acquire_context(sr_session_get_connection(session));
    char *val_str = NULL;
    char buf[1024] = {0};
    size_t i;
    int rc = SR_ERR_OK;

    /* create the container */
    if ((err_info = DSC_val_sr2ly(ly_ctx, path, NULL, 0, 0, parent))) {
        goto cleanup;
    }

    /* transform input into a data tree */
    for (i = 0; i < value_cnt; ++i) {
        val_str = DSC_val_sr2ly_str(ly_ctx, &values[i], values[i].xpath, buf, 0);
        if ((err_info = DSC_val_sr2ly(ly_ctx, values[i].xpath, val_str, values[i].dflt, 0, parent))) {
            LOG_WRITE(EN_LOG_ERROR, "sr_val_sr2ly: %s error", values[i].xpath);
            rc = SR_ERR_INTERNAL;
            goto cleanup;
        }
    }

cleanup:
    sr_release_context(sr_session_get_connection(session));
    return rc;
}

int
DSC_ReplaceStr(char *result, const char *str, const char *substr, const char *replace)
{
    char *ptr = NULL;
    if (NULL == result || NULL == str || NULL == substr || NULL == replace) {
        LOG_WRITE(EN_LOG_ERROR, "input error: %p, %p, %p, %p",
                  result, str, substr, replace);

        return SR_ERR_INTERNAL;
    }
    ptr = strstr(str, substr);
    if (NULL == ptr) {
        LOG_WRITE(EN_LOG_DEBUG, "not found '%s' in '%s'", substr, str);
        return SR_ERR_INTERNAL;
    }
    /*copy characters from 'str' start to 'substr' start */
    strncpy(result, str, ptr - str);
    result[ptr - str] = '\0';
    sprintf(result + (ptr - str), "%s%s", replace, ptr + strlen(substr));

    return SR_ERR_OK;
}

static int
__replace_delete_path(const char *pcInXpath, char *pcDelPath)
{
    char *pcState = NULL;

    if (NULL == pcInXpath || NULL == pcDelPath) {
        return SR_ERR_INVAL_ARG;
    }

    if (strlen(pcInXpath) > OPLK_PATH_MAX_LEN - 1) {
        LOG_WRITE(EN_LOG_ERROR, "path len error: %lu, %s'", strlen(pcInXpath), pcInXpath);
        return SR_ERR_INVAL_ARG;
    }

    pcState = strstr(pcInXpath, "']/");
    if (NULL == pcState) {
        return SR_ERR_INVAL_ARG;
    }

    strncpy(pcDelPath, pcInXpath, pcState - pcInXpath);
    pcDelPath[pcState - pcInXpath] = '\0';
    strcat(pcDelPath, "']");

    return SR_ERR_OK;
}

int
__uninstall_module(const char *xpath)
{
    char module[32] = {0};
    int rc;

    rc = DSC_GetNamespace(xpath, module, sizeof(module) - 1);
    if (rc == SR_ERR_OK)
        return sr_remove_module(DSC_GetConn(), module, 0);

    return rc;
}

/*Load data in running datastore to operational datastore. */
/*Use case1: For the operational data does not change, it is stored in internal
   datastore, such as:
   1. source_xpath: /com-oplink-device:org-openroadm-device
      dest_xpath:  /org-openroadm-device:org-openroadm-device
   2. source_xpath: com-oplink-optical-i:optical-control
      dest_xpath: /com-oplink-optical:optical-control
   3. source_xpath:  /com-oplink-platform:componets
      dest_xpath: /openconfig-platform:componets
   This function read the devce internal datastore and
   load it to the operational datastore of the external module
 */

/*Use case2: Load openconfig yang model's state in operational datastore config data*/
/*source_xpath: /openconfig-optical-amplifier:optical-amplifier/amplifiers/amplifier/config */
/*dest_xpath: NULL */

int
DSC_LoadOperData(const char *pcSrcXpath, const char *pcDestXpath, const char *pcFunTrace)
{
    int rc = SR_ERR_OK;

    /* session used */
    sr_session_ctx_t *pstOperSess = NULL;
    sr_session_ctx_t *pstRunningSess = NULL;

    /* destination xpath. */
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    /* this is the partial xpath without source_xoath as prefix*/
    char *pcXpathWithoutPrefix = NULL;

    /* values in internal data store */
    int i = 0;
    size_t nCnt = 0;
    sr_val_t *pstVals = NULL;

    /* working variable set to destination */
    sr_val_t stDestVal;
    char *pcTmp = NULL;

    /* check input path valid */
    if (NULL == pcSrcXpath) {
        LOG_WRITE(EN_LOG_DEBUG, "Input source path null");
        goto cleanup;
    }

    /* check the input path.                */
    /* Now ONLY support the root container  */
    if ((NULL == strstr(pcSrcXpath, "//.")) &&
        (NULL == strstr(pcSrcXpath, "//*"))) {
        LOG_WRITE(EN_LOG_DEBUG, "Input source path not valid, need add '//*' or '//.'");
        goto cleanup;
    }

    /* session used */
    pstOperSess = DSC_GetOperSession();
    pstRunningSess = DSC_GetRunningSession();

    /* libyang will print some error logs to terminal, add some information for debug. */
#if 0

    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("function:     %s\n", pcFunTrace);
    printf("source_xpath: %s\n", pcSrcXpath);
    printf("dest_xpath  : %s\n", pcDestXpath);

#endif

    /* get data from internal datastore or config part of openconfig */
    rc = sr_get_items(pstRunningSess, pcSrcXpath, 0, SR_OPER_DEFAULT, &pstVals, &nCnt);
    if (SR_ERR_OK != rc) {
        goto cleanup;
    }

    for (i = 0; i < nCnt; i++) {
        /* skip some values */
        if ((pstVals[i].type != SR_CONTAINER_T) &&
            (pstVals[i].type != SR_CONTAINER_PRESENCE_T) &&
            (pstVals[i].type != SR_LIST_T)) {

/* libyang will print some error logs to terminal, add some information for debug. */
#if 0
            sr_print_val(pstVals + i);
#endif
            memset(&stDestVal, 0x00, sizeof(stDestVal));
            memset(acXpath, 0x00, sizeof(acXpath));
            sr_dup_val_data(&stDestVal, &pstVals[i]);
            if (NULL == pcDestXpath) {
                /*this is openconfig from config to state */
                DSC_ReplaceStr(acXpath, pstVals[i].xpath, "/config/", "/state/");
            } else {
                /* this is internal data model to external operational      */
                /* obtain string without source xapth                       */
                /* source_xpath: /com-oplink-device:org-openroadm-device//. */
                /* need to remove the "//." or "*"                          */
                pcXpathWithoutPrefix = pstVals[i].xpath + strlen(pcSrcXpath) - strlen("//.");
                /* put dest xpath at front */
                snprintf(acXpath, OPLK_PATH_MAX_LEN, "%s%s", pcDestXpath, pcXpathWithoutPrefix);
            }

            /* set the xpath to destination */
            sr_val_set_xpath(&stDestVal, acXpath);
            /* push to the operational datastore */
            pcTmp = strstr(acXpath, "[.=");
            if (NULL != pcTmp) {
                *pcTmp = '\0';
                /* special handler for leaf-list path, example:
                   /org-openroadm-device:org-openroadm-device/shelves[shelf-name='OLSP']/slots[slot-name='slot-scc']/provisioned-circuit-pack[.='cp-scc']
                   =>
                   /org-openroadm-device:org-openroadm-device/shelves[shelf-name='OLSP']/slots[slot-name='slot-scc']/provisioned-circuit-pack
                 */
            }

            sr_set_item(pstOperSess, acXpath, &stDestVal, SR_EDIT_DEFAULT);
#if 0
            sr_print_val(&stDestVal);
#endif

        }
    }

    /* apply change to operatonal datastore */
    sr_apply_changes(pstOperSess, 0);

    if (pcDestXpath != NULL) {
        /*TODO TEST later */
        /*__uninstall_module (source_xpath); */
    }
cleanup:

    LOG_WRITE(EN_LOG_DEBUG, "%s, total %lu items returned.", pcFunTrace, nCnt);
    if (pstVals != NULL) {
        sr_free_values(pstVals, nCnt);
        pstVals = NULL;
    }

    /* libyang will print some error logs to terminal, add some information for debug. */
#if 0
    printf("----------------------------------------------------\n");
#endif

    return rc;
}

/*update openconfig state in operational datastore when config changes.*/
/*USe case : This function opencofig yang model's config change to state: */
/*change_xpath: /openconfig-optical-amplifier:optical-amplifier/amplifiers/amplifier[name=BA]/config/... */
int
DSC_UpdateOpenconfigState(sr_session_ctx_t *session, const char *change_xpath)
{
    sr_change_iter_t *pstIter = NULL;
    int rc = SR_ERR_OK;
    sr_change_oper_t enOper;
    sr_val_t *pstOldValue = NULL;
    sr_val_t *pstNewValue = NULL;
    sr_val_t val;
    char xpath[OPLK_PATH_MAX_LEN] = {0};

    /* delete xpath */
    char acDelXpath[OPLK_PATH_MAX_LEN] = {0};

    rc = sr_get_changes_iter(session, change_xpath, &pstIter);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get changes iter failed for xpath %s", OPS_XPATH_HEADER);
        goto cleanup;
    }

    while (SR_ERR_OK == sr_get_change_next(session, pstIter,
                                           &enOper, &pstOldValue, &pstNewValue)) {

        switch (enOper) {
        case SR_OP_CREATED:
        case SR_OP_MODIFIED:
            memset(&val, 0x00, sizeof(val));
            sr_dup_val_data(&val, pstNewValue);
            rc = DSC_ReplaceStr(xpath, pstNewValue->xpath, "/config/", "/state/");
            if (SR_ERR_OK == rc) {
                sr_val_set_xpath(&val, xpath);
                rc = sr_set_item(DSC_GetOperSession(), xpath, &val, SR_EDIT_DEFAULT);
                if (SR_ERR_OK != rc) {
                    LOG_WRITE(EN_LOG_DEBUG, "sr_set_item: %s %s", pstNewValue->xpath, sr_strerror(rc));
                }
            }
            break;
        case SR_OP_DELETED:
            /* may cause core dump, comment temporary */
            rc = DSC_ReplaceStr(xpath, pstOldValue->xpath, "/config/", "/state/");
            if (SR_ERR_OK == rc) {

                memset(acDelXpath, 0x00, sizeof(acDelXpath));
                __replace_delete_path(xpath, acDelXpath);
                /* sr_delete_item Cannot be used for ::SR_DS_OPERATIONAL.                      */
                /* Use ::sr_oper_delete_item_str() instead.                                    */
                /* Cause 'sr_oper_delete_item_str' delete string, just delete the list instead */
                rc = sr_oper_delete_item_str(DSC_GetOperSession(), acDelXpath, NULL, SR_EDIT_DEFAULT);
                if (SR_ERR_OK != rc) {
                    LOG_WRITE(EN_LOG_ERROR, "sr_oper_delete_item_str: %s %s", xpath, sr_strerror(rc));
                }
            }
            break;
        case SR_OP_MOVED:
            DSC_ReplaceStr(xpath, pstOldValue->xpath, "/config/", "/state/");
            memset(acDelXpath, 0x00, sizeof(acDelXpath));
            __replace_delete_path(xpath, acDelXpath);
            rc = sr_oper_delete_item_str(DSC_GetOperSession(), acDelXpath, NULL, SR_EDIT_DEFAULT);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_DEBUG, "sr_oper_delete_item_str: %s %s", xpath, sr_strerror(rc));
            }
            sr_dup_val_data(pstNewValue, &val);
            rc = DSC_ReplaceStr(xpath, pstNewValue->xpath, "/config/", "/state/");
            if (SR_ERR_OK == rc) {
                sr_val_set_xpath(&val, xpath);
                rc = sr_set_item(DSC_GetOperSession(), xpath, pstNewValue, SR_EDIT_DEFAULT);
                if (SR_ERR_OK != rc) {
                    LOG_WRITE(EN_LOG_DEBUG, "sr_set_item: %s %s", pstNewValue->xpath, sr_strerror(rc));
                }
            }
            break;
        default:
            break;
        }
        SR_SAFE_FREE_VAL(pstOldValue);
        SR_SAFE_FREE_VAL(pstNewValue);
    }

    rc = DSC_Commit(DSC_GetOperSession());

cleanup:
    SR_SAFE_FREE_VAL(pstOldValue);
    SR_SAFE_FREE_VAL(pstNewValue);
    sr_free_change_iter(pstIter);
    return rc;
}

/*push data operational datastore. */
int
DSC_PushOperData(const char *xpath, sr_val_t *value)
{
    int rc = SR_ERR_OK;

    rc = sr_set_item(DSC_GetOperSession(), xpath, value, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_DEBUG, "sr_set_item: %s %s", xpath, sr_strerror(rc));
        return rc;
    }

    return DSC_Commit(DSC_GetOperSession());
}

/*For the operational data does not change, it is stored in internal
   datastore, such as:
   1) com-oplink-device
   2) com-oplink-waveplan
   3) com-oplink-optical-i
   4) com-oplink-alarm
   5) com-oplink-pm

   This function read the devce internal datastore and return the data to
   sysrepo as external data models:
   1) org-openroadm-device
   2) org-openroadm-device:default-waveplan and org-openroadm-device:flex-waveplan
   3) openconfig-gnoi-otdr:/optical-time-domain-reflectometer/otdrs
 */
int
DSC_GeneralDataProviderCb(sr_session_ctx_t *session, const char *pcModuleExternal, const char *xpath,
                          const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_ctx, const char *func_trace)
{
    int rc = SR_ERR_OK;
    char xpath_i[OPLK_PATH_MAX_LEN] = {'\0'}; /* working varible */
    char xpath_alm[OPLK_PATH_MAX_LEN] = {'\0'}; /* working varible */
    char *xpath_ = NULL; /* this is the xpath with no module name */
    char *pcModuleInternal = (char *)private_ctx;
    sr_val_t *val = NULL; /* values in internal data store */
    sr_val_t *v = NULL; /* return data */
    int i, j = 0;
    size_t cnt;
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == xpath ||
        NULL == func_trace) {
        LOG_WRITE(EN_LOG_ERROR, "Input error.");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (NULL == private_ctx) {
        LOG_WRITE(EN_LOG_WARNING, "nothing to be get form internal: %s", xpath);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_DEBUG, "%s, Data for '%s' requested.", func_trace, xpath);
    if ((0 == strcasecmp(pcModuleInternal, MODULE_COA)) ||
        (0 == strcasecmp(pcModuleInternal, MODULE_COD)) ||
        (0 == strcasecmp(pcModuleInternal, MODULE_COP)) ||
        (0 == strcasecmp(pcModuleInternal, MODULE_NS_OTDR_I)) ||
        (0 == strcasecmp(pcModuleInternal, DATA_MODULE_WAVEPLAN))) {
        if (NULL == session) {
            session = DSC_GetRunningSession();
        }
    } else {
        goto cleanup;
    }

    /* find the xpath without module name in front */
    xpath_ = strchr(xpath, ':');

    if (0 == strcasecmp(pcModuleInternal, DATA_MODULE_WAVEPLAN)) {
        strncpy(xpath_i, WAVEPLAN_XPATH_DEFAULT_ALL_I, OPLK_PATH_MAX_LEN);
    } else {
        snprintf(xpath_i, OPLK_PATH_MAX_LEN, "/%s%s//*", pcModuleInternal, xpath_);
    }

    /* get data from internal datastore */
    rc = sr_get_items(pstSess, xpath_i, 0, SR_OPER_DEFAULT, &val, &cnt);
    if (SR_ERR_OK != rc || cnt == 0) {
        /* get the sub tree failed, must be leaf node */
        snprintf(xpath_i, OPLK_PATH_MAX_LEN, "/%s%s", pcModuleInternal, xpath_);
        rc = sr_get_items(pstSess, xpath_i, 0, SR_OPER_DEFAULT, &val, &cnt);
        /* ? what options */
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_DEBUG, "sr_get_items: %s %s", xpath_i, sr_strerror(rc));
            goto cleanup;
        }
    }

    /* allocate space for data to return */
    rc = sr_new_values(cnt, &v);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error %s", sr_strerror(rc));
        goto cleanup;
    }

    for (i = 0; i < cnt; i++) {
        /* skip some values */
        if ((val[i].type != SR_CONTAINER_T) &&
            (val[i].type != SR_CONTAINER_PRESENCE_T) &&
            (val[i].type != SR_LIST_T) &&
            /* data provider can not set the node which is key, */
            (strcmp(sr_xpath_node_name(val[i].xpath), "wavelength-number") != 0) &&
            (strcmp(sr_xpath_node_name(val[i].xpath), "component-name") != 0) &&
            (strcmp(sr_xpath_node_name(val[i].xpath), "name") != 0) &&
            (strcmp(sr_xpath_node_name(val[i].xpath), "completion-time") != 0) &&
            (strcmp(sr_xpath_node_name(val[i].xpath), "event-id") != 0)) {
            /* skip macAddress key */
            if ((strstr(val[i].xpath, "macAddrs") != NULL) &&
                (strcmp(sr_xpath_node_name(val[i].xpath), "name") == 0)) {
                continue;
            }
            sr_dup_val_data(&v[j], &val[i]);
            /* reset the xpath back to external module name */
            xpath_ = strchr(val[i].xpath, ':');
            if (0 == strcasecmp(pcModuleInternal, DATA_MODULE_WAVEPLAN)) {
                xpath_ = strchr(xpath_, '/');
                snprintf(xpath_i, OPLK_PATH_MAX_LEN, "%s:%s", WAVEPLAN_XPATH_DEFAULT_COM, xpath_ + 1);
            } else if (0 == strcasecmp(pcModuleInternal, MODULE_COA)) {
                /* Replace /org-openroadm-alarm:alarm-notification/external-id                  */
                /* with    /org-openroadm-alarm:alarm-notification/com-oplink-alarm:external-id */
                memset(xpath_i, 0x00, sizeof(xpath_i));
                memset(xpath_alm, 0x00, sizeof(xpath_alm));
                snprintf(xpath_alm, OPLK_PATH_MAX_LEN, "/%s%s", pcModuleExternal, xpath_);
                ALM_UpdateNodeNamespace((const char *)xpath_alm, xpath_i, sizeof(xpath_i));
            } else {
                snprintf(xpath_i, OPLK_PATH_MAX_LEN, "/%s%s", pcModuleExternal, xpath_);
            }

            sr_val_set_xpath(&v[j++], xpath_i);
        }
    }

    rc = DSC_SrValues2LydNode(session, xpath, v, j, parent);
cleanup:

    LOG_WRITE(EN_LOG_DEBUG, "%s, total %lu items returned.", func_trace, cnt);

    sr_free_values(val, cnt);
    sr_free_values(v, j);
    return rc;
}


/* This is a supplement to DSC_LoadOperData */
int
DSC_LoadOperDataOfSingleLeafNode(const char *pcSrcXpath, const char *pcDestXpath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstOperSess = DSC_GetOperSession();
    sr_session_ctx_t *pstRunningSess = DSC_GetRunningSession();
    sr_val_t *pstVal = NULL;

    if (NULL == pcSrcXpath || NULL == pcDestXpath) {
        LOG_WRITE(EN_LOG_DEBUG, "Input path null");
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = sr_get_item(pstRunningSess, pcSrcXpath, 0, &pstVal);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_get_item: %s %s", pcSrcXpath, sr_strerror(rc));
        goto END_LABEL;
    }

    sr_val_set_xpath(pstVal, pcDestXpath);

    rc = sr_set_item(pstOperSess, pcDestXpath, pstVal, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_set_item: %s %s", pcDestXpath, sr_strerror(rc));
        goto END_LABEL;
    }

END_LABEL:
    SR_SAFE_FREE_VAL(pstVal);
    return rc;
}

