/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_queue.c
   Description : util queue
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
#include "sysrepo.h"
#include "DSC_inc.h"
#include <sys/time.h>
#include <unistd.h>

#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"
#include "DevOscData.h"
#include "DevEdfaCApi.h"
#include "DevEdfaData.h"
#include "DevOpsCApi.h"
#include "DevOcmCApi.h"
#include "DevTdcmCApi.h"
#include "DevOscCApi.h"
#include "DevOtdrCApi.h"
#include "oplkupgd.h"
#include "oplkalm.h"
#include "operationd_queue.h"
#include "operationd_alm.h"
#include "operationd_config_mfg.h"
#include "oplkipc.h"
#include "DevWssCApi.h"
#include "DevOcsCApi.h"

static sr_conn_ctx_t *m_queue_conn = NULL;
static sr_session_ctx_t *m_queue_session = NULL;

static bool m_bStopQueueLoop = false;
static int8_t m_cQueueIdx = 0;
static pthread_attr_t m_attrQueueProc;
static bool m_bLamptestFlag = false;
static int m_iThreadHeartBeartId = 0;
static bool m_abCardResetFlag[DSC_SLOT_NUM + 1] = {false};

/*luowei modify*/
static void
__ExecModuleReset(int iType, char *pcResetOption)
{
    int iResetOption = 0;
    int rc = OPLK_OK;
    uint32_t ulSlotIndex = DSC_SLOT_NUM;

    if (RST_CARD_OPS == iType) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_OPS)->ulSlotIndex;
    } else if ((RST_CARD_OCM == iType) || (RST_CARD_EDFA == iType) ||
               (RST_CARD_AMP == iType) || (RST_CARD_OTDR == iType)) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_AMP)->ulSlotIndex;
    } else if ((RST_CARD_OCM_P == iType) || (RST_CARD_EDFA_P == iType) ||
               (RST_CARD_AMP_P == iType) || (RST_CARD_OTDR_P == iType)) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_AMP_P)->ulSlotIndex;
    } else if (RST_CARD_OCS == iType) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_LC_OCS)->ulSlotIndex;
    } else if (RST_CARD_OPS4_A == iType) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_OPS_4_A)->ulSlotIndex;
    } else if (RST_CARD_OPS4_B == iType) {
        ulSlotIndex = DSC_GetComponentInfoByName(DEF_OCP_COMPONENT_KEY_OPS_4_B)->ulSlotIndex;
    }

    /*
     * Attention:
     * We do not need to control LEDs status for the slot modules before restart,
     * because the LEDs were controlled by the module itself for OLSP project.
     * This would be different and need to change for other project.
     * (Mao Lu, 2022/11/21)
     */
    /* SLOT_SetLed(ulSlotIndex, 0, LED_STATUS_GREEN_BLINK); */
    if (0 == strcmp(pcResetOption, COLD_RESTART_STR)) {
        iResetOption = RST_MODE_COLD;
    } else {
        iResetOption = RST_MODE_WARM;
    }

    switch (iType) {
    case RST_CARD_OCM:
        rc = OCM_Reset(DEV_TYPE_OCM, iResetOption);
        break;
    case RST_CARD_OCM_P:
        rc = OCM_Reset(DEV_TYPE_P_OCM, iResetOption);
        break;
    case RST_CARD_EDFA:
        rc = EDFA_Reset(DEV_TYPE_BA, iResetOption);
        break;
    case RST_CARD_EDFA_P:
        rc = EDFA_Reset(DEV_TYPE_P_BA, iResetOption);
        break;
    case RST_CARD_AMP:
#if 0
        if (RST_MODE_COLD == iResetOption) {
            rc = OSC_SetEnable(DEV_TYPE_OSC, false);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error OSC_SetEnable failed rc = %d", rc);
            }
        }
#endif
        rc = SLOT_Reset(ulSlotIndex, iResetOption);
        break;
    case RST_CARD_AMP_P:
#if 0
        if (RST_MODE_COLD == iResetOption) {
            rc = OSC_SetEnable(DEV_TYPE_P_OSC, false);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error OSC_SetEnable failed rc = %d", rc);
            }
        }
