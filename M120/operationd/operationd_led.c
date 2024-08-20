/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_led.c
   Description : led rpc.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "sysrepo.h"
#include "util_inc.h"
#include "DSC_inc.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"
#include "DevEdfaCApi.h"
#include "DevOpsCApi.h"
#include "DevWssCApi.h"
#include "DevDbgCApi.h"
#include "DevOcsCApi.h"

#include "operationd_led.h"

#define OPS_LED_LAMP_ON (1)
#define AMP_LED_LAMP_ON (7)


/* Running flag for Device LEDs */
static bool m_bDevLedThreadRunning = false;
static pthread_mutex_t m_stLedMutex = PTHREAD_MUTEX_INITIALIZER;
static OCP_LED_OPERATION_ST m_stDeviceOper;
static LED_CTRL_MODE_EN m_enLedCtrlMode = EN_LED_CTRL_NONE;


int __RpcLedOperationCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                        sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);

static void
__LedOperLock()
{
    pthread_mutex_lock(&m_stLedMutex);
}

static void
__LedOperUnlock()
{
    pthread_mutex_unlock(&m_stLedMutex);
}

static ST_RPC_SUB m_astLedRpcSub[] = {
    {MODULE_NS_OCP_EXT, __RpcLedOperationCb, RPC_XPATH_LED_OPERTION},
};

static void *
__BoardLedLampTest(void *argv)
{
    unsigned long ulIdx = 0;
    unsigned long ulAddress = 0;
    unsigned char ucVal = 0;
    bool *pbLampingProc = (bool *)argv;

#if 0
    uint16_t usWaitingTime = 0;

    __LedOperLock();
    usWaitingTime = m_stDeviceOper.usInterval;
    __LedOperUnlock();
#endif

    LOG_WRITE(EN_LOG_NOTICE, "boardlamptest start");
    /* set scc led lamptest on*/
    ulAddress = 0x107c;
    ucVal = 1;
    DBG_WriteLogicReg(ulIdx, ulAddress, 1, &ucVal);

    /* set rpc fan led lamptest on*/
    ulAddress = 0x434;
    ucVal = 1;
    DBG_WriteLogicReg(ulIdx, ulAddress, 1, &ucVal);

    while (m_bDevLedThreadRunning && pbLampingProc[0]) {
        usleep(200);
    }

#if 0
    DEF_SLEEP_S(usWaitingTime);
#endif

    /* set scc led lamptest off*/
    ulAddress = 0x107c;
    ucVal = 0;
    DBG_WriteLogicReg(ulIdx, ulAddress, 1, &ucVal);

    /* set rpc fan led lamptest off*/
    ulAddress = 0x434;
    ucVal = 0;
    DBG_WriteLogicReg(ulIdx, ulAddress, 1, &ucVal);
    LOG_WRITE(EN_LOG_NOTICE, "boardlamptest finish");


#if 0
    uint32_t ulLedIndex = 0;
    uint32_t ulLedCount = 0;
    uint32_t ulComponentIndex = 0;
    const ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;
    bool *pbLampingProc = (bool *)argv;

    for (ulComponentIndex = 0; ulComponentIndex < DSC_GetComponentInfoCount(); ulComponentIndex++) {
        pstComponentInfo = DSC_GetComponentInfoByIndex(ulComponentIndex);
        if (CARD_SCC == pstComponentInfo->ulDevType || CARD_FAN == pstComponentInfo->ulDevType) {
            ulLedCount += pstComponentInfo->ulLedCount;
        }
    }

    while (m_bDevLedThreadRunning && pbLampingProc[0]) {
        /* lamptest: green(0.5s) -> off(0.5s) -> red(0.5s) -> off(0.5s) */
        for (ulLedIndex = 0; ulLedIndex < ulLedCount; ulLedIndex++) {
            BOARD_SetLedState(ulLedIndex, DSC_LED_STATE_GREEN);
        }

        DEF_SLEEP_MS(200);

        for (ulLedIndex = 0; ulLedIndex < ulLedCount; ulLedIndex++) {
            BOARD_SetLedState(ulLedIndex, DSC_LED_STATE_OFF);
        }

        DEF_SLEEP_MS(200);

        for (ulLedIndex = 0; ulLedIndex < ulLedCount; ulLedIndex++) {
            BOARD_SetLedState(ulLedIndex, DSC_LED_STATE_RED);
        }

        DEF_SLEEP_MS(200);

        for (ulLedIndex = 0; ulLedIndex < ulLedCount; ulLedIndex++) {
            BOARD_SetLedState(ulLedIndex, DSC_LED_STATE_OFF);
        }

        DEF_SLEEP_MS(200);
    }
#endif
    pbLampingProc[1] = false;


    return 0;
}
static bool
__IsMatchedCardTypeAndSlot(uint32_t uiCardType, uint32_t uiSlotIdx)
{
    int rc = OPLK_OK;
    CSlotInfo stSlotInfo;

    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(uiSlotIdx, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot[%d] info error. errNum = %d", uiSlotIdx, rc);
        goto ERR_LABEL;
    }

    if ((SLOT_STATE_ONLINE == stSlotInfo.uiSlotState) && (uiCardType == stSlotInfo.uiCardType)) {
        return true;
    } else {
        LOG_WRITE(EN_LOG_DEBUG, "uiSlotIdx: %d, uiSlotState: %s, ExpectedCardType: %d, ActualCardType: %d",
                  uiSlotIdx, stSlotInfo.uiSlotState, uiCardType, stSlotInfo.uiCardType);
        return false;
    }

ERR_LABEL:
    return false;
}

