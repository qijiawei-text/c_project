/*
 * CmdTask.c
 */

#include "CmdTypes.h"
#include "CmdCommon.h"
#include "CmdTask.h"
#include "CmdTree.h"
#include "CmdLex.h"

CMD_Ctrl * CmdCtrl        = NULL;
CHAR *     CmdPrompt      = NULL;
ULONG      m_MaxSession    = 0;
ULONG      m_CmdInitilized = FALSE;

extern struct termios g_stTermios;

extern CMD_TreeRecd CmdBasicTable[];
extern CMD_TreeRecd CmdSdvTestTable[];


ULONG tickGet(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);

    return time.tv_sec;
}

void taskDelay(ULONG ulDelay)
{
    ulDelay = 10000 * ulDelay;
    usleep(ulDelay);
}

VOID CMD_SendData(ULONG ulFd, CHAR *pSendBuf, ULONG ulLen)
{
    INT i = 0;
    if (0 == ulLen || NULL == pSendBuf || NULL_LONG == ulFd)
    {
        return;
    }

    #if 1
    for (i=0; i<ulLen; i++)
    {
        putchar(pSendBuf[i]);
    }
    #endif

    //write(ulFd, pSendBuf, ulLen);
}

ULONG CMD_RecvData(ULONG ulFd, CHAR *pRecvBuf, ULONG ulLen)
{
    int lRead;

    if (NULL == pRecvBuf || NULL_LONG == ulFd)
    {
        return CMD_ERROR;
    }

    pRecvBuf[0] = getchar();
    lRead = 1;

    #if 0
    lRead = read (ulFd, pRecvBuf, ulLen);
    if (lRead <= 0)
    {
        return CMD_ERROR;
    }
    #endif

    return (ULONG)lRead;
}

ULONG CMD_InitCtrlInfo(ULONG ulSes)
{
    ULONG ulTemp;
    ULONG ulIndex;

    if (NULL != CmdCtrl)
    {
        return CMD_OK;
    }

    CmdCtrl = (CMD_Ctrl *)calloc(ulSes, sizeof(CMD_Ctrl));
    if (NULL == CmdCtrl)
    {
        return CMD_ERROR;
    }

    for (ulIndex = 0; ulIndex < ulSes; ulIndex ++)
    {
        CMD_InitUserMode(&CmdCtrl[ulIndex]);

        CmdCtrl[ulIndex].UserLevel        = UNORMAL;
        CmdCtrl[ulIndex].BaseIo.Session   = ulIndex;
        CmdCtrl[ulIndex].BaseIo.ulIFd     = NULL_LONG;
        CmdCtrl[ulIndex].BaseIo.ulOFd     = NULL_LONG;
        CmdCtrl[ulIndex].BaseIo.acRBuf[0] = 0;
        CmdCtrl[ulIndex].BaseIo.ulRLen    = 0;
        CmdCtrl[ulIndex].BaseIo.acSBuf[0] = 0;
        CmdCtrl[ulIndex].BaseIo.ulSLen    = 0;
        CmdCtrl[ulIndex].BaseIo.cmd_send  = CMD_SendData;
        CmdCtrl[ulIndex].BaseIo.cmd_recv  = CMD_RecvData;

        for(ulTemp = 0; ulTemp < HIST_NUM; ulTemp++)
        {
            CmdCtrl[ulIndex].History[ulTemp][0] = 0;
        }

        CmdCtrl[ulIndex].ulHisNums   = 0;
        CmdCtrl[ulIndex].ulHisHead   = 0;
        CmdCtrl[ulIndex].ulHisTail   = 0;
        CmdCtrl[ulIndex].ulCurHisPos = 0;
        CmdCtrl[ulIndex].acEBuf[0]   = 0;
        CmdCtrl[ulIndex].ulELen      = 0;
        CmdCtrl[ulIndex].ulESC       = NULL_LONG;

        for (ulTemp = 0; ulTemp < TREE_DEPTH; ulTemp++)
        {
            CmdCtrl[ulIndex].CmdTree[ulTemp] = NULL;
        }

        CmdCtrl[ulIndex].TreeDep    = 0;
        CmdCtrl[ulIndex].ulParaBLen = 0;
        CmdCtrl[ulIndex].ParaBuf[0] = 0;
        CmdCtrl[ulIndex].ParaNum    = 0;

        for (ulTemp = 0; ulTemp < PARA_NUM; ulTemp++)
        {
            CmdCtrl[ulIndex].CurPara[ulTemp] = NULL;
        }

        CmdCtrl[ulIndex].ulPromLen = 0;
        CmdCtrl[ulIndex].ulLines   = 0;
        CmdCtrl[ulIndex].ulScroll  = PAGE_SCROLL;
        CmdCtrl[ulIndex].ulQuit    = FALSE;
        CmdCtrl[ulIndex].usPing    = FALSE;
        CmdCtrl[ulIndex].usString  = NONE_QUOT;
        CmdCtrl[ulIndex].ulLogin   = FALSE;
    }

    return CMD_OK;
}