#endif
        rc = SLOT_Reset(ulSlotIndex, iResetOption);
        break;
    case RST_CARD_OTDR:
        rc = OTDR_Reset(DEV_TYPE_OTDR, iResetOption);
        break;
    case RST_CARD_OTDR_P:
        rc = OTDR_Reset(DEV_TYPE_P_OTDR, iResetOption);
        break;
    case RST_CARD_OPS:
        rc = OPS_Reset(DEV_TYPE_OPS, iResetOption);
        break;
    case RST_CARD_OCS:
        rc = OCS_Reset(DEV_TYPE_OCS, iResetOption);
        break;
    case RST_CARD_OPS4_A:
        rc = OPS_Reset(DEV_TYPE_OPS4A, iResetOption);
        break;
    case RST_CARD_OPS4_B:
        rc = OPS_Reset(DEV_TYPE_OPS4B, iResetOption);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "Unknown module type.");
        rc = OPLK_ERR;
        break;
    }

    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Reset module error iType = %d, iResetOption = %d", iType, iResetOption);
        /* SLOT_SetLed(ulSlotIndex, 0, LED_STATUS_GREEN); */
        OPERATIOND_SetCardRestFlag(ulSlotIndex, false);
        return;
    }

    if (RST_MODE_COLD == iResetOption) {
        switch (iType) {
        case RST_CARD_OCM:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OCM);
            break;
        case RST_CARD_OCM_P:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_P_OCM);
            break;
        case RST_CARD_EDFA:
            sleep(16);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_PA);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_BA);
            break;
        case RST_CARD_EDFA_P:
            sleep(16);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_P_PA);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_P_BA);
            break;
        case RST_CARD_AMP:
            sleep(16);
            Slot_Config(ulSlotIndex);
            break;
        case RST_CARD_AMP_P:
            sleep(16);
            Slot_Config(ulSlotIndex);
            break;
        case RST_CARD_OTDR:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OTDR);
            break;
        case RST_CARD_OTDR_P:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_P_OTDR);
            break;
        case RST_CARD_OPS:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OPS);
            break;
        case RST_CARD_OCS:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OCS);
            break;
        case RST_CARD_OPS4_A:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OPS4A);
            break;
        case RST_CARD_OPS4_B:
            sleep(10);
            Slot_ConfigModule(ulSlotIndex, DEV_TYPE_OPS4B);
            break;
        default:
            LOG_WRITE(EN_LOG_ERROR, "Unknown module type.");
            break;
        }
    } else {

        switch (iType) {
        case RST_CARD_OCM:
            sleep(10);
            break;
        case RST_CARD_OCM_P:
            sleep(10);
            break;
        case RST_CARD_EDFA:
            sleep(16);
            break;
        case RST_CARD_EDFA_P:
            sleep(16);
            break;
        case RST_CARD_AMP:
            sleep(16);
            break;
        case RST_CARD_AMP_P:
            sleep(16);
            break;
        case RST_CARD_OTDR:
            sleep(10);
            break;
        case RST_CARD_OTDR_P:
            sleep(10);
            break;
        case RST_CARD_OPS:
            sleep(10);
            break;
        case RST_CARD_OCS:
            sleep(10);
            break;
        case RST_CARD_OPS4_A:
            sleep(10);
            break;
        case RST_CARD_OPS4_B:
            sleep(10);
            break;
        default:
            LOG_WRITE(EN_LOG_ERROR, "Unknown module type.");
            break;
        }
    }

    /* SLOT_SetLed(uiLedSlot, 0, LED_STATUS_GREEN); */
    OPERATIOND_SetCardRestFlag(ulSlotIndex, false);
    return;
}


static int
__ExecLogicReset(int iDevIdx, int iResetType)
{
    int rc = SR_ERR_OK;
    int iCnt = 0;
    const int iDefautWaitTime = 30;

    LOG_WRITE(EN_LOG_NOTICE, "exec logic reset, iDevIdx:%d, iResetType:%d!", iDevIdx, iResetType);
    rc = BOARD_ResetLogic(iDevIdx, iResetType);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "BOARD_ResetLogic(%d, %d)failed.", iDevIdx, iResetType);
        goto cleanup;
    }

    DEF_SLEEP_MS(1000);

    while (iCnt <= iDefautWaitTime) {
        iCnt++;
        DEF_SLEEP_MS(1000);
        rc = BOARD_GetUpgadeStatus();
        if (OPLK_OK == rc) {
            break;
        }
    }
    LOG_WRITE(EN_LOG_NOTICE, "wait logic reset about %ds, iDevIdx:%d, iResetType:%d!", iCnt, iDevIdx, iResetType);

