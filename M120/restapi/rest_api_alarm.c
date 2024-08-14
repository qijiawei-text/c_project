/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_api_alarm.c
* Description:   REST APIs of alarm module
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/06/04  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"
#include "rest_api_alarm.h"
#include "rest_msft.h"
#include "rest_log.h"

#define MAX_SIZE_ALARMS 512

void
API_GetCurrentAlarms(REST_REQ_ST *pstReq, REST_RESP_ST *pstResp)
{
    cJSON *pstBody = NULL;
    cJSON *pstJsonObj = NULL;
    ALARM_INFO_ST *pstAlmInfo = NULL;
    char acString[64] = {0};
    size_t nCount = 0, nDesLen = 0, nEntityLen = 0, nStrSize = 0;
    int iResult = HTTP_STATUS_CODE_200;
    int i = 0;
    int rc = 0;
    char *pcPos = NULL;
    bool bSkip = false;
    bool bTca = false;

    /* only clear compile warning */
    CLEAR_NO_USED_WARNING(pstReq);

    pstBody = pstResp->pstResponse;

    rc = DSC_GetAlarmList(&pstAlmInfo, &nCount);
    if (SR_ERR_OK != rc) {
        iResult = HTTP_STATUS_CODE_406;
        goto END_LABEL;
    }

    nStrSize = sizeof(acString);

#if defined(__OCS__)
    CLEAR_NO_USED_WARNING(bSkip);
    CLEAR_NO_USED_WARNING(bTca);
    CLEAR_NO_USED_WARNING(nEntityLen);
    CLEAR_NO_USED_WARNING(nStrSize);
    CLEAR_NO_USED_WARNING(nDesLen);

    for (i = 0; i < nCount; i++) {
        pstJsonObj = cJSON_AddObjectToObject(pstBody, pstAlmInfo[i].acAlarmId);
        pcPos = strchr(pstAlmInfo[i].acCaseExt, ':');
        if (NULL == pcPos) {
            cJSON_AddStringToObject(pstJsonObj, STR_ALARM_SEVERITY, pstAlmInfo[i].acCaseExt);
        } else {
            nStrSize = pcPos - pstAlmInfo[i].acCaseExt;
            if (nStrSize >= sizeof(acString)) {
                LOG_WRITE(EN_LOG_ERROR, "Over range: %s", pstAlmInfo[i].acCaseExt);
                continue;
            }
            memset(acString, 0, sizeof(acString));
            strncpy(acString, pstAlmInfo[i].acCaseExt, nStrSize);
            cJSON_AddStringToObject(pstJsonObj, STR_ALARM_SEVERITY, acString);
        }
        cJSON_AddStringToObject(pstJsonObj, STR_TIMESTAMP, pstAlmInfo[i].acRaiseTime);
    }
#else

    CLEAR_NO_USED_WARNING(pcPos);

    rc = DSC_GetRainierAlmList(&pstAlmInfo, &nCount, true, bTca);
    if (SR_ERR_OK != rc) {
        goto END_LABEL;
    }

    if (nCount > 0) {

        for (i = 0; i < nCount; i++) {
            bSkip = (true == bTca) ^ (NULL != strstr(pstAlmInfo[i].acAlarmId, "TCA"));
            if (true == bSkip) {
                continue;
            }
            memset(acString, 0, nStrSize);
            nDesLen = strlen(pstAlmInfo[i].acExternalId);
            nEntityLen = strlen(pstAlmInfo[i].acEntity);
            if (nStrSize > nDesLen + nEntityLen + 2) {
                memcpy(acString, pstAlmInfo[i].acExternalId, nDesLen);
                memcpy(acString + nDesLen, ", ", 2);
                memcpy(acString + nDesLen + 2, pstAlmInfo[i].acEntity, nEntityLen);
            } else {
                LOG_WRITE(EN_LOG_ERROR, "Over range: %s, %s", pstAlmInfo[i].acExternalId, pstAlmInfo[i].acEntity);
                continue;
            }
            /* snprintf(acString, sizeof(acString), "%s, %s", stRainierAlarm[i].acDescription, stRainierAlarm[i].acEntity); */
            pstJsonObj = cJSON_AddObjectToObject(pstBody, acString);
            cJSON_AddStringToObject(pstJsonObj, STR_ALARM_SEVERITY, pstAlmInfo[i].acCaseExt);
            cJSON_AddStringToObject(pstJsonObj, STR_TIMESTAMP, pstAlmInfo[i].acRaiseTime);
        }
    }

#endif
END_LABEL:
    pstResp->iStatus = iResult;

    REST_USER_LOG(pstReq, pstResp);

    return;
}
