/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_aes.h
* Description:   aes cbc 256 algorithm
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/

#ifndef __UTIL_AES_H__
#define __UTIL_AES_H__

#include "util_internalinc.h"

#ifdef  __cplusplus
extern "C" {
#endif

extern void UTIL_AesCbc256InitKey(void);
extern void UTIL_AesCbc256Encrypt(const uint8_t *pucInBuf, uint32_t ulLen, uint8_t *pucOutBuf);
extern void UTIL_AesCbc256Decrypt(const uint8_t *pucInBuf, uint32_t ulLen, uint8_t *pucOutBuf);

#ifdef  __cplusplus
}
#endif

#endif
