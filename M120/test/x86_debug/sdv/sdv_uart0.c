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
#include "sdv_uart0.h"

#define UART0_SEM_KEY    (0xFFEEDD33)

static INT m_iUart0Fd = -1;
static INT m_iUart0Lock;

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
    INT iFdUart0 = -1;
    struct termios stTio;

    if (NULL == pcDev) {
        return OPLK_ERROR;
    }

    iFdUart0 = open(pcDev, O_RDWR | O_NDELAY | O_NONBLOCK | O_NOCTTY);
    if (iFdUart0 < 0) {
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
    tcflush(iFdUart0, TCIFLUSH);

    /* set io attribute is  O_NONBLOCK                                          */
    fcntl(iFdUart0, F_SETFL, O_NONBLOCK);

    /* apply new attribute immediately.                                         */
    tcsetattr(iFdUart0, TCSANOW, &stTio);


    return iFdUart0;
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
    if(-1 == semctl(m_iUart0Lock,0,IPC_RMID,0))
    {
       OPLK_LOG_PRINT_ERR("semctl error.");
       return -1;
    }  

    SAFE_CLOSE(m_iUart0Fd);

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __Uart0Init                                                                           */
/* Descrp : uart init                                                                             */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
static INT
__Uart0Init(VOID)
{
    if (m_iUart0Fd < 0) {

        union semun arg;
        m_iUart0Lock = semget((key_t)UART0_SEM_KEY, 1, IPC_CREAT | 0666);
    
        if(m_iUart0Lock < 0)
        {
           OPLK_LOG_PRINT_ERR("semget error.");
           return -1;
        }

        arg.setval = 1;
        if(-1 == semctl(m_iUart0Lock,0,SETVAL,arg))
        {
           OPLK_LOG_PRINT_ERR("semctl error.");
           return -1;
        }

        m_iUart0Fd = __UART_Open(UART_TTY2);
        if (m_iUart0Fd < 0) {
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:
    SAFE_CLOSE(m_iUart0Fd);
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0Lock                                                                           */
/* Descrp : Uart0 lock                                                                              */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0Lock(VOID)
{
    struct sembuf stSembuf = {0,-1,SEM_UNDO};
    
    if (semop(m_iUart0Lock, &stSembuf, 1) < 0)
    {
        printf("SDV_Uart0Lock error.\n");
        return -1;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0UnLock                                                                         */
/* Descrp : Uart0 unlock                                                                            */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0UnLock(VOID)
{
    struct sembuf stSembuf = {0,1,SEM_UNDO};
          
    if (semop(m_iUart0Lock, &stSembuf, 1) < 0)
    {
        printf("SDV_IicUnLock error.\n");
        return -1;
    }

    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __Uart01Write                                                                          */
/* Descrp : uart write data                                                                       */
/* Input  : pcBuf  --                                                                             */
/*          nBufLen--                                                                             */
/*          nRetry --                                                                             */
/* Output : None                                                                                  */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
static INT
__Uart0Write(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToWrite = 0;
    INT iWrite = 0;
    INT iWriteNum = 0;
    INT iUart0Fd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __Uart0Init()) {
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

    iUart0Fd = m_iUart0Fd;
    if (iUart0Fd < 0) {
        if ((iUart0Fd = __UART_Open(UART_TTY2)) < 0) {
            goto ERR_LABEL;
        }

        m_iUart0Fd = iUart0Fd;
    }

    //SDV_Uart0Lock();
    
    UART_TCFLUSH(iUart0Fd);

    iToWrite = nBufLen;

    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iWrite = write(iUart0Fd, pcTmpBuf, iToWrite);
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

   // SDV_Uart0UnLock();
    
OK_LABEL:
    return iWriteNum;

ERR_LABEL:
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : __Uart01Read                                                                           */
/* Descrp : uart read data                                                                        */
/* Input  : nBufLen--                                                                             */
/*          nRetry --                                                                             */
/* Output : pcBuf  --                                                                             */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
static INT
__Uart0Read(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    INT iIndex = 0;
    INT iToRead = 0;
    INT iRead = 0;
    INT iReadNum = 0;
    INT iUart0Fd = 0;
    CHAR *pcTmpBuf = pcBuf;


    if (OPLK_OK != __Uart0Init()) {
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

    iUart0Fd = m_iUart0Fd;
    if (iUart0Fd < 0) {
        if ((iUart0Fd = __UART_Open(UART_TTY2)) < 0) {
            goto ERR_LABEL;
        }

        m_iUart0Fd = iUart0Fd;
    }

    //SDV_Uart0Lock();
    
    UART_TCDRAIN(iUart0Fd);

    iToRead = nBufLen;
    for (iIndex = 0; iIndex < nRetry; iIndex++) {
        iRead = read(iUart0Fd, pcTmpBuf, iToRead);
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

    //SDV_Uart0UnLock();
    
OK_LABEL:
    return iReadNum;

ERR_LABEL:
    return OPLK_ERROR;
}


/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0Write                                                                         */
/* Descrp : uart write data                                                                       */
/* Input  : ucModeIndex--                                                                         */
/*          pcBuf      --                                                                         */
/*          nBufLen    --                                                                         */
/*          nRetry     --                                                                         */
/* Output : None                                                                                  */
/* Return : write number                                                                          */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0Write(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        return OPLK_ERROR;
    }
    
    return __Uart0Write(ucModeIndex,pcBuf, nBufLen, nRetry);
       
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0Read                                                                          */
/* Descrp : uart read data                                                                        */
/* Input  : ucModeIndex--                                                                         */
/*          nBufLen    --                                                                         */
/*          nRetry     --                                                                         */
/* Output : pcBuf      --                                                                         */
/* Return : read data length                                                                      */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0Read(UCHAR ucModeIndex, CHAR *pcBuf, UINT nBufLen, UINT nRetry)
{
    if (NULL == pcBuf) {
        OPLK_LOG_PRINT_ERR("input parameter error!");
        return OPLK_ERROR;
    }

    return __Uart0Read(ucModeIndex, pcBuf, nBufLen, nRetry);

}

INT
SDV_Uart0SetBaud(ULONG ulBaud)
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
    
    tcgetattr(m_iUart0Fd, &stTio);

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
    
    tcsetattr(m_iUart0Fd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_Uart0SetStopBit(ULONG ulStopBit)
{
    struct termios stTio;

    if (UART_STOP_BIT1 != ulStopBit &&
            UART_STOP_BIT2 != ulStopBit) {
        OPLK_LOG_PRINT_ERR("the stop bit error!");
        goto ERR_LABEL;
    }
    
    tcgetattr(m_iUart0Fd, &stTio);

    if(UART_STOP_BIT1 == ulStopBit)
    {
        stTio.c_cflag &= ~CSTOPB;
    }
    else if(UART_STOP_BIT2 == ulStopBit)
    {
        stTio.c_cflag |= CSTOPB;
    }
    
    tcsetattr(m_iUart0Fd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

INT
SDV_Uart0SetFlowCtl(ULONG ulFlowCtl)
{
    struct termios stTio;

    if (UART_SFLOW_DISABLE != ulFlowCtl &&
            UART_SFLOW_ENABLE != ulFlowCtl) {
        OPLK_LOG_PRINT_ERR("the flow contrl error!");
        goto ERR_LABEL;
    }
    
    tcgetattr(m_iUart0Fd, &stTio);

    if(UART_SFLOW_DISABLE == ulFlowCtl)
    {
        stTio.c_cflag &= ~(IXON | IXOFF | IXANY);
    }
    else if(UART_SFLOW_ENABLE == ulFlowCtl)
    {
        stTio.c_cflag |= (IXON | IXOFF | IXANY);
    }
    
    tcsetattr(m_iUart0Fd, TCSANOW, &stTio);

    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0Init                                                                          */
/* Descrp : uart init                                                                             */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0Init(VOID)
{
    if (OPLK_OK != __Uart0Init()) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:
    __UART_Close();
    return OPLK_ERROR;
}

/***FUNC+******************************************************************************************/
/* Name   : SDV_Uart0Close                                                                         */
/* Descrp : uart close                                                                            */
/* Input  : None                                                                                  */
/* Output : None                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK   :                                                                  */
/*              @arg OPLK_ERROR:                                                                  */
/***FUNC-******************************************************************************************/
INT
SDV_Uart0Close(VOID)
{

    __UART_Close();

    return OPLK_OK;
}
