/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_cmn.c
* Description:   common
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include <math.h>
#include <ctype.h>

#include "util_cmn.h"
#include "util_log.h"



#define DEF_GET_PROC_NAME                        ("grep 'Name:' /proc/%d/status | awk '{print $2}'")
#define DEF_READ_PROC_NAME_PATH                  ("/proc/self/exe")

#define DEF_CMD_TRY_TIME                         (5)
#define DEF_CHAR_NEW_LINE                        ('\n')
#define DEF_EXECUTE_CMD_TIMEOUT                  (500)                      /*unit: ms                                                 */
#define DEF_CMD_LARGE_TRY_TIME                   (16)
/* case the PIPE page size is 4K, we set the read max len to 4K                                   */
#define DEF_PIPE_PAGE_SIZE                       (4 * 1024)

#define DEF_MTD_NO_STR                  "/dev/mtdblock"

static pthread_mutex_t m_stUtilMutex = PTHREAD_MUTEX_INITIALIZER;

static const uint32_t m_aulMtdSize[] = {
    DEF_MTD_LEN_A__RCW,
    DEF_MTD_LEN_A_BOOT,
    DEF_MTD_LEN_A__ENV,
    DEF_MTD_LEN_A__PPA,
    DEF_MTD_LEN_A_FMAN,
    DEF_MTD_LEN_A_HEAD,
    DEF_MTD_LEN_A___OS,
    DEF_MTD_LEN_A__APP,

    DEF_MTD_LEN_B__RCW,
    DEF_MTD_LEN_B_BOOT,
    DEF_MTD_LEN_B__ENV,
    DEF_MTD_LEN_B__PPA,
    DEF_MTD_LEN_B_FMAN,
    DEF_MTD_LEN_B_HEAD,
    DEF_MTD_LEN_B___OS,
    DEF_MTD_LEN_B__APP,

    DEF_MTD_LEN_RESV,
    DEF_MTD_LEN__LOG,
    DEF_MTD_LEN___PM,
};

