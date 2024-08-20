/***MODU+*******************************************************************************************
   Copyright(C), 2022, Molex Tech.Co.,Ltd
 * FileName    : dsc_connectity.h
 * Description : Define the connectivity validation control functions.
 * History     :
 *     [Author]      [Date]          [Version]        [Description]
 * [1] Chun Ye       2022/09/29      Ver 1.0.0        Initial file.
 ***MODU-******************************************************************************************/

#include "DSC_connectivity.h"
#include "DSC_tca.h"

char m_aacRefName[EN_REFLECT_MAX][16] = {
    DEF_CV_REF_MUX,
    DEF_CV_REF_LINE_T,
    DEF_CV_REF_CLIENT_T,
    DEF_CV_REF_LINE_T_P,
    DEF_CV_REF_CLIENT_T_P
};

/**************************************************************************************************/
/* definition of get tca funcion for DSC_CvGetConfig(ST_CV_CONFIG *pstConfig) using               */
/**************************************************************************************************/
/* @ {                                                                                            */
#define __get_cv_tca(tca_idx, differ_idx) do {                                       \
        DSC_GetTcaThresh(tca_idx, pstTcaInfo, &(pstConfig->astThr[differ_idx].dHighThr), \
                         &(pstConfig->astThr[differ_idx].dLowThr)); \
} while (0)
/* } @                                                                                            */


