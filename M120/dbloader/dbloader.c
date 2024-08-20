/**********************************************************************************
   Copyright(C), 2012, OPLINK Tech.Co.,Ltd
   FileName    : dbloader.c
   Description : Loader configuration from disk/flash.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    fandy             2019/05/06   V1.0.0       Initial file.
   [2]    Chun Ye           2022/10.10   V1.0.1       Optimize the config restore flow.
**********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "util_inc.h"
#include "DevBoardCApi.h"
#include "oplkpersist.h"
#include "oplk_file.h"
#include "dbloader.h"
#include <libyang/libyang.h>
#include "sysrepo.h"

/**************************************************************************************************/
/* definition the default model type                                                              */
/**************************************************************************************************/
/* @ {                                                                                            */
#ifdef _NR1004_
static char m_acModel[MFG_INFO_LEN_MAX] = {"NR1004"};
/* #else */
/* static char m_acModel[MFG_INFO_LEN_MAX] = {"OLSP"}; */
#endif

#ifdef __OLSP__
static char m_acModel[MFG_INFO_LEN_MAX] = {"OLSP"};
#endif

#ifdef __HCF__
static char m_acModel[MFG_INFO_LEN_MAX] = {"HCF"};
#endif

#ifdef _OCS_
static char m_acModel[MFG_INFO_LEN_MAX] = {"OCS"};
#endif

#ifdef __OTDR__
static char m_acModel[MFG_INFO_LEN_MAX] = {"OTDR"};
#endif

/* } @                                                                                            */

/* #define __debug_skip_read_config (1)                                                           */

/* For porduct NR1004 and NR1004-OPS-4 has same startup files, so ONLY use the NR1004 file to save*/
static char *
__ConvertTail(const char *pcInputModel, const char *pcModuleName, char *pcOutputTail)
{
    if (NULL == pcInputModel || NULL == pcModuleName || NULL == pcOutputTail) {
        return m_acModel;
    }

    strcpy(pcOutputTail, pcInputModel);

    if (0 == strcmp(pcModuleName, "ietf-snmp") ||
        0 == strcmp(pcModuleName, "openconfig-system") ||
        0 == strcmp(pcModuleName, "openconfig-acl") ||
        0 == strcmp(pcModuleName, "openconfig-lldp") ||
        0 == strcmp(pcModuleName, "org-openroadm-rstp") ||
        0 == strcmp(pcModuleName, "org-openroadm-syslog") ||
        0 == strcmp(pcModuleName, "ietf-system") ||
        0 == strcmp(pcModuleName, "com-oplink-service") ||
        0 == strcmp(pcModuleName, "com-oplink-auth") ||
        0 == strcmp(pcModuleName, "openconfig-telemetry")) {
        if (0 == strcmp(pcInputModel, "NR1004-OPS-4")) {
            strcpy(pcOutputTail, "NR1004");
        }
    }

    if (0 == strcmp(pcModuleName, "com-oplink-control-loop") ||
        0 == strcmp(pcModuleName, "com-oplink-control-loop-i")) {
        if (0 == strcmp(pcInputModel, "HCF")) {
            strcpy(pcOutputTail, "1-HCF");
        }
    }

    if (0 == strcmp(pcInputModel, "OCS-32X32")) {
        if ((0 == strcmp(pcModuleName, "org-openroadm-tca")) ||
            (0 == strcmp(pcModuleName, "com-oplink-service")) ||
            (0 == strcmp(pcModuleName, "org-openroadm-device"))) {
            strcpy(pcOutputTail, "OCS");
        }

    }

    return pcOutputTail;
}

