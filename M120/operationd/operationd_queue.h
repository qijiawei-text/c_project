/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_queue.h
   Description : util queue
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef __OPERATIOND_QUEUE_H__
#define __OPERATIOND_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif


#define OID_LAMP_TEST              (0x00001001)
#define OID_SAVE_OPS_MODE          (0x00001002)
#define OID_START_UPGD             (0x00001003)
#define OID_ONEKEY_UPGD            (0x00001004)
#define OID_DOWNLOAD_IMAGE         (0x00001005)
#define OID_CARD_RESET             (0x00001006)


extern bool OPERATIOND_GetLamptestFlag(void);
extern int8_t OPERATIOND_GetQueueIdx(void);
extern void OPERATIOND_StartQueueThread();
extern void OPERATIOND_QueueExit();
extern bool OPERATIOND_GetCardRestFlag(uint32_t uiSlotIdx);
extern void OPERATIOND_SetCardRestFlag(uint32_t uiSlotIdx, bool bFlag);



#ifdef __cplusplus
}
#endif
#endif
