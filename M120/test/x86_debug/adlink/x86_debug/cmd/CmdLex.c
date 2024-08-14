/*
 * CmdLex.c
 */

#include "CmdTypes.h"
#include "CmdCommon.h"
#include "CmdTask.h"
#include "CmdTree.h"
#include "CmdLex.h"

extern struct termios g_stTermios;
static CMD_CmdInfo *m_apstSortPrompt[CLI_MAX_TELNET + 1][SORT_NUM];

ULONG
CMD_StringCmp(CHAR *pSrc, CHAR *pDst)
{
    CHAR c1, c2;

    while (*pSrc && *pDst) {
        c1 = *pSrc++;
        c2 = *pDst++;
        if (cmd_tolower(c1) != cmd_tolower(c2)) {
            return 1;
        }
    }

    if (*pSrc != *pDst) {
        return 1;
    }

    return 0;
}

ULONG
CMD_SortCompare(VOID *pD1, VOID *pD2)
{
    CHAR *pcD1 = (*((CMD_CmdInfo **)pD1))->pcName;
    CHAR *pcD2 = (*((CMD_CmdInfo **)pD2))->pcName;
    CHAR cChar1 = 0;
    CHAR cChar2 = 0;

    while ((cChar1 = (*pcD1++)) != 0 && (cChar2 = (*pcD2++)) != 0) {
        if (cChar1 == cChar2) {
            continue;
        } else {
            return (cChar1 < cChar2) ? -1 : 1;
        }
    }
    if (cChar1 == cChar2) {
        return 0;
    } else {
        return (cChar1 < cChar2) ? -1 : 1;
    }
}

ULONG
CMD_SortBuffer(ULONG ulSes, ULONG ulCount)
{
    qsort(&m_apstSortPrompt[ulSes], ulCount, sizeof(CMD_CmdInfo *), (VOID *)CMD_SortCompare);

    return CMD_OK;
}

ULONG
CMD_DisPSortBuf(ULONG ulSes, ULONG ulCount)
{
    ULONG ulIndex;
    ULONG ulFlag = 0;
    CHAR acTempBuf[100];

    CMD_CmdInfo *pstCurNode;

    for (ulIndex = 0; ulIndex < ulCount; ulIndex++) {
        pstCurNode = m_apstSortPrompt[ulSes][ulIndex];

        if (2 == ulFlag) {
            sprintf(acTempBuf, "\r%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
            ulFlag = 0;
        } else if (3 == ulFlag) {
            if (ulIndex < ulCount - 1) {
                sprintf(acTempBuf, "\r%-15s %s\r\n", pstCurNode->pcName, pstCurNode->pcTip);
            } else {
                sprintf(acTempBuf, "\r%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
            }

            ulFlag = 0;
        } else {
            sprintf(acTempBuf, "\r\n%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
        }

        CMD_OutputStr(ulSes, acTempBuf);

        if (ulIndex < ulCount - 1) {
            CmdCtrl[ulSes].ulLines++;

            if ((PAGE_SCROLL == CmdCtrl[ulSes].ulScroll && CmdCtrl[ulSes].ulLines == SCR_ROWS) ||
                LINE_SCROLL == CmdCtrl[ulSes].ulScroll) {
                if (PAGE_SCROLL == CmdCtrl[ulSes].ulScroll) {
                    CMD_OutputStr(ulSes, "\r\n");
                    CMD_DispNextTips(ulSes);
                    ulFlag = 1;
                } else {
                    CMD_DispNextTips(ulSes);
                    ulFlag = 1;
                }

                CMD_GetScrollMode(ulSes);

                if (NONE_SCROLL == CmdCtrl[ulSes].ulScroll) {
                    break;
                }

                if (ulFlag == 1) {
                    CMD_OutputStr(ulSes, "\x1b[2K");
                    CMD_OutputStr(ulSes, "\r");
                    if (LINE_SCROLL == CmdCtrl[ulSes].ulScroll) {
                        ulFlag = 3;
                    } else {
                        ulFlag = 2;
                    }
                }
            }
        }
    }

    return CMD_OK;
}

ULONG
CMD_IsCanGroupOper(ULONG ulSes, CMD_CmdInfo *pstCurNode)
{
    if (pstCurNode->pFunc == NULL ||
        CmdCtrl[ulSes].CurMode.ulPorts == NULL_LONG ||
        (CmdCtrl[ulSes].CurMode.ulPorts & 0xF0000000) != 0xF0000000) {
        return CMD_ERROR;
    }

    if (CMD_GetGroupType(ulSes, pstCurNode)) {
        return CMD_OK;
    }

    return CMD_ERROR;
}

ULONG
CMD_IfParaLegal(CMD_CmdInfo *pstNode, CHAR *pcPara)
{
    ULONG ulInteger;
    ULONG ulIntLen = strlen(pcPara);

    switch (pstNode->stVarInfo.usType) {
    case CMD_STR:

        if ((pstNode->stVarInfo.ulSLimit != NULL_LONG && pstNode->stVarInfo.ulSLimit < ulIntLen) ||
            (pstNode->stVarInfo.ulDLimit != NULL_LONG && pstNode->stVarInfo.ulDLimit > ulIntLen)) {
            return CMD_ERROR;
        }
        break;

    case CMD_INT:

        if (OK != CMD_StrToInt(pcPara, ulIntLen, &ulInteger)) {
            return CMD_ERROR;
        }
        if ((pstNode->stVarInfo.ulDLimit != NULL_LONG && ulInteger < pstNode->stVarInfo.ulDLimit) ||
            (pstNode->stVarInfo.ulSLimit != NULL_LONG && ulInteger > pstNode->stVarInfo.ulSLimit)) {
            return CMD_ERROR;
        }
        break;

    default:

        return CMD_ERROR;
    }

    return CMD_OK;
}

VOID
CMD_GetScrollMode(ULONG ulSes)
{
    for (;; ) {
        CHAR acChar[1];

        CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd, acChar, 1);
        CmdCtrl[ulSes].ulTrig = tickGet();

        if (acChar[0] == 0x4) {
            CmdCtrl[ulSes].ulScroll = NONE_SCROLL;
            break;
        } else if (acChar[0] == ' ') {
            CmdCtrl[ulSes].ulScroll = PAGE_SCROLL;
            break;
        } else if (acChar[0] == '\n' || acChar[0] == '\r') {
            CmdCtrl[ulSes].ulScroll = LINE_SCROLL;
            break;
        } else {
            continue;
        }
    }

    CmdCtrl[ulSes].ulLines = 0;
}

ULONG
CMD_CheckQuotMark(ULONG ulSes)
{
    ULONG ulIndex;
    ULONG ulCount = 0;

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].ulELen; ulIndex++) {
        if ('"' == CmdCtrl[ulSes].acEBuf[ulIndex]) {
            ulCount++;
        }
    }
    if ((ulCount & 0x1) == 0) {
        return CMD_OK;
    } else {
        return CMD_ERROR;
    }
}

