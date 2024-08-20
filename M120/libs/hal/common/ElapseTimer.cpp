#include "ElapseTimer.h"

struct timeval CElapseTimer::m_stStartTime, CElapseTimer::m_stStopTime;


void
CElapseTimer::Start()
{
    gettimeofday(&m_stStartTime, NULL);
}

void
CElapseTimer::Stop()
{
    gettimeofday(&m_stStopTime, NULL);

    long elapsed = (m_stStopTime.tv_sec - m_stStartTime.tv_sec) * 1000000 + (m_stStopTime.tv_usec - m_stStartTime.tv_usec);
    elapsed /= 1000;

    LOG_WRITE(EN_LOG_NOTICE, "elapsed time : %ld", elapsed);
    std::cout << "elapsed time : " << elapsed << std::endl;

}
