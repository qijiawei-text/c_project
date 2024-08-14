
/**
 * Copyright (c) Oplink Communications, LLC
 */
#include <stdio.h>

#include "DSC_pam4.h"
#include "DSC_tca.h"

int
DSC_SetPam4RxThreshold(double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;

    rc = DSC_SetTcaThreshold(NULL, TCA_IDX_PAM4_RX, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set pam4 rx thresh: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetPam4TxThreshold(double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;

    rc = DSC_SetTcaThreshold(NULL, TCA_IDX_PAM4_TX, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set pam4 tx thresh: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetPam4BerThreshold(double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;

    rc = DSC_SetTcaThreshold(NULL, TCA_IDX_PAM4_BER, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set pam4 tx thresh: %s", sr_strerror(rc));
    }

    return rc;
}

int
DSC_SetPam4TxEnable(bool state)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetPam4TxEnable xpath: %s, state : %s", PAM4_XPATH_TX_ENABLE, state ? "enable" : "disable");

    rc = set_bool_item(sess, PAM4_XPATH_TX_ENABLE, state);
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
DSC_SetPam4WavelenNum(uint32_t u32WavelenNum)
{
    int rc = SR_ERR_OK;

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "DSC_SetPam4WavelenNum xpath: %s, dispersn : %u", PAM4_XPATH_WAVELEN_NUM, u32WavelenNum);

    rc = set_uint32_item(sess, PAM4_XPATH_WAVELEN_NUM, u32WavelenNum);
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
DSC_GetPam4Info(ST_PAM4_INFO *psPam4Info, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;

    ANONYMOUS_ST astPam4Maps[] = {
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_TX_ENABLE, SR_BOOL_T, (char *)(&psPam4Info->bTxenable)),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_BER, SR_STRING_T, psPam4Info->acBer),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_LOCK_STATE, SR_ENUM_T, psPam4Info->acLockState),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_ADD_POWER, SR_DECIMAL64_T, (char *)(&psPam4Info->dTxPower)),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_DROP_POWER, SR_DECIMAL64_T, (char *)(&psPam4Info->dRxPower)),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_LASER_TEMP, SR_DECIMAL64_T, (char *)(&psPam4Info->dLaserTemp)),
        ANONYMOUS_MAP_ENTRY(PAM4_XPATH_CASE_TEMP, SR_DECIMAL64_T, (char *)(&psPam4Info->dCaseTemp)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == psPam4Info || size < sizeof(ST_PAM4_INFO)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)psPam4Info, 0x00, size);
    rc = DSC_GetItems(sess, PAM4_XPATH_ALL, astPam4Maps, sizeof(astPam4Maps));

    /* Get thresh from TCA */
    {
        int rc = SR_ERR_OK;
        TCA_INFO_ST *pstTcaInfo = NULL;
        size_t count = 0;

        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
            return rc;
        }

        DSC_GetTcaThresh(TCA_IDX_PAM4_TX, pstTcaInfo, &(psPam4Info->dRxHighThr), &(psPam4Info->dRxLowThr));
        DSC_GetTcaThresh(TCA_IDX_PAM4_RX, pstTcaInfo, &(psPam4Info->dTxHighThr), &(psPam4Info->dTxLowThr));
    }

    return rc;
}

int
DSC_Pam4Reacquire(bool bBoolean)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_PAM4_REACQUIRE_OPTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PAM4_REACQUIRE_OPTION, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;  /* "true" : "false" */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_PAM4_REACQUIRE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_PAM4_REACQUIRE, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_Pam4ClearStatistics(bool bBoolean)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_PAM4_CLEAR_STA_OPTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PAM4_CLEAR_STA_OPTION, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;  /* "true" : "false" */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_PAM4_CLEAR_STATICS, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_PAM4_CLEAR_STATICS, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_Pam4SetFecStatus(unsigned char ucStatus)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_PAM4_FEC_ENABLE_OPTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PAM4_FEC_ENABLE_OPTION, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    input[0].type = SR_ENUM_T;
    input[0].data.enum_val = (1 == ucStatus ? strdup("on") : strdup("off"));  /* "on" : "off" */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_PAM4_FEC_ENABLE, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_PAM4_FEC_ENABLE, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}

int
DSC_Pam4ResetMutify(bool bBoolean)
{
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;
    sr_session_ctx_t *sess = DSC_GetRunningSession();

    /* allocate input values */
    input_cnt = 1;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        return rc;
    }

    rc = sr_val_set_xpath(&input[0], RPC_XPATH_PAM4_RESET_MUTIFY_OPTION);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_val_set_xpath: %s %s", RPC_XPATH_PAM4_RESET_MUTIFY_OPTION, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    input[0].type = SR_BOOL_T;
    input[0].data.bool_val = bBoolean;  /* "true" : "false" */

    /* execute RPC */
    rc = DSC_RpcSend(sess, RPC_XPATH_PAM4_RESET_MUTIFY, input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_RpcSend: %s %s", RPC_XPATH_PAM4_RESET_MUTIFY, sr_strerror(rc));
        sr_free_values(input, input_cnt);
        return rc;
    }

    sr_free_values(input, input_cnt);

    return DSC_ProcessRPCResponse(output, output_cnt);
}