CMD_CmdInfo *
CMD_ScanTokens(ULONG ulSes, CMD_List *pstList, CHAR *pcName, ULONG *pulTimes)
{
    ULONG ulIndex;
    ULONG ulNameLen;
    ULONG ulMaxMatch = 0;

    CMD_CmdInfo *pstNode;
    CMD_CmdInfo *pstRet = NULL;

    *pulTimes = 0;

    if (CMD_STRCMP(pcName, "?") == 0) {
        return NULL;
    }

    ulNameLen = strlen(pcName);
    pstNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstList);

    if (pstNode != NULL &&
        ((pstNode->TotleType & VALUE) || (pstNode->TotleType & V_LEAF)) &&
        CMD_GetNodeType(ulSes, pstNode) != 0) {
        *pulTimes = 1;
        return pstNode;
    }

    for (; pstNode != NULL; ) {
        if (TRUE != CMD_IsCommandCanUse(ulSes, pstNode->ulMode, pstNode->ulLevel)) {
            pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
            continue;
        }

        if (CMD_GetNodeType(ulSes, pstNode) == 0) {
            pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
            continue;
        }

        if (ulNameLen == strlen(pstNode->pcName)) {
            if (CMD_STRCMP(pstNode->pcName, pcName) == 0) {
                *pulTimes = 1;
                pstRet = pstNode;
                break;
            }
        } else if (ulNameLen < strlen(pstNode->pcName)) {
            for (ulIndex = 0; ulIndex < ulNameLen; ulIndex++) {
                if (cmd_tolower(pcName[ulIndex]) != cmd_tolower((pstNode->pcName)[ulIndex])) {
                    break;
                }
            }

            if (ulIndex == ulNameLen) {
                *pulTimes = *pulTimes + 1;

                if (ulIndex > ulMaxMatch) {
                    ulMaxMatch = ulIndex;
                    pstRet = pstNode;
                }
            }
        }

        pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
    }

    return (CMD_CmdInfo *)pstRet;
}

ULONG
CMD_ExtendToken(ULONG ulSes, CMD_List *pstList, CHAR *pcToken)
{
    ULONG ulIndex;
    ULONG ulRet = 0;
    ULONG ulLen = strlen(pcToken);

    CMD_CmdInfo *pstNode;

    pstNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstList);

    if (pstNode != NULL &&
        ((pstNode->TotleType & VALUE) || (pstNode->TotleType & V_LEAF)) &&
        CMD_GetNodeType(ulSes, pstNode) != 0) {
        return ulRet;
    }

    for (; pstNode != NULL; ) {
        if (TRUE != CMD_IsCommandCanUse(ulSes, pstNode->ulMode, pstNode->ulLevel) ||
            (0 == strcmp(pstNode->pcName, "no"))) {
            pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
            continue;
        }

        if (CMD_GetNodeType(ulSes, pstNode) == 0) {
            pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
            continue;
        }

        if (ulLen <= strlen(pstNode->pcName)) {
            for (ulIndex = 0; ulIndex < ulLen; ulIndex++) {
                if ((cmd_tolower(pcToken[ulIndex]) != cmd_tolower((pstNode->pcName)[ulIndex]))) {
                    break;
                }
            }
            if (ulIndex == ulLen) {
                m_apstSortPrompt[ulSes][ulRet++] = pstNode;
            }
        }
        pstNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)pstNode);
    }

    return ulRet;
}

CMD_CmdInfo *
CMD_ScanDefTokens(ULONG ulSes, CMD_List *pstList, CHAR *pcName, ULONG *pulTimes)
{
    ULONG ulIndex;
    ULONG ulNameLen;
    ULONG ulMaxMatch;

    CMD_CmdInfo *pstNode;
    CMD_CmdInfo *pstRet = NULL;

    ulMaxMatch = 0;
    *pulTimes = 0;
    ulNameLen = strlen(pcName);

    pstNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstList);

    for (; pstNode != NULL; ) {
        if (ulNameLen == strlen(pstNode->pcName)) {
            if (CMD_STRCMP(pstNode->pcName, pcName) == 0) {
                *pulTimes = 1;
                pstRet = pstNode;
                break;
            }
        } else if (ulNameLen < strlen(pstNode->pcName)) {
            for (ulIndex = 0; ulIndex < ulNameLen; ulIndex++) {
                if (cmd_tolower(pcName[ulIndex]) != cmd_tolower((pstNode->pcName)[ulIndex])) {
                    break;
                }
            }
            if (ulIndex == ulNameLen) {
                *pulTimes = *pulTimes + 1;
                if (ulIndex > ulMaxMatch) {
                    ulMaxMatch = ulIndex;
                    pstRet = pstNode;
                }
            }
        }
        pstNode = (CMD_CmdInfo *)(VOID *)LIST_First(&pstNode->stList);
        pstNode = (CMD_CmdInfo *)(VOID *)LIST_First(&pstNode->stList);
    }

    return pstRet;
}

