/*
 * CmdTask.h
 */

#ifndef _CMD_TASK_H
#define _CMD_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#define NONE_QUOT       0
#define LEFT_QUOT       1
#define RIGHT_QUOT      2
#define PROMPT_LEN      32
#define PROMPT_DEF      ""
#define TOUT_VALUE      300

extern CHAR *CmdPrompt;

extern ULONG m_MaxSession;
extern ULONG m_CmdInitilized;

extern ULONG CMD_Init(ULONG ulClient);
extern ULONG CMD_EnterEnable(char *pcBuf, int Len, char **argv, int argc);
extern VOID CMD_TimerEntry(ULONG *ulTime);
extern VOID CMD_Entry(ULONG ulSes, ULONG ulIFd, ULONG ulOFd);

extern ULONG tickGet(void);
extern void taskDelay(ULONG ulDelay);

#ifdef __cplusplus
}
#endif

#endif

