/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : dsc_parameter.c
   Description : System parameter range.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/06/24   V1.0.0       Initial file.
**********************************************************************************/
#include <string.h>

#include "DSC_log.h"
#include "DSC_common.h"
#include "DSC_parameter.h"
#include "DSC_edfa.h"

static ST_PARAMETER m_astParameter[DEF_MAX_PARA];
static size_t m_nParaCnt = 0;
static bool m_bReadFlag = false;

int
DSC_GetParameterLists(sr_session_ctx_t *pstStartupSess, ST_PARAMETER **ppstParaList, size_t *pnCnt)
{
    int rc = 0;
    int i = 0, j = 0;
    sr_val_t *pstVal = NULL;
    size_t nCnt = 0;
    ST_PARAMETER *pstPara = (ST_PARAMETER *)&m_astParameter;

    if (NULL == ppstParaList || NULL == pstPara || NULL == pnCnt || NULL == pstStartupSess) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    if (m_bReadFlag) {
        *ppstParaList = pstPara;
        *pnCnt = m_nParaCnt;
        rc = SR_ERR_OK;
        goto cleanup;
    }

    memset(&m_astParameter, 0, sizeof(m_astParameter));
    rc = get_items(pstStartupSess, DEF_PARA_XPATH, &pstVal, &nCnt);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), DEF_PARA_XPATH);
        *pnCnt = 0;
        rc = SR_ERR_OK;
        goto cleanup;
    }

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
        *pnCnt = 0;
        goto cleanup;
    }

    j = -1;
    for (i = 0; i < nCnt; i++) {
        if (strstr(pstVal[i].xpath, "/name") != NULL) {
            j++;
            if (j >= DEF_MAX_PARA) {
                break;
            }
            strncpy(pstPara[j].acParaName, pstVal[i].data.string_val, sizeof(pstPara[j].acParaName) - 1);
        } else if (strstr(pstVal[i].xpath, "high-thr") != NULL) {
            pstPara[j].dHighVal = pstVal[i].data.decimal64_val;
        } else if (strstr(pstVal[i].xpath, "low-thr") != NULL) {
            pstPara[j].dLowVal = pstVal[i].data.decimal64_val;
        }
    }

    sr_free_values(pstVal, nCnt);
    pstVal = NULL;

    *ppstParaList = pstPara;
    m_nParaCnt = j + 1;
    *pnCnt = m_nParaCnt;

    m_bReadFlag = true;

cleanup:
    if (NULL != pstVal) {
        sr_free_values(pstVal, nCnt);
    }
    return rc;
}

int
DSC_GetParameterByName(sr_session_ctx_t *pstStartupSess, const char *pcParaName, double *pdHighVal, double *pdLowVal)
{
    int rc = 0;
    int i = 0;
    ST_PARAMETER *pstPara = NULL;
    size_t nCnt = 0;
    if (NULL == pcParaName || NULL == pdHighVal || NULL == pdLowVal) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    rc = DSC_GetParameterLists(pstStartupSess, &pstPara, &nCnt);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    for (i = 0; i < nCnt; i++) {
        if (0 == strcmp(pcParaName, m_astParameter[i].acParaName)) {
            *pdHighVal = m_astParameter[i].dHighVal;
            *pdLowVal = m_astParameter[i].dLowVal;
            break;
        }
    }

    if (nCnt == i) {
        rc = SR_ERR_NOT_FOUND;
    }

cleanup:
    return rc;

}