ULONG CMD_InitPromptInfo()
{
    if (NULL != CmdPrompt)
    {
        return CMD_OK;
    }

    CmdPrompt = calloc(PROMPT_LEN * 2, sizeof(CHAR));

    if (NULL == CmdPrompt)
    {
        return CMD_ERROR;
    }

        strcpy(CmdPrompt, PROMPT_DEF);

    return CMD_OK;
}

VOID CMD_ResetCtrlInfo(ULONG ulSes)
{
    ULONG ulIndex;
    //BYTE aucCommand[MSG_LEN_MAX] = {0};
    //BYTE aucResponse[MSG_LEN_MAX] = {0};
    //BYTE aucMsgBuf[MSG_LEN_MAX] = {0};
    
    CMD_SetUserMode(ulSes, INIT_MODE);
    CmdCtrl[ulSes].ulQuit      = FALSE;

    CmdCtrl[ulSes].ulHisNums   = 0;
    CmdCtrl[ulSes].ulHisHead   = 0;
    CmdCtrl[ulSes].ulHisTail   = 0;
    CmdCtrl[ulSes].ulCurHisPos = 0;

    for (ulIndex = 0; ulIndex < HIST_NUM; ulIndex++)
    {
        CmdCtrl[ulSes].History[ulIndex][0] = 0;
    }

    CmdCtrl[ulSes].ulLines  = 0;
    CmdCtrl[ulSes].usPing   = FALSE;
    CmdCtrl[ulSes].usString = NONE_QUOT;
    CmdCtrl[ulSes].ulESC    = NULL_LONG;
    CmdCtrl[ulSes].ulELen   = 0;
}

ULONG CMD_GetUserName(ULONG ulSes, CHAR *pcBuf, ULONG ulLen)
{
    ULONG ulCount = 0;

    for (;;)
    {
        CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd, &pcBuf[ulCount], 1);

        CmdCtrl[ulSes].ulTrig = tickGet();

        if (pcBuf[ulCount] == '\n'|| pcBuf[ulCount] == '\r')
        {
            pcBuf[ulCount] = 0;
            break;
        }
        else if (pcBuf[ulCount] >= 0x20)
        {
            if (ulCount >= ulLen - 1)
            {
                continue;
            }
            else
            {
                CMD_OutputChar(ulSes, pcBuf[ulCount]);
                ulCount++;
            }
        }
        else if (pcBuf[ulCount] == '\b')
        {
            if (ulCount > 0)
            {
                ulCount--;
                CMD_DeletePrevChar(ulSes);
            }
        }
        else
        {
            continue;
        }
    }

    return CMD_OK;
}

ULONG CMD_GetUserPWord(ULONG ulSes, CHAR *pcBuf, ULONG ulLen)
{
    ULONG ulCount = 0;

    for (; ;)
    {
        CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd, &pcBuf[ulCount], 1);

        CmdCtrl[ulSes].ulTrig = tickGet();

        if (pcBuf[ulCount] == '\n'|| pcBuf[ulCount] == '\r')
        {
            pcBuf[ulCount] = 0;
            break;
        }
        else if (pcBuf[ulCount] == '\b')
        {
            if (ulCount > 0)
            {
                ulCount--;
                CMD_DeletePrevChar(ulSes);
            }
        }
        else
        {
            if (ulCount >= ulLen - 1)
            {
                continue;
            }
            else
            {
                CMD_OutputChar(ulSes, '*');
                ulCount++;
            }
        }
    }

    return CMD_OK;
}