int
UTIL_ExecuteCmd(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs)
{
    int iRet = OPLK_ERR;
    int iPopenFD = -1;
    int iMaxFD = -1;
    fd_set stAllSet;
    fd_set stFdSet;
    struct timeval stTimeout;
    FILE *pstFd = NULL;
    uint32_t i = 0;
    int32_t lLen = 0;
    char acLogBuf[DEF_BUF_SIZE_128] = {0};
    uint32_t ulTimeout = 0;

    if (NULL == pcCmd || NULL == pcResp || 0 == ulRespLen) {
        goto INPUT_ERR_LABEL;
    }

    /* default timeout 500ms                                                    */
    if (0 == ulTimeoutMs) {
        ulTimeout = DEF_EXECUTE_CMD_TIMEOUT;
    } else {
        ulTimeout = ulTimeoutMs;
    }

    pstFd = popen(pcCmd, "r");
    if (NULL == pstFd) {
        goto ERR_LABEL;
    }

    iPopenFD = fileno(pstFd);
    iMaxFD = iPopenFD;

    FD_ZERO(&stAllSet);
    FD_SET(iPopenFD, &stAllSet);

    for (i = 0; i < DEF_CMD_TRY_TIME; i++) {
        stFdSet = stAllSet;

        /* ms.                                                                  */
        stTimeout.tv_sec = 0;
        stTimeout.tv_usec = (ulTimeout * 1000);

        iRet = select(iMaxFD + 1, &stFdSet, NULL, NULL, &stTimeout);
        if (0 > iRet) {
            goto ERR_LABEL;
        } else if (0 == iRet) {
            continue;                                                       /*timeout                                                  */
        }

        /*select return > 0                                                     */
        if ((iPopenFD > 0) && (FD_ISSET(iPopenFD, &stFdSet))) {
            if ((lLen = read(iPopenFD, pcResp, ulRespLen)) > 0) {
                /* remove new line character                                    */
                if (lLen >= ulRespLen) {
                    pcResp[lLen - 1] = 0;
                } else {
                    if (DEF_CHAR_NEW_LINE == pcResp[lLen - 1]) {
                        pcResp[lLen - 1] = 0;
                    }
                }
                break;
            }
        }
    }

    DEF_SAFE_PCLOSE(iRet, pstFd);
    if (OPLK_ERR == iRet) {
        goto ERR_LABEL;
    }

    if (i >= DEF_CMD_TRY_TIME) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

INPUT_ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL || 0  == ulRespLen");
    return OPLK_ERR;

ERR_LABEL:

    DEF_SAFE_PCLOSE(iRet, pstFd);
    if (NULL == pstFd || OPLK_ERR == iRet) {
        strncpy(pcResp, strerror(errno), ulRespLen);
        memset(acLogBuf, 0x00, sizeof(acLogBuf));
        strncpy(acLogBuf, pcResp, DEF_BUF_SIZE_128 - 1);
        LOG_WRITE(EN_LOG_ERROR, "ExecuteCmd %s Error: %s", pcCmd, acLogBuf);
    }

    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_ExecuteCmdLargeResp                                                              */
/* Descrp : This function is nearly same with the  UTIL_ExecuteCmd. When ulRespLen exceeds 4096   */
/*          UTIL_ExecuteCmd ONLY read one time, so some information is LOST.                      */
/* Input  : pcCmd -- A pointer of process command                                                 */
/* Input  : pcResp -- A pointer of response buffer                                                */
/* Output : ulRespLen -- response buffer len                                                      */
/* Input  : ulTimeoutMs -- timeout ms                                                             */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_ExecuteCmdLargeResp(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs)
{
    int iRet = OPLK_ERR;

    /* PIPE information                                                         */
    FILE *fpPipe = NULL;
    int iPopenFD = -1;
    int iMaxFD = -1;

    /* select information                                                       */
    fd_set stAllSet;
    fd_set stFdSet;
    struct timeval stTimeout;
    uint32_t ulTimeout = 0;

    /* retry read information                                                   */
    int32_t iRetryIdx = 0;
    int32_t lLen = 0;
    int32_t lLeftLen = 0;
    int32_t lTmpLen = 0;
    int32_t lReadLenOneTime = 0;

    /* check input parameter.                                                   */
    if ((NULL == pcCmd) ||
        (NULL == pcResp) ||
        (0 == ulRespLen)) {
        LOG_WRITE(EN_LOG_ERROR, "pcCmd: 0x%p, pcResp: 0x%p, ulRespLen: %hu", pcCmd, pcResp, ulRespLen);
        goto INPUT_ERR_LABEL;
    }

    /* default timeout 500ms                                                    */
    ulTimeout = (0 == ulTimeoutMs ? DEF_EXECUTE_CMD_TIMEOUT : ulTimeoutMs);

    /* open PIPE                                                                */
    fpPipe = popen(pcCmd, "r");
    if (NULL == fpPipe) {
        goto ERR_LABEL;
    }

    /* get the file descriptor, we will use read function to get information    */
    iPopenFD = fileno(fpPipe);
    iMaxFD = iPopenFD;

    /* clear fd set.                                                            */
    FD_ZERO(&stAllSet);
    FD_SET(iPopenFD, &stAllSet);

    /* set the tmplen and left len                                              */
    lTmpLen = 0;
    lLeftLen = ulRespLen;

    /* read function                                                            */
    while (true) {
        if (iRetryIdx++ >= DEF_CMD_LARGE_TRY_TIME) {
            break;
        }

        /* reset stFdSet                                                        */
        stFdSet = stAllSet;

        /* ms.                                                                  */
        stTimeout.tv_sec = 0;
        stTimeout.tv_usec = (ulTimeout * 1000);

        iRet = select(iMaxFD + 1, &stFdSet, NULL, NULL, &stTimeout);
        if (0 > iRet) {
            LOG_WRITE(EN_LOG_ERROR, "Select error: %s", strerror(errno));
            goto ERR_LABEL;
        } else if (0 == iRet) {
            iRetryIdx++;
            continue;                                                       /*timeout                                                  */
        }

        /*select return > 0                                                     */
        if ((iPopenFD > 0) && (FD_ISSET(iPopenFD, &stFdSet))) {
            if (lLeftLen < DEF_PIPE_PAGE_SIZE) {
                lReadLenOneTime = lLeftLen;
            } else {
                lReadLenOneTime = DEF_PIPE_PAGE_SIZE;
            }

            lLen = read(iPopenFD, pcResp + lTmpLen, lReadLenOneTime);
            if (lLen > 0) {
                lTmpLen += lLen;
                lLeftLen -= lLen;
                if (lLeftLen <= 0) {
                    break;
                }
            } else if (0 == lLen) {
                break;
            } else {
                if (errno != EAGAIN) {
                    LOG_WRITE(EN_LOG_ERROR, "Read error: %s", strerror(errno));
                    break;
                }
            }
        }
    }

    pcResp[lTmpLen - 1] = '\0';

    DEF_SAFE_PCLOSE(iRet, fpPipe);
    if (OPLK_ERR == iRet) {
        LOG_WRITE(EN_LOG_ERROR, "Fclose: %s error: %s", pcCmd, strerror(errno));
        goto ERR_LABEL;
    }

    if (iRetryIdx >= DEF_CMD_LARGE_TRY_TIME) {
        LOG_WRITE(EN_LOG_ERROR, "ExecuteCmd %s try idx: %d", pcCmd, iRetryIdx);
        goto ERR_LABEL;
    }

    return OPLK_OK;

INPUT_ERR_LABEL:

    return OPLK_ERR;

ERR_LABEL:

    DEF_SAFE_PCLOSE(iRet, fpPipe);

    return OPLK_ERR;
}

int
UTIL_ExceCmdUnresp(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs)
{
    int iRet = OPLK_ERR;
    int iPopenFD = -1;
    int iMaxFD = -1;
    fd_set stAllSet;
    fd_set stFdSet;
    struct timeval stTimeout;
    FILE *pstFd = NULL;
    int32_t lLen = 0;
    char acLogBuf[DEF_BUF_SIZE_128] = {0};
    uint32_t ulTimeout = 0;

    if (NULL == pcCmd || NULL == pcResp || 0 == ulRespLen) {
        goto INPUT_ERR_LABEL;
    }

    /* default timeout 500ms                                                    */
    if (0 == ulTimeoutMs) {
        ulTimeout = DEF_EXECUTE_CMD_TIMEOUT;
    } else {
        ulTimeout = ulTimeoutMs;
    }

    pstFd = popen(pcCmd, "r");
    if (NULL == pstFd) {
        goto ERR_LABEL;
    }

    iPopenFD = fileno(pstFd);
    iMaxFD = iPopenFD;

    FD_ZERO(&stAllSet);
    FD_SET(iPopenFD, &stAllSet);
    stFdSet = stAllSet;

    /* ms.                                                                      */
    stTimeout.tv_sec = 0;
    stTimeout.tv_usec = (ulTimeout * 1000);

    iRet = select(iMaxFD + 1, &stFdSet, NULL, NULL, &stTimeout);
    if (0 > iRet) {
        goto ERR_LABEL;
    } else if (0 == iRet) {
        LOG_WRITE(EN_LOG_NOTICE, "cmd timeout unresponse");
        goto OK_LABEL;                                                      /*timeout                                                  */
    }

    /*select return > 0                                                         */
    if ((iPopenFD > 0) && (FD_ISSET(iPopenFD, &stFdSet))) {
        if ((lLen = read(iPopenFD, pcResp, ulRespLen)) > 0) {
            /* remove new line character                                        */
            if (lLen >= ulRespLen) {
                pcResp[lLen - 1] = 0;
            } else {
                if (DEF_CHAR_NEW_LINE == pcResp[lLen - 1]) {
                    pcResp[lLen - 1] = 0;
                }
            }
        }
    }

OK_LABEL:

    DEF_SAFE_PCLOSE(iRet, pstFd);
    if (OPLK_ERR == iRet) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

INPUT_ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL || 0  == ulRespLen");
    return OPLK_ERR;

ERR_LABEL:

    DEF_SAFE_PCLOSE(iRet, pstFd);
    if (NULL == pstFd || OPLK_ERR == iRet) {
        strncpy(pcResp, strerror(errno), ulRespLen);
        memset(acLogBuf, 0x00, sizeof(acLogBuf));
        strncpy(acLogBuf, pcResp, DEF_BUF_SIZE_128 - 1);
        LOG_WRITE(EN_LOG_WARNING, "ExecuteCmd %s Error: %s", pcCmd, acLogBuf);
    }

    return OPLK_ERR;
}


/***FUNC+******************************************************************************************/
/* Name   : UTIL_ExportProcessInfo                                                                */
/* Descrp : export process infomation to file name-id-fd.info                                     */
/* Input  : pcProcessName -- A pointer of process name                                            */
/* Output : NULL                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_ExportProcessInfo(char *pcProcessName)
{
    int iRet = -1;

    /* PIPE for command                                                         */
    FILE *pstPipe = NULL;
    char acLine[DEF_BUF_SIZE_1024] = {0};

    /* system shell command                                                     */
    char acCommand[DEF_BUF_SIZE_1024] = {0};

    /* process id and name.                                                     */
    int32_t lId = 0;
    char acName[DEF_BUF_SIZE_128] = {0};
    char *pcName = NULL;

    if (NULL == pcProcessName) {
        goto ERR_LABEL;
    }

    /* system shell 'ps aux' command and get the process name and ID            */
    memset(acCommand, 0x00, sizeof(acCommand));
    snprintf(acCommand, sizeof(acCommand),
             "ps aux | grep %s | grep -v grep | awk \'{print $2,$11}\'",
             pcProcessName);

    /* Open pipe to execute commands                                            */
    pstPipe = popen(acCommand, "r");
    if (NULL == pstPipe) {
        LOG_WRITE(EN_LOG_ERROR, "Popen '%s' error: %s", acCommand, strerror(errno));
        goto ERR_LABEL;
    }

    /* Gets the buffer from pipe.                                               */
    memset(acLine, 0x00, sizeof(acLine));
    while (fgets(acLine, sizeof(acLine), pstPipe) != NULL) {
        /* Get the process id and process name                                  */
        iRet = sscanf(acLine, "%d %s", &lId, acName);
        if (iRet != 2) {
            LOG_WRITE(EN_LOG_ERROR, "sscanf %s error: %s", acLine, strerror(errno));
            break;
        }

        /* Remove the prefix '/usr/local/bin/'                                  */
        pcName = strrchr(acName, '/');
        if (NULL == pcName) {
            pcName = acName;
        } else {
            pcName += strlen("/");
        }

        memset(acCommand, 0x00, sizeof(acCommand));
        snprintf(acCommand, sizeof(acCommand),
                 "ls -l /proc/%d/exe > /var/log/%s-%d-fd.info; ls -l /proc/%d/fd/* >> /var/log/%s-%d-fd.info",
                 lId, pcName, lId, lId, pcName, lId);
        iRet = UTIL_System(acCommand);
        if (iRet != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "acCommand: %s error", acCommand);
        }

        memset(acLine, 0x00, sizeof(acLine));
        DEF_SLEEP_MS(1);
    }

    DEF_SAFE_PCLOSE(iRet, pstPipe);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_PCLOSE(iRet, pstPipe);
    iRet = iRet;                                                            /* just for compilation                                    */
    return OPLK_ERR;
}