/***FUNC+******************************************************************************************/
/* Name   : __ReplaceValue                                                                        */
/* Descrp : Replace the value in com-oplink-device-startup-[Model].xml                            */
/* Input  : pcNode -- A pointer of node name                                                      */
/*        : pcOldValue -- A pointer of old value                                                  */
/*        : pcNewValue -- A pointer of new value                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__ReplaceValue(const char *pcNode, char *pcOldValue, char *pcNewValue)
{
    int rc = OPLK_OK;
    char acCmd[DEF_BUF_SIZE_256] = {0};

    /* check paramter.                                                          */
    if (NULL == pcNode || NULL == pcOldValue || NULL == pcNewValue) {
        LOG_WRITE(EN_LOG_ERROR, "input null, 0x%p, 0x%p, 0x%p",
                  pcNode, pcOldValue, pcNewValue);
        rc = OPLK_ERR;
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Node=%s, OldValue=%s, NewValue=%s.", pcNode, pcOldValue, pcNewValue);

    /* If new value is "", just return                                          */
    if (0 == strlen(pcNewValue)) {
        goto cleanup;
    }

    /* replace by sed cmd                                                       */
    memset(acCmd, 0x00, sizeof(acCmd));

    if (0 == strcmp(m_acModel, "OCS-32X32")) {
        /* For OCS and OCS-32X32, use the same xml */
        snprintf(acCmd, sizeof(acCmd), "sed -i 's/%s/%s/' %s/com-oplink-device-startup-%s.xml",
                 pcOldValue, pcNewValue, DEF_YANG_INSTALL_PATH, "OCS");
    } else {
        snprintf(acCmd, sizeof(acCmd), "sed -i 's/%s/%s/' %s/com-oplink-device-startup-%s.xml",
                 pcOldValue, pcNewValue, DEF_YANG_INSTALL_PATH, m_acModel);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s", acCmd);
    rc = UTIL_System(acCmd);

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __RestoreMfg                                                                          */
/* Descrp : Rostore mfg from CHASSIS/RPC EEPROM                                                   */
/*          and update  com-oplink-device-startup-[Model].xml                                     */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__RestoreMfg()
{
    int rc = OPLK_OK;
    CMfg stMfgInfo;
    CMfg stSccMfgInfo;

    /* Get mfginfo from CHASSIS/RPC EEPROM                                      */
    memset(&stMfgInfo, 0, sizeof(stMfgInfo));
    rc = BOARD_GetChassisMfg(&stMfgInfo);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetChassisMfg");
        goto cleanup;
    }

    /* set product name                                                         */
    LOG_WRITE(EN_LOG_NOTICE, "Chassis EEPROM, model: %s, config-type: %s",
              stMfgInfo.acProductName, stMfgInfo.acConfigType);

#ifdef __HCF__
    memset(m_acModel, 0x00, sizeof(m_acModel));
    strncpy(m_acModel, "HCF", sizeof(m_acModel) - 1);
#else
#ifdef _OCS_
    memset(m_acModel, 0x00, sizeof(m_acModel));
    if (0 == strcmp(stMfgInfo.acConfigType, "OCS-32X32")) {
        strncpy(m_acModel, "OCS-32X32", sizeof(m_acModel) - 1);
    } else {
        strncpy(m_acModel, "OCS", sizeof(m_acModel) - 1);
    }
#else
    if (0 == strcmp(stMfgInfo.acProductName, "OLS-P")) {
        memset(m_acModel, 0x00, sizeof(m_acModel));
        strncpy(m_acModel, "OLSP", sizeof(m_acModel) - 1);
    } else {
        /* Using config type to seperate AMP or OPS-4                           */
        if (0 == strcmp(stMfgInfo.acConfigType, "OPS")) {
            memset(m_acModel, 0x00, sizeof(m_acModel));
            strncpy(m_acModel, "NR1004-OPS-4", sizeof(m_acModel) - 1);
        } else {
            /* Using default value 'NR1004'                                     */
        }
    }
#endif
#endif

    /* write to com-oplink-device.xml                                           */
    /* vendor:                                                                  */
    __ReplaceValue("vendor", "Molex", stMfgInfo.acManufacturer);
    /* model:                                                                   */
    __ReplaceValue("model", "DEF_MODEL", stMfgInfo.acProductName);
    /* sn:                                                                      */
    __ReplaceValue("serial-id", "DEF_SN_01", stMfgInfo.acSN);
    /* pn:                                                                      */
    __ReplaceValue("product-code", "DEF_PN_01", stMfgInfo.acPN);
    /* hw-ver:                                                                  */
    __ReplaceValue("hardware-version", "DEF_HW_01", stMfgInfo.acHwVer);
    /* clei:                                                                    */
    __ReplaceValue("clei", "DEF_CLEI_01", stMfgInfo.acCleiCode);
    /* mfg-date:                                                                */
    __ReplaceValue("manufacture-date", "1999-12-31T00:00:00Z", stMfgInfo.acMfgDate);

    /* extended mfg info for NR1004:                                            */
    __ReplaceValue("unit-name", "DEF_UNIT_NAME", stMfgInfo.acUnitName);
    __ReplaceValue("fc-number", "DEF_FC_NO", stMfgInfo.acFCPN);
    __ReplaceValue("piu-issue-number", "DEF_PIU_ISSUE_NO", stMfgInfo.acFCIss);
    __ReplaceValue("fujitsu-part-number", "DEF_FUJITSU_PN", stMfgInfo.acFujitsuPN);

    /* mac:                                                                     */

    memset(&stSccMfgInfo, 0, sizeof(stSccMfgInfo));
    rc = BOARD_GetSccMfg(&stSccMfgInfo);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetSccMfg");
        goto cleanup;
    }
    memset(&stMfgInfo, 0, sizeof(stMfgInfo));
    memcpy(&stMfgInfo, &stSccMfgInfo, sizeof(CMfg));

    __ReplaceValue("macAddress", "fe:fe:fe:00:01:00", stMfgInfo.acEth0Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:01", stMfgInfo.acEth1Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:02", stMfgInfo.acEth2Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:03", stMfgInfo.acEth3Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:04", stMfgInfo.acEth4Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:05", stMfgInfo.acEth5Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:06", stMfgInfo.acEth6Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:07", stMfgInfo.acEth7Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:08", stMfgInfo.acEth8Mac);
    __ReplaceValue("macAddress", "fe:fe:fe:00:01:09", stMfgInfo.acEth9Mac);

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __InstallYang                                                                         */
/* Descrp : Call /etc/sysrepo/yang-model/oplink/device/install.sh                                 */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__InstallYang()
{
    int rc = OPLK_OK;
    char acCmdBuf[128] = {0};

    memset(acCmdBuf, 0, sizeof(acCmdBuf));
    snprintf(acCmdBuf, sizeof(acCmdBuf), "cd %s; ./install.sh %s",
             DEF_YANG_INSTALL_PATH, m_acModel);
    LOG_WRITE(EN_LOG_NOTICE, "acCmdBuf = %s", acCmdBuf);
    rc = UTIL_System(acCmdBuf);
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __InstallYang                                                                         */
/* Descrp : save the path data to EEPROM                                                          */
/* Input  : iId -- DEF_SAVE_PRIMARY/DEF_SAVE_SECONDARY                                            */
/*        : pcXpath -- A pointer of path                                                          */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__SaveCfg(int iId, const char *pcXpath)
{
    int rc = OPLK_OK;
    char acBuffer[DEF_BUF_SIZE_256] = {0};
    ST_PERSIST *pstCfgStatic = NULL;
    size_t nCnt = 0;
    int iAddrSlice = 0;
    char acFilePath[DEF_BUF_SIZE_256] = {0};

    /* check parameter                                                          */
    if (NULL == pcXpath) {
        rc = OPLK_ERR;
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "++ __SaveCfg %d", iId);

    /* Get persist config data count                                            */
    pstCfgStatic = PERSIST_GetCfgStaticData(&nCnt);
    if (NULL == pstCfgStatic) {
        rc = OPLK_ERR;
        goto cleanup;
    }

    /* Loop each config data, then set to EEPROM                                */
    for (int i = 0; i < nCnt; i++) {
        /* export bin to xml                                                    */
        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "/usr/local/bin/sysrepocfg --export=%s/%s.xml --datastore startup --module %s",
                 DEF_CFG_RUN_PATH,
                 pstCfgStatic[i].pcYangName,
                 pstCfgStatic[i].pcYangName);
        LOG_WRITE(EN_LOG_NOTICE, "Command:%s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "system %s", acBuffer);
            goto cleanup;
        }

        /* Using bz2 to tar                                                     */
        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "tar jcvf /tmp/%s.tar.bz2 %s/%s.xml", pstCfgStatic[i].pcYangName, pcXpath, pstCfgStatic[i].pcYangName);
        LOG_WRITE(EN_LOG_NOTICE, "%s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "system %s", acBuffer);
            goto cleanup;
        }

        DEF_SLEEP_MS(10);

        /* What's meaning? tbd                                                  */
        if (0 != iId) {
            iAddrSlice = pstCfgStatic[i].iAddrSlice + 1;
        } else {
            iAddrSlice = pstCfgStatic[i].iAddrSlice;
        }

        snprintf(acFilePath, sizeof(acFilePath), "/tmp/%s.tar.bz2", pstCfgStatic[i].pcYangName);
        LOG_WRITE(EN_LOG_NOTICE, "%s", acFilePath);
        rc = BOARD_SetChassisCfg(iAddrSlice, pstCfgStatic[i].iSize, acFilePath);
        if (0 != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_SetChassisCfg");
            goto cleanup;
        }
    }

    LOG_WRITE(EN_LOG_NOTICE, "-- __SaveCfg %d", iId);

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __LoadCfg                                                                             */
/* Descrp : Load config                                                                           */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__LoadCfg()
{
    return __InstallYang();
}

/***FUNC+******************************************************************************************/
/* Name   : __RestoreCfgById                                                                      */
/* Descrp :  Restore cfg by id                                                                    */
/* Input  : bFlag -- flag of restore data from EEPROM                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__RestoreCfgById(bool bFlag)
{
    int rc = OPLK_OK;
    char acBuffer[512];
    char acFileName[64];
    ST_PERSIST *pstCfgStatic = NULL;
    size_t nCnt = 0;
    char acTail[MFG_INFO_LEN_MAX] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "++ __RestoreCfgById: %d", bFlag);

#ifdef __debug_skip_read_config

    /* Load config by install.sh yang                                           */
    rc = __LoadCfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__LoadCfg failed");
        goto cleanup;
    }

    return rc;