/***FUNC+******************************************************************************************/
/* Name   : DSC_CvGetState                                                                        */
/* Descrp : Get connectivity validation state                                                     */
/* Input  : NONE                                                                                  */
/* Output : pstState -- A pointer of cv state buffer                                              */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_CvGetState(ST_CV_STATE *pstState)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;
    char aacXpath[32][OPLK_PATH_MAX_LEN] = {0};

    memset(aacXpath, 0x00, sizeof(aacXpath));

    ANONYMOUS_ST astMaps[35] = {

        /* mux connectivity                                                     */
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_R_OUT_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dOutputPowerR),
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_R_IN_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dInputPowerR),
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_R_DIFFER_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dDifferPowerR),
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_T_OUT_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dOutputPowerT),
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_T_IN_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dInputPowerT),
        ANONYMOUS_MAP_ENTRY(DEF_CV_XPATH_MUX_T_DIFFER_POWER, SR_DECIMAL64_T, (char *)&pstState->stMux.dDifferPowerT),

        /* client-w connectivity                                                */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[0], DEF_CV_XPATH_CLIENT_DFB_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dDfbOutputPower),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[1], DEF_CV_XPATH_CLIENT_R_OUT_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dOutputPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[2], DEF_CV_XPATH_CLIENT_R_IN_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dInputPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[3], DEF_CV_XPATH_CLIENT_R_DIFFER_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dDifferPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[4], DEF_CV_XPATH_CLIENT_T_OUT_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dOutputPowerT),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[5], DEF_CV_XPATH_CLIENT_T_IN_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dInputPowerT),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[6], DEF_CV_XPATH_CLIENT_T_DIFFER_POWER, DEF_CV_CLIENT_NAME_CLIENT_W),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_W].dDifferPowerT),

        /* client-p connectivity                                                 */
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[7], DEF_CV_XPATH_CLIENT_DFB_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dDfbOutputPower),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[8], DEF_CV_XPATH_CLIENT_R_OUT_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dOutputPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[9], DEF_CV_XPATH_CLIENT_R_IN_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dInputPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[10], DEF_CV_XPATH_CLIENT_R_DIFFER_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dDifferPowerR),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[11], DEF_CV_XPATH_CLIENT_T_OUT_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dOutputPowerT),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[12], DEF_CV_XPATH_CLIENT_T_IN_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dInputPowerT),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[13], DEF_CV_XPATH_CLIENT_T_DIFFER_POWER, DEF_CV_CLIENT_NAME_CLIENT_P),
                            SR_DECIMAL64_T, (char *)&pstState->astClient[EN_CLIENT_P].dDifferPowerT),
    };

    int j = 20, k = 14;
    for (int i = EN_REFLECT_MUX; i < EN_REFLECT_MAX; i++) {
        ANONYMOUS_ST mapOutput = ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[k++], DEF_CV_XPATH_REF_OUTPUT_POWER, m_aacRefName[i]),
                                                     SR_DECIMAL64_T, (char *)&pstState->astReflect[i].dOutputPower);
        memcpy(&astMaps[j++], &mapOutput, sizeof(ANONYMOUS_ST));

        ANONYMOUS_ST mapRef = ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[k++], DEF_CV_XPATH_REF_REF_POWER, m_aacRefName[i]),
                                                  SR_DECIMAL64_T, (char *)&pstState->astReflect[i].dRefPower);
        memcpy(&astMaps[j++], &mapRef, sizeof(ANONYMOUS_ST));

        ANONYMOUS_ST mapDiff = ANONYMOUS_MAP_ENTRY(__sprintfStr(aacXpath[k++], DEF_CV_XPATH_REF_DIFFER_POWER, m_aacRefName[i]),
                                                   SR_DECIMAL64_T, (char *)&pstState->astReflect[i].dDifferPower);
        memcpy(&astMaps[j++], &mapDiff, sizeof(ANONYMOUS_ST));
    }

    pstSess = DSC_GetOperSession();

    if (NULL == pstSess || NULL == pstState) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p", pstSess, pstState);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset((void *)pstState, 0, sizeof(ST_CV_STATE));
    rc = DSC_GetItems(pstSess, DEF_CV_XPATH_STATE_ALL, astMaps, sizeof(astMaps));

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CvGetConfig                                                                       */
/* Descrp : Get connectivity validation config                                                    */
/* Input  : NONE                                                                                  */
/* Output : pstState -- A pointer of cv config buffer                                             */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_CvGetConfig(ST_CV_CONFIG *pstConfig)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *pstTcaInfo = NULL;
    size_t nCount = 0;

    if (NULL == pstConfig) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* Set ivalid initial value                                                 */
    for (int i = 0; i < EN_DIFFER_MAX; i++) {
        pstConfig->astThr[i].dHighThr = DEF_CV_DIFF_THR_IVALID_VAL;
        pstConfig->astThr[i].dLowThr = DEF_CV_DIFF_THR_IVALID_VAL;
    }

    /* Get threshold from TCA                                                   */
    rc = DSC_GetTcaList(NULL, &pstTcaInfo, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
        goto ERR_LABEL;
    }

    __get_cv_tca(TCA_IDX_CV_REF, EN_DIFFER_REF);
    __get_cv_tca(TCA_IDX_CV_MUX_R, EN_DIFFER_MUX_R);
    __get_cv_tca(TCA_IDX_CV_MUX_T, EN_DIFFER_MUX_T);
    __get_cv_tca(TCA_IDX_CV_CLIENT_W_R, EN_DIFFER_CLIENT_W_R);
    __get_cv_tca(TCA_IDX_CV_CLIENT_W_T, EN_DIFFER_CLIENT_W_T);
    __get_cv_tca(TCA_IDX_CV_CLIENT_P_R, EN_DIFFER_CLIENT_P_R);
    __get_cv_tca(TCA_IDX_CV_CLIENT_P_T, EN_DIFFER_CLIENT_P_T);

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CvSetDifferThr                                                                    */
/* Descrp : Set connectivity validation threshold                                                 */
/* Input  : enIdx -- differ threshold index                                                       */
/* Input  : dHighThr -- high threshold                                                            */
/* Input  : dLowThr --  low threshold                                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_CvSetDifferThr(EN_DIFFER enIdx, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    EN_TCA_IDX enTcaIdx = 0;

    switch (enIdx) {
    case EN_DIFFER_REF: enTcaIdx = TCA_IDX_CV_REF; break;
    case EN_DIFFER_MUX_R: enTcaIdx = TCA_IDX_CV_MUX_R; break;
    case EN_DIFFER_MUX_T: enTcaIdx = TCA_IDX_CV_MUX_T; break;
    case EN_DIFFER_CLIENT_W_R: enTcaIdx = TCA_IDX_CV_CLIENT_W_R; break;
    case EN_DIFFER_CLIENT_W_T: enTcaIdx = TCA_IDX_CV_CLIENT_W_T; break;
    case EN_DIFFER_CLIENT_P_R: enTcaIdx = TCA_IDX_CV_CLIENT_P_R; break;
    case EN_DIFFER_CLIENT_P_T: enTcaIdx = TCA_IDX_CV_CLIENT_P_T; break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "Error input differ: %d, tca: %d, high: %.2f, low: %.2f",
                  enIdx, enTcaIdx, dHighThr, dLowThr);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    rc = DSC_SetTcaThreshold(NULL, enTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set threshold: %s, differ: %d, tca: %d, high: %.2f, low: %.2f",
                  sr_strerror(rc), enIdx, enTcaIdx, dHighThr, dLowThr);
    }

ERR_LABEL:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CvRpcNameStrToIdx                                                                 */
/* Descrp : CV rpc name string to index                                                           */
/* Input  : pcName -- function name                                                               */
/*        : bIsEnable -- enable config                                                            */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_CV_RPC_IDX :                                                              */
/***FUNC-******************************************************************************************/
EN_CV_RPC_IDX
DSC_CvRpcNameStrToIdx(char *pcName)
{
    if (NULL == pcName) {
        return EN_CV_RPC_IDX_MAX;
    }

    if (0 == strcmp(pcName, DEF_CV_RPC_NAME_ALL)) {
        return EN_CV_RPC_IDX_ALL;
    } else if (0 == strcmp(pcName, DEF_CV_RPC_NAME_REFLECT)) {
        return EN_CV_RPC_IDX_REFLECT;
    } else if (0 == strcmp(pcName, DEF_CV_RPC_NAME_MUX)) {
        return EN_CV_RPC_IDX_MUX;
    } else if (0 == strcmp(pcName, DEF_CV_RPC_NAME_CLIENT_W)) {
        return EN_CV_RPC_IDX_CLIENT_W;
    } else if (0 == strcmp(pcName, DEF_CV_RPC_NAME_CLIENT_P)) {
        return EN_CV_RPC_IDX_CLIENT_P;
    } else {
        return EN_CV_RPC_IDX_MAX;
    }

}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CvRpcNameIdxToStr                                                                 */
/* Descrp : CV rpc name idx to string                                                             */
/* Input  : enIdx -- function index                                                               */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg name string :                                                                */
/***FUNC-******************************************************************************************/
char *
DSC_CvRpcNameIdxToStr(EN_CV_RPC_IDX enIdx)
{
    switch (enIdx) {
    case EN_CV_RPC_IDX_ALL:
        return strdup(DEF_CV_RPC_NAME_ALL);
    case EN_CV_RPC_IDX_REFLECT:
        return strdup(DEF_CV_RPC_NAME_REFLECT);
    case EN_CV_RPC_IDX_MUX:
        return strdup(DEF_CV_RPC_NAME_MUX);
    case EN_CV_RPC_IDX_CLIENT_W:
        return strdup(DEF_CV_RPC_NAME_CLIENT_W);
    case EN_CV_RPC_IDX_CLIENT_P:
        return strdup(DEF_CV_RPC_NAME_CLIENT_P);
    default:
        return strdup("");
    }

    return strdup("");
}

