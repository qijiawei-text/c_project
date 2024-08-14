/***MODU+******************************************************************************************/
/* Copyright(C), 2014, OPLINK Tech.Co.,Ltd                                                        */
/* FileName    : sdv_uart.c                                                                       */
/* Description : uart sdv source.                                                                 */
/* History     :                                                                                  */
/*     [Author]          [Date]          [Version]        [Description]                           */
/* [1] fandy             2014/05/19      Ver 1.0.0        Initial file.                           */
/***MODU-******************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#include "sdv_cpld.h"
#include "sdv_uart.h"

#define UART_SEM_KEY    (0xFFEEDD22)

static INT m_iUartFd = -1;
static INT m_iUartLock = -1;

/***FUNC+******************************************************************************************/
/* Name   : __UART_Open                                                                           */
/* Descrp : open uart fd                                                                          */
/* Input  : pcDev--                                                                               */
/* Output : None                                                                                  */
/* Return : uart fd                                                                               */
/***FUNC-******************************************************************************************/
static INT
__UART_Open(CHAR *pcDev)
{
    INT iFdUart = -1;
    struct termios stTio;

    if (NULL == pcDev) {
        return OPLK_ERROR;
    }

    iFdUart = open(pcDev, O_RDWR | O_NDELAY | O_NONBLOCK | O_NOCTTY);
    if (iFdUart < 0) {
        OPLK_LOG_PRINT_ERR("open uart error!");
        return OPLK_ERROR;
    }

    /* set attribute of console io                                              */
    memset(&stTio, 0x00, sizeof(stTio));

    /*115200 is master baudrate                                                 */
    stTio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;

    /*
     * CSTOPB:set one stop bits,rather than one;
     * PARENB:enable parity generation on output and parity check for input;
     * CRTSCTS:enable RTS/CTS flow control;
     */
    stTio.c_cflag &= ~(CSTOPB | PARENB | CRTSCTS);

    /* ignore framing errors and parity errors for input                        */
    stTio.c_iflag = IGNPAR;

    /* enable implementation-defined output processing.                         */
    stTio.c_oflag &= ~OPOST;

    /*
     * ICANON:enable canonical mode.
     * This enables special characters EOF,EOL,EOL2,ERASE,KILL,etc and buffers by lines.
     * ECHO:echo input characters;
     * ECHOE:if ICANON is also set,ERASE character erases the preceding input character,
     *      and WERASE erases the preceding word.
     * ISIG:when any of the characters INTR,QUIT,SUSP,or DSUSP are received,generate the
     *      corresponding signal.
     */
    stTio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /*timeout in deciseconds for non-canonical read is 0.                       */
    stTio.c_cc[VTIME] = 0;

    /*mininum number of characters for non-canonical read is 1.                 */
    stTio.c_cc[VMIN] = 1;

    /*flush buffer before implementing new attributes.                          */
    tcflush(iFdUart, TCIFLUSH);

    /* set io attribute is  O_NONBLOCK                                          */
    fcntl(iFdUart, F_SETFL, O_NONBLOCK);

    /* apply new attribute immediately.                                         */
    tcsetattr(iFdUart, TCSANOW, &stTio);


    return iFdUart;
}

