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
DSC_SetTdcmFrequency_test(void **state)
{
    int rc = SR_ERR_OK;
    double d32Frequency = 191300.000;
    rc = DSC_SetTdcmFrequency(TDCM_CFG_KEY, d32Frequency);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetTdcmDispersn_test(void **state)
{
    int rc = SR_ERR_OK;
    double d32Dispersn = 5.00;
    rc = DSC_SetTdcmDispersn(TDCM_CFG_KEY, d32Dispersn);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetTdcmDispersnStep_test(void **state)
{
    int rc = SR_ERR_OK;
    double dRoughStep = 40.00;
    double dSmoothStep = 5.00;
    rc = DSC_SetTdcmDispersnStep(TDCM_CFG_KEY, dRoughStep, dSmoothStep);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetTdcmDispersnStep_test(void **state)
{
    int rc = SR_ERR_OK;
    double dRoughStep;
    double dSmoothStep;
    rc = DSC_GetTdcmDispersnStep(TDCM_CFG_KEY, &dRoughStep, &dSmoothStep);
    printf("Rough: %f\n Smooth: %f\n", dRoughStep, dSmoothStep);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetTdcmInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    ST_TDCM_INFO sTdcmInfo;
    memset(&sTdcmInfo, 0x00, sizeof(sTdcmInfo));
    rc = DSC_GetTdcmInfo(TDCM_CFG_KEY, &sTdcmInfo, sizeof(sTdcmInfo));
    assert_int_equal(rc, SR_ERR_OK);
    assert_in_range(sTdcmInfo.dChlSpacing, 0, 20);
    assert_in_range(sTdcmInfo.dDispersn, 1, 100);
    assert_in_range(sTdcmInfo.dDispersnActual, 1, 405000); /*range unknown*/
    assert_in_range(sTdcmInfo.dDispsnRoughStep, 1, 100);
    assert_in_range(sTdcmInfo.dDispsnSmoothStep, 1, 100);
    assert_in_range(sTdcmInfo.dFirstFreqncy, 10000.00, 196200.00);  /*range unknown*/
    assert_in_range(sTdcmInfo.dFrequency, 191300.000, 196200.000);
    assert_in_range(sTdcmInfo.dMaxDispersion, -10000.000, 10000.000);
    assert_in_range(sTdcmInfo.dMinDispersion, -10000.000, 10000.000);
/*    assert_in_range(sTdcmInfo.u32ChlNumber, 0, 3859419269);  / *range unknown* / */
}

static void
DSC_TdcmScan_test(void **state)
{
    int rc = SR_ERR_OK;
    double dStart = 0.00;
    double dStop = 20.00;
    double dStep = 5.00;
    rc = DSC_TdcmScan(dStart, dStop, dStep);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_TdcmScanBySpanloss_test(void **state)
{
    int rc = SR_ERR_OK;
    double dSpanloss = 5.00;  /*range unknown*/
    rc = DSC_TdcmScanBySpanloss(dSpanloss);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_SetTdcmFrequency_test),
        cmocka_unit_test(DSC_SetTdcmDispersn_test),
        cmocka_unit_test(DSC_GetTdcmDispersnStep_test),
        cmocka_unit_test(DSC_SetTdcmDispersnStep_test),
        cmocka_unit_test(DSC_GetTdcmInfo_test),
        cmocka_unit_test(DSC_TdcmScan_test),
        cmocka_unit_test(DSC_TdcmScanBySpanloss_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