/* Start the lamping test for all LEDs. */
static int
__SetAllLedState(bool *pbProcFlag)
{
    /* uint32_t ulLedIndex = 0; */
    uint32_t ulComponentIndex = 0;
    uint32_t ulDevType = 0;
    /* uint32_t ulFanIndex = 0; */
    const ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;
    pthread_t tThreadId = -1;
    pthread_attr_t tThreadAttr;

    for (ulComponentIndex = 0; ulComponentIndex < DSC_GetComponentInfoCount(); ulComponentIndex++) {
        /* Query the slot information */
        pstComponentInfo = DSC_GetComponentInfoByIndex(ulComponentIndex);
        if ((NULL == pstComponentInfo) || (0 == pstComponentInfo->ulLedCount)) {
            continue;
        }

        /* Set SCC LEDs by board API, others by module API */
        if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_SCC)) {
            /* Board LED */
            pthread_attr_init(&tThreadAttr);
            pthread_attr_setdetachstate(&tThreadAttr, PTHREAD_CREATE_DETACHED);
            pthread_create(&tThreadId, &tThreadAttr, __BoardLedLampTest, pbProcFlag);
        } else if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS)) {
            /* OPS */
            LOG_WRITE(EN_LOG_NOTICE, "OPS_SetLedDebugMode, dev: %u, state: %u", ulDevType, pstComponentInfo->ulSlotIndex);
            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS)) {
                    ulDevType = DEV_TYPE_OPS;
                } else {
                    ulDevType = (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS_4_A)) ? DEV_TYPE_OPS4A : DEV_TYPE_OPS4B;
                }

                if (OPLK_OK != OPS_SetLedDebugMode(ulDevType, OPS_LED_LAMP_ON)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetLedDebugMode, dev: %u, state: %u", ulDevType, OPS_LED_LAMP_ON);
                }
            }
        } else if ((0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP)) ||
                   (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP_P))) {
            /* AMP */
            LOG_WRITE(EN_LOG_NOTICE, "EDFA_SetLedState, dev: %u, state: %u", ulDevType, pstComponentInfo->ulSlotIndex);

            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                ulDevType = (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP)) ? DEV_TYPE_BA : DEV_TYPE_P_BA;
                if (OPLK_OK != EDFA_SetLedState(ulDevType, AMP_LED_LAMP_ON)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by EDFA_SetLedState, dev: %u, state: %u", ulDevType, AMP_LED_LAMP_ON);
                }
            }
        } else if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_LC_OCS)) {
            /* OCS */
            LOG_WRITE(EN_LOG_NOTICE, "OCS_SetLedMode, dev: %u, state: %u", ulDevType, pstComponentInfo->ulSlotIndex);
            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                ulDevType = DEV_TYPE_OCS;
                if (OPLK_OK != OCS_SetLedMode(ulDevType, OCS_LED_LAMP_TEST_STATE)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by EDFA_SetLedState, dev: %u, state: %u", ulDevType, OCS_LED_LAMP_TEST_STATE);
                }
            }
        }
    }

    return OPLK_OK;
}

