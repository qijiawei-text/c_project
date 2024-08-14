/*
 * CmdCommon.c
 */

#include "CmdTypes.h"
#include "CmdCommon.h"
#include "CmdTask.h"
#include "CmdTree.h"
#include "CmdLex.h"

ULONG CMD_MathLog2(ULONG x)
{
    switch (x)
    {
        case 0x1:
            return 0;
        case 0x2:
            return 1;
        case 0x4:
            return 2;
        case 0x8:
            return 3;
        case 0x10:
            return 4;
        case 0x20:
            return 5;
        case 0x40:
            return 6;
        case 0x80:
            return 7;
        case 0x100:
            return 8;
        case 0x200:
            return 9;
        case 0x400:
            return 10;
        case 0x800:
            return 11;
        case 0x1000:
            return 12;
        case 0x2000:
            return 13;
        case 0x4000:
            return 14;
        case 0x8000:
            return 15;
        default :
            return 0;
    }
}

ULONG CMD_StrToInt(CHAR *pcSourcr, ULONG ulLen, ULONG *pulNum)
{
    CHAR  cChar;
    ULONG ulIndex;
    ULONG ulResult = 0;

    if (pcSourcr == NULL || ulLen > strlen(pcSourcr))
    {
        *pulNum = 0xFFFFFFFF;
        return ERROR;
    }

    for (ulIndex = 0; ulIndex < ulLen; ulIndex++)
    {
        cChar = pcSourcr[ulIndex];

        if (!IS_NUMBER(cChar))
        {
            *pulNum = 0xFFFFFFFF;
            return ERROR;
        }

        ulResult = ulResult*10 + CHAR2NUM(cChar);
    }

    *pulNum = ulResult;

    return OK;
}

ULONG CMD_Sprintf(char **buf, char *fmt, ...)
{
    ULONG   ulCount;
    va_list vaList;

    va_start(vaList, fmt);
    ulCount  = vsprintf(*buf, fmt, vaList);
    *buf     = *buf  + ulCount;
    va_end(vaList);

    return ulCount;
}

ULONG CMD_ConfirmQuery(ULONG ulSes)
{
    ULONG ulCount = 0;
    CHAR  acChar[2];
    int   iRet = ERROR;

    CMD_OutputStr(ulSes, "\r\nDo you want to continue (y/n) [n] :");

    for(;;)
    {
        CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd, &acChar[ulCount], 1);

        CmdCtrl[ulSes].ulTrig = tickGet();

        if (0 == ulCount)
        {
            if (acChar[0] == 'y' || acChar[0] == 'Y')
            {
                CMD_OutputChar(ulSes, acChar[0]);
                ulCount ++;
                iRet = OK;
            }
            else if (acChar[0] == 'n' || acChar[0] == 'N')
            {
                CMD_OutputChar(ulSes, acChar[0]);
                ulCount ++;
                iRet = ERROR;
            }
            else if (acChar[0] == '\r' || acChar[0] == '\n')
            {
                return ERROR;
            }
            else
            {
                continue;
            }
        }
        else
        {
            if (acChar[1] == '\n' || acChar[1] == '\r')
            {
                return iRet;
            }
            else if (acChar[1] == '\b')
            {
                CMD_DeletePrevChar(ulSes);
                ulCount = 0;
                continue;
            }
            else
            {
                continue;
            }
        }
    }
}

VOID CMD_ClearRecvBuf(ULONG ulSes)
{
    CmdCtrl[ulSes].BaseIo.ulRLen    = 0;
    CmdCtrl[ulSes].BaseIo.acRBuf[0] = 0;
}

VOID CMD_ClearTreeInfo(ULONG ulSes)
{
    CmdCtrl[ulSes].TreeDep    = 0;
    CmdCtrl[ulSes].CmdTree[0] = CmdCtrl[ulSes].acEBuf;
}

