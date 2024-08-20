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
DSC_Edfa_Info_test(void **state)
{
    int rc = SR_ERR_OK;
    EDFA_STATE_ST stEdfaState;
    EDFA_CFG_ST stEdfaCfg;

    rc = DSC_GetEdfaConfig(EDFA_PA_CFG_KEY, &stEdfaCfg, sizeof(stEdfaCfg));
    assert_int_equal(rc, SR_ERR_OK);
    /* check target-gain, valid length [6.0, 35.0] */
    assert_in_range(stEdfaCfg.dGainSetPoint, 6, 35);
    /* check target-gain-tilt, valid length [-3.0, 3.0] */
    if (stEdfaCfg.dTiltSetPoint < 0) {
        assert_in_range(-stEdfaCfg.dTiltSetPoint, 0, 3);
    } else {
        assert_in_range(stEdfaCfg.dTiltSetPoint, 0, 3);
    }

    rc = DSC_GetEdfaState(EDFA_BA_CFG_KEY, &stEdfaState, sizeof(stEdfaState));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaGain_test(void **state)
{
    int rc = SR_ERR_OK;
    /* valid range [6.0, 35.0] */
    double dEdfaGain = 8.0;
    rc = DSC_SetEdfaGain(EDFA_PA_CFG_KEY, dEdfaGain);
    assert_int_equal(rc, SR_ERR_OK);
    dEdfaGain = 35.0;
    rc = DSC_SetEdfaGain(EDFA_PA_CFG_KEY, dEdfaGain);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaGainRange_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaGainRange(EDFA_PA_CFG_KEY, EDFA_GAIN_RANGE_HIGH);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaGainRange(EDFA_PA_CFG_KEY, EDFA_GAIN_RANGE_LOW);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaTilt_test(void **state)
{
    /* valid range [-3.0, 3.0] */
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaTilt(EDFA_PA_CFG_KEY, 1.0);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaTilt(EDFA_PA_CFG_KEY, 0.0);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaOutputEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaOutputEnable(EDFA_PA_CFG_KEY, true);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaOutputEnable(EDFA_PA_CFG_KEY, false);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaAutolosEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaAutolosEnable(EDFA_PA_CFG_KEY, true);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaAutolosEnable(EDFA_BA_CFG_KEY, true);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaAprEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaAprEnable(EDFA_PA_CFG_KEY, true);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaAprPower_test(void **state)
{
    /* valid range not known */
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, 1.0);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, 99.0);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaPlimEnable_test(void **state)
{
    /* valid range not known */
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, true);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, false);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaPlimPower_test(void **state)
{
    /* valid range not known */
    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, 1.0);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaAprPower(EDFA_PA_CFG_KEY, 12.0);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_ChangeEdfaName_test(void **state)
{

    int rc = SR_ERR_OK;
    char *pcOldName = "oldname";
    char *pcNewName = "newname";
    rc = DSC_ChangeEdfaName(pcOldName, pcNewName);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_EdfaGainRangeToEnum_test(void **state)
{

    int rc = SR_ERR_OK;
    char *pcStr = NULL;
    rc = DSC_EdfaGainRangeToEnum(pcStr);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_EdfaGainRangeToEnum("0");
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_EdfaGainRangeToEnum("1");
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_EdfaGainRangeToEnum("2");
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetEdfaConfig_test(void **state)
{

    int rc = SR_ERR_OK;
    EDFA_CFG_ST stEdfaCfg;
    memset(&stEdfaCfg, 0x00, sizeof(stEdfaCfg));
    rc = DSC_GetEdfaConfig(EDFA_PA_CFG_KEY, &stEdfaCfg, sizeof(stEdfaCfg));
    assert_int_equal(rc, SR_ERR_OK);
    assert_in_range(stEdfaCfg.dGainSetPoint, 5.00, 35.00);
    assert_in_range(stEdfaCfg.dTiltSetPoint, -5.00, 5.00);
/*    assert_in_range(stEdfaCfg.dInLosHighThr, 31.50, 99000000000000000000000000.00); */
/*    assert_in_range(stEdfaCfg.dInLosLowThr, 20.00, 28.50); */
/*    assert_in_range(stEdfaCfg.dOutLosHighThr, 00.00, 11.50); / * valid range not known * / */
/*    assert_in_range(stEdfaCfg.dOutLosLowThr, 00.00, 8.50); / * valid range not known * / */
/*    assert_in_range(stEdfaCfg.dDcmInHighThr, 00.00, 74.00); / * valid range not known * / */
/*    assert_in_range(stEdfaCfg.dDcmInLowThr, 00.00, 20.00); / * valid range not known * / */
/*    assert_in_range(stEdfaCfg.dDcmOutHighThr, 00.00, 61.00); / * valid range not known * / */
/*    assert_in_range(stEdfaCfg.dDcmOutLowThr, 00.00, 11.00); / * valid range not known * / */
    assert_in_range(stEdfaCfg.dAprPower, 1.00, 99.00); /* valid range not known */
    assert_in_range(stEdfaCfg.dPlimPower, 0.00, 12.00); /* valid range not known */
}

static void
DSC_GetEdfaState_test(void **state)
{

    int rc = SR_ERR_OK;
    EDFA_STATE_ST stEdfaState;
    rc = DSC_GetEdfaState(EDFA_PA_CFG_KEY, &stEdfaState, sizeof(stEdfaState));
    assert_int_equal(rc, SR_ERR_OK);
}

/*static void  (Not in use)
   DSC_SetEdfaBackRefRatio_test(void **state)
   {

        int rc = SR_ERR_OK;
        rc = DSC_SetEdfaBackRefRatio(EDFA_PA_CFG_KEY, 99.0);
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SetEdfaInLosThreshold_test(void **state)
{

    int rc = SR_ERR_OK;
    double dHighThr = 31.50;
    double dLowThr = 28.50;
    rc = DSC_SetEdfaInLosThreshold(EDFA_PA_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaOutLosThreshold_test(void **state)
{

    int rc = SR_ERR_OK;
    double dHighThr = 11.50;
    double dLowThr = 08.50;
    rc = DSC_SetEdfaOutLosThreshold(EDFA_PA_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaDcmInLosThreshold_test(void **state)
{

    int rc = SR_ERR_OK;
    double dHighThr = 74.00;
    double dLowThr = 00.00;
    rc = DSC_SetEdfaDcmInLosThreshold(EDFA_PA_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaDcmOutLosThreshold_test(void **state)
{

    int rc = SR_ERR_OK;
    double dHighThr = 61.00;
    double dLowThr = 11.00;
    rc = DSC_SetEdfaDcmOutLosThreshold(EDFA_PA_CFG_KEY, dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaMode_test(void **state)
{

    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaMode(EDFA_PA_CFG_KEY, 0);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaMode(EDFA_PA_CFG_KEY, 1);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaMode(EDFA_PA_CFG_KEY, 2);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaMode(EDFA_BA_CFG_KEY, 0);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaMode(EDFA_BA_CFG_KEY, 1);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaMode(EDFA_BA_CFG_KEY, 2);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetEdfaName_test(void **state)
{

    int rc = SR_ERR_OK;
    char *pcName = "ab";
    rc = DSC_SetEdfaName(1, pcName);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_SetEdfaOutputPower_test(void **state)
   {

        int rc = SR_ERR_OK;
        double dPower = -99.00;
        rc = DSC_SetEdfaOutputPower(EDFA_PA_CFG_KEY, dPower);
    assert_int_equal(rc, SR_ERR_OK);
   //    rc = DSC_SetEdfaOutputPower(EDFA_BA_CFG_KEY, dPower);
   //    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SetEdfaType_test(void **state)
{

    int rc = SR_ERR_OK;
    rc = DSC_SetEdfaType(EDFA_PA_CFG_KEY, 1);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaType(EDFA_PA_CFG_KEY, 2);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaType(EDFA_PA_CFG_KEY, 3);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaType(EDFA_BA_CFG_KEY, 1);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaType(EDFA_BA_CFG_KEY, 2);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEdfaType(EDFA_BA_CFG_KEY, 3);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_SetEdfaDcmTargetLos_test(void **state)
   {

        int rc = SR_ERR_OK;
        double dDcmTargetLos = 0.00;
        rc = DSC_SetEdfaDcmTargetLos(EDFA_PA_CFG_KEY, dDcmTargetLos);
    assert_int_equal(rc, SR_ERR_OK);
   }
 */

static void
DSC_SetEvoaAttn_test(void **state)
{

    int rc = SR_ERR_OK;
    double dAttn = 25.5;
    rc = DSC_SetEvoaAttn(EVOA1_CFG_KEY, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetEvoaAttn(EVOA1_CFG_KEY, dAttn);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetEvoaAttn_test(void **state)
{

    int rc = SR_ERR_OK;
    double dAttn = 25.5;
    rc = DSC_GetEvoaAttn(EVOA1_CFG_KEY, &dAttn);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetEvoaAttn(EVOA1_CFG_KEY, &dAttn);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetEvoaActualAttn_test(void **state)
{

    int rc = SR_ERR_OK;
    double dAttn = 25.5;
/*	double dAttn3 = 15.00; */
    rc = DSC_GetEvoaActualAttn(EVOA1_CFG_KEY, &dAttn);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetEvoaActualAttn(EVOA1_CFG_KEY, &dAttn);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetEdfaGainByName_test(void **state)
{

    int rc = SR_ERR_OK;
    char *name = "target-gain";
    double gain = 4.00;
    rc = DSC_GetEdfaGainByName(name, &gain);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_Edfa_Info_test),
        cmocka_unit_test(DSC_SetEdfaGain_test),
        cmocka_unit_test(DSC_SetEdfaGainRange_test),
        cmocka_unit_test(DSC_SetEdfaTilt_test),
        cmocka_unit_test(DSC_SetEdfaOutputEnable_test),
        cmocka_unit_test(DSC_SetEdfaAutolosEnable_test),
        cmocka_unit_test(DSC_SetEdfaAprEnable_test),
        cmocka_unit_test(DSC_SetEdfaAprPower_test),
        cmocka_unit_test(DSC_SetEdfaPlimEnable_test),
        cmocka_unit_test(DSC_SetEdfaPlimPower_test),
        cmocka_unit_test(DSC_ChangeEdfaName_test),         /*fahad*/
        cmocka_unit_test(DSC_EdfaGainRangeToEnum_test),
        cmocka_unit_test(DSC_GetEdfaConfig_test),
        cmocka_unit_test(DSC_GetEdfaState_test),
/*		cmocka_unit_test(DSC_SetEdfaBackRefRatio_test), not in use */
        cmocka_unit_test(DSC_SetEdfaInLosThreshold_test),
        cmocka_unit_test(DSC_SetEdfaOutLosThreshold_test),
        cmocka_unit_test(DSC_SetEdfaDcmInLosThreshold_test),
        cmocka_unit_test(DSC_SetEdfaDcmOutLosThreshold_test),
        cmocka_unit_test(DSC_SetEdfaMode_test),
        cmocka_unit_test(DSC_SetEdfaName_test),
/*		cmocka_unit_test(DSC_SetEdfaOutputPower_test), */
        cmocka_unit_test(DSC_SetEdfaType_test),
/*		cmocka_unit_test(DSC_SetEdfaDcmTargetLos_test) unable to find value for targetLos */
        cmocka_unit_test(DSC_SetEvoaAttn_test),
        cmocka_unit_test(DSC_GetEvoaAttn_test),
        cmocka_unit_test(DSC_GetEvoaActualAttn_test),
        cmocka_unit_test(DSC_GetEdfaGainByName_test)

    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
