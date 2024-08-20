/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      WssDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "WssDriver.h"
#include "File.h"
#include "RspHandler.h"
#include <bits/stdc++.h>

/*==============================function=========================*/

#define WSS_ATTN_MAX_VAL               (255)
#define WSS_CMD_WR_RETRY_TMS           (3)
#define WSS_CMD_RD_RETRY_TMS           (3)                                /* Unit: ms  */



#define DEF_WSS_NIS_FRM_START           (0xDD01)
#define DEF_WSS_NIS_FRM_STOP            (0xDD02)

#define DEF_WSS_NIS_FRM_LEN_START       (2)
#define DEF_WSS_NIS_FRM_LEN_STOP        (2)
#define DEF_WSS_NIS_FRM_LEN_START_STOP  (DEF_WSS_NIS_FRM_LEN_START + DEF_WSS_NIS_FRM_LEN_STOP)

#define DEF_WSS_NIS_FRM_L_TAG           (0x00FF)

#define DEF_WSS_NIS_FRM_S_LEN_MAX_W     (244u)                              /* 256  - 12                                               */
#define DEF_WSS_NIS_FRM_L_LEN_MAX_W     (2000u)                             /* W: 1017 object max, R: 1021 object max                  */
#define DEF_WSS_NIS_FRM_L_LEN_MAX_R     (2042u)                             /* 1021 object max                                         */

#define DEF_WSS_NIS_CMD_S_LEN_MID2LEN   (2)                                 /* MID(1B) + LEN(1B)                                       */
#define DEF_WSS_NIS_CMD_L_LEN_MID2LEN_W (5)                                 /* MID(1B) + TAG(2B) + LEN(2B)                             */
#define DEF_WSS_NIS_CMD_L_LEN_MID2LEN_R (3)                                 /* MID(1B) + LEN(2B)                                       */

#define DEF_WSS_NIS_CMD_S_LEN_CMD2PARA  (4)                                 /* CMD(1B) + OID(1B) + INST(1B) + PARA(1B)                 */
#define DEF_WSS_NIS_CMD_L_LEN_CMD2PARA  (5)                                 /* CMD(2B) + OID(1B) + INST(1B) + PARA(1B)                 */
#define DEF_WSS_NIS_CMD_L_LEN_CMD       (2)                                 /* CMD(2B)                                                 */

#define DEF_WSS_NIS_CMD_S_LEN_CMD2OID   (2)                                 /* CMD(1B) + OID(1B)                                       */
#define DEF_WSS_NIS_CMD_L_LEN_CMD2OID   (3)                                 /* CMD(2B) + OID(1B)                                       */

#define DEF_WSS_NIS_CMD_S_LEN_TAIL      (1)                                 /* CS(1B)                                                  */
#define DEF_WSS_NIS_CMD_L_LEN_TAIL      (2)                                 /* CRC(2B)                                                 */

#define DEF_WSS_NIS_RES_S_LEN_CRC_ERR   (4)                                 /* MID(1B) + LEN(1B) + RSLT(1B) + CS(1B)                   */
#define DEF_WSS_NIS_RES_L_LEN_MID2CRC   (7)                                 /* MID(1B) + LEN(2B) + RSLT(1B) + CNT(1B) + CRC(2B)        */


#define DEF_WSS_NIS_CHAR_ESCAPE         (0xdd)
#define DEF_WSS_NIS_CHAR_START          (0x01)
#define DEF_WSS_NIS_CHAR_STOP           (0x02)
#define DEF_WSS_NIS_CHAR_PAD            (0x04)


#define DEF_WSS_NIS_INST_MAX            (1023u)                             /* Maximum Instance                                        */
#define FLEXGRID_WSS_RATIO              (6.25 / 3.125)


#define __DBG_WSS_PrintSend(pucBuf, ulLen)      \
    __DBG_WSS_Print("Send", pucBuf, ulLen)

#define __DBG_WSS_PrintRecv(pucBuf, ulLen)      \
    __DBG_WSS_Print("Recv", pucBuf, ulLen)

/* #define DEF_NIS_DBG_OPEN */

