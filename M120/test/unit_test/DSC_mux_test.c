/**
 * Copyright (c) Oplink Communications, LLC
 * fahad <fahad.siddiqui@Molex.com>
 * August. 13th, 2018
 */
#include "DSC_inc.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <regex.h>

#define SESSION_NAME ("CmockaTest")

/*DSC alarm test*/
ALARM_INFO_ST stAlarmList[] = {

};

/*DSC DSC_StartSession test*/
static int
DSC_StartSession_setup(void **state)
{
    int rc;
    /* connect to sysrepo */
    rc = DSC_StartSession(SESSION_NAME, SR_DS_RUNNING);
    assert_int_equal(rc, SR_ERR_OK);
    *state = (void *)DSC_GetSession();
    return 0;
}

static int
DSC_StartSession_teardown(void **state)
{
    int rc;
    rc = DSC_StopSession();
    assert_int_equal(rc, 0);
    /*stop function not finished*/
    return 0;
}

static void
DSC_GetMuxId_test(void **state)
{
	int mux_id = -1;
	int rc = mux_id;
    char Xpath;
    rc = DSC_GetMuxId(&Xpath);
    assert_int_equal(rc, mux_id);
}

static void
DSC_GetMuxCh_test(void **state)
{
	int mux_ch = -1;
	int rc = mux_ch;
    char Xpath;
    rc = DSC_GetMuxCh(&Xpath);
    assert_int_equal(rc, mux_ch);
}

static void
DSC_GetMuxConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    MUX_CFG_ST MuxCfg;
    memset(&MuxCfg, 0x00, sizeof(MuxCfg));
    rc = DSC_GetMuxConfig(1, &MuxCfg, sizeof(MuxCfg));
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetMuxConfig(2, &MuxCfg, sizeof(MuxCfg));
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetMuxConfig(3, &MuxCfg, sizeof(MuxCfg));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetMuxState_test(void **state)
{
    int rc = SR_ERR_OK;
    MUX_STATE_ST MuxState;
    memset(&MuxState, 0x00, sizeof(MuxState));
    rc = DSC_GetMuxState(1, &MuxState, sizeof(MuxState));
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetMuxState(2, &MuxState, sizeof(MuxState));
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetMuxState(3, &MuxState, sizeof(MuxState));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetMuxAttn_test(void **state)
{
    int rc = SR_ERR_OK;
    double dAttn = 4.00;
    rc = DSC_SetMuxAttn(1, 1, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetMuxAttn(2, 1, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetMuxAttnAll_test(void **state)
{
    int rc = SR_ERR_OK;
    double dAttn = 4.00;
    rc = DSC_SetMuxAttnAll(1, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetMuxAttnAll(2, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
    	cmocka_unit_test(DSC_GetMuxCh_test),
    	cmocka_unit_test(DSC_GetMuxId_test),
		cmocka_unit_test(DSC_GetMuxConfig_test),
		cmocka_unit_test(DSC_GetMuxState_test),
		cmocka_unit_test(DSC_SetMuxAttn_test),
		cmocka_unit_test(DSC_SetMuxAttnAll_test)

    };
       int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
       return ret;
}