#endif

    /* Check static data.                                                       */
    rc = PERSIST_CheckStaticData();
    if (rc != OPLK_OK) {
        rc = OPLK_ERR;
        goto cleanup;
    }

    /* Get persist config data count                                            */
    pstCfgStatic = PERSIST_GetCfgStaticData(&nCnt);
    if (NULL == pstCfgStatic) {
        rc = OPLK_ERR;
        goto cleanup;
    }

    /* If restore flag is false, tar data in mSata.                             */
    if (false == bFlag) {
        LOG_WRITE(EN_LOG_NOTICE, "dbloader tar db begin at switch mode!");
        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "tar jxvf %s*.db -C %s", DB_RESTORE_DIR, DB_RESTORE_DIR);
        LOG_WRITE(EN_LOG_NOTICE, "#### %s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
            goto cleanup;
        }
        LOG_WRITE(EN_LOG_NOTICE, "dbloader tar db end at switch mode!");
    }

    /* Loop each data in EEPROM and copy them to yang-mode path                 */
    for (int i = 0; i < nCnt; i++) {

        /* skip reserve slice                                                   */
        if (NULL == pstCfgStatic[i].pcYangName) {
            continue;
        }

        memset(acFileName, 0, sizeof(acFileName));
        snprintf(acFileName, sizeof(acFileName), "/tmp/%s.tar.bz2", pstCfgStatic[i].pcYangName);

        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "rm -fr %s", acFileName);
        LOG_WRITE(EN_LOG_TRACE, "#### %s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
            continue;
        }

        /* If restore flag is true, read data from EEPROM                       */
        if (true == bFlag) {
            rc = BOARD_GetChassisCfg(pstCfgStatic[i].iAddrSlice, acFileName);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetChassisCfg: %d, %s",
                          pstCfgStatic[i].iAddrSlice, acFileName);
                continue;
            }
        } else {
            memset(acBuffer, 0, sizeof(acBuffer));
            snprintf(acBuffer, sizeof(acBuffer), "mv %s/%s.tar.bz2 %s",
                     DB_RESTORE_DIR, pstCfgStatic[i].pcYangName, acFileName);
            LOG_WRITE(EN_LOG_TRACE, "#### %s", acBuffer);
            rc = UTIL_System(acBuffer);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
                continue;
            }
        }

        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "tar jxvf %s -C /tmp", acFileName);
        LOG_WRITE(EN_LOG_TRACE, "#### %s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
            continue;
        }

        /* sleep(1);                                                            */

        /*
            NOTE:
            Find the production specific startup file first.
            Then determin to use general startup file or production specific startup file
         */
        memset(acTail, 0x00, sizeof(acTail));
        __ConvertTail(m_acModel, pstCfgStatic[i].pcYangName, acTail);
        snprintf(acBuffer, sizeof(acBuffer), "%s%s-startup-%s.xml",
                 pstCfgStatic[i].pcDefXpath,
                 pstCfgStatic[i].pcYangName,
                 acTail);

        if (access(acBuffer, F_OK) == -1) {
            LOG_WRITE(EN_LOG_NOTICE, "Cannot find %s, will load general startup file.", acBuffer);
            /* general startup file                                             */
            snprintf(acBuffer, sizeof(acBuffer), "cp -f %s%s.xml %s%s-startup.xml",
                     DEF_CFG_TMP_PATH,
                     pstCfgStatic[i].pcYangName,
                     pstCfgStatic[i].pcDefXpath,
                     pstCfgStatic[i].pcYangName);
        } else {
            /* production specific startup file                                 */
            snprintf(acBuffer, sizeof(acBuffer), "cp -f %s%s.xml %s%s-startup-%s.xml",
                     DEF_CFG_TMP_PATH,
                     pstCfgStatic[i].pcYangName,
                     pstCfgStatic[i].pcDefXpath,
                     pstCfgStatic[i].pcYangName,
                     acTail);
        }

        LOG_WRITE(EN_LOG_TRACE, "#### %s", acBuffer);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
            continue;
        }

    }

    /* If restore flag is false, delete switch data.                            */
    if (false == bFlag) {
        memset(acBuffer, 0, sizeof(acBuffer));
        snprintf(acBuffer, sizeof(acBuffer), "rm -f %s*.db", DB_RESTORE_DIR);
        rc = UTIL_System(acBuffer);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);
            goto cleanup;
        }
        LOG_WRITE(EN_LOG_NOTICE, "dbloader del db file when it is used completed at switch mode!");
    }

    /* Load config by install.sh yang                                           */
    rc = __LoadCfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__LoadCfg failed");
        goto cleanup;
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __DefCfg                                                                              */
/* Descrp : Using EEPROM mfg data to update com-oplink-device data. If failure, using default.    */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__DefCfg()
{
    int rc = OPLK_OK;

    /* Unzip yang-model.tgz file.                                               */
    printf("#### %s \n", DEF_UNZIP_DEF_YANG);
    rc = UTIL_System(DEF_UNZIP_DEF_YANG);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "UNZIP_DEF_YANG failed");
        /* goto cleanup;                                                        */
    }

    /* Rostore mfg from CHASSIS/RPC EEPROM and update  com-oplink-device-startup-[Model].xml*/
    rc = __RestoreMfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__RestoreMfg failed");
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "__RestoreMfg %s, Model=%s", (0 == rc) ? "ok" : "ng", m_acModel);

    return rc;