ULONG
CMD_ProcComTree(ULONG ulSession)
{
    ULONG ulType;
    ULONG ulIndex;
    ULONG ulTimes;
    ULONG ulCanEnd = FALSE;
    CHAR acTempBuf[100];
    CMD_CmdInfo *pstCurNode;
    CMD_CmdInfo *pstPreNode = NULL;
    CMD_CmdInfo *pstCurLeaf = NULL;
    CMD_CmdInfo *pstResult = NULL;
    CMD_List *pstCurList;
    Promt_Enum enScanState;

    enScanState = enCmdButt;
    pstCurList = &CmdRoot.stList;
    pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
    CMD_ClearParameter(ulSession);
    CMD_AddPrestorePara(ulSession);

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSession].TreeDep; ulIndex++) {
        ulTimes = 0;
        if (pstCurNode->TotleType & TOKEN || pstCurNode->TotleType & T_LEAF) {
            pstResult = CMD_ScanTokens(ulSession, pstCurList, CmdCtrl[ulSession].CmdTree[ulIndex], &ulTimes);
            if (1 == ulTimes && pstResult != NULL) {
                ulType = CMD_GetNodeType(ulSession, pstResult);

                if (ulIndex < CmdCtrl[ulSession].TreeDep - 1) {
                    if (ulType & TOKEN && ulType & T_LEAF) {
                        if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                            ulCanEnd = TRUE;
                            if (NULL != pstResult->stList.NODE_HEAD) {
                                pstCurList = &pstResult->stList;
                                pstPreNode = pstCurNode;
                                pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                            }
                            enScanState = enHelpCommand;
                        } else if (NULL != pstResult->stList.NODE_HEAD) {
                            pstCurLeaf = pstResult;
                            pstPreNode = pstCurNode;
                            pstCurList = &pstResult->stList;
                            pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                        } else {
                            enScanState = enUnwantedToken;
                        }
                    } else if (ulType & TOKEN) {
                        pstCurList = &pstResult->stList;
                        pstPreNode = pstCurNode;
                        pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                    } else {
                        if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                            if (NULL != pstResult->stList.NODE_HEAD) {
                                pstCurList = &pstResult->stList;
                                pstPreNode = pstCurNode;
                                pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                                enScanState = enHelpCommand;
                            } else {
                                enScanState = enCommandEnd;
                            }
                        } else if (NULL != pstResult->stList.NODE_HEAD) {
                            pstCurLeaf = pstResult;
                            pstPreNode = pstCurNode;
                            pstCurList = &pstResult->stList;
                            pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                        } else {
                            enScanState = enUnwantedToken;
                        }
                    }
                } else {
                    if (ulType & T_LEAF) {
                        pstCurNode = pstResult;
                        enScanState = enExecutable;
                    } else {
                        enScanState = enUncompleted;
                    }
                }
            } else if (0 == ulTimes) {
                if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], "?")) {
                    enScanState = enHelpCommand;
                } else {
                    enScanState = enUnknowed;
                }
            } else {
                ULONG ulTmp;
                ulTmp = CMD_ExtendToken(ulSession, pstCurList, CmdCtrl[ulSession].CmdTree[ulIndex]);
                if (0 != ulTmp) {
                    CMD_SortBuffer(ulSession, ulTmp);
                    CMD_DisPSortBuf(ulSession, ulTmp);
                }
                enScanState = enWaitingExtend;
                break;
            }
        } else if (pstCurNode->TotleType & VALUE || pstCurNode->TotleType & V_LEAF) {
            if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], "?")) {
                enScanState = enHelpCommand;
            } else if (CMD_OK != CMD_IfParaLegal(pstCurNode, CmdCtrl[ulSession].CmdTree[ulIndex])) {
                enScanState = enParameterError;
            } else {
                CMD_AddParameter(ulSession, CmdCtrl[ulSession].CmdTree[ulIndex]);
                if (pstCurNode->TotleType & VALUE && pstCurNode->TotleType & V_LEAF) {
                    if (ulIndex < CmdCtrl[ulSession].TreeDep - 1) {
                        if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                            ulCanEnd = TRUE;
                            if (NULL != pstCurNode->stList.NODE_HEAD) {
                                pstCurList = &pstCurNode->stList;
                                pstPreNode = pstCurNode;
                                pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                            }
                            enScanState = enHelpCommand;
                        } else if (NULL != pstCurNode->stList.NODE_HEAD) {
                            pstCurLeaf = pstCurNode;
                            pstPreNode = pstCurNode;
                            pstCurList = &pstCurNode->stList;
                            pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                        } else {
                            enScanState = enUnwantedToken;
                        }
                    } else {
                        enScanState = enExecutable;
                    }
                } else if (pstCurNode->TotleType & VALUE) {
                    if (ulIndex == CmdCtrl[ulSession].TreeDep - 1) {
                        if ((pstCurNode->TotleType & V_LEAF)) {
                            enScanState = enExecutable;
                        } else {
                            enScanState = enUncompleted;
                        }
                    } else {
                        pstCurList = &pstCurNode->stList;
                        pstPreNode = pstCurNode;
                        pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                    }
                } else {
                    if (ulIndex < CmdCtrl[ulSession].TreeDep - 1) {
                        if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                            if (NULL != pstCurNode->stList.NODE_HEAD) {
                                pstCurList = &pstCurNode->stList;
                                pstPreNode = pstCurNode;
                                pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                                enScanState = enHelpCommand;
                            } else {
                                enScanState = enCommandEnd;
                            }
                        } else if (NULL != pstCurNode->stList.NODE_HEAD) {
                            pstCurLeaf = pstCurNode;
                            pstPreNode = pstCurNode;
                            pstCurList = &pstCurNode->stList;
                            pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                        } else {
                            enScanState = enUnwantedToken;
                        }
                    } else {
                        enScanState = enExecutable;
                    }
                }
            }
        } else if (pstCurNode->TotleType & D_TOKEN) {
            pstResult = CMD_ScanDefTokens(ulSession, pstCurList, CmdCtrl[ulSession].CmdTree[ulIndex], &ulTimes);
            if (NULL != pstResult && 1 == ulTimes) {
                if (ulIndex == CmdCtrl[ulSession].TreeDep - 1) {
                    enScanState = enUncompleted;
                } else {
                    CMD_CmdInfo *pstTmp = NULL;
                    for (pstTmp = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                         0 != CMD_StringCmp(pstTmp->pcName, pstResult->pcName); ) {
                        pstTmp = (CMD_CmdInfo *)(VOID *)LIST_First(&pstTmp->stList);
                        pstTmp = (CMD_CmdInfo *)(VOID *)LIST_First(&pstTmp->stList);
                        CMD_AddParameter(ulSession, NULL);
                    }
                    pstCurList = &pstResult->stList;
                    pstPreNode = pstCurNode;
                    pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                }
            } else if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], "?")) {
                enScanState = enHelpCommand;
            } else {
                enScanState = enUnknowed;
            }
        } else if (pstCurNode->TotleType & D_VALUE) {
            if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], "?")) {
                enScanState = enHelpCommand;
            } else if ( CMD_OK != CMD_IfParaLegal(pstCurNode, CmdCtrl[ulSession].CmdTree[ulIndex])) {
                enScanState = enParameterError;
            } else {
                CMD_AddParameter(ulSession, CmdCtrl[ulSession].CmdTree[ulIndex]);

                if (ulIndex == CmdCtrl[ulSession].TreeDep - 1) {
                    enScanState = enExecutable;
                } else {
                    if (NULL == pstCurNode->stList.NODE_HEAD) {
                        if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                            enScanState = enCommandEnd;
                        } else {
                            enScanState = enUnknowed;
                        }
                    } else {
                        pstCurList = &pstCurNode->stList;
                        pstPreNode = pstCurNode;
                        pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
                    }
                }
            }
        } else if (pstCurNode->TotleType & DT_LEAF) {
            if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], "?")) {
                enScanState = enHelpCommand;
            } else if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex], pstCurNode->pcName)) {
                if (ulIndex == CmdCtrl[ulSession].TreeDep - 1) {
                    enScanState = enExecutable;
                } else {
                    if (0 == CMD_STRCMP(CmdCtrl[ulSession].CmdTree[ulIndex + 1], "?")) {
                        enScanState = enCommandEnd;
                    } else {
                        enScanState = enUnwantedToken;
                    }
                }
            } else {
                enScanState = enUnknowed;
            }
        }
        if (enScanState != enCmdButt)
            break;
    }
    switch (enScanState) {
    case enUncompleted:
        break;
    case enUnknowed:
        CMD_OutputStr(ulSession, "\r\nUnknown command, type ? for help");
        break;
    case enExecutable:
        if (!(pstCurNode->TotleType & T_LEAF) && !(pstCurNode->TotleType & V_LEAF) && !(pstCurNode->TotleType & DT_LEAF)) {
            pstCurNode = pstCurLeaf;
        }
        if (pstCurNode->pcDomain != NULL) {
            CMD_AddParameter(ulSession, pstCurNode->pcDomain);
        }

        if (CMD_IsCanGroupOper(ulSession, pstCurNode) == CMD_OK) {
            ULONG ulPortIndex;
            CHAR acPortBuf[10];
            ULONG ulPortList = CmdCtrl[ulSession].CurMode.ulPorts;

            for (ulPortIndex = 1; ulPortIndex <= 24; ulPortIndex++) {
                if (!(ulPortList & 0x1)) {
                    ulPortList = ulPortList >> 1;
                    continue;
                }
                ulPortList = ulPortList >> 1;
                memset(CmdCtrl[ulSession].acPBuf, 0, PBUF_LEN);

                if (CmdCtrl[ulSession].CurMode.ucThree == ETHE_MODE3) {
                    sprintf(acPortBuf, "%ld", ulPortIndex);
                } else {
                    sprintf(acPortBuf, "%ld", ulPortIndex + 24);
                }
                CmdCtrl[ulSession].CurPara[2] = acPortBuf;

                if (pstCurNode->pFunc) {
                    (*pstCurNode->pFunc)(CmdCtrl[ulSession].acPBuf, PBUF_LEN, CmdCtrl[ulSession].CurPara, CmdCtrl[ulSession].ParaNum);
                    if (CmdCtrl[ulSession].acPBuf[0] != 0) {
                        CMD_OutputBuf(ulSession, CmdCtrl[ulSession].acPBuf);
                    }
                }
            }
        } else {
            if (CmdCtrl[ulSession].CurMode.ulPorts != NULL_LONG &&
                (CmdCtrl[ulSession].CurMode.ulPorts & 0xF0000000) == 0xF0000000) {
                CMD_OutputStr(ulSession, "\r\nThis command cann't support group-operation!");
            } else {
                memset(CmdCtrl[ulSession].acPBuf, 0, PBUF_LEN);

                if (pstCurNode->pFunc) {
                    (*pstCurNode->pFunc)(CmdCtrl[ulSession].acPBuf, PBUF_LEN, CmdCtrl[ulSession].CurPara, CmdCtrl[ulSession].ParaNum);

                    if (CmdCtrl[ulSession].acPBuf[0] != 0) {
                        CMD_OutputBuf(ulSession, CmdCtrl[ulSession].acPBuf);
                    }
                }
            }
        }
        break;
    case enUnwantedToken:
        CMD_OutputStr(ulSession, "\r\nToo many arguments, type ? for help");
        break;
    case enHelpCommand:
        if (pstCurNode->TotleType & D_TOKEN) {
            CMD_OutputStr(ulSession, "\r\n<enter>");
            while (NULL != pstCurNode) {
                if (pstCurNode->TotleType & D_TOKEN) {
                    sprintf(acTempBuf, "\r\n%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
                    CMD_OutputStr(ulSession, acTempBuf);
                }
                pstCurList = &pstCurNode->stList;
                pstCurNode = (CMD_CmdInfo *)(VOID *)LIST_First(pstCurList);
            }

        } else if (pstCurNode->TotleType & D_VALUE) {
            if (pstPreNode->TotleType & T_LEAF || pstPreNode->TotleType & V_LEAF) {
                CMD_OutputStr(ulSession, "\r\n<enter>");
            }
            sprintf(acTempBuf, "\r\n%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
            CMD_OutputStr(ulSession, acTempBuf);
        } else if (pstCurNode->TotleType & DT_LEAF) {
            CMD_OutputStr(ulSession, "\r\n<enter>");
            sprintf(acTempBuf, "\r\n%-15s %s", pstCurNode->pcName, pstCurNode->pcTip);
            CMD_OutputStr(ulSession, acTempBuf);
        } else {
            ulTimes = 0;
            CmdCtrl[ulSession].ulLines = 0;
            CmdCtrl[ulSession].ulScroll = PAGE_SCROLL;
            pstCurNode = (CMD_CmdInfo *)LIST_First(pstCurList);
            if (ulCanEnd)
                CMD_OutputStr(ulSession, "\r\n<enter>");
            while (pstCurNode != NULL) {
                if ((TRUE == CMD_IsCommandCanDisplay(ulSession, pstCurNode->ulMode, pstCurNode->ulLevel) &&
                     0 != strcmp(pstCurNode->pcName, "no")) ||
                    (!(pstCurNode->TotleType & TOKEN) && !(pstCurNode->TotleType & T_LEAF))) {
                    m_apstSortPrompt[ulSession][ulTimes++] = pstCurNode;
                }
                pstCurNode = (CMD_CmdInfo *)LIST_NEXT(&pstCurNode->stNode);
            }
            CMD_SortBuffer(ulSession, ulTimes);
            CMD_DisPSortBuf(ulSession, ulTimes);
        }
        break;
    case enCommandEnd:
        CMD_OutputStr(ulSession, "\r\nPlease input <enter> to excute the command.");
        break;
    case enParameterError:
        CMD_OutputStr(ulSession, "\r\nParameter error, type ? for help");
        break;
    case enWaitingExtend:
    case enCmdButt:
        break;
    default:
        break;
    }
    CMD_ClearParameter(ulSession);
    return enScanState;
}

VOID
CMD_SubmitCommand(ULONG ulSes)
{
    CHAR cChar;
    ULONG ulIndex;
    ULONG ulRepair = FALSE;
    ULONG ulResult = 0;
    ULONG ulLexState = 0;
    ULONG ulQuoting = FALSE;

    ULONG ulAskFlag = FALSE;

    CMD_ClearTreeInfo(ulSes);

    CMD_HisNewPrompt(ulSes);

    if (CMD_OK != CMD_CheckQuotMark(ulSes)) {
        CMD_OutputStr(ulSes, "\r\nBe sure quots marked");
        CMD_DisplayPrompt(ulSes);
        CmdCtrl[ulSes].ulELen = 0;
        CmdCtrl[ulSes].usString = NONE_QUOT;
        return;
    }

    if (CmdCtrl[ulSes].ulELen > 1 &&
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == '?' &&
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 2] != ' ') {
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] = ' ';
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = '?';
        ulRepair = TRUE;
    }

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].ulELen; ulIndex++) {
        cChar = CmdCtrl[ulSes].acEBuf[ulIndex];

        if (cChar == '\n' || cChar == '\r') {
            if (ulLexState == 2) {
                CmdCtrl[ulSes].TreeDep++;
                CmdCtrl[ulSes].acEBuf[ulIndex] = 0;
            }
            break;
        } else if (cChar == '"') {
            ulQuoting = !ulQuoting;
            if (ulQuoting) {
                CmdCtrl[ulSes].CmdTree[CmdCtrl[ulSes].TreeDep] = CmdCtrl[ulSes].acEBuf + ulIndex;
                ulLexState = 2;
            }
        } else if (cChar == ' ') {
            if (ulQuoting) {
                continue;
            }
            if (ulLexState == 0) {
                ulLexState = 1;
                continue;
            } else if (ulLexState == 2) {
                CmdCtrl[ulSes].TreeDep++;
                CmdCtrl[ulSes].acEBuf[ulIndex] = 0;
                ulLexState = 0;
            }
        } else if (cChar == '?') {
            if (ulQuoting) {
                continue;
            }

            ulAskFlag = TRUE;

            if (ulLexState != 2) {
                CmdCtrl[ulSes].acEBuf[ulIndex + 1] = 0;
                CmdCtrl[ulSes].CmdTree[CmdCtrl[ulSes].TreeDep++] = CmdCtrl[ulSes].acEBuf + ulIndex;
                break;
            } else {
                CmdCtrl[ulSes].acEBuf[ulIndex + 1] = 0;
                CmdCtrl[ulSes].TreeDep++;
                break;
            }
        } else {
            if (ulQuoting) {
                continue;
            }
            if (ulLexState != 2) {
                CmdCtrl[ulSes].CmdTree[CmdCtrl[ulSes].TreeDep] = CmdCtrl[ulSes].acEBuf + ulIndex;
                ulLexState = 2;
            }
        }
    }

    if (CmdCtrl[ulSes].TreeDep > 0) {
        ulResult = CMD_ProcComTree(ulSes);
    }

    CMD_DisplayPrompt(ulSes);

    if (TRUE == ulAskFlag) {
        CMD_RebuildEditBuf(ulSes);
        if (ulResult == enCommandEnd || ulRepair == TRUE) {
            if (CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == ' ') {
                CmdCtrl[ulSes].ulELen--;
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen] = 0;
            }
        }
        CMD_OutputStr(ulSes, CmdCtrl[ulSes].acEBuf);
    } else if (ulResult == enUncompleted) {
        CMD_RebuildEditBuf(ulSes);
        CMD_OutputStr(ulSes, CmdCtrl[ulSes].acEBuf);
    } else {
        CmdCtrl[ulSes].ulELen = 0;
    }

    CmdCtrl[ulSes].usString = NONE_QUOT;
}

