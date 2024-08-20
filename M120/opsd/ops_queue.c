#include "ops_common.h"
#include "ops_queue.h"
#include "ops_protocol.h"
#include "util_queue.h"


#define OID_UPDATE_OPS_CFG  (0x00001001)

static int8_t m_ucQueueIndex = 0;
static bool m_bQueueLoop = true;

static int
__OPS_UpdateCfg(uint8_t ucState)
{
    int rc = OPLK_OK;
    OPS_CONFIG_ST cfg;

    /* when a far end LO/SF-P is received, we need to reset force-to-port to NONE in sysrepo.   */
    /* Exceptionally, do not reset sysrepo when local state is LO.                              */
    if (ucState <= APS_STATE_FS) {
        return rc;
    }

    if (SR_ERR_OK != DSC_GetOpsConfig(OPS_CFG_KEY, &cfg, sizeof(cfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsConfig");
        return OPLK_ERR;
    }

    /* Note: When module is in LO state, nothing to do.                                         */
    if (SWITCH_TO_SECONDARY != cfg.enForceToPort) {
        return rc;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Reset force-to-port to NONE in sysrepo, requested state = %u", ucState);

    if (SR_ERR_OK != DSC_SetForceSwitch(OPS_CFG_KEY, STR_NONE)) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SetForceSwitch");
        rc = OPLK_ERR;
    }

    return rc;
}

static void *
__OPS_QueueProc(void *argv)
{
    uint32_t ulLen = 0;
    ST_UTIL_QUEUE_MSG stQueueNode;

    memset(&stQueueNode, 0, sizeof(stQueueNode));

    while (m_bQueueLoop) {
        ulLen = sizeof(ST_UTIL_QUEUE_MSG);
        while (0 == UTIL_GetQueueNode(m_ucQueueIndex, (char *)&stQueueNode, &ulLen)) {
            switch (stQueueNode.ulMsgOid) {
            case OID_UPDATE_OPS_CFG:
                if (sizeof(uint8_t) == stQueueNode.ulMsgLen) {
                    __OPS_UpdateCfg(stQueueNode.acMsgBuf[0]);
                }
                break;
            default:
                break;
            }
        }

        DEF_SLEEP_MS(100);
    }

    return NULL;
}

int
OPS_QueueStart(void)
{
    pthread_t thread_id = -1;
    m_ucQueueIndex = UTIL_InitQueue(0, 0);

    if (0 != pthread_create(&thread_id, NULL, __OPS_QueueProc, NULL)) {
        LOG_WRITE(EN_LOG_ERROR, "start OPS queue thread fail!");
        return OPLK_ERR;
    }

    return OPLK_OK;
}

void
OPS_QueueStop(void)
{
    m_bQueueLoop = false;
    UTIL_CloseQueue(m_ucQueueIndex);
}

void
OPS_StateAdd(uint8_t ucState)
{
    LOG_WRITE(EN_LOG_DEBUG, "OPS_StateAdd ++");
    ST_UTIL_QUEUE_MSG msg = {OID_UPDATE_OPS_CFG, sizeof(uint8_t), {(char)ucState}};

    UTIL_AddQueueNode(m_ucQueueIndex, (char *)&msg, sizeof(msg));
    LOG_WRITE(EN_LOG_DEBUG, "OPS_StateAdd --");
}