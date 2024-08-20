
#include "DSC_inc.h"
#include "rest_common.h"
#include "rest_sr_util.h"
#include "rest_sr_get.h"
#include <limits.h>

static void
__DelModuleNameofNode(cJSON *pstJson)
{
    char *pcPos = NULL;
    size_t nLen = 0;

    if (pstJson != NULL && pstJson->child != NULL) {

        pcPos = strchr(pstJson->child->string, ':');
        if (pcPos != NULL) {
            LOG_WRITE(EN_LOG_DEBUG, "[before]pstJson->child->string: %s", pstJson->child->string);
            nLen = strlen(pstJson->child->string) - (pcPos - pstJson->child->string + 1) + 1;
            memmove(pstJson->child->string, pcPos + 1, nLen);
            LOG_WRITE(EN_LOG_DEBUG, "[after]pstJson->child->string: %s", pstJson->child->string);
        }
    }

}
static cJSON *
__REST_GetJsonDataByXpath(const char *pcYangPath)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    /* sr_node_t *pstSubtree = NULL; */
    sr_data_t *pstSubtreeData = NULL;
    char *pcJsonStr = NULL;
    cJSON *pstJson = NULL;

    if (NULL == pcYangPath) {
        LOG_WRITE(EN_LOG_ERROR, "pcYangPath == NULL");
        goto END_LABEL;
    }

    pstSess = DSC_GetOperSession();

    rc = sr_get_subtree(pstSess, pcYangPath, 0, &pstSubtreeData);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get subtree faild. error: %s, xpath: %s", sr_strerror(rc), pcYangPath);
        goto END_LABEL;
    }

    if (NULL == pstSubtreeData) {
        LOG_WRITE(EN_LOG_ERROR, "error sr_get_subtree: %s", pcYangPath);
        goto END_LABEL;
    }

    rc = lyd_print_mem(&pcJsonStr, pstSubtreeData->tree, LYD_JSON, LYD_PRINT_WITHSIBLINGS | LYD_PRINT_SHRINK);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }

    /* printf("json string: %s\n", pcJsonStr); */

    /* json format */
    pstJson = cJSON_Parse(pcJsonStr);

#if 0
    cha printf("json object: %s\n", cJSON_Print(pstJson));
#endif
    __DelModuleNameofNode(pstJson);

END_LABEL:
    sr_release_data(pstSubtreeData);
    DEF_SAFE_FREE(pcJsonStr);

    return pstJson;
}

/* free *ppstJson by caller */
int
REST_Get(char *pcUrl, cJSON **ppstJson)
{
    int rc = SR_ERR_OK;
    char acYangXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == pcUrl || NULL == ppstJson || NULL != *ppstJson) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "invalid input parameters");
        goto END_LABEL;
    }

    /* url to yang xpath */
    rc = REST_UrlToYangXpath((const char *)pcUrl, acYangXpath, sizeof(acYangXpath));
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
    LOG_WRITE(EN_LOG_DEBUG, "acYangXpath: %s", acYangXpath);
    /* get json data */
    *ppstJson = __REST_GetJsonDataByXpath((const char *)acYangXpath);
    if (NULL == *ppstJson) {
        LOG_WRITE(EN_LOG_ERROR, "empty json object. xpath: %s", acYangXpath);
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

END_LABEL:
    return rc;
}