cleanup:

    return rc;
}


static void
__InitQueue()
{
    int rc = SR_ERR_OK;

    if (NULL == m_queue_conn) {
        rc = sr_connect(SR_CONN_DEFAULT, &m_queue_conn);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "sr_connect error: %s", sr_strerror(rc));
            return;
        }
    }

    if (NULL == m_queue_session) {
        rc = sr_session_start(m_queue_conn, SR_DS_RUNNING, &m_queue_session);
        if (SR_ERR_OK != rc) {
            sr_disconnect(m_queue_conn);
            m_queue_conn = NULL;
            LOG_WRITE(EN_LOG_ERROR, "sr_session_start error: %s", sr_strerror(rc));
            return;
        }
    }

    m_cQueueIdx = UTIL_InitQueue(0, 0);      /*create util-queue index for OPERATIOND*/

    return;
}

static int
__Lamptest(void)
{
#if 0

#define   FIXED_SLOT_ID    32

    char acSlotName[32] = {0};
    int iSlotId = 0;
    CBoardStatusData stBoardStaData;
    CSlotStatusData stSlotStaData;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    ST_CIRCUIT_PACK *pstCp = NULL;
    CP_LED_INFO_ST stOldLedInfo[CIRCUIT_PACK_NUM_MAX];
    size_t nCpCount = 0;
    size_t nLedCount = 0;
    sr_val_t *pstLedIdValues = NULL;
    int rc = SR_ERR_OK;
    int iCpLedCount = 0;
    int iLedIndex = 0;
    int i = 0;
    int j = 0;
    int k = 0;


    rc = DSC_CP_GetInfos(&pstCp, &nCpCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetInfos");
        goto cleanup;
    }

    for (i = 0; i < nCpCount; i++) {
        if (0 == strlen(pstCp[i].acRefSlotName) ||
            0 != strstr(pstCp[i].acName, CIRCUIT_PACK_KEY_FAN) ||
            0 != strstr(pstCp[i].acName, CIRCUIT_PACK_KEY_POWER)) {
            continue;
        }

        memset(acXpath, 0x00, sizeof(acXpath));
        snprintf(acXpath, sizeof(acXpath), DEVICE_XPATH_LED_KEY, pstCp[i].acName);
        rc = sr_get_items(DSC_GetOperSession(), acXpath, 0, 0, &pstLedIdValues, &nLedCount);
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_ERROR, "Not found item: %s, xpath: %s", sr_strerror(rc), acXpath);
            continue;
        }

        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "get_items: %s", acXpath);
            goto cleanup;
        }

        if (0 == strcmp(pstCp[i].acName, CIRCUIT_PACK_BOARD) ||
            0 == strcmp(pstCp[i].acName, CIRCUIT_PACK_SCC)) {
            memset(&stBoardStaData, 0, sizeof(stBoardStaData));
            rc = BOARD_GetStatusData(&stBoardStaData);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "BOARD_GetStatusData failed!");
                goto cleanup;
            }

            memset(&stOldLedInfo, 0, sizeof(stOldLedInfo));
            strcpy(stOldLedInfo[iCpLedCount].acCpName, pstCp[i].acName);
            stOldLedInfo[iCpLedCount].uiLedNum = nLedCount;
            stOldLedInfo[iCpLedCount].uiSlotId = FIXED_SLOT_ID;

            for (j = 0; j < nLedCount; j++) {
                if (NULL != strstr(pstLedIdValues[j].xpath, "/name")) {
                    strncpy(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName, \
                            pstLedIdValues[j].data.string_val, sizeof(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName) - 1);
                    rc = DSC_LedNameToIndex(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName, &iLedIndex);
                    if (rc != SR_ERR_OK) {
                        continue;
                    }

                    stOldLedInfo[iCpLedCount].astLedInfo[j].enStatus = stBoardStaData.astLed[iLedIndex].uiState;
                }
            }

            iCpLedCount++;
        } else {
            memset(acSlotName, 0, sizeof(acSlotName));
            rc = DSC_CP_GetModuleSlot(pstCp[i].acName, acSlotName);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetModuleSlot failed acCpName = %s", pstCp[i].acName);
                goto cleanup;
            }

            rc = DSC_SHELF_GetSlotsIdByName(acSlotName, &iSlotId);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_SHELF_GetSlotsIdByName acSlotName = %s", acSlotName);
                goto cleanup;
            }

            memset(&stSlotStaData, 0, sizeof(stSlotStaData));
            rc = SLOT_GetStatusData(iSlotId, &stSlotStaData);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "SLOT_GetStatusData failed slot-id = %d", iSlotId);
                goto cleanup;
            }

            strcpy(stOldLedInfo[iCpLedCount].acCpName, pstCp[i].acName);
            stOldLedInfo[iCpLedCount].uiLedNum = nLedCount;
            stOldLedInfo[iCpLedCount].uiSlotId = iSlotId;

            for (j = 0; j < nLedCount; j++) {
                if (NULL != strstr(pstLedIdValues[j].xpath, "/name")) {
                    strncpy(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName, \
                            pstLedIdValues[j].data.string_val, sizeof(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName) - 1);
                    rc = DSC_LedNameToIndex(stOldLedInfo[iCpLedCount].astLedInfo[j].acLedName, &iLedIndex);
                    if (rc != SR_ERR_OK) {
                        continue;
                    }

                    stOldLedInfo[iCpLedCount].astLedInfo[j].enStatus = stSlotStaData.astLed[iLedIndex].uiState;
                }
            }

            iCpLedCount++;
        }
    }

    m_bLamptestFlag = true;         /*reject repetitive operation*/
    IPC_LedCtrlEvent(DSC_GetRunningSession(), DEF_CTRL_MANUAL);

    for (j = 0; j < iCpLedCount; j++) {
        for (k = 0; k < stOldLedInfo[j].uiLedNum; k++) {
            rc = DSC_LedNameToIndex(stOldLedInfo[j].astLedInfo[k].acLedName, &iLedIndex);
            if (rc != SR_ERR_OK) {
                continue;
            }

            if (FIXED_SLOT_ID == stOldLedInfo[j].uiSlotId) {
                rc = BOARD_SetLedState(iLedIndex, DSC_LED_STATE_AMBER_BLINK);
            } else {
                rc = SLOT_SetLed(stOldLedInfo[j].uiSlotId, iLedIndex, DSC_LED_STATE_AMBER_BLINK);
            }
            if (rc != 0) {
                LOG_WRITE(EN_LOG_ERROR, "SLOT_SetLed slot-name = %s, slot-id = %d, led-name = %s, led-index = %d",
                          stOldLedInfo[j].acCpName, stOldLedInfo[j].uiSlotId, stOldLedInfo[j].astLedInfo[k].acLedName, iLedIndex);
            }
        }
    }

    WSS_SetLedMode(0, 1); /*0 �C normal1 �C horse race lamp mode */
    sleep(30);
    WSS_SetLedMode(0, 0);

    /*recover leds*/
    for (j = 0; j < iCpLedCount; j++) {
        for (k = 0; k < stOldLedInfo[j].uiLedNum; k++) {
            rc = DSC_LedNameToIndex(stOldLedInfo[j].astLedInfo[k].acLedName, &iLedIndex);
            if (rc != SR_ERR_OK) {
                continue;
            }

            if (FIXED_SLOT_ID == stOldLedInfo[j].uiSlotId) {
                rc = BOARD_SetLedState(iLedIndex, stOldLedInfo[j].astLedInfo[k].enStatus);
            } else {
                rc = SLOT_SetLed(stOldLedInfo[j].uiSlotId, iLedIndex, stOldLedInfo[j].astLedInfo[k].enStatus);
            }
            if (rc != 0) {
                LOG_WRITE(EN_LOG_ERROR, "SLOT_SetLed slot-name = %s, slot-id = %d, led-name = %s, led-index = %d",
                          stOldLedInfo[j].acCpName, stOldLedInfo[j].uiSlotId, stOldLedInfo[j].astLedInfo[k].acLedName, iLedIndex);
            }
        }
    }

    IPC_LedCtrlEvent(DSC_GetRunningSession(), DEF_CTRL_AUTO);
    m_bLamptestFlag = false;   /*reject repetitive operation*/

    DEF_SAFE_FREE(pstCp);
    return 0;