cleanup:
    /* If failure, using default value                                          */
    rc = UTIL_System(DEF_UNZIP_DEF_YANG);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "UNZIP_DEF_YANG failed");
    }
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __UseDefCfg                                                                           */
/* Descrp : Using config file from EEPROM, if succeed, save to mSata.                             */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__UseDefCfg()
{
    int rc = OPLK_OK;

    /* unzip                                                                    */
    rc = __DefCfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __DefCfg");
        goto cleanup;
    }

    rc = __LoadCfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__LoadCfg failed");
        goto cleanup;
    }

    rc = __SaveCfg(DEF_SAVE_PRIMARY, DEF_CFG_RUN_PATH);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__SaveCfg DEF_SAVE_PRIMARY failed");
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DBLOADER_RestoreCfg                                                                   */
/* Descrp : Restore cfg by id                                                                     */
/* Input  : bFlag -- flag of restore data from EEPROM                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
int
DBLOADER_RestoreCfg(bool bFlag)
{
    int rc = OPLK_OK;
    LOG_WRITE(EN_LOG_NOTICE, "++ DBLOADER_RestoreCfg");

    rc = __DefCfg();
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "__DefCfg failed");
        goto cleanup;
    }

    /* Restore from bank to /tmp/etc/sysrepo/data                               */
    rc = __RestoreCfgById(bFlag);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by __RestoreCfgById");

        if (true == bFlag) {
            LOG_WRITE(EN_LOG_ERROR, "dbloader start failed at normal mode!");
            /* use default xml                                                  */
            rc = __UseDefCfg();
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "__UseDefCfg failed");
                goto cleanup;
            }
        }
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Use main config");
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "-- DBLOADER_RestoreCfg");
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : DBLOADER_RestoreLog                                                                   */
/* Descrp : Restore log information.                                                              */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/***FUNC-******************************************************************************************/
int
DBLOADER_RestoreLog()
{
    LOG_WRITE(EN_LOG_NOTICE, "++ DBLOADER_RestoreLog");
    LOG_WRITE(EN_LOG_NOTICE, "-- DBLOADER_RestoreLog");
    return OPLK_OK;
}

