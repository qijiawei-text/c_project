/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : operationd_alm.c
   Description : Alarm cb and optical alarm report.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "sysrepo.h"
#include "util_inc.h"
#include "DSC_inc.h"
#include "oplkalm.h"
#include "operationd_alm.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"

#include "operationd_config_mfg.h"

/* #define UPGRADE_ALM_NUM                    3 */
#define RESET_ALM_NUM                      2
#define SFTP_IP_UNREACHABLE_ALM_NUM        1
#define ALM_TABLE_UNIT_SIZE                10


static ALM_UNIT_ST m_astUpgdAlm[] = {
    {UPGRADE_ALM_FAILED, "", "", 0, false, false},
    {UPGRADE_ALM_ACTIVE, "", "", 0, false, false},
    {UPGRADE_ALM_IN_PROGRESS, "", "", 0, false, false},
    {UPGRADE_ALM_OCS_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_OPS_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_AMP_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_AMP_P_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_SCC_SW_MISMATCH, "", "", 0, false, false},
    {CARD_ALM_OCS_ABSENT, "", "", 0, false, false},
    {CARD_ALM_OPS_ABSENT, "", "", 0, false, false},
    {CARD_ALM_AMP_ABSENT, "", "", 0, false, false},
    {CARD_ALM_AMP_P_ABSENT, "", "", 0, false, false},
    /*{CARD_ALM_OPS_FAIL, "", "", 0, false, false},
       {CARD_ALM_AMP_FAIL, "", "", 0, false, false},
       {CARD_ALM_AMP_P_FAIL, "", "", 0, false, false},*/
    /* For multiple OPS card */
    {UPGRADE_ALM_OPS1_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_OPS2_SW_MISMATCH, "", "", 0, false, false},
    {UPGRADE_ALM_OPS3_SW_MISMATCH, "", "", 0, false, false},
    {CARD_ALM_OPS1_ABSENT, "", "", 0, false, false},
    {CARD_ALM_OPS2_ABSENT, "", "", 0, false, false},
    {CARD_ALM_OPS3_ABSENT, "", "", 0, false, false}
};

static ALM_UNIT_ST m_astRstAlm[RESET_ALM_NUM] = {
    {RESET_ALM_WARM, "", "", 0, false, false},
    {RESET_ALM_COLD, "", "", 0, false, false},
};

static ALM_UNIT_ST m_astSftpIpAlm[SFTP_IP_UNREACHABLE_ALM_NUM] = {
    {SFTP_IP_UNREACHABLE, "", "", 0, false, false},
};

static size_t m_nAlarmGroup = 0;

int
OPERATIOND_SetUpgdAlm(char *pcAlmStr, bool bAlm)
{
    int i = 0;

    for (i = 0; i < sizeof(m_astUpgdAlm) / sizeof(ALM_UNIT_ST); i++) {
        if (0 == strcmp(pcAlmStr, m_astUpgdAlm[i].acAlmStr)) {
            m_astUpgdAlm[i].bCurAlm = bAlm;
            ALM_Set(m_astUpgdAlm, sizeof(m_astUpgdAlm) / sizeof(ALM_UNIT_ST));
            break;
        }
    }

    return 0;
}

int
OPERATIOND_SetRstAlm(char *pcAlmStr, bool bAlm)
{
    int i = 0;

    for (i = 0; i < sizeof(m_astRstAlm) / sizeof(ALM_UNIT_ST); i++) {
        if (0 == strcmp(pcAlmStr, m_astRstAlm[i].acAlmStr)) {
            m_astRstAlm[i].bCurAlm = bAlm;
            ALM_Set(m_astRstAlm, sizeof(m_astRstAlm) / sizeof(ALM_UNIT_ST));
            break;
        }
    }

    return 0;
}

int
OPERATIOND_SetSftpIpAlm(char *pcAlmStr, bool bAlm)
{
    int i = 0;

    for (i = 0; i < sizeof(m_astSftpIpAlm) / sizeof(ALM_UNIT_ST); i++) {
        if (0 == strcmp(pcAlmStr, m_astSftpIpAlm[i].acAlmStr)) {
            m_astSftpIpAlm[i].bCurAlm = bAlm;
            ALM_Set(m_astSftpIpAlm, sizeof(m_astSftpIpAlm) / sizeof(ALM_UNIT_ST));
            break;
        }
    }

    return 0;
}

int
OPERATIOND_AlmSub(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess)
{
    int rc = 0;

    LOG_WRITE(EN_LOG_NOTICE, "++ OPERATIOND_AlmSub");

    if (NULL == pstStartupSess || NULL == pstRunningSess) {
        goto cleanup;
    }

    /* init upgd alarm */
    ALM_Init(pstStartupSess, pstRunningSess, m_astUpgdAlm, sizeof(m_astUpgdAlm) / sizeof(ALM_UNIT_ST));
    ALM_Init(pstStartupSess, pstRunningSess, m_astRstAlm, sizeof(m_astRstAlm) / sizeof(ALM_UNIT_ST));
    ALM_Init(pstStartupSess, pstRunningSess, m_astSftpIpAlm, sizeof(m_astSftpIpAlm) / sizeof(ALM_UNIT_ST));

    m_nAlarmGroup = BOARD_GetProductType();

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "-- OPERATIOND_AlmSub");
    return rc;
}