cleanup:
    DEF_SAFE_FREE(pstCp);
#endif
    return -1;
}

static int
__UpgdTransFile(char *pcCmd)
{
    int rc = OPLK_OK;

    if (NULL == pcCmd) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        goto ERR_LABEL;
    }

    UPGD_SetSysUpgdStatus(EN_SYS_TRANSFERRING);
    UPGD_SetSysUpgdPerc(0);

    rc = system(pcCmd);
    if (rc != OPLK_OK) {
        /* LOG_WRITE(EN_LOG_ERROR, "%s", pcCmd); */
        LOG_WRITE(EN_LOG_ERROR, "Download image from remote server failed: %d", rc); /* NOTE: Don't print password to syslog */
        UPGD_SetSysUpgdStatus(EN_SYS_TRANSFER_FAILED);
        goto ERR_LABEL;
    }

    rc = system("sync");
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "exec sync cmd failed!");
    }

    UPGD_SetSysUpgdStatus(EN_SYS_TRANSFER_COMPLETE);
    UPGD_SetSysUpgdPerc(100);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static int
__OneKeyUpgdTransFile(char *pcCmd, char *pcOutBuf)
{
    char acFileName[128] = {0};
    char acSrcBuffer[256] = {0};
    char acDestBuffer[256] = {0};
    int rc = OPLK_OK;

    if (NULL == pcCmd || NULL == pcOutBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        goto ERR_LABEL;
    }

    rc = __UpgdTransFile(pcCmd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __UpgdTransFile"); /* NOTE: Don't print password to syslog */
        goto ERR_LABEL;
    }

    memset(acFileName, 0, sizeof(acFileName));
    rc = UTIL_GetFileName(UPGD_IMAGE_TRANSFERRING_DIRECTORY, acFileName, sizeof(acFileName) - 1);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "File not exist!");
        goto ERR_LABEL;
    }

    snprintf(acSrcBuffer, sizeof(acSrcBuffer), "%s/%s", UPGD_IMAGE_TRANSFERRING_DIRECTORY, acFileName);
    snprintf(acDestBuffer, sizeof(acDestBuffer), "%s/%s", UPGD_IMAGE_EXEC_DIRECTORY, acFileName);

    rc = UTIL_CopyFile(acSrcBuffer, acDestBuffer);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CopyFile failed: %s to %s", acSrcBuffer, acDestBuffer);
        goto ERR_LABEL;
    }

    if (access((const char *)acDestBuffer, F_OK) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "File: %s not exist!", acDestBuffer);
        goto ERR_LABEL;
    }

    strcpy(pcOutBuf, acDestBuffer);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static void
