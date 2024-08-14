/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : test_persistd.c
   Description : Save config/log/pm to flash or disk.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include "sysrepo.h"
#include "UTIL_inc.h"
#include "oplkipc.pb-c.h"
#include "oplkipc.h"
#include "test_persistd.h"


static sr_subscription_ctx_t *conf_sub = NULL;

static int
__system_cb(sr_session_ctx_t *session, const char *module_name, sr_notif_event_t event, void *private_ctx)
{
	LOG_WRITE(OPT_LOG, "++__system_cb");

	return 0;
}


int
main()
{
    int rc = SR_ERR_OK;
    sr_conn_ctx_t *pstConn = NULL;
    sr_session_ctx_t *pstStartup = NULL;
    sr_session_ctx_t *pstRunning = NULL;
	
    LOG_WRITE(OPT_LOG, "++ Test_Persistd start");
	
	/* connect to sysrepo */    
	rc = sr_connect("test_persistd", SR_CONN_DAEMON_REQUIRED, &pstConn);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_connect: %s", sr_strerror(rc));
        goto cleanup;
    }

	/* start startup session */
    rc = sr_session_start(pstConn, SR_DS_STARTUP, SR_SESS_DEFAULT, &pstStartup);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_session_start startup: %s", sr_strerror(rc));
        goto cleanup;
    }
		
	/* start running session */
    rc = sr_session_start(pstConn, SR_DS_RUNNING, SR_SESS_DEFAULT, &pstRunning);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_session_start running: %s", sr_strerror(rc));
        goto cleanup;
    }

	#if 0
	/* send reset rpc */
	rc = IPC_SendResetRpc(pstRunning, 2, 2);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by IPC_SendResetRpc: %s", sr_strerror(rc));
        goto cleanup;
    }
	#endif

	#if 0
	/* event */
	rc = IPC_PlugEvent(pstRunning);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by IPC_PlugEvent: %s", sr_strerror(rc));
        goto cleanup;
    }
	#endif

	/* change config */

    rc = sr_module_change_subscribe(pstStartup, "ietf-system",
            __system_cb,
            NULL, 0, SR_SUBSCR_CTX_REUSE | SR_SUBSCR_NO_ABORT_FOR_REFUSED_CFG, &conf_sub);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by subscribe ietf-system module config change: %s", sr_strerror(rc));
        goto cleanup;
    }
	
    sr_val_t _v = {0};
    _v.type = SR_STRING_T;
    _v.data.string_val = "HOST1";
    rc = sr_set_item(pstRunning, "/ietf-system:system/hostname", &_v, SR_EDIT_DEFAULT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by set_string_item: %s", sr_strerror(rc));
        goto cleanup;
    }

	rc = sr_commit(pstRunning);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_commit: %s", sr_strerror(rc));
        goto cleanup;
    }


	rc = sr_delete_item(pstRunning, "/ietf-system:system/hostname", SR_EDIT_STRICT);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_delete_item: %s", sr_strerror(rc));
        goto cleanup;
    }

	rc = sr_commit(pstRunning);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(ERR_LOG, "Error by sr_commit: %s", sr_strerror(rc));
        goto cleanup;
    }

cleanup:
	
    sr_session_stop(pstStartup);
    sr_session_stop(pstRunning);
    sr_disconnect(pstConn);

    LOG_WRITE(OPT_LOG, "-- Test_Persistd end");

    return 0;
}
