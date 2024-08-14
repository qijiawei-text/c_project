/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_msft.c
* Description:   only used for MSFT, parse requested json data and
*                      get business data for reponsing http/https request
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/17  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "util_inc.h"
#include "rest_edfa.h"

int
REST_GetAmpCardCount()
{
    int iCount = 0;

#if defined(__OLSP__)
    iCount = EN_REST_AMP_CARD_MAX;
#elif defined(__HCF__)
    iCount = EN_REST_AMP_P_CARD;
#else
    iCount = EN_REST_AMP_P_CARD;   /* default */
#endif

    return iCount;
}

int
REST_GetLinePathMax()
{
    int iLinePathMax = 0;

#if defined(__OLSP__)
    iLinePathMax = EN_REST_OPTICAL_PATH_MAX;
#elif defined(__HCF__)
    iLinePathMax = EN_REST_OPTICAL_PATH_P;
#else
    iLinePathMax = EN_REST_OPTICAL_PATH_P;       /* default */
#endif

    return iLinePathMax;
}

bool
REST_IsAmpCardOnline(REST_AMP_CARD_TYPE_EN enAmpCardType)
{
    const char *pcCpName = (EN_REST_AMP_W_CARD == enAmpCardType) ? CIRCUIT_PACK_AMP : CIRCUIT_PACK_AMP_P;

    return DSC_IsCircuitPackInService(pcCpName);
}

int
REST_ParseEdfaPA(const cJSON *pstJson, REST_EDFA_PA_ST *pstEdfaPA)
{
    REST_EDFA_PA_ST stEdfaPA;
    char aacBuf[2][32] = {{0}};
    JSON_OBJ_ST astJsonObj[] = {{STR_TARGET_GAIN, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dGain), sizeof(stEdfaPA.dGain)},
                                {STR_TARGET_GAIN_TILT, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dGainTilt), sizeof(stEdfaPA.dGainTilt)},
                                {STR_TARGET_VOA, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dVoa), sizeof(stEdfaPA.dVoa)},
                                {STR_APR_ENABLE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[0]), sizeof(aacBuf[0])},
                                {STR_STATE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[1]), sizeof(aacBuf[1])},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stEdfaPA, 0, sizeof(stEdfaPA));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(aacBuf[0], STR_ENABLE)) {
        stEdfaPA.bAprEnable = true;
    } else if (0 == strcasecmp(aacBuf[0], STR_DISABLE)) {
        stEdfaPA.bAprEnable = false;
    } else {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(aacBuf[1], STR_UP)) {
        stEdfaPA.bState = true;
    } else if (0 == strcasecmp(aacBuf[1], STR_DOWN)) {
        stEdfaPA.bState = false;
    } else {
        goto ERR_LABEL;
    }

    memcpy(pstEdfaPA, &stEdfaPA, sizeof(stEdfaPA));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_SetEdfaPA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA)
{
    int rc = SR_ERR_OK;
    REST_EDFA_FLAG_ST stExistFlag;

    memset(&stExistFlag, 0, sizeof(stExistFlag));

    if (OPLK_OK != REST_SetEachFlag(&stExistFlag, sizeof(stExistFlag), true)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = REST_SetEdfaPaOptional(enAmpCardType, pstEdfaPA, &stExistFlag);

END_LABEL:

    return rc;
}

int
REST_GetEdfaPA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA)
{
    int rc = SR_ERR_OK;
    EDFA_CFG_ST stEdfaCfg;
    double dValue = 0;
    char *pcEdfaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_PA_CFG_KEY : EDFA_PAP_CFG_KEY;
    char *pcEvoaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PA_CFG_KEY : EVOA_PA_P_CFG_KEY;

    memset(&stEdfaCfg, 0, sizeof(stEdfaCfg));
    rc = DSC_GetEdfaConfig(pcEdfaCfgKey, &stEdfaCfg, sizeof(stEdfaCfg));
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
    pstEdfaPA->dGain = stEdfaCfg.dGainSetPoint;
    pstEdfaPA->dGainTilt = stEdfaCfg.dTiltSetPoint;
    pstEdfaPA->bState = stEdfaCfg.bOutputEnable;
    pstEdfaPA->bAprEnable = stEdfaCfg.bAprEnable;

    rc = DSC_GetEvoaAttn(pcEvoaCfgKey, &dValue);
    if (rc != SR_ERR_OK) {
        goto END_LABEL;
    }
    pstEdfaPA->dVoa = dValue;

END_LABEL:
    return rc;
}