__OneKeyUpgdDownloadFile(char *pcInBuf)
{
    CBoardStatusData stBoardStaData;
    ST_UPGD_INFO stUpgdInfo;
    int rc = OPLK_OK;
    bool bGetSysLedStatus = false;
    int iResetType = RST_MODE_WARM;
#define RESET_SCC_FPGA    0
#define RESET_SCC_CPLD    1

    if (NULL == pcInBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        goto ERR_LABEL;
    }

    memset(&stBoardStaData, 0, sizeof(stBoardStaData));
    rc = BOARD_GetStatusData(&stBoardStaData);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "BOARD_GetStatusData failed!");
    } else {
        bGetSysLedStatus = true;
        rc = BOARD_SetLedState(LED0, DSC_LED_STATE_GREEN_BLINK);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Init system led green error");
        }
    }
    rc = UPGD_DwnldImg(pcInBuf);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error UPGD_DwnldImg");
        goto ERR_LABEL;
    }
    rc = UPGD_Switch();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error UPGD_Switch");
        goto ERR_LABEL;
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "UPGD_Switch ok.");
    }

    /* restore upgrade led status for all SLC card */
    if (OPLK_OK != UPGD_RestoreSlotSysLed(DEF_UPGD_ALL_SLOT)) {
        LOG_WRITE(EN_LOG_ERROR, "error UPGD_RestoreSlotSysLed(%hhd)", DEF_UPGD_ALL_SLOT);
    }

    rc = UPGD_SaveBdl(pcInBuf);
    if (rc != OPLK_OK) {
        goto ERR_LABEL;
    }
    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);

    if (bGetSysLedStatus) {
        BOARD_SetLedState(LED0, stBoardStaData.astLed[DSC_LED_SYS].uiState);
    }

    /* scc-fpga must be reset at last when both scc-cpld and scc-fpga are upgraded */
    if (stUpgdInfo.bLogicMod1Upgded) {
        LOG_WRITE(EN_LOG_NOTICE, "exec logic scc-cpld warm reset!");
        OPERATIOND_SetCardRestFlag(DSC_SLOT_NUM, true);
        __ExecLogicReset(RESET_SCC_CPLD, RST_MODE_WARM);
    }

    if (stUpgdInfo.bLogicMod0Upgded) {
        /* NOTE: after scc-fpga reset, the scc-fpga register will be reverted to the default. */
        LOG_WRITE(EN_LOG_NOTICE, "exec logic scc-fpga warm reset!");
        OPERATIOND_SetCardRestFlag(DSC_SLOT_NUM, true);
        __ExecLogicReset(RESET_SCC_FPGA, RST_MODE_WARM);
    }

    BOARD_SetResetType(RST_MODE_WARM);

    if (true == stUpgdInfo.bHitless) {
        iResetType = RST_MODE_WARM;
    } else {
        LOG_WRITE(EN_LOG_WARNING, "NOT support upgrade hard reset!");
        iResetType = RST_MODE_WARM;
        /* iResetType = RST_MODE_COLD; */
    }

    rc = IPC_SendResetRpc(DSC_GetSession(), RST_CHASSIS, iResetType);
    if (OPLK_OK == rc) {
        LOG_WRITE(EN_LOG_NOTICE, "exec upgrade soft reset successful!");
    } else {
        LOG_WRITE(EN_LOG_ERROR, "exec upgrade soft reset failed!");
    }

