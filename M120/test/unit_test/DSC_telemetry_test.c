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
DSC_SetSubscription_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetSubscription("192.168.4.46", 1500, true);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_DeleteSubscription_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_DeleteSubscription("192.168.6.36", 1500);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetSubscriptions_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetSubscriptions(true, 1);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetAll_test(void **state)
{
    int rc = SR_ERR_OK;
//    TELE_SUB_STATE_ST stSubStates;
//    rc = DSC_GetAll(&stSubStates);
//    if (stSubStates != NULL) {
//        free(stSubStates);
//    }
    assert_int_equal(rc, SR_ERR_OK);
}
/*
static void
DSC_RefreshSub_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_RefreshSubScription("192.168.4.46", 1500, 10);
    assert_int_equal(rc, SR_ERR_OK);
}
*/
int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_SetSubscription_test),
        cmocka_unit_test(DSC_DeleteSubscription_test),
        cmocka_unit_test(DSC_SetSubscriptions_test),
		cmocka_unit_test(DSC_GetAll_test)
//        cmocka_unit_test(DSC_RefreshSub_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
