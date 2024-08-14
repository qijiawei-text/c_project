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

#define IP_V4_PATTERN "^([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]).([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]).([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]).([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])$"
#define MAC_PATTERN "^[a-fA-F0-9:]{17}|[a-fA-F0-9]{12}$"

static int
__check_ipv4(char *acIpAddrV4)
{
    regex_t regex;
    int retval;

    retval = regcomp(&regex, IP_V4_PATTERN, REG_EXTENDED | REG_NEWLINE);
    if (retval) {
        printf("Could not compile regex\n");
        return SR_ERR_OK;
    }
    retval = regexec(&regex, acIpAddrV4, 0, NULL, 0);
    if (!retval) {
        return SR_ERR_OK;
    } else {
        return SR_ERR_INTERNAL;
    }
}

static int
__check_mac(char *acMacAddr)
{
    regex_t regex;
    int retval;

    retval = regcomp(&regex, MAC_PATTERN, REG_EXTENDED | REG_NEWLINE);
    if (retval) {
        printf("Could not compile regex\n");
        return SR_ERR_OK;
    }
    retval = regexec(&regex, acMacAddr, 0, NULL, 0);
    if (!retval) {
        return SR_ERR_OK;
    } else {
        return SR_ERR_INTERNAL;
    }
}

static void
DSC_GetConfigInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    DEVICE_CONFIG_ST stDeviceConfig;
    memset(&stDeviceConfig, 0x00, sizeof(stDeviceConfig));

    rc = DSC_GetConfigInfo(&stDeviceConfig, sizeof(stDeviceConfig));
    assert_int_equal(rc, SR_ERR_OK);
    /* check Node-id, valid length [1, 15] */
/*    printf("stDeviceConfig.acNodeId:%s\n", stDeviceConfig.acNodeId); */
    assert_in_range(strlen(stDeviceConfig.acNodeId), 1, 15);
    /* check Node-number, valid value [1, 65535] */
/*    printf("stDeviceConfig.u32NodeNumber:%d\n", stDeviceConfig.u32NodeNumber); */
    assert_in_range(stDeviceConfig.u32NodeNumber, 1, 65535);
    /* check timezone, valid value [-12, 12] */
/*    printf("stDeviceConfig.i32Timezone:%d\n", stDeviceConfig.i32Timezone); */
    if (stDeviceConfig.i32Timezone < 0) {
        assert_in_range(-stDeviceConfig.i32Timezone, 0, 12);
    } else {
        assert_in_range(stDeviceConfig.i32Timezone, 0, 12);
    }
    /* check temperature-up-thr, valid value [55.0, 75.0] */
/*    printf("stDeviceConfig.dTemperatureHighThr:%f\n", stDeviceConfig.dTemperatureHighThr); */
    assert_in_range(stDeviceConfig.dTemperatureHighThr, 55.0, 75.0);
    /* check ttemperature-low-thr, valid value [-25.0, 0] */
/*    printf("stDeviceConfig.dTemperatureLowThr:%f\n", stDeviceConfig.dTemperatureLowThr); */
    assert_in_range(-stDeviceConfig.dTemperatureLowThr, 0, 25);
    /* check ipv4 address */
    rc = __check_ipv4(stDeviceConfig.astDeviceNet[0].acIpAddrV4);
    assert_int_equal(rc, SR_ERR_OK);
    /* check Mac address */
    printf("stDeviceConfig.astDeviceNet[0].acMacAddr:%s\n", stDeviceConfig.astDeviceNet[0].acMacAddr);
    rc = __check_mac(stDeviceConfig.astDeviceNet[0].acMacAddr);
    assert_int_equal(rc, SR_ERR_OK);

}