ULONG CMD_GetVarLimit(CHAR *pcLimit, ULONG *pulDLimit, ULONG *pulSLimit)
{
    ULONG  ulTemp;
    CHAR  *pcPos = NULL;
    ULONG  ulLen = 0;

    if (pcLimit == NULL || pulDLimit == NULL || pulSLimit == NULL)
    {
        return CMD_ERROR;
    }

    ulLen = strlen(pcLimit);

    if (pcLimit[0] != '[' || pcLimit[ulLen - 1] != ']')
    {
        return CMD_ERROR;
    }
    if (ulLen == 3 && pcLimit[1] == '.')
    {
        *pulDLimit = *pulSLimit = NULL_LONG;
        return CMD_OK;
    }

    pcPos = strchr(pcLimit, '-');
    if (NULL == pcPos)
    {
        if (OK != CMD_StrToInt(pcLimit + 1, ulLen - 2, &ulTemp))
        {
            return CMD_ERROR;
        }
        *pulDLimit = ulTemp;
        *pulSLimit = ulTemp;
    }
    else
    {
        if ((pcPos - pcLimit - 1) == 1 &&  pcLimit[1] == '.')
        {
            ulTemp = NULL_LONG;
        }
        else
        {
            if (OK != CMD_StrToInt(pcLimit + 1, pcPos - pcLimit - 1, &ulTemp))
            {
                return CMD_ERROR;
            }
        }

        *pulDLimit = ulTemp;

        ulLen = strlen(pcPos + 1);
        if ((ulLen - 1) == 1 &&  pcPos[1] == '.')
        {
            ulTemp = NULL_LONG;
        }
        else
        {
            if (OK != CMD_StrToInt(pcPos + 1, strlen(pcPos + 1) - 1, &ulTemp))
            {
                return CMD_ERROR;
            }
        }

        *pulSLimit = ulTemp;
    }

    return CMD_OK;
}

VOID CMD_AddParameter(ULONG ulSes, CHAR *pcPara)
{
    ULONG ulIndex = 0;
    ULONG ulQuot  = FALSE;
    ULONG ulLen;

    if (NULL == pcPara)
        ulLen = 0;
    else
        ulLen = strlen(pcPara);

    if (CmdCtrl[ulSes].ParaNum >= PARA_NUM)
    {
        return;
    }
    if (NULL == pcPara)
    {
        CmdCtrl[ulSes].CurPara[CmdCtrl[ulSes].ParaNum] = NULL;
    }
    else
    {
        CmdCtrl[ulSes].CurPara[CmdCtrl[ulSes].ParaNum] = CmdCtrl[ulSes].ParaBuf + CmdCtrl[ulSes].ulParaBLen;

        if (*pcPara == '"')
        {
            ulQuot = TRUE;
            ulIndex++;
            ulLen--;
        }

        if (ulQuot)
        {
            for (; ulIndex < ulLen; ulIndex++)
            {
                CmdCtrl[ulSes].ParaBuf[CmdCtrl[ulSes].ulParaBLen++] = cmd_tolower(pcPara[ulIndex]);
            }
        }
        else
        {
            for (; ulIndex < ulLen; ulIndex++)
            {
                CmdCtrl[ulSes].ParaBuf[CmdCtrl[ulSes].ulParaBLen++] = pcPara[ulIndex];
            }
        }

        CmdCtrl[ulSes].ParaBuf[CmdCtrl[ulSes].ulParaBLen++] = 0;
    }

    CmdCtrl[ulSes].ParaNum++;
}