#if 0
static int
__Disp_Hex(uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    char acBuf[128] = {0};
    char acBufC[8] = {0};
    uint32_t ulRow = 0;
    uint32_t ulCol = 0;
    uint32_t ulIdx = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    uint8_t ucLen = 0;
    uint8_t ucPad = 0;
    const char *pacDisp[] = {
        "			 0	1  2  3  4	5  6  7    8  9  A	B  C  D  E	F		0		8\r\n",
        "			 |	|  |  |  |	|  |  |    |  |  |	|  |  |  |	|		|		|\r\n\r\n",
        "		 :																			\r\n",
    };

#define DEF_CLI_DISP_ADDR_START         (12u)
#define DEF_CLI_DISP_ADDR_LEN           (8u)
#define DEF_CLI_DISP_COMM_START         (DEF_CLI_DISP_ADDR_START + 55u)
#define DEF_CLI_DISP_COL_MAX            (16u)
#define DEF_CLI_DISP_UCHAR_LEN          (3u)


    if (NULL == pucBuf) {
        goto ERR_LABEL;
    }

    if (ulLen > DEF_1KB * 3) {
        ulLen = DEF_1KB * 3;
    }

    printf("\r\n");
    printf("%s", pacDisp[0]);
    printf("%s", pacDisp[1]);

    ucPad = ulAddr % DEF_CLI_DISP_COL_MAX;
    ulAddr -= ucPad;
    ulLen += ucPad;
    ulRow = (ulLen + DEF_CLI_DISP_COL_MAX - 1) / DEF_CLI_DISP_COL_MAX;
    for (i = 0; i < ulRow; i++) {
        sprintf(acBuf, "%08X", ulAddr + i * DEF_CLI_DISP_COL_MAX);
        strcpy(acBuf + DEF_CLI_DISP_ADDR_LEN, pacDisp[2] + DEF_CLI_DISP_ADDR_LEN);

        ulCol = (ulLen - i * DEF_CLI_DISP_COL_MAX) >= DEF_CLI_DISP_COL_MAX
                ? DEF_CLI_DISP_COL_MAX
                : (ulLen - i * DEF_CLI_DISP_COL_MAX);

        j = i == 0 ? ucPad : 0;
        for (; j < ulCol; j++) {
            ulIdx = i * DEF_CLI_DISP_COL_MAX + j - ucPad;

            if (pucBuf[ulIdx] >= 0x20 && pucBuf[ulIdx] <= 0x7E) {
                acBuf[DEF_CLI_DISP_COMM_START + j] = pucBuf[ulIdx];
            } else {
                acBuf[DEF_CLI_DISP_COMM_START + j] = '.';
            }

            sprintf(acBufC, "%02X", pucBuf[ulIdx]);

            ucLen = j >= DEF_CLI_DISP_COL_MAX / 2
                    ? 2
                    : 0;
            acBuf[DEF_CLI_DISP_ADDR_START + j * DEF_CLI_DISP_UCHAR_LEN + ucLen + 0] = acBufC[0];
            acBuf[DEF_CLI_DISP_ADDR_START + j * DEF_CLI_DISP_UCHAR_LEN + ucLen + 1] = acBufC[1];
        }

        printf("%s", acBuf);
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}
#endif

static int
__DBG_WSS_Print(const char *pcStr, uint8_t *pucBuf, uint32_t ulLen)
{
#ifdef DEF_NIS_DBG_OPEN
    if (0 == strcmp(pcStr, "Send")) {
        printf("\n-----------------------------------------------------------------------------------");
    }
    printf("\n%s: %d", pcStr, ulLen - 4);

    if (ulLen >= 4) {
        __Disp_Hex(0, pucBuf + 2, ulLen - 4);
    }

    printf("\n");
#endif
    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __WSS_NIS_MID                                                                         */
/* Descrp : Get the MID value                                                                     */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The MID value range from 1 to 255                                                     */
/***FUNC-******************************************************************************************/
static uint8_t
__WSS_NIS_MID(void)
{
    static uint8_t m_ucMid = 0;


    ++m_ucMid;
    if (0 == m_ucMid) {
        m_ucMid = 1;
    }

    return m_ucMid;
}

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_Crc16                                                                         */
/* Descrp : Calculates CRC16 value for a array                                                    */
/*            Note: The polynomial is 0x8005                                                      */
/* Input  : pucBuf-- pointer to the data buffer to calculate                                      */
/*        : ulLen -- the length of data to calculate, unit: byte                                  */
/* Output : None                                                                                  */
/* Return : The calculated CRC16 value or 0 if error                                              */
/***FUNC-******************************************************************************************/
uint16_t
_WssNis_Crc16(uint8_t *pucBuf, uint32_t ulLen)
{
    /* CRC-16, polynomial = 0x8005                                              */
    const uint16_t ausCrc16Tbl[256] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0,
        0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41,
        0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0,
        0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
        0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341,
        0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1,
        0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
        0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940,
        0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1,
        0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0,
        0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
        0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0,
        0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0,
        0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
        0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0,
        0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0,
        0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40,
        0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
        0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0,
        0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    uint16_t usCrc16 = 0;


    /* Check parameters                                                         */
    if ((NULL == pucBuf) ||
        (0 == ulLen)) {
        goto ERR_LABEL;
    }

    while (ulLen--) {
        usCrc16 = ausCrc16Tbl[(uint8_t)(usCrc16) ^ (*pucBuf++)] ^ (usCrc16 >> 8);
    }

    return usCrc16;

ERR_LABEL:
    return 0;
}

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_ChkSum                                                                        */
/* Descrp : Calculate the check sum                                                               */
/* Input  : pucBuf-- pointer to the data buffer to calculate                                      */
/*        : ulLen -- the specified data length to calculate, unit: byte                           */
/* Output : None                                                                                  */
/* Return : The calculated check sum                                                              */
/***FUNC-******************************************************************************************/
static uint8_t
_WssNis_ChkSum(uint8_t *pucBuf, uint32_t ulLen)
{
    uint8_t ucData = 0;
    uint32_t i = 0;


    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    for (i = 0; i < ulLen; i++) {
        ucData = ucData ^ pucBuf[i];
    }

    return ucData;

ERR_LABEL:
    return 0;
}


#if 0
/***FUNC+******************************************************************************************/
/* Name   : _WssNis_FetchLastFrm                                                                  */
/* Descrp : Fetch the last frame                                                                  */
/* Input  : pucBuf-- pointer to the data buffer to fetch                                          */
/*        : pusLen-- pointer to the data length to fetch, unit: byte                              */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERR:                                                                  */
/***FUNC-******************************************************************************************/
static int
_WssNis_FetchLastFrm(uint8_t *pucBuf, uint16_t *pusLen)
{
    uint8_t *pucFrm = NULL;
    uint16_t usLenFrm = 0;
    uint16_t usLenSkip = 0;
    bool bHasStart = false;
    bool bHasStop = false;
    int i = 0;


    if ((NULL == pucBuf) ||
        (NULL == pusLen)) {
        goto ERR_LABEL;
    }

    pucFrm = pucBuf;
    usLenFrm = *pusLen;

    /* Find the last frame start 'dd 01'                                        */
    for (i = 0; i < usLenFrm; i++) {
        if (i + 1 < usLenFrm) {
            if ((DEF_WSS_NIS_CHAR_ESCAPE == pucFrm[i + 0]) &&
                (DEF_WSS_NIS_CHAR_START == pucFrm[i + 1])) {
                bHasStart = true;
                usLenSkip = i;
            }
        }
    }

    /* No frame start 'dd 01'                                                   */
    if (false == bHasStart) {
        goto ERR_LABEL;
    }

    /* Skip the data before frame start 'dd 01'                                 */
    pucFrm += usLenSkip;
    usLenFrm -= usLenSkip;

    /* Find the first frame stop 'dd 02'                                        */
    for (i = DEF_WSS_NIS_FRM_LEN_START; i < usLenFrm; i++) {
        if (i + 1 < usLenFrm) {
            if ((DEF_WSS_NIS_CHAR_ESCAPE == pucFrm[i + 0]) &&
                (DEF_WSS_NIS_CHAR_STOP == pucFrm[i + 1])) {
                bHasStop = true;
                break;
            }
        }
    }

    /* No frame stop 'dd 02'                                                    */
    if (false == bHasStop) {
        goto ERR_LABEL;
    }

    usLenFrm = i + DEF_WSS_NIS_FRM_LEN_STOP;
    memcpy(pucBuf, pucBuf + usLenSkip, usLenFrm);
    *pusLen = usLenFrm;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_IsInProc                                                                      */
/* Descrp : Check command response code                                                           */
/* Input  : pucBuf   -- pointer to the response buffer to check                                   */
/*        : usLen    -- the specified buffer length to check, unit: byte                          */
/*        : ucFrmType-- the spcified frame type to check                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_WSS_NIS_FRM_TYPE_S:                                                       */
/*              @arg EN_WSS_NIS_FRM_TYPE_L:                                                       */
/* Output : None                                                                                  */
/* Return : The response code is OK or not                                                        */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/***FUNC-******************************************************************************************/
static bool
_WssNis_IsInProc(uint8_t *pucBuf, uint16_t usLen, uint8_t ucFrmType)
{
    ST_WSS_NIS_RESP_S_HEAD_CMN *pstRespS = NULL;
    ST_WSS_NIS_RESP_L_HEAD_CMN *pstRespL = NULL;
    uint8_t ucRslt = 0;


    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    pstRespS = (ST_WSS_NIS_RESP_S_HEAD_CMN *)pucBuf;
    pstRespL = (ST_WSS_NIS_RESP_L_HEAD_CMN *)pucBuf;

#if 0
    /* Check the MID                                                            */
    if (0 == pstRespS->ucMid) {
        LOG_WRITE(EN_LOG_ERROR, "MID error: %d", pstRespS->ucMid);
        goto ERR_LABEL;
    }
#endif


    if ((EN_WSS_NIS_FRM_TYPE_S == ucFrmType) ||
        (DEF_WSS_NIS_RES_S_LEN_CRC_ERR == usLen)) {
        ucRslt = pstRespS->ucRslt;
        /* LOG_WRITE(EN_LOG_NOTICE, "S: %d", ucRslt); */
    } else {
        ucRslt = pstRespL->ucRslt;
        /* LOG_WRITE(EN_LOG_NOTICE, "L: %d", ucRslt); */
    }


    if (EN_WSS_NIS_ERR_CMD_INPROGRESS != ucRslt) {
        LOG_WRITE(EN_LOG_DEBUG, "ucRslt: %d", ucRslt);
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;
}

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_IsCmdOK                                                                       */
/* Descrp : Check the response frame is OK or not                                                 */
/* Input  : pucBuf   -- pointer to the data buffer to check                                       */
/*        : usLen    -- the spcified data length to check, unit: byte                             */
/*        : ucFrmType-- the spcified frame type to check                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_WSS_NIS_FRM_TYPE_S:                                                       */
/*              @arg EN_WSS_NIS_FRM_TYPE_L:                                                       */
/* Output : None                                                                                  */
/* Return : The response frame is OK or not                                                       */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/***FUNC-******************************************************************************************/
static int
_WssNis_IsCmdOK(uint8_t *pucBuf, uint16_t usLen, uint8_t ucFrmType)
{
    ST_WSS_NIS_RESP_S_HEAD_CMN *pstRespS = NULL;
    ST_WSS_NIS_RESP_L_HEAD_CMN *pstRespL = NULL;
    uint16_t usCrc16 = 0;
    uint16_t usLenT = 0;
    uint8_t ucRslt = 0;
    char const *pacRslt[] = {
        "0 : No Error",
        "1 : Invalid Command",
        "2 : Value out of Range",
        "3 : Command Failed",
        "4 : Checksum Failed",
        "5 : Not Allowed",
        "6 : Command in Progress",
        "7 : End of Data",
        "8 : ",
        "9 : ",
        "10: Command Too Long",
        ">10"
    };


    if (NULL == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    if (0 == usLen) {
        LOG_WRITE(EN_LOG_ERROR, "No frame");
        goto ERR_LABEL;
    }

    pstRespS = (ST_WSS_NIS_RESP_S_HEAD_CMN *)pucBuf;
    pstRespL = (ST_WSS_NIS_RESP_L_HEAD_CMN *)pucBuf;


#if 1
    if ((EN_WSS_NIS_FRM_TYPE_S == ucFrmType) ||
        (DEF_WSS_NIS_RES_S_LEN_CRC_ERR == usLen)) {
        ucRslt = pstRespS->ucRslt;
        usLenT = pstRespS->ucLen + DEF_WSS_NIS_CMD_S_LEN_MID2LEN;
    } else {
        ucRslt = pstRespL->ucRslt;
        usLenT = ntohs(pstRespL->usLen) + DEF_WSS_NIS_CMD_L_LEN_MID2LEN_R;
    }
#endif


#if 0
    /* Check the MID                                                            */
    if (0 == pstRespS->ucMid) {
        LOG_WRITE(EN_LOG_ERROR, "MID error: %d", pstRespS->ucMid);
        goto ERR_LABEL;
    }
#endif


    /* Check the result code                                                    */
    if (EN_WSS_NIS_ERR_NONE != ucRslt) {
        ucRslt = ucRslt > 11 ? 11 : ucRslt;
        LOG_WRITE(EN_LOG_ERROR, "Response result error [%s]", pacRslt[ucRslt]);
        printf("Response result error [%s]\n", pacRslt[ucRslt]);
        goto ERR_LABEL;
    }



    /* Check the frame length                                                   */
    if (usLenT != usLen) {
        LOG_WRITE(EN_LOG_ERROR, "Response frame length error: expect one is %d, real one is %d",
                  usLenT,
                  usLen);
        __DBG_WSS_PrintRecv(pucBuf, usLen);
        goto ERR_LABEL;
    }

    if (EN_WSS_NIS_FRM_TYPE_S == ucFrmType) {
        /* Check the checksum                                                   */
        if (pucBuf[usLen - 1] != _WssNis_ChkSum(pucBuf, usLen - 1)) {
            LOG_WRITE(EN_LOG_ERROR, "Check sum error");
            goto ERR_LABEL;
        }
    } else {
        /* Check the CRC                                                        */
        usCrc16 = pucBuf[usLen - 2];
        usCrc16 <<= 8;
        usCrc16 |= pucBuf[usLen - 1];
        if (usCrc16 != _WssNis_Crc16(pucBuf, usLen - 2)) {
            LOG_WRITE(EN_LOG_ERROR, "Crc16 error");
            goto ERR_LABEL;
        }
    }

    /* printf("OK!\n"); */

    return true;

ERR_LABEL:
    return false;
}

#endif


/***FUNC+******************************************************************************************/
/* Name   : _WssNis_PackFrm                                                                       */
/* Descrp : Pack the send frame, add frame header DD01 and frame tailer DD02, then replace "DD"   */
/*          with "DD 04"                                                                          */
/* Input  : pucBuf-- pointer to the command buffer to pack                                        */
/*        : pusLen-- pointer to the packed frame length, unit: byte                               */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERR:                                                                  */
/***FUNC-******************************************************************************************/
static int
_WssNis_PackFrm(uint8_t *pucBuf, uint16_t *pusLen)
{
    uint8_t aucBufP[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usLenP = 0;
    int i = 0;


    if ((NULL == pucBuf) ||
        (NULL == pusLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    if ((*pusLen < 1) ||
        (*pusLen > (DEF_WSS_NIS_BUF_LEN * 2 / 3))) {
        LOG_WRITE(EN_LOG_ERROR, "The frame length(%d) error", *pusLen);
        goto ERR_LABEL;
    }

    memset(aucBufP, 0, sizeof(aucBufP));

    /* Frame start                                                              */
    aucBufP[0] = DEF_WSS_NIS_FRM_START >> 8;
    aucBufP[1] = DEF_WSS_NIS_FRM_START & 0xFF;

    /* Frame data                                                               */
    usLenP = DEF_WSS_NIS_FRM_LEN_START;
    for (i = 0; i < *pusLen; i++) {
        /* Replace 'dd' with 'dd 04'                                            */
        if (DEF_WSS_NIS_CHAR_ESCAPE == pucBuf[i]) {
            aucBufP[usLenP++] = DEF_WSS_NIS_CHAR_ESCAPE;
            aucBufP[usLenP++] = DEF_WSS_NIS_CHAR_PAD;
        } else {
            aucBufP[usLenP++] = pucBuf[i];
        }
    }

    /* Frame stop                                                               */
    aucBufP[usLenP++] = DEF_WSS_NIS_FRM_STOP >> 8;
    aucBufP[usLenP++] = DEF_WSS_NIS_FRM_STOP & 0xFF;

    memcpy(pucBuf, aucBufP, usLenP);
    *pusLen = usLenP;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

#if 0

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_ParseFrm                                                                      */
/* Descrp : Parse the command from the frame, remove the frame header and tailer, replace 'dd 04' */
/*          with 'dd'                                                                             */
/* Input  : pucBuf-- pointer to the raw frame to parse                                            */
/*        : pusLen-- pointer to the raw frame length to parse, unit: byte                         */
/* Output : pucBuf-- pointer to the parsed command from the raw frame                             */
/*        : pusLen-- pointer to the parsed command length, unit: byte                             */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERR:                                                                  */
/***FUNC-******************************************************************************************/
static int
_WssNis_ParseFrm(uint8_t *pucBuf, uint16_t *pusLen)
{
    uint8_t aucBufP[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usLenFrm = 0;
    uint16_t usLenP = 0;
    uint16_t usLenMax = 0;
    int i = 0;


    if ((NULL == pucBuf) ||
        (NULL == pusLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    if (*pusLen <= DEF_WSS_NIS_FRM_LEN_START_STOP) {
        LOG_WRITE(EN_LOG_ERROR, "The frame length error");
        goto ERR_LABEL;
    }

    usLenFrm = *pusLen;

    /* Check the frame start and stop                                           */
    if ((DEF_WSS_NIS_CHAR_ESCAPE != pucBuf[0]) ||
        (DEF_WSS_NIS_CHAR_START != pucBuf[1]) ||
        (DEF_WSS_NIS_CHAR_ESCAPE != pucBuf[usLenFrm - 2]) ||
        (DEF_WSS_NIS_CHAR_STOP != pucBuf[usLenFrm - 1])) {
        LOG_WRITE(EN_LOG_ERROR, "No frame start or stop");
        goto ERR_LABEL;
    }

    memset(aucBufP, 0, sizeof(aucBufP));

    /* Skip the frame start and stop                                            */
    usLenMax = usLenFrm - DEF_WSS_NIS_FRM_LEN_STOP;
    for (i = DEF_WSS_NIS_FRM_LEN_START; i < usLenMax; i++) {
        if (i + 1 < usLenMax) {
            /* Replace 'dd 04' with 'dd'                                        */
            if ((DEF_WSS_NIS_CHAR_ESCAPE == pucBuf[i]) &&
                (DEF_WSS_NIS_CHAR_PAD == pucBuf[i + 1])) {
                aucBufP[usLenP++] = DEF_WSS_NIS_CHAR_ESCAPE;
                i++;
            } else {
                aucBufP[usLenP++] = pucBuf[i];
            }
        } else {
            aucBufP[usLenP++] = pucBuf[i];
        }
    }

    if (usLenP > (DEF_WSS_NIS_FRM_L_LEN_MAX_R + DEF_WSS_NIS_RES_L_LEN_MID2CRC)) {
        LOG_WRITE(EN_LOG_ERROR, "The parsed command length error, length is %d", usLenP);
        goto ERR_LABEL;
    }

    memcpy(pucBuf, aucBufP, usLenP);
    *pusLen = usLenP;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}
#endif
/***FUNC+******************************************************************************************/
/* Name   : _WssNis_ParseCmd                                                                      */
/* Descrp : Parse the command, fetch the payload                                                  */
/* Input  : pucBuf   -- pointer to the source data buffer to parse                                */
/*        : pusLen   -- pointer to the source data length to parse, unit: byte                    */
/*        : bArray   -- is array frame or not                                                     */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/*        : ucFrmType-- the specified frame type to parse                                         */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_WSS_NIS_FRM_TYPE_S:                                                       */
/*              @arg EN_WSS_NIS_FRM_TYPE_L:                                                       */
/* Output : pucBuf   -- pointer to the parsed payload excluding the command header and tailer     */
/*        : pusLen   -- pointer to the parsed payload length, unit: byte                          */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
static int
_WssNis_ParseCmd(uint8_t *pucBuf, uint16_t *pusLen, bool bArray, uint8_t ucFrmType)
{
    uint8_t ucLenH = 0;
    uint8_t ucLenT = 0;
    uint16_t usLen = 0;
    int i = 0;


    if ((NULL == pucBuf) ||
        (NULL == pusLen)) {
        goto ERR_LABEL;
    }


    /*| Short Command Response Format:          |                               */
    /*|-----------------------------------------|                               */
    /*|  0  |  1  |  2  |  3  |  4 бн n+3 | n+4 |                               */
    /*|-----|-----|-----|-----|-----------|-----|                               */
    /*| MID | Len |Rslt | Cnt |    Data   |Cksum|                               */
    /*|-----------------------------------------|                               */
    /*|                 |Array|                 |                               */
    /*|-----------------------------------------|                               */
    if (EN_WSS_NIS_FRM_TYPE_S == ucFrmType) {
        ucLenH = sizeof(ST_WSS_NIS_RESP_S_HEAD_CMN);
        ucLenT = DEF_WSS_NIS_CMD_S_LEN_TAIL;
    }
    /*| Long Command Response Format:                       |                   */
    /*|-----------------------------------------------------|                   */
    /*|  0  |  1  |  2  |  3  |  4  |  5 бн n+4 | n+5 | n+6 |                   */
    /*|-----|-----------|-----|-----|-----------|-----------|                   */
    /*| MID |    Len    |Rslt | Cnt |    Data   |   Crc16   |                   */
    /*|-----------------------------------------------------|                   */
    /*|                       |Array|                       |                   */
    /*|-----------------------------------------------------|                   */
    else {
        ucLenH = sizeof(ST_WSS_NIS_RESP_L_HEAD_CMN);
        ucLenT = DEF_WSS_NIS_CMD_L_LEN_TAIL;
    }

    if (true == bArray) {
        ucLenH += 1;
    }

    /* Remove the command header and tailer                                     */
    usLen = *pusLen - ucLenH - ucLenT;
    for (i = 0; i < usLen; i++) {
        pucBuf[i] = pucBuf[i + ucLenH];
    }

    *pusLen = usLen;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : _WssNis_SendRecv                                                                      */
/* Descrp : write data into uart and read data from uart                                          */
/* Input  : pucBufW  -- pointer to the data buffer to send                                        */
/*        : usLenW   -- the specified data length to send, unit: byte                             */
/*        : pucBufR  -- pointer to the data buffer to receive                                     */
/*        : pusLenR  -- pointer to the data length to receive, unit: byte                         */
/*        : ucFrmType-- the spcified frame type to send                                           */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_WSS_NIS_FRM_TYPE_S:                                                       */
/*              @arg EN_WSS_NIS_FRM_TYPE_L:                                                       */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERR:                                                                  */
/***FUNC-******************************************************************************************/
#if 0
int
CWssUartDriver::__SendRecv(uint8_t *pucBufW, uint16_t usLenW,
                           uint8_t *pucBufR, uint16_t *pusLenR,
                           uint8_t ucFrmType, uint32_t uiTimeOut)
{
    bool bRet = false;
    uint8_t aucBufR[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usLenR = 0;
    uint16_t usTryW = 0;
    uint16_t usTryR = 0;
    std::shared_ptr<CRspHandler> stpEndPos = nullptr;

#define DEF_WSS_NIS_RECV_WAIT           (160)                               /* Unit: 0.25s for Nistica WSS                             */


    if ((NULL == pucBufW) ||
        (NULL == pucBufR) ||
        (NULL == pusLenR) ||
        (0 == usLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    /* SDV_OplkFpgaUartSwitch(OPLK_MOD_WSS); */

    /* LOG_WRITE(EN_LOG_NOTICE, "ucFrmType = %d", ucFrmType); */

    if (OPLK_OK != _WssNis_PackFrm(pucBufW, &usLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "Pack the frame error");
        goto ERR_LABEL;
    }

    __DBG_WSS_PrintSend(pucBufW, usLenW);

    for (usTryW = 0; usTryW < WSS_CMD_WR_RETRY_TMS; usTryW++) {

        if (nullptr == m_stpUartDrvPtr) {
            LOG_WRITE(EN_LOG_ERROR, "m_stpUartDrvPtr is nullptr.");
            goto ERR_LABEL;
        }

        bRet = m_stpUartDrvPtr->Write(m_uiHwIdx, pucBufW, usLenW, false);
        if (false == bRet) {
            LOG_WRITE(EN_LOG_ERROR, "ReadWrite error.");
            CDelay::Delay(5, MS);
            goto ERR_LABEL;
        }

        for (usTryR = 0; usTryR < WSS_CMD_RD_RETRY_TMS; usTryR++) {

            memset(aucBufR, 0, sizeof(aucBufR));

            stpEndPos = std::static_pointer_cast<CRspHandler>(std::make_shared<CEndTokenWss>());
            m_stpUartDrvPtr->Read(m_uiHwIdx, aucBufR, stpEndPos, uiTimeOut, false);

            usLenR = stpEndPos->m_uiLenR;

            __DBG_WSS_PrintRecv(aucBufR, usLenR);

            /* No frame                                                         */
            if (OPLK_OK != _WssNis_FetchLastFrm(aucBufR, &usLenR)) {
                LOG_WRITE(EN_LOG_ERROR, "No Frame.");
                goto ERR_LABEL;;
            }

            /* Bad frame                                                        */
            if (OPLK_OK != _WssNis_ParseFrm(aucBufR, &usLenR)) {
                LOG_WRITE(EN_LOG_ERROR, "Bad Frame.");
                goto ERR_LABEL;
            }

            /* Read data again if it is in progress                             */
            /* Nistica WSS will reply "Inprogress" repeatly until other result  */
            if (true == _WssNis_IsInProc(aucBufR, usLenR, ucFrmType)) {
                LOG_WRITE(EN_LOG_ERROR, "In Proc.");

                CDelay::Delay(5, MS);
                continue;
            }
            break;
        }

        if (usTryR >= WSS_CMD_RD_RETRY_TMS) {
            goto ERR_LABEL;
        }

        if (true != _WssNis_IsCmdOK(aucBufR, usLenR, ucFrmType)) {
            LOG_WRITE(EN_LOG_ERROR, "Cmd Error.");
            goto ERR_LABEL;
            /* CDelay::Delay(5, MS); */
            /* continue; */
        }

        break;
    }


    if (usTryW >= WSS_CMD_WR_RETRY_TMS) {
        LOG_WRITE(EN_LOG_ERROR, "usTryW(%d) >= WSS_CMD_WR_RETRY_T", usTryW);
        goto ERR_LABEL;
    }



    memcpy(pucBufR, aucBufR, usLenR);
    *pusLenR = usLenR;

    stpEndPos = std::make_shared<CRspHandler>(0);
    m_stpUartDrvPtr->Read(m_uiHwIdx, aucBufR, stpEndPos, 0, true);

    return OPLK_OK;

ERR_LABEL:

    stpEndPos = std::make_shared<CRspHandler>(0);
    m_stpUartDrvPtr->Read(m_uiHwIdx, aucBufR, stpEndPos, 0, true);
    return OPLK_ERR;
}

#endif

int
CWssUartDriver::__SendRecv(uint8_t *pucBufW, uint16_t usLenW,
                           uint8_t *pucBufR, uint16_t *pusLenR,
                           uint8_t ucFrmType, uint32_t uiTimeOut)
{
#if 0
    bool bRet = false;
    uint8_t aucBufR[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usLenR = 0;
    uint16_t usTryW = 0;
    uint16_t usTryR = 0;
#endif
    std::shared_ptr<CRspHandler> stpRsp = nullptr;

#define DEF_WSS_NIS_RECV_WAIT           (160)                               /* Unit: 0.25s for Nistica WSS                             */


    if ((NULL == pucBufW) ||
        (NULL == pucBufR) ||
        (NULL == pusLenR) ||
        (0 == usLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    /* SDV_OplkFpgaUartSwitch(OPLK_MOD_WSS); */

    /* LOG_WRITE(EN_LOG_NOTICE, "ucFrmType = %d", ucFrmType); */

    if (OPLK_OK != _WssNis_PackFrm(pucBufW, &usLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "Pack the frame error");
        goto ERR_LABEL;
    }

    __DBG_WSS_PrintSend(pucBufW, usLenW);

#if 0 /* to do */
    stpRsp = std::static_pointer_cast<CRspHandler>(std::make_shared<CRspWss>());

    m_stpUartDrvPtr->ReadWrite(m_uiHwIdx, pucBufW, usLenW, nullptr, stpRsp);

    if (stpRsp->Len() > 0) {
        memcpy(pucBufR, stpRsp->Data(), stpRsp->Len());
    }
    *pusLenR = stpRsp->Len();
#endif
    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}
/***FUNC+******************************************************************************************/
/* Name   : SDV_WssNisCmdS                                                                        */
/* Descrp : Nistica WSS command write and read                                                    */
/* Input  : pucBufW  -- pointer to the data buffer to write, can be NULL when read                */
/*        : usLenW   -- the specified data length to write, unit: byte                            */
/*        : usCmd    -- the specified command to write and read                                   */
/*        : uOid     -- the specified Object-ID to write and read                                 */
/*        : bTbl     -- has table parameter or not                                                */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/*        : ucTbl    -- the specified table parameter to write                                    */
/*        : bRow     -- has row parameter or not                                                  */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/*        : ucRow    -- the specified row parameter to write                                      */
/*        : usInst1  -- the specified Instance1 parameter to write                                */
/*        : ucPara   -- the specified parameter to write                                          */
/*        : bArray   -- is array messages or single-instance messages                             */
/*            This parameter can be one of the following values:                                  */
/*              @arg true :                                                                  */
/*              @arg false:                                                                  */
/*        : usInstN  -- the InstanceN parameter for the array messages                            */
/*        : ucFrmType-- the spcified frame type to write or read                                  */
/*            This parameter can be one of the following values:                                  */
/*              @arg EN_WSS_NIS_FRM_TYPE_S:                                                       */
/*              @arg EN_WSS_NIS_FRM_TYPE_L:                                                       */
/* Output : pucBufR  -- pointer to the data buffer to read, can be NULL when write                */
/*        : pusLenR  -- pointer to the data length to read, unit: byte                            */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERR:                                                                  */
/***FUNC-******************************************************************************************/
int
CWssUartDriver::__WssNisCmd(uint8_t *pucBufW, uint16_t usLenW,
                            uint8_t *pucBufR, uint16_t *pusLenR,
                            uint16_t usCmd, uint8_t uOid,
                            bool bTbl, uint8_t ucTbl,
                            bool bRow, uint8_t ucRow,
                            uint16_t usInst1, uint8_t ucPara,
                            bool bArray, uint16_t usInstN,
                            uint8_t ucFrmType, uint32_t uiTimeOut)
{
    ST_WSS_NIS_CMD_S_HEAD_ARRAY *pstCmdS = NULL;
    ST_WSS_NIS_CMD_L_HEAD_ARRAY *pstCmdL = NULL;
    uint8_t aucBufW[DEF_WSS_NIS_BUF_LEN] = {0};
    uint8_t aucBufR[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usInstP = 0;
    uint16_t usLenR = 0;
    uint16_t usLenF = 0;
    uint16_t usLenT = 0;
    uint16_t usLen = 0;
    uint16_t usCrc = 0;
    int i = 0;
    bool bLoop = false;


    /* LOG_WRITE(EN_LOG_NOTICE, "__WssNisCmd."); */

    /* LOG_WRITE(EN_LOG_NOTICE, "ucFrmType = %d", ucFrmType); */


#define MACRO_SHIFT_R(pucStart, pucStop, ucLen)                       \
    do {                                                                \
        for (i = ((size_t)pucStop - (size_t)pucStart); i >= 0; i--) {   \
            pucStart[i + ucLen] = pucStart[i];                          \
        }                                                               \
    } while (0)


    if ((usInst1 > DEF_WSS_NIS_INST_MAX) ||
        (usInstN > DEF_WSS_NIS_INST_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "Inst1(%d) or InstN(%d) parameter is overflow", usInst1, usInstN);
        goto ERR_LABEL;
    }

    /* Why????                                                                  */
    /* usCmd |= 0x80; */

    do {
        memset(aucBufW, 0, sizeof(aucBufW));

        pstCmdS = (ST_WSS_NIS_CMD_S_HEAD_ARRAY *)aucBufW;
        pstCmdL = (ST_WSS_NIS_CMD_L_HEAD_ARRAY *)aucBufW;

        /*| Short Command Format:                                                  |*/
        /*|------------------------------------------------------------------------|*/
        /*|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  | 9 бн n+8 |  n+9  |*/
        /*|-----|-----|-----|-----|-----|-----|-----|-----|-----|----------|-------|*/
        /*| MID | Len | Cmd | Oid | Tbl | Row |Inst1|Para |InstN|   Data   | Cksum |*/
        /*|------------------------------------------------------------------------|*/
        /*|                       | Optional  |           |Array|                  |*/
        /*|------------------------------------------------------------------------|*/
        if (EN_WSS_NIS_FRM_TYPE_S == ucFrmType) {
            usLenF = usLenW > DEF_WSS_NIS_FRM_S_LEN_MAX_W
                     ? DEF_WSS_NIS_FRM_S_LEN_MAX_W
                     : usLenW;

            /* Array                                                            */
            if (true == bArray) {
                bLoop = true;

                if (usInstN < usInst1) {
                    LOG_WRITE(EN_LOG_ERROR, "InstN is less than Inst1");
                    goto ERR_LABEL;
                }

                uint32_t usInstNum = usInstN - usInst1 + 1;
                usInstP = usInstNum > DEF_WSS_NIS_FRM_S_LEN_MAX_W / 2
                          ? (usInst1 + DEF_WSS_NIS_FRM_S_LEN_MAX_W / 2 - 1)
                          : usInstN;
            }
            /* Single                                                           */
            else {
                bLoop = false;
            }

            pstCmdS->stHeadS.ucMid = __WSS_NIS_MID();
            pstCmdS->stHeadS.ucLen = DEF_WSS_NIS_CMD_S_LEN_CMD2PARA + usLenF + DEF_WSS_NIS_CMD_S_LEN_TAIL;
            pstCmdS->stHeadS.ucCmd = usCmd;
            pstCmdS->ucOid = uOid;
            /* pstCmdS->ucTbl = ucTbl  (if bTbl)                                */
            /* pstCmdS->ucRow = ucRow  (if bRow)                                */
            pstCmdS->ucInst1 = usInst1 & 0xFF;
            /* Bit 7~6 in ucPara is bit 9~8 of Inst1                            */
            /* Bit 5~4 in ucPara is bit 9~8 of InstN                            */
            pstCmdS->ucPara = ((usInst1 >> 2) & 0xC0) | ((usInstP >> 4) & 0x30) | (ucPara & 0x0F);
            pstCmdS->ucInstN = usInstP & 0xFF;

            /* Append the 'InstN' item                                          */
            if (true == bArray) {
                pstCmdS->stHeadS.ucLen += 1;                                        /* Append InstN(1B)                                        */
            }

            /* Insert the 'Row' item after 'Oid' item                           */
            if (true == bRow) {
                pstCmdS->stHeadS.ucCmd |= EN_WSS_NIS_CMD_ADD_ROW;
                MACRO_SHIFT_R((&pstCmdS->ucInst1),
                              (&pstCmdS->ucInst1 + pstCmdS->stHeadS.ucLen - DEF_WSS_NIS_CMD_S_LEN_CMD2OID),
                              1);
                pstCmdS->stHeadS.ucLen += 1;

                pstCmdS->ucInst1 = ucRow;
            }

            /* Insert the 'Tbl' item after 'Oid' item                           */
            if (true == bTbl) {
                pstCmdS->stHeadS.ucCmd |= EN_WSS_NIS_CMD_ADD_TABLE;
                MACRO_SHIFT_R((&pstCmdS->ucInst1),
                              (&pstCmdS->ucInst1 + pstCmdS->stHeadS.ucLen - DEF_WSS_NIS_CMD_S_LEN_CMD2OID),
                              1);
                pstCmdS->stHeadS.ucLen += 1;

                pstCmdS->ucInst1 = ucTbl;
            }

            /* Frame header length                                              */
            usLen = DEF_WSS_NIS_CMD_S_LEN_MID2LEN
                    + pstCmdS->stHeadS.ucLen
                    - usLenF
                    - DEF_WSS_NIS_CMD_S_LEN_TAIL;

            /* Append payload                                                   */
            if (nullptr != pucBufW) {
                printf("Append payload!");
                memcpy(aucBufW + usLen, pucBufW, usLenF);
                usLen += usLenF;
            }

            /* Append tailer                                                    */
            usCrc = _WssNis_ChkSum(aucBufW, usLen);
            aucBufW[usLen++] = usCrc & 0xFF;
        }
        /*| Long Command Format:                                                                                 |*/
        /*|------------------------------------------------------------------------------------------------------|*/
        /*|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  |13 бн n+12| n+13 | n+14 |*/
        /*|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----------|------|------|*/
        /*| MID |0x00 |0xFF |    Len    |    Cmd    | Oid | Tbl | Row |Inst1|Para |InstN|   Data   |    CRC16    |*/
        /*|------------------------------------------------------------------------------------------------------|*/
        /*|                                               | Optional  |           |Array|                        |*/
        /*|------------------------------------------------------------------------------------------------------|*/
        else {
            usLenF = usLenW > DEF_WSS_NIS_FRM_L_LEN_MAX_W
                     ? DEF_WSS_NIS_FRM_L_LEN_MAX_W
                     : usLenW;

            /* Array                                                            */
            if (true == bArray) {
                bLoop = true;

                if (usInstN < usInst1) {
                    LOG_WRITE(EN_LOG_ERROR, "InstN is less than Inst1");
                    goto ERR_LABEL;
                }

                uint32_t usInstNum = usInstN - usInst1 + 1;
                usInstP = usInstNum > DEF_WSS_NIS_FRM_L_LEN_MAX_W / 2
                          ? (usInst1 + DEF_WSS_NIS_FRM_L_LEN_MAX_W / 2 - 1)
                          : usInstN;
            }
            /* Single                                                           */
            else {
                bLoop = false;
            }

            pstCmdL->stHeadL.ucMid = __WSS_NIS_MID();
            pstCmdL->stHeadL.usTag = DEF_WSS_NIS_FRM_L_TAG;
            pstCmdL->stHeadL.usLen = DEF_WSS_NIS_CMD_L_LEN_CMD2PARA + usLenF + DEF_WSS_NIS_CMD_L_LEN_TAIL;
            pstCmdL->stHeadL.usCmd = usCmd;

            pstCmdL->ucOid = uOid;


            /* pstCmdS->ucTbl = ucTbl  (if bTbl)                                */
            /* pstCmdS->ucRow = ucRow  (if bRow)                                */
            pstCmdL->ucInst1 = usInst1 & 0xFF;
            /* Bit 7~6 in ucPara is bit 9~8 of Inst1                            */
            /* Bit 5~4 in ucPara is bit 9~8 of InstN                            */
            pstCmdL->ucPara = ((usInst1 >> 2) & 0xC0) | ((usInstP >> 4) & 0x30) | (ucPara & 0x0F);
            pstCmdL->ucInstN = usInstP & 0xFF;

            /* Append the 'InstN' item                                          */
            if (true == bArray) {
                pstCmdL->stHeadL.usLen += 1;                                        /* Append InstN(1B)                                        */
            }

            /* Insert the 'Row' item after 'Oid' item                           */
            if (true == bRow) {


                pstCmdL->stHeadL.usCmd |= EN_WSS_NIS_CMD_ADD_ROW;
                MACRO_SHIFT_R((&pstCmdL->ucInst1),
                              (&pstCmdL->ucInst1 + pstCmdL->stHeadL.usLen - DEF_WSS_NIS_CMD_L_LEN_CMD2OID),
                              1);
                pstCmdL->stHeadL.usLen += 1;

                pstCmdL->ucInst1 = ucRow;
            }

            /* Insert the 'Tbl' item after 'Oid' item                           */
            if (true == bTbl) {

                pstCmdL->stHeadL.usCmd |= EN_WSS_NIS_CMD_ADD_TABLE;
                MACRO_SHIFT_R((&pstCmdL->ucInst1),
                              (&pstCmdL->ucInst1 + pstCmdL->stHeadL.usLen - DEF_WSS_NIS_CMD_L_LEN_CMD2OID),
                              1);
                pstCmdL->stHeadL.usLen += 1;

                pstCmdL->ucInst1 = ucTbl;
            }

            /* Frame header length                                              */
            usLen = DEF_WSS_NIS_CMD_L_LEN_MID2LEN_W
                    + pstCmdL->stHeadL.usLen
                    - usLenF
                    - DEF_WSS_NIS_CMD_L_LEN_TAIL;
#if 0
            printf("usLen = %d:\n", usLen);
            printf("pstCmdL->stHeadL.usLen = %d:\n", pstCmdL->stHeadL.usLen);
            for (int i = 0; i < 32; ++i) {
                printf("%02x ", *((uint8_t *)aucBufW + i));
                if (((i + 1) % 16) == 0) {
                    printf("\r\n");
                }
            }
            printf("\r\n");
#endif

            pstCmdL->stHeadL.usTag = htons(pstCmdL->stHeadL.usTag);
            pstCmdL->stHeadL.usLen = htons(pstCmdL->stHeadL.usLen);
            pstCmdL->stHeadL.usCmd = htons(pstCmdL->stHeadL.usCmd);

            /* Append payload                                                   */
            if (NULL != pucBufW) {
                memcpy(aucBufW + usLen, pucBufW, usLenF);
                usLen += usLenF;
            }

            /* Append tailer                                                    */
            usCrc = _WssNis_Crc16(aucBufW, usLen);
            aucBufW[usLen++] = usCrc >> 8;
            aucBufW[usLen++] = usCrc & 0xFF;
        }

        usLenR = 0;


        if (OPLK_OK != __SendRecv(aucBufW, usLen, aucBufR, &usLenR, ucFrmType, uiTimeOut)) {
            LOG_WRITE(EN_LOG_ERROR, "Send data to WSS error");
            goto ERR_LABEL;
        }

        pucBufW += usLenF;
        usLenW -= usLenF;

        /* Array                                                                */
        if (true == bArray) {
            usInst1 = usInstP + 1;

            if (usInst1 >= usInstN) {
                bLoop = false;
            }
        }

        if (NULL != pucBufR) {
            if (OPLK_OK == _WssNis_ParseCmd(aucBufR, &usLenR, bArray, ucFrmType)) {
                memcpy(pucBufR + usLenT, aucBufR, usLenR);
                usLenT += usLenR;
            }
        }
    } while (bLoop);

    if (NULL != pusLenR) {
        *pusLenR = usLenT;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_WssNisCmdMultiW                                                                   */
/* Descrp : Nistica WSS multi-object commands to write for center frequency and bandwidth         */
/* Input  : pstMulti-- pointer to the ST_WSS_NIS_MULTI structure which contain some channels'     */
/*                     center frequency and bandwidth to write                                    */
/*        : usCnt   -- the specified counters of all the multi-object commands to write, unit:    */
/*                     ST_WSS_NIS_MULTI                                                           */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
CWssUartDriver::__WssNisCmdMultiW(ST_WSS_NIS_MULTI *pstMulti, uint16_t usCnt)
{
    ST_WSS_NIS_CMD_L_HEAD_CMN *pstCmdL = NULL;
    ST_WSS_NIS_CMD_L_MULTI *pstCmdMulti = NULL;
    uint8_t aucBufW[DEF_WSS_NIS_BUF_LEN] = {0};
    uint8_t aucBufR[DEF_WSS_NIS_BUF_LEN] = {0};
    uint16_t usLenW = 0;
    uint16_t usLenR = 0;
    uint16_t usCntAll = 0;
    uint16_t usCntFrm = 0;
    uint16_t usCrc = 0;
    uint16_t usCh;
    uint16_t usFreq;
    uint16_t usBand;
    uint8_t ucPara;
    int i;

#define DEF_WSS_NIS_MULTI_PAIR          (2)


    if ((NULL == pstMulti) ||
        (0 == usCnt)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        goto ERR_LABEL;
    }

    /*| Multi-Object Commands in Long Format:                                                                |*/
    /*|------------------------------------------------------------------------------------------------------|*/
    /*|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  |13 бн n+12| n+13 | n+14 |*/
    /*|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----------|------|------|*/
    /*| MID |0x00 |0xFF |    Len    | Cmd(0x12) |Oid1 |Tbl1 |Inst1|Para1|   Data1   |                        |*/
    /*|                                         |Oid2 |Tbl2 |Inst2|Para2|   Data2   |                        |*/
    /*|                                                      ...                                             |*/
    /*|                                         |OidN |TblN |InstN|ParaN|   DataN   |          |    CRC16    |*/
    /*|------------------------------------------------------------------------------------------------------|*/
    do {
        usCntFrm = usCnt > DEF_WSS_NIS_MULTI_CNT_FRM
                   ? DEF_WSS_NIS_MULTI_CNT_FRM
                   : usCnt;

        pstCmdL = (ST_WSS_NIS_CMD_L_HEAD_CMN *)aucBufW;
        pstCmdMulti = (ST_WSS_NIS_CMD_L_MULTI *)&aucBufW[sizeof(ST_WSS_NIS_CMD_L_HEAD_CMN)];

        /* Command header                                                       */
        pstCmdL->ucMid = __WSS_NIS_MID();
        pstCmdL->usTag = DEF_WSS_NIS_FRM_L_TAG;
        pstCmdL->usLen = DEF_WSS_NIS_CMD_L_LEN_CMD + DEF_WSS_NIS_CMD_L_LEN_TAIL
                         + sizeof(ST_WSS_NIS_CMD_L_MULTI) * usCntFrm * DEF_WSS_NIS_MULTI_PAIR;
        pstCmdL->usCmd = EN_WSS_NIS_CMD_MULTI_WR | EN_WSS_NIS_CMD_ADD_TABLE;
        usLenW = sizeof(ST_WSS_NIS_CMD_L_HEAD_CMN);

        /* printf("usLenW = %d\n", usLenW); */
        ucPara = EN_WSS_NIS_PARA_SET_POINT;

        /* Payload                                                              */
        for (i = 0; i < usCntFrm; i++) {
            usCh = pstMulti[usCntAll + i].usCh;
            usFreq = pstMulti[usCntAll + i].usFreq;
            usBand = pstMulti[usCntAll + i].usBand;


            /* A. Center frequency                                              */
            pstCmdMulti->ucOid = EN_WSS_NIS_OID_WP_CH_FREQ;
            pstCmdMulti->ucTbl = 0;
            pstCmdMulti->ucInst = usCh & 0xFF;

            /* Bit 7~6 in ucPara is bit 9~8 of Inst                             */
            pstCmdMulti->ucPara = ((usCh >> 2) & 0xC0) | (ucPara & 0x3F);
            pstCmdMulti->usData = htons(usFreq);

            pstCmdMulti++;

            /* B. Bandwidth                                                     */
            pstCmdMulti->ucOid = EN_WSS_NIS_OID_WP_CH_BW;
            pstCmdMulti->ucTbl = 0;
            pstCmdMulti->ucInst = usCh & 0xFF;
            /* Bit 7~6 in ucPara is bit 9~8 of Inst                             */
            pstCmdMulti->ucPara = ((usCh >> 2) & 0xC0) | (ucPara & 0x3F);
            pstCmdMulti->usData = htons(usBand);

            pstCmdMulti++;

            usLenW += sizeof(ST_WSS_NIS_CMD_L_MULTI) * DEF_WSS_NIS_MULTI_PAIR;
        }

        pstCmdL->usLen = htons(pstCmdL->usLen);
        pstCmdL->usCmd = htons(pstCmdL->usCmd);
        pstCmdL->usTag = htons(pstCmdL->usTag);

        /* Append tailer                                                        */
        usCrc = _WssNis_Crc16(aucBufW, usLenW);
        aucBufW[usLenW++] = usCrc >> 8;
        aucBufW[usLenW++] = usCrc & 0xFF;

        usLenR = 0;
        if (OPLK_OK != __SendRecv(aucBufW, usLenW,
                                  aucBufR, &usLenR,
                                  EN_WSS_NIS_FRM_TYPE_L)) {
            LOG_WRITE(EN_LOG_ERROR, "Send multi-object data to WSS error");
            goto ERR_LABEL;
        }

        /* Update the counter variable                                          */
        usCntAll += usCntFrm;
        usCnt -= usCntFrm;

    } while (usCnt);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}


bool
CWssUartDriver::IsOnline(void)
{

    return true;
}

typedef enum {
    EN_WSS_NIS_VENDOR,
    EN_WSS_NIS_PN,
    EN_WSS_NIS_SN,
    EN_WSS_NIS_VER_FW,
    EN_WSS_NIS_VER_HW,
    EN_WSS_NIS_BUILD,
    EN_WSS_NIS_DATE,

    EN_WSS_NIS_VER_MAX

} EN_WSS_NIS_VER;


bool
CWssUartDriver::GetMfg(CMfg &rstMfg)
{

    uint16_t usLenR = 0;
    char pcVerInfo[EN_WSS_NIS_VER_MAX][MFG_INFO_LEN_MAX] = {{0}};
    const char *pcDelim = ";";
    char *pcTmp = nullptr;
    int i = 0;
    int iRet = 0;

    LOG_WRITE(EN_LOG_DEBUG, "CWssUartDriver::GetMfg +++");

    if ((nullptr == m_stpUartDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staRsp(1024, 0);
    /* m_stpProtoPtr->GetMfgCmd(staRsg); */

    iRet = WssNisCmdSglR((uint8_t *)staRsp.data(),
                         &usLenR,
                         EN_WSS_NIS_OID_MOUDLE_INFO,
                         DEF_WSS_NIS_INST);

    LOG_WRITE(EN_LOG_DEBUG, "wss mfg: %s", staRsp.c_str());

    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "CWssUartDriver::GetMfg ---");
        return false;
    }

    /* Parse version information                                                */
    snprintf(pcVerInfo[EN_WSS_NIS_VENDOR],
             MFG_INFO_LEN_MAX,
             "%s",
             strtok((char *)staRsp.data(), pcDelim));

    while ((pcTmp = strtok(NULL, pcDelim))) {
        i++;
        if (i >= EN_WSS_NIS_VER_MAX) {
            break;
        }
        snprintf(pcVerInfo[i], MFG_INFO_LEN_MAX, "%s", pcTmp);
        LOG_WRITE(EN_LOG_DEBUG, "EN_WSS_NIS_VER_FW[%d] = %s", i, pcVerInfo[i]);
    }

    snprintf(rstMfg.acManufacturer, MFG_INFO_LEN_MAX, "%s", "Molex");
    snprintf(rstMfg.acPN, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_PN]);
    snprintf(rstMfg.acSN, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_SN]);
    snprintf(rstMfg.acSwVer, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_VER_FW]);
    snprintf(rstMfg.acFwVer, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_VER_FW]);
    snprintf(rstMfg.acHwVer, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_VER_HW]);
    snprintf(rstMfg.acHwFuncRev, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_VER_HW]);
    snprintf(rstMfg.acPcbRev, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_VER_HW]);
    snprintf(rstMfg.acMfgDate, MFG_INFO_LEN_MAX, "%s", pcVerInfo[EN_WSS_NIS_DATE]);

    /* std::vector<std::string> staSnVec = {"N019L26976", "N020J57762", "N020L61073"}; */
    std::vector<std::string> staSnVec = {"N020L61541",
                                         "N020L61827",
                                         "N020K57948",
                                         "N020L61054",
                                         "N020L61077",
                                         "N020L61540",
                                         "N020L61160",
                                         "N020J57237",
                                         "N020J57260"};

    auto it = std::find(staSnVec.begin(), staSnVec.end(), std::string(rstMfg.acSN));

    if (it != staSnVec.end()) {
        m_uiPortCnt = 33;
    }
    /* printf("sn = %s, port num = %d\n", rstMfg.acSN, m_uiPortCnt); */

    LOG_WRITE(EN_LOG_DEBUG, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_DEBUG, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_DEBUG, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_DEBUG, "acSwVer %s", rstMfg.acSwVer);
    LOG_WRITE(EN_LOG_DEBUG, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_DEBUG, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_DEBUG, "acHwFuncRev %s", rstMfg.acHwFuncRev);
    LOG_WRITE(EN_LOG_DEBUG, "acPcbRev %s", rstMfg.acPcbRev);
    LOG_WRITE(EN_LOG_DEBUG, "acMfgDate %s", rstMfg.acMfgDate);


    LOG_WRITE(EN_LOG_DEBUG, "CWssUartDriver::GetMfg ---");

    return true;
/*
    m_stpUartDrvPtr->Write(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size());

    std::string staRsp(OA_CLI_RSP_MAX_LEN, 0);
    m_stpUartDrvPtr->Read(m_uiHwIdx, (uint8_t *)staRsp.data(), OA_READ_NUM);

    if (m_stpProtoPtr->ProcRsp(staRsp))
    {
        m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg);

        return true;
    }

    return false;*/

    sprintf(rstMfg.acSN, "%s", "SN:123456");
    sprintf(rstMfg.acPN, "%s", "PN:abcd");
    sprintf(rstMfg.acModuleType, "%s", "WSS");
    sprintf(rstMfg.acBootVer, "0x%02X%02X", 0x12, 0x34);
    sprintf(rstMfg.acFwVer, "0x%02X%02X", 0x1, 0x2);
    sprintf(rstMfg.acHwVer, "%d.%d.%d", 1, 1, 1);
    sprintf(rstMfg.acCaliDate, "%02d%02d.%d.%d", 20, 18, 11, 22);
    return true;

}

bool
CWssUartDriver::GetStatusData(CWssStatusData &rstData)
{
    if (m_bCommFailDebug) {
        return false;
    }

    CMfg stMfg = {0};
    return GetMfg(stMfg);
}

bool
CWssUartDriver::GetCfgData(CWssAllCfg &rstData)
{
    uint16_t usChMax = WSS_CH_CNT_TOTAL;
    uint16_t usLen = 0;

    CWssCfg *pstWssCfgAdd = rstData.astWssChCfgAdd;
    CWssCfg *pstWssCfgDrop = rstData.astWssChCfgDrop;

    ST_WSS_NIS_VAL stNisVal;
    memset(&stNisVal, 0, sizeof(stNisVal));

    /* Frequency																*/
    if (OPLK_OK != WssNisCmdArrR((uint8_t *)stNisVal.ausFreq, &usLen,
                                 EN_WSS_NIS_OID_WP_CH_FREQ,
                                 0, WSS_CH_IDX_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "Flexgrid read frequency error\n");
        goto ERR_LABEL;
    }

    /* Bandwidth																*/
    if (OPLK_OK != WssNisCmdArrR((uint8_t *)stNisVal.ausBand, &usLen,
                                 EN_WSS_NIS_OID_WP_CH_BW,
                                 0, WSS_CH_IDX_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "Flexgrid read bandwidth error\n");
        goto ERR_LABEL;
    }

    /* printf("Waveplan:\n"); */
    for (int i = 0; i < WSS_CH_CNT_TOTAL; ++i) {
        uint32_t uiChId = i;
        uint32_t uiBand = ntohs(stNisVal.ausBand[i]);
        uint32_t uiCenterFreq = ntohs(stNisVal.ausFreq[i]);

        rstData.astWavePlan[i].uiChId = uiChId;
        rstData.astWavePlan[i].uiCenterFreq = uiCenterFreq / 2;
        rstData.astWavePlan[i].usSliceCount = uiBand / 2;
    }

    ST_PORT_ATTN stPortAttn[WSS_CH_CNT_TOTAL];

    /* WSS1 port and attn														*/
    if (OPLK_OK != WssNisCmdArrTblR((uint8_t *)stNisVal.ausPortAttn1, &usLen,
                                    EN_WSS_NIS_OID_PACK_CH_PORT_ATTN,
                                    EN_WSS_DIR_ADD,
                                    0, WSS_CH_IDX_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "Flexgrid read port and attn 1 error\n");
        goto ERR_LABEL;
    }


    memset(stPortAttn, 0, sizeof(stPortAttn));
    memcpy(stPortAttn, stNisVal.ausPortAttn1, sizeof(stPortAttn));

    for (int i = 0; i < WSS_CH_CNT_TOTAL; ++i) {
        uint32_t uiId = i;
        uint32_t uiExpPort = stPortAttn[i].ucPort;


        uint32_t uiComPort = 0;

        if (uiExpPort > m_uiPortCnt) {
            uiComPort = WSS_LOOPBACK_PORT;
            uiExpPort = uiExpPort - m_uiPortCnt;
        }

        pstWssCfgAdd->uiId = uiId;
        pstWssCfgAdd->uiComPort = uiComPort;
        pstWssCfgAdd->uiExpPort = uiExpPort;
        pstWssCfgAdd->uiAtten = stPortAttn[i].ucAttn;

        pstWssCfgAdd++;
    }

    /* WSS2 port and attn														*/
    if (OPLK_OK != WssNisCmdArrTblR((uint8_t *)stNisVal.ausPortAttn2, &usLen,
                                    EN_WSS_NIS_OID_PACK_CH_PORT_ATTN,
                                    EN_WSS_DIR_DROP,
                                    0, WSS_CH_IDX_MAX)) {
        LOG_WRITE(EN_LOG_ERROR, "Flexgrid read port and attn 2 error\n");
        goto ERR_LABEL;
    }

    memset(stPortAttn, 0, sizeof(stPortAttn));
    memcpy(stPortAttn, stNisVal.ausPortAttn2, sizeof(stPortAttn));

    for (int i = 0; i < WSS_CH_CNT_TOTAL; ++i) {
        uint32_t uiId = i;
        uint32_t uiExpPort = stPortAttn[i].ucPort;
        uint32_t uiComPort = 0;

        if (uiExpPort > m_uiPortCnt) {
            uiComPort = WSS_LOOPBACK_PORT;
            uiExpPort = uiExpPort - m_uiPortCnt;
        }

        pstWssCfgDrop->uiId = uiId;
        pstWssCfgDrop->uiComPort = uiComPort;
        pstWssCfgDrop->uiExpPort = uiExpPort;
        pstWssCfgDrop->uiAtten = stPortAttn[i].ucAttn;

        pstWssCfgDrop++;
    }

    /* Update the channel count                                                                                                 */
    if (OPLK_OK != WssNisCmdSglW((uint8_t *)&usChMax, sizeof(usChMax),
                                 EN_WSS_NIS_OID_WP_CH_NUM,
                                 DEF_WSS_NIS_INST)) {
        LOG_WRITE(EN_LOG_ERROR, "Write WSS channel number error");
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;

}


bool
CWssUartDriver::SetCfg(uint32_t uiInstBegin, uint32_t uiInstEnd, CWssCfg *pstData, uint32_t uiSubIdx)
{
    ST_WSS_CH_INFO astCfg[WSS_CH_CNT_TOTAL];
    memset(&astCfg, 0, sizeof(astCfg));

    LOG_WRITE(EN_LOG_NOTICE, "uiInst1 = %d, uiInstN = %d", uiInstBegin, uiInstEnd);

    for (uint32_t i = 0; i < (uiInstEnd - uiInstBegin + 1); ++i) {
        astCfg[i].ucPort = pstData->uiExpPort;
        LOG_WRITE(EN_LOG_DEBUG, "pstData->uiExpPort = %d", pstData->uiExpPort);

        if (WSS_LOOPBACK_PORT == pstData->uiComPort) {
            astCfg[i].ucPort += m_uiPortCnt;
        }
        astCfg[i].ucAttn = pstData->uiAtten;
        LOG_WRITE(EN_LOG_DEBUG, "pstData->uiAtten = %d", pstData->uiAtten);

        pstData++;
    }

    if (OPLK_OK != WssNisCmdArrTblW((uint8_t *)astCfg,
                                    EN_WSS_NIS_OID_PACK_CH_PORT_ATTN,
                                    uiSubIdx,
                                    uiInstBegin, uiInstEnd)) {
        return false;
    }

    return true;
}

bool
CWssUartDriver::SetOneChCfg(const CWssCfg &c_rstData, uint32_t uiSubIdx)
{

    return true;
}

bool
CWssUartDriver::SetAllChCfg(uint32_t uiChCnt, CWssCfg *pstData, uint32_t uiSubIdx)
{
    ST_WSS_CH_INFO astCfg[WSS_CH_CNT_TOTAL];
    memset(&astCfg, 0, sizeof(astCfg));

#if 0
    printf("CWssUartDriver::SetAllChCfg Cfg +++ %d:\r\n", uiChCnt);
    for (int i = 0; i < (int)(sizeof(CWssCfg) * uiChCnt); ++i) {
        printf("%02x ", *((uint8_t *)pstData + i));
        if (((i + 1) % 32) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
    printf("Cfg Up ---:\r\n");
#endif

    LOG_WRITE(EN_LOG_NOTICE, "uiChCnt = %d, uiSubIdx = %d", uiChCnt, uiSubIdx);

    for (uint32_t i = 0; i < uiChCnt; ++i) {
        astCfg[i].ucPort = pstData->uiExpPort;
        LOG_WRITE(EN_LOG_DEBUG, "pstData->uiExpPort = %d", pstData->uiExpPort);
        if (WSS_LOOPBACK_PORT == pstData->uiComPort) {

            astCfg[i].ucPort += m_uiPortCnt;
            printf("m_uiPortCnt = %d\n", m_uiPortCnt);
        }
        astCfg[i].ucAttn = pstData->uiAtten;
        LOG_WRITE(EN_LOG_DEBUG, "pstData->uiAtten = %d", pstData->uiAtten);

        pstData++;
    }

#if 0
    printf("CWssUartDriver::SetAllChCfg Cfg Up +++ %d:\r\n", uiChCnt);
    for (uint32_t i = 0; i < sizeof(astCfg); ++i) {
        printf("%02x ", *((uint8_t *)astCfg + i));
        if (((i + 1) % 32) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
    printf("Cfg Up ---:\r\n");
#endif

    printf("uiSubIdx = %d\n", uiSubIdx);
    if (OPLK_OK != WssNisCmdArrTblW((uint8_t *)astCfg,
                                    EN_WSS_NIS_OID_PACK_CH_PORT_ATTN,
                                    uiSubIdx,
                                    1, uiChCnt)) {
        return false;
    }

    return true;
}

bool
CWssUartDriver::SetWavePlan(uint32_t uiChCnt, CWavePlan *pstWavePlan, uint32_t uiSubIdx)
{
    ST_WSS_NIS_MULTI astWaveplan[WSS_CH_CNT_TOTAL];
    memset(&astWaveplan, 0, sizeof(astWaveplan));

    for (uint32_t i = 0; i < uiChCnt; i++) {

        /* uint16_t usFreq = (pstWavePlan->uiCenterFreq * FLEXGRID_WSS_RATIO) - (usBand / 2); */
        uint16_t usFreq = pstWavePlan->uiCenterFreq * FLEXGRID_WSS_RATIO;
        uint16_t usBand = pstWavePlan->usSliceCount * FLEXGRID_WSS_RATIO;

        astWaveplan[i].usCh = pstWavePlan->uiChId;
        astWaveplan[i].usFreq = usFreq;
        astWaveplan[i].usBand = usBand;

        pstWavePlan++;
    }

    __WssNisCmdMultiW(astWaveplan, uiChCnt);
    return true;
}


bool
CWssUartDriver::Reset(uint8_t ucResetType)
{
    if (RST_MODE_WARM == ucResetType) {
        LOG_WRITE(EN_LOG_NOTICE, "Wss Warm Reset. ResetIdx = %d", m_uiWarmResetIdx);
        if (!m_stpWarmResetBoard->Reset(m_uiWarmResetIdx)) {
            LOG_WRITE(EN_LOG_ERROR, "Wss Warm Reset error. Reset Idx = %d", m_uiWarmResetIdx);
            return false;
        }
    } else if (RST_MODE_COLD == ucResetType) {
        LOG_WRITE(EN_LOG_NOTICE, "Wss Cold Reset. ResetIdx = %d", m_uiColdResetIdx);
        if (!m_stpColdResetBoard->Reset(m_uiColdResetIdx)) {
            LOG_WRITE(EN_LOG_ERROR, "Wss Cold Reset error. Reset Idx = %d", m_uiColdResetIdx);
            return false;
        }
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Wss Unknown Reset Type(%d).", ucResetType);
        return false;
    }
    return true;
}

bool
CWssUartDriver::Upgrade(const char *pcFilePath)
{
    uint8_t *pucBufW = nullptr;
    uint8_t *pucData = nullptr;
    int32_t iFileLen = 0;
    uint16_t usLenW = 0;
    uint32_t uiRetryCnt = 0;

#define DEF_WSS_NIS_ERASE_WAIT          (20)                                /* Unit: 1s                                                */
#define DEF_WSS_NIS_RESET_WAIT          (10)                                /* Unit: 1s                                                */
#define DEF_WSS_NIS_COMMIT_RETRY_TIMES  (10)                                                            /* Unit: 1s                                                */
#define DEF_WSS_NIS_COMMIT_RETRY_DELAY  (1)                                                                     /* Unit: 1s                                                */

    LOG_WRITE(EN_LOG_NOTICE, " ++ _NIS_WssUpgdFw");
    LOG_WRITE(EN_LOG_NOTICE, "start down load fw");

    if (OPLK_OK != WssNisCmdUpgdR(EN_WSS_NIS_CMD_UPGD_START, 100)) {
        LOG_WRITE(EN_LOG_ERROR, "Start download fw error.");
        goto ABORT_LABEL;
    }

    /* 30 seconds erase time in Nistica WSS Spec                                */
    CDelay::Delay(DEF_WSS_NIS_ERASE_WAIT, S);

    LOG_WRITE(EN_LOG_NOTICE, "write image into wss");

    pucBufW = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBufW) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    pucData = pucBufW;

    /* Try to download the first package of the image                           */
    /* pucBufW = (uint8_t *)pData; */

    do {
        usLenW = iFileLen > DEF_WSS_NIS_FW_LEN
                 ? DEF_WSS_NIS_FW_LEN
                 : iFileLen;

        if (OPLK_OK != WssNisCmdUpgdW(pucBufW, usLenW)) {
            LOG_WRITE(EN_LOG_ERROR, "Write image into wss error");
            goto ABORT_LABEL;
        }

        pucBufW += usLenW;
        iFileLen -= usLenW;
    } while (iFileLen);

    LOG_WRITE(EN_LOG_NOTICE, "verify down load fw");

#if 1
    if (OPLK_OK != WssNisCmdUpgdR(EN_WSS_NIS_CMD_UPGD_VERIFY, 100)) {
        LOG_WRITE(EN_LOG_ERROR, "Verify down load fw failure.");
        /* goto ABORT_LABEL; */
    }
#endif

    LOG_WRITE(EN_LOG_NOTICE, "switch fw");
    if (OPLK_OK != WssNisCmdUpgdR(EN_WSS_NIS_CMD_UPGD_SWITCH, 100)) {
        LOG_WRITE(EN_LOG_ERROR, "Switch fw failure.");
        goto ABORT_LABEL;
    }

    CDelay::Delay(DEF_WSS_NIS_RESET_WAIT, S);

    LOG_WRITE(EN_LOG_NOTICE, "commit fw");
    for (uiRetryCnt = 0; uiRetryCnt < DEF_WSS_NIS_COMMIT_RETRY_TIMES; uiRetryCnt++) {
        if (OPLK_OK != WssNisCmdUpgdR(EN_WSS_NIS_CMD_UPGD_COMMIT, 1000)) {
            LOG_WRITE(EN_LOG_ERROR, "Commit fw failure. uiRetryCnt = %d", uiRetryCnt);
            CDelay::Delay(DEF_WSS_NIS_COMMIT_RETRY_DELAY, S);
            continue;
        }
        break;
    }

    if (uiRetryCnt >= DEF_WSS_NIS_COMMIT_RETRY_TIMES) {
        goto ABORT_LABEL;
    }

    CDelay::Delay(DEF_WSS_NIS_RESET_WAIT, S);

    DEF_SAFE_FREE(pucData);
    LOG_WRITE(EN_LOG_NOTICE, " -- _NIS_WssUpgdFw");
    return true;

ABORT_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "abort fw");
    WssNisCmdUpgdR(EN_WSS_NIS_CMD_UPGD_ABORT, 100);
    DEF_SAFE_FREE(pucData);

    LOG_WRITE(EN_LOG_ERROR, " ** _NIS_WssUpgdFw");
    return false;

ERR_LABEL:
    DEF_SAFE_FREE(pucData);
    LOG_WRITE(EN_LOG_ERROR, " ** _NIS_WssUpgdFw");
    return false;
}


