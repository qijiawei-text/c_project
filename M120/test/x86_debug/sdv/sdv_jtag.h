/***MODU+******************************************************************************************/
/* Copyright(C), 2014, OPLINK Tech.Co.,Ltd                                                        */
/* FileName    : sdv_jtag.h                                                                       */
/* Description : The header file of sdv_jtag.h.                                                   */
/* History     :                                                                                  */
/*     [Author]          [Date]          [Version]        [Description]                           */
/* [1] YuanzhengLi       2014/06/03      Ver 1.0.0        Initial file.                           */
/***MODU-******************************************************************************************/

#ifndef __SDV_JTAG_H__
#define __SDV_JTAG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "oplk_datatype.h"


/**
 * @defgroup  CPLD Index
 * @{
 */
typedef enum
{
    CPLD0_IDX = 0,
    CPLD1_IDX,

    CPLD_IDX_MAX

} EN_CPLD_IDX;
/**
 * @}
 */

/**
 * @defgroup  JTAG PINs Definition (Compatible with the definition in Hardware.c)
 * @{
 */
#define JTAG_PIN_TDI                    0x01
#define JTAG_PIN_TCK                    0x02
#define JTAG_PIN_TMS                    0x04
#define JTAG_PIN_TDO                    0x40
/**
 * @}
 */

/**************************************************************************************************/
/* @defgroup sdv_jtag_Exported_Functions                                                          */
/**************************************************************************************************/

extern INT SDV_JtagWritePin(UCHAR ucPin, UCHAR ucValue);
extern INT SDV_JtagReadTDO(UCHAR *pucValue);
extern INT SDV_JtagGetImg(UCHAR *pucData);

extern INT SDV_JtagCpld(UCHAR ucCpldIdx, UCHAR *pucImg, ULONG ulLen);
extern INT SDV_JtagCpldReset(UCHAR ucCpldIdx);

#ifdef __cplusplus
}
#endif

#endif