char *
UTIL_UpperStr(char *pcStr)
{
    size_t i = 0, nLen = 0;

    if (NULL == pcStr) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        return NULL;
    }

    nLen = strlen(pcStr);
    for (i = 0; i < nLen; ++i) {
        pcStr[i] = toupper(pcStr[i]);
    }

    return pcStr;
}

char *
UTIL_LowerStr(char *pcStr)
{
    size_t i = 0, nLen = 0;

    if (NULL == pcStr) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        return NULL;
    }

    nLen = strlen(pcStr);
    for (i = 0; i < nLen; ++i) {
        pcStr[i] = tolower(pcStr[i]);
    }

    return pcStr;
}

void
UTIL_DelDigital(char acX[])
{
    int i = 0, j = 0;

    while (acX[i] != '\0') {
        if (acX[i] >= '0' && acX[i] <= '9') {
            i++;
        } else {
            acX[j] = acX[i];
            i++;
            j++;
        }
    }

    acX[j] = '\0';

    return;
}

static int
__UTIL_IsSpecialChar(char cChar)
{
    static char m_acSpecialChars[] = {
        '`', '-', '=', '[', ']', ';', '\'', ',', '.', '/',
        '~', '!', '@', '#', '$', '^', '&', '*', '(', ')',
        '_', '+', '{', '}', '|', ':', '<', '>', '\?'
    };

    uint32_t ulLen = 0;
    uint32_t i = 0;

    ulLen = sizeof(m_acSpecialChars) / sizeof(m_acSpecialChars[0]);

    for (i = 0; i < ulLen; i++) {
        if (m_acSpecialChars[i] == cChar) {
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}

int
UTIL_ChkStrSpecChr(char *pcStr)
{
    uint16_t usLetter = 0x00;

    if (NULL == pcStr || 0 == strlen(pcStr)) {
        LOG_WRITE(EN_LOG_ERROR, "input pcStr == NULL");
        goto ERR_LABEL;
    }

    while (*pcStr) {
        if (islower(*pcStr)) {
            usLetter |= DEF_IS_LOWER_ALPHA;
        } else if (isupper(*pcStr)) {
            usLetter |= DEF_IS_UPPER_ALPHA;
        } else if (isdigit(*pcStr)) {
            usLetter |= DEF_IS_DIGIT;
        } else if (isprint(*pcStr)) {
            usLetter |= DEF_IS_PRINT;
            if (OPLK_OK != __UTIL_IsSpecialChar(*pcStr)) {
                goto ERR_LABEL;
            }
        } else {
            goto ERR_LABEL;
        }
        pcStr++;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_ChkStrNumberLetter(char *pcStr)
{
    uint16_t usLetter = 0x00;

    if (NULL == pcStr || 0 == strlen(pcStr)) {
        LOG_WRITE(EN_LOG_ERROR, "input pcStr is NULL or empty");
        goto ERR_LABEL;
    }

    while (*pcStr) {
        if (islower(*pcStr)) {
            usLetter |= DEF_IS_LOWER_ALPHA;
        } else if (isupper(*pcStr)) {
            usLetter |= DEF_IS_UPPER_ALPHA;
        } else if (isdigit(*pcStr)) {
            usLetter |= DEF_IS_DIGIT;
        } else {
            goto ERR_LABEL;
        }
        pcStr++;
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}


/* support all special chars, except for space character                                          */
static const char m_acAllSpecialChars[] =
{'`', '-', '=', '[', ']', ';', '\'', ',', '.', '/',
 '~', '!', '@', '#', '$', '^', '&', '*', '(', ')',
 '_', '+', '{', '}', '|', ':', '<', '>', '\?', '%',
 '\"', '\\'};

static int
__IsSpecialCharByRangeList(char cChar, const char *pcSpecialCharList, size_t nCount)
{
    size_t i = 0;

    if (NULL == pcSpecialCharList) {
        return -1;
    }

    for (i = 0; i < nCount; i++) {
        if (*(pcSpecialCharList + i) == cChar) {
            return 0;
        }
    }

    return -1;
}

#define IS_LOWER_ALPHA          (0x1000)
#define IS_UPPER_ALPHA          (0x0100)
#define IS_DIGIT                (0x0010)
#define IS_PRINT                (0X0001)

int
UTIL_CheckStrWithAllSpecChars(char *pcString)
{
    int iLetter = 0x0;

    if (NULL == pcString) {
        goto ERR_LABEL;
    }

    while (*pcString) {
        if (islower(*pcString)) {
            iLetter |= IS_LOWER_ALPHA;
        } else if (isupper(*pcString)) {
            iLetter |= IS_UPPER_ALPHA;
        } else if (isdigit(*pcString)) {
            iLetter |= IS_DIGIT;
        } else if (isprint(*pcString)) {
            iLetter |= IS_PRINT;
            if (0 != __IsSpecialCharByRangeList(*pcString, m_acAllSpecialChars, sizeof(m_acAllSpecialChars) / sizeof(m_acAllSpecialChars[0]))) {
                goto ERR_LABEL;
            }
        } else {
            goto ERR_LABEL;
        }
        pcString++;
    }

    /* TBD                                                                      */

    return 0;

ERR_LABEL:

    return -1;
}

int
UTIL_TranslateStrForExpect(const char *pcIn, char *pcOut, size_t nOutLen)
{
    char *pcPos = pcOut;
    size_t i = 0, nInLen = 0;
    if (NULL == pcIn || NULL == pcOut) {
        return -1;
    }

    /* check if output buffer have enough space                                 */
    nInLen = strlen(pcIn);
    if ((0 == nInLen) || (nOutLen < 2 * nInLen + 1)) {

        return -1;
    }

    for (i = 0; i < nInLen; i++) {
        switch (*pcIn) {
        case '~':
        case '&':
        case '`':
        case '|':
        case '\\':
        case '(':
        case ')':
        case ';':
        case '<':
        case '>':
        case '\'':
        case '"':
        case '!':
        case '$':
            *pcPos++ = '\\';
            *pcPos++ = *pcIn;
            break;
        default:
            *pcPos++ = *pcIn;
            break;
        }

        pcIn++;
    }

    /* string termination character                                             */

    *pcPos = '\0';

    return 0;
}

int
UTIL_GetProcName(char *pcProcName, uint32_t ulLen)
{
    int iRet = -1;
    char acProcPath[DEF_BUF_SIZE_128] = {0};
    char *pcName = NULL;

    if (NULL == pcProcName) {
        LOG_WRITE(EN_LOG_ERROR, "input pcProcName == NULL");
        goto ERR_LABEL;
    }

    memset(acProcPath, 0x00, DEF_BUF_SIZE_128);
    iRet = readlink(DEF_READ_PROC_NAME_PATH, acProcPath, DEF_BUF_SIZE_128 - 1);
    if (0 > iRet) {
        LOG_WRITE(EN_LOG_ERROR, "readlink path %s failure", DEF_READ_PROC_NAME_PATH);
        goto ERR_LABEL;
    }

    pcName = strrchr(acProcPath, '/');
    if (NULL == pcName) {
        pcName = acProcPath;
    }
    /* del the first '/'                                                        */
    pcName++;

    memcpy(pcProcName, pcName, (ulLen > strlen(pcName)) ? strlen(pcName) : ulLen);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

void
UTIL_AscToBcd(char *pcASC, char *pcBCD, uint32_t ulLen)
{
    uint32_t i = 0;
    uint8_t ucCh1 = 0, ucCh2 = 0;

    if (NULL == pcASC || NULL == pcBCD) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        return;
    }

    for (i = 0; i < ulLen / 2; i++) {
        ucCh1 = (uint8_t)pcASC[i * 2];
        ucCh2 = (uint8_t)pcASC[i * 2 + 1];

        if (ucCh1 >= 'a' && ucCh1 <= 'f') {
            ucCh1 = ucCh1 - 'a' + 0xa;
        } else if (ucCh1 >= 'A' && ucCh1 <= 'F') {
            ucCh1 = ucCh1 - 'A' + 0xa;
        } else {
            ucCh1 = ucCh1 - '0';
        }

        if (ucCh2 >= 'a' && ucCh2 <= 'f') {
            ucCh2 = ucCh2 - 'a' + 0xa;
        } else if (ucCh2 >= 'A' && ucCh2 <= 'F') {
            ucCh2 = ucCh2 - 'A' + 0xa;
        } else {
            ucCh2 = ucCh2 - '0';
        }
        pcBCD[i] = (ucCh1 << 4) | ucCh2;

    }

    return;
}

char *
UTIL_BcdToAsc(char *pcBCD, uint32_t ulLen, char *pcASC)
{
    const char acAscii[] = "0123456789ABCDEF";
    uint32_t i = 0;

    if (NULL == pcBCD || NULL == pcASC) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        return NULL;
    }

    for (i = 0; i < ulLen; i++) {
        pcASC[2 * i] = acAscii[(pcBCD[i] >> 4)];
        pcASC[2 * i + 1] = acAscii[(pcBCD[i] & 0x0F)];
    }

    return pcASC;
}

int
UTIL_AscToHex(const char *pcstr, char *pcHexStr, uint32_t ulLen)
{
    char *p = NULL, *pcHexstrTmp = pcHexStr;
    uint32_t ulSize = 0, ulLenTmp = ulLen;

    if (NULL == pcstr || NULL == pcHexStr) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        return OPLK_ERR;
    }

    for (p = (char *)pcstr; *p != 0; p++) {
        ulSize = snprintf(pcHexstrTmp, ulLen, "%02x", *p);
        ulLenTmp -= ulSize;
        if (ulLenTmp <= 0) {
            return OPLK_ERR;
        }
        pcHexstrTmp += ulSize;
    }

    *pcHexstrTmp = 0;

    return ulLen - ulLenTmp;
}

int
UTIL_StrToInt(char *pcSrc, uint32_t ulMultiple, int *piNum)
{
    char cTmp = 0;
    uint32_t i = 0, ulLen = 0, ulOffset = 0;
    int iDate = 0, iFlag = 0;

    if (NULL == pcSrc || NULL == piNum) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr == NULL");
        goto ERR_LABEL;
    }

    ulLen = strlen(pcSrc);
    iFlag = 0;
    for (i = 0; i < ulLen; i++) {
        if ('.' == pcSrc[i]) {
            if (1 == iFlag) {
                *piNum = 0xFFFFFFFF;
                LOG_WRITE(EN_LOG_ERROR, "'.' == pcSrc[i]");
                goto ERR_LABEL;
            }
            iFlag = 1;
        }
    }

    i = 0;
    if ('-' == pcSrc[i]) {
        i++;
    }
    if (!DEF_IS_NUMBER(pcSrc[i])) {
        *piNum = 0xFFFFFFFF;
        LOG_WRITE(EN_LOG_ERROR, "DEF_IS_NUMBER %d", pcSrc[i]);
        goto ERR_LABEL;
    }
    i++;

    for (; i < ulLen; i++) {
        if ('.' == pcSrc[i]) {
            cTmp = pcSrc[i];
            pcSrc[i] = pcSrc[i + 1];
            pcSrc[i + 1] = cTmp;
            ulOffset++;
        }

        if (0x00 == pcSrc[i]) {
            if (0 != ulOffset) {
                ulOffset--;
            }
            break;
        } else if (!DEF_IS_NUMBER(pcSrc[i])) {
            LOG_WRITE(EN_LOG_ERROR, "DEF_IS_NUMBER %d", pcSrc[i]);
            *piNum = 0xFFFFFFFF;
            goto ERR_LABEL;
        }

    }

    if (ulOffset > ulMultiple) {
        *piNum = 0xFFFFFFFF;
        LOG_WRITE(EN_LOG_ERROR, "ulOffset > ulMultiple");
        goto  ERR_LABEL;
    }

    iDate = atoi(pcSrc);
    for (; ulOffset < ulMultiple; ulOffset++) {
        iDate = iDate * 10;
    }

    *piNum = iDate;

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_StrToFloat(char *pcSrc, size_t nMaxFractionDigits, double *pdNum)
{
    bool bFlag = false;
    double dNum = 0;
    size_t i = 0, nLen = 0;
    size_t nFractionDigits = 0;

    if (NULL == pcSrc || nMaxFractionDigits >= 6) {
        return OPLK_ERR;
    }

    nLen = strlen(pcSrc);
    bFlag = false;
    for (i = 0; i < nLen; i++) {
        if ('.' == pcSrc[i]) {
            if (bFlag) {
                return OPLK_ERR;
            }
            bFlag = true;
        }
    }

    i = 0;
    if ('-' == pcSrc[i]) {
        i++;
    }
    if (!DEF_IS_NUMBER(pcSrc[i])) {
        return OPLK_ERR;
    }
    i++;

    bFlag = false;
    nFractionDigits = 0;
    for (; i < nLen; i++) {
        if ('.' == pcSrc[i]) {
            bFlag = true;
            continue;
        }

        if (!DEF_IS_NUMBER(pcSrc[i])) {
            return OPLK_ERR;
        }

        if (bFlag) {
            nFractionDigits++;
        }

    }

    if (nFractionDigits > nMaxFractionDigits) {
        return OPLK_ERR;
    }

    dNum = atof(pcSrc);
    *pdNum = dNum;

    return OPLK_OK;
}

char *
UTIL_StrRpc(char *pcStr, char *pcOldStr, char *pcNewStr)
{
    uint32_t ulLen = 0;
    uint32_t ulRpcLen = 0;
    char *pcBuf = NULL;
    int i = 0;

    if (NULL == pcStr || NULL == pcOldStr || NULL == pcNewStr) {
        return NULL;
    }

    ulLen = strlen(pcStr);
    ulRpcLen = strlen(pcOldStr);

    if (ulRpcLen < strlen(pcNewStr)) {
        ulLen += strlen(pcNewStr);
    }

    /* malloc RAM                                                               */
    if (NULL == (pcBuf = (char *)malloc(ulLen + 1))) {
        return NULL;
    }

    memset(pcBuf, 0, ulLen + 1);
    for (i = 0; i < ulLen; i++) {
        if (!strncmp(pcStr + i, pcOldStr, ulRpcLen)) {
            strcat(pcBuf, pcNewStr);
            i += ulRpcLen - 1;
        } else {
            strncat(pcBuf, pcStr + i, 1);
        }
    }

    strcpy(pcStr, pcBuf);
    DEF_SAFE_FREE(pcBuf);

    return pcStr;
}

char *
UTIL_TimeToStr(time_t timeT, char *time_str)
{
    struct tm tm;
    tm = *localtime(&timeT);
    sprintf(time_str, "%d-%02d-%02dT%02d:%02d:%02dZ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return time_str;
}

int
UTIL_System(char *pcCmdBuf)
{
    int iStatus = -1;

    if (NULL == pcCmdBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pcCmdBuf");
        goto ERR_LABEL;
    }

    iStatus = system(pcCmdBuf);
    if (0 > iStatus) {
        LOG_WRITE(EN_LOG_ERROR, "system Status error, cmd %s", pcCmdBuf);
        goto ERR_LABEL;
    }

    if (0 == WIFEXITED(iStatus)) {
        LOG_WRITE(EN_LOG_ERROR, "exit status = [%d]", WEXITSTATUS(iStatus));
        goto ERR_LABEL;
    } else {
        if (0 != WEXITSTATUS(iStatus)) {
            LOG_WRITE(EN_LOG_ERROR, "run shell script fail:%s, script exit code: %d", strerror(errno), WEXITSTATUS(iStatus));
            LOG_WRITE(EN_LOG_ERROR, "shell: %s", pcCmdBuf);
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

static int
__UTIL_MutexInit(void)
{
    static bool m_bInit = false;


    if (false == m_bInit) {
        pthread_mutex_init(&m_stUtilMutex, NULL);

        m_bInit = true;
    }

    return OPLK_OK;
}

int
UTIL_ReadMtd(uint8_t ucMtd, uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    char acMtd[32] = {0};
    FILE *pfFile = NULL;
    uint8_t *pucBufR = NULL;


    if ((ucMtd >= EN_MTD_NO_MAX) ||
        (NULL == pucBuf) ||
        (0 == ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        return OPLK_ERR;
    }

    /* Check the MTD size                                                       */
    if ((ulAddr + ulLen) > m_aulMtdSize[ucMtd]) {
        LOG_WRITE(EN_LOG_ERROR, "(ulAddr + ulLen) = 0x%08X, m_aulMtdSize[ucMtd] = 0x%08X",
                  (ulAddr + ulLen), m_aulMtdSize[ucMtd]);
        return OPLK_ERR;
    }

    __UTIL_MutexInit();

    pthread_mutex_lock(&m_stUtilMutex);

/*    if (OPLK_OK != CMN_FlashSwitch(ucMtd)) {                                                    */
/*        LOG_WRITE(EN_LOG_ERROR, "Switch to MTD%d error", ucMtd);                                */
/*        goto ERR_LABEL;                                                                         */
/*    }                                                                                           */

    strcpy(acMtd, DEF_MTD_NO_STR);
    sprintf(acMtd + strlen(DEF_MTD_NO_STR), "%d", ucMtd);

    /* Malloc RAM                                                               */
    if (NULL == (pucBufR = (uint8_t *)malloc(ulLen))) {
        goto ERR_LABEL;
    }

    if (NULL == (pfFile = fopen(acMtd, "r"))) {
        LOG_WRITE(EN_LOG_ERROR, "Open %s error", acMtd);
        goto ERR_LABEL;
    }

    memset(pucBufR, 0, ulLen);

    fseek(pfFile, ulAddr, SEEK_SET);
    if (ulLen != fread(pucBufR, sizeof(uint8_t), ulLen, pfFile)) {
        LOG_WRITE(EN_LOG_ERROR, "Read data from %s error", acMtd);
        goto ERR_LABEL;
    }

    memcpy(pucBuf, pucBufR, ulLen);

    DEF_SAFE_FREE(pucBufR);
    DEF_SAFE_FCLOSE(pfFile);
    pthread_mutex_unlock(&m_stUtilMutex);
    return OPLK_OK;

ERR_LABEL:
    DEF_SAFE_FREE(pucBufR);
    DEF_SAFE_FCLOSE(pfFile);
    pthread_mutex_unlock(&m_stUtilMutex);
    return OPLK_ERR;
}

int
UTIL_SaveMtd(uint8_t ucMtd, uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    char acMtd[32] = {0};
    FILE *pfFile = NULL;


    if ((ucMtd >= EN_MTD_NO_MAX) ||
        (NULL == pucBuf) ||
        (0 == ulLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Input parameter error");
        return OPLK_ERR;
    }

    /* Check the MTD size                                                       */
    if ((ulAddr + ulLen) > m_aulMtdSize[ucMtd]) {
        LOG_WRITE(EN_LOG_ERROR, "(ulAddr + ulLen) = 0x%08X, m_aulMtdSize[ucMtd] = 0x%08X",
                  (ulAddr + ulLen), m_aulMtdSize[ucMtd]);
        return OPLK_ERR;
    }

    __UTIL_MutexInit();

    pthread_mutex_lock(&m_stUtilMutex);

/*    if (OPLK_OK != CMN_FlashSwitch(ucMtd)) {                                                    */
/*        LOG_WRITE(EN_LOG_ERROR, "Switch to MTD%d error", ucMtd);                                */
/*        goto ERR_LABEL;                                                                         */
/*    }                                                                                           */

    strcpy(acMtd, DEF_MTD_NO_STR);
    sprintf(acMtd + strlen(DEF_MTD_NO_STR), "%d", ucMtd);

    if (NULL == (pfFile = fopen(acMtd, "w"))) {
        LOG_WRITE(EN_LOG_ERROR, "Open %s error", acMtd);
        goto ERR_LABEL;
    }

    fseek(pfFile, ulAddr, SEEK_SET);
    if (ulLen != fwrite(pucBuf, sizeof(uint8_t), ulLen, pfFile)) {
        LOG_WRITE(EN_LOG_ERROR, "Write data to %s error", acMtd);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(pfFile);
    pthread_mutex_unlock(&m_stUtilMutex);
    return OPLK_OK;

ERR_LABEL:
    DEF_SAFE_FCLOSE(pfFile);
    pthread_mutex_unlock(&m_stUtilMutex);
    return OPLK_ERR;
}

bool
UTIL_ChkProcExist(char *pcProcName)
{
    char acCmd[DEF_BUF_SIZE_256];
    char acBufTmp[DEF_BUF_SIZE_256] = {0};
    int rc = 0;

    if (NULL == pcProcName) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_ChkProcExist input ptr is null");
        return OPLK_ERR;
    }

    memset(acCmd, 0, sizeof(acCmd));
    memset(acBufTmp, 0, sizeof(acBufTmp));
    snprintf(acCmd, sizeof(acCmd), "ps -ef | grep \"%s\" | grep -v \"grep\" | awk '{print $8}'", pcProcName);

    rc = UTIL_ExceCmdUnresp(acCmd, acBufTmp, sizeof(acBufTmp), 0);
    if (rc != 0) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_ExecuteCmd:%s failed!", acCmd);
    }

    if (0 == strlen(acBufTmp)) {
        LOG_WRITE(EN_LOG_DEBUG, "process:%s not exist!", pcProcName);
        return false;
    }

    return true;
}


float
UTIL_Ntohf(float fIn)
{
    uint8_t *pData = NULL, *pDataDest = NULL;
    float fOut = 0.0;

    pData = (uint8_t *)(&fIn);
    pDataDest = (uint8_t *)(&fOut);
    pDataDest[0] = pData[3];
    pDataDest[1] = pData[2];
    pDataDest[2] = pData[1];
    pDataDest[3] = pData[0];

    return fOut;
}

bool
UTIL_htonl(uint32_t *puiBufIn, uint32_t *puiBufOut, uint32_t uiLen)
{
    uint32_t i;

    for (i = 0; i < uiLen; i++) {
        *(puiBufOut + i) = htonl(*(puiBufIn + i));
    }

    return true;
}

bool
UTIL_ntohs(uint16_t *pusBufIn, uint16_t *pusBufOut, uint32_t uiLen)
{
    uint32_t i;

    for (i = 0; i < uiLen; i++) {
        *(pusBufOut + i) = ntohs(*(pusBufIn + i));
    }

    return true;
}

bool
UTIL_ntohl(uint32_t *puiBufIn, uint32_t *puiBufOut, uint32_t uiLen)
{
    uint32_t i;

    for (i = 0; i < uiLen; i++) {
        *(puiBufOut + i) = ntohl(*(puiBufIn + i));
    }

    return true;
}


bool
UTIL_IsDoubleEqual(double dX, double dY)
{
    double dDelta = 1e-5;

    if (fabs(dX - dY) > dDelta) {
        return false;
    }

    return true;
}

bool
UTIL_IsDoubleMore(double dX, double dY)
{
    double dDelta = 1e-5;

    if (dX - dY > dDelta) {
        return true;
    }

    return false;
}

bool
UTIL_IsDoubleLess(double dX, double dY)
{
    double dDelta = 1e-5;

    if (dX - dY < -dDelta) {
        return true;
    }

    return false;
}

bool
UTIL_IsDoubleMoreEqual(double dX, double dY)
{
    double dDelta = 1e-5;

    if (dX - dY > -dDelta) {
        return true;
    }

    return false;
}

bool
UTIL_IsDoubleLessEqual(double dX, double dY)
{
    double dDelta = 1e-5;

    if (dX - dY < dDelta) {
        return true;
    }

    return false;
}

int
UTIL_GetLocalIp(const char *pcEth, char *pcIp, size_t nIpSize)
{
    int iSocketFd = -1;
    struct sockaddr_in stSin;
    struct ifreq stIfr;

    if (NULL == pcEth || NULL == pcIp) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return -1;
    }

    iSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "socket error: %d", iSocketFd);
        return iSocketFd;
    }

    strncpy(stIfr.ifr_name, pcEth, IFNAMSIZ);
    stIfr.ifr_name[IFNAMSIZ - 1] = 0;

    /* if error: No such device                                                 */
    if (ioctl(iSocketFd, SIOCGIFADDR, &stIfr) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "ioctl error");
        DEF_SAFE_CLOSE(iSocketFd);
        return -2;
    }

    memcpy(&stSin, &stIfr.ifr_addr, sizeof(stSin));
    snprintf(pcIp, nIpSize, "%s", inet_ntoa(stSin.sin_addr));

    DEF_SAFE_CLOSE(iSocketFd);
    return 0;
}

#if 0
int
UTIL_GetLocalMac(const char *pcEth, char *pcMac, size_t nMacSize)
{
    int iSocketFd = -1;
    struct ifreq stIfr;

    if (NULL == pcEth || NULL == pcMac) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return -1;
    }

    iSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "socket error: %d", iSocketFd);
        return iSocketFd;
    }

    strncpy(stIfr.ifr_name, pcEth, IFNAMSIZ);
    stIfr.ifr_name[IFNAMSIZ - 1] = 0;

    /* if error: No such device                                                 */
    if (ioctl(iSocketFd, SIOCGIFADDR, &stIfr) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "ioctl error");
        DEF_SAFE_CLOSE(iSocketFd);
        return -2;
    }

    snprintf(pcMac, nMacSize, "%02x:%02x:%02x:%02x:%02x:%02x",
             stIfr.ifr_hwaddr.sa_data[0],
             stIfr.ifr_hwaddr.sa_data[1],
             stIfr.ifr_hwaddr.sa_data[2],
             stIfr.ifr_hwaddr.sa_data[3],
             stIfr.ifr_hwaddr.sa_data[4],
             stIfr.ifr_hwaddr.sa_data[5]);

    DEF_SAFE_CLOSE(iSocketFd);
    return 0;
}
#endif