int
REST_ParseEdfaBA(const cJSON *pstJson, REST_EDFA_BA_ST *pstEdfaBA)
{
    REST_EDFA_BA_ST stEdfaBA;
    char aacBuf[2][32] = {{0}};
    JSON_OBJ_ST astJsonObj[] = {{STR_TARGET_GAIN, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dGain), sizeof(stEdfaBA.dGain)},
                                {STR_TARGET_GAIN_TILT, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dGainTilt), sizeof(stEdfaBA.dGainTilt)},
                                {STR_TARGET_VOA, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dVoa), sizeof(stEdfaBA.dVoa)},
                                {STR_APR_ENABLE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[0]), sizeof(aacBuf[0])},
                                {STR_STATE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[1]), sizeof(aacBuf[1])},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};


    memset(&stEdfaBA, 0, sizeof(stEdfaBA));

    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(aacBuf[0], STR_ENABLE)) {
        stEdfaBA.bAprEnable = true;
    } else if (0 == strcasecmp(aacBuf[0], STR_DISABLE)) {
        stEdfaBA.bAprEnable = false;
    } else {
        goto ERR_LABEL;
    }

    if (0 == strcasecmp(aacBuf[1], STR_UP)) {
        stEdfaBA.bState = true;
    } else if (0 == strcasecmp(aacBuf[1], STR_DOWN)) {
        stEdfaBA.bState = false;
    } else {
        goto ERR_LABEL;
    }

    memcpy(pstEdfaBA, &stEdfaBA, sizeof(stEdfaBA));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_SetEdfaBA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA)
{
    int rc = SR_ERR_OK;
    REST_EDFA_FLAG_ST stExistFlag;

    memset(&stExistFlag, 0, sizeof(stExistFlag));

    if (OPLK_OK != REST_SetEachFlag(&stExistFlag, sizeof(stExistFlag), true)) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    rc = REST_SetEdfaBaOptional(enAmpCardType, pstEdfaBA, &stExistFlag);

END_LABEL:

    return rc;
}

int
REST_GetEdfaBA(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA)
{
    int rc = SR_ERR_OK;
    EDFA_CFG_ST stEdfaCfg;
    double dValue = 0;
    char *pcEdfaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_BA_CFG_KEY : EDFA_BAP_CFG_KEY;
    char *pcEvoaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PADDING_CFG_KEY : EVOA_PADDING_P_CFG_KEY;

    memset(&stEdfaCfg, 0, sizeof(stEdfaCfg));
    rc = DSC_GetEdfaConfig(pcEdfaCfgKey, &stEdfaCfg, sizeof(stEdfaCfg));
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    rc = DSC_GetEvoaAttn(pcEvoaCfgKey, &dValue);
    if (rc != SR_ERR_OK) {
        goto ERR_LABEL;
    }

    pstEdfaBA->dGain = stEdfaCfg.dGainSetPoint;
    pstEdfaBA->dGain = pstEdfaBA->dGain < -60 ? -60.00 : pstEdfaBA->dGain;
    pstEdfaBA->dGainTilt = stEdfaCfg.dTiltSetPoint;
    pstEdfaBA->dVoa = dValue;
    pstEdfaBA->bState = stEdfaCfg.bOutputEnable;
    pstEdfaBA->bAprEnable = stEdfaCfg.bAprEnable;

    return SR_ERR_OK;
ERR_LABEL:
    return rc;
}


