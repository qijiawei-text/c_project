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

#if 0
static void
DSC_GetOscId_test(void **state)
{
    int edfa_id = -1;
    int rc = edfa_id;
    char xpath;
    rc = DSC_GetOscId(&xpath);
    assert_int_equal(rc, edfa_id);
}
#endif

static void
DSC_SetOscOutputEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    bool stat = true;

    rc = DSC_SetOscOutputEnable(OSC1_CFG_KEY, stat);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOscRxThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr = 31.00;
    double dLowThr = 28.00;

    rc = DSC_SetOscRxThreshold(OSC1_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOscTxThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr = 31.00;
    double dLowThr = 28.00;

    rc = DSC_SetOscTxThreshold(OSC1_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetOscDropThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr = 31.00;
    double dLowThr = 28.00;

    rc = DSC_SetOscDropThreshold(OSC1_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

/* static void */
/* DSC_SetOscTargetPower_test(void **state) */
/* { */
/*	int rc = SR_ERR_OK; */
/*	double dEgressPower = 4.8; / *valid range unknown* / */
/*	DIRECTION_EN enDirection = 1; */
/*	rc = DSC_SetOscTargetPower(enDirection, dEgressPower); */
/*	assert_int_equal(rc, SR_ERR_OK); */
/* } */

static void
DSC_GetOscInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    OSC_INFO_ST stOscInfo;
    memset(&stOscInfo, 0x00, sizeof(stOscInfo));
    rc = DSC_GetOscInfo(OSC1_CFG_KEY, &stOscInfo, sizeof(stOscInfo));
    assert_int_equal(rc, SR_ERR_OK);
    assert_in_range(stOscInfo.enOscFormat, 0, 2);
}

/*
   static void
   DSC_SetJ0_test(void **state)
   {
        int rc = SR_ERR_OK;
        unsigned char *ucBuf = NULL;
        size_t size = 0;
        rc = DSC_SetJ0(1, ucBuf, size);
        assert_int_equal(rc, SR_ERR_OK);
   }

   static void
   DSC_GetJ0Tx_test(void **state)
   {
        int rc = SR_ERR_OK;
        unsigned char *ucBuf = NULL;
        size_t size = 0;
        rc = DSC_GetJ0Tx(1, ucBuf, size);
        assert_int_equal(rc, SR_ERR_OK);
   }
 */

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetOscId_test),
        cmocka_unit_test(DSC_SetOscOutputEnable_test),
        cmocka_unit_test(DSC_SetOscRxThreshold_test),
        cmocka_unit_test(DSC_SetOscTxThreshold_test),
        cmocka_unit_test(DSC_SetOscDropThreshold_test),
/*		cmocka_unit_test(DSC_SetOscTargetPower_test), */
/*		cmocka_unit_test(DSC_SetJ0_test), */
/*		cmocka_unit_test(DSC_GetJ0Tx_test), */
        cmocka_unit_test(DSC_GetOscInfo_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
