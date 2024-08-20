#include "rest_common.h"
#include "rest_msft.h"
#include "rest_auth.h"
#include "rest_log.h"

static char m_acNodeName[128] = "1001";

/* fandy: set nodename to gloabl */
void
REST_SetNodeName(char *pcNodeName)
{
    if (NULL != pcNodeName) {
        strncpy(m_acNodeName, pcNodeName, sizeof(m_acNodeName));
        m_acNodeName[sizeof(m_acNodeName) - 1] = '\0';
    }

    return;
}


/* fandy: add event log */
void
REST_EventLog(char *pcEvent, int iResult)
{
    char acEventInfo[512] = {0};
    char acTimeStr[64] = {0};
    TOKEN_ST *pstToken = NULL;

    if (NULL == pcEvent) {
        return;
    }

    /* get time str */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t); /*struct tm tm = *gmtime(&t);*/
    memset(acTimeStr, 0, sizeof(acTimeStr));
    sprintf(acTimeStr, "%d-%02d-%02dT%02d:%02d:%02dZ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    /* get user */
    pstToken = REST_GetCurTokenRecord();

    /* format event log */
    memset(acEventInfo, 0, sizeof(acEventInfo));
    if (HTTP_STATUS_CODE_200 == iResult) {
        snprintf(acEventInfo, sizeof(acEventInfo), "%s, NSA, %s:%s, %s, SUCCESS", acTimeStr, m_acNodeName, pstToken->acUsername, pcEvent);
    } else {
        snprintf(acEventInfo, sizeof(acEventInfo), "%s, NSA, %s:%s, %s, FAIL", acTimeStr, m_acNodeName, pstToken->acUsername, pcEvent);
    }

    /* log to syslog_notification.csv */
    ALARM_WRITE(acEventInfo);

    return;
}


void
REST_UserLog(const REST_REQ_ST *pstReq, const REST_RESP_ST *pstResp)
{
    int iStatus;
    TOKEN_ST *pstToken = NULL;
    char acEvLog[DEF_BUF_SIZE_256] = {0};

    iStatus = pstResp->iStatus;

    pstToken = REST_GetCurTokenRecord();

    /* call UserLog Interface, TBD */

    switch (iStatus) {
    case HTTP_STATUS_CODE_200:
    case HTTP_STATUS_CODE_201:
        LOG_WRITE(EN_LOG_NOTICE, "status code: %d, %s %s, %s, OK", pstResp->iStatus, REST_MethodToString(pstReq->enMethod), pstReq->acUrl, pstToken->acUsername);
        break;
    case HTTP_STATUS_CODE_400:
    case HTTP_STATUS_CODE_401:
    case HTTP_STATUS_CODE_403:
    case HTTP_STATUS_CODE_404:
    case HTTP_STATUS_CODE_405:
    case HTTP_STATUS_CODE_406:
    case HTTP_STATUS_CODE_408:
    case HTTP_STATUS_CODE_421:
    case HTTP_STATUS_CODE_500:
        LOG_WRITE(EN_LOG_ERROR, "status code: %d, %s %s, %s, Failed", pstResp->iStatus, REST_MethodToString(pstReq->enMethod), pstReq->acUrl, pstToken->acUsername);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "status code: %d, %s %s, %s, Unknown Error", pstResp->iStatus, REST_MethodToString(pstReq->enMethod), pstReq->acUrl, pstToken->acUsername);
    }

    /* add event log for post/put/delete/patch method, TBD */
    if (METHOD_POST == pstReq->enMethod || METHOD_PUT == pstReq->enMethod
        || METHOD_DELETE == pstReq->enMethod || METHOD_PATCH == pstReq->enMethod) {
        snprintf(acEvLog, sizeof(acEvLog), "%s %s", REST_MethodToString(pstReq->enMethod), pstReq->acUrl);
        REST_EventLog(acEvLog, iStatus);
    }
    return;
}

