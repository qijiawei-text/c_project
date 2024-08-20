/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCVCApi.h
   Author:        Zhu WeiAn
   Date:          2022-10-10
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu WeiAn] [2022-10-10] [1.0] [Creator]


*****************************************************************************************************/
#include "DevApiManager.h"

#include "DevCvCApi.h"
#include "DevSlotCApi.h"
#include "DevEdfaCApi.h"
#include "DevMuxCApi.h"
#include "DevOpsCApi.h"

typedef enum {
    EN_REFLECT_MUX = 0,
    EN_REFLECT_LINE_T = 1,
    EN_REFLECT_CLIENT_T = 2,
    EN_REFLECT_LINE_T_P = 3,
    EN_REFLECT_CLIENT_T_P = 4,

    EN_REFLECT_MAX

} EN_REFLECT;


int
Cv_GetMuxStatusData(EnCvLine enCvLine, CCvMuxStatusData *pstMuxStatus)
{
    int rc = OPLK_OK;
    uint32_t uiMuxTOut = OPS_CONN_CNT;
    static int16_t sLastOutputPowerT = -6000;
    static int16_t sLastInputPowerT = -6000;
    COpsStatusData stOpsStatus;

    if (NULL == pstMuxStatus) {
        return OPLK_ERR;
    }   

    pstMuxStatus->sOutputPowerR = -6000;
    pstMuxStatus->sInputPowerR = -6000;
    pstMuxStatus->sOutputPowerT = -6000;
    pstMuxStatus->sInputPowerT = -6000;
    
    memset(&stOpsStatus, 0, sizeof(COpsStatusData));
    rc = OPS_GetStatusData(DEV_TYPE_OPS, &stOpsStatus, OPS_SUB_INDEX_DEFAULT);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "get ops status data error. rc = %d", rc);
        return rc;
    }

    /* Note: stOpsStatus.stCurData.ucSwitchPos, 1: primary, 0: secondary */
    /* ops switch pos == primary/secondary, dfb == primary/secondary, then read corresponding pd power, otherwise remain last value */
    if (CV_LINE_W == enCvLine && (1 == stOpsStatus.stCurData.ucSwitchPos)) {
        uiMuxTOut = OPS_CONN_MUX_T_W_OUT;
    } else if (CV_LINE_P == enCvLine && (0 == stOpsStatus.stCurData.ucSwitchPos)) {
        uiMuxTOut = OPS_CONN_MUX_T_P_OUT;
    }

    pstMuxStatus->sOutputPowerR = stOpsStatus.stConnectionPds.asPd[OPS_CONN_MUX_R_OUT];
    pstMuxStatus->sInputPowerR = stOpsStatus.stConnectionPds.asPd[OPS_CONN_MUX_R_IN];
    pstMuxStatus->sOutputPowerT = (uiMuxTOut != OPS_CONN_CNT) ? stOpsStatus.stConnectionPds.asPd[uiMuxTOut] : sLastOutputPowerT;
    pstMuxStatus->sInputPowerT = (uiMuxTOut != OPS_CONN_CNT) ? stOpsStatus.stConnectionPds.asPd[OPS_CONN_MUX_T_IN] : sLastInputPowerT;

    sLastOutputPowerT = pstMuxStatus->sOutputPowerT;
    sLastInputPowerT = pstMuxStatus->sInputPowerT;

    return rc;
}

int
Cv_GetClientStatusData(EnCvLine enCvLine, CCvClientStatusData *pstClientStatus)
{
    int rc = OPLK_OK;
    uint32_t uiOaType = DEV_TYPE_NONE;
    uint32_t uiClientR = OPS_CONN_CNT;
    uint32_t uiClientT = OPS_CONN_CNT;
    COaStatusData stOaStatus;
    COpsStatusData stOpsStatus;

    if (NULL == pstClientStatus) {
        return OPLK_ERR;
    }

    pstClientStatus->sOutputPowerR = -6000;
    pstClientStatus->sInputPowerR = -6000;
    pstClientStatus->sOutputPowerT = -6000;
    pstClientStatus->sInputPowerT = -6000;
    
    if (CV_LINE_W == enCvLine) {
        uiOaType = DEV_TYPE_BA;
        uiClientR = OPS_CONN_CLIENT_W_R;
        uiClientT = OPS_CONN_CLIENT_W_T;
    } else if (CV_LINE_P == enCvLine) {
        uiOaType = DEV_TYPE_P_BA;
        uiClientR = OPS_CONN_CLIENT_P_R;
        uiClientT = OPS_CONN_CLIENT_P_T;
    }

    memset(&stOaStatus, 0, sizeof(COaStatusData));
    rc = EDFA_GetStatusData(uiOaType, &stOaStatus);
    if (OPLK_OK == rc) {
        pstClientStatus->sOutputPowerR = stOaStatus.stOaCurData.sDfbOutPower2;
        pstClientStatus->sOutputPowerT = stOaStatus.stOaCurData.sDfbOutPower1;
    }

    memset(&stOpsStatus, 0, sizeof(COpsStatusData));
    rc = OPS_GetStatusData(DEV_TYPE_OPS, &stOpsStatus, OPS_SUB_INDEX_DEFAULT);
    if (OPLK_OK == rc) {
        pstClientStatus->sInputPowerR = stOpsStatus.stConnectionPds.asPd[uiClientT];
        pstClientStatus->sInputPowerT = stOpsStatus.stConnectionPds.asPd[uiClientR];
    }

    return rc;
}