VOID
CMD_FinishToken(ULONG ulSes)
{
    CHAR cChar;
    ULONG ulIndex;

    ULONG ulTimes;
    ULONG ulLexState = 0;
    ULONG ulQuoting = FALSE;

    CMD_CmdInfo *CurNode;
    CMD_List *CurList;
    CMD_CmdInfo *PreNode = NULL;
    CMD_CmdInfo *RetNode = NULL;

    CMD_ClearTreeInfo(ulSes);

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].ulELen; ulIndex++) {
        cChar = CmdCtrl[ulSes].acEBuf[ulIndex];

        if (cChar == ' ') {
            if (ulQuoting) {
                continue;
            }
            if (ulLexState == 0) {
                ulLexState = 1;
                continue;
            } else if (ulLexState == 2) {
                CmdCtrl[ulSes].TreeDep++;
                CmdCtrl[ulSes].acEBuf[ulIndex] = 0;
                ulLexState = 0;
            }
        } else if (cChar == '"') {
            ulQuoting = !ulQuoting;

            if (ulQuoting) {
                CmdCtrl[ulSes].CmdTree[CmdCtrl[ulSes].TreeDep] = CmdCtrl[ulSes].acEBuf + ulIndex;
                ulLexState = 2;
            }
        } else {
            if (ulQuoting) {
                continue;
            }
            if (ulLexState != 2) {
                CmdCtrl[ulSes].CmdTree[CmdCtrl[ulSes].TreeDep] = CmdCtrl[ulSes].acEBuf + ulIndex;
                ulLexState = 2;
            }
        }
    }

    if (CmdCtrl[ulSes].TreeDep <= 0) {
        return;
    }

    CurList = &CmdRoot.stList;

    CurNode = (CMD_CmdInfo *)LIST_First(CurList);

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].TreeDep - 1; ulIndex++) {
        ulTimes = 0;

        if (CurNode == NULL) {
            CMD_RebuildEditBuf(ulSes);
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
            return;
        }

        if (CurNode->TotleType & TOKEN || CurNode->TotleType & T_LEAF) {
            RetNode = CMD_ScanTokens(ulSes, CurList, CmdCtrl[ulSes].CmdTree[ulIndex], &ulTimes);

            if (1 == ulTimes && RetNode != NULL) {
                CurList = &RetNode->stList;
                PreNode = CurNode;
                CurNode = (CMD_CmdInfo *)LIST_First(CurList);
            } else {
                CMD_RebuildEditBuf(ulSes);
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
                return;
            }
        } else if (((CurNode->TotleType & VALUE) || (CurNode->TotleType & V_LEAF)) &&
                   CMD_GetNodeType(ulSes, CurNode) != 0) {
            if (CMD_OK == CMD_IfParaLegal(CurNode, CmdCtrl[ulSes].CmdTree[ulIndex])) {
                CurList = &CurNode->stList;
                PreNode = CurNode;
                CurNode = (CMD_CmdInfo *)LIST_First(CurList);
            } else {
                CMD_RebuildEditBuf(ulSes);
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
                return;
            }
        } else if (CurNode->TotleType & D_TOKEN) {
            if (0 == CMD_STRCMP(CurNode->pcName, CmdCtrl[ulSes].CmdTree[ulIndex])) {
                CurList = &CurNode->stList;
                PreNode = CurNode;
                CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);
            } else {
                ulIndex--;
                CurList = &CurNode->stList;
                CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);
                CurList = &CurNode->stList;
                PreNode = CurNode;
                CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);

                if (NULL == CurNode) {
                    CMD_RebuildEditBuf(ulSes);
                    CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
                    return;
                }
            }
        } else if (CurNode->TotleType & DT_LEAF) {
            CMD_RebuildEditBuf(ulSes);
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
            return;
        } else if (CurNode->TotleType & D_VALUE) {
            if (PreNode->TotleType == T_LEAF || PreNode->TotleType == V_LEAF) {
                CMD_RebuildEditBuf(ulSes);
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
                return;
            }
            CurList = &CurNode->stList;
            CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);
        } else {
            return;
        }
    }
    if (NULL == CurNode) {
        return;
    }
    if (CurNode->TotleType & TOKEN || CurNode->TotleType & T_LEAF ||
        CurNode->TotleType & D_TOKEN || CurNode->TotleType & DT_LEAF) {
        CHAR *pcName;
        ULONG ulCount;
        ULONG ulNameLen;

        CMD_CmdInfo *pastNode[MAX_SAME_TOKEN];

        ulCount = 0;
        pcName = CmdCtrl[ulSes].CmdTree[ulIndex];
        ulNameLen = strlen(pcName);

        if (CurNode->TotleType & TOKEN || CurNode->TotleType & T_LEAF) {
            for (; CurNode != NULL; ) {
                if (TRUE != CMD_IsCommandCanUse(ulSes, CurNode->ulMode, CurNode->ulLevel)) {
                    CurNode = (CMD_CmdInfo *)(VOID *)LIST_NEXT((CMD_Node *)(VOID *)CurNode);
                    continue;
                }
                if (CMD_GetNodeType(ulSes, CurNode) == 0) {
                    CurNode = (CMD_CmdInfo *)LIST_NEXT((CMD_Node *)(VOID *)CurNode);
                    continue;
                }

                if (ulNameLen <= strlen(CurNode->pcName)) {
                    for (ulIndex = 0; ulIndex < ulNameLen; ulIndex++) {
                        if (cmd_tolower(pcName[ulIndex]) != cmd_tolower((CurNode->pcName)[ulIndex])) {
                            break;
                        }
                    }
                    if (ulIndex == ulNameLen) {
                        pastNode[ulCount++] = CurNode;
                        if (MAX_SAME_TOKEN == ulCount) {
                            break;
                        }
                    }
                }

                CurNode = (CMD_CmdInfo *)LIST_NEXT((CMD_Node *)CurNode);
            }
        } else if (D_TOKEN & CurNode->TotleType) {
            for (; CurNode != NULL; ) {
                if (ulNameLen <= strlen(CurNode->pcName)) {
                    for (ulIndex = 0; ulIndex < ulNameLen; ulIndex++) {
                        if (cmd_tolower(pcName[ulIndex]) != cmd_tolower((CurNode->pcName)[ulIndex])) {
                            break;
                        }
                    }

                    if (ulIndex == ulNameLen) {
                        pastNode[ulCount++] = CurNode;
                        if (MAX_SAME_TOKEN == ulCount) {
                            break;
                        }
                    }
                }
                CurList = &CurNode->stList;
                CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);
                CurList = &CurNode->stList;
                CurNode = (CMD_CmdInfo *)(VOID *)LIST_First(CurList);
            }
        } else {
            if (ulNameLen <= strlen(CurNode->pcName)) {
                for (ulIndex = 0; ulIndex < ulNameLen; ulIndex++) {
                    if (cmd_tolower(pcName[ulIndex]) != cmd_tolower((CurNode->pcName)[ulIndex])) {
                        break;
                    }
                }
                if (ulIndex == ulNameLen) {
                    pastNode[ulCount++] = CurNode;
                }
            }
        }
        CMD_RebuildEditBuf(ulSes);
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
        if (ulCount == 0) {
            return;
        } else if (ulCount == 1) {
            ULONG ulType;

            pcName = (pastNode[0])->pcName;
            pcName = pcName + ulNameLen;
            strncpy(CmdCtrl[ulSes].acEBuf + CmdCtrl[ulSes].ulELen, pcName, strlen(pcName));
            CmdCtrl[ulSes].ulELen += strlen(pcName);
            CMD_OutputStr(ulSes, pcName);
            ulType = CMD_GetNodeType(ulSes, pastNode[0]);
            if (!((ulType & T_LEAF) == T_LEAF) && !((ulType & DT_LEAF) == DT_LEAF)) {
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = ' ';
                CMD_OutputStr(ulSes, " ");
            }
        } else {
            CHAR cTemp;
            ULONG ulFlag = TRUE;
            for (;; ) {
                if (strlen((pastNode[0])->pcName) <= ulNameLen) {
                    break;
                }
                cTemp = (pastNode[0])->pcName[ulNameLen];
                for (ulIndex = 1; ulIndex < ulCount; ulIndex++) {
                    if (strlen((pastNode[ulIndex])->pcName) <= ulNameLen) {
                        ulFlag = FALSE;
                        break;
                    }
                    if (cmd_tolower((pastNode[ulIndex])->pcName[ulNameLen]) != cmd_tolower(cTemp)) {
                        ulFlag = FALSE;
                        break;
                    }
                }
                if (TRUE == ulFlag) {
                    CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cTemp;
                    ulNameLen++;
                } else {
                    break;
                }
            }
            CMD_OutputStr(ulSes, "\r\n");
            CmdCtrl[ulSes].BaseIo.ulSLen = 0;
            for (ulIndex = 0; ulIndex < ulCount; ) {
                CmdCtrl[ulSes].BaseIo.ulSLen += sprintf(CmdCtrl[ulSes].BaseIo.acSBuf + CmdCtrl[ulSes].BaseIo.ulSLen,
                                                        "%-15s",
                                                        (pastNode[ulIndex])->pcName);
                ulIndex++;
                if ((ulIndex % TAB_DISP_NUM) == 0) {
                    CmdCtrl[ulSes].BaseIo.acSBuf[CmdCtrl[ulSes].BaseIo.ulSLen] = 0;
                    CMD_OutputStr(ulSes, CmdCtrl[ulSes].BaseIo.acSBuf);
                    CMD_OutputStr(ulSes, "\r\n");
                    CmdCtrl[ulSes].BaseIo.ulSLen = 0;
                }
            }
            if ((ulIndex % TAB_DISP_NUM) != 0) {
                CmdCtrl[ulSes].BaseIo.acSBuf[CmdCtrl[ulSes].BaseIo.ulSLen] = 0;
                CMD_OutputStr(ulSes, CmdCtrl[ulSes].BaseIo.acSBuf);
            }
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen] = 0;
            CMD_DisplayPrompt(ulSes);
            CMD_OutputStr(ulSes, CmdCtrl[ulSes].acEBuf);
        }
    } else {
        CMD_RebuildEditBuf(ulSes);
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen--] = 0;
    }
}

