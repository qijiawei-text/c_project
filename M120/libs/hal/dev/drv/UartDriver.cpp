/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      UartDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "UartDriver.h"
#include <termios.h>
#include <fcntl.h>
#include <sstream>


/*=============================variables=========================*/

std::array<CRecursiveMutex, UART_HW_MAX_NUM> CUartHwDriver::ms_staMutexs;
std::vector<int32_t> CUartHwDriver::ms_staFds(UART_HW_MAX_NUM, -1);

std::array<CRecursiveMutex, UART_USB_MAX_NUM> CUartUsbDriver::ms_staMutexs;
std::vector<int32_t> CUartUsbDriver::ms_staFds(UART_USB_MAX_NUM, -1);



/*==============================function=========================*/

#if 0
bool
CUartDriver::Open(uint32_t uiIdx)
{
    LOG_WRITE(EN_LOG_DEBUG, "Open +++");
    if (uiIdx >= GetUartMaxNum()) {
        LOG_WRITE(EN_LOG_ERROR, "uiIdx(%d) >= %d", uiIdx, GetUartMaxNum());
        return false;
    }

    if (GetFd(uiIdx) > 0) {
        m_iFd = GetFd(uiIdx);
        return true;
    }

    std::string staPath = GetFilePath(uiIdx);
    m_iFd = open(staPath.c_str(), O_RDWR | O_NDELAY | O_NONBLOCK | O_NOCTTY);
    if (m_iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error, m_iFd = %d", staPath.c_str(), m_iFd);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Open uart %s, %d", staPath.c_str(), m_iFd);

    SetFd(uiIdx, m_iFd);

    if (!SetCfg()) {
        return false;
    }

    return true;

}


bool
CUartDriver::Close(uint32_t uiIdx)
{

    if (uiIdx >= GetUartMaxNum()) {
        return false;
    }

    if (GetFd(uiIdx) < 0) {
        return true;
    }

    MutexLock(uiIdx);

    close(GetFd(uiIdx));

    m_iFd = -1;
    SetFd(uiIdx, -1);

    MutexUnlock(uiIdx);

    return true;

}
#endif

bool
CUartDriver::Open()
{
    LOG_WRITE(EN_LOG_DEBUG, "Open +++");

    if (m_iFd > 0) {
        return true;
    }

    std::string staPath = GetFilePath();
    m_iFd = open(staPath.c_str(), O_RDWR | O_NDELAY | O_NONBLOCK | O_NOCTTY);
    if (m_iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "open %s error, m_iFd = %d", staPath.c_str(), m_iFd);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Open uart %s, %d", staPath.c_str(), m_iFd);

    if (!SetCfg()) {
        return false;
    }

    return true;

}


bool
CUartDriver::Close()
{
    if (m_iFd < 0) {
        return true;
    }

    close(m_iFd);

    m_iFd = -1;

    return true;
}


bool
CUartDriver::SetCfg(void)
{

    if (m_iFd < 0) {
        return false;
    }

    struct termios stTio = {0};

    stTio.c_cflag |= CLOCAL | CREAD;

    /* disable RTS/CTS flow control; */
    stTio.c_cflag &= ~CRTSCTS;

    /* ignore framing errors and parity errors for input */
    stTio.c_iflag |= IGNPAR;

    /* enable implementation-defined output processing */
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

    /* set baudrate */
    switch (m_uiBd) {
    case UART_BD_2400:

        stTio.c_cflag |= B2400;
        break;

    case UART_BD_4800:

        stTio.c_cflag |= B4800;
        break;

    case UART_BD_9600:

        stTio.c_cflag |= B9600;
        break;

    case UART_BD_19200:

        stTio.c_cflag |= B19200;
        break;

    case UART_BD_38400:

        stTio.c_cflag |= B38400;
        break;

    case UART_BD_115200:
    default:

        stTio.c_cflag |= B115200;
        break;
    }

    /* set data bits */
    switch (m_uiDataBits) {
    case UART_DATA_BITS_7:

        stTio.c_cflag |= CS7;
        break;

    case UART_DATA_BITS_8:
    default:

        stTio.c_cflag |= CS8;
        break;
    }

    /* set stop bits */
    switch (m_uiStopBits) {
    case UART_STOP_BITS_2:

        stTio.c_cflag |= CSTOPB;
        break;

    case UART_STOP_BITS_1:
    default:

        stTio.c_cflag &= ~CSTOPB;
        break;
    }

    /* set parity */
    switch (m_uiParity) {
    case UART_PARITY_ODD:

        stTio.c_cflag |= PARODD | PARENB;
        stTio.c_iflag |= INPCK;
        break;

    case UART_PARITY_EVEN:

        stTio.c_cflag |= PARENB;
        stTio.c_cflag &= ~PARODD;
        stTio.c_iflag |= INPCK;
        break;

    case UART_PARITY_NO:
    default:

        stTio.c_cflag &= ~PARENB;
        break;
    }

    /*flush buffer before implementing new attributes.                          */
    tcflush(m_iFd, TCIOFLUSH);

    /* set io attribute is  O_NONBLOCK                                          */
    fcntl(m_iFd, F_SETFL, O_NONBLOCK);

    /* apply new attribute immediately.                                         */
    tcsetattr(m_iFd, TCSANOW, &stTio);

    return true;

}