/***FUNC+******************************************************************************************/
/* Name   : UTIL_GetMeminfo                                                                       */
/* Descrp : Get meminfo                                                                           */
/* Input  : NONE                                                                                  */
/* Output : pstMemState -- A pointer of MEM information                                           */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
int
UTIL_GetMeminfo(ST_MEM_STAT *pstMemStat)
{
    FILE *fpFile = NULL;
    char acLine[DEF_BUF_SIZE_256] = {0};

    /* Check input parameter                                                    */
    if (NULL == pstMemStat) {
        LOG_WRITE(EN_LOG_ERROR, "Input null, 0x%p", pstMemStat);
        return OPLK_ERR;
    }

    /* fopen '/proc/meminfo'                                                    */
    fpFile = fopen("/proc/meminfo", "rt");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Open '/proc/meminfo' error: %s", strerror(errno));
        return OPLK_ERR;
    }

    /* The format:                                                              */
    /*   $ cat /proc/meminfo                                                    */
    /*   MemTotal:        3913080 kB                                            */
    /*   MemFree:         2853376 kB                                            */
    /*   MemAvailable:    2791544 kB                                            */
    /*   Buffers:            5884 kB                                            */
    /*   Cached:           936548 kB                                            */
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        if (!strncmp(acLine, "MemTotal:", strlen("MemTotal:"))) {
            pstMemStat->lMemTotal = atoi(acLine + strlen("MemTotal:"));
        } else if (!strncmp(acLine, "MemFree:", strlen("MemFree:"))) {
            pstMemStat->lMemFree = atoi(acLine + strlen("MemFree:"));
        } else if (!strncmp(acLine, "MemAvailable:", strlen("MemAvailable:"))) {
            pstMemStat->lMemAvailable = atoi(acLine + strlen("MemAvailable:"));
        } else if (!strncmp(acLine, "Buffers:", strlen("Buffers:"))) {
            pstMemStat->lMemBuffers = atoi(acLine + strlen("Buffers:"));
        } else if (!strncmp(acLine, "Cached:", strlen("Cached:"))) {
            pstMemStat->lMemCached = atoi(acLine + strlen("Cached:"));
            break;
        }
    }
    pstMemStat->lMemUtilized = pstMemStat->lMemTotal - pstMemStat->lMemAvailable;

    DEF_SAFE_FCLOSE(fpFile);

    return OPLK_OK;
}

int
UTIL_GetCpu1MinAvg(double *pdValue)
{
    char acBuf[64] = {0};
    int iFd = -1;
    int iRet = 0;

    if (NULL == pdValue) {
        goto ERR_LABEL;
    }

    memset(acBuf, 0x00, sizeof(acBuf));
    iFd = open("/proc/loadavg", O_RDONLY);
    if (iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error open /proc/loadavg : %d, error: %s", iFd, strerror(errno));
        goto ERR_LABEL;
    }

    iRet = read(iFd, acBuf, sizeof(acBuf) - 1);
    if (iRet < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error read /proc/loadavg : %s", strerror(errno));
        goto ERR_LABEL;
    }

    DEF_SAFE_CLOSE(iFd);

    *pdValue = atof(acBuf) / 4;  /* cup include 4 processors,ref https://blog.csdn.net/scugxl/article/details/77199403 */

    return OPLK_OK;
ERR_LABEL:
    DEF_SAFE_CLOSE(iFd);
    return OPLK_ERR;
}

int
UTIL_SetRunningThreadName(const char *pcThreadName)
{
    if (NULL == pcThreadName) {
        return OPLK_ERR;
    }

    prctl(PR_SET_NAME, pcThreadName);

    return OPLK_OK;
}