/***FUNC+******************************************************************************************/
/* Name   : __Usage                                                                               */
/* Descrp : Print usage information.                                                              */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg NONE :                                                                       */
/***FUNC-******************************************************************************************/
static void
__Usage()
{
    printf("\nUsage:\n  dbloader <operation-option> [other-options]\n\n");
    printf("operation-options:\n");
    printf("  r,      Read config from EEprom\n");
    printf("  d,      Restore default config\n");
    printf("  w 0,    Write config to main EEprom\n");
    printf("  w 1,    Write config to backup EEprom\n");
    printf("\n");
}

/***FUNC+******************************************************************************************/
/* Name   : __TestLib                                                                             */
/* Descrp : Test lib, a debug function.                                                           */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg NONE :                                                                       */
/***FUNC-******************************************************************************************/
static void
__TestLib()
{
    ST_PERSIST *pstCfgStatic = NULL;
    size_t nCnt = 0;

    pstCfgStatic = PERSIST_GetCfgStaticData(&nCnt);
    if (NULL == pstCfgStatic) {
        return;
    }

    for (int i = 0; i < nCnt; i++) {
        printf("%d, %s \n", PERSIST_GetNeedCfg(i), pstCfgStatic[i].pcDefXpath);
    }

    printf("nCnt = %ld\n", nCnt);
    PERSIST_SetNeedCfg(0, 2);
    PERSIST_SetNeedCfg(1, 3);

    for (int i = 0; i < nCnt; i++) {
        printf("%d, %s \n", PERSIST_GetNeedCfg(i), pstCfgStatic[i].pcDefXpath);
    }
}