ULONG
CMD_ProcDelete(ULONG ulSes)
{
    ULONG Quots;
    ULONG ulIndex;

    if (CmdCtrl[ulSes].ulELen <= 0) {
        return CMD_OK;
    }

    Quots = 0;

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].ulELen; ulIndex++) {
        if ('"' == CmdCtrl[ulSes].acEBuf[ulIndex]) {
            Quots++;
        }
    }

    CmdCtrl[ulSes].ulELen--;

    if (CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen] == '"') {
        if (0x1 == (Quots & 0x1)) {
            CmdCtrl[ulSes].usString = NONE_QUOT;
        } else {
            CmdCtrl[ulSes].usString = LEFT_QUOT;
        }
    }

    CMD_DeletePrevChar(ulSes);

    if ((Quots & 0x1) == 0x0 && CmdCtrl[ulSes].ulELen > 0 &&
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == '"') {
        CmdCtrl[ulSes].usString = RIGHT_QUOT;
    }

    return CMD_OK;
}

ULONG
CMD_ProcTabKey(ULONG ulSes)
{
    if (CmdCtrl[ulSes].ulELen > 0 && CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] != ' ') {
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = ' ';
        CMD_FinishToken(ulSes);
    }

    return CMD_OK;
}

ULONG
CMD_ProcCtrlZ(ULONG ulSes)
{
    int iOpfd = 1;
    CHAR acPassword[PWORD_LEN + 1];
    char acPaswd[PWORD_LEN + 1] = _CLI_PASSWORS;
    struct termios cnsl_flags;

    if (CMD_OK != CMD_GetSuperUserPWord(ulSes, acPassword, PWORD_LEN + 1)) {
        return CMD_ERROR;
    }

    if (0 == strcmp(acPassword, acPaswd)) {
        tcgetattr(iOpfd, &cnsl_flags);

        if (tcsetattr(iOpfd, TCSAFLUSH, &g_stTermios) < 0) {
            printf("set attr err...\r\n");
            return CMD_ERROR;
        }

        system("sh");

        if (tcsetattr(iOpfd, TCSAFLUSH, &cnsl_flags) < 0) {
            printf("set attr err...\r\n");
            return CMD_ERROR;
        }
        /* exit(EXIT_SUCCESS); */
    }
    return CMD_OK;
}

