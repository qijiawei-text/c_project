
/******************************************************************************
*
* Copyright(C), 2020, OPLINK Tech.Co.,Ltd
*
* Filename:      util_cali.h
* Description:   Optical cali
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                fandy      2020/12/02  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util_cmn.h"
#include "util_cali.h"
#include "util_log.h"


static ST_K_VAL m_stKVal;

static ST_WSS_CALI m_stWss1DropT0Cali;
static ST_WSS_CALI m_stWss1DropT30Cali;
static ST_WSS_CALI m_stWss1DropT65Cali;

static ST_WSS_CALI m_stWss2DropT0Cali;
static ST_WSS_CALI m_stWss2DropT30Cali;
static ST_WSS_CALI m_stWss2DropT65Cali;

static ST_WSS_CALI m_stWss1AddT0Cali;
static ST_WSS_CALI m_stWss1AddT30Cali;
static ST_WSS_CALI m_stWss1AddT65Cali;

static ST_WSS_CALI m_stWss2AddT0Cali;
static ST_WSS_CALI m_stWss2AddT30Cali;
static ST_WSS_CALI m_stWss2AddT65Cali;

/* add for DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration */
/* @ { */
static ST_OFS_FGA m_stOfsFgaVal = {0.0, 0.0};
/* @ } */

/* add for DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration */
/* @ { */
static ST_SFP_WSS_PANEL_CALI m_stSfpWssPanelCali = {0.0, 0.0, 0.0, 0.0};
/* @ } */

static int
__GetWssCali(const char *pcFileName, ST_WSS_CALI *pstWssCali)
{
    int rc = 0;
    int iRow = 0;

    FILE *fpFile = NULL;
    char acLine[256] = {0};

    if (NULL == pstWssCali || NULL == pcFileName) {
        return -1;
    }

    fpFile = fopen(pcFileName, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", pcFileName);
        return -3;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        /* skip comment line */
        if ('#' == acLine[0]) {
            continue;
        }

        /* skip first line */
        if ((NULL != strstr(acLine, "Channel")) ||
            (NULL != strstr(acLine, "P"))) {
            continue;
        }

        sscanf(acLine, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
               &pstWssCali->aadWssRawData[iRow][0],
               &pstWssCali->aadWssRawData[iRow][1],
               &pstWssCali->aadWssRawData[iRow][2],
               &pstWssCali->aadWssRawData[iRow][3],
               &pstWssCali->aadWssRawData[iRow][4],
               &pstWssCali->aadWssRawData[iRow][5],
               &pstWssCali->aadWssRawData[iRow][6],
               &pstWssCali->aadWssRawData[iRow][7],
               &pstWssCali->aadWssRawData[iRow][8],
               &pstWssCali->aadWssRawData[iRow][9],
               &pstWssCali->aadWssRawData[iRow][10],
               &pstWssCali->aadWssRawData[iRow][11],
               &pstWssCali->aadWssRawData[iRow][12],
               &pstWssCali->aadWssRawData[iRow][13],
               &pstWssCali->aadWssRawData[iRow][14],
               &pstWssCali->aadWssRawData[iRow][15],
               &pstWssCali->aadWssRawData[iRow][16],
               &pstWssCali->aadWssRawData[iRow][17],
               &pstWssCali->aadWssRawData[iRow][18],
               &pstWssCali->aadWssRawData[iRow][19],
               &pstWssCali->aadWssRawData[iRow][20],
               &pstWssCali->aadWssRawData[iRow][21],
               &pstWssCali->aadWssRawData[iRow][22],
               &pstWssCali->aadWssRawData[iRow][23],
               &pstWssCali->aadWssRawData[iRow][24],
               &pstWssCali->aadWssRawData[iRow][25],
               &pstWssCali->aadWssRawData[iRow][26],
               &pstWssCali->aadWssRawData[iRow][27],
               &pstWssCali->aadWssRawData[iRow][28],
               &pstWssCali->aadWssRawData[iRow][29],
               &pstWssCali->aadWssRawData[iRow][30],
               &pstWssCali->aadWssRawData[iRow][31],
               &pstWssCali->aadWssRawData[iRow][32]);


        iRow++;
        memset(acLine, 0, sizeof(acLine));
        if (iRow >= WSS_CALI_ROW_MAX_CNT + 1) {
            LOG_WRITE(EN_LOG_ERROR, "Row is overflow. Row = %d", iRow);
            break;
        }
    }

    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}