int
REST_ParseEdfaPaOptional(const cJSON *pstJson, REST_EDFA_PA_ST *pstEdfaPA, REST_EDFA_FLAG_ST *pstExistFlag)
{
    REST_EDFA_PA_ST stEdfaPA;
    REST_EDFA_FLAG_ST stPaExistFlag;
    char aacBuf[2][32] = {{0}};
    JSON_OBJ_NEW_ST astJsonObj[] = {{STR_TARGET_GAIN, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dGain), sizeof(stEdfaPA.dGain), &(stPaExistFlag.bGainFlag)},
                                    {STR_TARGET_GAIN_TILT, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dGainTilt), sizeof(stEdfaPA.dGainTilt), &(stPaExistFlag.bTiltFlag)},
                                    {STR_TARGET_VOA, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaPA.dVoa), sizeof(stEdfaPA.dVoa), &(stPaExistFlag.bVoaFlag)},
                                    {STR_APR_ENABLE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[0]), sizeof(aacBuf[0]), &(stPaExistFlag.bAprEnableFlag)},
                                    {STR_STATE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[1]), sizeof(aacBuf[1]), &(stPaExistFlag.bStateFlag)},
                                    {NULL, EN_DATA_TYPE_MAX, NULL, 0, NULL}};

    memset(&stEdfaPA, 0, sizeof(stEdfaPA));
    memset(&stPaExistFlag, 0, sizeof(stPaExistFlag));

    if (OPLK_OK != JSON_GetObjItemsOptional(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (stPaExistFlag.bAprEnableFlag) {
        if (0 == strcasecmp(aacBuf[0], STR_ENABLE)) {
            stEdfaPA.bAprEnable = true;
        } else if (0 == strcasecmp(aacBuf[0], STR_DISABLE)) {
            stEdfaPA.bAprEnable = false;
        } else {
            goto ERR_LABEL;
        }
    }

    if (stPaExistFlag.bStateFlag) {
        if (0 == strcasecmp(aacBuf[1], STR_UP)) {
            stEdfaPA.bState = true;
        } else if (0 == strcasecmp(aacBuf[1], STR_DOWN)) {
            stEdfaPA.bState = false;
        } else {
            goto ERR_LABEL;
        }
    }

    memcpy(pstEdfaPA, &stEdfaPA, sizeof(stEdfaPA));
    memcpy(pstExistFlag, &stPaExistFlag, sizeof(stPaExistFlag));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static int
__REST_CheckEdfaPaOptional(REST_EDFA_PA_ST *pstEdfaPA, REST_EDFA_FLAG_ST *pstExistFlag)
{

    if (NULL == pstEdfaPA || NULL == pstExistFlag) {
        goto ERR_LABEL;
    }

    if ((false == pstExistFlag->bGainFlag) && (false == pstExistFlag->bTiltFlag) &&
        (false == pstExistFlag->bVoaFlag) && (false == pstExistFlag->bStateFlag) &&
        (false == pstExistFlag->bAprEnableFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "no changes for pa");
        goto ERR_LABEL;
    }
#if 0
    if ((pstExistFlag->bGainFlag) &&
        (pstEdfaPA->dGain < REST_PA_GAIN_MIN || pstEdfaPA->dGain > REST_PA_GAIN_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "pa gain out range %.02f ", pstEdfaPA->dGain);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bTiltFlag) &&
        (pstEdfaPA->dGainTilt < REST_PA_GAIN_TILT_MIN || pstEdfaPA->dGainTilt > REST_PA_GAIN_TILT_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "pa gain tilt out range %.02f ", pstEdfaPA->dGainTilt);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bVoaFlag) &&
        (pstEdfaPA->dTcdc < REST_PA_TDCM_CD_MIN || pstEdfaPA->dTcdc > REST_PA_TDCM_CD_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "pa tdcm cd out range %0.02f ", pstEdfaPA->dTcdc);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bStateFlag) &&
        (pstEdfaPA->bState != REST_PUMP_STATE_ENABLE && pstEdfaPA->bState != REST_PUMP_STATE_DISABLE)) {
        LOG_WRITE(EN_LOG_ERROR, "pa state out range %d ", pstEdfaPA->bState);
        goto ERR_LABEL;
    }
#endif

    return SR_ERR_OK;
ERR_LABEL:
    return SR_ERR_INVAL_ARG;
}

