/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_config_mfg.h
   Description : slot config and mfg
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef _OPERATIOND_CONFIG_MFG_H
#define _OPERATIOND_CONFIG_MFG_H

#ifdef __cplusplus
extern "C" {
#endif

extern int Slot_Config(uint32_t ulSlotIdx);
extern int Slot_ConfigModule(uint32_t ulSlotIdx, uint32_t uiDevType);
extern int Slot_PutPlugableDeviceMfgToOrgAndOc(int iSlotId, int iSlotStatus, CSlotInfo *pstSlotInfo);
extern void Slot_Process(uint32_t ulSlotNum, CSlotInfo *pstSlotInfo, bool bNeedConfig);
extern int Slot_HalEventCb(int iEventId, int iSubEventId);


#ifdef __cplusplus
}
#endif

#endif

