/**
 * Copyright (c) Oplink Communications, LLC
 * fahad <fahad.siddiqui@Molex.com>
 * July. 27th, 2018
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
DSC_GetTcaList_test(void **state)
{
    int rc = SR_ERR_OK;
    TCA_INFO_ST *pstTca;
    size_t count;
    rc = DSC_GetTcaList(&pstTca, &count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetTcaIdx_test(void **state)
{
    int rc = SR_ERR_OK;
    char *xpath = NULL;
    rc = DSC_GetTcaIdx(xpath);
    assert_int_not_equal(rc, SR_ERR_OK);
}

static void
DSC_SetTcaThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr = -41.50;
	double dLowThr = -44.50;
    rc = DSC_SetTcaThreshold(TCA_IDX_OSC1_RX, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetTcaThreshold(TCA_IDX_EDFA1_INPUT, -24.00, -27.00);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetTcaThreshold(TCA_IDX_EDFA1_OUTPUT, -11.50, -14.50);
    assert_int_equal(rc, SR_ERR_OK);
//    rc = DSC_SetTcaThreshold(TCA_IDX_PAM4_RX, -11.50, -14.50);
//    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetTcaThresh_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr;
	double dLowThr;
	TCA_INFO_ST stInfo;
    rc = DSC_GetTcaThresh(1, &stInfo, &dHighThr, &dLowThr);
    printf("HighThr: %f \n LowThr : %f\n", dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}


int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetTcaList_test),
		cmocka_unit_test(DSC_GetTcaIdx_test),
		cmocka_unit_test(DSC_SetTcaThreshold_test),
		cmocka_unit_test(DSC_GetTcaThresh_test)


    };
       int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
       return ret;
}