static int
__GetKValCali(ST_K_VAL *pstKVal)
{
    int rc = 0;
    int iRow = 0;

    FILE *fpFile = NULL;
    char acLine[256] = {0};
    char acBuf[256] = {0};

    if (NULL == pstKVal) {
        return -1;
    }

    fpFile = fopen(COM_K_VAL_DATA, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", COM_K_VAL_DATA);
        rc = -2;
        goto ERR_LABEL;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        /* skip comment line */
        if ('#' == acLine[0]) {
            continue;
        }

        /* skip first line */
        if ((NULL != strstr(acLine, "K0")) ||
            (NULL != strstr(acLine, "PortName"))) {
            continue;
        }

        if (NULL != strstr(acLine, "WssCali")) {
            sscanf(acLine, "%lf,%lf,%lf,%lf,%lf,%s",
                   &pstKVal->adWssLoss[iRow],
                   &pstKVal->aadKVal[iRow][0],
                   &pstKVal->aadKVal[iRow][1],
                   &pstKVal->aadKVal[iRow][2],
                   &pstKVal->aadKVal[iRow][3],
                   acBuf);
        } else {
            sscanf(acLine, "%lf,%lf,%lf,%lf,%s",
                   &pstKVal->aadKVal[iRow][0],
                   &pstKVal->aadKVal[iRow][1],
                   &pstKVal->aadKVal[iRow][2],
                   &pstKVal->aadKVal[iRow][3],
                   acBuf);
        }

#if 0
        LOG_WRITE(EN_LOG_NOTICE, "%s,%lf,%lf,%lf,%lf",
                  acBuf,
                  pstKVal->aadKVal[iRow][0],
                  pstKVal->aadKVal[iRow][1],
                  pstKVal->aadKVal[iRow][2],
                  pstKVal->aadKVal[iRow][3]);
#endif

        iRow++;
        memset(acLine, 0, sizeof(acLine));
        if (iRow >= EN_K_VAL_CNT) {
            LOG_WRITE(EN_LOG_DEBUG, "Row is overflow. Row = %d", iRow);
            break;
        }
    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}

/* add for DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration */
/* @ { */
static int
__GetOfsFgaValCali(ST_OFS_FGA *pstOfsVal)
{
    int rc = 0;

    FILE *fpFile = NULL;
    char acLine[DEF_BUF_SIZE_256] = {0};

    if (NULL == pstOfsVal) {
        return -1;
    }

    fpFile = fopen(DEF_OFS_FGA_VAL_DATA, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s, %s", DEF_OFS_FGA_VAL_DATA, strerror(errno));
        rc = -2;
        goto ERR_LABEL;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        /* skip comment line */
        if ('#' == acLine[0]) {
            continue;
        }

        if (NULL != strstr(acLine, DEF_OCM_DROP1COM1_TO_FGA1)) {
            pstOfsVal->dFga1 = atof(acLine + strlen(DEF_OCM_DROP1COM1_TO_FGA1 ","));
        } else if (NULL != strstr(acLine, DEF_OCM_DROP2COM1_TO_FGA2)) {
            pstOfsVal->dFga2 = atof(acLine + strlen(DEF_OCM_DROP1COM1_TO_FGA1 ","));
        }

        memset(acLine, 0, sizeof(acLine));
    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}
/* @ } */


/* iDirection: drop 0
               add 1
 */
static double
__GetWssCaliByPort(int iDirection, int iPort, double dStartFreq, double dEndFreq, double dTemperature, bool bIsSfp)
{
#if 0
    int i = 0;
    int iPortIdx = 0;
    int iChlIdx = 0;

    ST_WSS_CALI *pstT0Cali = NULL;
    ST_WSS_CALI *pstT30Cali = NULL;
    ST_WSS_CALI *pstT65Cali = NULL;


    double dWssIl = 0.0;
    double dTap = 0.0;
    double dCenterFreq = (dStartFreq + dEndFreq) / 2;
    double dDeltaCenterFreq = 0.0;
#endif
    double dCali = 0.0;
    /*   LBAND temporary notes   */
    /*   if 0 start endif end    */
#if 0
    EN_K_VAL_PORT enComPort = EN_K_VAL_DROP1;
#endif
    static bool m_bRead = false;

    if (!m_bRead) {
        /* drop */
        memset(&m_stWss1DropT0Cali, 0, sizeof(m_stWss1DropT0Cali));
        memset(&m_stWss1DropT30Cali, 0, sizeof(m_stWss1DropT30Cali));
        memset(&m_stWss1DropT65Cali, 0, sizeof(m_stWss1DropT65Cali));
        __GetWssCali(WSS1_DROP_T0_CALI_DATA, &m_stWss1DropT0Cali);
        __GetWssCali(WSS1_DROP_T30_CALI_DATA, &m_stWss1DropT30Cali);
        __GetWssCali(WSS1_DROP_T65_CALI_DATA, &m_stWss1DropT65Cali);

        memset(&m_stWss2DropT0Cali, 0, sizeof(m_stWss2DropT0Cali));
        memset(&m_stWss2DropT30Cali, 0, sizeof(m_stWss2DropT30Cali));
        memset(&m_stWss2DropT65Cali, 0, sizeof(m_stWss2DropT65Cali));
        __GetWssCali(WSS2_DROP_T0_CALI_DATA, &m_stWss2DropT0Cali);
        __GetWssCali(WSS2_DROP_T30_CALI_DATA, &m_stWss2DropT30Cali);
        __GetWssCali(WSS2_DROP_T65_CALI_DATA, &m_stWss2DropT65Cali);

        /* add */
        memset(&m_stWss1AddT0Cali, 0, sizeof(m_stWss1AddT0Cali));
        memset(&m_stWss1AddT30Cali, 0, sizeof(m_stWss1AddT30Cali));
        memset(&m_stWss1AddT65Cali, 0, sizeof(m_stWss1AddT65Cali));
        __GetWssCali(WSS1_ADD_T0_CALI_DATA, &m_stWss1AddT0Cali);
        __GetWssCali(WSS1_ADD_T30_CALI_DATA, &m_stWss1AddT30Cali);
        __GetWssCali(WSS1_ADD_T65_CALI_DATA, &m_stWss1AddT65Cali);

        memset(&m_stWss2AddT0Cali, 0, sizeof(m_stWss2AddT0Cali));
        memset(&m_stWss2AddT30Cali, 0, sizeof(m_stWss2AddT30Cali));
        memset(&m_stWss2AddT65Cali, 0, sizeof(m_stWss2AddT65Cali));
        __GetWssCali(WSS2_ADD_T0_CALI_DATA, &m_stWss2AddT0Cali);
        __GetWssCali(WSS2_ADD_T30_CALI_DATA, &m_stWss2AddT30Cali);
        __GetWssCali(WSS2_ADD_T65_CALI_DATA, &m_stWss2AddT65Cali);

        m_bRead = true;
    }

    /*   LBAND temporary notes   */
    /*   if 0 start endif end    */
 #if 0
    if (0 == iDirection) {     /* drop */
        if (iPort > 0 && iPort <= 32) {
            enComPort = EN_K_VAL_DROP1;
            iPortIdx = iPort;
            pstT0Cali = &m_stWss1DropT0Cali;
            pstT30Cali = &m_stWss1DropT30Cali;
            pstT65Cali = &m_stWss1DropT65Cali;
        } else if (iPort > 32 && iPort <= 64) {
            enComPort = EN_K_VAL_DROP2;
            iPortIdx = iPort - 32;
            pstT0Cali = &m_stWss2DropT0Cali;
            pstT30Cali = &m_stWss2DropT30Cali;
            pstT65Cali = &m_stWss2DropT65Cali;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Input port error = %d", iPort);
            return 0;
        }
    } else {     /* Add */
        if (iPort > 0 && iPort <= 32) {
            enComPort = EN_K_VAL_ADD1;
            iPortIdx = iPort;
            pstT0Cali = &m_stWss1AddT0Cali;
            pstT30Cali = &m_stWss1AddT30Cali;
            pstT65Cali = &m_stWss1AddT65Cali;
        } else if (iPort > 32 && iPort <= 64) {
            enComPort = EN_K_VAL_ADD2;
            iPortIdx = iPort - 32;
            pstT0Cali = &m_stWss2AddT0Cali;
            pstT30Cali = &m_stWss2AddT30Cali;
            pstT65Cali = &m_stWss2AddT65Cali;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Input port error = %d", iPort);
            return 0;
        }
    }

    /* find channel index, THz * 1000 = GHz */
    dDeltaCenterFreq = abs(dCenterFreq - (pstT0Cali->aadWssRawData[0][0] * 1000));
    for (i = 0; i <= WSS_CALI_ROW_MAX_CNT; i++) {
        if (dDeltaCenterFreq > abs(dCenterFreq - (pstT0Cali->aadWssRawData[i][0]) * 1000)) {
            dDeltaCenterFreq = abs(dCenterFreq - (pstT0Cali->aadWssRawData[i][0] * 1000));
            iChlIdx = i;
        }
    }
#endif

#if 0
    LOG_WRITE(EN_LOG_NOTICE, "Channel %d Port %d, dCenterFreq %lf, dDeltaCenterFreq %lf",
              iChlIdx, iPortIdx, dCenterFreq, dDeltaCenterFreq);
    LOG_WRITE(EN_LOG_NOTICE, "aadWssRawData T65 = %lf", pstT65Cali->aadWssRawData[iChlIdx][iPortIdx]);
    LOG_WRITE(EN_LOG_NOTICE, "aadWssRawData T30 = %lf", pstT30Cali->aadWssRawData[iChlIdx][iPortIdx]);
    LOG_WRITE(EN_LOG_NOTICE, "aadWssRawData T0  = %lf", pstT0Cali->aadWssRawData[iChlIdx][iPortIdx]);
#endif


    /*   LBAND temporary notes   */
    /*   if 0 start endif end    */
#if 0
    if (dTemperature > 30) {     /* dTemperature > 30 */
        /* (65- t)/35* T(65)(port)(CF) + (t-30)/35*T(30)(port)(CF)=IL(WSS_IL)(port)(CF) */
        dWssIl = (65 - dTemperature) / 35 * pstT30Cali->aadWssRawData[iChlIdx][iPortIdx] +
                 (dTemperature - 30) / 35 * pstT65Cali->aadWssRawData[iChlIdx][iPortIdx];
    } else {     /* dTemperature <= 30 */
        /* (30- t)/30* T(30)(port)(CF) + (t-0)/30*T(30)(port)(CF)=IL(WSS_IL)(port)(CF) */
        dWssIl = (30 - dTemperature) / 30 * pstT0Cali->aadWssRawData[iChlIdx][iPortIdx] +
                 (dTemperature - 0) / 30 * pstT30Cali->aadWssRawData[iChlIdx][iPortIdx];
    }

    if (false == bIsSfp ) {

        if (1 == iDirection) {     /* Add */
            dCali = dWssIl;
            return dCali;
        }

        /* Drop */



        /* IL_TAP(CF)=CF^3*K3+CF^2*k2+CF*k1+k0 */
        dTap = UTIL_GetComPortCali(enComPort, dStartFreq, dEndFreq);


        dCali = dTap + dWssIl + m_stKVal.adWssLoss[enComPort];

        LOG_WRITE(EN_LOG_DEBUG, "enComPort %d, dTap %lf, dWssIl %lf, adWssLoss %lf, dCali %lf",
                  enComPort, dTap, dWssIl, m_stKVal.adWssLoss[enComPort], dCali);
    } else {
        dCali = dWssIl;
        return dCali;
    }
#endif
    return dCali;
}


double
UTIL_GetComPortCali(EN_K_VAL_PORT enPort, double dStartFreq, double dEndFreq)
{
    double dCali = 0.0;

    /* THz */
    double dCenterFreq = ((dStartFreq + dEndFreq) / 2) / 1000;

    static bool m_bRead = false;

    if (!m_bRead) {
        memset(&m_stKVal, 0, sizeof(m_stKVal));
        __GetKValCali(&m_stKVal);

        /* add for DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration */
        /* @ { */
        memset(&m_stOfsFgaVal, 0x00, sizeof(m_stOfsFgaVal));
        __GetOfsFgaValCali(&m_stOfsFgaVal);
        m_bRead = true;
        /* @ } */
    }

    dCali = pow(dCenterFreq, 3) * m_stKVal.aadKVal[enPort][3] +
            pow(dCenterFreq, 2) * m_stKVal.aadKVal[enPort][2] +
            dCenterFreq * m_stKVal.aadKVal[enPort][1] +
            m_stKVal.aadKVal[enPort][0];


    /*   LBAND temporary notes   */
    /*   if 0 start endif end    */
    /* add for DROP1_COM1 to FGA1 calibration, DROP2_COM1 to FGA2 calibration */
    /* @ { */

#if 0
    if (EN_K_VAL_DROP1 == enPort) {
        dCali = m_stOfsFgaVal.dFga1 + dCali;
    } else if (EN_K_VAL_DROP2 == enPort) {
        dCali = m_stOfsFgaVal.dFga2 + dCali;
    }
#endif

    /* @ } */

#if 0
    LOG_WRITE(EN_LOG_NOTICE, "Port %d, K3 %lf, K2 %lf, K1 %lf, K0 %lf, dCali %lf",
              enPort,
              m_stKVal.aadKVal[enPort][3],
              m_stKVal.aadKVal[enPort][2],
              m_stKVal.aadKVal[enPort][1],
              m_stKVal.aadKVal[enPort][0],
              dCali);
#endif

    return dCali;
}



double
UTIL_GetDropPortCali(int iPort, double dStartFreq, double dEndFreq, double dTemperature)
{
    return __GetWssCaliByPort(0, iPort, dStartFreq, dEndFreq, dTemperature, false);
}

double
UTIL_GetDropPortCaliNoTab(int iPort, double dStartFreq, double dEndFreq, double dTemperature)
{
    return __GetWssCaliByPort(0, iPort, dStartFreq, dEndFreq, dTemperature, true);
}

double
UTIL_GetAddPortCali(int iPort, double dStartFreq, double dEndFreq, double dTemperature)
{
    return __GetWssCaliByPort(1, iPort, dStartFreq, dEndFreq, dTemperature, false);
}


double
UTIL_GetTotalPower(double *pdDbm, int iLen)
{
    int i = 0;
    double dTotalDbm = 0.0;
    double dTotalMw = 0.0;

    double adMw[512];

    if (NULL == pdDbm) {
        return 0;
    }

    for (i = 0; i < iLen; i++) {
        adMw[i] = pow(10, pdDbm[i] / 10);
        dTotalMw += adMw[i];
    }

    dTotalDbm = 10 * log10(dTotalMw);

    if (dTotalDbm < -45.0) {
        dTotalDbm = -60.0;
    }

    return dTotalDbm;
}

double
UTIL_GetChanPowerByRawdata(int16_t *psSliceRawData, uint32_t uiSliceCnt, double dLosThr)
{
    uint32_t i = 0;
    double dTotalDbm = 0.0;
    double dTotalMw = 0.0;
    double dRawPwr = 0;
    double dMw = 0;

    if (NULL == psSliceRawData) {
        return -60.0;
    }

    for (i = 0; i < uiSliceCnt; i++) {
        dRawPwr = (double)(*(psSliceRawData + i)) / 100;
        dMw = pow(10, dRawPwr / 10);
        dTotalMw += dMw;
    }

    dTotalDbm = 10 * log10(dTotalMw);

    /* meiwu req report actual power */
    if (dTotalDbm < dLosThr) {
        dTotalDbm = -60.0;
    }

    return dTotalDbm;
}

/**************************************************************************************************/
/* definition for SFP port tx/rx calibration                                                      */
/**************************************************************************************************/
/* @ { */
/***FUNC+******************************************************************************************/
/* Name   : __GetWssPanelValCali                                                                  */
/* Descrp : Get WSS panel val calibration data from  file                                         */
/* Input  : NONE                                                                                  */
/* Output : pstSfpWssPanelVal -- A pointer of wss panel calibration                               */
/* Return : 0 -- OK, -1 -- failed                                                                 */
/***FUNC-******************************************************************************************/
static int
__GetWssPanelValCali(ST_SFP_WSS_PANEL_CALI *pstSfpWssPanelVal)
{
    int rc = 0;

    FILE *fpFile = NULL;
    char acLine[DEF_BUF_SIZE_256] = {0};

    if (NULL == pstSfpWssPanelVal) {
        return -1;
    }

    fpFile = fopen(DEF_SFP_WSS_PANEL_VAL_DATA, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s, %s", DEF_SFP_WSS_PANEL_VAL_DATA, strerror(errno));
        rc = -2;
        goto ERR_LABEL;
    }

    fseek(fpFile, 0, SEEK_SET);
    memset(acLine, 0, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        /* skip comment line */
        if ('#' == acLine[0]) {
            continue;
        }

        if (NULL != strstr(acLine, DEF_SFP_WSS1_ADD_PANEL)) {
            pstSfpWssPanelVal->dWss1Add = atof(acLine + strlen(DEF_SFP_WSS1_ADD_PANEL ","));
        } else if (NULL != strstr(acLine, DEF_SFP_WSS1_DROP_PANEL)) {
            pstSfpWssPanelVal->dWss1Drop = atof(acLine + strlen(DEF_SFP_WSS1_DROP_PANEL ","));
        } else if (NULL != strstr(acLine, DEF_SFP_WSS2_ADD_PANEL)) {
            pstSfpWssPanelVal->dWss2Add = atof(acLine + strlen(DEF_SFP_WSS2_ADD_PANEL ","));
        } else if (NULL != strstr(acLine, DEF_SFP_WSS2_DROP_PANEL)) {
            pstSfpWssPanelVal->dWss2Drop = atof(acLine + strlen(DEF_SFP_WSS2_DROP_PANEL ","));
        }

        memset(acLine, 0, sizeof(acLine));
    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return rc;
}
/* @ } */

/***FUNC+******************************************************************************************/
/* Name   : Util_CaliCalcSfpPower                                                                 */
/* Descrp : Cali the sfp panel power by calibration                                               */
/* Input  : dPower -- the sfp module tx/rx power                                                  */
/* Input  : ucDir  -- the sfp power is TX or RX(refer to EN_CALI_SFP_DIR)                         */
/* Input  : iWssPortIdx -- the wss port idx (from 1 to 64)                                        */
/* Input  : dAttn -- the WSS attn                                                                 */
/* Input  : dTemperature -- system temperature                                                    */
/* Output : NONE                                                                                  */
/* Return : the panel sfp power. If the calibrate panel sfp power <= -60dBm, just return -60dBm   */
/***FUNC-******************************************************************************************/
double
Util_CaliCalcSfpPower(double dPower, uint8_t ucDir, int32_t iWssPortIdx, double dAttn, double dTemperature)
{
    double dSfpCaliPower = 0.0;
    uint8_t ucWssDir = 0;
    double dWssCali = 0.0;
    double dWssPanelOffset = 0.0;
    static bool m_bRead = false;

    /* check input parameter */
    if ((ucDir >= EN_CALI_SFP_DIR_MAX) ||
        (dAttn > 25.5)) {
        LOG_WRITE(EN_LOG_ERROR, "Input error, dir: %hhu, attn: %lf", ucDir, dAttn);
        return dPower;
    }

    /* First time read calibration data from file.  */
    if (false == m_bRead) {
        memset(&m_stSfpWssPanelCali, 0x00, sizeof(m_stSfpWssPanelCali));
        __GetWssPanelValCali(&m_stSfpWssPanelCali);
        m_bRead = true;
    }

    /* get the WSS panel calibration data.  */
    if (EN_CALI_SFP_DIR_TX == ucDir) {
        if ((iWssPortIdx >= DEF_CALI_WSS1_PORT_MIN) && (iWssPortIdx <= DEF_CALI_WSS1_PORT_MAX)) {
            dWssPanelOffset = m_stSfpWssPanelCali.dWss1Add;
        } else if ((iWssPortIdx >= DEF_CALI_WSS2_PORT_MIN) && (iWssPortIdx <= DEF_CALI_WSS2_PORT_MAX)) {
            dWssPanelOffset = m_stSfpWssPanelCali.dWss2Add;
        } else {
            dSfpCaliPower = DEF_SFP_NO_POWER;
            goto cleanup;
        }
    } else {
        if ((iWssPortIdx >= DEF_CALI_WSS1_PORT_MIN) && (iWssPortIdx <= DEF_CALI_WSS1_PORT_MAX)) {
            dWssPanelOffset = m_stSfpWssPanelCali.dWss1Drop;
        } else if ((iWssPortIdx >= DEF_CALI_WSS2_PORT_MIN) && (iWssPortIdx <= DEF_CALI_WSS2_PORT_MAX)) {
            dWssPanelOffset = m_stSfpWssPanelCali.dWss2Drop;
        } else {
            dSfpCaliPower = DEF_SFP_NO_POWER;
            goto cleanup;
        }
    }

    /* SFP tx means WSS add, wss add is 1*/
    ucWssDir = ((EN_CALI_SFP_DIR_TX == ucDir) ? 1 : 0);
    dWssCali = __GetWssCaliByPort(ucWssDir, (int)iWssPortIdx, DEF_CALI_SFP_FREQ, DEF_CALI_SFP_FREQ, dTemperature, true);

    /* calc the sfp panel power. */
    if (EN_CALI_SFP_DIR_TX == ucDir) {
        dSfpCaliPower = dPower + dWssCali + dWssPanelOffset - dAttn;
        LOG_WRITE(EN_LOG_NOTICE, "sfptx(%.2lf) + wsscali(%.2lf) + paneloffset(%.2lf) - attn(%.2lf) = %.2lf, wssport: %d",
                  dPower, dWssCali, dWssPanelOffset, dAttn, dSfpCaliPower, iWssPortIdx);
    } else {
        dSfpCaliPower = dPower - dWssCali - dWssPanelOffset + dAttn;
        LOG_WRITE(EN_LOG_NOTICE, "sfprx(%.2lf) - wsscali(%.2lf) - paneloffset(%.2lf) + attn(%.2lf) = %.2lf, wssport: %d",
                  dPower, dWssCali, dWssPanelOffset, dAttn, dSfpCaliPower, iWssPortIdx);
    }

cleanup:
    return dSfpCaliPower < DEF_SFP_NO_POWER ? DEF_SFP_NO_POWER : dSfpCaliPower;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_GetSfpWssPanelCali                                                               */
/* Descrp : get the sfp panel wss calibration data                                                */
/* Input  : iIndex -- wss index                                                                   */
/* Output : NONE                                                                                  */
/* Return : the panel sfp calibrate data                                                          */
/***FUNC-******************************************************************************************/
double
UTIL_GetSfpWssPanelCali(int iIndex)
{
    static bool m_bRead = false;

    /* First time read calibration data from file.  */
    if (false == m_bRead) {
        memset(&m_stSfpWssPanelCali, 0x00, sizeof(m_stSfpWssPanelCali));
        __GetWssPanelValCali(&m_stSfpWssPanelCali);
        m_bRead = true;
    }

    switch (iIndex) {
    case 0: return m_stSfpWssPanelCali.dWss1Add;
    case 1: return m_stSfpWssPanelCali.dWss1Drop;
    case 2: return m_stSfpWssPanelCali.dWss2Add;
    case 3: return m_stSfpWssPanelCali.dWss2Drop;
    }

    return 0.0;
}
