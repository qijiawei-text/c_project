/**
 * Copyright (c) Oplink Communications, LLC
 * fahad <fahad.siddiqui@Molex.com>
 * July. 24th, 2018
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
DSC_RunPowerBalance_test(void **state)
{
    int rc = SR_ERR_OK;
    double egressPower = 1.0;
	double ingressPower = 0.0;
    rc = DSC_RunPowerBalance(egressPower, ingressPower);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetPeerIp_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcAddr = "192.168.1.2";
    rc = DSC_SetPeerIp(pcAddr, sizeof(pcAddr));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetBERThr_test(void **state)
{
    int rc = SR_ERR_OK;
    double dRough = 0.005;
	double dSmooth = 0.002;
    rc = DSC_SetBERThr(dRough, dSmooth);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetBerMutiply_test(void **state)
{
    int rc = SR_ERR_OK;
    double dMutiply = 10.00;
    rc = DSC_SetBerMutiply(dMutiply);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetDcfCdCompensation_test(void **state)
{
    int rc = SR_ERR_OK;
    double dCdCompensation = 0.00;
    rc = DSC_SetDcfCdCompensation(dCdCompensation);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetFiberCd_test(void **state)
{
    int rc = SR_ERR_OK;
    double dMin = 16.00;
    double dMax = 18.00;
    rc = DSC_SetFiberCd(dMin, dMax);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaChlTarget_test(void **state)
{
    int rc = SR_ERR_OK;
    int iIndex = 1;
    double dValue = 2.5;
    rc = DSC_SetEdfaChlTarget(iIndex, dValue);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaChlTarget(2, 6);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaChlTarget(3, 6);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetCtrlLoopState_test(void **state)
{
    int rc = SR_ERR_OK;
    CL_ALL_STATE_ST stState;
    memset(&stState, 0x00, sizeof(stState));
    rc = DSC_GetCtrlLoopState(&stState, sizeof(stState));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetCtrlLoopConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    CL_CONFIG_ST stConfig;
    memset(&stConfig, 0x00, sizeof(stConfig));
    rc = DSC_GetCtrlLoopConfig(&stConfig, sizeof(stConfig));
    assert_int_equal(rc, SR_ERR_OK);
    assert_in_range(stConfig.dBerRoughThr, 0.00001, 0.1);
    printf("stConfig.dBerRoughThr:%f\n", stConfig.dBerRoughThr);
    assert_in_range(stConfig.dBerSmoothThr, 0.00001, 0.1);
    assert_in_range(stConfig.dE1ChlTarget, 0, 23);
    assert_in_range(stConfig.dE2ChlTarget, 0, 23);
    assert_in_range(stConfig.dE3ChlTarget, 0, 23);
}

static void
DSC_SetControlState_test(void **state)
{
    int rc = SR_ERR_OK;
    char *mode = "ingress-mode";
    char *states = "0";
    rc = DSC_SetControlState(mode, states);
    assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_SetIngressMode_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    char *ingress_mode = "off";
//    rc = DSC_SetIngressMode(ingress_mode);
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_SetTdcmState_test(void **state)
{
    int rc = SR_ERR_OK;
    char *tdcm_state = "scan";
    rc = DSC_SetTdcmState(tdcm_state);
    assert_int_equal(rc, SR_ERR_OK);
    char *tdcm_state1 = "tracking";
    rc = DSC_SetTdcmState(tdcm_state1);
    assert_int_equal(rc, SR_ERR_OK);
    char *tdcm_state2 = "locked";
    rc = DSC_SetTdcmState(tdcm_state2);
    assert_int_equal(rc, SR_ERR_OK);
    char *tdcm_state3 = "failed";
    rc = DSC_SetTdcmState(tdcm_state3);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetSpanLoss_test(void **state)
{
    int rc = SR_ERR_OK;
    double dSpanLoss = 12.00;
    rc = DSC_SetSpanLoss(1, dSpanLoss);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetSpanLoss_test(void **state)
{
    int rc = SR_ERR_OK;
    double dSpanLoss;
    rc = DSC_GetSpanLoss(1, &dSpanLoss);
    printf("span loss: %f\n", dSpanLoss);
    assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_SetControlLoop_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    int iType = 1;
//    rc = DSC_SetControlLoop(iType);
//    assert_int_equal(rc, SR_ERR_OK);
//}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_RunPowerBalance_test),
		cmocka_unit_test(DSC_SetPeerIp_test),
		cmocka_unit_test(DSC_SetBERThr_test),
		cmocka_unit_test(DSC_SetBerMutiply_test),
		cmocka_unit_test(DSC_SetDcfCdCompensation_test),
		cmocka_unit_test(DSC_SetFiberCd_test),
		cmocka_unit_test(DSC_SetEdfaChlTarget_test),
		cmocka_unit_test(DSC_GetCtrlLoopState_test),
		cmocka_unit_test(DSC_SetControlState_test),
		cmocka_unit_test(DSC_GetCtrlLoopConfig_test),
//		cmocka_unit_test(DSC_SetIngressMode_test),
		cmocka_unit_test(DSC_SetTdcmState_test),
		cmocka_unit_test(DSC_SetSpanLoss_test),
		cmocka_unit_test(DSC_GetSpanLoss_test),
//		cmocka_unit_test(DSC_SetControlLoop_test)

    };
       int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
       return ret;
}