/* Set the configurated state for all LEDs. */
static int
__RestoreAllLedState(uint8_t(*pSta)[LED_MAX_NUM])
{
    uint32_t ulLedIndex = 0;
    uint32_t ulComponentIndex = 0;
    uint32_t ulDevType = 0;
    uint32_t ulFanIndex = 0;
    uint8_t *pucBoardLeds = NULL;
    const ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    /* Check the input argument. */
    if (NULL == pSta) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid parameters, NULL pointer.");
        return OPLK_ERR;
    }

    for (ulComponentIndex = 0; ulComponentIndex < DSC_GetComponentInfoCount(); ulComponentIndex++) {
        /* Query the slot information */
        pstComponentInfo = DSC_GetComponentInfoByIndex(ulComponentIndex);
        if ((NULL == pstComponentInfo) || (0 == pstComponentInfo->ulLedCount)) {
            continue;
        }
        LOG_WRITE(EN_LOG_NOTICE, "ulComponentIndex: %d, pcComponentName: %s, ulLedCount: %d", ulComponentIndex, pstComponentInfo->pcComponentName, pstComponentInfo->ulLedCount);
        /* Set SCC LEDs by board API, others by module API */
        if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_SCC)) {
            /* SCC Board LED */
            ulFanIndex = pstComponentInfo->ulLedCount;
            pucBoardLeds = *pSta;

            for (ulLedIndex = 0; ulLedIndex < pstComponentInfo->ulLedCount; ulLedIndex++) {
                if (OPLK_OK != BOARD_SetLedState(ulLedIndex, *(pucBoardLeds + ulLedIndex))) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_SetLedState, led: %u, state: %u", ulLedIndex, *(pucBoardLeds + ulLedIndex));
                }
            }
            LOG_WRITE(EN_LOG_DEBUG, "pSta: %p, restore scc led: (%d, %d, %d)", pSta, pucBoardLeds[0], pucBoardLeds[1], pucBoardLeds[2]);

            /* Next slot */
            ++pSta;
        } else if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS)) {
            /* OPS */
            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS)) {
                    ulDevType = DEV_TYPE_OPS;
                } else {
                    ulDevType = (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS_4_A)) ? DEV_TYPE_OPS4A : DEV_TYPE_OPS4B;
                }

                if (OPLK_OK != OPS_SetLedDebugMode(ulDevType, DSC_LED_STATE_OFF)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by OPS_SetLedDebugMode, dev: %u, state: %u", ulDevType, DSC_LED_STATE_OFF);
                }
            }

            /* Next slot */
            ++pSta;
        } else if ((0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP)) ||
                   (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP_P))) {
            /* AMP */
            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                ulDevType = (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP)) ? DEV_TYPE_BA : DEV_TYPE_P_BA;
                if (OPLK_OK != EDFA_SetLedState(ulDevType, DSC_LED_STATE_OFF)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by EDFA_SetLedState, dev: %u, state: %u", ulDevType, DSC_LED_STATE_OFF);
                }
            }

            /* Next slot */
            ++pSta;
        } else if (0 == strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_LC_OCS)) {
            /* OCS */
            if (__IsMatchedCardTypeAndSlot(pstComponentInfo->ulDevType, pstComponentInfo->ulSlotIndex)) {
                ulDevType = DEV_TYPE_OCS;
                if (OPLK_OK != OCS_SetLedMode(ulDevType, OCS_LED_NORMAL_WORK_STATE)) {
                    LOG_WRITE(EN_LOG_ERROR, "Error by OCS_SetLedMode, dev: %u, state: %u", ulDevType, OCS_LED_NORMAL_WORK_STATE);
                }
            }

            /* Next slot */
            ++pSta;
        } else {
            /* Fan */
            if (NULL == strstr(pstComponentInfo->pcComponentName, LED_NAME_FAN)) {
                continue;
            }

            if (NULL == pucBoardLeds) {
                continue;
            }

            ulLedIndex = ulFanIndex + pstComponentInfo->ulSubIndex;
            if (OPLK_OK != BOARD_SetLedState(ulLedIndex, *(pucBoardLeds + ulLedIndex))) {
                LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_SetLedState, led: %u, state: %u", ulLedIndex, *(pucBoardLeds + ulLedIndex));
            }
            LOG_WRITE(EN_LOG_NOTICE, "pSta: %p, FAN[%d] led: %d", pucBoardLeds, pstComponentInfo->ulSubIndex + 1, pucBoardLeds[ulLedIndex]);
        }
    }

    return OPLK_OK;
}