int
REST_SetEdfaPaOptional(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_PA_ST *pstEdfaPA, REST_EDFA_FLAG_ST *pstExistFlag)
{
    int rc = SR_ERR_OK;
    REST_EDFA_PA_ST stOldEdfaPA;
    char *pcEdfaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_PA_CFG_KEY : EDFA_PAP_CFG_KEY;
    char *pcEvoaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PA_CFG_KEY : EVOA_PA_P_CFG_KEY;

    rc = __REST_CheckEdfaPaOptional(pstEdfaPA, pstExistFlag);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    memset(&stOldEdfaPA, 0x00, sizeof(stOldEdfaPA));
    rc = REST_GetEdfaPA(enAmpCardType, &stOldEdfaPA);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    if (pstExistFlag->bGainFlag) {
        rc = DSC_SetEdfaGain(pcEdfaCfgKey, pstEdfaPA->dGain);
        if (SR_ERR_OK != rc) {
            goto ERR_LABEL;
        }
    }

    if (pstExistFlag->bTiltFlag) {
        rc = DSC_SetEdfaTilt(pcEdfaCfgKey, pstEdfaPA->dGainTilt);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bAprEnableFlag) {
        rc = DSC_SetEdfaAprEnable(pcEdfaCfgKey, pstEdfaPA->bAprEnable);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bStateFlag) {
        rc = DSC_SetEdfaOutputEnable(pcEdfaCfgKey, pstEdfaPA->bState);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bVoaFlag) {
        rc = DSC_SetEvoaAttn(pcEvoaCfgKey, pstEdfaPA->dVoa);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    return SR_ERR_OK;

DATA_ROLLBACK_LABEL:

    if (pstExistFlag->bGainFlag) {
        DSC_SetEdfaGain(pcEdfaCfgKey, stOldEdfaPA.dGain);
    }

    if (pstExistFlag->bTiltFlag) {
        DSC_SetEdfaTilt(pcEdfaCfgKey, stOldEdfaPA.dGainTilt);
    }

    if (pstExistFlag->bStateFlag) {
        DSC_SetEdfaOutputEnable(pcEdfaCfgKey, stOldEdfaPA.bState);
    }

    if (pstExistFlag->bAprEnableFlag) {
        DSC_SetEdfaAprEnable(pcEdfaCfgKey, stOldEdfaPA.bAprEnable);
    }

    if (pstExistFlag->bVoaFlag) {
        DSC_SetEvoaAttn(pcEvoaCfgKey, pstEdfaPA->dVoa);
    }

ERR_LABEL:
    return rc;

}

int
REST_ParseEdfaBaOptional(const cJSON *pstJson, REST_EDFA_BA_ST *pstEdfaBA, REST_EDFA_FLAG_ST *pstExistFlag)
{
    REST_EDFA_BA_ST stEdfaBA;
    char aacBuf[2][32] = {{0}};
    REST_EDFA_FLAG_ST stBaExistFlag;
    JSON_OBJ_NEW_ST astJsonObj[] = {{STR_TARGET_GAIN, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dGain), sizeof(stEdfaBA.dGain), &(stBaExistFlag.bGainFlag)},
                                    {STR_TARGET_GAIN_TILT, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dGainTilt), sizeof(stEdfaBA.dGainTilt), &(stBaExistFlag.bTiltFlag)},
                                    {STR_TARGET_VOA, EN_DATA_TYPE_DOUBLE, TAKE_VOID_ADDRESS(stEdfaBA.dVoa), sizeof(stEdfaBA.dVoa), &(stBaExistFlag.bVoaFlag)},
                                    {STR_APR_ENABLE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[0]), sizeof(aacBuf[0]), &(stBaExistFlag.bAprEnableFlag)},
                                    {STR_STATE, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(aacBuf[1]), sizeof(aacBuf[1]), &(stBaExistFlag.bStateFlag)},
                                    {NULL, EN_DATA_TYPE_MAX, NULL, 0, NULL}};


    memset(&stEdfaBA, 0, sizeof(stEdfaBA));
    memset(&stBaExistFlag, 0, sizeof(stBaExistFlag));

    if (OPLK_OK != JSON_GetObjItemsOptional(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    if (stBaExistFlag.bAprEnableFlag) {
        if (0 == strcasecmp(aacBuf[0], STR_ENABLE)) {
            stEdfaBA.bAprEnable = true;
        } else if (0 == strcasecmp(aacBuf[0], STR_DISABLE)) {
            stEdfaBA.bAprEnable = false;
        } else {
            goto ERR_LABEL;
        }
    }

    if (stBaExistFlag.bStateFlag) {
        if (0 == strcasecmp(aacBuf[1], STR_UP)) {
            stEdfaBA.bState = true;
        } else if (0 == strcasecmp(aacBuf[1], STR_DOWN)) {
            stEdfaBA.bState = false;
        } else {
            goto ERR_LABEL;
        }
    }

    memcpy(pstEdfaBA, &stEdfaBA, sizeof(stEdfaBA));
    memcpy(pstExistFlag, &stBaExistFlag, sizeof(stBaExistFlag));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static int
__REST_CheckEdfaBaOptional(REST_EDFA_BA_ST *pstEdfaBA, REST_EDFA_FLAG_ST *pstExistFlag)
{
    if (NULL == pstEdfaBA || NULL == pstExistFlag) {
        goto ERR_LABEL;
    }

    if ((false == pstExistFlag->bGainFlag) && (false == pstExistFlag->bTiltFlag) &&
        (false == pstExistFlag->bVoaFlag) && (false == pstExistFlag->bStateFlag) &&
        (false == pstExistFlag->bAprEnableFlag)) {
        LOG_WRITE(EN_LOG_ERROR, "no changes for ba");
        goto ERR_LABEL;
    }

    /* check parameters range by yang model */
#if 0
    if ((pstExistFlag->bGainFlag) &&
        (pstEdfaBA->dGain < REST_BA_GAIN_MIN || pstEdfaBA->dGain > REST_BA_GAIN_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "ba gain out range %.02f ", pstEdfaBA->dGain);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bTiltFlag) &&
        (pstEdfaBA->dGainTilt < REST_BA_GAIN_TILT_MIN || pstEdfaBA->dGainTilt > REST_BA_GAIN_TILT_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "ba gain tilt out range %.02f ", pstEdfaBA->dGainTilt);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bVoaFlag) &&
        (pstEdfaBA->dVoa < REST_BA_PADDING_VOA_MIN || pstEdfaBA->dVoa > REST_BA_PADDING_VOA_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "ba padding voa cd out range %.02f ", pstEdfaBA->dVoa);
        goto ERR_LABEL;
    }

    if ((pstExistFlag->bStateFlag) &&
        (pstEdfaBA->iState != REST_PUMP_STATE_ENABLE && pstEdfaBA->iState != REST_PUMP_STATE_DISABLE)) {
        LOG_WRITE(EN_LOG_ERROR, "ba state out range %d ", pstEdfaBA->iState);
        goto ERR_LABEL;
    }
#endif
    return SR_ERR_OK;
ERR_LABEL:
    return SR_ERR_INVAL_ARG;
}

