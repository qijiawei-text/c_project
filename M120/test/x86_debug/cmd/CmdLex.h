/*
 * CmdLex.h
 */

#ifndef _CMD_LEX_H
#define _CMD_LEX_H

#ifdef __cplusplus
extern "C" {
#endif

#define SORT_NUM     100

typedef enum Prompt_State
{
    enUncompleted,
    enUnknowed,
    enExecutable,
    enHelpCommand,
    enUnwantedToken,
    enCommandEnd,
    enWaitingExtend,
    enParameterError,
    enCmdButt
} Promt_Enum;

#define cmd_issuper(ch)  (((ch) >= 'A' && (ch) <= 'Z') ? 1 : 0)
#define cmd_tolower(ch)  (cmd_issuper(ch) ? ((ch) + 32) : (ch))

extern VOID  CMD_LexProcess(ULONG ulSession);
extern VOID CMD_SubmitCommand(ULONG ulSes);
extern ULONG CMD_IsNormalChar(CHAR cChar);

#ifdef __cplusplus
}
#endif

#endif

