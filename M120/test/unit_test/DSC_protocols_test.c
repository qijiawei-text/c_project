/**
 * Copyright (c) Oplink Communications, LLC
 * fahad <fahad.siddiqui@Molex.com>
 * July. 26th, 2018
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
DSC_GetRstpPortIfName_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcXpath = NULL;
    rc = DSC_GetRstpPortIfName(pcXpath);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetRstpBridgeState_test(void **state)
{
    int rc = SR_ERR_OK;
    RSTP_BRIDGE_STATE_ST stRstpBridgeState;
    rc = DSC_GetRstpBridgeState(&stRstpBridgeState);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetRstpBridgeConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    RSTP_BRIDGE_CONFIG_ST stRstpBridgeConfig;
    rc = DSC_GetRstpBridgeConfig(&stRstpBridgeConfig);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
static void
DSC_SetRstpCommon_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcXPath = NULL;
    uint32_t u32Value = 1;
    rc = DSC_SetRstpCommon(pcXPath, u32Value);
    assert_int_equal(rc, SR_ERR_OK);
}
*/
static void
DSC_SetRstpBridgePriority_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32BridgePriority = 32768;
    rc = DSC_SetRstpBridgePriority(u32BridgePriority);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpHoldTime_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32HoldTime = 2;
    rc = DSC_SetRstpHoldTime(u32HoldTime);
    assert_int_equal(rc, SR_ERR_OK);
    printf("HoldTime: %d\n", u32HoldTime);
}

static void
DSC_SetRstpHelloTime_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32HelloTime = 2;
    rc = DSC_SetRstpHelloTime(u32HelloTime);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpMaxAge_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32MaxAge = 20;
    rc = DSC_SetRstpMaxAge(u32MaxAge);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpForwardDelay_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32ForwardDelay = 15;
    rc = DSC_SetRstpForwardDelay(u32ForwardDelay);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpTransmitHoldCount_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32TransmitHoldCount = 2;
    rc = DSC_SetRstpTransmitHoldCount(u32TransmitHoldCount);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpBridgePortPriority_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32PortPriority = 1;
    rc = DSC_SetRstpBridgePortPriority(1, u32PortPriority);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetRstpBridgePortPriority(2, u32PortPriority);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetRstpBridgePortCost_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32PortCost = 20000;
    rc = DSC_SetRstpBridgePortCost(1, u32PortCost);
    assert_int_equal(rc, SR_ERR_OK);
    rc = DSC_SetRstpBridgePortCost(2, u32PortCost);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetLldpPortIfName_test(void **state)
{
    int rc = SR_ERR_OK;
    char pcXpath;
    rc = DSC_GetLldpPortIfName(&pcXpath);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_LldpAdminStatusToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char Buf;
    rc = DSC_LldpAdminStatusToEnum(&Buf);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetLldpState_test(void **state)
{
    int rc = SR_ERR_OK;
    LLDP_CONFIG_ST stLldpConfig;
    LLDP_NBR_LIST_INFO_ST stLldpNbrList;
    uint32_t u32NbrListNum;
    rc = DSC_GetLldpState(&stLldpConfig, &stLldpNbrList, &u32NbrListNum);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetLldpConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    LLDP_CONFIG_ST stLldpConfig;
    rc = DSC_GetLldpConfig(&stLldpConfig);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetLldpGlobalAdminStatus_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcAdminStatus = "disable";
    rc = DSC_SetLldpGlobalAdminStatus(pcAdminStatus);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetLldpGlobalMsgTxInterval_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32MsgTxInterval = 30;
    rc = DSC_SetLldpGlobalMsgTxInterval(u32MsgTxInterval);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetLldpGlobalMsgTxHoldMultiplier_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32MsgTxHoldMultiplier = 4;
    rc = DSC_SetLldpGlobalMsgTxHoldMultiplier(u32MsgTxHoldMultiplier);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetLldpPortAdminStatus_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcAdminStatus = "disable";
    rc = DSC_SetLldpPortAdminStatus(1, pcAdminStatus);
    assert_int_equal(rc, SR_ERR_OK);
}



int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
    	cmocka_unit_test(DSC_GetRstpPortIfName_test),
        cmocka_unit_test(DSC_GetRstpBridgeState_test),
		cmocka_unit_test(DSC_GetRstpBridgeConfig_test),
//		cmocka_unit_test(DSC_SetRstpCommon_test),
		cmocka_unit_test(DSC_SetRstpBridgePriority_test),
		cmocka_unit_test(DSC_SetRstpHoldTime_test),
		cmocka_unit_test(DSC_SetRstpHelloTime_test),
		cmocka_unit_test(DSC_SetRstpMaxAge_test),
		cmocka_unit_test(DSC_SetRstpForwardDelay_test),
		cmocka_unit_test(DSC_SetRstpTransmitHoldCount_test),
		cmocka_unit_test(DSC_SetRstpBridgePortPriority_test),
		cmocka_unit_test(DSC_SetRstpBridgePortCost_test),
		cmocka_unit_test(DSC_GetLldpPortIfName_test),
		cmocka_unit_test(DSC_LldpAdminStatusToEnum_test),
		cmocka_unit_test(DSC_GetLldpState_test),
		cmocka_unit_test(DSC_GetLldpConfig_test),
		cmocka_unit_test(DSC_SetLldpGlobalAdminStatus_test),
		cmocka_unit_test(DSC_SetLldpGlobalMsgTxInterval_test),
		cmocka_unit_test(DSC_SetLldpGlobalMsgTxHoldMultiplier_test),
		cmocka_unit_test(DSC_SetLldpPortAdminStatus_test)

};
   int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
   return ret;
}
