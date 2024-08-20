#include "NetLink.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "util_inc.h"

#define NULL_SOCK   (-1)
#define MSG_LEN     (1024)


CNetLink::CNetLink() : m_sock(NULL_SOCK)
{

}

CNetLink::~CNetLink()
{
    Close();
}

bool
CNetLink::Open(int iNlType, int iGroup)
{
    m_sock = socket(AF_NETLINK, SOCK_RAW, iNlType);
    if (m_sock < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to create socket, errno = %d", errno);
        return false;
    }

    struct sockaddr_nl addr;
    memset((void *)&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    if (bind(m_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to bind, errno = %d", errno);
        Close();
        return false;
    }

    if (setsockopt(m_sock, SOL_NETLINK, NETLINK_ADD_MEMBERSHIP, &iGroup, sizeof(iGroup)) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to setsockopt, errno = %d", errno);
        Close();
        return false;
    }

    return true;
}

void
CNetLink::Close()
{
    if (m_sock > NULL_SOCK) {
        close(m_sock);
        m_sock = NULL_SOCK;
    }
}

bool
CNetLink::Read(char *buf, int iBufSize, int &iRecvLens)
{
    if (NULL_SOCK == m_sock) {
        LOG_WRITE(EN_LOG_ERROR, "Socket not created or closed, m_sock = %d", m_sock);
        return false;
    }

    struct timeval tv;
    fd_set rfds;
    while (m_sock != NULL_SOCK) {
        FD_ZERO(&rfds);
        FD_SET(m_sock, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int ret = select(m_sock + 1, &rfds, NULL, NULL, &tv);
        if (ret < 0) {
            LOG_WRITE(EN_LOG_ERROR, "Select error, errno = %d", errno);
            return false;
        }

        if (0 == ret) {
            continue;
        }

        if (FD_ISSET(m_sock, &rfds)) {
            break;
        }
    }

    if (NULL_SOCK == m_sock) {
        LOG_WRITE(EN_LOG_ERROR, "Socket closed, m_sock = %d", m_sock);
        return false;
    }

    char recvBuf[MSG_LEN];
    memset(recvBuf, 0, sizeof(recvBuf));

    struct sockaddr_nl nladdr;
    struct msghdr msg;
    struct iovec iov;
    iov.iov_base = (void *)recvBuf;
    iov.iov_len = sizeof(recvBuf);
    msg.msg_name = (void *)&nladdr;
    msg.msg_namelen = sizeof(nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int ret = recvmsg(m_sock, &msg, 0);
    if (ret < NLMSG_HDRLEN) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to receive message, errno = %d", errno);
        return false;
    }
    iRecvLens = ret - NLMSG_HDRLEN;

    if (NULL != buf && iBufSize > 0) {
        memcpy(buf, NLMSG_DATA(recvBuf), iBufSize > iRecvLens ? iRecvLens : iBufSize);
    }

    return true;
}