/* Query state from every LED. */
static int
__GetAllLedState(uint8_t(*pSta)[LED_MAX_NUM])
{
    int i = 0;
    const ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;
    uint32_t ulComponentIndex = 0;
    uint32_t ulComponentCount = 0;
    CBoardStatusData stBoardSta;

    /* Check the input argument. */
    if (NULL == pSta) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid parameters, NULL pointer.");
        return OPLK_ERR;
    }

    ulComponentCount = DSC_GetComponentInfoCount();
    for (ulComponentIndex = 0; ulComponentIndex < ulComponentCount; ulComponentIndex++) {
        /* Query the slot information */
        pstComponentInfo = DSC_GetComponentInfoByIndex(ulComponentIndex);
        if ((NULL == pstComponentInfo) || (0 == pstComponentInfo->ulLedCount)) {
            continue;
        }

        /* Set SCC LEDs by board API, others by module API */
        if (strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_SCC) == 0) {
            /* SCC Board LED */
            memset(&stBoardSta, 0, sizeof(stBoardSta));
            if (OPLK_OK != BOARD_GetStatusData(&stBoardSta)) {
                LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetStatusData");
                return OPLK_ERR;
            }
            for (i = 0; i < LED_MAX_NUM; i++) {
                (*pSta)[i] = (uint8_t)stBoardSta.astLed[i].uiState;
            }
            LOG_WRITE(EN_LOG_DEBUG, "pSta: %p, scc led: (%d, %d, %d); FAN led: (%d, %d, %d, %d)", pSta, stBoardSta.astLed[0].uiState, stBoardSta.astLed[1].uiState, stBoardSta.astLed[2].uiState,
                      stBoardSta.astLed[3].uiState, stBoardSta.astLed[4].uiState, stBoardSta.astLed[5].uiState, stBoardSta.astLed[5].uiState);
            ++pSta;
        } else if (strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_OPS) == 0) {
            /* When restoring the LED, it is controlled by the module itself */
            ++pSta;
        } else if (strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP) == 0 ||
                   strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_AMP_P) == 0) {
            /* When restoring the LED, it is controlled by the module itself */
            ++pSta;
        } else if (strcmp(pstComponentInfo->pcComponentName, DEF_OCP_COMPONENT_KEY_LC_OCS) == 0) {
            /* When restoring the LED, it is controlled by the module itself */
            ++pSta;
        }

    }

    if (0 == ulComponentCount) {
        return OPLK_ERR;
    }

    return OPLK_OK;
}

