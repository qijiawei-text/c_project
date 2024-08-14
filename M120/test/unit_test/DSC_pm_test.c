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

//static void
//DSC_SetPmGranularity_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    rc = DSC_SetPmGranularity();
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_GetCurrentPM_test(void **state)
{
    int rc = SR_ERR_OK;
    PM_DATA_ST stPmData;
    size_t count;
    memset(&stPmData, 0x00, sizeof(stPmData));
    rc = DSC_GetCurrentPM(0, &stPmData, &count);
    printf("startTime: %s\n EndTime: %s\n", stPmData.acStartTime, stPmData.acEndTime);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetCurrentPM(1, &stPmData, &count);
    assert_int_equal(rc, SR_ERR_OK);
}
/* (Segmentation fault (core dumped))
static void
DSC_GetHistoryPM_test(void **state)
{
    int rc = SR_ERR_OK;
    PM_DATA_ST stPmItem;
    size_t count;
//    memset(&stPmItem, 0x00, sizeof(stPmItem));
    rc = DSC_GetHistoryPM(2, &stPmItem, &count);
    assert_int_equal(rc, SR_ERR_OK);
//    rc = DSC_GetHistoryPM(1, &stPmItem, &count);
//    assert_int_equal(rc, SR_ERR_OK);
}
*/

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
//        cmocka_unit_test(DSC_SetPmGranularity_test),
		cmocka_unit_test(DSC_GetCurrentPM_test),
//		cmocka_unit_test(DSC_GetHistoryPM_test)
    };
     int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
     return ret;
 }