ERR_LABEL:

    UTIL_DeleteFile((const char *)pcInBuf);

    if (bGetSysLedStatus) {
        BOARD_SetLedState(LED0, stBoardStaData.astLed[DSC_LED_SYS].uiState);
    }

    OPERATIOND_SetCardRestFlag(DSC_SLOT_NUM, false);

    if (rc != OPLK_OK) {
        OPERATIOND_SetUpgdAlm(UPGRADE_ALM_FAILED, true);
    } else {
        /* OPERATIOND_SetUpgdAlm(UPGRADE_ALM_FAILED, false); */
    }

}

static void *
__QueueProc(void *argv)
{
    ST_UTIL_QUEUE_MSG stQueueNode;
    char acDestBuffer[256] = {0};
    uint32_t ulLen = 0;
    int rc = OPLK_OK;

    memset(&stQueueNode, 0, sizeof(stQueueNode));
    /* SAVE_OPS_MODE_T stOpsMode; */

    while (!m_bStopQueueLoop) {
        sleep(1);
        UTIL_IncreaseThreadHeartBeat(m_iThreadHeartBeartId);

        ulLen = sizeof(ST_UTIL_QUEUE_MSG);
        while (0 == UTIL_GetQueueNode(m_cQueueIdx, (char *)&stQueueNode, &ulLen)) {

            switch (stQueueNode.ulMsgOid) {
            case OID_LAMP_TEST:
                LOG_WRITE(EN_LOG_NOTICE, "excute queue node OID_LAMP_TEST");
                /*lamptest trigger*/
                if (0 != __Lamptest()) {
                    LOG_WRITE(EN_LOG_ERROR, "lamptest failed!");
                }
                break;
            case OID_SAVE_OPS_MODE:
                LOG_WRITE(EN_LOG_NOTICE, "OID_SAVE_OPS_MODE");
                /* bzero(&stOpsMode, sizeof(stOpsMode)); */
                /* memcpy(&stOpsMode, stQueueNode.acMsgBuf, sizeof(stOpsMode)); */
                /* DSC_SaveOPSMode(OPS_CFG_KEY, stOpsMode); */
                break;

            /*luowei modify*/
            case OID_CARD_RESET + RST_CARD_AMP:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC AMP CARD RESET.");
                __ExecModuleReset(RST_CARD_AMP, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_AMP_P:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC AMP_P CARD RESET.");
                __ExecModuleReset(RST_CARD_AMP_P, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_EDFA:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC EDFA MODULE RESET.");
                __ExecModuleReset(RST_CARD_EDFA, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_EDFA_P:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC EDFA_P MODULE RESET.");
                __ExecModuleReset(RST_CARD_EDFA_P, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OTDR:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OTDR MODULE RESET.");
                __ExecModuleReset(RST_CARD_OTDR, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OTDR_P:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OTDR_P MODULE RESET.");
                __ExecModuleReset(RST_CARD_OTDR_P, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OCM:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OCM MODULE RESET.");
                __ExecModuleReset(RST_CARD_OCM, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OCM_P:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OCM_P MODULE RESET.");
                __ExecModuleReset(RST_CARD_OCM_P, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OPS:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OPS MODULE RESET.");
                __ExecModuleReset(RST_CARD_OPS, stQueueNode.acMsgBuf);
                break;

            case OID_CARD_RESET + RST_CARD_OCS:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OCS MODULE RESET.");
                __ExecModuleReset(RST_CARD_OCS, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OPS4_A:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OPS4_A MODULE RESET.");
                __ExecModuleReset(RST_CARD_OPS4_A, stQueueNode.acMsgBuf);
                break;
            case OID_CARD_RESET + RST_CARD_OPS4_B:
                LOG_WRITE(EN_LOG_NOTICE, "EXEC OPS4_B MODULE RESET.");
                __ExecModuleReset(RST_CARD_OPS4_B, stQueueNode.acMsgBuf);
                break;

            case OID_START_UPGD:
                LOG_WRITE(EN_LOG_NOTICE, "OID_START_UPGD");
                OPERATIOND_SetUpgdAlm(UPGRADE_ALM_IN_PROGRESS, true);
                __OneKeyUpgdDownloadFile(stQueueNode.acMsgBuf);
                break;
            case OID_ONEKEY_UPGD:
                LOG_WRITE(EN_LOG_NOTICE, "OID_ONEKEY_UPGD");
                OPERATIOND_SetUpgdAlm(UPGRADE_ALM_IN_PROGRESS, true);
                memset(acDestBuffer, 0, sizeof(acDestBuffer));
                rc = __OneKeyUpgdTransFile(stQueueNode.acMsgBuf, acDestBuffer);
                if (rc != OPLK_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "OID_ONEKEY_UPGD __OneKeyUpgdTransFile failed!");
                } else {
                    __OneKeyUpgdDownloadFile(acDestBuffer);
                }
                break;
            case OID_DOWNLOAD_IMAGE:
                rc = __UpgdTransFile(stQueueNode.acMsgBuf);
                if (rc != OPLK_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "OID_DOWNLOAD_IMAGE __UpgdTransFile failed!");
                }
                break;
            default:
                break;
            }
        }
    }

    return NULL;
}

bool
OPERATIOND_GetLamptestFlag(void)
{
    return m_bLamptestFlag;
}

/*luowei modify*/
bool
OPERATIOND_GetCardRestFlag(uint32_t uiSlotIdx)
{
    if (uiSlotIdx > DSC_SLOT_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "over range for uiSlotIdx: %d", uiSlotIdx);
        return false;
    }

    return m_abCardResetFlag[uiSlotIdx];
}

void
OPERATIOND_SetCardRestFlag(uint32_t uiSlotIdx, bool bFlag)
{
    if (uiSlotIdx > DSC_SLOT_NUM) {
        LOG_WRITE(EN_LOG_ERROR, "over range for uiSlotIdx: %d", uiSlotIdx);
        return;
    }
    m_abCardResetFlag[uiSlotIdx] = bFlag;
}

int8_t
OPERATIOND_GetQueueIdx(void)
{
    return m_cQueueIdx;
}

void
OPERATIOND_StartQueueThread()
{
    pthread_t thread_id;
    int iResult;

    __InitQueue();

    (void)pthread_attr_init(&m_attrQueueProc);
    (void)pthread_attr_setdetachstate(&m_attrQueueProc, PTHREAD_CREATE_DETACHED);

    m_bStopQueueLoop = false;
    iResult = pthread_create(&thread_id, &m_attrQueueProc, __QueueProc, NULL);
    if (0 != iResult) {
        LOG_WRITE(EN_LOG_ERROR, "start queue thread fail!");
    }

    m_iThreadHeartBeartId = UTIL_InitThreadHeartBeart();

    pthread_attr_destroy(&m_attrQueueProc);
}

void
OPERATIOND_QueueExit()
{
    if (NULL != m_queue_session) {
        sr_session_stop(m_queue_session);
        m_queue_session = NULL;
    }

    if (NULL != m_queue_conn) {
        sr_disconnect(m_queue_conn);
        m_queue_conn = NULL;
    }

    m_bStopQueueLoop = true;
    pthread_attr_destroy(&m_attrQueueProc);

    return;
}
