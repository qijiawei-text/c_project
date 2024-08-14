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

static void
DSC_GetOpsId_test(void **state)
{
    int rc = OPS_INDEX_MAX;
    char Xpath;
    /*   rc = DSC_GetOpsId(&Xpath); */
/*    printf("OPS Count:%c\n", Xpath); */
    assert_int_equal(rc, OPS_INDEX_MAX);
}


static void
DSC_GetOPSMode_test(void **state)
{
    int rc = SR_ERR_OK;
    OPS_MODE_EN mode;
    rc = DSC_GetOPSMode(OPS_CFG_KEY, &mode);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOPSMode_test(void **state)
{
    int rc = SR_ERR_OK;
    OPS_MODE_EN mode = 0;
    rc = DSC_SetOPSMode(OPS_CFG_KEY, mode);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSSwitchCnt_test(void **state)
{
    int rc = SR_ERR_OK;
    int iSwitchCnt;
    rc = DSC_GetOPSSwitchCnt(OPS_CFG_KEY, OPS_PORT_LINE_IN1, &iSwitchCnt);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSWtr_test(void **state)
{
    int rc = SR_ERR_OK;
    int OpsWtr;
    rc = DSC_GetOPSWtr(OPS_CFG_KEY, &OpsWtr);
    printf("opsWtr: %d\n", OpsWtr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOPSWtr_test(void **state)
{
    int rc = SR_ERR_OK;
    int iOpsWtr = 300;
    rc = DSC_SetOPSWtr(OPS_CFG_KEY, iOpsWtr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSState_test(void **state)
{
    int rc = SR_ERR_OK;
    OPS_STATE_EN enState = 0;
    rc = DSC_GetOPSState(1, &enState);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSPortThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    double dThreshold;
    double dHysteresis;
    rc = DSC_GetOPSPortThreshold(OPS_CFG_KEY, 3, &dThreshold, &dHysteresis);
    printf("Threshold: %lf\n Hysteresis: %lf\n", dThreshold, dHysteresis);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOPSPortThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    int iIndex = 1;
    double dThreshold = -35.00;
    double dHysteresis = 1.50;
    rc = DSC_SetOPSPortThreshold(iIndex, 3, dThreshold, dHysteresis);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOpsPower_test(void **state)
{
    int rc = SR_ERR_OK;
    double dPower;
    rc = DSC_GetOpsPower(OPS_CFG_KEY, 3, &dPower);
    printf("power: %lf\n", dPower);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSRevertive_test(void **state)
{
    int rc = SR_ERR_OK;
    bool revertive;
    rc = DSC_GetOPSRevertive(OPS_CFG_KEY, &revertive);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOPSRevertive_test(void **state)
{
    int rc = SR_ERR_OK;
    bool revertive = true;
    rc = DSC_SetOPSRevertive(OPS_CFG_KEY, revertive);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOPSCount_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count = 0;
    rc = DSC_GetOPSCount(&count);
    assert_int_equal(rc, SR_ERR_OK);
}
/*stack smashing detected*/
/* static void */
/* DSC_OpsPathEnumToString_test(void **state) */
/* { */
/*    int rc = SR_ERR_OK; */
/*    char PathStr; */
/*    rc = DSC_OpsPathEnumToString(1, &PathStr); */
/*    assert_int_equal(rc, SR_ERR_OK); */
/* } */

static void
DSC_GetOPSConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    OPS_CONFIG config;
    rc = DSC_GetOPSConfig(OPS_CFG_KEY, &config);
    assert_int_equal(rc, SR_ERR_OK);
}

/*stack smashing detected*/
/* static void */
/* DSC_OpsModuleIndexToName_test(void **state) */
/* { */
/*    int rc = SR_ERR_OK; */
/*    int iIndex = 1; */
/*    char Name; */
/*    rc = DSC_OpsModuleIndexToName(iIndex, &Name); */
/*    assert_int_equal(rc, SR_ERR_OK); */
/* } */

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetOpsId_test),
        cmocka_unit_test(DSC_GetNumOfOPS_test),
        cmocka_unit_test(DSC_GetOPSMode_test),
        cmocka_unit_test(DSC_SetOPSMode_test),
        cmocka_unit_test(DSC_GetOPSSwitchCnt_test),
        cmocka_unit_test(DSC_GetOPSWtr_test),
        cmocka_unit_test(DSC_SetOPSWtr_test),
        cmocka_unit_test(DSC_GetOPSState_test),
        cmocka_unit_test(DSC_GetOPSPortThreshold_test),
        cmocka_unit_test(DSC_SetOPSPortThreshold_test),
        cmocka_unit_test(DSC_GetOpsPower_test),
        cmocka_unit_test(DSC_GetOPSRevertive_test),
        cmocka_unit_test(DSC_SetOPSRevertive_test),
        cmocka_unit_test(DSC_GetOPSCount_test),
        cmocka_unit_test(DSC_GetOPSConfig_test),
/*		cmocka_unit_test(DSC_OpsPathEnumToString_test) */
/*		cmocka_unit_test(DSC_OpsModuleIndexToName_test) */
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}



