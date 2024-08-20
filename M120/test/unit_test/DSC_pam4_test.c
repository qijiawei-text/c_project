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
DSC_SetPam4RxThreshold_test(void **state)
{
	int rc = SR_ERR_OK;
	double dHighThr = -8.50;
	double dLowThr = -11.50;
	rc = DSC_SetPam4RxThreshold(dHighThr, dLowThr);
	assert_int_equal(rc, SR_ERR_OK);

}

static void
DSC_SetPam4TxThreshold_test(void **state)
{
	int rc = SR_ERR_OK;
	double dHighThr = -8.50;
	double dLowThr = -11.50;
	rc = DSC_SetPam4TxThreshold(dHighThr, dLowThr);
	assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_SetPam4BerThreshold_test(void **state)
//{
//	int rc = SR_ERR_OK;
//	double dHighThr = -31.50;  /*valid range unknown*/
//	double dLowThr = -34.50;   /*valid range unknown*/
//	rc = DSC_SetPam4BerThreshold(dHighThr, dLowThr);
//	assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_SetPam4TxEnable_test(void **state)
{
	int rc = SR_ERR_OK;
	bool bbool = NULL;
	rc = DSC_SetPam4TxEnable(bbool);
	assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetPam4WavelenNum_test(void **state)
{
	int rc = SR_ERR_OK;
	uint32_t u32WavelenNum = 41;
	rc = DSC_SetPam4WavelenNum(u32WavelenNum);
	assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_Pam4ClearStatistics_test(void **state)
{
	int rc = SR_ERR_OK;
	bool bbool = NULL;
	rc = DSC_Pam4ClearStatistics(bbool);
	assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPam4Info_test(void **state)
{
	int rc = SR_ERR_OK;
	ST_PAM4_INFO psPam4Info;
	memset(&psPam4Info, 0x00, sizeof(psPam4Info));
	rc = DSC_GetPam4Info(&psPam4Info, sizeof(psPam4Info));
	printf("HighThrRx: %lf\n LowThrRx: %lf\n HighThrRx: %lf\n LowThrRx: %lf\n", psPam4Info.dRxHighThr, psPam4Info.dRxLowThr, psPam4Info.dTxHighThr, psPam4Info.dTxLowThr);
	assert_int_equal(rc, SR_ERR_OK);
	assert_in_range(strlen(psPam4Info.acLockState), 0, 8);
	assert_in_range(strlen(psPam4Info.acBer), 0, 2047);
	assert_in_range(psPam4Info.dRxPower, -30.0, 15.0);
	assert_in_range(psPam4Info.dLaserTemp, -5.0, 70.0);
	assert_in_range(psPam4Info.dCaseTemp, -5.0, 70.0);
}

static void
DSC_Pam4Reacquire_test(void **state)
{
	int rc = SR_ERR_OK;
	bool bbool = NULL;
	rc = DSC_Pam4Reacquire(bbool);
	assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_Pam4SetFecStatus_test(void **state)
{
	int rc = SR_ERR_OK;
	rc = DSC_Pam4SetFecStatus('1'); /*value for on = 1 and off = 2 */
	assert_int_equal(rc, SR_ERR_OK);
	rc = DSC_Pam4SetFecStatus('2');
	assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_Pam4ResetMutify_test(void **state)
{
	int rc = SR_ERR_OK;
	bool bBoolean = true;
	rc = DSC_Pam4ResetMutify(bBoolean);
	assert_int_equal(rc, SR_ERR_OK);

}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
    	cmocka_unit_test(DSC_SetPam4RxThreshold_test),
    	cmocka_unit_test(DSC_SetPam4TxThreshold_test),
//		cmocka_unit_test(DSC_SetPam4BerThreshold_test),
		cmocka_unit_test(DSC_SetPam4TxEnable_test),
		cmocka_unit_test(DSC_Pam4ClearStatistics_test),
		cmocka_unit_test(DSC_GetPam4Info_test),
		cmocka_unit_test(DSC_Pam4Reacquire_test),
		cmocka_unit_test(DSC_Pam4SetFecStatus_test),
		cmocka_unit_test(DSC_SetPam4WavelenNum_test),
		cmocka_unit_test(DSC_Pam4ResetMutify_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
