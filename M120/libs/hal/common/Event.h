#pragma once

#include "DevInternal.h"


class CEvent
{
public:
    CEvent()
    {
    }
    ~CEvent()
    {
    }

    static bool CreateSocket();
    static bool SendEvent(char *pcDestPath, int iEventId, int iSubEventId);
    static bool CloseSocket();

};