/***FUNC+******************************************************************************************/
/* Name   : DBLOADER_SetMfgDefault                                                                */
/* Descrp : Set mfg default                                                                       */
/* Input  : NONE                                                                                  */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
DBLOADER_SetMfgDefault()
{
    int rc = OPLK_OK;
    ST_PERSIST *pstCfgStatic = NULL;
    size_t nCnt = 0;
    char acBuffer[DEF_BUF_SIZE_128] = {0};

    const char *pcDefCfgFile = "/tmp/DefCfgFile";

    pstCfgStatic = PERSIST_GetCfgStaticData(&nCnt);
    if (NULL == pstCfgStatic) {
        LOG_WRITE(EN_LOG_ERROR, "pstCfgStatic null");
        rc = OPLK_ERR;
        goto cleanup;
    }

    /* Just write 'aaaa' to a tar file. This file is invalid .                  */
    memset(acBuffer, 0, sizeof(acBuffer));
    sprintf(acBuffer, "echo aaaa > %s", pcDefCfgFile);
    rc = UTIL_System(acBuffer);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by system");
        goto cleanup;
    }

    for (int i = 0; i < nCnt; i++) {
        rc |= BOARD_SetChassisCfg(pstCfgStatic[i].iAddrSlice, pstCfgStatic[i].iSize, pcDefCfgFile);
    }

    /* Delete the tmp file.                                                     */
    memset(acBuffer, 0, sizeof(acBuffer));
    sprintf(acBuffer, "rm -f %s", pcDefCfgFile);
    rc = UTIL_System(acBuffer);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by system");
        goto cleanup;
    }

