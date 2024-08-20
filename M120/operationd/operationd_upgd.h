/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_upgd.h
   Description : upgrade
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef _OPERATIOND_UPGD_H
#define _OPERATIOND_UPGD_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEF_FEATURE_AUTO_UPGRADE_ENABLE        (0)

typedef struct
{
    int iSlotId;
    bool bNeedConfig;
} AUTO_UPGD_SLOT_INFO_ST;


extern int CreateAutoUpgdThread(AUTO_UPGD_SLOT_INFO_ST *pstAutoUpgdSlotInfo);
extern int StopAutoUpgdThread(int iSlotId);
extern int Upgd_OperDataSub(sr_session_ctx_t *pstSess);
extern int Upgd_RegisterRpc(sr_session_ctx_t *pstSess);

extern void Upgd_Process();


#ifdef __cplusplus
}
#endif

#endif

