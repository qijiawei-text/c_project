/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : operationd_alm.h
   Description : operationd alarm report.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef _OPERATIOND_ALM_H
#define _OPERATIOND_ALM_H

#ifdef __cplusplus
extern "C" {
#endif

#define UPGRADE_ALM_FAILED                  "TOA_SOFTWARE_UPGRADE_FAIL"
#define UPGRADE_ALM_ACTIVE                  "TOA_SOFTWARE_UPGRADE_ACTIVE"
#define UPGRADE_ALM_IN_PROGRESS             "TOA_SOFTWARE_UPGRADE_IN_PROGRESS"

#define UPGRADE_ALM_OCS_SW_MISMATCH         "OCS_CARD_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_OPS_SW_MISMATCH         "OPS_CARD_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_AMP_SW_MISMATCH         "AMP_CARD_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_AMP_P_SW_MISMATCH       "AMP-P_CARD_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_SCC_SW_MISMATCH         "SCC_CARD_SOFTWARE_MISMATCH"

#define CARD_ALM_OCS_ABSENT                 "OCS_CARD_ABSENT"
#define CARD_ALM_OPS_ABSENT                 "OPS_CARD_ABSENT"
#define CARD_ALM_AMP_ABSENT                 "AMP_CARD_ABSENT"
#define CARD_ALM_AMP_P_ABSENT               "AMP-P_CARD_ABSENT"

#define CARD_ALM_OCS_FAIL                   "OCS_CARD_FAIL"
#define CARD_ALM_OPS_FAIL                   "OPS_CARD_FAIL"
#define CARD_ALM_AMP_FAIL                   "AMP_CARD_FAIL"
#define CARD_ALM_AMP_P_FAIL                 "AMP-P_CARD_FAIL"

#define RESET_ALM_WARM                      "TOA_SYSTEM_BOOT_WARM"         /* NOT used in OLS-P */
#define RESET_ALM_COLD                      "TOA_SYSTEM_BOOT_COLD"         /* NOT used in OLS-P */

#define SFTP_IP_UNREACHABLE                 "TOA_SFTP_SERVER_UNREACHABLE"  /* NOT used in OLS-P */

/* Alarms for multiple OPS card */
#define UPGRADE_ALM_OPS1_SW_MISMATCH        "OPS_CARD1_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_OPS2_SW_MISMATCH        "OPS_CARD2_SOFTWARE_MISMATCH"
#define UPGRADE_ALM_OPS3_SW_MISMATCH        "OPS_CARD3_SOFTWARE_MISMATCH"
#define CARD_ALM_OPS1_ABSENT                "OPS_CARD1_ABSENT"
#define CARD_ALM_OPS2_ABSENT                "OPS_CARD2_ABSENT"
#define CARD_ALM_OPS3_ABSENT                "OPS_CARD3_ABSENT"
#define CARD_ALM_OPS1_FAIL                  "OPS_CARD1_FAIL"
#define CARD_ALM_OPS2_FAIL                  "OPS_CARD2_FAIL"
#define CARD_ALM_OPS3_FAIL                  "OPS_CARD3_FAIL"


typedef enum {
    EN_CARD_ALM_SW_MISMATCH = 0,
    EN_CARD_ALM_ABSENT,
    EN_CARD_ALM_FAIL,
    EN_CARD_ALM_MAX
} CARD_ALM_TYPE_EN;
typedef struct {
    int iSlotId;                         /* refer to DSC_SLOT_NO_EN */
    CARD_ALM_TYPE_EN enCardAlmType;
    char *pcAlmStr;                      /* define in com-oplink-alarm */
} SLOT_ALM_TABLE_ST;

int OPERATIOND_SetUpgdAlm(char *pcAlmStr, bool bAlm);
int OPERATIOND_SetRstAlm(char *pcAlmStr, bool bAlm);
int OPERATIOND_SetSftpIpAlm(char *pcAlmStr, bool bAlm);
int OPERATIOND_AlmSub(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess);
int OPERATIOND_AlmUnSub(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess);

int OPERATIOND_SetSlotAlm(int iSlotId, CARD_ALM_TYPE_EN enCardAlmType, bool bAlm);

#ifdef __cplusplus
}
#endif

#endif