cleanup:
    return rc;
}

/***FUNC+******************************************************************************************/
/* Name   : __CopyStartupDbToRunningDb                                                            */
/* Descrp : Copy startup db to running db                                                         */
/* Input  : bFlag -- flag of restore data from EEPROM                                             */
/* Output : NONE                                                                                  */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR :                                                                   */
/***FUNC-******************************************************************************************/
static int
__CopyStartupDbToRunningDb()
{
    int rc = SR_ERR_OK;
    sr_conn_ctx_t *pstConn = NULL;
    sr_session_ctx_t *pstSess = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);
    /* turn logging on                                                          */
    sr_log_stderr(SR_LL_WRN);

    /* connect to sysrepo                                                       */
    rc = sr_connect(0, &pstConn);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_connect: %s", sr_strerror(rc));
        goto cleanup;
    }

    /* start session                                                            */
    rc = sr_session_start(pstConn, SR_DS_RUNNING, &pstSess);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_session_start: %s", sr_strerror(rc));
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "sr_copy_config begin");
    rc = sr_copy_config(pstSess, NULL, SR_DS_STARTUP, 0);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_copy_config: %s", sr_strerror(rc));
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "sr_copy_config over");

cleanup:
    sr_disconnect(pstConn);
    LOG_WRITE(EN_LOG_NOTICE, "%s --, rc: %d", __func__, rc);

    return rc;
}

