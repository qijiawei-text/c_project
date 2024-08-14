/***MODU+******************************************************************************************/
/* Copyright(C), 2014, OPLINK Tech.Co.,Ltd                                                        */
/* FileName    : sdv_uart.h                                                                       */
/* Description : The header file of sdv_uart.h.                                                   */
/* History     :                                                                                  */
/*     [Author]          [Date]          [Version]        [Description]                           */
/* [1] fandy             2014/05/08      Ver 1.0.0        Initial file.                           */
/***MODU-******************************************************************************************/

#ifndef __SDV_UART_H__
#define __SDV_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "oplk_datatype.h"

#define UART_READ_MAX_LEN               (128)

#define UART_BAUDRATE_9600              (9600)
#define UART_BAUDRATE_19200             (19200)
#define UART_BAUDRATE_38400             (38400)
#define UART_BAUDRATE_57600             (57600)
#define UART_BAUDRATE_115200            (115200)

#define UART_SFLOW_DISABLE            0
#define UART_SFLOW_ENABLE             1


#if 0
#define UART_DATA_BIT5                  5
#define UART_DATA_BIT6                  6
#define UART_DATA_BIT7                  7
#define UART_DATA_BIT8                  8

#define UART_PARITY_DISABLE             0
#define UART_PARITY_ODD                 1
#define UART_PARITY_EVEN                2
#endif

#define UART_STOP_BIT1                  1
#define UART_STOP_BIT2                  2

#define UART_NAME_LEN                   (32)
#define UART_TTY2                       ("/dev/ttyS2")

#define UART_TCFLUSH(fd)                {tcflush((fd), TCIFLUSH); tcflush((fd), TCOFLUSH); }
#define UART_TCDRAIN(fd)                {tcdrain((fd)); }

/**************************************************************************************************/
/* @defgroup sdv_uart_Exported_Functions                                                          */
/**************************************************************************************************/
extern INT SDV_Uart0Write(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry);
extern INT SDV_Uart0Read(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry);
extern INT SDV_Uart0SetBaud(ULONG ulBaud);
extern INT SDV_Uart0Init(VOID);
extern INT SDV_Uart0Close(VOID);

extern INT SDV_Uart0Lock(VOID);
extern INT SDV_Uart0UnLock(VOID);

#ifdef __cplusplus
}
#endif

#endif

