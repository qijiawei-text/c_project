/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_reset.h
   Description : reset
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/

#ifndef _OPERATIOND_RESET_H
#define _OPERATIOND_RESET_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t usReg;
    uint8_t ucState;
    uint8_t ucRsv;
} ST_RPC_BUTTON_CTRL;

enum ERpcButtonFunc
{
    BT_FUNC_WARM,
    BT_FUNC_COLD,
    BT_FUNC_RTFD,
    BT_FUNC_MAX,
};

enum ERpcButtonReg
{
    BT_REG_WARM = 0x10A4,
    BT_REG_COLD = 0x10A5,
    BT_REG_RTFD = 0x10A6,
    BT_REG_MAX,
};

extern int Reset_RegisterRpc(sr_session_ctx_t *pstSess);
extern void Reset_Process(uint32_t ulSlotNum, CSlotInfo *pstSlotInfo, bool *pbNeedConfig, uint8_t *pucResetType);


#ifdef __cplusplus
}
#endif

#endif

