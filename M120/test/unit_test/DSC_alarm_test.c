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

#define SESSION_NAME ("CmockaTest")

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

//static void
//buildAlarmList_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    ALARM_INFO_ST *ppstAlarm = NULL;
//    sr_val_t *val = NULL;
//    size_t count = 0;
//    const size_t cnt = 0;
//    rc = buildAlarmList(&val, &cnt, &ppstAlarm, &count);
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_GetAlarmList_test(void **state)
{
    int rc = SR_ERR_OK;
    ALARM_INFO_ST *ppstAlarm;
    size_t count = 0;
    rc = DSC_GetAlarmList(&ppstAlarm, &count);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetAlarmList_test),
//		cmocka_unit_test(buildAlarmList_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}


