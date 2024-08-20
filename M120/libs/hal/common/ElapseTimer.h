#include "DevInternal.h"

#pragma once

class CElapseTimer
{
public:
    static void Start();
    static void Stop();

private:

    static struct timeval m_stStartTime, m_stStopTime;
};