bool
CUartDriver::WriteBytes(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "WriteBytes +++");

    if (!Open()) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write %s", pucData);

    int32_t iWriteLen = 0;

    CDelay::Delay(10, MS);
    MACRO_UART_TCFLUSH(m_iFd);

    while (uiLen > 0) {
        LOG_WRITE(EN_LOG_DEBUG, "uiLen = %d", uiLen);
        iWriteLen = write(m_iFd, pucData, uiLen);
        LOG_WRITE(EN_LOG_DEBUG, "iWriteLen = %d", iWriteLen);

        if ((iWriteLen <= 0) && (EAGAIN != errno)) {
            /* LOG_WRITE(EN_LOG_ERROR, "WriteBytes %d %d.", iWriteLen, errno); */
            return false;
        } else if (iWriteLen > 0) {
            uiLen -= iWriteLen;
            pucData += iWriteLen;
        }

        CDelay::Delay(20, MS);
    }

    LOG_WRITE(EN_LOG_DEBUG, "WriteBytes ---");
    /* LOG_WRITE(EN_LOG_NOTICE, "%d WriteBytes %d", m_iFd, iWriteLen); */

    return true;

}


bool
CUartDriver::ReadBytes(uint32_t uiIdx, std::shared_ptr<CRspHandler> stpRsp)
{
    uint32_t uiMs = 0;
    uint8_t *pucData = stpRsp->Data();
    int32_t iLen = 0;

    if (!Open()) {
        return false;
    }

    MACRO_UART_TCDRAIN(m_iFd);

    for (uiMs = 0; uiMs < stpRsp->TimeOut(); uiMs++) {
        iLen = read(m_iFd, pucData, stpRsp->Len());
        if (iLen <= 0) {
            if (EAGAIN == errno) {
                CDelay::Delay(10, MS);
                continue;
            }
            return false;
        } else if (iLen > 0) {
            pucData += iLen;
            stpRsp->Update(iLen);
        }

        int32_t iRspStatus = stpRsp->Proc();

        if (RSP_STATUS_OK == iRspStatus) {
            return true;
        } else if (RSP_STATUS_AGAIN == iRspStatus) {
            CDelay::Delay(10, MS);
            continue;
        } else {
            return false;
        }
    }

    if (uiMs >= stpRsp->TimeOut()) {
        LOG_WRITE(EN_LOG_ERROR, "Fd = %d ReadBytes timeout. uiReadLen = %d", m_iFd, stpRsp->Pos());
        return false;
    }

    return true;

}


bool
CUartDriver::ReadWrite(uint32_t uiIdx, uint8_t *pucDataW, uint32_t uiLenW,
                       uint8_t *pucDataR, std::shared_ptr<CRspHandler> stpRspHandler)
{
    uint32_t uiRetryTimes = 0;

    LOG_WRITE(EN_LOG_DEBUG, "ReadWrite +++");

    if (!WriteBytes(uiIdx, pucDataW, uiLenW)) {
        LOG_WRITE(EN_LOG_ERROR, "WriteBytes error. uiIdx = %d", uiIdx);
        return false;
    }

    CDelay::Delay(10, MS);

    for (uiRetryTimes = 0; uiRetryTimes < stpRspHandler->RetryTimes(); uiRetryTimes++) {
        if (!ReadBytes(uiIdx, stpRspHandler)) {
            if (stpRspHandler->InProgress()) {
                continue;
            }
            return false;
        }
        break;
    }

    if (uiRetryTimes >= stpRspHandler->RetryTimes()) {

        LOG_WRITE(EN_LOG_ERROR, "ReadWrite timeout.");
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "ReadWrite ---");

    return true;
}



bool
CUartDriver::Read(uint32_t uiIdx, uint8_t *pucData, std::shared_ptr<CRspHandler> stpRspHandler)
{

    bool bRet = true;

    if (!ReadBytes(uiIdx, stpRspHandler)) {
        bRet = false;
    }

    return bRet;

}

bool
CUartDriver::Write(uint32_t uiIdx, uint8_t *pucData, uint32_t uiLen)
{
    bool bRet = true;

    LOG_WRITE(EN_LOG_DEBUG, "Write +++");


    if (!WriteBytes(uiIdx, pucData, uiLen)) {
        bRet = false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Write ---");

    return bRet;

}