ULONG
CMD_ProcCtrlD(ULONG ulSes)
{

    CHAR acPassword[PWORD_LEN + 1];
    char acPaswd[PWORD_LEN + 1] = _CLI_PASSWORS;

    if (CMD_OK != CMD_GetSuperUserPWord(ulSes, acPassword, PWORD_LEN + 1)) {
        return CMD_ERROR;
    }

    if (0 == strcmp(acPassword, acPaswd)) {
        CmdCtrl[ulSes].UserLevel = UADMIN;
        CMD_SetUserMode(ulSes, PRIV_MODE);
    }

    CMD_DisplayPrompt(ulSes);

    return CMD_OK;
}


VOID
CMD_EditSpecChar(ULONG ulSes, CHAR cChar)
{
    switch (cChar) {
    case '\b':
        CMD_ProcDelete(ulSes);
        break;
    case '\t':
        CMD_ProcTabKey(ulSes);
        break;
    case 4:
        CMD_ProcCtrlD(ulSes);
        break;
    case 16:
        CMD_PrevHisRecord(ulSes);
        break;
    case 14:
        CMD_NextHisRecord(ulSes);
        break;
    case 26:
        CMD_ProcCtrlZ(ulSes);
        break;
    default:
        break;
    }
}

ULONG
CMD_IsNormalChar(CHAR cChar)
{
    if ((cChar >= 'A' && cChar <= 'Z') || (cChar >= 'a' && cChar <= 'z') ||
        (cChar >= '0' && cChar <= '9') || cChar == '-' || cChar == '_' ||
        cChar == '.' || cChar == ':' || cChar == '/' || cChar == ',' ||
        cChar == ' ') {
        return TRUE;
    }

    return FALSE;
}

