/*
 * CmdCommon.h
 */

#ifndef _CMD_COMMON_H
#define _CMD_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_LEN         256
#define EBUF_LEN         256
#define HIST_NUM         10
#define TREE_DEPTH       20
#define PARA_NUM         20
#define UNAME_LEN        19
#define PWORD_LEN        19
#define PBUF_LEN         0x8000

#define CONF_FILE_SIZE   0x8000
#define MAX_FILE_CMD     0x1D00
#define MAX_IF_CMD       0x20

#define PAGE_SCROLL      0xAAAA
#define LINE_SCROLL      0xBBBB
#define NONE_SCROLL      0xCCCC

#define MAX_SAME_TOKEN   100
#define TAB_DISP_NUM     5

#define SCR_ROWS         21
#define SCR_COLS         256

#define CONFIG_PH        0x11
#define INTERF_PH        0x22
#define INTILI_PH        0x33

#define PRES_ARG 3

/***ENM+**********************************************************************/
/* The index of parameters of command.                                       */
/***ENM-**********************************************************************/
typedef enum {
    CMD_PARA_IDX0 = 0,   /* The 1st parameters of command. */
    CMD_PARA_IDX1,       /* The 2nd parameters of command. */
    CMD_PARA_IDX2,       /* The 3rd parameters of command. */
    CMD_PARA_IDX3,       /* The 4th parameters of command. */
    CMD_PARA_IDX4,       /* The 5th parameters of command. */
    CMD_PARA_IDX_CNT
} CMD_PARA_IDX_EN;

#define CMD_ASSERT(flag, string) if(flag) printf(string)
#define IS_BACKSPACE(c) (((c) == '\b')   ? 1 : 0)
#define IS_TAB(c)       (((c) == '\t')   ? 1 : 0)
#define IS_DELETE(c)    (((c) == 127)    ? 1 : 0)

#define CHAR2NUM(c)  (c - 0x30)
#define IS_NUMBER(c) ((c >= '0' && c <= '9') ? 1 : 0)

#define CMD_STRCMP      CMD_StringCmp

#define CMD_WRITE_CONFIG(pcString, flag) CMD_WriteCmdToFile((pcString), strlen(pcString), (flag), 0)

#ifdef  _DEBUG_OPEN       
#define CMD_LOG_LEN                      (0x00000100)
#define CMD_DEBUG_LOG_FILE          ("/var/cmd_debug.log")

#define CMD_DEBUG_PRINT(strInfo, args...)                         \
{                                                                                      \
    CMD_DebugPrintf(__FILE__, __LINE__, strInfo, ##args);     \
}

#define CMD_DEBUG_PRINT_NORETURN(strInfo, args...)         \
{                                                                                       \
    printf(strInfo, ##args);                                                   \
}

VOID CMD_DebugOpen();

#else
#define CMD_DEBUG_PRINT(strInfo, args...) 
#define CMD_DEBUG_PRINT_NORETURN(strInfo, args...) 

#endif 

typedef struct cmd_base_info
{
    ULONG  Session;
    ULONG  ulIFd;
    ULONG  ulOFd;
    CHAR   acRBuf[EBUF_LEN];
    ULONG  ulRLen;
    CHAR   acSBuf[EBUF_LEN];
    ULONG  ulSLen;
    VOID   (*cmd_send)(ULONG ulFd, CHAR *cBuf, ULONG ulLen);
    ULONG  (*cmd_recv)(ULONG ulFd, CHAR *cBuf, ULONG ulLen);
} CMD_Base;

typedef struct mode_info
{
    UINT8   ucOne;
    UINT16  usTwo;
    UINT8   ucThree;
    ULONG   ulPorts;
} CMD_Mode;

typedef struct cmd_ctrl_info
{
    CMD_Mode CurMode;
    ULONG    UserLevel;
    CMD_Base BaseIo;
    CHAR     History[HIST_NUM][DISP_LEN];
    ULONG    ulHisNums;
    ULONG    ulHisHead;
    ULONG    ulHisTail;
    ULONG    ulCurHisPos;
    CHAR     acPBuf[PBUF_LEN];
    CHAR     acEBuf[DISP_LEN];
    ULONG    ulELen;
    ULONG    ulESC;
    ULONG    TreeDep;
    CHAR    *CmdTree[TREE_DEPTH];

    ULONG    ulParaBLen;
    CHAR     ParaBuf[DISP_LEN];
    ULONG    ParaNum;
    CHAR    *CurPara[PARA_NUM];

    USHORT   usPing;
    USHORT   usString;
    ULONG    ulLogin;
    ULONG    ulTrig;
    ULONG    ulPromLen;
    ULONG    ulLines;
    ULONG    ulScroll;
    ULONG    ulQuit;
} CMD_Ctrl;

typedef struct _cmd_DefIfCon
{
    char *DefCmd[MAX_IF_CMD];
    ULONG CmdNum;
} CMD_IFProm;

extern CMD_Ctrl *CmdCtrl;

extern ULONG     m_CmdInitilized;

extern ULONG CMD_StrToInt(CHAR *pcSourcr, ULONG ulLen, ULONG *pulNum);
extern ULONG CMD_MathLog2(ULONG x);
extern VOID  CMD_WelComeInfo(ULONG ulSession);
extern VOID  CMD_OutputChar(ULONG ulSession, CHAR cChar);
extern VOID  CMD_OutputStr(ULONG ulSession, CHAR *pcString);
extern VOID  CMD_OutputBuf(ULONG ulSession, CHAR *Buf);
extern VOID  CMD_ClearScreen(ULONG ulSession);
extern VOID  CMD_DeletePrevChar(ULONG ulSession);
extern VOID  CMD_ClearParameter(ULONG ulSession);
extern VOID  CMD_ClearCurPrompt(ULONG ulSession);
extern VOID  CMD_ClearTreeInfo(ULONG ulSession);
extern VOID  CMD_ClearRecvBuf(ULONG ulSession);
extern VOID  CMD_HisNewPrompt(ULONG ulSession);
extern VOID  CMD_PrevHisRecord(ULONG ulSession);
extern VOID  CMD_NextHisRecord(ULONG ulSession);
extern VOID  CMD_AddParameter(ULONG ulSession, CHAR *pcArg);
extern VOID  CMD_AddPrestorePara(ULONG ulSession);
extern VOID  CMD_RebuildEditBuf(ULONG ulSession);
extern ULONG CMD_StringCmp(CHAR *pSrc, CHAR * pDet);
extern VOID  CMD_GetScrollMode(ULONG ulSession);
extern VOID  CMD_StartPing(ULONG ulSession, ULONG ulTaskId);
extern VOID  CMD_StopPing(ULONG ulSession);
extern ULONG CMD_Sprintf(char **buf, char *fmt, ...);
extern VOID  CMD_PrintfStr(ULONG ulSession, CHAR *pcString, ...);
extern ULONG CMD_ConfirmQuery(ULONG ulSes);
extern ULONG CMD_GetVarLimit(CHAR *pcLimit, ULONG *pulDLimit, ULONG *pulSLimit);
extern VOID  CMD_DispNextTips(ULONG ulSession);
extern unsigned long crc32(unsigned long crc, unsigned char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif

