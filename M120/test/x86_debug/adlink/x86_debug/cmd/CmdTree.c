/*
 * CmdTree.c
 */

#include "CmdTypes.h"
#include "CmdCommon.h"
#include "CmdTask.h"
#include "CmdTree.h"
#include "CmdLex.h"


#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

static const CHAR *m_pacOnePrompt[] =
{
    ">",
    "->",
    ")#",
    "(debug)#",
};

static const CHAR *m_pacTwoPrompt[] =
{
    "(config",
    "(config-if",
    "(config-rip",
    "(config-ospf",
};

static const CHAR *m_pacThreePrompt[] =
{
    "",
    "-ethernet",
    "-slot",
    "-vlan",
    "-ethernet",
};

CMD_CmdInfo CmdRoot;

extern struct termios g_stTermios;


CMD_Node *
LIST_First(CMD_List *pList)
{
    return pList->NODE_HEAD;
}

VOID
LIST_Insert(CMD_List *pList, CMD_Node *pNode)
{
    CMD_Node *p = (CMD_Node *)pList;

    while (p->pstNext != NULL) {
        p = p->pstNext;
    }

    p->pstNext = pNode;
    pNode->pstPrev = p;
    pList->ulCount++;
}

CMD_Node *
LIST_Walk(CMD_List *pList, KEY_STRU stKey, int compare(void *, KEY_STRU))
{
    CMD_Node *pstNext = LIST_First(pList);

    while (pstNext != NULL) {
        if (CMD_OK == compare(pstNext, stKey)) {
            return pstNext;
        }
        pstNext = LIST_NEXT(pstNext);
    }

    return NULL;
}

int
CMD_TreeCompare(void *p, KEY_STRU stkey)
{
    CMD_CmdInfo *pstRed = (CMD_CmdInfo *)p;

    if ((CMD_STRCMP(pstRed->pcName, (CHAR *)stkey.pValue) == 0)) {
        return CMD_OK;
    }

    return CMD_ERROR;
}

VOID *
CMD_FindTreeNode(CMD_List *pstlist, CHAR *pName)
{
    KEY_STRU stKey;
    stKey.pValue = pName;

    return LIST_Walk(pstlist, stKey, CMD_TreeCompare);
}

ULONG
CMD_AddTreeNode(CMD_List *pstlist, CMD_CmdInfo *pstCmd)
{
    LIST_Insert(pstlist, (CMD_Node *)pstCmd);

    return CMD_OK;
}

ULONG
CMD_TreeInit()
{
    NODE_INIT(&CmdRoot.stList.stNode);

    CmdRoot.usIType = TOKEN;
    CmdRoot.usPType = TOKEN;
    CmdRoot.usCType = TOKEN;
    CmdRoot.usEType = TOKEN;
    CmdRoot.usGType = TOKEN;
    CmdRoot.usVType = TOKEN;
    CmdRoot.usOther = TOKEN;
    CmdRoot.usGroup = TRUE;
    CmdRoot.TotleType = TOKEN;

    return CMD_OK;
}

ULONG
CMD_ISValueNode(CMD_CmdInfo *pstCmd)
{
    if (pstCmd->TotleType & VALUE || pstCmd->TotleType & V_LEAF ||
        pstCmd->TotleType & D_VALUE || pstCmd->TotleType & D_TOKEN) {
        return TRUE;
    }

    return FALSE;
}

VOID
CMD_SetNodeType(CMD_CmdInfo *pstCmd, ULONG ulType, ULONG ulMode)
{
    if ((ulMode & INIT_MODE) == INIT_MODE) {
        pstCmd->usIType = pstCmd->usIType | (USHORT)ulType;
    }
    if ((ulMode & PRIV_MODE) == PRIV_MODE) {
        pstCmd->usPType = pstCmd->usPType | (USHORT)ulType;
    }
    if ((ulMode & CNNN_MODE) == CNNN_MODE) {
        pstCmd->usCType = pstCmd->usCType | (USHORT)ulType;
    }
    if (ulMode != ~INIT_MODE && ulMode != GLOB_MODE &&
        (ulMode & GROP_MODE) == GROP_MODE) {
        pstCmd->usGroup = pstCmd->usGroup = TRUE;
    }

    return;
}

ULONG
CMD_GetNodeType(ULONG ulSes, CMD_CmdInfo *pstCmd)
{
    ULONG ulRet = 0;

    if (pstCmd == NULL) {
        return CMD_ERROR;
    }

    if (CmdCtrl[ulSes].CurMode.ucOne == GENE_MODE1) {
        ulRet = ulRet | pstCmd->usIType;
    } else if (CmdCtrl[ulSes].CurMode.ucOne == PRIV_MODE1) {
        ulRet = ulRet | pstCmd->usPType;
    } else if (CmdCtrl[ulSes].CurMode.ucOne == DBUG_MODE1) {
        ulRet = ulRet | pstCmd->usOther;
    } else {
        if (CmdCtrl[ulSes].CurMode.usTwo == NONE_MODE2) {
            ulRet = ulRet | pstCmd->usCType;
        } else {
            if (CmdCtrl[ulSes].CurMode.ucThree == ETHE_MODE3) {
                ulRet = ulRet | pstCmd->usEType;
            } else if (CmdCtrl[ulSes].CurMode.ucThree == GIGA_MODE3) {
                ulRet = ulRet | pstCmd->usGType;
            } else {
                ulRet = ulRet | pstCmd->usVType;
            }
        }
    }

    return ulRet;
}

ULONG
CMD_GetGroupType(ULONG ulSes, CMD_CmdInfo *pstCmd)
{
    if (pstCmd == NULL) {
        return FALSE;
    }

    if (pstCmd->TotleType == GLOB_MODE || pstCmd->TotleType == (~INIT_MODE)) {
        return FALSE;
    }

    if (CmdCtrl[ulSes].CurMode.ucOne != CONF_MODE1 ||
        CmdCtrl[ulSes].CurMode.usTwo != IF_MODE2) {
        return FALSE;
    }

    if (CmdCtrl[ulSes].CurMode.ucThree != ETHE_MODE3 &&
        CmdCtrl[ulSes].CurMode.ucThree != GIGA_MODE3 &&
        CmdCtrl[ulSes].CurMode.ucThree != VLAN_MODE3) {
        return FALSE;
    }

    if (pstCmd->usGroup == 0) {
        return FALSE;
    }

    return TRUE;
}

ULONG
CMD_BuildTree(CMD_TreeRecd *m_pastCmdTable)
{
    CMD_List *pstlist;
    CMD_CmdInfo *pstCmd;
    CMD_TreeRecd *pstTable;
    pstlist = &CmdRoot.stList;

    for (pstTable = m_pastCmdTable; pstTable->ulType != END_TREE; pstTable++) {
        if (0 == pstTable->ulType) {
            pstlist = &CmdRoot.stList;
            continue;
        }

        if ((pstTable->usVarType == CMD_STR || pstTable->usVarType == CMD_INT) && pstTable->pcVarLimit == NULL) {
            return ERROR;
        }

        if ((pstCmd = (CMD_CmdInfo *)CMD_FindTreeNode(pstlist, pstTable->pcName)) == NULL) {
            if (pstlist->ulCount != 0) {
                pstCmd = (CMD_CmdInfo *)(VOID *)LIST_First(pstlist);

                if (CMD_ISValueNode(pstCmd) == TRUE) {
                    return ERROR;
                }
            }

            pstCmd = (CMD_CmdInfo *)calloc(1, sizeof(CMD_CmdInfo));
            if (NULL == pstCmd) {
                return ERROR;
            }

            pstCmd->TotleType = pstTable->ulType;
            pstCmd->ulLevel = pstTable->ulLevel;
            pstCmd->ulMode = pstTable->ulMode;
            pstCmd->pcName = pstTable->pcName;
            pstCmd->pcTip = pstTable->pcTip;
            pstCmd->pcDomain = pstTable->pcDomain;
            pstCmd->pFunc = pstTable->pFunc;
            pstCmd->stVarInfo.usType = pstTable->usVarType;
            pstCmd->stVarInfo.usResever = NULL_WORD;

            CMD_SetNodeType(pstCmd, pstTable->ulType, pstTable->ulMode);

            if (pstCmd->stVarInfo.usType != NULL_WORD) {
                if (CMD_OK != CMD_GetVarLimit(pstTable->pcVarLimit, &pstCmd->stVarInfo.ulDLimit, &pstCmd->stVarInfo.ulSLimit)) {
                    return ERROR;
                }
            } else {
                pstCmd->stVarInfo.ulDLimit = NULL_LONG;
                pstCmd->stVarInfo.ulSLimit = NULL_LONG;
            }

            NODE_INIT(&pstCmd->stNode)
            NODE_INIT(&pstCmd->stList.stNode);

            pstCmd->stList.ulCount = 0;

            CMD_AddTreeNode(pstlist, pstCmd);
        } else {
            if (pstCmd->pcDomain == NULL) {
                pstCmd->pcDomain = pstTable->pcDomain;
            }
            if (pstCmd->pFunc == NULL) {
                pstCmd->pFunc = pstTable->pFunc;
            }

            pstCmd->ulMode = pstCmd->ulMode | pstTable->ulMode;
            pstCmd->TotleType = pstCmd->TotleType | pstTable->ulType;

            CMD_SetNodeType(pstCmd, pstTable->ulType, pstTable->ulMode);
        }

        pstlist = &pstCmd->stList;
    }

    return OK;
}

