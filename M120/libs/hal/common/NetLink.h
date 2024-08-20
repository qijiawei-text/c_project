#pragma once

#include <linux/netlink.h>

class CNetLink
{
public:
    CNetLink();
    ~CNetLink();
    bool Open(int iNlType, int iGroup);
    void Close(void);
    bool Read(char *buf, int iBufSize, int &iRecvLens);

private:
    int m_sock;
};