VOID
CMD_LexProcess(ULONG ulSes)
{
    CHAR cChar;
    ULONG ulCount;
    ULONG ulIndex;
    ULONG ulQuotIndex;

    for (ulIndex = 0; ulIndex < CmdCtrl[ulSes].BaseIo.ulRLen; ulIndex++) {
        cChar = CmdCtrl[ulSes].BaseIo.acRBuf[ulIndex];

        if (TRUE == CmdCtrl[ulSes].usPing) {
            if (cChar == 0x3) {
                CMD_StopPing(ulSes);
            }
            break;
        }
        if ('\b' == cChar) {
            CMD_EditSpecChar(ulSes, cChar);
            break;
        }
        if (cChar == '\n' || cChar == '\r') {
            if (CmdCtrl[ulSes].ulELen == 0) {
                CMD_DisplayPrompt(ulSes);
                return;
            } else {
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
                CMD_SubmitCommand(ulSes);
                break;
            }
        }
        if (CmdCtrl[ulSes].ulELen >= DISP_LEN - 1) {
            break;
        }
        if (CmdCtrl[ulSes].usString == LEFT_QUOT && cChar != '"') {
            if (TRUE == CMD_IsNormalChar(cChar)) {
                CMD_OutputChar(ulSes, cChar);
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
            }
            break;
        }
        if (CmdCtrl[ulSes].usString == RIGHT_QUOT && cChar != '?') {
            if (cChar == ' ') {
                CMD_OutputChar(ulSes, cChar);
                CmdCtrl[ulSes].usString = NONE_QUOT;
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
            }
            break;
        }
        if (cChar == '"') {
            ulCount = 0;
            for (ulQuotIndex = 0; ulQuotIndex < CmdCtrl[ulSes].ulELen; ulQuotIndex++) {
                if ('"' == CmdCtrl[ulSes].acEBuf[ulQuotIndex]) {
                    ulCount++;
                }
            }
            if (0x1 == (ulCount & 0x1)) {
                if (CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == '"') {
                    break;
                } else {
                    CMD_OutputChar(ulSes, cChar);
                    CmdCtrl[ulSes].usString = RIGHT_QUOT;
                    CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
                }
            } else {
                if (CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] != ' ') {
                    break;
                } else {
                    CMD_OutputChar(ulSes, cChar);
                    CmdCtrl[ulSes].usString = LEFT_QUOT;
                    CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
                }
            }
        } else if (cChar == '?') {
            CMD_OutputChar(ulSes, '?');
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
            CMD_SubmitCommand(ulSes);
            break;
        } else if (cChar == ' ') {
            if (CmdCtrl[ulSes].ulELen != 0 && CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] != ' ') {
                CMD_OutputChar(ulSes, cChar);
                CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
            }
        } else if (cChar == 27 && NULL_LONG == CmdCtrl[ulSes].ulESC) {
            CmdCtrl[ulSes].ulESC = 27;
        } else if (cChar == '[' && 27 == CmdCtrl[ulSes].ulESC) {
            CmdCtrl[ulSes].ulESC = '[';
        } else if ('[' == CmdCtrl[ulSes].ulESC) {
            CmdCtrl[ulSes].ulESC = NULL_LONG;

            if ((cChar == 'A') || (cChar == 'B')) {
                if (cChar == 'A') {
                    cChar = 16;
                } else {
                    cChar = 14;
                }

                CMD_EditSpecChar(ulSes, cChar);
            }
        } else if (TRUE == CMD_IsNormalChar(cChar)) {
            CMD_OutputChar(ulSes, cChar);
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = cChar;
        } else {
            CMD_EditSpecChar(ulSes, cChar);
        }
    }

    CMD_ClearRecvBuf(ulSes);
}