VOID CMD_AddPrestorePara(ULONG ulSes)
{
    CHAR  acTempBuf[16];
    ULONG ulPort = CmdCtrl[ulSes].CurMode.ulPorts;

    sprintf(acTempBuf, "%ld", ulSes);
    CMD_AddParameter(ulSes, acTempBuf);

    if (CmdCtrl[ulSes].CurMode.ucOne  == CONF_MODE1 && CmdCtrl[ulSes].CurMode.usTwo  == IF_MODE2)
    {
        if (CmdCtrl[ulSes].CurMode.ucThree == ETHE_MODE3)
        {
            CMD_AddParameter(ulSes, "ethernet");
        }
        else if (CmdCtrl[ulSes].CurMode.ucThree == GIGA_MODE3)
        {
            CMD_AddParameter(ulSes, "slot");
            ulPort = ulPort + 24;
        }
        else
        {
            CMD_AddParameter(ulSes, "vlan");
        }
        sprintf(acTempBuf, "%ld", ulPort);
        CMD_AddParameter(ulSes, acTempBuf);
    }
    else
    {
        CMD_AddParameter(ulSes, NULL);
        CMD_AddParameter(ulSes, NULL);
    }
}

VOID CMD_ClearParameter(ULONG ulSes)
{
    ULONG ulTemp;

    CmdCtrl[ulSes].ParaNum    = 0;
    CmdCtrl[ulSes].ulParaBLen = 0;

    for (ulTemp = 0; ulTemp < PARA_NUM; ulTemp++)
    {
        CmdCtrl[ulSes].CurPara[ulTemp] = NULL;
    }
}

VOID CMD_HisNewPrompt(ULONG ulSes)
{
    char acCmp[2];

    acCmp[0] = cmd_tolower(CmdCtrl[ulSes].acEBuf[0]);
    acCmp[1] = cmd_tolower(CmdCtrl[ulSes].acEBuf[1]);


    if (CmdCtrl[ulSes].ulHisNums < HIST_NUM)
    {
        CmdCtrl[ulSes].ulHisNums++;
    }

    strncpy(CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulHisTail],
            CmdCtrl[ulSes].acEBuf, CmdCtrl[ulSes].ulELen);

    if (CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == '\n' ||
        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen - 1] == '\r')
    {
        CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulHisTail][CmdCtrl[ulSes].ulELen - 1] = 0;
    }
    else
    {
        CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulHisTail][CmdCtrl[ulSes].ulELen] = 0;
    }
    CmdCtrl[ulSes].ulHisTail ++;

    if (CmdCtrl[ulSes].ulHisTail == HIST_NUM)
    {
        CmdCtrl[ulSes].ulHisTail = 0;
    }

    if (CmdCtrl[ulSes].ulHisNums == HIST_NUM)
    {
        CmdCtrl[ulSes].ulHisHead = CmdCtrl[ulSes].ulHisTail;
    }

    CmdCtrl[ulSes].ulCurHisPos = CmdCtrl[ulSes].ulHisTail;
}

VOID CMD_PrevHisRecord(ULONG ulSes)
{
    ULONG ulHistory = CmdCtrl[ulSes].ulCurHisPos;

    if (CmdCtrl[ulSes].ulHisNums == 0)
    {
        return;
    }

    if (CmdCtrl[ulSes].ulCurHisPos == 0)
    {
        CmdCtrl[ulSes].ulCurHisPos = HIST_NUM - 1;
    }
    else
    {
        CmdCtrl[ulSes].ulCurHisPos--;
    }

    if (CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulCurHisPos][0] != 0)
    {
        CMD_ClearCurPrompt(ulSes);

        strcpy(CmdCtrl[ulSes].acEBuf, CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulCurHisPos]);
        CmdCtrl[ulSes].ulELen = strlen(CmdCtrl[ulSes].acEBuf);

        CMD_OutputStr(ulSes, CmdCtrl[ulSes].acEBuf);
    }
    else
    {
        CmdCtrl[ulSes].ulCurHisPos = ulHistory;
    }
}

