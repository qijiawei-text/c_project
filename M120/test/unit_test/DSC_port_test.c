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
DSC_PortDirectionToEnum_test(void **state)
{
    int rc;
    char Str;
    rc = DSC_PortDirectionToEnum(&Str);
//    printf("rc:%d\n", rc);
    if (rc == 1){
    return assert_int_equal(rc, DIRECTION_TX);
    }
    return assert_int_equal(rc, DIRECTION_RX);
}

static void
DSC_PortExist_test(void **state)
{
    int rc;
    uint32_t u32PortId = 1;
    rc = DSC_PortExist(u32PortId);
    assert_true(rc);
}

static void
DSC_GetPortNumber_test(void **state)
{
    int rc = SR_ERR_OK;
    char *xpath = NULL;
    rc = DSC_GetPortNumber(xpath);
    assert_int_not_equal(rc, SR_ERR_OK);
}


static void
DSC_GetChannelNumber_test(void **state)
{
    int rc = SR_ERR_OK;
    char xpath;
    rc = DSC_GetChannelNumber(&xpath);
    assert_int_not_equal(rc, SR_ERR_OK);
}

static void
DSC_PortDirection_test(void **state)
{
	int rc = SR_ERR_OK;
	DIRECTION_EN enDirection = 1;
    rc = DSC_PortDirection(1,&enDirection);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_PortEdfaDirection_test(void **state)
{
	int rc = SR_ERR_OK;
	DIRECTION_EN enDirection = 1;
    rc = DSC_PortEdfaDirection(1, &enDirection);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_PortQualType_test(void **state)
{
	int rc = SR_ERR_OK;
	PORT_QUAL_TYPE_EN enType = 2;
    rc = DSC_PortQualType(2, &enType);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortsCnt_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count;
    rc = DSC_GetPortsCnt(&count);
    printf("Port count:%zd\n", count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortsInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    PORT_INFO_ST *pstPortInfo = NULL;
    size_t count = 0;
    int i = 0;

    rc = DSC_GetPortsInfo(&pstPortInfo, &count);
    assert_int_equal(rc, SR_ERR_OK);
    /* assert_int_equal(count, 84); */
    for (i = 0; i < count; i++) {
        /* check port-id, valid value larger than 0 */
        assert_int_not_equal(pstPortInfo[i].u32PortId, 0);
        /* check port-name, valid value not null */
        assert_string_not_equal(pstPortInfo[i].acPortName, "");
        /* check port-direction, valid value [1,2,3] */
        assert_in_range(pstPortInfo[i].enPortDirection, 1, 3);
        /* check port-enPortWavelengthType, valid value [1,2,3] */
/*        assert_in_range(pstPortInfo[i].enPortWavelengthType, 1, 3); */
        /* check port-enPortWavelengthType, valid value [1,2,3,4,5,6] */
        assert_in_range(pstPortInfo[i].enPortQualType, 1, 6);
/*        assert_memory_equal(pstPortInfo[i].dTargetPortPower, 0, sizeof(double)); */
/*        assert_memory_equal(pstPortInfo[i].dCurrentPortPower, 0, sizeof(double)); */
/*        assert_int_not_equal(pstPortInfo[i].enEdfaDirection, 0); */
/*        assert_int_not_equal(pstPortInfo[i].enOscDirection, 0); */
/*        assert_memory_not_equal(pstPortInfo[i].stPortProperty.dCapabilityMaxRxPower, 0, sizeof(double)); */
/*        assert_memory_not_equal(pstPortInfo[i].stPortProperty.dCapabilityMinRxPower, 0, sizeof(double)); */
/*        assert_memory_not_equal(pstPortInfo[i].stPortProperty.dCapabilityMaxTxPower, 0, sizeof(double)); */
/*        assert_memory_not_equal(pstPortInfo[i].stPortProperty.dCapabilityMinTxPower, 0, sizeof(double)); */
    }

    if (NULL != pstPortInfo) {
        pstPortInfo = NULL;
    }
}
static void
DSC_GetPortInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    PORT_INFO_ST stPortInfo;
    memset(&stPortInfo, 0x00, sizeof(stPortInfo));
    rc = DSC_GetPortInfo(1, &stPortInfo, sizeof(stPortInfo));
    assert_int_equal(rc, SR_ERR_OK);
    assert_in_range(strlen(stPortInfo.acPortName), 1, 7);
//    printf("stPortInfo.acPortName:%s\n", stPortInfo.acPortName);
//    assert_in_range(stPortInfo.dHighThr, 0.0, 75.0);
}

static void
DSC_GetPortCurrentPower_test(void **state)
{
    int rc = SR_ERR_OK;
    double dCurrentPower;
    rc = DSC_GetPortCurrentPower(1, &dCurrentPower);
    printf("Current Power:%lf\n", dCurrentPower);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetPortTargetPower_test(void **state)
{
    int rc = SR_ERR_OK;
    double dTargetPortPower = 20.82;
    rc = DSC_SetPortTargetPower(1, dTargetPortPower);
    assert_int_not_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortUsedChannelsCnt_test(void **state)
{
    int rc = SR_ERR_OK;
    size_t count = 0;

    rc = DSC_GetPortUsedChannelsCnt(1, &count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortUsedChannelsInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    USED_WAVELENGTHS_ST *pstUsedWavelengths;
    size_t channel_count;
    rc = DSC_GetPortUsedChannelsInfo(1, &pstUsedWavelengths, &channel_count);
    printf("channel_count:%zd\n", channel_count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortUsedChannelsInfo_Iter_test(void **state)
{
    int rc = SR_ERR_OK;
    USED_WAVELENGTHS_ST *pstChannelInfo;
    size_t count;
    rc = DSC_GetPortUsedChannelsInfo_Iter(1, &pstChannelInfo, &count);
//    printf("channel_count:%zd\n", channel_count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetChannelTargetPower_test(void **state)
{
    int rc = SR_ERR_OK;
    double dTargetPortPower = 20.82;
    rc = DSC_SetChannelTargetPower(1, 1, dTargetPortPower);
    assert_int_not_equal(rc, SR_ERR_OK);
}

static void
DSC_SetPortThreshold_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetPortThreshold(1, 10, 1);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetPortThreshold(2, 60, -10);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPortIdByName_test(void **state)
{
	int rc = SR_ERR_OK;
	char *port_name = "LINE IN";
	unsigned int id;
    rc = DSC_GetPortIdByName(port_name, &id);
    printf("Port name: LINE IN, Port id:%d\n", id);
    assert_int_equal(rc, SR_ERR_OK);
    char *port_name2 = "AUX1 IN";
    rc = DSC_GetPortIdByName(port_name2, &id);
    printf("Port name: AUX1 IN, Port id:%d\n", id);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_PortDirectionToEnum_test),
		cmocka_unit_test(DSC_PortExist_test),
		cmocka_unit_test(DSC_GetPortNumber_test),
		cmocka_unit_test(DSC_GetChannelNumber_test),
		cmocka_unit_test(DSC_PortDirection_test),
		cmocka_unit_test(DSC_PortEdfaDirection_test),
		cmocka_unit_test(DSC_PortQualType_test),
		cmocka_unit_test(DSC_GetPortsCnt_test),
        cmocka_unit_test(DSC_GetPortsInfo_test),
        cmocka_unit_test(DSC_GetPortInfo_test),
        cmocka_unit_test(DSC_GetPortCurrentPower_test),
        cmocka_unit_test(DSC_SetPortTargetPower_test),
        cmocka_unit_test(DSC_GetPortUsedChannelsCnt_test),
		cmocka_unit_test(DSC_GetPortUsedChannelsInfo_Iter_test),
        cmocka_unit_test(DSC_GetPortUsedChannelsInfo_test),
		cmocka_unit_test(DSC_SetChannelTargetPower_test),
		cmocka_unit_test(DSC_SetPortThreshold_test),
		cmocka_unit_test(DSC_GetPortIdByName_test)
    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