int
main(int argc, char **argv)
{
    int rc = OPLK_OK;
    DB_STATE_ST stDbState;

    rc = UTIL_InitClientLog();
    if (rc != OPLK_OK) {
        printf("Init client error\n");
    }

    /* chuny This watchdog time tbd                                             */
    BOARD_CfgWatchDogTime(true, DEF_WDT_TIMEOUT_7MIN);

    LOG_WRITE(EN_LOG_NOTICE, "DBloader start argc=%d, argv[1]=%s", argc, argv[1]);
    printf("argc=%d, argv[0]=%s, argv[1]=%s\n", argc, argv[0], argv[1]);
    if (argc < 2) {
        __Usage();
        LOG_WRITE(EN_LOG_ERROR, "arg error");
        exit(OPLK_ERR);
    }

    switch (argv[1][0]) {
    case 't':
        __TestLib();
        break;
    case 'r':
        /* Restore config                                                       */
        /* Install yang                                                         */
        memset(&stDbState, 0, sizeof(stDbState));
        FILE_ReadDbState(&stDbState);
        if (stDbState.ucDbRestoreState != DB_RESTORE_SWITCH) {
            LOG_WRITE(EN_LOG_NOTICE, "dbloader start at normal mode!");
            rc = DBLOADER_RestoreCfg(true);
            if (rc != 0) {
                LOG_WRITE(EN_LOG_ERROR, "DBLOADER_RestoreCfg failed");
                goto cleanup;
            }
        } else {
            FILE_WriteDbState(stDbState.ulRollBackTimer, DB_RESTORE_PILOT);
            LOG_WRITE(EN_LOG_NOTICE, "dbloader start at switch mode!");

            rc = DBLOADER_RestoreCfg(false);
            if (rc != 0) {
                FILE_WriteDbState(stDbState.ulRollBackTimer, DB_RESTORE_COMMIT);
                LOG_WRITE(EN_LOG_ERROR, "dbloader start failed at switch mode!");
                rc = DBLOADER_RestoreCfg(true);
                if (rc != 0) {
                    LOG_WRITE(EN_LOG_ERROR, "DBLOADER_RestoreCfg failed");
                    goto cleanup;
                }
            }
        }
        printf("DBLOADER_RestoreCfg ok\n");

        /* Restore log                                                          */
        rc = DBLOADER_RestoreLog();
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "DBLOADER_RestoreLog failed");
            goto cleanup;
        }
        printf("DBLOADER_RestoreLog ok\n");

        /* Copy startup to running                                              */
        rc = __CopyStartupDbToRunningDb();
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "__CopyStartupDbToRunningDb failed");
            goto cleanup;
        }
        break;
    case 'd':
        rc = DBLOADER_SetMfgDefault();
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "DBLOADER_SetMfgDefault failed");
            goto cleanup;
        }
        printf("DBLOADER_SetMfgDefault ok\n");
        break;
    case 'w':
        if (argc < 3) {
            __Usage();
            LOG_WRITE(EN_LOG_ERROR, "arg error");
            goto cleanup;
        }

        if ('1' == argv[2][0]) {
            rc = __SaveCfg(DEF_SAVE_BACKUP, DEF_CFG_RUN_PATH);
            if (rc != 0) {
                LOG_WRITE(EN_LOG_ERROR, "__SaveCfg DEF_SAVE_BACKUP failed");
                goto cleanup;
            }
            printf("__SaveCfg DEF_SAVE_BACKUP ok\n");
        } else {
            rc = __SaveCfg(DEF_SAVE_PRIMARY, DEF_CFG_RUN_PATH);
            if (rc != 0) {
                LOG_WRITE(EN_LOG_ERROR, "__SaveCfg DEF_SAVE_PRIMARY failed");
                goto cleanup;
            }
            printf("__SaveCfg DEF_SAVE_PRIMARY ok\n");
        }
        break;
    case 'c':
        /* Copy startup to running                                              */
        rc = __CopyStartupDbToRunningDb();
        if (rc != 0) {
            LOG_WRITE(EN_LOG_ERROR, "__CopyStartupDbToRunningDb failed");
            goto cleanup;
        }
        break;
    default:
        __Usage();
        LOG_WRITE(EN_LOG_ERROR, "arg error");
        goto cleanup;
    }

    UTIL_CloseClientLog();
    exit(OPLK_OK);
cleanup:
    UTIL_CloseClientLog();
    exit(OPLK_ERR);
}