int
OPERATIOND_AlmUnSub(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess)
{

    int rc = 0;

    ALM_Free();

    return rc;
}

int
OPERATIOND_SetSlotAlm(int iSlotId, CARD_ALM_TYPE_EN enCardAlmType, bool bAlm)
{
    static const SLOT_ALM_TABLE_ST m_astSlotAlmTbl[EN_PRODUCT_TYPE_MAX][ALM_TABLE_UNIT_SIZE] = {

        /* OLS-P */
        {
            {DSC_SLOT_4, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_SCC_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OPS_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_ABSENT, CARD_ALM_OPS_ABSENT},
            {DSC_SLOT_1, EN_CARD_ALM_FAIL, CARD_ALM_OPS_FAIL},
            {DSC_SLOT_2, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_AMP_SW_MISMATCH},
            {DSC_SLOT_2, EN_CARD_ALM_ABSENT, CARD_ALM_AMP_ABSENT},
            {DSC_SLOT_2, EN_CARD_ALM_FAIL, CARD_ALM_AMP_FAIL},
            {DSC_SLOT_3, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_AMP_P_SW_MISMATCH},
            {DSC_SLOT_3, EN_CARD_ALM_ABSENT, CARD_ALM_AMP_P_ABSENT},
            {DSC_SLOT_3, EN_CARD_ALM_FAIL, CARD_ALM_AMP_P_FAIL}
        },
        /* NR1004 AMP */
        {
            {DSC_SLOT_4, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_SCC_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OPS_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_ABSENT, CARD_ALM_OPS_ABSENT},
            {DSC_SLOT_1, EN_CARD_ALM_FAIL, CARD_ALM_OPS_FAIL},
            {DSC_SLOT_2, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_AMP_SW_MISMATCH},
            {DSC_SLOT_2, EN_CARD_ALM_ABSENT, CARD_ALM_AMP_ABSENT},
            {DSC_SLOT_2, EN_CARD_ALM_FAIL, CARD_ALM_AMP_FAIL},
            {DSC_SLOT_3, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_AMP_P_SW_MISMATCH},
            {DSC_SLOT_3, EN_CARD_ALM_ABSENT, CARD_ALM_AMP_P_ABSENT},
            {DSC_SLOT_3, EN_CARD_ALM_FAIL, CARD_ALM_AMP_P_FAIL}
        },
        /* NR1004 OPS */
        {
            {DSC_SLOT_4, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_SCC_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OPS1_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_ABSENT, CARD_ALM_OPS1_ABSENT},
            {DSC_SLOT_1, EN_CARD_ALM_FAIL, CARD_ALM_OPS1_FAIL},
            {DSC_SLOT_2, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OPS2_SW_MISMATCH},
            {DSC_SLOT_2, EN_CARD_ALM_ABSENT, CARD_ALM_OPS2_ABSENT},
            {DSC_SLOT_2, EN_CARD_ALM_FAIL, CARD_ALM_OPS2_FAIL},
            {DSC_SLOT_3, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OPS3_SW_MISMATCH},
            {DSC_SLOT_3, EN_CARD_ALM_ABSENT, CARD_ALM_OPS3_ABSENT},
            {DSC_SLOT_3, EN_CARD_ALM_FAIL, CARD_ALM_OPS3_FAIL}
        },
        /* OLS-HP or HCF */
        {
            {DSC_SLOT_4, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_SCC_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_AMP_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_ABSENT, CARD_ALM_AMP_ABSENT},
            {DSC_SLOT_1, EN_CARD_ALM_FAIL, CARD_ALM_AMP_FAIL}
        },
        /* OCS */
        {
            {DSC_SLOT_4, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_SCC_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_SW_MISMATCH, UPGRADE_ALM_OCS_SW_MISMATCH},
            {DSC_SLOT_1, EN_CARD_ALM_ABSENT, CARD_ALM_OCS_ABSENT},
            {DSC_SLOT_1, EN_CARD_ALM_FAIL, CARD_ALM_OCS_FAIL}
        }
    };

    const SLOT_ALM_TABLE_ST *table = m_astSlotAlmTbl[m_nAlarmGroup];
    size_t nCnt = ALM_TABLE_UNIT_SIZE;
    size_t i = 0;

    for (i = 0; i < nCnt; i++) {
        if ((DSC_SLOT_MIN == table[i].iSlotId) || (NULL == table[i].pcAlmStr)) {
            continue;
        }

        if ((iSlotId == table[i].iSlotId) && (enCardAlmType == table[i].enCardAlmType)) {

            break;
        }
    }

    if (i >= nCnt) {
        LOG_WRITE(EN_LOG_ERROR, "NOT found! iSlotId: %d, enCardAlmType: %d, bAlm: %d", iSlotId, enCardAlmType, bAlm);
        return ERR_NOT_FOUND;
    }

    OPERATIOND_SetUpgdAlm(table[i].pcAlmStr, bAlm);

    return OPLK_OK;
}