static void *
__RemoteCtrlDevLedProc(void *argv)
{
    uint16_t usSeconds = 0;
    bool bLedRestoreFlag = false;
    bool bLampingOn[2] = {true, true};
    OCP_LED_OPERATION_ST stLedOper;
    /* All LEDs status cache for all slots. */
    uint8_t aucAllLedsSta[SLOT_MAX_NUM][LED_MAX_NUM];

    LOG_WRITE(EN_LOG_NOTICE, "++ %s", __func__);

    /* Clear */
    memset(&stLedOper, 0, sizeof(stLedOper));
    memset(aucAllLedsSta, 0, sizeof(aucAllLedsSta));
    __LedOperLock();
    memcpy(&stLedOper, &m_stDeviceOper, sizeof(stLedOper));
    __LedOperUnlock();

    /* get all LEDs state */
    if (OPLK_OK == __GetAllLedState(aucAllLedsSta)) {
        bLedRestoreFlag = true;
    } else {
        bLedRestoreFlag = false;
        LOG_WRITE(EN_LOG_ERROR, "Error by __GetAllLedState.");
    }

    /* Set the lamp test flag */
    DSC_OCP_SetLamptestFlag(true);

    /* Set all LEDs to blink state*/
    if (OPLK_OK != __SetAllLedState(bLampingOn)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __SetAllLedState");
    }

    /* Running for specified seconds.*/
    while (0 == stLedOper.usInterval) {
        if (!m_bDevLedThreadRunning) {
            break;
        }
        /* Sleep for 1 second */
        sleep(1);
    }

    /* Running for specified seconds.*/
    for (usSeconds = 0; usSeconds < stLedOper.usInterval; usSeconds++) {
        /* Check if stopped manually */
        if (!m_bDevLedThreadRunning) {
            break;
        }
        /* Sleep for 1 second */
        sleep(1);
    }
    bLampingOn[0] = false;

    /* Wait for thread task finished */
    while (bLampingOn[1]) {
        usleep(1000);
    }

    /* restore led state */
    if (bLedRestoreFlag) {
        if (OPLK_OK != __RestoreAllLedState(aucAllLedsSta)) {
            LOG_WRITE(EN_LOG_ERROR, "Error by __RestoreAllLedState");
        }
    }

    /* clear cache */
    __LedOperLock();
    memset(&m_stDeviceOper, 0, sizeof(m_stDeviceOper));
    __LedOperUnlock();

    LOG_WRITE(EN_LOG_NOTICE, "-- %s", __func__);
    DSC_OCP_SetLamptestFlag(false);
    m_bDevLedThreadRunning = false;
    m_enLedCtrlMode = EN_LED_CTRL_NONE;

    return NULL;
}

int
__RpcLedOperationCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                    sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK, iResult = SR_ERR_OK;
    pthread_t tThreadId = -1;
    pthread_attr_t attr;
    char acOper[32] = {0};
    OCP_LED_OPERATION_ST stLedOper;
    ANONYMOUS_ST astAnonymous[] = {{RPC_XPATH_LED_OPER_OPERTION, SR_ENUM_T, acOper, sizeof(acOper) - 1},
                                   {RPC_XPATH_LED_OPER_INTERVAL, SR_UINT16_T, (char *)&stLedOper.usInterval, sizeof(stLedOper.usInterval)},
                                   {RPC_XPATH_LED_OPER_SRC_COMPONENT, SR_STRING_T, stLedOper.acSrcComponent, sizeof(stLedOper.acSrcComponent) - 1},
                                   {RPC_XPATH_LED_OPER_SRC_LED, SR_STRING_T, stLedOper.acSrcLed, sizeof(stLedOper.acSrcLed) - 1}};
    char acBufMsg[DEF_BUF_SIZE_256] = {0};
    bool bStartDevThread = false;

    memset(&stLedOper, 0, sizeof(stLedOper));
    rc = DSC_ParseValuesByXpath(input, input_cnt, astAnonymous, sizeof(astAnonymous));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_ParseValuesByXpath.");
        goto cleanup;
    }

    if (OPLK_OK != UTIL_ChkStrNumberLetter(stLedOper.acSrcComponent)) {
        rc = SR_ERR_INVAL_ARG;
        snprintf(acBufMsg, sizeof(acBufMsg), "%s", "Invalid source-component!");
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "operation: %s, interval: %d, componet: %s, led: %s", acOper, stLedOper.usInterval, stLedOper.acSrcComponent, stLedOper.acSrcLed);

    stLedOper.bEnabled = (0 == strcasecmp(DSC_EXT_LED_OPERATION_ENABLED_STR, acOper)) ? true : false;

    UTIL_UpperStr(stLedOper.acSrcComponent);

    if (!stLedOper.bEnabled) {
        /* Stop lamptest for all Leds */
        if (m_bDevLedThreadRunning) {
            m_bDevLedThreadRunning = false;
            sleep(1);
        } else {
            /* Clear cache */
            __LedOperLock();
            memset(&m_stDeviceOper, 0, sizeof(m_stDeviceOper));
            __LedOperUnlock();
        }
        m_enLedCtrlMode = EN_LED_CTRL_NONE;
        DSC_OCP_SetLamptestFlag(false);
        rc = SR_ERR_OK;

        goto cleanup;
    } else {
        /* Lamping test for all components with LED. */
        if (0 == strcasecmp(stLedOper.acSrcComponent, "ALL")) {
            if (m_bDevLedThreadRunning) {
                LOG_WRITE(EN_LOG_NOTICE, "All LEDs are already running.");
                rc = SR_ERR_OPERATION_FAILED;
                snprintf(acBufMsg, sizeof(acBufMsg), "%s", "LED operation is already running.");
                goto cleanup;
            }

            m_enLedCtrlMode = EN_LED_CTRL_ALL;
            bStartDevThread = true;

            __LedOperLock();
            memcpy(&m_stDeviceOper, &stLedOper, sizeof(stLedOper));
            __LedOperUnlock();
        } else {
            /* Not supported. */
            snprintf(acBufMsg, sizeof(acBufMsg), "Not support the current source-component: %s.", stLedOper.acSrcComponent);
            LOG_WRITE(EN_LOG_ERROR, "%s", acBufMsg);
            rc = SR_ERR_OPERATION_FAILED;
            goto cleanup;
        }
    }

    /* start thread */
    if (bStartDevThread) {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        m_bDevLedThreadRunning = true;
        rc = pthread_create(&tThreadId, &attr, __RemoteCtrlDevLedProc, NULL);
        if (0 != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by creating thread __RemoteCtrlDevLedProc! rc=%d", rc);
            m_bDevLedThreadRunning = false;
            rc = SR_ERR_OPERATION_FAILED;
            goto cleanup;
        }
    }