VOID CMD_NextHisRecord(ULONG ulSes)
{
    ULONG ulHistory = CmdCtrl[ulSes].ulCurHisPos;

    if (CmdCtrl[ulSes].ulHisNums == 0)
    {
        return;
    }

    CmdCtrl[ulSes].ulCurHisPos = (CmdCtrl[ulSes].ulCurHisPos + 1) % HIST_NUM;

    if (CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulCurHisPos][0] != 0)
    {
        CMD_ClearCurPrompt(ulSes);

        strcpy(CmdCtrl[ulSes].acEBuf, CmdCtrl[ulSes].History[CmdCtrl[ulSes].ulCurHisPos]);
        CmdCtrl[ulSes].ulELen = strlen(CmdCtrl[ulSes].acEBuf);

        CMD_OutputStr(ulSes, CmdCtrl[ulSes].acEBuf);
    }
    else
    {
        CmdCtrl[ulSes].ulCurHisPos = ulHistory;
    }
}

VOID CMD_StopPing(ULONG ulSes)
{
    if(ulSes > CLI_MAX_TELNET)
    {
        return;
    }

    CmdCtrl[ulSes].usPing = FALSE;
}

VOID CMD_RebuildEditBuf(ULONG ulSes)
{
    ULONG ulIndex;

    CmdCtrl[ulSes].ulELen = 0;

    if (CmdCtrl[ulSes].TreeDep > 0)
    {
        for(ulIndex = 0; ulIndex < CmdCtrl[ulSes].TreeDep - 1; ulIndex++)
        {
            strncpy(CmdCtrl[ulSes].acEBuf + CmdCtrl[ulSes].ulELen,
                    CmdCtrl[ulSes].CmdTree[ulIndex], strlen(CmdCtrl[ulSes].CmdTree[ulIndex]));
            CmdCtrl[ulSes].ulELen += strlen(CmdCtrl[ulSes].CmdTree[ulIndex]);
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = ' ';
        }

        if (0 != CMD_STRCMP("?", CmdCtrl[ulSes].CmdTree[ulIndex]))
        {
            strncpy(CmdCtrl[ulSes].acEBuf + CmdCtrl[ulSes].ulELen,
                    CmdCtrl[ulSes].CmdTree[ulIndex], strlen(CmdCtrl[ulSes].CmdTree[ulIndex]));
            CmdCtrl[ulSes].ulELen += strlen(CmdCtrl[ulSes].CmdTree[ulIndex]);
            CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen++] = ' ';
        }

        CmdCtrl[ulSes].acEBuf[CmdCtrl[ulSes].ulELen] = 0;
    }
}

VOID CMD_OutputStr(ULONG ulSes, CHAR *pcStr)
{
    ULONG ulLen = strlen(pcStr);

    if (pcStr == NULL || 0 == ulLen)
    {
        return;
    }

    CmdCtrl[ulSes].BaseIo.cmd_send(CmdCtrl[ulSes].BaseIo.ulOFd, pcStr, ulLen);
}

VOID CMD_PrintfStr(ULONG ulSes, CHAR *pcStr, ...)
{
    va_list vaList;
    CHAR    acBuf[1024];
    ULONG   ulLen = 0;

    if (pcStr == NULL)
    {
        return;
    }

    va_start (vaList, pcStr);
    vsprintf(acBuf, pcStr, vaList);
    va_end (vaList);

    ulLen = strlen(acBuf);

    if (0 != ulLen)
    {
        CmdCtrl[ulSes].BaseIo.cmd_send(CmdCtrl[ulSes].BaseIo.ulOFd, acBuf, ulLen);
    }
}