int
Cv_GetRef(CCvRef *pstRef)
{
    int rc = OPLK_OK;
    COaStatusData stOaStatus;
    CMuxStatusData stMuxStatus;
    int16_t sAttn = 0;

    if (NULL == pstRef) {
        return OPLK_ERR;
    }

    for (int32_t i = EN_REFLECT_MUX; i < EN_REFLECT_MAX; i++) {
        pstRef[i].sOutputPower = -6000;
        pstRef[i].sRefPower = -6000;
    }
    
    memset(&stOaStatus, 0, sizeof(COaStatusData));
    rc = EDFA_GetStatusData(DEV_TYPE_BA, &stOaStatus);
    if (OPLK_OK == rc) {
        rc = EDFA_GetAttn(DEV_TYPE_BA, &sAttn);
        if (OPLK_OK == rc) {
            pstRef[EN_REFLECT_LINE_T].sOutputPower = stOaStatus.stOaCurData.sOutTotalPower - sAttn;
            pstRef[EN_REFLECT_LINE_T].sRefPower = stOaStatus.stOaCurData.sBackRefPower;
        }
    }

    memset(&stOaStatus, 0, sizeof(COaStatusData));
    rc = EDFA_GetStatusData(DEV_TYPE_PA, &stOaStatus);
    if (OPLK_OK == rc) {
        rc = EDFA_GetAttn(DEV_TYPE_PA, &sAttn);
        if (OPLK_OK == rc) {
            pstRef[EN_REFLECT_CLIENT_T].sOutputPower = stOaStatus.stOaCurData.sOutTotalPower - sAttn;
            pstRef[EN_REFLECT_CLIENT_T].sRefPower = stOaStatus.stOaCurData.sBackRefPower;
        }
    }

    memset(&stOaStatus, 0, sizeof(COaStatusData));
    rc = EDFA_GetStatusData(DEV_TYPE_P_BA, &stOaStatus);
    if (OPLK_OK == rc) {
        rc = EDFA_GetAttn(DEV_TYPE_P_BA, &sAttn);
        if (OPLK_OK == rc) {
            pstRef[EN_REFLECT_LINE_T_P].sOutputPower = stOaStatus.stOaCurData.sOutTotalPower - sAttn;
            pstRef[EN_REFLECT_LINE_T_P].sRefPower = stOaStatus.stOaCurData.sBackRefPower;
        }
    }

    memset(&stOaStatus, 0, sizeof(COaStatusData));
    rc = EDFA_GetStatusData(DEV_TYPE_P_PA, &stOaStatus);
    if (OPLK_OK == rc) {
        rc = EDFA_GetAttn(DEV_TYPE_P_PA, &sAttn);
        if (OPLK_OK == rc) {
            pstRef[EN_REFLECT_CLIENT_T_P].sOutputPower = stOaStatus.stOaCurData.sOutTotalPower - sAttn;
            pstRef[EN_REFLECT_CLIENT_T_P].sRefPower = stOaStatus.stOaCurData.sBackRefPower;
        }
    }

    memset(&stMuxStatus, 0, sizeof(CMuxStatusData));
    rc = MUX_GetStatusData(0, &stMuxStatus);
    if (OPLK_OK == rc) {
        pstRef[EN_REFLECT_MUX].sOutputPower = stMuxStatus.asPdPower[0];
        pstRef[EN_REFLECT_MUX].sRefPower = stMuxStatus.asPdPower[1];
    }

    return rc;
}
