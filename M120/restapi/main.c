/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include <unistd.h>
#include <getopt.h>
#include <strings.h>
#include "DSC_inc.h"
#include "rest_main.h"
#include <sys/prctl.h>

#define DEBUG_MODE                       (0)    /* 1: enable debug mode, 0: disable debug mode (release) */

#define SINGLETON_PROCESS_RESTAPI_WITH_PORT        "/var/run/%s.pid"

static volatile bool m_bExitFlag = false;

/* We use the same event handler function for HTTP and HTTPS connections */
/* pvUserData is NULL for plain HTTP, and non-NULL for HTTPS (TODO). */

static void
__EventHandler(struct mg_connection *pstMgConn, int iEvent, void *pvEventData, void *pvUserData)
{
    struct mg_http_message *pstHttpMsg = NULL;

    switch (iEvent) {
    case MG_EV_HTTP_MSG:
        pstHttpMsg = (struct mg_http_message *)pvEventData;
        /* main process entry */
        REST_MainEntry(pstMgConn, pstHttpMsg);
        break;
    default:
        break;
    }

    return;
}

static void
__SignalHandler(int iSigNum)
{
    LOG_WRITE(EN_LOG_ERROR, "receive signal(%d).", iSigNum);

    m_bExitFlag = true;
}


int
Daemonize()
{
    pid_t nPid;
    int i = 0;
    int rc = 0;

    /* become a session leader to lose controlling TTY */
    nPid = fork();
    if (nPid > 0 ) {
        /* parent */
        exit(0);
    } else if (nPid < 0) {
        perror("fork");
        fprintf(stderr, "unable to fork new process\n");
        exit(1);
    }

    /* child */

    setsid();

    nPid = fork();
    if (nPid > 0 ) {
        /* parent */
        exit(0);
    } else if (nPid < 0) {
        exit(1);
    }

    for (i = sysconf(_SC_OPEN_MAX); i >= 0; i--) {
        close(i);
    }

    rc = chdir("/");
    if (rc < 0) {
        goto ERR_LABEL;
    }

    /* clear file creation mask */
    umask(0);

    return 0;

ERR_LABEL:

    return -1;
}

typedef struct LISTEN_LIST_st {
    const char *pcAppName;
    const char *pcListenPort;
} LISTEN_LIST_ST;

int
__GetListenPortByAppName(const char *pcAppName, char *pcListenPort, uint32_t uiLen)
{
    size_t i = 0, nCount = 0;
    LISTEN_LIST_ST astListenList[] = {{"restapi", HTTP_PORT_DEFAULT},
                                      {"restapi_8080", HTTP_PORT_8080},
                                      {"restapi_8081", HTTP_PORT_8081},
                                      {"restapi_8082", HTTP_PORT_8082},
                                      {"restapi_8083", HTTP_PORT_8083}};

    if (NULL == pcAppName || NULL == pcListenPort || 0 == uiLen) {
        LOG_WRITE(EN_LOG_ERROR, "Error input para");
        goto ERR_LABEL;
    }

    nCount = sizeof(astListenList) / sizeof(astListenList[0]);
    for (i = 0; i < nCount; i++) {
        if (0 == strcmp(astListenList[i].pcAppName, pcAppName)) {
            snprintf(pcListenPort, uiLen, "%s", astListenList[i].pcListenPort);
            return OPLK_OK;
        }
    }

ERR_LABEL:
    return OPLK_ERR;
}


/*
   usage:
   /usr/local/bin/restapi
   /usr/local/bin/restapi_8080
   /usr/local/bin/restapi_8081
   /usr/local/bin/restapi_8082
   /usr/local/bin/restapi_8083
 */

int
main(int argc, char *argv[])
{
    int rc = SR_ERR_OK;
    struct mg_mgr stMgr;
    int iPidFd = 0;
    char acPidFile[40] = {0};
    uint32_t uiTimes = 0;
    char acCurAppName[32] = {0};
    char acListenPort[16] = {0};
    char *pcHttpPort = NULL;

    /* daemon */
#if DEBUG_MODE
    printf("%s [%d] debug mode\r\n", __func__, __LINE__);
#else
    rc = Daemonize();
    if (rc < 0) {
        goto END_LABEL;
    }
#endif

    /* signal(SIGCHLD, SIG_IGN); */
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, __SignalHandler);
    signal(SIGINT, __SignalHandler);
    signal(SIGKILL, __SignalHandler);

    /* init log */
    UTIL_InitClientLog();

    /* get listen port by appname */
    UTIL_GetProcName(acCurAppName, sizeof(acCurAppName));
    rc = __GetListenPortByAppName((const char *)acCurAppName, acListenPort, sizeof(acListenPort));
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error AppName: %s", acCurAppName);
        goto END_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "+++++++restapi is start: %s", acListenPort);

    /* singleton daemon */
    snprintf(acPidFile, sizeof(acPidFile), SINGLETON_PROCESS_RESTAPI_WITH_PORT, acCurAppName);
    rc = UTIL_CreatePidFile(acPidFile, &iPidFd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "%s is already running", acCurAppName);
        goto END_LABEL;
    }

    /* close log print of mongoose library */
    mg_log_set(MG_LL_NONE);

    /* start http server */
    mg_mgr_init(&stMgr);

    /* Create HTTP listener */
    mg_http_listen(&stMgr, acListenPort, __EventHandler, NULL);

    /* init rest */
    pcHttpPort = strchr(acListenPort, ':');
    if (pcHttpPort != NULL) {
        pcHttpPort++;
    } else {
        pcHttpPort = acListenPort;
    }
    rc = REST_Init(pcHttpPort);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by REST_Init, listen port: %s", pcHttpPort);
        goto END_LABEL;
    }

#if DEBUG_MODE
    LOG_WRITE(EN_LOG_NOTICE, "debug mode: no heartbeat");
#else
    /* set up heartbeat monitor */
    UTIL_HeartbeatCfgChkTimeout(60);
    UTIL_HeartbeatCfgEnalbe(1);
    UTIL_SendHeartbeat();
#endif

    LOG_WRITE(EN_LOG_NOTICE, "-----%s is running", acCurAppName);
    while (false == m_bExitFlag) {
        mg_mgr_poll(&stMgr, 1000);

        uiTimes++;
        if (uiTimes > 10) {
#if DEBUG_MODE

#else
            UTIL_SendHeartbeat();
#endif
            /* LOG_WRITE(EN_LOG_NOTICE, "[%s], uiTimes: %d\r\n", m_acListenPort, uiTimes); */
            uiTimes = 0;
        }
    }

END_LABEL:

    mg_mgr_free(&stMgr);

    /*stop rest*/
    REST_Stop((pcHttpPort != NULL) ? pcHttpPort : acListenPort);

    UTIL_CloseHeartBeat();
    LOG_WRITE(EN_LOG_NOTICE, "-----%s is exit", acCurAppName);
    if (strlen(acPidFile) != 0) {
        UTIL_DelPidFile(acPidFile, iPidFd);
    }

    UTIL_CloseClientLog();

    return 0;
}
