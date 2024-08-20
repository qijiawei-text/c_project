/*
 * CmdTypes.h
 */
#ifndef __CMD_TYPES_H
#define __CMD_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>

typedef int             BOOL;
typedef void            VOID;
typedef long            LRESULT;
typedef int             FILE_DESCRIPTOR,INT;
typedef long            LONG, *PLONG;
typedef short           SHORT, *PSHORT;
typedef char            CHAR, *PCHAR, *LPSTR;
typedef unsigned char   BYTE, *PBYTE, UINT8, UCHAR;
typedef unsigned int    UINT;
typedef unsigned long   DWORD, *PDWORD, ULONG, ulong_t;
typedef unsigned short  WORD, *PWORD, USHORT, UINT16;
typedef float           FLOAT;


#ifndef NULL
#define NULL    0L
#endif

#ifndef TRUE
#define TRUE      1
#endif
#ifndef FALSE
#define FALSE     0
#endif

#define CMD_OK    0
#define CMD_ERROR 0xFFFFFFFF

#ifndef NULL_LONG
#define NULL_LONG 0xFFFFFFFF
#endif
#ifndef NULL_WORD
#define NULL_WORD 0xFFFF
#endif
#ifndef NULL_BYTE
#define NULL_BYTE 0xFF
#endif

#define OK        0
#define ERROR     -1

#define MANU_INFO_FILE   "/etc/MfgInfo"

#define _CLI_PASSWORS   "oplkadmin"

#define _CLI_WELCOME_INFO_  "\r\n\n\t\t\t\t\tWelcome\n"

#define CLI_MAX_TELNET  0
#define CLI_CONSOLE_ID  0x00

#define USER_TYPE_OFFSET    0x30
#define UGUEST     (USER_TYPE_OFFSET+1)
#define UNORMAL    (USER_TYPE_OFFSET+2)
#define UADMIN     (USER_TYPE_OFFSET+3)
#define UPOWER     (USER_TYPE_OFFSET+4)

#ifdef  ASB_DEBUG
#define CMD_DEBUG
#endif
#ifdef  CMD_DEBUG
#define CMD_PRINTF       printf
#else
#define CMD_PRINTF
#endif

#ifdef __cplusplus
}
#endif
#endif

