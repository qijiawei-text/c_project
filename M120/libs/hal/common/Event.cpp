#include "Event.h"
#include "util_inc.h"

static int m_iFd = -1;

bool
CEvent::CreateSocket()
{
    m_iFd = UTIL_CreateMsgSock((char *)DEF_HAL_EVENT_SOCKET_PATH);
    if (m_iFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Create msg sock error. m_iFd = %d", m_iFd);
        return false;
    }

    return true;
}

bool
CEvent::SendEvent(char *pcDestPath, int iEventId, int iSubEventId)
{
    if (OPLK_OK != UTIL_SendEvent(m_iFd, pcDestPath, iEventId, iSubEventId)) {
        return false;
    }

    return true;
}

bool
CEvent::CloseSocket()
{
    if (m_iFd > 0) {
        UTIL_CloseMsgSock((char *)DEF_HAL_EVENT_SOCKET_PATH, m_iFd);
    }

    return true;
}