ULONG
CMD_IsCommandCanUse(ULONG ulSession, ULONG ulMode, ULONG ulLevel)
{
    if (CmdCtrl[ulSession].UserLevel < ulLevel) {
        return FALSE;
    }

    if (ulMode == GLOB_MODE) {
        return TRUE;
    } else if (ulMode == (GLOB_MODE & (~INIT_MODE))) {
        return (CmdCtrl[ulSession].CurMode.ucOne != GENE_MODE1) ? TRUE : FALSE;
    } else {
        UINT8 ucOne = (ulMode >> 24);
        UINT16 usTwo = (ulMode & MASK_VALUE2) >> 8;
        UINT8 ucThree = (ulMode & MASK_VALUE3);

        switch (CmdCtrl[ulSession].CurMode.ucOne) {
        case GENE_MODE1:
            return (ucOne & GENE_MODE1) ? TRUE : FALSE;
        case PRIV_MODE1:
            return (ucOne & PRIV_MODE1) ? TRUE : FALSE;
        case DBUG_MODE1:
            return (ucOne & DBUG_MODE1) ? TRUE : FALSE;
        case CONF_MODE1:
            if (CmdCtrl[ulSession].CurMode.usTwo == NONE_MODE2) {
                return (usTwo & NONE_MODE2) ? TRUE : FALSE;
            } else if (CmdCtrl[ulSession].CurMode.usTwo == RIP_MODE2) {
                return (usTwo & RIP_MODE2) ? TRUE : FALSE;
            } else if (CmdCtrl[ulSession].CurMode.usTwo == OSPF_MODE2) {
                return (usTwo & OSPF_MODE2) ? TRUE : FALSE;
            } else if (CmdCtrl[ulSession].CurMode.usTwo == IF_MODE2) {
                if ((usTwo & IF_MODE2) && (ucThree & CmdCtrl[ulSession].CurMode.ucThree)) {
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        default:
            return FALSE;
        }
    }
}

ULONG
CMD_IsCommandCanDisplay(ULONG ulSession, ULONG ulMode, ULONG ulLevel)
{
    if (CmdCtrl[ulSession].UserLevel < ulLevel)
        return FALSE;
    if (ulMode == GLOB_MODE) {
        return (CmdCtrl[ulSession].CurMode.ucOne == GENE_MODE1) ? TRUE : FALSE;
    } else if (ulMode == (GLOB_MODE & (~INIT_MODE))) {
        return (CmdCtrl[ulSession].CurMode.ucOne != GENE_MODE1) ? TRUE : FALSE;
    } else {
        UINT8 ucOne = ulMode >> 24;
        UINT16 usTwo = (ulMode & MASK_VALUE2) >> 8;
        UINT8 ucThree = ulMode & MASK_VALUE3;
        switch (CmdCtrl[ulSession].CurMode.ucOne) {
        case GENE_MODE1:
            return (ucOne & GENE_MODE1) ? TRUE : FALSE;
        case PRIV_MODE1:
            return (ucOne & PRIV_MODE1) ? TRUE : FALSE;
        case DBUG_MODE1:
            return (ucOne & DBUG_MODE1) ? TRUE : FALSE;
        case CONF_MODE1:
            if (CmdCtrl[ulSession].CurMode.usTwo == NONE_MODE2)
                return (usTwo & NONE_MODE2) ? TRUE : FALSE;
            else if (CmdCtrl[ulSession].CurMode.usTwo == RIP_MODE2)
                return (usTwo & RIP_MODE2) ? TRUE : FALSE;
            else if (CmdCtrl[ulSession].CurMode.usTwo == OSPF_MODE2)
                return (usTwo & OSPF_MODE2) ? TRUE : FALSE;
            else if (CmdCtrl[ulSession].CurMode.usTwo == IF_MODE2) {
                if ((usTwo & IF_MODE2) &&
                    (ucThree & CmdCtrl[ulSession].CurMode.ucThree))
                    return TRUE;
                else
                    return FALSE;
            }
        default:
            return FALSE;
        }
    }
}

VOID
CMD_DisplayPrompt(ULONG ulSes)
{
    ULONG ulLog2;
    ULONG ulLen = 0;
    CHAR acFmt[128];

    if (ulSes > CLI_MAX_TELNET) {
        return;
    }

    if (CmdCtrl[ulSes].usPing == TRUE) {
        return;
    }
    if (ulSes == CLI_CONSOLE_ID) {
        ulLen += sprintf(acFmt + ulLen, "\r\n\r\n%s", CmdPrompt);
    } else {
        ulLen += sprintf(acFmt + ulLen, "\r\n\r\n%s-%ld", "Vty", ulSes);
    }

    if (CmdCtrl[ulSes].CurMode.ucOne != CONF_MODE1) {
        ulLog2 = CMD_MathLog2(CmdCtrl[ulSes].CurMode.ucOne);
        ulLen += sprintf(acFmt + ulLen, "%s", m_pacOnePrompt[ulLog2]);
    } else {
        ulLog2 = CMD_MathLog2(CmdCtrl[ulSes].CurMode.usTwo);
        ulLen += sprintf(acFmt + ulLen, "%s", m_pacTwoPrompt[ulLog2]);

        if ((CmdCtrl[ulSes].CurMode.ucThree == ETHE_MODE3 &&
             ((CmdCtrl[ulSes].CurMode.ulPorts & 0xF0000000) == 0xF0000000)) ||
            (CmdCtrl[ulSes].CurMode.ucThree == GIGA_MODE3 &&
             ((CmdCtrl[ulSes].CurMode.ulPorts & 0xF0000000) == 0xF0000000))) {
            ulLen += sprintf(acFmt + ulLen, "%s", "-group");
        } else {
            ulLog2 = CMD_MathLog2(CmdCtrl[ulSes].CurMode.ucThree);
            ulLen += sprintf(acFmt + ulLen, "%s", m_pacThreePrompt[ulLog2]);
        }

        if (CmdCtrl[ulSes].CurMode.ucThree != NONE_MODE3 &&
            (CmdCtrl[ulSes].CurMode.ucThree == VLAN_MODE3 ||
             ((CmdCtrl[ulSes].CurMode.ucThree == ETHE_MODE3 &&
               ((CmdCtrl[ulSes].CurMode.ulPorts & 0xF0000000) != 0xF0000000)) ||
              (CmdCtrl[ulSes].CurMode.ucThree == GIGA_MODE3 &&
               ((CmdCtrl[ulSes].CurMode.ulPorts & 0xF0000000) != 0xF0000000))))) {
            ulLen += sprintf(acFmt + ulLen, "%ld", CmdCtrl[ulSes].CurMode.ulPorts);
        }

        ulLog2 = CMD_MathLog2(CmdCtrl[ulSes].CurMode.ucOne);
        ulLen += sprintf(acFmt + ulLen, "%s", m_pacOnePrompt[ulLog2]);
    }

    CmdCtrl[ulSes].ulPromLen = ulLen - 4;

    CMD_OutputStr(ulSes, acFmt);
}

VOID
CMD_InitUserMode(CMD_Ctrl *CmdCtrl)
{
    CmdCtrl->CurMode.ucOne = GENE_MODE1;
    CmdCtrl->CurMode.usTwo = NONE_MODE2;
    CmdCtrl->CurMode.ucThree = NONE_MODE3;
    CmdCtrl->CurMode.ulPorts = NULL_LONG;
}

VOID
CMD_SetUserMode(ULONG ulSes, ULONG ulMode)
{
    CmdCtrl[ulSes].CurMode.ucOne = (ulMode >> 24);
    CmdCtrl[ulSes].CurMode.usTwo = (ulMode & MASK_VALUE2) >> 8;
    CmdCtrl[ulSes].CurMode.ucThree = (ulMode & MASK_VALUE3);

    if (CmdCtrl[ulSes].CurMode.usTwo != IF_MODE2 ||
        (CmdCtrl[ulSes].CurMode.ucThree != ETHE_MODE3 &&
         CmdCtrl[ulSes].CurMode.ucThree != GIGA_MODE3 &&
         CmdCtrl[ulSes].CurMode.ucThree != VLAN_MODE3)) {
        CmdCtrl[ulSes].CurMode.ulPorts = NULL_LONG;
    }
}

ULONG
CMD_ClrScr(char *pcBuf, int Len, char **argv, int argc)
{
    ULONG ulSes = atoi(argv[0]);

    CMD_OutputStr(ulSes, "\x1b[2J");

    return CMD_OK;
}

int
CMD_HelpDisplay(char *pcBuf, int Len, char **argv, int argc)
{
    CMD_Sprintf(&pcBuf, "\n\r Help may be requested at any point in a command by entering"
                "\n\r a question mark '?'.  If nothing matches, the help list will be empty."
                "\n\r Two styles of help are provided:"
                "\n\r 1. Full help is available, when you are ready to enter a command"
                "\n\r    argument ( e.g. 'show ?' ) and describes each possible argument."
                "\n\r 2. Partial help is provided, when an abbreviated argument is entered"
                "\n\r    and you want to know what arguments match the input (e.g. 'show v?')");

    return OK;
}

ULONG
CMD_ShowHistory(ULONG ulSes)
{
    ULONG ulHead;
    ULONG ulIndex;

    if (CmdCtrl[ulSes].ulHisNums == 0) {
        return CMD_OK;
    }

    if (CmdCtrl[ulSes].ulHisTail > CmdCtrl[ulSes].ulHisHead) {
        for (ulHead = CmdCtrl[ulSes].ulHisHead, ulIndex = 1; ulHead < CmdCtrl[ulSes].ulHisTail; ulHead++, ulIndex++) {
            CMD_PrintfStr(ulSes, "\r\n%-5ld : %s", ulIndex, CmdCtrl[ulSes].History[ulHead]);
        }
    } else {
        for (ulHead = CmdCtrl[ulSes].ulHisHead, ulIndex = 1; ulIndex <= HIST_NUM; ulIndex++) {
            CMD_PrintfStr(ulSes, "\r\n%-5ld : %s", ulIndex, CmdCtrl[ulSes].History[ulHead]);
            ulHead = (ulHead + 1) % HIST_NUM;
        }
    }

    return CMD_OK;
}

ULONG
CMD_History(char *pcBuf, int Len, char **argv, int argc)
{
    CMD_ShowHistory(atoi(argv[0]));

    return CMD_OK;
}

unsigned long
CMD_Reboot(char *pcBuf, int Len, char **argv, int argc)
{
    if (Len < 256 || argc < 0) {
        return CMD_ERROR;
    }

    if (ERROR == CMD_ConfirmQuery(atoi(argv[0]))) {
        return CMD_OK;
    } else {
        system("reboot");
    }

    return CMD_OK;
}

ULONG
CMD_Exit(char *pcBuf, int Len, char **argv, int argc)
{
    UINT ulSes = 0;


    CMD_PrintfStr(ulSes, "\r\n");
    if (tcsetattr(CmdCtrl[ulSes].BaseIo.ulIFd, TCSAFLUSH, &g_stTermios) < 0) {
        CMD_PrintfStr(ulSes, "tcsetattr fail\r\n");
    }
    exit(EXIT_SUCCESS);

}


CMD_TreeRecd CmdBasicTable[] =
{
    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "clr", CMD_ClrScr, NULL, "Clear screen"),
    END_RECORD,

    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "reboot", CMD_Reboot, NULL, "Reboot system"),
    END_RECORD,

    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "history", CMD_History, NULL, "Display contents in command history"),
    END_RECORD,

    CMD_TLEAF(UGUEST, PRIV_MODE | INIT_MODE | GROP_MODE, "help", CMD_HelpDisplay, NULL, "Display the description of interactive help system"),
    END_RECORD,

    CMD_TLEAF(UGUEST, GLOB_MODE, "exit", CMD_Exit, NULL, "Exit CLI"), END_RECORD,


    {END_TREE, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL},
};