cleanup:

    iResult = sr_new_values(1, output);
    if (SR_ERR_OK != iResult) {
        LOG_WRITE(EN_LOG_ERROR, "%s", sr_strerror(iResult));
        *output_cnt = 0;
        return iResult;
    }

    sr_val_set_xpath(&(*output)[0], RPC_XPATH_LED_OPER_MESSAGE);
    if (SR_ERR_OK != rc) {
        sr_val_set_str_data(&(*output)[0], SR_STRING_T, (0 == strlen(acBufMsg)) ? "Internal Error" : acBufMsg);
    } else {
        sr_val_set_str_data(&(*output)[0], SR_STRING_T, RPC_STATUS_SUCCESS);
    }

    *output_cnt = 1;

    /* record user log */
    if (SR_ERR_OK == rc) {
        snprintf(acBufMsg, sizeof(acBufMsg), "%s,%d,%s,%s",
                 stLedOper.bEnabled ? DSC_EXT_LED_OPERATION_ENABLED_STR : DSC_EXT_LED_OPERATION_DISABLED_STR,
                 stLedOper.usInterval,
                 stLedOper.acSrcComponent, stLedOper.acSrcLed);
        DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, USER_LOG_TYPE_ID_SET_LAMPTEST, acBufMsg);
    }

    return SR_ERR_OK;
}

int
Led_RegisterRpc(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    bool bIsInstall = false;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    for (int i = 0; i < (sizeof(m_astLedRpcSub) / sizeof(ST_RPC_SUB)); i++) {
        bIsInstall = true;
        if (m_astLedRpcSub[i].pcModuleName != NULL) {
            bIsInstall = DSC_IsSchemaInstalled(pstSess, m_astLedRpcSub[i].pcModuleName);
        }

        if (true == bIsInstall) {
            rc = DSC_RpcSubscribe(m_astLedRpcSub[i].pcXpath, m_astLedRpcSub[i].pFCallback, (void *)pstSess);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_rpc_subscribe: %s: %s", m_astLedRpcSub[i].pcXpath, sr_strerror(rc));
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Sub rpc: %s succ", m_astLedRpcSub[i].pcXpath);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "Not install: %s", m_astLedRpcSub[i].pcModuleName);
        }
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);
    return rc;
}