static void
DSC_GetNetConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    DEVICE_CONFIG_ST stDevCfg;
    memset(&stDevCfg, 0x00, sizeof(stDevCfg));
    rc = DSC_GetNetConfig(&stDevCfg, sizeof(stDevCfg));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetTemperatureThr_test(void **state)
{
    int rc = SR_ERR_OK;
    double dHighThr = 60;
    double dLowThr = -10;
    rc = DSC_SetTemperatureThr(dHighThr, dLowThr);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPhysicalInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    DEVICE_PHYSICAL_ST stDevicePhysical;
    memset(&stDevicePhysical, 0x00, sizeof(stDevicePhysical));

    rc = DSC_GetPhysicalInfo(&stDevicePhysical, sizeof(stDevicePhysical));
    assert_int_equal(rc, SR_ERR_OK);
    assert_string_not_equal(stDevicePhysical.acNodeType, "");
    assert_string_not_equal(stDevicePhysical.acVendor, "");
    assert_string_not_equal(stDevicePhysical.acModel, "");
    assert_string_not_equal(stDevicePhysical.acProductCode, "");
    assert_string_not_equal(stDevicePhysical.acSerialId, "");
    assert_string_not_equal(stDevicePhysical.acManufactorDate, "");
    assert_string_not_equal(stDevicePhysical.acClei, "");
/*    assert_string_not_equal(stDevicePhysical.acType, ""); */
    assert_string_not_equal(stDevicePhysical.acHwVer, "");
    assert_string_not_equal(stDevicePhysical.acSwVer, "");
}

