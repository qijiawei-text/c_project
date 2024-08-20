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
//DSC_GetWaveplansCnt_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    size_t count;
//    rc = DSC_GetWaveplansCnt(&count);
//    printf("count:%zu\n", count);
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_GetDefaultWaveplansCnt_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count;
    rc = DSC_GetDefaultWaveplansCnt(&count);
    printf("count:%zu\n", count);
    assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_GetWaveplansInfo_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    WAVEPLAN_ST *pstWaveplan;
//    size_t count;
//    memset(&pstWaveplan, 0x00, sizeof(pstWaveplan));
//    rc = DSC_GetWaveplansInfo(&pstWaveplan, &count);
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_GetDefaultWaveplansInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    WAVEPLAN_ST *pstWaveplan;
    size_t count;
    memset(&pstWaveplan, 0x00, sizeof(pstWaveplan));
    rc = DSC_GetDefaultWaveplansInfo(&pstWaveplan, &count);
    printf("frequency: %lf", pstWaveplan->dFrequency);
    assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_SetWaveplansInfo_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    WAVEPLAN_ST channel[MAX_CHL];
//
//    channel[0].u32ChannelId = 1;
//    channel[0].dFrequency = 192050.00;
//    channel[0].u32Bandwidth = 16;/* 16x3.125 = 50G */
//    channel[0].dWavelength = 1565.50;
//
//    rc = DSC_SetWaveplansInfo(channel, sizeof(WAVEPLAN_ST));
//    assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_GetInnerDefaultWaveplansInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t tCount = 0;
    WAVEPLAN_ST *pstWaveplan;
//    printf("frequency: %lf\n", pstWaveplan->dFrequency);
    rc = DSC_GetInnerDefaultWaveplansInfo(&pstWaveplan, &tCount);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
//        cmocka_unit_test(DSC_GetWaveplansCnt_test),
        cmocka_unit_test(DSC_GetDefaultWaveplansCnt_test),
//        cmocka_unit_test(DSC_GetWaveplansInfo_test),
        cmocka_unit_test(DSC_GetDefaultWaveplansInfo_test),
//        cmocka_unit_test(DSC_SetWaveplansInfo_test),
        cmocka_unit_test(DSC_GetInnerDefaultWaveplansInfo_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
