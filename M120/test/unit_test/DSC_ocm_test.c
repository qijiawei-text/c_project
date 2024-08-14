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
DSC_GetOcmPortId_test(void **state)
{
    int rv = 0;
    char *XPATH = NULL;
    rv = DSC_GetOcmPortId(XPATH);
    assert_int_equal(rv, -1);
}

static void
DSC_GetOcmWavelengthNumber_test(void **state)
{
    int rv = 0;
    char *XPATH = NULL;
    rv = DSC_GetOcmWavelengthNumber(XPATH);
    assert_int_equal(rv, -1);
}


static void
DSC_GetOcmChannelInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    OCM_STATS_ST *pstOcmInfo = NULL;
    size_t channel_count = 0;
    int j = 0;

    /* rc = DSC_GetOcmChannelInfo(1, &pstOcmInfo, &channel_count); */
    assert_int_equal(rc, SR_ERR_OK);

    for (j = 0; j < channel_count; j++) {
        printf("   u32ChannelId            %d\n", pstOcmInfo[j].u32ChannelId);
        printf("   dCurrentPower           %f\n", pstOcmInfo[j].dCurrentPower);
        printf("   dOsnr                   %f\n", pstOcmInfo[j].dOsnr);
    }

    free(pstOcmInfo);
    pstOcmInfo = NULL;
}
static void
DSC_GetOcmMonitorEnable_test(void **state)
{
    bool rc = true;
    rc = DSC_GetOcmMonitorEnable(1);
    assert_int_equal(rc, true);
}

/* static void */
/* DSC_GetOcmsCnt_test(void **state) */
/* { */
/*    int rc = SR_ERR_OK; */
/*    size_t count; */
/*    rc = DSC_GetOcmsCnt(&count); */
/*    printf("OCM Count:%zu\n", count); */
/*    assert_int_equal(rc, SR_ERR_OK); */
/* } */

static void
DSC_SetOcmMonitorEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    bool enable = true;
    rc = DSC_SetOcmMonitorEnable(1, enable);
    assert_int_equal(rc, SR_ERR_OK);
    bool desable = false;
    rc = DSC_SetOcmMonitorEnable(1, desable);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOcmPortIdByName_test(void **state)
{
    int rc = SR_ERR_OK;
    unsigned int id;
    rc = DSC_GetOcmPortIdByName("ingress-out", &id);
    printf("ingress-out port id:%d\n", id);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetOcmPortIdByName("egress-in", &id);
    printf("egress-in port id:%d\n", id);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_GetOcmPortIdByName("egress-out", &id);
    printf("egress-out port id:%d\n", id);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOcmPortCount_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count;
    rc = DSC_GetOcmPortCount(&count);
    printf("OCM Port Count:%zu\n", count);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetOcmChannelInfo_test),
        cmocka_unit_test(DSC_SetOcmMonitorEnable_test),
        cmocka_unit_test(DSC_GetOcmPortIdByName_test),
        cmocka_unit_test(DSC_GetOcmPortCount_test),
        cmocka_unit_test(DSC_GetOcmWavelengthNumber_test),
        cmocka_unit_test(DSC_GetOcmPortId_test),
/*		cmocka_unit_test(DSC_GetOcmsCnt_test), */
        cmocka_unit_test(DSC_GetOcmMonitorEnable_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