static void
DSC_SetNodeId_test(void **state)
{
    int rc = SR_ERR_OK;
    char *cpNodeId = "1001";
    rc = DSC_SetNodeId(cpNodeId, sizeof("1001"));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetNodeNumber_test(void **state)
{
    int rc = SR_ERR_OK;
    uint32_t u32NodeNumber = 1;
    rc = DSC_SetNodeNumber(u32NodeNumber);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetTimezone_test(void **state)
{
    int rc = SR_ERR_OK;
    int32_t i32Timezone = 0;
    rc = DSC_SetTimezoneUtcOffset(i32Timezone);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetTimezone_test(void **state)
{
    int rc = SR_ERR_OK;
    int16_t sTimezone = 0;
    rc = DSC_GetTimezoneUtcOffset(&sTimezone);
    printf("sTimezone:%d\n", sTimezone);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_SetMacAddr_test(void **state)
   {
    int rc = SR_ERR_OK;
    DSC_ETH_INDEX_EN enEthIndex;
    char *MacAddress = "fe:fe:fe:00:01:02";
    rc = DSC_SetMacAddr(1 , MacAddress, sizeof(enEthIndex));
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SetNetworkIpv4_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcIpAddress = "192.168.1.2";
    char *pcNetmask = "255.255.0.0";
    char *pcGateway = "192.168.1.1";
    rc = DSC_SetNetworkIpv4(0, pcIpAddress, pcNetmask, pcGateway, sizeof(DSC_ETH_INDEX_EN));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetNetworkIpv6_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcIpAddress = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
    char *pcNetmask = NULL;
    char *pcGateway = "2001::1";
    rc = DSC_SetNetworkIpv6(0, pcIpAddress, pcNetmask, pcGateway, sizeof(DSC_ETH_INDEX_EN));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetServiceState_test(void **state)
{
    int rc = SR_ERR_OK;
    SERVICE_EN Service = 0;
    rc = DSC_GetServiceState(Service);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SetServiceEnable_test(void **state)
{
    int rc = SR_ERR_OK;
    SERVICE_EN Service = 1;
    bool bState = NULL;
    rc = DSC_SetServiceEnable(Service, bState);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_ClientToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcClient = "0";
    rc = DSC_ClientToEnum(pcClient);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_RoleToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcClient = "1";
    rc = DSC_RoleToEnum(pcClient);
    assert_int_equal(rc, SR_ERR_OK);
}

/* static void */
/* DSC_GetClientServerInfo_test(void **state) */
/* { */
/*    int rc = SR_ERR_OK; */
/*    CLIENT_EN Client = 4; */
/*    CLIENT_SERVER_ST stServerInfo; */
/*    memset(&stServerInfo, 0x00, sizeof(stServerInfo)); */
/*    rc = DSC_GetClientServerInfo(Client, stServerInfo, sizeof(stServerInfo)); */
/*    assert_int_equal(rc, SR_ERR_OK); */
/* } */


static void
DSC_AddClientServerIp_test(void **state)
{
    int rc = SR_ERR_OK;
    char *IpAddress = "127.0.0.1";
    rc = DSC_AddClientServerIp(CLIENT_NTP, IpAddress);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetOfConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    OF_CONFIG_ST stOfConfig;
    memset(&stOfConfig, 0x00, sizeof(stOfConfig));
    rc = DSC_GetOfConfig(&stOfConfig, sizeof(stOfConfig));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_LedToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcBuf = "2";
    rc = DSC_LedToEnum(pcBuf);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetLedStatus_test(void **state)
{
#if 0
    int rc = SR_ERR_OK;
    LED_INFO_ST *stLedInfo;
    size_t *count = NULL;
    rc = DSC_GetLedStatus(&stLedInfo, count);
    assert_int_equal(rc, SR_ERR_OK);
#endif
}

static void
DSC_GetTemperature_test(void **state)
{
    int rc = SR_ERR_OK;
    TEMP_INFO_ST stTempInfo;
    memset(&stTempInfo, 0x00, sizeof(stTempInfo));
    rc = DSC_GetTemperature(&stTempInfo, sizeof(stTempInfo));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetUptime_test(void **state)
{
    int rc = SR_ERR_OK;
    char Uptime[] = "1530";
    rc = DSC_GetUptime(Uptime, sizeof(Uptime));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetRunTime_test(void **state)
{
    int rc = SR_ERR_OK;
    char RunTime[] = "1530";
    rc = DSC_GetRunTime(RunTime, sizeof(RunTime));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetSystemTime_test(void **state)
{
    int rc = SR_ERR_OK;
    char Time[] = "1530";
    rc = DSC_GetSystemTime(Time, sizeof(Time));
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SysStateToEnum_test(void **state)
{
    int rc = SYS_STATE_IN_SERVICE;
    char *pcSysState = "1";
    rc = DSC_SysStateToEnum(pcSysState);
    assert_int_equal(rc, SYS_STATE_IN_SERVICE);
}

static void
DSC_GetSystemStatus_test(void **state)
{
    int rc = SR_ERR_OK;
    SYS_STATE_EN penSysState = 1;
    rc = DSC_GetSystemStatus(&penSysState);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetPhysicalInfoTimeout_test(void **state)
{
    int rc = SR_ERR_OK;
    DEVICE_PHYSICAL_ST DevPhysical;
    memset(&DevPhysical, 0x00, sizeof(DevPhysical));
    rc = DSC_GetPhysicalInfoTimeout(&DevPhysical, sizeof(DevPhysical), 20);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_GetPhysicalExtension_test(void **state)
   {
   //	int rc = SR_ERR_OK;
        EXT_DEVICE_PHYSICAL_ST stIdeeprom;
   //	memset(&stIdeeprom, 0x00, sizeof(stIdeeprom));
   //    rc = DSC_GetPhysicalExtension(&stIdeeprom);
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_GetModMfgInfo_test(void **state)
{
    int rc = SR_ERR_OK;
    DEVICE_PHYSICAL_ST *pstDevPhysical;
    memset(&pstDevPhysical, 0x00, sizeof(pstDevPhysical));
    size_t count = sizeof(pstDevPhysical);
    rc = DSC_GetModMfgInfo(&pstDevPhysical, &count);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_NodetypeStrToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcNodeType = "2";
    rc = DSC_NodetypeStrToEnum(pcNodeType);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_ModuleTypeStrToEnum_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcModuleType = "2";
    rc = DSC_ModuleTypeStrToEnum(pcModuleType);
    assert_int_equal(rc, SR_ERR_OK);
}

/* static void */
/* DSC_ModuleTypeEnumToString_test(void **state) */
/* { */
/*    int rc = SR_ERR_OK; */
/*    char *pcModuleType = "edfa"; */
/*    MOD_IDX_EN enModType = NULL; */
/*    rc = DSC_ModuleTypeEnumToString(enModType, pcModuleType, sizeof(MOD_IDX_EN)); */
/*    assert_int_equal(rc, SR_ERR_OK); */
/* } */

static void
DSC_Reset_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcResetType = "warm";
    rc = DSC_Reset(pcResetType);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_SwStage_test(void **state)
   {
        int rc = SR_ERR_OK;
        char *pcFilename = "http://org/openroadm/device";
    rc = DSC_SwStage(pcFilename);
    assert_int_equal(rc, SR_ERR_OK);
   }

   static void
   DSC_SwActivate_test(void **state)
   {
    int rc = SR_ERR_OK;
    char *pcVersion = "1.2.1";
    char *pcValidationTimer = NULL;
        char *pcStatus = NULL;
    rc = DSC_SwActivate(pcVersion, pcValidationTimer, pcStatus);
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SwCancel_test(void **state)
{
    int rc = SR_ERR_OK;
    bool bBoolean = true;
    rc = DSC_SwCancel(bBoolean);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetUpgState_test(void **state)
{
    int rc = SR_ERR_OK;
    ST_UPG_STATE stUpgState;
    memset(&stUpgState, 0x00, sizeof(stUpgState));
    size_t count = sizeof(stUpgState);
    rc = DSC_GetUpgState(&stUpgState, count);
    assert_int_equal(rc, SR_ERR_OK);
/*    assert_in_range(stUpgState.acState, 20, 30); don't know the range for acstate */
/*    assert_in_range(stUpgState.u32Percent, 0, 100); */
/*    assert_in_range(stUpgState.acResetType, 0, 1); don't know the range for acstate */

}

static void
DSC_RestoreDefault_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_RestoreDefault();
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_SaveConfig_test(void **state)
{
    int rc = SR_ERR_OK;
    const char *module = NULL;
    rc = DSC_SaveConfig(module);
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_NtpSync_test(void **state)
   {
    int rc = SR_ERR_OK;
    rc = DSC_NtpSync();
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SaveLog_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SaveLog();
    assert_int_equal(rc, SR_ERR_OK);
}
/*
   static void
   DSC_GetSystemFile_test(void **state)
   {
    int rc = SR_ERR_OK;
    char *pcFilePath = "/var/shared/example.txt";
    char *pcStat = "1";
    char *pcMessage = "example test";
    rc = DSC_GetSystemFile(pcFilePath, pcStat, pcMessage);
    assert_int_equal(rc, SR_ERR_OK);
   }

   static void
   DSC_DelSystemFile_test(void **state)
   {
    int rc = SR_ERR_OK;
    char *pcFilePath = "/home/fahad/shasta/apps/test/unit_test/sampleFile";
    rc = DSC_DelSystemFile(pcFilePath);
    assert_int_equal(rc, SR_ERR_OK);
   }
 */
static void
DSC_SetAdminState_test(void **state)
{
    int rc = SR_ERR_OK;
    char *pcState = "outOfService";
    rc = DSC_SetAdminState(pcState);
    assert_int_equal(rc, SR_ERR_OK);
}

/*static void
   DSC_GetAdminState_test(void **state)
   {
    int rc = SR_ERR_OK;
    char *pcState = "outOfService";
    rc = DSC_GetAdminState(pcState, sizeof(pcState));
    assert_int_equal(rc, SR_ERR_OK);
   }
 */

static void
DSC_SetSystemMode_test(void **state)
{
    int rc = SR_ERR_OK;
    rc = DSC_SetSystemMode(1);
    assert_int_equal(rc, SR_ERR_OK);
}

static void
DSC_GetSystemMode_test(void **state)
{
    int rc = SR_ERR_OK;
    SYSTEM_MODE_EN penSystemMode = 0;
    rc = DSC_GetSystemMode(&penSystemMode);
    printf("penSystemMode:%d\n", penSystemMode);
    assert_int_equal(rc, SR_ERR_OK);
}

int
main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(DSC_GetConfigInfo_test),
        cmocka_unit_test(DSC_GetNetConfig_test),
        cmocka_unit_test(DSC_SetNodeId_test),
        cmocka_unit_test(DSC_SetNodeNumber_test),
        cmocka_unit_test(DSC_SetTimezone_test),
        cmocka_unit_test(DSC_GetTimezone_test),
        cmocka_unit_test(DSC_SetTemperatureThr_test),
        cmocka_unit_test(DSC_GetPhysicalInfo_test),
/*		cmocka_unit_test(DSC_SetMacAddr_test), //fahad */
        cmocka_unit_test(DSC_SetNetworkIpv4_test),
        cmocka_unit_test(DSC_SetNetworkIpv6_test),
        cmocka_unit_test(DSC_GetServiceState_test),
        cmocka_unit_test(DSC_SetServiceEnable_test),
        cmocka_unit_test(DSC_ClientToEnum_test),
        cmocka_unit_test(DSC_RoleToEnum_test),
/*		cmocka_unit_test(DSC_GetClientServerInfo_test), */
        cmocka_unit_test(DSC_AddClientServerIp_test),
        cmocka_unit_test(DSC_GetOfConfig_test),
        cmocka_unit_test(DSC_LedToEnum_test),
        cmocka_unit_test(DSC_GetLedStatus_test),
        cmocka_unit_test(DSC_GetTemperature_test),
        cmocka_unit_test(DSC_GetUptime_test),
        cmocka_unit_test(DSC_GetRunTime_test),
        cmocka_unit_test(DSC_GetSystemTime_test),
        cmocka_unit_test(DSC_SysStateToEnum_test),
        cmocka_unit_test(DSC_GetSystemStatus_test),
        cmocka_unit_test(DSC_GetPhysicalInfoTimeout_test),
/*		cmocka_unit_test(DSC_GetPhysicalExtension_test), */
        cmocka_unit_test(DSC_GetModMfgInfo_test),
        cmocka_unit_test(DSC_NodetypeStrToEnum_test),
        cmocka_unit_test(DSC_ModuleTypeStrToEnum_test),
/*		cmocka_unit_test(DSC_ModuleTypeEnumToString_test), */
        cmocka_unit_test(DSC_Reset_test),
/*		cmocka_unit_test(DSC_SwStage_test), */
/*		cmocka_unit_test(DSC_SwActivate_test), */
        cmocka_unit_test(DSC_SwCancel_test),
        cmocka_unit_test(DSC_GetUpgState_test),
        cmocka_unit_test(DSC_RestoreDefault_test),
        cmocka_unit_test(DSC_SaveConfig_test),
/*		cmocka_unit_test(DSC_NtpSync_test), */
        cmocka_unit_test(DSC_SaveLog_test),
/*		cmocka_unit_test(DSC_GetSystemFile_test), */
/*		cmocka_unit_test(DSC_DelSystemFile_test), */
        cmocka_unit_test(DSC_SetAdminState_test),
/*		cmocka_unit_test(DSC_GetAdminState_test), */
        cmocka_unit_test(DSC_SetSystemMode_test),
        cmocka_unit_test(DSC_GetSystemMode_test)

    };
    int ret = cmocka_run_group_tests(tests, DSC_StartSession_setup, DSC_StartSession_teardown);
    return ret;
}