/***FUNC+******************************************************************************************/
/* Name   : __UART_Close                                                                          */
/* Descrp : Close uart fd                                                                         */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : OPLK_OK                                                                               */
/***FUNC-******************************************************************************************/
static INT
__UART_Close(VOID)
{
    if(m_iUartLock < 0)
    {
        printf("uart_Lock need init.\n");
        return -1;
    }

    if(-1 == semctl(m_iUartLock,0,IPC_RMID,0))
    {
       OPLK_LOG_PRINT_ERR("semctl error.");
       return -1;
    }  

    m_iUartLock = -1;

    SAFE_CLOSE(m_iUartFd);

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __UartInit                                                                           */
/* Descrp : uart init                                                                             */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
__UartInit(VOID)
{
    if (m_iUartFd < 0) {

        union semun arg;
        m_iUartLock = semget((key_t)UART_SEM_KEY, 1, IPC_CREAT | 0666);
    
        if(m_iUartLock < 0)
        {
           OPLK_LOG_PRINT_ERR("semget error.");
           return -1;
        }

        arg.setval = 1;
        if(-1 == semctl(m_iUartLock,0,SETVAL,arg))
        {
           OPLK_LOG_PRINT_ERR("semctl error.");
           return -1;
        }

        m_iUartFd = __UART_Open(UART_TTY3);
        if (m_iUartFd < 0) {
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:
    SAFE_CLOSE(m_iUartFd);
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_UartLock                                                                           */
/* Descrp : Uart lock                                                                              */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_UartLock(VOID)
{
    struct sembuf stSembuf = {0,-1,SEM_UNDO};

    if(m_iUartLock < 0)
    {
        printf("uart_Lock need init.\n");
        return -1;
    }
    
    if (semop(m_iUartLock, &stSembuf, 1) < 0)
    {
        printf("SDV_UartLock error.\n");
        return -1;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_UartUnLock                                                                         */
/* Descrp : Uart unlock                                                                            */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_UartUnLock(VOID)
{
    struct sembuf stSembuf = {0,1,SEM_UNDO};

    if(m_iUartLock < 0)
    {
        printf("uart_Lock need init.\n");
        return -1;
    }
          
    if (semop(m_iUartLock, &stSembuf, 1) < 0)
    {
        printf("SDV_IicUnLock error.\n");
        return -1;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __Uart1Write                                                                          */
/* Descrp : uart write data                                                                       */
/* Input  : pcBuf  --                                                                             */
/*          nBufLen--                                                                             */
/*          nRetry --                                                                             */
/* Output : None                                                                                  */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
static INT
__UartWrite(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToWrite = 0;
    INT iWrite = 0;
    INT iWriteNum = 0;
    INT iUartFd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __UartInit()) {
        return OPLK_ERROR;
    }

    /* switch uart */
    if (OPLK_OK != SDV_CpldUartSwitch(ucModeIndex)) {
        OPLK_LOG_PRINT_ERR("switch uart error!");
        return OPLK_ERROR;
    }

    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        goto ERR_LABEL;
    }

    iUartFd = m_iUartFd;
    if (iUartFd < 0) {
        if ((iUartFd = __UART_Open(UART_TTY3)) < 0) {
            goto ERR_LABEL;
        }

        m_iUartFd = iUartFd;
    }

    //SDV_UartLock();
    
    UART_TCFLUSH(iUartFd);

    iToWrite = nBufLen;

    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iWrite = write(iUartFd, pcTmpBuf, iToWrite);
        if ((OPLK_ERROR == iWrite) &&
                (EAGAIN != errno)) {
            OPLK_LOG_PRINT_ERR("write uart error!");
            goto ERR_LABEL;
        } else if (0 < iWrite) {
            iToWrite -= iWrite;
            pcTmpBuf += iWrite;
            iWriteNum += iWrite;
        }

        if (0 >= iToWrite) {
            goto OK_LABEL;
        }

        usleep(TIME_20MS);
    }

   // SDV_UartUnLock();
    
OK_LABEL:
    return iWriteNum;

ERR_LABEL:
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : __Uart1Read                                                                           */
/* Descrp : uart read data                                                                        */
/* Input  : nBufLen--                                                                             */
/*          nRetry --                                                                             */
/* Output : pcBuf  --                                                                             */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
static INT
__UartRead(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToRead = 0;
    INT iRead = 0;
    INT iReadNum = 0;
    INT iUartFd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __UartInit()) {
        return OPLK_ERROR;
    }

    /* switch uart */
    if (OPLK_OK != SDV_CpldUartSwitch(ucModeIndex)) {
        OPLK_LOG_PRINT_ERR("switch uart error!");
        return OPLK_ERROR;
    }

    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        goto ERR_LABEL;
    }

    iUartFd = m_iUartFd;
    if (iUartFd < 0) {
        if ((iUartFd = __UART_Open(UART_TTY3)) < 0) {
            goto ERR_LABEL;
        }

        m_iUartFd = iUartFd;
    }

    //SDV_UartLock();
    
    UART_TCDRAIN(iUartFd);

    iToRead = nBufLen;
    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iRead = read(iUartFd, pcTmpBuf, iToRead);
        if ((OPLK_ERROR == iRead) &&
                (EAGAIN != errno)) {
            OPLK_LOG_PRINT_ERR("===read uart error!");
            goto ERR_LABEL;
        } else if (iRead >= 0) {
            iToRead -= iRead;
            pcTmpBuf += iRead;
            iReadNum += iRead;
        }

        if (iToRead <= 0) {
            goto OK_LABEL;
        }

        usleep(TIME_20MS);
    }

    //SDV_UartUnLock();
    
OK_LABEL:
    return iReadNum;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : __UartCheckEndChar                                                                    */
/* Descrp : UART check end char                                                                   */
/* Input  : ucSubCardType--                                                                       */
/*          pcBuf--                                                                               */
/*          iBufLen--                                                                             */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
__UartCheckEndChar(UCHAR ucSubCardType, CHAR *pcBuf, INT iBufLen)
{
#if 0

    switch (ucSubCardType) {

    case OPLK_MOD_EDFA1:

        if (strstr(pcBuf, ">") != NULL) {
            return OPLK_OK;
        }
        break;
    case OPLK_MOD_EDFA2:

        if (strstr(pcBuf, ">") != NULL) {
            return OPLK_OK;
        }
        break;

/*        case OPLK_MOD_WSS:

            if (iBufLen == (pcBuf[1] + 3))
            {
                return OPLK_OK;
            }
            break;*/

    default:
        goto ERR_LABEL;
    }
#endif

    return OPLK_OK;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_UartWrite                                                                         */
/* Descrp : uart write data                                                                       */
/* Input  : ucModeIndex--                                                                         */
/*          pcBuf      --                                                                         */
/*          nBufLen    --                                                                         */
/*          nRetry     --                                                                         */
/* Output : None                                                                                  */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
INT
SDV_UartWrite(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        return OPLK_ERROR;
    }

    return __UartWrite(ucModeIndex, pcBuf, nBufLen, nRetry);

}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0ReadCmdResp                                                                  */
/* Descrp : uart read data check end resp                                                         */
/* Input  : nBufLen--                                                                             */
/*          nRetry --                                                                             */
/*          ucModeIndex--                                                                         */
/*          pcBuf--                                                                               */
/* Output : pcBuf  --                                                                             */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0ReadCmdResp(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToRead = 0;
    INT iRead = 0;
    INT iReadNum = 0;
    INT iUartFd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __UartInit()) {
        return OPLK_ERROR;
    }

    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        goto ERR_LABEL;
    }

    iUartFd = m_iUartFd;
    if (iUartFd < 0) {
        if ((iUartFd = __UART_Open(UART_TTY3)) < 0) {
            goto ERR_LABEL;
        }

        m_iUartFd = iUartFd;
    }

    UART_TCDRAIN(iUartFd);

    iToRead = nBufLen;
    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iRead = read(iUartFd, pcTmpBuf, iToRead);
        if ((OPLK_ERROR == iRead) &&
                (EAGAIN != errno)) {
            OPLK_LOG_PRINT_ERR("===read uart error!");
            goto ERR_LABEL;
        } else if (iRead >= 0) {
            iToRead -= iRead;
            pcTmpBuf += iRead;
            iReadNum += iRead;
        }

        if (OPLK_OK == __UartCheckEndChar(ucModeIndex, pcBuf, iRead)) {
            goto OK_LABEL;
        }

        if (iToRead <= 0) {
            goto OK_LABEL;
        }

        usleep(TIME_20MS);
    }

OK_LABEL:
    return iReadNum;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_UartWssReadCmdResp(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToRead = 0;
    INT iRead = 0;
    INT iReadNum = 0;
    INT iUartFd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __UartInit()) {
        OPLK_LOG_PRINT_ERR("__UartInit!");
        return OPLK_ERROR;
    }

    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        goto ERR_LABEL;
    }

    /* switch uart */
    if (OPLK_OK != SDV_CpldUartSwitch(ucModeIndex)) {
        OPLK_LOG_PRINT_ERR("switch uart error!");
        return OPLK_ERROR;
    }

    iUartFd = m_iUartFd;
    if (iUartFd < 0) {
        if ((iUartFd = __UART_Open(UART_TTY3)) < 0) {
            OPLK_LOG_PRINT_ERR("__UART_Open!");
            goto ERR_LABEL;
        }

        m_iUartFd = iUartFd;
    }

    UART_TCDRAIN(iUartFd);

    iToRead = nBufLen;

    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iRead = read(iUartFd, pcTmpBuf, iToRead);
        if ((OPLK_ERROR == iRead) &&
                (EAGAIN != errno)) {
            OPLK_LOG_PRINT_ERR("===read uart error!");
            goto ERR_LABEL;
        } else if (iRead >= 0) {
            iToRead -= iRead;
            pcTmpBuf += iRead;
            iReadNum += iRead;
        }

        if ((0xDD == pcBuf[iRead - 2]) && (0x02 == pcBuf[iRead - 1])) {
            goto OK_LABEL;
        }

        if (iToRead <= 0) {
            goto OK_LABEL;
        }

        usleep(TIME_20MS);
    }

