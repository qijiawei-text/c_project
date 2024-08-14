/***MODU+*********************************************************************/
/* Copyright(C), 2015, OPLINK Tech.Co.,Ltd                                   */
/* FileName    : oplk_datatype.h                                             */
/* Description : The data type define.                                       */
/* History     :                                                             */
/*     [Author]   [Date]      [Version]    [Description]                     */
/* [1]    FeiTu   2015/04/22  Ver 1.0.0    Initial file.                     */
/***MODU-*********************************************************************/

#ifndef __OPLK_DATATYPE_H__
#define __OPLK_DATATYPE_H__

/***TYP+**********************************************************************/
/* Data type redefinition.                                                   */
/***DEF-**********************************************************************/
typedef float FLOAT;
typedef unsigned long U64;
typedef long S64;
typedef unsigned int *PDWORD, DWORD, ULONG, U32, UINT;
typedef int *PLONG, LONG, S32, INT;
typedef unsigned short *PWORD, WORD, U16, USHORT;
typedef short S16, SHORT;
typedef unsigned char *PBYTE, BYTE, U8, UCHAR, BOOL;
typedef char *PSCHAR, S8, CHAR;
typedef void VOID;

/***DEF+**********************************************************************/
/* define NULL UCHAR/USHORT/ULONG value                                      */
/***DEF-**********************************************************************/
#define NULL_BYTE            (0xFF)
#define NULL_WORD            (0xFFFF)
#define NULL_DWORD           (0xFFFFFFFF)

/***DEF+**********************************************************************/
/* define OK or ERR.                                                         */
/***DEF-**********************************************************************/
#define NULL_PTR             (0)
#define VAL_OK               (1)
#define VAL_ERR              (0)

#define NULL                 ((void *)0)

/***DEF+**********************************************************************/
/* Define max or min value.                                                  */
/***DEF-**********************************************************************/
#define MAX(a, b)                        ((a) > (b) ? (a) : (b))
#define MIN(a, b)                        ((a) < (b) ? (a) : (b))

/***DEF+**********************************************************************/
/* Kinetis is little endian ARCH,                                            */
/* Modify from the big-endian to little-endian                               */
/***DEF-**********************************************************************/
#define TRUE    (1 == 1)
#define FALSE   (!TRUE)

#define OPLK_FALSE  0
#define OPLK_TRUE   1

#define OPLK_ERROR  -1
#define OPLK_OK     0


#define OK    (0)

#define OPLK_LOG_PRINT_ERR printf

#define TIME_1MS                        (1000 * 1)
#define TIME_2MS                        (1000 * 2)
#define TIME_5MS                        (1000 * 5)
#define TIME_10MS                       (1000 * 10)
#define TIME_20MS                       (1000 * 20)
#define TIME_30MS                       (1000 * 30)
#define TIME_40MS                       (1000 * 40)
#define TIME_50MS                       (1000 * 50)
#define TIME_100MS                      (1000 * 100)
#define TIME_200MS                      (1000 * 200)
#define TIME_440MS                      (1000 * 440)
#define TIME_500MS                      (1000 * 500)
#define TIME_1S                         (1)
#define TIME_2S                         (2)
#define TIME_3S                         (3)
#define TIME_5S                         (5)
#define TIME_6S                         (6)
#define TIME_10S                        (10)
#define TIME_15S                        (15)
#define TIME_30S                        (30)
#define TIME_60S                        (60)
#define TIME_120S                       (120)

#define SAFE_FREE(ptr)        do {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}} while (0);
#define SAFE_FCLOSE(ptr)      do {if (NULL != (ptr)) {fclose(ptr); (ptr) = NULL;}} while (0);
#define SAFE_CLOSE(fd)        do {if ((fd) >= 0) {close(fd); (fd) = -1;}} while (0);
#define SAFE_PCLOSE(ptr)      do {if (ptr != NULL) {pclose(ptr); (ptr) = NULL;}} while (0)


/* error code */
#define ERR_GENERAL                (-1)
#define ERR_PARA                   (-2)
#define ERR_PARA_NUM               (-3)
#define ERR_FILE_CAN_NOT_CREATE    (-4)
#define ERR_UART_CAN_NOT_OPEN      (-5)
#define ERR_UART_CFG               (-6)
#define ERR_UART_OPEN              (-7)
#define ERR_UART_SET               (-8)
#define ERR_UART_RECV              (-9)
#define ERR_UART_SEND              (-10)
#define ERR_UART_PIN_CFG           (-11)
#define ERR_MEM_OPEN               (-12)
#define ERR_MEM_MAP                (-13)
#define ERR_IO_MEM_MAP             (-14)

#endif   /* __OPLK_DATATYPE_H__ */