int
REST_SetEdfaBaOptional(REST_AMP_CARD_TYPE_EN enAmpCardType, REST_EDFA_BA_ST *pstEdfaBA, REST_EDFA_FLAG_ST *pstExistFlag)
{
    int rc = SR_ERR_OK;
    REST_EDFA_BA_ST stOldEdfaBA;
    char *pcEdfaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EDFA_BA_CFG_KEY : EDFA_BAP_CFG_KEY;
    char *pcEvoaCfgKey = (EN_REST_AMP_W_CARD == enAmpCardType) ? EVOA_PADDING_CFG_KEY : EVOA_PADDING_P_CFG_KEY;

    rc = __REST_CheckEdfaBaOptional(pstEdfaBA, pstExistFlag);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    /* rollback old data when excuting failed */
    memset(&stOldEdfaBA, 0x00, sizeof(stOldEdfaBA));
    rc = REST_GetEdfaBA(enAmpCardType, &stOldEdfaBA);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    if (pstExistFlag->bGainFlag) {
        rc = DSC_SetEdfaGain(pcEdfaCfgKey, pstEdfaBA->dGain);
        if (SR_ERR_OK != rc) {
            goto ERR_LABEL;
        }
    }

    if (pstExistFlag->bTiltFlag) {
        rc = DSC_SetEdfaTilt(pcEdfaCfgKey, pstEdfaBA->dGainTilt);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bVoaFlag) {
        rc = DSC_SetEvoaAttn(pcEvoaCfgKey, (double)pstEdfaBA->dVoa);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bAprEnableFlag) {
        rc = DSC_SetEdfaAprEnable(pcEdfaCfgKey, pstEdfaBA->bAprEnable);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    if (pstExistFlag->bStateFlag) {
        rc = DSC_SetEdfaOutputEnable(pcEdfaCfgKey, pstEdfaBA->bState);
        if (SR_ERR_OK != rc) {
            goto DATA_ROLLBACK_LABEL;
        }
    }

    return SR_ERR_OK;

DATA_ROLLBACK_LABEL:

    if (pstExistFlag->bGainFlag) {
        DSC_SetEdfaGain(pcEdfaCfgKey, stOldEdfaBA.dGain);
    }

    if (pstExistFlag->bTiltFlag) {
        DSC_SetEdfaTilt(pcEdfaCfgKey, stOldEdfaBA.dGainTilt);
    }

    if (pstExistFlag->bVoaFlag) {
        DSC_SetEvoaAttn(pcEvoaCfgKey, stOldEdfaBA.dVoa);
    }

    if (pstExistFlag->bAprEnableFlag) {
        DSC_SetEdfaAprEnable(pcEdfaCfgKey, pstEdfaBA->bAprEnable);
    }

    if (pstExistFlag->bStateFlag) {
        DSC_SetEdfaOutputEnable(pcEdfaCfgKey, stOldEdfaBA.bState);
    }

ERR_LABEL:
    return rc;
}