VOID CMD_OutputBuf(ULONG ulSes, CHAR *Buf)
{
    ULONG ulLen = 0;
    CHAR *pcBuf = Buf;
    CHAR *pcPosition = strchr(pcBuf, '\n');

    if (NULL_LONG == CmdCtrl[ulSes].BaseIo.ulOFd)
    {
        return;
    }

    CmdCtrl[ulSes].ulLines = 0;
    CmdCtrl[ulSes].ulScroll = PAGE_SCROLL;

    while (pcPosition != NULL)
    {
        ulLen = pcPosition - pcBuf;
        ulLen++;

        CmdCtrl[ulSes].BaseIo.cmd_send(CmdCtrl[ulSes].BaseIo.ulOFd, pcBuf, ulLen);
        CmdCtrl[ulSes].ulLines ++;

        if (*(pcPosition + 1) == 0)
        {
            return;
        }
        if ((PAGE_SCROLL == CmdCtrl[ulSes].ulScroll && SCR_ROWS == CmdCtrl[ulSes].ulLines) ||
             LINE_SCROLL == CmdCtrl[ulSes].ulScroll)
        {
            if (PAGE_SCROLL == CmdCtrl[ulSes].ulScroll || LINE_SCROLL == CmdCtrl[ulSes].ulScroll)
            {
                CMD_DispNextTips(ulSes);
            }

            CMD_GetScrollMode(ulSes);

            if (NONE_SCROLL == CmdCtrl[ulSes].ulScroll)
            {
                break;
            }
            else
            {
                CMD_OutputStr(ulSes, "\x1b[2K");
                CMD_OutputStr(ulSes, "\r");
            }
        }

        pcBuf = pcPosition + 1;
        pcPosition = strchr(pcBuf, '\n');
    }

    if (NONE_SCROLL != CmdCtrl[ulSes].ulScroll && *pcBuf != 0)
    {
        CmdCtrl[ulSes].BaseIo.cmd_send(CmdCtrl[ulSes].BaseIo.ulOFd, pcBuf, strlen(pcBuf));
    }
}

VOID CMD_OutputChar(ULONG ulSes, CHAR cChar)
{
    CmdCtrl[ulSes].BaseIo.acSBuf[0] = cChar;
    CmdCtrl[ulSes].BaseIo.acSBuf[1] = 0;
    CmdCtrl[ulSes].BaseIo.cmd_send(CmdCtrl[ulSes].BaseIo.ulOFd,
                                   CmdCtrl[ulSes].BaseIo.acSBuf, 1);
}

VOID CMD_ClearScreen(ULONG ulSes)
{
    CMD_OutputStr(ulSes, "\x1b[37;40m");
    CMD_OutputStr(ulSes, "\x1b[2J");
    CMD_OutputStr(ulSes, "\x1b[0m");
    CMD_OutputStr(ulSes, "\x1b[2J");
}

ULONG CMD_PreviousLine(ULONG ulSes)
{
    ULONG ulIndex;
    ULONG ulRow   = 0;
    ULONG ulCol   = 0;
    ULONG ulCount = 0;
    CHAR  acBuffer[256];
    int   temp;

    CMD_OutputStr(ulSes, "\x1b[6n");

    ioctl(CmdCtrl[ulSes].BaseIo.ulIFd, FIONREAD, (int)&temp);

    for (ulCount = 0; ulCount < temp; ulCount++)
    {
        while(1 != CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd,
                                                  &acBuffer[ulCount], 1));
    }

    for (ulIndex = 0; ulIndex < ulCount; ulIndex++)
    {
        if (acBuffer[ulIndex] == '[')
        {
            ulRow = atoi(acBuffer + ulIndex + 1);
        }
        else if ((acBuffer[ulIndex] == ';'))
        {
            ulCol = atoi(acBuffer + ulIndex + 1);
            break;
        }
    }

    if (1 != ulCol)
    {
        return FALSE;
    }

    sprintf(acBuffer, "\x1b[%ld;80H", ulRow - 1);

    CMD_OutputStr(ulSes, acBuffer);

    CMD_OutputChar  (ulSes, ' ');

    CMD_OutputStr(ulSes, acBuffer);

    return TRUE;
}

VOID CMD_DeletePrevChar(ULONG ulSes)
{
    if ((CmdCtrl[ulSes].ulPromLen + CmdCtrl[ulSes].ulELen + 1) % 80 == 0)
    {
        if (CMD_PreviousLine(ulSes))
        {
            return;
        }
    }

    CMD_OutputChar(ulSes, '\b');
    CMD_OutputChar(ulSes, ' ');
    CMD_OutputChar(ulSes, '\b');
}

