#include <stdio.h>
#include <signal.h>
#include "util_inc.h"
#include "DSC_inc.h"
#include "ops_callback.h"
#include "ops_protocol.h"

#define SINGLETON_PROCESS_OPSD        "/var/run/opsd.pid"
#define OPSD_START_SESSION_TIMEOUT     (30)

static bool m_bExitOpsdLoop = false;


static void
__SignalHandler(int iSignum)
{
    LOG_WRITE(EN_LOG_ERROR, "receive signal(%d).", iSignum);
    m_bExitOpsdLoop = true;
}

static int
__OPS_RegisterSubscribe(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;

    rc = DSC_ChangeSubscribe(MODULE_NS_OPS, OPS_XPATH_HEADER, OPS_OpsChangeCb, pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_ChangeSubscribe: %s: %s", OPS_XPATH_HEADER, sr_strerror(rc));
    }

    rc = DSC_GetItemsSubscribe(MODULE_NS_OPS, OPS_XPATH_HEADER, OPS_OpsDataProviderCb, pstSess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItemsSubscribe: %s: %s", OPS_XPATH_HEADER, sr_strerror(rc));
    }

    LOG_WRITE(EN_LOG_DEBUG, "Sub subtree change: %s succ", OPS_XPATH_HEADER);

    rc = DSC_RpcSubscribe(RPC_XPATH_OPS_MANUAL_TO_PORT, OPS_RpcOpsCb, pstSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_RpcSubscribe %s: %s", RPC_XPATH_OPS_MANUAL_TO_PORT, sr_strerror(rc));
        goto cleanup;
    }

    rc = DSC_ChangeSubscribe(DATA_MODULE_TCA, TCA_XPATH_HEADER, OPS_TcaChangeCb, pstSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_ChangeSubscribe %s: %s", TCA_XPATH_HEADER, sr_strerror(rc));
        goto cleanup;
    }

    /* register dev event */
    rc = UTIL_RegEventCb(DEF_OPSD_EVENT_SOCKET_PATH, OPS_OpsDevEventCb);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by UTIL_RegEventCb: %s", sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    return rc;
}

static int
__OPS_StartOpsd()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstStartup = NULL;
    sr_session_ctx_t *pstRunning = NULL;
    sr_session_ctx_t *pstOper = NULL;


    /* start session */
    rc = DSC_StartDeamonSess("opsd", OPSD_START_SESSION_TIMEOUT, &pstStartup, &pstRunning, &pstOper);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "start agent session failed.");
        goto cleanup;
    }

    /* Start running seesion */
    DSC_SwitchToRunningSess();

    /* Check if the product supports the feature of auto control. */
    if (DSC_IsSchemaInstalled(pstRunning, MODULE_NS_OPS)) {
    } else {
        LOG_WRITE(EN_LOG_ERROR, "This product not support protection.");
        rc = SR_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* init alarm */
    OPS_OpsAlmInit(pstStartup, pstRunning, true);

    /* register subscription */
    rc = __OPS_RegisterSubscribe(pstRunning);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "register subscribe failed.");
        goto cleanup;
    }

    OPS_OpsAlmInit(pstStartup, pstRunning, false);

    /* start ops server threads */
    if (OPLK_OK != OPS_Server_StartThreads()) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "start OPS server failed.");
        goto cleanup;
    }

    /* start ops client threads */
    if (OPLK_OK != OPS_Client_StartThreads()) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "start OPS client failed.");
        goto cleanup;
    }

cleanup:
    return rc;
}

static void
__OPS_StopOpsd()
{
    /* 1. unregister subscription */
    /* OPS_UnRegisterSubscribe(); */

    /* 2. stop egress / ingress threads */
    OPS_Client_StopThreads();
    OPS_Server_StopThreads();

    /* 3. alarm free */
    OPS_OpsAlmFree();

    /* 4. free sysrepo */
    DSC_StopSess();
}


int
main()
{
    int rc = OPLK_ERR;
    int iPidFd = 0;

    /* 1. init client log */
    rc = UTIL_InitClientLog();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "create log client socket failed.");
        goto cleanup;
    }

    /* 2. create single instance process */
    rc = UTIL_CreatePidFile(SINGLETON_PROCESS_OPSD, &iPidFd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "opsd process has already existed.");
        goto cleanup;
    }

    /* 3. register signal */
    signal(SIGINT, __SignalHandler);        /* catch kill signal or ctrl-c*/
    signal(SIGTERM, __SignalHandler);
    signal(SIGUSR1, __SignalHandler);       /* debug */

    LOG_WRITE(EN_LOG_NOTICE, "++ Opsd start");

    /* 4. start ops functions */
    rc = __OPS_StartOpsd();
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "start opsd failed.");
        goto cleanup;
    }

    /* 5. register heartbeat enable to monitor process */
    UTIL_HeartbeatCfgEnalbe(1);

    LOG_WRITE(EN_LOG_NOTICE, "++ Opsd loop");

    /* 6. send heartbeat to monitor every 1 seconds */
    while (false == m_bExitOpsdLoop) {
        DEF_SLEEP_S(1);
        UTIL_SendHeartbeat();
    }

cleanup:
    /* stop ops functions */
    __OPS_StopOpsd();

    /* delete the flag of single instance process */
    UTIL_DelPidFile(SINGLETON_PROCESS_OPSD, iPidFd);

    /* close client heartbeat socket */
    UTIL_CloseHeartBeat();

    LOG_WRITE(EN_LOG_NOTICE, "-- Opsd end");

    /* close client log socket */
    UTIL_CloseClientLog();

    return rc;
}
