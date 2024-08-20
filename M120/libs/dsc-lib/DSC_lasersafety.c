/**
 * Copyright (c) Oplink Communications, LLC
 *
 *
 */

#include <stdarg.h>
#include "DSC_lasersafety.h"
#include "DSC_tca.h"


int
DSC_GetLaserSafetyCfg(LASERSAFETY_CFG_ST *pstLaserSafetyCfg, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char xpath[32][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetLaserSafetyCfg++");

    sess = DSC_GetRunningSession();

    if (NULL == sess || NULL == pstLaserSafetyCfg || size < sizeof(LASERSAFETY_CFG_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset(xpath, 0, sizeof(xpath));

    ANONYMOUS_ST lasersafety_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], "%s", LASERSAFETY_CFG_SHUTTER_ON), SR_BOOL_T, (char *)(&pstLaserSafetyCfg->bShutterCfg)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], "%s", LASERSAFETY_CFG_REMOTE_INTERLOCK_ENABLE), SR_BOOL_T, (char *)(&pstLaserSafetyCfg->bRemoteInterlockEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], "%s", LASERSAFETY_CFG_DUMMY_ADMIN_KEY_ENABLE), SR_BOOL_T, (char *)(&pstLaserSafetyCfg->bDummyAdminKeyEnable)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], "%s", LASERSAFETY_CFG_PANEL_SAFE_CMPT_ENABLE), SR_BOOL_T, (char *)(&pstLaserSafetyCfg->bPanelSafeCmptEnable)),
    };

    memset((void *)pstLaserSafetyCfg, 0, size);
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_XPATH_CFG_ALL);
    rc = DSC_GetItems(sess, acSelectXpath, lasersafety_maps, sizeof(lasersafety_maps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        return rc;
    }

    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetLaserSafetyCfg--");
    return rc;
}


int
DSC_GetLaserSafetyState(LASERSAFETY_STATE_ST *pstLaserSafetyState, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char xpath[20][OPLK_PATH_MAX_LEN];
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetLaserSafetyState++");

    memset(xpath, 0, sizeof(xpath));
    ANONYMOUS_ST edfa_maps[5] = {
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[1], "%s", LASERSAFETY_STATE_INTER_LOCK_ON), SR_BOOL_T, (char *)(&pstLaserSafetyState->bInterLock)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[2], "%s", LASERSAFETY_STATE_ADMIN_KEY_ON), SR_INT32_T, (char *)(&pstLaserSafetyState->iAdminKey)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[3], "%s", LASERSAFETY_STATE_LC_PRESENT), SR_BOOL_T, (char *)(&pstLaserSafetyState->bHcfLcPresent)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[4], "%s", LASERSAFETY_STATE_LC_DOOR_LOCK), SR_BOOL_T, (char *)(&pstLaserSafetyState->bHcfLcDoorLock)),
        ANONYMOUS_MAP_ENTRY(__sprintfStr(xpath[5], "%s", LASERSAFETY_STATE_SHUTTER_ON), SR_BOOL_T, (char *)(&pstLaserSafetyState->bShutterSta)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstLaserSafetyState || size < sizeof(LASERSAFETY_STATE_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstLaserSafetyState, 0, size);
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_XPATH_STATE_ALL);
    rc = DSC_GetItems(sess, acSelectXpath, edfa_maps, sizeof(edfa_maps));
    LOG_WRITE(EN_LOG_NOTICE, "DSC_GetLaserSafetyState--");
    return rc;
}


int
DSC_SetShutter(bool state)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetShutter++, state: %d", state);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_CFG_SHUTTER_ON);

    rc = set_bool_item(sess, acSelectXpath, state);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetShutter--");
    return rc;
}

int
DSC_SetRemoteInterlockEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetRemoteInterlockEnable++, bEnable: %d", bEnable);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_CFG_REMOTE_INTERLOCK_ENABLE);

    rc = set_bool_item(sess, acSelectXpath, bEnable);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetRemoteInterlockEnable--");
    return rc;
}

int
DSC_SetDummyAdminKeyEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetDummyAdminKeyEnable++, bEnabled: %d", bEnable);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_CFG_DUMMY_ADMIN_KEY_ENABLE);

    rc = set_bool_item(sess, acSelectXpath, bEnable);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetDummyAdminKeyEnable--");
    return rc;
}

int
DSC_SetPanelSafeCmptEnable(bool bEnable)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetPanelSafeCmptEnable++, bEnabled: %d", bEnable);

    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, LASERSAFETY_CFG_PANEL_SAFE_CMPT_ENABLE);

    rc = set_bool_item(sess, acSelectXpath, bEnable);
    if (SR_ERR_OK != rc) {
        sr_discard_changes(sess);
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    LOG_WRITE(EN_LOG_NOTICE, "DSC_SetPanelSafeCmptEnable--");
    return rc;
}