OK_LABEL:

    return iReadNum;

ERR_LABEL:

    return iReadNum;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_UartRead                                                                          */
/* Descrp : uart read data                                                                        */
/* Input  : ucModeIndex--                                                                         */
/*          nBufLen    --                                                                         */
/*          nRetry     --                                                                         */
/* Output : pcBuf      --                                                                         */
/* Return : read data length                                                                      */
/***FUNC-******************************************************************************************/
INT
SDV_UartRead(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        return OPLK_ERROR;
    }

    return __UartRead(ucModeIndex, pcBuf, nBufLen, nRetry);

}

INT
SDV_UartSetBaud(ULONG ulBaud)
{
    if (UART_BAUDRATE_9600 != ulBaud &&
            UART_BAUDRATE_19200 != ulBaud &&
            UART_BAUDRATE_38400 != ulBaud &&
            UART_BAUDRATE_57600 != ulBaud &&
            UART_BAUDRATE_115200 != ulBaud) {
        OPLK_LOG_PRINT_ERR("the baudrate error!");
        goto ERR_LABEL;
    }

    struct termios stTio;
    
    tcgetattr(m_iUartFd, &stTio);

    if (UART_BAUDRATE_115200 == ulBaud)
    {
        cfsetispeed(&stTio, B115200);
    }
    else if(UART_BAUDRATE_57600 == ulBaud)
    {
        cfsetispeed(&stTio, B57600);
    }
    else if(UART_BAUDRATE_38400 == ulBaud)
    {
        cfsetispeed(&stTio, B38400);
    }
    else if(UART_BAUDRATE_19200 == ulBaud)
    {
        cfsetispeed(&stTio, B19200);
    }
    else
    {
        cfsetispeed(&stTio, B9600);
    }
    
    tcsetattr(m_iUartFd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_UartSetStopBit(ULONG ulStopBit)
{
    struct termios stTio;

    if (UART_STOP_BIT1 != ulStopBit &&
            UART_STOP_BIT2 != ulStopBit) {
        OPLK_LOG_PRINT_ERR("the stop bit error!");
        goto ERR_LABEL;
    }
    
    tcgetattr(m_iUartFd, &stTio);

    if(UART_STOP_BIT1 == ulStopBit)
    {
        stTio.c_cflag &= ~CSTOPB;
    }
    else if(UART_STOP_BIT2 == ulStopBit)
    {
        stTio.c_cflag |= CSTOPB;
    }
    
    tcsetattr(m_iUartFd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_UartSetFlowCtl(ULONG ulFlowCtl)
{
    struct termios stTio;

    if (UART_SFLOW_DISABLE != ulFlowCtl &&
            UART_SFLOW_ENABLE != ulFlowCtl) {
        OPLK_LOG_PRINT_ERR("the flow contrl error!");
        goto ERR_LABEL;
    }
    
    tcgetattr(m_iUartFd, &stTio);

    if(UART_SFLOW_DISABLE == ulFlowCtl)
    {
        stTio.c_cflag &= ~(IXON | IXOFF | IXANY);
    }
    else if(UART_SFLOW_ENABLE == ulFlowCtl)
    {
        stTio.c_cflag |= (IXON | IXOFF | IXANY);
    }
    
    tcsetattr(m_iUartFd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_UartBreak                                                                         */
/* Descrp : UART break                                                                            */
/* Input  : ucModeIndex-- the specified UART to break                                             */
/*            This parameter can be one of the following values:                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_UartBreak(UCHAR ucModeIndex)
{
    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_UartInit                                                                          */
/* Descrp : uart init                                                                             */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_UartInit(VOID)
{
    if (OPLK_OK != __UartInit()) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    __UART_Close();
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_UartClose                                                                         */
/* Descrp : uart close                                                                            */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_UartClose(VOID)
{

    __UART_Close();

    return OPLK_OK;
}
