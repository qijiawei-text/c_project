/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_crc.c
* Description:   CRC16 and CRC32
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_CRC_H__
#define __UTIL_CRC_H__

#include "util_internalinc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t UTIL_Crc16CalcWithFeed(uint32_t ulFeed, uint8_t *pucBuf,
                                       uint32_t ulLen);
extern uint16_t UTIL_Crc16Calc(uint8_t *pucBuf, uint32_t ulLen);
extern uint32_t UTIL_Crc32CalcWithFeed(uint32_t ulFeed, uint8_t *pucBuf,
                                       uint32_t ulLen);
extern uint32_t UTIL_Crc32Calc(uint8_t *pucBuf, uint32_t ulLen);


#ifdef __cplusplus
}
#endif

#endif