/***FUNC+******************************************************************************************/
/* Name   : DSC_CvRpcEnable                                                                       */
/* Descrp : Set connectivity validation  enable/disable                                           */
/* Input  : pcName -- function name                                                               */
/*        : bIsEnable -- enable config                                                            */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg SR_ERR_OK :                                                                  */
/*              @arg other SR_ERR_*:                                                              */
/***FUNC-******************************************************************************************/
int
DSC_CvRpcEnable(EN_CV_RPC_IDX enIdx, bool bIsEnable)
{
    size_t nInputCnt = 0;
    sr_val_t *pstInput = NULL;
    size_t nOutputCnt = 0;
    sr_val_t *pstOutput = NULL;
    int rc = SR_ERR_OK;

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null: %p", pstSess);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    if (enIdx >= EN_CV_RPC_IDX_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "input error index: %d", enIdx);
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    /* allocate input values filename                                           */
    nInputCnt = 2;
    rc = sr_new_values(nInputCnt, &pstInput);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    rc = sr_val_set_xpath(&pstInput[0], DEF_CV_XPATH_RPC_NAME);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_CV_XPATH_RPC_NAME, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[0].type = SR_STRING_T;
    pstInput[0].data.string_val = DSC_CvRpcNameIdxToStr(enIdx);

    rc = sr_val_set_xpath(&pstInput[0], DEF_CV_XPATH_RPC_VAL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", DEF_CV_XPATH_RPC_VAL, sr_strerror(rc));
        goto ERR_LABEL;
    }
    pstInput[1].type = SR_BOOL_T;
    pstInput[1].data.bool_val = bIsEnable;

    /* execute RPC                                                              */
    rc = DSC_RpcSend(pstSess, DEF_CV_XPATH_RPC_ENABLE, pstInput, nInputCnt, &pstOutput, &nOutputCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", DEF_CV_XPATH_RPC_ENABLE, sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }

    return DSC_ProcessRPCResponse(pstOutput, nOutputCnt);

ERR_LABEL:
    if (pstInput != NULL) {
        sr_free_values(pstInput, nInputCnt);
        pstInput = NULL;
    }
    return rc;
}

int
DSC_SetCvProtectionEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_CV_XPATH_ENABLE_CV_PROTECTION);
    rc = set_bool_item(sess, acXpath, bEnable);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetCvProtectionEnable(sr_session_ctx_t *sess, bool *pbEnable)
{
    int rc = SR_ERR_OK;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};

    if (NULL == sess) {
        sess = DSC_GetSession();
    }

    if (NULL == sess || NULL == pbEnable) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto ERR_LABEL;
    }

    memset(acXpath, 0, sizeof(acXpath));
    snprintf(acXpath, sizeof(acXpath), DEF_CV_XPATH_ENABLE_CV_PROTECTION);
    rc = get_bool_item(sess, acXpath, pbEnable);
    if (SR_ERR_NOT_FOUND == rc) {
        LOG_WRITE(EN_LOG_NOTICE, "NOT find mux enable-cv-protection, return defalut true.");
        *pbEnable = false;
        rc = SR_ERR_OK;
    } else if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_bool_item(%s): %s", acXpath, sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

