/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_file.h
   Description : file upload and download.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef _OPERATIOND_FILE_H
#define _OPERATIOND_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FILE_TRANSFER_SUCCESS,
    FILE_TRANSFER_FAIL,
    FILE_TRANSFER_CNT
} FILE_TRANSFER_STATUS_E;

typedef enum {
    DB_ACTIVATE,
    DB_COMMIT,
    DB_CANCEL
} DB_ACTIVATE_NOTICE_E;


extern int File_RegisterRpc(sr_session_ctx_t *pstSess);
extern int Operationd_GetNodeId(char *pcNodeId);
extern int Db_Init(void);

extern int __ReplaceValue(const char *pcNode, char *pcOldValue, char *pcNewValue);
extern int __RestoreMfg();
extern int __InstallYang();
extern int __SaveCfg(int iId, const char *pcXpath);
extern int __LoadCfg();
extern int __RestoreCfg();
extern int __DefCfg();
extern int __UseDefCfg();
extern int __SetMfgDefault(int32_t iCfgType);
extern int RestoreCfg_Exit();

#ifdef __cplusplus
}
#endif

#endif