VOID CMD_ClearCurPrompt(ULONG ulSes)
{
    ULONG ulIndex;

    for (ulIndex = CmdCtrl[ulSes].ulELen; ulIndex > 0; ulIndex--)
    {
        CmdCtrl[ulSes].ulELen--;
        CMD_DeletePrevChar(ulSes);
    }

    CmdCtrl[ulSes].ulELen = 0;
}

VOID CMD_WelComeInfo(ULONG ulSes)
{
    CMD_OutputStr(ulSes, _CLI_WELCOME_INFO_);
}

VOID CMD_DispNextTips(ULONG ulSes)
{
    CMD_OutputStr(ulSes, "\x1b[2K");
    CMD_OutputStr(ulSes, "\r");

    CMD_OutputStr(ulSes, "Next page: ");
    CMD_OutputStr(ulSes, "<SPACE>");

    CMD_OutputStr(ulSes, " Next line: ");
    CMD_OutputStr(ulSes, "<ENTER>");

    CMD_OutputStr(ulSes, " Quit: ");
    CMD_OutputStr(ulSes, "<Ctrl+D>");
}


static const unsigned long crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

#define DO1(buf) crc = crc_table[((int)crc ^ (*(buf)++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
#define GX16                              0x11021
unsigned long crc32(unsigned long crc, unsigned char *buf, int len)
{
    if (buf == NULL) return 0L;
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}

#ifdef _DEBUG_OPEN
INT m_iDebugLogFd = -1;

VOID CMD_DebugOpen()
{
    m_iDebugLogFd = open(CMD_DEBUG_LOG_FILE, O_CREAT | O_RDWR);
    if (m_iDebugLogFd < 0)
    {
        printf("ERROR : CMD open debug log is failed.");
    }

    return;
}

VOID CMD_DebugPrintf(CHAR *pcFilename, INT iLine, CHAR *pcStrInfo, ...)
{
    CHAR acStrInfo[CMD_LOG_LEN]   = {0};
    CHAR acTimeStr[CMD_LOG_LEN]   = {0};
    CHAR acTempStr[CMD_LOG_LEN]   = {0};
    CHAR acLogInfo[CMD_LOG_LEN]   = {0};
    va_list  args;

    struct timeval tv;
    struct tm *p;

    memset(acStrInfo, 0x00, sizeof(acStrInfo));
    memset(acTimeStr, 0x00, sizeof(acTimeStr));
    memset(acTempStr, 0x00, sizeof(acTempStr));
    memset(acLogInfo, 0x00, sizeof(acLogInfo));

    va_start(args, pcStrInfo);
    vsnprintf(acStrInfo, CMD_LOG_LEN - 1, pcStrInfo, args);   
    va_end(args);

    gettimeofday(&tv, NULL);
    p = localtime(&tv.tv_sec);

    sprintf(acTempStr, "%ld", tv.tv_usec);
    memset(acTempStr + 3, 0x00, CMD_LOG_LEN - 3);
    
    sprintf(acTimeStr, "%04d-%02d-%02d %02d:%02d:%02d.%s",
                       (1900 + p->tm_year),
                       (1 + p->tm_mon),
                       p->tm_mday,
                       p->tm_hour,
                       p->tm_min,
                       p->tm_sec,
                       acTempStr);
    
    sprintf(acLogInfo, "\n[%s] [%s:%d] %s", acTimeStr, pcFilename, iLine, acStrInfo);

    if (m_iDebugLogFd < 0)
    {
        printf(acLogInfo);
        return;
    }
    
    if (sizeof(acLogInfo) != write(m_iDebugLogFd, acLogInfo, sizeof(acLogInfo)))
    {
        printf("Write debug log error.\n");
    }

    return;
}
#endif