ULONG CMD_GetSuperUserPWord(ULONG ulSes, CHAR *pcBuf, ULONG ulLen)
{
    ULONG ulCount = 0;

    for (; ;)
    {
        CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd, &pcBuf[ulCount], 1);

        CmdCtrl[ulSes].ulTrig = tickGet();

        if (pcBuf[ulCount] == '\n'|| pcBuf[ulCount] == '\r')
        {
            pcBuf[ulCount] = 0;
            break;
        }
        else if (pcBuf[ulCount] == '\b')
        {
            if (ulCount > 0)
            {
                ulCount--;
                //CMD_DeletePrevChar(ulSes);
            }
        }
        else
        {
            if (ulCount >= ulLen - 1)
            {
                continue;
            }
            else
            {
                ulCount++;
            }
        }
    }

    return CMD_OK;
}

VOID CMD_TimerEntry(ULONG *ulTime)
{
    ULONG ulDelay;
    ULONG ulIndex;
    ULONG ulCurTick;

    ulDelay = (0 == ulTime) ? 1000 : *ulTime;

    for (; ;)
    {
        taskDelay(ulDelay);

        ulCurTick = tickGet();

        for (ulIndex = 1; ulIndex <= CLI_MAX_TELNET; ulIndex ++)
        {
            if (FALSE == CmdCtrl[ulIndex].ulLogin)
            {
                continue;
            }
            else
            {
                if ((ulCurTick - CmdCtrl[ulIndex].ulTrig) > TOUT_VALUE)
                {
                    CmdCtrl[ulIndex].ulQuit = TRUE;
                    //telnet_exit(ulIndex);
                    CmdCtrl[ulIndex].ulLogin    = FALSE;
                }
            }
        }

        if (CmdCtrl[CLI_CONSOLE_ID].CurMode.ucOne != GENE_MODE1)
        {
            if ((ulCurTick - CmdCtrl[CLI_CONSOLE_ID].ulTrig) > TOUT_VALUE)
            {
                CMD_SetUserMode(CLI_CONSOLE_ID, INIT_MODE);
            }
        }
    }
}

VOID CMD_Entry(ULONG ulSes, ULONG ulIFd, ULONG ulOFd)
{
    int lRead;

    if (ulSes >= m_MaxSession)
    {
        return;
    }

    CmdCtrl[ulSes].BaseIo.ulIFd   = ulIFd;
    CmdCtrl[ulSes].BaseIo.ulOFd   = ulOFd;
    CmdCtrl[ulSes].BaseIo.Session = ulSes;
    CmdCtrl[ulSes].ulTrig         = tickGet();

    CmdCtrl[ulSes].ulLogin = TRUE;

    for (; ;)
    {
        CMD_ClearScreen(ulSes);
        CMD_WelComeInfo(ulSes);
        CMD_ResetCtrlInfo(ulSes);
        CMD_DisplayPrompt(ulSes);

        while ( CmdCtrl[ulSes].ulQuit == FALSE )
        {
            
            lRead = CmdCtrl[ulSes].BaseIo.cmd_recv(CmdCtrl[ulSes].BaseIo.ulIFd,
                                           CmdCtrl[ulSes].BaseIo.acRBuf, 1);

           if(CMD_ERROR == lRead)
            {
                return;
            }
             
            CmdCtrl[ulSes].ulTrig = tickGet();

            CmdCtrl[ulSes].BaseIo.ulRLen = 1;

            CMD_LexProcess(ulSes);

        }
    }
}

ULONG CMD_Init(ULONG ulClient)
{
    if (m_CmdInitilized)
    {
        return OK;
    }

#ifdef _DEBUG_OPEN
    CMD_DebugOpen();
#endif

    m_MaxSession = ulClient;

    if (CMD_OK != CMD_InitCtrlInfo(ulClient))
    {
        return ERROR;
    }

    if (CMD_OK != CMD_TreeInit())
    {
        return ERROR;
    }

    if (CMD_OK != CMD_InitPromptInfo())
    {
        return ERROR;
    }

    CMD_BuildTree(CmdBasicTable);

    CMD_BuildTree(CmdSdvTestTable);

    m_CmdInitilized = TRUE;
    
    return OK;
}

