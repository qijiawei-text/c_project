/**
 * @oplk_dsc_api_test.c
 * @author Liuyuze <33716326@qq.com>
 * @brief Dsc Api Library unit tests.
 */

/*ALL  DSC  lib*/
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

/* #define SESSION_NAME ("CmockaTest") */
#define SESSION_NAME ("agentExample")


/*DSC alarm test*/
ALARM_INFO_ST stAlarmList[] = {

};

static int
DSC_Alarm_setup(void **state)
{

    return 0;
}

static int
DSC_Alarm_teardown(void **state)
{
    return 0;
}

static void
DSC_Alarm_test(void **state)
{
}


/*DSC DSC_StartSession test*/
#if 0
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
#endif

static void
DSC_StartSession_test(void **state)
{
    int rc;
    /* Start session */
    rc = DSC_StartSession(SESSION_NAME, SR_DS_RUNNING);
    assert_int_equal(rc, SR_ERR_OK);

    /* Stop session */
    rc = DSC_StopSession();
    assert_int_equal(rc, 0);

}


int
main(int argc, char **argv)
{

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(DSC_StartSession_test, NULL, NULL),
        cmocka_unit_test_setup_teardown(DSC_Alarm_test, DSC_Alarm_setup, DSC_Alarm_teardown)
    };
    /* watchdog_start(300); */
    int ret = cmocka_run_group_tests(tests, NULL, NULL);
    /* watchdog_stop(); */
    return ret;
}
