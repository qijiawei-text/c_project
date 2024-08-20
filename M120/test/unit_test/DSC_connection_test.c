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
DSC_GetConnectionsInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    CONNECTION_INFO_ST *ppstConn = NULL;
    size_t count = 0;
    int i = 0;

    rc = DSC_GetConnectionsInfo(&ppstConn, &count);
    assert_int_equal(rc, SR_ERR_OK);
    for (i = 0; i < count; i++) {
        assert_int_not_equal(ppstConn[i].conn_id, 0);
        assert_int_not_equal(ppstConn[i].source.port_id, 0);
        assert_int_not_equal(ppstConn[i].source.wavelength_num, 0);
/*        assert_int_not_equal(ppstConn[i].dAttenuation, 0); */
    }
}

//static void
//DSC_AddConnection_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    //size_t count = 0;
//    rc = DSC_AddConnection(3, 4, 1, 10);
//    assert_int_equal(rc, SR_ERR_OK);
//    //rc = DSC_GetConnectionsCnt(&count);
//    //assert_int_equal(count, 1);
//}

static void
DSC_GetConnectionId_test(void **state)
{
    int rc = -1;
    char xpath;
    rc = DSC_GetConnectionId(&xpath);
    assert_int_equal(rc, -1);
    //rc = DSC_GetConnectionsCnt(&count);
    //assert_int_equal(count, 1);
}


//static void
//DSC_DeleteConnection_test(void **state)
//{
//    int rc = SR_ERR_OK;
//    //size_t count = 0;
//    rc = DSC_DeleteConnection(1, 2, 10);
//    assert_int_equal(rc, SR_ERR_OK);
//    //rc = DSC_GetConnectionsCnt(&count);
//    //assert_int_equal(count, 0);
//}

static void
DSC_GetConnectionsCnt_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count = 0;
    rc = DSC_GetConnectionsCnt(&count);
    assert_int_equal(rc, 0);
    printf("count:%zu\n", count);
}

static void
DSC_SetConnections_test(void **state)
{
    int rc = SR_ERR_OK;
    double *pdCurAttn = NULL;
    size_t iOcmChannelNum = 0;
    uint32_t *u32SrcPortId = NULL;
    uint32_t *u32DestPortId = NULL;

    rc = DSC_GetDefaultWaveplansCnt(&iOcmChannelNum);
    printf("iOcmChannelNum:%zu\n", iOcmChannelNum);
    assert_int_equal(rc, 0);
    pdCurAttn = (double *)malloc(sizeof(double) * iOcmChannelNum);
    u32SrcPortId = (uint32_t *)malloc(sizeof(unsigned int) * iOcmChannelNum);
    u32DestPortId = (uint32_t *)malloc(sizeof(unsigned int) * iOcmChannelNum);
    for (int i = 0; i < iOcmChannelNum; i++) {
        u32SrcPortId[i] = 3;
        u32DestPortId[i] = 2;
    }
    memset(pdCurAttn, 0x00, sizeof(double) * iOcmChannelNum);
    rc = DSC_SetConnections(u32SrcPortId, u32DestPortId, pdCurAttn);
    assert_int_equal(rc, 0);

    free(pdCurAttn);
    free(u32SrcPortId);
    free(u32DestPortId);
    pdCurAttn = NULL;
    u32SrcPortId = NULL;
    u32DestPortId = NULL;

}

static void
DSC_getChannelCount_test(void **state)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess_get = NULL;
    rc = sr_session_start(DSC_GetConn(), SR_DS_STARTUP, SR_SESS_DEFAULT, &sess_get);
    assert_int_equal(rc, SR_ERR_OK);
    size_t size = 0;
    sr_val_t *values = NULL;
    char XPATH[] = "/com-oplink-waveplan:open-optical-device/default-waveplan/wavelengths/wavelength-number";
    rc = sr_get_items(sess_get, XPATH, &values, &size);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetWssConfig_test(void **state)
{
	int rc = SR_ERR_OK;
	WSS_CFG_ST *pstWssCfg;
	size_t count;
	rc = DSC_GetWssConfig(6, 1, &pstWssCfg, &count);
	printf("channel: %d \n port: %d \n", pstWssCfg->u32Channel, pstWssCfg->u32Port);
	assert_int_equal(rc, SR_ERR_OK);
}

//static void
//DSC_SetWssConfig_test(void **state)
//{
//	int rc = SR_ERR_OK;
//	rc = DSC_SetWssConfig(6, 1, 1, 0, 10.0);
//	assert_int_equal(rc, SR_ERR_OK);
//}

static void
DSC_SetWssAllConfig_test(void **state)
{
	int rc = SR_ERR_OK;
	rc = DSC_SetWssAllConfig(6, 1, 0, 10.0);
	assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetConnectionsInfo_test),
//        cmocka_unit_test(DSC_AddConnection_test),
        cmocka_unit_test(DSC_getChannelCount_test),
//		cmocka_unit_test(DSC_DeleteConnection_test),
        cmocka_unit_test(DSC_GetConnectionsCnt_test),
        cmocka_unit_test(DSC_SetConnections_test),
//		cmocka_unit_test(DSC_SetWssConfig_test),
		cmocka_unit_test(DSC_GetWssConfig_test),
		cmocka_unit_test(DSC_GetConnectionId_test),
		cmocka_unit_test(DSC_SetWssAllConfig_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
